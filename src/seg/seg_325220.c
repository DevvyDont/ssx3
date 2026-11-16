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


/* SIF DMA transfer descriptor (as passed to func_00424160 / func_00424170) */
typedef struct { int data; int addr; int size; int attr; } sSifDmaData;
extern int func_00424160(sSifDmaData *sdd, int n);
extern int func_00424170(sSifDmaData *sdd, int n);


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
extern void func_00424110_vs(volatile int *, volatile unsigned long *) __asm__("func_00424110");
unsigned long func_004242C0(void) {
    volatile int flag;
    volatile unsigned long gsCsr;
    int intrEnabled;
    flag = 0;
    func_00424110_vs(&flag, &gsCsr);
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
extern unsigned func_00423D80(void);
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
    if (func_00423D80() < end) {
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
        if (!(t < lo) && !(hi < t)) CACHE_4248(0x14, 0, i);
        CACHE_4248(0x10, 1, i);
        MFC0_TAGLO_4248(t);
        t = (t & 0xFFFFF000) + i;
        if (!(t < lo) && !(hi < t)) CACHE_4248(0x14, 1, i);
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
        if (!(t < lo) && !(hi < t)) CACHE_4248(0x16, 0, i);
        CACHE_4248(0x10, 1, i);
        MFC0_TAGLO_4248(t);
        t = (t & 0xFFFFF000) + i;
        if (!(t < lo) && !(hi < t)) CACHE_4248(0x16, 1, i);
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
extern int func_00423AF0(int);
int func_00424880(int cause) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = func_00423AF0(cause);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004248E8);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern int func_00423AE0(int);
int func_004248E8(int cause) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = func_00423AE0(cause);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424950);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern int func_00423B10(int);
int func_00424950(int channel) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = func_00423B10(channel);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004249B8);
#ifdef SKIP_ASM
extern void func_0042C078(void);
extern int func_00423B00(int);
int func_004249B8(int channel) {
    int s, result;
    MFC0_STATUS_4248(s);
    s &= 0x10000;
    if (s) func_0042C078();
    result = func_00423B00(channel);
    SYNC_4248();
    if (s) func_0042C0C0();
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A20);
#ifdef SKIP_ASM
extern void func_00423B40(void);
void func_00424A20(void) {
    func_00423B40();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A40);
#ifdef SKIP_ASM
extern void func_00423B50(void);
void func_00424A40(void) {
    func_00423B50();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A60);
#ifdef SKIP_ASM
extern void func_00423B60(void);
void func_00424A60(void) {
    func_00423B60();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424A80);
