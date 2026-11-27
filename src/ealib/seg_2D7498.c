#include "common.h"
#include "ealib/locale.h"

/* EA's speech driver (names ours, from use: the library's own names are not known). Its banks and tables are
   little-endian blobs that the code reads byte by byte, except for the halfword fields the headers below name. */

/* The marker D_004A4820 holds while the driver is initialised (name ours): [U] SPCH_initDriver stores it last; the
   shutdown, the hook installers and the per-frame update test it. 0x01789A34 is 24681012 in decimal. */
#define SPEECH_INITIALISED_MARKER 24681012

/* A speech key (SPCH_makeKey packs one): a bank entry's id and the ids of the bank it belongs to. */
typedef struct SPCHKEY {
    unsigned short entryId;         /* 0x0 the entry's id (its first halfword) */
    unsigned char subId;            /* 0x2 the bank's sub id (SPCHBANKHDR::subId) */
    unsigned char bankId;           /* 0x3 the bank's id (SPCHBANKHDR::bankId) */
} SPCHKEY;

/* A speech bank's fixed header (SPCHI_registerBank registers banks). entryCount halfword entry offsets follow it (entry i
   lies entryOffsets[i] * 4 bytes from the bank), padded to 4 bytes, then usageCount usage bytes (SPCHI_resetBankUsage). */
typedef struct SPCHBANKHDR {
    unsigned short magic;           /* 0x00 3075 (0xC03): SPCHI_registerBank registers only such data */
    unsigned char pad02[6];
    unsigned char bankId;           /* 0x08 SPCHI_getBankIds; a key's byte 3 */
    unsigned char subId;            /* 0x09 SPCHI_getBankIds; a key's byte 2 */
    unsigned char pad0A[6];
    unsigned short entryCount;      /* 0x10 the entry offsets' count */
    unsigned char usageCount;       /* 0x12 the usage bytes' count */
    unsigned char pad13;
    unsigned short value14;         /* 0x14 SPCHI_getBankLengthLimit returns it (its meaning is not known) */
    unsigned short minPriority;     /* 0x16 SPCHI_pickRequest: in channel mode 1 a request below it is held back */
    unsigned short entryOffsets[1]; /* 0x18 */
} SPCHBANKHDR;

/* One of the 8 bank slots D_0044FFB8 (SPCHI_registerBank fills them, SPCHI_clearBanks clears them). */
typedef struct SPCHBANKSLOT {
    unsigned char *data;            /* the bank (SPCHBANKHDR first), 0 when the slot is free */
    unsigned int key;               /* the key (channel) it was registered under */
} SPCHBANKSLOT;

/* An id list {count, ids} (SPCHI_clearIdList clears one; SPCHI_pickRequest / SPCHI_isInIdList search one). */
typedef struct SPCHIDLIST {
    int count;
    unsigned short *ids;
} SPCHIDLIST;

/* A bank entry: one speech event (the bank's entry offsets lead to it). variantCount halfword variant offsets follow
   the header (variant i lies variantOffsets[i] * 4 bytes from the entry), padded to 4 bytes; then tagCount 3-byte tag
   entries {tag id, argument index, type} and constraintCount 3-byte constraints, each list padded to 4 bytes. With
   flag 0x8 a further halfword table follows (SPCHI_pickNextRequest), and with flag 0x10 the follow-up list {count, ids}. */
typedef struct SPCHEVENT {
    unsigned short id;              /* 0x0 a key's entryId */
    unsigned short lifetime;        /* 0x2 ticks a queued request lives (0: no limit) */
    unsigned short priority;        /* 0x4 */
    unsigned char variantCount;     /* 0x6 */
    unsigned char counts;           /* 0x7 SPCH_TAG_COUNT / SPCH_CONSTRAINT_COUNT */
    signed char maxRepeat;          /* 0x8 plays in a row allowed (SPCHI_canRepeatEntry; 0 or less: no limit) */
    signed char mode;               /* 0x9 100: a newer request flags the channel's queue (SPCHI_pruneChannelQueue) */
    unsigned char flags;            /* 0xA 0x1 held below the bank's minimum, 0x4 never pruned, 0x8 / 0x10 above */
    unsigned char pad0B;
    unsigned short variantOffsets[1]; /* 0xC */
} SPCHEVENT;

/* SPCHEVENT::counts holds the tag count in its low nibble and the constraint count in its high one (signed). */
#define SPCH_TAG_COUNT(counts) ((counts) & 0xF)
#define SPCH_CONSTRAINT_COUNT(counts) ((signed char)(counts) >> 4)

/* A bank entry's tag entry (SPCHI_reportEventTags, SPCHI_askTagConditions) or constraint (SPCHI_checkConstraints). */
typedef struct SPCHTAG {
    unsigned char id;               /* the tag id */
    unsigned char argIndex;         /* the request argument it reads */
    unsigned char type;
} SPCHTAG;

/* A tag entry widened to words, the copy the tag readers make of one. */
typedef struct SPCHTAGWORDS {
    int id;
    int argIndex;
    int type;
} SPCHTAGWORDS;

/* A bank entry's follow-up list (flag 0x10): the ids that may follow it (SPCHI_pickNextRequest). */
typedef struct SPCHFOLLOWLIST {
    unsigned char count;
    unsigned char pad01;
    unsigned short ids[1];
} SPCHFOLLOWLIST;

/* A request's 20 argument words, as SPCH_postRequest posts them: word 0 is the request's key, words 1 to 19 its
   arguments (the tags' argument indices pick them). */
typedef union SPCHARGS {
    SPCHKEY key;
    int words[20];
} SPCHARGS;

/* A queued speech request, one of 16 in the request state D_0044FFF8 (from +0x64). A request can chain continuation
   records (next), which carry continuation != 0. */
typedef struct SPCHREQUEST {
    unsigned int time;              /* 0x00 the tick it was queued */
    unsigned short seq;             /* 0x04 its sequence number */
    unsigned char active;           /* 0x06 */
    unsigned char channel;          /* 0x07 */
    short continuation;             /* 0x08 */
    short next;                     /* 0x0A the next chained record, -1 for none */
    SPCHEVENT *event;               /* 0x0C the bank entry it plays */
    SPCHARGS args;                  /* 0x10 */
} SPCHREQUEST;

/* The request state D_0044FFF8 (SPCHI_resetRequests resets it). */
typedef struct SPCHREQSTATE {
    int queued[8];                  /* 0x000 the chain heads queued per channel */
    int head[8];                    /* 0x020 per channel: a request index, -1 for none (SPCH_playLatestRequest) */
    int latest[8];                  /* 0x040 per channel: the latest request's index, -1 for none */
    int newerFlagged;               /* 0x060 SPCHI_pruneChannelQueue: a newer mode-100 request waits on the channel */
    SPCHREQUEST requests[16];       /* 0x064 */
} SPCHREQSTATE;

/* A speech channel's current line, D_00450660 [8] (SPCHI_resetChannelLines resets it: entry id 0xFFFF, count 0). */
typedef struct SPCHCHANNEL {
    SPCHKEY line;                   /* 0x0 the line playing (entryId 0xFFFF: none) */
    int repeatCount;                /* 0x4 how many times in a row it played (SPCHI_playList; SPCHI_canRepeatEntry limits it) */
} SPCHCHANNEL;

/* A speech table row as SPCHI_readRow reads it. */
struct sShapeInfo { int len; int start; int n; unsigned char col[4]; };
/* A phrase slot's selection: its table slot, candidates (start, len in the set's row list) and chosen candidate. */
struct sSel { short x; short y; unsigned char start; unsigned char len; unsigned char cur; unsigned char flag; };
/* A phrase's selection: one sSel per slot, the slot count, and the shared candidate row list. */
struct sSelSet { struct sSel ent[12]; unsigned char n; unsigned char ntbl; unsigned char tbl[200]; };
struct sTblEnt { unsigned short a; unsigned short b; };
/* A phrase template (a bank entry's variant): +0x4 holds its kind (low 2 bits) and slot count (>> 2); the slot list
   (word offsets of the slots' conditions) starts at +0x8. */
/* numVars / varOffset (names ours): [U] SPCHI_checkVariables clears numVars variable words that start varOffset words after
   the slot list; SPCHI_findFittingCombination / SPCHI_chooseRows test numVars. */
struct sLayer {
    char pad[4];
    unsigned char cnt;
    unsigned char numVars;
    unsigned char varOffset;
    char pad7;
    unsigned char list[1];
};
/* A slot's condition: its table reference {id, argument index, kind} and up to 4 column rules. */
struct sCond {
    unsigned short id; unsigned char idx; unsigned char mode; signed char n; char pad5[3];
    unsigned char a[8]; unsigned char b[4]; unsigned int m[4];
};

/* A speech table's header (the tables in the slot list D_004A4824; SPCH_addTable adds them). */
typedef struct SPCHTABLEHDR {
    unsigned short id;              /* 0x0 SPCHI_findTableById / SPCHI_findTableBySubId match it */
    unsigned short subId;           /* 0x2 SPCHI_findTableBySubId */
    unsigned char flags;            /* 0x4 low 4 bits: the column count; 0x80: a random-subset table */
    unsigned char rowCount;         /* 0x5 */
    unsigned char historyLength;    /* 0x6 */
    unsigned char startScale;       /* 0x7 row starts are in units of (startScale + 1) * 256 (SPCHI_readRow) */
    unsigned short totalLength;     /* 0x8 the samples' total length, in those units */
    unsigned short entryCount;      /* 0xA SPCHI_isTableEntryInRange's range (0xFFFF: no limit) */
} SPCHTABLEHDR;

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_registerBank);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_6498 { unsigned char *p; unsigned int x; };
extern struct E_6498 D_0044FFB8_6498[] __asm__("D_0044FFB8");
void SPCHI_getBankIds(void *arg0, int *arg1, int *arg2);

static __inline__ void clear_6498(unsigned long key) {
    int i, j;
    for (i = 0; i < 8; i++) {
        unsigned char *p = D_0044FFB8_6498[i].p;
        if (p != 0 && D_0044FFB8_6498[i].x == key) {
            unsigned char *q = p + (((*(unsigned short *)(p + 0x10) * 2 + 3) & 0xFFFFFFFC) + 0x18);
            for (j = 0; j < D_0044FFB8_6498[i].p[0x12]; j++) {
                q[j] = 0xFF;
            }
        }
    }
}

int SPCHI_registerBank_6498(unsigned short *arg0, unsigned int arg1) __asm__("SPCHI_registerBank");
/**
 * Registers a speech bank in the first free of the 8 bank slots (D_0044FFB8) under a key, unless one with the same ids
 * is already registered.
 *
 * Only data whose first halfword is 3075 (0xC03) is taken. Its two ids (bytes +0x8 and +0x9, read by SPCHI_getBankIds)
 * must not match a registered bank's. The slot gets the data and @p bankKey; then every bank registered under that key has
 * its usage bytes reset to 0xFF (the inlined SPCHI_resetBankUsage), even when no slot was free.
 *
 * @param bankData  the speech bank data
 * @param bankKey  the key (channel) it is registered under
 * @return 1 when registered, 0 otherwise
 */
int SPCHI_registerBank_6498(unsigned short *bankData, unsigned int bankKey) {
    int bankId, subId;
    int i;
    int ok = 0;
    if (*bankData == 3075) {
        SPCHI_getBankIds(bankData, &bankId, &subId);
        for (i = 0; i < 8; i++) {
            unsigned char *slotData = D_0044FFB8_6498[i].p;
            if (slotData != 0 && slotData[9] == subId && slotData[8] == bankId) {
                goto out;
            }
        }
        for (i = 0; i < 8; i++) {
            if (D_0044FFB8_6498[i].p == 0) {
                D_0044FFB8_6498[i].p = (unsigned char *)bankData;
                D_0044FFB8_6498[i].x = bankKey;
                ok = 1;
                break;
            }
        }
        clear_6498(bankKey);
    }
out:
    return ok;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_resetBankUsage);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHBANKSLOT D_0044FFB8_6618[] __asm__("D_0044FFB8");

/**
 * Resets the usage bytes of every registered speech bank with a key: sets the +0x12 bytes that follow the bank's header
 * and its +0x10 halfword table (at ((+0x10 * 2 + 3) & ~3) + 0x18) to 0xFF.
 *
 * @param key  the key the banks were registered under
 */
