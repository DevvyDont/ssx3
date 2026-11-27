#ifndef EALIB_NETGAMELINK_H
#define EALIB_NETGAMELINK_H

// NetGameLinkRefT: the EE side of DirtySock's NetGameLink (C), the packet link of an online game. The link itself runs
// on the IOP; the EE keeps this block, which NetGameLinkCreate allocates with RpcAlloc (n + 916 bytes, zero-filled):
// the 0x180-byte header below, then three 64-byte-aligned areas the IOP writes: the receive buffer (n bytes), the
// 336-byte status area and a 4-byte word with the IOP's read position in the sent data. The EE reads all three through
// the uncached alias of their addresses ((address & 0x0FFFFFFF) | 0x20000000). Requests go to the IOP as 'gmlk'
// commands (func_003F4C98 / func_003F4C68): 'crea', 'dest', 'stat', 'recv', 'send', 'ctrl'. Retail code: unit
// ealib/seg_2DCAE8 (NetGameLinkCreate .. NetGameLinkControl, NetGameLink_completeStatus). Owner: cDSockChannel::mGameLink
// (online/dsockchannel.h).
//
// Name: DirtySock's NetGameLinkRefT ([O] On Tour's DWARF has the struct, opaque (size 0), and cDSockChannel's
// mGameLink as NetGameLinkRefT*; On Tour's PSP link has no IOP, so no layout). Member names ours, from retail use.
// C structs. Tags: [R] NetGameLinkCreate (allocates and initialises the block); [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct NetGameLinkRefT NetGameLinkRefT;
typedef struct NetGameLinkPacketHead NetGameLinkPacketHead;
typedef struct NetGameLinkHistT NetGameLinkHistT;
typedef struct NetGameLinkStatT NetGameLinkStatT;
#endif

struct NetGameLinkRefT {
    int iopref;                     // 0x000 [R] the 'crea' reply (the IOP's link reference) lands here; [U] Destroy,
                                    //       Status, Recv, Send and Control pass it with their commands
    int statstate;                  // 0x004 [R] -1 (no status read yet); [U] NetGameLinkStatus: -1 reads it once
                                    //       synchronously, 0 sends 'stat' and sets 1; NetGameLink_completeStatus (the reply) sets 0
    char stat[0x150];               // 0x008 [U] NetGameLink_completeStatus copies the 336-byte status area in; NetGameLinkStatus
                                    //       returns it (On Tour's NetGameLinkStatT is 0x150 too)
    char* statbuf;                  // 0x158 [R] the status area the IOP fills
    int fillid;                     // 0x15C [U] NetGameLinkRecv: the receive-buffer fill last seen
    int readpos;                    // 0x160 [U] NetGameLinkPeek / NetGameLinkRecv: the read offset in recvbuf
    int readend;                    // 0x164 [U] the end of the received data; NetGameLinkRecv sets -1 (empty) and asks
                                    //       the IOP for more ('recv')
    int recvsize;                   // 0x168 [R] the receive buffer's size (n)
    char* recvbuf;                  // 0x16C [R] the receive buffer
    int recvtick;                   // 0x170 [U] NetGameLinkRecv: NetTick when it asked for more data
    int sendwindow;                 // 0x174 [R] n; [U] NetGameLinkSend: the room in the IOP's send buffer
    int sendpos;                    // 0x178 [U] NetGameLinkSend: the bytes sent so far
    int* sendreadpos;               // 0x17C [R] the word where the IOP reports how far it has read the sent data;
                                    //       [U] NetGameLinkSend reads it
};

CHECK_OFFSET(NetGameLinkRefT, iopref, 0x000);
CHECK_OFFSET(NetGameLinkRefT, statstate, 0x004);
CHECK_OFFSET(NetGameLinkRefT, stat, 0x008);
CHECK_OFFSET(NetGameLinkRefT, statbuf, 0x158);
CHECK_OFFSET(NetGameLinkRefT, fillid, 0x15C);
CHECK_OFFSET(NetGameLinkRefT, readpos, 0x160);
CHECK_OFFSET(NetGameLinkRefT, readend, 0x164);
CHECK_OFFSET(NetGameLinkRefT, recvsize, 0x168);
CHECK_OFFSET(NetGameLinkRefT, recvbuf, 0x16C);
CHECK_OFFSET(NetGameLinkRefT, recvtick, 0x170);
CHECK_OFFSET(NetGameLinkRefT, sendwindow, 0x174);
CHECK_OFFSET(NetGameLinkRefT, sendpos, 0x178);
CHECK_OFFSET(NetGameLinkRefT, sendreadpos, 0x17C);
CHECK_SIZE(NetGameLinkRefT, 0x180);

