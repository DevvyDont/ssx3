#ifndef ONLINE_COMMSYSTEM_H
#define ONLINE_COMMSYSTEM_H

// cCommSystem: the online connection manager. It brings the PS2 network interface up through the IOP's 'netw'
// requests (a netcnf connection from mNetConns, the 'onln'/'host'/'type'/'bbnd'/'conn'/'addr' status words), owns
// the peer-to-peer channels (one cDSockChannel in retail) and a pool of message elements. cGameComm talks to the peer
// through it (readPacket/writePacket/getLatency on its channel index). EA's source: d:/ssx/code/source/online/
// commsystem.cpp and online/commsystem.h (the preview build's assert strings).
//
// One instance: cCommSystem_construct does `cMemMan_alloc(0x34, "cCommSystem", ...)` + cCommSystem_cCommSystem into
// cCommSystem::mCommSystem (retail cCommSystem_mCommSystem); cCommSystem_destroy (destroy) runs cCommSystem__cCommSystem. Size 0x34 [R].
// No base class and no virtuals (the constructor stores no vtable; On Tour has none).
// Statics (retail addresses): mCommSystem cCommSystem_mCommSystem, mNumNetConn cCommSystem_mNumNetConn (func_00255F20 stores the count),
// mNetConns cCommSystem_mNetConns (NetConfigRecT[10], 0x100 each: the 'netw'/'list' reply buffer; retail reads strings at
// +0x4 (list box), +0x42 (display name) and +0x80 (passed to 'star' by update), which are On Tour's NetConfigRecT
// strDev/strIsp/strLoad four bytes earlier: retail's record lacks one of On Tour's two leading ints).
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] retail's constructor cCommSystem_cCommSystem / destructor cCommSystem__cCommSystem and the allocation;
//   [U] how retail code uses the field (the methods 0x255860..0x256088 in seg_156860 and the users of cCommSystem_mCommSystem:
//       cGameComm, debugfe, loadscreens, feasyncfile, tagsunk);
//   [O] SSX On Tour's DWARF (class cCommSystem, size 0x40): the same members in the same order; On Tour has four
//       channels (mChannels[4]) and 1-byte bools, retail one channel (every loop is `i < 1`) and 4-byte bools.
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

struct cCommChannel;

// tCommMsgElem: one element of the message pool (0xFC bytes; [R] the constructor carves 0x100 of them out of one
// 0xFC00-byte "Comm pool" block and links them into the free list). On Tour: tCommMsgElem (0xFC), mSize/mOffset/mData
// after a 4-byte base its dump does not show.
struct tCommMsgElem {
    tCommMsgElem* mNext;         // 0x00 (name ours) [R] the free-list link (the constructor stores the next element;
                                 //      alloc/free pop and push through it)
    int mSize;                   // 0x04 [O]; [U] freeCommMsgElem (cCommSystem_freeCommMsgElem) clears it
    int mOffset;                 // 0x08 [O]; [U] freeCommMsgElem clears it
    char mData[240];             // 0x0C [O]
};

// mNetConnState's values (names ours, from the roles on mNetConnState below: [R] the constructor's 1; [U]
// cCommSystem_update's switch, netConnConnect's 2, netConnDisconnect's 1, netConnFailed's 6). Nothing stores 0.
enum eNetConnState {
    kNetConn_None = 0,
    kNetConn_Idle = 1,
    kNetConn_ConnectRequested = 2,
    kNetConn_Starting = 3,
    kNetConn_WaitingForAddress = 4,
    kNetConn_Connected = 5,
    kNetConn_Failed = 6
};

