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

extern int sceSifBindRpc(sSifClient *cd, unsigned int rpc_number, int mode);
extern int sceSifCallRpc(sSifClient *cd, int fno, int mode, void *send, int ssize, void *recv, int rsize,
                         void (*end_function)(void *), void *end_param);
extern int sceSifCheckStatRpc(sSifClient *cd);

extern sSifClient libpad_clients[2]; /* libpad */
extern sSifClient D_005266C0;
extern sSifClient _sceCd_cd_ncmd;
extern sSifClient _sceCd_cd_scmd;
extern sSifClient libcdvd_diskReadyClient;
extern sSifClient libmc_client; /* libmc */
extern sSifClient libusbkb_client; /* usb keyboard */

/* libmc state */
extern int libmc_pendingCall;  /* RPC function number of the pending asynchronous call, 0 when idle */
extern int libmc_lockSema;  /* semaphore serialising the libmc RPCs */
extern int libmc_sleepSema;  /* semaphore for the RPC wait sleep */
extern int *libmc_getInfoTypeOut; /* getInfo result pointers (filled by libmc_onGetInfoDone) */
extern int *libmc_getInfoFreeOut;
extern int *libmc_getInfoFormatOut;

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
extern sMcCmd libmc_cmd;

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
extern sMcFileCmd libmc_fileCmd;
extern sMcDirEntry libmc_fileInfoEntry;  /* entry sent with RPC 14 */
extern char D_00527220[];       /* libmc_fileInfoEntry.name (its own splat symbol) */
extern int libmc_callbackRecvBuffer[];        /* uncached receive buffer for the read / getInfo callbacks */
extern char libmc_chdirRecvBuffer[];       /* receive buffer for RPC 12 (libmc_onChdirDone) */
extern int libmc_rpcResult[];        /* RPC result buffer */

/* USB keyboard library state */
extern int libusbkb_rpcSema;            /* semaphore serialising the keyboard RPCs (-1 = not initialised) */
extern unsigned char libusbkb_sendBuffer[]; /* RPC send buffer (16 bytes) */
extern unsigned char libusbkb_recvBuffer[]; /* RPC receive buffer */
extern char str_sceUsbKbRead_sceSifCallRpc_pct_d[];
extern unsigned char D_00529101[]; /* libusbkb_sendBuffer[1] (own splat symbol) */
extern unsigned char D_00529141[]; /* libusbkb_recvBuffer[1] (own splat symbol) */

/* key repeat state and 4-entry key ring of one keyboard (sKbPort + 0x10) */
typedef struct {
    int state;              /* 0 idle, 1 waiting for the first repeat, 2 repeating */
    int count;              /* vblank count (libusbkb_onVblank) */
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
    int f04;                /* code translation on/off (sceUsbKbSetCodeType, libusbkb_copyKeyCodes) */
    int f08;                /* key table 0..2 (sceUsbKbSetArrangement, sceUsbKbCnvRawCode) */
    int f0C;                /* 1 = raw reports (sceUsbKbSetReadMode, libusbkb_onReadResult) */
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
    int cmd;                /* RPC of the pending asynchronous call (dispatched by libusbkb_finishPendingCall) */
    int arg;                /* its keyboard index */
    void *out;              /* its result buffer */
    int thread;
} sUsbKb;
extern sUsbKb libusbkb_state;
extern sKbPort *D_00529004; /* libusbkb_state.port (own splat symbol) */
extern sKbData D_00529008;  /* libusbkb_state.prev (own splat symbol) */
extern void *D_00529098;    /* libusbkb_state.out (own splat symbol) */
extern int D_0052909C;      /* libusbkb_state.thread (own splat symbol) */
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
extern int libcdvd_debugLevel;          /* debug print level */
extern int libcdvd_callbackThreadId;          /* callback thread id (0 = none) */
extern int D_00451618;
extern int libcdvd_ncmdKeepCmd;
/* libcdvd state shared with its RPC callback thread: volatile in every user
   (retail re-reads each one; tools/gen/phaseb_import.py VOL_DECL_OK) */
extern volatile int libcdvd_callbackSema;          /* callback thread semaphore */
extern volatile int libcdvd_setupFlag;  /* callback-entry flag: read by the SIF callback libcdvd_onPowerOffCmd */
extern volatile int _sceCd_ncmd_semid;          /* semaphores */
extern volatile int _sceCd_scmd_semid;
extern volatile int libcdvd_diskReadySema;
extern volatile int libcdvd_busyFlag;          /* busy flag */
extern volatile int libcdvd_readOptionFlags;
extern int libcdvd_ncmdBindState;
extern int libcdvd_powerOffHandlerState;
extern int libcdvd_searchFileBindState;          /* RPC bind state (< 0 = unbound) */
extern int libcdvd_diskReadyBindState;
extern int libcdvd_scmdBindState;
extern volatile int libcdvd_pendingCmd;  /* asynchronous command result */
extern volatile int libcdvd_finishedCmd;
extern void (*libcdvd_userCallback)(int); /* user callback */
extern int libcdvd_userCallbackGp;          /* its $gp */
extern void (*libcdvd_powerOffCallback)(int); /* interrupt callback */
extern int libcdvd_powerOffCallbackGp;          /* its $gp */
extern int libcdvd_powerOffCallbackArg;          /* its argument */
extern volatile int D_005264D4;
extern int libcdvd_initThreadId;          /* id of the thread that initialised the library */
extern sThreadParam libcdvd_initThreadParam;
extern sThreadParam libcdvd_callbackThreadParam;

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
} sCdReadCmd;                   /* RPC 1 parameters, sent from libcdvd_ncmdSendBuffer */
typedef struct sCdSearchCmd {
    char hdr[0x24];             /* file record returned to the caller */
    char name[0x100];
    struct sCdSearchCmd *self;
    int arg;
} sCdSearchCmd;
extern int _sceCd_ncmdrdata[];        /* RPC result buffer */
extern int libcdvd_ncmdSendBuffer[];        /* RPC send buffer */
extern char D_00452700[];
extern int D_004527C0[];
extern int _sceCd_scmdrdata[];        /* RPC result buffer */
extern int libcdvd_scmdSendBuffer[];        /* RPC send buffer */
extern int D_004531A8;          /* RPC function, send and receive size used by sceCdMmode */
extern int D_004531AC;
extern int D_004531B0;
extern sCdSearchCmd D_00526540;
extern char D_00526564[];       /* D_00526540.name (own splat symbol) */
extern int D_00526680[];
extern int libcdvd_diskReadySendBuffer[];
extern char D_00526780[];
extern int D_00453080[];

/* libpad state */
typedef struct {
    int dmaBuf;                 /* 0x100-byte DMA area holding two 0x80-byte frames */
    int actBuf;                 /* actuator command block (scePadSetActDirect) */
    int f08;
    int f0C;
    int open;                   /* non-zero while the port/slot is open */
    int f14;
    int f18;
} sPadSlot;                     /* 0x1C bytes */
extern sPadSlot libpad_slots[2][4];
extern int libpad_rpcBuffer[];        /* RPC send / receive block (128 bytes) */
extern int libpad_initializedFlag;
extern int D_004515CC;
/* one 0x80-byte pad data frame in the DMA area (libpad_getNewestFrame returns the newer one) */
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
extern sPadFrame *libpad_getNewestFrame(int port, int slot);

/* DMA controller environment (D_CTRL / PCR / SQWC / RBOR / RBSR settings, 20 bytes) */
typedef struct { unsigned char sts, std, mfd, rcyc; unsigned short h4, h6, h8, hA; void *w0C; unsigned w10; } sDmaEnv;
extern sDmaEnv libdma_lastEnv;     /* copy of the last environment set by sceDmaPutEnv */

/* system configuration copy set by func_0040BA48 (8 bytes; each byte has its own splat symbol) */
typedef struct {
    short f0;
    unsigned char f2[6];
} sSysConfig;
extern sSysConfig libscf_configOverride;
extern unsigned char D_004533FA[]; /* libscf_configOverride.f2[0..5] */
extern unsigned char D_004533FB[];
extern unsigned char D_004533FC[];
extern unsigned char D_004533FD[];
extern unsigned char D_004533FE[];
extern unsigned char D_004533FF[];

/* other shared tables and buffers */
extern int libdma_channelRegTable[];        /* DMA channel register block addresses, 10 channels */
extern int libmpeg_ipuCmdTable[];        /* table indexed by the top 4 bits of a command word (libmpeg_issueIpuCommand..libmpeg_readBits) */
extern char libscf_romVersion[];       /* 14 bytes read from the file str_rom0_ROMVER names (libscf_readRomVersion) */
extern char D_00528FC0[];       /* buffer shared by sceSifMBindRpcParam / sceSifMUnBindRpc / sceSifMCallRpc */

//100%
INCLUDE_ASM("seg/seg_2FE810", DirtyDnas_importTable);
#ifdef SKIP_ASM
/**
 * Holds the DNAS module's import table: the body is 59 words of game function and data addresses (printf-style output,
 * thread, semaphore, cache, SIF RPC, alarm, interrupt and GS calls, several data blocks), followed by a bare return.
 *
 * DirtyDnas_loadModule relocates the module's references into its "reloctable" symbol to the matching words here; the table
 * ends at DirtyDnas_importTableEnd.
 */
// From peak3project's fork (github.com/peak3project/ssx3); peak3 spelled the table as raw numbers.
// PORT: PS2-only, not code: a function whose body is an asm table of the addresses of 59 functions and data objects
// (the compiler adds the `jr $31; nop` after it). The loader DirtyDnas_loadModule hands it, with its end marker DirtyDnas_importTableEnd,
// to elfModule_relocate. The fork's config/text_word_relocs.txt gives retail's copy the same relocations. A port replaces
// it with an array of pointers.
void DirtyDnas_importTable(void) {
    __asm__ __volatile__(
        ".word kprintf\n"
        ".word scePrintf\n"
        ".word sceSifMCallRpc\n"
        ".word sceSifMBindRpcParam\n"
        ".word sceSifMUnBindRpc\n"
        ".word sceCdInit\n"
        ".word sceCdGetDiskType\n"
        ".word sceCdReadClock\n"
        ".word sceCdMmode\n"
        ".word sceCdNcmdDiskReady\n"
        ".word sceCdDiskReady\n"
        ".word _sceCd_scmd_prechk\n"
        ".word _sceCd_ncmdrdata\n"
        ".word _sceCd_ncmd_semid\n"
        ".word _sceCd_cd_ncmd\n"
        ".word _sceCd_ncmd_prechk\n"
        ".word _sceCd_scmdrdata\n"
        ".word _sceCd_cd_scmd\n"
        ".word _sceCd_scmd_semid\n"
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
        ".word _getpid\n"
        ".word _stat\n"
        ".word sceSifBindRpc\n"
        ".word iSignalSema\n"
        ".word iSetAlarm\n"
        ".word SetAlarm\n"
        ".word ReleaseAlarm\n"
        ".word DisableIntc\n"
        ".word RemoveIntcHandler\n"
        ".word GsPutIMR\n"
        ".word SetGsCrt\n"
        ".word _fstat\n"
        ".word isatty\n"
        ".word _open\n"
        ".word _write\n"
        ".word _close\n"
        ".word _lseek\n"
        ".word _read\n"
        ".word AddIntcHandler\n"
        ".word EnableIntc\n"
        ".word __sce_sema_id\n"
    );
}
#endif

/**
 * Empty function that marks the end of the DNAS import table DirtyDnas_importTable (DirtyDnas_loadModule uses the distance between the
 * two as the table size).
 */
void DirtyDnas_importTableEnd(void) {
}

//100%
INCLUDE_ASM("seg/seg_2FE810", sceGsResetGraph);
#ifdef SKIP_ASM
struct S3FD910 { short a; short b; short c; unsigned short d; int e; int f; };
extern int RemoveIntcHandler(int, int);
extern void DisableIntc(int);
extern void SetGsCrt(int, int, int);
extern int GsPutIMR(int);
void *sceGsGetGParam(void);
/**
 * Resets or re-initialises the GS display (the state block libgraph_gsParam, see sceGsGetGParam).
 *
 * Mode 0: resets the GS (CSR 0x200), stores the interlace, video mode and field/frame setting and the GS revision (CSR
 * bits 16..23), masks GS interrupts (GsPutIMR 0xFF00), removes the interrupt handler recorded in the block from INTC
 * line 2 and calls SetGsCrt. Mode 1: only flushes the GS (CSR 0x100). Mode 5: like mode 0 without the reset, the mask
 * and the handler removal. cPS2GraphicsMan_applyDisplayMode calls it.
 *
 * @param mode  0, 1 or 5
 * @param interlace  interlace flag (bit 0 goes to SetGsCrt)
 * @param videoMode  video mode
 * @param frameMode  nonzero for frame mode
 */
void sceGsResetGraph(short mode, short interlace, short videoMode, short frameMode) {
    struct S3FD910 *gsState;
    switch (mode) {
    case 0:
        gsState = sceGsGetGParam();
        *(volatile long *)0x12001000 = 0x200;
        gsState->a = interlace;
        gsState->b = videoMode;
        gsState->d = (*(volatile unsigned long *)0x12001000 >> 16) & 0xFF;
        GsPutIMR(0xFF00);
        gsState->c = !!frameMode;
        if (gsState->e) {
            DisableIntc(2);
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
        gsState = sceGsGetGParam();
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
INCLUDE_ASM("seg/seg_2FE810", sceGsGetGParam);
#ifdef SKIP_ASM
extern char libgraph_gsParam[];
/**
 * Returns the GS state block libgraph_gsParam (settings stored by sceGsResetGraph; SNDPLATFORM_init reads it too).
 *
 * @return the block
 */
void *sceGsGetGParam(void) {
    return libgraph_gsParam;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceGsResetPath);
#ifdef SKIP_ASM
typedef unsigned int u_long128_DAB0 __attribute__((mode(TI))); /* PORT: Sony's u_long128 (eetypes.h) */
extern u_long128_DAB0 libgraph_vif1ResetCodes[2]; /* VIF1 init codes sent to the VIF1 FIFO */
/**
 * Resets the drawing path to the GS: VIF1 reset with its errors masked, VU1 reset (FBRST bit 9), two quadwords from
 * libgraph_vif1ResetCodes into the VIF1 FIFO, then a GIF reset. cPS2GraphicsMan_Init calls it. Sony's libgraph sceGsResetPath.
 */
void sceGsResetPath(void) {
    *(volatile int *)0x10003C10 = 1;    /* VIF1_FBRST */
    *(volatile int *)0x10003C20 = 2;    /* VIF1_ERR */
    /* PORT: PS2-only barrier and VU0 FBRST ($vi28) write: sync.l, cfc2/ori/ctc2, sync.p. */
    __asm__ volatile("sync.l\n\tcfc2 %0, $vi28\n\tori %0, %0, 0x200\n\tctc2 %0, $vi28\n\tsync.p" : : "r"(0));
    *(volatile u_long128_DAB0 *)0x10005000 = libgraph_vif1ResetCodes[0];    /* VIF1_FIFO */
    *(volatile u_long128_DAB0 *)0x10005000 = libgraph_vif1ResetCodes[1];
    *(volatile int *)0x10003000 = 1;    /* GIF_CTRL */
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceGsSyncPath);
#ifdef SKIP_ASM
extern void scePrintf();   /* printf */
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
    scePrintf(msg);
    scePrintf(D_00497678, *SYNC_D1_CHCR);
    scePrintf(D_00497688, *SYNC_D1_TADR);
    scePrintf(D_00497698, *SYNC_D1_MADR);
    scePrintf(D_004976A8, *SYNC_D1_QWC);
    scePrintf(D_004976B8, *SYNC_D2_CHCR);
    scePrintf(D_004976C8, *SYNC_D2_TADR);
    scePrintf(D_004976D8, *SYNC_D2_MADR);
    scePrintf(D_004976E8, *SYNC_D2_QWC);
    scePrintf(D_004976F8, *SYNC_VIF1_STAT);
    scePrintf(D_00497710, *SYNC_GIF_STAT);
    return -1;
}
/**
 * Waits for (mode 0) or polls (mode 1) the DMA ch.1 and ch.2, VIF1, VU1 and GIF paths (libgraph).
 *
 * Mode 0 spins until DMA ch.1 and ch.2 CHCR bit 0x100 (STR) clear, VIF1_STAT & 0x1F000003, the VU1 busy bit 0x100 of
 * VPU_STAT and GIF_STAT & 0xC00 are all clear, one after the other, with one shared counter: past 0x1000000 polls it
 * prints "sceGsSyncPath: <path> does not terminate" and the DMA, VIF1 and GIF registers (scePrintf) and returns -1.
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
INCLUDE_ASM("seg/seg_2FE810", libdma_waitChannel);
#ifdef SKIP_ASM
extern void scePrintf();
extern char str_libdma_sync_timeout[];
/**
 * Waits for a DMA channel to finish (its CHCR STR bit to clear). After 0x1000000 polls it prints "libdma: sync timeout"
 * and force-stops the channel (sceDmaPause), which ends the wait.
 *
 * @param dmaChan  the channel's registers
 * @param waitMode  ignored (sceDmaSync passes its mode)
 */
void libdma_waitChannel(dmaChan, waitMode)
    sDmaChan *dmaChan;
    int waitMode;
{
    int n = 0x1000000;
    if (dmaChan->chcr & 0x100) {
        do {
            if (--n < 0) {
                scePrintf(str_libdma_sync_timeout);
                sceDmaPause(dmaChan);
            }
        } while (dmaChan->chcr & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdma_waitMadr);
#ifdef SKIP_ASM
extern char str_libdma_sync_timeout[];
extern void libdma_waitMadr_log(char *) __asm__("scePrintf");
extern int sceDmaPause();
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
void libdma_waitMadr(sDmaChan *dmaChan, unsigned lim, int tmo) {
    int pollsLeft = 0x1000000;
    if (tmo)
        pollsLeft = tmo;
    while (dmaChan->madr < lim) {
        if (--pollsLeft < 0) {
            libdma_waitMadr_log(str_libdma_sync_timeout);
            sceDmaPause(dmaChan);
        }
        }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdma_toDmaAddress);
#ifdef SKIP_ASM
/**
 * Converts a scratchpad address (0x7xxxxxxx) into the form DMA registers take (low 28 bits with bit 31 set); other
 * addresses pass through.
 *
 * @param addr  the address
 * @return the DMA address
 */
unsigned libdma_toDmaAddress(unsigned addr) {
    if ((addr >> 28) == 7) {
        addr &= 0xFFFFFFF;
        addr |= 0x80000000;
    }
    return addr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdma_zeroBytes);
#ifdef SKIP_ASM
/**
 * Zeroes @p n bytes at @p dst.
 *
 * @param dst  destination
 * @param n  byte count
 */
void libdma_zeroBytes(char *dst, int n) {
    while (n--)
        *dst++ = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaGetChan);
#ifdef SKIP_ASM
/**
 * Returns the register block address of DMA channel @p n (table libdma_channelRegTable), or 0 for a channel above 9.
 *
 * @param n  channel number
 * @return the register address, or 0
 */
int sceDmaGetChan(unsigned n) {
    return n < 10 ? libdma_channelRegTable[n] : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaReset);
#ifdef SKIP_ASM
extern int libdma_resetChannelFlags[];
extern void libdma_zeroBytes(char *, int);
extern int sceDmaPutEnv(sDmaEnv *);
/**
 * Resets the DMA controller.
 *
 * Clears CHCR, MADR, TADR, ASR0, ASR1 and SADR of every channel enabled in libdma_resetChannelFlags, clears the D_STAT status flags
 * and turns off its interrupt masks (writing back the set mask bits toggles them), applies an all-zero environment
 * (sceDmaPutEnv) and, when @p enableDma is 1, sets the DMA enable bit of D_CTRL. cPS2GraphicsMan_Init calls it.
 *
 * @param enableDma  1 to enable DMA afterwards
 * @return the D_CTRL enable bit as it was on entry
 */
int sceDmaReset(int enableDma) {
    sDmaEnv env;
    int i;
    int saved = *(volatile int *)0x1000E000 & 1;
    for (i = 0; i < 10; i++) {
        if (libdma_resetChannelFlags[i] != 0) {
            int *chanRegs = (int *)libdma_channelRegTable[i];
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
    libdma_zeroBytes((char *)&env, 20);
    sceDmaPutEnv(&env);
    if (enableDma == 1)
        *(volatile int *)0x1000E000 |= 1;
    return saved;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaPutEnv);
#ifdef SKIP_ASM
// DMA environment write: validate the table indices, merge them into D_CTRL, write PCR/SQWC/RBOR/RBSR, keep a copy.
// The field at +0x0C is a pointer (the ring-buffer address), stored as its integer value.
extern unsigned char D_00497818[];
extern unsigned char D_00497828[];
extern unsigned char D_00497838[];
/**
 * Applies a DMA environment to the controller registers and keeps a copy in libdma_lastEnv.
 *
 * D_CTRL gets the stall source, stall drain and MFIFO fields from the tables D_00497818 / D_00497828 / D_00497838 and
 * the release cycle (0 turns release off); D_PCR, D_SQWC, D_RBOR and D_RBSR come from the other fields.
 *
 * @param dmaEnv  the environment
 * @return 0, or -1 .. -4 when the stall source, stall drain, MFIFO or release-cycle field is out of range
 */
int sceDmaPutEnv(sDmaEnv *dmaEnv) {
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
    libdma_lastEnv = *dmaEnv;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaGetEnv);
#ifdef SKIP_ASM
/**
 * Copies the DMA environment last applied (libdma_lastEnv) into @p envOut.
 *
 * @param envOut  destination
 * @return @p envOut
 */
sDmaEnv *sceDmaGetEnv(sDmaEnv *envOut) {
    *envOut = libdma_lastEnv;
    return envOut;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaPutStallAddr);
#ifdef SKIP_ASM
/**
 * Writes the DMA stall address register D_STADR and returns its previous value.
 *
 * @param newValue  new value
 * @return the old value
 */
int sceDmaPutStallAddr(int newValue) {
    volatile int *stadrReg = (volatile int *)0x1000E060;
    int oldValue = *stadrReg;
    *stadrReg = newValue;
    return oldValue;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaSend);
#ifdef SKIP_ASM
extern unsigned libdma_toDmaAddress(unsigned);
extern void libdma_waitChannel();
/**
 * Starts a source-chain DMA transfer: waits for the channel (libdma_waitChannel), sets TADR to @p addr (DMA form), QWC 0 and
 * CHCR to chain mode, from memory, started.
 *
 * TADR is written only when it does not read back as 0xFFFFFFFF.
 *
 * @param dmaChan  the channel's registers
 * @param addr  first DMA tag
 */
void sceDmaSend(sDmaChan *dmaChan, unsigned addr) {
    unsigned physAddr = libdma_toDmaAddress(addr);
    libdma_waitChannel(dmaChan);
    if (dmaChan->tadr != 0xFFFFFFFF)
        dmaChan->tadr = physAddr;
    dmaChan->qwc = 0;
    dmaChan->chcr = (dmaChan->chcr & ~0xC) | 0x105;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaSendN);
#ifdef SKIP_ASM
/**
 * Starts a normal-mode DMA transfer from memory: waits for the channel, sets MADR (unless it reads back 0xFFFFFFFF) and
 * QWC, and starts it.
 *
 * @param dmaChan  the channel's registers
 * @param addr  source address
 * @param qwordCount  quadword count
 */
void sceDmaSendN(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = libdma_toDmaAddress(addr);
    libdma_waitChannel(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    dmaChan->chcr = (dmaChan->chcr & ~0xC) | 0x101;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaSendI);
#ifdef SKIP_ASM
/**
 * Starts an interleave-mode DMA transfer from memory (as sceDmaSendN, CHCR mode 2).
 *
 * @param dmaChan  the channel's registers
 * @param addr  source address
 * @param qwordCount  quadword count
 */
void sceDmaSendI(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = libdma_toDmaAddress(addr);
    libdma_waitChannel(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    dmaChan->chcr = (dmaChan->chcr & ~0xC) | 0x109;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaRecv);
#ifdef SKIP_ASM
extern void libdma_waitChannel();
/**
 * Starts a chain-mode DMA transfer into memory: waits for the channel, clears QWC and starts it in chain mode with the
 * direction bit clear (to memory).
 *
 * @param dmaChan  the channel's registers
 */
void sceDmaRecv(sDmaChan *dmaChan) {
    libdma_waitChannel(dmaChan);
    dmaChan->qwc = 0;
    dmaChan->chcr = ((dmaChan->chcr & ~0xC | 0x4) & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaRecvN);
#ifdef SKIP_ASM
/**
 * Starts a normal-mode DMA transfer into memory: waits for the channel, sets MADR (unless it reads back 0xFFFFFFFF) and
 * QWC, and starts it with the direction bit clear.
 *
 * @param dmaChan  the channel's registers
 * @param addr  destination address
 * @param qwordCount  quadword count
 */
void sceDmaRecvN(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = libdma_toDmaAddress(addr);
    int chcrBits;
    libdma_waitChannel(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    chcrBits = dmaChan->chcr & ~0xC;
    dmaChan->chcr = (chcrBits & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaRecvI);
#ifdef SKIP_ASM
/**
 * Starts an interleave-mode DMA transfer into memory (as sceDmaRecvN, CHCR mode 2).
 *
 * @param dmaChan  the channel's registers
 * @param addr  destination address
 * @param qwordCount  quadword count
 */
void sceDmaRecvI(sDmaChan *dmaChan, unsigned addr, int qwordCount) {
    unsigned physAddr = libdma_toDmaAddress(addr);
    int chcrBits;
    libdma_waitChannel(dmaChan);
    if (dmaChan->madr != 0xFFFFFFFF)
        dmaChan->madr = physAddr;
    dmaChan->qwc = qwordCount;
    chcrBits = (dmaChan->chcr & ~0xC) | 8;
    dmaChan->chcr = (chcrBits & ~1) | 0x100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaSync);
#ifdef SKIP_ASM
extern void libdma_waitChannel(sDmaChan *, int);
/**
 * Checks or waits for a DMA channel: mode 1 returns its STR (busy) bit; any other mode waits for it (libdma_waitChannel) and
 * returns 0.
 *
 * @param dmaChan  the channel's registers
 * @param mode  1 to poll, else wait
 * @return the busy bit, or 0
 */
int sceDmaSync(sDmaChan *dmaChan, int mode) {
    if (mode == 1)
        return ((unsigned)dmaChan->chcr >> 8) & 1;
    libdma_waitChannel(dmaChan, mode);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaWatch);
#ifdef SKIP_ASM
extern void libdma_waitMadr(sDmaChan *, unsigned, int);
/**
 * Checks or waits for a DMA channel's progress: mode 1 returns whether MADR is still below @p n; any other mode waits
 * until it is not (libdma_waitMadr with poll limit @p x) and returns 0.
 *
 * @param dmaChan  the channel's registers
 * @param n  address to reach
 * @param mode  1 to poll, else wait
 * @param x  poll limit for the wait (0 for the default)
 * @return 1 while below, or 0
 */
int sceDmaWatch(sDmaChan *dmaChan, unsigned n, int mode, int x) {
    if (mode == 1)
        return (unsigned)dmaChan->madr < n;
    libdma_waitMadr(dmaChan, n, x);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDmaPause);
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
int sceDmaPause(sDmaChan *dmaChan) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDmaRestart);
#ifdef SKIP_ASM
/**
 * Writes a DMA CHCR register and returns its previous STR bit.
 *
 * @param reg  the register
 * @param newValue  new value
 * @return bit 8 of the old value
 */
unsigned sceDmaRestart(unsigned *reg, unsigned newValue) {
    unsigned oldValue = *reg;
    *reg = newValue;
    return (oldValue >> 8) & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdev_isVu0Running);
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
int libdev_isVu0Running(void) {
    return (vu0_cfc2_678() & 0xF) == 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0Reset);
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
void sceDevVu0Reset(void) {
    vu0_ctc2_vi28_690(vu0_cfc2_vi28_690() | 0x2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0Pause);
#ifdef SKIP_ASM
// Act on the VU0 status decode: case 4 sets bit 0x1 in $vi28; returns 1 for cases 0, 3 and 4.
// PORT: PS2-only VU0 control-register access. The EE interlocks COP2 moves, so no hazard nop follows.
extern int libdev_getVu0State_6A8(void) __asm__("libdev_getVu0State");
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
 * @return 1 when VU0 is now stopped or idle (states 0, 3, 4 of libdev_getVu0State), 0 when it is halted by a D or T bit
 */
int sceDevVu0Pause(void) {
    int result = 0;
    switch (libdev_getVu0State_6A8()) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0Continue);
#ifdef SKIP_ASM
extern int libdev_getVu0State(void);
extern int sceDevVu0GetTpc(void);
extern void sceDevVu0Exec(int);
/**
 * Resumes VU0 after a D-bit, T-bit or forced stop: restarts it at its TPC (sceDevVu0GetTpc, sceDevVu0Exec).
 *
 * @return 1 when restarted, 0 when it was not stopped
 */
int sceDevVu0Continue(void) {
    int result = 0;
    int n = libdev_getVu0State();
    if (n < 4) {
        if (n > 0) {
            sceDevVu0Exec(sceDevVu0GetTpc());
            result = 1;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0PutDBit);
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
void sceDevVu0PutDBit(int enable) {
    unsigned int ctrlBits = vu0_cfc2_758();
    if (enable == 1)
        ctrlBits |= 0x4;
    else
        ctrlBits &= ~0x4;
    vu0_ctc2_758(ctrlBits);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0PutTBit);
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
void sceDevVu0PutTBit(int enable) {
    unsigned int ctrlBits = vu0_cfc2_788();
    if (enable == 1)
        ctrlBits |= 0x8;
    else
        ctrlBits &= ~0x8;
    vu0_ctc2_788(ctrlBits);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0GetDBit);
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
int sceDevVu0GetDBit(void) {
    return (vu0_cfc2_vi28_7B8() >> 2) & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0GetTBit);
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
int sceDevVu0GetTBit(void) {
    return (vu0_cfc2_7C8() >> 3) & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdev_getVu0State);
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
int libdev_getVu0State(void) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0Exec);
#ifdef SKIP_ASM
/* PORT: PS2-only VU0 macro-mode ctc2 + vcallmsr. */
/**
 * Starts VU0 at a microprogram address (CMSAR0, then vcallmsr).
 *
 * @param addr  start address (low 16 bits)
 */
void sceDevVu0Exec(int addr) {
    __asm__ volatile("ctc2 %0, $vi27\n\tvcallmsr $vi27" : : "r"(addr & 0xFFFF));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0GetTpc);
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
int sceDevVu0GetTpc(void) {
    return vu0_cfc2_828() & 0xFFFF;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0GetCnd);
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
extern int libdev_isVu0Running(void);
/**
 * Saves VU0's registers into @p cnd unless VU0 is running (Sony's libdev sceDevVu0GetCnd).
 *
 * Stores the status, MAC, clipping, R, I and Q registers and VI00..VI15. Quirk: the 32 vector registers are stored from
 * address 0, not into @p cnd (the retail base register is 0).
 *
 * @param cnd  destination
 * @return 1, or 0 when VU0 is running
 */
int sceDevVu0GetCnd(sVu0Cnd *cnd) {
    unsigned int regValue;
    if (libdev_isVu0Running() != 0)
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVu0PutCnd);
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
extern int libdev_isVu0Running(void);
/**
 * Loads VU0's registers from @p cnd unless VU0 is running: VF00..VF31, then status, MAC, clipping, R, I, Q and
 * VI00..VI15 (Sony's libdev sceDevVu0PutCnd).
 *
 * @param cnd  source
 * @return 1, or 0 when VU0 is running
 */
int sceDevVu0PutCnd(sVu0Cnd_E998 *cnd) {
    u_long128_E998 *vf = cnd->vf;
    unsigned int regValue;
    if (libdev_isVu0Running() != 0)
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
INCLUDE_ASM("seg/seg_2FE810", libdev_isAnyVuRunning);
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
int libdev_isAnyVuRunning(void) {
    unsigned int vpuStat = vu0_cfc2_vi29_EB08();
    if ((vpuStat & 0xF) == 1)
        return 1;
    else if ((vpuStat & 0xF00) == 0x100)
        return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdev_writeVu0Quad);
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
void libdev_writeVu0Quad(unsigned short addr, Q3EB38 value) {
    vu0_write_qword_EB38(addr, value.q);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdev_readVu0Quad);
#ifdef SKIP_ASM
// FAKEMATCH: `unsigned long vf` holds the saved 128-bit $vf1 between the two asm statements (only asm touches it; `Q3EB78_i`, the
// 128-bit type, swaps $2/$3); plain form 98.82%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
typedef int Q3EB78_i __attribute__((mode(TI)));
typedef union { Q3EB78_i q; unsigned long d[2]; int w[4]; short h[8]; } Q3EB78;
/* Read the qword at VU0 data address `addr` through $vi1/$vf1, restoring both. */
Q3EB78 libdev_readVu0Quad_impl(unsigned short addr) __asm__("libdev_readVu0Quad");
/**
 * Reads one quadword from VU0 memory at quadword address @p addr, restoring VI01 and VF01 afterwards.
 *
 * @param addr  quadword address
 * @return the quadword
 */
Q3EB78 libdev_readVu0Quad_impl(unsigned short addr) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1Reset);
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
void sceDevVu1Reset(void) {
    vu0_ctc2_vi28_BC0(vu0_cfc2_vi28_BC0() | 0x200);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1Pause);
#ifdef SKIP_ASM
// Act on the VU0 status decode: case 4 sets bit 0x100 in $vi28; returns 1 for cases 0, 3 and 4.
// PORT: PS2-only VU0 control-register access. The EE interlocks COP2 moves, so no hazard nop follows.
extern int libdev_getVu1State_BD8(void) __asm__("libdev_getVu1State");
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
 * @return 1 when VU1 is now stopped or idle (states 0, 3, 4 of libdev_getVu1State), 0 when it is halted by a D or T bit
 */
int sceDevVu1Pause(void) {
    int result = 0;
    switch (libdev_getVu1State_BD8()) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1Continue);
#ifdef SKIP_ASM
extern int libdev_getVu1State(void);
extern unsigned short sceDevVu1GetTpc(void);
extern void sceDevVu1Exec(int);
/**
 * Resumes VU1 after a D-bit, T-bit or forced stop: restarts it at its TPC (sceDevVu1GetTpc, sceDevVu1Exec).
 *
 * @return 1 when restarted, 0 when it was not stopped
 */
int sceDevVu1Continue(void) {
    int result = 0;
    int n = libdev_getVu1State();
    if (n < 4) {
        if (n > 0) {
            sceDevVu1Exec(sceDevVu1GetTpc());
            result = 1;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1PutDBit);
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
void sceDevVu1PutDBit(int enable) {
    unsigned int ctrlBits = vu0_cfc2_C88();
    if (enable == 1)
        ctrlBits |= 0x400;
    else
        ctrlBits &= ~0x400;
    vu0_ctc2_C88(ctrlBits);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1PutTBit);
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
void sceDevVu1PutTBit(int enable) {
    unsigned int ctrlBits = vu0_cfc2_CB8();
    if (enable == 1)
        ctrlBits |= 0x800;
    else
        ctrlBits &= ~0x800;
    vu0_ctc2_CB8(ctrlBits);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1GetDBit);
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
int sceDevVu1GetDBit(void) {
    return (vu0_cfc2_CE8() >> 10) & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1GetTBit);
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
int sceDevVu1GetTBit(void) {
    return (vu0_cfc2_CF8() >> 11) & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdev_getVu1State);
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
int libdev_getVu1State(void) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1Exec);
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
void sceDevVu1Exec(int value) {
    vu0_ctc2_D50(value & 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1GetTpc);
#ifdef SKIP_ASM
extern void libdev_readVu0Quad(void *, int);
/**
 * Returns VU1's TPC (the address it stopped at), read from VU0 memory quadword 0x43A.
 *
 * @return the address
 */
unsigned short sceDevVu1GetTpc(void) {
    unsigned short value;
    libdev_readVu0Quad(&value, 1082);
    return value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1GetCnd);
#ifdef SKIP_ASM
typedef int Q3ED88_i __attribute__((mode(TI)));
typedef union { Q3ED88_i q; int w[4]; short h[8]; } Q3ED88;
typedef struct { Q3ED88 a[32]; int b[7]; short c[16]; } S3ED88;
extern int e3ED88_a(void) __asm__("libdev_isAnyVuRunning");
extern Q3ED88 e3ED88_b(unsigned short) __asm__("libdev_readVu0Quad");
int sceDevVu1GetCnd_impl(S3ED88 *s) __asm__("sceDevVu1GetCnd");
/**
 * Saves VU1's registers into @p cnd unless a VU is running (libdev_isAnyVuRunning), reading them through VU0 memory: VF00..VF31
 * (0x400..0x41F), the control registers at 0x430..0x432 and 0x434..0x437, and VI00..VI15 (0x420..0x42F).
 *
 * @param cnd  destination
 * @return 1, or 0 when a VU is running
 */
int sceDevVu1GetCnd_impl(S3ED88 *cnd) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVu1PutCnd);
#ifdef SKIP_ASM
typedef int Q3EF18_i __attribute__((mode(TI)));
typedef union { Q3EF18_i q; unsigned long d[2]; int w[4]; short h[8]; } Q3EF18;
typedef struct { Q3EF18 a[32]; unsigned b[7]; unsigned short c[16]; } S3EF18;
extern int e3EF18_a(void) __asm__("libdev_isAnyVuRunning");
extern void e3EF18_b(unsigned short, Q3EF18) __asm__("libdev_writeVu0Quad");
int sceDevVu1PutCnd_impl(S3EF18 *s) __asm__("sceDevVu1PutCnd");
/**
 * Loads VU1's registers from @p cnd unless a VU is running, writing them through VU0 memory (the layout of
 * sceDevVu1GetCnd).
 *
 * @param cnd  source
 * @return 1, or 0 when a VU is running
 */
int sceDevVu1PutCnd_impl(S3EF18 *cnd) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVif0Reset);
#ifdef SKIP_ASM
/**
 * Resets VIF0 and masks its DMA-tag and command errors (FBRST 1, ERR 6).
 */
void sceDevVif0Reset(void) {
    *(volatile int *)0x10003810 = 1;
    *(volatile int *)0x10003820 = 6;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVif0Pause);
#ifdef SKIP_ASM
extern int libdev_getVif0State(void);
/**
 * Stops VIF0 when it is busy: mode 0 forces a break (FBRST 2), mode 1 requests a stop (FBRST 4).
 *
 * @param mode  0 or 1
 * @return 1, or 0 when VIF0 is idle or already stopped
 */
int sceDevVif0Pause(int mode) {
    int vifState = libdev_getVif0State();
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVif0Continue);
#ifdef SKIP_ASM
extern int libdev_getVif0State(void);
/**
 * Lets a stopped VIF0 continue (FBRST 8).
 *
 * @return 1, or 0 when it was not stopped
 */
int sceDevVif0Continue(void) {
    if (libdev_getVif0State() != 1)
        return 0;
    *(volatile int *)0x10003810 = 8;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVif0PutErr);
#ifdef SKIP_ASM
/**
 * Sets VIF0's error masks (ERR): the interrupt-bit, DMA-tag-error and command-error masks.
 *
 * @param maskInt  bit 0
 * @param maskTagErr  bit 1
 * @param maskCmdErr  bit 2
 * @return the value written
 */
unsigned sceDevVif0PutErr(unsigned maskInt, unsigned maskTagErr, unsigned maskCmdErr) {
    unsigned errReg = maskInt | (maskTagErr << 1) | (maskCmdErr << 2);
    *(volatile unsigned *)0x10003820 = errReg;
    return errReg;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceDevVif0GetErr);
#ifdef SKIP_ASM
/**
 * Returns VIF0's ERR register.
 *
 * @return the register
 */
int sceDevVif0GetErr(void) {
    return *(volatile int *)0x10003820;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libdev_getVif0State);
#ifdef SKIP_ASM
/**
 * Returns VIF0's state from its STAT register: 0 idle, 1 stopped (stop, force-break or interrupt stall), 2 waiting, 3
 * error, 4 running.
 *
 * @return the state
 */
int libdev_getVif0State(void) {
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVif0GetCnd);
#ifdef SKIP_ASM
struct S3FF1B0 { int w[11]; short h[4]; char c[4]; };
extern int libdev_getVif0State(void);
#define R(a) (*(volatile int *)(a))
/**
 * Saves VIF0's registers into @p cnd when it is idle or stopped: R0..R3, C0..C3, MASK, CODE, STAT, ITOP, ITOPS, MARK,
 * NUM, ERR, the two CYCLE bytes and MODE.
 *
 * @param cnd  destination
 * @return 1, or 0 when VIF0 is busy
 */
int sceDevVif0GetCnd(struct S3FF1B0 *cnd) {
    unsigned cycleReg = *(volatile unsigned *)0x10003840;
    if ((unsigned)libdev_getVif0State() >= 2)
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
INCLUDE_ASM("seg/seg_2FE810", sceDevVif0PutFifo);
#ifdef SKIP_ASM
typedef int i128 __attribute__((mode(TI)));
/**
 * Writes up to @p n quadwords into the VIF0 FIFO, stopping early when STAT bit 28 is set.
 *
 * @param src  the quadwords
 * @param n  how many
 * @return the number written
 */
int sceDevVif0PutFifo(i128 *src, int n) {
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
INCLUDE_ASM("seg/seg_2FE810", libpad_sendCommandBlock);
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
extern void SyncDCache(int *, int *);                /* SyncDCache */
extern void scePrintf();                            /* printf */
extern char D_00497888[];
/**
 * Sends a pad port/slot's 32-byte command block (libpad_slots +0x4) to the IOP pad driver over SIF DMA.
 *
 * Returns 0 while the previous transfer is still pending. Otherwise increments the block's counter (word 0), writes it
 * back from the cache and DMAs it into one of two alternating 32-byte areas at +0x8. Prints "libpad: sceSifSetDma
 * faild" (when D_004515CC is set) on a pending or failed transfer.
 *
 * @param port  pad port
 * @param slot  pad slot
 * @return 1 when sent, else 0
 */
int libpad_sendCommandBlock(int port, int slot) {
    sSifDmaData dma[16];
    int *act;
    int dst;
    int id;
    int n;
    act = (int *)libpad_slots[port][slot].actBuf;
    if (libpad_slots[port][slot].f0C != 0 && sceSifDmaStat(libpad_slots[port][slot].f0C) >= 0) {
        if (D_004515CC != 0)
            scePrintf(D_00497888);
        return 0;
    }
    n = act[0] + 1;
    dst = libpad_slots[port][slot].f08 + ((n & 1) << 5);
    act[0] = n;
    SyncDCache(act, (int *)((char *)act + 0x1F));
    dma[0].addr = dst;
    dma[0].data = (unsigned int)act;
    dma[0].size = 0x20;
    dma[0].mode = 0;
    id = sceSifSetDma(dma, 1);
    if (id == 0) {
        if (D_004515CC != 0)
            scePrintf(D_00497888);
        return 0;
    }
    libpad_slots[port][slot].f0C = id;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadInit);
#ifdef SKIP_ASM
extern char D_004978D0[];
extern char D_004978F8[];
extern int scePadGetModVersion(void);
extern void scePrintf();
extern int libpad_initDriver_a(int) __asm__("libpad_initDriver");
/**
 * Starts the pad library.
 *
 * Sets libpad_initializedFlag, binds the pad driver's two SIF RPC servers 0x80000100 and 0x80000101 (retrying with a busy wait),
 * and checks the driver's version (scePadGetModVersion): with a major version other than 4 it prints "libpad: Module version
 * mismatch [libpad.a = 4.0, padman.irx = ...]" (when D_004515CC is set) and fails. Then initialises (libpad_initDriver).
 * systemInit calls it with 0.
 *
 * @param initArg  passed on to libpad_initDriver, which ignores it
 * @return libpad_initDriver's result, or 0 on a version mismatch
 */
int scePadInit(int initArg) {
    int i;
    int version;
    int majorVersion;
    libpad_initializedFlag = 1;
    for (;;) {
        sceSifBindRpc(&libpad_clients[0], 0x80000100, 0);
        if (libpad_clients[0].server != 0)
            break;
        for (i = 0x10000; i--; )
            ;
    }
    for (;;) {
        sceSifBindRpc(&libpad_clients[1], 0x80000101, 0);
        if (libpad_clients[1].server != 0)
            break;
        for (i = 0x10000; i--; )
            ;
    }
    version = scePadGetModVersion();
    majorVersion = version >> 8;
    if (majorVersion != 4) {
        if (D_004515CC != 0) {
            scePrintf(D_004978D0);
            scePrintf(D_004978F8, 4, 0, majorVersion, version & 0xFF);
        }
        return 0;
    }
    return libpad_initDriver_a(initArg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libpad_initDriver);
#ifdef SKIP_ASM
/**
 * Initialises the pad driver: marks every port/slot closed and clears two of its words, then sends RPC command 16.
 *
 * @return the driver's reply (word 3), or 0 when the call fails
 */
int libpad_initDriver(void) {
    int i;
    for (i = 0; i < 4; i++) {
        libpad_slots[0][i].open = 0;
        libpad_slots[0][i].f18 = 0;
        libpad_slots[0][i].f14 = 0;
        libpad_slots[1][i].open = 0;
        libpad_slots[1][i].f18 = 0;
        libpad_slots[1][i].f14 = 0;
    }
    libpad_rpcBuffer[0] = 16;
    libpad_rpcBuffer[4] = 0;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    return libpad_rpcBuffer[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadEnd);
#ifdef SKIP_ASM
/**
 * Ends the pad driver: RPC command 15; a reply of 1 clears libpad_initializedFlag.
 *
 * @return the reply (word 3), or 0 when the call fails
 */
int scePadEnd(void) {
    int result;
    libpad_rpcBuffer[0] = 15;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    result = libpad_rpcBuffer[3];
    if (result == 1)
        libpad_initializedFlag = 0;
    return result;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_003FF708);

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadPortClose);
#ifdef SKIP_ASM
/**
 * Closes a pad port/slot: RPC command 14, then marks it closed. cPS2PadDevice's destructor calls it.
 *
 * @param port  port
 * @param slot  slot
 * @return the reply (word 3); 0 when not open or the call fails
 */
int scePadPortClose(int port, int slot) {
    if (!libpad_slots[port][slot].open)
        return 0;
    libpad_rpcBuffer[0] = 14;
    libpad_rpcBuffer[1] = port;
    libpad_rpcBuffer[2] = slot;
    libpad_rpcBuffer[4] = 1;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    libpad_slots[port][slot].open = 0;
    return libpad_rpcBuffer[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libpad_getNewestFrame);
#ifdef SKIP_ASM
extern void SyncDCache(int *, int *);
/**
 * Returns the newer of the two 0x80-byte pad frames in a port/slot's DMA area (the one with the higher frame counter at
 * +0x58), after syncing the cache over the area (SyncDCache).
 *
 * @param port  port
 * @param slot  slot
 * @return the frame
 */
sPadFrame *libpad_getNewestFrame(int port, int slot) {
    sPadFrame *dmaArea;
    int newerOffset;
    dmaArea = (sPadFrame *)libpad_slots[port][slot].dmaBuf;
    SyncDCache((int *)dmaArea, (int *)((char *)dmaArea + 0xFF));
    newerOffset = (dmaArea[0].frame < dmaArea[1].frame) << 7;
    return (sPadFrame *)((char *)dmaArea + newerOffset);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libpad_getFrameCount);
#ifdef SKIP_ASM
/**
 * Returns the frame counter of a port/slot's newest pad frame.
 *
 * @param port  port
 * @param slot  slot
 * @return the counter, or 0 when not open
 */
int libpad_getFrameCount(int port, int slot) {
    if (!libpad_slots[port][slot].open) return 0;
    return libpad_getNewestFrame(port, slot)->frame;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadRead);
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
int scePadRead(int port, int slot, int dataOut) {
    struct B128 frameCopy;
    int *frameWords;
    int intrState;
    if (!libpad_slots[port][slot].open)
        return 0;
    intrState = DIntr();
    frameWords = (int *)&frameCopy;
    frameCopy = *(struct B128 *)libpad_getNewestFrame(port, slot);
    func_0041605C_k(dataOut, frameWords, frameWords[0x60 / 4]);
    if (intrState == 1)
        EIntr();
    return frameWords[0x60 / 4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadGetState);
#ifdef SKIP_ASM
/**
 * Returns a port/slot's pad state (frame byte +0x70; names in scePadStateIntToStr). STABLE (6) while a request is BUSY (2) is
 * reported as EXECCMD (5).
 *
 * @param port  port
 * @param slot  slot
 * @return the state, or 99 when not open
 */
int scePadGetState(int port, int slot) {
    unsigned char *frameBytes;
    if (!libpad_slots[port][slot].open)
        return 99;
    frameBytes = (unsigned char *)libpad_getNewestFrame(port, slot); /* byte reads: a sPadFrame * makes one lhu of state/reqState */
    if (frameBytes[0x70] == 6 && frameBytes[0x71] == 2)
        return 5;
    return frameBytes[0x70];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadStateIntToStr);
#ifdef SKIP_ASM
extern char *libpad_stateNameTable[];
extern char str_empty[];
extern char *strcpy(char *, const char *);
/**
 * Copies the name of pad state @p i: DISCONNECT, FINDCTP1, EXECCMD, STABLE, ERROR for 0, 2, 5, 6, 7; empty for the
 * others and for 8 or more.
 *
 * @param i  the state
 * @param dst  receives the name
 */
void scePadStateIntToStr(unsigned i, char *dst) {
    if (i < 8) {
        strcpy(dst, libpad_stateNameTable[i]);
        return;
    }
    *dst = str_empty[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadSetReqState);
#ifdef SKIP_ASM
/**
 * Sets the request state (frame byte +0x71) of a port/slot's newest pad frame; the command senders set BUSY (2).
 *
 * @param port  port
 * @param slot  slot
 * @param newReqState  the request state
 * @return 1, or 0 when not open
 */
int scePadSetReqState(int port, int slot, int newReqState) {
    if (!libpad_slots[port][slot].open)
        return 0;
    libpad_getNewestFrame(port, slot)->reqState = newReqState;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadGetReqState);
#ifdef SKIP_ASM
/**
 * Returns the request state (frame byte +0x71) of a port/slot: 0 COMPLETE, 1 FAILED, 2 BUSY.
 *
 * @param port  port
 * @param slot  slot
 * @return the request state, or 0 when not open
 */
int scePadGetReqState(int port, int slot) {
    if (!libpad_slots[port][slot].open) return 0;
    return libpad_getNewestFrame(port, slot)->reqState;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadReqIntToStr);
#ifdef SKIP_ASM
extern char *libpad_reqStateNameTable[];
extern char str_empty[];
extern char *strcpy(char *, const char *);
/**
 * Copies the name of request state @p i: COMPLETE, FAILED, BUSY; empty for 4 or more. Quirk: entry 3 of the table
 * libpad_reqStateNameTable is a null pointer, so @p i = 3 copies from address 0.
 *
 * @param i  the request state
 * @param dst  receives the name
 */
void scePadReqIntToStr(unsigned i, char *dst) {
    if (i < 4) {
        strcpy(dst, libpad_reqStateNameTable[i]);
        return;
    }
    *dst = str_empty[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadInfoAct);
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
int scePadInfoAct(int port, int slot, int actIndex, int infoKind) {
    sPadFrame *frame;
    if (!libpad_slots[port][slot].open) return 0;
    frame = libpad_getNewestFrame(port, slot);
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
INCLUDE_ASM("seg/seg_2FE810", scePadInfoComb);
#ifdef SKIP_ASM
/**
 * Returns actuator-combination information of a port/slot's pad (frame +0x40; count at +0x6B), like scePadInfoAct;
 * @p infoKind -1..2 picks byte 0..3 of entry @p combIndex.
 *
 * @param port  port
 * @param slot  slot
 * @param combIndex  entry index, or -1 for the count
 * @param infoKind  which byte (-1..2)
 * @return the value, or 0
 */
int scePadInfoComb(int port, int slot, int combIndex, int infoKind) {
    sPadFrame *frame;
    if (!libpad_slots[port][slot].open) return 0;
    frame = libpad_getNewestFrame(port, slot);
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
INCLUDE_ASM("seg/seg_2FE810", scePadInfoMode);
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
int scePadInfoMode(int port, int slot, int term, int offs) {
    sPadFrame *frame;
    int t;
    if (!libpad_slots[port][slot].open) return 0;
    frame = libpad_getNewestFrame(port, slot);
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
INCLUDE_ASM("seg/seg_2FE810", scePadSetMainMode);
#ifdef SKIP_ASM
extern int scePadSetReqState(int, int, int);
/**
 * Sets a port/slot's main mode: RPC command 6 with the mode and lock words; on success marks the request BUSY.
 *
 * @param port  port
 * @param slot  slot
 * @param modeIndex  mode index
 * @param lockMode  lock setting
 * @return the reply (word 5), or 0 when the call fails
 */
int scePadSetMainMode(int port, int slot, int modeIndex, int lockMode) {
    int result;
    libpad_rpcBuffer[3] = modeIndex;
    libpad_rpcBuffer[4] = lockMode;
    libpad_rpcBuffer[0] = 6;
    libpad_rpcBuffer[1] = port;
    libpad_rpcBuffer[2] = slot;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    result = libpad_rpcBuffer[5];
    if (result == 1)
        scePadSetReqState(port, slot, 2);
    return libpad_rpcBuffer[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadSetActDirect);
#ifdef SKIP_ASM
extern int s0_F390(int, int) __asm__("libpad_sendCommandBlock");
/**
 * Sends 6 actuator values to a port/slot's pad: copies them into its command block (+0xC, with words 1 and 2 set to 1
 * and 6) and sends it (libpad_sendCommandBlock). Needs a valid frame.
 *
 * @param port  port
 * @param slot  slot
 * @param src  the 6 values
 * @return 1 when sent, else 0
 */
int scePadSetActDirect(int port, int slot, unsigned char *src) {
    int i;
    int ok;
    int *cmdWords;
    char *cmdBytes;
    if (libpad_getNewestFrame(port, slot)->ok != 1)
        return 0;
    cmdWords = (int *)libpad_slots[port][slot].actBuf;
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
INCLUDE_ASM("seg/seg_2FE810", scePadSetActAlign);
#ifdef SKIP_ASM
extern int scePadSetReqState(int, int, int);
/**
 * Sets a port/slot's actuator alignment: RPC command 8 with 6 bytes; on success marks the request BUSY.
 *
 * @param port  pad port
 * @param slot  pad slot
 * @param data  the 6 alignment bytes
 * @return the reply (word 5), or 0 when the call fails
 */
int scePadSetActAlign(int port, int slot, unsigned char *data) {
    int i;
    unsigned char *alignBytes;
    int *rpcBlock = libpad_rpcBuffer;
    rpcBlock[0] = 8;
    rpcBlock[1] = port;
    rpcBlock[2] = slot;
    alignBytes = (unsigned char *)&rpcBlock[3];
    for (i = 0; i < 6; i++)
        alignBytes[i] = data[i];
    if (sceSifCallRpc(libpad_clients, 1, 0, rpcBlock, 128, rpcBlock, 128, 0, 0) < 0)
        return 0;
    if (rpcBlock[5] == 1)
        scePadSetReqState(port, slot, 2);
    return rpcBlock[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadGetButtonMask);
#ifdef SKIP_ASM
/**
 * Returns the little-endian word at frame +0x79 of a port/slot's pad (0x3FFFF when all pressure-sensitive buttons are
 * available, per scePadInfoPressMode). Needs a valid frame with bytes +0x64 and +0x66 at least 2.
 *
 * @param port  port
 * @param slot  slot
 * @return the word, or 0
 */
int scePadGetButtonMask(int port, int slot) {
    sPadFrame *frame;
    if (!libpad_slots[port][slot].open)
        return 0;
    frame = libpad_getNewestFrame(port, slot);
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
INCLUDE_ASM("seg/seg_2FE810", scePadSetButtonInfo);
#ifdef SKIP_ASM
extern int scePadSetReqState(int, int, int);
/**
 * Sends a port/slot's button mask (RPC command 10); on success marks the request BUSY.
 *
 * @param port  port
 * @param slot  slot
 * @param buttonMask  the mask (scePadEnterPressMode passes 0xFFF, scePadExitPressMode 0)
 * @return the reply (word 4), or 0 when the call fails
 */
int scePadSetButtonInfo(int port, int slot, int buttonMask) {
    int result;
    libpad_rpcBuffer[3] = buttonMask;
    libpad_rpcBuffer[0] = 10;
    libpad_rpcBuffer[1] = port;
    libpad_rpcBuffer[2] = slot;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    result = libpad_rpcBuffer[4];
    if (result == 1)
        scePadSetReqState(port, slot, 2);
    return libpad_rpcBuffer[4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadInfoPressMode);
#ifdef SKIP_ASM
extern int scePadGetButtonMask(int, int);
/**
 * Returns whether a port/slot's pad reports the full pressure-sensitive mask 0x3FFFF (scePadGetButtonMask).
 *
 * @param port  port
 * @param slot  slot
 * @return 1 or 0
 */
int scePadInfoPressMode(int port, int slot) {
    if (!libpad_slots[port][slot].open) return 0;
    return scePadGetButtonMask(port, slot) == 0x3FFFF;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadEnterPressMode);
#ifdef SKIP_ASM
extern int scePadSetButtonInfo(int, int, int);
/**
 * Turns on all pressure-sensitive buttons of a port/slot (scePadSetButtonInfo with mask 0xFFF).
 *
 * @param port  port
 * @param slot  slot
 * @return scePadSetButtonInfo's result, or 0 when not open
 */
int scePadEnterPressMode(int port, int slot) {
    if (!libpad_slots[port][slot].open) return 0;
    return scePadSetButtonInfo(port, slot, 0xFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadExitPressMode);
#ifdef SKIP_ASM
extern int scePadSetButtonInfo(int, int, int);
/**
 * Turns off the pressure-sensitive buttons of a port/slot (scePadSetButtonInfo with mask 0). Nothing in retail calls it.
 *
 * @param port  port
 * @param slot  slot
 * @return scePadSetButtonInfo's result, or 0 when not open
 */
int scePadExitPressMode(int port, int slot) {
    if (!libpad_slots[port][slot].open) return 0;
    return scePadSetButtonInfo(port, slot, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadSetVrefParam);
#ifdef SKIP_ASM
struct P12 { char b[12]; } __attribute__((packed));
extern int scePadSetReqState(int, int, int);
/**
 * Sends RPC command 11 with a 12-byte block for a port/slot; on success marks the request BUSY. Nothing in retail calls
 * it.
 *
 * @param port  port
 * @param slot  slot
 * @param cmdBlock  the 12-byte block
 * @return the reply (word 7), or 0 when the call fails
 */
int scePadSetVrefParam(int port, int slot, struct P12 *cmdBlock) {
    libpad_rpcBuffer[1] = port;
    libpad_rpcBuffer[0] = 11;
    libpad_rpcBuffer[2] = slot;
    *(struct P12 *)&libpad_rpcBuffer[3] = *cmdBlock;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    if (libpad_rpcBuffer[7] == 1)
        scePadSetReqState(port, slot, 2);
    return libpad_rpcBuffer[7];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadGetPortMax);
#ifdef SKIP_ASM
/**
 * Sends pad RPC command 12. Nothing in retail calls it.
 *
 * @return the reply (word 3), or 0 when the call fails
 */
int scePadGetPortMax(void) {
    libpad_rpcBuffer[0] = 12;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    return libpad_rpcBuffer[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadGetSlotMax);
#ifdef SKIP_ASM
/**
 * Sends pad RPC command 13 with one argument. Nothing in retail calls it.
 *
 * @param argWord  the argument
 * @return the reply (word 3), or 0 when the call fails
 */
int scePadGetSlotMax(int argWord) {
    libpad_rpcBuffer[1] = argWord;
    libpad_rpcBuffer[0] = 13;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    return libpad_rpcBuffer[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scePadGetModVersion);
#ifdef SKIP_ASM
/**
 * Asks the pad driver for its version (RPC command 18); scePadInit requires major version 4 (bits 8..15).
 *
 * @return the version, or 0 when the call fails
 */
int scePadGetModVersion(void) {
    libpad_rpcBuffer[0] = 18;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    return libpad_rpcBuffer[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libpad_callRpc20);
#ifdef SKIP_ASM
/**
 * Sends pad RPC command 20 with one argument. Nothing in retail calls it.
 *
 * @param argWord  the argument
 * @return the reply (word 2), or 0 when the call fails
 */
int libpad_callRpc20(int argWord) {
    libpad_rpcBuffer[1] = argWord;
    libpad_rpcBuffer[0] = 20;
    if (sceSifCallRpc(libpad_clients, 1, 0, libpad_rpcBuffer, 128, libpad_rpcBuffer, 128, 0, 0) < 0)
        return 0;
    return libpad_rpcBuffer[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_onDelayAlarm);
#ifdef SKIP_ASM
// PORT: PS2-only COP0 (enable interrupts) and memory barrier.
#define SYNC_0810() __asm__ volatile("sync")
#define EI_0810() __asm__ volatile("ei")
extern void iSignalSema(int);
/**
 * Alarm handler of the delay libcdvd_sleepTicks: signals the semaphore it waits on, then sync and ei.
 *
 * @param alarmId  alarm id (unused)
 * @param ticks  alarm time (unused)
 * @param sema  the semaphore
 */
void libcdvd_onDelayAlarm(int alarmId, int ticks, int sema) {
    iSignalSema(sema);
    SYNC_0810();
    EI_0810();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_sleepTicks);
#ifdef SKIP_ASM
extern int CreateSema(int *);
extern void libcdvd_onDelayAlarm(int, int, int);
/**
 * Sleeps the calling thread for @p ticks alarm ticks (horizontal-blank periods).
 *
 * Creates a semaphore (max count 1, initial count 0), sets a kernel alarm whose handler libcdvd_onDelayAlarm signals it, waits
 * on it and deletes it. The libcdvd sync waits (sceCdSync, sceCdSyncS) use it with 60 between polls.
 *
 * @param ticks  the delay in alarm ticks
 */
void libcdvd_sleepTicks(unsigned short ticks) {
    int semaParam[6];
    int sema;
    semaParam[1] = 1;
    semaParam[2] = 0;
    semaParam[5] = 0;
    sema = CreateSema(semaParam);
    SetAlarm(ticks, libcdvd_onDelayAlarm, sema);
    WaitSema(sema);
    DeleteSema(sema);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdCallback);
#ifdef SKIP_ASM
extern int sceCdSync(int);
extern int DIntr();
extern void EIntr(void);
/**
 * Installs the libcdvd user callback that the callback thread runs when an asynchronous command ends.
 *
 * Fails (returns 0) while an N-command is still running (sceCdSync(1)). Otherwise, with interrupts off (DIntr /
 * EIntr), stores @p cb in libcdvd_userCallback and the caller's $gp in libcdvd_userCallbackGp (the thread libcdvd_callbackThread restores that $gp
 * around the call) and returns the previous callback. Called with 0 by the EA CD streaming code to remove it.
 *
 * @param cb  the callback, given the finished command's function code; 0 to remove it
 * @return the previous callback, or 0 when a command is busy
 */
void (*sceCdCallback(void (*cb)(int)))(int) {
    void (*old)(int);
    int intrEnabled;
    if (sceCdSync(1) != 0)
        return 0;
    intrEnabled = DIntr();
    old = libcdvd_userCallback;
    libcdvd_userCallback = cb;
    __asm__ volatile("sw $28,%0" : "=m"(libcdvd_userCallbackGp)); /* the callback runs with this $gp */
    if (intrEnabled != 0)
        EIntr();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_finishNcmd);
#ifdef SKIP_ASM
extern int iSignalSema_i(int) __asm__("iSignalSema");
/**
 * Finishes a libcdvd asynchronous N-command: releases the command semaphore and wakes the callback thread.
 *
 * Copies the pending command's function code (*@p pendingCode, the global libcdvd_pendingCmd: 1 read, 3 get-TOC) to libcdvd_finishedCmd for the
 * callback. Code 11 only clears it and the busy flag libcdvd_busyFlag. Otherwise it signals the N-command semaphore
 * _sceCd_ncmd_semid (which the issuing call left held) and, when the callback thread libcdvd_callbackThreadId exists and a callback
 * libcdvd_userCallback is set, signals the thread's semaphore libcdvd_callbackSema; with no callback it clears the busy flag itself. Uses
 * iSignalSema, so it runs in the SIF RPC completion (interrupt) context: libcdvd_onReadDone calls it.
 *
 * @param pendingCode  the pending command's function code (always &libcdvd_pendingCmd)
 */
void libcdvd_finishNcmd(int *pendingCode) {
    libcdvd_pendingCmd = *pendingCode;
    libcdvd_finishedCmd = libcdvd_pendingCmd;
    if (libcdvd_pendingCmd == 11) {
        libcdvd_pendingCmd = 0;
        libcdvd_busyFlag = 0;
        return;
    }
    iSignalSema_i(_sceCd_ncmd_semid);
    if (libcdvd_callbackThreadId != 0 && libcdvd_userCallback != 0)
        iSignalSema_i(libcdvd_callbackSema);
    else
        libcdvd_busyFlag = 0;
    libcdvd_pendingCmd = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_callbackThread);
#ifdef SKIP_ASM
extern char D_004979E0[];
extern void WaitSema(int);
extern int ExitDeleteThread(void);
extern void scePrintf();
/**
 * Body of the libcdvd callback thread: runs the user callback each time an asynchronous command ends.
 *
 * Loops forever waiting on libcdvd_callbackSema. When libcdvd_pendingCmd is -1 (the shutdown request from libcdvd_shutdown) it clears the
 * busy flag, the pending code, the thread id libcdvd_callbackThreadId and D_005264D4 and exits with ExitDeleteThread. Otherwise, with
 * debug output on (libcdvd_debugLevel > 0), it prints the callback and the code ("sceCdCbfunc= %d sceCdCbfunc_num= %d"), calls
 * the callback libcdvd_userCallback with the finished command's code libcdvd_finishedCmd under the $gp saved when it was installed, and
 * clears the busy flag libcdvd_busyFlag. sceCdInitEeCB creates the thread.
 */
void libcdvd_callbackThread(void) {
    void (*fn)(int);
    for (;;) {
        WaitSema(libcdvd_callbackSema);
        if (libcdvd_pendingCmd == -1) {
            libcdvd_busyFlag = 0;
            libcdvd_pendingCmd = 0;
            libcdvd_callbackThreadId = 0;
            D_005264D4 = 0;
            ExitDeleteThread();
        }
        if (libcdvd_debugLevel > 0)
            scePrintf(D_004979E0, libcdvd_userCallback, libcdvd_finishedCmd);
        fn = libcdvd_userCallback;
        if (fn != 0 && libcdvd_finishedCmd != 0) {
            int saved;
            __asm__ volatile("move %0,$28\n\tlw $28,%1" : "=r"(saved) : "m"(libcdvd_userCallbackGp));
            fn(libcdvd_finishedCmd);
            __asm__ volatile("move $28,%0" : : "r"(saved));
        }
        libcdvd_busyFlag = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdInitEeCB);
#ifdef SKIP_ASM
extern int GetThreadId();
extern int ReferThreadStatus_i(int, sThreadParam *) __asm__("ReferThreadStatus");
extern int CreateThread(sThreadParam *);
extern void StartThread(int, int);
extern void ChangeThreadPriority(int, int);
extern void libcdvd_callbackThread(void);
/**
 * Creates the libcdvd callback thread, or changes its priority when it already exists.
 *
 * The first time (libcdvd_callbackThreadId == 0) it records the current thread id (libcdvd_initThreadId) and its status (libcdvd_initThreadParam), fills the
 * thread parameters libcdvd_callbackThreadParam (entry libcdvd_callbackThread, stack, size, priority, gp 0), creates the thread and starts it,
 * keeping its id in libcdvd_callbackThreadId. Later calls only call ChangeThreadPriority.
 *
 * @param priority  the thread priority
 * @param stackAddr  the thread's stack
 * @param stackBytes  the stack size in bytes
 * @return 1 when the thread was created, 0 when only its priority changed
 */
int sceCdInitEeCB(int priority, int stackAddr, int stackBytes) {
    int ok;
    ok = 1;
    if (libcdvd_callbackThreadId == 0) {
        libcdvd_initThreadId = GetThreadId();
        ReferThreadStatus_i(libcdvd_initThreadId, &libcdvd_initThreadParam);
        libcdvd_callbackThreadParam.stackSize = stackBytes;
        libcdvd_callbackThreadParam.entry = libcdvd_callbackThread;
        libcdvd_callbackThreadParam.stack = stackAddr;
        libcdvd_callbackThreadParam.initPriority = priority;
        libcdvd_callbackThreadParam.gpReg = 0;
        libcdvd_callbackThreadId = CreateThread(&libcdvd_callbackThreadParam);
        StartThread(libcdvd_callbackThreadId, 0);
    } else {
        ChangeThreadPriority(libcdvd_callbackThreadId, priority);
        ok = 0;
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_onReadDone);
#ifdef SKIP_ASM
struct S0B48 { int n0; int n1; unsigned char *d0; unsigned char *d1; unsigned char b0[0x40]; unsigned char b1[0x40]; };
extern void libcdvd_finishNcmd(int *);
/**
 * SIF RPC completion handler of a CD read: patches the unaligned head and tail bytes into the read buffer.
 *
 * The IOP returns the bytes of the read that share a 64-byte cache line with other data in a fixup block (@p fixupBlock, read
 * through its uncached alias): n0 bytes for d0 from b0 and n1 bytes for d1 from b1. Each byte is written through both
 * the uncached and the cached address of the destination. Then libcdvd_finishNcmd finishes the command (&libcdvd_pendingCmd).
 * sceCdRead passes it with the fixup buffer D_00452700.
 *
 * @param fixupBlock  the fixup block (the RPC end parameter)
 */
void libcdvd_onReadDone(void *fixupBlock) {
    struct S0B48 *fixup = (struct S0B48 *)((unsigned)fixupBlock | 0x20000000);
    unsigned char *uncachedDst;
    int i;
    unsigned char *cachedDst;
    if (fixup->n0 > 0) {
        uncachedDst = (unsigned char *)((unsigned)fixup->d0 | 0x20000000);
        cachedDst = fixup->d0;
        for (i = 0; i < fixup->n0; i++) {
            unsigned char byteValue = fixup->b0[i];
            uncachedDst[i] = byteValue;
            cachedDst[i] = byteValue;
        }
    }
    if (fixup->n1 > 0) {
        uncachedDst = (unsigned char *)((unsigned)fixup->d1 | 0x20000000);
        cachedDst = fixup->d1;
        for (i = 0; i < fixup->n1; i++) {
            unsigned char byteValue = fixup->b1[i];
            uncachedDst[i] = byteValue;
            cachedDst[i] = byteValue;
        }
    }
    libcdvd_finishNcmd((int *)&libcdvd_pendingCmd);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_createSemas);
#ifdef SKIP_ASM
struct QSema { int count; int max_count; int init_count; int wait_threads; int attr; int option; };
extern int Q0C00_create(struct QSema *) __asm__("CreateSema");
/**
 * Creates the libcdvd semaphores the first time they are needed.
 *
 * When any of the N-command, S-command or third semaphore (_sceCd_ncmd_semid, _sceCd_scmd_semid, libcdvd_diskReadySema) is still -1, it creates
 * all three with count 1 and the callback thread's libcdvd_callbackSema with count 0, and clears the busy flag libcdvd_busyFlag. Every
 * libcdvd entry point that takes a semaphore calls it first.
 */
void libcdvd_createSemas(void) {
    struct QSema semaParam;
    if (_sceCd_ncmd_semid == -1 || _sceCd_scmd_semid == -1 || libcdvd_diskReadySema == -1) {
        semaParam.option = 0;
        semaParam.init_count = 1;
        semaParam.max_count = 1;
        _sceCd_ncmd_semid = Q0C00_create(&semaParam);
        _sceCd_scmd_semid = Q0C00_create(&semaParam);
        libcdvd_diskReadySema = Q0C00_create(&semaParam);
        semaParam.init_count = 0;
        libcdvd_callbackSema = Q0C00_create(&semaParam);
        libcdvd_busyFlag = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_shutdown);
#ifdef SKIP_ASM
extern int cb_DB0(int) __asm__("DeleteSema");
extern void cb_DC0(int) __asm__("SignalSema");
extern void sceSifRemoveCmdHandler(int);
extern int cb_078() __asm__("DIntr");
extern void EIntr(void);
/**
 * Tears down libcdvd's threads, semaphores and power-off handler (the exit mode of sceCdInit).
 *
 * When the callback thread exists it sets the pending code libcdvd_pendingCmd to -1 and signals the thread so it exits itself
 * (libcdvd_callbackThread). Deletes the four semaphores and, with interrupts off, removes the SIF command handler 0x80000012
 * (sceSifRemoveCmdHandler) that libcdvd_installPowerOffHandler installed.
 */
void libcdvd_shutdown(void) {
    int intrEnabled;
    if (libcdvd_callbackThreadId != 0) {
        libcdvd_pendingCmd = -1;
        cb_DC0(libcdvd_callbackSema);
    }
    cb_DB0(_sceCd_ncmd_semid);
    cb_DB0(_sceCd_scmd_semid);
    cb_DB0(libcdvd_diskReadySema);
    cb_DB0(libcdvd_callbackSema);
    intrEnabled = cb_078();
    sceSifRemoveCmdHandler(0x80000012);
    if (intrEnabled)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdPOffCallback);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
extern int libcdvd_installPowerOffHandler(void);
/**
 * Installs the power-off callback libcdvd calls when the console's power button is pressed.
 *
 * Installs the SIF command handler first if needed (libcdvd_installPowerOffHandler, while libcdvd_powerOffHandlerState < 0), then with interrupts off
 * stores the function in libcdvd_powerOffCallback, its argument in libcdvd_powerOffCallbackArg and the caller's $gp in libcdvd_powerOffCallbackGp; the handler
 * libcdvd_onPowerOffCmd calls it. The game's bxIop_installPowerOffHandler installs bxIop_onPowerOff with the power-off thread's semaphore.
 *
 * @param callback  the callback
 * @param callbackArg  the argument it is called with
 * @return the previous callback
 */
void (*sceCdPOffCallback(void (*callback)(int), int callbackArg))(int) {
    int intrEnabled;
    void (*old)(int);
    if (libcdvd_powerOffHandlerState < 0)
        libcdvd_installPowerOffHandler();
    intrEnabled = DIntr();
    old = libcdvd_powerOffCallback;
    libcdvd_powerOffCallbackArg = callbackArg;
    libcdvd_powerOffCallback = callback;
    __asm__ volatile("sw $28,%0" : "=m"(libcdvd_powerOffCallbackGp));
    if (intrEnabled)
        EIntr();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_onPowerOffCmd);
#ifdef SKIP_ASM
/**
 * SIF command handler for the IOP's power-off notice (command 0x80000012): calls the power-off callback.
 *
 * Calls libcdvd_powerOffCallback(libcdvd_powerOffCallbackArg) under the $gp saved with it, unless no callback is set or the handler is being installed
 * (libcdvd_setupFlag). libcdvd_installPowerOffHandler installs it.
 */
void libcdvd_onPowerOffCmd(void) {
    void (*fn)(int) = libcdvd_powerOffCallback;
    if (fn) {
        if (libcdvd_setupFlag == 0) {
            int saved;
            __asm__ volatile("move %0,$28\n\tlw $28,%1" : "=r"(saved) : "m"(libcdvd_powerOffCallbackGp));
            fn(libcdvd_powerOffCallbackArg);
            __asm__ volatile("move $28,%0" : : "r"(saved));
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_installPowerOffHandler);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
extern void libcdvd_onPowerOffCmd(void);
extern void sceSifAddCmdHandler(int, void (*)(void), int);
/**
 * Installs libcdvd's SIF command handler libcdvd_onPowerOffCmd for the power-off notice 0x80000012.
 *
 * Sets the in-progress flag libcdvd_setupFlag around the install (sceSifAddCmdHandler, interrupts off) and marks the handler
 * installed (libcdvd_powerOffHandlerState = 1). Called by sceCdInit and sceCdPOffCallback.
 *
 * @return 1
 */
int libcdvd_installPowerOffHandler(void) {
    int intrEnabled;
    libcdvd_setupFlag = 1;
    intrEnabled = DIntr();
    sceSifAddCmdHandler(0x80000012, libcdvd_onPowerOffCmd, 0);
    if (intrEnabled)
        EIntr();
    libcdvd_setupFlag = 0;
    libcdvd_powerOffHandlerState = 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_searchFileRpc);
#ifdef SKIP_ASM
typedef struct { char b[0x24]; } R0EA8;
extern char D_00497A08[];
extern char D_00497A28[];
extern char D_00497A40[];
extern char D_00497A50[];
extern char D_00497A60[];
extern void libcdvd_createSemas();
extern int PollSema(int);
extern void SignalSema(int);
extern void ReferThreadStatus(int, sThreadParam *);
extern void scePrintf();
extern int sceCdSync(int);
extern void func_00426408(int);
extern void func_00426358(int, int);
int f0EA8(char *out, char *name, int arg) __asm__("libcdvd_searchFileRpc");
/**
 * Looks a file up on the disc through libcdvd's search-file RPC server and fills its 0x24-byte file record.
 *
 * Takes the N-command semaphore _sceCd_ncmd_semid without blocking (returns 0 when it is held or an N-command runs), binds
 * server 0x80000597 on first use (retrying, "Libcdvd bind err CdSearchFile"), copies @p name (up to 0xFF characters)
 * into the request D_00526540 with @p searchParam, writes it back from the cache and calls RPC 0. The record (sector, size,
 * name at +8; printed with the debug strings "search name/size/loc lbn") is copied from the request's uncached alias to
 * @p out. sceCdSearchFile calls it with 0.
 *
 * @param out  the 0x24-byte file record to fill
 * @param name  the file's path on the disc
 * @param searchParam  an extra word sent with the request
 * @return the server's result word (non-zero when found), 0 on failure
 */
int f0EA8(char *out, char *name, int searchParam) {
    int i;
    int n;
    int searchResult;
    libcdvd_createSemas();
    if (_sceCd_ncmd_semid != PollSema(_sceCd_ncmd_semid))
        return 0;
    libcdvd_ncmdKeepCmd = 1;
    ReferThreadStatus(libcdvd_initThreadId, &libcdvd_initThreadParam);
    if (sceCdSync(1) != 0) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    func_00426408(0);
    if (libcdvd_searchFileBindState < 0) {
        for (;;) {
            if (sceSifBindRpc(&D_005266C0, 0x80000597, 0) < 0) {
                if (libcdvd_debugLevel > 0)
                    scePrintf(D_00497A08);
                for (i = 0x100000; i--; )
                    ;
                continue;
            }
            if (D_005266C0.server != 0) {
                libcdvd_searchFileBindState = 0;
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
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497A28, D_00526564);
    D_00526540.arg = searchParam;
    D_00526540.self = &D_00526540;
    func_00426358((int)&D_00526540, 300);
    if (sceSifCallRpc(&D_005266C0, 0, 0, &D_00526540, 300, D_00526680, 4, 0, 0) < 0) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    *(R0EA8 *)out = *(R0EA8 *)((int)&D_00526540 | 0x20000000);
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497A40, out + 8);
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497A50, ((int *)out)[1]);
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497A60, ((int *)out)[0]);
    searchResult = *(int *)((int)D_00526680 | 0x20000000);
    SignalSema(_sceCd_ncmd_semid);
    return searchResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdSearchFile);
#ifdef SKIP_ASM
extern int libcdvd_searchFileRpc(int, int, int);
/**
 * Looks a file up on the disc: libcdvd_searchFileRpc with an extra request word of 0.
 *
 * @param fileRecord  the 0x24-byte file record to fill (sector, size, name)
 * @param path  the file's path on the disc
 * @return non-zero when the file was found, 0 otherwise
 */
int sceCdSearchFile(int fileRecord, int path) {
    return libcdvd_searchFileRpc(fileRecord, path, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _sceCd_ncmd_prechk);
#ifdef SKIP_ASM
extern char D_00497A78[];
extern char D_00497AA0[];
extern void libcdvd_createSemas();
extern int PollSema(int);
extern void SignalSema(int);
extern void ReferThreadStatus(int, sThreadParam *);
extern void scePrintf();
extern int sceCdSync(int);
extern void func_00426408(int);
/**
 * Prepares a libcdvd N-command: takes the N-command semaphore and binds the N-command RPC server.
 *
 * Fails (prints "Ncmd fail sema cur_cmd:%d keep_cmd:%d" in debug) when _sceCd_ncmd_semid is already held. Records the command
 * in libcdvd_ncmdKeepCmd, refreshes the caller's thread status, and fails (releasing the semaphore) while a previous N-command
 * is still running (sceCdSync(1)). Then flushes the cache (func_00426408(0)) and binds server 0x80000595 on first
 * use (libcdvd_ncmdBindState), retrying on error ("Libcdvd bind err N CMD"). On success the caller holds the semaphore and must
 * release it.
 *
 * @param cmdNumber  the command being issued (for the debug output)
 * @return 1 when the command may be issued, 0 otherwise
 */
int _sceCd_ncmd_prechk(int cmdNumber) {
    int i;
    libcdvd_createSemas();
    if (_sceCd_ncmd_semid != PollSema(_sceCd_ncmd_semid)) {
        if (libcdvd_debugLevel > 0)
            scePrintf(D_00497A78, cmdNumber, libcdvd_ncmdKeepCmd);
        return 0;
    }
    libcdvd_ncmdKeepCmd = cmdNumber;
    ReferThreadStatus(libcdvd_initThreadId, &libcdvd_initThreadParam);
    if (sceCdSync(1) != 0) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    func_00426408(0);
    if (libcdvd_ncmdBindState >= 0)
        return 1;
    for (;;) {
        if (sceSifBindRpc(&_sceCd_cd_ncmd, 0x80000595, 0) < 0) {
            if (libcdvd_debugLevel > 0)
                scePrintf(D_00497AA0);
            for (i = 0x100000; i--; )
                ;
            continue;
        }
        if (_sceCd_cd_ncmd.server != 0)
            break;
        for (i = 0x100000; i--; )
            ;
    }
    libcdvd_ncmdBindState = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdNcmdDiskReady);
#ifdef SKIP_ASM
extern int _sceCd_ncmd_prechk();
extern void SignalSema(int);
/**
 * Asks the CD N-command server for the drive's ready state (RPC function 14).
 *
 * Uses _sceCd_ncmd_prechk(2) to take the N-command channel, calls the function with a 4-byte reply in _sceCd_ncmdrdata and
 * releases the semaphore. sceCdRead refuses to read when it returns 6 (not ready).
 *
 * @return the ready state (6 = not ready), or 0 on failure
 */
int sceCdNcmdDiskReady(void) {
    int readyState;
    if (_sceCd_ncmd_prechk(2) == 0)
        return 0;
    if (sceSifCallRpc(&_sceCd_cd_ncmd, 14, 0, 0, 0, _sceCd_ncmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    readyState = *(int *)((unsigned)_sceCd_ncmdrdata | 0x20000000);
    SignalSema(_sceCd_ncmd_semid);
    return readyState;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdSync);
#ifdef SKIP_ASM
extern char str_N_cmd_wait[];
extern void scePrintf();
/**
 * Waits for or polls the end of the running libcdvd N-command.
 *
 * Mode 0 prints "N cmd wait" (debug) and sleeps 60 ticks at a time (libcdvd_sleepTicks) while the busy flag libcdvd_busyFlag is
 * set or the N-command RPC client _sceCd_cd_ncmd is still busy (sceSifCheckStatRpc), then returns 0. Any other mode returns
 * at once.
 *
 * @param mode  0 to block until the command ends, otherwise only poll
 * @return 1 while a command is still running, else 0
 */
int sceCdSync(int mode) {
    if (mode == 0) {
        if (libcdvd_debugLevel > 0)
            scePrintf(str_N_cmd_wait);
        while (libcdvd_busyFlag || sceSifCheckStatRpc(&_sceCd_cd_ncmd))
            libcdvd_sleepTicks(60);
        return 0;
    }
    if (libcdvd_busyFlag || sceSifCheckStatRpc(&_sceCd_cd_ncmd))
        return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdSyncS);
#ifdef SKIP_ASM
extern char str_S_cmd_wait[];
extern void scePrintf();
/**
 * Waits for or polls the end of the running libcdvd S-command.
 *
 * Mode 0 prints "S cmd wait" (debug) and sleeps 60 ticks at a time (libcdvd_sleepTicks) while the S-command RPC client
 * _sceCd_cd_scmd is busy (sceSifCheckStatRpc), then returns 0. Any other mode returns the busy state at once.
 *
 * @param mode  0 to block until the command ends, otherwise only poll
 * @return non-zero while an S-command is still running, else 0
 */
int sceCdSyncS(int mode) {
    if (mode == 0) {
        if (libcdvd_debugLevel > 0)
            scePrintf(str_S_cmd_wait);
        while (sceSifCheckStatRpc(&_sceCd_cd_scmd))
            libcdvd_sleepTicks(60);
        return 0;
    }
    return sceSifCheckStatRpc(&_sceCd_cd_scmd);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _sceCd_scmd_prechk);
#ifdef SKIP_ASM
extern char D_00497AD8[];
extern char D_00497B00[];
extern void libcdvd_createSemas();
extern int PollSema(int);
extern void SignalSema(int);
extern void ReferThreadStatus(int, sThreadParam *);
extern void scePrintf();
extern int sceCdSyncS(int);
extern void func_00426408(int);
/**
 * Prepares a libcdvd S-command: takes the S-command semaphore and binds the S-command RPC server.
 *
 * Fails (prints "Scmd fail sema cur_cmd:%d keep_cmd:%d" in debug) when _sceCd_scmd_semid is already held. Records the command
 * in D_00451618, refreshes the caller's thread status, and fails (releasing the semaphore) while an S-command still
 * runs (sceCdSyncS(1)). Then flushes the cache (func_00426408(0)) and binds server 0x80000593 on first use
 * (libcdvd_scmdBindState), retrying on error ("Libcdvd bind err S cmd"). On success the caller holds the semaphore and must
 * release it.
 *
 * @param cmdNumber  the command being issued (for the debug output)
 * @return 1 when the command may be issued, 0 otherwise
 */
int _sceCd_scmd_prechk(int cmdNumber) {
    int i;
    libcdvd_createSemas();
    if (_sceCd_scmd_semid != PollSema(_sceCd_scmd_semid)) {
        if (libcdvd_debugLevel > 0)
            scePrintf(D_00497AD8, cmdNumber, D_00451618);
        return 0;
    }
    D_00451618 = cmdNumber;
    ReferThreadStatus(libcdvd_initThreadId, &libcdvd_initThreadParam);
    if (sceCdSyncS(1) != 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    func_00426408(0);
    if (libcdvd_scmdBindState >= 0)
        return 1;
    for (;;) {
        if (sceSifBindRpc(&_sceCd_cd_scmd, 0x80000593, 0) < 0) {
            if (libcdvd_debugLevel > 0)
                scePrintf(D_00497B00);
            for (i = 0x100000; i--; )
                ;
            continue;
        }
        if (_sceCd_cd_scmd.server != 0)
            break;
        for (i = 0x100000; i--; )
            ;
    }
    libcdvd_scmdBindState = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_getInternalObject);
#ifdef SKIP_ASM
/**
 * Returns the address (or id) of one of libcdvd's internal objects, chosen by a code.
 *
 * 0x8001 gives the N-command semaphore id _sceCd_ncmd_semid, 0x8002 the N-command send buffer libcdvd_ncmdSendBuffer, 0x8003 its result
 * buffer _sceCd_ncmdrdata, 0x8004 the N-command RPC client _sceCd_cd_ncmd; 0x8011..0x8014 give the S-command equivalents
 * (_sceCd_scmd_semid, libcdvd_scmdSendBuffer, _sceCd_scmdrdata, _sceCd_cd_scmd). Other codes give 0. These are the objects the DNAS import table
 * DirtyDnas_importTable also exports.
 *
 * @param id  the object code
 * @param out  receives the id or address (0 for an unknown code)
 * @return 1
 */
int libcdvd_getInternalObject(long id, int *out) {
    int entry;
    if (id != 32769) {
        if (id != 32771) {
            if (id != 32770) {
                if (id != 32772) {
                    if (id != 32785) {
                        if (id != 32787) {
                            if (id != 32786) {
                                if (id == 32788)
                                    entry = (int)&_sceCd_cd_scmd;
                                else
                                    entry = 0;
                            } else {
                                entry = (int)libcdvd_scmdSendBuffer;
                            }
                        } else {
                            entry = (int)_sceCd_scmdrdata;
                        }
                    } else {
                        entry = _sceCd_scmd_semid;
                    }
                } else {
                    entry = (int)&_sceCd_cd_ncmd;
                }
            } else {
                entry = (int)libcdvd_ncmdSendBuffer;
            }
        } else {
            entry = (int)_sceCd_ncmdrdata;
        }
    } else {
        entry = _sceCd_ncmd_semid;
    }
    *out = entry;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdInit);
#ifdef SKIP_ASM
// Has the shape of libcdvd's sceCdInit(mode): binds RPC server 0x80000592, sends the mode, reads the status words
// back uncached; mode 5 (SCECdEXIT) tears the client down. Own match (agent fp401718, 2026-11-15); libcdvd_setupFlag is
// volatile at file scope (phaseb_import.py VIEW_SUBS).
extern int D_00451650;
extern int D_00451654;
extern int D_00526740;
extern sSifClient D_005266E8;
extern char D_00497B18[];
extern char D_00497B38[];
extern int sceCdSyncS(int);
extern void func_00426408(int);
extern int GetThreadId();
extern void scePrintf();
extern void func_00426358(int, int);

/**
 * Initialises (or, in mode 5, shuts down) the CD/DVD library and binds its init RPC server.
 *
 * Fails while an S-command runs (sceCdSyncS(1)). Records the current thread id, resets every RPC bind state
 * (libcdvd_searchFileBindState, libcdvd_ncmdBindState, libcdvd_scmdBindState, libcdvd_diskReadyBindState, D_00451650, libcdvd_powerOffHandlerState) to unbound, counts the call in D_00451654,
 * and binds server 0x80000592, retrying on error ("Libcdvd bind err %d CD_Init %d"). Sends @p mode (RPC 0, 16-byte
 * reply in _sceCd_scmdrdata): a reply status of 0xFE turns debug output on (libcdvd_debugLevel = 1); otherwise status words 1 or 2
 * below 0x200 make it return 2. Mode 5 prints "Libcdvd Exit", tears everything down (libcdvd_shutdown) and marks the
 * semaphores gone; other modes create the semaphores (libcdvd_createSemas) and install the power-off handler (libcdvd_installPowerOffHandler).
 * Called by systemInit's boot code (shutdownAndLoadExec) and the EA CD code.
 *
 * @param mode  0 to initialise, 5 to exit
 * @return 1 on success, 2 when the IOP side reports an old version, 0 on failure
 */
int sceCdInit(int mode)
{
    int i;
    int bindResult;
    int ret;
    int st, versionWord1, versionWord2;
    int *buf;

    if (sceCdSyncS(1) != 0)
        return 0;
    func_00426408(0);
    libcdvd_initThreadId = GetThreadId();
    libcdvd_setupFlag = 1;
    libcdvd_searchFileBindState = -1;
    libcdvd_ncmdBindState = -1;
    libcdvd_scmdBindState = -1;
    libcdvd_diskReadyBindState = -1;
    D_00451650 = -1;
    libcdvd_powerOffHandlerState = -1;
    libcdvd_readOptionFlags = 0;
    D_00451654++;
    while (1) {
        if ((bindResult = sceSifBindRpc(&D_005266E8, 0x80000592, 0)) < 0) {
            if (libcdvd_debugLevel > 0)
                scePrintf(D_00497B18, bindResult, D_00451654);
            for (i = 0x100000; i--; )
                ;
            continue;
        }
        if (D_005266E8.server != 0) {
            D_00526740 = mode;
            D_00451650 = 0;
            func_00426358((int)&D_00526740, 4);
            buf = _sceCd_scmdrdata;
            if (sceSifCallRpc(&D_005266E8, 0, 0, &D_00526740, 4, buf, 0x10, 0, 0) < 0) {
                libcdvd_setupFlag = 0;
                return 0;
            }
            break;
        }
        for (i = 0x100000; i--; )
            ;
    }
    ret = 1;
    versionWord1 = *(int *)((unsigned)(buf + 1) | 0x20000000);
    versionWord2 = *(int *)((unsigned)(buf + 2) | 0x20000000);
    st = *(int *)((unsigned)(buf + 3) | 0x20000000);
    if (st != 0xFF) {
        if (st == 0xFE)
            libcdvd_debugLevel = 1;
        else if (versionWord1 / 256 < 2 || versionWord2 / 256 < 2)
            ret = 2;
    }
    libcdvd_setupFlag = 0;
    switch (mode) {
    case 5:
        if (libcdvd_debugLevel > 0)
            scePrintf(D_00497B38);
        libcdvd_shutdown();
        _sceCd_ncmd_semid = -1;
        _sceCd_scmd_semid = -1;
        libcdvd_callbackSema = -1;
        break;
    case 0:
    case 1:
    default:
        libcdvd_createSemas();
        libcdvd_installPowerOffHandler();
        break;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libcdvd_oldDiskReady);
#ifdef SKIP_ASM
extern char D_00497B48[];
extern char str_Libcdvd_bind_err_CdDiskReady[];
extern char str_DiskReady_ended[];
extern void scePrintf();
extern void libcdvd_createSemas();
extern int PollSema(int);
extern void SignalSema(int);
extern int sceCdSyncS(int);
extern void func_00426408(int);
extern void func_00426358(int, int);
/**
 * Older form of the disc-ready check: asks the disk-ready RPC server (0x8000059A) whether a disc is ready.
 *
 * Prints "OLD DiskReady Call" (debug), takes the S-command semaphore _sceCd_scmd_semid without blocking (6 when held), fails
 * while an S-command runs, binds server 0x8000059A on first use ("Libcdvd bind err CdDiskReady"), sends @p mode (RPC 0)
 * and returns the reply word ("DiskReady ended"). sceCdDiskReady falls back to it when the newer server 0x8000059C does
 * not answer.
 *
 * @param mode  the mode passed to the server (8 makes failures return -1 instead of 6)
 * @return the ready state from the IOP; 6 (not ready) on failure, or -1 in mode 8
 */
int libcdvd_oldDiskReady(int mode) {
    int spinCount;
    int readyState;
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497B48);
    libcdvd_createSemas();
    if (_sceCd_scmd_semid != PollSema(_sceCd_scmd_semid))
        return 6;
    if (sceCdSyncS(1) != 0) {
        SignalSema(_sceCd_scmd_semid);
        return mode != 8 ? 6 : -1;
    }
    func_00426408(0);
    if (libcdvd_diskReadyBindState < 0) {
        for (;;) {
            if (sceSifBindRpc(&libcdvd_diskReadyClient, 0x8000059A, 0) < 0) {
                if (libcdvd_debugLevel > 0)
                    scePrintf(str_Libcdvd_bind_err_CdDiskReady);
                for (spinCount = 0x100000; spinCount--; )
                    ;
                continue;
            }
            if (libcdvd_diskReadyClient.server != 0) {
                libcdvd_diskReadyBindState = 0;
                break;
            }
            for (spinCount = 0x100000; spinCount--; )
                ;
        }
    }
    libcdvd_diskReadySendBuffer[0] = mode;
    func_00426358((int)libcdvd_diskReadySendBuffer, 4);
    if (sceSifCallRpc(&libcdvd_diskReadyClient, 0, 0, libcdvd_diskReadySendBuffer, 4, _sceCd_scmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return mode != 8 ? 6 : -1;
    }
    if (libcdvd_debugLevel > 0)
        scePrintf(str_DiskReady_ended);
    readyState = *(int *)((unsigned)_sceCd_scmdrdata | 0x20000000);
    SignalSema(_sceCd_scmd_semid);
    return readyState;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdDiskReady);
#ifdef SKIP_ASM
extern char D_00497B98[];
extern char str_Libcdvd_bind_err_CdDiskReady[];
extern char str_DiskReady_ended[];
extern void b1_log() __asm__("scePrintf");
extern void b1_0C00() __asm__("libcdvd_createSemas");
extern int b1_3DF0(int) __asm__("PollSema");
extern void b1_3DC0(int) __asm__("SignalSema");
extern void b1_6408(int) __asm__("func_00426408");
extern void b1_6358(int *, int) __asm__("func_00426358");
extern int b1_19F8(int) __asm__("libcdvd_oldDiskReady");
/* The sema id passed to the Sema calls is read volatile throughout libcdvd (see the retail delay slots). */
static inline int semaId1BF0(void) {
    return libcdvd_diskReadySema;
}
/**
 * Asks the IOP whether a disc is ready to read (the libcdvd disk-ready call).
 *
 * Prints "NEW DiskReady Call" (debug), takes the third semaphore libcdvd_diskReadySema without blocking and binds server
 * 0x8000059C on first use; when that server has not appeared after 17 tries it releases the semaphore and uses the
 * older server through libcdvd_oldDiskReady. Sends @p mode (RPC 0), returns the reply word from D_00453080 ("DiskReady ended")
 * and releases the semaphore.
 *
 * @param mode  the mode passed to the server (8 makes failures return -1 instead of 6)
 * @return the ready state (6 = not ready); 6 on failure, or -1 in mode 8
 */
int sceCdDiskReady(int mode) {
    int i;
    int spinCount;
    int readyState;
    if (libcdvd_debugLevel > 0)
        b1_log(D_00497B98);
    b1_0C00();
    if (libcdvd_diskReadySema != b1_3DF0(semaId1BF0()))
        return mode != 8 ? 6 : -1;
    b1_6408(0);
    i = 0;
    if (libcdvd_diskReadyBindState < 0) {
        for (;;) {
            if (sceSifBindRpc(&libcdvd_diskReadyClient, 0x8000059C, 0) < 0) {
                if (libcdvd_debugLevel > 0)
                    b1_log(str_Libcdvd_bind_err_CdDiskReady);
                for (spinCount = 0x100000; spinCount--; )
                    ;
                continue;
            }
            if (libcdvd_diskReadyClient.server != 0) {
                libcdvd_diskReadyBindState = 0;
                break;
            }
            if (i >= 17) {
                b1_3DC0(semaId1BF0());
                return b1_19F8(mode);
            }
            for (spinCount = 0x100000; spinCount--; )
                ;
            i++;
        }
    }
    libcdvd_diskReadySendBuffer[0] = mode;
    b1_6358(libcdvd_diskReadySendBuffer, 4);
    if (sceSifCallRpc(&libcdvd_diskReadyClient, 0, 0, libcdvd_diskReadySendBuffer, 4, D_00453080, 4, 0, 0) < 0) {
        b1_3DC0(semaId1BF0());
        return mode != 8 ? 6 : -1;
    }
    if (libcdvd_debugLevel > 0)
        b1_log(str_DiskReady_ended);
    readyState = *(int *)((unsigned)D_00453080 | 0x20000000);
    b1_3DC0(semaId1BF0());
    return readyState;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdRead);
#ifdef SKIP_ASM
extern char D_00497BB0[];
extern char D_00497BC8[];
extern int sceCdNcmdDiskReady(void);
extern int _sceCd_ncmd_prechk();
extern void scePrintf();
extern void SignalSema(int);
extern void func_00426358(int, int);
/**
 * Starts an asynchronous CD/DVD sector read into memory (libcdvd N-command 1).
 *
 * Unless reads skip the check (bit 0 of libcdvd_readOptionFlags) it refuses when the drive reports not ready (sceCdNcmdDiskReady() == 6).
 * Takes the N-command channel (_sceCd_ncmd_prechk(4)), fills the read command libcdvd_ncmdSendBuffer (sector, count, buffer, the try
 * count, spindle control and data pattern from @p mode, the fixup buffer D_00452700 and the result word D_004527C0),
 * writes the destination back from the cache (sectors of 2048, 2328 or 2340 bytes for data pattern 0, 1 or 2; skipped
 * when bit 1 of libcdvd_readOptionFlags is set) and sends RPC 1 without waiting, with libcdvd_onReadDone as the completion handler. Sets
 * the pending code libcdvd_pendingCmd to 1 and the busy flag; the N-command semaphore stays held until the read ends. Prints
 * "call cdread cmd" and "cdread end" in debug.
 *
 * @param lsn  the first logical sector
 * @param sectors  the number of sectors
 * @param buf  the destination address
 * @param mode  the read mode: try count, spindle control, data pattern
 * @return 1 when the read was started, 0 otherwise
 */
int sceCdRead(int lsn, int sectors, int buf, unsigned char *mode) {
    int size;
    sCdReadCmd *readCmd = (sCdReadCmd *)libcdvd_ncmdSendBuffer;
    if ((libcdvd_readOptionFlags & 1) == 0) {
        if (sceCdNcmdDiskReady() == 6)
            return 0;
    }
    if (_sceCd_ncmd_prechk(4) == 0)
        return 0;
    readCmd->lsn = lsn;
    readCmd->sectors = sectors;
    readCmd->buf = buf;
    readCmd->trycount = mode[0];
    readCmd->spindlctrl = mode[1];
    readCmd->datapattern = mode[2];
    readCmd->ebuf = D_00452700;
    readCmd->res = D_004527C0;
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
    if ((libcdvd_readOptionFlags & 2) == 0)
        func_00426358(buf, size);
    func_00426358((int)D_00452700, 144);
    func_00426358((int)readCmd, 0x18);
    func_00426358((int)D_004527C0, 4);
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497BB0);
    libcdvd_pendingCmd = 1;
    libcdvd_busyFlag = 1;
    if (sceSifCallRpc(&_sceCd_cd_ncmd, 1, 1, readCmd, 0x18, 0, 0, libcdvd_onReadDone, D_00452700) < 0) {
        libcdvd_pendingCmd = 0;
        libcdvd_busyFlag = 0;
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497BC8);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdGetToc);
#ifdef SKIP_ASM
extern int _sceCd_ncmd_prechk();
extern void func_00426358(int, int);
extern void SignalSema(int);
/**
 * Reads the disc's table of contents (libcdvd N-command 4) into @p dst.
 *
 * Takes the N-command channel (_sceCd_ncmd_prechk(8)), sends the address of the work buffer D_00526780 (written back from
 * the cache first) and waits for the 8-byte reply in _sceCd_ncmdrdata. The pending code libcdvd_pendingCmd is 3 during the call.
 * Copies 0x400 bytes (CD) or 2064 bytes (when reply word 1 is non-zero, a DVD) from the buffer's uncached alias to @p
 * dst. The EA disc check CDVD_initFileSystem calls it after checking for a PS2 CD or DVD.
 *
 * @param dst  the destination (2064 bytes for a DVD)
 * @return the reply's result word, or 0 on failure
 */
int sceCdGetToc(char *dst) {
    int tocResult;
    int *sendBuf = libcdvd_ncmdSendBuffer;
    if (_sceCd_ncmd_prechk(8) == 0)
        return 0;
    sendBuf[0] = (int)D_00526780;
    func_00426358((int)D_00526780, 2064);
    func_00426358((int)sendBuf, 12);
    libcdvd_pendingCmd = 3;
    if (sceSifCallRpc(&_sceCd_cd_ncmd, 4, 0, sendBuf, 12, _sceCd_ncmdrdata, 8, 0, 0) < 0) {
        libcdvd_pendingCmd = 0;
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    libcdvd_pendingCmd = 0;
    tocResult = _sceCd_ncmdrdata[0];
    if (_sceCd_ncmdrdata[1] == 0)
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 0x400);
    else
        __builtin_memcpy(dst, (char *)((unsigned)D_00526780 | 0x20000000), 2064);
    SignalSema(_sceCd_ncmd_semid);
    return tocResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdGetDiskType);
#ifdef SKIP_ASM
extern int _sceCd_scmd_prechk();
extern void SignalSema(int);
/**
 * Asks the drive what kind of disc is loaded (libcdvd S-command, RPC function 3).
 *
 * Takes the S-command channel (_sceCd_scmd_prechk(1)), calls the function with a 4-byte reply in _sceCd_scmdrdata and releases the
 * semaphore. The EA disc check CDVD_initFileSystem compares the result with 18 and 20 (PS2 CD and PS2 DVD).
 *
 * @return the disc type, or 0 on failure
 */
int sceCdGetDiskType(void) {
    int diskType;
    if (_sceCd_scmd_prechk(1) == 0)
        return 0;
    if (sceSifCallRpc(&_sceCd_cd_scmd, 3, 0, 0, 0, _sceCd_scmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    diskType = *(int *)((unsigned)_sceCd_scmdrdata | 0x20000000);
    SignalSema(_sceCd_scmd_semid);
    return diskType;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdGetError);
#ifdef SKIP_ASM
extern int _sceCd_scmd_prechk();
extern void SignalSema(int);
/**
 * Returns the drive's error code for the last command (libcdvd S-command, RPC function 4).
 *
 * Takes the S-command channel (_sceCd_scmd_prechk(3)), calls the function with a 4-byte reply in _sceCd_scmdrdata and releases the
 * semaphore. The EA CD driver retries a read until it returns 0.
 *
 * @return the error code (0 = no error), or -1 on failure
 */
int sceCdGetError(void) {
    int errorCode;
    if (_sceCd_scmd_prechk(3) == 0)
        return -1;
    if (sceSifCallRpc(&_sceCd_cd_scmd, 4, 0, 0, 0, _sceCd_scmdrdata, 4, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return -1;
    }
    errorCode = *(int *)((unsigned)_sceCd_scmdrdata | 0x20000000);
    SignalSema(_sceCd_scmd_semid);
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdPowerOff);
#ifdef SKIP_ASM
extern int _sceCd_scmd_prechk();
extern void SignalSema(int);
/**
 * Sends libcdvd S-command 33 and returns its status word (the console power-off request).
 *
 * Takes the S-command channel (_sceCd_scmd_prechk(33)), calls RPC function 33 with an 8-byte reply in _sceCd_scmdrdata, stores
 * reply word 1 in @p out and releases the semaphore. The power-off thread bxIop_powerOffThread repeats it until it returns
 * non-zero with a status of 0, after shutting dev9 down.
 *
 * @param out  receives the status word
 * @return the result word (non-zero on success), 0 on failure
 */
int sceCdPowerOff(int *out) {
    int rpcResult;
    if (_sceCd_scmd_prechk(33) == 0)
        return 0;
    if (sceSifCallRpc(&_sceCd_cd_scmd, 33, 0, 0, 0, _sceCd_scmdrdata, 8, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    *out = *(int *)((unsigned)(_sceCd_scmdrdata + 1) | 0x20000000);
    rpcResult = *(int *)((unsigned)_sceCd_scmdrdata | 0x20000000);
    SignalSema(_sceCd_scmd_semid);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdMmode);
#ifdef SKIP_ASM
extern int _sceCd_scmd_prechk();
extern void SignalSema(int);
extern void func_00426358_p(int *, int) __asm__("func_00426358");
/**
 * Sets the drive's media mode (CD or DVD) with a libcdvd S-command.
 *
 * Takes the S-command channel (_sceCd_scmd_prechk(34)), sends @p media with the function number and send/reply sizes held in
 * D_004531A8, D_004531AC and D_004531B0, and returns the reply word from _sceCd_scmdrdata. The boot code shutdownAndLoadExec and
 * bxIop_initIop call it with 2; the EA CD code passes its configured media mode.
 *
 * @param media  the media mode (2 = DVD)
 * @return the result word, or 0 on failure
 */
int sceCdMmode(int media) {
    int result;
    int *sendBuf = libcdvd_scmdSendBuffer;
    if (_sceCd_scmd_prechk(34) == 0)
        return 0;
    libcdvd_scmdSendBuffer[0] = media;
    func_00426358_p(sendBuf, 4);
    if (sceSifCallRpc(&_sceCd_cd_scmd, D_004531A8, 0, sendBuf, D_004531AC, _sceCd_scmdrdata, D_004531B0, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    result = *(int *)((unsigned)_sceCd_scmdrdata | 0x20000000);
    SignalSema(_sceCd_scmd_semid);
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdReadClock);
#ifdef SKIP_ASM
struct P8 { char b[8]; } __attribute__((packed));
extern char D_00497BD8[];
extern char D_00497BF8[];
extern int _sceCd_scmd_prechk();
extern void SignalSema(int);
extern void scePrintf();
/**
 * Reads the console's real-time clock (libcdvd S-command, RPC function 1) into an 8-byte record.
 *
 * Takes the S-command channel (_sceCd_scmd_prechk(15)), prints "Libcdvd call Clock read 1" and "... 2" around the call
 * (debug), copies the 8 bytes after the result word of the 16-byte reply _sceCd_scmdrdata to @p clockOut and releases the semaphore.
 * systemInit and cPS2ConsoleConfig_cPS2ConsoleConfig call it.
 *
 * @param clockOut  receives the 8-byte clock record
 * @return the result word (non-zero on success), 0 on failure
 */
int sceCdReadClock(struct P8 *clockOut) {
    int result;
    if (_sceCd_scmd_prechk(15) == 0)
        return 0;
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497BD8);
    if (sceSifCallRpc(&_sceCd_cd_scmd, 1, 0, 0, 0, _sceCd_scmdrdata, 16, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    *clockOut = *(struct P8 *)((unsigned)&_sceCd_scmdrdata[1] | 0x20000000);
    if (libcdvd_debugLevel > 0)
        scePrintf(D_00497BF8);
    result = *(int *)((unsigned)_sceCd_scmdrdata | 0x20000000);
    SignalSema(_sceCd_scmd_semid);
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceCdTrayReq);
#ifdef SKIP_ASM
extern int _sceCd_scmd_prechk();
extern void SignalSema(int);
extern void func_00426358_p(int *, int) __asm__("func_00426358");
/**
 * Sends a disc-tray request with a libcdvd S-command (RPC function 5).
 *
 * Takes the S-command channel (_sceCd_scmd_prechk(26)), sends @p request, stores reply word 1 in @p out when given and returns the
 * reply's result word. cd_recoverReadError calls it with 2 to check whether the tray has been opened.
 *
 * @param request  the request (2 = check the tray state)
 * @param out  receives the tray status, may be 0
 * @return the result word, or -1 on failure
 */
int sceCdTrayReq(int request, int *out) {
    int result;
    int *sendBuf = libcdvd_scmdSendBuffer;
    if (_sceCd_scmd_prechk(26) == 0)
        return -1;
    libcdvd_scmdSendBuffer[0] = request;
    func_00426358_p(sendBuf, 4);
    if (sceSifCallRpc(&_sceCd_cd_scmd, 5, 0, sendBuf, 4, _sceCd_scmdrdata, 8, 0, 0) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return -1;
    }
    if (out)
        *out = *(int *)((unsigned)&_sceCd_scmdrdata[1] | 0x20000000);
    result = *(int *)((unsigned)_sceCd_scmdrdata | 0x20000000);
    SignalSema(_sceCd_scmd_semid);
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegInit);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
extern void sceIpuInit(void);
/**
 * Stops the IPU's DMA channels and resets the IPU before MPEG decoding starts.
 *
 * With interrupts off it holds the DMA controller (D_ENABLEW = D_ENABLER | 0x10000), clears the start bit of the
 * IPU-from and IPU-to channels (0x1000B000 and 0x1000B400), and releases it; then zeroes both channels' QWC and resets
 * and re-initialises the IPU (sceIpuInit). PS2_SONY_CODEC_INTERNAL_Init calls it right before setting the decoder up
 * with sceMpegCreate.
 *
 * @return 1
 */
int sceMpegInit(void) {
    int intrState = DIntr();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 &= 0xFFFFFEFF;
    *(volatile int *)0x1000B400 &= 0xFFFFFEFF;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        EIntr();
    *(volatile int *)0x1000B020 = 0;
    *(volatile int *)0x1000B420 = 0;
    sceIpuInit();
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegCreate);
#ifdef SKIP_ASM
extern char str_The_size_of_work_area_is_too_small[];
extern int i7B8_memset(int, int, int) __asm__("memset");
extern void i7B8_34A8(int *, char *) __asm__("libmpeg_reportError");
extern void i7B8_2D50(int *, int, int) __asm__("libmpeg_initPool");
extern unsigned i7B8_2D88(int *, int *, unsigned, unsigned) __asm__("libmpeg_allocFromPool");
extern void i7B8_3338(int *) __asm__("libmpeg_setupScratchpad");
extern int i7B8_2B58(int *) __asm__("sceMpegReset");
extern int i7B8_2BB0(int *) __asm__("sceMpegClearRefBuff");
extern void i7B8_2D68(int *) __asm__("libmpeg_markPool");
extern int i7B8_3DB0(int *) __asm__("libmpeg_defaultStopDma");
extern int i7B8_3DD8(int *) __asm__("libmpeg_defaultRestartDma");
/**
 * Sets up an MPEG decoder in a caller-supplied work area.
 *
 * Clears the work area, aligns its start to 4 bytes as the decoder's internal state and fails with "The size of work
 * area is too small" (libmpeg_reportError) when less than 4288 bytes remain. The memory after the first 4288 bytes becomes
 * the decoder's pool (libmpeg_initPool at state + 264). Resets the public fields of @p mpeg (width, height and frame count
 * words to 0, the 64-bit time stamps to -1) and links the state at a[16] and back at +0x858; installs the default
 * callbacks libmpeg_defaultStopDma and libmpeg_defaultRestartDma, allocates a 0x600-byte block (libmpeg_allocFromPool), resets the stream state
 * (libmpeg_setupScratchpad, sceMpegReset, sceMpegClearRefBuff), points the nine per-picture tables at the state's own arrays and sets
 * the scratchpad address 0x70003600. Called by PS2_SONY_CODEC_INTERNAL_Init.
 *
 * @param mpeg  the decoder handle
 * @param buf  the work area
 * @param size  the work area size in bytes
 * @return 1, or 0 when the work area is too small
 */
// sceMpegCreate(mp, buff, size)
int sceMpegCreate(int *mpeg, int buf, int size) {
    int *decState;
    int *pool;
    int unsetValue;
    int rem;
    i7B8_memset(buf, 0, size);
    decState = (int *)(((unsigned)(buf + 3) >> 2) << 2);
    rem = size - ((int)decState - buf);
    if ((unsigned)rem < 4288) {
        i7B8_34A8(decState, str_The_size_of_work_area_is_too_small);
        return 0;
    }
    pool = decState + 264 / 4;
    mpeg[16] = (int)decState;
    i7B8_2D50(pool, (int)decState + 4288, rem - 4288);
    unsetValue = -1;
    mpeg[0] = 0;
    mpeg[1] = 0;
    mpeg[2] = 0;
    *(long *)&mpeg[4] = -1;
    *(long *)&mpeg[6] = -1;
    *(long *)&mpeg[8] = 0;
    *(long *)&mpeg[10] = -1;
    *(long *)&mpeg[12] = -1;
    *(long *)&mpeg[14] = 0;
    decState[45] = 0;
    decState[46] = 0;
    decState[47] = 0;
    decState[48] = 0;
    decState[49] = 0;
    decState[50] = 0;
    decState[51] = 0;
    decState[52] = 0;
    decState[53] = 0;
    decState[54] = 0;
    decState[55] = 0;
    decState[56] = 0;
    decState[57] = 0;
    decState[58] = 0;
    decState[62] = 0;
    decState[3] = 0;
    decState[5] = 0;
    decState[11] = 0;
    decState[13] = 0;
    decState[15] = 0;
    *(long *)&decState[60] = -1;
    decState[7] = (int)i7B8_3DB0;
    decState[9] = (int)i7B8_3DD8;
    decState[17] = i7B8_2D88(decState, pool, 0x600, 8);
    decState[18] = 0;
    decState[63] = 0;
    decState[64] = 0;
    decState[65] = 0;
    decState[28] = 0;
    *(long *)&decState[30] = 0;
    decState[32] = unsetValue;
    *(long *)&decState[34] = 0;
    decState[36] = 0;
    decState[43] = 0;
    decState[37] = unsetValue;
    decState[38] = unsetValue;
    decState[39] = unsetValue;
    decState[534] = (int)mpeg;
    decState[44] = 1;
    i7B8_3338(decState);
    i7B8_2B58(mpeg);
    i7B8_2BB0(mpeg);
    decState[110] = (int)decState + 488;
    decState[111] = (int)decState + 592;
    decState[113] = (int)decState + 696;
    decState[114] = (int)decState + 800;
    decState[115] = (int)decState + 904;
    decState[117] = (int)decState + 0x3F0;
    decState[118] = (int)decState + 1112;
    decState[119] = (int)decState + 1216;
    decState[121] = (int)decState + 1320;
    i7B8_2D68(pool);
    decState[532] = unsetValue;
    decState[533] = 0;
    decState[531] = 0;
    decState[519] = 0x70003600;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegDelete);
#ifdef SKIP_ASM
/**
 * Returns 1; an empty libmpeg entry point.
 *
 * @return 1
 */
int sceMpegDelete(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegAddBs);
#ifdef SKIP_ASM
extern int libmpeg_startInputDma(int, int, int);
/**
 * Hands a block of MPEG bitstream to the decoder: starts the IPU input DMA over it.
 *
 * Passes the decoder's internal state (mpeg[16]), @p dataAddr and the size plus 4 rounded up to 16 bytes to
 * libmpeg_startInputDma, which builds the DMA tag chain and starts channel 4 (to IPU). The EA codec PS2_SONY_CODEC_INTERNAL_feedChunk calls it with
 * each chunk it has read.
 *
 * @param mpeg  the decoder handle
 * @param dataAddr  the bitstream data (16-byte aligned)
 * @param n  its size in bytes
 * @return 1
 */
int sceMpegAddBs(int *mpeg, int dataAddr, int n) {
    libmpeg_startInputDma(mpeg[16], dataAddr, (n + 19) / 16 * 16);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegGetPicture);
#ifdef SKIP_ASM
extern int libmpeg_decodeUntilPicture(int *);
/**
 * Decodes the next picture into an RGB32 output buffer (with IPU colour conversion).
 *
 * Sets the conversion flag (+0xB0 = 1), the output address (its uncached alias, +0xD8), the macroblock count (+0xE4)
 * and clears the RAW8 width/height words, then runs the decode loop libmpeg_decodeUntilPicture. PS2_SONY_CODEC_INTERNAL_DecodeFrame
 * calls it with (width / 16) * (height / 16) macroblocks.
 *
 * @param mpeg  the decoder handle
 * @param picture  the output buffer (64-byte aligned)
 * @param mbCount  the number of macroblocks in the picture
 * @return libmpeg_decodeUntilPicture's result: 1 when a picture (or the stream end) was reached, -1 for a misaligned buffer
 */
int sceMpegGetPicture(int *mpeg, unsigned picture, int mbCount) {
    int *mpegSys = (int *)mpeg[16];
    picture = (picture & 0xFFFFFFF) | 0x20000000;
    mpegSys[44] = 1;
    mpegSys[54] = picture;
    mpegSys[57] = mbCount;
    mpegSys[56] = 0;
    mpegSys[55] = 0;
    return libmpeg_decodeUntilPicture(mpeg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegGetPictureRAW8);
#ifdef SKIP_ASM
extern int libmpeg_decodeUntilPicture(int *);
/**
 * Decodes the next picture into a RAW8 (unconverted YCbCr) output buffer.
 *
 * Like sceMpegGetPicture but with the conversion flag (+0xB0) cleared: sets the output address (uncached, +0xD8) and
 * macroblock count (+0xE4), clears the width/height words and runs libmpeg_decodeUntilPicture.
 *
 * @param mpeg  the decoder handle
 * @param picture  the output buffer (64-byte aligned)
 * @param mbCount  the number of macroblocks in the picture
 * @return libmpeg_decodeUntilPicture's result
 */
int sceMpegGetPictureRAW8(int *mpeg, unsigned picture, int mbCount) {
    int *mpegSys = (int *)mpeg[16];
    picture = (picture & 0xFFFFFFF) | 0x20000000;
    mpegSys[57] = mbCount;
    mpegSys[54] = picture;
    mpegSys[44] = 0;
    mpegSys[56] = 0;
    mpegSys[55] = 0;
    return libmpeg_decodeUntilPicture(mpeg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegGetPictureRAW8xy);
#ifdef SKIP_ASM
extern int libmpeg_decodeUntilPicture(int *);
/**
 * Decodes the next picture into a RAW8 buffer laid out by macroblock columns and rows.
 *
 * Like sceMpegGetPictureRAW8, but stores the picture width and height in pixels (mbWidth * 16 at +0xDC, mbHeight * 16 at +0xE0)
 * and the macroblock count mbWidth * mbHeight, then runs libmpeg_decodeUntilPicture.
 *
 * @param mpeg  the decoder handle
 * @param picture  the output buffer (64-byte aligned)
 * @param mbWidth  the picture width in macroblocks
 * @param mbHeight  the picture height in macroblocks
 * @return libmpeg_decodeUntilPicture's result
 */
int sceMpegGetPictureRAW8xy(int *mpeg, unsigned picture, int mbWidth, int mbHeight) {
    int *mpegSys = (int *)mpeg[16];
    picture = (picture & 0xFFFFFFF) | 0x20000000;
    mpegSys[56] = mbHeight * 16;
    mpegSys[54] = picture;
    mpegSys[57] = mbWidth * mbHeight;
    mpegSys[55] = mbWidth * 16;
    mpegSys[44] = 0;
    return libmpeg_decodeUntilPicture(mpeg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402AF0);
#ifdef SKIP_ASM
/**
 * Sets the decoder flag at internal +0x10A0 to 1.
 *
 * The flag is tested (and cleared) by the slice decoder libmpeg_waitMacroblock.
 *
 * @param mpeg  the decoder handle
 */
void func_00402AF0(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[1064] = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegSetDecodeMode);
#ifdef SKIP_ASM
/**
 * Sets how many I, P and B pictures the decoder decodes (-1 = all; others are skipped).
 *
 * Stores the three limits at internal +0x94, +0x98 and +0x9C; sceMpeg setup (sceMpegCreate) initialises them to -1. The
 * decode loop libmpeg_decodeUntilPicture passes each picture type's counter and limit to libmpeg_decodePicture, which skips pictures past
 * the limit.
 *
 * @param mpeg  the decoder handle
 * @param maxIPictures  the I-picture limit
 * @param maxPPictures  the P-picture limit
 * @param maxBPictures  the B-picture limit
 */
void sceMpegSetDecodeMode(int *mpeg, int maxIPictures, int maxPPictures, int maxBPictures) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[37] = maxIPictures;
    mpegSys[38] = maxPPictures;
    mpegSys[39] = maxBPictures;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegGetDecodeMode);
#ifdef SKIP_ASM
/**
 * Reads back the I, P and B picture decode limits (see sceMpegSetDecodeMode).
 *
 * @param mpeg  the decoder handle
 * @param maxIOut  receives the I-picture limit
 * @param maxPOut  receives the P-picture limit
 * @param maxBOut  receives the B-picture limit
 */
void sceMpegGetDecodeMode(int *mpeg, int *maxIOut, int *maxPOut, int *maxBOut) {
    int *mpegSys = (int *)mpeg[16];
    *maxIOut = mpegSys[37];
    *maxPOut = mpegSys[38];
    *maxBOut = mpegSys[39];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegIsEnd);
#ifdef SKIP_ASM
/**
 * Tells whether the decoder has reached the end of the stream.
 *
 * Returns internal word 0, which the decode loop libmpeg_decodeUntilPicture sets when no further picture is found.
 * PS2_SONY_CODEC_INTERNAL_GetFrame checks it.
 *
 * @param mpeg  the decoder handle
 * @return non-zero at the end of the stream
 */
int sceMpegIsEnd(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegIsRefBuffEmpty);
#ifdef SKIP_ASM
/**
 * Tells whether the decoder holds no reference picture still waiting to be output.
 *
 * Returns 1 when internal word 1 is 0; libmpeg_flushHeldPicture outputs the held reference picture and clears that word at the end
 * of the stream.
 *
 * @param mpeg  the decoder handle
 * @return 1 when no reference picture is pending, else 0
 */
int sceMpegIsRefBuffEmpty(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    return mpegSys[1] == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegReset);
#ifdef SKIP_ASM
extern void libmpeg_stopIpu(int *);
/**
 * Resets the decoder's stream state, the IPU and its DMA channels.
 *
 * Clears the end flag and the reference flags (internal words 0..2), the handle's frame count (mpeg[2]) and the
 * counters at +0xAC/+0x118, sets +0x80 to -1, stops and resets the IPU and its DMA (libmpeg_stopIpu) and clears the
 * pending slice state (libmpeg_resetToMpeg1). sceMpeg setup sceMpegCreate calls it.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
int sceMpegReset(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[0] = 0;
    mpegSys[1] = 0;
    mpegSys[2] = 0;
    mpeg[2] = 0;
    mpegSys[43] = 0;
    mpegSys[32] = -1;
    libmpeg_stopIpu(mpegSys);
    mpegSys[70] = 0;
    libmpeg_resetToMpeg1(mpegSys);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegClearRefBuff);
#ifdef SKIP_ASM
/**
 * Marks the decoder's six reference picture buffers as empty.
 *
 * Clears word 10 of each per-picture record the internal state points to at +0x1B8, +0x1C8, +0x1D8, +0x1BC, +0x1CC and
 * +0x1DC (the records sceMpeg setup sceMpegCreate places in the state), skipping null pointers. Called by
 * sceMpegCreate.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
int sceMpegClearRefBuff(int *mpeg) {
    char *mpegSys = (char *)mpeg[16];
    int *picRecord;
    if ((picRecord = *(int **)(mpegSys + 0x1B8))) picRecord[10] = 0;
    if ((picRecord = *(int **)(mpegSys + 0x1C8))) picRecord[10] = 0;
    if ((picRecord = *(int **)(mpegSys + 0x1D8))) picRecord[10] = 0;
    if ((picRecord = *(int **)(mpegSys + 0x1BC))) picRecord[10] = 0;
    if ((picRecord = *(int **)(mpegSys + 0x1CC))) picRecord[10] = 0;
    if ((picRecord = *(int **)(mpegSys + 0x1DC))) picRecord[10] = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMpegAddCallback);
#ifdef SKIP_ASM
/**
 * Installs a decoder callback of one type and returns the one it replaces.
 *
 * The callbacks live in the internal state at +0x0C as (function, data) pairs indexed by type: 0 error (see
 * libmpeg_reportError), 1 needs data (libmpeg_raiseNeedData), 2 / 3 around the decoder's own use of the IPU DMA (libmpeg_loadDefaultMatrix).
 * PS2_SONY_CODEC_INTERNAL_Init installs RCMP_mpegNoDataCallback for type 1.
 *
 * @param mpeg  the decoder handle
 * @param n  the callback type
 * @param fn  the callback
 * @param cbData  the data word passed to it
 * @return the previous callback of that type
 */
int sceMpegAddCallback(int *mpeg, int n, int fn, void *cbData) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_dispatchCallback);
#ifdef SKIP_ASM
struct FA_402C30 { int (*fn)(int *, int *, int); int arg; };
struct T_402C30 { char pad[12]; struct FA_402C30 e[1]; };
/**
 * Calls the decoder callback whose type is the first word of @p cbData.
 *
 * Looks the (function, data) pair up in the internal state's table at +0x0C and calls function(mpeg, cbData, data) when
 * one is installed (see sceMpegAddCallback). Every libmpeg routine that reports an event goes through it.
 *
 * @param mpeg  the decoder handle (may be 0)
 * @param cbData  the event: type in word 0, then its arguments
 * @return the callback's result, or 0 when there is none
 */
int libmpeg_dispatchCallback(int *mpeg, int *cbData) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_raiseNeedData);
#ifdef SKIP_ASM
extern int libmpeg_dispatchCallback(int *, int *);
/**
 * Raises the decoder's "needs data" callback (type 1).
 *
 * Builds an event block with type 1 and passes it to libmpeg_dispatchCallback. The bitstream readers call it when the IPU input
 * runs dry; libmpeg_decodeFramePicture and libmpeg_decodeFieldPair also call it for a skipped picture.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
int libmpeg_raiseNeedData(int *mpeg) {
    int cbData[8];
    cbData[0] = 1;
    libmpeg_dispatchCallback(mpeg, cbData);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_enableTimeStampStep);
#ifdef SKIP_ASM
/**
 * Turns on time-stamp interpolation with a step value.
 *
 * Sets the flag at internal +0x70 and stores the 64-bit step at +0x78; libmpeg_computeTimeStamps then derives a picture's time
 * stamp from the previous one and this step when the picture carries none.
 *
 * @param mpeg  the decoder handle
 * @param timeStep  the 64-bit interpolation step
 * @return 1
 */
int libmpeg_enableTimeStampStep(int *mpeg, long timeStep) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[28] = 1;
    *(long *)&mpegSys[30] = timeStep;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_disableTimeStampStep);
#ifdef SKIP_ASM
/**
 * Turns off time-stamp interpolation (clears the flag at internal +0x70 and the step at +0x78).
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
int libmpeg_disableTimeStampStep(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[28] = 0;
    *(long *)&mpegSys[30] = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_setOutputAddress);
#ifdef SKIP_ASM
/**
 * Sets the decoder's output picture address (internal +0xD8) without decoding.
 *
 * @param mpeg  the decoder handle
 * @param outputAddr  the output address
 * @return 1
 */
int libmpeg_setOutputAddress(int *mpeg, int outputAddr) {
    int *mpegSys = (int *)mpeg[16];
    mpegSys[54] = outputAddr;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_00402CE8);
#ifdef SKIP_ASM
/**
 * Returns the decoder word at internal +0xCC (set from the current picture by libmpeg_outputFramePicture / libmpeg_outputFieldPair).
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
 * Returns the decoder word at internal +0xD0 (set from the current picture by libmpeg_outputFramePicture / libmpeg_outputFieldPair).
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
 * libmpeg_selectPictureRecord reads and clears the flag while searching for the next picture.
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_queueTimeStamp);
#ifdef SKIP_ASM
/**
 * Queues a 64-bit time stamp for the next decoded picture.
 *
 * Stores it at internal +0xF0 and sets the state word +0xF8 to 1; once that state reaches 2, libmpeg_computeTimeStamps hands the
 * stamp to the next picture instead of its own and resets it to -1.
 *
 * @param mpeg  the decoder handle
 * @param timeStamp  the time stamp
 * @return 1
 */
int libmpeg_queueTimeStamp(int *mpeg, long timeStamp) {
    int *mpegSys = (int *)mpeg[16];
    *(long *)&mpegSys[60] = timeStamp;
    mpegSys[62] = 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_initPool);
#ifdef SKIP_ASM
/**
 * Initialises a memory pool over a block: base, size, current and mark all start at the base.
 *
 * @param heap  the pool record (base, size, current, mark)
 * @param base  the block's address
 * @param size  the block's size in bytes
 */
void libmpeg_initPool(int *heap, int base, int size) {
    heap[0] = base;
    heap[1] = size;
    heap[2] = base;
    heap[3] = base;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_markPool);
#ifdef SKIP_ASM
/**
 * Records the pool's current position as its mark (mark = current).
 *
 * @param heap  the pool record
 * @return the new mark
 */
int libmpeg_markPool(int *heap) {
    return heap[3] = heap[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_rewindPool);
#ifdef SKIP_ASM
/**
 * Rewinds the pool to its mark, freeing everything allocated since (current = mark).
 *
 * func_00403748 uses it to rebuild the picture buffers for a new sequence.
 *
 * @param heap  the pool record
 * @return the new current position
 */
int libmpeg_rewindPool(int *heap) {
    return heap[2] = heap[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_allocFromPool);
#ifdef SKIP_ASM
extern char str_work_area_size_is_too_small[];
extern void libmpeg_reportError(int *, int);
/**
 * Allocates an aligned block from a memory pool.
 *
 * Rounds the current position up to @p align and takes @p size bytes when they fit before base + size; otherwise
 * reports "work area size is too small" through the decoder's error path (libmpeg_reportError).
 *
 * @param decState  the decoder state errors are reported for
 * @param pool  the pool record (base, size, current, mark)
 * @param size  the block size in bytes
 * @param align  the alignment in bytes
 * @return the block's address, or 0 when the pool is exhausted
 */
unsigned libmpeg_allocFromPool(int *decState, unsigned *pool, unsigned size, unsigned align) {
    unsigned blockStart = (pool[2] + align - 1) / align * align;
    unsigned blockEnd = blockStart + size;
    if (pool[0] + pool[1] >= blockEnd) {
        pool[2] = blockEnd;
        return blockStart;
    }
    libmpeg_reportError(decState, (int)str_work_area_size_is_too_small);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_getPoolFree);
#ifdef SKIP_ASM
/**
 * Returns how many bytes are left in a memory pool (base + size - current).
 *
 * @param heap  the pool record
 * @return the free bytes
 */
int libmpeg_getPoolFree(int *heap) {
    return heap[0] + heap[1] - heap[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeUntilPicture);
#ifdef SKIP_ASM
extern char str_image_buffer_needs_to_be_aligned_to_64by[];
extern void libmpeg_reportErrorf_k() __asm__("libmpeg_reportErrorf");
extern int libmpeg_flushHeldPicture();
extern int libmpeg_decodePicture();
extern int libmpeg_findNextPicture();
/**
 * Runs the MPEG decode loop until one picture has been output or the stream ends.
 *
 * Fails with "image buffer needs to be aligned to 64byte boundary" (libmpeg_reportErrorf) unless the output address (internal
 * +0xD8) is 64-byte aligned. Otherwise it repeatedly finds the next picture (libmpeg_findNextPicture, which returns its type: 0
 * none, 1 I, 2 P, 3 or 4 B) and decodes it with libmpeg_decodePicture, passing the per-type counter (+0xA0/+0xA4/+0xA8; an I
 * picture restarts all three) and the type's decode limit (+0x94..+0x9C). When no picture is left it flushes the held
 * reference picture (libmpeg_flushHeldPicture) and sets the end flag (word 0). Returns once a picture was output (+0x820) or the
 * end flag is set.
 *
 * @param mpeg  the decoder handle
 * @return 1, or -1 when the output buffer is misaligned
 */
int libmpeg_decodeUntilPicture(int *mpeg) {
    int *decState = (int *)mpeg[16];
    int picResult = 0;
    int picType = 1;
    decState[0] = 0;
    if ((decState[54] & 0x3F) != 0) {
        libmpeg_reportErrorf_k(decState, str_image_buffer_needs_to_be_aligned_to_64by, decState[54]);
        return -1;
    }
    decState[520] = 0;
    for (;;) {
        if (picResult != -1) {
            do {
                picType = libmpeg_findNextPicture(decState);
                if (picType == 0)
                    break;
                if (decState[93] == decState[53])
                    break;
            } while (decState[530] != 0);
        }
        switch (picType) {
        case 0:
            libmpeg_flushHeldPicture(mpeg);
            decState[0] = 1;
            break;
        case 1:
            decState[42] = 0;
            decState[41] = 0;
            decState[40] = 0;
            picResult = libmpeg_decodePicture(mpeg, 0, decState[37]);
            decState[40]++;
            break;
        case 2:
            picResult = libmpeg_decodePicture(mpeg, decState[41], decState[38]);
            decState[41]++;
            break;
        case 3:
        case 4:
            picResult = libmpeg_decodePicture(mpeg, decState[42], decState[39]);
            decState[42]++;
            break;
        }
        if (decState[520] != 0)
            return 1;
        if (decState[0] != 0)
            return 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeFramePicture);
#ifdef SKIP_ASM
struct P_2F80 { char pad[4]; int f4; int f8; char padC[0xA0]; int fAC; char pad2[0x68]; int f118; char pad3[4]; int f120; char pad4[0x50]; int f174; };
struct A_2F80 { char pad[8]; int f8; char pad2[0x34]; struct P_2F80 *f40; };
extern int s2_7F20(struct P_2F80 *, int) __asm__("libmpeg_selectPictureRecord");
extern int s2_7DC0(struct P_2F80 *) __asm__("libmpeg_decodePictureSlices");
extern void s2_2C80(struct A_2F80 *) __asm__("libmpeg_raiseNeedData");
extern void s2_7E90(struct P_2F80 *, int, int) __asm__("libmpeg_outputDuePicture");
/**
 * Decodes (or skips) one frame picture and advances the picture counters.
 *
 * A picture whose index is below the type's limit (or with no limit, -1) is decoded: libmpeg_selectPictureRecord(state, 0) then
 * libmpeg_decodePictureSlices; past the limit only libmpeg_selectPictureRecord runs and the needs-data callback is raised (libmpeg_raiseNeedData). Then
 * libmpeg_outputDuePicture queues the output, the alternating flag +0x120 flips for frame pictures (unless skipped), the handle's
 * frame number mpeg[2] becomes +0x118 - +0xAC, and the counters +0x118 / +0x04 advance when +0x120 is clear.
 * libmpeg_decodePicture calls it for frame pictures (+0x174 == 3).
 *
 * @param mpeg  the decoder handle
 * @param picIndex  the index of this picture among its type
 * @param picLimit  the type's decode limit, -1 for none
 * @return non-zero when the picture was decoded
 */
int libmpeg_decodeFramePicture(struct A_2F80 *mpeg, int picIndex, int picLimit) {
    struct P_2F80 *decState = mpeg->f40;
    int t, ret;
    int flag = 0;
    if (picLimit == -1 || picIndex < picLimit) {
        if (decState->f8 == 0) {
            mpeg->f8 = 0;
            decState->f8 = 1;
        }
        t = s2_7F20(decState, 0) ? s2_7DC0(decState) != 0 : 0;
        ret = t;
    } else {
        ret = s2_7F20(decState, 0);
        flag = 1;
        s2_2C80(mpeg);
    }
    s2_7E90(decState, decState->f118, decState->f4);
    if ((t = decState->f174) != 3 && flag == 0)
        decState->f120 = decState->f120 == 0;
    mpeg->f8 = decState->f118 - decState->fAC;
    if (decState->f120 == 0) {
        decState->f118++;
        decState->f4++;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodePicture);
#ifdef SKIP_ASM
struct P_3098 { char pad[0x174]; int f174; };
struct A_3098 { char pad[0x40]; struct P_3098 *f40; };
extern int s98_30E0(struct A_3098 *, int, int) __asm__("libmpeg_decodeFieldPair");
extern int s98_2F80(struct A_3098 *, int, int) __asm__("libmpeg_decodeFramePicture");
/**
 * Decodes one picture: field pictures go to libmpeg_decodeFieldPair, frame pictures to libmpeg_decodeFramePicture.
 *
 * Chooses by the picture structure at internal +0x174 (3 = frame). Called by the decode loop libmpeg_decodeUntilPicture.
 *
 * @param mpeg  the decoder handle
 * @param picIndex  the index of this picture among its type
 * @param picLimit  the type's decode limit, -1 for none
 * @return the chosen routine's result
 */
int libmpeg_decodePicture(struct A_3098 *mpeg, int picIndex, int picLimit) {
    if (mpeg->f40->f174 != 3) return s98_30E0(mpeg, picIndex, picLimit);
    return s98_2F80(mpeg, picIndex, picLimit);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeFieldPair);
#ifdef SKIP_ASM
struct P_30E0 { int f0; int f4; int f8; char padC[0xA0]; int fAC; char pad2[0x24]; int fD4; char pad3[0x40]; int f118; char pad4[4]; int f120; char pad5[0x50]; int f174; };
struct A_30E0 { char pad[8]; int f8; char pad2[0x34]; struct P_30E0 *f40; };
extern int s3_7F20(struct P_30E0 *, int) __asm__("libmpeg_selectPictureRecord");
extern int s3_7DC0(struct P_30E0 *) __asm__("libmpeg_decodePictureSlices");
extern void s3_2C80(struct A_30E0 *) __asm__("libmpeg_raiseNeedData");
extern void s3_7E90(struct P_30E0 *, int, int) __asm__("libmpeg_outputDuePicture");
extern int s3_7520(struct P_30E0 *) __asm__("libmpeg_findNextPicture");
extern void s3_3240(struct A_30E0 *) __asm__("libmpeg_flushHeldPicture");
/**
 * Decodes (or skips) a field-picture pair: the first field, then the matching second field.
 *
 * Decodes the first field when the picture is within the type's limit (libmpeg_selectPictureRecord(state, 0), libmpeg_decodePictureSlices), then
 * finds the next picture (libmpeg_findNextPicture); none flushes the reference picture and sets the end flag. The second field
 * must have the other field structure (2 when +0xD4 is 1, else 1) or it returns -1. Decodes it the same way, queues the
 * output (libmpeg_outputDuePicture), sets the frame number mpeg[2], advances +0x118 / +0x04 and raises the needs-data callback
 * (libmpeg_raiseNeedData) when the pair was skipped.
 *
 * @param mpeg  the decoder handle
 * @param picIndex  the index of this picture among its type
 * @param picLimit  the type's decode limit, -1 for none
 * @return 1 when the second field was decoded, 0 otherwise, -1 for a field mismatch
 */
int libmpeg_decodeFieldPair(struct A_30E0 *mpeg, int picIndex, int picLimit) {
    struct P_30E0 *decState = mpeg->f40;
    int flag = 0;
    int ret, k, secondFieldOk;
    decState->f120 = 0;
    if (picLimit == -1 || picIndex < picLimit)
        flag = 1;
    if (decState->f8 == 0) {
        mpeg->f8 = 0;
        decState->f8 = 1;
    }
    if (s3_7F20(decState, 0)) {
        if (flag)
            s3_7DC0(decState);
    }
    decState->f120 = 1;
    if (s3_7520(decState) == 0) {
        s3_3240(mpeg);
        decState->f0 = 1;
        return 0;
    }
    k = 2;
    if (decState->fD4 != 1)
        k = 1;
    if (decState->f174 != k)
        return -1;
    ret = 0;
    secondFieldOk = 0;
    if (s3_7F20(decState, 1))
        secondFieldOk = 1;
    if (secondFieldOk && flag) {
        if (s3_7DC0(decState))
            ret = 1;
    }
    s3_7E90(decState, decState->f118, decState->f4);
    decState->f120 = 0;
    mpeg->f8 = decState->f118 - decState->fAC;
    decState->f118++;
    decState->f4++;
    if (!flag)
        s3_2C80(mpeg);
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_flushHeldPicture);
#ifdef SKIP_ASM
extern void libmpeg_outputHeldPicture(int *);
/**
 * Outputs the reference picture still held at the end of the stream.
 *
 * When a reference picture is held (internal words 1 and 2), converts/outputs it (libmpeg_outputHeldPicture), sets the handle's
 * frame number mpeg[2] to +0x118 - +0xAC and clears the held flag.
 *
 * @param mpeg  the decoder handle
 * @return 1 when a picture was output, else 0
 */
int libmpeg_flushHeldPicture(int *mpeg) {
    int *mpegSys = (int *)mpeg[16];
    int result = 0;
    if (mpegSys[1] != 0 && mpegSys[2] != 0) {
        libmpeg_outputHeldPicture(mpegSys);
        mpeg[2] = mpegSys[70] - mpegSys[43];
        mpegSys[1] = 0;
        result = 1;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_resetToMpeg1);
#ifdef SKIP_ASM
extern void libmpeg_setIpuMpeg1(unsigned);
/**
 * Puts the decoder back in MPEG-1 mode: clears the MPEG-2 flag (+0x848) and calls libmpeg_setIpuMpeg1(1).
 *
 * The sequence extension parser libmpeg_parseSequenceExtension sets the flag (and calls libmpeg_setIpuMpeg1(0)). Called by sceMpegReset.
 *
 * @param p  the decoder's internal state
 */
void libmpeg_resetToMpeg1(int *p) {
    p[530] = 0;
    libmpeg_setIpuMpeg1(1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_outputHeldPicture);
#ifdef SKIP_ASM
extern void libmpeg_reportError(int *, int);
extern char str_the_second_field_is_missing[];
extern void libmpeg_outputFramePicture(int *, int, int);
extern void libmpeg_outputFieldPair(int *, int, int, int);
/**
 * Outputs the held reference picture into the caller's buffer.
 *
 * Reports "the second field is missing" (libmpeg_reportError) when a field picture is still incomplete (+0x120); otherwise
 * converts it with libmpeg_outputFramePicture (frame pictures, +0x174 == 3) or libmpeg_outputFieldPair (field pictures), giving the picture
 * count +0x118 minus one. Clears +0x120.
 *
 * @param decState  the decoder's internal state
 */
void libmpeg_outputHeldPicture(int *decState) {
    int n = decState[280 / 4];
    if (decState[288 / 4] != 0) {
        libmpeg_reportError(decState, (int)str_the_second_field_is_missing);
    } else if (decState[372 / 4] == 3) {
        libmpeg_outputFramePicture(decState, decState[444 / 4], n - 1);
    } else {
        libmpeg_outputFieldPair(decState, decState[460 / 4], decState[476 / 4], n - 1);
    }
    decState[288 / 4] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_setupScratchpad);
#ifdef SKIP_ASM
extern void libmpeg_setIpuMpeg1(unsigned);
/**
 * Lays out the decoder's scratchpad buffers.
 *
 * Calls libmpeg_setIpuMpeg1(1), then points the two buffer pairs at scratchpad 0x70000000 / 0x70001800 (internal
 * +0x590/+0x594) and 0x70001B00 / 0x70003300 (+0x6D0/+0x6D4) and clears +0x810. Called by sceMpeg setup sceMpegCreate.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_setupScratchpad(int *mpegSys) {
    int scratchBase = 0x70000000;
    libmpeg_setIpuMpeg1(1);
    mpegSys[356] = scratchBase;
    mpegSys[357] = 0x70001800;
    mpegSys[436] = 0x70001B00;
    mpegSys[437] = 0x70003300;
    mpegSys[516] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_stopIpu);
#ifdef SKIP_ASM
extern int DIntr_0() __asm__("DIntr");
extern void EIntr(void);
extern void sceIpuSync_2(int, int) __asm__("sceIpuSync");
/**
 * Stops the IPU, its DMA channels and the to-scratchpad DMA, and resets the IPU.
 *
 * Sets +0x818 and clears +0x1B0; with interrupts off holds the DMA controller (D_ENABLEW) while zeroing the CHCR of
 * channels 3 and 4 (from/to IPU) and 9 (to scratchpad), then zeroes their QWC, resets the IPU (IPU_CTRL = 0x40000000)
 * and calls sceIpuSync(0, 0). Called by sceMpegReset.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_stopIpu(int *mpegSys) {
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
    sceIpuSync_2(0, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_printError);
#ifdef SKIP_ASM
extern char str_MPEG_ERROR_pct_s[];
extern void scePrintf(char *, int);
/**
 * Prints a decoder error message as "[MPEG ERROR]%s".
 *
 * The fallback of libmpeg_reportError when no error callback is installed.
 *
 * @param message  the message
 */
void libmpeg_printError(int message) {
    scePrintf(str_MPEG_ERROR_pct_s, message);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_reportErrorf);
#ifdef SKIP_ASM
extern int sprintf(char *, const char *, ...);
extern void libmpeg_reportError(int *, int);
/**
 * Formats a decoder error message into a 256-byte buffer and reports it (libmpeg_reportError).
 *
 * The callers pass one value for the format (e.g. the misaligned address) as a third argument that this C does not
 * declare; it still reaches sprintf because nothing between entry and the call touches the third argument register.
 *
 * @param mpegSys  the decoder's internal state
 * @param fmt  the printf-style message
 */
void libmpeg_reportErrorf(int *mpegSys, char *fmt) {
    char buf[256];
    sprintf(buf, fmt);
    libmpeg_reportError(mpegSys, (int)buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_reportError);
#ifdef SKIP_ASM
extern int libmpeg_dispatchCallback(int *, int *);
extern void libmpeg_printError(int);
/**
 * Reports a decoder error: to the error callback when one is installed, else to the console.
 *
 * When the state links back to its handle (+0x858) and an error callback (type 0, +0x0C) is set, calls it with an event
 * block of type 0 and the message (libmpeg_dispatchCallback); otherwise prints "[MPEG ERROR]%s" (libmpeg_printError).
 *
 * @param mpegSys  the decoder's internal state
 * @param message  the message
 */
void libmpeg_reportError(int *mpegSys, int message) {
    int cbData[2];
    int *mpeg = (int *)mpegSys[534];
    if (mpeg && mpegSys && mpegSys[3]) {
        cbData[0] = 0;
        cbData[1] = message;
        libmpeg_dispatchCallback(mpeg, cbData);
        return;
    }
    libmpeg_printError(message);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_startInputDma);
#ifdef SKIP_ASM
extern void FlushCache(int) __asm__("FlushCache");
extern int DIntr();
extern void EIntr(void);
void q3500(int base, unsigned addr, int n) __asm__("libmpeg_startInputDma");
/**
 * Starts the IPU input DMA (channel 4, chain mode) over a bitstream block.
 *
 * Builds DMA ref tags in the 16-byte aligned tag area at state + 0x880: one per chunk of at most 0xFFF40 bytes, ID 3
 * (ref) for all but the last (ID 0, refe), each with the chunk's quadword count and address. Then flushes the cache
 * and, with interrupts off, sets D4_TADR to the tags, D4_QWC to 0 and D4_CHCR to 0x105 (start, chain). sceMpegAddBs
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_setPictureSize);
#ifdef SKIP_ASM
/**
 * Fills a picture-size record: width, height and both in macroblocks (>> 4).
 *
 * @param picInfo  the record (words 1..4 written)
 * @param width  the width in pixels
 * @param height  the height in pixels
 * @return 1
 */
int libmpeg_setPictureSize(int *picInfo, int width, int height) {
    picInfo[1] = width;
    picInfo[2] = height;
    picInfo[3] = width >> 4;
    picInfo[4] = height >> 4;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parseSequenceHeader);
#ifdef SKIP_ASM
extern char D_00453200[];
extern char D_00453240[];
extern char D_00497CF8[];
extern int libmpeg_readBits();
extern void libmpeg_loadDefaultMatrix();
extern void func_00403748();
extern void libmpeg_readExtensions();
/**
 * Parses an MPEG sequence header and loads its quantiser matrices into the IPU.
 *
 * Reads the 12-bit horizontal and vertical sizes (+0x124, +0x128; "vertical size > 2800" is reported), the bit rate
 * (+0x134) and VBV buffer size (+0x138) through libmpeg_readBits. For each of the intra and non-intra matrices the load
 * flag goes to +0x840 / +0x844: when set the IPU reads the matrix from the bitstream (SETIQ command 0x50000000 /
 * 0x58000000 via libmpeg_issueIpuCommand), otherwise the default table D_00453200 / D_00453240 is DMA'd in (libmpeg_loadDefaultMatrix). Then
 * reads the extensions and user data (libmpeg_readExtensions) and sets the picture buffers up (func_00403748).
 *
 * @param decState  the decoder's internal state
 */
void libmpeg_parseSequenceHeader(int *decState) {
    unsigned headerBits;
    int w;
    int loadMatrix;
    decState[212 / 4] = 0;
    headerBits = libmpeg_readBits(decState, 32);
    w = (headerBits >> 8) & 0xFFF;
    decState[292 / 4] = headerBits >> 20;
    decState[296 / 4] = w;
    if (w >= 2801)
        libmpeg_reportError(decState, (int)D_00497CF8);
    headerBits = libmpeg_readBits(decState, 30);
    decState[308 / 4] = headerBits >> 12;
    decState[312 / 4] = (headerBits >> 1) & 0x3FF;
    loadMatrix = libmpeg_readBits(decState, 1);
    decState[2112 / 4] = loadMatrix;
    if (loadMatrix != 0) {
        libmpeg_waitIpuIdle((int)decState);
        libmpeg_issueIpuCommand(decState, 0x50000000);
        libmpeg_waitIpuIdle((int)decState);
    } else {
        libmpeg_loadDefaultMatrix(decState, 0x50000000, D_00453200);
    }
    loadMatrix = libmpeg_readBits(decState, 1);
    decState[2116 / 4] = loadMatrix;
    if (loadMatrix != 0) {
        libmpeg_waitIpuIdle((int)decState);
        libmpeg_issueIpuCommand(decState, 0x58000000);
        libmpeg_waitIpuIdle((int)decState);
    } else {
        libmpeg_loadDefaultMatrix(decState, 0x58000000, D_00453240);
    }
    libmpeg_readExtensions(decState);
    func_00403748(decState[2136 / 4]);
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00403748);

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_computePlaneAddresses);
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
void libmpeg_computePlaneAddresses(int *y0, int *uOut0, int *vOut0, int *y1, int *uOut1, int *vOut1, int *y2, int *uOut2, int *vOut2,
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_loadDefaultMatrix);
#ifdef SKIP_ASM
extern int f3AD0_DI() __asm__("DIntr");
extern void f3AD0_EI(void) __asm__("EIntr");
extern int f3AD0_2C30(int, int *) __asm__("libmpeg_dispatchCallback");
extern void f3AD0_6E20(int *) __asm__("libmpeg_waitIpuIdle");
extern void f3AD0_6DF0(int *, int) __asm__("libmpeg_issueIpuCommand");
/**
 * Loads a default quantiser matrix into the IPU by DMA, between the stop and restart DMA callbacks.
 *
 * Raises callback 2 (stop DMA), waits for the IPU (libmpeg_waitIpuIdle), clears its input FIFO (BCLR), then with interrupts
 * off points channel 4 (to IPU) at @p srcAddr for 4 quadwords and starts it (CHCR 0x101); issues @p ipuCmd
 * (libmpeg_issueIpuCommand), waits again and raises callback 3 (restart DMA). Called by libmpeg_parseSequenceHeader.
 *
 * @param mpegSys  the decoder's internal state
 * @param ipuCmd  the IPU SETIQ command (0x50000000 or 0x58000000)
 * @param srcAddr  the 64-byte matrix
 */
void libmpeg_loadDefaultMatrix(int *mpegSys, int ipuCmd, int srcAddr) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parseSequenceExtension);
#ifdef SKIP_ASM
extern char D_00497D10[];
extern char D_00497D38[];
extern void libmpeg_setIpuMpeg1(unsigned);
/**
 * Parses an MPEG-2 sequence extension (extension table libmpeg_extensionHandlerTable, id 1).
 *
 * Marks the stream as MPEG-2 (+0x848 = 1, libmpeg_setIpuMpeg1(0)) and reads profile/level, progressive_sequence (+0x13C),
 * chroma_format (+0x140; anything but 1 reports "_chroma_format needs to be 1: 420"), the size, bit-rate and VBV-size
 * extensions, which it merges into +0x124, +0x128, +0x134 and +0x138. Profile/level other than 0x48, 0x58 or 0x44
 * (main/simple profile at main level, main at high) reports "Unsupported profile/level".
 *
 * @param decState  the decoder's internal state
 */
void libmpeg_parseSequenceExtension(int *decState) {
    unsigned extBits;
    unsigned profileLevel;
    unsigned w;
    int chromaFormat;
    unsigned x;
    unsigned y;
    unsigned z;
    decState[2120 / 4] = 1;
    libmpeg_setIpuMpeg1(0);
    extBits = libmpeg_readBits(decState, 28);
    x = (extBits >> 1) & 0xFFF;
    chromaFormat = (extBits >> 17) & 3;
    y = (extBits >> 13) & 3;
    z = (extBits >> 15) & 3;
    decState[320 / 4] = chromaFormat;
    if (chromaFormat != 1)
        libmpeg_reportError(decState, (int)D_00497D10);
    decState[316 / 4] = (extBits >> 19) & 1;
    profileLevel = extBits >> 20;
    w = (unsigned)libmpeg_readBits(decState, 16) >> 8;
    if (profileLevel != 72 && profileLevel != 88 && profileLevel != 68)
        libmpeg_reportError(decState, (int)D_00497D38);
    decState[292 / 4] = (z << 12) | (decState[292 / 4] & 0xFFF);
    decState[296 / 4] = (y << 12) | (decState[296 / 4] & 0xFFF);
    decState[308 / 4] = decState[308 / 4] + (x << 18);
    decState[312 / 4] = decState[312 / 4] + (w << 10);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parseSequenceDisplayExtension);
#ifdef SKIP_ASM
extern int libmpeg_readBits(int, int);
/**
 * Parses an MPEG-2 sequence display extension (extension table libmpeg_extensionHandlerTable, id 2).
 *
 * Skips video_format; with colour_description set skips the primaries and transfer characteristics and keeps the matrix
 * coefficients (+0x144); keeps the display width (+0x148) and height (+0x14C).
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_parseSequenceDisplayExtension(int *mpegSys) {
    libmpeg_readBits((int)mpegSys, 3);
    if (libmpeg_readBits((int)mpegSys, 1)) {
        libmpeg_readBits((int)mpegSys, 8);
        libmpeg_readBits((int)mpegSys, 8);
        mpegSys[324 / 4] = libmpeg_readBits((int)mpegSys, 8);
    }
    mpegSys[328 / 4] = libmpeg_readBits((int)mpegSys, 0xE);
    libmpeg_readBits((int)mpegSys, 1);
    mpegSys[332 / 4] = libmpeg_readBits((int)mpegSys, 0xE);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _sequenceScalableExtension);
#ifdef SKIP_ASM
extern char str_sequenceScalableExtension_is_not_impleme[];
extern void libmpeg_reportError(int *, int);
/**
 * Reports that MPEG-2 sequence scalable extensions are not supported.
 *
 * Passes "_sequenceScalableExtension() is not implemented" to libmpeg_reportError(), which hands it to the decoder's
 * error callback when one is set and prints it otherwise.
 *
 * @param self  the MPEG decoder state
 */
void _sequenceScalableExtension(int *self) {
    libmpeg_reportError(self, (int)str_sequenceScalableExtension_is_not_impleme);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_onUnknownExtension);
#ifdef SKIP_ASM
extern char str_Unknown_Extension[];
extern void libmpeg_reportError(int *, int);
/**
 * Reports "Unknown Extension" for an MPEG extension id the decoder does not handle.
 *
 * Entries 0 and 6 (reserved ids) of the extension handler table libmpeg_extensionHandlerTable; reports through libmpeg_reportError.
 *
 * @param decState  the decoder's internal state
 */
void libmpeg_onUnknownExtension(int *decState) {
    libmpeg_reportError(decState, (int)str_Unknown_Extension);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _pictureSpatialScalableExtension);
#ifdef SKIP_ASM
extern char str_pictureSpatialScalableExtension_is_not_s[];
extern void libmpeg_reportError(int *, int);
/**
 * Reports that the MPEG picture spatial scalable extension is not supported.
 *
 * Entry 9 of the extension handler table libmpeg_extensionHandlerTable (MPEG-2 extension id 9); reports its "not supported" message
 * str_pictureSpatialScalableExtension_is_not_s through libmpeg_reportError.
 *
 * @param decState  the decoder's internal state
 */
void _pictureSpatialScalableExtension(int *decState) {
    libmpeg_reportError(decState, (int)str_pictureSpatialScalableExtension_is_not_s);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _pictureTemporalScalableExtension);
#ifdef SKIP_ASM
extern char str_pictureTemporalScalableExtension_is_not[];
extern void libmpeg_reportError(int *, int);
/**
 * Reports that the MPEG picture temporal scalable extension is not supported.
 *
 * Entry 10 of the extension handler table libmpeg_extensionHandlerTable (MPEG-2 extension id 10); reports its "not supported" message
 * str_pictureTemporalScalableExtension_is_not through libmpeg_reportError.
 *
 * @param decState  the decoder's internal state
 */
void _pictureTemporalScalableExtension(int *decState) {
    libmpeg_reportError(decState, (int)str_pictureTemporalScalableExtension_is_not);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_defaultStopDma);
#ifdef SKIP_ASM
extern void sceIpuStopDMA(void *);
/**
 * Default "stop DMA" decoder callback (type 2): pauses the IPU DMA and saves its registers.
 *
 * Runs sceIpuStopDMA on the save block at internal +0x4C. sceMpegCreate installs it; libmpeg_loadDefaultMatrix and libmpeg_convertToRgb32
 * raise type 2 before using the IPU themselves.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
int libmpeg_defaultStopDma(int *mpeg) {
    sceIpuStopDMA((char *)mpeg[16] + 0x4C);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_defaultRestartDma);
#ifdef SKIP_ASM
extern void sceIpuRestartDMA(void *);
/**
 * Default "restart DMA" decoder callback (type 3): restores and restarts the IPU DMA.
 *
 * Runs sceIpuRestartDMA on the save block at internal +0x4C that libmpeg_defaultStopDma filled. sceMpegCreate installs it.
 *
 * @param mpeg  the decoder handle
 * @return 1
 */
int libmpeg_defaultRestartDma(int *mpeg) {
    sceIpuRestartDMA((char *)mpeg[16] + 0x4C);
    return 1;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00403E00);

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_formPredictions);
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
extern void q4050_5E30(int *, int *, int *, int, int) __asm__("libmpeg_calcDualPrimeVectors");
extern void q4050_3470(int *, char *, int) __asm__("libmpeg_reportErrorf");

/**
 * Forms a macroblock's motion-compensated prediction (MSSG mpeg2decode's form_predictions).
 *
 * Clears the current macroblock record's prediction flag (+0x6BC of slot +0x810), then for a forward vector (bit 8 of
 * @p mbtype, or any P picture) and a backward vector (bit 4) issues the predictions with func_00404758 from the forward
 * / backward reference pictures (+0x1B8, +0x1BC, or the field references +0x1C8..+0x1DC for field pictures), by motion
 * type: frame, field, or dual prime (vectors from libmpeg_calcDualPrimeVectors). Backward after forward averages. An invalid motion
 * type reports "(a)/(b)/(c) invalid motion_type(%d)" (libmpeg_reportErrorf). Called by func_00403E00.
 *
 * @param decState  the decoder's internal state
 * @param bx  the macroblock's x position in pixels
 * @param by  the macroblock's y position in pixels
 * @param mbtype  the macroblock_type bits
 * @param mtype  the motion type
 * @param PMV  the motion vectors [r][s][t]
 * @param mvfs  the motion_vertical_field_select flags
 * @param dmv  the dual-prime differential vector
 */
void libmpeg_formPredictions(int *decState, int bx, int by, int mbtype, int mtype, int PMV[2][2][2], int mvfs[2][2], int *dmv)
{
    int DMV[2][2];
    int ref[2][2];
    int cur, sel, avg, fwd;

    sel = 1;
    avg = 0;
    *(int *)((char *)decState + decState[0x810 / 4] * 0x140 + 0x6BC) = 0;
    fwd = mbtype & 8;
    if (fwd || decState[336 / 4] == 2) {
        if (decState[372 / 4] == 3) {
            if (mtype == 2 || !fwd) {
                q4050_4758(decState, decState[440 / 4], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
            } else if (mtype == 1) {
                q4050_4758(decState, decState[440 / 4], mvfs[0][0], 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, 1, 0);
                q4050_4758(decState, decState[440 / 4], mvfs[1][0], 1, 0, 8, bx, by, PMV[1][0][0], PMV[1][0][1] >> 1, 1, 0);
            } else if (mtype == 3) {
                q4050_5E30(decState, &DMV[0][0], dmv, PMV[0][0][0], PMV[0][0][1] >> 1);
                q4050_4758(decState, decState[440 / 4], 0, 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, sel, 0);
                q4050_4758(decState, decState[440 / 4], 1, 0, 0, 8, bx, by, DMV[0][0], DMV[0][1], sel, sel);
                q4050_4758(decState, decState[440 / 4], 1, 1, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1] >> 1, sel, 0);
                q4050_4758(decState, decState[440 / 4], 0, 1, 0, 8, bx, by, DMV[1][0], DMV[1][1], sel, sel);
            } else {
                q4050_3470(decState, D_00497E38, mtype);
            }
        } else {
            cur = decState[372 / 4] == 2;
            ref[0][0] = decState[456 / 4];
            ref[0][1] = decState[472 / 4];
            ref[1][0] = decState[460 / 4];
            ref[1][1] = decState[476 / 4];
            if (decState[336 / 4] == 2 && decState[288 / 4] && cur != mvfs[0][0])
                sel = 1;
            else
                sel = 0;
            if (mtype == 1 || !fwd) {
                q4050_4758(decState, ref[sel][mvfs[0][0]], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
            } else if (mtype == 2) {
                q4050_4758(decState, ref[sel][mvfs[0][0]], 0, 0, 0, 8, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
                if (decState[336 / 4] == 2 && decState[288 / 4] && cur != mvfs[1][0])
                    sel = 1;
                else
                    sel = 0;
                q4050_4758(decState, ref[sel][mvfs[1][0]], 0, 0, 8, 8, bx, by, PMV[1][0][0], PMV[1][0][1], 0, 0);
            } else if (mtype == 3) {
                if (decState[288 / 4])
                    sel = 1;
                else
                    sel = 0;
                q4050_5E30(decState, &DMV[0][0], dmv, PMV[0][0][0], PMV[0][0][1]);
                q4050_4758(decState, ref[0][cur], 0, 0, 0, 16, bx, by, PMV[0][0][0], PMV[0][0][1], 0, 0);
                q4050_4758(decState, ref[sel][!cur], 0, 0, 0, 16, bx, by, DMV[0][0], DMV[0][1], 0, 1);
            } else {
                q4050_3470(decState, D_00497E58, mtype);
            }
        }
        avg = 1;
    }
    if (mbtype & 4) {
        if (decState[372 / 4] == 3) {
            sel = 1;
            if (mtype == 2) {
                q4050_4758(decState, decState[444 / 4], 0, 0, 0, 16, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
            } else {
                q4050_4758(decState, decState[444 / 4], mvfs[0][1], 0, 0, 8, bx, by, PMV[0][1][0], PMV[0][1][1] >> 1, sel, avg);
                q4050_4758(decState, decState[444 / 4], mvfs[1][1], 1, 0, 8, bx, by, PMV[1][1][0], PMV[1][1][1] >> 1, sel, avg);
            }
        } else {
            if (mtype == 1) {
                q4050_4758(decState, mvfs[0][1] ? decState[476 / 4] : decState[460 / 4], 0, 0, 0, 16, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
            } else if (mtype == 2) {
                q4050_4758(decState, mvfs[0][1] ? decState[476 / 4] : decState[460 / 4], 0, 0, 0, 8, bx, by, PMV[0][1][0], PMV[0][1][1], 0, avg);
                q4050_4758(decState, mvfs[1][1] ? decState[476 / 4] : decState[460 / 4], 0, 0, 8, 8, bx, by, PMV[1][1][0], PMV[1][1][1], 0, avg);
            } else {
                q4050_3470(decState, D_00497E78, mtype);
            }
        }
    }
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_00404758);

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_finishMacroblock);
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
extern void libmpeg_reportError(int *, int);
extern char D_00497E98[];
extern void add_4B78(void *, void *) __asm__("libmpeg_storeClamped");
extern void mix_4B78(void *, void *, void *) __asm__("libmpeg_addResidual");
void f4B78(struct S4B78 *s, int n) __asm__("libmpeg_finishMacroblock");
/**
 * Finishes one decoded macroblock: runs its block copy routines and merges prediction and residual.
 *
 * Works on macroblock record @p n (two alternate, 0x140 bytes at +0x590). When it is enabled, runs each of its count
 * block routine pairs (fa / fb tables) on its block descriptors. A record flagged both intra (+0x130) and skipped
 * (+0x13C) reports "intra && skip MB". Intra: libmpeg_storeClamped(dst, src); skipped: libmpeg_storeClamped(dst, the prediction
 * +0x81C); otherwise libmpeg_addResidual combines dst, the prediction and the residual. Called by libmpeg_decodeSlice and
 * libmpeg_decodeSlices.
 *
 * @param decState  the decoder's internal state
 * @param n  the macroblock record (0 or 1)
 */
void f4B78(struct S4B78 *decState, int n) {
    int i;
    if (decState->e[n].enable != 0) {
        for (i = 0; i < decState->e[n].count; i++) {
            decState->e[n].fa[i](&decState->e[n].blk[i]);
            decState->e[n].fb[i](&decState->e[n].blk[i + 4]);
        }
    }
    if (decState->e[n].f130 != 0 && decState->e[n].f13C != 0)
        libmpeg_reportError((int *)decState, (int)D_00497E98);
    if (decState->e[n].f130 != 0)
        add_4B78(decState->e[n].dst, decState->e[n].src);
    else if (decState->e[n].f13C != 0)
        add_4B78(decState->e[n].dst, decState->f81C);
    else
        mix_4B78(decState->e[n].dst, decState->f81C, decState->e[n].src);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaCopy);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, full-pixel copy: widens a 16-pixel-wide block of 8-bit reference pixels to 16-bit.
 *
 * Luma routine D_00453280[0], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaCopy(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaCopy);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, full-pixel copy: widens an 8-pixel-wide block of 8-bit reference pixels
 * to 16-bit.
 *
 * Chroma routine D_004532A0[0], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaCopy(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaHalfV);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, vertical half-pixel: each sample is the rounded average of a pixel and the one below.
 *
 * Luma routine D_00453280[1], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaHalfV(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaHalfV);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, vertical half-pixel: each sample is the rounded average of a pixel and
 * the one below.
 *
 * Chroma routine D_004532A0[1], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaHalfV(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaHalfH);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, horizontal half-pixel: each sample is the rounded average of a pixel and its right
 * neighbour.
 *
 * Luma routine D_00453280[2], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaHalfH(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaHalfH);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, horizontal half-pixel: each sample is the rounded average of a pixel and
 * its right neighbour.
 *
 * Chroma routine D_004532A0[2], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaHalfH(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaHalfHV);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, half-pixel in both directions: each sample is the 2x2 neighbourhood's sum
 * plus 2, shifted right by 2.
 *
 * Luma routine D_00453280[3], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaHalfHV(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaHalfHV);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, half-pixel in both directions: each sample is the 2x2 neighbourhood's
 * sum plus 2, shifted right by 2.
 *
 * Chroma routine D_004532A0[3], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaHalfHV(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaCopyAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, full-pixel average: averages a 16-pixel-wide reference block into the
 * prediction already in the destination ((dst + ref + 1) >> 1), for bidirectional prediction.
 *
 * Luma routine D_00453280[4], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaCopyAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaCopyAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, full-pixel average: averages an 8-pixel-wide reference block into the
 * prediction already in the destination, rounding up, for bidirectional prediction.
 *
 * Chroma routine D_004532A0[4], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaCopyAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaHalfVAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, vertical half-pixel average: interpolates as D_00453280[1] does, then
 * averages the result into the prediction already in the destination, rounding up.
 *
 * Luma routine D_00453280[5], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaHalfVAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaHalfVAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, vertical half-pixel average: interpolates as D_004532A0[1] does, then
 * averages the result into the prediction already in the destination, rounding up.
 *
 * Chroma routine D_004532A0[5], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaHalfVAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaHalfHAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, horizontal half-pixel average: interpolates as D_00453280[2] does, then
 * averages the result into the prediction already in the destination, rounding up.
 *
 * Luma routine D_00453280[6], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaHalfHAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaHalfHAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, horizontal half-pixel average: interpolates as D_004532A0[2] does, then
 * averages the result into the prediction already in the destination, rounding up.
 *
 * Chroma routine D_004532A0[6], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaHalfHAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcLumaHalfHVAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, luma, two-way half-pixel average: interpolates as D_00453280[3] does, then
 * averages the result into the prediction already in the destination, rounding up.
 *
 * Luma routine D_00453280[7], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a luma block descriptor
 * ($4: 16-bit destination +0x0, rows two source strides apart; byte offset +0x4 for qfsrv; row counts +0x8 and +0xC,
 * the sources stepping 0x80 between the two runs; source stride +0x10; the two source quadword rows +0x14 / +0x18).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcLumaHalfHVAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_mcChromaHalfHVAvg);
#ifdef SKIP_ASM
/**
 * MPEG motion compensation, chroma, two-way half-pixel average: interpolates as D_004532A0[3] does, then
 * averages the result into the prediction already in the destination, rounding up.
 *
 * Chroma routine D_004532A0[7], chosen per block by func_00404758 and run by libmpeg_finishMacroblock on a chroma block descriptor
 * (fields as for the luma routines; the sources step 0x140 between the two row runs). It runs twice, the second time
 * for the other chroma plane (sources +0x40, destination +0x80).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_mcChromaHalfHVAvg(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_addResidual);
#ifdef SKIP_ASM
/**
 * Adds a macroblock's residual to its prediction and stores the result as 8-bit pixels.
 *
 * $4 the destination, $5 the 16-bit prediction, $6 the 16-bit residual: 24 passes of 16 samples (384, one 4:2:0
 * macroblock) are summed, clamped to 0..255 (D_00405B30) and packed to bytes (ppacb). libmpeg_finishMacroblock uses it for
 * macroblocks that are neither intra nor skipped.
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_addResidual(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_storeClamped);
#ifdef SKIP_ASM
/**
 * Stores a macroblock of 16-bit samples as 8-bit pixels, clamped to 0..255.
 *
 * $4 the destination, $5 the 384 16-bit samples (24 passes of 16; one 4:2:0 macroblock), clamped with the
 * 0x00FF halfword mask D_00405B30 that follows the code and packed to bytes (ppacb). libmpeg_finishMacroblock uses it for intra
 * macroblocks (the decoded block) and skipped ones (the prediction).
 */
// PORT: PS2-only hand-written MMI routine (no compiler emits its addi/mtsab/fixed-register code); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function body.
void libmpeg_storeClamped(void) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_setIpuMpeg1);
#ifdef SKIP_ASM
/**
 * Sets or clears the IPU's MPEG-1 mode bit (IPU_CTRL bit 23).
 *
 * libmpeg_resetToMpeg1 and libmpeg_setupScratchpad set it; the sequence extension parser libmpeg_parseSequenceExtension clears it for MPEG-2.
 *
 * @param mpeg1  1 for an MPEG-1 stream, 0 for MPEG-2
 */
void libmpeg_setIpuMpeg1(unsigned mpeg1) {
    volatile unsigned *ipuCtrl = (volatile unsigned *)0x10002010;
    *ipuCtrl = (*ipuCtrl & 0xFF7FFFFF) | (mpeg1 << 23);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_waitMacroblock);
#ifdef SKIP_ASM
extern void libmpeg_waitIpuIdle(int);
extern int libmpeg_dispatchCallback(int *, int *);
extern void libmpeg_reportError(int *, int);
extern int DIntr();
extern void EIntr(void);
extern char D_00497EB0[];
struct M5B70 { char pad0[0x838]; int f838; int f83C; char pad840[0x18]; int *f858; char pad85C[0x10A0 - 0x85C]; int f10A0; };
int f5B70(struct M5B70 *p) __asm__("libmpeg_waitMacroblock");
/**
 * Waits for the IPU to finish a macroblock and records its bit position; recovers from decode errors.
 *
 * Waits for the IPU (libmpeg_waitIpuIdle), then while the IPU-from DMA still has data and no error is flagged (IPU_CTRL bit
 * 14), raises the needs-data callback whenever the IPU-to DMA is idle. When the stop flag +0x10A0 is set it stops both
 * IPU DMA channels (DMA controller held, interrupts off), resets the IPU and returns 0. Otherwise it stores IPU_TOP in
 * +0x838 and the remaining bits in the buffer (+0x83C, 32 when the top is valid). On an IPU error it reports "Error
 * code detected(BDEC)", raises the stop/restart DMA callbacks around an IPU reset, stops channel 3 and returns 0.
 *
 * @param decState  the decoder's internal state
 * @return 1, or 0 after an error or a stop request
 */
int f5B70(struct M5B70 *decState) {
    int cmd[8];
    int cmd2[8];
    int ret = 1;
    int st;
    long top;
    int bp;
    int n;
    libmpeg_waitIpuIdle((int)decState);
    while (*(volatile int *)0x1000B020 != 0 && !(*(volatile int *)0x10002010 & 0x4000)) {
        if (*(volatile int *)0x1000B420 == 0 && !(*(volatile int *)0x1000B400 & 0x100)) {
            cmd[0] = 1;
            libmpeg_dispatchCallback(decState->f858, cmd);
        }
    }
    if (decState->f10A0 != 0) {
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
    decState->f838 = (int)top;
    if (top < 0) {
        if (bp & 0x1F)
            n = 0x20 - (bp & 0x1F);
        else
            n = 0;
        decState->f83C = n;
    } else {
        decState->f83C = 0x20;
    }
    if (*(volatile int *)0x10002010 & 0x4000) {
        libmpeg_reportError((int *)decState, (int)D_00497EB0);
        cmd2[0] = 2;
        libmpeg_dispatchCallback(decState->f858, cmd2);
        *(int *)0x10002010 = 0x40000000;
        cmd2[0] = 3;
        libmpeg_dispatchCallback(decState->f858, cmd2);
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeDualPrimeVector);
#ifdef SKIP_ASM
extern int libmpeg_decodeVlc(int, int);
/**
 * Decodes a dual-prime differential motion vector (IPU VDEC with table 3).
 *
 * Called twice by libmpeg_decodeMotionVector for a dual-prime vector.
 *
 * @param mpegSys  the decoder's internal state
 * @return the decoded value
 */
int libmpeg_decodeDualPrimeVector(int mpegSys) {
    return libmpeg_decodeVlc(mpegSys, 3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_calcDualPrimeVectors);
#ifdef SKIP_ASM
#define H1(x) (((x) + ((x) > 0)) >> 1)
#define H3(x) ((3 * (x) + ((x) > 0)) >> 1)
/**
 * Computes the dual-prime derived motion vectors (MSSG mpeg2decode's calc_DMV).
 *
 * For frame pictures (+0x174 == 3) it derives both field vectors, scaled by 1/2 and 3/2 with rounding away from zero,
 * in the order set by top_field_first (+0x178), with the vertical -1 / +1 field corrections; for field pictures one
 * vector, corrected by -1 (top field, +0x174 == 1) or +1. Called by libmpeg_formPredictions.
 *
 * @param mpegSys  the decoder's internal state
 * @param outVec  receives the derived vector(s)
 * @param in  the differential vector
 * @param a  the horizontal motion vector
 * @param b  the vertical motion vector
 */
void libmpeg_calcDualPrimeVectors(int *mpegSys, int *outVec, int *in, int a, int b) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeAddressIncrement);
#ifdef SKIP_ASM
extern char str_Invalid_macroblock_address_increment_cod[];
extern int lx5_6F78(int *, int) __asm__("libmpeg_decodeVlc");
extern int lx5_70F0(int *, int) __asm__("libmpeg_peekBits");
extern void lx5_71F8(int *, int) __asm__("libmpeg_skipBits");
extern void lx5_3470(int *, char *, int) __asm__("libmpeg_reportErrorf");
/**
 * Decodes a macroblock_address_increment, including stuffing and escape codes.
 *
 * Reads VLC codes with the IPU (libmpeg_decodeVlc table 0): 34 (stuffing) is skipped, 35 (escape) adds 33. A code of 0 is
 * an error unless it is MPEG-2 stuffing (11 bits of 15, flushed with libmpeg_skipBits): it reports "Invalid
 * macroblock_address_increment code(0x%08x)", sets the fault flag +0x11C and returns 1.
 *
 * @param decState  the decoder's internal state
 * @return the address increment
 */
int libmpeg_decodeAddressIncrement(int *decState) {
    int sum = 0;
    int vlcCode;
    int peekBits;
    int again;
    do {
        vlcCode = lx5_6F78(decState, 0);
        switch ((unsigned)vlcCode) {
        case 35:
            again = 1;
            sum += 33;
            break;
        case 34:
            again = 1;
            break;
        case 0:
            peekBits = lx5_70F0(decState, 11);
            if (decState[2120 / 4] != 0 && peekBits == 15) {
                lx5_71F8(decState, 11);
                again = 1;
                break;
            }
            lx5_3470(decState, str_Invalid_macroblock_address_increment_cod, vlcCode);
            decState[284 / 4] = 1;
            return 1;
        default:
            sum += vlcCode;
            again = 0;
            break;
        }
    } while (again);
    return sum;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeSlices);
#ifdef SKIP_ASM
extern char str_Skip_to_the_next_picture[];
extern int libmpeg_decodeSlice(int *, int);
extern void libmpeg_waitIpuIdle(int);
extern int libmpeg_waitMacroblock(int *);
extern void libmpeg_finishMacroblock(int *, int);
extern void libmpeg_reportError(int *, int);
/**
 * Decodes all slices of the current picture.
 *
 * Clears the macroblock record selector (+0x810) and +0x814 and computes the picture's macroblock count (+0x12C times
 * +0x130, halved for field pictures), then decodes slices with libmpeg_decodeSlice while it returns 1 or 3. Waits for the IPU
 * and the last macroblock (libmpeg_waitMacroblock; a stop request gives 4, an error 2), waits for the to-scratchpad DMA,
 * finishes the last macroblock (libmpeg_finishMacroblock) when the picture decoded cleanly and reports "= Skip to the next picture
 * =" after results 1 or 2. Called by libmpeg_decodePictureSlices.
 *
 * @param decState  the decoder's internal state
 * @return 1 when the picture decoded completely, else 0
 */
int libmpeg_decodeSlices(int *decState) {
    int n;
    int sliceResult;
    decState[2064 / 4] = 0;
    n = decState[300 / 4] * decState[304 / 4];
    decState[2068 / 4] = 0;
    if (decState[372 / 4] != 3)
        n >>= 1;
    do {
        sliceResult = libmpeg_decodeSlice(decState, n);
    } while (sliceResult == 1 || sliceResult == 3);
    libmpeg_waitIpuIdle((int)decState);
    if (libmpeg_waitMacroblock(decState) == 0) {
        sliceResult = 4;
        if (decState[4256 / 4] != 0)
            decState[4256 / 4] = 0;
        else
            sliceResult = 2;
    }
    while ((*(volatile unsigned *)0x1000D400 >> 8) & 1)
        ;
    if (sliceResult == 0)
        libmpeg_finishMacroblock(decState, decState[2064 / 4] == 0);
    if ((unsigned)(sliceResult - 1) < 2)
        libmpeg_reportError(decState, (int)str_Skip_to_the_next_picture);
    return sliceResult == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", _sliceA0);
#ifdef SKIP_ASM
extern char D_00497F28[];
extern char D_00497F50[];
extern void libmpeg_reportErrorf_k() __asm__("libmpeg_reportErrorf");
extern int libmpeg_peekBits(int, int);
extern void libmpeg_skipBits(int, int);
extern void libmpeg_nextStartCode();
extern int libmpeg_decodeAddressIncrement(int *);
/**
 * Starts decoding an MPEG video slice: checks its start code and works out its first macroblock (Sony
 * libmpeg).
 *
 * Reads the 32-bit slice start code (0x101-0x1AF, else an error and return 2), the rest of the slice header
 * via libmpeg_readSliceHeaderRest and the first macroblock address increment, and sets the macroblock address to (extension
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
    libmpeg_nextStartCode();
    startCode = libmpeg_peekBits((int)self, 32);
    if ((unsigned)(startCode - 257) >= 175) {
        libmpeg_reportErrorf_k(self, D_00497F28, startCode);
        return 2;
    }
    libmpeg_skipBits((int)self, 32);
    rowExtension = libmpeg_readSliceHeaderRest(self);
    mbAddrInc = libmpeg_decodeAddressIncrement(self);
    *mbIncrement = mbAddrInc;
    if (self[284 / 4] != 0) {
        libmpeg_reportError(self, (int)D_00497F50);
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeSlice);
#ifdef SKIP_ASM
extern char str_Too_many_macroblocks_in_picture[];
extern int _sliceA0(int *, int, int *, int *, int *);
extern int libmpeg_waitMacroblock(int *);
extern int libmpeg_peekBits(int, int);
extern int libmpeg_decodeAddressIncrement(int *);
extern void libmpeg_reportError(int *, int);
extern int libmpeg_decodeMacroblock(int *, int *, int *, int *, int *, int *, int *);
extern int libmpeg_setupSkippedMacroblock(int *, int *, int *, int *, int *);
extern int func_00403E00_k() __asm__("func_00403E00");
extern void libmpeg_finishMacroblock(int *, int);
/**
 * Decodes one slice: its header, then each macroblock until the next start code.
 *
 * The slice header comes from _sliceA0 (which returns non-zero to stop). For each macroblock: waits for the IPU
 * (libmpeg_waitMacroblock), reads the next address increment (libmpeg_decodeAddressIncrement) when needed (23 zero bits ahead means a start code:
 * return 3), decodes a coded macroblock's modes and vectors (libmpeg_decodeMacroblock) or sets up a skipped one (libmpeg_setupSkippedMacroblock),
 * issues it (func_00403E00) and finishes the previous one (libmpeg_finishMacroblock), alternating the two macroblock records
 * (+0x810). Reports "Too many macroblocks in picture" past @p n.
 *
 * @param decState  the decoder's internal state
 * @param n  the number of macroblocks in the picture
 * @return 0 at the end of the picture, 1 or 3 to continue with the next slice, 2 on an error, 4 on a stop request
 */
int libmpeg_decodeSlice(int *decState, int n) {
    int mbAddress;
    int mbIncrement;
    int mbType;
    int motionType;
    int dctType;
    int pmv[8];
    int fieldSelect[4];
    int dmVector[4];
    int sliceResult;
    mbAddress = 0;
    mbIncrement = 0;
    sliceResult = _sliceA0(decState, n, &mbAddress, &mbIncrement, pmv);
    if (sliceResult != 0)
        return sliceResult;
    decState[284 / 4] = 0;
    for (;;) {
        if (mbAddress >= n)
            return 0;
        decState[(1740 + decState[2064 / 4] * 320) / 4] = 0;
        if (libmpeg_waitMacroblock(decState) == 0) {
            if (decState[4256 / 4] != 0) {
                decState[4256 / 4] = 0;
                return 4;
            }
            return 2;
        }
        if (mbIncrement == 0) {
            if (libmpeg_peekBits((int)decState, 23) == 0) {
                decState[284 / 4] = 0;
                return 3;
            }
            if (decState[284 / 4] != 0) {
                decState[284 / 4] = 0;
                return 3;
            }
            mbIncrement = libmpeg_decodeAddressIncrement(decState);
            if (decState[284 / 4] != 0) {
                decState[284 / 4] = 0;
                return 1;
            }
        }
        if (mbAddress >= n) {
            libmpeg_reportError(decState, (int)str_Too_many_macroblocks_in_picture);
            return 2;
        }
        if (mbIncrement == 1) {
            if (libmpeg_decodeMacroblock(decState, &mbType, &motionType, &dctType, pmv, fieldSelect, dmVector) == 0) {
                decState[284 / 4] = 0;
                return 1;
            }
        } else {
            if (libmpeg_setupSkippedMacroblock(decState, pmv, &motionType, fieldSelect, &mbType) == 0) {
                decState[284 / 4] = 0;
                return 2;
            }
        }
        if (func_00403E00_k(decState, mbAddress, mbIncrement, mbType, motionType, pmv, fieldSelect, dmVector) == 0) {
            decState[284 / 4] = 0;
            return 2;
        }
        if (mbAddress != 0)
            libmpeg_finishMacroblock(decState, decState[2064 / 4] ^ 1);
        mbAddress++;
        decState[2064 / 4] ^= 1;
        mbIncrement--;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_setupSkippedMacroblock);
#ifdef SKIP_ASM
extern char str_skiped_macroblock_in_I_picure_is_not_all[];
extern void libmpeg_reportError(int *, int);
/**
 * Sets up a skipped macroblock (MSSG mpeg2decode's skipped_macroblock).
 *
 * Marks the current macroblock record as skipped (+0x6CC) and +0x1B0; in P pictures resets the forward vectors, sets
 * the motion type to frame (2, frame pictures) or field (1, with both field selects set to the bottom-field flag) and
 * clears the intra bit of the macroblock type. A skipped macroblock in an I picture reports "skiped macroblock in I
 * picure is not allowed".
 *
 * @param decState  the decoder's internal state
 * @param pmv  the motion vectors (PMV)
 * @param motionType  receives the motion type
 * @param fieldSelect  the motion_vertical_field_select flags
 * @param mbType  the macroblock type (intra bit cleared)
 * @return 1, or 0 in an I picture
 */
int libmpeg_setupSkippedMacroblock(int *decState, int *pmv, int *motionType, int *fieldSelect, int *mbType) {
    int ok = 1;
    int bottomField;
    decState[decState[2064 / 4] * 320 / 4 + 1740 / 4] = 1;
    decState[432 / 4] = 1;
    if (decState[336 / 4] == 2) {
        pmv[5] = 0;
        pmv[4] = 0;
        pmv[1] = 0;
        pmv[0] = 0;
    }
    if (decState[372 / 4] == 3) {
        *motionType = 2;
    } else {
        *motionType = 1;
        bottomField = decState[372 / 4] == 2;
        fieldSelect[1] = bottomField;
        fieldSelect[0] = bottomField;
    }
    if (decState[336 / 4] == 1) {
        libmpeg_reportError(decState, (int)str_skiped_macroblock_in_I_picure_is_not_all);
        ok = 0;
    }
    *mbType &= ~1;
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeMacroblock);
#ifdef SKIP_ASM
/* MSSG mpeg2decode getpic.c decode_macroblock() with macroblock_modes() folded in (Sony libmpeg): p is the decoder
   state (0x11C Fault_Flag, 0x150 picture_coding_type, 0x174 picture_structure, 0x17C frame_pred_frame_dct,
   0x180 concealment_motion_vectors, 0x848 MPEG2_Flag); the block is decoded by an IPU BDEC command. */
extern char str_Invalid_macroblock_type_code_0[];
extern void libmpeg_reportError(int *, int);
extern int m65D0_6F78(int *, int) __asm__("libmpeg_decodeVlc");
extern int m65D0_72E8(int *, int) __asm__("libmpeg_readBits");
extern void m65D0_71F8(int *, int) __asm__("libmpeg_skipBits");
extern void m65D0_6B00(int *, int *, int *, int *, int, int, int, int, int, int, int) __asm__("libmpeg_decodeMotionVectors");
extern void m65D0_6CA0(int *, int *, int *, int, int, int, int, int) __asm__("libmpeg_decodeMotionVector");
extern void m65D0_6DF0(int *, unsigned) __asm__("libmpeg_issueIpuCommand");
extern void m65D0_6E20(int *) __asm__("libmpeg_waitIpuIdle");
extern void m65D0_8AF8(int, int) __asm__("libmpeg_startFromIpuDma");
/**
 * Decodes a macroblock's modes and motion vectors and starts the IPU block decode (MSSG decode_macroblock).
 *
 * Puts the picture coding type into IPU_CTRL, decodes macroblock_type (libmpeg_decodeVlc table 1; 0 reports "Invalid
 * macroblock_type code: 0"), the frame/field motion type, dct_type and quantiser scale (+0x1B4), then the forward and
 * backward vectors (libmpeg_decodeMotionVectors for MPEG-2, libmpeg_decodeMotionVector for MPEG-1). For coded blocks it clears the record's
 * 0x300-byte output (libmpeg_startFromIpuDma) and issues an IPU BDEC command; otherwise marks the record skipped. Resets the
 * vectors after intra macroblocks without concealment vectors and for P macroblocks without a forward vector.
 *
 * @param decState  the decoder's internal state
 * @param mbtype  receives the macroblock_type bits
 * @param motion_type  receives the motion type
 * @param dct_type  receives the DCT type
 * @param PMV  the motion vectors
 * @param mvfs  the motion_vertical_field_select flags
 * @param dmvector  receives the dual-prime differential vector
 * @return 1, or 0 on an error (fault flag +0x11C set)
 */
int libmpeg_decodeMacroblock(int *decState, int *mbtype, int *motion_type, int *dct_type, int *PMV, int *mvfs, int *dmvector) {
    int macroblock_type;
    int motion_vector_count, mv_format, dmv, mvscale;
    *(volatile unsigned int *)0x10002010 = (*(volatile unsigned int *)0x10002010 & 0xF8FFFFFF) | (decState[336 / 4] << 24);
    macroblock_type = m65D0_6F78(decState, 1);
    *mbtype = macroblock_type;
    if (macroblock_type == 0) {
        libmpeg_reportError(decState, (int)str_Invalid_macroblock_type_code_0);
        decState[284 / 4] = 1;
        return 0;
    }
    if (macroblock_type & 0xC) {
        if (decState[372 / 4] == 3 && decState[380 / 4])
            *motion_type = 2;
        else
            *motion_type = m65D0_72E8(decState, 2);
    } else if ((macroblock_type & 1) && decState[0x180 / 4]) {
        *motion_type = (decState[372 / 4] == 3) ? 2 : 1;
    }
    if (decState[372 / 4] == 3) {
        motion_vector_count = (*motion_type == 1) ? 2 : 1;
        mv_format = (*motion_type == 2);
    } else {
        motion_vector_count = (*motion_type == 2) ? 2 : 1;
        mv_format = 0;
    }
    dmv = (*motion_type == 3);
    mvscale = (mv_format == 0 && decState[372 / 4] == 3);
    *dct_type = (decState[372 / 4] == 3 && !decState[380 / 4] && (*mbtype & 3)) ? m65D0_72E8(decState, 1) : 0;

    if (*mbtype & 0x10)
        decState[436 / 4] = m65D0_72E8(decState, 5);

    if ((*mbtype & 8) || ((*mbtype & 1) && decState[0x180 / 4])) {
        if (decState[2120 / 4])
            m65D0_6B00(decState, PMV, dmvector, mvfs, 0, motion_vector_count, mv_format, decState[356 / 4] - 1, decState[360 / 4] - 1, dmv, mvscale);
        else
            m65D0_6CA0(decState, PMV, dmvector, decState[344 / 4] - 1, decState[344 / 4] - 1, 0, 0, decState[340 / 4]);
    }
    if (decState[284 / 4])
        return 0;

    if (*mbtype & 4) {
        if (decState[2120 / 4])
            m65D0_6B00(decState, PMV, dmvector, mvfs, 1, motion_vector_count, mv_format, decState[364 / 4] - 1, decState[368 / 4] - 1, 0, mvscale);
        else
            m65D0_6CA0(decState, PMV + 2, dmvector, decState[352 / 4] - 1, decState[352 / 4] - 1, 0, 0, decState[348 / 4]);
    }
    if (decState[284 / 4])
        return 0;

    if ((*mbtype & 1) && decState[0x180 / 4])
        m65D0_71F8(decState, 1);

    if (*mbtype & 3) {
        m65D0_8AF8(decState[decState[2064 / 4] * 320 / 4 + 1428 / 4], 0x300);
        m65D0_6E20(decState);
        m65D0_6DF0(decState, 0x20000000 | ((*mbtype & 1) << 27) | (decState[432 / 4] << 26) | (*dct_type << 25) | (decState[436 / 4] << 16));
    } else {
        decState[decState[2064 / 4] * 320 / 4 + 1740 / 4] = 1;
    }
    decState[432 / 4] = 0;
    if (decState[284 / 4])
        return 0;

    if (!(*mbtype & 1))
        decState[432 / 4] = 1;

    if ((*mbtype & 1) && !decState[0x180 / 4]) {
        PMV[0] = PMV[1] = PMV[4] = PMV[5] = 0;
        PMV[2] = PMV[3] = PMV[6] = PMV[7] = 0;
    }

    if (decState[336 / 4] == 2 && !(*mbtype & 9)) {
        PMV[0] = PMV[1] = PMV[4] = PMV[5] = 0;
        if (decState[372 / 4] == 3) {
            *motion_type = 2;
        } else {
            *motion_type = 1;
            mvfs[0] = (decState[372 / 4] == 2);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_applyMotionCode);
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
void libmpeg_applyMotionCode(int *pred, int r_size, int motion_code, int motion_residual, int full_pel_vector) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeMotionVectors);
#ifdef SKIP_ASM
extern int Q_72E8(int, int) __asm__("libmpeg_readBits");
extern void Q_6CA0(int, int *, int *, int, int, int, int, int) __asm__("libmpeg_decodeMotionVector");
/**
 * Decodes one or two MPEG-2 motion vectors with their field selects (MSSG motion_vectors).
 *
 * With one vector, reads its field select (unless frame prediction or dual prime) into both entries, decodes it
 * (libmpeg_decodeMotionVector) and copies it to the second vector; with two, reads each field select and decodes each vector.
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
void libmpeg_decodeMotionVectors(int mpegSys, int *x, int *out, int *fieldSelect, int idx, int mvCount, int k, int rSizeH, int rSizeV, int dualPrime, int mvScale) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeMotionVector);
#ifdef SKIP_ASM
extern int libmpeg_decodeVlc();
extern int libmpeg_readBits();
extern int libmpeg_decodeDualPrimeVector();
extern void libmpeg_applyMotionCode();
/**
 * Decodes one motion vector, horizontal then vertical (MSSG motion_vector).
 *
 * Each component is a motion code (libmpeg_decodeVlc table 2) plus a residual of r_size bits when both are non-zero,
 * applied with libmpeg_applyMotionCode; the vertical one is halved around the update when @p mvScale is set. For dual prime it
 * also reads each differential component (libmpeg_decodeDualPrimeVector).
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
void libmpeg_decodeMotionVector(int mpegSys, int *x, int *out, int rSizeH, int rSizeV, int dualPrime, int mvScale, int z) {
    int motionCode;
    int residual;
    motionCode = libmpeg_decodeVlc(mpegSys, 2);
    if (rSizeH != 0 && motionCode != 0)
        residual = libmpeg_readBits(mpegSys, rSizeH);
    else
        residual = 0;
    libmpeg_applyMotionCode(x, rSizeH, motionCode, residual, z);
    if (dualPrime != 0)
        out[0] = libmpeg_decodeDualPrimeVector(mpegSys);
    motionCode = libmpeg_decodeVlc(mpegSys, 2);
    if (rSizeV != 0 && motionCode != 0)
        residual = libmpeg_readBits(mpegSys, rSizeV);
    else
        residual = 0;
    if (mvScale != 0)
        x[1] = x[1] >> 1;
    libmpeg_applyMotionCode(x + 1, rSizeV, motionCode, residual, z);
    if (mvScale != 0)
        x[1] = x[1] * 2;
    if (dualPrime != 0)
        out[1] = libmpeg_decodeDualPrimeVector(mpegSys);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_issueIpuCommand);
#ifdef SKIP_ASM
/**
 * Issues an IPU command and records its table entry.
 *
 * Writes the command to IPU_CMD and stores libmpeg_ipuCmdTable[command >> 28] in +0x818, which the bit readers (libmpeg_peekBits
 * and its neighbours) check to know whether IPU_TOP still holds their bits.
 *
 * @param p  the decoder's internal state
 * @param a  the IPU command word
 */
void libmpeg_issueIpuCommand(int *p, unsigned a) {
    volatile int *r = (volatile int *)0x10002000;
    *r = a;
    p[518] = libmpeg_ipuCmdTable[a >> 28];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_waitIpuIdle);
#ifdef SKIP_ASM
extern int libmpeg_raiseNeedData(int *);
/**
 * Waits until the IPU is no longer busy (or has flagged an error).
 *
 * Spins while IPU_CTRL has BUSY set and ECD clear, raising the needs-data callback (libmpeg_raiseNeedData) every 5001 polls so
 * the input DMA can be refilled.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_waitIpuIdle(int mpegSys) {
    int n = 0;
    if ((*(volatile unsigned *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 5001) {
                libmpeg_raiseNeedData(*(int **)(mpegSys + 0x858));
                n = 0;
            }
        } while ((*(volatile unsigned *)0x10002010 & 0x80004000) == 0x80000000);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_waitIpuResult);
#ifdef SKIP_ASM
extern int libmpeg_raiseNeedData(int *);
/**
 * Waits for the running IPU command's result and returns IPU_CMD.
 *
 * Spins while IPU_CMD's BUSY bit (bit 63) is set and no error is flagged, raising the needs-data callback every 5001
 * polls.
 *
 * @param mpegSys  the decoder's internal state
 * @return the 64-bit IPU_CMD value
 */
long libmpeg_waitIpuResult(int mpegSys) {
    int n = 0;
    long cmdResult = *(volatile long *)0x10002000;
    if (cmdResult < 0 && !(*(volatile unsigned *)0x10002010 & 0x4000)) {
        do {
            if (n++ >= 5001) {
                libmpeg_raiseNeedData(*(int **)(mpegSys + 0x858));
                n = 0;
            }
            cmdResult = *(volatile long *)0x10002000;
        } while (cmdResult < 0 && !(*(volatile unsigned *)0x10002010 & 0x4000));
    }
    return cmdResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodeVlc);
#ifdef SKIP_ASM
extern int libmpeg_raiseNeedData(int *);
/**
 * Decodes one variable-length code with the IPU (VDEC) and refreshes the bit-buffer state.
 *
 * Waits for the IPU, issues VDEC with the table @p cmd (0 address increment, 1 macroblock type, 2 motion code, 3
 * dual-prime vector), waits for the result, then stores IPU_TOP (+0x838) and the bits left in the buffer (+0x83C). Sets
 * the fault flag +0x11C when the code is 0. Raises the needs-data callback every 5001 polls.
 *
 * @param decState  the decoder's internal state
 * @param cmd  the VLC table
 * @return the decoded value (low 16 bits, signed)
 */
int libmpeg_decodeVlc(int decState, int cmd) {
    int waitCount = 0;
    int n = 0;
    int w;
    long vdecResult;
    long topBits;
    int bitPos;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 5001) {
                libmpeg_raiseNeedData(*(int **)(decState + 0x858));
                n = 0;
            }
        } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
    }
    w = (cmd << 26) | 0x30000000;
    *(volatile int *)0x10002000 = w;
    *(int *)(decState + 0x818) = libmpeg_ipuCmdTable[w >> 28];
    vdecResult = *(volatile long *)0x10002000;
    if (vdecResult < 0) {
        do {
            if (waitCount++ >= 5001) {
                libmpeg_raiseNeedData(*(int **)(decState + 0x858));
                waitCount = 0;
            }
            vdecResult = *(volatile long *)0x10002000;
        } while (vdecResult < 0);
    }
    bitPos = *(volatile int *)0x10002020;
    topBits = *(long *)0x10002030;
    *(int *)(decState + 0x838) = (int)topBits;
    if (topBits < 0)
        *(int *)(decState + 0x83C) = -(bitPos & 0x1F) & 0x1F;
    else
        *(int *)(decState + 0x83C) = 32;
    *(int *)(decState + 0x11C) = (int)vdecResult == 0;
    return (short)vdecResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_peekBits);
#ifdef SKIP_ASM
extern int libmpeg_raiseNeedData(int *);
extern long libmpeg_waitIpuResult_k() __asm__("libmpeg_waitIpuResult");
/**
 * Peeks at the next @p n bits of the bitstream without consuming them.
 *
 * When the cached IPU_TOP is stale (+0x818) or has fewer than @p n bits left (+0x83C), waits for the IPU, issues FDEC 0
 * and caches the new top word with 32 bits. Returns its top @p n bits.
 *
 * @param decState  the decoder's internal state
 * @param n  the number of bits (1..32)
 * @return the bits
 */
int libmpeg_peekBits(int decState, int n) {
    int waitCount;
    int x;
    int *e;
    if (*(int *)(decState + 0x818) || *(int *)(decState + 0x83C) < n) {
        waitCount = 0;
        if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
            do {
                if (waitCount++ >= 5001) {
                    libmpeg_raiseNeedData(*(int **)(decState + 0x858));
                    waitCount = 0;
                }
            } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
        }
        *(volatile int *)0x10002000 = 0x40000000;
        *(int *)(decState + 0x818) = libmpeg_ipuCmdTable[4];
        x = (int)libmpeg_waitIpuResult_k(decState);
        *(int *)(decState + 0x838) = x;
        *(int *)(decState + 0x83C) = 32;
    }
    return (unsigned)*(int *)(decState + 0x838) >> (32 - n);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_skipBits);
#ifdef SKIP_ASM
extern int libmpeg_raiseNeedData(int *);
extern long libmpeg_waitIpuResult_k() __asm__("libmpeg_waitIpuResult");
/**
 * Skips @p bitCount bits of the bitstream (IPU FDEC with a bit step) and caches the next top word.
 *
 * Like the other bit readers it first waits for the IPU, raising the needs-data callback every 5001 polls. Records the
 * command's table entry in +0x818 and stores the new IPU_TOP in +0x838 with 32 bits available (+0x83C).
 *
 * @param decState  the decoder's internal state
 * @param bitCount  the number of bits to skip (0..32)
 */
void libmpeg_skipBits(int decState, int bitCount) {
    int n = 0;
    int w;
    int x;
    int *tableEntry;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (n++ >= 5001) {
                libmpeg_raiseNeedData(*(int **)(decState + 0x858));
                n = 0;
            }
        } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
    }
    w = bitCount | 0x40000000;
    *(volatile int *)0x10002000 = w;
    tableEntry = &libmpeg_ipuCmdTable[(unsigned)w >> 28];
    *(int *)(decState + 0x818) = *tableEntry;
    x = (int)libmpeg_waitIpuResult_k(decState, tableEntry);
    *(int *)(decState + 0x838) = x;
    *(int *)(decState + 0x83C) = 32;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_readBits);
#ifdef SKIP_ASM
extern int libmpeg_raiseNeedData(int *);
extern long libmpeg_waitIpuResult_k() __asm__("libmpeg_waitIpuResult");
/**
 * Reads and consumes the next @p n bits of the bitstream.
 *
 * Like the other bit readers it first waits for the IPU, raising the needs-data callback every 5001 polls. Refreshes
 * the cached top word when it is stale or too short (FDEC 0), takes its top @p n bits, then advances the IPU by @p n
 * bits (FDEC n) and caches the next top word.
 *
 * @param decState  the decoder's internal state
 * @param n  the number of bits (1..32)
 * @return the bits read
 */
int libmpeg_readBits(int decState, int n) {
    int waitCount = 0;
    int w;
    unsigned bits;
    int x;
    int *tableEntry;
    if ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (waitCount++ >= 5001) {
                libmpeg_raiseNeedData(*(int **)(decState + 0x858));
                waitCount = 0;
            }
        } while ((*(volatile int *)0x10002010 & 0x80004000) == 0x80000000);
    }
    if (*(int *)(decState + 0x818) || *(int *)(decState + 0x83C) < n) {
        *(volatile int *)0x10002000 = 0x40000000;
        *(int *)(decState + 0x818) = libmpeg_ipuCmdTable[4];
        x = (int)libmpeg_waitIpuResult_k(decState);
        *(int *)(decState + 0x838) = x;
    }
    bits = (unsigned)*(int *)(decState + 0x838) >> (32 - n);
    *(int *)(decState + 0x83C) = 32;
    w = n | 0x40000000;
    *(volatile int *)0x10002000 = w;
    tableEntry = &libmpeg_ipuCmdTable[(unsigned)w >> 28];
    *(int *)(decState + 0x818) = *tableEntry;
    x = (int)libmpeg_waitIpuResult_k(decState);
    *(int *)(decState + 0x838) = x;
    return bits;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_nextStartCode);
#ifdef SKIP_ASM
extern void libmpeg_skipBits(int, int);
extern int libmpeg_peekBits(int, int);
extern void libmpeg_waitIpuIdle(int);
/**
 * Moves the bitstream to the next start code (MSSG next_start_code).
 *
 * Waits for the IPU, skips to a byte boundary (IPU_BP & 7), then skips a byte at a time until the next 24 bits are the
 * start code prefix 0x000001.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_nextStartCode(int mpegSys) {
    int n;
    libmpeg_waitIpuIdle(mpegSys);
    n = -(*(volatile int *)0x10002020 & 7) & 7;
    if (n)
        libmpeg_skipBits(mpegSys, n);
    while (libmpeg_peekBits(mpegSys, 0x18) != 1)
        libmpeg_skipBits(mpegSys, 8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_readSliceHeaderRest);
#ifdef SKIP_ASM
extern void libmpeg_skipBits(int, int);
extern int libmpeg_readBits(int, int);
extern void libmpeg_skipExtraInfo(int);
/**
 * Reads the rest of an MPEG slice header after its start code.
 *
 * Stores quantiser_scale_code in +0x1B4; when the extra bit is set skips intra_slice and the next 7 bits and the
 * extra_information_slice bytes (libmpeg_skipExtraInfo). Called by _sliceA0.
 *
 * @param mpegSys  the decoder's internal state
 * @return 0 (the slice vertical position extension, never read here)
 */
int libmpeg_readSliceHeaderRest(int *mpegSys) {
    mpegSys[109] = libmpeg_readBits((int)mpegSys, 5);
    if (libmpeg_readBits((int)mpegSys, 1)) {
        libmpeg_readBits((int)mpegSys, 1);
        libmpeg_skipBits((int)mpegSys, 7);
        libmpeg_skipExtraInfo((int)mpegSys);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_findNextPicture);
#ifdef SKIP_ASM
struct B7520 { int op; long l1; long l2; };
struct S_7520 { char pad[0x150]; int f150; char pad2[0x6D4]; long f828; long f830; char pad3[0x20]; int f858; };
extern void s7_7438(struct S_7520 *) __asm__("libmpeg_nextStartCode");
extern int s7_72E8(struct S_7520 *, int) __asm__("libmpeg_readBits");
extern void s7_3620(struct S_7520 *) __asm__("libmpeg_parseSequenceHeader");
extern void s7_7AD0(struct S_7520 *) __asm__("libmpeg_parseGopHeader");
extern void s7_7630(struct S_7520 *) __asm__("libmpeg_parsePictureHeader");
extern void s7_2C30(int, struct B7520 *) __asm__("libmpeg_dispatchCallback");
/**
 * Reads stream headers until the next picture and returns its coding type.
 *
 * Loops: next start code (libmpeg_nextStartCode), then by the 32-bit code: 0x1B3 sequence header (libmpeg_parseSequenceHeader), 0x1B8 group
 * of pictures (libmpeg_parseGopHeader), 0x1B7 sequence end (returns 0), 0x100 picture: parses its header (libmpeg_parsePictureHeader), raises
 * callback 5 with two 64-bit stamps preset to -1 and stores what comes back in +0x828 / +0x830, and returns the picture
 * coding type (+0x150). Called by the decode loop libmpeg_decodeUntilPicture and libmpeg_decodeFieldPair.
 *
 * @param decState  the decoder's internal state
 * @return the picture coding type (1 I, 2 P, 3 B, 4 D), or 0 at the sequence end
 */
int libmpeg_findNextPicture(struct S_7520 *decState) {
    struct B7520 stampEvent;
    for (;;) {
        s7_7438(decState);
        switch ((unsigned)s7_72E8(decState, 0x20)) {
        case 435:
            s7_3620(decState);
            break;
        case 440:
            s7_7AD0(decState);
            break;
        case 439:
            return 0;
        case 0x100:
            s7_7630(decState);
            stampEvent.op = 5;
            stampEvent.l1 = -1;
            stampEvent.l2 = -1;
            s7_2C30(decState->f858, &stampEvent);
            decState->f830 = stampEvent.l2;
            decState->f828 = stampEvent.l1;
            return decState->f150;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parsePictureHeader);
#ifdef SKIP_ASM
extern int libmpeg_readBits(int, int);
extern void libmpeg_skipExtraInfo(int);
extern void libmpeg_readExtensions(int);
extern void libmpeg_numberPicture(int, int);
/**
 * Parses an MPEG picture header.
 *
 * Reads temporal_reference (10 bits), picture_coding_type (+0x150) and vbv_delay; for P and B pictures the forward
 * full_pel / f_code (+0x154, +0x158), for B pictures the backward ones (+0x15C, +0x160); skips the extra information
 * (libmpeg_skipExtraInfo), reads extensions and user data (libmpeg_readExtensions) and numbers the picture from its temporal reference
 * (libmpeg_numberPicture).
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_parsePictureHeader(int *mpegSys) {
    int temporalRef = libmpeg_readBits((int)mpegSys, 10);
    mpegSys[336 / 4] = libmpeg_readBits((int)mpegSys, 3);
    libmpeg_readBits((int)mpegSys, 16);
    if ((unsigned)(mpegSys[336 / 4] - 2) < 2) {
        mpegSys[340 / 4] = libmpeg_readBits((int)mpegSys, 1);
        mpegSys[344 / 4] = libmpeg_readBits((int)mpegSys, 3);
    }
    if (mpegSys[336 / 4] == 3) {
        mpegSys[348 / 4] = libmpeg_readBits((int)mpegSys, 1);
        mpegSys[352 / 4] = libmpeg_readBits((int)mpegSys, 3);
    }
    libmpeg_skipExtraInfo((int)mpegSys);
    libmpeg_readExtensions((int)mpegSys);
    libmpeg_numberPicture((int)mpegSys, temporalRef);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_readExtensions);
#ifdef SKIP_ASM
struct PT44 { int v[11]; };
extern char libmpeg_extensionHandlerTable[];
extern int libmpeg_peekBits(int, int);
extern void libmpeg_skipBits(int, int);
extern int libmpeg_readBits();
/**
 * Reads the extension and user-data blocks that follow a header (MSSG extension_and_user_data).
 *
 * After moving to the next start code, loops while it is 0x1B5 (extension) or 0x1B2 (user data): an extension's 4-bit
 * id selects its handler from a copy of the table libmpeg_extensionHandlerTable (ids above 10 use entry 0); user data is skipped.
 *
 * @param decState  the decoder's internal state
 */
void libmpeg_readExtensions(int decState) {
    struct PT44 handlers = *(struct PT44 *)libmpeg_extensionHandlerTable;
    unsigned i;
    int startCode;
    libmpeg_nextStartCode(decState);
    while ((startCode = libmpeg_peekBits(decState, 32)) == 437 || startCode == 434) {
        if (startCode == 437) {
            libmpeg_skipBits(decState, 32);
            i = libmpeg_readBits(decState, 4);
            i = i > 10 ? 0 : i;
            ((void (*)(int))handlers.v[i])(decState);
            libmpeg_nextStartCode(decState);
        } else {
            libmpeg_skipBits(decState, 32);
            libmpeg_nextStartCode(decState);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parsePictureCodingExtension);
#ifdef SKIP_ASM
extern int Q_72E8(int, int) __asm__("libmpeg_readBits");
/**
 * Parses an MPEG-2 picture coding extension (extension table libmpeg_extensionHandlerTable, id 8).
 *
 * Reads the four f_codes (+0x164..+0x170), intra_dc_precision into IPU_CTRL bits 16-17, picture_structure (+0x174; the
 * first one is also kept in +0xD4), top_field_first (+0x178), frame_pred_frame_dct (+0x17C), concealment_motion_vectors
 * (+0x180), q_scale_type, intra_vlc_format and alternate_scan into IPU_CTRL bits 22, 21 and 20, repeat_first_field
 * (+0x184), progressive_frame (+0x188), and skips the composite display fields.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_parsePictureCodingExtension(int *mpegSys) {
    mpegSys[356 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[360 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[364 / 4] = Q_72E8((int)mpegSys, 4);
    mpegSys[368 / 4] = Q_72E8((int)mpegSys, 4);
    {
        int headerField = Q_72E8((int)mpegSys, 2);
        *(volatile unsigned *)0x10002010 = (*(volatile unsigned *)0x10002010 & 0xFFFCFFFF) | (headerField << 16);
    }
    {
        int headerField = Q_72E8((int)mpegSys, 2);
        mpegSys[372 / 4] = headerField;
        if (mpegSys[212 / 4] == 0)
            mpegSys[212 / 4] = headerField;
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_skipExtraInfo);
#ifdef SKIP_ASM
extern void libmpeg_skipBits(int, int);
extern int libmpeg_readBits(int, int);
/**
 * Skips extra-information bytes: while the next bit is 1, skips the 8 bits after it.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_skipExtraInfo(int mpegSys) {
    while (libmpeg_readBits(mpegSys, 1))
        libmpeg_skipBits(mpegSys, 8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_numberPicture);
#ifdef SKIP_ASM
struct S407A58 { char p0[0x150]; int f150; char p1[0x58]; int f1AC; char p2[0x69C]; int f84C; int f850; int f854; };
int f_407A58(struct S407A58 *p, int n) __asm__("libmpeg_numberPicture");
/**
 * Numbers a picture from its temporal reference and returns the highest number seen in the GOP.
 *
 * The picture number (+0x1AC) is the GOP's base (+0x84C) plus @p n; the running maximum is kept in +0x850. For non-B
 * pictures with a non-zero reference it clears +0x854 and remembers the reference; a negative reference with +0x854
 * clear would add 0x400, which cannot happen for the 10-bit value the caller reads (dead branch).
 *
 * @param decState  the decoder's internal state
 * @param n  the temporal reference
 * @return the highest picture number so far
 */
int f_407A58(struct S407A58 *decState, int n) {
    int refWrapped = 0;
    int anchorRef = 0;
    int x;
    if (decState->f150 != 3) {
        if (n != 0) {
            if (n < 0)
                refWrapped = decState->f854 == 0;
            decState->f854 = 0;
            anchorRef = n;
        }
    }
    decState->f1AC = decState->f84C + n;
    if (refWrapped && anchorRef >= n)
        decState->f1AC += 0x400;
    x = decState->f850;
    if (x < decState->f1AC)
        x = decState->f1AC;
    decState->f850 = x;
    return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parseGopHeader);
#ifdef SKIP_ASM
extern int libmpeg_readBits(int, int);
extern void libmpeg_readExtensions();
/**
 * Parses an MPEG group-of-pictures header.
 *
 * Clears +0xE8, starts the GOP's picture numbering after the highest number so far (+0x84C = +0x850 + 1) and sets
 * +0x854; skips the time code, keeps closed_gop (+0x1A4) and broken_link (+0x1A8), then reads extensions and user data
 * (libmpeg_readExtensions).
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_parseGopHeader(int *mpegSys) {
    mpegSys[232 / 4] = 0;
    mpegSys[2124 / 4] = mpegSys[2128 / 4] + 1;
    mpegSys[2132 / 4] = 1;
    libmpeg_readBits((int)mpegSys, 1);
    libmpeg_readBits((int)mpegSys, 5);
    libmpeg_readBits((int)mpegSys, 6);
    libmpeg_readBits((int)mpegSys, 1);
    libmpeg_readBits((int)mpegSys, 6);
    libmpeg_readBits((int)mpegSys, 6);
    mpegSys[420 / 4] = libmpeg_readBits((int)mpegSys, 1);
    mpegSys[424 / 4] = libmpeg_readBits((int)mpegSys, 1);
    libmpeg_readExtensions(mpegSys);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parseQuantMatrixExtension);
#ifdef SKIP_ASM
extern int libmpeg_readBits(int, int);
extern void libmpeg_waitIpuIdle(int);
extern void libmpeg_reportError(int *, int);
extern char D_00498010[];
extern char D_00498038[];
/**
 * Parses an MPEG-2 quant matrix extension (extension table libmpeg_extensionHandlerTable, id 3).
 *
 * For each of the intra and non-intra load flags (stored in +0x840 / +0x844) has the IPU read the matrix from the
 * bitstream (SETIQ 0x50000000 / 0x58000000); a chroma matrix flag reports "load_chroma_intra_quantizer_matrix == 1" or
 * the non-intra one.
 *
 * @param decState  the decoder's internal state
 */
void libmpeg_parseQuantMatrixExtension(int *decState) {
    if ((decState[2112 / 4] = libmpeg_readBits((int)decState, 1)) != 0) {
        libmpeg_waitIpuIdle((int)decState);
        libmpeg_issueIpuCommand(decState, 0x50000000);
        libmpeg_waitIpuIdle((int)decState);
    }
    if ((decState[2116 / 4] = libmpeg_readBits((int)decState, 1)) != 0) {
        libmpeg_waitIpuIdle((int)decState);
        libmpeg_issueIpuCommand(decState, 0x58000000);
        libmpeg_waitIpuIdle((int)decState);
    }
    if (libmpeg_readBits((int)decState, 1) != 0)
        libmpeg_reportError(decState, (int)D_00498010);
    if (libmpeg_readBits((int)decState, 1) != 0)
        libmpeg_reportError(decState, (int)D_00498038);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_parsePictureDisplayExtension);
#ifdef SKIP_ASM
struct S_7C38 { char pad[0x13C]; int f13C; char pad2[0x34]; int f174; int f178; char pad3[8]; int f184; int pad188; int f18C[3]; int f198[3]; };
extern int sc_72E8(struct S_7C38 *, int) __asm__("libmpeg_readBits");
/**
 * Parses an MPEG-2 picture display extension (extension table libmpeg_extensionHandlerTable, id 7).
 *
 * Reads one to three frame centre offsets (horizontal into +0x18C[i], vertical into +0x198[i]); the count follows
 * progressive_sequence, repeat_first_field, top_field_first and the picture structure, as in MSSG mpeg2decode.
 *
 * @param decState  the decoder's internal state
 */
void libmpeg_parsePictureDisplayExtension(struct S_7C38 *decState) {
    int n;
    int i;
    if (decState->f13C) {
        if (decState->f184 == 0) n = 1;
        else n = decState->f178 ? 3 : 2;
    } else {
        if (decState->f174 != 3) n = 1;
        else n = decState->f184 ? 3 : 2;
    }
    for (i = 0; i < n; i++) {
        decState->f18C[i] = sc_72E8(decState, 16);
        sc_72E8(decState, 1);
        decState->f198[i] = sc_72E8(decState, 16);
        sc_72E8(decState, 1);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_skipCopyrightExtension);
#ifdef SKIP_ASM
extern int libmpeg_readBits(int, int);
/**
 * Skips an MPEG-2 copyright extension (extension table libmpeg_extensionHandlerTable, id 4): all its fields are read and dropped.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_skipCopyrightExtension(int mpegSys) {
    libmpeg_readBits(mpegSys, 1);
    libmpeg_readBits(mpegSys, 8);
    libmpeg_readBits(mpegSys, 1);
    libmpeg_readBits(mpegSys, 7);
    libmpeg_readBits(mpegSys, 1);
    libmpeg_readBits(mpegSys, 20);
    libmpeg_readBits(mpegSys, 1);
    libmpeg_readBits(mpegSys, 22);
    libmpeg_readBits(mpegSys, 1);
    libmpeg_readBits(mpegSys, 22);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_decodePictureSlices);
#ifdef SKIP_ASM
extern char D_00498068[];
extern char D_00498088[];
extern void libmpeg_reportError();
extern int libmpeg_decodeSlices(int *);
/**
 * Decodes the current picture's slices and marks its picture record decoded.
 *
 * A frame picture while a field is still pending reports "odd number of field pictures" and clears +0x120. The record
 * is the frame one (+0x1C0) or the top / bottom field one (+0x1D0 / +0x1E0) by picture structure; an unknown structure
 * reports "unknown picture sutructure". When libmpeg_decodeSlices succeeds, word 10 of the record is set.
 *
 * @param decState  the decoder's internal state
 * @return libmpeg_decodeSlices's result (non-zero when decoded)
 */
int libmpeg_decodePictureSlices(int *decState) {
    int *picRecord;
    int decoded;
    if (decState[372 / 4] == 3 && decState[288 / 4] != 0) {
        libmpeg_reportError(decState, D_00498068);
        decState[288 / 4] = 0;
    }
    switch (decState[372 / 4]) {
    case 3:
        picRecord = (int *)decState[0x1C0 / 4];
        break;
    case 1:
        picRecord = (int *)decState[464 / 4];
        break;
    case 2:
        picRecord = (int *)decState[0x1E0 / 4];
        break;
    default:
        picRecord = (int *)decState[0x1C0 / 4];
        libmpeg_reportError(decState, D_00498088);
        break;
    }
    decoded = libmpeg_decodeSlices(decState);
    if (decoded != 0)
        picRecord[40 / 4] = 1;
    return decoded;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_outputDuePicture);
#ifdef SKIP_ASM
extern void libmpeg_outputFramePicture(int *, int, int);
extern void libmpeg_outputFieldPair(int *, int, int, int);
/**
 * Outputs the picture due for display after a decode, then advances the queued time stamp state.
 *
 * When @p flag is set: frame pictures output the backward reference (B picture) or the forward one with libmpeg_outputFramePicture;
 * field pictures output a field pair with libmpeg_outputFieldPair. A queued time stamp state of 1 becomes 2 (see libmpeg_queueTimeStamp).
 *
 * @param mpegSys  the decoder's internal state
 * @param n  the picture counter (output as n - 1)
 * @param flag  non-zero to output a picture
 */
void libmpeg_outputDuePicture(int *mpegSys, int n, int flag) {
    int firstRec, secondRec;
    if (flag) {
        if (mpegSys[372 / 4] == 3) {
            if (mpegSys[336 / 4] == 3)
                firstRec = mpegSys[452 / 4];
            else
                firstRec = mpegSys[440 / 4];
            libmpeg_outputFramePicture(mpegSys, firstRec, n - 1);
        } else {
            if (mpegSys[336 / 4] == 3) {
                firstRec = mpegSys[468 / 4];
                secondRec = mpegSys[484 / 4];
            } else {
                firstRec = mpegSys[456 / 4];
                secondRec = mpegSys[472 / 4];
            }
            libmpeg_outputFieldPair(mpegSys, firstRec, secondRec, n - 1);
        }
    }
    if (mpegSys[0xF8 / 4] == 1)
        mpegSys[0xF8 / 4] = 2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_selectPictureRecord);
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
 * @param decState  the decoder's internal state
 * @param secondField  non-zero for the second field of a pair
 * @return 1 when the needed reference pictures are present, else 0
 */
int libmpeg_selectPictureRecord(struct M7F20 *decState, int secondField) {
    int lim = 4;
    struct F7F20 *x;
    struct F7F20 *curRec = 0;
    int refsReady = 0;
    if (decState->f174 == 3)
        lim = 2;
    if (decState->f150 == 3) {
        decState->c[0].cur = decState->c[0].next;
        decState->c[1].cur = decState->c[1].next;
        decState->c[2].cur = decState->c[2].next;
        if (decState->fA0 + decState->fA4 >= lim) {
            decState->fE8 = 0;
            decState->f1A8 = 0;
            decState->f1A4 = 0;
        }
        if ((decState->fE8 != 0 || decState->f1A8 != 0) && decState->f1A4 == 0) {
            decState->c[0].ref0->state = 0;
            decState->c[1].ref0->state = 0;
            decState->c[2].ref0->state = 0;
        }
        decState->fE8 = 0;
        decState->f1A8 = 0;
        if (decState->f174 == 3) {
            if (decState->c[0].ref0->state == 1 || decState->f1A4 != 0)
                if (decState->c[0].ref1->state == 1)
                    refsReady = 1;
        } else {
            if ((decState->c[1].ref0->state == 1 && decState->c[2].ref0->state == 1) || decState->f1A4 != 0) {
                if (decState->c[1].ref1->state == 1)
                    if (decState->c[2].ref1->state == 1)
                        refsReady = 1;
            }
        }
    } else {
        if (secondField == 0) {
            struct F7F20 *swapTmp;
            swapTmp = decState->c[0].ref0; decState->c[0].ref0 = decState->c[0].ref1; decState->c[0].ref1 = swapTmp;
            swapTmp = decState->c[1].ref0; decState->c[1].ref0 = decState->c[1].ref1; decState->c[1].ref1 = swapTmp;
            swapTmp = decState->c[2].ref0; decState->c[2].ref0 = decState->c[2].ref1; decState->c[2].ref1 = swapTmp;
        }
        decState->c[0].cur = decState->c[0].ref1;
        decState->c[1].cur = decState->c[1].ref1;
        decState->c[2].cur = decState->c[2].ref1;
        if (decState->f174 == 3) {
            if (decState->f150 != 2 || decState->c[0].ref0->state == 1)
                refsReady = 1;
        } else {
            x = decState->f174 == 1 ? decState->c[2].ref1 : decState->c[1].ref1;
            if (decState->f150 != 2 || (secondField != 0 && x->state == 1) || (decState->c[1].ref0->state == 1 && decState->c[2].ref0->state == 1))
                refsReady = 1;
        }
    }
    switch (decState->f174) {
    case 3:
        curRec = decState->c[0].cur;
        break;
    case 1:
        curRec = decState->c[1].cur;
        break;
    case 2:
        curRec = decState->c[2].cur;
        break;
    }
    curRec->state = 0;
    curRec->t18 = decState->t828;
    curRec->f2C = decState->f150;
    curRec->t20 = decState->t830;
    curRec->f30 = decState->f174;
    curRec->f34 = decState->f13C;
    curRec->f38 = decState->f188;
    curRec->f3C = decState->f178;
    curRec->f40 = decState->f184;
    curRec->f44 = decState->f18C;
    curRec->f48 = decState->f190;
    curRec->f4C = decState->f194;
    curRec->f50 = decState->f198;
    curRec->f54 = decState->f19C;
    curRec->f58 = decState->f1A0;
    curRec->f5C = decState->f148;
    curRec->f60 = decState->f14C;
    return refsReady;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_checkOutputSize);
#ifdef SKIP_ASM
extern int sprintf(char *, const char *, ...);
extern void libmpeg_reportError(int *, int);
extern char str_Too_small_buffer_size_for_pct_dxpct_d_pi[];
/**
 * Checks that the caller's output buffer is big enough for the decoded picture.
 *
 * With a RAW8 width/height given (+0xDC / +0xE0) both must cover the picture; otherwise the macroblock count (+0xE4)
 * must cover its width times height in macroblocks. Reports "Too small buffer size for %dx%d picture" when it does not.
 *
 * @param decState  the decoder's internal state
 * @param picSize  the picture-size record (see libmpeg_setPictureSize)
 * @return 1 when it fits, else 0
 */
int libmpeg_checkOutputSize(int *decState, int *picSize) {
    char buf[256];
    int fits;
    int n = decState[0xE0 / 4];
    if (n != 0)
        fits = decState[220 / 4] >= picSize[1] && n >= picSize[2];
    else
        fits = decState[228 / 4] >= picSize[3] * picSize[4];
    if (fits == 0) {
        sprintf(buf, str_Too_small_buffer_size_for_pct_dxpct_d_pi, picSize[1], picSize[2]);
        libmpeg_reportError(decState, (int)buf);
    }
    return fits;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_copyRaw8Picture);
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
 * @param decState  the decoder's internal state
 * @param picRecord  the picture record
 */
void libmpeg_copyRaw8Picture(struct S8278 *decState, unsigned *picRecord) {
    int n;
    unsigned src = picRecord[0] & 0x0FFFFFFF;
    unsigned dst = decState->fD8 & 0x0FFFFFFF;
    int size, stride, qwc;
    int i, j, st;
    unsigned dstRow;
    if (decState->f174 == 3 || decState->fE0 == 0) {
        size = picRecord[4] * 0x180;
        qwc = size >> 4;
        if (decState->fE0 != 0)
            stride = (decState->fE0 >> 4) * 0x180;
        else
            stride = size;
        n = 1;
    } else {
        stride = (decState->fE0 >> 4) * 0xC0;
        size = ((int)picRecord[4] >> 1) * 0x180;
        n = 2;
        qwc = size >> 4;
    }
    for (j = 0; j < n; j++) {
        dstRow = dst;
        for (i = 0; i < (int)picRecord[3]; i++) {
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
            *(volatile int *)0x1000D010 = dstRow;
            *(volatile int *)0x1000D020 = qwc;
            *(volatile int *)0x1000D000 = 0x100;
            if (st)
                EIntr();
            while (*(volatile int *)0x1000D000 & 0x100)
                ;
            while (*(volatile int *)0x1000D020 != 0)
                ;
            dstRow += stride;
            src += size;
        }
        dst += decState->fE4 * 0xC0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_markPictureOutput);
#ifdef SKIP_ASM
/**
 * Marks a picture as output: the decode loop then returns.
 *
 * Sets the reference state word 2 to 2 (copying +0x118 to +0xAC the first time) and the output flag +0x820.
 *
 * @param mpegSys  the decoder's internal state
 */
void libmpeg_markPictureOutput(int *mpegSys) {
    if (mpegSys[2] != 2) {
        mpegSys[2] = 2;
        mpegSys[43] = mpegSys[70];
    }
    mpegSys[520] = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_computeTimeStamps);
#ifdef SKIP_ASM
// libmpeg_computeTimeStamps 97.98 (sweep: schedule). IPU/MPEG picture-timestamp routine: a=decoder state, b=picture header, o1/o2/o3 -> r->f10/f18/f20 in caller libmpeg_outputFramePicture. __muldi3=__muldi3 (long*long). Only diff: 'move $4,$18' (4th muldi3 arg) placed before dsra32 in ours, after it in target. Tried named locals, bitfield union, w/z temps, sweep 716 variants, permuter 4min. Chain order f34|f38|f3c|f40|f30|f2c found by permutation search.
struct S85 { char pad0[0x70]; int f70; int pad74; long f78; int f80; int pad84; long f88; int f90; char pad94[0x5C]; long fF0; int fF8; };
struct B85 { char pad0[0x18]; long f18; long f20; int pad28; int f2c; int f30; int f34; int f38; int f3c; int f40; };
extern long mul85(long, long) __asm__("__muldi3");
/**
 * Computes a picture's presentation and decode time stamps and its display flags.
 *
 * The presentation stamp is the picture's own (+0x18) unless interpolation is on (+0x70) and it has none: then the
 * previous stamp (+0x80) plus half the step (+0x78) times the frame-period multiplier (+0x88), with the odd half-units
 * alternated through +0x90. A queued stamp in state 2 (+0xF0 / +0xF8) overrides it and is cleared. The flags word packs
 * progressive_sequence, progressive_frame, top_field_first, repeat_first_field, picture structure and type. The C still
 * differs from retail in one instruction's position (97.98%).
 *
 * @param decState  the decoder's internal state
 * @param picRecord  the picture record
 * @param ptsOut  receives the presentation stamp
 * @param dtsOut  receives the decode stamp
 * @param flagsOut  receives the flags word
 * @return the flags word
 */
long libmpeg_computeTimeStamps(struct S85 *decState, struct B85 *picRecord, long *ptsOut, long *dtsOut, long *flagsOut) {
    long picFlags;
    long picPts;
    int x;
    int roundCount;
    int lastPts;
    if (decState->f70 != 0) {
        picPts = picRecord->f18;
        if (picPts >= 0) {
            *ptsOut = picPts;
        } else {
            lastPts = decState->f80;
            if (lastPts < 0) {
                *ptsOut = picPts;
            } else {
                long bothOdd;
                int y;
                x = decState->f88;

                bothOdd = mul85(x & 1, decState->f78 & 1);
                roundCount = decState->f90;
                y = mul85(bothOdd, roundCount & 1);
                *ptsOut = lastPts + ((int)(mul85(decState->f78, x) >> 1) + y);
                if (mul85(x & 1, decState->f78 & 1))
                    decState->f90 = roundCount + 1;
            }
        }
    } else {
        *ptsOut = picRecord->f18;
    }
    if (decState->fF8 == 2 && decState->fF0 >= 0) {
        *ptsOut = decState->fF0;
        decState->fF8 = 0;
        decState->fF0 = -1;
    }
    *dtsOut = picRecord->f20;
    picFlags = (long)picRecord->f34 << 8 | (long)picRecord->f38 << 7 | (long)picRecord->f3c << 6 | (long)picRecord->f40 << 5 | (long)picRecord->f30 << 3 | (long)picRecord->f2c;
    *flagsOut = picFlags;
    return picFlags;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_outputFramePicture);
#ifdef SKIP_ASM
struct S86C0 { char pad0[0x80]; int f80; int pad84; unsigned long f88; char pad90[0x20]; int fB0; int fB4; int fB8; int fBC; int fC0; int fC4; int fC8; int fCC; int fD0; char padD4[0x784]; int f858; };
struct B86C0 { char pad0[0x28]; int f28; char pad2C[0x18]; int f44; int f48; int f4C; int f50; int f54; int f58; int f5C; int f60; };
struct R86C0 { char pad[0x10]; int f10; int f14; int f18; int f1C; long f20; };
extern unsigned D_004532E8[];
extern void q86_8530(struct S86C0 *, struct B86C0 *, void *, void *, void *) __asm__("libmpeg_computeTimeStamps");
extern int q86_81D8(struct S86C0 *, struct B86C0 *) __asm__("libmpeg_checkOutputSize");
extern int q86_9088(struct S86C0 *, struct B86C0 *) __asm__("libmpeg_convertToRgb32");
extern int q86_8278(struct S86C0 *, struct B86C0 *) __asm__("libmpeg_copyRaw8Picture");
extern void q86_8508(struct S86C0 *) __asm__("libmpeg_markPictureOutput");
void d86C0(struct S86C0 *a, struct B86C0 *b, int c) __asm__("libmpeg_outputFramePicture");
/**
 * Outputs a decoded frame picture to the caller: time stamps, display fields, then the pixels.
 *
 * Writes the stamps and flags into the handle (+0x10 / +0x18 / +0x20, libmpeg_computeTimeStamps), keeps the presentation stamp
 * (+0x80) and the frame-period multiplier from D_004532E8 (+0x88), copies the record's display fields to +0xB4..+0xD0,
 * and when the buffer is big enough (libmpeg_checkOutputSize) and the record is decoded converts it (libmpeg_convertToRgb32, colour
 * conversion on) or copies it (libmpeg_copyRaw8Picture), then marks the output (libmpeg_markPictureOutput).
 *
 * @param decState  the decoder's internal state
 * @param picRecord  the picture record
 * @param picNumber  the picture counter (unused)
 */
void d86C0(struct S86C0 *decState, struct B86C0 *picRecord, int picNumber) {
    struct R86C0 *handle = (struct R86C0 *)decState->f858;
    int flagBits;
    q86_8530(decState, picRecord, &handle->f10, &handle->f18, &handle->f20);
    handle = (struct R86C0 *)decState->f858;
    decState->f80 = handle->f10;
    flagBits = handle->f20 >> 5;
    decState->f88 = D_004532E8[flagBits & 0xF];
    decState->fCC = picRecord->f5C;
    decState->fD0 = picRecord->f60;
    decState->fB4 = picRecord->f44;
    decState->fB8 = picRecord->f48;
    decState->fBC = picRecord->f4C;
    decState->fC0 = picRecord->f50;
    decState->fC4 = picRecord->f54;
    decState->fC8 = picRecord->f58;
    if (q86_81D8(decState, picRecord) && picRecord->f28 == 1) {
        if (decState->fB0 != 0) q86_9088(decState, picRecord);
        else q86_8278(decState, picRecord);
        q86_8508(decState);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_outputFieldPair);
#ifdef SKIP_ASM
// FAKEMATCH: field +0x88 declared as `union { unsigned long l; int i[2]; }` (alias set 0; `i` is never used) so the two 64-bit
// stores don't force the reloads a plain field gets; plain form (`unsigned long f88`) 87.07%. Byte-identical and behaves like the
// plain form; landed marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest
// spelling matches.
struct S87D0 { char pad0[0x80]; int f80; int pad84; union { unsigned long l; int i[2]; } f88; char pad90[0x20]; int fB0; int fB4; int fB8; int fBC; int fC0; int fC4; int fC8; int fCC; int fD0; char padD4[0xA0]; int f174; char pad178[0x6E0]; int f858; };
struct B87D0 { char pad0[0x10]; int f10; char pad14[0x14]; int f28; char pad2C[0x18]; int f44; int f48; int f4C; int f50; int f54; int f58; int f5C; int f60; };
extern void q87_8530(struct S87D0 *, struct B87D0 *, void *, void *, void *) __asm__("libmpeg_computeTimeStamps");
struct R87D0 { char pad[0x10]; int f10; int f14; int f18; int f1C; long f20; int f28; int f2C; int f30; int f34; long f38; };
extern int q87_81D8(struct S87D0 *, struct B87D0 *) __asm__("libmpeg_checkOutputSize");
extern int q87_9088(struct S87D0 *, struct B87D0 *) __asm__("libmpeg_convertToRgb32");
extern int q87_8278(struct S87D0 *, struct B87D0 *) __asm__("libmpeg_copyRaw8Picture");
extern void q87_8508(struct S87D0 *) __asm__("libmpeg_markPictureOutput");
void d87D0(struct S87D0 *a, struct B87D0 *b, struct B87D0 *c) __asm__("libmpeg_outputFieldPair");
/**
 * Outputs a decoded field pair to the caller: both fields' time stamps, then the pixels.
 *
 * Takes the fields in display order (top field first sets 0x40 in both flag words): stamps for the first go to handle
 * +0x10..+0x20, for the second to +0x28..+0x38. Copies the display fields to +0xB4..+0xD0 and, when the buffer is big
 * enough and both fields are decoded, converts or copies the pair with the record's row count doubled for the call,
 * then marks the output.
 *
 * @param decState  the decoder's internal state
 * @param fieldRec1  the first field's record
 * @param fieldRec2  the second field's record
 */
void d87D0(struct S87D0 *decState, struct B87D0 *fieldRec1, struct B87D0 *fieldRec2) {
    struct B87D0 *firstField, *secondField;
    struct R87D0 *mpegHandle, *mpegHandle2, *mpegHandle3;
    int topFirstFlag = 0;
    if (decState->f174 == 2) {
        firstField = fieldRec1;
        secondField = fieldRec2;
        topFirstFlag = 0x40;
    } else {
        firstField = fieldRec2;
        secondField = fieldRec1;
    }
    mpegHandle = (struct R87D0 *)decState->f858;
    q87_8530(decState, firstField, &mpegHandle->f10, &mpegHandle->f18, &mpegHandle->f20);
    mpegHandle2 = (struct R87D0 *)decState->f858;
    decState->f80 = mpegHandle2->f10;
    decState->f88.l = 1;
    q87_8530(decState, secondField, &mpegHandle2->f28, &mpegHandle2->f30, &mpegHandle2->f38);
    mpegHandle3 = (struct R87D0 *)decState->f858;
    decState->f80 = mpegHandle3->f28;
    decState->f88.l = 1;
    mpegHandle3->f20 |= topFirstFlag;
    mpegHandle3->f38 |= topFirstFlag;
    decState->fCC = firstField->f5C;
    decState->fD0 = firstField->f60;
    decState->fB4 = firstField->f44;
    decState->fB8 = secondField->f48;
    decState->fC0 = firstField->f50;
    decState->fC4 = secondField->f54;
    if (q87_81D8(decState, fieldRec1) && fieldRec1->f28 == 1 && fieldRec2->f28 == 1) {
        fieldRec1->f10 *= 2;
        if (decState->fB0 != 0)
            q87_9088(decState, fieldRec1);
        else
            q87_8278(decState, fieldRec1);
        fieldRec1->f10 >>= 1;
        q87_8508(decState);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_startPredictionDma);
#ifdef SKIP_ASM
struct Ent_8988 { int f0; int f4; unsigned a[4]; unsigned b[69]; int cnt; int pad[4]; };
struct P_8988 { char pad[0x590]; struct Ent_8988 e[2]; int idx; };
extern char D_00526FC0[];
extern int q8988_C078() __asm__("DIntr");
extern void q8988_C0C0(void) __asm__("EIntr");
void q8988(struct P_8988 *p) __asm__("libmpeg_startPredictionDma");
/**
 * Starts the scratchpad DMA (channel 9, chain mode) that brings a macroblock's prediction blocks in.
 *
 * Builds two 48-quadword ref tags per entry of the current macroblock record (+0x810 selects it) in the uncached tag
 * area D_00526FC0, the last one ending the chain, then with interrupts off sets the scratchpad address from the record
 * and starts the channel. Called by func_00403E00.
 *
 * @param decState  the decoder's internal state
 */
void q8988(struct P_8988 *decState) {
    int i = 0;
    unsigned long long *tag = (unsigned long long *)(((unsigned)D_00526FC0 & 0x0FFFFFFF) | 0x20000000);
    int n = decState->e[decState->idx].cnt;
    int intrEnabled;
    if (n > 0) {
        do {
            unsigned firstAddr = decState->e[decState->idx].a[i] & 0x0FFFFFFF;
            unsigned secondAddr = decState->e[decState->idx].b[i] & 0x0FFFFFFF;
            int last = (i == n - 1) ? 0 : 3;
            i++;
            tag[0] = ((unsigned long long)firstAddr << 32) | 0x30000030;
            tag[2] = ((unsigned long long)secondAddr << 32) | ((unsigned long long)last << 28) | 0x30;
            tag += 4;
        } while (i < n);
    }
    intrEnabled = q8988_C078();
    __asm__ volatile("sync");
    *(volatile int *)0x1000D480 = decState->e[decState->idx].f0;
    *(volatile int *)0x1000D430 = (int)D_00526FC0;
    *(volatile int *)0x1000D420 = 0;
    *(volatile int *)0x1000D400 = 261;
    if (intrEnabled)
        q8988_C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_startFromIpuDma);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
/**
 * Starts the from-IPU DMA (channel 3) into scratchpad memory.
 *
 * Sets D3_MADR to the address with the scratchpad bit (0x80000000), D3_QWC to @p byteCount / 16 and starts the channel,
 * interrupts off. libmpeg_decodeMacroblock uses it to receive a macroblock's 0x300 bytes of decoded blocks.
 *
 * @param dstAddr  the scratchpad destination
 * @param byteCount  the number of bytes
 */
void libmpeg_startFromIpuDma(unsigned dstAddr, int byteCount) {
    int intrState = DIntr();
    *(volatile unsigned *)0x1000B010 = (dstAddr & 0xFFFFFFF) | 0x80000000;
    *(volatile int *)0x1000B020 = byteCount >> 4;
    *(volatile int *)0x1000B000 = 0x100;
    if (intrState)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_convertMacroblocks);
#ifdef SKIP_ASM
extern int q8B80_C078() __asm__("DIntr");
extern void q8B80_C0C0(void) __asm__("EIntr");
extern void q8B80_6DF0(int *, int) __asm__("libmpeg_issueIpuCommand");
extern int q8B80_2C30(int *, int *) __asm__("libmpeg_dispatchCallback");
void q8B80(int *p, unsigned addr, int n) __asm__("libmpeg_convertMacroblocks");
/**
 * Colour-converts up to 1023 macroblocks with the IPU (CSC) straight into the output buffer.
 *
 * Waits for the IPU, starts channel 3 (from IPU) to @p addr for 64 quadwords per macroblock, issues CSC with the count,
 * raises callback 4 and waits for the DMA and the IPU to finish. Called by libmpeg_convertToRgb32.
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_onCscChunkDone);
#ifdef SKIP_ASM
struct S408CA8 { volatile int cnt; int stop; int rest; int addr; int n; };
/**
 * DMA channel 3 interrupt handler of a long CSC conversion: queues the next 1023-macroblock chunk.
 *
 * Acknowledges the channel's interrupt (D_STAT bit 3) and counts the chunk. If the channel is still busy it sets the
 * stop flag. Otherwise it starts the next chunk (0xFFC0 quadwords and CSC 0x3FF) or the last one (the remaining
 * macroblocks). Returns with sync and ei. libmpeg_convertLargePicture installs it.
 *
 * @param channel  the DMA channel (unused)
 * @param xferState  the transfer state
 * @return 0
 */
int libmpeg_onCscChunkDone(int channel, struct S408CA8 *xferState) {
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
INCLUDE_ASM("seg/seg_2FE810", libmpeg_convertLargePicture);
#ifdef SKIP_ASM
extern char str_CSC_handler_error[];
extern int q8DF0_AddH(int, void *, int, void *) __asm__("AddDmacHandler2");
extern int q8DF0_RemH(int, int) __asm__("RemoveDmacHandler");
extern void q8DF0_En(int) __asm__("EnableDmac");
extern void q8DF0_Dis(int) __asm__("DisableDmac");
extern int q8DF0_C078() __asm__("DIntr");
extern void q8DF0_C0C0() __asm__("EIntr");
extern int q8DF0_2C30(int *, int *) __asm__("libmpeg_dispatchCallback");
extern void q8DF0_34A8(int *, int) __asm__("libmpeg_reportError");
extern int libmpeg_onCscChunkDone();
void q8DF0(int *p, int addr, int size) __asm__("libmpeg_convertLargePicture");
/**
 * Colour-converts a large picture (1024 macroblocks or more) with the IPU, 1023 at a time.
 *
 * Installs libmpeg_onCscChunkDone as the channel 3 interrupt handler (AddDmacHandler2) with a transfer state on the stack, starts
 * the first chunk (0xFFC0 quadwords, CSC 0x3FF), raises callback 4 and spins until every chunk is done. A stop reports
 * "CSC handler error". Then waits for the IPU, disables and removes the handler.
 *
 * @param decState  the decoder's internal state
 * @param addr  the RGB32 output
 * @param size  the number of macroblocks
 */
void q8DF0(int *decState, int addr, int size) {

    int sp[8];
    struct S408CA8 xferState; /* libmpeg_onCscChunkDone's state: it counts the chunks and sets stop */
    int handlerId;
    int i;
    xferState.n = size / 1023 + 1;
    xferState.rest = size;
    xferState.addr = (addr + 0xFFC00) & 0x0FFFFFFF;
    xferState.cnt = xferState.stop = 0;
    while (*(volatile int *)0x10002010 < 0)
        ;
    handlerId = q8DF0_AddH(3, libmpeg_onCscChunkDone, 0, &xferState);
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
    q8DF0_2C30(*(int **)((char *)decState + 0x858), sp);
    while (xferState.cnt < xferState.n)
        ;
    if (xferState.stop)
        q8DF0_34A8(decState, (int)str_CSC_handler_error);
    while (*(volatile int *)0x10002010 < 0)
        ;
    q8DF0_Dis(3);
    q8DF0_RemH(3, handlerId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_onIpuInputChunkDone);
#ifdef SKIP_ASM
struct S408FB0 { unsigned f0; int f4; };
/**
 * DMA channel 4 interrupt handler: feeds the next chunk of a long transfer to the IPU.
 *
 * Acknowledges the channel's interrupt (D_STAT bit 4). With quadwords left it starts the next chunk of at most 0xFFFF
 * from the saved address and advances it; returns with sync and ei. libmpeg_convertToRgb32 installs it.
 *
 * @param channel  the DMA channel (unused)
 * @param xferState  the transfer state: quadwords left, next address
 * @return 1 when nothing was left, else 0
 */
int libmpeg_onIpuInputChunkDone(int channel, struct S408FB0 *xferState) {
    unsigned n;
    *(volatile int *)0x1000E010 = 0x10;
    n = xferState->f0;
    if (n == 0) {
        SYNC_0810();
        EI_0810();
        return 1;
    }
    if (n > 0xFFFF) {
        *(volatile int *)0x1000B410 = xferState->f4;
        *(volatile int *)0x1000B420 = 0xFFFF;
        *(volatile int *)0x1000B400 = 257;
        xferState->f4 = (xferState->f4 + 0xFFFF0) & 0xFFFFFFF;
        xferState->f0 = xferState->f0 - 0xFFFF;
    } else {
        *(volatile int *)0x1000B410 = xferState->f4;
        *(volatile int *)0x1000B420 = xferState->f0;
        *(volatile int *)0x1000B400 = 257;
        xferState->f0 = 0;
    }
    SYNC_0810();
    EI_0810();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmpeg_convertToRgb32);
#ifdef SKIP_ASM
struct L9 { int ev; int pad[7]; unsigned cnt; unsigned addr; };
extern int E_2C30(int, void *) __asm__("libmpeg_dispatchCallback");
extern void E_6DF0(char *, int) __asm__("libmpeg_issueIpuCommand");
extern int E_AddH(int, void *, int, void *) __asm__("AddDmacHandler2");
extern int E_RemH(int, int) __asm__("RemoveDmacHandler");
extern void E_49B8(int) __asm__("EnableDmac");
extern void E_4950(int) __asm__("DisableDmac");
extern int E_C078() __asm__("DIntr");
extern void E_C0C0() __asm__("EIntr");
extern void E_8B80(char *, int, int) __asm__("libmpeg_convertMacroblocks");
extern void E_8DF0(char *, int, int) __asm__("libmpeg_convertLargePicture");
extern void E_8FB0(void) __asm__("libmpeg_onIpuInputChunkDone");
void q9088(char *d, unsigned *b) __asm__("libmpeg_convertToRgb32");
/**
 * Converts a decoded picture to RGB32 with the IPU and writes it to the output buffer.
 *
 * Raises callback 2 (stop DMA), resets the IPU after an error and clears it (BCLR), then feeds the picture's 24
 * quadwords per macroblock to the IPU on channel 4 (in 0xFFFF-quadword chunks through the handler libmpeg_onIpuInputChunkDone when
 * larger) while libmpeg_convertMacroblocks or, from 1024 macroblocks, libmpeg_convertLargePicture runs CSC into +0xD8. Raises callback 3 (restart
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
INCLUDE_ASM("seg/seg_2FE810", libipu_setFromIpuChcr);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
/**
 * Writes D3_CHCR (from-IPU DMA) with the DMA controller held and interrupts off.
 *
 * @param chcrValue  the new CHCR value
 */
void libipu_setFromIpuChcr(int chcrValue) {
    int intrState = DIntr();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B000 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libipu_setToIpuChcr);
#ifdef SKIP_ASM
extern int DIntr();
extern void EIntr(void);
/**
 * Writes D4_CHCR (to-IPU DMA) with the DMA controller held and interrupts off.
 *
 * @param chcrValue  the new CHCR value
 */
void libipu_setToIpuChcr(int chcrValue) {
    int intrState = DIntr();
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    if (intrState)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceIpuStopDMA);
#ifdef SKIP_ASM
extern void q9400_9388(int) __asm__("libipu_setToIpuChcr");
extern void q9400_9310(int) __asm__("libipu_setFromIpuChcr");
void q9400(int *p) __asm__("sceIpuStopDMA");
/**
 * Pauses the IPU's DMA channels and saves their state and the IPU's (the stop-DMA callback's work).
 *
 * Stops channel 4 and saves its MADR, TADR, QWC and CHCR; waits until the IPU output FIFO is empty; stops channel 3 and
 * saves its MADR, QWC and CHCR, then IPU_BP and IPU_CTRL. sceIpuRestartDMA undoes it.
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
INCLUDE_ASM("seg/seg_2FE810", sceIpuRestartDMA);
#ifdef SKIP_ASM
extern void q94E8_9388(int) __asm__("libipu_setToIpuChcr");
extern void q94E8_9310(int) __asm__("libipu_setFromIpuChcr");
void q94E8(int *p) __asm__("sceIpuRestartDMA");
/**
 * Restores and restarts the IPU DMA saved by sceIpuStopDMA.
 *
 * Restarts channel 3 when it had data left. Clears the IPU with BCLR at the saved bit pointer, then moves channel 4's
 * address back by the quadwords that were still in the IPU's input FIFO (IPU_BP's FP and IFC), adds them to its count
 * and restarts it with the saved tag address.
 *
 * @param savedRegs  the save block
 */
void q94E8(int *savedRegs) {
    unsigned ipuBp = savedRegs[7];
    int bitPointer = ipuBp & 0x7F;
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
    *(volatile int *)0x10002000 = bitPointer;
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
INCLUDE_ASM("seg/seg_2FE810", sceIpuSync);
#ifdef SKIP_ASM
/**
 * Waits for the IPU to finish, or reports whether it is busy.
 *
 * Mode 0 spins until the BUSY bit (31) of IPU_CTRL (0x10002010) clears and returns 0; mode 1 returns that bit; any
 * other mode returns 0. The only caller, libmpeg_stopIpu, passes a second argument that the body ignores.
 *
 * @param mode  0 to wait, 1 to poll
 * @return 1 while the IPU is busy (mode 1), else 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// NOTE: volatile = the IPU_CTRL hardware register (0x10002010), polled until BUSY clears; without it the poll is hoisted
// (strip test 85.54). The R5900 short-loop nops in the loop come from the assembler, not from the source.
int sceIpuSync(int mode) {
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
INCLUDE_ASM("seg/seg_2FE810", libipu_initToIpuChcr);
#ifdef SKIP_ASM
extern int DIntr(int);
extern void EIntr(void);
/**
 * Writes D4_CHCR (the to-IPU DMA channel) with the DMA controller held.
 *
 * Calls DIntr (interrupts off; its result is ignored), sets D_ENABLEW to D_ENABLER | 0x10000, writes @p chcrValue to
 * 0x1000B400, clears the hold bit again and calls EIntr, which turns interrupts back on even if they were off before.
 * Same work as libipu_setToIpuChcr; sceIpuInit calls it with 1.
 *
 * @param chcrValue  the new CHCR value
 */
void libipu_initToIpuChcr(int chcrValue) {
    DIntr(chcrValue);
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
    *(volatile int *)0x1000B400 = chcrValue;
    *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
    EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceIpuInit);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef int i128_9708 __attribute__((mode(TI)));
extern volatile i128_9708 D_00453340[];
extern i128_9708 D_00453390[];
extern void libipu_initToIpuChcr(int);
#define WAIT_9708() while (*(volatile int *)0x10002010 < 0)
/**
 * Resets the IPU and loads its default quantiser matrices, colour table and thresholds.
 *
 * Sets D4_CHCR to 1 (libipu_initToIpuChcr), resets the IPU (IPU_CTRL 0x40000000) and clears its input (BCLR), waiting for BUSY
 * to clear after each command. It then pushes eight quadwords into the IPU input FIFO (0x10007010): D_00453340[0..3]
 * and four copies of D_00453340[4], loaded by SETIQ 0x50000000 (intra) and 0x58000000 (non-intra); then
 * D_00453390[0..1] for SETVQ (0x60000000), and sets both thresholds to 0 (SETTH 0x90000000). It ends with another reset
 * and BCLR. sceMpegInit calls it before MPEG decoding starts.
 */
// FAKEMATCH: `extern volatile` view of the IPU quantiser table D_00453340 (only this function reads it), so its four
// copies of entry 4 are reloaded for each FIFO write as in retail instead of being CSEd; plain form 97.11%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
void sceIpuInit(void) {
    libipu_initToIpuChcr(1);
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
INCLUDE_ASM("seg/seg_2FE810", sceMcInit);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004980F0[];
extern char D_00498108[];
extern char D_00498130[];
extern int Q_sema(int *) __asm__("CreateSema");
extern void Q_sig(int) __asm__("SignalSema");
extern void Q_log(char *) __asm__("scePrintf");
extern int Q_A360(int, int, int) __asm__("sceMcSync");
extern void Q_6408(int) __asm__("func_00426408");
/**
 * Initialises the memory-card RPC client: creates its semaphores, binds the server and checks versions.
 *
 * On the first call creates the lock semaphore libmc_lockSema (count 1) and the sleep semaphore libmc_sleepSema (count 0). Waits
 * for any pending call (sceMcSync in mode 0), takes the lock and calls func_00426408(0), then binds SIF RPC server
 * 0x80000400 into libmc_client, retrying with a 0x100000-step busy wait until the server is set; a bind error prints
 * "bind error libmc" and hangs forever. It runs call 0xFE synchronously (12 result bytes into libmc_rpcResult) and releases
 * the lock.
 *
 * A failed call clears the server and returns its error - 100. Result word 1 below 522 prints "libmc: too old release
 * of mcserv.irx" and word 2 below 526 the same for mcman.irx; both clear the server. systemInit calls it.
 *
 * @return result word 0 on success, the RPC error - 100, -120 (old mcserv) or -121 (old mcman)
 */
// NOTE: the nops in retail's two wait loops are the assembler's R5900 short-loop padding, so the loops are plain C.
int sceMcInit(void) {
    int semaParam[6];
    int rpcResult;
    int i;
    if (libmc_lockSema < 0) {
        semaParam[5] = 0;
        semaParam[2] = 1;
        semaParam[1] = 1;
        libmc_lockSema = Q_sema(semaParam);
        semaParam[1] = 1;
        semaParam[2] = 0;
        semaParam[5] = 0;
        libmc_sleepSema = Q_sema(semaParam);
    }
    Q_A360(0, 0, 0);
    WaitSema(libmc_lockSema);
    Q_6408(0);
    for (;;) {
        if (sceSifBindRpc(&libmc_client, 0x80000400, 0) < 0) {
            Q_log(D_004980F0);
            for (;;)
                ;
        }
        if (libmc_client.server != 0)
            break;
        i = 0x100000;
        do {
            i--;
        } while (i != 0);
    }
    rpcResult = sceSifCallRpc(&libmc_client, 0xFE, 0, &libmc_cmd, 48, libmc_rpcResult, 12, 0, 0);
    Q_sig(libmc_lockSema);
    if (rpcResult < 0) {
        libmc_client.server = 0;
        return rpcResult - 100;
    }
    if (libmc_rpcResult[1] < 522) {
        Q_log(D_00498108);
        libmc_client.server = 0;
        return -120;
    }
    if (libmc_rpcResult[2] < 526) {
        Q_log(D_00498130);
        libmc_client.server = 0;
        return -121;
    }
    return libmc_rpcResult[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcEnd);
#ifdef SKIP_ASM
extern int DeleteSema(int);
/**
 * Deletes the memory-card client's semaphores.
 *
 * When the lock semaphore libmc_lockSema exists it deletes it, sets it to -1 and deletes the sleep semaphore libmc_sleepSema
 * (whose variable is not reset). The server binding in libmc_client is left as it is.
 *
 * @return 1
 */
int sceMcEnd(void) {
    if (libmc_lockSema >= 0) {
        DeleteSema(libmc_lockSema);
        libmc_lockSema = -1;
        DeleteSema(libmc_sleepSema);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmc_getClientState);
#ifdef SKIP_ASM
/**
 * Returns the memory-card client block and the addresses of its result and pending-call words.
 *
 * Stores the address of the RPC result buffer libmc_rpcResult in *@p recvBufOut and of the pending-call code libmc_pendingCall in *@p pendingCmdOut, and
 * copies the lock semaphore libmc_lockSema into libmc_rpcResult[15]. No caller in the source.
 *
 * @param recvBufOut  receives the result buffer's address
 * @param pendingCmdOut  receives the pending-call word's address
 * @return &libmc_client, the client block
 */
sSifClient *libmc_getClientState(int *recvBufOut, int *pendingCmdOut) {
    *recvBufOut = (int)libmc_rpcResult;
    *pendingCmdOut = (int)&libmc_pendingCall;
    libmc_rpcResult[15] = libmc_lockSema;
    return &libmc_client;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcChangeThreadPriority);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Starts memory-card call 20 with one argument (asynchronous).
 *
 * Puts @p callArg into the origin field of the command block libmc_cmd. Fails with -100 when the library is not bound
 * (libmc_client.server is 0) and -200 when another call still holds the lock semaphore libmc_lockSema (PollSema). On success
 * it records 20 as the pending call (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish; when
 * sceSifCallRpc fails it releases the lock.
 *
 * @param callArg  the call's argument
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcChangeThreadPriority(int callArg) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.origin = callArg;
    rpcResult = sceSifCallRpc(&libmc_client, 20, 1, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 20;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcGetSlotMax);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Runs memory-card call 21 for a port synchronously and returns the server's result word.
 *
 * Puts @p cardPort into the port field of libmc_cmd and waits for the call (mode 0). Fails with -100 when the library is not
 * bound (libmc_client.server is 0) and -200 when another call still holds the lock semaphore libmc_lockSema (PollSema).
 * Releases the lock afterwards in both cases.
 *
 * @param cardPort  the memory-card port
 * @return libmc_rpcResult[0], or -100 / -200 / sceSifCallRpc's error
 */
int sceMcGetSlotMax(int cardPort) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.port = cardPort;
    rpcResult = sceSifCallRpc(&libmc_client, 21, 0, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult != 0) {
        SignalSema(libmc_lockSema);
        return rpcResult;
    }
    SignalSema(libmc_lockSema);
    return libmc_rpcResult[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcOpen);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
/**
 * Starts opening a file on a memory card (call 2, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into libmc_fileCmd.name (at most 1023
 * characters, always terminated). Fills port, slot and mode of the name block libmc_fileCmd and sends it (1044 bytes).
 * Fails with -100 when the library is not bound (libmc_client.server is 0) and -200 when another call still holds the
 * lock semaphore libmc_lockSema (PollSema). On success it records 2 as the pending call (libmc_pendingCall) and keeps the lock
 * until sceMcSync sees the call finish; when sceSifCallRpc fails it releases the lock. The descriptor arrives as
 * sceMcSync's result.
 *
 * cPS2MemCard's callers pass mode 515 (0x203) to write and 1 to read; sceMcMkdir passes 0x40.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param s_  the file path
 * @param openMode  the open mode bits
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
int sceMcOpen(int cardPort, int cardSlot, int s_, int openMode) {
    char *fileName = (char *)s_;
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    if (fileName == 0 || fileName[0] == 0) {
        SignalSema(libmc_lockSema);
        return -210;
    }
    strncpy(libmc_fileCmd.name, fileName, 0x3FF);
    libmc_fileCmd.port = cardPort;
    libmc_fileCmd.mode = openMode;
    libmc_fileCmd.slot = cardSlot;
    libmc_fileCmd.name[0x3FF] = 0;
    rpcResult = sceSifCallRpc(&libmc_client, 2, 1, &libmc_fileCmd, 1044, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 2;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcMkdir);
#ifdef SKIP_ASM
extern int sceMcOpen(int, int, int, int);
/**
 * Starts creating a directory on a memory card.
 *
 * Calls sceMcOpen with mode 0x40 and, when that started, changes the pending-call code libmc_pendingCall from 2 to 11 so
 * sceMcSync reports it as a directory creation. cPS2MemCard calls it to make the save directory.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param dirPath  the directory path
 * @return sceMcOpen's result
 */
int sceMcMkdir(int cardPort, int cardSlot, int dirPath) {
    int rpcResult = sceMcOpen(cardPort, cardSlot, dirPath, 0x40);
    if (rpcResult == 0) {
        libmc_pendingCall = 11;
    }
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcClose);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Starts closing a memory-card file (call 3, asynchronous).
 *
 * Puts @p fileDesc into the fd field of libmc_cmd. Fails with -100 when the library is not bound (libmc_client.server is 0) and
 * -200 when another call still holds the lock semaphore libmc_lockSema (PollSema). On success it records 3 as the pending
 * call (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish; when sceSifCallRpc fails it releases
 * the lock.
 *
 * @param fileDesc  the file descriptor
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcClose(int fileDesc) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.fd = fileDesc;
    rpcResult = sceSifCallRpc(&libmc_client, 3, 1, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 3;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcSeek);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Starts moving a memory-card file's position (call 4, asynchronous).
 *
 * Fills fd, offset and origin of libmc_cmd. Fails with -100 when the library is not bound (libmc_client.server is 0) and
 * -200 when another call still holds the lock semaphore libmc_lockSema (PollSema). On success it records 4 as the pending
 * call (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish; when sceSifCallRpc fails it releases
 * the lock.
 *
 * @param fileDesc  the file descriptor
 * @param seekOffset  the offset
 * @param seekOrigin  where the offset counts from
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcSeek(int fileDesc, int seekOffset, int seekOrigin) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.fd = fileDesc;
    libmc_cmd.offset = seekOffset;
    libmc_cmd.origin = seekOrigin;
    rpcResult = sceSifCallRpc(&libmc_client, 4, 1, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 4;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmc_onReadDone);
#ifdef SKIP_ASM
struct S0A000 { int n0; int n1; char *d0; char *d1; char b0[0x40]; char b1[0x40]; };
/**
 * End callback of the memory-card read: copies the unaligned head and tail bytes to the destination.
 *
 * Reads the 0xC0-byte block @p fixupBuf through the uncached mirror (| 0x20000000): two counts, two destinations and two
 * 64-byte buffers. Copies the first count of bytes from the first buffer to the first destination and the second count
 * from the second buffer to the second destination. sceMcRead registers it with libmc_callbackRecvBuffer.
 *
 * @param fixupBuf  the block the server filled (libmc_callbackRecvBuffer)
 */
void libmc_onReadDone(void *fixupBuf) {
    struct S0A000 *fixupInfo = (struct S0A000 *)((unsigned)fixupBuf | 0x20000000);
    char *destByte;
    int i;
    if (fixupInfo->n0 != 0) {
        destByte = fixupInfo->d0;
        for (i = 0; i < fixupInfo->n0; i++)
            *destByte++ = fixupInfo->b0[i];
    }
    if (fixupInfo->n1 != 0) {
        destByte = fixupInfo->d1;
        for (i = 0; i < fixupInfo->n1; i++)
            *destByte++ = fixupInfo->b1[i];
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcRead);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern void func_00426358(int, int);
/**
 * Starts reading from a memory-card file into a buffer (call 5, asynchronous).
 *
 * Fills fd, buffer, size and param (libmc_callbackRecvBuffer) of libmc_cmd and writes the data cache back over the buffer and the
 * 0xC0-byte block (func_00426358). libmc_onReadDone runs when the call ends and copies the bytes the server returns
 * separately. Fails with -100 when the library is not bound (libmc_client.server is 0) and -200 when another call still
 * holds the lock semaphore libmc_lockSema (PollSema). On success it records 5 as the pending call (libmc_pendingCall) and keeps
 * the lock until sceMcSync sees the call finish; when sceSifCallRpc fails it releases the lock. The byte count
 * arrives as sceMcSync's result.
 *
 * @param fileDesc  the file descriptor
 * @param readBuf  the destination buffer
 * @param readSize  the number of bytes
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcRead(int fileDesc, int readBuf, int readSize) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.fd = fileDesc;
    libmc_cmd.param = (int)libmc_callbackRecvBuffer;
    libmc_cmd.buffer = readBuf;
    libmc_cmd.size = readSize;
    func_00426358(readBuf, readSize);
    func_00426358((int)libmc_callbackRecvBuffer, 0xC0);
    rpcResult = sceSifCallRpc(&libmc_client, 5, 1, &libmc_cmd, 48, libmc_rpcResult, 4, libmc_onReadDone, libmc_callbackRecvBuffer);
    if (rpcResult == 0)
        libmc_pendingCall = 5;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcWrite);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern void FlushCache(int);
/**
 * Starts writing a buffer to a memory-card file (call 6, asynchronous).
 *
 * Up to 16 bytes travel entirely inside the command block (origin = @p n, no DMA). Longer data sends its first bytes up
 * to the next 16-byte boundary in the command block (origin) and the rest from there by DMA (buffer, size). Copies
 * those head bytes into libmc_cmd.data and flushes the cache (FlushCache(0)). Fails with -100 when the library is not
 * bound (libmc_client.server is 0) and -200 when another call still holds the lock semaphore libmc_lockSema (PollSema). On
 * success it records 6 as the pending call (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish;
 * when sceSifCallRpc fails it releases the lock. The byte count arrives as sceMcSync's result.
 *
 * @param fileDesc  the file descriptor
 * @param writeData  the data
 * @param n  the number of bytes
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcWrite(int fileDesc, unsigned char *writeData, int n) {
    int rpcResult;
    unsigned i;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.fd = fileDesc;
    if (n < 17) {
        libmc_cmd.origin = n;
        libmc_cmd.buffer = libmc_cmd.size = 0;
    } else {
        int headBase = (int)writeData - 16;
        int headBytes = (((unsigned)writeData - 1) & 0xFFFFFFF0u) - headBase;
        libmc_cmd.size = n - headBytes;
        libmc_cmd.origin = headBytes;
        libmc_cmd.buffer = (int)writeData + headBytes;
    }
    for (i = 0; i < libmc_cmd.origin; i++)
        libmc_cmd.data[i] = writeData[i];
    FlushCache(0);
    rpcResult = sceSifCallRpc(&libmc_client, 6, 1, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 6;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmc_onSleepAlarm);
#ifdef SKIP_ASM
extern void iSignalSema(int);
/**
 * Alarm handler that wakes the thread sleeping in libmc_sleepTicks.
 *
 * Signals @p sema from interrupt context (iSignalSema).
 *
 * @param alarmId  the alarm (unused)
 * @param ticks  the alarm's time (unused)
 * @param sema  the semaphore to signal
 */
void libmc_onSleepAlarm(int alarmId, int ticks, int sema) {
    iSignalSema(sema);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmc_sleepTicks);
#ifdef SKIP_ASM
extern void libmc_onSleepAlarm(int, int, int);
extern void SetAlarm(int, void (*)(int, int, int), int);
extern void WaitSema(int);
/**
 * Sleeps for a number of horizontal-blank ticks.
 *
 * Sets an alarm (SetAlarm) that runs libmc_onSleepAlarm with the sleep semaphore libmc_sleepSema, then waits on that semaphore.
 * sceMcSync sleeps 60 ticks between polls.
 *
 * @param alarmTicks  the number of ticks
 */
void libmc_sleepTicks(unsigned short alarmTicks) {
    SetAlarm(alarmTicks, libmc_onSleepAlarm, libmc_sleepSema);
    WaitSema(libmc_sleepSema);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcSync);
#ifdef SKIP_ASM
extern void SignalSema(int);
extern void sleep_A328(int) __asm__("libmc_sleepTicks");
/**
 * Waits for or polls the pending memory-card call and hands back its code and result.
 *
 * Returns -1 when no call is pending (libmc_pendingCall is 0). Otherwise checks the RPC (sceSifCheckStatRpc); in mode 0 it
 * polls until the call is done, sleeping 60 ticks between polls (libmc_sleepTicks). Stores the pending call's code in *@p
 * p when @p cmdOut is set. When the call is done it clears libmc_pendingCall, stores the result word libmc_rpcResult[0] in *@p resultOut (when
 * set) and releases the lock libmc_lockSema. Any mode other than 0 only polls.
 *
 * cPS2MemCard_Update and cPS2MemCard_CleanUp poll it; sceMcInit waits with it before re-initialising.
 *
 * @param syncMode  0 to wait, otherwise poll
 * @param cmdOut  receives the call's code, or 0
 * @param resultOut  receives the call's result, or 0
 * @return 1 when the call has finished, 0 while it runs, -1 when nothing is pending
 */
int sceMcSync(int syncMode, int *cmdOut, int *resultOut) {
    int callDone;
    if (libmc_pendingCall == 0)
        return -1;
    callDone = sceSifCheckStatRpc(&libmc_client);
    if (syncMode == 0 && callDone != 0) {
        while (sceSifCheckStatRpc(&libmc_client) != 0)
            sleep_A328(60);
        callDone = 0;
    }
    callDone = callDone == 0;
    if (cmdOut)
        *cmdOut = libmc_pendingCall;
    if (callDone) {
        libmc_pendingCall = 0;
        if (resultOut)
            *resultOut = libmc_rpcResult[0];
        SignalSema(libmc_lockSema);
    }
    return callDone;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmc_onGetInfoDone);
#ifdef SKIP_ASM
/**
 * End callback of sceMcGetInfo: copies the three card-information words to the caller's variables.
 *
 * Reads the server's block @p recvBlock through the uncached mirror (| 0x20000000) and stores word 0 in *libmc_getInfoTypeOut, word 1 in
 * *libmc_getInfoFreeOut and word 36 in *libmc_getInfoFormatOut, each only when that pointer is set.
 *
 * @param recvBlock  the block the server filled (libmc_callbackRecvBuffer)
 */
void libmc_onGetInfoDone(void *recvBlock) {
    int *infoWords = (int *)((unsigned)recvBlock | 0x20000000);
    if (libmc_getInfoTypeOut)
        *libmc_getInfoTypeOut = infoWords[0];
    if (libmc_getInfoFreeOut)
        *libmc_getInfoFreeOut = infoWords[1];
    if (libmc_getInfoFormatOut)
        *libmc_getInfoFormatOut = infoWords[36];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcGetInfo);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern void func_00426358(int, int);
/**
 * Starts reading a memory card's type, free space and format state (call 1, asynchronous).
 *
 * Fills port and slot of libmc_cmd and sets its origin, offset and size fields to 1 for each output pointer that is
 * set, so the server knows which values are wanted. Saves the pointers in libmc_getInfoTypeOut / libmc_getInfoFreeOut / libmc_getInfoFormatOut for
 * libmc_onGetInfoDone, which the call runs at its end, and writes the 0xC0-byte block libmc_callbackRecvBuffer back from the cache. Fails
 * with -100 when the library is not bound (libmc_client.server is 0) and -200 when another call still holds the lock
 * semaphore libmc_lockSema (PollSema). On success it records 1 as the pending call (libmc_pendingCall) and keeps the lock until
 * sceMcSync sees the call finish; when sceSifCallRpc fails it releases the lock.
 *
 * cPS2MemCard's cPS2MemCard_startGetInfo passes cPS2MemCard_mcInfoType, cPS2MemCard_mcInfoFree and cPS2MemCard_mcInfoFormat.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param typeOut  receives the card type, or 0
 * @param freeOut  receives the free space, or 0
 * @param formatOut  receives the format state, or 0
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcGetInfo(int cardPort, int cardSlot, int *typeOut, int *freeOut, int *formatOut) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.port = cardPort;
    libmc_cmd.slot = cardSlot;
    libmc_cmd.param = (int)libmc_callbackRecvBuffer;
    if (typeOut != 0)
        libmc_cmd.origin = 1;
    else
        libmc_cmd.origin = 0;
    if (freeOut != 0)
        libmc_cmd.offset = 1;
    else
        libmc_cmd.offset = 0;
    if (formatOut != 0)
        libmc_cmd.size = 1;
    else
        libmc_cmd.size = 0;
    libmc_getInfoTypeOut = typeOut;
    libmc_getInfoFreeOut = freeOut;
    libmc_getInfoFormatOut = formatOut;
    func_00426358((int)libmc_callbackRecvBuffer, 0xC0);
    rpcResult = sceSifCallRpc(&libmc_client, 1, 1, &libmc_cmd, 48, libmc_rpcResult, 4, libmc_onGetInfoDone, libmc_callbackRecvBuffer);
    if (rpcResult == 0)
        libmc_pendingCall = 1;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcUdCheckNewCard);
#ifdef SKIP_ASM
extern void sceMcUdCheckNewCard_log(char *) __asm__("scePrintf");
extern char str_sceMcUdCheckNewCard_RPC_faild[];
/**
 * Runs memory-card call 53 synchronously and returns the server's result word.
 *
 * Sends the command block libmc_cmd without arguments and without taking the lock semaphore or checking that the
 * server is bound. A failed call prints the message str_sceMcUdCheckNewCard_RPC_faild and returns -1. No caller in the source.
 *
 * @return libmc_rpcResult[0], or -1 when the call failed
 */
int sceMcUdCheckNewCard(void) {
    if (sceSifCallRpc(&libmc_client, 53, 0, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0)) {
        sceMcUdCheckNewCard_log(str_sceMcUdCheckNewCard_RPC_faild);
        return -1;
    }
    return libmc_rpcResult[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcGetDir);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
extern void func_00426358(int, int);
/**
 * Starts listing a memory-card directory into an entry table (call 13, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into libmc_fileCmd.name (at most 1023
 * characters, always terminated). Fills port, slot, mode, count (the maximum number of entries) and buffer (the table)
 * of libmc_fileCmd, and writes the cache back over the table's 64-byte entries when the count is not negative. Fails with
 * -100 when the library is not bound (libmc_client.server is 0) and -200 when another call still holds the lock semaphore
 * libmc_lockSema (PollSema). On success it records 13 as the pending call (libmc_pendingCall) and keeps the lock until
 * sceMcSync sees the call finish; when sceSifCallRpc fails it releases the lock. The number of entries arrives as
 * sceMcSync's result.
 *
 * cPS2MemCard's cPS2MemCard_issueGetDir lists up to 6 entries and cPS2MemCard_issueGetFileInfo one entry.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param dirPattern  the path or pattern
 * @param listMode  the listing mode
 * @param maxEntries  the maximum number of entries
 * @param entryTable  the entry table (64 bytes per entry)
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
int sceMcGetDir(int cardPort, int cardSlot, char *dirPattern, int listMode, int maxEntries, int entryTable) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    if (dirPattern == 0 || dirPattern[0] == 0) {
        SignalSema(libmc_lockSema);
        return -210;
    }
    libmc_fileCmd.port = cardPort;
    libmc_fileCmd.slot = cardSlot;
    libmc_fileCmd.mode = listMode;
    libmc_fileCmd.count = maxEntries;
    libmc_fileCmd.buffer = entryTable;
    strncpy(libmc_fileCmd.name, dirPattern, 0x3FF);
    libmc_fileCmd.name[0x3FF] = 0;
    if (maxEntries >= 0)
        func_00426358(entryTable, maxEntries << 6);
    rpcResult = sceSifCallRpc(&libmc_client, 13, 1, &libmc_fileCmd, 1044, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 13;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmc_onChdirDone);
#ifdef SKIP_ASM
extern unsigned strlen(char *);
extern int func_0041605C(int, int, int);
/**
 * End callback of sceMcChdir: copies the returned current directory to the caller's buffer.
 *
 * When @p dst is set it copies the string in libmc_chdirRecvBuffer (read through the uncached mirror; at most 1023 characters)
 * with func_0041605C and terminates it.
 *
 * @param dst  the caller's buffer, or 0
 */
void libmc_onChdirDone(void *dst) {
    char *src;
    int n;
    if (dst) {
        src = (char *)((unsigned)libmc_chdirRecvBuffer | 0x20000000);
        if (strlen(src) < 0x400)
            n = strlen(src);
        else
            n = 0x3FF;
        func_0041605C((int)dst, (unsigned)libmc_chdirRecvBuffer | 0x20000000, n);
        ((char *)dst)[n] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcChdir);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
extern void func_00426358(int, int);
/**
 * Starts changing a memory card's current directory (call 12, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into libmc_fileCmd.name (at most 1023
 * characters, always terminated). Fills port and slot of libmc_fileCmd and points its buffer at libmc_chdirRecvBuffer (written back
 * from the cache, 0x400 bytes), where the server returns the current directory; libmc_onChdirDone copies it to @p curDirOut when
 * the call ends. Fails with -100 when the library is not bound (libmc_client.server is 0) and -200 when another call
 * still holds the lock semaphore libmc_lockSema (PollSema). On success it records 12 as the pending call (libmc_pendingCall) and
 * keeps the lock until sceMcSync sees the call finish; when sceSifCallRpc fails it releases the lock. No caller in
 * the source.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param newDir  the new directory
 * @param curDirOut  receives the current directory, or 0
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
int sceMcChdir(int cardPort, int cardSlot, char *newDir, void *curDirOut) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    if (newDir == 0 || newDir[0] == 0) {
        SignalSema(libmc_lockSema);
        return -210;
    }
    libmc_fileCmd.port = cardPort;
    libmc_fileCmd.buffer = (int)libmc_chdirRecvBuffer;
    libmc_fileCmd.slot = cardSlot;
    strncpy(libmc_fileCmd.name, newDir, 0x3FF);
    libmc_fileCmd.name[0x3FF] = 0;
    func_00426358((int)libmc_chdirRecvBuffer, 0x400);
    rpcResult = sceSifCallRpc(&libmc_client, 12, 1, &libmc_fileCmd, 1044, libmc_rpcResult, 4, libmc_onChdirDone, curDirOut);
    if (rpcResult == 0)
        libmc_pendingCall = 12;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcFormat);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Starts formatting a memory card (call 16, asynchronous).
 *
 * Fills port and slot of libmc_cmd. Fails with -100 when the library is not bound (libmc_client.server is 0) and -200
 * when another call still holds the lock semaphore libmc_lockSema (PollSema). On success it records 16 as the pending call
 * (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish; when sceSifCallRpc fails it releases the
 * lock.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcFormat(int cardPort, int cardSlot) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.port = cardPort;
    libmc_cmd.slot = cardSlot;
    rpcResult = sceSifCallRpc(&libmc_client, 16, 1, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 16;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcDelete);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
/**
 * Starts deleting a file or directory on a memory card (call 15, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into libmc_fileCmd.name (at most 1023
 * characters, always terminated). Fills port and slot of libmc_fileCmd and clears its mode. Fails with -100 when the
 * library is not bound (libmc_client.server is 0) and -200 when another call still holds the lock semaphore libmc_lockSema
 * (PollSema). On success it records 15 as the pending call (libmc_pendingCall) and keeps the lock until sceMcSync sees the
 * call finish; when sceSifCallRpc fails it releases the lock.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param filePath  the path
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
int sceMcDelete(int cardPort, int cardSlot, char *filePath) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    if (filePath == 0 || filePath[0] == 0) {
        SignalSema(libmc_lockSema);
        return -210;
    }
    strncpy(libmc_fileCmd.name, filePath, 0x3FF);
    libmc_fileCmd.port = cardPort;
    libmc_fileCmd.slot = cardSlot;
    libmc_fileCmd.name[0x3FF] = 0;
    libmc_fileCmd.mode = 0;
    rpcResult = sceSifCallRpc(&libmc_client, 15, 1, &libmc_fileCmd, 1044, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 15;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcFlush);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Starts flushing a memory-card file (call 10, asynchronous).
 *
 * Puts @p fileDesc into the fd field of libmc_cmd. Fails with -100 when the library is not bound (libmc_client.server is 0) and
 * -200 when another call still holds the lock semaphore libmc_lockSema (PollSema). On success it records 10 as the pending
 * call (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish; when sceSifCallRpc fails it releases
 * the lock.
 *
 * @param fileDesc  the file descriptor
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcFlush(int fileDesc) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.fd = fileDesc;
    rpcResult = sceSifCallRpc(&libmc_client, 10, 1, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 10;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcSetFileInfo);
#ifdef SKIP_ASM
extern int R_3DF0(int) __asm__("PollSema");
extern void R_3DC0(int) __asm__("SignalSema");
extern void R_4020(int) __asm__("FlushCache");
extern char *strncpy(char *, const char *, int);
/**
 * Starts changing a memory-card file's information record (call 14, asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into libmc_fileCmd.name (at most 1023
 * characters, always terminated). Masks @p validFlags to its low three bits and stores it as the mode, copies the 64-byte record
 * *@p newInfo into libmc_fileInfoEntry and points the buffer at it, then flushes the cache. Fails with -100 when the library is not
 * bound (libmc_client.server is 0) and -200 when another call still holds the lock semaphore libmc_lockSema (PollSema). On
 * success it records 14 as the pending call (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish;
 * when sceSifCallRpc fails it releases the lock. No caller in the source.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param filePath  the path
 * @param newInfo  the new information record
 * @param validFlags  which parts to change (bits 0..2)
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
int sceMcSetFileInfo(int cardPort, int cardSlot, char *filePath, sMcDirEntry *newInfo, int validFlags) {
    int rpcResult;
    sSifClient *mcClient = &libmc_client;
    if (mcClient->server == 0)
        return -100;
    if (R_3DF0(libmc_lockSema) < 0)
        return -200;
    if (filePath == 0 || filePath[0] == 0) {
        R_3DC0(libmc_lockSema);
        return -210;
    }
    validFlags &= 7;
    libmc_fileCmd.port = cardPort;
    libmc_fileCmd.slot = cardSlot;
    libmc_fileCmd.mode = validFlags;
    libmc_fileInfoEntry = *newInfo;
    libmc_fileCmd.buffer = (int)&libmc_fileInfoEntry;
    strncpy(libmc_fileCmd.name, filePath, 0x3FF);
    libmc_fileCmd.name[0x3FF] = 0;
    R_4020(0);
    rpcResult = sceSifCallRpc(mcClient, 14, 1, &libmc_fileCmd, 1044, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 14;
    else
        R_3DC0(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcRename);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
extern void FlushCache(int);
/**
 * Starts renaming a file on a memory card (call 14 with the rename flag, asynchronous).
 *
 * An empty or null old name, or a null new name, releases the lock and fails with -210. Sets mode 16 in libmc_fileCmd,
 * copies the old name (at most 1023 characters) and the new name into D_00527220 (32 bytes; byte 31 cleared), the name
 * field of the 64-byte record at libmc_fileInfoEntry that the buffer points at, and flushes the cache. Fails with -100 when the
 * library is not bound (libmc_client.server is 0) and -200 when another call still holds the lock semaphore libmc_lockSema
 * (PollSema). On success it records 19 as the pending call (libmc_pendingCall) and keeps the lock until sceMcSync sees the
 * call finish; when sceSifCallRpc fails it releases the lock. (19 rather than the call number). No caller in the
 * source.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param oldPath  the current path
 * @param newName  the new name
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
int sceMcRename(int cardPort, int cardSlot, char *oldPath, char *newName) {
    int rpcResult;
    char *infoRecord;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    if (oldPath == 0 || oldPath[0] == 0 || newName == 0) {
        SignalSema(libmc_lockSema);
        return -210;
    }
    libmc_fileCmd.port = cardPort;
    libmc_fileCmd.slot = cardSlot;
    libmc_fileCmd.mode = 16;
    strncpy(libmc_fileCmd.name, oldPath, 0x3FF);
    libmc_fileCmd.name[0x3FF] = 0;
    strncpy(D_00527220, newName, 0x20);
    infoRecord = D_00527220 - 0x20;
    infoRecord[0x3F] = 0;
    libmc_fileCmd.buffer = (int)infoRecord;
    FlushCache(0);
    rpcResult = sceSifCallRpc(&libmc_client, 14, 1, &libmc_fileCmd, 1044, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 19;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcUnformat);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
/**
 * Starts memory-card call 17 for a port and slot (asynchronous).
 *
 * Fills port and slot of libmc_cmd. Fails with -100 when the library is not bound (libmc_client.server is 0) and -200
 * when another call still holds the lock semaphore libmc_lockSema (PollSema). On success it records 17 as the pending call
 * (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish; when sceSifCallRpc fails it releases the
 * lock. No caller in the source.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @return 0 when the call started, -100 / -200 as above, otherwise sceSifCallRpc's error
 */
int sceMcUnformat(int cardPort, int cardSlot) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    libmc_cmd.port = cardPort;
    libmc_cmd.slot = cardSlot;
    rpcResult = sceSifCallRpc(&libmc_client, 17, 1, &libmc_cmd, 48, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 17;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceMcGetEntSpace);
#ifdef SKIP_ASM
extern int PollSema(int);
extern void SignalSema(int);
extern char *strncpy(char *, const char *, int);
/**
 * Starts memory-card call 18 for a path on a port and slot (asynchronous).
 *
 * An empty or null name releases the lock and fails with -210. The name is copied into libmc_fileCmd.name (at most 1023
 * characters, always terminated). Fills port and slot of libmc_fileCmd. Fails with -100 when the library is not bound
 * (libmc_client.server is 0) and -200 when another call still holds the lock semaphore libmc_lockSema (PollSema). On success
 * it records 18 as the pending call (libmc_pendingCall) and keeps the lock until sceMcSync sees the call finish; when
 * sceSifCallRpc fails it releases the lock. No caller in the source.
 *
 * @param cardPort  the memory-card port
 * @param cardSlot  the slot
 * @param dirPath  the path
 * @return 0 when the call started, -100 / -200 / -210 as above, otherwise sceSifCallRpc's error
 */
int sceMcGetEntSpace(int cardPort, int cardSlot, char *dirPath) {
    int rpcResult;
    if (libmc_client.server == 0)
        return -100;
    if (PollSema(libmc_lockSema) < 0)
        return -200;
    if (dirPath == 0 || dirPath[0] == 0) {
        SignalSema(libmc_lockSema);
        return -210;
    }
    libmc_fileCmd.port = cardPort;
    libmc_fileCmd.slot = cardSlot;
    strncpy(libmc_fileCmd.name, dirPath, 0x3FF);
    libmc_fileCmd.name[0x3FF] = 0;
    rpcResult = sceSifCallRpc(&libmc_client, 18, 1, &libmc_fileCmd, 1044, libmc_rpcResult, 4, 0, 0);
    if (rpcResult == 0)
        libmc_pendingCall = 18;
    else
        SignalSema(libmc_lockSema);
    return rpcResult;
}
#endif

INCLUDE_ASM("seg/seg_2FE810", func_0040B130);

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B1F8);
#ifdef SKIP_ASM
extern int libmrpc_initFlag[];
/**
 * Clears the flag libmrpc_initFlag that func_0040B130 sets when it sets up the multi-client RPC layer.
 *
 * Only func_00319800, which no retail code calls, calls it.
 */
void func_0040B1F8(void) {
    libmrpc_initFlag[0] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmrpc_takePacket);
#ifdef SKIP_ASM
struct E0B208 { char pad[0x10]; int f10; int f14; int f18; char pad2[0x24]; };
struct S0B208 { int f0; struct E0B208 *tab; int n; };
extern int DIntr_rq(void) __asm__("DIntr");
extern void EIntr(void);
struct E0B208 *f0B208(struct S0B208 *s) __asm__("libmrpc_takePacket");
/**
 * Takes a free request packet from a packet pool and gives it the next serial number.
 *
 * With interrupts off (DIntr; EIntr turns them back on unconditionally) it scans the pool's table for the first packet
 * whose flags (+0x10) lack bit 0, sets them to (index << 16) | 5, points +0x14 at the packet itself and stores the pool
 * counter's next value (+0x0) as the serial (+0x18); when that value is 1 the counter is advanced once more.
 * libmrpc_freePacket frees a packet again.
 *
 * @param packetPool  the pool: serial counter, packet table, packet count
 * @return the packet, or 0 when all are in use
 */
struct E0B208 *f0B208(struct S0B208 *packetPool) {
    struct E0B208 *packet;
    int i;
    int n;
    int prevSerial;
    int id;
    int seq;
    DIntr_rq();
    n = packetPool->n;
    i = 0;
    packet = packetPool->tab;
    while (i < n) {
        if (!(packet->f10 & 1)) {
            packet->f10 = (i << 16) | 5;
            prevSerial = packetPool->f0;
            id = prevSerial + 1;
            packetPool->f0 = id;
            if (id == 1) {
                packetPool->f0 = prevSerial + 2;
                seq = 1;
            } else {
                seq = id;
            }
            id = seq;
            packet->f14 = (int)packet;
            packet->f18 = id;
            EIntr();
            return packet;
        }
        i++;
        packet++;
    }
    EIntr();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmrpc_freePacket);
#ifdef SKIP_ASM
/**
 * Frees a request packet taken by libmrpc_takePacket.
 *
 * Clears its serial (+0x18) and the in-use bit 0 of its flags (+0x10).
 *
 * @param packet  the packet
 */
void libmrpc_freePacket(unsigned *packet) {
    packet[6] = 0;
    packet[4] &= 0xFFFFFFFE;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libmrpc_onReplyCmd);
#ifdef SKIP_ASM
struct C_B2D0 { unsigned *f0; int pad4; int f8; int pad[2]; int f14; int f18; void (*f1C)(int); int f20; int f24; int f28; int f2C; };
struct E_B2D0 { char pad[0x1C]; struct C_B2D0 *ctx; unsigned code; int f24; int f28; int f2C; };
extern void iSignalSema(int);
extern void libmrpc_freePacket(unsigned *);
/**
 * SIF command handler for the multi-client RPC layer's replies.
 *
 * func_0040B130 installs it. By the reply code at +0x20: 0x8000001A (call done) runs the client's end function (+0x1C)
 * with its argument (+0x20) and signals the client lock (+0x28); 0x80000019 (bind done) stores the server (+0x24) and
 * two buffer words (+0x14, +0x18) from the reply and signals the lock; 0x8000001D (terminate done) stores the reply
 * word in +0x2C and signals the lock; 0x8000001C does nothing. In every case it then signals the client's wait
 * semaphore (+0x8) when it is not negative, frees the request packet (libmrpc_freePacket) and clears the client's packet
 * pointer.
 *
 * @param reply  the reply: client at +0x1C, code at +0x20, data words from +0x24
 */
void libmrpc_onReplyCmd(struct E_B2D0 *reply) {
    struct C_B2D0 *clientData;
    switch (reply->code) {
    case 0x8000001A:
        clientData = reply->ctx;
        if (clientData->f1C)
            clientData->f1C(clientData->f20);
        iSignalSema(clientData->f28);
        break;
    case 0x80000019:
        clientData = reply->ctx;
        clientData->f24 = reply->f24;
        clientData->f14 = reply->f28;
        clientData->f18 = reply->f2C;
        iSignalSema(clientData->f28);
        break;
    case 0x8000001C:
        break;
    case 0x8000001D:
        clientData = reply->ctx;
        clientData->f2C = reply->f24;
        iSignalSema(clientData->f28);
        break;
    }
    clientData = reply->ctx;
    if (clientData->f8 >= 0)
        iSignalSema(clientData->f8);
    libmrpc_freePacket(clientData->f0);
    clientData->f0 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040B3D8);
#ifdef SKIP_ASM
extern int sceSifMBindRpcParam(int, int, int, int, int, int);
/**
 * Binds a multi-client RPC client to a server with the default parameters 0x800, 0x2000 and 0x20.
 *
 * Calls sceSifMBindRpcParam with them.
 *
 * @param clientData  the client block
 * @param rpcNumber  the server's RPC number
 * @param bindMode  the mode (must be 0)
 * @return sceSifMBindRpcParam's result
 */
int func_0040B3D8(int clientData, int rpcNumber, int bindMode) {
    return sceSifMBindRpcParam(clientData, rpcNumber, bindMode, 0x800, 0x2000, 0x20);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceSifMBindRpcParam);
#ifdef SKIP_ASM
struct N_B400 { char pad0[0x14]; struct N_B400 *f14; int f18; struct C_B400 *f1C; int f20; int f24; int f28; int f2C; };
struct C_B400 { struct N_B400 *f0; int f4; int f8; int padC; int f10; char pad14[0x10]; int f24; int f28; int pad2C; int f30; int f34; int f38; };
extern int CreateSema(int *);
extern int fr_B400(int) __asm__("DeleteSema");
extern void WaitSema(int);
extern void lg_B400(char *) __asm__("scePrintf");
extern struct N_B400 *libmrpc_takePacket(char *);
extern void rl_B400(struct N_B400 *) __asm__("libmrpc_freePacket");
extern int sceSifSendCmd(unsigned, struct N_B400 *, int, int, int, int);
extern char str_mode_must_be_0_in_this_version[];
/**
 * Binds a multi-client RPC client to an IOP server and waits for the answer.
 *
 * A mode other than 0 prints "mode must be 0 in this version." and returns -1. Creates the client's lock semaphore
 * (+0x28, count 1) and takes it, stores the RPC number (+0x10), clears the server (+0x24) and keeps @p bindParam1, @p bindParam2 and @p bindParam3
 * at +0x30..+0x38. Takes a packet from the pool D_00528FC0 (libmrpc_takePacket), records it and its serial (+0x0, +0x4) and
 * fills it with the number, the three parameters and the client. Creates a wait semaphore (+0x8), sends command
 * 0x80000019 with the 64-byte packet (sceSifSendCmd) and waits until libmrpc_onReplyCmd has stored the reply. Deletes the
 * wait semaphore, and the lock too when no server was found (+0x24 still 0).
 *
 * Quirk: it returns 0 even when no server was bound; callers must check +0x24. func_0040B3D8 supplies the defaults; the
 * DNAS import table DirtyDnas_importTable holds it.
 *
 * @param cc  the client block
 * @param rpcNumber  the server's RPC number
 * @param flag  the mode (must be 0)
 * @param bindParam1  first bind parameter (0x800 by default)
 * @param bindParam2  second bind parameter (0x2000 by default)
 * @param bindParam3  third bind parameter (0x20 by default)
 * @return 0 when the request was answered, -1 for a bad mode or no free packet, -2 when the command could not be sent,
 *   -3 when a semaphore could not be created
 */
int sceSifMBindRpcParam(int cc, int rpcNumber, int flag, int bindParam1, int bindParam2, int bindParam3) {
    struct C_B400 *clientData = (struct C_B400 *)cc;
    int semaParam[6];
    int waitSema;
    struct N_B400 *n;
    if (flag != 0) {
        lg_B400(str_mode_must_be_0_in_this_version);
        return -1;
    }
    semaParam[5] = 0;
    semaParam[2] = 1;
    semaParam[1] = 1;
    clientData->f28 = CreateSema(semaParam);
    if (clientData->f28 < 0)
        return -3;
    WaitSema(clientData->f28);
    clientData->f10 = rpcNumber;
    clientData->f24 = 0;
    clientData->f30 = bindParam1;
    clientData->f34 = bindParam2;
    clientData->f38 = bindParam3;
    n = libmrpc_takePacket(D_00528FC0);
    if (n == 0) {
        fr_B400(clientData->f28);
        return -1;
    }
    clientData->f0 = n;
    clientData->f4 = n->f18;
    n->f20 = rpcNumber;
    n->f24 = bindParam1;
    n->f28 = bindParam2;
    n->f2C = bindParam3;
    n->f14 = n;
    n->f1C = clientData;
    semaParam[1] = 1;
    semaParam[2] = 0;
    semaParam[5] = 0;
    waitSema = CreateSema(semaParam);
    clientData->f8 = waitSema;
    if (waitSema < 0) {
        rl_B400(n);
        fr_B400(clientData->f28);
        return -3;
    }
    if (sceSifSendCmd(0x80000019, n, 0x40, 0, 0, 0) == 0) {
        rl_B400(n);
        fr_B400(waitSema);
        fr_B400(clientData->f28);
        return -2;
    }
    WaitSema(waitSema);
    fr_B400(waitSema);
    if (clientData->f24 == 0)
        fr_B400(clientData->f28);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceSifMUnBindRpc);
#ifdef SKIP_ASM
struct O_B5B0 { int f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; int f28; int f2C; };
extern char str_mode_must_be_0_in_this_version[];
extern void b5_00425C60(char *, ...) __asm__("scePrintf");
extern int b5_00423DE0(int) __asm__("WaitSema");
extern int b5_00423DB0(int) __asm__("DeleteSema");
extern struct O_B5B0 *b5_0040B208(char *) __asm__("libmrpc_takePacket");
extern void b5_0040B2B0(struct O_B5B0 *) __asm__("libmrpc_freePacket");
extern int b5_00423DA0(struct QSema *) __asm__("CreateSema");
extern int b5_004261B0(int, struct O_B5B0 *, int, int, int, int) __asm__("sceSifSendCmd");
/**
 * Ends a multi-client RPC client's binding and returns the server's answer.
 *
 * A mode other than 0 prints "mode must be 0 in this version." and returns -1. Takes the client's lock (+0x28), then a
 * packet from D_00528FC0 and fills it with the RPC number (+0x10), the server (+0x24) and the client. Creates a wait
 * semaphore (+0x8), sends command 0x8000001D (sceSifSendCmd), waits for libmrpc_onReplyCmd, then deletes the wait semaphore
 * and the client's lock.
 *
 * Likely bug: when no packet is free it returns -1 with the lock still taken, so later calls on the client block.
 *
 * @param clientData  the client block
 * @param termMode  the mode (must be 0)
 * @return the reply word (+0x2C), -1, -2 when the command could not be sent, or -3 when the semaphore could not be
 *   created
 */
int sceSifMUnBindRpc(struct O_B5B0 *clientData, int termMode) {
    struct O_B5B0 *packet;
    struct QSema semaParam;
    int waitSema;
    if (termMode) {
        b5_00425C60(str_mode_must_be_0_in_this_version);
        return -1;
    }
    b5_00423DE0(clientData->f28);
    packet = b5_0040B208(D_00528FC0);
    if (packet == 0)
        return -1;
    clientData->f4 = packet->f18;
    clientData->f0 = (int)packet;
    packet->f20 = clientData->f10;
    packet->f14 = (int)packet;
    packet->f1C = (int)clientData;
    packet->f24 = clientData->f24;
    semaParam.max_count = 1;
    semaParam.init_count = 0;
    semaParam.option = 0;
    waitSema = b5_00423DA0(&semaParam);
    clientData->f8 = waitSema;
    if (waitSema < 0) {
        b5_0040B2B0(packet);
        return -3;
    }
    if (b5_004261B0(0x8000001D, packet, 0x40, 0, 0, 0) == 0) {
        b5_0040B2B0(packet);
        b5_00423DB0(waitSema);
        return -2;
    }
    b5_00423DE0(waitSema);
    b5_00423DB0(waitSema);
    b5_00423DB0(clientData->f28);
    return clientData->f2C;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceSifMCallRpc);
#ifdef SKIP_ASM
struct N_B6D0 { int f0; int f4; int f8; char padC[8]; int f14; int f18; int f1C; int f20; int f24; int f28; int f2C; int f30; int f34; };
extern void lg3_B6D0(char *, char *, int) __asm__("scePrintf");
extern void lg1_B6D0(char *) __asm__("scePrintf");
extern int wt_B6D0(int) __asm__("WaitSema");
extern int fr_B6D0(int) __asm__("DeleteSema");
extern int sm_B6D0(int *) __asm__("CreateSema");
extern struct N_B6D0 *al_B6D0(char *) __asm__("libmrpc_takePacket");
extern void rl_B6D0(struct N_B6D0 *) __asm__("libmrpc_freePacket");
extern void sz_B6D0(int, int) __asm__("func_00426358");
extern int st_B6D0(unsigned, struct N_B6D0 *, int, int, int, int) __asm__("sceSifSendCmd");
extern char str_mode_must_be_0_in_this_version[];
extern char D_004981A0[];
extern char D_004981D8[];
extern char D_004981E8[];
/**
 * Calls a function of a bound IOP RPC server through the multi-client RPC layer.
 *
 * A mode other than 0 prints "mode must be 0 in this version." and returns -1, which leaves the branch for mode bit 0
 * (no wait; packet +0x30 = whether an end function is set) unreachable. Takes the client's lock (+0x28); a send or
 * receive buffer that is not 64-byte aligned prints D_004981A0 / D_004981E8 (with D_004981D8) and returns -1. Takes a
 * packet from D_00528FC0, stores its serial (+0x4), @p endFunc (+0x1C) and @p i (+0x20) in the client and the RPC number,
 * send size, receive buffer and size, client and server in the packet. Writes the cache back over both buffers (once,
 * with the larger size, when they are the same). Creates a wait semaphore (+0x8), sends command 0x8000001A with the
 * packet and @p sendSize bytes from @p sendBuf to the server's buffer (+0x14) (sceSifSendCmd), waits for libmrpc_onReplyCmd and deletes
 * the semaphore.
 *
 * Likely bug: the alignment and no-packet failures return with the client's lock still taken.
 *
 * @param clientData  the client block
 * @param funcNumber  the function number
 * @param flag  the mode (must be 0)
 * @param sendBuf  the send buffer (64-byte aligned)
 * @param sendSize  the send size
 * @param recvBuf  the receive buffer (64-byte aligned)
 * @param recvSize  the receive size
 * @param endFunc  the end function, or 0
 * @param i  the end function's argument
 * @return 0, -1, -2 when the command could not be sent, or -3 when the semaphore could not be created
 */
int sceSifMCallRpc(struct N_B6D0 *clientData, int funcNumber, int flag, int sendBuf, int sendSize, int recvBuf, int recvSize, int endFunc, int i) {
    int semaParam[6];
    int sem;
    struct N_B6D0 *n;
    if (flag != 0) {
        lg1_B6D0(str_mode_must_be_0_in_this_version);
        return -1;
    }
    wt_B6D0(clientData->f28);
    if (sendBuf & 0x3F) {
        lg3_B6D0(D_004981A0, D_004981D8, sendBuf);
        return -1;
    }
    if (recvBuf & 0x3F) {
        lg3_B6D0(D_004981E8, D_004981D8, recvBuf);
        return -1;
    }
    n = al_B6D0(D_00528FC0);
    if (n == 0)
        return -1;
    clientData->f4 = n->f18;
    clientData->f20 = i;
    clientData->f0 = (int)n;
    clientData->f1C = endFunc;
    n->f20 = funcNumber;
    n->f14 = (int)n;
    n->f24 = sendSize;
    n->f28 = recvBuf;
    n->f2C = recvSize;
    n->f1C = (int)clientData;
    n->f34 = clientData->f24;
    if (sendBuf == recvBuf) {
        sz_B6D0(sendBuf, sendSize >= recvSize ? sendSize : recvSize);
    } else {
        if (sendSize > 0)
            sz_B6D0(sendBuf, sendSize);
        if (recvSize > 0)
            sz_B6D0(recvBuf, recvSize);
    }
    if (flag & 1) {
        if (endFunc == 0)
            n->f30 = 0;
        else
            n->f30 = 1;
        clientData->f8 = -1;
        if (st_B6D0(0x8000001A, n, 0x40, sendBuf, clientData->f14, sendSize) == 0) {
            rl_B6D0(n);
            return -2;
        }
        return 0;
    }
    semaParam[2] = 0;
    semaParam[1] = 1;
    semaParam[5] = 0;
    sem = sm_B6D0(semaParam);
    clientData->f8 = sem;
    if (sem < 0) {
        rl_B6D0(n);
        return -3;
    }
    n->f30 = 1;
    if (st_B6D0(0x8000001A, n, 0x40, sendBuf, clientData->f14, sendSize) == 0) {
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
INCLUDE_ASM("seg/seg_2FE810", libscf_readRomVersion);
#ifdef SKIP_ASM
extern int sceOpen(char *, int);
extern void sceRead(int, char *, int);
extern void sceClose(int);
extern char str_rom0_ROMVER[];
/**
 * Reads the console's ROM version string once and returns its buffer.
 *
 * While libscf_romVersion is still empty it opens the file str_rom0_ROMVER ("rom0:ROMVER") with sceOpen (mode 1), reads 14 bytes
 * into libscf_romVersion (sceRead) and closes it (sceClose). The buffer stays empty when the open fails.
 *
 * @return the address of libscf_romVersion
 */
int libscf_readRomVersion(void) {
    int romverFd;
    if (libscf_romVersion[0] == 0) {
        romverFd = sceOpen(str_rom0_ROMVER, 1);
        if (romverFd >= 0) {
            sceRead(romverFd, libscf_romVersion, 14);
            sceClose(romverFd);
        }
    }
    return (int)libscf_romVersion;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_isT10000);
#ifdef SKIP_ASM
extern int libscf_readRomVersion(void);
/**
 * Returns whether the console is a T10000 development kit.
 *
 * Reads the ROM version string first when needed (libscf_readRomVersion) and tests its region character (byte 4) for 'T'. The
 * console-setting getters sceScfGetLanguage .. sceScfGetTimeNotation use it to choose the override record libscf_configOverride over the OSD
 * configuration.
 *
 * @return 1 for a T10000 (tool) console, else 0
 */
int libscf_isT10000(void) {
    if (libscf_romVersion[0] == 0)
        libscf_readRomVersion();
    return libscf_romVersion[4] == 84;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetLanguage);
#ifdef SKIP_ASM
extern int libscf_isT10000(void);
extern void GetOsdConfigParam(unsigned *);
/**
 * Returns the console's language setting.
 *
 * On a T10000 development kit (libscf_isT10000) it returns the override byte D_004533FC that func_0040BA48 sets.
 * Otherwise it reads the OSD configuration (GetOsdConfigParam): Consoles whose OSD configuration has version 0 (bits
 * 13..15) give the Japanese-language flag (bit 4)., newer ones the language field (bits 16..20). Quirk: it calls
 * GetOsdConfigParam once more at the start, unused. cPS2ConsoleConfig_cPS2ConsoleConfig keeps 0..7 as its language.
 *
 * @return the language number
 */
int sceScfGetLanguage(void) {
    unsigned buf[1];
    int language;
    GetOsdConfigParam(buf);
    if (libscf_isT10000()) {
        language = D_004533FC[0];
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0)
            language = (buf[0] >> 4) & 1;
        else
            language = (buf[0] >> 16) & 0x1F;
    }
    return language;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", func_0040BA48);
#ifdef SKIP_ASM
/**
 * Sets the 8-byte console-setting record that T10000 development kits use instead of the OSD.
 *
 * Copies *@p consoleConfig into libscf_configOverride: the time zone in minutes (+0x0, short), then one byte each for the aspect, date format,
 * language, S/PDIF, summer time and time notation (read by sceScfGetTimeZone, sceScfGetAspect, sceScfGetDateFormat, sceScfGetLanguage,
 * sceScfGetSpdif, sceScfGetSummerTime and sceScfGetTimeNotation). No caller in the source.
 *
 * @param consoleConfig  the new settings
 */
void func_0040BA48(sSysConfig *consoleConfig) {
    libscf_configOverride = *consoleConfig;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetAspect);
#ifdef SKIP_ASM
extern int libscf_isT10000(void);
extern void GetOsdConfigParam(unsigned *);
/**
 * Returns the console's screen aspect setting.
 *
 * On a T10000 development kit (libscf_isT10000) it returns the override byte D_004533FA that func_0040BA48 sets.
 * Otherwise it returns the screen-type field of the OSD configuration (bits 1..2).
 *
 * @return the aspect setting (0..2)
 */
int sceScfGetAspect(void) {
    unsigned buf[1];
    if (libscf_isT10000())
        return D_004533FA[0];
    GetOsdConfigParam(buf);
    return (buf[0] >> 1) & 3;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetSpdif);
#ifdef SKIP_ASM
extern int libscf_isT10000(void);
extern void GetOsdConfigParam(unsigned *);
/**
 * Returns the console's S/PDIF (digital audio output) setting.
 *
 * On a T10000 development kit (libscf_isT10000) it returns the override byte D_004533FD that func_0040BA48 sets.
 * Otherwise it returns bit 0 of the OSD configuration. cPS2ConsoleConfig_cPS2ConsoleConfig treats 0 as enabled.
 *
 * @return the S/PDIF setting (0 on, 1 off)
 */
int sceScfGetSpdif(void) {
    unsigned buf[1];
    if (libscf_isT10000())
        return D_004533FD[0];
    GetOsdConfigParam(buf);
    return buf[0] & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetTimeZone);
#ifdef SKIP_ASM
extern int libscf_isT10000(void);
extern void GetOsdConfigParam(unsigned *);
/**
 * Returns the console's time zone as minutes from GMT.
 *
 * On a T10000 development kit (libscf_isT10000) it returns the override libscf_configOverride.f0 that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 540 (GMT+9, Japan). Newer ones give the signed field in bits
 * 21..31 of the OSD configuration (GetOsdConfigParam).
 *
 * @return the offset in minutes
 */
int sceScfGetTimeZone(void) {
    unsigned buf[1];
    int zoneMinutes;
    if (libscf_isT10000()) {
        zoneMinutes = libscf_configOverride.f0;
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) zoneMinutes = 540; else zoneMinutes = (int)buf[0] >> 21;
    }
    return zoneMinutes;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetDateFormat);
#ifdef SKIP_ASM
extern int libscf_isT10000(void);
extern void GetOsdConfigParam(unsigned *);
extern int GetOsdConfigParam2(unsigned *, int, int);
/**
 * Returns the console's date format setting.
 *
 * On a T10000 development kit (libscf_isT10000) it returns the override byte D_004533FB that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 0. Newer ones read the second byte of the extra OSD
 * configuration (GetOsdConfigParam2) and return its bits 6..7. cPS2ConsoleConfig_cPS2ConsoleConfig keeps 0..2.
 *
 * @return the date format (0..2)
 */
int sceScfGetDateFormat(void) {
    unsigned buf[2];
    unsigned char config2Byte;
    int dateFormat;
    if (libscf_isT10000()) {
        dateFormat = D_004533FB[0];
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            dateFormat = 0;
        } else {
            GetOsdConfigParam2(&buf[1], 1, 1);
            config2Byte = ((unsigned char *)buf)[4];
            dateFormat = config2Byte >> 6;
        }
    }
    return dateFormat;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetSummerTime);
#ifdef SKIP_ASM
extern int libscf_isT10000(void);
extern void GetOsdConfigParam(unsigned *);
extern int GetOsdConfigParam2(unsigned *, int, int);
/**
 * Returns whether the console's summer time (daylight saving) setting is on.
 *
 * On a T10000 development kit (libscf_isT10000) it returns the override byte D_004533FE that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 0. Newer ones return bit 4 of the extra OSD configuration
 * byte (GetOsdConfigParam2).
 *
 * @return 1 when summer time is on, else 0
 */
int sceScfGetSummerTime(void) {
    unsigned buf[2];
    unsigned char config2Byte;
    int summerTime;
    if (libscf_isT10000()) {
        summerTime = D_004533FE[0];
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            summerTime = 0;
        } else {
            GetOsdConfigParam2(&buf[1], 1, 1);
            config2Byte = ((unsigned char *)buf)[4];
            summerTime = (config2Byte >> 4) & 1;
        }
    }
    return summerTime;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetTimeNotation);
#ifdef SKIP_ASM
extern int libscf_isT10000(void);
extern void GetOsdConfigParam(unsigned *);
extern int GetOsdConfigParam2(unsigned *, int, int);
/**
 * Returns the console's time notation setting (24- or 12-hour clock).
 *
 * On a T10000 development kit (libscf_isT10000) it returns the override byte D_004533FF that func_0040BA48 sets. Consoles
 * whose OSD configuration has version 0 (bits 13..15) give 0. Newer ones return bit 5 of the extra OSD configuration
 * byte (GetOsdConfigParam2). cPS2ConsoleConfig_cPS2ConsoleConfig keeps 0 or 1.
 *
 * @return the time notation (0 or 1)
 */
int sceScfGetTimeNotation(void) {
    unsigned buf[2];
    unsigned char config2Byte;
    int timeNotation;
    if (libscf_isT10000()) {
        timeNotation = D_004533FF[0];
    } else {
        GetOsdConfigParam(buf);
        if (((buf[0] >> 13) & 7) == 0) {
            timeNotation = 0;
        } else {
            GetOsdConfigParam2(&buf[1], 1, 1);
            config2Byte = ((unsigned char *)buf)[4];
            timeNotation = (config2Byte >> 5) & 1;
        }
    }
    return timeNotation;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_toBcd);
#ifdef SKIP_ASM
/**
 * Converts a binary number from 0 to 99 into packed BCD.
 *
 * @param decimal  the number
 * @return the BCD byte
 */
unsigned char libscf_toBcd(unsigned char decimal) {
    return decimal + (decimal / 10) * 6;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_fromBcd);
#ifdef SKIP_ASM
/**
 * Converts a packed BCD byte into binary.
 *
 * @param bcd  the BCD byte
 * @return the number
 */
unsigned char libscf_fromBcd(unsigned char bcd) {
    unsigned char tensPart = bcd >> 4;
    tensPart = tensPart * 6;
    return bcd - tensPart;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_clockFromBcd);
#ifdef SKIP_ASM
/**
 * Converts a console clock record's BCD fields to binary in place.
 *
 * Converts bytes 1..3 (second, minute, hour) and 5..7 (day, month, year) with libscf_fromBcd.
 *
 * @param clock  the 8-byte clock record
 */
void libscf_clockFromBcd(unsigned char *clock) {
    clock[7] = libscf_fromBcd(clock[7]);
    clock[6] = libscf_fromBcd(clock[6]);
    clock[5] = libscf_fromBcd(clock[5]);
    clock[3] = libscf_fromBcd(clock[3]);
    clock[2] = libscf_fromBcd(clock[2]);
    clock[1] = libscf_fromBcd(clock[1]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_clockToBcd);
#ifdef SKIP_ASM
/**
 * Converts a console clock record's binary fields back to BCD in place.
 *
 * Converts bytes 1..3 (second, minute, hour) and 5..7 (day, month, year) with libscf_toBcd.
 *
 * @param clock  the 8-byte clock record
 */
void libscf_clockToBcd(unsigned char *clock) {
    clock[7] = libscf_toBcd(clock[7]);
    clock[6] = libscf_toBcd(clock[6]);
    clock[5] = libscf_toBcd(clock[5]);
    clock[3] = libscf_toBcd(clock[3]);
    clock[2] = libscf_toBcd(clock[2]);
    clock[1] = libscf_toBcd(clock[1]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_clockNextDay);
#ifdef SKIP_ASM
typedef struct { char b[12]; } sDaysTbl;
typedef struct { unsigned char stat, sec, min, hour, pad, day, month, year; } sClk;
extern sDaysTbl libscf_monthDaysTable;
/**
 * Advances a binary clock record by one day.
 *
 * Uses the month lengths in libscf_monthDaysTable, with February 29 in years divisible by 4 (two-digit year). Past the end of the
 * month it goes to day 1 of the next month, and after December to January of the next year (99 wraps to 0).
 *
 * @param clockBytes  the clock record (binary fields)
 */
void libscf_clockNextDay(unsigned char *clockBytes) {
    sClk *clock = (sClk *)clockBytes;
    sDaysTbl days = libscf_monthDaysTable;
    clock->day++;
    if ((clock->year % 4) == 0)
        days.b[1] = 29;
    if (days.b[clock->month - 1] < clock->day) {
        clock->day = 1;
        clock->month++;
        if (clock->month == 13) {
            if (clock->year == 99)
                clock->year = 0;
            else
                clock->year++;
            clock->month = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_clockPrevDay);
#ifdef SKIP_ASM
/**
 * Moves a binary clock record back by one day.
 *
 * Before day 1 it goes to the last day of the previous month (month lengths in libscf_monthDaysTable, February 29 in years
 * divisible by 4), and before January to December of the previous year (0 wraps to 99).
 *
 * @param clockBytes  the clock record (binary fields)
 */
void libscf_clockPrevDay(unsigned char *clockBytes) {
    sClk *clock = (sClk *)clockBytes;
    sDaysTbl days = libscf_monthDaysTable;
    clock->day--;
    if ((clock->year % 4) == 0)
        days.b[1] = 29;
    if (clock->day == 0) {
        clock->month--;
        if (clock->month == 0) {
            if (clock->year == 0)
                clock->year = 99;
            else
                clock->year--;
            clock->month = 12;
        }
        clock->day = days.b[clock->month - 1];
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_clockNextHour);
#ifdef SKIP_ASM
extern void libscf_clockNextDay(unsigned char *);
/**
 * Advances a binary clock record by one hour, moving to the next day at 24.
 *
 * @param clock  the clock record (binary fields)
 */
void libscf_clockNextHour(unsigned char *clock) {
    clock[3]++;
    if (clock[3] == 24) {
        clock[3] = 0;
        libscf_clockNextDay(clock);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_clockPrevHour);
#ifdef SKIP_ASM
extern void libscf_clockPrevDay(unsigned char *);
/**
 * Moves a binary clock record back by one hour, moving to the previous day before 0.
 *
 * @param clock  the clock record (binary fields)
 */
void libscf_clockPrevHour(unsigned char *clock) {
    if (clock[3] == 0) {
        clock[3] = 23;
        libscf_clockPrevDay(clock);
    } else {
        clock[3]--;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libscf_addMinutes);
#ifdef SKIP_ASM
extern void libscf_clockFromBcd(unsigned char *);
extern void libscf_clockNextHour(unsigned char *);
extern void libscf_clockPrevHour(unsigned char *);
extern void libscf_clockToBcd(unsigned char *);
/**
 * Adds a number of minutes to a BCD console clock record, carrying into hours and days.
 *
 * Converts the record to binary (libscf_clockFromBcd), adds @p deltaMinutes to the minute, then moves the hour forward
 * (libscf_clockNextHour) or back (libscf_clockPrevHour) by one per 60 minutes, and converts it back to BCD (libscf_clockToBcd). Likely
 * bug: the forward loop runs only while the minute is above 60, so a sum of exactly 60 (or 120, ...) leaves minute 60
 * in the record. Seconds are not touched.
 *
 * @param clockAddr  the 8-byte clock record
 * @param deltaMinutes  minutes to add (may be negative)
 */
void libscf_addMinutes(int clockAddr, int deltaMinutes) {
    int minutes;
    unsigned char *clock = (unsigned char *)clockAddr;
    libscf_clockFromBcd(clock);
    minutes = clock[2] + deltaMinutes;
    if (minutes >= 0) {
        if (minutes > 60) {
            do {
                minutes -= 60;
                libscf_clockNextHour(clock);
            } while (minutes > 60);
        }
        clock[2] = minutes;
    } else {
        while (minutes < 0) {
            minutes += 60;
            libscf_clockPrevHour(clock);
        }
        clock[2] = minutes;
    }
    libscf_clockToBcd(clock);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetGMTfromRTC);
#ifdef SKIP_ASM
extern void libscf_addMinutes(int, int);
/**
 * Converts a console clock record from Japan time to GMT.
 *
 * Subtracts 540 minutes (9 hours) through libscf_addMinutes.
 *
 * @param clockAddr  the BCD clock record
 */
void sceScfGetGMTfromRTC(int clockAddr) {
    libscf_addMinutes(clockAddr, -540);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceScfGetLocalTimefromRTC);
#ifdef SKIP_ASM
extern int sceScfGetTimeZone(void);
extern int sceScfGetSummerTime(void);
extern void libscf_addMinutes(int, int);
/**
 * Converts a console clock record from Japan time to the console's local time.
 *
 * Adds the time zone (sceScfGetTimeZone) minus 540 minutes, plus 60 when summer time is on (sceScfGetSummerTime), through
 * libscf_addMinutes. cPS2ConsoleConfig_cPS2ConsoleConfig converts the clock read by sceCdReadClock with it.
 *
 * @param clockAddr  the BCD clock record
 */
void sceScfGetLocalTimefromRTC(int clockAddr) {
    int timeZone = sceScfGetTimeZone();
    libscf_addMinutes(clockAddr, timeZone - 540 + sceScfGetSummerTime() * 60);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbInit);
#ifdef SKIP_ASM
struct KbInfo_C028 { int n; char pad[0x8C]; };
extern char D_00498240[];
extern char D_00498258[];
extern void k_6408(int) __asm__("func_00426408");
extern int k_sema(struct QSema *) __asm__("CreateSema");
extern int k_dsema(int) __asm__("DeleteSema");
extern void k_log(char *) __asm__("scePrintf");
extern int sceUsbKbGetInfo(void *info);
extern int k_C810(int, int *) __asm__("sceUsbKbSync");
extern int k_3A80(int, int (*)(void), int) __asm__("AddIntcHandler");
extern int k_48E8(int) __asm__("EnableIntc");
extern int libusbkb_onVblank(void);
/**
 * Initialises the USB keyboard client: binds its server, checks the connection count and sets up every keyboard.
 *
 * Calls func_00426408(0) and binds SIF RPC server 0x80000211 into libusbkb_client, retrying with a busy wait until the
 * server is set; a bind error prints "libusbkb : bind errr". Creates the semaphore libusbkb_rpcSema ("libusbkb : CreateSema
 * Error" on failure), reads the information block with sceUsbKbGetInfo and waits for it (sceUsbKbSync). The maximum
 * number of keyboards must be 1..127; it goes to libusbkb_state.n and *@p out. Takes the keyboard array from D_005291D0 and
 * sets every keyboard to LED mode 1, code type 1, arrangement 1, raw mode off and an idle repeat. Clears the previous
 * report, installs libusbkb_onVblank as INTC 3 (vertical blank) handler (AddIntcHandler; id in libusbkb_state.thread) and
 * enables that line (EnableIntc).
 *
 * Every failure after the semaphore deletes it again and sets libusbkb_rpcSema to -1.
 *
 * @param out  receives the maximum number of keyboards
 * @return 0 on success, -1 on any failure
 */
int sceUsbKbInit(int *out) {
    struct KbInfo_C028 info;
    struct QSema semaParam;
    int st;
    unsigned i;
    k_6408(0);
    for (;;) {
        if (sceSifBindRpc(&libusbkb_client, 0x80000211, 0) < 0) {
            k_log(D_00498240);
            return -1;
        }
        if (libusbkb_client.server != 0)
            break;
        i = 0x10000;
        while (i--);
    }
    semaParam.init_count = 1;
    semaParam.max_count = 1;
    libusbkb_rpcSema = k_sema(&semaParam);
    if (libusbkb_rpcSema == -1) {
        k_log(D_00498258);
        return -1;
    }
    if (sceUsbKbGetInfo(&info) != 0) {
        k_dsema(libusbkb_rpcSema);
        libusbkb_rpcSema = -1;
        return -1;
    }
    k_C810(0, &st);
    if (st != 0) {
        k_dsema(libusbkb_rpcSema);
        libusbkb_rpcSema = -1;
        return -1;
    }
    if (info.n <= 0) {
        k_dsema(libusbkb_rpcSema);
        libusbkb_rpcSema = -1;
        return -1;
    }
    if (info.n >= 0x80) {
        k_dsema(libusbkb_rpcSema);
        libusbkb_rpcSema = -1;
        return -1;
    }
    libusbkb_state.n = info.n;
    *out = info.n;
    libusbkb_state.port = D_005291D0;
    if (libusbkb_state.port == 0) {
        k_dsema(libusbkb_rpcSema);
        libusbkb_rpcSema = -1;
        return -1;
    }
    for (i = 0; i < info.n; i++) {
        libusbkb_state.port[i].rep.n = 0;
        libusbkb_state.port[i].ledMode = 1;
        libusbkb_state.port[i].f04 = 1;
        libusbkb_state.port[i].f08 = 1;
        libusbkb_state.port[i].f0C = 0;
        libusbkb_state.port[i].rep.state = 0;
        libusbkb_state.port[i].rep.count = 0;
        libusbkb_state.port[i].rep.delay = 0;
        libusbkb_state.port[i].rep.rate = 0;
        libusbkb_state.port[i].rep.wr = 0;
        libusbkb_state.port[i].rep.rd = 0;
        libusbkb_state.port[i].rep.last = 0;
    }
    libusbkb_state.prev.n = 0;
    libusbkb_state.thread = k_3A80(3, libusbkb_onVblank, -1);
    if (libusbkb_state.thread < 0) {
        k_dsema(libusbkb_rpcSema);
        libusbkb_rpcSema = -1;
        return -1;
    }
    k_48E8(3);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbEnd);
#ifdef SKIP_ASM
extern int DIntr_0() __asm__("DIntr");
extern void EIntr(void);
extern int RemoveIntcHandler(int, int);
extern void DisableIntc(int);
extern int DeleteSema(int);
extern char D_00498278[];
extern char D_004982A8[];
/**
 * Shuts the USB keyboard client down: removes its vertical-blank handler and deletes its semaphore.
 *
 * With interrupts off (DIntr; restored with EIntr when they were on) it removes the INTC 3 handler D_0052909C and, when
 * that worked, disables the line (DisableIntc). A failed removal prints "libusbkb.a : RemoveIntcHandler() --> error".
 * Deletes the semaphore libusbkb_rpcSema when it exists and sets it to -1; a failure prints "libusbkb.a : DeleteSema() -->
 * error" and keeps the value. The server binding stays. kbReader_shutdown calls it.
 *
 * @return 0 when both steps worked, else -1
 */
int sceUsbKbEnd(void) {
    int n = 0;
    int intrState, removeResult;
    intrState = DIntr_0();
    removeResult = RemoveIntcHandler(3, D_0052909C);
    if (removeResult == 0)
        DisableIntc(3);
    if (intrState)
        EIntr();
    if (removeResult < 0) {
        n = 1;
        sceMcUdCheckNewCard_log(D_00498278);
    }
    if (libusbkb_rpcSema != -1) {
        if (DeleteSema(libusbkb_rpcSema) < 0) {
            n++;
            sceMcUdCheckNewCard_log(D_004982A8);
        } else {
            libusbkb_rpcSema = -1;
        }
    }
    return n == 0 ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbGetInfo);
#ifdef SKIP_ASM
extern char str_sceUsbKbGetInfo_sceSifCallRpc_pct_d[];
extern void libusbkb_onRpcDone(void *);
extern void WaitSema(int);
extern void scePrintf(char *, int);
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
    if (libusbkb_rpcSema == -1)
        return -1;
    WaitSema(libusbkb_rpcSema);
    result = sceSifCallRpc(&libusbkb_client, 1, 1, libusbkb_sendBuffer, 16, libusbkb_recvBuffer, 144, libusbkb_onRpcDone, 0);
    if (result != 0) {
        scePrintf(str_sceUsbKbGetInfo_sceSifCallRpc_pct_d, result);
        return -2;
    }
    libusbkb_state.cmd = 1;
    libusbkb_state.out = info;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbRead);
#ifdef SKIP_ASM
extern void libusbkb_onRpcDone(void *);
extern void WaitSema(int);
extern void scePrintf(char *, int);
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
    if (libusbkb_rpcSema == -1)
        return -1;
    if (kbdNo >= libusbkb_state.n)
        return -11;
    libusbkb_sendBuffer[0] = kbdNo;
    WaitSema(libusbkb_rpcSema);
    {
        int result = sceSifCallRpc(&libusbkb_client, 2, 1, libusbkb_sendBuffer, 16, libusbkb_recvBuffer, 80, libusbkb_onRpcDone, 0);
        if (result != 0) {
            scePrintf(str_sceUsbKbRead_sceSifCallRpc_pct_d, result);
            return -2;
        }
    }
    libusbkb_state.cmd = 2;
    libusbkb_state.arg = kbdNo;
    libusbkb_state.out = data;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbGetLocation);
#ifdef SKIP_ASM
extern char str_sceUsbKbGetLocation_sceSifCallRpc_pct_d[];
extern void libusbkb_onRpcDone(void *);
extern void WaitSema(int);
extern void scePrintf(char *, int);
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
    if (libusbkb_rpcSema == -1)
        return -1;
    if (kbdNo >= libusbkb_state.n)
        return -11;
    libusbkb_sendBuffer[0] = kbdNo;
    WaitSema(libusbkb_rpcSema);
    {
        int result = sceSifCallRpc(&libusbkb_client, 3, 1, libusbkb_sendBuffer, 16, libusbkb_recvBuffer, 0x10, libusbkb_onRpcDone, 0);
        if (result != 0) {
            scePrintf(str_sceUsbKbGetLocation_sceSifCallRpc_pct_d, result);
            return -2;
        }
    }
    libusbkb_state.cmd = 3;
    libusbkb_state.arg = kbdNo;
    libusbkb_state.out = location;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetLEDStatus);
#ifdef SKIP_ASM
extern char str_sceUsbKbSetLEDStatus_sceSifCallRpc_pct_d[];
extern void libusbkb_onRpcDone(void *);
extern void WaitSema(int);
extern void scePrintf(char *, int);
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
    if (libusbkb_rpcSema == -1)
        return -1;
    if (kbdNo >= libusbkb_state.n)
        return -11;
    libusbkb_sendBuffer[0] = kbdNo;
    libusbkb_sendBuffer[1] = ledStatus;
    WaitSema(libusbkb_rpcSema);
    {
        int result = sceSifCallRpc(&libusbkb_client, 4, 1, libusbkb_sendBuffer, 16, libusbkb_recvBuffer, 0x10, libusbkb_onRpcDone, 0);
        if (result != 0) {
            scePrintf(str_sceUsbKbSetLEDStatus_sceSifCallRpc_pct_d, result);
            return -2;
        }
    }
    libusbkb_state.cmd = 4;
    libusbkb_state.out = 0;
    libusbkb_state.arg = kbdNo;
    return 0;
}

#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetLEDMode);
#ifdef SKIP_ASM
extern char str_sceUsbKbSetLEDMode_sceSifCallRpc_pct_d[];
extern void libusbkb_onRpcDone(void *);
extern void WaitSema(int);
extern void scePrintf(char *, int);
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
    int semaId = libusbkb_rpcSema;
    if (semaId == -1)
        return -1;
    if (kbdNo >= libusbkb_state.n)
        return -11;
    if (mode > 2)
        return -12;
    libusbkb_state.port[kbdNo].ledMode = mode;
    libusbkb_sendBuffer[0] = kbdNo;
    libusbkb_sendBuffer[1] = mode;
    WaitSema(semaId);
    {
        int rpcResult = sceSifCallRpc(&libusbkb_client, 5, 1, libusbkb_sendBuffer, 16, libusbkb_recvBuffer, 0x10, libusbkb_onRpcDone, 0);
        if (rpcResult != 0) {
            scePrintf(str_sceUsbKbSetLEDMode_sceSifCallRpc_pct_d, rpcResult);
            return -2;
        }
    }
    libusbkb_state.cmd = 5;
    libusbkb_state.out = 0;
    libusbkb_state.arg = kbdNo;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetRepeat);
#ifdef SKIP_ASM
/**
 * Sets a keyboard's key-repeat delay and rate (in vertical blanks).
 *
 * Stores @p repeatRate as the rate and @p repeatDelay as the delay of keyboard @p kbIndex's repeat state and clears its count; libusbkb_onVblank
 * counts and libusbkb_onReadResult repeats. Unlike its neighbours it does not check @p kbIndex against the keyboard count.
 * kbReader_defaultOnConnect sets 30 and 2.
 *
 * @param kbIndex  the keyboard number
 * @param repeatDelay  vertical blanks before the first repeat
 * @param repeatRate  vertical blanks between repeats
 * @return 0, -11 for a negative delay or -12 for a negative rate
 */
int sceUsbKbSetRepeat(int kbIndex, int repeatDelay, int repeatRate) {
    int errCode;
    errCode = -11;
    if (repeatDelay < 0)
        return errCode;
    if (repeatRate < 0)
        return -12;
    D_00529004[kbIndex].rep.rate = repeatRate;
    D_00529004[kbIndex].rep.delay = repeatDelay;
    D_00529004[kbIndex].rep.count = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetCodeType);
#ifdef SKIP_ASM
/**
 * Turns a keyboard's key-code translation on or off.
 *
 * Stores @p codeType in the keyboard's code type (f04), which libusbkb_copyKeyCodes reads. kbReader_defaultOnConnect sets 0.
 *
 * @param kbIndex  the keyboard number
 * @param codeType  0 raw codes, 1 translated
 * @return 0, -11 for a bad keyboard or -12 for a bad type
 */
int sceUsbKbSetCodeType(int kbIndex, unsigned codeType) {

    if (kbIndex >= libusbkb_state.n)
        return -11;
    if (codeType >= 2)
        return -12;
    libusbkb_state.port[kbIndex].f04 = codeType;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetArrangement);
#ifdef SKIP_ASM
/**
 * Selects a keyboard's key layout table (0..2).
 *
 * Stores @p layout in the keyboard's arrangement (f08), which sceUsbKbCnvRawCode reads. usbkeyboard_setupKeyboard sets 0.
 *
 * @param kbIndex  the keyboard number
 * @param layout  the layout (0..2)
 * @return 0, -11 for a bad keyboard or -12 for a bad layout
 */
int sceUsbKbSetArrangement(int kbIndex, unsigned layout) {
    if (kbIndex >= libusbkb_state.n)
        return -11;
    if (layout >= 3)
        return -12;
    libusbkb_state.port[kbIndex].f08 = layout;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSync);
#ifdef SKIP_ASM
extern void kb_signal4(int) __asm__("SignalSema");
extern int libusbkb_finishPendingCall(void);
/**
 * Waits for or polls the pending USB keyboard call and finishes it.
 *
 * Mode 0 waits on the semaphore libusbkb_rpcSema (signalled by libusbkb_onRpcDone when the call ends), stores libusbkb_finishPendingCall's
 * result in *@p out and releases the semaphore. Mode 1 checks the RPC (sceSifCheckStatRpc) and, when it is done, stores
 * libusbkb_finishPendingCall's result in *@p out.
 *
 * @param mode  0 to wait, 1 to poll
 * @param out  receives the call's result
 * @return 0 when finished, 1 while busy (mode 1), -11 for another mode, -1 before initialisation
 */
int sceUsbKbSync(int mode, int *out) {
    if (libusbkb_rpcSema == -1)
        return -1;
    switch (mode) {
    case 0:
        WaitSema(libusbkb_rpcSema);
        *out = libusbkb_finishPendingCall();
        kb_signal4(libusbkb_rpcSema);
        return 0;
    case 1:
        if (sceSifCheckStatRpc(&libusbkb_client) == 0) {
            *out = libusbkb_finishPendingCall();
            return 0;
        }
        return 1;
    default:
        return -11;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_onRpcDone);
#ifdef SKIP_ASM
extern void iSignalSema(int);
/**
 * End callback of every USB keyboard RPC: signals the semaphore libusbkb_rpcSema from interrupt context.
 *
 * @param callbackArg  the end-function argument (always 0, unused)
 */
void libusbkb_onRpcDone(void *callbackArg) {
    iSignalSema(libusbkb_rpcSema);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_finishPendingCall);
#ifdef SKIP_ASM
extern int (*libusbkb_resultHandlerTable[])(void);
/**
 * Finishes the pending USB keyboard call by running its result handler.
 *
 * For call codes 1..5 (libusbkb_state.cmd) it runs the handler libusbkb_resultHandlerTable[code]; then clears the pending call and its
 * output pointer.
 *
 * @return the handler's result, or 0
 */
int libusbkb_finishPendingCall(void) {
    int handlerResult = 0;
    unsigned pendingCmd = libusbkb_state.cmd;
    if (pendingCmd - 1 < 5)
        handlerResult = libusbkb_resultHandlerTable[pendingCmd]();
    libusbkb_state.cmd = 0;
    libusbkb_state.out = 0;
    return handlerResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_onGetInfoResult);
#ifdef SKIP_ASM
struct S40C928 { int n; int m; unsigned char d[1]; };
/**
 * Result handler of call 1 (keyboard information): copies the reply into the caller's block.
 *
 * From the receive buffer libusbkb_recvBuffer it stores the maximum count (byte 0) and the connected count (byte 1) in the
 * caller's block (libusbkb_state.out) and copies one status byte per possible keyboard from byte 2 on. Handler 1 of
 * libusbkb_resultHandlerTable.
 *
 * @return 0
 */
int libusbkb_onGetInfoResult(void) {
    struct S40C928 *infoOut = (struct S40C928 *)D_00529098;
    unsigned char *recvBytes = libusbkb_recvBuffer;
    int i = 0;
    int n = recvBytes[0];
    infoOut->n = n;
    infoOut->m = recvBytes[1];
    if (n != 0) {
        do {
            infoOut->d[i] = recvBytes[2 + i];
            i++;
        } while (i < infoOut->n);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_onReadResult);
#ifdef SKIP_ASM
extern int libusbkb_clearKeyRing(int);
extern int libusbkb_pushKey(int, unsigned short);
extern int libusbkb_popKey(int, unsigned short *);
extern int libusbkb_copyKeyCodes(int, unsigned char, unsigned char, int, unsigned short *, unsigned short *);
/**
 * Result handler of call 2 (read): turns the keyboard report into new key codes with key repeat.
 *
 * Returns -1 when the report's count byte is 0xFF. In raw mode (f0C 1) it copies LED, count and modifiers into the
 * caller's sKbData, keeps the non-zero codes (a single 0 when none), converts them (libusbkb_copyKeyCodes) and returns.
 *
 * Otherwise it first advances the repeat: in state 1, once the count reaches the delay, it empties the ring
 * (libusbkb_clearKeyRing) and queues the repeated key (with a 0 before it when the delay is 2 or more and after it when the
 * rate is), then enters state 2; in state 2 it queues the key again whenever the count reaches the rate. A report
 * without keys returns one queued key (libusbkb_popKey) with the previous LED and modifiers, or none. A first code of 1
 * (roll-over error) stops the repeat and returns no keys. Otherwise it empties the ring; a first code of 0 or changed
 * modifiers returns one 0 key and stops the repeat; else it returns only the codes missing from the previous report
 * D_00529008 (all of them when that was empty), converts them and starts repeating the last one when delay and rate are
 * set. With no new codes it stops the repeat once the repeated key is no longer held. The report is saved as the
 * previous one. Handler 2 of libusbkb_resultHandlerTable.
 *
 * @return 0, or -1 for a bad report
 */
int libusbkb_onReadResult(void) {
    sKbData cur;
    unsigned short heldKeys[64];
    unsigned short key;
    unsigned char *recvBytes = libusbkb_recvBuffer;
    sKbData *out = libusbkb_state.out;
    sKbPort *kbPort = &libusbkb_state.port[libusbkb_state.arg];
    sKbRepeat *repeat = &kbPort->rep;
    int i, j, k;
    if (recvBytes[1] == 0xFF)
        return -1;
    if (kbPort->f0C == 1) {
        out->led = recvBytes[0];
        out->n = recvBytes[1];
        out->mod = recvBytes[2];
        if (out->n == 0)
            return 0;
        j = 0;
        for (i = 0; i < out->n; i++) {
            if (recvBytes[4 + i])
                out->keys[j++] = recvBytes[4 + i];
        }
        out->n = j;
        if (j == 0) {
            out->keys[0] = 0;
            out->n = 1;
        }
        libusbkb_copyKeyCodes(libusbkb_state.arg, out->mod, out->led, out->n, out->keys, out->keys);
        return 0;
    }
    switch (repeat->state) {
    case 1:
        if (repeat->count >= repeat->delay) {
            repeat->count = 0;
            libusbkb_clearKeyRing(libusbkb_state.arg);
            if (repeat->delay >= 2)
                libusbkb_pushKey(libusbkb_state.arg, 0);
            libusbkb_pushKey(libusbkb_state.arg, repeat->last);
            if (repeat->rate >= 2)
                libusbkb_pushKey(libusbkb_state.arg, 0);
            repeat->state = 2;
        }
        break;
    case 2:
        if (repeat->count >= repeat->rate) {
            repeat->count = 0;
            libusbkb_pushKey(libusbkb_state.arg, repeat->last);
            if (repeat->rate >= 2)
                libusbkb_pushKey(libusbkb_state.arg, 0);
        }
        break;
    case 0:
        break;
    }
    if (recvBytes[1] == 0) {
        if (libusbkb_popKey(libusbkb_state.arg, &key) == 0) {
            out->n = 1;
            out->keys[0] = key;
            out->led = libusbkb_state.prev.led;
            out->mod = libusbkb_state.prev.mod;
        } else {
            out->n = 0;
        }
        return 0;
    }
    if (recvBytes[4] == 1) {
        repeat->state = 0;
        out->n = 0;
        return 0;
    }
    cur.led = recvBytes[0];
    cur.n = recvBytes[1];
    cur.mod = recvBytes[2];
    j = 0;
    for (i = 0; i < cur.n; i++) {
        if (recvBytes[4 + i])
            cur.keys[j++] = recvBytes[4 + i];
    }
    cur.n = j;
    libusbkb_clearKeyRing(libusbkb_state.arg);
    if (cur.keys[0] == 0 || cur.mod != libusbkb_state.prev.mod) {
        out->keys[0] = 0;
        out->n = 1;
        repeat->last = 0;
        out->led = cur.led;
        out->mod = cur.mod;
        repeat->state = 0;
    } else {
        if (libusbkb_state.prev.keys[0] != 0) {
            k = 0;
            for (i = 0; i < cur.n; i++) {
                for (j = 0; j < libusbkb_state.prev.n && cur.keys[i] != libusbkb_state.prev.keys[j]; j++)
                    ;
                if (j == libusbkb_state.prev.n)
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
        libusbkb_copyKeyCodes(libusbkb_state.arg, out->mod, out->led, out->n, out->keys, out->keys);
        if (out->n > 0) {
            repeat->last = out->keys[out->n - 1];
            if (repeat->delay > 0 && repeat->rate > 0)
                repeat->state = 1;
        } else {
            libusbkb_copyKeyCodes(libusbkb_state.arg, cur.mod, cur.led, cur.n, cur.keys, heldKeys);
            for (i = 0; i < cur.n && heldKeys[i] != repeat->last; i++)
                ;
            if (i == cur.n) {
                repeat->state = 0;
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
INCLUDE_ASM("seg/seg_2FE810", libusbkb_onGetLocationResult);
#ifdef SKIP_ASM
/**
 * Result handler of call 3 (location): copies the 7-byte reply into the caller's buffer.
 *
 * Fails when byte 6 of the receive buffer libusbkb_recvBuffer is not 0; otherwise copies bytes 0..6 to libusbkb_state.out. Handler 3
 * of libusbkb_resultHandlerTable.
 *
 * @return 0, or -1 when the reply reports an error
 */
int libusbkb_onGetLocationResult(void) {
    unsigned char *recvBytes = libusbkb_recvBuffer;
    unsigned char *locationOut = D_00529098;
    int i;
    if (recvBytes[6])
        return -1;
    i = 6;
    do {
        *locationOut = *recvBytes; i--; recvBytes++; locationOut++;
    } while (i >= 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_onSetLedStatusResult);
#ifdef SKIP_ASM
/**
 * Result handler of call 4 (LED status): checks that the reply echoes the keyboard number sent.
 *
 * Compares byte 1 of the receive buffer with byte 1 of the send buffer. Handler 4 of libusbkb_resultHandlerTable.
 *
 * @return 0 when they match, else -1
 */
int libusbkb_onSetLedStatusResult(void) {
    return D_00529141[0] == D_00529101[0] ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_onSetLedModeResult);
#ifdef SKIP_ASM
/**
 * Result handler of call 5 (LED mode): checks that the reply echoes the keyboard number sent.
 *
 * Same test as libusbkb_onSetLedStatusResult. Handler 5 of libusbkb_resultHandlerTable.
 *
 * @return 0 when they match, else -1
 */
int libusbkb_onSetLedModeResult(void) {
    return D_00529141[0] == D_00529101[0] ? 0 : -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_onVblank);
#ifdef SKIP_ASM
/**
 * Vertical-blank interrupt handler that counts the key-repeat time of every keyboard.
 *
 * For each keyboard with a delay and rate set and a repeat in progress (state 1 or 2) it increments the repeat count.
 * Ends with sync and ei. sceUsbKbInit installs it on INTC 3.
 *
 * @return 0
 */
int libusbkb_onVblank(void) {
    int i;

    for (i = 0; i < libusbkb_state.n; i++) {
        sKbRepeat *repeat = &libusbkb_state.port[i].rep;
        if (repeat->delay > 0 && repeat->rate > 0 && repeat->state != 0 && repeat->state >= 0 && repeat->state < 3)
            repeat->count++;
    }
    SYNC_0810();
    EI_0810();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_clearKeyRing);
#ifdef SKIP_ASM
/**
 * Empties a keyboard's 4-entry key ring and clears its repeat count.
 *
 * @param i  the keyboard number
 * @return 0
 */
int libusbkb_clearKeyRing(int i) {
    sKbRepeat *repeat = &D_00529004[i].rep;
    repeat->n = 0;
    repeat->wr = 0;
    repeat->rd = 0;
    repeat->count = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_pushKey);
#ifdef SKIP_ASM
/**
 * Queues a key code in a keyboard's 4-entry key ring.
 *
 * @param kbIndex  the keyboard number
 * @param keyCode  the key code
 * @return 0, or -1 when the ring is full
 */
int libusbkb_pushKey(int kbIndex, unsigned short keyCode) {
    sKbRepeat *repeat = &D_00529004[kbIndex].rep;
    if (repeat->n >= 4)
        return -1;
    repeat->ring[repeat->wr++] = keyCode;
    if (repeat->wr >= 4)
        repeat->wr = 0;
    repeat->n++;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_popKey);
#ifdef SKIP_ASM
/**
 * Takes the oldest key code from a keyboard's 4-entry key ring.
 *
 * @param kbIndex  the keyboard number
 * @param out  receives the key code
 * @return 0, or -1 when the ring is empty
 */
int libusbkb_popKey(int kbIndex, unsigned short *out) {
    sKbRepeat *repeat = &D_00529004[kbIndex].rep;
    if (repeat->n <= 0)
        return -1;
    *out = repeat->ring[repeat->rd++];
    if (repeat->rd >= 4)
        repeat->rd = 0;
    repeat->n--;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_copyKeyCodes);
#ifdef SKIP_ASM
extern short q_0040D268(int, int, int, unsigned short) __asm__("sceUsbKbCnvRawCode");
/**
 * Copies a keyboard's raw key codes to an output list, converting them when its code type asks.
 *
 * Stops after @p n codes or at a 0 code. With code type 0 (f04) the codes are copied unchanged; with 1 each is
 * converted by sceUsbKbCnvRawCode using the keyboard's arrangement (f08), @p modKeys and @p ledState. Any other code type writes nothing.
 *
 * @param idx  the keyboard number
 * @param modKeys  the modifier-key bits
 * @param ledState  the LED state
 * @param n  the number of codes
 * @param src  the raw codes
 * @param dst  receives the codes (may equal @p src)
 * @return the number of codes written
 */
int libusbkb_copyKeyCodes(int idx, unsigned char modKeys, unsigned char ledState, int n, unsigned short *src, unsigned short *dst) {
    sKbPort *kbPort = &D_00529004[idx];
    int i;
    int keyCount = 0;
    switch (kbPort->f04) {
    case 0:
        i = 0;
        if (n > 0 && src[0] != 0) {
            do {
                dst[i] = src[i];
                i++;
            } while (i < n && src[i] != 0);
        }
        keyCount = i;
        break;
    case 1:
        i = 0;
        if (n > 0 && src[0] != 0) {
            do {
                dst[i] = q_0040D268(kbPort->f08, modKeys, ledState, src[i]);
                i++;
            } while (i < n && src[i] != 0);
        }
        keyCount = i;
        break;
    }
    return keyCount;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbCnvRawCode);
#ifdef SKIP_ASM
extern char D_004983B0[];
extern char D_004983C8[];
extern char D_004987C8[];
extern char D_00498BC8[];
extern void u268_00425C60(char *) __asm__("scePrintf");
extern unsigned short u268_0040D3C0(int, int) __asm__("libusbkb_convertKeypadCode");
/**
 * Converts one raw USB key code into a character code for a key layout.
 *
 * @p mode picks the table D_004983C8, D_004987C8 or D_00498BC8 (each 256 plain entries, then 256 for Shift, used when
 * @p modKeys has a Shift bit, 0x02 or 0x20). Code 0 gives 0 and codes from 0x100 come back unchanged. A table value of 0x4000
 * marks a keypad key, converted by libusbkb_convertKeypadCode with the LED state; 0x8000 returns the raw code with bit 15 set. With
 * Caps Lock (LED bit 1) lower-case letters turn upper-case, and back to lower-case when Shift is held too.
 *
 * Likely bug: another @p mode prints "Arrangement error!" and then reads the null table for codes 1..0xFF.
 *
 * @param mode  the layout (0..2)
 * @param modKeys  the modifier-key bits
 * @param ledState  the LED state
 * @param ch  the raw key code
 * @return the character code
 */
unsigned short sceUsbKbCnvRawCode(int mode, int modKeys, int ledState, unsigned short ch) {
    unsigned short *tbl = 0;
    unsigned short charCode;
    int up = (modKeys & 0x22) != 0;
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
    charCode = tbl[ch];
    if (charCode == 0x4000)
        charCode = u268_0040D3C0(ledState & 0xFF, ch & 0xFF);
    if (charCode == 0x8000)
        charCode = ch | 0x8000;
    if ((unsigned short)(charCode - 97) < 26 && (ledState & 2))
        charCode -= 0x20;
    if ((unsigned short)(charCode - 65) < 26) {
        if (ledState & 2) {
            if (up)
                charCode += 0x20;
        }
    }
    return charCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", libusbkb_convertKeypadCode);
#ifdef SKIP_ASM
struct K_D3C0 { unsigned char k; unsigned char p; unsigned short v; };
extern struct K_D3C0 libusbkb_keypadCodeTable[2][21];
/**
 * Converts a keypad key's raw code for the Num Lock state.
 *
 * Looks the code up in the 21-entry table libusbkb_keypadCodeTable[@p ledState & 1] (Num Lock is LED bit 0) and returns the matching code
 * with bit 0x4000 set; codes not in the table come back unchanged.
 *
 * @param ledState  the LED state
 * @param rawCode  the raw key code
 * @return the converted code
 */
unsigned short libusbkb_convertKeypadCode(int ledState, unsigned char rawCode) {
    unsigned i;
    int keyCode = rawCode;
    for (i = 0; i < 21; i++) {
        if (rawCode == libusbkb_keypadCodeTable[ledState & 1][i].k) {
            keyCode = libusbkb_keypadCodeTable[ledState & 1][i].v | 0x4000;
            break;
        }
    }
    return keyCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbSetReadMode);
#ifdef SKIP_ASM
/* Sets a keyboard's read mode, clearing its key repeat and the previous report's key count when it changes.
 * (sceUsbKbSetReadMode passes &port[kbIndex]: an inline function's argument keeps retail's index-first add.) */
static inline void kbPortSetReadMode(sKbPort *kbPort, unsigned readMode) {
    if (readMode != kbPort->f0C) {
        kbPort->rep.count = 0;
        kbPort->rep.wr = 0;
        kbPort->rep.rd = 0;
        libusbkb_state.prev.n = 0;
        kbPort->rep.last = 0;
    }
    kbPort->f0C = readMode;
}

/**
 * Switches a keyboard between converted key codes and raw reports.
 *
 * When the mode changes it clears the keyboard's repeat count, ring positions and repeated key and the previous
 * report's key count. libusbkb_onReadResult returns raw reports while the mode (f0C) is 1. usbkeyboard_setupKeyboard sets 0.
 *
 * @param kbIndex  the keyboard number
 * @param readMode  0 converted keys, 1 raw reports
 * @return 0, -11 for a bad keyboard or -12 for a bad mode
 */
int sceUsbKbSetReadMode(int kbIndex, unsigned readMode) {
    if (kbIndex >= libusbkb_state.n)
        return -11;
    if (readMode >= 2)
        return -12;
    kbPortSetReadMode(&libusbkb_state.port[kbIndex], readMode);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", sceUsbKbClearRbuf);
#ifdef SKIP_ASM
extern void q_00425C60(char *, int) __asm__("scePrintf");
/**
 * Clears a keyboard's input buffer on the IOP side and its key ring and repeat state here.
 *
 * Takes the semaphore libusbkb_rpcSema and sends RPC 6 with the keyboard number asynchronously (libusbkb_onRpcDone releases the
 * semaphore when it ends), then resets the keyboard's repeat state, repeated key and ring.
 *
 * Quirks: a failed call prints the message of the read call (str_sceUsbKbRead_sceSifCallRpc_pct_d) and returns with the semaphore still taken.
 *
 * @param kbIndex  the keyboard number
 * @return 0, -1 before initialisation, -11 for a bad keyboard or -2 when the call failed
 */
int sceUsbKbClearRbuf(unsigned kbIndex) {
    int rpcResult;
    sKbPort *kbPort;
    if (libusbkb_rpcSema == -1)
        return -1;
    if (kbIndex >= libusbkb_state.n)
        return -11;
    libusbkb_sendBuffer[0] = kbIndex;
    WaitSema(libusbkb_rpcSema);
    rpcResult = sceSifCallRpc(&libusbkb_client, 6, 1, libusbkb_sendBuffer, 16, libusbkb_recvBuffer, 0, libusbkb_onRpcDone, 0);
    if (rpcResult != 0) {
        q_00425C60(str_sceUsbKbRead_sceSifCallRpc_pct_d, rpcResult);
        return -2;
    }
    kbPort = &libusbkb_state.port[kbIndex];
    kbPort->rep.state = 0;
    kbPort->rep.count = 0;
    kbPort->rep.last = 0;
    kbPort->rep.wr = 0;
    kbPort->rep.rd = 0;
    kbPort->rep.n = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", tanf);
#ifdef SKIP_ASM
extern int __ieee754_rem_pio2f(float x, float *y);
extern float __kernel_tanf(float x, float y, int iy);
typedef union { float f; int bits; } FloatBits_D588;
#define FLOAT_TO_BITS_D588(i, d) do { FloatBits_D588 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
/**
 * Returns the tangent of a float angle.
 *
 * Up to pi/4 (bits 0x3F490FDA) it calls __kernel_tanf(x, 0, 1) directly; infinity and NaN give NaN (x - x). Larger
 * angles are reduced by __ieee754_rem_pio2f to a remainder and quadrant, and __kernel_tanf gets the remainder with 1 for even
 * quadrants or -1 (minus the cotangent) for odd ones. cPS2GraphicsMan_SetViewportPerspective takes the reciprocal of
 * its result for the field of view.
 *
 * @param x  the angle in radians
 * @return the tangent
 */
float tanf(float x) {
    float y[2];
    int hx, ix;
    FLOAT_TO_BITS_D588(hx, x);
    ix = hx & 0x7fffffff;
    if (ix <= 0x3f490fda) {
        return __kernel_tanf(x, 0.0f, 1);
    } else if (ix > 0x7f7fffff) {
        return x - x;
    } else {
        int n = __ieee754_rem_pio2f(x, y);
        return __kernel_tanf(y[0], y[1], 1 - ((n & 1) << 1));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", log);
#ifdef SKIP_ASM
struct E40D610 { int type; char *name; double arg1; double arg2; double retval; int err; };
extern const int D_00499688[]; /* fdlibm _LIB_VERSION (newlib: _CONST) */
extern char D_00499070[];
extern double __infinity[];
extern double core_40D610(double) __asm__("__ieee754_log");
extern int isn_40D610(double) __asm__("isnan");
extern int cmp_40D610(double, double) __asm__("dpcmp");
extern double dpsub(double, double);
extern int matherr_40D610(struct E40D610 *) __asm__("matherr");
extern int *__errno(void);
/**
 * Returns the natural logarithm of a double, with the math library's error handling.
 *
 * Computes it with __ieee754_log. Unless the library mode D_00499688 is -1 (IEEE), @p x is NaN (isnan) or
 * positive, it fills an error record (name D_00499070, both arguments @p x, result -3.4e38 in SVID mode 0 or 0 -
 * __infinity otherwise). For 0 the type is 2 (singularity) and errno (__errno) becomes 34 in POSIX mode 2, else
 * 33 when matherr returns 0; below 0 the type is 1 (domain) with errno 33. A non-zero err field overrides errno.
 * The double arithmetic runs through the soft-float routines (dpcmp, dpsub). cPS2GraphicsMan_AddTexture uses it to pick
 * a texture's mip level.
 *
 * @param x  the value
 * @return the logarithm, or the error record's result
 */
double log(double x) {
    struct E40D610 mathErr;
    double zero;
    double z = core_40D610(x);
    int ver = D_00499688[0];
    if (ver == -1 || isn_40D610(x) || (zero = 0.0, cmp_40D610(x, zero) > 0))
        return z;
    mathErr.err = 0;
    mathErr.name = D_00499070;
    mathErr.arg2 = mathErr.arg1 = x;
    if (ver == 0)
        mathErr.retval = -3.4028234663852886e+38;
    else
        mathErr.retval = dpsub(zero, __infinity[0]);
    if (cmp_40D610(x, 0.0) == 0) {
        mathErr.type = 2;
        if (D_00499688[0] == 2)
            *__errno() = 34;
        else if (!matherr_40D610(&mathErr))
            *__errno() = 33;
    } else {
        mathErr.type = 1;
        if (D_00499688[0] == 2)
            *__errno() = 33;
        else if (!matherr_40D610(&mathErr))
            *__errno() = 33;
    }
    if (mathErr.err != 0)
        *__errno() = mathErr.err;
    return mathErr.retval;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", expf);
#ifdef SKIP_ASM
/* newlib libm expf wrapper (wf_exp.c; fdlibm; notice at isnan).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
struct exception_D758 { int type; char *name; double arg1; double arg2; double retval; int err; };
extern float __ieee754_expf(float);                                  /* __ieee754_expf */
extern int finitef(float);                                    /* finitef */
extern int matherr_D758(struct exception_D758 *) __asm__("matherr");
enum fdlibm_version_D758 { fdlibm_ieee_D758 = -1, fdlibm_svid_D758, fdlibm_xopen_D758, fdlibm_posix_D758 };
extern const enum fdlibm_version_D758 fdlib_version_D758 __asm__("D_00499688");
#define _LIB_VERSION_D758 fdlib_version_D758
#define errno_D758 (*__errno())
#define HUGE_D758 ((float)3.40282346638528860e+38)
#define HUGE_VAL_D758 (__infinity[0])
static const float
o_threshold_D758=  8.8721679688e+01,  /* 0x42b17180 */
u_threshold_D758= -1.0397208405e+02;  /* 0xc2cff1b5 */

	/**
	 * Returns e raised to a float power, with the math library's overflow and underflow handling.
	 *
	 * Computes it with __ieee754_expf. Unless the library mode D_00499688 is -1, a finite @p x (finitef) above 88.72
	 * fills an overflow record (type 3, result 3.4e38 in SVID mode or HUGE_VAL) and one below -103.97 an underflow record
	 * (type 4, result 0); errno (__errno) becomes 34 in POSIX mode or when matherr returns 0, a non-zero err
	 * field overrides it, and the record's result is returned.
	 *
	 * Callers include cLoadTimer_update, the input-map expression code (cInputExec_execute),
	 * cChunkResolve_calcMaxLightDistance and cRenderStateMan_calculateIrradianceBlend.
	 *
	 * @param x  the exponent
	 * @return the power, or the error record's result
	 */
	float expf(float x)		/* wrapper expf */
{
	float z;
	struct exception_D758 exc;
	z = __ieee754_expf(x);
	if(_LIB_VERSION_D758 == -1) return z;
	if(finitef(x)) {
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
INCLUDE_ASM("seg/seg_2FE810", fmodf);
#ifdef SKIP_ASM
/* newlib libm fmodf wrapper (wf_fmod.c; fdlibm; notice at isnan).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
struct exception_D8D0 { int type; char *name; double arg1; double arg2; double retval; int err; };
enum fdlibm_version_D8D0 { fdlibm_ieee_D8D0 = -1, fdlibm_svid_D8D0, fdlibm_xopen_D8D0, fdlibm_posix_D8D0 };
extern const enum fdlibm_version_D8D0 fdlib_version_D8D0 __asm__("D_00499688");
extern float __ieee754_fmodf(float, float);                           /* __ieee754_fmodf */
extern int isnanf(float);                                    /* isnanf */
extern int matherr_D8D0(struct exception_D8D0 *) __asm__("matherr");
#define _LIB_VERSION_D8D0 fdlib_version_D8D0
#define errno_D8D0 (*__errno())

	/**
	 * Returns the float remainder of x / y, with the math library's error handling for y = 0.
	 *
	 * Computes it with __ieee754_fmodf. Unless the library mode is -1 or either argument is NaN (isnanf), y = 0 fills
	 * a domain record (type 1, result x in SVID mode, else NaN) and sets errno 33 in POSIX mode or when matherr
	 * returns 0. cSongInstance_GetTimeToNextBeat (bank monitor) uses it for the position within a beat.
	 *
	 * @param x  the dividend
	 * @param y  the divisor
	 * @return the remainder, or the error record's result
	 */
	float fmodf(float x, float y)	/* wrapper fmodf */
{
	float z;
	struct exception_D8D0 exc;
	z = __ieee754_fmodf(x,y);
	if(_LIB_VERSION_D8D0 == fdlibm_ieee_D8D0 ||isnanf(y)||isnanf(x)) return z;
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
INCLUDE_ASM("seg/seg_2FE810", logf);
#ifdef SKIP_ASM
/* newlib libm logf wrapper (wf_log.c; fdlibm; notice at isnan).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
struct exception_DA10 { int type; char *name; double arg1; double arg2; double retval; int err; };
enum fdlibm_version_DA10 { fdlibm_ieee_DA10 = -1, fdlibm_svid_DA10, fdlibm_xopen_DA10, fdlibm_posix_DA10 };
extern const enum fdlibm_version_DA10 fdlib_version_DA10 __asm__("D_00499688");
extern float __ieee754_logf(float);                                  /* __ieee754_logf */
extern int isnanf(float);                                    /* isnanf */
extern int matherr_DA10(struct exception_DA10 *) __asm__("matherr");
#define _LIB_VERSION_DA10 fdlib_version_DA10
#define errno_DA10 (*__errno())
#define HUGE_DA10 ((float)3.40282346638528860e+38)
#define HUGE_VAL_DA10 (__infinity[0])

	/**
	 * Returns the natural logarithm of a float, with the math library's error handling.
	 *
	 * Computes it with __ieee754_logf. Unless the library mode is -1, @p x is NaN (isnanf) or positive, it fills an
	 * error record (result -3.4e38 in SVID mode, else -HUGE_VAL): for 0 type 2 with errno 34 in POSIX mode (33 when
	 * matherr returns 0), below 0 type 1 with errno 33. cRenderStateMan_calculateIrradianceBlend uses it.
	 *
	 * @param x  the value
	 * @return the logarithm, or the error record's result
	 */
	float logf(float x)		/* wrapper logf */
{
	float z;
	struct exception_DA10 exc;
	z = __ieee754_logf(x);
	if(_LIB_VERSION_DA10 == fdlibm_ieee_DA10 || isnanf(x) || x > (float)0.0) return z;
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
INCLUDE_ASM("seg/seg_2FE810", __ieee754_log);
#ifdef SKIP_ASM
/* newlib libm __ieee754_log (e_log.c; fdlibm; notice at isnan) */
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
	double __ieee754_log(double x)
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
INCLUDE_ASM("seg/seg_2FE810", __ieee754_expf);
#ifdef SKIP_ASM
/* newlib libm __ieee754_expf (ef_exp.c; fdlibm; notice at isnan).
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
	float __ieee754_expf(float x)	/* default IEEE double exp */
{
	float y,hi,lo,polyCorr,t;
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
	polyCorr  = x - t*(P1_E048+t*(P2_E048+t*(P3_E048+t*(P4_E048+t*P5_E048))));
	if(k==0) 	return one_E048-((x*polyCorr)/(polyCorr-(float)2.0)-x);
	else 		y = one_E048-((lo-(x*polyCorr)/((float)2.0-polyCorr))-hi);
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
INCLUDE_ASM("seg/seg_2FE810", __ieee754_fmodf);
#ifdef SKIP_ASM
// FAKEMATCH: `while((unsigned int)n-- > 0)` for fdlibm's `while(n--)` (the same test for every n); the plain
// form is strength-reduced to `--n != -1`; plain form 93.24%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
/* newlib libm __ieee754_fmodf (ef_fmod.c; fdlibm; notice at isnan).
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
extern const float ef_fmod_Zero[];	/* Zero[] = {0.0, -0.0} */
#define Zero_E300 ef_fmod_Zero

	/**
	 * Computes the float remainder of x / y with the sign of x, without error handling.
	 *
	 * Works on the integer bits: y = 0, infinite x or NaN y give NaN; |x| < |y| returns x; |x| = |y| a signed zero from
	 * ef_fmod_Zero. Otherwise it aligns both mantissas (subnormals included), subtracts by shift and compare one bit at a
	 * time and rebuilds the result.
	 *
	 * @param x  the dividend
	 * @param y  the divisor
	 * @return the remainder
	 */
	float __ieee754_fmodf(float x, float y)
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
INCLUDE_ASM("seg/seg_2FE810", __ieee754_logf);
#ifdef SKIP_ASM
/* newlib libm __ieee754_logf (ef_log.c; fdlibm; notice at isnan).
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
	 * Same method as __ieee754_log in single precision (Lg1..Lg7, ln2 high and low parts); 0 gives minus infinity, negative
	 * values NaN, subnormals are scaled by 2^25 first.
	 *
	 * @param x  the value
	 * @return the logarithm
	 */
	float __ieee754_logf(float x)
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
INCLUDE_ASM("seg/seg_2FE810", __ieee754_rem_pio2f);
#ifdef SKIP_ASM
/* newlib libm __ieee754_rem_pio2f (ef_rem_pio2.c; fdlibm; notice at isnan).
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

extern float fabsf_a(float) __asm__("fabsf");	/* a call, not the builtin (gcc inlines fabsf as abs.s) */
extern int __kernel_rem_pio2f(float *, float *, int, int, int, const int *);	/* __kernel_rem_pio2f */
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
	 * __kernel_rem_pio2f with the bits of 2/pi (D_00499228). Negative angles negate remainder and quadrant.
	 *
	 * @param x  the angle
	 * @param y  receives the remainder as two floats
	 * @return the quadrant number n (only n & 3 matters)
	 */
	int __ieee754_rem_pio2f(float x, float *y)
{
	float z,w,reduceBase,remainder,fn;
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
	    reduceBase  = fabsf_a(x);
	    n  = (int) (reduceBase*invpio2_E878+half_E878);
	    fn = (float)n;
	    remainder  = reduceBase-fn*pio2_1_E878;
	    w  = fn*pio2_1t_E878;	/* 1st round good to 40 bit */
	    if(n<32&&(ix&0xffffff00)!=npio2_hw_E878[n-1]) {
		y[0] = remainder-w;	/* quick check no cancellation */
	    } else {
	        unsigned int high;
	        j  = ix>>23;
	        y[0] = remainder-w;
		GET_FLOAT_WORD_E878(high,y[0]);
	        i = j-((high>>23)&0xff);
	        if(i>8) {  /* 2nd iteration needed, good to 57 */
		    reduceBase  = remainder;
		    w  = fn*pio2_2_E878;
		    remainder  = reduceBase-w;
		    w  = fn*pio2_2t_E878-((reduceBase-remainder)-w);
		    y[0] = remainder-w;
		    GET_FLOAT_WORD_E878(high,y[0]);
		    i = j-((high>>23)&0xff);
		    if(i>25)  {	/* 3rd iteration need, 74 bits acc */
		    	reduceBase  = remainder;	/* will cover all possible cases */
		    	w  = fn*pio2_3_E878;
		    	remainder  = reduceBase-w;
		    	w  = fn*pio2_3t_E878-((reduceBase-remainder)-w);
		    	y[0] = remainder-w;
		    }
		}
	    }
	    y[1] = (remainder-y[0])-w;
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
	n  =  __kernel_rem_pio2f(tx,y,e0,nx,2,two_over_pi_E878);
	if(hx<0) {y[0] = -y[0]; y[1] = -y[1]; return -n;}
	return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", __kernel_rem_pio2f);
#ifdef SKIP_ASM
/* newlib libm __kernel_rem_pio2f (kf_rem_pio2.c; fdlibm; notice at isnan).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
extern float scalbnf(float, int);	/* scalbnf */
extern float floorf(float);	/* floorf */
#define scalbnf_EC58 scalbnf
#define floorf_EC58 floorf
/* the file's static tables, at their retail addresses */
extern const int init_jk[];	/* init_jk[] = {4,7,9} */
extern const float PIo2[];	/* PIo2[] */
#define init_jk_EC58 init_jk
#define PIo2_EC58 PIo2

static const float
zero_EC58   = 0.0,
one_EC58    = 1.0,
two8_EC58   =  2.5600000000e+02, /* 0x43800000 */
twon8_EC58  =  3.9062500000e-03; /* 0x3b800000 */

	/**
	 * Reduces a huge float angle modulo pi/2 by multiplying it with enough bits of 2/pi.
	 *
	 * @p x holds the angle as @p nx 8-bit chunks with @p xExponent the exponent of the first. It multiplies them with terms of @p
	 * ipio2 (as floats), distils the products into 8-bit integers, takes the integer part modulo 8 as the quadrant and
	 * recomputes with more terms while the fraction has lost precision. The fraction goes to @p y in one, two or three
	 * floats depending on @p prec (term counts init_jk, pi/2 pieces PIo2). Uses scalbnf and floorf.
	 * __ieee754_rem_pio2f calls it.
	 *
	 * @param x  the input chunks
	 * @param y  receives the remainder
	 * @param xExponent  the exponent of x[0]
	 * @param nx  the number of chunks
	 * @param prec  the precision class (0..3)
	 * @param ipio2  the integer bits of 2/pi
	 * @return the quadrant (0..7)
	 */
	int __kernel_rem_pio2f(float *x, float *y, int xExponent, int nx, int prec, const int *ipio2)
{
	int jz,jx,jv,jp,jk,carry,n,iq[20],i,j,k,lastTerm,qExponent,ih;
	float z,fw,piTerms[20],fq[20],products[20];

    /* initialize jk*/
	jk = init_jk_EC58[prec];
	jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
	jx =  nx-1;
	jv = (xExponent-3)/8; if(jv<0) jv=0;
	qExponent =  xExponent-8*(jv+1);

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
	j = jv-jx; lastTerm = jx+jk;
	for(i=0;i<=lastTerm;i++,j++) piTerms[i] = (j<0)? zero_EC58 : (float) ipio2[j];

    /* compute q[0],q[1],...q[jk] */
	for (i=0;i<=jk;i++) {
	    for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*piTerms[jx+i-j]; products[i] = fw;
	}

	jz = jk;
recompute:
    /* distill q[] into iq[] reversingly */
	for(i=0,j=jz,z=products[jz];j>0;i++,j--) {
	    fw    =  (float)((int)(twon8_EC58* z));
	    iq[i] =  (int)(z-two8_EC58*fw);
	    z     =  products[j-1]+fw;
	}

    /* compute n */
	z  = scalbnf_EC58(z,(int)qExponent);	/* actual value of z */
	z -= (float)8.0*floorf_EC58(z*(float)0.125);	/* trim off integer >= 8 */
	n  = (int) z;
	z -= (float)n;
	ih = 0;
	if(qExponent>0) {	/* need iq[jz-1] to determine n */
	    i  = (iq[jz-1]>>(8-qExponent)); n += i;
	    iq[jz-1] -= i<<(8-qExponent);
	    ih = iq[jz-1]>>(7-qExponent);
	} 
	else if(qExponent==0) ih = iq[jz-1]>>8;
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
	    if(qExponent>0) {		/* rare case: chance is 1 in 12 */
	        switch(qExponent) {
	        case 1:
	    	   iq[jz-1] &= 0x7f; break;
	    	case 2:
	    	   iq[jz-1] &= 0x3f; break;
	        }
	    }
	    if(ih==2) {
		z = one_EC58 - z;
		if(carry!=0) z -= scalbnf_EC58(one_EC58,(int)qExponent);
	    }
	}

    /* check if recomputation is needed */
	if(z==zero_EC58) {
	    j = 0;
	    for (i=jz-1;i>=jk;i--) j |= iq[i];
	    if(j==0) { /* need recomputation */
		for(k=1;iq[jk-k]==0;k++);   /* k = no. of terms needed */

		for(i=jz+1;i<=jz+k;i++) {   /* add q[jz+1] to q[jz+k] */
		    piTerms[jx+i] = (float) ipio2[jv+i];
		    for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*piTerms[jx+i-j];
		    products[i] = fw;
		}
		jz += k;
		goto recompute;
	    }
	}

    /* chop off zero terms */
	if(z==(float)0.0) {
	    jz -= 1; qExponent -= 8;
	    while(iq[jz]==0) { jz--; qExponent-=8;}
	} else { /* break z into 8-bit if necessary */
	    z = scalbnf_EC58(z,-(int)qExponent);
	    if(z>=two8_EC58) { 
		fw = (float)((int)(twon8_EC58*z));
		iq[jz] = (int)(z-two8_EC58*fw);
		jz += 1; qExponent += 8;
		iq[jz] = (int) fw;
	    } else iq[jz] = (int) z ;
	}

    /* convert integer "bit" chunk to floating-point value */
	fw = scalbnf_EC58(one_EC58,(int)qExponent);
	for(i=jz;i>=0;i--) {
	    products[i] = fw*(float)iq[i]; fw*=twon8_EC58;
	}

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
	for(i=jz;i>=0;i--) {
	    for(fw=0.0,k=0;k<=jp&&k<=jz-i;k++) fw += PIo2_EC58[k]*products[i+k];
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
INCLUDE_ASM("seg/seg_2FE810", __kernel_tanf);
#ifdef SKIP_ASM
/* newlib libm __kernel_tanf (kf_tan.c; fdlibm; notice at isnan).
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

extern float fabsf_a(float) __asm__("fabsf");	/* a call, not the builtin (gcc inlines fabsf as abs.s) */
#define fabsf_F5A8 fabsf_a
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
	float __kernel_tanf(float x, float y, int iy)
{
	float z,polyTail,v,w,s;
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
	polyTail = T_F5A8[1]+w*(T_F5A8[3]+w*(T_F5A8[5]+w*(T_F5A8[7]+w*(T_F5A8[9]+w*T_F5A8[11]))));
	v = z*(T_F5A8[2]+w*(T_F5A8[4]+w*(T_F5A8[6]+w*(T_F5A8[8]+w*(T_F5A8[10]+w*T_F5A8[12])))));
	s = z*x;
	polyTail = y + z*(s*(polyTail+v)+y);
	polyTail += T_F5A8[0]*s;
	w = x+polyTail;
	if(ix>=0x3f2ca140) {
	    v = (float)iy;
	    return (float)(1-((hx>>30)&2))*(v-(float)2.0*(x-(w*w/(w+v)-polyTail)));
	}
	if(iy==1) return w;
	else {		/* if allow error up to 2 ulp,
			   simply return -1.0/(x+r) here */
     /*  compute -1.0/(x+r) accurately */
	    float negRecip,t;
	    int i;
	    z  = w;
	    GET_FLOAT_WORD_F5A8(i,z);
	    SET_FLOAT_WORD_F5A8(z,i&0xfffff000);
	    v  = polyTail-(z - x); 	/* z+v = r+x */
	    t = negRecip  = -(float)1.0/w;	/* a = -1.0/w */
	    GET_FLOAT_WORD_F5A8(i,t);
	    SET_FLOAT_WORD_F5A8(t,i&0xfffff000);
	    s  = (float)1.0+t*z;
	    return t+negRecip*(s+t*v);
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", isnan);
#ifdef SKIP_ASM
/* newlib libm isnan (fdlibm s_isnan.c): the EE has no double hardware, so the double arrives in one 64-bit GPR.
 * The bodies of isnan, finitef (finitef) and isnanf (isnanf) and the word-access macros are fdlibm's:
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
int isnan(double x) {
    int hx, lx;
    EXTRACT_WORDS(hx, lx, x);
    hx &= 0x7fffffff;
    hx |= (unsigned int)(lx | (-lx)) >> 31;
    hx = 0x7ff00000 - hx;
    return (int)(((unsigned int)(hx)) >> 31);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", matherr);
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
int matherr(long *errInfo) {
    long firstArg = errInfo[1];
    dpcmp(firstArg, firstArg);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", fabsf);
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
float fabsf(float x) {
    int ix;
    FLOAT_TO_BITS_F8A0(ix, x);
    BITS_TO_FLOAT_F8A0(x, ix & 0x7fffffff);
    return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", finitef);
#ifdef SKIP_ASM
/* newlib libm finitef (fdlibm sf_finite.c; notice at isnan) */
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
int finitef(float x) {
    int ix;
    GET_FLOAT_WORD(ix, x);
    return (int)((unsigned int)((ix & 0x7fffffff) - 0x7f800000) >> 31);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", floorf);
#ifdef SKIP_ASM
/* newlib libm floorf (sf_floor.c; fdlibm; notice at isnan).
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com. */
/* GET_FLOAT_WORD and ieee_float_shape_type: defined with finitef (finitef) above.
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
	float floorf(float x)
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
INCLUDE_ASM("seg/seg_2FE810", isnanf);
#ifdef SKIP_ASM
/* newlib libm isnanf (fdlibm sf_isnan.c; notice at isnan) */
/* GET_FLOAT_WORD: defined with finitef (finitef) above */
/**
 * Returns whether a float is NaN.
 *
 * @param x  the value
 * @return 1 for NaN, else 0
 */
int isnanf(float x) {
    int ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;
    ix = 0x7f800000 - ix;
    return (int)(((unsigned int)(ix)) >> 31);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", scalbnf);
#ifdef SKIP_ASM
extern float copysignf_F9F8(float, float) __asm__("copysignf");
typedef union { float f; int bits; } FloatBits_F9F8;
#define FLOAT_TO_BITS_F9F8(i, d) do { FloatBits_F9F8 fb_; fb_.f = (d); (i) = fb_.bits; } while (0)
#define BITS_TO_FLOAT_F9F8(d, i) do { FloatBits_F9F8 fb_; fb_.bits = (i); (d) = fb_.f; } while (0)
static const float k_2pow25_F9F8 = 3.3554432e+07f, k_2powm25_F9F8 = 2.9802322e-08f, k_big_F9F8 = 1.0e+30f, k_small_F9F8 = 1.0e-30f;
/**
 * Returns x * 2^n by adjusting the float's exponent bits.
 *
 * Subnormals are scaled by 2^25 first (a very negative @p n then gives 1e-30 * x). Infinity and NaN return x + x.
 * Results past the range give 1e30 or 1e-30 times itself with x's sign (copysignf); results that become subnormal
 * are built with 25 extra exponent steps and scaled back.
 *
 * @param x  the value
 * @param n  the power of two
 * @return the scaled value
 */
float scalbnf(float x, int n) {
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
INCLUDE_ASM("seg/seg_2FE810", copysignf);
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
float copysignf(float x, float y) {
    int ix, iy;
    FLOAT_TO_BITS_FB58(ix, x);
    FLOAT_TO_BITS_FB58(iy, y);
    BITS_TO_FLOAT_FB58(x, (ix & 0x7fffffff) | (iy & 0x80000000));
    return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", __do_global_dtors);
#ifdef SKIP_ASM
extern void (**__do_global_dtors_p[])(void);
/**
 * Runs the static destructors that the list pointer __do_global_dtors_p walks, in order, once each.
 *
 * The pointer walks a null-terminated list and is advanced before each call, so a destructor that ends the program does
 * not run twice. exit calls it first, passing its status, which is ignored.
 *
 * @param exitStatus  the status of the caller (unused)
 */
void __do_global_dtors(int exitStatus) {
    void (**dtorSlot)(void);
    while (*__do_global_dtors_p[0]) {
        dtorSlot = __do_global_dtors_p[0];
        dtorSlot++;
        __do_global_dtors_p[0] = dtorSlot;
        dtorSlot[-1]();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", __do_global_ctors);
#ifdef SKIP_ASM
typedef void (*func_ptr)(void);
extern func_ptr __CTOR_LIST__[0];



/* libgcc2's __do_global_ctors: DO_GLOBAL_CTORS_BODY from gcc's gbl-ctors.h, a do { } while (0) macro (the plain body
   scores 98.52; its loop notes shape the register allocation) */
#define DO_GLOBAL_CTORS_BODY_FBE0                                       \
do {                                                                    \
  unsigned long nptrs = (unsigned long) __CTOR_LIST__[0];                  \
  unsigned i;                                                           \
  if (nptrs == (unsigned long)-1)                                       \
    for (nptrs = 0; __CTOR_LIST__[nptrs + 1] != 0; nptrs++);               \
  for (i = nptrs; i >= 1; i--)                                          \
    __CTOR_LIST__[i] ();                                                   \
} while (0)

/**
 * Runs the static constructors listed in __CTOR_LIST__, last to first.
 *
 * The list's first word is the count, or -1 when the list is null-terminated (then it is counted first).
 */
void __do_global_ctors(void)
{
  DO_GLOBAL_CTORS_BODY_FBE0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", __main);
#ifdef SKIP_ASM
extern int __main_initialized[];
extern void __do_global_ctors(void);
/**
 * Runs the static constructors once (__do_global_ctors), guarded by the flag __main_initialized.
 *
 * main (main_impl) calls it first; g++ inserts this call at the start of main.
 */
void __main(void) {
    if (__main_initialized[0] == 0) {
        __main_initialized[0] = 1;
        __do_global_ctors();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FE810", __divdi3);
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

DItype __divdi3 (DItype u, DItype v) __asm__("__divdi3");

UDItype __udivmoddi4 ();

/**
 * Divides two signed 64-bit integers, truncating toward zero (GCC runtime helper).
 *
 * Negates negative operands while tracking the result's sign, divides the magnitudes with the inlined unsigned divider
 * (__udivmoddi4) and negates the quotient when the signs differed. cDynamicMemMan_sYSTRIm and the printf core
 * libkernl_formatOutput call it.
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
