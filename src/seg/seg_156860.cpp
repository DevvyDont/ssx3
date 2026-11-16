#include "common.h"
#include "online/buddyman.h"
#include "online/lobbyman.h"
#include "be/beintnetwork.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "online/dsockchannel.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_construct);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cCommSystem_cCommSystem_5860(int) __asm__("cCommSystem_cCommSystem");
extern "C" int cMemMan_alloc_5860(unsigned int, void *, unsigned int, int) __asm__("cMemMan_alloc");
extern char str_cCommSystem[];

/**
 * Creates the comm system singleton: allocates 0x34 bytes tagged "cCommSystem" and constructs it.
 *
 * Stores the result in cCommSystem_mCommSystem without checking for an existing one; func_00255898 destroys
 * it.
 */
extern "C" void cCommSystem_construct(void) {
    cCommSystem_mCommSystem = (cCommSystem*)(cCommSystem_cCommSystem_5860(cMemMan_alloc_5860(0x34, str_cCommSystem, 0x20000000, 0)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255898);
#ifdef SKIP_ASM
extern "C" void cCommSystem__cCommSystem(int, int);

extern "C" void func_00255898(void) {
    if (cCommSystem_mCommSystem != 0) {
        cCommSystem__cCommSystem((int)cCommSystem_mCommSystem, 3);
        cCommSystem_mCommSystem = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_cCommSystem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void ccs_5698(void *) __asm__("MUTEX_create");
extern "C" void ccs_6448(void *, int, int) __asm__("MEM_fill");
extern "C" void *ccs_new(unsigned int, void *, unsigned int, int) __asm__("operator_new__FUi");
extern "C" void ccs_3B18(void *) __asm__("func_003F3B18");
extern char str_Comm_pool[];
extern char str_www_ea_com[];
extern int cCommSystem_mNumNetConn;
extern "C" int *cCommSystem_cCommSystem_(int *self) __asm__("cCommSystem_cCommSystem");
/**
 * Constructs the comm system: its message-element pool, the channel slot and an idle net connection.
 *
 * Creates the pool mutex, allocates the zeroed 0xFC00-byte "Comm pool" and links its 256 tCommMsgElem entries
 * into the free list, clears the channel slot and cCommSystem_mNumNetConn, sets mActiveNetConn to -1 and
 * mNetConnState and mInterfaceType to 1, then sends the IOP's 'netw'/'crea' request
 * (func_003F3B18("www.ea.com")).
 *
 * @return self
 */
extern "C" int *cCommSystem_cCommSystem_(int *self) {
    int nextOffset, blockOffset, n;
    int i;
    ccs_5698(self + 1);
    ccs_6448(self, 0, 4);
    self[5] = (int)ccs_new(0xFC00, str_Comm_pool, 0x20000000, 0);
    ccs_6448((void *)self[5], 0, 0xFC00);
    self[6] = self[5];
    nextOffset = 0xFC;
    blockOffset = 0;
    n = 0xFE;
    do {
        n--;
        *(int *)(blockOffset + self[5]) = self[5] + nextOffset;
        nextOffset += 0xFC;
        blockOffset += 0xFC;
    } while (n >= 0);
    for (i = 0; i < 1; i++) {
        self[i] = 0;
    }
    cCommSystem_mNumNetConn = 0;
    self[7] = -1;
    self[9] = 0;
    self[10] = 0;
    self[11] = 0;
    self[12] = 1;
    self[8] = 1;
    ccs_3B18(str_www_ea_com);
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem__cCommSystem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E56E0_59B0(void *) __asm__("func_003E56E0");
extern "C" void cMemMan_free__FPv_59B0(void *) __asm__("cMemMan_free__FPv");
extern "C" void operator_delete__FPi_59B0(void *) __asm__("operator_delete__FPi");

extern "C" void cCommSystem__cCommSystem_59B0(void *self, int flags) __asm__("cCommSystem__cCommSystem");
/**
 * Destroys the comm system: deletes its pool mutex and frees the message-element pool.
 *
 * Clears mPool and mFree; it does not close the channel.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cCommSystem__cCommSystem_59B0(void *self, int flags) {
    func_003E56E0_59B0((char*)self + 4);
    void *blockPool = *(void **)((char*)self + 0x14);
    if (blockPool != 0) {
        cMemMan_free__FPv_59B0(blockPool);
    }
    *(int *)((char*)self + 0x14) = 0;
    *(int *)((char*)self + 0x18) = 0;
    if (flags & 1) {
        operator_delete__FPi_59B0(self);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_getSignature);
#ifdef SKIP_ASM
extern char str_NA_R_F004_b[];

/**
 * Returns the build signature that online peers compare.
 *
 * The lobby code rejects another player whose message carries a different signature.
 *
 * @return the string "NA_R_F004" (str_NA_R_F004_b)
 */
extern "C" char *cCommSystem_getSignature(void) {
    return str_NA_R_F004_b;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255A20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void NetConnIdle(void);
extern "C" int func_003F3CC0_5A20(int sel, int a, int b) __asm__("func_003F3CC0");
extern "C" int func_003F3C50(void* params, int a);
extern "C" void func_00255FE8(void* self, int reason);
extern "C" void func_00256038(void* self, int* out);
// cCommSystem_mNetConns: the 'netw'/'list' reply buffer (func_00255F20), 10 entries of 0x100 bytes; +0x80 is an entry's name field
extern char cCommSystem_mNetConns[];

struct sCommVE_5A20 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sCommObj_5A20 {
    char pad0[8];
    sCommVE_5A20* vt;   // 0x8
};

struct sComm_5A20 {
    sCommObj_5A20* objs[1];   // 0x0
    char pad4[0x1C - 4];
    int index;                // 0x1C
    int state;                // 0x20
    int timer;                // 0x24
    char pad28[4];
    int bband;                // 0x2C
    int type;                 // 0x30
};

extern "C" void func_00255A20(sComm_5A20* self)
{
    int i;
    NetConnIdle();
    switch (self->state) {
    case 0:
    case 1:
    case 6:
        break;
    case 2:
        if (func_003F3CC0_5A20(0x6F6E6C6E, 0, 0) == 0) {
            func_003F3C50(cCommSystem_mNetConns + 0x80 + (self->index << 8), 0);
            self->state = 3;
        }
        break;
    case 3: {
        if (func_003F3CC0_5A20(0x6F6E6C6E, 0, 0) != 0 && func_003F3CC0_5A20(0x686F7374, 0, 0) != 0) {
            self->state = 4;
            self->timer = 0x708;
            self->type = func_003F3CC0_5A20(0x74797065, 0, 0);
            int bb = func_003F3CC0_5A20(0x62626E64, 0, 0);
            if (bb == 1) {
                self->bband = bb;
            }
        }
        int conn = func_003F3CC0_5A20(0x636F6E6E, 0, 0);
        if ((conn >> 24) == '-') {
            func_00255FE8(self, conn);
            self->timer = 0;
        }
        break;
    }
    case 4: {
        int addr;
        func_00256038(self, &addr);
        if (addr != 0) {
            self->timer = 0;
            self->state = 5;
        }
        break;
    }
    case 5:
        for (i = 0; i < 1; i++) {
            sCommObj_5A20* channel = self->objs[i];
            if (channel != 0) {
                channel->vt[3].fn((char*)channel + channel->vt[3].delta);
            }
        }
        break;
    }
    if (self->timer > 0) {
        self->timer--;
        if (self->timer <= 0) {
            func_00255FE8(self, 0x74696D65);
            self->timer = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_openDSockChannel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_5BF8(int, char *, int, int) __asm__("cMemMan_alloc");
extern "C" void *func_00256698_5BF8(void *) __asm__("func_00256698");
extern "C" void cDSockChannel_listen_5BF8(void *, int, int) __asm__("cDSockChannel_listen");
extern "C" void cDSockChannel_connect_5BF8(void *, int, int) __asm__("cDSockChannel_connect");
extern char str_cDSockChannel[];

/**
 * Creates and opens a DirtySock channel in the comm system's single channel slot.
 *
 * If the slot is free, allocates a 0x20-byte "cDSockChannel", constructs it and opens it on the address
 * "<a1>:<port>:<port>" with type 0x102 (cDSockChannel_connect) when @p isListener is set, or 0x101 (cDSockChannel_listen) otherwise.
 *
 * @param slots  the comm system's channel slots (only the first is used)
 * @param addr     peer address string
 * @param port     port; only the low 16 bits are used
 * @param isListener     the lobby's host flag (nonzero when this machine hosts)
 * @return the slot index (0), or -1 when the slot is already taken
 */
extern "C" int cCommSystem_openDSockChannel(void **slots, int addr, int port, int isListener) {
    int i;
    int n = port & 0xFFFF;
    for (i = 0; i < 1; i++) {
        if (slots[i] == 0) {
            void *obj = func_00256698_5BF8(cMemMan_alloc_5BF8(0x20, str_cDSockChannel, 0x20000000, 0));
            if (isListener != 0) {
                cDSockChannel_connect_5BF8(obj, addr, n);
            } else {
                cDSockChannel_listen_5BF8(obj, addr, n);
            }
            slots[i] = obj;
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255CC8);
#ifdef SKIP_ASM
extern "C" void func_00255CC8(char *commSystem, int channelIndex) {
    void **slot = (void **)(commSystem + channelIndex * 4);
    char *channel = (char *)*slot;
    if (*(int *)channel != 0) {
        char *vt = *(char **)(channel + 8);
        void (*fn)(void *) = *(void (**)(void *))(vt + 0x14);
        fn(channel + *(short *)(vt + 0x10));
    }
    char *channelToFree = (char *)*slot;
    if (channelToFree != 0) {
        char *vt = *(char **)(channelToFree + 8);
        void (*fn)(void *, int) = *(void (**)(void *, int))(vt + 0xC);
        fn(channelToFree + *(short *)(vt + 8), 3);
    }
    *slot = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255D38);
#ifdef SKIP_ASM
extern "C" int func_00255D38(void *commSystem, int channelIndex) {
    return *(int *)((char*)commSystem + (channelIndex << 2)) != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255D50);
#ifdef SKIP_ASM
extern "C" int func_00255D50(void *commSystem, int channelIndex) {
    char *obj = *(char **)((char*)commSystem + (channelIndex << 2));
    char *vt = *(char **)(obj + 8);
    return (*(int (**)(void *))(vt + 0x24))(obj + *(short *)(vt + 0x20));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255D88);
#ifdef SKIP_ASM
extern "C" float func_00255D88(int commSystem, int channelIndex) {
    char *obj = *(char **)(commSystem + (channelIndex << 2));
    char *vt = *(char **)(obj + 8);
    return (*(float (**)(void *))(vt + 0x2C))(obj + *(short *)(vt + 0x28));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255DC0);
#ifdef SKIP_ASM
extern "C" int func_00255DC0(void *commSystem, int channelIndex, int data, int maxSize) {
    char *obj = *(char **)((char*)commSystem + (channelIndex << 2));
    char *vt = *(char **)(obj + 8);
    return (*(int (**)(void *, int, int))(vt + 0x3C))(obj + *(short *)(vt + 0x38), data, maxSize);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255E00);
#ifdef SKIP_ASM
extern "C" int func_00255E00(void *commSystem, int channelIndex, int data, int size) {
    char *obj = *(char **)((char*)commSystem + (channelIndex << 2));
    char *vt = *(char **)(obj + 8);
    return (*(int (**)(void *, int, int))(vt + 0x44))(obj + *(short *)(vt + 0x40), data, size);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255E40);
#ifdef SKIP_ASM
extern "C" int func_00255E40(void *commSystem, int channelIndex) {
    if ((*(int *)((char*)(commSystem) + (0x28))) != 0) {
        return 1;
    }
    return *(int *)((char*)(*(void **)((char*)commSystem + (channelIndex << 2))) + 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255E68);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void *);
extern "C" void MUTEX_unlock(void *);

extern "C" int *func_00255E68(void *pool) {
    void *mutex = (char*)pool + 4;
    MUTEX_lock(mutex);
    int *node = *(int **)((char*)pool + 0x18);
    *(int **)((char*)pool + 0x18) = (int *)*node;
    MUTEX_unlock(mutex);
    *node = 0;
    return node;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255EC0);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void *);
extern "C" void MUTEX_unlock(void *);

extern "C" void func_00255EC0(void *pool, void *node) {
    void *mutex = (char*)pool + 4;
    MUTEX_lock(mutex);
    void *head = *(void **)((char*)pool + 0x18);
    *(int *)((char*)node + 4) = 0;
    *(void **)((char*)node + 0) = head;
    *(int *)((char*)node + 8) = 0;
    *(void **)((char*)pool + 0x18) = node;
    MUTEX_unlock(mutex);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255F20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003F3BC8_5F20(int, void *, int) __asm__("func_003F3BC8");
extern char cCommSystem_mNetConns[];
extern int cCommSystem_mNumNetConn;

extern "C" int func_00255F20(void *arg0, int arg1) {
    cCommSystem_mNumNetConn = func_003F3BC8_5F20(arg1, cCommSystem_mNetConns, 0xA);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255F50);
#ifdef SKIP_ASM
extern "C" void func_00255FA0(void *);

extern "C" void func_00255F50(void *commSystem, int configIndex) {
    func_00255FA0(commSystem);
    (*(int *)((char*)(commSystem) + (0x1C))) = configIndex;
    (*(int *)((char*)(commSystem) + (0x30))) = 1;
    (*(int *)((char*)(commSystem) + (0x24))) = 0x2328;
    (*(int *)((char*)(commSystem) + (0x20))) = 2;
    (*(int *)((char*)(commSystem) + (0x2C))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255FA0);
#ifdef SKIP_ASM
extern "C" void func_003F40B8();

extern "C" void func_00255FA0(void *commSystem) {
    if ((*(int *)((char*)(commSystem) + (0x1C))) >= 0) {
        (*(int *)((char*)(commSystem) + (0x1C))) = -1;
        func_003F40B8();
    }
    (*(int *)((char*)(commSystem) + (0x24))) = 0;
    (*(int *)((char*)(commSystem) + (0x20))) = 1;
    (*(int *)((char*)(commSystem) + (0x28))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255FE8);
#ifdef SKIP_ASM
extern "C" void func_003F40B8();

extern "C" void func_00255FE8(void *commSystem, int errorCode) {
    if ((*(int *)((char*)(commSystem) + (0x1C))) >= 0) {
        (*(int *)((char*)(commSystem) + (0x1C))) = -1;
        func_003F40B8();
    }
    (*(int *)((char*)(commSystem) + (0x28))) = errorCode;
    (*(int *)((char*)(commSystem) + (0x20))) = 6;
    (*(int *)((char*)(commSystem) + (0x24))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256038);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003F3CC0_6038(int, int, int) __asm__("func_003F3CC0");

extern "C" void func_00256038(void *commSystem, int *localAddr) {
    int temp_2;

    *localAddr = 0;
    temp_2 = func_003F3CC0_6038(0x61646472, 0, 0);
    if (temp_2 & 0xFFFFFF00) {
        *localAddr = temp_2;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256088);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0031B008_6088(void *, int, void *) __asm__("func_0031B008");
extern char str_cdrom0_NETGUI_NTGUI_ELF_1[];
extern int D_004A2EC0;

extern "C" void func_00256088(void) {
    func_0031B008_6088(str_cdrom0_NETGUI_NTGUI_ELF_1, 1, &D_004A2EC0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002560B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_60B0(int, char *, int, int) __asm__("cMemMan_alloc");
extern "C" int *func_002561B8_60B0(void *) __asm__("func_002561B8");
extern "C" void func_002561E0_60B0(int *, int) __asm__("func_002561E0");
extern "C" void func_00256258_60B0(int *, int) __asm__("func_00256258");
extern "C" void func_002562D0_60B0(int *) __asm__("func_002562D0");
extern "C" void func_002563A8_60B0(int *) __asm__("func_002563A8");
extern "C" void func_002563F8_60B0(int *) __asm__("func_002563F8");
extern "C" void SYNCTASK_run_60B0(int) __asm__("SYNCTASK_run");
extern "C" void THREAD_yieldticks_60B0(int) __asm__("THREAD_yieldticks");
extern char str_DNASMAN[];

static inline int ge3_60B0(int *o) { return *o >= 3; }

extern "C" void func_002560B0(void) {
    int *dnasMan = func_002561B8_60B0(cMemMan_alloc_60B0(0xA50, str_DNASMAN, 0x20000000, 0));
    func_00256258_60B0(dnasMan, 0);
    while (!ge3_60B0(dnasMan)) {
        func_002563F8_60B0(dnasMan);
        SYNCTASK_run_60B0(0);
        THREAD_yieldticks_60B0(1);
    }
    func_002562D0_60B0(dnasMan);
    while ((*dnasMan ^ 5) != 0) {
        func_002563F8_60B0(dnasMan);
        SYNCTASK_run_60B0(0);
        THREAD_yieldticks_60B0(1);
    }
    func_002563A8_60B0(dnasMan);
    if (dnasMan != 0) {
        func_002561E0_60B0(dnasMan, 3);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256188);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003DED50_6188(void *, int, int, void *) __asm__("func_003DED50");
extern char str_data_config_slusovf_big[];
extern int D_004A2ED4;

extern "C" void func_00256188(void) {
    func_003DED50_6188(str_data_config_slusovf_big, 0, 0x64, &D_004A2ED4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002561B8);
#ifdef SKIP_ASM
extern "C" void func_00256228();

extern "C" int func_002561B8(int dnasMan) {
    func_00256228();
    return dnasMan;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002561E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00256250_61E0(int *) __asm__("func_00256250");
extern "C" void operator_delete__FPi(int *);

extern "C" void func_002561E0(int *dnasMan, int flags) {
    func_00256250_61E0(dnasMan);
    if (flags & 1) {
        operator_delete__FPi(dnasMan);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256228);
#ifdef SKIP_ASM
extern "C" void func_00256228(void *arg0) {
    (*(int *)((char*)(arg0) + (4))) = 0;
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x10))) = 0;
    (*(int *)((char*)(arg0) + (0x14))) = 0;
    (*(int *)((char*)(arg0) + (0x18))) = -1;
    (*(int *)((char*)(arg0) + (0))) = 0;
}
#endif

extern "C" void func_00256250(void) {
}

//100%
INCLUDE_ASM("seg/seg_156860", func_00256258);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003DF748(int *, int, int);
extern int D_004803E8[];
extern int D_004A2EE0[];
extern "C" void *operator_new__FUi_6258(unsigned int, int *, int, int) __asm__("operator_new__FUi");

extern "C" void func_00256258(char *self, int arg1) {
    *(int *)(self + 0x20) = arg1;
    *(int *)(self + 0x14) = 0;
    *(int *)(self + 0x1C) = 0;
    *(int *)(self + 0) = 1;
    void *mem = operator_new__FUi_6258(0x195000, D_004A2EE0, 0x04000100, 0);
    *(void **)(self + 4) = mem;
    *(int *)(self + 0x18) = func_003DF748(D_004803E8, (int)mem, 0x195000);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002562D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sKey_62D0 { int a; int b; };
extern sKey_62D0 D_004A2ED8;
extern "C" void MEM_fill_62D0(void *, int, int) __asm__("MEM_fill");
extern "C" char *func_003F3F78();
extern "C" void func_003FD0D8(void *);
extern "C" char *strcpy(char *, const char *);

extern "C" void func_002562D0(int *self)
{
    self[7] = 0;
    MEM_fill_62D0(self + 9, 0, 0xA0);
    self[0x1B] = self[3];
    self[0x1C] = self[4];
    self[0x24] = 0;
    self[0x23] = 0;
    *(sKey_62D0 *)(self + 0x1D) = D_004A2ED8;
    self[0x21] = 8;
    strcpy((char *)(self + 0x26), func_003F3F78());
    strcpy((char *)(self + 0x2A), "SLUS_20772");
    self[0x2E] = 0;
    self[0x2F] = self[8];
    self[0x30] = 1;
    self[0] = 4;
    func_003FD0D8(self + 9);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002563A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cMemMan_free__FPv_63A8(int) __asm__("cMemMan_free__FPv");

extern "C" void func_002563A8(void *dnasMan) {
    int temp_4;
    int temp_4_2;

    temp_4 = (*(int *)((char*)(dnasMan) + (0xC)));
    if (temp_4 != 0) {
        cMemMan_free__FPv_63A8(temp_4);
        (*(int *)((char*)(dnasMan) + (0xC))) = 0;
    }
    temp_4_2 = (*(int *)((char*)(dnasMan) + (4)));
    if (temp_4_2 != 0) {
        cMemMan_free__FPv_63A8(temp_4_2);
        (*(int *)((char*)(dnasMan) + (4))) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002563F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00256460(char* self);
extern "C" void func_00256510(char* self);
extern "C" void func_002565E0(char* self);

extern "C" void func_002563F8(char* self)
{
    switch (*(int*)self) {
    case 1:
        func_00256460(self);
        break;
    case 2:
        func_00256510(self);
        break;
    case 4:
        func_002565E0(self);
        break;
    case 0:
    case 3:
    case 5:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256460);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int ASYNCFILE_getstatus_6460(int) __asm__("ASYNCFILE_getstatus");
extern "C" void ASYNCFILE_release_6460(int, int, void *) __asm__("ASYNCFILE_release");
extern "C" int func_003FCFD8_6460(int, int) __asm__("func_003FCFD8");
extern "C" void *operator_new__FUi_6460(unsigned int, char *, int, int) __asm__("operator_new__FUi");
extern "C" int func_003DF748_6460(char *, void *, int) __asm__("func_003DF748");
extern char str_AuthData[];
extern char str_CONFIG_DAT[];

extern "C" void func_00256460(char *self) {
    if (ASYNCFILE_getstatus_6460(*(int *)(self + 0x18)) == 1) {
        *(int *)(self + 8) = 0;
        ASYNCFILE_release_6460(*(int *)(self + 0x18), 0, self + 8);
        int readBuffer = *(int *)(self + 4);
        if (readBuffer != 0) {
            int readSize = *(int *)(self + 8);
            if (readSize > 0 && func_003FCFD8_6460(readBuffer, readSize) >= 0) {
                void *authData = operator_new__FUi_6460(0x10000, str_AuthData, 0x04000100, 0);
                *(void **)(self + 0xC) = authData;
                *(int *)(self + 0x18) = func_003DF748_6460(str_CONFIG_DAT, authData, 0x10000);
                *(int *)(self + 0) = 2;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256510);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int ASYNCFILE_getstatus_6510(int) __asm__("ASYNCFILE_getstatus");
extern "C" void ASYNCFILE_release_6510(int, int, void *) __asm__("ASYNCFILE_release");
struct K6510 { unsigned char b[4]; };
extern K6510 D_004A2EE8[];

extern "C" void func_00256510(char *self) {
    if (ASYNCFILE_getstatus_6510(*(int *)(self + 0x18)) == 1) {
        *(int *)(self + 0x10) = 0;
        ASYNCFILE_release_6510(*(int *)(self + 0x18), 0, self + 0x10);
        unsigned char *d = *(unsigned char **)(self + 0xC);
        if (d != 0) {
            int n = *(int *)(self + 0x10);
            if (n > 0) {
                *(int *)(self + 0) = 3;
                K6510 key = D_004A2EE8[0];
                int i;
                unsigned char *p;
                for (i = 0, p = d; i < *(int *)(self + 0x10); i++, p++) {
                    *p ^= key.b[i % 4];
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002565E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003FD100_65E0(int) __asm__("func_003FD100");
extern "C" void MEM_fill_65E0(char *, int, int) __asm__("MEM_fill");
extern "C" int func_003FD128_65E0(char *) __asm__("func_003FD128");
extern "C" void USTR_copy_65E0(char *, char *) __asm__("USTR_copy");

extern "C" void func_002565E0(char *self) {
    if (func_003FD100_65E0(*(int *)(self + 0x14)) == 0) {
        char *infoBuf = self + 0xC4;
        MEM_fill_65E0(infoBuf, 0, 0x38C);
        char *b = self + 0x650;
        char *c = self + 0x850;
        int result = func_003FD128_65E0(infoBuf);
        *(short *)(self + 0x450) = 0;
        *(short *)b = 0;
        *(short *)c = 0;
        USTR_copy_65E0(self + 0x450, self + 0xCC);
        USTR_copy_65E0(b, self + 0x10C);
        USTR_copy_65E0(c, self + 0x20C);
        if (result == 0) {
            *(int *)(self + 0x1C) = 1;
        } else {
            *(int *)(self + 0x1C) = 0;
        }
        *(int *)(self + 0) = 5;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256698);
#ifdef SKIP_ASM
void func_002557E0(void *);
extern char cDSockChannel_vtbl[];

extern "C" void *func_00256698(void *channel) {
    func_002557E0(channel);
    (*(char **)((char*)(channel) + (8))) = cDSockChannel_vtbl;
    (*(float *)((char*)(channel) + (0x10))) = 0.10000000149011612f;
    (*(int *)((char*)(channel) + (0xC))) = 0;
    (*(int *)((char*)(channel) + (0x18))) = 0;
    (*(int *)((char*)(channel) + (0x1C))) = 0;
    (*(int *)((char*)(channel) + (0x14))) = 0;
    return channel;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel__cDSockChannel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cCommChannel__cCommChannel_66E8(void *, int) __asm__("cCommChannel__cCommChannel");
extern "C" void cDSockChannel_disconnect(void *);
extern char cDSockChannel_vtbl[];

/**
 * Destroys a DirtySock comm channel: closes its link, then destroys the cCommChannel base.
 *
 * Stores cDSockChannel_vtbl in mVtbl (+0x08), calls cDSockChannel_disconnect (disconnect: closes the channel and destroys its
 * NetGameLink and NetGameUtil), then cCommChannel__cCommChannel, which frees the object when bit 0 of @p flags is set.
 * It is the destructor slot (kCommChannelVt_dtor) of the channel's vtable.
 *
 * @param dSockChannel  the channel (this)
 * @param flags          g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cDSockChannel__cDSockChannel(cDSockChannel* dSockChannel, int flags) {
    ((char*)dSockChannel->mVtbl) = cDSockChannel_vtbl;
    cDSockChannel_disconnect(dSockChannel);
    cCommChannel__cCommChannel_66E8(dSockChannel, flags);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_readPacket);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy_6730(char *, char *, int) __asm__("MEM_copy");
extern "C" int func_003E6B10_6730(int, char *, int) __asm__("func_003E6B10");

/**
 * Receives the next game packet from the channel's NetGameLink and copies its payload out.
 *
 * Only while connected (mState 2). It pulls packets one at a time (func_003E6B10, probably NetGameLinkRecv):
 * a kind-5 packet (byte 2) whose length (byte 3) is nonzero and fits is copied, other kind-5 packets are
 * dropped, and any other kind sets mPacketError (state 4 on the next updateConnected).
 *
 * @param dst     buffer for the payload
 * @param maxlen  size of @p dst in bytes
 * @return the payload length (1-255), or 0 when not connected, when nothing usable arrived or on a bad packet
 */
extern "C" unsigned char cDSockChannel_readPacket(cDSockChannel* self, char *dst, int maxlen) {
    unsigned char buf[0x108];
    if (self->mState == 2) {
        while (func_003E6B10_6730((int)self->mGameLink, (char *)buf, 1) > 0) {
            if (buf[2] == 5) {
                if (buf[3] != 0 && maxlen >= (int)buf[3]) {
                    unsigned char n = buf[3];
                    MEM_copy_6730(dst, (char *)buf + 4, n);
                    return buf[3];
                }
            } else {
                self->mPacketError = 1;
                return 0;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_writePacket);
#ifdef SKIP_ASM
extern "C" void MEM_copy(char *, int, int);
extern "C" int NetGameLinkSend(int, void *, int);

/**
 * Sends one game packet over the channel's NetGameLink.
 *
 * Only while connected (mState 2) with no channel error: wraps the payload in a kind-5 packet whose length
 * byte (byte 3) holds the low 8 bits of @p len. Nothing checks that @p len fits the 0x104 bytes after the
 * header of the stack packet.
 *
 * @param data  payload to send (a pointer passed as an int)
 * @param len   payload size in bytes; 0 or less sends nothing
 * @return 1 when NetGameLinkSend reports a positive result, otherwise 0
 */
extern "C" int cDSockChannel_writePacket(cDSockChannel* self, int data, int len) {
    char buf[0x108];
    if (self->mState != 2) return 0;
    int error = self->mError;
    if (error != 0) return 0;
    if (len > 0) {
        buf[2] = 5;
        buf[3] = len;
        MEM_copy(buf + 4, data, len);
        return error < NetGameLinkSend((int)self->mGameLink, buf, 1);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_listen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00255830__FPv_6860(void *) __asm__("func_00255830__FPv");
extern "C" int func_003E6E08_6860() __asm__("func_003E6E08");
extern "C" int func_003E6F40_6860(int, int, void *) __asm__("func_003E6F40");
extern "C" int sprintf_6860(char *, const char *, ...) __asm__("sprintf");
extern char str_pct_s_pct_d_pct_d[];

/**
 * Starts listening for the peer's game connection on "addr:port:port".
 *
 * Disconnects first (virtual slot 2), opens the channel (func_00255830, channelOpen), creates a NetGameUtil
 * (func_003E6E08) into mGameUtil and, if that worked, starts it in mode 0x101 (On Tour's listen) on the
 * formatted address and sets mState to 1 (connecting). Unlike connect, it leaves mError as it was.
 *
 * @param addr  peer address (dotted-decimal string)
 * @param port  port; only the low 16 bits are used, written twice into the address
 */
extern "C" void cDSockChannel_listen(cDSockChannel* self, int addr, int port) {
    int n = port & 0xFFFF;
    char buf[0x100];
    char *vt = (char*)self->mVtbl;
    (*(void (**)(void *))(vt + 0x14))(((char*)self) + *(short *)(vt + 0x10));
    func_00255830__FPv_6860(((char*)self));
    int gameUtil = func_003E6E08_6860();
    self->mGameUtil = (NetGameUtilRefT*)gameUtil;
    if (gameUtil != 0) {
        sprintf_6860(buf, str_pct_s_pct_d_pct_d, addr, n, n);
        func_003E6F40_6860((int)self->mGameUtil, 0x101, buf);
        self->mState = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_connect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00255830__FPv_68F8(void *) __asm__("func_00255830__FPv");
extern "C" int func_003E6E08_68F8() __asm__("func_003E6E08");
extern "C" int func_003E6F40_68F8(int, int, void *) __asm__("func_003E6F40");
extern "C" int sprintf_68F8(char *, const char *, ...) __asm__("sprintf");
extern char str_pct_s_pct_d_pct_d[];

/**
 * Starts connecting to the peer's game link at "addr:port:port".
 *
 * Clears mError, disconnects (virtual slot 2), opens the channel (func_00255830, channelOpen), creates a
 * NetGameUtil (func_003E6E08) into mGameUtil and, if that worked, starts it in mode 0x102 (On Tour's connect)
 * on the formatted address and sets mState to 1 (connecting).
 *
 * @param addr  peer address (dotted-decimal string)
 * @param port  port; only the low 16 bits are used, written twice into the address
 */
extern "C" void cDSockChannel_connect(cDSockChannel* self, int addr, int port) {
    int n = port & 0xFFFF;
    char buf[0x100];
    self->mError = 0;
    char *vt = (char*)self->mVtbl;
    (*(void (**)(void *))(vt + 0x14))(((char*)self) + *(short *)(vt + 0x10));
    func_00255830__FPv_68F8(((char*)self));
    int gameUtil = func_003E6E08_68F8();
    self->mGameUtil = (NetGameUtilRefT*)gameUtil;
    if (gameUtil != 0) {
        sprintf_68F8(buf, str_pct_s_pct_d_pct_d, addr, n, n);
        func_003E6F40_68F8((int)self->mGameUtil, 0x102, buf);
        self->mState = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_disconnect);
#ifdef SKIP_ASM
extern "C" void cCommChannel_channelClose();
extern "C" void NetGameLinkDestroy(int);
extern "C" void NetGameUtilDestroy(int);

/**
 * Disconnects a DirtySock comm channel: closes it and destroys its game link and game util.
 *
 * Does nothing unless mState (+0x0C) is non-zero (connecting, connected or failed). Then cCommChannel_channelClose
 * clears mChannelOpen, mGameLink (+0x1C) is destroyed with NetGameLinkDestroy (NetGameLinkDestroy) and mGameUtil (+0x18)
 * with NetGameUtilDestroy (NetGameUtilDestroy), each set to 0 if present, and mState returns to 0 (closed). The disconnect
 * slot (kCommChannelVt_disconnect) of the channel's vtable.
 *
 * @param channel  the channel (this, a cDSockChannel)
 */
extern "C" void cDSockChannel_disconnect(void *channel) {
    int temp_4;
    int temp_4_2;

    if ((*(int *)((char*)(channel) + (0xC))) != 0) {
        cCommChannel_channelClose();
        temp_4 = (*(int *)((char*)(channel) + (0x1C)));
        if (temp_4 != 0) {
            NetGameLinkDestroy(temp_4);
            (*(int *)((char*)(channel) + (0x1C))) = 0;
        }
        temp_4_2 = (*(int *)((char*)(channel) + (0x18)));
        if (temp_4_2 != 0) {
            NetGameUtilDestroy(temp_4_2);
            (*(int *)((char*)(channel) + (0x18))) = 0;
        }
        (*(int *)((char*)(channel) + (0xC))) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00256A58(void* self);
extern "C" void cDSockChannel_updateConnected(cDSockChannel* self);

/**
 * Advances the channel's connection state machine.
 *
 * State 1 (connecting) runs func_00256A58 (updateConnecting) and state 2 (connected) runs
 * cDSockChannel_updateConnected(); the closed, timed-out and packet-error states (0, 3, 4) do nothing.
 */
extern "C" void cDSockChannel_update(cDSockChannel* self)
{
    switch (self->mState) {
    case 1:
        func_00256A58(((char*)self));
        break;
    case 2:
        cDSockChannel_updateConnected(self);
        break;
    case 0:
    case 3:
    case 4:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256A58);
#ifdef SKIP_ASM
extern "C" int func_003E66F0(int, int, int);
extern "C" int func_003E6FC8(int);

extern "C" void func_00256A58(void *channel) {
    int temp_2;
    int temp_2_2;

    temp_2 = func_003E6FC8((*(int *)((char*)(channel) + (0x18))));
    if (temp_2 != 0) {
        temp_2_2 = func_003E66F0(temp_2, 0, 0x237C);
        (*(int *)((char*)(channel) + (0x1C))) = temp_2_2;
        if (temp_2_2 != 0) {
            (*(int *)((char*)(channel) + (0xC))) = 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_updateConnected);
#ifdef SKIP_ASM
struct VEnt_6AA8 {
    short delta;
    short index;
    void (*fn)(char*);
};

extern "C" char* func_003E6958(int id);

/**
 * Checks a connected channel's link health and smooths its latency.
 *
 * Reads the NetGameLink status (func_003E6958): a gap over 30000 between its first word and +0x20 (probably
 * ms since the last receive) sets state 3, a zero byte at +0x45 (link lost) disconnects; both set mError.
 * Otherwise mLatency = (4 * mLatency + ping) / 5, the ping (+0xC, ms) in seconds capped at 1, and a pending
 * mPacketError then sets state 4 and mError.
 *
 * @param dSockChannel  the channel (this)
 */
extern "C" void cDSockChannel_updateConnected(cDSockChannel* dSockChannel)
{
    char* st = func_003E6958((int)dSockChannel->mGameLink);
    if (st) {
        if (*(int*)(st + 0x0) - *(int*)(st + 0x20) > 30000) {
            dSockChannel->mState = 3;
            dSockChannel->mError = 1;
            return;
        }
        if (*(unsigned char*)(st + 0x45) == 0) {
            VEnt_6AA8* vt = (VEnt_6AA8*)dSockChannel->mVtbl;
            vt[2].fn(((char*)dSockChannel) + vt[2].delta);
            dSockChannel->mError = 1;
            return;
        }
        float latency = (float)*(unsigned int*)(st + 0xC) * 0.0010000000474974513f;
        if (latency > 1.0f) latency = 1.0f;
        dSockChannel->mLatency = (dSockChannel->mLatency * 4.0f + latency) * 0.20000000298023224f;
    }
    if (dSockChannel->mPacketError) {
        dSockChannel->mState = 4;
        dSockChannel->mError = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_getLatency);
#ifdef SKIP_ASM
/**
 * Returns the channel's smoothed link latency.
 *
 * Reads mLatency (+0x10), which cDSockChannel_updateConnected averages from the link's ping ((4 * old + new) / 5,
 * capped at 1 s; 0.1 after construction). The getLatency slot (kCommChannelVt_getLatency) of the channel's vtable.
 *
 * @param dSockChannel  the channel (this)
 * @return the latency in seconds
 */
extern "C" float cDSockChannel_getLatency(cDSockChannel* dSockChannel) {
    return (dSockChannel->mLatency);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", RpcAlloc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *operator_new_6BB0(unsigned int, void *, unsigned int, int) __asm__("operator_new__FUi");
extern char str_RpcAlloc[];

/**
 * Allocates a buffer for the SIF RPC layer.
 *
 * Calls operator new with the name "RpcAlloc" and type 0x20000000.
 *
 * @param size  bytes to allocate
 * @return the new buffer
 */
extern "C" void *RpcAlloc(unsigned int size) {
    return operator_new_6BB0(size, str_RpcAlloc, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256BD8);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv();

extern "C" void func_00256BD8(int buffer) {
    if (buffer != 0) {
        cMemMan_free__FPv();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_construct);
#ifdef SKIP_ASM
extern "C" int cMemMan_alloc(int, int *, int, int);
extern "C" int cGameComm_cGameComm(int, int, int);
extern int D_004804B8[];

/**
 * Creates the game comm singleton: 0x5F0 bytes tagged "cGameComm", constructed by cGameComm_cGameComm.
 *
 * Stores the result in cGameComm_mGameComm; func_00256C50 destroys it.
 *
 * @param channel  the cCommSystem channel index the game link uses (stored in unk_06C)
 * @param isServer  nonzero to run the server side of the handshake (mServer)
 */
extern "C" void cGameComm_construct(int channel, int isServer) {
    cGameComm_mGameComm = (cGameComm*)(cGameComm_cGameComm(cMemMan_alloc(0x5F0, D_004804B8, 0x20000000, 0), channel, isServer));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256C50);
#ifdef SKIP_ASM
extern "C" void func_00256EE0(int, int);

extern "C" void func_00256C50(void) {
    if (cGameComm_mGameComm != 0) {
        func_00256EE0((int)cGameComm_mGameComm, 3);
        cGameComm_mGameComm = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_cGameComm);
#ifdef SKIP_ASM
void* cBE_getInterface_6C78(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENetworkInterface_getSyncDataSize(void* iface);
extern "C" void cGameComm_setState_6C78(void* self, int mode) __asm__("cGameComm_setState");
extern "C" void MEM_fill_6C78(void* p, int c, int n) __asm__("MEM_fill");
extern "C" int func_003E7340();

struct sGcQueue_6C78 {
    int wr;                 // 0x0
    int rd;                 // 0x4
    int* slots[32];         // 0x8
};

// cGameComm (0x5F0 bytes, allocated by cGameComm_construct).
struct sGameComm_6C78 {
    int unk_0;          // 0x0
    int unk_4;          // 0x4
    int unk_8;          // 0x8
    int unk_C;          // 0xC
    int unk_10;         // 0x10
    int unk_14;         // 0x14
    int unk_18;         // 0x18
    int unk_1C;         // 0x1C
    int unk_20;         // 0x20
    int unk_24;         // 0x24
    int unk_28;         // 0x28
    int unk_2C;         // 0x2C
    int unk_30;         // 0x30
    int unk_34;         // 0x34
    int unk_38;         // 0x38
    int unk_3C;         // 0x3C
    int unk_40;         // 0x40
    int unk_44;         // 0x44
    int unk_48;         // 0x48
    int unk_4C;         // 0x4C
    int unk_50;         // 0x50
    int unk_54;         // 0x54
    int unk_58;         // 0x58
    int unk_5C;         // 0x5C
    int unk_60;         // 0x60
    int unk_64;         // 0x64
    int unk_68;         // 0x68
    int unk_6C;         // 0x6C
    int unk_70;         // 0x70
    int unk_74;         // 0x74
    int unk_78;         // 0x78
    int unk_7C;         // 0x7C
    int unk_80;         // 0x80
    int unk_84;         // 0x84
    int unk_88;         // 0x88
    int unk_8C;         // 0x8C
    int unk_90;         // 0x90
    int unk_94;         // 0x94
    int unk_98;         // 0x98
    int unk_9C;         // 0x9C
    int unk_A0;         // 0xA0
    int unk_A4;         // 0xA4
    int unk_A8;         // 0xA8
    int unk_AC;         // 0xAC
    int unk_B0;         // 0xB0
    int unk_B4;         // 0xB4
    int unk_B8;         // 0xB8
    int unk_BC;         // 0xBC
    int unk_C0;         // 0xC0
    int unk_C4[4];          // 0xC4 (cleared high to low)
    char unk_D4[0x18];      // 0xD4 (random state; AIgetseed / AIsetseed)
    int unk_EC;         // 0xEC
    char unk_F0[0x10];      // 0xF0 (name, host side)
    int unk_100;        // 0x100
    char unk_104[0x10];     // 0x104 (name, client side)
    float unk_114;      // 0x114
    float unk_118;      // 0x118
    float unk_11C;      // 0x11C
    int unk_120;        // 0x120
    int unk_124;        // 0x124
    int unk_128;        // 0x128
    int unk_12C;        // 0x12C
    int unk_130;        // 0x130
    int unk_134;        // 0x134
    int unk_138;        // 0x138
    int unk_13C;        // 0x13C
    int unk_140;        // 0x140
    sGcQueue_6C78 q[5];     // 0x144
    void* buf;              // 0x3EC
    int bufSize;            // 0x3F0
    int unk_3F4;            // 0x3F4
    int unk_3F8;            // 0x3F8
    int unk_3FC;            // 0x3FC
    int unk_400;            // 0x400
    int unk_404;            // 0x404
    int unk_408;            // 0x408
    int unk_40C;            // 0x40C
    char unk_410[0xF0];     // 0x410
    char unk_500[0xF0];     // 0x500
};

struct sGcGlobals_6C78 {
    char pad0[0x10];
    int unk_10;             // 0x10
    float unk_14;           // 0x14
    int unk_18;             // 0x18
    char pad1C[0x5C];
    void* be;               // 0x78
};
extern sGcGlobals_6C78* D_004A28A8_6C78 __asm__("SSXApp");

/**
 * Constructs the online game link: the handshake state, timers, counters and empty packet queues.
 *
 * Stores the cCommSystem channel index in unk_06C and @p isServer in mServer, sets mState to kGameComm_Init through
 * cGameComm_setState, and sizes mBEBufferSize with cBENetworkInterface_getSyncDataSize__FPv (cBENetworkInterface::getSyncDataSize) on BE interface
 * 7. Then: mConnected 1, mSyncInput 1, unk_060 30, unk_09C 1, mLoading 1, mOverlayTimeout/mPauseTimeout -1, mNumSamples
 * 3, both mNetLatency entries and mLatency to the default frame time (SSXApp +0x14), unk_034 to the current time
 * (SSXApp +0x18 / +0x10), and the other flags, counters and ping state to 0. The five 32-slot packet queues (+0x144)
 * and the two 0xF0-byte message buffers (+0x410, +0x500) are cleared with MEM_fill, and unk_140 gets a DirtySock handle
 * from func_003E7340 (the ping server's socket).
 *
 * Retail's arguments are (channel, server), not On Tour's (playerID, numPlayers): this build links two machines.
 *
 * @param vself  the object to construct (this)
 * @param port   the cCommSystem channel index the link uses
 * @param isServer      non-zero on the machine that hosts the race (stored in mServer)
 * @return @p vself
 */
// cGameComm setup (called by cGameComm_construct on a fresh 0x5F0-byte block): clears the session state,
// the five packet queues and the two message buffers, and returns self.
// PORT: the unit declares this as (int, int, int) returning int.
extern "C" int cGameComm_cGameComm(int vself, int port, int isServer)
{
    sGameComm_6C78* self = (sGameComm_6C78*)vself;
    self->unk_6C = port;
    self->unk_44 = isServer;
    cGameComm_setState_6C78(self, 0);
    self->bufSize = cBENetworkInterface_getSyncDataSize(cBE_getInterface_6C78(D_004A28A8_6C78->be, 7));
    self->unk_60 = 30;
    self->unk_0 = 1;
    self->buf = 0;
    self->unk_40 = 1;
    self->unk_24 = 0;
    self->unk_28 = 0;
    self->unk_5C = 0;
    self->unk_2C = 0;
    self->unk_64 = -1;
    self->unk_48 = 0;
    self->unk_7C = 0;
    self->unk_80 = 0;
    self->unk_84 = 0;
    self->unk_30 = 0;
    self->unk_18 = 0;
    self->unk_94 = 0;
    self->unk_88 = 0;
    self->unk_8C = 0;
    self->unk_90 = 0;
    self->unk_9C = 1;
    self->unk_68 = -1;
    self->unk_AC = 1;
    self->unk_A0 = 0;
    self->unk_A4 = 0;
    self->unk_B0 = 0;
    self->unk_A8 = 0;
    self->unk_98 = 0;
    self->unk_4C = 0;
    self->unk_50 = 0;
    self->unk_54 = 0;
    self->unk_58 = 0;
    self->unk_124 = 0;
    self->unk_12C = 0;
    self->unk_128 = 0;
    self->unk_114 = D_004A28A8_6C78->unk_14;
    self->unk_118 = D_004A28A8_6C78->unk_14;
    self->unk_11C = D_004A28A8_6C78->unk_14;
    self->unk_120 = 3;
    self->unk_4 = 0;
    self->unk_8 = 0;
    self->unk_10 = 0;
    self->unk_14 = 0;
    self->unk_134 = 0;
    self->unk_74 = 0;
    self->unk_78 = 0;
    self->unk_3F4 = 0;
    self->unk_C = 0;
    self->unk_3F8 = 0;
    self->unk_3FC = 0;
    self->unk_B4 = 0;
    self->unk_B8 = 0;
    self->unk_BC = 0;
    self->unk_C0 = 0;
    for (int j = 3; j >= 0; j--) {
        self->unk_C4[j] = 0;
    }
    self->unk_34 = D_004A28A8_6C78->unk_18 / D_004A28A8_6C78->unk_10;
    self->unk_38 = 0;
    self->unk_3C = 0;
    self->unk_138 = 0;
    self->unk_140 = 0;
    self->unk_13C = 0;
    for (int i = 0; i < 4; i++) {
        sGcQueue_6C78* q = &self->q[i];
        q->wr = 0;
        q->rd = 0;
        MEM_fill_6C78(q->slots, 0, sizeof(q->slots));
    }
    self->q[4].wr = 0;
    self->q[4].rd = 0;
    MEM_fill_6C78(self->q[4].slots, 0, sizeof(self->q[4].slots));
    self->unk_400 = 0;
    self->unk_404 = 0;
    self->unk_408 = -1;
    self->unk_40C = -1;
    MEM_fill_6C78(self->unk_410, 0, sizeof(self->unk_410));
    MEM_fill_6C78(self->unk_500, 0, sizeof(self->unk_500));
    self->unk_140 = func_003E7340();
    return (int)self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256EE0);
#ifdef SKIP_ASM
extern "C" void cMemMan_free_6EE0(void *) __asm__("cMemMan_free__FPv");
extern "C" void operator_delete__FPi(int *);
extern "C" void func_002581B8(cGameComm* arg0, int *arg1);
extern "C" void func_003E73E0(void* p);

struct PacketQueue_6EE0 {
    int wr;                 // 0x0
    int rd;                 // 0x4
    int* slots[32];         // 0x8
};

static inline int* Peek_6EE0(PacketQueue_6EE0* q)
{
    return q->rd < q->wr ? q->slots[q->rd % 32] : 0;
}

static inline int* Pop_6EE0(PacketQueue_6EE0* q)
{
    return q->slots[q->rd++ % 32];
}

// PORT: the unit declares this as (int, int)
extern "C" void func_00256EE0(int self, int flags)
{
    char* gameComm = (char*)self;
    int i;
    if (*(void**)(gameComm + 0x3EC)) cMemMan_free_6EE0(*(void**)(gameComm + 0x3EC));
    if (*(int**)(gameComm + 0x134)) func_002581B8((cGameComm*)gameComm, *(int**)(gameComm + 0x134));
    for (i = 0; i < 4; i++) {
        while (Peek_6EE0((PacketQueue_6EE0*)(gameComm + 0x144) + i)) {
            func_002581B8((cGameComm*)gameComm, Pop_6EE0((PacketQueue_6EE0*)(gameComm + 0x144) + i));
        }
    }
    if (*(void**)(gameComm + 0x140)) {
        func_003E73E0(*(void**)(gameComm + 0x140));
        *(void**)(gameComm + 0x140) = 0;
    }
    if (flags & 1) operator_delete__FPi((int*)gameComm);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_setState);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00258330(cGameComm* arg0);
extern "C" void func_00258870(cGameComm*, int, int);
extern "C" void func_0031AD18(void);
/**
 * Switches the handshake and race state machine (mState) to a new tGameComm state.
 *
 * Entering GameActive (14) computes the latency (func_00258330), arms a 3600-frame link timeout and clears
 * the flag D_004A3ED0 (func_0031AD18); GameOver (15) clears the waiting message and the peer's pause; the
 * states 1, 3, ..., 13 arm an 1800-frame timeout. func_00258870 skips arming while either side is paused
 * before the start gate.
 *
 * @param mode  the new tGameComm state
 */
// Switches the controller to a new mode, running per-mode setup first.
extern "C" void cGameComm_setState(cGameComm* self, int mode)
{
    switch (mode) {
    case 14:
        func_00258330(self);
        func_00258870(self, 0xE10, 0);
        func_0031AD18();
        self->mState = mode;
        return;
    case 15:
        self->mWaitingMessage = 0;
        self->mPauseMenu[1] = 0;
        break;
    case 1: case 3: case 5: case 7: case 9: case 11: case 13:
        func_00258870(self, 0x708, 0);
        break;
    case 2: case 4: case 6: case 8: case 10: case 12:
        break;
    }
    self->mState = mode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_update);
#ifdef SKIP_ASM
// PORT: several callees are defined later in the unit with their own parameter types; bind local views by asm label
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENetworkInterface_getSyncData_70D8(void* iface, int a1, int a2) __asm__("cBENetworkInterface_getSyncData");
extern "C" void cBENetworkInterface_setSyncData_70D8(void* iface, int a1, int a2) __asm__("cBENetworkInterface_setSyncData");
extern "C" float func_00255D88_70D8(int arg0, int arg1) __asm__("func_00255D88");
extern "C" void cGameComm_setState_70D8(void* self, int mode) __asm__("cGameComm_setState");
extern "C" void func_00257750_70D8(char* p) __asm__("func_00257750");
extern "C" int func_002577C8_70D8(char* s) __asm__("func_002577C8");
extern "C" void func_00257A80_70D8(char* s) __asm__("func_00257A80");
extern "C" void func_00257CD8_70D8(char* s) __asm__("func_00257CD8");
extern "C" void cGameComm_updateWriteQueue_70D8(char* s) __asm__("cGameComm_updateWriteQueue");
extern "C" void func_00257F98_70D8(char* s) __asm__("func_00257F98");
extern "C" int func_002581D8_70D8(char* s) __asm__("func_002581D8");
extern "C" void func_002581E8_70D8(char* s, int a1, int a2) __asm__("func_002581E8");
extern "C" void func_002581F8_70D8(char* s) __asm__("func_002581F8");
extern "C" int func_00258280_70D8(char* s) __asm__("func_00258280");
extern "C" void func_00258290_70D8(char* s, int a1, int a2) __asm__("func_00258290");
extern "C" void func_002582A0_70D8(char* s) __asm__("func_002582A0");
extern "C" void cGameComm_updateMessageDisplay_70D8(void* s) __asm__("cGameComm_updateMessageDisplay");
extern "C" int func_003E7450_70D8(int sock, int a1, char* tag, int n) __asm__("func_003E7450");
extern "C" int func_003E7500_70D8(int sock, char* buf, int* len, int* info) __asm__("func_003E7500");
extern "C" int func_00416B18_70D8(const char* a, const char* b, int n) __asm__("func_00416B18");
extern char str_SSXPingServer[];

/**
 * Runs one frame of the online game link: ping replies, timers, the packet queues and the pre-race handshake.
 *
 * While the ping server is active (unk_138 > 0) it counts it down, reads replies from unk_140 (func_003E7500) and stops
 * when one matches a sent ping id (unk_0C4), else sends up to 4 "SSXPingServer" pings (func_003E7450), one every 5
 * frames. Then mStallTimer drops by 0.15 (not below 0), mUnPauseTimer counts down, and it runs
 * cGameComm_updateMessageDisplay, func_00257750 (timeouts) and func_002577C8 (checkErrors; an error returns),
 * func_00257CD8 (updateReadQueue), cGameComm_updateWriteQueue, func_002581F8 / func_002582A0 (async read / write
 * updates) and func_00257F98 (updateCmdQueue), and checks errors again.
 *
 * Last comes the handshake on mState: the server sends its settings (mServerSettings, 0x2C bytes), receives the
 * client's (+0x100, 0x14 bytes), sends then receives the BE sync data (mBEBuffer, getSyncData/setSyncData on BE
 * interface 7) and exchanges latencies (mNetLatency, measured with func_00255D88), moving through states 1..13 to
 * kGameComm_GameActive (14), where func_00257A80 runs each frame. The client walks the same states with every send and
 * receive swapped. Sends use func_00258290 / func_00258280 (asyncWrite / complete), receives func_002581E8 /
 * func_002581D8.
 *
 * @param vs  the game link (this)
 */
// PORT: the unit declares cGameComm_update as (void*)
extern "C" void cGameComm_update(cGameComm* vs)
{
    if (vs->unk_138 > 0) {
        char msg[0x100];
        int info[4];
        int len;
        vs->unk_138 = vs->unk_138 - 1;
        for (;;) {
            len = 0x100;
            if (func_003E7500_70D8((int)vs->unk_140, msg, &len, info) == 0) {
                break;
            }
            if (func_00416B18_70D8(msg, str_SSXPingServer, 0xD) != 0) {
                continue;
            }
            for (int i = 0; i < vs->unk_0B8; i++) {
                if (*(int*)(((char*)vs) + (i << 2) + 0xC4) == info[2]) {
                    vs->unk_138 = 0;
                    vs->unk_13C = 1;
                    break;
                }
            }
            if (vs->unk_138 <= 0) {
                break;
            }
        }
        if (vs->unk_138 > 0 && vs->unk_0B8 < 4) {
            if (vs->unk_0BC > 0) {
                vs->unk_0BC = vs->unk_0BC - 1;
            } else {
                int sock = func_003E7450_70D8((int)vs->unk_140, vs->unk_0C0, str_SSXPingServer, 0xE);
                int k = vs->unk_0B8;
                *(int*)(((char*)vs) + (k << 2) + 0xC4) = sock;
                vs->unk_0B8 = k + 1;
                vs->unk_0BC = 4;
            }
        }
    }
    vs->mStallTimer = vs->mStallTimer - 0.15000000596046448f;
    if (vs->mStallTimer < 0.0f) {
        vs->mStallTimer = 0.0f;
    }
    if (vs->mUnPauseTimer > 0) {
        vs->mUnPauseTimer = vs->mUnPauseTimer - 1;
    }
    cGameComm_updateMessageDisplay_70D8(((char*)vs));
    func_00257750_70D8(((char*)vs));
    if (func_002577C8_70D8(((char*)vs)) != 0) {
        return;
    }
    func_00257CD8_70D8(((char*)vs));
    cGameComm_updateWriteQueue_70D8(((char*)vs));
    func_002581F8_70D8(((char*)vs));
    func_002582A0_70D8(((char*)vs));
    func_00257F98_70D8(((char*)vs));
    if (func_002577C8_70D8(((char*)vs)) != 0) {
        return;
    }
    void* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    if (vs->mServer != 0) {
        switch ((unsigned int)vs->mState) {
        case 0:
            cGameComm_setState_70D8(((char*)vs), 1);
            break;
        case 1:
            func_00258290_70D8(((char*)vs), (int)(((char*)vs) + 0xD4), 0x2C);
            cGameComm_setState_70D8(((char*)vs), 2);
            break;
        case 2:
            if (func_00258280_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 3);
            }
            break;
        case 3:
            func_002581E8_70D8(((char*)vs), (int)(((char*)vs) + 0x100), 0x14);
            cGameComm_setState_70D8(((char*)vs), 4);
            break;
        case 4:
            if (func_002581D8_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 5);
            }
            break;
        case 5:
            cBENetworkInterface_getSyncData_70D8(iface, 0, (int)vs->mBEBuffer);
            func_00258290_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 6);
            break;
        case 6:
            if (func_00258280_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 7);
            }
            break;
        case 7:
            func_002581E8_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 8);
            break;
        case 8:
            if (func_002581D8_70D8(((char*)vs)) != 0) {
                cBENetworkInterface_setSyncData_70D8(iface, 1, (int)vs->mBEBuffer);
                cGameComm_setState_70D8(((char*)vs), 9);
            }
            break;
        case 9:
            vs->mNetLatency[0] = func_00255D88_70D8((int)cCommSystem_mCommSystem, vs->unk_06C);
            func_00258290_70D8(((char*)vs), (int)(((char*)vs) + 0x114), 4);
            cGameComm_setState_70D8(((char*)vs), 10);
            break;
        case 10:
            if (func_00258280_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 11);
            }
            break;
        case 11:
            func_002581E8_70D8(((char*)vs), (int)(((char*)vs) + 0x118), 4);
            cGameComm_setState_70D8(((char*)vs), 12);
            break;
        case 12:
            if (func_002581D8_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 13);
            }
            break;
        case 13:
            cGameComm_setState_70D8(((char*)vs), 14);
            break;
        case 14:
            func_00257A80_70D8(((char*)vs));
            break;
        case 15:
            break;
        }
    } else {
        switch ((unsigned int)vs->mState) {
        case 0:
            cGameComm_setState_70D8(((char*)vs), 3);
            break;
        case 3:
            func_002581E8_70D8(((char*)vs), (int)(((char*)vs) + 0xD4), 0x2C);
            cGameComm_setState_70D8(((char*)vs), 4);
            break;
        case 4:
            if (func_002581D8_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 1);
            }
            break;
        case 1:
            func_00258290_70D8(((char*)vs), (int)(((char*)vs) + 0x100), 0x14);
            cGameComm_setState_70D8(((char*)vs), 2);
            break;
        case 2:
            if (func_00258280_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 7);
            }
            break;
        case 7:
            func_002581E8_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 8);
            break;
        case 8:
            if (func_002581D8_70D8(((char*)vs)) != 0) {
                cBENetworkInterface_setSyncData_70D8(iface, 0, (int)vs->mBEBuffer);
                cGameComm_setState_70D8(((char*)vs), 5);
            }
            break;
        case 5:
            cBENetworkInterface_getSyncData_70D8(iface, 1, (int)vs->mBEBuffer);
            func_00258290_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 6);
            break;
        case 6:
            if (func_00258280_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 11);
            }
            break;
        case 11:
            func_002581E8_70D8(((char*)vs), (int)(((char*)vs) + 0x114), 4);
            cGameComm_setState_70D8(((char*)vs), 12);
            break;
        case 12:
            if (func_002581D8_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 9);
            }
            break;
        case 9:
            vs->mNetLatency[1] = func_00255D88_70D8((int)cCommSystem_mCommSystem, vs->unk_06C);
            func_00258290_70D8(((char*)vs), (int)(((char*)vs) + 0x118), 4);
            cGameComm_setState_70D8(((char*)vs), 10);
            break;
        case 10:
            if (func_00258280_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 13);
            }
            break;
        case 13:
            cGameComm_setState_70D8(((char*)vs), 14);
            break;
        case 14:
            func_00257A80_70D8(((char*)vs));
            break;
        case 15:
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_localDisconnected);
#ifdef SKIP_ASM

/**
 * Marks this machine as disconnected from the online game.
 *
 * If @p force is set, or there is no error (mError) and the race is not over (mState < kGameComm_GameOver), and this
 * machine's end time (unk_038) is not stamped yet, it stores the current time (SSXApp +0x18 / +0x10). Then clears
 * mConnected, mRaceInProgress and mPauseMenu[1] (the peer's pause) and sets mLocalDisconnected (reported as "DIDQUIT").
 *
 * @param gameComm  the game link (this)
 * @param force      non-zero to stamp the end time even after an error or the end of the race
 */
extern "C" void cGameComm_localDisconnected(cGameComm* gameComm, int force) {
    if (((force != 0) || (((gameComm->mError) == 0) && ((gameComm->mState) < 0xF))) && ((gameComm->unk_038) == 0)) {
        (gameComm->unk_038) = (int) ((int) (*(int *)((char*)(SSXApp) + (0x18))) / (int) (*(int *)((char*)(SSXApp) + (0x10))));
    }
    (gameComm->mConnected) = 0;
    (gameComm->mRaceInProgress) = 0;
    (gameComm->mPauseMenu[1]) = 0;
    (gameComm->mLocalDisconnected) = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_setCommError);
#ifdef SKIP_ASM
/**
 * Records the first comm error of the session for the results report.
 *
 * Only the first call acts: sets mCommError, sets mCommErrorWasSignificant when the game was not over (mState
 * below GameOver) and not both riders had finished, packs 11 flags into mFlagsOnCommError (mLocalFinishedRace
 * in bit 0 ... mLoading in bit 10) and copies mState into mStateOnCommError.
 */
extern "C" void cGameComm_setCommError(cGameComm* self) {
    if (self->mCommError == 0) {
        self->mCommError = 1;
        int *significant = (int *)(((char*)self) + 0x18);
        int isSignificant = 0;
        if (self->mLocalFinishedRace == 0 || self->mRemoteFinishedRace == 0) {
            int lateState = self->mState >= 0xF;
            if (!lateState) isSignificant = 1;
        }
        *significant = isSignificant;
        self->mFlagsOnCommError = (unsigned int)(self->mLocalFinishedRace | (self->mRemoteFinishedRace << 1) | (self->mLocalDisconnected << 2) | (self->mRemoteDisconnected << 3)
            | (self->mGameStartGate << 4) | (self->mRaceInProgress << 5) | (self->mAlone << 6) | (self->mRemoteExitGame << 7)
            | (self->mRemoteEndGame << 8) | (self->unk_090 << 9) | (self->mLoading << 10));
        self->mStateOnCommError = self->mState;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00257750);
#ifdef SKIP_ASM
extern "C" void func_00257750(cGameComm* gameComm) {
    if (gameComm->mDisplayError == 0 && gameComm->mAlone == 0 && (gameComm->mError == 0 || gameComm->mRaceInProgress == 0)) {
        int overlayTimeout = gameComm->mOverlayTimeout;
        if (overlayTimeout > 0) {
            if (gameComm->mPauseMenu[0] == 0 || gameComm->mPauseMenu[1] == 0) {
                gameComm->mOverlayTimeout = overlayTimeout - 1;
            }
        }
        int pauseTimeout = gameComm->mPauseTimeout;
        if (pauseTimeout > 0) {
            gameComm->mPauseTimeout = pauseTimeout - 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002577C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00255E40_77C8(int sys, int port) __asm__("func_00255E40");
extern "C" void cGameComm_setCommError(cGameComm* self);
extern "C" void cGameComm_displayError(cGameComm* self);
extern "C" void cGameComm_updateDisplayError(cGameComm* self);

extern "C" int func_002577C8(cGameComm* gameComm)
{
    if (gameComm->mDisplayError > 0) {
        cGameComm_updateDisplayError(gameComm);
        return 1;
    }
    if (gameComm->mError != 0) return 1;
    if (gameComm->mRemoteEndGame != 0) {
        cGameComm_displayError(gameComm);
        return 1;
    }
    if (func_00255E40_77C8((int)cCommSystem_mCommSystem, gameComm->unk_06C) != 0) {
        cGameComm_setCommError(gameComm);
        cGameComm_displayError(gameComm);
        return 1;
    }
    if (gameComm->unk_054 >= 3) {
        gameComm->mCheating[0] = 3;
        cGameComm_displayError(gameComm);
        return 1;
    }
    if (gameComm->mPauseCount[0] >= 6) {
        gameComm->mCheating[0] = 2;
        cGameComm_displayError(gameComm);
        return 1;
    }
    if (gameComm->unk_058 >= 3) {
        gameComm->mCheating[1] = 3;
        cGameComm_displayError(gameComm);
        return 1;
    }
    if (gameComm->mPauseCount[1] >= 6) {
        gameComm->mCheating[1] = 2;
        cGameComm_displayError(gameComm);
        return 1;
    }
    if (gameComm->mStallTimer >= 3600.0f) {
        cGameComm_setCommError(gameComm);
        cGameComm_displayError(gameComm);
        return 1;
    }
    if (gameComm->unk_094 <= 0) return 0;
    if (--gameComm->unk_094 > 0) return 0;
    gameComm->unk_02C = 1;
    cGameComm_setCommError(gameComm);
    cGameComm_displayError(gameComm);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_displayError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGameComm_updateDisplayError_78F8(void *) __asm__("cGameComm_updateDisplayError");
extern "C" void cGameComm_updateMessageDisplay_78F8(void *) __asm__("cGameComm_updateMessageDisplay");
extern "C" void func_00266DF8_78F8(int) __asm__("func_00266DF8");
extern int D_004A33F4;

/**
 * Ends the online session on an error: stops input syncing and brings up the online-error overlay.
 *
 * Always clears the waiting message, the peer's pause and the unpause flag and timer. On the first error
 * only, it sets mCheating[0] to 4 if this machine's controller is unplugged mid-race with no other error,
 * closes the voice link (func_00266DF8 on D_004A33F4, probably cVoipMan), lets the connection popup close,
 * clears mSyncInput and mConnected, sets mError and mDisplayError and runs updateDisplayError.
 */
extern "C" void cGameComm_displayError(cGameComm* self) {
    self->mWaitingMessage = 0;
    self->mPauseMenu[1] = 0;
    self->mUnPauseFlag = 0;
    self->mUnPauseTimer = 0;
    if (self->mError == 0) {
        if (self->mDesync == 0 && self->mCheating[0] == 0) {
            if ((self->mCommError == 0 || self->mRemoteExitGame != 0) && self->mLocalFinishedRace == 0 && self->mRaceInProgress != 0) {
                if (SSXApp->Game->ControllerUnplugged != 0) {
                    self->mCheating[0] = 4;
                }
            }
        }
        if (D_004A33F4 != 0) {
            func_00266DF8_78F8(D_004A33F4);
        }
        cGameComm_updateMessageDisplay_78F8(((char*)self));
        self->mSyncInput = 0;
        self->mConnected = 0;
        self->mError = 1;
        self->mDisplayError = 1;
        cGameComm_updateDisplayError_78F8(((char*)self));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateDisplayError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" char *cUIStateStack_getCurrentState_79C8(char *) __asm__("cUIStateStack_getCurrentState");
extern "C" void cOVStateManager_triggerOverlay_79C8(int) __asm__("cOVStateManager_triggerOverlay");
extern int cPauseState_Flags;
extern int D_004A2A54;
extern int D_00442924[];
extern int D_005366E8[];

/**
 * Counts down a pending error display and then raises the online-error overlay.
 *
 * Waits while mLoading is set and until the UI stack's current state is empty or of kind 5. When
 * mDisplayError reaches 0 it triggers overlay 0x27 (cOVState_PAUSE_ONLINE_ERROR) and pushes 13 onto the stack
 * at D_005366E8 (cPauseState_Flags = D_004428F0[13]), which the overlays pop.
 */
extern "C" void cGameComm_updateDisplayError(cGameComm* self) {
    if (self->mLoading == 0 && self->mDisplayError > 0) {
        char *uiEngine = (char*)SSXApp->Game->UI;
        char *st;
        if (uiEngine != 0 && (uiEngine += 0x18) != 0 && ((st = cUIStateStack_getCurrentState_79C8(uiEngine), st == 0) || (st[0x1D] & 0x3F) == 5)) {
            int n = self->mDisplayError - 1;
            self->mDisplayError = n;
            if (n <= 0) {
                cOVStateManager_triggerOverlay_79C8(0x27);
                int idx = D_004A2A54 + 1;
                D_005366E8[idx] = 0xD;
                cPauseState_Flags = D_00442924[0];
                D_004A2A54 = idx;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00257A80);
#ifdef SKIP_ASM
extern "C" void cGameComm_setCommError(cGameComm* self);
extern "C" void cGameComm_displayError(cGameComm* self);
extern int D_004A3ED0;

extern "C" void func_00257A80(cGameComm* gameComm)
{
    if (gameComm->mCheating[0] != 0) return;
    if (gameComm->mPauseMenu[0] == 0) {
        if (gameComm->mGameStartGate != 0) {
            if (gameComm->mPauseMenu[1] == 0 && gameComm->mUnPauseTimer <= 0) {
                char* g = (char*)SSXApp;
                int n = ++gameComm->mStallThreshold;
                ++gameComm->unk_060;
                unsigned int st = *(int*)(*(char**)(*(char**)(g + 0x84) + 0xC)) - 4;
                if (st < 2) {
                    if (n >= 10) gameComm->mStallTimer += 1.0f;
                    if (gameComm->mStallThreshold == 10) ++gameComm->mStallCount;
                }
            }
            if (gameComm->unk_060 == 60) gameComm->mWaitingMessage = 1;
        }
        if (gameComm->mRaceInProgress != 0) {
            if (gameComm->mDesync != 0) {
                cGameComm_setCommError(gameComm);
                cGameComm_displayError(gameComm);
                return;
            }
            if (gameComm->mPauseMenu[1] == 0) ++gameComm->unk_05C;
            if (gameComm->unk_05C >= 1800) {
                gameComm->unk_02C = 1;
                cGameComm_setCommError(gameComm);
                cGameComm_displayError(gameComm);
                return;
            }
        }
    }
    if (D_004A3ED0 != 0) {
        gameComm->mCheating[0] = 1;
        cGameComm_displayError(gameComm);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_readPacket);
#ifdef SKIP_ASM
struct Packet_7BD8 {
    unsigned char type;
    unsigned char flags;
    unsigned short size;    // 0x2, includes the 4-byte header
    char data[1];           // 0x4
};

struct PacketQueue_7BD8 {
    int wr;                         // 0x0
    int rd;                         // 0x4
    Packet_7BD8* slots[32];         // 0x8
};


extern "C" void func_00257CD8(cGameComm* c);
extern "C" void func_002581B8(cGameComm* arg0, int *arg1);
extern "C" void func_00258870(cGameComm*, int, int);
extern "C" void MEM_copy_7BD8(void* dst, const void* src, int n) __asm__("MEM_copy");

/**
 * Pops the oldest received packet of one type and copies its payload out.
 *
 * Returns 0 after a link error. Otherwise it first queues newly received packets (func_00257CD8,
 * updateReadQueue), then copies and frees the oldest packet of type @p ch. An input packet (type 0) also
 * clears the silence counters and the waiting message, and an armed link timeout is re-armed to 1800 frames.
 *
 * @param ch      packet type, which picks the read queue: 0 input samples, 1 chat, 2 BE sync data, 3 commands
 * @param dst     buffer for the payload (a pointer passed as an int)
 * @param maxsize  size of @p dst; never checked, so the whole payload (up to 236 bytes) is copied
 * @return the payload length (packet size minus the 4-byte header), or 0 when the queue is empty or after an
 *         error
 */
// PORT: the unit declares this as (void*, int, int, int); dst is a pointer passed as int
extern "C" int cGameComm_readPacket(cGameComm* self, int ch, int dst, int maxsize)
{
    cGameComm* gameComm = self;
    if (gameComm->mError != 0) return 0;
    func_00257CD8(gameComm);
    PacketQueue_7BD8* queue = &((PacketQueue_7BD8*)&gameComm->mReadQueue[0])[ch];
    if (queue->rd != queue->wr) {
        Packet_7BD8* packet = queue->slots[queue->rd++ % 32];
        int len = packet->size - 4;
        MEM_copy_7BD8((void*)dst, packet->data, len);
        func_002581B8((cGameComm*)gameComm, (int*)packet);
        if (ch == 0) {
            gameComm->unk_05C = 0;
            gameComm->unk_060 = 0;
            gameComm->mStallThreshold = 0;
            gameComm->mWaitingMessage = 0;
        }
        if (gameComm->unk_094 > 0) func_00258870((cGameComm*)gameComm, 0x708, 0);
        return len;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00257CD8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Fuller layout of Conn_7BD8 (ours lacks unk6C and pending), under a local name.
struct Conn_7CD8 {
    char pad0[4];
    int busy;                       // 0x4
    char pad8[0x6C - 0x8];
    int unk6C;                      // 0x6C
    char pad70[0x134 - 0x70];
    Packet_7BD8* pending;           // 0x134
    char pad138[0x144 - 0x138];
    PacketQueue_7BD8 queues[2];     // 0x144
};

// PORT: func_00258188 takes no arguments; the caller passes the connection anyway.
Packet_7BD8* func_00258188_7CD8(Conn_7CD8* c) __asm__("func_00258188");
extern "C" void operator_delete__FPi_7CD8(void *) __asm__("operator_delete__FPi");

static inline int queueFull_7CD8(PacketQueue_7BD8* q)
{
    return q->wr - q->rd >= 0x1F;
}

// Moves received packets into their channel queue. A packet that cannot be
// fetched is freed; the whole fetch sits in the loop condition, as in the original.
extern "C" void func_00257CD8_7CD8(Conn_7CD8* c) __asm__("func_00257CD8");
extern "C" void func_00257CD8_7CD8(Conn_7CD8* c)
{
    while (c->busy == 0
           && (c->pending != 0
               || (c->pending = func_00258188_7CD8(c), func_00255DC0((void*)cCommSystem_mCommSystem, c->unk6C, (int)c->pending, 0xF0) != 0)
               || (operator_delete__FPi_7CD8(c->pending), c->pending = 0, 0))) {
        Packet_7BD8* p = c->pending;
        PacketQueue_7BD8* q = &c->queues[p->type];
        if (queueFull_7CD8(q)) {
            return;
        }
        q->slots[q->wr++ % 32] = p;
        c->pending = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateWriteQueue);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00255E00_7DC0(int, int, char *, unsigned short) __asm__("func_00255E00");
extern "C" void func_002581B8_7DC0(char *, char *) __asm__("func_002581B8");
struct S7DC0 { char pad[0x36C]; char *arr[32]; };

/**
 * Sends the packets waiting in the write queue, oldest first, until the channel refuses one.
 *
 * Each packet goes to the comm system's writePacket for this link's channel (func_00255E00); a sent packet is
 * popped and freed (func_002581B8). It stops at once after a link error.
 */
extern "C" void cGameComm_updateWriteQueue(cGameComm* self) {
    if (self->mWriteQueue.mTail != self->mWriteQueue.mHead) {
        do {
            if (self->mError != 0) return;
            int idx = self->mWriteQueue.mTail;
            char *item;
            if (idx < self->mWriteQueue.mHead) {
                item = ((S7DC0 *)self)->arr[idx % 32];
            } else {
                item = 0;
            }
            if (func_00255E00_7DC0((int)cCommSystem_mCommSystem, self->unk_06C, item, *(unsigned short *)(item + 2)) == 0) return;
            self->mWriteQueue.mTail = self->mWriteQueue.mTail + 1;
            func_002581B8_7DC0(((char*)self), item);
        } while (self->mWriteQueue.mTail != self->mWriteQueue.mHead);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00257E98);
#ifdef SKIP_ASM
struct Packet_7E98 {
    unsigned char type;     // 0x0
    unsigned char flags;    // 0x1
    unsigned short size;    // 0x2, includes this 4-byte header
    char data[0xEC];        // 0x4
};

extern "C" int func_00255E00_7E98(int sys, int port, Packet_7E98* p, int size) __asm__("func_00255E00");
extern "C" Packet_7E98* func_00258188_7E98(char* self) __asm__("func_00258188");
extern "C" void MEM_copy_7E98(void* dst, const void* src, int n) __asm__("MEM_copy");
extern "C" void cGameComm_updateWriteQueue(cGameComm* self);

struct Conn_7E98 {
    char pad0[0x36C];
    Packet_7E98* slots[32];     // 0x36C
};

// PORT: the unit declares this void(void*, int, int, int); the target returns a status
extern "C" int func_00257E98_impl(char* self, int type, const void* data, int len) __asm__("func_00257E98");
extern "C" int func_00257E98_impl(char* self, int type, const void* data, int len)
{
    Packet_7E98 pkt;
    if (*(int*)(self + 4) != 0) return 0;
    cGameComm_updateWriteQueue((cGameComm*)self);
    pkt.type = type;
    pkt.size = len + 4;
    pkt.flags = 0;
    MEM_copy_7E98(pkt.data, data, len);
    if (*(int*)(self + 0x368) == *(int*)(self + 0x364)) {
        if (func_00255E00_7E98((int)cCommSystem_mCommSystem, *(int*)(self + 0x6C), &pkt, pkt.size)) return 1;
    }
    {
        Packet_7E98* queuedPacket = func_00258188_7E98(self);
        MEM_copy_7E98(queuedPacket, &pkt, pkt.size);
        ((Conn_7E98*)self)->slots[(*(int*)(self + 0x364))++ % 32] = queuedPacket;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00257F98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: the unit declares this as (void*, int, int, int); dst is the packet buffer
extern "C" int cGameComm_readPacket_7F98(void* self, int ch, void* dst, int size) __asm__("cGameComm_readPacket");
// PORT: defined later in the unit with a struct param; bind a void* view by asm label
extern "C" void cGameComm_setUnPauseTimer_7F98(void* self) __asm__("cGameComm_setUnPauseTimer");

struct sGamePacket_7F98 {
    unsigned char type;
    int a;      // 0x4
    int b;      // 0x8
};

struct sGameComm_7F98 {
    int f0;
    int f4;
    char pad8[4];
    int fC;
    int f10;
    int f14;
    char pad18[0x3C - 0x18];
    int f3C;
    char pad40[0x50 - 0x40];
    int f50;
    char pad54[4];
    int f58;
    int f5C;
    int f60;
    int f64;
    char pad68[0x70 - 0x68];
    int f70;
    int f74;
    int f78;
    int f7C;
    char pad80[4];
    int f84;
    int f88;
    int f8C;
    char pad90[0x3F4 - 0x90];
    int f3F4;
    int f3F8;
    int f3FC;
    char pad400[4];
    int f404;
    char pad408[4];
    int f40C;
    char pad410[0x500 - 0x410];
    int f500[60];
};

extern "C" void func_00257F98(sGameComm_7F98* self)
{
    sGamePacket_7F98 msg;
    while (cGameComm_readPacket_7F98(self, 3, &msg, 0xC)) {
        switch (msg.type) {
        case 0:
            if (self->f7C == 0) {
                func_00258870((cGameComm*)self, 0xE4C, 1);
            } else {
                self->f3F8 = 1;
            }
            self->f78 = 1;
            self->f5C = 0;
            self->f50++;
            self->f3FC = 0;
            break;
        case 1:
            self->f5C = 0;
            self->f78 = 0;
            if (self->f74 != 0 && self->f64 > 0) {
                self->f64 = 0x4B0;
            }
            self->f3F8 = 0;
            self->f60 = 0;
            self->f84 = 0;
            if (self->f7C == 0) {
                func_00258870((cGameComm*)self, 0x708, 0);
            } else {
                cGameComm_setUnPauseTimer_7F98(self);
            }
            break;
        case 3:
            self->f3F4 = 1;
            break;
        case 5:
            self->f88 = 1;
            if (self->f3C == 0) {
                self->f3C = *(int*)((char*)SSXApp + 0x18) / *(int*)((char*)SSXApp + 0x10);
            }
            self->f78 = 0;
            self->f0 = 0;
            self->f14 = 1;
            if (self->f4 == 0 && self->f70 < 15) {
                self->f8C = 1;
            }
            break;
        case 2:
            self->f58++;
            break;
        case 4:
            self->f404 = 1;
            self->f40C = msg.a;
            self->f500[msg.a / 60 % 60] = msg.b;
            break;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258160);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cGameComm_readPacket_8160(int, int, int, int) __asm__("cGameComm_readPacket");

extern "C" int func_00258160(int gameComm, int data, int maxSize) {
    return cGameComm_readPacket_8160(gameComm, 1, data, maxSize) > 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258188);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_8188(unsigned int, void *, unsigned int, int) __asm__("cMemMan_alloc");
extern char str_tGameCommPacket[];

extern "C" void *func_00258188(void) {
    return cMemMan_alloc_8188(0xF0, str_tGameCommPacket, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002581B8);
#ifdef SKIP_ASM
extern "C" void operator_delete__FPi(int *);

extern "C" void func_002581B8(cGameComm* gameComm, int *packet) {
    operator_delete__FPi(packet);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002581D8);
#ifdef SKIP_ASM
extern "C" int func_002581D8(cGameComm* gameComm) {
    return ((int)gameComm->mAsyncReadData) == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002581E8);
#ifdef SKIP_ASM
extern "C" void func_002581E8(void *gameComm, int data, int size) {
    (*(int *)((char*)(gameComm) + (0x124))) = data;
    (*(int *)((char*)(gameComm) + (0x128))) = size;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_002581F8);
#ifdef SKIP_ASM
extern "C" int cGameComm_readPacket(cGameComm*, int, int, int);

extern "C" void func_002581F8(cGameComm* gameComm) {
    int readData;
    while ((readData = (int)gameComm->mAsyncReadData) != 0) {
        int n = gameComm->mAsyncReadSize;
        if (n >= 0xED) n = 0xEC;
        if (cGameComm_readPacket(gameComm, 2, readData, n) == 0) break;
        gameComm->mAsyncReadData = (char*)((int)gameComm->mAsyncReadData + n);
        int rem = gameComm->mAsyncReadSize - n;
        gameComm->mAsyncReadSize = rem;
        if (rem <= 0) {
            gameComm->mAsyncReadData = 0;
            gameComm->mAsyncReadSize = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258280);
#ifdef SKIP_ASM
extern "C" int func_00258280(cGameComm* gameComm) {
    return ((int)gameComm->mAsyncWriteData) == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258290);
#ifdef SKIP_ASM
extern "C" void func_00258290(void *gameComm, int data, int size) {
    (*(int *)((char*)(gameComm) + (0x12C))) = data;
    (*(int *)((char*)(gameComm) + (0x130))) = size;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_002582A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00257E98_82A0(void *, int, int, int) __asm__("func_00257E98");

extern "C" void func_002582A0(cGameComm* gameComm) {
    int writeData = (int)gameComm->mAsyncWriteData;
    if (writeData != 0 && gameComm->mWriteQueue.mTail == gameComm->mWriteQueue.mHead) {
        int n = gameComm->mAsyncWriteSize;
        if (n >= 0xED) n = 0xEC;
        if (func_00257E98_82A0(((char*)gameComm), 2, writeData, n) != 0) {
            gameComm->mAsyncWriteData = (char*)((int)gameComm->mAsyncWriteData + n);
            int rem = gameComm->mAsyncWriteSize - n;
            gameComm->mAsyncWriteSize = rem;
            if (rem <= 0) {
                gameComm->mAsyncWriteData = 0;
                gameComm->mAsyncWriteSize = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258330);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.

extern "C" void func_00258330(cGameComm* gameComm) {
    char *app = (char*)SSXApp;
    float latency1 = gameComm->mNetLatency[1];
    float latency0 = gameComm->mNetLatency[0];
    float maxLatency = latency1 >? latency0;
    gameComm->mLatency = maxLatency;
    float frameTime = *(float *)(app + 0x14);
    if (maxLatency < frameTime) {
        gameComm->mLatency = frameTime;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258360);
#ifdef SKIP_ASM
extern "C" float func_00255D88(int, int);

extern "C" float func_00258360(cGameComm* gameComm) {
    return func_00255D88((int)cCommSystem_mCommSystem, (gameComm->unk_06C)) + ((float) (gameComm->mNumSamples) * (*(float *)((char*)(SSXApp) + (0x14))));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002583A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003E7450_83A8(int, int, char *, int) __asm__("func_003E7450");
extern char str_SSXPingServer[];

struct S83A8 {
    char pad0[0xB8];
    int count;
    int bc;
    int c0;
    int arr[0x1D];
    int t138;
    int t13c;
    int h140;
};

extern "C" void func_002583A8(cGameComm* self, int arg1, int arg2) {
    if (self->unk_138 <= 0) {
        self->unk_0B8 = 0;
        self->unk_0C0 = arg1;
        self->unk_0C4[self->unk_0B8++] = func_003E7450_83A8((int)self->unk_140, arg1, str_SSXPingServer, 0xE);
        self->unk_0BC = 4;
        self->unk_138 = arg2 * 0x3C;
        self->unk_13C = 0;
    }
    if (cCommSystem_mCommSystem->mBroadBand == 0) {
        self->unk_13C = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_syncGame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator_new_8448(unsigned int size, const char* tag, int flags, int align) __asm__("operator_new__FUi");
void cMemMan_free_8448(void* p) __asm__("cMemMan_free__FPv");
extern "C" void cBENetworkInterface_preInitBE(void* iface);
extern "C" void cBENetworkInterface_postInitBE(void* iface);
extern "C" void func_0014F898();
extern "C" void cBENetworkInterface_getSyncData_8448(void* iface, int which, void* buf) __asm__("cBENetworkInterface_getSyncData");
extern "C" void cBENetworkInterface_setSyncData_8448(void* iface, int which, void* buf) __asm__("cBENetworkInterface_setSyncData");
extern "C" void MEM_fill_8448(char* p, int c, int n) __asm__("MEM_fill");
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void* cBE_getBE();
extern "C" int cBENewPlayerInterface_getPlayerCtrlID(void* iface, int a1);
extern "C" void cBENewPlayerInterface_setPlayerCtrlID(void* iface, int a1, int a2);
extern "C" void GMsrand(unsigned int seed);
extern "C" void THREAD_yieldticks(int);
unsigned int AIrand();
void AIgetseed_8448(void* self) __asm__("AIgetseed__FPv");
void* AIsetseed(void* self);

struct sVEnt_8448 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern char str_bebuffer[];
extern char str_NA_R_F004[];

static inline int ggHost_8448(char* g) { return *(int*)(g + 8); }
static inline void ggSetTime_8448(char* g, float t) { *(float*)(g + 0x10) = t; }
static inline int ggSynced_8448(char* s) { return *(int*)(s + 0x70) == 0xE; }
static inline int ggDropped_8448(char* s) { return *(int*)(s + 4); }

/**
 * Runs the blocking pre-race handshake with the peer: exchanges back-end sync data and the AI random state.
 *
 * Packs this side's BE sync data into mBEBuffer (player 0 on the host; the client first swaps the option
 * records and gives player 1 player 0's controller), saves the AI random state and pumps update
 * (cGameComm_update) until GameActive or an error. Then it runs postInitBE, restores the AI random state from
 * mServerSettings, stores mLatency in networkOpt.fLatency, uses 2 input samples per packet when both sides
 * are broadband, and raises a comm error when the two build signatures differ.
 */
// Joins the game session: sets up the sync buffer and this side's name slot, then pumps
// messages until the sync state (0xE) is reached or the link drops.
extern "C" void cGameComm_syncGame(cGameComm* self)
{
    void* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    self->mBEBuffer = (char*)operator_new_8448((unsigned int)self->mBEBufferSize, str_bebuffer, 0x100, 0);
    cBENetworkInterface_preInitBE(iface);
    if (ggHost_8448(((char*)&cBENetworkInterface_networkOpt)) != 0) {
        MEM_fill_8448(((char*)self) + 0xF0, 0, 0x10);
        strncpy(((char*)self) + 0xF0, str_NA_R_F004, 0xF);
        self->mServerSettings.unk_18 = cCommSystem_mCommSystem->mBroadBand;
        cBENetworkInterface_getSyncData_8448(iface, 0, self->mBEBuffer);
        cBENetworkInterface_setSyncData_8448(iface, 0, self->mBEBuffer);
    } else {
        func_0014F898();
        MEM_fill_8448(((char*)self) + 0x104, 0, 0x10);
        strncpy(((char*)self) + 0x104, str_NA_R_F004, 0xF);
        self->unk_100 = cCommSystem_mCommSystem->mBroadBand;
        cBENetworkInterface_getSyncData_8448(iface, 1, self->mBEBuffer);
        cBENetworkInterface_setSyncData_8448(iface, 1, self->mBEBuffer);
        void* ui = cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerCtrlID(ui, 1, cBENewPlayerInterface_getPlayerCtrlID(ui, 0));
        sVEnt_8448* vt = *(sVEnt_8448**)((char*)ui + 0xC);
        vt[1].fn((char*)ui + vt[1].delta);
    }
    AIgetseed_8448(((char*)self) + 0xD4);
    while (!ggSynced_8448(((char*)self)) && ggDropped_8448(((char*)self)) == 0) {
        cGameComm_update(self);
        THREAD_yieldticks(1);
    }
    if (self->mBEBuffer) {
        cMemMan_free_8448(self->mBEBuffer);
    }
    self->mNumSamples = 3;
    self->mBEBuffer = 0;
    if (self->mError == 0) {
        cBENetworkInterface_postInitBE(iface);
        AIsetseed(((char*)self) + 0xD4);
        GMsrand(AIrand());
        ggSetTime_8448(((char*)&cBENetworkInterface_networkOpt), self->mLatency);
        if (self->mServerSettings.unk_18 != 0 && self->unk_100 != 0) {
            self->mNumSamples = 2;
        }
        if (self->mError == 0 && func_00416B18_70D8(((char*)self) + 0xF0, ((char*)self) + 0x104, 9) != 0) {
            cGameComm_setCommError(self);
            cGameComm_displayError(self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_syncWait);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cBE_getInterface_86B0(int, int) __asm__("cBE_getInterface__Fv");
extern "C" void func_00257E98(void *, int, int, int);
extern "C" void func_00258870(cGameComm*, int, int);
extern "C" void cGameComm_update(cGameComm*);
extern "C" void THREAD_yieldticks(int);
static inline int rd8(char *p) { return *(int*)(p + 8); }
/**
 * Waits for the peer at the end of loading by exchanging the sync-wait command (3).
 *
 * The host sends command 3 first and then waits; the client waits for the host's and then answers. While
 * waiting it pumps update (cGameComm_update) with a 3600-frame link timeout armed, until mSyncWaitComplete or an
 * error. Afterwards it disarms the timeout (unk_094) and clears mLoading.
 */
extern "C" void cGameComm_syncWait(cGameComm* self) {
    char buf[12];
    cBE_getInterface_86B0((int)SSXApp->BE, 7);
    if (rd8(((char*)&cBENetworkInterface_networkOpt)) != 0) {
        buf[0] = 3;
        func_00257E98(self, 3, (int)buf, 0xC);
    }
    func_00258870(self, 0xE10, 0);
    while (self->mSyncWaitComplete == 0 && self->mError == 0) {
        cGameComm_update(self);
        THREAD_yieldticks(1);
    }
    self->unk_094 = 0;
    if (self->mError == 0 && rd8(((char*)&cBENetworkInterface_networkOpt)) == 0) {
        buf[0] = 3;
        func_00257E98(self, 3, (int)buf, 0xC);
    }
    self->mLoading = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_sendPauseCmd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00257E98(void *, int, int, int);
extern "C" void func_00258870(cGameComm*, int, int);
extern void cGameComm_setUnPauseTimer_8790(void *) __asm__("cGameComm_setUnPauseTimer");
/**
 * Tells the peer that this machine paused or unpaused, and updates the local pause bookkeeping.
 *
 * Needs a live link. Pausing sets mPauseMenu[0], counts mPauseCount[0] and force-arms a 3660-frame link
 * timeout before the start gate, or sets mUnPauseFlag after it. Unpausing clears those, the silence counters
 * and the waiting message, then arms an 1800-frame timeout or, after the gate, starts the unpause countdown.
 * Only the command byte (0 or 1) of the 12-byte payload is set; the rest is uninitialised stack.
 *
 * @param pause  nonzero to pause, 0 to unpause
 */
extern "C" void cGameComm_sendPauseCmd(cGameComm* self, int pause) {
    char buf[12];
    if (self->mError == 0 && self->mConnected != 0) {
        if (pause != 0) {
            if (self->mGameStartGate == 0) {
                func_00258870(self, 0xE4C, 1);
            } else {
                self->mUnPauseFlag = 1;
            }
            buf[0] = 0;
            self->mPauseMenu[0] = 1;
            self->mPauseCount[0] = self->mPauseCount[0] + 1;
            self->mUnPauseTimer = 0;
            self->unk_05C = 0;
        } else {
            buf[0] = 1;
            self->mPauseMenu[0] = 0;
            self->mUnPauseFlag = 0;
            self->unk_060 = 0;
            self->mWaitingMessage = 0;
            self->unk_05C = 0;
            if (self->mGameStartGate == 0) {
                func_00258870(self, 0x708, 0);
            } else {
                cGameComm_setUnPauseTimer_8790(self);
            }
        }
        func_00257E98(self, 3, (int)buf, 0xC);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258870);
#ifdef SKIP_ASM
extern "C" void func_00258870(cGameComm* gameComm, int arg1, int arg2) {
    if ((arg2 != 0) || (((gameComm->mPauseMenu[1]) == 0) && ((gameComm->mPauseMenu[0]) == 0)) || ((gameComm->mGameStartGate) != 0)) {
        (gameComm->unk_094) = arg1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_setUnPauseTimer);
#ifdef SKIP_ASM

struct S88A8_4 { int k0; char p[0x14]; int k18; int k1c; };
struct S88A8_S { int k0; int k4; char p0[0x40]; int k48; char p1[0x24]; int k70; int k74; int k78; char p2[0x380]; int k3fc; };

/**
 * Starts the 180-frame unpause countdown (mUnPauseTimer) when the race can resume, otherwise clears it.
 *
 * The countdown starts only outside game state 2, with a live link (no error, not alone, connected), before
 * GameOver, with neither side paused, the AI's EndOfRaceTimer at 0 or below, and not in AI state 4 with more
 * than 150 countdown ticks left.
 */
extern "C" void cGameComm_setUnPauseTimer(cGameComm* self) {
    cGame *game;
    S88A8_4 *gameAI;
    self->mUnPauseTimer = 0;
    game = ((SSXApp)->Game);
    if (game->mCurrentStateID != 2 && self->mError == 0 && self->mAlone == 0 && self->mConnected != 0
        && self->mState < 0xF
        && ((gameAI = ((S88A8_4*)game->AI), gameAI->k0 != 4) || gameAI->k1c < 0x97)
        && gameAI->k18 <= 0 && self->mPauseMenu[0] == 0 && self->mPauseMenu[1] == 0) {
        self->mUnPauseTimer = 0xB4;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258950);
#ifdef SKIP_ASM
extern "C" void func_00258950(cGameComm* gameComm) {
    (gameComm->mWaitingMessage) = 0;
    (gameComm->mAlone) = 1;
    (gameComm->mPauseMenu[1]) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_gameStartGate);
#ifdef SKIP_ASM
/**
 * Notes that the race's start gate is up: arms the start-gate flag while the game is still running.
 *
 * Only while mState is below kGameComm_GameOver (15): clears the link timeout unk_094 and sets mGameStartGate. Called
 * by cGFGateState_gainFocus. On Tour also clears mRaceInProgress and runs heatInit here.
 *
 * @param gameComm  the game link (this)
 */
extern "C" void cGameComm_gameStartGate(cGameComm* gameComm) {
    if ((gameComm->mState) < 0xF) {
        (gameComm->unk_094) = 0;
        (gameComm->mGameStartGate) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_gameStartRace);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00260F80_8988(void) __asm__("func_00260F80");
extern "C" int func_00261460_8988(int) __asm__("func_00261460");
extern "C" void func_002613C8_8988(int) __asm__("func_002613C8");
extern "C" void func_0025B800_8988(void *) __asm__("func_0025B800");
extern "C" void func_00262768_8988(int, int, int, int, int, int) __asm__("func_00262768");

/**
 * Marks the online race as started and tells the buddy server that this player is racing.
 *
 * Does nothing once the game is over. Creates (func_00260F80) and connects (func_002613C8) BuddyMan when
 * needed, then sets its presence to LOCID 9 with the opponent (LobbyMan mOpponent) through func_00262768.
 * Stores the start time (SSXApp +0x18 / +0x10) in unk_034, sets mRaceInProgress and disarms the link timeout.
 */
extern "C" void cGameComm_gameStartRace(cGameComm* self) {
    if (self->mState < 0xF) {
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80_8988();
        }
        if (func_00261460_8988((int)BuddyMan_mBuddyMan) == 0) {
            func_002613C8_8988((int)BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800_8988((char*)LobbyMan_mLobbyMan);
        }
        BuddyMan* buddyMan = BuddyMan_mBuddyMan;
        if (buddyMan != 0) {
            func_00262768_8988((int)buddyMan, 1, 9, ((int)LobbyMan_mLobbyMan->mOpponent), 0, 0);
        }
        self->unk_034 = *(int *)((char*)SSXApp + 0x18) / *(int *)((char*)SSXApp + 0x10);
        self->mRaceInProgress = 1;
        self->unk_094 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_gameEnded);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGameComm_setState_8A48(void *, int) __asm__("cGameComm_setState");

/**
 * Ends the online race: stops input sync, enters the game-over state and stamps both end times.
 *
 * Clears mRaceInProgress and mSyncInput, calls cGameComm_setState(kGameComm_GameOver) unless mState is already there,
 * and stamps this machine's (unk_038) and the peer's (unk_03C) end times with the current time (SSXApp +0x18 / +0x10)
 * if they are still 0. Called by cGameComm_gameExit and func_00113E18 (ai).
 *
 * @param gameComm  the game link (this)
 */
extern "C" void cGameComm_gameEnded(cGameComm* gameComm) {
    (gameComm->mRaceInProgress) = 0;
    (gameComm->mSyncInput) = 0;
    if ((gameComm->mState) < 0xF) {
        cGameComm_setState_8A48(gameComm, 0xF);
    }
    if ((gameComm->unk_038) == 0) {
        (gameComm->unk_038) = (int) ((int) (*(int *)((char*)(SSXApp) + (0x18))) / (int) (*(int *)((char*)(SSXApp) + (0x10))));
    }
    if ((gameComm->unk_03C) == 0) {
        (gameComm->unk_03C) = (int) ((int) (*(int *)((char*)(SSXApp) + (0x18))) / (int) (*(int *)((char*)(SSXApp) + (0x10))));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_gameExit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cIface_AE0 { public: int f0, f4, f8; virtual void v0(); virtual void v1(); };
void* cBE_getInterface_8AE0(int, int) __asm__("cBE_getInterface__Fv");
extern "C" void func_00260F80();
extern "C" int func_00261460(int);
extern "C" void func_002613C8(int);
extern "C" void func_0025B800(int);
extern "C" void func_00262768(int, int, int, int, int, int);
extern "C" void func_00258BC8(cGameComm*);
extern "C" void cGameComm_gameEnded(cGameComm*);
extern "C" void cBENetworkInterface_copyProfile(void *, int, int);
extern "C" void* cBE_getBE();
extern int D_00534B38[];
/**
 * Leaves the online game: resets the buddy presence, tells the peer and ends the game locally.
 *
 * Makes sure BuddyMan exists and is connected, as gameStartRace does, and sets presence LOCID 0. Sends the
 * exit-game command (func_00258BC8) and runs gameEnded (cGameComm_gameEnded). On the client it then copies profile
 * 1 into profile 0 and reloads the Stat interface (BE interface 3, virtual discard).
 */
extern "C" void cGameComm_gameExit(cGameComm* self) {
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460((int)BuddyMan_mBuddyMan) == 0) {
        func_002613C8((int)BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800((int)LobbyMan_mLobbyMan);
    }
    BuddyMan* buddyMan = BuddyMan_mBuddyMan;
    if (buddyMan != 0) {
        func_00262768((int)buddyMan, 1, 0, 0, 0, 0);
    }
    func_00258BC8(self);
    cGameComm_gameEnded(self);
    void *i = cBE_getInterface_8AE0((int)SSXApp->BE, 7);
    if (D_00534B38[0] == 0) {
        cBENetworkInterface_copyProfile(i, 1, 0);
        cIface_AE0 *beStat = (cIface_AE0*)cBE_getInterface_8AE0((int)cBE_getBE(), 3);
        beStat->v1();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258BC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00257E98_8BC8(void *, int, void *, int) __asm__("func_00257E98");

extern "C" void func_00258BC8(cGameComm* gameComm) {
    if ((gameComm->mError) == 0) {
        char buf[12];
        buf[0] = 5;
        func_00257E98_8BC8(gameComm, 3, buf, 0xC);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258C00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00257E98_8C00(void *, int, void *, int) __asm__("func_00257E98");

extern "C" void func_00258C00(cGameComm* gameComm) {
    if (gameComm->mError == 0 && gameComm->mConnected != 0) {
        char buf[12];
        buf[0] = 2;
        gameComm->unk_054 = gameComm->unk_054 + 1;
        func_00257E98_8C00(gameComm, 3, buf, 0xC);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258C50);
#ifdef SKIP_ASM
extern "C" void func_00257E98(void *, int, int, int);
extern "C" int strlen(int);

extern "C" void func_00258C50(cGameComm* gameComm, int message) {
    if (((gameComm->mError) == 0) && ((gameComm->mConnected) != 0)) {
        func_00257E98(gameComm, 1, message, strlen(message) + 1);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateMessageDisplay);
#ifdef SKIP_ASM
int GetHashValue32(char* s);
extern "C" void* cUIStateStack_findState(void* mgr, int hash);
extern "C" void strcpy1(char* dst, const char* src);
extern char str_hud_d[];
extern char str_kT_POPUPCMNOtherPauseMenu[];
extern char str_kT_OVRCMNWaitPlayer_b[];

struct sVEntS_8CB0 {
    short delta;
    short index;
    const char* (*fn)(void*, int);
};

struct sVEntM_8CB0 {
    short delta;
    short index;
    int (*fn)(void*, int, char*);
};

// Whether no NIS cut-scene list is playing: mPlayingNISList is 2 when none is (the engine's constructor sets 2,
// and IsNISActive tests != 2). An earlier reading of this field as "online: connected" was wrong.
static inline int gcNoNISPlaying_8CB0()
{
    return ScriptEngine->mPlayingNISList == 2;
}

// Sends message `msg` (with text `arg`) to the UI item through its vtable slot 24.
static inline void gcSend_8CB0(char* item, int msg, char* arg)
{
    sVEntM_8CB0* vt = *(sVEntM_8CB0**)(item + 8);
    vt[24].fn(item + vt[24].delta, msg, arg);
}

// Looks up the string table entry for `name` (vtable slot 4 of the global string table at SSXApp+0x8C).
static inline const char* gcString_8CB0(char* name)
{
    char* tbl = (char*)SSXApp->FELocale;
    sVEntS_8CB0* vt = *(sVEntS_8CB0**)(tbl + 4);
    return vt[4].fn(tbl + vt[4].delta, GetHashValue32(name));
}

/**
 * Opens, retitles or closes the HUD's connection popup as the peer pauses or goes quiet.
 *
 * Sends the "hud" UI state message 0xE (open, with "kT_POPUPCMNOtherPauseMenu" if the peer paused, else
 * "kT_OVRCMNWaitPlayer") or 0xF (close), tracking it in mMessageDisplay (0 closed, 1 peer paused, 2 waiting).
 * The peer-paused popup opens only in AI state 4 or 5 or during an NIS; any popup closes when this machine
 * pauses. While mWaitingMessage is set, unk_060 is cleared on every call.
 */
// Drives the connection-status popup: picks its text, then opens (0xE) or closes (0xF) it as the
// link state changes (self+0x98: 0 closed, 1 shown while connected, 2 shown while waiting).
extern "C" void cGameComm_updateMessageDisplay(cGameComm* self)
{
    char buf[0x100];
    if (self->mWaitingMessage != 0) {
        self->unk_060 = 0;
    }
    char* ui = (char*)SSXApp->Game->UI;
    if (ui == 0) {
        return;
    }
    char* item = (char*)cUIStateStack_findState(ui + 0x18, GetHashValue32(str_hud_d));
    if (item == 0) {
        return;
    }
    if (self->mPauseMenu[1] != 0) {
        strcpy1(buf, gcString_8CB0(str_kT_POPUPCMNOtherPauseMenu));
    } else {
        strcpy1(buf, gcString_8CB0(str_kT_OVRCMNWaitPlayer_b));
    }
    switch (self->mMessageDisplay) {
    case 0:
        if (self->mPauseMenu[0] == 0) {
            if (self->mPauseMenu[1] != 0) {
                int kind = *(int*)SSXApp->Game->AI;
                if (kind == 4 || kind == 5 || !gcNoNISPlaying_8CB0()) {
                    gcSend_8CB0(item, 0xE, buf);
                    self->mMessageDisplay = 1;
                }
            } else if (self->mWaitingMessage != 0) {
                gcSend_8CB0(item, 0xE, buf);
                self->mMessageDisplay = 2;
            }
        }
        break;
    case 2:
        if (self->mPauseMenu[0] != 0) {
            gcSend_8CB0(item, 0xF, 0);
            self->mMessageDisplay = 0;
        } else if (self->mPauseMenu[1] != 0) {
            gcSend_8CB0(item, 0xE, buf);
            self->mMessageDisplay = 1;
        } else if (self->mWaitingMessage == 0) {
            gcSend_8CB0(item, 0xF, 0);
            self->mMessageDisplay = 0;
        } else if (self->mUnPauseTimer > 0) {
            gcSend_8CB0(item, 0xF, 0);
            self->mMessageDisplay = 0;
        }
        break;
    case 1:
        if (SSXApp->Game->mCurrentStateID == 2 && gcNoNISPlaying_8CB0()) {
            gcSend_8CB0(item, 0xF, 0);
            self->mMessageDisplay = 0;
        } else if (self->mPauseMenu[1] == 0 || self->mPauseMenu[0] != 0) {
            gcSend_8CB0(item, 0xF, 0);
            self->mMessageDisplay = 0;
        }
        break;
    default:
        gcSend_8CB0(item, 0xF, 0);
        self->mMessageDisplay = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258F20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00257E98_8F20(void* self, int type, const void* data, int len) __asm__("func_00257E98");

struct SyncMsg_8F20 {
    unsigned char type;     // 0x0
    int frame;              // 0x4
    int value;              // 0x8
};

struct Ring_8F20 {
    int v[60];
};

static inline int& RingAt_8F20(Ring_8F20* r, int frame)
{
    return r->v[(frame / 60) % 60];
}

extern "C" void func_00258F20(cGameComm* gameComm, int frame, int value)
{
    char* gs = (char*)SSXApp->Game;
    int st = **(int**)(gs + 0xC);
    if (st != 5) {
        if (st != 3) return;
        if (*(int*)(gs + 0x214) != st) return;
    }
    if (gameComm->mSyncInput == 0) return;
    if (gameComm->mError != 0) return;
    if (gameComm->mConnected == 0) return;
    if (gameComm->mDesync != 0) return;
    RingAt_8F20((Ring_8F20*)(((char*)gameComm) + 0x410), frame) = value;
    gameComm->mAITick[0] = frame;
    if (gameComm->mCheckSumSent != 0) {
        if (gameComm->mCheckSumReceived != 0) {
            gameComm->mCheckSumReceived = 0;
            gameComm->mCheckSumSent = 0;
        }
    } else {
        SyncMsg_8F20 msg;
        msg.frame = frame;
        msg.value = value;
        msg.type = 4;
        func_00257E98_8F20(((char*)gameComm), 3, &msg, 12);
        gameComm->mCheckSumSent = 1;
    }
    {
        int localTick = gameComm->mAITick[0];
        if (localTick < 0) return;
        int peerTick = gameComm->mAITick[1];
        if (peerTick < 0) return;
        int minTick = peerTick < localTick ? peerTick : localTick;
        if (RingAt_8F20((Ring_8F20*)(((char*)gameComm) + 0x410), minTick) != RingAt_8F20((Ring_8F20*)(((char*)gameComm) + 0x500), minTick)) gameComm->mDesync = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259098);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_9098(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void*, int);
extern "C" void func_0014EBB8(void* iface, int v);
extern "C" void func_0014EBD8(void* iface, int v);
extern "C" void func_002639E0(int a, int b, int c, int d, int e);
extern int D_00534E58[];

struct VEnt_9098 {
    short delta;
    short index;
    void (*fn)(char*);
};

extern "C" void func_00259098(int a0, int a1, int a2)
{
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int v;
    if (lobbyMan->mIsHost) v = lobbyMan->mOppoAddr;
    else v = lobbyMan->mHostAddr;
    void* pi = cBE_getInterface_9098(cBE_getBE(), 1);
    char* ri = (char*)cBE_getInterface_9098(SSXApp->BE, 7);
    D_00534E58[0] = cBENewPlayerInterface_getPlayerCharID(pi, 0);
    func_0014EBB8(ri, ((int)LobbyMan_mLobbyMan->mOpponent));
    func_0014EBD8(ri, ((int)LobbyMan_mLobbyMan->mPersona));
    {
        VEnt_9098* vt = *(VEnt_9098**)(ri + 0xC);
        vt[1].fn(ri + vt[1].delta);
    }
    func_002639E0(a0, v, lobbyMan->mIsHost, a1, a2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259198);
#ifdef SKIP_ASM
extern char str_en[];

extern "C" char *func_00259198(void) {
    return str_en;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591A8);
#ifdef SKIP_ASM
extern char str_US[];

extern "C" char *func_002591A8(void) {
    return str_US;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591B8);
#ifdef SKIP_ASM

extern "C" int func_002591B8(void) {
    return ((int)SSXApp->RealFE) + 0xB5AB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591D0);
#ifdef SKIP_ASM

extern "C" int func_002591D0(void) {
    return ((int)SSXApp->RealFE) + 0xB5ABC;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *operator_new_91E8(unsigned int, void *, unsigned int, int) __asm__("operator_new__FUi");
extern char str_Lobby_b[];

extern "C" void *func_002591E8(unsigned int size) {
    return operator_new_91E8(size, str_Lobby_b, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259210);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv();

extern "C" void func_00259210(int buffer) {
    if (buffer != 0) {
        cMemMan_free__FPv();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259230);
#ifdef SKIP_ASM
extern "C" void *func_0039FAE8(int);

extern "C" void func_00259230(int *event) {
    void *obj = SSXApp->RealFE;
    if (obj != 0) {
        char *topState = (char *)func_0039FAE8(*(int *)((char*)obj + 0xC) + 0x18);
        char *vt = *(char **)(topState + 8);
        void (*fn)(void *, int, int *) = *(void (**)(void *, int, int *))(vt + 0xC4);
        fn(topState + *(short *)(vt + 0xC0), *event, event);
    }
}
#endif
