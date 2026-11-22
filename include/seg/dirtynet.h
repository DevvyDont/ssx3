#ifndef SEG_DIRTYNET_H
#define SEG_DIRTYNET_H

// HostentT: DirtySock's asynchronous host-name lookup (DirtyNet's SocketLookup(text, timeout) returns one; the caller
// polls Done until it is non-zero, reads addr, then calls Free). Retail code: unit seg/seg_2F41F8 (SocketLookup ->
// func_003F39C8, _SocketLookupDone, _SocketLookupFree; the lookup itself runs on the IOP). Users: BuddyApi's connect
// (func_003F08E8, BuddyApiRefT.mpHostLookup in seg/buddyapi.h) and LobbyMan's connect (online/lobbyman.h mpHostLookup).
//
// Name and the public members: [O] SSX On Tour's DWARF struct HostentT (0x5C) {int done; unsigned int addr;
// int (*Done)(); void (*Free)(); char name[64]; int sema; int thread; unsigned int timeout;} with SocketLookup /
// _SocketLookupDone(HostentT*) / _SocketLookupFree(HostentT*) (DirtySock dirtynetpsp*.c); [R] retail func_003F39C8
// allocates the same 92 bytes (RpcAlloc(92), zero-filled) and stores _SocketLookupDone at +0x8 and _SocketLookupFree at
// +0xC. The PS2 build keeps the IOP's lookup id at +0x58 (On Tour's PSP build has its own thread data in 0x10..0x5C);
// 0x10..0x57 is not touched on the EE.
//
// C struct, flat. Evidence tags: [O] On Tour's DWARF; [R] func_003F39C8 / _SocketLookupDone / _SocketLookupFree;
// [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct HostentT HostentT;
#endif

struct HostentT {
    int done;                           // 0x00 [O]; [R] _SocketLookupDone: the IOP's 'name'/'done' answer (> 0 resolved,
                                        //      < 0 failed); [U] the return value of Done
    unsigned int addr;                  // 0x04 [O]; [R] _SocketLookupDone fetches 'name'/'addr' into it once resolved;
                                        //      [U] func_003F17D8 connects to it
    int (*Done)(struct HostentT* host); // 0x08 [O]; [R] = _SocketLookupDone; [U] func_003F17D8 polls it
    void (*Free)(struct HostentT* host);// 0x0C [O]; [R] = _SocketLookupFree; [U] func_003F07A0 / func_003F0B50 /
                                        //      func_003F17D8 release a lookup through it
    char unk_10[0x48];                  // 0x10 [O] On Tour: name[64], sema, thread (PSP lookup thread); not touched on
                                        //      the PS2's EE side
    int unk_58;                         // 0x58 [R] the IOP's lookup id: func_003F39C8 stores the 'name'/'look' reply,
                                        //      _SocketLookupDone / _SocketLookupFree send it back (On Tour: timeout)
};

CHECK_OFFSET(HostentT, done, 0x00);
CHECK_OFFSET(HostentT, addr, 0x04);
CHECK_OFFSET(HostentT, Done, 0x08);
CHECK_OFFSET(HostentT, Free, 0x0C);
CHECK_OFFSET(HostentT, unk_58, 0x58);
CHECK_SIZE(HostentT, 0x5C);

#endif /* SEG_DIRTYNET_H */
