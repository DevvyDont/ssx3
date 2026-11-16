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
} sThreadParam;                 /* 0x30 bytes, as passed to CreateThread / ReferThreadStatus */
extern int D_00451610;          /* debug print level */
extern int D_00451614;          /* callback thread id (0 = none) */
extern int D_00451618;
extern int D_0045161C;
/* libcdvd state shared with its RPC callback thread: volatile in every user
   (retail re-reads each one; tools/gen/phaseb_import.py VOL_DECL_OK) */
extern volatile int D_00451620;          /* callback thread semaphore */
extern volatile int D_00451624;  /* callback-entry flag: read by the SIF callback func_00400DD8 */
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FD810);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3); peak3 spelled the table as raw numbers.
// PORT: PS2-only, not code: a function whose body is an asm table of the addresses of 59 functions and data objects
// (the compiler adds the `jr $31; nop` after it). The loader func_003FCFD8 hands it, with its end marker func_003FD908,
// to func_003FD660. The fork's config/text_word_relocs.txt gives retail's copy the same relocations. A port replaces
// it with an array of pointers.
void func_003FD810(void) {
    __asm__ __volatile__(
        ".word func_00425C28\n"
        ".word func_00425C60\n"
        ".word func_0040B6D0\n"
        ".word func_0040B400\n"
        ".word func_0040B5B0\n"
        ".word func_00401718\n"
        ".word func_00402268\n"
        ".word func_00402520\n"
        ".word func_00402450\n"
        ".word func_00401350\n"
        ".word func_00401BF0\n"
        ".word func_004014F8\n"
        ".word D_00451680\n"
        ".word D_00451628\n"
        ".word D_004527D0\n"
        ".word func_004011E0\n"
        ".word D_00452800\n"
        ".word D_00453180\n"
        ".word D_0045162C\n"
        ".word CreateThread\n"
        ".word StartThread\n"
        ".word DeleteThread\n"
        ".word ChangeThreadPriority\n"
        ".word ReferThreadStatus\n"
        ".word TerminateThread\n"
        ".word GetThreadId\n"
        ".word CreateSema\n"
        ".word WaitSema\n"
        ".word DeleteSema\n"
        ".word SignalSema\n"
        ".word PollSema\n"
        ".word FlushCache\n"
        ".word func_00426D18\n"
        ".word func_00427D60\n"
        ".word func_00427FE8\n"
        ".word func_004283A0\n"
        ".word func_00428600\n"
        ".word func_00428168\n"
        ".word func_00424570\n"
        ".word func_004245A0\n"
        ".word func_00426B48\n"
        ".word iSignalSema\n"
        ".word func_00423B80\n"
        ".word SetAlarm\n"
        ".word func_00423B30\n"
        ".word func_00424880\n"
        ".word RemoveIntcHandler\n"
        ".word GsPutIMR\n"
        ".word SetGsCrt\n"
        ".word func_00424558\n"
        ".word func_00424550\n"
        ".word func_00424460\n"
        ".word func_00424368\n"
        ".word func_00424488\n"
        ".word func_00424498\n"
        ".word func_004243E8\n"
        ".word func_00423A80\n"
        ".word func_004248E8\n"
        ".word D_00455248\n"
    );
}
#endif

