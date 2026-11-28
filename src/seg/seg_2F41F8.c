#include "common.h"
#include "seg/dirtynet.h"
#include "ui/virtualkey.h"

//100%
INCLUDE_ASM("seg/seg_2F41F8", LobbyMd5_final);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char *LobbyMd5_hexDigits;
extern void f2220(void *) __asm__("LobbyMd5_transform");
typedef struct { unsigned count; unsigned st[4]; unsigned char buf[64]; } sMd5;
/**
 * Finishes an MD5 hash and writes the digest as 16 raw bytes or as 32 lower-case hex characters.
 *
 * Pads the buffered block with 0x80 and zeros up to byte 56 (hashing a full block with LobbyMd5_transform on the way),
 * stores the message length in bits (byte count << 3) little-endian in bytes 56..63 and hashes the last block. Then
 * walks the 16 digest bytes of the state words: @p n == 16 writes them raw; @p n > 32 writes two hex digits each
 * (digits from LobbyMd5_hexDigits) and a NUL. Likely bug: for any other positive @p n the zero-fill loop never decrements
 * @p n, so it writes zeros forever.
 *
 * @param ctx  the MD5 context
 * @param out  digest output
 * @param n  output size: 16 for raw bytes, more than 32 for hex text
 */
void LobbyMd5_final(sMd5 *ctx, unsigned char *out, int n) {
    unsigned char fillByte = 0x80;
    unsigned w = 0;
    int i;
    i = ctx->count & 0x3F;
    do {
        ctx->buf[i] = fillByte;
        i++;
        fillByte = 0;
        if (i == 0x40) {
            f2220(ctx);
            i = 0;
        }
    } while (i != 0x38);
    ctx->buf[0x3D] = 0;
    ctx->buf[0x38] = (unsigned char)ctx->count << 3;
    ctx->buf[0x3E] = 0;
    ctx->buf[0x3F] = 0;
    ctx->buf[0x39] = ctx->count >> 5;
    ctx->buf[0x3A] = ctx->count >> 13;
    ctx->buf[0x3B] = ctx->count >> 21;
    ctx->buf[0x3C] = ctx->count >> 29;
    f2220(ctx);
    for (i = 0; i < 16; i++) {
        if ((i & 3) == 0) w = ctx->st[i >> 2];
        if (n == 16) {
            *out = w;
            out++;
        } else if (n > 32) {
            *out = LobbyMd5_hexDigits[(w >> 4) & 0xF];
            out++;
            *out = LobbyMd5_hexDigits[w & 0xF];
            out++;
        }
        w >>= 8;
    }
    if (n > 32) {
        *out = 0;
    } else if (n != 16) {
        while (n > 0) {
            *out = 0;
            out++;
        }
    }
}
#endif

/* The state of the CRC-mixed byte-permutation stream cipher (LobbyCrypt_setKey keys it, LobbyCrypt_apply runs it): [U] both
   read the permutation at +0x0, the index byte at +0x100 and the CRC word at +0x104; [R] the shared state LobbyCrypt_sharedState
   is 0x108 bytes (DirtyNet_lookupDoneService follows it). */
struct sCipherState {
    unsigned char perm[256];            /* 0x000: byte permutation */
    unsigned char index;                /* 0x100: keystream position */
    unsigned crc;                       /* 0x104: CRC-32 of the bytes mixed so far */
};

//100%
INCLUDE_ASM("seg/seg_2F41F8", LobbyCrypt_setKey);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* Globals used by several functions. */
extern unsigned LobbyCrypt_crcTable[];

extern struct sCipherState LobbyCrypt_sharedState;

extern int D_004A4B9C;
extern unsigned char str_hello_world[];
/**
 * Keys a CRC-mixed byte-permutation stream cipher state (0x108 bytes: 256-byte permutation, index byte at +0x100, CRC
 * word at +0x104).
 *
 * On first use it also keys the shared state LobbyCrypt_sharedState with "hello world", 10 rounds. A non-negative @p rounds resets
 * the state to the identity permutation with index and CRC 0; a negative one keeps the current state and uses -@p
 * rounds. Then, for rounds * 256 steps, it mixes the next key byte and the current permutation byte into a CRC-32
 * (table LobbyCrypt_crcTable) and swaps the current slot with the slot the CRC's low byte names. A negative @p klen means
 * @p key is NUL-terminated; an empty key leaves the permutation as it is.
 *
 * @param cipherState  the cipher state
 * @param key  key bytes
 * @param klen  key length, or negative for a C string
 * @param rounds  number of 256-step passes; negative continues without resetting
 */
