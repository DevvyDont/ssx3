#include "common.h"

/* SIF RPC client data (the SDK's sceSifClientData, 0x28 bytes); `server` is non-zero once bound. */
typedef struct {
    int rpcd[4];
    unsigned int command;
    void *buff;
    void *cbuff;
    void (*end_function)(void *);
    void *end_param;
    void *server;
} sSifClient;

/* A DMA channel's registers (Dn_CHCR +0x00, Dn_MADR +0x10, Dn_QWC +0x20, Dn_TADR +0x30). The wait routines
   poll CHCR.STR and MADR while a transfer runs, so those two are volatile (tools/gen/phaseb_overrides.c). */
typedef struct {
    volatile int chcr; int p0[3];
    volatile int madr; int p1[3];
    int qwc; int p2[3];
    int tadr; int p3[3];
} sDmaChan;

extern int func_00426B48(sSifClient *cd, unsigned int rpc_number, int mode);
extern int func_00426D18(sSifClient *cd, int fno, int mode, void *send, int ssize, void *recv, int rsize,
                         void (*end_function)(void *), void *end_param);
extern int func_00426F08(sSifClient *cd);

extern sSifClient D_00526200[2]; /* libpad */
extern sSifClient D_005266C0;
extern sSifClient D_004527D0;
extern sSifClient D_00453180;
extern sSifClient D_00526710;
extern sSifClient D_005271C0; /* libmc */
extern sSifClient D_005290A0; /* usb keyboard */

/* libmc state */
extern int D_004533C0;  /* RPC function number of the pending asynchronous call, 0 when idle */
extern int D_004533C4;  /* semaphore serialising the libmc RPCs */
extern int D_004533C8;  /* semaphore for the RPC wait sleep */
extern int *D_005271E8; /* getInfo result pointers (filled by func_0040A440) */
extern int *D_005271EC;
extern int *D_005271F0;

/* libmc RPC command block (48 bytes, sent with every call except the file-name ones) */
typedef struct {
    int fd;
    int port;
    int slot;
    int size;
    int offset;
    int origin;
    int buffer;
    int param;
    unsigned char data[16];
} sMcCmd;
extern sMcCmd D_00527240;

/* libmc RPC command block for the calls that pass a file name (0x414 bytes) */
typedef struct {
    int port;
    int slot;
    int mode;
    int count;
    int buffer;
    char name[0x400];
} sMcFileCmd;
/* libmc directory-table entry (0x40 bytes; the name starts at +0x20) */
typedef struct {
    char info[0x20];
    char name[0x20];
} sMcDirEntry;
extern sMcFileCmd D_00527270;
extern sMcDirEntry D_00527200;  /* entry sent with RPC 14 */
extern char D_00527220[];       /* D_00527200.name (its own splat symbol) */
extern int D_005276C0[];        /* uncached receive buffer for the read / getInfo callbacks */
extern char D_00527780[];       /* receive buffer for RPC 12 (func_0040A7D0) */
extern int D_00528780[];        /* RPC result buffer */

/* USB keyboard library state */
extern int D_00453438;            /* semaphore serialising the keyboard RPCs (-1 = not initialised) */
extern unsigned char D_00529100[]; /* RPC send buffer (16 bytes) */
extern unsigned char D_00529140[]; /* RPC receive buffer */
extern char D_004982F8[];
extern unsigned char D_00529101[]; /* D_00529100[1] (own splat symbol) */
extern unsigned char D_00529141[]; /* D_00529140[1] (own splat symbol) */

/* key repeat state and 4-entry key ring of one keyboard (sKbPort + 0x10) */
typedef struct {
    int state;              /* 0 idle, 1 waiting for the first repeat, 2 repeating */
    int count;              /* vblank count (func_0040CF88) */
    int delay;
    int rate;
    unsigned short last;    /* key being repeated */
    unsigned short ring[4];
    int wr;
    int rd;
    int n;
} sKbRepeat;
/* one keyboard (0x38 bytes) */
typedef struct {
    int ledMode;            /* sceUsbKbSetLEDMode */
    int f04;                /* code translation on/off (func_0040C770, func_0040D118) */
    int f08;                /* key table 0..2 (func_0040C7C0, func_0040D268) */
    int f0C;                /* 1 = raw reports (func_0040D430, func_0040C980) */
    sKbRepeat rep;
} sKbPort;
/* one keyboard report (0x88 bytes) */
typedef struct {
    int led;
    int mod;
    int n;
    unsigned short keys[62];
} sKbData;
typedef struct {
    int n;                  /* number of keyboards */
    sKbPort *port;
    sKbData prev;           /* previous report */
    int cmd;                /* RPC of the pending asynchronous call (dispatched by func_0040C8C0) */
    int arg;                /* its keyboard index */
    void *out;              /* its result buffer */
    int thread;
} sUsbKb;
extern sUsbKb D_00529000;
extern sKbPort *D_00529004; /* D_00529000.port (own splat symbol) */
extern sKbData D_00529008;  /* D_00529000.prev (own splat symbol) */
extern void *D_00529098;    /* D_00529000.out (own splat symbol) */
extern int D_0052909C;      /* D_00529000.thread (own splat symbol) */
extern sKbPort D_005291D0[];

/* libcdvd state */
typedef struct {
    int status;
    void (*entry)(void);
    int stack;
    int stackSize;
    int gpReg;
    int initPriority;
    int currentPriority;
    int attr;
    int option;
    int waitType;
    int waitId;
    int wakeupCount;
} sThreadParam;                 /* 0x30 bytes, as passed to func_00423BA0 / func_00423CA0 */
extern int D_00451610;          /* debug print level */
extern int D_00451614;          /* callback thread id (0 = none) */
extern int D_00451618;
extern int D_0045161C;
/* libcdvd state shared with its RPC callback thread: volatile in every user
   (retail re-reads each one; tools/gen/phaseb_import.py VOL_DECL_OK) */
extern volatile int D_00451620;          /* callback thread semaphore */
extern int D_00451624;
extern volatile int D_00451628;          /* semaphores */
extern volatile int D_0045162C;
extern volatile int D_00451630;
extern volatile int D_00451634;          /* busy flag */
extern volatile int D_00451638;
extern int D_0045163C;
extern int D_00451640;
extern int D_00451644;          /* RPC bind state (< 0 = unbound) */
extern int D_00451648;
extern int D_0045164C;
extern volatile int D_00451658;  /* asynchronous command result */
extern volatile int D_0045165C;
extern void (*D_005264C0)(int); /* user callback */
extern int D_005264C4;          /* its $gp */
extern void (*D_005264C8)(int); /* interrupt callback */
extern int D_005264CC;          /* its $gp */
extern int D_005264D0;          /* its argument */
extern volatile int D_005264D4;
extern int D_005264D8;          /* id of the thread that initialised the library */
extern sThreadParam D_005264E0;
extern sThreadParam D_00526510;

/* libcdvd RPC parameter blocks and buffers */
typedef struct {
    int lsn;
    int sectors;
    int buf;
    unsigned char trycount;
    unsigned char spindlctrl;
    unsigned char datapattern;
    char *ebuf;
    int *res;
} sCdReadCmd;                   /* RPC 1 parameters, sent from D_00451700 */
typedef struct sCdSearchCmd {
    char hdr[0x24];             /* file record returned to the caller */
    char name[0x100];
    struct sCdSearchCmd *self;
    int arg;
} sCdSearchCmd;
extern int D_00451680[];        /* RPC result buffer */
extern int D_00451700[];        /* RPC send buffer */
extern char D_00452700[];
extern int D_004527C0[];
extern int D_00452800[];        /* RPC result buffer */
extern int D_00452C40[];        /* RPC send buffer */
extern int D_004531A8;          /* RPC function, send and receive size used by func_00402450 */
extern int D_004531AC;
extern int D_004531B0;
extern sCdSearchCmd D_00526540;
extern char D_00526564[];       /* D_00526540.name (own splat symbol) */
extern int D_00526680[];
extern int D_00526750[];
extern char D_00526780[];
extern int D_00453080[];

/* libpad state */
typedef struct {
    int dmaBuf;                 /* 0x100-byte DMA area holding two 0x80-byte frames */
    int actBuf;                 /* actuator command block (func_00400190) */
    int f08;
    int f0C;
    int open;                   /* non-zero while the port/slot is open */
    int f14;
    int f18;
} sPadSlot;                     /* 0x1C bytes */
extern sPadSlot D_00526250[2][4];
extern int D_00526440[];        /* RPC send / receive block (128 bytes) */
extern int D_004515C8;
extern int D_004515CC;
/* one 0x80-byte pad data frame in the DMA area (func_003FF9A8 returns the newer one) */
typedef struct {
    unsigned char a, b, c, d;
} sPadAct;
typedef struct {
    unsigned char f00[0x30];
    sPadAct act[4];             /* 0x30 */
    sPadAct comb[4];            /* 0x40 */
    unsigned short modes[4];    /* 0x50 */
    int frame;                  /* 0x58 frame counter */
    int f5C;
    int len;                    /* 0x60 */
    unsigned char f64;
    unsigned char f65;
    unsigned char f66;
    unsigned char f67;
    unsigned char f68;
    unsigned char f69;
    unsigned char f6A;
    unsigned char f6B;
    unsigned char f6C[4];
    unsigned char state;        /* 0x70 */
    unsigned char reqState;     /* 0x71 */
    unsigned char ok;           /* 0x72 */
    unsigned char f73[6];
    unsigned char f79[4];       /* 0x79 little-endian word */
    unsigned char f7D[3];
} sPadFrame;
extern sPadFrame *func_003FF9A8(int port, int slot);

/* DMA controller environment (D_CTRL / PCR / SQWC / RBOR / RBSR settings, 20 bytes) */
typedef struct { unsigned char sts, std, mfd, rcyc; unsigned short h4, h6, h8, hA; void *w0C; unsigned w10; } sDmaEnv;
extern sDmaEnv D_005261D0;     /* copy of the last environment set by func_003FE090 */

/* system configuration copy set by func_0040BA48 (8 bytes; each byte has its own splat symbol) */
typedef struct {
    short f0;
    unsigned char f2[6];
} sSysConfig;
extern sSysConfig D_004533F8;
extern unsigned char D_004533FA[]; /* D_004533F8.f2[0..5] */
extern unsigned char D_004533FB[];
extern unsigned char D_004533FC[];
extern unsigned char D_004533FD[];
extern unsigned char D_004533FE[];
extern unsigned char D_004533FF[];

/* other shared tables and buffers */
extern int D_00451590[];        /* DMA channel register block addresses, 10 channels */
extern int D_004532C0[];        /* table indexed by the top 4 bits of a command word (func_00406DF0..func_004072E8) */
extern char D_00453400[];       /* 14 bytes read from the file D_00498220 names (func_0040B938) */
extern char D_00528FC0[];       /* buffer shared by func_0040B400 / func_0040B5B0 / func_0040B6D0 */

INCLUDE_ASM("seg/seg_2FE810", func_003FD810);

