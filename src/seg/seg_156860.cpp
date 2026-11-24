#include "common.h"
#include "ui/uistate.h"
#include "be/beinterface.h"
#include "online/commchannel.h"
#include "vtable.h"
#include "online/dnasman.h"
#include "online/buddyman.h"
#include "online/lobbyman.h"
#include "be/beintnetwork.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "online/dsockchannel.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "util/locale.h"

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
 * Stores the result in cCommSystem_mCommSystem without checking for an existing one; cCommSystem_destroy destroys
 * it.
 */
extern "C" void cCommSystem_construct(void) {
    cCommSystem_mCommSystem = (cCommSystem*)(cCommSystem_cCommSystem_5860(cMemMan_alloc_5860(52, str_cCommSystem, 0x20000000, 0)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_destroy);
#ifdef SKIP_ASM
extern "C" void cCommSystem__cCommSystem(int, int);

/**
 * Destroys the online connection manager, if there is one.
 *
 * Runs cCommSystem__cCommSystem on cCommSystem_mCommSystem with flags 3 (destroy and free) and clears the pointer.
 * cFEStateMainMenu_onCreateScreen and the debug menu screen call it.
 */
extern "C" void cCommSystem_destroy(void) {
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
extern "C" void ccs_3B18(void *) __asm__("NetConnStartup");
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
 * (NetConnStartup("www.ea.com")).
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
extern "C" void MUTEX_destroy_59B0(void *) __asm__("MUTEX_destroy");
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
extern "C" void cCommSystem__cCommSystem_59B0(void *vself, int flags) {
    cCommSystem* self = (cCommSystem*)vself;
    MUTEX_destroy_59B0(&self->mCommMsgAllocMutex[0]);
    void *blockPool = self->mPool;
    if (blockPool != 0) {
        cMemMan_free__FPv_59B0(blockPool);
    }
    self->mPool = 0;
    self->mFree = 0;
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
INCLUDE_ASM("seg/seg_156860", cCommSystem_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void NetConnIdle(void);
extern "C" int NetConnStatus_5A20(int sel, int a, int b) __asm__("NetConnStatus");
extern "C" int NetConnConnect(void* params, int a);
extern "C" void cCommSystem_netConnFailed(cCommSystem* self, int reason);
extern "C" void cCommSystem_netConnLocalAddr(cCommSystem* self, int* out);
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

/**
 * Runs the online connection manager: brings the network interface up and updates the channels.
 *
 * Calls NetConnIdle, then by the state (+0x20): in 2 (connect requested), once the 'onln' status is 0 it starts the
 * selected configuration (NetConnConnect with the name at +0x80 of its cCommSystem_mNetConns entry) and moves to 3. In
 * 3, once 'onln' and 'host' are set it moves to 4 with an 1800-frame timeout and stores the 'type' status (+0x30) and a
 * 'bbnd' answer of 1 (+0x2C); a 'conn' status starting with '-' fails the connection (cCommSystem_netConnFailed). In 4 it moves to
 * 5 once cCommSystem_netConnLocalAddr reports a local address. In 5 each channel gets its virtual update. The timeout (+0x24) counts
 * down every call and fails the connection with 'time' when it runs out.
 */
extern "C" void cCommSystem_update(cCommSystem* self)
{
    int i;
    NetConnIdle();
    switch (self->mNetConnState) {
    case 0:
    case 1:
    case 6:
        break;
    case 2:
        if (NetConnStatus_5A20(0x6F6E6C6E, 0, 0) == 0) {
            NetConnConnect(cCommSystem_mNetConns + 0x80 + (self->mActiveNetConn << 8), 0);
            self->mNetConnState = 3;
        }
        break;
    case 3: {
        if (NetConnStatus_5A20(0x6F6E6C6E, 0, 0) != 0 && NetConnStatus_5A20(0x686F7374, 0, 0) != 0) {
            self->mNetConnState = 4;
            self->mNetConnTimeOut = 1800;
            self->mInterfaceType = NetConnStatus_5A20(0x74797065, 0, 0);
            int bb = NetConnStatus_5A20(0x62626E64, 0, 0);
            if (bb == 1) {
                self->mBroadBand = bb;
            }
        }
        int conn = NetConnStatus_5A20(0x636F6E6E, 0, 0);
        if ((conn >> 24) == '-') {
            cCommSystem_netConnFailed(self, conn);
            self->mNetConnTimeOut = 0;
        }
        break;
    }
    case 4: {
        int addr;
        cCommSystem_netConnLocalAddr(self, &addr);
        if (addr != 0) {
            self->mNetConnTimeOut = 0;
            self->mNetConnState = 5;
        }
        break;
    }
    case 5:
        for (i = 0; i < 1; i++) {
            sCommObj_5A20* channel = ((sCommObj_5A20*)self->mChannels[i]);
            if (channel != 0) {
                channel->vt[3].fn(vthis(channel, channel->vt[3]));
            }
        }
        break;
    }
    if (self->mNetConnTimeOut > 0) {
        self->mNetConnTimeOut--;
        if (self->mNetConnTimeOut <= 0) {
            cCommSystem_netConnFailed(self, 0x74696D65);
            self->mNetConnTimeOut = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_openDSockChannel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_5BF8(int, char *, int, int) __asm__("cMemMan_alloc");
extern "C" void *cDSockChannel_cDSockChannel_5BF8(void *) __asm__("cDSockChannel_cDSockChannel");
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
            void *obj = cDSockChannel_cDSockChannel_5BF8(cMemMan_alloc_5BF8(0x20, str_cDSockChannel, 0x20000000, 0));
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
INCLUDE_ASM("seg/seg_156860", cCommSystem_closeChannel);
#ifdef SKIP_ASM
/**
 * Closes and deletes one of the connection manager's channels.
 *
 * When the channel is open (its first word nonzero) it calls its virtual disconnect (cDSockChannel_vtbl slot 2), then
 * its virtual destructor with flags 3, and clears the slot. cRealFE_loadStartState and the game-connection code call
 * it.
 *
 * @param commSystem  the connection manager
 * @param channelIndex  the channel slot
 */
extern "C" void cCommSystem_closeChannel(cCommSystem* commSystem, int channelIndex) {
    void **slot = (void **)&commSystem->mChannels[channelIndex];
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
INCLUDE_ASM("seg/seg_156860", cCommSystem_isChannelOpen);
#ifdef SKIP_ASM
/**
 * Tells whether a channel slot of the connection manager holds a channel.
 *
 * @param commSystem  the connection manager
 * @param channelIndex  the channel slot
 * @return 1 when the slot is set, else 0
 */
extern "C" int cCommSystem_isChannelOpen(cCommSystem* commSystem, int channelIndex) {
    return (int)commSystem->mChannels[channelIndex] != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_isChannelConnected);
#ifdef SKIP_ASM
/**
 * Returns whether a channel is connected (the channel's virtual isConnected, slot 4 of cDSockChannel_vtbl).
 *
 * The channel slot is not checked for null. The debug lobby screens and the game-connection update call it.
 *
 * @param commSystem  the connection manager
 * @param channelIndex  the channel slot
 * @return the channel's answer
 */
extern "C" int cCommSystem_isChannelConnected(cCommSystem* commSystem, int channelIndex) {
    cCommChannel* obj = commSystem->mChannels[channelIndex];
    char *vt = (char*)obj->mVtbl;
    return (*(int (**)(void *))(vt + 0x24))(((char*)obj) + *(short *)(vt + 0x20));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_getLatency);
#ifdef SKIP_ASM
/**
 * Returns a channel's latency (the channel's virtual getLatency, slot 5 of cDSockChannel_vtbl).
 *
 * cGameComm_update and cGameComm_getLatency use it.
 *
 * @param commSystem  the connection manager
 * @param channelIndex  the channel slot
 * @return the latency
 */
extern "C" float cCommSystem_getLatency(int commSystem, int channelIndex) {
    char *obj = *(char **)(commSystem + (channelIndex << 2));
    char *vt = *(char **)(obj + 8);
    return (*(float (**)(void *))(vt + 0x2C))(obj + *(short *)(vt + 0x28));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_readPacket);
#ifdef SKIP_ASM
/**
 * Reads a packet from a channel (the channel's virtual readPacket, slot 7 of cDSockChannel_vtbl).
 *
 * cGameComm_updateReadQueue reads the game packets with it.
 *
 * @param commSystem  the connection manager
 * @param channelIndex  the channel slot
 * @param data  the buffer
 * @param maxSize  the buffer size
 * @return the channel's result: nonzero when a packet was read
 */
extern "C" int cCommSystem_readPacket(cCommSystem* commSystem, int channelIndex, int data, int maxSize) {
    cCommChannel* obj = commSystem->mChannels[channelIndex];
    char *vt = (char*)obj->mVtbl;
    return (*(int (**)(void *, int, int))(vt + 0x3C))(((char*)obj) + *(short *)(vt + 0x38), data, maxSize);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_writePacket);
#ifdef SKIP_ASM
/**
 * Writes a packet to a channel (the channel's virtual writePacket, slot 8 of cDSockChannel_vtbl).
 *
 * cGameComm_writePacket, cGameComm_updateWriteQueue and the game-connection code send with it.
 *
 * @param commSystem  the connection manager
 * @param channelIndex  the channel slot
 * @param data  the packet
 * @param size  its size in bytes
 * @return the channel's result: nonzero when the packet was taken
 */
extern "C" int cCommSystem_writePacket(cCommSystem* commSystem, int channelIndex, int data, int size) {
    cCommChannel* obj = commSystem->mChannels[channelIndex];
    char *vt = (char*)obj->mVtbl;
    return (*(int (**)(void *, int, int))(vt + 0x44))(((char*)obj) + *(short *)(vt + 0x40), data, size);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_getError);
#ifdef SKIP_ASM
/**
 * Tells whether the connection or a channel has failed.
 *
 * Returns 1 when the connection error (+0x28, set by cCommSystem_netConnFailed) is set, otherwise the channel's error word (+4).
 *
 * @param commSystem  the connection manager
 * @param channelIndex  the channel slot
 * @return nonzero on an error
 */
extern "C" int cCommSystem_getError(cCommSystem* commSystem, int channelIndex) {
    if (commSystem->mNetConnError != 0) {
        return 1;
    }
    return commSystem->mChannels[channelIndex]->mError;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_allocCommMsgElem);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void *);
extern "C" void MUTEX_unlock(void *);

/**
 * Takes a message element from the connection manager's pool.
 *
 * Pops the head of the free list (+0x18) with the mutex at +4 held and clears the element's link word. There is no
 * check for an empty list. Nothing in the build calls it.
 *
 * @param pool  the connection manager
 * @return the element
 */
extern "C" int *cCommSystem_allocCommMsgElem(cCommSystem* pool) {
    void *mutex = (char*)&pool->mCommMsgAllocMutex;
    MUTEX_lock(mutex);
    int *node = (int*)pool->mFree;
    pool->mFree = (tCommMsgElem*)((int *)*node);
    MUTEX_unlock(mutex);
    *node = 0;
    return node;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_freeCommMsgElem);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void *);
extern "C" void MUTEX_unlock(void *);

/**
 * Returns a message element to the connection manager's pool.
 *
 * With the mutex at +4 held it clears the element's size (+4) and offset (+8) and pushes it onto the free list (+0x18).
 * Nothing in the build calls it.
 *
 * @param pool  the connection manager
 * @param node  the element
 */
extern "C" void cCommSystem_freeCommMsgElem(cCommSystem* pool, tCommMsgElem* node) {
    void *mutex = (char*)&pool->mCommMsgAllocMutex;
    MUTEX_lock(mutex);
    void *head = pool->mFree;
    node->mSize = 0;
    node->unk_00 = (tCommMsgElem*)head;
    node->mOffset = 0;
    pool->mFree = (tCommMsgElem*)node;
    MUTEX_unlock(mutex);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255F20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int NetConnQuery_5F20(int, void *, int) __asm__("NetConnQuery");
extern char cCommSystem_mNetConns[];
extern int cCommSystem_mNumNetConn;

/**
 * Reads the list of network configurations into cCommSystem_mNetConns.
 *
 * Asks for up to 10 entries (NetConnQuery with @p listParam and the 0x100-byte records of cCommSystem_mNetConns) and stores
 * the count in cCommSystem_mNumNetConn. The debug menu screen and func_001B4CD8 call it.
 *
 * @param commSystem  the connection manager (not used)
 * @param listParam  passed to NetConnQuery first
 * @return 1
 */
extern "C" int func_00255F20(void *commSystem, int listParam) {
    cCommSystem_mNumNetConn = NetConnQuery_5F20(listParam, cCommSystem_mNetConns, 10);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_netConnConnect);
#ifdef SKIP_ASM
extern "C" void cCommSystem_netConnDisconnect(cCommSystem*);

/**
 * Starts bringing the network interface up with one of the listed configurations.
 *
 * Disconnects first (cCommSystem_netConnDisconnect), stores the configuration index (+0x1C), sets the interface type (+0x30) to 1, the
 * timeout (+0x24) to 9000 frames, the state (+0x20) to 2 (connect requested, see cCommSystem_update) and clears the
 * broadband flag (+0x2C).
 *
 * @param commSystem  the connection manager
 * @param configIndex  the cCommSystem_mNetConns entry
 */
extern "C" void cCommSystem_netConnConnect(cCommSystem* commSystem, int configIndex) {
    cCommSystem_netConnDisconnect(commSystem);
    commSystem->mActiveNetConn = configIndex;
    commSystem->mInterfaceType = 1;
    commSystem->mNetConnTimeOut = 9000;
    commSystem->mNetConnState = 2;
    commSystem->mBroadBand = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_netConnDisconnect);
#ifdef SKIP_ASM
extern "C" void NetConnDisconnect();

/**
 * Takes the network interface down.
 *
 * When a configuration is active (+0x1C not negative) it clears it to -1 and stops it (NetConnDisconnect). Then it
 * clears the timeout (+0x24) and the error (+0x28) and sets the state (+0x20) to 1, idle.
 *
 * @param commSystem  the connection manager
 */
extern "C" void cCommSystem_netConnDisconnect(cCommSystem* commSystem) {
    if (commSystem->mActiveNetConn >= 0) {
        commSystem->mActiveNetConn = -1;
        NetConnDisconnect();
    }
    commSystem->mNetConnTimeOut = 0;
    commSystem->mNetConnState = 1;
    commSystem->mNetConnError = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_netConnFailed);
#ifdef SKIP_ASM
extern "C" void NetConnDisconnect();

/**
 * Records a failed connection.
 *
 * Stops the active configuration as cCommSystem_netConnDisconnect does, stores @p errorCode (+0x28), sets the state (+0x20) to 6 and
 * clears the timeout. cCommSystem_update calls it with the 'conn' status or 'time'.
 *
 * @param commSystem  the connection manager
 * @param errorCode  the error ('time' or a status starting with '-')
 */
extern "C" void cCommSystem_netConnFailed(cCommSystem* commSystem, int errorCode) {
    if (commSystem->mActiveNetConn >= 0) {
        commSystem->mActiveNetConn = -1;
        NetConnDisconnect();
    }
    commSystem->mNetConnError = errorCode;
    commSystem->mNetConnState = 6;
    commSystem->mNetConnTimeOut = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_netConnLocalAddr);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int NetConnStatus_6038(int, int, int) __asm__("NetConnStatus");

/**
 * Reads the console's local network address.
 *
 * Stores the 'addr' status (NetConnStatus) in @p localAddr when any of its upper 24 bits is set, otherwise 0.
 *
 * @param commSystem  the connection manager (not used)
 * @param localAddr  receives the address, or 0
 */
extern "C" void cCommSystem_netConnLocalAddr(cCommSystem* commSystem, int *localAddr) {
    int temp_2;

    *localAddr = 0;
    temp_2 = NetConnStatus_6038(0x61646472, 0, 0);
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

/**
 * Starts the network configuration program "cdrom0:\NETGUI\NTGUI.ELF;1" (func_0031B008 with 1 and D_004A2EC0).
 *
 * func_001B5BD8 and the debug menu screen call it.
 */
extern "C" void func_00256088(void) {
    func_0031B008_6088(str_cdrom0_NETGUI_NTGUI_ELF_1, 1, &D_004A2EC0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002560B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_60B0(int, char *, int, int) __asm__("cMemMan_alloc");
extern "C" int *DNASMan_DNASMan_60B0(void *) __asm__("DNASMan_DNASMan");
extern "C" void DNASMan__DNASMan_60B0(int *, int) __asm__("DNASMan__DNASMan");
extern "C" void func_00256258_60B0(int *, int) __asm__("func_00256258");
extern "C" void func_002562D0_60B0(int *) __asm__("func_002562D0");
extern "C" void func_002563A8_60B0(int *) __asm__("func_002563A8");
extern "C" void func_002563F8_60B0(int *) __asm__("func_002563F8");
extern "C" void SYNCTASK_run_60B0(int) __asm__("SYNCTASK_run");
extern "C" void THREAD_yieldticks_60B0(int) __asm__("THREAD_yieldticks");
extern char str_DNASMAN[];

static inline int ge3_60B0(int *o) { return *o >= 3; }

/**
 * Runs a DNAS authentication to the end, waiting in a loop (debug menu).
 *
 * Creates a 2640-byte "DNASMAN" object (DNASMan_DNASMan), starts it (func_00256258 with 0) and updates it (func_002563F8,
 * SYNCTASK_run, THREAD_yieldticks(1)) until its state reaches 3, then starts the check (func_002562D0) and updates it
 * until state 5. Then it frees the buffers (func_002563A8) and deletes the object. cDebugMenuScreen_update calls it.
 */
extern "C" void func_002560B0(void) {
    int *dnasMan = DNASMan_DNASMan_60B0(cMemMan_alloc_60B0(2640, str_DNASMAN, 0x20000000, 0));
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
        DNASMan__DNASMan_60B0(dnasMan, 3);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256188);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void FILESYS_addbigsync_6188(void *, int, int, void *) __asm__("FILESYS_addbigsync");
extern char str_data_config_slusovf_big[];
extern int D_004A2ED4;

/**
 * Adds the archive "data/config/slusovf.big" to the file system (FILESYS_addbigsync with 0, 100 and D_004A2ED4).
 *
 * cSSXApp_initload calls it.
 */
extern "C" void func_00256188(void) {
    FILESYS_addbigsync_6188(str_data_config_slusovf_big, 0, 100, &D_004A2ED4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_DNASMan);
#ifdef SKIP_ASM
extern "C" void func_00256228();

/**
 * Constructs a DNAS authentication object (func_00256228 clears it) and returns it.
 *
 * cFEStateAuthDNAS_onCreateScreen and func_002560B0 call it on a 2640-byte block.
 *
 * @param dnasMan  the object
 * @return @p dnasMan
 */
extern "C" int DNASMan_DNASMan(int dnasMan) {
    func_00256228();
    return dnasMan;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan__DNASMan);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00256250_61E0(int *) __asm__("func_00256250");
extern "C" void operator_delete__FPi(int *);

/**
 * Destroys a DNAS authentication object (func_00256250, which does nothing) and frees it when bit 0 of @p flags is set.
 *
 * @param dnasMan  the object
 * @param flags  bit 0: free the memory
 */
extern "C" void DNASMan__DNASMan(int *dnasMan, int flags) {
    func_00256250_61E0(dnasMan);
    if (flags & 1) {
        operator_delete__FPi(dnasMan);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256228);
#ifdef SKIP_ASM
/**
 * Clears a DNAS authentication object: state (+0) and the words +4 to +0x14 become 0, the file handle (+0x18) -1.
 *
 * @param arg0  the object
 */
extern "C" void func_00256228(void *arg0) {
    (*(int *)((char*)(arg0) + 4)) = 0;
    (*(int *)((char*)(arg0) + 8)) = 0;
    (*(int *)((char*)(arg0) + 0xC)) = 0;
    (*(int *)((char*)(arg0) + 0x10)) = 0;
    (*(int *)((char*)(arg0) + 0x14)) = 0;
    (*(int *)((char*)(arg0) + 0x18)) = -1;
    (*(int *)((char*)(arg0) + 0)) = 0;
}
#endif

/**
 * Does nothing: the body of the DNAS authentication object's destructor (DNASMan__DNASMan calls it).
 */
extern "C" void func_00256250(void) {
}

//100%
INCLUDE_ASM("seg/seg_156860", func_00256258);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int ASYNCFILE_loadat(int *, int, int);
extern int D_004803E8[];
extern int D_004A2EE0[];
extern "C" void *operator_new__FUi_6258(unsigned int, int *, int, int) __asm__("operator_new__FUi");

/**
 * Starts a DNAS authentication: begins loading its program file.
 *
 * Stores @p authMode at +0x20, clears +0x14 and +0x1C, sets the state (+0) to 1, allocates a 0x195000-byte "ElfData" buffer
 * (+4) and starts loading "OVERLAY.DAT" into it (ASYNCFILE_loadat; handle at +0x18). func_002563F8 continues.
 *
 * @param authMode  the mode, passed on to the authentication (+0x20)
 */
extern "C" void func_00256258(DNASMan* self, int authMode) {
    self->AuthMode = authMode;
    self->CancelRequest = 0;
    self->AuthError = 0;
    self->State = 1;
    void *mem = operator_new__FUi_6258(0x195000, D_004A2EE0, 0x04000100, 0);
    self->ElfData = (char*)mem;
    self->FileHandle = ASYNCFILE_loadat(D_004803E8, (int)mem, 0x195000);
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
extern "C" char *NetConnMAC();
extern "C" void func_003FD0D8(void *);
extern "C" char *strcpy(char *, const char *);

/**
 * Starts the DNAS check itself once the program and data are loaded.
 *
 * Clears +0x1C and the 160-byte parameter block at +0x24, then fills it: the data buffer (+0xC) and size (+0x10), the
 * key D_004A2ED8, 8, the text from NetConnMAC, "SLUS_20772", the mode (+0x20) and 1. Sets the state (+0) to 4 and
 * starts the module (func_003FD0D8, the DirtyDnasRelInit entry). func_002563F8 continues.
 */
extern "C" void func_002562D0(int *self)
{
    self[7] = 0;
    MEM_fill_62D0(self + 9, 0, 160);
    self[27] = self[3];
    self[0x1C] = self[4];
    self[36] = 0;
    self[35] = 0;
    *(sKey_62D0 *)(self + 0x1D) = D_004A2ED8;
    self[33] = 8;
    strcpy((char *)(self + 0x26), NetConnMAC());
    strcpy((char *)(self + 0x2A), "SLUS_20772");
    self[46] = 0;
    self[47] = self[8];
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

/**
 * Frees a DNAS authentication object's buffers: the data (+0xC) and the program (+4), clearing each pointer.
 *
 * @param dnasMan  the object
 */
extern "C" void func_002563A8(void *dnasMan) {
    int temp_4;
    int temp_4_2;

    temp_4 = (*(int *)((char*)(dnasMan) + 0xC));
    if (temp_4 != 0) {
        cMemMan_free__FPv_63A8(temp_4);
        (*(int *)((char*)(dnasMan) + 0xC)) = 0;
    }
    temp_4_2 = (*(int *)((char*)(dnasMan) + 4));
    if (temp_4_2 != 0) {
        cMemMan_free__FPv_63A8(temp_4_2);
        (*(int *)((char*)(dnasMan) + 4)) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002563F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00256460(DNASMan* self);
extern "C" void func_00256510(DNASMan* self);
extern "C" void func_002565E0(DNASMan* self);

/**
 * Advances a DNAS authentication by its state (+0).
 *
 * State 1 runs func_00256460, 2 runs func_00256510, 4 runs func_002565E0; states 0, 3 and 5 wait.
 */
extern "C" void func_002563F8(DNASMan* self)
{
    switch (*(int*)self) {
    case 1:
        func_00256460((DNASMan*)self);
        break;
    case 2:
        func_00256510((DNASMan*)self);
        break;
    case 4:
        func_002565E0((DNASMan*)self);
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
extern "C" int ASYNCFILE_loadat_6460(char *, void *, int) __asm__("ASYNCFILE_loadat");
extern char str_AuthData[];
extern char str_CONFIG_DAT[];

/**
 * DNAS state 1: once "OVERLAY.DAT" has loaded, links it and starts loading "CONFIG.DAT".
 *
 * When the load (+0x18) is done it releases it with the size in +8 and, for a nonzero size, links the program
 * (func_003FCFD8, which resolves DirtyDnasRelInit / Updt / Exit). On success it allocates a 64 KB "AuthData" buffer
 * (+0xC), starts loading "CONFIG.DAT" into it and sets the state to 2.
 */
extern "C" void func_00256460(DNASMan* self) {
    if (ASYNCFILE_getstatus_6460(self->FileHandle) == 1) {
        self->ElfSize = 0;
        ASYNCFILE_release_6460(self->FileHandle, 0, &self->ElfSize);
        int readBuffer = (int)self->ElfData;
        if (readBuffer != 0) {
            int readSize = self->ElfSize;
            if (readSize > 0 && func_003FCFD8_6460(readBuffer, readSize) >= 0) {
                void *authData = operator_new__FUi_6460(0x10000, str_AuthData, 0x04000100, 0);
                self->AuthData = (char*)authData;
                self->FileHandle = ASYNCFILE_loadat_6460(str_CONFIG_DAT, authData, 0x10000);
                self->State = 2;
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

/**
 * DNAS state 2: once "CONFIG.DAT" has loaded, decodes it.
 *
 * When the load (+0x18) is done it releases it with the size in +0x10; for a nonzero size it sets the state to 3 and
 * XORs every byte of the data (+0xC) with the 4-byte key D_004A2EE8, repeating.
 */
extern "C" void func_00256510(DNASMan* self) {
    if (ASYNCFILE_getstatus_6510(self->FileHandle) == 1) {
        self->AuthSize = 0;
        ASYNCFILE_release_6510(self->FileHandle, 0, &self->AuthSize);
        unsigned char *fileData = (unsigned char*)self->AuthData;
        if (fileData != 0) {
            int n = self->AuthSize;
            if (n > 0) {
                self->State = 3;
                K6510 key = D_004A2EE8[0];
                int i;
                unsigned char *bytePtr;
                for (i = 0, bytePtr = fileData; i < self->AuthSize; i++, bytePtr++) {
                    *bytePtr ^= key.b[i % 4];
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

/**
 * DNAS state 4: once the check is finished, collects its result.
 *
 * When func_003FD100 (the module's update) returns 0 it clears the 908-byte result block at +0xC4, gets the result
 * (func_003FD128), copies three wide strings of it (+0xCC, +0x10C, +0x20C) to ErrorText1, ErrorText2 and ErrorText3
 * (USTR_copy), stores whether the result was 0 in AuthError and sets the state to 5.
 */
extern "C" void func_002565E0(DNASMan* self) {
    if (func_003FD100_65E0(self->CancelRequest) == 0) {
        char *infoBuf = ((char*)self) + 0xC4;
        MEM_fill_65E0(infoBuf, 0, 908);
        char *secondText = (char*)&self->ErrorText2[0];
        char *thirdText = (char*)&self->ErrorText3[0];
        int result = func_003FD128_65E0(infoBuf);
        *(short*)&self->ErrorText1[0] = 0;
        *(short *)secondText = 0;
        *(short *)thirdText = 0;
        USTR_copy_65E0((char*)&self->ErrorText1[0], ((char*)self) + 0xCC);
        USTR_copy_65E0(secondText, ((char*)self) + 0x10C);
        USTR_copy_65E0(thirdText, ((char*)self) + 0x20C);
        if (result == 0) {
            self->AuthError = 1;
        } else {
            self->AuthError = 0;
        }
        self->State = 5;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_cDSockChannel);
#ifdef SKIP_ASM
void cCommChannel_cCommChannel(void *);
extern char cDSockChannel_vtbl[];

/**
 * Constructs a DirtySock channel.
 *
 * Runs the cCommChannel constructor, stores cDSockChannel_vtbl (+8), sets the float at +0x10 to 0.1 and clears the
 * state (+0xC) and the words +0x14, +0x18 and +0x1C.
 *
 * @param channel  the channel
 * @return @p channel
 */
extern "C" void *cDSockChannel_cDSockChannel(cDSockChannel* channel) {
    cCommChannel_cCommChannel(channel);
    channel->mVtbl = (void**)cDSockChannel_vtbl;
    channel->mLatency = 0.10000000149011612f;
    channel->mState = 0;
    channel->mGameUtil = 0;
    channel->mGameLink = 0;
    channel->mPacketError = 0;
    return ((void*)channel);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel__cDSockChannel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cCommChannel__cCommChannel_66E8(void *, int) __asm__("cCommChannel__cCommChannel");
extern "C" void cDSockChannel_disconnect(cDSockChannel*);
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
    dSockChannel->mVtbl = (void**)cDSockChannel_vtbl;
    cDSockChannel_disconnect(dSockChannel);
    cCommChannel__cCommChannel_66E8(dSockChannel, flags);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_readPacket);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy_6730(char *, char *, int) __asm__("MEM_copy");
extern "C" int NetGameLinkRecv_6730(int, char *, int) __asm__("NetGameLinkRecv");

/**
 * Receives the next game packet from the channel's NetGameLink and copies its payload out.
 *
 * Only while connected (mState 2). It pulls packets one at a time (NetGameLinkRecv, probably NetGameLinkRecv):
 * a kind-5 packet (byte 2) whose length (byte 3) is nonzero and fits is copied, other kind-5 packets are
 * dropped, and any other kind sets mPacketError (state 4 on the next updateConnected).
 *
 * @param dst     buffer for the payload
 * @param maxlen  size of @p dst in bytes
 * @return the payload length (1-255), or 0 when not connected, when nothing usable arrived or on a bad packet
 */
extern "C" unsigned char cDSockChannel_readPacket(cDSockChannel* self, char *dst, int maxlen) {
    unsigned char buf[264];
    if (self->mState == 2) {
        while (NetGameLinkRecv_6730((int)self->mGameLink, (char *)buf, 1) > 0) {
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
    char buf[264];
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
extern "C" void cCommChannel_channelOpen__FPv_6860(void *) __asm__("cCommChannel_channelOpen__FPv");
extern "C" int NetGameUtilCreate_6860() __asm__("NetGameUtilCreate");
extern "C" int NetGameUtilConnect_6860(int, int, void *) __asm__("NetGameUtilConnect");
extern "C" int sprintf_6860(char *, const char *, ...) __asm__("sprintf");
extern char str_pct_s_pct_d_pct_d[];

/**
 * Starts listening for the peer's game connection on "addr:port:port".
 *
 * Disconnects first (virtual slot 2), opens the channel (cCommChannel_channelOpen, channelOpen), creates a NetGameUtil
 * (NetGameUtilCreate) into mGameUtil and, if that worked, starts it in mode 0x101 (On Tour's listen) on the
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
    cCommChannel_channelOpen__FPv_6860(((char*)self));
    int gameUtil = NetGameUtilCreate_6860();
    self->mGameUtil = (NetGameUtilRefT*)gameUtil;
    if (gameUtil != 0) {
        sprintf_6860(buf, str_pct_s_pct_d_pct_d, addr, n, n);
        NetGameUtilConnect_6860((int)self->mGameUtil, 257, buf);
        self->mState = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_connect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cCommChannel_channelOpen__FPv_68F8(void *) __asm__("cCommChannel_channelOpen__FPv");
extern "C" int NetGameUtilCreate_68F8() __asm__("NetGameUtilCreate");
extern "C" int NetGameUtilConnect_68F8(int, int, void *) __asm__("NetGameUtilConnect");
extern "C" int sprintf_68F8(char *, const char *, ...) __asm__("sprintf");
extern char str_pct_s_pct_d_pct_d[];

/**
 * Starts connecting to the peer's game link at "addr:port:port".
 *
 * Clears mError, disconnects (virtual slot 2), opens the channel (cCommChannel_channelOpen, channelOpen), creates a
 * NetGameUtil (NetGameUtilCreate) into mGameUtil and, if that worked, starts it in mode 0x102 (On Tour's connect)
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
    cCommChannel_channelOpen__FPv_68F8(((char*)self));
    int gameUtil = NetGameUtilCreate_68F8();
    self->mGameUtil = (NetGameUtilRefT*)gameUtil;
    if (gameUtil != 0) {
        sprintf_68F8(buf, str_pct_s_pct_d_pct_d, addr, n, n);
        NetGameUtilConnect_68F8((int)self->mGameUtil, 258, buf);
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
extern "C" void cDSockChannel_disconnect(cDSockChannel* channel) {
    int temp_4;
    int temp_4_2;

    if (channel->mState != 0) {
        cCommChannel_channelClose();
        temp_4 = ((int)channel->mGameLink);
        if (temp_4 != 0) {
            NetGameLinkDestroy(temp_4);
            channel->mGameLink = 0;
        }
        temp_4_2 = ((int)channel->mGameUtil);
        if (temp_4_2 != 0) {
            NetGameUtilDestroy(temp_4_2);
            channel->mGameUtil = 0;
        }
        channel->mState = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cDSockChannel_updateConnecting(cDSockChannel* self);
extern "C" void cDSockChannel_updateConnected(cDSockChannel* self);

/**
 * Advances the channel's connection state machine.
 *
 * State 1 (connecting) runs cDSockChannel_updateConnecting (updateConnecting) and state 2 (connected) runs
 * cDSockChannel_updateConnected(); the closed, timed-out and packet-error states (0, 3, 4) do nothing.
 */
extern "C" void cDSockChannel_update(cDSockChannel* self)
{
    switch (self->mState) {
    case 1:
        cDSockChannel_updateConnecting((cDSockChannel*)(((char*)self)));
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
INCLUDE_ASM("seg/seg_156860", cDSockChannel_updateConnecting);
#ifdef SKIP_ASM
extern "C" int NetGameLinkCreate(int, int, int);
extern "C" int NetGameUtilComplete(int);

/**
 * Checks whether a DirtySock channel's pending connection has completed.
 *
 * When NetGameUtilComplete (on the utility object at +0x18) returns a connection, it creates a game link on it (NetGameLinkCreate
 * with 0 and 9084) at +0x1C and, when that worked, sets the state (+0xC) to 2.
 *
 * @param channel  the channel
 */
extern "C" void cDSockChannel_updateConnecting(cDSockChannel* channel) {
    int temp_2;
    int temp_2_2;

    temp_2 = NetGameUtilComplete(((int)channel->mGameUtil));
    if (temp_2 != 0) {
        temp_2_2 = NetGameLinkCreate(temp_2, 0, 9084);
        channel->mGameLink = (NetGameLinkRefT*)temp_2_2;
        if (temp_2_2 != 0) {
            channel->mState = 2;
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

extern "C" char* NetGameLinkStatus(int id);

/**
 * Checks a connected channel's link health and smooths its latency.
 *
 * Reads the NetGameLink status (NetGameLinkStatus): a gap over 30000 between its first word and +0x20 (probably
 * ms since the last receive) sets state 3, a zero byte at +0x45 (link lost) disconnects; both set mError.
 * Otherwise mLatency = (4 * mLatency + ping) / 5, the ping (+0xC, ms) in seconds capped at 1, and a pending
 * mPacketError then sets state 4 and mError.
 *
 * @param dSockChannel  the channel (this)
 */
extern "C" void cDSockChannel_updateConnected(cDSockChannel* dSockChannel)
{
    char* st = NetGameLinkStatus((int)dSockChannel->mGameLink);
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
INCLUDE_ASM("seg/seg_156860", RpcFree);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv();

/**
 * Frees a buffer from RpcAlloc (cMemMan_free) unless it is null.
 *
 * The DirtySock code (NetGameLinkDestroy, func_003E73E0, func_003F55B8 and others) frees its blocks with it. The C
 * declares cMemMan_free without arguments; the buffer passes through in its register.
 *
 * @param buffer  the buffer, or 0
 */
extern "C" void RpcFree(int buffer) {
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
 * Stores the result in cGameComm_mGameComm; cGameComm_destroy destroys it.
 *
 * @param channel  the cCommSystem channel index the game link uses (stored in mChannel)
 * @param isServer  nonzero to run the server side of the handshake (mServer)
 */
extern "C" void cGameComm_construct(int channel, int isServer) {
    cGameComm_mGameComm = (cGameComm*)(cGameComm_cGameComm(cMemMan_alloc(1520, D_004804B8, 0x20000000, 0), channel, isServer));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_destroy);
#ifdef SKIP_ASM
extern "C" void cGameComm__cGameComm(int, int);

/**
 * Destroys the online game link, if there is one.
 *
 * Runs cGameComm__cGameComm on cGameComm_mGameComm with flags 3 (destroy and free) and clears the pointer. The front-end
 * online screens and the lobby code call it.
 */
extern "C" void cGameComm_destroy(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm__cGameComm((int)cGameComm_mGameComm, 3);
        cGameComm_mGameComm = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_cGameComm);
#ifdef SKIP_ASM
cBEInterface* cBE_getInterface_6C78(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern sGcGlobals_6C78* SSXApp_6C78 __asm__("SSXApp");

/**
 * Constructs the online game link: the handshake state, timers, counters and empty packet queues.
 *
 * Stores the cCommSystem channel index in mChannel and @p isServer in mServer, sets mState to kGameComm_Init through
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
    self->bufSize = cBENetworkInterface_getSyncDataSize(cBE_getInterface_6C78(SSXApp_6C78->be, 7));
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
    self->unk_114 = SSXApp_6C78->unk_14;
    self->unk_118 = SSXApp_6C78->unk_14;
    self->unk_11C = SSXApp_6C78->unk_14;
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
    self->unk_34 = SSXApp_6C78->unk_18 / SSXApp_6C78->unk_10;
    self->unk_38 = 0;
    self->unk_3C = 0;
    self->unk_138 = 0;
    self->unk_140 = 0;
    self->unk_13C = 0;
    for (int i = 0; i < 4; i++) {
        sGcQueue_6C78* queue = &self->q[i];
        queue->wr = 0;
        queue->rd = 0;
        MEM_fill_6C78(queue->slots, 0, sizeof(queue->slots));
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
INCLUDE_ASM("seg/seg_156860", cGameComm__cGameComm);
#ifdef SKIP_ASM
extern "C" void cMemMan_free_6EE0(void *) __asm__("cMemMan_free__FPv");
extern "C" void operator_delete__FPi(int *);
extern "C" void cGameComm_freePacket(cGameComm* arg0, int *arg1);
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

/**
 * Destroys an online game link.
 *
 * Frees the buffer at +0x3EC, the packet being received (+0x134) and every packet left in the four queues at +0x144
 * (cGameComm_freePacket), destroys the ping object (+0x140, func_003E73E0) and frees the object when bit 0 of @p flags is set.
 *
 * @param flags  bit 0: free the memory
 */
// PORT: the unit declares this as (int, int)
extern "C" void cGameComm__cGameComm(int self, int flags)
{
    cGameComm* gameComm = (cGameComm*)self;
    int i;
    if (gameComm->mBEBuffer) cMemMan_free_6EE0(gameComm->mBEBuffer);
    if ((int*)gameComm->mPacketCache) cGameComm_freePacket((cGameComm*)gameComm, (int*)gameComm->mPacketCache);
    for (i = 0; i < 4; i++) {
        while (Peek_6EE0((PacketQueue_6EE0*)(&gameComm->mReadQueue[0]) + i)) {
            cGameComm_freePacket((cGameComm*)gameComm, Pop_6EE0((PacketQueue_6EE0*)(&gameComm->mReadQueue[0]) + i));
        }
    }
    if (gameComm->unk_140) {
        func_003E73E0(gameComm->unk_140);
        gameComm->unk_140 = 0;
    }
    if (flags & 1) operator_delete__FPi((int*)gameComm);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_setState);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cGameComm_calcLatency(cGameComm* arg0);
extern "C" void func_00258870(cGameComm*, int, int);
extern "C" void func_0031AD18(void);
/**
 * Switches the handshake and race state machine (mState) to a new tGameComm state.
 *
 * Entering GameActive (14) computes the latency (cGameComm_calcLatency), arms a 3600-frame link timeout and clears
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
        cGameComm_calcLatency(self);
        func_00258870(self, 3600, 0);
        func_0031AD18();
        self->mState = mode;
        return;
    case 15:
        self->mWaitingMessage = 0;
        self->mPauseMenu[1] = 0;
        break;
    case 1: case 3: case 5: case 7: case 9: case 11: case 13:
        func_00258870(self, 1800, 0);
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENetworkInterface_getSyncData_70D8(void* iface, int a1, int a2) __asm__("cBENetworkInterface_getSyncData");
extern "C" void cBENetworkInterface_setSyncData_70D8(void* iface, int a1, int a2) __asm__("cBENetworkInterface_setSyncData");
extern "C" float cCommSystem_getLatency_70D8(int arg0, int arg1) __asm__("cCommSystem_getLatency");
extern "C" void cGameComm_setState_70D8(void* self, int mode) __asm__("cGameComm_setState");
extern "C" void cGameComm_updateTimeout_70D8(char* p) __asm__("cGameComm_updateTimeout");
extern "C" int cGameComm_checkErrors_70D8(char* s) __asm__("cGameComm_checkErrors");
extern "C" void cGameComm_checkActive_70D8(char* s) __asm__("cGameComm_checkActive");
extern "C" void cGameComm_updateReadQueue_70D8(char* s) __asm__("cGameComm_updateReadQueue");
extern "C" void cGameComm_updateWriteQueue_70D8(char* s) __asm__("cGameComm_updateWriteQueue");
extern "C" void cGameComm_updateCmdQueue_70D8(char* s) __asm__("cGameComm_updateCmdQueue");
extern "C" int cGameComm_asyncReadComplete_70D8(char* s) __asm__("cGameComm_asyncReadComplete");
extern "C" void cGameComm_asyncRead_70D8(char* s, int a1, int a2) __asm__("cGameComm_asyncRead");
extern "C" void cGameComm_asyncReadUpdate_70D8(char* s) __asm__("cGameComm_asyncReadUpdate");
extern "C" int cGameComm_asyncWriteComplete_70D8(char* s) __asm__("cGameComm_asyncWriteComplete");
extern "C" void cGameComm_asyncWrite_70D8(char* s, int a1, int a2) __asm__("cGameComm_asyncWrite");
extern "C" void cGameComm_asyncWriteUpdate_70D8(char* s) __asm__("cGameComm_asyncWriteUpdate");
extern "C" void cGameComm_updateMessageDisplay_70D8(void* s) __asm__("cGameComm_updateMessageDisplay");
extern "C" int func_003E7450_70D8(int sock, int a1, char* tag, int n) __asm__("func_003E7450");
extern "C" int func_003E7500_70D8(int sock, char* buf, int* len, int* info) __asm__("func_003E7500");
extern "C" int strncmp_70D8(const char* a, const char* b, int n) __asm__("strncmp");
extern char str_SSXPingServer[];

/**
 * Runs one frame of the online game link: ping replies, timers, the packet queues and the pre-race handshake.
 *
 * While the ping server is active (unk_138 > 0) it counts it down, reads replies from unk_140 (func_003E7500) and stops
 * when one matches a sent ping id (unk_0C4), else sends up to 4 "SSXPingServer" pings (func_003E7450), one every 5
 * frames. Then mStallTimer drops by 0.15 (not below 0), mUnPauseTimer counts down, and it runs
 * cGameComm_updateMessageDisplay, cGameComm_updateTimeout (timeouts) and cGameComm_checkErrors (checkErrors; an error returns),
 * cGameComm_updateReadQueue (updateReadQueue), cGameComm_updateWriteQueue, cGameComm_asyncReadUpdate / cGameComm_asyncWriteUpdate (async read / write
 * updates) and cGameComm_updateCmdQueue (updateCmdQueue), and checks errors again.
 *
 * Last comes the handshake on mState: the server sends its settings (mServerSettings, 0x2C bytes), receives the
 * client's (+0x100, 0x14 bytes), sends then receives the BE sync data (mBEBuffer, getSyncData/setSyncData on BE
 * interface 7) and exchanges latencies (mNetLatency, measured with cCommSystem_getLatency), moving through states 1..13 to
 * kGameComm_GameActive (14), where cGameComm_checkActive runs each frame. The client walks the same states with every send and
 * receive swapped. Sends use cGameComm_asyncWrite / cGameComm_asyncWriteComplete (asyncWrite / complete), receives cGameComm_asyncRead /
 * cGameComm_asyncReadComplete.
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
            if (strncmp_70D8(msg, str_SSXPingServer, 13) != 0) {
                continue;
            }
            for (int i = 0; i < vs->unk_0B8; i++) {
                if (vs->unk_0C4[i] == info[2]) {
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
                vs->unk_0C4[k] = sock;
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
    cGameComm_updateTimeout_70D8(((char*)vs));
    if (cGameComm_checkErrors_70D8(((char*)vs)) != 0) {
        return;
    }
    cGameComm_updateReadQueue_70D8(((char*)vs));
    cGameComm_updateWriteQueue_70D8(((char*)vs));
    cGameComm_asyncReadUpdate_70D8(((char*)vs));
    cGameComm_asyncWriteUpdate_70D8(((char*)vs));
    cGameComm_updateCmdQueue_70D8(((char*)vs));
    if (cGameComm_checkErrors_70D8(((char*)vs)) != 0) {
        return;
    }
    void* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    if (vs->mServer != 0) {
        switch ((unsigned int)vs->mState) {
        case 0:
            cGameComm_setState_70D8(((char*)vs), 1);
            break;
        case 1:
            cGameComm_asyncWrite_70D8(((char*)vs), (int)(&vs->mServerSettings), 44);
            cGameComm_setState_70D8(((char*)vs), 2);
            break;
        case 2:
            if (cGameComm_asyncWriteComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 3);
            }
            break;
        case 3:
            cGameComm_asyncRead_70D8(((char*)vs), (int)(&vs->unk_100), 20);
            cGameComm_setState_70D8(((char*)vs), 4);
            break;
        case 4:
            if (cGameComm_asyncReadComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 5);
            }
            break;
        case 5:
            cBENetworkInterface_getSyncData_70D8(iface, 0, (int)vs->mBEBuffer);
            cGameComm_asyncWrite_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 6);
            break;
        case 6:
            if (cGameComm_asyncWriteComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 7);
            }
            break;
        case 7:
            cGameComm_asyncRead_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 8);
            break;
        case 8:
            if (cGameComm_asyncReadComplete_70D8(((char*)vs)) != 0) {
                cBENetworkInterface_setSyncData_70D8(iface, 1, (int)vs->mBEBuffer);
                cGameComm_setState_70D8(((char*)vs), 9);
            }
            break;
        case 9:
            vs->mNetLatency[0] = cCommSystem_getLatency_70D8((int)cCommSystem_mCommSystem, vs->mChannel);
            cGameComm_asyncWrite_70D8(((char*)vs), (int)(&vs->mNetLatency[0]), 4);
            cGameComm_setState_70D8(((char*)vs), 10);
            break;
        case 10:
            if (cGameComm_asyncWriteComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 11);
            }
            break;
        case 11:
            cGameComm_asyncRead_70D8(((char*)vs), (int)(&vs->mNetLatency[1]), 4);
            cGameComm_setState_70D8(((char*)vs), 12);
            break;
        case 12:
            if (cGameComm_asyncReadComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 13);
            }
            break;
        case 13:
            cGameComm_setState_70D8(((char*)vs), 14);
            break;
        case 14:
            cGameComm_checkActive_70D8(((char*)vs));
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
            cGameComm_asyncRead_70D8(((char*)vs), (int)(&vs->mServerSettings), 44);
            cGameComm_setState_70D8(((char*)vs), 4);
            break;
        case 4:
            if (cGameComm_asyncReadComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 1);
            }
            break;
        case 1:
            cGameComm_asyncWrite_70D8(((char*)vs), (int)(&vs->unk_100), 20);
            cGameComm_setState_70D8(((char*)vs), 2);
            break;
        case 2:
            if (cGameComm_asyncWriteComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 7);
            }
            break;
        case 7:
            cGameComm_asyncRead_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 8);
            break;
        case 8:
            if (cGameComm_asyncReadComplete_70D8(((char*)vs)) != 0) {
                cBENetworkInterface_setSyncData_70D8(iface, 0, (int)vs->mBEBuffer);
                cGameComm_setState_70D8(((char*)vs), 5);
            }
            break;
        case 5:
            cBENetworkInterface_getSyncData_70D8(iface, 1, (int)vs->mBEBuffer);
            cGameComm_asyncWrite_70D8(((char*)vs), (int)vs->mBEBuffer, vs->mBEBufferSize);
            cGameComm_setState_70D8(((char*)vs), 6);
            break;
        case 6:
            if (cGameComm_asyncWriteComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 11);
            }
            break;
        case 11:
            cGameComm_asyncRead_70D8(((char*)vs), (int)(&vs->mNetLatency[0]), 4);
            cGameComm_setState_70D8(((char*)vs), 12);
            break;
        case 12:
            if (cGameComm_asyncReadComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 9);
            }
            break;
        case 9:
            vs->mNetLatency[1] = cCommSystem_getLatency_70D8((int)cCommSystem_mCommSystem, vs->mChannel);
            cGameComm_asyncWrite_70D8(((char*)vs), (int)(&vs->mNetLatency[1]), 4);
            cGameComm_setState_70D8(((char*)vs), 10);
            break;
        case 10:
            if (cGameComm_asyncWriteComplete_70D8(((char*)vs)) != 0) {
                cGameComm_setState_70D8(((char*)vs), 13);
            }
            break;
        case 13:
            cGameComm_setState_70D8(((char*)vs), 14);
            break;
        case 14:
            cGameComm_checkActive_70D8(((char*)vs));
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
    if (((force != 0) || ((gameComm->mError == 0) && (gameComm->mState < 0xF))) && (gameComm->unk_038 == 0)) {
        gameComm->unk_038 = (int) ((int) (SSXApp->TimerTick) / (int) (SSXApp->TimerHz));
    }
    gameComm->mConnected = 0;
    gameComm->mRaceInProgress = 0;
    gameComm->mPauseMenu[1] = 0;
    gameComm->mLocalDisconnected = 1;
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
        int *significant = (int *)(&self->mCommErrorWasSignificant);
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
INCLUDE_ASM("seg/seg_156860", cGameComm_updateTimeout);
#ifdef SKIP_ASM
/**
 * Counts down the game link's overlay and pause timeouts.
 *
 * Only without an error display, while not alone, and with no error or no race in progress: mOverlayTimeout counts down
 * when positive unless both players are in the pause menu, and mPauseTimeout counts down when positive.
 * cGameComm_update calls it.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_updateTimeout(cGameComm* gameComm) {
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
INCLUDE_ASM("seg/seg_156860", cGameComm_checkErrors);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cCommSystem_getError_77C8(int sys, int port) __asm__("cCommSystem_getError");
extern "C" void cGameComm_setCommError(cGameComm* self);
extern "C" void cGameComm_displayError(cGameComm* self);
extern "C" void cGameComm_updateDisplayError(cGameComm* self);

/**
 * Checks the game link for errors and starts the error display; returns whether there is one.
 *
 * In order: a running error display is updated (cGameComm_updateDisplayError); an existing error returns 1; a remote
 * end of game, a channel error (cCommSystem_getError, also cGameComm_setCommError), 3 or more of the local or remote counts
 * +0x54 / +0x58 (mCheating 3), 6 or more pauses (mCheating 2), mStallTimer reaching 3600 or the link timeout +0x94
 * running out (+0x2C set, cGameComm_setCommError) each show the error (cGameComm_displayError) and return 1.
 *
 * @param gameComm  the game link
 * @return 1 when there is an error, else 0
 */
extern "C" int cGameComm_checkErrors(cGameComm* gameComm)
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
    if (cCommSystem_getError_77C8((int)cCommSystem_mCommSystem, gameComm->mChannel) != 0) {
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
    if (gameComm->mLinkTimeout <= 0) return 0;
    if (--gameComm->mLinkTimeout > 0) return 0;
    gameComm->mLinkTimedOut = 1;
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
extern int VoipMan;

/**
 * Ends the online session on an error: stops input syncing and brings up the online-error overlay.
 *
 * Always clears the waiting message, the peer's pause and the unpause flag and timer. On the first error
 * only, it sets mCheating[0] to 4 if this machine's controller is unplugged mid-race with no other error,
 * closes the voice link (func_00266DF8 on VoipMan, probably cVoipMan), lets the connection popup close,
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
        if (VoipMan != 0) {
            func_00266DF8_78F8(VoipMan);
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
extern int cPauseState_StackIdx;
extern int D_00442924[];
extern int cPauseState_Stack[];

/**
 * Counts down a pending error display and then raises the online-error overlay.
 *
 * Waits while mLoading is set and until the UI stack's current state is empty or of kind 5. When
 * mDisplayError reaches 0 it triggers overlay 0x27 (cOVState_PAUSE_ONLINE_ERROR) and pushes 13 onto the stack
 * at cPauseState_Stack (cPauseState_Flags = cPauseState_FlagsTable[13]), which the overlays pop.
 */
extern "C" void cGameComm_updateDisplayError(cGameComm* self) {
    if (self->mLoading == 0 && self->mDisplayError > 0) {
        char *uiEngine = (char*)SSXApp->Game->UI;
        char *st;
        if (uiEngine != 0 && (uiEngine += 0x18) != 0 && ((st = cUIStateStack_getCurrentState_79C8(uiEngine), st == 0) || (st[0x1D] & 0x3F) == 5)) {
            int n = self->mDisplayError - 1;
            self->mDisplayError = n;
            if (n <= 0) {
                cOVStateManager_triggerOverlay_79C8(39);
                int idx = cPauseState_StackIdx + 1;
                cPauseState_Stack[idx] = 13;
                cPauseState_Flags = D_00442924[0];
                cPauseState_StackIdx = idx;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_checkActive);
#ifdef SKIP_ASM
extern "C" void cGameComm_setCommError(cGameComm* self);
extern "C" void cGameComm_displayError(cGameComm* self);
extern int D_004A3ED0;

/**
 * Watches the game link for a stalled or silent peer.
 *
 * Without mCheating[0] and with the local player not in the pause menu: once the start gate is up and the peer is not
 * paused or unpausing, it counts mStallThreshold and +0x60; in AI states 4 and 5 mStallTimer grows once mStallThreshold
 * reaches 10 (mStallCount counts each time it reaches 10). At 60 for +0x60 mWaitingMessage is set. During a race a
 * desync, or 1800 frames of +0x5C (counted while the peer is not paused), raise the comm error and show it. Last, when
 * D_004A3ED0 is set, mCheating[0] becomes 1 and the error is shown.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_checkActive(cGameComm* gameComm)
{
    if (gameComm->mCheating[0] != 0) return;
    if (gameComm->mPauseMenu[0] == 0) {
        if (gameComm->mGameStartGate != 0) {
            if (gameComm->mPauseMenu[1] == 0 && gameComm->mUnPauseTimer <= 0) {
                cSSXApp* appObj = SSXApp;
                int n = ++gameComm->mStallThreshold;
                ++gameComm->mWaitingTimer;
                unsigned int st = *(int*)appObj->Game->AI - 4;
                if (st < 2) {
                    if (n >= 10) gameComm->mStallTimer += 1.0f;
                    if (gameComm->mStallThreshold == 10) ++gameComm->mStallCount;
                }
            }
            if (gameComm->mWaitingTimer == 60) gameComm->mWaitingMessage = 1;
        }
        if (gameComm->mRaceInProgress != 0) {
            if (gameComm->mDesync != 0) {
                cGameComm_setCommError(gameComm);
                cGameComm_displayError(gameComm);
                return;
            }
            if (gameComm->mPauseMenu[1] == 0) ++gameComm->mSyncInputDelay;
            if (gameComm->mSyncInputDelay >= 1800) {
                gameComm->mLinkTimedOut = 1;
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


extern "C" void cGameComm_updateReadQueue(cGameComm* c);
extern "C" void cGameComm_freePacket(cGameComm* arg0, int *arg1);
extern "C" void func_00258870(cGameComm*, int, int);
extern "C" void MEM_copy_7BD8(void* dst, const void* src, int n) __asm__("MEM_copy");

/**
 * Pops the oldest received packet of one type and copies its payload out.
 *
 * Returns 0 after a link error. Otherwise it first queues newly received packets (cGameComm_updateReadQueue,
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
    cGameComm_updateReadQueue(gameComm);
    PacketQueue_7BD8* queue = &((PacketQueue_7BD8*)&gameComm->mReadQueue[0])[ch];
    if (queue->rd != queue->wr) {
        Packet_7BD8* packet = queue->slots[queue->rd++ % 32];
        int len = packet->size - 4;
        MEM_copy_7BD8((void*)dst, packet->data, len);
        cGameComm_freePacket((cGameComm*)gameComm, (int*)packet);
        if (ch == 0) {
            gameComm->mSyncInputDelay = 0;
            gameComm->mWaitingTimer = 0;
            gameComm->mStallThreshold = 0;
            gameComm->mWaitingMessage = 0;
        }
        if (gameComm->mLinkTimeout > 0) func_00258870((cGameComm*)gameComm, 1800, 0);
        return len;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateReadQueue);
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

// PORT: cGameComm_allocPacket takes no arguments; the caller passes the connection anyway.
Packet_7BD8* cGameComm_allocPacket_7CD8(Conn_7CD8* c) __asm__("cGameComm_allocPacket");
extern "C" void operator_delete__FPi_7CD8(void *) __asm__("operator_delete__FPi");

static inline int queueFull_7CD8(PacketQueue_7BD8* q)
{
    return q->wr - q->rd >= 0x1F;
}

// Moves received packets into their channel queue. A packet that cannot be
// fetched is freed; the whole fetch sits in the loop condition, as in the original.
extern "C" void cGameComm_updateReadQueue_7CD8(Conn_7CD8* c) __asm__("cGameComm_updateReadQueue");
/**
 * Moves received packets into the game link's per-type queues.
 *
 * While the link is not busy (+4): a packet already held (+0x134) or a new one (cGameComm_allocPacket, filled by cCommSystem_readPacket
 * from the link's channel +0x6C, up to 0xF0 bytes) goes to the queue of its type byte (+0x144, 32 slots each); a packet
 * that could not be filled is deleted and the loop ends. It also stops, keeping the packet held, when that queue has 31
 * or more packets.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_updateReadQueue_7CD8(Conn_7CD8* gameComm)
{
    while (gameComm->busy == 0
           && (gameComm->pending != 0
               || (gameComm->pending = cGameComm_allocPacket_7CD8(gameComm), cCommSystem_readPacket((cCommSystem*)((void*)cCommSystem_mCommSystem), gameComm->unk6C, (int)gameComm->pending, 0xF0) != 0)
               || (operator_delete__FPi_7CD8(gameComm->pending), gameComm->pending = 0, 0))) {
        Packet_7BD8* inPacket = gameComm->pending;
        PacketQueue_7BD8* typeQueue = &gameComm->queues[inPacket->type];
        if (queueFull_7CD8(typeQueue)) {
            return;
        }
        typeQueue->slots[typeQueue->wr++ % 32] = inPacket;
        gameComm->pending = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateWriteQueue);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cCommSystem_writePacket_7DC0(int, int, char *, unsigned short) __asm__("cCommSystem_writePacket");
extern "C" void cGameComm_freePacket_7DC0(char *, char *) __asm__("cGameComm_freePacket");
struct S7DC0 { char pad[0x36C]; char *arr[32]; };

/**
 * Sends the packets waiting in the write queue, oldest first, until the channel refuses one.
 *
 * Each packet goes to the comm system's writePacket for this link's channel (cCommSystem_writePacket); a sent packet is
 * popped and freed (cGameComm_freePacket). It stops at once after a link error.
 */
extern "C" void cGameComm_updateWriteQueue(cGameComm* self) {
    if (self->mWriteQueue.mTail != self->mWriteQueue.mHead) {
        do {
            if (self->mError != 0) return;
            int idx = self->mWriteQueue.mTail;
            tGameCommPacket* item;
            if (idx < self->mWriteQueue.mHead) {
                item = self->mWriteQueue.mQueue[idx % 32];
            } else {
                item = 0;
            }
            if (cCommSystem_writePacket_7DC0((int)cCommSystem_mCommSystem, self->mChannel, (char *)item, item->mSize) == 0) return;
            self->mWriteQueue.mTail = self->mWriteQueue.mTail + 1;
            cGameComm_freePacket_7DC0(((char*)self), (char *)item);
        } while (self->mWriteQueue.mTail != self->mWriteQueue.mHead);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_writePacket);
#ifdef SKIP_ASM
struct Packet_7E98 {
    unsigned char type;     // 0x0
    unsigned char flags;    // 0x1
    unsigned short size;    // 0x2, includes this 4-byte header
    char data[0xEC];        // 0x4
};

extern "C" int cCommSystem_writePacket_7E98(int sys, int port, Packet_7E98* p, int size) __asm__("cCommSystem_writePacket");
extern "C" Packet_7E98* cGameComm_allocPacket_7E98(char* self) __asm__("cGameComm_allocPacket");
extern "C" void MEM_copy_7E98(void* dst, const void* src, int n) __asm__("MEM_copy");
extern "C" void cGameComm_updateWriteQueue(cGameComm* self);

struct Conn_7E98 {
    char pad0[0x36C];
    Packet_7E98* slots[32];     // 0x36C
};

// PORT: the unit declares this void(void*, int, int, int); the target returns a status
extern "C" int cGameComm_writePacket_impl(char* self, int type, const void* data, int len) __asm__("cGameComm_writePacket");
/**
 * Sends a packet to the peer, queueing it when it cannot go out now.
 *
 * Returns 0 while the link is busy (+4). Otherwise flushes the write queue (cGameComm_updateWriteQueue), builds the
 * packet: type byte, flags 0, size @p len + 4 and the data. When the write queue (+0x364 / +0x368) is empty and the
 * channel (+0x6C) takes it (cCommSystem_writePacket) it is sent at once; otherwise a copy (cGameComm_allocPacket) is queued.
 *
 * @param type  the packet type
 * @param data  the data
 * @param len  the data size (at most 0xEC)
 * @return 1 when sent or queued, 0 when busy
 */
extern "C" int cGameComm_writePacket_impl(char* vself, int type, const void* data, int len)
{
    cGameComm* self = (cGameComm*)vself;
    Packet_7E98 pkt;
    if (self->mError != 0) return 0;
    cGameComm_updateWriteQueue((cGameComm*)self);
    pkt.type = type;
    pkt.size = len + 4;
    pkt.flags = 0;
    MEM_copy_7E98(pkt.data, data, len);
    if (self->mWriteQueue.mTail == self->mWriteQueue.mHead) {
        if (cCommSystem_writePacket_7E98((int)cCommSystem_mCommSystem, self->mChannel, &pkt, pkt.size)) return 1;
    }
    {
        Packet_7E98* queuedPacket = cGameComm_allocPacket_7E98(((char*)self));
        MEM_copy_7E98(queuedPacket, &pkt, pkt.size);
        ((Conn_7E98*)self)->slots[(self->mWriteQueue.mHead)++ % 32] = queuedPacket;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateCmdQueue);
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

/**
 * Handles the command packets received from the peer.
 *
 * Reads 12-byte packets of type 3 (cGameComm_readPacket) and by the command byte: 0 (pause) arms the link timeout
 * (func_00258870, 3660, forced) unless locally paused, marks the peer paused and counts its pauses (+0x50); 1 (unpause)
 * clears the peer's pause, resets the pause timeout to 1200 when running and arms 1800 frames or
 * cGameComm_setUnPauseTimer; 2 counts +0x58; 3 sets +0x3F4; 4 stores the peer's tick (+0x40C) and checksum in the ring
 * at +0x500; 5 marks the peer as exited (+0x88, +0x14), records the end time (+0x3C) and may set +0x8C.
 */
extern "C" void cGameComm_updateCmdQueue(sGameComm_7F98* self)
{
    sGamePacket_7F98 msg;
    while (cGameComm_readPacket_7F98(self, 3, &msg, 0xC)) {
        switch (msg.type) {
        case 0:
            if (self->f7C == 0) {
                func_00258870((cGameComm*)self, 3660, 1);
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
                self->f64 = 1200;
            }
            self->f3F8 = 0;
            self->f60 = 0;
            self->f84 = 0;
            if (self->f7C == 0) {
                func_00258870((cGameComm*)self, 1800, 0);
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
                self->f3C = SSXApp->TimerTick / SSXApp->TimerHz;
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
INCLUDE_ASM("seg/seg_156860", cGameComm_getQueuedChat);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cGameComm_readPacket_8160(int, int, int, int) __asm__("cGameComm_readPacket");

/**
 * Reads a text message from the peer (a packet of type 1, cGameComm_readPacket).
 *
 * The chat screens (cOVState_onUpdate_HandleIncomingChat and the trophy screen) call it with a 65-byte buffer.
 *
 * @param gameComm  the game link
 * @param data  the buffer
 * @param maxSize  its size
 * @return 1 when a message was read, else 0
 */
extern "C" int cGameComm_getQueuedChat(int gameComm, int data, int maxSize) {
    return cGameComm_readPacket_8160(gameComm, 1, data, maxSize) > 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_allocPacket);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_8188(unsigned int, void *, unsigned int, int) __asm__("cMemMan_alloc");
extern char str_tGameCommPacket[];

/**
 * Allocates a 0xF0-byte game packet ("tGameCommPacket", cMemMan_alloc).
 *
 * The C declares no parameters; callers pass the game link, which is not used.
 *
 * @return the packet
 */
extern "C" void *cGameComm_allocPacket(void) {
    return cMemMan_alloc_8188(0xF0, str_tGameCommPacket, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_freePacket);
#ifdef SKIP_ASM
extern "C" void operator_delete__FPi(int *);

/**
 * Frees a game packet (operator delete).
 *
 * @param gameComm  the game link (not used)
 * @param packet  the packet
 */
extern "C" void cGameComm_freePacket(cGameComm* gameComm, int *packet) {
    operator_delete__FPi(packet);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncReadComplete);
#ifdef SKIP_ASM
/**
 * Tells whether the game link's large read has finished (mAsyncReadData is 0).
 *
 * @param gameComm  the game link
 * @return 1 when no read is pending
 */
extern "C" int cGameComm_asyncReadComplete(cGameComm* gameComm) {
    return ((int)gameComm->mAsyncReadData) == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncRead);
#ifdef SKIP_ASM
/**
 * Starts a large read into a buffer: stores @p data (mAsyncReadData) and @p size (mAsyncReadSize).
 *
 * cGameComm_asyncReadUpdate fills it piece by piece.
 *
 * @param gameComm  the game link
 * @param data  the buffer
 * @param size  bytes to read
 */
extern "C" void cGameComm_asyncRead(cGameComm* gameComm, int data, int size) {
    gameComm->mAsyncReadData = (char*)data;
    gameComm->mAsyncReadSize = size;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncReadUpdate);
#ifdef SKIP_ASM
extern "C" int cGameComm_readPacket(cGameComm*, int, int, int);

/**
 * Continues a large read: fills the pending buffer from packets of type 2.
 *
 * While a buffer is pending it reads pieces of at most 236 bytes (cGameComm_readPacket), advancing the buffer and
 * reducing the size, until no packet is waiting or the size reaches 0 (then the read is cleared).
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_asyncReadUpdate(cGameComm* gameComm) {
    int readData;
    while ((readData = (int)gameComm->mAsyncReadData) != 0) {
        int n = gameComm->mAsyncReadSize;
        if (n >= 237) n = 236;
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
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncWriteComplete);
#ifdef SKIP_ASM
/**
 * Tells whether the game link's large write has finished (mAsyncWriteData is 0).
 *
 * @param gameComm  the game link
 * @return 1 when no write is pending
 */
extern "C" int cGameComm_asyncWriteComplete(cGameComm* gameComm) {
    return ((int)gameComm->mAsyncWriteData) == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncWrite);
#ifdef SKIP_ASM
/**
 * Starts a large write from a buffer: stores @p data (mAsyncWriteData) and @p size (mAsyncWriteSize).
 *
 * cGameComm_asyncWriteUpdate sends it piece by piece.
 *
 * @param gameComm  the game link
 * @param data  the buffer
 * @param size  bytes to send
 */
extern "C" void cGameComm_asyncWrite(cGameComm* gameComm, int data, int size) {
    gameComm->mAsyncWriteData = (char*)data;
    gameComm->mAsyncWriteSize = size;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncWriteUpdate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cGameComm_writePacket_82A0(void *, int, int, int) __asm__("cGameComm_writePacket");

/**
 * Continues a large write: sends the next piece of the pending buffer as a packet of type 2.
 *
 * Only when the write queue is empty: sends at most 236 bytes (cGameComm_writePacket) and, when that worked, advances the
 * buffer and reduces the size, clearing the write when it reaches 0.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_asyncWriteUpdate(cGameComm* gameComm) {
    int writeData = (int)gameComm->mAsyncWriteData;
    if (writeData != 0 && gameComm->mWriteQueue.mTail == gameComm->mWriteQueue.mHead) {
        int n = gameComm->mAsyncWriteSize;
        if (n >= 237) n = 236;
        if (cGameComm_writePacket_82A0(((char*)gameComm), 2, writeData, n) != 0) {
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
INCLUDE_ASM("seg/seg_156860", cGameComm_calcLatency);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.

/**
 * Sets the game link's latency (mLatency) to the larger of the two measured latencies, at least one frame period
 * (SSXApp +0x14).
 *
 * cGameComm_setState calls it.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_calcLatency(cGameComm* gameComm) {
    cSSXApp* app = SSXApp;
    float latency1 = gameComm->mNetLatency[1];
    float latency0 = gameComm->mNetLatency[0];
    float maxLatency = latency1 >? latency0;
    gameComm->mLatency = maxLatency;
    float frameTime = app->TimerPeriod;
    if (maxLatency < frameTime) {
        gameComm->mLatency = frameTime;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_getLatency);
#ifdef SKIP_ASM
extern "C" float cCommSystem_getLatency(int, int);

/**
 * Returns the game link's input latency: the channel's latency (cCommSystem_getLatency on channel +0x6C) plus mNumSamples frame
 * periods (SSXApp +0x14).
 *
 * cInputSync_preupdate divides it by the frame period.
 *
 * @param gameComm  the game link
 * @return the latency
 */
extern "C" float cGameComm_getLatency(cGameComm* gameComm) {
    return cCommSystem_getLatency((int)cCommSystem_mCommSystem, gameComm->mChannel) + ((float) (gameComm->mNumSamples) * (SSXApp->TimerPeriod));
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

/**
 * Starts pinging a server through the game link's ping object.
 *
 * Only when no ping is running (+0x138 not positive): clears the ping count (+0xB8), stores the address (+0xC0), sends
 * the first ping "SSXPingServer" (func_003E7450 on the ping object +0x140, size 0xE) and keeps its id, sets +0xBC to 4,
 * the running time (+0x138) to @p durationSecs * 60 frames and clears +0x13C. +0x13C is set when the connection is not
 * broadband. The online-error pause screen calls it with the lobby server's address and 3.
 *
 * @param serverAddr  the server address
 * @param durationSecs  how long to ping, in seconds
 */
extern "C" void func_002583A8(cGameComm* self, int serverAddr, int durationSecs) {
    if (self->unk_138 <= 0) {
        self->unk_0B8 = 0;
        self->unk_0C0 = serverAddr;
        self->unk_0C4[self->unk_0B8++] = func_003E7450_83A8((int)self->unk_140, serverAddr, str_SSXPingServer, 0xE);
        self->unk_0BC = 4;
        self->unk_138 = durationSecs * 0x3C;
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
extern "C" void cBERules_swap();
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
        cBERules_swap();
        MEM_fill_8448(((char*)self) + 0x104, 0, 0x10);
        strncpy(((char*)self) + 0x104, str_NA_R_F004, 0xF);
        self->unk_100 = cCommSystem_mCommSystem->mBroadBand;
        cBENetworkInterface_getSyncData_8448(iface, 1, self->mBEBuffer);
        cBENetworkInterface_setSyncData_8448(iface, 1, self->mBEBuffer);
        cBEInterface* ui = cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerCtrlID(ui, 1, cBENewPlayerInterface_getPlayerCtrlID(ui, 0));
        ui->submit();
    }
    AIgetseed_8448(&self->mServerSettings);
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
        AIsetseed(&self->mServerSettings);
        GMsrand(AIrand());
        ggSetTime_8448(((char*)&cBENetworkInterface_networkOpt), self->mLatency);
        if (self->mServerSettings.unk_18 != 0 && self->unk_100 != 0) {
            self->mNumSamples = 2;
        }
        if (self->mError == 0 && strncmp_70D8(((char*)self) + 0xF0, ((char*)self) + 0x104, 9) != 0) {
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
extern "C" void cGameComm_writePacket(void *, int, int, int);
extern "C" void func_00258870(cGameComm*, int, int);
extern "C" void cGameComm_update(cGameComm*);
extern "C" void THREAD_yieldticks(int);
static inline int rd8(char *p) { return *(int*)(p + 8); }
/**
 * Waits for the peer at the end of loading by exchanging the sync-wait command (3).
 *
 * The host sends command 3 first and then waits; the client waits for the host's and then answers. While
 * waiting it pumps update (cGameComm_update) with a 3600-frame link timeout armed, until mSyncWaitComplete or an
 * error. Afterwards it disarms the timeout (mLinkTimeout) and clears mLoading.
 */
extern "C" void cGameComm_syncWait(cGameComm* self) {
    char buf[12];
    cBE_getInterface_86B0((int)SSXApp->BE, 7);
    if (rd8(((char*)&cBENetworkInterface_networkOpt)) != 0) {
        buf[0] = 3;
        cGameComm_writePacket(self, 3, (int)buf, 0xC);
    }
    func_00258870(self, 3600, 0);
    while (self->mSyncWaitComplete == 0 && self->mError == 0) {
        cGameComm_update(self);
        THREAD_yieldticks(1);
    }
    self->mLinkTimeout = 0;
    if (self->mError == 0 && rd8(((char*)&cBENetworkInterface_networkOpt)) == 0) {
        buf[0] = 3;
        cGameComm_writePacket(self, 3, (int)buf, 0xC);
    }
    self->mLoading = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_sendPauseCmd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGameComm_writePacket(void *, int, int, int);
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
                func_00258870(self, 3660, 1);
            } else {
                self->mUnPauseFlag = 1;
            }
            buf[0] = 0;
            self->mPauseMenu[0] = 1;
            self->mPauseCount[0] = self->mPauseCount[0] + 1;
            self->mUnPauseTimer = 0;
            self->mSyncInputDelay = 0;
        } else {
            buf[0] = 1;
            self->mPauseMenu[0] = 0;
            self->mUnPauseFlag = 0;
            self->mWaitingTimer = 0;
            self->mWaitingMessage = 0;
            self->mSyncInputDelay = 0;
            if (self->mGameStartGate == 0) {
                func_00258870(self, 1800, 0);
            } else {
                cGameComm_setUnPauseTimer_8790(self);
            }
        }
        cGameComm_writePacket(self, 3, (int)buf, 0xC);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258870);
#ifdef SKIP_ASM
/**
 * Arms the game link's link timeout (+0x94, which cGameComm_checkErrors counts down).
 *
 * Stores @p timeoutFrames when @p forceArm is set, when neither player is in the pause menu, or once the start gate is up.
 *
 * @param gameComm  the game link
 * @param timeoutFrames  the timeout in frames (1800, 3600 or 3660)
 * @param forceArm  nonzero to store it in any case
 */
extern "C" void func_00258870(cGameComm* gameComm, int timeoutFrames, int forceArm) {
    if ((forceArm != 0) || ((gameComm->mPauseMenu[1] == 0) && (gameComm->mPauseMenu[0] == 0)) || (gameComm->mGameStartGate != 0)) {
        gameComm->mLinkTimeout = timeoutFrames;
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
        && ((gameAI = ((S88A8_4*)game->AI), gameAI->k0 != 4) || gameAI->k1c < 151)
        && gameAI->k18 <= 0 && self->mPauseMenu[0] == 0 && self->mPauseMenu[1] == 0) {
        self->mUnPauseTimer = 180;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_setFinishRaceAlone);
#ifdef SKIP_ASM
/**
 * Lets the local player finish the race alone: clears mWaitingMessage and the peer's pause flag, sets mAlone.
 *
 * cAI_finishRemotePlayer calls it.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_setFinishRaceAlone(cGameComm* gameComm) {
    gameComm->mWaitingMessage = 0;
    gameComm->mAlone = 1;
    gameComm->mPauseMenu[1] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_gameStartGate);
#ifdef SKIP_ASM
/**
 * Notes that the race's start gate is up: arms the start-gate flag while the game is still running.
 *
 * Only while mState is below kGameComm_GameOver (15): clears the link timeout mLinkTimeout and sets mGameStartGate.
 * Called by cGFGateState_gainFocus. On Tour also clears mRaceInProgress and runs heatInit here.
 *
 * @param gameComm  the game link (this)
 */
extern "C" void cGameComm_gameStartGate(cGameComm* gameComm) {
    if (gameComm->mState < 0xF) {
        gameComm->mLinkTimeout = 0;
        gameComm->mGameStartGate = 1;
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
        self->unk_034 = SSXApp->TimerTick / SSXApp->TimerHz;
        self->mRaceInProgress = 1;
        self->mLinkTimeout = 0;
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
 * if they are still 0. Called by cGameComm_gameExit and cEndRaceHandler_OnGainState (ai).
 *
 * @param gameComm  the game link (this)
 */
extern "C" void cGameComm_gameEnded(cGameComm* gameComm) {
    gameComm->mRaceInProgress = 0;
    gameComm->mSyncInput = 0;
    if (gameComm->mState < 0xF) {
        cGameComm_setState_8A48(gameComm, 0xF);
    }
    if (gameComm->unk_038 == 0) {
        gameComm->unk_038 = (int) ((int) (SSXApp->TimerTick) / (int) (SSXApp->TimerHz));
    }
    if (gameComm->unk_03C == 0) {
        gameComm->unk_03C = (int) ((int) (SSXApp->TimerTick) / (int) (SSXApp->TimerHz));
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
extern "C" void cGameComm_sendExitGame(cGameComm*);
extern "C" void cGameComm_gameEnded(cGameComm*);
extern "C" void cBENetworkInterface_copyProfile(void *, int, int);
extern "C" void* cBE_getBE();
/**
 * Leaves the online game: resets the buddy presence, tells the peer and ends the game locally.
 *
 * Makes sure BuddyMan exists and is connected, as gameStartRace does, and sets presence LOCID 0. Sends the
 * exit-game command (cGameComm_sendExitGame) and runs gameEnded (cGameComm_gameEnded). On the client it then copies profile
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
    cGameComm_sendExitGame(self);
    cGameComm_gameEnded(self);
    void *i = cBE_getInterface_8AE0((int)SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsHost == 0) {
        cBENetworkInterface_copyProfile(i, 1, 0);
        cIface_AE0 *beStat = (cIface_AE0*)cBE_getInterface_8AE0((int)cBE_getBE(), 3);
        beStat->v1();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_sendExitGame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGameComm_writePacket_8BC8(void *, int, void *, int) __asm__("cGameComm_writePacket");

/**
 * Tells the peer that this player has left the game: unless there is an error, sends command 5 (a 12-byte packet of
 * type 3, cGameComm_writePacket).
 *
 * cGameComm_gameExit calls it.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_sendExitGame(cGameComm* gameComm) {
    if (gameComm->mError == 0) {
        char buf[12];
        buf[0] = 5;
        cGameComm_writePacket_8BC8(gameComm, 3, buf, 0xC);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_sendUnplugCmd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGameComm_writePacket_8C00(void *, int, void *, int) __asm__("cGameComm_writePacket");

/**
 * Sends command 2 to the peer and counts it locally (+0x54).
 *
 * Only without an error and while connected. The peer counts these in +0x58 (cGameComm_updateCmdQueue); cGameComm_checkErrors treats 3 or
 * more on either side as cheating (mCheating 3). cOVStateManager_triggerPauseMenu calls it.
 *
 * @param gameComm  the game link
 */
extern "C" void cGameComm_sendUnplugCmd(cGameComm* gameComm) {
    if (gameComm->mError == 0 && gameComm->mConnected != 0) {
        char buf[12];
        buf[0] = 2;
        gameComm->unk_054 = gameComm->unk_054 + 1;
        cGameComm_writePacket_8C00(gameComm, 3, buf, 0xC);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_sendChat);
#ifdef SKIP_ASM
extern "C" void cGameComm_writePacket(void *, int, int, int);
extern "C" int strlen(int);

/**
 * Sends a text message to the peer (a packet of type 1 with the terminating zero), when there is no error and the link
 * is connected.
 *
 * The trophy screen's chat sends with it; cGameComm_getQueuedChat reads these.
 *
 * @param gameComm  the game link
 * @param message  the text
 */
extern "C" void cGameComm_sendChat(cGameComm* gameComm, int message) {
    if ((gameComm->mError == 0) && (gameComm->mConnected != 0)) {
        cGameComm_writePacket(gameComm, 1, message, strlen(message) + 1);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateMessageDisplay);
#ifdef SKIP_ASM
int GetHashValue32(char* s);
extern "C" cUIState* cUIStateStack_findState(void* mgr, int hash);
extern "C" void strcpy1(char* dst, const char* src);
extern char str_hud_d[];
extern char str_kT_POPUPCMNOtherPauseMenu[];
extern char str_kT_OVRCMNWaitPlayer_b[];

struct sVEntS_8CB0 {
    short delta;
    short index;
    const char* (*fn)(void*, int);
};

// Whether no NIS cut-scene list is playing: mPlayingNISList is 2 when none is (the engine's constructor sets 2,
// and IsNISActive tests != 2). An earlier reading of this field as "online: connected" was wrong.
static inline int gcNoNISPlaying_8CB0()
{
    return ScriptEngine->mPlayingNISList == 2;
}

// Looks up the string table entry for `name` (vtable slot 4 of the global string table at SSXApp+0x8C).
static inline const char* gcString_8CB0(char* name)
{
    cFELocale* tbl = SSXApp->FELocale;
    sVEntS_8CB0* vt = (sVEntS_8CB0*)VPTR_OF(cUILocaleInterface, tbl);
    return vt[4].fn(vthis(tbl, vt[4]), GetHashValue32(name));
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
        self->mWaitingTimer = 0;
    }
    char* ui = (char*)SSXApp->Game->UI;
    if (ui == 0) {
        return;
    }
    cUIState* item = cUIStateStack_findState(ui + 0x18, GetHashValue32(str_hud_d));
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
                    item->handleEvent(0xE, buf);
                    self->mMessageDisplay = 1;
                }
            } else if (self->mWaitingMessage != 0) {
                item->handleEvent(0xE, buf);
                self->mMessageDisplay = 2;
            }
        }
        break;
    case 2:
        if (self->mPauseMenu[0] != 0) {
            item->handleEvent(0xF, 0);
            self->mMessageDisplay = 0;
        } else if (self->mPauseMenu[1] != 0) {
            item->handleEvent(0xE, buf);
            self->mMessageDisplay = 1;
        } else if (self->mWaitingMessage == 0) {
            item->handleEvent(0xF, 0);
            self->mMessageDisplay = 0;
        } else if (self->mUnPauseTimer > 0) {
            item->handleEvent(0xF, 0);
            self->mMessageDisplay = 0;
        }
        break;
    case 1:
        if (SSXApp->Game->mCurrentStateID == 2 && gcNoNISPlaying_8CB0()) {
            item->handleEvent(0xF, 0);
            self->mMessageDisplay = 0;
        } else if (self->mPauseMenu[1] == 0 || self->mPauseMenu[0] != 0) {
            item->handleEvent(0xF, 0);
            self->mMessageDisplay = 0;
        }
        break;
    default:
        item->handleEvent(0xF, 0);
        self->mMessageDisplay = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateSyncPacket);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cGameComm_writePacket_8F20(void* self, int type, const void* data, int len) __asm__("cGameComm_writePacket");



static inline int& RingAt_8F20(cGameComm* r, int frame)
{
    return ((int*)&r->mConnected)[(frame / 60) % 60];
}

/**
 * Exchanges the race's sync checksum with the peer and flags a desync.
 *
 * Only in game state 5 (or 3 when +0x214 is 3), with input sync on, no error, connected and not yet desynced: stores
 * @p value in the local ring (+0x410, 60 slots, one per 60 frames) and @p frame as the local tick. Unless one is in
 * flight it sends the checksum as command 4 (cGameComm_writePacket); an answered one is cleared. Once both ticks are known it
 * compares the local and peer rings (+0x500) at the lower tick and sets mDesync when they differ.
 *
 * @param gameComm  the game link
 * @param frame  the AI tick
 * @param value  the checksum
 */
extern "C" void cGameComm_updateSyncPacket(cGameComm* gameComm, int frame, int value)
{
    cGame* gs = SSXApp->Game;
    int st = *(int*)gs->AI;
    if (st != 5) {
        if (st != 3) return;
        if (gs->mCurrentStateID != st) return;
    }
    if (gameComm->mSyncInput == 0) return;
    if (gameComm->mError != 0) return;
    if (gameComm->mConnected == 0) return;
    if (gameComm->mDesync != 0) return;
    RingAt_8F20((cGameComm*)(&gameComm->mCheckSumBuffer[0]), frame) = value;
    gameComm->mAITick[0] = frame;
    if (gameComm->mCheckSumSent != 0) {
        if (gameComm->mCheckSumReceived != 0) {
            gameComm->mCheckSumReceived = 0;
            gameComm->mCheckSumSent = 0;
        }
    } else {
        tGameCmdPacket msg;
        msg.AITick = frame;
        msg.CheckSum = value;
        msg.Cmd = 4;
        cGameComm_writePacket_8F20(((char*)gameComm), 3, &msg, 12);
        gameComm->mCheckSumSent = 1;
    }
    {
        int localTick = gameComm->mAITick[0];
        if (localTick < 0) return;
        int peerTick = gameComm->mAITick[1];
        if (peerTick < 0) return;
        int minTick = peerTick < localTick ? peerTick : localTick;
        if (RingAt_8F20((cGameComm*)(&gameComm->mCheckSumBuffer[0]), minTick) != RingAt_8F20((cGameComm*)(&gameComm->mCheckSumBuffer[1]), minTick)) gameComm->mDesync = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259098);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_9098(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void*, int);
extern "C" void func_0014EBB8(void* iface, int v);
extern "C" void func_0014EBD8(void* iface, int v);
extern "C" void func_002639E0(int a, int b, int c, int d, int e);

struct VEnt_9098 {
    short delta;
    short index;
    void (*fn)(char*);
};

/**
 * Starts the peer-to-peer race connection for the lobby's game.
 *
 * Takes the peer's address from LobbyMan (the opponent's when hosting, else the host's), stores player 0's character
 * (BE interface 1) in cBENetworkInterface_networkOpt.MyCharacterID[0], gives BE interface 7 the opponent and the persona (func_0014EBB8, func_0014EBD8)
 * and calls its virtual slot 1. Then it calls func_002639E0 with the peer address and the host flag. LobbyMan_onplaymsg
 * calls it with func_002591D0's object.
 *
 * @param peerSession  the game-connection object
 * @param ctrlId  passed to func_002639E0 as the controller id
 * @param cancelFlag  passed to func_002639E0 as its cancel flag
 */
extern "C" void func_00259098(int peerSession, int ctrlId, int cancelFlag)
{
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int peerAddr;
    if (lobbyMan->mIsHost) peerAddr = lobbyMan->mOppoAddr;
    else peerAddr = lobbyMan->mHostAddr;
    void* pi = cBE_getInterface_9098(cBE_getBE(), 1);
    char* ri = (char*)cBE_getInterface_9098(SSXApp->BE, 7);
    cBENetworkInterface_networkOpt.MyCharacterID = cBENewPlayerInterface_getPlayerCharID(pi, 0);
    func_0014EBB8(ri, ((int)LobbyMan_mLobbyMan->mOpponent));
    func_0014EBD8(ri, ((int)LobbyMan_mLobbyMan->mPersona));
    {
        VEnt_9098* vt = *(VEnt_9098**)(ri + 0xC);
        vt[1].fn(ri + vt[1].delta);
    }
    func_002639E0(peerSession, peerAddr, lobbyMan->mIsHost, ctrlId, cancelFlag);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259198);
#ifdef SKIP_ASM
extern char str_en[];

/**
 * Returns the language code "en" for the lobby tags.
 *
 * @return "en"
 */
extern "C" char *func_00259198(void) {
    return str_en;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591A8);
#ifdef SKIP_ASM
extern char str_US[];

/**
 * Returns the country code "US". Nothing in the build calls it.
 *
 * @return "US"
 */
extern "C" char *func_002591A8(void) {
    return str_US;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591B8);
#ifdef SKIP_ASM

/**
 * Returns the front end's connection manager (cRealFE + 0xB5AB0, built by cConnMan_cConnMan).
 *
 * @return the connection manager (as an int)
 */
extern "C" int func_002591B8(void) {
    return ((int)SSXApp->RealFE) + 0xB5AB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591D0);
#ifdef SKIP_ASM

/**
 * Returns the front end's game-connection manager (cRealFE + 0xB5ABC, built by cGameConnMan_cGameConnMan).
 *
 * @return the game-connection manager (as an int)
 */
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

/**
 * Allocates a block for the lobby code (operator new with the name "Lobby" and type 0x20000000).
 *
 * The LobbyApi, buddy and list code allocate with it; func_00259210 frees.
 *
 * @param size  bytes to allocate
 * @return the block
 */
extern "C" void *func_002591E8(unsigned int size) {
    return operator_new_91E8(size, str_Lobby_b, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259210);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv();

/**
 * Frees a block from func_002591E8 (cMemMan_free) unless it is null.
 *
 * The C declares cMemMan_free without arguments; the block passes through in its register.
 *
 * @param buffer  the block, or 0
 */
extern "C" void func_00259210(int buffer) {
    if (buffer != 0) {
        cMemMan_free__FPv();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00259230);
#ifdef SKIP_ASM
extern "C" void *cUIStateStack_getTopState(int);

/**
 * Passes an event to the front end's top screen.
 *
 * When the front end exists, takes the top state of its state stack (cUIStateStack_getTopState) and calls its virtual
 * at +0xC4 with the event's first word and the event. func_0017D260 calls it.
 *
 * @param event  the event (first word: its id)
 */
extern "C" void func_00259230(int *event) {
    void *obj = SSXApp->RealFE;
    if (obj != 0) {
        char *topState = (char *)cUIStateStack_getTopState(*(int *)((char*)obj + 0xC) + 0x18);
        char *vt = *(char **)(topState + 8);
        void (*fn)(void *, int, int *) = *(void (**)(void *, int, int *))(vt + 0xC4);
        fn(topState + *(short *)(vt + 0xC0), *event, event);
    }
}
#endif