void LobbyCrypt_setKey(struct sCipherState *cipherState, unsigned char *key, int klen, int rounds) {
    int i;
    int lim;
    unsigned crc;

    if (D_004A4B9C == 0) {
        D_004A4B9C = 1;
        LobbyCrypt_setKey(&LobbyCrypt_sharedState, str_hello_world, -1, 10);
    }
    i = 0xFF;
    if (rounds < 0) {
        rounds = -rounds;
    } else {
        unsigned char *permSlot = &cipherState->perm[0xFF];
        for (; i >= 0; i--) *permSlot-- = i;
        cipherState->index = 0;
        cipherState->crc = 0;
    }
    if (klen < 0) {
        klen = 0;
        while (key[klen] != 0) klen++;
    }
    if (klen > 0) {
        crc = 0;
        i = 0;
        lim = rounds << 8;
        if (lim > 0) {
            do {
                unsigned char *permSlot;
                unsigned char *swapSlot;
                unsigned char swapByte;
                int keyIndex = i % klen;
                permSlot = &cipherState->perm[i & 0xFF];
                swapByte = *permSlot;
                i++;
                crc = (crc >> 8) ^ LobbyCrypt_crcTable[(crc & 0xFF) ^ key[keyIndex]];
                crc = (crc >> 8) ^ LobbyCrypt_crcTable[(crc & 0xFF) ^ swapByte];
                swapSlot = &cipherState->perm[crc & 0xFF];
                *permSlot = *swapSlot;
                *swapSlot = swapByte;
            } while (i < lim);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", LobbyCrypt_apply);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* Globals used by several functions. */
extern unsigned LobbyCrypt_crcTable[];

/**
 * XORs @p n bytes of a buffer with the keystream of the cipher state keyed by LobbyCrypt_setKey.
 *
 * Per byte: advances the index (+0x100), folds the permutation byte there into the CRC word (+0x104) with the CRC-32
 * table LobbyCrypt_crcTable, swaps the permutation bytes at the index and at the CRC's low byte, and XORs the buffer byte with
 * the permutation byte at the difference of the two swapped values. Saves the index and CRC back.
 *
 * @param cipherState  the cipher state
 * @param buf  bytes to encrypt or decrypt in place
 * @param n  byte count
 */
void LobbyCrypt_apply(struct sCipherState *cipherState, unsigned char *buf, int n) {
    unsigned char i = cipherState->index;
    unsigned j = cipherState->crc;
    for (; n > 0; n--) {
        unsigned char valueAtIndex, valueAtCrc;
        j = (j >> 8) ^ LobbyCrypt_crcTable[(j & 0xFF) ^ cipherState->perm[i++]];
        valueAtIndex = cipherState->perm[i];
        valueAtCrc = cipherState->perm[j & 0xFF];
        cipherState->perm[i] = valueAtCrc;
        cipherState->perm[j & 0xFF] = valueAtIndex;
        *buf++ ^= cipherState->perm[(unsigned char)(valueAtIndex - valueAtCrc)];
    }
    cipherState->index = i;
    cipherState->crc = j;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", LobbyCrypt_encodeText);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct sCipherState LobbyCrypt_sharedState;

extern void f33B0w(struct sCipherState *, unsigned char *, int, int) __asm__("LobbyCrypt_setKey");
extern void f3530w(struct sCipherState *, unsigned char *, int) __asm__("LobbyCrypt_apply");
extern unsigned char str_ru_paranoid[];
/**
 * Encrypts a text into a fixed-length printable string (the inverse of LobbyCrypt_decodeText).
 *
 * Keys a local cipher state with the key (LobbyCrypt_setKey), and also stirs the key and "ru paranoid?" into the shared
 * state LobbyCrypt_sharedState. Writes @p n - 1 characters and a NUL: each input character (anything outside 0x20..0x7E becomes
 * 0x7F, which also marks the end of the input) is shifted by a keystream byte mod 96 into the range 0x20..0x7F; after
 * the input's NUL the rest is random padding drawn from the shared state, so the output length does not reveal the
 * text's. LobbyApi_sendCommand uses it to send the PASS field as "~" + this encoding under the 16-byte session key.
 *
 * @param out  output buffer
 * @param n  output buffer size
 * @param in  text to encrypt (may equal @p out)
 * @param cipherKey  key bytes
 * @param cipherKeyLen  key length, or negative for a C string
 * @param cipherRounds  key rounds
 * @return 0
 */
int LobbyCrypt_encodeText(char *out, int n, unsigned char *in, unsigned char *cipherKey, int cipherKeyLen, int cipherRounds) {
    struct sCipherState ctx;
    unsigned char ch;
    unsigned char k;
    k = 0;
    f33B0w(&ctx, cipherKey, cipherKeyLen, cipherRounds);
    f33B0w(&LobbyCrypt_sharedState, cipherKey, cipherKeyLen, -cipherRounds);
    f33B0w(&LobbyCrypt_sharedState, str_ru_paranoid, -1, -1);
    for (; n > 1; n--) {
        if (in == 0) {
            f3530w(&LobbyCrypt_sharedState, &ch, 1);
            ch = (ch & 0x3F) + 0x20;
        } else {
            ch = *in;
            in++;
            if (ch == 0) in = 0;
        }
        if ((unsigned)(ch - 0x20) > 94) ch = 0x7F;
        f3530w(&ctx, &k, 1);
        *out = (ch + 0x40 + (int)(k % 0x60U)) % 0x60 + 0x20;
        out++;
    }
    if (n > 0) *out = 0;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", LobbyCrypt_decodeText);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void f33B0(void *, int, int, int) __asm__("LobbyCrypt_setKey");
extern void f3530(void *, unsigned char *, int) __asm__("LobbyCrypt_apply");
/**
 * Decrypts a string made by LobbyCrypt_encodeText.
 *
 * Keys a local cipher state (LobbyCrypt_setKey) and shifts each input character back by a keystream byte mod 96, stopping
 * at the input's NUL, at a decoded 0x7F (the end marker) or after @p n - 1 characters, then NUL-terminates. Nothing in
 * retail calls it.
 *
 * @param out  output buffer
 * @param n  output buffer size
 * @param in  encrypted text
 * @param key  key bytes
 * @param keyLen  key length, or negative for a C string
 * @param rounds  key rounds
 * @return 0
 */
int LobbyCrypt_decodeText(char *out, int n, unsigned char *in, int key, int keyLen, int rounds) {
    unsigned char ctx[272];
    unsigned char k;
    int i;
    int ch;
    int cur;
    unsigned shift;
    k = 0;
    f33B0(ctx, key, keyLen, rounds);
    i = 0;
    while (i < n - 1) {
        cur = in[i];
        i++;
        if (cur == 0) break;
        f3530(ctx, &k, 1);
        shift = k % 96U - 0x40;
        ch = (int)(cur - shift) % 96 + 0x20;
        *out = ch;
        if ((char)ch == 0x7F) break;
        out++;
    }
    if (i < n) *out = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", _SocketLookupDone);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char DirtyNet_lookupDoneService[];
extern int NetLib_resolveService(char *, int, int);
extern int f4C68g(char *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
/**
 * Polls a host-name lookup made by DirtyNet_startHostLookup (its "done" function, slot +0x8).
 *
 * Asks the IOP 'name'/'done' for the lookup id at +0x58 and waits for the answer into +0x0; when that is positive
 * (resolved) it also fetches 'name'/'addr' into +0x4.
 *
 * @return 0 while pending, positive when resolved, negative on failure
 */
int _SocketLookupDone(int *self) {
    int id = self[88 / 4];
    NetLib_resolveService(DirtyNet_lookupDoneService, 'name', 'done');
    f4C68g(DirtyNet_lookupDoneService, 1, id, 0, 0, 0, 0, 0, 1, self);
    if (self[0] > 0) {
        NetLib_resolveService(DirtyNet_lookupDoneService, 'name', 'addr');
        f4C68g(DirtyNet_lookupDoneService, 1, id, 0, 0, 0, 0, 0, 1, self + 1);
    }
    return self[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", _SocketLookupFree);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char DirtyNet_lookupFreeService[];
extern int NetLib_resolveService(char *, int, int);
extern void RpcFree(void *);
/**
 * Frees a host-name lookup made by DirtyNet_startHostLookup (its "free" function, slot +0xC).
 *
 * Sends 'name'/'free' for the lookup id at +0x58 without waiting, then frees the object with RpcFree.
 */
void _SocketLookupFree(int *self) {
    int lookupId = self[88 / 4];
    NetLib_resolveService(DirtyNet_lookupFreeService, 'name', 'free');
    NetLib_callService(DirtyNet_lookupFreeService, 1, lookupId, 0, 0, 0, 0, 0, 0, 0);
    RpcFree(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", DirtyNet_startHostLookup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char DirtyNet_lookupService[];
extern void *RpcAlloc(int);
extern void f6210k(int *, int, int) __asm__("memset");
extern int NetLib_resolveService(char *, int, int);
extern int f4C68k(char *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern int _SocketLookupDone(int *);
extern void _SocketLookupFree(int *);
int *f39C8impl(int a, int b) __asm__("DirtyNet_startHostLookup");
/**
 * Starts an asynchronous host-name lookup on the IOP and returns its lookup object.
 *
 * Allocates a zeroed 92-byte object with RpcAlloc, installs _SocketLookupDone as its done function (+0x8) and _SocketLookupFree
 * as its free function (+0xC), and sends 'name'/'look' with the host name and the timeout, waiting for the IOP's lookup
 * id into +0x58. The caller polls the done function until it is nonzero, then reads the address at +0x4 and calls the
 * free function.
 *
 * @param hostName  host name text
 * @param timeout  timeout (passed through to the IOP)
 * @return the lookup object, or 0 when the allocation fails
 */
int *f39C8impl(int hostName, int timeout) {
    int word;
    int *lookup = RpcAlloc(92);
    if (lookup) {
        f6210k(lookup, 0, 92);
        word = (int)_SocketLookupDone;
        lookup[3] = (int)_SocketLookupFree;
        lookup[2] = word;
        NetLib_resolveService(DirtyNet_lookupService, 'name', 'look');
        word = 1;
        f4C68k(DirtyNet_lookupService, word, 0, timeout, hostName, -word, 0, 0, word, lookup + 88 / 4);
    }
    return lookup;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", SocketLookup);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern HostentT *DirtyNet_startHostLookup(void);
/**
 * Starts a host-name lookup: forwards both arguments to DirtyNet_startHostLookup and returns its lookup object.
 *
 * Used by BuddyApi's connect (BuddyApi_connect) and by LobbyMan; both poll the object's done function (slot +0x8) and free
 * it through slot +0xC.
 *
 * @param hostName  host name text
 * @param timeoutMs  timeout
 * @return the lookup object, or 0
 */
HostentT *SocketLookup(void* hostName, int timeoutMs) { return DirtyNet_startHostLookup(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", DirtyNet_lookupHostBlocking);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern HostentT *DirtyNet_startHostLookup(void);
/**
 * Resolves a host name synchronously: starts a lookup (DirtyNet_startHostLookup, arguments passed through), spins on its done
 * function until it is nonzero, frees it and returns the address.
 *
 * Nothing in retail calls it.
 *
 * @return the resolved address, or 0 when the lookup could not be started (also 0 when the lookup failed)
 */
int DirtyNet_lookupHostBlocking(void) {
    HostentT *hostLookup;
    int resolvedAddr = 0;
    hostLookup = DirtyNet_startHostLookup();
    if (hostLookup) {
        while (hostLookup->Done(hostLookup) == 0)
            ;
        resolvedAddr = hostLookup->addr;
        hostLookup->Free(hostLookup);
    }
    return resolvedAddr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnStartup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetConn_startedFlag;

extern int NetConn_moduleId;

extern int NetLib_isQueueStarted(void);
extern void f5000(int) __asm__("NetLib_startQueue");
extern int NetLib_fetchServiceDirectory(void);
extern int NetLib_resolveService(char *, int, int);
extern int f4C68e(char *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
/**
 * Starts the IOP network module ('netw'/'crea') once.
 *
 * Returns -1 when it is already open (NetConn_startedFlag). If the IOP request queue is not running (NetLib_isQueueStarted) it starts it
 * (NetLib_startQueue) and fetches the IOP's service directory (NetLib_fetchServiceDirectory). Then sends 'netw'/'crea' with the parameter
 * text and the argument '1731', waits for the module handle into NetConn_moduleId and sets NetConn_startedFlag. The cCommSystem
 * constructor calls it with "www.ea.com".
 *
 * @param startupParams  startup parameter text
 * @return 0, or -1 when already started
 */
int NetConnStartup(int startupParams) {
    char buf[16];
    if (NetConn_startedFlag != 0) return -1;
    if (NetLib_isQueueStarted() <= 0) {
        f5000(0);
        NetLib_fetchServiceDirectory();
    }
    NetLib_resolveService(buf, 'netw', 'crea');
    f4C68e(buf, 1, 0, '1731', startupParams, -1, 0, 0, 1, &NetConn_moduleId);
    NetConn_startedFlag = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnQuery);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetConn_moduleId;

extern int NetLib_resolveService(char *, int, int);
extern int func_003F4C68b(char *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
/**
 * Fetches the list of network configurations from the IOP ('netw'/'list').
 *
 * Sends the device text (@p self) and waits for up to @p maxConfigs 0x100-byte entries into @p configList (flags 5: kick the IOP and
 * write back the buffer). cCommSystem fills cCommSystem_mNetConns (10 entries) with it.
 *
 * @param configList  output list of 0x100-byte configuration entries
 * @param maxConfigs  capacity in entries
 * @return the number of entries
 */
int NetConnQuery(int self, int configList, int maxConfigs) {
    char buf[16];
    int out;
    NetLib_resolveService(buf, 'netw', 'list');
    func_003F4C68b(buf, 5, NetConn_moduleId, 0, self, -1, configList, maxConfigs << 8, 1, &out);
    return out;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnConnect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: `if (a > 0)` around an identical copy of the body: the target has no compare; jump2 merges the arms and the leftover block boundary holds sched2 back; straight-line form 78.57%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int NetConn_moduleId;

extern int NetLib_resolveService(char *, int, int);
extern int f4C68s(char *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
/**
 * Starts connecting with a network configuration ('netw'/'star').
 *
 * Sends the configuration text (a NUL-terminated string) and waits for the IOP's result. Both arms of the original
 * branch on @p configName are identical (see the FAKEMATCH note). cCommSystem passes the text at +0x80 of the chosen
 * cCommSystem_mNetConns entry.
 *
 * @param configName  configuration text
 * @return the IOP's result
 */
// NOTE: retail had a branch on `a` whose two paths were identical (jump2
// cross-jumped them after sched2, leaving only a block boundary before the
// 'netw'/'star' setup). The original condition is unknown.
int NetConnConnect(int configName) {
    char buf[16];
    int out;
    if (configName > 0) {
        NetLib_resolveService(buf, 'netw', 'star');
        f4C68s(buf, 1, NetConn_moduleId, 0, configName, -1, 0, 0, 1, &out);
        return out;
    }
    NetLib_resolveService(buf, 'netw', 'star');
    f4C68s(buf, 1, NetConn_moduleId, 0, configName, -1, 0, 0, 1, &out);
    return out;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnStatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetConn_startedFlag;

extern int NetConn_moduleId;

extern int NetLib_resolveService(char *, int, int);
extern int f4C68m(char *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
/**
 * Answers a network-status query by its four-character selector.
 *
 * 'open' returns the started flag NetConn_startedFlag without asking the IOP. The others send 'netw'/'stat' with a selector and
 * wait for the answer: 'onln' is whether 'host' is nonzero; 'conn' (flags 5, with @p statusBuf / @p statusBufSize as a reply buffer)
 * returns the connection state; 'host' and 'addr' return those words; 'macx' copies the MAC address into @p statusBuf and
 * returns 0; 'type' returns the interface type word and 'bbnd' its bit 2 (broadband). Any other selector returns -1.
 *
 * @param tag  selector ('open', 'onln', 'conn', 'host', 'addr', 'macx', 'type', 'bbnd')
 * @param statusBuf  reply buffer for 'conn', 'macx' and 'type'
 * @param statusBufSize  reply buffer size
 * @return the selected status, or -1 for an unknown selector
 */
int NetConnStatus(int tag, int statusBuf, int statusBufSize) {
    char buf[16];
    int statusReply;
    int addrReply;
    if (tag == 'open') {
        return NetConn_startedFlag;
    }
    if (tag == 'onln') {
        NetLib_resolveService(buf, 'netw', 'stat');
        f4C68m(buf, 1, NetConn_moduleId, 'host', 0, 0, 0, 0, 1, &statusReply);
        return statusReply != 0;
    }
    if (tag == 'conn') {
        NetLib_resolveService(buf, 'netw', 'stat');
        f4C68m(buf, 5, NetConn_moduleId, 'conn', 0, 0, statusBuf, statusBufSize, 1, &statusReply);
        return statusReply;
    }
    if (tag == 'host') {
        NetLib_resolveService(buf, 'netw', 'stat');
        f4C68m(buf, 1, NetConn_moduleId, 'host', 0, 0, 0, 0, 1, &addrReply);
        return addrReply;
    }
    if (tag == 'addr') {
        NetLib_resolveService(buf, 'netw', 'stat');
        f4C68m(buf, 1, NetConn_moduleId, 'addr', 0, 0, 0, 0, 1, &addrReply);
        return addrReply;
    }
    if (tag == 'macx') {
        NetLib_resolveService(buf, 'netw', 'stat');
        NetLib_callService(buf, 5, NetConn_moduleId, 'macx', 0, 0, statusBuf, statusBufSize, 1, 0);
        return 0;
    }
    if (tag == 'bbnd' || tag == 'type') {
        NetLib_resolveService(buf, 'netw', 'stat');
        f4C68m(buf, 1, NetConn_moduleId, 'type', 0, 0, statusBuf, statusBufSize, 1, &statusReply);
        if (tag == 'bbnd') {
            statusReply = (statusReply >> 2) & 1;
        }
        return statusReply;
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", NetConnMAC);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char NetConn_macText[];
extern const char str_0123456789abcdefUSBD_IRX[];
extern void f3CC0(int, unsigned char *, int) __asm__("NetConnStatus");
/**
 * Returns the console's MAC address as text: "$" followed by twelve lower-case hex digits.
 *
 * Built once into the static buffer NetConn_macText from NetConnStatus('macx'); later calls return the cached text. LobbyMan
 * sends it as the MID tag and cCommSystem copies it.
 *
 * @return the static MAC text
 */
char *NetConnMAC(void) {
    unsigned char macBytes[16];
    if (NetConn_macText[0] == 0) {
        f3CC0('macx', macBytes, 0x10);
        NetConn_macText[1] = str_0123456789abcdefUSBD_IRX[macBytes[0] >> 4];
        NetConn_macText[2] = str_0123456789abcdefUSBD_IRX[macBytes[0] & 0xF];
        NetConn_macText[3] = str_0123456789abcdefUSBD_IRX[macBytes[1] >> 4];
        NetConn_macText[4] = str_0123456789abcdefUSBD_IRX[macBytes[1] & 0xF];
        NetConn_macText[5] = str_0123456789abcdefUSBD_IRX[macBytes[2] >> 4];
        NetConn_macText[6] = str_0123456789abcdefUSBD_IRX[macBytes[2] & 0xF];
        NetConn_macText[7] = str_0123456789abcdefUSBD_IRX[macBytes[3] >> 4];
        NetConn_macText[8] = str_0123456789abcdefUSBD_IRX[macBytes[3] & 0xF];
        NetConn_macText[9] = str_0123456789abcdefUSBD_IRX[macBytes[4] >> 4];
        NetConn_macText[10] = str_0123456789abcdefUSBD_IRX[macBytes[4] & 0xF];
        NetConn_macText[11] = str_0123456789abcdefUSBD_IRX[macBytes[5] >> 4];
        NetConn_macText[12] = str_0123456789abcdefUSBD_IRX[macBytes[5] & 0xF];
        NetConn_macText[0] = '$';
        NetConn_macText[13] = 0;
    }
    return NetConn_macText;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnDisconnect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetConn_moduleId;

extern int NetLib_resolveService(char *, int, int);
extern int NetLib_callService(char *, int, int, int, int, int, int, int, int, int);
/**
 * Asks the IOP network module to disconnect ('netw'/'shut') and waits for it.
 *
 * cCommSystem calls it when it drops its net connection; NetConnShutdown calls it before destroying the module.
 *
 * @return 0
 */
int NetConnDisconnect(void) {
    char buf[16];
    NetLib_resolveService(buf, 'netw', 'shut');
    NetLib_callService(buf, 1, NetConn_moduleId, 0, 0, 0, 0, 0, 1, 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnShutdown);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetConn_startedFlag;

extern int NetConn_moduleId;

extern int NetConnDisconnect(void);
extern int NetLib_resolveService(char *, int, int);
extern int NetLib_callService(char *, int, int, int, int, int, int, int, int, int);
/**
 * Shuts the IOP network module down: disconnects (NetConnDisconnect), sends 'netw'/'dest' and clears the started flag
 * NetConn_startedFlag.
 *
 * Nothing in retail calls it.
 *
 * @return 0, or -1 when the module was not started
 */
int NetConnShutdown(void) {
    char buf[16];
    if (NetConn_startedFlag == 0) return -1;
    NetConnDisconnect();
    NetLib_resolveService(buf, 'netw', 'dest');
    NetLib_callService(buf, 1, NetConn_moduleId, 0, 0, 0, 0, 0, 1, 0);
    NetConn_startedFlag = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4198);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetTick(void);
/**
 * Returns the millisecond tick (NetTick).
 *
 * Nothing in retail calls it.
 *
 * @return milliseconds since the IOP queue's timer started
 */
int func_003F4198(void) { return NetTick(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnSleep);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int NetTicku(void) __asm__("NetTick");
/**
 * Busy-waits for @p n milliseconds of NetTick ticks (0 means 5).
 *
 * Nothing in retail calls it.
 *
 * @param n  milliseconds to wait
 */
void NetConnSleep(int n) {
    if (n == 0) n = 5;
    if (n > 0) {
        unsigned int end = NetTicku() + n;
        while (NetTicku() < end) NetTicku();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnIdleAdd);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* _NETidle: the periodic callbacks run by NetConnIdle (fn 0 = free slot). */
struct sTick { void (*fn)(int, int); int arg; };

extern struct sTick _NETidle[16];

/**
 * Registers an idle callback for NetConnIdle in the first free of the 16 slots of _NETidle.
 *
 * NetConnIdle calls each registered function with its argument and the current tick.
 *
 * @param fn  the callback
 * @param procData  its argument
 * @return 0, or -1 when all slots are taken
 */
int NetConnIdleAdd(void (*fn)(int, int), int procData) {
    int i;
    struct sTick *idleSlot = _NETidle;
    for (i = 0; i < 16; i++) {
        if (idleSlot->fn == 0) {
            idleSlot->fn = fn;
            idleSlot->arg = procData;
            return 0;
        }
        idleSlot++;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnIdleDel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* _NETidle: the periodic callbacks run by NetConnIdle (fn 0 = free slot). */
extern struct sTick _NETidle[16];

/**
 * Removes an idle callback registered with NetConnIdleAdd (the slot whose function and argument both match).
 *
 * @param fn  the callback
 * @param procData  its argument
 * @return 0, or -1 when no slot matches
 */
int NetConnIdleDel(void (*fn)(int, int), int procData) {
    int i;
    struct sTick *idleSlot = _NETidle;
    for (i = 0; i < 16; i++) {
        if (idleSlot->fn == fn && idleSlot->arg == procData) {
            idleSlot->fn = 0;
            idleSlot->arg = 0;
            return 0;
        }
        idleSlot++;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnIdle);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* _NETidle: the periodic callbacks run by NetConnIdle (fn 0 = free slot). */
extern struct sTick _NETidle[16];

extern int NetConn_startedFlag;

extern int NetTick(void);

extern unsigned _NetConn_uNextIdleTick;
extern int NetLib_completeReplies(void);
/**
 * Runs DirtySock's idle processing, at most every 5 ticks while the network connection is up.
 *
 * When NetConn_startedFlag is set it returns early until NetTick (the tick) reaches _NetConn_uNextIdleTick and then sets that to tick
 * + 5. Then NetLib_completeReplies runs and each of the 16 idle callbacks in _NETidle with a function is called with its
 * argument and the current tick. On Tour checks NetConnStatus('open') instead of the flag and runs the callbacks in
 * NetConnIdleProcs.
 */
void NetConnIdle(void) {
    int i;
    struct sTick *idleProc;
    if (NetConn_startedFlag != 0) {
        if (NetTick() < _NetConn_uNextIdleTick) return;
        _NetConn_uNextIdleTick = NetTick() + 5;
    }
    NetLib_completeReplies();
    idleProc = _NETidle;
    for (i = 0xF; i >= 0; i--) {
        if (idleProc->fn) idleProc->fn(idleProc->arg, NetTick());
        idleProc++;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4330);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetTick(void);
/**
 * Returns the millisecond tick (NetTick).
 *
 * LobbyApi code (LobbyApi_onServerPing) uses it to time its pings.
 *
 * @return milliseconds since the IOP queue's timer started
 */
int func_003F4330(void) { return NetTick(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetCompare);
#ifdef SKIP_ASM
/**
 * Compares two byte strings: @p n bytes, or up to the NUL of @p lhs when @p n is negative.
 *
 * Nothing in retail calls it.
 *
 * @param lhs  first string
 * @param rhs  second string
 * @param n  byte count, or negative for C strings
 * @return the difference of the first differing bytes, or 0 when equal
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int NetCompare(unsigned char *lhs, unsigned char *rhs, int n) {
    int diff;
    if (n < 0) {
        while (*lhs) {
            if (*lhs != *rhs)
                return *lhs - *rhs;
            lhs++;
            rhs++;
        }
        return 0;
    }
    while (n > 0) { diff = *lhs++ - *rhs; if (diff) return diff; n--; rhs++; }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetPrintfCode);
#ifdef SKIP_ASM
/**
 * Does nothing and returns 1 (a printf-style debug stub; nothing in retail calls it).
 *
 * @param format  first argument (ignored)
 * @return 1
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int NetPrintfCode(int format, ...) { return 1; }
#endif

/* NetLib_queue: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store result and wake th). */
struct sSifReq {                        /* one request record (0x20 bytes, DMAed to the IOP); names ours,
        [U] NetLib_queueRequest
                                           fills every word from its arguments */
    int payloadAddr;                    /* 0x00: IOP address of the payload (iopPayload + its offset) */
    int payloadSize;                    /* 0x04: payload byte count */
    int replyAddr;                      /* 0x08: the reply buffer (dst) */
    int replySize;                      /* 0x0C: its size (dstsize) */
    int objectId;                       /* 0x10: target object id on the IOP (fno) */
    int callArg;                        /* 0x14: call argument word */
    int serviceId;                      /* 0x18: the service's IOP id (handle +0xC) */
    int key;                            /* 0x1C */
};

/* One reply record (0x20 bytes, written by the IOP). Names ours: [U] NetLib_completeReplies stores result in a blocking
   request's result pointer and passes the other words to a callback completion; NetSession_onRecvDone ('sess'/'recv') reads
   replySize as the received byte count. replyAddr / objectId are named after the request words at the same offsets
   (guess: the IOP echoes the record back; no reader in retail). */
struct sSifRes { int pad0[2]; int replyAddr; int replySize; int objectId; int result; int pad18; int key; };

/* A request's completion: SIF_DONE_NONE, SIF_DONE_WAIT (the sender sleeps until the reply's result is stored), or a
   callback run with the reply (sentinel function pointers, as signal()'s SIG_DFL / SIG_IGN). */
typedef int (*SifDoneFn)(int, int, int, int, char *, int, int, int, int *);
#define SIF_DONE_NONE ((SifDoneFn)0)
#define SIF_DONE_WAIT ((SifDoneFn)1)

struct sSifCb { SifDoneFn fn; int *out; int arg; int th; };

/* A service handle / service directory entry (16 bytes): [U] NetLib_resolveService matches moduleTag and funcTag (names ours:
   the 'netw' / 'sess' ... and 'crea' / 'look' ... tags it is called with) and copies idx and v (the IOP id). */
struct sSifEnt { int idx; int moduleTag; int funcTag; int v; };

/* The areas NetLib_startQueue lays out in buf (each used through the EE's uncached segment, and mirrored on the IOP at
   iopBase + 0x40): service directory 0x0 (0x800 bytes; byte 0 is also NetLib_pingIop's answer flag), requests a[]
   0x800, replies b[] 0xA00, payloads 0xC00 (0x4000 bytes), DMA ids 0x4C00 and completions c[] 0x4C40. Addresses named
   iop* are IOP addresses. */
struct sSifQueue {
    char buf[0x4D80];                   /* 0x0000 */
    int headerWord0;                    /* 0x4D80: 0x10, the first word of the header sent to the IOP */
    int keyLimit;                       /* 0x4D84: 0x7FFFFFFF: request keys wrap at it (also sent in the header) */
    struct sSifEnt *beg;                /* 0x4D88 */
    struct sSifEnt *end;
    int iopDir;                         /* 0x4D90: IOP address of the directory */
    struct sSifReq *a;                  /* 0x4D94 */
    struct sSifReq *aEnd;               /* 0x4D98 */
    int iopReqs;                        /* 0x4D9C: IOP address of the request ring */
    struct sSifRes *b;                  /* 0x4DA0 */
    struct sSifRes *bEnd;               /* 0x4DA4 */
    int iopRes;                         /* 0x4DA8: IOP address of the reply ring */
    char *payload;                      /* 0x4DAC: the payload area */
    char *payloadEnd;                   /* 0x4DB0 */
    int iopPayload;                     /* 0x4DB4: IOP address of the payload area */
    int *dmaIds;                        /* 0x4DB8: sceSifSetDma id of each request's DMA */
    int *dmaIdsEnd;                     /* 0x4DBC */
    int iopBase;                        /* 0x4DC0: the IOP-side copy (sceSifAllocIopHeap) */
    struct sSifCb *c;                   /* 0x4DC4 */
    char *area4D40;                     /* 0x4DC8: &buf[0x4D40] (not read on the EE) */
    int nextKey;                        /* 0x4DCC */
    int head;                           /* 0x4DD0 */
    int tail;
    int cnt;
    int writeOff;                       /* 0x4DDC: payload offset of the next request */
    int pos;                            /* 0x4DE0 */
    int payloadSize;                    /* 0x4DE4 */
    int threadId;                       /* 0x4DE8: the queue thread (CreateThread; NetLib_onPollAlarm wakes it) */
    int sendAlarmId;                    /* 0x4DEC: the wake alarm NetLib_queueRequest arms for a blocking request, -1 none */
    int pollAlarmId;                    /* 0x4DF0: the alarm NetLib_onPollAlarm re-arms itself with, -1 none */
    int intcHandlerId;                  /* 0x4DF4: the Count interrupt handler (AddIntcHandler2, NetLib_onCountInterrupt) */
    volatile unsigned last;             /* 0x4DF8: Count at the last update */
    int pad4DFC;
    volatile unsigned long total;       /* 0x4E00 */
    int sema;                           /* 0x4E08 */
};

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_onCountInterrupt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (read the Count register, enable interrupts) and memory barrier.
#define MFC0_COUNT_2F41(v) __asm__ volatile("mfc0 %0, $9" : "=r"(v))
#define EI_2F41() __asm__ volatile("ei")
#define SYNC_2F41() __asm__ volatile("sync")

/**
 * Interrupt handler that extends the COP0 Count register into the IOP queue's 64-bit cycle total.
 *
 * Installed by NetLib_startQueue for interrupt cause 2 with the queue NetLib_queue as its argument: adds the Count cycles
 * since the last update (+0x4DF8) to the total (+0x4E00), stores the new Count, then syncs and re-enables interrupts.
 * NetTick turns the total into milliseconds.
 *
 * @param cause  interrupt cause (unused)
 * @param handlerArg  the queue
 * @return 0
 */
// FAKEMATCH: the Count read is wrapped in `do { ... } while (0)`; its loop notes make sched1 treat the next insn as a
// barrier, so the volatile `total` load issues before `c - last` as in retail; plain form 84.62%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int NetLib_onCountInterrupt(int cause, void *handlerArg) {
    struct sSifQueue *timing = (struct sSifQueue *)handlerArg;
    unsigned c;
    unsigned last = timing->last;
    unsigned long total;
    do {
        MFC0_COUNT_2F41(c);
    } while (0);
    total = timing->total;
    timing->total = total + (unsigned)(c - last);
    timing->last = c;
    SYNC_2F41();
    EI_2F41();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_onPollAlarm);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern int f4C50(int) __asm__("iWakeupThread");
extern int f3B80(int, void *, void *) __asm__("iSetAlarm");
void NetLib_onPollAlarm(int a, int b, struct sSifQueue *s);
/**
 * Alarm handler that wakes the IOP queue thread when a reply has arrived, re-arming itself while requests are pending.
 *
 * Forgets the alarm id that fired (+0x4DEC, armed by NetLib_queueRequest; +0x4DF0, its own). While requests are outstanding,
 * wakes the queue thread (+0x4DE8, iWakeupThread) when the oldest one's reply key matches, and sets a new 16-tick alarm
 * (iSetAlarm) when none is armed.
 *
 * @param alarmId  id of the alarm that fired
 * @param alarmTime  alarm time (unused)
 * @param queue  the queue (NetLib_queue)
 */
void NetLib_onPollAlarm(int alarmId, int alarmTime, struct sSifQueue *queue) {
    if (alarmId == queue->sendAlarmId) queue->sendAlarmId = -1;
    if (alarmId == queue->pollAlarmId) queue->pollAlarmId = -1;
    if (queue->tail != queue->head) {
        if (queue->b[queue->tail].key == queue->a[queue->tail].key) f4C50(queue->threadId);
        if (queue->sendAlarmId < 0 && queue->pollAlarmId < 0) queue->pollAlarmId = f3B80(0x10, (void *)NetLib_onPollAlarm,
                queue);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_queueThread);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetLib_completeReplies(void);
extern int SleepThread(void);
extern volatile int NetLib_queueSleepingFlag;
/**
 * Entry point of the IOP queue thread (created by NetLib_startQueue): forever processes arrived replies (NetLib_completeReplies) and
 * sleeps, with NetLib_queueSleepingFlag set while it sleeps.
 */
void NetLib_queueThread(void) {
    while (1) {
        NetLib_completeReplies();
        NetLib_queueSleepingFlag = 1;
        SleepThread();
        NetLib_queueSleepingFlag = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_queueRequest);
#ifdef SKIP_ASM
/* NetLib_queue: the SIF request queue (layout from every user in this unit). */
extern struct sSifQueue NetLib_queue;

/* One piece of a gathered SIF payload (flag 2 of NetLib_queueRequest): a list of these ends with a 0 length. */
struct sSifSeg { int len; void *ptr; };
/* Sony SDK sif.h: one EE-to-IOP DMA transfer for sceSifSetDma (addresses are words). */
typedef struct { unsigned int data; unsigned int addr; unsigned int size; unsigned int mode; } sceSifDmaData;
extern struct { unsigned char b; char pad[11]; int w; } D_00451300;
extern volatile int D_004A5B00;
extern volatile int D_004A5B08;
extern int WaitSema(int);
extern int SignalSema(int);
extern int NetLib_completeReplies(void);
extern int GetThreadId(void);
extern void func_0041605C(char *, char *, int);
extern int SyncDCache(int, int);
extern int sceSifSetDma(sceSifDmaData *, int);
extern int NetTick(void);
extern void sceSifSendCmd(int, void *, int, int, int, int);
extern int SetAlarm(int, void *, void *);
extern int SleepThread(void);
extern unsigned int strlen(const char *);

/* Writes back the D-cache over [a, a + n). */
static __inline__ void f4528_wb(int a, int n) { SyncDCache(a, a + n); }

/**
 * Queues a request to an IOP service and sends it over SIF DMA; optionally blocks until the reply.
 *
 * @p cl is a service handle resolved by NetLib_resolveService (+0xC is the service's IOP id; -1 without one). @p dst must be
 * 16-byte aligned (its size is rounded down to 16). A negative @p size means @p data is a C string; flag 2 makes
 * @p data a {len, ptr} list ending with len 0, gathered into one payload. Under the queue semaphore, while the ring is
 * full it pumps replies (NetLib_completeReplies). It takes room in the 64-byte-aligned payload area (-1 when none), records the
 * completion (@p fn, @p out, flags, thread id) in the callback ring, copies the payload, fills the request record
 * (payload address and size, reply buffer and size, @p fno, @p callArg, service id, key) and DMAs payload and record to the
 * IOP (sceSifSetDma, retried every tick until accepted). Flag 4 writes the reply buffer back from the cache first; flag
 * 1 sends SIF command 1 to kick the IOP. With @p fn == 1 it arms the wake alarm (NetLib_onPollAlarm) and sleeps until
 * NetLib_completeReplies stores the reply's result in *@p out and wakes it.
 *
 * @param cl  service handle
 * @param flags  1 kick the IOP, 2 gather list, 4 write back the reply buffer
 * @param fno  target object id on the IOP (module or session handle)
 * @param callArg  call argument word
 * @param data  payload
 * @param size  payload size, or negative for a C string
 * @param dst  reply buffer (IOP writes it by DMA)
 * @param dstsize  reply buffer size
 * @param fn  completion: 0 none, 1 block and store the result, else a callback
 * @param out  result pointer (or the callback's last argument)
 * @return the request's key, or -1 on error
 */
int NetLib_queueRequest(struct sSifEnt *cl, int flags, int fno, int callArg, char *data, int size, int dst, int dstsize,
                  SifDoneFn fn, int *out) {
    struct sSifQueue *queue = &NetLib_queue;
    sceSifDmaData dma[2];
    struct sSifSeg *seg;
    int i;
    int off;
    int avail;
    int key;
    int tickNow;
    int nowait;
    int wb;
    int asize;

    if (cl == 0 || cl->v == 0) return -1;
    dstsize &= 0xFFFFFFF0;
    if (dst & 0xF) return -1;
    if (size < 0) size = strlen(data) + 1;
    seg = (flags & 2) ? (struct sSifSeg *)data : 0;
    if (seg != 0) {
        size = 0;
        for (i = 0; seg[i].len > 0; i++) size += seg[i].len;
    }
    D_004A5B00 = 2;
    WaitSema(queue->sema);
    D_004A5B00 = 0;
    if ((queue->head + 1) % queue->cnt == queue->tail) {
        while ((queue->head + 1) % queue->cnt == queue->tail) {
            SignalSema(queue->sema);
            NetLib_completeReplies();
            WaitSema(queue->sema);
        }
    }
    if (queue->writeOff < queue->pos) {
        off = queue->writeOff;
        avail = queue->pos - off - 0x40;
    } else if (queue->payloadSize - queue->writeOff - 0x40 >= size) {
        off = queue->writeOff;
        avail = queue->payloadSize - queue->writeOff;
    } else {
        off = 0;
        avail = queue->pos - 0x40;
    }
    if (avail < size) {
        SignalSema(queue->sema);
        return -1;
    }
    nowait = flags & 1;
    wb = flags & 4;
    asize = size + 0x3F;
    queue->c[queue->head].fn = fn;
    queue->c[queue->head].out = out;
    queue->c[queue->head].arg = flags;
    queue->c[queue->head].th = GetThreadId();
    if (size > 0) {
        if (seg != 0) {
            char *copyDst = queue->payload + off;
            for (i = 0; seg[i].len > 0; i++) {
                func_0041605C(copyDst, seg[i].ptr, seg[i].len);
                copyDst += seg[i].len;
            }
        } else {
            func_0041605C(queue->payload + off, data, size);
        }
    }
    queue->a[queue->head].payloadAddr = queue->iopPayload + off;
    queue->a[queue->head].payloadSize = size;
    queue->a[queue->head].replyAddr = dst;
    queue->a[queue->head].replySize = dstsize;
    queue->a[queue->head].objectId = fno;
    queue->a[queue->head].callArg = callArg;
    queue->a[queue->head].serviceId = cl->v;
    queue->a[queue->head].key = queue->nextKey;
    if (wb) f4528_wb(dst, dstsize + 0x80);
    // ORIGINAL-PTR: Sony SDK sceSifDmaData holds the transfer's EE source address as an unsigned int word
    dma[0].data = (int)(queue->payload + off);
    dma[0].addr = queue->iopPayload + off;
    dma[0].size = asize & ~0x3F;
    dma[0].mode = 0;
    dma[1].data = (int)&queue->a[queue->head];
    dma[1].addr = queue->iopReqs + queue->head * sizeof(struct sSifReq);
    dma[1].size = 0x20;
    dma[1].mode = 0;
    queue->dmaIds[queue->head] = dma[0].size != 0 ? sceSifSetDma(&dma[0], 2) : sceSifSetDma(&dma[1], 1);
    while (queue->dmaIds[queue->head] == 0) {
        tickNow = NetTick();
        while (tickNow == NetTick()) {
        }
        queue->dmaIds[queue->head] = dma[0].size != 0 ? sceSifSetDma(&dma[0], 2) : sceSifSetDma(&dma[1], 1);
    }
    if (nowait) sceSifSendCmd(1, &D_00451300, D_00451300.b, 0, 0, 0);
    key = queue->nextKey;
    if (size > 0) queue->writeOff = off + (asize & ~0x3F);
    queue->nextKey = (queue->nextKey + 1) % queue->keyLimit;
    queue->head = (queue->head + 1) % queue->cnt;
    SignalSema(queue->sema);
    if (queue->sendAlarmId < 0 && fn == SIF_DONE_WAIT) queue->sendAlarmId = SetAlarm(8, (void *)NetLib_onPollAlarm, queue);
    if (fn == SIF_DONE_WAIT) {
        D_004A5B08 = 2;
        SleepThread();
        D_004A5B08 = 0;
    }
    return key;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_completeReplies);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#define SYNC_2F41() __asm__ volatile("sync")

/* NetLib_queue: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store result and wake th). */
extern struct sSifQueue NetLib_queue;

extern volatile int D_004A5B00;
extern int WaitSema(int);
extern int SignalSema(int);
extern int WakeupThread(int);
/**
 * Completes every IOP request whose reply has arrived, in order.
 *
 * While the oldest request's reply key matches its request key (checked again under the queue semaphore): a blocking
 * request (completion 1) gets the reply's result word (+0x14) in its result pointer and its thread is woken after the
 * semaphore is released; a callback completion is called with the key, flags, the reply words +0x10 and +0x14, the
 * payload address and size, the reply words +0x8 and +0xC and the result pointer. The payload area up to that request
 * is freed and the tail advances. Called by the queue thread, NetConnIdle and senders waiting for room.
 *
 * @return the number of requests completed
 */
int NetLib_completeReplies(void) {
    struct sSifQueue *queue = &NetLib_queue;
    int n = 0;
    while (queue->tail != queue->head && queue->b[queue->tail].key == queue->a[queue->tail].key) {
        int th, key, reqFlags, replyWord10, replyResult, len, replyWord8, pC;
        SifDoneFn done;
        char *addr;
        D_004A5B00 = 1;
        WaitSema(queue->sema);
        D_004A5B00 = 0;
        key = queue->b[queue->tail].key;
        if (key != queue->a[queue->tail].key) {
            SignalSema(queue->sema);
            return n;
        }
        th = 0;
        addr = queue->payload + (queue->a[queue->tail].payloadAddr - queue->iopPayload);
        replyWord8 = queue->b[queue->tail].replyAddr;
        pC = queue->b[queue->tail].replySize;
        replyWord10 = queue->b[queue->tail].objectId;
        replyResult = queue->b[queue->tail].result;
        done = queue->c[queue->tail].fn;
        reqFlags = queue->c[queue->tail].arg;
        len = queue->a[queue->tail].payloadSize;
        if (done != SIF_DONE_NONE) {
            if (done == SIF_DONE_WAIT) {
                if (queue->c[queue->tail].out != 0) *queue->c[queue->tail].out = replyResult;
                th = queue->c[queue->tail].th;
            } else {
                queue->c[queue->tail].fn(key, reqFlags, replyWord10, replyResult, addr, len, replyWord8, pC, queue->c[queue->tail].out);
                SYNC_2F41();
            }
        }
        if (queue->a[queue->tail].payloadSize > 0) {
            queue->pos = queue->a[queue->tail].payloadAddr - queue->iopPayload;
            queue->pos += (queue->a[queue->tail].payloadSize + 0x3F) & ~0x3F;
        }
        queue->tail = (queue->tail + 1) % queue->cnt;
        SignalSema(queue->sema);
        if (th != 0) WakeupThread(th);
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_callService);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an unused `int unused[12]` gives the 0x60 frame (perhaps a compiled-out debug buffer); plain form 99.50%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int NetLib_queueRequest_4C68(int, int, int, int, int, int, int, int, int, int, int) __asm__("NetLib_queueRequest");
int NetLib_callService_impl(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) __asm__("NetLib_callService");
/**
 * Sends an IOP request: forwards its ten arguments to NetLib_queueRequest (passing the call argument a second time as an
 * eleventh).
 *
 * @param handle  service handle (from NetLib_resolveService)
 * @param flags  flags (1 kick, 2 gather list, 4 write back the reply buffer)
 * @param objectId  target object id on the IOP
 * @param callArg  call argument word
 * @param payload  payload
 * @param payloadSize  payload size, or negative for a C string
 * @param replyBuf  reply buffer
 * @param replySize  reply buffer size
 * @param completion  completion: 0 none, 1 block, else a callback
 * @param resultOut  result pointer
 * @return the request's key, or -1
 */
int NetLib_callService_impl(int handle, int flags, int objectId, int callArg, int payload, int payloadSize, int replyBuf, int replySize, int completion, int resultOut) {
    int stackPad[12];
    return NetLib_queueRequest_4C68(handle, flags, objectId, callArg, payload, payloadSize, replyBuf, replySize, completion, resultOut, callArg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_resolveService);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* NetLib_queue: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store result and wake th). */
extern struct sSifQueue NetLib_queue;

int f4C98impl(struct sSifEnt *c, int k1, int k2) __asm__("NetLib_resolveService");
/**
 * Resolves an IOP service by its module and function tags into a 16-byte handle, caching the result.
 *
 * When the handle already holds the same tags and its cached index still names a directory entry (index < 129) with the
 * same id and module tag, returns 0. Otherwise stores the tags and searches the service directory (+0x4D88 .. +0x4D8C
 * of NetLib_queue, fetched by NetLib_fetchServiceDirectory) for the pair: found, it copies the entry's index and IOP id and returns 1;
 * not found, it clears both and returns -1.
 *
 * @param handle  the handle: index, module tag, function tag, IOP id
 * @param moduleTag  module tag ('netw', 'name', 'sess', ...)
 * @param funcTag  function tag ('crea', 'look', 'send', ...)
 * @return 0 cached, 1 found, -1 unknown service
 */
int f4C98impl(struct sSifEnt *handle, int moduleTag, int funcTag) {
    struct sSifQueue *queue = &NetLib_queue;
    struct sSifEnt *dirEntry;
    if (moduleTag == handle->moduleTag && funcTag == handle->funcTag) {
        int i = handle->idx;
        if (i >= 0) {
            if (i < 129) {
                struct sSifEnt *ent = (struct sSifEnt *)(i * 16 + (int)queue->beg);
                if (ent->v == handle->v && ent->moduleTag == moduleTag) return 0;
            }
        }
    }
    handle->funcTag = funcTag;
    handle->moduleTag = moduleTag;
    for (dirEntry = queue->beg; dirEntry != queue->end; dirEntry++) {
        if (dirEntry->moduleTag == handle->moduleTag && dirEntry->funcTag == handle->funcTag) {
            handle->idx = dirEntry->idx;
            handle->v = dirEntry->v;
            return 1;
        }
    }
    handle->idx = 0;
    handle->v = 0;
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_fetchServiceDirectory);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* NetLib_queue: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store result and wake th). */
extern struct sSifQueue NetLib_queue;

extern int func_003F4C68d(int *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_completeReplies(void);
/**
 * Fetches the IOP's service directory into the queue and waits until every queued request has completed.
 *
 * Sends a blocking request to the fixed directory service (a handle whose IOP id is 1) with the directory area of
 * NetLib_queue (+0x4D88 .. +0x4D8C, 0x800 bytes) as the reply buffer, then pumps replies (NetLib_completeReplies) until the ring
 * is empty. NetLib_resolveService searches that directory. Called by NetConnStartup right after starting the queue.
 *
 * @return 0
 */
int NetLib_fetchServiceDirectory(void) {
    int dirHandle[4];
    dirHandle[3] = 1;
    func_003F4C68d(dirHandle, 1, 0, 0, 0, 0, (int)NetLib_queue.beg, (char *)NetLib_queue.end - (char *)NetLib_queue.beg, 1, 0);
    while (NetLib_queue.head != NetLib_queue.tail) NetLib_completeReplies();
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetTick);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (read the Count register).
#define MFC0_COUNT_2F41(v) __asm__ volatile("mfc0 %0, $9" : "=r"(v))

extern long __udivdi3_4DE8(long, long) __asm__("__udivdi3");
/**
 * Returns the time in milliseconds since the IOP queue's timer started.
 *
 * Reads a consistent snapshot of the last Count value (+0x4DF8) and the 64-bit cycle total (+0x4E00) kept by the
 * interrupt handler NetLib_onCountInterrupt (re-reading while either changes), adds the Count cycles since that update and
 * divides by 0x48000 (294,912 cycles per millisecond at the EE's 294.912 MHz) with __udivdi3.
 *
 * @return the tick in milliseconds
 */
// FAKEMATCH: `t = cnt; t = last;` (a dead read of the volatile local cnt into t, which then carries last into the
// test) reproduces retail's `lw v1,0(sp); lw v1,4(sp)`; plain form 96.97% (volatile locals kept: retail stores each
// snapshot to the stack and reloads it; without them 43.24%).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int NetTick(void) {
    volatile unsigned cnt;
    volatile unsigned last;
    volatile unsigned long total;
    volatile unsigned long sum;
    unsigned c, lastSeen;
    do {
        last = NetLib_queue.last;
        total = NetLib_queue.total;
        MFC0_COUNT_2F41(c);
        cnt = c;
        lastSeen = cnt;
        lastSeen = last;
    } while (lastSeen != NetLib_queue.last || total != NetLib_queue.total);
    sum = total + (unsigned)(cnt - last);
    return (int)__udivdi3_4DE8(sum, 0x48000);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_writeBackRange);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int SyncDCache(int, int);
/**
 * Writes a memory range back from the data cache (SyncDCache over @p start .. @p start + @p size).
 *
 * @param start  first address
 * @param size  byte count
 * @return SyncDCache's result
 */
int NetLib_writeBackRange(int start, int size) { return SyncDCache(start, start + size); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_checkSharedBlock);
#ifdef SKIP_ASM
/**
 * Checks a block in shared memory written by the IOP and returns its tag and size.
 *
 * Reads the 16-byte header through the uncached mapping: +0x0 link, +0x8 tag, +0xC payload size. Returns -1 when the
 * link is 0 or the size is outside 0..0x8000; -2 when the link does not point just past the 16-byte-rounded payload or
 * the linked block's own link does not point back; -3 when the linked block's tag differs. NetGameLinkRecv uses it.
 *
 * @param blockAddr  address of the block header
 * @param out  receives the payload size (may be 0)
 * @return the block's tag, or -1, -2 or -3
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int NetLib_checkSharedBlock(unsigned blockAddr, int *out) {
    // ORIGINAL-PTR: the IOP writes these blocks to EE memory by DMA, so they are read through the EE's uncached
    // segment (address & 0x0FFFFFFF | 0x20000000, a hardware address segment), and each block's link holds the next
    // block's address, 16 + its rounded payload size bytes on (variable-length records)
    unsigned *header = (unsigned *)((blockAddr & 0xFFFFFFF) | 0x20000000);
    unsigned x = header[0];
    int sz;
    int n;
    unsigned *nextHeader;
    unsigned blockTag;
    if (x == 0 || (sz = header[3]) < 0 || sz > 0x8000) return -1;
    n = ((sz + 0xF) & 0x7FFFFFF0) + 0x10;
    nextHeader = (unsigned *)((x & 0xFFFFFFF) | 0x20000000);
    blockTag = header[2];
    if (nextHeader != (unsigned *)((char *)header + n)) return -2;
    if ((((nextHeader[0]) & 0xFFFFFFF) | 0x20000000) != (unsigned)header) return -2;
    if (blockTag != nextHeader[2]) return -3;
    if (out) *out = sz;
    return blockTag;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_pingIop);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (read the Count register).
#define MFC0_COUNT_2F41(v) __asm__ volatile("mfc0 %0, $9" : "=r"(v))

/* NetLib_queue: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store result and wake th). */
extern struct sSifQueue NetLib_queue;

extern struct { unsigned char b; char pad[11]; int w; } D_00451340;
extern void FlushCache(int);
extern void sceSifSendCmd(int, void *, int, int, int, int);
/**
 * Checks that the IOP side of the request queue answers.
 *
 * Clears the flag byte at the start of NetLib_queue, sends SIF command 1 (sceSifSendCmd) carrying the flag's address + 1,
 * and spins (flushing the cache) until the IOP sets the flag or 50,000,000 Count cycles (about 170 ms) pass.
 *
 * @return 1 when the IOP answered, 0 on timeout
 */
int NetLib_pingIop(void) {
    int answered;
    unsigned start;
    unsigned now;
    char *flag = NetLib_queue.buf;
    unsigned limit = 0x2FAF080;
    // ORIGINAL-PTR: the IOP gets the flag's EE address as a word of the SIF command packet (Sony SDK sceSifCmdHdr's
    // opt word is an unsigned int) and writes the flag there
    *flag = 0;
    D_00451340.w = (int)flag + 1;
    FlushCache(0);
    sceSifSendCmd(1, &D_00451340, D_00451340.b, 0, 0, 0);
    answered = 1;
    MFC0_COUNT_2F41(start);
    while (*flag == 0) {
        MFC0_COUNT_2F41(now);
        if (now - start > limit) {
            answered = 0;
            break;
        }
        FlushCache(0);
    }
    return answered;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", NetLib_startQueue);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* NetLib_queue: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store result and wake th). */
extern struct sSifQueue NetLib_queue;

extern int NetLib_queueStartedFlag;
extern char D_00523F00[];   /* the queue thread's stack (0x2000 bytes) */
extern int _gp;   /* the linker-script $gp value (0x4A30F0, D_004A30F0); gas never addresses _gp off $gp */

/* What NetLib_startQueue DMAs to the IOP buffer's first 0x40 bytes: per region, its IOP start / end and EE address. */
/* (names ours: [U] NetLib_startQueue copies the queue's headerWord0 and keyLimit into the first two words) */
struct sSifQueueHdr { int firstWord; int keyLimit; int v[14]; };
/* The kernel's CreateSema / CreateThread parameter blocks (Sony eekernel.h SemaParam / ThreadParam), as far as this
   call fills them. */
struct sSemaParam {
    int count;
    int max_count;
    int init_count;
    int wait_threads;
    unsigned int attr;
    unsigned int option;
};
struct sThreadParam {
    int status;
    void (*func)(void);
    void *stack;
    int stackSize;
    void *gpReg;
    int initPriority;
    int currentPriority;
    unsigned int attr;
    unsigned int option;
};

extern void f5000_6210(void *, int, int) __asm__("memset");
/* sceSifSendCmd (sceSifSendCmd) returns its packet id; the unit declares it void. */
extern int f5000_61B0(int, void *, int, int, int, int) __asm__("sceSifSendCmd");
extern int sceSifAllocIopHeap(int);
extern int sceSifDmaStat(int);
extern void sceSifInitCmd(void);
extern int CreateSema(struct sSemaParam *);
extern int CreateThread(struct sThreadParam *);
extern int AddIntcHandler2(int, int (*)(int, void *), int, void *);
extern int EnableIntc(int);
extern void StartThread(int, void *);
extern int NetLib_onCountInterrupt(int, void *);

/**
 * Starts the EE-IOP request queue NetLib_queue.
 *
 * Fails (-1) when the IOP does not answer (NetLib_pingIop). Zeroes the queue and lays out its uncached areas: service
 * directory (0x800 bytes), request ring and reply ring (0x200 each), payload area (0x4000), DMA-id words and the
 * completion ring. Allocates the IOP-side copy (sceSifAllocIopHeap, -2 on failure), DMAs a 0x40-byte header describing the
 * areas and then the request ring (sceSifSetDma, -3 / -4 on failure), initialises SIF commands (sceSifInitCmd) and
 * sends command 1 with the IOP address. Creates the queue semaphore and the queue thread (NetLib_queueThread, priority 10,
 * 0x2000-byte stack D_00523F00; -5 on failure), installs the Count handler NetLib_onCountInterrupt on interrupt 2 and enables it,
 * starts the thread and sets the started flag NetLib_queueStartedFlag.
 *
 * @return 0, or -1 .. -5 on failure
 */
int NetLib_startQueue(void) {
    struct sSifQueue *queue = &NetLib_queue;
    struct sSifQueueHdr hdr;
    sceSifDmaData dma;
    struct sSemaParam sema;
    struct sThreadParam tp;
    int iop;
    int size;
    int id;

    if (NetLib_pingIop() == 0) return -1;
    // ORIGINAL-PTR: the queue's areas are shared with the IOP, so the EE uses them through its uncached segment
    // (address & 0x0FFFFFFF | 0x20000000, a hardware address segment); their EE addresses go to the IOP as words of
    // the header, and Sony SDK sceSifDmaData holds the DMA source address as an unsigned int
    f5000_6210(queue, 0, 20032);
    queue->headerWord0 = 0x10;
    queue->keyLimit = 0x7FFFFFFF;
    queue->beg = (struct sSifEnt *)(((int)&queue->buf[0] & 0xFFFFFFF) | 0x20000000);
    queue->end = (struct sSifEnt *)(((int)&queue->buf[0x800] & 0xFFFFFFF) | 0x20000000);
    queue->a = (struct sSifReq *)(((int)&queue->buf[0x800] & 0xFFFFFFF) | 0x20000000);
    queue->aEnd = (struct sSifReq *)(((int)&queue->buf[0xA00] & 0xFFFFFFF) | 0x20000000);
    queue->b = (struct sSifRes *)(((int)&queue->buf[0xA00] & 0xFFFFFFF) | 0x20000000);
    queue->bEnd = (struct sSifRes *)(((int)&queue->buf[0xC00] & 0xFFFFFFF) | 0x20000000);
    queue->payload = (char *)(((int)&queue->buf[0xC00] & 0xFFFFFFF) | 0x20000000);
    queue->payloadEnd = (char *)(((int)&queue->buf[0x4C00] & 0xFFFFFFF) | 0x20000000);
    queue->dmaIds = (int *)&queue->buf[0x4C00];
    queue->dmaIdsEnd = (int *)&queue->buf[0x4C40];
    queue->c = (struct sSifCb *)&queue->buf[0x4C40];
    queue->area4D40 = &queue->buf[0x4D40];
    size = 0x40;
    size += (queue->end - queue->beg) * sizeof(struct sSifEnt);
    size += (queue->aEnd - queue->a) * sizeof(struct sSifReq);
    size += (queue->bEnd - queue->b) * sizeof(struct sSifRes);
    size += queue->payloadEnd - queue->payload;
    size += (queue->dmaIdsEnd - queue->dmaIds) * sizeof(int);
    queue->iopBase = sceSifAllocIopHeap(size);
    if (queue->iopBase == 0) return -2;
    iop = (queue->iopBase & 0xFFFFFFF) | 0xA0000000;
    queue->iopBase = iop;
    hdr.firstWord = queue->headerWord0;
    hdr.keyLimit = queue->keyLimit;
    hdr.v[0] = iop + 0x40;
    hdr.v[1] = hdr.v[0] + (queue->end - queue->beg) * sizeof(struct sSifEnt);
    hdr.v[2] = (int)queue->beg;
    hdr.v[3] = hdr.v[1];
    hdr.v[4] = hdr.v[3] + (queue->aEnd - queue->a) * sizeof(struct sSifReq);
    hdr.v[5] = (int)queue->a;
    hdr.v[6] = hdr.v[4];
    hdr.v[7] = hdr.v[6] + (queue->bEnd - queue->b) * sizeof(struct sSifRes);
    hdr.v[8] = (int)queue->b;
    hdr.v[9] = hdr.v[7];
    hdr.v[10] = hdr.v[9] + (queue->payloadEnd - queue->payload);
    hdr.v[11] = (int)queue->payload;
    hdr.v[12] = hdr.v[10];
    hdr.v[13] = hdr.v[12] + (queue->dmaIdsEnd - queue->dmaIds) * sizeof(int);
    queue->iopDir = hdr.v[0];
    queue->iopReqs = hdr.v[3];
    queue->iopRes = hdr.v[6];
    queue->iopPayload = hdr.v[9];
    FlushCache(0);
    dma.data = (int)&hdr;
    dma.addr = queue->iopBase;
    dma.size = 0x40;
    dma.mode = 0;
    id = sceSifSetDma(&dma, 1);
    if (id == 0) return -3;
    while (sceSifDmaStat(id) >= 0) {
    }
    dma.data = (int)queue->a;
    dma.addr = queue->iopReqs;
    dma.size = 0x200;
    dma.mode = 0;
    id = sceSifSetDma(&dma, 1);
    if (id == 0) return -4;
    while (sceSifDmaStat(id) >= 0) {
    }
    sceSifInitCmd();
    D_00451340.w = queue->iopBase;
    FlushCache(0);
    f5000_61B0(1, &D_00451340, D_00451340.b, 0, 0, 0);
    queue->nextKey = 1;
    queue->cnt = (queue->aEnd - queue->a);
    queue->tail = 1;
    queue->head = 1;
    queue->payloadSize = queue->payloadEnd - queue->payload;
    f5000_6210(&sema, 0, sizeof(sema));
    sema.init_count = 1;
    sema.max_count = 1;
    sema.option = 0;
    queue->sema = CreateSema(&sema);
    queue->sendAlarmId = -1;
    queue->pollAlarmId = -1;
    tp.func = NetLib_queueThread;
    tp.initPriority = 10;
    tp.stack = D_00523F00;
    tp.stackSize = 0x2000;
    tp.gpReg = &_gp;
    tp.option = 0;
    queue->threadId = CreateThread(&tp);
    if (queue->threadId <= 0) return -5;
    queue->intcHandlerId = AddIntcHandler2(2, NetLib_onCountInterrupt, -1, queue);
    EnableIntc(2);
    StartThread(queue->threadId, queue);
    NetLib_queueStartedFlag = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_stopQueue);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* NetLib_queue: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store result and wake th). */
extern struct sSifQueue NetLib_queue;

extern int NetLib_queueStartedFlag;

extern void ReleaseAlarm(int);
extern void RemoveIntcHandler(int, int);
extern void TerminateThread(int);
extern void DeleteThread(int);
extern void DeleteSema(int);
/**
 * Stops the EE-IOP request queue: releases its alarms, removes the interrupt handler, terminates and deletes the queue
 * thread, deletes the semaphore and clears the started flag NetLib_queueStartedFlag.
 *
 * Nothing in retail calls it.
 *
 * @return 0
 */
int NetLib_stopQueue(void) {
    struct sSifQueue *queue = &NetLib_queue;
    if (queue->sendAlarmId >= 0) ReleaseAlarm(queue->sendAlarmId);
    if (queue->pollAlarmId >= 0) ReleaseAlarm(queue->pollAlarmId);
    RemoveIntcHandler(2, queue->intcHandlerId);
    TerminateThread(queue->threadId);
    DeleteThread(queue->threadId);
    DeleteSema(queue->sema);
    NetLib_queueStartedFlag = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetLib_isQueueStarted);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetLib_queueStartedFlag;

/**
 * Returns whether the EE-IOP request queue has been started (NetLib_queueStartedFlag, set by NetLib_startQueue).
 *
 * @return the started flag
 */
int NetLib_isQueueStarted(void) {
    return NetLib_queueStartedFlag;
}
#endif

/* DirtySock's socket address ([O] SSX On Tour's DWARF: struct sockaddr {unsigned short sa_family; unsigned char
   sa_data[14];}, 0x10 bytes). An IPv4 address keeps its big-endian port in sa_data[0..1] and the address in
   sa_data[2..5]. */
struct sockaddr {
    unsigned short sa_family;           /* 0x00 */
    unsigned char sa_data[14];          /* 0x02 */
};

/* A network session's 0x20-byte address block: [U] NetSession_connect writes the peer for 'sess'/'conn'; the IOP's
   'sess'/'stat' reply fills both (NetSession_getStatus reads them). */
struct sSessAddr {
    struct sockaddr peer;               /* 0x00 */
    struct sockaddr local;              /* 0x10 */
};

/* Network session handle (0x9C bytes, created by NetSession_create). The receive callback NetSession_onRecvDone (an RPC
   completion) writes recvPending / readOffset / recvLength, so they are volatile in every accessor. Member names are
   ours, from that use. */
struct sSess {
    int id;                             /* 0x00: the IOP session id */
    struct sSessAddr *addr;             /* 0x04: the address block, 64-byte aligned inside space */
    volatile int recvPending;           /* 0x08: [U] NetSession_peek sets it when it sends 'sess'/'recv', the callback
                                           clears it */
    volatile int readOffset;            /* 0x0C: [U] offset of the next unread packet in recvBuf (NetSession_recv
                                           advances it) */
    volatile int recvLength;            /* 0x10: [U] bytes received ('ibuf' of NetSession_getStatus) */
    int recvSize;                       /* 0x14: receive buffer size */
    unsigned char *recvBuf;             /* 0x18: receive buffer, 0x40 bytes after the address block */
    unsigned char space[0x80];          /* 0x1C: room for the aligned address block (the receive buffer starts in it
                                           and runs on into the allocation) */
};

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_create);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char NetSession_createService[];
extern void *RpcAlloc(int);
extern void *f6210h(void *, int, int) __asm__("memset");
extern int f5478_SyncDCache(void *, void *) __asm__("SyncDCache");
extern void FlushCache(int);
extern void RpcFree(void *);
extern int NetLib_resolveService(char *, int, int);
extern int f5478_4C68(char *, int, int, int, int, int, int, int, int, struct sSess *) __asm__("NetLib_callService");
/**
 * Creates a network packet session on the IOP ('sess'/'crea').
 *
 * Clamps @p size to 0x1000..0x2000 and allocates a 156-byte header plus the buffers with RpcAlloc: a 64-byte-aligned
 * 0x20-byte address block (+0x4) and, 0x40 bytes after it, the receive buffer (+0x18) of @p size bytes (+0x14). Zeroes
 * both and writes them back from the cache, then waits for the IOP session id into +0x0; with id 0 the block is freed
 * and 0 returned. BuddyApi (BuddyApi_create) creates its session with 0x8000, clamped to 0x2000.
 *
 * @param size  receive buffer size
 * @return the session, or 0
 */
struct sSess *NetSession_create(int size) {
    struct sSess *session;

    if (size < 0x1000) size = 0x1000;
    if (size > 0x2000) size = 0x2000;

    session = RpcAlloc(size + 156);
    if (session == 0) return 0;
    f6210h(session, 0, 156);
    session->addr = (struct sSessAddr *)((int)&session->space[0x3F] & ~0x3F);
    session->recvSize = size;
    session->recvBuf = (unsigned char *)&session->addr[2];
    f6210h(session->addr, 0, 0x20);
    f6210h(session->recvBuf, 0, session->recvSize);
    f5478_SyncDCache(session->addr, session->addr + 1);
    f5478_SyncDCache(session->recvBuf, session->recvBuf + session->recvSize);
    FlushCache(0);
    NetLib_resolveService(NetSession_createService, 'sess', 'crea');
    f5478_4C68(NetSession_createService, 1, 0, size, 0, 0, 0, 0, 1, session);
    if (session->id == 0) {
        RpcFree(session);
        session = 0;
    }
    return session;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_destroy);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char NetSession_destroyService[];
extern int NetLib_resolveService(char *, int, int);
extern void RpcFree(void *);
/**
 * Destroys a network packet session: sends 'sess'/'dest' for its id and waits, then frees it (RpcFree).
 *
 * @param session  the session
 */
void NetSession_destroy(int *session) {
    NetLib_resolveService(NetSession_destroyService, 'sess', 'dest');
    NetLib_callService(NetSession_destroyService, 1, *session, 0, 0, 0, 0, 0, 1, 0);
    RpcFree(session);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_connect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char NetSession_connectService[];
extern int NetLib_resolveService(char *, int, int);
extern int f5638_4C68(char *, int, int, int, struct sSessAddr *, int, int, int, int, int) __asm__("NetLib_callService");
extern int f5638_SyncDCache(void *, void *) __asm__("SyncDCache");
/* the address block's peer as a BSD sockaddr_in; sin_zero (8 bytes) is cleared as two words */
typedef struct { unsigned short family; unsigned short port; int addr; int sin_zero0; int sin_zero1; } SA5638;
/**
 * Connects a network packet session to an IPv4 address and port ('sess'/'conn', not waited for).
 *
 * Returns -1 when the session's address block (+0x4) is already in use (family set). Otherwise fills it as a
 * sockaddr_in (family 2, big-endian port and address), writes it back from the cache, sends it and clears the
 * received-byte count (+0x10). NetSession_disconnect disconnects.
 *
 * @param addr  IPv4 address (host order)
 * @param port  port
 * @return 0, or -1 when already connected
 */
int NetSession_connect(struct sSess *self, unsigned addr, int port) {
    unsigned addrShift;
    if (((SA5638 *)self->addr)->family != 0) return -1;
    ((SA5638 *)self->addr)->family = 2;
    ((SA5638 *)self->addr)->port = 0;
    ((SA5638 *)self->addr)->addr = 0;
    ((SA5638 *)self->addr)->sin_zero0 = 0;
    ((SA5638 *)self->addr)->sin_zero1 = 0;
    self->addr->peer.sa_data[0] = port >> 8;
    self->addr->peer.sa_data[1] = port;
    addrShift = addr;
    self->addr->peer.sa_data[5] = addrShift;
    addrShift >>= 8;
    self->addr->peer.sa_data[4] = addrShift;
    addrShift >>= 8;
    self->addr->peer.sa_data[3] = addrShift;
    addrShift >>= 8;
    self->addr->peer.sa_data[2] = addrShift;
    f5638_SyncDCache(self->addr, self->addr + 1);
    NetLib_resolveService(NetSession_connectService, 'sess', 'conn');
    f5638_4C68(NetSession_connectService, 0, self->id, 0, self->addr, 0x10, 0, 0, 0, 0);
    self->recvLength = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_disconnect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char NetSession_disconnectService[];
extern int NetLib_resolveService(char *, int, int);
extern void func_00416210b(void *, int, int) __asm__("memset");
/**
 * Disconnects a network packet session ('sess'/'disc', not waited for), zeroes its address block (+0x4) and clears the
 * received-byte count (+0x10).
 *
 * @return 0
 */
int NetSession_disconnect(struct sSess *self) {
    NetLib_resolveService(NetSession_disconnectService, 'sess', 'disc');
    NetLib_callService(NetSession_disconnectService, 0, self->id, 0, 0, 0, 0, 0, 0, 0);
    func_00416210b(self->addr, 0, 0x20);
    self->recvLength = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_getStatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char NetSession_statusService[];
extern int NetLib_resolveService(char *, int, int);
extern int c57_4C68(char *, int, int, int, int, int, struct sSessAddr *, int, int, int) __asm__("NetLib_callService");
int NetSession_getStatus_impl(struct sSess *self, int key) __asm__("NetSession_getStatus");
/**
 * Returns one field of a network packet session's status.
 *
 * Fetches the 0x20-byte status from the IOP ('sess'/'stat', waiting) into the address block (+0x4), then returns by
 * @p key: 'port' / 'addr' the peer's port and address (bytes 2-3 / 4-7, big-endian), 'lprt' / 'ladr' the local port and
 * address (bytes 0x12-0x13 / 0x14-0x17), 'ibuf' the bytes received and not yet read (+0x10); 0 for anything else.
 * LobbyApi_update calls it twice, for 'lprt' and 'ladr', to record the session's local address.
 *
 * @param key  selector ('port', 'addr', 'lprt', 'ladr', 'ibuf')
 * @return the selected value
 */
int NetSession_getStatus_impl(struct sSess *self, int key) {
    NetLib_resolveService(NetSession_statusService, 'sess', 'stat');
    c57_4C68(NetSession_statusService, 0, self->id, 0, 0, 0, self->addr, 0x20, 1, 0);
    if (key == 'port') {
        return (self->addr->peer.sa_data[0] << 8) | self->addr->peer.sa_data[1];
    }
    if (key == 'addr') {
        return (((self->addr->peer.sa_data[2] << 8 | self->addr->peer.sa_data[3]) << 8 |
                 self->addr->peer.sa_data[4]) << 8) |
               self->addr->peer.sa_data[5];
    }
    if (key == 'ladr') {
        return (((self->addr->local.sa_data[2] << 8 | self->addr->local.sa_data[3]) << 8 | self->addr->local.sa_data[4])
                << 8) | self->addr->local.sa_data[5];
    }
    if (key == 'lprt') {
        return (self->addr->local.sa_data[0] << 8) | self->addr->local.sa_data[1];
    }
    if (key == 'ibuf') {
        return self->recvLength;
    }
    return 0;
}
#endif

/**
 * Does nothing (an empty per-session update hook; BuddyApi_update and NetSession_send call it).
 */
void NetSession_update(void) {
}

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_send);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* The gathered payload NetSession_send sends: the 12-byte header, the data and the 0-length end of the list. */
typedef struct { struct sSifSeg header; struct sSifSeg data; int end; } sSessSendList;
extern char NetSession_sendService[];
extern int NetLib_resolveService(char *, int, int);
extern int c59_4C68(char *, int, int, int, sSessSendList *, int, int, int, int, int) __asm__("NetLib_callService");
extern void c59_5908(int *) __asm__("NetSession_update");
/**
 * Sends one packet on a network packet session ('sess'/'send', not waited for).
 *
 * The packet is a 12-byte big-endian header (kind, argument, total length = data length + 12) followed by the data,
 * sent as a two-piece gather list. A negative @p len means @p str is a C string sent with its NUL. BuddyApi sends its
 * tag-text commands this way (kind 'AUTH', 'PSET', ...).
 *
 * @param packetKind  packet kind (four characters)
 * @param packetArg  packet argument word
 * @param str  packet data
 * @param len  data length, or negative for a C string
 * @return 0
 */
int NetSession_send(int *self, int packetKind, int packetArg, char *str, int len) {
    unsigned char hdr[12];
    sSessSendList io;
    c59_5908(self);
    if (len < 0) {
        len = strlen(str) + 1;
    }
    len += 0xC;
    hdr[8] = len >> 24;
    hdr[9] = len >> 16;
    hdr[10] = len >> 8;
    hdr[11] = len;
    hdr[0] = packetKind >> 24;
    hdr[1] = packetKind >> 16;
    hdr[2] = packetKind >> 8;
    hdr[3] = packetKind;
    hdr[4] = packetArg >> 24;
    hdr[5] = packetArg >> 16;
    hdr[6] = packetArg >> 8;
    hdr[7] = packetArg;
    io.header.len = 0xC;
    io.header.ptr = hdr;
    io.data.len = len - 0xC;
    io.data.ptr = str;
    io.end = 0;
    NetLib_resolveService(NetSession_sendService, 'sess', 'send');
    c59_4C68(NetSession_sendService, 2, self[0], 0, &io, 0, 0, 0, 0, 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_onRecvDone);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* A network session (NetSession_peek / NetSession_recv read from its receive buffer f18). */

int NetSession_onRecvDone_cb(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int len, struct sSess *self) __asm__("NetSession_onRecvDone");
/**
 * Completion callback of a session's 'sess'/'recv' request: records how many bytes arrived.
 *
 * Called by NetLib_completeReplies with the reply's words; stores the received length (the eighth argument, reply word +0xC) in
 * +0x10 and clears the read offset (+0xC) and the receive-pending flag (+0x8).
 *
 * @param requestKey  request key (unused)
 * @param requestFlags  request flags (unused)
 * @param replyWord10  reply word +0x10 (unused)
 * @param replyResult  reply result word (unused)
 * @param payloadAddr  payload address (unused)
 * @param payloadSize  payload size (unused)
 * @param replyWord8  reply word +0x8 (unused)
 * @param len  number of bytes received
 * @return 0
 */
int NetSession_onRecvDone_cb(int requestKey, int requestFlags, int replyWord10, int replyResult, int payloadAddr, int payloadSize, int replyWord8, int len, struct sSess *self) {
    self->recvLength = len;
    self->readOffset = 0;
    self->recvPending = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_peek);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* A network session (NetSession_peek / NetSession_recv read from its receive buffer f18). */

extern char NetSession_peekService[];
extern char str_empty_l[];
extern int NetLib_resolveService(char *, int, int);
extern void a70_47D8(unsigned char *, unsigned char *) __asm__("InvalidDCache");
extern int a70_4C68(char *, int, int, int, int, int, unsigned char *, int, int (*)(void), struct sSess *)
    __asm__("NetLib_callService");
extern int NetSession_onRecvDone(void);
/**
 * Peeks at the next received packet of a network packet session, starting a new receive when all are read.
 *
 * When every received byte has been read (+0xC >= +0x10) and no receive is pending (+0x8), it marks one pending,
 * invalidates the cache over the receive buffer, resets the counts and sends 'sess'/'recv' with the receive buffer and
 * NetSession_onRecvDone as completion; then returns -1. Otherwise it parses the 12-byte header at the read offset: kind and
 * argument (big-endian) go to *@p kindOut / *@p argOut, and *@p dataOut points at the data, NUL-terminated by overwriting its last
 * byte (or at an empty string when there is none). The packet stays unread; NetSession_recv consumes it.
 *
 * @param kindOut  receives the packet kind (may be 0)
 * @param argOut  receives the packet argument (may be 0)
 * @param dataOut  receives the data pointer (may be 0)
 * @return the data length, or -1 when no packet is waiting
 */
int NetSession_peek(struct sSess *self, unsigned *kindOut, unsigned *argOut, char **dataOut) {
    int n;
    if (self->readOffset >= self->recvLength) {
        if (self->recvPending == 0 && self->readOffset >= self->recvLength) {
            self->recvPending = 1;
            if (self->recvLength > 0) {
                a70_47D8(self->recvBuf, self->recvBuf + self->recvSize - 1);
                self->recvLength = 0;
                self->readOffset = self->recvLength;
            }
            NetLib_resolveService(NetSession_peekService, 'sess', 'recv');
            a70_4C68(NetSession_peekService, 0, self->id, 0, 0, 0, self->recvBuf, self->recvSize, NetSession_onRecvDone, self);
        }
        return -1;
    } else {
        unsigned char *header = self->recvBuf + self->readOffset;
        // ORIGINAL-PTR: the receive buffer is a byte stream of variable-length packets (a 12-byte big-endian header,
        // then the payload), so the payload is reached by a byte offset from the packet
        char *payload = (char *)(self->recvBuf + self->readOffset) + 0xC;
        n = header[8] << 24 | header[9] << 16 | header[10] << 8 | header[11];
        n -= 0xC;
        if (n < 1) payload = str_empty_l;
        else payload[n - 1] = 0;
        if (kindOut) *kindOut = header[0] << 24 | header[1] << 16 | header[2] << 8 | header[3];
        if (argOut) *argOut = header[4] << 24 | header[5] << 16 | header[6] << 8 | header[7];
        if (dataOut) *dataOut = payload;
        return n;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_recv);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* A network session (NetSession_peek / NetSession_recv read from its receive buffer f18). */

extern char NetSession_recvService[];
extern int c38_5A70(struct sSess *, int, int, int) __asm__("NetSession_peek");
extern void c38_605C(char *, void *, int) __asm__("func_0041605C");
extern void c38_47D8(unsigned char *, unsigned char *) __asm__("InvalidDCache");
extern int NetLib_resolveService(char *, int, int);
extern int c38_4C68(char *, int, int, int, int, int, unsigned char *, int, int (*)(void), struct sSess *)
    __asm__("NetLib_callService");
extern int NetSession_onRecvDone(void);
int NetSession_recv_impl(struct sSess *self, int a, int b, char *buf, int max) __asm__("NetSession_recv");
/**
 * Reads and consumes the next received packet of a network packet session.
 *
 * Peeks with NetSession_peek (kind and argument to @p kindOut / @p argOut), copies up to @p max data bytes into @p buf when given,
 * and advances the read offset past the packet. When that empties the buffer and no receive is pending it starts the
 * next 'sess'/'recv' as NetSession_peek does. BuddyApi passes no outputs, just to drop the packet it peeked.
 *
 * @param kindOut  receives the packet kind (may be 0)
 * @param argOut  receives the packet argument (may be 0)
 * @param buf  data output (may be 0)
 * @param max  data output size
 * @return the data length, or -1 when no packet is waiting
 */
int NetSession_recv_impl(struct sSess *self, int kindOut, int argOut, char *buf, int max) {
    int dataStart;
    int n = c38_5A70(self, kindOut, argOut, 0);
    if (n >= 0) {
        if (n < max) max = n;
        if (buf) c38_605C(buf, self->recvBuf + self->readOffset + 0xC, max);
        dataStart = self->readOffset + 0xC;
        self->readOffset = dataStart + n;
        if (self->readOffset >= self->recvLength && self->recvPending == 0 && self->readOffset >= self->recvLength) {
            self->recvPending = 1;
            if (self->recvLength > 0) {
                c38_47D8(self->recvBuf, self->recvBuf + self->recvSize - 1);
                self->recvLength = 0;
                self->readOffset = self->recvLength;
            }
            NetLib_resolveService(NetSession_recvService, 'sess', 'recv');
            c38_4C68(NetSession_recvService, 0, self->id, 0, 0, 0, self->recvBuf, self->recvSize, NetSession_onRecvDone, self);
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetSession_getTick);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetTick(void);
/**
 * Returns the millisecond tick (NetTick); the session argument its callers pass is ignored.
 *
 * @return milliseconds since the IOP queue's timer started
 */
int NetSession_getTick(void) { return NetTick(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_loadModules);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int strlen(const char *);

/* A USB keyboard's 0x8C-byte read buffer (sceUsbKbRead fills it). [U] kbReader_startRead / kbReader_finishRead compare the
   first two words with the last read's, kbReader_finishRead tests the key count, kbReader_translateKey / kbReader_isKeyDown read the key
   codes from +0xC, kbReader_getVirtualKey / kbReader_getKeyChar read the LED and modifier words; the reader keeps its ready flag at
   +0x88. The first 0x88 bytes look like Sony libusbkb's USBKBDATA_t (LED, modifiers, count, key codes). */
struct sKbRead {
    unsigned led;                       /* 0x00: LED word (bit 0 Num Lock) */
    unsigned mkey;                      /* 0x04: modifier-key word */
    int len;                            /* 0x08: number of key codes */
    unsigned short keycode[62];         /* 0x0C: USB HID key codes */
    unsigned char ready;                /* 0x88: 1 when the buffer holds unread data */
};

/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
struct sKb {
    int fd[2];                          /* 0x00 */
    int info[2];                        /* 0x08 */
    unsigned char v[128];               /* 0x10: connected */
    int cnt;                            /* 0x90 */
    unsigned char prev[8];              /* 0x94 */
    unsigned st;                        /* 0x9C */
    unsigned char flag;                 /* 0xA0 */
    int cur;                            /* 0xA4 */
    struct sKbRead *ents;               /* 0xA8 */
    int a;                              /* 0xAC */
    int b;
    int tick;                           /* 0xB4 */
    int period;
    void (*onConnect)(int, int);        /* 0xBC: (names ours) [U] called with (keyboard, connected) on a change */
    int (*idleHook)(int);               /* 0xC0: [U] polled by kbReader_update */
};

extern struct sKb kbReader_state;

extern char *strcpy(char *, const char *);
extern char* strcat(char *, const char *);
extern int fBA68(char *, int, const char *) __asm__("sceSifLoadModule");
extern char str_USBD_IRX_b[], str_USBKB_IRX_b[];
/**
 * Loads the USB driver modules USBD.IRX and USBKB.IRX from a directory.
 *
 * With @p name 0 it only marks both module ids in kbReader_state as -1 and returns 0. Otherwise loads @p name + "USBD.IRX"
 * without arguments and @p name + "USBKB.IRX" with @p str as its argument text (sceSifLoadModule, the SIF module loader),
 * keeping the ids in kbReader_state.fd[0] and fd[1]. Called by the USB keyboard setup kbReader_init.
 *
 * @param name  directory prefix of the IRX files, or 0
 * @param str  argument text for USBKB.IRX
 * @return 0, or -1 when @p str is 0 or a load fails
 */
int kbReader_loadModules(char *name, char *str) {
    char path[0x100];
    if (name == 0) {
        kbReader_state.fd[0] = -1;
        kbReader_state.fd[1] = -1;
        return 0;
    }
    if (str == 0) goto fail;
    strcpy(path, name);
    strcat(path, str_USBD_IRX_b);
    kbReader_state.fd[0] = fBA68(path, 0, 0);
    if (kbReader_state.fd[0] < 0) goto fail;
    strcpy(path, name);
    strcat(path, str_USBKB_IRX_b);
    kbReader_state.fd[1] = fBA68(path, strlen(str) + 1, str); if (kbReader_state.fd[1] < 0) return -1; return 0;
fail:
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_unloadModule);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int sceSifStopModule(int, int, int, int *);
extern int sceSifUnloadModule(int);
/**
 * Stops and unloads an IOP module by its id (sceSifStopModule, then sceSifUnloadModule).
 *
 * @param handle  module id (nothing is done when negative)
 * @param errorValue  value to return when either call fails
 * @return 0, or @p errorValue on failure
 */
int kbReader_unloadModule(int handle, int errorValue) {
    int buf[4];
    int result = 0;
    if (handle >= 0) {
        if (sceSifStopModule(handle, 0, 0, buf) < 0) result = errorValue;
        if (sceSifUnloadModule(handle) < 0) result = errorValue;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_startInfoQuery);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

extern char D_00525F88[];
extern int sceUsbKbGetInfo(char *);
/**
 * Starts an asynchronous USB keyboard connection query.
 *
 * Copies each keyboard's connected flag (kbReader_state +0x10) to the previous flags (+0x94), then asks for the keyboard
 * info (sceUsbKbGetInfo into +0x8); when the request is accepted sets the state (+0x9C) to 1 and the busy flag (+0xA0).
 * kbReader_finishInfoQuery finishes it.
 */
void kbReader_startInfoQuery(void) {
    int i;
    char *kbState;
    for (i = 0; i < kbReader_state.cnt; i++) kbReader_state.prev[i] = kbReader_state.v[i];
    if (sceUsbKbGetInfo(D_00525F88) == 0) {
        kbState = D_00525F88 - 8;
        *(int *)(kbState + 0x9C) = 1;
        kbState[0xA0] = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_finishInfoQuery);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

extern int f810(int, int *) __asm__("sceUsbKbSync");
/**
 * Finishes the USB keyboard connection query started by kbReader_startInfoQuery.
 *
 * Polls the request without waiting (sceUsbKbSync with mode 1). When it has finished, calls the connect callback
 * (+0xBC) with (keyboard, connected) for every keyboard whose connected flag changed, and clears the busy flag and the
 * state.
 *
 * @return the poll result: nonzero while the query is still running
 */
int kbReader_finishInfoQuery(void) {
    int syncResult;
    int syncBusy;
    int i;
    syncBusy = f810(1, &syncResult);
    if (syncBusy != 0) return syncBusy;
    for (i = 0; i < kbReader_state.cnt; i++) {
        if (kbReader_state.prev[i] != kbReader_state.v[i]) kbReader_state.onConnect(i, kbReader_state.v[i]);
    }
    kbReader_state.flag = 0;
    kbReader_state.st = 0;
    return syncBusy;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_findFreeKeyboard);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

/**
 * Finds the next connected USB keyboard, starting at @p i and wrapping, whose read buffer is not holding unread data
 * (ready flag at +0x88 of its 0x8C-byte buffer clear).
 *
 * @param i  keyboard to start from
 * @return the keyboard index, or -1 when there is none
 */
int kbReader_findFreeKeyboard(int i) {
    int checked;
    for (checked = 0; checked < kbReader_state.cnt; checked++) {
        if (i >= kbReader_state.cnt) i = 0;
        if (kbReader_state.v[i] != 0) {
            if (kbReader_state.ents[i].ready == 0) return i;
        }
        i++;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_startRead);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

extern int kbReader_findFreeKeyboard(int);
extern int sceUsbKbRead(int, struct sKbRead *);
/**
 * Starts reading the next USB keyboard that has room (kbReader_findFreeKeyboard from the one after the current, +0xA4).
 *
 * Makes it current, remembers its buffer's first two words (LED and modifier state, +0xAC / +0xB0) and starts an
 * asynchronous read into the buffer (sceUsbKbRead); when accepted sets the state to 2 and the busy flag.
 */
void kbReader_startRead(void) {
    int id = kbReader_findFreeKeyboard(kbReader_state.cur + 1);
    struct sKbRead *readBuf;
    if (id != -1) {
        kbReader_state.cur = id;
        readBuf = &kbReader_state.ents[id];
        kbReader_state.a = readBuf->led;
        kbReader_state.b = readBuf->mkey;
        if (sceUsbKbRead(id, readBuf) == 0) {
            kbReader_state.st = 2;
            kbReader_state.flag = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_finishRead);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

extern int f810(int, int *) __asm__("sceUsbKbSync");
/**
 * Finishes the USB keyboard read started by kbReader_startRead.
 *
 * Waits for the read to finish (sceUsbKbSync with mode 0, the blocking wait). Then marks the current keyboard's buffer
 * ready (+0x88 = 1) if it holds key codes or its LED / modifier words changed from the remembered ones, and clears the
 * busy flag and the state.
 *
 * @return 0, or sceUsbKbSync's error (-1 when the keyboard library is not running)
 */
int kbReader_finishRead(void) {
    int syncResult;
    int syncStatus;
    syncStatus = f810(0, &syncResult);
    if (syncStatus != 0) return syncStatus;
    {
        struct sKbRead *readBuf = &kbReader_state.ents[kbReader_state.cur];
        if (readBuf->len > 0 || readBuf->mkey != kbReader_state.b || readBuf->led != kbReader_state.a) readBuf->ready = 1;
    }
    kbReader_state.flag = 0;
    kbReader_state.st = 0;
    return syncStatus;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_defaultIdleHook);
#ifdef SKIP_ASM
/**
 * Default idle hook of the USB keyboard reader: does nothing and returns 0 (never claims the update).
 *
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int kbReader_defaultIdleHook(void) { return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

extern int D_00526010[];

extern int kbReader_loadModules(char *, char *);
extern int f0C028(int *) __asm__("sceUsbKbInit");
extern void f5F08(void) __asm__("kbReader_startInfoQuery");
extern int f5F88(void) __asm__("kbReader_finishInfoQuery");
extern void f6358(int, int) __asm__("kbReader_defaultOnConnect");
extern int f61C8(int) __asm__("kbReader_defaultIdleHook");
// The keyboard state block sits 0x90 bytes below the max-connect word passed to sceUsbKbInit.
static inline struct sKb *ctl(void) { return (struct sKb *)((char *)D_00526010 - 0x90); }
/**
 * Starts the USB keyboard reader.
 *
 * Loads the USB modules (kbReader_loadModules with @p name / @p str) and initialises the keyboard library (sceUsbKbInit with
 * the max-connect word D_00526010, capped at 8). Clears every keyboard's connected and previous flags and the ready
 * flag of each 0x8C-byte buffer in @p ents, resets the current keyboard, sets the info-refresh period and counter to 60
 * updates, installs the callbacks (defaults kbReader_defaultOnConnect and kbReader_defaultIdleHook) and runs one connection query to
 * completion (kbReader_startInfoQuery, then kbReader_finishInfoQuery until done). The USB keyboard reader object (cUsbKeyboard::cUsbKeyboard) calls it
 * with no module path, so the modules are expected to be loaded already (initOnline loads them).
 *
 * @param name  directory of USBD.IRX / USBKB.IRX, or 0 to skip loading
 * @param str  argument text for USBKB.IRX
 * @param ents  read buffers, 0x8C bytes per keyboard
 * @param onConnect  called with (keyboard, connected) when a keyboard connects or disconnects (0 for kbReader_defaultOnConnect)
 * @param idleHook  idle hook polled by kbReader_update (0 for kbReader_defaultIdleHook)
 * @return 0, or -1 when a module or the library fails to start
 */
int kbReader_init(char *name, char *str, struct sKbRead *ents, void (*onConnect)(int, int), int (*idleHook)(int)) {
    int i;
    if (kbReader_loadModules(name, str) < 0) return -1;
    if (f0C028(D_00526010) == -1) return -1;
    if (ctl()->cnt > 8) ctl()->cnt = 8;
    ctl()->ents = ents;
    for (i = 0; i < ctl()->cnt; i++) {
        ctl()->v[i] = 0;
        ctl()->prev[i] = 0;
        ents[i].ready = 0;
    }
    kbReader_state.cur = 0;
    kbReader_state.period = 0x3C;
    kbReader_state.tick = 0x3C;
    kbReader_state.flag = 0;
    kbReader_state.onConnect = onConnect ? onConnect : f6358;
    kbReader_state.idleHook = idleHook ? idleHook : f61C8;
    f5F08();
    while (f5F88()) ;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_shutdown);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

extern int sceUsbKbEnd(void);
extern int kbReader_unloadModule_62F8(int, int) __asm__("kbReader_unloadModule");
/**
 * Shuts the USB keyboard reader down: ends the keyboard library (sceUsbKbEnd) and unloads USBKB.IRX and USBD.IRX
 * (kbReader_unloadModule with error values 1 and 2).
 *
 * @return 0, or the OR of the error values of the unloads that failed
 */
int kbReader_shutdown(void) {
    int unloadErrors;
    sceUsbKbEnd();
    unloadErrors = kbReader_unloadModule_62F8(kbReader_state.fd[1], 1);
    return unloadErrors | kbReader_unloadModule_62F8(kbReader_state.fd[0], 2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_getMaxKeyboards);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526010[];

/**
 * Returns the number of USB keyboards the reader handles (the max-connect word D_00526010).
 *
 * @return the keyboard count
 */
int kbReader_getMaxKeyboards(void) { return D_00526010[0]; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_defaultOnConnect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int sceUsbKbSetLEDStatus(int, int);
extern int sceUsbKbSetLEDMode(int, int);
extern void sceUsbKbSync(int, int *);
extern void sceUsbKbClearRbuf(int);
extern void sceUsbKbSetRepeat(int, int, int);
extern void sceUsbKbSetCodeType(int, int);
/**
 * Default connect callback of the USB keyboard reader: sets up a keyboard that has just connected.
 *
 * When @p on is set: LED status 1 and LED mode 1 (each waited for with sceUsbKbSync), clears its buffered keys
 * (sceUsbKbClearRbuf), sets key repeat to delay 30, rate 2 (sceUsbKbSetRepeat) and the code type to 0 (sceUsbKbSetCodeType).
 *
 * @param keyboard  keyboard index
 * @param on  nonzero when it connected
 */
void kbReader_defaultOnConnect(int keyboard, int on) {
    int res;
    if (on) {
        res = sceUsbKbSetLEDStatus(keyboard, 1);
        if (res == 0) sceUsbKbSync(0, &res);
        res = sceUsbKbSetLEDMode(keyboard, 1);
        if (res == 0) sceUsbKbSync(0, &res);
        sceUsbKbClearRbuf(keyboard);
        sceUsbKbSetRepeat(keyboard, 0x1E, 2);
        sceUsbKbSetCodeType(keyboard, 0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_setQueryPeriod);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526038[];
/**
 * Sets how many reader updates pass between USB keyboard connection queries (kbReader_state +0xB8).
 *
 * @param queryPeriod  the period in updates
 */
void kbReader_setQueryPeriod(int queryPeriod) { D_00526038[0] = queryPeriod; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

extern int kbReader_finishInfoQuery(void);
extern int kbReader_finishRead(void);
extern void kbReader_startInfoQuery(void);
extern void kbReader_startRead(void);
void kbReader_update_63F0(void) __asm__("kbReader_update");
/**
 * Runs one step of the USB keyboard reader.
 *
 * By state (+0x9C): 1 finishes a connection query (kbReader_finishInfoQuery), 2 finishes a read (kbReader_finishRead), 3 polls the idle
 * hook with 1 until it returns nonzero. When idle (state 0), a nonzero idle hook with 0 moves to state 3; otherwise the
 * counter (+0xB4) advances, and past the period (+0xB8) it restarts at 0 and queries the connections (kbReader_startInfoQuery),
 * else it starts reading the next keyboard (kbReader_startRead).
 */
void kbReader_update_63F0(void) {
    switch (kbReader_state.st) {
    case 1: kbReader_finishInfoQuery(); break;
    case 0: break;
    case 2: kbReader_finishRead(); break;
    case 3: if (kbReader_state.idleHook(1) != 0) kbReader_state.st = 0; break;
    }
    if (kbReader_state.st == 0) {
        if (kbReader_state.idleHook(0) != 0) kbReader_state.st = 3;
        if (kbReader_state.st == 0) {
            kbReader_state.tick = kbReader_state.tick + 1;
            if (kbReader_state.period < kbReader_state.tick) { kbReader_state.tick = 0; kbReader_startInfoQuery(); } else { kbReader_startRead(); }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_takeReadBuffer);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

/**
 * Takes a USB keyboard's read buffer when it holds unread data.
 *
 * @param n  keyboard index
 * @return the 0x8C-byte buffer (its ready flag is cleared), or 0 when nothing new arrived
 */
struct sKbRead *kbReader_takeReadBuffer(int n) {
    if (kbReader_state.ents[n].ready == 1) {
        kbReader_state.ents[n].ready = 0;
        return &kbReader_state.ents[n];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_isConnected);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* kbReader_state: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb kbReader_state;

/**
 * Returns whether a USB keyboard is connected (its flag at kbReader_state +0x10).
 *
 * @param keyboard  keyboard index
 * @return 1 when connected, else 0
 */
int kbReader_isConnected(int keyboard) { return kbReader_state.v[keyboard] != 0; }
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", kbReader_updateAnyConnected);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void kbReader_update(void);
extern int kbReader_isConnected(int);
extern int kbReader_getMaxKeyboards(void);
/**
 * Runs the USB keyboard reader once (kbReader_update) and returns 1 when any keyboard is connected, else 0.
 *
 * Nothing in retail calls it.
 *
 * @return 1 when a keyboard is connected
 */
int kbReader_updateAnyConnected(void) {
    int i;
    kbReader_update();
    for (i = 0; i < kbReader_getMaxKeyboards(); i++) {
        if (kbReader_isConnected(i) == 1) return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_getAltKeyMapEntry);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern short D_004513DC[], D_0045133A[], D_00451350[];
/**
 * Returns the 2-byte key-map entry for a USB key code in the alternate layout: codes 46..53 use D_004513DC, codes
 * 135..139 D_0045133A, every other code the default map D_00451350 (all indexed by the code).
 *
 * @param keyCode  USB HID key code
 * @return the key-map entry
 */
short *kbReader_getAltKeyMapEntry(unsigned int keyCode) {
    if (keyCode - 46 < 8) return &D_004513DC[keyCode];
    if (keyCode - 135 >= 5) return &D_00451350[keyCode];
    return &D_0045133A[keyCode];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_getVirtualKey);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern short D_00451420[], D_00451350[];
extern short *kbReader_getAltKeyMapEntry(unsigned int);
/**
 * Translates a USB key code to its virtual-key byte (byte 1 of its key-map entry).
 *
 * With Num Lock on (bit 0 of the LED word at @p flags) the keypad codes 89..99 use D_00451420; otherwise map mode 0
 * uses the default map D_00451350 and any other mode the alternate one (kbReader_getAltKeyMapEntry).
 *
 * @param out  receives the virtual-key byte
 * @param flags  the keyboard read buffer (LED word first)
 * @param mode  key-map mode
 * @param keyCode  USB HID key code
 */
void kbReader_getVirtualKey(char *out, struct sKbRead *flags, int mode, int keyCode) {
    unsigned char *mapEntry;
    unsigned int keypadIndex;
    if ((flags->led & 1) && (keypadIndex = keyCode - 89, keypadIndex < 11)) {
        mapEntry = (unsigned char *)&D_00451420[keypadIndex];
    } else if (mode == 0) {
        mapEntry = (unsigned char *)&D_00451350[keyCode];
    } else {
        mapEntry = (unsigned char *)kbReader_getAltKeyMapEntry(keyCode);
    }
    *out = mapEntry[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_getKeyChar);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char kbReader_keypadCharTable[];
extern int f268(int, int, int, int) __asm__("sceUsbKbCnvRawCode");
void kbReader_getKeyChar_impl(unsigned char *self, int *fl, int kb, unsigned code) __asm__("kbReader_getKeyChar");
/**
 * Translates a USB key code to its character, stored in byte 1 of the output pair @p self.
 *
 * Keypad codes 84..99 come from kbReader_keypadCharTable (its first four are the "/", "*", "-" and "+" keys); the digit and period
 * keys (89 and up) give 0 unless Num Lock (bit 0 of @p fl[0]) is on. Other codes go through sceUsbKbCnvRawCode with
 * the layout @p kb, the modifier word @p fl[1] and the LED word; results of 0x100 or more give 0.
 *
 * @param fl  the keyboard read buffer: LED word, modifier word
 * @param kb  keyboard layout passed to sceUsbKbCnvRawCode
 * @param code  USB HID key code
 */
void kbReader_getKeyChar_impl(unsigned char *self, int *fl, int kb, unsigned code) {
    unsigned keypadIndex = code - 84;
    if (keypadIndex < 0x10) {
        if (code < 89 || (fl[0] & 1)) self[1] = kbReader_keypadCharTable[keypadIndex];
        else self[1] = 0;
    } else {
        int converted = f268(kb, fl[1], fl[0], code & 0xFFFF);
        if (converted < 0x100) self[1] = converted;
        else self[1] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_testModifierKey);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int kbReader_testModifierKey_impl(unsigned flags, unsigned key, unsigned *out) __asm__("kbReader_testModifierKey");
/**
 * Tests a modifier key, named by its Windows virtual-key code, in a USB keyboard modifier word.
 *
 * VK_LWIN 91 bit 0x8, VK_RWIN 92 bit 0x80, VK_SHIFT 16 and VK_LSHIFT 160 bit 0x2, VK_RSHIFT 161 bit 0x20, VK_CONTROL 17
 * bits 0x11, VK_LCONTROL 162 bit 0x1, VK_RCONTROL 163 bit 0x10, VK_MENU 18 and VK_LMENU 164 bit 0x4, VK_RMENU 165 bit
 * 0x40. Quirk: VK_SHIFT and VK_MENU only test the left key, while VK_CONTROL tests both.
 *
 * @param flags  modifier word
 * @param key  virtual-key code
 * @param out  receives the masked bits when @p key is a modifier
 * @return 1 when @p key is a modifier, else 0
 */
int kbReader_testModifierKey_impl(unsigned flags, unsigned key, unsigned *out) {
    int ok = 1;
    unsigned modifierBits = 0;
    switch (key) {
    case kVK_LWin: modifierBits = flags & 0x8; break;
    case kVK_RControl: modifierBits = flags & 0x10; break;
    case kVK_RShift: modifierBits = flags & 0x20; break;
    case kVK_LShift:
    case kVK_Shift: modifierBits = flags & 0x2; break;
    case kVK_Menu:
    case kVK_LMenu: modifierBits = flags & 0x4; break;
    case kVK_Control: modifierBits = flags & 0x11; break;
    case kVK_RWin: modifierBits = flags & 0x80; break;
    case kVK_LControl: modifierBits = flags & 0x1; break;
    case kVK_RMenu: modifierBits = flags & 0x40; break;
    default: ok = 0; break;
    }
    if (ok) *out = modifierBits;
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_testLockKey);
#ifdef SKIP_ASM
/**
 * Tests a lock key, named by its Windows virtual-key code, in a USB keyboard LED word: VK_NUMLOCK 144 bit 0x1,
 * VK_CAPITAL 20 bit 0x2, VK_SCROLL 145 bit 0x4.
 *
 * @param lockFlags  LED word
 * @param mode  virtual-key code
 * @param out  receives the masked bit when @p mode is a lock key
 * @return 1 when @p mode is a lock key, else 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int kbReader_testLockKey(int lockFlags, unsigned int mode, int *out) {
    int handled = 1;
    int lockState = 0;
    switch (mode) {
    case kVK_NumLock: lockState = lockFlags & 1; break;
    case kVK_Capital: lockState = lockFlags & 2; break;
    case kVK_Scroll: lockState = lockFlags & 4; break;
    default: handled = 0; break;
    }
    if (handled) *out = lockState;
    return handled;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_isKeyDown);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int f6700(int, int, int *) __asm__("kbReader_testModifierKey");
extern int f67E0(int, int, int *) __asm__("kbReader_testLockKey");
extern void f65F8(char *, struct sKbRead *, int, int) __asm__("kbReader_getVirtualKey");
/**
 * Returns whether a virtual key is down in a USB keyboard read buffer.
 *
 * Modifier keys are tested in the modifier word (kbReader_testModifierKey) and lock keys in the LED word (kbReader_testLockKey); any
 * other key compares the virtual-key byte of the buffer's @p idx-th key code (kbReader_getVirtualKey) with @p virtKey. The
 * keyboard reader update (cUsbKeyboard_update) asks it for VK_SHIFT (0x10).
 *
 * @param kbdState  the read buffer: LED word, modifier word, count, key codes
 * @param mapMode  key-map mode for kbReader_getVirtualKey
 * @param idx  index of the key code to test
 * @param virtKey  virtual-key code
 * @return nonzero when the key is down
 */
int kbReader_isKeyDown(struct sKbRead *kbdState, int mapMode, int idx, int virtKey) {
    int res = 0;
    char buf[8];
    int key = kbdState->keycode[idx];
    if (f6700(kbdState->mkey, virtKey, &res) == 0) {
        if (f67E0(kbdState->led, virtKey, &res) == 0) {
            f65F8(buf, kbdState, mapMode, key);
            if ((unsigned char)buf[0] == virtKey) res = 1;
        }
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", kbReader_translateKey);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char kbReader_translatedKey;
extern void kbReader_getVirtualKey(char *, struct sKbRead *, int, int);
extern void kbReader_getKeyChar(char *, struct sKbRead *, int, int);
/**
 * Translates one key code of a USB keyboard read buffer into the static pair kbReader_translatedKey: virtual-key byte
 * (kbReader_getVirtualKey) and character (kbReader_getKeyChar).
 *
 * @param readBuf  the read buffer (key codes from +0xC)
 * @param mapMode  key-map mode / layout
 * @param keyIndex  index of the key code
 * @return kbReader_translatedKey (overwritten by the next call)
 */
char *kbReader_translateKey(struct sKbRead *readBuf, int mapMode, int keyIndex) {
    unsigned int keyCode = readBuf->keycode[keyIndex];
    kbReader_getVirtualKey(&kbReader_translatedKey, readBuf, mapMode, keyCode);
    kbReader_getKeyChar(&kbReader_translatedKey, readBuf, mapMode, keyCode);
    return &kbReader_translatedKey;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_call);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* SIF RPC client (voipRpc_bind binds it): voipRpc_boundFlag = bound, D_00526100 the client data, D_00526080 the
   send buffer. */
extern unsigned char voipRpc_boundFlag;

extern int D_00526100[];

struct sRpcSend { int a; int b; };

extern struct sRpcSend D_00526080[];

extern int f6F08(int *) __asm__("sceSifCheckStatRpc");
extern int f6D18(int *, int, int, struct sRpcSend *, int, int, int, int, int) __asm__("sceSifCallRpc");
void voipRpc_call_impl(int a, int b, int c, int d, int e) __asm__("voipRpc_call");
/**
 * Calls the voice-chat IOP module (VOIPF) through its SIF RPC client.
 *
 * Only when bound (voipRpc_boundFlag): waits for the previous call to finish (sceSifCheckStatRpc), puts two words in the send
 * buffer D_00526080 and calls RPC function @p rpcFunc (sceSifCallRpc on the client D_00526100, 0x80-byte send buffer),
 * retrying until the call is accepted.
 *
 * @param rpcFunc  RPC function number
 * @param argWord0  first argument word
 * @param argWord1  second argument word
 * @param recvBuf  receive buffer
 * @param recvSize  receive buffer size
 */
void voipRpc_call_impl(int rpcFunc, int argWord0, int argWord1, int recvBuf, int recvSize) {
    struct sRpcSend *sendBuf = D_00526080;
    if (voipRpc_boundFlag) {
        while (f6F08(D_00526100) > 0) ;
        sendBuf->a = argWord0;
        sendBuf->b = argWord1;
        do {
            if (f6D18(D_00526100, rpcFunc, 0, D_00526080, 0x80, recvBuf, recvSize, 0, 0) >= 0) break;
        } while (1);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_bind);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* SIF RPC client (voipRpc_bind binds it): voipRpc_boundFlag = bound, D_00526100 the client data, D_00526080 the
   send buffer. */
extern unsigned char voipRpc_boundFlag;

extern int D_00526100[];

extern int D_005261C0[];

extern int voipRpc_recvBuffer[];

extern int *voipRpc_uncachedStatusWord;

extern int sceSifBindRpc(int *, int, int);
extern void func_003F6960b(int, int, int, int *, int) __asm__("voipRpc_call");
/**
 * Binds the voice-chat SIF RPC client (server id 567, sceSifBindRpc) and registers the status word with the module.
 *
 * Marks the client bound first, keeps an uncached pointer to D_005261C0 in voipRpc_uncachedStatusWord and sends it with RPC function 0.
 * initOnline calls it when VOIPF.IRX loaded. Quirk: the bind result is not checked.
 */
void voipRpc_bind(void) {
    int *statusWord;
    voipRpc_boundFlag = 1;
    sceSifBindRpc(D_00526100, 567, 0);
    // ORIGINAL-PTR: the IOP module gets the status word's EE address as an RPC argument word and writes it there, so
    // the EE reads it through the uncached segment (address & 0x0FFFFFFF | 0x20000000, a hardware address segment)
    statusWord = (int *)(((int)D_005261C0 & 0xFFFFFFF) | 0x20000000);
    voipRpc_uncachedStatusWord = statusWord;
    func_003F6960b(0, (int)statusWord, 0, voipRpc_recvBuffer, 0x80);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_unbind);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* SIF RPC client (voipRpc_bind binds it): voipRpc_boundFlag = bound, D_00526100 the client data, D_00526080 the
   send buffer. */
extern unsigned char voipRpc_boundFlag;

/**
 * Marks the voice-chat RPC client unbound (voipRpc_boundFlag = 0), which turns every later call into a no-op.
 */
void voipRpc_unbind(void) { voipRpc_boundFlag = 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_connect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int voipRpc_recvBuffer[];

extern int voipRpc_call(int, int, int, int *, int);
/**
 * Connects voice chat to a peer: RPC function 2 with the address and the two 16-bit ports packed in one word.
 *
 * The voice manager (cVoipMan_connect) passes ports 6000 / 6000.
 *
 * @param peerAddr  peer address
 * @param firstPort  first port (low half)
 * @param secondPort  second port (high half)
 * @return no meaningful value (voipRpc_call returns nothing)
 */
int voipRpc_connect(int peerAddr, unsigned short firstPort, int secondPort) { return voipRpc_call(2, peerAddr, firstPort | (secondPort << 16), voipRpc_recvBuffer, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_disconnect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int voipRpc_recvBuffer[];

extern int voipRpc_call(int, int, int, int *, int);
/**
 * Stops voice chat: RPC function 3.
 *
 * @return no meaningful value (voipRpc_call returns nothing)
 */
int voipRpc_disconnect(void) { return voipRpc_call(3, 0, 0, voipRpc_recvBuffer, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_turnVoiceOn);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int voipRpc_recvBuffer[];

extern int voipRpc_call(int, int, int, int *, int);
/**
 * Turns the voice on: RPC function 4.
 */
void voipRpc_turnVoiceOn() { voipRpc_call(4, 0, 0, voipRpc_recvBuffer, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_turnVoiceOff);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int voipRpc_recvBuffer[];

extern int voipRpc_call(int, int, int, int *, int);
/**
 * Turns the voice off: RPC function 5.
 *
 * @return no meaningful value (voipRpc_call returns nothing)
 */
int voipRpc_turnVoiceOff(void) { return voipRpc_call(5, 0, 0, voipRpc_recvBuffer, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_getStatusBit0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int voipRpc_getStatusWord(void);
/**
 * Returns bit 0 of the voice module's status word (voipRpc_getStatusWord).
 *
 * @return the bit
 */
int voipRpc_getStatusBit0(void) { return voipRpc_getStatusWord() & 1; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_getVoiceLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int voipRpc_recvBuffer[];

extern void func_003F6960b(int, int, int, int *, int) __asm__("voipRpc_call");
/**
 * Reads the voice level and its maximum from the voice module: RPC function 7, then the first two received words.
 *
 * @param levelOut  receives the level
 * @param maxOut  receives the maximum
 */
void voipRpc_getVoiceLevel(int *levelOut, int *maxOut) {
    func_003F6960b(7, 0, 0, voipRpc_recvBuffer, 0x80);
    *levelOut = voipRpc_recvBuffer[0];
    *maxOut = voipRpc_recvBuffer[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_setVoiceLevel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int voipRpc_recvBuffer[];

extern int voipRpc_call(int, int, int, int *, int);
/**
 * Sets the voice level: RPC function 6 with the level and the maximum.
 *
 * @param level  voice level
 * @param maxLevel  maximum level
 */
void voipRpc_setVoiceLevel(int level, int maxLevel) { voipRpc_call(6, level, maxLevel, voipRpc_recvBuffer, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", voipRpc_getStatusWord);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int *voipRpc_uncachedStatusWord;

/**
 * Returns the voice module's status word, which the IOP writes into D_005261C0 (read through the uncached pointer
 * voipRpc_uncachedStatusWord).
 *
 * @return the status word
 */
int voipRpc_getStatusWord(void) { return *voipRpc_uncachedStatusWord; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_setStarted);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char slogin_startedFlag;

/**
 * Sets the network-account library's started flag slogin_startedFlag (slogin_init calls it first).
 *
 * @return 0
 */
int slogin_setStarted(void) { slogin_startedFlag = 1; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_clearStarted);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char slogin_startedFlag;

/**
 * Clears the network-account library's started flag slogin_startedFlag (slogin_shutdown calls it last).
 *
 * @return 0
 */
int slogin_clearStarted(void) { slogin_startedFlag = 0; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_createSession);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* The caller's 0x64-byte save-parameter block. [U] +0x4 the port, +0x48 the save name (saveRec_startReopen), +0x60 the record
   table's CRC (saveRec_startClose, saveRec_update). */
struct sMcParams {
    int w[18];                          /* 0x00 */
    char name[24];                      /* 0x48 */
    int crc;                            /* 0x60 */
};

/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
struct sNetHdr { int w[17]; };

/* What slogin_init / slogin_createSession get: the memory-card callback table and the heap. */
struct sNetInit {
    struct sNetHdr *callbacks;          /* 0x0 */
    int heap;                           /* 0x4 */
};

/* The step state of a phase (+0x14 of it): slogin_runModeStep saves it as one block (cur.step -> saved.step) when a step
   begins or fails. The handler slots hold code addresses that callers cast to the handler's type. */
struct sNetStep {
    int phase;                          /* +0x00 (session +0xEC): 4 begin, 0 start, 1 poll, 2 finish, 5 done */
    void *start;                        /* +0x04: the mode's start function (phase 0) */
    void *poll;                         /* +0x08: the poll function (phase 1, slogin_pollCardMode) */
    void *finish;                       /* +0x0C: the phase-2 function */
    void *completion;                   /* +0x10: run once by the poll when the card work is done */
    void *cleanup;                      /* +0x14: run once by the poll when the poll itself fails */
};

/* Handler names are ours, from [U] slogin_enterCardMode (which installs them per mode) and slogin_advanceOperation (which runs them). */
struct sNetPhase {
    unsigned mode;                      /* +0x00 */
    void *stepFn;                       /* +0x04 (session +0xDC): a card mode's step function (slogin_runModeStep) */
    void *nextModeFn;                   /* +0x08: picks the next mode when one finishes (slogin_enterNextCardMode, ...) */
    void *errorFn;                      /* +0x0C: the mode's error handler (slogin_onCardModeError, ...) */
    void *dispatchFn;                   /* +0x10: mode 9's dispatcher that chooses the first mode (slogin_enterFirstMode) */
    struct sNetStep step;               /* +0x14 */
};

struct sNetAcct {
    struct sNetHdr hdr;                 /* 0x00 */
    struct sMcParams params;            /* 0x44 */
    char name[0x11];                    /* 0xA8 */
    char pass[0x11];                    /* 0xB9 */
    struct sAcctOut *out;               /* 0xCC */
    unsigned st;                        /* 0xD0 */
    void *operationPoll;                /* 0xD4: (name ours) [U] slogin_beginOperation installs slogin_advanceOperation, which
                                           slogin_update runs every frame */
    struct sNetPhase cur;               /* 0xD8 */
    unsigned lastOperation;             /* 0x104: (name ours) [U] slogin_beginOperation stores the operation started;
                                           slogin_getLastOperation returns it (st goes back to 4 when idle, this does not) */
    int f108;
    struct sNetPhase saved;             /* 0x10C */
    int portList;                       /* 0x138: (names ours) [U] slogin_createSession 0 and -1: the -1-terminated unit */
    int portListEnd;                    /* 0x13C: list {0, -1} slogin_openSaveRecords passes to saveRec_open */
    int heap;                           /* 0x140 */
    char *recordBuf;                    /* 0x144: (name ours) [U] the 17-byte ACCT / PASS record buffer
                                           (saveRec_allocBuffer allocates it, slogin_freeNameBuffer / slogin_freePasswordBuffer free it) */
    int pendingError;                   /* 0x148: (name ours) [U] slogin_onCardModeError keeps an error here while the save
                                           closes; slogin_reportKeptError hands it back */
};

extern struct sNetAcct *slogin_session;

extern struct sNetAcct *f6728(int, int, int) __asm__("TibExtMemAlloc");
extern unsigned f6210x(struct sNetAcct *, int, int) __asm__("memset");
int slogin_createSession_impl(struct sNetInit *initParams) __asm__("slogin_createSession");
/**
 * Creates the network-account session slogin_session (0x14C bytes).
 *
 * Allocates it from the heap @p initParams->heap with TibExtMemAlloc and zeroes it, keeps the heap (+0x140), copies the
 * 17 memory-card callbacks from the table @p initParams->callbacks into its head, and sets it idle: operation 4 (+0xD0,
 * +0x104), mode 10 (+0xD8), phase 5 (+0xEC), +0x148 and +0x138 0, +0x13C -1 (so +0x138 is the port list {0, -1} for
 * slogin_openSaveRecords).
 *
 * @param initParams  {callback table, heap}
 * @return 0
 */
int slogin_createSession_impl(struct sNetInit *initParams) {
    struct sNetAcct *session;
    struct sNetAcct *sessionReload;
    slogin_session = f6728(initParams->heap, 332, 4);
    f6210x(slogin_session, 0, 332);
    session = slogin_session;
    session->heap = initParams->heap;
    session->hdr = *initParams->callbacks;
    sessionReload = slogin_session;
    sessionReload->st = 4;
    sessionReload->lastOperation = 4;
    sessionReload->pendingError = 0;
    sessionReload->portList = 0;
    sessionReload->cur.mode = 10;
    sessionReload->cur.step.phase = 5;
    sessionReload->portListEnd = -1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_freeSession);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int TibExtMemFree(int, char *, int, int);
/**
 * Frees the network-account session slogin_session back to its heap (TibExtMemFree) and clears the pointer.
 *
 * @return 0
 */
int slogin_freeSession(void) {
    TibExtMemFree(slogin_session->heap, (char *)slogin_session, 332, 4);
    slogin_session = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_openSaveRecords);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

typedef struct { char b[0x1A]; } K6DC8;
extern K6DC8 D_00497060;
extern int fB800(void) __asm__("xorIface_get");
extern int fC0E8(void *) __asm__("saveRec_open");
extern int slogin_mapSaveRecordError_6DC8(int) __asm__("slogin_mapSaveRecordError");
typedef struct { int a; int *p; struct sNetAcct *d; K6DC8 *buf; int len; int r; int v; } R6DC8;
/**
 * Opens the save-record manager for the network-account session (saveRec_open).
 *
 * Passes: at most 2 records, the port list at +0x138 (port 0 only), the session's 17 card callbacks, the 26-byte cipher
 * key "Copyright Electronic Arts" (with its NUL), the XOR cipher interface (xorIface_get) and the session's heap. The
 * result goes through slogin_mapSaveRecordError.
 *
 * @return the mapped error, 0 on success
 */
int slogin_openSaveRecords(void) {
    K6DC8 buf;
    R6DC8 req;
    struct sNetAcct *session = slogin_session;
    buf = D_00497060;
    req.a = 2;
    req.p = &session->portList;
    req.d = session;
    req.len = 26;
    req.buf = &buf;
    req.r = fB800();
    req.v = slogin_session->heap;
    return slogin_mapSaveRecordError_6DC8(fC0E8(&req));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_closeSaveRecords);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int saveRec_close(void);
extern int slogin_mapSaveRecordError(int);
/**
 * Closes the save-record manager (saveRec_close) and returns the error mapped by slogin_mapSaveRecordError.
 *
 * @return the mapped error, 0 on success
 */
int slogin_closeSaveRecords(void) { return slogin_mapSaveRecordError(saveRec_close()); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startOperation);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int slogin_beginOperation(int);
/**
 * Starts a network-account operation when the session is idle (operation word +0xD0 == 4), with slogin_beginOperation.
 *
 * Operations: 0 create the account save, 1 save the name and password, 2 load them, 3 delete the save.
 *
 * @param operation  the operation
 * @return 0, or 22 when an operation is already running
 */
int slogin_startOperation(int operation) {
    int errorCode = 0;
    if (slogin_session->st == 4) {
        slogin_beginOperation(operation);
    } else {
        errorCode = 22;
    }
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_advanceOperation);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/**
 * Advances the running network-account operation (installed at +0xD4 by slogin_beginOperation, called by slogin_update).
 *
 * Returns 0xF when idle. In a card mode (below 9) it runs the step function (+0xDC, slogin_runModeStep); in mode 9 it calls
 * the dispatcher (+0xE8, slogin_enterFirstMode) to choose the first mode. Likely bug: *@p out is cleared before @p out is
 * checked for 0.
 *
 * @param out  receives 1 while running, 2 when the operation has finished
 * @return 0 or an error code
 */
int slogin_advanceOperation(int *out) {
    int err = 0; struct sNetAcct *session;
    *out = 0;
    if (out == 0) err = 7;
    session = slogin_session;
    if (session->st >= 4) err = 0xF;
    if (err == 0) {
        if (slogin_session->cur.mode < 9) err = ((int (*)(int *, struct sNetAcct *))session->cur.stepFn)(out, session);
        else ((void (*)(int *, int *))slogin_session->cur.dispatchFn)(out, &err);
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_runModeStep);
#ifdef SKIP_ASM
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;
extern int slogin_finishPhase(void);
/**
 * Runs one step of the current network-account mode through its phase (+0xEC).
 *
 * Phase 4 begins (saving the phase block to +0x10C) and moves to 0; phase 0 calls the start function (+0xF0), then goes
 * to 1, or on failure to 2, running the step once more for its cleanup and finishing (slogin_finishPhase); phase 1 polls
 * (+0xF4, slogin_pollCardMode) and finishes when the card work is done (@p out 2), failing like phase 0; phase 2 calls +0xF8;
 * phase 5 returns 0x10, anything else 17. Then a finished step calls the next-mode handler (+0xE0) and a failed one the
 * error handler (+0xE4). Likely bug: *@p out is written before @p out is checked for 0.
 *
 * @param out  receives 1 while running, 2 when the mode finished
 * @return 0 or an error code
 */
int slogin_runModeStep(int *out) {
    int startFailOut;
    int st;
    int pollFailOut;
    int result;
    result = 0;
    *out = 0;
    if (out == 0) result = 7;
    if (slogin_session->cur.mode >= 9) {
        if (slogin_session->cur.mode == 10) result = 0xC;
        else result = 13;
    }
    if (result == 0) {
        switch (slogin_session->cur.step.phase) {
        case 4:
            slogin_session->saved.step = slogin_session->cur.step;
            slogin_session->cur.step.phase = 0;
            *out = 1;
            break;
        case 0:
            result = ((int (*)(void))slogin_session->cur.step.start)();
            if (result == 0) {
                slogin_session->saved.step = slogin_session->cur.step;
                slogin_session->cur.step.phase = 1;
                *out = 1;
            } else {
                slogin_session->saved.step = slogin_session->cur.step;
                slogin_session->cur.step.phase = 2;
                ((int (*)(int *))slogin_session->cur.stepFn)(&startFailOut);
                slogin_finishPhase();
            }
            break;
        case 1:
            result = ((int (*)(int *))slogin_session->cur.step.poll)(&st);
            if (result == 0) {
                *out = 1;
                if (st == 2) {
                    *out = 2;
                    slogin_finishPhase();
                }
            } else {
                slogin_session->saved.step = slogin_session->cur.step;
                slogin_session->cur.step.phase = 2;
                ((int (*)(int *))slogin_session->cur.stepFn)(&pollFailOut);
                slogin_finishPhase();
            }
            break;
        case 2:
            result = ((int (*)(void))slogin_session->cur.step.finish)();
            break;
        case 5:
            result = 0x10;
            break;
        default:
            result = 17;
            break;
        }
    }
    if (result == 0) {
        if (*out == 2) ((void (*)(int *, int *))slogin_session->cur.nextModeFn)(out, &result);
    } else {
        ((void (*)(int *, int *))slogin_session->cur.errorFn)(out, &result);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onCardModeError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int f7A60c(int, int) __asm__("slogin_enterCardMode");
/**
 * Error handler of network-account modes 0..5: keeps the error in +0x148, clears it and switches to mode 7 (close the
 * save) so the operation reports the error after closing.
 *
 * @param statusOut  receives 1 (still running)
 * @param errorInOut  the error; cleared
 * @return slogin_enterCardMode's result
 */
int slogin_onCardModeError(int *statusOut, int *errorInOut) {
    *statusOut = 1;
    slogin_session->pendingError = *errorInOut;
    *errorInOut = 0;
    return f7A60c(7, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onCloseModeError);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_returnToIdle(void);
/**
 * Error handler of network-account modes 6 and 8: returns the session to idle (slogin_returnToIdle).
 *
 * @return 0
 */
int slogin_onCloseModeError(void) { return slogin_returnToIdle(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_enterNextCardMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int f7A60c(int, int) __asm__("slogin_enterCardMode");
int slogin_enterNextCardMode_impl(int *a, int *b) __asm__("slogin_enterNextCardMode");
/**
 * Next-mode handler of network-account modes 0..5: picks the following card step of the operation.
 *
 * Mode 0 (create) goes to 1 (open); mode 1 goes to 2 (write ACCT) for operations 0 and 1 or to 4 (read ACCT) for
 * operation 2; 2 goes to 3 (write PASS), 4 to 5 (read PASS), and 3 and 5 to 6 (close). Enters the new mode with
 * slogin_enterCardMode, saving the current phase block. Any other case fails with 0xC.
 *
 * @param statusOut  receives 1 (still running), or 0 on failure
 * @param errorOut  receives the error on failure
 * @return slogin_enterCardMode's result, or 0xC
 */
int slogin_enterNextCardMode_impl(int *statusOut, int *errorOut) {
    int x; int err = 0xC;
    *statusOut = 1;
    switch (slogin_session->cur.mode) {
    case 0: x = 1; break;
    case 1: { unsigned sel = slogin_session->st;
        if (sel >= 2) { if (sel != 2) goto fail; x = 4; } else x = 2; break; }
    case 2: x = 3; break;
    case 4: x = 5; break;
    case 3: case 5: x = 6; break;
    default: goto fail;
    }
    return f7A60c(x, 1);
fail:
    *errorOut = err; *statusOut = 0; return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onCloseModeDone);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_returnToIdle(void);
/**
 * Next-mode handler of network-account modes 6 and 8: returns the session to idle and reports the operation finished.
 *
 * @param statusOut  receives 2 (finished)
 */
void slogin_onCloseModeDone(int *statusOut) { slogin_returnToIdle(); *statusOut = 2; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_reportKeptError);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int slogin_returnToIdle(void);
/**
 * Handler of network-account mode 7 (closing after an error): hands back the error kept in +0x148, clears it and
 * returns the session to idle.
 *
 * @param statusOut  status output (not written)
 * @param errorOut  receives the kept error
 * @return slogin_returnToIdle's result (0)
 */
int slogin_reportKeptError(int statusOut, int *errorOut) {
    *errorOut = slogin_session->pendingError;
    slogin_session->pendingError = 0;
    return slogin_returnToIdle();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_enterFirstMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int f7A60c(int, int) __asm__("slogin_enterCardMode");
int slogin_enterFirstMode_impl(int *a, int *b) __asm__("slogin_enterFirstMode");
/**
 * Mode-9 dispatcher of the network-account session: enters the first card mode of the operation.
 *
 * Operation 0 starts with mode 0 (create the save), operations 1 and 2 with mode 1 (open it), operation 3 with mode 8
 * (delete it); entered with slogin_enterCardMode, saving the phase block. Outside mode 9 or for another operation it fails
 * with 0xC.
 *
 * @param statusOut  receives 1 (still running), or 0 on failure
 * @param errorOut  receives the error on failure
 * @return slogin_enterCardMode's result, or 0xC
 */
int slogin_enterFirstMode_impl(int *statusOut, int *errorOut) {
    int ret;
    if (slogin_session->cur.mode == 9) {
        *statusOut = 1;
        switch (slogin_session->st) {
        case 0: ret = f7A60c(0, 1); break;
        case 1: case 2: ret = f7A60c(1, 1); break;
        case 3: ret = f7A60c(8, 1); break;
        default: ret = 0xC; *errorOut = ret; *statusOut = 0; break;
        }
    } else {
        ret = 0xC; *errorOut = ret; *statusOut = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startCreateSave);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern char str_slogin[];

extern int slogin_onSaveCreated(void);
extern int saveRec_startCreate(char *, int, int);
extern int slogin_mapSaveRecordError_73A8(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account mode 0: creates the "slogin" save on any port (saveRec_startCreate), with slogin_onSaveCreated
 * as completion.
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startCreateSave(void) {
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = slogin_onSaveCreated;
    session->cur.step.cleanup = 0;
    return slogin_mapSaveRecordError_73A8(saveRec_startCreate(str_slogin, -1, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startOpenSave);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern char str_slogin[];

extern int slogin_onSaveOpened_73F0(void) __asm__("slogin_onSaveOpened");
extern int saveRec_startOpen_73F0(char *, int, int) __asm__("saveRec_startOpen");
extern int slogin_mapSaveRecordError_73F0(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account mode 1: opens the "slogin" save on any port (saveRec_startOpen), with slogin_onSaveOpened as
 * completion.
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startOpenSave(void) {
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = slogin_onSaveOpened_73F0;
    session->cur.step.cleanup = 0;
    return slogin_mapSaveRecordError_73F0(saveRec_startOpen_73F0(str_slogin, -1, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startDeleteSave);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern char str_slogin[];

extern int saveRec_startDelete(char *, int, int);
extern int slogin_mapSaveRecordError_7438(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account mode 8: deletes the "slogin" save on any port (saveRec_startDelete), without a
 * completion.
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startDeleteSave(void) {
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = 0;
    session->cur.step.cleanup = 0;
    return slogin_mapSaveRecordError_7438(saveRec_startDelete(str_slogin, -1, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startWriteName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/* A 0x11-byte name copied as a block. */
struct sName17 { char b[17]; };

/* The caller's 34-byte account buffer (slogin_startReadAccount): the user name, then the password at +0x11. */
struct sAcctOut {
    struct sName17 name;                /* 0x00 */
    struct sName17 pass;                /* 0x11 */
};

extern char fn7920[] __asm__("slogin_freeNameBuffer");
extern char fn78E0[] __asm__("slogin_onNameWritten");
extern int saveRec_allocBuffer(char *, int, int, int);
extern int fC548(struct sMcParams *, int, int, char *, int) __asm__("saveRec_startWriteRecord");
extern int slogin_mapSaveRecordError_7478(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account mode 2: writes the user name as the save's 'ACCT' record.
 *
 * Allocates a 17-byte record buffer (saveRec_allocBuffer, into +0x144), copies the name (+0xA8) into it and starts the write
 * (saveRec_startWriteRecord with the open save's parameters at +0x44). Completion slogin_onNameWritten and error cleanup slogin_freeNameBuffer
 * free the buffer.
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startWriteName(void) {
    int result;
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = fn78E0;
    session->cur.step.cleanup = fn7920;
    result = slogin_mapSaveRecordError_7478(saveRec_allocBuffer((char *)&session->recordBuf, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = slogin_session;
        struct sName17 *dst = (struct sName17 *)sessionReload->recordBuf;
        *dst = *(struct sName17 *)sessionReload->name;
        result = slogin_mapSaveRecordError_7478(fC548(&slogin_session->params, 0x41434354, 0, slogin_session->recordBuf, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startWritePassword);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/* A 0x11-byte name copied as a block. */
extern char fn7958[] __asm__("slogin_freePasswordBuffer");
extern char fn7900[] __asm__("slogin_onPasswordWritten");
extern int saveRec_allocBuffer(char *, int, int, int);
extern int fC548(struct sMcParams *, int, int, char *, int) __asm__("saveRec_startWriteRecord");
extern int slogin_mapSaveRecordError_7528(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account mode 3: writes the password as the save's 'PASS' record.
 *
 * Like slogin_startWriteName with the password (+0xB9); completion slogin_onPasswordWritten and cleanup slogin_freePasswordBuffer free the buffer.
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startWritePassword(void) {
    int result;
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = fn7900;
    session->cur.step.cleanup = fn7958;
    result = slogin_mapSaveRecordError_7528(saveRec_allocBuffer((char *)&session->recordBuf, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = slogin_session;
        struct sName17 *dst = (struct sName17 *)sessionReload->recordBuf;
        *dst = *(struct sName17 *)sessionReload->pass;
        result = slogin_mapSaveRecordError_7528(fC548(&slogin_session->params, 0x50415353, 0, slogin_session->recordBuf, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startReadName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern char fn7920[] __asm__("slogin_freeNameBuffer");
extern char fn7840[] __asm__("slogin_onNameRead");
extern int saveRec_allocBuffer(char *, int, int, int);
extern int saveRec_startReadRecord(struct sMcParams *, int, int, char *, int);
extern int slogin_mapSaveRecordError_75D8(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account mode 4: reads the save's 'ACCT' record into a new 17-byte buffer (+0x144) with
 * saveRec_startReadRecord.
 *
 * Completion slogin_onNameRead copies it to the caller's output; cleanup slogin_freeNameBuffer frees it.
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startReadName(void) {
    int result;
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = fn7840;
    session->cur.step.cleanup = fn7920;
    result = slogin_mapSaveRecordError_75D8(saveRec_allocBuffer((char *)&session->recordBuf, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = slogin_session;
        result = slogin_mapSaveRecordError_75D8(saveRec_startReadRecord(&sessionReload->params, 0x41434354, 0, sessionReload->recordBuf, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startReadPassword);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern char fn7958[] __asm__("slogin_freePasswordBuffer");
extern char fn7890[] __asm__("slogin_onPasswordRead");
extern int saveRec_allocBuffer(char *, int, int, int);
extern int saveRec_startReadRecord(struct sMcParams *, int, int, char *, int);
extern int slogin_mapSaveRecordError_7658(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account mode 5: reads the save's 'PASS' record into a new 17-byte buffer (+0x144) with
 * saveRec_startReadRecord.
 *
 * Completion slogin_onPasswordRead copies it to the caller's output + 0x11; cleanup slogin_freePasswordBuffer frees it.
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startReadPassword(void) {
    int result;
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = fn7890;
    session->cur.step.cleanup = fn7958;
    result = slogin_mapSaveRecordError_7658(saveRec_allocBuffer((char *)&session->recordBuf, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = slogin_session;
        result = slogin_mapSaveRecordError_7658(saveRec_startReadRecord(&sessionReload->params, 0x50415353, 0, sessionReload->recordBuf, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startCloseSave);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int saveRec_startClose(struct sMcParams *);
extern int slogin_mapSaveRecordError_76D8(int) __asm__("slogin_mapSaveRecordError");
/**
 * Start function of network-account modes 6 and 7: closes the open save (saveRec_startClose with its parameters at +0x44).
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_startCloseSave(void) {
    struct sNetAcct *session = slogin_session;
    session->cur.step.completion = 0;
    session->cur.step.cleanup = 0;
    return slogin_mapSaveRecordError_76D8(saveRec_startClose(&session->params));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_pollCardMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int saveRec_update(int *, int *);
extern int slogin_mapSaveRecordError_7708(int) __asm__("slogin_mapSaveRecordError");
/**
 * Poll function of every network-account card mode: polls the save-record manager (saveRec_update).
 *
 * Status 1 (busy) is passed on; status 2 (done) first runs the mode's completion (+0xFC) once; status 0 gives error 23
 * and anything else 18. When the poll itself fails, the mode's cleanup (+0x100) runs once instead.
 *
 * @param out  receives 1 while busy, 2 when done
 * @return 0 or the mapped error
 */
int slogin_pollCardMode(int *out) {
    int st[2];
    int err;
    unsigned pollStatus;
    st[0] = 0;
    err = out ? 0 : 7;
    if (err == 0)
        err = slogin_mapSaveRecordError_7708(saveRec_update(st, st + 1));
    if (err == 0) {
        pollStatus = st[0];
        *out = 0;
        switch (pollStatus) {
        case 0:
            err = 23;
            break;
        case 2:
            if (slogin_session->cur.step.completion) {
                err = ((int (*)(void))slogin_session->cur.step.completion)();
                slogin_session->cur.step.completion = 0;
            }
            if (err != 0)
                break;
        case 1:
            *out = pollStatus;
            break;
        default:
            err = 18;
            break;
        }
    } else {
        if (slogin_session->cur.step.cleanup) {
            err = ((int (*)(void))slogin_session->cur.step.cleanup)();
            slogin_session->cur.step.cleanup = 0;
        }
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_defaultPhase2);
#ifdef SKIP_ASM
/**
 * Phase-2 function of the network-account card modes: does nothing.
 *
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int slogin_defaultPhase2(void) { return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onSaveCreated);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int saveRec_copySaveParams(struct sMcParams *);
extern int slogin_mapSaveRecordError_77F8(int) __asm__("slogin_mapSaveRecordError");
/**
 * Completion of network-account mode 0: copies the save-record manager's open-save parameters (saveRec_copySaveParams) into the
 * session (+0x44).
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_onSaveCreated(void) { return slogin_mapSaveRecordError_77F8(saveRec_copySaveParams(&slogin_session->params)); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onSaveOpened);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_onSaveCreated(void);
/**
 * Completion of network-account mode 1: the same as slogin_onSaveCreated (stores the open-save parameters).
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_onSaveOpened(void) { return slogin_onSaveCreated(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onNameRead);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/* A 0x11-byte name copied as a block. */
extern int slogin_freeNameBuffer(int);
/**
 * Completion of network-account mode 4: copies the 17 bytes read for 'ACCT' to the caller's output (+0xCC) and frees
 * the record buffer (slogin_freeNameBuffer).
 */
void slogin_onNameRead(void) {
    slogin_session->out->name = *(struct sName17 *)slogin_session->recordBuf;
    slogin_freeNameBuffer(0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onPasswordRead);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/* A 0x11-byte name copied as a block. */
extern int slogin_freePasswordBuffer(int);
/**
 * Completion of network-account mode 5: copies the 17 bytes read for 'PASS' to the caller's output + 0x11 and frees the
 * record buffer (slogin_freePasswordBuffer).
 */
void slogin_onPasswordRead(void) {
    slogin_session->out->pass = *(struct sName17 *)slogin_session->recordBuf;
    slogin_freePasswordBuffer(0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onNameWritten);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_freeNameBuffer(int);
/**
 * Completion of network-account mode 2: frees the record buffer (slogin_freeNameBuffer).
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_onNameWritten(void) { return slogin_freeNameBuffer(0); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_onPasswordWritten);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_freePasswordBuffer(int);
/**
 * Completion of network-account mode 3: frees the record buffer (slogin_freePasswordBuffer).
 *
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_onPasswordWritten(void) { return slogin_freePasswordBuffer(0); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_freeNameBuffer);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int saveRec_freeBuffer_7920(char *, int, int, int) __asm__("saveRec_freeBuffer");
extern int slogin_mapSaveRecordError_7920(int) __asm__("slogin_mapSaveRecordError");
/**
 * Frees the network-account record buffer (+0x144, 17 bytes) with saveRec_freeBuffer. Used for the 'ACCT' modes.
 *
 * @param ignored  ignored (callers pass 0)
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_freeNameBuffer(int ignored) {
    return slogin_mapSaveRecordError_7920(saveRec_freeBuffer_7920(slogin_session->recordBuf, slogin_session->heap, 17, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_freePasswordBuffer);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int saveRec_freeBuffer_7958(char *, int, int, int) __asm__("saveRec_freeBuffer");
extern int slogin_mapSaveRecordError_7958(int) __asm__("slogin_mapSaveRecordError");
/**
 * Frees the network-account record buffer (+0x144, 17 bytes) with saveRec_freeBuffer; identical to slogin_freeNameBuffer and used
 * for the 'PASS' modes.
 *
 * @param ignored  ignored (callers pass 0)
 * @return the error mapped by slogin_mapSaveRecordError
 */
int slogin_freePasswordBuffer(int ignored) {
    return slogin_mapSaveRecordError_7958(saveRec_freeBuffer_7958(slogin_session->recordBuf, slogin_session->heap, 17, 0));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", slogin_beginOperation);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern char fn6EE0[] __asm__("slogin_advanceOperation");
extern char fn7328[] __asm__("slogin_enterFirstMode");
int f7990(unsigned a) __asm__("slogin_beginOperation");
/**
 * Begins network-account operation @p operation.
 *
 * Stores it in +0xD0 and +0x104, installs the operation poll slogin_advanceOperation at +0xD4 (only for operations below 4), and
 * sets mode 9 (choose the first card step) with phase 4, the dispatcher slogin_enterFirstMode at +0xE8 and every other handler
 * cleared.
 *
 * @param operation  operation (0 create, 1 save, 2 load, 3 delete)
 * @return 0, or 0xF for an unknown operation
 */
int f7990(unsigned operation) {
    int errorCode = 0;
    unsigned i;
    slogin_session->st = operation;
    slogin_session->lastOperation = operation;
    if (operation < 4) {
        slogin_session->operationPoll = fn6EE0;
    } else {
        errorCode = 0xF;
    }
    slogin_session->cur.mode = 9;
    slogin_session->cur.step.phase = 4;
    slogin_session->cur.dispatchFn = fn7328;
    slogin_session->cur.nextModeFn = 0;
    slogin_session->cur.errorFn = 0;
    slogin_session->cur.stepFn = 0;
    for (i = 0; i < 3; i++)
        ;
    slogin_session->cur.step.start = 0;
    slogin_session->cur.step.poll = 0;
    slogin_session->cur.step.finish = 0;
    slogin_session->cur.step.completion = 0;
    slogin_session->cur.step.cleanup = 0;
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_endOperation);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int slogin_returnToIdle(void);
/**
 * Ends the network-account operation: marks the session idle (operation 4), removes the operation poll and the
 * dispatcher and returns to mode 10 (slogin_returnToIdle).
 *
 * @return 0
 */
int slogin_endOperation(void) {
    struct sNetAcct *session = slogin_session;
    session->st = 4;
    session->operationPoll = 0;
    session->cur.dispatchFn = 0;
    return slogin_returnToIdle();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", slogin_enterCardMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

// (prototype from their definition at line 1929)
int slogin_onCloseModeError_7A60(void) __asm__("slogin_onCloseModeError");

// (prototype from their definition at line 1917)
int slogin_onCardModeError(int *a, int *b);

// (prototype from their definition at line 1960)
void slogin_onCloseModeDone_7A60(int *a) __asm__("slogin_onCloseModeDone");

// (prototype from their definition at line 1967)
int slogin_reportKeptError(int a, int *b);

extern int slogin_runModeStep();
extern int slogin_enterNextCardMode();
extern int slogin_installCardFunctions(void);
/**
 * Enters a network-account card mode.
 *
 * Sets the mode (+0xD8), optionally saving the whole phase block to +0x10C, installs the step function slogin_runModeStep,
 * the next-mode handler (modes 0..5 slogin_enterNextCardMode, 6 and 8 slogin_onCloseModeDone, 7 slogin_reportKeptError) and the error handler (0..5
 * slogin_onCardModeError, 6 and 8 slogin_onCloseModeError, 7 slogin_reportKeptError), then the mode's card functions (slogin_installCardFunctions). Modes: 0
 * create the save, 1 open it, 2 / 3 write ACCT / PASS, 4 / 5 read them, 6 and 7 close, 8 delete.
 *
 * @param mode  the mode
 * @param copy  nonzero to save the phase block first
 * @return 0, or 13 for a mode of 9 or more
 */
int slogin_enterCardMode(int mode, unsigned char copy) {
    int err = 0;
    slogin_session->cur.mode = mode;
    if (copy)
        slogin_session->saved = slogin_session->cur;
    if (slogin_session->cur.mode < 9)
        slogin_session->cur.stepFn = (void *)slogin_runModeStep;
    else
        err = 13;
    if (err == 0) {
        switch (slogin_session->cur.mode) {
        case 0: case 1: case 2: case 3: case 4: case 5:
            slogin_session->cur.nextModeFn = (void *)slogin_enterNextCardMode;
            break;
        case 6: case 8:
            slogin_session->cur.nextModeFn = (void *)slogin_onCloseModeDone_7A60;
            break;
        case 7:
            slogin_session->cur.nextModeFn = (void *)slogin_reportKeptError;
            break;
        default:
            err = 13;
            break;
        }
    }
    if (err == 0) {
        switch (slogin_session->cur.mode) {
        case 0: case 1: case 2: case 3: case 4: case 5:
            slogin_session->cur.errorFn = (void *)slogin_onCardModeError;
            break;
        case 6: case 8:
            slogin_session->cur.errorFn = (void *)slogin_onCloseModeError_7A60;
            break;
        case 7:
            slogin_session->cur.errorFn = (void *)slogin_reportKeptError;
            break;
        }
    }
    return slogin_installCardFunctions();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_returnToIdle);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int slogin_finishPhase(void);
/**
 * Returns the network-account session to mode 10 (idle) with no step, next-mode or error handler, and finishes the
 * phase (slogin_finishPhase).
 *
 * @return 0
 */
int slogin_returnToIdle(void) {
    struct sNetAcct *session = slogin_session;
    session->cur.mode = 10;
    session->cur.stepFn = 0;
    session->cur.nextModeFn = 0;
    session->cur.errorFn = 0;
    return slogin_finishPhase();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_installCardFunctions);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

// (prototype from their definition at line 2049)
int slogin_startWritePassword(void);

// (prototype from their definition at line 1996)
int slogin_startOpenSave(void);

// (prototype from their definition at line 2114)
int slogin_startCloseSave(void);

// (prototype from their definition at line 2025)
int slogin_startWriteName(void);

// (prototype from their definition at line 2169)
int slogin_defaultPhase2_7C00(void) __asm__("slogin_defaultPhase2");

// (prototype from their definition at line 1982)
int slogin_startCreateSave(void);

// (prototype from their definition at line 2009)
int slogin_startDeleteSave(void);

// (prototype from their definition at line 2127)
int slogin_pollCardMode(int *out);

// (prototype from their definition at line 2095)
int slogin_startReadPassword(void);

// (prototype from their definition at line 2073)
int slogin_startReadName(void);

/**
 * Installs the card functions of the current network-account mode and resets its phase to 4.
 *
 * Start function (+0xF0): mode 0 slogin_startCreateSave, 1 slogin_startOpenSave, 2 slogin_startWriteName, 3 slogin_startWritePassword, 4 slogin_startReadName, 5
 * slogin_startReadPassword, 6 and 7 slogin_startCloseSave, 8 slogin_startDeleteSave. Poll (+0xF4) slogin_pollCardMode, phase-2 function (+0xF8)
 * slogin_defaultPhase2; completion and cleanup (+0xFC, +0x100) cleared. Also copies the phase word to the saved block.
 *
 * @return 0, or 13 for a mode of 9 or more
 */
int slogin_installCardFunctions(void) {
    int errorCode;
    unsigned i;
    i = errorCode = 0;
    slogin_session->cur.step.phase = 4;
    slogin_session->saved.step = slogin_session->cur.step;
    for (i = 0; i < 3; i++) ;
    switch (slogin_session->cur.mode) {
    case 0: slogin_session->cur.step.start = (void *)slogin_startCreateSave; break;
    case 1: slogin_session->cur.step.start = (void *)slogin_startOpenSave; break;
    case 2: slogin_session->cur.step.start = (void *)slogin_startWriteName; break;
    case 3: slogin_session->cur.step.start = (void *)slogin_startWritePassword; break;
    case 4: slogin_session->cur.step.start = (void *)slogin_startReadName; break;
    case 5: slogin_session->cur.step.start = (void *)slogin_startReadPassword; break;
    case 6:
    case 7: slogin_session->cur.step.start = (void *)slogin_startCloseSave; break;
    case 8: slogin_session->cur.step.start = (void *)slogin_startDeleteSave; break;
    default: errorCode = 13;
    }
    if (errorCode == 0) {
        if (slogin_session->cur.mode < 9) slogin_session->cur.step.poll = (void *)slogin_pollCardMode;
        else errorCode = 13;
        if (errorCode == 0) {
            if (slogin_session->cur.mode < 9) slogin_session->cur.step.finish = (void *)slogin_defaultPhase2_7C00;
            else errorCode = 13;
        }
    }
    slogin_session->cur.step.completion = 0;
    slogin_session->cur.step.cleanup = 0;
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_finishPhase);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/**
 * Finishes the current network-account mode's phase: phase 5, with the start, poll and phase-2 functions, the
 * completion and the cleanup cleared.
 *
 * @return 0
 */
int slogin_finishPhase(void) {
    unsigned int i;
    int *handlerSlot;
    struct sNetAcct *session = slogin_session;
    struct sNetAcct *sessionReload;
    session->cur.step.phase = 5;
    handlerSlot = (int *)&session->cur.step.start;
    for (i = 0; i < 3; i++) {
        *handlerSlot++ = 0;
    }
    sessionReload = slogin_session;
    sessionReload->cur.step.completion = 0;
    sessionReload->cur.step.cleanup = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_checkIconCrc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* The output block a digest interface's finish slot hands back (the CRC one: crcIface_finish, +0x4 of crcIface_state). */
typedef struct { int size; unsigned *digest; } sDigestBlock;
/* The CRC interface's reset / update / finish slots (as sMcIfB, names ours) */
typedef struct { char pad[0xC]; int (*reset)(void); int (*update)(int,
        int); int (*finish)(sDigestBlock **); } sDigestIface;
extern sDigestIface *fB5E8(void) __asm__("crcIface_get");
/**
 * Checks the embedded save icon: its 10606 bytes must have the CRC-32 0x0836C71D.
 *
 * Runs the CRC interface from crcIface_get (reset, update over @p iconData, final) and compares the digest.
 *
 * @param iconData  the icon data (view.ico)
 * @return 0 when it matches, 21 when the CRC differs, 0x18 when the CRC interface fails
 */
int slogin_checkIconCrc(int iconData) {
    int err = 0;
    sDigestIface *crcIface = fB5E8();
    int crcStatus;
    sDigestBlock *w;
    crcStatus = crcIface->reset();
    if (crcStatus == 0) {
        crcStatus = crcIface->update(iconData, 10606);
        if (crcStatus == 0) {
            crcStatus = crcIface->finish(&w);
            if (crcStatus == 0) {
                err = 21;
                if (*w->digest == 0x836C71D) err = 0;
            }
        }
    }
    if (crcStatus != 0) return 0x18;
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_copyStringBounded);
#ifdef SKIP_ASM
/**
 * Copies a C string into a buffer of @p n bytes, always NUL-terminating it (strncpy-like but bounded by @p n - 1).
 *
 * Does nothing when either pointer is 0, @p n is 0 or the two are the same.
 *
 * @param dst  destination
 * @param src  source
 * @param n  destination size
 * @return @p dst
 */
// From peak3project's fork (github.com/peak3project/ssx3).
char *slogin_copyStringBounded(char *dst, char *src, int n) {
    char *dstCursor;
    unsigned char *srcCursor = (unsigned char*)src;
    if (srcCursor && dst && n && srcCursor != (unsigned char*)dst) {
        dstCursor = dst;
        do {
            if (--n == 0) { *dstCursor = 0; break; }
        } while ((*dstCursor++ = *srcCursor++) != 0);
    }
    return dst;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", slogin_init);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_setStarted(void);
extern int slogin_createSession(struct sNetInit *);
extern int slogin_openSaveRecords(void);
/**
 * Starts the network-account library: sets its started flag (slogin_setStarted), creates the session (slogin_createSession) and
 * opens the save-record manager (slogin_openSaveRecords).
 *
 * The shared-login manager (cSharedLoginMan_init) passes {its 17 memory-card callbacks, 0}.
 *
 * @param initParams  {callback table, heap}
 */
void slogin_init(struct sNetInit *initParams) {
    slogin_setStarted();
    slogin_createSession(initParams);
    slogin_openSaveRecords();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", slogin_shutdown);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_closeSaveRecords(void);
extern int slogin_freeSession(void);
extern int slogin_clearStarted(void);
/**
 * Stops the network-account library: closes the save-record manager (slogin_closeSaveRecords), frees the session (slogin_freeSession)
 * and clears the started flag (slogin_clearStarted). The shared-login manager (cSharedLoginMan_shutdown) calls it.
 *
 * @return 0
 */
int slogin_shutdown(void) {
    slogin_closeSaveRecords();
    slogin_freeSession();
    return slogin_clearStarted();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startCreateAccount);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

// (prototype from their definition at line 1882)
int slogin_startOperation_7F48(int a) __asm__("slogin_startOperation");

extern char* f7E90(char *, int, int) __asm__("slogin_copyStringBounded");
extern int slogin_checkIconCrc_7F48(int) __asm__("slogin_checkIconCrc");
extern int f8258(int) __asm__("slogin_buildSaveDescription");
/**
 * Starts creating the network-account save (operation 0) with a user name and password.
 *
 * Copies the name (17 bytes) and password (17 bytes at +0x11) from @p accountBuf into the session, checks the icon data
 * (slogin_checkIconCrc), builds the save description (slogin_buildSaveDescription) and starts the operation (slogin_startOperation(0)). Likely bug:
 * it refuses (7) only when both arguments are 0, so a 0 @p accountBuf is copied from.
 *
 * @param iconData  the save icon data (view.ico, 10606 bytes)
 * @param accountBuf  the 34-byte account buffer: name, password at +0x11
 * @return 0 or an error code
 */
int slogin_startCreateAccount(int iconData, int accountBuf) {
    int err = 0;
    if (iconData == 0 && accountBuf == 0) {
        err = 7;
    } else {
        f7E90(slogin_session->name, accountBuf, 17);
        f7E90(slogin_session->pass, accountBuf + 17, 17);
    }
    if (err == 0) {
        err = slogin_checkIconCrc_7F48(iconData);
        if (err == 0) {
            err = f8258(iconData);
            if (err == 0) err = slogin_startOperation_7F48(0);
        }
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startReadAccount);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int slogin_startOperation(int);
/**
 * Starts loading the network-account save (operation 2) into a 34-byte buffer: name at +0, password at +0x11.
 *
 * @param accountBuf  the output buffer (kept at +0xCC)
 * @return 0, 7 when @p accountBuf is 0, or 22 when busy
 */
int slogin_startReadAccount(struct sAcctOut *accountBuf) {
    int errorCode = 0;
    if (accountBuf == 0) {
        errorCode = 7;
    } else {
        slogin_session->out = accountBuf;
    }
    if (errorCode == 0) {
        errorCode = slogin_startOperation(2);
    }
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_startSaveAccount);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern char *slogin_copyStringBounded_8048(char *, char *, int) __asm__("slogin_copyStringBounded");
extern int slogin_startOperation(int);
/**
 * Starts saving a user name and password to the network-account save (operation 1).
 *
 * Copies the name and the password (at +0x11) from the buffer into the session first.
 *
 * @param name  the 34-byte account buffer
 * @return 0, 7 when @p name is 0, or 22 when busy
 */
int slogin_startSaveAccount(char *name) {
    int errorCode = 0;
    if (name == 0) {
        errorCode = 7;
    } else {
        slogin_copyStringBounded_8048(slogin_session->name, name, 17);
        slogin_copyStringBounded_8048(slogin_session->pass, name + 0x11, 17);
    }
    if (errorCode == 0) errorCode = slogin_startOperation(1);
    return errorCode;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", slogin_startDeleteAccount);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int slogin_startOperation(int);
/**
 * Starts deleting the network-account save (operation 3).
 *
 * @return 0, or 22 when busy
 */
int slogin_startDeleteAccount(void) { return slogin_startOperation(3); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

extern int slogin_endOperation(void);
/**
 * Polls the running network-account operation.
 *
 * Unless idle, runs the operation poll (+0xD4, slogin_advanceOperation); when it finishes (status 2) or fails, the session goes
 * back to idle (slogin_endOperation). The shared-login manager (cSharedLoginMan_update) calls it every frame.
 *
 * @param out  receives 0 when idle, 1 while running, 2 when finished
 * @return 0 or an error code (7 when @p out is 0)
 */
int slogin_update(int *out) {
    int err = 0;
    int status;
    if (out == 0) {
        err = 7;
    } else {
        *out = 0;
        if (slogin_session->st != 4) {
            err = ((int (*)(int *))slogin_session->operationPoll)(out);
            if (err == 0) {
                status = *out;
                if (status == 2) {
                    slogin_endOperation();
                    *out = status;
                }
            } else {
                slogin_endOperation();
            }
        }
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_mapSaveRecordError);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int slogin_mapSaveRecordError(int a);
/**
 * Maps a save-record manager error (card-layer errors are offset by 100) to a network-account error code.
 *
 * 0 to 0; 104 to 1; 103 to 2; 114 to 3; 108 to 4; 3, 7 and 110 to 5; 9 to 6; 1 to 19; 2 to 20; 4 to 7; 6 and 107 to 11;
 * anything else to 0x18.
 *
 * @param sysError  the manager's error
 * @return the account error
 */
int slogin_mapSaveRecordError(int sysError) {
    switch (sysError) {
    case 0: return 0;
    case 104: return 1;
    case 103: return 2;
    case 114: return 3;
    case 108: return 4;
    case 3:
    case 7:
    case 110: return 5;
    case 9: return 6;
    case 1: return 19;
    case 2: return 20;
    case 4: return 7;
    case 6:
    case 107: return 11;
    }
    return 0x18;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_getLastOperation);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/**
 * Stores the last network-account operation started (+0x104) in *@p operationOut.
 *
 * @param operationOut  output
 * @return 0
 */
int slogin_getLastOperation(int *operationOut) { *operationOut = slogin_session->lastOperation; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_getSavedMode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* slogin_session: the network account session (0x14C bytes, slogin_createSession). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *slogin_session;

/**
 * Stores the mode kept in the saved phase block (+0x10C) in *@p modeOut.
 *
 * @param modeOut  output
 * @return 0
 */
int slogin_getSavedMode(int *modeOut) { *modeOut = slogin_session->saved.mode; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_getAccountRecordSize);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int saveRec_getStoredSize(int, int);
/**
 * Stores the on-card size of a 34-byte account record (saveRec_getStoredSize) through @p sizeOut.
 *
 * @param sizeOut  output pointer (declared int)
 * @return 0
 */
int slogin_getAccountRecordSize(int sizeOut) { saveRec_getStoredSize(34, sizeOut); return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", slogin_buildSaveDescription);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* The memory-card save's icon.sys (Sony libmc sceMcIconSys, 0x3C4 bytes). */
typedef int iconIVECTOR[4];
typedef float iconFVECTOR[4];
typedef struct {
    unsigned char Head[4];
    unsigned short Reserv1;
    unsigned short OffsLF;
    unsigned Reserv2;
    unsigned TransRate;
    iconIVECTOR BgColor[4];
    iconFVECTOR LightDir[3];
    iconFVECTOR LightColor[3];
    iconFVECTOR Ambient;
    unsigned short TitleName[34];
    unsigned char FnameView[64];
    unsigned char FnameCopy[64];
    unsigned char FnameDel[64];
    unsigned char Reserve3[512];
} sceMcIconSys;
typedef struct {
    void *port;                         /* +0x000: the icon data (slogin_buildSaveDescription's parameter) */
    int id;                             /* +0x004: the icon data size (10606) */
    sceMcIconSys icon;                  /* +0x008 */
    int recordSize;                     /* +0x3CC: [U] slogin_buildSaveDescription stores 34, mcHook_computeNewSaveSize reads it */
} sMcSaveParam;

extern void *f8258_memset(void *, int, int) __asm__("memset");
/* Constant data this function copies from (string literals and local array initialisers). */
typedef struct { iconFVECTOR v[3]; } sIconLight3;
typedef struct { iconFVECTOR v; } sIconLight1;
extern sIconLight3 D_00497320; /* light directions */
extern sIconLight3 D_00497350; /* light colours */
extern sIconLight1 D_00497380; /* ambient */
extern char D_004A4BD0[] __attribute__((aligned(8))); /* "PS2D" (.sdata) */
extern char str_EA_Login[];         /* "EA Login" */
extern char str_view_ico[];         /* "view.ico" */
extern void mcReq_asciiToSjis(char *src, unsigned short *dst);
extern int saveRec_setSaveDescription(sMcSaveParam *);

/**
 * Builds the network-account save's description and hands it to the save-record manager (saveRec_setSaveDescription).
 *
 * Fills the icon.sys header: magic "PS2D", title "EA Login" converted to Shift-JIS (mcReq_asciiToSjis), line break at 16,
 * transparency 0x60, two blue background corners, light directions, colours and ambient from D_00497320 / D_00497350 /
 * D_00497380, and "view.ico" as the list, copy and delete icon. Then stores the icon data pointer and size 10606 and
 * the record size 34.
 *
 * @param port  the icon data (not a port; named after the field it fills)
 * @return 0, 7 when @p port is 0, or the error mapped by slogin_mapSaveRecordError
 */
int slogin_buildSaveDescription(void *port) {
    sMcSaveParam saveDesc;
    iconIVECTOR bgcolor[4];
    sIconLight3 lightdir;
    sIconLight3 lightcol;
    sIconLight1 ambient;
    int err = 0;

    f8258_memset(bgcolor, 0, sizeof(bgcolor));
    bgcolor[0][2] = 0x80;
    bgcolor[1][2] = 0x80;
    lightdir = D_00497320;
    lightcol = D_00497350;
    ambient = D_00497380;

    if (port == 0) err = 7;
    if (err == 0) {
        f8258_memset(&saveDesc.icon, 0, sizeof(saveDesc.icon));
        __builtin_memcpy(saveDesc.icon.Head, D_004A4BD0, 5);
        mcReq_asciiToSjis(str_EA_Login, saveDesc.icon.TitleName);
        saveDesc.icon.OffsLF = 16;
        saveDesc.icon.TransRate = 0x60;
        __builtin_memcpy(saveDesc.icon.BgColor, bgcolor, sizeof(bgcolor));
        __builtin_memcpy(saveDesc.icon.LightDir, &lightdir, sizeof(lightdir));
        __builtin_memcpy(saveDesc.icon.LightColor, &lightcol, sizeof(lightcol));
        __builtin_memcpy(saveDesc.icon.Ambient, &ambient, sizeof(ambient));
        __builtin_memcpy(saveDesc.icon.FnameView, str_view_ico, 9);
        __builtin_memcpy(saveDesc.icon.FnameCopy, str_view_ico, 9);
        __builtin_memcpy(saveDesc.icon.FnameDel, str_view_ico, 9);
        saveDesc.port = port;
        saveDesc.id = 10606;
        saveDesc.recordSize = 34;
        err = slogin_mapSaveRecordError(saveRec_setSaveDescription(&saveDesc));
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_findLowestSlot);
#ifdef SKIP_ASM
/**
 * Returns the index of the lowest of the low 8 bits set in @p mask (8 when none is).
 *
 * @param mask  slot bit mask
 * @return the slot index
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcReq_findLowestSlot(unsigned mask) {
    int i = 0;
    mask &= 0xFFFF;
    if (!(mask & 1)) {
        do {
            i++;
        } while (i < 8 && !(mask & (1 << i)));
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_findHighestSlot);
#ifdef SKIP_ASM
/**
 * Returns the index of the highest of the low 8 bits set in @p mask (-1 when none is).
 *
 * @param mask  slot bit mask
 * @return the slot index
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcReq_findHighestSlot(unsigned mask) {
    int i = 7;
    mask &= 0xFFFF;
    if (!(mask & 0x80)) {
        do {
            i--;
        } while (i >= 0 && !(mask & (1 << i)));
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_findNextSlot);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
struct sMcReqBody { int port; int slot; char dir[32]; char file[32]; char name[0x18]; };

struct sMcReq {
    int op;                             /* 0x00: request kind, 0 = none */
    int st;                             /* 0x04: state */
    int error;                          /* 0x08: (name ours) [U] mcReq_setError records the error, mcReq_getError
                                           returns it */
    struct sMcReqBody body;             /* 0x0C */
    int searchDir;                      /* 0x6C: (name ours) [U] the start functions store their searchDir;
                                           mcReq_findNextSlot takes it to pick the next unit */
    int expected;                       /* 0x70: (name ours) [U] the seek target / byte count a step compares with
                                           the card call's result */
};

extern struct sMcReq *mcReq_current;

/**
 * Returns the next usable memory-card slot after the request's current one (+0x10 of mcReq_current).
 *
 * @param slotMask  usable-slot bit mask
 * @param dir  0 to search upward, 1 downward
 * @return the slot, -1 when there is none, or 18 for another @p dir
 */
int mcReq_findNextSlot(unsigned short slotMask, int dir) {
    short i = mcReq_current->body.slot;
    int nextSlot;
    if (dir == 0) {
        i++;
        while (i < 8 && !((slotMask >> i) & 1)) i++;
        if (i >= 8) nextSlot = -1; else nextSlot = i;
    } else if (dir == 1) {
        i--;
        while (i >= 0 && !((slotMask >> i) & 1)) i--;
        if (i < 0) nextSlot = -1; else nextSlot = i;
    } else {
        return 18;
    }
    return nextSlot;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_countUnits);
#ifdef SKIP_ASM
/**
 * Counts the bits set among the low 8 of a usable-unit mask (the number of memory-card units the request layer may
 * use).
 *
 * @param mask  unit bit mask
 * @return the number of units
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcReq_countUnits(unsigned short mask) {
    int count = 0; int i = 0;
    for (i = 0; i < 8; i++) { int bit = 1 << i; if (mask & bit) count++; }
    return count;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_setError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

void mcReq_setError_impl(int a) __asm__("mcReq_setError");
/**
 * Records the memory-card request's error (mcReq_current +0x8), unless the request is already one of the cleanup
 * operations 11..13, which keep the error that caused them.
 *
 * @param errorCode  the error (0 clears it)
 */
void mcReq_setError_impl(int errorCode) {
    struct sMcReq *request = mcReq_current;
    if ((unsigned)(request->op - 11) >= 3) request->error = errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_getError);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Returns the error recorded for the memory-card request (mcReq_current +0x8, set by mcReq_setError).
 *
 * @return the recorded error
 */
int mcReq_getError(void) { return mcReq_current->error; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startFlushCleanup);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. The callback
   names are ours, from [U] the step handlers (mcReq_stepCreate create, mcReq_stepDelete delete, the cleanups mcReq_startFlushCleanup /
   mcReq_startCloseCleanup / mcReq_startReleaseCleanup, the read / write / seek / close starters and the poll mcReq_update). */
struct sMcDrv {
    unsigned short ports;               /* 0x00: bit per usable slot */
    short pad02;
    void (*getDir)(char *, int);        /* 0x04: [U] (search pattern, slot): the save search */
    void (*queryFreeSpace)(int);        /* 0x08: [U] (slot); its result must reach saveSize */
    void (*queryFreeEntries)(int);      /* 0x0C: [U] (slot); its result must reach entriesNeeded */
    void (*checkUnit)(int);             /* 0x10: [U] (slot): every request starts with it */
    void (*releaseUnit)(int);           /* 0x14: [U] (slot): every request ends with it */
    void (*open)(char *, char *, int, int); /* 0x18: [U] (directory, file name, slot, mode) */
    void (*close)(int);                 /* 0x1C: [U] (descriptor), after flush */
    void (*makeDir)(char *, int, int);  /* 0x20: [U] (directory, size, slot) */
    void (*deleteFile)(char *, char *, int); /* 0x24: [U] (directory, file name, slot) */
    void (*deleteDir)(char *, int);     /* 0x28: [U] (directory, slot) */
    void (*read)(int, int, int);        /* 0x2C: [U] (descriptor, buffer, size) */
    void (*write)(int, void *, int);    /* 0x30: [U] (descriptor, data, size) */
    void (*seek)(int, int, int);        /* 0x34: [U] (descriptor, offset, origin) */
    void (*flush)(int);                 /* 0x38: [U] (descriptor), before close */
    int reserved;                       /* 0x3C: [U] must be set (mcReq_open); never called */
    int (*sync)(int *, int *);          /* 0x40: [U] the poll: (status, result) of the running card call */
    void (*getFoundName)(char *);       /* 0x44: [U] (directory): the name the search found */
    int (**opHandlers)(int, int *, int *); /* 0x48: by op */
    int (**stateHandlers)(int, int, int); /* 0x4C: by state */
    sMcSaveParam saveDesc;              /* 0x50: [U] mcReq_setSaveDescription copies the 976-byte save description here */
    int saveSize;                       /* 0x420: (name ours) [U] bytes a new save needs (mcHook_computeNewSaveSize) */
    int entriesNeeded;                  /* 0x424: (name ours) [U] 3 (mcHook_computeNewSaveSize): the save's three files */
    int h;                              /* 0x428: heap */
};

extern struct sMcDrv *mcReq_driver;

/**
 * Starts the cleanup of a failed memory-card request that has a file open: operation 11, state 24, flushing the file
 * (driver callback +0x38 with the descriptor in the request body).
 *
 * mcReq_stepCleanup then closes the file and releases the unit, and the request ends with the recorded error.
 *
 * @param reqBody  ignored (callers pass the request body)
 * @param out  receives 1 (still running)
 * @return 0
 */
int mcReq_startFlushCleanup(struct sMcReqBody *reqBody, int *out) {
    struct sMcReq *request;
    struct sMcDrv *driver;
    *out = 1;
    request = mcReq_current;
    driver = mcReq_driver;
    request->op = 11;
    request->st = 0x18;
    driver->flush(request->body.port);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startCloseCleanup);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

/**
 * Starts the cleanup of a failed memory-card request at the close step: operation 12, state 25, closing the file
 * (driver callback +0x1C with the descriptor in the request body).
 *
 * @param reqBody  ignored (callers pass the request body)
 * @param out  receives 1 (still running)
 * @return 0
 */
int mcReq_startCloseCleanup(struct sMcReqBody *reqBody, int *out) {
    struct sMcReq *request;
    struct sMcDrv *driver;
    *out = 1;
    request = mcReq_current;
    driver = mcReq_driver;
    request->op = 0xC;
    request->st = 25;
    driver->close(request->body.port);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startReleaseCleanup);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

/**
 * Starts the cleanup of a failed memory-card request at the last step: operation 13, state 26, releasing the card unit
 * (driver callback +0x14).
 *
 * @param unitCode  the card unit code
 * @param out  receives 1 (still running)
 * @return 0
 */
int mcReq_startReleaseCleanup(int unitCode, int *out) {
    struct sMcReq *request;
    struct sMcDrv *driver;
    *out = 1;
    request = mcReq_current;
    driver = mcReq_driver;
    request->op = 13;
    request->st = 26;
    driver->releaseUnit(unitCode);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_acceptAnyResult);
#ifdef SKIP_ASM
/**
 * Result check of memory-card request state 0 (entry 0 of the per-state table D_004514A0): accepts any result.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcReq_acceptAnyResult(int cardResult) { return cardResult; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkUnitResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 1 (unit check): accepts results 0, 3 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 when the request is not in state 1, or 0x10 for any other result
 */
int mcReq_checkUnitResult(int cardResult) {
    if (mcReq_current->st != 1) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkSearchResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 2 (directory search): accepts results 0, 3, 4, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 when the request is not in state 2, or 0x10 for any other result
 */
int mcReq_checkSearchResult(int cardResult) {
    if (mcReq_current->st != 2) return 18;
    if (cardResult != 0 && cardResult != 4 && cardResult != 3 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkTransferResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of the memory-card read, write and seek states (27, 28, 29 and the create steps 7 and 11): accepts
 * results 0, 3, 4, 9, 10, 13, 14 and 17.
 *
 * Quirk: the per-state table also maps state 15 here, which this check rejects (state 15 is never entered), and it
 * accepts state 24, whose table entry is mcReq_checkFlushResult.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkTransferResult(int cardResult) {
    int state = mcReq_current->st;
    if (state != 27 && state != 29 && state != 0x1C && state != 7 && state != 11 && state != 0x18) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 0xE && cardResult != 13 && cardResult != 17 && cardResult != 10) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkFlushResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of the memory-card flush states (8, 12, 16 and 24): accepts results 0, 3, 4, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkFlushResult(int cardResult) {
    int state = mcReq_current->st;
    if (state != 0x18 && state != 8 && state != 0xC && state != 0x10) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkCloseResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of the memory-card close states (9, 13, 17 and 25): accepts results 0, 3, 4, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkCloseResult(int cardResult) {
    int state = mcReq_current->st;
    if (state != 25 && state != 9 && state != 13 && state != 17) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkReleaseResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 26 (unit released): accepts results 0, 3, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkReleaseResult(int cardResult) {
    if (mcReq_current->st != 26) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkMkdirResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 5 (directory created): accepts results 0, 3, 8, 9, 14 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkMkdirResult(int cardResult) {
    if (mcReq_current->st != 5) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 9 && cardResult != 0xE && cardResult != 8 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkOpenResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of the memory-card file-open states (6, 10, 14 and 22): accepts results 0, 3, 4, 9, 13 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkOpenResult(int cardResult) {
    int state = mcReq_current->st;
    if (state != 22 && state != 6 && state != 0xE && state != 10) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 13 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkDeleteResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of the memory-card file-delete states (19..21): accepts results 0, 3, 4, 5, 6, 9, 13 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkDeleteResult(int cardResult) {
    if ((unsigned)(mcReq_current->st - 19) >= 3) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 13 && cardResult != 6 && cardResult != 5 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkRmdirResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 18 (directory deleted): accepts results 0, 3, 4, 9, 13 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkRmdirResult(int cardResult) {
    if (mcReq_current->st != 18) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 13 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkFreeSpaceResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 3 (free space read): accepts results 0, 3 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkFreeSpaceResult(int cardResult) {
    int state = mcReq_current->st;
    if (state != 3) return 18;
    if (cardResult != 0 && cardResult != state && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkSpaceQueryResult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 4 (second space query): accepts results 0, 3 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkSpaceQueryResult(int cardResult) {
    if (mcReq_current->st != 4) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_checkState23Result);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Result check of memory-card request state 23: accepts results 0, 3, 4, 9 and 17. No request step enters state 23.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int mcReq_checkState23Result(int cardResult) {
    if (mcReq_current->st != 23) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_stepNoOperation);
#ifdef SKIP_ASM
/**
 * Step handler of memory-card request operation 0 (entry 0 of the per-operation table D_00451468): passes the result
 * on.
 *
 * @param cardResult  the checked result
 * @return @p cardResult
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcReq_stepNoOperation(int cardResult) { return cardResult; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_stepOpen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;
/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;
extern void f86F0(int) __asm__("mcReq_setError");
extern int mcReq_findNextSlot(unsigned short v, int dir);
extern int mcReq_getError(void);
extern int mcReq_startReleaseCleanup(int a, int *out);
extern char *mcReq_buildSearchPattern(char *dst, sMcSaveParam *unused, char *src);
extern void fA818(char *, char *, sMcSaveParam *, char *) __asm__("mcReq_buildSaveNames");
/**
 * Step handler of the memory-card open operations (3: search the usable units, 6: one unit).
 *
 * Unit checked (state 1): searches the directory for "*" + the save name (state 2); with no card (3) operation 3 tries
 * the next usable unit in the search direction. Search done: when found, builds the directory and file names
 * ("PSCD10088" + name) and opens dir/file with mode 3 (state 22); when not found records error 4 and releases the unit
 * (state 26). Open done: keeps the descriptor in the request body and finishes (10 when it is negative). Unit released:
 * finishes with the recorded error, or for operation 3 with an error tries the next unit. Failures go to the release
 * cleanup (mcReq_startReleaseCleanup).
 *
 * @param callResult  checked result of the finished card call
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (search: 1 found / 0 not; open: the descriptor)
 * @return 0 or an error code
 */
int mcReq_stepOpen(int callResult, int *out, int *pollResult) {
    int op;
    int slot;
    char buf[80];
    if (out == 0) return 18;
    if (pollResult == 0) return 18;
    op = mcReq_current->op;
    if (op != 3 && op != 6) return 18;
    *out = 1;
    switch ((unsigned)mcReq_current->st) {
    case 1:
        if (callResult == 0) {
            mcReq_buildSearchPattern(buf, &mcReq_driver->saveDesc, mcReq_current->body.name);
            mcReq_current->st = 2;
            mcReq_driver->getDir(buf, mcReq_current->body.slot);
            break;
        }
        if (callResult != 3) {
            *out = 2;
            return callResult;
        }
        if (mcReq_current->op == 3) {
            slot = mcReq_findNextSlot(mcReq_driver->ports, mcReq_current->searchDir);
            mcReq_current->body.slot = slot;
            if (slot != -1) {
                mcReq_current->st = 1;
                mcReq_driver->checkUnit(slot);
                return 0;
            }
        }
        *out = 2;
        return callResult;
    case 2:
        if (callResult == 0) {
            if (*pollResult == 1) {
                fA818(mcReq_current->body.dir, mcReq_current->body.file, &mcReq_driver->saveDesc, mcReq_current->body.name);
                mcReq_current->st = 22;
                mcReq_driver->open(mcReq_current->body.dir, mcReq_current->body.file, mcReq_current->body.slot, 3);
                break;
            }
            if (*pollResult == 0) {
                f86F0(4);
                mcReq_current->st = 26;
                mcReq_driver->releaseUnit(mcReq_current->body.slot);
                break;
            }
            *out = 2;
            return 10;
        }
        f86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, out);
    case 22:
        if (callResult == 0) {
            if (*pollResult >= 0) {
                mcReq_current->body.port = *pollResult;
                *out = 2;
                return 0;
            }
            mcReq_current->body.port = *pollResult;
            *out = 2;
            return 10;
        }
        f86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, out);
    case 26:
        if (callResult == 0) {
            if (mcReq_current->op == 4 || mcReq_current->op == 6) {
                *out = 2;
                return mcReq_getError();
            }
            if (mcReq_getError() == 0) {
                *out = 2;
                break;
            }
            slot = mcReq_findNextSlot(mcReq_driver->ports, mcReq_current->searchDir);
            mcReq_current->body.slot = slot;
            if (slot != -1) {
                mcReq_current->st = 1;
                mcReq_driver->checkUnit(slot);
                return 0;
            }
            *out = 2;
            return mcReq_getError();
        }
        *out = 2;
        return callResult;
    default:
        *out = 2;
        return 18;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_stepTransfer);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

extern void f86F0(int) __asm__("mcReq_setError");
/**
 * Step handler of the memory-card seek, write and read operations (8, 9, 10).
 *
 * Seek (state 27) finishes on success; write and read (states 28, 29) finish with 0 when the byte count equals the
 * request's expected count (+0x70), else 10. A failed call records the error and starts the flush cleanup
 * (mcReq_startFlushCleanup).
 *
 * @param callResult  checked result of the finished card call
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (bytes moved)
 * @return 0 or an error code
 */
int mcReq_stepTransfer(int callResult, int *out, int *pollResult) {
    int op;
    if (out == 0) return 18;
    if (pollResult == 0) return 18;
    op = mcReq_current->op;
    if (op != 8 && op != 10 && op != 9) return 18;
    *out = 1;
    switch ((unsigned)mcReq_current->st) {
    case 27:
        if (callResult == 0) {
            *out = 2;
            return 0;
        }
        f86F0(callResult);
        return mcReq_startFlushCleanup(&mcReq_current->body, out);
    case 0x1C:
    case 29:
        if (callResult == 0) {
            if (mcReq_current->expected == *pollResult) {
                *out = 2;
                return 0;
            }
            *out = 2;
            return 10;
        }
        f86F0(callResult);
        return mcReq_startFlushCleanup(&mcReq_current->body, out);
    }
    *out = 2;
    return 18;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_stepClose);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;
/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern void f86F0(int) __asm__("mcReq_setError");
/**
 * Step handler of the memory-card close operation (7): flush done (state 24) closes the file (25), close done releases
 * the unit (26), release done finishes. A failed step records the error and continues with the matching cleanup
 * (mcReq_startCloseCleanup, mcReq_startReleaseCleanup).
 *
 * @param callResult  checked result of the finished card call
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (only checked for 0)
 * @return 0 or an error code
 */
int mcReq_stepClose(int callResult, int *out, int pollResult) {
    if (out == 0) return 18;
    if (pollResult == 0) return 18;
    if (mcReq_current->op != 7) return 18;
    *out = 1;
    switch ((unsigned)mcReq_current->st) {
    case 0x18:
        if (callResult == 0) {
            mcReq_current->st = 25;
            mcReq_driver->close(mcReq_current->body.port);
            break;
        }
        f86F0(callResult);
        return mcReq_startCloseCleanup(&mcReq_current->body, out);
    case 25:
        if (callResult == 0) {
            mcReq_current->st = 26;
            mcReq_driver->releaseUnit(mcReq_current->body.slot);
            break;
        }
        f86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, out);
    case 26:
        if (callResult == 0) {
            *out = 2;
            return 0;
        }
        *out = 2;
        return callResult;
    default:
        *out = 2;
        return 18;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_stepCleanup);
#ifdef SKIP_ASM
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_getError(void);
/**
 * Step handler of the memory-card cleanup operations (11..13): flush (state 24), then close (25), then release the unit
 * (26), and finish with the recorded error (mcReq_getError), whatever each step returned.
 *
 * @param callResult  checked result of the finished card call (ignored)
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (only checked for 0)
 * @return 0 while running, the recorded error when finished, or 18 for a bad state
 */
int mcReq_stepCleanup(int callResult, int *out, int pollResult) {
    if (out == 0 || pollResult == 0 || (unsigned)(mcReq_current->op - 11) > 2) return 18;
    *out = 1;
    switch ((unsigned)mcReq_current->st) {
    case 0x18:
        mcReq_current->st = 25;
        mcReq_driver->close(mcReq_current->body.port);
        break;
    case 25:
        mcReq_current->st = 26;
        mcReq_driver->releaseUnit(mcReq_current->body.slot);
        break;
    case 26:
        *out = 2;
        return mcReq_getError();
    default:
        *out = 2;
        return 18;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_isOpen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

/**
 * Returns whether the memory-card request layer is open (its driver block mcReq_driver exists).
 *
 * @return 1 when open, else 0
 */
int mcReq_isOpen(void) { return mcReq_driver != 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_open);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

/* The caller's callback table (17 entries, copied slot by slot to sMcDrv 0x04..0x44, so each slot has the type and the
   name of the sMcDrv slot it fills); every entry is required. */
struct sMcCallbacks {
    void (*getDir)(char *, int);
    void (*queryFreeSpace)(int);
    void (*queryFreeEntries)(int);
    void (*checkUnit)(int);
    void (*releaseUnit)(int);
    void (*open)(char *, char *, int, int);
    void (*close)(int);
    void (*makeDir)(char *, int, int);
    void (*deleteFile)(char *, char *, int);
    void (*deleteDir)(char *, int);
    void (*read)(int, int, int);
    void (*write)(int, void *, int);
    void (*seek)(int, int, int);
    void (*flush)(int);
    int reserved;
    int (*sync)(int *, int *);
    void (*getFoundName)(char *);
};

extern int mcReq_isOpen(void);
extern void *TibExtMemAlloc(int, int, int);
extern int (*D_00451468[])(int, int *, int *);  /* per-op handler table */
extern int (*D_004514A0[])(int, int, int);      /* per-state handler table */

/* The unit's later caller declares this (int, int, int). */
int mcReq_open_impl(int *ports, struct sMcCallbacks *cb, int heap) __asm__("mcReq_open");
/**
 * Opens the memory-card request layer.
 *
 * Checks the arguments (every one of the 17 callbacks must be set), allocates the 1068-byte driver block mcReq_driver and
 * the 116-byte request mcReq_current from @p heap, builds the usable-unit mask from @p ports (a -1-terminated list of unit
 * codes 0..7, or 9 for all eight; at most 8 entries), copies the callbacks to driver +0x4..+0x44 and installs the
 * per-operation and per-state tables D_00451468 / D_004514A0. Quirk: on the second allocation failure the driver block
 * stays allocated and set.
 *
 * @param ports  the unit-code list
 * @param cb  the 17 memory-card callbacks
 * @param heap  heap for the blocks
 * @return 0; 1 when already open, 0xC for bad arguments, 9 for a bad unit code, 7 when an allocation fails
 */
int mcReq_open_impl(int *ports, struct sMcCallbacks *cb, int heap) {
    int *unitEntry;
    unsigned char n = 0;
    char i;

    if (mcReq_isOpen() != 0) return 1;
    if (ports == 0) return 0xC;
    if (*ports == -1) return 0xC;
    if (cb == 0) return 0xC;
    if (cb->getDir == 0 || cb->queryFreeSpace == 0 || cb->queryFreeEntries == 0 || cb->checkUnit == 0 ||
        cb->releaseUnit == 0 || cb->open == 0 || cb->close == 0 || cb->makeDir == 0 || cb->deleteFile == 0 ||
        cb->deleteDir == 0 || cb->read == 0 || cb->write == 0 || cb->seek == 0 || cb->flush == 0 || cb->reserved == 0 ||
        cb->sync == 0 || cb->getFoundName == 0)
        return 0xC;
    if (mcReq_driver != 0 || mcReq_current != 0) return 1;
    mcReq_driver = TibExtMemAlloc(heap, 1068, 4);
    if (mcReq_driver == 0) return 7;
    mcReq_current = TibExtMemAlloc(heap, 116, 4);
    if (mcReq_current == 0) return 7;
    mcReq_driver->h = heap;
    mcReq_driver->ports = 0;
    for (unitEntry = ports; *unitEntry != -1; unitEntry++, n++) {
        if ((unsigned)*unitEntry >= 8 && *unitEntry != 9) return 9;
        if (n == 8) return 0xC;
        if (*unitEntry == 9) {
            for (i = 0; i < 8; i++) mcReq_driver->ports |= 1 << i;
        } else {
            mcReq_driver->ports |= 1 << (unsigned short)*unitEntry;
        }
    }
    mcReq_driver->getDir = cb->getDir;
    mcReq_driver->queryFreeSpace = cb->queryFreeSpace;
    mcReq_driver->queryFreeEntries = cb->queryFreeEntries;
    mcReq_driver->checkUnit = cb->checkUnit;
    mcReq_driver->releaseUnit = cb->releaseUnit;
    mcReq_driver->open = cb->open;
    mcReq_driver->close = cb->close;
    mcReq_driver->makeDir = cb->makeDir;
    mcReq_driver->deleteFile = cb->deleteFile;
    mcReq_driver->deleteDir = cb->deleteDir;
    mcReq_driver->read = cb->read;
    mcReq_driver->write = cb->write;
    mcReq_driver->seek = cb->seek;
    mcReq_driver->flush = cb->flush;
    mcReq_driver->reserved = cb->reserved;
    mcReq_driver->sync = cb->sync;
    mcReq_driver->getFoundName = cb->getFoundName;
    mcReq_driver->opHandlers = D_00451468;
    mcReq_driver->stateHandlers = D_004514A0;
    mcReq_current->op = 0;
    mcReq_current->st = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_close);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);
extern int TibExtMemFree(int, char *, int, int);
/**
 * Closes the memory-card request layer: clears the 17 callbacks and the request, frees the request and the driver block
 * to the driver's heap and clears both pointers.
 *
 * @return 0, or 2 when the layer is not open
 */
int mcReq_close(void) {
    struct sMcDrv *driver;
    struct sMcReq *request;
    if (mcReq_isOpen() == 0) return 2;
    driver = mcReq_driver;
    request = mcReq_current;
    if (driver == 0 || request == 0) return 2;
    driver->getDir = 0;
    driver->queryFreeSpace = 0;
    driver->queryFreeEntries = 0;
    driver->checkUnit = 0;
    driver->releaseUnit = 0;
    driver->open = 0;
    driver->close = 0;
    driver->makeDir = 0;
    driver->deleteFile = 0;
    driver->deleteDir = 0;
    driver->read = 0;
    driver->write = 0;
    driver->seek = 0;
    driver->flush = 0;
    driver->reserved = 0;
    driver->sync = 0;
    driver->getFoundName = 0;
    request->op = 0;
    request->st = 0;
    TibExtMemFree(driver->h, (char *)request, 116, 4);
    mcReq_current = 0;
    TibExtMemFree(mcReq_driver->h, (char *)mcReq_driver, 1068, 4);
    mcReq_driver = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_copyRequestBody);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/**
 * Copies the current memory-card request body (0x60 bytes: file descriptor, unit code, directory and file names, save
 * name) to @p dst.
 *
 * @param dst  destination
 * @return 0, or 0xC when @p dst is 0
 */
int mcReq_copyRequestBody(char *dst) {
    if (dst == 0) return 0xC;
    __builtin_memcpy(dst, &mcReq_current->body, 0x60);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startDelete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);

extern int c9698_86B8(int) __asm__("mcReq_countUnits");
extern int c9698_8550(int) __asm__("mcReq_findLowestSlot");
extern int c9698_8588(int) __asm__("mcReq_findHighestSlot");
extern void c9698_86F0(int) __asm__("mcReq_setError");
extern void c9698_B2F8(int) __asm__("mcHook_notifyBusy");
extern unsigned c9698_6210(void *, int, int) __asm__("memset");
extern char *strcpy(char *, const char *);
int mcReq_startDelete(char *str, int a, int b);
/**
 * Starts a memory-card request that deletes the save @p str.
 *
 * With unit -1 it searches the usable units (operation 2) from the lowest (@p searchDir 0) or the highest (@p searchDir 1), or uses the
 * only one (operation 5); otherwise it uses unit @p unitCode (operation 5). Clears the body and the recorded error, stores the
 * name and direction, calls the start hook mcHook_notifyBusy and the unit-check callback (driver +0x10), and enters state
 * 1. mcReq_stepDelete runs the steps.
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction (0 upward, 1 downward)
 * @return 0; 2 when the layer is not open, 0xC for a bad name or direction, 9 for a bad unit, 11 when the unit is not
 * usable, 13 when a request is running
 */
int mcReq_startDelete(char *str, int unitCode, int searchDir) {
    int firstUnit;
    struct sMcReq *request;
    struct sMcReq *request2;
    unsigned unitBit;
    if (mcReq_isOpen() == 0) return 2;
    if (str == 0) return 0xC;
    if ((unsigned)(unitCode + 1) >= 9) return 9;
    if (unitCode != -1) {
        unitBit = 1 << unitCode;
        if (!(mcReq_driver->ports & unitBit)) return 11;
    }
    if (mcReq_current->op != 0) return 13;
    c9698_6210(&mcReq_current->body, 0, 0x60);
    if (unitCode == -1) {
        if (c9698_86B8(mcReq_driver->ports) == 1) {
            firstUnit = c9698_8550(mcReq_driver->ports);
            request = mcReq_current;
            request->body.slot = firstUnit;
            request->op = 5;
        } else if (searchDir == 0) {
            firstUnit = c9698_8550(mcReq_driver->ports);
            request = mcReq_current;
            request->body.slot = firstUnit;
            request->op = 2;
        } else if (searchDir == 1) {
            firstUnit = c9698_8588(mcReq_driver->ports);
            request = mcReq_current;
            request->body.slot = firstUnit;
            request->op = 2;
        } else {
            return 0xC;
        }
    } else {
        request2 = mcReq_current;
        request2->body.slot = unitCode;
        request2->op = 5;
    }
    c9698_86F0(0);
    strcpy(mcReq_current->body.name, str);
    mcReq_current->searchDir = searchDir;
    c9698_B2F8(1);
    mcReq_driver->checkUnit(mcReq_current->body.slot);
    mcReq_current->st = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int strlen(const char *);

/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);

extern int c9818_86B8(int) __asm__("mcReq_countUnits");
extern int c9818_8550(int) __asm__("mcReq_findLowestSlot");
extern int c9818_8588(int) __asm__("mcReq_findHighestSlot");
extern void c9818_86F0(int) __asm__("mcReq_setError");
extern void c9818_B2F8(int) __asm__("mcHook_notifyBusy");
extern unsigned c9818_6210(void *, int, int) __asm__("memset");
extern char *strcpy(char *, const char *);
int mcReq_startCreate(char *str, int a, int b);
/**
 * Starts a memory-card request that creates the save @p str (operations 1 / 4).
 *
 * Like mcReq_startDelete, with a name limit: names of 0x18 characters or more give 0xC. mcReq_stepCreate runs the steps.
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction (0 upward, 1 downward)
 * @return 0 or the error codes of mcReq_startDelete
 */
int mcReq_startCreate(char *str, int unitCode, int searchDir) {
    int firstUnit;
    struct sMcReq *request;
    struct sMcReq *request2;
    unsigned unitBit;
    if (mcReq_isOpen() == 0) return 2;
    if (str == 0) return 0xC;
    if (strlen(str) >= 0x18) return 0xC;
    if ((unsigned)(unitCode + 1) >= 9) return 9;
    if (unitCode != -1) {
        unitBit = 1 << unitCode;
        if (!(mcReq_driver->ports & unitBit)) return 11;
    }
    if (mcReq_current->op != 0) return 13;
    c9818_6210(&mcReq_current->body, 0, 0x60);
    if (unitCode == -1) {
        if (c9818_86B8(mcReq_driver->ports) == 1) {
            firstUnit = c9818_8550(mcReq_driver->ports);
            request = mcReq_current;
            request->body.slot = firstUnit;
            request->op = 4;
        } else if (searchDir == 0) {
            firstUnit = c9818_8550(mcReq_driver->ports);
            request2 = mcReq_current;
            request2->body.slot = firstUnit;
            request2->op = 1;
        } else if (searchDir == 1) {
            firstUnit = c9818_8588(mcReq_driver->ports);
            request2 = mcReq_current;
            request2->body.slot = firstUnit;
            request2->op = searchDir;
        } else {
            return 0xC;
        }
    } else {
        request2 = mcReq_current;
        request2->body.slot = unitCode;
        request2->op = 4;
    }
    c9818_86F0(0);
    strcpy(mcReq_current->body.name, str);
    mcReq_current->searchDir = searchDir;
    c9818_B2F8(1);
    mcReq_driver->checkUnit(mcReq_current->body.slot);
    mcReq_current->st = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startOpen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);

extern int c99b8_86B8(int) __asm__("mcReq_countUnits");
extern int c99b8_8550(int) __asm__("mcReq_findLowestSlot");
extern int c99b8_8588(int) __asm__("mcReq_findHighestSlot");
extern void c99b8_86F0(int) __asm__("mcReq_setError");
extern void c99b8_B2F8(int) __asm__("mcHook_notifyBusy");
extern unsigned c99b8_6210(void *, int, int) __asm__("memset");
extern char *strcpy(char *, const char *);
int mcReq_startOpen_impl(char *str, int a, int b) __asm__("mcReq_startOpen");
/**
 * Starts a memory-card request that opens the save @p str (operations 3 / 6).
 *
 * Like mcReq_startDelete; mcReq_stepOpen runs the steps.
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction (0 upward, 1 downward)
 * @return 0 or the error codes of mcReq_startDelete
 */
int mcReq_startOpen_impl(char *str, int unitCode, int searchDir) {
    int firstUnit;
    struct sMcReq *request;
    struct sMcReq *request2;
    unsigned unitBit;
    if (mcReq_isOpen() == 0) return 2;
    if (str == 0) return 0xC;
    if ((unsigned)(unitCode + 1) >= 9) return 9;
    if (unitCode != -1) {
        unitBit = 1 << unitCode;
        if (!(mcReq_driver->ports & unitBit)) return 11;
    }
    if (mcReq_current->op != 0) return 13;
    c99b8_6210(&mcReq_current->body, 0, 0x60);
    if (unitCode == -1) {
        if (c99b8_86B8(mcReq_driver->ports) == 1) {
            firstUnit = c99b8_8550(mcReq_driver->ports);
            request = mcReq_current;
            request->body.slot = firstUnit;
            request->op = 6;
        } else if (searchDir == 0) {
            firstUnit = c99b8_8550(mcReq_driver->ports);
            request = mcReq_current;
            request->body.slot = firstUnit;
            request->op = 3;
        } else if (searchDir == 1) {
            firstUnit = c99b8_8588(mcReq_driver->ports);
            request = mcReq_current;
            request->body.slot = firstUnit;
            request->op = 3;
        } else {
            return 0xC;
        }
    } else {
        request2 = mcReq_current;
        request2->body.slot = unitCode;
        request2->op = 6;
    }
    c99b8_86F0(0);
    strcpy(mcReq_current->body.name, str);
    mcReq_current->searchDir = searchDir;
    c99b8_B2F8(1);
    mcReq_driver->checkUnit(mcReq_current->body.slot);
    mcReq_current->st = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startClose);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int f9208(void) __asm__("mcReq_isOpen");
int mcReq_startClose_impl(int *src) __asm__("mcReq_startClose");
/**
 * Starts a memory-card request that closes an open save file (operation 7): copies the request body from @p src, enters
 * state 24 and flushes the file (driver +0x38 with the descriptor @p src[0]). mcReq_stepClose runs the steps.
 *
 * @param src  the 0x60-byte request body of the open file
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int mcReq_startClose_impl(int *src) {
    if (f9208() == 0) return 2;
    if (src == 0) return 0xC;
    if (mcReq_current->op != 0) return 13;
    __builtin_memcpy(&mcReq_current->body, src, 0x60);
    mcReq_current->op = 7;
    mcReq_current->st = 0x18;
    mcReq_driver->flush(src[0]);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startSeek);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);

extern void c9c48_86F0(int) __asm__("mcReq_setError");
extern int mcReq_getSeekBase_9C48(void) __asm__("mcReq_getSeekBase");
int mcReq_startSeek_impl(int *src, int a, int b) __asm__("mcReq_startSeek");
/**
 * Starts a memory-card request that seeks in an open save file (operation 8, state 27).
 *
 * Copies the body from @p src; with origin 0 seeks to @p offset plus the base offset (mcReq_getSeekBase, 0) from the start,
 * otherwise passes @p offset and @p origin on. Keeps the target in +0x70.
 *
 * @param src  the 0x60-byte request body of the open file
 * @param offset  offset
 * @param origin  seek origin
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int mcReq_startSeek_impl(int *src, int offset, int origin) {
    struct sMcReq *request;
    struct sMcReq *requestAgain;
    int target;
    struct sMcDrv *driver;
    if (mcReq_isOpen() == 0) return 2;
    if (src == 0) return 0xC;
    request = mcReq_current;
    if (request->op != 0) return 13;
    request->op = 8;
    request->st = 27;
    c9c48_86F0(0);
    __builtin_memcpy(&mcReq_current->body, (char *)src, 0x60);
    if (origin == 0) {
        target = offset + mcReq_getSeekBase_9C48();
        requestAgain = mcReq_current;
        driver = mcReq_driver;
        requestAgain->expected = target;
        driver->seek(requestAgain->body.port, offset + mcReq_getSeekBase_9C48(), 0);
    } else {
        struct sMcReq *requestInner = mcReq_current;
        struct sMcDrv *driverInner = mcReq_driver;
        requestInner->expected = offset;
        driverInner->seek(requestInner->body.port, offset, origin);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startRead);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);

extern void f86F0(int) __asm__("mcReq_setError");
int mcReq_startRead_impl(int *src, int a, int b) __asm__("mcReq_startRead");
/**
 * Starts a memory-card request that reads from an open save file (operation 10, state 29): read callback (driver +0x2C)
 * with the descriptor, @p buffer and @p byteCount; @p byteCount is kept as the expected count.
 *
 * @param src  the 0x60-byte request body of the open file
 * @param buffer  destination buffer
 * @param byteCount  byte count
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int mcReq_startRead_impl(int *src, int buffer, int byteCount) {
    struct sMcReq *request;
    struct sMcReq *requestAgain;
    struct sMcDrv *driver;
    if (mcReq_isOpen() == 0) return 2;
    if (src == 0) return 0xC;
    request = mcReq_current;
    if (request->op != 0) return 13;
    request->op = 10;
    request->st = 29;
    f86F0(0);
    __builtin_memcpy(&mcReq_current->body, (char *)src, 0x60);
    requestAgain = mcReq_current;
    driver = mcReq_driver;
    requestAgain->expected = byteCount;
    driver->read(src[0], buffer, byteCount);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_startWrite);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);
extern void f86F0(int) __asm__("mcReq_setError");
int mcReq_startWrite_impl(int *src, void *a, int b) __asm__("mcReq_startWrite");
/**
 * Starts a memory-card request that writes to an open save file (operation 9, state 28): write callback (driver +0x30)
 * with the descriptor, @p buffer and @p byteCount; @p byteCount is kept as the expected count.
 *
 * @param src  the 0x60-byte request body of the open file
 * @param buffer  source buffer
 * @param byteCount  byte count
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int mcReq_startWrite_impl(int *src, void *buffer, int byteCount) {
    struct sMcReq *request;
    struct sMcReq *requestAgain;
    struct sMcDrv *driver;
    if (mcReq_isOpen() == 0) return 2;
    if (src == 0) return 0xC;
    request = mcReq_current;
    if (request->op != 0) return 13;
    request->op = 9;
    request->st = 0x1C;
    f86F0(0);
    __builtin_memcpy(&mcReq_current->body, (char *)src, 0x60);
    requestAgain = mcReq_current;
    driver = mcReq_driver;
    requestAgain->expected = byteCount;
    driver->write(src[0], buffer, byteCount);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);

extern void fB2F8w(int) __asm__("mcHook_notifyBusy");
/**
 * Polls the memory-card request layer and advances the running request.
 *
 * Calls the poll callback (driver +0x40). When the card call has finished (status 2), checks its result with the
 * current state's entry of the per-state table (10 ends the request, 0x10 is returned) and runs the operation's step
 * handler. When that finishes the request, calls the hook mcHook_notifyBusy for operations 2, 5, 7 and 11..13 and clears
 * the operation and state.
 *
 * @param statusOut  receives the status: 0 idle, 1 running, 2 finished
 * @param resultOut  receives the card call's result word
 * @return the result; 2 when the layer is not open, 13 without a poll callback, 0xC for a 0 argument, 0xF when the card
 * is idle but a request is pending
 */
int mcReq_update(int *statusOut, int *resultOut) {
    int result;
    if (mcReq_isOpen() == 0) return 2;
    if (mcReq_driver->sync == 0) return 13;
    if (statusOut == 0) return 0xC;
    if (resultOut == 0) return 0xC;
    result = mcReq_driver->sync(statusOut, resultOut);
    if (statusOut[0] == 0) {
        if (mcReq_current->op != 0) return 0xF;
    }
    if (statusOut[0] != 2) return result;
    result = mcReq_driver->stateHandlers[mcReq_current->st](result, 2, resultOut[0]);
    if (result == 10) {
        struct sMcReq *request = mcReq_current;
        request->op = 0;
        request->st = 0;
        return 10;
    }
    if (result == 0x10) return 0x10;
    result = mcReq_driver->opHandlers[mcReq_current->op](result, statusOut, resultOut);
    if (statusOut[0] == 2) {
        int finishedOp = mcReq_current->op;
        struct sMcReq *request;
        if (finishedOp == 5 || finishedOp == 2 || finishedOp == 7 || finishedOp == 11 || finishedOp == 0xC || finishedOp == 13) fB2F8w(0);
        request = mcReq_current;
        request->op = 0;
        request->st = 0;
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", mcReq_getPortSlot);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int mcReq_isOpen(void);
extern int mcReq_splitUnitCode(unsigned int, int);
/**
 * Decodes a memory-card unit code into its port and slot (mcReq_splitUnitCode). The game's card callbacks call it.
 *
 * @param unitIndex  unit code 0..7
 * @param portSlot  receives the port and slot bytes
 * @return 0; 12 when @p portSlot is 0, 2 when the layer is not open, 9 for a code of 8 or more
 */
int mcReq_getPortSlot(unsigned int unitIndex, int portSlot) {
    if (portSlot == 0) return 12;
    if (mcReq_isOpen() == 0) return 2;
    if (unitIndex >= 8) return 9;
    mcReq_splitUnitCode(unitIndex, portSlot);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_setSaveDescription);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern int mcReq_isOpen(void);
extern void fA6A8(char *, int *, int *) __asm__("mcReq_computeNewSaveSize");
extern int fB308(char *) __asm__("mcHook_checkSaveDescription");
int mcReq_setSaveDescription_impl(char *src) __asm__("mcReq_setSaveDescription");
/**
 * Stores the save description (976 bytes: icon data pointer and size, the icon.sys block, the record size at +0x3CC) in
 * the driver block (+0x50) and computes the space a new save needs into +0x420 / +0x424 (mcReq_computeNewSaveSize).
 *
 * @param src  the description
 * @return 0; 0xC when @p src is 0, 2 when the layer is not open
 */
int mcReq_setSaveDescription_impl(char *src) {
    if (src == 0) return 0xC;
    if (mcReq_isOpen() == 0) return 2;
    __builtin_memcpy(&mcReq_driver->saveDesc, src, 976);
    fA6A8(src, &mcReq_driver->saveSize, &mcReq_driver->entriesNeeded);
    return fB308(src);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_filterDirEntry);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB278(void);
/**
 * Directory-entry filter the game's search callback uses: accepts every entry (mcHook_acceptDirEntry).
 *
 * @param name  the entry name (ignored)
 * @return 1
 */
int mcReq_filterDirEntry(char* name) { return mcHook_acceptDirEntry(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_asciiToSjis);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { unsigned char b[33]; } sSjisSymbols;
extern sSjisSymbols mcReq_sjisSymbolTable;
/**
 * Converts an ASCII string to full-width Shift-JIS, as icon.sys titles need.
 *
 * Letters and digits map to 0x8260.. / 0x8281.. / 0x824F..; other printable characters to 0x81xx through the table
 * mcReq_sjisSymbolTable; Shift-JIS lead bytes 0x81..0x98 copy their two-byte character through. Each character is stored
 * byte-swapped (big-endian in memory). Quirk: no terminator is written; slogin_buildSaveDescription converts into a zeroed buffer.
 *
 * @param src  ASCII text
 * @param dst  Shift-JIS output
 */
void mcReq_asciiToSjis(char *src, unsigned short *dst) {
    sSjisSymbols tbl = mcReq_sjisSymbolTable;
    unsigned char i = 0;
    unsigned short sjisChar = *src++;
    while (sjisChar != 0) {
        if ((unsigned short)(sjisChar - 129) < 0x18) {
            sjisChar = (sjisChar << 8) + *src++;
        } else if (sjisChar >= 123) {
            sjisChar = tbl.b[sjisChar - 94] + 33024;
        } else if (sjisChar >= 97) {
            sjisChar = sjisChar + 33312;
        } else if (sjisChar >= 91) {
            sjisChar = tbl.b[sjisChar - 68] + 33024;
        } else if (sjisChar >= 65) {
            sjisChar = sjisChar + 33311;
        } else if (sjisChar >= 58) {
            sjisChar = tbl.b[sjisChar - 42] + 33024;
        } else if (sjisChar >= 0x30) {
            sjisChar = sjisChar + 33311;
        } else {
            sjisChar = tbl.b[sjisChar - 0x20] + 33024;
        }
        sjisChar = (sjisChar >> 8) | ((sjisChar & 0xFF) << 8);
        dst[i++] = sjisChar;
        sjisChar = *src++;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_sjisToAscii);
#ifdef SKIP_ASM
/**
 * Converts a full-width Shift-JIS string (byte-swapped, as mcReq_asciiToSjis writes it) back to ASCII and NUL-terminates
 * it. Stops at the first character it cannot map. Nothing in retail calls it.
 *
 * @param src  Shift-JIS text
 * @param dst  ASCII output
 */
void mcReq_sjisToAscii(unsigned short *src, char *dst) {
    sSjisSymbols tbl = mcReq_sjisSymbolTable;
    unsigned char i = 0;
    unsigned short sjisChar = *src++;
    while (sjisChar != 0) {
        sjisChar = (sjisChar >> 8) | ((sjisChar & 0xFF) << 8 & 0xFF00);
        if ((sjisChar & 0x8100) == 33024) {
            char j;
            sjisChar &= 0xFF;
            for (j = 0; j < 33; j++) {
                if (tbl.b[j] == sjisChar) {
                    sjisChar = j;
                    if (sjisChar >= 29) {
                        dst[i++] = sjisChar + 123;
                    } else if (sjisChar >= 23) {
                        dst[i++] = sjisChar + 91;
                    } else if (sjisChar >= 0x10) {
                        dst[i++] = sjisChar + 58;
                    } else {
                        dst[i++] = sjisChar + 0x20;
                    }
                    break;
                }
            }
        } else if ((sjisChar & 0x8200) == 33280) {
            if ((unsigned short)(sjisChar - 33409) < 26) {
                dst[i++] = sjisChar - 0x20;
            } else if ((unsigned short)(sjisChar - 33376) < 26) {
                dst[i++] = sjisChar - 0x1F;
            } else if ((unsigned short)(sjisChar - 33359) < 10) {
                dst[i++] = sjisChar - 0x1F;
            }
        } else {
            break;
        }
        sjisChar = *src++;
    }
    dst[i] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_computeNewSaveSize);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB280(void);
/**
 * Forwards to mcHook_computeNewSaveSize (the space a new save needs) with the caller's arguments.
 *
 * @return no meaningful value (mcHook_computeNewSaveSize returns nothing)
 */
int mcReq_computeNewSaveSize(void) { return mcHook_computeNewSaveSize(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_getCrcInterface);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB2C8(void);
/**
 * Forwards to mcHook_getCrcInterface (fetch the CRC interface) with the caller's argument.
 *
 * @return no meaningful value (mcHook_getCrcInterface returns nothing)
 */
int mcReq_getCrcInterface(void) { return mcHook_getCrcInterface(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_getSeekBase);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB300(void);
/**
 * Returns the base offset of seeks in a save file (mcHook_getSeekBase, always 0).
 *
 * @return 0
 */
int mcReq_getSeekBase(void) { return mcHook_getSeekBase(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_splitUnitCode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void mcReq_splitUnitCode_impl(unsigned a, unsigned char *p) __asm__("mcReq_splitUnitCode");
/**
 * Splits a memory-card unit code into port and slot bytes: codes 0..3 are port 0 slots 0..3, codes 4..7 port 1 slots
 * 0..3. Other codes leave the output untouched.
 *
 * @param unitCode  unit code
 * @param portSlotOut  receives port, slot
 */
void mcReq_splitUnitCode_impl(unsigned unitCode, unsigned char *portSlotOut) {
    switch (unitCode) {
    case 0: portSlotOut[0] = 0; portSlotOut[1] = 0; break;
    case 1: portSlotOut[0] = 0; portSlotOut[1] = 1; break;
    case 2: portSlotOut[0] = 0; portSlotOut[1] = 2; break;
    case 3: portSlotOut[0] = 0; portSlotOut[1] = 3; break;
    case 4: portSlotOut[0] = 1; portSlotOut[1] = 0; break;
    case 5: portSlotOut[0] = 1; portSlotOut[1] = 1; break;
    case 6: portSlotOut[0] = 1; portSlotOut[1] = 2; break;
    case 7: portSlotOut[0] = 1; portSlotOut[1] = 3; break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_buildSearchPattern);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char str_star_b[];
extern void memset(char *, int, int);
extern void func_0041605C(char *, char *, int);
/**
 * Builds the directory-search pattern for a save: "*" followed by the save name, in a zeroed 32-byte buffer.
 *
 * @param dst  pattern buffer (32 bytes)
 * @param saveDesc  ignored (callers pass the save description)
 * @param src  the save name
 * @return the end of the pattern
 */
char *mcReq_buildSearchPattern(char *dst, sMcSaveParam *saveDesc, char *src) {
    char *nameDst = dst + 1;
    memset(dst, 0, 0x20);
    *dst = str_star_b[0];
    func_0041605C(nameDst, src, strlen(src));
    return nameDst + strlen(src);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_buildSaveNames);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { char b[9]; } K9;
extern K9 str_PSCD10088;
extern void f6210(char *, int, int) __asm__("memset");
extern void f605C(char *, char *, int) __asm__("func_0041605C");
void mcReq_buildSaveNames_impl(char *a, char *b, sMcSaveParam *unused, char *str) __asm__("mcReq_buildSaveNames");
/**
 * Builds a save's directory name and file name, both "PSCD10088" followed by the save name, each in a zeroed 32-byte
 * buffer.
 *
 * @param dirName  receives the directory name
 * @param fileName  receives the file name
 * @param saveDesc  ignored (callers pass the save description)
 * @param str  the save name
 */
void mcReq_buildSaveNames_impl(char *dirName, char *fileName, sMcSaveParam *saveDesc, char *str) {
    char *nameDst;
    f6210(dirName, 0, 0x20);
    *(K9 *)dirName = str_PSCD10088;
    nameDst = dirName + 9;
    f605C(nameDst, str, strlen(str));
    nameDst = fileName + 9;
    strlen(str);
    f6210(fileName, 0, 0x20);
    *(K9 *)fileName = str_PSCD10088;
    f605C(nameDst, str, strlen(str));
    strlen(str);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_stepCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern char str_view_ico_b[];   /* "view.ico" */
extern char str_icon_sys_b[];   /* "icon.sys" */
extern void fA8F8_86F0(int) __asm__("mcReq_setError");
extern char *fA8F8_A7A0(char *, sMcSaveParam *, char *) __asm__("mcReq_buildSearchPattern");

/**
 * Step handler of the memory-card create operations (1: search the usable units, 4: one unit); the per-op handler
 * table D_00451468 holds it for ops 1 and 4.
 *
 * Unit check, then a search for the save (it must not exist: error 8), the free space (at least driver +0x420 bytes,
 * else 14), a second query against +0x424 and the directory ("PSCD10088" + name). Then creates the main file dir/dir
 * (mode 515), flushes and closes it; writes the icon data to view.ico and the 964-byte icon.sys block from the save
 * description, each opened, written, flushed and closed; and releases the unit. With no card (3) operation 1 tries the
 * next usable unit; when the unit is released with an error it does too. Failures go to the flush, close or release
 * cleanup (mcReq_startFlushCleanup, mcReq_startCloseCleanup, mcReq_startReleaseCleanup).
 *
 * @param callResult  checked result of the finished card call
 * @param st  receives 1 while running, 2 when finished
 * @param res  the poll's result word
 * @return 0 or an error code
 */
int mcReq_stepCreate(int callResult, int *st, int *res) {
    char path[80];
    int pollValue;

    if (st == 0) return 18;
    if (res == 0) return 18;
    if (mcReq_current->op != 1 && mcReq_current->op != 4) return 18;
    *st = 1;
    switch (mcReq_current->st) {
    case 1:
        if (callResult == 0) {
            fA8F8_A7A0(path, &mcReq_driver->saveDesc, mcReq_current->body.name);
            mcReq_current->st = 2;
            mcReq_driver->getDir(path, mcReq_current->body.slot);
            break;
        }
        if (callResult == 3) {
            if (mcReq_current->op == 1) {
                mcReq_current->body.slot = mcReq_findNextSlot(mcReq_driver->ports, mcReq_current->searchDir);
                if (mcReq_current->body.slot != -1) {
                    mcReq_current->st = 1;
                    mcReq_driver->checkUnit(mcReq_current->body.slot);
                    return 0;
                }
                *st = 2;
                return mcReq_getError();
            }
            *st = 2;
            return 3;
        }
        *st = 2;
        return callResult;
    case 2:
        if (callResult == 0) {
            pollValue = *res;
            if (pollValue == 0) {
                mcReq_current->st = 3;
                mcReq_driver->queryFreeSpace(mcReq_current->body.slot);
                break;
            }
            if (pollValue == 1) {
                fA8F8_86F0(8);
                mcReq_current->st = 26;
                mcReq_driver->releaseUnit(mcReq_current->body.slot);
                break;
            }
            *st = 2;
            return 10;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 3:
        if (callResult == 0) {
            pollValue = *res;
            if (pollValue < 0) {
                *st = 2;
                return 10;
            }
            if ((unsigned)pollValue >= (unsigned)mcReq_driver->saveSize) {
                mcReq_current->st = 4;
                mcReq_driver->queryFreeEntries(mcReq_current->body.slot);
                break;
            }
            fA8F8_86F0(0xE);
            mcReq_current->st = 26;
            mcReq_driver->releaseUnit(mcReq_current->body.slot);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 4:
        if (callResult == 0) {
            pollValue = *res;
            if (pollValue < 0) {
                *st = 2;
                return 10;
            }
            if ((unsigned)pollValue >= (unsigned)mcReq_driver->entriesNeeded) {
                mcReq_buildSaveNames_impl(mcReq_current->body.dir, mcReq_current->body.file, &mcReq_driver->saveDesc,
                                   mcReq_current->body.name);
                mcReq_current->st = 5;
                mcReq_driver->makeDir(mcReq_current->body.dir, mcReq_driver->saveSize,
                                                              mcReq_current->body.slot);
                break;
            }
            fA8F8_86F0(0xE);
            mcReq_current->st = 26;
            mcReq_driver->releaseUnit(mcReq_current->body.slot);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 5:
        if (callResult == 0) {
            mcReq_current->st = 0xE;
            mcReq_driver->open(mcReq_current->body.dir,
                                                                  mcReq_current->body.dir, mcReq_current->body.slot, 515);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 0xE:
        if (callResult == 0) {
            int openResult = *res;
            if (openResult >= 0) {
                mcReq_current->st = 0x10;
                mcReq_current->body.port = openResult;
                mcReq_driver->flush(openResult);
                break;
            }
            fA8F8_86F0(0);
            return mcReq_startFlushCleanup(&mcReq_current->body, st);
        }
        fA8F8_86F0(callResult);
        return mcReq_startFlushCleanup(&mcReq_current->body, st);
    case 0x10:
        if (callResult == 0) {
            mcReq_current->st = 17;
            mcReq_driver->close(mcReq_current->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startCloseCleanup(&mcReq_current->body, st);
    case 17:
        if (callResult == 0) {
            mcReq_current->st = 6;
            mcReq_driver->open(mcReq_current->body.dir, str_view_ico_b,
                                                                  mcReq_current->body.slot, 515);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 6:
        if (callResult == 0) {
                int openResult = *res;
            mcReq_current->st = 7;
            mcReq_current->body.port = openResult;
            mcReq_driver->write(openResult, mcReq_driver->saveDesc.port, mcReq_driver->saveDesc.id);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 7:
        if (callResult == 0) {
            mcReq_current->st = 8;
            mcReq_driver->flush(mcReq_current->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startFlushCleanup(&mcReq_current->body, st);
    case 8:
        if (callResult == 0) {
            mcReq_current->st = 9;
            mcReq_driver->close(mcReq_current->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startCloseCleanup(&mcReq_current->body, st);
    case 9:
        if (callResult == 0) {
            mcReq_current->st = 10;
            mcReq_driver->open(mcReq_current->body.dir, str_icon_sys_b,
                                                                  mcReq_current->body.slot, 515);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 10:
        if (callResult == 0) {
                int openResult = *res;
            mcReq_current->body.port = openResult;
            mcReq_current->st = 11;
            mcReq_driver->write(openResult, &mcReq_driver->saveDesc.icon, 964);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 11:
        if (callResult == 0) {
            mcReq_current->st = 0xC;
            mcReq_driver->flush(mcReq_current->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startFlushCleanup(&mcReq_current->body, st);
    case 0xC:
        if (callResult == 0) {
            mcReq_current->st = 13;
            mcReq_driver->close(mcReq_current->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startCloseCleanup(&mcReq_current->body, st);
    case 13:
        if (callResult == 0) {
            mcReq_current->st = 26;
            mcReq_driver->releaseUnit(mcReq_current->body.slot);
            break;
        }
        fA8F8_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 26:
        if (callResult == 0) {
            if (mcReq_current->op == 4) {
                *st = 2;
                return mcReq_getError();
            }
            if (mcReq_getError() == 0) {
                *st = 2;
                return 0;
            }
            mcReq_current->body.slot = mcReq_findNextSlot(mcReq_driver->ports, mcReq_current->searchDir);
            if (mcReq_current->body.slot != -1) {
                mcReq_current->st = 1;
                mcReq_driver->checkUnit(mcReq_current->body.slot);
                return 0;
            }
            *st = 2;
            return mcReq_getError();
        }
        *st = 2;
        return callResult;
    default:
        *st = 2;
        return 18;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcReq_stepDelete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* mcReq_current: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *mcReq_current;

/* mcReq_driver: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *mcReq_driver;

extern char str_view_ico_b[];   /* "view.ico" */
extern char str_icon_sys_b[];   /* "icon.sys" */
extern void fAE50_86F0(int) __asm__("mcReq_setError");
extern char *fAE50_A7A0(char *, sMcSaveParam *, char *) __asm__("mcReq_buildSearchPattern");

/**
 * Step handler of the memory-card delete operations (2: search the usable units, 5: one unit); the per-op handler
 * table D_00451468 holds it for ops 2 and 5.
 *
 * Unit check, then a search for the save; when found, copies its name (driver callback +0x44) and deletes icon.sys,
 * view.ico and the main file in its directory, then the directory, and releases the unit. Not finding a file (4) is
 * ignored; no card (3) skips to releasing the unit; other errors are recorded while the deletes go on. Not finding the
 * save records error 4. Operation 2 tries the next usable unit after an error.
 *
 * @param callResult  checked result of the finished card call
 * @param st  receives 1 while running, 2 when finished
 * @param res  the poll's result word
 * @return 0 or an error code
 */
int mcReq_stepDelete(int callResult, int *st, int *res) {
    char path[80];

    if (st == 0) return 18;
    if (res == 0) return 18;
    if (mcReq_current->op != 2 && mcReq_current->op != 5) return 18;
    *st = 1;
    switch (mcReq_current->st) {
    case 1:
        fAE50_86F0(0);
        if (callResult == 0) {
            fAE50_A7A0(path, &mcReq_driver->saveDesc, mcReq_current->body.name);
            mcReq_current->st = 2;
            mcReq_driver->getDir(path, mcReq_current->body.slot);
            break;
        }
        if (callResult == 3) {
            if (mcReq_current->op == 2) {
                mcReq_current->body.slot = mcReq_findNextSlot(mcReq_driver->ports, mcReq_current->searchDir);
                if (mcReq_current->body.slot != -1) {
                    mcReq_current->st = 1;
                    mcReq_driver->checkUnit(mcReq_current->body.slot);
                    return 0;
                }
                *st = 2;
                return mcReq_getError();
            }
            *st = 2;
            return 3;
        }
        *st = 2;
        return callResult;
    case 2:
        if (callResult == 0) {
            if (*res == 1) {
                mcReq_driver->getFoundName(mcReq_current->body.dir);
                mcReq_current->st = 19;
                mcReq_driver->deleteFile(mcReq_current->body.dir, str_icon_sys_b,
                                                                mcReq_current->body.slot);
                break;
            }
            if (*res == 0) {
                fAE50_86F0(4);
                return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
            }
            *st = 2;
            return 10;
        }
        fAE50_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 19:
        if (callResult == 3) {
            if (mcReq_getError() == 0) fAE50_86F0(callResult);
            return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
        }
        if (callResult != 4) {
            if (mcReq_getError() == 0) fAE50_86F0(callResult);
        }
        mcReq_current->st = 20;
        mcReq_driver->deleteFile(mcReq_current->body.dir, str_view_ico_b,
                                                        mcReq_current->body.slot);
        break;
    case 20:
        if (callResult == 3) {
            if (mcReq_getError() == 0) fAE50_86F0(callResult);
            return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
        }
        if (callResult != 4) {
            if (mcReq_getError() == 0) fAE50_86F0(callResult);
        }
        mcReq_current->st = 21;
        mcReq_driver->deleteFile(mcReq_current->body.dir,
                                                        mcReq_current->body.dir, mcReq_current->body.slot);
        break;
    case 21:
        if (callResult == 3) {
            if (mcReq_getError() == 0) fAE50_86F0(callResult);
            return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
        }
        if (callResult != 4) {
            if (mcReq_getError() == 0) fAE50_86F0(callResult);
        }
        mcReq_current->st = 18;
        mcReq_driver->deleteDir(mcReq_current->body.dir, mcReq_current->body.slot);
        break;
    case 18:
        if (callResult == 0) {
            if (mcReq_getError() == 0) fAE50_86F0(0);
            mcReq_current->st = 26;
            mcReq_driver->releaseUnit(mcReq_current->body.slot);
            break;
        }
        if (mcReq_getError() == 0) fAE50_86F0(callResult);
        return mcReq_startReleaseCleanup(mcReq_current->body.slot, st);
    case 26:
        if (callResult == 0) {
            if (mcReq_current->op == 5) {
                *st = 2;
                return mcReq_getError();
            }
            if (mcReq_getError() == 0) {
                *st = 2;
                return 0;
            }
            mcReq_current->body.slot = mcReq_findNextSlot(mcReq_driver->ports, mcReq_current->searchDir);
            if (mcReq_current->body.slot != -1) {
                mcReq_current->st = 1;
                mcReq_driver->checkUnit(mcReq_current->body.slot);
                return 0;
            }
            *st = 2;
            return mcReq_getError();
        }
        *st = 2;
        return callResult;
    default:
        *st = 2;
        return 18;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcHook_acceptDirEntry);
#ifdef SKIP_ASM
/**
 * Returns 1 (the save-entry filter behind mcReq_filterDirEntry accepts everything).
 *
 * @return 1
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcHook_acceptDirEntry(void) { return 1; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcHook_computeNewSaveSize);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
static inline unsigned roundup_kb(unsigned v) { if (v & 0x3FF) return ((v >> 10) + 1) << 10; return (v >> 10) << 10; }
/**
 * Computes the space a new save needs: the record size (+0x3CC) and the icon size (+0x4) of the save description, each
 * rounded up to 1 KB, plus 5120 bytes; @p kind is set to 3.
 *
 * @param saveDesc  the save description
 * @param out  receives the byte count
 * @param kind  receives 3 (compared with the second space query)
 */
void mcHook_computeNewSaveSize(sMcSaveParam *saveDesc, int *out, int *kind) {
    unsigned recordBytes = roundup_kb(saveDesc->recordSize);
    unsigned iconBytes = roundup_kb(saveDesc->id);
    *out = recordBytes + iconBytes + 5120;
    *kind = 3;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcHook_getCrcInterface);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char *crcIface_get(void);
/**
 * Stores the CRC interface (crcIface_get, crcIface_funcTable) through @p out when it is not 0.
 *
 * @param out  output pointer
 */
void mcHook_getCrcInterface(char **out) {
    char *crcIface = crcIface_get();
    if (out) *out = crcIface;
}
#endif

/**
 * Empty hook of the memory-card request layer (called with 1 when a request starts and 0 when some finish).
 */
void mcHook_notifyBusy(void) {
}

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcHook_getSeekBase);
#ifdef SKIP_ASM
/**
 * Returns 0 (the base offset of seeks in a save file).
 *
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcHook_getSeekBase(void) { return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", mcHook_checkSaveDescription);
#ifdef SKIP_ASM
/**
 * Checks the save description pointer: 0 when set, 12 when 0.
 *
 * @param saveDesc  the description
 * @return 0 or 12
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int mcHook_checkSaveDescription(int saveDesc) { return saveDesc ? 0 : 12; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_makeTableEntry);
#ifdef SKIP_ASM
/**
 * Computes one entry of the CRC-32 table: eight shifts of @p crc with the reflected polynomial 0xEDB88320.
 *
 * @param crc  the table index
 * @return the table entry
 */
// From peak3project's fork (github.com/peak3project/ssx3).
unsigned int crcIface_makeTableEntry(unsigned int crc) {
    unsigned char i = 8;
    do {
        if (crc & 1) {
            crc = (crc >> 1) ^ 0xEDB88320;
        } else {
            crc >>= 1;
        }
    } while (--i != 0);
    return crc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_fillTable);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int crcIface_makeTableEntry(unsigned int);
/**
 * Fills a 256-entry CRC-32 table (crcIface_makeTableEntry for each index).
 *
 * @param table  the table
 */
void crcIface_fillTable(unsigned int *table) {
    unsigned int i;
    for (i = 0; i < 256; i++) {
        *table++ = crcIface_makeTableEntry(i);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_start);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* crcIface_state: the CRC-32 state (crcIface_start allocates the 0x400-byte table). */
/* outSize / outDigest are the output block (sDigestBlock: size, digest pointer) its finish slot hands back */
struct sCrc { unsigned *table; int outSize; int *outDigest; unsigned crc; int heap; };

extern struct sCrc crcIface_state;

extern void *TibExtMemAlloc(int, int, int);
/**
 * Starts the CRC interface crcIface_state: allocates its 1 KB table from @p heapId, fills it, and resets the digest (all ones)
 * and the output block.
 *
 * @param heapId  heap
 * @return 0; 5 when already started, 1 when the allocation fails
 */
int crcIface_start(unsigned heapId) {
    unsigned *table;
    if (crcIface_state.table != 0) return 5;
    table = TibExtMemAlloc(heapId, 0x400, 4);
    crcIface_state.table = table;
    if (table == 0) return 1;
    crcIface_state.heap = heapId;
    crcIface_fillTable(table);
    crcIface_state.outSize = 4;
    crcIface_state.crc = 0xFFFFFFFF;
    crcIface_state.outDigest = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_stop);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* crcIface_state: the CRC-32 state (crcIface_start allocates the 0x400-byte table). */
extern struct sCrc crcIface_state;

extern int TibExtMemFree(int, char *, int, int);
/**
 * Stops the CRC interface: frees its table to the heap it came from.
 *
 * @return 0, or 6 when not started
 */
int crcIface_stop(void) {
    struct sCrc *crcState = &crcIface_state;
    if (crcIface_state.table == 0) return 6;
    TibExtMemFree(crcState->heap, (char *)crcState->table, 0x400, 4);
    crcIface_state.table = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_isStarted);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* crcIface_state: the CRC-32 state (crcIface_start allocates the 0x400-byte table). */
extern struct sCrc crcIface_state;

/**
 * Returns whether the CRC interface is started (its table exists). Slot 2 of the CRC interface crcIface_funcTable.
 *
 * @return 1 when started, else 0
 */
int crcIface_isStarted(void) { return crcIface_state.table != 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_reset);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* crcIface_state: the CRC-32 state (crcIface_start allocates the 0x400-byte table). */
extern struct sCrc crcIface_state;

/**
 * Resets the CRC-32 digest to all ones. Slot 3 of the CRC interface.
 *
 * @return 0, or 6 when not started
 */
int crcIface_reset(void) {
    unsigned long long initialCrc = 0xFFFFFFFF;
    if (crcIface_state.table == 0) return 6;
    crcIface_state.crc = initialCrc;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* crcIface_state: the CRC-32 state (crcIface_start allocates the 0x400-byte table). */
extern struct sCrc crcIface_state;

/**
 * Adds @p n bytes to the CRC-32 digest (table-driven, reflected). Slot 4 of the CRC interface.
 *
 * @param buf  the data
 * @param n  byte count
 * @return 0; 6 when not started, 2 when @p buf is 0
 */
int crcIface_update(unsigned char *buf, unsigned n) {
    unsigned crc = crcIface_state.crc;
    unsigned i;
    if (crcIface_state.table == 0) return 6;
    if (buf == 0) return 2;
    for (i = 0; i < n; i++)
        crc = ((crc >> 8) & 0xFFFFFF) ^ crcIface_state.table[(crc ^ buf[i]) & 0xFF];
    crcIface_state.crc = crc;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_finish);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* crcIface_state: the CRC-32 state (crcIface_start allocates the 0x400-byte table). */
extern struct sCrc crcIface_state;

extern int D_004A5B1C;
/**
 * Finishes the CRC-32 digest. Slot 5 of the CRC interface.
 *
 * Stores the inverted digest in D_004A5B1C and points the output block (size 4, then that pointer, at +0x4 of
 * crcIface_state) at it.
 *
 * @param out  receives the output block
 * @return 0; 6 when not started, 2 when @p out is 0
 */
int crcIface_finish(int **out) {
    struct sCrc *crcState = &crcIface_state;
    if (crcIface_state.table == 0) return 6;
    if (out == 0) return 2;
    crcState->outDigest = &D_004A5B1C;
    crcState->outSize = 4;
    D_004A5B1C = ~crcState->crc;
    *out = &crcState->outSize;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_getDigestSize);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* crcIface_state: the CRC-32 state (crcIface_start allocates the 0x400-byte table). */
extern struct sCrc crcIface_state;

/**
 * Returns the digest size of the CRC interface (4 bytes). Slot 6 of the CRC interface.
 *
 * @param out  receives 4
 * @return 0; 6 when not started, 2 when @p out is 0
 */
int crcIface_getDigestSize(int *out) {
    if (crcIface_state.table == 0) return 6;
    if (out == 0) return 2;
    *out = 4;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", crcIface_get);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char crcIface_funcTable[];
/**
 * Returns the CRC interface crcIface_funcTable: start (crcIface_start), stop, started, reset, update, finish, digest size.
 *
 * @return the interface table
 */
char *crcIface_get(void) { return crcIface_funcTable; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_start);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int xorIface_cipherState;

/**
 * Starts the XOR cipher interface (slot 0 of xorIface_funcTable): moves it from stopped (state 4) to started without a key (6).
 *
 * @return 0, or 3 when it is not stopped
 */
int xorIface_start(void) {
    if (xorIface_cipherState != 4) return 3;
    xorIface_cipherState = 6;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_stop);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (xorIface_setKey): xorIface_keyStart .. xorIface_keyEnd is the key, xorIface_keyCursor the next key byte, xorIface_cipherState
   the cipher state (4 = idle). */
extern unsigned char *xorIface_keyStart;

extern unsigned char *xorIface_keyEnd;

extern unsigned char *xorIface_keyCursor;

extern int xorIface_cipherState;

/**
 * Stops the XOR cipher interface (slot 1): forgets the key and returns to state 4.
 *
 * @return 0
 */
int xorIface_stop(void) {
    xorIface_keyStart = 0;
    xorIface_keyEnd = 0;
    xorIface_keyCursor = 0;
    xorIface_cipherState = 4;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_isStarted);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int xorIface_cipherState;

/**
 * Returns whether the XOR cipher interface is started (state not 4). Slot 2.
 *
 * @return 1 when started
 */
int xorIface_isStarted(void) { return xorIface_cipherState != 4; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_setKey);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (xorIface_setKey): xorIface_keyStart .. xorIface_keyEnd is the key, xorIface_keyCursor the next key byte, xorIface_cipherState
   the cipher state (4 = idle). */
extern unsigned char *xorIface_keyStart;

extern unsigned char *xorIface_keyEnd;

extern unsigned char *xorIface_keyCursor;

extern int xorIface_cipherState;

/**
 * Sets the XOR cipher key (slot 3): the key runs from @p keyBytes for @p keyLen bytes and the position restarts at its first byte
 * (state 5).
 *
 * @param keyBytes  key bytes (kept by pointer)
 * @param keyLen  key length
 * @return 0; 2 for a 0 argument, 4 when stopped
 */
int xorIface_setKey(unsigned char *keyBytes, int keyLen) {
    if (keyBytes == 0 || keyLen == 0) return 2;
    if (xorIface_cipherState == 4) return 4;
    xorIface_keyStart = keyBytes;
    xorIface_keyEnd = keyBytes + keyLen - 1;
    xorIface_keyCursor = keyBytes;
    xorIface_cipherState = 5;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_encrypt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (xorIface_setKey): xorIface_keyStart .. xorIface_keyEnd is the key, xorIface_keyCursor the next key byte, xorIface_cipherState
   the cipher state (4 = idle). */
extern unsigned char *xorIface_keyStart;

extern unsigned char *xorIface_keyEnd;

extern unsigned char *xorIface_keyCursor;

extern int xorIface_cipherState;

/**
 * Encrypts @p n bytes in place (slot 4): XORs each with the next key byte, wrapping to the key's start, and continues
 * from the key position the previous call left (state 7).
 *
 * @param buf  the data
 * @param n  byte count
 * @return 0; 2 for a 0 argument, or the current state when it is neither keyed (5) nor encrypting (7)
 */
int xorIface_encrypt(unsigned char *buf, unsigned n) {
    unsigned i;
    if (buf == 0 || n == 0) return 2;
    if (xorIface_cipherState != 5 && xorIface_cipherState != 7) return xorIface_cipherState;
    for (i = 0; i < n; i++) {
        buf[i] ^= *xorIface_keyCursor;
        if (xorIface_keyCursor == xorIface_keyEnd) xorIface_keyCursor = xorIface_keyStart;
        else xorIface_keyCursor = xorIface_keyCursor + 1;
    }
    xorIface_cipherState = 7;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_decrypt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (xorIface_setKey): xorIface_keyStart .. xorIface_keyEnd is the key, xorIface_keyCursor the next key byte, xorIface_cipherState
   the cipher state (4 = idle). */
extern unsigned char *xorIface_keyStart;

extern unsigned char *xorIface_keyEnd;

extern unsigned char *xorIface_keyCursor;

extern int xorIface_cipherState;

/**
 * Decrypts @p n bytes in place (slot 5): the same XOR as xorIface_encrypt, allowed when keyed (5) or decrypting (8), and
 * leaves state 8.
 *
 * @param buf  the data
 * @param n  byte count
 * @return 0; 2 for a 0 argument, or the current state when it is neither 5 nor 8
 */
int xorIface_decrypt(unsigned char *buf, unsigned n) {
    unsigned i;
    if (buf == 0 || n == 0) return 2;
    if (xorIface_cipherState != 5 && xorIface_cipherState != 8) return xorIface_cipherState;
    for (i = 0; i < n; i++) {
        buf[i] ^= *xorIface_keyCursor;
        if (xorIface_keyCursor == xorIface_keyEnd) xorIface_keyCursor = xorIface_keyStart;
        else xorIface_keyCursor = xorIface_keyCursor + 1;
    }
    xorIface_cipherState = 8;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_rewind);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (xorIface_setKey): xorIface_keyStart .. xorIface_keyEnd is the key, xorIface_keyCursor the next key byte, xorIface_cipherState
   the cipher state (4 = idle). */
extern unsigned char *xorIface_keyStart;

extern unsigned char *xorIface_keyCursor;

extern int xorIface_cipherState;

/**
 * Rewinds the XOR cipher to the key's first byte and back to state 5 (slot 6); does nothing unless a key is set.
 *
 * @return 0
 */
int xorIface_rewind(void) {
    if (xorIface_cipherState == 5 || xorIface_cipherState == 7 || xorIface_cipherState == 8) {
        xorIface_cipherState = 5;
        xorIface_keyCursor = xorIface_keyStart;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", xorIface_get);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char xorIface_funcTable[];
/**
 * Returns the XOR cipher interface xorIface_funcTable: start, stop, started, set key, encrypt, decrypt, rewind.
 *
 * @return the interface table
 */
char *xorIface_get(void) { return xorIface_funcTable; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_offsetCardError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *saveRec_offsetCardError_impl(char *p) __asm__("saveRec_offsetCardError");
/**
 * Offsets an error of the memory-card request layer into the save-record manager's range: nonzero codes get 100 added.
 *
 * @param errorCode  the error (declared as a pointer)
 * @return 0, or @p errorCode + 100
 */
char *saveRec_offsetCardError_impl(char *errorCode) {
    if (errorCode == 0) return 0;
    return errorCode + 100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_offsetCipherError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *saveRec_offsetCipherError_impl(char *p) __asm__("saveRec_offsetCipherError");
/**
 * Offsets an XOR cipher error into the save-record manager's range: nonzero codes get 200 added.
 *
 * @param errorCode  the error (declared as a pointer)
 * @return 0, or @p errorCode + 200
 */
char *saveRec_offsetCipherError_impl(char *errorCode) {
    if (errorCode == 0) return 0;
    return errorCode + 200;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_offsetCrcError);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *saveRec_offsetCrcError_impl(char *p) __asm__("saveRec_offsetCrcError");
/**
 * Offsets a CRC interface error into the save-record manager's range: nonzero codes get 300 added.
 *
 * @param errorCode  the error (declared as a pointer)
 * @return 0, or @p errorCode + 300
 */
char *saveRec_offsetCrcError_impl(char *errorCode) {
    if (errorCode == 0) return 0;
    return errorCode + 300;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_getMemTypeAlignment);
#ifdef SKIP_ASM
/**
 * Returns the allocation alignment (4) and size granularity (1) of a save-record memory type; all five types are the
 * same.
 *
 * @param memType  memory type
 * @param x  receives the alignment
 * @param y  receives the granularity
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void saveRec_getMemTypeAlignment(unsigned memType, int *x, int *y) {
    int sx = 4, sy = 1;
    switch (memType) {
    case 0: sx = 4; sy = 1; break;
    case 1: sx = 4; sy = 1; break;
    case 2: sx = 4; sy = 1; break;
    case 3: sx = 4; sy = 1; break;
    case 4: sx = 4; sy = 1; break;
    }
    *x = sx;
    *y = sy;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_getBlockSize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003FB858b(int, int *, int *) __asm__("saveRec_getMemTypeAlignment");
int saveRec_getBlockSize(int a, int b);
/**
 * Returns the block size of a save record: @p size plus its 12-byte header, rounded up to the type's granularity
 * (saveRec_getMemTypeAlignment, which is 1, so @p size + 12).
 *
 * @param size  record data size
 * @param memType  memory type
 * @return the block size
 */
int saveRec_getBlockSize(int size, int memType) {
    int x = 4;
    unsigned y = 1;
    unsigned rawSize;
    unsigned extra;
    func_003FB858b(memType, &x, (int *)&y);
    rawSize = size + 0xC;
    if (rawSize < y) return y;
    extra = (y - rawSize % y) % y;
    return size + 0xC + extra;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_getHeader);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL[];

extern int memcmp(int, char *, int);
/**
 * Returns the 12-byte header in front of a save-record buffer from saveRec_allocBuffer.
 *
 * @param userPtr  the record buffer
 * @param checkTag  nonzero to require the header to still hold "TAG_SENTINEL"
 * @return @p userPtr - 12, or 0 when the check fails
 */
int saveRec_getHeader(int userPtr, int checkTag) {
    int header = userPtr - 0xC;
    if (checkTag & 0xFF) {
        if (memcmp(header, str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL, 0xC) != 0) header = 0;
    }
    return header;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_fillHeader);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { char b[4]; } W3FB958;
typedef struct { W3FB958 x, y; } V3FB958;
/**
 * Fills a 12-byte save-record header: key word, size word, then the 4 CRC bytes.
 *
 * @param dst  the header
 * @param recordKey  record key
 * @param recordSize  record size
 * @param recordCrc  record CRC
 */
void saveRec_fillHeader(W3FB958 *dst, int recordKey, int recordSize, W3FB958 *recordCrc) {
    V3FB958 packed;
    *(int *)&packed.x = recordKey;
    *(int *)&packed.y = recordSize;
    dst[0] = packed.x;
    dst[1] = packed.y;
    dst[2] = *recordCrc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_restoreSentinel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL[];

/**
 * Writes "TAG_SENTINEL" (12 bytes) back into a save-record header area.
 *
 * @param dst  the header
 */
void saveRec_restoreSentinel(char *dst) {
    __builtin_memcpy(dst, str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL, 12);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_splitHeader);
#ifdef SKIP_ASM
/**
 * Splits a 12-byte save-record header into its key, size and CRC words; outputs may be 0.
 *
 * @param src  the header
 * @param outWord0  receives the key
 * @param outWord1  receives the size
 * @param outWord2  receives the CRC
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void saveRec_splitHeader(char *src, char *outWord0, char *outWord1, char *outWord2) {
    if (outWord0) __builtin_memcpy(outWord0, src, 4);
    if (outWord1) __builtin_memcpy(outWord1, src + 4, 4);
    if (outWord2) __builtin_memcpy(outWord2, src + 8, 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_findTableEntry);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
struct sMcEnt { int key; int a; int b; int c; };

struct sMcKey { char b[4]; };

/* The save cipher interface (names ours): [U] saveRec_open starts it with the heap, saveRec_close stops it;
   saveRec_encrypt / saveRec_decrypt set the key, encrypt or decrypt a buffer, then rewind. */
struct sMcIfA {
    int (*start)(int);                  /* 0x00 */
    int (*stop)(void);                  /* 0x04 */
    int pad08;
    int (*setKey)(char *, int);         /* 0x0C: (key, length) */
    int (*encrypt)(int, int);           /* 0x10: (data, byte count) */
    int (*decrypt)(int, int);           /* 0x14: (data, byte count) */
    int (*rewind)(void);                /* 0x18 */
};

/* The CRC interface (names ours): [U] start / stop as sMcIfA; saveRec_computeCrc resets, updates over a buffer and
   finishes into the output block. */
struct sMcIfB {
    int (*start)(int);                  /* 0x00 */
    int (*stop)(void);                  /* 0x04 */
    int pad08;
    int (*reset)(int, int, int);        /* 0x0C */
    int (*update)(int, int);            /* 0x10: (data, byte count) */
    int (*finish)(int);                 /* 0x14: (output block) */
};

/* Member names are ours, from [U]: saveRec_open / saveRec_close (open, close), saveRec_beginOpen.. (the operations) and
   the poll saveRec_update. f74 .. f9C keep their offsets: code that may not change spells them. */
struct sMcState {
    struct sMcParams params;            /* 0x00 */
    char *cipherKey;                    /* 0x64: copy of the open parameters' cipher key */
    int cipherKeyLen;                   /* 0x68 */
    struct sMcIfA *cipher;              /* 0x6C: the save cipher (may be 0) */
    struct sMcIfB *crcIface;            /* 0x70: the CRC interface (mcReq_getCrcInterface) */
    int f74;
    int f78;                            /* current step */
    int recordBuf;                      /* 0x7C: the record being read or written (saveRec_startWriteRecord / saveRec_startReadRecord) */
    int headerBuf;                      /* 0x80: the 12-byte record header buffer */
    int recordSize;                     /* 0x84 */
    struct sMcKey recordCrc;            /* 0x88: the record's CRC (saveRec_computeCrc) */
    int recordKey;                      /* 0x8C */
    int f90;
    int f94;
    int f98;
    struct sMcEnt *f9C;
    int heap;                           /* 0xA0 */
    int readOffset;                     /* 0xA4: saveRec_startReadRecord's read offset */
    char justCreated;                   /* 0xA8: set when a create finished; the next open then stops after its
                                           first seek (a just-created save has no records) */
};

extern struct sMcState *saveRec_session;

/**
 * Finds the @p n-th entry with key @p key in the open save's record table (saveRec_session +0x9C, +0x94 entries).
 *
 * @param key  record key
 * @param n  index among the entries with that key
 * @return the entry (key, offset, size, CRC), or 0
 */
struct sMcEnt *saveRec_findTableEntry(int key, int n) {
    struct sMcState *state = saveRec_session;
    struct sMcEnt *end = state->f9C + state->f94;
    struct sMcEnt *entry = state->f9C;
    int cnt = 0;
    for (; entry < end; entry++) {
        if (key == entry->key) {
            if (n == cnt) return entry;
            cnt++;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_setEntryCrc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

/**
 * Sets the CRC of the @p n-th record-table entry with key @p key.
 *
 * @param key  record key
 * @param n  index among the entries with that key
 * @param val  the CRC
 */
void saveRec_setEntryCrc(int key, int n, int *val) {
    struct sMcState *state = saveRec_session;
    struct sMcEnt *end = state->f9C + state->f94;
    struct sMcEnt *entry = state->f9C;
    int cnt = 0;
    for (; entry < end; entry++) {
        if (key == entry->key) {
            if (n == cnt) entry->c = *val;
            cnt++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_appendTableEntry);
#ifdef SKIP_ASM
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
#include "fakematch.h"
extern struct sMcState *saveRec_session;

/**
 * Appends an entry to the open save's record table.
 *
 * The entry gets the next file offset (+0x98; 0 for the first), @p key, @p size and the CRC; the next offset moves past
 * its 12-byte header and data and is pre-stored in the following entry.
 *
 * @param key  record key
 * @param size  record size
 * @param val  record CRC
 * @return the entry's file offset, or the unchanged next offset when the table is full
 */
// FAKEMATCH: FM_OPAQUE(e) after the e->c store hides that e == t, so the pre-if `t = &f9C[f94]` (v0) is copied into e (a3) at the join as in retail; e->c is stored through an int view (retail reloads saveRec_session after that store, which a struct-field store lets sched hoist); plain form 67.84% (91.38% with the struct-field store).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int saveRec_appendTableEntry(int key, int size, int *val) {
    struct sMcEnt *e, *newEntry;
    int len;
    int end;
    if (saveRec_session->f94 == saveRec_session->f90) return saveRec_session->f98;
    newEntry = saveRec_session->f9C + saveRec_session->f94;
    if (saveRec_session->f94 == 0) saveRec_session->f9C->a = 0;
    e = newEntry;
    e->key = key;
    e->b = size;
    len = size + 0xC;
    if (len == 0) len = 1;
    *(int *)&e->c = *val;
    FM_OPAQUE(e);
    saveRec_session->f94++;
    end = e->a + len;
    saveRec_session->f98 = end;
    if (saveRec_session->f94 != saveRec_session->f90) e[1].a = end;
    return e->a;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_computeCrc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int func_003FB840c(int) __asm__("saveRec_offsetCrcError");
int saveRec_computeCrc_impl(int a, int b, int c) __asm__("saveRec_computeCrc");
/**
 * Computes the CRC-32 of a buffer through the CRC interface (+0x70): reset, update, finish.
 *
 * @param data  the data
 * @param byteCount  byte count
 * @param digestOut  receives the output block (size, then a pointer to the digest)
 * @return 0, or the CRC error + 300
 */
int saveRec_computeCrc_impl(int data, int byteCount, int digestOut) {
    struct sMcIfB *vt = saveRec_session->crcIface;
    int result = vt->reset(data, byteCount, digestOut);
    if (result == 0) {
        result = vt->update(data, byteCount);
        if (result == 0) {
            result = vt->finish(digestOut);
            if (result == 0) return 0;
        }
    }
    return func_003FB840c(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_computeTableCrc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_computeCrc(void *, int, sDigestBlock **);
/**
 * Computes the CRC-32 of the open save's record table (16 bytes per entry) into @p out; 0 when the table is empty.
 *
 * @param out  receives the 4 digest bytes
 */
void saveRec_computeTableCrc(struct sMcKey *out) {
    struct sMcState *state = saveRec_session;
    sDigestBlock *digestBlock;
    int sz = state->f94 << 4;
    if (sz == 0) {
        *(int *)out = 0;
    } else {
        saveRec_computeCrc(state->f9C, sz, &digestBlock);
        *out = *(struct sMcKey *)digestBlock->digest;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_encrypt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int fB828b(int) __asm__("saveRec_offsetCipherError");
int saveRec_encrypt_impl(int x, int y) __asm__("saveRec_encrypt");
/**
 * Encrypts a buffer with the save cipher (+0x6C): sets the stored key (+0x64, +0x68), encrypts, rewinds. Does nothing
 * without a cipher.
 *
 * @param x  the data
 * @param y  byte count
 * @return 0, or the cipher error + 200
 */
int saveRec_encrypt_impl(int x, int y) {
    int result;
    if (saveRec_session->cipher == 0) return 0;
    result = saveRec_session->cipher->setKey(saveRec_session->cipherKey, saveRec_session->cipherKeyLen);
    if (result == 0) {
        result = saveRec_session->cipher->encrypt(x, y);
        if (result == 0) {
            result = saveRec_session->cipher->rewind();
            if (result == 0) return 0;
        }
    }
    return fB828b(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_decrypt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int fB828(int) __asm__("saveRec_offsetCipherError");
int saveRec_decrypt_impl(int x, int y) __asm__("saveRec_decrypt");
/**
 * Decrypts a buffer with the save cipher: as saveRec_encrypt with the decrypt slot.
 *
 * @param x  the data
 * @param y  byte count
 * @return 0, or the cipher error + 200
 */
int saveRec_decrypt_impl(int x, int y) {
    int result;
    if (saveRec_session->cipher == 0) return 0;
    result = saveRec_session->cipher->setKey(saveRec_session->cipherKey, saveRec_session->cipherKeyLen);
    if (result == 0) {
        result = saveRec_session->cipher->decrypt(x, y);
        if (result == 0) {
            result = saveRec_session->cipher->rewind();
            if (result == 0) return 0;
        }
    }
    return fB828(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_beginOpen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int mcReq_startOpen(int, int, int);
/**
 * Starts opening a save (mcReq_startOpen) as save-record operation @p operation at step 4, with an empty record table.
 *
 * @param saveName  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @param operation  the operation (4 open, 5 reopen and verify)
 * @return 0, or the request error + 100
 */
int saveRec_beginOpen(int saveName, int unitCode, int searchDir, int operation) {
    int result = mcReq_startOpen(saveName, unitCode, searchDir);
    struct sMcState *state = saveRec_session;
    state->f74 = operation;
    state->f78 = 4;
    state->f94 = 0;
    state->f98 = 0;
    return saveRec_offsetCardError(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_seekToStart);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int mcReq_startSeek(struct sMcState *, int, int);
extern int saveRec_offsetCardError(int);
/**
 * Save-record step 9: seeks to the start of the open save file to read its record headers.
 *
 * @return 0, or the request error + 100
 */
int saveRec_seekToStart(void) {
    struct sMcState *state = saveRec_session;
    state->f78 = 9;
    return saveRec_offsetCardError(mcReq_startSeek(state, 0, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_readNextHeader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_getBlockSize_BDF0(int, int) __asm__("saveRec_getBlockSize");
extern int mcReq_startRead(struct sMcState *, int, int);
extern int saveRec_offsetCardError(int);
/**
 * Save-record step 7: reads the next 12-byte record header of the open file into the header buffer (+0x80).
 *
 * @return 0, or the request error + 100
 */
int saveRec_readNextHeader(void) {
    struct sMcState *state = saveRec_session;
    state->f78 = 7;
    return saveRec_offsetCardError(mcReq_startRead(saveRec_session, state->headerBuf, saveRec_getBlockSize_BDF0(0, 1)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", saveRec_addReadHeader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_offsetCardError(int);

extern int fBCD8(int, int) __asm__("saveRec_decrypt");
extern void fB9D0(int, int *, int *, char *) __asm__("saveRec_splitHeader");
extern int fBAD0(int, int, char *) __asm__("saveRec_appendTableEntry");
extern int f9C48(struct sMcState *, int, int) __asm__("mcReq_startSeek");
/**
 * Save-record step 11: decrypts the record header just read, adds the record to the table (saveRec_appendTableEntry) and seeks
 * past its data to the next header.
 *
 * @return 0, the cipher or request error, or 5 when the table is full
 */
int saveRec_addReadHeader(void) {
    int x[2];
    int result;
    int recordKey, recordSize;
    x[1] = 0;
    saveRec_session->f78 = 11;
    result = fBCD8(saveRec_session->headerBuf, 0xC);
    if (result != 0) return result;
    fB9D0(saveRec_session->headerBuf, &x[0], &x[1], saveRec_session->recordCrc.b);
    recordSize = x[1];
    recordKey = x[0];
    result = fBAD0(recordKey, recordSize, saveRec_session->recordCrc.b);
    if (saveRec_session->f98 == result) return 5;
    return saveRec_offsetCardError(f9C48(saveRec_session, saveRec_session->f98, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_writeRecord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_getBlockSize_BED0(int, int) __asm__("saveRec_getBlockSize");

extern int saveRec_offsetCardError(int);

extern int saveRec_getHeader(int, int);
extern void fB958(int, int, int, char *) __asm__("saveRec_fillHeader");
extern int fBC40(int, int) __asm__("saveRec_encrypt");
extern int f9F00(struct sMcState *, int, int) __asm__("mcReq_startWrite");
/**
 * Save-record step 6: writes a record.
 *
 * Fills the header in front of the record buffer (+0x7C; 4 when its sentinel is gone) with the key, size and CRC,
 * encrypts the header and the data, and writes both (mcReq_startWrite).
 *
 * @return 0, the cipher error, or the request error + 100
 */
int saveRec_writeRecord(void) {
    int header;
    int result;
    saveRec_session->f78 = 6;
    header = saveRec_getHeader(saveRec_session->recordBuf, 1);
    if (header == 0) return 4;
    fB958(header, saveRec_session->recordKey, saveRec_session->recordSize, saveRec_session->recordCrc.b);
    result = fBC40(header, 0xC);
    if (result != 0) return result;
    result = fBC40(saveRec_session->recordBuf, saveRec_session->recordSize);
    if (result != 0) return result;
    result = saveRec_getBlockSize_BED0(saveRec_session->recordSize, 2);
    return saveRec_offsetCardError(f9F00(saveRec_session, header, result));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_readRecord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_getHeader(int, int);

extern int saveRec_getBlockSize_BF80(int, int) __asm__("saveRec_getBlockSize");
extern int mcReq_startRead(struct sMcState *, int, int);
extern int saveRec_offsetCardError(int);
/**
 * Save-record step 10: reads a record's header and data into the header in front of the record buffer.
 *
 * @return 0, or the request error + 100
 */
int saveRec_readRecord(void) {
    struct sMcState *state = saveRec_session;
    int header;
    state->f78 = 10;
    header = saveRec_getHeader(state->recordBuf, 0);
    return saveRec_offsetCardError(mcReq_startRead(saveRec_session, header, saveRec_getBlockSize_BF80(saveRec_session->recordSize, 1)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_verifyRecord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

typedef struct { int tag; int p; } P3FBFE0;
typedef struct { int a; int b; P3FBFE0 *p; } L3FBFE0;
extern int saveRec_getHeader(int, int);
extern int fBCD8(int, int) __asm__("saveRec_decrypt");
extern void fB9D0(int, int *, int *, char *) __asm__("saveRec_splitHeader");
extern int fBB50(int, int, P3FBFE0 **) __asm__("saveRec_computeCrc");
extern int f5FC8(int, char *, int) __asm__("memcmp");
/**
 * Verifies a record just read: decrypts its header, checks the key (11) and size (110), decrypts the data and compares
 * its CRC-32 with the header's.
 *
 * @return 0 when the record is valid, 10 when the CRC differs, 11 or 110, or the cipher / CRC error
 */
int saveRec_verifyRecord(void) {
    L3FBFE0 x; int header; int result; int err = 10; int hh;
    x.p = 0; hh = saveRec_session->recordBuf; x.b = 0;
    saveRec_session->f78 = err;
    header = saveRec_getHeader(hh, 0);
    result = fBCD8(header, 0xC);
    if (result != 0) return result;
    fB9D0(header, &x.a, &x.b, saveRec_session->recordCrc.b);
    if (x.a != saveRec_session->recordKey) return 11;
    if (x.b != saveRec_session->recordSize) return 110;
    result = fBCD8(saveRec_session->recordBuf, x.b);
    if (result != 0) return result;
    result = fBB50(saveRec_session->recordBuf, saveRec_session->recordSize, &x.p);
    if (result != 0) return result;
    if (x.p->tag != 4) return 11;
    if (f5FC8(x.p->p, saveRec_session->recordCrc.b, 4) == 0) err = 0;
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_isOpen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

/**
 * Returns whether the save-record manager saveRec_session is open.
 *
 * @return 1 when open
 */
int saveRec_isOpen(void) { return saveRec_session != 0; }
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", saveRec_open);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

/* the open parameters (names ours, from [U] below) */
typedef struct {
    int maxRecords;                     /* 0x00: record table size */
    int units;                          /* 0x04: the card unit list */
    int callbacks;                      /* 0x08: the 17 card callbacks */
    char *cipherKey;                    /* 0x0C */
    int cipherKeyLen;                   /* 0x10 */
    struct sMcIfA *cipher;              /* 0x14: may be 0 */
    int heap;                           /* 0x18 */
} P0E8;
extern int saveRec_isOpen(void);
extern int mcReq_open(int, int, int);
extern int saveRec_offsetCardError(int);
extern int func_003FB840c(int) __asm__("saveRec_offsetCrcError");
extern int fB828(int) __asm__("saveRec_offsetCipherError");
extern void *c0e8_6728(int, int, int) __asm__("TibExtMemAlloc");
extern void c0e8_A6C8(struct sMcIfB **) __asm__("mcReq_getCrcInterface");
extern int saveRec_allocBuffer(char *, int, int, int);
extern void func_0041605C(char *, char *, int);
/**
 * Opens the save-record manager.
 *
 * @p openParams holds: +0x0 the record limit, +0x4 the card unit list, +0x8 the 17 card callbacks, +0xC / +0x10 the cipher key
 * and its length, +0x14 the cipher interface (may be 0), +0x18 the heap. Opens the request layer (mcReq_open),
 * allocates the 172-byte state, starts the CRC interface and the cipher, copies the key, allocates the record table (16
 * bytes per record) and the 12-byte header buffer (+0x80). Likely bug: the header buffer is allocated from +0xA0 before
 * +0xA0 is set to the heap.
 *
 * @param openParams  the open parameters
 * @return 0; 1 when already open, 6 when an allocation fails, or a layer error offset by 100 / 200 / 300
 */
int saveRec_open(P0E8 *openParams) {
    int result;
    struct sMcIfA *cb;
    if (saveRec_isOpen() != 0) return 1;
    result = mcReq_open(openParams->units, openParams->callbacks, openParams->heap);
    if (result != 0) return saveRec_offsetCardError(result);
    saveRec_session = c0e8_6728(openParams->heap, 172, 4);
    if (saveRec_session == 0) return 6;
    c0e8_A6C8(&saveRec_session->crcIface);
    result = saveRec_session->crcIface->start(openParams->heap);
    if (result != 0) return func_003FB840c(result);
    saveRec_session->cipher = cb = openParams->cipher;
    if (cb != 0) {
        int n;
        result = cb->start(openParams->heap);
        if (result != 0) return fB828(result);
        n = openParams->cipherKeyLen;
        saveRec_session->cipherKeyLen = n;
        saveRec_session->cipherKey = c0e8_6728(openParams->heap, n, 4);
        if (saveRec_session->cipherKey == 0) return 6;
        func_0041605C(saveRec_session->cipherKey, openParams->cipherKey, saveRec_session->cipherKeyLen);
    }
    saveRec_session->f90 = openParams->maxRecords;
    saveRec_session->f94 = 0;
    saveRec_session->f98 = 0;
    saveRec_session->f9C = c0e8_6728(openParams->heap, openParams->maxRecords << 4, 4);
    if (saveRec_session->f9C == 0) return 6;
    result = saveRec_allocBuffer((char *)&saveRec_session->headerBuf, saveRec_session->heap, 0, 1);
    if (result != 0) return result;
    saveRec_session->headerBuf -= 0xC;
    saveRec_session->heap = openParams->heap;
    saveRec_session->justCreated = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_close);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_freeBuffer(int, int, int, int);

// 99.47: only $2/$3 swap between D ptr and q (f6C) in blocks 3,7 (target D in $2). tried decl order sweep, q vs int v, permuter 4min nothing. Names prefixed c260_ in file to avoid clashes
extern int saveRec_isOpen(void);
extern void c260_B9A0(int) __asm__("saveRec_restoreSentinel");
extern void c260_6760(int, void *, int, int) __asm__("TibExtMemFree");
extern int c260_B828(int) __asm__("saveRec_offsetCipherError");
extern int c260_B840(int) __asm__("saveRec_offsetCrcError");
extern int c260_B810(int, struct sMcState *) __asm__("saveRec_offsetCardError");
extern int c260_9530(void) __asm__("mcReq_close");
/**
 * Closes the save-record manager: frees the header buffer, the record table and the key, stops the cipher and the CRC
 * interface, closes the request layer (mcReq_close) and frees the state.
 *
 * @return 0; 2 when not open, or a layer error offset by 100 / 200 / 300
 */
int saveRec_close(void) {
    int result;
    int userBuf;
    struct sMcIfA *cipher;
    if (saveRec_isOpen() == 0) return 2;
    c260_B9A0(saveRec_session->headerBuf);
    userBuf = saveRec_session->headerBuf + 0xC;
    saveRec_session->headerBuf = userBuf;
    result = saveRec_freeBuffer(userBuf, saveRec_session->heap, 0, 1);
    if (result != 0) return result;
    c260_6760(saveRec_session->heap, saveRec_session->f9C, saveRec_session->f90 * 16, 4);
    cipher = saveRec_session->cipher;
    saveRec_session->f9C = 0;
    if (cipher != 0) {
        result = cipher->stop();
        if (result != 0) return c260_B828(result);
        c260_6760(saveRec_session->heap, saveRec_session->cipherKey, saveRec_session->cipherKeyLen, 4);
        saveRec_session->cipherKey = 0;
    }
    saveRec_session->cipher = 0;
    result = saveRec_session->crcIface->stop();
    if (result != 0) return c260_B840(result);
    saveRec_session->crcIface = 0;
    result = c260_9530();
    if (result != 0) return c260_B810(result, saveRec_session);
    c260_6760(saveRec_session->heap, saveRec_session, 172, 4);
    saveRec_session = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_startCreate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int mcReq_startCreate(char *str, int a, int b);
extern int saveRec_offsetCardError(int);
/**
 * Starts creating a save (save-record operation 1, mcReq_startCreate).
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @return 0, or the request error + 100
 */
int saveRec_startCreate(char *str, int unitCode, int searchDir) {
    struct sMcState *state = saveRec_session;
    state->f74 = 1;
    state->f78 = 1;
    return saveRec_offsetCardError(mcReq_startCreate(str, unitCode, searchDir));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_startDelete);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int mcReq_startDelete(char *str, int a, int b);
extern int saveRec_offsetCardError(int);
/**
 * Starts deleting a save (save-record operation 2, mcReq_startDelete).
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @return 0, or the request error + 100
 */
int saveRec_startDelete(char *str, int unitCode, int searchDir) {
    struct sMcState *state = saveRec_session;
    state->f74 = 2;
    state->f78 = 2;
    return saveRec_offsetCardError(mcReq_startDelete(str, unitCode, searchDir));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_startReopen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int fBD70(char *, int, int, int) __asm__("saveRec_beginOpen");
int saveRec_startReopen_impl(struct sMcParams *a) __asm__("saveRec_startReopen");
/**
 * Starts reopening a save from parameters kept from an earlier open (operation 5): copies them, opens the save by its
 * name (+0x48) on its unit (+0x4); when the headers have been read, the record table's CRC must equal the kept one
 * (+0x60).
 *
 * @param savedParams  the kept 100-byte save parameters
 * @return 0, or the request error + 100
 */
int saveRec_startReopen_impl(struct sMcParams *savedParams) {
    saveRec_session->params = *savedParams;
    return fBD70(savedParams->name, savedParams->w[1], -1, 5);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", saveRec_startOpen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int saveRec_beginOpen(int, int, int, int);
/**
 * Starts opening a save and reading its record table (save-record operation 4, saveRec_beginOpen).
 *
 * @param saveName  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @return 0, or the request error + 100
 */
int saveRec_startOpen(int saveName, int unitCode, int searchDir) { return saveRec_beginOpen(saveName, unitCode, searchDir, 4); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_startClose);
#ifdef SKIP_ASM
// FAKEMATCH: `*&` store of params.crc through its address (the target reloads saveRec_session); plain form 94.26%.
// Byte-identical and behaves like the plain form; landed marked under the 2026-11-14 ruling (portable,
// behaviour-preserving steering only); replace it when an honest spelling matches.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *saveRec_session;

extern int mcReq_startClose(struct sMcParams *);
extern int saveRec_offsetCardError(int);
extern void func_003FBBE0b(int *) __asm__("saveRec_computeTableCrc");
/**
 * Starts closing an open save (save-record operation 3): stores the record table's CRC in the caller's parameters
 * (+0x60) and the manager's, clears the table and closes the file (mcReq_startClose).
 *
 * @param x  the save parameters
 * @return 0, or the request error + 100
 */
int saveRec_startClose(struct sMcParams *x) {
    struct sMcState *state;
    func_003FBBE0b(&x->crc);
    /* stored through a pointer (*&), not as a plain field store: the target reloads saveRec_session after this store */
    *&saveRec_session->params.crc = x->crc;
    state = saveRec_session;
    state->f74 = 3;
    state->f78 = 3;
    state->f98 = 0;
    state->f94 = 0;
    return saveRec_offsetCardError(mcReq_startClose(x));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", saveRec_startWriteRecord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_offsetCardError(int);

// (prototype from their definition at line 3805)
struct sMcEnt *saveRec_findTableEntry(int key, int n);

/* sMcParams as this function's callers pass it: 8-byte aligned (the copy into the manager uses doubleword loads). */
typedef struct {
    int w[18];                          /* 0x00 */
    char name[24];                      /* 0x48 */
    int crc;                            /* 0x60 */
} __attribute__((aligned(8))) sMcParamsAligned;
extern int fBB50w(int, int, sDigestBlock **) __asm__("saveRec_computeCrc");
extern int fBAD0w(int, int, void *) __asm__("saveRec_appendTableEntry");
extern void fBA78w(int, int, void *) __asm__("saveRec_setEntryCrc");
extern void fBBE0w(int *) __asm__("saveRec_computeTableCrc");
extern int f9C48w(void *, int, int) __asm__("mcReq_startSeek");
/**
 * Starts writing a record to an open save (save-record operation 6).
 *
 * Copies the save parameters, computes the data's CRC and either updates the @p recordIndex-th record with key @p recordKey or appends a
 * new one at the end of the file; recomputes the table CRC into @p saveParams +0x60 and seeks to the record's offset.
 * saveRec_writeRecord then writes it. Quirk: when the CRC computation fails it reaches its end without a return value.
 *
 * @param saveParams  the open save's parameters
 * @param recordKey  record key
 * @param recordIndex  index among the records with that key
 * @param recordBuf  the record data (a buffer from saveRec_allocBuffer)
 * @param recordSize  data size
 * @return 0, or the request error + 100
 */
int saveRec_startWriteRecord(sMcParamsAligned *saveParams, int recordKey, int recordIndex, int recordBuf, int recordSize) {
    sDigestBlock *digestBlock[4];
    int writeOffset;
    struct sMcEnt *k;
    digestBlock[0] = 0;
    saveRec_session->f74 = 6;
    saveRec_session->f78 = 5;
    __builtin_memcpy(saveRec_session, saveParams, 100);
    saveRec_session->recordSize = recordSize;
    saveRec_session->recordBuf = recordBuf;
    saveRec_session->recordKey = recordKey;
    k = saveRec_findTableEntry(recordKey, recordIndex);
    if (fBB50w(saveRec_session->recordBuf, saveRec_session->recordSize, digestBlock) == 0) {
        saveRec_session->recordCrc = *(struct sMcKey *)digestBlock[0]->digest;
        if (k == 0) {
            writeOffset = saveRec_session->f98;
            fBAD0w(recordKey, saveRec_session->recordSize, &saveRec_session->recordCrc);
        } else {
            writeOffset = k->a;
            fBA78w(recordKey, recordIndex, &saveRec_session->recordCrc);
        }
        fBBE0w(&saveParams->crc);
        return saveRec_offsetCardError(f9C48w(saveParams, writeOffset, 0));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_startReadRecord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_offsetCardError(int);

// (prototype from their definition at line 3805)
struct sMcEnt *saveRec_findTableEntry(int key, int n);

extern int c708_9C48(void *, int, int) __asm__("mcReq_startSeek");
int saveRec_startReadRecord_impl(struct sMcParams *src, int a, int b, int c, int d) __asm__("saveRec_startReadRecord");
/**
 * Starts reading a record of an open save (save-record operation 8): finds the @p recordIndex-th record with key @p recordKey and seeks
 * to it. saveRec_readRecord then reads it and saveRec_verifyRecord checks it.
 *
 * @param src  the open save's parameters
 * @param recordKey  record key
 * @param recordIndex  index among the records with that key
 * @param recordBuf  the destination (a buffer from saveRec_allocBuffer)
 * @param recordSize  expected data size
 * @return 0; 7 when there is no such record, 110 when its size differs, or the request error + 100
 */
int saveRec_startReadRecord_impl(struct sMcParams *src, int recordKey, int recordIndex, int recordBuf, int recordSize) {
    struct sMcState *state = saveRec_session;
    struct sMcEnt *entry;
    int readOffset;
    state->f74 = 8;
    state->f78 = 5;
    state->params = *src;
    saveRec_session->recordBuf = recordBuf;
    saveRec_session->recordKey = recordKey;
    saveRec_session->recordSize = recordSize;
    entry = saveRec_findTableEntry(recordKey, recordIndex);
    if (entry == 0) return 7;
    if (recordSize != entry->b) return 110;
    readOffset = entry->a;
    saveRec_session->readOffset = readOffset;
    return saveRec_offsetCardError(c708_9C48(src, readOffset, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_countRecords);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_isOpen(void);
int saveRec_countRecords_impl(int a, int key, int *out) __asm__("saveRec_countRecords");
/**
 * Counts the records with key @p key in the open save.
 *
 * @param saveParams  the save parameters (only checked for 0)
 * @param key  record key
 * @param out  receives the count
 * @return 0; 2 when the manager is not open, 4 for a 0 argument
 */
int saveRec_countRecords_impl(int saveParams, int key, int *out) {
    struct sMcState *state;
    struct sMcEnt *entry, *end;
    if (!saveRec_isOpen()) return 2;
    if (saveParams == 0) return 4;
    if (out == 0) return 4;
    *out = 0;
    state = saveRec_session;
    end = state->f9C + state->f94;
    entry = state->f9C;
    for (; entry < end; entry++) {
        if (key == entry->key) *out = *out + 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_getRecordSize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
// (prototype from their definition at line 3805)
struct sMcEnt *saveRec_findTableEntry(int key, int n);

extern int saveRec_isOpen(void);
int saveRec_getRecordSize_impl(int a, int b, int c, int *out) __asm__("saveRec_getRecordSize");
/**
 * Returns the size of the @p recordIndex-th record with key @p recordKey in the open save.
 *
 * @param saveParams  the save parameters (only checked for 0)
 * @param recordKey  record key
 * @param recordIndex  index among the records with that key
 * @param out  receives the size (0 when not found)
 * @return 0; 7 when not found, 2 when the manager is not open, 4 for a 0 argument
 */
int saveRec_getRecordSize_impl(int saveParams, int recordKey, int recordIndex, int *out) {
    int err = 0;
    struct sMcEnt *entry;
    if (!saveRec_isOpen()) return 2;
    if (saveParams == 0) return 4;
    if (out == 0) return 4;
    *out = 0;
    entry = saveRec_findTableEntry(recordKey, recordIndex);
    if (entry) *out = entry->b;
    else err = 7;
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_copySaveParams);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

int saveRec_copySaveParams_impl(struct sMcParams *dst) __asm__("saveRec_copySaveParams");
/**
 * Copies the open save's 100-byte parameters (request body and table CRC) to @p dst.
 *
 * @param dst  destination
 * @return 0, or 4 when @p dst is 0
 */
int saveRec_copySaveParams_impl(struct sMcParams *dst) {
    if (dst == 0) return 4;
    *dst = saveRec_session->params;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", saveRec_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* saveRec_session: the memory-card session (0xAC bytes, allocated by saveRec_open). params is the caller's 0x64-byte
   block; f9C points to f94 entries; cipher / crcIface are the two driver interfaces. */
extern struct sMcState *saveRec_session;

extern int saveRec_offsetCardError(int);
extern int saveRec_offsetCipherError(int);
extern int saveRec_decrypt(int, int);
extern void fCA40_B9A0(int) __asm__("saveRec_restoreSentinel");
extern int fCA40_5FC8(void *, void *, int) __asm__("memcmp");

/**
 * Polls the save-record manager and runs the next step of its operation.
 *
 * Polls the request layer (mcReq_update). Write: after the seek writes the record (saveRec_writeRecord); after the write
 * checks the count, decrypts the data back and recomputes the table CRC. Read: after the seek reads the record
 * (saveRec_readRecord); after the read checks the count and verifies it (saveRec_verifyRecord). Open / reopen: after the open seeks
 * to the start; then reads header after header (saveRec_readNextHeader, saveRec_addReadHeader) until the file or the table ends (a
 * just-created save has none), and finishes with the table CRC (reopen: 9 when it differs from the kept one). Other
 * operations finish when the request does.
 *
 * @param st  receives 1 while running, 2 when finished
 * @param res  the request's result word; receives the record size after a read or write
 * @return 0 or an error code (request errors + 100)
 */
int saveRec_update(int *st, int *res) {
    int cardResult;
    int key;

    if (saveRec_isOpen() == 0) return 2;
    if (st == 0) return 4;
    if (res == 0) return 4;
    cardResult = mcReq_update(st, res);
    if (st[0] == 0) {
        if (saveRec_session->f74 != 0) return 8;
    }
    if (st[0] != 2) return saveRec_offsetCardError(cardResult);
    if (cardResult != 0 && saveRec_session->f74 != 4 && saveRec_session->f74 != 5) return saveRec_offsetCardError(cardResult);
    if (saveRec_session->f74 == 1 && cardResult == 0) saveRec_session->justCreated = 1;
    if (saveRec_session->f74 != 1 && saveRec_session->f74 != 4) saveRec_session->justCreated = 0;
    if (saveRec_session->f74 == 6) {
        if (saveRec_session->f78 == 5) {
            *st = 1;
            return saveRec_writeRecord();
        }
        if (saveRec_session->f78 == 6) {
            int header = saveRec_getHeader(saveRec_session->recordBuf, 0);
            int stepError;
            fCA40_B9A0(header);
            if (saveRec_getBlockSize(saveRec_session->recordSize, 2) != *res) return 110;
            *res = saveRec_session->recordSize;
            stepError = saveRec_decrypt(saveRec_session->recordBuf, saveRec_session->recordSize);
            if (stepError != 0) return saveRec_offsetCipherError(stepError);
            if (cardResult == 0) saveRec_computeTableCrc((struct sMcKey *)&saveRec_session->params.crc);
        }
    }
    if (saveRec_session->f74 == 8) {
        if (saveRec_session->f78 == 5) {
            *st = 1;
            return saveRec_readRecord();
        }
        if (saveRec_session->f78 == 10) {
            int header = saveRec_getHeader(saveRec_session->recordBuf, 0);
            int stepError;
            if (saveRec_getBlockSize(saveRec_session->recordSize, 1) == *res) {
                *res = saveRec_session->recordSize;
                stepError = saveRec_verifyRecord();
            } else {
                stepError = 110;
            }
            fCA40_B9A0(header);
            return stepError;
        }
    }
    if (saveRec_session->f74 == 4 || saveRec_session->f74 == 5) {
        if (saveRec_session->f78 == 4) {
            if (cardResult != 0) {
                saveRec_session->justCreated = 0;
                return saveRec_offsetCardError(cardResult);
            }
            mcReq_copyRequestBody((char *)saveRec_session);
            *st = 1;
            return saveRec_seekToStart();
        }
        if (saveRec_session->f78 == 9) {
            if (cardResult != 0) {
                saveRec_session->justCreated = 0;
                return saveRec_offsetCardError(cardResult);
            }
            if ((unsigned char)saveRec_session->justCreated != 0) {
                saveRec_session->justCreated = 0;
                *st = 2;
                return 0;
            }
            saveRec_session->justCreated = 0;
            *st = 1;
            return saveRec_readNextHeader();
        }
        if (saveRec_session->f78 == 7) {
            if (cardResult != 0 && cardResult != 10) {
                saveRec_session->justCreated = 0;
                return saveRec_offsetCardError(cardResult);
            }
            if (cardResult == 10 || saveRec_session->f94 == saveRec_session->f90) {
                *st = 2;
                fCA40_B9A0(saveRec_session->headerBuf);
                saveRec_computeTableCrc((struct sMcKey *)&key);
                if (saveRec_session->f74 == 5) return fCA40_5FC8(&key, &saveRec_session->params.crc, 4) != 0 ? 9 : 0;
                saveRec_session->params.crc = key;
                return 0;
            }
            *st = 1;
            return saveRec_addReadHeader();
        }
        if (cardResult == 0) {
            *st = 1;
            return saveRec_readNextHeader();
        }
    } else if (*st == 2) {
        saveRec_session->f74 = 0;
        saveRec_session->f78 = 0;
    }
    return saveRec_offsetCardError(cardResult);
}
#endif
