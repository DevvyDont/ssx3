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
extern int sceSifCallRpc(sSifClient *cd, int fno, int mode, void *send, int ssize, void *recv, int rsize,
                         void (*end_function)(void *), void *end_param);
extern int sceSifCheckStatRpc(sSifClient *cd);

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
/**
 * Holds the DNAS module's import table: the body is 59 words of game function and data addresses (printf-style output,
 * thread, semaphore, cache, SIF RPC, alarm, interrupt and GS calls, several data blocks), followed by a bare return.
 *
 * func_003FCFD8 relocates the module's references into its "reloctable" symbol to the matching words here; the table
 * ends at func_003FD908.
 */
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
        ".word sceSifCallRpc\n"
        ".word sceOpen\n"
        ".word sceClose\n"
        ".word sceRead\n"
        ".word sceWrite\n"
        ".word sceLseek\n"
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

/**
 * Empty function that marks the end of the DNAS import table func_003FD810 (func_003FCFD8 uses the distance between the
 * two as the table size).
 */
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
/**
 * Resets or re-initialises the GS display (the state block D_00451550, see func_003FDAA0).
 *
 * Mode 0: resets the GS (CSR 0x200), stores the interlace, video mode and field/frame setting and the GS revision (CSR
 * bits 16..23), masks GS interrupts (GsPutIMR 0xFF00), removes the interrupt handler recorded in the block from INTC
 * line 2 and calls SetGsCrt. Mode 1: only flushes the GS (CSR 0x100). Mode 5: like mode 0 without the reset, the mask
 * and the handler removal. func_0037BD98 calls it.
 *
 * @param mode  0, 1 or 5
 * @param interlace  interlace flag (bit 0 goes to SetGsCrt)
 * @param videoMode  video mode
 * @param frameMode  nonzero for frame mode
 */
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
/**
 * Returns the GS state block D_00451550 (settings stored by func_003FD910; SNDPLATFORM_init reads it too).
 *
 * @return the block
 */
void *func_003FDAA0(void) {
    return D_00451550;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDAB0);
#ifdef SKIP_ASM
typedef unsigned int u_long128_DAB0 __attribute__((mode(TI))); /* PORT: Sony's u_long128 (eetypes.h) */
extern u_long128_DAB0 D_00451560[2]; /* VIF1 init codes sent to the VIF1 FIFO */
/**
 * Resets the drawing path to the GS: VIF1 reset with its errors masked, VU1 reset (FBRST bit 9), two quadwords from
 * D_00451560 into the VIF1 FIFO, then a GIF reset. cPS2GraphicsMan_Init calls it. Sony's libgraph sceGsResetPath.
 */
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
/**
 * Waits for (mode 0) or polls (mode 1) the DMA ch.1 and ch.2, VIF1, VU1 and GIF paths (libgraph).
 *
 * Mode 0 spins until DMA ch.1 and ch.2 CHCR bit 0x100 (STR) clear, VIF1_STAT & 0x1F000003, the VU1 busy bit 0x100 of
 * VPU_STAT and GIF_STAT & 0xC00 are all clear, one after the other, with one shared counter: past 0x1000000 polls it
 * prints "sceGsSyncPath: <path> does not terminate" and the DMA, VIF1 and GIF registers (func_00425C60) and returns -1.
 * Any other mode returns at once a bit mask of the paths still busy: 1 ch.1, 2 ch.2, 4 VIF1, 8 VU1, 0x10 GIF.
 *
 * @param mode     0 to wait, otherwise poll
 * @param timeout  not used: the wait limit is fixed at 0x1000000 polls
 * @return mode 0: 0, or -1 on a timeout; otherwise the busy mask
 */
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
/**
 * Waits for a DMA channel to finish (its CHCR STR bit to clear). After 0x1000000 polls it prints "libdma: sync timeout"
 * and force-stops the channel (func_003FE5C8), which ends the wait.
 *
 * @param dmaChan  the channel's registers
 * @param waitMode  ignored (func_003FE558 passes its mode)
 */
