#include "common.h"
#include "seg/newlib_stat.h"

// PORT: PS2 kernel syscall stub: the syscall number goes in $3. Defined under a second C name with an
// asm label because this file declares these symbols with their argument lists elsewhere.
#define SYSCALL_STUB(name, n) \
    void name##_syscall(void) __asm__(#name); \
    void name##_syscall(void) { register int syscallNo __asm__("$3") = (n); __asm__ volatile("syscall" : : "r"(syscallNo)); }

// PORT: PS2-only COP0 (read the Status register; ee-gcc 2.9 needs noreorder so gas adds no nop).
#define MFC0_STATUS_4248(v) __asm__ volatile(".set push\n\t.set noreorder\n\tmfc0 %0, $12\n\t.set pop" : "=r"(v))
// PORT: PS2-only COP0 (enable / disable interrupts).
#define EI_4248() __asm__ volatile("ei")
#define DI_4248() __asm__ volatile("di")
// PORT: PS2-only memory barrier.
#define SYNC_4248() __asm__ volatile("sync")
#define SYNCP_4248() __asm__ volatile("sync.p")
// PORT: PS2-only COP0 (write the TLB Wired register).
#define MTC0_WIRED_4248(v) __asm__ volatile("mtc0 %z0, $6\n\tsync.p" : : "rJ"(v))

/* SIF RPC client data (the SDK's sceSifClientData, 0x28 bytes); `server` is non-zero once bound.
   sceSifBindRpc / sceSifCheckStatRpc in this unit are the implementation. */
typedef struct {
    int rpcd[4];
    unsigned int command;
    void *buff;
    void *cbuff;
    void (*end_function)(void *);
    void *end_param;
    void *server;
} sSifClient;
extern int sceSifBindRpc(sSifClient *cd, unsigned int rpc_number, int mode);
extern int sceSifCallRpc(sSifClient *cd, int fno, int mode, void *send, int ssize, void *recv, int rsize,
                         void (*end_function)(void *), void *end_param);
extern sSifClient D_0052EE00;   /* fileio */
extern sSifClient D_0052EEC0;
extern sSifClient D_0052F280;


/* 4-byte server version tags returned by the fileio and loadfile RPC servers */
typedef struct { char b[4]; } sRpcVer;
extern sRpcVer D_0052EE28;      /* fileio (follows D_0052EE00) */
extern sRpcVer D_0052F2A8;      /* loadfile (follows D_0052F280) */


/* loadfile RPC (0x80000006) command block D_0052F080, 0x200 bytes, sent to D_0052F280 */
typedef struct { char b[0xFC]; } sLfData;
typedef struct { int f0; int f4; char n1[0xFC]; char n2[0xFC]; } sLfCmd;
typedef struct { union { unsigned char b; unsigned short h; int w; } f0; int f4; } sLfGetCmd;            /* 3 */
typedef struct { int f0; int f4; union { unsigned char b; unsigned short h; unsigned w; } f8; } sLfSetCmd; /* 2 */
typedef union {
    sLfCmd cmd;                 /* 6..10 and the name calls */
    sLfGetCmd get;
    sLfSetCmd set;
    sRpcVer ver;                /* 0xFF reply */
} uLfCmd;
extern uLfCmd D_0052F080;


/* fileio descriptor table: 32 entries of {IOP fd, open flags (0 = free), ...} */
typedef struct { int f0; int f4; int f8; int fC; } sFioFile;
extern sFioFile D_0052EC00[32];
extern sFioFile *func_00427498(void);
extern sFioFile *func_00427520(unsigned fd);


/* fileio RPC buffers */
typedef struct { void (*fn)(int); int arg; } sFioCb;
extern sFioCb D_0052EE80;      /* user callback run by func_00427A60 (set by func_004279C8) */
extern char D_0052E340[];      /* RPC receive buffer */
extern char D_0052E380[];      /* 0x440-byte read-ahead buffers */
extern char D_0052EE40[];
extern int D_0052D6C0[];


/* byte ring buffer (header + data, wraps at size + 16) */
/* count is shared with the RPC callback that fills the ring (volatile in every user) */
typedef struct { int size; volatile int count; char *rd; char *wr; char data[1]; } sRing;
/* serial console state; fd, wlen, pos and fC are shared with the RPC callback func_00424EA8 */
typedef struct {
    volatile int fd;
    volatile int wlen;
    volatile int pos;
    volatile int fC;
    char *wp;
    unsigned char *buf;
    sRing *ring;
} sTty;
extern sTty D_0052B8D0;
extern sRing *D_0052B8E8;      /* D_0052B8D0.ring (own splat symbol) */
extern int D_0052B7C0[];       /* storage of the console ring (func_00424E00) */
extern sRing *func_00424E00(int size);


/* SIF RPC library state D_0052D680 (implementation side) */
typedef struct { char pad[0x10]; int f10; int f14; int f18; char pad2[0x24]; } sSifRpcPkt;    /* 0x40-byte packet */
typedef struct sSifServer { int key; char pad[0x34]; struct sSifServer *next; } sSifServer;
typedef struct sSifQueue { int f0; int f4; sSifServer *servers; int fC; int f10; struct sSifQueue *next; } sSifQueue;
typedef struct {
    int pid;                    /* last packet id */
    sSifRpcPkt *pkt_table;
    int pkt_table_len;
    int fC;
    int f10;
    char *rbuf;                 /* 64-byte receive packets, used round robin */
    int rbuf_len;
    int rdata;                  /* 64-byte packets addressed by number */
    int rdata_len;
    int rbuf_idx;
    sSifQueue *queues;          /* +0x28 */
} sSifRpcState;
extern sSifRpcState D_0052D680;


/* wake-up request queue of the helper thread started by InitThread (512 {type, thread id} entries) */
typedef struct {
    int f0;
    unsigned idx;
    struct { char a; char b; } e[512];
} sWakeQueue;
extern sWakeQueue D_0052B3A8;


/* SIF DMA transfer descriptor (as passed to sceSifSetDma / isceSifSetDma) */
typedef struct { int data; int addr; int size; int attr; } sSifDmaData;
extern int sceSifSetDma(sSifDmaData *sdd, int n);
extern int isceSifSetDma(sSifDmaData *sdd, int n);


/* library state used by more than one function */
extern int D_00455168;
extern int D_00455170;          /* helper thread id (InitThread) */
extern int D_0052B3A0;          /* its wake-up semaphore */
extern int D_0052BCD8[];
extern int *D_0052BCE4;
extern int *D_0052BCEC;
extern int D_0052BE00[];
extern volatile int D_00455188[];
extern int D_0045520C;          /* fileio initialised (func_00427A90) */
extern int D_00455210;
extern int D_00455214;
extern int D_00455218;          /* fileio descriptor table semaphore */
extern int D_0045521C;
extern int D_00455224;
extern int D_0052EF00;          /* RPC result for D_0052EEC0 */
extern int D_0052EF40;          /* RPC argument for D_0052EEC0 */
extern int D_00455228;          /* loadfile bound (< 0 = not yet) */
extern int D_00455230;
extern int D_00456538;


/* fileio RPC send block D_0052D700: one packet layout per RPC function number (sent to D_0052EE00) */
typedef struct { int f0; int f4; int f8; int fC; } sFioCmd4;                                    /* 15 */
typedef struct { int f0; int f4; int f8; int fC; int f10; } sFioFdCmd;                          /* 1, 10, 11 */
typedef struct { int f0; int f4; int f8; int fC; int f10; int f14; int f18; } sFioLseekCmd;    /* 4 */
typedef struct { int f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; } sFioReadCmd; /* 2 */
typedef struct { int f0; int f4; int f8; int fC; unsigned f10; int f14; int f18; char data[16]; int f2C; } sFioWriteCmd; /* 3 */
typedef struct { int f0; int f4; int f8; int fC; long f10; int f18; int f1C; } sFioLseek64Cmd;  /* 22 */
typedef struct { int f0; int f4; int f8; char name[0x400]; } sFioPathCmd;                       /* 6, 8, 9, 16, 18, 21 */
typedef struct { int f0; int f4; int f8; int fC; char name[0x400]; } sFioPathModeCmd;           /* 7, 12 */
typedef struct { int f0; int f4; int f8; int fC; int f10; char name[0x400]; } sFioPathArgCmd;   /* 19, 25 */
typedef struct { int f0; int f4; int f8; int fC; int f10; char name[0x400]; int f414; } sFioOpenCmd; /* 0 */
typedef struct { int f0; int f4; int f8; char name[0x400]; char name2[0x400]; } sFioPath2Cmd;   /* 17, 24 */
typedef struct { int f0; int f4; int f8; char name[0x400]; char name2[0x400]; char data[0x400]; int fC0C; } sFioFormatCmd; /* 14 */
typedef struct { int f0; int f4; int f8; char name[0x400]; char name2[0x400]; char data[0x400]; int fC0C; int fC10; } sFioMountCmd; /* 20 */
typedef struct { int w[16]; } sFioStat;
typedef struct { int f0; int f4; int f8; int fC; sFioStat v; char name[0x400]; } sFioChstatCmd; /* 13 */
typedef struct { int f0; int f4; int f8; char name[0x400]; char data[0x400]; int f80C; int f810; int f814; int f818; } sFioDevctlCmd; /* 23 */
typedef struct { int f0; int f4; int f8; int fC; int f10; char data[0x400]; int f414; int f418; int f41C; } sFioIoctl2Cmd; /* 26 */
typedef union {
    sFioOpenCmd open;           /* 0 */
    sFioFdCmd close;            /* 1 */
    sFioReadCmd read;           /* 2 */
    sFioWriteCmd write;         /* 3 */
    sFioLseekCmd lseek;         /* 4 */
    sFioPathCmd path;           /* 6, 8, 9, 16, 18, 21 (func_00428E18 takes the number) */
    sFioPathModeCmd mkdir;      /* 7 */
    sFioFormatCmd format;       /* 14 */
    sFioCmd4 adddrv;            /* 15 */
    sFioFdCmd dclose;           /* 10 */
    sFioFdCmd dread;            /* 11 */
    sFioPathModeCmd getstat;    /* 12 */
    sFioChstatCmd chstat;       /* 13 */
    sFioPath2Cmd rename;        /* 17 */
    sFioPathArgCmd sync;        /* 19 */
    sFioMountCmd mount;         /* 20 */
    sFioLseek64Cmd lseek64;     /* 22 */
    sFioDevctlCmd devctl;       /* 23 */
    sFioPath2Cmd symlink;       /* 24 */
    sFioPathArgCmd readlink;    /* 25 */
    sFioIoctl2Cmd ioctl2;       /* 26 */
} uFioCmd;
extern uFioCmd D_0052D700;

//100%
INCLUDE_ASM("seg/seg_325220", func_00424220);
#ifdef SKIP_ASM
/**
 * Marks the DECI2 console channel as closed so the next console read or write reopens it.
 *
 * Clears D_00455168, the flag _write and _read set after func_00425260 opened the channel. sceSifSyncIop
 * calls it after the IOP has been rebooted.
 */
void func_00424220(void) {
    D_00455168 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424230);
#ifdef SKIP_ASM
extern int DIntr_r(void) __asm__("DIntr");
extern void EIntr(void);
/**
 * Waits for the start of the next vertical blank by polling the interrupt controller.
 *
 * Clears the VBLANK-start bit (bit 2) of the interrupt status register 0x1000F000 with interrupts briefly off (DIntr,
 * EIntr when they were on), polls until it is set again, then clears it once more. No code calls it.
 */
void func_00424230(void) {
    int intrEnabled;
    intrEnabled = DIntr_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) EIntr();
    while (!(*(volatile int *)0x1000F000 & 4));
    intrEnabled = DIntr_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004242C0);
#ifdef SKIP_ASM
extern void EIntr(void);
extern void SetVSyncFlag_vs(volatile int *, volatile unsigned long *) __asm__("SetVSyncFlag");
/**
 * Waits for the start of the next vertical blank and returns the GS CSR value the kernel recorded.
 *
 * Registers a local flag and a CSR slot with SetVSyncFlag, clears bit 2 of 0x1000F000 as func_00424230 does, and polls
 * until the bit is set again or the kernel has set the flag; then clears the bit and returns the CSR slot. No code
 * calls it.
 *
 * @return the GS CSR value stored by the kernel's vsync handler
 */
unsigned long func_004242C0(void) {
    volatile int flag;
    volatile unsigned long gsCsr;
    int intrEnabled;
    flag = 0;
    SetVSyncFlag_vs(&flag, &gsCsr);
    intrEnabled = DIntr_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) EIntr();
    while (!(*(volatile int *)0x1000F000 & 4) && flag == 0);
    intrEnabled = DIntr_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) EIntr();
    return gsCsr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _write);
#ifdef SKIP_ASM
extern int func_00425260(void);
extern int func_00425040(int, int);
/**
 * Writes bytes to the console: descriptors 1 and 2 go to the DECI2 TTY channel; returns the count or -1.
 *
 * Opens the channel with func_00425260 on first use (flag D_00455168) and sends with func_00425040. Other descriptors,
 * or a channel that cannot be opened, return -1 without setting errno. _write_r and the console buffers
 * func_0041B550 and func_0041C938 call it.
 *
 * @param fileDesc  the descriptor
 * @param buffer  the bytes
 * @param byteCount  the byte count
 * @return the bytes taken, or -1
 */