#ifdef SKIP_ASM
extern void func_00423B70(void);
void func_00424A80(void) {
    func_00423B70();
    __asm__ volatile("sync");
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424AA0);
#ifdef SKIP_ASM
extern char D_0049AAE0[];
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
extern void func_00423C50(int);
extern void func_00423CD0(int);
extern void func_00423D10(int);
void func_00424AA0(unsigned *p) {
    unsigned i;
    unsigned j;
    unsigned char *t = (unsigned char *)p + 8;
    unsigned char *v = (unsigned char *)p + 9;
    for (;;) {
        func_00423DE0_i(D_0052B3A0);
        i = *p & 0x1FF;
        j = i << 1;
        *p = i + 1;
        switch (*(t + j)) {
        case 0:
            func_00423CD0(*(v + j));
            break;
        case 1:
            func_00423C50(*(v + j));
            break;
        case 2:
            func_00423D10(*(v + j));
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
extern int func_00423DA0(int *p);
extern int func_00423BA0(int *);
extern void func_00423BC0(int, void *);
extern int func_00423C90(void);
extern void func_00423C30(int, int);
extern void func_00423DB0(int);
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
    D_0052B3A0 = func_00423DA0(buf);
    if (D_0052B3A0 < 0) {
        return -1;
    }
    cfg[1] = (int)func_00424AA0;
    cfg[2] = (int)D_0052AFA0;
    cfg[3] = 0x400;
    cfg[4] = (int)D_004A30F0;
    cfg[5] = 0;
    D_00455170 = func_00423BA0(cfg);
    if (D_00455170 < 0) {
        func_00423DB0(D_0052B3A0);
        return -1;
    }
    D_0052B3A8.f0 = 0;
    D_0052B3A8.idx = 0;
    func_00423BC0(D_00455170, &D_0052B3A8);
    func_00423C30(func_00423C90(), 1);
    return D_00455170;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424C50);
#ifdef SKIP_ASM
extern void func_00423DD0(int);
extern int func_00423CE0(void);
// PORT: PS2 kernel syscall (number in $3, result in $2).
#define SYSCALL_RET_4C50(n, r) do { register int r_ __asm__("$2"); __asm__ volatile("li $3, " #n "\n\tsyscall" : "=r"(r_) : : "$3"); r = r_; } while (0)
int func_00424C50_r(int a0) __asm__("func_00424C50");
int func_00424C50_r(int a0) {
    int id;
    unsigned i;
    SYSCALL_RET_4C50(-0x2F, id);
    if (id != a0) return func_00423CE0();
    if ((unsigned)id >= 0x100 || D_00455170 == 0) return -1;
    i = D_0052B3A8.idx & 0x1FF;
    D_0052B3A8.idx = i + 1;
    D_0052B3A8.e[i].a = 0;
    D_0052B3A8.e[i].b = id;
    func_00423DD0(D_0052B3A0);
    return id;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424CE8);
#ifdef SKIP_ASM
extern void func_00423DD0(int);
int func_00424CE8(unsigned a0) {
    unsigned i;
    if (a0 >= 0x80 || D_00455170 == 0) {
        return -1;
    }
    i = D_0052B3A8.idx & 0x1FF;
    D_0052B3A8.idx = i + 1;
    D_0052B3A8.e[i].a = 1;
    D_0052B3A8.e[i].b = a0;
    func_00423DD0(D_0052B3A0);
    return a0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00424D68);
#ifdef SKIP_ASM
extern int func_00423D20(void);
int func_00424D68_r(int a0) __asm__("func_00424D68");
int func_00424D68_r(int a0) {
    int id;
    unsigned i;
    SYSCALL_RET_4C50(-0x2F, id);
    if (id != a0) return func_00423D20();
    if ((unsigned)id >= 0x100 || D_00455170 == 0) return -1;
    i = D_0052B3A8.idx & 0x1FF;
    D_0052B3A8.idx = i + 1;
    D_0052B3A8.e[i].a = 2;
    D_0052B3A8.e[i].b = id;
    func_00423DD0(D_0052B3A0);
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
            if ((unsigned)(s->pos + n) > 0x140)
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
extern void func_00424020(int);
extern int f5260_open(int, void *, void *) __asm__("func_0042C8A0");
int func_00425260(void) {
    struct Pkt_5260 *p;
    func_00424020(0);
    D_0052B8D0.fd = f5260_open(0x210, &D_0052B8D0, func_00424EA8);
    if (D_0052B8D0.fd < 0) return 0;
    D_0052B8D0.fC = 0;
    D_0052B8D0.wlen = 0;
    D_0052B8D0.pos = 0;
    D_0052B8D0.buf = (unsigned char *)((int)D_0052BA40 | 0x20000000);
    p = (struct Pkt_5260 *)((int)D_0052B900 | 0x20000000);
    D_0052B8D0.wp = (char *)p;
    p->f4 = 0x210;
    p->f6 = 0x45;
    p->f7 = 0x48;
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
    exponent -= 0x433;
    if (exponent < -0x35) {
        return 0;
    }
    if (exponent >= 0xD) {
        return 0x270F;
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
        D_00455178(0x2D);
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
extern void func_00424180(void);
extern int func_004241B0(int a0);
extern unsigned int f5D38_getreg(unsigned int) __asm__("func_004241B0");
extern int func_004241A0(int, int);
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
    func_00424020(0);
    if (*(volatile int *)0x1000E010 & 0x20)
        *(volatile int *)0x1000E010 = 0x20;
    if (!(*(volatile int *)0x1000C000 & 0x100))
        func_00424180();
    D_0052BCD4 = f5D38_adddh(5, func_00426230, 0);
    func_004249B8(5);
    cmd_5D38.iopbuf = (void *)f5D38_getreg(0x80000000);
    if (cmd_5D38.iopbuf) {
        D_0052BCC0[4] = (int)D_0052BC00;
        f5D38_send(0x80000000, D_0052BCC0, 0x14, 0, 0, 0);
        return;
    } else {
        while (!(f5D38_getreg(4) & 0x20000))
            ;
        cmd_5D38.iopbuf = (void *)f5D38_getreg(2);
        func_004241A0(0x80000000, (int)cmd_5D38.iopbuf);
        func_004241A0(0x80000001, (int)&cmd_5D38);
        D_0052BCC0[4] = (int)D_0052BC00;
        D_0052BCC0[3] = 0;
        f5D38_send(0x80000002, D_0052BCC0, 0x14, 0, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00425FB8);
#ifdef SKIP_ASM
extern int func_00424950(int a0);
extern int func_00423AD0(int a0, int a1);
extern int D_0045517C;
extern int D_0052BCD4;
void func_00425FB8(void) {
    func_00424950(5);
    func_00423AD0(5, D_0052BCD4);
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
    ent[n].attr = 0x44;
    func_00426358((int)a2, a3);
    n++;
    if (a1 & 1) {
        return func_00424170(ent, n);
    }
    return func_00424160(ent, n);
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
extern void func_00423DD0(int);
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
        func_00423DD0(q->f8);
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
extern int func_00423DA0_j(int *) __asm__("func_00423DA0");
extern int func_00423DE0_j(int) __asm__("func_00423DE0");
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
        a0->f8 = func_00423DA0_j(buf);
        if (a0->f8 < 0) {
            func_00426678_j(t);
            return -3;
        }
        if (func_004261B0_j(0x8000000C, (int)t, 0x40, 0, 0, 0) == 0) {
            func_00426678_j(t);
            func_00423DB0(a0->f8);
            return -2;
        }
        func_00423DE0_j(a0->f8);
        func_00423DB0(a0->f8);
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
extern int func_00423DA0(int *);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
        a0->f8 = func_00423DA0(buf);
        if (a0->f8 < 0) {
            func_00426678_i(t);
            return -3;
        }
        if (func_004261B0(0x80000009, (int)t, 0x40, 0, 0, 0) == 0) {
            func_00426678_i(t);
            func_00423DB0(a0->f8);
            return -2;
        }
        func_00423DE0_i(a0->f8);
        func_00423DB0(a0->f8);
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
extern int f6D18_csema(struct Sema6D18 *) __asm__("func_00423DA0");
extern int f6D18_wsema(int) __asm__("func_00423DE0");
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
        func_00423DB0(cd->rpcd.tid);
        f6D18_free(call);
        return -2;
    }
    f6D18_wsema(cd->rpcd.tid);
    func_00423DB0(cd->rpcd.tid);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00426F08);
#ifdef SKIP_ASM
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
        r = func_00424160(ent, n);
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
extern void func_00423CC0(void);
int func_004273F8(int queue) {
    int server;
    while (1) {
        while ((server = func_004271D8(queue)) != 0) {
            func_00427230(server);
        }
        func_00423CC0();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427438);
#ifdef SKIP_ASM
extern int func_00423DA0(int *p);
void func_00427438(void) {
    int buf[8];
    if (D_00455218 == -1) {
        buf[5] = 0;
        buf[2] = 1;
        buf[1] = 1;
        D_00455218 = func_00423DA0(buf);
        D_0045521C = func_00423DA0(buf);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427498);
#ifdef SKIP_ASM
extern void func_00427438(void);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
extern void func_00423DC0(int);
sFioFile *func_00427498(void) {
    sFioFile *p;
    sFioFile *end;
    func_00427438();
    func_00423DE0_i(D_00455218);
    p = D_0052EC00;
    end = p + 32;
    while (p < end) {
        if (p->f4 == 0) {
            p->f4 = 0x10000000;
            func_00423DC0(D_00455218);
            return p;
        }
        p++;
    }
    func_00423DC0(D_00455218);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427520);
#ifdef SKIP_ASM
extern void func_00427438(void);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
extern void func_00423DC0(int);
sFioFile *func_00427520(unsigned a0) {
    sFioFile *p;
    func_00427438();
    func_00423DE0_i(D_00455218);
    if (a0 >= 0x20) {
        func_00423DC0(D_00455218);
        return 0;
    }
    p = &D_0052EC00[a0];
    func_00423DC0(D_00455218);
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
extern void func_00423DD0(int);
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
    case 0xB:
        *(struct W7590 *)&buf = *(struct W7590 *)(p + 0x14);
        *(struct B7590 *)buf = *(struct B7590 *)(p + 0x18);
        break;
    case 0xC:
        *(struct W7590 *)&buf = *(struct W7590 *)(p + 0x14);
        *(struct C7590 *)buf = *(struct C7590 *)(p + 0x18);
        break;
    case 0x17:
    case 0x19:
    case 0x1A:
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
        func_00423DD0(sema);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427938);
#ifdef SKIP_ASM
extern int func_00423DA0(int *p);
void func_00427938(void) {
    int buf[8];
    if (D_00455214 == -1) {
        buf[5] = 0;
        buf[2] = 1;
        buf[1] = 1;
        D_00455214 = func_00423DA0(buf);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427988);
#ifdef SKIP_ASM
extern void func_00427938(void);
extern void func_00423DE0(int a0);
int func_00427988(void) {
    func_00427938();
    func_00423DE0(D_00455214);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004279B8);
#ifdef SKIP_ASM
extern void func_00423DC0(int);
void func_004279B8(void) {
    func_00423DC0(D_00455214);
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
void (*func_004279C8(void (*a0)(int), int a1))(int) {
    int r;
    void (*old)(int);
    sFioCb *p = &D_0052EE80;
    func_00427988_a(0x1B);
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
extern int func_00423DE0_z(int) __asm__("func_00423DE0");
extern int func_00423DC0_z(int) __asm__("func_00423DC0");
int func_00427A90_d(void) __asm__("func_00427A90");
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
    func_00423DE0_z(D_00455218);
    for (p = D_0052EC00; p < &D_0052EC00[32]; p++)
        p->f4 = 0;
    func_00423DC0_z(D_00455218);
    D_0052D6C0[0] = (int)D_0052E380;
    D_0052D6C0[1] = D_0052D6C0[0] + 0x440;
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
extern int func_00423DA0(int *p);
extern int db0i(int) __asm__("func_00423DB0");
extern void func_00423DC0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00427D60_d(unsigned char *, unsigned, ...) __asm__("func_00427D60");
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
        return -0x13;
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
    sem = func_00423DA0(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0, 0, &D_0052D700.open, 0x418, D_0052E340, 4, 0, 0) < 0) {
        db0i(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        db0i(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    db0i(sem);
    if (buf[8] < 0) {
        func_00423DE0_i(D_00455218);
        t->f4 = 0;
        func_00423DC0(D_00455218);
        return buf[8];
    }
    v = idx;
    func_00423DE0_i(D_00455218);
    t->f0 = buf[8];
    t->f4 |= flags;
    func_00423DC0(D_00455218);
    return v;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00427FE8);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
    sem = func_00423DA0(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 1, 0, p, 0x14, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    t->f4 = 0;
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern void func_00423DC0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
  sem = func_00423DA0(&buf[0]);
  out = &buf[8];
  p->f8 = 4;
  p->f4 = (int) out;
  flags = fl;
  D_0052D700.lseek.f0 = sem;
  if (flags & 0x8000)
  {
    func_00423DE0_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    func_00423DC0(D_0045521C);
  }
  if (func_00426D18(&D_0052EE00, 4, 0, &D_0052D700.lseek, 0x1C, D_0052E340, 4, 0, 0) < 0)
  {
    func_00423DB0(sem);
    func_004279B8();
    return -0xB;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
  {
    func_00423DB0(sem);
    return -0xB;
  }
  if (fl & 0x8000)
  {
    func_00423DB0(sem);
    return 0;
  }
  func_00423DE0_i(sem);
  func_00423DB0(sem);
  return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004283A0);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern void func_00423DC0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
  sem = func_00423DA0(&buf[0]);
  out = &buf[8];
  p->f8 = 4;
  p->f4 = (int) out;
  flags = fl;
  D_0052D700.read.f0 = sem;
  if (flags & 0x8000)
  {
    func_00423DE0_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    func_00423DC0(D_0045521C);
  }
  if (!(fl & 0x20000000))
    func_00426358(a1, a2);
  func_00426358((int) p, 0x20);
  if (func_00426D18(&D_0052EE00, 2, 0, &D_0052D700.read, 0x20, D_0052E340, 4, 0, 0) < 0)
  {
    func_00423DB0(sem);
    func_004279B8();
    return -0xB;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
  {
    func_00423DB0(sem);
    return -0xB;
  }
  if (fl & 0x8000)
  {
    func_00423DB0(sem);
    return 0;
  }
  func_00423DE0_i(sem);
  func_00423DB0(sem);
  return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00428600);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern void func_00423DC0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
  sem = func_00423DA0(&buf[0]);
  out = &buf[8];
  p->f8 = 4;
  p->f4 = (int) out;
  flags = fl;
  D_0052D700.write.f0 = sem;
  if (flags & 0x8000)
  {
    func_00423DE0_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    func_00423DC0(D_0045521C);
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
    func_00423DB0(sem);
    func_004279B8();
    return -0xB;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
  {
    func_00423DB0(sem);
    return -0xB;
  }
  if (fl & 0x8000)
  {
    func_00423DB0(sem);
    return 0;
  }
  func_00423DE0_i(sem);
  func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern void func_00423DC0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
struct Blk88C0 { char b[0x400]; };
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
        func_00423DE0_i(D_0045521C);
        for (i = 0; i < 32; i++) {
            if (D_00455188[i] != -1)
                break;
        }
        if (i == 32)
            *D_0052D6C8 = 0;
        else
            *D_0052D6C8 = 1;
        one = 0; /* FAKEMATCH: dead store, keeps `one` a variable */
        func_00423DC0(D_0045521C);
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
        sem = func_00423DA0(&buf[0]);
        p->f0 = sem;
        p->f4 = (int)&buf[8];
        p->f8 = 4;
        func_00426358((int)&D_0052D700.ioctl2, 0x420);
        if (func_00426D18(&D_0052EE00, 5, 0, &D_0052D700.ioctl2, 0x420, D_0052E340, 4, 0, 0) < 0) {
            func_00423DB0(sem);
            func_004279B8();
            return -0xB;
        }
        v = *(int *)((int)D_0052E340 | 0x20000000);
        func_004279B8();
        if (v == 0) {
            func_00423DB0(sem);
            return -0xB;
        }
        func_00423DE0_i(sem);
        func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00428C38_d(int, int, void *, unsigned, int, unsigned) __asm__("func_00428C38");
int func_00428C38_d(int a0, int a1, void *data, unsigned len, int a4, unsigned a5) {
    int buf[12];
    int sem;
    int v;
    sFioFile *t;
    sFioIoctl2Cmd *p = &D_0052D700.ioctl2;
    t = func_00427520(a0);
    func_00427988_b(0x1A);
    if (D_0045520C == 0) {
        func_00427A90();
    }
    if (t == 0 || t->f4 == 0) {
        func_004279B8();
        return -9;
    }
    if (len > 0x400 || a5 > 0x400) {
        func_004279B8();
        return -0x16;
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
    sem = func_00423DA0(&buf[0]);
    p->f418 = a5;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    p->f414 = a4;
    p->f0 = sem;
    func_00426358((int)&D_0052D700.ioctl2, 0x420);
    if (func_00426D18(&D_0052EE00, 0x1A, 0, &D_0052D700.ioctl2, 0x420, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00428E18);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00428E18_d(char *, int) __asm__("func_00428E18");
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, a1, 0, &D_0052D700.path, n + 0xD, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00428FC0);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00428FE0_d(char *, int) __asm__("func_00428FE0");
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 7, 0, &D_0052D700.mkdir, n + 0x11, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429190);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_004291B0_d(char *, char *, char *, int) __asm__("func_004291B0");
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    func_00426358((int)&D_0052D700.format, 0xC10);
    if (func_00426D18(&D_0052EE00, 0xE, 0, &D_0052D700.format, 0xC10, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429420);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
    sem = func_00423DA0(&buf[0]);
    D_0052D700.adddrv.f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0xF, 0, &D_0052D700.adddrv, 0x10, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -1;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -1;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429540);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
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
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
extern void func_00423DC0(int);
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
        return -0x13;
    }
    r = func_00428E18(a0, 9);
    if (r < 0) {
        func_00423DE0_i(D_00455218);
        p->f4 = 0;
        func_00423DC0(D_00455218);
        return r;
    } else {
        func_00423DE0_i(D_00455218);
        p->f0 = r;
        r = p - D_0052EC00;
        func_00423DC0(D_00455218);
        return r;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429628);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00429628(int a0) {
    int buf[12];
    int sem;
    int v;
    sFioFile *e;
    sFioFdCmd *p = &D_0052D700.dclose;
    e = func_00427520(a0);
    func_00427988_b(0xA);
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
    sem = func_00423DA0(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0xA, 0, p, 0x14, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    e->f4 = 0;
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00429790_d(int, int) __asm__("func_00429790");
int func_00429790_d(int a0, int a1) {
    int buf[12];
    int sem;
    int v;
    sFioFile *t;
    sFioFdCmd *p = &D_0052D700.dread;
    t = func_00427520(a0);
    func_00427988_b(0xB);
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
    sem = func_00423DA0(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0xB, 0, p, 0x20, D_0052E340, 4, 0, 0) < 0) {
        func_00423DE0_i(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_004298E8);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0xC, 0, &D_0052D700.getstat, i + 0x11, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00429A88_d(char *, sFioStat *, int) __asm__("func_00429A88");
int func_00429A88_d(char *s, sFioStat *src, int a2) {
    int buf[12];
    int sem;
    int v;
    int n;
    unsigned char c;
    sFioChstatCmd *p = &D_0052D700.chstat;
    func_00427988_b(0xD);
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
    sem = func_00423DA0(&buf[0]);
    p->f0 = sem;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    func_00426358((int)&D_0052D700.chstat, 0x450);
    if (func_00426D18(&D_0052EE00, 0xD, 0, &D_0052D700.chstat, n + 0x51, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00429CC8_d(char *, char *) __asm__("func_00429CC8");
int func_00429CC8_d(char *s, char *t) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPath2Cmd *p = &D_0052D700.rename;
    func_00427988_b(0x11);
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    func_00426358((int)&D_0052D700.rename, 0x80C);
    if (func_00426D18(&D_0052EE00, 0x11, 0, &D_0052D700.rename, 0x80C, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429EB8);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
int func_00429EB8(int path) {
    return func_00428E18(path, 0x12);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_00429ED8);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_00429ED8_d(char *, int) __asm__("func_00429ED8");
int func_00429ED8_d(char *s, int a1) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPathArgCmd *p = &D_0052D700.sync;
    func_00427988_b(0x13);
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0x13, 0, &D_0052D700.sync, 0x414, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_0042A070_d(char *, char *, int, char *, int) __asm__("func_0042A070");
int func_0042A070_d(char *s, char *t, int a2, char *data, int len) {
    int buf[12];
    int sem;
    int v;
    int n;
    int i;
    sFioMountCmd *p = &D_0052D700.mount;
    func_00427988_b(0x14);
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    func_00426358((int)&D_0052D700.mount, 0xC14);
    if (func_00426D18(&D_0052EE00, 0x14, 0, &D_0052D700.mount, 0xC14, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A2E0);
#ifdef SKIP_ASM
extern int func_00428E18(int a0, int a1);
int func_0042A2E0(int path) {
    return func_00428E18(path, 0x15);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A300);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern void func_00423DC0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
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
  func_00427988_b(0x16);
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
  sem = func_00423DA0(&buf[0]);
  p->f4 = (int) &res;
  p->f8 = 8;
  flags = fl;
  D_0052D700.lseek64.f0 = sem;
  if (flags & 0x8000)
  {
    func_00423DE0_i(D_0045521C);
    for (i = 0; i < 0x20; i++)
    {
      if (D_00455188[i] == (-1))
      {
        D_00455188[i] = p->f0;
        p->f0 = -p->f0;
        break;
      }
    }

    func_00423DC0(D_0045521C);
  }
  if (func_00426D18(&D_0052EE00, 0x16, 0, &D_0052D700.lseek64, 0x20, D_0052E340, 4, 0, 0) < 0)
  {
    func_00423DB0(sem);
    func_004279B8();
    return -0xB;
  }
  v = *((int *) (((int) D_0052E340) | 0x20000000));
  func_004279B8();
  if (v == 0)
  {
    func_00423DB0(sem);
    return -0xB;
  }
  if (fl & 0x8000)
  {
    func_00423DB0(sem);
    return 0;
  }
  func_00423DE0_i(sem);
  func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_0042A538_d(char *, int, char *, unsigned, int, unsigned) __asm__("func_0042A538");
int func_0042A538_d(char *s, int a1, char *data, unsigned len, int a4, unsigned a5) {
    int buf[12];
    int sem;
    int v;
    int n;
    unsigned i;
    sFioDevctlCmd *p = &D_0052D700.devctl;
    func_00427988_b(0x17);
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
        return -0x16;
    }
    for (i = 0; i < len; i++) {
        p->data[i] = data[i];
    }
    p->f810 = len;
    p->f80C = a1;
    buf[1] = 1;
    buf[2] = 0;
    buf[5] = 0;
    sem = func_00423DA0(&buf[0]);
    p->f818 = a5;
    p->f4 = (int)&buf[8];
    p->f8 = 4;
    p->f814 = a4;
    p->f0 = sem;
    func_00426358((int)&D_0052D700.devctl, 0x81C);
    if (func_00426D18(&D_0052EE00, 0x17, 0, &D_0052D700.devctl, 0x81C, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042A770);
#ifdef SKIP_ASM
extern int func_00427988_b(int) __asm__("func_00427988");
extern void func_00427A90(void);
extern void func_004279B8(void);
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_0042A770_d(char *, char *) __asm__("func_0042A770");
int func_0042A770_d(char *s, char *t) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPath2Cmd *p = &D_0052D700.symlink;
    func_00427988_b(0x11);
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0x18, 0, &D_0052D700.symlink, 0x80C, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
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
extern int func_00423DA0(int *p);
extern void func_00423DB0(int);
extern int func_00423DE0_i(int) __asm__("func_00423DE0");
int func_0042A950_d(char *, int, unsigned) __asm__("func_0042A950");
int func_0042A950_d(char *s, int a1, unsigned len) {
    int buf[12];
    int sem;
    int v;
    int n;
    sFioPathArgCmd *p = &D_0052D700.readlink;
    func_00427988_b(0x11);
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
    sem = func_00423DA0(&buf[0]);
    p->f4 = (int)&buf[8];
    p->f0 = sem;
    p->f8 = 4;
    if (func_00426D18(&D_0052EE00, 0x19, 0, &D_0052D700.readlink, 0x80C, D_0052E340, 4, 0, 0) < 0) {
        func_00423DB0(sem);
        func_004279B8();
        return -0xB;
    }
    v = *(int *)((int)D_0052E340 | 0x20000000);
    func_004279B8();
    if (v == 0) {
        func_00423DB0(sem);
        return -0xB;
    }
    func_00423DE0_i(sem);
    func_00423DB0(sem);
    return buf[8];
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AB18);
#ifdef SKIP_ASM
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
int func_0042AD08(void) {
    return func_0042AC90();
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042AD28);
#ifdef SKIP_ASM
struct S2AD28 { int cmd; char name[0xFC]; };
extern struct S2AD28 D_0052EF80;
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
        i = 0xFB;
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
        if (a1 >= 0xFD) {
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
        if (a1 >= 0xFD) {
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
int func_0042B770(int a0) {
    if (func_0042B068() < 0) {
        return 0xFFFF0000;
    }
    if (func_0042B168() != 0) {
        return 0xFFFEFFFC;
    }
    D_0052F080.cmd.f0 = a0;
    if (func_00426D18(&D_0052F280, 0xA, 0, &D_0052F080, 4, &D_0052F080, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    return D_0052F080.cmd.f0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B800);
#ifdef SKIP_ASM
extern int func_0042B230(int a0, int a1, int a2, int *a3);
int func_0042B800(int modBuf, int argLen, int args) {
    int sp;
    return func_0042B230(modBuf, argLen, args, &sp);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042B820);
#ifdef SKIP_ASM
extern int func_0042B230(int a0, int a1, int a2, int *a3);
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
        if (a1 >= 0xFD) {
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
int func_0042BA68(int path, int argLen, int args) {
    int sp;
    return func_0042B840(path, argLen, args, &sp, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BA88);
#ifdef SKIP_ASM
extern int func_0042B840(int a0, int a1, int a2, int *a3, int a4);
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
int func_0042BBB0(int path, char *secName, int execData) {
    return func_0042BAA8(path, secName, execData, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BBD0);
#ifdef SKIP_ASM
extern char D_0049AD58[];
extern int func_0042BAA8(int a0, char *a1, int a2, int a3);
int func_0042BBD0(int a0, int a1) {
    return func_0042BAA8(a0, D_0049AD58, a1, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BBF8);
#ifdef SKIP_ASM
extern int func_0042B068(void);
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
extern void fBDC8_stop(void) __asm__("func_00424070");
extern int fBDC8_getreg(unsigned int) __asm__("func_004241B0");
extern unsigned int fBDC8_setreg(unsigned int, unsigned int) __asm__("func_004241A0");
extern void func_00426358(int, int);
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
    D_0052F2C0.psize = 0x68;
    dma.data = (int)&D_0052F2C0;
    dma.addr = reg;
    dma.size = 0x68;
    dma.attr = 0x44;
    func_00426358((int)&D_0052F2C0, 0x68);
    fBDC8_setreg(4, 0x40000);
    if (func_00424160(&dma, 1) != 0) {
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
extern int func_004241B0(int a0);
int func_0042BF08(void) { int bootEndFlag = func_004241B0(4) & 0x10000; return bootEndFlag != 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042BF30);
#ifdef SKIP_ASM
extern int func_004241B0(int a0);
extern void func_00424220(void);
int func_0042BF30(void) {
    if (func_004241B0(4) & 0x40000) {
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
extern int func_00423DA0(int *a0);
void func_0042C0D8(void) {
    int a[8];
    int b[8];
    a[1] = 1;
    a[2] = 1;
    b[1] = 1;
    b[2] = 1;
    D_00455248 = func_00423DA0(a);
    D_0045524C = func_00423DA0(b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C120);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042C120, 0x5A)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C130);
#ifdef SKIP_ASM
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
int *func_0042C168(int *cursor, int *end, int value) {
    while (*cursor != value && cursor < end) cursor++;
    return cursor < end ? cursor : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C1A8);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042C1A8, 0x83)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C1B8);
#ifdef SKIP_ASM
extern void func_0042C120(void *a0, int a1, int a2);
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
extern int func_0042C1A8(int, int, void *);
extern void func_0042C2F0(int, int);
void func_0042C1F0(void) {
    int a;
    int b;
    unsigned x;
    unsigned y;
    func_0042C2F0(D_00455238[0], D_00455238[1]);
    func_0042C2F0(D_00455238[2], D_00455238[3]);
    a = func_0042C1A8(0x80000000, 0x80080000, (void *)func_0042C168);
    x = a - 0x20C;
    b = func_0042C1A8(0x80000000, 0x80080000, (void *)func_0042C130);
    y = b - 0x168;
    while (x != y) {
        if (x < y) {
            a = func_0042C1A8(a + 4, 0x80080000, (void *)func_0042C168);
            x = a - 0x20C;
        } else {
            b = func_0042C1A8(b + 4, 0x80080000, (void *)func_0042C130);
            y = b - 0x168;
        }
    }
    D_00455230 = x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C2F0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042C2F0, 0x74)
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
SYSCALL_STUB(func_0042C340, 0x74)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C350);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042C350, 0x5A)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C360);
#ifdef SKIP_ASM
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
SYSCALL_STUB(func_0042C398, 0x5B)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C3A8);
#ifdef SKIP_ASM
extern void func_00423E50(unsigned *p);
extern void func_00423E40(unsigned *p);
int func_0042C3A8(void) {
    unsigned osdConfig[2];
    func_00423E50(&osdConfig[0]);
    osdConfig[1] = (osdConfig[0] & 0xFFFF1FFF) | 0x2000;
    func_00423E40(&osdConfig[1]);
    func_00423E50(&osdConfig[1]);
    func_00423E40(&osdConfig[0]);
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
extern void func_00424020(int);
extern int func_0042C398(int);
void func_0042C410(void) {
    unsigned i;
    if (func_0042C3A8() != 0) {
        func_0042C340(D_004559F8[0].a, D_004559F8[0].b);
        func_0042C350(0x80074000, D_00455250, 0x7A8);
        func_00424020(0);
        func_00424020(2);
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
SYSCALL_STUB(func_0042C4C0, 0x5A)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C4D0);
#ifdef SKIP_ASM
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
SYSCALL_STUB(func_0042C500, 0x74)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C510);
#ifdef SKIP_ASM
extern int D_00455A10[];
extern int strlen(char *);
extern int func_0042C4C0(int, void *, int);
extern void func_0042C500(int, int);
int func_0042C510_d(char *, int, char **) __asm__("func_0042C510");
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
extern int func_004239F0(int, int, int, int);
int func_0042C630(int a0, int a1, int a2, int a3) {
    func_0042C510_v(D_0049AD90, a2, a3);
    func_0042C628();
    return func_004239F0(a0, a1, a2, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C6A0);
#ifdef SKIP_ASM
extern int func_0042C510(int a0, int a1);
extern void func_004239E0(int a0, int a1, int a2);
void func_0042C6A0(int a0, int a1) {
    int r = func_0042C510(a0, a1);
    func_0042C628();
    func_004239E0(r, a1, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C6E8);
#ifdef SKIP_ASM
extern void func_004239C0(int);
void func_0042C6E8(int status) {
    func_0042C628();
    func_004239C0(status);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C710);
#ifdef SKIP_ASM
extern char D_0049AD90[];
extern int func_0042C510_3(char *a0, int a1, int a2) __asm__("func_0042C510");
extern void func_004241C0(int a0, int a1);
void func_0042C710(int a0, int a1) {
    func_0042C510_3(D_0049AD90, a0, a1);
    func_0042C628();
    func_004241C0(a0, D_00456538 + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C760);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042C760, 0x74)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C770);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042C770, 0x5A)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C780);
#ifdef SKIP_ASM
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
SYSCALL_STUB(func_0042C7B8, 0x5B)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C7C8);
#ifdef SKIP_ASM
extern int D_00456180[];
extern char D_00455A18[];
extern char D_00456158[];
extern void func_00424020(int);
extern int func_0042C7B8(int);
extern void func_0042C760(int, int);
extern void func_0042C770(int, char *, int);
void func_0042C7C8(void) {
    unsigned i;
    int *p;
    if (!(*(volatile int *)0x10001810 & 0x100)) {
        func_0042C760(D_00456180[0], D_00456180[1]);
        func_0042C770(0x80076000, D_00455A18, 0x740);
        func_0042C770(0x82000, D_00456158, 0x28);
        func_00424020(0);
        func_00424020(2);
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
extern int func_004241D0(int a0, int *a1);
int f8A0(unsigned short a0, int a1, int a2) __asm__("func_0042C8A0");
int f8A0(unsigned short a0, int a1, int a2) {
    int buf[4];
    buf[0] = a0;
    buf[1] = a1;
    buf[2] = a2;
    buf[3] = (int)D_0052F328 | 0x20000000;
    return func_004241D0(1, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C8E8);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
int func_0042C8E8(int socketId) {
    return func_004241D0(2, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C910);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
void func_0042C910(int socketId, char dest) {
    int buf[2];
    buf[0] = socketId;
    buf[1] = dest;
    func_004241D0(3, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C940);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
int func_0042C940(int socketId) {
    return func_004241D0(4, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C968);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
void func_0042C968(int socketId, int dataBuf, unsigned short len) {
    int buf[3];
    buf[0] = socketId;
    buf[1] = dataBuf;
    buf[2] = len;
    func_004241D0(-5, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C9A0);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
void func_0042C9A0(int socketId, int dataBuf, unsigned short len) {
    int buf[3];
    buf[0] = socketId;
    buf[1] = dataBuf;
    buf[2] = len;
    func_004241D0(-6, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042C9D8);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
void func_0042C9D8(int socketId, char dest) {
    int buf[2];
    buf[0] = socketId;
    buf[1] = dest;
    func_004241D0(-7, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA08);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
int func_0042CA08(int socketId) {
    return func_004241D0(-8, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA30);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
int func_0042CA30(int socketId) {
    return func_004241D0(-9, &socketId);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA58);
#ifdef SKIP_ASM
extern int func_004241D0(int a0, int *a1);
int func_0042CA58(int str) {
    return func_004241D0(0x10, &str);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CA80);
#ifdef SKIP_ASM
extern int D_004564F0;
extern void func_0042CFC0(void);
extern void func_00423A50(int, void (*)(void));
int func_0042CA80(int a0) {
    D_004564F0 = a0;
    func_00423A50(1, func_0042CFC0);
    func_00423A50(2, func_0042CFC0);
    func_00423A50(3, func_0042CFC0);
    return a0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CAE0);
#ifdef SKIP_ASM
extern int D_004564F8[];
extern char D_0042D200[];
extern void func_00423A50(int, void (*)(void));
extern void func_00423A60(int, void *);
unsigned func_0042CAE0(int a0, int a1) {
    int old;
    if (a0 - 1 >= 13U) {
        return 0xFFFFFFFF;
    }
    old = D_004564F8[a0];
    D_004564F8[a0] = a1;
    if (a0 - 1 < 3U) {
        func_00423A50(a0, (void (*)(void))D_0042D200);
    } else {
        func_00423A60(a0, (void (*)(void))D_0042D200);
    }
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CB68);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CB68, 0x5A)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CB78);
#ifdef SKIP_ASM
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
SYSCALL_STUB(func_0042CBB0, 0x5B)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CBC0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CBC0, 0x74)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CBD0);
#ifdef SKIP_ASM
extern int D_00456540[];
extern char D_004561C0[];
extern void func_00424020(int);
extern int func_0042CBB0(int);
extern void func_0042CBC0(int, int);
extern void func_0042CB68(int, char *, int);
void func_0042CBD0(void) {
    unsigned i;
    int *p;
    func_0042CBC0(D_00456540[0], D_00456540[1]);
    func_0042CB68(0x80075000, D_004561C0, 0x330);
    func_00424020(0);
    func_00424020(2);
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
INCLUDE_ASM("seg/seg_325220", func_0042CC98);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CC98, 0x55)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CCA8);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CCA8, -0x55)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CCB8);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CCB8, 0x56)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CCC8);
#ifdef SKIP_ASM
extern int func_0042CCB8(int a0);
int func_0042CCC8(int tlbIndex) {
    if ((unsigned int)(tlbIndex - 13) >= 0x23)
        return -1;
    return func_0042CCB8(tlbIndex);
}
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CCF8);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CCF8, -0x56)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD08);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CD08, 0x57)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD18);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CD18, -0x57)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD28);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CD28, 0x58)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD38);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CD38, -0x58)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD48);
#ifdef SKIP_ASM
SYSCALL_STUB(func_0042CD48, 0x59)
#endif

//100%
INCLUDE_ASM("seg/seg_325220", func_0042CD58);
#ifdef SKIP_ASM
extern int func_00424200(void);
extern int func_0042CD98(void);
extern int func_00424210(void);
int func_0042CD58(void) {
    if (func_00424200() == 0x2000000) {
        return func_0042CD98();
    }
    return func_00424210();
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
