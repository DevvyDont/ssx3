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
#include "ealib/netgamelink.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "util/locale.h"
#include "fe/fereal.h"
#include "ui/uiengine.h"

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_construct);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" cCommSystem *cCommSystem_cCommSystem_5860(void *) __asm__("cCommSystem_cCommSystem");
extern "C" void *cMemMan_alloc_5860(unsigned int, void *, unsigned int, int) __asm__("cMemMan_alloc");
extern char str_cCommSystem[];

/**
 * Creates the comm system singleton: allocates 0x34 bytes tagged "cCommSystem" and constructs it.
 *
 * Stores the result in cCommSystem_mCommSystem without checking for an existing one; cCommSystem_destroy destroys
 * it.
 */
extern "C" void cCommSystem_construct(void) {
    cCommSystem_mCommSystem = cCommSystem_cCommSystem_5860(cMemMan_alloc_5860(52, str_cCommSystem, 0x20000000, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_destroy);
#ifdef SKIP_ASM
extern "C" void cCommSystem__cCommSystem(cCommSystem *, int);

/**
 * Destroys the online connection manager, if there is one.
 *
 * Runs cCommSystem__cCommSystem on cCommSystem_mCommSystem with flags 3 (destroy and free) and clears the pointer.
 * cFEStateMainMenu_onCreateScreen and the debug menu screen call it.
 */
extern "C" void cCommSystem_destroy(void) {
    if (cCommSystem_mCommSystem != 0) {
        cCommSystem__cCommSystem(cCommSystem_mCommSystem, 3);
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
extern "C" cCommSystem *cCommSystem_cCommSystem_(cCommSystem *self) __asm__("cCommSystem_cCommSystem");
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
extern "C" cCommSystem *cCommSystem_cCommSystem_(cCommSystem *self) {
    int k;
    int i;
    ccs_5698(self->mCommMsgAllocMutex);
    ccs_6448(self->mChannels, 0, 4);
    self->mPool = (tCommMsgElem *)ccs_new(0xFC00, str_Comm_pool, 0x20000000, 0);
    ccs_6448(self->mPool, 0, 0xFC00);
    self->mFree = self->mPool;
    for (k = 0; k < 255; k++) {
        self->mPool[k].mNext = &self->mPool[k + 1];
    }
    for (i = 0; i < 1; i++) {
        self->mChannels[i] = 0;
    }
    cCommSystem_mNumNetConn = 0;
    self->mActiveNetConn = -1;
    self->mNetConnTimeOut = 0;
    self->mNetConnError = 0;
    self->mBroadBand = 0;
    self->mInterfaceType = 1;
    self->mNetConnState = kNetConn_Idle;
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
// cCommSystem_mNetConns: the 'netw'/'list' reply buffer (cCommSystem_netConnQuery), 10 entries of 0x100 bytes; +0x80 is an entry's name field
extern char cCommSystem_mNetConns[];

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
void cCommSystem::update()
{
    int i;
    NetConnIdle();
    switch (mNetConnState) {
    case kNetConn_None:
    case kNetConn_Idle:
    case kNetConn_Failed:
        break;
    case kNetConn_ConnectRequested:
        if (NetConnStatus_5A20('onln', 0, 0) == 0) {
            NetConnConnect(cCommSystem_mNetConns + 0x80 + (mActiveNetConn << 8), 0);
            mNetConnState = kNetConn_Starting;
        }
        break;
    case kNetConn_Starting: {
        if (NetConnStatus_5A20('onln', 0, 0) != 0 && NetConnStatus_5A20('host', 0, 0) != 0) {
            mNetConnState = kNetConn_WaitingForAddress;
            mNetConnTimeOut = 1800;
            mInterfaceType = NetConnStatus_5A20('type', 0, 0);
            int bb = NetConnStatus_5A20('bbnd', 0, 0);
            if (bb == 1) {
                mBroadBand = bb;
            }
        }
        int conn = NetConnStatus_5A20('conn', 0, 0);
        if ((conn >> 24) == '-') {
            netConnFailed(conn);
            mNetConnTimeOut = 0;
        }
        break;
    }
    case kNetConn_WaitingForAddress: {
        int addr;
        netConnLocalAddr(&addr);
        if (addr != 0) {
            mNetConnTimeOut = 0;
            mNetConnState = kNetConn_Connected;
        }
        break;
    }
    case kNetConn_Connected:
        for (i = 0; i < 1; i++) {
            cCommChannel* channel = mChannels[i];
            if (channel != 0) {
                sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)channel->mVtbl;
                vt[kCommChannelVt_update].fn(vthis(channel, vt[kCommChannelVt_update]));
            }
        }
        break;
    }
    if (mNetConnTimeOut > 0) {
        mNetConnTimeOut--;
        if (mNetConnTimeOut <= 0) {
            netConnFailed('time');
            mNetConnTimeOut = 0;
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
 * When the channel is open (mChannelOpen nonzero) it calls its virtual disconnect (cDSockChannel_vtbl slot 2), then
 * its virtual destructor with flags 3, and clears the slot. cRealFE_loadStartState and the game-connection code call
 * it.
 *
 * @param channelIndex  the channel slot
 */
void cCommSystem::closeChannel(int channelIndex)
{
    cCommChannel **slot = &mChannels[channelIndex];
    cCommChannel *channel = *slot;
    if (channel->mChannelOpen != 0) {
        sVEntry<void(void *)> *vt = (sVEntry<void(void *)> *)channel->mVtbl;
        vt[kCommChannelVt_disconnect].fn(vthis(channel, vt[kCommChannelVt_disconnect]));
    }
    cCommChannel *channelToFree = *slot;
    if (channelToFree != 0) {
        sVEntry<void(void *, int)> *vt = (sVEntry<void(void *, int)> *)channelToFree->mVtbl;
        vt[kCommChannelVt_dtor].fn(vthis(channelToFree, vt[kCommChannelVt_dtor]), 3);
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
 * @param channelIndex  the channel slot
 * @return true when the slot is set
 */
bool cCommSystem::isChannelOpen(int channelIndex)
{
    return mChannels[channelIndex] != 0;
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
 * @param channelIndex  the channel slot
 * @return the channel's answer
 */
int cCommSystem::isChannelConnected(int channelIndex)
{
    cCommChannel* obj = mChannels[channelIndex];
    sVEntry<int(void *)> *vt = (sVEntry<int(void *)> *)obj->mVtbl;
    return vt[kCommChannelVt_isConnected].fn(vthis(obj, vt[kCommChannelVt_isConnected]));
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
 * @param channelIndex  the channel slot
 * @return the latency
 */
float cCommSystem::getLatency(int channelIndex)
{
    cCommChannel* obj = mChannels[channelIndex];
    sVEntry<float(void *)> *vt = (sVEntry<float(void *)> *)obj->mVtbl;
    return vt[kCommChannelVt_getLatency].fn(vthis(obj, vt[kCommChannelVt_getLatency]));
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
 * @param channelIndex  the channel slot
 * @param data  the buffer
 * @param maxSize  the buffer size
 * @return the channel's result: nonzero when a packet was read
 */
int cCommSystem::readPacket(int channelIndex, void *data, int maxSize)
{
    cCommChannel* obj = mChannels[channelIndex];
    sVEntry<int(void *, void *, int)> *vt = (sVEntry<int(void *, void *, int)> *)obj->mVtbl;
    return vt[kCommChannelVt_readPacket].fn(vthis(obj, vt[kCommChannelVt_readPacket]), data, maxSize);
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
 * @param channelIndex  the channel slot
 * @param data  the packet
 * @param size  its size in bytes
 * @return the channel's result: nonzero when the packet was taken
 */
int cCommSystem::writePacket(int channelIndex, const void *data, int size)
{
    cCommChannel* obj = mChannels[channelIndex];
    sVEntry<int(void *, const void *, int)> *vt = (sVEntry<int(void *, const void *, int)> *)obj->mVtbl;
    return vt[kCommChannelVt_writePacket].fn(vthis(obj, vt[kCommChannelVt_writePacket]), data, size);
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
 * @param channelIndex  the channel slot
 * @return nonzero on an error
 */
int cCommSystem::getError(int channelIndex)
{
    if (mNetConnError != 0) {
        return 1;
    }
    return mChannels[channelIndex]->mError;
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
 * @return the element
 */
tCommMsgElem * cCommSystem::allocCommMsgElem()
{
    void *mutex = &mCommMsgAllocMutex;
    MUTEX_lock(mutex);
    tCommMsgElem *node = mFree;
    mFree = node->mNext;
    MUTEX_unlock(mutex);
    node->mNext = 0;
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
 * @param node  the element
 */
void cCommSystem::freeCommMsgElem(tCommMsgElem* node)
{
    void *mutex = (char*)&mCommMsgAllocMutex;
    MUTEX_lock(mutex);
    void *head = mFree;
    node->mSize = 0;
    node->mNext = (tCommMsgElem*)head;
    node->mOffset = 0;
    mFree = (tCommMsgElem*)node;
    MUTEX_unlock(mutex);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_netConnQuery);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int NetConnQuery_5F20(int, void *, int) __asm__("NetConnQuery");
extern char cCommSystem_mNetConns[];
extern int cCommSystem_mNumNetConn;

/**
 * Reads the list of network configurations into cCommSystem_mNetConns.
 *
 * Asks for up to 10 entries (NetConnQuery with @p listParam and the 0x100-byte records of cCommSystem_mNetConns) and stores
 * the count in cCommSystem_mNumNetConn. The debug menu screen and cFEStateNetConfig_loadConfigs call it.
 *
 * @param commSystem  the connection manager (not used)
 * @param listParam  passed to NetConnQuery first
 * @return 1
 */
extern "C" int cCommSystem_netConnQuery(void *commSystem, int listParam) {
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
 * @param configIndex  the cCommSystem_mNetConns entry
 */
void cCommSystem::netConnConnect(int configIndex)
{
    netConnDisconnect();
    mActiveNetConn = configIndex;
    mInterfaceType = 1;
    mNetConnTimeOut = 9000;
    mNetConnState = kNetConn_ConnectRequested;
    mBroadBand = 0;
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
 */
void cCommSystem::netConnDisconnect()
{
    if (mActiveNetConn >= 0) {
        mActiveNetConn = -1;
        NetConnDisconnect();
    }
    mNetConnTimeOut = 0;
    mNetConnState = kNetConn_Idle;
    mNetConnError = 0;
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
 * @param errorCode  the error ('time' or a status starting with '-')
 */
void cCommSystem::netConnFailed(int errorCode)
{
    if (mActiveNetConn >= 0) {
        mActiveNetConn = -1;
        NetConnDisconnect();
    }
    mNetConnError = errorCode;
    mNetConnState = kNetConn_Failed;
    mNetConnTimeOut = 0;
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
 * @param localAddr  receives the address, or 0
 */
void cCommSystem::netConnLocalAddr(int *localAddr)
{
    int temp_2;

    *localAddr = 0;
    temp_2 = NetConnStatus_6038('addr', 0, 0);
    if (temp_2 & 0xFFFFFF00) {
        *localAddr = temp_2;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", netConfig_launchGui);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void shutdownAndLoadExec_6088(void *, int, void *) __asm__("shutdownAndLoadExec");
extern char str_cdrom0_NETGUI_NTGUI_ELF_1[];
extern int netConfig_launchArgs;

/**
 * Starts the network configuration program "cdrom0:\NETGUI\NTGUI.ELF;1" (shutdownAndLoadExec with 1 and netConfig_launchArgs).
 *
 * cFEStateNetConfig_onCreateConfigPopupEvent and the debug menu screen call it.
 */
extern "C" void netConfig_launchGui(void) {
    shutdownAndLoadExec_6088(str_cdrom0_NETGUI_NTGUI_ELF_1, 1, &netConfig_launchArgs);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", dnasDebug_runAuth);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_60B0(int, char *, int, int) __asm__("cMemMan_alloc");
extern "C" int *DNASMan_DNASMan_60B0(void *) __asm__("DNASMan_DNASMan");
extern "C" void DNASMan__DNASMan_60B0(int *, int) __asm__("DNASMan__DNASMan");
extern "C" void DNASMan_start_60B0(int *, int) __asm__("DNASMan_start");
extern "C" void DNASMan_startCheck_60B0(int *) __asm__("DNASMan_startCheck");
extern "C" void DNASMan_freeBuffers_60B0(int *) __asm__("DNASMan_freeBuffers");
extern "C" void DNASMan_update_60B0(int *) __asm__("DNASMan_update");
extern "C" void SYNCTASK_run_60B0(int) __asm__("SYNCTASK_run");
extern "C" void THREAD_yieldticks_60B0(int) __asm__("THREAD_yieldticks");
extern char str_DNASMAN[];

static inline int ge3_60B0(int *o) { return *o >= 3; }

/**
 * Runs a DNAS authentication to the end, waiting in a loop (debug menu).
 *
 * Creates a 2640-byte "DNASMAN" object (DNASMan_DNASMan), starts it (DNASMan_start with 0) and updates it (DNASMan_update,
 * SYNCTASK_run, THREAD_yieldticks(1)) until its state reaches 3, then starts the check (DNASMan_startCheck) and updates it
 * until state 5. Then it frees the buffers (DNASMan_freeBuffers) and deletes the object. cDebugMenuScreen_update calls it.
 */
extern "C" void dnasDebug_runAuth(void) {
    int *dnasMan = DNASMan_DNASMan_60B0(cMemMan_alloc_60B0(2640, str_DNASMAN, 0x20000000, 0));
    DNASMan_start_60B0(dnasMan, 0);
    while (!ge3_60B0(dnasMan)) {
        DNASMan_update_60B0(dnasMan);
        SYNCTASK_run_60B0(0);
        THREAD_yieldticks_60B0(1);
    }
    DNASMan_startCheck_60B0(dnasMan);
    while ((*dnasMan ^ 5) != 0) {
        DNASMan_update_60B0(dnasMan);
        SYNCTASK_run_60B0(0);
        THREAD_yieldticks_60B0(1);
    }
    DNASMan_freeBuffers_60B0(dnasMan);
    if (dnasMan != 0) {
        DNASMan__DNASMan_60B0(dnasMan, 3);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", dnas_addOverlayArchive);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void FILESYS_addbigsync_6188(void *, int, int, void *) __asm__("FILESYS_addbigsync");
extern char str_data_config_slusovf_big[];
extern int dnas_overlayArchiveResult;

/**
 * Adds the archive "data/config/slusovf.big" to the file system (FILESYS_addbigsync with 0, 100 and dnas_overlayArchiveResult).
 *
 * cSSXApp_initload calls it.
 */
extern "C" void dnas_addOverlayArchive(void) {
    FILESYS_addbigsync_6188(str_data_config_slusovf_big, 0, 100, &dnas_overlayArchiveResult);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_DNASMan);
#ifdef SKIP_ASM
extern "C" void DNASMan_clear();

/**
 * Constructs a DNAS authentication object (DNASMan_clear clears it) and returns it.
 *
 * cFEStateAuthDNAS_onCreateScreen and dnasDebug_runAuth call it on a 2640-byte block.
 *
 * @param dnasMan  the object
 * @return @p dnasMan
 */
extern "C" int DNASMan_DNASMan(int dnasMan) {
    DNASMan_clear();
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
INCLUDE_ASM("seg/seg_156860", DNASMan_clear);
#ifdef SKIP_ASM
/**
 * Clears a DNAS authentication object: state (+0) and the words +4 to +0x14 become 0, the file handle (+0x18) -1.
 *
 */
void DNASMan::clear()
{
    ElfData = 0;
    ElfSize = 0;
    AuthData = 0;
    AuthSize = 0;
    CancelRequest = 0;
    FileHandle = -1;
    State = 0;
}
#endif

/**
 * Does nothing: the body of the DNAS authentication object's destructor (DNASMan__DNASMan calls it).
 */
extern "C" void func_00256250(void) {
}

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_start);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int ASYNCFILE_loadat(int *, void *, int);
extern int D_004803E8[];
extern int D_004A2EE0[];
extern "C" void *operator_new__FUi_6258(unsigned int, int *, int, int) __asm__("operator_new__FUi");

/**
 * Starts a DNAS authentication: begins loading its program file.
 *
 * Stores @p authMode at +0x20, clears +0x14 and +0x1C, sets the state (+0) to 1, allocates a 0x195000-byte "ElfData" buffer
 * (+4) and starts loading "OVERLAY.DAT" into it (ASYNCFILE_loadat; handle at +0x18). DNASMan_update continues.
 *
 * @param authMode  the mode, passed on to the authentication (+0x20)
 */
void DNASMan::start(int authMode)
{
    AuthMode = authMode;
    CancelRequest = 0;
    AuthError = 0;
    State = 1;
    void *mem = operator_new__FUi_6258(0x195000, D_004A2EE0, 0x04000100, 0);
    ElfData = (char*)mem;
    FileHandle = ASYNCFILE_loadat(D_004803E8, mem, 0x195000);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_startCheck);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern tDNASKey D_004A2ED8;
extern "C" void MEM_fill_62D0(void *, int, int) __asm__("MEM_fill");
extern "C" char *NetConnMAC();
extern "C" void DirtyDnas_init(void *);
extern "C" char *strcpy(char *, const char *);

/**
 * Starts the DNAS check itself once the program and data are loaded.
 *
 * Clears +0x1C and the 160-byte parameter block at +0x24, then fills it: the data buffer (+0xC) and size (+0x10), the
 * key D_004A2ED8, 8, the text from NetConnMAC, "SLUS_20772", the mode (+0x20) and 1. Sets the state (+0) to 4 and
 * starts the module (DirtyDnas_init, the DirtyDnasRelInit entry). DNASMan_update continues.
 */
void DNASMan::startCheck()
{
    AuthError = 0;
    MEM_fill_62D0(unk_024, 0, 160);
    unk_06C = AuthData;
    unk_070 = AuthSize;
    unk_090 = 0;
    unk_08C = 0;
    unk_074 = D_004A2ED8;
    unk_084 = 8;
    strcpy(unk_098, NetConnMAC());
    strcpy(unk_0A8, "SLUS_20772");
    unk_0B8 = 0;
    unk_0BC = AuthMode;
    unk_0C0 = 1;
    State = 4;
    DirtyDnas_init(unk_024);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_freeBuffers);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cMemMan_free__FPv_63A8(void *) __asm__("cMemMan_free__FPv");

/**
 * Frees a DNAS authentication object's buffers: the data (+0xC) and the program (+4), clearing each pointer.
 *
 */
void DNASMan::freeBuffers()
{
    char *authData = AuthData;
    if (authData != 0) {
        cMemMan_free__FPv_63A8(authData);
        AuthData = 0;
    }
    char *elfData = ElfData;
    if (elfData != 0) {
        cMemMan_free__FPv_63A8(elfData);
        ElfData = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void DNASMan_updateLoadProgram(DNASMan* self);
extern "C" void DNASMan_updateLoadConfig(DNASMan* self);
extern "C" void DNASMan_updateResult(DNASMan* self);

/**
 * Advances a DNAS authentication by its state (+0).
 *
 * State 1 runs DNASMan_updateLoadProgram, 2 runs DNASMan_updateLoadConfig, 4 runs DNASMan_updateResult; states 0, 3 and 5 wait.
 */
void DNASMan::update()
{
    switch (*(int*)this) {
    case 1:
        updateLoadProgram();
        break;
    case 2:
        updateLoadConfig();
        break;
    case 4:
        updateResult();
        break;
    case 0:
    case 3:
    case 5:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_updateLoadProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int ASYNCFILE_getstatus_6460(int) __asm__("ASYNCFILE_getstatus");
extern "C" void ASYNCFILE_release_6460(int, int, void *) __asm__("ASYNCFILE_release");
extern "C" int DirtyDnas_loadModule_6460(char *, int) __asm__("DirtyDnas_loadModule");
extern "C" void *operator_new__FUi_6460(unsigned int, char *, int, int) __asm__("operator_new__FUi");
extern "C" int ASYNCFILE_loadat_6460(char *, void *, int) __asm__("ASYNCFILE_loadat");
extern char str_AuthData[];
extern char str_CONFIG_DAT[];

/**
 * DNAS state 1: once "OVERLAY.DAT" has loaded, links it and starts loading "CONFIG.DAT".
 *
 * When the load (+0x18) is done it releases it with the size in +8 and, for a nonzero size, links the program
 * (DirtyDnas_loadModule, which resolves DirtyDnasRelInit / Updt / Exit). On success it allocates a 64 KB "AuthData" buffer
 * (+0xC), starts loading "CONFIG.DAT" into it and sets the state to 2.
 */
void DNASMan::updateLoadProgram()
{
    if (ASYNCFILE_getstatus_6460(FileHandle) == 1) {
        ElfSize = 0;
        ASYNCFILE_release_6460(FileHandle, 0, &ElfSize);
        char *readBuffer = ElfData;
        if (readBuffer != 0) {
            int readSize = ElfSize;
            if (readSize > 0 && DirtyDnas_loadModule_6460(readBuffer, readSize) >= 0) {
                void *authData = operator_new__FUi_6460(0x10000, str_AuthData, 0x04000100, 0);
                AuthData = (char*)authData;
                FileHandle = ASYNCFILE_loadat_6460(str_CONFIG_DAT, authData, 0x10000);
                State = 2;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_updateLoadConfig);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int ASYNCFILE_getstatus_6510(int) __asm__("ASYNCFILE_getstatus");
extern "C" void ASYNCFILE_release_6510(int, int, void *) __asm__("ASYNCFILE_release");
extern tDNASFileKey DNASMan_configKey[];

/**
 * DNAS state 2: once "CONFIG.DAT" has loaded, decodes it.
 *
 * When the load (+0x18) is done it releases it with the size in +0x10; for a nonzero size it sets the state to 3 and
 * XORs every byte of the data (+0xC) with the 4-byte key DNASMan_configKey, repeating.
 */
void DNASMan::updateLoadConfig()
{
    if (ASYNCFILE_getstatus_6510(FileHandle) == 1) {
        AuthSize = 0;
        ASYNCFILE_release_6510(FileHandle, 0, &AuthSize);
        unsigned char *fileData = (unsigned char*)AuthData;
        if (fileData != 0) {
            int n = AuthSize;
            if (n > 0) {
                State = 3;
                tDNASFileKey key = DNASMan_configKey[0];
                int i;
                unsigned char *bytePtr;
                for (i = 0, bytePtr = fileData; i < AuthSize; i++, bytePtr++) {
                    *bytePtr ^= key.b[i % 4];
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", DNASMan_updateResult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int DirtyDnas_update_65E0(int) __asm__("DirtyDnas_update");
extern "C" void MEM_fill_65E0(void *, int, int) __asm__("MEM_fill");
extern "C" int DirtyDnas_exit_65E0(tDNASResult *) __asm__("DirtyDnas_exit");
extern "C" void USTR_copy_65E0(unsigned short *, unsigned short *) __asm__("USTR_copy");

/**
 * DNAS state 4: once the check is finished, collects its result.
 *
 * When DirtyDnas_update (the module's update) returns 0 it clears the 908-byte result block at +0xC4, gets the result
 * (DirtyDnas_exit), copies three wide strings of it (+0xCC, +0x10C, +0x20C) to ErrorText1, ErrorText2 and ErrorText3
 * (USTR_copy), stores whether the result was 0 in AuthError and sets the state to 5.
 */
void DNASMan::updateResult()
{
    if (DirtyDnas_update_65E0(CancelRequest) == 0) {
        tDNASResult *infoBuf = &Result;
        MEM_fill_65E0(infoBuf, 0, sizeof(Result));
        unsigned short *secondText = ErrorText2;
        unsigned short *thirdText = ErrorText3;
        int result = DirtyDnas_exit_65E0(infoBuf);
        ErrorText1[0] = 0;
        secondText[0] = 0;
        thirdText[0] = 0;
        USTR_copy_65E0(ErrorText1, Result.Text1);
        USTR_copy_65E0(secondText, Result.Text2);
        USTR_copy_65E0(thirdText, Result.Text3);
        if (result == 0) {
            AuthError = 1;
        } else {
            AuthError = 0;
        }
        State = 5;
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
    channel->mState = cDSockChannel::kState_Closed;
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
    dSockChannel->disconnect();
    cCommChannel__cCommChannel_66E8(dSockChannel, flags);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_readPacket);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy_6730(char *, const void *, int) __asm__("MEM_copy");
extern "C" int NetGameLinkRecv_6730(NetGameLinkRefT *, char *, int) __asm__("NetGameLinkRecv");

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
unsigned char cDSockChannel::readPacket(char *dst, int maxlen)
{
    unsigned char buf[264];
    if (mState == kState_Connected) {
        while (NetGameLinkRecv_6730(mGameLink, (char *)buf, 1) > 0) {
            if (buf[2] == 5) {
                if (buf[3] != 0 && maxlen >= (int)buf[3]) {
                    unsigned char n = buf[3];
                    MEM_copy_6730(dst, buf + 4, n);
                    return buf[3];
                }
            } else {
                mPacketError = 1;
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
extern "C" void MEM_copy(char *, const void *, int);
extern "C" int NetGameLinkSend(NetGameLinkRefT *, void *, int);

/**
 * Sends one game packet over the channel's NetGameLink.
 *
 * Only while connected (mState 2) with no channel error: wraps the payload in a kind-5 packet whose length
 * byte (byte 3) holds the low 8 bits of @p len. Nothing checks that @p len fits the 0x104 bytes after the
 * header of the stack packet.
 *
 * @param data  payload to send
 * @param len   payload size in bytes; 0 or less sends nothing
 * @return true when NetGameLinkSend reports a positive result
 */
bool cDSockChannel::writePacket(const void *data, int len)
{
    char buf[264];
    if (mState != kState_Connected) return false;
    int error = mError;
    if (error != 0) return false;
    if (len > 0) {
        buf[2] = 5;
        buf[3] = len;
        MEM_copy(buf + 4, data, len);
        return error < NetGameLinkSend(mGameLink, buf, 1);
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_listen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cCommChannel_channelOpen__FPv_6860(void *) __asm__("cCommChannel_channelOpen__FPv");
extern "C" NetGameUtilRefT *NetGameUtilCreate_6860() __asm__("NetGameUtilCreate");
extern "C" int NetGameUtilConnect_6860(NetGameUtilRefT *, int, void *) __asm__("NetGameUtilConnect");
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
void cDSockChannel::listen(int addr, int port)
{
    int n = port & 0xFFFF;
    char buf[0x100];
    sVEntry<void(void *)> *vt = (sVEntry<void(void *)> *)mVtbl;
    vt[kCommChannelVt_disconnect].fn(vthis(this, vt[kCommChannelVt_disconnect]));
    cCommChannel_channelOpen__FPv_6860(((char*)this));
    NetGameUtilRefT *gameUtil = NetGameUtilCreate_6860();
    mGameUtil = gameUtil;
    if (gameUtil != 0) {
        sprintf_6860(buf, str_pct_s_pct_d_pct_d, addr, n, n);
        NetGameUtilConnect_6860(mGameUtil, 257, buf);
        mState = kState_Connecting;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_connect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cCommChannel_channelOpen__FPv_68F8(void *) __asm__("cCommChannel_channelOpen__FPv");
extern "C" NetGameUtilRefT *NetGameUtilCreate_68F8() __asm__("NetGameUtilCreate");
extern "C" int NetGameUtilConnect_68F8(NetGameUtilRefT *, int, void *) __asm__("NetGameUtilConnect");
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
void cDSockChannel::connect(int addr, int port)
{
    int n = port & 0xFFFF;
    char buf[0x100];
    mError = 0;
    sVEntry<void(void *)> *vt = (sVEntry<void(void *)> *)mVtbl;
    vt[kCommChannelVt_disconnect].fn(vthis(this, vt[kCommChannelVt_disconnect]));
    cCommChannel_channelOpen__FPv_68F8(((char*)this));
    NetGameUtilRefT *gameUtil = NetGameUtilCreate_68F8();
    mGameUtil = gameUtil;
    if (gameUtil != 0) {
        sprintf_68F8(buf, str_pct_s_pct_d_pct_d, addr, n, n);
        NetGameUtilConnect_68F8(mGameUtil, 258, buf);
        mState = kState_Connecting;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_disconnect);
#ifdef SKIP_ASM
extern "C" void cCommChannel_channelClose();
extern "C" void NetGameLinkDestroy(NetGameLinkRefT *);
extern "C" void NetGameUtilDestroy(NetGameUtilRefT *);

/**
 * Disconnects a DirtySock comm channel: closes it and destroys its game link and game util.
 *
 * Does nothing unless mState (+0x0C) is non-zero (connecting, connected or failed). Then cCommChannel_channelClose
 * clears mChannelOpen, mGameLink (+0x1C) is destroyed with NetGameLinkDestroy (NetGameLinkDestroy) and mGameUtil (+0x18)
 * with NetGameUtilDestroy (NetGameUtilDestroy), each set to 0 if present, and mState returns to 0 (closed). The disconnect
 * slot (kCommChannelVt_disconnect) of the channel's vtable.
 *
 */
void cDSockChannel::disconnect()
{
    if (mState != kState_Closed) {
        cCommChannel_channelClose();
        NetGameLinkRefT *gameLink = mGameLink;
        if (gameLink != 0) {
            NetGameLinkDestroy(gameLink);
            mGameLink = 0;
        }
        NetGameUtilRefT *gameUtil = mGameUtil;
        if (gameUtil != 0) {
            NetGameUtilDestroy(gameUtil);
            mGameUtil = 0;
        }
        mState = kState_Closed;
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
void cDSockChannel::update()
{
    switch (mState) {
    case kState_Connecting:
        updateConnecting();
        break;
    case kState_Connected:
        updateConnected();
        break;
    case kState_Closed:
    case kState_TimedOut:
    case kState_PacketError:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_updateConnecting);
#ifdef SKIP_ASM
extern "C" NetGameLinkRefT *NetGameLinkCreate(void *, int, int);
extern "C" void *NetGameUtilComplete(NetGameUtilRefT *);

/**
 * Checks whether a DirtySock channel's pending connection has completed.
 *
 * When NetGameUtilComplete (on the utility object at +0x18) returns a connection, it creates a game link on it (NetGameLinkCreate
 * with 0 and 9084) at +0x1C and, when that worked, sets the state (+0xC) to 2.
 *
 */
void cDSockChannel::updateConnecting()
{
    void *commRef = NetGameUtilComplete(mGameUtil);
    if (commRef != 0) {
        NetGameLinkRefT *gameLink = NetGameLinkCreate(commRef, 0, 9084);
        mGameLink = gameLink;
        if (gameLink != 0) {
            mState = kState_Connected;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cDSockChannel_updateConnected);
#ifdef SKIP_ASM
extern "C" NetGameLinkStatT* NetGameLinkStatus(NetGameLinkRefT* link);

/**
 * Checks a connected channel's link health and smooths its latency.
 *
 * Reads the NetGameLink status (NetGameLinkStatus): a gap over 30000 between its tick and rcvdlast (probably
 * ms since the last receive) sets state 3, isopen 0 (link lost) disconnects; both set mError.
 * Otherwise mLatency = (4 * mLatency + late) / 5, the link latency (late, ms) in seconds capped at 1, and a pending
 * mPacketError then sets state 4 and mError.
 *
 */
void cDSockChannel::updateConnected()
{
    NetGameLinkStatT* st = NetGameLinkStatus(mGameLink);
    if (st) {
        if ((int)(st->tick - st->rcvdlast) > 30000) {
            mState = kState_TimedOut;
            mError = 1;
            return;
        }
        if (st->isopen == 0) {
            sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)mVtbl;
            vt[kCommChannelVt_disconnect].fn(vthis(this, vt[kCommChannelVt_disconnect]));
            mError = 1;
            return;
        }
        float latency = (float)st->late * 0.0010000000474974513f;
        if (latency > 1.0f) latency = 1.0f;
        mLatency = (mLatency * 4.0f + latency) * 0.20000000298023224f;
    }
    if (mPacketError) {
        mState = kState_PacketError;
        mError = 1;
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
 * @return the latency in seconds
 */
float cDSockChannel::getLatency()
{
    return (mLatency);
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
 * The DirtySock code (NetGameLinkDestroy, PingRpc_destroy, NetSession_destroy and others) frees its blocks with it. The C
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
extern "C" void *cMemMan_alloc(int, int *, int, int);
extern "C" cGameComm *__9cGameCommii(void *, int, int);
extern int str_cGameComm[];

/**
 * Creates the game comm singleton: 0x5F0 bytes tagged "cGameComm", constructed by cGameComm::cGameComm.
 *
 * Stores the result in cGameComm_mGameComm; cGameComm_destroy destroys it.
 *
 * @param channel  the cCommSystem channel index the game link uses (stored in mChannel)
 * @param isServer  nonzero to run the server side of the handshake (mServer)
 */
extern "C" void cGameComm_construct(int channel, int isServer) {
    cGameComm_mGameComm = __9cGameCommii(cMemMan_alloc(1520, str_cGameComm, 0x20000000, 0), channel, isServer);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_destroy);
#ifdef SKIP_ASM
extern "C" void cGameComm__cGameComm(cGameComm *, int);

/**
 * Destroys the online game link, if there is one.
 *
 * Runs cGameComm__cGameComm on cGameComm_mGameComm with flags 3 (destroy and free) and clears the pointer. The front-end
 * online screens and the lobby code call it.
 */
extern "C" void cGameComm_destroy(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm__cGameComm(cGameComm_mGameComm, 3);
        cGameComm_mGameComm = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", __9cGameCommii);
#ifdef SKIP_ASM
cBEInterface* cBE_getInterface_6C78(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENetworkInterface_getSyncDataSize(void* iface);
extern "C" void cGameComm_setState_6C78(void* self, int mode) __asm__("cGameComm_setState");
extern "C" void MEM_fill_6C78(void* p, int c, int n) __asm__("MEM_fill");
extern "C" void* PingRpc_create();

/**
 * Constructs the online game link: the handshake state, timers, counters and empty packet queues.
 *
 * Stores the cCommSystem channel index in mChannel and @p isServer in mServer, sets mState to kGameComm_Init through
 * cGameComm_setState, and sizes mBEBufferSize with cBENetworkInterface_getSyncDataSize__FPv (cBENetworkInterface::getSyncDataSize) on BE interface
 * 7. Then: mConnected 1, mSyncInput 1, mWaitingTimer 30, mPingResult 1, mLoading 1, mOverlayTimeout/mPauseTimeout -1,
 * mNumSamples 3, both mNetLatency entries and mLatency to the default frame time (SSXApp->TimerPeriod), mRaceStartTime
 * to the current time (TimerTick / TimerHz), and the other flags, counters and ping state to 0. The five 32-slot packet
 * queues (mReadQueue, mWriteQueue) and the two 0xF0-byte checksum buffers (mCheckSumBuffer) are cleared with MEM_fill,
 * and mPingSocket gets a DirtySock handle from PingRpc_create (the ping server's socket).
 *
 * Retail's arguments are (channel, server), not On Tour's (playerID, numPlayers): this build links two machines.
 *
 * @param port   the cCommSystem channel index the link uses
 * @param isServer      non-zero on the machine that hosts the race (stored in mServer)
 */
// cGameComm setup (called by cGameComm_construct on a fresh 0x5F0-byte block): clears the session state,
// the five packet queues and the two message buffers, and returns self.
cGameComm::cGameComm(int port, int isServer)
{
    mChannel = port;
    mServer = isServer;
    cGameComm_setState_6C78(this, kGameComm_Init);
    mBEBufferSize = cBENetworkInterface_getSyncDataSize(cBE_getInterface_6C78(SSXApp->BE, 7));
    mWaitingTimer = 30;
    mConnected = 1;
    mBEBuffer = 0;
    mSyncInput = 1;
    mCheating[0] = 0;
    mCheating[1] = 0;
    mSyncInputDelay = 0;
    mLinkTimedOut = 0;
    mOverlayTimeout = -1;
    mAlone = 0;
    mGameStartGate = 0;
    mRaceInProgress = 0;
    mWaitingMessage = 0;
    mCommError = 0;
    mCommErrorWasSignificant = 0;
    mLinkTimeout = 0;
    mRemoteExitGame = 0;
    mRemoteEndGame = 0;
    mAtFinishLine = 0;
    mPingResult = 1;
    mPauseTimeout = -1;
    mLoading = 1;
    mLocalFinishedRace = 0;
    mRemoteFinishedRace = 0;
    mStallTimer = 0;
    mStallThreshold = 0;
    mMessageDisplay = 0;
    mPauseCount[0] = 0;
    mPauseCount[1] = 0;
    mLocalUnplugCount = 0;
    mRemoteUnplugCount = 0;
    mAsyncReadData = 0;
    mAsyncWriteData = 0;
    mAsyncReadSize = 0;
    mNetLatency[0] = SSXApp->TimerPeriod;
    mNetLatency[1] = SSXApp->TimerPeriod;
    mLatency = SSXApp->TimerPeriod;
    mNumSamples = 3;
    mError = 0;
    mDisplayError = 0;
    mLocalDisconnected = 0;
    mRemoteDisconnected = 0;
    mPacketCache = 0;
    mPauseMenu[0] = 0;
    mPauseMenu[1] = 0;
    mSyncWaitComplete = 0;
    mDesync = 0;
    mUnPauseFlag = 0;
    mUnPauseTimer = 0;
    mStallCount = 0;
    mPingCount = 0;
    mPingDelay = 0;
    mPingAddr = 0;
    for (int j = 3; j >= 0; j--) {
        mPingID[j] = 0;
    }
    mRaceStartTime = SSXApp->TimerTick / SSXApp->TimerHz;
    mLocalEndTime = 0;
    mRemoteEndTime = 0;
    mPingTimer = 0;
    mPingSocket = 0;
    mPingReceived = 0;
    for (int i = 0; i < 4; i++) {
        tGameCommQueue* queue = &mReadQueue[i];
        queue->mHead = 0;
        queue->mTail = 0;
        MEM_fill_6C78(queue->mQueue, 0, sizeof(queue->mQueue));
    }
    mWriteQueue.mHead = 0;
    mWriteQueue.mTail = 0;
    MEM_fill_6C78(mWriteQueue.mQueue, 0, sizeof(mWriteQueue.mQueue));
    mCheckSumSent = 0;
    mCheckSumReceived = 0;
    mAITick[0] = -1;
    mAITick[1] = -1;
    MEM_fill_6C78(mCheckSumBuffer[0], 0, sizeof(mCheckSumBuffer[0]));
    MEM_fill_6C78(mCheckSumBuffer[1], 0, sizeof(mCheckSumBuffer[1]));
    mPingSocket = PingRpc_create();
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm__cGameComm);
#ifdef SKIP_ASM
extern "C" void cMemMan_free_6EE0(void *) __asm__("cMemMan_free__FPv");
extern "C" void operator_delete__FPi(int *);
extern "C" void cGameComm_freePacket(cGameComm* arg0, int *arg1);
extern "C" void PingRpc_destroy(void* p);

static inline tGameCommPacket* Peek_6EE0(tGameCommQueue* q)
{
    return q->mTail < q->mHead ? q->mQueue[q->mTail % 32] : 0;
}

static inline tGameCommPacket* Pop_6EE0(tGameCommQueue* q)
{
    return q->mQueue[q->mTail++ % 32];
}

/**
 * Destroys an online game link.
 *
 * Frees mBEBuffer, the packet being received (mPacketCache) and every packet left in the four read queues
 * (cGameComm_freePacket), destroys the ping object (mPingSocket, PingRpc_destroy) and frees the object when bit 0 of
 * @p flags is set.
 *
 * @param flags  bit 0: free the memory
 */
extern "C" void cGameComm__cGameComm(cGameComm* self, int flags)
{
    int i;
    if (self->mBEBuffer) cMemMan_free_6EE0(self->mBEBuffer);
    if ((int*)self->mPacketCache) self->freePacket((int*)self->mPacketCache);
    for (i = 0; i < 4; i++) {
        while (Peek_6EE0(&self->mReadQueue[i])) {
            self->freePacket((int*)Pop_6EE0(&self->mReadQueue[i]));
        }
    }
    if (self->mPingSocket) {
        PingRpc_destroy(self->mPingSocket);
        self->mPingSocket = 0;
    }
    if (flags & 1) operator_delete__FPi((int*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_setState);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cGameComm_calcLatency(cGameComm* arg0);
extern "C" void cGameComm_armLinkTimeout(cGameComm*, int, int);
extern "C" void clearDiscTrayOpened(void);
/**
 * Switches the handshake and race state machine (mState) to a new tGameComm state.
 *
 * Entering GameActive (14) computes the latency (cGameComm_calcLatency), arms a 3600-frame link timeout and clears the
 * flag cd_discTrayOpenedFlag (clearDiscTrayOpened); GameOver (15) clears the waiting message and the peer's pause; the states 1, 3, ...,
 * 13 (each step's start, not its Wait) arm an 1800-frame timeout. cGameComm_armLinkTimeout skips arming while either side is
 * paused before the start gate.
 *
 * @param mode  the new tGameComm state
 */
// Switches the controller to a new mode, running per-mode setup first.
void cGameComm::setState(int mode)
{
    switch (mode) {
    case kGameComm_GameActive:
        calcLatency();
        this->armLinkTimeout(3600, 0);
        clearDiscTrayOpened();
        mState = mode;
        return;
    case kGameComm_GameOver:
        mWaitingMessage = 0;
        mPauseMenu[1] = 0;
        break;
    case kGameComm_SendSettings:
    case kGameComm_RecvSettings:
    case kGameComm_SendBE:
    case kGameComm_RecvBE:
    case kGameComm_SendLatency:
    case kGameComm_RecvLatency:
    case kGameComm_InitGameActive:
        this->armLinkTimeout(1800, 0);
        break;
    case kGameComm_SendSettingsWait:
    case kGameComm_RecvSettingsWait:
    case kGameComm_SendBEWait:
    case kGameComm_RecvBEWait:
    case kGameComm_SendLatencyWait:
    case kGameComm_RecvLatencyWait:
        break;
    }
    mState = mode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_update);
#ifdef SKIP_ASM
// PORT: several callees are defined later in the unit with their own parameter types; bind local views by asm label
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENetworkInterface_getSyncData_70D8(void* iface, int a1,
        char* buf) __asm__("cBENetworkInterface_getSyncData");
extern "C" void cBENetworkInterface_setSyncData_70D8(void* iface, int a1,
        char* buf) __asm__("cBENetworkInterface_setSyncData");
extern "C" float cCommSystem_getLatency_70D8(cCommSystem* commSystem, int channel) __asm__("cCommSystem_getLatency");
extern "C" void cGameComm_setState_70D8(void* self, int mode) __asm__("cGameComm_setState");
extern "C" void cGameComm_updateTimeout_70D8(char* p) __asm__("cGameComm_updateTimeout");
extern "C" int cGameComm_checkErrors_70D8(char* s) __asm__("cGameComm_checkErrors");
extern "C" void cGameComm_checkActive_70D8(char* s) __asm__("cGameComm_checkActive");
extern "C" void cGameComm_updateReadQueue_70D8(char* s) __asm__("cGameComm_updateReadQueue");
extern "C" void cGameComm_updateWriteQueue_70D8(char* s) __asm__("cGameComm_updateWriteQueue");
extern "C" void cGameComm_updateCmdQueue_70D8(char* s) __asm__("cGameComm_updateCmdQueue");
extern "C" int cGameComm_asyncReadComplete_70D8(char* s) __asm__("cGameComm_asyncReadComplete");
extern "C" void cGameComm_asyncRead_70D8(char* s, void* data, int size) __asm__("cGameComm_asyncRead");
extern "C" void cGameComm_asyncReadUpdate_70D8(char* s) __asm__("cGameComm_asyncReadUpdate");
extern "C" int cGameComm_asyncWriteComplete_70D8(char* s) __asm__("cGameComm_asyncWriteComplete");
extern "C" void cGameComm_asyncWrite_70D8(char* s, void* data, int size) __asm__("cGameComm_asyncWrite");
extern "C" void cGameComm_asyncWriteUpdate_70D8(char* s) __asm__("cGameComm_asyncWriteUpdate");
extern "C" void cGameComm_updateMessageDisplay_70D8(void* s) __asm__("cGameComm_updateMessageDisplay");
extern "C" int PingRpc_request_70D8(void* sock, int a1, char* tag, int n) __asm__("PingRpc_request");
extern "C" int PingRpc_pollResponse_70D8(void* sock, char* buf, int* len, int* info) __asm__("PingRpc_pollResponse");
extern "C" int strncmp_70D8(const char* a, const char* b, int n) __asm__("strncmp");
extern char str_SSXPingServer[];

/**
 * Runs one frame of the online game link: ping replies, timers, the packet queues and the pre-race handshake.
 *
 * While the ping server is active (mPingTimer > 0) it counts it down, reads replies from mPingSocket (PingRpc_pollResponse)
 * and stops when one matches a sent ping id (mPingID), else sends up to 4 "SSXPingServer" pings (PingRpc_request), one
 * every 5 frames. Then mStallTimer drops by 0.15 (not below 0), mUnPauseTimer counts down, and it runs
 * cGameComm_updateMessageDisplay, cGameComm_updateTimeout (timeouts) and cGameComm_checkErrors (checkErrors; an error
 * returns), cGameComm_updateReadQueue (updateReadQueue), cGameComm_updateWriteQueue, cGameComm_asyncReadUpdate /
 * cGameComm_asyncWriteUpdate (async read / write updates) and cGameComm_updateCmdQueue (updateCmdQueue), and checks
 * errors again.
 *
 * Last comes the handshake on mState: the server sends its settings (mServerSettings, 0x2C bytes), receives the
 * client's (mClientBroadBand / mClientSignature, 0x14 bytes), sends then receives the BE sync data (mBEBuffer,
 * getSyncData/setSyncData on BE interface 7) and exchanges latencies (mNetLatency, measured with
 * cCommSystem_getLatency), moving through states 1..13 to kGameComm_GameActive (14), where cGameComm_checkActive runs
 * each frame. The client walks the same states with every send and receive swapped. Sends use cGameComm_asyncWrite /
 * cGameComm_asyncWriteComplete (asyncWrite / complete), receives cGameComm_asyncRead / cGameComm_asyncReadComplete.
 *
 * @param vs  the game link (this)
 */
// PORT: the unit declares cGameComm_update as (void*)
void cGameComm::update()
{
    if (mPingTimer > 0) {
        char msg[0x100];
        int info[4];
        int len;
        mPingTimer = mPingTimer - 1;
        for (;;) {
            len = 0x100;
            if (PingRpc_pollResponse_70D8(mPingSocket, msg, &len, info) == 0) {
                break;
            }
            if (strncmp_70D8(msg, str_SSXPingServer, 13) != 0) {
                continue;
            }
            for (int i = 0; i < mPingCount; i++) {
                if (mPingID[i] == info[2]) {
                    mPingTimer = 0;
                    mPingReceived = 1;
                    break;
                }
            }
            if (mPingTimer <= 0) {
                break;
            }
        }
        if (mPingTimer > 0 && mPingCount < 4) {
            if (mPingDelay > 0) {
                mPingDelay = mPingDelay - 1;
            } else {
                int sock = PingRpc_request_70D8(mPingSocket, mPingAddr, str_SSXPingServer, 0xE);
                int k = mPingCount;
                mPingID[k] = sock;
                mPingCount = k + 1;
                mPingDelay = 4;
            }
        }
    }
    mStallTimer = mStallTimer - 0.15000000596046448f;
    if (mStallTimer < 0.0f) {
        mStallTimer = 0.0f;
    }
    if (mUnPauseTimer > 0) {
        mUnPauseTimer = mUnPauseTimer - 1;
    }
    cGameComm_updateMessageDisplay_70D8(((char*)this));
    cGameComm_updateTimeout_70D8(((char*)this));
    if (cGameComm_checkErrors_70D8(((char*)this)) != 0) {
        return;
    }
    cGameComm_updateReadQueue_70D8(((char*)this));
    cGameComm_updateWriteQueue_70D8(((char*)this));
    cGameComm_asyncReadUpdate_70D8(((char*)this));
    cGameComm_asyncWriteUpdate_70D8(((char*)this));
    cGameComm_updateCmdQueue_70D8(((char*)this));
    if (cGameComm_checkErrors_70D8(((char*)this)) != 0) {
        return;
    }
    void* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    if (mServer != 0) {
        switch ((unsigned int)mState) {
        case kGameComm_Init:
            cGameComm_setState_70D8(((char*)this), 1);
            break;
        case kGameComm_SendSettings:
            cGameComm_asyncWrite_70D8(((char*)this), &mServerSettings, 44);
            cGameComm_setState_70D8(((char*)this), 2);
            break;
        case kGameComm_SendSettingsWait:
            if (cGameComm_asyncWriteComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 3);
            }
            break;
        case kGameComm_RecvSettings:
            cGameComm_asyncRead_70D8(((char*)this), &mClientBroadBand, 20);
            cGameComm_setState_70D8(((char*)this), 4);
            break;
        case kGameComm_RecvSettingsWait:
            if (cGameComm_asyncReadComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 5);
            }
            break;
        case kGameComm_SendBE:
            cBENetworkInterface_getSyncData_70D8(iface, 0, mBEBuffer);
            cGameComm_asyncWrite_70D8(((char*)this), mBEBuffer, mBEBufferSize);
            cGameComm_setState_70D8(((char*)this), 6);
            break;
        case kGameComm_SendBEWait:
            if (cGameComm_asyncWriteComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 7);
            }
            break;
        case kGameComm_RecvBE:
            cGameComm_asyncRead_70D8(((char*)this), mBEBuffer, mBEBufferSize);
            cGameComm_setState_70D8(((char*)this), 8);
            break;
        case kGameComm_RecvBEWait:
            if (cGameComm_asyncReadComplete_70D8(((char*)this)) != 0) {
                cBENetworkInterface_setSyncData_70D8(iface, 1, mBEBuffer);
                cGameComm_setState_70D8(((char*)this), 9);
            }
            break;
        case kGameComm_SendLatency:
            mNetLatency[0] = cCommSystem_getLatency_70D8(cCommSystem_mCommSystem, mChannel);
            cGameComm_asyncWrite_70D8(((char*)this), &mNetLatency[0], 4);
            cGameComm_setState_70D8(((char*)this), 10);
            break;
        case kGameComm_SendLatencyWait:
            if (cGameComm_asyncWriteComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 11);
            }
            break;
        case kGameComm_RecvLatency:
            cGameComm_asyncRead_70D8(((char*)this), &mNetLatency[1], 4);
            cGameComm_setState_70D8(((char*)this), 12);
            break;
        case kGameComm_RecvLatencyWait:
            if (cGameComm_asyncReadComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 13);
            }
            break;
        case kGameComm_InitGameActive:
            cGameComm_setState_70D8(((char*)this), 14);
            break;
        case kGameComm_GameActive:
            cGameComm_checkActive_70D8(((char*)this));
            break;
        case kGameComm_GameOver:
            break;
        }
    } else {
        switch ((unsigned int)mState) {
        case kGameComm_Init:
            cGameComm_setState_70D8(((char*)this), 3);
            break;
        case kGameComm_RecvSettings:
            cGameComm_asyncRead_70D8(((char*)this), &mServerSettings, 44);
            cGameComm_setState_70D8(((char*)this), 4);
            break;
        case kGameComm_RecvSettingsWait:
            if (cGameComm_asyncReadComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 1);
            }
            break;
        case kGameComm_SendSettings:
            cGameComm_asyncWrite_70D8(((char*)this), &mClientBroadBand, 20);
            cGameComm_setState_70D8(((char*)this), 2);
            break;
        case kGameComm_SendSettingsWait:
            if (cGameComm_asyncWriteComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 7);
            }
            break;
        case kGameComm_RecvBE:
            cGameComm_asyncRead_70D8(((char*)this), mBEBuffer, mBEBufferSize);
            cGameComm_setState_70D8(((char*)this), 8);
            break;
        case kGameComm_RecvBEWait:
            if (cGameComm_asyncReadComplete_70D8(((char*)this)) != 0) {
                cBENetworkInterface_setSyncData_70D8(iface, 0, mBEBuffer);
                cGameComm_setState_70D8(((char*)this), 5);
            }
            break;
        case kGameComm_SendBE:
            cBENetworkInterface_getSyncData_70D8(iface, 1, mBEBuffer);
            cGameComm_asyncWrite_70D8(((char*)this), mBEBuffer, mBEBufferSize);
            cGameComm_setState_70D8(((char*)this), 6);
            break;
        case kGameComm_SendBEWait:
            if (cGameComm_asyncWriteComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 11);
            }
            break;
        case kGameComm_RecvLatency:
            cGameComm_asyncRead_70D8(((char*)this), &mNetLatency[0], 4);
            cGameComm_setState_70D8(((char*)this), 12);
            break;
        case kGameComm_RecvLatencyWait:
            if (cGameComm_asyncReadComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 9);
            }
            break;
        case kGameComm_SendLatency:
            mNetLatency[1] = cCommSystem_getLatency_70D8(cCommSystem_mCommSystem, mChannel);
            cGameComm_asyncWrite_70D8(((char*)this), &mNetLatency[1], 4);
            cGameComm_setState_70D8(((char*)this), 10);
            break;
        case kGameComm_SendLatencyWait:
            if (cGameComm_asyncWriteComplete_70D8(((char*)this)) != 0) {
                cGameComm_setState_70D8(((char*)this), 13);
            }
            break;
        case kGameComm_InitGameActive:
            cGameComm_setState_70D8(((char*)this), 14);
            break;
        case kGameComm_GameActive:
            cGameComm_checkActive_70D8(((char*)this));
            break;
        case kGameComm_GameOver:
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
 * machine's end time (mLocalEndTime) is not stamped yet, it stores the current time (SSXApp +0x18 / +0x10). Then clears
 * mConnected, mRaceInProgress and mPauseMenu[1] (the peer's pause) and sets mLocalDisconnected (reported as "DIDQUIT").
 *
 * @param force      non-zero to stamp the end time even after an error or the end of the race
 */
void cGameComm::localDisconnected(int force)
{
    if (((force != 0) || ((mError == 0) && (mState < kGameComm_GameOver))) && (mLocalEndTime == 0)) {
        mLocalEndTime = (int) ((int) (SSXApp->TimerTick) / (int) (SSXApp->TimerHz));
    }
    mConnected = 0;
    mRaceInProgress = 0;
    mPauseMenu[1] = 0;
    mLocalDisconnected = 1;
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
void cGameComm::setCommError()
{
    if (mCommError == 0) {
        mCommError = 1;
        int *significant = (int *)(&mCommErrorWasSignificant);
        int isSignificant = 0;
        if (mLocalFinishedRace == 0 || mRemoteFinishedRace == 0) {
            int lateState = mState >= kGameComm_GameOver;
            if (!lateState) isSignificant = 1;
        }
        *significant = isSignificant;
        mFlagsOnCommError = (unsigned int)(mLocalFinishedRace | (mRemoteFinishedRace << 1) | (mLocalDisconnected << 2) | (mRemoteDisconnected << 3)
            | (mGameStartGate << 4) | (mRaceInProgress << 5) | (mAlone << 6) | (mRemoteExitGame << 7)
            | (mRemoteEndGame << 8) | (mAtFinishLine << 9) | (mLoading << 10));
        mStateOnCommError = mState;
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
 */
void cGameComm::updateTimeout()
{
    if (mDisplayError == 0 && mAlone == 0 && (mError == 0 || mRaceInProgress == 0)) {
        int overlayTimeout = mOverlayTimeout;
        if (overlayTimeout > 0) {
            if (mPauseMenu[0] == 0 || mPauseMenu[1] == 0) {
                mOverlayTimeout = overlayTimeout - 1;
            }
        }
        int pauseTimeout = mPauseTimeout;
        if (pauseTimeout > 0) {
            mPauseTimeout = pauseTimeout - 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_checkErrors);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cCommSystem_getError_77C8(cCommSystem* sys, int port) __asm__("cCommSystem_getError");
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
 * @return 1 when there is an error, else 0
 */
int cGameComm::checkErrors()
{
    if (mDisplayError > 0) {
        updateDisplayError();
        return 1;
    }
    if (mError != 0) return 1;
    if (mRemoteEndGame != 0) {
        displayError();
        return 1;
    }
    if (cCommSystem_getError_77C8(cCommSystem_mCommSystem, mChannel) != 0) {
        setCommError();
        displayError();
        return 1;
    }
    if (mLocalUnplugCount >= 3) {
        mCheating[0] = 3;
        displayError();
        return 1;
    }
    if (mPauseCount[0] >= 6) {
        mCheating[0] = 2;
        displayError();
        return 1;
    }
    if (mRemoteUnplugCount >= 3) {
        mCheating[1] = 3;
        displayError();
        return 1;
    }
    if (mPauseCount[1] >= 6) {
        mCheating[1] = 2;
        displayError();
        return 1;
    }
    if (mStallTimer >= 3600.0f) {
        setCommError();
        displayError();
        return 1;
    }
    if (mLinkTimeout <= 0) return 0;
    if (--mLinkTimeout > 0) return 0;
    mLinkTimedOut = 1;
    setCommError();
    displayError();
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_displayError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGameComm_updateDisplayError_78F8(void *) __asm__("cGameComm_updateDisplayError");
extern "C" void cGameComm_updateMessageDisplay_78F8(void *) __asm__("cGameComm_updateMessageDisplay");
extern "C" void cVoipMan_disconnect_78F8(int) __asm__("cVoipMan_disconnect");
extern int VoipMan;

/**
 * Ends the online session on an error: stops input syncing and brings up the online-error overlay.
 *
 * Always clears the waiting message, the peer's pause and the unpause flag and timer. On the first error
 * only, it sets mCheating[0] to 4 if this machine's controller is unplugged mid-race with no other error,
 * closes the voice link (cVoipMan_disconnect on VoipMan, probably cVoipMan), lets the connection popup close,
 * clears mSyncInput and mConnected, sets mError and mDisplayError and runs updateDisplayError.
 */
void cGameComm::displayError()
{
    mWaitingMessage = 0;
    mPauseMenu[1] = 0;
    mUnPauseFlag = 0;
    mUnPauseTimer = 0;
    if (mError == 0) {
        if (mDesync == 0 && mCheating[0] == 0) {
            if ((mCommError == 0 || mRemoteExitGame != 0) && mLocalFinishedRace == 0 && mRaceInProgress != 0) {
                if (SSXApp->Game->ControllerUnplugged != 0) {
                    mCheating[0] = 4;
                }
            }
        }
        if (VoipMan != 0) {
            cVoipMan_disconnect_78F8(VoipMan);
        }
        cGameComm_updateMessageDisplay_78F8(((char*)this));
        mSyncInput = 0;
        mConnected = 0;
        mError = 1;
        mDisplayError = 1;
        cGameComm_updateDisplayError_78F8(((char*)this));
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
void cGameComm::updateDisplayError()
{
    if (mLoading == 0 && mDisplayError > 0) {
        char *uiEngine = (char*)SSXApp->Game->UI;
        char *st;
        if (uiEngine != 0 && (uiEngine += 0x18) != 0 && ((st = cUIStateStack_getCurrentState_79C8(uiEngine), st == 0) || (st[0x1D] & 0x3F) == 5)) {
            int n = mDisplayError - 1;
            mDisplayError = n;
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
extern int cd_discTrayOpenedFlag;

/**
 * Watches the game link for a stalled or silent peer.
 *
 * Without mCheating[0] and with the local player not in the pause menu: once the start gate is up and the peer is not
 * paused or unpausing, it counts mStallThreshold and +0x60; in AI states 4 and 5 mStallTimer grows once mStallThreshold
 * reaches 10 (mStallCount counts each time it reaches 10). At 60 for +0x60 mWaitingMessage is set. During a race a
 * desync, or 1800 frames of +0x5C (counted while the peer is not paused), raise the comm error and show it. Last, when
 * cd_discTrayOpenedFlag is set, mCheating[0] becomes 1 and the error is shown.
 *
 */
void cGameComm::checkActive()
{
    if (mCheating[0] != 0) return;
    if (mPauseMenu[0] == 0) {
        if (mGameStartGate != 0) {
            if (mPauseMenu[1] == 0 && mUnPauseTimer <= 0) {
                cSSXApp* appObj = SSXApp;
                int n = ++mStallThreshold;
                ++mWaitingTimer;
                unsigned int st = *(int*)appObj->Game->AI - 4;
                if (st < 2) {
                    if (n >= 10) mStallTimer += 1.0f;
                    if (mStallThreshold == 10) ++mStallCount;
                }
            }
            if (mWaitingTimer == 60) mWaitingMessage = 1;
        }
        if (mRaceInProgress != 0) {
            if (mDesync != 0) {
                setCommError();
                displayError();
                return;
            }
            if (mPauseMenu[1] == 0) ++mSyncInputDelay;
            if (mSyncInputDelay >= 1800) {
                mLinkTimedOut = 1;
                setCommError();
                displayError();
                return;
            }
        }
    }
    if (cd_discTrayOpenedFlag != 0) {
        mCheating[0] = 1;
        displayError();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_readPacket);
#ifdef SKIP_ASM

extern "C" void cGameComm_updateReadQueue(cGameComm* c);
extern "C" void cGameComm_freePacket(cGameComm* arg0, int *arg1);
extern "C" void cGameComm_armLinkTimeout(cGameComm*, int, int);
extern "C" void MEM_copy_7BD8(void* dst, const void* src, int n) __asm__("MEM_copy");

/**
 * Pops the oldest received packet of one type and copies its payload out.
 *
 * Returns 0 after a link error. Otherwise it first queues newly received packets (cGameComm_updateReadQueue,
 * updateReadQueue), then copies and frees the oldest packet of type @p ch. An input packet (type 0) also
 * clears the silence counters and the waiting message, and an armed link timeout is re-armed to 1800 frames.
 *
 * @param ch      packet type, which picks the read queue: 0 input samples, 1 chat, 2 BE sync data, 3 commands
 * @param dst     buffer for the payload
 * @param maxsize  size of @p dst; never checked, so the whole payload (up to 236 bytes) is copied
 * @return the payload length (packet size minus the 4-byte header), or 0 when the queue is empty or after an
 *         error
 */
int cGameComm::readPacket(int ch, void* dst, int maxsize)
{
    cGameComm* gameComm = this;
    if (gameComm->mError != 0) return 0;
    gameComm->updateReadQueue();
    tGameCommQueue* queue = &gameComm->mReadQueue[ch];
    if (queue->mTail != queue->mHead) {
        tGameCommPacket* packet = queue->mQueue[queue->mTail++ % 32];
        int len = packet->mSize - 4;
        MEM_copy_7BD8(dst, packet->mData, len);
        gameComm->freePacket((int*)packet);
        if (ch == 0) {
            gameComm->mSyncInputDelay = 0;
            gameComm->mWaitingTimer = 0;
            gameComm->mStallThreshold = 0;
            gameComm->mWaitingMessage = 0;
        }
        if (gameComm->mLinkTimeout > 0) gameComm->armLinkTimeout(1800, 0);
        return len;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateReadQueue);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.

// PORT: cGameComm_allocPacket takes no arguments; the caller passes the connection anyway.
tGameCommPacket* cGameComm_allocPacket_7CD8(cGameComm* c) __asm__("cGameComm_allocPacket");
extern "C" void operator_delete__FPi_7CD8(void *) __asm__("operator_delete__FPi");

static inline int queueFull_7CD8(tGameCommQueue* q)
{
    return q->mHead - q->mTail >= 0x1F;
}

// Moves received packets into their channel queue. A packet that cannot be
// fetched is freed; the whole fetch sits in the loop condition, as in the original.
/**
 * Moves received packets into the game link's per-type queues.
 *
 * While the link has no error (mError): a packet already held (mPacketCache) or a new one (cGameComm_allocPacket,
 * filled by cCommSystem_readPacket from the link's channel mChannel, up to 0xF0 bytes) goes to the read queue of its
 * type byte (mReadQueue, 32 slots each); a packet that could not be filled is deleted and the loop ends. It also
 * stops, keeping the packet held, when that queue has 31 or more packets.
 *
 */
void cGameComm::updateReadQueue()
{
    while (mError == 0
           && (mPacketCache != 0
               || (mPacketCache = cGameComm_allocPacket_7CD8(this),
                   cCommSystem_mCommSystem->readPacket(mChannel,
                                          mPacketCache, 0xF0) != 0)
               || (operator_delete__FPi_7CD8(mPacketCache), mPacketCache = 0, 0))) {
        tGameCommPacket* inPacket = mPacketCache;
        tGameCommQueue* typeQueue = &mReadQueue[inPacket->mType];
        if (queueFull_7CD8(typeQueue)) {
            return;
        }
        typeQueue->mQueue[typeQueue->mHead++ % 32] = inPacket;
        mPacketCache = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_updateWriteQueue);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cCommSystem_writePacket_7DC0(cCommSystem *, int, char *,
        unsigned short) __asm__("cCommSystem_writePacket");
extern "C" void cGameComm_freePacket_7DC0(char *, char *) __asm__("cGameComm_freePacket");

/**
 * Sends the packets waiting in the write queue, oldest first, until the channel refuses one.
 *
 * Each packet goes to the comm system's writePacket for this link's channel (cCommSystem_writePacket); a sent packet is
 * popped and freed (cGameComm_freePacket). It stops at once after a link error.
 */
void cGameComm::updateWriteQueue()
{
    if (mWriteQueue.mTail != mWriteQueue.mHead) {
        do {
            if (mError != 0) return;
            int idx = mWriteQueue.mTail;
            tGameCommPacket* item;
            if (idx < mWriteQueue.mHead) {
                item = mWriteQueue.mQueue[idx % 32];
            } else {
                item = 0;
            }
            if (cCommSystem_writePacket_7DC0(cCommSystem_mCommSystem, mChannel, (char *)item,
                    item->mSize) == 0) return;
            mWriteQueue.mTail = mWriteQueue.mTail + 1;
            cGameComm_freePacket_7DC0(((char*)this), (char *)item);
        } while (mWriteQueue.mTail != mWriteQueue.mHead);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_writePacket);
#ifdef SKIP_ASM
extern "C" int cCommSystem_writePacket_7E98(cCommSystem* sys, int port, tGameCommPacket* p, int size)
    __asm__("cCommSystem_writePacket");
extern "C" tGameCommPacket* cGameComm_allocPacket_7E98(char* self) __asm__("cGameComm_allocPacket");
extern "C" void MEM_copy_7E98(void* dst, const void* src, int n) __asm__("MEM_copy");
extern "C" void cGameComm_updateWriteQueue(cGameComm* self);

/**
 * Sends a packet to the peer, queueing it when it cannot go out now.
 *
 * Returns 0 after a link error (mError). Otherwise flushes the write queue (cGameComm_updateWriteQueue), builds the
 * packet: type byte, player id 0, size @p len + 4 and the data. When the write queue (mWriteQueue) is empty and the
 * channel (mChannel) takes it (cCommSystem_writePacket) it is sent at once; otherwise a copy (cGameComm_allocPacket) is
 * queued.
 *
 * @param type  the packet type
 * @param data  the data
 * @param len  the data size (at most 0xEC)
 * @return 1 when sent or queued, 0 when busy
 */
int cGameComm::writePacket(int type, const void* data, int len)
{
    tGameCommPacket pkt;
    if (mError != 0) return 0;
    updateWriteQueue();
    pkt.mType = type;
    pkt.mSize = len + 4;
    pkt.mPlayerID = 0;
    MEM_copy_7E98(pkt.mData, data, len);
    if (mWriteQueue.mTail == mWriteQueue.mHead) {
        if (cCommSystem_writePacket_7E98(cCommSystem_mCommSystem, mChannel, &pkt, pkt.mSize)) return 1;
    }
    {
        tGameCommPacket* queuedPacket = cGameComm_allocPacket_7E98(((char*)this));
        MEM_copy_7E98(queuedPacket, &pkt, pkt.mSize);
        mWriteQueue.mQueue[(mWriteQueue.mHead)++ % 32] = queuedPacket;
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

/**
 * Handles the command packets received from the peer.
 *
 * Reads 12-byte packets of type 3 (cGameComm_readPacket) and by the command byte: 0 (pause) arms the link timeout
 * (cGameComm_armLinkTimeout, 3660, forced) unless locally paused, marks the peer paused and counts its pauses (mPauseCount[1]);
 * 1 (unpause) clears the peer's pause, resets the overlay timeout to 1200 when running and arms 1800 frames or
 * cGameComm_setUnPauseTimer; 2 counts mRemoteUnplugCount; 3 sets mSyncWaitComplete; 4 stores the peer's tick
 * (mAITick[1]) and checksum in its ring (mCheckSumBuffer[1]); 5 marks the peer as exited (mRemoteExitGame,
 * mRemoteDisconnected), records its end time (mRemoteEndTime) and may set mRemoteEndGame.
 */
void cGameComm::updateCmdQueue()
{
    tGameCmdPacket msg;
    while (cGameComm_readPacket_7F98(this, 3, &msg, 0xC)) {
        switch (msg.Cmd) {
        case 0:
            if (mGameStartGate == 0) {
                this->armLinkTimeout(3660, 1);
            } else {
                mUnPauseFlag = 1;
            }
            mPauseMenu[1] = 1;
            mSyncInputDelay = 0;
            mPauseCount[1]++;
            mUnPauseTimer = 0;
            break;
        case 1:
            mSyncInputDelay = 0;
            mPauseMenu[1] = 0;
            if (mPauseMenu[0] != 0 && mOverlayTimeout > 0) {
                mOverlayTimeout = 1200;
            }
            mUnPauseFlag = 0;
            mWaitingTimer = 0;
            mWaitingMessage = 0;
            if (mGameStartGate == 0) {
                this->armLinkTimeout(1800, 0);
            } else {
                cGameComm_setUnPauseTimer_7F98(this);
            }
            break;
        case 3:
            mSyncWaitComplete = 1;
            break;
        case 5:
            mRemoteExitGame = 1;
            if (mRemoteEndTime == 0) {
                mRemoteEndTime = SSXApp->TimerTick / SSXApp->TimerHz;
            }
            mPauseMenu[1] = 0;
            mConnected = 0;
            mRemoteDisconnected = 1;
            if (mError == 0 && mState < kGameComm_GameOver) {
                mRemoteEndGame = 1;
            }
            break;
        case 2:
            mRemoteUnplugCount++;
            break;
        case 4:
            mCheckSumReceived = 1;
            mAITick[1] = msg.AITick;
            mCheckSumBuffer[1][msg.AITick / 60 % 60] = msg.CheckSum;
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
 * @param packet  the packet
 */
void cGameComm::freePacket(int *packet)
{
    operator_delete__FPi(packet);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncReadComplete);
#ifdef SKIP_ASM
/**
 * Tells whether the game link's large read has finished (mAsyncReadData is 0).
 *
 * @return 1 when no read is pending
 */
int cGameComm::asyncReadComplete()
{
    return mAsyncReadData == 0;
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
 * @param data  the buffer
 * @param size  bytes to read
 */
void cGameComm::asyncRead(void* data, int size)
{
    mAsyncReadData = (char*)data;
    mAsyncReadSize = size;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncReadUpdate);
#ifdef SKIP_ASM
extern "C" int cGameComm_readPacket(cGameComm*, int, void*, int);

/**
 * Continues a large read: fills the pending buffer from packets of type 2.
 *
 * While a buffer is pending it reads pieces of at most 236 bytes (cGameComm_readPacket), advancing the buffer and
 * reducing the size, until no packet is waiting or the size reaches 0 (then the read is cleared).
 *
 */
void cGameComm::asyncReadUpdate()
{
    char* readData;
    while ((readData = mAsyncReadData) != 0) {
        int n = mAsyncReadSize;
        if (n >= 237) n = 236;
        if (readPacket(2, readData, n) == 0) break;
        mAsyncReadData += n;
        int rem = mAsyncReadSize - n;
        mAsyncReadSize = rem;
        if (rem <= 0) {
            mAsyncReadData = 0;
            mAsyncReadSize = 0;
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
 * @return 1 when no write is pending
 */
int cGameComm::asyncWriteComplete()
{
    return mAsyncWriteData == 0;
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
 * @param data  the buffer
 * @param size  bytes to send
 */
void cGameComm::asyncWrite(void* data, int size)
{
    mAsyncWriteData = (char*)data;
    mAsyncWriteSize = size;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_asyncWriteUpdate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cGameComm_writePacket_82A0(void *, int, const void *, int) __asm__("cGameComm_writePacket");

/**
 * Continues a large write: sends the next piece of the pending buffer as a packet of type 2.
 *
 * Only when the write queue is empty: sends at most 236 bytes (cGameComm_writePacket) and, when that worked, advances the
 * buffer and reduces the size, clearing the write when it reaches 0.
 *
 */
void cGameComm::asyncWriteUpdate()
{
    char* writeData = mAsyncWriteData;
    if (writeData != 0 && mWriteQueue.mTail == mWriteQueue.mHead) {
        int n = mAsyncWriteSize;
        if (n >= 237) n = 236;
        if (cGameComm_writePacket_82A0(((char*)this), 2, writeData, n) != 0) {
            mAsyncWriteData += n;
            int rem = mAsyncWriteSize - n;
            mAsyncWriteSize = rem;
            if (rem <= 0) {
                mAsyncWriteData = 0;
                mAsyncWriteSize = 0;
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
 */
void cGameComm::calcLatency()
{
    cSSXApp* app = SSXApp;
    float latency1 = mNetLatency[1];
    float latency0 = mNetLatency[0];
    float maxLatency = latency1 >? latency0;
    mLatency = maxLatency;
    float frameTime = app->TimerPeriod;
    if (maxLatency < frameTime) {
        mLatency = frameTime;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_getLatency);
#ifdef SKIP_ASM
extern "C" float cCommSystem_getLatency(cCommSystem*, int);

/**
 * Returns the game link's input latency: the channel's latency (cCommSystem_getLatency on channel +0x6C) plus mNumSamples frame
 * periods (SSXApp +0x14).
 *
 * cInputSync_preupdate divides it by the frame period.
 *
 * @return the latency
 */
float cGameComm::getLatency()
{
    return cCommSystem_mCommSystem->getLatency(mChannel)
           + ((float) (mNumSamples) * (SSXApp->TimerPeriod));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_startServerPing);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int PingRpc_request_83A8(void *, int, char *, int) __asm__("PingRpc_request");
extern char str_SSXPingServer[];

/**
 * Starts pinging a server through the game link's ping object.
 *
 * Only when no ping is running (+0x138 not positive): clears the ping count (+0xB8), stores the address (+0xC0), sends
 * the first ping "SSXPingServer" (PingRpc_request on the ping object +0x140, size 0xE) and keeps its id, sets +0xBC to 4,
 * the running time (+0x138) to @p durationSecs * 60 frames and clears +0x13C. +0x13C is set when the connection is not
 * broadband. The online-error pause screen calls it with the lobby server's address and 3.
 *
 * @param serverAddr  the server address
 * @param durationSecs  how long to ping, in seconds
 */
void cGameComm::startServerPing(int serverAddr, int durationSecs)
{
    if (mPingTimer <= 0) {
        mPingCount = 0;
        mPingAddr = serverAddr;
        mPingID[mPingCount++] =
            PingRpc_request_83A8(mPingSocket, serverAddr, str_SSXPingServer, 0xE);
        mPingDelay = 4;
        mPingTimer = durationSecs * 0x3C;
        mPingReceived = 0;
    }
    if (cCommSystem_mCommSystem->mBroadBand == 0) {
        mPingReceived = 1;
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

static inline int ggSynced_8448(cGameComm* gameComm) { return gameComm->mState == kGameComm_GameActive; }
static inline int ggDropped_8448(cGameComm* gameComm) { return gameComm->mError; }


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
void cGameComm::syncGame()
{
    void* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    mBEBuffer = (char*)operator_new_8448((unsigned int)mBEBufferSize, str_bebuffer, 0x100, 0);
    cBENetworkInterface_preInitBE(iface);
    if (cBENetworkInterface_networkOpt.bIsHost != 0) {
        MEM_fill_8448(mServerSettings.mSignature, 0, 0x10);
        strncpy(mServerSettings.mSignature, str_NA_R_F004, 0xF);
        mServerSettings.mBroadBand = cCommSystem_mCommSystem->mBroadBand;
        cBENetworkInterface_getSyncData_8448(iface, 0, mBEBuffer);
        cBENetworkInterface_setSyncData_8448(iface, 0, mBEBuffer);
    } else {
        cBERules_swap();
        MEM_fill_8448(mClientSignature, 0, 0x10);
        strncpy(mClientSignature, str_NA_R_F004, 0xF);
        mClientBroadBand = cCommSystem_mCommSystem->mBroadBand;
        cBENetworkInterface_getSyncData_8448(iface, 1, mBEBuffer);
        cBENetworkInterface_setSyncData_8448(iface, 1, mBEBuffer);
        cBEInterface* ui = cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerCtrlID(ui, 1, cBENewPlayerInterface_getPlayerCtrlID(ui, 0));
        ui->submit();
    }
    AIgetseed_8448(&mServerSettings);
    while (!ggSynced_8448(this) && ggDropped_8448(this) == 0) {
        update();
        THREAD_yieldticks(1);
    }
    if (mBEBuffer) {
        cMemMan_free_8448(mBEBuffer);
    }
    mNumSamples = 3;
    mBEBuffer = 0;
    if (mError == 0) {
        cBENetworkInterface_postInitBE(iface);
        AIsetseed(&mServerSettings);
        GMsrand(AIrand());
        cBENetworkInterface_networkOpt.fLatency = mLatency;
        if (mServerSettings.mBroadBand != 0 && mClientBroadBand != 0) {
            mNumSamples = 2;
        }
        if (mError == 0 && strncmp_70D8(mServerSettings.mSignature, mClientSignature, 9) != 0) {
            setCommError();
            displayError();
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_syncWait);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cBE_getInterface_86B0(void*, int) __asm__("cBE_getInterface__Fv");
extern "C" void cGameComm_writePacket(void *, int, const void *, int);
extern "C" void cGameComm_armLinkTimeout(cGameComm*, int, int);
extern "C" void cGameComm_update(cGameComm*);
extern "C" void THREAD_yieldticks(int);
/**
 * Waits for the peer at the end of loading by exchanging the sync-wait command (3).
 *
 * The host sends command 3 first and then waits; the client waits for the host's and then answers. While
 * waiting it pumps update (cGameComm_update) with a 3600-frame link timeout armed, until mSyncWaitComplete or an
 * error. Afterwards it disarms the timeout (mLinkTimeout) and clears mLoading.
 */
void cGameComm::syncWait()
{
    char buf[12];
    cBE_getInterface_86B0(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsHost != 0) {
        buf[0] = 3;
        cGameComm_writePacket(this, 3, buf, 0xC);
    }
    this->armLinkTimeout(3600, 0);
    while (mSyncWaitComplete == 0 && mError == 0) {
        update();
        THREAD_yieldticks(1);
    }
    mLinkTimeout = 0;
    if (mError == 0 && cBENetworkInterface_networkOpt.bIsHost == 0) {
        buf[0] = 3;
        cGameComm_writePacket(this, 3, buf, 0xC);
    }
    mLoading = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_sendPauseCmd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGameComm_writePacket(void *, int, const void *, int);
extern "C" void cGameComm_armLinkTimeout(cGameComm*, int, int);
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
void cGameComm::sendPauseCmd(int pause)
{
    char buf[12];
    if (mError == 0 && mConnected != 0) {
        if (pause != 0) {
            if (mGameStartGate == 0) {
                this->armLinkTimeout(3660, 1);
            } else {
                mUnPauseFlag = 1;
            }
            buf[0] = 0;
            mPauseMenu[0] = 1;
            mPauseCount[0] = mPauseCount[0] + 1;
            mUnPauseTimer = 0;
            mSyncInputDelay = 0;
        } else {
            buf[0] = 1;
            mPauseMenu[0] = 0;
            mUnPauseFlag = 0;
            mWaitingTimer = 0;
            mWaitingMessage = 0;
            mSyncInputDelay = 0;
            if (mGameStartGate == 0) {
                this->armLinkTimeout(1800, 0);
            } else {
                cGameComm_setUnPauseTimer_8790(this);
            }
        }
        cGameComm_writePacket(this, 3, buf, 0xC);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_armLinkTimeout);
#ifdef SKIP_ASM
/**
 * Arms the game link's link timeout (+0x94, which cGameComm_checkErrors counts down).
 *
 * Stores @p timeoutFrames when @p forceArm is set, when neither player is in the pause menu, or once the start gate is up.
 *
 * @param timeoutFrames  the timeout in frames (1800, 3600 or 3660)
 * @param forceArm  nonzero to store it in any case
 */
void cGameComm::armLinkTimeout(int timeoutFrames, int forceArm)
{
    if ((forceArm != 0) || ((mPauseMenu[1] == 0) && (mPauseMenu[0] == 0)) || (mGameStartGate != 0)) {
        mLinkTimeout = timeoutFrames;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_setUnPauseTimer);
#ifdef SKIP_ASM


/**
 * Starts the 180-frame unpause countdown (mUnPauseTimer) when the race can resume, otherwise clears it.
 *
 * The countdown starts only outside game state 2, with a live link (no error, not alone, connected), before
 * GameOver, with neither side paused, the AI's EndOfRaceTimer at 0 or below, and not in AI state 4 with more
 * than 150 countdown ticks left.
 */
void cGameComm::setUnPauseTimer()
{
    cGame *game;
    cAI *gameAI;
    mUnPauseTimer = 0;
    game = ((SSXApp)->Game);
    if (game->mCurrentStateID != 2 && mError == 0 && mAlone == 0 && mConnected != 0
        && mState < kGameComm_GameOver
        && ((gameAI = game->AI, gameAI->CurrentAIState != 4) || gameAI->CountdownTicksLeft < 151)
        && gameAI->EndOfRaceTimer <= 0 && mPauseMenu[0] == 0 && mPauseMenu[1] == 0) {
        mUnPauseTimer = 180;
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
 */
void cGameComm::setFinishRaceAlone()
{
    mWaitingMessage = 0;
    mAlone = 1;
    mPauseMenu[1] = 0;
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
 */
void cGameComm::gameStartGate()
{
    if (mState < kGameComm_GameOver) {
        mLinkTimeout = 0;
        mGameStartGate = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_gameStartRace);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void BuddyMan_createInstance_8988(void) __asm__("BuddyMan_createInstance");
extern "C" int BuddyMan_isLoggedIn_8988(BuddyMan *) __asm__("BuddyMan_isLoggedIn");
extern "C" void BuddyMan_reconnect_8988(BuddyMan *) __asm__("BuddyMan_reconnect");
extern "C" void LobbyMan_disconnect_8988(LobbyMan *) __asm__("LobbyMan_disconnect");
extern "C" void BuddyMan_setPresence_8988(BuddyMan *, int, int, const char *, const char *, const char *)
    __asm__("BuddyMan_setPresence");

/**
 * Marks the online race as started and tells the buddy server that this player is racing.
 *
 * Does nothing once the game is over. Creates (BuddyMan_createInstance) and connects (BuddyMan_reconnect) BuddyMan when
 * needed, then sets its presence to LOCID 9 with the opponent (LobbyMan mOpponent) through BuddyMan_setPresence.
 * Stores the start time (SSXApp +0x18 / +0x10) in mRaceStartTime, sets mRaceInProgress and disarms the link timeout.
 */
void cGameComm::gameStartRace()
{
    if (mState < kGameComm_GameOver) {
        if (BuddyMan_mBuddyMan == 0) {
            BuddyMan_createInstance_8988();
        }
        if (BuddyMan_isLoggedIn_8988(BuddyMan_mBuddyMan) == 0) {
            BuddyMan_reconnect_8988(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            LobbyMan_disconnect_8988(LobbyMan_mLobbyMan);
        }
        BuddyMan* buddyMan = BuddyMan_mBuddyMan;
        if (buddyMan != 0) {
            BuddyMan_setPresence_8988(buddyMan, 1, 9, LobbyMan_mLobbyMan->mOpponent, 0, 0);
        }
        mRaceStartTime = SSXApp->TimerTick / SSXApp->TimerHz;
        mRaceInProgress = 1;
        mLinkTimeout = 0;
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
 * and stamps this machine's (mLocalEndTime) and the peer's (mRemoteEndTime) end times with the current time (SSXApp
 * +0x18 / +0x10) if they are still 0. Called by cGameComm_gameExit and cEndRaceHandler_OnGainState (ai).
 *
 */
void cGameComm::gameEnded()
{
    mRaceInProgress = 0;
    mSyncInput = 0;
    if (mState < kGameComm_GameOver) {
        cGameComm_setState_8A48(this, 0xF);
    }
    if (mLocalEndTime == 0) {
        mLocalEndTime = (int) ((int) (SSXApp->TimerTick) / (int) (SSXApp->TimerHz));
    }
    if (mRemoteEndTime == 0) {
        mRemoteEndTime = (int) ((int) (SSXApp->TimerTick) / (int) (SSXApp->TimerHz));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", cGameComm_gameExit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cBE_getInterface_8AE0(void*, int) __asm__("cBE_getInterface__Fv");
extern "C" void BuddyMan_createInstance();
extern "C" int BuddyMan_isLoggedIn(BuddyMan*);
extern "C" void BuddyMan_reconnect(BuddyMan*);
extern "C" void LobbyMan_disconnect(LobbyMan*);
extern "C" void BuddyMan_setPresence(BuddyMan*, int, int, const char*, const char*, const char*);
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
void cGameComm::gameExit()
{
    if (BuddyMan_mBuddyMan == 0) {
        BuddyMan_createInstance();
    }
    if (BuddyMan_isLoggedIn(BuddyMan_mBuddyMan) == 0) {
        BuddyMan_reconnect(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        LobbyMan_disconnect(LobbyMan_mLobbyMan);
    }
    BuddyMan* buddyMan = BuddyMan_mBuddyMan;
    if (buddyMan != 0) {
        BuddyMan_setPresence(buddyMan, 1, 0, 0, 0, 0);
    }
    sendExitGame();
    gameEnded();
    void *i = cBE_getInterface_8AE0(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsHost == 0) {
        cBENetworkInterface_copyProfile(i, 1, 0);
        cBEInterface *beStat = (cBEInterface*)cBE_getInterface_8AE0(cBE_getBE(), 3);
        beStat->discard();
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
 */
void cGameComm::sendExitGame()
{
    if (mError == 0) {
        char buf[12];
        buf[0] = 5;
        cGameComm_writePacket_8BC8(this, 3, buf, 0xC);
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
 */
void cGameComm::sendUnplugCmd()
{
    if (mError == 0 && mConnected != 0) {
        char buf[12];
        buf[0] = 2;
        mLocalUnplugCount = mLocalUnplugCount + 1;
        cGameComm_writePacket_8C00(this, 3, buf, 0xC);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", cGameComm_sendChat);
#ifdef SKIP_ASM
extern "C" void cGameComm_writePacket(void *, int, const void *, int);
extern "C" unsigned int strlen(const char *);

/**
 * Sends a text message to the peer (a packet of type 1 with the terminating zero), when there is no error and the link
 * is connected.
 *
 * The trophy screen's chat sends with it; cGameComm_getQueuedChat reads these.
 *
 * @param message  the text
 */
void cGameComm::sendChat(const char* message)
{
    if ((mError == 0) && (mConnected != 0)) {
        cGameComm_writePacket(this, 1, message, strlen(message) + 1);
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
void cGameComm::updateMessageDisplay()
{
    char buf[0x100];
    if (mWaitingMessage != 0) {
        mWaitingTimer = 0;
    }
    char* ui = (char*)SSXApp->Game->UI;
    if (ui == 0) {
        return;
    }
    cUIState* item = cUIStateStack_findState(ui + 0x18, GetHashValue32(str_hud_d));
    if (item == 0) {
        return;
    }
    if (mPauseMenu[1] != 0) {
        strcpy1(buf, gcString_8CB0(str_kT_POPUPCMNOtherPauseMenu));
    } else {
        strcpy1(buf, gcString_8CB0(str_kT_OVRCMNWaitPlayer_b));
    }
    switch (mMessageDisplay) {
    case 0:
        if (mPauseMenu[0] == 0) {
            if (mPauseMenu[1] != 0) {
                int kind = *(int*)SSXApp->Game->AI;
                if (kind == 4 || kind == 5 || !gcNoNISPlaying_8CB0()) {
                    item->handleEvent(0xE, buf);
                    mMessageDisplay = 1;
                }
            } else if (mWaitingMessage != 0) {
                item->handleEvent(0xE, buf);
                mMessageDisplay = 2;
            }
        }
        break;
    case 2:
        if (mPauseMenu[0] != 0) {
            item->handleEvent(0xF, 0);
            mMessageDisplay = 0;
        } else if (mPauseMenu[1] != 0) {
            item->handleEvent(0xE, buf);
            mMessageDisplay = 1;
        } else if (mWaitingMessage == 0) {
            item->handleEvent(0xF, 0);
            mMessageDisplay = 0;
        } else if (mUnPauseTimer > 0) {
            item->handleEvent(0xF, 0);
            mMessageDisplay = 0;
        }
        break;
    case 1:
        if (SSXApp->Game->mCurrentStateID == 2 && gcNoNISPlaying_8CB0()) {
            item->handleEvent(0xF, 0);
            mMessageDisplay = 0;
        } else if (mPauseMenu[1] == 0 || mPauseMenu[0] != 0) {
            item->handleEvent(0xF, 0);
            mMessageDisplay = 0;
        }
        break;
    default:
        item->handleEvent(0xF, 0);
        mMessageDisplay = 0;
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
 * @param frame  the AI tick
 * @param value  the checksum
 */
void cGameComm::updateSyncPacket(int frame, int value)
{
    cGame* gs = SSXApp->Game;
    int st = *(int*)gs->AI;
    if (st != 5) {
        if (st != 3) return;
        if (gs->mCurrentStateID != st) return;
    }
    if (mSyncInput == 0) return;
    if (mError != 0) return;
    if (mConnected == 0) return;
    if (mDesync != 0) return;
    RingAt_8F20((cGameComm*)(&mCheckSumBuffer[0]), frame) = value;
    mAITick[0] = frame;
    if (mCheckSumSent != 0) {
        if (mCheckSumReceived != 0) {
            mCheckSumReceived = 0;
            mCheckSumSent = 0;
        }
    } else {
        tGameCmdPacket msg;
        msg.AITick = frame;
        msg.CheckSum = value;
        msg.Cmd = 4;
        cGameComm_writePacket_8F20(((char*)this), 3, &msg, 12);
        mCheckSumSent = 1;
    }
    {
        int localTick = mAITick[0];
        if (localTick < 0) return;
        int peerTick = mAITick[1];
        if (peerTick < 0) return;
        int minTick = peerTick < localTick ? peerTick : localTick;
        if (RingAt_8F20((cGameComm*)(&mCheckSumBuffer[0]), minTick) != RingAt_8F20((cGameComm*)(&mCheckSumBuffer[1]),
                minTick)) mDesync = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_startRaceConnection);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_9098(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void*, int);
extern "C" void cBENetworkInterface_setOpponentName(void* iface, const char* name);
extern "C" void cBENetworkInterface_setMyName(void* iface, const char* name);
extern "C" void cGameConnMan_connect(int a, int b, int c, int d, int e);

/**
 * Starts the peer-to-peer race connection for the lobby's game.
 *
 * Takes the peer's address from LobbyMan (the opponent's when hosting, else the host's), stores player 0's character
 * (BE interface 1) in cBENetworkInterface_networkOpt.MyCharacterID[0], gives BE interface 7 the opponent and the
 * persona (cBENetworkInterface_setOpponentName, cBENetworkInterface_setMyName) and calls its submit (virtual slot 1). Then it calls cGameConnMan_connect with the
 * peer address and the host flag. LobbyMan_onplaymsg calls it with lobbyman_getGameConnMan's object.
 *
 * @param peerSession  the game-connection object
 * @param ctrlId  passed to cGameConnMan_connect as the controller id
 * @param cancelFlag  passed to cGameConnMan_connect as its cancel flag
 */
extern "C" void lobbyman_startRaceConnection(int peerSession, int ctrlId, int cancelFlag)
{
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int peerAddr;
    if (lobbyMan->mIsHost) peerAddr = lobbyMan->mOppoAddr;
    else peerAddr = lobbyMan->mHostAddr;
    void* pi = cBE_getInterface_9098(cBE_getBE(), 1);
    cBEInterface* ri = cBE_getInterface_9098(SSXApp->BE, 7);
    cBENetworkInterface_networkOpt.MyCharacterID = cBENewPlayerInterface_getPlayerCharID(pi, 0);
    cBENetworkInterface_setOpponentName(ri, LobbyMan_mLobbyMan->mOpponent);
    cBENetworkInterface_setMyName(ri, LobbyMan_mLobbyMan->mPersona);
    ri->submit();
    cGameConnMan_connect(peerSession, peerAddr, lobbyMan->mIsHost, ctrlId, cancelFlag);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_getLanguageCode);
#ifdef SKIP_ASM
extern char str_en[];

/**
 * Returns the language code "en" for the lobby tags.
 *
 * @return "en"
 */
extern "C" char *lobbyman_getLanguageCode(void) {
    return str_en;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_getCountryCode);
#ifdef SKIP_ASM
extern char str_US[];

/**
 * Returns the country code "US". Nothing in the build calls it.
 *
 * @return "US"
 */
extern "C" char *lobbyman_getCountryCode(void) {
    return str_US;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_getConnMan);
#ifdef SKIP_ASM

/**
 * Returns the front end's connection manager (cRealFE::mConnMan at +0xB5AB0, built by cConnMan_cConnMan).
 *
 * @return the connection manager
 */
extern "C" cConnMan* lobbyman_getConnMan(void) {
    return &((cRealFE*)SSXApp->RealFE)->mConnMan;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_getGameConnMan);
#ifdef SKIP_ASM

/**
 * Returns the front end's game-connection manager (cRealFE::mGameConnMan at +0xB5ABC, built by
 * cGameConnMan_cGameConnMan).
 *
 * @return the game-connection manager
 */
extern "C" cGameConnMan* lobbyman_getGameConnMan(void) {
    return &((cRealFE*)SSXApp->RealFE)->mGameConnMan;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_alloc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *operator_new_91E8(unsigned int, void *, unsigned int, int) __asm__("operator_new__FUi");
extern char str_Lobby_b[];

/**
 * Allocates a block for the lobby code (operator new with the name "Lobby" and type 0x20000000).
 *
 * The LobbyApi, buddy and list code allocate with it; lobbyman_free frees.
 *
 * @param size  bytes to allocate
 * @return the block
 */
extern "C" void *lobbyman_alloc(unsigned int size) {
    return operator_new_91E8(size, str_Lobby_b, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_free);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv();

/**
 * Frees a block from lobbyman_alloc (cMemMan_free) unless it is null.
 *
 * The C declares cMemMan_free without arguments; the block passes through in its register.
 *
 * @param buffer  the block, or 0
 */
extern "C" void lobbyman_free(int buffer) {
    if (buffer != 0) {
        cMemMan_free__FPv();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", lobbyman_postEventToTopScreen);
#ifdef SKIP_ASM
/**
 * Passes an event to the front end's top screen.
 *
 * When the front end exists, takes the top state of its UI engine's state stack (cUIStateStack_getTopState) and calls
 * its handleEvent (virtual slot 24) with the event's first word and the event. dispatchLobbyEvent calls it.
 *
 * @param event  the event (first word: its id)
 */
extern "C" void lobbyman_postEventToTopScreen(int *event) {
    cFE *fe = SSXApp->RealFE;
    if (fe != 0) {
        cUIState *topState = (cUIState *)fe->mpUI->mStateStack.getTopState();
        topState->handleEvent(*event, event);
    }
}
#endif