void SPCHI_resetBankUsage(unsigned long key) {
    int i, j;
    for (i = 0; i < 8; i++) {
        unsigned char *bankData = D_0044FFB8_6618[i].data;
        if (bankData != 0 && D_0044FFB8_6618[i].key == key) {
            unsigned char *usage = bankData + (((((SPCHBANKHDR *)bankData)->entryCount * 2 + 3) & 0xFFFFFFFC) + 0x18);
            for (j = 0; j < D_0044FFB8_6618[i].data[0x12]; j++) {
                usage[j] = 0xFF;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_registerBank);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int SPCHI_registerBank_66C0(void *arg0, unsigned int arg1) __asm__("SPCHI_registerBank");

/**
 * Registers a speech bank under key @p key (SPCHI_registerBank) when the key is below 8.
 *
 * @param data  the speech bank data
 * @param key  the key, 0..7
 * @return 1 when registered, 0 otherwise
 */
int SPCH_registerBank(void *data, unsigned int key) {
    int registered = 0;
    if (key < 8) {
        registered = SPCHI_registerBank_66C0(data, key);
    }
    return registered;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_unregisterBank);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFB8_FFB8[] __asm__("D_0044FFB8");

/**
 * Unregisters the speech bank registered with this data and key: clears its slot in D_0044FFB8.
 *
 * @param bankData  the speech bank data (0 does nothing)
 * @param bankKey  the key it was registered under
 * @return 1 when found, 0 otherwise
 */
int SPCH_unregisterBank(int bankData, int bankKey) {
    int i;
    if (bankData == 0) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (D_0044FFB8_FFB8[i * 2] == bankData && D_0044FFB8_FFB8[i * 2 + 1] == bankKey) {
            D_0044FFB8_FFB8[i * 2] = 0;
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCH_registerBankKey0);
#ifdef SKIP_ASM
extern void SPCHI_registerBank(int, int);

/**
 * Registers a speech bank under key 0 (SPCHI_registerBank) and discards the result.
 *
 * @param data  the speech bank data
 */
void SPCH_registerBankKey0(int data) {
    SPCHI_registerBank(data, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_getCallbackBlock);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFA8_6768[4] __asm__("D_0044FFA8");

/**
 * Returns the speech driver's callback block D_0044FFA8 (+0x8 is the time source; SPCH_Init stores TIMER_gettick
 * there).
 *
 * @return D_0044FFA8
 */
int *SPCH_getCallbackBlock(void) {
    return D_0044FFA8_6768;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_getBufferSize);
#ifdef SKIP_ASM
/**
 * Returns a speech buffer size in bytes: @p sampleRate * @p bitsPerSample / 8 bytes per second, then / 10 for mode 1 or * 2 / 7 for
 * mode 2 (SPCH_Init asks for 22050 Hz, 16 bits, mode 1).
 *
 * @param sampleRate  the sample rate
 * @param bitsPerSample  the bits per sample
 * @param mode  0 one second, 1 a tenth, 2 two sevenths
 * @return the size in bytes
 */
int SPCH_getBufferSize(int sampleRate, int bitsPerSample, unsigned int mode) {
    int result = sampleRate * bitsPerSample / 8;
    switch (mode) {
    case 0:
        break;
    case 1:
        result = result / 10;
        break;
    case 2:
        result = result * 2 / 7;
        break;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_shutdown);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_67F8 { int a; int b; int c; };
extern struct S_67F8 D_0044FF90_67F8 __asm__("D_0044FF90");
extern int D_004A4820;
void SPCHI_clearTableList(void);
void SPCHI_clearBanks(void);

/**
 * Shuts the speech driver down when it is initialised (D_004A4820 == 0x01789A34): clears the marker and the callback
 * block D_0044FF90, then SPCHI_clearTableList and SPCHI_clearBanks (clears the bank slots).
 */
void SPCH_shutdown(void) {
    if (D_004A4820 == SPEECH_INITIALISED_MARKER) {
        D_004A4820 = 0;
        D_0044FF90_67F8.a = 0;
        D_0044FF90_67F8.b = 0;
        D_0044FF90_67F8.c = 0;
        SPCHI_clearTableList();
        SPCHI_clearBanks();
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_initDriver);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_6840 { int a; int b; int c; };
struct E_6840 { unsigned short h; int w; };
extern struct S_6840 D_0044FF90_6840 __asm__("D_0044FF90");
extern struct S_6840 D_0044FFA8_6840 __asm__("D_0044FFA8");
extern struct E_6840 D_00450660_6840[] __asm__("D_00450660");
extern int D_004506A0_6840[] __asm__("D_004506A0");
extern int D_004A4820;
extern void *D_004A482C_6840 __asm__("D_004A482C");
extern int D_004A4834_6840 __asm__("D_004A4834");
extern int D_004A4838_6840 __asm__("D_004A4838");
void SPCH_postRequest(int, int, ...);
void SPCHI_seedRandom(int);
void SPCHI_resetPlayList(void);
void SPCH_setClockOffset(int);
void SPCHI_clearBanks(void);
void SPCHI_resetTableList(void);
void SPCHI_resetRequests(void);

/**
 * Initialises the speech driver: installs the play callback and buffers, resets every table and sets the initialised
 * marker.
 *
 * D_0044FF90 gets @p playCallback as its first word (the rest cleared) and D_0044FFA8 is cleared; D_004A482C = SPCH_postRequest,
 * D_004A4834 = @p seed, D_004A4838 = @p workBuf. SPCHI_seedRandom seeds its tables with @p seed, then SPCHI_resetPlayList,
 * SPCH_setClockOffset(0), the 8 words at D_004506A0, SPCHI_clearBanks (bank slots), the 8 entries of D_00450660 (0xFFFF, 0),
 * SPCHI_resetTableList and SPCHI_resetRequests are reset. Finally D_004A4820 = 0x01789A34.
 *
 * @param playCallback  the play callback (SPCH_Init passes SPCH_StartSpeechCallback)
 * @param seed  the seed (SPCH_Init's "rNum")
 * @param workBuf  the work buffer from SPCH_getBufferSize
 * @return 1
 */
int SPCH_initDriver(int playCallback, int seed, int workBuf) {
    int i;
    long j;
    D_0044FF90_6840.a = playCallback;
    D_0044FF90_6840.b = 0;
    D_0044FF90_6840.c = 0;
    D_0044FFA8_6840.a = 0;
    D_0044FFA8_6840.b = 0;
    D_0044FFA8_6840.c = 0;
    D_004A482C_6840 = (void *)SPCH_postRequest;
    D_004A4834_6840 = seed;
    D_004A4838_6840 = workBuf;
    SPCHI_seedRandom(seed);
    SPCHI_resetPlayList();
    SPCH_setClockOffset(0);
    for (i = 7; i >= 0; i--) {
        D_004506A0_6840[i] = 0;
    }
    SPCHI_clearBanks();
    for (j = 0; j < 8; j++) {
        D_00450660_6840[(int)j].h = 0xFFFF;
        D_00450660_6840[(int)j].w = 0;
    }
    SPCHI_resetTableList();
    SPCHI_resetRequests();
    D_004A4820 = SPEECH_INITIALISED_MARKER;
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_setTagHooks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_6940 { int pad0; int a; int b; };
extern struct S_6940 D_0044FF90_6940 __asm__("D_0044FF90");
extern int D_004A4820;
/**
 * Installs the speech driver's two hooks, when it is initialised (D_004A4820 == 0x01789A34): D_0044FF90 +0x4, the
 * condition hook SPCHI_askTagConditions asks, and +0x8, the tag event hook SPCHI_reportEventTags and SPCHI_reportSlotTags call. No callers.
 *
 * @param x  the condition hook (+0x4)
 * @param y  the tag event hook (+0x8)
 */
void SPCH_setTagHooks(int x, int y) {
    if (D_004A4820 == SPEECH_INITIALISED_MARKER) {
        D_0044FF90_6940.a = x;
        D_0044FF90_6940.b = y;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_setRetryHook);
#ifdef SKIP_ASM
extern int D_0044FFA0[];
extern int D_004A4820;

/**
 * Installs the speech driver's retry hook D_0044FFA0 (D_0044FF90 +0x10, called by SPCHI_buildRequest when a request builds
 * nothing) when the driver is initialised. No callers.
 *
 * @param hookFunc  the hook
 */
void SPCH_setRetryHook(int hookFunc) {
    if (D_004A4820 == SPEECH_INITIALISED_MARKER) {
        D_0044FFA0[0] = hookFunc;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_setApproveHook);
#ifdef SKIP_ASM
extern int D_0044FF9C[];
extern int D_004A4820;

/**
 * Installs the speech driver's approve hook D_0044FF9C (D_0044FF90 +0xC, asked by SPCH_startNextRequest before a request plays)
 * when the driver is initialised. No callers.
 *
 * @param hookFunc  the hook
 */
void SPCH_setApproveHook(int hookFunc) {
    if (D_004A4820 == SPEECH_INITIALISED_MARKER) {
        D_0044FF9C[0] = hookFunc;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_resetChannelLines);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_69A8 { unsigned short h; int w; };
extern struct E_69A8 D_00450660_69A8[] __asm__("D_00450660");

/**
 * Resets the 8 speech channel entries of D_00450660: id 0xFFFF and word 0.
 */
void SPCHI_resetChannelLines(void) {
    long i;
    for (i = 0; i < 8; i++) {
        D_00450660_69A8[(int)i].h = 0xFFFF;
        D_00450660_69A8[(int)i].w = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_addTable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char **D_004A4824_69F0 __asm__("D_004A4824");
extern int D_004A4828_69F0 __asm__("D_004A4828");
extern unsigned int D_004A4834_69F0 __asm__("D_004A4834");

static inline int findfree_69F0(void) {
    int i, idx;

    idx = -1;
    for (i = 0; i < D_004A4828_69F0; i++) {
        if (D_004A4824_69F0[i] == 0) {
            idx = i;
            break;
        }
    }
    return idx;
}

static inline unsigned char *tail_69F0(unsigned char *s) {
    int n = s[4] & 0xF;
    return s + ((s[5] * (n + 2) + 0xF) & 0x3FFC) + n * 4;
}

/**
 * Adds a speech table (bank) to the driver's table list (D_004A4824, D_004A4828 slots); returns its slot or -1.
 *
 * A table with a history (byte +0x6) is refused while its history area is in use (position byte not 0 or an entry not
 * 0xFF). It goes into the first free slot; a table with the random-subset flag (0x80 in +0x4) first gets the rows of
 * subset (seed D_004A4834 % subset count) marked in its row bitmask (the inlined SPCHI_markRandomSubset). The table layout: +0x4
 * low nibble column count, +0x5 row count, rows of count + 2 bytes and 4 bytes per column, then the subset count and
 * bitmask, then the history.
 *
 * @param shp  the speech table
 * @return the slot, or -1 (no list, no free slot or history in use)
 */
int SPCH_addTable(unsigned char *shp) {
    int ret;
    int i, j, w, extra, subsetCount, cnt, bit;
    unsigned char *history, *subsetArea;
    unsigned int subsetIndex, start, end, byte;

    ret = -1;
    if (D_004A4824_69F0 != 0) {
        if (shp[6] != 0) {
            extra = 0;
            history = tail_69F0(shp);
            if (shp[4] & 0x80) {
                extra = (shp[5] + 7) / 8 + 1;
            }
            history += extra;
            if (*history != 0) {
                goto out;
            }
            for (i = 0; i < shp[6]; i++) {
                if (history[i + 1] != 0xFF) {
                    goto out;
                }
            }
        }
        ret = findfree_69F0();
        if (ret >= 0) {
            if (shp[4] & 0x80) {
                subsetArea = tail_69F0(shp);
                w = shp[5];
                subsetCount = *subsetArea;
                if (subsetCount > 0) {
                    subsetIndex = D_004A4834_69F0 % subsetCount;
                    start = subsetIndex * w / subsetCount;
                    end = (subsetIndex + 1) * w / subsetCount;
                    bit = start & 7;
                    byte = (start >> 3) + 1;
                    cnt = end - start;
                    for (j = 0; j < cnt; j++) {
                        subsetArea[byte] |= 1 << bit;
                        bit++;
                        if (bit == 8) {
                            bit = 0;
                            byte++;
                        }
                    }
                }
            }
            D_004A4824_69F0[ret] = shp;
        }
    }
out:
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_setSeed);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A4820;
extern unsigned char **D_004A4824_6BC0 __asm__("D_004A4824");
extern int D_004A4828_6BC0 __asm__("D_004A4828");
extern int D_004A4834_6BC0 __asm__("D_004A4834");
void SPCHI_clearRandomSubset(unsigned char *p);
void SPCHI_markRandomSubset(unsigned char *p);

/**
 * Sets the speech driver's seed (D_004A4834) and re-draws the random subset of every registered random-subset table.
 *
 * For tables with flag 0x80 (+0x4) SPCHI_clearRandomSubset clears the row bitmask and SPCHI_markRandomSubset marks the new subset. Does
 * nothing when the driver is not initialised (D_004A4820 == 0).
 *
 * @param seed  the new seed
 */
void SPCH_setSeed(int seed) {
    int i;
    unsigned char *table;
    if (D_004A4820 != 0) {
        D_004A4834_6BC0 = seed;
        if (D_004A4824_6BC0 != 0) {
            for (i = 0; i < D_004A4828_6BC0; i++) {
                table = D_004A4824_6BC0[i];
                if (table != 0 && (table[4] & 0x80)) {
                    SPCHI_clearRandomSubset(table);
                    SPCHI_markRandomSubset(table);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_getTableListSize);
#ifdef SKIP_ASM
/**
 * Returns the memory a speech table list of @p slotCount slots needs: 4 bytes each (SPCH_Init asks for 500).
 *
 * @param slotCount  the number of slots
 * @return the size in bytes
 */
int SPCH_getTableListSize(int slotCount) {
    return slotCount * 4;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_initTableList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A4824_6C60 __asm__("D_004A4824");
extern int D_004A4828_6C60 __asm__("D_004A4828");

/**
 * Sets up the speech table list: D_004A4828 = @p slotCount slots at @p slotMem (D_004A4824), all cleared.
 *
 * @param slotCount  the number of slots
 * @param slotMem  the slot memory (SPCH_getTableListSize bytes), or 0
 */
void SPCH_initTableList(int slotCount, int *slotMem) {
    int i;

    D_004A4828_6C60 = slotCount;
    D_004A4824_6C60 = slotMem;
    if (slotMem != 0) {
        for (i = 0; i < D_004A4828_6C60; i++) {
            D_004A4824_6C60[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_removeTable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A4824_6CA0 __asm__("D_004A4824");

/**
 * Clears slot @p slot of the speech table list (the cSpeechBank destructor's release). No range check.
 *
 * @param slot  the slot
 */
void SPCH_removeTable(int slot) {
    if (D_004A4824_6CA0 != 0) {
        D_004A4824_6CA0[slot] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_getTable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A4820;
extern int *D_004A4824_6CC0 __asm__("D_004A4824");

/**
 * Returns the speech table in slot @p slot, or 0 when the driver is not initialised or has no list. No range check.
 *
 * @param slot  the slot
 * @return the table, or 0
 */
int SPCH_getTable(int slot) {
    int table = 0;
    if (D_004A4820 == SPEECH_INITIALISED_MARKER && D_004A4824_6CC0 != 0) {
        table = D_004A4824_6CC0[slot];
    }
    return table;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_isTableEntryInRange);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHTABLEHDR **D_004A4824_6CF8 __asm__("D_004A4824");

/**
 * Returns whether @p entryIndex is below the entry count (halfword +0xA) of the speech table in slot @p slot; 0 when the
 * count is 0xFFFF, the slot is negative or there is no list.
 *
 * @param slot  the slot
 * @param entryIndex  the entry index to check
 * @return 1 when in range, else 0
 */
int SPCHI_isTableEntryInRange(int slot, unsigned int entryIndex) {
    int inRange = 0;
    unsigned short *countPtr;

    if (D_004A4824_6CF8 != 0 && slot >= 0) {
        countPtr = &D_004A4824_6CF8[slot]->entryCount;
        if (*countPtr != 0xFFFF) {
            inRange = entryIndex < *countPtr;
        }
    }
    return inRange;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_clearTableList);
#ifdef SKIP_ASM
extern int D_004A4824;
extern int D_004A4828;

/**
 * Forgets the speech table list (D_004A4824 and its count D_004A4828 cleared). Used by SPCH_shutdown at shutdown.
 */
void SPCHI_clearTableList(void) {
    D_004A4824 = 0;
    D_004A4828 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_resetTableList);
#ifdef SKIP_ASM
extern int D_004A4824;
extern int D_004A4828;

/**
 * Forgets the speech table list (D_004A4824 and its count D_004A4828 cleared); the same body as SPCHI_clearTableList, used by
 * SPCH_initDriver at init.
 */
void SPCHI_resetTableList(void) {
    D_004A4824 = 0;
    D_004A4828 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_findTableById);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short **D_004A4824_6D50 __asm__("D_004A4824");
extern int D_004A4828_6D50 __asm__("D_004A4828");

/**
 * Returns the slot of the first registered speech table whose id (first halfword) is @p tableId, or -1.
 *
 * @param tableId  the table id
 * @return the slot, or -1
 */
int SPCHI_findTableById(unsigned short tableId) {
    unsigned short **slotPtr;
    short i;

    slotPtr = D_004A4824_6D50;
    if (slotPtr != 0) {
        for (i = 0; i < D_004A4828_6D50; i++) {
            if (*slotPtr != 0 && **slotPtr == tableId) {
                return i;
            }
            slotPtr++;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_findTableBySubId);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short **D_004A4824_6DB8 __asm__("D_004A4824");
extern int D_004A4828_6DB8 __asm__("D_004A4828");

/**
 * Returns the slot of the registered speech table whose two first halfwords are @p tableId and @p subId, or -1.
 *
 * @param tableId  the table id
 * @param subId  the second halfword (a sub id)
 * @return the slot, or -1
 */
int SPCHI_findTableBySubId(unsigned short tableId, unsigned short subId) {
    short i;
    unsigned short *table;

    if (D_004A4824_6DB8 != 0) {
        for (i = 0; i < D_004A4828_6DB8; i++) {
            table = D_004A4824_6DB8[i];
            if (table != 0 && table[0] == tableId && table[1] == subId) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getHistoryRecency);
#ifdef SKIP_ASM
/**
 * Returns how recently a speech table picked @p key: 1 for the latest entry of its history ring, 2 the one before, ...;
 * 0 when it is not in the history or the table keeps none (byte +0x6).
 *
 * @param node  the speech table
 * @param key  the row it picked
 * @return the recency, 0 when absent
 */
int SPCHI_getHistoryRecency(unsigned char *node, int key) {
    int cnt = node[6];
    int recency = 0;
    if (cnt > 0) {
        unsigned char flags = node[4];
        int off = 0;
        unsigned char w = node[5];
        int colCount = flags & 0xF;
        unsigned char *rowsEnd, *history;
        int i, j;
        rowsEnd = node + ((w * (colCount + 2) + 0xF) & 0x3FFC) + colCount * 4;
        if (flags & 0x80)
            off = (w + 7) / 8 + 1;
        history = rowsEnd + off;
        i = 0;
        j = *history;
        history++;
        j -= 1;
        do {
            if (j <= -1)
                j = cnt - 1;
            if (history[j] == key) {
                recency = i + 1;
                break;
            }
            i += 1;
            j -= 1;
        } while (i < cnt);
    }
    return recency;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_recordHistory);
#ifdef SKIP_ASM
/**
 * Records @p key in a speech table's history ring (length byte +0x6; position byte, then the entries) and advances the
 * position, wrapping at the length. Tables without a history are left alone.
 *
 * @param node  the speech table
 * @param key  the row just picked
 */
void SPCHI_recordHistory(unsigned char *node, int key) {
    int colCount;
    int maskBytes;
    unsigned char *rowsEnd;
    unsigned char *history;
    unsigned char flags;
    unsigned char rowCount;
    unsigned char histLen;
    unsigned char pos;

    histLen = node[6];
    maskBytes = 0;
    if (histLen != 0) {
        flags = node[4];
        rowCount = node[5];
        colCount = flags & 0xF;
        rowsEnd = node + (((rowCount * (colCount + 2)) + 0xF) & 0x3FFC) + colCount * 4;
        if (flags & 0x80) {
            maskBytes = (rowCount + 7) / 8 + 1;
        }
        history = rowsEnd + maskBytes;
        pos = *history;
        if (pos < histLen) {
            history[pos + 1] = key;
        }
        pos++;
        if (pos >= node[6]) pos = 0;
        *history = pos;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_markRandomSubset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_004A4834_6F80 __asm__("D_004A4834");

/**
 * Marks a random-subset speech table's current subset in its row bitmask.
 *
 * With c the subset count (first byte after the rows), subset r = seed D_004A4834 % c covers rows r * rows / c up to (r
 * + 1) * rows / c, whose bits are set. Nothing when c is 0.
 *
 * @param shp  the speech table
 */
void SPCHI_markRandomSubset(unsigned char *shp) {
    int j, w, subsetCount, cnt, bit;
    unsigned char *subsetArea;
    unsigned int subsetIndex, start, end, byte;
    int n = shp[4] & 0xF;

    subsetArea = shp + ((shp[5] * (n + 2) + 0xF) & 0x3FFC) + n * 4;
    w = shp[5];
    subsetCount = *subsetArea;
    if (subsetCount > 0) {
        subsetIndex = D_004A4834_6F80 % subsetCount;
        start = subsetIndex * w / subsetCount;
        end = (subsetIndex + 1) * w / subsetCount;
        bit = start & 7;
        byte = (start >> 3) + 1;
        cnt = end - start;
        for (j = 0; j < cnt; j++) {
            subsetArea[byte] |= 1 << bit;
            bit++;
            if (bit == 8) {
                bit = 0;
                byte++;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_clearRandomSubset);
#ifdef SKIP_ASM
/**
 * Clears a random-subset speech table's row bitmask, when its subset count is not 0.
 *
 * @param node  the speech table
 */
void SPCHI_clearRandomSubset(unsigned char *node) {
    int n = node[4] & 0xF;
    int rowCount = node[5];
    unsigned char *rowsEnd = node + ((rowCount * (n + 2) + 0xF) & 0x3FFC) + n * 4;
    if (*rowsEnd != 0) {
        int cnt = (rowCount + 7) / 8;
        if (cnt != 0) {
            int i = 1;
            do {
                rowsEnd[i] = 0;
                i++;
            } while (i <= cnt);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_findFreeTableSlot);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A4824_4824 __asm__("D_004A4824");
extern int D_004A4828_4828 __asm__("D_004A4828");

/**
 * Returns the first free slot of the speech table list, or -1.
 *
 * @return the slot, or -1
 */
int SPCHI_findFreeTableSlot(void) {
    int i;
    int freeSlot = -1;

    for (i = 0; i < D_004A4828_4828; i++) {
        if (D_004A4824_4824[i] == 0) {
            freeSlot = i;
            break;
        }
    }
    return freeSlot;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_findBankEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHBANKSLOT D_0044FFB8_7110[] __asm__("D_0044FFB8");

static __inline__ unsigned short *lookup_7110(unsigned char *base, unsigned short id) {
    unsigned short n = *(unsigned short *)(base + 0x10);
    int j;
    for (j = 0; j < n; j++) {
        unsigned short *r = (unsigned short *)(base + ((unsigned short *)(base + 0x18))[j] * 4);
        if (*r == id) {
            return r;
        }
    }
    return 0;
}

/**
 * Finds a speech event's entry in the speech bank registered for its ids: returns the entry or 0.
 *
 * @p eventKey holds the entry id (halfword +0x0) and the bank ids (+0x2 and +0x3, compared with bank bytes +0x9 and +0x8).
 * The first of the 8 bank slots (D_0044FFB8) that matches is searched: its entries (count +0x10, word offsets at +0x18)
 * are compared on their first halfword.
 *
 * @param eventKey  the 4-byte key {entry id, bank ids}
 * @return the bank entry, or 0
 */
unsigned short *SPCHI_findBankEntry(SPCHKEY *eventKey) {
    SPCHBANKSLOT *res;
    SPCHBANKSLOT *bankSlot = D_0044FFB8_7110;
    unsigned short *ret = 0;
    SPCHBANKSLOT **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *bankData = bankSlot->data;
        if (bankData != 0 && bankData[9] == eventKey->subId && bankData[8] == eventKey->bankId) {
            *pres = bankSlot;
            found = 1;
            goto done;
        }
        i++;
        bankSlot++;
    } while (i < 8);
done:
    if (found) {
        ret = lookup_7110(res->data, eventKey->entryId);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_resetRequests);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct R_71D8 { int time; short h; char c; unsigned char d; int pad8; int fc; int arr[20]; };
struct G_71D8 { int z[8]; int a[8]; int b[8]; int n; struct R_71D8 r[16]; };
extern struct G_71D8 D_0044FFF8_71D8 __asm__("D_0044FFF8");
extern int D_004A4840;
extern short D_004A4844;

/**
 * Resets the speech driver's request state (D_0044FFF8): the 8 channel words (0, -1, -1), the count, and 16 request
 * records (cleared, +0x7 = 0xFF, 20 data words zeroed); D_004A4840 and D_004A4844 are cleared too.
 */
void SPCHI_resetRequests(void) {
    int i, j;
    for (i = 0; i < 8; i++) {
        D_0044FFF8_71D8.z[i] = 0;
        D_0044FFF8_71D8.a[i] = -1;
        D_0044FFF8_71D8.b[i] = -1;
    }
    D_0044FFF8_71D8.n = 0;
    for (i = 0; i < 16; i++) {
        D_0044FFF8_71D8.r[i].c = 0;
        D_0044FFF8_71D8.r[i].h = 0;
        D_0044FFF8_71D8.r[i].time = 0;
        D_0044FFF8_71D8.r[i].fc = 0;
        D_0044FFF8_71D8.r[i].d = 0xFF;
        for (j = 19; j >= 0; j--) {
            D_0044FFF8_71D8.r[i].arr[j] = 0;
        }
    }
    D_004A4840 = 0;
    D_004A4844 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_allocRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct H_72A0 { unsigned short h0; unsigned short lifetime; unsigned short priority; };   /* names ours, from use */
struct R_72A0 {
    unsigned int time;
    short h;
    unsigned char c;
    unsigned char d;
    short isContinuation;
    struct H_72A0 *p;
    int arr[20];
};
struct G_72A0 { int z[8]; int a[8]; int b[8]; int n; struct R_72A0 r[16]; };
extern struct G_72A0 D_0044FFF8_72A0 __asm__("D_0044FFF8");
int SPCHI_getClock(void);
void SPCHI_freeRequest(unsigned int);

/**
 * Finds a speech request record (D_0044FFF8, 16 of 0x60 bytes from +0x64) for a new request: returns its index or -1.
 *
 * Takes a free record first. Otherwise it evicts (SPCHI_freeRequest) a chain-head record whose lifetime (header +0x2, when
 * not 0) has passed, then any chain-head record on channel @p channel whose priority (header +0x4) is not above
 * @p priority, which is freed in place. With @p keepLatest set the channel's latest request (+0x40 [channel]) is never taken.
 *
 * @param priority  the new request's priority
 * @param channel  the channel
 * @param keepLatest  nonzero to keep the channel's latest request
 * @return the record index, or -1
 */
int SPCHI_allocRequest(unsigned int priority, int channel, int keepLatest) {
    int i;
    int ret = -1;
    int skip = -1;
    unsigned int now;
    int reqChannel;
    unsigned int lifetime;

    for (i = 0; i < 16; i++) {
        if (D_0044FFF8_72A0.r[i].c == 0) {
            ret = i;
            goto out;
        }
    }
    if (keepLatest != 0) {
        skip = D_0044FFF8_72A0.b[channel];
    }
    i = 0;
    now = SPCHI_getClock();
    for (; i < 16; i++) {
        if (D_0044FFF8_72A0.r[i].isContinuation == 0 && (lifetime = D_0044FFF8_72A0.r[i].p->lifetime) != 0 &&
            lifetime < now - D_0044FFF8_72A0.r[i].time && skip != i) {
            SPCHI_freeRequest(i);
            ret = i;
            goto out;
        }
    }
    for (i = 0; i < 16; i++) {
        if (D_0044FFF8_72A0.r[i].isContinuation == 0) {
            reqChannel = D_0044FFF8_72A0.r[i].d;
            if (priority >= D_0044FFF8_72A0.r[i].p->priority && reqChannel == channel && i != skip) {
                D_0044FFF8_72A0.r[i].c = 0;
                D_0044FFF8_72A0.z[reqChannel]--;
                ret = i;
                break;
            }
        }
    }
out:
    return ret;
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003D7418);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCH_postRequest);
#ifdef SKIP_ASM
int func_003D7418(unsigned char *arg0);

// PORT: SN-specific va_start (gcc 2.95 EE EABI form); use <stdarg.h> off-PS2.
#define SPCH_post_va_start(ap)                                           \
    (ap = (char *)__builtin_next_arg()                                   \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
#define SPCH_post_va_arg_int(ap) (*(int *)((ap += 8) - 8))

/**
 * Posts a speech request: copies the packed key @p speechKey (SPCH_makeKey) and @p n further int arguments into a 20-word
 * buffer and hands it to func_003D7418, the request queue. SPCH_initDriver installs it as D_004A482C, the game's speech
 * post function.
 *
 * @param speechKey  the packed speech key
 * @param n  the number of extra arguments
 */
void SPCH_postRequest(int speechKey, int n, ...) {
    int buf[20];
    int *cursor;
    char *ap;
    SPCH_post_va_start(ap);
    buf[0] = speechKey;
    if (n > 0) {
        cursor = &buf[1];
        do {
            *cursor++ = SPCH_post_va_arg_int(ap);
        } while (--n != 0);
    }
    func_003D7418((unsigned char *)buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_pickRequest);
#ifdef SKIP_ASM
// FAKEMATCH: `*&` read of the request's key (k = *&...args.key) forms its address first; plain form 97.60%.
// Byte-identical and behaves like the plain form; landed marked under the 2026-11-14 ruling (portable,
// behaviour-preserving steering only); replace it when an honest spelling matches.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHREQSTATE D_0044FFF8_7760 __asm__("D_0044FFF8");
extern SPCHBANKSLOT D_0044FFB8_7760[] __asm__("D_0044FFB8");
extern int D_004506A0_7760[] __asm__("D_004506A0");
extern int D_004A483C;
int SPCHI_getClock(void);
void SPCHI_freeRequest(unsigned int);

static __inline__ int find_7760(SPCHKEY *key, SPCHBANKSLOT **pres) {
    int found = 0;
    int i = 0;
    do {
        unsigned char *q = D_0044FFB8_7760[i].data;
        if (q != 0 && q[9] == key->subId && q[8] == key->bankId) {
            *pres = &D_0044FFB8_7760[i];
            found = 1;
            goto done;
        }
        i++;
    } while (i < 8);
done:
    return found;
}

/**
 * Picks the queued speech request to play next on a channel: returns its record index or -1.
 *
 * Looks at the chain-head records (+0x6C == 0) in use on @p channel. Expired ones (age past the header's lifetime +0x2,
 * age measured from SPCHI_getClock + D_004A483C) are dropped with SPCHI_freeRequest. With @p list only ids (header +0x0) in
 * it count. When the channel's mode (D_004506A0) is 1, a request with header flag 1 (+0xA) is skipped while its
 * priority is below its bank's threshold (+0x16). The best has the highest priority (+0x4), then the smallest age, then
 * the highest sequence number (+0x68).
 *
 * @param list  the allowed ids {count, ids}, or 0 for any
 * @param channel  the channel
 * @return the record index, or -1
 */
int SPCHI_pickRequest(SPCHIDLIST *list, int channel) {
    int best = -1;
    unsigned int bestAge = 0xFFFFFFFF;
    int maxPri = 0;
    unsigned short bestH = 0;
    unsigned int now;
    int i, j;
    SPCHEVENT *reqHeader;
    int expired, inList, blocked;
    unsigned int age;

    now = SPCHI_getClock() + D_004A483C;
    for (i = 0; i < 16; i++) {
        if (D_0044FFF8_7760.requests[i].active == 0) continue;
        if (D_0044FFF8_7760.requests[i].channel != channel) continue;
        if (D_0044FFF8_7760.requests[i].continuation != 0) continue;
        expired = 0;
        blocked = 0;
        inList = 1;
        reqHeader = D_0044FFF8_7760.requests[i].event;
        if (list != 0) {
            unsigned short id = reqHeader->id;
            int idFound = 0;
            for (j = 0; j < list->count; j++) {
                if (list->ids[j] == id) {
                    idFound = 1;
                    break;
                }
            }
            inList = idFound;
        }
        age = now - D_0044FFF8_7760.requests[i].time;
        if (reqHeader->lifetime != 0 && reqHeader->lifetime < age) {
            expired = 1;
        }
        if (D_004506A0_7760[channel] == 1 && (reqHeader->flags & 1)) {
            SPCHKEY k;
            SPCHBANKSLOT *res;
            int belowMin = 0;
            k = *&D_0044FFF8_7760.requests[i].args.key; /* read through its address: retail forms it first */
            if (find_7760(&k, &res) && reqHeader->priority < ((SPCHBANKHDR *)res->data)->minPriority) {
                belowMin = 1;
            }
            blocked = belowMin;
        }
        if (expired) {
            SPCHI_freeRequest(i);
            continue;
        }
        if (!inList) continue;
        if (blocked) continue;
        if (maxPri < reqHeader->priority) {
            best = i;
            bestAge = age;
            maxPri = reqHeader->priority;
            bestH = D_0044FFF8_7760.requests[best].seq;
        } else if (reqHeader->priority == maxPri &&
                   (age < bestAge || (age == bestAge && bestH < D_0044FFF8_7760.requests[i].seq))) {
            best = i;
            bestAge = age;
            bestH = D_0044FFF8_7760.requests[best].seq;
        }
    }
    return best;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_pickNextRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHCHANNEL D_00450660_7A50[] __asm__("D_00450660");
unsigned short *SPCHI_findBankEntry(SPCHKEY *arg0);
int SPCHI_pickRequest_7A50(SPCHIDLIST *list, int arg1) __asm__("SPCHI_pickRequest");

/**
 * Picks the next speech request for a channel, preferring the follow-ups the channel's current line allows.
 *
 * When the channel's current line (D_00450660 [@p idx]) is set and its bank entry (SPCHI_findBankEntry) has flag 0x10 at
 * +0xA, the entry carries a follow-up id list (after its tables) and SPCHI_pickRequest is asked with that list first; with
 * no list, or when the list gives nothing, any request is taken.
 *
 * @param idx  the channel
 * @return the record index, or -1
 */
int SPCHI_pickNextRequest(int idx) {
    SPCHIDLIST list;
    int ok = 0;
    unsigned char *entryData;
    SPCHEVENT *bankEntry;
    unsigned char *followList;
    int reqIndex;

    list.count = 0;
    list.ids = 0;
    if (D_00450660_7A50[idx].line.entryId != 0xFFFF) {
        entryData = (unsigned char *)SPCHI_findBankEntry(&D_00450660_7A50[idx].line);
        bankEntry = (SPCHEVENT *)entryData;
        if (bankEntry != 0) {
            int hasList = 0;
            if (entryData[0xA] >> 4 & 1) {
                followList = entryData + (((bankEntry->variantCount * 2 + 3) & 0xFFFFFFFC) + 0xC) +
                             ((SPCH_TAG_COUNT(bankEntry->counts) * 3 + 3) & ~3) +
                             ((SPCH_CONSTRAINT_COUNT(bankEntry->counts) * 3 + 3) & ~3);
                hasList = 1;
                if (entryData[0xA] >> 3 & 1) {
                    followList += (bankEntry->variantCount * 2 + 7) & 0xFFFFFFFC;
                }
                list.count = ((SPCHFOLLOWLIST *)followList)->count;
                list.ids = ((SPCHFOLLOWLIST *)followList)->ids;
            }
            ok = hasList;
        }
    }
    if (ok) {
        reqIndex = SPCHI_pickRequest_7A50(&list, idx);
    } else {
        reqIndex = SPCHI_pickRequest_7A50(0, idx);
    }
    if (ok && reqIndex < 0) {
        reqIndex = SPCHI_pickRequest_7A50(0, idx);
    }
    return reqIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_cancelAllRequests);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct R_7B98 { unsigned int f0; unsigned short h; unsigned char c; unsigned char d; short s8; short next; void *p; int arr[20]; };
struct G_7B98 { int z[8]; int a[8]; int b[8]; int n; struct R_7B98 r[16]; };
extern struct G_7B98 D_0044FFF8_7B98 __asm__("D_0044FFF8");
void SPCHI_resetChannelLines(void);

/**
 * Cancels every queued speech request: frees the records in use, resets each channel's count (+0x0) and its two request
 * words (+0x20 and +0x40, -1), and the channel lines (SPCHI_resetChannelLines).
 */
void SPCH_cancelAllRequests(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (D_0044FFF8_7B98.r[i].c != 0) {
            D_0044FFF8_7B98.r[i].c = 0;
            D_0044FFF8_7B98.z[D_0044FFF8_7B98.r[i].d]--;
        }
    }
    for (i = 0; i < 8; i++) {
        D_0044FFF8_7B98.b[i] = -1;
        D_0044FFF8_7B98.a[i] = -1;
        if (D_0044FFF8_7B98.z[i] != 0) {
            D_0044FFF8_7B98.z[i] = 0;
        }
    }
    SPCHI_resetChannelLines();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_freeRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct R_7C38 {
    unsigned int time;
    unsigned short h;
    unsigned char c;
    unsigned char d;
    short isContinuation;
    short next;
    void *p;
    int arr[20];
};
struct G_7C38 { int z[8]; int a[8]; int b[8]; int n; struct R_7C38 r[16]; };
extern struct G_7C38 D_0044FFF8_7C38 __asm__("D_0044FFF8");

/**
 * Frees speech request @p reqIndex and the continuation records chained after it (+0x6E links, +0x6C set).
 *
 * Only a chain head (+0x6C == 0) below 16 is freed. The channel's request words (+0x20 and +0x40) pointing at it, or
 * the first at a continuation, become -1, and the channel's count drops by one (not below 0).
 *
 * @param reqIndex  the record index
 */
void SPCHI_freeRequest(unsigned int reqIndex) {
    int channel;
    int j = -1;
    if (reqIndex < 16 && D_0044FFF8_7C38.r[reqIndex].isContinuation == 0) {
        channel = D_0044FFF8_7C38.r[reqIndex].d;
        D_0044FFF8_7C38.r[reqIndex].c = 0;
        if (reqIndex == D_0044FFF8_7C38.b[channel]) {
            D_0044FFF8_7C38.b[channel] = j;
        }
        if (reqIndex == D_0044FFF8_7C38.a[channel]) {
            D_0044FFF8_7C38.a[channel] = j;
        }
        j = D_0044FFF8_7C38.r[reqIndex].next;
        while (j != -1) {
            if ((unsigned int)j >= 16) {
                break;
            }
            if (D_0044FFF8_7C38.r[j].isContinuation != 0 && D_0044FFF8_7C38.r[j].c != 0) {
                if (j == D_0044FFF8_7C38.a[channel]) {
                    D_0044FFF8_7C38.a[channel] = -1;
                }
                D_0044FFF8_7C38.r[j].c = 0;
                j = D_0044FFF8_7C38.r[j].next;
            } else {
                j = -1;
            }
        }
        if (D_0044FFF8_7C38.r[reqIndex].isContinuation == 0) {
            D_0044FFF8_7C38.z[channel]--;
        }
        if (D_0044FFF8_7C38.z[channel] < 0) {
            D_0044FFF8_7C38.z[channel] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_pruneChannelQueue);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct P_7D58 { char pad[9]; signed char b9; unsigned char ba; };
struct R_7D58 {
    unsigned int time;
    unsigned short h;
    unsigned char c;
    unsigned char d;
    short isContinuation;
    short next;
    struct P_7D58 *p;
    int arr[20];
};
struct G_7D58 { int z[8]; int a[8]; int b[8]; int n; struct R_7D58 r[16]; };
extern struct G_7D58 D_0044FFF8_7D58 __asm__("D_0044FFF8");
void SPCHI_freeRequest(unsigned int);

/**
 * Prunes a channel's queue before speech request @p idx plays.
 *
 * Clears the flag D_0044FFF8 +0x60, then looks at the other chain-head requests on the same channel. Older ones
 * (earlier time +0x64, or the same time and a lower sequence number) are dropped (SPCHI_freeRequest) unless their header
 * has flag 4 (+0xA); a newer one whose header +0x9 is 100 sets the flag.
 *
 * @param idx  the record about to play
 */
void SPCHI_pruneChannelQueue(int idx) {
    int i;
    unsigned int otherTime;
    unsigned short otherSeq;
    int channel;
    unsigned int reqTime;
    unsigned short reqSeq;
    reqTime = D_0044FFF8_7D58.r[idx].time;
    channel = D_0044FFF8_7D58.r[idx].d;
    reqSeq = D_0044FFF8_7D58.r[idx].h;
    D_0044FFF8_7D58.n = 0;
    for (i = 0; i < 16; i++) {
        if (i == idx) {
            continue;
        }
        if (D_0044FFF8_7D58.r[i].c == 0 || D_0044FFF8_7D58.r[i].d != channel ||
            D_0044FFF8_7D58.r[i].isContinuation != 0) {
            continue;
        }
        otherTime = D_0044FFF8_7D58.r[i].time;
        otherSeq = D_0044FFF8_7D58.r[i].h;
        if (otherTime < reqTime || (otherTime == reqTime && otherSeq < reqSeq)) {
            if (((D_0044FFF8_7D58.r[i].p->ba >> 2) & 1) == 0) {
                SPCHI_freeRequest(i);
            }
        } else if (D_0044FFF8_7D58.r[i].p->b9 == 100 && D_0044FFF8_7D58.r[i].d == channel) {
            D_0044FFF8_7D58.n = 1;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCH_startNextRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct K_7EC8 { unsigned char b[4]; };
struct R_7EC8 { unsigned int f0; unsigned short h; unsigned char c; unsigned char d; short s8; short next; void *p; int arr[20]; };
struct G_7EC8 { int z[8]; int a[8]; int b[8]; int n; struct R_7EC8 r[16]; };
extern struct G_7EC8 D_0044FFF8_7EC8 __asm__("D_0044FFF8");
struct S_7EC8 { int a; int b; int c; int (*fn)(struct K_7EC8 *); };
extern struct S_7EC8 D_0044FF90_7EC8 __asm__("D_0044FF90");
int SPCHI_pickNextRequest(int idx);
void SPCHI_freeRequest(unsigned int);
void SPCHI_pruneChannelQueue(int idx);
void SPCHI_resetPlayList(void);
int SPCHI_buildRequest(int);

int SPCH_startNextRequest_7EC8(int a) __asm__("SPCH_startNextRequest");
/**
 * Starts the next queued speech on channel @p channel; returns the build result (0 when nothing started).
 *
 * Does nothing when the channel has no requests. Requests are picked with SPCHI_pickNextRequest and offered to the approve hook
 * (D_0044FF90 +0xC, given the packed key); a refused one is dropped and the next tried. The chosen request prunes the
 * queue (SPCHI_pruneChannelQueue), the play list is reset (SPCHI_resetPlayList) and SPCHI_buildRequest adds the request and its chained
 * continuations until one fails. The request is then freed.
 *
 * @param channel  the channel
 * @return the last SPCHI_buildRequest result, 0 when none
 */
int SPCH_startNextRequest_7EC8(int channel) {
    int result = 0;
    int done = 0;
    int idx, approved, chainIdx;
    struct K_7EC8 k;
    if (D_0044FFF8_7EC8.z[channel] != 0) {
        do {
            idx = SPCHI_pickNextRequest(channel);
            if (idx < 0) goto out;
            k = *(struct K_7EC8 *)D_0044FFF8_7EC8.r[idx].arr;
            approved = 1;
            if (D_0044FF90_7EC8.fn != 0) {
                approved = D_0044FF90_7EC8.fn(&k);
            }
            if (approved == 1) {
                done = 1;
            } else {
                SPCHI_freeRequest(idx);
            }
        } while (done == 0);
        SPCHI_pruneChannelQueue(idx);
        chainIdx = idx;
        SPCHI_resetPlayList();
        do {
            result = SPCHI_buildRequest(chainIdx);
            chainIdx = D_0044FFF8_7EC8.r[chainIdx].next;
        } while (chainIdx != -1 && result != 0);
        SPCHI_freeRequest(idx);
    }
out:
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_makeKey);
#ifdef SKIP_ASM

/**
 * Packs a speech key: @p bankId in byte 3, @p subId in byte 2 and @p entryId in the low halfword (the bank entry id). The two
 * bytes are matched against a bank's +0x8 and +0x9 ids (SPCHI_findBankEntry).
 *
 * @param bankId  the bank id
 * @param subId  the sub id
 * @param entryId  the entry id
 * @return the packed key
 */
int SPCH_makeKey(int bankId, int subId, int entryId) {
    SPCHKEY packed;
    packed.bankId = bankId;
    packed.subId = subId;
    packed.entryId = entryId;
    return *(int *)&packed;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_getQueuedCount);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFF8_8028[128] __asm__("D_0044FFF8");

/**
 * Returns the number of speech requests queued on a channel (D_0044FFF8 [@p channel]).
 *
 * @param channel  the channel
 * @return the count
 */
int SPCH_getQueuedCount(int channel) {
    return D_0044FFF8_8028[channel];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_updateChannel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A484C_8040 __asm__("D_004A484C");
int SPCHI_hasPlayList_8040(void) __asm__("SPCHI_hasPlayList");
int SPCH_startNextRequest_8040(int) __asm__("SPCH_startNextRequest");
int SPCHI_playList_8040(int) __asm__("SPCHI_playList");

/**
 * Updates speech channel @p channel: when its play list still has lines (SPCHI_hasPlayList) or a new request starts
 * (SPCH_startNextRequest), plays the list (SPCHI_playList). Guarded against re-entry by D_004A484C.
 *
 * @param channel  the channel
 * @return SPCHI_playList's result, 0 when nothing played or re-entered
 */
int SPCH_updateChannel(int channel) {
    int result = 0;
    if (D_004A484C_8040 == 0) {
        D_004A484C_8040 = 1;
        if (SPCHI_hasPlayList_8040() != 0 || SPCH_startNextRequest_8040(channel) != 0) {
            result = SPCHI_playList_8040(channel);
        }
    }
    D_004A484C_8040 = 0;
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_playLatestRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_80B0 { int z[8]; unsigned int a[8]; unsigned int b[8]; };
extern struct G_80B0 D_0044FFF8_80B0 __asm__("D_0044FFF8");
extern int D_004A484C_80B0 __asm__("D_004A484C");
void SPCHI_resetPlayList(void);
int SPCHI_buildRequest(int);
void SPCHI_freeRequest(unsigned int);
void SPCHI_playList(int);

/**
 * Plays channel @p x's latest speech request at once: when both of its request words (+0x20, +0x40) are valid, the play
 * list is reset (SPCHI_resetPlayList), filled from the latest request (SPCHI_buildRequest), the request freed, and the list played
 * (SPCHI_playList) when it was filled. Guarded against re-entry by D_004A484C.
 *
 * @param x  the channel
 */
void SPCH_playLatestRequest(int x) {
    if (D_004A484C_80B0 == 0) {
        unsigned int headReq = D_0044FFF8_80B0.a[x];
        unsigned int latestReq = D_0044FFF8_80B0.b[x];
        int ok;
        D_004A484C_80B0 = 1;
        ok = 1;
        if (headReq >= 16) ok = 0;
        if (latestReq >= 16) ok = 0;
        if (ok) {
            int built;
            SPCHI_resetPlayList();
            built = SPCHI_buildRequest(latestReq);
            SPCHI_freeRequest(latestReq);
            if (built != 0) {
                SPCHI_playList(x);
            }
        }
    }
    D_004A484C_80B0 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_setChannelMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004506A0_8158[128] __asm__("D_004506A0");

/**
 * Sets a speech channel's mode (D_004506A0 [@p channel]); mode 1 makes SPCHI_pickRequest enforce the bank's priority threshold
 * for flagged requests.
 *
 * @param mode  the mode
 * @param channel  the channel
 */
void SPCH_setChannelMode(int mode, int channel) {
    D_004506A0_8158[channel] = mode;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCH_startNextRequestChannel0);
#ifdef SKIP_ASM
extern void SPCH_startNextRequest(int);

/**
 * Starts the next queued speech on channel 0 (SPCH_startNextRequest(0)) and discards the result.
 */
void SPCH_startNextRequestChannel0(void) {
    SPCH_startNextRequest(0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCH_updateChannel0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SPCH_startNextRequest_8190(int) __asm__("SPCH_startNextRequest");
extern void SPCHI_playList(int);
extern int SPCHI_hasPlayList(int);
extern int D_004A484C_484C __asm__("D_004A484C");

/**
 * Updates speech channel 0 (the body of SPCH_updateChannel for channel 0, without a result).
 */
void SPCH_updateChannel0(void) {
    if (D_004A484C_484C == 0) {
        D_004A484C_484C = 1;
        if (SPCHI_hasPlayList(0) != 0) {
            SPCHI_playList(0);
        } else if (SPCH_startNextRequest_8190(0) != 0) {
            SPCHI_playList(0);
        }
    }
    D_004A484C_484C = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getBankRegKey);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHBANKSLOT D_0044FFB8_82B0[] __asm__("D_0044FFB8");

/**
 * Looks up the key the speech bank for a speech key was registered under (SPCHI_registerBank's key).
 *
 * The bank is the first of the 8 bank slots (D_0044FFB8) whose ids (+0x9, +0x8) match key bytes 2 and 3.
 *
 * @param speechKey  the 4-byte speech key
 * @param regKeyOut  receives the bank's registration key when found
 * @return 1 when found, else 0
 */
int SPCHI_getBankRegKey(SPCHKEY *speechKey, int *regKeyOut) {
    SPCHBANKSLOT *res;
    SPCHBANKSLOT *bankSlot = D_0044FFB8_82B0;
    int ret = 0;
    SPCHBANKSLOT **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *bankData = bankSlot->data;
        if (bankData != 0 && bankData[9] == speechKey->subId && bankData[8] == speechKey->bankId) {
            *pres = bankSlot;
            found = 1;
            goto done;
        }
        i++;
        bankSlot++;
    } while (i < 8);
done:
    if (found) {
        ret = 1;
        *regKeyOut = res->key;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_clearBanks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFB8_8278[] __asm__("D_0044FFB8");

/**
 * Clears the 8 speech bank slots (D_0044FFB8: data and key).
 */
void SPCHI_clearBanks(void) {
    int *var_2;
    int var_3;

    var_3 = 7;
    var_2 = D_0044FFB8_8278;
    do {
        var_2[0] = 0;
        var_3 -= 1;
        var_2[1] = 0;
        var_2 += 2;
    } while (var_3 >= 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getBankLengthLimit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHBANKSLOT D_0044FFB8_82B0[] __asm__("D_0044FFB8");

/**
 * Returns halfword +0x14 of the speech bank registered for a speech key (matched as in SPCHI_getBankRegKey), or 0.
 *
 * @param speechKey  the 4-byte speech key
 * @return the bank's +0x14 value, 0 when no bank matches
 */
unsigned short SPCHI_getBankLengthLimit(SPCHKEY *speechKey) {
    SPCHBANKSLOT *res;
    SPCHBANKSLOT *bankSlot = D_0044FFB8_82B0;
    unsigned short ret = 0;
    SPCHBANKSLOT **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *bankData = bankSlot->data;
        if (bankData != 0 && bankData[9] == speechKey->subId && bankData[8] == speechKey->bankId) {
            *pres = bankSlot;
            found = 1;
            goto done;
        }
        i++;
        bankSlot++;
    } while (i < 8);
done:
    if (found) {
        ret = ((SPCHBANKHDR *)res->data)->value14;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getBankSubId);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHBANKSLOT D_0044FFB8_82B0[] __asm__("D_0044FFB8");

/**
 * Looks up the sub id (+0x9) of the speech bank registered for a speech key (matched as in SPCHI_getBankRegKey).
 *
 * @param speechKey  the 4-byte speech key
 * @param subIdOut  receives the bank's +0x9 byte when found
 * @return 1 when found, else 0
 */
int SPCHI_getBankSubId(SPCHKEY *speechKey, int *subIdOut) {
    SPCHBANKSLOT *res;
    SPCHBANKSLOT *bankSlot = D_0044FFB8_82B0;
    int ret = 0;
    SPCHBANKSLOT **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *bankData = bankSlot->data;
        if (bankData != 0 && bankData[9] == speechKey->subId && bankData[8] == speechKey->bankId) {
            *pres = bankSlot;
            found = 1;
            goto done;
        }
        i++;
        bankSlot++;
    } while (i < 8);
done:
    if (found) {
        ret = 1;
        *subIdOut = res->data[9];
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getBankUsage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHBANKSLOT D_0044FFB8_83B8[] __asm__("D_0044FFB8");

/**
 * Looks up the usage bytes of the speech bank registered for a speech key (the area SPCHI_resetBankUsage resets).
 *
 * @param speechKey  the 4-byte speech key
 * @param usageOut  receives the bank's usage area when found
 * @return 1 when found, else 0
 */
int SPCHI_getBankUsage(SPCHKEY *speechKey, unsigned char **usageOut) {
    SPCHBANKSLOT *res;
    SPCHBANKSLOT *bankSlot = D_0044FFB8_83B8;
    int ret = 0;
    SPCHBANKSLOT **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *bankData = bankSlot->data;
        if (bankData != 0 && bankData[9] == speechKey->subId && bankData[8] == speechKey->bankId) {
            *pres = bankSlot;
            found = 1;
            goto done;
        }
        i++;
        bankSlot++;
    } while (i < 8);
done:
    if (found) {
        unsigned char *bankBase = res->data;
        ret = 1;
        *usageOut = bankBase + (((((SPCHBANKHDR *)bankBase)->entryCount * 2 + 3) & 0xFFFFFFFC) + 0x18);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_findEntryInBank);
#ifdef SKIP_ASM
/**
 * Returns the entry of a speech bank whose first halfword is @p key, or 0. The entries are reached through the word
 * offsets at +0x18 (count +0x10). The same search is inlined in SPCHI_findBankEntry.
 *
 * @param header  the speech bank data
 * @param key  the entry id
 * @return the entry, or 0
 */
unsigned short *SPCHI_findEntryInBank(void *header, int key) {
    int var_7;
    unsigned short *var_2;
    unsigned short *var_6;
    unsigned short temp_8;

    temp_8 = ((SPCHBANKHDR *)header)->entryCount;
    var_7 = 0;
    if (temp_8 != 0) {
        var_6 = header + 0x18;
loop_2:
        var_2 = header + (*var_6 * 4);
        var_7 += 1;
        if (*var_2 != key) {
            var_6 += 1;
            if (var_7 >= (int) temp_8) {
                goto block_4;
            }
            goto loop_2;
        }
    } else {
block_4:
        var_2 = 0;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_findBankSlot);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern SPCHBANKSLOT D_0044FFB8_84A0[] __asm__("D_0044FFB8");

/**
 * Finds the speech bank slot (D_0044FFB8) for a speech key: the first whose ids (+0x9, +0x8) match key bytes 2 and 3.
 * The same search is inlined in SPCHI_findBankEntry and its siblings.
 *
 * @param speechKey  the 4-byte speech key
 * @param pres  receives the slot when found
 * @return 1 when found, else 0
 */
int SPCHI_findBankSlot(SPCHKEY *speechKey, SPCHBANKSLOT **pres) {
    SPCHBANKSLOT *bankSlot = D_0044FFB8_84A0;
    int found = 0;
    int i = 0;
    do {
        unsigned char *bankData = bankSlot->data;
        if (bankData != 0 && bankData[9] == speechKey->subId && bankData[8] == speechKey->bankId) {
            *pres = bankSlot;
            found = 1;
            goto done;
        }
        i++;
        bankSlot++;
    } while (i < 8);
done:
    return found;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_hasLatestRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_8500 { unsigned int z[8]; unsigned int a[8]; unsigned int b[8]; };
extern struct S_8500 D_0044FFF8_8500 __asm__("D_0044FFF8");
/**
 * Returns whether speech channel @p i has both request words (+0x20 and +0x40 [channel]) set, the test SPCH_playLatestRequest
 * makes before it plays the latest request.
 *
 * @param i  the channel
 * @return 1 when both are valid indices (below 16), else 0
 */
int SPCHI_hasLatestRequest(int i) {
    int valid = D_0044FFF8_8500.a[i] < 0x10;
    if (D_0044FFF8_8500.b[i] >= 0x10) valid = 0;
    return valid;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_clearIdList);
#ifdef SKIP_ASM
/**
 * Clears an id list {count, ids} as SPCHI_pickRequest takes it.
 *
 * @param idList  the list
 */
void SPCHI_clearIdList(SPCHIDLIST *idList) {
    idList->count = 0;
    idList->ids = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_isInIdList);
#ifdef SKIP_ASM
/**
 * Returns whether the low halfword of @p wantedId is in an id list {count, ids}: the test SPCHI_pickRequest inlines.
 *
 * @param wantedId  the id
 * @param list  the list
 * @return 1 when present, else 0
 */
int SPCHI_isInIdList(int wantedId, SPCHIDLIST *list) {
    unsigned key = wantedId & 0xFFFF;
    int i;
    int found = 0;
    for (i = 0; i < list->count; i++) {
        if (list->ids[i] == key) {
            found = 1;
            break;
        }
    }
    return found;
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003D8598);

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_meetsConditions);
#ifdef SKIP_ASM
int SPCHI_getBankUsage(SPCHKEY *arg0, unsigned char **arg1);

/**
 * Returns whether a set of tag values meets a rule's column conditions (up to 4; a rule with 5 or more fails).
 *
 * For each condition i the value @p vals [i] must be below 32 and its bit must be in the rule's mask (+0x14 [i]). Its
 * kind (+0x8 [i]) then decides: 0 or 0xFE pass; 0xFF compares through +0x10 [i], either (bit 0x80) with the object's
 * variable word [low 7 bits] or with that byte of the usage area of the bank named by the key at @p tbl
 * (SPCHI_getBankUsage); any other kind needs the bit in argument word @p tbl [kind].
 *
 * @param obj  the phrase template (its variable words follow the slot list)
 * @param rule  the condition rule
 * @param tbl  the request arguments (word 0 is the speech key)
 * @param vals  the tag values to test
 * @return 1 when every condition holds, else 0
 */
int SPCHI_meetsConditions(unsigned char *obj, struct sCond *rule, SPCHARGS *tbl, unsigned char *vals) {
    int ok = 1;
    int n = rule->n;
    int i;

    if (n >= 5) {
        ok = 0;
    } else {
        for (i = 0; i < n; i++) {
            unsigned int value = vals[i];
            unsigned int mask;
            int k;
            unsigned int *mp;
            ok = 0;
            if (value < 32) {
                mask = 1 << value;
                k = rule->a[i];
                mp = &rule->m[i];
                if (mask & *mp) {
                    if (k == 0 || k == 0xFE) {
                        ok = 1;
                    } else if (k == 0xFF) {
                        unsigned int cond = rule->b[i];
                        if (cond & 0x80) {
                            unsigned char *maskTable;
                            cond &= 0x7F;
                            maskTable = obj + ((((obj[4] >> 2) + 3) & 0xFFFFFFFC) + 8) + ((obj[6] * 4 + 3) & 0xFFFFFFFC);
                            ok = 0;
                            if (mask & ((int *)maskTable)[cond]) ok = 1;
                        } else {
                            SPCHKEY key;
                            unsigned char *res;
                            key = tbl->key;
                            if (SPCHI_getBankUsage(&key, &res)) {
                                ok = 0;
                                if (value == res[cond]) ok = 1;
                            }
                        }
                    } else {
                        ok = 0;
                        if (mask & *(tbl->words + k)) ok = 1; /* pointer form: retail adds the index first */
                    }
                }
            } else {
                break;
            }
            if (!ok) break;
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_isRowAllowed);
#ifdef SKIP_ASM
/**
 * Returns whether row @p idx of a speech table may be picked, given the previous pick.
 *
 * Rows past the row count (+0x5) fail. When @p state [1] holds a previous row (not -1) below the entry count (+0xA),
 * the transition bit (previous * rows + @p idx) of the matrix after the subset area and history must be set. A
 * random-subset table (flag 0x80) also needs @p idx's bit in its current subset.
 *
 * @param state  the selection state; halfword +0x2 is the previous row or -1
 * @param node  the speech table
 * @param idx  the row
 * @return nonzero when allowed
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
int SPCHI_isRowAllowed(short *state, unsigned char *node, int idx) {
    int w = node[5];
    int result = 1;
    if (w < idx) {
        result = 0;
    } else {
        int current = state[1];
        if (current != -1) {
            int isSet = 0;
            if (current < ((SPCHTABLEHDR *)node)->entryCount) {
                int bit = w * current + idx;
                unsigned char flags = node[4];
                int colCount = flags & 0xF;
                int off = 0;
                unsigned char *rowsEnd = node + ((w * (colCount + 2) + 0xF) & 0x3FFC) + colCount * 4;
                unsigned char *matrix;
                int k;
                unsigned char mask;
                if (flags & 0x80) {
                    off = (w + 7) / 8 + 1;
                }
                matrix = rowsEnd + off;
                if (node[6] != 0) {
                    matrix += node[6] + 1;
                }
                k = bit / 8;
                mask = 1 << (bit - k * 8);
                isSet = matrix[k] & mask;
            }
            result = isSet;
            if (result == 0) {
                goto out;
            }
        }
        if (node[4] & 0x80) {
            int colCount = node[4] & 0xF;
            unsigned char *rowsEnd = node + ((node[5] * (colCount + 2) + 0xF) & 0x3FFC) + colCount * 4;
            int k = idx / 8;
            unsigned char mask = 1 << (idx - k * 8);
            result = (rowsEnd + k)[1] & mask;
        }
    }
out:
    return result;
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003D88C8);

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_orderOptionsByWeight);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_004506E0_8CB0[] __asm__("D_004506E0");

#define WEIGHT_8CB0(p) ((*(p) & 0x1F) * D_004506E0_8CB0[*(p) >> 5])

/**
 * Orders the options of a set by weighted random draw, without repeats, into @p out.
 *
 * The set has +0x6 options reached through the halfword word-offsets at +0xC; an option's weight is its first byte's
 * low 5 bits times the scale D_004506E0 [its top 3 bits]. Options are drawn with SPCHI_random(total, -1) and removed
 * from the pool; the zero-weight options follow at the end.
 *
 * @param optionSet  the option set
 * @param out  receives the option indices in order
 */
void SPCHI_orderOptionsByWeight(SPCHEVENT *optionSet, unsigned char *out) {
    int w[100];
    int n = optionSet->variantCount;
    int total = 0;
    int k;
    int i;
    int pick;
    unsigned char *option;

    for (i = 0; i < n; i++) {
        // ORIGINAL-PTR: an entry's variants lie at the word offsets its variant table gives (a variable-length
        // record), so they have no fixed member
        option = (unsigned char *)optionSet + optionSet->variantOffsets[i] * 4;
        w[i] = WEIGHT_8CB0(option);
        total += WEIGHT_8CB0(option);
    }
    k = 0;
    while (total > 0) {
        pick = SPCHI_random(total, -1);
        for (i = 0; i < n; i++) {
            pick -= w[i];
            if (pick < 0) break;
        }
        out[k++] = i;
        total -= w[i];
        w[i] = 0;
    }
    for (i = 0; i < n; i++) {
        option = (unsigned char *)optionSet + optionSet->variantOffsets[i] * 4;
        if (WEIGHT_8CB0(option) == 0) {
            out[k++] = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_checkVariables);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char **D_004A4824_8E58 __asm__("D_004A4824");      /* shape table, D_004A4828 entries */
int SPCHI_readRow(unsigned char *shp, int idx, struct sShapeInfo *out);

/**
 * Checks a phrase's chosen rows against its shared variables: returns 1 when they agree, 0 on a conflict or an
 * unreadable row.
 *
 * Clears the template's variable words (+0x5 of them after the slot list) and each slot's done flag, then passes over
 * the slots until every one is done. For each condition column with a variable (+0x10 bit 0x80), kind 0xFE binds the
 * variable to the chosen row's tag bit (SPCHI_readRow) and kind 0xFF requires the tag bit in it; a slot that needs a
 * variable not bound yet waits for the next pass. Quirk: a variable no slot binds makes it loop forever.
 *
 * @param selSet  the selection set
 * @param phrase  the phrase template
 * @return 1 when consistent, else 0
 */
int SPCHI_checkVariables(struct sSelSet *selSet, struct sLayer *phrase) {
    struct sShapeInfo rowInfo;
    int ok = 1;
    int i;
    int varIndex;
    int j;
    int cnt;
    int redo;
    int n;
    int *tab;
    unsigned char *lp;
    struct sCond *slotCond;
    int bv;
    int varBits;

    // ORIGINAL-PTR: a template's variable words follow its variable-length slot list, so their offset comes from
    // the slot count
    tab = (int *)((unsigned char *)phrase + ((((phrase->cnt >> 2) + 3) & 0x7C) + 8) + phrase->varOffset * 4);
    for (varIndex = 0; varIndex < phrase->numVars; varIndex++) {
        tab[varIndex] = 0;
    }
    for (i = 11; i >= 0; i--) {
        selSet->ent[i].flag = 0;
    }
    cnt = phrase->cnt >> 2;
    do {
        redo = 1;
        i = 0;
        if (cnt != 0) {
            lp = phrase->list;
            do {
                if (selSet->ent[i].flag == 0) {
                    selSet->ent[i].flag = 1;
                    slotCond = (struct sCond *)((int *)phrase + lp[i]);
                    n = slotCond->n;
                    if (!SPCHI_readRow(D_004A4824_8E58[selSet->ent[i].x], selSet->tbl[selSet->ent[i].cur], &rowInfo)) {
                        ok = 0;
                        goto out;
                    }
                    for (j = 0; j < n; j++) {
                        bv = slotCond->b[j];
                        if (bv == 0xFF) continue;
                        if (!(bv & 0x80)) continue;
                        bv &= 0x7F;
                        if (slotCond->a[j] == 0xFF) {
                            varBits = tab[bv];
                            if (varBits == 0) {
                                selSet->ent[i].flag = 0;
                                redo = 0;
                                continue;
                            }
                            if (!((varBits >> rowInfo.col[j]) & 1)) {
                                ok = 0;
                                goto out;
                            }
                        } else if (slotCond->a[j] == 0xFE) {
                            tab[bv] = 1 << rowInfo.col[j];
                        }
                    }
                }
                i++;
            } while (i < cnt);
        }
    } while (!redo);
out:
    return ok;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_buildCandidates);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int SPCHI_readRow(unsigned char *shp, int idx, struct sShapeInfo *out);
extern int SPCHI_matchFlags(unsigned char a, unsigned char b, unsigned char *p);
extern int SPCHI_findTableById(unsigned short arg0);
extern int SPCHI_findTableBySubId(unsigned short arg0, unsigned short arg1);
extern int SPCHI_isTableEntryInRange(int arg0, unsigned int arg1);
extern int func_003D88C8(struct sSelSet *, unsigned char *, struct sSel *, struct sCond *, struct sTblEnt *);


static __inline__ int modeOk_9088(void *arg0, int arg1) {
    int temp_3;

    temp_3 = (*(unsigned char *)((char*)(arg0) + (4))) & 3;
    if (((temp_3 == 1) && (arg1 == 2)) || ((temp_3 == 2) && (arg1 != temp_3))) {
        return 1;
    }
    return temp_3 == 0;
}

static __inline__ void clear_9088(unsigned char *arg0) {
    int *q;
    int i;
    q = (int *)(arg0 + ((((arg0[4] >> 2) + 3) & 0x7C) + 8) + (arg0[6] * 4));
    for (i = 0; i < arg0[5]; i++) {
        q[i] = 0;
    }
}

static __inline__ int check_9088(unsigned char *h, struct sCond *s) {
    int ok = 1;
    int i;
    int *tab;
    int n = s->n;
    int b;
    for (i = 0; i < n; i++) {
        if (s->a[i] == 0xFF) {
            b = s->b[i];
            if (b != 0xFF && (b & 0x80)) {
                tab = (int *)(h + ((((h[4] >> 2) + 3) & 0xFFFFFFFC) + 8) + ((h[6] * 4 + 3) & 0xFFFFFFFC));
                if (tab != 0) {
                    b &= 0x7F;
                    if (tab[b] == 0) {
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }
    return ok;
}

static __inline__ int resolve_9088(struct sCond *p, struct sTblEnt *tbl, struct sSel *out) {
    unsigned short v = p->id;
    unsigned int mode = p->mode;
    unsigned int idx = p->idx;
    int r;
    out->y = -1;
    out->x = -1;
    switch (mode) {
    case 0:
        out->x = SPCHI_findTableById(v);
        break;
    case 1:
        if (idx >= 20) {
            out->x = -1;
        } else {
            out->x = SPCHI_findTableBySubId(v, tbl[idx].a);
        }
        break;
    case 2: {
        struct sTblEnt *e;
        int t = SPCHI_findTableById(v);
        e = (struct sTblEnt *)(idx * 4 + (int)tbl);
        if (SPCHI_isTableEntryInRange(t, *(int *)e) != 0) {
            out->y = e->a;
        } else {
            t = -1;
        }
        out->x = t;
        break;
    }
    }
    r = out->x;
    if (r <= -1)
        return 0;
    return 1;
}

/**
 * Builds the candidate rows of every slot of a phrase template for a request; returns 1 on success.
 *
 * Fails when the template has 13 or more slots (+0x4 >> 2), when its kind (+0x4 & 3) does not suit @p mode (1 needs
 * mode 2, 2 needs another mode, 0 suits any) or when @p flags fails its care mask (SPCHI_matchFlags). Each slot whose
 * required variables are set gets its speech table (by id, by id and an argument word, or by id with an entry check;
 * SPCHI_findTableById, SPCHI_findTableBySubId, SPCHI_isTableEntryInRange) and its candidates from func_003D88C8 into the shared row list. Up to
 * 200 passes are made until all slots resolve.
 *
 * @param hdr  the phrase template
 * @param tbl  the request arguments
 * @param flags  the request's flag byte
 * @param ignoreMask  flag bits not compared
 * @param mode  the request mode
 * @return 1 when every slot has candidates, else 0
 */
int SPCHI_buildCandidates(struct sSelSet *self, unsigned char *hdr, struct sTblEnt *tbl, int flags, int ignoreMask, int mode) {
    int ret = 1;
    int all;
    int tries = 200;
    int n = hdr[4] >> 2;
    unsigned char *lp;
    int i;

    if (n >= 13 || !modeOk_9088(hdr, mode) || !SPCHI_matchFlags(flags, ignoreMask, hdr)) {
        ret = 0;
    } else {
        clear_9088(hdr);
        self->n = n;
        do {
            all = 1;
            tries--;
            i = 0;
            if (n != 0) {
                lp = hdr + 8;
                do {
                    struct sSel *slot = &self->ent[i];
                    // ORIGINAL-PTR: the slot conditions lie at the word offsets the slot list gives (a
                    // variable-length record), so they have no fixed member
                    struct sCond *cond = (struct sCond *)(hdr + lp[i] * 4);
                    if (!check_9088(hdr, cond)) {
                        all = 0;
                    } else if (slot->flag == 0) {
                        int count;
                        slot->start = self->ntbl;
                        if (!resolve_9088(cond, tbl, slot)) {
                            ret = 0;
                            goto out;
                        }
                        count = func_003D88C8(self, hdr, slot, cond, tbl);
                        if (count == 0) {
                            ret = 0;
                            goto out;
                        }
                        slot->len = count;
                        slot->flag = 1;
                    }
                    i++;
                } while (i < n);
            }
        } while (!all && tries > 0);
        if (tries == 0) {
            ret = 0;
        }
    }
out:
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_findFittingCombination);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char **D_004A4824_93C0 __asm__("D_004A4824");      /* shape table, D_004A4828 entries */
extern int D_004A4838_93C0 __asm__("D_004A4838");
int SPCHI_readRow(unsigned char *shp, int idx, struct sShapeInfo *out);
/* Combination search: reset every entry, then advance the odometer until the usage
   percentage fits under `thr` and the constraints (SPCHI_checkVariables) hold, or the odometer
   wraps. The helpers are the inline bodies of SPCHI_getSelectionPercent (usage percent) and
   SPCHI_nextCombination (next combination); the retail TU keeps their out-of-line copies at the
   end of the file. */

/* same body as SPCHI_getSelectionPercent */
static inline int usage_93C0(unsigned char *a0, struct sSelSet *h) {
    struct sShapeInfo o;
    int total = 0;
    int i;
    int r;
    int cnt = a0[4] >> 2;

    for (i = 0; i < cnt; i++) {
        if (SPCHI_readRow(D_004A4824_93C0[h->ent[i].x], h->tbl[h->ent[i].cur], &o)) {
            total += o.len;
        }
    }
    r = 0;
    if (D_004A4838_93C0 != 0) {
        r = total * 100 / D_004A4838_93C0;
    }
    return r;
}

/* same body as SPCHI_nextCombination */
static inline int next_93C0(unsigned char *h, struct sSel *e) {
    int wrap = 0;
    int done = 0;
    int i = (h[4] >> 2) - 1;
    struct sSel *p = &e[i];
    int t = p->len;
    int limit = p->start + t;
    do {
        if (++p->cur < limit) {
            done = 1;
        } else {
            p->cur = p->start;
            if (--i < 0) {
                done = 1;
                wrap = 1;
            }
            p = &e[i];
            t = p->len;
            limit = p->start + t;
        }
    } while (!done);
    return wrap;
}

/**
 * Searches the slots' candidate combinations for one that fits: returns 1 when found, 0 when all were tried.
 *
 * Starts each slot at its first candidate and steps through the combinations, last slot fastest (the inlined
 * SPCHI_nextCombination). A combination fits when its total length is at most @p thr percent of the buffer (D_004A4838; the
 * inlined SPCHI_getSelectionPercent, skipped when @p thr is 0) and, for a template with variables, SPCHI_checkVariables accepts it.
 *
 * @param phrase  the phrase template
 * @param selSet  the selection set
 * @param thr  the length limit in percent of the buffer, 0 for none
 * @return 1 when a combination fits, else 0
 */
int SPCHI_findFittingCombination(struct sLayer *phrase, struct sSelSet *selSet, int thr) {
    int ok = 0;
    int of = 0;
    int i;
    int n;
    int pct;

    n = phrase->cnt >> 2;
    for (i = 0; i < n; i++) {
        selSet->ent[i].cur = selSet->ent[i].start;
    }
    do {
        ok = 1;
        pct = 0;
        if (thr != 0) {
            pct = usage_93C0((unsigned char *)phrase, selSet);
            if (thr < pct) ok = 0;
        }
        if (ok) {
            if (phrase->numVars != 0) {
                if (SPCHI_checkVariables(selSet, phrase) == 0) ok = 0;
            }
        }
        if (!ok) {
            of = next_93C0((unsigned char *)phrase, selSet->ent);
        }
    } while (!of && !ok);
    return ok;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_chooseRows);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SPCHI_getBankUsage_95B8(SPCHKEY *, unsigned char **) __asm__("SPCHI_getBankUsage");
extern int SPCHI_readRow_95B8(unsigned char *, int, struct sShapeInfo *) __asm__("SPCHI_readRow");
extern int SPCHI_findFittingCombination_95B8(struct sLayer *, struct sSelSet *, int) __asm__("SPCHI_findFittingCombination");
extern unsigned char **D_004A4824_95B8 __asm__("D_004A4824");
extern int SPCHI_random(int, int);

static __inline__ void shuffle_95B8(struct sLayer *l, struct sSelSet *h) {
    int i = 0;
    int j;
    int n = l->cnt >> 2;
    struct sSel *e;
    unsigned char *p;
    unsigned char *q;
    unsigned char t;

    for (; i < n; i++) {
        e = &h->ent[i];
        p = &h->tbl[e->start];
        for (j = e->len; j >= 2; j--) {
            q = &p[SPCHI_random(j, -1)];
            t = p[j - 1];
            p[j - 1] = *q;
            *q = t;
        }
    }
}

static __inline__ void apply_95B8(SPCHKEY *a0, struct sSelSet *h, struct sLayer *l) {
    struct sShapeInfo o;
    unsigned char *out;
    int n;
    int i;
    int j;
    struct sCond *c;

    if (SPCHI_getBankUsage_95B8(a0, &out) != 0) {
        i = 0;
        n = l->cnt >> 2;
        for (; i < n; i++) {
            c = (struct sCond *)((int *)l + l->list[i]);
            for (j = 0; j < c->n; j++) {
                if (c->a[j] == 0xFE && !(c->b[j] & 0x80)) {
                    if (SPCHI_readRow_95B8(D_004A4824_95B8[h->ent[i].x], h->tbl[h->ent[i].cur], &o)) {
                        out[c->b[j]] = o.col[j];
                    }
                }
            }
        }
    }
}

/**
 * Chooses the rows a phrase plays from its slots' candidates; returns 1 on success.
 *
 * In mode 1, or for a template with variables, each slot's candidates are shuffled (SPCHI_random) and the combinations
 * searched (SPCHI_findFittingCombination, with no length limit in mode 2). Otherwise each slot takes a random candidate (SPCHI_random
 * keyed by its table, so recent picks repeat less), and an empty slot fails. Bind columns without a variable (kind
 * 0xFE) then store the chosen tags in the bank's usage area (SPCHI_getBankUsage for @p speechKey).
 *
 * @param speechKey  the speech key
 * @param selSet  the selection set
 * @param phrase  the phrase template
 * @param mode  the request mode
 * @param thr  the length limit in percent of the buffer
 * @return 1 on success, else 0
 */
int SPCHI_chooseRows(SPCHKEY *speechKey, struct sSelSet *selSet, struct sLayer *phrase, int mode, int thr) {
    int ok;
    int i;
    int n;

    if (mode == 1 || phrase->numVars != 0) {
        shuffle_95B8(phrase, selSet);
        ok = SPCHI_findFittingCombination_95B8(phrase, selSet, mode != 2 ? thr : 0);
    } else {
        ok = 1;
        n = phrase->cnt >> 2;
        for (i = 0; i < n; i++) {
            if (selSet->ent[i].len == 0) {
                ok = 0;
                goto end;
            }
            selSet->ent[i].cur = selSet->ent[i].start + SPCHI_random(selSet->ent[i].len, selSet->ent[i].x);
        }
    }
    apply_95B8(speechKey, selSet, phrase);
end:
    return ok;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_sendPhraseLines);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char **D_004A4824_9830 __asm__("D_004A4824");      /* shape table, D_004A4828 entries */
extern int D_004A4830_9830 __asm__("D_004A4830");
extern int D_004A4838_9830 __asm__("D_004A4838");
/* regKey (SPCHI_getBankRegKey), tableSubId (the table's subId) and bankSubId (SPCHI_getBankSubId): names ours, from use */
struct sFF90Msg { int id; int start; int len; SPCHKEY k; int regKey; int tableSubId; int bankSubId; };
struct sFF90 {
    int (*play)(struct sFF90Msg *);                 /* SPCHI_sendPhraseLines (name ours: the play callback) */
    int (*f04)(unsigned char *, int, int, int);     /* SPCHI_askTagConditions */
    void (*f08)(void *, int, int, int);             /* SPCHI_reportSlotTags, SPCHI_reportEventTags */
    int (*f0C)(SPCHKEY *);                     /* SPCH_startNextRequest */
    int (*f10)(int, void *);                        /* SPCHI_buildRequest */
    int f14;
};
struct sFFA8 { int f00; int f04; int (*f08)(); int f0C; };
extern struct sFF90 D_0044FF90_9830 __asm__("D_0044FF90");
/* A play list line (D_00450700 +0xC, 0x30 of them): its speech table slot, repeat index and row. */
struct sPlayLine { short x; short idx; unsigned short y; short d; };
struct G_9830 { int f0, f4, f8; struct sPlayLine q[0x30]; };
extern struct G_9830 D_00450700_9830 __asm__("D_00450700");
int SPCHI_readRow(unsigned char *shp, int idx, struct sShapeInfo *out);

static __inline__ void clearBit_9830(unsigned char *s, int bit) {
    int n = s[4] & 0xF;
    int q = bit / 8;
    unsigned char *p = s + ((s[5] * (n + 2) + 0xF) & 0x3FFC) + n * 4 + (q + 1);
    *p &= ~(1 << (bit - q * 8));
}

/**
 * Sends a phrase's lines to the speech play callback and returns their total length as a percentage of the buffer.
 *
 * Does nothing without a play callback (D_0044FF90 +0x0, SPCH_StartSpeechCallback). Each of the template's slots takes a play list
 * entry from @p firstEntry (D_00450700): table and row. A random-subset table drops the row from its subset when D_004A4830 is
 * set. The message holds the table slot, the row's start (plus the repeat index times the table length when set), its
 * length, the key, the bank's sub id (SPCHI_getBankSubId) and registration key (SPCHI_getBankRegKey) and the table's +0x2
 * halfword. The row is then recorded in the table's history (SPCHI_recordHistory).
 *
 * @param phrase  the phrase template
 * @param k  the speech key
 * @param firstEntry  the first play list entry
 * @return the total length * 100 / D_004A4838 (0 without a buffer size)
 */
int SPCHI_sendPhraseLines(unsigned char *phrase, SPCHKEY *k, int firstEntry) {
    struct sShapeInfo rowInfo;
    struct sFF90Msg playMsg;
    int w30;
    int w34;
    int sum;
    int cnt;
    int n;
    int pct;
    int st;
    int tableLen;
    int x;
    int y;
    struct sPlayLine *listEntry;
    unsigned char *table;

    sum = 0;
    pct = 0;
    w34 = 0;
    if (D_0044FF90_9830.play != 0) {
        SPCHI_getBankSubId(k, &w30);
        SPCHI_getBankRegKey(k, &w34);
        n = phrase[4] >> 2;
        if (n != 0) {
            cnt = n;
            listEntry = D_00450700_9830.q + firstEntry;
            do {
                x = listEntry->x;
                table = D_004A4824_9830[x];
                y = listEntry->y;
                if ((table[4] & 0x80) && D_004A4830_9830 != 0) {
                    clearBit_9830(table, y);
                }
                if (SPCHI_readRow(table, y, &rowInfo)) {
                    tableLen = ((SPCHTABLEHDR *)table)->totalLength * ((table[7] + 1) << 8);
                    st = rowInfo.start;
                    if (listEntry->idx != -1) {
                        st = listEntry->idx * tableLen + st;
                    }
                    playMsg.start = st;
                    playMsg.id = x;
                    sum += rowInfo.len;
                    playMsg.len = rowInfo.len;
                    playMsg.k = *k;
                    playMsg.bankSubId = w30;
                    playMsg.regKey = w34;
                    playMsg.tableSubId = ((SPCHTABLEHDR *)table)->subId;
                    D_0044FF90_9830.play(&playMsg);
                    SPCHI_recordHistory(table, y);
                }
                cnt--;
                listEntry++;
            } while (cnt != 0);
        }
        pct = 0;
        if (D_004A4838_9830 != 0) {
            pct = sum * 100 / D_004A4838_9830;
        }
    }
    return pct;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_resetPlayList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Q9A40 { short a, b, c, d; };
struct R9A40 {
    int eventEntry;
    int phrase;
    unsigned char variantIdx;
    unsigned char used;
    unsigned char ba;
    unsigned char bb;
    char pad[0x5C - 12];
};
/* names ours (SPCHI_appendPhrase's use) */
struct G9A40 {
    int channel, phraseCount, lineCount;
    struct Q9A40 q[0x30];
    struct R9A40 r[8];
};
extern struct G9A40 D_00450700_9A40 __asm__("D_00450700");

/**
 * Resets the speech play list (D_00450700): channel -1, both counts 0, the 8 phrase records cleared and the 0x30 line
 * entries set to (-1, -1, 0, 0).
 */
void SPCHI_resetPlayList(void) {
    int i;
    D_00450700_9A40.channel = -1;
    D_00450700_9A40.phraseCount = 0;
    D_00450700_9A40.lineCount = 0;
    for (i = 0; i < 8; i++) {
        D_00450700_9A40.r[i].eventEntry = 0;
        D_00450700_9A40.r[i].phrase = 0;
        D_00450700_9A40.r[i].variantIdx = 0;
        D_00450700_9A40.r[i].used = 0;
        D_00450700_9A40.r[i].bb = 0;
    }
    for (i = 0; i < 0x30; i++) {
        D_00450700_9A40.q[i].a = -1;
        D_00450700_9A40.q[i].b = -1;
        D_00450700_9A40.q[i].c = 0;
        D_00450700_9A40.q[i].d = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_appendPhrase);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Q_9AC8 { short a, b, c, d; };
struct R_9AC8 {
    int eventEntry;
    unsigned char *phrase;
    char variantIdx;
    unsigned char used;
    unsigned char ba;
    unsigned char bb;
    int data[20];
};
/* names ours, from use */
struct G_9AC8 {
    int channel, phraseCount, lineCount;
    struct Q_9AC8 q[0x30];
    struct R_9AC8 r[8];
};
struct E_9AC8 { unsigned short x; unsigned short y; unsigned short z; unsigned char idx; unsigned char pad; };
struct H_9AC8 { struct E_9AC8 ent[12]; char pad[2]; unsigned char tbl[1]; };
extern struct G_9AC8 D_00450700_9AC8 __asm__("D_00450700");

/**
 * Appends a built phrase to the speech play list (D_00450700) for channel @p id, when the list is free or already that
 * channel's.
 *
 * Takes phrase record n (count +0x4, at most 8): the channel, the bank event entry, the template, the variant index and
 * the request's 20 argument words, marked used (+0x9). Each template slot then appends a line entry (table slot, the
 * slot's index value, chosen row) after the existing ones (+0x8).
 *
 * @param selSet  the selection set holding the chosen rows
 * @param id  the channel
 * @param eventEntry  the bank event entry
 * @param phrase  the phrase template (the event's variant)
 * @param variantIdx  the variant index
 * @param reqArgs  the request's argument words
 */
void SPCHI_appendPhrase(struct H_9AC8 *selSet, int id, int eventEntry, unsigned char *phrase, int variantIdx, int *reqArgs) {
    int n;
    int i;
    int cnt;
    int j;

    if (D_00450700_9AC8.channel < 0 || D_00450700_9AC8.channel == id) {
        n = D_00450700_9AC8.phraseCount++;
        if (n < 8) {
            D_00450700_9AC8.channel = id;
            D_00450700_9AC8.r[n].eventEntry = eventEntry;
            D_00450700_9AC8.r[n].phrase = phrase;
            D_00450700_9AC8.r[n].variantIdx = variantIdx;
            for (i = 0; i < 20; i++) {
                D_00450700_9AC8.r[n].data[i] = reqArgs[i];
            }
            D_00450700_9AC8.r[n].used = 1;
            cnt = phrase[4] >> 2;
            j = D_00450700_9AC8.lineCount;
            for (i = 0; i < cnt; i++) {
                D_00450700_9AC8.q[j + i].a = selSet->ent[i].x;
                D_00450700_9AC8.q[j + i].b = selSet->ent[i].y;
                D_00450700_9AC8.q[j + i].c = selSet->tbl[selSet->ent[i].idx];
            }
            D_00450700_9AC8.lineCount += cnt;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_playList);
#ifdef SKIP_ASM
// FAKEMATCH: `*&` read of the record's key (key = *&...data.key) forms its address first; plain form 94.29%.
// Byte-identical and behaves like the plain form; landed marked under the 2026-11-14 ruling (portable,
// behaviour-preserving steering only); replace it when an honest spelling matches.
// From peak3project's fork (github.com/peak3project/ssx3).
                  /* 0x01789A34 while initialised (SPCH_initDriver) */
extern unsigned char **D_004A4824_9BD8 __asm__("D_004A4824");

extern struct sFF90 D_0044FF90;

extern SPCHCHANNEL D_00450660[8];

struct s0700Req {
    int f0; unsigned char *f4; unsigned char b8; unsigned char b9; unsigned char ba; unsigned char bb; SPCHARGS data;
};

struct s0700 { int f0; int f4; int f8; struct sPlayLine q[0x30]; struct s0700Req r[8]; };

extern struct s0700 D_00450700;

// (prototype from their definition at line 1338)
int SPCHI_getBankSubId_9BD8(unsigned char *arg0, int *arg1) __asm__("SPCHI_getBankSubId");

// (prototype from their definition at line 1963)
int SPCHI_sendPhraseLines_9BD8(unsigned char *h, SPCHKEY *k, int a2) __asm__("SPCHI_sendPhraseLines");

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern void SPCHI_reportEventTags_9BD8(SPCHKEY *, int, int, int *) __asm__("SPCHI_reportEventTags");
extern int SPCHI_getTagId_9BD8(int, int) __asm__("SPCHI_getTagId");

/* The body of SPCHI_reportSlotTags, inlined. */
static __inline__ void layerHits_9BD8(void *a0, int a1, struct sLayer *l, int a3) {
    struct sShapeInfo o;
    int out;
    int i;
    int j;
    int cnt;
    unsigned char *tbl;
    unsigned char *shp;
    unsigned char b;
    int h;
    unsigned yy;
    long m;

    if (D_0044FF90.f08 != 0) {
        SPCHI_getBankSubId_9BD8(a0, &out);
        cnt = l->cnt >> 2;
        for (i = 0; i < cnt; i++) {
            tbl = (unsigned char *)l + l->list[i] * 4 + 0xC;
            for (j = 0; j < 4; j++) {
                if ((b = tbl[j]) != 0xFF) {
                    h = SPCHI_getTagId_9BD8(a1, b);
                    yy = D_00450700.q[i + a3].y;
                    shp = D_004A4824_9BD8[D_00450700.q[i + a3].x];
                    if (SPCHI_readRow(shp, yy, &o)) {
                        m = 1 << o.col[j];
                        D_0044FF90.f08(a0, h, (int)m, out);
                    }
                }
            }
        }
    }
}

/* SPCHI_hasPlayList, inlined. */
static __inline__ int reqOwned_9BD8(int x) {
    int r = 0;
    if (D_00450700.f0 == x)
        r = D_00450700.f4 > 0;
    return r;
}

int SPCHI_playList_impl(int id) __asm__("SPCHI_playList");
/**
 * Plays the speech play list when it belongs to channel @p id, then clears it (SPCHI_resetPlayList); returns the number of
 * lines sent.
 *
 * For each phrase record: the event hooks run for the event's tags (SPCHI_reportEventTags) and for each slot's chosen tags (the
 * inlined SPCHI_reportSlotTags), both through D_0044FF90 +0x8, and SPCHI_sendPhraseLines sends the lines to the play callback. The
 * first phrase updates the channel's current line (D_00450660): the same entry id counts a repeat, another one replaces
 * it with count 1.
 *
 * @param id  the channel
 * @return the number of lines sent, 0 when the list is not this channel's
 */
int SPCHI_playList_impl(int id) {
    SPCHKEY key;
    int ret = 0;
    int n;
    int k;

    if (reqOwned_9BD8(id)) {
        n = D_00450700.f4;
        for (k = 0; k < n; k++) {
            key = *&D_00450700.r[k].data.key; /* read through its address: retail forms it first */
            SPCHI_reportEventTags_9BD8(&key, D_00450700.r[k].f0, D_00450700.r[k].b8, D_00450700.r[k].data.words);
            layerHits_9BD8(&key, D_00450700.r[k].f0, (struct sLayer *)D_00450700.r[k].f4, ret);
            SPCHI_sendPhraseLines_9BD8(D_00450700.r[k].f4, &key, ret);
            ret += D_00450700.r[k].f4[4] >> 2;
            if (k == 0) {
                if (key.entryId == D_00450660[id].line.entryId) {
                    D_00450660[id].repeatCount++;
                } else {
                    D_00450660[id].line = key;
                    D_00450660[id].repeatCount = 1;
                }
            }
        }
    }
    SPCHI_resetPlayList();
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_checkConstraints);
#ifdef SKIP_ASM
/**
 * Returns whether a phrase template's set variables agree with the request: each set variable word i must share a bit
 * with request word @p masks [b] of the event's constraint i (3-byte entries after its tag events, count in the high
 * nibble of +0x7).
 *
 * @param eventEntry  the bank event entry
 * @param phrase  the phrase template
 * @param masks  the request's argument words
 * @return 1 when they agree, else 0
 */
int SPCHI_checkConstraints(SPCHEVENT *eventEntry, unsigned char *phrase, int *masks) {
    SPCHTAGWORDS constraint;
    int *varWords;
    SPCHTAG *constraints;
    int n, ok, i;

    n = SPCH_CONSTRAINT_COUNT(eventEntry->counts);
    ok = 1;
    if (n != 0) {
        // ORIGINAL-PTR: both lists sit after variable-length tables (the template's condition offsets, the entry's
        // variant offsets and tags), so their offsets come from those counts, not from a member
        varWords = (int *)(phrase + ((((phrase[4] >> 2) + 3) & 0xFFFFFFFC) + 8));
        constraints = (SPCHTAG *)((unsigned char *)eventEntry +
                                  ((((eventEntry->variantCount * 2) + 3) & 0xFFFFFFFC) + 0xC) +
                                  ((SPCH_TAG_COUNT(eventEntry->counts) * 3 + 3) & ~3));
        for (i = 0; i < n; i++) {
            constraint.id = constraints[i].id;
            constraint.argIndex = constraints[i].argIndex;
            constraint.type = constraints[i].type;
            if (varWords[i] != 0 && !(varWords[i] & masks[constraint.argIndex])) {
                ok = 0;
                break;
            }
        }
    }
    return ok;
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003D9FB0);

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getBankIds);
#ifdef SKIP_ASM
/**
 * Reads a speech bank's two ids: bytes +0x8 and +0x9.
 *
 * @param data  the speech bank data
 * @param bankIdOut  receives the bank id (+0x8)
 * @param subIdOut  receives the sub id (+0x9)
 */
void SPCHI_getBankIds(void *data, int *bankIdOut, int *subIdOut) {
    *bankIdOut = ((SPCHBANKHDR *)data)->bankId;
    *subIdOut = ((SPCHBANKHDR *)data)->subId;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCH_setClockOffset);
#ifdef SKIP_ASM
extern int D_004A483C;

/**
 * Sets the speech clock offset D_004A483C, added to the time source when SPCHI_pickRequest ages requests.
 *
 * @param timeOffset  the offset
 */
void SPCH_setClockOffset(int timeOffset) {
    D_004A483C = timeOffset;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getOptionWeight);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_004506E0_A3C8[] __asm__("D_004506E0");
/**
 * Returns an option's weight from its first byte: the low 5 bits times the scale D_004506E0 [top 3 bits] (as in
 * SPCHI_orderOptionsByWeight).
 *
 * @param weightByte  the option's first byte
 * @return the weight
 */
unsigned int SPCHI_getOptionWeight(unsigned int weightByte) {
    weightByte &= 0xFF;
    return (weightByte & 0x1F) * D_004506E0_A3C8[weightByte >> 5];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_hasPlayList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_A3F0 { int a; int b; int c; };
extern struct S_A3F0 D_00450700_A3F0 __asm__("D_00450700");
/**
 * Returns whether the speech play list belongs to channel @p x and holds phrases.
 *
 * @param x  the channel
 * @return 1 when it has phrases for @p x, else 0
 */
int SPCHI_hasPlayList(int x) {
    int pending = 0;
    if (D_00450700_A3F0.a == x)
        pending = D_00450700_A3F0.b > 0;
    return pending;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_buildRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_A418 { int pad[4]; int (*cb)(int, void *); };
extern struct G_A418 D_0044FF90_A418 __asm__("D_0044FF90");
extern SPCHREQSTATE D_0044FFF8_A418 __asm__("D_0044FFF8");
extern int func_003D9FB0_A418(SPCHARGS *) __asm__("func_003D9FB0");

/**
 * Builds speech request @p idx into the play list (func_003D9FB0); returns the result, negative results as 0.
 *
 * When nothing gets built and the retry hook (D_0044FF90 +0x10, installed by SPCH_setRetryHook) is set, the hook is called
 * with attempt 0, 1, ... and the request's argument words, and each non-negative answer is followed by another build,
 * while nothing is built and the hook returns more than 0.
 *
 * @param idx  the request record
 * @return the build result, at least 0
 */
int SPCHI_buildRequest(int idx) {
    SPCHARGS *reqArgs = &D_0044FFF8_A418.requests[idx].args;
    int result;
    int i;
    int k;

    result = func_003D9FB0_A418(reqArgs);
    if (result == 0 && D_0044FF90_A418.cb != 0) {
        i = 0;
        do {
            k = D_0044FF90_A418.cb(i, reqArgs);
            if (k >= 0) {
                result = func_003D9FB0_A418(reqArgs);
            }
            i++;
        } while (result == 0 && k > 0);
    }
    if (result > -1) {
        return result;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_bindVariables);
#ifdef SKIP_ASM
/**
 * Binds a rule's variables: for each condition of kind 0xFE (+0x8 [i]) whose target (+0x10 [i]) has bit 0x80, ORs bit
 * @p values [i] into the phrase template's variable word [target & 0x7F].
 *
 * @param node  the phrase template
 * @param rule  the condition rule (count at +0x4)
 * @param values  the chosen tag values
 */
void SPCHI_bindVariables(unsigned char *node, struct sCond *rule, unsigned char *values) {
    int i;
    signed char n;
    unsigned char cond;
    unsigned char *base;
    int *w;
    i = 0;
    n = rule->n;
    // A template's variable words follow its variable-length slot list, so their offset comes from the slot count
    // (a word's address is summed as an integer below: every pointer form reorders the add)
    base = node + ((((node[4] >> 2) + 3) & 0x7C) + 8) + node[6] * 4;
    if (n > 0) {
        unsigned char *entry = rule->b;
        do {
            if (entry[-8] == 0xFE) {
                cond = entry[0];
                if (cond & 0x80) {
                    w = (int *)((cond & 0x7F) * 4 + (int)base);
                    *w |= 1 << values[i];
                }
            }
            i++;
            entry++;
        } while (i < n);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_hasBindCondition);
#ifdef SKIP_ASM
/**
 * Returns whether any of a rule's conditions (count +0x4, kinds from +0x8) is a bind (kind 0xFE).
 *
 * @param rule  the condition rule
 * @return 1 when it binds, else 0
 */
int SPCHI_hasBindCondition(struct sCond *rule) {
    int i;
    int found = 0;
    int n = rule->n;
    for (i = 0; i < n; i++) {
        if (rule->a[i] == 0xFE) {
            found = 1;
            break;
        }
    }
    return found;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_resolveSlotTable);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
enum E_A5C8 { E_A5C8_0, E_A5C8_1, E_A5C8_2 };
/* A request argument word; its low halfword alone is a table's sub id or an entry index. */
typedef union SPCHARGWORD {
    unsigned int word;
    unsigned short low;
} SPCHARGWORD;
/**
 * Resolves a phrase slot's speech table from its reference; the out-of-line copy of the step SPCHI_buildCandidates inlines.
 *
 * Kind @p ref [3] 0 finds the table by id (SPCHI_findTableById); 1 by id and the halfword of argument word [@p ref [2]]
 * (SPCHI_findTableBySubId, index below 20); 2 by id, with that argument word as an entry index that must be in range
 * (SPCHI_isTableEntryInRange) and is stored in @p out [1].
 *
 * @param ref  the slot reference {id, argument index, kind}
 * @param tbl  the request's argument words
 * @param out  receives the table slot and the entry index (-1 when unused)
 * @return 1 when a table was found, else 0
 */
int SPCHI_resolveSlotTable(struct sCond *ref, SPCHARGWORD *tbl, short *out) {
    unsigned int idx = ref->idx;
    int kind = ref->mode;
    unsigned short id = ref->id;
    int handle;
    out[1] = -1;
    out[0] = -1;
    switch ((enum E_A5C8)kind) {
    case 0:
        out[0] = SPCHI_findTableById(id);
        break;
    case 1:
        if (idx >= 20) {
            out[0] = -1;
        } else {
            out[0] = SPCHI_findTableBySubId(id, tbl[idx].low);
        }
        break;
    case 2:
        handle = SPCHI_findTableById(id);
        if (SPCHI_isTableEntryInRange(handle, tbl[idx].word) != 0) {
            out[1] = tbl[idx].low;
        } else {
            handle = -1;
        }
        out[0] = handle;
        break;
    }
    if (out[0] > -1) return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_removeFromSubset);
#ifdef SKIP_ASM
/**
 * Clears row @p index's bit in a random-subset speech table's subset mask (marks it used), as SPCHI_sendPhraseLines does
 * inline.
 *
 * @param node  the speech table
 * @param index  the row
 */
void SPCHI_removeFromSubset(unsigned char *node, int index) {
    int colCount;
    int byteIndex;
    unsigned char *maskByte;

    colCount = node[4] & 0xF;
    byteIndex = index / 8;
    maskByte = node + ((node[5] * (colCount + 2) + 0xF) & 0x3FFC) + colCount * 4 + (byteIndex + 1);
    *maskByte &= ~(1 << (index - byteIndex * 8));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_testBit);
#ifdef SKIP_ASM
/**
 * Returns bit @p index of a byte-packed bit array (nonzero when set).
 *
 * @param bits  the bit array
 * @param index  the bit
 * @return the masked byte, nonzero when the bit is set
 */
int SPCHI_testBit(unsigned char *bits, int index) {
    int temp_3 = index / 8;
    unsigned char mask = 1 << (index - temp_3 * 8);
    return bits[temp_3] & mask;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_isTransitionAllowed);
#ifdef SKIP_ASM
/**
 * Returns the transition bit for row @p bit after the previous row (@p state +0x2) in a speech table's matrix: the test
 * SPCHI_isRowAllowed inlines. 0 when the previous row is not below the entry count (+0xA).
 *
 * @param state  the selection state
 * @param node  the speech table
 * @param bit  the row
 * @return nonzero when the transition is allowed
 */
int SPCHI_isTransitionAllowed(short *state, unsigned char *node, int bit) {
    int current = state[1];
    int res = 0;
    if (current < ((SPCHTABLEHDR *)node)->entryCount) {
        int k = 0;
        unsigned char *matrix, *rowsEnd;
        unsigned char mask;
        int idx;
        bit += node[5] * current;
        rowsEnd = node + ((node[5] * ((node[4] & 0xF) + 2) + 0xF) & 0x3FFC) + (node[4] & 0xF) * 4;
        if (node[4] & 0x80) {
            k = (node[5] + 7) / 8 + 1;
        }
        matrix = rowsEnd + k;
        if (node[6]) {
            matrix += node[6] + 1;
        }
        idx = bit / 8;
        mask = 1 << (bit - idx * 8);
        res = matrix[idx] & mask;
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_addCandidate);
#ifdef SKIP_ASM

/**
 * Appends a row to a selection set's candidate list (count +0x61, at most 200; rows from +0x62).
 *
 * @param list  the selection set
 * @param value  the row
 * @return 1 when added, 0 when the list is full
 */
int SPCHI_addCandidate(struct sSelSet *list, int value) {
    int added = 0;
    if (list->ntbl < 200U) {
        added = 1;
        list->tbl[list->ntbl] = value;
        list->ntbl++;
    }
    return added;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_isTableSuitable);
#ifdef SKIP_ASM
/**
 * Returns whether a speech table suits a slot condition: for each table column whose condition kind (+0x8 [i]) names an
 * argument word (not 0, 0xFE or 0xFF), the table's column mask (after its rows) must share a bit with that word.
 *
 * @param sel  the slot condition
 * @param node  the speech table
 * @param masks  the request's argument words
 * @return 1 when it suits, else 0
 */
int SPCHI_isTableSuitable(struct sCond *sel, unsigned char *node, int *masks) {
    int *colMasks;
    int n;
    int ok;
    int i;
    unsigned char value;

    ok = 1;
    n = node[4] & 0xF;
    i = 0;
    // ORIGINAL-PTR: a speech table's column masks follow its variable-length rows, so their offset comes from the
    // row and column counts
    colMasks = (int *)(node + ((node[5] * (n + 2) + 0xF) & 0x3FFC));
    if (n != 0) {
        do {
            value = sel->a[i];
            if (value != 0 && value != 0xFF && value != 0xFE) {
                if (!(colMasks[i] & masks[value])) {
                    ok = 0;
                    break;
                }
            }
            i++;
        } while (i < n);
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_lengthToPercent);
#ifdef SKIP_ASM
extern int D_004A4838;

/**
 * Returns a length as a percentage of the speech buffer size D_004A4838 (0 when the size is 0).
 *
 * @param len  the length
 * @return the percentage
 */
int SPCHI_lengthToPercent(int len) {
    int var_2;

    var_2 = 0;
    if (D_004A4838 != 0) {
        var_2 = (int) (len * 100) / (int) D_004A4838;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getSelectionPercent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SPCHI_readRow_A910(unsigned char *, int, struct sShapeInfo *) __asm__("SPCHI_readRow");
extern unsigned char **D_004A4824_A910 __asm__("D_004A4824");
extern int D_004A4838;

/**
 * Returns the total length of a selection's current rows as a percentage of the speech buffer size D_004A4838; the
 * out-of-line copy of the helper SPCHI_findFittingCombination inlines.
 *
 * @param phrase  the phrase template
 * @param selSet  the selection set
 * @return the percentage
 */
int SPCHI_getSelectionPercent(unsigned char *phrase, struct sSelSet *selSet) {
    struct sShapeInfo rowInfo;
    int total = 0;
    int i;
    int pct;
    int cnt = phrase[4] >> 2;

    for (i = 0; i < cnt; i++) {
        if (SPCHI_readRow_A910(D_004A4824_A910[selSet->ent[i].x], selSet->tbl[selSet->ent[i].cur], &rowInfo)) {
            total += rowInfo.len;
        }
    }
    pct = 0;
    if (D_004A4838 != 0) {
        pct = total * 100 / D_004A4838;
    }
    return pct;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_canRepeatEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_A9C8 { unsigned short h; int w; };
extern struct E_A9C8 D_00450660_A9C8[] __asm__("D_00450660");

/**
 * Returns whether a bank entry may play again on a channel: 0 when it is the channel's current line (D_00450660) and
 * its repeat limit (+0x8, when above 0) has been reached.
 *
 * @param eventEntry  the bank event entry
 * @param idx  the channel
 * @return 1 when allowed, else 0
 */
int SPCHI_canRepeatEntry(char *eventEntry, int idx) {
    int allowed = 1;
    if (*(unsigned short *)eventEntry == D_00450660_A9C8[idx].h) {
        int maxRepeat = eventEntry[8];
        if (maxRepeat > 0) {
            if (!(D_00450660_A9C8[idx].w < maxRepeat)) {
                allowed = 0;
            }
        }
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_isTemplateModeOk);
#ifdef SKIP_ASM
/**
 * Returns whether a phrase template's kind (+0x4 & 3) suits a request mode: kind 0 always, 1 only in mode 2, 2 in any
 * other mode. The check SPCHI_buildCandidates inlines.
 *
 * @param node  the phrase template
 * @param mode  the request mode
 * @return 1 when it suits, else 0
 */
int SPCHI_isTemplateModeOk(struct sLayer *node, int mode) {
    int kind;

    kind = node->cnt & 3;
    if (((kind == 1) && (mode == 2)) || ((kind == 2) && (mode != kind))) {
        return 1;
    }
    return kind == 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_clearVariables);
#ifdef SKIP_ASM
/**
 * Clears a phrase template's variable words (+0x5 of them, after the slot list and +0x6 words).
 *
 * @param node  the phrase template
 */
void SPCHI_clearVariables(unsigned char *node) {
    int *maskTable;
    int i;
    // ORIGINAL-PTR: a template's variable words follow its variable-length slot list, so their offset comes from
    // the slot count
    maskTable = (int *)(node + ((((node[4] >> 2) + 3) & 0x7C) + 8) + (node[6] * 4));
    for (i = 0; i < node[5]; i++) {
        maskTable[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_hasRequiredVariables);
#ifdef SKIP_ASM
/**
 * Returns whether every variable a rule requires (kind 0xFF with target bit 0x80) is already set in the phrase
 * template; the check SPCHI_buildCandidates inlines.
 *
 * @param node  the phrase template
 * @param rule  the condition rule
 * @return 1 when all are set, else 0
 */
int SPCHI_hasRequiredVariables(unsigned char *node, struct sCond *rule) {
    int ok = 1;
    int i;
    int *tab;
    int n = rule->n;
    int cond;
    for (i = 0; i < n; i++) {
        if (rule->a[i] == 0xFF) {
            cond = rule->b[i];
            if (cond != 0xFF && (cond & 0x80)) {
                // ORIGINAL-PTR: a template's variable words follow its variable-length slot list, so their offset
                // comes from the slot count
                tab = (int *)(node + ((((node[4] >> 2) + 3) & 0xFFFFFFFC) + 8) + ((node[6] * 4 + 3) & 0xFFFFFFFC));
                if (tab != 0) {
                    cond &= 0x7F;
                    if (tab[cond] == 0) {
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_shuffleCandidates);
#ifdef SKIP_ASM
struct E_AB58 { char pad[4]; unsigned char off; unsigned char cnt; char pad2[2]; };
struct H_AB58 { struct E_AB58 ent[12]; char pad[2]; unsigned char tbl[1]; };
extern int SPCHI_random(int, int);

/**
 * Shuffles each slot's candidate rows in a selection set (Fisher-Yates with SPCHI_random), as SPCHI_chooseRows does
 * inline.
 *
 * @param node  the phrase template (slot count in +0x4 >> 2)
 * @param selSet  the selection set
 */
void SPCHI_shuffleCandidates(unsigned char *node, struct H_AB58 *selSet) {
    int i = 0;
    int j;
    int n = node[4] >> 2;
    struct E_AB58 *entry;
    unsigned char *items;
    unsigned char *pick;
    unsigned char swap;

    for (; i < n; i++) {
        entry = &selSet->ent[i];
        items = &selSet->tbl[entry->off];
        for (j = entry->cnt; j >= 2; j--) {
            pick = &items[SPCHI_random(j, -1)];
            swap = items[j - 1];
            items[j - 1] = *pick;
            *pick = swap;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_nextCombination);
#ifdef SKIP_ASM
struct E_AC20 {
    int pad;
    unsigned char start;
    unsigned char len;
    unsigned char cur;
    unsigned char pad2;
};
/**
 * Steps a selection's slots to the next combination of candidates, last slot fastest (an odometer); returns 1 when it
 * wrapped past the last combination. The step SPCHI_findFittingCombination inlines.
 *
 * @param node  the phrase template (slot count in +0x4 >> 2)
 * @param slots  the selection slots
 * @return 1 on wrap, else 0
 */
int SPCHI_nextCombination(unsigned char *node, struct E_AC20 *slots) {
    int wrap = 0;
    int done = 0;
    int i = (node[4] >> 2) - 1;
    struct E_AC20 *slot = &slots[i];
    int length = slot->len;
    int limit = slot->start + length;
    do {
        if (++slot->cur < limit) {
            done = 1;
        } else {
            slot->cur = slot->start;
            if (--i < 0) {
                done = 1;
                wrap = 1;
            }
            slot = &slots[i];
            length = slot->len;
            limit = slot->start + length;
        }
    } while (!done);
    return wrap;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_pickRandomCandidate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SPCHI_random_ACA8(unsigned char, short) __asm__("SPCHI_random");

/**
 * Picks a random candidate for a selection slot: current (+0x6) = first (+0x4) + SPCHI_random(count +0x5, keyed by the
 * slot's table +0x0).
 *
 * @param selSlot  the selection slot
 * @return 1
 */
int SPCHI_pickRandomCandidate(struct sSel *selSlot) {
    selSlot->cur = (signed char)(selSlot->start + SPCHI_random_ACA8(selSlot->len, selSlot->x));
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_storeBoundTags);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SPCHI_getBankUsage_ACE8(SPCHKEY *, unsigned char **) __asm__("SPCHI_getBankUsage");
extern int SPCHI_readRow_ACE8(unsigned char *, int, struct sShapeInfo *) __asm__("SPCHI_readRow");
extern unsigned char **D_004A4824_ACE8 __asm__("D_004A4824");

/**
 * Stores the chosen rows' tags for a phrase's bind columns (kind 0xFE without a variable) in the usage area of the bank
 * for @p speechKey (SPCHI_getBankUsage); the step SPCHI_chooseRows inlines.
 *
 * @param speechKey  the speech key
 * @param selSet  the selection set
 * @param phrase  the phrase template
 */
void SPCHI_storeBoundTags(SPCHKEY *speechKey, struct sSelSet *selSet, struct sLayer *phrase) {
    struct sShapeInfo rowInfo;
    unsigned char *out;
    int n;
    int i;
    int j;
    struct sCond *slotCond;

    if (SPCHI_getBankUsage_ACE8(speechKey, &out) != 0) {
        i = 0;
        n = phrase->cnt >> 2;
        for (; i < n; i++) {
            // ORIGINAL-PTR: a template's slot conditions lie at the word offsets its slot list gives (a variable-length
            // record), so they have no fixed member
            slotCond = (struct sCond *)((int *)phrase + phrase->list[i]);
            for (j = 0; j < slotCond->n; j++) {
                if (slotCond->a[j] == 0xFE && !(slotCond->b[j] & 0x80)) {
                    if (SPCHI_readRow_ACE8(D_004A4824_ACE8[selSet->ent[i].x], selSet->tbl[selSet->ent[i].cur],
                                           &rowInfo)) {
                        out[slotCond->b[j]] = rowInfo.col[j];
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_reportSlotTags);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char **D_004A4824_AE48 __asm__("D_004A4824");      /* shape table, D_004A4828 entries */
extern struct sFF90 D_0044FF90_AE48 __asm__("D_0044FF90");
struct G_AE48 { int f0, f4, f8; struct sPlayLine q[0x30]; };
extern struct G_AE48 D_00450700_AE48 __asm__("D_00450700");
int SPCHI_readRow(unsigned char *shp, int idx, struct sShapeInfo *out);
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SPCHI_getTagId_AE48(int, int) __asm__("SPCHI_getTagId");

/**
 * Reports a phrase's tag hits to the event hook D_0044FF90 +0x8 (installed by SPCH_setTagHooks); SPCHI_playList inlines it.
 *
 * For each slot of @p phrase and each of the 4 tag bytes at its condition's +0xC that is not 0xFF, the event entry's tag
 * (SPCHI_getTagId) is reported with the bit of the matching column of the slot's chosen row (the play list entry
 * @p firstEntry + slot) and the bank's sub id (SPCHI_getBankSubId).
 *
 * @param speechKey  the speech key
 * @param eventEntry  the bank event entry
 * @param phrase  the phrase template
 * @param firstEntry  the phrase's first play list entry
 */
void SPCHI_reportSlotTags(void *speechKey, int eventEntry, struct sLayer *phrase, int firstEntry) {
    struct sShapeInfo rowInfo;
    int out;
    int i;
    int j;
    int cnt;
    unsigned char *tbl;
    unsigned char *shp;
    unsigned char tagRef;
    int tagId;
    unsigned yy;
    long tagBit;

    if (D_0044FF90_AE48.f08 != 0) {
        SPCHI_getBankSubId(speechKey, &out);
        cnt = phrase->cnt >> 2;
        for (i = 0; i < cnt; i++) {
            // ORIGINAL-PTR: a template's slot conditions lie at the word offsets its slot list gives (a variable-length
            // record), so they have no fixed member
            tbl = (unsigned char *)phrase + phrase->list[i] * 4 + 0xC;
            for (j = 0; j < 4; j++) {
                tagRef = tbl[j];
                if (tagRef != 0xFF) {
                    tagId = SPCHI_getTagId_AE48(eventEntry, tagRef);
                    yy = D_00450700_AE48.q[i + firstEntry].y;
                    shp = D_004A4824_AE48[D_00450700_AE48.q[i + firstEntry].x];
                    if (SPCHI_readRow(shp, yy, &rowInfo)) {
                        tagBit = 1 << rowInfo.col[j];
                        D_0044FF90_AE48.f08(speechKey, tagId, (int)tagBit, out);
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_clearSelectionSet);
#ifdef SKIP_ASM
/**
 * Clears a selection set: both counts (+0x60, +0x61) and the 12 slots' done flags.
 *
 * @param selSet  the selection set
 */
void SPCHI_clearSelectionSet(struct sSelSet *selSet) {
    int i;

    selSet->ntbl = 0;
    selSet->n = 0;
    for (i = 11; i >= 0; i--) {
        selSet->ent[i].flag = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_rollChance);
#ifdef SKIP_ASM
extern int SPCHI_random(int, int);

/**
 * Rolls a percentage chance: returns 1 when SPCHI_random(100, -1) is below byte +0x1 of @p chanceRec.
 *
 * @param chanceRec  the record holding the chance in percent at +0x1
 * @return 1 when the roll passes, else 0
 */
int SPCHI_rollChance(unsigned char *chanceRec) {
    return SPCHI_random(100, -1) < (int) chanceRec[1];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_reportEventTags);
#ifdef SKIP_ASM
struct S_B040 {
    int a;
    int b;
    void (*cb)(SPCHKEY *, int, int, int);
};
extern struct S_B040 D_0044FF90_B040 __asm__("D_0044FF90");


/**
 * Reports a bank event's tag events to the event hook D_0044FF90 +0x8 (installed by SPCH_setTagHooks).
 *
 * Does nothing without the hook. The event's tag entries (3 bytes: tag id, argument index, type; count in the low
 * nibble of +0x7, after the +0x6 variant offsets) of type 0 or 3 are reported when variant @p idx's template uses that
 * argument (the test SPCHI_templateUsesArg makes): hook(key, tag id, argument word, the bank's sub id from SPCHI_getBankSubId).
 *
 * @param ctx  the speech key
 * @param data  the bank event entry
 * @param idx  the variant index
 * @param tbl  the request's argument words
 */
void SPCHI_reportEventTags(SPCHKEY *ctx, unsigned char *data, int idx, int *tbl) {
    SPCHTAGWORDS ev;
    int time;
    int count;
    int i;
    unsigned char *track;
    unsigned char *events;
    if (D_0044FF90_B040.cb != 0) {
        SPCHI_getBankSubId(ctx, &time);
        count = data[7] & 0xF;
        track = data + ((SPCHEVENT *)data)->variantOffsets[idx] * 4;
        events = data + (((data[6] * 2 + 3) & 0x3FC) + 0xC);
        for (i = 0; i < count; i++) {
            int type;
            int tagId;
            int argIndex;
            /* 3-byte tag entries (id, argument index, type) read straight off the subscript: retail adds index first */
            ev.id = events[i * 3];
            ev.argIndex = events[i * 3 + 1];
            ev.type = events[i * 3 + 2];
            type = ev.type;
            tagId = ev.id;
            argIndex = ev.argIndex;
            switch (type) {
            case 0:
            case 3: {
                int n = track[4] >> 2;
                int found = 0;
                int k;
                for (k = 0; k < n; k++) {
                    unsigned char *ent = track + (track + k)[8] * 4;
                    int condIdx;
                    if (ent[2] == argIndex) {
                        found = 1;
                        break;
                    }
                    for (condIdx = 0; condIdx < 4; condIdx++) {
                        if ((ent + condIdx)[8] == argIndex) {
                            found = 1;
                            break;
                        }
                    }
                }
                if (found)
                    D_0044FF90_B040.cb(ctx, tagId, tbl[argIndex], time);
                break;
            }
            case 1:
            case 2:
            case 4:
            case 5:
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_askTagConditions);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct F_B208 { int pad; int (*cb)(SPCHKEY *, int, int, int); int pad2[3]; };
extern struct F_B208 D_0044FF90_B208 __asm__("D_0044FF90");

/**
 * Asks the condition hook (D_0044FF90 +0x4) about each of a bank event's tags; returns the bits of those that hold.
 *
 * Tags are taken by argument index 1 to 19 (the hook gets that argument word), then those with index 0 (value 0). Tag j
 * answers in bit 7 - j: a positive answer sets it in the result; a negative one, or no hook, sets it in
 * @p negmask instead.
 *
 * @param speechKey  the speech key
 * @param eventEntry  the bank event entry
 * @param vals  the request's argument words
 * @param negmask  receives the bits of tags the hook could not answer
 * @return the bits of the tags that hold
 */
int SPCHI_askTagConditions(SPCHKEY *speechKey, SPCHEVENT *eventEntry, int *vals, unsigned char *negmask) {
    SPCHTAGWORDS tagEnt;
    int out;
    int res = 0;
    unsigned char neg = 0;
    int k = 1;
    int n;
    int j;
    int argValue;
    int hookResult;
    int bit;
    int tagId;
    unsigned char mask;
    SPCHTAG *tbl;

    SPCHI_getBankSubId(speechKey, &out);
    n = SPCH_TAG_COUNT(eventEntry->counts);
    // ORIGINAL-PTR: the tags follow the variable-length variant-offset table, so their offset comes from its count
    tbl = (SPCHTAG *)((unsigned char *)eventEntry + (((eventEntry->variantCount * 2 + 3) & 0x3FC) + 0xC));
    for (; k < 21; k++) {
        for (j = 0; j < n; j++) {
            bit = 0;
            tagEnt.id = tbl[j].id;
            tagEnt.argIndex = tbl[j].argIndex;
            tagEnt.type = tbl[j].type;
            if (k == 20) {
                if (tagEnt.argIndex != 0) continue;
                argValue = 0;
            } else {
                if (tagEnt.argIndex != k) continue;
                argValue = vals[k];
            }
            mask = 1 << (7 - j);
            tagId = tagEnt.id;
            if (D_0044FF90_B208.cb != 0) {
                hookResult = D_0044FF90_B208.cb(speechKey, tagId, argValue, out);
            } else {
                hookResult = -1;
            }
            if (hookResult == 0) {
                bit = 0;
            } else if (hookResult > 0) {
                bit = mask;
            } else {
                neg |= mask;
            }
            res |= bit;
        }
    }
    *negmask = neg;
    return res;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_matchFlags);
#ifdef SKIP_ASM
/**
 * Returns whether a request's flag byte matches a phrase template's flag value (+0x3) on its care bits (+0x2) minus
 * @p ignoreMask.
 *
 * @param flags  the request's flag byte
 * @param ignoreMask  bits not compared
 * @param node  the phrase template
 * @return 1 when they match, else 0
 */
int SPCHI_matchFlags(unsigned char flags, unsigned char ignoreMask, unsigned char *node) {
    int careMask = node[2] & ~ignoreMask;
    return ((node[3] ^ flags) & careMask) == 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getTagId);
#ifdef SKIP_ASM
/**
 * Returns the tag id (first byte) of a bank event's tag entry @p idx, or -1 when @p idx is 8 or more.
 *
 * @param header  the bank event entry
 * @param idx  the tag entry
 * @return the tag id, or -1
 */
int SPCHI_getTagId(SPCHEVENT *header, unsigned int idx) {
    int triple[3];
    int result = -1;
    // ORIGINAL-PTR: the tags follow the variable-length variant-offset table, so their offset comes from its count
    unsigned char *tbl = (unsigned char *)header + (((header->variantCount * 2 + 3) & 0x3FC) + 0xC);

    if (idx < 8) {
        unsigned char *entry = (unsigned char *)(idx * 3 + (unsigned)tbl); // PORT: ptr in int
        triple[0] = entry[0];
        triple[1] = entry[1];
        triple[2] = entry[2];
        result = triple[0];
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_templateUsesArg);
#ifdef SKIP_ASM
/**
 * Returns whether any slot of a phrase template uses argument word @p wantedId, as its table argument (+0x2) or in one
 * of its four condition kinds (+0x8); the test SPCHI_reportEventTags inlines.
 *
 * @param node  the phrase template
 * @param wantedId  the argument index
 * @return 1 when used, else 0
 */
int SPCHI_templateUsesArg(unsigned char *node, int wantedId) {
    int i, j;
    int found = 0;
    int n = node[4] >> 2;
    unsigned char *entry;
    for (i = 0; i < n; i++) {
        entry = node + *(node + i + 8) * 4;
        if (entry[2] == wantedId) {
            found = 1;
            break;
        }
        for (j = 0; j < 4; j++) {
            if (*(entry + j + 8) == wantedId) {
                found = 1;
                break;
            }
        }
    }
    return found;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_nextRandom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_00450B70_B4D0[6] __asm__("D_00450B70");

/**
 * Advances the speech random generator and returns its top word.
 *
 * The state is six words at D_00450B70: the last one is incremented every call and added, with carries, into the words
 * above it.
 *
 * @return the new top word
 */
unsigned int SPCHI_nextRandom(void) {
    unsigned int sum, carry, n;

    sum = D_00450B70_B4D0[5] + D_00450B70_B4D0[4];
    carry = 0;
    if (sum < D_00450B70_B4D0[5] || sum < D_00450B70_B4D0[4]) {
        carry = 1;
    }
    D_00450B70_B4D0[4] = sum;
    sum = sum + D_00450B70_B4D0[3] + carry;
    carry = sum < D_00450B70_B4D0[3];
    D_00450B70_B4D0[3] = sum;
    sum = sum + D_00450B70_B4D0[2] + carry;
    carry = sum < D_00450B70_B4D0[2];
    D_00450B70_B4D0[2] = sum;
    n = ++D_00450B70_B4D0[5];
    sum = sum + D_00450B70_B4D0[1] + carry;
    carry = sum < D_00450B70_B4D0[1];
    D_00450B70_B4D0[1] = sum;
    sum = sum + D_00450B70_B4D0[0] + carry;
    D_00450B70_B4D0[0] = sum;
    if (n == 0) {
        if (++D_00450B70_B4D0[4] == 0) {
            if (++D_00450B70_B4D0[3] == 0) {
                if (++D_00450B70_B4D0[2] == 0) {
                    if (++D_00450B70_B4D0[1] == 0) {
                        sum = ++D_00450B70_B4D0[0];
                    }
                }
            }
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_random);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
typedef struct { unsigned short val; unsigned short key; } Hist_B5C0;
extern Hist_B5C0 D_00450B88_B5C0[] __asm__("D_00450B88");
extern int D_004A4858;
extern unsigned int SPCHI_nextRandom(void);

/**
 * Returns a random number below @p range; with a key it tries to avoid the key's recent results.
 *
 * Without a key (-1) one draw is returned. Otherwise up to 32 draws are made until one is not among the last min(@p
 * range / 2, 10) results kept for that key in the 32-entry history (D_00450B88), keeping the draw whose repeat is
 * oldest; the kept value is recorded under the key. Quirk: it returns the last draw rather than the kept value, so
 * after 32 repeating draws the history records a value that was not returned.
 *
 * @param range  the number of values
 * @param key  the history key (a table slot), or -1
 * @return the drawn value
 */
int SPCHI_random(int range, int key) {
    int half = range / 2;
    int tries = 0;
    int done = 0;
    int kept;
    int draw;
    int best = -1;
    int n;
    int idx;
    int found;
    int i;
    int k;
    int stop;
    int kk;

    if (half >= 11) half = 10;
    kept = (SPCHI_nextRandom() >> 16) * range >> 16;
    if (key == -1) return kept;
    kk = key & 0xFFFF;
    do {
        draw = (SPCHI_nextRandom() >> 16) * range >> 16;
        n = half;
        if (n > 32) n = 32;
        idx = D_004A4858;
        found = -1;
        i = 0;
        k = 0;
        stop = 0;
        do {
            if (i < n) {
                if (D_00450B88_B5C0[idx].key == kk) {
                    i++;
                    if (D_00450B88_B5C0[idx].val == draw) {
                        stop = 1;
                        found = i - 1;
                    }
                }
            } else {
                stop = 1;
            }
            idx--;
            k++;
            if (idx < 0) idx += 32;
            if (k >= 33) stop = 1;
        } while (!stop);
        if (found == -1) {
            kept = draw;
            done = 1;
        } else if (found > best) {
            best = found;
            kept = draw;
        }
        tries++;
        if (tries >= 32) done = 1;
    } while (!done);
    D_004A4858++;
    if (D_004A4858 >= 32) D_004A4858 = 0;
    D_00450B88_B5C0[D_004A4858].val = kept;
    D_00450B88_B5C0[D_004A4858].key = kk;
    return draw;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_seedRandom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_B790 {
    unsigned short a;
    unsigned short b;
};
extern struct P_B790 D_00450B88_B790[32] __asm__("D_00450B88");
extern unsigned int D_00450B70_B790[6] __asm__("D_00450B70");
extern int D_004A4858;

/**
 * Seeds the speech random generator from @p seed (D_00450B70, six words from seed + (seed << 16) and fixed constants)
 * and clears its 32-entry history (D_00450B88, 0xFFFF) and position (D_004A4858).
 *
 * @param seed  the seed (SPCH_Init's "rNum")
 */
void SPCHI_seedRandom(int seed) {
    int i;
    unsigned int state;

    for (i = 0; i < 32; i++) {
        D_00450B88_B790[i].a = 0xFFFF;
        D_00450B88_B790[i].b = 0xFFFF;
    }
    seed += seed << 16;
    state = seed + 0xF22D0E56;
    D_00450B70_B790[0] = state;
    state += 0x96041893;
    D_00450B70_B790[1] = state;
    state += 0x3DF3B646;
    D_00450B70_B790[2] = state;
    state += 0x40DDE76D;
    D_00450B70_B790[3] = state;
    state += 0x97327AE1;
    D_00450B70_B790[4] = state;
    state += 0xD1A9FBE7;
    D_004A4858 = 0;
    D_00450B70_B790[5] = state;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_readRow);
#ifdef SKIP_ASM
/**
 * Reads row @p idx of a speech table: its sample start, length and column tags; returns 1, or 0 for a row past the row
 * count (+0x5).
 *
 * A row is 2 start bytes then one tag per column (+0x4 low nibble). An alias row (start bit 0x80) takes its start from
 * the row its second byte names. Starts are scaled by (+0x7 + 1) << 8; the length runs to the next row that is not an
 * alias, or to the table's total (+0x8) after the last row. @p info is reset first (start -1, tags 0xFF).
 *
 * @param shp  the speech table
 * @param idx  the row
 * @param info  receives length, start, column count and tags
 * @return 1 when the row exists, else 0
 */
int SPCHI_readRow(unsigned char *shp, int idx, struct sShapeInfo *info) {
    unsigned char *row;
    unsigned char *next;
    int n, stride, i, scale, start, cnt;
    int res = 0;
    int done = 0;
    int end = 0;

    info->start = -1;
    info->len = 0;
    info->n = 0;
    info->col[0] = 0xFF;
    info->col[1] = 0xFF;
    info->col[2] = 0xFF;
    info->col[3] = 0xFF;
    if (idx < shp[5]) {
        row = shp + 0xC;
        n = shp[4] & 0xF;
        stride = n + 2;
        info->n = n;
        row += idx * stride;
        for (i = 0; i < n; i++) {
            info->col[i] = row[i + 2];
        }
        if (row[0] & 0x80) {
            idx = row[1];
            row = shp + 0xC;
            row += idx * stride;
        }
        next = row + stride;
        idx++;
        scale = (shp[7] + 1) << 8;
        start = (row[0] << 8) + row[1];
        start *= scale;
        info->start = start;
        cnt = shp[5];
        while (done == 0) {
            if (idx >= cnt) {
                done = 1;
                end = ((SPCHTABLEHDR *)shp)->totalLength * scale;
            } else if (next[0] & 0x80) {
                next += stride;
                idx++;
            } else {
                done = 1;
                end = (next[0] << 8) + next[1];
                end *= scale;
            }
        }
        info->len = end - start;
        res = 1;
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_resetRowInfo);
#ifdef SKIP_ASM
/**
 * Resets a speech row info: length 0, start -1, column count 0, tags 0xFF.
 *
 * @param info  the row info
 */
void SPCHI_resetRowInfo(struct sShapeInfo *info) {
    info->start = -1;
    info->len = 0;
    info->n = 0;
    info->col[0] = 0xFF;
    info->col[1] = 0xFF;
    info->col[2] = 0xFF;
    info->col[3] = 0xFF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", SPCHI_getClock);
#ifdef SKIP_ASM
extern int (*D_0044FFB0[])();

/**
 * Returns the speech clock: the time source D_0044FFB0 (D_0044FFA8 +0x8, TIMER_gettick from SPCH_Init), or 0 when none
 * is set.
 *
 * @return the time
 */
int SPCHI_getClock(void) {
    int now = 0;
    if (D_0044FFB0[0] != 0) {
        now = D_0044FFB0[0]();
    }
    return now;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", LOCALE_getstate);
#ifdef SKIP_ASM
/* little-endian 32- and 16-bit file fields, read byte by byte */
static __inline__ int get32_B9D8(const void *field) {
    const unsigned char *q = field;
    return (q[3] << 24) | (q[2] << 16) | (q[1] << 8) | q[0];
}
static __inline__ int get16_B9D8(const void *field) {
    const unsigned char *q = field;
    return (q[1] << 8) | q[0];
}
/**
 * Returns a value of a locale (.loc) file's header, read little-endian: state 0 the current languageid, 1 the
 * languagecount, 2 the current language's stringcount (its LOCALE_LANGUAGE chunk's offset is entry [languageid] of the
 * table after the header), 3 whether it has an index (bit 0 of the flags); 0 for other states. cLocaleFile_loadFile
 * reads state 2.
 *
 * @param table  the locale file
 * @param sel  the state
 * @return the value
 */
int LOCALE_getstate(unsigned char *table, int sel) {
    LOCALE_HEADER *header = (LOCALE_HEADER *)table;
    LOCALE_LANGUAGE *language;

    switch (sel) {
    case LOCALE_LANGUAGEID:
        return get16_B9D8(&header->languageid);
    case LOCALE_LANGUAGECOUNT:
        return get16_B9D8(&header->languagecount);
    case LOCALE_STRINGCOUNT:
        // ORIGINAL-PTR: a locale file blob: the per-language offset table follows the header chunk and holds byte
        // offsets from the file start.
        language = (LOCALE_LANGUAGE *)(table + get32_B9D8((unsigned int *)(header + 1) + header->languageid));
        return get32_B9D8(&language->stringcount);
    case LOCALE_HASINDEX:
        return get32_B9D8(&header->flags) & 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", LOCALE_setstate);
#ifdef SKIP_ASM
/**
 * Sets a value of a locale file's header: only LOCALE_LANGUAGEID (0), the current languageid, is writable (stored, then
 * rewritten little-endian byte by byte).
 *
 * @param table  the locale file
 * @param which  the state
 * @param value  the new value
 */
void LOCALE_setstate(void *table, int which, unsigned int value) {
    LOCALE_HEADER *header = table;
    if (which == LOCALE_LANGUAGEID) {
        unsigned char *field = (unsigned char *)&header->languageid;
        header->languageid = value;
        field[0] = value;
        field[1] = value >> 8;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", compare);
#ifdef SKIP_ASM
/**
 * Compares a string id with a locale index entry's little-endian id halfword (the binary search's comparator in
 * getstringidbyindex).
 *
 * @param key  the string id
 * @param entry  the index entry
 * @return the difference, negative when @p key sorts first
 */
int compare(unsigned short *key, void *entry) {
    unsigned char *entryId = entry;
    return *key - ((entryId[1] << 8) | entryId[0]);
}
#endif
