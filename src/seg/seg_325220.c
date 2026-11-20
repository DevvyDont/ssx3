#include "common.h"

// PORT: PS2 kernel syscall stub: the syscall number goes in $3. Defined under a second C name with an
// asm label because this file declares these symbols with their argument lists elsewhere.
#define SYSCALL_STUB(name, n) \
    void name##_syscall(void) __asm__(#name); \
    void name##_syscall(void) { register int v __asm__("$3") = (n); __asm__ volatile("syscall" : : "r"(v)); }

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
   func_00426B48 / func_00426F08 in this unit are the implementation. */
typedef struct {
    int rpcd[4];
    unsigned int command;
    void *buff;
    void *cbuff;
    void (*end_function)(void *);
    void *end_param;
    void *server;
} sSifClient;
extern int func_00426B48(sSifClient *cd, unsigned int rpc_number, int mode);
extern int func_00426D18(sSifClient *cd, int fno, int mode, void *send, int ssize, void *recv, int rsize,
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


/* wake-up request queue of the helper thread started by func_00424B78 (512 {type, thread id} entries) */
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
extern int D_00455170;          /* helper thread id (func_00424B78) */
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
void func_00424220(void) {
    D_00455168 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424230);
#ifdef SKIP_ASM
extern int func_0042C078_r(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
void func_00424230(void) {
    int intrEnabled;
    intrEnabled = func_0042C078_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) func_0042C0C0();
    while (!(*(volatile int *)0x1000F000 & 4));
    intrEnabled = func_0042C078_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004242C0);
#ifdef SKIP_ASM
extern void func_0042C0C0(void);
extern void SetVSyncFlag_vs(volatile int *, volatile unsigned long *) __asm__("SetVSyncFlag");
unsigned long func_004242C0(void) {
    volatile int flag;
    volatile unsigned long gsCsr;
    int intrEnabled;
    flag = 0;
    SetVSyncFlag_vs(&flag, &gsCsr);
    intrEnabled = func_0042C078_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) func_0042C0C0();
    while (!(*(volatile int *)0x1000F000 & 4) && flag == 0);
    intrEnabled = func_0042C078_r();
    *(volatile int *)0x1000F000 = 4;
    SYNC_4248();
    if (intrEnabled) func_0042C0C0();
    return gsCsr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424368);