void func_003FDE30(dmaChan, waitMode)
    sDmaChan *dmaChan;
    int waitMode;
{
    int n = 0x1000000;
    if (dmaChan->chcr & 0x100) {
        do {
            if (--n < 0) {
                func_00425C60(D_004977D8);
                func_003FE5C8(dmaChan);
            }
        } while (dmaChan->chcr & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDEA0);
#ifdef SKIP_ASM
extern char D_004977D8[];
extern void func_003FDEA0_log(char *) __asm__("func_00425C60");
extern int func_003FE5C8();
/**
 * Waits until a DMA channel's MADR reaches @p lim.
 *
 * Polls up to @p tmo times (0x1000000 when 0); past that, every further poll prints "libdma: sync timeout" and
 * force-stops the channel. Quirk: the loop's only exit is MADR reaching @p lim, so a stopped channel that never gets
 * there loops forever.
 *
 * @param dmaChan  the channel's registers
 * @param lim  address to wait for
 * @param tmo  poll limit, or 0 for the default
 */
void func_003FDEA0(sDmaChan *dmaChan, unsigned lim, int tmo) {
    int pollsLeft = 0x1000000;
    if (tmo)
        pollsLeft = tmo;
    while (dmaChan->madr < lim) {
        if (--pollsLeft < 0) {
            func_003FDEA0_log(D_004977D8);
            func_003FE5C8(dmaChan);
        }
        }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDF28);
#ifdef SKIP_ASM
/**
 * Converts a scratchpad address (0x7xxxxxxx) into the form DMA registers take (low 28 bits with bit 31 set); other
 * addresses pass through.
 *
 * @param addr  the address
 * @return the DMA address
 */
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
/**
 * Zeroes @p n bytes at @p dst.
 *
 * @param dst  destination
 * @param n  byte count
 */
void func_003FDF50(char *dst, int n) {
    while (n--)
        *dst++ = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FDF88);
#ifdef SKIP_ASM
/**
 * Returns the register block address of DMA channel @p n (table D_00451590), or 0 for a channel above 9.
 *
 * @param n  channel number
 * @return the register address, or 0
 */
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
/**
 * Resets the DMA controller.
 *
 * Clears CHCR, MADR, TADR, ASR0, ASR1 and SADR of every channel enabled in D_004977F0, clears the D_STAT status flags
 * and turns off its interrupt masks (writing back the set mask bits toggles them), applies an all-zero environment
 * (func_003FE090) and, when @p enableDma is 1, sets the DMA enable bit of D_CTRL. cPS2GraphicsMan_Init calls it.
 *
 * @param enableDma  1 to enable DMA afterwards
 * @return the D_CTRL enable bit as it was on entry
 */
int func_003FDFB0(int enableDma) {
    sDmaEnv env;
    int i;
    int saved = *(volatile int *)0x1000E000 & 1;
    for (i = 0; i < 10; i++) {
        if (D_004977F0[i] != 0) {
            int *chanRegs = (int *)D_00451590[i];
            chanRegs[0] = 0;
            chanRegs[0x30 / 4] = 0;
            chanRegs[0x10 / 4] = 0;
            chanRegs[80 / 4] = 0;
            chanRegs[0x40 / 4] = 0;
            chanRegs[0x80 / 4] = 0;
        }
    }
    *(volatile int *)0x1000E010 = 65311;
    *(int *)0x1000E010 &= 0xFF1F0000;
    func_003FDF50((char *)&env, 20);
    func_003FE090(&env);
    if (enableDma == 1)
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
/**
 * Applies a DMA environment to the controller registers and keeps a copy in D_005261D0.
 *
 * D_CTRL gets the stall source, stall drain and MFIFO fields from the tables D_00497818 / D_00497828 / D_00497838 and
 * the release cycle (0 turns release off); D_PCR, D_SQWC, D_RBOR and D_RBSR come from the other fields.
 *
 * @param dmaEnv  the environment
 * @return 0, or -1 .. -4 when the stall source, stall drain, MFIFO or release-cycle field is out of range
 */
int func_003FE090(sDmaEnv *dmaEnv) {
    unsigned ctrlReg;
    unsigned pcr;
    unsigned sqwc;
    unsigned rbor;
    unsigned rbsr;
    ctrlReg = *(volatile unsigned *)0x1000E000;
    pcr = *(volatile unsigned *)0x1000E020;
    sqwc = *(volatile unsigned *)0x1000E030;
    rbor = *(volatile unsigned *)0x1000E050;
    rbsr = *(volatile unsigned *)0x1000E040;
    if (dmaEnv->sts >= 10) return -1;
    if (dmaEnv->std >= 10) return -2;
    if (dmaEnv->mfd >= 10) return -3;
    if (dmaEnv->rcyc >= 7) return -4;
    ctrlReg = (ctrlReg & ~0x30) | (D_00497818[dmaEnv->sts] << 4);
    ctrlReg = (ctrlReg & ~0xC0) | (D_00497828[dmaEnv->std] << 6);
    ctrlReg = (ctrlReg & ~0xC) | (D_00497838[dmaEnv->mfd] << 2);
    if (dmaEnv->rcyc != 0) {
        ctrlReg |= 2;
        ctrlReg = (ctrlReg & ~0x300) | ((dmaEnv->rcyc - 1) << 8);
    } else {
        ctrlReg &= ~2;
    }
    sqwc = (dmaEnv->hA << 16) | dmaEnv->h8;
    rbsr = dmaEnv->w10;
    rbor = (unsigned)dmaEnv->w0C;
    *(volatile unsigned *)0x1000E000 = ctrlReg;
    pcr = (dmaEnv->h4 << 16) | dmaEnv->h6;
    *(volatile unsigned *)0x1000E020 = pcr;
    *(volatile unsigned *)0x1000E030 = sqwc;
    *(volatile unsigned *)0x1000E050 = rbor;
    *(volatile unsigned *)0x1000E040 = rbsr;
    D_005261D0 = *dmaEnv;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE268);
#ifdef SKIP_ASM
/**
 * Copies the DMA environment last applied (D_005261D0) into @p envOut.
 *
 * @param envOut  destination
 * @return @p envOut
 */
sDmaEnv *func_003FE268(sDmaEnv *envOut) {
    *envOut = D_005261D0;
    return envOut;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE2A0);
#ifdef SKIP_ASM
/**
 * Writes the DMA stall address register D_STADR and returns its previous value.
 *
 * @param newValue  new value
 * @return the old value
 */
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
/**
 * Starts a source-chain DMA transfer: waits for the channel (func_003FDE30), sets TADR to @p addr (DMA form), QWC 0 and
 * CHCR to chain mode, from memory, started.
 *
 * TADR is written only when it does not read back as 0xFFFFFFFF.
 *
 * @param dmaChan  the channel's registers
 * @param addr  first DMA tag
 */
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
/**
 * Starts a normal-mode DMA transfer from memory: waits for the channel, sets MADR (unless it reads back 0xFFFFFFFF) and
 * QWC, and starts it.
 *
 * @param dmaChan  the channel's registers
 * @param addr  source address
 * @param qwordCount  quadword count
 */
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
/**
 * Starts an interleave-mode DMA transfer from memory (as func_003FE320, CHCR mode 2).
 *
 * @param dmaChan  the channel's registers
 * @param addr  source address
 * @param qwordCount  quadword count
 */
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
/**
 * Starts a chain-mode DMA transfer into memory: waits for the channel, clears QWC and starts it in chain mode with the
 * direction bit clear (to memory).
 *
 * @param dmaChan  the channel's registers
 */
void func_003FE410(sDmaChan *dmaChan) {
    func_003FDE30(dmaChan);
    dmaChan->qwc = 0;
    dmaChan->chcr = ((dmaChan->chcr & ~0xC | 0x4) & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE458);
#ifdef SKIP_ASM
/**
 * Starts a normal-mode DMA transfer into memory: waits for the channel, sets MADR (unless it reads back 0xFFFFFFFF) and
 * QWC, and starts it with the direction bit clear.
 *
 * @param dmaChan  the channel's registers
 * @param addr  destination address
 * @param qwordCount  quadword count
 */
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
/**
 * Starts an interleave-mode DMA transfer into memory (as func_003FE458, CHCR mode 2).
 *
 * @param dmaChan  the channel's registers
 * @param addr  destination address
 * @param qwordCount  quadword count
 */
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
/**
 * Checks or waits for a DMA channel: mode 1 returns its STR (busy) bit; any other mode waits for it (func_003FDE30) and
 * returns 0.
 *
 * @param dmaChan  the channel's registers
 * @param mode  1 to poll, else wait
 * @return the busy bit, or 0
 */
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
/**
 * Checks or waits for a DMA channel's progress: mode 1 returns whether MADR is still below @p n; any other mode waits
 * until it is not (func_003FDEA0 with poll limit @p x) and returns 0.
 *
 * @param dmaChan  the channel's registers
 * @param n  address to reach
 * @param mode  1 to poll, else wait
 * @param x  poll limit for the wait (0 for the default)
 * @return 1 while below, or 0
 */
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
extern int DIntr();
extern void EIntr(void);
/**
 * Force-stops a DMA channel: with interrupts disabled, suspends all DMA through D_ENABLEW (bit 16) unless already
 * suspended, clears the channel's STR bit, restores D_ENABLEW and re-enables interrupts if they were on.
 *
 * @param dmaChan  the channel's registers
 * @return the channel's CHCR before the stop
 */
int func_003FE5C8(sDmaChan *dmaChan) {
    int x, enabler, intrState;
    unsigned oldChcr;
    intrState = DIntr();
    enabler = *(volatile int *)0x1000F520;
    if ((enabler & 0x10000) == 0)
        *(volatile int *)0x1000F590 = enabler | 0x10000;
    x = *(volatile int *)0x1000E000;
    oldChcr = dmaChan->chcr;
    dmaChan->chcr = oldChcr & 0xFFFFFEFF;
    *(volatile int *)0x1000F590 = enabler;
    if (intrState)
        EIntr();
    return oldChcr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FE660);
#ifdef SKIP_ASM
/**
 * Writes a DMA CHCR register and returns its previous STR bit.
 *
 * @param reg  the register
 * @param newValue  new value
 * @return bit 8 of the old value
 */
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
/**
 * Returns whether VU0 is running a microprogram (VPU-STAT bits 0..3 equal to 1: busy, no stop condition).
 *
 * @return 1 when running
 */
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
/**
 * Resets VU0 (FBRST bit 1).
 */
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
/**
 * Stops VU0: forces a break (FBRST bit 0) when it is running.
 *
 * @return 1 when VU0 is now stopped or idle (states 0, 3, 4 of func_003FE7D8), 0 when it is halted by a D or T bit
 */
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
/**
 * Resumes VU0 after a D-bit, T-bit or forced stop: restarts it at its TPC (func_003FE828, func_003FE818).
 *
 * @return 1 when restarted, 0 when it was not stopped
 */
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
/**
 * Enables or disables VU0's D-bit stops (FBRST bit 2).
 *
 * @param enable  1 to enable
 */
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
/**
 * Enables or disables VU0's T-bit stops (FBRST bit 3).
 *
 * @param enable  1 to enable
 */
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
/**
 * Returns whether VU0's D-bit stops are enabled (FBRST bit 2).
 *
 * @return 1 or 0
 */
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
/**
 * Returns whether VU0's T-bit stops are enabled (FBRST bit 3).
 *
 * @return 1 or 0
 */
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
/**
 * Returns VU0's state from VPU-STAT: 1 stopped by a D bit, 2 by a T bit, 3 by a forced break, 4 running, 0 idle.
 *
 * @return the state
 */
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
/**
 * Starts VU0 at a microprogram address (CMSAR0, then vcallmsr).
 *
 * @param addr  start address (low 16 bits)
 */
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
/**
 * Returns VU0's TPC (the address it stopped at), low 16 bits.
 *
 * @return the address
 */
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
/**
 * Saves VU0's registers into @p cnd unless VU0 is running (Sony's libdev sceDevVu0GetCnd).
 *
 * Stores the status, MAC, clipping, R, I and Q registers and VI00..VI15. Quirk: the 32 vector registers are stored from
 * address 0, not into @p cnd (the retail base register is 0).
 *
 * @param cnd  destination
 * @return 1, or 0 when VU0 is running
 */
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
/**
 * Loads VU0's registers from @p cnd unless VU0 is running: VF00..VF31, then status, MAC, clipping, R, I, Q and
 * VI00..VI15 (Sony's libdev sceDevVu0PutCnd).
 *
 * @param cnd  source
 * @return 1, or 0 when VU0 is running
 */
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
/**
 * Returns 1 when VU0 or VU1 is running a microprogram with no stop condition (VPU-STAT low nibble or bits 8..11 equal
 * to 1).
 *
 * @return 1 or 0
 */
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
/**
 * Writes one quadword into VU0 memory at quadword address @p addr (VU1's registers are mapped there from 0x400),
 * restoring VI01 and VF01 afterwards.
 *
 * @param addr  quadword address
 * @param value  the quadword
 */
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
/**
 * Reads one quadword from VU0 memory at quadword address @p addr, restoring VI01 and VF01 afterwards.
 *
 * @param addr  quadword address
 * @return the quadword
 */
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
/**
 * Resets VU1 (FBRST bit 9).
 */
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
/**
 * Stops VU1: forces a break (FBRST bit 8) when it is running.
 *
 * @return 1 when VU1 is now stopped or idle (states 0, 3, 4 of func_003FED08), 0 when it is halted by a D or T bit
 */
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
/**
 * Resumes VU1 after a D-bit, T-bit or forced stop: restarts it at its TPC (func_003FED60, func_003FED50).
 *
 * @return 1 when restarted, 0 when it was not stopped
 */
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
/**
 * Enables or disables VU1's D-bit stops (FBRST bit 10).
 *
 * @param enable  1 to enable
 */
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
/**
 * Enables or disables VU1's T-bit stops (FBRST bit 11).
 *
 * @param enable  1 to enable
 */
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
/**
 * Returns whether VU1's D-bit stops are enabled (FBRST bit 10).
 *
 * @return 1 or 0
 */
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
/**
 * Returns whether VU1's T-bit stops are enabled (FBRST bit 11).
 *
 * @return 1 or 0
 */
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
/**
 * Returns VU1's state from VPU-STAT: 1 stopped by a D bit, 2 by a T bit, 3 by a forced break, 4 running, 0 idle.
 *
 * @return the state
 */
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
/**
 * Starts VU1 at a microprogram address (writes CMSAR1).
 *
 * @param value  start address (low 16 bits)
 */
void func_003FED50(int value) {
    vu0_ctc2_D50(value & 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FED60);
#ifdef SKIP_ASM
extern void func_003FEB78(void *, int);
/**
 * Returns VU1's TPC (the address it stopped at), read from VU0 memory quadword 0x43A.
 *
 * @return the address
 */
unsigned short func_003FED60(void) {
    unsigned short value;
    func_003FEB78(&value, 1082);
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
/**
 * Saves VU1's registers into @p cnd unless a VU is running (func_003FEB08), reading them through VU0 memory: VF00..VF31
 * (0x400..0x41F), the control registers at 0x430..0x432 and 0x434..0x437, and VI00..VI15 (0x420..0x42F).
 *
 * @param cnd  destination
 * @return 1, or 0 when a VU is running
 */
int func_003FED88_impl(S3ED88 *cnd) {
    unsigned i;
    if (e3ED88_a() != 0)
        return 0;
    for (i = 0; i < 0x20; i++) {
        cnd->a[i].q = e3ED88_b(0x400 + i).q;
    }
    cnd->b[0] = e3ED88_b(1072).w[0];
    cnd->b[1] = e3ED88_b(1073).w[0];
    cnd->b[2] = e3ED88_b(1074).w[0];
    cnd->b[3] = e3ED88_b(1076).w[0];
    cnd->b[4] = e3ED88_b(1077).w[0];
    cnd->b[5] = e3ED88_b(1078).w[0];
    cnd->b[6] = e3ED88_b(1079).w[0];
    for (i = 0; i < 0x10; i++)
        cnd->c[i] = e3ED88_b(1056 + i).h[0];
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
/**
 * Loads VU1's registers from @p cnd unless a VU is running, writing them through VU0 memory (the layout of
 * func_003FED88).
 *
 * @param cnd  source
 * @return 1, or 0 when a VU is running
 */
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
    e3EF18_b(1072, value);
    value.d[0] = cnd->b[1];
    e3EF18_b(1073, value);
    value.d[0] = cnd->b[2];
    e3EF18_b(1074, value);
    value.d[0] = cnd->b[3];
    e3EF18_b(1076, value);
    value.d[0] = cnd->b[4];
    e3EF18_b(1077, value);
    value.d[0] = cnd->b[5];
    e3EF18_b(1078, value);
    value.d[0] = cnd->b[6];
    e3EF18_b(1079, value);
    for (i = 0; i < 0x10; i++) {
        value.d[0] = cnd->c[i];
        e3EF18_b(1056 + i, value);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF060);
#ifdef SKIP_ASM
/**
 * Resets VIF0 and masks its DMA-tag and command errors (FBRST 1, ERR 6).
 */
void func_003FF060(void) {
    *(volatile int *)0x10003810 = 1;
    *(volatile int *)0x10003820 = 6;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF088);
#ifdef SKIP_ASM
extern int func_003FF160(void);
/**
 * Stops VIF0 when it is busy: mode 0 forces a break (FBRST 2), mode 1 requests a stop (FBRST 4).
 *
 * @param mode  0 or 1
 * @return 1, or 0 when VIF0 is idle or already stopped
 */
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
/**
 * Lets a stopped VIF0 continue (FBRST 8).
 *
 * @return 1, or 0 when it was not stopped
 */
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
/**
 * Sets VIF0's error masks (ERR): the interrupt-bit, DMA-tag-error and command-error masks.
 *
 * @param maskInt  bit 0
 * @param maskTagErr  bit 1
 * @param maskCmdErr  bit 2
 * @return the value written
 */
unsigned func_003FF130(unsigned maskInt, unsigned maskTagErr, unsigned maskCmdErr) {
    unsigned errReg = maskInt | (maskTagErr << 1) | (maskCmdErr << 2);
    *(volatile unsigned *)0x10003820 = errReg;
    return errReg;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF150);
#ifdef SKIP_ASM
/**
 * Returns VIF0's ERR register.
 *
 * @return the register
 */
int func_003FF150(void) {
    return *(volatile int *)0x10003820;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF160);
#ifdef SKIP_ASM
/**
 * Returns VIF0's state from its STAT register: 0 idle, 1 stopped (stop, force-break or interrupt stall), 2 waiting, 3
 * error, 4 running.
 *
 * @return the state
 */
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
/**
 * Saves VIF0's registers into @p cnd when it is idle or stopped: R0..R3, C0..C3, MASK, CODE, STAT, ITOP, ITOPS, MARK,
 * NUM, ERR, the two CYCLE bytes and MODE.
 *
 * @param cnd  destination
 * @return 1, or 0 when VIF0 is busy
 */
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
/**
 * Writes up to @p n quadwords into the VIF0 FIFO, stopping early when STAT bit 28 is set.
 *
 * @param src  the quadwords
 * @param n  how many
 * @return the number written
 */
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
/**
 * Sends a pad port/slot's 32-byte command block (D_00526250 +0x4) to the IOP pad driver over SIF DMA.
 *
 * Returns 0 while the previous transfer is still pending. Otherwise increments the block's counter (word 0), writes it
 * back from the cache and DMAs it into one of two alternating 32-byte areas at +0x8. Prints "libpad: sceSifSetDma
 * faild" (when D_004515CC is set) on a pending or failed transfer.
 *
 * @param port  pad port
 * @param slot  pad slot
 * @return 1 when sent, else 0
 */
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
/**
 * Starts the pad library.
 *
 * Sets D_004515C8, binds the pad driver's two SIF RPC servers 0x80000100 and 0x80000101 (retrying with a busy wait),
 * and checks the driver's version (func_00400740): with a major version other than 4 it prints "libpad: Module version
 * mismatch [libpad.a = 4.0, padman.irx = ...]" (when D_004515CC is set) and fails. Then initialises (func_003FF5E8).
 * systemInit calls it with 0.
 *
 * @param initArg  passed on to func_003FF5E8, which ignores it
 * @return func_003FF5E8's result, or 0 on a version mismatch
 */
int func_003FF4A8(int initArg) {
    int i;
    int version;
    int majorVersion;
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
    version = func_00400740();
    majorVersion = version >> 8;
    if (majorVersion != 4) {
        if (D_004515CC != 0) {
            func_00425C60(D_004978D0);
            func_00425C60(D_004978F8, 4, 0, majorVersion, version & 0xFF);
        }
        return 0;
    }
    return func_003FF5E8_a(initArg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF5E8);
#ifdef SKIP_ASM
/**
 * Initialises the pad driver: marks every port/slot closed and clears two of its words, then sends RPC command 16.
 *
 * @return the driver's reply (word 3), or 0 when the call fails
 */
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
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF688);
#ifdef SKIP_ASM
/**
 * Ends the pad driver: RPC command 15; a reply of 1 clears D_004515C8.
 *
 * @return the reply (word 3), or 0 when the call fails
 */
int func_003FF688(void) {
    int result;
    D_00526440[0] = 15;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    result = D_00526440[3];
    if (result == 1)
        D_004515C8 = 0;
    return result;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FF708);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF8F0);
#ifdef SKIP_ASM
/**
 * Closes a pad port/slot: RPC command 14, then marks it closed. cPS2PadDevice's destructor calls it.
 *
 * @param port  port
 * @param slot  slot
 * @return the reply (word 3); 0 when not open or the call fails
 */
int func_003FF8F0(int port, int slot) {
    if (!D_00526250[port][slot].open)
        return 0;
    D_00526440[0] = 14;
    D_00526440[1] = port;
    D_00526440[2] = slot;
    D_00526440[4] = 1;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    D_00526250[port][slot].open = 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FF9A8);
#ifdef SKIP_ASM
extern void func_00424698(int *, int *);
/**
 * Returns the newer of the two 0x80-byte pad frames in a port/slot's DMA area (the one with the higher frame counter at
 * +0x58), after syncing the cache over the area (func_00424698).
 *
 * @param port  port
 * @param slot  slot
 * @return the frame
 */
sPadFrame *func_003FF9A8(int port, int slot) {
    sPadFrame *dmaArea;
    int newerOffset;
    dmaArea = (sPadFrame *)D_00526250[port][slot].dmaBuf;
    func_00424698((int *)dmaArea, (int *)((char *)dmaArea + 0xFF));
    newerOffset = (dmaArea[0].frame < dmaArea[1].frame) << 7;
    return (sPadFrame *)((char *)dmaArea + newerOffset);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFA08);
#ifdef SKIP_ASM
/**
 * Returns the frame counter of a port/slot's newest pad frame.
 *
 * @param port  port
 * @param slot  slot
 * @return the counter, or 0 when not open
 */
int func_003FFA08(int port, int slot) {
    if (!D_00526250[port][slot].open) return 0;
    return func_003FF9A8(port, slot)->frame;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFA58);
#ifdef SKIP_ASM
struct B128 { char b[128]; };
extern int DIntr();
extern void EIntr();
extern int func_0041605C_k() __asm__("func_0041605C");
/**
 * Reads a port/slot's newest pad data: copies the newest frame with interrupts disabled, then its data (length at
 * +0x60) to @p dataOut. cPS2PadDevice_sampleButtons calls it.
 *
 * @param port  port
 * @param slot  slot
 * @param dataOut  destination
 * @return the data length, or 0 when not open
 */
int func_003FFA58(int port, int slot, int dataOut) {
    struct B128 frameCopy;
    int *frameWords;
    int intrState;
    if (!D_00526250[port][slot].open)
        return 0;
    intrState = DIntr();
    frameWords = (int *)&frameCopy;
    frameCopy = *(struct B128 *)func_003FF9A8(port, slot);
    func_0041605C_k(dataOut, frameWords, frameWords[0x60 / 4]);
    if (intrState == 1)
        EIntr();
    return frameWords[0x60 / 4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFBC0);
#ifdef SKIP_ASM
/**
 * Returns a port/slot's pad state (frame byte +0x70; names in func_003FFC38). STABLE (6) while a request is BUSY (2) is
 * reported as EXECCMD (5).
 *
 * @param port  port
 * @param slot  slot
 * @return the state, or 99 when not open
 */
int func_003FFBC0(int port, int slot) {
    unsigned char *frameBytes;
    if (!D_00526250[port][slot].open)
        return 99;
    frameBytes = (unsigned char *)func_003FF9A8(port, slot); /* byte reads: a sPadFrame * makes one lhu of state/reqState */
    if (frameBytes[0x70] == 6 && frameBytes[0x71] == 2)
        return 5;
    return frameBytes[0x70];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFC38);
#ifdef SKIP_ASM
extern char *D_004515D0[];
extern char D_004979A8[];
extern char *strcpy(char *, const char *);
/**
 * Copies the name of pad state @p i: DISCONNECT, FINDCTP1, EXECCMD, STABLE, ERROR for 0, 2, 5, 6, 7; empty for the
 * others and for 8 or more.
 *
 * @param i  the state
 * @param dst  receives the name
 */
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
/**
 * Sets the request state (frame byte +0x71) of a port/slot's newest pad frame; the command senders set BUSY (2).
 *
 * @param port  port
 * @param slot  slot
 * @param newReqState  the request state
 * @return 1, or 0 when not open
 */
int func_003FFC70(int port, int slot, int newReqState) {
    if (!D_00526250[port][slot].open)
        return 0;
    func_003FF9A8(port, slot)->reqState = newReqState;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFCD8);
#ifdef SKIP_ASM
/**
 * Returns the request state (frame byte +0x71) of a port/slot: 0 COMPLETE, 1 FAILED, 2 BUSY.
 *
 * @param port  port
 * @param slot  slot
 * @return the request state, or 0 when not open
 */
int func_003FFCD8(int port, int slot) {
    if (!D_00526250[port][slot].open) return 0;
    return func_003FF9A8(port, slot)->reqState;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFD28);
#ifdef SKIP_ASM
extern char *D_004515F0[];
extern char D_004979A8[];
extern char *strcpy(char *, const char *);
/**
 * Copies the name of request state @p i: COMPLETE, FAILED, BUSY; empty for 4 or more. Quirk: entry 3 of the table
 * D_004515F0 is a null pointer, so @p i = 3 copies from address 0.
 *
 * @param i  the request state
 * @param dst  receives the name
 */
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
/**
 * Returns actuator information of a port/slot's pad (frame +0x30, four 4-byte entries; count at +0x6A).
 *
 * Needs a valid frame (+0x72 == 1) with byte +0x64 at least 2. @p actIndex -1 returns the count; otherwise @p infoKind 1..4 picks
 * byte 0..3 of entry @p actIndex. cPS2PadDevice_sampleButtons calls it.
 *
 * @param port  port
 * @param slot  slot
 * @param actIndex  entry index, or -1
 * @param infoKind  which byte (1..4)
 * @return the value, or 0
 */
int func_003FFD60(int port, int slot, int actIndex, int infoKind) {
    sPadFrame *frame;
    if (!D_00526250[port][slot].open) return 0;
    frame = func_003FF9A8(port, slot);
    if (frame->ok != 1) return 0;
    if (frame->f64 < 2) return 0;
    if (actIndex >= frame->f6A) return 0;
    if (actIndex == -1) return frame->f6A;
    switch (infoKind) {
    case 1:
        return frame->act[actIndex].a;
    case 2:
        return frame->act[actIndex].b;
    case 3:
        return frame->act[actIndex].c;
    case 4:
        return frame->act[actIndex].d;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFE80);
#ifdef SKIP_ASM
/**
 * Returns actuator-combination information of a port/slot's pad (frame +0x40; count at +0x6B), like func_003FFD60;
 * @p infoKind -1..2 picks byte 0..3 of entry @p combIndex.
 *
 * @param port  port
 * @param slot  slot
 * @param combIndex  entry index, or -1 for the count
 * @param infoKind  which byte (-1..2)
 * @return the value, or 0
 */
int func_003FFE80(int port, int slot, int combIndex, int infoKind) {
    sPadFrame *frame;
    if (!D_00526250[port][slot].open) return 0;
    frame = func_003FF9A8(port, slot);
    if (frame->ok != 1) return 0;
    if (frame->f64 < 2) return 0;
    if (combIndex == -1) return frame->f6B;
    if (combIndex >= frame->f6B) return 0;
    switch (infoKind) {
    case -1:
        return frame->comb[combIndex].a;
    case 0:
        return frame->comb[combIndex].b;
    case 1:
        return frame->comb[combIndex].c;
    case 2:
        return frame->comb[combIndex].d;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_003FFFA0);
#ifdef SKIP_ASM
/**
 * Returns mode information of a port/slot's pad.
 *
 * @p term 1: the current controller id (frame byte +0x65 >> 4; 0 for 0xF3); 2: the current mode's id from the mode
 * table (+0x50, indexed by +0x69); 3: the current mode index; 4: the mode count (+0x68) for @p offs -1, else mode
 * @p offs's id. Terms 2..4 give 0 when byte +0x64 is 1. cPS2PadDevice_sampleButtons calls it.
 *
 * @param port  pad port
 * @param slot  pad slot
 * @param term  what to return (1..4)
 * @param offs  mode index for term 4, or -1
 * @return the value; 0 when not open, the frame is not valid or a request is BUSY
 */
// scePadInfoMode(port, slot, term, offs)
int func_003FFFA0(int port, int slot, int term, int offs) {
    sPadFrame *frame;
    int t;
    if (!D_00526250[port][slot].open) return 0;
    frame = func_003FF9A8(port, slot);
    if (frame->ok != 1) return 0;
    if (frame->reqState == 2) return 0;
    switch (term) {
    case 1:
        t = frame->f65;
        if (t == 243) return 0;
        return t >> 4;
    case 2:
        if (frame->f64 == 1) return 0;
        return frame->modes[frame->f69];
    case 3:
        if (frame->f64 == 1) return 0;
        return frame->f69;
    case 4:
        if (frame->f64 == 1) return 0;
        if (offs == -1) return frame->f68;
        if (offs < frame->f68) return frame->modes[offs];
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004000D8);
#ifdef SKIP_ASM
extern int func_003FFC70(int, int, int);
/**
 * Sets a port/slot's main mode: RPC command 6 with the mode and lock words; on success marks the request BUSY.
 *
 * @param port  port
 * @param slot  slot
 * @param modeIndex  mode index
 * @param lockMode  lock setting
 * @return the reply (word 5), or 0 when the call fails
 */
int func_004000D8(int port, int slot, int modeIndex, int lockMode) {
    int result;
    D_00526440[3] = modeIndex;
    D_00526440[4] = lockMode;
    D_00526440[0] = 6;
    D_00526440[1] = port;
    D_00526440[2] = slot;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    result = D_00526440[5];
    if (result == 1)
        func_003FFC70(port, slot, 2);
    return D_00526440[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400190);
#ifdef SKIP_ASM
extern int s0_F390(int, int) __asm__("func_003FF390");
/**
 * Sends 6 actuator values to a port/slot's pad: copies them into its command block (+0xC, with words 1 and 2 set to 1
 * and 6) and sends it (func_003FF390). Needs a valid frame.
 *
 * @param port  port
 * @param slot  slot
 * @param src  the 6 values
 * @return 1 when sent, else 0
 */
int func_00400190(int port, int slot, unsigned char *src) {
    int i;
    int ok;
    int *cmdWords;
    char *cmdBytes;
    if (func_003FF9A8(port, slot)->ok != 1)
        return 0;
    cmdWords = (int *)D_00526250[port][slot].actBuf;
    cmdBytes = (char *)cmdWords + 0xC;
    for (i = 0; i < 6; i++)
        cmdBytes[i] = src[i];
    ok = 1;
    cmdWords[1] = ok;
    cmdWords[2] = 6;
    if (s0_F390(port, slot) != 1)
        ok = 0;
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400250);
#ifdef SKIP_ASM
extern int func_003FFC70(int, int, int);
/**
 * Sets a port/slot's actuator alignment: RPC command 8 with 6 bytes; on success marks the request BUSY.
 *
 * @param port  pad port
 * @param slot  pad slot
 * @param data  the 6 alignment bytes
 * @return the reply (word 5), or 0 when the call fails
 */
int func_00400250(int port, int slot, unsigned char *data) {
    int i;
    unsigned char *alignBytes;
    int *rpcBlock = D_00526440;
    rpcBlock[0] = 8;
    rpcBlock[1] = port;
    rpcBlock[2] = slot;
    alignBytes = (unsigned char *)&rpcBlock[3];
    for (i = 0; i < 6; i++)
        alignBytes[i] = data[i];
    if (sceSifCallRpc(D_00526200, 1, 0, rpcBlock, 128, rpcBlock, 128, 0, 0) < 0)
        return 0;
    if (rpcBlock[5] == 1)
        func_003FFC70(port, slot, 2);
    return rpcBlock[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400328);
#ifdef SKIP_ASM
/**
 * Returns the little-endian word at frame +0x79 of a port/slot's pad (0x3FFFF when all pressure-sensitive buttons are
 * available, per func_00400490). Needs a valid frame with bytes +0x64 and +0x66 at least 2.
 *
 * @param port  port
 * @param slot  slot
 * @return the word, or 0
 */
int func_00400328(int port, int slot) {
    sPadFrame *frame;
    if (!D_00526250[port][slot].open)
        return 0;
    frame = func_003FF9A8(port, slot);
    if (frame->ok != 1)
        return 0;
    if (frame->f64 < 2)
        return 0;
    if (frame->f66 < 2)
        return 0;
    return frame->f79[0] + ((long)frame->f79[1] << 8) + ((long)frame->f79[2] << 16) + ((long)frame->f79[3] << 24);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004003E0);
#ifdef SKIP_ASM
extern int func_003FFC70(int, int, int);
/**
 * Sends a port/slot's button mask (RPC command 10); on success marks the request BUSY.
 *
 * @param port  port
 * @param slot  slot
 * @param buttonMask  the mask (func_004004F0 passes 0xFFF, func_00400548 0)
 * @return the reply (word 4), or 0 when the call fails
 */
int func_004003E0(int port, int slot, int buttonMask) {
    int result;
    D_00526440[3] = buttonMask;
    D_00526440[0] = 10;
    D_00526440[1] = port;
    D_00526440[2] = slot;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    result = D_00526440[4];
    if (result == 1)
        func_003FFC70(port, slot, 2);
    return D_00526440[4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400490);
#ifdef SKIP_ASM
extern int func_00400328(int, int);
/**
 * Returns whether a port/slot's pad reports the full pressure-sensitive mask 0x3FFFF (func_00400328).
 *
 * @param port  port
 * @param slot  slot
 * @return 1 or 0
 */
int func_00400490(int port, int slot) {
    if (!D_00526250[port][slot].open) return 0;
    return func_00400328(port, slot) == 0x3FFFF;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004004F0);
#ifdef SKIP_ASM
extern int func_004003E0(int, int, int);
/**
 * Turns on all pressure-sensitive buttons of a port/slot (func_004003E0 with mask 0xFFF).
 *
 * @param port  port
 * @param slot  slot
 * @return func_004003E0's result, or 0 when not open
 */
int func_004004F0(int port, int slot) {
    if (!D_00526250[port][slot].open) return 0;
    return func_004003E0(port, slot, 0xFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400548);
#ifdef SKIP_ASM
extern int func_004003E0(int, int, int);
/**
 * Turns off the pressure-sensitive buttons of a port/slot (func_004003E0 with mask 0). Nothing in retail calls it.
 *
 * @param port  port
 * @param slot  slot
 * @return func_004003E0's result, or 0 when not open
 */
int func_00400548(int port, int slot) {
    if (!D_00526250[port][slot].open) return 0;
    return func_004003E0(port, slot, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004005A0);
#ifdef SKIP_ASM
struct P12 { char b[12]; } __attribute__((packed));
extern int func_003FFC70(int, int, int);
/**
 * Sends RPC command 11 with a 12-byte block for a port/slot; on success marks the request BUSY. Nothing in retail calls
 * it.
 *
 * @param port  port
 * @param slot  slot
 * @param cmdBlock  the 12-byte block
 * @return the reply (word 7), or 0 when the call fails
 */
int func_004005A0(int port, int slot, struct P12 *cmdBlock) {
    D_00526440[1] = port;
    D_00526440[0] = 11;
    D_00526440[2] = slot;
    *(struct P12 *)&D_00526440[3] = *cmdBlock;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    if (D_00526440[7] == 1)
        func_003FFC70(port, slot, 2);
    return D_00526440[7];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400670);
#ifdef SKIP_ASM
/**
 * Sends pad RPC command 12. Nothing in retail calls it.
 *
 * @return the reply (word 3), or 0 when the call fails
 */
int func_00400670(void) {
    D_00526440[0] = 12;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004006D8);
#ifdef SKIP_ASM
/**
 * Sends pad RPC command 13 with one argument. Nothing in retail calls it.
 *
 * @param argWord  the argument
 * @return the reply (word 3), or 0 when the call fails
 */
int func_004006D8(int argWord) {
    D_00526440[1] = argWord;
    D_00526440[0] = 13;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400740);
#ifdef SKIP_ASM
/**
 * Asks the pad driver for its version (RPC command 18); func_003FF4A8 requires major version 4 (bits 8..15).
 *
 * @return the version, or 0 when the call fails
 */
int func_00400740(void) {
    D_00526440[0] = 18;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
        return 0;
    return D_00526440[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004007A8);
#ifdef SKIP_ASM
/**
 * Sends pad RPC command 20 with one argument. Nothing in retail calls it.
 *
 * @param argWord  the argument
 * @return the reply (word 2), or 0 when the call fails
 */
int func_004007A8(int argWord) {
    D_00526440[1] = argWord;
    D_00526440[0] = 20;
    if (sceSifCallRpc(D_00526200, 1, 0, D_00526440, 128, D_00526440, 128, 0, 0) < 0)
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
/**
 * Alarm handler of the delay func_00400838: signals the semaphore it waits on, then sync and ei.
 *
 * @param alarmId  alarm id (unused)
 * @param ticks  alarm time (unused)
 * @param sema  the semaphore
 */
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
/**
 * Sleeps the calling thread for @p ticks alarm ticks (horizontal-blank periods).
 *
 * Creates a semaphore (max count 1, initial count 0), sets a kernel alarm whose handler func_00400810 signals it, waits
 * on it and deletes it. The libcdvd sync waits (func_004013E8, func_00401488) use it with 60 between polls.
 *
 * @param ticks  the delay in alarm ticks
 */
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
extern int DIntr();
extern void EIntr(void);
/**
 * Installs the libcdvd user callback that the callback thread runs when an asynchronous command ends.
 *
 * Fails (returns 0) while an N-command is still running (func_004013E8(1)). Otherwise, with interrupts off (DIntr /
 * EIntr), stores @p cb in D_005264C0 and the caller's $gp in D_005264C4 (the thread func_004009A8 restores that $gp
 * around the call) and returns the previous callback. Called with 0 by the EA CD streaming code to remove it.
 *
 * @param cb  the callback, given the finished command's function code; 0 to remove it
 * @return the previous callback, or 0 when a command is busy
 */
void (*func_004008A0(void (*cb)(int)))(int) {
    void (*old)(int);
    int s;
    if (func_004013E8(1) != 0)
        return 0;
    s = DIntr();
    old = D_005264C0;
    D_005264C0 = cb;
    __asm__ volatile("sw $28,%0" : "=m"(D_005264C4)); /* the callback runs with this $gp */
    if (s != 0)
        EIntr();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400908);
#ifdef SKIP_ASM
extern int iSignalSema_i(int) __asm__("iSignalSema");
/**
 * Finishes a libcdvd asynchronous N-command: releases the command semaphore and wakes the callback thread.
 *
 * Copies the pending command's function code (*@p p, the global D_00451658: 1 read, 3 get-TOC) to D_0045165C for the
 * callback. Code 11 only clears it and the busy flag D_00451634. Otherwise it signals the N-command semaphore
 * D_00451628 (which the issuing call left held) and, when the callback thread D_00451614 exists and a callback
 * D_005264C0 is set, signals the thread's semaphore D_00451620; with no callback it clears the busy flag itself. Uses
 * iSignalSema, so it runs in the SIF RPC completion (interrupt) context: func_00400B48 calls it.
 *
 * @param p  the pending command's function code (always &D_00451658)
 */
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
/**
 * Body of the libcdvd callback thread: runs the user callback each time an asynchronous command ends.
 *
 * Loops forever waiting on D_00451620. When D_00451658 is -1 (the shutdown request from func_00400CB8) it clears the
 * busy flag, the pending code, the thread id D_00451614 and D_005264D4 and exits with ExitDeleteThread. Otherwise, with
 * debug output on (D_00451610 > 0), it prints the callback and the code ("sceCdCbfunc= %d sceCdCbfunc_num= %d"), calls
 * the callback D_005264C0 with the finished command's code D_0045165C under the $gp saved when it was installed, and
 * clears the busy flag D_00451634. func_00400A78 creates the thread.
 */
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
/**
 * Creates the libcdvd callback thread, or changes its priority when it already exists.
 *
 * The first time (D_00451614 == 0) it records the current thread id (D_005264D8) and its status (D_005264E0), fills the
 * thread parameters D_00526510 (entry func_004009A8, stack, size, priority, gp 0), creates the thread and starts it,
 * keeping its id in D_00451614. Later calls only call ChangeThreadPriority.
 *
 * @param a  the thread priority
 * @param b  the thread's stack
 * @param c  the stack size in bytes
 * @return 1 when the thread was created, 0 when only its priority changed
 */
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
/**
 * SIF RPC completion handler of a CD read: patches the unaligned head and tail bytes into the read buffer.
 *
 * The IOP returns the bytes of the read that share a 64-byte cache line with other data in a fixup block (@p a, read
 * through its uncached alias): n0 bytes for d0 from b0 and n1 bytes for d1 from b1. Each byte is written through both
 * the uncached and the cached address of the destination. Then func_00400908 finishes the command (&D_00451658).
 * func_00401DF8 passes it with the fixup buffer D_00452700.
 *
 * @param a  the fixup block (the RPC end parameter)
 */
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
/**
 * Creates the libcdvd semaphores the first time they are needed.
 *
 * When any of the N-command, S-command or third semaphore (D_00451628, D_0045162C, D_00451630) is still -1, it creates
 * all three with count 1 and the callback thread's D_00451620 with count 0, and clears the busy flag D_00451634. Every
 * libcdvd entry point that takes a semaphore calls it first.
 */
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
extern int cb_078() __asm__("DIntr");
extern void EIntr(void);
/**
 * Tears down libcdvd's threads, semaphores and power-off handler (the exit mode of func_00401718).
 *
 * When the callback thread exists it sets the pending code D_00451658 to -1 and signals the thread so it exits itself
 * (func_004009A8). Deletes the four semaphores and, with interrupts off, removes the SIF command handler 0x80000012
 * (func_00426050) that func_00400E30 installed.
 */
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
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400D58);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
extern int func_00400E30(void);
/**
 * Installs the power-off callback libcdvd calls when the console's power button is pressed.
 *
 * Installs the SIF command handler first if needed (func_00400E30, while D_00451640 < 0), then with interrupts off
 * stores the function in D_005264C8, its argument in D_005264D0 and the caller's $gp in D_005264CC; the handler
 * func_00400DD8 calls it. The game's func_00319930 installs func_00319910 with the power-off thread's semaphore.
 *
 * @param a  the callback
 * @param b  the argument it is called with
 * @return the previous callback
 */
void (*func_00400D58(void (*a)(int), int b))(int) {
    int s;
    void (*old)(int);
    if (D_00451640 < 0)
        func_00400E30();
    s = DIntr();
    old = D_005264C8;
    D_005264D0 = b;
    D_005264C8 = a;
    __asm__ volatile("sw $28,%0" : "=m"(D_005264CC));
    if (s)
        EIntr();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00400DD8);
#ifdef SKIP_ASM
/**
 * SIF command handler for the IOP's power-off notice (command 0x80000012): calls the power-off callback.
 *
 * Calls D_005264C8(D_005264D0) under the $gp saved with it, unless no callback is set or the handler is being installed
 * (D_00451624). func_00400E30 installs it.
 */
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
extern int DIntr();
extern void EIntr(void);
extern void func_00400DD8(void);
extern void func_00426020(int, void (*)(void), int);
/**
 * Installs libcdvd's SIF command handler func_00400DD8 for the power-off notice 0x80000012.
 *
 * Sets the in-progress flag D_00451624 around the install (func_00426020, interrupts off) and marks the handler
 * installed (D_00451640 = 1). Called by func_00401718 and func_00400D58.
 *
 * @return 1
 */
int func_00400E30(void) {
    int s;
    D_00451624 = 1;
    s = DIntr();
    func_00426020(0x80000012, func_00400DD8, 0);
    if (s)
        EIntr();
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
/**
 * Looks a file up on the disc through libcdvd's search-file RPC server and fills its 0x24-byte file record.
 *
 * Takes the N-command semaphore D_00451628 without blocking (returns 0 when it is held or an N-command runs), binds
 * server 0x80000597 on first use (retrying, "Libcdvd bind err CdSearchFile"), copies @p name (up to 0xFF characters)
 * into the request D_00526540 with @p arg, writes it back from the cache and calls RPC 0. The record (sector, size,
 * name at +8; printed with the debug strings "search name/size/loc lbn") is copied from the request's uncached alias to
 * @p out. func_004011C0 calls it with 0.
 *
 * @param out  the 0x24-byte file record to fill
 * @param name  the file's path on the disc
 * @param arg  an extra word sent with the request
 * @return the server's result word (non-zero when found), 0 on failure
 */
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
    func_00426358((int)&D_00526540, 300);
    if (sceSifCallRpc(&D_005266C0, 0, 0, &D_00526540, 300, D_00526680, 4, 0, 0) < 0) {
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
/**
 * Looks a file up on the disc: func_00400EA8 with an extra request word of 0.
 *
 * @param a  the 0x24-byte file record to fill (sector, size, name)
 * @param b  the file's path on the disc
 * @return non-zero when the file was found, 0 otherwise
 */
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
/**
 * Prepares a libcdvd N-command: takes the N-command semaphore and binds the N-command RPC server.
 *
 * Fails (prints "Ncmd fail sema cur_cmd:%d keep_cmd:%d" in debug) when D_00451628 is already held. Records the command
 * in D_0045161C, refreshes the caller's thread status, and fails (releasing the semaphore) while a previous N-command
 * is still running (func_004013E8(1)). Then flushes the cache (func_00426408(0)) and binds server 0x80000595 on first
 * use (D_0045163C), retrying on error ("Libcdvd bind err N CMD"). On success the caller holds the semaphore and must
 * release it.
 *
 * @param a  the command being issued (for the debug output)
 * @return 1 when the command may be issued, 0 otherwise
 */
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
/**
 * Asks the CD N-command server for the drive's ready state (RPC function 14).
 *
 * Uses func_004011E0(2) to take the N-command channel, calls the function with a 4-byte reply in D_00451680 and
 * releases the semaphore. func_00401DF8 refuses to read when it returns 6 (not ready).
 *
 * @return the ready state (6 = not ready), or 0 on failure
 */
int func_00401350(void) {
    int v;
    if (func_004011E0(2) == 0)
        return 0;
    if (sceSifCallRpc(&D_004527D0, 14, 0, 0, 0, D_00451680, 4, 0, 0) < 0) {
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
/**
 * Waits for or polls the end of the running libcdvd N-command.
 *
 * Mode 0 prints "N cmd wait" (debug) and sleeps 60 ticks at a time (func_00400838) while the busy flag D_00451634 is
 * set or the N-command RPC client D_004527D0 is still busy (sceSifCheckStatRpc), then returns 0. Any other mode returns
 * at once.
 *
 * @param a  0 to block until the command ends, otherwise only poll
 * @return 1 while a command is still running, else 0
 */
int func_004013E8(int a) {
    if (a == 0) {
        if (D_00451610 > 0)
            func_00425C60(D_00497AB8);
        while (D_00451634 || sceSifCheckStatRpc(&D_004527D0))
            func_00400838(60);
        return 0;
    }
    if (D_00451634 || sceSifCheckStatRpc(&D_004527D0))
        return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00401488);
#ifdef SKIP_ASM
extern char D_00497AC8[];
extern void func_00425C60();
/**
 * Waits for or polls the end of the running libcdvd S-command.
 *
 * Mode 0 prints "S cmd wait" (debug) and sleeps 60 ticks at a time (func_00400838) while the S-command RPC client
 * D_00453180 is busy (sceSifCheckStatRpc), then returns 0. Any other mode returns the busy state at once.
 *
 * @param a  0 to block until the command ends, otherwise only poll
 * @return non-zero while an S-command is still running, else 0
 */
int func_00401488(int a) {
    if (a == 0) {
        if (D_00451610 > 0)
            func_00425C60(D_00497AC8);
        while (sceSifCheckStatRpc(&D_00453180))
            func_00400838(60);
        return 0;
    }
    return sceSifCheckStatRpc(&D_00453180);
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
/**
 * Prepares a libcdvd S-command: takes the S-command semaphore and binds the S-command RPC server.
 *
 * Fails (prints "Scmd fail sema cur_cmd:%d keep_cmd:%d" in debug) when D_0045162C is already held. Records the command
 * in D_00451618, refreshes the caller's thread status, and fails (releasing the semaphore) while an S-command still
 * runs (func_00401488(1)). Then flushes the cache (func_00426408(0)) and binds server 0x80000593 on first use
 * (D_0045164C), retrying on error ("Libcdvd bind err S cmd"). On success the caller holds the semaphore and must
 * release it.
 *
 * @param a  the command being issued (for the debug output)
 * @return 1 when the command may be issued, 0 otherwise
 */
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
/**
 * Returns the address (or id) of one of libcdvd's internal objects, chosen by a code.
 *
 * 0x8001 gives the N-command semaphore id D_00451628, 0x8002 the N-command send buffer D_00451700, 0x8003 its result
 * buffer D_00451680, 0x8004 the N-command RPC client D_004527D0; 0x8011..0x8014 give the S-command equivalents
 * (D_0045162C, D_00452C40, D_00452800, D_00453180). Other codes give 0. These are the objects the DNAS import table
 * func_003FD810 also exports.
 *
 * @param id  the object code
 * @param out  receives the id or address (0 for an unknown code)
 * @return 1
 */
int func_00401668(long id, int *out) {
    int v;
    if (id != 32769) {
        if (id != 32771) {
            if (id != 32770) {
                if (id != 32772) {
                    if (id != 32785) {
                        if (id != 32787) {
                            if (id != 32786) {
                                if (id == 32788)
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

/**
 * Initialises (or, in mode 5, shuts down) the CD/DVD library and binds its init RPC server.
 *
 * Fails while an S-command runs (func_00401488(1)). Records the current thread id, resets every RPC bind state
 * (D_00451644, D_0045163C, D_0045164C, D_00451648, D_00451650, D_00451640) to unbound, counts the call in D_00451654,
 * and binds server 0x80000592, retrying on error ("Libcdvd bind err %d CD_Init %d"). Sends @p mode (RPC 0, 16-byte
 * reply in D_00452800): a reply status of 0xFE turns debug output on (D_00451610 = 1); otherwise status words 1 or 2
 * below 0x200 make it return 2. Mode 5 prints "Libcdvd Exit", tears everything down (func_00400CB8) and marks the
 * semaphores gone; other modes create the semaphores (func_00400C00) and install the power-off handler (func_00400E30).
 * Called by systemInit's boot code (func_0031B008) and the EA CD code.
 *
 * @param mode  0 to initialise, 5 to exit
 * @return 1 on success, 2 when the IOP side reports an old version, 0 on failure
 */
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
            if (sceSifCallRpc(&D_005266E8, 0, 0, &D_00526740, 4, buf, 0x10, 0, 0) < 0) {
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
/**
 * Older form of the disc-ready check: asks the disk-ready RPC server (0x8000059A) whether a disc is ready.
 *
 * Prints "OLD DiskReady Call" (debug), takes the S-command semaphore D_0045162C without blocking (6 when held), fails
 * while an S-command runs, binds server 0x8000059A on first use ("Libcdvd bind err CdDiskReady"), sends @p a (RPC 0)
 * and returns the reply word ("DiskReady ended"). func_00401BF0 falls back to it when the newer server 0x8000059C does
 * not answer.
 *
 * @param a  the mode passed to the server (8 makes failures return -1 instead of 6)
 * @return the ready state from the IOP; 6 (not ready) on failure, or -1 in mode 8
 */
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
    if (sceSifCallRpc(&D_00526710, 0, 0, D_00526750, 4, D_00452800, 4, 0, 0) < 0) {
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
/**
 * Asks the IOP whether a disc is ready to read (the libcdvd disk-ready call).
 *
 * Prints "NEW DiskReady Call" (debug), takes the third semaphore D_00451630 without blocking and binds server
 * 0x8000059C on first use; when that server has not appeared after 17 tries it releases the semaphore and uses the
 * older server through func_004019F8. Sends @p a (RPC 0), returns the reply word from D_00453080 ("DiskReady ended")
 * and releases the semaphore.
 *
 * @param a  the mode passed to the server (8 makes failures return -1 instead of 6)
 * @return the ready state (6 = not ready); 6 on failure, or -1 in mode 8
 */
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
            if (i >= 17) {
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
    if (sceSifCallRpc(&D_00526710, 0, 0, D_00526750, 4, D_00453080, 4, 0, 0) < 0) {
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
/**
 * Starts an asynchronous CD/DVD sector read into memory (libcdvd N-command 1).
 *
 * Unless reads skip the check (bit 0 of D_00451638) it refuses when the drive reports not ready (func_00401350() == 6).
 * Takes the N-command channel (func_004011E0(4)), fills the read command D_00451700 (sector, count, buffer, the try
 * count, spindle control and data pattern from @p mode, the fixup buffer D_00452700 and the result word D_004527C0),
 * writes the destination back from the cache (sectors of 2048, 2328 or 2340 bytes for data pattern 0, 1 or 2; skipped
 * when bit 1 of D_00451638 is set) and sends RPC 1 without waiting, with func_00400B48 as the completion handler. Sets
 * the pending code D_00451658 to 1 and the busy flag; the N-command semaphore stays held until the read ends. Prints
 * "call cdread cmd" and "cdread end" in debug.
 *
 * @param lsn  the first logical sector
 * @param sectors  the number of sectors
 * @param buf  the destination address
 * @param mode  the read mode: try count, spindle control, data pattern
 * @return 1 when the read was started, 0 otherwise
 */
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
        size = sectors * 2328;
        break;
    case 2:
        size = sectors * 2340;
        break;
    case 0:
    default:
        size = sectors << 11;
        break;
    }
    D_004527C0[0] = 0;
    if ((D_00451638 & 2) == 0)
        func_00426358(buf, size);
    func_00426358((int)D_00452700, 144);
    func_00426358((int)p, 0x18);
    func_00426358((int)D_004527C0, 4);
    if (D_00451610 > 0)
        func_00425C60(D_00497BB0);
    D_00451658 = 1;
    D_00451634 = 1;
    if (sceSifCallRpc(&D_004527D0, 1, 1, p, 0x18, 0, 0, func_00400B48, D_00452700) < 0) {
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
/**
 * Reads the disc's table of contents (libcdvd N-command 4) into @p dst.
 *
 * Takes the N-command channel (func_004011E0(8)), sends the address of the work buffer D_00526780 (written back from
 * the cache first) and waits for the 8-byte reply in D_00451680. The pending code D_00451658 is 3 during the call.
 * Copies 0x400 bytes (CD) or 2064 bytes (when reply word 1 is non-zero, a DVD) from the buffer's uncached alias to @p
 * dst. The EA disc check func_003E4040 calls it after checking for a PS2 CD or DVD.
 *
 * @param dst  the destination (2064 bytes for a DVD)
 * @return the reply's result word, or 0 on failure
 */
int func_00401FD8(char *dst) {
    int v;
    int *p = D_00451700;
    if (func_004011E0(8) == 0)
        return 0;
    p[0] = (int)D_00526780;
    func_00426358((int)D_00526780, 2064);
    func_00426358((int)p, 12);
    D_00451658 = 3;
    if (sceSifCallRpc(&D_004527D0, 4, 0, p, 12, D_00451680, 8, 0, 0) < 0) {
        D_00451658 = 0;
        SignalSema(D_00451628);
        return 0;
    }
    D_00451658 = 0;
    v = D_00451680[0];
    if (D_00451680[1] == 0)
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 0x400);
    else
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 2064);
    SignalSema(D_00451628);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402268);
#ifdef SKIP_ASM
extern int func_004014F8();
extern void SignalSema(int);
/**
 * Asks the drive what kind of disc is loaded (libcdvd S-command, RPC function 3).
 *
 * Takes the S-command channel (func_004014F8(1)), calls the function with a 4-byte reply in D_00452800 and releases the
 * semaphore. The EA disc check func_003E4040 compares the result with 18 and 20 (PS2 CD and PS2 DVD).
 *
 * @return the disc type, or 0 on failure
 */
int func_00402268(void) {
    int v;
    if (func_004014F8(1) == 0)
        return 0;
    if (sceSifCallRpc(&D_00453180, 3, 0, 0, 0, D_00452800, 4, 0, 0) < 0) {
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
/**
 * Returns the drive's error code for the last command (libcdvd S-command, RPC function 4).
 *
 * Takes the S-command channel (func_004014F8(3)), calls the function with a 4-byte reply in D_00452800 and releases the
 * semaphore. The EA CD driver retries a read until it returns 0.
 *
 * @return the error code (0 = no error), or -1 on failure
 */
int func_00402300(void) {
    int v;
    if (func_004014F8(3) == 0)
        return -1;
    if (sceSifCallRpc(&D_00453180, 4, 0, 0, 0, D_00452800, 4, 0, 0) < 0) {
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
/**
 * Sends libcdvd S-command 33 and returns its status word (the console power-off request).
 *
 * Takes the S-command channel (func_004014F8(33)), calls RPC function 33 with an 8-byte reply in D_00452800, stores
 * reply word 1 in @p out and releases the semaphore. The power-off thread func_003198A0 repeats it until it returns
 * non-zero with a status of 0, after shutting dev9 down.
 *
 * @param out  receives the status word
 * @return the result word (non-zero on success), 0 on failure
 */
int func_00402398(int *out) {
    int v;
    if (func_004014F8(33) == 0)
        return 0;
    if (sceSifCallRpc(&D_00453180, 33, 0, 0, 0, D_00452800, 8, 0, 0) < 0) {
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
/**
 * Sets the drive's media mode (CD or DVD) with a libcdvd S-command.
 *
 * Takes the S-command channel (func_004014F8(34)), sends @p a with the function number and send/reply sizes held in
 * D_004531A8, D_004531AC and D_004531B0, and returns the reply word from D_00452800. The boot code func_0031B008 and
 * func_00319610 call it with 2; the EA CD code passes its configured media mode.
 *
 * @param a  the media mode (2 = DVD)
 * @return the result word, or 0 on failure
 */
int func_00402450(int a) {
    int v;
    int *p = D_00452C40;
    if (func_004014F8(34) == 0)
        return 0;
    D_00452C40[0] = a;
    func_00426358_p(p, 4);
    if (sceSifCallRpc(&D_00453180, D_004531A8, 0, p, D_004531AC, D_00452800, D_004531B0, 0, 0) < 0) {
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
/**
 * Reads the console's real-time clock (libcdvd S-command, RPC function 1) into an 8-byte record.
 *
 * Takes the S-command channel (func_004014F8(15)), prints "Libcdvd call Clock read 1" and "... 2" around the call
 * (debug), copies the 8 bytes after the result word of the 16-byte reply D_00452800 to @p a and releases the semaphore.
 * systemInit and cPS2ConsoleConfig_cPS2ConsoleConfig call it.
 *
 * @param a  receives the 8-byte clock record
 * @return the result word (non-zero on success), 0 on failure
 */
int func_00402520(struct P8 *a) {
    int v;
    if (func_004014F8(15) == 0)
        return 0;
    if (D_00451610 > 0)
        func_00425C60(D_00497BD8);
    if (sceSifCallRpc(&D_00453180, 1, 0, 0, 0, D_00452800, 16, 0, 0) < 0) {
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
/**
 * Sends a disc-tray request with a libcdvd S-command (RPC function 5).
 *
 * Takes the S-command channel (func_004014F8(26)), sends @p a, stores reply word 1 in @p out when given and returns the
 * reply's result word. func_0031AD20 calls it with 2 to check whether the tray has been opened.
 *
 * @param a  the request (2 = check the tray state)
 * @param out  receives the tray status, may be 0
 * @return the result word, or -1 on failure
 */
int func_00402618(int a, int *out) {
    int v;
    int *p = D_00452C40;
    if (func_004014F8(26) == 0)
        return -1;
    D_00452C40[0] = a;
    func_00426358_p(p, 4);
    if (sceSifCallRpc(&D_00453180, 5, 0, p, 4, D_00452800, 8, 0, 0) < 0) {
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
extern int DIntr();
extern void EIntr(void);
extern void func_00409708(void);
/**
 * Stops the IPU's DMA channels and resets the IPU before MPEG decoding starts.
 *
 * With interrupts off it holds the DMA controller (D_ENABLEW = D_ENABLER | 0x10000), clears the start bit of the
 * IPU-from and IPU-to channels (0x1000B000 and 0x1000B400), and releases it; then zeroes both channels' QWC and resets
 * and re-initialises the IPU (func_00409708). PS2_SONY_CODEC_INTERNAL_Init calls it right before setting the decoder up
 * with func_004027B8.
 *
 * @return 1
 */
int func_00402708(void) {
    int intrState = DIntr();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 &= 0xFFFFFEFF;
    *(volatile int *)0x1000B400 &= 0xFFFFFEFF;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        EIntr();
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
/**
 * Sets up an MPEG decoder in a caller-supplied work area.
 *
 * Clears the work area, aligns its start to 4 bytes as the decoder's internal state and fails with "The size of work
 * area is too small" (func_004034A8) when less than 4288 bytes remain. The memory after the first 4288 bytes becomes
 * the decoder's pool (func_00402D50 at state + 264). Resets the public fields of @p a (width, height and frame count
 * words to 0, the 64-bit time stamps to -1) and links the state at a[16] and back at +0x858; installs the default
 * callbacks func_00403DB0 and func_00403DD8, allocates a 0x600-byte block (func_00402D88), resets the stream state
 * (func_00403338, func_00402B58, func_00402BB0), points the nine per-picture tables at the state's own arrays and sets
 * the scratchpad address 0x70003600. Called by PS2_SONY_CODEC_INTERNAL_Init.
 *
 * @param a  the decoder handle
 * @param buf  the work area
 * @param size  the work area size in bytes
 * @return 1, or 0 when the work area is too small
 */
// sceMpegCreate(mp, buff, size)
int func_004027B8(int *a, int buf, int size) {
    int *p;
    int *q;
    int v;
    int rem;
    i7B8_memset(buf, 0, size);
    p = (int *)(((unsigned)(buf + 3) >> 2) << 2);
    rem = size - ((int)p - buf);
    if ((unsigned)rem < 4288) {
        i7B8_34A8(p, D_00497C20);
        return 0;
    }
    q = p + 264 / 4;
    a[16] = (int)p;
    i7B8_2D50(q, (int)p + 4288, rem - 4288);
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
    p[110] = (int)p + 488;
    p[111] = (int)p + 592;
    p[113] = (int)p + 696;
    p[114] = (int)p + 800;
    p[115] = (int)p + 904;
    p[117] = (int)p + 0x3F0;
    p[118] = (int)p + 1112;
    p[119] = (int)p + 1216;
    p[121] = (int)p + 1320;
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
/**
 * Returns 1; an empty libmpeg entry point.
 *
 * @return 1
 */
int func_004029C8(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004029D0);
#ifdef SKIP_ASM
extern int func_00403500(int, int, int);
/**
 * Hands a block of MPEG bitstream to the decoder: starts the IPU input DMA over it.
 *
 * Passes the decoder's internal state (mpeg[16]), @p dataAddr and the size plus 4 rounded up to 16 bytes to
 * func_00403500, which builds the DMA tag chain and starts channel 4 (to IPU). The EA codec func_003B0B40 calls it with
 * each chunk it has read.
 *
 * @param mpeg  the decoder handle
 * @param dataAddr  the bitstream data (16-byte aligned)
 * @param n  its size in bytes
 * @return 1
 */
int func_004029D0(int *mpeg, int dataAddr, int n) {
    func_00403500(mpeg[16], dataAddr, (n + 19) / 16 * 16);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402A10);
#ifdef SKIP_ASM
extern int func_00402E10(int *);
/**
 * Decodes the next picture into an RGB32 output buffer (with IPU colour conversion).
 *
 * Sets the conversion flag (+0xB0 = 1), the output address (its uncached alias, +0xD8), the macroblock count (+0xE4)
 * and clears the RAW8 width/height words, then runs the decode loop func_00402E10. PS2_SONY_CODEC_INTERNAL_DecodeFrame
 * calls it with (width / 16) * (height / 16) macroblocks.
 *
 * @param mpeg  the decoder handle
 * @param picture  the output buffer (64-byte aligned)
 * @param mbCount  the number of macroblocks in the picture
 * @return func_00402E10's result: 1 when a picture (or the stream end) was reached, -1 for a misaligned buffer
 */
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
/**
 * Decodes the next picture into a RAW8 (unconverted YCbCr) output buffer.
 *
 * Like func_00402A10 but with the conversion flag (+0xB0) cleared: sets the output address (uncached, +0xD8) and
 * macroblock count (+0xE4), clears the width/height words and runs func_00402E10.
 *
 * @param mpeg  the decoder handle
 * @param picture  the output buffer (64-byte aligned)
 * @param mbCount  the number of macroblocks in the picture
 * @return func_00402E10's result
 */
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
/**
 * Decodes the next picture into a RAW8 buffer laid out by macroblock columns and rows.
 *
 * Like func_00402A58, but stores the picture width and height in pixels (mbWidth * 16 at +0xDC, mbHeight * 16 at +0xE0)
 * and the macroblock count mbWidth * mbHeight, then runs func_00402E10.
 *
 * @param mpeg  the decoder handle
 * @param picture  the output buffer (64-byte aligned)
 * @param mbWidth  the picture width in macroblocks
 * @param mbHeight  the picture height in macroblocks
 * @return func_00402E10's result
 */
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
/**
 * Sets the decoder flag at internal +0x10A0 to 1.
 *
 * The flag is tested (and cleared) by the slice decoder func_00405B70.
 *
 * @param mpeg  the decoder handle
 */
void func_00402AF0(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[1064] = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B00);
#ifdef SKIP_ASM
/**
 * Sets how many I, P and B pictures the decoder decodes (-1 = all; others are skipped).
 *
 * Stores the three limits at internal +0x94, +0x98 and +0x9C; sceMpeg setup (func_004027B8) initialises them to -1. The
 * decode loop func_00402E10 passes each picture type's counter and limit to func_00403098, which skips pictures past
 * the limit.
 *
 * @param mpeg  the decoder handle
 * @param a  the I-picture limit
 * @param b  the P-picture limit
 * @param c  the B-picture limit
 */
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
/**
 * Reads back the I, P and B picture decode limits (see func_00402B00).
 *
 * @param mpeg  the decoder handle
 * @param a  receives the I-picture limit
 * @param b  receives the P-picture limit
 * @param c  receives the B-picture limit
 */
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
/**
 * Tells whether the decoder has reached the end of the stream.
 *
 * Returns internal word 0, which the decode loop func_00402E10 sets when no further picture is found.
 * PS2_SONY_CODEC_INTERNAL_GetFrame checks it.
 *
 * @param mpeg  the decoder handle
 * @return non-zero at the end of the stream
 */
int func_00402B38(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B48);
#ifdef SKIP_ASM
/**
 * Tells whether the decoder holds no reference picture still waiting to be output.
 *
 * Returns 1 when internal word 1 is 0; func_00403240 outputs the held reference picture and clears that word at the end
 * of the stream.
 *
 * @param mpeg  the decoder handle
 * @return 1 when no reference picture is pending, else 0
 */
int func_00402B48(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[1] == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402B58);
#ifdef SKIP_ASM
extern void func_00403398(int *);
/**
 * Resets the decoder's stream state, the IPU and its DMA channels.
 *
 * Clears the end flag and the reference flags (internal words 0..2), the handle's frame count (mpeg[2]) and the
 * counters at +0xAC/+0x118, sets +0x80 to -1, stops and resets the IPU and its DMA (func_00403398) and clears the
 * pending slice state (func_004032A8). sceMpeg setup func_004027B8 calls it.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
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
/**
 * Marks the decoder's six reference picture buffers as empty.
 *
 * Clears word 10 of each per-picture record the internal state points to at +0x1B8, +0x1C8, +0x1D8, +0x1BC, +0x1CC and
 * +0x1DC (the records sceMpeg setup func_004027B8 places in the state), skipping null pointers. Called by
 * func_004027B8.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
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
/**
 * Installs a decoder callback of one type and returns the one it replaces.
 *
 * The callbacks live in the internal state at +0x0C as (function, data) pairs indexed by type: 0 error (see
 * func_004034A8), 1 needs data (func_00402C80), 2 / 3 around the decoder's own use of the IPU DMA (func_00403AD0).
 * PS2_SONY_CODEC_INTERNAL_Init installs func_003B0B10 for type 1.
 *
 * @param mpeg  the decoder handle
 * @param n  the callback type
 * @param fn  the callback
 * @param cbData  the data word passed to it
 * @return the previous callback of that type
 */
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
/**
 * Calls the decoder callback whose type is the first word of @p cbData.
 *
 * Looks the (function, data) pair up in the internal state's table at +0x0C and calls function(mpeg, cbData, data) when
 * one is installed (see func_00402C08). Every libmpeg routine that reports an event goes through it.
 *
 * @param mpeg  the decoder handle (may be 0)
 * @param cbData  the event: type in word 0, then its arguments
 * @return the callback's result, or 0 when there is none
 */
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
/**
 * Raises the decoder's "needs data" callback (type 1).
 *
 * Builds an event block with type 1 and passes it to func_00402C30. The bitstream readers call it when the IPU input
 * runs dry; func_00402F80 and func_004030E0 also call it for a skipped picture.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
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
/**
 * Turns on time-stamp interpolation with a step value.
 *
 * Sets the flag at internal +0x70 and stores the 64-bit step at +0x78; func_00408530 then derives a picture's time
 * stamp from the previous one and this step when the picture carries none.
 *
 * @param mpeg  the decoder handle
 * @param v  the 64-bit interpolation step
 * @return 1
 */
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
/**
 * Turns off time-stamp interpolation (clears the flag at internal +0x70 and the step at +0x78).
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
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
/**
 * Sets the decoder's output picture address (internal +0xD8) without decoding.
 *
 * @param mpeg  the decoder handle
 * @param v  the output address
 * @return 1
 */
int func_00402CD8(int *mpeg, int v) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[54] = v;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CE8);
#ifdef SKIP_ASM
/**
 * Returns the decoder word at internal +0xCC (set from the current picture by func_004086C0 / func_004087D0).
 *
 * @param mpeg  the decoder handle
 * @return the word at +0xCC
 */
int func_00402CE8(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[51];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CF8);
#ifdef SKIP_ASM
/**
 * Returns the decoder word at internal +0xD0 (set from the current picture by func_004086C0 / func_004087D0).
 *
 * @param mpeg  the decoder handle
 * @return the word at +0xD0
 */
int func_00402CF8(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[52];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D08);
#ifdef SKIP_ASM
/**
 * Returns the address of the block at internal +0xB4 of the decoder state.
 *
 * func_00402D18 is identical.
 *
 * @param mpeg  the decoder handle
 * @return internal state + 0xB4
 */
char *func_00402D08(int *mpeg) {
    return (char *)mpeg[16] + 0xB4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D18);
#ifdef SKIP_ASM
/**
 * Returns the address of the block at internal +0xB4 of the decoder state (same as func_00402D08).
 *
 * @param mpeg  the decoder handle
 * @return internal state + 0xB4
 */
char *func_00402D18(int *mpeg) {
    return (char *)mpeg[16] + 0xB4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D28);
#ifdef SKIP_ASM
/**
 * Replaces the decoder flag at internal +0xE8 and returns its old value.
 *
 * func_00407F20 reads and clears the flag while searching for the next picture.
 *
 * @param mpeg  the decoder handle
 * @param newValue  the new value
 * @return the previous value
 */
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
/**
 * Queues a 64-bit time stamp for the next decoded picture.
 *
 * Stores it at internal +0xF0 and sets the state word +0xF8 to 1; once that state reaches 2, func_00408530 hands the
 * stamp to the next picture instead of its own and resets it to -1.
 *
 * @param mpeg  the decoder handle
 * @param v  the time stamp
 * @return 1
 */
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
/**
 * Initialises a memory pool over a block: base, size, current and mark all start at the base.
 *
 * @param heap  the pool record (base, size, current, mark)
 * @param base  the block's address
 * @param size  the block's size in bytes
 */
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
/**
 * Records the pool's current position as its mark (mark = current).
 *
 * @param heap  the pool record
 * @return the new mark
 */
int func_00402D68(int *heap) {
    return heap[3] = heap[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D78);
#ifdef SKIP_ASM
/**
 * Rewinds the pool to its mark, freeing everything allocated since (current = mark).
 *
 * func_00403748 uses it to rebuild the picture buffers for a new sequence.
 *
 * @param heap  the pool record
 * @return the new current position
 */
int func_00402D78(int *heap) {
    return heap[2] = heap[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402D88);
#ifdef SKIP_ASM
extern char D_00497C48[];
extern void func_004034A8(int *, int);
/**
 * Allocates an aligned block from a memory pool.
 *
 * Rounds the current position up to @p align and takes @p size bytes when they fit before base + size; otherwise
 * reports "work area size is too small" through the decoder's error path (func_004034A8).
 *
 * @param a  the decoder state errors are reported for
 * @param p  the pool record (base, size, current, mark)
 * @param size  the block size in bytes
 * @param align  the alignment in bytes
 * @return the block's address, or 0 when the pool is exhausted
 */
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
/**
 * Returns how many bytes are left in a memory pool (base + size - current).
 *
 * @param heap  the pool record
 * @return the free bytes
 */
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
/**
 * Runs the MPEG decode loop until one picture has been output or the stream ends.
 *
 * Fails with "image buffer needs to be aligned to 64byte boundary" (func_00403470) unless the output address (internal
 * +0xD8) is 64-byte aligned. Otherwise it repeatedly finds the next picture (func_00407520, which returns its type: 0
 * none, 1 I, 2 P, 3 or 4 B) and decodes it with func_00403098, passing the per-type counter (+0xA0/+0xA4/+0xA8; an I
 * picture restarts all three) and the type's decode limit (+0x94..+0x9C). When no picture is left it flushes the held
 * reference picture (func_00403240) and sets the end flag (word 0). Returns once a picture was output (+0x820) or the
 * end flag is set.
 *
 * @param a  the decoder handle
 * @return 1, or -1 when the output buffer is misaligned
 */
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
/**
 * Decodes (or skips) one frame picture and advances the picture counters.
 *
 * A picture whose index is below the type's limit (or with no limit, -1) is decoded: func_00407F20(state, 0) then
 * func_00407DC0; past the limit only func_00407F20 runs and the needs-data callback is raised (func_00402C80). Then
 * func_00407E90 queues the output, the alternating flag +0x120 flips for frame pictures (unless skipped), the handle's
 * frame number mpeg[2] becomes +0x118 - +0xAC, and the counters +0x118 / +0x04 advance when +0x120 is clear.
 * func_00403098 calls it for frame pictures (+0x174 == 3).
 *
 * @param a  the decoder handle
 * @param b  the index of this picture among its type
 * @param c  the type's decode limit, -1 for none
 * @return non-zero when the picture was decoded
 */
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
/**
 * Decodes one picture: field pictures go to func_004030E0, frame pictures to func_00402F80.
 *
 * Chooses by the picture structure at internal +0x174 (3 = frame). Called by the decode loop func_00402E10.
 *
 * @param a  the decoder handle
 * @param b  the index of this picture among its type
 * @param c  the type's decode limit, -1 for none
 * @return the chosen routine's result
 */
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
/**
 * Decodes (or skips) a field-picture pair: the first field, then the matching second field.
 *
 * Decodes the first field when the picture is within the type's limit (func_00407F20(state, 0), func_00407DC0), then
 * finds the next picture (func_00407520); none flushes the reference picture and sets the end flag. The second field
 * must have the other field structure (2 when +0xD4 is 1, else 1) or it returns -1. Decodes it the same way, queues the
 * output (func_00407E90), sets the frame number mpeg[2], advances +0x118 / +0x04 and raises the needs-data callback
 * (func_00402C80) when the pair was skipped.
 *
 * @param a  the decoder handle
 * @param b  the index of this picture among its type
 * @param c  the type's decode limit, -1 for none
 * @return 1 when the second field was decoded, 0 otherwise, -1 for a field mismatch
 */
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
/**
 * Outputs the reference picture still held at the end of the stream.
 *
 * When a reference picture is held (internal words 1 and 2), converts/outputs it (func_004032B8), sets the handle's
 * frame number mpeg[2] to +0x118 - +0xAC and clears the held flag.
 *
 * @param mpeg  the decoder handle
 * @return 1 when a picture was output, else 0
 */
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
/**
 * Puts the decoder back in MPEG-1 mode: clears the MPEG-2 flag (+0x848) and calls func_00405B48(1).
 *
 * The sequence extension parser func_00403BB0 sets the flag (and calls func_00405B48(0)). Called by func_00402B58.
 *
 * @param p  the decoder's internal state
 */
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
/**
 * Outputs the held reference picture into the caller's buffer.
 *
 * Reports "the second field is missing" (func_004034A8) when a field picture is still incomplete (+0x120); otherwise
 * converts it with func_004086C0 (frame pictures, +0x174 == 3) or func_004087D0 (field pictures), giving the picture
 * count +0x118 minus one. Clears +0x120.
 *
 * @param p  the decoder's internal state
 */
void func_004032B8(int *p) {
    int n = p[280 / 4];
    if (p[288 / 4] != 0) {
        func_004034A8(p, (int)D_00497CC8);
    } else if (p[372 / 4] == 3) {
        func_004086C0(p, p[444 / 4], n - 1);
    } else {
        func_004087D0(p, p[460 / 4], p[476 / 4], n - 1);
    }
    p[288 / 4] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403338);
#ifdef SKIP_ASM
extern void func_00405B48(unsigned);
/**
 * Lays out the decoder's scratchpad buffers.
 *
 * Calls func_00405B48(1), then points the two buffer pairs at scratchpad 0x70000000 / 0x70001800 (internal
 * +0x590/+0x594) and 0x70001B00 / 0x70003300 (+0x6D0/+0x6D4) and clears +0x810. Called by sceMpeg setup func_004027B8.
 *
 * @param mpegSys  the decoder's internal state
 */
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
extern int DIntr_0() __asm__("DIntr");
extern void EIntr(void);
extern void func_00409638_2(int, int) __asm__("func_00409638");
/**
 * Stops the IPU, its DMA channels and the to-scratchpad DMA, and resets the IPU.
 *
 * Sets +0x818 and clears +0x1B0; with interrupts off holds the DMA controller (D_ENABLEW) while zeroing the CHCR of
 * channels 3 and 4 (from/to IPU) and 9 (to scratchpad), then zeroes their QWC, resets the IPU (IPU_CTRL = 0x40000000)
 * and calls func_00409638(0, 0). Called by func_00402B58.
 *
 * @param mpegSys  the decoder's internal state
 */
void func_00403398(int *mpegSys) {
    int intrState;
    mpegSys[2072 / 4] = 1;
    mpegSys[432 / 4] = 0;
    intrState = DIntr_0();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 = 0;
    *(volatile int *)0x1000B400 = 0;
    *(volatile int *)0x1000D400 = 0;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        EIntr();
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
/**
 * Prints a decoder error message as "[MPEG ERROR]%s".
 *
 * The fallback of func_004034A8 when no error callback is installed.
 *
 * @param a  the message
 */
void func_00403460(int a) {
    func_00425C60(D_00497CE8, a);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403470);
#ifdef SKIP_ASM
extern int sprintf(char *, const char *, ...);
extern void func_004034A8(int *, int);
/**
 * Formats a decoder error message into a 256-byte buffer and reports it (func_004034A8).
 *
 * The callers pass one value for the format (e.g. the misaligned address) as a third argument that this C does not
 * declare; it still reaches sprintf because nothing between entry and the call touches the third argument register.
 *
 * @param mpegSys  the decoder's internal state
 * @param fmt  the printf-style message
 */
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
/**
 * Reports a decoder error: to the error callback when one is installed, else to the console.
 *
 * When the state links back to its handle (+0x858) and an error callback (type 0, +0x0C) is set, calls it with an event
 * block of type 0 and the message (func_00402C30); otherwise prints "[MPEG ERROR]%s" (func_00403460).
 *
 * @param mpegSys  the decoder's internal state
 * @param message  the message
 */
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
extern int DIntr();
extern void EIntr(void);
void q3500(int base, unsigned addr, int n) __asm__("func_00403500");
/**
 * Starts the IPU input DMA (channel 4, chain mode) over a bitstream block.
 *
 * Builds DMA ref tags in the 16-byte aligned tag area at state + 0x880: one per chunk of at most 0xFFF40 bytes, ID 3
 * (ref) for all but the last (ID 0, refe), each with the chunk's quadword count and address. Then flushes the cache
 * and, with interrupts off, sets D4_TADR to the tags, D4_QWC to 0 and D4_CHCR to 0x105 (start, chain). func_004029D0
 * calls it.
 *
 * @param base  the decoder's internal state
 * @param addr  the bitstream data
 * @param n  its size in bytes
 */
void q3500(int base, unsigned addr, int n) {
    unsigned tagBase = ((unsigned)(base + 2191) >> 4) << 4;
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
    ret = DIntr();
    *(volatile int *)0x1000B430 = tagBase & 0xFFFFFFF;
    *(volatile int *)0x1000B420 = 0;
    *(volatile int *)0x1000B400 = 261;
    if (ret)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403600);
#ifdef SKIP_ASM
/**
 * Fills a picture-size record: width, height and both in macroblocks (>> 4).
 *
 * @param picInfo  the record (words 1..4 written)
 * @param width  the width in pixels
 * @param height  the height in pixels
 * @return 1
 */
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
/**
 * Parses an MPEG sequence header and loads its quantiser matrices into the IPU.
 *
 * Reads the 12-bit horizontal and vertical sizes (+0x124, +0x128; "vertical size > 2800" is reported), the bit rate
 * (+0x134) and VBV buffer size (+0x138) through func_004072E8. For each of the intra and non-intra matrices the load
 * flag goes to +0x840 / +0x844: when set the IPU reads the matrix from the bitstream (SETIQ command 0x50000000 /
 * 0x58000000 via func_00406DF0), otherwise the default table D_00453200 / D_00453240 is DMA'd in (func_00403AD0). Then
 * reads the extensions and user data (func_004076F8) and sets the picture buffers up (func_00403748).
 *
 * @param p  the decoder's internal state
 */
void func_00403620(int *p) {
    unsigned v;
    int w;
    int b;
    p[212 / 4] = 0;
    v = func_004072E8(p, 32);
    w = (v >> 8) & 0xFFF;
    p[292 / 4] = v >> 20;
    p[296 / 4] = w;
    if (w >= 2801)
        func_004034A8(p, (int)D_00497CF8);
    v = func_004072E8(p, 30);
    p[308 / 4] = v >> 12;
    p[312 / 4] = (v >> 1) & 0x3FF;
    b = func_004072E8(p, 1);
    p[2112 / 4] = b;
    if (b != 0) {
        func_00406E20((int)p);
        func_00406DF0(p, 0x50000000);
        func_00406E20((int)p);
    } else {
        func_00403AD0(p, 0x50000000, D_00453200);
    }
    b = func_004072E8(p, 1);
    p[2116 / 4] = b;
    if (b != 0) {
        func_00406E20((int)p);
        func_00406DF0(p, 0x58000000);
        func_00406E20((int)p);
    } else {
        func_00403AD0(p, 0x58000000, D_00453240);
    }
    func_004076F8(p);
    func_00403748(p[2136 / 4]);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00403748);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_004039F0);
#ifdef SKIP_ASM
static __inline__ int unc(int a) { return (a & 0x0FFFFFFF) | 0x20000000; }
static __inline__ int adr(int base, int n) { return unc(base + n * 0x180); }
/**
 * Computes the uncached addresses of the Y, U and V planes for three picture slots.
 *
 * Slots 0 and 1 get the bases themselves; slot 2 starts w * height / 512 blocks of 0x180 bytes in. All are returned as
 * uncached (0x20000000) addresses. Called by func_00403748.
 *
 * @param y0  receives slot 0's Y address
 * @param uOut0  receives slot 0's U address
 * @param vOut0  receives slot 0's V address
 * @param y1  receives slot 1's Y address
 * @param uOut1  receives slot 1's U address
 * @param vOut1  receives slot 1's V address
 * @param y2  receives slot 2's Y address
 * @param uOut2  receives slot 2's U address
 * @param vOut2  receives slot 2's V address
 * @param y  the Y buffer
 * @param uBuffer  the U buffer
 * @param vBuffer  the V buffer
 * @param w  the picture width
 * @param height  the picture height
 */
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
extern int f3AD0_DI() __asm__("DIntr");
extern void f3AD0_EI(void) __asm__("EIntr");
extern int f3AD0_2C30(int, int *) __asm__("func_00402C30");
extern void f3AD0_6E20(int *) __asm__("func_00406E20");
extern void f3AD0_6DF0(int *, int) __asm__("func_00406DF0");
/**
 * Loads a default quantiser matrix into the IPU by DMA, between the stop and restart DMA callbacks.
 *
 * Raises callback 2 (stop DMA), waits for the IPU (func_00406E20), clears its input FIFO (BCLR), then with interrupts
 * off points channel 4 (to IPU) at @p srcAddr for 4 quadwords and starts it (CHCR 0x101); issues @p ipuCmd
 * (func_00406DF0), waits again and raises callback 3 (restart DMA). Called by func_00403620.
 *
 * @param mpegSys  the decoder's internal state
 * @param ipuCmd  the IPU SETIQ command (0x50000000 or 0x58000000)
 * @param srcAddr  the 64-byte matrix
 */
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
    *(volatile int *)0x1000B400 = 257;
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
/**
 * Parses an MPEG-2 sequence extension (extension table D_00497FE0, id 1).
 *
 * Marks the stream as MPEG-2 (+0x848 = 1, func_00405B48(0)) and reads profile/level, progressive_sequence (+0x13C),
 * chroma_format (+0x140; anything but 1 reports "_chroma_format needs to be 1: 420"), the size, bit-rate and VBV-size
 * extensions, which it merges into +0x124, +0x128, +0x134 and +0x138. Profile/level other than 0x48, 0x58 or 0x44
 * (main/simple profile at main level, main at high) reports "Unsupported profile/level".
 *
 * @param p  the decoder's internal state
 */
void func_00403BB0(int *p) {
    unsigned v;
    unsigned h;
    unsigned w;
    int t;
    unsigned x;
    unsigned y;
    unsigned z;
    p[2120 / 4] = 1;
    func_00405B48(0);
    v = func_004072E8(p, 28);
    x = (v >> 1) & 0xFFF;
    t = (v >> 17) & 3;
    y = (v >> 13) & 3;
    z = (v >> 15) & 3;
    p[320 / 4] = t;
    if (t != 1)
        func_004034A8(p, (int)D_00497D10);
    p[316 / 4] = (v >> 19) & 1;
    h = v >> 20;
    w = (unsigned)func_004072E8(p, 16) >> 8;
    if (h != 72 && h != 88 && h != 68)
        func_004034A8(p, (int)D_00497D38);
    p[292 / 4] = (z << 12) | (p[292 / 4] & 0xFFF);
    p[296 / 4] = (y << 12) | (p[296 / 4] & 0xFFF);
    p[308 / 4] = p[308 / 4] + (x << 18);
    p[312 / 4] = p[312 / 4] + (w << 10);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403CE0);
#ifdef SKIP_ASM
extern int func_004072E8(int, int);
/**
 * Parses an MPEG-2 sequence display extension (extension table D_00497FE0, id 2).
 *
 * Skips video_format; with colour_description set skips the primaries and transfer characteristics and keeps the matrix
 * coefficients (+0x144); keeps the display width (+0x148) and height (+0x14C).
 *
 * @param mpegSys  the decoder's internal state
 */
void func_00403CE0(int *mpegSys) {
    func_004072E8((int)mpegSys, 3);
    if (func_004072E8((int)mpegSys, 1)) {
        func_004072E8((int)mpegSys, 8);
        func_004072E8((int)mpegSys, 8);
        mpegSys[324 / 4] = func_004072E8((int)mpegSys, 8);
    }
    mpegSys[328 / 4] = func_004072E8((int)mpegSys, 0xE);
    func_004072E8((int)mpegSys, 1);
    mpegSys[332 / 4] = func_004072E8((int)mpegSys, 0xE);
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
/**
 * Reports "Unknown Extension" for an MPEG extension id the decoder does not handle.
 *
 * Entries 0 and 6 (reserved ids) of the extension handler table D_00497FE0; reports through func_004034A8.
 *
 * @param p  the decoder's internal state
 */
void func_00403D80(int *p) {
    func_004034A8(p, (int)D_00497D88);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403D90);
#ifdef SKIP_ASM
extern char D_00497DA0[];
extern void func_004034A8(int *, int);
/**
 * Reports that the MPEG picture spatial scalable extension is not supported.
 *
 * Entry 9 of the extension handler table D_00497FE0 (MPEG-2 extension id 9); reports its "not supported" message
 * D_00497DA0 through func_004034A8.
 *
 * @param p  the decoder's internal state
 */
void func_00403D90(int *p) {
    func_004034A8(p, (int)D_00497DA0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403DA0);
#ifdef SKIP_ASM
extern char D_00497DD8[];
extern void func_004034A8(int *, int);
/**
 * Reports that the MPEG picture temporal scalable extension is not supported.
 *
 * Entry 10 of the extension handler table D_00497FE0 (MPEG-2 extension id 10); reports its "not supported" message
 * D_00497DD8 through func_004034A8.
 *
 * @param p  the decoder's internal state
 */
void func_00403DA0(int *p) {
    func_004034A8(p, (int)D_00497DD8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403DB0);
#ifdef SKIP_ASM
extern void func_00409400(void *);
/**
 * Default "stop DMA" decoder callback (type 2): pauses the IPU DMA and saves its registers.
 *
 * Runs func_00409400 on the save block at internal +0x4C. func_004027B8 installs it; func_00403AD0 and func_00409088
 * raise type 2 before using the IPU themselves.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
int func_00403DB0(int *mpeg) {
    func_00409400((char *)mpeg[16] + 0x4C);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00403DD8);
#ifdef SKIP_ASM
extern void func_004094E8(void *);
/**
 * Default "restart DMA" decoder callback (type 3): restores and restarts the IPU DMA.
 *
 * Runs func_004094E8 on the save block at internal +0x4C that func_00403DB0 filled. func_004027B8 installs it.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
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

/**
 * Forms a macroblock's motion-compensated prediction (MSSG mpeg2decode's form_predictions).
 *
 * Clears the current macroblock record's prediction flag (+0x6BC of slot +0x810), then for a forward vector (bit 8 of
 * @p mbtype, or any P picture) and a backward vector (bit 4) issues the predictions with func_00404758 from the forward
 * / backward reference pictures (+0x1B8, +0x1BC, or the field references +0x1C8..+0x1DC for field pictures), by motion
 * type: frame, field, or dual prime (vectors from func_00405E30). Backward after forward averages. An invalid motion
 * type reports "(a)/(b)/(c) invalid motion_type(%d)" (func_00403470). Called by func_00403E00.
 *
 * @param p  the decoder's internal state
 * @param bx  the macroblock's x position in pixels
 * @param by  the macroblock's y position in pixels
 * @param mbtype  the macroblock_type bits
 * @param mtype  the motion type
 * @param PMV  the motion vectors [r][s][t]
 * @param mvfs  the motion_vertical_field_select flags
 * @param dmv  the dual-prime differential vector
 */
void func_00404050(int *p, int bx, int by, int mbtype, int mtype, int PMV[2][2][2], int mvfs[2][2], int *dmv)
{
    int DMV[2][2];
    int ref[2][2];
    int cur, sel, avg, fwd;

    sel = 1;
    avg = 0;
    *(int *)((char *)p + p[0x810 / 4] * 0x140 + 0x6BC) = 0;
    fwd = mbtype & 8;
    if (fwd || p[336 / 4] == 2) {
        if (p[372 / 4] == 3) {
            if (mtype == 2 || !fwd) {
                q4050_4758(p, p[440 / 4], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
            } else if (mtype == 1) {
                q4050_4758(p, p[440 / 4], mvfs[0][0], 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, 1, 0);
                q4050_4758(p, p[440 / 4], mvfs[1][0], 1, 0, 8, bx, by, PMV[1][0][0], PMV[1][0][1] >> 1, 1, 0);
            } else if (mtype == 3) {
                q4050_5E30(p, &DMV[0][0], dmv, PMV[0][0][0], PMV[0][0][1] >> 1);
                q4050_4758(p, p[440 / 4], 0, 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, sel, 0);
                q4050_4758(p, p[440 / 4], 1, 0, 0, 8, bx, by, DMV[0][0], DMV[0][1], sel, sel);
                q4050_4758(p, p[440 / 4], 1, 1, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, sel, 0);
                q4050_4758(p, p[440 / 4], 0, 1, 0, 8, bx, by, DMV[1][0], DMV[1][1], sel, sel);
            } else {
                q4050_3470(p, D_00497E38, mtype);
            }
        } else {
            cur = p[372 / 4] == 2;
            ref[0][0] = p[456 / 4];
            ref[0][1] = p[472 / 4];
            ref[1][0] = p[460 / 4];
            ref[1][1] = p[476 / 4];
            if (p[336 / 4] == 2 && p[288 / 4] && cur != mvfs[0][0])
                sel = 1;
            else
                sel = 0;
            if (mtype == 1 || !fwd) {
                q4050_4758(p, ref[sel][mvfs[0][0]], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
            } else if (mtype == 2) {
                q4050_4758(p, ref[sel][mvfs[0][0]], 0, 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
                if (p[336 / 4] == 2 && p[288 / 4] && cur != mvfs[1][0])
                    sel = 1;
                else
                    sel = 0;
                q4050_4758(p, ref[sel][mvfs[1][0]], 0, 0, 8, 8, bx, by, PMV[1][0][0], PMV[1][0][1], 0, 0);
            } else if (mtype == 3) {
                if (p[288 / 4])
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
        if (p[372 / 4] == 3) {
            sel = 1;
            if (mtype == 2) {
                q4050_4758(p, p[444 / 4], 0, 0, 0, 16, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
            } else {
                q4050_4758(p, p[444 / 4], mvfs[0][1], 0, 0, 8, bx, by, PMV[0][1][0], PMV[0][1][1] >> 1, sel, avg);
                q4050_4758(p, p[444 / 4], mvfs[1][1], 1, 0, 8, bx, by, PMV[1][1][0], PMV[1][1][1] >> 1, sel, avg);
            }
        } else {
            if (mtype == 1) {
                q4050_4758(p, mvfs[0][1] ? p[476 / 4] : p[460 / 4], 0, 0, 0, 16, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
            } else if (mtype == 2) {
                q4050_4758(p, mvfs[0][1] ? p[476 / 4] : p[460 / 4], 0, 0, 0, 8, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
                q4050_4758(p, mvfs[1][1] ? p[476 / 4] : p[460 / 4], 0, 0, 8, 8, bx, by, PMV[1][1][0], PMV[1][1][1], 0, avg);
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
/**
 * Finishes one decoded macroblock: runs its block copy routines and merges prediction and residual.
 *
 * Works on macroblock record @p n (two alternate, 0x140 bytes at +0x590). When it is enabled, runs each of its count
 * block routine pairs (fa / fb tables) on its block descriptors. A record flagged both intra (+0x130) and skipped
 * (+0x13C) reports "intra && skip MB". Intra: func_00405AE8(dst, src); skipped: func_00405AE8(dst, the prediction
 * +0x81C); otherwise func_00405A88 combines dst, the prediction and the residual. Called by func_00406318 and
 * func_004060C8.
 *
 * @param s  the decoder's internal state
 * @param n  the macroblock record (0 or 1)
 */
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
/**
 * Sets or clears the IPU's MPEG-1 mode bit (IPU_CTRL bit 23).
 *
 * func_004032A8 and func_00403338 set it; the sequence extension parser func_00403BB0 clears it for MPEG-2.
 *
 * @param mpeg1  1 for an MPEG-1 stream, 0 for MPEG-2
 */
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
extern int DIntr();
extern void EIntr(void);
extern char D_00497EB0[];
struct M5B70 { char pad0[0x838]; int f838; int f83C; char pad840[0x18]; int *f858; char pad85C[0x10A0 - 0x85C]; int f10A0; };
int f5B70(struct M5B70 *p) __asm__("func_00405B70");
/**
 * Waits for the IPU to finish a macroblock and records its bit position; recovers from decode errors.
 *
 * Waits for the IPU (func_00406E20), then while the IPU-from DMA still has data and no error is flagged (IPU_CTRL bit
 * 14), raises the needs-data callback whenever the IPU-to DMA is idle. When the stop flag +0x10A0 is set it stops both
 * IPU DMA channels (DMA controller held, interrupts off), resets the IPU and returns 0. Otherwise it stores IPU_TOP in
 * +0x838 and the remaining bits in the buffer (+0x83C, 32 when the top is valid). On an IPU error it reports "Error
 * code detected(BDEC)", raises the stop/restart DMA callbacks around an IPU reset, stops channel 3 and returns 0.
 *
 * @param p  the decoder's internal state
 * @return 1, or 0 after an error or a stop request
 */
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
        st = DIntr();
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
        *(volatile int *)0x1000B400 = 0;
        *(volatile int *)0x1000B000 = 0;
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & ~0x10000;
        if (st)
            EIntr();
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
        st = DIntr();
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
        *(volatile int *)0x1000B000 = 0;
        *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & ~0x10000;
        if (st)
            EIntr();
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
/**
 * Decodes a dual-prime differential motion vector (IPU VDEC with table 3).
 *
 * Called twice by func_00406CA0 for a dual-prime vector.
 *
 * @param mpegSys  the decoder's internal state
 * @return the decoded value
 */
int func_00405E10(int mpegSys) {
    return func_00406F78(mpegSys, 3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00405E30);
#ifdef SKIP_ASM
#define H1(x) (((x) + ((x) > 0)) >> 1)
#define H3(x) ((3 * (x) + ((x) > 0)) >> 1)
/**
 * Computes the dual-prime derived motion vectors (MSSG mpeg2decode's calc_DMV).
 *
 * For frame pictures (+0x174 == 3) it derives both field vectors, scaled by 1/2 and 3/2 with rounding away from zero,
 * in the order set by top_field_first (+0x178), with the vertical -1 / +1 field corrections; for field pictures one
 * vector, corrected by -1 (top field, +0x174 == 1) or +1. Called by func_00404050.
 *
 * @param mpegSys  the decoder's internal state
 * @param outVec  receives the derived vector(s)
 * @param in  the differential vector
 * @param a  the horizontal motion vector
 * @param b  the vertical motion vector
 */
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
/**
 * Decodes a macroblock_address_increment, including stuffing and escape codes.
 *
 * Reads VLC codes with the IPU (func_00406F78 table 0): 34 (stuffing) is skipped, 35 (escape) adds 33. A code of 0 is
 * an error unless it is MPEG-2 stuffing (11 bits of 15, flushed with func_004071F8): it reports "Invalid
 * macroblock_address_increment code(0x%08x)", sets the fault flag +0x11C and returns 1.
 *
 * @param p  the decoder's internal state
 * @return the address increment
 */
int func_00405FB8(int *p) {
    int sum = 0;
    int c;
    int t;
    int again;
    do {
        c = lx5_6F78(p, 0);
        switch ((unsigned)c) {
        case 35:
            again = 1;
            sum += 33;
            break;
        case 34:
            again = 1;
            break;
        case 0:
            t = lx5_70F0(p, 11);
            if (p[2120 / 4] != 0 && t == 15) {
                lx5_71F8(p, 11);
                again = 1;
                break;
            }
            lx5_3470(p, D_00497ED0, c);
            p[284 / 4] = 1;
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
/**
 * Decodes all slices of the current picture.
 *
 * Clears the macroblock record selector (+0x810) and +0x814 and computes the picture's macroblock count (+0x12C times
 * +0x130, halved for field pictures), then decodes slices with func_00406318 while it returns 1 or 3. Waits for the IPU
 * and the last macroblock (func_00405B70; a stop request gives 4, an error 2), waits for the to-scratchpad DMA,
 * finishes the last macroblock (func_00404B78) when the picture decoded cleanly and reports "= Skip to the next picture
 * =" after results 1 or 2. Called by func_00407DC0.
 *
 * @param p  the decoder's internal state
 * @return 1 when the picture decoded completely, else 0
 */
int func_004060C8(int *p) {
    int n;
    int r;
    p[2064 / 4] = 0;
    n = p[300 / 4] * p[304 / 4];
    p[2068 / 4] = 0;
    if (p[372 / 4] != 3)
        n >>= 1;
    do {
        r = func_00406318(p, n);
    } while (r == 1 || r == 3);
    func_00406E20((int)p);
    if (func_00405B70(p) == 0) {
        r = 4;
        if (p[4256 / 4] != 0)
            p[4256 / 4] = 0;
        else
            r = 2;
    }
    while ((*(volatile unsigned *)0x1000D400 >> 8) & 1)
        ;
    if (r == 0)
        func_00404B78(p, p[2064 / 4] == 0);
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
    self[284 / 4] = 0;
    func_00407438();
    startCode = func_004070F0((int)self, 32);
    if ((unsigned)(startCode - 257) >= 175) {
        func_00403470_k(self, D_00497F28, startCode);
        return 2;
    }
    func_004071F8((int)self, 32);
    rowExtension = func_004074B8(self);
    mbAddrInc = func_00405FB8(self);
    *mbIncrement = mbAddrInc;
    if (self[284 / 4] != 0) {
        func_004034A8(self, (int)D_00497F50);
        return 1;
    }
    *mbAddress = ((((rowExtension << 7) + (startCode & 0xFF)) - 1) * self[300 / 4] + mbAddrInc) - 1;
    *mbIncrement = 1;
    self[432 / 4] = 1;
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
/**
 * Decodes one slice: its header, then each macroblock until the next start code.
 *
 * The slice header comes from _sliceA0 (which returns non-zero to stop). For each macroblock: waits for the IPU
 * (func_00405B70), reads the next address increment (func_00405FB8) when needed (23 zero bits ahead means a start code:
 * return 3), decodes a coded macroblock's modes and vectors (func_004065D0) or sets up a skipped one (func_00406510),
 * issues it (func_00403E00) and finishes the previous one (func_00404B78), alternating the two macroblock records
 * (+0x810). Reports "Too many macroblocks in picture" past @p n.
 *
 * @param p  the decoder's internal state
 * @param n  the number of macroblocks in the picture
 * @return 0 at the end of the picture, 1 or 3 to continue with the next slice, 2 on an error, 4 on a stop request
 */
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
    p[284 / 4] = 0;
    for (;;) {
        if (a >= n)
            return 0;
        p[(1740 + p[2064 / 4] * 320) / 4] = 0;
        if (func_00405B70(p) == 0) {
            if (p[4256 / 4] != 0) {
                p[4256 / 4] = 0;
                return 4;
            }
            return 2;
        }
        if (b == 0) {
            if (func_004070F0((int)p, 23) == 0) {
                p[284 / 4] = 0;
                return 3;
            }
            if (p[284 / 4] != 0) {
                p[284 / 4] = 0;
                return 3;
            }
            b = func_00405FB8(p);
            if (p[284 / 4] != 0) {
                p[284 / 4] = 0;
                return 1;
            }
        }
        if (a >= n) {
            func_004034A8(p, (int)D_00497F70);
            return 2;
        }
        if (b == 1) {
            if (func_004065D0(p, &c, &d, &e, s, q, r) == 0) {
                p[284 / 4] = 0;
                return 1;
            }
        } else {
            if (func_00406510(p, s, &d, q, &c) == 0) {
                p[284 / 4] = 0;
                return 2;
            }
        }
        if (func_00403E00_k(p, a, b, c, d, s, q, r) == 0) {
            p[284 / 4] = 0;
            return 2;
        }
        if (a != 0)
            func_00404B78(p, p[2064 / 4] ^ 1);
        a++;
        p[2064 / 4] ^= 1;
        b--;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406510);
#ifdef SKIP_ASM
extern char D_00497F90[];
extern void func_004034A8(int *, int);
/**
 * Sets up a skipped macroblock (MSSG mpeg2decode's skipped_macroblock).
 *
 * Marks the current macroblock record as skipped (+0x6CC) and +0x1B0; in P pictures resets the forward vectors, sets
 * the motion type to frame (2, frame pictures) or field (1, with both field selects set to the bottom-field flag) and
 * clears the intra bit of the macroblock type. A skipped macroblock in an I picture reports "skiped macroblock in I
 * picure is not allowed".
 *
 * @param p  the decoder's internal state
 * @param q  the motion vectors (PMV)
 * @param r  receives the motion type
 * @param s  the motion_vertical_field_select flags
 * @param t  the macroblock type (intra bit cleared)
 * @return 1, or 0 in an I picture
 */
int func_00406510(int *p, int *q, int *r, int *s, int *t) {
    int ok = 1;
    int v;
    p[p[2064 / 4] * 320 / 4 + 1740 / 4] = 1;
    p[432 / 4] = 1;
    if (p[336 / 4] == 2) {
        q[5] = 0;
        q[4] = 0;
        q[1] = 0;
        q[0] = 0;
    }
    if (p[372 / 4] == 3) {
        *r = 2;
    } else {
        *r = 1;
        v = p[372 / 4] == 2;
        s[1] = v;
        s[0] = v;
    }
    if (p[336 / 4] == 1) {
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
/**
 * Decodes a macroblock's modes and motion vectors and starts the IPU block decode (MSSG decode_macroblock).
 *
 * Puts the picture coding type into IPU_CTRL, decodes macroblock_type (func_00406F78 table 1; 0 reports "Invalid
 * macroblock_type code: 0"), the frame/field motion type, dct_type and quantiser scale (+0x1B4), then the forward and
 * backward vectors (func_00406B00 for MPEG-2, func_00406CA0 for MPEG-1). For coded blocks it clears the record's
 * 0x300-byte output (func_00408AF8) and issues an IPU BDEC command; otherwise marks the record skipped. Resets the
 * vectors after intra macroblocks without concealment vectors and for P macroblocks without a forward vector.
 *
 * @param p  the decoder's internal state
 * @param mbtype  receives the macroblock_type bits
 * @param motion_type  receives the motion type
 * @param dct_type  receives the DCT type
 * @param PMV  the motion vectors
 * @param mvfs  the motion_vertical_field_select flags
 * @param dmvector  receives the dual-prime differential vector
 * @return 1, or 0 on an error (fault flag +0x11C set)
 */
int func_004065D0(int *p, int *mbtype, int *motion_type, int *dct_type, int *PMV, int *mvfs, int *dmvector) {
    int macroblock_type;
    int motion_vector_count, mv_format, dmv, mvscale;
    *(volatile unsigned int *)0x10002010 = (*(volatile unsigned int *)0x10002010 & 0xF8FFFFFF) | (p[336 / 4] << 24);
    macroblock_type = m65D0_6F78(p, 1);
    *mbtype = macroblock_type;
    if (macroblock_type == 0) {
        func_004034A8(p, (int)D_00497FC0);
        p[284 / 4] = 1;
        return 0;
    }
    if (macroblock_type & 0xC) {
        if (p[372 / 4] == 3 && p[380 / 4])
            *motion_type = 2;
        else
            *motion_type = m65D0_72E8(p, 2);
    } else if ((macroblock_type & 1) && p[0x180 / 4]) {
        *motion_type = (p[372 / 4] == 3) ? 2 : 1;
    }
    if (p[372 / 4] == 3) {
        motion_vector_count = (*motion_type == 1) ? 2 : 1;
        mv_format = (*motion_type == 2);
    } else {
        motion_vector_count = (*motion_type == 2) ? 2 : 1;
        mv_format = 0;
    }
    dmv = (*motion_type == 3);
    mvscale = (mv_format == 0 && p[372 / 4] == 3);
    *dct_type = (p[372 / 4] == 3 && !p[380 / 4] && (*mbtype & 3)) ? m65D0_72E8(p, 1) : 0;

    if (*mbtype & 0x10)
        p[436 / 4] = m65D0_72E8(p, 5);

    if ((*mbtype & 8) || ((*mbtype & 1) && p[0x180 / 4])) {
        if (p[2120 / 4])
            m65D0_6B00(p, PMV, dmvector, mvfs, 0, motion_vector_count, mv_format, p[356 / 4] - 1, p[360 / 4] - 1, dmv, mvscale);
        else
            m65D0_6CA0(p, PMV, dmvector, p[344 / 4] - 1, p[344 / 4] - 1, 0, 0, p[340 / 4]);
    }
    if (p[284 / 4])
        return 0;

    if (*mbtype & 4) {
        if (p[2120 / 4])
            m65D0_6B00(p, PMV, dmvector, mvfs, 1, motion_vector_count, mv_format, p[364 / 4] - 1, p[368 / 4] - 1, 0, mvscale);
        else
            m65D0_6CA0(p, PMV + 2, dmvector, p[352 / 4] - 1, p[352 / 4] - 1, 0, 0, p[348 / 4]);
    }
    if (p[284 / 4])
        return 0;

    if ((*mbtype & 1) && p[0x180 / 4])
        m65D0_71F8(p, 1);

    if (*mbtype & 3) {
        m65D0_8AF8(p[p[2064 / 4] * 320 / 4 + 1428 / 4], 0x300);
        m65D0_6E20(p);
        m65D0_6DF0(p, 0x20000000 | ((*mbtype & 1) << 27) | (p[432 / 4] << 26) | (*dct_type << 25) | (p[436 / 4] << 16));
    } else {
        p[p[2064 / 4] * 320 / 4 + 1740 / 4] = 1;
    }
    p[432 / 4] = 0;
    if (p[284 / 4])
        return 0;

    if (!(*mbtype & 1))
        p[432 / 4] = 1;

    if ((*mbtype & 1) && !p[0x180 / 4]) {
        PMV[0] = PMV[1] = PMV[4] = PMV[5] = 0;
        PMV[2] = PMV[3] = PMV[6] = PMV[7] = 0;
    }

    if (p[336 / 4] == 2 && !(*mbtype & 9)) {
        PMV[0] = PMV[1] = PMV[4] = PMV[5] = 0;
        if (p[372 / 4] == 3) {
            *motion_type = 2;
        } else {
            *motion_type = 1;
            mvfs[0] = (p[372 / 4] == 2);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00406A78);
#ifdef SKIP_ASM
/* MSSG mpeg2decode motion.c decode_motion_vector() */
/**
 * Applies a decoded motion code and residual to a motion vector predictor (MSSG decode_motion_vector).
 *
 * Wraps the result into the range -16 << r_size .. (16 << r_size) - 1; full-pel vectors are halved before and doubled
 * after.
 *
 * @param pred  the predictor, updated in place
 * @param r_size  the f_code minus 1
 * @param motion_code  the decoded motion code
 * @param motion_residual  the residual bits
 * @param full_pel_vector  non-zero for full-pel vectors (MPEG-1)
 */
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
/**
 * Decodes one or two MPEG-2 motion vectors with their field selects (MSSG motion_vectors).
 *
 * With one vector, reads its field select (unless frame prediction or dual prime) into both entries, decodes it
 * (func_00406CA0) and copies it to the second vector; with two, reads each field select and decodes each vector.
 *
 * @param mpegSys  the decoder's internal state
 * @param x  the motion vectors
 * @param out  the dual-prime vector
 * @param fieldSelect  the motion_vertical_field_select flags
 * @param idx  0 forward, 1 backward
 * @param mvCount  the number of vectors
 * @param k  the motion vector format (0 field, 1 frame)
 * @param rSizeH  the horizontal f_code minus 1
 * @param rSizeV  the vertical f_code minus 1
 * @param dualPrime  non-zero for dual-prime prediction
 * @param mvScale  non-zero to halve the vertical vector
 */
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
/**
 * Decodes one motion vector, horizontal then vertical (MSSG motion_vector).
 *
 * Each component is a motion code (func_00406F78 table 2) plus a residual of r_size bits when both are non-zero,
 * applied with func_00406A78; the vertical one is halved around the update when @p mvScale is set. For dual prime it
 * also reads each differential component (func_00405E10).
 *
 * @param mpegSys  the decoder's internal state
 * @param x  the vector, updated in place
 * @param out  receives the dual-prime differential vector
 * @param rSizeH  the horizontal f_code minus 1
 * @param rSizeV  the vertical f_code minus 1
 * @param dualPrime  non-zero to read the dual-prime vector
 * @param mvScale  non-zero to work on half the vertical vector
 * @param z  non-zero for full-pel vectors
 */
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
/**
 * Issues an IPU command and records its table entry.
 *
 * Writes the command to IPU_CMD and stores D_004532C0[command >> 28] in +0x818, which the bit readers (func_004070F0
 * and its neighbours) check to know whether IPU_TOP still holds their bits.
 *
 * @param p  the decoder's internal state
 * @param a  the IPU command word
 */
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
/**
 * Waits until the IPU is no longer busy (or has flagged an error).
 *
 * Spins while IPU_CTRL has BUSY set and ECD clear, raising the needs-data callback (func_00402C80) every 5001 polls so
 * the input DMA can be refilled.
 *
 * @param mpegSys  the decoder's internal state
 */
void func_00406E20(int mpegSys) {
    int n = 0;
    if ((*(volatile unsigned *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 5001) {
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
/**
 * Waits for the running IPU command's result and returns IPU_CMD.
 *
 * Spins while IPU_CMD's BUSY bit (bit 63) is set and no error is flagged, raising the needs-data callback every 5001
 * polls.
 *
 * @param mpegSys  the decoder's internal state
 * @return the 64-bit IPU_CMD value
 */
long func_00406EC8(int mpegSys) {
    int n = 0;
    long cmdResult = *(volatile long *)0x10002000;
    if (cmdResult < 0 && !(*(volatile unsigned *)0x10002010 & 0x4000)) {
        do {
            if (n++ >= 5001) {
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
/**
 * Decodes one variable-length code with the IPU (VDEC) and refreshes the bit-buffer state.
 *
 * Waits for the IPU, issues VDEC with the table @p cmd (0 address increment, 1 macroblock type, 2 motion code, 3
 * dual-prime vector), waits for the result, then stores IPU_TOP (+0x838) and the bits left in the buffer (+0x83C). Sets
 * the fault flag +0x11C when the code is 0. Raises the needs-data callback every 5001 polls.
 *
 * @param p  the decoder's internal state
 * @param cmd  the VLC table
 * @return the decoded value (low 16 bits, signed)
 */
int func_00406F78(int p, int cmd) {
    int m = 0;
    int n = 0;
    int w;
    long v;
    long b;
    int s;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 5001) {
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
            if (m++ >= 5001) {
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
/**
 * Peeks at the next @p n bits of the bitstream without consuming them.
 *
 * When the cached IPU_TOP is stale (+0x818) or has fewer than @p n bits left (+0x83C), waits for the IPU, issues FDEC 0
 * and caches the new top word with 32 bits. Returns its top @p n bits.
 *
 * @param p  the decoder's internal state
 * @param n  the number of bits (1..32)
 * @return the bits
 */
int func_004070F0(int p, int n) {
    int c;
    int x;
    int *e;
    if (*(int *)(p + 0x818) || *(int *)(p + 0x83C) < n) {
        c = 0;
        if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
            do {
                if (c++ >= 5001) {
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
/**
 * Skips @p v bits of the bitstream (IPU FDEC with a bit step) and caches the next top word.
 *
 * Like the other bit readers it first waits for the IPU, raising the needs-data callback every 5001 polls. Records the
 * command's table entry in +0x818 and stores the new IPU_TOP in +0x838 with 32 bits available (+0x83C).
 *
 * @param p  the decoder's internal state
 * @param v  the number of bits to skip (0..32)
 */
void func_004071F8(int p, int v) {
    int n = 0;
    int w;
    int x;
    int *e;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 5001) {
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
/**
 * Reads and consumes the next @p n bits of the bitstream.
 *
 * Like the other bit readers it first waits for the IPU, raising the needs-data callback every 5001 polls. Refreshes
 * the cached top word when it is stale or too short (FDEC 0), takes its top @p n bits, then advances the IPU by @p n
 * bits (FDEC n) and caches the next top word.
 *
 * @param p  the decoder's internal state
 * @param n  the number of bits (1..32)
 * @return the bits read
 */
int func_004072E8(int p, int n) {
    int c = 0;
    int w;
    unsigned r;
    int x;
    int *e;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (c++ >= 5001) {
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
/**
 * Moves the bitstream to the next start code (MSSG next_start_code).
 *
 * Waits for the IPU, skips to a byte boundary (IPU_BP & 7), then skips a byte at a time until the next 24 bits are the
 * start code prefix 0x000001.
 *
 * @param mpegSys  the decoder's internal state
 */
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
/**
 * Reads the rest of an MPEG slice header after its start code.
 *
 * Stores quantiser_scale_code in +0x1B4; when the extra bit is set skips intra_slice and the next 7 bits and the
 * extra_information_slice bytes (func_00407A10). Called by _sliceA0.
 *
 * @param mpegSys  the decoder's internal state
 * @return 0 (the slice vertical position extension, never read here)
 */
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
/**
 * Reads stream headers until the next picture and returns its coding type.
 *
 * Loops: next start code (func_00407438), then by the 32-bit code: 0x1B3 sequence header (func_00403620), 0x1B8 group
 * of pictures (func_00407AD0), 0x1B7 sequence end (returns 0), 0x100 picture: parses its header (func_00407630), raises
 * callback 5 with two 64-bit stamps preset to -1 and stores what comes back in +0x828 / +0x830, and returns the picture
 * coding type (+0x150). Called by the decode loop func_00402E10 and func_004030E0.
 *
 * @param a  the decoder's internal state
 * @return the picture coding type (1 I, 2 P, 3 B, 4 D), or 0 at the sequence end
 */
int func_00407520(struct S_7520 *a) {
    struct B7520 b;
    for (;;) {
        s7_7438(a);
        switch ((unsigned)s7_72E8(a, 0x20)) {
        case 435:
            s7_3620(a);
            break;
        case 440:
            s7_7AD0(a);
            break;
        case 439:
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
/**
 * Parses an MPEG picture header.
 *
 * Reads temporal_reference (10 bits), picture_coding_type (+0x150) and vbv_delay; for P and B pictures the forward
 * full_pel / f_code (+0x154, +0x158), for B pictures the backward ones (+0x15C, +0x160); skips the extra information
 * (func_00407A10), reads extensions and user data (func_004076F8) and numbers the picture from its temporal reference
 * (func_00407A58).
 *
 * @param mpegSys  the decoder's internal state
 */
void func_00407630(int *mpegSys) {
    int temporalRef = func_004072E8((int)mpegSys, 10);
    mpegSys[336 / 4] = func_004072E8((int)mpegSys, 3);
    func_004072E8((int)mpegSys, 16);
    if ((unsigned)(mpegSys[336 / 4] - 2) < 2) {
        mpegSys[340 / 4] = func_004072E8((int)mpegSys, 1);
        mpegSys[344 / 4] = func_004072E8((int)mpegSys, 3);
    }
    if (mpegSys[336 / 4] == 3) {
        mpegSys[348 / 4] = func_004072E8((int)mpegSys, 1);
        mpegSys[352 / 4] = func_004072E8((int)mpegSys, 3);
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
/**
 * Reads the extension and user-data blocks that follow a header (MSSG extension_and_user_data).
 *
 * After moving to the next start code, loops while it is 0x1B5 (extension) or 0x1B2 (user data): an extension's 4-bit
 * id selects its handler from a copy of the table D_00497FE0 (ids above 10 use entry 0); user data is skipped.
 *
 * @param p  the decoder's internal state
 */
void func_004076F8(int p) {
    struct PT44 t = *(struct PT44 *)D_00497FE0;
    unsigned i;
    int r;
    func_00407438(p);
    while ((r = func_004070F0(p, 32)) == 437 || r == 434) {
        if (r == 437) {
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
/**
 * Parses an MPEG-2 picture coding extension (extension table D_00497FE0, id 8).
 *
 * Reads the four f_codes (+0x164..+0x170), intra_dc_precision into IPU_CTRL bits 16-17, picture_structure (+0x174; the
 * first one is also kept in +0xD4), top_field_first (+0x178), frame_pred_frame_dct (+0x17C), concealment_motion_vectors
 * (+0x180), q_scale_type, intra_vlc_format and alternate_scan into IPU_CTRL bits 22, 21 and 20, repeat_first_field
 * (+0x184), progressive_frame (+0x188), and skips the composite display fields.
 *
 * @param mpegSys  the decoder's internal state
 */
void func_00407820(int *mpegSys) {
    mpegSys[356 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[360 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[364 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[368 / 4] = Q_72E8((int)mpegSys, 4);
    {
        int v = Q_72E8((int)mpegSys, 2);
        *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFFCFFFF) | (v << 16);
    }
    {
        int v = Q_72E8((int)mpegSys, 2);
        mpegSys[372 / 4] = v;
        if (mpegSys[212 / 4] == 0)
            mpegSys[212 / 4] = v;
    }
    mpegSys[376 / 4] = Q_72E8((int)mpegSys, 1);
    mpegSys[380 / 4] = Q_72E8((int)mpegSys, 1);
    mpegSys[0x180 / 4] = Q_72E8((int)mpegSys, 1);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFBFFFFF) | (Q_72E8((int)mpegSys, 1) << 22);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFDFFFFF) | (Q_72E8((int)mpegSys, 1) << 21);
    *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFEFFFFF) | (Q_72E8((int)mpegSys, 1) << 20);
    mpegSys[388 / 4] = Q_72E8((int)mpegSys, 1);
    Q_72E8((int)mpegSys, 1);
    mpegSys[392 / 4] = Q_72E8((int)mpegSys, 1);
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
/**
 * Skips extra-information bytes: while the next bit is 1, skips the 8 bits after it.
 *
 * @param mpegSys  the decoder's internal state
 */
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
/**
 * Numbers a picture from its temporal reference and returns the highest number seen in the GOP.
 *
 * The picture number (+0x1AC) is the GOP's base (+0x84C) plus @p n; the running maximum is kept in +0x850. For non-B
 * pictures with a non-zero reference it clears +0x854 and remembers the reference; a negative reference with +0x854
 * clear would add 0x400, which cannot happen for the 10-bit value the caller reads (dead branch).
 *
 * @param p  the decoder's internal state
 * @param n  the temporal reference
 * @return the highest picture number so far
 */
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
/**
 * Parses an MPEG group-of-pictures header.
 *
 * Clears +0xE8, starts the GOP's picture numbering after the highest number so far (+0x84C = +0x850 + 1) and sets
 * +0x854; skips the time code, keeps closed_gop (+0x1A4) and broken_link (+0x1A8), then reads extensions and user data
 * (func_004076F8).
 *
 * @param mpegSys  the decoder's internal state
 */
void func_00407AD0(int *mpegSys) {
    mpegSys[232 / 4] = 0;
    mpegSys[2124 / 4] = mpegSys[2128 / 4] + 1;
    mpegSys[2132 / 4] = 1;
    func_004072E8((int)mpegSys, 1);
    func_004072E8((int)mpegSys, 5);
    func_004072E8((int)mpegSys, 6);
    func_004072E8((int)mpegSys, 1);
    func_004072E8((int)mpegSys, 6);
    func_004072E8((int)mpegSys, 6);
    mpegSys[420 / 4] = func_004072E8((int)mpegSys, 1);
    mpegSys[424 / 4] = func_004072E8((int)mpegSys, 1);
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
/**
 * Parses an MPEG-2 quant matrix extension (extension table D_00497FE0, id 3).
 *
 * For each of the intra and non-intra load flags (stored in +0x840 / +0x844) has the IPU read the matrix from the
 * bitstream (SETIQ 0x50000000 / 0x58000000); a chroma matrix flag reports "load_chroma_intra_quantizer_matrix == 1" or
 * the non-intra one.
 *
 * @param p  the decoder's internal state
 */
void func_00407B70(int *p) {
    if ((p[2112 / 4] = func_004072E8((int)p, 1)) != 0) {
        func_00406E20((int)p);
        func_00406DF0(p, 0x50000000);
        func_00406E20((int)p);
    }
    if ((p[2116 / 4] = func_004072E8((int)p, 1)) != 0) {
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
/**
 * Parses an MPEG-2 picture display extension (extension table D_00497FE0, id 7).
 *
 * Reads one to three frame centre offsets (horizontal into +0x18C[i], vertical into +0x198[i]); the count follows
 * progressive_sequence, repeat_first_field, top_field_first and the picture structure, as in MSSG mpeg2decode.
 *
 * @param a  the decoder's internal state
 */
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
/**
 * Skips an MPEG-2 copyright extension (extension table D_00497FE0, id 4): all its fields are read and dropped.
 *
 * @param mpegSys  the decoder's internal state
 */
void func_00407D30(int mpegSys) {
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 8);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 7);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 20);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 22);
    func_004072E8(mpegSys, 1);
    func_004072E8(mpegSys, 22);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407DC0);
#ifdef SKIP_ASM
extern char D_00498068[];
extern char D_00498088[];
extern void func_004034A8();
extern int func_004060C8(int *);
/**
 * Decodes the current picture's slices and marks its picture record decoded.
 *
 * A frame picture while a field is still pending reports "odd number of field pictures" and clears +0x120. The record
 * is the frame one (+0x1C0) or the top / bottom field one (+0x1D0 / +0x1E0) by picture structure; an unknown structure
 * reports "unknown picture sutructure". When func_004060C8 succeeds, word 10 of the record is set.
 *
 * @param p  the decoder's internal state
 * @return func_004060C8's result (non-zero when decoded)
 */
int func_00407DC0(int *p) {
    int *q;
    int r;
    if (p[372 / 4] == 3 && p[288 / 4] != 0) {
        func_004034A8(p, D_00498068);
        p[288 / 4] = 0;
    }
    switch (p[372 / 4]) {
    case 3:
        q = (int *)p[0x1C0 / 4];
        break;
    case 1:
        q = (int *)p[464 / 4];
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
        q[40 / 4] = 1;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00407E90);
#ifdef SKIP_ASM
extern void func_004086C0(int *, int, int);
extern void func_004087D0(int *, int, int, int);
/**
 * Outputs the picture due for display after a decode, then advances the queued time stamp state.
 *
 * When @p flag is set: frame pictures output the backward reference (B picture) or the forward one with func_004086C0;
 * field pictures output a field pair with func_004087D0. A queued time stamp state of 1 becomes 2 (see func_00402D38).
 *
 * @param mpegSys  the decoder's internal state
 * @param n  the picture counter (output as n - 1)
 * @param flag  non-zero to output a picture
 */
void func_00407E90(int *mpegSys, int n, int flag) {
    int a, b;
    if (flag) {
        if (mpegSys[372 / 4] == 3) {
            if (mpegSys[336 / 4] == 3)
                a = mpegSys[452 / 4];
            else
                a = mpegSys[440 / 4];
            func_004086C0(mpegSys, a, n - 1);
        } else {
            if (mpegSys[336 / 4] == 3) {
                a = mpegSys[468 / 4];
                b = mpegSys[484 / 4];
            } else {
                a = mpegSys[456 / 4];
                b = mpegSys[472 / 4];
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
/**
 * Picks the picture record the next decode writes and fills it from the current headers.
 *
 * For B pictures it decodes into the 'next' records and checks both references are valid (clearing them after a broken
 * link or once the reference counters reach 2 or 4); for I/P pictures it swaps the forward/backward references (first
 * field only) and decodes into the backward one. The chosen frame/top/bottom record (by +0x174) gets state 0, the
 * picture's time stamps (+0x828 / +0x830) and its type, structure and display fields.
 *
 * @param m  the decoder's internal state
 * @param a1  non-zero for the second field of a pair
 * @return 1 when the needed reference pictures are present, else 0
 */
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
/**
 * Checks that the caller's output buffer is big enough for the decoded picture.
 *
 * With a RAW8 width/height given (+0xDC / +0xE0) both must cover the picture; otherwise the macroblock count (+0xE4)
 * must cover its width times height in macroblocks. Reports "Too small buffer size for %dx%d picture" when it does not.
 *
 * @param p  the decoder's internal state
 * @param q  the picture-size record (see func_00403600)
 * @return 1 when it fits, else 0
 */
int func_004081D8(int *p, int *q) {
    char buf[256];
    int r;
    int n = p[0xE0 / 4];
    if (n != 0)
        r = p[220 / 4] >= q[1] && n >= q[2];
    else
        r = p[228 / 4] >= q[3] * q[4];
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
extern int DIntr();
extern void EIntr(void);
/**
 * Copies a decoded picture to the RAW8 output buffer through the scratchpad.
 *
 * Moves the picture's macroblock rows (picture record word 4 macroblocks of 0x180 bytes each, word 3 rows) to the
 * scratchpad (channel 9) and on to the output (+0xD8, channel 8), one row per DMA pair, waiting for each. With a RAW8
 * width (+0xE0) the output rows are strided; field pictures copy two interleaved passes.
 *
 * @param s  the decoder's internal state
 * @param p  the picture record
 */
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
            st = DIntr();
            *(volatile int *)0x1000D480 = 0;
            *(volatile int *)0x1000D410 = src;
            *(volatile int *)0x1000D420 = qwc;
            *(volatile int *)0x1000D400 = 257;
            if (st)
                EIntr();
            while (*(volatile int *)0x1000D400 & 0x100)
                ;
            st = DIntr();
            *(volatile int *)0x1000D080 = 0;
            *(volatile int *)0x1000D010 = d;
            *(volatile int *)0x1000D020 = qwc;
            *(volatile int *)0x1000D000 = 0x100;
            if (st)
                EIntr();
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
/**
 * Marks a picture as output: the decode loop then returns.
 *
 * Sets the reference state word 2 to 2 (copying +0x118 to +0xAC the first time) and the output flag +0x820.
 *
 * @param mpegSys  the decoder's internal state
 */
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
/**
 * Computes a picture's presentation and decode time stamps and its display flags.
 *
 * The presentation stamp is the picture's own (+0x18) unless interpolation is on (+0x70) and it has none: then the
 * previous stamp (+0x80) plus half the step (+0x78) times the frame-period multiplier (+0x88), with the odd half-units
 * alternated through +0x90. A queued stamp in state 2 (+0xF0 / +0xF8) overrides it and is cleared. The flags word packs
 * progressive_sequence, progressive_frame, top_field_first, repeat_first_field, picture structure and type. The C still
 * differs from retail in one instruction's position (97.98%).
 *
 * @param a  the decoder's internal state
 * @param b  the picture record
 * @param o1  receives the presentation stamp
 * @param o2  receives the decode stamp
 * @param o3  receives the flags word
 * @return the flags word
 */
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
/**
 * Outputs a decoded frame picture to the caller: time stamps, display fields, then the pixels.
 *
 * Writes the stamps and flags into the handle (+0x10 / +0x18 / +0x20, func_00408530), keeps the presentation stamp
 * (+0x80) and the frame-period multiplier from D_004532E8 (+0x88), copies the record's display fields to +0xB4..+0xD0,
 * and when the buffer is big enough (func_004081D8) and the record is decoded converts it (func_00409088, colour
 * conversion on) or copies it (func_00408278), then marks the output (func_00408508).
 *
 * @param a  the decoder's internal state
 * @param b  the picture record
 * @param c  the picture counter (unused)
 */
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
/**
 * Outputs a decoded field pair to the caller: both fields' time stamps, then the pixels.
 *
 * Takes the fields in display order (top field first sets 0x40 in both flag words): stamps for the first go to handle
 * +0x10..+0x20, for the second to +0x28..+0x38. Copies the display fields to +0xB4..+0xD0 and, when the buffer is big
 * enough and both fields are decoded, converts or copies the pair with the record's row count doubled for the call,
 * then marks the output.
 *
 * @param a  the decoder's internal state
 * @param b  the first field's record
 * @param c  the second field's record
 */
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
extern int q8988_C078() __asm__("DIntr");
extern void q8988_C0C0(void) __asm__("EIntr");
void q8988(struct P_8988 *p) __asm__("func_00408988");
/**
 * Starts the scratchpad DMA (channel 9, chain mode) that brings a macroblock's prediction blocks in.
 *
 * Builds two 48-quadword ref tags per entry of the current macroblock record (+0x810 selects it) in the uncached tag
 * area D_00526FC0, the last one ending the chain, then with interrupts off sets the scratchpad address from the record
 * and starts the channel. Called by func_00403E00.
 *
 * @param p  the decoder's internal state
 */
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
    *(volatile int *)0x1000D400 = 261;
    if (s)
        q8988_C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408AF8);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
/**
 * Starts the from-IPU DMA (channel 3) into scratchpad memory.
 *
 * Sets D3_MADR to the address with the scratchpad bit (0x80000000), D3_QWC to @p byteCount / 16 and starts the channel,
 * interrupts off. func_004065D0 uses it to receive a macroblock's 0x300 bytes of decoded blocks.
 *
 * @param dstAddr  the scratchpad destination
 * @param byteCount  the number of bytes
 */
void func_00408AF8(unsigned dstAddr, int byteCount) {
    int intrState = DIntr();
    *(volatile unsigned *)0x1000B010 = (dstAddr & 0xFFFFFFF) | 0x80000000;
    *(volatile int *)0x1000B020 = byteCount >> 4;
    *(volatile int *)0x1000B000 = 0x100;
    if (intrState)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00408B80);
#ifdef SKIP_ASM
extern int q8B80_C078() __asm__("DIntr");
extern void q8B80_C0C0(void) __asm__("EIntr");
extern void q8B80_6DF0(int *, int) __asm__("func_00406DF0");
extern int q8B80_2C30(int *, int *) __asm__("func_00402C30");
void q8B80(int *p, unsigned addr, int n) __asm__("func_00408B80");
/**
 * Colour-converts up to 1023 macroblocks with the IPU (CSC) straight into the output buffer.
 *
 * Waits for the IPU, starts channel 3 (from IPU) to @p addr for 64 quadwords per macroblock, issues CSC with the count,
 * raises callback 4 and waits for the DMA and the IPU to finish. Called by func_00409088.
 *
 * @param mpegSys  the decoder's internal state
 * @param addr  the RGB32 output
 * @param n  the number of macroblocks
 */
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
/**
 * DMA channel 3 interrupt handler of a long CSC conversion: queues the next 1023-macroblock chunk.
 *
 * Acknowledges the channel's interrupt (D_STAT bit 3) and counts the chunk. If the channel is still busy it sets the
 * stop flag. Otherwise it starts the next chunk (0xFFC0 quadwords and CSC 0x3FF) or the last one (the remaining
 * macroblocks). Returns with sync and ei. func_00408DF0 installs it.
 *
 * @param channel  the DMA channel (unused)
 * @param xferState  the transfer state
 * @return 0
 */
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
extern int q8DF0_C078() __asm__("DIntr");
extern void q8DF0_C0C0() __asm__("EIntr");
extern int q8DF0_2C30(int *, int *) __asm__("func_00402C30");
extern void q8DF0_34A8(int *, int) __asm__("func_004034A8");
extern int func_00408CA8();
void q8DF0(int *p, int addr, int size) __asm__("func_00408DF0");
/**
 * Colour-converts a large picture (1024 macroblocks or more) with the IPU, 1023 at a time.
 *
 * Installs func_00408CA8 as the channel 3 interrupt handler (func_00423AC0) with a transfer state on the stack, starts
 * the first chunk (0xFFC0 quadwords, CSC 0x3FF), raises callback 4 and spins until every chunk is done. A stop reports
 * "CSC handler error". Then waits for the IPU, disables and removes the handler.
 *
 * @param p  the decoder's internal state
 * @param addr  the RGB32 output
 * @param size  the number of macroblocks
 */
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
/**
 * DMA channel 4 interrupt handler: feeds the next chunk of a long transfer to the IPU.
 *
 * Acknowledges the channel's interrupt (D_STAT bit 4). With quadwords left it starts the next chunk of at most 0xFFFF
 * from the saved address and advances it; returns with sync and ei. func_00409088 installs it.
 *
 * @param a  the DMA channel (unused)
 * @param s  the transfer state: quadwords left, next address
 * @return 1 when nothing was left, else 0
 */
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
        *(volatile int *)0x1000B400 = 257;
        s->f4 = (s->f4 + 0xFFFF0) & 0xFFFFFFF;
        s->f0 = s->f0 - 0xFFFF;
    } else {
        *(volatile int *)0x1000B410 = s->f4;
        *(volatile int *)0x1000B420 = s->f0;
        *(volatile int *)0x1000B400 = 257;
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
extern int E_C078() __asm__("DIntr");
extern void E_C0C0() __asm__("EIntr");
extern void E_8B80(char *, int, int) __asm__("func_00408B80");
extern void E_8DF0(char *, int, int) __asm__("func_00408DF0");
extern void E_8FB0(void) __asm__("func_00408FB0");
void q9088(char *d, unsigned *b) __asm__("func_00409088");
/**
 * Converts a decoded picture to RGB32 with the IPU and writes it to the output buffer.
 *
 * Raises callback 2 (stop DMA), resets the IPU after an error and clears it (BCLR), then feeds the picture's 24
 * quadwords per macroblock to the IPU on channel 4 (in 0xFFFF-quadword chunks through the handler func_00408FB0 when
 * larger) while func_00408B80 or, from 1024 macroblocks, func_00408DF0 runs CSC into +0xD8. Raises callback 3 (restart
 * DMA) at the end.
 *
 * @param mpegSys  the decoder's internal state
 * @param picInfo  the picture record (address, macroblock rows / columns)
 */
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
        *(volatile unsigned *)0x1000B400 = 257;
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
        *(volatile unsigned *)0x1000B400 = 257;
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
extern int DIntr();
extern void EIntr(void);
/**
 * Writes D3_CHCR (from-IPU DMA) with the DMA controller held and interrupts off.
 *
 * @param chcrValue  the new CHCR value
 */
void func_00409310(int chcrValue) {
    int intrState = DIntr();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409388);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
/**
 * Writes D4_CHCR (to-IPU DMA) with the DMA controller held and interrupts off.
 *
 * @param chcrValue  the new CHCR value
 */
void func_00409388(int chcrValue) {
    int intrState = DIntr();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409400);
#ifdef SKIP_ASM
extern void q9400_9388(int) __asm__("func_00409388");
extern void q9400_9310(int) __asm__("func_00409310");
void q9400(int *p) __asm__("func_00409400");
/**
 * Pauses the IPU's DMA channels and saves their state and the IPU's (the stop-DMA callback's work).
 *
 * Stops channel 4 and saves its MADR, TADR, QWC and CHCR; waits until the IPU output FIFO is empty; stops channel 3 and
 * saves its MADR, QWC and CHCR, then IPU_BP and IPU_CTRL. func_004094E8 undoes it.
 *
 * @param savedRegs  the nine-word save block
 */
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
/**
 * Restores and restarts the IPU DMA saved by func_00409400.
 *
 * Restarts channel 3 when it had data left. Clears the IPU with BCLR at the saved bit pointer, then moves channel 4's
 * address back by the quadwords that were still in the IPU's input FIFO (IPU_BP's FP and IFC), adds them to its count
 * and restarts it with the saved tag address.
 *
 * @param savedRegs  the save block
 */
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
/**
 * Waits for the IPU to finish, or reports whether it is busy.
 *
 * Mode 0 spins until the BUSY bit (31) of IPU_CTRL (0x10002010) clears and returns 0; mode 1 returns that bit; any
 * other mode returns 0. The only caller, func_00403398, passes a second argument that the body ignores.
 *
 * @param mode  0 to wait, 1 to poll
 * @return 1 while the IPU is busy (mode 1), else 0
 */
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
extern int DIntr(int);
extern void EIntr(void);
/**
 * Writes D4_CHCR (the to-IPU DMA channel) with the DMA controller held.
 *
 * Calls DIntr (interrupts off; its result is ignored), sets D_ENABLEW to D_ENABLER | 0x10000, writes @p chcrValue to
 * 0x1000B400, clears the hold bit again and calls EIntr, which turns interrupts back on even if they were off before.
 * Same work as func_00409388; func_00409708 calls it with 1.
 *
 * @param chcrValue  the new CHCR value
 */
void func_004096A0(int chcrValue) {
    DIntr(chcrValue);
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    EIntr();
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
/**
 * Resets the IPU and loads its default quantiser matrices, colour table and thresholds.
 *
 * Sets D4_CHCR to 1 (func_004096A0), resets the IPU (IPU_CTRL 0x40000000) and clears its input (BCLR), waiting for BUSY
 * to clear after each command. It then pushes eight quadwords into the IPU input FIFO (0x10007010): D_00453340[0..3]
 * and four copies of D_00453340[4], loaded by SETIQ 0x50000000 (intra) and 0x58000000 (non-intra); then
 * D_00453390[0..1] for SETVQ (0x60000000), and sets both thresholds to 0 (SETTH 0x90000000). It ends with another reset
 * and BCLR. func_00402708 calls it before MPEG decoding starts.
 */
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
/**
 * Initialises the memory-card RPC client: creates its semaphores, binds the server and checks versions.
 *
 * On the first call creates the lock semaphore D_004533C4 (count 1) and the sleep semaphore D_004533C8 (count 0). Waits
 * for any pending call (func_0040A360 in mode 0), takes the lock and calls func_00426408(0), then binds SIF RPC server
 * 0x80000400 into D_005271C0, retrying with a 0x100000-step busy wait until the server is set; a bind error prints
 * "bind error libmc" and hangs forever. It runs call 0xFE synchronously (12 result bytes into D_00528780) and releases
 * the lock.
 *
 * A failed call clears the server and returns its error - 100. Result word 1 below 522 prints "libmc: too old release
 * of mcserv.irx" and word 2 below 526 the same for mcman.irx; both clear the server. systemInit calls it.
 *
 * @return result word 0 on success, the RPC error - 100, -120 (old mcserv) or -121 (old mcman)
 */
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
    r = sceSifCallRpc(&D_005271C0, 0xFE, 0, &D_00527240, 48, D_00528780, 12, 0, 0);
    Q_sig(D_004533C4);
    if (r < 0) {
        D_005271C0.server = 0;
        return r - 100;
    }
    if (D_00528780[1] < 522) {
        Q_log(D_00498108);
        D_005271C0.server = 0;
        return -120;
    }
    if (D_00528780[2] < 526) {
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
/**
 * Deletes the memory-card client's semaphores.
 *
 * When the lock semaphore D_004533C4 exists it deletes it, sets it to -1 and deletes the sleep semaphore D_004533C8
 * (whose variable is not reset). The server binding in D_005271C0 is left as it is.
 *
 * @return 1
 */
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
/**
 * Returns the memory-card client block and the addresses of its result and pending-call words.
 *
 * Stores the address of the RPC result buffer D_00528780 in *@p a and of the pending-call code D_004533C0 in *@p b, and
 * copies the lock semaphore D_004533C4 into D_00528780[15]. No caller in the source.
 *
 * @param a  receives the result buffer's address
 * @param b  receives the pending-call word's address
 * @return &D_005271C0, the client block
 */
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
/**
 * Starts memory-card call 20 with one argument (asynchronous).
 *
 * Puts @p a into the origin field of the command block D_00527240. Fails with -100 when the library is not bound
 * (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore D_004533C4 (PollSema). On success
 * it records 20 as the pending call (D_004533C0) and keeps the lock until func_0040A360 sees the call finish; when
 * sceSifCallRpc fails it releases the lock.
 *
 * @param a  the call's argument
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int func_00409B90(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.origin = a;
    r = sceSifCallRpc(&D_005271C0, 20, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * Runs memory-card call 21 for a port synchronously and returns the server's result word.
 *
 * Puts @p a into the port field of D_00527240 and waits for the call (mode 0). Fails with -100 when the library is not
 * bound (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore D_004533C4 (PollSema).
 * Releases the lock afterwards in both cases.
 *
 * @param a  the memory-card port
 * @return D_00528780[0], or -100 / -200 / sceSifCallRpc's error
 */
int func_00409C48(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    r = sceSifCallRpc(&D_005271C0, 21, 0, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * Starts opening a file on a memory card (call 2, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into D_00527270.name (at most 1023
 * characters, always terminated). Fills port, slot and mode of the name block D_00527270 and sends it (1044 bytes).
 * Fails with -100 when the library is not bound (D_005271C0.server is 0) and -200 when another call still holds the
 * lock semaphore D_004533C4 (PollSema). On success it records 2 as the pending call (D_004533C0) and keeps the lock
 * until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases the lock. The descriptor arrives as
 * func_0040A360's result.
 *
 * cPS2MemCard's callers pass mode 515 (0x203) to write and 1 to read; func_00409E38 passes 0x40.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param s_  the file path
 * @param c  the open mode bits
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 2, 1, &D_00527270, 1044, D_00528780, 4, 0, 0);
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
/**
 * Starts creating a directory on a memory card.
 *
 * Calls func_00409D10 with mode 0x40 and, when that started, changes the pending-call code D_004533C0 from 2 to 11 so
 * func_0040A360 reports it as a directory creation. cPS2MemCard calls it to make the save directory.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param c  the directory path
 * @return func_00409D10's result
 */
int func_00409E38(int a, int b, int c) {
    int r = func_00409D10(a, b, c, 0x40);
    if (r == 0) {
        D_004533C0 = 11;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00409E70);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Starts closing a memory-card file (call 3, asynchronous).
 *
 * Puts @p a into the fd field of D_00527240. Fails with -100 when the library is not bound (D_005271C0.server is 0) and
 * -200 when another call still holds the lock semaphore D_004533C4 (PollSema). On success it records 3 as the pending
 * call (D_004533C0) and keeps the lock until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases
 * the lock.
 *
 * @param a  the file descriptor
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int func_00409E70(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    r = sceSifCallRpc(&D_005271C0, 3, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * Starts moving a memory-card file's position (call 4, asynchronous).
 *
 * Fills fd, offset and origin of D_00527240. Fails with -100 when the library is not bound (D_005271C0.server is 0) and
 * -200 when another call still holds the lock semaphore D_004533C4 (PollSema). On success it records 4 as the pending
 * call (D_004533C0) and keeps the lock until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases
 * the lock.
 *
 * @param a  the file descriptor
 * @param b  the offset
 * @param c  where the offset counts from
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int func_00409F28(int a, int b, int c) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    D_00527240.offset = b;
    D_00527240.origin = c;
    r = sceSifCallRpc(&D_005271C0, 4, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * End callback of the memory-card read: copies the unaligned head and tail bytes to the destination.
 *
 * Reads the 0xC0-byte block @p a through the uncached mirror (| 0x20000000): two counts, two destinations and two
 * 64-byte buffers. Copies the first count of bytes from the first buffer to the first destination and the second count
 * from the second buffer to the second destination. func_0040A090 registers it with D_005276C0.
 *
 * @param a  the block the server filled (D_005276C0)
 */
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
/**
 * Starts reading from a memory-card file into a buffer (call 5, asynchronous).
 *
 * Fills fd, buffer, size and param (D_005276C0) of D_00527240 and writes the data cache back over the buffer and the
 * 0xC0-byte block (func_00426358). func_0040A000 runs when the call ends and copies the bytes the server returns
 * separately. Fails with -100 when the library is not bound (D_005271C0.server is 0) and -200 when another call still
 * holds the lock semaphore D_004533C4 (PollSema). On success it records 5 as the pending call (D_004533C0) and keeps
 * the lock until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases the lock. The byte count
 * arrives as func_0040A360's result.
 *
 * @param a  the file descriptor
 * @param b  the destination buffer
 * @param c  the number of bytes
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 5, 1, &D_00527240, 48, D_00528780, 4, func_0040A000, D_005276C0);
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
/**
 * Starts writing a buffer to a memory-card file (call 6, asynchronous).
 *
 * Up to 16 bytes travel entirely inside the command block (origin = @p n, no DMA). Longer data sends its first bytes up
 * to the next 16-byte boundary in the command block (origin) and the rest from there by DMA (buffer, size). Copies
 * those head bytes into D_00527240.data and flushes the cache (FlushCache(0)). Fails with -100 when the library is not
 * bound (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore D_004533C4 (PollSema). On
 * success it records 6 as the pending call (D_004533C0) and keeps the lock until func_0040A360 sees the call finish;
 * when sceSifCallRpc fails it releases the lock. The byte count arrives as func_0040A360's result.
 *
 * @param a  the file descriptor
 * @param p  the data
 * @param n  the number of bytes
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 6, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * Alarm handler that wakes the thread sleeping in func_0040A328.
 *
 * Signals @p sema from interrupt context (iSignalSema).
 *
 * @param alarmId  the alarm (unused)
 * @param ticks  the alarm's time (unused)
 * @param sema  the semaphore to signal
 */
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
/**
 * Sleeps for a number of horizontal-blank ticks.
 *
 * Sets an alarm (SetAlarm) that runs func_0040A320 with the sleep semaphore D_004533C8, then waits on that semaphore.
 * func_0040A360 sleeps 60 ticks between polls.
 *
 * @param a  the number of ticks
 */
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
/**
 * Waits for or polls the pending memory-card call and hands back its code and result.
 *
 * Returns -1 when no call is pending (D_004533C0 is 0). Otherwise checks the RPC (sceSifCheckStatRpc); in mode 0 it
 * polls until the call is done, sleeping 60 ticks between polls (func_0040A328). Stores the pending call's code in *@p
 * p when @p p is set. When the call is done it clears D_004533C0, stores the result word D_00528780[0] in *@p q (when
 * set) and releases the lock D_004533C4. Any mode other than 0 only polls.
 *
 * cPS2MemCard_Update and cPS2MemCard_CleanUp poll it; func_00409940 waits with it before re-initialising.
 *
 * @param a  0 to wait, otherwise poll
 * @param p  receives the call's code, or 0
 * @param q  receives the call's result, or 0
 * @return 1 when the call has finished, 0 while it runs, -1 when nothing is pending
 */
int func_0040A360(int a, int *p, int *q) {
    int r;
    if (D_004533C0 == 0)
        return -1;
    r = sceSifCheckStatRpc(&D_005271C0);
    if (a == 0 && r != 0) {
        while (sceSifCheckStatRpc(&D_005271C0) != 0)
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
/**
 * End callback of func_0040A498: copies the three card-information words to the caller's variables.
 *
 * Reads the server's block @p a through the uncached mirror (| 0x20000000) and stores word 0 in *D_005271E8, word 1 in
 * *D_005271EC and word 36 in *D_005271F0, each only when that pointer is set.
 *
 * @param a  the block the server filled (D_005276C0)
 */
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
/**
 * Starts reading a memory card's type, free space and format state (call 1, asynchronous).
 *
 * Fills port and slot of D_00527240 and sets its origin, offset and size fields to 1 for each output pointer that is
 * set, so the server knows which values are wanted. Saves the pointers in D_005271E8 / D_005271EC / D_005271F0 for
 * func_0040A440, which the call runs at its end, and writes the 0xC0-byte block D_005276C0 back from the cache. Fails
 * with -100 when the library is not bound (D_005271C0.server is 0) and -200 when another call still holds the lock
 * semaphore D_004533C4 (PollSema). On success it records 1 as the pending call (D_004533C0) and keeps the lock until
 * func_0040A360 sees the call finish; when sceSifCallRpc fails it releases the lock.
 *
 * cPS2MemCard's func_002C50E0 passes D_004A3938, D_004A393C and D_004A3944.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param c  receives the card type, or 0
 * @param d  receives the free space, or 0
 * @param e  receives the format state, or 0
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 1, 1, &D_00527240, 48, D_00528780, 4, func_0040A440, D_005276C0);
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
/**
 * Runs memory-card call 53 synchronously and returns the server's result word.
 *
 * Sends the command block D_00527240 without arguments and without taking the lock semaphore or checking that the
 * server is bound. A failed call prints the message D_00498158 and returns -1. No caller in the source.
 *
 * @return D_00528780[0], or -1 when the call failed
 */
int func_0040A618(void) {
    if (sceSifCallRpc(&D_005271C0, 53, 0, &D_00527240, 48, D_00528780, 4, 0, 0)) {
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
/**
 * Starts listing a memory-card directory into an entry table (call 13, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into D_00527270.name (at most 1023
 * characters, always terminated). Fills port, slot, mode, count (the maximum number of entries) and buffer (the table)
 * of D_00527270, and writes the cache back over the table's 64-byte entries when the count is not negative. Fails with
 * -100 when the library is not bound (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore
 * D_004533C4 (PollSema). On success it records 13 as the pending call (D_004533C0) and keeps the lock until
 * func_0040A360 sees the call finish; when sceSifCallRpc fails it releases the lock. The number of entries arrives as
 * func_0040A360's result.
 *
 * cPS2MemCard's func_002C4210 lists up to 6 entries and func_002C42B0 one entry.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param s  the path or pattern
 * @param c  the listing mode
 * @param d  the maximum number of entries
 * @param e  the entry table (64 bytes per entry)
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 13, 1, &D_00527270, 1044, D_00528780, 4, 0, 0);
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
/**
 * End callback of func_0040A858: copies the returned current directory to the caller's buffer.
 *
 * When @p dst is set it copies the string in D_00527780 (read through the uncached mirror; at most 1023 characters)
 * with func_0041605C and terminates it.
 *
 * @param dst  the caller's buffer, or 0
 */
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
/**
 * Starts changing a memory card's current directory (call 12, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into D_00527270.name (at most 1023
 * characters, always terminated). Fills port and slot of D_00527270 and points its buffer at D_00527780 (written back
 * from the cache, 0x400 bytes), where the server returns the current directory; func_0040A7D0 copies it to @p u when
 * the call ends. Fails with -100 when the library is not bound (D_005271C0.server is 0) and -200 when another call
 * still holds the lock semaphore D_004533C4 (PollSema). On success it records 12 as the pending call (D_004533C0) and
 * keeps the lock until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases the lock. No caller in
 * the source.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param s  the new directory
 * @param u  receives the current directory, or 0
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 12, 1, &D_00527270, 1044, D_00528780, 4, func_0040A7D0, u);
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
/**
 * Starts formatting a memory card (call 16, asynchronous).
 *
 * Fills port and slot of D_00527240. Fails with -100 when the library is not bound (D_005271C0.server is 0) and -200
 * when another call still holds the lock semaphore D_004533C4 (PollSema). On success it records 16 as the pending call
 * (D_004533C0) and keeps the lock until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases the
 * lock.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int func_0040A998(int a, int b) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    D_00527240.slot = b;
    r = sceSifCallRpc(&D_005271C0, 16, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * Starts deleting a file or directory on a memory card (call 15, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into D_00527270.name (at most 1023
 * characters, always terminated). Fills port and slot of D_00527270 and clears its mode. Fails with -100 when the
 * library is not bound (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore D_004533C4
 * (PollSema). On success it records 15 as the pending call (D_004533C0) and keeps the lock until func_0040A360 sees the
 * call finish; when sceSifCallRpc fails it releases the lock.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param s  the path
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 15, 1, &D_00527270, 1044, D_00528780, 4, 0, 0);
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
/**
 * Starts flushing a memory-card file (call 10, asynchronous).
 *
 * Puts @p a into the fd field of D_00527240. Fails with -100 when the library is not bound (D_005271C0.server is 0) and
 * -200 when another call still holds the lock semaphore D_004533C4 (PollSema). On success it records 10 as the pending
 * call (D_004533C0) and keeps the lock until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases
 * the lock.
 *
 * @param a  the file descriptor
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int func_0040AB80(int a) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.fd = a;
    r = sceSifCallRpc(&D_005271C0, 10, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * Starts changing a memory-card file's information record (call 14, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into D_00527270.name (at most 1023
 * characters, always terminated). Masks @p e to its low three bits and stores it as the mode, copies the 64-byte record
 * *@p c into D_00527200 and points the buffer at it, then flushes the cache. Fails with -100 when the library is not
 * bound (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore D_004533C4 (PollSema). On
 * success it records 14 as the pending call (D_004533C0) and keeps the lock until func_0040A360 sees the call finish;
 * when sceSifCallRpc fails it releases the lock. No caller in the source.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param s  the path
 * @param c  the new information record
 * @param e  which parts to change (bits 0..2)
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(h, 14, 1, &D_00527270, 1044, D_00528780, 4, 0, 0);
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
/**
 * Starts renaming a file on a memory card (call 14 with the rename flag, asynchronous).
 *
 * An empty or null old name, or a null new name, releases the lock and fails with -210. Sets mode 16 in D_00527270,
 * copies the old name (at most 1023 characters) and the new name into D_00527220 (32 bytes; byte 31 cleared), the name
 * field of the 64-byte record at D_00527200 that the buffer points at, and flushes the cache. Fails with -100 when the
 * library is not bound (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore D_004533C4
 * (PollSema). On success it records 19 as the pending call (D_004533C0) and keeps the lock until func_0040A360 sees the
 * call finish; when sceSifCallRpc fails it releases the lock. (19 rather than the call number). No caller in the
 * source.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param s  the current path
 * @param t  the new name
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 14, 1, &D_00527270, 1044, D_00528780, 4, 0, 0);
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
/**
 * Starts memory-card call 17 for a port and slot (asynchronous).
 *
 * Fills port and slot of D_00527240. Fails with -100 when the library is not bound (D_005271C0.server is 0) and -200
 * when another call still holds the lock semaphore D_004533C4 (PollSema). On success it records 17 as the pending call
 * (D_004533C0) and keeps the lock until func_0040A360 sees the call finish; when sceSifCallRpc fails it releases the
 * lock. No caller in the source.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int func_0040AF58(int a, int b) {
    int r;
    if (D_005271C0.server == 0)
        return -100;
    if (PollSema(D_004533C4) < 0)
        return -200;
    D_00527240.port = a;
    D_00527240.slot = b;
    r = sceSifCallRpc(&D_005271C0, 17, 1, &D_00527240, 48, D_00528780, 4, 0, 0);
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
/**
 * Starts memory-card call 18 for a path on a port and slot (asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into D_00527270.name (at most 1023
 * characters, always terminated). Fills port and slot of D_00527270. Fails with -100 when the library is not bound
 * (D_005271C0.server is 0) and -200 when another call still holds the lock semaphore D_004533C4 (PollSema). On success
 * it records 18 as the pending call (D_004533C0) and keeps the lock until func_0040A360 sees the call finish; when
 * sceSifCallRpc fails it releases the lock. No caller in the source.
 *
 * @param a  the memory-card port
 * @param b  the slot
 * @param s  the path
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
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
    r = sceSifCallRpc(&D_005271C0, 18, 1, &D_00527270, 1044, D_00528780, 4, 0, 0);
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
/**
 * Clears the flag D_004533E0 that func_0040B130 sets when it sets up the multi-client RPC layer.
 *
 * Only func_00319800, which no retail code calls, calls it.
 */
void func_0040B1F8(void) {
    D_004533E0[0] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B208);
#ifdef SKIP_ASM
struct E0B208 { char pad[0x10]; int f10; int f14; int f18; char pad2[0x24]; };
struct S0B208 { int f0; struct E0B208 *tab; int n; };
extern int DIntr_rq(void) __asm__("DIntr");
extern void EIntr(void);
struct E0B208 *f0B208(struct S0B208 *s) __asm__("func_0040B208");
/**
 * Takes a free request packet from a packet pool and gives it the next serial number.
 *
 * With interrupts off (DIntr; EIntr turns them back on unconditionally) it scans the pool's table for the first packet
 * whose flags (+0x10) lack bit 0, sets them to (index << 16) | 5, points +0x14 at the packet itself and stores the pool
 * counter's next value (+0x0) as the serial (+0x18); when that value is 1 the counter is advanced once more.
 * func_0040B2B0 frees a packet again.
 *
 * @param s  the pool: serial counter, packet table, packet count
 * @return the packet, or 0 when all are in use
 */
struct E0B208 *f0B208(struct S0B208 *s) {
    struct E0B208 *e;
    int i;
    int n;
    int c;
    int id;
    int seq;
    DIntr_rq();
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
            EIntr();
            return e;
        }
        i++;
        e++;
    }
    EIntr();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B2B0);
#ifdef SKIP_ASM
/**
 * Frees a request packet taken by func_0040B208.
 *
 * Clears its serial (+0x18) and the in-use bit 0 of its flags (+0x10).
 *
 * @param p  the packet
 */
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
/**
 * SIF command handler for the multi-client RPC layer's replies.
 *
 * func_0040B130 installs it. By the reply code at +0x20: 0x8000001A (call done) runs the client's end function (+0x1C)
 * with its argument (+0x20) and signals the client lock (+0x28); 0x80000019 (bind done) stores the server (+0x24) and
 * two buffer words (+0x14, +0x18) from the reply and signals the lock; 0x8000001D (terminate done) stores the reply
 * word in +0x2C and signals the lock; 0x8000001C does nothing. In every case it then signals the client's wait
 * semaphore (+0x8) when it is not negative, frees the request packet (func_0040B2B0) and clears the client's packet
 * pointer.
 *
 * @param e  the reply: client at +0x1C, code at +0x20, data words from +0x24
 */
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
/**
 * Binds a multi-client RPC client to a server with the default parameters 0x800, 0x2000 and 0x20.
 *
 * Calls func_0040B400 with them.
 *
 * @param a  the client block
 * @param b  the server's RPC number
 * @param c  the mode (must be 0)
 * @return func_0040B400's result
 */
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
/**
 * Binds a multi-client RPC client to an IOP server and waits for the answer.
 *
 * A mode other than 0 prints "mode must be 0 in this version." and returns -1. Creates the client's lock semaphore
 * (+0x28, count 1) and takes it, stores the RPC number (+0x10), clears the server (+0x24) and keeps @p d, @p e and @p f
 * at +0x30..+0x38. Takes a packet from the pool D_00528FC0 (func_0040B208), records it and its serial (+0x0, +0x4) and
 * fills it with the number, the three parameters and the client. Creates a wait semaphore (+0x8), sends command
 * 0x80000019 with the 64-byte packet (func_004261B0) and waits until func_0040B2D0 has stored the reply. Deletes the
 * wait semaphore, and the lock too when no server was found (+0x24 still 0).
 *
 * Quirk: it returns 0 even when no server was bound; callers must check +0x24. func_0040B3D8 supplies the defaults; the
 * DNAS import table func_003FD810 holds it.
 *
 * @param cc  the client block
 * @param a  the server's RPC number
 * @param flag  the mode (must be 0)
 * @param d  first bind parameter (0x800 by default)
 * @param e  second bind parameter (0x2000 by default)
 * @param f  third bind parameter (0x20 by default)
 * @return 0 when the request was answered, -1 for a bad mode or no free packet, -2 when the command could not be sent,
 *   -3 when a semaphore could not be created
 */
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
/**
 * Ends a multi-client RPC client's binding and returns the server's answer.
 *
 * A mode other than 0 prints "mode must be 0 in this version." and returns -1. Takes the client's lock (+0x28), then a
 * packet from D_00528FC0 and fills it with the RPC number (+0x10), the server (+0x24) and the client. Creates a wait
 * semaphore (+0x8), sends command 0x8000001D (func_004261B0), waits for func_0040B2D0, then deletes the wait semaphore
 * and the client's lock.
 *
 * Likely bug: when no packet is free it returns -1 with the lock still taken, so later calls on the client block.
 *
 * @param a  the client block
 * @param b  the mode (must be 0)
 * @return the reply word (+0x2C), -1, -2 when the command could not be sent, or -3 when the semaphore could not be
 *   created
 */
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
/**
 * Calls a function of a bound IOP RPC server through the multi-client RPC layer.
 *
 * A mode other than 0 prints "mode must be 0 in this version." and returns -1, which leaves the branch for mode bit 0
 * (no wait; packet +0x30 = whether an end function is set) unreachable. Takes the client's lock (+0x28); a send or
 * receive buffer that is not 64-byte aligned prints D_004981A0 / D_004981E8 (with D_004981D8) and returns -1. Takes a
 * packet from D_00528FC0, stores its serial (+0x4), @p h (+0x1C) and @p i (+0x20) in the client and the RPC number,
 * send size, receive buffer and size, client and server in the packet. Writes the cache back over both buffers (once,
 * with the larger size, when they are the same). Creates a wait semaphore (+0x8), sends command 0x8000001A with the
 * packet and @p e bytes from @p d to the server's buffer (+0x14) (func_004261B0), waits for func_0040B2D0 and deletes
 * the semaphore.
 *
 * Likely bug: the alignment and no-packet failures return with the client's lock still taken.
 *
 * @param c  the client block
 * @param a  the function number
 * @param flag  the mode (must be 0)
 * @param d  the send buffer (64-byte aligned)
 * @param e  the send size
 * @param f  the receive buffer (64-byte aligned)
 * @param g  the receive size
 * @param h  the end function, or 0
 * @param i  the end function's argument
 * @return 0, -1, -2 when the command could not be sent, or -3 when the semaphore could not be created
 */
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
extern int sceOpen(char *, int);
extern void sceRead(int, char *, int);
extern void sceClose(int);
extern char D_00498220[];
/**
 * Reads the console's ROM version string once and returns its buffer.
 *
 * While D_00453400 is still empty it opens the file D_00498220 ("rom0:ROMVER") with sceOpen (mode 1), reads 14 bytes
 * into D_00453400 (sceRead) and closes it (sceClose). The buffer stays empty when the open fails.
 *
 * @return the address of D_00453400
 */
int func_0040B938(void) {
    int h;
    if (D_00453400[0] == 0) {
        h = sceOpen(D_00498220, 1);
        if (h >= 0) {
            sceRead(h, D_00453400, 14);
            sceClose(h);
        }
    }
    return (int)D_00453400;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B9A8);
#ifdef SKIP_ASM
extern int func_0040B938(void);
/**
 * Returns whether the console is a T10000 development kit.
 *
 * Reads the ROM version string first when needed (func_0040B938) and tests its region character (byte 4) for 'T'. The
 * console-setting getters func_0040B9E8 .. func_0040BC08 use it to choose the override record D_004533F8 over the OSD
 * configuration.
 *
 * @return 1 for a T10000 (tool) console, else 0
 */
int func_0040B9A8(void) {
    if (D_00453400[0] == 0)
        func_0040B938();
    return D_00453400[4] == 84;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B9E8);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void GetOsdConfigParam(unsigned *);
/**
 * Returns the console's language setting.
 *
 * On a T10000 development kit (func_0040B9A8) it returns the override byte D_004533FC that func_0040BA48 sets.
 * Otherwise it reads the OSD configuration (GetOsdConfigParam): Consoles whose OSD configuration has version 0 (bits
 * 13..15) give the Japanese-language flag (bit 4)., newer ones the language field (bits 16..20). Quirk: it calls
 * GetOsdConfigParam once more at the start, unused. cPS2ConsoleConfig_cPS2ConsoleConfig keeps 0..7 as its language.
 *
 * @return the language number
 */
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
/**
 * Sets the 8-byte console-setting record that T10000 development kits use instead of the OSD.
 *
 * Copies *@p p into D_004533F8: the time zone in minutes (+0x0, short), then one byte each for the aspect, date format,
 * language, S/PDIF, summer time and time notation (read by func_0040BAE8, func_0040BA68, func_0040BB38, func_0040B9E8,
 * func_0040BAA8, func_0040BBA0 and func_0040BC08). No caller in the source.
 *
 * @param p  the new settings
 */
void func_0040BA48(sSysConfig *p) {
    D_004533F8 = *p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BA68);
#ifdef SKIP_ASM
extern int func_0040B9A8(void);
extern void GetOsdConfigParam(unsigned *);
/**
 * Returns the console's screen aspect setting.
 *
 * On a T10000 development kit (func_0040B9A8) it returns the override byte D_004533FA that func_0040BA48 sets.
 * Otherwise it returns the screen-type field of the OSD configuration (bits 1..2).
 *
 * @return the aspect setting (0..2)
 */
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
/**
 * Returns the console's S/PDIF (digital audio output) setting.
 *
 * On a T10000 development kit (func_0040B9A8) it returns the override byte D_004533FD that func_0040BA48 sets.
 * Otherwise it returns bit 0 of the OSD configuration. cPS2ConsoleConfig_cPS2ConsoleConfig treats 0 as enabled.
 *
 * @return the S/PDIF setting (0 on, 1 off)
 */
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
/**
 * Returns the console's time zone as minutes from GMT.
 *
 * On a T10000 development kit (func_0040B9A8) it returns the override D_004533F8.f0 that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 540 (GMT+9, Japan). Newer ones give the signed field in bits
 * 21..31 of the OSD configuration (GetOsdConfigParam).
 *
 * @return the offset in minutes
 */
int func_0040BAE8(void) {
    unsigned buf[1];
    int r;
    if (func_0040B9A8()) {
        r = D_004533F8.f0;
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) r = 540; else r = (int)buf[0] >> 21;
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
/**
 * Returns the console's date format setting.
 *
 * On a T10000 development kit (func_0040B9A8) it returns the override byte D_004533FB that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 0. Newer ones read the second byte of the extra OSD
 * configuration (GetOsdConfigParam2) and return its bits 6..7. cPS2ConsoleConfig_cPS2ConsoleConfig keeps 0..2.
 *
 * @return the date format (0..2)
 */
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
/**
 * Returns whether the console's summer time (daylight saving) setting is on.
 *
 * On a T10000 development kit (func_0040B9A8) it returns the override byte D_004533FE that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 0. Newer ones return bit 4 of the extra OSD configuration
 * byte (GetOsdConfigParam2).
 *
 * @return 1 when summer time is on, else 0
 */
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
/**
 * Returns the console's time notation setting (24- or 12-hour clock).
 *
 * On a T10000 development kit (func_0040B9A8) it returns the override byte D_004533FF that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 0. Newer ones return bit 5 of the extra OSD configuration
 * byte (GetOsdConfigParam2). cPS2ConsoleConfig_cPS2ConsoleConfig keeps 0 or 1.
 *
 * @return the time notation (0 or 1)
 */
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
/**
 * Converts a binary number from 0 to 99 into packed BCD.
 *
 * @param decimal  the number
 * @return the BCD byte
 */
unsigned char func_0040BC70(unsigned char decimal) {
    return decimal + (decimal / 10) * 6;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BCA0);
#ifdef SKIP_ASM
/**
 * Converts a packed BCD byte into binary.
 *
 * @param bcd  the BCD byte
 * @return the number
 */
unsigned char func_0040BCA0(unsigned char bcd) {
    unsigned char t = bcd >> 4;
    t = t * 6;
    return bcd - t;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BCC0);
#ifdef SKIP_ASM
/**
 * Converts a console clock record's BCD fields to binary in place.
 *
 * Converts bytes 1..3 (second, minute, hour) and 5..7 (day, month, year) with func_0040BCA0.
 *
 * @param clock  the 8-byte clock record
 */
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
/**
 * Converts a console clock record's binary fields back to BCD in place.
 *
 * Converts bytes 1..3 (second, minute, hour) and 5..7 (day, month, year) with func_0040BC70.
 *
 * @param clock  the 8-byte clock record
 */
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
/**
 * Advances a binary clock record by one day.
 *
 * Uses the month lengths in D_00498230, with February 29 in years divisible by 4 (two-digit year). Past the end of the
 * month it goes to day 1 of the next month, and after December to January of the next year (99 wraps to 0).
 *
 * @param a  the clock record (binary fields)
 */
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
/**
 * Moves a binary clock record back by one day.
 *
 * Before day 1 it goes to the last day of the previous month (month lengths in D_00498230, February 29 in years
 * divisible by 4), and before January to December of the previous year (0 wraps to 99).
 *
 * @param a  the clock record (binary fields)
 */
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
/**
 * Advances a binary clock record by one hour, moving to the next day at 24.
 *
 * @param clock  the clock record (binary fields)
 */
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
/**
 * Moves a binary clock record back by one hour, moving to the previous day before 0.
 *
 * @param clock  the clock record (binary fields)
 */
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
/**
 * Adds a number of minutes to a BCD console clock record, carrying into hours and days.
 *
 * Converts the record to binary (func_0040BCC0), adds @p deltaMinutes to the minute, then moves the hour forward
 * (func_0040BEF0) or back (func_0040BF20) by one per 60 minutes, and converts it back to BCD (func_0040BD28). Likely
 * bug: the forward loop runs only while the minute is above 60, so a sum of exactly 60 (or 120, ...) leaves minute 60
 * in the record. Seconds are not touched.
 *
 * @param clockAddr  the 8-byte clock record
 * @param deltaMinutes  minutes to add (may be negative)
 */
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
/**
 * Converts a console clock record from Japan time to GMT.
 *
 * Subtracts 540 minutes (9 hours) through func_0040BF48.
 *
 * @param clockAddr  the BCD clock record
 */
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
/**
 * Converts a console clock record from Japan time to the console's local time.
 *
 * Adds the time zone (func_0040BAE8) minus 540 minutes, plus 60 when summer time is on (func_0040BBA0), through
 * func_0040BF48. cPS2ConsoleConfig_cPS2ConsoleConfig converts the clock read by func_00402520 with it.
 *
 * @param clockAddr  the BCD clock record
 */
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
/**
 * Initialises the USB keyboard client: binds its server, checks the connection count and sets up every keyboard.
 *
 * Calls func_00426408(0) and binds SIF RPC server 0x80000211 into D_005290A0, retrying with a busy wait until the
 * server is set; a bind error prints "libusbkb : bind errr". Creates the semaphore D_00453438 ("libusbkb : CreateSema
 * Error" on failure), reads the information block with sceUsbKbGetInfo and waits for it (func_0040C810). The maximum
 * number of keyboards must be 1..127; it goes to D_00529000.n and *@p out. Takes the keyboard array from D_005291D0 and
 * sets every keyboard to LED mode 1, code type 1, arrangement 1, raw mode off and an idle repeat. Clears the previous
 * report, installs func_0040CF88 as INTC 3 (vertical blank) handler (func_00423A80; id in D_00529000.thread) and
 * enables that line (func_004248E8).
 *
 * Every failure after the semaphore deletes it again and sets D_00453438 to -1.
 *
 * @param out  receives the maximum number of keyboards
 * @return 0 on success, -1 on any failure
 */
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
extern int DIntr_0() __asm__("DIntr");
extern void EIntr(void);
extern int RemoveIntcHandler(int, int);
extern void func_00424880(int);
extern int DeleteSema(int);
extern char D_00498278[];
extern char D_004982A8[];
/**
 * Shuts the USB keyboard client down: removes its vertical-blank handler and deletes its semaphore.
 *
 * With interrupts off (DIntr; restored with EIntr when they were on) it removes the INTC 3 handler D_0052909C and, when
 * that worked, disables the line (func_00424880). A failed removal prints "libusbkb.a : RemoveIntcHandler() --> error".
 * Deletes the semaphore D_00453438 when it exists and sets it to -1; a failure prints "libusbkb.a : DeleteSema() -->
 * error" and keeps the value. The server binding stays. func_003F62F8 calls it.
 *
 * @return 0 when both steps worked, else -1
 */
int func_0040C220(void) {
    int n = 0;
    int s, r;
    s = DIntr_0();
    r = RemoveIntcHandler(3, D_0052909C);
    if (r == 0)
        func_00424880(3);
    if (s)
        EIntr();
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
    result = sceSifCallRpc(&D_005290A0, 1, 1, D_00529100, 16, D_00529140, 144, func_0040C8B0, 0);
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
        int result = sceSifCallRpc(&D_005290A0, 2, 1, D_00529100, 16, D_00529140, 80, func_0040C8B0, 0);
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
        int result = sceSifCallRpc(&D_005290A0, 3, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
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
        int result = sceSifCallRpc(&D_005290A0, 4, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
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
        int rpcResult = sceSifCallRpc(&D_005290A0, 5, 1, D_00529100, 16, D_00529140, 0x10, func_0040C8B0, 0);
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
/**
 * Sets a keyboard's key-repeat delay and rate (in vertical blanks).
 *
 * Stores @p c as the rate and @p b as the delay of keyboard @p a's repeat state and clears its count; func_0040CF88
 * counts and func_0040C980 repeats. Unlike its neighbours it does not check @p a against the keyboard count.
 * func_003F6358 sets 30 and 2.
 *
 * @param a  the keyboard number
 * @param b  vertical blanks before the first repeat
 * @param c  vertical blanks between repeats
 * @return 0, -11 for a negative delay or -12 for a negative rate
 */
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
/**
 * Turns a keyboard's key-code translation on or off.
 *
 * Stores @p b in the keyboard's code type (f04), which func_0040D118 reads. func_003F6358 sets 0.
 *
 * @param a  the keyboard number
 * @param b  0 raw codes, 1 translated
 * @return 0, -11 for a bad keyboard or -12 for a bad type
 */
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
/**
 * Selects a keyboard's key layout table (0..2).
 *
 * Stores @p b in the keyboard's arrangement (f08), which func_0040D268 reads. func_002668A8 sets 0.
 *
 * @param a  the keyboard number
 * @param b  the layout (0..2)
 * @return 0, -11 for a bad keyboard or -12 for a bad layout
 */
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
/**
 * Waits for or polls the pending USB keyboard call and finishes it.
 *
 * Mode 0 waits on the semaphore D_00453438 (signalled by func_0040C8B0 when the call ends), stores func_0040C8C0's
 * result in *@p out and releases the semaphore. Mode 1 checks the RPC (sceSifCheckStatRpc) and, when it is done, stores
 * func_0040C8C0's result in *@p out.
 *
 * @param mode  0 to wait, 1 to poll
 * @param out  receives the call's result
 * @return 0 when finished, 1 while busy (mode 1), -11 for another mode, -1 before initialisation
 */
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
        if (sceSifCheckStatRpc(&D_005290A0) == 0) {
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
/**
 * End callback of every USB keyboard RPC: signals the semaphore D_00453438 from interrupt context.
 *
 * @param arg  the end-function argument (always 0, unused)
 */
void func_0040C8B0(void *arg) {
    iSignalSema(D_00453438);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040C8C0);
#ifdef SKIP_ASM
extern int (*D_00453420[])(void);
/**
 * Finishes the pending USB keyboard call by running its result handler.
 *
 * For call codes 1..5 (D_00529000.cmd) it runs the handler D_00453420[code]; then clears the pending call and its
 * output pointer.
 *
 * @return the handler's result, or 0
 */
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
/**
 * Result handler of call 1 (keyboard information): copies the reply into the caller's block.
 *
 * From the receive buffer D_00529140 it stores the maximum count (byte 0) and the connected count (byte 1) in the
 * caller's block (D_00529000.out) and copies one status byte per possible keyboard from byte 2 on. Handler 1 of
 * D_00453420.
 *
 * @return 0
 */
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
/**
 * Result handler of call 2 (read): turns the keyboard report into new key codes with key repeat.
 *
 * Returns -1 when the report's count byte is 0xFF. In raw mode (f0C 1) it copies LED, count and modifiers into the
 * caller's sKbData, keeps the non-zero codes (a single 0 when none), converts them (func_0040D118) and returns.
 *
 * Otherwise it first advances the repeat: in state 1, once the count reaches the delay, it empties the ring
 * (func_0040D010) and queues the repeated key (with a 0 before it when the delay is 2 or more and after it when the
 * rate is), then enters state 2; in state 2 it queues the key again whenever the count reaches the rate. A report
 * without keys returns one queued key (func_0040D0B0) with the previous LED and modifiers, or none. A first code of 1
 * (roll-over error) stops the repeat and returns no keys. Otherwise it empties the ring; a first code of 0 or changed
 * modifiers returns one 0 key and stops the repeat; else it returns only the codes missing from the previous report
 * D_00529008 (all of them when that was empty), converts them and starts repeating the last one when delay and rate are
 * set. With no new codes it stops the repeat once the repeated key is no longer held. The report is saved as the
 * previous one. Handler 2 of D_00453420.
 *
 * @return 0, or -1 for a bad report
 */
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
/**
 * Result handler of call 3 (location): copies the 7-byte reply into the caller's buffer.
 *
 * Fails when byte 6 of the receive buffer D_00529140 is not 0; otherwise copies bytes 0..6 to D_00529000.out. Handler 3
 * of D_00453420.
 *
 * @return 0, or -1 when the reply reports an error
 */
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
/**
 * Result handler of call 4 (LED status): checks that the reply echoes the keyboard number sent.
 *
 * Compares byte 1 of the receive buffer with byte 1 of the send buffer. Handler 4 of D_00453420.
 *
 * @return 0 when they match, else -1
 */
int func_0040CF48(void) {
    return D_00529141[0] == D_00529101[0] ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040CF68);
#ifdef SKIP_ASM
/**
 * Result handler of call 5 (LED mode): checks that the reply echoes the keyboard number sent.
 *
 * Same test as func_0040CF48. Handler 5 of D_00453420.
 *
 * @return 0 when they match, else -1
 */
int func_0040CF68(void) {
    return D_00529141[0] == D_00529101[0] ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040CF88);
#ifdef SKIP_ASM
/**
 * Vertical-blank interrupt handler that counts the key-repeat time of every keyboard.
 *
 * For each keyboard with a delay and rate set and a repeat in progress (state 1 or 2) it increments the repeat count.
 * Ends with sync and ei. func_0040C028 installs it on INTC 3.
 *
 * @return 0
 */
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
/**
 * Empties a keyboard's 4-entry key ring and clears its repeat count.
 *
 * @param i  the keyboard number
 * @return 0
 */
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
/**
 * Queues a key code in a keyboard's 4-entry key ring.
 *
 * @param a  the keyboard number
 * @param b  the key code
 * @return 0, or -1 when the ring is full
 */
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
/**
 * Takes the oldest key code from a keyboard's 4-entry key ring.
 *
 * @param a  the keyboard number
 * @param out  receives the key code
 * @return 0, or -1 when the ring is empty
 */
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
/**
 * Copies a keyboard's raw key codes to an output list, converting them when its code type asks.
 *
 * Stops after @p n codes or at a 0 code. With code type 0 (f04) the codes are copied unchanged; with 1 each is
 * converted by func_0040D268 using the keyboard's arrangement (f08), @p b and @p c. Any other code type writes nothing.
 *
 * @param idx  the keyboard number
 * @param b  the modifier-key bits
 * @param c  the LED state
 * @param n  the number of codes
 * @param src  the raw codes
 * @param dst  receives the codes (may equal @p src)
 * @return the number of codes written
 */
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
/**
 * Converts one raw USB key code into a character code for a key layout.
 *
 * @p mode picks the table D_004983C8, D_004987C8 or D_00498BC8 (each 256 plain entries, then 256 for Shift, used when
 * @p a has a Shift bit, 0x02 or 0x20). Code 0 gives 0 and codes from 0x100 come back unchanged. A table value of 0x4000
 * marks a keypad key, converted by func_0040D3C0 with the LED state; 0x8000 returns the raw code with bit 15 set. With
 * Caps Lock (LED bit 1) lower-case letters turn upper-case, and back to lower-case when Shift is held too.
 *
 * Likely bug: another @p mode prints "Arrangement error!" and then reads the null table for codes 1..0xFF.
 *
 * @param mode  the layout (0..2)
 * @param a  the modifier-key bits
 * @param b  the LED state
 * @param ch  the raw key code
 * @return the character code
 */
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
    if ((unsigned short)(v - 97) < 26 && (b & 2))
        v -= 0x20;
    if ((unsigned short)(v - 65) < 26) {
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
/**
 * Converts a keypad key's raw code for the Num Lock state.
 *
 * Looks the code up in the 21-entry table D_00498FC8[@p a & 1] (Num Lock is LED bit 0) and returns the matching code
 * with bit 0x4000 set; codes not in the table come back unchanged.
 *
 * @param a  the LED state
 * @param b  the raw key code
 * @return the converted code
 */
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
/**
 * Switches a keyboard between converted key codes and raw reports.
 *
 * When the mode changes it clears the keyboard's repeat count, ring positions and repeated key and the previous
 * report's key count. func_0040C980 returns raw reports while the mode (f0C) is 1. func_002668A8 sets 0.
 *
 * @param a  the keyboard number
 * @param b  0 converted keys, 1 raw reports
 * @return 0, -11 for a bad keyboard or -12 for a bad mode
 */
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
/**
 * Clears a keyboard's input buffer on the IOP side and its key ring and repeat state here.
 *
 * Takes the semaphore D_00453438 and sends RPC 6 with the keyboard number asynchronously (func_0040C8B0 releases the
 * semaphore when it ends), then resets the keyboard's repeat state, repeated key and ring.
 *
 * Quirks: a failed call prints the message of the read call (D_004982F8) and returns with the semaphore still taken.
 *
 * @param a  the keyboard number
 * @return 0, -1 before initialisation, -11 for a bad keyboard or -2 when the call failed
 */
int func_0040D4A0(unsigned a) {
    int r;
    sKbPort *e;
    if (D_00453438 == -1)
        return -1;
    if (a >= D_00529000.n)
        return -11;
    D_00529100[0] = a;
    WaitSema(D_00453438);
    r = sceSifCallRpc(&D_005290A0, 6, 1, D_00529100, 16, D_00529140, 0, func_0040C8B0, 0);
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
/**
 * Returns the tangent of a float angle.
 *
 * Up to pi/4 (bits 0x3F490FDA) it calls func_0040F5A8(x, 0, 1) directly; infinity and NaN give NaN (x - x). Larger
 * angles are reduced by func_0040E878 to a remainder and quadrant, and func_0040F5A8 gets the remainder with 1 for even
 * quadrants or -1 (minus the cotangent) for odd ones. cPS2GraphicsMan_SetViewportPerspective takes the reciprocal of
 * its result for the field of view.
 *
 * @param x  the angle in radians
 * @return the tangent
 */
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
/**
 * Returns the natural logarithm of a double, with the math library's error handling.
 *
 * Computes it with func_0040DB58. Unless the library mode D_00499688 is -1 (IEEE), @p x is NaN (func_0040F840) or
 * positive, it fills an error record (name D_00499070, both arguments @p x, result -3.4e38 in SVID mode 0 or 0 -
 * D_00499680 otherwise). For 0 the type is 2 (singularity) and errno (func_00415FB8) becomes 34 in POSIX mode 2, else
 * 33 when func_0040F878 returns 0; below 0 the type is 1 (domain) with errno 33. A non-zero err field overrides errno.
 * The double arithmetic runs through the soft-float routines (dpcmp, dpsub). cPS2GraphicsMan_AddTexture uses it to pick
 * a texture's mip level.
 *
 * @param x  the value
 * @return the logarithm, or the error record's result
 */
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

	/**
	 * Returns e raised to a float power, with the math library's overflow and underflow handling.
	 *
	 * Computes it with func_0040E048. Unless the library mode D_00499688 is -1, a finite @p x (func_0040F8C0) above 88.72
	 * fills an overflow record (type 3, result 3.4e38 in SVID mode or HUGE_VAL) and one below -103.97 an underflow record
	 * (type 4, result 0); errno (func_00415FB8) becomes 34 in POSIX mode or when func_0040F878 returns 0, a non-zero err
	 * field overrides it, and the record's result is returned.
	 *
	 * Callers include cLoadTimer_update, the input-map expression code (cInputExec_execute),
	 * cChunkResolve_calcMaxLightDistance and cRenderStateMan_calculateIrradianceBlend.
	 *
	 * @param x  the exponent
	 * @return the power, or the error record's result
	 */
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

	/**
	 * Returns the float remainder of x / y, with the math library's error handling for y = 0.
	 *
	 * Computes it with func_0040E300. Unless the library mode is -1 or either argument is NaN (func_0040F9D0), y = 0 fills
	 * a domain record (type 1, result x in SVID mode, else NaN) and sets errno 33 in POSIX mode or when func_0040F878
	 * returns 0. func_002B2718 (bank monitor) uses it for the position within a beat.
	 *
	 * @param x  the dividend
	 * @param y  the divisor
	 * @return the remainder, or the error record's result
	 */
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

	/**
	 * Returns the natural logarithm of a float, with the math library's error handling.
	 *
	 * Computes it with func_0040E550. Unless the library mode is -1, @p x is NaN (func_0040F9D0) or positive, it fills an
	 * error record (result -3.4e38 in SVID mode, else -HUGE_VAL): for 0 type 2 with errno 34 in POSIX mode (33 when
	 * func_0040F878 returns 0), below 0 type 1 with errno 33. cRenderStateMan_calculateIrradianceBlend uses it.
	 *
	 * @param x  the value
	 * @return the logarithm, or the error record's result
	 */
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

	/**
	 * Computes the natural logarithm of a double without error handling (the library's core routine).
	 *
	 * Writes @p x as 2^k * (1 + f) with 1 + f between sqrt(2)/2 and sqrt(2), approximates the logarithm of 1 + f with the
	 * Lg1..Lg7 polynomial in s = f / (2 + f) and adds k * ln 2 as a high and a low part. 0 gives minus infinity, negative
	 * values NaN, infinity and NaN pass through; subnormals are scaled by 2^54 first.
	 *
	 * @param x  the value
	 * @return the logarithm
	 */
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

	/**
	 * Computes e to a float power without error handling (the library's core routine).
	 *
	 * Reduces @p x to k * ln 2 + r with |r| at most ln 2 / 2 (high and low parts from D_004991B8 / D_004991C0),
	 * approximates e^r with the P1..P5 polynomial and scales by 2^k through the exponent bits. Above 88.72 it overflows
	 * (huge * huge), below -103.97 it underflows (twom100 squared); below 2^-28 it returns 1 + x; infinity gives infinity
	 * or 0 and NaN passes through.
	 *
	 * @param x  the exponent
	 * @return the power
	 */
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

	/**
	 * Computes the float remainder of x / y with the sign of x, without error handling.
	 *
	 * Works on the integer bits: y = 0, infinite x or NaN y give NaN; |x| < |y| returns x; |x| = |y| a signed zero from
	 * D_004991F0. Otherwise it aligns both mantissas (subnormals included), subtracts by shift and compare one bit at a
	 * time and rebuilds the result.
	 *
	 * @param x  the dividend
	 * @param y  the divisor
	 * @return the remainder
	 */
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

	/**
	 * Computes the natural logarithm of a float without error handling (the library's core routine).
	 *
	 * Same method as func_0040DB58 in single precision (Lg1..Lg7, ln2 high and low parts); 0 gives minus infinity, negative
	 * values NaN, subnormals are scaled by 2^25 first.
	 *
	 * @param x  the value
	 * @return the logarithm
	 */
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

	/**
	 * Reduces a float angle modulo pi/2: stores the remainder in y[0] + y[1] and returns the quadrant.
	 *
	 * Up to pi/4 the angle is the remainder (quadrant 0). Below 3pi/4 one subtraction of pi/2 (high and tail parts) does;
	 * up to 2^7 * pi/2 it rounds n = |x| * 2/pi and subtracts n * pi/2 in up to three rounds of more precise parts,
	 * checking the table D_00499540 for cancellation. Infinity and NaN give NaN (quadrant 0). Larger angles go to
	 * func_0040EC58 with the bits of 2/pi (D_00499228). Negative angles negate remainder and quadrant.
	 *
	 * @param x  the angle
	 * @param y  receives the remainder as two floats
	 * @return the quadrant number n (only n & 3 matters)
	 */
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

	/**
	 * Reduces a huge float angle modulo pi/2 by multiplying it with enough bits of 2/pi.
	 *
	 * @p x holds the angle as @p nx 8-bit chunks with @p e0 the exponent of the first. It multiplies them with terms of @p
	 * ipio2 (as floats), distils the products into 8-bit integers, takes the integer part modulo 8 as the quadrant and
	 * recomputes with more terms while the fraction has lost precision. The fraction goes to @p y in one, two or three
	 * floats depending on @p prec (term counts D_004995E8, pi/2 pieces D_004995F8). Uses func_0040F9F8 and func_0040F8E8.
	 * func_0040E878 calls it.
	 *
	 * @param x  the input chunks
	 * @param y  receives the remainder
	 * @param e0  the exponent of x[0]
	 * @param nx  the number of chunks
	 * @param prec  the precision class (0..3)
	 * @param ipio2  the integer bits of 2/pi
	 * @return the quadrant (0..7)
	 */
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

	/**
	 * Computes the tangent, or minus the cotangent, of a reduced float angle x + y (|x| up to pi/4).
	 *
	 * Below 2^-28 it returns x, or -1/x (infinity for 0) when @p iy is -1. From 0.6744 it works on pi/4 - x. Otherwise it
	 * evaluates the odd and even halves of the T[13] polynomial (D_00499648) and returns x + the correction, or for @p iy
	 * -1 minus its reciprocal computed in high and low parts.
	 *
	 * @param x  the angle's high part
	 * @param y  its low part
	 * @param iy  1 for the tangent, -1 for minus the cotangent
	 * @return the result
	 */
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
/**
 * Returns whether a double is NaN.
 *
 * Uses the integer bits of both words, so no comparison is needed.
 *
 * @param x  the value
 * @return 1 for NaN, else 0
 */
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
/**
 * Default math-error hook: returns 0, so the calling wrapper sets errno.
 *
 * It compares the record's first argument with itself (dpcmp; the source's NaN test) and returns 0 either way.
 *
 * @param errInfo  the error record
 * @return 0
 */
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
/**
 * Returns the absolute value of a float by clearing its sign bit.
 *
 * @param x  the value
 * @return |x|
 */
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
/**
 * Returns whether a float is finite (neither infinite nor NaN).
 *
 * @param x  the value
 * @return 1 when finite, else 0
 */
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

	/**
	 * Rounds a float down to the nearest integer value.
	 *
	 * Clears the fraction bits below the exponent, adding one unit first for negative non-integers; |x| < 1 gives 0 or -1,
	 * infinity and NaN return x + x.
	 *
	 * @param x  the value
	 * @return the rounded value
	 */
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
/**
 * Returns whether a float is NaN.
 *
 * @param x  the value
 * @return 1 for NaN, else 0
 */
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
/**
 * Returns x * 2^n by adjusting the float's exponent bits.
 *
 * Subnormals are scaled by 2^25 first (a very negative @p n then gives 1e-30 * x). Infinity and NaN return x + x.
 * Results past the range give 1e30 or 1e-30 times itself with x's sign (func_0040FB58); results that become subnormal
 * are built with 25 extra exponent steps and scaled back.
 *
 * @param x  the value
 * @param n  the power of two
 * @return the scaled value
 */
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
/**
 * Returns x with the sign of y.
 *
 * @param x  the magnitude
 * @param y  the sign source
 * @return the combined value
 */
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
/**
 * Runs the static destructors that the list pointer D_0045343C walks, in order, once each.
 *
 * The pointer walks a null-terminated list and is advanced before each call, so a destructor that ends the program does
 * not run twice. func_00411310 calls it first, passing its status, which is ignored.
 *
 * @param a  the status of the caller (unused)
 */
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

/**
 * Runs the static constructors listed in D_0043CE38, last to first.
 *
 * The list's first word is the count, or -1 when the list is null-terminated (then it is counted first).
 */
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
/**
 * Runs the static constructors once (func_0040FBE0), guarded by the flag D_0052AD98.
 *
 * main (main_impl) calls it first; g++ inserts this call at the start of main.
 */
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

/**
 * Divides two signed 64-bit integers, truncating toward zero (GCC runtime helper).
 *
 * Negates negative operands while tracking the result's sign, divides the magnitudes with the inlined unsigned divider
 * (__udivmoddi4) and negates the quotient when the signs differed. cDynamicMemMan_sYSTRIm and the printf core
 * func_00425638 call it.
 *
 * @param dividend  the number divided
 * @param divisor  the number to divide by
 * @return the quotient
 */
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