void func_003FD908(void) {
}

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FD910);
#ifdef SKIP_ASM
struct S3FD910 { short a; short b; short c; unsigned short d; int e; int f; };
extern int func_00423AA0(int, int);
extern void func_00424880(int);
extern void func_004239A0(int, int, int);
extern int func_004240E0(int);
void *func_003FDAA0(void);
void func_003FD910(short a, short b, short c, short d) {
    struct S3FD910 *p;
    int e;
    switch (a) {
    case 0:
        p = func_003FDAA0();
        *(volatile long *)0x12001000 = 0x200;
        p->a = b;
        p->b = c;
        p->d = (*(volatile unsigned long *)0x12001000 >> 16) & 0xFF;
        func_004240E0(0xFF00);
        p->c = !!d;
        if (p->e) {
            func_00424880(2);
            func_00423AA0(2, p->f);
            p->e = 0;
            p->f = 0;
        }
        func_004239A0(b & 1, c & 0xFF, d & 1);
        break;
    case 1:
        *(volatile long *)0x12001000 = 0x100;
        break;
    case 5:
        p = func_003FDAA0();
        p->c = d != 0;
        p->a = b;
        p->b = c;
        p->d = (*(volatile unsigned long *)0x12001000 >> 16) & 0xFF;
        func_004239A0(b & 1, c & 0xFF, d & 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDAA0);
#ifdef SKIP_ASM
extern char D_00451550[];
void *func_003FDAA0(void) {
    return D_00451550;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FDAB0);

INCLUDE_ASM("seg/seg_2FE810", sceGsSyncPath);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDE30);
#ifdef SKIP_ASM
extern void func_00425C60();
extern char D_004977D8[];
/* wait for the channel's STR bit to clear; after 0x1000000 polls log and force-stop it */
void func_003FDE30(d, m)
    sDmaChan *d;
    int m;
{
    int n = 0x1000000;
    if (d->chcr & 0x100) {
        do {
            if (--n < 0) {
                func_00425C60(D_004977D8);
                func_003FE5C8(d);
            }
        } while (d->chcr & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDEA0);
#ifdef SKIP_ASM
extern char D_004977D8[];
extern void func_003FDEA0_log(char *) __asm__("func_00425C60");
extern int func_003FE5C8();
/* wait until the channel's MADR reaches lim (tmo polls, default 0x1000000) */
void func_003FDEA0(sDmaChan *d, unsigned lim, int tmo) {
    int t = 0x1000000;
    if (tmo)
        t = tmo;
    while (d->madr < lim) {
        if (--t < 0) {
            func_003FDEA0_log(D_004977D8);
            func_003FE5C8(d);
        }
        }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDF28);
#ifdef SKIP_ASM
unsigned func_003FDF28(unsigned a) {
    if ((a >> 28) == 7) {
        a &= 0xFFFFFFF;
        a |= 0x80000000;
    }
    return a;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDF50);
#ifdef SKIP_ASM
void func_003FDF50(char *p, int n) {
    while (n--)
        *p++ = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDF88);
#ifdef SKIP_ASM
int func_003FDF88(unsigned n) {
    return n < 10 ? D_00451590[n] : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDFB0);
#ifdef SKIP_ASM
extern int D_004977F0[];
extern void func_003FDF50(char *, int);
extern int func_003FE090(sDmaEnv *);
int func_003FDFB0(int a) {
    sDmaEnv env;
    int i;
    int saved = *(volatile int *)0x1000E000 & 1;
    for (i = 0; i < 10; i++) {
        if (D_004977F0[i] != 0) {
            int *t = (int *)D_00451590[i];
            t[0] = 0;
            t[0x30 / 4] = 0;
            t[0x10 / 4] = 0;
            t[0x50 / 4] = 0;
            t[0x40 / 4] = 0;
            t[0x80 / 4] = 0;
        }
    }
    *(volatile int *)0x1000E010 = 0xFF1F;
    *(int *)0x1000E010 &= 0xFF1F0000;
    func_003FDF50((char *)&env, 20);
    func_003FE090(&env);
    if (a == 1)
        *(volatile int *)0x1000E000 |= 1;
    return saved;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE090);
#ifdef SKIP_ASM
// DMA environment write: validate the table indices, merge them into D_CTRL, write PCR/SQWC/RBOR/RBSR, keep a copy.
// The field at +0x0C is a pointer (the ring-buffer address), stored as its integer value.
extern unsigned char D_00497818[];
extern unsigned char D_00497828[];
extern unsigned char D_00497838[];
int func_003FE090(sDmaEnv *p) {
    unsigned c;
    unsigned pcr;
    unsigned sqwc;
    unsigned rbor;
    unsigned rbsr;
    c = *(volatile unsigned *)0x1000E000;
    pcr = *(volatile unsigned *)0x1000E020;
    sqwc = *(volatile unsigned *)0x1000E030;
    rbor = *(volatile unsigned *)0x1000E050;
    rbsr = *(volatile unsigned *)0x1000E040;
    if (p->sts >= 10) return -1;
    if (p->std >= 10) return -2;
    if (p->mfd >= 10) return -3;
    if (p->rcyc >= 7) return -4;
    c = (c & ~0x30) | (D_00497818[p->sts] << 4);
    c = (c & ~0xC0) | (D_00497828[p->std] << 6);
    c = (c & ~0xC) | (D_00497838[p->mfd] << 2);
    if (p->rcyc != 0) {
        c |= 2;
        c = (c & ~0x300) | ((p->rcyc - 1) << 8);
    } else {
        c &= ~2;
    }
    sqwc = (p->hA << 16) | p->h8;
    rbsr = p->w10;
    rbor = (unsigned)p->w0C;
    *(volatile unsigned *)0x1000E000 = c;
    pcr = (p->h4 << 16) | p->h6;
    *(volatile unsigned *)0x1000E020 = pcr;
    *(volatile unsigned *)0x1000E030 = sqwc;
    *(volatile unsigned *)0x1000E050 = rbor;
    *(volatile unsigned *)0x1000E040 = rbsr;
    D_005261D0 = *p;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE268);
#ifdef SKIP_ASM
sDmaEnv *func_003FE268(sDmaEnv *d) {
    *d = D_005261D0;
    return d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE2A0);
#ifdef SKIP_ASM
int func_003FE2A0(int v) {
    volatile int *r = (volatile int *)0x1000E060;
    int o = *r;
    *r = v;
    return o;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE2B8);
#ifdef SKIP_ASM
extern unsigned func_003FDF28(unsigned);
extern void func_003FDE30();
void func_003FE2B8(sDmaChan *d, unsigned a) {
    unsigned v = func_003FDF28(a);
    func_003FDE30(d);
    if (d->tadr != 0xFFFFFFFF)
        d->tadr = v;
    d->qwc = 0;
    d->chcr = (d->chcr & ~0xC) | 0x105;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE320);
#ifdef SKIP_ASM
void func_003FE320(sDmaChan *d, unsigned a, int c) {
    unsigned v = func_003FDF28(a);
    func_003FDE30(d);
    if (d->madr != 0xFFFFFFFF)
        d->madr = v;
    d->qwc = c;
    d->chcr = (d->chcr & ~0xC) | 0x101;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE398);
#ifdef SKIP_ASM
void func_003FE398(sDmaChan *d, unsigned a, int c) {
    unsigned v = func_003FDF28(a);
    func_003FDE30(d);
    if (d->madr != 0xFFFFFFFF)
        d->madr = v;
    d->qwc = c;
    d->chcr = (d->chcr & ~0xC) | 0x109;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE410);
#ifdef SKIP_ASM
extern void func_003FDE30();
void func_003FE410(sDmaChan *d) {
    func_003FDE30(d);
    d->qwc = 0;
    d->chcr = ((d->chcr & ~0xC | 0x4) & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE458);
#ifdef SKIP_ASM
void func_003FE458(sDmaChan *d, unsigned a, int c) {
    unsigned v = func_003FDF28(a);
    int t;
    func_003FDE30(d);
    if (d->madr != 0xFFFFFFFF)
        d->madr = v;
    d->qwc = c;
    t = d->chcr & ~0xC;
    d->chcr = (t & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE4D8);
#ifdef SKIP_ASM
void func_003FE4D8(sDmaChan *d, unsigned a, int c) {
    unsigned v = func_003FDF28(a);
    int t;
    func_003FDE30(d);
    if (d->madr != 0xFFFFFFFF)
        d->madr = v;
    d->qwc = c;
    t = (d->chcr & ~0xC) | 8;
    d->chcr = (t & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE558);
#ifdef SKIP_ASM
extern void func_003FDE30(sDmaChan *, int);
int func_003FE558(sDmaChan *d, int m) {
    if (m == 1)
        return ((unsigned)d->chcr >> 8) & 1;
    func_003FDE30(d, m);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE590);
#ifdef SKIP_ASM
extern void func_003FDEA0(sDmaChan *, unsigned, int);
int func_003FE590(sDmaChan *d, unsigned n, int m, int x) {
    if (m == 1)
        return (unsigned)d->madr < n;
    func_003FDEA0(d, n, x);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE5C8);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
/* force-stop a channel: suspend DMA (D_ENABLEW), clear CHCR.STR, restore */
int func_003FE5C8(sDmaChan *d) {
    int x, r, s;
    unsigned v;
    s = func_0042C078();
    r = *(volatile int *)0x1000F520;
    if ((r & 0x10000) == 0)
        *(volatile int *)0x1000F590 = r | 0x10000;
    x = *(volatile int *)0x1000E000;
    v = d->chcr;
    d->chcr = v & 0xFFFFFEFF;
    *(volatile int *)0x1000F590 = r;
    if (s)
        func_0042C0C0();
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE660);
#ifdef SKIP_ASM
unsigned func_003FE660(unsigned *p, unsigned v) {
    unsigned o = *p;
    *p = v;
    return (o >> 8) & 1;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE678);
#ifdef SKIP_ASM
// VU0 status read: (status >> 0) & 1 from $vi29.
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_678(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi29
	.set reorder" : "=r"(v));
    return v;
}
int func_003FE678(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE690);
#ifdef SKIP_ASM
// VU0 control: set bit 0x2 in $vi28 (read-modify-write).
// PORT: PS2-only VU0 control-register access. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_vi28_690(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi28
	.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_vi28_690(unsigned int v)
{
    __asm__ volatile(".set noreorder
	ctc2.ni %0, $vi28
	.set reorder" : : "r"(v));
}
void func_003FE690(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE6A8);
#ifdef SKIP_ASM
// Act on the VU0 status decode: case 4 sets bit 0x1 in $vi28; returns 1 for cases 0, 3 and 4.
// PORT: PS2-only VU0 control-register access. The EE interlocks COP2 moves, so no hazard nop follows.
extern int func_003FE7D8_6A8(void) __asm__("func_003FE7D8");
static inline unsigned int vu0_cfc2_vi28_6A8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi28
	.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_vi28_6A8(unsigned int v)
{
    __asm__ volatile(".set noreorder
	ctc2.ni %0, $vi28
	.set reorder" : : "r"(v));
}
int func_003FE6A8(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE708);
#ifdef SKIP_ASM
extern int func_003FE7D8(void);
extern int func_003FE828(void);
extern void func_003FE818(int);
int func_003FE708(void) {
    int r = 0;
    int n = func_003FE7D8();
    if (n < 4) {
        if (n > 0) {
            func_003FE818(func_003FE828());
            r = 1;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE758);
#ifdef SKIP_ASM
// VU0 status flag set/clear: bit 2 of the VU0 status register ($vi28).
// PORT: PS2-only VU0 control-register access.
static inline unsigned int vu0_cfc2_758(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi28\n\t.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_758(unsigned int v)
{
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi28\n\t.set reorder" : : "r"(v));
}
void func_003FE758(int a) {
    unsigned int s = vu0_cfc2_758();
    if (a == 1)
        s |= 0x4;
    else
        s &= ~0x4;
    vu0_ctc2_758(s);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE788);
#ifdef SKIP_ASM
// VU0 status flag set/clear: bit 3 of the VU0 status register ($vi28).
// PORT: PS2-only VU0 control-register access.
static inline unsigned int vu0_cfc2_788(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi28\n\t.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_788(unsigned int v)
{
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi28\n\t.set reorder" : : "r"(v));
}
void func_003FE788(int a) {
    unsigned int s = vu0_cfc2_788();
    if (a == 1)
        s |= 0x8;
    else
        s &= ~0x8;
    vu0_ctc2_788(s);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE7B8);
#ifdef SKIP_ASM
// VU0 status flag read: bit 2 of the VU0 status register ($vi28).
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_vi28_7B8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi28\n\t.set reorder" : "=r"(v));
    return v;
}
int func_003FE7B8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE7C8);
#ifdef SKIP_ASM
// VU0 status read: (status >> 3) & 1 from $vi28.
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_7C8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi28
	.set reorder" : "=r"(v));
    return v;
}
int func_003FE7C8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE7D8);
#ifdef SKIP_ASM
// VU0 status decode from $vi29.
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_vi29_7D8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi29
	.set reorder" : "=r"(v));
    return v;
}
int func_003FE7D8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE818);

INCLUDE_ASM("seg/seg_2FE810", func_003FE828);
#ifdef SKIP_ASM
// VU0 status read: (status >> 0) & 1 from $vi26.
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_828(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi26
	.set reorder" : "=r"(v));
    return v;
}
int func_003FE828(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FE838);

INCLUDE_ASM("seg/seg_2FE810", func_003FE998);

INCLUDE_ASM("seg/seg_2FE810", func_003FEB08);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_vi29_EB08(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi29\n\t.set reorder" : "=r"(v));
    return v;
}
int func_003FEB08(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FEB38);
#ifdef SKIP_ASM
typedef int Q3EB38_i __attribute__((mode(TI)));
typedef union { Q3EB38_i q; unsigned long d[2]; int w[4]; short h[8]; } Q3EB38;
// PORT: PS2-only VU0 data-memory write: store the qword `v` at VU0 address `addr` through $vi1/$vf1, restoring both.
static inline void vu0_write_qword_EB38(unsigned int addr, Q3EB38_i v)
{
    unsigned int vi;
    Q3EB38_i vf;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi1\n\tqmfc2.ni %1, $vf1\n\tqmtc2.ni %2, $vf1\n\tctc2.ni %3, $vi1\n\tvsqi.xyzw $vf1, ($vi1++)\n\tctc2.ni %0, $vi1\n\tqmtc2.ni %1, $vf1\n\t.set reorder"
                     : "=&r"(vi), "=&r"(vf) : "r"(v), "r"(addr));
}
void func_003FEB38(unsigned short addr, Q3EB38 v) {
    vu0_write_qword_EB38(addr, v.q);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FEB78);

INCLUDE_ASM("seg/seg_2FE810", func_003FEBC0);
#ifdef SKIP_ASM
// VU0 control: set bit 0x200 in $vi28 (read-modify-write).
// PORT: PS2-only VU0 control-register access. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_vi28_BC0(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi28
	.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_vi28_BC0(unsigned int v)
{
    __asm__ volatile(".set noreorder
	ctc2.ni %0, $vi28
	.set reorder" : : "r"(v));
}
void func_003FEBC0(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FEBD8);
#ifdef SKIP_ASM
// Act on the VU0 status decode: case 4 sets bit 0x100 in $vi28; returns 1 for cases 0, 3 and 4.
// PORT: PS2-only VU0 control-register access. The EE interlocks COP2 moves, so no hazard nop follows.
extern int func_003FED08_BD8(void) __asm__("func_003FED08");
static inline unsigned int vu0_cfc2_vi28_BD8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi28
	.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_vi28_BD8(unsigned int v)
{
    __asm__ volatile(".set noreorder
	ctc2.ni %0, $vi28
	.set reorder" : : "r"(v));
}
int func_003FEBD8(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FEC38);
#ifdef SKIP_ASM
extern int func_003FED08(void);
extern unsigned short func_003FED60(void);
extern void func_003FED50(int);
int func_003FEC38(void) {
    int r = 0;
    int n = func_003FED08();
    if (n < 4) {
        if (n > 0) {
            func_003FED50(func_003FED60());
            r = 1;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FEC88);
#ifdef SKIP_ASM
// VU0 status flag set/clear: bit 10 of the VU0 status register ($vi28).
// PORT: PS2-only VU0 control-register access.
static inline unsigned int vu0_cfc2_C88(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi28\n\t.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_C88(unsigned int v)
{
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi28\n\t.set reorder" : : "r"(v));
}
void func_003FEC88(int a) {
    unsigned int s = vu0_cfc2_C88();
    if (a == 1)
        s |= 0x400;
    else
        s &= ~0x400;
    vu0_ctc2_C88(s);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FECB8);
#ifdef SKIP_ASM
// VU0 status flag set/clear: bit 11 of the VU0 status register ($vi28).
// PORT: PS2-only VU0 control-register access.
static inline unsigned int vu0_cfc2_CB8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi28\n\t.set reorder" : "=r"(v));
    return v;
}
static inline void vu0_ctc2_CB8(unsigned int v)
{
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi28\n\t.set reorder" : : "r"(v));
}
void func_003FECB8(int a) {
    unsigned int s = vu0_cfc2_CB8();
    if (a == 1)
        s |= 0x800;
    else
        s &= ~0x800;
    vu0_ctc2_CB8(s);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FECE8);
#ifdef SKIP_ASM
// VU0 status read: (status >> 10) & 1 from $vi28.
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_CE8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi28
	.set reorder" : "=r"(v));
    return v;
}
int func_003FECE8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FECF8);
#ifdef SKIP_ASM
// VU0 status read: (status >> 11) & 1 from $vi28.
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_CF8(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder
	cfc2.ni %0, $vi28
	.set reorder" : "=r"(v));
    return v;
}
int func_003FECF8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FED08);
#ifdef SKIP_ASM
// VU0 status decode from $vi29 (bits 9, 10, 11, 8).
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_vi29_ED08(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi29\n\t.set reorder" : "=r"(v));
    return v;
}
int func_003FED08(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FED50);
#ifdef SKIP_ASM
// Write the VU0 control register $vi31 (low 16 bits).
// PORT: PS2-only VU0 control-register write.
static inline void vu0_ctc2_D50(unsigned int v)
{
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi31\n\t.set reorder" : : "r"(v));
}
void func_003FED50(int v) {
    vu0_ctc2_D50(v & 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FED60);
#ifdef SKIP_ASM
extern void func_003FEB78(void *, int);
unsigned short func_003FED60(void) {
    unsigned short v;
    func_003FEB78(&v, 0x43A);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FED88);
#ifdef SKIP_ASM
typedef int Q3ED88_i __attribute__((mode(TI)));
typedef union { Q3ED88_i q; int w[4]; short h[8]; } Q3ED88;
typedef struct { Q3ED88 a[32]; int b[7]; short c[16]; } S3ED88;
extern int e3ED88_a(void) __asm__("func_003FEB08");
extern Q3ED88 e3ED88_b(unsigned short) __asm__("func_003FEB78");
int func_003FED88_impl(S3ED88 *s) __asm__("func_003FED88");
int func_003FED88_impl(S3ED88 *s) {
    unsigned i;
    if (e3ED88_a() != 0)
        return 0;
    for (i = 0; i < 0x20; i++) {
        s->a[i].q = e3ED88_b(0x400 + i).q;
    }
    s->b[0] = e3ED88_b(0x430).w[0];
    s->b[1] = e3ED88_b(0x431).w[0];
    s->b[2] = e3ED88_b(0x432).w[0];
    s->b[3] = e3ED88_b(0x434).w[0];
    s->b[4] = e3ED88_b(0x435).w[0];
    s->b[5] = e3ED88_b(0x436).w[0];
    s->b[6] = e3ED88_b(0x437).w[0];
    for (i = 0; i < 0x10; i++)
        s->c[i] = e3ED88_b(0x420 + i).h[0];
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FEF18);
#ifdef SKIP_ASM
typedef int Q3EF18_i __attribute__((mode(TI)));
typedef union { Q3EF18_i q; unsigned long d[2]; int w[4]; short h[8]; } Q3EF18;
typedef struct { Q3EF18 a[32]; unsigned b[7]; unsigned short c[16]; } S3EF18;
extern int e3EF18_a(void) __asm__("func_003FEB08");
extern void e3EF18_b(unsigned short, Q3EF18) __asm__("func_003FEB38");
int func_003FEF18_impl(S3EF18 *s) __asm__("func_003FEF18");
int func_003FEF18_impl(S3EF18 *s) {
    unsigned i;
    Q3EF18 t;
    if (e3EF18_a() != 0)
        return 0;
    for (i = 0; i < 0x20; i++) {
        t.q = s->a[i].q;
        e3EF18_b(0x400 + i, t);
    }
    t.q = 0;
    t.d[0] = s->b[0];
    e3EF18_b(0x430, t);
    t.d[0] = s->b[1];
    e3EF18_b(0x431, t);
    t.d[0] = s->b[2];
    e3EF18_b(0x432, t);
    t.d[0] = s->b[3];
    e3EF18_b(0x434, t);
    t.d[0] = s->b[4];
    e3EF18_b(0x435, t);
    t.d[0] = s->b[5];
    e3EF18_b(0x436, t);
    t.d[0] = s->b[6];
    e3EF18_b(0x437, t);
    for (i = 0; i < 0x10; i++) {
        t.d[0] = s->c[i];
        e3EF18_b(0x420 + i, t);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF060);
#ifdef SKIP_ASM
void func_003FF060(void) {
    *(volatile int *)0x10003810 = 1;
    *(volatile int *)0x10003820 = 6;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF088);
#ifdef SKIP_ASM
extern int func_003FF160(void);
int func_003FF088(int a) {
    int r = func_003FF160();
    if (r == 0) return 0;
    if (r == 1) return 0;
    if (a == 0) {
        *(volatile int *)0x10003810 = 2;
    } else if (a == 1) {
        *(volatile int *)0x10003810 = 4;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF0F0);
#ifdef SKIP_ASM
extern int func_003FF160(void);
int func_003FF0F0(void) {
    if (func_003FF160() != 1)
        return 0;
    *(volatile int *)0x10003810 = 8;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF130);
#ifdef SKIP_ASM
unsigned func_003FF130(unsigned a, unsigned b, unsigned c) {
    unsigned v = a | (b << 1) | (c << 2);
    *(volatile unsigned *)0x10003820 = v;
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF150);
#ifdef SKIP_ASM
int func_003FF150(void) {
    return *(volatile int *)0x10003820;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF160);
#ifdef SKIP_ASM
int func_003FF160(void) {
    int v = *(volatile int *)0x10003800;
    if (!(v & 3)) {
        return 0;
    }
    if (v & 0x700) {
        return 1;
    }
    if (v & 0xC) {
        return 2;
    }
    return (v & 0x3000) ? 3 : 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF1B0);
#ifdef SKIP_ASM
struct S3FF1B0 { int w[11]; short h[4]; char c[4]; };
extern int func_003FF160(void);
#define R(a) (*(volatile int *)(a))
int func_003FF1B0(struct S3FF1B0 *p) {
    unsigned v = *(volatile unsigned *)0x10003840;
    if ((unsigned)func_003FF160() >= 2)
        return 0;
    p->w[0] = R(0x10003900);
    p->w[1] = R(0x10003910);
    p->w[2] = R(0x10003920);
    p->w[3] = R(0x10003930);
    p->w[4] = R(0x10003940);
    p->w[5] = R(0x10003950);
    p->w[6] = R(0x10003960);
    p->w[7] = R(0x10003970);
    p->w[8] = R(0x10003870);
    p->w[9] = R(0x10003880);
    p->w[10] = R(0x10003800);
    p->h[0] = R(0x100038D0);
    p->h[1] = R(0x10003890);
    p->h[2] = R(0x10003830);
    p->h[3] = R(0x10003860);
    p->c[0] = R(0x10003820);
    p->c[1] = v;
    p->c[2] = v >> 8;
    p->c[3] = R(0x10003850);
    return 1;
}
#undef R
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF330);
#ifdef SKIP_ASM
typedef int i128 __attribute__((mode(TI)));
int func_003FF330(i128 *src, int n) {
    int c = 0;
    while (n--) {
        if (*(volatile unsigned *)0x10003800 & 0x10000000)
            return c;
        *(i128 *)0x10004000 = *src++;
        c++;
    }
    return c;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FF390);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF4A8);
#ifdef SKIP_ASM
extern char D_004978D0[];
extern char D_004978F8[];
extern int func_00400740(void);
extern void func_00425C60();
extern int func_003FF5E8_a(int) __asm__("func_003FF5E8");
int func_003FF4A8(int a) {
    int i;
    int v;
    int h;
    D_004515C8 = 1;
    for (;;) {
        func_00426B48(&D_00526200[0], 0x80000100, 0);
        if (D_00526200[0].server != 0)
            break;
        for (i = 0x10000; i--; )
            ;
    }
    for (;;) {
        func_00426B48(&D_00526200[1], 0x80000101, 0);
        if (D_00526200[1].server != 0)
            break;
        for (i = 0x10000; i--; )
            ;
    }
    v = func_00400740();
    h = v >> 8;
    if (h != 4) {
        if (D_004515CC != 0) {
            func_00425C60(D_004978D0);
            func_00425C60(D_004978F8, 4, 0, h, v & 0xFF);
        }
        return 0;
    }
    return func_003FF5E8_a(a);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF5E8);
#ifdef SKIP_ASM
int func_003FF5E8(void) {
    int i;
    for (i = 0; i < 4; i++) {
        D_00526250[0][i].open = 0;
        D_00526250[0][i].f18 = 0;
        D_00526250[0][i].f14 = 0;
        D_00526250[1][i].open = 0;
        D_00526250[1][i].f18 = 0;
        D_00526250[1][i].f14 = 0;
    }
    D_00526440[0] = 16;
    D_00526440[4] = 0;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF688);
#ifdef SKIP_ASM
int func_003FF688(void) {
    int v;
    D_00526440[0] = 15;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    v = D_00526440[3];
    if (v == 1)
        D_004515C8 = 0;
    return v;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FF708);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF8F0);
#ifdef SKIP_ASM
int func_003FF8F0(int a, int b) {
    if (!D_00526250[a][b].open)
        return 0;
    D_00526440[0] = 14;
    D_00526440[1] = a;
    D_00526440[2] = b;
    D_00526440[4] = 1;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    D_00526250[a][b].open = 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF9A8);
#ifdef SKIP_ASM
extern void func_00424698(int *, int *);
sPadFrame *func_003FF9A8(int a, int b) {
    sPadFrame *p;
    int c;
    p = (sPadFrame *)D_00526250[a][b].dmaBuf;
    func_00424698((int *)p, (int *)((char *)p + 0xFF));
    c = (p[0].frame < p[1].frame) << 7;
    return (sPadFrame *)((char *)p + c);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFA08);
#ifdef SKIP_ASM
int func_003FFA08(int a, int b) {
    if (!D_00526250[a][b].open) return 0;
    return func_003FF9A8(a, b)->frame;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFA58);
#ifdef SKIP_ASM
struct B128 { char b[128]; };
extern int func_0042C078();
extern void func_0042C0C0();
extern int func_0041605C_k() __asm__("func_0041605C");
int func_003FFA58(int a, int b, int c) {
    struct B128 s;
    int *q;
    int r;
    if (!D_00526250[a][b].open)
        return 0;
    r = func_0042C078();
    q = (int *)&s;
    s = *(struct B128 *)func_003FF9A8(a, b);
    func_0041605C_k(c, q, q[0x60 / 4]);
    if (r == 1)
        func_0042C0C0();
    return q[0x60 / 4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFBC0);
#ifdef SKIP_ASM
int func_003FFBC0(int a, int b) {
    unsigned char *p;
    if (!D_00526250[a][b].open)
        return 99;
    p = (unsigned char *)func_003FF9A8(a, b); /* byte reads: a sPadFrame * makes one lhu of state/reqState */
    if (p[0x70] == 6 && p[0x71] == 2)
        return 5;
    return p[0x70];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFC38);
#ifdef SKIP_ASM
extern char *D_004515D0[];
extern char D_004979A8[];
extern char *strcpy(char *, const char *);
void func_003FFC38(unsigned i, char *dst) {
    if (i < 8) {
        strcpy(dst, D_004515D0[i]);
        return;
    }
    *dst = D_004979A8[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFC70);
#ifdef SKIP_ASM
int func_003FFC70(int a, int b, int c) {
    if (!D_00526250[a][b].open)
        return 0;
    func_003FF9A8(a, b)->reqState = c;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFCD8);
#ifdef SKIP_ASM
int func_003FFCD8(int a, int b) {
    if (!D_00526250[a][b].open) return 0;
    return func_003FF9A8(a, b)->reqState;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFD28);
#ifdef SKIP_ASM
extern char *D_004515F0[];
extern char D_004979A8[];
extern char *strcpy(char *, const char *);
void func_003FFD28(unsigned i, char *dst) {
    if (i < 4) {
        strcpy(dst, D_004515F0[i]);
        return;
    }
    *dst = D_004979A8[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFD60);
#ifdef SKIP_ASM
int func_003FFD60(int a, int b, int c, int d) {
    sPadFrame *p;
    if (!D_00526250[a][b].open) return 0;
    p = func_003FF9A8(a, b);
    if (p->ok != 1) return 0;
    if (p->f64 < 2) return 0;
    if (c >= p->f6A) return 0;
    if (c == -1) return p->f6A;
    switch (d) {
    case 1:
        return p->act[c].a;
    case 2:
        return p->act[c].b;
    case 3:
        return p->act[c].c;
    case 4:
        return p->act[c].d;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFE80);
#ifdef SKIP_ASM
int func_003FFE80(int a, int b, int c, int d) {
    sPadFrame *p;
    if (!D_00526250[a][b].open) return 0;
    p = func_003FF9A8(a, b);
    if (p->ok != 1) return 0;
    if (p->f64 < 2) return 0;
    if (c == -1) return p->f6B;
    if (c >= p->f6B) return 0;
    switch (d) {
    case -1:
        return p->comb[c].a;
    case 0:
        return p->comb[c].b;
    case 1:
        return p->comb[c].c;
    case 2:
        return p->comb[c].d;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFFA0);
#ifdef SKIP_ASM
// scePadInfoMode(port, slot, term, offs)
int func_003FFFA0(int port, int slot, int term, int offs) {
    sPadFrame *p;
    int t;
    if (!D_00526250[port][slot].open) return 0;
    p = func_003FF9A8(port, slot);
    if (p->ok != 1) return 0;
    if (p->reqState == 2) return 0;
    switch (term) {
    case 1:
        t = p->f65;
        if (t == 0xF3) return 0;
        return t >> 4;
    case 2:
        if (p->f64 == 1) return 0;
        return p->modes[p->f69];
    case 3:
        if (p->f64 == 1) return 0;
        return p->f69;
    case 4:
        if (p->f64 == 1) return 0;
        if (offs == -1) return p->f68;
        if (offs < p->f68) return p->modes[offs];
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004000D8);
#ifdef SKIP_ASM
extern int func_003FFC70(int, int, int);
int func_004000D8(int a, int b, int c, int d) {
    int v;
    D_00526440[3] = c;
    D_00526440[4] = d;
    D_00526440[0] = 6;
    D_00526440[1] = a;
    D_00526440[2] = b;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    v = D_00526440[5];
    if (v == 1)
        func_003FFC70(a, b, 2);
    return D_00526440[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400190);
#ifdef SKIP_ASM
extern int s0_F390(int, int) __asm__("func_003FF390");
int func_00400190(int a, int b, unsigned char *src) {
    int i;
    int ok;
    int *q;
    char *d;
    if (func_003FF9A8(a, b)->ok != 1)
        return 0;
    q = (int *)D_00526250[a][b].actBuf;
    d = (char *)q + 0xC;
    for (i = 0; i < 6; i++)
        d[i] = src[i];
    ok = 1;
    q[1] = ok;
    q[2] = 6;
    if (s0_F390(a, b) != 1)
        ok = 0;
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400250);
#ifdef SKIP_ASM
extern int func_003FFC70(int, int, int);
int func_00400250(int port, int slot, unsigned char *data) {
    int i;
    unsigned char *d;
    int *p = D_00526440;
    p[0] = 8;
    p[1] = port;
    p[2] = slot;
    d = (unsigned char *)&p[3];
    for (i = 0; i < 6; i++)
        d[i] = data[i];
    if (func_00426D18(D_00526200, 1, 0, p, 128, p, 128, 0, 0) < 0)
        return 0;
    if (p[5] == 1)
        func_003FFC70(port, slot, 2);
    return p[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400328);
#ifdef SKIP_ASM
int func_00400328(int a, int b) {
    sPadFrame *p;
    if (!D_00526250[a][b].open)
        return 0;
    p = func_003FF9A8(a, b);
    if (p->ok != 1)
        return 0;
    if (p->f64 < 2)
        return 0;
    if (p->f66 < 2)
        return 0;
    return p->f79[0] + ((long)p->f79[1] << 8) + ((long)p->f79[2] << 16) + ((long)p->f79[3] << 24);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004003E0);
#ifdef SKIP_ASM
extern int func_003FFC70(int, int, int);
int func_004003E0(int a, int b, int c) {
    int v;
    D_00526440[3] = c;
    D_00526440[0] = 10;
    D_00526440[1] = a;
    D_00526440[2] = b;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    v = D_00526440[4];
    if (v == 1)
        func_003FFC70(a, b, 2);
    return D_00526440[4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400490);
#ifdef SKIP_ASM
extern int func_00400328(int, int);
int func_00400490(int a, int b) {
    if (!D_00526250[a][b].open) return 0;
    return func_00400328(a, b) == 0x3FFFF;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004004F0);
#ifdef SKIP_ASM
extern int func_004003E0(int, int, int);
int func_004004F0(int a, int b) {
    if (!D_00526250[a][b].open) return 0;
    return func_004003E0(a, b, 0xFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400548);
#ifdef SKIP_ASM
extern int func_004003E0(int, int, int);
int func_00400548(int a, int b) {
    if (!D_00526250[a][b].open) return 0;
    return func_004003E0(a, b, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004005A0);
#ifdef SKIP_ASM
struct P12 { char b[12]; } __attribute__((packed));
extern int func_003FFC70(int, int, int);
int func_004005A0(int a, int b, struct P12 *c) {
    D_00526440[1] = a;
    D_00526440[0] = 11;
    D_00526440[2] = b;
    *(struct P12 *)&D_00526440[3] = *c;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    if (D_00526440[7] == 1)
        func_003FFC70(a, b, 2);
    return D_00526440[7];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400670);
#ifdef SKIP_ASM
int func_00400670(void) {
    D_00526440[0] = 12;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004006D8);
#ifdef SKIP_ASM
int func_004006D8(int a) {
    D_00526440[1] = a;
    D_00526440[0] = 13;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400740);
#ifdef SKIP_ASM
int func_00400740(void) {
    D_00526440[0] = 18;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004007A8);
#ifdef SKIP_ASM
int func_004007A8(int a) {
    D_00526440[1] = a;
    D_00526440[0] = 20;
    if (func_00426D18(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400810);
#ifdef SKIP_ASM
// PORT: PS2-only COP0 (enable interrupts) and memory barrier.
#define SYNC_0810() __asm__ volatile("sync")
#define EI_0810() __asm__ volatile("ei")
extern void func_00423DD0(int);
void func_00400810(int a, int b, int c) {
    func_00423DD0(c);
    SYNC_0810();
    EI_0810();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400838);
#ifdef SKIP_ASM
extern int func_00423DA0(int *);
extern void func_00400810(int, int, int);
void func_00400838(unsigned short a) {
    int s[6];
    int h;
    s[1] = 1;
    s[2] = 0;
    s[5] = 0;
    h = func_00423DA0(s);
    func_00423B20(a, func_00400810, h);
    func_00423DE0(h);
    func_00423DB0(h);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_004008A0);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400908);
#ifdef SKIP_ASM
extern int func_00423DD0_i(int) __asm__("func_00423DD0");
void func_00400908(int *p) {
    D_00451658 = *p;
    D_0045165C = D_00451658;
    if (D_00451658 == 11) {
        D_00451658 = 0;
        D_00451634 = 0;
        return;
    }
    func_00423DD0_i(D_00451628);
    if (D_00451614 != 0 && D_005264C0 != 0)
        func_00423DD0_i(D_00451620);
    else
        D_00451634 = 0;
    D_00451658 = 0;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_004009A8);
#ifdef SKIP_ASM
extern char D_004979E0[];
extern void func_00423DE0(int);
extern int func_00423BE0(void);
extern void func_00425C60();
void func_004009A8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00400A78);
#ifdef SKIP_ASM
extern int func_00423C90();
extern int func_00423CA0_i(int, sThreadParam *) __asm__("func_00423CA0");
extern int func_00423BA0(sThreadParam *);
extern void func_00423BC0(int, int);
extern void func_00423C30(int, int);
extern void func_004009A8(void);
int func_00400A78(int a, int b, int c); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400B48);
#ifdef SKIP_ASM
struct S0B48 { int n0; int n1; unsigned char *d0; unsigned char *d1; unsigned char b0[0x40]; unsigned char b1[0x40]; };
extern void func_00400908(int *);
void func_00400B48(void *a) {
    struct S0B48 *s = (struct S0B48 *)((unsigned)a | 0x20000000);
    unsigned char *e;
    int i;
    unsigned char *d;
    if (s->n0 > 0) {
        e = (unsigned char *)((unsigned)s->d0 | 0x20000000);
        d = s->d0;
        for (i = 0; i < s->n0; i++) {
            unsigned char c = s->b0[i];
            e[i] = c;
            d[i] = c;
        }
    }
    if (s->n1 > 0) {
        e = (unsigned char *)((unsigned)s->d1 | 0x20000000);
        d = s->d1;
        for (i = 0; i < s->n1; i++) {
            unsigned char c = s->b1[i];
            e[i] = c;
            d[i] = c;
        }
    }
    func_00400908((int *)&D_00451658);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400C00);
#ifdef SKIP_ASM
struct QSema { int count; int max_count; int init_count; int wait_threads; int attr; int option; };
extern int Q0C00_create(struct QSema *) __asm__("func_00423DA0");
void func_00400C00(void) {
    struct QSema s;
    if (D_00451628 == -1 || D_0045162C == -1 || D_00451630 == -1) {
        s.option = 0;
        s.init_count = 1;
        s.max_count = 1;
        D_00451628 = Q0C00_create(&s);
        D_0045162C = Q0C00_create(&s);
        D_00451630 = Q0C00_create(&s);
        s.init_count = 0;
        D_00451620 = Q0C00_create(&s);
        D_00451634 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400CB8);
#ifdef SKIP_ASM
extern int cb_DB0(int) __asm__("func_00423DB0");
extern void cb_DC0(int) __asm__("func_00423DC0");
extern void func_00426050(int);
extern int cb_078() __asm__("func_0042C078");
extern void func_0042C0C0(void);
void func_00400CB8(void) {
    int s;
    if (D_00451614 != 0) {
        D_00451658 = -1;
        cb_DC0(D_00451620);
    }
    cb_DB0(D_00451628);
    cb_DB0(D_0045162C);
    cb_DB0(D_00451630);
    cb_DB0(D_00451620);
    s = cb_078();
    func_00426050(0x80000012);
    if (s)
        func_0042C0C0();
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00400D58);

INCLUDE_ASM("seg/seg_2FE810", func_00400DD8);
#ifdef SKIP_ASM
void func_00400DD8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00400E30);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
extern void func_00400DD8(void);
extern void func_00426020(int, void (*)(void), int);
int func_00400E30(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400EA8);
#ifdef SKIP_ASM
typedef struct { char b[0x24]; } R0EA8;
extern char D_00497A08[];
extern char D_00497A28[];
extern char D_00497A40[];
extern char D_00497A50[];
extern char D_00497A60[];
extern void func_00400C00();
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern void func_00423CA0(int, sThreadParam *);
extern void func_00425C60();
extern int func_004013E8(int);
extern void func_00426408(int);
extern void func_00426358(int, int);
int f0EA8(char *out, char *name, int arg) __asm__("func_00400EA8");
int f0EA8(char *out, char *name, int arg) {
    int i;
    int n;
    int v;
    func_00400C00();
    if (D_00451628 != func_00423DF0(D_00451628))
        return 0;
    D_0045161C = 1;
    func_00423CA0(D_005264D8, &D_005264E0);
    if (func_004013E8(1) != 0) {
        func_00423DC0(D_00451628);
        return 0;
    }
    func_00426408(0);
    if (D_00451644 < 0) {
        for (;;) {
            if (func_00426B48(&D_005266C0, 0x80000597, 0) < 0) {
                if (D_00451610 > 0)
                    func_00425C60(D_00497A08);
                for (i = 0x100000; i--; )
                    ;
                continue;
            }
            if (D_005266C0.server != 0) {
                D_00451644 = 0;
                break;
            }
            for (i = 0x100000; i--; )
                ;
        }
    }
    for (n = 0; n < 0x100; n++) {
        if ((D_00526540.name[n] = name[n]) == 0)
            break;
    }
    if (n == 0x100)
        D_00526540.name[0xFF] = 0;
    if (D_00451610 > 0)
        func_00425C60(D_00497A28, D_00526564);
    D_00526540.arg = arg;
    D_00526540.self = &D_00526540;
    func_00426358((int)&D_00526540, 0x12C);
    if (func_00426D18(&D_005266C0, 0, 0, &D_00526540, 0x12C, D_00526680, 4, 0, 0) < 0) {
        func_00423DC0(D_00451628);
        return 0;
    }
    *(R0EA8 *)out = *(R0EA8 *)((int)&D_00526540 | 0x20000000);
    if (D_00451610 > 0)
        func_00425C60(D_00497A40, out + 8);
    if (D_00451610 > 0)
        func_00425C60(D_00497A50, ((int *)out)[1]);
    if (D_00451610 > 0)
        func_00425C60(D_00497A60, ((int *)out)[0]);
    v = *(int *)((int)D_00526680 | 0x20000000);
    func_00423DC0(D_00451628);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004011C0);
#ifdef SKIP_ASM
extern int func_00400EA8(int, int, int);
int func_004011C0(int a, int b) {
    return func_00400EA8(a, b, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004011E0);
#ifdef SKIP_ASM
extern char D_00497A78[];
extern char D_00497AA0[];
extern void func_00400C00();
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern void func_00423CA0(int, sThreadParam *);
extern void func_00425C60();
extern int func_004013E8(int);
extern void func_00426408(int);
int func_004011E0(int a) {
    int i;
    func_00400C00();
    if (D_00451628 != func_00423DF0(D_00451628)) {
        if (D_00451610 > 0)
            func_00425C60(D_00497A78, a, D_0045161C);
        return 0;
    }
    D_0045161C = a;
    func_00423CA0(D_005264D8, &D_005264E0);
    if (func_004013E8(1) != 0) {
        func_00423DC0(D_00451628);
        return 0;
    }
    func_00426408(0);
    if (D_0045163C >= 0)
        return 1;
    for (;;) {
        if (func_00426B48(&D_004527D0, 0x80000595, 0) < 0) {
            if (D_00451610 > 0)
                func_00425C60(D_00497AA0);
            for (i = 0x100000; i--; )
                ;
            continue;
        }
        if (D_004527D0.server != 0)
            break;
        for (i = 0x100000; i--; )
            ;
    }
    D_0045163C = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401350);
#ifdef SKIP_ASM
extern int func_004011E0();
extern void func_00423DC0(int);
int func_00401350(void) {
    int v;
    if (func_004011E0(2) == 0)
        return 0;
    if (func_00426D18(&D_004527D0, 14, 0, 0, 0, D_00451680, 4, 0, 0) < 0) {
        func_00423DC0(D_00451628);
        return 0;
    }
    v = *(int *)((unsigned)D_00451680 | 0x20000000);
    func_00423DC0(D_00451628);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004013E8);
#ifdef SKIP_ASM
extern char D_00497AB8[];
extern void func_00425C60();
int func_004013E8(int a) {
    if (a == 0) {
        if (D_00451610 > 0)
            func_00425C60(D_00497AB8);
        while (D_00451634 || func_00426F08(&D_004527D0))
            func_00400838(60);
        return 0;
    }
    if (D_00451634 || func_00426F08(&D_004527D0))
        return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401488);
#ifdef SKIP_ASM
extern char D_00497AC8[];
extern void func_00425C60();
int func_00401488(int a) {
    if (a == 0) {
        if (D_00451610 > 0)
            func_00425C60(D_00497AC8);
        while (func_00426F08(&D_00453180))
            func_00400838(60);
        return 0;
    }
    return func_00426F08(&D_00453180);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004014F8);
#ifdef SKIP_ASM
extern char D_00497AD8[];
extern char D_00497B00[];
extern void func_00400C00();
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern void func_00423CA0(int, sThreadParam *);
extern void func_00425C60();
extern int func_00401488(int);
extern void func_00426408(int);
int func_004014F8(int a) {
    int i;
    func_00400C00();
    if (D_0045162C != func_00423DF0(D_0045162C)) {
        if (D_00451610 > 0)
            func_00425C60(D_00497AD8, a, D_00451618);
        return 0;
    }
    D_00451618 = a;
    func_00423CA0(D_005264D8, &D_005264E0);
    if (func_00401488(1) != 0) {
        func_00423DC0(D_0045162C);
        return 0;
    }
    func_00426408(0);
    if (D_0045164C >= 0)
        return 1;
    for (;;) {
        if (func_00426B48(&D_00453180, 0x80000593, 0) < 0) {
            if (D_00451610 > 0)
                func_00425C60(D_00497B00);
            for (i = 0x100000; i--; )
                ;
            continue;
        }
        if (D_00453180.server != 0)
            break;
        for (i = 0x100000; i--; )
            ;
    }
    D_0045164C = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401668);
#ifdef SKIP_ASM
int func_00401668(long id, int *out) {
    int v;
    if (id != 0x8001) {
        if (id != 0x8003) {
            if (id != 0x8002) {
                if (id != 0x8004) {
                    if (id != 0x8011) {
                        if (id != 0x8013) {
                            if (id != 0x8012) {
                                if (id == 0x8014)
                                    v = (int)&D_00453180;
                                else
                                    v = 0;
                            } else {
                                v = (int)D_00452C40;
                            }
                        } else {
                            v = (int)D_00452800;
                        }
                    } else {
                        v = D_0045162C;
                    }
                } else {
                    v = (int)&D_004527D0;
                }
            } else {
                v = (int)D_00451700;
            }
        } else {
            v = (int)D_00451680;
        }
    } else {
        v = D_00451628;
    }
    *out = v;
    return 1;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00401718);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004019F8);
#ifdef SKIP_ASM
extern char D_00497B48[];
extern char D_00497B60[];
extern char D_00497B80[];
extern void func_00425C60();
extern void func_00400C00();
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern int func_00401488(int);
extern void func_00426408(int);
extern void func_00426358(int, int);
int func_004019F8(int a) {
    int r;
    int v;
    if (D_00451610 > 0)
        func_00425C60(D_00497B48);
    func_00400C00();
    if (D_0045162C != func_00423DF0(D_0045162C))
        return 6;
    if (func_00401488(1) != 0) {
        func_00423DC0(D_0045162C);
        return a != 8 ? 6 : -1;
    }
    func_00426408(0);
    if (D_00451648 < 0) {
        for (;;) {
            if (func_00426B48(&D_00526710, 0x8000059A, 0) < 0) {
                if (D_00451610 > 0)
                    func_00425C60(D_00497B60);
                for (r = 0x100000; r--; )
                    ;
                continue;
            }
            if (D_00526710.server != 0) {
                D_00451648 = 0;
                break;
            }
            for (r = 0x100000; r--; )
                ;
        }
    }
    D_00526750[0] = a;
    func_00426358((int)D_00526750, 4);
    if (func_00426D18(&D_00526710, 0, 0, D_00526750, 4, D_00452800, 4, 0, 0) < 0) {
        func_00423DC0(D_0045162C);
        return a != 8 ? 6 : -1;
    }
    if (D_00451610 > 0)
        func_00425C60(D_00497B80);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    func_00423DC0(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401BF0);
#ifdef SKIP_ASM
extern char D_00497B98[];
extern char D_00497B60[];
extern char D_00497B80[];
extern void b1_log() __asm__("func_00425C60");
extern void b1_0C00() __asm__("func_00400C00");
extern int b1_3DF0(int) __asm__("func_00423DF0");
extern void b1_3DC0(int) __asm__("func_00423DC0");
extern void b1_6408(int) __asm__("func_00426408");
extern void b1_6358(int *, int) __asm__("func_00426358");
extern int b1_19F8(int) __asm__("func_004019F8");
/* The sema id passed to the Sema calls is read volatile throughout libcdvd (see the retail delay slots). */
static inline int semaId1BF0(void) {
    return D_00451630;
}
int func_00401BF0(int a) {
    int i;
    int r;
    int v;
    if (D_00451610 > 0)
        b1_log(D_00497B98);
    b1_0C00();
    if (D_00451630 != b1_3DF0(semaId1BF0()))
        return a != 8 ? 6 : -1;
    b1_6408(0);
    i = 0;
    if (D_00451648 < 0) {
        for (;;) {
            if (func_00426B48(&D_00526710, 0x8000059C, 0) < 0) {
                if (D_00451610 > 0)
                    b1_log(D_00497B60);
                for (r = 0x100000; r--; )
                    ;
                continue;
            }
            if (D_00526710.server != 0) {
                D_00451648 = 0;
                break;
            }
            if (i >= 0x11) {
                b1_3DC0(semaId1BF0());
                return b1_19F8(a);
            }
            for (r = 0x100000; r--; )
                ;
            i++;
        }
    }
    D_00526750[0] = a;
    b1_6358(D_00526750, 4);
    if (func_00426D18(&D_00526710, 0, 0, D_00526750, 4, D_00453080, 4, 0, 0) < 0) {
        b1_3DC0(semaId1BF0());
        return a != 8 ? 6 : -1;
    }
    if (D_00451610 > 0)
        b1_log(D_00497B80);
    v = *(int *)((unsigned)D_00453080 | 0x20000000);
    b1_3DC0(semaId1BF0());
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401DF8);
#ifdef SKIP_ASM
extern char D_00497BB0[];
extern char D_00497BC8[];
extern int func_00401350(void);
extern int func_004011E0();
extern void func_00425C60();
extern void func_00423DC0(int);
extern void func_00426358(int, int);
int func_00401DF8(int lsn, int sectors, int buf, unsigned char *mode) {
    int size;
    sCdReadCmd *p = (sCdReadCmd *)D_00451700;
    if ((D_00451638 & 1) == 0) {
        if (func_00401350() == 6)
            return 0;
    }
    if (func_004011E0(4) == 0)
        return 0;
    p->lsn = lsn;
    p->sectors = sectors;
    p->buf = buf;
    p->trycount = mode[0];
    p->spindlctrl = mode[1];
    p->datapattern = mode[2];
    p->ebuf = D_00452700;
    p->res = D_004527C0;
    switch (mode[2]) {
    case 1:
        size = sectors * 0x918;
        break;
    case 2:
        size = sectors * 0x924;
        break;
    case 0:
    default:
        size = sectors << 11;
        break;
    }
    D_004527C0[0] = 0;
    if ((D_00451638 & 2) == 0)
        func_00426358(buf, size);
    func_00426358((int)D_00452700, 0x90);
    func_00426358((int)p, 0x18);
    func_00426358((int)D_004527C0, 4);
    if (D_00451610 > 0)
        func_00425C60(D_00497BB0);
    D_00451658 = 1;
    D_00451634 = 1;
    if (func_00426D18(&D_004527D0, 1, 1, p, 0x18, 0, 0, func_00400B48, D_00452700) < 0) {
        D_00451658 = 0;
        D_00451634 = 0;
        func_00423DC0(D_00451628);
        return 0;
    }
    if (D_00451610 > 0)
        func_00425C60(D_00497BC8);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401FD8);
#ifdef SKIP_ASM
extern int func_004011E0();
extern void func_00426358(int, int);
extern void func_00423DC0(int);
int func_00401FD8(char *dst) {
    int v;
    int *p = D_00451700;
    if (func_004011E0(8) == 0)
        return 0;
    p[0] = (int)D_00526780;
    func_00426358((int)D_00526780, 0x810);
    func_00426358((int)p, 12);
    D_00451658 = 3;
    if (func_00426D18(&D_004527D0, 4, 0, p, 12, D_00451680, 8, 0, 0) < 0) {
        D_00451658 = 0;
        func_00423DC0(D_00451628);
        return 0;
    }
    D_00451658 = 0;
    v = D_00451680[0];
    if (D_00451680[1] == 0)
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 0x400);
    else
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 0x810);
    func_00423DC0(D_00451628);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402268);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void func_00423DC0(int);
int func_00402268(void) {
    int v;
    if (func_004014F8(1) == 0)
        return 0;
    if (func_00426D18(&D_00453180, 3, 0, 0, 0, D_00452800, 4, 0, 0) < 0) {
        func_00423DC0(D_0045162C);
        return 0;
    }
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    func_00423DC0(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402300);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void func_00423DC0(int);
int func_00402300(void) {
    int v;
    if (func_004014F8(3) == 0)
        return -1;
    if (func_00426D18(&D_00453180, 4, 0, 0, 0, D_00452800, 4, 0, 0) < 0) {
        func_00423DC0(D_0045162C);
        return -1;
    }
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    func_00423DC0(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402398);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void func_00423DC0(int);
int func_00402398(int *out) {
    int v;
    if (func_004014F8(0x21) == 0)
        return 0;
    if (func_00426D18(&D_00453180, 0x21, 0, 0, 0, D_00452800, 8, 0, 0) < 0) {
        func_00423DC0(D_0045162C);
        return 0;
    }
    *out = *(int *)((unsigned)(D_00452800 + 1) | 0x20000000);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    func_00423DC0(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402450);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void func_00423DC0(int);
extern void func_00426358_p(int *, int) __asm__("func_00426358");
int func_00402450(int a) {
    int v;
    int *p = D_00452C40;
    if (func_004014F8(34) == 0)
        return 0;
    D_00452C40[0] = a;
    func_00426358_p(p, 4);
    if (func_00426D18(&D_00453180, D_004531A8, 0, p, D_004531AC, D_00452800, D_004531B0, 0, 0) < 0) {
        func_00423DC0(D_0045162C);
        return 0;
    }
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    func_00423DC0(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402520);
#ifdef SKIP_ASM
struct P8 { char b[8]; } __attribute__((packed));
extern char D_00497BD8[];
extern char D_00497BF8[];
extern int func_004014F8();
extern void func_00423DC0(int);
extern void func_00425C60();
int func_00402520(struct P8 *a) {
    int v;
    if (func_004014F8(15) == 0)
        return 0;
    if (D_00451610 > 0)
        func_00425C60(D_00497BD8);
    if (func_00426D18(&D_00453180, 1, 0, 0, 0, D_00452800, 16, 0, 0) < 0) {
        func_00423DC0(D_0045162C);
        return 0;
    }
    *a = *(struct P8 *)((unsigned)&D_00452800[1] | 0x20000000);
    if (D_00451610 > 0)
        func_00425C60(D_00497BF8);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    func_00423DC0(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402618);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void func_00423DC0(int);
extern void func_00426358_p(int *, int) __asm__("func_00426358");
int func_00402618(int a, int *out) {
    int v;
    int *p = D_00452C40;
    if (func_004014F8(26) == 0)
        return -1;
    D_00452C40[0] = a;
    func_00426358_p(p, 4);
    if (func_00426D18(&D_00453180, 5, 0, p, 4, D_00452800, 8, 0, 0) < 0) {
        func_00423DC0(D_0045162C);
        return -1;
    }
    if (out)
        *out = *(int *)((unsigned)&D_00452800[1] | 0x20000000);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    func_00423DC0(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402708);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
extern void func_00409708(void);
int func_00402708(void) {
    int s = func_0042C078();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 &= 0xFFFFFEFF;
    *(volatile int *)0x1000B400 &= 0xFFFFFEFF;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (s)
        func_0042C0C0();
    *(volatile int *)0x1000B020 = 0;
    *(volatile int *)0x1000B420 = 0;
    func_00409708();
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004027B8);
#ifdef SKIP_ASM
extern char D_00497C20[];
extern int i7B8_memset(int, int, int) __asm__("func_00416210");
extern void i7B8_34A8(int *, char *) __asm__("func_004034A8");
extern void i7B8_2D50(int *, int, int) __asm__("func_00402D50");
extern unsigned i7B8_2D88(int *, int *, unsigned, unsigned) __asm__("func_00402D88");
extern void i7B8_3338(int *) __asm__("func_00403338");
extern int i7B8_2B58(int *) __asm__("func_00402B58");
extern int i7B8_2BB0(int *) __asm__("func_00402BB0");
extern void i7B8_2D68(int *) __asm__("func_00402D68");
extern int i7B8_3DB0(int *) __asm__("func_00403DB0");
extern int i7B8_3DD8(int *) __asm__("func_00403DD8");
// sceMpegCreate(mp, buff, size)
int func_004027B8(int *a, int buf, int size) {
    int *p;
    int *q;
    int v;
    int rem;
    i7B8_memset(buf, 0, size);
    p = (int *)(((unsigned)(buf + 3) >> 2) << 2);
    rem = size - ((int)p - buf);
    if ((unsigned)rem < 0x10C0) {
        i7B8_34A8(p, D_00497C20);
        return 0;
    }
    q = p + 0x108 / 4;
    a[16] = (int)p;
    i7B8_2D50(q, (int)p + 0x10C0, rem - 0x10C0);
    v = -1;
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    *(long *)&a[4] = -1;
    *(long *)&a[6] = -1;
    *(long *)&a[8] = 0;
    *(long *)&a[10] = -1;
    *(long *)&a[12] = -1;
    *(long *)&a[14] = 0;
    p[45] = 0;
    p[46] = 0;
    p[47] = 0;
    p[48] = 0;
    p[49] = 0;
    p[50] = 0;
    p[51] = 0;
    p[52] = 0;
    p[53] = 0;
    p[54] = 0;
    p[55] = 0;
    p[56] = 0;
    p[57] = 0;
    p[58] = 0;
    p[62] = 0;
    p[3] = 0;
    p[5] = 0;
    p[11] = 0;
    p[13] = 0;
    p[15] = 0;
    *(long *)&p[60] = -1;
    p[7] = (int)i7B8_3DB0;
    p[9] = (int)i7B8_3DD8;
    p[17] = i7B8_2D88(p, q, 0x600, 8);
    p[18] = 0;
    p[63] = 0;
    p[64] = 0;
    p[65] = 0;
    p[28] = 0;
    *(long *)&p[30] = 0;
    p[32] = v;
    *(long *)&p[34] = 0;
    p[36] = 0;
    p[43] = 0;
    p[37] = v;
    p[38] = v;
    p[39] = v;
    p[534] = (int)a;
    p[44] = 1;
    i7B8_3338(p);
    i7B8_2B58(a);
    i7B8_2BB0(a);
    p[110] = (int)p + 0x1E8;
    p[111] = (int)p + 0x250;
    p[113] = (int)p + 0x2B8;
    p[114] = (int)p + 0x320;
    p[115] = (int)p + 0x388;
    p[117] = (int)p + 0x3F0;
    p[118] = (int)p + 0x458;
    p[119] = (int)p + 0x4C0;
    p[121] = (int)p + 0x528;
    i7B8_2D68(q);
    p[532] = v;
    p[533] = 0;
    p[531] = 0;
    p[519] = 0x70003600;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004029C8);
#ifdef SKIP_ASM
int func_004029C8(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004029D0);
#ifdef SKIP_ASM
extern int func_00403500(int, int, int);
int func_004029D0(int *p, int b, int n) {
    func_00403500(p[16], b, (n + 19) / 16 * 16);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402A10);
#ifdef SKIP_ASM
extern int func_00402E10(int *);
int func_00402A10(int *a, unsigned b, int c) {
    int *p = (int *)a[16];
    b = (b & 0xFFFFFFF) | 0x20000000;
    p[44] = 1;
    p[54] = b;
    p[57] = c;
    p[56] = 0;
    p[55] = 0;
    return func_00402E10(a);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402A58);
#ifdef SKIP_ASM
extern int func_00402E10(int *);
int func_00402A58(int *a, unsigned b, int c) {
    int *p = (int *)a[16];
    b = (b & 0xFFFFFFF) | 0x20000000;
    p[57] = c;
    p[54] = b;
    p[44] = 0;
    p[56] = 0;
    p[55] = 0;
    return func_00402E10(a);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402AA0);
#ifdef SKIP_ASM
extern int func_00402E10(int *);
int func_00402AA0(int *a, unsigned b, int c, int d) {
    int *p = (int *)a[16];
    b = (b & 0xFFFFFFF) | 0x20000000;
    p[56] = d * 16;
    p[54] = b;
    p[57] = c * d;
    p[55] = c * 16;
    p[44] = 0;
    return func_00402E10(a);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402AF0);
#ifdef SKIP_ASM
void func_00402AF0(int *p) {
    int *q = (int *)p[16];
    q[0x428] = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B00);
#ifdef SKIP_ASM
void func_00402B00(int *p, int a, int b, int c) {
    int *q = (int *)p[16];
    q[37] = a;
    q[38] = b;
    q[39] = c;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B18);
#ifdef SKIP_ASM
void func_00402B18(int *p, int *a, int *b, int *c) {
    int *q = (int *)p[16];
    *a = q[37];
    *b = q[38];
    *c = q[39];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B38);
#ifdef SKIP_ASM
int func_00402B38(int *p) {
    int *q = (int *)p[16];
    return q[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B48);
#ifdef SKIP_ASM
int func_00402B48(int *p) {
    int *q = (int *)p[16];
    return q[1] == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B58);
#ifdef SKIP_ASM
extern void func_00403398(int *);
int func_00402B58(int *a) {
    int *p = (int *)a[16];
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    a[2] = 0;
    p[43] = 0;
    p[32] = -1;
    func_00403398(p);
    p[70] = 0;
    func_004032A8(p);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402BB0);
#ifdef SKIP_ASM
int func_00402BB0(int *p) {
    char *q = (char *)p[16];
    int *t;
    if ((t = *(int **)(q + 0x1B8))) t[10] = 0;
    if ((t = *(int **)(q + 0x1C8))) t[10] = 0;
    if ((t = *(int **)(q + 0x1D8))) t[10] = 0;
    if ((t = *(int **)(q + 0x1BC))) t[10] = 0;
    if ((t = *(int **)(q + 0x1CC))) t[10] = 0;
    if ((t = *(int **)(q + 0x1DC))) t[10] = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402C08);
#ifdef SKIP_ASM
int func_00402C08(int *p, int n, int fn, void *arg) {
    char *q = (char *)p[16];
    char *tbl = q + 0xC;
    char *ea = q + n * 8;
    char *eb = tbl + n * 8;
    int old;
    old = *(int *)eb;
    *(int *)eb = fn;
    *(void **)(ea + 0x10) = arg;
    return old;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00402C30);
#ifdef SKIP_ASM
struct FA_402C30 { int (*fn)(int *, int *, int); int arg; };
struct T_402C30 { char pad[12]; struct FA_402C30 e[1]; };
int func_00402C30(int *p, int *v); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402C80);
#ifdef SKIP_ASM
extern int func_00402C30(int *, int *);
int func_00402C80(int *p) {
    int v[8];
    v[0] = 1;
    func_00402C30(p, v);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CA8);
#ifdef SKIP_ASM
int func_00402CA8(int *p, long v) {
    int *q = (int *)p[16];
    q[28] = 1;
    *(long *)&q[30] = v;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CC0);
#ifdef SKIP_ASM
int func_00402CC0(int *p) {
    int *q = (int *)p[16];
    q[28] = 0;
    *(long *)&q[30] = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CD8);
#ifdef SKIP_ASM
int func_00402CD8(int *p, int v) {
    int *q = (int *)p[16];
    q[54] = v;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CE8);
#ifdef SKIP_ASM
int func_00402CE8(int *p) {
    int *q = (int *)p[16];
    return q[51];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CF8);
#ifdef SKIP_ASM
int func_00402CF8(int *p) {
    int *q = (int *)p[16];
    return q[52];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D08);
#ifdef SKIP_ASM
char *func_00402D08(int *p) {
    return (char *)p[16] + 0xB4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D18);
#ifdef SKIP_ASM
char *func_00402D18(int *p) {
    return (char *)p[16] + 0xB4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D28);
#ifdef SKIP_ASM
int func_00402D28(int *p, int v) {
    int *q = (int *)p[16];
    int old = q[58];
    q[58] = v;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D38);
#ifdef SKIP_ASM
int func_00402D38(int *p, long v) {
    int *q = (int *)p[16];
    *(long *)&q[60] = v;
    q[62] = 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D50);
#ifdef SKIP_ASM
void func_00402D50(int *p, int a, int b) {
    p[0] = a;
    p[1] = b;
    p[2] = a;
    p[3] = a;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D68);
#ifdef SKIP_ASM
int func_00402D68(int *p) {
    return p[3] = p[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D78);
#ifdef SKIP_ASM
int func_00402D78(int *p) {
    return p[2] = p[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D88);
#ifdef SKIP_ASM
extern char D_00497C48[];
extern void func_004034A8(int *, int);
unsigned func_00402D88(int *a, unsigned *p, unsigned size, unsigned align) {
    unsigned s = (p[2] + align - 1) / align * align;
    unsigned e = s + size;
    if (p[0] + p[1] >= e) {
        p[2] = e;
        return s;
    }
    func_004034A8(a, (int)D_00497C48);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402DF8);
#ifdef SKIP_ASM
int func_00402DF8(int *p) {
    return p[0] + p[1] - p[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402E10);
#ifdef SKIP_ASM
extern char D_00497C68[];
extern void func_00403470_k() __asm__("func_00403470");
extern int func_00403240();
extern int func_00403098();
extern int func_00407520();
int func_00402E10(int *a) {
    int *p = (int *)a[16];
    int r = 0;
    int s = 1;
    p[0] = 0;
    if ((p[54] & 0x3F) != 0) {
        func_00403470_k(p, D_00497C68, p[54]);
        return -1;
    }
    p[520] = 0;
    for (;;) {
        if (r != -1) {
            do {
                s = func_00407520(p);
                if (s == 0)
                    break;
                if (p[93] == p[53])
                    break;
            } while (p[530] != 0);
        }
        switch (s) {
        case 0:
            func_00403240(a);
            p[0] = 1;
            break;
        case 1:
            p[42] = 0;
            p[41] = 0;
            p[40] = 0;
            r = func_00403098(a, 0, p[37]);
            p[40]++;
            break;
        case 2:
            r = func_00403098(a, p[41], p[38]);
            p[41]++;
            break;
        case 3:
        case 4:
            r = func_00403098(a, p[42], p[39]);
            p[42]++;
            break;
        }
        if (p[520] != 0)
            return 1;
        if (p[0] != 0)
            return 1;
    }
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00402F80);
#ifdef SKIP_ASM
struct P_2F80 { char pad[4]; int f4; int f8; char padC[0xA0]; int fAC; char pad2[0x68]; int f118; char pad3[4]; int f120; char pad4[0x50]; int f174; };
struct A_2F80 { char pad[8]; int f8; char pad2[0x34]; struct P_2F80 *f40; };
extern int s2_7F20(struct P_2F80 *, int) __asm__("func_00407F20");
extern int s2_7DC0(struct P_2F80 *) __asm__("func_00407DC0");
extern void s2_2C80(struct A_2F80 *) __asm__("func_00402C80");
extern void s2_7E90(struct P_2F80 *, int, int) __asm__("func_00407E90");
int func_00402F80(struct A_2F80 *a, int b, int c); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403098);
#ifdef SKIP_ASM
struct P_3098 { char pad[0x174]; int f174; };
struct A_3098 { char pad[0x40]; struct P_3098 *f40; };
extern int s98_30E0(struct A_3098 *, int, int) __asm__("func_004030E0");
extern int s98_2F80(struct A_3098 *, int, int) __asm__("func_00402F80");
int func_00403098(struct A_3098 *a, int b, int c) {
    if (a->f40->f174 != 3) return s98_30E0(a, b, c);
    return s98_2F80(a, b, c);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_004030E0);
#ifdef SKIP_ASM
struct P_30E0 { int f0; int f4; int f8; char padC[0xA0]; int fAC; char pad2[0x24]; int fD4; char pad3[0x40]; int f118; char pad4[4]; int f120; char pad5[0x50]; int f174; };
struct A_30E0 { char pad[8]; int f8; char pad2[0x34]; struct P_30E0 *f40; };
extern int s3_7F20(struct P_30E0 *, int) __asm__("func_00407F20");
extern int s3_7DC0(struct P_30E0 *) __asm__("func_00407DC0");
extern void s3_2C80(struct A_30E0 *) __asm__("func_00402C80");
extern void s3_7E90(struct P_30E0 *, int, int) __asm__("func_00407E90");
extern int s3_7520(struct P_30E0 *) __asm__("func_00407520");
extern void s3_3240(struct A_30E0 *) __asm__("func_00403240");
int func_004030E0(struct A_30E0 *a, int b, int c); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403240);
#ifdef SKIP_ASM
extern void func_004032B8(int *);
int func_00403240(int *a) {
    int *s = (int *)a[16];
    int r = 0;
    if (s[1] != 0 && s[2] != 0) {
        func_004032B8(s);
        a[2] = s[70] - s[43];
        s[1] = 0;
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004032A8);
#ifdef SKIP_ASM
extern void func_00405B48(unsigned);
void func_004032A8(int *p) {
    p[530] = 0;
    func_00405B48(1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004032B8);
#ifdef SKIP_ASM
extern void func_004034A8(int *, int);
extern char D_00497CC8[];
extern void func_004086C0(int *, int, int);
extern void func_004087D0(int *, int, int, int);
void func_004032B8(int *p) {
    int n = p[0x118 / 4];
    if (p[0x120 / 4] != 0) {
        func_004034A8(p, (int)D_00497CC8);
    } else if (p[0x174 / 4] == 3) {
        func_004086C0(p, p[0x1BC / 4], n - 1);
    } else {
        func_004087D0(p, p[0x1CC / 4], p[0x1DC / 4], n - 1);
    }
    p[0x120 / 4] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403338);
#ifdef SKIP_ASM
extern void func_00405B48(unsigned);
void func_00403338(int *p) {
    int v = 0x70000000;
    func_00405B48(1);
    p[356] = v;
    p[357] = 0x70001800;
    p[436] = 0x70001B00;
    p[437] = 0x70003300;
    p[516] = 0;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00403398);
#ifdef SKIP_ASM
extern int func_0042C078_0() __asm__("func_0042C078");
extern void func_0042C0C0(void);
extern void func_00409638_2(int, int) __asm__("func_00409638");
void func_00403398(int *p); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403460);
#ifdef SKIP_ASM
extern char D_00497CE8[];
extern void func_00425C60(char *, int);
void func_00403460(int a) {
    func_00425C60(D_00497CE8, a);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403470);
#ifdef SKIP_ASM
extern int sprintf(char *, const char *, ...);
extern void func_004034A8(int *, int);
void func_00403470(int *a, char *fmt) {
    char buf[256];
    sprintf(buf, fmt);
    func_004034A8(a, (int)buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004034A8);
#ifdef SKIP_ASM
extern int func_00402C30(int *, int *);
extern void func_00403460(int);
void func_004034A8(int *p, int b) {
    int v[2];
    int *q = (int *)p[534];
    if (q && p && p[3]) {
        v[0] = 0;
        v[1] = b;
        func_00402C30(q, v);
        return;
    }
    func_00403460(b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403500);
#ifdef SKIP_ASM
extern void func_00424020(int) __asm__("func_00424020");
extern int func_0042C078();
extern void func_0042C0C0(void);
void q3500(int base, unsigned addr, int n) __asm__("func_00403500");
void q3500(int base, unsigned addr, int n) {
    unsigned t = ((unsigned)(base + 0x88F) >> 4) << 4;
    long *tag = (long *)t;
    int ret;
    while (n > 0) {
        int chunk = n > 0xFFF40 ? 0xFFF40 : n;
        int id = 0;
        long hi = addr & 0xFFFFFFF;
        n -= chunk;
        if (n != 0) id = 3;
        *tag = (hi << 32) | ((long)id << 28) | (unsigned long)(unsigned)((chunk + 0xF) / 16);
        __asm__ volatile("sync");
        addr += chunk;
        tag += 2;
    }
    func_00424020(0);
    ret = func_0042C078();
    *(volatile int *)0x1000B430 = t & 0xFFFFFFF;
    *(volatile int *)0x1000B420 = 0;
    *(volatile int *)0x1000B400 = 0x105;
    if (ret)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403600);
#ifdef SKIP_ASM
int func_00403600(int *p, int a, int b) {
    p[1] = a;
    p[2] = b;
    p[3] = a >> 4;
    p[4] = b >> 4;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403620);
#ifdef SKIP_ASM
extern char D_00453200[];
extern char D_00453240[];
extern char D_00497CF8[];
extern int func_004072E8();
extern void func_00403AD0();
extern void func_00403748();
extern void func_004076F8();
void func_00403620(int *p) {
    unsigned v;
    int w;
    int b;
    p[0xD4 / 4] = 0;
    v = func_004072E8(p, 32);
    w = (v >> 8) & 0xFFF;
    p[0x124 / 4] = v >> 20;
    p[0x128 / 4] = w;
    if (w >= 0xAF1)
        func_004034A8(p, (int)D_00497CF8);
    v = func_004072E8(p, 30);
    p[0x134 / 4] = v >> 12;
    p[0x138 / 4] = (v >> 1) & 0x3FF;
    b = func_004072E8(p, 1);
    p[0x840 / 4] = b;
    if (b != 0) {
        func_00406E20((int)p);
        func_00406DF0(p, 0x50000000);
        func_00406E20((int)p);
    } else {
        func_00403AD0(p, 0x50000000, D_00453200);
    }
    b = func_004072E8(p, 1);
    p[0x844 / 4] = b;
    if (b != 0) {
        func_00406E20((int)p);
        func_00406DF0(p, 0x58000000);
        func_00406E20((int)p);
    } else {
        func_00403AD0(p, 0x58000000, D_00453240);
    }
    func_004076F8(p);
    func_00403748(p[0x858 / 4]);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00403748);

INCLUDE_ASM("seg/seg_2FE810", func_004039F0);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403AD0);
#ifdef SKIP_ASM
extern int f3AD0_DI() __asm__("func_0042C078");
extern void f3AD0_EI(void) __asm__("func_0042C0C0");
extern int f3AD0_2C30(int, int *) __asm__("func_00402C30");
extern void f3AD0_6E20(int *) __asm__("func_00406E20");
extern void f3AD0_6DF0(int *, int) __asm__("func_00406DF0");
void func_00403AD0(int *a, int b, int c) {
    int v[8];
    int s;
    v[0] = 2;
    f3AD0_2C30(a[534], v);
    f3AD0_6E20(a);
    *(volatile int *)0x10002000 = 0;
    f3AD0_6E20(a);
    s = f3AD0_DI();
    *(volatile int *)0x1000B410 = c & 0xFFFFFFF;
    *(volatile int *)0x1000B420 = 4;
    *(volatile int *)0x1000B400 = 0x101;
    if (s) f3AD0_EI();
    f3AD0_6DF0(a, b);
    f3AD0_6E20(a);
    v[0] = 3;
    f3AD0_2C30(a[534], v);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403BB0);
#ifdef SKIP_ASM
extern char D_00497D10[];
extern char D_00497D38[];
extern void func_00405B48(unsigned);
void func_00403BB0(int *p) {
    unsigned v;
    unsigned h;
    unsigned w;
    int t;
    unsigned x;
    unsigned y;
    unsigned z;
    p[0x848 / 4] = 1;
    func_00405B48(0);
    v = func_004072E8(p, 28);
    x = (v >> 1) & 0xFFF;
    t = (v >> 17) & 3;
    y = (v >> 13) & 3;
    z = (v >> 15) & 3;
    p[0x140 / 4] = t;
    if (t != 1)
        func_004034A8(p, (int)D_00497D10);
    p[0x13C / 4] = (v >> 19) & 1;
    h = v >> 20;
    w = (unsigned)func_004072E8(p, 16) >> 8;
    if (h != 0x48 && h != 0x58 && h != 0x44)
        func_004034A8(p, (int)D_00497D38);
    p[0x124 / 4] = (z << 12) | (p[0x124 / 4] & 0xFFF);
    p[0x128 / 4] = (y << 12) | (p[0x128 / 4] & 0xFFF);
    p[0x134 / 4] = p[0x134 / 4] + (x << 18);
    p[0x138 / 4] = p[0x138 / 4] + (w << 10);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403CE0);
#ifdef SKIP_ASM
extern int func_004072E8(int, int);
void func_00403CE0(int *p) {
    func_004072E8((int)p, 3);
    if (func_004072E8((int)p, 1)) {
        func_004072E8((int)p, 8);
        func_004072E8((int)p, 8);
        p[0x144 / 4] = func_004072E8((int)p, 8);
    }
    p[0x148 / 4] = func_004072E8((int)p, 0xE);
    func_004072E8((int)p, 1);
    p[0x14C / 4] = func_004072E8((int)p, 0xE);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _sequenceScalableExtension);
#ifdef SKIP_ASM
extern char D_00497D58[];
extern void func_004034A8(int *, int);
void _sequenceScalableExtension(int *p) {
    func_004034A8(p, (int)D_00497D58);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403D80);
#ifdef SKIP_ASM
extern char D_00497D88[];
extern void func_004034A8(int *, int);
void func_00403D80(int *p) {
    func_004034A8(p, (int)D_00497D88);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403D90);
#ifdef SKIP_ASM
extern char D_00497DA0[];
extern void func_004034A8(int *, int);
void func_00403D90(int *p) {
    func_004034A8(p, (int)D_00497DA0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403DA0);
#ifdef SKIP_ASM
extern char D_00497DD8[];
extern void func_004034A8(int *, int);
void func_00403DA0(int *p) {
    func_004034A8(p, (int)D_00497DD8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403DB0);
#ifdef SKIP_ASM
extern void func_00409400(void *);
int func_00403DB0(int *p) {
    func_00409400((char *)p[16] + 0x4C);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403DD8);
#ifdef SKIP_ASM
extern void func_004094E8(void *);
int func_00403DD8(int *p) {
    func_004094E8((char *)p[16] + 0x4C);
    return 1;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00403E00);

INCLUDE_ASM("seg/seg_2FE810", func_00404050);

INCLUDE_ASM("seg/seg_2FE810", func_00404758);

INCLUDE_ASM("seg/seg_2FE810", func_00404B78);

INCLUDE_ASM("seg/seg_2FE810", func_00404D98);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404D98(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00404E10);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404E10(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00404EA8);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404EA8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00404F60);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404F60(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405030);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405030(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_004050E0);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004050E0(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405198);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405198(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405290);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405290(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405390);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405390(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405430);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405430(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_004054E0);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004054E0(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_004055C0);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004055C0(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_004056A8);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004056A8(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405780);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405780(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405850);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405850(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405970);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405970(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405A88);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405A88(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00405AE8);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405AE8(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405B48);
#ifdef SKIP_ASM
void func_00405B48(unsigned a) {
    volatile unsigned *r = (volatile unsigned *)0x10002010;
    *r = (*r & 0xFF7FFFFF) | (a << 23);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405B70);
#ifdef SKIP_ASM
extern void func_00406E20(int);
extern int func_00402C30(int *, int *);
extern void func_004034A8(int *, int);
extern int func_0042C078();
extern void func_0042C0C0(void);
extern char D_00497EB0[];
struct M5B70 { char pad0[0x838]; int f838; int f83C; char pad840[0x18]; int *f858; char pad85C[0x10A0 - 0x85C]; int f10A0; };
int f5B70(struct M5B70 *p) __asm__("func_00405B70");
int f5B70(struct M5B70 *p) {
    int cmd[8];
    int cmd2[8];
    int ret = 1;
    int st;
    long top;
    int bp;
    int n;
    func_00406E20((int)p);
    while (*(volatile int *)0x1000B020 != 0 && !(*(volatile int *)0x10002010 & 0x4000)) {
        if (*(volatile int *)0x1000B420 == 0 && !(*(volatile int *)0x1000B400 & 0x100)) {
            cmd[0] = 1;
            func_00402C30(p->f858, cmd);
        }
    }
    if (p->f10A0 != 0) {
        st = func_0042C078();
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
        *(volatile int *)0x1000B400 = 0;
        *(volatile int *)0x1000B000 = 0;
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & ~0x10000;
        if (st)
            func_0042C0C0();
        *(volatile int *)0x1000B020 = 0;
        *(volatile int *)0x1000B420 = 0;
        *(volatile int *)0x10002010 = 0x40000000;
        return 0;
    }
    bp = *(volatile int *)0x10002020;
    top = *(long *)0x10002030;
    p->f838 = (int)top;
    if (top < 0) {
        if (bp & 0x1F)
            n = 0x20 - (bp & 0x1F);
        else
            n = 0;
        p->f83C = n;
    } else {
        p->f83C = 0x20;
    }
    if (*(volatile int *)0x10002010 & 0x4000) {
        func_004034A8((int *)p, (int)D_00497EB0);
        cmd2[0] = 2;
        func_00402C30(p->f858, cmd2);
        *(int *)0x10002010 = 0x40000000;
        cmd2[0] = 3;
        func_00402C30(p->f858, cmd2);
        st = func_0042C078();
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
        *(volatile int *)0x1000B000 = 0;
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & ~0x10000;
        if (st)
            func_0042C0C0();
        ret = 0;
        *(volatile int *)0x1000B020 = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405E10);
#ifdef SKIP_ASM
extern int func_00406F78(int, int);
int func_00405E10(int a) {
    return func_00406F78(a, 3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405E30);
#ifdef SKIP_ASM
#define H1(x) (((x) + ((x) > 0)) >> 1)
#define H3(x) ((3 * (x) + ((x) > 0)) >> 1)
void func_00405E30(int *p, int *o, int *in, int a, int b) {
    if (p[93] == 3) {
        if (p[94] != 0) {
            o[0] = H1(a) + in[0];
            o[1] = H1(b) + in[1] - 1;
            o[2] = H3(a) + in[0];
            o[3] = H3(b) + in[1] + 1;
        } else {
            o[0] = H3(a) + in[0];
            o[1] = H3(b) + in[1] - 1;
            o[2] = H1(a) + in[0];
            o[3] = H1(b) + in[1] + 1;
        }
    } else {
        o[0] = H1(a) + in[0];
        o[1] = H1(b) + in[1];
        if (p[93] == 1)
            o[1] = o[1] - 1;
        else
            o[1] = o[1] + 1;
    }
}
#undef H1
#undef H3
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405FB8);
#ifdef SKIP_ASM
extern char D_00497ED0[];
extern int lx5_6F78(int *, int) __asm__("func_00406F78");
extern int lx5_70F0(int *, int) __asm__("func_004070F0");
extern void lx5_71F8(int *, int) __asm__("func_004071F8");
extern void lx5_3470(int *, char *, int) __asm__("func_00403470");
int func_00405FB8(int *p) {
    int sum = 0;
    int c;
    int t;
    int again;
    do {
        c = lx5_6F78(p, 0);
        switch ((unsigned)c) {
        case 0x23:
            again = 1;
            sum += 0x21;
            break;
        case 0x22:
            again = 1;
            break;
        case 0:
            t = lx5_70F0(p, 11);
            if (p[0x848 / 4] != 0 && t == 15) {
                lx5_71F8(p, 11);
                again = 1;
                break;
            }
            lx5_3470(p, D_00497ED0, c);
            p[0x11C / 4] = 1;
            return 1;
        default:
            sum += c;
            again = 0;
            break;
        }
    } while (again);
    return sum;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004060C8);
#ifdef SKIP_ASM
extern char D_00497F08[];
extern int func_00406318(int *, int);
extern void func_00406E20(int);
extern int func_00405B70(int *);
extern void func_00404B78(int *, int);
extern void func_004034A8(int *, int);
int func_004060C8(int *p) {
    int n;
    int r;
    p[0x810 / 4] = 0;
    n = p[0x12C / 4] * p[0x130 / 4];
    p[0x814 / 4] = 0;
    if (p[0x174 / 4] != 3)
        n >>= 1;
    do {
        r = func_00406318(p, n);
    } while (r == 1 || r == 3);
    func_00406E20((int)p);
    if (func_00405B70(p) == 0) {
        r = 4;
        if (p[0x10A0 / 4] != 0)
            p[0x10A0 / 4] = 0;
        else
            r = 2;
    }
    while ((*(volatile unsigned *)0x1000D400 >> 8) & 1)
        ;
    if (r == 0)
        func_00404B78(p, p[0x810 / 4] == 0);
    if ((unsigned)(r - 1) < 2)
        func_004034A8(p, (int)D_00497F08);
    return r == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _sliceA0);
#ifdef SKIP_ASM
extern char D_00497F28[];
extern char D_00497F50[];
extern void func_00403470_k() __asm__("func_00403470");
extern int func_004070F0(int, int);
extern void func_004071F8(int, int);
extern void func_00407438();
extern int func_00405FB8(int *);
int _sliceA0(int *p, int unused, int *o1, int *o2, int *s) {
    int v;
    int a;
    int b;
    p[0x11C / 4] = 0;
    func_00407438();
    v = func_004070F0((int)p, 32);
    if ((unsigned)(v - 0x101) >= 0xAF) {
        func_00403470_k(p, D_00497F28, v);
        return 2;
    }
    func_004071F8((int)p, 32);
    a = func_004074B8(p);
    b = func_00405FB8(p);
    *o2 = b;
    if (p[0x11C / 4] != 0) {
        func_004034A8(p, (int)D_00497F50);
        return 1;
    }
    *o1 = ((((a << 7) + (v & 0xFF)) - 1) * p[0x12C / 4] + b) - 1;
    *o2 = 1;
    p[0x1B0 / 4] = 1;
    s[5] = 0;
    s[4] = 0;
    s[1] = 0;
    s[0] = 0;
    s[7] = 0;
    s[6] = 0;
    s[3] = 0;
    s[2] = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406318);
#ifdef SKIP_ASM
extern char D_00497F70[];
extern int _sliceA0(int *, int, int *, int *, int *);
extern int func_00405B70(int *);
extern int func_004070F0(int, int);
extern int func_00405FB8(int *);
extern void func_004034A8(int *, int);
extern int func_004065D0(int *, int *, int *, int *, int *, int *, int *);
extern int func_00406510(int *, int *, int *, int *, int *);
extern int func_00403E00_k() __asm__("func_00403E00");
extern void func_00404B78(int *, int);
int func_00406318(int *p, int n) {
    int a;
    int b;
    int c;
    int d;
    int e;
    int s[8];
    int q[4];
    int r[4];
    int v;
    a = 0;
    b = 0;
    v = _sliceA0(p, n, &a, &b, s);
    if (v != 0)
        return v;
    p[0x11C / 4] = 0;
    for (;;) {
        if (a >= n)
            return 0;
        p[(0x6CC + p[0x810 / 4] * 0x140) / 4] = 0;
        if (func_00405B70(p) == 0) {
            if (p[0x10A0 / 4] != 0) {
                p[0x10A0 / 4] = 0;
                return 4;
            }
            return 2;
        }
        if (b == 0) {
            if (func_004070F0((int)p, 0x17) == 0) {
                p[0x11C / 4] = 0;
                return 3;
            }
            if (p[0x11C / 4] != 0) {
                p[0x11C / 4] = 0;
                return 3;
            }
            b = func_00405FB8(p);
            if (p[0x11C / 4] != 0) {
                p[0x11C / 4] = 0;
                return 1;
            }
        }
        if (a >= n) {
            func_004034A8(p, (int)D_00497F70);
            return 2;
        }
        if (b == 1) {
            if (func_004065D0(p, &c, &d, &e, s, q, r) == 0) {
                p[0x11C / 4] = 0;
                return 1;
            }
        } else {
            if (func_00406510(p, s, &d, q, &c) == 0) {
                p[0x11C / 4] = 0;
                return 2;
            }
        }
        if (func_00403E00_k(p, a, b, c, d, s, q, r) == 0) {
            p[0x11C / 4] = 0;
            return 2;
        }
        if (a != 0)
            func_00404B78(p, p[0x810 / 4] ^ 1);
        a++;
        p[0x810 / 4] ^= 1;
        b--;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406510);
#ifdef SKIP_ASM
extern char D_00497F90[];
extern void func_004034A8(int *, int);
int func_00406510(int *p, int *q, int *r, int *s, int *t) {
    int ok = 1;
    int v;
    p[p[0x810 / 4] * 0x140 / 4 + 0x6CC / 4] = 1;
    p[0x1B0 / 4] = 1;
    if (p[0x150 / 4] == 2) {
        q[5] = 0;
        q[4] = 0;
        q[1] = 0;
        q[0] = 0;
    }
    if (p[0x174 / 4] == 3) {
        *r = 2;
    } else {
        *r = 1;
        v = p[0x174 / 4] == 2;
        s[1] = v;
        s[0] = v;
    }
    if (p[0x150 / 4] == 1) {
        func_004034A8(p, (int)D_00497F90);
        ok = 0;
    }
    *t &= ~1;
    return ok;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_004065D0);

INCLUDE_ASM("seg/seg_2FE810", func_00406A78);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406B00);
#ifdef SKIP_ASM
extern int Q_72E8(int, int) __asm__("func_004072E8");
extern void Q_6CA0(int, int *, int *, int, int, int, int, int) __asm__("func_00406CA0");
void func_00406B00(int p, int *x, int *out, int *t, int idx, int m, int k, int n0, int n1, int f1, int f2) {
    int v;
    int *q;
    if (m == 1) {
        if (k == 0 && f1 == 0) {
            v = Q_72E8(p, 1);
            t[idx] = v;
            t[idx + 2] = v;
        }
        q = &x[idx * 2];
        Q_6CA0(p, q, out, n0, n1, f1, f2, 0);
        q[4] = q[0];
        q[5] = q[1];
    } else {
        t[idx] = Q_72E8(p, 1);
        Q_6CA0(p, &x[idx * 2], out, n0, n1, f1, f2, 0);
        t[idx + 2] = Q_72E8(p, 1);
        Q_6CA0(p, &x[idx * 2 + 4], out, n0, n1, f1, f2, 0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406CA0);
#ifdef SKIP_ASM
extern int func_00406F78();
extern int func_004072E8();
extern int func_00405E10();
extern void func_00406A78();
void func_00406CA0(int p, int *x, int *out, int n0, int n1, int f1, int f2, int z) {
    int h;
    int v;
    h = func_00406F78(p, 2);
    if (n0 != 0 && h != 0)
        v = func_004072E8(p, n0);
    else
        v = 0;
    func_00406A78(x, n0, h, v, z);
    if (f1 != 0)
        out[0] = func_00405E10(p);
    h = func_00406F78(p, 2);
    if (n1 != 0 && h != 0)
        v = func_004072E8(p, n1);
    else
        v = 0;
    if (f2 != 0)
        x[1] = x[1] >> 1;
    func_00406A78(x + 1, n1, h, v, z);
    if (f2 != 0)
        x[1] = x[1] * 2;
    if (f1 != 0)
        out[1] = func_00405E10(p);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406DF0);
#ifdef SKIP_ASM
void func_00406DF0(int *p, unsigned a) {
    volatile int *r = (volatile int *)0x10002000;
    *r = a;
    p[518] = D_004532C0[a >> 28];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406E20);
#ifdef SKIP_ASM
extern int func_00402C80(int *);
void func_00406E20(int p) {
    int n = 0;
    if ((*(volatile unsigned *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 0x1389) {
                func_00402C80(*(int **)(p + 0x858));
                n = 0;
            }
        } while ((*(volatile unsigned *)0x10002010 & 0x80004000) == 0x80000000);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406EC8);
#ifdef SKIP_ASM
extern int func_00402C80(int *);
long func_00406EC8(int p) {
    int n = 0;
    long v = *(volatile long *)0x10002000;
    if (v < 0 && !(*(volatile unsigned *)0x10002010 & 0x4000)) {
        do {
            if (n++ >= 0x1389) {
                func_00402C80(*(int **)(p + 0x858));
                n = 0;
            }
            v = *(volatile long *)0x10002000;
        } while (v < 0 && !(*(volatile unsigned *)0x10002010 & 0x4000));
    }
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406F78);
#ifdef SKIP_ASM
extern int func_00402C80(int *);
int func_00406F78(int p, int cmd) {
    int m = 0;
    int n = 0;
    int w;
    long v;
    long b;
    int s;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 0x1389) {
                func_00402C80(*(int **)(p + 0x858));
                n = 0;
            }
        } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
    }
    w = (cmd << 26) | 0x30000000;
    *(volatile int *)0x10002000 = w;
    *(int *)(p + 0x818) = D_004532C0[w >> 28];
    v = *(volatile long *)0x10002000;
    if (v < 0) {
        do {
            if (m++ >= 0x1389) {
                func_00402C80(*(int **)(p + 0x858));
                m = 0;
            }
            v = *(volatile long *)0x10002000;
        } while (v < 0);
    }
    s = *(volatile int *)0x10002020;
    b = *(long *)0x10002030;
    *(int *)(p + 0x838) = (int)b;
    if (b < 0)
        *(int *)(p + 0x83C) = -(s & 0x1F) & 0x1F;
    else
        *(int *)(p + 0x83C) = 32;
    *(int *)(p + 0x11C) = (int)v == 0;
    return (short)v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004070F0);
#ifdef SKIP_ASM
extern int func_00402C80(int *);
extern long func_00406EC8_k() __asm__("func_00406EC8");
int func_004070F0(int p, int n) {
    int c;
    int x;
    int *e;
    if (*(int *)(p + 0x818) || *(int *)(p + 0x83C) < n) {
        c = 0;
        if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
            do {
                if (c++ >= 0x1389) {
                    func_00402C80(*(int **)(p + 0x858));
                    c = 0;
                }
            } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
        }
        *(volatile int *)0x10002000 = 0x40000000;
        *(int *)(p + 0x818) = D_004532C0[4];
        x = (int)func_00406EC8_k(p);
        *(int *)(p + 0x838) = x;
        *(int *)(p + 0x83C) = 32;
    }
    return (unsigned)*(int *)(p + 0x838) >> (32 - n);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004071F8);
#ifdef SKIP_ASM
extern int func_00402C80(int *);
extern long func_00406EC8_k() __asm__("func_00406EC8");
void func_004071F8(int p, int v) {
    int n = 0;
    int w;
    int x;
    int *e;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 0x1389) {
                func_00402C80(*(int **)(p + 0x858));
                n = 0;
            }
        } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
    }
    w = v | 0x40000000;
    *(volatile int *)0x10002000 = w;
    e = &D_004532C0[(unsigned)w >> 28];
    *(int *)(p + 0x818) = *e;
    x = (int)func_00406EC8_k(p, e);
    *(int *)(p + 0x838) = x;
    *(int *)(p + 0x83C) = 32;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004072E8);
#ifdef SKIP_ASM
extern int func_00402C80(int *);
extern long func_00406EC8_k() __asm__("func_00406EC8");
int func_004072E8(int p, int n) {
    int c = 0;
    int w;
    unsigned r;
    int x;
    int *e;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (c++ >= 0x1389) {
                func_00402C80(*(int **)(p + 0x858));
                c = 0;
            }
        } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
    }
    if (*(int *)(p + 0x818) || *(int *)(p + 0x83C) < n) {
        *(volatile int *)0x10002000 = 0x40000000;
        *(int *)(p + 0x818) = D_004532C0[4];
        x = (int)func_00406EC8_k(p);
        *(int *)(p + 0x838) = x;
    }
    r = (unsigned)*(int *)(p + 0x838) >> (32 - n);
    *(int *)(p + 0x83C) = 32;
    w = n | 0x40000000;
    *(volatile int *)0x10002000 = w;
    e = &D_004532C0[(unsigned)w >> 28];
    *(int *)(p + 0x818) = *e;
    x = (int)func_00406EC8_k(p);
    *(int *)(p + 0x838) = x;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407438);
#ifdef SKIP_ASM
extern void func_004071F8(int, int);
extern int func_004070F0(int, int);
extern void func_00406E20(int);
void func_00407438(int p) {
    int n;
    func_00406E20(p);
    n = -(*(volatile int *)0x10002020 & 7) & 7;
    if (n)
        func_004071F8(p, n);
    while (func_004070F0(p, 0x18) != 1)
        func_004071F8(p, 8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004074B8);
#ifdef SKIP_ASM
extern void func_004071F8(int, int);
extern int func_004072E8(int, int);
extern void func_00407A10(int);
int func_004074B8(int *p) {
    p[109] = func_004072E8((int)p, 5);
    if (func_004072E8((int)p, 1)) {
        func_004072E8((int)p, 1);
        func_004071F8((int)p, 7);
        func_00407A10((int)p);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407520);
#ifdef SKIP_ASM
struct B7520 { int op; long l1; long l2; };
struct S_7520 { char pad[0x150]; int f150; char pad2[0x6D4]; long f828; long f830; char pad3[0x20]; int f858; };
extern void s7_7438(struct S_7520 *) __asm__("func_00407438");
extern int s7_72E8(struct S_7520 *, int) __asm__("func_004072E8");
extern void s7_3620(struct S_7520 *) __asm__("func_00403620");
extern void s7_7AD0(struct S_7520 *) __asm__("func_00407AD0");
extern void s7_7630(struct S_7520 *) __asm__("func_00407630");
extern void s7_2C30(int, struct B7520 *) __asm__("func_00402C30");
int func_00407520(struct S_7520 *a) {
    struct B7520 b;
    for (;;) {
        s7_7438(a);
        switch ((unsigned)s7_72E8(a, 0x20)) {
        case 0x1B3:
            s7_3620(a);
            break;
        case 0x1B8:
            s7_7AD0(a);
            break;
        case 0x1B7:
            return 0;
        case 0x100:
            s7_7630(a);
            b.op = 5;
            b.l1 = -1;
            b.l2 = -1;
            s7_2C30(a->f858, &b);
            a->f830 = b.l2;
            a->f828 = b.l1;
            return a->f150;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407630);
#ifdef SKIP_ASM
extern int func_004072E8(int, int);
extern void func_00407A10(int);
extern void func_004076F8(int);
extern void func_00407A58(int, int);
void func_00407630(int *p) {
    int v = func_004072E8((int)p, 10);
    p[0x150 / 4] = func_004072E8((int)p, 3);
    func_004072E8((int)p, 16);
    if ((unsigned)(p[0x150 / 4] - 2) < 2) {
        p[0x154 / 4] = func_004072E8((int)p, 1);
        p[0x158 / 4] = func_004072E8((int)p, 3);
    }
    if (p[0x150 / 4] == 3) {
        p[0x15C / 4] = func_004072E8((int)p, 1);
        p[0x160 / 4] = func_004072E8((int)p, 3);
    }
    func_00407A10((int)p);
    func_004076F8((int)p);
    func_00407A58((int)p, v);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004076F8);
#ifdef SKIP_ASM
struct PT44 { int v[11]; };
extern char D_00497FE0[];
extern int func_004070F0(int, int);
extern void func_004071F8(int, int);
extern int func_004072E8();
void func_004076F8(int p) {
    struct PT44 t = *(struct PT44 *)D_00497FE0;
    unsigned i;
    int r;
    func_00407438(p);
    while ((r = func_004070F0(p, 32)) == 0x1B5 || r == 0x1B2) {
        if (r == 0x1B5) {
            func_004071F8(p, 32);
            i = func_004072E8(p, 4);
            i = i > 10 ? 0 : i;
            ((void (*)(int))t.v[i])(p);
            func_00407438(p);
        } else {
            func_004071F8(p, 32);
            func_00407438(p);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407820);
#ifdef SKIP_ASM
extern int Q_72E8(int, int) __asm__("func_004072E8");
void func_00407820(int *p) {
    p[0x164 / 4] = Q_72E8((int)p, 4);
    p[0x168 / 4] = Q_72E8((int)p, 4);
    p[0x16C / 4] = Q_72E8((int)p, 4);
    p[0x170 / 4] = Q_72E8((int)p, 4);
    {
        int v = Q_72E8((int)p, 2);
        *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFFCFFFF) | (v << 16);
    }
    {
        int v = Q_72E8((int)p, 2);
        p[0x174 / 4] = v;
        if (p[0xD4 / 4] == 0)
            p[0xD4 / 4] = v;
    }
    p[0x178 / 4] = Q_72E8((int)p, 1);
    p[0x17C / 4] = Q_72E8((int)p, 1);
    p[0x180 / 4] = Q_72E8((int)p, 1);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFBFFFFF) | (Q_72E8((int)p, 1) << 22);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFDFFFFF) | (Q_72E8((int)p, 1) << 21);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFEFFFFF) | (Q_72E8((int)p, 1) << 20);
    p[0x184 / 4] = Q_72E8((int)p, 1);
    Q_72E8((int)p, 1);
    p[0x188 / 4] = Q_72E8((int)p, 1);
    if (Q_72E8((int)p, 1)) {
        Q_72E8((int)p, 1);
        Q_72E8((int)p, 3);
        Q_72E8((int)p, 1);
        Q_72E8((int)p, 7);
        Q_72E8((int)p, 8);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407A10);
#ifdef SKIP_ASM
extern void func_004071F8(int, int);
extern int func_004072E8(int, int);
void func_00407A10(int p) {
    while (func_004072E8(p, 1))
        func_004071F8(p, 8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407A58);
#ifdef SKIP_ASM
struct S407A58 { char p0[0x150]; int f150; char p1[0x58]; int f1AC; char p2[0x69C]; int f84C; int f850; int f854; };
int f_407A58(struct S407A58 *p, int n) __asm__("func_00407A58");
int f_407A58(struct S407A58 *p, int n) {
    int t = 0;
    int m = 0;
    int x;
    if (p->f150 != 3) {
        if (n != 0) {
            if (n < 0)
                t = p->f854 == 0;
            p->f854 = 0;
            m = n;
        }
    }
    p->f1AC = p->f84C + n;
    if (t && m >= n)
        p->f1AC += 0x400;
    x = p->f850;
    if (x < p->f1AC)
        x = p->f1AC;
    p->f850 = x;
    return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407AD0);
#ifdef SKIP_ASM
extern int func_004072E8(int, int);
extern void func_004076F8();
void func_00407AD0(int *p) {
    p[0xE8 / 4] = 0;
    p[0x84C / 4] = p[0x850 / 4] + 1;
    p[0x854 / 4] = 1;
    func_004072E8((int)p, 1);
    func_004072E8((int)p, 5);
    func_004072E8((int)p, 6);
    func_004072E8((int)p, 1);
    func_004072E8((int)p, 6);
    func_004072E8((int)p, 6);
    p[0x1A4 / 4] = func_004072E8((int)p, 1);
    p[0x1A8 / 4] = func_004072E8((int)p, 1);
    func_004076F8(p);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407B70);
#ifdef SKIP_ASM
extern int func_004072E8(int, int);
extern void func_00406E20(int);
extern void func_004034A8(int *, int);
extern char D_00498010[];
extern char D_00498038[];
void func_00407B70(int *p) {
    if ((p[0x840 / 4] = func_004072E8((int)p, 1)) != 0) {
        func_00406E20((int)p);
        func_00406DF0(p, 0x50000000);
        func_00406E20((int)p);
    }
    if ((p[0x844 / 4] = func_004072E8((int)p, 1)) != 0) {
        func_00406E20((int)p);
        func_00406DF0(p, 0x58000000);
        func_00406E20((int)p);
    }
    if (func_004072E8((int)p, 1) != 0)
        func_004034A8(p, (int)D_00498010);
    if (func_004072E8((int)p, 1) != 0)
        func_004034A8(p, (int)D_00498038);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00407C38);
#ifdef SKIP_ASM
struct S_7C38 { char pad[0x13C]; int f13C; char pad2[0x34]; int f174; int f178; char pad3[8]; int f184; int pad188; int f18C[3]; int f198[3]; };
extern int sc_72E8(struct S_7C38 *, int) __asm__("func_004072E8");
void func_00407C38(struct S_7C38 *a); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407D30);
#ifdef SKIP_ASM
extern int func_004072E8(int, int);
void func_00407D30(int p) {
    func_004072E8(p, 1);
    func_004072E8(p, 8);
    func_004072E8(p, 1);
    func_004072E8(p, 7);
    func_004072E8(p, 1);
    func_004072E8(p, 0x14);
    func_004072E8(p, 1);
    func_004072E8(p, 0x16);
    func_004072E8(p, 1);
    func_004072E8(p, 0x16);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407DC0);
#ifdef SKIP_ASM
extern char D_00498068[];
extern char D_00498088[];
extern void func_004034A8();
extern int func_004060C8(int *);
int func_00407DC0(int *p) {
    int *q;
    int r;
    if (p[0x174 / 4] == 3 && p[0x120 / 4] != 0) {
        func_004034A8(p, D_00498068);
        p[0x120 / 4] = 0;
    }
    switch (p[0x174 / 4]) {
    case 3:
        q = (int *)p[0x1C0 / 4];
        break;
    case 1:
        q = (int *)p[0x1D0 / 4];
        break;
    case 2:
        q = (int *)p[0x1E0 / 4];
        break;
    default:
        q = (int *)p[0x1C0 / 4];
        func_004034A8(p, D_00498088);
        break;
    }
    r = func_004060C8(p);
    if (r != 0)
        q[0x28 / 4] = 1;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407E90);
#ifdef SKIP_ASM
extern void func_004086C0(int *, int, int);
extern void func_004087D0(int *, int, int, int);
void func_00407E90(int *p, int n, int flag) {
    int a, b;
    if (flag) {
        if (p[0x174 / 4] == 3) {
            if (p[0x150 / 4] == 3)
                a = p[0x1C4 / 4];
            else
                a = p[0x1B8 / 4];
            func_004086C0(p, a, n - 1);
        } else {
            if (p[0x150 / 4] == 3) {
                a = p[0x1D4 / 4];
                b = p[0x1E4 / 4];
            } else {
                a = p[0x1C8 / 4];
                b = p[0x1D8 / 4];
            }
            func_004087D0(p, a, b, n - 1);
        }
    }
    if (p[0xF8 / 4] == 1)
        p[0xF8 / 4] = 2;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00407F20);
#ifdef SKIP_ASM
struct F7F20 {
    char pad0[0x18];
    long t18;
    long t20;
    int state;
    int f2C, f30, f34, f38, f3C, f40, f44, f48, f4C, f50, f54, f58, f5C, f60;
};
struct C7F20 { struct F7F20 *ref0; struct F7F20 *ref1; struct F7F20 *cur; struct F7F20 *next; };
struct M7F20 {
    char pad0[0xA0];
    int fA0, fA4;
    char padA8[0xE8 - 0xA8];
    int fE8;
    char padEC[0x13C - 0xEC];
    int f13C;
    int pad140[2];
    int f148, f14C, f150;
    int pad154[8];
    int f174, f178;
    int pad17C[2];
    int f184, f188, f18C, f190, f194, f198, f19C, f1A0, f1A4, f1A8;
    int pad1AC[3];
    struct C7F20 c[3];
    char pad1E8[0x828 - 0x1E8];
    long t828;
    long t830;
};
int func_00407F20(struct M7F20 *m, int a1); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004081D8);
#ifdef SKIP_ASM
extern int sprintf(char *, const char *, ...);
extern void func_004034A8(int *, int);
extern char D_004980A8[];
int func_004081D8(int *p, int *q) {
    char buf[256];
    int r;
    int n = p[0xE0 / 4];
    if (n != 0)
        r = p[0xDC / 4] >= q[1] && n >= q[2];
    else
        r = p[0xE4 / 4] >= q[3] * q[4];
    if (r == 0) {
        sprintf(buf, D_004980A8, q[1], q[2]);
        func_004034A8(p, (int)buf);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408278);
#ifdef SKIP_ASM
// Scratchpad DMA copy: per pass, move p[3] blocks of p[4]*0x180 bytes to the scratchpad (ch 9) and back out (ch 8).
struct S8278 { char pad0[0xD8]; unsigned fD8; int padDC; int fE0; int fE4; char padE8[0x174 - 0xE8]; int f174; };
extern int func_0042C078();
extern void func_0042C0C0(void);
void func_00408278(struct S8278 *s, unsigned *p) {
    int n;
    unsigned src = p[0] & 0x0FFFFFFF;
    unsigned dst = s->fD8 & 0x0FFFFFFF;
    int size, stride, qwc;
    int i, j, st;
    unsigned d;
    if (s->f174 == 3 || s->fE0 == 0) {
        size = p[4] * 0x180;
        qwc = size >> 4;
        if (s->fE0 != 0)
            stride = (s->fE0 >> 4) * 0x180;
        else
            stride = size;
        n = 1;
    } else {
        stride = (s->fE0 >> 4) * 0xC0;
        size = ((int)p[4] >> 1) * 0x180;
        n = 2;
        qwc = size >> 4;
    }
    for (j = 0; j < n; j++) {
        d = dst;
        for (i = 0; i < (int)p[3]; i++) {
            st = func_0042C078();
            *(volatile int *)0x1000D480 = 0;
            *(volatile int *)0x1000D410 = src;
            *(volatile int *)0x1000D420 = qwc;
            *(volatile int *)0x1000D400 = 0x101;
            if (st)
                func_0042C0C0();
            while (*(volatile int *)0x1000D400 & 0x100)
                ;
            st = func_0042C078();
            *(volatile int *)0x1000D080 = 0;
            *(volatile int *)0x1000D010 = d;
            *(volatile int *)0x1000D020 = qwc;
            *(volatile int *)0x1000D000 = 0x100;
            if (st)
                func_0042C0C0();
            while (*(volatile int *)0x1000D000 & 0x100)
                ;
            while (*(volatile int *)0x1000D020 != 0)
                ;
            d += stride;
            src += size;
        }
        dst += s->fE4 * 0xC0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408508);
#ifdef SKIP_ASM
void func_00408508(int *p) {
    if (p[2] != 2) {
        p[2] = 2;
        p[43] = p[70];
    }
    p[520] = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408530);
#ifdef SKIP_ASM
// func_00408530 97.98 (sweep: schedule). IPU/MPEG picture-timestamp routine: a=decoder state, b=picture header, o1/o2/o3 -> r->f10/f18/f20 in caller func_004086C0. func_004114D0=__muldi3 (long*long). Only diff: 'move $4,$18' (4th muldi3 arg) placed before dsra32 in ours, after it in target. Tried named locals, bitfield union, w/z temps, sweep 716 variants, permuter 4min. Chain order f34|f38|f3c|f40|f30|f2c found by permutation search.
struct S85 { char pad0[0x70]; int f70; int pad74; long f78; int f80; int pad84; long f88; int f90; char pad94[0x5C]; long fF0; int fF8; };
struct B85 { char pad0[0x18]; long f18; long f20; int pad28; int f2c; int f30; int f34; int f38; int f3c; int f40; };
extern long mul85(long, long) __asm__("func_004114D0");
long func_00408530(struct S85 *a, struct B85 *b, long *o1, long *o2, long *o3) {
    long r;
    long v;
    int x;
    int e;
    int s;
    if (a->f70 != 0) {
        v = b->f18;
        if (v >= 0) {
            *o1 = v;
        } else {
            s = a->f80;
            if (s < 0) {
                *o1 = v;
            } else {
                long t;
                int y;
                x = a->f88;

                t = mul85(x & 1, a->f78 & 1);
                e = a->f90;
                y = mul85(t, e & 1);
                *o1 = s + ((int)(mul85(a->f78, x) >> 1) + y);
                if (mul85(x & 1, a->f78 & 1))
                    a->f90 = e + 1;
            }
        }
    } else {
        *o1 = b->f18;
    }
    if (a->fF8 == 2 && a->fF0 >= 0) {
        *o1 = a->fF0;
        a->fF8 = 0;
        a->fF0 = -1;
    }
    *o2 = b->f20;
    r = (long)b->f34 << 8 | (long)b->f38 << 7 | (long)b->f3c << 6 | (long)b->f40 << 5 | (long)b->f30 << 3 | (long)b->f2c;
    *o3 = r;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004086C0);
#ifdef SKIP_ASM
struct S86C0 { char pad0[0x80]; int f80; int pad84; unsigned long f88; char pad90[0x20]; int fB0; int fB4; int fB8; int fBC; int fC0; int fC4; int fC8; int fCC; int fD0; char padD4[0x784]; int f858; };
struct B86C0 { char pad0[0x28]; int f28; char pad2C[0x18]; int f44; int f48; int f4C; int f50; int f54; int f58; int f5C; int f60; };
struct R86C0 { char pad[0x10]; int f10; int f14; int f18; int f1C; long f20; };
extern unsigned D_004532E8[];
extern void q86_8530(struct S86C0 *, struct B86C0 *, void *, void *, void *) __asm__("func_00408530");
extern int q86_81D8(struct S86C0 *, struct B86C0 *) __asm__("func_004081D8");
extern int q86_9088(struct S86C0 *, struct B86C0 *) __asm__("func_00409088");
extern int q86_8278(struct S86C0 *, struct B86C0 *) __asm__("func_00408278");
extern void q86_8508(struct S86C0 *) __asm__("func_00408508");
void d86C0(struct S86C0 *a, struct B86C0 *b, int c) __asm__("func_004086C0");
void d86C0(struct S86C0 *a, struct B86C0 *b, int c) {
    struct R86C0 *r = (struct R86C0 *)a->f858;
    int m;
    q86_8530(a, b, &r->f10, &r->f18, &r->f20);
    r = (struct R86C0 *)a->f858;
    a->f80 = r->f10;
    m = r->f20 >> 5;
    a->f88 = D_004532E8[m & 0xF];
    a->fCC = b->f5C;
    a->fD0 = b->f60;
    a->fB4 = b->f44;
    a->fB8 = b->f48;
    a->fBC = b->f4C;
    a->fC0 = b->f50;
    a->fC4 = b->f54;
    a->fC8 = b->f58;
    if (q86_81D8(a, b) && b->f28 == 1) {
        if (a->fB0 != 0) q86_9088(a, b);
        else q86_8278(a, b);
        q86_8508(a);
    }
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_004087D0);

INCLUDE_ASM("seg/seg_2FE810", func_00408988);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408AF8);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
void func_00408AF8(unsigned a, int b) {
    int s = func_0042C078();
    *(volatile unsigned *)0x1000B010 = (a & 0xFFFFFFF) | 0x80000000;
    *(volatile int *)0x1000B020 = b >> 4;
    *(volatile int *)0x1000B000 = 0x100;
    if (s)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408B80);
#ifdef SKIP_ASM
extern int q8B80_C078() __asm__("func_0042C078");
extern void q8B80_C0C0(void) __asm__("func_0042C0C0");
extern void q8B80_6DF0(int *, int) __asm__("func_00406DF0");
extern int q8B80_2C30(int *, int *) __asm__("func_00402C30");
void q8B80(int *p, unsigned addr, int n) __asm__("func_00408B80");
void q8B80(int *p, unsigned addr, int n) {
    int s;
    int sp[8];
    while (*(volatile int *)0x10002010 < 0)
        ;
    s = q8B80_C078();
    *(volatile int *)0x1000B010 = addr & 0x0FFFFFFF;
    *(volatile int *)0x1000B020 = n << 6;
    *(volatile int *)0x1000B000 = 0x100;
    if (s)
        q8B80_C0C0();
    q8B80_6DF0(p, n | 0x70000000);
    sp[0] = 4;
    q8B80_2C30(*(int **)((char *)p + 0x858), sp);
    while (((*(volatile unsigned *)0x1000B000 >> 8) & 1))
        ;
    while (*(volatile int *)0x10002010 < 0)
        ;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00408CA8);

INCLUDE_ASM("seg/seg_2FE810", func_00408DF0);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408FB0);
#ifdef SKIP_ASM
struct S408FB0 { unsigned f0; int f4; };
int func_00408FB0(int a, struct S408FB0 *s) {
    unsigned n;
    *(volatile int *)0x1000E010 = 0x10;
    n = s->f0;
    if (n == 0) {
        SYNC_0810();
        EI_0810();
        return 1;
    }
    if (n > 0xFFFF) {
        *(volatile int *)0x1000B410 = s->f4;
        *(volatile int *)0x1000B420 = 0xFFFF;
        *(volatile int *)0x1000B400 = 0x101;
        s->f4 = (s->f4 + 0xFFFF0) & 0xFFFFFFF;
        s->f0 = s->f0 - 0xFFFF;
    } else {
        *(volatile int *)0x1000B410 = s->f4;
        *(volatile int *)0x1000B420 = s->f0;
        *(volatile int *)0x1000B400 = 0x101;
        s->f0 = 0;
    }
    SYNC_0810();
    EI_0810();
    return 0;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00409088);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409310);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
void func_00409310(int a) {
    int s = func_0042C078();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 = a;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (s)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409388);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
void func_00409388(int a) {
    int s = func_0042C078();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = a;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (s)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409400);
#ifdef SKIP_ASM
extern void q9400_9388(int) __asm__("func_00409388");
extern void q9400_9310(int) __asm__("func_00409310");
void q9400(int *p) __asm__("func_00409400");
void q9400(int *p) {
    q9400_9388(1);
    p[0] = *(volatile int *)0x1000B410;
    p[1] = *(volatile int *)0x1000B430;
    p[2] = *(volatile int *)0x1000B420;
    p[3] = *(volatile int *)0x1000B400;
    while (*(volatile int *)0x10002010 & 0xF0)
        ;
    q9400_9310(0);
    p[4] = *(volatile int *)0x1000B010;
    p[5] = *(volatile int *)0x1000B020;
    p[6] = *(volatile int *)0x1000B000;
    p[7] = *(volatile int *)0x10002020;
    p[8] = *(volatile int *)0x10002010;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004094E8);
#ifdef SKIP_ASM
extern void q94E8_9388(int) __asm__("func_00409388");
extern void q94E8_9310(int) __asm__("func_00409310");
void q94E8(int *p) __asm__("func_004094E8");
void q94E8(int *p) {
    unsigned v = p[7];
    int e = v & 0x7F;
    int n = ((v >> 16) & 3) + ((v >> 8) & 0xF);
    int r = p[0] - (n << 4);
    int q = p[2] + n;
    if (p[4] != 0 && p[5] != 0) {
        *(volatile int *)0x1000B010 = p[4];
        *(volatile int *)0x1000B020 = p[5];
        q94E8_9310(p[6] | 0x100);
    }
    while (*(volatile int *)0x10002010 < 0)
        ;
    *(volatile int *)0x10002000 = e;
    while (*(volatile int *)0x10002010 < 0)
        ;
    if (r != 0 && q != 0) {
        *(volatile int *)0x1000B410 = r;
        *(volatile int *)0x1000B430 = p[1];
        *(volatile int *)0x1000B420 = q;
        q94E8_9388(p[3] | 0x100);
    }
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00409638);
#ifdef SKIP_ASM
int func_00409638(int a); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004096A0);
#ifdef SKIP_ASM
extern int func_0042C078(int);
extern void func_0042C0C0(void);
void func_004096A0(int a) {
    func_0042C078(a);
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = a;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    func_0042C0C0();
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00409708);
#ifdef SKIP_ASM
typedef int i128_9708 __attribute__((mode(TI)));
extern volatile i128_9708 D_00453340[];
extern i128_9708 D_00453390[];
extern void func_004096A0(int);
#define WAIT_9708() while (*(volatile int *)0x10002010 < 0)
void func_00409708(void); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_00409940);
#ifdef SKIP_ASM
extern char D_004980F0[];
extern char D_00498108[];
extern char D_00498130[];
extern int Q_sema(int *) __asm__("func_00423DA0");
extern void Q_sig(int) __asm__("func_00423DC0");
extern void Q_log(char *) __asm__("func_00425C60");
extern int Q_A360(int, int, int) __asm__("func_0040A360");
extern void Q_6408(int) __asm__("func_00426408");
int func_00409940(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409B10);
#ifdef SKIP_ASM
extern int func_00423DB0(int);
int func_00409B10(void) {
    if (D_004533C4 >= 0) {
        func_00423DB0(D_004533C4);
        D_004533C4 = -1;
        func_00423DB0(D_004533C8);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409B60);
#ifdef SKIP_ASM
sSifClient *func_00409B60(int *a, int *b) {
    *a = (int)D_00528780;
    *b = (int)&D_004533C0;
    D_00528780[15] = D_004533C4;
    return &D_005271C0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409B90);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
int func_00409B90(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.origin = a;
    r = func_00426D18(&D_005271C0, 20, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 20;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409C48);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
int func_00409C48(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    r = func_00426D18(&D_005271C0, 21, 0, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r != 0) {
        func_00423DC0(D_004533C4);
        return r;
    }
    func_00423DC0(D_004533C4);
    return D_00528780[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409D10);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern char *strncpy(char *, const char *, int);
int func_00409D10(int a, int b, int s_, int c) {
    char *s = (char *)s_;
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        func_00423DC0(D_004533C4);
        return -210;
    }
    strncpy(D_00527270.name, s, 0x3FF);
    D_00527270.port = a;
    D_00527270.mode = c;
    D_00527270.slot = b;
    D_00527270.name[0x3FF] = 0;
    r = func_00426D18(&D_005271C0, 2, 1, &D_00527270, 0x414, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 2;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409E38);
#ifdef SKIP_ASM
extern int func_00409D10(int, int, int, int);
int func_00409E38(int a, int b, int c) {
    int r = func_00409D10(a, b, c, 0x40);
    if (r == 0) {
        D_004533C0 = 0xB;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409E70);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
int func_00409E70(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    r = func_00426D18(&D_005271C0, 3, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 3;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409F28);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
int func_00409F28(int a, int b, int c) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    D_00527240.offset = b;
    D_00527240.origin = c;
    r = func_00426D18(&D_005271C0, 4, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 4;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A000);
#ifdef SKIP_ASM
struct S0A000 { int n0; int n1; char *d0; char *d1; char b0[0x40]; char b1[0x40]; };
void func_0040A000(void *a) {
    struct S0A000 *s = (struct S0A000 *)((unsigned)a | 0x20000000);
    char *d;
    int i;
    if (s->n0 != 0) {
        d = s->d0;
        for (i = 0; i < s->n0; i++)
            *d++ = s->b0[i];
    }
    if (s->n1 != 0) {
        d = s->d1;
        for (i = 0; i < s->n1; i++)
            *d++ = s->b1[i];
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A090);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern void func_00426358(int, int);
int func_0040A090(int a, int b, int c) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    D_00527240.param = (int)D_005276C0;
    D_00527240.buffer = b;
    D_00527240.size = c;
    func_00426358(b, c);
    func_00426358((int)D_005276C0, 0xC0);
    r = func_00426D18(&D_005271C0, 5, 1, &D_00527240, 48, D_00528780, 4, func_0040A000, D_005276C0);
    if (r == 0)
        D_004533C0 = 5;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A1A8);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern void func_00424020(int);
int func_0040A1A8(int a, unsigned char *p, int n) {
    int r;
    unsigned i;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    if (n < 17) {
        D_00527240.origin = n;
        D_00527240.buffer = D_00527240.size = 0;
    } else {
        int q = (int)p - 16;
        int d = (((unsigned)p - 1) & 0xFFFFFFF0u) - q;
        D_00527240.size = n - d;
        D_00527240.origin = d;
        D_00527240.buffer = (int)p + d;
    }
    for (i = 0; i < D_00527240.origin; i++)
        D_00527240.data[i] = p[i];
    func_00424020(0);
    r = func_00426D18(&D_005271C0, 6, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 6;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A320);
#ifdef SKIP_ASM
extern void func_00423DD0(int);
void func_0040A320(int a, int b, int c) {
    func_00423DD0(c);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A328);
#ifdef SKIP_ASM
extern void func_0040A320(int, int, int);
extern void func_00423B20(int, void (*)(int, int, int), int);
extern void func_00423DE0(int);
void func_0040A328(unsigned short a) {
    func_00423B20(a, func_0040A320, D_004533C8);
    func_00423DE0(D_004533C8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A360);
#ifdef SKIP_ASM
extern void func_00423DC0(int);
extern void sleep_A328(int) __asm__("func_0040A328");
int func_0040A360(int a, int *p, int *q) {
    int r;
    if (D_004533C0 == 0)
        return -1;
    r = func_00426F08(&D_005271C0);
    if (a == 0 && r != 0) {
        while (func_00426F08(&D_005271C0) != 0)
            sleep_A328(60);
        r = 0;
    }
    r = r == 0;
    if (p)
        *p = D_004533C0;
    if (r) {
        D_004533C0 = 0;
        if (q)
            *q = D_00528780[0];
        func_00423DC0(D_004533C4);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A440);
#ifdef SKIP_ASM
void func_0040A440(void *a) {
    int *p = (int *)((unsigned)a | 0x20000000);
    if (D_005271E8)
        *D_005271E8 = p[0];
    if (D_005271EC)
        *D_005271EC = p[1];
    if (D_005271F0)
        *D_005271F0 = p[36];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A498);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern void func_00426358(int, int);
int func_0040A498(int a, int b, int *c, int *d, int *e) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    D_00527240.slot = b;
    D_00527240.param = (int)D_005276C0;
    if (c != 0)
        D_00527240.origin = 1;
    else
        D_00527240.origin = 0;
    if (d != 0)
        D_00527240.offset = 1;
    else
        D_00527240.offset = 0;
    if (e != 0)
        D_00527240.size = 1;
    else
        D_00527240.size = 0;
    D_005271E8 = c;
    D_005271EC = d;
    D_005271F0 = e;
    func_00426358((int)D_005276C0, 0xC0);
    r = func_00426D18(&D_005271C0, 1, 1, &D_00527240, 48, D_00528780, 4, func_0040A440, D_005276C0);
    if (r == 0)
        D_004533C0 = 1;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A618);
#ifdef SKIP_ASM
extern void func_0040A618_log(char *) __asm__("func_00425C60");
extern char D_00498158[];
int func_0040A618(void) {
    if (func_00426D18(&D_005271C0, 53, 0, &D_00527240, 48, D_00528780, 4, 0, 0)) {
        func_0040A618_log(D_00498158);
        return -1;
    }
    return D_00528780[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A688);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern char *strncpy(char *, const char *, int);
extern void func_00426358(int, int);
int func_0040A688(int a, int b, char *s, int c, int d, int e) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        func_00423DC0(D_004533C4);
        return -210;
    }
    D_00527270.port = a;
    D_00527270.slot = b;
    D_00527270.mode = c;
    D_00527270.count = d;
    D_00527270.buffer = e;
    strncpy(D_00527270.name, s, 0x3FF);
    D_00527270.name[0x3FF] = 0;
    if (d >= 0)
        func_00426358(e, d << 6);
    r = func_00426D18(&D_005271C0, 13, 1, &D_00527270, 0x414, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 13;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A7D0);
#ifdef SKIP_ASM
extern unsigned strlen(char *);
extern int func_0041605C(int, int, int);
void func_0040A7D0(void *dst) {
    char *src;
    int n;
    if (dst) {
        src = (char *)((unsigned)D_00527780 | 0x20000000);
        if (strlen(src) < 0x400)
            n = strlen(src);
        else
            n = 0x3FF;
        func_0041605C((int)dst, (unsigned)D_00527780 | 0x20000000, n);
        ((char *)dst)[n] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A858);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern char *strncpy(char *, const char *, int);
extern void func_00426358(int, int);
int func_0040A858(int a, int b, char *s, void *u) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        func_00423DC0(D_004533C4);
        return -210;
    }
    D_00527270.port = a;
    D_00527270.buffer = (int)D_00527780;
    D_00527270.slot = b;
    strncpy(D_00527270.name, s, 0x3FF);
    D_00527270.name[0x3FF] = 0;
    func_00426358((int)D_00527780, 0x400);
    r = func_00426D18(&D_005271C0, 12, 1, &D_00527270, 0x414, D_00528780, 4, func_0040A7D0, u);
    if (r == 0)
        D_004533C0 = 12;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A998);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
int func_0040A998(int a, int b) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    D_00527240.slot = b;
    r = func_00426D18(&D_005271C0, 16, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 16;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AA68);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern char *strncpy(char *, const char *, int);
int func_0040AA68(int a, int b, char *s) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        func_00423DC0(D_004533C4);
        return -210;
    }
    strncpy(D_00527270.name, s, 0x3FF);
    D_00527270.port = a;
    D_00527270.slot = b;
    D_00527270.name[0x3FF] = 0;
    D_00527270.mode = 0;
    r = func_00426D18(&D_005271C0, 15, 1, &D_00527270, 0x414, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 15;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AB80);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
int func_0040AB80(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    r = func_00426D18(&D_005271C0, 10, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 10;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AC38);
#ifdef SKIP_ASM
extern int R_3DF0(int) __asm__("func_00423DF0");
extern void R_3DC0(int) __asm__("func_00423DC0");
extern void R_4020(int) __asm__("func_00424020");
extern char *strncpy(char *, const char *, int);
int func_0040AC38(int a, int b, char *s, sMcDirEntry *c, int e) {
    int r;
    sSifClient *h = &D_005271C0;
    if (h->server == 0)
        return -100;
    if (R_3DF0(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        R_3DC0(D_004533C4);
        return -210;
    }
    e &= 7;
    D_00527270.port = a;
    D_00527270.slot = b;
    D_00527270.mode = e;
    D_00527200 = *c;
    D_00527270.buffer = (int)&D_00527200;
    strncpy(D_00527270.name, s, 0x3FF);
    D_00527270.name[0x3FF] = 0;
    R_4020(0);
    r = func_00426D18(h, 14, 1, &D_00527270, 0x414, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 14;
    else
        R_3DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AE00);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern char *strncpy(char *, const char *, int);
extern void func_00424020(int);
int func_0040AE00(int a, int b, char *s, char *t) {
    int r;
    char *p;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0 || t == 0) {
        func_00423DC0(D_004533C4);
        return -210;
    }
    D_00527270.port = a;
    D_00527270.slot = b;
    D_00527270.mode = 16;
    strncpy(D_00527270.name, s, 0x3FF);
    D_00527270.name[0x3FF] = 0;
    strncpy(D_00527220, t, 0x20);
    p = D_00527220 - 0x20;
    p[0x3F] = 0;
    D_00527270.buffer = (int)p;
    func_00424020(0);
    r = func_00426D18(&D_005271C0, 14, 1, &D_00527270, 0x414, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 19;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AF58);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
int func_0040AF58(int a, int b) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    D_00527240.slot = b;
    r = func_00426D18(&D_005271C0, 17, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 17;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B028);
#ifdef SKIP_ASM
extern int func_00423DF0(int);
extern void func_00423DC0(int);
extern char *strncpy(char *, const char *, int);
int func_0040B028(int a, int b, char *s) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (func_00423DF0(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        func_00423DC0(D_004533C4);
        return -210;
    }
    D_00527270.port = a;
    D_00527270.slot = b;
    strncpy(D_00527270.name, s, 0x3FF);
    D_00527270.name[0x3FF] = 0;
    r = func_00426D18(&D_005271C0, 18, 1, &D_00527270, 0x414, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 18;
    else
        func_00423DC0(D_004533C4);
    return r;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040B130);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B1F8);
#ifdef SKIP_ASM
extern int D_004533E0[];
void func_0040B1F8(void) {
    D_004533E0[0] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B208);
#ifdef SKIP_ASM
struct E0B208 { char pad[0x10]; int f10; int f14; int f18; char pad2[0x24]; };
struct S0B208 { int f0; struct E0B208 *tab; int n; };
extern int func_0042C078_rq(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
struct E0B208 *f0B208(struct S0B208 *s) __asm__("func_0040B208");
struct E0B208 *f0B208(struct S0B208 *s) {
    struct E0B208 *e;
    int i;
    int n;
    int c;
    int id;
    int seq;
    func_0042C078_rq();
    n = s->n;
    i = 0;
    e = s->tab;
    while (i < n) {
        if (!(e->f10 & 1)) {
            e->f10 = (i << 16) | 5;
            c = s->f0;
            id = c + 1;
            s->f0 = id;
            if (id == 1) {
                s->f0 = c + 2;
                seq = 1;
            } else {
                seq = id;
            }
            id = seq;
            e->f14 = (int)e;
            e->f18 = id;
            func_0042C0C0();
            return e;
        }
        i++;
        e++;
    }
    func_0042C0C0();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B2B0);
#ifdef SKIP_ASM
void func_0040B2B0(unsigned *p) {
    p[6] = 0;
    p[4] &= 0xFFFFFFFE;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040B2D0);
#ifdef SKIP_ASM
struct C_B2D0 { unsigned *f0; int pad4; int f8; int pad[2]; int f14; int f18; void (*f1C)(int); int f20; int f24; int f28; int f2C; };
struct E_B2D0 { char pad[0x1C]; struct C_B2D0 *ctx; unsigned code; int f24; int f28; int f2C; };
extern void func_00423DD0(int);
extern void func_0040B2B0(unsigned *);
void func_0040B2D0(struct E_B2D0 *e); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B3D8);
#ifdef SKIP_ASM
extern int func_0040B400(int, int, int, int, int, int);
int func_0040B3D8(int a, int b, int c) {
    return func_0040B400(a, b, c, 0x800, 0x2000, 0x20);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B400);
#ifdef SKIP_ASM
struct N_B400 { char pad0[0x14]; struct N_B400 *f14; int f18; struct C_B400 *f1C; int f20; int f24; int f28; int f2C; };
struct C_B400 { struct N_B400 *f0; int f4; int f8; int padC; int f10; char pad14[0x10]; int f24; int f28; int pad2C; int f30; int f34; int f38; };
extern int func_00423DA0(int *);
extern int fr_B400(int) __asm__("func_00423DB0");
extern void func_00423DE0(int);
extern void lg_B400(char *) __asm__("func_00425C60");
extern struct N_B400 *func_0040B208(char *);
extern void rl_B400(struct N_B400 *) __asm__("func_0040B2B0");
extern int func_004261B0(unsigned, struct N_B400 *, int, int, int, int);
extern char D_00498178[];
int func_0040B400(int cc, int a, int flag, int d, int e, int f) {
    struct C_B400 *c = (struct C_B400 *)cc;
    int s[6];
    int h;
    struct N_B400 *n;
    if (flag != 0) {
        lg_B400(D_00498178);
        return -1;
    }
    s[5] = 0;
    s[2] = 1;
    s[1] = 1;
    c->f28 = func_00423DA0(s);
    if (c->f28 < 0)
        return -3;
    func_00423DE0(c->f28);
    c->f10 = a;
    c->f24 = 0;
    c->f30 = d;
    c->f34 = e;
    c->f38 = f;
    n = func_0040B208(D_00528FC0);
    if (n == 0) {
        fr_B400(c->f28);
        return -1;
    }
    c->f0 = n;
    c->f4 = n->f18;
    n->f20 = a;
    n->f24 = d;
    n->f28 = e;
    n->f2C = f;
    n->f14 = n;
    n->f1C = c;
    s[1] = 1;
    s[2] = 0;
    s[5] = 0;
    h = func_00423DA0(s);
    c->f8 = h;
    if (h < 0) {
        rl_B400(n);
        fr_B400(c->f28);
        return -3;
    }
    if (func_004261B0(0x80000019, n, 0x40, 0, 0, 0) == 0) {
        rl_B400(n);
        fr_B400(h);
        fr_B400(c->f28);
        return -2;
    }
    func_00423DE0(h);
    fr_B400(h);
    if (c->f24 == 0)
        fr_B400(c->f28);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B5B0);
#ifdef SKIP_ASM
struct O_B5B0 { int f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; int f28; int f2C; };
extern char D_00498178[];
extern void b5_00425C60(char *, ...) __asm__("func_00425C60");
extern int b5_00423DE0(int) __asm__("func_00423DE0");
extern int b5_00423DB0(int) __asm__("func_00423DB0");
extern struct O_B5B0 *b5_0040B208(char *) __asm__("func_0040B208");
extern void b5_0040B2B0(struct O_B5B0 *) __asm__("func_0040B2B0");
extern int b5_00423DA0(struct QSema *) __asm__("func_00423DA0");
extern int b5_004261B0(int, struct O_B5B0 *, int, int, int, int) __asm__("func_004261B0");
int func_0040B5B0(struct O_B5B0 *a, int b) {
    struct O_B5B0 *t;
    struct QSema s;
    int h;
    if (b) {
        b5_00425C60(D_00498178);
        return -1;
    }
    b5_00423DE0(a->f28);
    t = b5_0040B208(D_00528FC0);
    if (t == 0)
        return -1;
    a->f4 = t->f18;
    a->f0 = (int)t;
    t->f20 = a->f10;
    t->f14 = (int)t;
    t->f1C = (int)a;
    t->f24 = a->f24;
    s.max_count = 1;
    s.init_count = 0;
    s.option = 0;
    h = b5_00423DA0(&s);
    a->f8 = h;
    if (h < 0) {
        b5_0040B2B0(t);
        return -3;
    }
    if (b5_004261B0(0x8000001D, t, 0x40, 0, 0, 0) == 0) {
        b5_0040B2B0(t);
        b5_00423DB0(h);
        return -2;
    }
    b5_00423DE0(h);
    b5_00423DB0(h);
    b5_00423DB0(a->f28);
    return a->f2C;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040B6D0);
#ifdef SKIP_ASM
struct N_B6D0 { int f0; int f4; int f8; char padC[8]; int f14; int f18; int f1C; int f20; int f24; int f28; int f2C; int f30; int f34; };
extern void lg3_B6D0(char *, char *, int) __asm__("func_00425C60");
extern void lg1_B6D0(char *) __asm__("func_00425C60");
extern int wt_B6D0(int) __asm__("func_00423DE0");
extern int fr_B6D0(int) __asm__("func_00423DB0");
extern int sm_B6D0(int *) __asm__("func_00423DA0");
extern struct N_B6D0 *al_B6D0(char *) __asm__("func_0040B208");
extern void rl_B6D0(struct N_B6D0 *) __asm__("func_0040B2B0");
extern void sz_B6D0(int, int) __asm__("func_00426358");
extern int st_B6D0(unsigned, struct N_B6D0 *, int, int, int, int) __asm__("func_004261B0");
extern char D_00498178[];
extern char D_004981A0[];
extern char D_004981D8[];
extern char D_004981E8[];
int func_0040B6D0(struct N_B6D0 *c, int a, int flag, int d, int e, int f, int g, int h, int i); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B938);
#ifdef SKIP_ASM
extern int func_00427D60(char *, int);
extern void func_004283A0(int, char *, int);
extern void func_00427FE8(int);
extern char D_00498220[];
int func_0040B938(void) {
    int h;
    if (D_00453400[0] == 0) {
        h = func_00427D60(D_00498220, 1);
        if (h >= 0) {
            func_004283A0(h, D_00453400, 14);
            func_00427FE8(h);
        }
    }
    return (int)D_00453400;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B9A8);
#ifdef SKIP_ASM
extern int func_0040B938(void);
int func_0040B9A8(void) {
    if (D_00453400[0] == 0)
        func_0040B938();
    return D_00453400[4] == 0x54;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B9E8);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void func_00423E50(unsigned *);
int func_0040B9E8(void) {
    unsigned buf[1];
    int r;
    func_00423E50(buf);
    if (func_0040B9A8()) {
        r = D_004533FC[0];
    } else {
        func_00423E50(buf);
        if (((buf[0] >> 13) & 7) == 0)
            r = (buf[0] >> 4) & 1;
        else
            r = (buf[0] >> 16) & 0x1F;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BA48);
#ifdef SKIP_ASM
void func_0040BA48(sSysConfig *p) {
    D_004533F8 = *p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BA68);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void func_00423E50(unsigned *);
int func_0040BA68(void) {
    unsigned buf[1];
    if (func_0040B9A8())
        return D_004533FA[0];
    func_00423E50(buf);
    return (buf[0] >> 1) & 3;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BAA8);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void func_00423E50(unsigned *);
int func_0040BAA8(void) {
    unsigned buf[1];
    if (func_0040B9A8())
        return D_004533FD[0];
    func_00423E50(buf);
    return buf[0] & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BAE8);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void func_00423E50(unsigned *);
int func_0040BAE8(void) {
    unsigned buf[1];
    int r;
    if (func_0040B9A8()) {
        r = D_004533F8.f0;
    } else {
        func_00423E50(buf);
        if (((buf[0] >> 13) & 7) == 0) r = 0x21C; else r = (int)buf[0] >> 21;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BB38);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void func_00423E50(unsigned *);
extern int func_004240B0(unsigned *, int, int);
int func_0040BB38(void) {
    unsigned buf[2];
    unsigned char v;
    int r;
    if (func_0040B9A8()) {
        r = D_004533FB[0];
    } else {
        func_00423E50(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            r = 0;
        } else {
            func_004240B0(&buf[1], 1, 1);
            v = ((unsigned char *)buf)[4];
            r = v >> 6;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BBA0);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void func_00423E50(unsigned *);
extern int func_004240B0(unsigned *, int, int);
int func_0040BBA0(void) {
    unsigned buf[2];
    unsigned char v;
    int r;
    if (func_0040B9A8()) {
        r = D_004533FE[0];
    } else {
        func_00423E50(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            r = 0;
        } else {
            func_004240B0(&buf[1], 1, 1);
            v = ((unsigned char *)buf)[4];
            r = (v >> 4) & 1;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BC08);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void func_00423E50(unsigned *);
extern int func_004240B0(unsigned *, int, int);
int func_0040BC08(void) {
    unsigned buf[2];
    unsigned char v;
    int r;
    if (func_0040B9A8()) {
        r = D_004533FF[0];
    } else {
        func_00423E50(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            r = 0;
        } else {
            func_004240B0(&buf[1], 1, 1);
            v = ((unsigned char *)buf)[4];
            r = (v >> 5) & 1;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BC70);
#ifdef SKIP_ASM
unsigned char func_0040BC70(unsigned char a) {
    return a + (a / 10) * 6;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040BCA0);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BCC0);
#ifdef SKIP_ASM
void func_0040BCC0(unsigned char *p) {
    p[7] = func_0040BCA0(p[7]);
    p[6] = func_0040BCA0(p[6]);
    p[5] = func_0040BCA0(p[5]);
    p[3] = func_0040BCA0(p[3]);
    p[2] = func_0040BCA0(p[2]);
    p[1] = func_0040BCA0(p[1]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BD28);
#ifdef SKIP_ASM
void func_0040BD28(unsigned char *p) {
    p[7] = func_0040BC70(p[7]);
    p[6] = func_0040BC70(p[6]);
    p[5] = func_0040BC70(p[5]);
    p[3] = func_0040BC70(p[3]);
    p[2] = func_0040BC70(p[2]);
    p[1] = func_0040BC70(p[1]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BD90);
#ifdef SKIP_ASM
typedef struct { char b[12]; } sDaysTbl;
typedef struct { unsigned char stat, sec, min, hour, pad, day, month, year; } sClk;
extern sDaysTbl D_00498230;
void func_0040BD90(unsigned char *a) {
    sClk *c = (sClk *)a;
    sDaysTbl days = D_00498230;
    c->day++;
    if ((c->year % 4) == 0)
        days.b[1] = 29;
    if (days.b[c->month - 1] < c->day) {
        c->day = 1;
        c->month++;
        if (c->month == 13) {
            if (c->year == 99)
                c->year = 0;
            else
                c->year++;
            c->month = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BE48);
#ifdef SKIP_ASM
void func_0040BE48(unsigned char *a) {
    sClk *c = (sClk *)a;
    sDaysTbl days = D_00498230;
    c->day--;
    if ((c->year % 4) == 0)
        days.b[1] = 29;
    if (c->day == 0) {
        c->month--;
        if (c->month == 0) {
            if (c->year == 0)
                c->year = 99;
            else
                c->year--;
            c->month = 12;
        }
        c->day = days.b[c->month - 1];
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BEF0);
#ifdef SKIP_ASM
extern void func_0040BD90(unsigned char *);
void func_0040BEF0(unsigned char *p) {
    p[3]++;
    if (p[3] == 24) {
        p[3] = 0;
        func_0040BD90(p);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BF20);
#ifdef SKIP_ASM
extern void func_0040BE48(unsigned char *);
void func_0040BF20(unsigned char *p) {
    if (p[3] == 0) {
        p[3] = 23;
        func_0040BE48(p);
    } else {
        p[3]--;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BF48);
#ifdef SKIP_ASM
extern void func_0040BCC0(unsigned char *);
extern void func_0040BEF0(unsigned char *);
extern void func_0040BF20(unsigned char *);
extern void func_0040BD28(unsigned char *);
void func_0040BF48(int a, int d0) {
    int d;
    unsigned char *p = (unsigned char *)a;
    func_0040BCC0(p);
    d = p[2] + d0;
    if (d >= 0) {
        if (d > 60) {
            do {
                d -= 60;
                func_0040BEF0(p);
            } while (d > 60);
        }
        p[2] = d;
    } else {
        while (d < 0) {
            d += 60;
            func_0040BF20(p);
        }
        p[2] = d;
    }
    func_0040BD28(p);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BFD8);
#ifdef SKIP_ASM
extern void func_0040BF48(int, int);
void func_0040BFD8(int a) {
    func_0040BF48(a, -540);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BFE0);
#ifdef SKIP_ASM
extern int func_0040BAE8(void);
extern int func_0040BBA0(void);
extern void func_0040BF48(int, int);
void func_0040BFE0(int a) {
    int t = func_0040BAE8();
    func_0040BF48(a, t - 540 + func_0040BBA0() * 60);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C028);
#ifdef SKIP_ASM
struct KbInfo_C028 { int n; char pad[0x8C]; };
extern char D_00498240[];
extern char D_00498258[];
extern void k_6408(int) __asm__("func_00426408");
extern int k_sema(struct QSema *) __asm__("func_00423DA0");
extern int k_dsema(int) __asm__("func_00423DB0");
extern void k_log(char *) __asm__("func_00425C60");
extern int sceUsbKbGetInfo(void *info);
extern int k_C810(int, int *) __asm__("func_0040C810");
extern int k_3A80(int, int (*)(void), int) __asm__("func_00423A80");
extern int k_48E8(int) __asm__("func_004248E8");
extern int func_0040CF88(void);
int func_0040C028(int *out) {
    struct KbInfo_C028 info;
    struct QSema s;
    int st;
    unsigned i;
    k_6408(0);
    for (;;) {
        if (func_00426B48(&D_005290A0, 0x80000211, 0) < 0) {
            k_log(D_00498240);
            return -1;
        }
        if (D_005290A0.server != 0)
            break;
        i = 0x10000;
        while (i--);
    }
    s.init_count = 1;
    s.max_count = 1;
    D_00453438 = k_sema(&s);
    if (D_00453438 == -1) {
        k_log(D_00498258);
        return -1;
    }
    if (sceUsbKbGetInfo(&info) != 0) {
        k_dsema(D_00453438);
        D_00453438 = -1;
        return -1;
    }
    k_C810(0, &st);
    if (st != 0) {
        k_dsema(D_00453438);
        D_00453438 = -1;
        return -1;
    }
    if (info.n <= 0) {
        k_dsema(D_00453438);
        D_00453438 = -1;
        return -1;
    }
    if (info.n >= 0x80) {
        k_dsema(D_00453438);
        D_00453438 = -1;
        return -1;
    }
    D_00529000.n = info.n;
    *out = info.n;
    D_00529000.port = D_005291D0;
    if (D_00529000.port == 0) {
        k_dsema(D_00453438);
        D_00453438 = -1;
        return -1;
    }
    for (i = 0; i < info.n; i++) {
        D_00529000.port[i].rep.n = 0;
        D_00529000.port[i].ledMode = 1;
        D_00529000.port[i].f04 = 1;
        D_00529000.port[i].f08 = 1;
        D_00529000.port[i].f0C = 0;
        D_00529000.port[i].rep.state = 0;
        D_00529000.port[i].rep.count = 0;
        D_00529000.port[i].rep.delay = 0;
        D_00529000.port[i].rep.rate = 0;
        D_00529000.port[i].rep.wr = 0;
        D_00529000.port[i].rep.rd = 0;
        D_00529000.port[i].rep.last = 0;
    }
    D_00529000.prev.n = 0;
    D_00529000.thread = k_3A80(3, func_0040CF88, -1);
    if (D_00529000.thread < 0) {
        k_dsema(D_00453438);
        D_00453438 = -1;
        return -1;
    }
    k_48E8(3);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C220);
#ifdef SKIP_ASM
extern int func_0042C078_0() __asm__("func_0042C078");
extern void func_0042C0C0(void);
extern int func_00423AA0(int, int);
extern void func_00424880(int);
extern int func_00423DB0(int);
extern char D_00498278[];
extern char D_004982A8[];
int func_0040C220(void) {
    int n = 0;
    int s, r;
    s = func_0042C078_0();
    r = func_00423AA0(3, D_0052909C);
    if (r == 0)
        func_00424880(3);
    if (s)
        func_0042C0C0();
    if (r < 0) {
        n = 1;
        func_0040A618_log(D_00498278);
    }
    if (D_00453438 != -1) {
        if (func_00423DB0(D_00453438) < 0) {
            n++;
            func_0040A618_log(D_004982A8);
        } else {
            D_00453438 = -1;
        }
    }
    return n == 0 ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbGetInfo);
#ifdef SKIP_ASM
extern char D_004982D0[];
extern void func_0040C8B0(void *);
extern void func_00423DE0(int);
extern void func_00425C60(char *, int);
int sceUsbKbGetInfo(void *info) {
    int r;
    if (D_00453438 == -1)
        return -1;
    func_00423DE0(D_00453438);
    r = func_00426D18(&D_005290A0, 1, 1, D_00529100, 16, D_00529140, 0x90, func_0040C8B0, 0);
    if (r != 0) {
        func_00425C60(D_004982D0, r);
        return -2;
    }
    D_00529000.cmd = 1;
    D_00529000.out = info;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbRead);
#ifdef SKIP_ASM
extern void func_0040C8B0(void *);
extern void func_00423DE0(int);
extern void func_00425C60(char *, int);
int sceUsbKbRead(unsigned a, void *b) {
    if (D_00453438 == -1)
        return -1;
    if (a >= D_00529000.n)
        return -11;
    D_00529100[0] = a;
    func_00423DE0(D_00453438);
    {
        int r = func_00426D18(&D_005290A0, 2, 1, D_00529100, 16, D_00529140, 0x50, func_0040C8B0, 0);
        if (r != 0) {
            func_00425C60(D_004982F8, r);
            return -2;
        }
    }
    D_00529000.cmd = 2;
    D_00529000.arg = a;
    D_00529000.out = b;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbGetLocation);
#ifdef SKIP_ASM
extern char D_00498320[];
extern void func_0040C8B0(void *);
extern void func_00423DE0(int);
extern void func_00425C60(char *, int);
int sceUsbKbGetLocation(int a, void *b) {
    if (D_00453438 == -1)
        return -1;
    if (a >= D_00529000.n)
        return -11;
    D_00529100[0] = a;
    func_00423DE0(D_00453438);
    {
        int r = func_00426D18(&D_005290A0, 3, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
        if (r != 0) {
            func_00425C60(D_00498320, r);
            return -2;
        }
    }
    D_00529000.cmd = 3;
    D_00529000.arg = a;
    D_00529000.out = b;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetLEDStatus);
#ifdef SKIP_ASM
extern char D_00498350[];
extern void func_0040C8B0(void *);
extern void func_00423DE0(int);
extern void func_00425C60(char *, int);
int sceUsbKbSetLEDStatus(int a, unsigned char b) {
    if (D_00453438 == -1)
        return -1;
    if (a >= D_00529000.n)
        return -11;
    D_00529100[0] = a;
    D_00529100[1] = b;
    func_00423DE0(D_00453438);
    {
        int r = func_00426D18(&D_005290A0, 4, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
        if (r != 0) {
            func_00425C60(D_00498350, r);
            return -2;
        }
    }
    D_00529000.cmd = 4;
    D_00529000.out = 0;
    D_00529000.arg = a;
    return 0;
}

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetLEDMode);
#ifdef SKIP_ASM
extern char D_00498380[];
extern void func_0040C8B0(void *);
extern void func_00423DE0(int);
extern void func_00425C60(char *, int);
int sceUsbKbSetLEDMode(int a, unsigned b) {
    int h = D_00453438;
    if (h == -1)
        return -1;
    if (a >= D_00529000.n)
        return -11;
    if (b > 2)
        return -12;
    D_00529000.port[a].ledMode = b;
    D_00529100[0] = a;
    D_00529100[1] = b;
    func_00423DE0(h);
    {
        int r = func_00426D18(&D_005290A0, 5, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
        if (r != 0) {
            func_00425C60(D_00498380, r);
            return -2;
        }
    }
    D_00529000.cmd = 5;
    D_00529000.out = 0;
    D_00529000.arg = a;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C728);
#ifdef SKIP_ASM
int func_0040C728(int a, int b, int c) {
    int r;
    r = -11;
    if (b < 0)
        return r;
    if (c < 0)
        return -12;
    D_00529004[a].rep.rate = c;
    D_00529004[a].rep.delay = b;
    D_00529004[a].rep.count = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C770);
#ifdef SKIP_ASM
int func_0040C770(int a, unsigned b) {

    if (a >= D_00529000.n)
        return -11;
    if (b >= 2)
        return -12;
    D_00529000.port[a].f04 = b;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C7C0);
#ifdef SKIP_ASM
int func_0040C7C0(int a, unsigned b) {
    if (a >= D_00529000.n)
        return -11;
    if (b >= 3)
        return -12;
    D_00529000.port[a].f08 = b;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C810);
#ifdef SKIP_ASM
extern void kb_signal4(int) __asm__("func_00423DC0");
extern int func_0040C8C0(void);
int func_0040C810(int mode, int *out) {
    if (D_00453438 == -1)
        return -1;
    switch (mode) {
    case 0:
        func_00423DE0(D_00453438);
        *out = func_0040C8C0();
        kb_signal4(D_00453438);
        return 0;
    case 1:
        if (func_00426F08(&D_005290A0) == 0) {
            *out = func_0040C8C0();
            return 0;
        }
        return 1;
    default:
        return -11;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C8B0);
#ifdef SKIP_ASM
extern void func_00423DD0(int);
void func_0040C8B0(void *arg) {
    func_00423DD0(D_00453438);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C8C0);
#ifdef SKIP_ASM
extern int (*D_00453420[])(void);
int func_0040C8C0(void) {
    int r = 0;
    unsigned s = D_00529000.cmd;
    if (s - 1 < 5)
        r = D_00453420[s]();
    D_00529000.cmd = 0;
    D_00529000.out = 0;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C928);
#ifdef SKIP_ASM
struct S40C928 { int n; int m; unsigned char d[1]; };
int func_0040C928(void) {
    struct S40C928 *p = (struct S40C928 *)D_00529098;
    unsigned char *s = D_00529140;
    int i = 0;
    int n = s[0];
    p->n = n;
    p->m = s[1];
    if (n != 0) {
        do {
            p->d[i] = s[2 + i];
            i++;
        } while (i < p->n);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C980);
#ifdef SKIP_ASM
extern int func_0040D010(int);
extern int func_0040D040(int, unsigned short);
extern int func_0040D0B0(int, unsigned short *);
extern int func_0040D118(int, unsigned char, unsigned char, int, unsigned short *, unsigned short *);
int func_0040C980(void) {
    sKbData cur;
    unsigned short tmp[64];
    unsigned short key;
    unsigned char *s = D_00529140;
    sKbData *out = D_00529000.out;
    sKbPort *e = &D_00529000.port[D_00529000.arg];
    sKbRepeat *r = &e->rep;
    int i, j, k;
    if (s[1] == 0xFF)
        return -1;
    if (e->f0C == 1) {
        out->led = s[0];
        out->n = s[1];
        out->mod = s[2];
        if (out->n == 0)
            return 0;
        j = 0;
        for (i = 0; i < out->n; i++) {
            if (s[4 + i])
                out->keys[j++] = s[4 + i];
        }
        out->n = j;
        if (j == 0) {
            out->keys[0] = 0;
            out->n = 1;
        }
        func_0040D118(D_00529000.arg, out->mod, out->led, out->n, out->keys, out->keys);
        return 0;
    }
    switch (r->state) {
    case 1:
        if (r->count >= r->delay) {
            r->count = 0;
            func_0040D010(D_00529000.arg);
            if (r->delay >= 2)
                func_0040D040(D_00529000.arg, 0);
            func_0040D040(D_00529000.arg, r->last);
            if (r->rate >= 2)
                func_0040D040(D_00529000.arg, 0);
            r->state = 2;
        }
        break;
    case 2:
        if (r->count >= r->rate) {
            r->count = 0;
            func_0040D040(D_00529000.arg, r->last);
            if (r->rate >= 2)
                func_0040D040(D_00529000.arg, 0);
        }
        break;
    case 0:
        break;
    }
    if (s[1] == 0) {
        if (func_0040D0B0(D_00529000.arg, &key) == 0) {
            out->n = 1;
            out->keys[0] = key;
            out->led = D_00529000.prev.led;
            out->mod = D_00529000.prev.mod;
        } else {
            out->n = 0;
        }
        return 0;
    }
    if (s[4] == 1) {
        r->state = 0;
        out->n = 0;
        return 0;
    }
    cur.led = s[0];
    cur.n = s[1];
    cur.mod = s[2];
    j = 0;
    for (i = 0; i < cur.n; i++) {
        if (s[4 + i])
            cur.keys[j++] = s[4 + i];
    }
    cur.n = j;
    func_0040D010(D_00529000.arg);
    if (cur.keys[0] == 0 || cur.mod != D_00529000.prev.mod) {
        out->keys[0] = 0;
        out->n = 1;
        r->last = 0;
        out->led = cur.led;
        out->mod = cur.mod;
        r->state = 0;
    } else {
        if (D_00529000.prev.keys[0] != 0) {
            k = 0;
            for (i = 0; i < cur.n; i++) {
                for (j = 0; j < D_00529000.prev.n && cur.keys[i] != D_00529000.prev.keys[j]; j++)
                    ;
                if (j == D_00529000.prev.n)
                    out->keys[k++] = cur.keys[i];
            }
            out->n = k;
        } else {
            for (i = 0; i < cur.n; i++)
                out->keys[i] = cur.keys[i];
            out->n = i;
        }
        out->led = cur.led;
        out->mod = cur.mod;
        func_0040D118(D_00529000.arg, out->mod, out->led, out->n, out->keys, out->keys);
        if (out->n > 0) {
            r->last = out->keys[out->n - 1];
            if (r->delay > 0 && r->rate > 0)
                r->state = 1;
        } else {
            func_0040D118(D_00529000.arg, cur.mod, cur.led, cur.n, cur.keys, tmp);
            for (i = 0; i < cur.n && tmp[i] != r->last; i++)
                ;
            if (i == cur.n) {
                r->state = 0;
                out->n = 1;
                out->keys[0] = 0;
                out->led = cur.led;
                out->mod = cur.mod;
            }
        }
    }
    {
        unsigned char *src = (unsigned char *)&cur;
        unsigned char *dst = (unsigned char *)&D_00529008;
        for (i = 0; i < sizeof(cur); i++)
            *dst++ = *src++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040CEF8);
#ifdef SKIP_ASM
int func_0040CEF8(void) {
    unsigned char *s = D_00529140;
    unsigned char *d = D_00529098;
    int i;
    if (s[6])
        return -1;
    i = 6;
    do {
        *d = *s; i--; s++; d++;
    } while (i >= 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040CF48);
#ifdef SKIP_ASM
int func_0040CF48(void) {
    return D_00529141[0] == D_00529101[0] ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040CF68);
#ifdef SKIP_ASM
int func_0040CF68(void) {
    return D_00529141[0] == D_00529101[0] ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040CF88);
#ifdef SKIP_ASM
int func_0040CF88(void) {
    int i;

    for (i = 0; i < D_00529000.n; i++) {
        sKbRepeat *v = &D_00529000.port[i].rep;
        if (v->delay > 0 && v->rate > 0 && v->state != 0 && v->state >= 0 && v->state < 3)
            v->count++;
    }
    SYNC_0810();
    EI_0810();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D010);
#ifdef SKIP_ASM
int func_0040D010(int i) {
    sKbRepeat *p = &D_00529004[i].rep;
    p->n = 0;
    p->wr = 0;
    p->rd = 0;
    p->count = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D040);
#ifdef SKIP_ASM
int func_0040D040(int a, unsigned short b) {
    sKbRepeat *q = &D_00529004[a].rep;
    if (q->n >= 4)
        return -1;
    q->ring[q->wr++] = b;
    if (q->wr >= 4)
        q->wr = 0;
    q->n++;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D0B0);
#ifdef SKIP_ASM
int func_0040D0B0(int a, unsigned short *out) {
    sKbRepeat *q = &D_00529004[a].rep;
    if (q->n <= 0)
        return -1;
    *out = q->ring[q->rd++];
    if (q->rd >= 4)
        q->rd = 0;
    q->n--;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D118);
#ifdef SKIP_ASM
extern short q_0040D268(int, int, int, unsigned short) __asm__("func_0040D268");
int func_0040D118(int idx, unsigned char b, unsigned char c, int n, unsigned short *src, unsigned short *dst) {
    sKbPort *e = &D_00529004[idx];
    int i;
    int r = 0;
    switch (e->f04) {
    case 0:
        i = 0;
        if (n > 0 && src[0] != 0) {
            do {
                dst[i] = src[i];
                i++;
            } while (i < n && src[i] != 0);
        }
        r = i;
        break;
    case 1:
        i = 0;
        if (n > 0 && src[0] != 0) {
            do {
                dst[i] = q_0040D268(e->f08, b, c, src[i]);
                i++;
            } while (i < n && src[i] != 0);
        }
        r = i;
        break;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D268);
#ifdef SKIP_ASM
extern char D_004983B0[];
extern char D_004983C8[];
extern char D_004987C8[];
extern char D_00498BC8[];
extern void u268_00425C60(char *) __asm__("func_00425C60");
extern unsigned short u268_0040D3C0(int, int) __asm__("func_0040D3C0");
unsigned short func_0040D268(int mode, int a, int b, unsigned short ch) {
    unsigned short *tbl = 0;
    unsigned short v;
    int up = (a & 0x22) != 0;
    switch (mode) {
    case 0:
        tbl = (unsigned short *)(D_004983C8 + (up << 9));
        break;
    case 1:
        tbl = (unsigned short *)(D_004987C8 + (up << 9));
        break;
    case 2:
        tbl = (unsigned short *)(D_00498BC8 + (up << 9));
        break;
    default:
        u268_00425C60(D_004983B0);
        break;
    }
    if (ch == 0)
        return 0;
    if (ch >= 0x100)
        return ch;
    v = tbl[ch];
    if (v == 0x4000)
        v = u268_0040D3C0(b & 0xFF, ch & 0xFF);
    if (v == 0x8000)
        v = ch | 0x8000;
    if ((unsigned short)(v - 0x61) < 0x1A && (b & 2))
        v -= 0x20;
    if ((unsigned short)(v - 0x41) < 0x1A) {
        if (b & 2) {
            if (up)
                v += 0x20;
        }
    }
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D3C0);
#ifdef SKIP_ASM
struct K_D3C0 { unsigned char k; unsigned char p; unsigned short v; };
extern struct K_D3C0 D_00498FC8[2][21];
unsigned short func_0040D3C0(int a, unsigned char b) {
    unsigned i;
    int r = b;
    for (i = 0; i < 21; i++) {
        if (b == D_00498FC8[a & 1][i].k) {
            r = D_00498FC8[a & 1][i].v | 0x4000;
            break;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D430);
#ifdef SKIP_ASM
int func_0040D430(int a, unsigned b) {
    sKbPort *e;
    if (a >= D_00529000.n)
        return -11;
    if (b >= 2)
        return -12;
    /* int-arithmetic address (the typed &D_00529000.port[a] swaps the addu operands) */
    e = (sKbPort *)((char *)(a * 0x38) + (int)D_00529000.port);
    if (b != e->f0C) {
        e->rep.count = 0;
        e->rep.wr = 0;
        e->rep.rd = 0;
        D_00529000.prev.n = 0;
        e->rep.last = 0;
    }
    e->f0C = b;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D4A0);
#ifdef SKIP_ASM
extern void q_00425C60(char *, int) __asm__("func_00425C60");
int func_0040D4A0(unsigned a) {
    int r;
    sKbPort *e;
    if (D_00453438 == -1)
        return -1;
    if (a >= D_00529000.n)
        return -11;
    D_00529100[0] = a;
    func_00423DE0(D_00453438);
    r = func_00426D18(&D_005290A0, 6, 1, D_00529100, 16, D_00529140, 0, func_0040C8B0, 0);
    if (r != 0) {
        q_00425C60(D_004982F8, r);
        return -2;
    }
    e = &D_00529000.port[a];
    e->rep.state = 0;
    e->rep.count = 0;
    e->rep.last = 0;
    e->rep.wr = 0;
    e->rep.rd = 0;
    e->rep.n = 0;
    return 0;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040D588);
#ifdef SKIP_ASM
extern int func_0040E878(float x, float *y);
extern float func_0040F5A8(float x, float y, int iy);
typedef union { float f; int bits; } FloatBits_D588;
#define FLOAT_TO_BITS_D588(i, d) do { FloatBits_D588 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
float func_0040D588(float x); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D610);
#ifdef SKIP_ASM
struct E40D610 { int type; char *name; double arg1; double arg2; double retval; int err; };
extern int D_00499688[];
extern char D_00499070[];
extern double D_00499680[];
extern double core_40D610(double) __asm__("func_0040DB58");
extern int isn_40D610(double) __asm__("func_0040F840");
extern int cmp_40D610(double, double) __asm__("dpcmp");
extern double dpsub(double, double);
extern int matherr_40D610(struct E40D610 *) __asm__("func_0040F878");
extern int *func_00415FB8(void);
double func_0040D610(double x) {
    struct E40D610 e;
    double zero;
    double z = core_40D610(x);
    int ver = D_00499688[0];
    if (ver == -1 || isn_40D610(x) || (zero = 0.0, cmp_40D610(x, zero) > 0))
        return z;
    e.err = 0;
    e.name = D_00499070;
    e.arg2 = e.arg1 = x;
    if (ver == 0)
        e.retval = -3.4028234663852886e+38;
    else
        e.retval = dpsub(zero, D_00499680[0]);
    if (cmp_40D610(x, 0.0) == 0) {
        e.type = 2;
        if (D_00499688[0] == 2)
            *func_00415FB8() = 34;
        else if (!matherr_40D610(&e))
            *func_00415FB8() = 33;
    } else {
        e.type = 1;
        if (D_00499688[0] == 2)
            *func_00415FB8() = 33;
        else if (!matherr_40D610(&e))
            *func_00415FB8() = 33;
    }
    if (e.err != 0)
        *func_00415FB8() = e.err;
    return e.retval;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040D758);

INCLUDE_ASM("seg/seg_2FE810", func_0040D8D0);

INCLUDE_ASM("seg/seg_2FE810", func_0040DA10);

INCLUDE_ASM("seg/seg_2FE810", func_0040DB58);

INCLUDE_ASM("seg/seg_2FE810", func_0040E048);

INCLUDE_ASM("seg/seg_2FE810", func_0040E300);

INCLUDE_ASM("seg/seg_2FE810", func_0040E550);

INCLUDE_ASM("seg/seg_2FE810", func_0040E878);

INCLUDE_ASM("seg/seg_2FE810", func_0040EC58);

INCLUDE_ASM("seg/seg_2FE810", func_0040F5A8);

INCLUDE_ASM("seg/seg_2FE810", func_0040F840);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F878);
#ifdef SKIP_ASM
extern void dpcmp(long, long);
int func_0040F878(long *p) {
    long v = p[1];
    dpcmp(v, v);
    return 0;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040F8A0);
#ifdef SKIP_ASM
typedef union { float f; int bits; } FloatBits_F8A0;
#define FLOAT_TO_BITS_F8A0(i, d) do { FloatBits_F8A0 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
#define BITS_TO_FLOAT_F8A0(d, i) do { FloatBits_F8A0 fb_; fb_.bits = (i); (d) = fb_.f; } while (0)
float func_0040F8A0(float x); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040F8C0);

INCLUDE_ASM("seg/seg_2FE810", func_0040F8E8);

INCLUDE_ASM("seg/seg_2FE810", func_0040F9D0);

INCLUDE_ASM("seg/seg_2FE810", func_0040F9F8);
#ifdef SKIP_ASM
extern float copysignf_F9F8(float, float) __asm__("func_0040FB58");
typedef union { float f; int bits; } FloatBits_F9F8;
#define FLOAT_TO_BITS_F9F8(i, d) do { FloatBits_F9F8 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
#define BITS_TO_FLOAT_F9F8(d, i) do { FloatBits_F9F8 fb_; fb_.bits = (i); (d) = fb_.f; } while (0)
float func_0040F9F8(float x, int n); /* peak3 body held: see notes/stuck.md */

#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040FB58);
#ifdef SKIP_ASM
typedef union { float f; int bits; } FloatBits_FB58;
#define FLOAT_TO_BITS_FB58(i, d) do { FloatBits_FB58 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
#define BITS_TO_FLOAT_FB58(d, i) do { FloatBits_FB58 fb_; fb_.bits = (i); (d) = fb_.f; } while (0)
float func_0040FB58(float x, float y); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040FB88);
#ifdef SKIP_ASM
extern void (**D_0045343C[])(void);
void func_0040FB88(int a) {
    void (**p)(void);
    while (*D_0045343C[0]) {
        p = D_0045343C[0];
        p++;
        D_0045343C[0] = p;
        p[-1]();
    }
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040FBE0);
#ifdef SKIP_ASM
// 98.52%: crt __do_global_ctors; only s0/s1 swapped in the final call loop (counter should be $17, ptr $16)
typedef void (*func_ptr)(void);
extern func_ptr D_0043CE38[0];



void func_0040FBE0(void); /* peak3 body held: see notes/stuck.md */

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040FC90);
#ifdef SKIP_ASM
extern int D_0052AD98[];
extern void func_0040FBE0(void);
void func_0040FC90(void) {
    if (D_0052AD98[0] == 0) {
        D_0052AD98[0] = 1;
        func_0040FBE0();
    }
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040FCB0);