#ifdef SKIP_ASM
extern int func_00425260(void);
extern int func_00425040(int, int);
int func_00424368(int a0, int a1, int a2) {
    if (a0 == 1 || a0 == 2) {
        if (D_00455168 == 0) {
            if (func_00425260() == 0) {
                return -1;
            }
            D_00455168 = 1;
        }
        return func_00425040(a1, a2);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004243E8);
#ifdef SKIP_ASM
extern int func_00425260(void);
extern int func_00425190(int, int);
int func_004243E8(int a0, int a1, int a2) {
    if (a0 == 0) {
        if (D_00455168 == 0) {
            if (func_00425260() == 0) {
                return -1;
            }
            D_00455168 = 1;
        }
        return func_00425190(a1, a2);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424460);
#ifdef SKIP_ASM
extern int *func_00415FB8(void);
int func_00424460(void) {
    *func_00415FB8() = 5;
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424488);
#ifdef SKIP_ASM
int func_00424488(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424490);
#ifdef SKIP_ASM
int func_00424490(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424498);
#ifdef SKIP_ASM
int func_00424498(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004244A0);
#ifdef SKIP_ASM
extern unsigned D_0045516C;
extern unsigned EndOfHeap(void);
extern int *func_00415FB8(void);
unsigned func_004244A0(unsigned n) {
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
        *func_00415FB8() = 12;
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
int func_00424550(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424558);
#ifdef SKIP_ASM
long func_00424558(int fileDesc, char *statBuf) {
    *(int *)(statBuf + 4) = 0x2000;
    *(long *)(statBuf + 0x48) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424570);
#ifdef SKIP_ASM
int func_00424570(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424578);
#ifdef SKIP_ASM
extern void func_0042C6E8(int a0);
int func_00424578(int pid, int sigNum) {
    if (pid == 1) {
        func_0042C6E8(sigNum);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004245A0);
#ifdef SKIP_ASM
extern int *func_00415FB8(void);
int func_004245A0(void) {
    *func_00415FB8() = 5;
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004245C8);
#ifdef SKIP_ASM
extern int *func_00415FB8(void);
int func_004245C8(void) {
    *func_00415FB8() = 5;
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
void func_004245F0(unsigned lo, unsigned hi) {
    int i;
    for (i = 0; i < 0x1000; i += 0x40) {
        unsigned t;
        CACHE_4248(0x10, 0, i);
        MFC0_TAGLO_4248(t);
        t = (t & 0xFFFFF000) + i;
        if (!(t < lo) && !(hi < t)) CACHE_4248(20, 0, i);
        CACHE_4248(0x10, 1, i);
        MFC0_TAGLO_4248(t);
        t = (t & 0xFFFFF000) + i;
        if (!(t < lo) && !(hi < t)) CACHE_4248(20, 1, i);
        SYNC_4248();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424698);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern void func_004245F0(unsigned, unsigned);
void func_00424698(unsigned startAddr, unsigned endAddr) {
    int s;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    func_004245F0(startAddr & ~63, endAddr & ~63);
    if (s) func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424718);
#ifdef SKIP_ASM
extern void func_004245F0(unsigned, unsigned);
void func_00424718(unsigned startAddr, unsigned endAddr) {
    func_004245F0(startAddr & ~63, endAddr & ~63);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424730);
#ifdef SKIP_ASM
void func_00424730(unsigned lo, unsigned hi) {
    int i;
    for (i = 0; i < 0x1000; i += 0x40) {
        unsigned t;
        CACHE_4248(0x10, 0, i);
        MFC0_TAGLO_4248(t);
        t = (t & 0xFFFFF000) + i;
        if (!(t < lo) && !(hi < t)) CACHE_4248(22, 0, i);
        CACHE_4248(0x10, 1, i);
        MFC0_TAGLO_4248(t);
        t = (t & 0xFFFFF000) + i;
        if (!(t < lo) && !(hi < t)) CACHE_4248(22, 1, i);
        SYNC_4248();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004247D8);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern void func_00424730(unsigned, unsigned);
void func_004247D8(unsigned startAddr, unsigned endAddr) {
    int s;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    func_00424730(startAddr & ~63, endAddr & ~63);
    if (s) func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424858);
#ifdef SKIP_ASM
extern void func_00424730(unsigned, unsigned);
void func_00424858(unsigned startAddr, unsigned endAddr) {
    func_00424730(startAddr & ~63, endAddr & ~63);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424870);
#ifdef SKIP_ASM
int func_00424870(void) {
    int s;
    MFC0_STATUS_4248(s);
    return (s ^ 1) & 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424880);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern int _DisableIntc(int);
int func_00424880(int cause) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = _DisableIntc(cause);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004248E8);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern int _EnableIntc(int);
int func_004248E8(int cause) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = _EnableIntc(cause);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424950);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern int _DisableDmac(int);
int func_00424950(int channel) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = _DisableDmac(channel);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004249B8);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern int _EnableDmac(int);
int func_004249B8(int channel) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = _EnableDmac(channel);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A20);
#ifdef SKIP_ASM
extern void _iEnableIntc(void);
void func_00424A20(void) {
    _iEnableIntc();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A40);
#ifdef SKIP_ASM
extern void _iDisableIntc(void);
void func_00424A40(void) {
    _iDisableIntc();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A60);
#ifdef SKIP_ASM
extern void _iEnableDmac(void);
void func_00424A60(void) {
    _iEnableDmac();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A80);
#ifdef SKIP_ASM
extern void _iDisableDmac(void);
void func_00424A80(void) {
    _iDisableDmac();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424AA0);
#ifdef SKIP_ASM
extern char D_0049AAE0[];
extern int WaitSema_i(int) __asm__("WaitSema");
extern void RotateThreadReadyQueue(int);
extern void WakeupThread(int);
extern void SuspendThread(int);
void func_00424AA0(unsigned *p) {
    unsigned i;
    unsigned j;
    unsigned char *t = (unsigned char *)p + 8;
    unsigned char *v = (unsigned char *)p + 9;
    for (;;) {
        WaitSema_i(D_0052B3A0);
        i = *p & 0x1FF;
        j = i << 1;
        *p = i + 1;
        switch (*(t + j)) {
        case 0:
            WakeupThread(*(v + j));
            break;
        case 1:
            RotateThreadReadyQueue(*(v + j));
            break;
        case 2:
            SuspendThread(*(v + j));
            break;
        default:
            func_00425C28((int)D_0049AAE0);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424B78);
#ifdef SKIP_ASM
extern char D_0052AFA0[];
extern char D_004A30F0[];
extern int CreateSema(int *p);
extern int CreateThread(int *);
extern void StartThread(int, void *);
extern int GetThreadId(void);
extern void ChangeThreadPriority(int, int);
extern void DeleteSema(int);
extern void func_00424AA0(unsigned *);
int func_00424B78_i(void) __asm__("func_00424B78");
int func_00424B78_i(void) {
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
INCLUDE_ASM("seg/seg_325220", func_00424C50);
#ifdef SKIP_ASM
extern void iSignalSema(int);
extern int _iWakeupThread(void);
// PORT: PS2 kernel syscall (number in $3, result in $2).
#define SYSCALL_RET_4C50(n, r) do { register int r_ __asm__("$2"); __asm__ volatile("li $3, " #n "\n\tsyscall" : "=r"(r_) : : "$3"); r = r_; } while (0)
int func_00424C50_r(int a0) __asm__("func_00424C50");
int func_00424C50_r(int a0) {
    int id;
    unsigned i;
    SYSCALL_RET_4C50(-47, id);
    if (id != a0) return _iWakeupThread();
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
INCLUDE_ASM("seg/seg_325220", func_00424CE8);
#ifdef SKIP_ASM
extern void iSignalSema(int);
int func_00424CE8(unsigned a0) {
    unsigned i;
    if (a0 >= 0x80 || D_00455170 == 0) {
        return -1;
    }
    i = D_0052B3A8.idx & 0x1FF;
    D_0052B3A8.idx = i + 1;
    D_0052B3A8.e[i].a = 1;
    D_0052B3A8.e[i].b = a0;
    iSignalSema(D_0052B3A0);
    return a0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424D68);
#ifdef SKIP_ASM
extern int _iSuspendThread(void);
int func_00424D68_r(int a0) __asm__("func_00424D68");
int func_00424D68_r(int a0) {
    int id;
    unsigned i;
    SYSCALL_RET_4C50(-47, id);
    if (id != a0) return _iSuspendThread();
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
sRing *func_00424E00(int a0) {
    D_0052B7C0[0] = a0;
    D_0052B7C0[1] = 0;
    D_0052B7C0[3] = (int)&D_0052B7C0[4];
    D_0052B7C0[2] = (int)&D_0052B7C0[4];
    return (sRing *)D_0052B7C0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424E28);
#ifdef SKIP_ASM
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
extern int rd_4EA8(int, unsigned char *, unsigned short) __asm__("func_0042C968");
extern int wr_4EA8(int, char *, unsigned short) __asm__("func_0042C9A0");
extern char D_0049AB08[], D_0049AB30[], D_0049AB48[], D_0049AB60[];
void func_00424EA8(int cmd, int n, sTty *s) {
    switch (cmd) {
    case 1:
    case 2:
        if (n != 0) {
            if ((unsigned)(s->pos + n) > 320)
                pf_4EA8(D_0049AB08);
            {
                int pos = s->pos;
                n = rd_4EA8(s->fd, s->buf + pos, n);
            }
            if (n < 0)
                pf_4EA8(D_0049AB30);
            s->pos += n;
        } else {
            unsigned short *hdr = (unsigned short *)s->buf;
            for (n = 12; n < *hdr; n++) {
                *s->ring->wr = s->buf[n];
                func_00424E28(s->ring);
            }
            s->pos = 0;
        }
        break;
    case 3: {
        int r = wr_4EA8(s->fd, s->wp, s->wlen);
        if (r < 0) {
            pf_4EA8(D_0049AB48, r);
            s->fC = 0;
            break;
        }
        s->wp += r;
        s->wlen -= r;
        break;
    }
    case 4:
        if (s->wlen != 0)
            pf_4EA8(D_0049AB60, s->wlen);
        s->fC = 0;
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
extern int f5040_send(int, char) __asm__("func_0042C910");
extern void f5040_poll(int) __asm__("func_0042C940");
int func_00425040_d(char *buf, int len) __asm__("func_00425040");
int func_00425040_d(char *buf, int len) {
    struct Pkt_5040 *pkt;
    char *p;
    int i = 0;
    int n = 0;
    if (D_0052B8D0.fC != 0)
        return -1;
    func_0042C078();
    pkt = (struct Pkt_5040 *)((int)D_0052B900 | 0x20000000);
    D_0052B8D0.fC = 1;
    D_0052B8D0.wp = (char *)pkt;
    p = (char *)pkt + 12;
    while (len-- != 0) {
        if (*buf == '\n') {
            *p++ = '\r';
            if (++i >= 256)
                break;
        }
        *p++ = *buf++;
        n++;
        if (++i >= 256)
            break;
    }
    D_0052B8D0.wlen = i + 12;
    pkt->f0 = D_0052B8D0.wlen;
    if (f5040_send(D_0052B8D0.fd, pkt->f7) < 0) {
        D_0052B8D0.fC = 0;
        func_0042C0C0();
        return -1;
    }
    while (D_0052B8D0.fC != 0)
        f5040_poll(D_0052B8D0.fd);
    func_0042C0C0();
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425190);
#ifdef SKIP_ASM
int func_00425190(int a0, int n) {
    char *buf = (char *)a0;
    int i;
    int j;
    char *p;
    for (i = 0; i < n; i = j) {
        p = buf + i;
        j = i + 1;
        while (D_0052B8E8->count == 0) {
        }
        *p = *D_0052B8D0.ring->rd;
        func_00424E68(D_0052B8D0.ring);
        if (*p == '\n' || *p == '\r') {
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
extern int f5260_open(int, void *, void *) __asm__("func_0042C8A0");
int func_00425260(void) {
    struct Pkt_5260 *p;
    FlushCache(0);
    D_0052B8D0.fd = f5260_open(528, &D_0052B8D0, func_00424EA8);
    if (D_0052B8D0.fd < 0) return 0;
    D_0052B8D0.fC = 0;
    D_0052B8D0.wlen = 0;
    D_0052B8D0.pos = 0;
    D_0052B8D0.buf = (unsigned char *)((int)D_0052BA40 | 0x20000000);
    p = (struct Pkt_5260 *)((int)D_0052B900 | 0x20000000);
    D_0052B8D0.wp = (char *)p;
    p->f4 = 528;
    p->f6 = 69;
    p->f7 = 72;
    p->f2 = 0;
    p->f8 = 0;
    D_0052B8D0.ring = func_00424E00(0x100);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425320);
#ifdef SKIP_ASM
int func_00425320_i(int a0) __asm__("func_00425320");
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
void func_00425358(int c) {
    int idx;
    if (D_00455174 >= 0x7E) {
        D_00455174 = 0;
        D_0052BB80[0x7F] = 0;
        func_0042CA58_v(D_0052BB80);
    }
    idx = D_00455174;
    if (c == 10) {
        D_00455174 = 0;
        D_0052BB80[idx] = c;
        D_0052BB80[idx + 1] = 0;
        func_0042CA58_v(D_0052BB80);
    } else {
        D_00455174 = idx + 1;
        D_0052BB80[idx] = c;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425408);
#ifdef SKIP_ASM
extern void func_00425320(int c);
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
extern double func_0042D380_d(double) __asm__("func_0042D380");
extern double func_00425440_d(double) __asm__("func_00425440");
extern void (*D_00455178)(int);
extern char D_0049AB80[];
extern char D_0049AB88[];
extern char D_0049AB90[];
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
        double r = func_00425440_d(func_0042D380_d(x));
        func_00425C28((int)D_0049AB80, r);
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
extern int func_0042C078_p(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
extern long func_0042D470(long, long);
extern long func_0040FCB0(long, long);
extern unsigned long func_0042DAD8(unsigned long, unsigned long);
extern unsigned long func_00411538(unsigned long, unsigned long);
extern double fptodp(float);
extern void func_004254D0(double x);
// printf core: fmt + va_list (8-byte argument slots); 64-bit div/mod go through the soft helpers.
void func_00425638_f(char *fmt, char *ap) __asm__("func_00425638");
void func_00425638_f(char *fmt, char *ap) {
    char buf[32];
    char *pad;
    char *p;
    char *str;
    int lmod;
    int w;
    int c;
    long v;
    unsigned long u;
    float f;
    int st;
    st = func_0042C078_p();
    while (*fmt) {
        pad = 0;
        lmod = 0;
        if (*fmt == '%') {
        again:
            switch (*++fmt) {
            case '0':
                w = fmt[1] - '0';
                c = fmt[2];
                if ((unsigned char)w < 10) {
                    if ((unsigned)(c - '0') < 10) {
                        w = w * 10 + (c - '0');
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
                    u = *(unsigned long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    u = *(unsigned short *)(ap - 8);
                } else {
                    ap += 8;
                    u = *(unsigned int *)(ap - 8);
                }
                p = &buf[31];
                *p = 0;
                if (u == 0) {
                    *--p = '0';
                } else {
                    while (u) {
                        *--p = (u & 7) + '0';
                        u >>= 3;
                    }
                }
                if (pad && pad < p)
                    p = pad;
                while (*p)
                    D_00455178(*p++);
                break;
            case 'x':
                if (lmod == 'l') {
                    ap += 8;
                    u = *(unsigned long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    u = *(unsigned short *)(ap - 8);
                } else {
                    ap += 8;
                    u = *(unsigned int *)(ap - 8);
                }
                p = &buf[31];
                *p = 0;
                if (u == 0) {
                    *--p = '0';
                } else {
                    while (u) {
                        unsigned long d = u & 0xF;
                        if (d < 10)
                            *--p = d + '0';
                        else
                            *--p = d + 'a' - 10;
                        u >>= 4;
                    }
                }
                if (pad && pad < p)
                    p = pad;
                while (*p)
                    D_00455178(*p++);
                break;
            case 'd':
                if (lmod == 'l') {
                    ap += 8;
                    v = *(long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    v = *(short *)(ap - 8);
                } else {
                    ap += 8;
                    v = *(int *)(ap - 8);
                }
                p = &buf[31];
                *p = 0;
                if (v == 0) {
                    *--p = '0';
                } else {
                    if (v < 0) {
                        v = -v;
                        D_00455178('-');
                    }
                    while (v) {
                        *--p = func_0042D470(v, 10) + '0';
                        v = func_0040FCB0(v, 10);
                    }
                }
                if (pad && pad < p)
                    p = pad;
                while (*p)
                    D_00455178(*p++);
                break;
            case 'u':
                if (lmod == 'l') {
                    ap += 8;
                    u = *(unsigned long *)(ap - 8);
                } else if (lmod == 'h') {
                    ap += 8;
                    u = *(unsigned short *)(ap - 8);
                } else {
                    ap += 8;
                    u = *(unsigned int *)(ap - 8);
                }
                p = &buf[31];
                *p = 0;
                if (u == 0) {
                    *--p = '0';
                } else {
                    while (u) {
                        *--p = func_0042DAD8(u, 10) + '0';
                        u = func_00411538(u, 10);
                    }
                }
                if (pad && pad < p)
                    p = pad;
                while (*p)
                    D_00455178(*p++);
                break;
            case 'e':
            case 'f':
                ap += 8;
                f = *(float *)(ap - 8);
                if (f == 0.0f)
                    D_00455178('0');
                else
                    func_004254D0(fptodp(f));
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
                    for (p = str; *p; p++)
                        D_00455178(*p);
                }
                break;
            case 'c':
                ap += 8;
                v = *(char *)(ap - 8);
                D_00455178(v);
                break;
            }
        } else {
            D_00455178(*fmt);
        }
        fmt++;
    }
    if (st)
        func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425C28);
#ifdef SKIP_ASM
// not matched: varargs wrapper (a0 passes through, $5 = &spill at sp+0x58 passed as ap). hand-rolled va_start gives right sd $5..$11
// but also emits swc1 $f12..$f18 saves (target has none) -> size mismatch (18 vs 14 insns).
extern void func_00425638(int a0, char *ap);
void func_00425C28(int a0, ...) {
    char *ap;
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_00425638(a0, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425C60);
#ifdef SKIP_ASM
extern void (*D_00455178)(int);
extern void func_00425358(int c);
extern void func_00425638(int a0, char *ap);
void func_00425C60(int a0, ...) {
    char *ap;
    void (*old)(int) = D_00455178;
    D_00455178 = func_00425358;
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_00425638(a0, ap);
    D_00455178 = old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425CC0);
#ifdef SKIP_ASM
void func_00425CC0(int *packet, int *cmdData) {
    ((int *)cmdData[7])[packet[4]] = packet[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425CE0);
#ifdef SKIP_ASM
void func_00425CE0(int *packet, int *cmdData) {
    cmdData[2] = packet[4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425CF0);
#ifdef SKIP_ASM
int func_00425CF0(int a0) {
    return D_0052BE00[a0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425D08);
#ifdef SKIP_ASM
int func_00425D08(int a0, int a1) {
    D_0052BE00[a0] = a1;
    return a1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425D28);
#ifdef SKIP_ASM
int *func_00425D28(void) {
    return D_0052BCD8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425D38);
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
extern int f5D38_adddh(int, int (*)(), int) __asm__("func_00423AB0");
extern void sceSifSetDChain(void);
extern int sceSifGetReg(int a0);
extern unsigned int f5D38_getreg(unsigned int) __asm__("sceSifGetReg");
extern int sceSifSetReg(int, int);
extern int f5D38_send(int, void *, int, void *, void *, int) __asm__("func_004261B0");
void func_00425D38(void) {
    int i;
    func_0042C078();
    if (D_0045517C) {
        func_0042C0C0();
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
    func_0042C0C0();
    FlushCache(0);
    if (*(volatile int *)0x1000E010 & 0x20)
        *(volatile int *)0x1000E010 = 0x20;
    if (!(*(volatile int *)0x1000C000 & 0x100))
        sceSifSetDChain();
    D_0052BCD4 = f5D38_adddh(5, func_00426230, 0);
    func_004249B8(5);
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
INCLUDE_ASM("seg/seg_325220", func_00425FB8);
#ifdef SKIP_ASM
extern int func_00424950(int a0);
extern int RemoveDmacHandler(int a0, int a1);
extern int D_0045517C;
extern int D_0052BCD4;
void func_00425FB8(void) {
    func_00424950(5);
    RemoveDmacHandler(5, D_0052BCD4);
    D_0045517C = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425FF0);
#ifdef SKIP_ASM
int func_00425FF0(int a0, int a1) {
    int old = D_0052BCD8[5];
    D_0052BCD8[5] = a0;
    D_0052BCD8[6] = a1;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426008);
#ifdef SKIP_ASM
int func_00426008(int a0, int a1) {
    int old = D_0052BCD8[3];
    D_0052BCD8[3] = a0;
    D_0052BCD8[4] = a1;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426020);
#ifdef SKIP_ASM
void func_00426020(int a0, int a1, int a2) {
    int off = a0 << 3;
    if (a0 < 0) {
        a0 = (int)D_0052BCE4;
    } else {
        a0 = (int)D_0052BCEC;
    }
    off += a0;
    ((int *)off)[0] = a1;
    ((int *)off)[1] = a2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426050);
#ifdef SKIP_ASM
void func_00426050(int a0) {
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
int func_00426078_d(int a0, int a1, int *a2, unsigned a3, int a4, int a5, int a6) {
    sSifDmaData ent[2];
    int n;
    if (a3 - 0x10 > 0x60) {
        return 0;
    }
    n = 0;
    if (a6 > 0) {
        ent[0].data = a4;
        n = 1;
        ent[0].addr = a5;
        a2[1] = a5;
        a2[0] = *(unsigned char *)a2 | (a6 << 8);
        ent[0].size = a6;
        ent[0].attr = 0;
        if (a1 & 4) {
            func_00426358(a4, a6);
        }
    } else {
        a2[1] = 0;
        a2[0] = *(unsigned char *)a2;
    }
    ent[n].data = (int)a2;
    ent[n].addr = D_0052BCE0;
    ent[n].size = a3;
    a2[2] = a0;
    *(unsigned char *)a2 = a3;
    ent[n].attr = 68;
    func_00426358((int)a2, a3);
    n++;
    if (a1 & 1) {
        return isceSifSetDma(ent, n);
    }
    return sceSifSetDma(ent, n);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004261B0);
#ifdef SKIP_ASM
extern int func_00426078(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
int func_004261B0(int cmdId, int packet, int packetSize, int extraSrc, int extraDest, int extraSize) {
    return func_00426078(cmdId, 0, packet, packetSize, extraSrc, extraDest, extraSize);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004261F0);
#ifdef SKIP_ASM
extern int func_00426078(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
int func_004261F0(int cmdId, int packet, int packetSize, int extraSrc, int extraDest, int extraSize) {
    return func_00426078(cmdId, 1, packet, packetSize, extraSrc, extraDest, extraSize);
}
#endif

INCLUDE_ASM("seg/seg_325220", func_00426230);

//100%
INCLUDE_ASM("seg/seg_325220", func_00426358);
#ifdef SKIP_ASM
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
INCLUDE_ASM("seg/seg_325220", func_004265A8);
#ifdef SKIP_ASM
extern void func_00425FB8(void);
extern int D_00455180;
void func_004265A8(void) {
    func_00425FB8();
    D_00455180 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004265D0);
#ifdef SKIP_ASM
extern int func_0042C078_rq(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
sSifRpcPkt *f65D0(sSifRpcState *s) __asm__("func_004265D0");
sSifRpcPkt *f65D0(sSifRpcState *s) {
    sSifRpcPkt *e;
    int i;
    int n;
    int c;
    int id;
    int seq;
    func_0042C078_rq();
    n = s->pkt_table_len;
    i = 0;
    e = s->pkt_table;
    while (i < n) {
        if (!(e->f10 & 1)) {
            e->f10 = (i << 16) | 5;
            c = s->pid;
            id = c + 1;
            s->pid = id;
            if (id == 1) {
                s->pid = c + 2;
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
INCLUDE_ASM("seg/seg_325220", func_00426678);
#ifdef SKIP_ASM
void func_00426678(unsigned int *packet) {
    packet[6] = 0;
    packet[4] &= 0xFFFFFFFE;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426698);
#ifdef SKIP_ASM
char *f6698(sSifRpcState *a0) __asm__("func_00426698");
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
void f6708(struct A6708 *a) {
    struct Q6708 *q;
    switch (a->f20) {
    case 0x8000000A:
        q = a->q;
        if (q->f1C != 0) {
            ((void (*)(int))q->f1C)(q->f20);
        }
        break;
    case 0x80000009:
        q = a->q;
        q->f24 = a->f24;
        ((int *)q)[5] = a->f28;
        ((int *)q)[6] = a->f2C;
        break;
    case 0x8000000B:
        break;
    }
    q = a->q;
    if (q->f8 >= 0) {
        iSignalSema(q->f8);
    }
    func_00426678_k(q->f0);
    q->f0 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004267C0);
#ifdef SKIP_ASM
extern int func_004261F0(int a0, int a1, int a2, int a3, int a4, int a5);
extern void func_00423B80(int a0, int (*a1)(int, int, int *), int *a2);
int func_004267C0(int alarmId, int alarmTime, int *packet) {
    if (func_004261F0(0x80000008, (int)packet, 0x40, packet[9], packet[10], packet[11]) == 0) {
        func_00423B80(10, func_004267C0, packet);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426820);
#ifdef SKIP_ASM
extern int func_00426698(sSifRpcState *a0);
extern int func_004266C8(sSifRpcState *a0, int a1);
extern int func_004261F0(int a0, int a1, int a2, int a3, int a4, int a5);
extern void func_00423B80(int a0, int (*a1)(int, int, int *), int *a2);
extern int func_004267C0(int a0, int a1, int *p);
extern int func_004266C8_u(sSifRpcState *, unsigned) __asm__("func_004266C8");
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
    if (func_004261F0(0x80000008, (int)reply, 0x40, request[8], request[9], request[10]) == 0) {
        func_00423B80(10, func_004267C0, reply);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004268F0);
#ifdef SKIP_ASM
struct S68F0 { struct S68F0 *f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; int f28; };
extern struct S68F0 *func_004265D0_p(sSifRpcState *) __asm__("func_004265D0");
extern void func_00426678_j(struct S68F0 *) __asm__("func_00426678");
extern int CreateSema_j(int *) __asm__("CreateSema");
extern int WaitSema_j(int) __asm__("WaitSema");
extern int func_004261B0_j(int, int, int, int, int, int) __asm__("func_004261B0");
int func_004268F0_d(struct S68F0 *, int, int, int, int) __asm__("func_004268F0");
int func_004268F0_d(struct S68F0 *a0, int a1, int a2, int a3, int a4) {
    int buf[8];
    struct S68F0 *t;
    t = func_004265D0_p(&D_0052D680);
    if (t == 0) {
        return -1;
    }
    a0->f0 = t;
    a0->f4 = t->f18;
    t->f20 = a1;
    t->f24 = a2;
    t->f28 = a3;
    t->f14 = (int)t;
    t->f1C = (int)a0;
    if (!(a4 & 1)) {
        buf[1] = 1;
        buf[2] = 0;
        a0->f8 = CreateSema_j(buf);
        if (a0->f8 < 0) {
            func_00426678_j(t);
            return -3;
        }
        if (func_004261B0_j(0x8000000C, (int)t, 0x40, 0, 0, 0) == 0) {
            func_00426678_j(t);
            DeleteSema(a0->f8);
            return -2;
        }
        WaitSema_j(a0->f8);
        DeleteSema(a0->f8);
        return 0;
    }
    a0->f8 = -1;
    if (func_004261B0_j(0x8000000C, (int)t, 0x40, 0, 0, 0) == 0) {
        func_00426678_j(t);
        return -2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426A48);
#ifdef SKIP_ASM
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
extern void func_004261F0_v(int, int *, int, int, int, int) __asm__("func_004261F0");
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
    func_004261F0_v(0x80000008, reply, 0x40, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426B48);
#ifdef SKIP_ASM
struct S6B48 { struct S6B48 *f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; };
extern struct S6B48 *func_004265D0(sSifRpcState *);
extern void func_00426678_i(struct S6B48 *) __asm__("func_00426678");
extern int CreateSema(int *);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00426B48_d(struct S6B48 *, int, int) __asm__("func_00426B48");
int func_00426B48_d(struct S6B48 *a0, int a1, int a2) {
    int buf[8];
    struct S6B48 *t;
    a0->f10 = 0;
    a0->f24 = 0;
    t = func_004265D0(&D_0052D680);
    if (t == 0) {
        return -1;
    }
    a0->f0 = t;
    a0->f4 = t->f18;
    t->f20 = a1;
    t->f14 = (int)t;
    t->f1C = (int)a0;
    if (!(a2 & 1)) {
        buf[1] = 1;
        buf[2] = 0;
        a0->f8 = CreateSema(buf);
        if (a0->f8 < 0) {
            func_00426678_i(t);
            return -3;
        }
        if (func_004261B0(0x80000009, (int)t, 0x40, 0, 0, 0) == 0) {
            func_00426678_i(t);
            DeleteSema(a0->f8);
            return -2;
        }
        WaitSema_i(a0->f8);
        DeleteSema(a0->f8);
        return 0;
    }
    a0->f8 = -1;
    if (func_004261B0(0x80000009, (int)t, 0x40, 0, 0, 0) == 0) {
        func_00426678_i(t);
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
extern void func_00424C50(int);
void func_00426C88(struct A26C88 *a) {
    struct E26C88 *e = a->e;
    struct B26C88 *b = e->owner;
    if (b->head == 0) {
        b->head = (int)e;
    } else {
        ((struct E26C88 *)b->tail)->next = (int)e;
    }
    b->tail = (int)e;
    {
        int t1 = a->f14;
        int t2 = a->f1C;
        e->f20 = t1;
        e->f1C = t2;
    }
    e->f24 = a->f20;
    e->fC = a->f24;
    e->f28 = a->f28;
    e->f2C = a->f2C;
    e->f30 = a->f30;
    e->f34 = a->f10;
    if (b->f0 >= 0 && b->f4 == 0) {
        func_00424C50(b->f0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426D18);
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
extern int f6D18_send(int, void *, int, void *, void *, int) __asm__("func_004261B0");
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
 * buffer (func_004261B0). With mode bit 0 set (no wait) the client's semaphore is -1, the packet's reply mode is 1 only
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
int func_00426D18(sSifClient *client, int fno, int mode, void *send, int ssize, void *recv, int rsize,
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
INCLUDE_ASM("seg/seg_325220", func_00426F08);
#ifdef SKIP_ASM
/**
 * Returns whether an RPC client's last call is still in progress.
 *
 * Reads the client's packet pointer (word 0): returns 1 when the packet exists, its id (+0x18) still equals the id the
 * client recorded (word 1) and its in-use bit (bit 0 of +0x10) is set; 0 otherwise. Polled by the pad, memory card and
 * USB keyboard library code after no-wait calls of func_00426D18.
 *
 * @param client  the RPC client data
 * @return 1 while the call is pending, else 0
 */
int func_00426F08(int *client) {
    int *packet = (int *)client[0];
    if (packet == 0 || client[1] != packet[6] || !(packet[4] & 1)) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426F48);
#ifdef SKIP_ASM
extern int func_0042C078_r(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
/**
 * Initialises an RPC server queue for a thread and appends it to the RPC state's queue list.
 *
 * With interrupts disabled (func_0042C078 ... func_0042C0C0) it sets the queue's thread id (f0) to @p a1, clears its
 * active flag (f4), its server list, the request start and end pointers (fC, f10) and its link, then appends it to the
 * end of D_0052D680.queues (the list func_00426A48 searches when a bind request names a server). No caller in this
 * build.
 *
 * @param n  the queue to set up
 * @param a1  id of the thread that serves the queue (the one woken when a request arrives)
 */
void func_00426F48(sSifQueue *n, int a1) {
    sSifQueue *p;
    sSifQueue *h;
    func_0042C078_r();
    n->f0 = a1;
    n->f4 = 0;
    n->servers = 0;
    n->fC = 0;
    n->f10 = 0;
    h = D_0052D680.queues;
    n->next = 0;
    if (h == 0) {
        D_0052D680.queues = n;
    } else {
        p = h;
        for (; p->next != 0; p = p->next)
            ;
        p->next = n;
    }
    func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426FE0);
#ifdef SKIP_ASM
struct N6FE0 { int f0; int f4; int f8; int fC; int f10; int f14; char pad[0x20]; struct N6FE0 *next; struct N6FE0 *f3C; struct S6FE0 *f40; };
struct S6FE0 { int f0; int f4; struct N6FE0 *head; };
extern int func_0042C078_r(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
/**
 * Registers an RPC server (id, service function and buffers) on a server queue.
 *
 * With interrupts disabled (func_0042C078 ... func_0042C0C0) it fills the server record: id (+0), service function
 * (+4), receive buffer (+8), cancel function (+0x10) and its buffer (+0x14), the owning queue (+0x40), and clears its
 * two link words (+0x38, +0x3C); then appends it to the end of the queue's server list (+8, linked through +0x38). No
 * caller in this build.
 *
 * @param n  the server record to register
 * @param a1  server id that clients bind to
 * @param a2  service function called for each request
 * @param a3  buffer that receives the request data
 * @param a4  cancel function
 * @param a5  buffer for the cancel function
 * @param a6  the queue the server is added to
 */
void func_00426FE0(struct N6FE0 *n, int a1, int a2, int a3, int a4, int a5, struct S6FE0 *a6) {
    struct N6FE0 *p;
    struct N6FE0 *h;
    func_0042C078_r();
    n->f3C = 0;
    n->next = 0;
    n->f0 = a1;
    h = a6->head;
    n->f4 = a2;
    n->f8 = a3;
    n->f10 = a4;
    n->f14 = a5;
    n->f40 = a6;
    if (h == 0) {
        a6->head = n;
    } else {
        p = h;
        for (; p->next != 0; p = p->next)
            ;
        p->next = n;
    }
    func_0042C0C0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004270B0);
#ifdef SKIP_ASM
struct N70B0 { char pad[0x38]; struct N70B0 *next; };
struct S70B0 { int f0; int f4; struct N70B0 *head; };
extern int func_0042C078_r(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
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
struct N70B0 *func_004270B0(struct N70B0 *x, struct S70B0 *list) {
    struct N70B0 *cursor;
    func_0042C078_r();
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
    func_0042C0C0();
    return cursor;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427148);
#ifdef SKIP_ASM
extern int f7148a(void) __asm__("func_0042C078");
extern void f7148b(void) __asm__("func_0042C0C0");
sSifQueue *f7148(sSifQueue *x) __asm__("func_00427148");
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
    sSifQueue *p;
    f7148a();
    p = D_0052D680.queues;
    if (p == x) {
        D_0052D680.queues = p->next;
    } else {
        for (; p != 0; p = p->next) {
            if (p->next == x) {
                p->next = x->next;
                break;
            }
        }
    }
    f7148b();
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004271D8);
#ifdef SKIP_ASM
struct N4271D8 { char pad[0x3C]; int next; };
struct S4271D8 { char pad[4]; int f4; char pad2[4]; struct N4271D8 *head; };
extern void func_0042C078(void);
extern void func_0042C0C0(void);
/**
 * Takes the next pending request (server record) off a server queue.
 *
 * With interrupts disabled it pops the head of the queue's request list (+0xC, linked through +0x3C) and sets the
 * queue's active flag (+4) to 1, or to 0 when the list is empty. Only func_004273F8 calls it.
 *
 * @param arg  the server queue (as an int)
 * @return the server record with a request to run, or 0
 */
int func_004271D8(int arg) {
    struct S4271D8 *a0 = (struct S4271D8 *)arg;
    struct N4271D8 *n;
    func_0042C078();
    n = a0->head;
    if (n == 0) {
        a0->f4 = 0;
    } else {
        struct N4271D8 *t = (struct N4271D8 *)n->next;
        a0->f4 = 1;
        a0->head = t;
    }
    func_0042C0C0();
    return (int)n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427230);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`ASSERT_7230(e != 0)`, `do { } while (0)`) after the lookup, and the stand-in local `w = 0x8000000A` (with `v = a0->f1C` read first) for the two header stores; plain form 98.10% (98.01% without the ASSERT only, 97.95% without `w` only). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct W7230 { int f0; int (*f4)(int, int, int); int f8; int fC; int f10; int f14; int f18; int f1C; int f20; int f24; int f28; int f2C; int f30; int f34; };
struct R7230 { int f0; int f4; int f8; int fC; int f10; int f14; int f18; int f1C; int f20; };
extern void func_00426358(int, int);
extern void func_0042C078_w(void) __asm__("func_0042C078");
extern int func_0042C0C0_w(void) __asm__("func_0042C0C0");
extern struct R7230 *func_004266C8_w(sSifRpcState *, unsigned) __asm__("func_004266C8");
extern struct R7230 *func_00426698_w(sSifRpcState *, unsigned) __asm__("func_00426698");
extern int func_004261B0_w(int, void *, int, int, int, int) __asm__("func_004261B0");
void func_00427230_d(struct W7230 *) __asm__("func_00427230");
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
 * result DMA'd to the caller's buffer (+0x28) through func_004261B0, retrying until it is accepted. Otherwise it clears
 * the packet's id and in-use words and DMAs the result (when any) and then the packet itself to the caller's packet
 * address (+0x20) with sceSifSetDma, spinning 0x100000 iterations between refused attempts. Only func_004273F8 calls
 * it.
 *
 * @param a0  the server record holding the request
 */
void func_00427230_d(struct W7230 *a0) {
    int s4 = 0;
    int t;
    int n;
    int i;
    int r;
    int fl;
    int v;
    int w;
    int x4;
    struct R7230 *e;
    sSifDmaData ent[2];
    t = a0->f4(a0->f24, a0->f8, a0->fC);
    if (t != 0) {
        s4 = a0->f2C;
    }
    if (a0->fC > 0) {
        func_00426358(a0->f8, a0->fC);
    }
    if (s4 > 0) {
        func_00426358(t, s4);
    }
    func_0042C078_w();
    fl = a0->f34;
    x4 = fl & 4;
    fl = (unsigned)fl >> 16;
    if (x4) {
        e = func_004266C8_w(&D_0052D680, fl);
    } else {
        e = func_00426698_w(&D_0052D680, fl);
    }
    func_0042C0C0_w();
    ASSERT_7230(e != 0);
    w = 0x8000000A;
    v = a0->f1C;
    e->f20 = w;
    e->f1C = v;
    if (a0->f30 != 0) {
        while (func_004261B0_w(0x80000008, e, 0x40, t, a0->f28, s4) == 0) {
        }
        return;
    }
    e->f18 = 0;
    n = 0;
    e->f10 = 0;
    if (s4 > 0) {
        n = 1;
        ent[0].data = t;
        ent[0].addr = a0->f28;
        ent[0].size = s4;
        ent[0].attr = 0;
    }
    ent[n].data = (int)e;
    ent[n].addr = a0->f20;
    ent[n].size = 0x40;
    ent[n].attr = 0;
    n++;
    do {
        r = sceSifSetDma(ent, n);
        if (r == 0) {
            i = 0x100000;
            do {
                i--;
            } while (i != -1);
        }
    } while (r == 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004273F8);
#ifdef SKIP_ASM
extern int func_004271D8(int a0);
extern void func_00427230(int a0);
extern void SleepThread(void);
/**
 * Serves a queue forever: runs every pending request, then sleeps until woken.
 *
 * Takes requests with func_004271D8 and runs each with func_00427230; when none is left it calls SleepThread and starts
 * over. Never returns. No caller in this build.
 *
 * @param queue  the server queue to serve
 * @return never returns
 */
int func_004273F8(int queue) {
    int server;
    while (1) {
        while ((server = func_004271D8(queue)) != 0) {
            func_00427230(server);
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
 * D_0052EC00 whose flags (f4) are 0 and marks it in use with 0x10000000. Called by func_00427D60 and func_00429560.
 *
 * @return the slot, or 0 when all 32 are taken
 */
sFioFile *func_00427498(void) {
    sFioFile *p;
    sFioFile *end;
    func_00427438();
    WaitSema_i(D_00455218);
    p = D_0052EC00;
    end = p + 32;
    while (p < end) {
        if (p->f4 == 0) {
            p->f4 = 0x10000000;
            SignalSema(D_00455218);
            return p;
        }
        p++;
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
 * @param a0  descriptor number (index into D_0052EC00)
 * @return &D_0052EC00[@p a0], or 0 when it is 32 or more
 */
sFioFile *func_00427520(unsigned a0) {
    sFioFile *p;
    func_00427438();
    WaitSema_i(D_00455218);
    if (a0 >= 0x20) {
        SignalSema(D_00455218);
        return 0;
    }
    p = &D_0052EC00[a0];
    SignalSema(D_00455218);
    return p;
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
 * @param a0  the received SIF command packet
 */
void func_00427590(int *a0) {
    char *p;
    int sema;
    int cmd;
    char *dst;
    int size;
    char *buf;
    unsigned len;
    int idx;
    int i;
    D_00455208 = idx = D_00455210 != 0 ? a0[3] : 0;
    p = (char *)((int)(D_0052E380 + idx * 0x440) | 0x20000000);
    *(struct W7590 *)&sema = *(struct W7590 *)(p + 0x0);
    *(struct W7590 *)&cmd = *(struct W7590 *)(p + 0x4);
    *(struct W7590 *)&dst = *(struct W7590 *)(p + 0x8);
    *(struct W7590 *)&size = *(struct W7590 *)(p + 0xC);
    if (sema >= 0)
        func_0041605C(dst, p + 0x10, size);
    switch (cmd) {
    case 2: {
        struct L7590 *q = (struct L7590 *)(p + 0x14);
        char *d;
        char *u;
        if (q->n1 > 0) {
            d = q->d1;
            u = (char *)((int)d | 0x20000000);
            for (i = 0; i < q->n1; i++) {
                unsigned char c = q->b1[i];
                u[i] = c;
                d[i] = c;
            }
        }
        if (q->n2 > 0) {
            d = q->d2;
            u = (char *)((int)d | 0x20000000);
            for (i = 0; i < q->n2; i++) {
                unsigned char c = q->b2[i];
                u[i] = c;
                d[i] = c;
            }
        }
        break;
    }
    case 11:
        *(struct W7590 *)&buf = *(struct W7590 *)(p + 0x14);
        *(struct B7590 *)buf = *(struct B7590 *)(p + 0x18);
        break;
    case 0xC:
        *(struct W7590 *)&buf = *(struct W7590 *)(p + 0x14);
        *(struct C7590 *)buf = *(struct C7590 *)(p + 0x18);
        break;
    case 23:
    case 25:
    case 26:
        *(struct W7590 *)&buf = *(struct W7590 *)(p + 0x14);
        *(struct W7590 *)&len = *(struct W7590 *)(p + 0x18);
        if (len > 0x400)
            len = 0x400;
        func_0041605C(buf, p + 0x1C, len);
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
extern int func_0042C078_i(void) __asm__("func_0042C078");
extern void func_0042C0C0(void);
extern void func_004279B8(void);
/**
 * Installs the callback run when the IOP file server sends SIF command 0x80000013, returning the previous one.
 *
 * Under the call lock (func_00427988 ... func_004279B8), initialising the library first when D_0045520C is 0
 * (func_00427A90), it stores @p a0 and @p a1 in D_0052EE80 with interrupts disabled (re-enabled only when they were
 * on). func_00427A60 runs the callback. No caller in this build.
 *
 * @param a0  callback, called with @p a1
 * @param a1  argument for the callback
 * @return the previous callback
 */
void (*func_004279C8(void (*a0)(int), int a1))(int) {
    int r;
    void (*old)(int);
    sFioCb *p = &D_0052EE80;
    func_00427988_a(27);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    r = func_0042C078_i();
    old = D_0052EE80.fn;
    p->arg = a1;
    D_0052EE80.fn = a0;
    if (r != 0) {
        func_0042C0C0();
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
extern int func_0042C078_z(void) __asm__("func_0042C078");
extern void func_0042C0C0_z(void) __asm__("func_0042C0C0");
extern void func_00426020_z(int, void *, void *) __asm__("func_00426020");
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
 * (SIF command 0x80000011, reply buffer D_0052EE40) and func_00427A60 (0x80000013, the callback) with func_00426020.
 * Then binds D_0052EE00 to server 0x80000001 (func_00426B48), retrying after a 0x100000-iteration spin until the server
 * is bound. It frees all 32 descriptor slots of D_0052EC00 under D_00455218, sends the addresses of the two 0x440-byte
 * reply buffers D_0052E380 (in D_0052D6C0) with RPC function 0xFF and copies the 4-byte server version to D_0052EE28;
 * D_00455210 becomes 1 when the second reply word is 2 (the server then names a reply buffer per request). Sets the
 * initialised flag D_0045520C. The file calls in this unit call it when the flag is 0.
 *
 * @return 0; -1 when the bind fails; 0xFFFEFFFF when the version call fails
 */
int func_00427A90_d(void) {
    int old;
    int v;
    sFioFile *p;
    sFioCb *q = &D_0052EE80;
    sRpcVer w;
    func_00426408_z(0);
    D_0052EE80.fn = 0;
    q->arg = 0;
    old = func_0042C078_z();
    func_00426020_z(0x80000011, func_00427590_z, D_0052EE40);
    func_00426020_z(0x80000013, func_00427A60_z, q);
    if (old != 0)
        func_0042C0C0_z();
    for (;;) {
        if (func_00426B48(&D_0052EE00, 0x80000001, 0) < 0)
            return -1;
        if (D_0052EE00.server != 0)
            break;
        v = 0x100000;
        do {
            v--;
        } while (v != -1);
    }
    func_00427438_z();
    WaitSema_z(D_00455218);
    for (p = D_0052EC00; p < &D_0052EC00[32]; p++)
        p->f4 = 0;
    SignalSema_z(D_00455218);
    D_0052D6C0[0] = (int)D_0052E380;
    D_0052D6C0[1] = D_0052D6C0[0] + 1088;
    if (func_00426D18(&D_0052EE00, 0xFF, 0, D_0052D6C0, 8, D_0052E340, 8, 0, 0) < 0)
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
extern int func_00415FC8(char *, char *, int);
/**
 * Returns whether the bound file server's version tag is one the library does not accept.
 *
 * Compares the 4-byte tag D_0052EE28 (set by func_00427A90) with the library's "2710" (D_00455164) and with the tag
 * D_00455220 points at ("....", D_0049AD48) using func_00415FC8 (memcmp). Returns 0 when it equals either; otherwise
 * the result of comparing the two accepted tags, which always differ, so 1. func_00427D60 refuses to open files then.
 *
 * @return 1 when the server's tag matches neither accepted tag, else 0
 */
int func_00427C98(void) {
    char *a = D_00455164;
    char *b = D_0052EE28.b;
    int r = 0;
    if (func_00415FC8(b, a, 4) != 0 &&
        func_00415FC8(b, D_00455220, 4) != 0) {
        r = func_00415FC8(a, D_00455220, 4) != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427D28);
#ifdef SKIP_ASM
extern void func_00416210(void *a0, int a1, int a2);
/**
 * Marks the file library as uninitialised so the next file call rebinds the server.
 *
 * Clears D_0045520C and zeroes the 4-byte server version D_0052EE28 (func_00416210, memset). func_00319610 calls it
 * after rebooting the IOP.
 *
 * @return 0
 */
int func_00427D28(void) {
    D_0045520C = 0;
    func_00416210(&D_0052EE28, 0, 4);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427D60);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int func_00427C98_z(void) __asm__("func_00427C98");
extern int CreateSema(int *p);
extern int db0i(int) __asm__("DeleteSema");
extern void SignalSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00427D60_d(unsigned char *, unsigned, ...) __asm__("func_00427D60");
/**
 * Opens a file on the IOP through the file server and returns a descriptor number.
 *
 * Under the call lock (func_00427988 ... func_004279B8), initialising the library when needed (func_00427A90), it fails
 * with 0xFFFEFFFC when the server's version is not accepted (func_00427C98) and with -19 when no descriptor slot is
 * free (func_00427498). The RPC function 0 request in D_0052D700.open holds a new semaphore, the address of a stack
 * result word and its size 4, @p flags without bit 31, the mode (the first variadic argument), the name (up to 0x3FF
 * characters, truncated) and the slot index; it is sent with func_00426D18 (0x418 bytes) and the server's first reply
 * word (D_0052E340, read uncached) says whether it was accepted. The lock is released before waiting on the semaphore
 * for the result (func_00427590 posts it). A negative result frees the slot and is returned; otherwise the IOP's
 * descriptor goes to the slot's f0 and @p flags are or-ed into its f4. Likely bug: when the RPC fails or is refused
 * (-11) the slot taken by func_00427498 is never freed. Called by FILEDEV_open.
 *
 * @param s  path of the file
 * @param flags  open flags (bit 15 makes later calls on the descriptor no-wait)
 * @return the descriptor number (slot index); a negative error
 */
// sceOpen
int func_00427D60_d(unsigned char *s, unsigned flags, ...) {
    int buf[12];
    int sem;
    int v;
    int n;
    int idx;
    char c;
    int mode;
    char *ap;
    sFioFile *t;
    sFioOpenCmd *p = &D_0052D700.open;
    func_00427988_b(0);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    if (func_00427C98_z() != 0) {
        func_004279B8();
        return 0xFFFEFFFC;
    }
    t = func_00427498();
    if (t == 0) {
        func_004279B8();
        return -19;
    }
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    mode = *(int *)ap;
    c = (p->name[0] = s[0]);
    n = 0;
    if (c != 0) {
        while (++n < 0x400) {
            c = (p->name[n] = s[n]);
            if (c == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
    }
    idx = t - D_0052EC00;
    p->fC = flags & 0x7FFFFFFF;
    p->f10 = mode;
    buf[1] = 1;
    p->f414 = idx;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0, 0, &D_0052D700.open, 1048, D_0052E340, 4, 0, 0) < 0) {
        db0i(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        db0i(sem);
        return -11;
    }
    WaitSema_i(sem);
    db0i(sem);
    if (buf[8] < 0) {
        WaitSema_i(D_00455218);
        t->f4 = 0;
        SignalSema(D_00455218);
        return buf[8];
    }
    v = idx;
    WaitSema_i(D_00455218);
    t->f0 = buf[8];
    t->f4 |= flags;
    SignalSema(D_00455218);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427FE8);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Closes a file descriptor through the IOP file server.
 *
 * Takes the call lock (func_00427988; released by func_004279B8), but unlike func_00427D60 does not initialise the
 * library: returns -1 when D_0045520C is 0 and -9 when the descriptor's slot (func_00427520) is not open. Sends RPC
 * function 1 with a new semaphore, the address and size (4) of a stack result word, the IOP descriptor and the slot
 * index (20 bytes); after a successful call it frees the slot (f4 = 0, without taking D_00455218). It waits on the
 * semaphore for the result even when the descriptor was opened no-wait. Called by FILEDEV_close.
 *
 * @param a0  descriptor number from func_00427D60
 * @return 0, the IOP's negative error, or -11 when the RPC fails or is refused
 */
// sceClose
int func_00427FE8(int a0) {
    int buf[12];
    int sem;
    int v;
    sFioFile *t;
    sFioFdCmd *p = &D_0052D700.close;
    t = func_00427520(a0);
    func_00427988_b(1);
    if (D_0045520C == 0) {
        func_004279B8();
        return -1;
    }
    if (t == 0 || t->f4 == 0) {
        func_004279B8();
        return -9;
    }
    p->fC = t->f0;
    buf[1] = 1;
    p->f10 = t - D_0052EC00;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 1, 0, p, 20, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    t->f4 = 0;
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    {
        int r = buf[8];
        if (r < 0)
            return r;
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00428168);
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
 * stack result word, the IOP descriptor, @p a1, @p a2 and the slot index (0x1C bytes). For a descriptor opened with
 * flag 0x8000 (no wait) the semaphore is parked, negated in the request, in a free entry of D_00455188 (under
 * D_0045521C) and the call returns 0 without waiting; func_00427590 clears that entry when the IOP is done. Called by
 * FILEDEV_getsize, FILEDEV_readexec and FILEDEV_write.
 *
 * @param a0  descriptor number
 * @param a1  offset
 * @param a2  origin of @p a1 (passed to the IOP)
 * @return the new position, 0 for a no-wait call, -11 when the RPC fails or is refused, or the IOP's error
 */
// sceLseek
int func_00428168(int a0, int a1, int a2)
{
  int buf[12];
  int sem;
  int v;
  int i;
  short flags;
  sFioFile *t;
  unsigned fl;
  int *out;
  sFioLseekCmd *p = &D_0052D700.lseek;
  t = func_00427520(a0);
  func_00427988_b(4);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((t == 0) || ((fl = t->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  p->fC = t->f0;
  p->f10 = a1;
  p->f14 = a2;
  p->f18 = t - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  p->f8 = 4;
  p->f4 = (int) out;
  flags = fl;
  D_0052D700.lseek.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if (func_00426D18(&D_0052EE00, 4, 0, &D_0052D700.lseek, 0x1C, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
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
INCLUDE_ASM("seg/seg_325220", func_004283A0);
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
 * stack result word, the IOP descriptor, @p a1, @p a2 and the slot index (0x20 bytes). Unless the descriptor has flag
 * 0x20000000 it first writes the buffer back from the data cache, and it always writes back the request block
 * (func_00426358). The IOP fills the buffer by DMA; func_00427590 patches its unaligned head and tail bytes. A
 * descriptor opened with flag 0x8000 parks the semaphore in D_00455188 and returns 0 at once, as in func_00428168.
 * Called by FILEDEV_readexec.
 *
 * @param a0  descriptor number
 * @param a1  destination buffer
 * @param a2  bytes to read
 * @return bytes read, 0 for a no-wait call, -11 when the RPC fails or is refused, or the IOP's error
 */
int func_004283A0(int a0, int a1, int a2)
{
  int buf[12];
  int sem;
  int v;
  int i;
  short flags;
  sFioFile *t;
  unsigned fl;
  int *out;
  sFioReadCmd *p = &D_0052D700.read;
  t = func_00427520(a0);
  func_00427988_b(2);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((t == 0) || ((fl = t->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  p->fC = t->f0;
  p->f10 = a1;
  p->f14 = a2;
  p->f1C = t - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  p->f8 = 4;
  p->f4 = (int) out;
  flags = fl;
  D_0052D700.read.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if (!(fl & 0x20000000))
    func_00426358(a1, a2);
  func_00426358((int) p, 0x20);
  if (func_00426D18(&D_0052EE00, 2, 0, &D_0052D700.read, 0x20, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
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
INCLUDE_ASM("seg/seg_325220", func_00428600);
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
 * size (4) of a stack result word, the IOP descriptor, @p a1, @p a2, the slot index and the unaligned head of the
 * buffer: the bytes up to the next 16-byte boundary (at most @p a2), copied through the uncached view, with their
 * count. Unless the descriptor has flag 0x20000000 the buffer is written back from the data cache first. A descriptor
 * opened with flag 0x8000 parks the semaphore in D_00455188 and returns 0 at once, as in func_00428168. Called by
 * FILEDEV_write.
 *
 * @param a0  descriptor number
 * @param a1  source buffer
 * @param a2  bytes to write
 * @return bytes written, 0 for a no-wait call, -11 when the RPC fails or is refused, or the IOP's error
 */
int func_00428600(int a0, unsigned a1, int a2)
{
  int buf[12];
  int sem;
  int v;
  int i;
  int n;
  int j;
  short flags;
  sFioFile *t;
  unsigned fl;
  int *out;
  sFioWriteCmd *p = &D_0052D700.write;
  t = func_00427520(a0);
  func_00427988_b(3);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((t == 0) || ((fl = t->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  p->fC = t->f0;
  p->f14 = a2;
  p->f10 = a1;
  p->f2C = t - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  out = &buf[8];
  p->f8 = 4;
  p->f4 = (int) out;
  flags = fl;
  D_0052D700.write.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if ((a1 & 0xF) == 0)
    n = 0;
  else
    {
      unsigned lo = a1 - 0x10;
      n = ((a1 >> 4) << 4) - lo;
    }
  if (a2 < n)
    n = a2;
  if (!(fl & 0x20000000))
    func_00426358(a1, a2);
  a1 |= 0x20000000;
  p->f18 = n;
  for (j = 0; j < n; j++)
    p->data[j] = ((char *) a1)[j];
  if (func_00426D18(&D_0052EE00, 3, 0, &D_0052D700.write, 0x30, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
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
INCLUDE_ASM("seg/seg_325220", func_004288C0);
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
 * Takes the call lock (func_00427988; released by func_004279B8), stores @p arg in D_0052D6C8 and initialises the
 * library when needed (func_00427A90); -9 when the slot (func_00427520) is not open. Request 1 sets *@p arg to 1 when
 * any no-wait call is still pending in D_00455188 (under D_0045521C), else 0. Requests 2 and 3 copy the 32-bit or
 * 64-bit result word of the last completed reply buffer (D_0052E390 + D_00455208 * 0x440, uncached) to *@p arg. Any
 * other request is sent as RPC function 5 (1056 bytes, written back first) with a new semaphore, the IOP descriptor,
 * the request number and 0x400 bytes copied from @p arg (length 0 when @p arg is 0); it waits for the result even on a
 * no-wait descriptor. No caller in this build.
 *
 * @param fd  descriptor number
 * @param req  request number
 * @param arg  request argument or result pointer
 * @return 0 for requests 1-3; else the IOP's result, or -11 when the RPC fails or is refused
 */
// FAKEMATCH: stand-in local `one = 1` (retail keeps the constant 1 in $21 from the prologue for the switch's case-1 compare) plus a dead `one = 0;` at the end of case 1, so jump1 keeps the local until cse substitutes it into the compare; plain form 97.75%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int func_004288C0(int fd, int req, void *arg) {
    int buf[12];
    int sem;
    int v;
    int i;
    sFioFile *t;
    sFioIoctl2Cmd *p = &D_0052D700.ioctl2;
    int one = 1; /* FAKEMATCH: stand-in for the case-1 constant */
    t = func_00427520(fd);
    func_00427988_b(5);
    D_0052D6C8 = arg;
    if (D_0045520C == 0) {
        func_00427A90();
    }
    if (t == 0 || t->f4 == 0) {
        func_004279B8();
        return -9;
    }
    p->f414 = 0;
    p->f418 = 0;
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
        *(int *)arg = *(int *)((int)(D_0052E390 + D_00455208 * 0x440) | 0x20000000);
        func_004279B8();
        return 0;
    case 3:
        *(long *)arg = *(long *)((int)(D_0052E390 + D_00455208 * 0x440) | 0x20000000);
        func_004279B8();
        return 0;
    default:
        p->fC = t->f0;
        p->f10 = req;
        if (arg == 0) {
            p->f41C = 0;
        } else {
            p->f41C = 0x400;
            *(struct Blk88C0 *)p->data = *(struct Blk88C0 *)arg;
        }
        buf[1] = 1;
        buf[2] = 0;
        buf[5] = 0;
        sem = CreateSema(&buf[0]);
        p->f0 = sem;
        p->f4 = (int)&buf[8];
        p->f8 = 4;
        func_00426358((int)&D_0052D700.ioctl2, 1056);
        if (func_00426D18(&D_0052EE00, 5, 0, &D_0052D700.ioctl2, 1056, D_0052E340, 4, 0, 0) < 0) {
            DeleteSema(sem);
            func_004279B8();
            return -11;
        }
        v = *(int *)((int)D_0052E340 | 0x20000000);
        func_004279B8();
        if (v == 0) {
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
INCLUDE_ASM("seg/seg_325220", func_00428C38);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern void *func_0041605C(void *, void *, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00428C38_d(int, int, void *, unsigned, int, unsigned) __asm__("func_00428C38");
/**
 * Performs an extended control request (input and output buffers) on a file descriptor through the IOP file server.
 *
 * Takes the call lock (func_00427988; released by func_004279B8) and initialises the library when needed
 * (func_00427A90); -9 when the slot (func_00427520) is not open, -22 when @p len or @p a5 exceeds 0x400. Copies @p len
 * bytes of @p data into the request (func_0041605C, memcpy) and sends RPC function 26 (1056 bytes, written back first)
 * with a new semaphore, the IOP descriptor, @p a1, the lengths and @p a4; func_00427590 copies the reply data to
 * @p a4. The length word is cleared when @p data is 0 and then overwritten with @p len anyway. It always waits for the
 * result. No caller in this build.
 *
 * @param a0  descriptor number
 * @param a1  request number
 * @param data  input data (up to 0x400 bytes), or 0
 * @param len  bytes of @p data
 * @param a4  output buffer
 * @param a5  size of @p a4 (up to 0x400)
 * @return the IOP's result, -9, -22, or -11 when the RPC fails or is refused
 */
int func_00428C38_d(int a0, int a1, void *data, unsigned len, int a4, unsigned a5) {
    int buf[12];
    int sem;
    int v;
    sFioFile *t;
    sFioIoctl2Cmd *p = &D_0052D700.ioctl2;
    t = func_00427520(a0);
    func_00427988_b(26);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    if (t == 0 || t->f4 == 0) {
        func_004279B8();
        return -9;
    }
    if (len > 0x400 || a5 > 0x400) {
        func_004279B8();
        return -22;
    }
    if (data == 0) {
        p->f41C = 0;
    } else {
        func_0041605C(p->data, data, len);
    }
    p->fC = t->f0;
    p->f10 = a1;
    p->f41C = len;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f418 = a5;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    p->f414 = a4;
    p->f0 = sem;
    func_00426358((int)&D_0052D700.ioctl2, 1056);
    if (func_00426D18(&D_0052EE00, 26, 0, &D_0052D700.ioctl2, 1056, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
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
 * Takes the call lock (func_00427988 with @p a1; released by func_004279B8) and initialises the library when needed
 * (func_00427A90). Copies @p s (up to 0x3FF characters, truncated) into D_0052D700.path and sends RPC function @p a1
 * with a new semaphore and the address and size (4) of a stack result word; the request is the 12-byte header plus the
 * name and its NUL. Used for requests 6, 8, 9, 16, 18 and 21 (func_00428FC0, func_00429190, func_00429560,
 * func_00429540, func_00429EB8, func_0042A2E0).
 *
 * @param s  the path
 * @param a1  file-server RPC function number
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_00428E18_d(char *s, int a1) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPathCmd *p = &D_0052D700.path;
    func_00427988_b(a1);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
        n = 0x3FF;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, a1, 0, &D_0052D700.path, n + 13, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00428FC0);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Removes a file: sends file-server request 6 with @p path through func_00428E18.
 *
 * @param path  the file's path
 * @return the IOP's result (0 or a negative error), or -11 when the RPC fails
 */
int func_00428FC0(int path) {
    return func_00428E18(path, 6);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00428FE0);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00428FE0_d(char *, int) __asm__("func_00428FE0");
/**
 * Creates a directory through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p s (up to 0x3FF characters) and
 * @p a1 into D_0052D700.mkdir and sends RPC function 7 (the 16-byte header plus the name and its NUL) with a new
 * semaphore and a stack result word, then waits for the result.
 *
 * @param s  the directory's path
 * @param a1  mode passed to the IOP
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_00428FE0_d(char *s, int a1) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPathModeCmd *p = &D_0052D700.mkdir;
    func_00427988_b(7);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
        n = 0x3FF;
    }
    p->fC = a1;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 7, 0, &D_0052D700.mkdir, n + 17, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429190);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Removes a directory: sends file-server request 8 with @p path through func_00428E18.
 *
 * @param path  the directory's path
 * @return the IOP's result, or -11 when the RPC fails
 */
int func_00429190(int path) {
    return func_00428E18(path, 8);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004291B0);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_004291B0_d(char *, char *, char *, int) __asm__("func_004291B0");
/**
 * Formats a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p s and @p t (each up to 0x3FF
 * characters; an empty second name when @p t is 0) and @p len bytes of @p data into D_0052D700.format; -7 when @p len
 * exceeds 0x400 (checked after the names are copied). Sends RPC function 14 (3088 bytes, written back first) with a new
 * semaphore and a stack result word and waits for the result.
 *
 * @param s  the device to format
 * @param t  the block device, or 0
 * @param data  argument data
 * @param len  bytes of @p data (up to 0x400)
 * @return the IOP's result, -7, or -11 when the RPC fails or is refused
 */
int func_004291B0_d(char *s, char *t, char *data, int len) {
    int buf[12];
    int sem;
    int v;
    int n;
    int i;
    sFioFormatCmd *p = &D_0052D700.format;
    func_00427988_b(0xE);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
    }
    if (t == 0) {
        p->name2[0] = 0;
    } else {
        n = 0;
        if ((p->name2[0] = t[0]) != 0) {
            while (++n < 0x400) {
                if ((p->name2[n] = t[n]) == 0) {
                    break;
                }
            }
        }
        if (n == 0x400) {
            p->name2[0x3FF] = 0;
        }
    }
    if (len > 0x400) {
        func_004279B8();
        return -7;
    }
    for (i = 0; i < len; i++) {
        p->data[i] = data[i];
    }
    p->fC0C = len;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    func_00426358((int)&D_0052D700.format, 3088);
    if (func_00426D18(&D_0052EE00, 0xE, 0, &D_0052D700.format, 3088, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
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
 * semaphore, a stack result word's address and size, and @p a0. Unlike the other file calls it returns -1 (not -11)
 * when the RPC fails or is refused. No caller in this build.
 *
 * @param a0  the word passed to the IOP
 * @return the IOP's result, or -1
 */
int func_00429420(int a0) {
    int buf[12];
    int sem;
    int v;
    sFioCmd4 *p = &D_0052D700.adddrv;
    func_00427988_b(0xF);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    p->fC = a0;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    D_0052D700.adddrv.f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0xF, 0, &D_0052D700.adddrv, 0x10, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -1;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
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
INCLUDE_ASM("seg/seg_325220", func_00429560);
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
 * @param a0  the directory's path
 * @return the descriptor number (slot index), or a negative error
 */
int func_00429560(int a0) {
    int r;
    sFioFile *p;
    func_00427988_b(9);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    func_004279B8();
    p = func_00427498();
    if (p == 0) {
        return -19;
    }
    r = func_00428E18(a0, 9);
    if (r < 0) {
        WaitSema_i(D_00455218);
        p->f4 = 0;
        SignalSema(D_00455218);
        return r;
    } else {
        WaitSema_i(D_00455218);
        p->f0 = r;
        r = p - D_0052EC00;
        SignalSema(D_00455218);
        return r;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429628);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
/**
 * Closes a directory descriptor through the IOP file server.
 *
 * Like func_00427FE8: takes the call lock, returns -1 when the library is not initialised and -9 when the slot
 * (func_00427520) is not open, sends RPC function 10 (20 bytes) with a new semaphore, a stack result word and the IOP
 * descriptor, frees the slot (f4 = 0) once the call is accepted and waits for the result.
 *
 * @param a0  descriptor number from func_00429560
 * @return 0, the IOP's negative error, or -11 when the RPC fails or is refused
 */
int func_00429628(int a0) {
    int buf[12];
    int sem;
    int v;
    sFioFile *e;
    sFioFdCmd *p = &D_0052D700.dclose;
    e = func_00427520(a0);
    func_00427988_b(10);
    if (D_0045520C == 0) {
        func_004279B8();
        return -1;
    }
    if (e == 0 || e->f4 == 0) {
        func_004279B8();
        return -9;
    }
    p->fC = e->f0;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 10, 0, p, 20, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    e->f4 = 0;
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    {
        int r = buf[8];
        if (r < 0)
            return r;
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429790);
#ifdef SKIP_ASM
extern int func_00427988_b(int);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00429790_d(int, int) __asm__("func_00429790");
/**
 * Reads the next directory entry of a directory descriptor through the IOP file server.
 *
 * Takes the call lock; returns -1 when the library is not initialised and -9 when the slot (func_00427520) is not open.
 * Sends RPC function 11 (0x20 bytes) with a new semaphore, a stack result word, the IOP descriptor and @p a1;
 * func_00427590 copies the 0x144-byte entry to @p a1. Likely bug: when func_00426D18 fails it calls WaitSema on the new
 * semaphore instead of DeleteSema, which blocks (nothing signals it) and leaks the semaphore.
 *
 * @param a0  descriptor number from func_00429560
 * @param a1  where the 0x144-byte directory entry is stored
 * @return the IOP's result (positive while entries remain), or -11 when the call is refused
 */
int func_00429790_d(int a0, int a1) {
    int buf[12];
    int sem;
    int v;
    sFioFile *t;
    sFioFdCmd *p = &D_0052D700.dread;
    t = func_00427520(a0);
    func_00427988_b(11);
    if (D_0045520C == 0) {
        func_004279B8();
        return -1;
    }
    if (t == 0 || t->f4 == 0) {
        func_004279B8();
        return -9;
    }
    p->fC = t->f0;
    p->f10 = a1;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 11, 0, p, 0x20, D_0052E340, 4, 0, 0) < 0) {
        WaitSema_i(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004298E8);
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
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p a0 (up to 0x3FF characters)
 * and @p a1 into D_0052D700.getstat and sends RPC function 12 (the 16-byte header plus the name and its NUL) with a new
 * semaphore and a stack result word; func_00427590 copies the 0x40-byte status block to @p a1.
 *
 * @param a0  the file's path
 * @param a1  where the 0x40-byte status block is stored
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_004298E8(char *a0, int a1) {
    int buf[12];
    int sem;
    int v;
    int i;
    unsigned char c;
    sFioPathModeCmd *p = &D_0052D700.getstat;
    func_00427988_b(0xC);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    c = (p->name[0] = a0[0]);
    i = 0;
    if (c != 0) {
        while (++i < 0x400) {
            c = (p->name[i] = a0[i]);
            if (c == 0) {
                break;
            }
        }
    }
    if (i == 0x400) {
        p->name[0x3FF] = 0;
        i = 0x3FF;
    }
    p->fC = a1;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0xC, 0, &D_0052D700.getstat, i + 17, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429A88);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00429A88_d(char *, sFioStat *, int) __asm__("func_00429A88");
/**
 * Changes a file's status through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p s (up to 0x3FF characters),
 * the 0x40-byte status block @p src and @p a2 into D_0052D700.chstat, writes the block back from the data cache (1104
 * bytes) and sends RPC function 13 (the 0x50-byte header plus the name and its NUL) with a new semaphore and a stack
 * result word.
 *
 * @param s  the file's path
 * @param src  the new status values
 * @param a2  mask of the fields to change
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_00429A88_d(char *s, sFioStat *src, int a2) {
    int buf[12];
    int sem;
    int v;
    int n;
    unsigned char c;
    sFioChstatCmd *p = &D_0052D700.chstat;
    func_00427988_b(13);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    c = (p->name[0] = s[0]);
    n = 0;
    if (c != 0) {
        while (++n < 0x400) {
            c = (p->name[n] = s[n]);
            if (c == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
        n = 0x3FF;
    }
    p->v = *src;
    p->fC = a2;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    func_00426358((int)&D_0052D700.chstat, 1104);
    if (func_00426D18(&D_0052EE00, 13, 0, &D_0052D700.chstat, n + 81, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429CC8);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00429CC8_d(char *, char *) __asm__("func_00429CC8");
/**
 * Renames a file through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies both names (each up to 0x3FF
 * characters) into D_0052D700.rename, writes the 2060-byte request back and sends RPC function 17 with a new semaphore
 * and a stack result word.
 *
 * @param s  the current path
 * @param t  the new path
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_00429CC8_d(char *s, char *t) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPath2Cmd *p = &D_0052D700.rename;
    func_00427988_b(17);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
    }
    n = 0;
    if ((p->name2[0] = t[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name2[n] = t[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name2[0x3FF] = 0;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    func_00426358((int)&D_0052D700.rename, 2060);
    if (func_00426D18(&D_0052EE00, 17, 0, &D_0052D700.rename, 2060, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429EB8);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Changes the IOP file server's current directory: sends request 18 with @p path through func_00428E18.
 *
 * @param path  the new current directory
 * @return the IOP's result, or -11 when the RPC fails
 */
int func_00429EB8(int path) {
    return func_00428E18(path, 18);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429ED8);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_00429ED8_d(char *, int) __asm__("func_00429ED8");
/**
 * Flushes a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies @p s (up to 0x3FF characters) and
 * @p a1 into D_0052D700.sync and sends RPC function 19 (1044 bytes) with a new semaphore and a stack result word.
 *
 * @param s  the device or path
 * @param a1  flag passed to the IOP
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_00429ED8_d(char *s, int a1) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPathArgCmd *p = &D_0052D700.sync;
    func_00427988_b(19);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
        n = 0x3FF;
    }
    p->f10 = a1;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 19, 0, &D_0052D700.sync, 1044, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A070);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_0042A070_d(char *, char *, int, char *, int) __asm__("func_0042A070");
/**
 * Mounts a file system through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies both names (each up to 0x3FF
 * characters), @p a2 and @p len bytes of @p data into D_0052D700.mount; -7 when @p len exceeds 0x400. Writes the
 * 3092-byte request back and sends RPC function 20 with a new semaphore and a stack result word.
 *
 * @param s  the file-system device to mount as
 * @param t  the block device
 * @param a2  mount flag
 * @param data  argument data
 * @param len  bytes of @p data (up to 0x400)
 * @return the IOP's result, -7, or -11 when the RPC fails or is refused
 */
int func_0042A070_d(char *s, char *t, int a2, char *data, int len) {
    int buf[12];
    int sem;
    int v;
    int n;
    int i;
    sFioMountCmd *p = &D_0052D700.mount;
    func_00427988_b(20);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
    }
    n = 0;
    if ((p->name2[0] = t[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name2[n] = t[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name2[0x3FF] = 0;
    }
    if (len > 0x400) {
        func_004279B8();
        return -7;
    }
    for (i = 0; i < len; i++) {
        p->data[i] = data[i];
    }
    p->fC10 = len;
    p->fC0C = a2;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    func_00426358((int)&D_0052D700.mount, 3092);
    if (func_00426D18(&D_0052EE00, 20, 0, &D_0052D700.mount, 3092, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A2E0);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
/**
 * Unmounts a file system: sends file-server request 21 with @p path through func_00428E18.
 *
 * @param path  the mounted file-system device
 * @return the IOP's result, or -11 when the RPC fails
 */
int func_0042A2E0(int path) {
    return func_00428E18(path, 21);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A300);
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
 * Like func_00428168 but sends RPC function 22 (0x20 bytes) with the 64-bit @p a1, the origin, the slot index and an
 * 8-byte stack result. A descriptor opened with flag 0x8000 parks the semaphore in D_00455188 and returns 0 at once.
 *
 * @param a0  descriptor number
 * @param a1  64-bit offset
 * @param a2  origin of @p a1 (passed to the IOP)
 * @return the new 64-bit position, 0 for a no-wait call, -1 or -9 as in func_00428168, -11 when the RPC fails
 */
long func_0042A300(int a0, long a1, int a2)
{
  int buf[8];
  long res;
  int sem;
  int v;
  int i;
  short flags;
  sFioFile *t;
  unsigned fl;
  sFioLseek64Cmd *p = &D_0052D700.lseek64;
  t = func_00427520(a0);
  func_00427988_b(22);
  if (D_0045520C == 0)
  {
    func_004279B8();
    return -1;
  }
  if ((t == 0) || ((fl = t->f4) == 0))
  {
    func_004279B8();
    return -9;
  }
  p->fC = t->f0;
  p->f10 = a1;
  p->f18 = a2;
  p->f1C = t - D_0052EC00;
  buf[1] = 1;
  buf[2] = 0;
  buf[5] = 0;
  sem = CreateSema(&buf[0]);
  p->f4 = (int) &res;
  p->f8 = 8;
  flags = fl;
  D_0052D700.lseek64.f0 = sem;
  if (flags & 0x8000)
  {
    WaitSema_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    SignalSema(D_0045521C);
  }
  if (func_00426D18(&D_0052EE00, 22, 0, &D_0052D700.lseek64, 0x20, D_0052E340, 4, 0, 0) < 0)
  {
    DeleteSema(sem);
    func_004279B8();
    return -11;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
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
INCLUDE_ASM("seg/seg_325220", func_0042A538);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_0042A538_d(char *, int, char *, unsigned, int, unsigned) __asm__("func_0042A538");
/**
 * Sends a device control request (input and output buffers) to a device through the IOP file server.
 *
 * Takes the call lock and initialises the library when needed (func_00427A90). Copies the device name (up to 0x3FF
 * characters); -22 when @p len or @p a5 exceeds 0x400. Copies @p len bytes of @p data, the request number, the lengths
 * and @p a4 into D_0052D700.devctl, writes the 2076-byte request back and sends RPC function 23 with a new semaphore
 * and a stack result word; func_00427590 copies the reply data to @p a4. func_003198A0 (the power-off thread) sends
 * request 0x4402 to "dev9x" through it.
 *
 * @param s  the device name
 * @param a1  request number
 * @param data  input data, or 0
 * @param len  bytes of @p data (up to 0x400)
 * @param a4  output buffer
 * @param a5  size of @p a4 (up to 0x400)
 * @return the IOP's result, -22, or -11 when the RPC fails or is refused
 */
int func_0042A538_d(char *s, int a1, char *data, unsigned len, int a4, unsigned a5) {
    int buf[12];
    int sem;
    int v;
    int n;
    unsigned i;
    sFioDevctlCmd *p = &D_0052D700.devctl;
    func_00427988_b(23);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
    }
    if (len > 0x400 || a5 > 0x400) {
        func_004279B8();
        return -22;
    }
    for (i = 0; i < len; i++) {
        p->data[i] = data[i];
    }
    p->f810 = len;
    p->f80C = a1;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f818 = a5;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    p->f814 = a4;
    p->f0 = sem;
    func_00426358((int)&D_0052D700.devctl, 2076);
    if (func_00426D18(&D_0052EE00, 23, 0, &D_0052D700.devctl, 2076, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A770);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_0042A770_d(char *, char *) __asm__("func_0042A770");
/**
 * Creates a symbolic link through the IOP file server.
 *
 * Takes the call lock (passing 17, which func_00427988 ignores) and initialises the library when needed
 * (func_00427A90). Copies both names (each up to 0x3FF characters) into D_0052D700.symlink and sends RPC function 24
 * (2060 bytes) with a new semaphore and a stack result word.
 *
 * @param s  the existing path
 * @param t  the link to create
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_0042A770_d(char *s, char *t) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPath2Cmd *p = &D_0052D700.symlink;
    func_00427988_b(17);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
    }
    n = 0;
    if ((p->name2[0] = t[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name2[n] = t[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name2[0x3FF] = 0;
    }
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0x18, 0, &D_0052D700.symlink, 2060, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A950);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern void func_00426358(int, int);
extern int CreateSema(int *p);
extern void DeleteSema(int);
extern int WaitSema_i(int) __asm__("WaitSema");
int func_0042A950_d(char *, int, unsigned) __asm__("func_0042A950");
/**
 * Reads a symbolic link's target through the IOP file server.
 *
 * Takes the call lock (passing 17, ignored) and initialises the library when needed (func_00427A90). Copies the path
 * (up to 0x3FF characters), caps @p len at 0x3FF, stores @p a1 and the length, writes @p a1 back from the data cache
 * and sends RPC function 25 with a new semaphore and a stack result word; func_00427590 copies the returned text to
 * @p a1. The send size (2060 bytes) is larger than this request's 1044-byte layout, so stale bytes of D_0052D700 go
 * along.
 *
 * @param s  the link's path
 * @param a1  buffer for the target
 * @param len  size of @p a1
 * @return the IOP's result, or -11 when the RPC fails or is refused
 */
int func_0042A950_d(char *s, int a1, unsigned len) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPathArgCmd *p = &D_0052D700.readlink;
    func_00427988_b(17);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    n = 0;
    if ((p->name[0] = s[0]) != 0) {
        while (++n < 0x400) {
            if ((p->name[n] = s[n]) == 0) {
                break;
            }
        }
    }
    if (n == 0x400) {
        p->name[0x3FF] = 0;
    }
    if (len >= 0x400) {
        len = 0x3FF;
    }
    p->f10 = a1;
    p->fC = len;
    func_00426358(a1, len);
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = CreateSema(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 25, 0, &D_0052D700.readlink, 2060, D_0052E340, 4, 0, 0) < 0) {
        DeleteSema(sem);
        func_004279B8();
        return -11;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        DeleteSema(sem);
        return -11;
    }
    WaitSema_i(sem);
    DeleteSema(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AB18);
#ifdef SKIP_ASM
/**
 * Binds the IOP heap RPC server, retrying until it answers.
 *
 * Binds D_0052EEC0 to server 0x80000003 (func_00426B48); while the client has no server yet it spins 0x100000
 * iterations and binds again. Once bound it sets D_00455224 to 0, which the heap calls (func_0042ABA0 ...
 * func_0042AFE8) check. Called by the IOP boot code (func_00319610, func_00319820) and SNDPLATFORM_init.
 *
 * @return 0, or -1 when a bind fails
 */
int func_0042AB18(void) {
    int i;
    for (;;) {
        i = func_00426B48(&D_0052EEC0, 0x80000003, 0);
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
INCLUDE_ASM("seg/seg_325220", func_0042ABA0);
#ifdef SKIP_ASM
/**
 * Allocates a block of IOP memory through the heap RPC server.
 *
 * When the heap server is bound (D_00455224 >= 0) it sends request 1 with @p a0 (D_0052EF40) and returns the reply word
 * D_0052EF00.
 *
 * @param a0  bytes to allocate
 * @return the IOP address of the block, or 0 when unbound or the RPC fails
 */
int func_0042ABA0(int a0) {
    if (D_00455224 >= 0) {
        D_0052EF40 = a0;
        if (func_00426D18(&D_0052EEC0, 1, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) >= 0) {
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
    int *p;
    if (D_00455224 < 0) {
        return 0;
    }
    p = &D_0052EF40;
    p[0] = a1;
    p[1] = a0;
    p[2] = a2;
    if (func_00426D18(&D_0052EEC0, 4, 0, p, 0xC, &D_0052EF00, 4, 0, 0) >= 0) {
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
 * When the heap server is bound (D_00455224 >= 0) it sends request 2 with @p a0 and returns the reply word D_0052EF00.
 * func_0042AD08 forwards to it.
 *
 * @param a0  IOP address of the block
 * @return the server's result; 0 when unbound, -1 when the RPC fails
 */
int func_0042AC90_impl(int a0) {
    if (D_00455224 < 0) {
        return 0;
    }
    D_0052EF40 = a0;
    if (func_00426D18(&D_0052EEC0, 2, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) < 0) {
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
INCLUDE_ASM("seg/seg_325220", func_0042AD28);
#ifdef SKIP_ASM
struct S2AD28 { int cmd; char name[0xFC]; };
extern struct S2AD28 D_0052EF80;
/**
 * Loads a file into IOP memory through the heap RPC server.
 *
 * When the heap server is bound (D_00455224 >= 0) it sends request 3 with @p a1 and the path (copied into D_0052EF80,
 * up to 0xFB characters; the request is 4 bytes plus the name and its NUL) and returns the reply word D_0052EF00. No
 * caller in this build.
 *
 * @param a0  the file's path
 * @param a1  IOP address to load it to
 * @return the server's result; 0 when unbound, -1 when the RPC fails
 */
int func_0042AD28(char *a0, int a1) {
    int i;
    if (D_00455224 < 0) {
        return 0;
    }
    i = 0;
    if ((D_0052EF80.name[0] = a0[0]) != 0) {
        do {
            i++;
            if (i >= 0xFC) {
                break;
            }
        } while ((D_0052EF80.name[i] = a0[i]) != 0);
    }
    if (i == 0xFC) {
        D_0052EF80.name[0xFB] = 0;
        i = 251;
    }
    D_0052EF80.cmd = a1;
    D_0052EF80.name[0xFB] = 0;
    if (func_00426D18(&D_0052EEC0, 3, 0, &D_0052EF80, i + 5, &D_0052EF00, 4, 0, 0) < 0) {
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
    if (func_00426D18(&D_0052EEC0, 5, 0, 0, 0, &D_0052EF00, 4, 0, 0) < 0) {
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
    if (func_00426D18(&D_0052EEC0, 6, 0, 0, 0, &D_0052EF00, 4, 0, 0) < 0) {
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
    if (func_00426D18(&D_0052EEC0, 7, 0, 0, 0, &D_0052EF00, 4, 0, 0) < 0) {
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
 * @param a0  IOP address of a memory block
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AF68(int a0) {
    if (D_00455224 < 0) {
        return 0;
    }
    D_0052EF40 = a0;
    if (func_00426D18(&D_0052EEC0, 8, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) < 0) {
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
 * @param a0  IOP address of a memory block
 * @return the reply; 0 when the heap server is not bound, 0xFFFFFFFF when the RPC fails
 */
unsigned int func_0042AFE8(int a0) {
    if (D_00455224 < 0) {
        return 0;
    }
    D_0052EF40 = a0;
    if (func_00426D18(&D_0052EEC0, 9, 0, &D_0052EF40, 4, &D_0052EF00, 4, 0, 0) < 0) {
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
 * Does nothing when D_00455228 is already >= 0. Otherwise binds D_0052F280 to server 0x80000006 (func_00426B48),
 * spinning 0x100000 iterations between attempts until the client has a server, sets D_00455228 to 0 and fetches the
 * 4-byte version with request 0xFF into D_0052F2A8. Every module-loader call in this unit calls it first.
 *
 * @return 0; -1 when a bind fails; 0xFFFEFFFF when the version call fails
 */
int func_0042B068(void) {
    int i;
    if (D_00455228 < 0) {
        for (;;) {
            if (func_00426B48(&D_0052F280, 0x80000006, 0) < 0) {
                return -1;
            }
            if (D_0052F280.server != 0) {
                D_00455228 = 0;
                if (func_00426D18(&D_0052F280, 0xFF, 0, 0, 0, &D_0052F080.ver, 4, 0, 0) < 0) {
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
extern int func_00415FC8(char *, char *, int);
/**
 * Returns whether the bound module-loader server's version tag is one the library does not accept.
 *
 * Same test as func_00427C98 on D_0052F2A8: 0 when it equals the library's "2710" (D_00455164) or the tag D_0045522C
 * points at (func_00415FC8, memcmp), else 1 (the final compare of the two accepted tags always differs).
 *
 * @return 1 when the tag matches neither accepted tag, else 0
 */
int func_0042B168(void) {
    char *a = D_00455164;
    char *b = D_0052F2A8.b;
    int r = 0;
    if (func_00415FC8(b, a, 4) != 0 &&
        func_00415FC8(b, D_0045522C, 4) != 0) {
        r = func_00415FC8(a, D_0045522C, 4) != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B1F8);
#ifdef SKIP_ASM
extern void func_00416210(void *a0, int a1, int a2);
/**
 * Marks the module-loader server as unbound (D_00455228 = -1) and zeroes its version D_0052F2A8 (func_00416210).
 *
 * The next module-loader call rebinds it (func_0042B068). No caller in this build.
 *
 * @return 0
 */
int func_0042B1F8(void) {
    D_00455228 = -1;
    func_00416210(&D_0052F2A8, 0, 4);
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
 * (func_0042B168; 0xFFFEFFFC). D_0052F080 gets @p a0, the argument length and the arguments (at most 0xFC bytes; a
 * length of 253 or more copies a full 0xFC block); request 6 is sent (0x200 bytes) and the 8-byte reply read back in
 * place. Called by func_0042B800 and func_0042B820.
 *
 * @param a0  IOP address of the module image
 * @param a1  bytes of @p a2
 * @param a2  argument block, or 0
 * @param a3  receives the reply's second word
 * @return the reply's first word, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B230_i(int a0, int a1, sLfData *a2, int *a3) {
    int r;
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = a0;
    if (a2 != 0) {
        if (a1 >= 253) {
            *(sLfData *)D_0052F080.cmd.n2 = *a2;
            D_0052F080.cmd.f4 = 0xFC;
        } else {
            func_0041605C(D_0052F080.cmd.n2, a2, a1);
            D_0052F080.cmd.f4 = a1;
        }
    } else {
        D_0052F080.cmd.f4 = 0;
    }
    if (func_00426D18(&D_0052F280, 6, 0, &D_0052F080, 0x200, &D_0052F080, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    r = D_0052F080.cmd.f0;
    *a3 = D_0052F080.cmd.f4;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B438);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
extern void *func_0041605C(void *, void *, int);
int func_0042B438_i(int a0, int a1, sLfData *a2, int *a3) __asm__("func_0042B438");
/**
 * Sends module-loader request 7 for a module with an argument block, and returns its results.
 *
 * Same checks and request layout as func_0042B230 (module id, argument length and up to 0xFC bytes of arguments).
 * func_00319790 and func_003F5E90 call it with no arguments and then func_0042B640 on the same module.
 *
 * @param a0  IOP module id
 * @param a1  bytes of @p a2
 * @param a2  argument block, or 0
 * @param a3  receives the reply's second word
 * @return the reply's first word, 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B438_i(int a0, int a1, sLfData *a2, int *a3) {
    int r;
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = a0;
    if (a2 != 0) {
        if (a1 >= 253) {
            *(sLfData *)D_0052F080.cmd.n2 = *a2;
            D_0052F080.cmd.f4 = 0xFC;
        } else {
            func_0041605C(D_0052F080.cmd.n2, a2, a1);
            D_0052F080.cmd.f4 = a1;
        }
    } else {
        D_0052F080.cmd.f4 = 0;
    }
    if (func_00426D18(&D_0052F280, 7, 0, &D_0052F080, 0x200, &D_0052F080, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    r = D_0052F080.cmd.f0;
    *a3 = D_0052F080.cmd.f4;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B640);
#ifdef SKIP_ASM
extern int func_0042B068(void);
extern int func_0042B168(void);
/**
 * Sends module-loader request 8 for a module id and returns the reply word.
 *
 * Binds the loader when needed (func_0042B068) and checks its version (func_0042B168). Called after func_0042B438 by
 * func_00319790 and func_003F5E90.
 *
 * @param a0  IOP module id
 * @return the reply, 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B640(int a0) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = a0;
    if (func_00426D18(&D_0052F280, 8, 0, &D_0052F080, 4, &D_0052F080, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return D_0052F080.cmd.f0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B6D0);
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
 * @param a0  module name
 * @return the reply (negative when not found), 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF
 */
int func_0042B6D0(char *a0) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(D_0052F080.cmd.n1, a0, 0xFC);
    D_0052F080.cmd.n1[0xFB] = 0;
    if (func_00426D18(&D_0052F280, 9, 0, &D_0052F080, 0x200, &D_0052F080, 4, 0, 0) < 0) {
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
 * @param a0  the word sent (an IOP address)
 * @return the reply, 0xFFFF0000 / 0xFFFEFFFC as in func_0042B230, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B770(int a0) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = a0;
    if (func_00426D18(&D_0052F280, 10, 0, &D_0052F080, 4, &D_0052F080, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return D_0052F080.cmd.f0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B800);
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
int func_0042B800(int modBuf, int argLen, int args) {
    int sp;
    return func_0042B230(modBuf, argLen, args, &sp);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B820);
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
int func_0042B820(int modBuf, int argLen, int args, int *modResult) {
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
 * string). Request @p a4 is sent (0x200 bytes) and the 8-byte reply read back in place. Called by func_0042BA68 and
 * func_0042BA88 with request 0.
 *
 * @param a0  the module's path
 * @param a1  bytes of @p a2
 * @param a2  argument block, or 0
 * @param a3  receives the reply's second word
 * @param a4  module-loader request number
 * @return the reply's first word, or 0xFFFEFFFF when the RPC fails
 */
int func_0042B840_i(char *a0, int a1, sLfData *a2, int *a3, int a4) {
    int r;
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(D_0052F080.cmd.n1, a0, 0xFC);
    D_0052F080.cmd.n1[0xFB] = 0;
    if (a2 != 0) {
        if (a1 >= 253) {
            *(sLfData *)D_0052F080.cmd.n2 = *a2;
            D_0052F080.cmd.f0 = 0xFC;
        } else {
            func_0041605C(D_0052F080.cmd.n2, a2, a1);
            D_0052F080.cmd.f0 = a1;
        }
    } else {
        D_0052F080.cmd.n2[0] = 0;
        D_0052F080.cmd.f0 = 0;
    }
    if (func_00426D18(&D_0052F280, a4, 0, &D_0052F080, 0x200, &D_0052F080, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    r = D_0052F080.cmd.f0;
    *a3 = D_0052F080.cmd.f4;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BA68);
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
int func_0042BA68(int path, int argLen, int args) {
    int sp;
    return func_0042B840(path, argLen, args, &sp, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BA88);
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
int func_0042BA88(int path, int argLen, int args, int *modResult) {
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
 * @p a3, reading a 16-byte reply back in place. A zero first reply word gives 0xFFFEFFFD; otherwise the first two words
 * go to @p a2[0] and @p a2[1]. Called by func_0042BBB0 and func_0042BBD0 with request 1.
 *
 * @param a0  the ELF's path
 * @param a1  section name ("all" for the whole file)
 * @param a2  receives the entry point and gp
 * @param a3  module-loader request number
 * @return 0, 0xFFFEFFFD when nothing was loaded, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BAA8_i(char *a0, char *a1, int *a2, int a3) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(D_0052F080.cmd.n1, a0, 0xFC);
    D_0052F080.cmd.n1[0xFB] = 0;
    strncpy(D_0052F080.cmd.n2, a1, 0xFC);
    D_0052F080.cmd.n2[0xFB] = 0;
    if (func_00426D18(&D_0052F280, a3, 0, &D_0052F080, 0x200, &D_0052F080, 0x10, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    if (D_0052F080.cmd.f0 == 0) {
        return 0xFFFEFFFD;
    }
    a2[0] = D_0052F080.cmd.f0;
    a2[1] = D_0052F080.cmd.f4;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BBB0);
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
int func_0042BBB0(int path, char *secName, int execData) {
    return func_0042BAA8(path, secName, execData, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BBD0);
#ifdef SKIP_ASM
extern char D_0049AD58[];
extern int func_0042BAA8(int a0, char *a1, int a2, int a3);
/**
 * Loads a whole ELF file into EE memory: func_0042BAA8 with the section name "all" (D_0049AD58) and request 1.
 *
 * @param a0  the ELF's path
 * @param a1  receives the entry point and gp
 * @return the result of func_0042BAA8
 */
int func_0042BBD0(int a0, int a1) {
    return func_0042BAA8(a0, D_0049AD58, a1, 1);
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
 * address and size (0x20 bytes) and stores the reply's first byte, halfword or word to @p a1. It does not check the
 * server version. No caller in this build.
 *
 * @param a0  IOP address
 * @param a1  where the value is stored
 * @param a2  size: 0 byte, 1 halfword, 2 word
 * @return 0, 0xFFFEFFFE for a bad size, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BBF8(int a0, void *a1, unsigned a2) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (a2 > 2) {
        return 0xFFFEFFFE;
    }
    D_0052F080.get.f0.w = a0;
    D_0052F080.get.f4 = a2;
    if (func_00426D18(&D_0052F280, 3, 0, &D_0052F080, 0x20, &D_0052F080, 0x20, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    if (a2 == 0) {
        *(unsigned char *)a1 = D_0052F080.get.f0.b;
    } else if (a2 == 1) {
        *(unsigned short *)a1 = D_0052F080.get.f0.h;
    } else if (a2 == 2) {
        *(int *)a1 = D_0052F080.get.f0.w;
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
 * address, the size and the value read from @p a1 (0x20 bytes, 16-byte reply). It does not check the server version. No
 * caller in this build.
 *
 * @param a0  IOP address
 * @param a1  the value to write
 * @param a2  size: 0 byte, 1 halfword, 2 word
 * @return 0, 0xFFFEFFFE for a bad size, or 0xFFFEFFFF when the RPC fails
 */
int func_0042BCE8(int a0, void *a1, int a2) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    D_0052F080.set.f0 = a0;
    D_0052F080.set.f4 = a2;
    if (a2 == 0) {
        D_0052F080.set.f8.b = *(unsigned char *)a1;
    } else if (a2 == 1) {
        D_0052F080.set.f8.h = *(unsigned short *)a1;
    } else if (a2 == 2) {
        D_0052F080.set.f8.w = *(unsigned *)a1;
    } else {
        return 0xFFFEFFFE;
    }
    if (func_00426D18(&D_0052F280, 2, 0, &D_0052F080.set, 0x20, &D_0052F080.set, 0x10, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BDC8);
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
 * 0x68-byte command D_0052F2C0: id 0x80000003, the argument string @p s (copied without its NUL, unchecked against the
 * 0x50-byte field), its length and @p mode. Writes it back from the data cache, sets SIF register 4 to 0x40000 and DMAs
 * it (attribute 0x44) with sceSifSetDma. When the transfer is queued it sets register 4 to 0x10000 and 0x20000 and
 * clears registers 0x80000002 and 0x80000000. Called by func_0042BF68.
 *
 * @param s  boot argument string (e.g. "rom0:UDNL <image>")
 * @param mode  reset mode passed to the IOP
 * @return 1 when the command was queued, 0 when the DMA was refused
 */
int func_0042BDC8(char *s, int mode) {
    sSifDmaData dma;
    int reg;
    int n;
    fBDC8_stop();
    reg = fBDC8_getreg(0x80000000);
    D_0052F2C0.mode = mode;
    for (n = 0; s[n] != 0; n++) D_0052F2C0.arg[n] = s[n];
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
INCLUDE_ASM("seg/seg_325220", func_0042BF08);
#ifdef SKIP_ASM
extern int sceSifGetReg(int a0);
/**
 * Returns whether the IOP has finished SIF initialisation: bit 0x10000 of SIF register 4 (sceSifGetReg).
 *
 * @return 1 when the bit is set, else 0
 */
int func_0042BF08(void) { int bootEndFlag = sceSifGetReg(4) & 0x10000; return bootEndFlag != 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BF30);
#ifdef SKIP_ASM
extern int sceSifGetReg(int a0);
extern void func_00424220(void);
/**
 * Returns whether the IOP has finished rebooting.
 *
 * When bit 0x40000 of SIF register 4 (sceSifGetReg) is set it clears D_00455168 (func_00424220) and returns 1.
 * func_00319820 polls it after rebooting the IOP with func_0042BF68.
 *
 * @return 1 when the reboot is done, else 0
 */
int func_0042BF30(void) {
    if (sceSifGetReg(4) & 0x40000) {
        func_00424220();
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BF68);
#ifdef SKIP_ASM
/* sceSifRebootIop (libkernel): reset the IOP with "rom0:UDNL <img>". */
extern void func_00425C60_s(const char *fmt, ...) __asm__("func_00425C60");
/**
 * Reboots the IOP with an IOP image through "rom0:UDNL".
 *
 * Refuses an image path whose length plus 11 exceeds 80 (prints "too long parameter '%s'\n" with func_00425C60 and
 * returns 0). Otherwise calls func_00426408(0) and func_004265A8, builds "rom0:UDNL " + @p img and sends it with
 * func_0042BDC8 (mode 0). func_00319820 retries it with the DNAS image until it succeeds.
 *
 * @param img  path of the IOP image
 * @return 1 when the reset command was sent, else 0
 */
int func_0042BF68(const char *img) {
    char buf[80];
    const char *prefix = "rom0:UDNL ";
    char *dst;
    const char *imgEnd = img;
    __SIZE_TYPE__ t;
    while (*imgEnd)
        imgEnd++;
    /* PORT: integer pointer arithmetic (__SIZE_TYPE__ is pointer-width). */
    /* NOTE: the length test is retail's p - (img - 11) > 80 (strlen + 11 > 80); the separate statement keeps fold from
       reassociating it into (p - img) + 11. Integer arithmetic, so no out-of-bounds pointer. */
    t = (__SIZE_TYPE__)img - 11;
    if ((__SIZE_TYPE__)imgEnd - t > 80) {
        func_00425C60_s("too long parameter '%s'\n", img);
        return 0;
    }
    func_00426408_z(0);
    func_004265A8();
    dst = buf;
    while (*prefix)
        *dst++ = *prefix++;
    while (*img)
        *dst++ = *img++;
    *dst = 0;
    return func_0042BDC8(buf, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C078);
#ifdef SKIP_ASM
/* DIntr (libkernel): disable interrupts, return whether they were enabled. */
int func_0042C078_r(void) {
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
INCLUDE_ASM("seg/seg_325220", func_0042C0C0);
#ifdef SKIP_ASM
int func_0042C0C0_r(void) __asm__("func_0042C0C0");
/**
 * Enables interrupts (ei) and returns whether they were already enabled.
 *
 * @return 1 when Status.EIE (0x10000) was set before, else 0
 */
int func_0042C0C0_r(void) {
    int s, eie;
    MFC0_STATUS_4248(s);
    eie = s & 0x10000;
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
 * D_00455248 is the one the recursive lock func_00418CA8 / func_00418D08 waits on and signals. Called by func_0042C300
 * at start-up.
 */
void func_0042C0D8(void) {
    int a[8];
    int b[8];
    a[1] = 1;
    a[2] = 1;
    b[1] = 1;
    b[2] = 1;
    D_00455248 = CreateSema(a);
    D_0045524C = CreateSema(b);
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
 * Returns entry @p a0 of the kernel's syscall table: copies the word at D_00455230 + 4 * @p a0 with syscall 90
 * (func_0042C120).
 *
 * D_00455230 is the table base func_0042C1F0 found. No caller in this build.
 *
 * @param a0  syscall number
 * @return the handler address in that slot
 */
int func_0042C1B8(int a0) {
    int v;
    func_0042C120(&v, D_00455230 + a0 * 4, 4);
    return v;
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
 * table base, and the lower candidate is advanced to its next hit until both agree. Called by func_0042C300 at
 * start-up.
 */
void func_0042C1F0(void) {
    int a;
    int b;
    unsigned x;
    unsigned y;
    func_0042C2F0(D_00455238[0], D_00455238[1]);
    func_0042C2F0(D_00455238[2], D_00455238[3]);
    a = FindAddress(0x80000000, 0x80080000, (void *)func_0042C168);
    x = a - 524;
    b = FindAddress(0x80000000, 0x80080000, (void *)func_0042C130);
    y = b - 360;
    while (x != y) {
        if (x < y) {
            a = FindAddress(a + 4, 0x80080000, (void *)func_0042C168);
            x = a - 524;
        } else {
            b = FindAddress(b + 4, 0x80080000, (void *)func_0042C130);
            y = b - 360;
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
INCLUDE_ASM("seg/seg_325220", func_0042C300);
#ifdef SKIP_ASM
extern void func_0042C0D8(void);
extern void func_0042C1F0(void);
extern void func_0042C7C8(void);
extern void func_00424B78(void);
extern void func_0042C410(void);
extern void func_0042CBD0(void);
/**
 * Runs the kernel library's start-up initialisation.
 *
 * Creates the two library semaphores (func_0042C0D8), installs the helper syscalls and finds the syscall table
 * (func_0042C1F0), patches the alarm syscalls (func_0042C7C8), starts the wake-up helper thread (func_00424B78),
 * patches the OSD configuration syscall when needed (func_0042C410) and installs the TLB syscall patch (func_0042CBD0).
 * Called by the program entry func_00100008 before main.
 */
void func_0042C300(void) {
    func_0042C0D8();
    func_0042C1F0();
    func_0042C7C8();
    func_00424B78();
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
 * third pair (0xFFFFC402), the handler address syscall 91 returns for it. Called by func_0042C300.
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
 * D_00456538 holds a table of string addresses followed, from +0x40, by the strings: first @p a0, then each argv
 * string, each with its NUL; the addresses and strings are written with syscall 90 (func_0042C4C0). Called by
 * func_0042C630, func_0042C6A0 and func_0042C710.
 *
 * @param a0  program name (first string)
 * @param argc  number of argument strings
 * @param argv  the argument strings
 * @return the kernel address of the copied name (D_00456538 + 0x40)
 */
int func_0042C510_d(char *a0, int argc, char **argv) {
    int pos;
    int p = D_00456538;
    int ret;
    int len;
    char **q;
    pos = p + 0x40;
    ret = pos;
    func_0042C500(D_00455A10[0], D_00455A10[1]);
    if (argc >= 0x10) {
        argc = 0xF;
    }
    func_0042C4C0(p, &pos, 4);
    p += 4;
    len = strlen(a0) + 1;
    func_0042C4C0(pos, a0, len);
    pos += len;
    if (argc > 0) {
        q = argv;
        do {
            func_0042C4C0(p, &pos, 4);
            p += 4;
            argc--;
            len = strlen(*q) + 1;
            func_0042C4C0(pos, *q, len);
            q++;
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
 * Called before control leaves the program (func_0042C630, func_0042C6A0, func_0042C6E8, func_0042C710).
 */
void func_0042C628(void) {
    func_0042CD58_v();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C630);
#ifdef SKIP_ASM
extern char D_0049AD90[];
extern void func_0042C510_v(char *, int, int) __asm__("func_0042C510");
extern void func_0042C628(void);
extern int _ExecPS2(int, int, int, int);
/**
 * Starts code already in memory with new arguments: copies the arguments to the kernel and calls the syscall 7 stub.
 *
 * Copies an empty name plus @p a3's @p a2 strings into the kernel argument area (func_0042C510), restores the TLB
 * layout (func_0042C628) and calls the syscall 7 stub (seg_3249A0) with the entry point, gp, the argument count and the
 * kernel copy of the argument table (D_00456538 + 4). No caller in this build.
 *
 * @param a0  entry point
 * @param a1  gp value
 * @param a2  argument count
 * @param a3  argument strings
 * @return the syscall's result (it does not return on success)
 */
int func_0042C630(int a0, int a1, int a2, int a3) {
    func_0042C510_v(D_0049AD90, a2, a3);
    func_0042C628();
    return _ExecPS2(a0, a1, a2, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C6A0);
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
 * @param a0  path of the program
 * @param a1  argument count
 */
void func_0042C6A0(int a0, int a1) {
    int r = func_0042C510(a0, a1);
    func_0042C628();
    _LoadExecPS2(r, a1, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C6E8);
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
void func_0042C6E8(int status) {
    func_0042C628();
    KExit(status);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C710);
#ifdef SKIP_ASM
extern char D_0049AD90[];
extern int func_0042C510_3(char *a0, int a1, int a2) __asm__("func_0042C510");
extern void _ExecOSD(int a0, int a1);
/**
 * Returns to the console's OSD (browser) with arguments: copies them to the kernel and calls the syscall 123 stub.
 *
 * Copies an empty name plus the argument strings into the kernel argument area (func_0042C510), restores the TLB
 * layout (func_0042C628) and calls the syscall 123 stub (seg_3249A0) with the count and the kernel argument table
 * (D_00456538 + 4). No caller in this build.
 *
 * @param a0  argument count
 * @param a1  argument strings
 */
void func_0042C710(int a0, int a1) {
    func_0042C510_3(D_0049AD90, a0, a1);
    func_0042C628();
    _ExecOSD(a0, D_00456538 + 4);
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
 * returns for it. Called by func_0042C300.
 */
void func_0042C7C8(void) {
    unsigned i;
    int *p;
    if (!(*(volatile int *)0x10001810 & 0x100)) {
        func_0042C760(D_00456180[0], D_00456180[1]);
        func_0042C770(0x80076000, D_00455A18, 1856);
        func_0042C770(0x82000, D_00456158, 40);
        FlushCache(0);
        FlushCache(2);
        func_0042C760(D_00456180[2], D_00456180[3]);
        p = &D_00456180[4];
        for (i = 2; i < 8; i++) {
            func_0042C760(p[0], func_0042C7B8(p[0]));
            p += 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C8A0);
#ifdef SKIP_ASM
extern char D_0052F328[];
extern int Deci2Call(int a0, int *a1);
int f8A0(unsigned short a0, int a1, int a2) __asm__("func_0042C8A0");
/**
 * Opens a DECI2 socket for a protocol (DECI2 kernel call 1).
 *
 * Passes the protocol, @p a1, the event handler and the uncached address of the work area D_0052F328 to Deci2Call(1).
 * The serial console set-up func_00425260 calls it.
 *
 * @param a0  DECI2 protocol number
 * @param a1  user data passed to the handler
 * @param a2  event handler
 * @return the socket id, or a negative error
 */
int f8A0(unsigned short a0, int a1, int a2) {
    int buf[4];
    buf[0] = a0;
    buf[1] = a1;
    buf[2] = a2;
    buf[3] = (int)D_0052F328 | 0x20000000;
    return Deci2Call(1, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C8E8);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Closes a DECI2 socket: Deci2Call(2) with the socket id.
 *
 * @param socketId  the socket
 * @return the kernel's result
 */
int func_0042C8E8(int socketId) {
    return Deci2Call(2, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C910);
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
void func_0042C910(int socketId, char dest) {
    int buf[2];
    buf[0] = socketId;
    buf[1] = dest;
    Deci2Call(3, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C940);
#ifdef SKIP_ASM
extern int Deci2Call(int a0, int *a1);
/**
 * Polls a DECI2 socket: Deci2Call(4) with the socket id.
 *
 * Called by the console send routine func_00425040 after func_0042C910.
 *
 * @param socketId  the socket
 * @return the kernel's result
 */
int func_0042C940(int socketId) {
    return Deci2Call(4, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C968);
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
void func_0042C968(int socketId, int dataBuf, unsigned short len) {
    int buf[3];
    buf[0] = socketId;
    buf[1] = dataBuf;
    buf[2] = len;
    Deci2Call(-5, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C9A0);
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
void func_0042C9A0(int socketId, int dataBuf, unsigned short len) {
    int buf[3];
    buf[0] = socketId;
    buf[1] = dataBuf;
    buf[2] = len;
    Deci2Call(-6, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C9D8);
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
void func_0042C9D8(int socketId, char dest) {
    int buf[2];
    buf[0] = socketId;
    buf[1] = dest;
    Deci2Call(-7, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA08);
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
int func_0042CA08(int socketId) {
    return Deci2Call(-8, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA30);
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
int func_0042CA30(int socketId) {
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
 * Stores @p a0 in D_004564F0 and points the TLB-refill vectors for exception codes 1, 2 and 3 at the handler stub
 * func_0042CFC0 (SetVTLBRefillHandler), which saves the registers and calls the stored function. No caller in this
 * build.
 *
 * @param a0  function called on a TLB exception
 * @return @p a0
 */
int func_0042CA80(int a0) {
    D_004564F0 = a0;
    SetVTLBRefillHandler(1, func_0042CFC0);
    SetVTLBRefillHandler(2, func_0042CFC0);
    SetVTLBRefillHandler(3, func_0042CFC0);
    return a0;
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
 * Codes 1-13 only: records @p a1 in D_004564F8[@p a0] and points the kernel's vector for the code at the dispatch stub
 * D_0042D200 (SetVTLBRefillHandler for codes 1-3, SetVCommonHandler otherwise), which calls the recorded handler. No
 * caller in this build.
 *
 * @param a0  exception code (1-13)
 * @param a1  the handler
 * @return the previous handler, or 0xFFFFFFFF for a code outside 1-13
 */
unsigned func_0042CAE0(int a0, int a1) {
    int old;
    if (a0 - 1 >= 13U) {
        return 0xFFFFFFFF;
    }
    old = D_004564F8[a0];
    D_004564F8[a0] = a1;
    if (a0 - 1 < 3U) {
        SetVTLBRefillHandler(a0, (void (*)(void))D_0042D200);
    } else {
        SetVCommonHandler(a0, (void (*)(void))D_0042D200);
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
 * in D_00456538, the kernel area func_0042C510 writes exec arguments to. func_0042C300 jumps to it last.
 */
void func_0042CBD0(void) {
    unsigned i;
    int *p;
    func_0042CBC0(D_00456540[0], D_00456540[1]);
    func_0042CB68(0x80075000, D_004561C0, 816);
    FlushCache(0);
    FlushCache(2);
    func_0042CBC0(D_00456540[2], D_00456540[3]);
    func_0042CBC0(D_00456540[4], D_00456540[5]);
    p = &D_00456540[6];
    for (i = 3; i < 8; i++) {
        func_0042CBC0(p[0], func_0042CBB0(p[0]));
        p += 2;
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
INCLUDE_ASM("seg/seg_325220", func_0042CCA8);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall -85 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. A negative number selects the interrupt-context variant. No caller
 * in this build.
 */
SYSCALL_STUB(func_0042CCA8, -85)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CCB8);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 86 with the caller's arguments (index, PageMask,
 * EntryHi, EntryLo0, EntryLo1).
 *
 * Loads the number into $v1 and executes `syscall`. Called by func_0042CCC8 (index-checked) and the TLB set-up
 * func_0042CD98.
 */
SYSCALL_STUB(func_0042CCB8, 86)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CCC8);
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
int func_0042CCC8(int tlbIndex) {
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
INCLUDE_ASM("seg/seg_325220", func_0042CD08);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 87 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. No caller in this build.
 */
SYSCALL_STUB(func_0042CD08, 87)
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
INCLUDE_ASM("seg/seg_325220", func_0042CD38);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall -88 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. A negative number selects the interrupt-context variant. No caller
 * in this build.
 */
SYSCALL_STUB(func_0042CD38, -88)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD48);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 89 with the caller's arguments.
 *
 * Loads the number into $v1 and executes `syscall`. No caller in this build.
 */
SYSCALL_STUB(func_0042CD48, 89)
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
extern void func_0042C6E8(int);
extern int func_0042CCB8_tlb(int, int, int, int, int) __asm__("func_0042CCB8");
/**
 * Programs the TLB from the layout table D_004567F0 and fills the remaining entries with unmapped pages.
 *
 * Prints the layout ("# TLB spad=0 kernel=1:%d default=%d:%d extended=%d:%d", func_00425C28) and clears the Wired
 * register. Writes the kernel entries, then the default entries (four words each: PageMask, EntryHi, EntryLo0,
 * EntryLo1) with syscall 86 (func_0042CCB8), records their total in the table's count word and makes them wired, then
 * writes the extended entries when there are any. Each group that would pass entry 48 prints "# TLB over flow (1)",
 * "(2)" or "(3)" and exits with status 1 (func_0042C6E8). Entries left up to 47 get EntryHi 0xE0000000 + index * 0x2000
 * and no mapping. Called by func_0042CD58.
 *
 * @return the number of programmed entries shifted left by 13 (not the count itself)
 */
// FAKEMATCH: `register int i FM_REG("$25")` pins the TLB index to $25, where retail keeps it across the calls to the
// func_0042CCB8 syscall stub (no allocator picks a call-clobbered register there, so the original most likely declared
// it as a register variable); plain form 88.93%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int func_0042CD98(void) {
    struct sTlbCfg_4248 *c = &D_004567F0;
    register int i FM_REG("$25");
    int n, k, *p;
    func_00425C28_pf(D_0049AD98, c->a - 1, c->a, c->a + c->b - 1, c->a + c->b, c->a + c->b + c->c - 1);
    MTC0_WIRED_4248(0);
    n = c->a;
    i = 0;
    if (n > 48) {
        func_00425C28_pf(D_0049ADD0);
        func_0042C6E8(1);
    }
    p = c->t1;
    for (; i < n; i++, p += 4) func_0042CCB8_tlb(i, p[0], p[1], p[2], p[3]);
    c = &D_004567F0;
    n = i + c->b;
    if (n > 48) {
        func_00425C28_pf(D_0049ADE8);
        func_0042C6E8(1);
    }
    p = c->t2;
    for (; i < n; i++, p += 4) func_0042CCB8_tlb(i, p[0], p[1], p[2], p[3]);
    c = &D_004567F0;
    c->cnt = i;
    MTC0_WIRED_4248(i);
    if (c->c > 0) {
        n = i + c->c;
        if (n > 48) {
            func_00425C28_pf(D_0049AE00);
            func_0042C6E8(1);
        }
        p = c->t3;
        for (; i < n; i++, p += 4) func_0042CCB8_tlb(i, p[0], p[1], p[2], p[3]);
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
extern long func_0042D380(double);
/**
 * Converts a double to a signed 64-bit integer (libgcc helper).
 *
 * Converts the magnitude with func_0042D380 (the unsigned conversion) and negates it for negative inputs.
 *
 * @param value  value to convert
 * @return the value truncated toward zero
 */
long __fixdfdi(double value) {
    if (dpcmp(value, 0.0) < 0)
        return -func_0042D380(dpsub(0.0, value));
    return func_0042D380(value);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042D380);
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
long func_0042D380(double value) {
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
INCLUDE_ASM("seg/seg_325220", func_0042D470);
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

DItype __moddi3 (DItype u, DItype v) __asm__("func_0042D470");

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
INCLUDE_ASM("seg/seg_325220", func_0042DAD8);
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

UDItype __umoddi3 (UDItype u, UDItype v) __asm__("func_0042DAD8");

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