struct cCommSystem {
    cCommChannel* mChannels[1];  // 0x00 [O] (On Tour [4]); [R] cleared; [U] openDSockChannel (0x255BF8) stores the new
                                 //      cDSockChannel; closeChannel (cCommSystem_closeChannel), isChannelOpen (cCommSystem_isChannelOpen) and the
                                 //      channel calls (cCommSystem_isChannelConnected..cCommSystem_writePacket) index it with the channel argument
    int mCommMsgAllocMutex[4];   // 0x04 [O] MUTEX (On Tour: int reserved[4]); [R] MUTEX_create(self + 4), destroyed by
                                 //      MUTEX_destroy in the destructor; [U] allocCommMsgElem/freeCommMsgElem lock it
    tCommMsgElem* mPool;         // 0x14 [O]; [R] operator new(0xFC00, "Comm pool"); the destructor frees it
    tCommMsgElem* mFree;         // 0x18 [O]; [R] = mPool; [U] the free-list head (cCommSystem_allocCommMsgElem pops, cCommSystem_freeCommMsgElem pushes)
    int mActiveNetConn;          // 0x1C [O]; [R] -1; [U] netConnConnect (cCommSystem_netConnConnect) stores the mNetConns index;
                                 //      netConnDisconnect (cCommSystem_netConnDisconnect) / netConnFailed (cCommSystem_netConnFailed) reset it to -1
    int mNetConnState;           // 0x20 [O]; [R] 1; [U] update (cCommSystem_update): 1 idle, 2 connect requested, 3 starting,
                                 //      4 waiting for the local address, 5 connected (channels updated), 6 failed;
                                 //      debugfe/tagsunk test it inline (netConnIsConnected etc.)
    int mNetConnTimeOut;         // 0x24 [O]; [R] 0; [U] netConnConnect sets 9000 frames, update 1800; 0 = timed out ('time')
    int mNetConnError;           // 0x28 [O]; [R] 0; [U] netConnFailed stores the error ('conn' status or 'time');
                                 //      getError(channel) (cCommSystem_getError) reports it first
    int mBroadBand;              // 0x2C [O] (On Tour: bool); [R] 0; [U] update sets 1 when 'bbnd' answers 1; read by
                                 //      cGameComm_syncGame (tServerSettings), the ping (func_002583A8), tagsunk
    int mInterfaceType;          // 0x30 [O]; [R] 1; [U] netConnConnect sets 1, update stores the 'type' status; read by
                                 //      tagsunk func_0025C610

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cCommSystem_update");
    void closeChannel(int channelIndex) __asm__("cCommSystem_closeChannel");
    bool isChannelOpen(int channelIndex) __asm__("cCommSystem_isChannelOpen");
    int isChannelConnected(int channelIndex) __asm__("cCommSystem_isChannelConnected");
    float getLatency(int channelIndex) __asm__("cCommSystem_getLatency");
    int readPacket(int channelIndex, void *data, int maxSize) __asm__("cCommSystem_readPacket");
    int writePacket(int channelIndex, const void *data, int size) __asm__("cCommSystem_writePacket");
    int getError(int channelIndex) __asm__("cCommSystem_getError");
    tCommMsgElem* allocCommMsgElem() __asm__("cCommSystem_allocCommMsgElem");
    void freeCommMsgElem(tCommMsgElem* node) __asm__("cCommSystem_freeCommMsgElem");
    void netConnConnect(int configIndex) __asm__("cCommSystem_netConnConnect");
    void netConnDisconnect() __asm__("cCommSystem_netConnDisconnect");
    void netConnFailed(int errorCode) __asm__("cCommSystem_netConnFailed");
    void netConnLocalAddr(int *localAddr) __asm__("cCommSystem_netConnLocalAddr");
};

CHECK_SIZE(tCommMsgElem, 0xFC);
CHECK_OFFSET(tCommMsgElem, mNext, 0x00);
CHECK_OFFSET(tCommMsgElem, mSize, 0x04);
CHECK_OFFSET(tCommMsgElem, mOffset, 0x08);
CHECK_OFFSET(tCommMsgElem, mData, 0x0C);

CHECK_OFFSET(cCommSystem, mChannels, 0x00);
CHECK_OFFSET(cCommSystem, mCommMsgAllocMutex, 0x04);
CHECK_OFFSET(cCommSystem, mPool, 0x14);
CHECK_OFFSET(cCommSystem, mFree, 0x18);
CHECK_OFFSET(cCommSystem, mActiveNetConn, 0x1C);
CHECK_OFFSET(cCommSystem, mNetConnState, 0x20);
CHECK_OFFSET(cCommSystem, mNetConnTimeOut, 0x24);
CHECK_OFFSET(cCommSystem, mNetConnError, 0x28);
CHECK_OFFSET(cCommSystem, mBroadBand, 0x2C);
CHECK_OFFSET(cCommSystem, mInterfaceType, 0x30);
CHECK_SIZE(cCommSystem, 0x34);

// The one comm system (On Tour: cCommSystem::mCommSystem), created by cCommSystem_construct, and the netconn count.
extern cCommSystem* cCommSystem_mCommSystem;
extern int cCommSystem_mNumNetConn;

#endif /* ONLINE_COMMSYSTEM_H */
