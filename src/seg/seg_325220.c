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
extern sSifClient fileio_client;   /* fileio */
extern sSifClient libkernl_iopHeapClient;
extern sSifClient sifload_client;


/* 4-byte server version tags returned by the fileio and loadfile RPC servers */
typedef struct { char b[4]; } sRpcVer;
extern sRpcVer fileio_serverVersion;      /* fileio (follows fileio_client) */
extern sRpcVer sifload_serverVersion;      /* loadfile (follows sifload_client) */


/* The size of the name and argument buffers of a loadfile or IOP heap request ([U] the senders strncpy a name into
   IOP_NAME_SIZE bytes and force the last to NUL; sifload_callFileRequest copies at most IOP_NAME_SIZE argument bytes).
   Name ours. */
#define IOP_NAME_SIZE 0xFC

/* loadfile RPC (0x80000006) command block sifload_cmd, 0x200 bytes, sent to sifload_client; each request's reply is read
   back into it. One view per request kind; field names ours, from the senders. */
typedef struct { char b[IOP_NAME_SIZE]; } sLfData;
/* The module requests: a module id or IOP address, the argument length, a name and the arguments. */
typedef struct { int id; int argLen; char name[IOP_NAME_SIZE]; char args[IOP_NAME_SIZE]; } sLfCmd;
/* sifload_callFileRequest's load request: the argument length first, then the path and the arguments. */
typedef struct { int argLen; int spare; char path[IOP_NAME_SIZE]; char args[IOP_NAME_SIZE]; } sLfLoadCmd;
/* sifload_callElfRequest's ELF request: the path and the section name; the reply holds the entry point and gp. */
typedef struct { int entry; int gp; char path[IOP_NAME_SIZE]; char section[IOP_NAME_SIZE]; } sLfElfCmd;
/* The module requests' reply: the module id (or the word asked for) and the module's result. */
typedef struct { int id; int result; } sLfReply;
/* The value size of the loader's IOP memory get / set requests ([U] func_0042BBF8 and func_0042BCE8 move a byte,
   halfword or word for 0, 1 and 2; above 2 is refused). Names ours. */
enum eIopValueSize { IOP_VALUE_BYTE = 0, IOP_VALUE_HALFWORD = 1, IOP_VALUE_WORD = 2 };
/* get: the IOP address in, the value out; set: the address and the value. size is an eIopValueSize. */
typedef struct { union { unsigned char b; unsigned short h; int w; } data; int size; } sLfGetCmd;        /* 3 */
typedef struct {
    int addr; int size; union { unsigned char b; unsigned short h; unsigned w; } value;
} sLfSetCmd; /* 2 */
typedef union {
    sLfCmd cmd;                 /* 6..10 and the name calls */
    sLfLoadCmd load;            /* sifload_callFileRequest */
    sLfElfCmd elf;              /* sifload_callElfRequest */
    sLfReply reply;             /* the module requests' reply */
    sLfGetCmd get;
    sLfSetCmd set;
    sRpcVer ver;                /* 0xFF reply */
} uLfCmd;
extern uLfCmd sifload_cmd;


/* fileio descriptor table: 32 entries of {IOP fd, open flags (0 = free), ...} */
typedef struct { int f0; int f4; int f8; int fC; } sFioFile;
extern sFioFile fileio_descriptors[32];
extern sFioFile *fileio_allocDescriptor(void);
extern sFioFile *fileio_getDescriptor(unsigned fd);


/* fileio RPC buffers */
typedef struct { void (*fn)(int); int arg; } sFioCb;
extern sFioCb fileio_userCallback;      /* user callback run by fileio_onCallbackCmd (set by fileio_setCallback) */
extern char fileio_recvBuffer[];      /* RPC receive buffer */
extern char D_0052E380[];      /* 0x440-byte read-ahead buffers */
extern char D_0052EE40[];
extern int D_0052D6C0[];


/* byte ring buffer (header + data, wraps at size + 16) */
/* count is shared with the RPC callback that fills the ring (volatile in every user) */
typedef struct { int size; volatile int count; char *rd; char *wr; char data[1]; } sRing;
/* serial console state; fd, wlen, pos and fC are shared with the RPC callback libkernl_onTtyDeci2Event */
typedef struct {
    volatile int fd;
    volatile int wlen;
    volatile int pos;
    volatile int fC;
    char *wp;
    unsigned char *buf;
    sRing *ring;
} sTty;
extern sTty libkernl_ttyState;
extern int libkernl_ttyRingStorage[];       /* storage of the console ring (libkernl_initTtyRing) */
extern sRing *libkernl_initTtyRing(int size);


/* SIF RPC library state sifrpc_state (implementation side) */
/* 0x40-byte packet: the SIF command header, then slot (table index << 16 | state bits, bit 0 in use), pkt (its own
   address) and rpc_id (the packet id). Names ours, as in sceSifCallRpc's view. */
typedef struct { char header[0x10]; int slot; int pkt; int rpc_id; char rest[0x24]; } sSifRpcPkt;
typedef struct sSifServer { int key; char pad[0x34]; struct sSifServer *next; } sSifServer;
/* An RPC server queue: the serving thread, the active flag (set while sceSifGetNextRequest hands out requests), its
   servers and the pending request list (head, tail). Names ours. */
typedef struct sSifQueue {
    int thread_id; int active; sSifServer *servers; int head; int tail; struct sSifQueue *next;
} sSifQueue;
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
extern sSifRpcState sifrpc_state;


/* wake-up request queue of the helper thread started by InitThread (512 {type, thread id} entries) */
/* An entry's request type ([U] iWakeupThread, iRotateThreadReadyQueue and iSuspendThread queue 0, 1 and 2; the helper
   thread libkernl_requestThread calls WakeupThread, RotateThreadReadyQueue and SuspendThread for them). Names ours. */
enum eWakeRequest { WAKE_REQUEST_WAKEUP = 0, WAKE_REQUEST_ROTATE_READY_QUEUE = 1, WAKE_REQUEST_SUSPEND = 2 };
typedef struct {
    int readIdx;                /* the helper thread's read index (name ours) */
    unsigned idx;               /* the write index */
    struct { char a; char b; } e[512];
} sWakeQueue;
extern sWakeQueue libkernl_requestQueue;


/* SIF DMA transfer descriptor (as passed to sceSifSetDma / isceSifSetDma) */
typedef struct { int data; int addr; int size; int attr; } sSifDmaData;
extern int sceSifSetDma(sSifDmaData *sdd, int n);
extern int isceSifSetDma(sSifDmaData *sdd, int n);


/* A semaphore id before CreateSema has made one ([U] the call lock fileio_callLock and the table lock fileio_tableSema start as
   -1; fileio_createCallLock and fileio_createSemas create them then). Name ours. */
#define NO_SEMAPHORE (-1)
/* A free entry of the pending no-wait call table D_00455188 ([U] sceRead, sceWrite, sceLseek and sceLseek64 claim an
   entry that holds -1; fileio_onCompletionCmd sets a finished call's entry back to -1). Name ours. */
#define FIO_NOWAIT_FREE (-1)

/* library state used by more than one function */
extern int libkernl_ttyOpenFlag;
extern int libkernl_helperThreadId;          /* helper thread id (InitThread) */
extern int libkernl_requestSema;          /* its wake-up semaphore */
extern int sifcmd_data[];
extern int sifcmd_sregs[];
extern volatile int D_00455188[];
extern int fileio_initFlag;          /* fileio initialised (fileio_init) */
extern int D_00455210;
extern int fileio_callLock;
extern int fileio_tableSema;          /* fileio descriptor table semaphore */
extern int fileio_noWaitSema;
extern int libkernl_iopHeapBindState;
extern int libkernl_iopHeapResult;          /* RPC result for libkernl_iopHeapClient */
extern int libkernl_iopHeapArgs;          /* RPC argument for libkernl_iopHeapClient */
extern int sifload_bindState;          /* loadfile bound (< 0 = not yet) */
extern int sysTable_base;
extern int execArgs_kernelArea;


/* The size of every name and data buffer in a fileio request packet ([U] the sce* calls copy a name up to
   FIO_BUFFER_SIZE - 1 characters and NUL-terminate the last byte; sceDevctl, sceIoctl2, sceFormat and sceMount refuse
   longer data; fileio_onCompletionCmd caps a reply's data there). Name ours. */
#define FIO_BUFFER_SIZE 0x400

/* The fileio RPC function numbers, named after the sce* call that sends each ([U] its sceSifCallRpc to fileio_client, or
   the number it passes to fileio_callPathRequest). 15 and 16 come from func_00429420 and func_00429540, which have no retail
   names. Names ours. */
enum eFioRpcFunc {
    FIO_RPC_OPEN = 0, FIO_RPC_CLOSE = 1, FIO_RPC_READ = 2, FIO_RPC_WRITE = 3, FIO_RPC_LSEEK = 4, FIO_RPC_IOCTL = 5,
    FIO_RPC_REMOVE = 6, FIO_RPC_MKDIR = 7, FIO_RPC_RMDIR = 8, FIO_RPC_DOPEN = 9, FIO_RPC_DCLOSE = 10,
    FIO_RPC_DREAD = 11, FIO_RPC_GETSTAT = 12, FIO_RPC_CHSTAT = 13, FIO_RPC_FORMAT = 14, FIO_RPC_RENAME = 17,
    FIO_RPC_CHDIR = 18, FIO_RPC_SYNC = 19, FIO_RPC_MOUNT = 20, FIO_RPC_UMOUNT = 21, FIO_RPC_LSEEK64 = 22,
    FIO_RPC_DEVCTL = 23, FIO_RPC_SYMLINK = 24, FIO_RPC_READLINK = 25, FIO_RPC_IOCTL2 = 26
};

/* fileio RPC send block fileio_cmd: one packet layout per RPC function number (sent to fileio_client). Every packet
   starts with the same three words: the caller's semaphore (negated for a no-wait call once its D_00455188 entry is
   taken), the address of its result word and the result size; fileio_onCompletionCmd reads them back from the reply. iopFd is
   the IOP's descriptor from the slot, slot the slot's index in fileio_descriptors. Field names ours, from the senders. */
typedef struct { int sema; int resultAddr; int resultSize; int driverArg; } sFioCmd4;                     /* 15 */
typedef struct { int sema; int resultAddr; int resultSize; int iopFd; int slot; } sFioFdCmd;              /* 1, 10 */
typedef struct { int sema; int resultAddr; int resultSize; int iopFd; int direntBuf; } sFioDreadCmd;      /* 11 */
typedef struct {
    int sema; int resultAddr; int resultSize; int iopFd; int offset; int whence; int slot;
} sFioLseekCmd; /* 4 */
typedef struct {
    int sema; int resultAddr; int resultSize; int iopFd; int buf; int size;
    int spare;                  /* not written by sceRead */
    int slot;
} sFioReadCmd; /* 2 */
typedef struct {
    int sema; int resultAddr; int resultSize; int iopFd; unsigned buf; int size;
    int dataLen;                /* the unaligned head bytes of buf copied into data */
    char data[16];
    int slot;
} sFioWriteCmd; /* 3 */
typedef struct {
    int sema; int resultAddr; int resultSize; int iopFd; long offset; int whence; int slot;
} sFioLseek64Cmd; /* 22 */
typedef struct {
    int sema; int resultAddr; int resultSize; char name[FIO_BUFFER_SIZE];
} sFioPathCmd; /* 6, 8, 9, 16, 18, 21 */
typedef struct {
    int sema; int resultAddr; int resultSize; int mode; char name[FIO_BUFFER_SIZE];
} sFioPathModeCmd; /* 7 */
typedef struct {
    int sema; int resultAddr; int resultSize; int statBuf; char name[FIO_BUFFER_SIZE];
} sFioGetstatCmd; /* 12 */
typedef struct {
    int sema; int resultAddr; int resultSize;
    int spare;                  /* not written by sceSync */
    int flag; char name[FIO_BUFFER_SIZE];
} sFioSyncCmd; /* 19 */
typedef struct {
    int sema; int resultAddr; int resultSize; int bufLen; int buf; char name[FIO_BUFFER_SIZE];
} sFioReadlinkCmd; /* 25 */
typedef struct {
    int sema; int resultAddr; int resultSize; int flags; int mode; char name[FIO_BUFFER_SIZE]; int slot;
} sFioOpenCmd; /* 0 */
typedef struct {
    int sema; int resultAddr; int resultSize; char name[FIO_BUFFER_SIZE]; char name2[FIO_BUFFER_SIZE];
} sFioPath2Cmd; /* 17, 24 */
typedef struct {
    int sema; int resultAddr; int resultSize; char name[FIO_BUFFER_SIZE]; char name2[FIO_BUFFER_SIZE];
    char data[FIO_BUFFER_SIZE];
    int dataLen;
} sFioFormatCmd; /* 14 */
typedef struct {
    int sema; int resultAddr; int resultSize; char name[FIO_BUFFER_SIZE]; char name2[FIO_BUFFER_SIZE];
    char data[FIO_BUFFER_SIZE];
    int flag; int dataLen;
} sFioMountCmd; /* 20 */
typedef struct { int w[16]; } sFioStat;
typedef struct {
    int sema; int resultAddr; int resultSize; int changeMask; sFioStat v; char name[FIO_BUFFER_SIZE];
} sFioChstatCmd; /* 13 */
typedef struct {
    int sema; int resultAddr; int resultSize; char name[FIO_BUFFER_SIZE]; char data[FIO_BUFFER_SIZE];
    int command; int dataLen; int outBuf; int outLen;
} sFioDevctlCmd; /* 23 */
/* The same three words, the descriptor, the request, the output buffer and lengths and the input data; left with
   offset names because sceIoctl's held form spells them. */
typedef struct {
    int f0; int f4; int f8; int fC; int f10; char data[FIO_BUFFER_SIZE]; int f414; int f418; int f41C;
} sFioIoctl2Cmd; /* 26 */
typedef union {
    sFioOpenCmd open;           /* 0 */
    sFioFdCmd close;            /* 1 */
    sFioReadCmd read;           /* 2 */
    sFioWriteCmd write;         /* 3 */
    sFioLseekCmd lseek;         /* 4 */
    sFioPathCmd path;           /* 6, 8, 9, 16, 18, 21 (fileio_callPathRequest takes the number) */
    sFioPathModeCmd mkdir;      /* 7 */
    sFioFormatCmd format;       /* 14 */
    sFioCmd4 adddrv;            /* 15 */
    sFioFdCmd dclose;           /* 10 */
    sFioDreadCmd dread;         /* 11 */
    sFioGetstatCmd getstat;     /* 12 */
    sFioChstatCmd chstat;       /* 13 */
    sFioPath2Cmd rename;        /* 17 */
    sFioSyncCmd sync;           /* 19 */
    sFioMountCmd mount;         /* 20 */
    sFioLseek64Cmd lseek64;     /* 22 */
    sFioDevctlCmd devctl;       /* 23 */
    sFioPath2Cmd symlink;       /* 24 */
    sFioReadlinkCmd readlink;   /* 25 */
    sFioIoctl2Cmd ioctl2;       /* 26 */
} uFioCmd;
extern uFioCmd fileio_cmd;

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_closeTtyChannel);
#ifdef SKIP_ASM
/**
 * Marks the DECI2 console channel as closed so the next console read or write reopens it.
 *
 * Clears libkernl_ttyOpenFlag, the flag _write and _read set after libkernl_openTtyChannel opened the channel. sceSifSyncIop
 * calls it after the IOP has been rebooted.
 */
void libkernl_closeTtyChannel(void) {
    libkernl_ttyOpenFlag = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_waitVblankStart);
#ifdef SKIP_ASM
extern int DIntr_r(void) __asm__("DIntr");
extern void EIntr(void);
/**
 * Waits for the start of the next vertical blank by polling the interrupt controller.
 *
 * Clears the VBLANK-start bit (bit 2) of the interrupt status register 0x1000F000 with interrupts briefly off (DIntr,
 * EIntr when they were on), polls until it is set again, then clears it once more. No code calls it.
 */