int _write(int fileDesc, int buffer, int byteCount) {
    if (fileDesc == 1 || fileDesc == 2) {
        if (D_00455168 == 0) {
            if (func_00425260() == 0) {
                return -1;
            }
            D_00455168 = 1;
        }
        return func_00425040(buffer, byteCount);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _read);
#ifdef SKIP_ASM
extern int func_00425260(void);
extern int func_00425190(int, int);
/**
 * Reads console input for descriptor 0 from the DECI2 TTY channel and returns the count or -1.
 *
 * Opens the channel with func_00425260 on first use (flag D_00455168) and reads with func_00425190, which waits for
 * input and stops after a newline. Other descriptors return -1. _read_r calls it.
 *
 * @param fileDesc  the descriptor
 * @param buffer  the destination
 * @param byteCount  the maximum byte count
 * @return the bytes read, or -1
 */
int _read(int fileDesc, int buffer, int byteCount) {
    if (fileDesc == 0) {
        if (D_00455168 == 0) {
            if (func_00425260() == 0) {
                return -1;
            }
            D_00455168 = 1;
        }
        return func_00425190(buffer, byteCount);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _open);
#ifdef SKIP_ASM
extern int *__errno(void);
/**
 * Fails a file open: sets errno 5 (EIO) and returns -1.
 *
 * Declared without parameters; errno is set through __errno. _open_r calls it, so every stdio file open
 * fails.
 *
 * @return -1
 */
int _open(void) {
    *__errno() = 5;
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _close);
#ifdef SKIP_ASM
/**
 * Returns -1: closing a descriptor is not supported (errno is left alone).
 *
 * _close_r calls it.
 *
 * @return -1
 */
int _close(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424490);
#ifdef SKIP_ASM
/**
 * Returns -1 (an unsupported system-call stub of the C library glue; errno is left alone).
 *
 * No code calls it.
 *
 * @return -1
 */
int func_00424490(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _lseek);
#ifdef SKIP_ASM
/**
 * Returns -1: seeking a descriptor is not supported (errno is left alone).
 *
 * _lseek_r calls it.
 *
 * @return -1
 */
int _lseek(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _sbrk);
#ifdef SKIP_ASM
extern unsigned D_0045516C;
extern unsigned EndOfHeap(void);
extern int *__errno(void);
/**
 * Moves the C heap break by n bytes and returns the old break, or all ones when it would pass the end of the heap.
 *
 * The break is D_0045516C; the limit comes from EndOfHeap. When interrupts are enabled (Status bit 16) they are
 * disabled for the update (di until the bit reads clear) and enabled again afterwards. Failure sets errno 12 (ENOMEM)
 * through __errno. _sbrk_r calls it.
 *
 * @param n  the byte change
 * @return the previous break, or 0xFFFFFFFF
 */
unsigned _sbrk(unsigned n) {
    int s, st;
    unsigned end;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) {
        do {
            DI_4248();
            SYNCP_4248();
            MFC0_STATUS_4248(st);
        } while (st & 0x10000);
    }
    end = D_0045516C + n;
    if (EndOfHeap() < end) {
        *__errno() = 12;
        if (s) EI_4248();
        return 0xFFFFFFFF;
    }
    {
        unsigned old = D_0045516C;
        D_0045516C = end;
        if (s) EI_4248();
        return old;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424550);
#ifdef SKIP_ASM
/**
 * Returns 1: every descriptor counts as a terminal.
 *
 * @return 1
 */
int func_00424550(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _fstat);
#ifdef SKIP_ASM
/**
 * Fills a stat buffer as a character device for any descriptor and returns 0.
 *
 * Sets the mode word (+4) to 0x2000 (S_IFCHR) and clears the 64-bit word at +0x48; nothing else is written.
 * _fstat_r calls it, so stdio never treats a stream as a seekable regular file.
 *
 * @param fileDesc  the descriptor (ignored)
 * @param statBuf   the stat buffer
 * @return 0
 */
long _fstat(int fileDesc, struct stat *statBuf) {
    statBuf->st_mode = 0x2000;
    statBuf->st_blksize = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424570);
#ifdef SKIP_ASM
/**
 * Returns 1, the only process id.
 *
 * @return 1
 */
int func_00424570(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424578);
#ifdef SKIP_ASM
extern void Exit(int a0);
/**
 * Ends the program with Exit(sigNum) when pid is 1; for any other pid it does nothing.
 *
 * @param pid     the process id
 * @param sigNum  the signal number, used as the exit status
 * @return 0 (only reached for other pids)
 */
int func_00424578(int pid, int sigNum) {
    if (pid == 1) {
        Exit(sigNum);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004245A0);
#ifdef SKIP_ASM
extern int *__errno(void);
/**
 * Fails an unsupported file operation: sets errno 5 (EIO) and returns -1.
 *
 * Declared without parameters; errno is set through __errno.
 *
 * @return -1
 */
int func_004245A0(void) {
    *__errno() = 5;
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004245C8);
#ifdef SKIP_ASM
extern int *__errno(void);
/**
 * Fails an unsupported file operation: sets errno 5 (EIO) and returns -1.
 *
 * Declared without parameters; errno is set through __errno. No code calls it.
 *
 * @return -1
 */
int func_004245C8(void) {
    *__errno() = 5;
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004245F0);
#ifdef SKIP_ASM
// PORT: PS2-only cache op wrapped in sync (cache instruction: op, byte offset, address).
#define CACHE_4248(op, off, p) __asm__ volatile("sync\n\tcache " #op ", " #off "(%0)\n\tsync" : : "r"(p))
// PORT: PS2-only COP0 (read TagLo).
#define MFC0_TAGLO_4248(v) __asm__ volatile(".set push\n\t.set noreorder\n\tmfc0 %0, $28\n\t.set pop" : "=r"(v))
/**
 * Writes back and invalidates every data-cache line whose address lies in [lo, hi].
 *
 * Walks the 64 sets of both ways (index 0..0xFC0 in 0x40 steps): loads the line tag (cache 0x10, then TagLo), rebuilds
 * the line address and runs cache 0x14 (index write-back invalidate) on the lines in range, with sync around each cache
 * operation. SyncDCache and iSyncDCache call it with 64-byte aligned bounds.
 *
 * @param lo  the first line address
 * @param hi  the last line address
 */
void func_004245F0(unsigned lo, unsigned hi) {
    int i;
    for (i = 0; i < 0x1000; i += 0x40) {
        unsigned tag;
        CACHE_4248(0x10, 0, i);
        MFC0_TAGLO_4248(tag);
        tag = (tag & 0xFFFFF000) + i;
        if (!(tag < lo) && !(hi < tag)) CACHE_4248(20, 0, i);
        CACHE_4248(0x10, 1, i);
        MFC0_TAGLO_4248(tag);
        tag = (tag & 0xFFFFF000) + i;
        if (!(tag < lo) && !(hi < tag)) CACHE_4248(20, 1, i);
        SYNC_4248();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", SyncDCache);
#ifdef SKIP_ASM
extern void DIntr(void);
extern void func_004245F0(unsigned, unsigned);
/**
 * Writes back and invalidates the data cache for an address range, with interrupts held off.
 *
 * Aligns both bounds down to 64 bytes and calls func_004245F0, between DIntr and EIntr when interrupts were enabled
 * (Status bit 16).
 *
 * @param startAddr  the first address
 * @param endAddr    the last address
 */
void SyncDCache(unsigned startAddr, unsigned endAddr) {
    int intrEnabled;
    MFC0_STATUS_4248(intrEnabled);
    intrEnabled &= 0x10000;
    if (intrEnabled) DIntr();
    func_004245F0(startAddr & ~63, endAddr & ~63);
    if (intrEnabled) EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iSyncDCache);
#ifdef SKIP_ASM
extern void func_004245F0(unsigned, unsigned);
/**
 * Writes back and invalidates the data cache for an address range (for interrupt handlers: no interrupt masking).
 *
 * Aligns both bounds down to 64 bytes and calls func_004245F0. No code calls it.
 *
 * @param startAddr  the first address
 * @param endAddr    the last address
 */
void iSyncDCache(unsigned startAddr, unsigned endAddr) {
    func_004245F0(startAddr & ~63, endAddr & ~63);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424730);
#ifdef SKIP_ASM
/**
 * Invalidates, without writing back, every data-cache line whose address lies in [lo, hi].
 *
 * The same tag walk as func_004245F0, but runs cache 0x16 (index invalidate), so dirty data in the range is discarded.
 * InvalidDCache and iInvalidDCache call it with 64-byte aligned bounds.
 *
 * @param lo  the first line address
 * @param hi  the last line address
 */
void func_00424730(unsigned lo, unsigned hi) {
    int i;
    for (i = 0; i < 0x1000; i += 0x40) {
        unsigned tag;
        CACHE_4248(0x10, 0, i);
        MFC0_TAGLO_4248(tag);
        tag = (tag & 0xFFFFF000) + i;
        if (!(tag < lo) && !(hi < tag)) CACHE_4248(22, 0, i);
        CACHE_4248(0x10, 1, i);
        MFC0_TAGLO_4248(tag);
        tag = (tag & 0xFFFFF000) + i;
        if (!(tag < lo) && !(hi < tag)) CACHE_4248(22, 1, i);
        SYNC_4248();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", InvalidDCache);
#ifdef SKIP_ASM
extern void DIntr(void);
extern void func_00424730(unsigned, unsigned);
/**
 * Invalidates the data cache for an address range without writing it back, with interrupts held off.
 *
 * Aligns both bounds down to 64 bytes and calls func_00424730, between DIntr and EIntr when interrupts were enabled.
 * Callers include cPS2GraphicsMan_DrawFrame.
 *
 * @param startAddr  the first address
 * @param endAddr    the last address
 */
void InvalidDCache(unsigned startAddr, unsigned endAddr) {
    int intrEnabled;
    MFC0_STATUS_4248(intrEnabled);
    intrEnabled &= 0x10000;
    if (intrEnabled) DIntr();
    func_00424730(startAddr & ~63, endAddr & ~63);
    if (intrEnabled) EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iInvalidDCache);
#ifdef SKIP_ASM
extern void func_00424730(unsigned, unsigned);
/**
 * Invalidates the data cache for an address range without writing it back (for interrupt handlers: no interrupt
 * masking).
 *
 * Aligns both bounds down to 64 bytes and calls func_00424730. No code calls it.
 *
 * @param startAddr  the first address
 * @param endAddr    the last address
 */
void iInvalidDCache(unsigned startAddr, unsigned endAddr) {
    func_00424730(startAddr & ~63, endAddr & ~63);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424870);
#ifdef SKIP_ASM
/**
 * Returns 1 when the COP0 Status register's IE bit (bit 0) is clear, else 0.
 *
 * No code calls it.
 *
 * @return 1 or 0
 */
int func_00424870(void) {
    int status;
    MFC0_STATUS_4248(status);
    return (status ^ 1) & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", DisableIntc);
#ifdef SKIP_ASM
extern void DIntr(void);
extern int _DisableIntc(int);
/**
 * Disables one interrupt-controller cause and returns the system call's result.
 *
 * Calls _DisableIntc followed by sync, between DIntr and EIntr when interrupts were enabled (Status bit 16).
 *
 * @param cause  the INTC cause number
 * @return _DisableIntc's result
 */
int DisableIntc(int cause) {
    int intrEnabled, result;
    MFC0_STATUS_4248(intrEnabled);
    intrEnabled &= 0x10000;
    if (intrEnabled) DIntr();
    result = _DisableIntc(cause);
    SYNC_4248();
    if (intrEnabled) EIntr();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", EnableIntc);
#ifdef SKIP_ASM
extern void DIntr(void);
extern int _EnableIntc(int);
/**
 * Enables one interrupt-controller cause and returns the system call's result.
 *
 * Calls _EnableIntc followed by sync, between DIntr and EIntr when interrupts were enabled.
 *
 * @param cause  the INTC cause number
 * @return _EnableIntc's result
 */
int EnableIntc(int cause) {
    int intrEnabled, result;
    MFC0_STATUS_4248(intrEnabled);
    intrEnabled &= 0x10000;
    if (intrEnabled) DIntr();
    result = _EnableIntc(cause);
    SYNC_4248();
    if (intrEnabled) EIntr();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", DisableDmac);
#ifdef SKIP_ASM
extern void DIntr(void);
extern int _DisableDmac(int);
/**
 * Disables one DMA channel's completion interrupt and returns the system call's result.
 *
 * Calls _DisableDmac followed by sync, between DIntr and EIntr when interrupts were enabled. Callers include
 * sceSifExitCmd (channel 5) and cPS2GraphicsMan_Purge.
 *
 * @param channel  the DMA channel
 * @return _DisableDmac's result
 */
int DisableDmac(int channel) {
    int intrEnabled, result;
    MFC0_STATUS_4248(intrEnabled);
    intrEnabled &= 0x10000;
    if (intrEnabled) DIntr();
    result = _DisableDmac(channel);
    SYNC_4248();
    if (intrEnabled) EIntr();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", EnableDmac);
#ifdef SKIP_ASM
extern void DIntr(void);
extern int _EnableDmac(int);
/**
 * Enables one DMA channel's completion interrupt and returns the system call's result.
 *
 * Calls _EnableDmac followed by sync, between DIntr and EIntr when interrupts were enabled. sceSifInitCmd calls it for
 * channel 5 after installing its handler.
 *
 * @param channel  the DMA channel
 * @return _EnableDmac's result
 */
int EnableDmac(int channel) {
    int intrEnabled, result;
    MFC0_STATUS_4248(intrEnabled);
    intrEnabled &= 0x10000;
    if (intrEnabled) DIntr();
    result = _EnableDmac(channel);
    SYNC_4248();
    if (intrEnabled) EIntr();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iEnableIntc);
#ifdef SKIP_ASM
extern void _iEnableIntc(void);
/**
 * Enables an interrupt-controller cause from interrupt context.
 *
 * Declared without parameters: the cause stays in the first argument register for _iEnableIntc, followed by sync. No
 * code calls it.
 */
void iEnableIntc(void) {
    _iEnableIntc();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iDisableIntc);
#ifdef SKIP_ASM
extern void _iDisableIntc(void);
/**
 * Disables an interrupt-controller cause from interrupt context.
 *
 * Declared without parameters: the cause stays in the first argument register for _iDisableIntc, followed by sync. No
 * code calls it.
 */
void iDisableIntc(void) {
    _iDisableIntc();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iEnableDmac);
#ifdef SKIP_ASM
extern void _iEnableDmac(void);
/**
 * Enables a DMA channel's completion interrupt from interrupt context.
 *
 * Declared without parameters: the channel stays in the first argument register for _iEnableDmac, followed by sync. No
 * code calls it.
 */
void iEnableDmac(void) {
    _iEnableDmac();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iDisableDmac);
#ifdef SKIP_ASM
extern void _iDisableDmac(void);
/**
 * Disables a DMA channel's completion interrupt from interrupt context.
 *
 * Declared without parameters: the channel stays in the first argument register for _iDisableDmac, followed by sync. No
 * code calls it.
 */
void iDisableDmac(void) {
    _iDisableDmac();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424AA0);
#ifdef SKIP_ASM
extern char D_0049AAE0[];
extern int func_00425C28_pf(const char *, ...) __asm__("func_00425C28");
extern int WaitSema_i(int) __asm__("WaitSema");
extern void RotateThreadReadyQueue(int);
extern void WakeupThread(int);
extern void SuspendThread(int);
/**
 * Runs forever as the helper thread that carries out thread requests queued from interrupt handlers.
 *
 * Each time the semaphore D_0052B3A0 is signalled it takes the next 2-byte entry {type, thread id} from the 512-entry
 * ring (read index in the first word, entries from +8) and calls WakeupThread (type 0), RotateThreadReadyQueue (type 1,
 * the id being a priority) or SuspendThread (type 2). Any other type prints "## internel error in libkernl.a!" (sic)
 * with func_00425C28. InitThread starts it with D_0052B3A8.
 *
 * @param wakeQueue  the request queue
 */
void func_00424AA0(sWakeQueue *wakeQueue) {
    unsigned i;
    unsigned j;
    unsigned char *entryTypes = (unsigned char *)&wakeQueue->e[0].a;
    unsigned char *entryThreads = (unsigned char *)&wakeQueue->e[0].b;
    for (;;) {
        WaitSema_i(D_0052B3A0);
        i = wakeQueue->f0 & 0x1FF;
        j = i << 1;
        wakeQueue->f0 = i + 1;
        switch (*(entryTypes + j)) {
        case 0:
            WakeupThread(*(entryThreads + j));
            break;
        case 1:
            RotateThreadReadyQueue(*(entryThreads + j));
            break;
        case 2:
            SuspendThread(*(entryThreads + j));
            break;
        default:
            func_00425C28_pf(D_0049AAE0);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", InitThread);
#ifdef SKIP_ASM
extern char D_0052AFA0[];
extern char D_004A30F0[];
extern int CreateSema(int *p);
extern int CreateThread(int *);
extern void StartThread(int, void *);
extern int GetThreadId(void);
extern void ChangeThreadPriority(int, int);
extern void DeleteSema(int);
extern void func_00424AA0(sWakeQueue *);
int InitThread_i(void) __asm__("InitThread");
/**
 * Creates and starts the helper thread for interrupt-context thread requests and returns its id, or -1.
 *
 * Returns -1 when the thread already exists (D_00455170 > 0). Creates the semaphore D_0052B3A0 (max 0xFF), then the
 * thread running func_00424AA0 with the 0x400-byte stack D_0052AFA0, gp D_004A30F0 and priority 0; a failed thread
 * deletes the semaphore again. It clears the queue D_0052B3A8, starts the thread with it as the argument and drops the
 * calling thread to priority 1. _InitSys calls it.
 *
 * @return the helper thread id, or -1
 */
int InitThread_i(void) {
    int cfg[12];
    int buf[8];
    if (D_00455170 > 0) {
        return -1;
    }
    buf[2] = 0;
    buf[1] = 0xFF;
    D_0052B3A0 = CreateSema(buf);
    if (D_0052B3A0 < 0) {
        return -1;
    }
    cfg[1] = (int)func_00424AA0;
    cfg[2] = (int)D_0052AFA0;
    cfg[3] = 0x400;
    cfg[4] = (int)D_004A30F0;
    cfg[5] = 0;
    D_00455170 = CreateThread(cfg);
    if (D_00455170 < 0) {
        DeleteSema(D_0052B3A0);
        return -1;
    }
    D_0052B3A8.f0 = 0;
    D_0052B3A8.idx = 0;
    StartThread(D_00455170, &D_0052B3A8);
    ChangeThreadPriority(GetThreadId(), 1);
    return D_00455170;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iWakeupThread);
#ifdef SKIP_ASM
extern void iSignalSema(int);
extern int _iWakeupThread(void);
// PORT: PS2 kernel syscall (number in $3, result in $2).
#define SYSCALL_RET_4C50(n, r) do { register int r_ __asm__("$2"); __asm__ volatile("li $3, " #n "\n\tsyscall" : "=r"(r_) : : "$3"); r = r_; } while (0)
int iWakeupThread_r(int a0) __asm__("iWakeupThread");
/**
 * Wakes a thread from interrupt context; waking the interrupted thread itself is passed to the helper thread.
 *
 * Gets the current thread id with system call -47. Any other thread goes straight to _iWakeupThread. For the current
 * thread (id below 0x100, helper running) it queues a type-0 entry in D_0052B3A8 and signals D_0052B3A0 with
 * iSignalSema. func_00426C88 calls it to wake an RPC server thread.
 *
 * @param threadId  the thread id
 * @return the thread id or _iWakeupThread's result, or -1 when it cannot be queued
 */
int iWakeupThread_r(int threadId) {
    int id;
    unsigned i;
    SYSCALL_RET_4C50(-47, id);
    if (id != threadId) return _iWakeupThread();
    if ((unsigned)id >= 0x100 || D_00455170 == 0) return -1;
    i = D_0052B3A8.idx & 0x1FF;
    D_0052B3A8.idx = i + 1;
    D_0052B3A8.e[i].a = 0;
    D_0052B3A8.e[i].b = id;
    iSignalSema(D_0052B3A0);
    return id;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iRotateThreadReadyQueue);
#ifdef SKIP_ASM
extern void iSignalSema(int);
/**
 * Queues a ready-queue rotation for a priority level from interrupt context, done later by the helper thread.
 *
 * Priorities from 0x80 up, or a missing helper thread, return -1. Otherwise it queues a type-1 entry and signals
 * D_0052B3A0 with iSignalSema. THREAD_yield and THREAD_yieldticks call it.
 *
 * @param priority  the priority
 * @return the priority, or -1
 */
int iRotateThreadReadyQueue(unsigned priority) {
    unsigned i;
    if (priority >= 0x80 || D_00455170 == 0) {
        return -1;
    }
    i = D_0052B3A8.idx & 0x1FF;
    D_0052B3A8.idx = i + 1;
    D_0052B3A8.e[i].a = 1;
    D_0052B3A8.e[i].b = priority;
    iSignalSema(D_0052B3A0);
    return priority;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iSuspendThread);
#ifdef SKIP_ASM
extern int _iSuspendThread(void);
int iSuspendThread_r(int a0) __asm__("iSuspendThread");
/**
 * Suspends a thread from interrupt context; suspending the interrupted thread itself is passed to the helper thread.
 *
 * Like iWakeupThread with _iSuspendThread and a type-2 entry. No code calls it.
 *
 * @param threadId  the thread id
 * @return the thread id or _iSuspendThread's result, or -1 when it cannot be queued
 */
int iSuspendThread_r(int threadId) {
    int id;
    unsigned i;
    SYSCALL_RET_4C50(-47, id);
    if (id != threadId) return _iSuspendThread();
    if ((unsigned)id >= 0x100 || D_00455170 == 0) return -1;
    i = D_0052B3A8.idx & 0x1FF;
    D_0052B3A8.idx = i + 1;
    D_0052B3A8.e[i].a = 2;
    D_0052B3A8.e[i].b = id;
    iSignalSema(D_0052B3A0);
    return id;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424E00);
#ifdef SKIP_ASM
/**
 * Initialises the console input ring buffer in D_0052B7C0 and returns it.
 *
 * Stores the data size, a zero count and read and write pointers at the start of the data (+0x10). The storage is
 * fixed, so every call returns the same ring. func_00425260 calls it with 0x100.
 *
 * @param dataSize  the data size in bytes
 * @return the ring
 */
sRing *func_00424E00(int dataSize) {
    D_0052B7C0[0] = dataSize;
    D_0052B7C0[1] = 0;
    D_0052B7C0[3] = (int)&D_0052B7C0[4];
    D_0052B7C0[2] = (int)&D_0052B7C0[4];
    return (sRing *)D_0052B7C0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424E28);
#ifdef SKIP_ASM
/**
 * Counts one byte written into a ring buffer and advances its write pointer, wrapping at the end of the data.
 *
 * @param ring  the ring
 */
void func_00424E28(sRing *ring) {
    ring->count++;
    ring->wr++;
    if (ring->wr == (char *)ring + (ring->size + 16)) {
        ring->wr = ring->data;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424E68);
#ifdef SKIP_ASM
/**
 * Counts one byte read from a ring buffer and advances its read pointer, wrapping at the end of the data.
 *
 * @param ring  the ring
 */
void func_00424E68(sRing *ring) {
    ring->count--;
    ring->rd++;
    if (ring->rd == (char *)ring + (ring->size + 16)) {
        ring->rd = ring->data;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424EA8);
#ifdef SKIP_ASM
// The read position (volatile in sTty) is taken once into a local just before the read call.
extern int pf_4EA8(const char *, ...) __asm__("func_00425C28");
extern int rd_4EA8(int, unsigned char *, unsigned short) __asm__("sceDeci2ExRecv");
extern int wr_4EA8(int, char *, unsigned short) __asm__("sceDeci2ExSend");
extern char D_0049AB08[], D_0049AB30[], D_0049AB48[], D_0049AB60[];
/**
 * Handles a DECI2 event for the console channel (the callback func_00425260 registers).
 *
 * Events 1 and 2 (data arrived): with n bytes pending it receives them with sceDeci2ExRecv into the receive buffer at
 * pos; n = 0 ends the packet, whose payload (from byte 12 up to the length in its first halfword) is copied into the
 * input ring with func_00424E28. Event 3 (ready to send) sends the rest of the packet with sceDeci2ExSend and advances
 * wp and wlen. Event 4 (send done) clears the busy flag fC. Problems are printed with func_00425C28 ("TTY: packet size
 * larger than expect", "TTY: receive error", "TTY: send err %d", "TTY: err ti->wlen=%08x").
 *
 * Likely bug: a packet over 320 bytes is only reported, then still received past the end of the buffer.
 *
 * @param cmd  the event code
 * @param n    the bytes pending (events 1 and 2)
 * @param ttyState    the console state D_0052B8D0
 */
void func_00424EA8(int cmd, int n, sTty *ttyState) {
    switch (cmd) {
    case 1:
    case 2:
        if (n != 0) {
            if ((unsigned)(ttyState->pos + n) > 320)
                pf_4EA8(D_0049AB08);
            {
                int pos = ttyState->pos;
                n = rd_4EA8(ttyState->fd, ttyState->buf + pos, n);
            }
            if (n < 0)
                pf_4EA8(D_0049AB30);
            ttyState->pos += n;
        } else {
            unsigned short *hdr = (unsigned short *)ttyState->buf;
            for (n = 12; n < *hdr; n++) {
                *ttyState->ring->wr = ttyState->buf[n];
                func_00424E28(ttyState->ring);
            }
            ttyState->pos = 0;
        }
        break;
    case 3: {
        int sentCount = wr_4EA8(ttyState->fd, ttyState->wp, ttyState->wlen);
        if (sentCount < 0) {
            pf_4EA8(D_0049AB48, sentCount);
            ttyState->fC = 0;
            break;
        }
        ttyState->wp += sentCount;
        ttyState->wlen -= sentCount;
        break;
    }
    case 4:
        if (ttyState->wlen != 0)
            pf_4EA8(D_0049AB60, ttyState->wlen);
        ttyState->fC = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425040);
#ifdef SKIP_ASM
/* DECI2 tty write: copy up to 256 bytes ('\n' -> "\r\n") into the uncached send packet and send it. */
struct Pkt_5040 { short f0; short f2; short f4; char f6; char f7; int f8; };
extern char D_0052B900[];
extern int f5040_send(int, char) __asm__("sceDeci2ReqSend");
extern void f5040_poll(int) __asm__("sceDeci2Poll");
int func_00425040_d(char *buf, int len) __asm__("func_00425040");
/**
 * Sends bytes to the host console over DECI2 and returns how many were taken, or -1.
 *
 * Fails when a send is in progress (fC). With interrupts off (DIntr) it builds the packet in the uncached buffer
 * D_0052B900: payload from +12, each '\n' expanded to "\r\n", at most 256 payload bytes (the rest is left to the
 * caller). It sets wlen and the packet length, requests the send with sceDeci2ReqSend and polls sceDeci2Poll until
 * func_00424EA8 clears fC, then calls EIntr. _write calls it.
 *
 * @param buf  the bytes
 * @param len  the byte count
 * @return the input bytes sent, or -1
 */
int func_00425040_d(char *buf, int len) {
    struct Pkt_5040 *pkt;
    char *payloadCursor;
    int i = 0;
    int n = 0;
    if (D_0052B8D0.fC != 0)
        return -1;
    DIntr();
    pkt = (struct Pkt_5040 *)((int)D_0052B900 | 0x20000000);
    D_0052B8D0.fC = 1;
    D_0052B8D0.wp = (char *)pkt;
    payloadCursor = (char *)pkt + 12;
    while (len-- != 0) {
        if (*buf == '\n') {
            *payloadCursor++ = '\r';
            if (++i >= 256)
                break;
        }
        *payloadCursor++ = *buf++;
        n++;
        if (++i >= 256)
            break;
    }
    D_0052B8D0.wlen = i + 12;
    pkt->f0 = D_0052B8D0.wlen;
    if (f5040_send(D_0052B8D0.fd, pkt->f7) < 0) {
        D_0052B8D0.fC = 0;
        EIntr();
        return -1;
    }
    while (D_0052B8D0.fC != 0)
        f5040_poll(D_0052B8D0.fd);
    EIntr();
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425190);
#ifdef SKIP_ASM
/**
 * Reads console input from the ring into a buffer, up to n bytes or through the first line end, and returns the count.
 *
 * Busy-waits while the ring (D_0052B8E8) is empty, takes one byte at a time with func_00424E68, and stops after a '\n'
 * or '\r'. _read calls it.
 *
 * @param bufAddr  the destination
 * @param n   the maximum byte count
 * @return the bytes read, including the line end
 */
int func_00425190(int bufAddr, int n) {
    char *buf = (char *)bufAddr;
    int i;
    int j;
    char *charSlot;
    for (i = 0; i < n; i = j) {
        charSlot = buf + i;
        j = i + 1;
        while (D_0052B8E8->count == 0) {
        }
        *charSlot = *D_0052B8D0.ring->rd;
        func_00424E68(D_0052B8D0.ring);
        if (*charSlot == '\n' || *charSlot == '\r') {
            return j;
        }
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425260);
#ifdef SKIP_ASM
// The binary re-reads fd right after storing it (sTty.fd is volatile).
struct Pkt_5260 { short f0; short f2; short f4; char f6; char f7; int f8; };
extern char D_0052B900[];
extern char D_0052BA40[];
extern void FlushCache(int);
extern int f5260_open(int, void *, void *) __asm__("sceDeci2Open");
/**
 * Opens the DECI2 console channel and returns 1, or 0 when it cannot.
 *
 * Flushes the caches (FlushCache(0)) and opens protocol 0x210 with sceDeci2Open, registering D_0052B8D0 and the
 * callback func_00424EA8. Then it clears the state, points the receive buffer at the uncached D_0052BA40, prepares the
 * send packet header in the uncached D_0052B900 (protocol 0x210, source 'E', destination 'H') and creates the
 * 0x100-byte input ring with func_00424E00.
 *
 * @return 1 when open, else 0
 */
int func_00425260(void) {
    struct Pkt_5260 *sendPacket;
    FlushCache(0);
    D_0052B8D0.fd = f5260_open(528, &D_0052B8D0, func_00424EA8);
    if (D_0052B8D0.fd < 0) return 0;
    D_0052B8D0.fC = 0;
    D_0052B8D0.wlen = 0;
    D_0052B8D0.pos = 0;
    D_0052B8D0.buf = (unsigned char *)((int)D_0052BA40 | 0x20000000);
    sendPacket = (struct Pkt_5260 *)((int)D_0052B900 | 0x20000000);
    D_0052B8D0.wp = (char *)sendPacket;
    sendPacket->f4 = 528;
    sendPacket->f6 = 69;
    sendPacket->f7 = 72;
    sendPacket->f2 = 0;
    sendPacket->f8 = 0;
    D_0052B8D0.ring = func_00424E00(0x100);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425320);
#ifdef SKIP_ASM
int func_00425320_i(int a0) __asm__("func_00425320");
/**
 * Writes one character to the SIO serial port and returns it.
 *
 * Waits while bit 15 of 0x1000F130 (transmit FIFO full) is set, then stores the character in 0x1000F180.
 *
 * @param outChar  the character
 * @return the character
 */
int func_00425320_i(int outChar) {
    while (*(volatile int *)0x1000F130 & 0x8000)
        ;
    *(volatile char *)0x1000F180 = outChar;
    return outChar;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425358);
#ifdef SKIP_ASM
extern int D_00455174;
extern char D_0052BB80[];
extern void func_0042CA58_v(char *) __asm__("func_0042CA58");
/**
 * Adds a character to the 128-byte console line buffer, printing the line at a newline or when the buffer is full.
 *
 * The buffer is D_0052BB80 with the fill count D_00455174; func_0042CA58 prints it. A newline is stored, terminated and
 * printed at once. scePrintf installs it as the output routine.
 *
 * Likely bug: when 126 characters are waiting it terminates the buffer at index 127, so the byte at index 126 (left
 * from an earlier line) is printed too.
 *
 * @param outChar  the character
 */
void func_00425358(int outChar) {
    int idx;
    if (D_00455174 >= 0x7E) {
        D_00455174 = 0;
        D_0052BB80[0x7F] = 0;
        func_0042CA58_v(D_0052BB80);
    }
    idx = D_00455174;
    if (outChar == 10) {
        D_00455174 = 0;
        D_0052BB80[idx] = outChar;
        D_0052BB80[idx + 1] = 0;
        func_0042CA58_v(D_0052BB80);
    } else {
        D_00455174 = idx + 1;
        D_0052BB80[idx] = outChar;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425408);
#ifdef SKIP_ASM
extern void func_00425320(int c);
/**
 * Writes a character to the SIO serial port, sending "\r\n" for a newline.
 *
 * It is the initial value of the output routine pointer D_00455178. Uses func_00425320.
 *
 * @param outChar  the character
 */
void func_00425408(int outChar) {
    if (outChar == 10) {
        func_00425320(13);
        func_00425320(10);
    } else {
        func_00425320(outChar);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425440);
#ifdef SKIP_ASM
int f5440(long a0) __asm__("func_00425440");
/**
 * Converts a non-negative double, given as its 64-bit pattern, to an integer.
 *
 * Values below 2^-54 give 0 and exponents of 13 and more (values of 2^65 and up) give 9999. Values below 1 keep two
 * guard bits and round up only when both are set.
 *
 * Likely bug: the rounding threshold is 0.75, not 0.5; the result is truncated to 32 bits. func_004254D0 calls it.
 *
 * @param bits  the double's bit pattern
 * @return the integer value
 */
int f5440(long bits) {
    unsigned long mantissa = bits;
    long exponent = mantissa << 1 >> 53;
    exponent -= 1075;
    if (exponent < -53) {
        return 0;
    }
    if (exponent >= 13) {
        return 9999;
    }
    mantissa = mantissa << 12 >> 12;
    mantissa = mantissa | 0x10000000000000UL;
    if (exponent < 0) {
        exponent = -exponent;
        mantissa >>= (int)(exponent - 2);
        if ((mantissa & 3) == 3) {
            mantissa = (mantissa >> 2) + 1;
        } else {
            mantissa >>= 2;
        }
    } else {
        mantissa <<= (int)exponent;
    }
    return (int)mantissa;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004254D0);
#ifdef SKIP_ASM
extern long dpcmp(double, double);
extern double dpsub(double, double);
extern double dpmul(double, double);
extern double dpdiv(double, double);
extern double __fixunsdfdi_d(double) __asm__("__fixunsdfdi");
extern double func_00425440_d(double) __asm__("func_00425440");
extern void (*D_00455178)(int);
extern char D_0049AB80[];
extern char D_0049AB88[];
extern char D_0049AB90[];
/**
 * Prints a double in "0.dddddd" plus exponent form through the output routine D_00455178.
 *
 * A negative value prints '-' and is negated. The value is scaled by powers of 10 into [0.1, 1) with dpmul / dpdiv,
 * counting the exponent, multiplied by 1e6 and converted with __fixunsdfdi and func_00425440; then func_00425C28 prints
 * "0.%d" and "e+%d" or "e%d". func_00425638 calls it for %e and %f.
 *
 * Likely bug: func_00425440 decodes its argument as a double's bit pattern but receives the integer from __fixunsdfdi,
 * so the digit part prints as 0. A zero argument would never leave the scaling loop; func_00425638 prints zero itself.
 *
 * @param x  the value
 */
void func_004254D0(double x) {
    int n = 0;
    if (dpcmp(x, 0.0) < 0) {
        x = dpsub(0.0, x);
        D_00455178(45);
    }
    if (dpcmp(x, 0.1) < 0) {
        while (dpcmp(x, 0.1) < 0) {
            n--;
            x = dpmul(x, 10.0);
        }
    } else if (dpcmp(x, 1.0) >= 0) {
        while (dpcmp(x, 1.0) >= 0) {
            n++;
            x = dpdiv(x, 10.0);
        }
    }
    x = dpmul(x, 1000000.0);
    {
        double scaledDigits = func_00425440_d(__fixunsdfdi_d(x));
        func_00425C28((int)D_0049AB80, scaledDigits);
    }
    if (n >= 0)
        func_00425C28((int)D_0049AB88, n);
    else
        func_00425C28((int)D_0049AB90, n);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425638);
#ifdef SKIP_ASM
extern void (*D_00455178)(int);
extern int DIntr_p(void) __asm__("DIntr");
extern void EIntr(void);
extern long __moddi3(long, long);
extern long __divdi3(long, long);
extern unsigned long __umoddi3(unsigned long, unsigned long);
extern unsigned long __udivdi3(unsigned long, unsigned long);
extern double fptodp(float);
extern void func_004254D0(double x);
// printf core: fmt + va_list (8-byte argument slots); 64-bit div/mod go through the soft helpers.
void func_00425638_f(char *fmt, char *ap) __asm__("func_00425638");
/**
 * Formats text from a format string and argument list, sending each character to the output routine D_00455178.
 *
 * Supports %d, %u, %x, %o with h or l sizes and a 0-prefixed width of up to 31 digits, %c, %s and %e / %f (printed by
 * func_004254D0). Arguments take 8-byte slots; 64-bit division uses __moddi3, __divdi3, __umoddi3 and
 * __udivdi3. Interrupts are off (DIntr) for the whole call and turned back on (EIntr) at the end when they were on.
 * func_00425C28 and scePrintf call it.
 *
 * Quirks: %s prints "(null)" for an empty string but dereferences a null pointer; %e and %f read the argument slot as a
 * float although C passes a double; %% and unknown conversions print nothing.
 *
 * @param fmt  the format
 * @param ap   the argument pointer
 */
void func_00425638_f(char *fmt, char *ap) {
    char buf[32];
    char *pad;
    char *printCursor;
    char *str;
    int lmod;
    int w;
    int nextChar;
    long signedValue;
    unsigned long unsignedValue;
    float floatValue;
    int st;
    st = DIntr_p();
    while (*fmt) {
        pad = 0;
        lmod = 0;
        if (*fmt == '%') {
        again:
            switch (*++fmt) {
            case '0':
                w = fmt[1] - '0';
                nextChar = fmt[2];
                if ((unsigned char)w < 10) {
                    if ((unsigned)(nextChar - '0') < 10) {
                        w = w * 10 + (nextChar - '0');
                        if (w > 31)
                            w = 31;
                        fmt += 2;
                    } else {
                        fmt += 1;
                    }
                    pad = &buf[31 - w];
                    for (; w > 0; w--)
                        buf[31 - w] = '0';
                }
                goto again;
            case 'l':
                lmod = 'l';
                goto again;
            case 'h':
                lmod = 'h';
                goto again;
            case 'o':
                if (lmod == 'l') {
                    ap += 8;
                    unsignedValue = *(unsigned long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    unsignedValue = *(unsigned short *)(ap - 8);
                } else {
                    ap += 8;
                    unsignedValue = *(unsigned int *)(ap - 8);
                }
                printCursor = &buf[31];
                *printCursor = 0;
                if (unsignedValue == 0) {
                    *--printCursor = '0';
                } else {
                    while (unsignedValue) {
                        *--printCursor = (unsignedValue & 7) + '0';
                        unsignedValue >>= 3;
                    }
                }
                if (pad && pad < printCursor)
                    printCursor = pad;
                while (*printCursor)
                    D_00455178(*printCursor++);
                break;
            case 'x':
                if (lmod == 'l') {
                    ap += 8;
                    unsignedValue = *(unsigned long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    unsignedValue = *(unsigned short *)(ap - 8);
                } else {
                    ap += 8;
                    unsignedValue = *(unsigned int *)(ap - 8);
                }
                printCursor = &buf[31];
                *printCursor = 0;
                if (unsignedValue == 0) {
                    *--printCursor = '0';
                } else {
                    while (unsignedValue) {
                        unsigned long hexDigit = unsignedValue & 0xF;
                        if (hexDigit < 10)
                            *--printCursor = hexDigit + '0';
                        else
                            *--printCursor = hexDigit + 'a' - 10;
                        unsignedValue >>= 4;
                    }
                }
                if (pad && pad < printCursor)
                    printCursor = pad;
                while (*printCursor)
                    D_00455178(*printCursor++);
                break;
            case 'd':
                if (lmod == 'l') {
                    ap += 8;
                    signedValue = *(long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    signedValue = *(short *)(ap - 8);
                } else {
                    ap += 8;
                    signedValue = *(int *)(ap - 8);
                }
                printCursor = &buf[31];
                *printCursor = 0;
                if (signedValue == 0) {
                    *--printCursor = '0';
                } else {
                    if (signedValue < 0) {
                        signedValue = -signedValue;
                        D_00455178('-');
                    }
                    while (signedValue) {
                        *--printCursor = __moddi3(signedValue, 10) + '0';
                        signedValue = __divdi3(signedValue, 10);
                    }
                }
                if (pad && pad < printCursor)
                    printCursor = pad;
                while (*printCursor)
                    D_00455178(*printCursor++);
                break;
            case 'u':
                if (lmod == 'l') {
                    ap += 8;
                    unsignedValue = *(unsigned long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    unsignedValue = *(unsigned short *)(ap - 8);
                } else {
                    ap += 8;
                    unsignedValue = *(unsigned int *)(ap - 8);
                }
                printCursor = &buf[31];
                *printCursor = 0;
                if (unsignedValue == 0) {
                    *--printCursor = '0';
                } else {
                    while (unsignedValue) {
                        *--printCursor = __umoddi3(unsignedValue, 10) + '0';
                        unsignedValue = __udivdi3(unsignedValue, 10);
                    }
                }
                if (pad && pad < printCursor)
                    printCursor = pad;
                while (*printCursor)
                    D_00455178(*printCursor++);
                break;
            case 'e':
            case 'f':
                ap += 8;
                floatValue = *(float *)(ap - 8);
                if (floatValue == 0.0f)
                    D_00455178('0');
                else
                    func_004254D0(fptodp(floatValue));
                break;
            case 's':
                ap += 8;
                str = *(char **)(ap - 8);
                if (*str == 0) {
                    D_00455178('(');
                    D_00455178('n');
                    D_00455178('u');
                    D_00455178('l');
                    D_00455178('l');
                    D_00455178(')');
                } else {
                    for (printCursor = str; *printCursor; printCursor++)
                        D_00455178(*printCursor);
                }
                break;
            case 'c':
                ap += 8;
                signedValue = *(char *)(ap - 8);
                D_00455178(signedValue);
                break;
            }
        } else {
            D_00455178(*fmt);
        }
        fmt++;
    }
    if (st)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425C28);
#ifdef SKIP_ASM
// not matched: varargs wrapper (a0 passes through, $5 = &spill at sp+0x58 passed as ap). hand-rolled va_start gives right sd $5..$11
// but also emits swc1 $f12..$f18 saves (target has none) -> size mismatch (18 vs 14 insns).
extern void func_00425638(int a0, char *ap);
/**
 * Prints formatted text through the current output routine D_00455178 (the serial port by default).
 *
 * Builds the argument pointer and calls func_00425638. The library's own diagnostics use it: func_00424AA0,
 * func_00424EA8 and func_004254D0.
 *
 * @param a0  the format
 */
void func_00425C28(int a0, ...) {
    char *ap;
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_00425638(a0, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", scePrintf);
#ifdef SKIP_ASM
extern void (*D_00455178)(int);
extern void func_00425358(int c);
extern void func_00425638(int a0, char *ap);
/**
 * Prints formatted text through the console line buffer.
 *
 * Swaps the output routine D_00455178 for func_00425358 while func_00425638 runs, then restores the old one. Callers
 * include sceGsSyncPath, sceUsbKbGetLocation and other Sony library code.
 *
 * @param format  the format
 */
void scePrintf(int format, ...) {
    char *ap;
    void (*old)(int) = D_00455178;
    D_00455178 = func_00425358;
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_00425638(format, ap);
    D_00455178 = old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425CC0);
#ifdef SKIP_ASM
/**
 * Handles the IOP's set-register system command: stores a value in one of the SIF software registers.
 *
 * Writes packet word 5 into the register array (command data +0x1C) at the index in packet word 4. sceSifInitCmd
 * installs it as system command 1.
 *
 * @param packet   the command packet
 * @param cmdData  the command layer data D_0052BCD8
 */
void func_00425CC0(int *packet, int *cmdData) {
    ((int *)cmdData[7])[packet[4]] = packet[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425CE0);
#ifdef SKIP_ASM
/**
 * Handles the IOP's change-address system command: records the IOP's command receive address.
 *
 * Stores packet word 4 as the IOP buffer address (command data +8). sceSifInitCmd installs it as system command 0.
 *
 * @param packet   the command packet
 * @param cmdData  the command layer data D_0052BCD8
 */
void func_00425CE0(int *packet, int *cmdData) {
    cmdData[2] = packet[4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifGetSreg);
#ifdef SKIP_ASM
/**
 * Returns one of the 32 SIF software registers (D_0052BE00).
 *
 * No bounds check. func_0040B130 and the RPC set-up func_00426408 call it.
 *
 * @param regIndex  the register index
 * @return the register value
 */
int sceSifGetSreg(int regIndex) {
    return D_0052BE00[regIndex];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSetSreg);
#ifdef SKIP_ASM
/**
 * Sets one of the 32 SIF software registers and returns the value.
 *
 * No bounds check. No code calls it.
 *
 * @param regIndex  the register index
 * @param regValue  the value
 * @return the value
 */
int sceSifSetSreg(int regIndex, int regValue) {
    D_0052BE00[regIndex] = regValue;
    return regValue;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifGetDataTable);
#ifdef SKIP_ASM
/**
 * Returns the SIF command layer data D_0052BCD8.
 *
 * No code calls it.
 *
 * @return the command layer data
 */
int *sceSifGetDataTable(void) {
    return D_0052BCD8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifInitCmd);
#ifdef SKIP_ASM
/* sceSifInitCmd (libkernel sifcmd): one-time SIF command layer setup and handshake with the IOP. */
typedef struct { void (*func)(int *, int *); void *data; } sCmdHandler_5D38;
typedef struct {
    void *pktbuf;               /* uncached receive buffer */
    void *unused;
    void *iopbuf;               /* IOP-side receive address */
    sCmdHandler_5D38 *sys;
    int nsys;
    sCmdHandler_5D38 *usr;
    int nusr;
    int *sregs;
} sCmdData_5D38;
/* D_0052BCD8 is declared int[] in this unit; this is its real layout */
extern sCmdData_5D38 cmd_5D38 __asm__("D_0052BCD8");
extern int D_0045517C;          /* SIF command layer initialised */
extern char D_0052BC00[];       /* receive packet buffer */
extern char D_0052BC80[];
extern sCmdHandler_5D38 D_0052BD00[32];
extern int D_0052BCC0[5];       /* change-address / init packet */
extern int D_0052BCD4;          /* DMAC 5 handler id */
extern int func_00426230();
extern int f5D38_adddh(int, int (*)(), int) __asm__("AddDmacHandler");
extern void sceSifSetDChain(void);
extern int sceSifGetReg(int a0);
extern unsigned int f5D38_getreg(unsigned int) __asm__("sceSifGetReg");
extern int sceSifSetReg(int, int);
extern int f5D38_send(int, void *, int, void *, void *, int) __asm__("sceSifSendCmd");
/**
 * Initialises the SIF command layer once and starts the handshake with the IOP.
 *
 * With interrupts off it returns at once when D_0045517C is set; otherwise it sets the flag and fills the command data
 * D_0052BCD8: the uncached receive buffer D_0052BC00, the 32-entry system handler table D_0052BD00 (cleared, then
 * entries 0 and 1 set to func_00425CE0 and func_00425CC0), no user table, and the 32 cleared software registers
 * D_0052BE00.
 *
 * After FlushCache(0) it acknowledges DMA status bit 5 (0x1000E010), sets the SIF DMA chain up with sceSifSetDChain
 * when 0x1000C000 bit 8 is clear, installs func_00426230 as the DMA channel 5 handler (AddDmacHandler) and enables it
 * (EnableDmac). If the IOP's buffer address is already in SIF register 0x80000000 it sends the init packet
 * D_0052BCC0 as command 0x80000000; otherwise it waits for register 4 bit 0x20000, reads the IOP buffer from register
 * 2, publishes both addresses in registers 0x80000000 and 0x80000001 and sends command 0x80000002. The RPC set-up
 * func_00426408 calls it.
 */
void sceSifInitCmd(void) {
    int i;
    DIntr();
    if (D_0045517C) {
        EIntr();
        return;
    }
    D_0045517C = 1;
    cmd_5D38.iopbuf = 0;
    cmd_5D38.pktbuf = (void *)((int)D_0052BC00 | 0x20000000);
    cmd_5D38.unused = (void *)((int)D_0052BC80 | 0x20000000);
    cmd_5D38.sys = D_0052BD00;
    cmd_5D38.nsys = 32;
    cmd_5D38.usr = 0;
    cmd_5D38.nusr = 0;
    cmd_5D38.sregs = D_0052BE00;
    for (i = 0; i < 32; i++) {
        D_0052BD00[i].func = 0;
        D_0052BD00[i].data = 0;
    }
    for (i = 0; i < 32; i++)
        D_0052BE00[i] = 0;
    D_0052BD00[0].func = func_00425CE0;
    D_0052BD00[0].data = &cmd_5D38;
    D_0052BD00[1].func = func_00425CC0;
    D_0052BD00[1].data = &cmd_5D38;
    EIntr();
    FlushCache(0);
    if (*(volatile int *)0x1000E010 & 0x20)
        *(volatile int *)0x1000E010 = 0x20;
    if (!(*(volatile int *)0x1000C000 & 0x100))
        sceSifSetDChain();
    D_0052BCD4 = f5D38_adddh(5, func_00426230, 0);
    EnableDmac(5);
    cmd_5D38.iopbuf = (void *)f5D38_getreg(0x80000000);
    if (cmd_5D38.iopbuf) {
        D_0052BCC0[4] = (int)D_0052BC00;
        f5D38_send(0x80000000, D_0052BCC0, 20, 0, 0, 0);
        return;
    } else {
        while (!(f5D38_getreg(4) & 0x20000))
            ;
        cmd_5D38.iopbuf = (void *)f5D38_getreg(2);
        sceSifSetReg(0x80000000, (int)cmd_5D38.iopbuf);
        sceSifSetReg(0x80000001, (int)&cmd_5D38);
        D_0052BCC0[4] = (int)D_0052BC00;
        D_0052BCC0[3] = 0;
        f5D38_send(0x80000002, D_0052BCC0, 20, 0, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifExitCmd);
#ifdef SKIP_ASM
extern int DisableDmac(int a0);
extern int RemoveDmacHandler(int a0, int a1);
extern int D_0045517C;
extern int D_0052BCD4;
/**
 * Shuts the SIF command layer down.
 *
 * Disables DMA channel 5 (DisableDmac), removes the channel 5 handler with RemoveDmacHandler and clears the
 * initialised flag D_0045517C. sceSifExitRpc and func_0031B008 call it.
 */
void sceSifExitCmd(void) {
    DisableDmac(5);
    RemoveDmacHandler(5, D_0052BCD4);
    D_0045517C = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSetCmdBuffer);
#ifdef SKIP_ASM
/**
 * Sets the user command handler table and returns the previous one.
 *
 * Stores the table and its entry count in the command data (+0x14, +0x18). SNDPLATFORM_init calls it.
 *
 * @param handlerTable  the handler table
 * @param handlerCount  the entry count
 * @return the previous table
 */
int sceSifSetCmdBuffer(int handlerTable, int handlerCount) {
    int old = D_0052BCD8[5];
    D_0052BCD8[5] = handlerTable;
    D_0052BCD8[6] = handlerCount;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSetSysCmdBuffer);
#ifdef SKIP_ASM
/**
 * Sets the system command handler table and returns the previous one.
 *
 * Stores the table and its entry count in the command data (+0xC, +0x10). No code calls it.
 *
 * @param handlerTable  the handler table
 * @param handlerCount  the entry count
 * @return the previous table
 */
int sceSifSetSysCmdBuffer(int handlerTable, int handlerCount) {
    int old = D_0052BCD8[3];
    D_0052BCD8[3] = handlerTable;
    D_0052BCD8[4] = handlerCount;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifAddCmdHandler);
#ifdef SKIP_ASM
/**
 * Installs a SIF command handler and its data in the system table (command id with the top bit set) or the user table.
 *
 * The slot is the id times 8 (the shift drops the top bit); there is no bounds check. The RPC set-up func_00426408
 * installs func_00426708, func_00426A98, func_00426C88 and func_00426820 for commands 0x80000008, 9, 0xA and 0xC;
 * SNDPLATFORM_init and func_00427A90 add their own.
 *
 * @param a0  the command id
 * @param handler  the handler
 * @param handlerData  the handler's data
 */
void sceSifAddCmdHandler(int a0, int handler, int handlerData) {
    int off = a0 << 3;
    if (a0 < 0) {
        a0 = (int)D_0052BCE4;
    } else {
        a0 = (int)D_0052BCEC;
    }
    off += a0;
    ((int *)off)[0] = handler;
    ((int *)off)[1] = handlerData;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifRemoveCmdHandler);
#ifdef SKIP_ASM
/**
 * Removes the SIF command handler for a command id (system table when the top bit is set, else user table).
 *
 * Clears the handler pointer of the slot (id times 8). func_00400CB8 calls it.
 *
 * @param a0  the command id
 */
void sceSifRemoveCmdHandler(int a0) {
    int off = a0 << 3;
    if (a0 < 0) {
        a0 = (int)D_0052BCE4;
    } else {
        a0 = (int)D_0052BCEC;
    }
    off += a0;
    *(int *)off = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426078);
#ifdef SKIP_ASM
extern int D_0052BCE0;
extern void func_00426358(int, int);
int func_00426078_d(int, int, int *, unsigned, int, int, int) __asm__("func_00426078");
/**
 * Sends a SIF command packet to the IOP, optionally preceded by an extra data block, and returns the DMA id (0 on
 * failure).
 *
 * The packet size must be 0x10..0x70. With extra data it first queues a DMA of a6 bytes from a4 to a5 and records the
 * destination and size in the packet header; flag 4 writes the extra data back from the data cache first
 * (func_00426358). The packet itself (size byte and command id at +8 filled in, written back with func_00426358) goes
 * to the IOP receive address D_0052BCE0 with attribute 0x44. Flag 1 queues both with isceSifSetDma (interrupt context),
 * otherwise sceSifSetDma.
 *
 * @param cmdId  the command id
 * @param sendFlags  flags: 1 interrupt context, 4 write back the extra data
 * @param packet  the packet
 * @param packetSize  the packet size
 * @param extraSrc  the extra data source
 * @param extraDest  the extra data destination on the IOP
 * @param extraSize  the extra data size (0 for none)
 * @return the DMA id, or 0
 */
int func_00426078_d(int cmdId, int sendFlags, int *packet, unsigned packetSize, int extraSrc, int extraDest, int extraSize) {
    sSifDmaData ent[2];
    int n;
    if (packetSize - 0x10 > 0x60) {
        return 0;
    }
    n = 0;
    if (extraSize > 0) {
        ent[0].data = extraSrc;
        n = 1;
        ent[0].addr = extraDest;
        packet[1] = extraDest;
        packet[0] = *(unsigned char *)packet | (extraSize << 8);
        ent[0].size = extraSize;
        ent[0].attr = 0;
        if (sendFlags & 4) {
            func_00426358(extraSrc, extraSize);
        }
    } else {
        packet[1] = 0;
        packet[0] = *(unsigned char *)packet;
    }
    ent[n].data = (int)packet;
    ent[n].addr = D_0052BCE0;
    ent[n].size = packetSize;
    packet[2] = cmdId;
    *(unsigned char *)packet = packetSize;
    ent[n].attr = 68;
    func_00426358((int)packet, packetSize);
    n++;
    if (sendFlags & 1) {
        return isceSifSetDma(ent, n);
    }
    return sceSifSetDma(ent, n);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSendCmd);
#ifdef SKIP_ASM
extern int func_00426078(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
/**
 * Sends a SIF command packet to the IOP from thread context and returns the DMA id (0 on failure).
 *
 * Calls func_00426078 with flags 0. Callers include sceSifCallRpc, sceSifExecRequest, sceSifBindRpc and sceSifGetOtherData.
 *
 * @param cmdId       the command id
 * @param packet      the packet
 * @param packetSize  its size (0x10..0x70)
 * @param extraSrc    the extra data source
 * @param extraDest   the extra data destination
 * @param extraSize   the extra data size (0 for none)
 * @return the DMA id, or 0
 */
int sceSifSendCmd(int cmdId, int packet, int packetSize, int extraSrc, int extraDest, int extraSize) {
    return func_00426078(cmdId, 0, packet, packetSize, extraSrc, extraDest, extraSize);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", isceSifSendCmd);
#ifdef SKIP_ASM
extern int func_00426078(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
/**
 * Sends a SIF command packet to the IOP from interrupt context and returns the DMA id (0 on failure).
 *
 * Calls func_00426078 with flag 1 (isceSifSetDma). The RPC command handlers func_00426820, func_00426A98 and
 * func_004267C0 reply with it.
 *
 * @param cmdId       the command id
 * @param packet      the packet
 * @param packetSize  its size (0x10..0x70)
 * @param extraSrc    the extra data source
 * @param extraDest   the extra data destination
 * @param extraSize   the extra data size (0 for none)
 * @return the DMA id, or 0
 */
int isceSifSendCmd(int cmdId, int packet, int packetSize, int extraSrc, int extraDest, int extraSize) {
    return func_00426078(cmdId, 1, packet, packetSize, extraSrc, extraDest, extraSize);
}
#endif

INCLUDE_ASM("seg/seg_325220", func_00426230);

//100%
INCLUDE_ASM("seg/seg_325220", func_00426358);
#ifdef SKIP_ASM
/**
 * Writes a memory range back from the data cache: cache op 0x18 (hit writeback-invalidate) on each 64-byte line.
 *
 * $4 is the start address and $5 the size (nothing happens when it is 0 or less). Every line from the start's to the
 * last byte's gets the op with a sync before and after it: (count mod 8) lines one at a time, then 8 per pass. The SIF
 * command and RPC senders call it on packets and buffers before DMA.
 */
// PORT: PS2-only hand-scheduled data-cache flush loop (fixed registers, 8x unrolled; starts 4 mod 8, so file-scope asm); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 2\n.globl func_00426358\n.type func_00426358, @function\nfunc_00426358:\n"
    "lui $25, (0xFFFFFFC0 >> 16)\n"
    "ori $25, $25, (0xFFFFFFC0 & 0xFFFF)\n"
    "blez $5, 4f\n"
    "addu $10, $4, $5\n"
    "and $8, $4, $25\n"
    "addiu $10, $10, -0x1\n"
    "and $9, $10, $25\n"
    "subu $10, $9, $8\n"
    "srl $11, $10, 6\n"
    "addiu $11, $11, 0x1\n"
    "andi $9, $11, 0x7\n"
    "beqz $9, 2f\n"
    "srl $10, $11, 3\n"
    "1:\n"
    "sync\n"
    "cache 0x18, 0x0($8)\n"
    "sync\n"
    "addiu $9, $9, -0x1\n"
    "nop\n"
    "bgtz $9, 1b\n"
    "addiu $8, $8, 0x40\n"
    "2:\n"
    "beqz $10, 4f\n"
    "3:\n"
    "addiu $10, $10, -0x1\n"
    "sync\n"
    "cache 0x18, 0x0($8)\n"
    "sync\n"
    "cache 0x18, 0x40($8)\n"
    "sync\n"
    "cache 0x18, 0x80($8)\n"
    "sync\n"
    "cache 0x18, 0xC0($8)\n"
    "sync\n"
    "cache 0x18, 0x100($8)\n"
    "sync\n"
    "cache 0x18, 0x140($8)\n"
    "sync\n"
    "cache 0x18, 0x180($8)\n"
    "sync\n"
    "cache 0x18, 0x1C0($8)\n"
    "sync\n"
    "bgtz $10, 3b\n"
    "addiu $8, $8, 0x200\n"
    "4:\n"
    "jr $31\n"
    "nop\n"
    ".size func_00426358, . - func_00426358\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426404);
#ifdef SKIP_ASM
/**
 * A lone return (jr $31) left after the hand-written cache flush func_00426358, its delay slot being the next
 * function's first instruction. Nothing calls it.
 */
// PORT: PS2-only. A lone `jr $31` after the hand-written cache flush func_00426358 (same .s file in the original): its
// delay slot is the first instruction of the next object's function, so it is written in asm with no nop. A port
// drops it.
__asm__(
    ".set push\n.set noreorder\n"
    ".text\n.align 2\n.globl func_00426404\n.type func_00426404, @function\nfunc_00426404:\n"
    "jr $31\n"
    ".size func_00426404, . - func_00426404\n"
    ".set pop\n");
#endif

INCLUDE_ASM("seg/seg_325220", func_00426408);

//100%
INCLUDE_ASM("seg/seg_325220", sceSifExitRpc);
#ifdef SKIP_ASM
extern void sceSifExitCmd(void);
extern int D_00455180;
/**
 * Shuts the SIF RPC layer down.
 *
 * Calls sceSifExitCmd and clears the RPC initialised flag D_00455180. sceSifRebootIop calls it.
 */
void sceSifExitRpc(void) {
    sceSifExitCmd();
    D_00455180 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004265D0);
#ifdef SKIP_ASM
extern int DIntr_rq(void) __asm__("DIntr");
extern void EIntr(void);
sSifRpcPkt *f65D0(sSifRpcState *s) __asm__("func_004265D0");
/**
 * Takes a free RPC packet from the packet table and returns it, or null.
 *
 * With interrupts off it scans the table for an entry whose word +0x10 has bit 0 clear, marks it (index << 16 | 5),
 * stores its own address at +0x14 and a new packet id at +0x18. The id comes from the counter (+0); the first call
 * hands out 1 and leaves the counter at 2, so id 2 is never used. sceSifCallRpc, sceSifBindRpc and sceSifGetOtherData call
 * it.
 *
 * @param rpcState  the RPC state D_0052D680
 * @return the packet, or null when all are in use
 */
sSifRpcPkt *f65D0(sSifRpcState *rpcState) {
    sSifRpcPkt *packet;
    int i;
    int n;
    int prevId;
    int id;
    int seq;
    DIntr_rq();
    n = rpcState->pkt_table_len;
    i = 0;
    packet = rpcState->pkt_table;
    while (i < n) {
        if (!(packet->f10 & 1)) {
            packet->f10 = (i << 16) | 5;
            prevId = rpcState->pid;
            id = prevId + 1;
            rpcState->pid = id;
            if (id == 1) {
                rpcState->pid = prevId + 2;
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
INCLUDE_ASM("seg/seg_325220", func_00426678);
#ifdef SKIP_ASM
/**
 * Releases an RPC packet: clears its id and its in-use bit.
 *
 * @param packet  the packet
 */
void func_00426678(unsigned int *packet) {
    packet[6] = 0;
    packet[4] &= 0xFFFFFFFE;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426698);
#ifdef SKIP_ASM
char *f6698(sSifRpcState *a0) __asm__("func_00426698");
/**
 * Returns the next 64-byte receive packet of the RPC state, round robin.
 *
 * Advances the receive index modulo the receive buffer count.
 *
 * @param rpcState  the RPC state
 * @return the packet
 */
char *f6698(sSifRpcState *rpcState) {
    int slot = rpcState->rbuf_idx % rpcState->rbuf_len;
    char *buf = rpcState->rbuf + slot * 64;
    rpcState->rbuf_idx = slot + 1;
    return buf;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004266C8);
#ifdef SKIP_ASM
extern int func_00426698(sSifRpcState *a0);
/**
 * Returns the numbered 64-byte RPC data packet, or the next round-robin receive packet when the number is out of range.
 *
 * @param rpcState  the RPC state
 * @param slot      the packet number
 * @return the packet address
 */
int func_004266C8(sSifRpcState *rpcState, int slot) {
    if (slot < 0 || slot >= rpcState->rdata_len) {
        return func_00426698(rpcState);
    }
    return rpcState->rdata + (slot << 6);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426708);
#ifdef SKIP_ASM
struct Q6708 { int f0; int pad4; int f8; char padC[0x10]; int f1C; int f20; int f24; };
struct A6708 { char pad[0x1C]; struct Q6708 *q; unsigned f20; int f24; int f28; int f2C; };
extern void iSignalSema(int);
extern void func_00426678_k(int) __asm__("func_00426678");
void f6708(struct A6708 *a) __asm__("func_00426708");
/**
 * Handles the IOP's RPC end command: completes a call or bind and releases the waiting client.
 *
 * The finished request type is at +0x20: 0x8000000A (call) runs the client's end_function with end_param; 0x80000009
 * (bind) stores the server, buff and cbuff from the packet; 0x8000000B does nothing. Then it signals the client's
 * semaphore (rpcd[2]) when it has one, releases its packet with func_00426678 and clears the client's packet pointer.
 * func_00426408 installs it for command 0x80000008.
 *
 * @param endRequest  the received end packet
 */
void f6708(struct A6708 *endRequest) {
    struct Q6708 *clientData;
    switch (endRequest->f20) {
    case 0x8000000A:
        clientData = endRequest->q;
        if (clientData->f1C != 0) {
            ((void (*)(int))clientData->f1C)(clientData->f20);
        }
        break;
    case 0x80000009:
        clientData = endRequest->q;
        clientData->f24 = endRequest->f24;
        ((int *)clientData)[5] = endRequest->f28;
        ((int *)clientData)[6] = endRequest->f2C;
        break;
    case 0x8000000B:
        break;
    }
    clientData = endRequest->q;
    if (clientData->f8 >= 0) {
        iSignalSema(clientData->f8);
    }
    func_00426678_k(clientData->f0);
    clientData->f0 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004267C0);
#ifdef SKIP_ASM
extern int isceSifSendCmd(int a0, int a1, int a2, int a3, int a4, int a5);
extern void iSetAlarm(int a0, int (*a1)(int, int, int *), int *a2);
/**
 * Retries sending an RPC reply from an alarm until the command DMA accepts it.
 *
 * Sends the 0x40-byte packet as command 0x80000008 with its extra data (packet words 9..11) through isceSifSendCmd; when
 * that fails it re-arms itself with iSetAlarm (10 ticks). Declared int but returns no value. func_00426820 sets the
 * first alarm.
 *
 * @param alarmId    the alarm id (unused)
 * @param alarmTime  the alarm time (unused)
 * @param packet     the reply packet
 * @return no defined value (declared int, but the body has no return statement)
 */
int func_004267C0(int alarmId, int alarmTime, int *packet) {
    if (isceSifSendCmd(0x80000008, (int)packet, 0x40, packet[9], packet[10], packet[11]) == 0) {
        iSetAlarm(10, func_004267C0, packet);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426820);
#ifdef SKIP_ASM
extern int func_00426698(sSifRpcState *a0);
extern int func_004266C8(sSifRpcState *a0, int a1);
extern int isceSifSendCmd(int a0, int a1, int a2, int a3, int a4, int a5);
extern void iSetAlarm(int a0, int (*a1)(int, int, int *), int *a2);
extern int func_004267C0(int a0, int a1, int *p);
extern int func_004266C8_u(sSifRpcState *, unsigned) __asm__("func_004266C8");
/**
 * Handles the IOP's get-other-data command: copies the requested block and sends the end reply.
 *
 * Takes a reply packet (the numbered one when request flag 4 is set, else round robin), copies the packet address and
 * client, sets the type 0x8000000C and the source, destination and size, and sends it as command 0x80000008 with the
 * block as extra data through isceSifSendCmd. When the DMA queue is full it retries from an alarm (iSetAlarm with
 * func_004267C0). func_00426408 installs it for command 0x8000000C.
 *
 * @param request   the received request packet
 * @param rpcState  the RPC state
 */
void func_00426820(int *request, sSifRpcState *rpcState) {
    int *reply;
    unsigned fl = request[4];
    int pktAddr;
    int client;
    if (fl & 4) {
        reply = (int *)func_004266C8_u(rpcState, fl >> 16);
    } else {
        reply = (int *)func_00426698(rpcState);
    }
    pktAddr = request[5];
    client = request[7];
    reply[5] = pktAddr;
    reply[7] = client;
    reply[8] = 0x8000000C;
    reply[9] = request[8];
    reply[10] = request[9];
    reply[11] = request[10];
    if (isceSifSendCmd(0x80000008, (int)reply, 0x40, request[8], request[9], request[10]) == 0) {
        iSetAlarm(10, func_004267C0, reply);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifGetOtherData);
#ifdef SKIP_ASM
struct S68F0 { struct S68F0 *f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; int f28; };
extern struct S68F0 *func_004265D0_p(sSifRpcState *) __asm__("func_004265D0");
extern void func_00426678_j(struct S68F0 *) __asm__("func_00426678");
extern int CreateSema_j(int *) __asm__("CreateSema");
extern int WaitSema_j(int) __asm__("WaitSema");
extern int sceSifSendCmd_j(int, int, int, int, int, int) __asm__("sceSifSendCmd");
int sceSifGetOtherData_d(struct S68F0 *, int, int, int, int) __asm__("sceSifGetOtherData");
/**
 * Asks the IOP to copy a block of IOP memory into EE memory and returns 0, or a negative error.
 *
 * Takes a packet with func_004265D0 (-1 when none), links it to the receive data (packet +0, id +4), fills the source,
 * destination and size (+0x20..+0x28) and the client (+0x1C), and sends command 0x8000000C with sceSifSendCmd. Without
 * mode bit 0 it waits on a new semaphore (stored at +8, -3 when it cannot be created) that the end handler signals,
 * then deletes it. A failed send releases the packet and returns -2. With bit 0 the semaphore field is -1 and it
 * returns at once.
 *
 * @param receiveData  the receive data
 * @param srcAddr  the IOP source address
 * @param destAddr  the EE destination
 * @param byteCount  the byte count
 * @param mode  the mode (bit 0: do not wait)
 * @return 0, -1 (no packet), -2 (send failed) or -3 (no semaphore)
 */
int sceSifGetOtherData_d(struct S68F0 *receiveData, int srcAddr, int destAddr, int byteCount, int mode) {
    int buf[8];
    struct S68F0 *packet;
    packet = func_004265D0_p(&D_0052D680);
    if (packet == 0) {
        return -1;
    }
    receiveData->f0 = packet;
    receiveData->f4 = packet->f18;
    packet->f20 = srcAddr;
    packet->f24 = destAddr;
    packet->f28 = byteCount;
    packet->f14 = (int)packet;
    packet->f1C = (int)receiveData;
    if (!(mode & 1)) {
        buf[1] = 1;
        buf[2] = 0;
        receiveData->f8 = CreateSema_j(buf);
        if (receiveData->f8 < 0) {
            func_00426678_j(packet);
            return -3;
        }
        if (sceSifSendCmd_j(0x8000000C, (int)packet, 0x40, 0, 0, 0) == 0) {
            func_00426678_j(packet);
            DeleteSema(receiveData->f8);
            return -2;
        }
        WaitSema_j(receiveData->f8);
        DeleteSema(receiveData->f8);
        return 0;
    }
    receiveData->f8 = -1;
    if (sceSifSendCmd_j(0x8000000C, (int)packet, 0x40, 0, 0, 0) == 0) {
        func_00426678_j(packet);
        return -2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426A48);
#ifdef SKIP_ASM
/**
 * Finds the RPC server registered under an id and returns it, or null.
 *
 * Walks every queue's server list and compares each server's key (+0). func_00426A98 calls it.
 *
 * @param serverId  the server id
 * @param rpcState  the RPC state
 * @return the server, or null
 */
sSifServer *func_00426A48(int serverId, sSifRpcState *rpcState) {
    sSifQueue *queue;
    sSifServer *n;
    for (queue = rpcState->queues; queue != 0; queue = queue->next) {
        for (n = queue->servers; n != 0; n = n->next) {
            if (n->key == serverId) {
                return n;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426A98);
#ifdef SKIP_ASM
extern int func_00426698(sSifRpcState *a0);
extern int *func_00426A48_p(int, sSifRpcState *) __asm__("func_00426A48");
extern void isceSifSendCmd_v(int, int *, int, int, int, int) __asm__("isceSifSendCmd");
/**
 * Handles the IOP's RPC bind command: looks up the server and sends the end reply.
 *
 * Takes a round-robin reply packet, copies the packet address and client, sets the type 0x80000009 and the server found
 * by func_00426A48 with its words at +8 and +0x14 (zeros when there is none), and sends it as command 0x80000008 with
 * isceSifSendCmd. func_00426408 installs it for command 0x80000009.
 *
 * @param request   the received request packet
 * @param rpcState  the RPC state
 */
void func_00426A98(int *request, sSifRpcState *rpcState) {
    int *reply = (int *)func_00426698(rpcState);
    int *n;
    int pktAddr = request[5];
    int client = request[7];
    reply[7] = client;
    reply[5] = pktAddr;
    reply[8] = 0x80000009;
    n = func_00426A48_p(request[8], rpcState);
    if (n == 0) {
        reply[9] = 0;
        reply[10] = 0;
        reply[11] = 0;
    } else {
        reply[9] = (int)n;
        reply[10] = n[2];
        reply[11] = n[5];
    }
    isceSifSendCmd_v(0x80000008, reply, 0x40, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifBindRpc);
#ifdef SKIP_ASM
struct S6B48 { struct S6B48 *f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; };
extern struct S6B48 *func_004265D0(sSifRpcState *);
extern void func_00426678_i(struct S6B48 *) __asm__("func_00426678");
extern int CreateSema(int *);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceSifBindRpc_d(struct S6B48 *, int, int) __asm__("sceSifBindRpc");
/**
 * Binds an RPC client to the IOP server with the given number and returns 0, or a negative error.
 *
 * Clears the client's command and server fields, takes a packet with func_004265D0 (-1 when none), links it to the
 * client (rpcd[0], rpcd[1]), stores the server number and client, and sends command 0x80000009 with sceSifSendCmd.
 * Without mode bit 0 it waits on a new semaphore (rpcd[2], -3 when it cannot be created) that the end handler signals;
 * a failed send returns -2. The server field stays 0 when the IOP has no such server yet, so callers retry. Callers
 * include sceSifInitIopHeap and SNDPLATFORM_init.
 *
 * @param client  the client data
 * @param rpcNumber  the server number
 * @param mode  the mode (bit 0: do not wait)
 * @return 0, -1 (no packet), -2 (send failed) or -3 (no semaphore)
 */
int sceSifBindRpc_d(struct S6B48 *client, int rpcNumber, int mode) {
    int buf[8];
    struct S6B48 *packet;
    client->f10 = 0;
    client->f24 = 0;
    packet = func_004265D0(&D_0052D680);
    if (packet == 0) {
        return -1;
    }
    client->f0 = packet;
    client->f4 = packet->f18;
    packet->f20 = rpcNumber;
    packet->f14 = (int)packet;
    packet->f1C = (int)client;
    if (!(mode & 1)) {
        buf[1] = 1;
        buf[2] = 0;
        client->f8 = CreateSema(buf);
        if (client->f8 < 0) {
            func_00426678_i(packet);
            return -3;
        }
        if (sceSifSendCmd(0x80000009, (int)packet, 0x40, 0, 0, 0) == 0) {
            func_00426678_i(packet);
            DeleteSema(client->f8);
            return -2;
        }
        WaitSema_i(client->f8);
        DeleteSema(client->f8);
        return 0;
    }
    client->f8 = -1;
    if (sceSifSendCmd(0x80000009, (int)packet, 0x40, 0, 0, 0) == 0) {
        func_00426678_i(packet);
        return -2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426C88);
#ifdef SKIP_ASM
struct B26C88 {
    int f0;
    int f4;
    int pad8;
    int head;
    int tail;
};
struct E26C88 {
    int pad0[3];
    int fC;
    int pad10[3];
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int pad38;
    int next;
    struct B26C88 *owner;
};
struct A26C88 {
    int pad0[4];
    int f10;
    int f14;
    int pad18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    struct E26C88 *e;
};
extern void iWakeupThread(int);
/**
 * Handles the IOP's RPC call command: queues the call on its server and wakes the server thread.
 *
 * Appends the server data (request +0x34) to its queue (head +0xC, tail +0x10, link +0x3C), copies the packet address,
 * client, call number, send size, receive buffer, receive size, receive mode and packet id into it, and wakes the
 * queue's thread (+0) with iWakeupThread when the id is valid and the queue is not active (+4). func_00426408 installs
 * it for command 0x8000000A.
 *
 * @param callRequest  the received call packet
 */
void func_00426C88(struct A26C88 *callRequest) {
    struct E26C88 *serverData = callRequest->e;
    struct B26C88 *rpcQueue = serverData->owner;
    if (rpcQueue->head == 0) {
        rpcQueue->head = (int)serverData;
    } else {
        ((struct E26C88 *)rpcQueue->tail)->next = (int)serverData;
    }
    rpcQueue->tail = (int)serverData;
    {
        int packetAddr = callRequest->f14;
        int clientAddr = callRequest->f1C;
        serverData->f20 = packetAddr;
        serverData->f1C = clientAddr;
    }
    serverData->f24 = callRequest->f20;
    serverData->fC = callRequest->f24;
    serverData->f28 = callRequest->f28;
    serverData->f2C = callRequest->f2C;
    serverData->f30 = callRequest->f30;
    serverData->f34 = callRequest->f10;
    if (rpcQueue->f0 >= 0 && rpcQueue->f4 == 0) {
        iWakeupThread(rpcQueue->f0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifCallRpc);
#ifdef SKIP_ASM
/* sceSifCallRpc (libkernel sifrpc): send an RPC call packet; wait for the reply unless mode bit 0 (no wait) is set. */
// FAKEMATCH: every void* field of the client view (rpcd.paddr, buff, para, serve) and of the call packet (pkt, recv,
// server) is a one-member union (uPtr6D18), which gives those accesses alias set 0 (TBAA steering: retail's void*
// accesses conflict with every other store); plain form 93.93%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
typedef union { void *p; } uPtr6D18;
struct SifServ6D18;
/* the SDK's sceSifClientData (sSifClient), as seen here */
struct SifClient6D18 {
    struct {
        uPtr6D18 paddr;             /* 0x00 packet */
        unsigned int pid;           /* 0x04 packet id */
        int tid;                    /* 0x08 semaphore (or -1) */
        unsigned int mode;          /* 0x0C */
    } rpcd;
    unsigned int command;           /* 0x10 */
    uPtr6D18 buff;                  /* 0x14 */
    void *cbuff;                    /* 0x18 */
    void (*func)(void *);           /* 0x1C end function */
    uPtr6D18 para;                  /* 0x20 end function argument */
    uPtr6D18 serve;                 /* 0x24 bound server */
};
/* RPC call packet (0x40 bytes) */
struct Call6D18 {
    int f0; int f4; int f8; int fC; int f10;
    uPtr6D18 pkt;                   /* 0x14 */
    int rpc_id;                     /* 0x18 */
    sSifClient *client;             /* 0x1C */
    int rpc_number;                 /* 0x20 */
    int send_size;                  /* 0x24 */
    uPtr6D18 recv;                  /* 0x28 */
    int recv_size;                  /* 0x2C */
    int rmode;                      /* 0x30 */
    uPtr6D18 server;                /* 0x34 */
};
struct Sema6D18 { int count; int max_count; int init_count; int wait_threads; unsigned attr; unsigned option; };
extern struct Call6D18 *f6D18_get(sSifRpcState *) __asm__("func_004265D0");
extern void f6D18_free(struct Call6D18 *) __asm__("func_00426678");
extern void f6D18_wb(void *, int) __asm__("func_00426358");
extern int f6D18_send(int, void *, int, void *, void *, int) __asm__("sceSifSendCmd");
extern int f6D18_csema(struct Sema6D18 *) __asm__("CreateSema");
extern int f6D18_wsema(int) __asm__("WaitSema");
/**
 * Sends a SIF RPC call to the IOP server a bound client points at and, unless asked not to, waits for the reply.
 *
 * Takes a free 0x40-byte call packet from the RPC state D_0052D680 (func_004265D0; -1 when none is free) and fills it:
 * the function number, send size, receive buffer and size, the packet's own address, the client's bound server and the
 * client. The client records the packet, its id, the end function and its argument. Unless mode bit 1 is set, it writes
 * the send and receive buffers back from the data cache first (func_00426358; once with the larger size when they are
 * the same buffer). The packet goes out as SIF command 0x8000000A with the send data DMA'd to the client's server
 * buffer (sceSifSendCmd). With mode bit 0 set (no wait) the client's semaphore is -1, the packet's reply mode is 1 only
 * when @p end_function is set, and it returns at once. Otherwise it creates a semaphore (max 1, initial 0), waits on it
 * until the reply handler signals it, then deletes it. A failed send frees the packet (func_00426678). Called by the
 * file, heap and module-load wrappers in this unit and by the pad, memory card and USB keyboard libraries.
 *
 * @param client  the bound RPC client (its packet id, semaphore, end function and argument are set here)
 * @param fno  the server's function number
 * @param mode  bit 0: do not wait for the reply; bit 1: skip the data cache write-back
 * @param send  data sent with the call
 * @param ssize  bytes of @p send
 * @param recv  buffer the server's reply is copied to
 * @param rsize  bytes of @p recv
 * @param end_function  function run when the reply arrives (no-wait mode), or 0
 * @param end_param  argument passed to @p end_function
 * @return 0 when sent (and answered, in wait mode); -1 no free packet, -2 the send failed, -3 no semaphore
 */
int sceSifCallRpc(sSifClient *client, int fno, int mode, void *send, int ssize, void *recv, int rsize,
                  void (*end_function)(void *), void *end_param) {
    struct SifClient6D18 *cd = (struct SifClient6D18 *)client;
    struct Sema6D18 sema;
    struct Call6D18 *call;
    call = f6D18_get(&D_0052D680);
    if (call == 0)
        return -1;
    cd->rpcd.pid = call->rpc_id;
    cd->para.p = end_param;
    cd->rpcd.paddr.p = call;
    cd->func = end_function;
    call->rpc_number = fno;
    call->send_size = ssize;
    call->recv.p = recv;
    call->recv_size = rsize;
    call->pkt.p = call;
    call->server.p = cd->serve.p;
    call->client = client;
    if (!(mode & 2)) {
        if (send == recv) {
            f6D18_wb(send, ssize < rsize ? rsize : ssize);
        } else {
            if (ssize > 0)
                f6D18_wb(send, ssize);
            if (rsize > 0)
                f6D18_wb(recv, rsize);
        }
    }
    if (mode & 1) {
        if (!end_function)
            call->rmode = 0;
        else
            call->rmode = 1;
        cd->rpcd.tid = -1;
        if (f6D18_send(0x8000000A, call, 0x40, send, cd->buff.p, ssize))
            return 0;
        f6D18_free(call);
        return -2;
    }
    sema.max_count = 1;
    sema.init_count = 0;
    cd->rpcd.tid = f6D18_csema(&sema);
    if (cd->rpcd.tid < 0) {
        f6D18_free(call);
        return -3;
    }
    call->rmode = 1;
    if (f6D18_send(0x8000000A, call, 0x40, send, cd->buff.p, ssize) == 0) {
        DeleteSema(cd->rpcd.tid);
        f6D18_free(call);
        return -2;
    }
    f6D18_wsema(cd->rpcd.tid);
    DeleteSema(cd->rpcd.tid);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifCheckStatRpc);
#ifdef SKIP_ASM
/**
 * Returns whether an RPC client's last call is still in progress.
 *
 * Reads the client's packet pointer (word 0): returns 1 when the packet exists, its id (+0x18) still equals the id the
 * client recorded (word 1) and its in-use bit (bit 0 of +0x10) is set; 0 otherwise. Polled by the pad, memory card and
 * USB keyboard library code after no-wait calls of sceSifCallRpc.
 *
 * @param client  the RPC client data
 * @return 1 while the call is pending, else 0
 */
int sceSifCheckStatRpc(int *client) {
    int *packet = (int *)client[0];
    if (packet == 0 || client[1] != packet[6] || !(packet[4] & 1)) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSetRpcQueue);
#ifdef SKIP_ASM
extern int DIntr_r(void) __asm__("DIntr");
extern void EIntr(void);
/**
 * Initialises an RPC server queue for a thread and appends it to the RPC state's queue list.
 *
 * With interrupts disabled (DIntr ... EIntr) it sets the queue's thread id (f0) to @p threadId, clears its
 * active flag (f4), its server list, the request start and end pointers (fC, f10) and its link, then appends it to the
 * end of D_0052D680.queues (the list func_00426A48 searches when a bind request names a server). No caller in this
 * build.
 *
 * @param n  the queue to set up
 * @param threadId  id of the thread that serves the queue (the one woken when a request arrives)
 */
void sceSifSetRpcQueue(sSifQueue *n, int threadId) {
    sSifQueue *lastQueue;
    sSifQueue *firstQueue;
    DIntr_r();
    n->f0 = threadId;
    n->f4 = 0;
    n->servers = 0;
    n->fC = 0;
    n->f10 = 0;
    firstQueue = D_0052D680.queues;
    n->next = 0;
    if (firstQueue == 0) {
        D_0052D680.queues = n;
    } else {
        lastQueue = firstQueue;
        for (; lastQueue->next != 0; lastQueue = lastQueue->next)
            ;
        lastQueue->next = n;
    }
    EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifRegisterRpc);
#ifdef SKIP_ASM
struct N6FE0 { int f0; int f4; int f8; int fC; int f10; int f14; char pad[0x20]; struct N6FE0 *next; struct N6FE0 *f3C; struct S6FE0 *f40; };
struct S6FE0 { int f0; int f4; struct N6FE0 *head; };
extern int DIntr_r(void) __asm__("DIntr");
extern void EIntr(void);
/**
 * Registers an RPC server (id, service function and buffers) on a server queue.
 *
 * With interrupts disabled (DIntr ... EIntr) it fills the server record: id (+0), service function
 * (+4), receive buffer (+8), cancel function (+0x10) and its buffer (+0x14), the owning queue (+0x40), and clears its
 * two link words (+0x38, +0x3C); then appends it to the end of the queue's server list (+8, linked through +0x38). No
 * caller in this build.
 *
 * @param n  the server record to register
 * @param serverId  server id that clients bind to
 * @param serveFunc  service function called for each request
 * @param serveBuf  buffer that receives the request data
 * @param cancelFunc  cancel function
 * @param cancelBuf  buffer for the cancel function
 * @param serverQueue  the queue the server is added to
 */
void sceSifRegisterRpc(struct N6FE0 *n, int serverId, int serveFunc, int serveBuf, int cancelFunc, int cancelBuf, struct S6FE0 *serverQueue) {
    struct N6FE0 *lastServer;
    struct N6FE0 *firstServer;
    DIntr_r();
    n->f3C = 0;
    n->next = 0;
    n->f0 = serverId;
    firstServer = serverQueue->head;
    n->f4 = serveFunc;
    n->f8 = serveBuf;
    n->f10 = cancelFunc;
    n->f14 = cancelBuf;
    n->f40 = serverQueue;
    if (firstServer == 0) {
        serverQueue->head = n;
    } else {
        lastServer = firstServer;
        for (; lastServer->next != 0; lastServer = lastServer->next)
            ;
        lastServer->next = n;
    }
    EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifRemoveRpc);
#ifdef SKIP_ASM
struct N70B0 { char pad[0x38]; struct N70B0 *next; };
struct S70B0 { int f0; int f4; struct N70B0 *head; };
extern int DIntr_r(void) __asm__("DIntr");
extern void EIntr(void);
/**
 * Unlinks a server record from a queue's server list.
 *
 * With interrupts disabled it walks the list at @p list +8 (linked through +0x38): a head match is replaced by its
 * successor; otherwise the predecessor of @p x is relinked past it. No caller in this build.
 *
 * @param x  the server record to remove
 * @param list  the queue that holds it
 * @return the old head when @p x was the head, else the predecessor of @p x (0 when not found)
 */
struct N70B0 *sceSifRemoveRpc(struct N70B0 *x, struct S70B0 *list) {
    struct N70B0 *cursor;
    DIntr_r();
    cursor = list->head;
    if (cursor == x) {
        list->head = cursor->next;
    } else {
        while (cursor != 0) {
            if (cursor->next == x) {
                cursor->next = x->next;
                break;
            }
            cursor = cursor->next;
        }
    }
    EIntr();
    return cursor;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifRemoveRpcQueue);
#ifdef SKIP_ASM
extern int f7148a(void) __asm__("DIntr");
extern void f7148b(void) __asm__("EIntr");
sSifQueue *f7148(sSifQueue *x) __asm__("sceSifRemoveRpcQueue");
/**
 * Unlinks a server queue from the RPC state's queue list.
 *
 * With interrupts disabled it walks D_0052D680.queues: a head match is replaced by its successor; otherwise the
 * predecessor of @p x is relinked past it. No caller in this build.
 *
 * @param x  the queue to remove
 * @return the old head when @p x was the head, else the predecessor of @p x (0 when not found)
 */
sSifQueue *f7148(sSifQueue *x) {
    sSifQueue *queueCursor;
    f7148a();
    queueCursor = D_0052D680.queues;
    if (queueCursor == x) {
        D_0052D680.queues = queueCursor->next;
    } else {
        for (; queueCursor != 0; queueCursor = queueCursor->next) {
            if (queueCursor->next == x) {
                queueCursor->next = x->next;
                break;
            }
        }
    }
    f7148b();
    return queueCursor;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifGetNextRequest);
#ifdef SKIP_ASM
struct N4271D8 { char pad[0x3C]; int next; };
struct S4271D8 { char pad[4]; int f4; char pad2[4]; struct N4271D8 *head; };
extern void DIntr(void);
extern void EIntr(void);
/**
 * Takes the next pending request (server record) off a server queue.
 *
 * With interrupts disabled it pops the head of the queue's request list (+0xC, linked through +0x3C) and sets the
 * queue's active flag (+4) to 1, or to 0 when the list is empty. Only sceSifRpcLoop calls it.
 *
 * @param queueAddr  the server queue (as an int)
 * @return the server record with a request to run, or 0
 */
int sceSifGetNextRequest(int queueAddr) {
    struct S4271D8 *serverQueue = (struct S4271D8 *)queueAddr;
    struct N4271D8 *n;
    DIntr();
    n = serverQueue->head;
    if (n == 0) {
        serverQueue->f4 = 0;
    } else {
        struct N4271D8 *nextServer = (struct N4271D8 *)n->next;
        serverQueue->f4 = 1;
        serverQueue->head = nextServer;
    }
    EIntr();
    return (int)n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifExecRequest);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`ASSERT_7230(e != 0)`, `do { } while (0)`) after the lookup, and the stand-in local `w = 0x8000000A` (with `v = a0->f1C` read first) for the two header stores; plain form 98.10% (98.01% without the ASSERT only, 97.95% without `w` only). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct W7230 { int f0; int (*f4)(int, int, int); int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; int f28; int f2C; int f30; int f34; };
struct R7230 { int f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; };
extern void func_00426358(int, int);
extern void DIntr_w(void) __asm__("DIntr");
extern int EIntr_w(void) __asm__("EIntr");
extern struct R7230 *func_004266C8_w(sSifRpcState *, unsigned) __asm__("func_004266C8");
extern struct R7230 *func_00426698_w(sSifRpcState *, unsigned) __asm__("func_00426698");
extern int sceSifSendCmd_w(int, void *, int, int, int, int) __asm__("sceSifSendCmd");
void sceSifExecRequest_d(struct W7230 *) __asm__("sceSifExecRequest");
// Release-build assert: compiles to an empty do { } while (0) (its loop notes still shape the code)
#define ASSERT_7230(cond) do { } while (0)
/**
 * Runs one RPC request on its server and sends the result back to the EE-side caller.
 *
 * Calls the server's service function with the request's function number, receive buffer and size (+0x24, +8, +0xC); a
 * non-null result is returned with the server's reply size (+0x2C). Writes the receive buffer and the result back from
 * the data cache (func_00426358), then, with interrupts disabled, takes a reply packet: the numbered packet from the
 * high half of +0x34 when its bit 2 is set (func_004266C8), else the next round-robin one (func_00426698). The reply
 * carries the client (+0x1C) and command 0x8000000A. In reply mode (+0x30 set) it sends SIF command 0x80000008 with the
 * result DMA'd to the caller's buffer (+0x28) through sceSifSendCmd, retrying until it is accepted. Otherwise it clears
 * the packet's id and in-use words and DMAs the result (when any) and then the packet itself to the caller's packet
 * address (+0x20) with sceSifSetDma, spinning 0x100000 iterations between refused attempts. Only sceSifRpcLoop calls
 * it.
 *
 * @param serveData  the server record holding the request
 */
void sceSifExecRequest_d(struct W7230 *serveData) {
    int replySize = 0;
    int replyData;
    int n;
    int i;
    int dmaId;
    int fl;
    int clientAddr;
    int w;
    int x4;
    struct R7230 *e;
    sSifDmaData ent[2];
    replyData = serveData->f4(serveData->f24, serveData->f8, serveData->fC);
    if (replyData != 0) {
        replySize = serveData->f2C;
    }
    if (serveData->fC > 0) {
        func_00426358(serveData->f8, serveData->fC);
    }
    if (replySize > 0) {
        func_00426358(replyData, replySize);
    }
    DIntr_w();
    fl = serveData->f34;
    x4 = fl & 4;
    fl = (unsigned)fl >> 16;
    if (x4) {
        e = func_004266C8_w(&D_0052D680, fl);
    } else {
        e = func_00426698_w(&D_0052D680, fl);
    }
    EIntr_w();
    ASSERT_7230(e != 0);
    w = 0x8000000A;
    clientAddr = serveData->f1C;
    e->f20 = w;
    e->f1C = clientAddr;
    if (serveData->f30 != 0) {
        while (sceSifSendCmd_w(0x80000008, e, 0x40, replyData, serveData->f28, replySize) == 0) {
        }
        return;
    }
    e->f18 = 0;
    n = 0;
    e->f10 = 0;
    if (replySize > 0) {
        n = 1;
        ent[0].data = replyData;
        ent[0].addr = serveData->f28;
        ent[0].size = replySize;
        ent[0].attr = 0;
    }
    ent[n].data = (int)e;
    ent[n].addr = serveData->f20;
    ent[n].size = 0x40;
    ent[n].attr = 0;
    n++;
    do {
        dmaId = sceSifSetDma(ent, n);
        if (dmaId == 0) {
            i = 0x100000;
            do {
                i--;
            } while (i != -1);
        }
    } while (dmaId == 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifRpcLoop);
#ifdef SKIP_ASM
extern int sceSifGetNextRequest(int a0);
extern void sceSifExecRequest(int a0);
extern void SleepThread(void);
/**
 * Serves a queue forever: runs every pending request, then sleeps until woken.
 *
 * Takes requests with sceSifGetNextRequest and runs each with sceSifExecRequest; when none is left it calls SleepThread and starts
 * over. Never returns. No caller in this build.
 *
 * @param queue  the server queue to serve
 * @return never returns
 */
int sceSifRpcLoop(int queue) {
    int server;
    while (1) {
        while ((server = sceSifGetNextRequest(queue)) != 0) {
            sceSifExecRequest(server);
        }
        SleepThread();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427438);
#ifdef SKIP_ASM
extern int CreateSema(int *p);
/**
 * Creates the file library's two semaphores the first time it is called.
 *
 * When D_00455218 is still -1, creates two semaphores (initial and max count 1, option 0): D_00455218 guards the
 * 32-entry descriptor table D_0052EC00 and D_0045521C guards the pending no-wait table D_00455188. Called by
 * func_00427498, func_00427520 and func_00427A90.
 */
void func_00427438(void) {
    int buf[8];
    if (D_00455218 == -1) {
        buf[5] = 0;
        buf[2] = 1;
        buf[1] = 1;
        D_00455218 = CreateSema(buf);
        D_0045521C = CreateSema(buf);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427498);
#ifdef SKIP_ASM
extern void func_00427438(void);
extern int WaitSema_i(int) __asm__("WaitSema");
extern void SignalSema(int);
/**
 * Allocates a free slot in the file library's descriptor table.
 *
 * Makes sure the semaphores exist (func_00427438), then under D_00455218 finds the first of the 32 entries of
 * D_0052EC00 whose flags (f4) are 0 and marks it in use with 0x10000000. Called by sceOpen and sceDopen.
 *
 * @return the slot, or 0 when all 32 are taken
 */
sFioFile *func_00427498(void) {
    sFioFile *fileSlot;
    sFioFile *end;
    func_00427438();
    WaitSema_i(D_00455218);
    fileSlot = D_0052EC00;
    end = fileSlot + 32;
    while (fileSlot < end) {
        if (fileSlot->f4 == 0) {
            fileSlot->f4 = 0x10000000;
            SignalSema(D_00455218);
            return fileSlot;
        }
        fileSlot++;
    }
    SignalSema(D_00455218);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427520);
#ifdef SKIP_ASM
extern void func_00427438(void);
extern int WaitSema_i(int) __asm__("WaitSema");
extern void SignalSema(int);
/**
 * Returns the descriptor-table slot for a file descriptor number.
 *
 * Makes sure the semaphores exist (func_00427438) and takes D_00455218 around the bounds check. The slot is returned
 * whether or not it is open; callers check its flags (f4).
 *
 * @param fdIndex  descriptor number (index into D_0052EC00)
 * @return &D_0052EC00[@p fdIndex], or 0 when it is 32 or more
 */
sFioFile *func_00427520(unsigned fdIndex) {
    sFioFile *fileSlot;
    func_00427438();
    WaitSema_i(D_00455218);
    if (fdIndex >= 0x20) {
        SignalSema(D_00455218);
        return 0;
    }
    fileSlot = &D_0052EC00[fdIndex];
    SignalSema(D_00455218);
    return fileSlot;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427590);
#ifdef SKIP_ASM
struct W7590 { char b[4]; };
struct B7590 { char b[0x144]; };
struct C7590 { char b[0x40]; };
struct L7590 { int n1; int n2; char *d1; char *d2; unsigned char b1[64]; unsigned char b2[64]; };
extern int D_00455208;
extern void *func_0041605C(void *, void *, int);
extern void iSignalSema(int);
/**
 * Handles the IOP file server's completion command: copies the result to the caller and wakes it.
 *
 * Installed by func_00427A90 as the handler of SIF command 0x80000011; runs in interrupt context. The reply lives in
 * one of the 0x440-byte buffers at D_0052E380, read through the uncached mirror (| 0x20000000): buffer 0, or the buffer
 * the packet's word 3 names when D_00455210 is set (the index is kept in D_00455208). Words 0..3 of the buffer are the
 * waiter's semaphore (negative for a no-wait call), the request number, the destination and the size; for a waiting
 * caller the result at +0x10 is copied to the destination (func_0041605C, memcpy). Then per request: 2 (read) writes
 * the unaligned head and tail bytes (up to 64 each, two counts, two addresses and two byte blocks from +0x14) to both
 * the cached and uncached views of the destination; 11 copies a 0x144-byte directory entry and 12 a 0x40-byte status
 * block to the address at +0x14; 23, 25 and 26 copy the length at +0x18 (capped at 0x400) of data from +0x1C. Finally a
 * no-wait call's entry is cleared to -1 in D_00455188, while a waiting caller's semaphore is signalled (iSignalSema).
 *
 * @param cmdPacket  the received SIF command packet
 */
void func_00427590(int *cmdPacket) {
    char *replyBuf;
    int sema;
    int cmd;
    char *dst;
    int size;
    char *buf;
    unsigned len;
    int idx;
    int i;
    D_00455208 = idx = D_00455210 != 0 ? cmdPacket[3] : 0;
    replyBuf = (char *)((int)(D_0052E380 + idx * 0x440) | 0x20000000);
    *(struct W7590 *)&sema = *(struct W7590 *)(replyBuf + 0x0);
    *(struct W7590 *)&cmd = *(struct W7590 *)(replyBuf + 0x4);
    *(struct W7590 *)&dst = *(struct W7590 *)(replyBuf + 0x8);
    *(struct W7590 *)&size = *(struct W7590 *)(replyBuf + 0xC);
    if (sema >= 0)
        func_0041605C(dst, replyBuf + 0x10, size);
    switch (cmd) {
    case 2: {
        struct L7590 *readFixup = (struct L7590 *)(replyBuf + 0x14);
        char *fixupDst;
        char *fixupUncached;
        if (readFixup->n1 > 0) {
            fixupDst = readFixup->d1;
            fixupUncached = (char *)((int)fixupDst | 0x20000000);
            for (i = 0; i < readFixup->n1; i++) {
                unsigned char fixupByte = readFixup->b1[i];
                fixupUncached[i] = fixupByte;
                fixupDst[i] = fixupByte;
            }
        }
        if (readFixup->n2 > 0) {
            fixupDst = readFixup->d2;
            fixupUncached = (char *)((int)fixupDst | 0x20000000);
            for (i = 0; i < readFixup->n2; i++) {
                unsigned char fixupByte = readFixup->b2[i];
                fixupUncached[i] = fixupByte;
                fixupDst[i] = fixupByte;
            }
        }
        break;
    }
    case 11:
        *(struct W7590 *)&buf = *(struct W7590 *)(replyBuf + 0x14);
        *(struct B7590 *)buf = *(struct B7590 *)(replyBuf + 0x18);
        break;
    case 0xC:
        *(struct W7590 *)&buf = *(struct W7590 *)(replyBuf + 0x14);
        *(struct C7590 *)buf = *(struct C7590 *)(replyBuf + 0x18);
        break;
    case 23:
    case 25:
    case 26:
        *(struct W7590 *)&buf = *(struct W7590 *)(replyBuf + 0x14);
        *(struct W7590 *)&len = *(struct W7590 *)(replyBuf + 0x18);
        if (len > 0x400)
            len = 0x400;
        func_0041605C(buf, replyBuf + 0x1C, len);
        break;
    }
    if (sema < 0) {
        sema = -sema;
        for (i = 0; i < 0x20; i++) {
            if (D_00455188[i] == sema) {
                D_00455188[i] = -1;
                break;
            }
        }
    } else {
        iSignalSema(sema);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427938);
#ifdef SKIP_ASM
extern int CreateSema(int *p);
/**
 * Creates the file library's call lock D_00455214 (a semaphore, initial and max count 1) the first time it is called.
 */
void func_00427938(void) {
    int buf[8];
    if (D_00455214 == -1) {
        buf[5] = 0;
        buf[2] = 1;
        buf[1] = 1;
        D_00455214 = CreateSema(buf);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427988);
#ifdef SKIP_ASM
extern void func_00427938(void);
extern void WaitSema(int a0);
/**
 * Takes the file library's call lock (WaitSema on D_00455214), creating it first if needed (func_00427938).
 *
 * Every file call wraps its RPC in this and func_004279B8; callers pass their RPC number, which this definition does
 * not use.
 *
 * @return 0
 */
int func_00427988(void) {
    func_00427938();
    WaitSema(D_00455214);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004279B8);
#ifdef SKIP_ASM
extern void SignalSema(int);
/**
 * Releases the file library's call lock (SignalSema on D_00455214) taken by func_00427988.
 */
void func_004279B8(void) {
    SignalSema(D_00455214);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004279C8);
#ifdef SKIP_ASM
extern void func_00427988_a(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern int DIntr_i(void) __asm__("DIntr");
extern void EIntr(void);
extern void func_004279B8(void);
/**
 * Installs the callback run when the IOP file server sends SIF command 0x80000013, returning the previous one.
 *
 * Under the call lock (func_00427988 ... func_004279B8), initialising the library first when D_0045520C is 0
 * (func_00427A90), it stores @p callbackFn and @p callbackArg in D_0052EE80 with interrupts disabled (re-enabled only when they were
 * on). func_00427A60 runs the callback. No caller in this build.
 *
 * @param callbackFn  callback, called with @p callbackArg
 * @param callbackArg  argument for the callback
 * @return the previous callback
 */
void (*func_004279C8(void (*callbackFn)(int), int callbackArg))(int) {
    int intrWasOn;
    void (*old)(int);
    sFioCb *callbackSlot = &D_0052EE80;
    func_00427988_a(27);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    intrWasOn = DIntr_i();
    old = D_0052EE80.fn;
    callbackSlot->arg = callbackArg;
    D_0052EE80.fn = callbackFn;
    if (intrWasOn != 0) {
        EIntr();
    }
    func_004279B8();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427A60);
#ifdef SKIP_ASM
/**
 * Runs the file library's user callback (SIF command 0x80000013 handler installed by func_00427A90).
 *
 * Calls @p callback->fn(@p callback->arg) when set, then executes sync and ei: interrupts are re-enabled on return.
 *
 * @param cause  the received command packet (unused)
 * @param callback  D_0052EE80, set by func_004279C8
 */
void func_00427A60(int cause, sFioCb *callback) {
    if (callback->fn)
        callback->fn(callback->arg);
    SYNC_4248();
    EI_4248();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427A90);
#ifdef SKIP_ASM
extern void func_00426408_z(int) __asm__("func_00426408");
extern int DIntr_z(void) __asm__("DIntr");
extern void EIntr_z(void) __asm__("EIntr");
extern void sceSifAddCmdHandler_z(int, void *, void *) __asm__("sceSifAddCmdHandler");
extern void func_00427438_z(void) __asm__("func_00427438");
extern void func_00427590_z(void) __asm__("func_00427590");
extern void func_00427A60_z(void) __asm__("func_00427A60");
extern int WaitSema_z(int) __asm__("WaitSema");
extern int SignalSema_z(int) __asm__("SignalSema");
int func_00427A90_d(void) __asm__("func_00427A90");
/**
 * Initialises the EE side of the IOP file library: binds the file server and reads its version.
 *
 * Calls func_00426408(0), clears the user callback D_0052EE80 and, with interrupts disabled, installs func_00427590
 * (SIF command 0x80000011, reply buffer D_0052EE40) and func_00427A60 (0x80000013, the callback) with sceSifAddCmdHandler.
 * Then binds D_0052EE00 to server 0x80000001 (sceSifBindRpc), retrying after a 0x100000-iteration spin until the server
 * is bound. It frees all 32 descriptor slots of D_0052EC00 under D_00455218, sends the addresses of the two 0x440-byte
 * reply buffers D_0052E380 (in D_0052D6C0) with RPC function 0xFF and copies the 4-byte server version to D_0052EE28;
 * D_00455210 becomes 1 when the second reply word is 2 (the server then names a reply buffer per request). Sets the
 * initialised flag D_0045520C. The file calls in this unit call it when the flag is 0.
 *
 * @return 0; -1 when the bind fails; 0xFFFEFFFF when the version call fails
 */
int func_00427A90_d(void) {
    int old;
    int spinCount;
    sFioFile *fileSlot;
    sFioCb *callbackSlot = &D_0052EE80;
    sRpcVer w;
    func_00426408_z(0);
    D_0052EE80.fn = 0;
    callbackSlot->arg = 0;
    old = DIntr_z();
    sceSifAddCmdHandler_z(0x80000011, func_00427590_z, D_0052EE40);
    sceSifAddCmdHandler_z(0x80000013, func_00427A60_z, callbackSlot);
    if (old != 0)
        EIntr_z();
    for (;;) {
        if (sceSifBindRpc(&D_0052EE00, 0x80000001, 0) < 0)
            return -1;
        if (D_0052EE00.server != 0)
            break;
        spinCount = 0x100000;
        do {
            spinCount--;
        } while (spinCount != -1);
    }
    func_00427438_z();
    WaitSema_z(D_00455218);
    for (fileSlot = D_0052EC00; fileSlot < &D_0052EC00[32]; fileSlot++)
        fileSlot->f4 = 0;
    SignalSema_z(D_00455218);
    D_0052D6C0[0] = (int)D_0052E380;
    D_0052D6C0[1] = D_0052D6C0[0] + 1088;
    if (sceSifCallRpc(&D_0052EE00, 0xFF, 0, D_0052D6C0, 8, D_0052E340, 8, 0, 0) < 0)
        return 0xFFFEFFFF;
    D_0052EE28 = *(sRpcVer *)((int)D_0052E340 | 0x20000000);
    w = *(sRpcVer *)((int)(D_0052E340 + 4) | 0x20000000);
    D_00455210 = *(int *)&w == 2;
    D_0045520C = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427C98);
#ifdef SKIP_ASM
extern char D_00455164[];
extern char *D_00455220;
extern int memcmp(char *, char *, int);
/**
 * Returns whether the bound file server's version tag is one the library does not accept.
 *
 * Compares the 4-byte tag D_0052EE28 (set by func_00427A90) with the library's "2710" (D_00455164) and with the tag
 * D_00455220 points at ("....", D_0049AD48) using memcmp (memcmp). Returns 0 when it equals either; otherwise
 * the result of comparing the two accepted tags, which always differ, so 1. sceOpen refuses to open files then.
 *
 * @return 1 when the server's tag matches neither accepted tag, else 0
 */
int func_00427C98(void) {
    char *libVersion = D_00455164;
    char *serverVersion = D_0052EE28.b;
    int mismatch = 0;
    if (memcmp(serverVersion, libVersion, 4) != 0 &&
        memcmp(serverVersion, D_00455220, 4) != 0) {
        mismatch = memcmp(libVersion, D_00455220, 4) != 0;
    }
    return mismatch;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceFsReset);
#ifdef SKIP_ASM
extern void memset(void *a0, int a1, int a2);
/**
 * Marks the file library as uninitialised so the next file call rebinds the server.
 *
 * Clears D_0045520C and zeroes the 4-byte server version D_0052EE28 (memset, memset). func_00319610 calls it
 * after rebooting the IOP.
 *
 * @return 0
 */
int sceFsReset(void) {
    D_0045520C = 0;
    memset(&D_0052EE28, 0, 4);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceOpen);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int func_00427C98_z(void) __asm__("func_00427C98");
extern int CreateSema(int *p);
extern int db0i(int) __asm__("DeleteSema");
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceOpen_d(unsigned char *, unsigned, ...) __asm__("sceOpen");
/**
 * Opens a file on the IOP through the file server and returns a descriptor number.
 *
 * Under the call lock (func_00427988 ... func_004279B8), initialising the library when needed (func_00427A90), it fails
 * with 0xFFFEFFFC when the server's version is not accepted (func_00427C98) and with -19 when no descriptor slot is
 * free (func_00427498). The RPC function 0 request in D_0052D700.open holds a new semaphore, the address of a stack
 * result word and its size 4, @p flags without bit 31, the mode (the first variadic argument), the name (up to 0x3FF
 * characters, truncated) and the slot index; it is sent with sceSifCallRpc (0x418 bytes) and the server's first reply
 * word (D_0052E340, read uncached) says whether it was accepted. The lock is released before waiting on the semaphore
 * for the result (func_00427590 posts it). A negative result frees the slot and is returned; otherwise the IOP's
 * descriptor goes to the slot's f0 and @p flags are or-ed into its f4. Likely bug: when the RPC fails or is refused
 * (-11) the slot taken by func_00427498 is never freed. Called by FILEDEV_open.
 *
 * @param filePath  path of the file
 * @param flags  open flags (bit 15 makes later calls on the descriptor no-wait)
 * @return the descriptor number (slot index); a negative error
 */
// sceOpen
int sceOpen_d(unsigned char *filePath, unsigned flags, ...) {
    int buf[12];
    int sem;
    int result;
    int n;
    int idx;
    char nameChar;
    int mode;
    char *ap;
    sFioFile *fileSlot;
    sFioOpenCmd *openCmd = &D_0052D700.open;
    func_00427988_b(0);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    if (func_00427C98_z() != 0) {
        func_004279B8();
        return 0xFFFEFFFC;
    }
    fileSlot = func_00427498();
    if (fileSlot == 0) {
        func_004279B8();
        return -19;
    }
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    mode = *(int *)ap;
    nameChar = (openCmd->name[0] = filePath[0]);
    n = 0;
    if (nameChar != 0) {
        while (++n < 0x400) {
            nameChar = (openCmd->name[n] = filePath[n]);
            if (nameChar == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        openCmd->name[0x3FF] = 0;
    }
    idx = fileSlot - D_0052EC00;
    openCmd->fC = flags & 0x7FFFFFFF;
    openCmd->f10 = mode;
    buf[1] = 1;
    openCmd->f414 = idx;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    openCmd->f0 = sem;
    openCmd->f4 = (int)&buf[8];
    openCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 0, 0, &D_0052D700.open, 1048, D_0052E340, 4, 0, 0) < 0) {
        db0i(sem);
        func_004279B8();
        return -11;
    }
    result = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (result == 0) {
        db0i(sem);
        return -11;
    }
    WaitSema_i(sem);
    db0i(sem);
    if (buf[8] < 0) {
        WaitSema_i(D_00455218);
        fileSlot->f4 = 0;
        SignalSema(D_00455218);
        return buf[8];
    }
    result = idx;
    WaitSema_i(D_00455218);
    fileSlot->f0 = buf[8];
    fileSlot->f4 |= flags;
    SignalSema(D_00455218);
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceClose);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Closes a file descriptor through the IOP file server.
 *
 * Takes the call lock (func_00427988; released by func_004279B8), but unlike sceOpen does not initialise the
 * library: returns -1 when D_0045520C is 0 and -9 when the descriptor's slot (func_00427520) is not open. Sends RPC
 * function 1 with a new semaphore, the address and size (4) of a stack result word, the IOP descriptor and the slot
 * index (20 bytes); after a successful call it frees the slot (f4 = 0, without taking D_00455218). It waits on the
 * semaphore for the result even when the descriptor was opened no-wait. Called by FILEDEV_close.
 *
 * @param fileDesc  descriptor number from sceOpen
 * @return 0, the IOP's negative error, or -11 when the RPC fails or is refused
 */
// sceClose
int sceClose(int fileDesc) {
    int buf[12];
    int sem;
    int accepted;
    sFioFile *fileSlot;
    sFioFdCmd *closeCmd = &D_0052D700.close;
    fileSlot = func_00427520(fileDesc);
    func_00427988_b(1);
    if (D_0045520C == 0) {
        func_004279B8();
        return -1;
    }
    if (fileSlot == 0 || fileSlot->f4 == 0) {
        func_004279B8();
        return -9;
    }
    closeCmd->fC = fileSlot->f0;
    buf[1] = 1;
    closeCmd->f10 = fileSlot - D_0052EC00;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    closeCmd->f0 = sem;
    closeCmd->f4 = (int)&buf[8];
    closeCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 1, 0, closeCmd, 20, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    fileSlot->f4 = 0;
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    {
        int ioResult = buf[8];
        if (ioResult < 0)
            return ioResult;
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceLseek);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Moves a file descriptor's position through the IOP file server.
 *
 * Takes the call lock (func_00427988; released by func_004279B8); returns -1 when the library is not initialised and -9
 * when the slot (func_00427520) is not open. Sends RPC function 4 with a new semaphore, the address and size (4) of a
 * stack result word, the IOP descriptor, @p offset, @p whence and the slot index (0x1C bytes). For a descriptor opened with
 * flag 0x8000 (no wait) the semaphore is parked, negated in the request, in a free entry of D_00455188 (under
 * D_0045521C) and the call returns 0 without waiting; func_00427590 clears that entry when the IOP is done. Called by
 * FILEDEV_getsize, FILEDEV_readexec and FILEDEV_write.
 *
 * @param fileDesc  descriptor number
 * @param offset  offset
 * @param whence  origin of @p offset (passed to the IOP)
 * @return the new position, 0 for a no-wait call, -11 when the RPC fails or is refused, or the IOP's error
 */
// sceLseek
int sceLseek(int fileDesc, int offset, int whence)
{
  int buf[12];
  int sem;
  int accepted;
  int i;
  short flags;
  sFioFile *fileSlot;
  unsigned fl;
  int *out;
  sFioLseekCmd *lseekCmd = &D_0052D700.lseek;
  fileSlot = func_00427520(fileDesc);
  func_00427988_b(4);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  lseekCmd->fC = fileSlot->f0;
  lseekCmd->f10 = offset;
  lseekCmd->f14 = whence;
  lseekCmd->f18 = fileSlot - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  lseekCmd->f8 = 4;
  lseekCmd->f4 = (int) out;
  flags = fl;
  D_0052D700.lseek.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = lseekCmd->f0;
        lseekCmd->f0 = -lseekCmd->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if (sceSifCallRpc(&D_0052EE00, 4, 0, &D_0052D700.lseek, 0x1C, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  accepted = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (accepted == 0)
  {
    DeleteSema(sem);
    return -11;
  }
  if (fl & 0x8000)
  {
    DeleteSema(sem);
    return 0;
  }
  WaitSema_i(sem);
  DeleteSema(sem);
  return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceRead);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Reads from a file descriptor into a buffer through the IOP file server.
 *
 * Takes the call lock (func_00427988; released by func_004279B8); returns -1 when the library is not initialised and -9
 * when the slot (func_00427520) is not open. Sends RPC function 2 with a new semaphore, the address and size (4) of a
 * stack result word, the IOP descriptor, @p readBuf, @p readSize and the slot index (0x20 bytes). Unless the descriptor has flag
 * 0x20000000 it first writes the buffer back from the data cache, and it always writes back the request block
 * (func_00426358). The IOP fills the buffer by DMA; func_00427590 patches its unaligned head and tail bytes. A
 * descriptor opened with flag 0x8000 parks the semaphore in D_00455188 and returns 0 at once, as in sceLseek.
 * Called by FILEDEV_readexec.
 *
 * @param fileDesc  descriptor number
 * @param readBuf  destination buffer
 * @param readSize  bytes to read
 * @return bytes read, 0 for a no-wait call, -11 when the RPC fails or is refused, or the IOP's error
 */
int sceRead(int fileDesc, int readBuf, int readSize)
{
  int buf[12];
  int sem;
  int accepted;
  int i;
  short flags;
  sFioFile *fileSlot;
  unsigned fl;
  int *out;
  sFioReadCmd *readCmd = &D_0052D700.read;
  fileSlot = func_00427520(fileDesc);
  func_00427988_b(2);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  readCmd->fC = fileSlot->f0;
  readCmd->f10 = readBuf;
  readCmd->f14 = readSize;
  readCmd->f1C = fileSlot - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  readCmd->f8 = 4;
  readCmd->f4 = (int) out;
  flags = fl;
  D_0052D700.read.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = readCmd->f0;
        readCmd->f0 = -readCmd->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if (!(fl & 0x20000000))
    func_00426358(readBuf, readSize);
  func_00426358((int) readCmd, 0x20);
  if (sceSifCallRpc(&D_0052EE00, 2, 0, &D_0052D700.read, 0x20, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  accepted = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (accepted == 0)
  {
    DeleteSema(sem);
    return -11;
  }
  if (fl & 0x8000)
  {
    DeleteSema(sem);
    return 0;
  }
  WaitSema_i(sem);
  DeleteSema(sem);
  return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceWrite);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Writes a buffer to a file descriptor through the IOP file server.
 *
 * Takes the call lock (func_00427988; released by func_004279B8); returns -1 when the library is not initialised and -9
 * when the slot (func_00427520) is not open. Sends RPC function 3 (0x30 bytes) with a new semaphore, the address and
 * size (4) of a stack result word, the IOP descriptor, @p writeBuf, @p writeSize, the slot index and the unaligned head of the
 * buffer: the bytes up to the next 16-byte boundary (at most @p writeSize), copied through the uncached view, with their
 * count. Unless the descriptor has flag 0x20000000 the buffer is written back from the data cache first. A descriptor
 * opened with flag 0x8000 parks the semaphore in D_00455188 and returns 0 at once, as in sceLseek. Called by
 * FILEDEV_write.
 *
 * @param fileDesc  descriptor number
 * @param writeBuf  source buffer
 * @param writeSize  bytes to write
 * @return bytes written, 0 for a no-wait call, -11 when the RPC fails or is refused, or the IOP's error
 */
int sceWrite(int fileDesc, unsigned writeBuf, int writeSize)
{
  int buf[12];
  int sem;
  int accepted;
  int i;
  int n;
  int j;
  short flags;
  sFioFile *fileSlot;
  unsigned fl;
  int *out;
  sFioWriteCmd *writeCmd = &D_0052D700.write;
  fileSlot = func_00427520(fileDesc);
  func_00427988_b(3);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  writeCmd->fC = fileSlot->f0;
  writeCmd->f14 = writeSize;
  writeCmd->f10 = writeBuf;
  writeCmd->f2C = fileSlot - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  writeCmd->f8 = 4;
  writeCmd->f4 = (int) out;
  flags = fl;
  D_0052D700.write.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = writeCmd->f0;
        writeCmd->f0 = -writeCmd->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if ((writeBuf & 0xF) == 0)
    n = 0;
  else
    {
      unsigned lo = writeBuf - 0x10;
      n = ((writeBuf >> 4) << 4) - lo;
    }
  if (writeSize < n)
    n = writeSize;
  if (!(fl & 0x20000000))
    func_00426358(writeBuf, writeSize);
  writeBuf |= 0x20000000;
  writeCmd->f18 = n;
  for (j = 0; j < n; j++)
    writeCmd->data[j] = ((char *) writeBuf)[j];
  if (sceSifCallRpc(&D_0052EE00, 3, 0, &D_0052D700.write, 0x30, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  accepted = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (accepted == 0)
  {
    DeleteSema(sem);
    return -11;
  }
  if (fl & 0x8000)
  {
    DeleteSema(sem);
    return 0;
  }
  WaitSema_i(sem);
  DeleteSema(sem);
  return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceIoctl);
#ifdef SKIP_ASM
/* sceIoctl (libkernel fileio): requests 1-3 are answered on the EE, the rest go to the IOP as RPC 5. */
extern int *D_0052D6C8;         /* argument of the pending sceIoctl */
extern char D_0052E390[];       /* D_0052E380 + 0x10: read-ahead buffer payload */
extern int D_00455208;
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
struct Blk88C0 { char b[0x400]; };
/**
 * Performs a control request on a file descriptor: requests 1-3 are answered on the EE, the rest go to the IOP.
 *
 * Takes the call lock (func_00427988; released by func_004279B8), stores @p ioctlArg in D_0052D6C8 and initialises the
 * library when needed (func_00427A90); -9 when the slot (func_00427520) is not open. Request 1 sets *@p ioctlArg to 1 when
 * any no-wait call is still pending in D_00455188 (under D_0045521C), else 0. Requests 2 and 3 copy the 32-bit or
 * 64-bit result word of the last completed reply buffer (D_0052E390 + D_00455208 * 0x440, uncached) to *@p ioctlArg. Any
 * other request is sent as RPC function 5 (1056 bytes, written back first) with a new semaphore, the IOP descriptor,
 * the request number and 0x400 bytes copied from @p ioctlArg (length 0 when @p ioctlArg is 0); it waits for the result even on a
 * no-wait descriptor. No caller in this build.
 *
 * @param fd  descriptor number
 * @param req  request number
 * @param ioctlArg  request argument or result pointer
 * @return 0 for requests 1-3; else the IOP's result, or -11 when the RPC fails or is refused
 */
// FAKEMATCH: stand-in local `one = 1` (retail keeps the constant 1 in $21 from the prologue for the switch's case-1 compare) plus a dead `one = 0;` at the end of case 1, so jump1 keeps the local until cse substitutes it into the compare; plain form 97.75%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int sceIoctl(int fd, int req, void *ioctlArg) {
    int buf[12];
    int sem;
    int accepted;
    int i;
    sFioFile *fileSlot;
    sFioIoctl2Cmd *ioctlCmd = &D_0052D700.ioctl2;
    int one = 1; /* FAKEMATCH: stand-in for the case-1 constant */
    fileSlot = func_00427520(fd);
    func_00427988_b(5);
    D_0052D6C8 = ioctlArg;
    if (D_0045520C == 0) {
        func_00427A90();
    }
    if (fileSlot == 0 || fileSlot->f4 == 0) {
        func_004279B8();
        return -9;
    }
    ioctlCmd->f414 = 0;
    ioctlCmd->f418 = 0;
    switch (req) {
    case 1:
        WaitSema_i(D_0045521C);
        for (i = 0; i < 32; i++) {
            if (D_00455188[i] != -1)
                break;
        }
        if (i == 32)
            *D_0052D6C8 = 0;
        else
            *D_0052D6C8 = 1;
        one = 0; /* FAKEMATCH: dead store, keeps `one` a variable */
        SignalSema(D_0045521C);
        func_004279B8();
        return 0;
    case 2:
        *(int *)ioctlArg = *(int *)((int)(D_0052E390 + D_00455208 * 0x440) | 0x20000000);
        func_004279B8();
        return 0;
    case 3:
        *(long *)ioctlArg = *(long *)((int)(D_0052E390 + D_00455208 * 0x440) | 0x20000000);
        func_004279B8();
        return 0;
    default:
        ioctlCmd->fC = fileSlot->f0;
        ioctlCmd->f10 = req;
        if (ioctlArg == 0) {
            ioctlCmd->f41C = 0;
        } else {
            ioctlCmd->f41C = 0x400;
            *(struct Blk88C0 *)ioctlCmd->data = *(struct Blk88C0 *)ioctlArg;
        }
        buf[1] = 1;
        buf[2] = 0;
        buf[5] = 0;
        sem = CreateSema(&buf[0]);
        ioctlCmd->f0 = sem;
        ioctlCmd->f4 = (int)&buf[8];
        ioctlCmd->f8 = 4;
        func_00426358((int)&D_0052D700.ioctl2, 1056);
        if (sceSifCallRpc(&D_0052EE00, 5, 0, &D_0052D700.ioctl2, 1056, D_0052E340, 4, 0, 0) < 0) {
            DeleteSema(sem);
            func_004279B8();
            return -11;
        }
        accepted = *(int *)((int)D_0052E340 | 0x20000000);
        func_004279B8();
        if (accepted == 0) {
            DeleteSema(sem);
            return -11;
        }
        WaitSema_i(sem);
        DeleteSema(sem);
        return buf[8];
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceIoctl2);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern void *func_0041605C(void *, void *, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceIoctl2_d(int, int, void *, unsigned, int, unsigned) __asm__("sceIoctl2");
/**
 * Performs an extended control request (input and output buffers) on a file descriptor through the IOP file server.
 *
 * Takes the call lock (func_00427988; released by func_004279B8) and initialises the library when needed
 * (func_00427A90); -9 when the slot (func_00427520) is not open, -22 when @p len or @p outLen exceeds 0x400. Copies @p len
 * bytes of @p data into the request (func_0041605C, memcpy) and sends RPC function 26 (1056 bytes, written back first)
 * with a new semaphore, the IOP descriptor, @p request, the lengths and @p outBuf; func_00427590 copies the reply data to
 * @p outBuf. The length word is cleared when @p data is 0 and then overwritten with @p len anyway. It always waits for the
 * result. No caller in this build.
 *
 * @param fileDesc  descriptor number
 * @param request  request number
 * @param data  input data (up to 0x400 bytes), or 0
 * @param len  bytes of @p data
 * @param outBuf  output buffer
 * @param outLen  size of @p outBuf (up to 0x400)
 * @return the IOP's result, -9, -22, or -11 when the RPC fails or is refused
 */
int sceIoctl2_d(int fileDesc, int request, void *data, unsigned len, int outBuf, unsigned outLen) {
    int buf[12];
    int sem;
    int accepted;
    sFioFile *fileSlot;
    sFioIoctl2Cmd *ioctl2Cmd = &D_0052D700.ioctl2;
    fileSlot = func_00427520(fileDesc);
    func_00427988_b(26);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    if (fileSlot == 0 || fileSlot->f4 == 0) {
        func_004279B8();
        return -9;
    }
    if (len > 0x400 || outLen > 0x400) {
        func_004279B8();
        return -22;
    }
    if (data == 0) {
        ioctl2Cmd->f41C = 0;
    } else {
        func_0041605C(ioctl2Cmd->data, data, len);
    }
    ioctl2Cmd->fC = fileSlot->f0;
    ioctl2Cmd->f10 = request;
    ioctl2Cmd->f41C = len;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    ioctl2Cmd->f418 = outLen;
    ioctl2Cmd->f4 = (int)&buf[8];
    ioctl2Cmd->f8 = 4;
    ioctl2Cmd->f414 = outBuf;
    ioctl2Cmd->f0 = sem;
    func_00426358((int)&D_0052D700.ioctl2, 1056);
    if (sceSifCallRpc(&D_0052EE00, 26, 0, &D_0052D700.ioctl2, 1056, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00428E18);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00428E18_d(char *, int) __asm__("func_00428E18");
/**
 * Sends a file-server request that takes only a path, and waits for its result.
 *
 * Takes the call lock (func_00427988 with @p rpcFunc; released by func_004279B8) and initialises the library when needed
 * (func_00427A90). Copies @p pathName (up to 0x3FF characters, truncated) into D_0052D700.path and sends RPC function @p rpcFunc
 * with a new semaphore and the address and size (4) of a stack result word; the request is the 12-byte header plus the
 * name and its NUL. Used for requests 6, 8, 9, 16, 18 and 21 (sceRemove, sceRmdir, sceDopen,
 * func_00429540, sceChdir, sceUmount).
 *
 * @param pathName  the path
 * @param rpcFunc  file-server RPC function number
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_00428E18_d(char *pathName, int rpcFunc) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    sFioPathCmd *pathCmd = &D_0052D700.path;
    func_00427988_b(rpcFunc);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((pathCmd->name[0] = pathName[0]) != 0) {
        while (++n < 0x400) {
            if ((pathCmd->name[n] = pathName[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        pathCmd->name[0x3FF] = 0;
        n = 0x3FF;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    pathCmd->f4 = (int)&buf[8];
    pathCmd->f0 = sem;
    pathCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, rpcFunc, 0, &D_0052D700.path, n + 13, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceRemove);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Removes a file: sends file-server request 6 with @p path through func_00428E18.
 *
 * @param path  the file's path
 * @return the IOP's result (0 or a negative error), or -11 when the RPC fails
 */
int sceRemove(int path) {
    return func_00428E18(path, 6);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceMkdir);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceMkdir_d(char *, int) __asm__("sceMkdir");
/**
 * Creates a directory through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p dirPath (up to 0x3FF characters) and
 * @p dirMode into D_0052D700.mkdir and sends RPC function 7 (the 16-byte header plus the name and its NUL) with a new
 * semaphore and a stack result word, then waits for the result.
 *
 * @param dirPath  the directory's path
 * @param dirMode  mode passed to the IOP
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int sceMkdir_d(char *dirPath, int dirMode) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    sFioPathModeCmd *mkdirCmd = &D_0052D700.mkdir;
    func_00427988_b(7);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((mkdirCmd->name[0] = dirPath[0]) != 0) {
        while (++n < 0x400) {
            if ((mkdirCmd->name[n] = dirPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        mkdirCmd->name[0x3FF] = 0;
        n = 0x3FF;
    }
    mkdirCmd->fC = dirMode;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    mkdirCmd->f4 = (int)&buf[8];
    mkdirCmd->f0 = sem;
    mkdirCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 7, 0, &D_0052D700.mkdir, n + 17, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceRmdir);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Removes a directory: sends file-server request 8 with @p path through func_00428E18.
 *
 * @param path  the directory's path
 * @return the IOP's result, or -11 when the RPC fails
 */
int sceRmdir(int path) {
    return func_00428E18(path, 8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceFormat);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceFormat_d(char *, char *, char *, int) __asm__("sceFormat");
/**
 * Formats a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p devName and @p blockDev (each up to 0x3FF
 * characters; an empty second name when @p blockDev is 0) and @p len bytes of @p data into D_0052D700.format; -7 when @p len
 * exceeds 0x400 (checked after the names are copied). Sends RPC function 14 (3088 bytes, written back first) with a new
 * semaphore and a stack result word and waits for the result.
 *
 * @param devName  the device to format
 * @param blockDev  the block device, or 0
 * @param data  argument data
 * @param len  bytes of @p data (up to 0x400)
 * @return the IOP's result, -7, or -11 when the RPC fails or is refused
 */
int sceFormat_d(char *devName, char *blockDev, char *data, int len) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    int i;
    sFioFormatCmd *formatCmd = &D_0052D700.format;
    func_00427988_b(0xE);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((formatCmd->name[0] = devName[0]) != 0) {
        while (++n < 0x400) {
            if ((formatCmd->name[n] = devName[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        formatCmd->name[0x3FF] = 0;
    }
    if (blockDev == 0) {
        formatCmd->name2[0] = 0;
    } else {
        n = 0;
        if ((formatCmd->name2[0] = blockDev[0]) != 0) {
            while (++n < 0x400) {
                if ((formatCmd->name2[n] = blockDev[n]) == 0) {
                    break;
                }
            }
        }
        if (n == 0x400) {
            formatCmd->name2[0x3FF] = 0;
        }
    }
    if (len > 0x400) {
        func_004279B8();
        return -7;
    }
    for (i = 0; i < len; i++) {
        formatCmd->data[i] = data[i];
    }
    formatCmd->fC0C = len;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    formatCmd->f4 = (int)&buf[8];
    formatCmd->f0 = sem;
    formatCmd->f8 = 4;
    func_00426358((int)&D_0052D700.format, 3088);
    if (sceSifCallRpc(&D_0052EE00, 0xE, 0, &D_0052D700.format, 3088, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429420);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Sends file-server request 15 with one word argument and waits for the result.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90); the 16-byte request holds a new
 * semaphore, a stack result word's address and size, and @p driverArg. Unlike the other file calls it returns -1 (not -11)
 * when the RPC fails or is refused. No caller in this build.
 *
 * @param driverArg  the word passed to the IOP
 * @return the IOP's result, or -1
 */
int func_00429420(int driverArg) {
    int buf[12];
    int sem;
    int accepted;
    sFioCmd4 *adddrvCmd = &D_0052D700.adddrv;
    func_00427988_b(0xF);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    adddrvCmd->fC = driverArg;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    D_0052D700.adddrv.f0 = sem;
    adddrvCmd->f4 = (int)&buf[8];
    adddrvCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 0xF, 0, &D_0052D700.adddrv, 0x10, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -1;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -1;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429540);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Sends file-server request 16 with @p path through func_00428E18.
 *
 * @param path  the name passed to the IOP
 * @return the IOP's result, or -11 when the RPC fails
 */
int func_00429540(int path) {
    return func_00428E18(path, 0x10);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDopen);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern void func_00427A90(void);
extern int func_00428E18(int a0, int a1);
extern int WaitSema_i(int) __asm__("WaitSema");
extern void SignalSema(int);
/**
 * Opens a directory through the IOP file server and returns a descriptor number.
 *
 * Takes and releases the call lock, initialising the library when needed (func_00427A90), then allocates a descriptor
 * slot (func_00427498; -19 when none is free) and sends request 9 with the path through func_00428E18. A negative
 * result frees the slot and is returned; otherwise the IOP's descriptor goes to the slot's f0 (under D_00455218). The
 * slot keeps only its in-use flag 0x10000000.
 *
 * @param dirPath  the directory's path
 * @return the descriptor number (slot index), or a negative error
 */
int sceDopen(int dirPath) {
    int result;
    sFioFile *dirSlot;
    func_00427988_b(9);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    func_004279B8();
    dirSlot = func_00427498();
    if (dirSlot == 0) {
        return -19;
    }
    result = func_00428E18(dirPath, 9);
    if (result < 0) {
        WaitSema_i(D_00455218);
        dirSlot->f4 = 0;
        SignalSema(D_00455218);
        return result;
    } else {
        WaitSema_i(D_00455218);
        dirSlot->f0 = result;
        result = dirSlot - D_0052EC00;
        SignalSema(D_00455218);
        return result;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDclose);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Closes a directory descriptor through the IOP file server.
 *
 * Like sceClose: takes the call lock, returns -1 when the library is not initialised and -9 when the slot
 * (func_00427520) is not open, sends RPC function 10 (20 bytes) with a new semaphore, a stack result word and the IOP
 * descriptor, frees the slot (f4 = 0) once the call is accepted and waits for the result.
 *
 * @param dirDesc  descriptor number from sceDopen
 * @return 0, the IOP's negative error, or -11 when the RPC fails or is refused
 */
int sceDclose(int dirDesc) {
    int buf[12];
    int sem;
    int accepted;
    sFioFile *dirSlot;
    sFioFdCmd *dcloseCmd = &D_0052D700.dclose;
    dirSlot = func_00427520(dirDesc);
    func_00427988_b(10);
    if (D_0045520C == 0) {
        func_004279B8();
        return -1;
    }
    if (dirSlot == 0 || dirSlot->f4 == 0) {
        func_004279B8();
        return -9;
    }
    dcloseCmd->fC = dirSlot->f0;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    dcloseCmd->f0 = sem;
    dcloseCmd->f4 = (int)&buf[8];
    dcloseCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 10, 0, dcloseCmd, 20, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    dirSlot->f4 = 0;
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    {
        int ioResult = buf[8];
        if (ioResult < 0)
            return ioResult;
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDread);
#ifdef SKIP_ASM
extern int func_00427988_b(int);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceDread_d(int, int) __asm__("sceDread");
/**
 * Reads the next directory entry of a directory descriptor through the IOP file server.
 *
 * Takes the call lock; returns -1 when the library is not initialised and -9 when the slot (func_00427520) is not open.
 * Sends RPC function 11 (0x20 bytes) with a new semaphore, a stack result word, the IOP descriptor and @p direntBuf;
 * func_00427590 copies the 0x144-byte entry to @p direntBuf. Likely bug: when sceSifCallRpc fails it calls WaitSema on the new
 * semaphore instead of DeleteSema, which blocks (nothing signals it) and leaks the semaphore.
 *
 * @param dirDesc  descriptor number from sceDopen
 * @param direntBuf  where the 0x144-byte directory entry is stored
 * @return the IOP's result (positive while entries remain), or -11 when the call is refused
 */
int sceDread_d(int dirDesc, int direntBuf) {
    int buf[12];
    int sem;
    int accepted;
    sFioFile *dirSlot;
    sFioFdCmd *dreadCmd = &D_0052D700.dread;
    dirSlot = func_00427520(dirDesc);
    func_00427988_b(11);
    if (D_0045520C == 0) {
        func_004279B8();
        return -1;
    }
    if (dirSlot == 0 || dirSlot->f4 == 0) {
        func_004279B8();
        return -9;
    }
    dreadCmd->fC = dirSlot->f0;
    dreadCmd->f10 = direntBuf;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    dreadCmd->f0 = sem;
    dreadCmd->f4 = (int)&buf[8];
    dreadCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 11, 0, dreadCmd, 0x20, D_0052E340, 4, 0, 0) < 0) {
        WaitSema_i(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceGetstat);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Reads a file's status block through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p filePath (up to 0x3FF characters)
 * and @p statBuf into D_0052D700.getstat and sends RPC function 12 (the 16-byte header plus the name and its NUL) with a new
 * semaphore and a stack result word; func_00427590 copies the 0x40-byte status block to @p statBuf.
 *
 * @param filePath  the file's path
 * @param statBuf  where the 0x40-byte status block is stored
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int sceGetstat(char *filePath, int statBuf) {
    int buf[12];
    int sem;
    int accepted;
    int i;
    unsigned char nameChar;
    sFioPathModeCmd *getstatCmd = &D_0052D700.getstat;
    func_00427988_b(0xC);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    nameChar = (getstatCmd->name[0] = filePath[0]);
    i = 0;
    if (nameChar != 0) {
        while (++i < 0x400) {
            nameChar = (getstatCmd->name[i] = filePath[i]);
            if (nameChar == 0) {
                break;
            }
        }
    }
    if (i == 0x400) {
        getstatCmd->name[0x3FF] = 0;
        i = 0x3FF;
    }
    getstatCmd->fC = statBuf;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    getstatCmd->f4 = (int)&buf[8];
    getstatCmd->f0 = sem;
    getstatCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 0xC, 0, &D_0052D700.getstat, i + 17, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceChstat);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceChstat_d(char *, sFioStat *, int) __asm__("sceChstat");
/**
 * Changes a file's status through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p filePath (up to 0x3FF characters),
 * the 0x40-byte status block @p src and @p changeMask into D_0052D700.chstat, writes the block back from the data cache (1104
 * bytes) and sends RPC function 13 (the 0x50-byte header plus the name and its NUL) with a new semaphore and a stack
 * result word.
 *
 * @param filePath  the file's path
 * @param src  the new status values
 * @param changeMask  mask of the fields to change
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int sceChstat_d(char *filePath, sFioStat *src, int changeMask) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    unsigned char nameChar;
    sFioChstatCmd *chstatCmd = &D_0052D700.chstat;
    func_00427988_b(13);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    nameChar = (chstatCmd->name[0] = filePath[0]);
    n = 0;
    if (nameChar != 0) {
        while (++n < 0x400) {
            nameChar = (chstatCmd->name[n] = filePath[n]);
            if (nameChar == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        chstatCmd->name[0x3FF] = 0;
        n = 0x3FF;
    }
    chstatCmd->v = *src;
    chstatCmd->fC = changeMask;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    chstatCmd->f0 = sem;
    chstatCmd->f4 = (int)&buf[8];
    chstatCmd->f8 = 4;
    func_00426358((int)&D_0052D700.chstat, 1104);
    if (sceSifCallRpc(&D_0052EE00, 13, 0, &D_0052D700.chstat, n + 81, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceRename);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceRename_d(char *, char *) __asm__("sceRename");
/**
 * Renames a file through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies both names (each up to 0x3FF
 * characters) into D_0052D700.rename, writes the 2060-byte request back and sends RPC function 17 with a new semaphore
 * and a stack result word.
 *
 * @param oldPath  the current path
 * @param newPath  the new path
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int sceRename_d(char *oldPath, char *newPath) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    sFioPath2Cmd *renameCmd = &D_0052D700.rename;
    func_00427988_b(17);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((renameCmd->name[0] = oldPath[0]) != 0) {
        while (++n < 0x400) {
            if ((renameCmd->name[n] = oldPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        renameCmd->name[0x3FF] = 0;
    }
    n = 0;
    if ((renameCmd->name2[0] = newPath[0]) != 0) {
        while (++n < 0x400) {
            if ((renameCmd->name2[n] = newPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        renameCmd->name2[0x3FF] = 0;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    renameCmd->f4 = (int)&buf[8];
    renameCmd->f0 = sem;
    renameCmd->f8 = 4;
    func_00426358((int)&D_0052D700.rename, 2060);
    if (sceSifCallRpc(&D_0052EE00, 17, 0, &D_0052D700.rename, 2060, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceChdir);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Changes the IOP file server's current directory: sends request 18 with @p path through func_00428E18.
 *
 * @param path  the new current directory
 * @return the IOP's result, or -11 when the RPC fails
 */
int sceChdir(int path) {
    return func_00428E18(path, 18);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSync);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceSync_d(char *, int) __asm__("sceSync");
/**
 * Flushes a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p devPath (up to 0x3FF characters) and
 * @p syncFlag into D_0052D700.sync and sends RPC function 19 (1044 bytes) with a new semaphore and a stack result word.
 *
 * @param devPath  the device or path
 * @param syncFlag  flag passed to the IOP
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int sceSync_d(char *devPath, int syncFlag) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    sFioPathArgCmd *syncCmd = &D_0052D700.sync;
    func_00427988_b(19);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((syncCmd->name[0] = devPath[0]) != 0) {
        while (++n < 0x400) {
            if ((syncCmd->name[n] = devPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        syncCmd->name[0x3FF] = 0;
        n = 0x3FF;
    }
    syncCmd->f10 = syncFlag;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    syncCmd->f4 = (int)&buf[8];
    syncCmd->f0 = sem;
    syncCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 19, 0, &D_0052D700.sync, 1044, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceMount);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceMount_d(char *, char *, int, char *, int) __asm__("sceMount");
/**
 * Mounts a file system through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies both names (each up to 0x3FF
 * characters), @p mountFlag and @p len bytes of @p data into D_0052D700.mount; -7 when @p len exceeds 0x400. Writes the
 * 3092-byte request back and sends RPC function 20 with a new semaphore and a stack result word.
 *
 * @param fsDevice  the file-system device to mount as
 * @param blockDev  the block device
 * @param mountFlag  mount flag
 * @param data  argument data
 * @param len  bytes of @p data (up to 0x400)
 * @return the IOP's result, -7, or -11 when the RPC fails or is refused
 */
int sceMount_d(char *fsDevice, char *blockDev, int mountFlag, char *data, int len) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    int i;
    sFioMountCmd *mountCmd = &D_0052D700.mount;
    func_00427988_b(20);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((mountCmd->name[0] = fsDevice[0]) != 0) {
        while (++n < 0x400) {
            if ((mountCmd->name[n] = fsDevice[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        mountCmd->name[0x3FF] = 0;
    }
    n = 0;
    if ((mountCmd->name2[0] = blockDev[0]) != 0) {
        while (++n < 0x400) {
            if ((mountCmd->name2[n] = blockDev[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        mountCmd->name2[0x3FF] = 0;
    }
    if (len > 0x400) {
        func_004279B8();
        return -7;
    }
    for (i = 0; i < len; i++) {
        mountCmd->data[i] = data[i];
    }
    mountCmd->fC10 = len;
    mountCmd->fC0C = mountFlag;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    mountCmd->f4 = (int)&buf[8];
    mountCmd->f0 = sem;
    mountCmd->f8 = 4;
    func_00426358((int)&D_0052D700.mount, 3092);
    if (sceSifCallRpc(&D_0052EE00, 20, 0, &D_0052D700.mount, 3092, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceUmount);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Unmounts a file system: sends file-server request 21 with @p path through func_00428E18.
 *
 * @param path  the mounted file-system device
 * @return the IOP's result, or -11 when the RPC fails
 */
int sceUmount(int path) {
    return func_00428E18(path, 21);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceLseek64);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Moves a file descriptor's position with a 64-bit offset through the IOP file server.
 *
 * Like sceLseek but sends RPC function 22 (0x20 bytes) with the 64-bit @p offset, the origin, the slot index and an
 * 8-byte stack result. A descriptor opened with flag 0x8000 parks the semaphore in D_00455188 and returns 0 at once.
 *
 * @param fileDesc  descriptor number
 * @param offset  64-bit offset
 * @param whence  origin of @p offset (passed to the IOP)
 * @return the new 64-bit position, 0 for a no-wait call, -1 or -9 as in sceLseek, -11 when the RPC fails
 */
long sceLseek64(int fileDesc, long offset, int whence)
{
  int buf[8];
  long res;
  int sem;
  int accepted;
  int i;
  short flags;
  sFioFile *fileSlot;
  unsigned fl;
  sFioLseek64Cmd *lseek64Cmd = &D_0052D700.lseek64;
  fileSlot = func_00427520(fileDesc);
  func_00427988_b(22);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  lseek64Cmd->fC = fileSlot->f0;
  lseek64Cmd->f10 = offset;
  lseek64Cmd->f18 = whence;
  lseek64Cmd->f1C = fileSlot - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  lseek64Cmd->f4 = (int) &res;
  lseek64Cmd->f8 = 8;
  flags = fl;
  D_0052D700.lseek64.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = lseek64Cmd->f0;
        lseek64Cmd->f0 = -lseek64Cmd->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if (sceSifCallRpc(&D_0052EE00, 22, 0, &D_0052D700.lseek64, 0x20, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  accepted = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (accepted == 0)
  {
    DeleteSema(sem);
    return -11;
  }
  if (fl & 0x8000)
  {
    DeleteSema(sem);
    return 0;
  }
  WaitSema_i(sem);
  DeleteSema(sem);
  return res;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDevctl);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceDevctl_d(char *, int, char *, unsigned, int, unsigned) __asm__("sceDevctl");
/**
 * Sends a device control request (input and output buffers) to a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies the device name (up to 0x3FF
 * characters); -22 when @p len or @p outLen exceeds 0x400. Copies @p len bytes of @p data, the request number, the lengths
 * and @p outBuf into D_0052D700.devctl, writes the 2076-byte request back and sends RPC function 23 with a new semaphore
 * and a stack result word; func_00427590 copies the reply data to @p outBuf. func_003198A0 (the power-off thread) sends
 * request 0x4402 to "dev9x" through it.
 *
 * @param devName  the device name
 * @param devCmd  request number
 * @param data  input data, or 0
 * @param len  bytes of @p data (up to 0x400)
 * @param outBuf  output buffer
 * @param outLen  size of @p outBuf (up to 0x400)
 * @return the IOP's result, -22, or -11 when the RPC fails or is refused
 */
int sceDevctl_d(char *devName, int devCmd, char *data, unsigned len, int outBuf, unsigned outLen) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    unsigned i;
    sFioDevctlCmd *devctlCmd = &D_0052D700.devctl;
    func_00427988_b(23);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((devctlCmd->name[0] = devName[0]) != 0) {
        while (++n < 0x400) {
            if ((devctlCmd->name[n] = devName[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        devctlCmd->name[0x3FF] = 0;
    }
    if (len > 0x400 || outLen > 0x400) {
        func_004279B8();
        return -22;
    }
    for (i = 0; i < len; i++) {
        devctlCmd->data[i] = data[i];
    }
    devctlCmd->f810 = len;
    devctlCmd->f80C = devCmd;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    devctlCmd->f818 = outLen;
    devctlCmd->f4 = (int)&buf[8];
    devctlCmd->f8 = 4;
    devctlCmd->f814 = outBuf;
    devctlCmd->f0 = sem;
    func_00426358((int)&D_0052D700.devctl, 2076);
    if (sceSifCallRpc(&D_0052EE00, 23, 0, &D_0052D700.devctl, 2076, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSymlink);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceSymlink_d(char *, char *) __asm__("sceSymlink");
/**
 * Creates a symbolic link through the IOP file server.
 *
 * Takes the call lock (passing 17, which func_00427988 ignores) and initialises the library when needed
 * (func_00427A90). Copies both names (each up to 0x3FF characters) into D_0052D700.symlink and sends RPC function 24
 * (2060 bytes) with a new semaphore and a stack result word.
 *
 * @param oldPath  the existing path
 * @param linkPath  the link to create
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int sceSymlink_d(char *oldPath, char *linkPath) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    sFioPath2Cmd *symlinkCmd = &D_0052D700.symlink;
    func_00427988_b(17);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((symlinkCmd->name[0] = oldPath[0]) != 0) {
        while (++n < 0x400) {
            if ((symlinkCmd->name[n] = oldPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        symlinkCmd->name[0x3FF] = 0;
    }
    n = 0;
    if ((symlinkCmd->name2[0] = linkPath[0]) != 0) {
        while (++n < 0x400) {
            if ((symlinkCmd->name2[n] = linkPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        symlinkCmd->name2[0x3FF] = 0;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    symlinkCmd->f4 = (int)&buf[8];
    symlinkCmd->f0 = sem;
    symlinkCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 0x18, 0, &D_0052D700.symlink, 2060, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceReadlink);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceReadlink_d(char *, int, unsigned) __asm__("sceReadlink");
/**
 * Reads a symbolic link's target through the IOP file server.
 *
 * Takes the call lock (passing 17, ignored) and initialises the library when needed (func_00427A90). Copies the path
 * (up to 0x3FF characters), caps @p len at 0x3FF, stores @p targetBuf and the length, writes @p targetBuf back from the data cache
 * and sends RPC function 25 with a new semaphore and a stack result word; func_00427590 copies the returned text to
 * @p targetBuf. The send size (2060 bytes) is larger than this request's 1044-byte layout, so stale bytes of D_0052D700 go
 * along.
 *
 * @param linkPath  the link's path
 * @param targetBuf  buffer for the target
 * @param len  size of @p targetBuf
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int sceReadlink_d(char *linkPath, int targetBuf, unsigned len) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    sFioPathArgCmd *readlinkCmd = &D_0052D700.readlink;
    func_00427988_b(17);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((readlinkCmd->name[0] = linkPath[0]) != 0) {
        while (++n < 0x400) {
            if ((readlinkCmd->name[n] = linkPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        readlinkCmd->name[0x3FF] = 0;
    }
    if (len >= 0x400) {
        len = 0x3FF;
    }
    readlinkCmd->f10 = targetBuf;
    readlinkCmd->fC = len;
    func_00426358(targetBuf, len);
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    readlinkCmd->f4 = (int)&buf[8];
    readlinkCmd->f0 = sem;
    readlinkCmd->f8 = 4;
    if (sceSifCallRpc(&D_0052EE00, 25, 0, &D_0052D700.readlink, 2060, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    accepted = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (accepted == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifInitIopHeap);
#ifdef SKIP_ASM
/**
 * Binds the IOP heap RPC server, retrying until it answers.
 *
 * Binds D_0052EEC0 to server 0x80000003 (sceSifBindRpc); while the client has no server yet it spins 0x100000
 * iterations and binds again. Once bound it sets D_00455224 to 0, which the heap calls (sceSifAllocIopHeap ...
 * func_0042AFE8) check. Called by the IOP boot code (func_00319610, func_00319820) and SNDPLATFORM_init.
 *
 * @return 0, or -1 when a bind fails
 */
int sceSifInitIopHeap(void) {
    int i;
    for (;;) {
        i = sceSifBindRpc(&D_0052EEC0, 0x80000003, 0);
        if (i < 0) {
            return -1;
        }
        if (D_0052EEC0.server != 0) {
            D_00455224 = 0;
            break;
        }
        i = 0x100000;
        do {
            i--;
        } while (i != -1);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifAllocIopHeap);
#ifdef SKIP_ASM
/**
 * Allocates a block of IOP memory through the heap RPC server.
 *
 * When the heap server is bound (D_00455224 >= 0) it sends request 1 with @p allocSize (D_0052EF40) and returns the reply word
 * D_0052EF00.
 *
 * @param allocSize  bytes to allocate
 * @return the IOP address of the block, or 0 when unbound or the RPC fails
 */
int sceSifAllocIopHeap(int allocSize) {
    if (D_00455224 >= 0) {
        D_0052EF40 = allocSize;
        if (sceSifCallRpc(&D_0052EEC0, 1, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) >= 0) {
            return D_0052EF00;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AC10);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 4 with three words and returns the reply.
 *
 * When the heap server is bound (D_00455224 >= 0) it stores @p a1, @p a0 and @p a2, in that order, in D_0052EF40, sends
 * them (12 bytes) and returns the reply word D_0052EF00. No caller in this build.
 *
 * @param a0  second word of the request
 * @param a1  first word of the request
 * @param a2  third word of the request
 * @return the reply (an IOP address), or 0 when unbound or the RPC fails
 */
int func_0042AC10(int a0, int a1, int a2) {
    int *rpcArgs;
    if (D_00455224 < 0) {
        return 0;
    }
    rpcArgs = &D_0052EF40;
    rpcArgs[0] = a1;
    rpcArgs[1] = a0;
    rpcArgs[2] = a2;
    if (sceSifCallRpc(&D_0052EEC0, 4, 0, rpcArgs, 0xC, &D_0052EF00, 4, 0, 0) >= 0) {
        return D_0052EF00;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AC90);
#ifdef SKIP_ASM
int func_0042AC90_impl(int a0) __asm__("func_0042AC90");
/**
 * Frees a block of IOP memory through the heap RPC server.
 *
 * When the heap server is bound (D_00455224 >= 0) it sends request 2 with @p blockAddr and returns the reply word D_0052EF00.
 * func_0042AD08 forwards to it.
 *
 * @param blockAddr  IOP address of the block
 * @return the server's result; 0 when unbound, -1 when the RPC fails
 */
int func_0042AC90_impl(int blockAddr) {
    if (D_00455224 < 0) {
        return 0;
    }
    D_0052EF40 = blockAddr;
    if (sceSifCallRpc(&D_0052EEC0, 2, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) < 0) {
        return -1;
    }
    return D_0052EF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AD08);
#ifdef SKIP_ASM
extern int func_0042AC90(void);
/**
 * Frees a block of IOP memory: forwards its argument register unchanged to func_0042AC90.
 *
 * Declared without parameters; the caller's address in the first argument register reaches func_0042AC90 as is. Called
 * by SNDPLATFORM_memfree and SNDPLATFORM_restore.
 *
 * @return the result of func_0042AC90
 */
int func_0042AD08(void) {
    return func_0042AC90();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadIopHeap);
#ifdef SKIP_ASM
struct S2AD28 { int cmd; char name[0xFC]; };
extern struct S2AD28 D_0052EF80;
/**
 * Loads a file into IOP memory through the heap RPC server.
 *
 * When the heap server is bound (D_00455224 >= 0) it sends request 3 with @p iopAddr and the path (copied into D_0052EF80,
 * up to 0xFB characters; the request is 4 bytes plus the name and its NUL) and returns the reply word D_0052EF00. No
 * caller in this build.
 *
 * @param filePath  the file's path
 * @param iopAddr  IOP address to load it to
 * @return the server's result; 0 when unbound, -1 when the RPC fails
 */
int sceSifLoadIopHeap(char *filePath, int iopAddr) {
    int i;
    if (D_00455224 < 0) {
        return 0;
    }
    i = 0;
    if ((D_0052EF80.name[0] = filePath[0]) != 0) {
        do {
            i++;
            if (i >= 0xFC) {
                break;
            }
        } while ((D_0052EF80.name[i] = filePath[i]) != 0);
    }
    if (i == 0xFC) {
        D_0052EF80.name[0xFB] = 0;
        i = 251;
    }
    D_0052EF80.cmd = iopAddr;
    D_0052EF80.name[0xFB] = 0;
    if (sceSifCallRpc(&D_0052EEC0, 3, 0, &D_0052EF80, i + 5, &D_0052EF00, 4, 0, 0) < 0) {
        return -1;
    }
    return D_0052EF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AE18);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 5 (no arguments) and returns the reply word D_0052EF00.
 *
 * No caller in this build.
 *
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AE18(void) {
    if (D_00455224 < 0) {
        return 0;
    }
    if (sceSifCallRpc(&D_0052EEC0, 5, 0, 0, 0, &D_0052EF00, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0052EF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AE88);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 6 (no arguments) and returns the reply word D_0052EF00.
 *
 * No caller in this build.
 *
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AE88(void) {
    if (D_00455224 < 0) {
        return 0;
    }
    if (sceSifCallRpc(&D_0052EEC0, 6, 0, 0, 0, &D_0052EF00, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0052EF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AEF8);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 7 (no arguments) and returns the reply word D_0052EF00.
 *
 * No caller in this build.
 *
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AEF8(void) {
    if (D_00455224 < 0) {
        return 0;
    }
    if (sceSifCallRpc(&D_0052EEC0, 7, 0, 0, 0, &D_0052EF00, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0052EF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AF68);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 8 with an IOP address and returns the reply word D_0052EF00.
 *
 * No caller in this build.
 *
 * @param blockAddr  IOP address of a memory block
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AF68(int blockAddr) {
    if (D_00455224 < 0) {
        return 0;
    }
    D_0052EF40 = blockAddr;
    if (sceSifCallRpc(&D_0052EEC0, 8, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0052EF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AFE8);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 9 with an IOP address and returns the reply word D_0052EF00.
 *
 * No caller in this build.
 *
 * @param blockAddr  IOP address of a memory block
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AFE8(int blockAddr) {
    if (D_00455224 < 0) {
        return 0;
    }
    D_0052EF40 = blockAddr;
    if (sceSifCallRpc(&D_0052EEC0, 9, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0052EF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B068);
#ifdef SKIP_ASM
/**
 * Binds the IOP module-loader RPC server once and reads its version.
 *
 * Does nothing when D_00455228 is already >= 0. Otherwise binds D_0052F280 to server 0x80000006 (sceSifBindRpc),
 * spinning 0x100000 iterations between attempts until the client has a server, sets D_00455228 to 0 and fetches the
 * 4-byte version with request 0xFF into D_0052F2A8. Every module-loader call in this unit calls it first.
 *
 * @return 0; -1 when a bind fails; 0xFFFEFFFF when the version call fails
 */
int func_0042B068(void) {
    int i;
    if (D_00455228 < 0) {
        for (;;) {
            if (sceSifBindRpc(&D_0052F280, 0x80000006, 0) < 0) {
                return -1;
            }
            if (D_0052F280.server != 0) {
                D_00455228 = 0;
                if (sceSifCallRpc(&D_0052F280, 0xFF, 0, 0, 0, &D_0052F080.ver, 4, 0, 0) < 0) {
                    return 0xFFFEFFFF;
                }
                D_0052F2A8 = D_0052F080.ver;
                return 0;
            }
            i = 0x100000;
            do {
                i--;
            } while (i != -1);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B168);
#ifdef SKIP_ASM
extern char D_00455164[];
extern char *D_0045522C;
extern int memcmp(char *, char *, int);
/**
 * Returns whether the bound module-loader server's version tag is one the library does not accept.
 *
 * Same test as func_00427C98 on D_0052F2A8: 0 when it equals the library's "2710" (D_00455164) or the tag D_0045522C
 * points at (memcmp, memcmp), else 1 (the final compare of the two accepted tags always differs).
 *
 * @return 1 when the tag matches neither accepted tag, else 0
 */
int func_0042B168(void) {
    char *libVersion = D_00455164;
    char *serverVersion = D_0052F2A8.b;
    int mismatch = 0;
    if (memcmp(serverVersion, libVersion, 4) != 0 &&
        memcmp(serverVersion, D_0045522C, 4) != 0) {
        mismatch = memcmp(libVersion, D_0045522C, 4) != 0;
    }
    return mismatch;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadFileReset);
#ifdef SKIP_ASM
extern void memset(void *a0, int a1, int a2);
/**
 * Marks the module-loader server as unbound (D_00455228 = -1) and zeroes its version D_0052F2A8 (memset).
 *
 * The next module-loader call rebinds it (func_0042B068). No caller in this build.
 *
 * @return 0
 */
int sceSifLoadFileReset(void) {
    D_00455228 = -1;
    memset(&D_0052F2A8, 0, 4);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B230);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
extern void *func_0041605C(void *, void *, int);
int func_0042B230_i(int a0, int a1, sLfData *a2, int *a3) __asm__("func_0042B230");
/**
 * Sends a module-loader request that names an IOP memory address plus an argument block, and returns its results.
 *
 * Binds the loader when needed (func_0042B068; 0xFFFF0000 on failure) and refuses an unaccepted server version
 * (func_0042B168; 0xFFFEFFFC). D_0052F080 gets @p modAddr, the argument length and the arguments (at most 0xFC bytes; a
 * length of 253 or more copies a full 0xFC block); request 6 is sent (0x200 bytes) and the 8-byte reply read back in
 * place. Called by sceSifLoadModuleBuffer and sceSifLoadStartModuleBuffer.
 *
 * @param modAddr  IOP address of the module image
 * @param argLen  bytes of @p argBlock
 * @param argBlock  argument block, or 0
 * @param resultOut  receives the reply's second word
 * @return the reply's first word, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B230_i(int modAddr, int argLen, sLfData *argBlock, int *resultOut) {
    int modId;
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = modAddr;
    if (argBlock != 0) {
        if (argLen >= 253) {
            *(sLfData *)D_0052F080.cmd.n2 = *argBlock;
            D_0052F080.cmd.f4 = 0xFC;
        } else {
            func_0041605C(D_0052F080.cmd.n2, argBlock, argLen);
            D_0052F080.cmd.f4 = argLen;
        }
    } else {
        D_0052F080.cmd.f4 = 0;
    }
    if (sceSifCallRpc(&D_0052F280, 6, 0, &D_0052F080, 0x200, &D_0052F080, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    modId = D_0052F080.cmd.f0;
    *resultOut = D_0052F080.cmd.f4;
    return modId;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifStopModule);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
extern void *func_0041605C(void *, void *, int);
int sceSifStopModule_i(int a0, int a1, sLfData *a2, int *a3) __asm__("sceSifStopModule");
/**
 * Sends module-loader request 7 for a module with an argument block, and returns its results.
 *
 * Same checks and request layout as func_0042B230 (module id, argument length and up to 0xFC bytes of arguments).
 * func_00319790 and func_003F5E90 call it with no arguments and then sceSifUnloadModule on the same module.
 *
 * @param modId  IOP module id
 * @param argLen  bytes of @p argBlock
 * @param argBlock  argument block, or 0
 * @param resultOut  receives the reply's second word
 * @return the reply's first word, 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF when the RPC fails
 */
int sceSifStopModule_i(int modId, int argLen, sLfData *argBlock, int *resultOut) {
    int replyId;
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = modId;
    if (argBlock != 0) {
        if (argLen >= 253) {
            *(sLfData *)D_0052F080.cmd.n2 = *argBlock;
            D_0052F080.cmd.f4 = 0xFC;
        } else {
            func_0041605C(D_0052F080.cmd.n2, argBlock, argLen);
            D_0052F080.cmd.f4 = argLen;
        }
    } else {
        D_0052F080.cmd.f4 = 0;
    }
    if (sceSifCallRpc(&D_0052F280, 7, 0, &D_0052F080, 0x200, &D_0052F080, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    replyId = D_0052F080.cmd.f0;
    *resultOut = D_0052F080.cmd.f4;
    return replyId;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifUnloadModule);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
/**
 * Sends module-loader request 8 for a module id and returns the reply word.
 *
 * Binds the loader when needed (func_0042B068) and checks its version (func_0042B168). Called after sceSifStopModule by
 * func_00319790 and func_003F5E90.
 *
 * @param modId  IOP module id
 * @return the reply, 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF when the RPC fails
 */
int sceSifUnloadModule(int modId) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = modId;
    if (sceSifCallRpc(&D_0052F280, 8, 0, &D_0052F080, 4, &D_0052F080, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return D_0052F080.cmd.f0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSearchModuleByName);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
extern char *strncpy(char *, const char *, int);
/**
 * Asks the IOP module loader for a module by name (request 9) and returns the reply word.
 *
 * Binds the loader when needed (func_0042B068) and checks its version (func_0042B168); the name is copied with strncpy
 * (0xFC bytes, last one forced to NUL). func_00319930 calls it with "dev9" and sets up the power-off thread only when
 * the result is not negative.
 *
 * @param modName  module name
 * @return the reply (negative when not found), 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF
 */
int sceSifSearchModuleByName(char *modName) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(D_0052F080.cmd.n1, modName, 0xFC);
    D_0052F080.cmd.n1[0xFB] = 0;
    if (sceSifCallRpc(&D_0052F280, 9, 0, &D_0052F080, 0x200, &D_0052F080, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return D_0052F080.cmd.f0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B770);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
/**
 * Sends module-loader request 10 with one word and returns the reply word.
 *
 * Binds the loader when needed (func_0042B068) and checks its version (func_0042B168). No caller in this build.
 *
 * @param iopAddr  the word sent (an IOP address)
 * @return the reply, 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B770(int iopAddr) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = iopAddr;
    if (sceSifCallRpc(&D_0052F280, 10, 0, &D_0052F080, 4, &D_0052F080, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return D_0052F080.cmd.f0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadModuleBuffer);
#ifdef SKIP_ASM
extern int func_0042B230(int a0, int a1, int a2, int *a3);
/**
 * Sends module-loader request 6 for a module image already in IOP memory, discarding the second reply word.
 *
 * Calls func_0042B230 with a stack word for its result. No caller in this build.
 *
 * @param modBuf  IOP address of the module image
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @return the result of func_0042B230
 */
int sceSifLoadModuleBuffer(int modBuf, int argLen, int args) {
    int sp;
    return func_0042B230(modBuf, argLen, args, &sp);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadStartModuleBuffer);
#ifdef SKIP_ASM
extern int func_0042B230(int a0, int a1, int a2, int *a3);
/**
 * Sends module-loader request 6 for a module image already in IOP memory and passes the second reply word back.
 *
 * Calls func_0042B230 with @p modResult. No caller in this build.
 *
 * @param modBuf  IOP address of the module image
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @param modResult  receives the reply's second word
 * @return the result of func_0042B230
 */
int sceSifLoadStartModuleBuffer(int modBuf, int argLen, int args, int *modResult) {
    return func_0042B230(modBuf, argLen, args, modResult);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B840);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
extern char *strncpy(char *, const char *, int);
extern void *func_0041605C(void *, void *, int);
int func_0042B840_i(char *a0, int a1, sLfData *a2, int *a3, int a4) __asm__("func_0042B840");
/**
 * Sends a module-loader request that names a file plus an argument block, and returns its results.
 *
 * Binds the loader when needed (func_0042B068; 0xFFFF0000 on failure) and refuses an unaccepted server version
 * (func_0042B168; 0xFFFEFFFC). D_0052F080 gets the path (strncpy, 0xFC bytes, last forced to NUL), the argument length
 * and the arguments (at most 0xFC bytes; a length of 253 or more copies a full block; no arguments leave an empty
 * string). Request @p rpcFunc is sent (0x200 bytes) and the 8-byte reply read back in place. Called by sceSifLoadModule and
 * sceSifLoadStartModule with request 0.
 *
 * @param modPath  the module's path
 * @param argLen  bytes of @p argBlock
 * @param argBlock  argument block, or 0
 * @param resultOut  receives the reply's second word
 * @param rpcFunc  module-loader request number
 * @return the reply's first word, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B840_i(char *modPath, int argLen, sLfData *argBlock, int *resultOut, int rpcFunc) {
    int modId;
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(D_0052F080.cmd.n1, modPath, 0xFC);
    D_0052F080.cmd.n1[0xFB] = 0;
    if (argBlock != 0) {
        if (argLen >= 253) {
            *(sLfData *)D_0052F080.cmd.n2 = *argBlock;
            D_0052F080.cmd.f0 = 0xFC;
        } else {
            func_0041605C(D_0052F080.cmd.n2, argBlock, argLen);
            D_0052F080.cmd.f0 = argLen;
        }
    } else {
        D_0052F080.cmd.n2[0] = 0;
        D_0052F080.cmd.f0 = 0;
    }
    if (sceSifCallRpc(&D_0052F280, rpcFunc, 0, &D_0052F080, 0x200, &D_0052F080, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    modId = D_0052F080.cmd.f0;
    *resultOut = D_0052F080.cmd.f4;
    return modId;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadModule);
#ifdef SKIP_ASM
extern int func_0042B840(int a0, int a1, int a2, int *a3, int a4);
/**
 * Loads an IOP module from a file (module-loader request 0), discarding the module's own result.
 *
 * Calls func_0042B840 with a stack result word. func_00319718 (the game's module loader, retrying on -204) and
 * SNDPLATFORM_init call it.
 *
 * @param path  the module's path
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @return the module id, or a negative error
 */
int sceSifLoadModule(int path, int argLen, int args) {
    int sp;
    return func_0042B840(path, argLen, args, &sp, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadStartModule);
#ifdef SKIP_ASM
extern int func_0042B840(int a0, int a1, int a2, int *a3, int a4);
/**
 * Loads an IOP module from a file (module-loader request 0) and passes the module's own result back.
 *
 * Calls func_0042B840 with @p modResult. No caller in this build.
 *
 * @param path  the module's path
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @param modResult  receives the module's start result
 * @return the module id, or a negative error
 */
int sceSifLoadStartModule(int path, int argLen, int args, int *modResult) {
    return func_0042B840(path, argLen, args, modResult, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BAA8);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
extern char *strncpy(char *, const char *, int);
int func_0042BAA8_i(char *, char *, int *, int) __asm__("func_0042BAA8");
/**
 * Loads (part of) an ELF file into EE memory through the IOP module loader and returns its entry point and gp.
 *
 * Binds the loader when needed (func_0042B068; 0xFFFF0000) and checks its version (func_0042B168; 0xFFFEFFFC). Copies
 * the path and the section name (strncpy, 0xFC bytes each, last forced to NUL) into D_0052F080 and sends request
 * @p rpcFunc, reading a 16-byte reply back in place. A zero first reply word gives 0xFFFEFFFD; otherwise the first two words
 * go to @p execData[0] and @p execData[1]. Called by sceSifLoadElfPart and sceSifLoadElf with request 1.
 *
 * @param elfPath  the ELF's path
 * @param sectionName  section name ("all" for the whole file)
 * @param execData  receives the entry point and gp
 * @param rpcFunc  module-loader request number
 * @return 0, 0xFFFEFFFD when nothing was loaded, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BAA8_i(char *elfPath, char *sectionName, int *execData, int rpcFunc) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(D_0052F080.cmd.n1, elfPath, 0xFC);
    D_0052F080.cmd.n1[0xFB] = 0;
    strncpy(D_0052F080.cmd.n2, sectionName, 0xFC);
    D_0052F080.cmd.n2[0xFB] = 0;
    if (sceSifCallRpc(&D_0052F280, rpcFunc, 0, &D_0052F080, 0x200, &D_0052F080, 0x10, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    if (D_0052F080.cmd.f0 == 0) {
        return 0xFFFEFFFD;
    }
    execData[0] = D_0052F080.cmd.f0;
    execData[1] = D_0052F080.cmd.f4;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadElfPart);
#ifdef SKIP_ASM
extern int func_0042BAA8(int a0, char *a1, int a2, int a3);
/**
 * Loads a named part of an ELF file into EE memory: func_0042BAA8 with request 1.
 *
 * @param path  the ELF's path
 * @param secName  section name
 * @param execData  receives the entry point and gp
 * @return the result of func_0042BAA8
 */
int sceSifLoadElfPart(int path, char *secName, int execData) {
    return func_0042BAA8(path, secName, execData, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadElf);
#ifdef SKIP_ASM
extern char D_0049AD58[];
extern int func_0042BAA8(int a0, char *a1, int a2, int a3);
/**
 * Loads a whole ELF file into EE memory: func_0042BAA8 with the section name "all" (D_0049AD58) and request 1.
 *
 * @param elfPath  the ELF's path
 * @param execData  receives the entry point and gp
 * @return the result of func_0042BAA8
 */
int sceSifLoadElf(int elfPath, int execData) {
    return func_0042BAA8(elfPath, D_0049AD58, execData, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BBF8);
#ifdef SKIP_ASM
extern int func_0042B068(void);
/**
 * Reads a byte, halfword or word of IOP memory through the module loader.
 *
 * Binds the loader when needed (func_0042B068; 0xFFFF0000). Sizes above 2 give 0xFFFEFFFE. Sends request 3 with the
 * address and size (0x20 bytes) and stores the reply's first byte, halfword or word to @p valueOut. It does not check the
 * server version. No caller in this build.
 *
 * @param iopAddr  IOP address
 * @param valueOut  where the value is stored
 * @param valueType  size: 0 byte, 1 halfword, 2 word
 * @return 0, 0xFFFEFFFE for a bad size, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BBF8(int iopAddr, void *valueOut, unsigned valueType) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (valueType > 2) {
        return 0xFFFEFFFE;
    }
    D_0052F080.get.f0.w = iopAddr;
    D_0052F080.get.f4 = valueType;
    if (sceSifCallRpc(&D_0052F280, 3, 0, &D_0052F080, 0x20, &D_0052F080, 0x20, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    if (valueType == 0) {
        *(unsigned char *)valueOut = D_0052F080.get.f0.b;
    } else if (valueType == 1) {
        *(unsigned short *)valueOut = D_0052F080.get.f0.h;
    } else if (valueType == 2) {
        *(int *)valueOut = D_0052F080.get.f0.w;
    } else {
        return 0xFFFEFFFE;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BCE8);
#ifdef SKIP_ASM
extern int func_0042B068(void);
/**
 * Writes a byte, halfword or word to IOP memory through the module loader.
 *
 * Binds the loader when needed (func_0042B068; 0xFFFF0000). Sizes above 2 give 0xFFFEFFFE. Sends request 2 with the
 * address, the size and the value read from @p valueIn (0x20 bytes, 16-byte reply). It does not check the server version. No
 * caller in this build.
 *
 * @param iopAddr  IOP address
 * @param valueIn  the value to write
 * @param valueType  size: 0 byte, 1 halfword, 2 word
 * @return 0, 0xFFFEFFFE for a bad size, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BCE8(int iopAddr, void *valueIn, int valueType) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    D_0052F080.set.f0 = iopAddr;
    D_0052F080.set.f4 = valueType;
    if (valueType == 0) {
        D_0052F080.set.f8.b = *(unsigned char *)valueIn;
    } else if (valueType == 1) {
        D_0052F080.set.f8.h = *(unsigned short *)valueIn;
    } else if (valueType == 2) {
        D_0052F080.set.f8.w = *(unsigned *)valueIn;
    } else {
        return 0xFFFEFFFE;
    }
    if (sceSifCallRpc(&D_0052F280, 2, 0, &D_0052F080.set, 0x20, &D_0052F080.set, 0x10, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifResetIop);
#ifdef SKIP_ASM
struct HdrBDC8 {
    unsigned long psize:8;
    unsigned long dsize:24;
    unsigned int dest;
    int cid;
    int opt;
    int size;
    int mode;
    char arg[0x50];
};
extern struct HdrBDC8 D_0052F2C0;
extern void fBDC8_stop(void) __asm__("sceSifStopDma");
extern int fBDC8_getreg(unsigned int) __asm__("sceSifGetReg");
extern unsigned int fBDC8_setreg(unsigned int, unsigned int) __asm__("sceSifSetReg");
extern void func_00426358(int, int);
/**
 * Sends the IOP a reset command with a boot argument string over SIF DMA.
 *
 * Stops SIF DMA (sceSifStopDma) and reads the IOP's command buffer address (sceSifGetReg(0x80000000)). Builds the
 * 0x68-byte command D_0052F2C0: id 0x80000003, the argument string @p bootArg (copied without its NUL, unchecked against the
 * 0x50-byte field), its length and @p mode. Writes it back from the data cache, sets SIF register 4 to 0x40000 and DMAs
 * it (attribute 0x44) with sceSifSetDma. When the transfer is queued it sets register 4 to 0x10000 and 0x20000 and
 * clears registers 0x80000002 and 0x80000000. Called by sceSifRebootIop.
 *
 * @param bootArg  boot argument string (e.g. "rom0:UDNL <image>")
 * @param mode  reset mode passed to the IOP
 * @return 1 when the command was queued, 0 when the DMA was refused
 */
int sceSifResetIop(char *bootArg, int mode) {
    sSifDmaData dma;
    int reg;
    int n;
    fBDC8_stop();
    reg = fBDC8_getreg(0x80000000);
    D_0052F2C0.mode = mode;
    for (n = 0; bootArg[n] != 0; n++) D_0052F2C0.arg[n] = bootArg[n];
    D_0052F2C0.dest = 0;
    D_0052F2C0.size = n;
    D_0052F2C0.cid = 0x80000003;
    D_0052F2C0.dsize = 0;
    D_0052F2C0.psize = 104;
    dma.data = (int)&D_0052F2C0;
    dma.addr = reg;
    dma.size = 104;
    dma.attr = 68;
    func_00426358((int)&D_0052F2C0, 104);
    fBDC8_setreg(4, 0x40000);
    if (sceSifSetDma(&dma, 1) != 0) {
        fBDC8_setreg(4, 0x10000);
        fBDC8_setreg(4, 0x20000);
        fBDC8_setreg(0x80000002, 0);
        fBDC8_setreg(0x80000000, 0);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifIsAliveIop);
#ifdef SKIP_ASM
extern int sceSifGetReg(int a0);
/**
 * Returns whether the IOP has finished SIF initialisation: bit 0x10000 of SIF register 4 (sceSifGetReg).
 *
 * @return 1 when the bit is set, else 0
 */
int sceSifIsAliveIop(void) { int bootEndFlag = sceSifGetReg(4) & 0x10000; return bootEndFlag != 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSyncIop);
#ifdef SKIP_ASM
extern int sceSifGetReg(int a0);
extern void func_00424220(void);
/**
 * Returns whether the IOP has finished rebooting.
 *
 * When bit 0x40000 of SIF register 4 (sceSifGetReg) is set it clears D_00455168 (func_00424220) and returns 1.
 * func_00319820 polls it after rebooting the IOP with sceSifRebootIop.
 *
 * @return 1 when the reboot is done, else 0
 */
int sceSifSyncIop(void) {
    if (sceSifGetReg(4) & 0x40000) {
        func_00424220();
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifRebootIop);
#ifdef SKIP_ASM
/* sceSifRebootIop (libkernel): reset the IOP with "rom0:UDNL <img>". */
extern void scePrintf_s(const char *fmt, ...) __asm__("scePrintf");
/**
 * Reboots the IOP with an IOP image through "rom0:UDNL".
 *
 * Refuses an image path whose length plus 11 exceeds 80 (prints "too long parameter '%s'\n" with scePrintf and
 * returns 0). Otherwise calls func_00426408(0) and sceSifExitRpc, builds "rom0:UDNL " + @p img and sends it with
 * sceSifResetIop (mode 0). func_00319820 retries it with the DNAS image until it succeeds.
 *
 * @param img  path of the IOP image
 * @return 1 when the reset command was sent, else 0
 */
int sceSifRebootIop(const char *img) {
    char buf[80];
    const char *prefix = "rom0:UDNL ";
    char *dst;
    const char *imgEnd = img;
    __SIZE_TYPE__ imgBias;
    while (*imgEnd)
        imgEnd++;
    /* PORT: integer pointer arithmetic (__SIZE_TYPE__ is pointer-width). */
    /* NOTE: the length test is retail's p - (img - 11) > 80 (strlen + 11 > 80); the separate statement keeps fold from
       reassociating it into (p - img) + 11. Integer arithmetic, so no out-of-bounds pointer. */
    imgBias = (__SIZE_TYPE__)img - 11;
    if ((__SIZE_TYPE__)imgEnd - imgBias > 80) {
        scePrintf_s("too long parameter '%s'\n", img);
        return 0;
    }
    func_00426408_z(0);
    sceSifExitRpc();
    dst = buf;
    while (*prefix)
        *dst++ = *prefix++;
    while (*img)
        *dst++ = *img++;
    *dst = 0;
    return sceSifResetIop(buf, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", DIntr);
#ifdef SKIP_ASM
/**
 * DIntr (libkernel): disable interrupts, return whether they were enabled.
 *
 * Reads the COP0 Status register's EIE bit (0x10000); when it is set, repeats di + sync.p until Status shows it clear.
 *
 * @return 1 when interrupts were enabled on entry, else 0
 */
int DIntr_r(void) {
    int eie, st;
    MFC0_STATUS_4248(eie);
    eie &= 0x10000;
    if (eie) {
        do {
            DI_4248();
            SYNCP_4248();
            MFC0_STATUS_4248(st);
        } while (st & 0x10000);
    }
    return eie != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", EIntr);
#ifdef SKIP_ASM
int EIntr_r(void) __asm__("EIntr");
/**
 * Enables interrupts (ei) and returns whether they were already enabled.
 *
 * @return 1 when Status.EIE (0x10000) was set before, else 0
 */
int EIntr_r(void) {
    int status, eie;
    MFC0_STATUS_4248(status);
    eie = status & 0x10000;
    EI_4248();
    return eie != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C0D8);
#ifdef SKIP_ASM
extern int D_00455248;
extern int D_0045524C;
extern int CreateSema(int *a0);
/**
 * Creates two semaphores (max and initial count 1) and stores their ids in D_00455248 and D_0045524C.
 *
 * D_00455248 is the one the recursive lock __malloc_lock / __malloc_unlock waits on and signals. Called by _InitSys
 * at start-up.
 */
void func_0042C0D8(void) {
    int firstSemaParam[8];
    int secondSemaParam[8];
    firstSemaParam[1] = 1;
    firstSemaParam[2] = 1;
    secondSemaParam[1] = 1;
    secondSemaParam[2] = 1;
    D_00455248 = CreateSema(firstSemaParam);
    D_0045524C = CreateSema(secondSemaParam);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C120);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * Loads the number into $v1 and executes `syscall`. func_0042C1F0 first installs the word-copy routine func_0042C130 as
 * syscall 90; func_0042C1B8 then uses this stub to read a word of kernel memory.
 */
SYSCALL_STUB(func_0042C120, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C130);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by func_0042C1F0 as kernel syscall 90.
 *
 * Runs in kernel mode as the syscall handler, so it can read and write kernel memory for the EE-side stubs.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int func_0042C130(int *dst, int *src, unsigned int size) {
    unsigned int n = size >> 2;
    unsigned int i;
    for (i = 0; i < n; i++) {
        *dst++ = *src++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C168);
#ifdef SKIP_ASM
/**
 * Returns the address of the first word in [@p cursor, @p end) equal to @p value, or 0.
 *
 * Installed by func_0042C1F0 as kernel syscall 131 so that the search runs over kernel memory. The word at @p cursor is
 * read before the bound check, so a failed search reads the word at @p end.
 *
 * @param cursor  first word to test
 * @param end  end of the range
 * @param value  the word to look for
 * @return the matching address, or 0
 */
int *func_0042C168(int *cursor, int *end, int value) {
    while (*cursor != value && cursor < end) cursor++;
    return cursor < end ? cursor : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", FindAddress);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 131 (FindAddress in the PS2 SDK's syscall table).
 *
 * Loads the number into $v1 and executes `syscall`; the kernel does the work. The stub declares no parameters; the
 * caller's arguments reach the kernel in the argument registers untouched.
 */
SYSCALL_STUB(FindAddress, 131)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C1B8);
#ifdef SKIP_ASM
extern void func_0042C120(void *a0, int a1, int a2);
/**
 * Returns entry @p syscallNum of the kernel's syscall table: copies the word at D_00455230 + 4 * @p syscallNum with syscall 90
 * (func_0042C120).
 *
 * D_00455230 is the table base func_0042C1F0 found. No caller in this build.
 *
 * @param syscallNum  syscall number
 * @return the handler address in that slot
 */
int func_0042C1B8(int syscallNum) {
    int handlerAddr;
    func_0042C120(&handlerAddr, D_00455230 + syscallNum * 4, 4);
    return handlerAddr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C1F0);
#ifdef SKIP_ASM
extern int D_00455238[];
extern int FindAddress(int, int, void *);
extern void func_0042C2F0(int, int);
/**
 * Installs two helper syscalls and locates the kernel's syscall table, storing its base in D_00455230.
 *
 * Installs the pairs of D_00455238 with syscall 116 (func_0042C2F0): func_0042C168 as syscall 131 (word search) and
 * func_0042C130 as syscall 90 (word copy). Then searches kernel memory 0x80000000-0x80080000 with FindAddress for words
 * holding those two handler addresses: each hit minus the slot offset (131 * 4 = 524, 90 * 4 = 360) is a candidate
 * table base, and the lower candidate is advanced to its next hit until both agree. Called by _InitSys at
 * start-up.
 */
void func_0042C1F0(void) {
    int searchHit;
    int copyHit;
    unsigned x;
    unsigned y;
    func_0042C2F0(D_00455238[0], D_00455238[1]);
    func_0042C2F0(D_00455238[2], D_00455238[3]);
    searchHit = FindAddress(0x80000000, 0x80080000, (void *)func_0042C168);
    x = searchHit - 524;
    copyHit = FindAddress(0x80000000, 0x80080000, (void *)func_0042C130);
    y = copyHit - 360;
    while (x != y) {
        if (x < y) {
            searchHit = FindAddress(searchHit + 4, 0x80080000, (void *)func_0042C168);
            x = searchHit - 524;
        } else {
            copyHit = FindAddress(copyHit + 4, 0x80080000, (void *)func_0042C130);
            y = copyHit - 360;
        }
    }
    D_00455230 = x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C2F0);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by func_0042C1F0 to install the pairs of D_00455238.
 */
SYSCALL_STUB(func_0042C2F0, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _InitSys);
#ifdef SKIP_ASM
extern void func_0042C0D8(void);
extern void func_0042C1F0(void);
extern void func_0042C7C8(void);
extern void InitThread(void);
extern void func_0042C410(void);
extern void func_0042CBD0(void);
/**
 * Runs the kernel library's start-up initialisation.
 *
 * Creates the two library semaphores (func_0042C0D8), installs the helper syscalls and finds the syscall table
 * (func_0042C1F0), patches the alarm syscalls (func_0042C7C8), starts the wake-up helper thread (InitThread),
 * patches the OSD configuration syscall when needed (func_0042C410) and installs the TLB syscall patch (func_0042CBD0).
 * Called by the program entry func_00100008 before main.
 */
void _InitSys(void) {
    func_0042C0D8();
    func_0042C1F0();
    func_0042C7C8();
    InitThread();
    func_0042C410();
    func_0042CBD0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C340);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by func_0042C410.
 */
SYSCALL_STUB(func_0042C340, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C350);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * func_0042C410 installs the word-copy routine func_0042C360 as syscall 90 and then copies its kernel patch with this
 * stub.
 */
SYSCALL_STUB(func_0042C350, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C360);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by func_0042C410 as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int func_0042C360(int *dst, int *src, unsigned int size) {
    unsigned int n = size >> 2;
    unsigned int i;
    for (i = 0; i < n; i++) {
        *dst++ = *src++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C398);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 91 with the caller's argument (a syscall number).
 *
 * func_0042C410 first points syscall 91 at the patch it copied to 0x80074000 and then asks it, through this stub, for
 * the handler address of a syscall.
 */
SYSCALL_STUB(func_0042C398, 91)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C3A8);
#ifdef SKIP_ASM
extern void GetOsdConfigParam(unsigned *p);
extern void SetOsdConfigParam(unsigned *p);
/**
 * Returns whether the kernel ignores bits 13-15 of the OSD configuration word.
 *
 * Reads the configuration (GetOsdConfigParam), writes it back with those bits set to 1, reads it again and restores the
 * original. Returns 1 when the read-back field is 0, i.e. the kernel does not keep it. func_0042C410 patches the kernel
 * in that case.
 *
 * @return 1 when the field was not stored, else 0
 */
int func_0042C3A8(void) {
    unsigned osdConfig[2];
    GetOsdConfigParam(&osdConfig[0]);
    osdConfig[1] = (osdConfig[0] & 0xFFFF1FFF) | 0x2000;
    SetOsdConfigParam(&osdConfig[1]);
    GetOsdConfigParam(&osdConfig[1]);
    SetOsdConfigParam(&osdConfig[0]);
    return ((osdConfig[1] >> 13) & 7) == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C410);
#ifdef SKIP_ASM
struct P42C410 { int a; int b; };
extern struct P42C410 D_004559F8[];
extern char D_00455250[];
extern int func_0042C3A8(void);
extern void func_0042C340(int, int);
extern void func_0042C350(int, char *, int);
extern void FlushCache(int);
extern int func_0042C398(int);
/**
 * Patches the kernel's OSD configuration syscalls when the kernel drops the extended configuration bits.
 *
 * Only when func_0042C3A8 reports so: installs func_0042C360 as syscall 90 (first pair of D_004559F8), copies 1960
 * bytes of patch code from D_00455250 to kernel address 0x80074000 with it, flushes the data and instruction caches
 * (FlushCache 0 and 2), installs the patch's entry as syscall 91 (second pair) and then installs, for the number in the
 * third pair (0xFFFFC402), the handler address syscall 91 returns for it. Called by _InitSys.
 */
void func_0042C410(void) {
    unsigned i;
    if (func_0042C3A8() != 0) {
        func_0042C340(D_004559F8[0].a, D_004559F8[0].b);
        func_0042C350(0x80074000, D_00455250, 1960);
        FlushCache(0);
        FlushCache(2);
        func_0042C340(D_004559F8[1].a, D_004559F8[1].b);
        for (i = 2; i < 3; i++) {
            func_0042C340(D_004559F8[i].a, func_0042C398(D_004559F8[i].a));
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C4C0);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * func_0042C510 installs the byte-copy routine func_0042C4D0 as syscall 90 and copies the exec arguments into kernel
 * memory with this stub.
 */
SYSCALL_STUB(func_0042C4C0, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C4D0);
#ifdef SKIP_ASM
/**
 * Copies @p size bytes from @p src to @p dst; installed by func_0042C510 as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int func_0042C4D0(unsigned char *dst, unsigned char *src, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        *dst++ = *src++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C500);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by func_0042C510.
 */
SYSCALL_STUB(func_0042C500, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C510);
#ifdef SKIP_ASM
extern int D_00455A10[];
extern int strlen(char *);
extern int func_0042C4C0(int, void *, int);
extern void func_0042C500(int, int);
int func_0042C510_d(char *, int, char **) __asm__("func_0042C510");
/**
 * Copies a program name and its arguments into the kernel's argument area for the exec calls.
 *
 * Installs the byte-copy routine func_0042C4D0 as syscall 90 (D_00455A10) and caps @p argc at 15. The area at
 * D_00456538 holds a table of string addresses followed, from +0x40, by the strings: first @p progName, then each argv
 * string, each with its NUL; the addresses and strings are written with syscall 90 (func_0042C4C0). Called by
 * ExecPS2, LoadExecPS2 and ExecOSD.
 *
 * @param progName  program name (first string)
 * @param argc  number of argument strings
 * @param argv  the argument strings
 * @return the kernel address of the copied name (D_00456538 + 0x40)
 */
int func_0042C510_d(char *progName, int argc, char **argv) {
    int pos;
    int ptrSlot = D_00456538;
    int ret;
    int len;
    char **argvCursor;
    pos = ptrSlot + 0x40;
    ret = pos;
    func_0042C500(D_00455A10[0], D_00455A10[1]);
    if (argc >= 0x10) {
        argc = 0xF;
    }
    func_0042C4C0(ptrSlot, &pos, 4);
    ptrSlot += 4;
    len = strlen(progName) + 1;
    func_0042C4C0(pos, progName, len);
    pos += len;
    if (argc > 0) {
        argvCursor = argv;
        do {
            func_0042C4C0(ptrSlot, &pos, 4);
            ptrSlot += 4;
            argc--;
            len = strlen(*argvCursor) + 1;
            func_0042C4C0(pos, *argvCursor, len);
            argvCursor++;
            pos += len;
        } while (argc != 0);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C628);
#ifdef SKIP_ASM
extern void func_0042CD58_v(void) __asm__("func_0042CD58");
/**
 * Restores the boot TLB layout: jumps to func_0042CD58.
 *
 * Called before control leaves the program (ExecPS2, LoadExecPS2, Exit, ExecOSD).
 */
void func_0042C628(void) {
    func_0042CD58_v();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", ExecPS2);
#ifdef SKIP_ASM
extern char str_empty_c[];
extern void func_0042C510_v(char *, int, int) __asm__("func_0042C510");
extern void func_0042C628(void);
extern int _ExecPS2(int, int, int, int);
/**
 * Starts code already in memory with new arguments: copies the arguments to the kernel and calls the syscall 7 stub.
 *
 * Copies an empty name plus @p argVector's @p argCount strings into the kernel argument area (func_0042C510), restores the TLB
 * layout (func_0042C628) and calls the syscall 7 stub (seg_3249A0) with the entry point, gp, the argument count and the
 * kernel copy of the argument table (D_00456538 + 4). No caller in this build.
 *
 * @param entryAddr  entry point
 * @param gpValue  gp value
 * @param argCount  argument count
 * @param argVector  argument strings
 * @return the syscall's result (it does not return on success)
 */
int ExecPS2(int entryAddr, int gpValue, int argCount, int argVector) {
    func_0042C510_v(str_empty_c, argCount, argVector);
    func_0042C628();
    return _ExecPS2(entryAddr, gpValue, argCount, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", LoadExecPS2);
#ifdef SKIP_ASM
extern int func_0042C510(int a0, int a1);
extern void _LoadExecPS2(int a0, int a1, int a2);
/**
 * Loads and runs another program: copies its name and arguments to the kernel and calls the syscall 6 stub.
 *
 * The argument strings arrive in the third argument register, which this definition passes on unnamed to
 * func_0042C510. Restores the TLB layout (func_0042C628) and calls the syscall 6 stub (seg_3249A0) with the kernel copy
 * of the name, the count and the kernel argument table (D_00456538 + 4). Called by func_0031B008.
 *
 * @param progPath  path of the program
 * @param argCount  argument count
 */
void LoadExecPS2(int progPath, int argCount) {
    int kernelPath = func_0042C510(progPath, argCount);
    func_0042C628();
    _LoadExecPS2(kernelPath, argCount, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", Exit);
#ifdef SKIP_ASM
extern void KExit(int);
/**
 * Ends the program: restores the TLB layout (func_0042C628) and calls the syscall 4 stub (seg_3249A0) with @p status.
 *
 * The program entry func_00100008 jumps here when main returns, and func_001001C8 jumps here; func_00424578 and the TLB
 * set-up func_0042CD98 (on overflow, status 1) call it.
 *
 * @param status  status code handed to the kernel
 */
void Exit(int status) {
    func_0042C628();
    KExit(status);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", ExecOSD);
#ifdef SKIP_ASM
extern char str_empty_c[];
extern int func_0042C510_3(char *a0, int a1, int a2) __asm__("func_0042C510");
extern void _ExecOSD(int a0, int a1);
/**
 * Returns to the console's OSD (browser) with arguments: copies them to the kernel and calls the syscall 123 stub.
 *
 * Copies an empty name plus the argument strings into the kernel argument area (func_0042C510), restores the TLB
 * layout (func_0042C628) and calls the syscall 123 stub (seg_3249A0) with the count and the kernel argument table
 * (D_00456538 + 4). No caller in this build.
 *
 * @param argCount  argument count
 * @param argVector  argument strings
 */
void ExecOSD(int argCount, int argVector) {
    func_0042C510_3(str_empty_c, argCount, argVector);
    func_0042C628();
    _ExecOSD(argCount, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C760);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by func_0042C7C8.
 */
SYSCALL_STUB(func_0042C760, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C770);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * func_0042C7C8 installs the word-copy routine func_0042C780 as syscall 90 and copies its kernel patch with this stub.
 */
SYSCALL_STUB(func_0042C770, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C780);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by func_0042C7C8 as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int func_0042C780(int *dst, int *src, unsigned int size) {
    unsigned int n = size >> 2;
    unsigned int i;
    for (i = 0; i < n; i++) {
        *dst++ = *src++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C7B8);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 91 with the caller's argument (a syscall number).
 *
 * func_0042C7C8 first points syscall 91 at the patch it copied to 0x80076000 and then asks it, through this stub, for
 * the handler address of each alarm syscall.
 */
SYSCALL_STUB(func_0042C7B8, 91)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C7C8);
#ifdef SKIP_ASM
extern int D_00456180[];
extern char D_00455A18[];
extern char D_00456158[];
extern void FlushCache(int);
extern int func_0042C7B8(int);
extern void func_0042C760(int, int);
extern void func_0042C770(int, char *, int);
/**
 * Patches the kernel's alarm syscalls when timer 3 is not already set up for them.
 *
 * Only when bit 0x100 of the timer 3 mode register (0x10001810) is clear: installs func_0042C780 as syscall 90 (first
 * pair of D_00456180), copies 1856 bytes of patch code from D_00455A18 to kernel address 0x80076000 and 40 bytes from
 * D_00456158 to 0x82000, flushes the data and instruction caches (FlushCache 0 and 2), installs the patch's entry as
 * syscall 91 and then installs, for each of the syscalls 252, 254, 253, 255, 300 and 8, the handler address syscall 91
 * returns for it. Called by _InitSys.
 */
void func_0042C7C8(void) {
    unsigned i;
    int *syscallPair;
    if (!(*(volatile int *)0x10001810 & 0x100)) {
        func_0042C760(D_00456180[0], D_00456180[1]);
        func_0042C770(0x80076000, D_00455A18, 1856);
        func_0042C770(0x82000, D_00456158, 40);
        FlushCache(0);
        FlushCache(2);
        func_0042C760(D_00456180[2], D_00456180[3]);
        syscallPair = &D_00456180[4];
        for (i = 2; i < 8; i++) {
            func_0042C760(syscallPair[0], func_0042C7B8(syscallPair[0]));
            syscallPair += 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2Open);
#ifdef SKIP_ASM
extern char D_0052F328[];
extern int Deci2Call(int a0, int *a1);
int f8A0(unsigned short a0, int a1, int a2) __asm__("sceDeci2Open");
/**
 * Opens a DECI2 socket for a protocol (DECI2 kernel call 1).
 *
 * Passes the protocol, @p handlerArg, the event handler and the uncached address of the work area D_0052F328 to Deci2Call(1).
 * The serial console set-up func_00425260 calls it.
 *
 * @param protocol  DECI2 protocol number
 * @param handlerArg  user data passed to the handler
 * @param eventHandler  event handler
 * @return the socket id, or a negative error
 */
int f8A0(unsigned short protocol, int handlerArg, int eventHandler) {
    int buf[4];
    buf[0] = protocol;
    buf[1] = handlerArg;
    buf[2] = eventHandler;
    buf[3] = (int)D_0052F328 | 0x20000000;
    return Deci2Call(1, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2Close);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Closes a DECI2 socket: Deci2Call(2) with the socket id.
 *
 * @param socketId  the socket
 * @return the kernel's result
 */
int sceDeci2Close(int socketId) {
    return Deci2Call(2, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2ReqSend);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Requests permission to send on a DECI2 socket to a destination: Deci2Call(3) with the socket and destination.
 *
 * Called by the console send routine func_00425040.
 *
 * @param socketId  the socket
 * @param dest  destination node
 */
void sceDeci2ReqSend(int socketId, char dest) {
    int buf[2];
    buf[0] = socketId;
    buf[1] = dest;
    Deci2Call(3, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2Poll);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Polls a DECI2 socket: Deci2Call(4) with the socket id.
 *
 * Called by the console send routine func_00425040 after sceDeci2ReqSend.
 *
 * @param socketId  the socket
 * @return the kernel's result
 */
int sceDeci2Poll(int socketId) {
    return Deci2Call(4, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2ExRecv);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Receives data on a DECI2 socket: Deci2Call(-5) with the socket, buffer and length.
 *
 * Called by the console's DECI2 event handler func_00424EA8.
 *
 * @param socketId  the socket
 * @param dataBuf  receive buffer
 * @param len  bytes to receive
 */
void sceDeci2ExRecv(int socketId, int dataBuf, unsigned short len) {
    int buf[3];
    buf[0] = socketId;
    buf[1] = dataBuf;
    buf[2] = len;
    Deci2Call(-5, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2ExSend);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Sends data on a DECI2 socket: Deci2Call(-6) with the socket, buffer and length.
 *
 * Called by the console's DECI2 event handler func_00424EA8.
 *
 * @param socketId  the socket
 * @param dataBuf  data to send
 * @param len  bytes to send
 */
void sceDeci2ExSend(int socketId, int dataBuf, unsigned short len) {
    int buf[3];
    buf[0] = socketId;
    buf[1] = dataBuf;
    buf[2] = len;
    Deci2Call(-6, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2ExReqSend);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Requests permission to send on a DECI2 socket from interrupt context: Deci2Call(-7) with the socket and destination.
 *
 * No caller in this build.
 *
 * @param socketId  the socket
 * @param dest  destination node
 */
void sceDeci2ExReqSend(int socketId, char dest) {
    int buf[2];
    buf[0] = socketId;
    buf[1] = dest;
    Deci2Call(-7, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2ExLock);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Locks a DECI2 socket: Deci2Call(-8) with the socket id.
 *
 * No caller in this build.
 *
 * @param socketId  the socket
 * @return the kernel's result
 */
int sceDeci2ExLock(int socketId) {
    return Deci2Call(-8, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDeci2ExUnLock);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Unlocks a DECI2 socket: Deci2Call(-9) with the socket id.
 *
 * No caller in this build.
 *
 * @param socketId  the socket
 * @return the kernel's result
 */
int sceDeci2ExUnLock(int socketId) {
    return Deci2Call(-9, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA58);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Prints a string through the DECI2 kernel console: Deci2Call(0x10) with the string's address.
 *
 * The console write routine func_00425358 calls it.
 *
 * @param str  the string
 * @return the kernel's result
 */
int func_0042CA58(int str) {
    return Deci2Call(0x10, &str);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA80);
#ifdef SKIP_ASM
extern int D_004564F0;
extern void func_0042CFC0(void);
extern void SetVTLBRefillHandler(int, void (*)(void));
/**
 * Installs a user handler for TLB exceptions (codes 1-3).
 *
 * Stores @p excHandler in D_004564F0 and points the TLB-refill vectors for exception codes 1, 2 and 3 at the handler stub
 * func_0042CFC0 (SetVTLBRefillHandler), which saves the registers and calls the stored function. No caller in this
 * build.
 *
 * @param excHandler  function called on a TLB exception
 * @return @p excHandler
 */
int func_0042CA80(int excHandler) {
    D_004564F0 = excHandler;
    SetVTLBRefillHandler(1, func_0042CFC0);
    SetVTLBRefillHandler(2, func_0042CFC0);
    SetVTLBRefillHandler(3, func_0042CFC0);
    return excHandler;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CAE0);
#ifdef SKIP_ASM
extern int D_004564F8[];
extern char D_0042D200[];
extern void SetVTLBRefillHandler(int, void (*)(void));
extern void SetVCommonHandler(int, void *);
/**
 * Installs a handler for one exception code, returning the previous one.
 *
 * Codes 1-13 only: records @p excHandler in D_004564F8[@p excCode] and points the kernel's vector for the code at the dispatch stub
 * D_0042D200 (SetVTLBRefillHandler for codes 1-3, SetVCommonHandler otherwise), which calls the recorded handler. No
 * caller in this build.
 *
 * @param excCode  exception code (1-13)
 * @param excHandler  the handler
 * @return the previous handler, or 0xFFFFFFFF for a code outside 1-13
 */
unsigned func_0042CAE0(int excCode, int excHandler) {
    int old;
    if (excCode - 1 >= 13U) {
        return 0xFFFFFFFF;
    }
    old = D_004564F8[excCode];
    D_004564F8[excCode] = excHandler;
    if (excCode - 1 < 3U) {
        SetVTLBRefillHandler(excCode, (void (*)(void))D_0042D200);
    } else {
        SetVCommonHandler(excCode, (void (*)(void))D_0042D200);
    }
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CB68);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * func_0042CBD0 installs the word-copy routine func_0042CB78 as syscall 90 and copies its kernel patch with this stub.
 */
SYSCALL_STUB(func_0042CB68, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CB78);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by func_0042CBD0 as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int func_0042CB78(int *dst, int *src, unsigned int size) {
    unsigned int n = size >> 2;
    unsigned int i;
    for (i = 0; i < n; i++) {
        *dst++ = *src++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CBB0);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 91 with the caller's argument (a syscall number).
 *
 * func_0042CBD0 first points syscall 91 at the patch it copied to 0x80075000 and then asks it, through this stub, for
 * handler addresses.
 */
SYSCALL_STUB(func_0042CBB0, 91)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CBC0);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by func_0042CBD0.
 */
SYSCALL_STUB(func_0042CBC0, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CBD0);
#ifdef SKIP_ASM
extern int D_00456540[];
extern char D_004561C0[];
extern void FlushCache(int);
extern int func_0042CBB0(int);
extern void func_0042CBC0(int, int);
extern void func_0042CB68(int, char *, int);
/**
 * Installs the TLB syscall patch and finds the kernel's exec-argument area.
 *
 * Installs func_0042CB78 as syscall 90 (first pair of D_00456540), copies 816 bytes of patch code from D_004561C0 to
 * kernel address 0x80075000, flushes the data and instruction caches (FlushCache 0 and 2), installs the patch's entry
 * as syscall 91 and the handler at 0x0042D100 as syscall 84, then installs, for each of the syscalls 85-89 (the TLB
 * entry and scratchpad calls), the handler address syscall 91 returns for it. Finally stores syscall 91's answer for 3
 * in D_00456538, the kernel area func_0042C510 writes exec arguments to. _InitSys jumps to it last.
 */
void func_0042CBD0(void) {
    unsigned i;
    int *syscallPair;
    func_0042CBC0(D_00456540[0], D_00456540[1]);
    func_0042CB68(0x80075000, D_004561C0, 816);
    FlushCache(0);
    FlushCache(2);
    func_0042CBC0(D_00456540[2], D_00456540[3]);
    func_0042CBC0(D_00456540[4], D_00456540[5]);
    syscallPair = &D_00456540[6];
    for (i = 3; i < 8; i++) {
        func_0042CBC0(syscallPair[0], func_0042CBB0(syscallPair[0]));
        syscallPair += 2;
    }
    D_00456538 = func_0042CBB0(3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", PutTLBEntry);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 85 (PutTLBEntry in the PS2 SDK's syscall table).
 *
 * Loads the number into $v1 and executes `syscall`; the kernel does the work. The stub declares no parameters; the
 * caller's arguments reach the kernel in the argument registers untouched.
 */
SYSCALL_STUB(PutTLBEntry, 85)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iPutTLBEntry);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall -85 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. A negative number selects the interrupt-context variant. No caller
 * in this build.
 */
SYSCALL_STUB(iPutTLBEntry, -85)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CCB8);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 86 with the caller's arguments (index, PageMask,
 * EntryHi, EntryLo0, EntryLo1).
 *
 * Loads the number into $v1 and executes `syscall`. Called by SetTLBEntry (index-checked) and the TLB set-up
 * func_0042CD98.
 */
SYSCALL_STUB(func_0042CCB8, 86)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", SetTLBEntry);
#ifdef SKIP_ASM
extern int func_0042CCB8(int a0);
/**
 * Writes a TLB entry when its index is one programs may use (13-47): forwards to syscall 86 (func_0042CCB8).
 *
 * Only the index is declared; the entry words (PageMask, EntryHi, EntryLo0, EntryLo1) pass through in their argument
 * registers. No caller in this build.
 *
 * @param tlbIndex  the TLB entry index
 * @return the syscall's result, or -1 for an index outside 13-47
 */
int SetTLBEntry(int tlbIndex) {
    if ((unsigned int)(tlbIndex - 13) >= 35)
        return -1;
    return func_0042CCB8(tlbIndex);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iSetTLBEntry);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall -86 (iSetTLBEntry in the PS2 SDK's syscall table).
 *
 * Loads the number into $v1 and executes `syscall`; the kernel does the work. The stub declares no parameters; the
 * caller's arguments reach the kernel in the argument registers untouched. A negative number selects the
 * interrupt-context (i-prefixed) variant.
 */
SYSCALL_STUB(iSetTLBEntry, -86)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", GetTLBEntry);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 87 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. No caller in this build.
 */
SYSCALL_STUB(GetTLBEntry, 87)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iGetTLBEntry);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall -87 (iGetTLBEntry in the PS2 SDK's syscall table).
 *
 * Loads the number into $v1 and executes `syscall`; the kernel does the work. The stub declares no parameters; the
 * caller's arguments reach the kernel in the argument registers untouched. A negative number selects the
 * interrupt-context (i-prefixed) variant.
 */
SYSCALL_STUB(iGetTLBEntry, -87)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", ProbeTLBEntry);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 88 (ProbeTLBEntry in the PS2 SDK's syscall table).
 *
 * Loads the number into $v1 and executes `syscall`; the kernel does the work. The stub declares no parameters; the
 * caller's arguments reach the kernel in the argument registers untouched.
 */
SYSCALL_STUB(ProbeTLBEntry, 88)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iProbeTLBEntry);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall -88 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. A negative number selects the interrupt-context variant. No caller
 * in this build.
 */
SYSCALL_STUB(iProbeTLBEntry, -88)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", ExpandScratchPad);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 89 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. No caller in this build.
 */
SYSCALL_STUB(ExpandScratchPad, 89)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD58);
#ifdef SKIP_ASM
extern int GetMemorySize(void);
extern int func_0042CD98(void);
extern int _InitTLB(void);
/**
 * Restores the TLB layout: on a 32 MB console (GetMemorySize() == 0x2000000) through func_0042CD98, otherwise through
 * the kernel's _InitTLB.
 *
 * func_0042C628 jumps here before the program exits or starts another one.
 *
 * @return the result of the call made
 */
int func_0042CD58(void) {
    if (GetMemorySize() == 0x2000000) {
        return func_0042CD98();
    }
    return _InitTLB();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "fakematch.h"
struct sTlbCfg_4248 { int a, b, c, cnt; int *t1; int *t2; int *t3; };
extern struct sTlbCfg_4248 D_004567F0;
extern char D_0049AD98[], D_0049ADD0[], D_0049ADE8[], D_0049AE00[];
extern int func_00425C28_pf(const char *, ...) __asm__("func_00425C28");
extern void Exit(int);
extern int func_0042CCB8_tlb(int, int, int, int, int) __asm__("func_0042CCB8");
/**
 * Programs the TLB from the layout table D_004567F0 and fills the remaining entries with unmapped pages.
 *
 * Prints the layout ("# TLB spad=0 kernel=1:%d default=%d:%d extended=%d:%d", func_00425C28) and clears the Wired
 * register. Writes the kernel entries, then the default entries (four words each: PageMask, EntryHi, EntryLo0,
 * EntryLo1) with syscall 86 (func_0042CCB8), records their total in the table's count word and makes them wired, then
 * writes the extended entries when there are any. Each group that would pass entry 48 prints "# TLB over flow (1)",
 * "(2)" or "(3)" and exits with status 1 (Exit). Entries left up to 47 get EntryHi 0xE0000000 + index * 0x2000
 * and no mapping. Called by func_0042CD58.
 *
 * @return the number of programmed entries shifted left by 13 (not the count itself)
 */
// FAKEMATCH: `register int i FM_REG("$25")` pins the TLB index to $25, where retail keeps it across the calls to the
// func_0042CCB8 syscall stub (no allocator picks a call-clobbered register there, so the original most likely declared
// it as a register variable); plain form 88.93%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int func_0042CD98(void) {
    struct sTlbCfg_4248 *tlbLayout = &D_004567F0;
    register int i FM_REG("$25");
    int n, k, *entryWords;
    func_00425C28_pf(D_0049AD98, tlbLayout->a - 1, tlbLayout->a, tlbLayout->a + tlbLayout->b - 1, tlbLayout->a + tlbLayout->b, tlbLayout->a + tlbLayout->b + tlbLayout->c - 1);
    MTC0_WIRED_4248(0);
    n = tlbLayout->a;
    i = 0;
    if (n > 48) {
        func_00425C28_pf(D_0049ADD0);
        Exit(1);
    }
    entryWords = tlbLayout->t1;
    for (; i < n; i++, entryWords += 4) func_0042CCB8_tlb(i, entryWords[0], entryWords[1], entryWords[2], entryWords[3]);
    tlbLayout = &D_004567F0;
    n = i + tlbLayout->b;
    if (n > 48) {
        func_00425C28_pf(D_0049ADE8);
        Exit(1);
    }
    entryWords = tlbLayout->t2;
    for (; i < n; i++, entryWords += 4) func_0042CCB8_tlb(i, entryWords[0], entryWords[1], entryWords[2], entryWords[3]);
    tlbLayout = &D_004567F0;
    tlbLayout->cnt = i;
    MTC0_WIRED_4248(i);
    if (tlbLayout->c > 0) {
        n = i + tlbLayout->c;
        if (n > 48) {
            func_00425C28_pf(D_0049AE00);
            Exit(1);
        }
        entryWords = tlbLayout->t3;
        for (; i < n; i++, entryWords += 4) func_0042CCB8_tlb(i, entryWords[0], entryWords[1], entryWords[2], entryWords[3]);
    }
    k = i;
    i <<= 13;
    if (k < 48) {
        int addr = i + 0xE0000000;
        do {
            func_0042CCB8_tlb(k, 0, addr, 0, 0);
            k++;
            addr += 0x2000;
        } while (k < 48);
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CFC0);
#ifdef SKIP_ASM
/**
 * EE exception handler stub for the TLB exceptions func_0042CA80 hooks (SetVTLBRefillHandler, codes 1-3).
 *
 * Saves every GPR, HI/LO, HI1/LO1 and SA in the frame D_00530380, loads Status, Cause, EPC and BadVAddr into $4-$7
 * (the frame in $8), keeps EPC in D_005305A8 and erets with IE cleared to D_0042D0C0. That calls the user handler
 * D_004564F0 on the frame as stack, makes syscall -0x54, restores the saved registers and EPC and returns from the
 * exception with eret. The second entry D_0042D200, the stub func_0042CAE0 installs for codes 1-13, saves the same
 * way (COP0 register 23 also in $8, the frame in $9), erets to D_0042D2FC and calls the handler D_004564F8[ExcCode];
 * if that returns it stops at a break.
 */
// PORT: PS2-only EE exception handler (eret, raw $at/$k0, never returns); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_0042CFC0\n.type func_0042CFC0, @function\nfunc_0042CFC0:\n"
    "lui $26, %hi(D_00530380)\n"
    "addiu $26, $26, %lo(D_00530380)\n"
    "sq $1, 0x10($26)\n"
    "sq $2, 0x20($26)\n"
    "sq $3, 0x30($26)\n"
    "sq $4, 0x40($26)\n"
    "sq $5, 0x50($26)\n"
    "sq $6, 0x60($26)\n"
    "sq $7, 0x70($26)\n"
    "sq $8, 0x80($26)\n"
    "sq $9, 0x90($26)\n"
    "sq $10, 0xA0($26)\n"
    "sq $11, 0xB0($26)\n"
    "sq $12, 0xC0($26)\n"
    "sq $13, 0xD0($26)\n"
    "sq $14, 0xE0($26)\n"
    "sq $15, 0xF0($26)\n"
    "sq $16, 0x100($26)\n"
    "sq $17, 0x110($26)\n"
    "sq $18, 0x120($26)\n"
    "sq $19, 0x130($26)\n"
    "sq $20, 0x140($26)\n"
    "sq $21, 0x150($26)\n"
    "sq $22, 0x160($26)\n"
    "sq $23, 0x170($26)\n"
    "sq $24, 0x180($26)\n"
    "sq $25, 0x190($26)\n"
    "sq $28, 0x1C0($26)\n"
    "sq $29, 0x1D0($26)\n"
    "sq $30, 0x1E0($26)\n"
    "sq $31, 0x1F0($26)\n"
    "mfhi $2\n"
    "lui $1, %hi(D_00530580)\n"
    "sd $2, %lo(D_00530580)($1)\n"
    "mfhi1 $2\n"
    "lui $1, %hi(D_00530588)\n"
    "sd $2, %lo(D_00530588)($1)\n"
    "mflo $2\n"
    "lui $1, %hi(D_00530590)\n"
    "sd $2, %lo(D_00530590)($1)\n"
    "mflo1 $2\n"
    "lui $1, %hi(D_00530598)\n"
    "sd $2, %lo(D_00530598)($1)\n"
    "mfsa $2\n"
    "lui $1, %hi(D_005305A0)\n"
    "sd $2, %lo(D_005305A0)($1)\n"
    "mfc0 $4, $12\n"
    "mfc0 $5, $13\n"
    "mfc0 $6, $14\n"
    "mfc0 $7, $8\n"
    "lui $8, %hi(D_00530380)\n"
    "addiu $8, $8, %lo(D_00530380)\n"
    "lui $1, %hi(D_005305A8)\n"
    "sw $6, %lo(D_005305A8)($1)\n"
    "lui $1, %hi(D_0042D0C0)\n"
    "addiu $1, $1, %lo(D_0042D0C0)\n"
    "mtc0 $1, $14\n"
    "sync.p\n"
    "mfc0 $1, $12\n"
    "addiu $2, $0, -0x2\n"
    "and $1, $1, $2\n"
    "mtc0 $1, $12\n"
    "sync.p\n"
    "eret\n"
    ".globl D_0042D0C0\n"
    "D_0042D0C0:\n"
    "lui $1, %hi(D_004564F0)\n"
    "lw $1, %lo(D_004564F0)($1)\n"
    "lui $29, %hi(D_00530380)\n"
    "jalr $1\n"
    "addiu $29, $29, %lo(D_00530380)\n"
    "addiu $3, $0, -0x54\n"
    "syscall 0\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "mfc0 $1, $12\n"
    "addiu $26, $0, -0x1C\n"
    "and $1, $1, $26\n"
    "mtc0 $1, $12\n"
    "sync.p\n"
    "lui $2, %hi(D_005305A8)\n"
    "lw $2, %lo(D_005305A8)($2)\n"
    "mtc0 $2, $14\n"
    "sync.p\n"
    "lui $2, %hi(D_00530580)\n"
    "ld $2, %lo(D_00530580)($2)\n"
    "mthi $2\n"
    "lui $2, %hi(D_00530588)\n"
    "ld $2, %lo(D_00530588)($2)\n"
    "mthi1 $2\n"
    "lui $2, %hi(D_00530590)\n"
    "ld $2, %lo(D_00530590)($2)\n"
    "mtlo $2\n"
    "lui $2, %hi(D_00530598)\n"
    "ld $2, %lo(D_00530598)($2)\n"
    "mtlo1 $2\n"
    "lui $2, %hi(D_005305A0)\n"
    "ld $2, %lo(D_005305A0)($2)\n"
    "mtsa $2\n"
    "sync.p\n"
    "lui $26, %hi(D_00530380)\n"
    "addiu $26, $26, %lo(D_00530380)\n"
    "lq $1, 0x10($26)\n"
    "lq $2, 0x20($26)\n"
    "lq $3, 0x30($26)\n"
    "lq $4, 0x40($26)\n"
    "lq $5, 0x50($26)\n"
    "lq $6, 0x60($26)\n"
    "lq $7, 0x70($26)\n"
    "lq $8, 0x80($26)\n"
    "lq $9, 0x90($26)\n"
    "lq $10, 0xA0($26)\n"
    "lq $11, 0xB0($26)\n"
    "lq $12, 0xC0($26)\n"
    "lq $13, 0xD0($26)\n"
    "lq $14, 0xE0($26)\n"
    "lq $15, 0xF0($26)\n"
    "lq $16, 0x100($26)\n"
    "lq $17, 0x110($26)\n"
    "lq $18, 0x120($26)\n"
    "lq $19, 0x130($26)\n"
    "lq $20, 0x140($26)\n"
    "lq $21, 0x150($26)\n"
    "lq $22, 0x160($26)\n"
    "lq $23, 0x170($26)\n"
    "lq $24, 0x180($26)\n"
    "lq $25, 0x190($26)\n"
    "lq $28, 0x1C0($26)\n"
    "lq $29, 0x1D0($26)\n"
    "lq $30, 0x1E0($26)\n"
    "lq $31, 0x1F0($26)\n"
    "mfc0 $26, $12\n"
    "ori $26, $26, 0x13\n"
    "mtc0 $26, $12\n"
    "sync.p\n"
    "eret\n"
    "nop\n"
    "nop\n"
    "nop\n"
    ".globl D_0042D200\n"
    "D_0042D200:\n"
    "lui $26, %hi(D_00530380)\n"
    "addiu $26, $26, %lo(D_00530380)\n"
    "sq $1, 0x10($26)\n"
    "sq $2, 0x20($26)\n"
    "sq $3, 0x30($26)\n"
    "sq $4, 0x40($26)\n"
    "sq $5, 0x50($26)\n"
    "sq $6, 0x60($26)\n"
    "sq $7, 0x70($26)\n"
    "sq $8, 0x80($26)\n"
    "sq $9, 0x90($26)\n"
    "sq $10, 0xA0($26)\n"
    "sq $11, 0xB0($26)\n"
    "sq $12, 0xC0($26)\n"
    "sq $13, 0xD0($26)\n"
    "sq $14, 0xE0($26)\n"
    "sq $15, 0xF0($26)\n"
    "sq $16, 0x100($26)\n"
    "sq $17, 0x110($26)\n"
    "sq $18, 0x120($26)\n"
    "sq $19, 0x130($26)\n"
    "sq $20, 0x140($26)\n"
    "sq $21, 0x150($26)\n"
    "sq $22, 0x160($26)\n"
    "sq $23, 0x170($26)\n"
    "sq $24, 0x180($26)\n"
    "sq $25, 0x190($26)\n"
    "sq $28, 0x1C0($26)\n"
    "sq $29, 0x1D0($26)\n"
    "sq $30, 0x1E0($26)\n"
    "sq $31, 0x1F0($26)\n"
    "mfhi $2\n"
    "lui $1, %hi(D_00530580)\n"
    "sd $2, %lo(D_00530580)($1)\n"
    "mfhi1 $2\n"
    "lui $1, %hi(D_00530588)\n"
    "sd $2, %lo(D_00530588)($1)\n"
    "mflo $2\n"
    "lui $1, %hi(D_00530590)\n"
    "sd $2, %lo(D_00530590)($1)\n"
    "mflo1 $2\n"
    "lui $1, %hi(D_00530598)\n"
    "sd $2, %lo(D_00530598)($1)\n"
    "mfsa $2\n"
    "lui $1, %hi(D_005305A0)\n"
    "sd $2, %lo(D_005305A0)($1)\n"
    "mfc0 $4, $12\n"
    "mfc0 $5, $13\n"
    "mfc0 $6, $14\n"
    "mfc0 $7, $8\n"
    "mfc0 $8, $23\n"
    "lui $9, %hi(D_00530380)\n"
    "addiu $9, $9, %lo(D_00530380)\n"
    "lui $1, %hi(D_0042D2FC)\n"
    "addiu $1, $1, %lo(D_0042D2FC)\n"
    "mtc0 $1, $14\n"
    "sync.p\n"
    "mfc0 $1, $12\n"
    "addiu $2, $0, -0x2\n"
    "and $1, $1, $2\n"
    "mtc0 $1, $12\n"
    "sync.p\n"
    "eret\n"
    ".globl D_0042D2FC\n"
    "D_0042D2FC:\n"
    "andi $2, $5, 0x7C\n"
    "lui $1, %hi(D_004564F8)\n"
    "addu $1, $1, $2\n"
    "lw $1, %lo(D_004564F8)($1)\n"
    "lui $29, %hi(D_00530380)\n"
    "jalr $1\n"
    "addiu $29, $29, %lo(D_00530380)\n"
    "break 1023, 1023\n"
    ".size func_0042CFC0, . - func_0042CFC0\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("seg/seg_325220", __fixdfdi);
#ifdef SKIP_ASM
extern long dpcmp(double, double);
extern double dpsub(double, double);
extern long __fixunsdfdi(double);
/**
 * Converts a double to a signed 64-bit integer (libgcc helper).
 *
 * Converts the magnitude with __fixunsdfdi (the unsigned conversion) and negates it for negative inputs.
 *
 * @param value  value to convert
 * @return the value truncated toward zero
 */
long __fixdfdi(double value) {
    if (dpcmp(value, 0.0) < 0)
        return -__fixunsdfdi(dpsub(0.0, value));
    return __fixunsdfdi(value);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", __fixunsdfdi);
#ifdef SKIP_ASM
/**
 * Converts a double to an unsigned 64-bit integer (libgcc helper).
 *
 * Negative values give 0. The high word is the value times 2^-32 truncated; the low word is what remains after
 * subtracting it (taken away when that remainder is negative).
 *
 * @param value  value to convert
 * @return the value truncated toward zero
 */
long __fixunsdfdi(double value) {
    unsigned long hi;
    if (value < 0.0) {
        return 0;
    }
    hi = (unsigned long)(unsigned)(value * 2.3283064365386963e-10) << 32;
    value = value - (double)hi;
    if (value < 0.0) {
        hi -= (unsigned)(0.0 - value);
    } else {
        hi += (unsigned)value;
    }
    return hi;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", __moddi3);
#ifdef SKIP_ASM
/* GNU CC runtime support, reconstructed: libgcc2.c __moddi3 with longlong.h. Original: Copyright (C) 1989, 92-98, 1999
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

/* libgcc2.c __moddi3 with its static inline __negdi2 and __udivmoddi4 (GCC runtime, GPL v2+ with the libgcc linking exception) */
/* The L_moddi3 object's own copy of libgcc2.c's `static const UQItype __clz_tab[]` (its rodata stays in asm). */
extern const UQItype __clz_tab[] __asm__("D_0049AE18");

/* (L_moddi3: static inline) */
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

/* (L_moddi3: static inline; UDIV_NEEDS_NORMALIZATION is 1, so the normalizing d1 == 0 branch) */
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

DItype __moddi3 (DItype u, DItype v) __asm__("__moddi3");

UDItype __udivmoddi4 ();
/**
 * Returns the remainder of a signed 64-bit division (libgcc helper).
 *
 * Divides the magnitudes with the inline unsigned divide (__udivmoddi4) and gives the remainder the dividend's sign.
 * The printf core func_00425638 calls it.
 *
 * @param dividend  number divided
 * @param divisor  number to divide by
 * @return dividend % divisor, with the sign of the dividend
 */
DItype
__moddi3 (DItype dividend, DItype divisor)
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
    vv.ll = __negdi2 (vv.ll);

  (void) __udivmoddi4 (uu.ll, vv.ll, &w);
  if (negate)
    w = __negdi2 (w);

  return w;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", __umoddi3);
#ifdef SKIP_ASM
/* GNU CC runtime support, reconstructed: libgcc2.c __umoddi3 with longlong.h. Original: Copyright (C) 1989, 92-98, 1999
   Free Software Foundation, Inc. Free software under the GNU General Public License, version 2 or (at your option) any
   later version, with this special exception: "if you link this library with other files, some of which are compiled
   with GCC, to produce an executable, this library does not by itself cause the resulting executable to be covered
   by the GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License." This block (declarations and function) stays
   under those terms; the repository's CC0 dedication does not apply to it (README, License). */
/* libgcc2.c __umoddi3 with its static inline __udivmoddi4 (GCC runtime, GPL v2+ with the libgcc linking exception) */
/* The L_umoddi3 object's own copies of libgcc2.c's static __clz_tab and static inline __udivmoddi4. This unit also
   holds the L_moddi3 object's copies under the source names, so these two are renamed here. */
#define __clz_tab __clz_tab_umoddi3
#define __udivmoddi4 __udivmoddi4_umoddi3
extern const UQItype __clz_tab[] __asm__("D_0049AF18");

/* (L_umoddi3: static inline; UDIV_NEEDS_NORMALIZATION is 1, so the normalizing d1 == 0 branch) */
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

UDItype __umoddi3 (UDItype u, UDItype v) __asm__("__umoddi3");

UDItype __udivmoddi4 ();
/**
 * Returns the remainder of an unsigned 64-bit division (libgcc helper).
 *
 * Uses the inline unsigned divide (__udivmoddi4). Called by _strtol_r, the printf core func_00425638 and other library
 * code.
 *
 * @param dividend  number divided
 * @param divisor  number to divide by
 * @return dividend % divisor
 */
UDItype
__umoddi3 (UDItype dividend, UDItype divisor)
{
  UDItype w;

  (void) __udivmoddi4 (dividend, divisor, &w);

  return w;
}

#undef __udivmoddi4
#undef __clz_tab
#endif