void func_003FD908(void) {
}

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FD910);
#ifdef SKIP_ASM
struct S3FD910 { short a; short b; short c; unsigned short d; int e; int f; };
extern int RemoveIntcHandler(int, int);
extern void func_00424880(int);
extern void SetGsCrt(int, int, int);
extern int GsPutIMR(int);
void *func_003FDAA0(void);
void func_003FD910(short mode, short interlace, short videoMode, short frameMode) {
    struct S3FD910 *gsState;
    int e;
    switch (mode) {
    case 0:
        gsState = func_003FDAA0();
        *(volatile long *)0x12001000 = 0x200;
        gsState->a = interlace;
        gsState->b = videoMode;
        gsState->d = (*(volatile unsigned long *)0x12001000 >> 16) & 0xFF;
        GsPutIMR(0xFF00);
        gsState->c = !!frameMode;
        if (gsState->e) {
            func_00424880(2);
            RemoveIntcHandler(2, gsState->f);
            gsState->e = 0;
            gsState->f = 0;
        }
        SetGsCrt(interlace & 1, videoMode & 0xFF, frameMode & 1);
        break;
    case 1:
        *(volatile long *)0x12001000 = 0x100;
        break;
    case 5:
        gsState = func_003FDAA0();
        gsState->c = frameMode != 0;
        gsState->a = interlace;
        gsState->b = videoMode;
        gsState->d = (*(volatile unsigned long *)0x12001000 >> 16) & 0xFF;
        SetGsCrt(interlace & 1, videoMode & 0xFF, frameMode & 1);
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDAB0);
#ifdef SKIP_ASM
typedef unsigned int u_long128_DAB0 __attribute__((mode(TI))); /* PORT: Sony's u_long128 (eetypes.h) */
extern u_long128_DAB0 D_00451560[2]; /* VIF1 init codes sent to the VIF1 FIFO */
/* Reset VIF1 and the GIF and send the VIF1 init codes (libgraph sceGsResetPath). */
void func_003FDAB0(void) {
    *(volatile int *)0x10003C10 = 1;    /* VIF1_FBRST */
    *(volatile int *)0x10003C20 = 2;    /* VIF1_ERR */
    /* PORT: PS2-only barrier and VU0 FBRST ($vi28) write: sync.l, cfc2/ori/ctc2, sync.p. */
    __asm__ volatile("sync.l\n\tcfc2 %0, $vi28\n\tori %0, %0, 0x200\n\tctc2 %0, $vi28\n\tsync.p" : : "r"(0));
    *(volatile u_long128_DAB0 *)0x10005000 = D_00451560[0];    /* VIF1_FIFO */
    *(volatile u_long128_DAB0 *)0x10005000 = D_00451560[1];
    *(volatile int *)0x10003000 = 1;    /* GIF_CTRL */
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceGsSyncPath);
#ifdef SKIP_ASM
extern void func_00425C60();   /* printf */
extern char D_00497648[];
extern char D_00497678[];
extern char D_00497688[];
extern char D_00497698[];
extern char D_004976A8[];
extern char D_004976B8[];
extern char D_004976C8[];
extern char D_004976D8[];
extern char D_004976E8[];
extern char D_004976F8[];
extern char D_00497710[];
extern char D_00497728[];
extern char D_00497758[];
extern char D_00497788[];
extern char D_004977B0[];
#define SYNC_D1_CHCR ((volatile unsigned int *)0x10009000)
#define SYNC_D1_MADR ((volatile unsigned int *)0x10009010)
#define SYNC_D1_QWC  ((volatile unsigned int *)0x10009020)
#define SYNC_D1_TADR ((volatile unsigned int *)0x10009030)
#define SYNC_D2_CHCR ((volatile unsigned int *)0x1000A000)
#define SYNC_D2_MADR ((volatile unsigned int *)0x1000A010)
#define SYNC_D2_QWC  ((volatile unsigned int *)0x1000A020)
#define SYNC_D2_TADR ((volatile unsigned int *)0x1000A030)
#define SYNC_VIF1_STAT ((volatile unsigned int *)0x10003C00)
#define SYNC_GIF_STAT ((volatile unsigned int *)0x10003020)
/* PORT: PS2-only VU1 status read (VPU_STAT, $vi29) through VU0 macro mode. */
static inline unsigned int sync_vpu_stat(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi29\n\t.set reorder" : "=r"(v));
    return v;
}
/* Report a path that did not drain: the message, then the DMA ch.1/ch.2, VIF1 and GIF registers. */
static inline int sync_timeout(char *msg)
{
    func_00425C60(msg);
    func_00425C60(D_00497678, *SYNC_D1_CHCR);
    func_00425C60(D_00497688, *SYNC_D1_TADR);
    func_00425C60(D_00497698, *SYNC_D1_MADR);
    func_00425C60(D_004976A8, *SYNC_D1_QWC);
    func_00425C60(D_004976B8, *SYNC_D2_CHCR);
    func_00425C60(D_004976C8, *SYNC_D2_TADR);
    func_00425C60(D_004976D8, *SYNC_D2_MADR);
    func_00425C60(D_004976E8, *SYNC_D2_QWC);
    func_00425C60(D_004976F8, *SYNC_VIF1_STAT);
    func_00425C60(D_00497710, *SYNC_GIF_STAT);
    return -1;
}
/* Wait for (mode 0) or poll (mode 1) the DMA ch.1/ch.2, VIF1, VU1 and GIF paths (libgraph). */
int sceGsSyncPath(int mode, unsigned short timeout) {
    unsigned int cnt;
    unsigned int stat;
    int ret;
    cnt = 0;
    if (mode == 0) {
        while (*SYNC_D1_CHCR & 0x100) {
            if (cnt++ > 0x1000000)
                return sync_timeout(D_00497648);
        }
        while (*SYNC_D2_CHCR & 0x100) {
            if (cnt++ > 0x1000000)
                return sync_timeout(D_00497728);
        }
        while (*SYNC_VIF1_STAT & 0x1F000003) {
            if (cnt++ > 0x1000000)
                return sync_timeout(D_00497758);
        }
        stat = sync_vpu_stat();
        while (stat & 0x100) {
            if (cnt++ > 0x1000000)
                return sync_timeout(D_00497788);
            stat = sync_vpu_stat();
        }
        while (*SYNC_GIF_STAT & 0xC00) {
            if (cnt++ > 0x1000000)
                return sync_timeout(D_004977B0);
        }
        return 0;
    }
    ret = 0;
    if (*SYNC_D1_CHCR & 0x100)
        ret |= 1;
    if (*SYNC_D2_CHCR & 0x100)
        ret |= 2;
    if (*SYNC_VIF1_STAT & 0x1F000003)
        ret |= 4;
    stat = sync_vpu_stat();
    if (stat & 0x100)
        ret |= 8;
    if (*SYNC_GIF_STAT & 0xC00)
        ret |= 0x10;
    return ret;
}
#endif

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
unsigned func_003FDF28(unsigned addr) {
    if ((addr >> 28) == 7) {
        addr &= 0xFFFFFFF;
        addr |= 0x80000000;
    }
    return addr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDF50);
#ifdef SKIP_ASM
void func_003FDF50(char *dst, int n) {
    while (n--)
        *dst++ = 0;
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
int func_003FE2A0(int newValue) {
    volatile int *stadrReg = (volatile int *)0x1000E060;
    int oldValue = *stadrReg;
    *stadrReg = newValue;
    return oldValue;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE2B8);
#ifdef SKIP_ASM
extern unsigned func_003FDF28(unsigned);
extern void func_003FDE30();
void func_003FE2B8(sDmaChan *dmaChan, unsigned addr) {
    unsigned physAddr = func_003FDF28(addr);
    func_003FDE30(dmaChan);
    if (dmaChan->tadr != 0xFFFFFFFF)
        dmaChan->tadr = physAddr;
    dmaChan->qwc = 0;
    dmaChan->chcr = (dmaChan->chcr & ~0xC) | 0x105;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE320);
#ifdef SKIP_ASM
void func_003FE320(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = func_003FDF28(addr);
    func_003FDE30(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    dmaChan->chcr = (dmaChan->chcr & ~0xC) | 0x101;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE398);
#ifdef SKIP_ASM
void func_003FE398(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = func_003FDF28(addr);
    func_003FDE30(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    dmaChan->chcr = (dmaChan->chcr & ~0xC) | 0x109;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE410);
#ifdef SKIP_ASM
extern void func_003FDE30();
void func_003FE410(sDmaChan *dmaChan) {
    func_003FDE30(dmaChan);
    dmaChan->qwc = 0;
    dmaChan->chcr = ((dmaChan->chcr & ~0xC | 0x4) & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE458);
#ifdef SKIP_ASM
void func_003FE458(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = func_003FDF28(addr);
    int chcrBits;
    func_003FDE30(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    chcrBits = dmaChan->chcr & ~0xC;
    dmaChan->chcr = (chcrBits & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE4D8);
#ifdef SKIP_ASM
void func_003FE4D8(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = func_003FDF28(addr);
    int chcrBits;
    func_003FDE30(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    chcrBits = (dmaChan->chcr & ~0xC) | 8;
    dmaChan->chcr = (chcrBits & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE558);
#ifdef SKIP_ASM
extern void func_003FDE30(sDmaChan *, int);
int func_003FE558(sDmaChan *dmaChan, int mode) {
    if (mode == 1)
        return ((unsigned)dmaChan->chcr >> 8) & 1;
    func_003FDE30(dmaChan, mode);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE590);
#ifdef SKIP_ASM
extern void func_003FDEA0(sDmaChan *, unsigned, int);
int func_003FE590(sDmaChan *dmaChan, unsigned n, int mode, int x) {
    if (mode == 1)
        return (unsigned)dmaChan->madr < n;
    func_003FDEA0(dmaChan, n, x);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE5C8);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
/* force-stop a channel: suspend DMA (D_ENABLEW), clear CHCR.STR, restore */
int func_003FE5C8(sDmaChan *dmaChan) {
    int x, enabler, intrState;
    unsigned oldChcr;
    intrState = func_0042C078();
    enabler = *(volatile int *)0x1000F520;
    if ((enabler & 0x10000) == 0)
        *(volatile int *)0x1000F590 = enabler | 0x10000;
    x = *(volatile int *)0x1000E000;
    oldChcr = dmaChan->chcr;
    dmaChan->chcr = oldChcr & 0xFFFFFEFF;
    *(volatile int *)0x1000F590 = enabler;
    if (intrState)
        func_0042C0C0();
    return oldChcr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE660);
#ifdef SKIP_ASM
unsigned func_003FE660(unsigned *reg, unsigned newValue) {
    unsigned oldValue = *reg;
    *reg = newValue;
    return (oldValue >> 8) & 1;
}
#endif

//100%
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
int func_003FE678(void) {
    return (vu0_cfc2_678() & 0xF) == 1;
}
#endif

//100%
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
void func_003FE690(void) {
    vu0_ctc2_vi28_690(vu0_cfc2_vi28_690() | 0x2);
}
#endif

//100%
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
int func_003FE6A8(void) {
    int result = 0;
    switch (func_003FE7D8_6A8()) {
    case 4:
        vu0_ctc2_vi28_6A8(vu0_cfc2_vi28_6A8() | 0x1);
    case 0:
    case 3:
        result = 1;
        break;
    case 1:
    case 2:
        break;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE708);
#ifdef SKIP_ASM
extern int func_003FE7D8(void);
extern int func_003FE828(void);
extern void func_003FE818(int);
int func_003FE708(void) {
    int result = 0;
    int n = func_003FE7D8();
    if (n < 4) {
        if (n > 0) {
            func_003FE818(func_003FE828());
            result = 1;
        }
    }
    return result;
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
void func_003FE758(int enable) {
    unsigned int ctrlBits = vu0_cfc2_758();
    if (enable == 1)
        ctrlBits |= 0x4;
    else
        ctrlBits &= ~0x4;
    vu0_ctc2_758(ctrlBits);
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
void func_003FE788(int enable) {
    unsigned int ctrlBits = vu0_cfc2_788();
    if (enable == 1)
        ctrlBits |= 0x8;
    else
        ctrlBits &= ~0x8;
    vu0_ctc2_788(ctrlBits);
}
#endif

//100%
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
int func_003FE7B8(void) {
    return (vu0_cfc2_vi28_7B8() >> 2) & 1;
}
#endif

//100%
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
int func_003FE7C8(void) {
    return (vu0_cfc2_7C8() >> 3) & 1;
}
#endif

//100%
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
int func_003FE7D8(void) {
    unsigned int vpuStat = vu0_cfc2_vi29_7D8();
    if (vpuStat & 2)
        return 1;
    if (vpuStat & 4)
        return 2;
    if (vpuStat & 8)
        return 3;
    return (vpuStat & 1) << 2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE818);
#ifdef SKIP_ASM
/* Start the VU0 micro program at address `addr` (low 16 bits) through $vi27 (CMSAR0). */
/* PORT: PS2-only VU0 macro-mode ctc2 + vcallmsr. */
void func_003FE818(int addr) {
    __asm__ volatile("ctc2 %0, $vi27\n\tvcallmsr $vi27" : : "r"(addr & 0xFFFF));
}
#endif

//100%
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
int func_003FE828(void) {
    return vu0_cfc2_828() & 0xFFFF;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE838);
#ifdef SKIP_ASM
typedef unsigned int u_long128_E838 __attribute__((mode(TI))); /* PORT: Sony's u_long128 (eetypes.h) */
/* VU0 register snapshot (libdev sceDevVu0Cnd, 0x238 bytes). */
typedef struct {
    u_long128_E838 vf[32];
    unsigned int status;
    unsigned int mac;
    unsigned int clipping;
    unsigned int r;
    unsigned int i;
    unsigned int q;
    unsigned short vi[16];
} sVu0Cnd;
extern int func_003FE678(void);
/* Snapshot the VU0 registers into *cnd (libdev sceDevVu0GetCnd); fails while VU0 is running. */
int func_003FE838(sVu0Cnd *cnd) {
    unsigned int regValue;
    if (func_003FE678() != 0)
        return 0;
    /* PORT: PS2-only VU0 register dump (sqc2 $vf0..$vf31). Retail's base register holds 0 here, not cnd. */
    __asm__ volatile("sqc2 $vf0, 0x0(%0)\n\tsqc2 $vf1, 0x10(%0)\n\tsqc2 $vf2, 0x20(%0)\n\tsqc2 $vf3, 0x30(%0)\n\tsqc2 $vf4, 0x40(%0)\n\tsqc2 $vf5, 0x50(%0)\n\tsqc2 $vf6, 0x60(%0)\n\tsqc2 $vf7, 0x70(%0)\n\tsqc2 $vf8, 0x80(%0)\n\tsqc2 $vf9, 0x90(%0)\n\tsqc2 $vf10, 0xA0(%0)\n\tsqc2 $vf11, 0xB0(%0)\n\tsqc2 $vf12, 0xC0(%0)\n\tsqc2 $vf13, 0xD0(%0)\n\tsqc2 $vf14, 0xE0(%0)\n\tsqc2 $vf15, 0xF0(%0)\n\tsqc2 $vf16, 0x100(%0)\n\tsqc2 $vf17, 0x110(%0)\n\tsqc2 $vf18, 0x120(%0)\n\tsqc2 $vf19, 0x130(%0)\n\tsqc2 $vf20, 0x140(%0)\n\tsqc2 $vf21, 0x150(%0)\n\tsqc2 $vf22, 0x160(%0)\n\tsqc2 $vf23, 0x170(%0)\n\tsqc2 $vf24, 0x180(%0)\n\tsqc2 $vf25, 0x190(%0)\n\tsqc2 $vf26, 0x1A0(%0)\n\tsqc2 $vf27, 0x1B0(%0)\n\tsqc2 $vf28, 0x1C0(%0)\n\tsqc2 $vf29, 0x1D0(%0)\n\tsqc2 $vf30, 0x1E0(%0)\n\tsqc2 $vf31, 0x1F0(%0)" : : "r"(0));
    /* PORT: PS2-only VU0 control-register reads (cfc2). The EE interlocks COP2 moves, so no hazard nop follows. */
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi16\n\t.set reorder" : "=r"(regValue));
    cnd->status = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi17\n\t.set reorder" : "=r"(regValue));
    cnd->mac = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi18\n\t.set reorder" : "=r"(regValue));
    cnd->clipping = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi20\n\t.set reorder" : "=r"(regValue));
    cnd->r = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi21\n\t.set reorder" : "=r"(regValue));
    cnd->i = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi22\n\t.set reorder" : "=r"(regValue));
    cnd->q = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi0\n\t.set reorder" : "=r"(regValue));
    cnd->vi[0] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi1\n\t.set reorder" : "=r"(regValue));
    cnd->vi[1] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi2\n\t.set reorder" : "=r"(regValue));
    cnd->vi[2] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi3\n\t.set reorder" : "=r"(regValue));
    cnd->vi[3] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi4\n\t.set reorder" : "=r"(regValue));
    cnd->vi[4] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi5\n\t.set reorder" : "=r"(regValue));
    cnd->vi[5] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi6\n\t.set reorder" : "=r"(regValue));
    cnd->vi[6] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi7\n\t.set reorder" : "=r"(regValue));
    cnd->vi[7] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi8\n\t.set reorder" : "=r"(regValue));
    cnd->vi[8] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi9\n\t.set reorder" : "=r"(regValue));
    cnd->vi[9] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi10\n\t.set reorder" : "=r"(regValue));
    cnd->vi[10] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi11\n\t.set reorder" : "=r"(regValue));
    cnd->vi[11] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi12\n\t.set reorder" : "=r"(regValue));
    cnd->vi[12] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi13\n\t.set reorder" : "=r"(regValue));
    cnd->vi[13] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi14\n\t.set reorder" : "=r"(regValue));
    cnd->vi[14] = regValue;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi15\n\t.set reorder" : "=r"(regValue));
    cnd->vi[15] = regValue;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE998);
#ifdef SKIP_ASM
typedef unsigned int u_long128_E998 __attribute__((mode(TI))); /* PORT: Sony's u_long128 (eetypes.h) */
/* VU0 register snapshot (libdev sceDevVu0Cnd, 0x238 bytes). */
typedef struct {
    u_long128_E998 vf[32];
    unsigned int status;
    unsigned int mac;
    unsigned int clipping;
    unsigned int r;
    unsigned int i;
    unsigned int q;
    unsigned short vi[16];
} sVu0Cnd_E998;
extern int func_003FE678(void);
/* Load the VU0 registers from *cnd (libdev sceDevVu0PutCnd); fails while VU0 is running. */
int func_003FE998(sVu0Cnd_E998 *cnd) {
    u_long128_E998 *vf = cnd->vf;
    unsigned int regValue;
    if (func_003FE678() != 0)
        return 0;
    /* PORT: PS2-only VU0 register load (lqc2 $vf0..$vf31). */
    __asm__ volatile("lqc2 $vf0, 0x0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf1, 0x10(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf2, 0x20(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf3, 0x30(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf4, 0x40(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf5, 0x50(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf6, 0x60(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf7, 0x70(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf8, 0x80(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf9, 0x90(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf10, 0xA0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf11, 0xB0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf12, 0xC0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf13, 0xD0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf14, 0xE0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf15, 0xF0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf16, 0x100(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf17, 0x110(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf18, 0x120(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf19, 0x130(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf20, 0x140(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf21, 0x150(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf22, 0x160(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf23, 0x170(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf24, 0x180(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf25, 0x190(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf26, 0x1A0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf27, 0x1B0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf28, 0x1C0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf29, 0x1D0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf30, 0x1E0(%0)" : : "r"(vf));
    __asm__ volatile("lqc2 $vf31, 0x1F0(%0)" : : "r"(vf));
    /* PORT: PS2-only VU0 control-register writes (ctc2). */
    regValue = cnd->status;
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi16\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->mac;
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi17\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->clipping;
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi18\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->r;
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi20\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->i;
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi21\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->q;
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi22\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[0];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi0\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[1];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi1\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[2];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi2\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[3];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi3\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[4];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi4\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[5];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi5\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[6];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi6\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[7];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi7\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[8];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi8\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[9];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi9\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[10];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi10\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[11];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi11\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[12];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi12\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[13];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi13\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[14];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi14\n\t.set reorder" : : "r"(regValue));
    regValue = cnd->vi[15];
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi15\n\t.set reorder" : : "r"(regValue));
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FEB08);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 control-register read. The EE interlocks COP2 moves, so no hazard nop follows.
static inline unsigned int vu0_cfc2_vi29_EB08(void)
{
    unsigned int v;
    __asm__ volatile(".set noreorder\n\tcfc2.ni %0, $vi29\n\t.set reorder" : "=r"(v));
    return v;
}
int func_003FEB08(void) {
    unsigned int vpuStat = vu0_cfc2_vi29_EB08();
    if ((vpuStat & 0xF) == 1)
        return 1;
    else if ((vpuStat & 0xF00) == 0x100)
        return 1;
    return 0;
}
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
void func_003FEB38(unsigned short addr, Q3EB38 value) {
    vu0_write_qword_EB38(addr, value.q);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FEB78);
#ifdef SKIP_ASM
// FAKEMATCH: `unsigned long vf` holds the saved 128-bit $vf1 between the two asm statements (only asm touches it; `Q3EB78_i`, the
// 128-bit type, swaps $2/$3); plain form 98.82%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
typedef int Q3EB78_i __attribute__((mode(TI)));
typedef union { Q3EB78_i q; unsigned long d[2]; int w[4]; short h[8]; } Q3EB78;
/* Read the qword at VU0 data address `addr` through $vi1/$vf1, restoring both. */
Q3EB78 func_003FEB78_impl(unsigned short addr) __asm__("func_003FEB78");
Q3EB78 func_003FEB78_impl(unsigned short addr) {
    unsigned int vi;
    unsigned long vf;
    Q3EB78_i rawData;
    Q3EB78 result;
    /* PORT: PS2-only VU0 macro-mode load (cfc2/qmfc2/ctc2/vlqi/qmfc2), then the $vi1/$vf1 restore (ctc2/qmtc2). */
    __asm__(".set noreorder\n\tcfc2.ni %0, $vi1\n\tqmfc2.ni %1, $vf1\n\tctc2.ni %3, $vi1\n\tvlqi.xyzw $vf1, ($vi1++)\n\tqmfc2.ni %2, $vf1\n\t.set reorder"
            : "=&r"(vi), "=&r"(vf), "=&r"(rawData) : "r"(addr));
    __asm__ volatile(".set noreorder\n\tctc2.ni %0, $vi1\n\tqmtc2.ni %1, $vf1\n\t.set reorder" : : "r"(vi), "r"(vf));
    result.q = rawData;
    return result;
}
#endif

//100%
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
void func_003FEBC0(void) {
    vu0_ctc2_vi28_BC0(vu0_cfc2_vi28_BC0() | 0x200);
}
#endif

//100%
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
int func_003FEBD8(void) {
    int result = 0;
    switch (func_003FED08_BD8()) {
    case 4:
        vu0_ctc2_vi28_BD8(vu0_cfc2_vi28_BD8() | 0x100);
    case 0:
    case 3:
        result = 1;
        break;
    case 1:
    case 2:
        break;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FEC38);
#ifdef SKIP_ASM
extern int func_003FED08(void);
extern unsigned short func_003FED60(void);
extern void func_003FED50(int);
int func_003FEC38(void) {
    int result = 0;
    int n = func_003FED08();
    if (n < 4) {
        if (n > 0) {
            func_003FED50(func_003FED60());
            result = 1;
        }
    }
    return result;
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
void func_003FEC88(int enable) {
    unsigned int ctrlBits = vu0_cfc2_C88();
    if (enable == 1)
        ctrlBits |= 0x400;
    else
        ctrlBits &= ~0x400;
    vu0_ctc2_C88(ctrlBits);
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
void func_003FECB8(int enable) {
    unsigned int ctrlBits = vu0_cfc2_CB8();
    if (enable == 1)
        ctrlBits |= 0x800;
    else
        ctrlBits &= ~0x800;
    vu0_ctc2_CB8(ctrlBits);
}
#endif

//100%
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
int func_003FECE8(void) {
    return (vu0_cfc2_CE8() >> 10) & 1;
}
#endif

//100%
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
int func_003FECF8(void) {
    return (vu0_cfc2_CF8() >> 11) & 1;
}
#endif

//100%
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
int func_003FED08(void) {
    unsigned int vpuStat = vu0_cfc2_vi29_ED08();
    int state;
    if (vpuStat & 0x200)
        state = 1;
    else if (vpuStat & 0x400)
        state = 2;
    else if (vpuStat & 0x800)
        state = 3;
    else if (vpuStat & 0x100)
        state = 4;
    else
        state = 0;
    return state;
}
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
void func_003FED50(int value) {
    vu0_ctc2_D50(value & 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FED60);
#ifdef SKIP_ASM
extern void func_003FEB78(void *, int);
unsigned short func_003FED60(void) {
    unsigned short value;
    func_003FEB78(&value, 0x43A);
    return value;
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
int func_003FED88_impl(S3ED88 *cnd) {
    unsigned i;
    if (e3ED88_a() != 0)
        return 0;
    for (i = 0; i < 0x20; i++) {
        cnd->a[i].q = e3ED88_b(0x400 + i).q;
    }
    cnd->b[0] = e3ED88_b(0x430).w[0];
    cnd->b[1] = e3ED88_b(0x431).w[0];
    cnd->b[2] = e3ED88_b(0x432).w[0];
    cnd->b[3] = e3ED88_b(0x434).w[0];
    cnd->b[4] = e3ED88_b(0x435).w[0];
    cnd->b[5] = e3ED88_b(0x436).w[0];
    cnd->b[6] = e3ED88_b(0x437).w[0];
    for (i = 0; i < 0x10; i++)
        cnd->c[i] = e3ED88_b(0x420 + i).h[0];
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
int func_003FEF18_impl(S3EF18 *cnd) {
    unsigned i;
    Q3EF18 value;
    if (e3EF18_a() != 0)
        return 0;
    for (i = 0; i < 0x20; i++) {
        value.q = cnd->a[i].q;
        e3EF18_b(0x400 + i, value);
    }
    value.q = 0;
    value.d[0] = cnd->b[0];
    e3EF18_b(0x430, value);
    value.d[0] = cnd->b[1];
    e3EF18_b(0x431, value);
    value.d[0] = cnd->b[2];
    e3EF18_b(0x432, value);
    value.d[0] = cnd->b[3];
    e3EF18_b(0x434, value);
    value.d[0] = cnd->b[4];
    e3EF18_b(0x435, value);
    value.d[0] = cnd->b[5];
    e3EF18_b(0x436, value);
    value.d[0] = cnd->b[6];
    e3EF18_b(0x437, value);
    for (i = 0; i < 0x10; i++) {
        value.d[0] = cnd->c[i];
        e3EF18_b(0x420 + i, value);
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
int func_003FF088(int mode) {
    int vifState = func_003FF160();
    if (vifState == 0) return 0;
    if (vifState == 1) return 0;
    if (mode == 0) {
        *(volatile int *)0x10003810 = 2;
    } else if (mode == 1) {
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
unsigned func_003FF130(unsigned maskInt, unsigned maskTagErr, unsigned maskCmdErr) {
    unsigned errReg = maskInt | (maskTagErr << 1) | (maskCmdErr << 2);
    *(volatile unsigned *)0x10003820 = errReg;
    return errReg;
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
    int vifStat = *(volatile int *)0x10003800;
    if (!(vifStat & 3)) {
        return 0;
    }
    if (vifStat & 0x700) {
        return 1;
    }
    if (vifStat & 0xC) {
        return 2;
    }
    return (vifStat & 0x3000) ? 3 : 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF1B0);
#ifdef SKIP_ASM
struct S3FF1B0 { int w[11]; short h[4]; char c[4]; };
extern int func_003FF160(void);
#define R(a) (*(volatile int *)(a))
int func_003FF1B0(struct S3FF1B0 *cnd) {
    unsigned cycleReg = *(volatile unsigned *)0x10003840;
    if ((unsigned)func_003FF160() >= 2)
        return 0;
    cnd->w[0] = R(0x10003900);
    cnd->w[1] = R(0x10003910);
    cnd->w[2] = R(0x10003920);
    cnd->w[3] = R(0x10003930);
    cnd->w[4] = R(0x10003940);
    cnd->w[5] = R(0x10003950);
    cnd->w[6] = R(0x10003960);
    cnd->w[7] = R(0x10003970);
    cnd->w[8] = R(0x10003870);
    cnd->w[9] = R(0x10003880);
    cnd->w[10] = R(0x10003800);
    cnd->h[0] = R(0x100038D0);
    cnd->h[1] = R(0x10003890);
    cnd->h[2] = R(0x10003830);
    cnd->h[3] = R(0x10003860);
    cnd->c[0] = R(0x10003820);
    cnd->c[1] = cycleReg;
    cnd->c[2] = cycleReg >> 8;
    cnd->c[3] = R(0x10003850);
    return 1;
}
#undef R
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF330);
#ifdef SKIP_ASM
typedef int i128 __attribute__((mode(TI)));
int func_003FF330(i128 *src, int n) {
    int sentCount = 0;
    while (n--) {
        if (*(volatile unsigned *)0x10003800 & 0x10000000)
            return sentCount;
        *(i128 *)0x10004000 = *src++;
        sentCount++;
    }
    return sentCount;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF390);
#ifdef SKIP_ASM
// FAKEMATCH: `dma[16]` fills the 0x100-byte local area of the retail frame; only dma[0] is used; plain form 99.83%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
/* SIF DMA transfer descriptor (the SDK's sceSifDmaData). */
typedef struct {
    unsigned int data;
    unsigned int addr;
    unsigned int size;
    unsigned int mode;
} sSifDmaData;
extern int sceSifDmaStat(int id);                       /* sceSifDmaStat */
extern int sceSifSetDma(sSifDmaData *sdd, int len);    /* sceSifSetDma */
extern void func_00424698(int *, int *);                /* SyncDCache */
extern void func_00425C60();                            /* printf */
extern char D_00497888[];
/* Send the slot's 0x20-byte actuator block to the IOP (into the half of its buffer picked by the block's counter). */
int func_003FF390(int port, int slot) {
    sSifDmaData dma[16];
    int *act;
    int dst;
    int id;
    int n;
    act = (int *)D_00526250[port][slot].actBuf;
    if (D_00526250[port][slot].f0C != 0 && sceSifDmaStat(D_00526250[port][slot].f0C) >= 0) {
        if (D_004515CC != 0)
            func_00425C60(D_00497888);
        return 0;
    }
    n = act[0] + 1;
    dst = D_00526250[port][slot].f08 + ((n & 1) << 5);
    act[0] = n;
    func_00424698(act, (int *)((char *)act + 0x1F));
    dma[0].addr = dst;
    dma[0].data = (unsigned int)act;
    dma[0].size = 0x20;
    dma[0].mode = 0;
    id = sceSifSetDma(dma, 1);
    if (id == 0) {
        if (D_004515CC != 0)
            func_00425C60(D_00497888);
        return 0;
    }
    D_00526250[port][slot].f0C = id;
    return 1;
}
#endif

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
extern void iSignalSema(int);
void func_00400810(int alarmId, int ticks, int sema) {
    iSignalSema(sema);
    SYNC_0810();
    EI_0810();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400838);
#ifdef SKIP_ASM
extern int CreateSema(int *);
extern void func_00400810(int, int, int);
void func_00400838(unsigned short ticks) {
    int semaParam[6];
    int sema;
    semaParam[1] = 1;
    semaParam[2] = 0;
    semaParam[5] = 0;
    sema = CreateSema(semaParam);
    SetAlarm(ticks, func_00400810, sema);
    WaitSema(sema);
    DeleteSema(sema);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004008A0);
#ifdef SKIP_ASM
extern int func_004013E8(int);
extern int func_0042C078();
extern void func_0042C0C0(void);
void (*func_004008A0(void (*cb)(int)))(int) {
    void (*old)(int);
    int s;
    if (func_004013E8(1) != 0)
        return 0;
    s = func_0042C078();
    old = D_005264C0;
    D_005264C0 = cb;
    __asm__ volatile("sw $28,%0" : "=m"(D_005264C4)); /* the callback runs with this $gp */
    if (s != 0)
        func_0042C0C0();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400908);
#ifdef SKIP_ASM
extern int iSignalSema_i(int) __asm__("iSignalSema");
void func_00400908(int *p) {
    D_00451658 = *p;
    D_0045165C = D_00451658;
    if (D_00451658 == 11) {
        D_00451658 = 0;
        D_00451634 = 0;
        return;
    }
    iSignalSema_i(D_00451628);
    if (D_00451614 != 0 && D_005264C0 != 0)
        iSignalSema_i(D_00451620);
    else
        D_00451634 = 0;
    D_00451658 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004009A8);
#ifdef SKIP_ASM
extern char D_004979E0[];
extern void WaitSema(int);
extern int ExitDeleteThread(void);
extern void func_00425C60();
void func_004009A8(void) {
    void (*fn)(int);
    for (;;) {
        WaitSema(D_00451620);
        if (D_00451658 == -1) {
            D_00451634 = 0;
            D_00451658 = 0;
            D_00451614 = 0;
            D_005264D4 = 0;
            ExitDeleteThread();
        }
        if (D_00451610 > 0)
            func_00425C60(D_004979E0, D_005264C0, D_0045165C);
        fn = D_005264C0;
        if (fn != 0 && D_0045165C != 0) {
            int saved;
            __asm__ volatile("move %0,$28\n\tlw $28,%1" : "=r"(saved) : "m"(D_005264C4));
            fn(D_0045165C);
            __asm__ volatile("move $28,%0" : : "r"(saved));
        }
        D_00451634 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400A78);
#ifdef SKIP_ASM
extern int GetThreadId();
extern int ReferThreadStatus_i(int, sThreadParam *) __asm__("ReferThreadStatus");
extern int CreateThread(sThreadParam *);
extern void StartThread(int, int);
extern void ChangeThreadPriority(int, int);
extern void func_004009A8(void);
int func_00400A78(int a, int b, int c) {
    int ok;
    ok = 1;
    if (D_00451614 == 0) {
        D_005264D8 = GetThreadId();
        ReferThreadStatus_i(D_005264D8, &D_005264E0);
        D_00526510.stackSize = c;
        D_00526510.entry = func_004009A8;
        D_00526510.stack = b;
        D_00526510.initPriority = a;
        D_00526510.gpReg = 0;
        D_00451614 = CreateThread(&D_00526510);
        StartThread(D_00451614, 0);
    } else {
        ChangeThreadPriority(D_00451614, a);
        ok = 0;
    }
    return ok;
}
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
extern int Q0C00_create(struct QSema *) __asm__("CreateSema");
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
extern int cb_DB0(int) __asm__("DeleteSema");
extern void cb_DC0(int) __asm__("SignalSema");
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400D58);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
extern int func_00400E30(void);
void (*func_00400D58(void (*a)(int), int b))(int) {
    int s;
    void (*old)(int);
    if (D_00451640 < 0)
        func_00400E30();
    s = func_0042C078();
    old = D_005264C8;
    D_005264D0 = b;
    D_005264C8 = a;
    __asm__ volatile("sw $28,%0" : "=m"(D_005264CC));
    if (s)
        func_0042C0C0();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400DD8);
#ifdef SKIP_ASM
void func_00400DD8(void) {
    void (*fn)(int) = D_005264C8;
    if (fn) {
        if (D_00451624 == 0) {
            int saved;
            __asm__ volatile("move %0,$28\n\tlw $28,%1" : "=r"(saved) : "m"(D_005264CC));
            fn(D_005264D0);
            __asm__ volatile("move $28,%0" : : "r"(saved));
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400E30);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
extern void func_00400DD8(void);
extern void func_00426020(int, void (*)(void), int);
int func_00400E30(void) {
    int s;
    D_00451624 = 1;
    s = func_0042C078();
    func_00426020(0x80000012, func_00400DD8, 0);
    if (s)
        func_0042C0C0();
    D_00451624 = 0;
    D_00451640 = 1;
    return 1;
}
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
extern int PollSema(int);
extern void SignalSema(int);
extern void ReferThreadStatus(int, sThreadParam *);
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
    if (D_00451628 != PollSema(D_00451628))
        return 0;
    D_0045161C = 1;
    ReferThreadStatus(D_005264D8, &D_005264E0);
    if (func_004013E8(1) != 0) {
        SignalSema(D_00451628);
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
        SignalSema(D_00451628);
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
    SignalSema(D_00451628);
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
extern int PollSema(int);
extern void SignalSema(int);
extern void ReferThreadStatus(int, sThreadParam *);
extern void func_00425C60();
extern int func_004013E8(int);
extern void func_00426408(int);
int func_004011E0(int a) {
    int i;
    func_00400C00();
    if (D_00451628 != PollSema(D_00451628)) {
        if (D_00451610 > 0)
            func_00425C60(D_00497A78, a, D_0045161C);
        return 0;
    }
    D_0045161C = a;
    ReferThreadStatus(D_005264D8, &D_005264E0);
    if (func_004013E8(1) != 0) {
        SignalSema(D_00451628);
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
extern void SignalSema(int);
int func_00401350(void) {
    int v;
    if (func_004011E0(2) == 0)
        return 0;
    if (func_00426D18(&D_004527D0, 14, 0, 0, 0, D_00451680, 4, 0, 0) < 0) {
        SignalSema(D_00451628);
        return 0;
    }
    v = *(int *)((unsigned)D_00451680 | 0x20000000);
    SignalSema(D_00451628);
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
extern int PollSema(int);
extern void SignalSema(int);
extern void ReferThreadStatus(int, sThreadParam *);
extern void func_00425C60();
extern int func_00401488(int);
extern void func_00426408(int);
int func_004014F8(int a) {
    int i;
    func_00400C00();
    if (D_0045162C != PollSema(D_0045162C)) {
        if (D_00451610 > 0)
            func_00425C60(D_00497AD8, a, D_00451618);
        return 0;
    }
    D_00451618 = a;
    ReferThreadStatus(D_005264D8, &D_005264E0);
    if (func_00401488(1) != 0) {
        SignalSema(D_0045162C);
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401718);
#ifdef SKIP_ASM
// Has the shape of libcdvd's sceCdInit(mode): binds RPC server 0x80000592, sends the mode, reads the status words
// back uncached; mode 5 (SCECdEXIT) tears the client down. Own match (agent fp401718, 2026-11-15); D_00451624 is
// volatile at file scope (phaseb_import.py VIEW_SUBS).
extern int D_00451650;
extern int D_00451654;
extern int D_00526740;
extern sSifClient D_005266E8;
extern char D_00497B18[];
extern char D_00497B38[];
extern int func_00401488(int);
extern void func_00426408(int);
extern int GetThreadId();
extern void func_00425C60();
extern void func_00426358(int, int);

int func_00401718(int mode)
{
    int i;
    int r;
    int ret;
    int st, a, b;
    int *buf;

    if (func_00401488(1) != 0)
        return 0;
    func_00426408(0);
    D_005264D8 = GetThreadId();
    D_00451624 = 1;
    D_00451644 = -1;
    D_0045163C = -1;
    D_0045164C = -1;
    D_00451648 = -1;
    D_00451650 = -1;
    D_00451640 = -1;
    D_00451638 = 0;
    D_00451654++;
    while (1) {
        if ((r = func_00426B48(&D_005266E8, 0x80000592, 0)) < 0) {
            if (D_00451610 > 0)
                func_00425C60(D_00497B18, r, D_00451654);
            for (i = 0x100000; i--; )
                ;
            continue;
        }
        if (D_005266E8.server != 0) {
            D_00526740 = mode;
            D_00451650 = 0;
            func_00426358((int)&D_00526740, 4);
            buf = D_00452800;
            if (func_00426D18(&D_005266E8, 0, 0, &D_00526740, 4, buf, 0x10, 0, 0) < 0) {
                D_00451624 = 0;
                return 0;
            }
            break;
        }
        for (i = 0x100000; i--; )
            ;
    }
    ret = 1;
    a = *(int *)((unsigned)(buf + 1) | 0x20000000);
    b = *(int *)((unsigned)(buf + 2) | 0x20000000);
    st = *(int *)((unsigned)(buf + 3) | 0x20000000);
    if (st != 0xFF) {
        if (st == 0xFE)
            D_00451610 = 1;
        else if (a / 256 < 2 || b / 256 < 2)
            ret = 2;
    }
    D_00451624 = 0;
    switch (mode) {
    case 5:
        if (D_00451610 > 0)
            func_00425C60(D_00497B38);
        func_00400CB8();
        D_00451628 = -1;
        D_0045162C = -1;
        D_00451620 = -1;
        break;
    case 0:
    case 1:
    default:
        func_00400C00();
        func_00400E30();
        break;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004019F8);
#ifdef SKIP_ASM
extern char D_00497B48[];
extern char D_00497B60[];
extern char D_00497B80[];
extern void func_00425C60();
extern void func_00400C00();
extern int PollSema(int);
extern void SignalSema(int);
extern int func_00401488(int);
extern void func_00426408(int);
extern void func_00426358(int, int);
int func_004019F8(int a) {
    int r;
    int v;
    if (D_00451610 > 0)
        func_00425C60(D_00497B48);
    func_00400C00();
    if (D_0045162C != PollSema(D_0045162C))
        return 6;
    if (func_00401488(1) != 0) {
        SignalSema(D_0045162C);
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
        SignalSema(D_0045162C);
        return a != 8 ? 6 : -1;
    }
    if (D_00451610 > 0)
        func_00425C60(D_00497B80);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    SignalSema(D_0045162C);
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
extern int b1_3DF0(int) __asm__("PollSema");
extern void b1_3DC0(int) __asm__("SignalSema");
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
extern void SignalSema(int);
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
        SignalSema(D_00451628);
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
extern void SignalSema(int);
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
        SignalSema(D_00451628);
        return 0;
    }
    D_00451658 = 0;
    v = D_00451680[0];
    if (D_00451680[1] == 0)
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 0x400);
    else
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 0x810);
    SignalSema(D_00451628);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402268);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void SignalSema(int);
int func_00402268(void) {
    int v;
    if (func_004014F8(1) == 0)
        return 0;
    if (func_00426D18(&D_00453180, 3, 0, 0, 0, D_00452800, 4, 0, 0) < 0) {
        SignalSema(D_0045162C);
        return 0;
    }
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    SignalSema(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402300);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void SignalSema(int);
int func_00402300(void) {
    int v;
    if (func_004014F8(3) == 0)
        return -1;
    if (func_00426D18(&D_00453180, 4, 0, 0, 0, D_00452800, 4, 0, 0) < 0) {
        SignalSema(D_0045162C);
        return -1;
    }
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    SignalSema(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402398);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void SignalSema(int);
int func_00402398(int *out) {
    int v;
    if (func_004014F8(0x21) == 0)
        return 0;
    if (func_00426D18(&D_00453180, 0x21, 0, 0, 0, D_00452800, 8, 0, 0) < 0) {
        SignalSema(D_0045162C);
        return 0;
    }
    *out = *(int *)((unsigned)(D_00452800 + 1) | 0x20000000);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    SignalSema(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402450);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void SignalSema(int);
extern void func_00426358_p(int *, int) __asm__("func_00426358");
int func_00402450(int a) {
    int v;
    int *p = D_00452C40;
    if (func_004014F8(34) == 0)
        return 0;
    D_00452C40[0] = a;
    func_00426358_p(p, 4);
    if (func_00426D18(&D_00453180, D_004531A8, 0, p, D_004531AC, D_00452800, D_004531B0, 0, 0) < 0) {
        SignalSema(D_0045162C);
        return 0;
    }
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    SignalSema(D_0045162C);
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
extern void SignalSema(int);
extern void func_00425C60();
int func_00402520(struct P8 *a) {
    int v;
    if (func_004014F8(15) == 0)
        return 0;
    if (D_00451610 > 0)
        func_00425C60(D_00497BD8);
    if (func_00426D18(&D_00453180, 1, 0, 0, 0, D_00452800, 16, 0, 0) < 0) {
        SignalSema(D_0045162C);
        return 0;
    }
    *a = *(struct P8 *)((unsigned)&D_00452800[1] | 0x20000000);
    if (D_00451610 > 0)
        func_00425C60(D_00497BF8);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    SignalSema(D_0045162C);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402618);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void SignalSema(int);
extern void func_00426358_p(int *, int) __asm__("func_00426358");
int func_00402618(int a, int *out) {
    int v;
    int *p = D_00452C40;
    if (func_004014F8(26) == 0)
        return -1;
    D_00452C40[0] = a;
    func_00426358_p(p, 4);
    if (func_00426D18(&D_00453180, 5, 0, p, 4, D_00452800, 8, 0, 0) < 0) {
        SignalSema(D_0045162C);
        return -1;
    }
    if (out)
        *out = *(int *)((unsigned)&D_00452800[1] | 0x20000000);
    v = *(int *)((unsigned)D_00452800 | 0x20000000);
    SignalSema(D_0045162C);
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
    int intrState = func_0042C078();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 &= 0xFFFFFEFF;
    *(volatile int *)0x1000B400 &= 0xFFFFFEFF;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
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
int func_004029D0(int *mpeg, int dataAddr, int n) {
    func_00403500(mpeg[16], dataAddr, (n + 19) / 16 * 16);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402A10);
#ifdef SKIP_ASM
extern int func_00402E10(int *);
int func_00402A10(int *mpeg, unsigned picture, int mbCount) {
    int *mpegSys = (int *)mpeg[16];
    picture = (picture & 0xFFFFFFF) | 0x20000000;
    mpegSys[44] = 1;
    mpegSys[54] = picture;
    mpegSys[57] = mbCount;
    mpegSys[56] = 0;
    mpegSys[55] = 0;
    return func_00402E10(mpeg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402A58);
#ifdef SKIP_ASM
extern int func_00402E10(int *);
int func_00402A58(int *mpeg, unsigned picture, int mbCount) {
    int *mpegSys = (int *)mpeg[16];
    picture = (picture & 0xFFFFFFF) | 0x20000000;
    mpegSys[57] = mbCount;
    mpegSys[54] = picture;
    mpegSys[44] = 0;
    mpegSys[56] = 0;
    mpegSys[55] = 0;
    return func_00402E10(mpeg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402AA0);
#ifdef SKIP_ASM
extern int func_00402E10(int *);
int func_00402AA0(int *mpeg, unsigned picture, int mbWidth, int mbHeight) {
    int *mpegSys = (int *)mpeg[16];
    picture = (picture & 0xFFFFFFF) | 0x20000000;
    mpegSys[56] = mbHeight * 16;
    mpegSys[54] = picture;
    mpegSys[57] = mbWidth * mbHeight;
    mpegSys[55] = mbWidth * 16;
    mpegSys[44] = 0;
    return func_00402E10(mpeg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402AF0);
#ifdef SKIP_ASM
void func_00402AF0(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[0x428] = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B00);
#ifdef SKIP_ASM
void func_00402B00(int *mpeg, int a, int b, int c) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[37] = a;
    mpegSys[38] = b;
    mpegSys[39] = c;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B18);
#ifdef SKIP_ASM
void func_00402B18(int *mpeg, int *a, int *b, int *c) {
    int *mpegSys = (int *)mpeg[16];
    *a = mpegSys[37];
    *b = mpegSys[38];
    *c = mpegSys[39];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B38);
#ifdef SKIP_ASM
int func_00402B38(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B48);
#ifdef SKIP_ASM
int func_00402B48(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[1] == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B58);
#ifdef SKIP_ASM
extern void func_00403398(int *);
int func_00402B58(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[0] = 0;
    mpegSys[1] = 0;
    mpegSys[2] = 0;
    mpeg[2] = 0;
    mpegSys[43] = 0;
    mpegSys[32] = -1;
    func_00403398(mpegSys);
    mpegSys[70] = 0;
    func_004032A8(mpegSys);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402BB0);
#ifdef SKIP_ASM
int func_00402BB0(int *mpeg) {
    char *mpegSys = (char *)mpeg[16];
    int *t;
    if ((t = *(int **)(mpegSys + 0x1B8))) t[10] = 0;
    if ((t = *(int **)(mpegSys + 0x1C8))) t[10] = 0;
    if ((t = *(int **)(mpegSys + 0x1D8))) t[10] = 0;
    if ((t = *(int **)(mpegSys + 0x1BC))) t[10] = 0;
    if ((t = *(int **)(mpegSys + 0x1CC))) t[10] = 0;
    if ((t = *(int **)(mpegSys + 0x1DC))) t[10] = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402C08);
#ifdef SKIP_ASM
int func_00402C08(int *mpeg, int n, int fn, void *cbData) {
    char *mpegSys = (char *)mpeg[16];
    char *tbl = mpegSys + 0xC;
    char *ea = mpegSys + n * 8;
    char *eb = tbl + n * 8;
    int old;
    old = *(int *)eb;
    *(int *)eb = fn;
    *(void **)(ea + 0x10) = cbData;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402C30);
#ifdef SKIP_ASM
struct FA_402C30 { int (*fn)(int *, int *, int); int arg; };
struct T_402C30 { char pad[12]; struct FA_402C30 e[1]; };
int func_00402C30(int *mpeg, int *cbData) {
    int result = 0;
    if (mpeg) {
        struct T_402C30 *mpegSys = (struct T_402C30 *)mpeg[16];
        if (mpegSys) {
            if (mpegSys->e[*cbData].fn) {
                result = mpegSys->e[*cbData].fn(mpeg, cbData, mpegSys->e[*cbData].arg);
            }
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402C80);
#ifdef SKIP_ASM
extern int func_00402C30(int *, int *);
int func_00402C80(int *mpeg) {
    int cbData[8];
    cbData[0] = 1;
    func_00402C30(mpeg, cbData);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CA8);
#ifdef SKIP_ASM
int func_00402CA8(int *mpeg, long v) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[28] = 1;
    *(long *)&mpegSys[30] = v;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CC0);
#ifdef SKIP_ASM
int func_00402CC0(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[28] = 0;
    *(long *)&mpegSys[30] = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CD8);
#ifdef SKIP_ASM
int func_00402CD8(int *mpeg, int v) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[54] = v;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CE8);
#ifdef SKIP_ASM
int func_00402CE8(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[51];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CF8);
#ifdef SKIP_ASM
int func_00402CF8(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[52];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D08);
#ifdef SKIP_ASM
char *func_00402D08(int *mpeg) {
    return (char *)mpeg[16] + 0xB4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D18);
#ifdef SKIP_ASM
char *func_00402D18(int *mpeg) {
    return (char *)mpeg[16] + 0xB4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D28);
#ifdef SKIP_ASM
int func_00402D28(int *mpeg, int newValue) {
    int *mpegSys = (int *)mpeg[16];
    int old = mpegSys[58];
    mpegSys[58] = newValue;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D38);
#ifdef SKIP_ASM
int func_00402D38(int *mpeg, long v) {
    int *mpegSys = (int *)mpeg[16];
    *(long *)&mpegSys[60] = v;
    mpegSys[62] = 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D50);
#ifdef SKIP_ASM
void func_00402D50(int *heap, int base, int size) {
    heap[0] = base;
    heap[1] = size;
    heap[2] = base;
    heap[3] = base;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D68);
#ifdef SKIP_ASM
int func_00402D68(int *heap) {
    return heap[3] = heap[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D78);
#ifdef SKIP_ASM
int func_00402D78(int *heap) {
    return heap[2] = heap[3];
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
int func_00402DF8(int *heap) {
    return heap[0] + heap[1] - heap[2];
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402F80);
#ifdef SKIP_ASM
struct P_2F80 { char pad[4]; int f4; int f8; char padC[0xA0]; int fAC; char pad2[0x68]; int f118; char pad3[4]; int f120; char pad4[0x50]; int f174; };
struct A_2F80 { char pad[8]; int f8; char pad2[0x34]; struct P_2F80 *f40; };
extern int s2_7F20(struct P_2F80 *, int) __asm__("func_00407F20");
extern int s2_7DC0(struct P_2F80 *) __asm__("func_00407DC0");
extern void s2_2C80(struct A_2F80 *) __asm__("func_00402C80");
extern void s2_7E90(struct P_2F80 *, int, int) __asm__("func_00407E90");
int func_00402F80(struct A_2F80 *a, int b, int c) {
    struct P_2F80 *p = a->f40;
    int t, ret;
    int flag = 0;
    if (c == -1 || b < c) {
        if (p->f8 == 0) {
            a->f8 = 0;
            p->f8 = 1;
        }
        t = s2_7F20(p, 0) ? s2_7DC0(p) != 0 : 0;
        ret = t;
    } else {
        ret = s2_7F20(p, 0);
        flag = 1;
        s2_2C80(a);
    }
    s2_7E90(p, p->f118, p->f4);
    if ((t = p->f174) != 3 && flag == 0)
        p->f120 = p->f120 == 0;
    a->f8 = p->f118 - p->fAC;
    if (p->f120 == 0) {
        p->f118++;
        p->f4++;
    }
    return ret;
}
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

//100%
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
int func_004030E0(struct A_30E0 *a, int b, int c) {
    struct P_30E0 *p = a->f40;
    int flag = 0;
    int ret, k, t;
    p->f120 = 0;
    if (c == -1 || b < c)
        flag = 1;
    if (p->f8 == 0) {
        a->f8 = 0;
        p->f8 = 1;
    }
    if (s3_7F20(p, 0)) {
        if (flag)
            s3_7DC0(p);
    }
    p->f120 = 1;
    if (s3_7520(p) == 0) {
        s3_3240(a);
        p->f0 = 1;
        return 0;
    }
    k = 2;
    if (p->fD4 != 1)
        k = 1;
    if (p->f174 != k)
        return -1;
    ret = 0;
    t = 0;
    if (s3_7F20(p, 1))
        t = 1;
    if (t && flag) {
        if (s3_7DC0(p))
            ret = 1;
    }
    s3_7E90(p, p->f118, p->f4);
    p->f120 = 0;
    a->f8 = p->f118 - p->fAC;
    p->f118++;
    p->f4++;
    if (!flag)
        s3_2C80(a);
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403240);
#ifdef SKIP_ASM
extern void func_004032B8(int *);
int func_00403240(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    int result = 0;
    if (mpegSys[1] != 0 && mpegSys[2] != 0) {
        func_004032B8(mpegSys);
        mpeg[2] = mpegSys[70] - mpegSys[43];
        mpegSys[1] = 0;
        result = 1;
    }
    return result;
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
void func_00403338(int *mpegSys) {
    int scratchBase = 0x70000000;
    func_00405B48(1);
    mpegSys[356] = scratchBase;
    mpegSys[357] = 0x70001800;
    mpegSys[436] = 0x70001B00;
    mpegSys[437] = 0x70003300;
    mpegSys[516] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403398);
#ifdef SKIP_ASM
extern int func_0042C078_0() __asm__("func_0042C078");
extern void func_0042C0C0(void);
extern void func_00409638_2(int, int) __asm__("func_00409638");
void func_00403398(int *mpegSys) {
    int intrState;
    mpegSys[0x818 / 4] = 1;
    mpegSys[0x1B0 / 4] = 0;
    intrState = func_0042C078_0();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 = 0;
    *(volatile int *)0x1000B400 = 0;
    *(volatile int *)0x1000D400 = 0;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        func_0042C0C0();
    *(volatile int *)0x1000B020 = 0;
    *(volatile int *)0x1000B420 = 0;
    *(volatile int *)0x1000D420 = 0;
    *(volatile int *)0x10002010 = 0x40000000;
    func_00409638_2(0, 0);
}
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
void func_00403470(int *mpegSys, char *fmt) {
    char buf[256];
    sprintf(buf, fmt);
    func_004034A8(mpegSys, (int)buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004034A8);
#ifdef SKIP_ASM
extern int func_00402C30(int *, int *);
extern void func_00403460(int);
void func_004034A8(int *mpegSys, int message) {
    int cbData[2];
    int *mpeg = (int *)mpegSys[534];
    if (mpeg && mpegSys && mpegSys[3]) {
        cbData[0] = 0;
        cbData[1] = message;
        func_00402C30(mpeg, cbData);
        return;
    }
    func_00403460(message);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403500);
#ifdef SKIP_ASM
extern void FlushCache(int) __asm__("FlushCache");
extern int func_0042C078();
extern void func_0042C0C0(void);
void q3500(int base, unsigned addr, int n) __asm__("func_00403500");
void q3500(int base, unsigned addr, int n) {
    unsigned tagBase = ((unsigned)(base + 0x88F) >> 4) << 4;
    long *tag = (long *)tagBase;
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
    FlushCache(0);
    ret = func_0042C078();
    *(volatile int *)0x1000B430 = tagBase & 0xFFFFFFF;
    *(volatile int *)0x1000B420 = 0;
    *(volatile int *)0x1000B400 = 0x105;
    if (ret)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403600);
#ifdef SKIP_ASM
int func_00403600(int *picInfo, int width, int height) {
    picInfo[1] = width;
    picInfo[2] = height;
    picInfo[3] = width >> 4;
    picInfo[4] = height >> 4;
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004039F0);
#ifdef SKIP_ASM
static __inline__ int unc(int a) { return (a & 0x0FFFFFFF) | 0x20000000; }
static __inline__ int adr(int base, int n) { return unc(base + n * 0x180); }
void func_004039F0(int *y0, int *uOut0, int *vOut0, int *y1, int *uOut1, int *vOut1, int *y2, int *uOut2, int *vOut2,
                   int y, int uBuffer, int vBuffer, int w, int height) {
    int n;
    *y0 = unc(y);
    *uOut0 = unc(uBuffer);
    *vOut0 = unc(vBuffer);
    *y1 = unc(y);
    *uOut1 = unc(uBuffer);
    *vOut1 = unc(vBuffer);
    n = w * height / 512;
    *y2 = adr(y, n);
    *uOut2 = adr(uBuffer, n);
    *vOut2 = adr(vBuffer, n);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403AD0);
#ifdef SKIP_ASM
extern int f3AD0_DI() __asm__("func_0042C078");
extern void f3AD0_EI(void) __asm__("func_0042C0C0");
extern int f3AD0_2C30(int, int *) __asm__("func_00402C30");
extern void f3AD0_6E20(int *) __asm__("func_00406E20");
extern void f3AD0_6DF0(int *, int) __asm__("func_00406DF0");
void func_00403AD0(int *mpegSys, int ipuCmd, int srcAddr) {
    int cbData[8];
    int intrState;
    cbData[0] = 2;
    f3AD0_2C30(mpegSys[534], cbData);
    f3AD0_6E20(mpegSys);
    *(volatile int *)0x10002000 = 0;
    f3AD0_6E20(mpegSys);
    intrState = f3AD0_DI();
    *(volatile int *)0x1000B410 = srcAddr & 0xFFFFFFF;
    *(volatile int *)0x1000B420 = 4;
    *(volatile int *)0x1000B400 = 0x101;
    if (intrState) f3AD0_EI();
    f3AD0_6DF0(mpegSys, ipuCmd);
    f3AD0_6E20(mpegSys);
    cbData[0] = 3;
    f3AD0_2C30(mpegSys[534], cbData);
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
void func_00403CE0(int *mpegSys) {
    func_004072E8((int)mpegSys, 3);
    if (func_004072E8((int)mpegSys, 1)) {
        func_004072E8((int)mpegSys, 8);
        func_004072E8((int)mpegSys, 8);
        mpegSys[0x144 / 4] = func_004072E8((int)mpegSys, 8);
    }
    mpegSys[0x148 / 4] = func_004072E8((int)mpegSys, 0xE);
    func_004072E8((int)mpegSys, 1);
    mpegSys[0x14C / 4] = func_004072E8((int)mpegSys, 0xE);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _sequenceScalableExtension);
#ifdef SKIP_ASM
extern char D_00497D58[];
extern void func_004034A8(int *, int);
/**
 * Reports that MPEG-2 sequence scalable extensions are not supported.
 *
 * Passes "_sequenceScalableExtension() is not implemented" to func_004034A8(), which hands it to the decoder's
 * error callback when one is set and prints it otherwise.
 *
 * @param self  the MPEG decoder state
 */
void _sequenceScalableExtension(int *self) {
    func_004034A8(self, (int)D_00497D58);
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
int func_00403DB0(int *mpeg) {
    func_00409400((char *)mpeg[16] + 0x4C);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403DD8);
#ifdef SKIP_ASM
extern void func_004094E8(void *);
int func_00403DD8(int *mpeg) {
    func_004094E8((char *)mpeg[16] + 0x4C);
    return 1;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00403E00);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00404050);
#ifdef SKIP_ASM
/* MSSG mpeg2decode recon.c form_predictions(), as compiled into Sony's libmpeg */
// FAKEMATCH: `sel = 1` at entry and again before the backward frame-picture branch, with `sel` passed where the frame-picture
// calls mean the constant 1 (retail keeps that 1 in sel's $19); plain form (literal 1s) 97.19%. Byte-identical and behaves like the
// plain form; landed marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest
// spelling matches.
extern char D_00497E38[];
extern char D_00497E58[];
extern char D_00497E78[];
extern void q4050_4758(int *, int, int, int, int, int, int, int, int, int, int, int) __asm__("func_00404758");
extern void q4050_5E30(int *, int *, int *, int, int) __asm__("func_00405E30");
extern void q4050_3470(int *, char *, int) __asm__("func_00403470");

void func_00404050(int *p, int bx, int by, int mbtype, int mtype, int PMV[2][2][2], int mvfs[2][2], int *dmv)
{
    int DMV[2][2];
    int ref[2][2];
    int cur, sel, avg, fwd;

    sel = 1;
    avg = 0;
    *(int *)((char *)p + p[0x810 / 4] * 0x140 + 0x6BC) = 0;
    fwd = mbtype & 8;
    if (fwd || p[0x150 / 4] == 2) {
        if (p[0x174 / 4] == 3) {
            if (mtype == 2 || !fwd) {
                q4050_4758(p, p[0x1B8 / 4], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
            } else if (mtype == 1) {
                q4050_4758(p, p[0x1B8 / 4], mvfs[0][0], 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, 1, 0);
                q4050_4758(p, p[0x1B8 / 4], mvfs[1][0], 1, 0, 8, bx, by, PMV[1][0][0], PMV[1][0][1] >> 1, 1, 0);
            } else if (mtype == 3) {
                q4050_5E30(p, &DMV[0][0], dmv, PMV[0][0][0], PMV[0][0][1] >> 1);
                q4050_4758(p, p[0x1B8 / 4], 0, 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, sel, 0);
                q4050_4758(p, p[0x1B8 / 4], 1, 0, 0, 8, bx, by, DMV[0][0], DMV[0][1], sel, sel);
                q4050_4758(p, p[0x1B8 / 4], 1, 1, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, sel, 0);
                q4050_4758(p, p[0x1B8 / 4], 0, 1, 0, 8, bx, by, DMV[1][0], DMV[1][1], sel, sel);
            } else {
                q4050_3470(p, D_00497E38, mtype);
            }
        } else {
            cur = p[0x174 / 4] == 2;
            ref[0][0] = p[0x1C8 / 4];
            ref[0][1] = p[0x1D8 / 4];
            ref[1][0] = p[0x1CC / 4];
            ref[1][1] = p[0x1DC / 4];
            if (p[0x150 / 4] == 2 && p[0x120 / 4] && cur != mvfs[0][0])
                sel = 1;
            else
                sel = 0;
            if (mtype == 1 || !fwd) {
                q4050_4758(p, ref[sel][mvfs[0][0]], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
            } else if (mtype == 2) {
                q4050_4758(p, ref[sel][mvfs[0][0]], 0, 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
                if (p[0x150 / 4] == 2 && p[0x120 / 4] && cur != mvfs[1][0])
                    sel = 1;
                else
                    sel = 0;
                q4050_4758(p, ref[sel][mvfs[1][0]], 0, 0, 8, 8, bx, by, PMV[1][0][0], PMV[1][0][1], 0, 0);
            } else if (mtype == 3) {
                if (p[0x120 / 4])
                    sel = 1;
                else
                    sel = 0;
                q4050_5E30(p, &DMV[0][0], dmv, PMV[0][0][0], PMV[0][0][1]);
                q4050_4758(p, ref[0][cur], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
                q4050_4758(p, ref[sel][!cur], 0, 0, 0, 16, bx, by, DMV[0][0], DMV[0][1], 0, 1);
            } else {
                q4050_3470(p, D_00497E58, mtype);
            }
        }
        avg = 1;
    }
    if (mbtype & 4) {
        if (p[0x174 / 4] == 3) {
            sel = 1;
            if (mtype == 2) {
                q4050_4758(p, p[0x1BC / 4], 0, 0, 0, 16, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
            } else {
                q4050_4758(p, p[0x1BC / 4], mvfs[0][1], 0, 0, 8, bx, by, PMV[0][1][0], PMV[0][1][1] >> 1, sel, avg);
                q4050_4758(p, p[0x1BC / 4], mvfs[1][1], 1, 0, 8, bx, by, PMV[1][1][0], PMV[1][1][1] >> 1, sel, avg);
            }
        } else {
            if (mtype == 1) {
                q4050_4758(p, mvfs[0][1] ? p[0x1DC / 4] : p[0x1CC / 4], 0, 0, 0, 16, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
            } else if (mtype == 2) {
                q4050_4758(p, mvfs[0][1] ? p[0x1DC / 4] : p[0x1CC / 4], 0, 0, 0, 8, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
                q4050_4758(p, mvfs[1][1] ? p[0x1DC / 4] : p[0x1CC / 4], 0, 0, 8, 8, bx, by, PMV[1][1][0], PMV[1][1][1], 0, avg);
            } else {
                q4050_3470(p, D_00497E78, mtype);
            }
        }
    }
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00404758);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00404B78);
#ifdef SKIP_ASM
struct B4B78 { char b[0x1C]; };
struct E4B78 {
    int f0;
    void *src;
    char pad8[0x20];
    void (*fa[4])(void *);
    void (*fb[4])(void *);
    struct B4B78 blk[8];
    void *dst;
    int count;
    int f130;
    int pad134;
    int enable;
    int f13C;
};
struct S4B78 { char pad0[0x590]; struct E4B78 e[2]; char pad810[0xC]; void *f81C; };
extern void func_004034A8(int *, int);
extern char D_00497E98[];
extern void add_4B78(void *, void *) __asm__("func_00405AE8");
extern void mix_4B78(void *, void *, void *) __asm__("func_00405A88");
void f4B78(struct S4B78 *s, int n) __asm__("func_00404B78");
void f4B78(struct S4B78 *s, int n) {
    int i;
    if (s->e[n].enable != 0) {
        for (i = 0; i < s->e[n].count; i++) {
            s->e[n].fa[i](&s->e[n].blk[i]);
            s->e[n].fb[i](&s->e[n].blk[i + 4]);
        }
    }
    if (s->e[n].f130 != 0 && s->e[n].f13C != 0)
        func_004034A8((int *)s, (int)D_00497E98);
    if (s->e[n].f130 != 0)
        add_4B78(s->e[n].dst, s->e[n].src);
    else if (s->e[n].f13C != 0)
        add_4B78(s->e[n].dst, s->f81C);
    else
        mix_4B78(s->e[n].dst, s->f81C, s->e[n].src);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00404D98);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404D98(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "sll $11, $12, 1\n"
        "addiu $15, $0, -0x1\n"
        "mtsab $13, 0x0\n"
        "1:\n"
        "lq $8, 0x0($5)\n"
        "addi $7, $7, -0x1\n"
        "lq $9, 0x0($6)\n"
        "addu $5, $5, $12\n"
        "qfsrv $10, $9, $8\n"
        "pextlb $8, $0, $10\n"
        "pextub $9, $0, $10\n"
        "sq $8, 0x0($14)\n"
        "addu $6, $6, $12\n"
        "sq $9, 0x10($14)\n"
        "bgtz $7, 1b\n"
        "addu $14, $14, $11\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $10, $15, $7\n"
        "bnez $10, 1b\n"
        "daddu $15, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00404E10);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404E10(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "sll $11, $12, 1\n"
        "mtsab $13, 0x0\n"
        "addiu $24, $0, -0x1\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "addiu $15, $0, -0x1\n"
        "2:\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "qfsrv $9, $8, $8\n"
        "pextlb $8, $0, $9\n"
        "sq $8, 0x0($14)\n"
        "addi $7, $7, -0x1\n"
        "addu $5, $5, $12\n"
        "addu $14, $14, $11\n"
        "bgtz $7, 2b\n"
        "addu $6, $6, $12\n"
        "addiu $5, $5, 0x140\n"
        "addiu $6, $6, 0x140\n"
        "lw $7, 0xC($4)\n"
        "and $10, $15, $7\n"
        "bnez $10, 2b\n"
        "daddu $15, $0, $0\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "bnez $24, 1b\n"
        "daddu $24, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00404EA8);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404EA8(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $24, 0x10($4)\n"
        "lq $8, 0x0($5)\n"
        "sll $12, $24, 1\n"
        "lq $9, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $10, $9, $8\n"
        "pextlb $8, $0, $10\n"
        "addiu $11, $0, -0x1\n"
        "beqz $7, 2f\n"
        "pextub $9, $0, $10\n"
        "1:\n"
        "addu $5, $5, $24\n"
        "addu $6, $6, $24\n"
        "lq $10, 0x0($5)\n"
        "lq $15, 0x0($6)\n"
        "qfsrv $2, $15, $10\n"
        "pextlb $10, $0, $2\n"
        "addi $7, $7, -0x1\n"
        "pextub $15, $0, $2\n"
        "paddh $2, $8, $10\n"
        "paddh $3, $9, $15\n"
        "por $8, $10, $0\n"
        "por $9, $15, $0\n"
        "paddh $2, $2, $25\n"
        "paddh $3, $3, $25\n"
        "psrlh $2, $2, 1\n"
        "psrlh $3, $3, 1\n"
        "sq $2, 0x0($14)\n"
        "sq $3, 0x10($14)\n"
        "bgtz $7, 1b\n"
        "addu $14, $14, $12\n"
        "2:\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $10, $11, $7\n"
        "bnez $10, 1b\n"
        "daddu $11, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00404F60);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00404F60(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "addiu $11, $0, 0x1\n"
        "sll $24, $12, 1\n"
        "mtsab $13, 0x0\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "qfsrv $8, $8, $8\n"
        "ori $11, $11, 0x8000\n"
        "beqz $7, 3f\n"
        "pextlb $15, $0, $8\n"
        "2:\n"
        "addu $5, $5, $12\n"
        "addu $6, $6, $12\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $10, $0, $8\n"
        "addi $7, $7, -0x1\n"
        "paddh $9, $10, $15\n"
        "por $15, $10, $0\n"
        "paddh $10, $9, $25\n"
        "psrlh $10, $10, 1\n"
        "sq $10, 0x0($14)\n"
        "bgtz $7, 2b\n"
        "addu $14, $14, $24\n"
        "3:\n"
        "psrah $10, $11, 15\n"
        "addiu $5, $5, 0x140\n"
        "lw $7, 0xC($4)\n"
        "addiu $6, $6, 0x140\n"
        "and $10, $10, $7\n"
        "bnez $10, 2b\n"
        "andi $11, $11, 0x7FFF\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "andi $10, $11, 0x1\n"
        "bnez $10, 1b\n"
        "andi $11, $11, 0xFFFE\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405030);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405030(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "addiu $24, $0, 0x1\n"
        "lw $9, 0x10($4)\n"
        "sll $8, $9, 1\n"
        "addiu $11, $0, -0x1\n"
        "1:\n"
        "lq $10, 0x0($5)\n"
        "lq $15, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $2, $15, $10\n"
        "qfsrv $3, $10, $15\n"
        "pextlb $10, $0, $2\n"
        "addi $7, $7, -0x1\n"
        "pextub $15, $0, $2\n"
        "mtsab $24, 0x0\n"
        "qfsrv $3, $3, $2\n"
        "pextlb $2, $0, $3\n"
        "pextub $3, $0, $3\n"
        "paddh $10, $10, $2\n"
        "paddh $15, $15, $3\n"
        "paddh $2, $10, $25\n"
        "paddh $3, $15, $25\n"
        "psrlh $2, $2, 1\n"
        "psrlh $3, $3, 1\n"
        "sq $2, 0x0($14)\n"
        "sq $3, 0x10($14)\n"
        "addu $5, $5, $9\n"
        "addu $6, $6, $9\n"
        "bgtz $7, 1b\n"
        "addu $14, $14, $8\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $12, $11, $7\n"
        "bnez $12, 1b\n"
        "daddu $11, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004050E0);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004050E0(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "addiu $24, $0, 0x1\n"
        "addiu $12, $0, -0x1\n"
        "lw $3, 0x10($4)\n"
        "sll $2, $3, 1\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "addiu $11, $0, -0x1\n"
        "2:\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "mtsab $13, 0x0\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $9, $0, $8\n"
        "addi $7, $7, -0x1\n"
        "addu $5, $5, $3\n"
        "addu $6, $6, $3\n"
        "mtsab $24, 0x0\n"
        "qfsrv $10, $0, $8\n"
        "pextlb $8, $0, $10\n"
        "paddh $10, $9, $8\n"
        "paddh $10, $10, $25\n"
        "psrlh $10, $10, 1\n"
        "sq $10, 0x0($14)\n"
        "bgtz $7, 2b\n"
        "addu $14, $14, $2\n"
        "addiu $5, $5, 0x140\n"
        "addiu $6, $6, 0x140\n"
        "lw $7, 0xC($4)\n"
        "and $10, $11, $7\n"
        "bnez $10, 2b\n"
        "daddu $11, $0, $0\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "bnez $12, 1b\n"
        "daddu $12, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405198);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405198(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "psllh $25, $25, 1\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "addiu $24, $0, 0x1\n"
        "lq $8, 0x0($5)\n"
        "lq $9, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $10, $9, $8\n"
        "qfsrv $15, $8, $9\n"
        "pextlb $8, $0, $10\n"
        "pextub $9, $0, $10\n"
        "addiu $11, $0, -0x1\n"
        "mtsab $24, 0x0\n"
        "qfsrv $15, $15, $10\n"
        "pextlb $10, $0, $15\n"
        "pextub $15, $0, $15\n"
        "paddh $8, $8, $10\n"
        "beqz $7, 2f\n"
        "paddh $9, $9, $15\n"
        "1:\n"
        "addu $5, $5, $12\n"
        "addu $6, $6, $12\n"
        "lq $10, 0x0($5)\n"
        "lq $15, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $2, $15, $10\n"
        "qfsrv $3, $10, $15\n"
        "pextlb $10, $0, $2\n"
        "addi $7, $7, -0x1\n"
        "pextub $15, $0, $2\n"
        "mtsab $24, 0x0\n"
        "qfsrv $3, $3, $2\n"
        "pextlb $2, $0, $3\n"
        "pextub $3, $0, $3\n"
        "paddh $10, $10, $2\n"
        "paddh $15, $15, $3\n"
        "paddh $2, $8, $10\n"
        "paddh $3, $9, $15\n"
        "por $8, $10, $0\n"
        "por $9, $15, $0\n"
        "paddh $2, $2, $25\n"
        "paddh $3, $3, $25\n"
        "psrlh $2, $2, 2\n"
        "psrlh $3, $3, 2\n"
        "sq $2, 0x0($14)\n"
        "sll $10, $12, 1\n"
        "sq $3, 0x10($14)\n"
        "bgtz $7, 1b\n"
        "addu $14, $14, $10\n"
        "2:\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $10, $11, $7\n"
        "bnez $10, 1b\n"
        "daddu $11, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405290);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405290(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "psllh $25, $25, 1\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "addiu $24, $0, 0x1\n"
        "addiu $11, $0, 0x1\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "mtsab $13, 0x0\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $9, $0, $8\n"
        "addu $5, $5, $12\n"
        "ori $11, $11, 0x8000\n"
        "mtsab $24, 0x0\n"
        "qfsrv $10, $0, $8\n"
        "pextlb $8, $0, $10\n"
        "beqz $7, 3f\n"
        "paddh $15, $9, $8\n"
        "2:\n"
        "addu $6, $6, $12\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "mtsab $13, 0x0\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $9, $0, $8\n"
        "addi $7, $7, -0x1\n"
        "addu $5, $5, $12\n"
        "mtsab $24, 0x0\n"
        "qfsrv $10, $0, $8\n"
        "pextlb $8, $0, $10\n"
        "paddh $10, $9, $8\n"
        "paddh $9, $10, $15\n"
        "por $15, $10, $0\n"
        "paddh $10, $9, $25\n"
        "sll $8, $12, 1\n"
        "psrlh $10, $10, 2\n"
        "sq $10, 0x0($14)\n"
        "bgtz $7, 2b\n"
        "addu $14, $14, $8\n"
        "3:\n"
        "psrah $10, $11, 15\n"
        "addiu $5, $5, 0x140\n"
        "lw $7, 0xC($4)\n"
        "addiu $6, $6, 0x140\n"
        "and $10, $10, $7\n"
        "bnez $10, 2b\n"
        "andi $11, $11, 0x7FFF\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "andi $10, $11, 0x1\n"
        "bnez $10, 1b\n"
        "andi $11, $11, 0xFFFE\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405390);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405390(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $9, 0x10($4)\n"
        "sll $8, $9, 1\n"
        "addiu $11, $0, -0x1\n"
        "mtsab $13, 0x0\n"
        "1:\n"
        "lq $10, 0x0($5)\n"
        "lq $15, 0x0($6)\n"
        "qfsrv $2, $15, $10\n"
        "pextlb $10, $0, $2\n"
        "pextub $15, $0, $2\n"
        "lq $2, 0x0($14)\n"
        "lq $3, 0x10($14)\n"
        "paddh $2, $2, $10\n"
        "paddh $3, $3, $15\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "paddh $10, $2, $25\n"
        "psrlh $2, $10, 1\n"
        "paddh $10, $3, $25\n"
        "psrlh $3, $10, 1\n"
        "sq $2, 0x0($14)\n"
        "sq $3, 0x10($14)\n"
        "addi $7, $7, -0x1\n"
        "addu $5, $5, $9\n"
        "addu $14, $14, $8\n"
        "bgtz $7, 1b\n"
        "addu $6, $6, $9\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $12, $11, $7\n"
        "bnez $12, 1b\n"
        "daddu $11, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405430);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405430(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "addiu $12, $0, -0x1\n"
        "lw $3, 0x10($4)\n"
        "sll $2, $3, 1\n"
        "mtsab $13, 0x0\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "addiu $11, $0, -0x1\n"
        "2:\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $9, $0, $8\n"
        "addi $7, $7, -0x1\n"
        "addu $5, $5, $3\n"
        "addu $6, $6, $3\n"
        "lq $8, 0x0($14)\n"
        "paddh $10, $9, $8\n"
        "pcgth $9, $10, $0\n"
        "psrlh $9, $9, 15\n"
        "paddh $10, $10, $9\n"
        "psrlh $10, $10, 1\n"
        "sq $10, 0x0($14)\n"
        "bgtz $7, 2b\n"
        "addu $14, $14, $2\n"
        "addiu $5, $5, 0x140\n"
        "addiu $6, $6, 0x140\n"
        "lw $7, 0xC($4)\n"
        "and $10, $11, $7\n"
        "bnez $10, 2b\n"
        "daddu $11, $0, $0\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "bnez $12, 1b\n"
        "daddu $12, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004054E0);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004054E0(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "lq $8, 0x0($5)\n"
        "lq $9, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $10, $9, $8\n"
        "sll $24, $12, 1\n"
        "pextlb $8, $0, $10\n"
        "addiu $11, $0, -0x1\n"
        "beqz $7, 2f\n"
        "pextub $9, $0, $10\n"
        "1:\n"
        "addu $5, $5, $12\n"
        "addu $6, $6, $12\n"
        "lq $10, 0x0($5)\n"
        "lq $15, 0x0($6)\n"
        "qfsrv $2, $15, $10\n"
        "pextlb $10, $0, $2\n"
        "addi $7, $7, -0x1\n"
        "pextub $15, $0, $2\n"
        "paddh $2, $8, $10\n"
        "paddh $3, $9, $15\n"
        "por $8, $10, $0\n"
        "por $9, $15, $0\n"
        "paddh $2, $2, $25\n"
        "paddh $3, $3, $25\n"
        "psrlh $2, $2, 1\n"
        "psrlh $3, $3, 1\n"
        "lq $10, 0x0($14)\n"
        "lq $15, 0x10($14)\n"
        "paddh $2, $2, $10\n"
        "paddh $3, $3, $15\n"
        "paddh $10, $2, $25\n"
        "psrlh $2, $10, 1\n"
        "pcgth $10, $3, $0\n"
        "psrlh $10, $10, 15\n"
        "paddh $10, $3, $10\n"
        "psrlh $3, $10, 1\n"
        "sq $2, 0x0($14)\n"
        "sq $3, 0x10($14)\n"
        "bgtz $7, 1b\n"
        "addu $14, $14, $24\n"
        "2:\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $10, $11, $7\n"
        "bnez $10, 1b\n"
        "daddu $11, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004055C0);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004055C0(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "addiu $11, $0, 0x1\n"
        "sll $24, $12, 1\n"
        "mtsab $13, 0x0\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "qfsrv $8, $8, $8\n"
        "ori $11, $11, 0x8000\n"
        "beqz $7, 3f\n"
        "pextlb $15, $0, $8\n"
        "2:\n"
        "addu $5, $5, $12\n"
        "addu $6, $6, $12\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $10, $0, $8\n"
        "addi $7, $7, -0x1\n"
        "paddh $9, $10, $15\n"
        "por $15, $10, $0\n"
        "paddh $10, $9, $25\n"
        "psrlh $10, $10, 1\n"
        "lq $8, 0x0($14)\n"
        "paddh $10, $10, $8\n"
        "pcgth $9, $10, $0\n"
        "psrlh $9, $9, 15\n"
        "paddh $10, $10, $9\n"
        "psrlh $10, $10, 1\n"
        "sq $10, 0x0($14)\n"
        "bgtz $7, 2b\n"
        "addu $14, $14, $24\n"
        "3:\n"
        "psrah $10, $11, 15\n"
        "addiu $5, $5, 0x140\n"
        "lw $7, 0xC($4)\n"
        "addiu $6, $6, 0x140\n"
        "and $10, $10, $7\n"
        "bnez $10, 2b\n"
        "andi $11, $11, 0x7FFF\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "andi $10, $11, 0x1\n"
        "bnez $10, 1b\n"
        "andi $11, $11, 0xFFFE\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004056A8);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_004056A8(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "addiu $24, $0, 0x1\n"
        "lw $9, 0x10($4)\n"
        "sll $8, $9, 1\n"
        "addiu $11, $0, -0x1\n"
        "1:\n"
        "lq $10, 0x0($5)\n"
        "lq $15, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $2, $15, $10\n"
        "qfsrv $3, $10, $15\n"
        "pextlb $10, $0, $2\n"
        "addi $7, $7, -0x1\n"
        "pextub $15, $0, $2\n"
        "mtsab $24, 0x0\n"
        "qfsrv $3, $3, $2\n"
        "pextlb $2, $0, $3\n"
        "pextub $3, $0, $3\n"
        "paddh $10, $10, $2\n"
        "paddh $15, $15, $3\n"
        "paddh $2, $10, $25\n"
        "paddh $3, $15, $25\n"
        "psrlh $2, $2, 1\n"
        "psrlh $3, $3, 1\n"
        "lq $10, 0x0($14)\n"
        "lq $15, 0x10($14)\n"
        "paddh $2, $2, $10\n"
        "paddh $3, $3, $15\n"
        "paddh $10, $2, $25\n"
        "psrlh $2, $10, 1\n"
        "pcgth $10, $3, $0\n"
        "psrlh $10, $10, 15\n"
        "paddh $10, $3, $10\n"
        "psrlh $3, $10, 1\n"
        "sq $2, 0x0($14)\n"
        "sq $3, 0x10($14)\n"
        "addu $5, $5, $9\n"
        "addu $6, $6, $9\n"
        "bgtz $7, 1b\n"
        "addu $14, $14, $8\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $12, $11, $7\n"
        "bnez $12, 1b\n"
        "daddu $11, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405780);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405780(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "addiu $24, $0, 0x1\n"
        "addiu $12, $0, -0x1\n"
        "lw $3, 0x10($4)\n"
        "sll $2, $3, 1\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "addiu $11, $0, -0x1\n"
        "2:\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "mtsab $13, 0x0\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $9, $0, $8\n"
        "addi $7, $7, -0x1\n"
        "addu $5, $5, $3\n"
        "addu $6, $6, $3\n"
        "mtsab $24, 0x0\n"
        "qfsrv $10, $0, $8\n"
        "pextlb $8, $0, $10\n"
        "paddh $10, $9, $8\n"
        "paddh $10, $10, $25\n"
        "psrlh $10, $10, 1\n"
        "lq $8, 0x0($14)\n"
        "paddh $10, $10, $8\n"
        "pcgth $9, $10, $0\n"
        "psrlh $9, $9, 15\n"
        "paddh $10, $10, $9\n"
        "psrlh $10, $10, 1\n"
        "sq $10, 0x0($14)\n"
        "bgtz $7, 2b\n"
        "addu $14, $14, $2\n"
        "addiu $5, $5, 0x140\n"
        "addiu $6, $6, 0x140\n"
        "lw $7, 0xC($4)\n"
        "and $10, $11, $7\n"
        "bnez $10, 2b\n"
        "daddu $11, $0, $0\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "bnez $12, 1b\n"
        "daddu $12, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405850);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405850(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "psllh $25, $25, 1\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $7, 0x8($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $24, 0x10($4)\n"
        "addiu $12, $0, 0x1\n"
        "lq $8, 0x0($5)\n"
        "lq $9, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $10, $9, $8\n"
        "qfsrv $15, $8, $9\n"
        "pextlb $8, $0, $10\n"
        "pextub $9, $0, $10\n"
        "addiu $11, $0, -0x1\n"
        "mtsab $12, 0x0\n"
        "qfsrv $15, $15, $10\n"
        "pextlb $10, $0, $15\n"
        "pextub $15, $0, $15\n"
        "paddh $8, $8, $10\n"
        "beqz $7, 2f\n"
        "paddh $9, $9, $15\n"
        "1:\n"
        "addu $5, $5, $24\n"
        "addu $6, $6, $24\n"
        "lq $10, 0x0($5)\n"
        "lq $15, 0x0($6)\n"
        "mtsab $13, 0x0\n"
        "qfsrv $2, $15, $10\n"
        "qfsrv $3, $10, $15\n"
        "pextlb $10, $0, $2\n"
        "addi $7, $7, -0x1\n"
        "pextub $15, $0, $2\n"
        "mtsab $12, 0x0\n"
        "qfsrv $3, $3, $2\n"
        "pextlb $2, $0, $3\n"
        "pextub $3, $0, $3\n"
        "paddh $10, $10, $2\n"
        "paddh $15, $15, $3\n"
        "paddh $2, $8, $10\n"
        "paddh $3, $9, $15\n"
        "por $8, $10, $0\n"
        "por $9, $15, $0\n"
        "paddh $2, $2, $25\n"
        "paddh $3, $3, $25\n"
        "psrlh $2, $2, 2\n"
        "psrlh $3, $3, 2\n"
        "lq $10, 0x0($14)\n"
        "lq $15, 0x10($14)\n"
        "paddh $2, $2, $10\n"
        "paddh $3, $3, $15\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "paddh $10, $2, $25\n"
        "psrlh $2, $10, 1\n"
        "paddh $10, $3, $25\n"
        "psrlh $3, $10, 1\n"
        "sq $2, 0x0($14)\n"
        "sll $10, $24, 1\n"
        "sq $3, 0x10($14)\n"
        "bgtz $7, 1b\n"
        "addu $14, $14, $10\n"
        "2:\n"
        "addiu $5, $5, 0x80\n"
        "addiu $6, $6, 0x80\n"
        "lw $7, 0xC($4)\n"
        "and $10, $11, $7\n"
        "bnez $10, 1b\n"
        "daddu $11, $0, $0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405970);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405970(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "pnor $25, $0, $0\n"
        "psrlh $25, $25, 15\n"
        "psllh $25, $25, 1\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "lw $13, 0x4($4)\n"
        "lw $12, 0x10($4)\n"
        "addiu $24, $0, 0x1\n"
        "addiu $11, $0, 0x1\n"
        "1:\n"
        "lw $7, 0x8($4)\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "mtsab $13, 0x0\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $9, $0, $8\n"
        "addu $5, $5, $12\n"
        "ori $11, $11, 0x8000\n"
        "mtsab $24, 0x0\n"
        "qfsrv $10, $0, $8\n"
        "pextlb $8, $0, $10\n"
        "beqz $7, 3f\n"
        "paddh $15, $9, $8\n"
        "2:\n"
        "addu $6, $6, $12\n"
        "ld $8, 0x0($5)\n"
        "ld $9, 0x0($6)\n"
        "pcpyld $8, $9, $8\n"
        "mtsab $13, 0x0\n"
        "qfsrv $8, $8, $8\n"
        "pextlb $9, $0, $8\n"
        "addi $7, $7, -0x1\n"
        "addu $5, $5, $12\n"
        "mtsab $24, 0x0\n"
        "qfsrv $10, $0, $8\n"
        "pextlb $8, $0, $10\n"
        "paddh $10, $9, $8\n"
        "paddh $9, $10, $15\n"
        "por $15, $10, $0\n"
        "paddh $10, $9, $25\n"
        "psrlh $10, $10, 2\n"
        "lq $8, 0x0($14)\n"
        "paddh $10, $10, $8\n"
        "pcgth $9, $10, $0\n"
        "psrlh $9, $9, 15\n"
        "paddh $10, $10, $9\n"
        "sll $8, $12, 1\n"
        "psrlh $10, $10, 1\n"
        "sq $10, 0x0($14)\n"
        "bgtz $7, 2b\n"
        "addu $14, $14, $8\n"
        "3:\n"
        "psrah $10, $11, 15\n"
        "addiu $5, $5, 0x140\n"
        "lw $7, 0xC($4)\n"
        "addiu $6, $6, 0x140\n"
        "and $10, $10, $7\n"
        "bnez $10, 2b\n"
        "andi $11, $11, 0x7FFF\n"
        "lw $5, 0x14($4)\n"
        "lw $6, 0x18($4)\n"
        "lw $14, 0x0($4)\n"
        "addiu $5, $5, 0x40\n"
        "addiu $6, $6, 0x40\n"
        "addiu $14, $14, 0x80\n"
        "andi $10, $11, 0x1\n"
        "bnez $10, 1b\n"
        "andi $11, $11, 0xFFFE\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405A88);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405A88(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "addiu $12, $0, 0x18\n"
        "lui $10, %%hi(D_00405B30)\n"
        "addiu $10, $10, %%lo(D_00405B30)\n"
        "lq $11, 0x0($10)\n"
        "1:\n"
        "lq $8, 0x0($5)\n"
        "addi $12, $12, -0x1\n"
        "lq $13, 0x0($6)\n"
        "addiu $4, $4, 0x10\n"
        "lq $9, 0x10($5)\n"
        "paddh $8, $8, $13\n"
        "lq $2, 0x10($6)\n"
        "pminh $8, $8, $11\n"
        "paddh $9, $9, $2\n"
        "pmaxh $8, $8, $0\n"
        "pminh $9, $9, $11\n"
        "addiu $5, $5, 0x20\n"
        "pmaxh $9, $9, $0\n"
        "addiu $6, $6, 0x20\n"
        "ppacb $10, $9, $8\n"
        "bnez $12, 1b\n"
        "sq $10, -0x10($4)\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405AE8);
#ifdef SKIP_ASM
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void func_00405AE8(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "addiu $12, $0, 0x18\n"
        "lui $10, %%hi(D_00405B30)\n"
        "addiu $10, $10, %%lo(D_00405B30)\n"
        "lq $11, 0x0($10)\n"
        "1:\n"
        "lq $8, 0x0($5)\n"
        "addi $12, $12, -0x1\n"
        "pminh $8, $8, $11\n"
        "lq $9, 0x10($5)\n"
        "pmaxh $8, $8, $0\n"
        "pminh $9, $9, $11\n"
        "addiu $5, $5, 0x20\n"
        "pmaxh $9, $9, $0\n"
        "addiu $4, $4, 0x10\n"
        "ppacb $10, $9, $8\n"
        "bnez $12, 1b\n"
        "sq $10, -0x10($4)\n"
        "nop\n"
        "nop\n"
        ".globl D_00405B30\n"
        "D_00405B30:\n"
        ".word 0x00FF00FF\n"
        ".word 0x00FF00FF\n"
        ".word 0x00FF00FF\n"
        ".word 0x00FF00FF\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405B48);
#ifdef SKIP_ASM
void func_00405B48(unsigned mpeg1) {
    volatile unsigned *ipuCtrl = (volatile unsigned *)0x10002010;
    *ipuCtrl = (*ipuCtrl & 0xFF7FFFFF) | (mpeg1 << 23);
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
int func_00405E10(int mpegSys) {
    return func_00406F78(mpegSys, 3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405E30);
#ifdef SKIP_ASM
#define H1(x) (((x) + ((x) > 0)) >> 1)
#define H3(x) ((3 * (x) + ((x) > 0)) >> 1)
void func_00405E30(int *mpegSys, int *outVec, int *in, int a, int b) {
    if (mpegSys[93] == 3) {
        if (mpegSys[94] != 0) {
            outVec[0] = H1(a) + in[0];
            outVec[1] = H1(b) + in[1] - 1;
            outVec[2] = H3(a) + in[0];
            outVec[3] = H3(b) + in[1] + 1;
        } else {
            outVec[0] = H3(a) + in[0];
            outVec[1] = H3(b) + in[1] - 1;
            outVec[2] = H1(a) + in[0];
            outVec[3] = H1(b) + in[1] + 1;
        }
    } else {
        outVec[0] = H1(a) + in[0];
        outVec[1] = H1(b) + in[1];
        if (mpegSys[93] == 1)
            outVec[1] = outVec[1] - 1;
        else
            outVec[1] = outVec[1] + 1;
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
/**
 * Starts decoding an MPEG video slice: checks its start code and works out its first macroblock (Sony
 * libmpeg).
 *
 * Reads the 32-bit slice start code (0x101-0x1AF, else an error and return 2), the rest of the slice header
 * via func_004074B8 and the first macroblock address increment, and sets the macroblock address to (extension
 * * 128 + vertical position - 1) * mb_width + increment - 1. Then sets the increment output to 1, sets the
 * flag at +0x1B0 and zeroes the eight predictor values.
 *
 * @param mbCount  unused here (the caller passes the picture's macroblock count)
 * @param mbAddress      receives the slice's first macroblock address
 * @param mbIncrement      receives the address increment (1 on success)
 * @param predictors       eight predictor values to reset, probably MPEG-2's motion vector predictors PMV[2][2][2]
 * @return 0 on success, 1 if a decode error was flagged, 2 for a bad slice start code
 */
int _sliceA0(int *self, int mbCount, int *mbAddress, int *mbIncrement, int *predictors) {
    int startCode;
    int rowExtension;
    int mbAddrInc;
    self[0x11C / 4] = 0;
    func_00407438();
    startCode = func_004070F0((int)self, 32);
    if ((unsigned)(startCode - 0x101) >= 0xAF) {
        func_00403470_k(self, D_00497F28, startCode);
        return 2;
    }
    func_004071F8((int)self, 32);
    rowExtension = func_004074B8(self);
    mbAddrInc = func_00405FB8(self);
    *mbIncrement = mbAddrInc;
    if (self[0x11C / 4] != 0) {
        func_004034A8(self, (int)D_00497F50);
        return 1;
    }
    *mbAddress = ((((rowExtension << 7) + (startCode & 0xFF)) - 1) * self[0x12C / 4] + mbAddrInc) - 1;
    *mbIncrement = 1;
    self[0x1B0 / 4] = 1;
    predictors[5] = 0;
    predictors[4] = 0;
    predictors[1] = 0;
    predictors[0] = 0;
    predictors[7] = 0;
    predictors[6] = 0;
    predictors[3] = 0;
    predictors[2] = 0;
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004065D0);
#ifdef SKIP_ASM
/* MSSG mpeg2decode getpic.c decode_macroblock() with macroblock_modes() folded in (Sony libmpeg): p is the decoder
   state (0x11C Fault_Flag, 0x150 picture_coding_type, 0x174 picture_structure, 0x17C frame_pred_frame_dct,
   0x180 concealment_motion_vectors, 0x848 MPEG2_Flag); the block is decoded by an IPU BDEC command. */
extern char D_00497FC0[];
extern void func_004034A8(int *, int);
extern int m65D0_6F78(int *, int) __asm__("func_00406F78");
extern int m65D0_72E8(int *, int) __asm__("func_004072E8");
extern void m65D0_71F8(int *, int) __asm__("func_004071F8");
extern void m65D0_6B00(int *, int *, int *, int *, int, int, int, int, int, int, int) __asm__("func_00406B00");
extern void m65D0_6CA0(int *, int *, int *, int, int, int, int, int) __asm__("func_00406CA0");
extern void m65D0_6DF0(int *, unsigned) __asm__("func_00406DF0");
extern void m65D0_6E20(int *) __asm__("func_00406E20");
extern void m65D0_8AF8(int, int) __asm__("func_00408AF8");
int func_004065D0(int *p, int *mbtype, int *motion_type, int *dct_type, int *PMV, int *mvfs, int *dmvector) {
    int macroblock_type;
    int motion_vector_count, mv_format, dmv, mvscale;
    *(volatile unsigned int *)0x10002010 = (*(volatile unsigned int *)0x10002010 & 0xF8FFFFFF) | (p[0x150 / 4] << 24);
    macroblock_type = m65D0_6F78(p, 1);
    *mbtype = macroblock_type;
    if (macroblock_type == 0) {
        func_004034A8(p, (int)D_00497FC0);
        p[0x11C / 4] = 1;
        return 0;
    }
    if (macroblock_type & 0xC) {
        if (p[0x174 / 4] == 3 && p[0x17C / 4])
            *motion_type = 2;
        else
            *motion_type = m65D0_72E8(p, 2);
    } else if ((macroblock_type & 1) && p[0x180 / 4]) {
        *motion_type = (p[0x174 / 4] == 3) ? 2 : 1;
    }
    if (p[0x174 / 4] == 3) {
        motion_vector_count = (*motion_type == 1) ? 2 : 1;
        mv_format = (*motion_type == 2);
    } else {
        motion_vector_count = (*motion_type == 2) ? 2 : 1;
        mv_format = 0;
    }
    dmv = (*motion_type == 3);
    mvscale = (mv_format == 0 && p[0x174 / 4] == 3);
    *dct_type = (p[0x174 / 4] == 3 && !p[0x17C / 4] && (*mbtype & 3)) ? m65D0_72E8(p, 1) : 0;

    if (*mbtype & 0x10)
        p[0x1B4 / 4] = m65D0_72E8(p, 5);

    if ((*mbtype & 8) || ((*mbtype & 1) && p[0x180 / 4])) {
        if (p[0x848 / 4])
            m65D0_6B00(p, PMV, dmvector, mvfs, 0, motion_vector_count, mv_format, p[0x164 / 4] - 1, p[0x168 / 4] - 1, dmv, mvscale);
        else
            m65D0_6CA0(p, PMV, dmvector, p[0x158 / 4] - 1, p[0x158 / 4] - 1, 0, 0, p[0x154 / 4]);
    }
    if (p[0x11C / 4])
        return 0;

    if (*mbtype & 4) {
        if (p[0x848 / 4])
            m65D0_6B00(p, PMV, dmvector, mvfs, 1, motion_vector_count, mv_format, p[0x16C / 4] - 1, p[0x170 / 4] - 1, 0, mvscale);
        else
            m65D0_6CA0(p, PMV + 2, dmvector, p[0x160 / 4] - 1, p[0x160 / 4] - 1, 0, 0, p[0x15C / 4]);
    }
    if (p[0x11C / 4])
        return 0;

    if ((*mbtype & 1) && p[0x180 / 4])
        m65D0_71F8(p, 1);

    if (*mbtype & 3) {
        m65D0_8AF8(p[p[0x810 / 4] * 0x140 / 4 + 0x594 / 4], 0x300);
        m65D0_6E20(p);
        m65D0_6DF0(p, 0x20000000 | ((*mbtype & 1) << 27) | (p[0x1B0 / 4] << 26) | (*dct_type << 25) | (p[0x1B4 / 4] << 16));
    } else {
        p[p[0x810 / 4] * 0x140 / 4 + 0x6CC / 4] = 1;
    }
    p[0x1B0 / 4] = 0;
    if (p[0x11C / 4])
        return 0;

    if (!(*mbtype & 1))
        p[0x1B0 / 4] = 1;

    if ((*mbtype & 1) && !p[0x180 / 4]) {
        PMV[0] = PMV[1] = PMV[4] = PMV[5] = 0;
        PMV[2] = PMV[3] = PMV[6] = PMV[7] = 0;
    }

    if (p[0x150 / 4] == 2 && !(*mbtype & 9)) {
        PMV[0] = PMV[1] = PMV[4] = PMV[5] = 0;
        if (p[0x174 / 4] == 3) {
            *motion_type = 2;
        } else {
            *motion_type = 1;
            mvfs[0] = (p[0x174 / 4] == 2);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406A78);
#ifdef SKIP_ASM
/* MSSG mpeg2decode motion.c decode_motion_vector() */
void func_00406A78(int *pred, int r_size, int motion_code, int motion_residual, int full_pel_vector) {
    int lim, vec;

    lim = 16 << r_size;
    vec = full_pel_vector ? (*pred >> 1) : (*pred);

    if (motion_code > 0) {
        vec += ((motion_code - 1) << r_size) + motion_residual + 1;
        if (vec >= lim)
            vec -= lim + lim;
    } else if (motion_code < 0) {
        vec -= ((-motion_code - 1) << r_size) + motion_residual + 1;
        if (vec < -lim)
            vec += lim + lim;
    }
    *pred = full_pel_vector ? (vec << 1) : vec;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406B00);
#ifdef SKIP_ASM
extern int Q_72E8(int, int) __asm__("func_004072E8");
extern void Q_6CA0(int, int *, int *, int, int, int, int, int) __asm__("func_00406CA0");
void func_00406B00(int mpegSys, int *x, int *out, int *fieldSelect, int idx, int mvCount, int k, int rSizeH, int rSizeV, int dualPrime, int mvScale) {
    int selectBit;
    int *vector;
    if (mvCount == 1) {
        if (k == 0 && dualPrime == 0) {
            selectBit = Q_72E8(mpegSys, 1);
            fieldSelect[idx] = selectBit;
            fieldSelect[idx + 2] = selectBit;
        }
        vector = &x[idx * 2];
        Q_6CA0(mpegSys, vector, out, rSizeH, rSizeV, dualPrime, mvScale, 0);
        vector[4] = vector[0];
        vector[5] = vector[1];
    } else {
        fieldSelect[idx] = Q_72E8(mpegSys, 1);
        Q_6CA0(mpegSys, &x[idx * 2], out, rSizeH, rSizeV, dualPrime, mvScale, 0);
        fieldSelect[idx + 2] = Q_72E8(mpegSys, 1);
        Q_6CA0(mpegSys, &x[idx * 2 + 4], out, rSizeH, rSizeV, dualPrime, mvScale, 0);
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
void func_00406CA0(int mpegSys, int *x, int *out, int rSizeH, int rSizeV, int dualPrime, int mvScale, int z) {
    int motionCode;
    int residual;
    motionCode = func_00406F78(mpegSys, 2);
    if (rSizeH != 0 && motionCode != 0)
        residual = func_004072E8(mpegSys, rSizeH);
    else
        residual = 0;
    func_00406A78(x, rSizeH, motionCode, residual, z);
    if (dualPrime != 0)
        out[0] = func_00405E10(mpegSys);
    motionCode = func_00406F78(mpegSys, 2);
    if (rSizeV != 0 && motionCode != 0)
        residual = func_004072E8(mpegSys, rSizeV);
    else
        residual = 0;
    if (mvScale != 0)
        x[1] = x[1] >> 1;
    func_00406A78(x + 1, rSizeV, motionCode, residual, z);
    if (mvScale != 0)
        x[1] = x[1] * 2;
    if (dualPrime != 0)
        out[1] = func_00405E10(mpegSys);
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
void func_00406E20(int mpegSys) {
    int n = 0;
    if ((*(volatile unsigned *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 0x1389) {
                func_00402C80(*(int **)(mpegSys + 0x858));
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
long func_00406EC8(int mpegSys) {
    int n = 0;
    long cmdResult = *(volatile long *)0x10002000;
    if (cmdResult < 0 && !(*(volatile unsigned *)0x10002010 & 0x4000)) {
        do {
            if (n++ >= 0x1389) {
                func_00402C80(*(int **)(mpegSys + 0x858));
                n = 0;
            }
            cmdResult = *(volatile long *)0x10002000;
        } while (cmdResult < 0 && !(*(volatile unsigned *)0x10002010 & 0x4000));
    }
    return cmdResult;
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
void func_00407438(int mpegSys) {
    int n;
    func_00406E20(mpegSys);
    n = -(*(volatile int *)0x10002020 & 7) & 7;
    if (n)
        func_004071F8(mpegSys, n);
    while (func_004070F0(mpegSys, 0x18) != 1)
        func_004071F8(mpegSys, 8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004074B8);
#ifdef SKIP_ASM
extern void func_004071F8(int, int);
extern int func_004072E8(int, int);
extern void func_00407A10(int);
int func_004074B8(int *mpegSys) {
    mpegSys[109] = func_004072E8((int)mpegSys, 5);
    if (func_004072E8((int)mpegSys, 1)) {
        func_004072E8((int)mpegSys, 1);
        func_004071F8((int)mpegSys, 7);
        func_00407A10((int)mpegSys);
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
void func_00407630(int *mpegSys) {
    int temporalRef = func_004072E8((int)mpegSys, 10);
    mpegSys[0x150 / 4] = func_004072E8((int)mpegSys, 3);
    func_004072E8((int)mpegSys, 16);
    if ((unsigned)(mpegSys[0x150 / 4] - 2) < 2) {
        mpegSys[0x154 / 4] = func_004072E8((int)mpegSys, 1);
        mpegSys[0x158 / 4] = func_004072E8((int)mpegSys, 3);
    }
    if (mpegSys[0x150 / 4] == 3) {
        mpegSys[0x15C / 4] = func_004072E8((int)mpegSys, 1);
        mpegSys[0x160 / 4] = func_004072E8((int)mpegSys, 3);
    }
    func_00407A10((int)mpegSys);
    func_004076F8((int)mpegSys);
    func_00407A58((int)mpegSys, temporalRef);
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
void func_00407820(int *mpegSys) {
    mpegSys[0x164 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[0x168 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[0x16C / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[0x170 / 4] = Q_72E8((int)mpegSys, 4);
    {
        int v = Q_72E8((int)mpegSys, 2);
        *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFFCFFFF) | (v << 16);
    }
    {
        int v = Q_72E8((int)mpegSys, 2);
        mpegSys[0x174 / 4] = v;
        if (mpegSys[0xD4 / 4] == 0)
            mpegSys[0xD4 / 4] = v;
    }
    mpegSys[0x178 / 4] = Q_72E8((int)mpegSys, 1);
    mpegSys[0x17C / 4] = Q_72E8((int)mpegSys, 1);
    mpegSys[0x180 / 4] = Q_72E8((int)mpegSys, 1);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFBFFFFF) | (Q_72E8((int)mpegSys, 1) << 22);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFDFFFFF) | (Q_72E8((int)mpegSys, 1) << 21);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFEFFFFF) | (Q_72E8((int)mpegSys, 1) << 20);
    mpegSys[0x184 / 4] = Q_72E8((int)mpegSys, 1);
    Q_72E8((int)mpegSys, 1);
    mpegSys[0x188 / 4] = Q_72E8((int)mpegSys, 1);
    if (Q_72E8((int)mpegSys, 1)) {
        Q_72E8((int)mpegSys, 1);
        Q_72E8((int)mpegSys, 3);
        Q_72E8((int)mpegSys, 1);
        Q_72E8((int)mpegSys, 7);
        Q_72E8((int)mpegSys, 8);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407A10);
#ifdef SKIP_ASM
extern void func_004071F8(int, int);
extern int func_004072E8(int, int);
void func_00407A10(int mpegSys) {
    while (func_004072E8(mpegSys, 1))
        func_004071F8(mpegSys, 8);
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
void func_00407AD0(int *mpegSys) {
    mpegSys[0xE8 / 4] = 0;
    mpegSys[0x84C / 4] = mpegSys[0x850 / 4] + 1;
    mpegSys[0x854 / 4] = 1;
    func_004072E8((int)mpegSys, 1);
    func_004072E8((int)mpegSys, 5);
    func_004072E8((int)mpegSys, 6);
    func_004072E8((int)mpegSys, 1);
    func_004072E8((int)mpegSys, 6);
    func_004072E8((int)mpegSys, 6);
    mpegSys[0x1A4 / 4] = func_004072E8((int)mpegSys, 1);
    mpegSys[0x1A8 / 4] = func_004072E8((int)mpegSys, 1);
    func_004076F8(mpegSys);
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407C38);
#ifdef SKIP_ASM
struct S_7C38 { char pad[0x13C]; int f13C; char pad2[0x34]; int f174; int f178; char pad3[8]; int f184; int pad188; int f18C[3]; int f198[3]; };
extern int sc_72E8(struct S_7C38 *, int) __asm__("func_004072E8");
void func_00407C38(struct S_7C38 *a) {
    int n;
    int i;
    if (a->f13C) {
        if (a->f184 == 0) n = 1;
        else n = a->f178 ? 3 : 2;
    } else {
        if (a->f174 != 3) n = 1;
        else n = a->f184 ? 3 : 2;
    }
    for (i = 0; i < n; i++) {
        a->f18C[i] = sc_72E8(a, 16);
        sc_72E8(a, 1);
        a->f198[i] = sc_72E8(a, 16);
        sc_72E8(a, 1);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407D30);
#ifdef SKIP_ASM
extern int func_004072E8(int, int);
void func_00407D30(int mpegSys) {
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 8);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 7);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 0x14);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 0x16);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 0x16);
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
void func_00407E90(int *mpegSys, int n, int flag) {
    int a, b;
    if (flag) {
        if (mpegSys[0x174 / 4] == 3) {
            if (mpegSys[0x150 / 4] == 3)
                a = mpegSys[0x1C4 / 4];
            else
                a = mpegSys[0x1B8 / 4];
            func_004086C0(mpegSys, a, n - 1);
        } else {
            if (mpegSys[0x150 / 4] == 3) {
                a = mpegSys[0x1D4 / 4];
                b = mpegSys[0x1E4 / 4];
            } else {
                a = mpegSys[0x1C8 / 4];
                b = mpegSys[0x1D8 / 4];
            }
            func_004087D0(mpegSys, a, b, n - 1);
        }
    }
    if (mpegSys[0xF8 / 4] == 1)
        mpegSys[0xF8 / 4] = 2;
}
#endif

//100%
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
int func_00407F20(struct M7F20 *m, int a1) {
    int lim = 4;
    struct F7F20 *x;
    struct F7F20 *f = 0;
    int r = 0;
    if (m->f174 == 3)
        lim = 2;
    if (m->f150 == 3) {
        m->c[0].cur = m->c[0].next;
        m->c[1].cur = m->c[1].next;
        m->c[2].cur = m->c[2].next;
        if (m->fA0 + m->fA4 >= lim) {
            m->fE8 = 0;
            m->f1A8 = 0;
            m->f1A4 = 0;
        }
        if ((m->fE8 != 0 || m->f1A8 != 0) && m->f1A4 == 0) {
            m->c[0].ref0->state = 0;
            m->c[1].ref0->state = 0;
            m->c[2].ref0->state = 0;
        }
        m->fE8 = 0;
        m->f1A8 = 0;
        if (m->f174 == 3) {
            if (m->c[0].ref0->state == 1 || m->f1A4 != 0)
                if (m->c[0].ref1->state == 1)
                    r = 1;
        } else {
            if ((m->c[1].ref0->state == 1 && m->c[2].ref0->state == 1) || m->f1A4 != 0) {
                if (m->c[1].ref1->state == 1)
                    if (m->c[2].ref1->state == 1)
                        r = 1;
            }
        }
    } else {
        if (a1 == 0) {
            struct F7F20 *t;
            t = m->c[0].ref0; m->c[0].ref0 = m->c[0].ref1; m->c[0].ref1 = t;
            t = m->c[1].ref0; m->c[1].ref0 = m->c[1].ref1; m->c[1].ref1 = t;
            t = m->c[2].ref0; m->c[2].ref0 = m->c[2].ref1; m->c[2].ref1 = t;
        }
        m->c[0].cur = m->c[0].ref1;
        m->c[1].cur = m->c[1].ref1;
        m->c[2].cur = m->c[2].ref1;
        if (m->f174 == 3) {
            if (m->f150 != 2 || m->c[0].ref0->state == 1)
                r = 1;
        } else {
            x = m->f174 == 1 ? m->c[2].ref1 : m->c[1].ref1;
            if (m->f150 != 2 || (a1 != 0 && x->state == 1) || (m->c[1].ref0->state == 1 && m->c[2].ref0->state == 1))
                r = 1;
        }
    }
    switch (m->f174) {
    case 3:
        f = m->c[0].cur;
        break;
    case 1:
        f = m->c[1].cur;
        break;
    case 2:
        f = m->c[2].cur;
        break;
    }
    f->state = 0;
    f->t18 = m->t828;
    f->f2C = m->f150;
    f->t20 = m->t830;
    f->f30 = m->f174;
    f->f34 = m->f13C;
    f->f38 = m->f188;
    f->f3C = m->f178;
    f->f40 = m->f184;
    f->f44 = m->f18C;
    f->f48 = m->f190;
    f->f4C = m->f194;
    f->f50 = m->f198;
    f->f54 = m->f19C;
    f->f58 = m->f1A0;
    f->f5C = m->f148;
    f->f60 = m->f14C;
    return r;
}
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
void func_00408508(int *mpegSys) {
    if (mpegSys[2] != 2) {
        mpegSys[2] = 2;
        mpegSys[43] = mpegSys[70];
    }
    mpegSys[520] = 1;
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004087D0);
#ifdef SKIP_ASM
// FAKEMATCH: field +0x88 declared as `union { unsigned long l; int i[2]; }` (alias set 0; `i` is never used) so the two 64-bit
// stores don't force the reloads a plain field gets; plain form (`unsigned long f88`) 87.07%. Byte-identical and behaves like the
// plain form; landed marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest
// spelling matches.
struct S87D0 { char pad0[0x80]; int f80; int pad84; union { unsigned long l; int i[2]; } f88; char pad90[0x20]; int fB0; int fB4; int fB8; int fBC; int fC0; int fC4; int fC8; int fCC; int fD0; char padD4[0xA0]; int f174; char pad178[0x6E0]; int f858; };
struct B87D0 { char pad0[0x10]; int f10; char pad14[0x14]; int f28; char pad2C[0x18]; int f44; int f48; int f4C; int f50; int f54; int f58; int f5C; int f60; };
extern void q87_8530(struct S87D0 *, struct B87D0 *, void *, void *, void *) __asm__("func_00408530");
struct R87D0 { char pad[0x10]; int f10; int f14; int f18; int f1C; long f20; int f28; int f2C; int f30; int f34; long f38; };
extern int q87_81D8(struct S87D0 *, struct B87D0 *) __asm__("func_004081D8");
extern int q87_9088(struct S87D0 *, struct B87D0 *) __asm__("func_00409088");
extern int q87_8278(struct S87D0 *, struct B87D0 *) __asm__("func_00408278");
extern void q87_8508(struct S87D0 *) __asm__("func_00408508");
void d87D0(struct S87D0 *a, struct B87D0 *b, struct B87D0 *c) __asm__("func_004087D0");
void d87D0(struct S87D0 *a, struct B87D0 *b, struct B87D0 *c) {
    struct B87D0 *t, *u;
    struct R87D0 *r, *r2, *r3;
    int m = 0;
    if (a->f174 == 2) {
        t = b;
        u = c;
        m = 0x40;
    } else {
        t = c;
        u = b;
    }
    r = (struct R87D0 *)a->f858;
    q87_8530(a, t, &r->f10, &r->f18, &r->f20);
    r2 = (struct R87D0 *)a->f858;
    a->f80 = r2->f10;
    a->f88.l = 1;
    q87_8530(a, u, &r2->f28, &r2->f30, &r2->f38);
    r3 = (struct R87D0 *)a->f858;
    a->f80 = r3->f28;
    a->f88.l = 1;
    r3->f20 |= m;
    r3->f38 |= m;
    a->fCC = t->f5C;
    a->fD0 = t->f60;
    a->fB4 = t->f44;
    a->fB8 = u->f48;
    a->fC0 = t->f50;
    a->fC4 = u->f54;
    if (q87_81D8(a, b) && b->f28 == 1 && c->f28 == 1) {
        b->f10 *= 2;
        if (a->fB0 != 0)
            q87_9088(a, b);
        else
            q87_8278(a, b);
        b->f10 >>= 1;
        q87_8508(a);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408988);
#ifdef SKIP_ASM
struct Ent_8988 { int f0; int f4; unsigned a[4]; unsigned b[69]; int cnt; int pad[4]; };
struct P_8988 { char pad[0x590]; struct Ent_8988 e[2]; int idx; };
extern char D_00526FC0[];
extern int q8988_C078() __asm__("func_0042C078");
extern void q8988_C0C0(void) __asm__("func_0042C0C0");
void q8988(struct P_8988 *p) __asm__("func_00408988");
void q8988(struct P_8988 *p) {
    int i = 0;
    unsigned long long *t = (unsigned long long *)(((unsigned)D_00526FC0 & 0x0FFFFFFF) | 0x20000000);
    int n = p->e[p->idx].cnt;
    int s;
    if (n > 0) {
        do {
            unsigned a = p->e[p->idx].a[i] & 0x0FFFFFFF;
            unsigned b = p->e[p->idx].b[i] & 0x0FFFFFFF;
            int last = (i == n - 1) ? 0 : 3;
            i++;
            t[0] = ((unsigned long long)a << 32) | 0x30000030;
            t[2] = ((unsigned long long)b << 32) | ((unsigned long long)last << 28) | 0x30;
            t += 4;
        } while (i < n);
    }
    s = q8988_C078();
    __asm__ volatile("sync");
    *(volatile int *)0x1000D480 = p->e[p->idx].f0;
    *(volatile int *)0x1000D430 = (int)D_00526FC0;
    *(volatile int *)0x1000D420 = 0;
    *(volatile int *)0x1000D400 = 0x105;
    if (s)
        q8988_C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408AF8);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
void func_00408AF8(unsigned dstAddr, int byteCount) {
    int intrState = func_0042C078();
    *(volatile unsigned *)0x1000B010 = (dstAddr & 0xFFFFFFF) | 0x80000000;
    *(volatile int *)0x1000B020 = byteCount >> 4;
    *(volatile int *)0x1000B000 = 0x100;
    if (intrState)
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
void q8B80(int *mpegSys, unsigned addr, int n) {
    int intrState;
    int sp[8];
    while (*(volatile int *)0x10002010 < 0)
        ;
    intrState = q8B80_C078();
    *(volatile int *)0x1000B010 = addr & 0x0FFFFFFF;
    *(volatile int *)0x1000B020 = n << 6;
    *(volatile int *)0x1000B000 = 0x100;
    if (intrState)
        q8B80_C0C0();
    q8B80_6DF0(mpegSys, n | 0x70000000);
    sp[0] = 4;
    q8B80_2C30(*(int **)((char *)mpegSys + 0x858), sp);
    while (((*(volatile unsigned *)0x1000B000 >> 8) & 1))
        ;
    while (*(volatile int *)0x10002010 < 0)
        ;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408CA8);
#ifdef SKIP_ASM
struct S408CA8 { volatile int cnt; int stop; int rest; int addr; int n; };
int func_00408CA8(int channel, struct S408CA8 *xferState) {
    int last;
    *(volatile int *)0x1000E010 = 8;
    xferState->cnt++;
    if (*(volatile int *)0x1000B020 != 0 || (*(volatile int *)0x1000B000 & 0x100)) {
        xferState->stop = 1;
        /* PORT: PS2-only memory barrier and COP0 interrupt enable (interrupt handler exit). */
        __asm__ volatile("sync");
        __asm__ volatile("ei");
        return 0;
    }
    last = xferState->n - 1;
    if (xferState->cnt < last) {
        *(volatile int *)0x1000B010 = xferState->addr;
        *(volatile int *)0x1000B020 = 0xFFC0;
        *(volatile int *)0x1000B000 = 0x100;
        *(volatile int *)0x10002000 = 0x700003FF;
        xferState->addr = (xferState->addr + 0xFFC00) & 0x0FFFFFFF;
    } else if (xferState->cnt == last) {
        xferState->rest -= xferState->cnt * 0x3FF;
        *(volatile int *)0x1000B010 = xferState->addr;
        *(volatile int *)0x1000B020 = xferState->rest << 6;
        *(volatile int *)0x1000B000 = 0x100;
        *(volatile int *)0x10002000 = xferState->rest | 0x70000000;
    }
    __asm__ volatile("sync");
    __asm__ volatile("ei");
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408DF0);
#ifdef SKIP_ASM
extern char D_004980D8[];
extern int q8DF0_AddH(int, void *, int, void *) __asm__("func_00423AC0");
extern int q8DF0_RemH(int, int) __asm__("RemoveDmacHandler");
extern void q8DF0_En(int) __asm__("func_004249B8");
extern void q8DF0_Dis(int) __asm__("func_00424950");
extern int q8DF0_C078() __asm__("func_0042C078");
extern void q8DF0_C0C0() __asm__("func_0042C0C0");
extern int q8DF0_2C30(int *, int *) __asm__("func_00402C30");
extern void q8DF0_34A8(int *, int) __asm__("func_004034A8");
extern int func_00408CA8();
void q8DF0(int *p, int addr, int size) __asm__("func_00408DF0");
void q8DF0(int *p, int addr, int size) {

    int sp[8];
    struct S408CA8 s; /* func_00408CA8's state: it counts the chunks and sets stop */
    int h;
    int i;
    s.n = size / 1023 + 1;
    s.rest = size;
    s.addr = (addr + 0xFFC00) & 0x0FFFFFFF;
    s.cnt = s.stop = 0;
    while (*(volatile int *)0x10002010 < 0)
        ;
    h = q8DF0_AddH(3, func_00408CA8, 0, &s);
    *(volatile int *)0x1000E010 = 8;
    q8DF0_En(3);
    i = q8DF0_C078();
    {
        volatile int *madr = (volatile int *)0x1000B010;
        volatile int *qwc = (volatile int *)0x1000B020;
        volatile int *chcr = (volatile int *)0x1000B000;
        *madr = addr & 0x0FFFFFFF;
        *qwc = 0xFFC0;
        *chcr = 0x100;
    }
    if (i)
        q8DF0_C0C0();
    *(volatile int *)0x10002000 = 0x700003FF;
    sp[0] = 4;
    q8DF0_2C30(*(int **)((char *)p + 0x858), sp);
    while (s.cnt < s.n)
        ;
    if (s.stop)
        q8DF0_34A8(p, (int)D_004980D8);
    while (*(volatile int *)0x10002010 < 0)
        ;
    q8DF0_Dis(3);
    q8DF0_RemH(3, h);
}
#endif

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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409088);
#ifdef SKIP_ASM
struct L9 { int ev; int pad[7]; unsigned cnt; unsigned addr; };
extern int E_2C30(int, void *) __asm__("func_00402C30");
extern void E_6DF0(char *, int) __asm__("func_00406DF0");
extern int E_AddH(int, void *, int, void *) __asm__("func_00423AC0");
extern int E_RemH(int, int) __asm__("RemoveDmacHandler");
extern void E_49B8(int) __asm__("func_004249B8");
extern void E_4950(int) __asm__("func_00424950");
extern int E_C078() __asm__("func_0042C078");
extern void E_C0C0() __asm__("func_0042C0C0");
extern void E_8B80(char *, int, int) __asm__("func_00408B80");
extern void E_8DF0(char *, int, int) __asm__("func_00408DF0");
extern void E_8FB0(void) __asm__("func_00408FB0");
void q9088(char *d, unsigned *b) __asm__("func_00409088");
void q9088(char *mpegSys, unsigned *picInfo) {
    struct L9 xferState;
    int n = picInfo[3] * picInfo[4];
    int handlerId;
    xferState.ev = 2;
    E_2C30(*(int *)(mpegSys + 0x858), &xferState);
    if (*(volatile unsigned *)0x10002010 & 0x4000)
        *(volatile unsigned *)0x10002010 = 0x40000000;
    while ((int)*(volatile unsigned *)0x10002010 < 0);
    E_6DF0(mpegSys, 0);
    while ((int)*(volatile unsigned *)0x10002010 < 0);
    xferState.cnt = n * 0x18;
    xferState.addr = picInfo[0] & 0xFFFFFFF;
    if (xferState.cnt > 0xFFFF) {
        int intrState;
        handlerId = E_AddH(4, E_8FB0, 0, &xferState.cnt);
        *(volatile unsigned *)0x1000E010 = 0x10;
        E_49B8(4);
        intrState = E_C078();
        *(volatile unsigned *)0x1000B410 = xferState.addr;
        *(volatile unsigned *)0x1000B420 = 0xFFFF;
        *(volatile unsigned *)0x1000B400 = 0x101;
        if (intrState)
            E_C0C0();
        xferState.addr = (xferState.addr + 0xFFFF0) & 0xFFFFFFF;
        xferState.cnt -= 0xFFFF;
        if (n < 0x400)
            E_8B80(mpegSys, *(int *)(mpegSys + 0xD8), n);
        else
            E_8DF0(mpegSys, *(int *)(mpegSys + 0xD8), n);
        E_4950(4);
        E_RemH(4, handlerId);
    } else {
        int intrState = E_C078();
        *(volatile unsigned *)0x1000B410 = picInfo[0] & 0xFFFFFFF;
        *(volatile unsigned *)0x1000B420 = xferState.cnt;
        *(volatile unsigned *)0x1000B400 = 0x101;
        if (intrState)
            E_C0C0();
        xferState.cnt = 0;
        if (n < 0x400)
            E_8B80(mpegSys, *(int *)(mpegSys + 0xD8), n);
        else
            E_8DF0(mpegSys, *(int *)(mpegSys + 0xD8), n);
    }
    xferState.ev = 3;
    E_2C30(*(int *)(mpegSys + 0x858), &xferState);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409310);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
void func_00409310(int chcrValue) {
    int intrState = func_0042C078();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409388);
#ifdef SKIP_ASM
extern int func_0042C078();
extern void func_0042C0C0(void);
void func_00409388(int chcrValue) {
    int intrState = func_0042C078();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409400);
#ifdef SKIP_ASM
extern void q9400_9388(int) __asm__("func_00409388");
extern void q9400_9310(int) __asm__("func_00409310");
void q9400(int *p) __asm__("func_00409400");
void q9400(int *savedRegs) {
    q9400_9388(1);
    savedRegs[0] = *(volatile int *)0x1000B410;
    savedRegs[1] = *(volatile int *)0x1000B430;
    savedRegs[2] = *(volatile int *)0x1000B420;
    savedRegs[3] = *(volatile int *)0x1000B400;
    while (*(volatile int *)0x10002010 & 0xF0)
        ;
    q9400_9310(0);
    savedRegs[4] = *(volatile int *)0x1000B010;
    savedRegs[5] = *(volatile int *)0x1000B020;
    savedRegs[6] = *(volatile int *)0x1000B000;
    savedRegs[7] = *(volatile int *)0x10002020;
    savedRegs[8] = *(volatile int *)0x10002010;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004094E8);
#ifdef SKIP_ASM
extern void q94E8_9388(int) __asm__("func_00409388");
extern void q94E8_9310(int) __asm__("func_00409310");
void q94E8(int *p) __asm__("func_004094E8");
void q94E8(int *savedRegs) {
    unsigned ipuBp = savedRegs[7];
    int e = ipuBp & 0x7F;
    int n = ((ipuBp >> 16) & 3) + ((ipuBp >> 8) & 0xF);
    int toIpuMadr = savedRegs[0] - (n << 4);
    int toIpuQwc = savedRegs[2] + n;
    if (savedRegs[4] != 0 && savedRegs[5] != 0) {
        *(volatile int *)0x1000B010 = savedRegs[4];
        *(volatile int *)0x1000B020 = savedRegs[5];
        q94E8_9310(savedRegs[6] | 0x100);
    }
    while (*(volatile int *)0x10002010 < 0)
        ;
    *(volatile int *)0x10002000 = e;
    while (*(volatile int *)0x10002010 < 0)
        ;
    if (toIpuMadr != 0 && toIpuQwc != 0) {
        *(volatile int *)0x1000B410 = toIpuMadr;
        *(volatile int *)0x1000B430 = savedRegs[1];
        *(volatile int *)0x1000B420 = toIpuQwc;
        q94E8_9388(savedRegs[3] | 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409638);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// NOTE: volatile = the IPU_CTRL hardware register (0x10002010), polled until BUSY clears; without it the poll is hoisted
// (strip test 85.54). The R5900 short-loop nops in the loop come from the assembler, not from the source.
int func_00409638(int mode) {
    int result = 0;
    int ipuCtrl;
    switch (mode) {
    case 0:
        do {
            ipuCtrl = *(volatile int *)0x10002010;
        } while (ipuCtrl < 0);
        result = 0;
        break;
    case 1:
        result = *(volatile unsigned *)0x10002010 >> 31;
        break;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004096A0);
#ifdef SKIP_ASM
extern int func_0042C078(int);
extern void func_0042C0C0(void);
void func_004096A0(int chcrValue) {
    func_0042C078(chcrValue);
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409708);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef int i128_9708 __attribute__((mode(TI)));
extern volatile i128_9708 D_00453340[];
extern i128_9708 D_00453390[];
extern void func_004096A0(int);
#define WAIT_9708() while (*(volatile int *)0x10002010 < 0)
// FAKEMATCH: `extern volatile` view of the IPU quantiser table D_00453340 (only this function reads it), so its four
// copies of entry 4 are reloaded for each FIFO write as in retail instead of being CSEd; plain form 97.11%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
void func_00409708(void) {
    func_004096A0(1);
    *(volatile int *)0x10002010 = 0x40000000;
    WAIT_9708();
    *(volatile int *)0x10002000 = 0;
    WAIT_9708();
    *(volatile i128_9708 *)0x10007010 = D_00453340[0];
    *(volatile i128_9708 *)0x10007010 = D_00453340[1];
    *(volatile i128_9708 *)0x10007010 = D_00453340[2];
    *(volatile i128_9708 *)0x10007010 = D_00453340[3];
    *(volatile i128_9708 *)0x10007010 = D_00453340[4];
    *(volatile i128_9708 *)0x10007010 = D_00453340[4];
    *(volatile i128_9708 *)0x10007010 = D_00453340[4];
    *(volatile i128_9708 *)0x10007010 = D_00453340[4];
    *(volatile int *)0x10002000 = 0x50000000;
    WAIT_9708();
    *(volatile int *)0x10002000 = 0x58000000;
    WAIT_9708();
    *(volatile i128_9708 *)0x10007010 = D_00453390[0];
    *(volatile i128_9708 *)0x10007010 = D_00453390[1];
    *(volatile int *)0x10002000 = 0x60000000;
    WAIT_9708();
    *(volatile int *)0x10002000 = 0x90000000;
    WAIT_9708();
    *(volatile int *)0x10002010 = 0x40000000;
    WAIT_9708();
    *(volatile int *)0x10002000 = 0;
    WAIT_9708();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409940);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004980F0[];
extern char D_00498108[];
extern char D_00498130[];
extern int Q_sema(int *) __asm__("CreateSema");
extern void Q_sig(int) __asm__("SignalSema");
extern void Q_log(char *) __asm__("func_00425C60");
extern int Q_A360(int, int, int) __asm__("func_0040A360");
extern void Q_6408(int) __asm__("func_00426408");
// NOTE: the nops in retail's two wait loops are the assembler's R5900 short-loop padding, so the loops are plain C.
int func_00409940(void) {
    int s[6];
    int r;
    int i;
    if (D_004533C4 < 0) {
        s[5] = 0;
        s[2] = 1;
        s[1] = 1;
        D_004533C4 = Q_sema(s);
        s[1] = 1;
        s[2] = 0;
        s[5] = 0;
        D_004533C8 = Q_sema(s);
    }
    Q_A360(0, 0, 0);
    WaitSema(D_004533C4);
    Q_6408(0);
    for (;;) {
        if (func_00426B48(&D_005271C0, 0x80000400, 0) < 0) {
            Q_log(D_004980F0);
            for (;;)
                ;
        }
        if (D_005271C0.server != 0)
            break;
        i = 0x100000;
        do {
            i--;
        } while (i != 0);
    }
    r = func_00426D18(&D_005271C0, 0xFE, 0, &D_00527240, 48, D_00528780, 12, 0, 0);
    Q_sig(D_004533C4);
    if (r < 0) {
        D_005271C0.server = 0;
        return r - 100;
    }
    if (D_00528780[1] < 0x20A) {
        Q_log(D_00498108);
        D_005271C0.server = 0;
        return -120;
    }
    if (D_00528780[2] < 0x20E) {
        Q_log(D_00498130);
        D_005271C0.server = 0;
        return -121;
    }
    return D_00528780[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409B10);
#ifdef SKIP_ASM
extern int DeleteSema(int);
int func_00409B10(void) {
    if (D_004533C4 >= 0) {
        DeleteSema(D_004533C4);
        D_004533C4 = -1;
        DeleteSema(D_004533C8);
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
extern int PollSema(int);
extern void SignalSema(int);
int func_00409B90(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.origin = a;
    r = func_00426D18(&D_005271C0, 20, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 20;
    else
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409C48);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
int func_00409C48(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    r = func_00426D18(&D_005271C0, 21, 0, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r != 0) {
        SignalSema(D_004533C4);
        return r;
    }
    SignalSema(D_004533C4);
    return D_00528780[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409D10);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
int func_00409D10(int a, int b, int s_, int c) {
    char *s = (char *)s_;
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        SignalSema(D_004533C4);
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
        SignalSema(D_004533C4);
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
extern int PollSema(int);
extern void SignalSema(int);
int func_00409E70(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    r = func_00426D18(&D_005271C0, 3, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 3;
    else
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409F28);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
int func_00409F28(int a, int b, int c) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    D_00527240.offset = b;
    D_00527240.origin = c;
    r = func_00426D18(&D_005271C0, 4, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 4;
    else
        SignalSema(D_004533C4);
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
extern int PollSema(int);
extern void SignalSema(int);
extern void func_00426358(int, int);
int func_0040A090(int a, int b, int c) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
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
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A1A8);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern void FlushCache(int);
int func_0040A1A8(int a, unsigned char *p, int n) {
    int r;
    unsigned i;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
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
    FlushCache(0);
    r = func_00426D18(&D_005271C0, 6, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 6;
    else
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A320);
#ifdef SKIP_ASM
extern void iSignalSema(int);
void func_0040A320(int alarmId, int ticks, int sema) {
    iSignalSema(sema);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A328);
#ifdef SKIP_ASM
extern void func_0040A320(int, int, int);
extern void SetAlarm(int, void (*)(int, int, int), int);
extern void WaitSema(int);
void func_0040A328(unsigned short a) {
    SetAlarm(a, func_0040A320, D_004533C8);
    WaitSema(D_004533C8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A360);
#ifdef SKIP_ASM
extern void SignalSema(int);
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
        SignalSema(D_004533C4);
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
extern int PollSema(int);
extern void SignalSema(int);
extern void func_00426358(int, int);
int func_0040A498(int a, int b, int *c, int *d, int *e) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
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
        SignalSema(D_004533C4);
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
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
extern void func_00426358(int, int);
int func_0040A688(int a, int b, char *s, int c, int d, int e) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        SignalSema(D_004533C4);
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
        SignalSema(D_004533C4);
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
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
extern void func_00426358(int, int);
int func_0040A858(int a, int b, char *s, void *u) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        SignalSema(D_004533C4);
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
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040A998);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
int func_0040A998(int a, int b) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    D_00527240.slot = b;
    r = func_00426D18(&D_005271C0, 16, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 16;
    else
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AA68);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
int func_0040AA68(int a, int b, char *s) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        SignalSema(D_004533C4);
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
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AB80);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
int func_0040AB80(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    r = func_00426D18(&D_005271C0, 10, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 10;
    else
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AC38);
#ifdef SKIP_ASM
extern int R_3DF0(int) __asm__("PollSema");
extern void R_3DC0(int) __asm__("SignalSema");
extern void R_4020(int) __asm__("FlushCache");
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
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
extern void FlushCache(int);
int func_0040AE00(int a, int b, char *s, char *t) {
    int r;
    char *p;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0 || t == 0) {
        SignalSema(D_004533C4);
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
    FlushCache(0);
    r = func_00426D18(&D_005271C0, 14, 1, &D_00527270, 0x414, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 19;
    else
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040AF58);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
int func_0040AF58(int a, int b) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    D_00527240.slot = b;
    r = func_00426D18(&D_005271C0, 17, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
    if (r == 0)
        D_004533C0 = 17;
    else
        SignalSema(D_004533C4);
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B028);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
int func_0040B028(int a, int b, char *s) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    if (s == 0 || s[0] == 0) {
        SignalSema(D_004533C4);
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
        SignalSema(D_004533C4);
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B2D0);
#ifdef SKIP_ASM
struct C_B2D0 { unsigned *f0; int pad4; int f8; int pad[2]; int f14; int f18; void (*f1C)(int); int f20; int f24; int f28; int f2C; };
struct E_B2D0 { char pad[0x1C]; struct C_B2D0 *ctx; unsigned code; int f24; int f28; int f2C; };
extern void iSignalSema(int);
extern void func_0040B2B0(unsigned *);
void func_0040B2D0(struct E_B2D0 *e) {
    struct C_B2D0 *c;
    switch (e->code) {
    case 0x8000001A:
        c = e->ctx;
        if (c->f1C)
            c->f1C(c->f20);
        iSignalSema(c->f28);
        break;
    case 0x80000019:
        c = e->ctx;
        c->f24 = e->f24;
        c->f14 = e->f28;
        c->f18 = e->f2C;
        iSignalSema(c->f28);
        break;
    case 0x8000001C:
        break;
    case 0x8000001D:
        c = e->ctx;
        c->f2C = e->f24;
        iSignalSema(c->f28);
        break;
    }
    c = e->ctx;
    if (c->f8 >= 0)
        iSignalSema(c->f8);
    func_0040B2B0(c->f0);
    c->f0 = 0;
}
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
extern int CreateSema(int *);
extern int fr_B400(int) __asm__("DeleteSema");
extern void WaitSema(int);
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
    c->f28 = CreateSema(s);
    if (c->f28 < 0)
        return -3;
    WaitSema(c->f28);
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
    h = CreateSema(s);
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
    WaitSema(h);
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
extern int b5_00423DE0(int) __asm__("WaitSema");
extern int b5_00423DB0(int) __asm__("DeleteSema");
extern struct O_B5B0 *b5_0040B208(char *) __asm__("func_0040B208");
extern void b5_0040B2B0(struct O_B5B0 *) __asm__("func_0040B2B0");
extern int b5_00423DA0(struct QSema *) __asm__("CreateSema");
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B6D0);
#ifdef SKIP_ASM
struct N_B6D0 { int f0; int f4; int f8; char padC[8]; int f14; int f18; int f1C; int f20; int f24; int f28; int f2C; int f30; int f34; };
extern void lg3_B6D0(char *, char *, int) __asm__("func_00425C60");
extern void lg1_B6D0(char *) __asm__("func_00425C60");
extern int wt_B6D0(int) __asm__("WaitSema");
extern int fr_B6D0(int) __asm__("DeleteSema");
extern int sm_B6D0(int *) __asm__("CreateSema");
extern struct N_B6D0 *al_B6D0(char *) __asm__("func_0040B208");
extern void rl_B6D0(struct N_B6D0 *) __asm__("func_0040B2B0");
extern void sz_B6D0(int, int) __asm__("func_00426358");
extern int st_B6D0(unsigned, struct N_B6D0 *, int, int, int, int) __asm__("func_004261B0");
extern char D_00498178[];
extern char D_004981A0[];
extern char D_004981D8[];
extern char D_004981E8[];
int func_0040B6D0(struct N_B6D0 *c, int a, int flag, int d, int e, int f, int g, int h, int i) {
    int s[6];
    int sem;
    struct N_B6D0 *n;
    if (flag != 0) {
        lg1_B6D0(D_00498178);
        return -1;
    }
    wt_B6D0(c->f28);
    if (d & 0x3F) {
        lg3_B6D0(D_004981A0, D_004981D8, d);
        return -1;
    }
    if (f & 0x3F) {
        lg3_B6D0(D_004981E8, D_004981D8, f);
        return -1;
    }
    n = al_B6D0(D_00528FC0);
    if (n == 0)
        return -1;
    c->f4 = n->f18;
    c->f20 = i;
    c->f0 = (int)n;
    c->f1C = h;
    n->f20 = a;
    n->f14 = (int)n;
    n->f24 = e;
    n->f28 = f;
    n->f2C = g;
    n->f1C = (int)c;
    n->f34 = c->f24;
    if (d == f) {
        sz_B6D0(d, e >= g ? e : g);
    } else {
        if (e > 0)
            sz_B6D0(d, e);
        if (g > 0)
            sz_B6D0(f, g);
    }
    if (flag & 1) {
        if (h == 0)
            n->f30 = 0;
        else
            n->f30 = 1;
        c->f8 = -1;
        if (st_B6D0(0x8000001A, n, 0x40, d, c->f14, e) == 0) {
            rl_B6D0(n);
            return -2;
        }
        return 0;
    }
    s[2] = 0;
    s[1] = 1;
    s[5] = 0;
    sem = sm_B6D0(s);
    c->f8 = sem;
    if (sem < 0) {
        rl_B6D0(n);
        return -3;
    }
    n->f30 = 1;
    if (st_B6D0(0x8000001A, n, 0x40, d, c->f14, e) == 0) {
        rl_B6D0(n);
        fr_B6D0(sem);
        return -2;
    }
    wt_B6D0(sem);
    fr_B6D0(sem);
    return 0;
}
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
extern void GetOsdConfigParam(unsigned *);
int func_0040B9E8(void) {
    unsigned buf[1];
    int r;
    GetOsdConfigParam(buf);
    if (func_0040B9A8()) {
        r = D_004533FC[0];
    } else {
        GetOsdConfigParam(buf);
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
extern void GetOsdConfigParam(unsigned *);
int func_0040BA68(void) {
    unsigned buf[1];
    if (func_0040B9A8())
        return D_004533FA[0];
    GetOsdConfigParam(buf);
    return (buf[0] >> 1) & 3;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BAA8);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void GetOsdConfigParam(unsigned *);
int func_0040BAA8(void) {
    unsigned buf[1];
    if (func_0040B9A8())
        return D_004533FD[0];
    GetOsdConfigParam(buf);
    return buf[0] & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BAE8);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void GetOsdConfigParam(unsigned *);
int func_0040BAE8(void) {
    unsigned buf[1];
    int r;
    if (func_0040B9A8()) {
        r = D_004533F8.f0;
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) r = 0x21C; else r = (int)buf[0] >> 21;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BB38);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void GetOsdConfigParam(unsigned *);
extern int GetOsdConfigParam2(unsigned *, int, int);
int func_0040BB38(void) {
    unsigned buf[2];
    unsigned char v;
    int r;
    if (func_0040B9A8()) {
        r = D_004533FB[0];
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            r = 0;
        } else {
            GetOsdConfigParam2(&buf[1], 1, 1);
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
extern void GetOsdConfigParam(unsigned *);
extern int GetOsdConfigParam2(unsigned *, int, int);
int func_0040BBA0(void) {
    unsigned buf[2];
    unsigned char v;
    int r;
    if (func_0040B9A8()) {
        r = D_004533FE[0];
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            r = 0;
        } else {
            GetOsdConfigParam2(&buf[1], 1, 1);
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
extern void GetOsdConfigParam(unsigned *);
extern int GetOsdConfigParam2(unsigned *, int, int);
int func_0040BC08(void) {
    unsigned buf[2];
    unsigned char v;
    int r;
    if (func_0040B9A8()) {
        r = D_004533FF[0];
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            r = 0;
        } else {
            GetOsdConfigParam2(&buf[1], 1, 1);
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
unsigned char func_0040BC70(unsigned char decimal) {
    return decimal + (decimal / 10) * 6;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BCA0);
#ifdef SKIP_ASM
unsigned char func_0040BCA0(unsigned char bcd) {
    unsigned char t = bcd >> 4;
    t = t * 6;
    return bcd - t;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BCC0);
#ifdef SKIP_ASM
void func_0040BCC0(unsigned char *clock) {
    clock[7] = func_0040BCA0(clock[7]);
    clock[6] = func_0040BCA0(clock[6]);
    clock[5] = func_0040BCA0(clock[5]);
    clock[3] = func_0040BCA0(clock[3]);
    clock[2] = func_0040BCA0(clock[2]);
    clock[1] = func_0040BCA0(clock[1]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BD28);
#ifdef SKIP_ASM
void func_0040BD28(unsigned char *clock) {
    clock[7] = func_0040BC70(clock[7]);
    clock[6] = func_0040BC70(clock[6]);
    clock[5] = func_0040BC70(clock[5]);
    clock[3] = func_0040BC70(clock[3]);
    clock[2] = func_0040BC70(clock[2]);
    clock[1] = func_0040BC70(clock[1]);
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
void func_0040BEF0(unsigned char *clock) {
    clock[3]++;
    if (clock[3] == 24) {
        clock[3] = 0;
        func_0040BD90(clock);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BF20);
#ifdef SKIP_ASM
extern void func_0040BE48(unsigned char *);
void func_0040BF20(unsigned char *clock) {
    if (clock[3] == 0) {
        clock[3] = 23;
        func_0040BE48(clock);
    } else {
        clock[3]--;
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
void func_0040BF48(int clockAddr, int deltaMinutes) {
    int minutes;
    unsigned char *clock = (unsigned char *)clockAddr;
    func_0040BCC0(clock);
    minutes = clock[2] + deltaMinutes;
    if (minutes >= 0) {
        if (minutes > 60) {
            do {
                minutes -= 60;
                func_0040BEF0(clock);
            } while (minutes > 60);
        }
        clock[2] = minutes;
    } else {
        while (minutes < 0) {
            minutes += 60;
            func_0040BF20(clock);
        }
        clock[2] = minutes;
    }
    func_0040BD28(clock);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BFD8);
#ifdef SKIP_ASM
extern void func_0040BF48(int, int);
void func_0040BFD8(int clockAddr) {
    func_0040BF48(clockAddr, -540);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BFE0);
#ifdef SKIP_ASM
extern int func_0040BAE8(void);
extern int func_0040BBA0(void);
extern void func_0040BF48(int, int);
void func_0040BFE0(int clockAddr) {
    int timeZone = func_0040BAE8();
    func_0040BF48(clockAddr, timeZone - 540 + func_0040BBA0() * 60);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C028);
#ifdef SKIP_ASM
struct KbInfo_C028 { int n; char pad[0x8C]; };
extern char D_00498240[];
extern char D_00498258[];
extern void k_6408(int) __asm__("func_00426408");
extern int k_sema(struct QSema *) __asm__("CreateSema");
extern int k_dsema(int) __asm__("DeleteSema");
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
extern int RemoveIntcHandler(int, int);
extern void func_00424880(int);
extern int DeleteSema(int);
extern char D_00498278[];
extern char D_004982A8[];
int func_0040C220(void) {
    int n = 0;
    int s, r;
    s = func_0042C078_0();
    r = RemoveIntcHandler(3, D_0052909C);
    if (r == 0)
        func_00424880(3);
    if (s)
        func_0042C0C0();
    if (r < 0) {
        n = 1;
        func_0040A618_log(D_00498278);
    }
    if (D_00453438 != -1) {
        if (DeleteSema(D_00453438) < 0) {
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
extern void WaitSema(int);
extern void func_00425C60(char *, int);
/**
 * Requests the USB keyboard driver's connection info over SIF RPC (Sony libusbkb).
 *
 * Asynchronous: the reply (0x90 bytes) is copied to @p info by the RPC callback; the pending command (1) is
 * recorded for the library's sync call.
 *
 * @param info  where the keyboard info is stored when the call completes
 * @return 0 when the request was sent, -1 if the library is not bound, -2 if sceSifCallRpc failed
 */
int sceUsbKbGetInfo(void *info) {
    int result;
    if (D_00453438 == -1)
        return -1;
    WaitSema(D_00453438);
    result = func_00426D18(&D_005290A0, 1, 1, D_00529100, 16, D_00529140, 0x90, func_0040C8B0, 0);
    if (result != 0) {
        func_00425C60(D_004982D0, result);
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
extern void WaitSema(int);
extern void func_00425C60(char *, int);
/**
 * Requests the latest input data of one USB keyboard over SIF RPC (Sony libusbkb).
 *
 * Asynchronous, like sceUsbKbGetInfo(): the 0x50-byte reply goes to @p data, and command 2 with the keyboard
 * number is recorded as pending.
 *
 * @param kbdNo  keyboard number
 * @param data  where the keyboard data is stored when the call completes
 * @return 0 when the request was sent, -1 if the library is not bound, -11 when the keyboard number is not
 *         below the driver's keyboard count, -2 if sceSifCallRpc failed
 */
int sceUsbKbRead(unsigned kbdNo, void *data) {
    if (D_00453438 == -1)
        return -1;
    if (kbdNo >= D_00529000.n)
        return -11;
    D_00529100[0] = kbdNo;
    WaitSema(D_00453438);
    {
        int result = func_00426D18(&D_005290A0, 2, 1, D_00529100, 16, D_00529140, 0x50, func_0040C8B0, 0);
        if (result != 0) {
            func_00425C60(D_004982F8, result);
            return -2;
        }
    }
    D_00529000.cmd = 2;
    D_00529000.arg = kbdNo;
    D_00529000.out = data;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbGetLocation);
#ifdef SKIP_ASM
extern char D_00498320[];
extern void func_0040C8B0(void *);
extern void WaitSema(int);
extern void func_00425C60(char *, int);
/**
 * Starts a request for the USB port location of a keyboard (Sony libusbkb).
 *
 * Takes the keyboard RPC semaphore and sends RPC 3 to the IOP asynchronously; the reply is copied to @p location when
 * the pending command is collected later.
 *
 * @param kbdNo  keyboard number
 * @param location  receives the location
 * @return 0 when the request was sent, -1 if the library is not initialised, -11 for a bad keyboard number,
 *         -2 if the RPC call failed
 */
int sceUsbKbGetLocation(int kbdNo, void *location) {
    if (D_00453438 == -1)
        return -1;
    if (kbdNo >= D_00529000.n)
        return -11;
    D_00529100[0] = kbdNo;
    WaitSema(D_00453438);
    {
        int result = func_00426D18(&D_005290A0, 3, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
        if (result != 0) {
            func_00425C60(D_00498320, result);
            return -2;
        }
    }
    D_00529000.cmd = 3;
    D_00529000.arg = kbdNo;
    D_00529000.out = location;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetLEDStatus);
#ifdef SKIP_ASM
extern char D_00498350[];
extern void func_0040C8B0(void *);
extern void WaitSema(int);
extern void func_00425C60(char *, int);
/**
 * Starts a request to set a keyboard's LED state (Sony libusbkb).
 *
 * Takes the keyboard RPC semaphore and sends RPC 4 with the keyboard number and @p ledStatus to the IOP asynchronously.
 *
 * @param kbdNo  keyboard number
 * @param ledStatus  LED state bits
 * @return 0 when the request was sent, -1 if the library is not initialised, -11 for a bad keyboard number,
 *         -2 if the RPC call failed
 */
int sceUsbKbSetLEDStatus(int kbdNo, unsigned char ledStatus) {
    if (D_00453438 == -1)
        return -1;
    if (kbdNo >= D_00529000.n)
        return -11;
    D_00529100[0] = kbdNo;
    D_00529100[1] = ledStatus;
    WaitSema(D_00453438);
    {
        int result = func_00426D18(&D_005290A0, 4, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
        if (result != 0) {
            func_00425C60(D_00498350, result);
            return -2;
        }
    }
    D_00529000.cmd = 4;
    D_00529000.out = 0;
    D_00529000.arg = kbdNo;
    return 0;
}

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetLEDMode);
#ifdef SKIP_ASM
extern char D_00498380[];
extern void func_0040C8B0(void *);
extern void WaitSema(int);
extern void func_00425C60(char *, int);
/**
 * Sets a keyboard's LED mode and sends it to the IOP (Sony libusbkb).
 *
 * Records the mode in the keyboard's state, takes the keyboard RPC semaphore and sends RPC 5 asynchronously.
 *
 * @param kbdNo  keyboard number
 * @param mode  LED mode, 0 to 2
 * @return 0 when the request was sent, -1 if the library is not initialised, -11 for a bad keyboard number,
 *         -12 for a mode above 2, -2 if the RPC call failed
 */
int sceUsbKbSetLEDMode(int kbdNo, unsigned mode) {
    int semaId = D_00453438;
    if (semaId == -1)
        return -1;
    if (kbdNo >= D_00529000.n)
        return -11;
    if (mode > 2)
        return -12;
    D_00529000.port[kbdNo].ledMode = mode;
    D_00529100[0] = kbdNo;
    D_00529100[1] = mode;
    WaitSema(semaId);
    {
        int rpcResult = func_00426D18(&D_005290A0, 5, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
        if (rpcResult != 0) {
            func_00425C60(D_00498380, rpcResult);
            return -2;
        }
    }
    D_00529000.cmd = 5;
    D_00529000.out = 0;
    D_00529000.arg = kbdNo;
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
extern void kb_signal4(int) __asm__("SignalSema");
extern int func_0040C8C0(void);
int func_0040C810(int mode, int *out) {
    if (D_00453438 == -1)
        return -1;
    switch (mode) {
    case 0:
        WaitSema(D_00453438);
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
extern void iSignalSema(int);
void func_0040C8B0(void *arg) {
    iSignalSema(D_00453438);
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
    WaitSema(D_00453438);
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D588);
#ifdef SKIP_ASM
extern int func_0040E878(float x, float *y);
extern float func_0040F5A8(float x, float y, int iy);
typedef union { float f; int bits; } FloatBits_D588;
#define FLOAT_TO_BITS_D588(i, d) do { FloatBits_D588 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
float func_0040D588(float x) {
    float y[2];
    int hx, ix;
    FLOAT_TO_BITS_D588(hx, x);
    ix = hx & 0x7fffffff;
    if (ix <= 0x3f490fda) {
        return func_0040F5A8(x, 0.0f, 1);
    } else if (ix > 0x7f7fffff) {
        return x - x;
    } else {
        int n = func_0040E878(x, y);
        return func_0040F5A8(y[0], y[1], 1 - ((n & 1) << 1));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D610);
#ifdef SKIP_ASM
struct E40D610 { int type; char *name; double arg1; double arg2; double retval; int err; };
extern const int D_00499688[]; /* fdlibm _LIB_VERSION (newlib: _CONST) */
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D758);
#ifdef SKIP_ASM
/* newlib libm expf wrapper (wf_exp.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
struct exception_D758 { int type; char *name; double arg1; double arg2; double retval; int err; };
extern float func_0040E048(float);                                  /* __ieee754_expf */
extern int func_0040F8C0(float);                                    /* finitef */
extern int matherr_D758(struct exception_D758 *) __asm__("func_0040F878");
enum fdlibm_version_D758 { fdlibm_ieee_D758 = -1, fdlibm_svid_D758, fdlibm_xopen_D758, fdlibm_posix_D758 };
extern const enum fdlibm_version_D758 fdlib_version_D758 __asm__("D_00499688");
#define _LIB_VERSION_D758 fdlib_version_D758
#define errno_D758 (*func_00415FB8())
#define HUGE_D758 ((float)3.40282346638528860e+38)
#define HUGE_VAL_D758 (D_00499680[0])
static const float
o_threshold_D758=  8.8721679688e+01,  /* 0x42b17180 */
u_threshold_D758= -1.0397208405e+02;  /* 0xc2cff1b5 */

	float func_0040D758(float x)		/* wrapper expf */
{
	float z;
	struct exception_D758 exc;
	z = func_0040E048(x);
	if(_LIB_VERSION_D758 == -1) return z;
	if(func_0040F8C0(x)) {
	    if(x>o_threshold_D758) {
		/* expf(finite) overflow */
		exc.type = 3;
		exc.name = "expf";
		exc.err = 0;
		exc.arg1 = exc.arg2 = (double)x;
		if (_LIB_VERSION_D758 == 0)
		  exc.retval = HUGE_D758;
		else
		  exc.retval = HUGE_VAL_D758;
		if (_LIB_VERSION_D758 == 2)
		  errno_D758 = 34;
		else if (!matherr_D758(&exc)) {
			errno_D758 = 34;
		}
	        if (exc.err != 0)
	           errno_D758 = exc.err;
	        return exc.retval;
	    } else if(x<u_threshold_D758) {
		/* expf(finite) underflow */
		exc.type = 4;
		exc.name = "expf";
		exc.err = 0;
		exc.arg1 = exc.arg2 = (double)x;
		exc.retval = 0.0;
		if (_LIB_VERSION_D758 == 2)
		  errno_D758 = 34;
		else if (!matherr_D758(&exc)) {
			errno_D758 = 34;
		}
	        if (exc.err != 0)
	           errno_D758 = exc.err;
	        return exc.retval;
	    }
	}
	return z;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040D8D0);
#ifdef SKIP_ASM
/* newlib libm fmodf wrapper (wf_fmod.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
struct exception_D8D0 { int type; char *name; double arg1; double arg2; double retval; int err; };
enum fdlibm_version_D8D0 { fdlibm_ieee_D8D0 = -1, fdlibm_svid_D8D0, fdlibm_xopen_D8D0, fdlibm_posix_D8D0 };
extern const enum fdlibm_version_D8D0 fdlib_version_D8D0 __asm__("D_00499688");
extern float func_0040E300(float, float);                           /* __ieee754_fmodf */
extern int func_0040F9D0(float);                                    /* isnanf */
extern int matherr_D8D0(struct exception_D8D0 *) __asm__("func_0040F878");
#define _LIB_VERSION_D8D0 fdlib_version_D8D0
#define errno_D8D0 (*func_00415FB8())

	float func_0040D8D0(float x, float y)	/* wrapper fmodf */
{
	float z;
	struct exception_D8D0 exc;
	z = func_0040E300(x,y);
	if(_LIB_VERSION_D8D0 == fdlibm_ieee_D8D0 ||func_0040F9D0(y)||func_0040F9D0(x)) return z;
	if(y==(float)0.0) {
            /* fmodf(x,0) */
            exc.type = 1;
            exc.name = "fmodf";
	    exc.err = 0;
	    exc.arg1 = (double)x;
	    exc.arg2 = (double)y;
            if (_LIB_VERSION_D8D0 == fdlibm_svid_D8D0)
               exc.retval = x;
	    else
	       exc.retval = 0.0/0.0;
            if (_LIB_VERSION_D8D0 == fdlibm_posix_D8D0)
               errno_D8D0 = 33;
            else if (!matherr_D8D0(&exc)) {
                  errno_D8D0 = 33;
            }
	    if (exc.err != 0)
	       errno_D8D0 = exc.err;
            return (float)exc.retval;
	} else
	    return z;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040DA10);
#ifdef SKIP_ASM
/* newlib libm logf wrapper (wf_log.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
struct exception_DA10 { int type; char *name; double arg1; double arg2; double retval; int err; };
enum fdlibm_version_DA10 { fdlibm_ieee_DA10 = -1, fdlibm_svid_DA10, fdlibm_xopen_DA10, fdlibm_posix_DA10 };
extern const enum fdlibm_version_DA10 fdlib_version_DA10 __asm__("D_00499688");
extern float func_0040E550(float);                                  /* __ieee754_logf */
extern int func_0040F9D0(float);                                    /* isnanf */
extern int matherr_DA10(struct exception_DA10 *) __asm__("func_0040F878");
#define _LIB_VERSION_DA10 fdlib_version_DA10
#define errno_DA10 (*func_00415FB8())
#define HUGE_DA10 ((float)3.40282346638528860e+38)
#define HUGE_VAL_DA10 (D_00499680[0])

	float func_0040DA10(float x)		/* wrapper logf */
{
	float z;
	struct exception_DA10 exc;
	z = func_0040E550(x);
	if(_LIB_VERSION_DA10 == fdlibm_ieee_DA10 || func_0040F9D0(x) || x > (float)0.0) return z;
	exc.name = "logf";
	exc.err = 0;
	exc.arg1 = exc.arg2 = (double)x;
	if (_LIB_VERSION_DA10 == fdlibm_svid_DA10)
           exc.retval = -HUGE_DA10;
	else
	   exc.retval = -HUGE_VAL_DA10;
	if(x==(float)0.0) {
	    /* logf(0) */
	    exc.type = 2;
	    if (_LIB_VERSION_DA10 == fdlibm_posix_DA10)
	       errno_DA10 = 34;
	    else if (!matherr_DA10(&exc)) {
	       errno_DA10 = 33;
	    }
	} else {
	    /* logf(x<0) */
	    exc.type = 1;
	    if (_LIB_VERSION_DA10 == fdlibm_posix_DA10)
	       errno_DA10 = 33;
	    else if (!matherr_DA10(&exc)) {
	       errno_DA10 = 33;
	    }
        }
	if (exc.err != 0)
           errno_DA10 = exc.err;
        return (float)exc.retval;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040DB58);
#ifdef SKIP_ASM
/* newlib libm __ieee754_log (e_log.c; fdlibm; notice at func_0040F840) */
/* newlib libm's word-access helpers (fdlibm.h), little-endian layout, verbatim */
typedef union
{
  double value;
  struct
  {
    unsigned int lsw;
    unsigned int msw;
  } parts;
} ieee_double_shape_type_DB58;
#define EXTRACT_WORDS_DB58(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type_DB58 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define GET_HIGH_WORD_DB58(i,d)					\
do {								\
  ieee_double_shape_type_DB58 gh_u;				\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)
#define SET_HIGH_WORD_DB58(d,v)					\
do {								\
  ieee_double_shape_type_DB58 sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

static const double
ln2_hi_DB58  =  6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
ln2_lo_DB58  =  1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
two54_DB58   =  1.80143985094819840000e+16,  /* 43500000 00000000 */
Lg1_DB58 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
Lg2_DB58 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
Lg3_DB58 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
Lg4_DB58 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
Lg5_DB58 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
Lg6_DB58 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
Lg7_DB58 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

static const double zero_DB58   =  0.0;

	double func_0040DB58(double x)
{
	double hfsq,frac,ratio,z,poly,w,evenTerms,oddTerms,dk;
	int k,hx,i,j;
	unsigned int lx;

	EXTRACT_WORDS_DB58(hx,lx,x);

	k=0;
	if (hx < 0x00100000) {			/* x < 2**-1022  */
	    if (((hx&0x7fffffff)|lx)==0)
		return -two54_DB58/zero_DB58;		/* log(+-0)=-inf */
	    if (hx<0) return (x-x)/zero_DB58;	/* log(-#) = NaN */
	    k -= 54; x *= two54_DB58; /* subnormal number, scale up x */
	    GET_HIGH_WORD_DB58(hx,x);
	}
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	hx &= 0x000fffff;
	i = (hx+0x95f64)&0x100000;
	SET_HIGH_WORD_DB58(x,hx|(i^0x3ff00000));	/* normalize x or x/2 */
	k += (i>>20);
	frac = x-1.0;
	if((0x000fffff&(2+hx))<3) {	/* |f| < 2**-20 */
          if(frac==zero_DB58) { if(k==0) return zero_DB58;  else {dk=(double)k;
                               return dk*ln2_hi_DB58+dk*ln2_lo_DB58;}}
	    poly = frac*frac*(0.5-0.33333333333333333*frac);
	    if(k==0) return frac-poly; else {dk=(double)k;
	    	     return dk*ln2_hi_DB58-((poly-dk*ln2_lo_DB58)-frac);}
	}
 	ratio = frac/(2.0+frac);
	dk = (double)k;
	z = ratio*ratio;
	i = hx-0x6147a;
	w = z*z;
	j = 0x6b851-hx;
	evenTerms= w*(Lg2_DB58+w*(Lg4_DB58+w*Lg6_DB58));
	oddTerms= z*(Lg1_DB58+w*(Lg3_DB58+w*(Lg5_DB58+w*Lg7_DB58)));
	i |= j;
	poly = oddTerms+evenTerms;
	if(i>0) {
	    hfsq=0.5*frac*frac;
	    if(k==0) return frac-(hfsq-ratio*(hfsq+poly)); else
		     return dk*ln2_hi_DB58-((hfsq-(ratio*(hfsq+poly)+dk*ln2_lo_DB58))-frac);
	} else {
	    if(k==0) return frac-ratio*(frac-poly); else
		     return dk*ln2_hi_DB58-((ratio*(frac-poly)-dk*ln2_lo_DB58)-frac);
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040E048);
#ifdef SKIP_ASM
/* newlib libm __ieee754_expf (ef_exp.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
/* newlib libm's word-access helpers (fdlibm.h), verbatim */
typedef union
{
  float value;
  unsigned int word;
} ieee_float_shape_type_E048;
#define GET_FLOAT_WORD_E048(i,d)				\
do {								\
  ieee_float_shape_type_E048 gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)
#define SET_FLOAT_WORD_E048(d,i)				\
do {								\
  ieee_float_shape_type_E048 sf_u;				\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

/* the file's static tables, at their retail addresses */
extern const float D_004991A0[];	/* halF[2], then huge, twom100, o_threshold, u_threshold */
extern const float D_004991B8[];	/* ln2HI[2] */
extern const float D_004991C0[];	/* ln2LO[2], then invln2, P1..P5 */
#define halF_E048 D_004991A0
#define ln2HI_E048 D_004991B8
#define ln2LO_E048 D_004991C0
static const float
one_E048	= 1.0,
huge_E048	= 1.0e+30,
twom100_E048 = 7.8886090522e-31,      /* 2**-100=0x0d800000 */
o_threshold_E048=  8.8721679688e+01,  /* 0x42b17180 */
u_threshold_E048= -1.0397208405e+02,  /* 0xc2cff1b5 */
invln2_E048 =  1.4426950216e+00, 		/* 0x3fb8aa3b */
P1_E048   =  1.6666667163e-01, /* 0x3e2aaaab */
P2_E048   = -2.7777778450e-03, /* 0xbb360b61 */
P3_E048   =  6.6137559770e-05, /* 0x388ab355 */
P4_E048   = -1.6533901999e-06, /* 0xb5ddea0e */
P5_E048   =  4.1381369442e-08; /* 0x3331bb4c */

	float func_0040E048(float x)	/* default IEEE double exp */
{
	float y,hi,lo,c,t;
	int k,xsb;
	unsigned int hx;

	GET_FLOAT_WORD_E048(hx,x);
	xsb = (hx>>31)&1;		/* sign bit of x */
	hx &= 0x7fffffff;		/* high word of |x| */

    /* filter out non-finite argument */
	if(hx >= 0x42b17218) {			/* if |x|>=88.721... */
	    if(hx>0x7f800000)
		 return x+x;	 		/* NaN */
            if(hx==0x7f800000)
		return (xsb==0)? x:0.0;		/* exp(+-inf)={inf,0} */
	    if(x > o_threshold_E048) return huge_E048*huge_E048; /* overflow */
	    if(x < u_threshold_E048) return twom100_E048*twom100_E048; /* underflow */
	}

    /* argument reduction */
	if(hx > 0x3eb17218) {		/* if  |x| > 0.5 ln2 */
	    if(hx < 0x3F851592) {	/* and |x| < 1.5 ln2 */
		hi = x-ln2HI_E048[xsb]; lo=ln2LO_E048[xsb]; k = 1-xsb-xsb;
	    } else {
		k  = invln2_E048*x+halF_E048[xsb];
		t  = k;
		hi = x - t*ln2HI_E048[0];	/* t*ln2HI is exact here */
		lo = t*ln2LO_E048[0];
	    }
	    x  = hi - lo;
	}
	else if(hx < 0x31800000)  {	/* when |x|<2**-28 */
	    if(huge_E048+x>one_E048) return one_E048+x;/* trigger inexact */
	}
	else k = 0;

    /* x is now in primary range */
	t  = x*x;
	c  = x - t*(P1_E048+t*(P2_E048+t*(P3_E048+t*(P4_E048+t*P5_E048))));
	if(k==0) 	return one_E048-((x*c)/(c-(float)2.0)-x);
	else 		y = one_E048-((lo-(x*c)/((float)2.0-c))-hi);
	if(k >= -125) {
	    unsigned int hy;
	    GET_FLOAT_WORD_E048(hy,y);
	    SET_FLOAT_WORD_E048(y,hy+(k<<23));	/* add k to y's exponent */
	    return y;
	} else {
	    unsigned int hy;
	    GET_FLOAT_WORD_E048(hy,y);
	    SET_FLOAT_WORD_E048(y,hy+((k+100)<<23));	/* add k to y's exponent */
	    return y*twom100_E048;
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040E300);
#ifdef SKIP_ASM
// FAKEMATCH: `while((unsigned int)n-- > 0)` for fdlibm's `while(n--)` (the same test for every n); the plain
// form is strength-reduced to `--n != -1`; plain form 93.24%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
/* newlib libm __ieee754_fmodf (ef_fmod.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
/* newlib libm's word-access helpers (fdlibm.h), verbatim */
typedef union
{
  float value;
  unsigned int word;
} ieee_float_shape_type_E300;
#define GET_FLOAT_WORD_E300(i,d)				\
do {								\
  ieee_float_shape_type_E300 gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)
#define SET_FLOAT_WORD_E300(d,i)				\
do {								\
  ieee_float_shape_type_E300 sf_u;				\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

static const float one_E300 = 1.0;
extern const float D_004991F0[];	/* Zero[] = {0.0, -0.0} */
#define Zero_E300 D_004991F0

	float func_0040E300(float x, float y)
{
	int n,hx,hy,hz,ix,iy,sx,i;

	GET_FLOAT_WORD_E300(hx,x);
	GET_FLOAT_WORD_E300(hy,y);
	sx = hx&0x80000000;		/* sign of x */
	hx ^=sx;		/* |x| */
	hy &= 0x7fffffff;	/* |y| */

    /* purge off exception values */
	if(hy==0||(hx>=0x7f800000)||		/* y=0,or x not finite */
	   (hy>0x7f800000))			/* or y is NaN */
	    return (x*y)/(x*y);
	if(hx<hy) return x;			/* |x|<|y| return x */
	if(hx==hy)
	    return Zero_E300[(unsigned int)sx>>31];	/* |x|=|y| return x*0*/

    /* determine ix = ilogb(x) */
	if(hx<0x00800000) {	/* subnormal x */
	    for (ix = -126,i=(hx<<8); i>0; i<<=1) ix -=1;
	} else ix = (hx>>23)-127;

    /* determine iy = ilogb(y) */
	if(hy<0x00800000) {	/* subnormal y */
	    for (iy = -126,i=(hy<<8); i>=0; i<<=1) iy -=1;
	} else iy = (hy>>23)-127;

    /* set up {hx,lx}, {hy,ly} and align y to x */
	if(ix >= -126)
	    hx = 0x00800000|(0x007fffff&hx);
	else {		/* subnormal x, shift x to normal */
	    n = -126-ix;
	    hx = hx<<n;
	}
	if(iy >= -126)
	    hy = 0x00800000|(0x007fffff&hy);
	else {		/* subnormal y, shift y to normal */
	    n = -126-iy;
	    hy = hy<<n;
	}

    /* fix point fmod */
	n = ix - iy;
	while((unsigned int)n-- > 0) {
	    hz=hx-hy;
	    if(hz<0){hx = hx+hx;}
	    else {
	    	if(hz==0) 		/* return sign(x)*0 */
		    return Zero_E300[(unsigned int)sx>>31];
	    	hx = hz+hz;
	    }
	}
	hz=hx-hy;
	if(hz>=0) {hx=hz;}

    /* convert back to floating value and restore the sign */
	if(hx==0) 			/* return sign(x)*0 */
	    return Zero_E300[(unsigned int)sx>>31];
	while(hx<0x00800000) {		/* normalize x */
	    hx = hx+hx;
	    iy -= 1;
	}
	if(iy>= -126) {		/* normalize output */
	    hx = ((hx-0x00800000)|((iy+127)<<23));
	    SET_FLOAT_WORD_E300(x,hx|sx);
	} else {		/* subnormal output */
	    n = -126 - iy;
	    hx >>= n;
	    SET_FLOAT_WORD_E300(x,hx|sx);
	    x *= one_E300;		/* create necessary signal */
	}
	return x;		/* exact output */
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040E550);
#ifdef SKIP_ASM
/* newlib libm __ieee754_logf (ef_log.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
/* newlib libm's word-access helpers (fdlibm.h), verbatim */
typedef union
{
  float value;
  unsigned int word;
} ieee_float_shape_type_E550;
#define GET_FLOAT_WORD_E550(i,d)				\
do {								\
  ieee_float_shape_type_E550 gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)
#define SET_FLOAT_WORD_E550(d,i)				\
do {								\
  ieee_float_shape_type_E550 sf_u;				\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

static const float
ln2_hi_E550 =   6.9313812256e-01,	/* 0x3f317180 */
ln2_lo_E550 =   9.0580006145e-06,	/* 0x3717f7d1 */
two25_E550 =    3.355443200e+07,	/* 0x4c000000 */
Lg1_E550 = 6.6666668653e-01,	/* 3F2AAAAB */
Lg2_E550 = 4.0000000596e-01,	/* 3ECCCCCD */
Lg3_E550 = 2.8571429849e-01, /* 3E924925 */
Lg4_E550 = 2.2222198546e-01, /* 3E638E29 */
Lg5_E550 = 1.8183572590e-01, /* 3E3A3325 */
Lg6_E550 = 1.5313838422e-01, /* 3E1CD04F */
Lg7_E550 = 1.4798198640e-01; /* 3E178897 */

static const float zero_E550   =  0.0;

	float func_0040E550(float x)
{
	float hfsq,frac,ratio,z,poly,w,evenTerms,oddTerms,dk;
	int k,ix,i,j;

	GET_FLOAT_WORD_E550(ix,x);

	k=0;
	if (ix < 0x00800000) {			/* x < 2**-126  */
	    if ((ix&0x7fffffff)==0)
		return -two25_E550/zero_E550;		/* log(+-0)=-inf */
	    if (ix<0) return (x-x)/zero_E550;	/* log(-#) = NaN */
	    k -= 25; x *= two25_E550; /* subnormal number, scale up x */
	    GET_FLOAT_WORD_E550(ix,x);
	}
	if (ix >= 0x7f800000) return x+x;
	k += (ix>>23)-127;
	ix &= 0x007fffff;
	i = (ix+(0x95f64<<3))&0x800000;
	SET_FLOAT_WORD_E550(x,ix|(i^0x3f800000));	/* normalize x or x/2 */
	k += (i>>23);
	frac = x-(float)1.0;
	if((0x007fffff&(15+ix))<16) {	/* |f| < 2**-20 */
           if(frac==zero_E550) { if(k==0) return zero_E550;  else {dk=(float)k;
                                return dk*ln2_hi_E550+dk*ln2_lo_E550;}}
	    poly = frac*frac*((float)0.5-(float)0.33333333333333333*frac);
	    if(k==0) return frac-poly; else {dk=(float)k;
	    	     return dk*ln2_hi_E550-((poly-dk*ln2_lo_E550)-frac);}
	}
 	ratio = frac/((float)2.0+frac);
	dk = (float)k;
	z = ratio*ratio;
	i = ix-(0x6147a<<3);
	w = z*z;
	j = (0x6b851<<3)-ix;
	evenTerms= w*(Lg2_E550+w*(Lg4_E550+w*Lg6_E550));
	oddTerms= z*(Lg1_E550+w*(Lg3_E550+w*(Lg5_E550+w*Lg7_E550)));
	i |= j;
	poly = oddTerms+evenTerms;
	if(i>0) {
	    hfsq=(float)0.5*frac*frac;
	    if(k==0) return frac-(hfsq-ratio*(hfsq+poly)); else
		     return dk*ln2_hi_E550-((hfsq-(ratio*(hfsq+poly)+dk*ln2_lo_E550))-frac);
	} else {
	    if(k==0) return frac-ratio*(frac-poly); else
		     return dk*ln2_hi_E550-((ratio*(frac-poly)-dk*ln2_lo_E550)-frac);
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040E878);
#ifdef SKIP_ASM
/* newlib libm __ieee754_rem_pio2f (ef_rem_pio2.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
/* newlib libm's word-access helpers (fdlibm.h), verbatim */
typedef union
{
  float value;
  unsigned int word;
} ieee_float_shape_type_E878;
#define GET_FLOAT_WORD_E878(i,d)				\
do {								\
  ieee_float_shape_type_E878 gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)
#define SET_FLOAT_WORD_E878(d,i)				\
do {								\
  ieee_float_shape_type_E878 sf_u;				\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

extern float func_0040F8A0(float);	/* fabsf */
extern int func_0040EC58(float *, float *, int, int, int, const int *);	/* __kernel_rem_pio2f */
/* the file's static tables, at their retail addresses */
extern const int D_00499228[];	/* two_over_pi[] */
extern const int D_00499540[];	/* npio2_hw[] */
#define two_over_pi_E878 D_00499228
#define npio2_hw_E878 D_00499540

static const float
zero_E878 =  0.0000000000e+00, /* 0x00000000 */
half_E878 =  5.0000000000e-01, /* 0x3f000000 */
two8_E878 =  2.5600000000e+02, /* 0x43800000 */
invpio2_E878 =  6.3661980629e-01, /* 0x3f22f984 */
pio2_1_E878  =  1.5707855225e+00, /* 0x3fc90f80 */
pio2_1t_E878 =  1.0804334124e-05, /* 0x37354443 */
pio2_2_E878  =  1.0804273188e-05, /* 0x37354400 */
pio2_2t_E878 =  6.0770999344e-11, /* 0x2e85a308 */
pio2_3_E878  =  6.0770943833e-11, /* 0x2e85a300 */
pio2_3t_E878 =  6.1232342629e-17; /* 0x248d3132 */

	int func_0040E878(float x, float *y)
{
	float z,w,t,r,fn;
	float tx[3];
	int i,j,n,ix,hx;
	int e0,nx;

	GET_FLOAT_WORD_E878(hx,x);
	ix = hx&0x7fffffff;
	if(ix<=0x3f490fd8)   /* |x| ~<= pi/4 , no need for reduction */
	    {y[0] = x; y[1] = 0; return 0;}
	if(ix<0x4016cbe4) {  /* |x| < 3pi/4, special case with n=+-1 */
	    if(hx>0) {
		z = x - pio2_1_E878;
		if((ix&0xfffffff0)!=0x3fc90fd0) { /* 24+24 bit pi OK */
		    y[0] = z - pio2_1t_E878;
		    y[1] = (z-y[0])-pio2_1t_E878;
		} else {		/* near pi/2, use 24+24+24 bit pi */
		    z -= pio2_2_E878;
		    y[0] = z - pio2_2t_E878;
		    y[1] = (z-y[0])-pio2_2t_E878;
		}
		return 1;
	    } else {	/* negative x */
		z = x + pio2_1_E878;
		if((ix&0xfffffff0)!=0x3fc90fd0) { /* 24+24 bit pi OK */
		    y[0] = z + pio2_1t_E878;
		    y[1] = (z-y[0])+pio2_1t_E878;
		} else {		/* near pi/2, use 24+24+24 bit pi */
		    z += pio2_2_E878;
		    y[0] = z + pio2_2t_E878;
		    y[1] = (z-y[0])+pio2_2t_E878;
		}
		return -1;
	    }
	}
	if(ix<=0x43490f80) { /* |x| ~<= 2^7*(pi/2), medium size */
	    t  = func_0040F8A0(x);
	    n  = (int) (t*invpio2_E878+half_E878);
	    fn = (float)n;
	    r  = t-fn*pio2_1_E878;
	    w  = fn*pio2_1t_E878;	/* 1st round good to 40 bit */
	    if(n<32&&(ix&0xffffff00)!=npio2_hw_E878[n-1]) {
		y[0] = r-w;	/* quick check no cancellation */
	    } else {
	        unsigned int high;
	        j  = ix>>23;
	        y[0] = r-w;
		GET_FLOAT_WORD_E878(high,y[0]);
	        i = j-((high>>23)&0xff);
	        if(i>8) {  /* 2nd iteration needed, good to 57 */
		    t  = r;
		    w  = fn*pio2_2_E878;
		    r  = t-w;
		    w  = fn*pio2_2t_E878-((t-r)-w);
		    y[0] = r-w;
		    GET_FLOAT_WORD_E878(high,y[0]);
		    i = j-((high>>23)&0xff);
		    if(i>25)  {	/* 3rd iteration need, 74 bits acc */
		    	t  = r;	/* will cover all possible cases */
		    	w  = fn*pio2_3_E878;
		    	r  = t-w;
		    	w  = fn*pio2_3t_E878-((t-r)-w);
		    	y[0] = r-w;
		    }
		}
	    }
	    y[1] = (r-y[0])-w;
	    if(hx<0) 	{y[0] = -y[0]; y[1] = -y[1]; return -n;}
	    else	 return n;
	}
    /*
     * all other (large) arguments
     */
	if(ix>=0x7f800000) {		/* x is inf or NaN */
	    y[0]=y[1]=x-x; return 0;
	}
    /* set z = scalbn(|x|,ilogb(x)-7) */
	e0 	= (int)((ix>>23)-134);	/* e0 = ilogb(z)-7; */
	SET_FLOAT_WORD_E878(z, ix - ((int)e0<<23));
	for(i=0;i<2;i++) {
		tx[i] = (float)((int)(z));
		z     = (z-tx[i])*two8_E878;
	}
	tx[2] = z;
	nx = 3;
	while(tx[nx-1]==zero_E878) nx--;	/* skip zero term */
	n  =  func_0040EC58(tx,y,e0,nx,2,two_over_pi_E878);
	if(hx<0) {y[0] = -y[0]; y[1] = -y[1]; return -n;}
	return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040EC58);
#ifdef SKIP_ASM
/* newlib libm __kernel_rem_pio2f (kf_rem_pio2.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
extern float func_0040F9F8(float, int);	/* scalbnf */
extern float func_0040F8E8(float);	/* floorf */
#define scalbnf_EC58 func_0040F9F8
#define floorf_EC58 func_0040F8E8
/* the file's static tables, at their retail addresses */
extern const int D_004995E8[];	/* init_jk[] = {4,7,9} */
extern const float D_004995F8[];	/* PIo2[] */
#define init_jk_EC58 D_004995E8
#define PIo2_EC58 D_004995F8

static const float
zero_EC58   = 0.0,
one_EC58    = 1.0,
two8_EC58   =  2.5600000000e+02, /* 0x43800000 */
twon8_EC58  =  3.9062500000e-03; /* 0x3b800000 */

	int func_0040EC58(float *x, float *y, int e0, int nx, int prec, const int *ipio2)
{
	int jz,jx,jv,jp,jk,carry,n,iq[20],i,j,k,m,q0,ih;
	float z,fw,f[20],fq[20],q[20];

    /* initialize jk*/
	jk = init_jk_EC58[prec];
	jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
	jx =  nx-1;
	jv = (e0-3)/8; if(jv<0) jv=0;
	q0 =  e0-8*(jv+1);

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
	j = jv-jx; m = jx+jk;
	for(i=0;i<=m;i++,j++) f[i] = (j<0)? zero_EC58 : (float) ipio2[j];

    /* compute q[0],q[1],...q[jk] */
	for (i=0;i<=jk;i++) {
	    for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*f[jx+i-j]; q[i] = fw;
	}

	jz = jk;
recompute:
    /* distill q[] into iq[] reversingly */
	for(i=0,j=jz,z=q[jz];j>0;i++,j--) {
	    fw    =  (float)((int)(twon8_EC58* z));
	    iq[i] =  (int)(z-two8_EC58*fw);
	    z     =  q[j-1]+fw;
	}

    /* compute n */
	z  = scalbnf_EC58(z,(int)q0);	/* actual value of z */
	z -= (float)8.0*floorf_EC58(z*(float)0.125);	/* trim off integer >= 8 */
	n  = (int) z;
	z -= (float)n;
	ih = 0;
	if(q0>0) {	/* need iq[jz-1] to determine n */
	    i  = (iq[jz-1]>>(8-q0)); n += i;
	    iq[jz-1] -= i<<(8-q0);
	    ih = iq[jz-1]>>(7-q0);
	} 
	else if(q0==0) ih = iq[jz-1]>>8;
	else if(z>=(float)0.5) ih=2;

	if(ih>0) {	/* q > 0.5 */
	    n += 1; carry = 0;
	    for(i=0;i<jz ;i++) {	/* compute 1-q */
		j = iq[i];
		if(carry==0) {
		    if(j!=0) {
			carry = 1; iq[i] = 0x100- j;
		    }
		} else  iq[i] = 0xff - j;
	    }
	    if(q0>0) {		/* rare case: chance is 1 in 12 */
	        switch(q0) {
	        case 1:
	    	   iq[jz-1] &= 0x7f; break;
	    	case 2:
	    	   iq[jz-1] &= 0x3f; break;
	        }
	    }
	    if(ih==2) {
		z = one_EC58 - z;
		if(carry!=0) z -= scalbnf_EC58(one_EC58,(int)q0);
	    }
	}

    /* check if recomputation is needed */
	if(z==zero_EC58) {
	    j = 0;
	    for (i=jz-1;i>=jk;i--) j |= iq[i];
	    if(j==0) { /* need recomputation */
		for(k=1;iq[jk-k]==0;k++);   /* k = no. of terms needed */

		for(i=jz+1;i<=jz+k;i++) {   /* add q[jz+1] to q[jz+k] */
		    f[jx+i] = (float) ipio2[jv+i];
		    for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*f[jx+i-j];
		    q[i] = fw;
		}
		jz += k;
		goto recompute;
	    }
	}

    /* chop off zero terms */
	if(z==(float)0.0) {
	    jz -= 1; q0 -= 8;
	    while(iq[jz]==0) { jz--; q0-=8;}
	} else { /* break z into 8-bit if necessary */
	    z = scalbnf_EC58(z,-(int)q0);
	    if(z>=two8_EC58) { 
		fw = (float)((int)(twon8_EC58*z));
		iq[jz] = (int)(z-two8_EC58*fw);
		jz += 1; q0 += 8;
		iq[jz] = (int) fw;
	    } else iq[jz] = (int) z ;
	}

    /* convert integer "bit" chunk to floating-point value */
	fw = scalbnf_EC58(one_EC58,(int)q0);
	for(i=jz;i>=0;i--) {
	    q[i] = fw*(float)iq[i]; fw*=twon8_EC58;
	}

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
	for(i=jz;i>=0;i--) {
	    for(fw=0.0,k=0;k<=jp&&k<=jz-i;k++) fw += PIo2_EC58[k]*q[i+k];
	    fq[jz-i] = fw;
	}

    /* compress fq[] into y[] */
	switch(prec) {
	    case 0:
		fw = 0.0;
		for (i=jz;i>=0;i--) fw += fq[i];
		y[0] = (ih==0)? fw: -fw; 
		break;
	    case 1:
	    case 2:
		fw = 0.0;
		for (i=jz;i>=0;i--) fw += fq[i]; 
		y[0] = (ih==0)? fw: -fw; 
		fw = fq[0]-fw;
		for (i=1;i<=jz;i++) fw += fq[i];
		y[1] = (ih==0)? fw: -fw; 
		break;
	    case 3:	/* painful */
		for (i=jz;i>0;i--) {
		    fw      = fq[i-1]+fq[i]; 
		    fq[i]  += fq[i-1]-fw;
		    fq[i-1] = fw;
		}
		for (i=jz;i>1;i--) {
		    fw      = fq[i-1]+fq[i]; 
		    fq[i]  += fq[i-1]-fw;
		    fq[i-1] = fw;
		}
		for (fw=0.0,i=jz;i>=2;i--) fw += fq[i]; 
		if(ih==0) {
		    y[0] =  fq[0]; y[1] =  fq[1]; y[2] =  fw;
		} else {
		    y[0] = -fq[0]; y[1] = -fq[1]; y[2] = -fw;
		}
	}
	return n&7;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F5A8);
#ifdef SKIP_ASM
/* newlib libm __kernel_tanf (kf_tan.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
/* newlib libm's word-access helpers (fdlibm.h), verbatim */
typedef union
{
  float value;
  unsigned int word;
} ieee_float_shape_type_F5A8;
#define GET_FLOAT_WORD_F5A8(i,d)				\
do {								\
  ieee_float_shape_type_F5A8 gf_u;				\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)
#define SET_FLOAT_WORD_F5A8(d,i)				\
do {								\
  ieee_float_shape_type_F5A8 sf_u;				\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

extern float func_0040F8A0(float);	/* fabsf */
#define fabsf_F5A8 func_0040F8A0
/* the file's static table, at its retail address */
extern const float D_00499648[];	/* T[13] */
#define T_F5A8 D_00499648

static const float
one_F5A8   =  1.0000000000e+00, /* 0x3f800000 */
pio4_F5A8  =  7.8539812565e-01, /* 0x3f490fda */
pio4lo_F5A8=  3.7748947079e-08; /* 0x33222168 */

	float func_0040F5A8(float x, float y, int iy)
{
	float z,r,v,w,s;
	int ix,hx;
	GET_FLOAT_WORD_F5A8(hx,x);
	ix = hx&0x7fffffff;	/* high word of |x| */
	if(ix<0x31800000)			/* x < 2**-28 */
	    {if((int)x==0) {			/* generate inexact */
		if((ix|(iy+1))==0) return one_F5A8/fabsf_F5A8(x);
		else return (iy==1)? x: -one_F5A8/x;
	    }
	    }
	if(ix>=0x3f2ca140) { 			/* |x|>=0.6744 */
	    if(hx<0) {x = -x; y = -y;}
	    z = pio4_F5A8-x;
	    w = pio4lo_F5A8-y;
	    x = z+w; y = 0.0;
	}
	z	=  x*x;
	w 	=  z*z;
    /* Break x^5*(T[1]+x^2*T[2]+...) into
     *	  x^5(T[1]+x^4*T[3]+...+x^20*T[11]) +
     *	  x^5(x^2*(T[2]+x^4*T[4]+...+x^22*[T12]))
     */
	r = T_F5A8[1]+w*(T_F5A8[3]+w*(T_F5A8[5]+w*(T_F5A8[7]+w*(T_F5A8[9]+w*T_F5A8[11]))));
	v = z*(T_F5A8[2]+w*(T_F5A8[4]+w*(T_F5A8[6]+w*(T_F5A8[8]+w*(T_F5A8[10]+w*T_F5A8[12])))));
	s = z*x;
	r = y + z*(s*(r+v)+y);
	r += T_F5A8[0]*s;
	w = x+r;
	if(ix>=0x3f2ca140) {
	    v = (float)iy;
	    return (float)(1-((hx>>30)&2))*(v-(float)2.0*(x-(w*w/(w+v)-r)));
	}
	if(iy==1) return w;
	else {		/* if allow error up to 2 ulp,
			   simply return -1.0/(x+r) here */
     /*  compute -1.0/(x+r) accurately */
	    float a,t;
	    int i;
	    z  = w;
	    GET_FLOAT_WORD_F5A8(i,z);
	    SET_FLOAT_WORD_F5A8(z,i&0xfffff000);
	    v  = r-(z - x); 	/* z+v = r+x */
	    t = a  = -(float)1.0/w;	/* a = -1.0/w */
	    GET_FLOAT_WORD_F5A8(i,t);
	    SET_FLOAT_WORD_F5A8(t,i&0xfffff000);
	    s  = (float)1.0+t*z;
	    return t+a*(s+t*v);
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F840);
#ifdef SKIP_ASM
/* newlib libm isnan (fdlibm s_isnan.c): the EE has no double hardware, so the double arrives in one 64-bit GPR.
 * The bodies of isnan, finitef (func_0040F8C0) and isnanf (func_0040F9D0) and the word-access macros are fdlibm's:
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this software is freely granted, provided that this notice
 * is preserved.
 */
/* newlib libm (fdlibm.h), little-endian layout, verbatim */
typedef union {
    double value;
    struct {
        unsigned int lsw;
        unsigned int msw;
    } parts;
} ieee_double_shape_type;
#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
int func_0040F840(double x) {
    int hx, lx;
    EXTRACT_WORDS(hx, lx, x);
    hx &= 0x7fffffff;
    hx |= (unsigned int)(lx | (-lx)) >> 31;
    hx = 0x7ff00000 - hx;
    return (int)(((unsigned int)(hx)) >> 31);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F878);
#ifdef SKIP_ASM
extern void dpcmp(long, long);
int func_0040F878(long *errInfo) {
    long firstArg = errInfo[1];
    dpcmp(firstArg, firstArg);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F8A0);
#ifdef SKIP_ASM
typedef union { float f; int bits; } FloatBits_F8A0;
#define FLOAT_TO_BITS_F8A0(i, d) do { FloatBits_F8A0 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
#define BITS_TO_FLOAT_F8A0(d, i) do { FloatBits_F8A0 fb_; fb_.bits = (i); (d) = fb_.f; } while (0)
float func_0040F8A0(float x) {
    int ix;
    FLOAT_TO_BITS_F8A0(ix, x);
    BITS_TO_FLOAT_F8A0(x, ix & 0x7fffffff);
    return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F8C0);
#ifdef SKIP_ASM
/* newlib libm finitef (fdlibm sf_finite.c; notice at func_0040F840) */
/* newlib libm's word-access helpers (fdlibm.h), verbatim: the do-while(0) is part of the library macro */
typedef union {
    float value;
    unsigned int word;
} ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)
int func_0040F8C0(float x) {
    int ix;
    GET_FLOAT_WORD(ix, x);
    return (int)((unsigned int)((ix & 0x7fffffff) - 0x7f800000) >> 31);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F8E8);
#ifdef SKIP_ASM
/* newlib libm floorf (sf_floor.c; fdlibm; notice at func_0040F840).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
/* GET_FLOAT_WORD and ieee_float_shape_type: defined with finitef (func_0040F8C0) above.
   SET_FLOAT_WORD: newlib libm's word-access helper (fdlibm.h), verbatim */
#define SET_FLOAT_WORD_F8E8(d,i)				\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

static const float huge_F8E8 = 1.0e30;

	float func_0040F8E8(float x)
{
	int i0,exponent;
	unsigned int i;
	GET_FLOAT_WORD(i0,x);
	exponent = ((i0>>23)&0xff)-0x7f;
	if(exponent<23) {
	    if(exponent<0) { 	/* raise inexact if x != 0 */
		if(huge_F8E8+x>(float)0.0) {/* return 0*sign(x) if |x|<1 */
		    if(i0>=0) {i0=0;}
		    else if((i0&0x7fffffff)!=0)
			{ i0=0xbf800000;}
		}
	    } else {
		i = (0x007fffff)>>exponent;
		if((i0&i)==0) return x; /* x is integral */
		if(huge_F8E8+x>(float)0.0) {	/* raise inexact flag */
		    if(i0<0) i0 += (0x00800000)>>exponent;
		    i0 &= (~i);
		}
	    }
	} else {
	    if(exponent==0x80) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	}
	SET_FLOAT_WORD_F8E8(x,i0);
	return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F9D0);
#ifdef SKIP_ASM
/* newlib libm isnanf (fdlibm sf_isnan.c; notice at func_0040F840) */
/* GET_FLOAT_WORD: defined with finitef (func_0040F8C0) above */
int func_0040F9D0(float x) {
    int ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;
    ix = 0x7f800000 - ix;
    return (int)(((unsigned int)(ix)) >> 31);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040F9F8);
#ifdef SKIP_ASM
extern float copysignf_F9F8(float, float) __asm__("func_0040FB58");
typedef union { float f; int bits; } FloatBits_F9F8;
#define FLOAT_TO_BITS_F9F8(i, d) do { FloatBits_F9F8 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
#define BITS_TO_FLOAT_F9F8(d, i) do { FloatBits_F9F8 fb_; fb_.bits = (i); (d) = fb_.f; } while (0)
static const float k_2pow25_F9F8 = 3.3554432e+07f, k_2powm25_F9F8 = 2.9802322e-08f, k_big_F9F8 = 1.0e+30f, k_small_F9F8 = 1.0e-30f;
float func_0040F9F8(float x, int n) {
    int k, ix;
    FLOAT_TO_BITS_F9F8(ix, x);
    k = (ix & 0x7f800000) >> 23;
    if (k == 0) {
        if ((ix & 0x7fffffff) == 0)
            return x;
        x *= k_2pow25_F9F8;
        FLOAT_TO_BITS_F9F8(ix, x);
        k = ((ix & 0x7f800000) >> 23) - 25;
        if (n < -50000)
            return k_small_F9F8 * x;
    }
    if (k == 0xff)
        return x + x;
    k = k + n;
    if (k > 0xfe)
        return k_big_F9F8 * copysignf_F9F8(k_big_F9F8, x);
    if (k > 0) {
        BITS_TO_FLOAT_F9F8(x, (ix & 0x807fffff) | (k << 23));
        return x;
    }
    if (k <= -25) {
        if (n > 50000)
            return k_big_F9F8 * copysignf_F9F8(k_big_F9F8, x);
        else
            return k_small_F9F8 * copysignf_F9F8(k_small_F9F8, x);
    }
    k += 25;
    BITS_TO_FLOAT_F9F8(x, (ix & 0x807fffff) | (k << 23));
    return x * k_2powm25_F9F8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040FB58);
#ifdef SKIP_ASM
typedef union { float f; int bits; } FloatBits_FB58;
#define FLOAT_TO_BITS_FB58(i, d) do { FloatBits_FB58 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
#define BITS_TO_FLOAT_FB58(d, i) do { FloatBits_FB58 fb_; fb_.bits = (i); (d) = fb_.f; } while (0)
float func_0040FB58(float x, float y) {
    int ix, iy;
    FLOAT_TO_BITS_FB58(ix, x);
    FLOAT_TO_BITS_FB58(iy, y);
    BITS_TO_FLOAT_FB58(x, (ix & 0x7fffffff) | (iy & 0x80000000));
    return x;
}
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040FBE0);
#ifdef SKIP_ASM
typedef void (*func_ptr)(void);
extern func_ptr D_0043CE38[0];



/* libgcc2's __do_global_ctors: DO_GLOBAL_CTORS_BODY from gcc's gbl-ctors.h, a do { } while (0) macro (the plain body
   scores 98.52; its loop notes shape the register allocation) */
#define DO_GLOBAL_CTORS_BODY_FBE0                                       \
do {                                                                    \
  unsigned long nptrs = (unsigned long) D_0043CE38[0];                  \
  unsigned i;                                                           \
  if (nptrs == (unsigned long)-1)                                       \
    for (nptrs = 0; D_0043CE38[nptrs + 1] != 0; nptrs++);               \
  for (i = nptrs; i >= 1; i--)                                          \
    D_0043CE38[i] ();                                                   \
} while (0)

void func_0040FBE0(void)
{
  DO_GLOBAL_CTORS_BODY_FBE0;
}
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

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040FCB0);
#ifdef SKIP_ASM
/* GNU CC runtime support, reconstructed: libgcc2.c __divdi3 with longlong.h. Original: Copyright (C) 1989, 92-98, 1999
   Free Software Foundation, Inc. Free software under the GNU General Public License, version 2 or (at your option) any
   later version, with this special exception: "if you link this library with other files, some of which are compiled
   with GCC, to produce an executable, this library does not by itself cause the resulting executable to be covered
   by the GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License." This block (declarations and function) stays
   under those terms; the repository's CC0 dedication does not apply to it (README, License). */
/* libgcc2.c + longlong.h shared text for the 64-bit integer routines (GCC runtime, GPL v2+ with the libgcc linking
   exception). Resolved for this target (mips r5900, little-endian, ee-gcc 2.9-ee-991111): */

/* tm.h */
#define BITS_PER_UNIT 8

/* libgcc2.c: mode-specific type names. (The source's `#define int bogus_type` ... guards are left out: this unit
   also holds code that uses the C type names.) */
typedef unsigned int UQItype	__attribute__ ((mode (QI)));
typedef 	 int SItype	__attribute__ ((mode (SI)));
typedef unsigned int USItype	__attribute__ ((mode (SI)));
typedef		 int DItype	__attribute__ ((mode (DI)));
typedef unsigned int UDItype	__attribute__ ((mode (DI)));

typedef int word_type __attribute__ ((mode (__word__)));

#define SI_TYPE_SIZE (sizeof (SItype) * BITS_PER_UNIT)

/* DIstructs are pairs of SItype values in the order determined by
   LIBGCC2_WORDS_BIG_ENDIAN.  */

/* LIBGCC2_WORDS_BIG_ENDIAN is 0 (little-endian EE) */
  struct DIstruct {SItype low, high;};

/* We need this union to unpack/pack DImode values, since we don't have
   any arithmetic yet.  Incoming DImode parameters are stored into the
   `ll' field, and the unpacked result is read from the struct `s'.  */

typedef union
{
  struct DIstruct s;
  DItype ll;
} DIunion;

/* longlong.h (SI_TYPE_SIZE is defined above, so longlong.h's default of 32 is not needed). */
#define __BITS4 (SI_TYPE_SIZE / 4)
#define __ll_B (1L << (SI_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((USItype) (t) % __ll_B)
#define __ll_highpart(t) ((USItype) (t) / __ll_B)

/* longlong.h, __mips__ */
// PORT: longlong.h umul_ppmm (MIPS multu); a port uses longlong.h's generic C version
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("multu %2,%3"						\
	   : "=l" ((USItype) (w0)),					\
	     "=h" ((USItype) (w1))					\
	   : "d" ((USItype) (u)),					\
	     "d" ((USItype) (v)))

/* longlong.h generic C macros (the mips section defines none of these) */
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do {									\
    USItype __x;							\
    __x = (al) - (bl);							\
    (sh) = (ah) - (bh) - (__x > (al));					\
    (sl) = __x;								\
  } while (0)

#define __umulsidi3(u, v) \
  ({DIunion __w;							\
    umul_ppmm (__w.s.high, __w.s.low, u, v);				\
    __w.ll; })

/* Define this unconditionally, so it can be used for debugging.  */
#define __udiv_qrnnd_c(q, r, n1, n0, d) \
  do {									\
    USItype __d1, __d0, __q1, __q0;					\
    USItype __r1, __r0, __m;						\
    __d1 = __ll_highpart (d);						\
    __d0 = __ll_lowpart (d);						\
									\
    __r1 = (n1) % __d1;							\
    __q1 = (n1) / __d1;							\
    __m = (USItype) __q1 * __d0;					\
    __r1 = __r1 * __ll_B | __ll_highpart (n0);				\
    if (__r1 < __m)							\
      {									\
	__q1--, __r1 += (d);						\
	if (__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */\
	  if (__r1 < __m)						\
	    __q1--, __r1 += (d);					\
      }									\
    __r1 -= __m;							\
									\
    __r0 = __r1 % __d1;							\
    __q0 = __r1 / __d1;							\
    __m = (USItype) __q0 * __d0;					\
    __r0 = __r0 * __ll_B | __ll_lowpart (n0);				\
    if (__r0 < __m)							\
      {									\
	__q0--, __r0 += (d);						\
	if (__r0 >= (d))						\
	  if (__r0 < __m)						\
	    __q0--, __r0 += (d);					\
      }									\
    __r0 -= __m;							\
									\
    (q) = (USItype) __q1 * __ll_B | __q0;				\
    (r) = __r0;								\
  } while (0)

/* If udiv_qrnnd was not defined for this processor, use __udiv_qrnnd_c.  */
/* (mips defines neither udiv_qrnnd nor sdiv_qrnnd) */
#define UDIV_NEEDS_NORMALIZATION 1
#define udiv_qrnnd __udiv_qrnnd_c

/* (mips defines no count_leading_zeros: the __clz_tab version; SI_TYPE_SIZE <= 32) */
#define count_leading_zeros(count, x) \
  do {									\
    USItype __xr = (x);							\
    USItype __a;							\
									\
    if (SI_TYPE_SIZE <= 32)						\
      {									\
	__a = __xr < ((USItype)1<<2*__BITS4)				\
	  ? (__xr < ((USItype)1<<__BITS4) ? 0 : __BITS4)		\
	  : (__xr < ((USItype)1<<3*__BITS4) ?  2*__BITS4 : 3*__BITS4);	\
      }									\
    else								\
      {									\
	for (__a = SI_TYPE_SIZE - 8; __a > 0; __a -= 8)			\
	  if (((__xr >> __a) & 0xff) != 0)				\
	    break;							\
      }									\
									\
    (count) = SI_TYPE_SIZE - (__clz_tab[__xr >> __a] + __a);		\
  } while (0)

/* libgcc2.c __divdi3 with its static inline __negdi2 and __udivmoddi4 (GCC runtime, GPL v2+ with the libgcc linking exception) */
/* The L_divdi3 object's own copy of libgcc2.c's `static const UQItype __clz_tab[]` (its rodata stays in asm). */
extern const UQItype __clz_tab[] __asm__("D_004996A0");

/* (L_divdi3: static inline) */
static inline
DItype
__negdi2 (DItype u)
{
  DIunion w;
  DIunion uu;

  uu.ll = u;

  w.s.low = -uu.s.low;
  w.s.high = -uu.s.high - ((USItype) w.s.low > 0);

  return w.ll;
}

/* (L_divdi3: static inline; UDIV_NEEDS_NORMALIZATION is 1, so the normalizing d1 == 0 branch) */
static inline
UDItype
__udivmoddi4 (UDItype n, UDItype d, UDItype *rp)
{
  DIunion ww;
  DIunion nn, dd;
  DIunion rr;
  USItype d0, d1, n0, n1, n2;
  USItype q0, q1;
  USItype b, bm;

  nn.ll = n;
  dd.ll = d;

  d0 = dd.s.low;
  d1 = dd.s.high;
  n0 = nn.s.low;
  n1 = nn.s.high;

  if (d1 == 0)
    {
      if (d0 > n1)
	{
	  /* 0q = nn / 0D */

	  count_leading_zeros (bm, d0);

	  if (bm != 0)
	    {
	      /* Normalize, i.e. make the most significant bit of the
		 denominator set.  */

	      d0 = d0 << bm;
	      n1 = (n1 << bm) | (n0 >> (SI_TYPE_SIZE - bm));
	      n0 = n0 << bm;
	    }

	  udiv_qrnnd (q0, n0, n1, n0, d0);
	  q1 = 0;

	  /* Remainder in n0 >> bm.  */
	}
      else
	{
	  /* qq = NN / 0d */

	  if (d0 == 0)
	    d0 = 1 / d0;	/* Divide intentionally by zero.  */

	  count_leading_zeros (bm, d0);

	  if (bm == 0)
	    {
	      /* From (n1 >= d0) /\ (the most significant bit of d0 is set),
		 conclude (the most significant bit of n1 is set) /\ (the
		 leading quotient digit q1 = 1).

		 This special case is necessary, not an optimization.
		 (Shifts counts of SI_TYPE_SIZE are undefined.)  */

	      n1 -= d0;
	      q1 = 1;
	    }
	  else
	    {
	      /* Normalize.  */

	      b = SI_TYPE_SIZE - bm;

	      d0 = d0 << bm;
	      n2 = n1 >> b;
	      n1 = (n1 << bm) | (n0 >> b);
	      n0 = n0 << bm;

	      udiv_qrnnd (q1, n1, n2, n1, d0);
	    }

	  /* n1 != d0...  */

	  udiv_qrnnd (q0, n0, n1, n0, d0);

	  /* Remainder in n0 >> bm.  */
	}

      if (rp != 0)
	{
	  rr.s.low = n0 >> bm;
	  rr.s.high = 0;
	  *rp = rr.ll;
	}
    }

  else
    {
      if (d1 > n1)
	{
	  /* 00 = nn / DD */

	  q0 = 0;
	  q1 = 0;

	  /* Remainder in n1n0.  */
	  if (rp != 0)
	    {
	      rr.s.low = n0;
	      rr.s.high = n1;
	      *rp = rr.ll;
	    }
	}
      else
	{
	  /* 0q = NN / dd */

	  count_leading_zeros (bm, d1);
	  if (bm == 0)
	    {
	      /* From (n1 >= d1) /\ (the most significant bit of d1 is set),
		 conclude (the most significant bit of n1 is set) /\ (the
		 quotient digit q0 = 0 or 1).

		 This special case is necessary, not an optimization.  */

	      /* The condition on the next line takes advantage of that
		 n1 >= d1 (true due to program flow).  */
	      if (n1 > d1 || n0 >= d0)
		{
		  q0 = 1;
		  sub_ddmmss (n1, n0, n1, n0, d1, d0);
		}
	      else
		q0 = 0;

	      q1 = 0;

	      if (rp != 0)
		{
		  rr.s.low = n0;
		  rr.s.high = n1;
		  *rp = rr.ll;
		}
	    }
	  else
	    {
	      USItype m1, m0;
	      /* Normalize.  */

	      b = SI_TYPE_SIZE - bm;

	      d1 = (d1 << bm) | (d0 >> b);
	      d0 = d0 << bm;
	      n2 = n1 >> b;
	      n1 = (n1 << bm) | (n0 >> b);
	      n0 = n0 << bm;

	      udiv_qrnnd (q0, n1, n2, n1, d1);
	      umul_ppmm (m1, m0, q0, d0);

	      if (m1 > n1 || (m1 == n1 && m0 > n0))
		{
		  q0--;
		  sub_ddmmss (m1, m0, m1, m0, d1, d0);
		}

	      q1 = 0;

	      /* Remainder in (n1n0 - m1m0) >> bm.  */
	      if (rp != 0)
		{
		  sub_ddmmss (n1, n0, n1, n0, m1, m0);
		  rr.s.low = (n1 << b) | (n0 >> bm);
		  rr.s.high = n1 >> bm;
		  *rp = rr.ll;
		}
	    }
	}
    }

  ww.s.low = q0;
  ww.s.high = q1;
  return ww.ll;
}

DItype __divdi3 (DItype u, DItype v) __asm__("func_0040FCB0");

UDItype __udivmoddi4 ();

DItype
__divdi3 (DItype dividend, DItype divisor)
{
  word_type negate = 0;
  DIunion uu, vv;
  DItype w;

  uu.ll = dividend;
  vv.ll = divisor;

  if (uu.s.high < 0)
    negate = ~negate,
    uu.ll = __negdi2 (uu.ll);
  if (vv.s.high < 0)
    negate = ~negate,
    vv.ll = __negdi2 (vv.ll);

  w = __udivmoddi4 (uu.ll, vv.ll, (UDItype *) 0);
  if (negate)
    w = __negdi2 (w);

  return w;
}
#endif