void libkernl_waitVblankStart(void) {
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
INCLUDE_ASM("seg/seg_325220", libkernl_waitVblankGetCsr);
#ifdef SKIP_ASM
extern void EIntr(void);
extern void SetVSyncFlag_vs(volatile int *, volatile unsigned long *) __asm__("SetVSyncFlag");
/**
 * Waits for the start of the next vertical blank and returns the GS CSR value the kernel recorded.
 *
 * Registers a local flag and a CSR slot with SetVSyncFlag, clears bit 2 of 0x1000F000 as libkernl_waitVblankStart does, and polls
 * until the bit is set again or the kernel has set the flag; then clears the bit and returns the CSR slot. No code
 * calls it.
 *
 * @return the GS CSR value stored by the kernel's vsync handler
 */
unsigned long libkernl_waitVblankGetCsr(void) {
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
extern int libkernl_openTtyChannel(void);
extern int libkernl_ttySend(int, int);
/**
 * Writes bytes to the console: descriptors 1 and 2 (STDOUT_FILENO, STDERR_FILENO) go to the DECI2 TTY channel; returns
 * the count or -1.
 *
 * Opens the channel with libkernl_openTtyChannel on first use (flag libkernl_ttyOpenFlag) and sends with libkernl_ttySend. Other descriptors,
 * or a channel that cannot be opened, return -1 without setting errno. _write_r and the console buffers
 * func_0041B550 and func_0041C938 call it.
 *
 * @param fileDesc  the descriptor
 * @param buffer  the bytes
 * @param byteCount  the byte count
 * @return the bytes taken, or -1
 */
int _write(int fileDesc, int buffer, int byteCount) {
    if (fileDesc == STDOUT_FILENO || fileDesc == STDERR_FILENO) {
        if (libkernl_ttyOpenFlag == 0) {
            if (libkernl_openTtyChannel() == 0) {
                return -1;
            }
            libkernl_ttyOpenFlag = 1;
        }
        return libkernl_ttySend(buffer, byteCount);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _read);
#ifdef SKIP_ASM
extern int libkernl_openTtyChannel(void);
extern int libkernl_ttyReadLine(int, int);
/**
 * Reads console input for descriptor 0 from the DECI2 TTY channel and returns the count or -1.
 *
 * Opens the channel with libkernl_openTtyChannel on first use (flag libkernl_ttyOpenFlag) and reads with libkernl_ttyReadLine, which waits for
 * input and stops after a newline. Other descriptors return -1. _read_r calls it.
 *
 * @param fileDesc  the descriptor
 * @param buffer  the destination
 * @param byteCount  the maximum byte count
 * @return the bytes read, or -1
 */
int _read(int fileDesc, int buffer, int byteCount) {
    if (fileDesc == 0) {
        if (libkernl_ttyOpenFlag == 0) {
            if (libkernl_openTtyChannel() == 0) {
                return -1;
            }
            libkernl_ttyOpenFlag = 1;
        }
        return libkernl_ttyReadLine(buffer, byteCount);
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
extern unsigned libkernl_heapBreak;
extern unsigned EndOfHeap(void);
extern int *__errno(void);
/**
 * Moves the C heap break by n bytes and returns the old break, or all ones when it would pass the end of the heap.
 *
 * The break is libkernl_heapBreak; the limit comes from EndOfHeap. When interrupts are enabled (Status bit 16) they are
 * disabled for the update (di until the bit reads clear) and enabled again afterwards. Failure sets errno 12 (ENOMEM)
 * through __errno. _sbrk_r calls it.
 *
 * @param n  the byte change
 * @return the previous break, or 0xFFFFFFFF
 */
unsigned _sbrk(unsigned n) {
    int intsOn, status;
    unsigned end;
    MFC0_STATUS_4248(intsOn);
    intsOn &= 0x10000;
    if (intsOn) {
        do {
            DI_4248();
            SYNCP_4248();
            MFC0_STATUS_4248(status);
        } while (status & 0x10000);
    }
    end = libkernl_heapBreak + n;
    if (EndOfHeap() < end) {
        *__errno() = 12;
        if (intsOn) EI_4248();
        return 0xFFFFFFFF;
    }
    {
        unsigned old = libkernl_heapBreak;
        libkernl_heapBreak = end;
        if (intsOn) EI_4248();
        return old;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", isatty);
#ifdef SKIP_ASM
/**
 * Returns 1: every descriptor counts as a terminal.
 *
 * @return 1
 */
int isatty(void) {
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
INCLUDE_ASM("seg/seg_325220", _getpid);
#ifdef SKIP_ASM
/**
 * Returns 1 (libgloss's __MYPID), the only process id.
 *
 * @return __MYPID (1)
 */
int _getpid(void) {
    return __MYPID;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _kill);
#ifdef SKIP_ASM
extern void Exit(int a0);
/**
 * Ends the program with Exit(sigNum) when pid is 1 (__MYPID); for any other pid it does nothing.
 *
 * @param pid     the process id
 * @param sigNum  the signal number, used as the exit status
 * @return 0 (only reached for other pids)
 */
int _kill(int pid, int sigNum) {
    if (pid == __MYPID) {
        Exit(sigNum);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _stat);
#ifdef SKIP_ASM
extern int *__errno(void);
/**
 * Fails an unsupported file operation: sets errno 5 (EIO) and returns -1.
 *
 * Declared without parameters; errno is set through __errno.
 *
 * @return -1
 */
int _stat(void) {
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
INCLUDE_ASM("seg/seg_325220", libkernl_writebackDCacheRange);
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
void libkernl_writebackDCacheRange(unsigned lo, unsigned hi) {
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
extern void libkernl_writebackDCacheRange(unsigned, unsigned);
/**
 * Writes back and invalidates the data cache for an address range, with interrupts held off.
 *
 * Aligns both bounds down to 64 bytes and calls libkernl_writebackDCacheRange, between DIntr and EIntr when interrupts were enabled
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
    libkernl_writebackDCacheRange(startAddr & ~63, endAddr & ~63);
    if (intrEnabled) EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iSyncDCache);
#ifdef SKIP_ASM
extern void libkernl_writebackDCacheRange(unsigned, unsigned);
/**
 * Writes back and invalidates the data cache for an address range (for interrupt handlers: no interrupt masking).
 *
 * Aligns both bounds down to 64 bytes and calls libkernl_writebackDCacheRange. No code calls it.
 *
 * @param startAddr  the first address
 * @param endAddr    the last address
 */
void iSyncDCache(unsigned startAddr, unsigned endAddr) {
    libkernl_writebackDCacheRange(startAddr & ~63, endAddr & ~63);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_invalidateDCacheRange);
#ifdef SKIP_ASM
/**
 * Invalidates, without writing back, every data-cache line whose address lies in [lo, hi].
 *
 * The same tag walk as libkernl_writebackDCacheRange, but runs cache 0x16 (index invalidate), so dirty data in the range is discarded.
 * InvalidDCache and iInvalidDCache call it with 64-byte aligned bounds.
 *
 * @param lo  the first line address
 * @param hi  the last line address
 */
void libkernl_invalidateDCacheRange(unsigned lo, unsigned hi) {
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
extern void libkernl_invalidateDCacheRange(unsigned, unsigned);
/**
 * Invalidates the data cache for an address range without writing it back, with interrupts held off.
 *
 * Aligns both bounds down to 64 bytes and calls libkernl_invalidateDCacheRange, between DIntr and EIntr when interrupts were enabled.
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
    libkernl_invalidateDCacheRange(startAddr & ~63, endAddr & ~63);
    if (intrEnabled) EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", iInvalidDCache);
#ifdef SKIP_ASM
extern void libkernl_invalidateDCacheRange(unsigned, unsigned);
/**
 * Invalidates the data cache for an address range without writing it back (for interrupt handlers: no interrupt
 * masking).
 *
 * Aligns both bounds down to 64 bytes and calls libkernl_invalidateDCacheRange. No code calls it.
 *
 * @param startAddr  the first address
 * @param endAddr    the last address
 */
void iInvalidDCache(unsigned startAddr, unsigned endAddr) {
    libkernl_invalidateDCacheRange(startAddr & ~63, endAddr & ~63);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_isIntrDisabled);
#ifdef SKIP_ASM
/**
 * Returns 1 when the COP0 Status register's IE bit (bit 0) is clear, else 0.
 *
 * No code calls it.
 *
 * @return 1 or 0
 */
int libkernl_isIntrDisabled(void) {
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
INCLUDE_ASM("seg/seg_325220", libkernl_requestThread);
#ifdef SKIP_ASM
extern char str_internel_error_in_libkernl_a[];
extern int kprintf_pf(const char *, ...) __asm__("kprintf");
extern int WaitSema_i(int) __asm__("WaitSema");
extern void RotateThreadReadyQueue(int);
extern void WakeupThread(int);
extern void SuspendThread(int);
/**
 * Runs forever as the helper thread that carries out thread requests queued from interrupt handlers.
 *
 * Each time the semaphore libkernl_requestSema is signalled it takes the next 2-byte entry {type, thread id} from the 512-entry
 * ring (read index in the first word, entries from +8) and calls WakeupThread (WAKE_REQUEST_WAKEUP),
 * RotateThreadReadyQueue (WAKE_REQUEST_ROTATE_READY_QUEUE, the id being a priority) or SuspendThread
 * (WAKE_REQUEST_SUSPEND). Any other type prints "## internel error in libkernl.a!" (sic) with kprintf. InitThread
 * starts it with libkernl_requestQueue.
 *
 * @param wakeQueue  the request queue
 */
void libkernl_requestThread(sWakeQueue *wakeQueue) {
    unsigned i;
    unsigned j;
    unsigned char *entryTypes = (unsigned char *)&wakeQueue->e[0].a;
    unsigned char *entryThreads = (unsigned char *)&wakeQueue->e[0].b;
    for (;;) {
        WaitSema_i(libkernl_requestSema);
        i = wakeQueue->readIdx & 0x1FF;
        j = i << 1;
        wakeQueue->readIdx = i + 1;
        switch (*(entryTypes + j)) {
        case WAKE_REQUEST_WAKEUP:
            WakeupThread(*(entryThreads + j));
            break;
        case WAKE_REQUEST_ROTATE_READY_QUEUE:
            RotateThreadReadyQueue(*(entryThreads + j));
            break;
        case WAKE_REQUEST_SUSPEND:
            SuspendThread(*(entryThreads + j));
            break;
        default:
            kprintf_pf(str_internel_error_in_libkernl_a);
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
extern void libkernl_requestThread(sWakeQueue *);
int InitThread_i(void) __asm__("InitThread");
/**
 * Creates and starts the helper thread for interrupt-context thread requests and returns its id, or -1.
 *
 * Returns -1 when the thread already exists (libkernl_helperThreadId > 0). Creates the semaphore libkernl_requestSema (max 0xFF), then the
 * thread running libkernl_requestThread with the 0x400-byte stack D_0052AFA0, gp D_004A30F0 and priority 0; a failed thread
 * deletes the semaphore again. It clears the queue libkernl_requestQueue, starts the thread with it as the argument and drops the
 * calling thread to priority 1. _InitSys calls it.
 *
 * @return the helper thread id, or -1
 */
int InitThread_i(void) {
    int cfg[12];
    int buf[8];
    if (libkernl_helperThreadId > 0) {
        return -1;
    }
    buf[2] = 0;
    buf[1] = 0xFF;
    libkernl_requestSema = CreateSema(buf);
    if (libkernl_requestSema < 0) {
        return -1;
    }
    cfg[1] = (int)libkernl_requestThread;
    cfg[2] = (int)D_0052AFA0;
    cfg[3] = 0x400;
    cfg[4] = (int)D_004A30F0;
    cfg[5] = 0;
    libkernl_helperThreadId = CreateThread(cfg);
    if (libkernl_helperThreadId < 0) {
        DeleteSema(libkernl_requestSema);
        return -1;
    }
    libkernl_requestQueue.readIdx = 0;
    libkernl_requestQueue.idx = 0;
    StartThread(libkernl_helperThreadId, &libkernl_requestQueue);
    ChangeThreadPriority(GetThreadId(), 1);
    return libkernl_helperThreadId;
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
 * thread (id below 0x100, helper running) it queues a type-0 entry in libkernl_requestQueue and signals libkernl_requestSema with
 * iSignalSema. sifrpc_onCallCmd calls it to wake an RPC server thread.
 *
 * @param threadId  the thread id
 * @return the thread id or _iWakeupThread's result, or -1 when it cannot be queued
 */
int iWakeupThread_r(int threadId) {
    int id;
    unsigned i;
    SYSCALL_RET_4C50(-47, id);
    if (id != threadId) return _iWakeupThread();
    if ((unsigned)id >= 0x100 || libkernl_helperThreadId == 0) return -1;
    i = libkernl_requestQueue.idx & 0x1FF;
    libkernl_requestQueue.idx = i + 1;
    libkernl_requestQueue.e[i].a = WAKE_REQUEST_WAKEUP;
    libkernl_requestQueue.e[i].b = id;
    iSignalSema(libkernl_requestSema);
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
 * libkernl_requestSema with iSignalSema. THREAD_yield and THREAD_yieldticks call it.
 *
 * @param priority  the priority
 * @return the priority, or -1
 */
int iRotateThreadReadyQueue(unsigned priority) {
    unsigned i;
    if (priority >= 0x80 || libkernl_helperThreadId == 0) {
        return -1;
    }
    i = libkernl_requestQueue.idx & 0x1FF;
    libkernl_requestQueue.idx = i + 1;
    libkernl_requestQueue.e[i].a = WAKE_REQUEST_ROTATE_READY_QUEUE;
    libkernl_requestQueue.e[i].b = priority;
    iSignalSema(libkernl_requestSema);
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
    if ((unsigned)id >= 0x100 || libkernl_helperThreadId == 0) return -1;
    i = libkernl_requestQueue.idx & 0x1FF;
    libkernl_requestQueue.idx = i + 1;
    libkernl_requestQueue.e[i].a = WAKE_REQUEST_SUSPEND;
    libkernl_requestQueue.e[i].b = id;
    iSignalSema(libkernl_requestSema);
    return id;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_initTtyRing);
#ifdef SKIP_ASM
/**
 * Initialises the console input ring buffer in libkernl_ttyRingStorage and returns it.
 *
 * Stores the data size, a zero count and read and write pointers at the start of the data (+0x10). The storage is
 * fixed, so every call returns the same ring. libkernl_openTtyChannel calls it with 0x100.
 *
 * @param dataSize  the data size in bytes
 * @return the ring
 */
sRing *libkernl_initTtyRing(int dataSize) {
    libkernl_ttyRingStorage[0] = dataSize;
    libkernl_ttyRingStorage[1] = 0;
    libkernl_ttyRingStorage[3] = (int)&libkernl_ttyRingStorage[4];
    libkernl_ttyRingStorage[2] = (int)&libkernl_ttyRingStorage[4];
    return (sRing *)libkernl_ttyRingStorage;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_ringAdvanceWrite);
#ifdef SKIP_ASM
/**
 * Counts one byte written into a ring buffer and advances its write pointer, wrapping at the end of the data.
 *
 * @param ring  the ring
 */
void libkernl_ringAdvanceWrite(sRing *ring) {
    ring->count++;
    ring->wr++;
    if (ring->wr == (char *)ring + (ring->size + 16)) {
        ring->wr = ring->data;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_ringAdvanceRead);
#ifdef SKIP_ASM
/**
 * Counts one byte read from a ring buffer and advances its read pointer, wrapping at the end of the data.
 *
 * @param ring  the ring
 */
void libkernl_ringAdvanceRead(sRing *ring) {
    ring->count--;
    ring->rd++;
    if (ring->rd == (char *)ring + (ring->size + 16)) {
        ring->rd = ring->data;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_onTtyDeci2Event);
#ifdef SKIP_ASM
// The read position (volatile in sTty) is taken once into a local just before the read call.
extern int pf_4EA8(const char *, ...) __asm__("kprintf");
extern int rd_4EA8(int, unsigned char *, unsigned short) __asm__("sceDeci2ExRecv");
extern int wr_4EA8(int, char *, unsigned short) __asm__("sceDeci2ExSend");
extern char D_0049AB08[], D_0049AB30[], D_0049AB48[], D_0049AB60[];
/**
 * Handles a DECI2 event for the console channel (the callback libkernl_openTtyChannel registers).
 *
 * Events 1 and 2 (data arrived): with n bytes pending it receives them with sceDeci2ExRecv into the receive buffer at
 * pos; n = 0 ends the packet, whose payload (from byte 12 up to the length in its first halfword) is copied into the
 * input ring with libkernl_ringAdvanceWrite. Event 3 (ready to send) sends the rest of the packet with sceDeci2ExSend and advances
 * wp and wlen. Event 4 (send done) clears the busy flag fC. Problems are printed with kprintf ("TTY: packet size
 * larger than expect", "TTY: receive error", "TTY: send err %d", "TTY: err ti->wlen=%08x").
 *
 * Likely bug: a packet over 320 bytes is only reported, then still received past the end of the buffer.
 *
 * @param cmd  the event code
 * @param n    the bytes pending (events 1 and 2)
 * @param ttyState    the console state libkernl_ttyState
 */
void libkernl_onTtyDeci2Event(int cmd, int n, sTty *ttyState) {
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
                libkernl_ringAdvanceWrite(ttyState->ring);
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
INCLUDE_ASM("seg/seg_325220", libkernl_ttySend);
#ifdef SKIP_ASM
/* DECI2 tty write: copy up to 256 bytes ('\n' -> "\r\n") into the uncached send packet and send it. */
/* DECI2 send packet: the 8-byte DECI2 header (length, reserved, protocol, source and destination node) and a word
   before the text (0; the text starts at +12). Names ours, from libkernl_openTtyChannel's set-up. */
struct Pkt_5040 { short length; short reserved; short protocol; char source; char destination; int textHeader; };
extern char libkernl_ttySendPacket[];
extern int f5040_send(int, char) __asm__("sceDeci2ReqSend");
extern void f5040_poll(int) __asm__("sceDeci2Poll");
int libkernl_ttySend_d(char *buf, int len) __asm__("libkernl_ttySend");
/**
 * Sends bytes to the host console over DECI2 and returns how many were taken, or -1.
 *
 * Fails when a send is in progress (fC). With interrupts off (DIntr) it builds the packet in the uncached buffer
 * libkernl_ttySendPacket: payload from +12, each '\n' expanded to "\r\n", at most 256 payload bytes (the rest is left to the
 * caller). It sets wlen and the packet length, requests the send with sceDeci2ReqSend and polls sceDeci2Poll until
 * libkernl_onTtyDeci2Event clears fC, then calls EIntr. _write calls it.
 *
 * @param buf  the bytes
 * @param len  the byte count
 * @return the input bytes sent, or -1
 */
int libkernl_ttySend_d(char *buf, int len) {
    struct Pkt_5040 *pkt;
    char *payloadCursor;
    int i = 0;
    int n = 0;
    if (libkernl_ttyState.fC != 0)
        return -1;
    DIntr();
    pkt = (struct Pkt_5040 *)((int)libkernl_ttySendPacket | 0x20000000);
    libkernl_ttyState.fC = 1;
    libkernl_ttyState.wp = (char *)pkt;
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
    libkernl_ttyState.wlen = i + 12;
    pkt->length = libkernl_ttyState.wlen;
    if (f5040_send(libkernl_ttyState.fd, pkt->destination) < 0) {
        libkernl_ttyState.fC = 0;
        EIntr();
        return -1;
    }
    while (libkernl_ttyState.fC != 0)
        f5040_poll(libkernl_ttyState.fd);
    EIntr();
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_ttyReadLine);
#ifdef SKIP_ASM
/**
 * Reads console input from the ring into a buffer, up to n bytes or through the first line end, and returns the count.
 *
 * Busy-waits while the ring (libkernl_ttyState.ring) is empty, takes one byte at a time with libkernl_ringAdvanceRead,
 * and stops after a '\n' or '\r'. _read calls it.
 *
 * @param bufAddr  the destination
 * @param n   the maximum byte count
 * @return the bytes read, including the line end
 */
int libkernl_ttyReadLine(int bufAddr, int n) {
    char *buf = (char *)bufAddr;
    int i;
    int j;
    char *charSlot;
    for (i = 0; i < n; i = j) {
        charSlot = buf + i;
        j = i + 1;
        while (libkernl_ttyState.ring->count == 0) {
        }
        *charSlot = *libkernl_ttyState.ring->rd;
        libkernl_ringAdvanceRead(libkernl_ttyState.ring);
        if (*charSlot == '\n' || *charSlot == '\r') {
            return j;
        }
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_openTtyChannel);
#ifdef SKIP_ASM
// The binary re-reads fd right after storing it (sTty.fd is volatile).
/* libkernl_ttySend's Pkt_5040 (names ours) */
struct Pkt_5260 { short length; short reserved; short protocol; char source; char destination; int textHeader; };
extern char libkernl_ttySendPacket[];
extern char D_0052BA40[];
extern void FlushCache(int);
extern int f5260_open(int, void *, void *) __asm__("sceDeci2Open");
/**
 * Opens the DECI2 console channel and returns 1, or 0 when it cannot.
 *
 * Flushes the caches (FlushCache(0)) and opens protocol 0x210 with sceDeci2Open, registering libkernl_ttyState and the
 * callback libkernl_onTtyDeci2Event. Then it clears the state, points the receive buffer at the uncached D_0052BA40, prepares the
 * send packet header in the uncached libkernl_ttySendPacket (protocol 0x210, source 'E', destination 'H') and creates the
 * 0x100-byte input ring with libkernl_initTtyRing.
 *
 * @return 1 when open, else 0
 */
int libkernl_openTtyChannel(void) {
    struct Pkt_5260 *sendPacket;
    FlushCache(0);
    libkernl_ttyState.fd = f5260_open(528, &libkernl_ttyState, libkernl_onTtyDeci2Event);
    if (libkernl_ttyState.fd < 0) return 0;
    libkernl_ttyState.fC = 0;
    libkernl_ttyState.wlen = 0;
    libkernl_ttyState.pos = 0;
    libkernl_ttyState.buf = (unsigned char *)((int)D_0052BA40 | 0x20000000);
    sendPacket = (struct Pkt_5260 *)((int)libkernl_ttySendPacket | 0x20000000);
    libkernl_ttyState.wp = (char *)sendPacket;
    sendPacket->protocol = 528;
    sendPacket->source = 69;
    sendPacket->destination = 72;
    sendPacket->reserved = 0;
    sendPacket->textHeader = 0;
    libkernl_ttyState.ring = libkernl_initTtyRing(0x100);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_sioPutc);
#ifdef SKIP_ASM
int libkernl_sioPutc_i(int a0) __asm__("libkernl_sioPutc");
/**
 * Writes one character to the SIO serial port and returns it.
 *
 * Waits while bit 15 of 0x1000F130 (transmit FIFO full) is set, then stores the character in 0x1000F180.
 *
 * @param outChar  the character
 * @return the character
 */
int libkernl_sioPutc_i(int outChar) {
    while (*(volatile int *)0x1000F130 & 0x8000)
        ;
    *(volatile char *)0x1000F180 = outChar;
    return outChar;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_bufferPrintChar);
#ifdef SKIP_ASM
extern int D_00455174;
extern char D_0052BB80[];
extern void libkernl_deci2Puts_v(char *) __asm__("libkernl_deci2Puts");
/**
 * Adds a character to the 128-byte console line buffer, printing the line at a newline or when the buffer is full.
 *
 * The buffer is D_0052BB80 with the fill count D_00455174; libkernl_deci2Puts prints it. A newline is stored, terminated and
 * printed at once. scePrintf installs it as the output routine.
 *
 * Likely bug: when 126 characters are waiting it terminates the buffer at index 127, so the byte at index 126 (left
 * from an earlier line) is printed too.
 *
 * @param outChar  the character
 */
void libkernl_bufferPrintChar(int outChar) {
    int idx;
    if (D_00455174 >= 0x7E) {
        D_00455174 = 0;
        D_0052BB80[0x7F] = 0;
        libkernl_deci2Puts_v(D_0052BB80);
    }
    idx = D_00455174;
    if (outChar == '\n') {
        D_00455174 = 0;
        D_0052BB80[idx] = outChar;
        D_0052BB80[idx + 1] = 0;
        libkernl_deci2Puts_v(D_0052BB80);
    } else {
        D_00455174 = idx + 1;
        D_0052BB80[idx] = outChar;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_sioPutcCrlf);
#ifdef SKIP_ASM
extern void libkernl_sioPutc(int c);
/**
 * Writes a character to the SIO serial port, sending "\r\n" for a newline.
 *
 * It is the initial value of the output routine pointer libkernl_putcHook. Uses libkernl_sioPutc.
 *
 * @param outChar  the character
 */
void libkernl_sioPutcCrlf(int outChar) {
    if (outChar == '\n') {
        libkernl_sioPutc('\r');
        libkernl_sioPutc('\n');
    } else {
        libkernl_sioPutc(outChar);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_doubleBitsToInt);
#ifdef SKIP_ASM
int f5440(long a0) __asm__("libkernl_doubleBitsToInt");
/* The two guard bits kept below the result while it is shifted down ([U] rounded up only when both are set).
   Name ours. */
#define GUARD_BITS 3
/**
 * Converts a non-negative double, given as its 64-bit pattern, to an integer.
 *
 * Values below 2^-54 give 0 and exponents of 13 and more (values of 2^65 and up) give 9999. Values below 1 keep two
 * guard bits and round up only when both are set.
 *
 * Likely bug: the rounding threshold is 0.75, not 0.5; the result is truncated to 32 bits. libkernl_printDouble calls it.
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
        if ((mantissa & GUARD_BITS) == GUARD_BITS) {
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
INCLUDE_ASM("seg/seg_325220", libkernl_printDouble);
#ifdef SKIP_ASM
extern long dpcmp(double, double);
extern double dpsub(double, double);
extern double dpmul(double, double);
extern double dpdiv(double, double);
extern double __fixunsdfdi_d(double) __asm__("__fixunsdfdi");
extern double libkernl_doubleBitsToInt_d(double) __asm__("libkernl_doubleBitsToInt");
extern void (*libkernl_putcHook)(int);
extern char D_0049AB80[];
extern char D_0049AB88[];
extern char D_0049AB90[];
/**
 * Prints a double in "0.dddddd" plus exponent form through the output routine libkernl_putcHook.
 *
 * A negative value prints '-' and is negated. The value is scaled by powers of 10 into [0.1, 1) with dpmul / dpdiv,
 * counting the exponent, multiplied by 1e6 and converted with __fixunsdfdi and libkernl_doubleBitsToInt; then kprintf prints
 * "0.%d" and "e+%d" or "e%d". libkernl_formatOutput calls it for %e and %f.
 *
 * Likely bug: libkernl_doubleBitsToInt decodes its argument as a double's bit pattern but receives the integer from __fixunsdfdi,
 * so the digit part prints as 0. A zero argument would never leave the scaling loop; libkernl_formatOutput prints zero itself.
 *
 * @param x  the value
 */
void libkernl_printDouble(double x) {
    int n = 0;
    if (dpcmp(x, 0.0) < 0) {
        x = dpsub(0.0, x);
        libkernl_putcHook(45);
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
        double scaledDigits = libkernl_doubleBitsToInt_d(__fixunsdfdi_d(x));
        kprintf((int)D_0049AB80, scaledDigits);
    }
    if (n >= 0)
        kprintf((int)D_0049AB88, n);
    else
        kprintf((int)D_0049AB90, n);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_formatOutput);
#ifdef SKIP_ASM
extern void (*libkernl_putcHook)(int);
extern int DIntr_p(void) __asm__("DIntr");
extern void EIntr(void);
extern long __moddi3(long, long);
extern long __divdi3(long, long);
extern unsigned long __umoddi3(unsigned long, unsigned long);
extern unsigned long __udivdi3(unsigned long, unsigned long);
extern double fptodp(float);
extern void libkernl_printDouble(double x);
// printf core: fmt + va_list (8-byte argument slots); 64-bit div/mod go through the soft helpers.
void libkernl_formatOutput_f(char *fmt, char *ap) __asm__("libkernl_formatOutput");
/**
 * Formats text from a format string and argument list, sending each character to the output routine libkernl_putcHook.
 *
 * Supports %d, %u, %x, %o with h or l sizes and a 0-prefixed width of up to 31 digits, %c, %s and %e / %f (printed by
 * libkernl_printDouble). Arguments take 8-byte slots; 64-bit division uses __moddi3, __divdi3, __umoddi3 and
 * __udivdi3. Interrupts are off (DIntr) for the whole call and turned back on (EIntr) at the end when they were on.
 * kprintf and scePrintf call it.
 *
 * Quirks: %s prints "(null)" for an empty string but dereferences a null pointer; %e and %f read the argument slot as a
 * float although C passes a double; %% and unknown conversions print nothing.
 *
 * @param fmt  the format
 * @param ap   the argument pointer
 */
void libkernl_formatOutput_f(char *fmt, char *ap) {
    char buf[32];
    char *zeroFill;
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
        zeroFill = 0;
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
                    zeroFill = &buf[31 - w];
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
                if (zeroFill && zeroFill < printCursor)
                    printCursor = zeroFill;
                while (*printCursor)
                    libkernl_putcHook(*printCursor++);
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
                if (zeroFill && zeroFill < printCursor)
                    printCursor = zeroFill;
                while (*printCursor)
                    libkernl_putcHook(*printCursor++);
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
                        libkernl_putcHook('-');
                    }
                    while (signedValue) {
                        *--printCursor = __moddi3(signedValue, 10) + '0';
                        signedValue = __divdi3(signedValue, 10);
                    }
                }
                if (zeroFill && zeroFill < printCursor)
                    printCursor = zeroFill;
                while (*printCursor)
                    libkernl_putcHook(*printCursor++);
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
                if (zeroFill && zeroFill < printCursor)
                    printCursor = zeroFill;
                while (*printCursor)
                    libkernl_putcHook(*printCursor++);
                break;
            case 'e':
            case 'f':
                ap += 8;
                floatValue = *(float *)(ap - 8);
                if (floatValue == 0.0f)
                    libkernl_putcHook('0');
                else
                    libkernl_printDouble(fptodp(floatValue));
                break;
            case 's':
                ap += 8;
                str = *(char **)(ap - 8);
                if (*str == 0) {
                    libkernl_putcHook('(');
                    libkernl_putcHook('n');
                    libkernl_putcHook('u');
                    libkernl_putcHook('l');
                    libkernl_putcHook('l');
                    libkernl_putcHook(')');
                } else {
                    for (printCursor = str; *printCursor; printCursor++)
                        libkernl_putcHook(*printCursor);
                }
                break;
            case 'c':
                ap += 8;
                signedValue = *(char *)(ap - 8);
                libkernl_putcHook(signedValue);
                break;
            }
        } else {
            libkernl_putcHook(*fmt);
        }
        fmt++;
    }
    if (st)
        EIntr();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", kprintf);
#ifdef SKIP_ASM
// not matched: varargs wrapper (a0 passes through, $5 = &spill at sp+0x58 passed as ap). hand-rolled va_start gives right sd $5..$11
// but also emits swc1 $f12..$f18 saves (target has none) -> size mismatch (18 vs 14 insns).
extern void libkernl_formatOutput(int fmt, char *ap);
/**
 * Prints formatted text through the current output routine libkernl_putcHook (the serial port by default).
 *
 * Builds the argument pointer and calls libkernl_formatOutput. The library's own diagnostics use it: libkernl_requestThread,
 * libkernl_onTtyDeci2Event and libkernl_printDouble.
 *
 * @param fmt  the format
 */
void kprintf(int fmt, ...) {
    char *ap;
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    libkernl_formatOutput(fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", scePrintf);
#ifdef SKIP_ASM
extern void (*libkernl_putcHook)(int);
extern void libkernl_bufferPrintChar(int c);
extern void libkernl_formatOutput(int a0, char *ap);
/**
 * Prints formatted text through the console line buffer.
 *
 * Swaps the output routine libkernl_putcHook for libkernl_bufferPrintChar while libkernl_formatOutput runs, then restores the old one. Callers
 * include sceGsSyncPath, sceUsbKbGetLocation and other Sony library code.
 *
 * @param format  the format
 */
void scePrintf(int format, ...) {
    char *ap;
    void (*old)(int) = libkernl_putcHook;
    libkernl_putcHook = libkernl_bufferPrintChar;
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    libkernl_formatOutput(format, ap);
    libkernl_putcHook = old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifcmd_onSetSregCmd);
#ifdef SKIP_ASM
/**
 * Handles the IOP's set-register system command: stores a value in one of the SIF software registers.
 *
 * Writes packet word 5 into the register array (command data +0x1C) at the index in packet word 4. sceSifInitCmd
 * installs it as system command 1.
 *
 * @param packet   the command packet
 * @param cmdData  the command layer data sifcmd_data
 */
void sifcmd_onSetSregCmd(int *packet, int *cmdData) {
    ((int *)cmdData[7])[packet[4]] = packet[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifcmd_onChangeAddrCmd);
#ifdef SKIP_ASM
/**
 * Handles the IOP's change-address system command: records the IOP's command receive address.
 *
 * Stores packet word 4 as the IOP buffer address (command data +8). sceSifInitCmd installs it as system command 0.
 *
 * @param packet   the command packet
 * @param cmdData  the command layer data sifcmd_data
 */
void sifcmd_onChangeAddrCmd(int *packet, int *cmdData) {
    cmdData[2] = packet[4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifGetSreg);
#ifdef SKIP_ASM
/**
 * Returns one of the 32 SIF software registers (sifcmd_sregs).
 *
 * No bounds check. func_0040B130 and the RPC set-up func_00426408 call it.
 *
 * @param regIndex  the register index
 * @return the register value
 */
int sceSifGetSreg(int regIndex) {
    return sifcmd_sregs[regIndex];
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
    sifcmd_sregs[regIndex] = regValue;
    return regValue;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifGetDataTable);
#ifdef SKIP_ASM
/**
 * Returns the SIF command layer data sifcmd_data.
 *
 * No code calls it.
 *
 * @return the command layer data
 */
int *sceSifGetDataTable(void) {
    return sifcmd_data;
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
/* sifcmd_data is declared int[] in this unit; this is its real layout */
extern sCmdData_5D38 cmd_5D38 __asm__("sifcmd_data");
extern int sifcmd_initializedFlag;          /* SIF command layer initialised */
extern char D_0052BC00[];       /* receive packet buffer */
extern char D_0052BC80[];
extern sCmdHandler_5D38 D_0052BD00[32];
extern int D_0052BCC0[5];       /* change-address / init packet */
extern int sifcmd_dmacHandlerId;          /* DMAC 5 handler id */
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
 * With interrupts off it returns at once when sifcmd_initializedFlag is set; otherwise it sets the flag and fills the command data
 * sifcmd_data: the uncached receive buffer D_0052BC00, the 32-entry system handler table D_0052BD00 (cleared, then
 * entries 0 and 1 set to sifcmd_onChangeAddrCmd and sifcmd_onSetSregCmd), no user table, and the 32 cleared software registers
 * sifcmd_sregs.
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
    if (sifcmd_initializedFlag) {
        EIntr();
        return;
    }
    sifcmd_initializedFlag = 1;
    cmd_5D38.iopbuf = 0;
    cmd_5D38.pktbuf = (void *)((int)D_0052BC00 | 0x20000000);
    cmd_5D38.unused = (void *)((int)D_0052BC80 | 0x20000000);
    cmd_5D38.sys = D_0052BD00;
    cmd_5D38.nsys = 32;
    cmd_5D38.usr = 0;
    cmd_5D38.nusr = 0;
    cmd_5D38.sregs = sifcmd_sregs;
    for (i = 0; i < 32; i++) {
        D_0052BD00[i].func = 0;
        D_0052BD00[i].data = 0;
    }
    for (i = 0; i < 32; i++)
        sifcmd_sregs[i] = 0;
    D_0052BD00[0].func = sifcmd_onChangeAddrCmd;
    D_0052BD00[0].data = &cmd_5D38;
    D_0052BD00[1].func = sifcmd_onSetSregCmd;
    D_0052BD00[1].data = &cmd_5D38;
    EIntr();
    FlushCache(0);
    if (*(volatile int *)0x1000E010 & 0x20)
        *(volatile int *)0x1000E010 = 0x20;
    if (!(*(volatile int *)0x1000C000 & 0x100))
        sceSifSetDChain();
    sifcmd_dmacHandlerId = f5D38_adddh(5, func_00426230, 0);
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
extern int sifcmd_initializedFlag;
extern int sifcmd_dmacHandlerId;
/**
 * Shuts the SIF command layer down.
 *
 * Disables DMA channel 5 (DisableDmac), removes the channel 5 handler with RemoveDmacHandler and clears the
 * initialised flag sifcmd_initializedFlag. sceSifExitRpc and shutdownAndLoadExec call it.
 */
void sceSifExitCmd(void) {
    DisableDmac(5);
    RemoveDmacHandler(5, sifcmd_dmacHandlerId);
    sifcmd_initializedFlag = 0;
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
    int old = sifcmd_data[5];
    sifcmd_data[5] = handlerTable;
    sifcmd_data[6] = handlerCount;
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
    int old = sifcmd_data[3];
    sifcmd_data[3] = handlerTable;
    sifcmd_data[4] = handlerCount;
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
 * installs sifrpc_onEndCmd, sifrpc_onBindCmd, sifrpc_onCallCmd and sifrpc_onGetOtherDataCmd for commands 0x80000008, 9, 0xA and 0xC;
 * SNDPLATFORM_init and fileio_init add their own.
 *
 * @param cmdId  the command id
 * @param handler  the handler
 * @param handlerData  the handler's data
 */
void sceSifAddCmdHandler(int cmdId, int handler, int handlerData) {
    int off = cmdId << 3;
    /* cmdId is reused for the table base (a separate local changes the code) */
    if (cmdId < 0) {
        cmdId = sifcmd_data[3];
    } else {
        cmdId = sifcmd_data[5];
    }
    off += cmdId;
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
 * Clears the handler pointer of the slot (id times 8). libcdvd_shutdown calls it.
 *
 * @param cmdId  the command id
 */
void sceSifRemoveCmdHandler(int cmdId) {
    int off = cmdId << 3;
    /* cmdId is reused for the table base (a separate local changes the code) */
    if (cmdId < 0) {
        cmdId = sifcmd_data[3];
    } else {
        cmdId = sifcmd_data[5];
    }
    off += cmdId;
    *(int *)off = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifcmd_sendPacket);
#ifdef SKIP_ASM
extern int sifcmd_iopRecvAddr;
extern void func_00426358(int, int);
int sifcmd_sendPacket_d(int, int, int *, unsigned, int, int, int) __asm__("sifcmd_sendPacket");
/**
 * Sends a SIF command packet to the IOP, optionally preceded by an extra data block, and returns the DMA id (0 on
 * failure).
 *
 * The packet size must be 0x10..0x70. With extra data it first queues a DMA of a6 bytes from a4 to a5 and records the
 * destination and size in the packet header; flag 4 writes the extra data back from the data cache first
 * (func_00426358). The packet itself (size byte and command id at +8 filled in, written back with func_00426358) goes
 * to the IOP receive address sifcmd_iopRecvAddr with attribute 0x44. Flag 1 queues both with isceSifSetDma (interrupt context),
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
int sifcmd_sendPacket_d(int cmdId, int sendFlags, int *packet, unsigned packetSize, int extraSrc, int extraDest, int extraSize) {
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
    ent[n].addr = sifcmd_iopRecvAddr;
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
extern int sifcmd_sendPacket(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
/**
 * Sends a SIF command packet to the IOP from thread context and returns the DMA id (0 on failure).
 *
 * Calls sifcmd_sendPacket with flags 0. Callers include sceSifCallRpc, sceSifExecRequest, sceSifBindRpc and sceSifGetOtherData.
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
    return sifcmd_sendPacket(cmdId, 0, packet, packetSize, extraSrc, extraDest, extraSize);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", isceSifSendCmd);
#ifdef SKIP_ASM
extern int sifcmd_sendPacket(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
/**
 * Sends a SIF command packet to the IOP from interrupt context and returns the DMA id (0 on failure).
 *
 * Calls sifcmd_sendPacket with flag 1 (isceSifSetDma). The RPC command handlers sifrpc_onGetOtherDataCmd, sifrpc_onBindCmd and
 * sifrpc_onRetryReplyAlarm reply with it.
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
    return sifcmd_sendPacket(cmdId, 1, packet, packetSize, extraSrc, extraDest, extraSize);
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
extern int sifrpc_initFlag;
/**
 * Shuts the SIF RPC layer down.
 *
 * Calls sceSifExitCmd and clears the RPC initialised flag sifrpc_initFlag. sceSifRebootIop calls it.
 */
void sceSifExitRpc(void) {
    sceSifExitCmd();
    sifrpc_initFlag = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifrpc_takePacket);
#ifdef SKIP_ASM
extern int DIntr_rq(void) __asm__("DIntr");
extern void EIntr(void);
sSifRpcPkt *f65D0(sSifRpcState *s) __asm__("sifrpc_takePacket");
/**
 * Takes a free RPC packet from the packet table and returns it, or null.
 *
 * With interrupts off it scans the table for an entry whose word +0x10 has bit 0 clear, marks it (index << 16 | 5),
 * stores its own address at +0x14 and a new packet id at +0x18. The id comes from the counter (+0); the first call
 * hands out 1 and leaves the counter at 2, so id 2 is never used. sceSifCallRpc, sceSifBindRpc and sceSifGetOtherData call
 * it.
 *
 * @param rpcState  the RPC state sifrpc_state
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
        if (!(packet->slot & 1)) {
            packet->slot = (i << 16) | 5;
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
            packet->pkt = (int)packet;
            packet->rpc_id = id;
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
INCLUDE_ASM("seg/seg_325220", sifrpc_freePacket);
#ifdef SKIP_ASM
/**
 * Releases an RPC packet: clears its id and its in-use bit.
 *
 * @param packet  the packet
 */
void sifrpc_freePacket(unsigned int *packet) {
    packet[6] = 0;
    packet[4] &= 0xFFFFFFFE;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifrpc_nextRecvPacket);
#ifdef SKIP_ASM
char *f6698(sSifRpcState *a0) __asm__("sifrpc_nextRecvPacket");
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
INCLUDE_ASM("seg/seg_325220", sifrpc_getDataPacket);
#ifdef SKIP_ASM
extern int sifrpc_nextRecvPacket(sSifRpcState *a0);
/**
 * Returns the numbered 64-byte RPC data packet, or the next round-robin receive packet when the number is out of range.
 *
 * @param rpcState  the RPC state
 * @param slot      the packet number
 * @return the packet address
 */
int sifrpc_getDataPacket(sSifRpcState *rpcState, int slot) {
    if (slot < 0 || slot >= rpcState->rdata_len) {
        return sifrpc_nextRecvPacket(rpcState);
    }
    return rpcState->rdata + (slot << 6);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifrpc_onEndCmd);
#ifdef SKIP_ASM
/* the client data (sSifClient) and the end packet; names ours, after sSifClient's */
struct Q6708 { int packet; int pad4; int sema; char padC[0x10]; int end_function; int end_param; int server; };
struct A6708 { char pad[0x1C]; struct Q6708 *client; unsigned request_cmd; int server; int buff; int cbuff; };
extern void iSignalSema(int);
extern void sifrpc_freePacket_k(int) __asm__("sifrpc_freePacket");
void f6708(struct A6708 *a) __asm__("sifrpc_onEndCmd");
/**
 * Handles the IOP's RPC end command: completes a call or bind and releases the waiting client.
 *
 * The finished request type is at +0x20: 0x8000000A (call) runs the client's end_function with end_param; 0x80000009
 * (bind) stores the server, buff and cbuff from the packet; 0x8000000B does nothing. Then it signals the client's
 * semaphore (rpcd[2]) when it has one, releases its packet with sifrpc_freePacket and clears the client's packet pointer.
 * func_00426408 installs it for command 0x80000008.
 *
 * @param endRequest  the received end packet
 */
void f6708(struct A6708 *endRequest) {
    struct Q6708 *clientData;
    switch (endRequest->request_cmd) {
    case 0x8000000A:
        clientData = endRequest->client;
        if (clientData->end_function != 0) {
            ((void (*)(int))clientData->end_function)(clientData->end_param);
        }
        break;
    case 0x80000009:
        clientData = endRequest->client;
        clientData->server = endRequest->server;
        ((int *)clientData)[5] = endRequest->buff;
        ((int *)clientData)[6] = endRequest->cbuff;
        break;
    case 0x8000000B:
        break;
    }
    clientData = endRequest->client;
    if (clientData->sema >= 0) {
        iSignalSema(clientData->sema);
    }
    sifrpc_freePacket_k(clientData->packet);
    clientData->packet = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifrpc_onRetryReplyAlarm);
#ifdef SKIP_ASM
extern int isceSifSendCmd(int a0, int a1, int a2, int a3, int a4, int a5);
extern void iSetAlarm(int a0, int (*a1)(int, int, int *), int *a2);
/**
 * Retries sending an RPC reply from an alarm until the command DMA accepts it.
 *
 * Sends the 0x40-byte packet as command 0x80000008 with its extra data (packet words 9..11) through isceSifSendCmd; when
 * that fails it re-arms itself with iSetAlarm (10 ticks). Declared int but returns no value. sifrpc_onGetOtherDataCmd sets the
 * first alarm.
 *
 * @param alarmId    the alarm id (unused)
 * @param alarmTime  the alarm time (unused)
 * @param packet     the reply packet
 * @return no defined value (declared int, but the body has no return statement)
 */
int sifrpc_onRetryReplyAlarm(int alarmId, int alarmTime, int *packet) {
    if (isceSifSendCmd(0x80000008, (int)packet, 0x40, packet[9], packet[10], packet[11]) == 0) {
        iSetAlarm(10, sifrpc_onRetryReplyAlarm, packet);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifrpc_onGetOtherDataCmd);
#ifdef SKIP_ASM
extern int sifrpc_nextRecvPacket(sSifRpcState *a0);
extern int sifrpc_getDataPacket(sSifRpcState *a0, int a1);
extern int isceSifSendCmd(int a0, int a1, int a2, int a3, int a4, int a5);
extern void iSetAlarm(int a0, int (*a1)(int, int, int *), int *a2);
extern int sifrpc_onRetryReplyAlarm(int a0, int a1, int *p);
extern int sifrpc_getDataPacket_u(sSifRpcState *, unsigned) __asm__("sifrpc_getDataPacket");
/**
 * Handles the IOP's get-other-data command: copies the requested block and sends the end reply.
 *
 * Takes a reply packet (the numbered one when request flag 4 is set, else round robin), copies the packet address and
 * client, sets the type 0x8000000C and the source, destination and size, and sends it as command 0x80000008 with the
 * block as extra data through isceSifSendCmd. When the DMA queue is full it retries from an alarm (iSetAlarm with
 * sifrpc_onRetryReplyAlarm). func_00426408 installs it for command 0x8000000C.
 *
 * @param request   the received request packet
 * @param rpcState  the RPC state
 */
void sifrpc_onGetOtherDataCmd(int *request, sSifRpcState *rpcState) {
    int *reply;
    unsigned fl = request[4];
    int pktAddr;
    int client;
    if (fl & 4) {
        reply = (int *)sifrpc_getDataPacket_u(rpcState, fl >> 16);
    } else {
        reply = (int *)sifrpc_nextRecvPacket(rpcState);
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
        iSetAlarm(10, sifrpc_onRetryReplyAlarm, reply);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifGetOtherData);
#ifdef SKIP_ASM
/* The receive data (packet, pid, sema) and the request packet (pkt, rpc_id, client, src, dest, size) share this view;
   names ours, after sSifClient and sSifRpcPkt. */
struct S68F0 {
    struct S68F0 *packet; int pid; int sema; int fC; int f10; int pkt; int rpc_id; int client; int src; int dest;
    int size;
};
extern struct S68F0 *sifrpc_takePacket_p(sSifRpcState *) __asm__("sifrpc_takePacket");
extern void sifrpc_freePacket_j(struct S68F0 *) __asm__("sifrpc_freePacket");
extern int CreateSema_j(int *) __asm__("CreateSema");
extern int WaitSema_j(int) __asm__("WaitSema");
extern int sceSifSendCmd_j(int, int, int, int, int, int) __asm__("sceSifSendCmd");
int sceSifGetOtherData_d(struct S68F0 *, int, int, int, int) __asm__("sceSifGetOtherData");
/**
 * Asks the IOP to copy a block of IOP memory into EE memory and returns 0, or a negative error.
 *
 * Takes a packet with sifrpc_takePacket (-1 when none), links it to the receive data (packet +0, id +4), fills the source,
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
    packet = sifrpc_takePacket_p(&sifrpc_state);
    if (packet == 0) {
        return -1;
    }
    receiveData->packet = packet;
    receiveData->pid = packet->rpc_id;
    packet->src = srcAddr;
    packet->dest = destAddr;
    packet->size = byteCount;
    packet->pkt = (int)packet;
    packet->client = (int)receiveData;
    if (!(mode & 1)) {
        buf[1] = 1;
        buf[2] = 0;
        receiveData->sema = CreateSema_j(buf);
        if (receiveData->sema < 0) {
            sifrpc_freePacket_j(packet);
            return -3;
        }
        if (sceSifSendCmd_j(0x8000000C, (int)packet, 0x40, 0, 0, 0) == 0) {
            sifrpc_freePacket_j(packet);
            DeleteSema(receiveData->sema);
            return -2;
        }
        WaitSema_j(receiveData->sema);
        DeleteSema(receiveData->sema);
        return 0;
    }
    receiveData->sema = -1;
    if (sceSifSendCmd_j(0x8000000C, (int)packet, 0x40, 0, 0, 0) == 0) {
        sifrpc_freePacket_j(packet);
        return -2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifrpc_findServer);
#ifdef SKIP_ASM
/**
 * Finds the RPC server registered under an id and returns it, or null.
 *
 * Walks every queue's server list and compares each server's key (+0). sifrpc_onBindCmd calls it.
 *
 * @param serverId  the server id
 * @param rpcState  the RPC state
 * @return the server, or null
 */
sSifServer *sifrpc_findServer(int serverId, sSifRpcState *rpcState) {
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
INCLUDE_ASM("seg/seg_325220", sifrpc_onBindCmd);
#ifdef SKIP_ASM
extern int sifrpc_nextRecvPacket(sSifRpcState *a0);
extern int *sifrpc_findServer_p(int, sSifRpcState *) __asm__("sifrpc_findServer");
extern void isceSifSendCmd_v(int, int *, int, int, int, int) __asm__("isceSifSendCmd");
/**
 * Handles the IOP's RPC bind command: looks up the server and sends the end reply.
 *
 * Takes a round-robin reply packet, copies the packet address and client, sets the type 0x80000009 and the server found
 * by sifrpc_findServer with its words at +8 and +0x14 (zeros when there is none), and sends it as command 0x80000008 with
 * isceSifSendCmd. func_00426408 installs it for command 0x80000009.
 *
 * @param request   the received request packet
 * @param rpcState  the RPC state
 */
void sifrpc_onBindCmd(int *request, sSifRpcState *rpcState) {
    int *reply = (int *)sifrpc_nextRecvPacket(rpcState);
    int *n;
    int pktAddr = request[5];
    int client = request[7];
    reply[7] = client;
    reply[5] = pktAddr;
    reply[8] = 0x80000009;
    n = sifrpc_findServer_p(request[8], rpcState);
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
/* The client data (packet, pid, sema, command, server) and the bind packet (pkt, rpc_id, client, rpc_number) share
   this view; names ours, after sSifClient and sSifRpcPkt. */
struct S6B48 {
    struct S6B48 *packet; int pid; int sema; int fC; int command; int pkt; int rpc_id; int client; int rpc_number;
    int server;
};
extern struct S6B48 *sifrpc_takePacket(sSifRpcState *);
extern void sifrpc_freePacket_i(struct S6B48 *) __asm__("sifrpc_freePacket");
extern int CreateSema(int *);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceSifBindRpc_d(struct S6B48 *, int, int) __asm__("sceSifBindRpc");
/**
 * Binds an RPC client to the IOP server with the given number and returns 0, or a negative error.
 *
 * Clears the client's command and server fields, takes a packet with sifrpc_takePacket (-1 when none), links it to the
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
    client->command = 0;
    client->server = 0;
    packet = sifrpc_takePacket(&sifrpc_state);
    if (packet == 0) {
        return -1;
    }
    client->packet = packet;
    client->pid = packet->rpc_id;
    packet->rpc_number = rpcNumber;
    packet->pkt = (int)packet;
    packet->client = (int)client;
    if (!(mode & 1)) {
        buf[1] = 1;
        buf[2] = 0;
        client->sema = CreateSema(buf);
        if (client->sema < 0) {
            sifrpc_freePacket_i(packet);
            return -3;
        }
        if (sceSifSendCmd(0x80000009, (int)packet, 0x40, 0, 0, 0) == 0) {
            sifrpc_freePacket_i(packet);
            DeleteSema(client->sema);
            return -2;
        }
        WaitSema_i(client->sema);
        DeleteSema(client->sema);
        return 0;
    }
    client->sema = -1;
    if (sceSifSendCmd(0x80000009, (int)packet, 0x40, 0, 0, 0) == 0) {
        sifrpc_freePacket_i(packet);
        return -2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifrpc_onCallCmd);
#ifdef SKIP_ASM
/* the server queue (sSifQueue), the server data and the call packet; names ours, after sSifQueue, sSifClient and
   sceSifCallRpc's view */
struct B26C88 {
    int thread_id;
    int active;
    int pad8;
    int head;
    int tail;
};
struct E26C88 {
    int pad0[3];
    int size;
    int pad10[3];
    int client;
    int pkt;
    int rpc_number;
    int recv;
    int recv_size;
    int rmode;
    int slot;
    int pad38;
    int next;
    struct B26C88 *owner;
};
struct A26C88 {
    int pad0[4];
    int slot;
    int pkt;
    int pad18;
    int client;
    int rpc_number;
    int send_size;
    int recv;
    int recv_size;
    int rmode;
    struct E26C88 *server;
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
void sifrpc_onCallCmd(struct A26C88 *callRequest) {
    struct E26C88 *serverData = callRequest->server;
    struct B26C88 *rpcQueue = serverData->owner;
    if (rpcQueue->head == 0) {
        rpcQueue->head = (int)serverData;
    } else {
        ((struct E26C88 *)rpcQueue->tail)->next = (int)serverData;
    }
    rpcQueue->tail = (int)serverData;
    {
        int packetAddr = callRequest->pkt;
        int clientAddr = callRequest->client;
        serverData->pkt = packetAddr;
        serverData->client = clientAddr;
    }
    serverData->rpc_number = callRequest->rpc_number;
    serverData->size = callRequest->send_size;
    serverData->recv = callRequest->recv;
    serverData->recv_size = callRequest->recv_size;
    serverData->rmode = callRequest->rmode;
    serverData->slot = callRequest->slot;
    if (rpcQueue->thread_id >= 0 && rpcQueue->active == 0) {
        iWakeupThread(rpcQueue->thread_id);
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
extern struct Call6D18 *f6D18_get(sSifRpcState *) __asm__("sifrpc_takePacket");
extern void f6D18_free(struct Call6D18 *) __asm__("sifrpc_freePacket");
extern void f6D18_wb(void *, int) __asm__("func_00426358");
extern int f6D18_send(int, void *, int, void *, void *, int) __asm__("sceSifSendCmd");
extern int f6D18_csema(struct Sema6D18 *) __asm__("CreateSema");
extern int f6D18_wsema(int) __asm__("WaitSema");
/**
 * Sends a SIF RPC call to the IOP server a bound client points at and, unless asked not to, waits for the reply.
 *
 * Takes a free 0x40-byte call packet from the RPC state sifrpc_state (sifrpc_takePacket; -1 when none is free) and fills it:
 * the function number, send size, receive buffer and size, the packet's own address, the client's bound server and the
 * client. The client records the packet, its id, the end function and its argument. Unless mode bit 1 is set, it writes
 * the send and receive buffers back from the data cache first (func_00426358; once with the larger size when they are
 * the same buffer). The packet goes out as SIF command 0x8000000A with the send data DMA'd to the client's server
 * buffer (sceSifSendCmd). With mode bit 0 set (no wait) the client's semaphore is -1, the packet's reply mode is 1 only
 * when @p end_function is set, and it returns at once. Otherwise it creates a semaphore (max 1, initial 0), waits on it
 * until the reply handler signals it, then deletes it. A failed send frees the packet (sifrpc_freePacket). Called by the
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
    call = f6D18_get(&sifrpc_state);
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
 * end of sifrpc_state.queues (the list sifrpc_findServer searches when a bind request names a server). No caller in this
 * build.
 *
 * @param n  the queue to set up
 * @param threadId  id of the thread that serves the queue (the one woken when a request arrives)
 */
void sceSifSetRpcQueue(sSifQueue *n, int threadId) {
    sSifQueue *lastQueue;
    sSifQueue *firstQueue;
    DIntr_r();
    n->thread_id = threadId;
    n->active = 0;
    n->servers = 0;
    n->head = 0;
    n->tail = 0;
    firstQueue = sifrpc_state.queues;
    n->next = 0;
    if (firstQueue == 0) {
        sifrpc_state.queues = n;
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
/* the server data (next links the queue's servers, next_request its pending requests) and the queue (sSifQueue);
   names ours */
struct N6FE0 {
    int id; int serve_func; int serve_buf; int size; int cancel_func; int cancel_buf; char pad[0x20];
    struct N6FE0 *next; struct N6FE0 *next_request; struct S6FE0 *queue;
};
struct S6FE0 { int thread_id; int active; struct N6FE0 *head; };
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
    n->next_request = 0;
    n->next = 0;
    n->id = serverId;
    firstServer = serverQueue->head;
    n->serve_func = serveFunc;
    n->serve_buf = serveBuf;
    n->cancel_func = cancelFunc;
    n->cancel_buf = cancelBuf;
    n->queue = serverQueue;
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
 * With interrupts disabled it walks sifrpc_state.queues: a head match is replaced by its successor; otherwise the
 * predecessor of @p x is relinked past it. No caller in this build.
 *
 * @param x  the queue to remove
 * @return the old head when @p x was the head, else the predecessor of @p x (0 when not found)
 */
sSifQueue *f7148(sSifQueue *x) {
    sSifQueue *queueCursor;
    f7148a();
    queueCursor = sifrpc_state.queues;
    if (queueCursor == x) {
        sifrpc_state.queues = queueCursor->next;
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
struct S4271D8 { char pad[4]; int active; char pad2[4]; struct N4271D8 *head; };   /* sSifQueue */
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
        serverQueue->active = 0;
    } else {
        struct N4271D8 *nextServer = (struct N4271D8 *)n->next;
        serverQueue->active = 1;
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
extern struct R7230 *sifrpc_getDataPacket_w(sSifRpcState *, unsigned) __asm__("sifrpc_getDataPacket");
extern struct R7230 *sifrpc_nextRecvPacket_w(sSifRpcState *, unsigned) __asm__("sifrpc_nextRecvPacket");
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
 * high half of +0x34 when its bit 2 is set (sifrpc_getDataPacket), else the next round-robin one (sifrpc_nextRecvPacket). The reply
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
        e = sifrpc_getDataPacket_w(&sifrpc_state, fl);
    } else {
        e = sifrpc_nextRecvPacket_w(&sifrpc_state, fl);
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
INCLUDE_ASM("seg/seg_325220", fileio_createSemas);
#ifdef SKIP_ASM
extern int CreateSema(int *p);
/**
 * Creates the file library's two semaphores the first time it is called.
 *
 * When fileio_tableSema is still -1, creates two semaphores (initial and max count 1, option 0): fileio_tableSema guards the
 * 32-entry descriptor table fileio_descriptors and fileio_noWaitSema guards the pending no-wait table D_00455188. Called by
 * fileio_allocDescriptor, fileio_getDescriptor and fileio_init.
 */
void fileio_createSemas(void) {
    int buf[8];
    if (fileio_tableSema == NO_SEMAPHORE) {
        buf[5] = 0;
        buf[2] = 1;
        buf[1] = 1;
        fileio_tableSema = CreateSema(buf);
        fileio_noWaitSema = CreateSema(buf);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_allocDescriptor);
#ifdef SKIP_ASM
extern void fileio_createSemas(void);
extern int WaitSema_i(int) __asm__("WaitSema");
extern void SignalSema(int);
/**
 * Allocates a free slot in the file library's descriptor table.
 *
 * Makes sure the semaphores exist (fileio_createSemas), then under fileio_tableSema finds the first of the 32 entries of
 * fileio_descriptors whose flags (f4) are 0 and marks it in use with 0x10000000. Called by sceOpen and sceDopen.
 *
 * @return the slot, or 0 when all 32 are taken
 */
sFioFile *fileio_allocDescriptor(void) {
    sFioFile *fileSlot;
    sFioFile *end;
    fileio_createSemas();
    WaitSema_i(fileio_tableSema);
    fileSlot = fileio_descriptors;
    end = fileSlot + 32;
    while (fileSlot < end) {
        if (fileSlot->f4 == 0) {
            fileSlot->f4 = 0x10000000;
            SignalSema(fileio_tableSema);
            return fileSlot;
        }
        fileSlot++;
    }
    SignalSema(fileio_tableSema);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_getDescriptor);
#ifdef SKIP_ASM
extern void fileio_createSemas(void);
extern int WaitSema_i(int) __asm__("WaitSema");
extern void SignalSema(int);
/**
 * Returns the descriptor-table slot for a file descriptor number.
 *
 * Makes sure the semaphores exist (fileio_createSemas) and takes fileio_tableSema around the bounds check. The slot is returned
 * whether or not it is open; callers check its flags (f4).
 *
 * @param fdIndex  descriptor number (index into fileio_descriptors)
 * @return &fileio_descriptors[@p fdIndex], or 0 when it is 32 or more
 */
sFioFile *fileio_getDescriptor(unsigned fdIndex) {
    sFioFile *fileSlot;
    fileio_createSemas();
    WaitSema_i(fileio_tableSema);
    if (fdIndex >= 0x20) {
        SignalSema(fileio_tableSema);
        return 0;
    }
    fileSlot = &fileio_descriptors[fdIndex];
    SignalSema(fileio_tableSema);
    return fileSlot;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_onCompletionCmd);
#ifdef SKIP_ASM
struct W7590 { char b[4]; };
struct B7590 { char b[0x144]; };
struct C7590 { char b[0x40]; };
/* a read's unaligned head and tail fix-up (names ours) */
struct L7590 {
    int headLen; int tailLen; char *headDst; char *tailDst; unsigned char headBytes[64]; unsigned char tailBytes[64];
};
extern int D_00455208;
extern void *func_0041605C(void *, void *, int);
extern void iSignalSema(int);
/**
 * Handles the IOP file server's completion command: copies the result to the caller and wakes it.
 *
 * Installed by fileio_init as the handler of SIF command 0x80000011; runs in interrupt context. The reply lives in
 * one of the 0x440-byte buffers at D_0052E380, read through the uncached mirror (| 0x20000000): buffer 0, or the buffer
 * the packet's word 3 names when D_00455210 is set (the index is kept in D_00455208). Words 0..3 of the buffer are the
 * waiter's semaphore (negative for a no-wait call), the request number, the destination and the size; for a waiting
 * caller the result at +0x10 is copied to the destination (func_0041605C, memcpy). Then per request: FIO_RPC_READ (2)
 * writes the unaligned head and tail bytes (up to 64 each, two counts, two addresses and two byte blocks from +0x14) to
 * both the cached and uncached views of the destination; FIO_RPC_DREAD copies a 0x144-byte directory entry and
 * FIO_RPC_GETSTAT a 0x40-byte status block to the address at +0x14; FIO_RPC_DEVCTL, FIO_RPC_READLINK and FIO_RPC_IOCTL2
 * copy the length at +0x18 (capped at 0x400) of data from +0x1C. Finally a no-wait call's entry is cleared to -1 in
 * D_00455188, while a waiting caller's semaphore is signalled (iSignalSema).
 *
 * @param cmdPacket  the received SIF command packet
 */
void fileio_onCompletionCmd(int *cmdPacket) {
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
    case FIO_RPC_READ: {
        struct L7590 *readFixup = (struct L7590 *)(replyBuf + 0x14);
        char *fixupDst;
        char *fixupUncached;
        if (readFixup->headLen > 0) {
            fixupDst = readFixup->headDst;
            fixupUncached = (char *)((int)fixupDst | 0x20000000);
            for (i = 0; i < readFixup->headLen; i++) {
                unsigned char fixupByte = readFixup->headBytes[i];
                fixupUncached[i] = fixupByte;
                fixupDst[i] = fixupByte;
            }
        }
        if (readFixup->tailLen > 0) {
            fixupDst = readFixup->tailDst;
            fixupUncached = (char *)((int)fixupDst | 0x20000000);
            for (i = 0; i < readFixup->tailLen; i++) {
                unsigned char fixupByte = readFixup->tailBytes[i];
                fixupUncached[i] = fixupByte;
                fixupDst[i] = fixupByte;
            }
        }
        break;
    }
    case FIO_RPC_DREAD:
        *(struct W7590 *)&buf = *(struct W7590 *)(replyBuf + 0x14);
        *(struct B7590 *)buf = *(struct B7590 *)(replyBuf + 0x18);
        break;
    case FIO_RPC_GETSTAT:
        *(struct W7590 *)&buf = *(struct W7590 *)(replyBuf + 0x14);
        *(struct C7590 *)buf = *(struct C7590 *)(replyBuf + 0x18);
        break;
    case FIO_RPC_DEVCTL:
    case FIO_RPC_READLINK:
    case FIO_RPC_IOCTL2:
        *(struct W7590 *)&buf = *(struct W7590 *)(replyBuf + 0x14);
        *(struct W7590 *)&len = *(struct W7590 *)(replyBuf + 0x18);
        if (len > FIO_BUFFER_SIZE)
            len = FIO_BUFFER_SIZE;
        func_0041605C(buf, replyBuf + 0x1C, len);
        break;
    }
    if (sema < 0) {
        sema = -sema;
        for (i = 0; i < 0x20; i++) {
            if (D_00455188[i] == sema) {
                D_00455188[i] = FIO_NOWAIT_FREE;
                break;
            }
        }
    } else {
        iSignalSema(sema);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_createCallLock);
#ifdef SKIP_ASM
extern int CreateSema(int *p);
/**
 * Creates the file library's call lock fileio_callLock (a semaphore, initial and max count 1) the first time it is called.
 */
void fileio_createCallLock(void) {
    int buf[8];
    if (fileio_callLock == NO_SEMAPHORE) {
        buf[5] = 0;
        buf[2] = 1;
        buf[1] = 1;
        fileio_callLock = CreateSema(buf);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_lockCall);
#ifdef SKIP_ASM
extern void fileio_createCallLock(void);
extern void WaitSema(int a0);
/**
 * Takes the file library's call lock (WaitSema on fileio_callLock), creating it first if needed (fileio_createCallLock).
 *
 * Every file call wraps its RPC in this and fileio_unlockCall; callers pass their RPC number, which this definition does
 * not use.
 *
 * @return 0
 */
int fileio_lockCall(void) {
    fileio_createCallLock();
    WaitSema(fileio_callLock);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_unlockCall);
#ifdef SKIP_ASM
extern void SignalSema(int);
/**
 * Releases the file library's call lock (SignalSema on fileio_callLock) taken by fileio_lockCall.
 */
void fileio_unlockCall(void) {
    SignalSema(fileio_callLock);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_setCallback);
#ifdef SKIP_ASM
extern void fileio_lockCall_a(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern int DIntr_i(void) __asm__("DIntr");
extern void EIntr(void);
extern void fileio_unlockCall(void);
/**
 * Installs the callback run when the IOP file server sends SIF command 0x80000013, returning the previous one.
 *
 * Under the call lock (fileio_lockCall ... fileio_unlockCall), initialising the library first when fileio_initFlag is 0
 * (fileio_init), it stores @p callbackFn and @p callbackArg in fileio_userCallback with interrupts disabled (re-enabled only when they were
 * on). fileio_onCallbackCmd runs the callback. No caller in this build.
 *
 * @param callbackFn  callback, called with @p callbackArg
 * @param callbackArg  argument for the callback
 * @return the previous callback
 */
void (*fileio_setCallback(void (*callbackFn)(int), int callbackArg))(int) {
    int intrWasOn;
    void (*old)(int);
    sFioCb *callbackSlot = &fileio_userCallback;
    fileio_lockCall_a(27);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    intrWasOn = DIntr_i();
    old = fileio_userCallback.fn;
    callbackSlot->arg = callbackArg;
    fileio_userCallback.fn = callbackFn;
    if (intrWasOn != 0) {
        EIntr();
    }
    fileio_unlockCall();
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_onCallbackCmd);
#ifdef SKIP_ASM
/**
 * Runs the file library's user callback (SIF command 0x80000013 handler installed by fileio_init).
 *
 * Calls @p callback->fn(@p callback->arg) when set, then executes sync and ei: interrupts are re-enabled on return.
 *
 * @param cause  the received command packet (unused)
 * @param callback  fileio_userCallback, set by fileio_setCallback
 */
void fileio_onCallbackCmd(int cause, sFioCb *callback) {
    if (callback->fn)
        callback->fn(callback->arg);
    SYNC_4248();
    EI_4248();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_init);
#ifdef SKIP_ASM
extern void func_00426408_z(int) __asm__("func_00426408");
extern int DIntr_z(void) __asm__("DIntr");
extern void EIntr_z(void) __asm__("EIntr");
extern void sceSifAddCmdHandler_z(int, void *, void *) __asm__("sceSifAddCmdHandler");
extern void fileio_createSemas_z(void) __asm__("fileio_createSemas");
extern void fileio_onCompletionCmd_z(void) __asm__("fileio_onCompletionCmd");
extern void fileio_onCallbackCmd_z(void) __asm__("fileio_onCallbackCmd");
extern int WaitSema_z(int) __asm__("WaitSema");
extern int SignalSema_z(int) __asm__("SignalSema");
int fileio_init_d(void) __asm__("fileio_init");
/**
 * Initialises the EE side of the IOP file library: binds the file server and reads its version.
 *
 * Calls func_00426408(0), clears the user callback fileio_userCallback and, with interrupts disabled, installs fileio_onCompletionCmd
 * (SIF command 0x80000011, reply buffer D_0052EE40) and fileio_onCallbackCmd (0x80000013, the callback) with sceSifAddCmdHandler.
 * Then binds fileio_client to server 0x80000001 (sceSifBindRpc), retrying after a 0x100000-iteration spin until the server
 * is bound. It frees all 32 descriptor slots of fileio_descriptors under fileio_tableSema, sends the addresses of the two 0x440-byte
 * reply buffers D_0052E380 (in D_0052D6C0) with RPC function 0xFF and copies the 4-byte server version to fileio_serverVersion;
 * D_00455210 becomes 1 when the second reply word is 2 (the server then names a reply buffer per request). Sets the
 * initialised flag fileio_initFlag. The file calls in this unit call it when the flag is 0.
 *
 * @return 0; -1 when the bind fails; 0xFFFEFFFF when the version call fails
 */
int fileio_init_d(void) {
    int old;
    int spinCount;
    sFioFile *fileSlot;
    sFioCb *callbackSlot = &fileio_userCallback;
    sRpcVer w;
    func_00426408_z(0);
    fileio_userCallback.fn = 0;
    callbackSlot->arg = 0;
    old = DIntr_z();
    sceSifAddCmdHandler_z(0x80000011, fileio_onCompletionCmd_z, D_0052EE40);
    sceSifAddCmdHandler_z(0x80000013, fileio_onCallbackCmd_z, callbackSlot);
    if (old != 0)
        EIntr_z();
    for (;;) {
        if (sceSifBindRpc(&fileio_client, 0x80000001, 0) < 0)
            return -1;
        if (fileio_client.server != 0)
            break;
        spinCount = 0x100000;
        do {
            spinCount--;
        } while (spinCount != -1);
    }
    fileio_createSemas_z();
    WaitSema_z(fileio_tableSema);
    for (fileSlot = fileio_descriptors; fileSlot < &fileio_descriptors[32]; fileSlot++)
        fileSlot->f4 = 0;
    SignalSema_z(fileio_tableSema);
    D_0052D6C0[0] = (int)D_0052E380;
    D_0052D6C0[1] = D_0052D6C0[0] + 1088;
    if (sceSifCallRpc(&fileio_client, 0xFF, 0, D_0052D6C0, 8, fileio_recvBuffer, 8, 0, 0) < 0)
        return 0xFFFEFFFF;
    fileio_serverVersion = *(sRpcVer *)((int)fileio_recvBuffer | 0x20000000);
    w = *(sRpcVer *)((int)(fileio_recvBuffer + 4) | 0x20000000);
    D_00455210 = *(int *)&w == 2;
    fileio_initFlag = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", fileio_isServerRejected);
#ifdef SKIP_ASM
extern char libkernl_versionTag[];
extern char *D_00455220;
extern int memcmp(char *, char *, int);
/**
 * Returns whether the bound file server's version tag is one the library does not accept.
 *
 * Compares the 4-byte tag fileio_serverVersion (set by fileio_init) with the library's "2710" (libkernl_versionTag) and with the tag
 * D_00455220 points at ("....", D_0049AD48) using memcmp (memcmp). Returns 0 when it equals either; otherwise
 * the result of comparing the two accepted tags, which always differ, so 1. sceOpen refuses to open files then.
 *
 * @return 1 when the server's tag matches neither accepted tag, else 0
 */
int fileio_isServerRejected(void) {
    char *libVersion = libkernl_versionTag;
    char *serverVersion = fileio_serverVersion.b;
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
 * Clears fileio_initFlag and zeroes the 4-byte server version fileio_serverVersion (memset, memset). bxIop_initIop calls it
 * after rebooting the IOP.
 *
 * @return 0
 */
int sceFsReset(void) {
    fileio_initFlag = 0;
    memset(&fileio_serverVersion, 0, 4);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceOpen);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern int fileio_isServerRejected_z(void) __asm__("fileio_isServerRejected");
extern int CreateSema(int *p);
extern int db0i(int) __asm__("DeleteSema");
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceOpen_d(unsigned char *, unsigned, ...) __asm__("sceOpen");
/**
 * Opens a file on the IOP through the file server and returns a descriptor number.
 *
 * Under the call lock (fileio_lockCall ... fileio_unlockCall), initialising the library when needed (fileio_init), it fails
 * with 0xFFFEFFFC when the server's version is not accepted (fileio_isServerRejected) and with -19 when no descriptor slot is
 * free (fileio_allocDescriptor). The RPC function 0 request in fileio_cmd.open holds a new semaphore, the address of a stack
 * result word and its size 4, @p flags without bit 31, the mode (the first variadic argument), the name (up to 0x3FF
 * characters, truncated) and the slot index; it is sent with sceSifCallRpc (0x418 bytes) and the server's first reply
 * word (fileio_recvBuffer, read uncached) says whether it was accepted. The lock is released before waiting on the semaphore
 * for the result (fileio_onCompletionCmd posts it). A negative result frees the slot and is returned; otherwise the IOP's
 * descriptor goes to the slot's f0 and @p flags are or-ed into its f4. Likely bug: when the RPC fails or is refused
 * (-11) the slot taken by fileio_allocDescriptor is never freed. Called by FILEDEV_open.
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
    sFioOpenCmd *openCmd = &fileio_cmd.open;
    fileio_lockCall_b(0);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    if (fileio_isServerRejected_z() != 0) {
        fileio_unlockCall();
        return 0xFFFEFFFC;
    }
    fileSlot = fileio_allocDescriptor();
    if (fileSlot == 0) {
        fileio_unlockCall();
        return -19;
    }
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    mode = *(int *)ap;
    nameChar = (openCmd->name[0] = filePath[0]);
    n = 0;
    if (nameChar != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            nameChar = (openCmd->name[n] = filePath[n]);
            if (nameChar == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        openCmd->name[FIO_BUFFER_SIZE - 1] = 0;
    }
    idx = fileSlot - fileio_descriptors;
    openCmd->flags = flags & 0x7FFFFFFF;
    openCmd->mode = mode;
    buf[1] = 1;
    openCmd->slot = idx;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    openCmd->sema = sem;
    openCmd->resultAddr = (int)&buf[8];
    openCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 0, 0, &fileio_cmd.open, 1048, fileio_recvBuffer, 4, 0, 0) < 0) {
        db0i(sem);
        fileio_unlockCall();
        return -11;
    }
    result = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
    if (result == 0) {
        db0i(sem);
        return -11;
    }
    WaitSema_i(sem);
    db0i(sem);
    if (buf[8] < 0) {
        WaitSema_i(fileio_tableSema);
        fileSlot->f4 = 0;
        SignalSema(fileio_tableSema);
        return buf[8];
    }
    result = idx;
    WaitSema_i(fileio_tableSema);
    fileSlot->f0 = buf[8];
    fileSlot->f4 |= flags;
    SignalSema(fileio_tableSema);
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceClose);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Closes a file descriptor through the IOP file server.
 *
 * Takes the call lock (fileio_lockCall; released by fileio_unlockCall), but unlike sceOpen does not initialise the
 * library: returns -1 when fileio_initFlag is 0 and -9 when the descriptor's slot (fileio_getDescriptor) is not open. Sends RPC
 * function 1 with a new semaphore, the address and size (4) of a stack result word, the IOP descriptor and the slot
 * index (20 bytes); after a successful call it frees the slot (f4 = 0, without taking fileio_tableSema). It waits on the
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
    sFioFdCmd *closeCmd = &fileio_cmd.close;
    fileSlot = fileio_getDescriptor(fileDesc);
    fileio_lockCall_b(1);
    if (fileio_initFlag == 0) {
        fileio_unlockCall();
        return -1;
    }
    if (fileSlot == 0 || fileSlot->f4 == 0) {
        fileio_unlockCall();
        return -9;
    }
    closeCmd->iopFd = fileSlot->f0;
    buf[1] = 1;
    closeCmd->slot = fileSlot - fileio_descriptors;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    closeCmd->sema = sem;
    closeCmd->resultAddr = (int)&buf[8];
    closeCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 1, 0, closeCmd, 20, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    fileSlot->f4 = 0;
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Moves a file descriptor's position through the IOP file server.
 *
 * Takes the call lock (fileio_lockCall; released by fileio_unlockCall); returns -1 when the library is not initialised and -9
 * when the slot (fileio_getDescriptor) is not open. Sends RPC function 4 with a new semaphore, the address and size (4) of a
 * stack result word, the IOP descriptor, @p offset, @p whence and the slot index (0x1C bytes). For a descriptor opened with
 * flag 0x8000 (no wait) the semaphore is parked, negated in the request, in a free entry of D_00455188 (under
 * fileio_noWaitSema) and the call returns 0 without waiting; fileio_onCompletionCmd clears that entry when the IOP is done. Called by
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
  sFioLseekCmd *lseekCmd = &fileio_cmd.lseek;
  fileSlot = fileio_getDescriptor(fileDesc);
  fileio_lockCall_b(4);
  if (fileio_initFlag == 0)
  {
    fileio_unlockCall();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    fileio_unlockCall();
    return -9;
  }
  lseekCmd->iopFd = fileSlot->f0;
  lseekCmd->offset = offset;
  lseekCmd->whence = whence;
  lseekCmd->slot = fileSlot - fileio_descriptors;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  lseekCmd->resultSize = 4;
  lseekCmd->resultAddr = (int) out;
  flags = fl;
  fileio_cmd.lseek.sema = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(fileio_noWaitSema);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == FIO_NOWAIT_FREE)
      {
        D_00455188[i] = lseekCmd->sema;
        lseekCmd->sema = -lseekCmd->sema;
        break;
      }
    }

    SignalSema(fileio_noWaitSema);
  }
  if (sceSifCallRpc(&fileio_client, 4, 0, &fileio_cmd.lseek, 0x1C, fileio_recvBuffer, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    fileio_unlockCall();
    return -11;
  }
  accepted = *((int *) (((int) fileio_recvBuffer) | 0x20000000));
  fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Reads from a file descriptor into a buffer through the IOP file server.
 *
 * Takes the call lock (fileio_lockCall; released by fileio_unlockCall); returns -1 when the library is not initialised and -9
 * when the slot (fileio_getDescriptor) is not open. Sends RPC function 2 with a new semaphore, the address and size (4) of a
 * stack result word, the IOP descriptor, @p readBuf, @p readSize and the slot index (0x20 bytes). Unless the descriptor has flag
 * 0x20000000 it first writes the buffer back from the data cache, and it always writes back the request block
 * (func_00426358). The IOP fills the buffer by DMA; fileio_onCompletionCmd patches its unaligned head and tail bytes. A
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
  sFioReadCmd *readCmd = &fileio_cmd.read;
  fileSlot = fileio_getDescriptor(fileDesc);
  fileio_lockCall_b(2);
  if (fileio_initFlag == 0)
  {
    fileio_unlockCall();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    fileio_unlockCall();
    return -9;
  }
  readCmd->iopFd = fileSlot->f0;
  readCmd->buf = readBuf;
  readCmd->size = readSize;
  readCmd->slot = fileSlot - fileio_descriptors;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  readCmd->resultSize = 4;
  readCmd->resultAddr = (int) out;
  flags = fl;
  fileio_cmd.read.sema = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(fileio_noWaitSema);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == FIO_NOWAIT_FREE)
      {
        D_00455188[i] = readCmd->sema;
        readCmd->sema = -readCmd->sema;
        break;
      }
    }

    SignalSema(fileio_noWaitSema);
  }
  if (!(fl & 0x20000000))
    func_00426358(readBuf, readSize);
  func_00426358((int) readCmd, 0x20);
  if (sceSifCallRpc(&fileio_client, 2, 0, &fileio_cmd.read, 0x20, fileio_recvBuffer, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    fileio_unlockCall();
    return -11;
  }
  accepted = *((int *) (((int) fileio_recvBuffer) | 0x20000000));
  fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Writes a buffer to a file descriptor through the IOP file server.
 *
 * Takes the call lock (fileio_lockCall; released by fileio_unlockCall); returns -1 when the library is not initialised and -9
 * when the slot (fileio_getDescriptor) is not open. Sends RPC function 3 (0x30 bytes) with a new semaphore, the address and
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
  sFioWriteCmd *writeCmd = &fileio_cmd.write;
  fileSlot = fileio_getDescriptor(fileDesc);
  fileio_lockCall_b(3);
  if (fileio_initFlag == 0)
  {
    fileio_unlockCall();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    fileio_unlockCall();
    return -9;
  }
  writeCmd->iopFd = fileSlot->f0;
  writeCmd->size = writeSize;
  writeCmd->buf = writeBuf;
  writeCmd->slot = fileSlot - fileio_descriptors;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  writeCmd->resultSize = 4;
  writeCmd->resultAddr = (int) out;
  flags = fl;
  fileio_cmd.write.sema = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(fileio_noWaitSema);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == FIO_NOWAIT_FREE)
      {
        D_00455188[i] = writeCmd->sema;
        writeCmd->sema = -writeCmd->sema;
        break;
      }
    }

    SignalSema(fileio_noWaitSema);
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
  writeCmd->dataLen = n;
  for (j = 0; j < n; j++)
    writeCmd->data[j] = ((char *) writeBuf)[j];
  if (sceSifCallRpc(&fileio_client, 3, 0, &fileio_cmd.write, 0x30, fileio_recvBuffer, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    fileio_unlockCall();
    return -11;
  }
  accepted = *((int *) (((int) fileio_recvBuffer) | 0x20000000));
  fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
struct Blk88C0 { char b[0x400]; };
/**
 * Performs a control request on a file descriptor: requests 1-3 are answered on the EE, the rest go to the IOP.
 *
 * Takes the call lock (fileio_lockCall; released by fileio_unlockCall), stores @p ioctlArg in D_0052D6C8 and initialises the
 * library when needed (fileio_init); -9 when the slot (fileio_getDescriptor) is not open. Request 1 sets *@p ioctlArg to 1 when
 * any no-wait call is still pending in D_00455188 (under fileio_noWaitSema), else 0. Requests 2 and 3 copy the 32-bit or
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
    sFioIoctl2Cmd *ioctlCmd = &fileio_cmd.ioctl2;
    int one = 1; /* FAKEMATCH: stand-in for the case-1 constant */
    fileSlot = fileio_getDescriptor(fd);
    fileio_lockCall_b(5);
    D_0052D6C8 = ioctlArg;
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    if (fileSlot == 0 || fileSlot->f4 == 0) {
        fileio_unlockCall();
        return -9;
    }
    ioctlCmd->f414 = 0;
    ioctlCmd->f418 = 0;
    switch (req) {
    case 1:
        WaitSema_i(fileio_noWaitSema);
        for (i = 0; i < 32; i++) {
            if (D_00455188[i] != -1)
                break;
        }
        if (i == 32)
            *D_0052D6C8 = 0;
        else
            *D_0052D6C8 = 1;
        one = 0; /* FAKEMATCH: dead store, keeps `one` a variable */
        SignalSema(fileio_noWaitSema);
        fileio_unlockCall();
        return 0;
    case 2:
        *(int *)ioctlArg = *(int *)((int)(D_0052E390 + D_00455208 * 0x440) | 0x20000000);
        fileio_unlockCall();
        return 0;
    case 3:
        *(long *)ioctlArg = *(long *)((int)(D_0052E390 + D_00455208 * 0x440) | 0x20000000);
        fileio_unlockCall();
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
        func_00426358((int)&fileio_cmd.ioctl2, 1056);
        if (sceSifCallRpc(&fileio_client, 5, 0, &fileio_cmd.ioctl2, 1056, fileio_recvBuffer, 4, 0, 0) < 0) {
            DeleteSema(sem);
            fileio_unlockCall();
            return -11;
        }
        accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
        fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern void *func_0041605C(void *, void *, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceIoctl2_d(int, int, void *, unsigned, int, unsigned) __asm__("sceIoctl2");
/**
 * Performs an extended control request (input and output buffers) on a file descriptor through the IOP file server.
 *
 * Takes the call lock (fileio_lockCall; released by fileio_unlockCall) and initialises the library when needed
 * (fileio_init); -9 when the slot (fileio_getDescriptor) is not open, -22 when @p len or @p outLen exceeds 0x400. Copies @p len
 * bytes of @p data into the request (func_0041605C, memcpy) and sends RPC function 26 (1056 bytes, written back first)
 * with a new semaphore, the IOP descriptor, @p request, the lengths and @p outBuf; fileio_onCompletionCmd copies the reply data to
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
    sFioIoctl2Cmd *ioctl2Cmd = &fileio_cmd.ioctl2;
    fileSlot = fileio_getDescriptor(fileDesc);
    fileio_lockCall_b(26);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    if (fileSlot == 0 || fileSlot->f4 == 0) {
        fileio_unlockCall();
        return -9;
    }
    if (len > FIO_BUFFER_SIZE || outLen > FIO_BUFFER_SIZE) {
        fileio_unlockCall();
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
    func_00426358((int)&fileio_cmd.ioctl2, 1056);
    if (sceSifCallRpc(&fileio_client, 26, 0, &fileio_cmd.ioctl2, 1056, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
INCLUDE_ASM("seg/seg_325220", fileio_callPathRequest);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int fileio_callPathRequest_d(char *, int) __asm__("fileio_callPathRequest");
/**
 * Sends a file-server request that takes only a path, and waits for its result.
 *
 * Takes the call lock (fileio_lockCall with @p rpcFunc; released by fileio_unlockCall) and initialises the library when needed
 * (fileio_init). Copies @p pathName (up to 0x3FF characters, truncated) into fileio_cmd.path and sends RPC function @p rpcFunc
 * with a new semaphore and the address and size (4) of a stack result word; the request is the 12-byte header plus the
 * name and its NUL. Used for requests 6, 8, 9, 16, 18 and 21 (sceRemove, sceRmdir, sceDopen,
 * func_00429540, sceChdir, sceUmount).
 *
 * @param pathName  the path
 * @param rpcFunc  file-server RPC function number
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int fileio_callPathRequest_d(char *pathName, int rpcFunc) {
    int buf[12];
    int sem;
    int accepted;
    int n;
    sFioPathCmd *pathCmd = &fileio_cmd.path;
    fileio_lockCall_b(rpcFunc);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((pathCmd->name[0] = pathName[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((pathCmd->name[n] = pathName[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        pathCmd->name[FIO_BUFFER_SIZE - 1] = 0;
        n = FIO_BUFFER_SIZE - 1;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    pathCmd->resultAddr = (int)&buf[8];
    pathCmd->sema = sem;
    pathCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, rpcFunc, 0, &fileio_cmd.path, n + 13, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_callPathRequest(int a0, int a1);
/**
 * Removes a file: sends file-server request 6 with @p path through fileio_callPathRequest.
 *
 * @param path  the file's path
 * @return the IOP's result (0 or a negative error), or -11 when the RPC fails
 */
int sceRemove(int path) {
    return fileio_callPathRequest(path, FIO_RPC_REMOVE);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceMkdir);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceMkdir_d(char *, int) __asm__("sceMkdir");
/**
 * Creates a directory through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies @p dirPath (up to 0x3FF characters) and
 * @p dirMode into fileio_cmd.mkdir and sends RPC function 7 (the 16-byte header plus the name and its NUL) with a new
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
    sFioPathModeCmd *mkdirCmd = &fileio_cmd.mkdir;
    fileio_lockCall_b(7);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((mkdirCmd->name[0] = dirPath[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((mkdirCmd->name[n] = dirPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        mkdirCmd->name[FIO_BUFFER_SIZE - 1] = 0;
        n = FIO_BUFFER_SIZE - 1;
    }
    mkdirCmd->mode = dirMode;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    mkdirCmd->resultAddr = (int)&buf[8];
    mkdirCmd->sema = sem;
    mkdirCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 7, 0, &fileio_cmd.mkdir, n + 17, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_callPathRequest(int a0, int a1);
/**
 * Removes a directory: sends file-server request 8 with @p path through fileio_callPathRequest.
 *
 * @param path  the directory's path
 * @return the IOP's result, or -11 when the RPC fails
 */
int sceRmdir(int path) {
    return fileio_callPathRequest(path, FIO_RPC_RMDIR);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceFormat);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceFormat_d(char *, char *, char *, int) __asm__("sceFormat");
/**
 * Formats a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies @p devName and @p blockDev (each up to 0x3FF
 * characters; an empty second name when @p blockDev is 0) and @p len bytes of @p data into fileio_cmd.format; -7 when @p len
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
    sFioFormatCmd *formatCmd = &fileio_cmd.format;
    fileio_lockCall_b(0xE);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((formatCmd->name[0] = devName[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((formatCmd->name[n] = devName[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        formatCmd->name[FIO_BUFFER_SIZE - 1] = 0;
    }
    if (blockDev == 0) {
        formatCmd->name2[0] = 0;
    } else {
        n = 0;
        if ((formatCmd->name2[0] = blockDev[0]) != 0) {
            while (++n < FIO_BUFFER_SIZE) {
                if ((formatCmd->name2[n] = blockDev[n]) == 0) {
                    break;
                }
            }
        }
        if (n == FIO_BUFFER_SIZE) {
            formatCmd->name2[FIO_BUFFER_SIZE - 1] = 0;
        }
    }
    if (len > FIO_BUFFER_SIZE) {
        fileio_unlockCall();
        return -7;
    }
    for (i = 0; i < len; i++) {
        formatCmd->data[i] = data[i];
    }
    formatCmd->dataLen = len;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    formatCmd->resultAddr = (int)&buf[8];
    formatCmd->sema = sem;
    formatCmd->resultSize = 4;
    func_00426358((int)&fileio_cmd.format, 3088);
    if (sceSifCallRpc(&fileio_client, 0xE, 0, &fileio_cmd.format, 3088, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Sends file-server request 15 with one word argument and waits for the result.
 *
 * Takes the call lock and initialises the library when needed (fileio_init); the 16-byte request holds a new
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
    sFioCmd4 *adddrvCmd = &fileio_cmd.adddrv;
    fileio_lockCall_b(0xF);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    adddrvCmd->driverArg = driverArg;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    fileio_cmd.adddrv.sema = sem;
    adddrvCmd->resultAddr = (int)&buf[8];
    adddrvCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 0xF, 0, &fileio_cmd.adddrv, 0x10, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -1;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_callPathRequest(int a0, int a1);
/**
 * Sends file-server request 16 with @p path through fileio_callPathRequest.
 *
 * @param path  the name passed to the IOP
 * @return the IOP's result, or -11 when the RPC fails
 */
int func_00429540(int path) {
    return fileio_callPathRequest(path, 0x10);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDopen);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_unlockCall(void);
extern void fileio_init(void);
extern int fileio_callPathRequest(int a0, int a1);
extern int WaitSema_i(int) __asm__("WaitSema");
extern void SignalSema(int);
/**
 * Opens a directory through the IOP file server and returns a descriptor number.
 *
 * Takes and releases the call lock, initialising the library when needed (fileio_init), then allocates a descriptor
 * slot (fileio_allocDescriptor; -19 when none is free) and sends request 9 with the path through fileio_callPathRequest. A negative
 * result frees the slot and is returned; otherwise the IOP's descriptor goes to the slot's f0 (under fileio_tableSema). The
 * slot keeps only its in-use flag 0x10000000.
 *
 * @param dirPath  the directory's path
 * @return the descriptor number (slot index), or a negative error
 */
int sceDopen(int dirPath) {
    int result;
    sFioFile *dirSlot;
    fileio_lockCall_b(9);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    fileio_unlockCall();
    dirSlot = fileio_allocDescriptor();
    if (dirSlot == 0) {
        return -19;
    }
    result = fileio_callPathRequest(dirPath, FIO_RPC_DOPEN);
    if (result < 0) {
        WaitSema_i(fileio_tableSema);
        dirSlot->f4 = 0;
        SignalSema(fileio_tableSema);
        return result;
    } else {
        WaitSema_i(fileio_tableSema);
        dirSlot->f0 = result;
        result = dirSlot - fileio_descriptors;
        SignalSema(fileio_tableSema);
        return result;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceDclose);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Closes a directory descriptor through the IOP file server.
 *
 * Like sceClose: takes the call lock, returns -1 when the library is not initialised and -9 when the slot
 * (fileio_getDescriptor) is not open, sends RPC function 10 (20 bytes) with a new semaphore, a stack result word and the IOP
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
    sFioFdCmd *dcloseCmd = &fileio_cmd.dclose;
    dirSlot = fileio_getDescriptor(dirDesc);
    fileio_lockCall_b(10);
    if (fileio_initFlag == 0) {
        fileio_unlockCall();
        return -1;
    }
    if (dirSlot == 0 || dirSlot->f4 == 0) {
        fileio_unlockCall();
        return -9;
    }
    dcloseCmd->iopFd = dirSlot->f0;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    dcloseCmd->sema = sem;
    dcloseCmd->resultAddr = (int)&buf[8];
    dcloseCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 10, 0, dcloseCmd, 20, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    dirSlot->f4 = 0;
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int);
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceDread_d(int, int) __asm__("sceDread");
/**
 * Reads the next directory entry of a directory descriptor through the IOP file server.
 *
 * Takes the call lock; returns -1 when the library is not initialised and -9 when the slot (fileio_getDescriptor) is not open.
 * Sends RPC function 11 (0x20 bytes) with a new semaphore, a stack result word, the IOP descriptor and @p direntBuf;
 * fileio_onCompletionCmd copies the 0x144-byte entry to @p direntBuf. Likely bug: when sceSifCallRpc fails it calls WaitSema on the new
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
    sFioDreadCmd *dreadCmd = &fileio_cmd.dread;
    dirSlot = fileio_getDescriptor(dirDesc);
    fileio_lockCall_b(11);
    if (fileio_initFlag == 0) {
        fileio_unlockCall();
        return -1;
    }
    if (dirSlot == 0 || dirSlot->f4 == 0) {
        fileio_unlockCall();
        return -9;
    }
    dreadCmd->iopFd = dirSlot->f0;
    dreadCmd->direntBuf = direntBuf;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    dreadCmd->sema = sem;
    dreadCmd->resultAddr = (int)&buf[8];
    dreadCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 11, 0, dreadCmd, 0x20, fileio_recvBuffer, 4, 0, 0) < 0) {
        WaitSema_i(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Reads a file's status block through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies @p filePath (up to 0x3FF characters)
 * and @p statBuf into fileio_cmd.getstat and sends RPC function 12 (the 16-byte header plus the name and its NUL) with a new
 * semaphore and a stack result word; fileio_onCompletionCmd copies the 0x40-byte status block to @p statBuf.
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
    sFioGetstatCmd *getstatCmd = &fileio_cmd.getstat;
    fileio_lockCall_b(0xC);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    nameChar = (getstatCmd->name[0] = filePath[0]);
    i = 0;
    if (nameChar != 0) {
        while (++i < FIO_BUFFER_SIZE) {
            nameChar = (getstatCmd->name[i] = filePath[i]);
            if (nameChar == 0) {
                break;
            }
        }
    }
    if (i == FIO_BUFFER_SIZE) {
        getstatCmd->name[FIO_BUFFER_SIZE - 1] = 0;
        i = FIO_BUFFER_SIZE - 1;
    }
    getstatCmd->statBuf = statBuf;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    getstatCmd->resultAddr = (int)&buf[8];
    getstatCmd->sema = sem;
    getstatCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 0xC, 0, &fileio_cmd.getstat, i + 17, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceChstat_d(char *, sFioStat *, int) __asm__("sceChstat");
/**
 * Changes a file's status through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies @p filePath (up to 0x3FF characters),
 * the 0x40-byte status block @p src and @p changeMask into fileio_cmd.chstat, writes the block back from the data cache (1104
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
    sFioChstatCmd *chstatCmd = &fileio_cmd.chstat;
    fileio_lockCall_b(13);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    nameChar = (chstatCmd->name[0] = filePath[0]);
    n = 0;
    if (nameChar != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            nameChar = (chstatCmd->name[n] = filePath[n]);
            if (nameChar == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        chstatCmd->name[FIO_BUFFER_SIZE - 1] = 0;
        n = FIO_BUFFER_SIZE - 1;
    }
    chstatCmd->v = *src;
    chstatCmd->changeMask = changeMask;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    chstatCmd->sema = sem;
    chstatCmd->resultAddr = (int)&buf[8];
    chstatCmd->resultSize = 4;
    func_00426358((int)&fileio_cmd.chstat, 1104);
    if (sceSifCallRpc(&fileio_client, 13, 0, &fileio_cmd.chstat, n + 81, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceRename_d(char *, char *) __asm__("sceRename");
/**
 * Renames a file through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies both names (each up to 0x3FF
 * characters) into fileio_cmd.rename, writes the 2060-byte request back and sends RPC function 17 with a new semaphore
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
    sFioPath2Cmd *renameCmd = &fileio_cmd.rename;
    fileio_lockCall_b(17);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((renameCmd->name[0] = oldPath[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((renameCmd->name[n] = oldPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        renameCmd->name[FIO_BUFFER_SIZE - 1] = 0;
    }
    n = 0;
    if ((renameCmd->name2[0] = newPath[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((renameCmd->name2[n] = newPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        renameCmd->name2[FIO_BUFFER_SIZE - 1] = 0;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    renameCmd->resultAddr = (int)&buf[8];
    renameCmd->sema = sem;
    renameCmd->resultSize = 4;
    func_00426358((int)&fileio_cmd.rename, 2060);
    if (sceSifCallRpc(&fileio_client, 17, 0, &fileio_cmd.rename, 2060, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_callPathRequest(int a0, int a1);
/**
 * Changes the IOP file server's current directory: sends request 18 with @p path through fileio_callPathRequest.
 *
 * @param path  the new current directory
 * @return the IOP's result, or -11 when the RPC fails
 */
int sceChdir(int path) {
    return fileio_callPathRequest(path, FIO_RPC_CHDIR);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSync);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceSync_d(char *, int) __asm__("sceSync");
/**
 * Flushes a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies @p devPath (up to 0x3FF characters) and
 * @p syncFlag into fileio_cmd.sync and sends RPC function 19 (1044 bytes) with a new semaphore and a stack result word.
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
    sFioSyncCmd *syncCmd = &fileio_cmd.sync;
    fileio_lockCall_b(19);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((syncCmd->name[0] = devPath[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((syncCmd->name[n] = devPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        syncCmd->name[FIO_BUFFER_SIZE - 1] = 0;
        n = FIO_BUFFER_SIZE - 1;
    }
    syncCmd->flag = syncFlag;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    syncCmd->resultAddr = (int)&buf[8];
    syncCmd->sema = sem;
    syncCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 19, 0, &fileio_cmd.sync, 1044, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceMount_d(char *, char *, int, char *, int) __asm__("sceMount");
/**
 * Mounts a file system through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies both names (each up to 0x3FF
 * characters), @p mountFlag and @p len bytes of @p data into fileio_cmd.mount; -7 when @p len exceeds 0x400. Writes the
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
    sFioMountCmd *mountCmd = &fileio_cmd.mount;
    fileio_lockCall_b(20);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((mountCmd->name[0] = fsDevice[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((mountCmd->name[n] = fsDevice[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        mountCmd->name[FIO_BUFFER_SIZE - 1] = 0;
    }
    n = 0;
    if ((mountCmd->name2[0] = blockDev[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((mountCmd->name2[n] = blockDev[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        mountCmd->name2[FIO_BUFFER_SIZE - 1] = 0;
    }
    if (len > FIO_BUFFER_SIZE) {
        fileio_unlockCall();
        return -7;
    }
    for (i = 0; i < len; i++) {
        mountCmd->data[i] = data[i];
    }
    mountCmd->dataLen = len;
    mountCmd->flag = mountFlag;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    mountCmd->resultAddr = (int)&buf[8];
    mountCmd->sema = sem;
    mountCmd->resultSize = 4;
    func_00426358((int)&fileio_cmd.mount, 3092);
    if (sceSifCallRpc(&fileio_client, 20, 0, &fileio_cmd.mount, 3092, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_callPathRequest(int a0, int a1);
/**
 * Unmounts a file system: sends file-server request 21 with @p path through fileio_callPathRequest.
 *
 * @param path  the mounted file-system device
 * @return the IOP's result, or -11 when the RPC fails
 */
int sceUmount(int path) {
    return fileio_callPathRequest(path, FIO_RPC_UMOUNT);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceLseek64);
#ifdef SKIP_ASM
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_unlockCall(void);
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
  sFioLseek64Cmd *lseek64Cmd = &fileio_cmd.lseek64;
  fileSlot = fileio_getDescriptor(fileDesc);
  fileio_lockCall_b(22);
  if (fileio_initFlag == 0)
  {
    fileio_unlockCall();
    return -1;
  }
  if ((fileSlot == 0) || ((fl = fileSlot->f4) == 0))
  {
    fileio_unlockCall();
    return -9;
  }
  lseek64Cmd->iopFd = fileSlot->f0;
  lseek64Cmd->offset = offset;
  lseek64Cmd->whence = whence;
  lseek64Cmd->slot = fileSlot - fileio_descriptors;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  lseek64Cmd->resultAddr = (int) &res;
  lseek64Cmd->resultSize = 8;
  flags = fl;
  fileio_cmd.lseek64.sema = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(fileio_noWaitSema);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == FIO_NOWAIT_FREE)
      {
        D_00455188[i] = lseek64Cmd->sema;
        lseek64Cmd->sema = -lseek64Cmd->sema;
        break;
      }
    }

    SignalSema(fileio_noWaitSema);
  }
  if (sceSifCallRpc(&fileio_client, 22, 0, &fileio_cmd.lseek64, 0x20, fileio_recvBuffer, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    fileio_unlockCall();
    return -11;
  }
  accepted = *((int *) (((int) fileio_recvBuffer) | 0x20000000));
  fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceDevctl_d(char *, int, char *, unsigned, int, unsigned) __asm__("sceDevctl");
/**
 * Sends a device control request (input and output buffers) to a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (fileio_init). Copies the device name (up to 0x3FF
 * characters); -22 when @p len or @p outLen exceeds 0x400. Copies @p len bytes of @p data, the request number, the lengths
 * and @p outBuf into fileio_cmd.devctl, writes the 2076-byte request back and sends RPC function 23 with a new semaphore
 * and a stack result word; fileio_onCompletionCmd copies the reply data to @p outBuf. bxIop_powerOffThread (the power-off thread) sends
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
    sFioDevctlCmd *devctlCmd = &fileio_cmd.devctl;
    fileio_lockCall_b(23);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((devctlCmd->name[0] = devName[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((devctlCmd->name[n] = devName[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        devctlCmd->name[FIO_BUFFER_SIZE - 1] = 0;
    }
    if (len > FIO_BUFFER_SIZE || outLen > FIO_BUFFER_SIZE) {
        fileio_unlockCall();
        return -22;
    }
    for (i = 0; i < len; i++) {
        devctlCmd->data[i] = data[i];
    }
    devctlCmd->dataLen = len;
    devctlCmd->command = devCmd;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    devctlCmd->outLen = outLen;
    devctlCmd->resultAddr = (int)&buf[8];
    devctlCmd->resultSize = 4;
    devctlCmd->outBuf = outBuf;
    devctlCmd->sema = sem;
    func_00426358((int)&fileio_cmd.devctl, 2076);
    if (sceSifCallRpc(&fileio_client, 23, 0, &fileio_cmd.devctl, 2076, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceSymlink_d(char *, char *) __asm__("sceSymlink");
/**
 * Creates a symbolic link through the IOP file server.
 *
 * Takes the call lock (passing 17, which fileio_lockCall ignores) and initialises the library when needed
 * (fileio_init). Copies both names (each up to 0x3FF characters) into fileio_cmd.symlink and sends RPC function 24
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
    sFioPath2Cmd *symlinkCmd = &fileio_cmd.symlink;
    fileio_lockCall_b(17);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((symlinkCmd->name[0] = oldPath[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((symlinkCmd->name[n] = oldPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        symlinkCmd->name[FIO_BUFFER_SIZE - 1] = 0;
    }
    n = 0;
    if ((symlinkCmd->name2[0] = linkPath[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((symlinkCmd->name2[n] = linkPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        symlinkCmd->name2[FIO_BUFFER_SIZE - 1] = 0;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    symlinkCmd->resultAddr = (int)&buf[8];
    symlinkCmd->sema = sem;
    symlinkCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 0x18, 0, &fileio_cmd.symlink, 2060, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
extern int fileio_lockCall_b(int) __asm__("fileio_lockCall");
extern void fileio_init(void);
extern void fileio_unlockCall(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int sceReadlink_d(char *, int, unsigned) __asm__("sceReadlink");
/**
 * Reads a symbolic link's target through the IOP file server.
 *
 * Takes the call lock (passing 17, ignored) and initialises the library when needed (fileio_init). Copies the path
 * (up to 0x3FF characters), caps @p len at 0x3FF, stores @p targetBuf and the length, writes @p targetBuf back from the data cache
 * and sends RPC function 25 with a new semaphore and a stack result word; fileio_onCompletionCmd copies the returned text to
 * @p targetBuf. The send size (2060 bytes) is larger than this request's 1044-byte layout, so stale bytes of fileio_cmd go
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
    sFioReadlinkCmd *readlinkCmd = &fileio_cmd.readlink;
    fileio_lockCall_b(17);
    if (fileio_initFlag == 0) {
        fileio_init();
    }
    n = 0;
    if ((readlinkCmd->name[0] = linkPath[0]) != 0) {
        while (++n < FIO_BUFFER_SIZE) {
            if ((readlinkCmd->name[n] = linkPath[n]) == 0) {
                break;
            }
        }
    }
    if (n == FIO_BUFFER_SIZE) {
        readlinkCmd->name[FIO_BUFFER_SIZE - 1] = 0;
    }
    if (len >= FIO_BUFFER_SIZE) {
        len = FIO_BUFFER_SIZE - 1;
    }
    readlinkCmd->buf = targetBuf;
    readlinkCmd->bufLen = len;
    func_00426358(targetBuf, len);
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    readlinkCmd->resultAddr = (int)&buf[8];
    readlinkCmd->sema = sem;
    readlinkCmd->resultSize = 4;
    if (sceSifCallRpc(&fileio_client, 25, 0, &fileio_cmd.readlink, 2060, fileio_recvBuffer, 4, 0, 0) < 0) {
        DeleteSema(sem);
        fileio_unlockCall();
        return -11;
    }
    accepted = *(int *)((int)fileio_recvBuffer | 0x20000000);
    fileio_unlockCall();
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
 * Binds libkernl_iopHeapClient to server 0x80000003 (sceSifBindRpc); while the client has no server yet it spins 0x100000
 * iterations and binds again. Once bound it sets libkernl_iopHeapBindState to 0, which the heap calls (sceSifAllocIopHeap ...
 * func_0042AFE8) check. Called by the IOP boot code (bxIop_initIop, bxIop_rebootWithDnasImage) and SNDPLATFORM_init.
 *
 * @return 0, or -1 when a bind fails
 */
int sceSifInitIopHeap(void) {
    int i;
    for (;;) {
        i = sceSifBindRpc(&libkernl_iopHeapClient, 0x80000003, 0);
        if (i < 0) {
            return -1;
        }
        if (libkernl_iopHeapClient.server != 0) {
            libkernl_iopHeapBindState = 0;
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
 * When the heap server is bound (libkernl_iopHeapBindState >= 0) it sends request 1 with @p allocSize (libkernl_iopHeapArgs) and returns the reply word
 * libkernl_iopHeapResult.
 *
 * @param allocSize  bytes to allocate
 * @return the IOP address of the block, or 0 when unbound or the RPC fails
 */
int sceSifAllocIopHeap(int allocSize) {
    if (libkernl_iopHeapBindState >= 0) {
        libkernl_iopHeapArgs = allocSize;
        if (sceSifCallRpc(&libkernl_iopHeapClient, 1, 0, &libkernl_iopHeapArgs, 4, &libkernl_iopHeapResult, 4, 0, 0) >= 0) {
            return libkernl_iopHeapResult;
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
 * When the heap server is bound (libkernl_iopHeapBindState >= 0) it stores @p firstWord, @p secondWord and @p thirdWord, in that
 * order, in libkernl_iopHeapArgs, sends them (12 bytes) and returns the reply word libkernl_iopHeapResult. No caller in this build.
 *
 * @param secondWord  second word of the request
 * @param firstWord  first word of the request
 * @param thirdWord  third word of the request
 * @return the reply (an IOP address), or 0 when unbound or the RPC fails
 */
int func_0042AC10(int secondWord, int firstWord, int thirdWord) {
    int *rpcArgs;
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    rpcArgs = &libkernl_iopHeapArgs;
    rpcArgs[0] = firstWord;
    rpcArgs[1] = secondWord;
    rpcArgs[2] = thirdWord;
    if (sceSifCallRpc(&libkernl_iopHeapClient, 4, 0, rpcArgs, 0xC, &libkernl_iopHeapResult, 4, 0, 0) >= 0) {
        return libkernl_iopHeapResult;
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
 * When the heap server is bound (libkernl_iopHeapBindState >= 0) it sends request 2 with @p blockAddr and returns the reply word libkernl_iopHeapResult.
 * func_0042AD08 forwards to it.
 *
 * @param blockAddr  IOP address of the block
 * @return the server's result; 0 when unbound, -1 when the RPC fails
 */
int func_0042AC90_impl(int blockAddr) {
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    libkernl_iopHeapArgs = blockAddr;
    if (sceSifCallRpc(&libkernl_iopHeapClient, 2, 0, &libkernl_iopHeapArgs, 4, &libkernl_iopHeapResult, 4, 0, 0) < 0) {
        return -1;
    }
    return libkernl_iopHeapResult;
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
struct S2AD28 { int cmd; char name[IOP_NAME_SIZE]; };
extern struct S2AD28 libkernl_iopHeapLoadArgs;
/**
 * Loads a file into IOP memory through the heap RPC server.
 *
 * When the heap server is bound (libkernl_iopHeapBindState >= 0) it sends request 3 with @p iopAddr and the path (copied into libkernl_iopHeapLoadArgs,
 * up to 0xFB characters; the request is 4 bytes plus the name and its NUL) and returns the reply word libkernl_iopHeapResult. No
 * caller in this build.
 *
 * @param filePath  the file's path
 * @param iopAddr  IOP address to load it to
 * @return the server's result; 0 when unbound, -1 when the RPC fails
 */
int sceSifLoadIopHeap(char *filePath, int iopAddr) {
    int i;
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    i = 0;
    if ((libkernl_iopHeapLoadArgs.name[0] = filePath[0]) != 0) {
        do {
            i++;
            if (i >= IOP_NAME_SIZE) {
                break;
            }
        } while ((libkernl_iopHeapLoadArgs.name[i] = filePath[i]) != 0);
    }
    if (i == IOP_NAME_SIZE) {
        libkernl_iopHeapLoadArgs.name[IOP_NAME_SIZE - 1] = 0;
        i = IOP_NAME_SIZE - 1;
    }
    libkernl_iopHeapLoadArgs.cmd = iopAddr;
    libkernl_iopHeapLoadArgs.name[IOP_NAME_SIZE - 1] = 0;
    if (sceSifCallRpc(&libkernl_iopHeapClient, 3, 0, &libkernl_iopHeapLoadArgs, i + 5, &libkernl_iopHeapResult, 4, 0, 0) < 0) {
        return -1;
    }
    return libkernl_iopHeapResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AE18);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 5 (no arguments) and returns the reply word libkernl_iopHeapResult.
 *
 * No caller in this build.
 *
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AE18(void) {
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    if (sceSifCallRpc(&libkernl_iopHeapClient, 5, 0, 0, 0, &libkernl_iopHeapResult, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return libkernl_iopHeapResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AE88);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 6 (no arguments) and returns the reply word libkernl_iopHeapResult.
 *
 * No caller in this build.
 *
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AE88(void) {
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    if (sceSifCallRpc(&libkernl_iopHeapClient, 6, 0, 0, 0, &libkernl_iopHeapResult, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return libkernl_iopHeapResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AEF8);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 7 (no arguments) and returns the reply word libkernl_iopHeapResult.
 *
 * No caller in this build.
 *
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AEF8(void) {
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    if (sceSifCallRpc(&libkernl_iopHeapClient, 7, 0, 0, 0, &libkernl_iopHeapResult, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return libkernl_iopHeapResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AF68);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 8 with an IOP address and returns the reply word libkernl_iopHeapResult.
 *
 * No caller in this build.
 *
 * @param blockAddr  IOP address of a memory block
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AF68(int blockAddr) {
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    libkernl_iopHeapArgs = blockAddr;
    if (sceSifCallRpc(&libkernl_iopHeapClient, 8, 0, &libkernl_iopHeapArgs, 4, &libkernl_iopHeapResult, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return libkernl_iopHeapResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AFE8);
#ifdef SKIP_ASM
/**
 * Sends heap-server request 9 with an IOP address and returns the reply word libkernl_iopHeapResult.
 *
 * No caller in this build.
 *
 * @param blockAddr  IOP address of a memory block
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AFE8(int blockAddr) {
    if (libkernl_iopHeapBindState < 0) {
        return 0;
    }
    libkernl_iopHeapArgs = blockAddr;
    if (sceSifCallRpc(&libkernl_iopHeapClient, 9, 0, &libkernl_iopHeapArgs, 4, &libkernl_iopHeapResult, 4, 0, 0) < 0) {
        return 0xFFFFFFFF;
    }
    return libkernl_iopHeapResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifload_bindLoader);
#ifdef SKIP_ASM
/**
 * Binds the IOP module-loader RPC server once and reads its version.
 *
 * Does nothing when sifload_bindState is already >= 0. Otherwise binds sifload_client to server 0x80000006 (sceSifBindRpc),
 * spinning 0x100000 iterations between attempts until the client has a server, sets sifload_bindState to 0 and fetches the
 * 4-byte version with request 0xFF into sifload_serverVersion. Every module-loader call in this unit calls it first.
 *
 * @return 0; -1 when a bind fails; 0xFFFEFFFF when the version call fails
 */
int sifload_bindLoader(void) {
    int i;
    if (sifload_bindState < 0) {
        for (;;) {
            if (sceSifBindRpc(&sifload_client, 0x80000006, 0) < 0) {
                return -1;
            }
            if (sifload_client.server != 0) {
                sifload_bindState = 0;
                if (sceSifCallRpc(&sifload_client, 0xFF, 0, 0, 0, &sifload_cmd.ver, 4, 0, 0) < 0) {
                    return 0xFFFEFFFF;
                }
                sifload_serverVersion = sifload_cmd.ver;
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
INCLUDE_ASM("seg/seg_325220", sifload_isServerRejected);
#ifdef SKIP_ASM
extern char libkernl_versionTag[];
extern char *D_0045522C;
extern int memcmp(char *, char *, int);
/**
 * Returns whether the bound module-loader server's version tag is one the library does not accept.
 *
 * Same test as fileio_isServerRejected on sifload_serverVersion: 0 when it equals the library's "2710" (libkernl_versionTag) or the tag D_0045522C
 * points at (memcmp, memcmp), else 1 (the final compare of the two accepted tags always differs).
 *
 * @return 1 when the tag matches neither accepted tag, else 0
 */
int sifload_isServerRejected(void) {
    char *libVersion = libkernl_versionTag;
    char *serverVersion = sifload_serverVersion.b;
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
 * Marks the module-loader server as unbound (sifload_bindState = -1) and zeroes its version sifload_serverVersion (memset).
 *
 * The next module-loader call rebinds it (sifload_bindLoader). No caller in this build.
 *
 * @return 0
 */
int sceSifLoadFileReset(void) {
    sifload_bindState = -1;
    memset(&sifload_serverVersion, 0, 4);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifload_callBufferRequest);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
extern int sifload_isServerRejected(void);
extern void *func_0041605C(void *, void *, int);
int sifload_callBufferRequest_i(int a0, int a1, sLfData *a2, int *a3) __asm__("sifload_callBufferRequest");
/**
 * Sends a module-loader request that names an IOP memory address plus an argument block, and returns its results.
 *
 * Binds the loader when needed (sifload_bindLoader; 0xFFFF0000 on failure) and refuses an unaccepted server version
 * (sifload_isServerRejected; 0xFFFEFFFC). sifload_cmd gets @p modAddr, the argument length and the arguments (at most 0xFC bytes; a
 * length of 253 or more copies a full 0xFC block); request 6 is sent (0x200 bytes) and the 8-byte reply read back in
 * place. Called by sceSifLoadModuleBuffer and sceSifLoadStartModuleBuffer.
 *
 * @param modAddr  IOP address of the module image
 * @param argLen  bytes of @p argBlock
 * @param argBlock  argument block, or 0
 * @param resultOut  receives the reply's second word
 * @return the reply's first word, or 0xFFFEFFFF when the RPC fails
 */
int sifload_callBufferRequest_i(int modAddr, int argLen, sLfData *argBlock, int *resultOut) {
    int modId;
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (sifload_isServerRejected() != 0) {
        return 0xFFFEFFFC;
    }
    sifload_cmd.cmd.id = modAddr;
    if (argBlock != 0) {
        if (argLen >= 253) {
            *(sLfData *)sifload_cmd.cmd.args = *argBlock;
            sifload_cmd.cmd.argLen = 0xFC;
        } else {
            func_0041605C(sifload_cmd.cmd.args, argBlock, argLen);
            sifload_cmd.cmd.argLen = argLen;
        }
    } else {
        sifload_cmd.cmd.argLen = 0;
    }
    if (sceSifCallRpc(&sifload_client, 6, 0, &sifload_cmd, 0x200, &sifload_cmd, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    modId = sifload_cmd.reply.id;
    *resultOut = sifload_cmd.reply.result;
    return modId;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifStopModule);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
extern int sifload_isServerRejected(void);
extern void *func_0041605C(void *, void *, int);
int sceSifStopModule_i(int a0, int a1, sLfData *a2, int *a3) __asm__("sceSifStopModule");
/**
 * Sends module-loader request 7 for a module with an argument block, and returns its results.
 *
 * Same checks and request layout as sifload_callBufferRequest (module id, argument length and up to 0xFC bytes of arguments).
 * bxIop_unloadModule and kbReader_unloadModule call it with no arguments and then sceSifUnloadModule on the same module.
 *
 * @param modId  IOP module id
 * @param argLen  bytes of @p argBlock
 * @param argBlock  argument block, or 0
 * @param resultOut  receives the reply's second word
 * @return the reply's first word, 0xFFFF0000 / 0xFFFEFFFC as in sifload_callBufferRequest, or 0xFFFEFFFF when the RPC fails
 */
int sceSifStopModule_i(int modId, int argLen, sLfData *argBlock, int *resultOut) {
    int replyId;
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (sifload_isServerRejected() != 0) {
        return 0xFFFEFFFC;
    }
    sifload_cmd.cmd.id = modId;
    if (argBlock != 0) {
        if (argLen >= 253) {
            *(sLfData *)sifload_cmd.cmd.args = *argBlock;
            sifload_cmd.cmd.argLen = 0xFC;
        } else {
            func_0041605C(sifload_cmd.cmd.args, argBlock, argLen);
            sifload_cmd.cmd.argLen = argLen;
        }
    } else {
        sifload_cmd.cmd.argLen = 0;
    }
    if (sceSifCallRpc(&sifload_client, 7, 0, &sifload_cmd, 0x200, &sifload_cmd, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    replyId = sifload_cmd.reply.id;
    *resultOut = sifload_cmd.reply.result;
    return replyId;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifUnloadModule);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
extern int sifload_isServerRejected(void);
/**
 * Sends module-loader request 8 for a module id and returns the reply word.
 *
 * Binds the loader when needed (sifload_bindLoader) and checks its version (sifload_isServerRejected). Called after sceSifStopModule by
 * bxIop_unloadModule and kbReader_unloadModule.
 *
 * @param modId  IOP module id
 * @return the reply, 0xFFFF0000 / 0xFFFEFFFC as in sifload_callBufferRequest, or 0xFFFEFFFF when the RPC fails
 */
int sceSifUnloadModule(int modId) {
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (sifload_isServerRejected() != 0) {
        return 0xFFFEFFFC;
    }
    sifload_cmd.cmd.id = modId;
    if (sceSifCallRpc(&sifload_client, 8, 0, &sifload_cmd, 4, &sifload_cmd, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return sifload_cmd.reply.id;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifSearchModuleByName);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
extern int sifload_isServerRejected(void);
extern char *strncpy(char *, const char *, int);
/**
 * Asks the IOP module loader for a module by name (request 9) and returns the reply word.
 *
 * Binds the loader when needed (sifload_bindLoader) and checks its version (sifload_isServerRejected); the name is copied with strncpy
 * (0xFC bytes, last one forced to NUL). bxIop_installPowerOffHandler calls it with "dev9" and sets up the power-off thread only when
 * the result is not negative.
 *
 * @param modName  module name
 * @return the reply (negative when not found), 0xFFFF0000 / 0xFFFEFFFC as in sifload_callBufferRequest, or 0xFFFEFFFF
 */
int sceSifSearchModuleByName(char *modName) {
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (sifload_isServerRejected() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(sifload_cmd.cmd.name, modName, IOP_NAME_SIZE);
    sifload_cmd.cmd.name[IOP_NAME_SIZE - 1] = 0;
    if (sceSifCallRpc(&sifload_client, 9, 0, &sifload_cmd, 0x200, &sifload_cmd, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return sifload_cmd.reply.id;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B770);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
extern int sifload_isServerRejected(void);
/**
 * Sends module-loader request 10 with one word and returns the reply word.
 *
 * Binds the loader when needed (sifload_bindLoader) and checks its version (sifload_isServerRejected). No caller in this build.
 *
 * @param iopAddr  the word sent (an IOP address)
 * @return the reply, 0xFFFF0000 / 0xFFFEFFFC as in sifload_callBufferRequest, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B770(int iopAddr) {
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (sifload_isServerRejected() != 0) {
        return 0xFFFEFFFC;
    }
    sifload_cmd.cmd.id = iopAddr;
    if (sceSifCallRpc(&sifload_client, 10, 0, &sifload_cmd, 4, &sifload_cmd, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return sifload_cmd.reply.id;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadModuleBuffer);
#ifdef SKIP_ASM
extern int sifload_callBufferRequest(int a0, int a1, int a2, int *a3);
/**
 * Sends module-loader request 6 for a module image already in IOP memory, discarding the second reply word.
 *
 * Calls sifload_callBufferRequest with a stack word for its result. No caller in this build.
 *
 * @param modBuf  IOP address of the module image
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @return the result of sifload_callBufferRequest
 */
int sceSifLoadModuleBuffer(int modBuf, int argLen, int args) {
    int sp;
    return sifload_callBufferRequest(modBuf, argLen, args, &sp);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadStartModuleBuffer);
#ifdef SKIP_ASM
extern int sifload_callBufferRequest(int a0, int a1, int a2, int *a3);
/**
 * Sends module-loader request 6 for a module image already in IOP memory and passes the second reply word back.
 *
 * Calls sifload_callBufferRequest with @p modResult. No caller in this build.
 *
 * @param modBuf  IOP address of the module image
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @param modResult  receives the reply's second word
 * @return the result of sifload_callBufferRequest
 */
int sceSifLoadStartModuleBuffer(int modBuf, int argLen, int args, int *modResult) {
    return sifload_callBufferRequest(modBuf, argLen, args, modResult);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifload_callFileRequest);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
extern int sifload_isServerRejected(void);
extern char *strncpy(char *, const char *, int);
extern void *func_0041605C(void *, void *, int);
int sifload_callFileRequest_i(char *a0, int a1, sLfData *a2, int *a3, int a4) __asm__("sifload_callFileRequest");
/**
 * Sends a module-loader request that names a file plus an argument block, and returns its results.
 *
 * Binds the loader when needed (sifload_bindLoader; 0xFFFF0000 on failure) and refuses an unaccepted server version
 * (sifload_isServerRejected; 0xFFFEFFFC). sifload_cmd gets the path (strncpy, 0xFC bytes, last forced to NUL), the argument length
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
int sifload_callFileRequest_i(char *modPath, int argLen, sLfData *argBlock, int *resultOut, int rpcFunc) {
    int modId;
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (sifload_isServerRejected() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(sifload_cmd.load.path, modPath, IOP_NAME_SIZE);
    sifload_cmd.load.path[IOP_NAME_SIZE - 1] = 0;
    if (argBlock != 0) {
        if (argLen >= 253) {
            *(sLfData *)sifload_cmd.load.args = *argBlock;
            sifload_cmd.load.argLen = IOP_NAME_SIZE;
        } else {
            func_0041605C(sifload_cmd.load.args, argBlock, argLen);
            sifload_cmd.load.argLen = argLen;
        }
    } else {
        sifload_cmd.load.args[0] = 0;
        sifload_cmd.load.argLen = 0;
    }
    if (sceSifCallRpc(&sifload_client, rpcFunc, 0, &sifload_cmd, 0x200, &sifload_cmd, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    modId = sifload_cmd.reply.id;
    *resultOut = sifload_cmd.reply.result;
    return modId;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadModule);
#ifdef SKIP_ASM
extern int sifload_callFileRequest(int a0, int a1, int a2, int *a3, int a4);
/**
 * Loads an IOP module from a file (module-loader request 0), discarding the module's own result.
 *
 * Calls sifload_callFileRequest with a stack result word. bxIop_loadModule (the game's module loader, retrying on -204) and
 * SNDPLATFORM_init call it.
 *
 * @param path  the module's path
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @return the module id, or a negative error
 */
int sceSifLoadModule(int path, int argLen, int args) {
    int sp;
    return sifload_callFileRequest(path, argLen, args, &sp, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadStartModule);
#ifdef SKIP_ASM
extern int sifload_callFileRequest(int a0, int a1, int a2, int *a3, int a4);
/**
 * Loads an IOP module from a file (module-loader request 0) and passes the module's own result back.
 *
 * Calls sifload_callFileRequest with @p modResult. No caller in this build.
 *
 * @param path  the module's path
 * @param argLen  bytes of @p args
 * @param args  argument block
 * @param modResult  receives the module's start result
 * @return the module id, or a negative error
 */
int sceSifLoadStartModule(int path, int argLen, int args, int *modResult) {
    return sifload_callFileRequest(path, argLen, args, modResult, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sifload_callElfRequest);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
extern int sifload_isServerRejected(void);
extern char *strncpy(char *, const char *, int);
int sifload_callElfRequest_i(char *, char *, int *, int) __asm__("sifload_callElfRequest");
/**
 * Loads (part of) an ELF file into EE memory through the IOP module loader and returns its entry point and gp.
 *
 * Binds the loader when needed (sifload_bindLoader; 0xFFFF0000) and checks its version (sifload_isServerRejected; 0xFFFEFFFC). Copies
 * the path and the section name (strncpy, 0xFC bytes each, last forced to NUL) into sifload_cmd and sends request
 * @p rpcFunc, reading a 16-byte reply back in place. A zero first reply word gives 0xFFFEFFFD; otherwise the first two words
 * go to @p execData[0] and @p execData[1]. Called by sceSifLoadElfPart and sceSifLoadElf with request 1.
 *
 * @param elfPath  the ELF's path
 * @param sectionName  section name ("all" for the whole file)
 * @param execData  receives the entry point and gp
 * @param rpcFunc  module-loader request number
 * @return 0, 0xFFFEFFFD when nothing was loaded, or 0xFFFEFFFF when the RPC fails
 */
int sifload_callElfRequest_i(char *elfPath, char *sectionName, int *execData, int rpcFunc) {
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (sifload_isServerRejected() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(sifload_cmd.elf.path, elfPath, IOP_NAME_SIZE);
    sifload_cmd.elf.path[IOP_NAME_SIZE - 1] = 0;
    strncpy(sifload_cmd.elf.section, sectionName, IOP_NAME_SIZE);
    sifload_cmd.elf.section[IOP_NAME_SIZE - 1] = 0;
    if (sceSifCallRpc(&sifload_client, rpcFunc, 0, &sifload_cmd, 0x200, &sifload_cmd, 0x10, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    if (sifload_cmd.elf.entry == 0) {
        return 0xFFFEFFFD;
    }
    execData[0] = sifload_cmd.elf.entry;
    execData[1] = sifload_cmd.elf.gp;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadElfPart);
#ifdef SKIP_ASM
extern int sifload_callElfRequest(int a0, char *a1, int a2, int a3);
/**
 * Loads a named part of an ELF file into EE memory: sifload_callElfRequest with request 1.
 *
 * @param path  the ELF's path
 * @param secName  section name
 * @param execData  receives the entry point and gp
 * @return the result of sifload_callElfRequest
 */
int sceSifLoadElfPart(int path, char *secName, int execData) {
    return sifload_callElfRequest(path, secName, execData, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sceSifLoadElf);
#ifdef SKIP_ASM
extern char str_all[];
extern int sifload_callElfRequest(int a0, char *a1, int a2, int a3);
/**
 * Loads a whole ELF file into EE memory: sifload_callElfRequest with the section name "all" (str_all) and request 1.
 *
 * @param elfPath  the ELF's path
 * @param execData  receives the entry point and gp
 * @return the result of sifload_callElfRequest
 */
int sceSifLoadElf(int elfPath, int execData) {
    return sifload_callElfRequest(elfPath, str_all, execData, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BBF8);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
/**
 * Reads a byte, halfword or word of IOP memory through the module loader.
 *
 * Binds the loader when needed (sifload_bindLoader; 0xFFFF0000). Sizes above 2 give 0xFFFEFFFE. Sends request 3 with the
 * address and size (0x20 bytes) and stores the reply's first byte, halfword or word to @p valueOut. It does not check the
 * server version. No caller in this build.
 *
 * @param iopAddr  IOP address
 * @param valueOut  where the value is stored
 * @param valueType  size: IOP_VALUE_BYTE, IOP_VALUE_HALFWORD or IOP_VALUE_WORD (0..2)
 * @return 0, 0xFFFEFFFE for a bad size, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BBF8(int iopAddr, void *valueOut, unsigned valueType) {
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    if (valueType > IOP_VALUE_WORD) {
        return 0xFFFEFFFE;
    }
    sifload_cmd.get.data.w = iopAddr;
    sifload_cmd.get.size = valueType;
    if (sceSifCallRpc(&sifload_client, 3, 0, &sifload_cmd, 0x20, &sifload_cmd, 0x20, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    if (valueType == IOP_VALUE_BYTE) {
        *(unsigned char *)valueOut = sifload_cmd.get.data.b;
    } else if (valueType == IOP_VALUE_HALFWORD) {
        *(unsigned short *)valueOut = sifload_cmd.get.data.h;
    } else if (valueType == IOP_VALUE_WORD) {
        *(int *)valueOut = sifload_cmd.get.data.w;
    } else {
        return 0xFFFEFFFE;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BCE8);
#ifdef SKIP_ASM
extern int sifload_bindLoader(void);
/**
 * Writes a byte, halfword or word to IOP memory through the module loader.
 *
 * Binds the loader when needed (sifload_bindLoader; 0xFFFF0000). Sizes above 2 give 0xFFFEFFFE. Sends request 2 with the
 * address, the size and the value read from @p valueIn (0x20 bytes, 16-byte reply). It does not check the server version. No
 * caller in this build.
 *
 * @param iopAddr  IOP address
 * @param valueIn  the value to write
 * @param valueType  size: IOP_VALUE_BYTE, IOP_VALUE_HALFWORD or IOP_VALUE_WORD (0..2)
 * @return 0, 0xFFFEFFFE for a bad size, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BCE8(int iopAddr, void *valueIn, int valueType) {
    if (sifload_bindLoader() < 0) {
        return 0xFFFF0000;
    }
    sifload_cmd.set.addr = iopAddr;
    sifload_cmd.set.size = valueType;
    if (valueType == IOP_VALUE_BYTE) {
        sifload_cmd.set.value.b = *(unsigned char *)valueIn;
    } else if (valueType == IOP_VALUE_HALFWORD) {
        sifload_cmd.set.value.h = *(unsigned short *)valueIn;
    } else if (valueType == IOP_VALUE_WORD) {
        sifload_cmd.set.value.w = *(unsigned *)valueIn;
    } else {
        return 0xFFFEFFFE;
    }
    if (sceSifCallRpc(&sifload_client, 2, 0, &sifload_cmd.set, 0x20, &sifload_cmd.set, 0x10, 0, 0) < 0) {
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
extern struct HdrBDC8 libkernl_iopResetCmd;
extern void fBDC8_stop(void) __asm__("sceSifStopDma");
extern int fBDC8_getreg(unsigned int) __asm__("sceSifGetReg");
extern unsigned int fBDC8_setreg(unsigned int, unsigned int) __asm__("sceSifSetReg");
extern void func_00426358(int, int);
/**
 * Sends the IOP a reset command with a boot argument string over SIF DMA.
 *
 * Stops SIF DMA (sceSifStopDma) and reads the IOP's command buffer address (sceSifGetReg(0x80000000)). Builds the
 * 0x68-byte command libkernl_iopResetCmd: id 0x80000003, the argument string @p bootArg (copied without its NUL, unchecked against the
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
    libkernl_iopResetCmd.mode = mode;
    for (n = 0; bootArg[n] != 0; n++) libkernl_iopResetCmd.arg[n] = bootArg[n];
    libkernl_iopResetCmd.dest = 0;
    libkernl_iopResetCmd.size = n;
    libkernl_iopResetCmd.cid = 0x80000003;
    libkernl_iopResetCmd.dsize = 0;
    libkernl_iopResetCmd.psize = 104;
    dma.data = (int)&libkernl_iopResetCmd;
    dma.addr = reg;
    dma.size = 104;
    dma.attr = 68;
    func_00426358((int)&libkernl_iopResetCmd, 104);
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
extern void libkernl_closeTtyChannel(void);
/**
 * Returns whether the IOP has finished rebooting.
 *
 * When bit 0x40000 of SIF register 4 (sceSifGetReg) is set it clears libkernl_ttyOpenFlag (libkernl_closeTtyChannel) and returns 1.
 * bxIop_rebootWithDnasImage polls it after rebooting the IOP with sceSifRebootIop.
 *
 * @return 1 when the reboot is done, else 0
 */
int sceSifSyncIop(void) {
    if (sceSifGetReg(4) & 0x40000) {
        libkernl_closeTtyChannel();
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
 * sceSifResetIop (mode 0). bxIop_rebootWithDnasImage retries it with the DNAS image until it succeeds.
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
INCLUDE_ASM("seg/seg_325220", libkernl_createMallocSemas);
#ifdef SKIP_ASM
extern int __sce_sema_id;
extern int libkernl_secondSemaId;
extern int CreateSema(int *a0);
/**
 * Creates two semaphores (max and initial count 1) and stores their ids in __sce_sema_id and libkernl_secondSemaId.
 *
 * __sce_sema_id is the one the recursive lock __malloc_lock / __malloc_unlock waits on and signals. Called by _InitSys
 * at start-up.
 */
void libkernl_createMallocSemas(void) {
    int firstSemaParam[8];
    int secondSemaParam[8];
    firstSemaParam[1] = 1;
    firstSemaParam[2] = 1;
    secondSemaParam[1] = 1;
    secondSemaParam[2] = 1;
    __sce_sema_id = CreateSema(firstSemaParam);
    libkernl_secondSemaId = CreateSema(secondSemaParam);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C120);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * Loads the number into $v1 and executes `syscall`. sysTable_init first installs the word-copy routine sysTable_kernelCopyWords as
 * syscall 90; sysTable_getEntry then uses this stub to read a word of kernel memory.
 */
SYSCALL_STUB(func_0042C120, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sysTable_kernelCopyWords);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by sysTable_init as kernel syscall 90.
 *
 * Runs in kernel mode as the syscall handler, so it can read and write kernel memory for the EE-side stubs.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int sysTable_kernelCopyWords(int *dst, int *src, unsigned int size) {
    unsigned int n = size >> 2;
    unsigned int i;
    for (i = 0; i < n; i++) {
        *dst++ = *src++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sysTable_kernelFindWord);
#ifdef SKIP_ASM
/**
 * Returns the address of the first word in [@p cursor, @p end) equal to @p value, or 0.
 *
 * Installed by sysTable_init as kernel syscall 131 so that the search runs over kernel memory. The word at @p cursor is
 * read before the bound check, so a failed search reads the word at @p end.
 *
 * @param cursor  first word to test
 * @param end  end of the range
 * @param value  the word to look for
 * @return the matching address, or 0
 */
int *sysTable_kernelFindWord(int *cursor, int *end, int value) {
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
INCLUDE_ASM("seg/seg_325220", sysTable_getEntry);
#ifdef SKIP_ASM
extern void func_0042C120(void *a0, int a1, int a2);
/**
 * Returns entry @p syscallNum of the kernel's syscall table: copies the word at sysTable_base + 4 * @p syscallNum with syscall 90
 * (func_0042C120).
 *
 * sysTable_base is the table base sysTable_init found. No caller in this build.
 *
 * @param syscallNum  syscall number
 * @return the handler address in that slot
 */
int sysTable_getEntry(int syscallNum) {
    int handlerAddr;
    func_0042C120(&handlerAddr, sysTable_base + syscallNum * 4, 4);
    return handlerAddr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", sysTable_init);
#ifdef SKIP_ASM
extern int sysTable_syscallList[];
extern int FindAddress(int, int, void *);
extern void func_0042C2F0(int, int);
/**
 * Installs two helper syscalls and locates the kernel's syscall table, storing its base in sysTable_base.
 *
 * Installs the pairs of sysTable_syscallList with syscall 116 (func_0042C2F0): sysTable_kernelFindWord as syscall 131 (word search) and
 * sysTable_kernelCopyWords as syscall 90 (word copy). Then searches kernel memory 0x80000000-0x80080000 with FindAddress for words
 * holding those two handler addresses: each hit minus the slot offset (131 * 4 = 524, 90 * 4 = 360) is a candidate
 * table base, and the lower candidate is advanced to its next hit until both agree. Called by _InitSys at
 * start-up.
 */
void sysTable_init(void) {
    int searchHit;
    int copyHit;
    unsigned x;
    unsigned y;
    func_0042C2F0(sysTable_syscallList[0], sysTable_syscallList[1]);
    func_0042C2F0(sysTable_syscallList[2], sysTable_syscallList[3]);
    searchHit = FindAddress(0x80000000, 0x80080000, (void *)sysTable_kernelFindWord);
    x = searchHit - 524;
    copyHit = FindAddress(0x80000000, 0x80080000, (void *)sysTable_kernelCopyWords);
    y = copyHit - 360;
    while (x != y) {
        if (x < y) {
            searchHit = FindAddress(searchHit + 4, 0x80080000, (void *)sysTable_kernelFindWord);
            x = searchHit - 524;
        } else {
            copyHit = FindAddress(copyHit + 4, 0x80080000, (void *)sysTable_kernelCopyWords);
            y = copyHit - 360;
        }
    }
    sysTable_base = x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C2F0);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by sysTable_init to install the pairs of sysTable_syscallList.
 */
SYSCALL_STUB(func_0042C2F0, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", _InitSys);
#ifdef SKIP_ASM
extern void libkernl_createMallocSemas(void);
extern void sysTable_init(void);
extern void alarmPatch_install(void);
extern void InitThread(void);
extern void osdPatch_install(void);
extern void tlbPatch_install(void);
/**
 * Runs the kernel library's start-up initialisation.
 *
 * Creates the two library semaphores (libkernl_createMallocSemas), installs the helper syscalls and finds the syscall table
 * (sysTable_init), patches the alarm syscalls (alarmPatch_install), starts the wake-up helper thread (InitThread),
 * patches the OSD configuration syscall when needed (osdPatch_install) and installs the TLB syscall patch (tlbPatch_install).
 * Called by the program entry func_00100008 before main.
 */
void _InitSys(void) {
    libkernl_createMallocSemas();
    sysTable_init();
    alarmPatch_install();
    InitThread();
    osdPatch_install();
    tlbPatch_install();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C340);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by osdPatch_install.
 */
SYSCALL_STUB(func_0042C340, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C350);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * osdPatch_install installs the word-copy routine osdPatch_kernelCopyWords as syscall 90 and then copies its kernel patch with this
 * stub.
 */
SYSCALL_STUB(func_0042C350, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", osdPatch_kernelCopyWords);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by osdPatch_install as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int osdPatch_kernelCopyWords(int *dst, int *src, unsigned int size) {
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
 * osdPatch_install first points syscall 91 at the patch it copied to 0x80074000 and then asks it, through this stub, for
 * the handler address of a syscall.
 */
SYSCALL_STUB(func_0042C398, 91)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", osdPatch_isNeeded);
#ifdef SKIP_ASM
extern void GetOsdConfigParam(unsigned *p);
extern void SetOsdConfigParam(unsigned *p);
/**
 * Returns whether the kernel ignores bits 13-15 of the OSD configuration word.
 *
 * Reads the configuration (GetOsdConfigParam), writes it back with those bits set to 1, reads it again and restores the
 * original. Returns 1 when the read-back field is 0, i.e. the kernel does not keep it. osdPatch_install patches the kernel
 * in that case.
 *
 * @return 1 when the field was not stored, else 0
 */
int osdPatch_isNeeded(void) {
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
INCLUDE_ASM("seg/seg_325220", osdPatch_install);
#ifdef SKIP_ASM
struct P42C410 { int a; int b; };
extern struct P42C410 D_004559F8[];
extern char D_00455250[];
extern int osdPatch_isNeeded(void);
extern void func_0042C340(int, int);
extern void func_0042C350(int, char *, int);
extern void FlushCache(int);
extern int func_0042C398(int);
/**
 * Patches the kernel's OSD configuration syscalls when the kernel drops the extended configuration bits.
 *
 * Only when osdPatch_isNeeded reports so: installs osdPatch_kernelCopyWords as syscall 90 (first pair of D_004559F8), copies 1960
 * bytes of patch code from D_00455250 to kernel address 0x80074000 with it, flushes the data and instruction caches
 * (FlushCache 0 and 2), installs the patch's entry as syscall 91 (second pair) and then installs, for the number in the
 * third pair (0xFFFFC402), the handler address syscall 91 returns for it. Called by _InitSys.
 */
void osdPatch_install(void) {
    unsigned i;
    if (osdPatch_isNeeded() != 0) {
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
 * execArgs_copyToKernel installs the byte-copy routine execArgs_kernelCopyBytes as syscall 90 and copies the exec arguments into kernel
 * memory with this stub.
 */
SYSCALL_STUB(func_0042C4C0, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", execArgs_kernelCopyBytes);
#ifdef SKIP_ASM
/**
 * Copies @p size bytes from @p src to @p dst; installed by execArgs_copyToKernel as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int execArgs_kernelCopyBytes(unsigned char *dst, unsigned char *src, unsigned int size) {
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
 * Used by execArgs_copyToKernel.
 */
SYSCALL_STUB(func_0042C500, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", execArgs_copyToKernel);
#ifdef SKIP_ASM
extern int execArgs_syscallEntry[];
extern int strlen(char *);
extern int func_0042C4C0(int, void *, int);
extern void func_0042C500(int, int);
int execArgs_copyToKernel_d(char *, int, char **) __asm__("execArgs_copyToKernel");
/**
 * Copies a program name and its arguments into the kernel's argument area for the exec calls.
 *
 * Installs the byte-copy routine execArgs_kernelCopyBytes as syscall 90 (execArgs_syscallEntry) and caps @p argc at 15. The area at
 * execArgs_kernelArea holds a table of string addresses followed, from +0x40, by the strings: first @p progName, then each argv
 * string, each with its NUL; the addresses and strings are written with syscall 90 (func_0042C4C0). Called by
 * ExecPS2, LoadExecPS2 and ExecOSD.
 *
 * @param progName  program name (first string)
 * @param argc  number of argument strings
 * @param argv  the argument strings
 * @return the kernel address of the copied name (execArgs_kernelArea + 0x40)
 */
int execArgs_copyToKernel_d(char *progName, int argc, char **argv) {
    int pos;
    int ptrSlot = execArgs_kernelArea;
    int ret;
    int len;
    char **argvCursor;
    pos = ptrSlot + 0x40;
    ret = pos;
    func_0042C500(execArgs_syscallEntry[0], execArgs_syscallEntry[1]);
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
extern void tlbPatch_restoreLayout_v(void) __asm__("tlbPatch_restoreLayout");
/**
 * Restores the boot TLB layout: jumps to tlbPatch_restoreLayout.
 *
 * Called before control leaves the program (ExecPS2, LoadExecPS2, Exit, ExecOSD).
 */
void func_0042C628(void) {
    tlbPatch_restoreLayout_v();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", ExecPS2);
#ifdef SKIP_ASM
extern char str_empty_c[];
extern void execArgs_copyToKernel_v(char *, int, int) __asm__("execArgs_copyToKernel");
extern void func_0042C628(void);
extern int _ExecPS2(int, int, int, int);
/**
 * Starts code already in memory with new arguments: copies the arguments to the kernel and calls the syscall 7 stub.
 *
 * Copies an empty name plus @p argVector's @p argCount strings into the kernel argument area (execArgs_copyToKernel), restores the TLB
 * layout (func_0042C628) and calls the syscall 7 stub (seg_3249A0) with the entry point, gp, the argument count and the
 * kernel copy of the argument table (execArgs_kernelArea + 4). No caller in this build.
 *
 * @param entryAddr  entry point
 * @param gpValue  gp value
 * @param argCount  argument count
 * @param argVector  argument strings
 * @return the syscall's result (it does not return on success)
 */
int ExecPS2(int entryAddr, int gpValue, int argCount, int argVector) {
    execArgs_copyToKernel_v(str_empty_c, argCount, argVector);
    func_0042C628();
    return _ExecPS2(entryAddr, gpValue, argCount, execArgs_kernelArea + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", LoadExecPS2);
#ifdef SKIP_ASM
extern int execArgs_copyToKernel(int a0, int a1);
extern void _LoadExecPS2(int a0, int a1, int a2);
/**
 * Loads and runs another program: copies its name and arguments to the kernel and calls the syscall 6 stub.
 *
 * The argument strings arrive in the third argument register, which this definition passes on unnamed to
 * execArgs_copyToKernel. Restores the TLB layout (func_0042C628) and calls the syscall 6 stub (seg_3249A0) with the kernel copy
 * of the name, the count and the kernel argument table (execArgs_kernelArea + 4). Called by shutdownAndLoadExec.
 *
 * @param progPath  path of the program
 * @param argCount  argument count
 */
void LoadExecPS2(int progPath, int argCount) {
    int kernelPath = execArgs_copyToKernel(progPath, argCount);
    func_0042C628();
    _LoadExecPS2(kernelPath, argCount, execArgs_kernelArea + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", Exit);
#ifdef SKIP_ASM
extern void KExit(int);
/**
 * Ends the program: restores the TLB layout (func_0042C628) and calls the syscall 4 stub (seg_3249A0) with @p status.
 *
 * The program entry func_00100008 jumps here when main returns, and func_001001C8 jumps here; _kill and the TLB
 * set-up tlbPatch_programTlb (on overflow, status 1) call it.
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
extern int execArgs_copyToKernel_3(char *a0, int a1, int a2) __asm__("execArgs_copyToKernel");
extern void _ExecOSD(int a0, int a1);
/**
 * Returns to the console's OSD (browser) with arguments: copies them to the kernel and calls the syscall 123 stub.
 *
 * Copies an empty name plus the argument strings into the kernel argument area (execArgs_copyToKernel), restores the TLB
 * layout (func_0042C628) and calls the syscall 123 stub (seg_3249A0) with the count and the kernel argument table
 * (execArgs_kernelArea + 4). No caller in this build.
 *
 * @param argCount  argument count
 * @param argVector  argument strings
 */
void ExecOSD(int argCount, int argVector) {
    execArgs_copyToKernel_3(str_empty_c, argCount, argVector);
    func_0042C628();
    _ExecOSD(argCount, execArgs_kernelArea + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C760);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 116 with the caller's arguments (syscall number, handler address): installs a syscall
 * handler.
 *
 * Used by alarmPatch_install.
 */
SYSCALL_STUB(func_0042C760, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C770);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 90 with the caller's arguments (destination, source, size).
 *
 * alarmPatch_install installs the word-copy routine alarmPatch_kernelCopyWords as syscall 90 and copies its kernel patch with this stub.
 */
SYSCALL_STUB(func_0042C770, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", alarmPatch_kernelCopyWords);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by alarmPatch_install as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int alarmPatch_kernelCopyWords(int *dst, int *src, unsigned int size) {
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
 * alarmPatch_install first points syscall 91 at the patch it copied to 0x80076000 and then asks it, through this stub, for
 * the handler address of each alarm syscall.
 */
SYSCALL_STUB(func_0042C7B8, 91)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", alarmPatch_install);
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
 * Only when bit 0x100 of the timer 3 mode register (0x10001810) is clear: installs alarmPatch_kernelCopyWords as syscall 90 (first
 * pair of D_00456180), copies 1856 bytes of patch code from D_00455A18 to kernel address 0x80076000 and 40 bytes from
 * D_00456158 to 0x82000, flushes the data and instruction caches (FlushCache 0 and 2), installs the patch's entry as
 * syscall 91 and then installs, for each of the syscalls 252, 254, 253, 255, 300 and 8, the handler address syscall 91
 * returns for it. Called by _InitSys.
 */
void alarmPatch_install(void) {
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
extern char libkernl_deci2WorkArea[];
extern int Deci2Call(int a0, int *a1);
int f8A0(unsigned short a0, int a1, int a2) __asm__("sceDeci2Open");
/**
 * Opens a DECI2 socket for a protocol (DECI2 kernel call 1).
 *
 * Passes the protocol, @p handlerArg, the event handler and the uncached address of the work area libkernl_deci2WorkArea to Deci2Call(1).
 * The serial console set-up libkernl_openTtyChannel calls it.
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
    buf[3] = (int)libkernl_deci2WorkArea | 0x20000000;
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
 * Called by the console send routine libkernl_ttySend.
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
 * Called by the console send routine libkernl_ttySend after sceDeci2ReqSend.
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
 * Called by the console's DECI2 event handler libkernl_onTtyDeci2Event.
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
 * Called by the console's DECI2 event handler libkernl_onTtyDeci2Event.
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
INCLUDE_ASM("seg/seg_325220", libkernl_deci2Puts);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Prints a string through the DECI2 kernel console: Deci2Call(0x10) with the string's address.
 *
 * The console write routine libkernl_bufferPrintChar calls it.
 *
 * @param str  the string
 * @return the kernel's result
 */
int libkernl_deci2Puts(int str) {
    return Deci2Call(0x10, &str);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_setTlbExceptionHandler);
#ifdef SKIP_ASM
extern int libkernl_tlbExceptionHandler;
extern void libkernl_tlbExceptionEntry(void);
extern void SetVTLBRefillHandler(int, void (*)(void));
/**
 * Installs a user handler for TLB exceptions (codes 1-3).
 *
 * Stores @p excHandler in libkernl_tlbExceptionHandler and points the TLB-refill vectors for exception codes 1, 2 and 3 at the handler stub
 * libkernl_tlbExceptionEntry (SetVTLBRefillHandler), which saves the registers and calls the stored function. No caller in this
 * build.
 *
 * @param excHandler  function called on a TLB exception
 * @return @p excHandler
 */
int libkernl_setTlbExceptionHandler(int excHandler) {
    libkernl_tlbExceptionHandler = excHandler;
    SetVTLBRefillHandler(1, libkernl_tlbExceptionEntry);
    SetVTLBRefillHandler(2, libkernl_tlbExceptionEntry);
    SetVTLBRefillHandler(3, libkernl_tlbExceptionEntry);
    return excHandler;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", libkernl_setExceptionHandler);
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
unsigned libkernl_setExceptionHandler(int excCode, int excHandler) {
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
 * tlbPatch_install installs the word-copy routine tlbPatch_kernelCopyWords as syscall 90 and copies its kernel patch with this stub.
 */
SYSCALL_STUB(func_0042CB68, 90)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", tlbPatch_kernelCopyWords);
#ifdef SKIP_ASM
/**
 * Copies @p size / 4 words from @p src to @p dst; installed by tlbPatch_install as kernel syscall 90.
 *
 * @param dst  destination
 * @param src  source
 * @param size  bytes to copy (rounded down to whole words)
 * @return 0
 */
// The bnez delay slot stays an unfilled nop with the addiu before it: ee-gcc 2.9 + cc_native's fill pass.
int tlbPatch_kernelCopyWords(int *dst, int *src, unsigned int size) {
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
 * tlbPatch_install first points syscall 91 at the patch it copied to 0x80075000 and then asks it, through this stub, for
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
 * Used by tlbPatch_install.
 */
SYSCALL_STUB(func_0042CBC0, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", tlbPatch_install);
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
 * Installs tlbPatch_kernelCopyWords as syscall 90 (first pair of D_00456540), copies 816 bytes of patch code from D_004561C0 to
 * kernel address 0x80075000, flushes the data and instruction caches (FlushCache 0 and 2), installs the patch's entry
 * as syscall 91 and the handler at 0x0042D100 as syscall 84, then installs, for each of the syscalls 85-89 (the TLB
 * entry and scratchpad calls), the handler address syscall 91 returns for it. Finally stores syscall 91's answer for 3
 * in execArgs_kernelArea, the kernel area execArgs_copyToKernel writes exec arguments to. _InitSys jumps to it last.
 */
void tlbPatch_install(void) {
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
    execArgs_kernelArea = func_0042CBB0(3);
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
 * tlbPatch_programTlb.
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
INCLUDE_ASM("seg/seg_325220", tlbPatch_restoreLayout);
#ifdef SKIP_ASM
extern int GetMemorySize(void);
/* The main memory of a retail console, 32 MB ([U] the doc below). Name ours. */
#define RETAIL_MEMORY_SIZE (32 * 1024 * 1024)
extern int tlbPatch_programTlb(void);
extern int _InitTLB(void);
/**
 * Restores the TLB layout: on a 32 MB console (GetMemorySize() == 0x2000000) through tlbPatch_programTlb, otherwise through
 * the kernel's _InitTLB.
 *
 * func_0042C628 jumps here before the program exits or starts another one.
 *
 * @return the result of the call made
 */
int tlbPatch_restoreLayout(void) {
    if (GetMemorySize() == RETAIL_MEMORY_SIZE) {
        return tlbPatch_programTlb();
    }
    return _InitTLB();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", tlbPatch_programTlb);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "fakematch.h"
struct sTlbCfg_4248 { int a, b, c, cnt; int *t1; int *t2; int *t3; };
extern struct sTlbCfg_4248 D_004567F0;
extern char D_0049AD98[], D_0049ADD0[], D_0049ADE8[], D_0049AE00[];
extern int kprintf_pf(const char *, ...) __asm__("kprintf");
extern void Exit(int);
extern int func_0042CCB8_tlb(int, int, int, int, int) __asm__("func_0042CCB8");
/**
 * Programs the TLB from the layout table D_004567F0 and fills the remaining entries with unmapped pages.
 *
 * Prints the layout ("# TLB spad=0 kernel=1:%d default=%d:%d extended=%d:%d", kprintf) and clears the Wired
 * register. Writes the kernel entries, then the default entries (four words each: PageMask, EntryHi, EntryLo0,
 * EntryLo1) with syscall 86 (func_0042CCB8), records their total in the table's count word and makes them wired, then
 * writes the extended entries when there are any. Each group that would pass entry 48 prints "# TLB over flow (1)",
 * "(2)" or "(3)" and exits with status 1 (Exit). Entries left up to 47 get EntryHi 0xE0000000 + index * 0x2000
 * and no mapping. Called by tlbPatch_restoreLayout.
 *
 * @return the number of programmed entries shifted left by 13 (not the count itself)
 */
// FAKEMATCH: `register int i FM_REG("$25")` pins the TLB index to $25, where retail keeps it across the calls to the
// func_0042CCB8 syscall stub (no allocator picks a call-clobbered register there, so the original most likely declared
// it as a register variable); plain form 88.93%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int tlbPatch_programTlb(void) {
    struct sTlbCfg_4248 *tlbLayout = &D_004567F0;
    register int i FM_REG("$25");
    int n, k, *entryWords;
    kprintf_pf(D_0049AD98, tlbLayout->a - 1, tlbLayout->a, tlbLayout->a + tlbLayout->b - 1, tlbLayout->a + tlbLayout->b, tlbLayout->a + tlbLayout->b + tlbLayout->c - 1);
    MTC0_WIRED_4248(0);
    n = tlbLayout->a;
    i = 0;
    if (n > 48) {
        kprintf_pf(D_0049ADD0);
        Exit(1);
    }
    entryWords = tlbLayout->t1;
    for (; i < n; i++, entryWords += 4) func_0042CCB8_tlb(i, entryWords[0], entryWords[1], entryWords[2], entryWords[3]);
    tlbLayout = &D_004567F0;
    n = i + tlbLayout->b;
    if (n > 48) {
        kprintf_pf(D_0049ADE8);
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
            kprintf_pf(D_0049AE00);
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
INCLUDE_ASM("seg/seg_325220", libkernl_tlbExceptionEntry);
#ifdef SKIP_ASM
/**
 * EE exception handler stub for the TLB exceptions libkernl_setTlbExceptionHandler hooks (SetVTLBRefillHandler, codes 1-3).
 *
 * Saves every GPR, HI/LO, HI1/LO1 and SA in the frame D_00530380, loads Status, Cause, EPC and BadVAddr into $4-$7
 * (the frame in $8), keeps EPC in D_005305A8 and erets with IE cleared to D_0042D0C0. That calls the user handler
 * libkernl_tlbExceptionHandler on the frame as stack, makes syscall -0x54, restores the saved registers and EPC and returns from the
 * exception with eret. The second entry D_0042D200, the stub libkernl_setExceptionHandler installs for codes 1-13, saves the same
 * way (COP0 register 23 also in $8, the frame in $9), erets to D_0042D2FC and calls the handler D_004564F8[ExcCode];
 * if that returns it stops at a break.
 */
// PORT: PS2-only EE exception handler (eret, raw $at/$k0, never returns); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl libkernl_tlbExceptionEntry\n.type libkernl_tlbExceptionEntry, @function\nlibkernl_tlbExceptionEntry:\n"
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
    "lui $1, %hi(libkernl_tlbExceptionHandler)\n"
    "lw $1, %lo(libkernl_tlbExceptionHandler)($1)\n"
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
    ".size libkernl_tlbExceptionEntry, . - libkernl_tlbExceptionEntry\n"
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
 * The printf core libkernl_formatOutput calls it.
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
 * Uses the inline unsigned divide (__udivmoddi4). Called by _strtol_r, the printf core libkernl_formatOutput and other library
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