// The head of a game-link packet (0x4; name ours). Packets sit back to back in the link's buffers with no alignment,
// so the head is a byte record (copied with lwl / lwr). [U] its first halfword is the packet's byte length:
// NetGameLinkPeek copies the head out and then the whole packet, NetGameLinkRecv moves readpos on by it,
// NetGameLinkSend computes it from byte 3 ((byte 3 + 0x17) & 0x7FFC) when called with length 1.
struct NetGameLinkPacketHead {
    unsigned char len[2];           // 0x0 [U] the packet's byte length
    unsigned char info[2];          // 0x2 [U] byte 3: NetGameLinkSend's length code
};

CHECK_OFFSET(NetGameLinkPacketHead, len, 0x0);
CHECK_OFFSET(NetGameLinkPacketHead, info, 0x2);
CHECK_SIZE(NetGameLinkPacketHead, 0x4);

// One ping-history slot of the link status. [O] On Tour's NetGameLinkHistT (0x8; its min / max are spelled
// __dont_use_min_use_MIN__ / __dont_use_max_use_MAX__ there, DirtySock's names mangled by MIN / MAX macros).
struct NetGameLinkHistT {
    unsigned short min;             // 0x0 [O]
    unsigned short max;             // 0x2 [O]
    unsigned short avg;             // 0x4 [O]
    unsigned short cnt;             // 0x6 [O]
};

// The link status NetGameLinkStatus returns (NetGameLinkRefT::stat, 336 bytes). [O] On Tour's NetGameLinkStatT
// (0x150, the same size), names and types (On Tour's tick / tickseqn are volatile). [U] cDSockChannel_updateConnected
// reads tick - rcvdlast (a gap over 30000 times the link out), isopen (0: the link is lost) and late (the latency in
// ms).
struct NetGameLinkStatT {
    unsigned int tick;              // 0x000 [O]; [U] updateConnected: the link's current time
    unsigned int tickseqn;          // 0x004 [O]
    unsigned int ping;              // 0x008 [O]
    unsigned int late;              // 0x00C [O]; [U] updateConnected: the latency (ms) it smooths into mLatency
    unsigned int conn;              // 0x010 [O]
    unsigned int sent;              // 0x014 [O]
    unsigned int rcvd;              // 0x018 [O]
    unsigned int sentlast;          // 0x01C [O]
    unsigned int rcvdlast;          // 0x020 [O]; [U] updateConnected: the time of the last receive
    unsigned int sentshow;          // 0x024 [O]
    unsigned int rcvdshow;          // 0x028 [O]
    unsigned int senthide;          // 0x02C [O]
    unsigned int rcvdhide;          // 0x030 [O]
    unsigned int bps;               // 0x034 [O]
    unsigned int pps;               // 0x038 [O]
    unsigned int rps;               // 0x03C [O]
    unsigned int stattick;          // 0x040 [O]
    unsigned char isconn;           // 0x044 [O]
    unsigned char isopen;           // 0x045 [O]; [U] updateConnected disconnects the channel when it is 0
    unsigned char _unused1;         // 0x046 [O]
    unsigned char _unused2;         // 0x047 [O]
    unsigned int pingtick;          // 0x048 [O]
    unsigned int pingslot;          // 0x04C [O]
    struct NetGameLinkHistT pinghist[32]; // 0x050 [O]
};

CHECK_OFFSET(NetGameLinkStatT, tick, 0x000);
CHECK_OFFSET(NetGameLinkStatT, late, 0x00C);
CHECK_OFFSET(NetGameLinkStatT, rcvdlast, 0x020);
CHECK_OFFSET(NetGameLinkStatT, isopen, 0x045);
CHECK_OFFSET(NetGameLinkStatT, pinghist, 0x050);
CHECK_SIZE(NetGameLinkStatT, 0x150);

#endif /* EALIB_NETGAMELINK_H */
