#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F31F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char *D_004A4B98;
extern void f2220(void *) __asm__("func_003F2220");
typedef struct { unsigned count; unsigned st[4]; unsigned char buf[64]; } sMd5;
/**
 * Finishes an MD5 hash and writes the digest as 16 raw bytes or as 32 lower-case hex characters.
 *
 * Pads the buffered block with 0x80 and zeros up to byte 56 (hashing a full block with func_003F2220 on the way),
 * stores the message length in bits (byte count << 3) little-endian in bytes 56..63 and hashes the last block. Then
 * walks the 16 digest bytes of the state words: @p n == 16 writes them raw; @p n > 32 writes two hex digits each
 * (digits from D_004A4B98) and a NUL. Likely bug: for any other positive @p n the zero-fill loop never decrements
 * @p n, so it writes zeros forever.
 *
 * @param ctx  the MD5 context
 * @param out  digest output
 * @param n  output size: 16 for raw bytes, more than 32 for hex text
 */
void func_003F31F8(sMd5 *ctx, unsigned char *out, int n) {
    unsigned char c = 0x80;
    unsigned w = 0;
    int i;
    i = ctx->count & 0x3F;
    do {
        ctx->buf[i] = c;
        i++;
        c = 0;
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
            *out = D_004A4B98[(w >> 4) & 0xF];
            out++;
            *out = D_004A4B98[w & 0xF];
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

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F33B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* Globals used by several functions. */
extern unsigned D_00496C08[];

extern char D_0051EEF8[];

extern int D_004A4B9C;
extern char str_hello_world[];
/**
 * Keys a CRC-mixed byte-permutation stream cipher state (0x108 bytes: 256-byte permutation, index byte at +0x100, CRC
 * word at +0x104).
 *
 * On first use it also keys the shared state D_0051EEF8 with "hello world", 10 rounds. A non-negative @p rounds resets
 * the state to the identity permutation with index and CRC 0; a negative one keeps the current state and uses -@p
 * rounds. Then, for rounds * 256 steps, it mixes the next key byte and the current permutation byte into a CRC-32
 * (table D_00496C08) and swaps the current slot with the slot the CRC's low byte names. A negative @p klen means
 * @p key is NUL-terminated; an empty key leaves the permutation as it is.
 *
 * @param cipherState  the cipher state
 * @param key  key bytes
 * @param klen  key length, or negative for a C string
 * @param rounds  number of 256-step passes; negative continues without resetting
 */
void func_003F33B0(unsigned char *cipherState, unsigned char *key, int klen, int rounds) {
    int i;
    int lim;
    unsigned crc;

    if (D_004A4B9C == 0) {
        D_004A4B9C = 1;
        func_003F33B0((unsigned char *)D_0051EEF8, (unsigned char *)str_hello_world, -1, 10);
    }
    i = 0xFF;
    if (rounds < 0) {
        rounds = -rounds;
    } else {
        unsigned char *permSlot = cipherState + 0xFF;
        for (; i >= 0; i--) *permSlot-- = i;
        cipherState[0x100] = 0;
        *(int *)(cipherState + 0x104) = 0;
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
                permSlot = cipherState + (i & 0xFF);
                swapByte = *permSlot;
                i++;
                crc = (crc >> 8) ^ D_00496C08[(crc & 0xFF) ^ key[keyIndex]];
                crc = (crc >> 8) ^ D_00496C08[(crc & 0xFF) ^ swapByte];
                swapSlot = cipherState + (crc & 0xFF);
                *permSlot = *swapSlot;
                *swapSlot = swapByte;
            } while (i < lim);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F3530);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* Globals used by several functions. */
extern unsigned D_00496C08[];

/**
 * XORs @p n bytes of a buffer with the keystream of the cipher state keyed by func_003F33B0.
 *
 * Per byte: advances the index (+0x100), folds the permutation byte there into the CRC word (+0x104) with the CRC-32
 * table D_00496C08, swaps the permutation bytes at the index and at the CRC's low byte, and XORs the buffer byte with
 * the permutation byte at the difference of the two swapped values. Saves the index and CRC back.
 *
 * @param cipherState  the cipher state
 * @param buf  bytes to encrypt or decrypt in place
 * @param n  byte count
 */
void func_003F3530(unsigned char *cipherState, unsigned char *buf, int n) {
    unsigned char i = cipherState[0x100];
    unsigned j = *(unsigned *)(cipherState + 0x104);
    for (; n > 0; n--) {
        unsigned char valueAtIndex, valueAtCrc;
        j = (j >> 8) ^ D_00496C08[(j & 0xFF) ^ cipherState[i++]];
        valueAtIndex = cipherState[i];
        valueAtCrc = cipherState[j & 0xFF];
        cipherState[i] = valueAtCrc;
        cipherState[j & 0xFF] = valueAtIndex;
        *buf++ ^= cipherState[(unsigned char)(valueAtIndex - valueAtCrc)];
    }
    cipherState[0x100] = i;
    *(unsigned *)(cipherState + 0x104) = j;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F35D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_0051EEF8[];

extern void f33B0w(void *, int, int, int) __asm__("func_003F33B0");
extern void f3530w(void *, unsigned char *, int) __asm__("func_003F3530");
extern char str_ru_paranoid[];
/**
 * Encrypts a text into a fixed-length printable string (the inverse of func_003F3748).
 *
 * Keys a local cipher state with the key (func_003F33B0), and also stirs the key and "ru paranoid?" into the shared
 * state D_0051EEF8. Writes @p n - 1 characters and a NUL: each input character (anything outside 0x20..0x7E becomes
 * 0x7F, which also marks the end of the input) is shifted by a keystream byte mod 96 into the range 0x20..0x7F; after
 * the input's NUL the rest is random padding drawn from the shared state, so the output length does not reveal the
 * text's. func_003E8B10 uses it to send the PASS field as "~" + this encoding under the 16-byte session key.
 *
 * @param out  output buffer
 * @param n  output buffer size
 * @param in  text to encrypt (may equal @p out)
 * @param cipherKey  key bytes
 * @param cipherKeyLen  key length, or negative for a C string
 * @param cipherRounds  key rounds
 * @return 0
 */
int func_003F35D0(char *out, int n, unsigned char *in, int cipherKey, int cipherKeyLen, int cipherRounds) {
    unsigned char ctx[272];
    unsigned char ch;
    unsigned char k;
    k = 0;
    f33B0w(ctx, cipherKey, cipherKeyLen, cipherRounds);
    f33B0w(D_0051EEF8, cipherKey, cipherKeyLen, -cipherRounds);
    f33B0w(D_0051EEF8, (int)str_ru_paranoid, -1, -1);
    for (; n > 1; n--) {
        if (in == 0) {
            f3530w(D_0051EEF8, &ch, 1);
            ch = (ch & 0x3F) + 0x20;
        } else {
            ch = *in;
            in++;
            if (ch == 0) in = 0;
        }
        if ((unsigned)(ch - 0x20) > 94) ch = 0x7F;
        f3530w(ctx, &k, 1);
        *out = (ch + 0x40 + (int)(k % 0x60U)) % 0x60 + 0x20;
        out++;
    }
    if (n > 0) *out = 0;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F3748);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void f33B0(void *, int, int, int) __asm__("func_003F33B0");
extern void f3530(void *, unsigned char *, int) __asm__("func_003F3530");
/**
 * Decrypts a string made by func_003F35D0.
 *
 * Keys a local cipher state (func_003F33B0) and shifts each input character back by a keystream byte mod 96, stopping
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
int func_003F3748(char *out, int n, unsigned char *in, int key, int keyLen, int rounds) {
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
extern char D_0051F000[];
extern int func_003F4C98(char *, int, int);
extern int f4C68g(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
/**
 * Polls a host-name lookup made by func_003F39C8 (its "done" function, slot +0x8).
 *
 * Asks the IOP 'name'/'done' for the lookup id at +0x58 and waits for the answer into +0x0; when that is positive
 * (resolved) it also fetches 'name'/'addr' into +0x4.
 *
 * @return 0 while pending, positive when resolved, negative on failure
 */
int _SocketLookupDone(int *self) {
    int id = self[88 / 4];
    func_003F4C98(D_0051F000, 'name', 'done');
    f4C68g(D_0051F000, 1, id, 0, 0, 0, 0, 0, 1, self);
    if (self[0] > 0) {
        func_003F4C98(D_0051F000, 'name', 'addr');
        f4C68g(D_0051F000, 1, id, 0, 0, 0, 0, 0, 1, self + 1);
    }
    return self[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", _SocketLookupFree);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_0051F010[];
extern int func_003F4C98(char *, int, int);
extern void RpcFree(int *);
/**
 * Frees a host-name lookup made by func_003F39C8 (its "free" function, slot +0xC).
 *
 * Sends 'name'/'free' for the lookup id at +0x58 without waiting, then frees the object with RpcFree.
 */
void _SocketLookupFree(int *self) {
    int lookupId = self[88 / 4];
    func_003F4C98(D_0051F010, 'name', 'free');
    func_003F4C68(D_0051F010, 1, lookupId, 0, 0, 0, 0, 0, 0, 0);
    RpcFree(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F39C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_0051F020[];
extern int *RpcAlloc(int);
extern void f6210k(int *, int, int) __asm__("memset");
extern int func_003F4C98(char *, int, int);
extern int f4C68k(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
extern int _SocketLookupDone(int *);
extern void _SocketLookupFree(int *);
int *f39C8impl(int a, int b) __asm__("func_003F39C8");
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
    int f;
    int *lookup = RpcAlloc(92);
    if (lookup) {
        f6210k(lookup, 0, 92);
        f = (int)_SocketLookupDone;
        lookup[3] = (int)_SocketLookupFree;
        lookup[2] = f;
        func_003F4C98(D_0051F020, 'name', 'look');
        f = 1;
        f4C68k(D_0051F020, f, 0, timeout, hostName, -f, 0, 0, f, lookup + 88 / 4);
    }
    return lookup;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", SocketLookup);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F39C8(void);
/**
 * Starts a host-name lookup: forwards both arguments to func_003F39C8 and returns its lookup object.
 *
 * Used by BuddyApi's connect (func_003F08E8) and by LobbyMan; both poll the object's done function (slot +0x8) and free
 * it through slot +0xC.
 *
 * @param hostName  host name text
 * @param timeoutMs  timeout
 * @return the lookup object, or 0
 */
int SocketLookup(void* hostName, int timeoutMs) { return func_003F39C8(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F3AB8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F39C8(void);
/**
 * Resolves a host name synchronously: starts a lookup (func_003F39C8, arguments passed through), spins on its done
 * function until it is nonzero, frees it and returns the address.
 *
 * Nothing in retail calls it.
 *
 * @return the resolved address, or 0 when the lookup could not be started (also 0 when the lookup failed)
 */
int func_003F3AB8(void) {
    int *hostLookup;
    int resolvedAddr = 0;
    hostLookup = (int *)func_003F39C8();
    if (hostLookup) {
        while (((int (*)(int *))hostLookup[2])(hostLookup) == 0)
            ;
        resolvedAddr = hostLookup[1];
        ((void (*)(int *))hostLookup[3])(hostLookup);
    }
    return resolvedAddr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnStartup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BA0;

extern int D_004A4BA4;

extern int func_003F5470(void);
extern void f5000(int) __asm__("func_003F5000");
extern int func_003F4D68(void);
extern int func_003F4C98(char *, int, int);
extern int f4C68e(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
/**
 * Starts the IOP network module ('netw'/'crea') once.
 *
 * Returns -1 when it is already open (D_004A4BA0). If the IOP request queue is not running (func_003F5470) it starts it
 * (func_003F5000) and fetches the IOP's service directory (func_003F4D68). Then sends 'netw'/'crea' with the parameter
 * text and the argument '1731', waits for the module handle into D_004A4BA4 and sets D_004A4BA0. The cCommSystem
 * constructor calls it with "www.ea.com".
 *
 * @param startupParams  startup parameter text
 * @return 0, or -1 when already started
 */
int NetConnStartup(int startupParams) {
    char buf[16];
    if (D_004A4BA0 != 0) return -1;
    if (func_003F5470() <= 0) {
        f5000(0);
        func_003F4D68();
    }
    func_003F4C98(buf, 'netw', 'crea');
    f4C68e(buf, 1, 0, '1731', startupParams, -1, 0, 0, 1, &D_004A4BA4);
    D_004A4BA0 = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnQuery);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BA4;

extern int func_003F4C98(char *, int, int);
extern int func_003F4C68b(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
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
    func_003F4C98(buf, 'netw', 'list');
    func_003F4C68b(buf, 5, D_004A4BA4, 0, self, -1, configList, maxConfigs << 8, 1, &out);
    return out;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnConnect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: `if (a > 0)` around an identical copy of the body: the target has no compare; jump2 merges the arms and the leftover block boundary holds sched2 back; straight-line form 78.57%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int D_004A4BA4;

extern int func_003F4C98(char *, int, int);
extern int f4C68s(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
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
        func_003F4C98(buf, 'netw', 'star');
        f4C68s(buf, 1, D_004A4BA4, 0, configName, -1, 0, 0, 1, &out);
        return out;
    }
    func_003F4C98(buf, 'netw', 'star');
    f4C68s(buf, 1, D_004A4BA4, 0, configName, -1, 0, 0, 1, &out);
    return out;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnStatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BA0;

extern int D_004A4BA4;

extern int func_003F4C98(char *, int, int);
extern int f4C68m(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
/**
 * Answers a network-status query by its four-character selector.
 *
 * 'open' returns the started flag D_004A4BA0 without asking the IOP. The others send 'netw'/'stat' with a selector and
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
        return D_004A4BA0;
    }
    if (tag == 'onln') {
        func_003F4C98(buf, 'netw', 'stat');
        f4C68m(buf, 1, D_004A4BA4, 'host', 0, 0, 0, 0, 1, &statusReply);
        return statusReply != 0;
    }
    if (tag == 'conn') {
        func_003F4C98(buf, 'netw', 'stat');
        f4C68m(buf, 5, D_004A4BA4, 'conn', 0, 0, statusBuf, statusBufSize, 1, &statusReply);
        return statusReply;
    }
    if (tag == 'host') {
        func_003F4C98(buf, 'netw', 'stat');
        f4C68m(buf, 1, D_004A4BA4, 'host', 0, 0, 0, 0, 1, &addrReply);
        return addrReply;
    }
    if (tag == 'addr') {
        func_003F4C98(buf, 'netw', 'stat');
        f4C68m(buf, 1, D_004A4BA4, 'addr', 0, 0, 0, 0, 1, &addrReply);
        return addrReply;
    }
    if (tag == 'macx') {
        func_003F4C98(buf, 'netw', 'stat');
        func_003F4C68(buf, 5, D_004A4BA4, 'macx', 0, 0, statusBuf, statusBufSize, 1, 0);
        return 0;
    }
    if (tag == 'bbnd' || tag == 'type') {
        func_003F4C98(buf, 'netw', 'stat');
        f4C68m(buf, 1, D_004A4BA4, 'type', 0, 0, statusBuf, statusBufSize, 1, &statusReply);
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
extern char D_004512E8[];
extern const char str_0123456789abcdefUSBD_IRX[];
extern void f3CC0(int, unsigned char *, int) __asm__("NetConnStatus");
/**
 * Returns the console's MAC address as text: "$" followed by twelve lower-case hex digits.
 *
 * Built once into the static buffer D_004512E8 from NetConnStatus('macx'); later calls return the cached text. LobbyMan
 * sends it as the MID tag and cCommSystem copies it.
 *
 * @return the static MAC text
 */
char *NetConnMAC(void) {
    unsigned char macBytes[16];
    if (D_004512E8[0] == 0) {
        f3CC0(0x6d616378, macBytes, 0x10);
        D_004512E8[1] = str_0123456789abcdefUSBD_IRX[macBytes[0] >> 4];
        D_004512E8[2] = str_0123456789abcdefUSBD_IRX[macBytes[0] & 0xF];
        D_004512E8[3] = str_0123456789abcdefUSBD_IRX[macBytes[1] >> 4];
        D_004512E8[4] = str_0123456789abcdefUSBD_IRX[macBytes[1] & 0xF];
        D_004512E8[5] = str_0123456789abcdefUSBD_IRX[macBytes[2] >> 4];
        D_004512E8[6] = str_0123456789abcdefUSBD_IRX[macBytes[2] & 0xF];
        D_004512E8[7] = str_0123456789abcdefUSBD_IRX[macBytes[3] >> 4];
        D_004512E8[8] = str_0123456789abcdefUSBD_IRX[macBytes[3] & 0xF];
        D_004512E8[9] = str_0123456789abcdefUSBD_IRX[macBytes[4] >> 4];
        D_004512E8[10] = str_0123456789abcdefUSBD_IRX[macBytes[4] & 0xF];
        D_004512E8[11] = str_0123456789abcdefUSBD_IRX[macBytes[5] >> 4];
        D_004512E8[12] = str_0123456789abcdefUSBD_IRX[macBytes[5] & 0xF];
        D_004512E8[0] = '$';
        D_004512E8[13] = 0;
    }
    return D_004512E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnDisconnect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BA4;

extern int func_003F4C98(char *, int, int);
extern int func_003F4C68(char *, int, int, int, int, int, int, int, int, int);
/**
 * Asks the IOP network module to disconnect ('netw'/'shut') and waits for it.
 *
 * cCommSystem calls it when it drops its net connection; NetConnShutdown calls it before destroying the module.
 *
 * @return 0
 */
int NetConnDisconnect(void) {
    char buf[16];
    func_003F4C98(buf, 'netw', 'shut');
    func_003F4C68(buf, 1, D_004A4BA4, 0, 0, 0, 0, 0, 1, 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", NetConnShutdown);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BA0;

extern int D_004A4BA4;

extern int NetConnDisconnect(void);
extern int func_003F4C98(char *, int, int);
extern int func_003F4C68(char *, int, int, int, int, int, int, int, int, int);
/**
 * Shuts the IOP network module down: disconnects (NetConnDisconnect), sends 'netw'/'dest' and clears the started flag
 * D_004A4BA0.
 *
 * Nothing in retail calls it.
 *
 * @return 0, or -1 when the module was not started
 */
int NetConnShutdown(void) {
    char buf[16];
    if (D_004A4BA0 == 0) return -1;
    NetConnDisconnect();
    func_003F4C98(buf, 'netw', 'dest');
    func_003F4C68(buf, 1, D_004A4BA4, 0, 0, 0, 0, 0, 1, 0);
    D_004A4BA0 = 0;
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

extern int D_004A4BA0;

extern int NetTick(void);

extern unsigned _NetConn_uNextIdleTick;
extern int func_003F4A78(void);
/**
 * Runs DirtySock's idle processing, at most every 5 ticks while the network connection is up.
 *
 * When D_004A4BA0 is set it returns early until NetTick (the tick) reaches _NetConn_uNextIdleTick and then sets that to tick
 * + 5. Then func_003F4A78 runs and each of the 16 idle callbacks in _NETidle with a function is called with its
 * argument and the current tick. On Tour checks NetConnStatus('open') instead of the flag and runs the callbacks in
 * NetConnIdleProcs.
 */
void NetConnIdle(void) {
    int i;
    struct sTick *idleProc;
    if (D_004A4BA0 != 0) {
        if (NetTick() < _NetConn_uNextIdleTick) return;
        _NetConn_uNextIdleTick = NetTick() + 5;
    }
    func_003F4A78();
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
 * LobbyApi code (func_003E94F0) uses it to time its pings.
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
 * @param a  first argument (ignored)
 * @return 1
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int NetPrintfCode(int a, ...) { return 1; }
#endif

/* D_0051F0C0: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store f14 and wake th). */
struct sSifReq { int f0; int f4; int pad8[5]; int key; };

struct sSifRes { int pad0[2]; int f8; int fC; int f10; int f14; int pad18; int key; };

struct sSifCb { int (*fn)(int, int, int, int, int, int, int, int, int *); int *out; int arg; int th; };

struct sSifEnt { int idx; int k1; int k2; int v; };

struct sSifQueue {
    char buf[0x4D88];                   /* byte 0: done flag (func_003F4F40) */
    struct sSifEnt *beg;                /* 0x4D88 */
    struct sSifEnt *end;
    int pad4D90;
    struct sSifReq *a;                  /* 0x4D94 */
    int pad4D98[2];
    struct sSifRes *b;                  /* 0x4DA0 */
    int pad4DA4[2];
    int base2;                          /* 0x4DAC */
    int pad4DB0;
    int base1;                          /* 0x4DB4 */
    int pad4DB8[3];
    struct sSifCb *c;                   /* 0x4DC4 */
    int pad4DC8[2];
    int head;                           /* 0x4DD0 */
    int tail;
    int cnt;
    int pad4DDC;
    int pos;                            /* 0x4DE0 */
    int pad4DE4;
    int h4DE8;
    int h4DEC;
    int h4DF0;
    int h4DF4;
    volatile unsigned last;             /* 0x4DF8: Count at the last update */
    int pad4DFC;
    volatile unsigned long total;       /* 0x4E00 */
    int sema;                           /* 0x4E08 */
};

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4410);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (read the Count register, enable interrupts) and memory barrier.
#define MFC0_COUNT_2F41(v) __asm__ volatile("mfc0 %0, $9" : "=r"(v))
#define EI_2F41() __asm__ volatile("ei")
#define SYNC_2F41() __asm__ volatile("sync")

/**
 * Interrupt handler that extends the COP0 Count register into the IOP queue's 64-bit cycle total.
 *
 * Installed by func_003F5000 for interrupt cause 2 with the queue D_0051F0C0 as its argument: adds the Count cycles
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
int func_003F4410(int cause, void *handlerArg) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F4448);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern int f4C50(int) __asm__("iWakeupThread");
extern int f3B80(int, void *, void *) __asm__("iSetAlarm");
void func_003F4448(int a, int b, struct sSifQueue *s);
/**
 * Alarm handler that wakes the IOP queue thread when a reply has arrived, re-arming itself while requests are pending.
 *
 * Forgets the alarm id that fired (+0x4DEC, armed by func_003F4528; +0x4DF0, its own). While requests are outstanding,
 * wakes the queue thread (+0x4DE8, iWakeupThread) when the oldest one's reply key matches, and sets a new 16-tick alarm
 * (iSetAlarm) when none is armed.
 *
 * @param alarmId  id of the alarm that fired
 * @param alarmTime  alarm time (unused)
 * @param queue  the queue (D_0051F0C0)
 */
void func_003F4448(int alarmId, int alarmTime, struct sSifQueue *queue) {
    if (alarmId == queue->h4DEC) queue->h4DEC = -1;
    if (alarmId == queue->h4DF0) queue->h4DF0 = -1;
    if (queue->tail != queue->head) {
        if (queue->b[queue->tail].key == queue->a[queue->tail].key) f4C50(queue->h4DE8);
        if (queue->h4DEC < 0 && queue->h4DF0 < 0) queue->h4DF0 = f3B80(0x10, (void *)func_003F4448, queue);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F44F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F4A78(void);
extern int SleepThread(void);
extern volatile int D_004A5B04;
/**
 * Entry point of the IOP queue thread (created by func_003F5000): forever processes arrived replies (func_003F4A78) and
 * sleeps, with D_004A5B04 set while it sleeps.
 */
void func_003F44F8(void) {
    while (1) {
        func_003F4A78();
        D_004A5B04 = 1;
        SleepThread();
        D_004A5B04 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4528);
#ifdef SKIP_ASM
/* D_0051F0C0: the SIF request queue (layout from every user in this unit). */
extern struct sSifQueue D_0051F0C0;

struct sSifClient_4528 { int pad0[3]; int fC; };
struct sSifSeg_4528 { int len; char *ptr; };
struct sSifDma_4528 { int data; int addr; int size; int mode; };
extern struct { unsigned char b; char pad[11]; int w; } D_00451300;
extern volatile int D_004A5B00;
extern volatile int D_004A5B08;
extern int WaitSema(int);
extern int SignalSema(int);
extern int func_003F4A78(void);
extern int GetThreadId(void);
extern void func_0041605C(char *, char *, int);
extern int SyncDCache(int, int);
extern int sceSifSetDma(struct sSifDma_4528 *, int);
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
 * @p cl is a service handle resolved by func_003F4C98 (+0xC is the service's IOP id; -1 without one). @p dst must be
 * 16-byte aligned (its size is rounded down to 16). A negative @p size means @p data is a C string; flag 2 makes
 * @p data a {len, ptr} list ending with len 0, gathered into one payload. Under the queue semaphore, while the ring is
 * full it pumps replies (func_003F4A78). It takes room in the 64-byte-aligned payload area (-1 when none), records the
 * completion (@p fn, @p out, flags, thread id) in the callback ring, copies the payload, fills the request record
 * (payload address and size, reply buffer and size, @p fno, @p callArg, service id, key) and DMAs payload and record to the
 * IOP (sceSifSetDma, retried every tick until accepted). Flag 4 writes the reply buffer back from the cache first; flag
 * 1 sends SIF command 1 to kick the IOP. With @p fn == 1 it arms the wake alarm (func_003F4448) and sleeps until
 * func_003F4A78 stores the reply's result in *@p out and wakes it.
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
int func_003F4528(struct sSifClient_4528 *cl, int flags, int fno, int callArg, char *data, int size, int dst, int dstsize,
                  int fn, int *out) {
    struct sSifQueue *queue = &D_0051F0C0;
    struct sSifDma_4528 dma[2];
    struct sSifSeg_4528 *seg;
    int i;
    int off;
    int avail;
    int key;
    int tickNow;
    int nowait;
    int wb;
    int asize;

    if (cl == 0 || cl->fC == 0) return -1;
    dstsize &= 0xFFFFFFF0;
    if (dst & 0xF) return -1;
    if (size < 0) size = strlen(data) + 1;
    seg = (flags & 2) ? (struct sSifSeg_4528 *)data : 0;
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
            func_003F4A78();
            WaitSema(queue->sema);
        }
    }
    if (queue->pad4DDC < queue->pos) {
        off = queue->pad4DDC;
        avail = queue->pos - off - 0x40;
    } else if (queue->pad4DE4 - queue->pad4DDC - 0x40 >= size) {
        off = queue->pad4DDC;
        avail = queue->pad4DE4 - queue->pad4DDC;
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
    queue->c[queue->head].fn = (int (*)(int, int, int, int, int, int, int, int, int *))fn;
    queue->c[queue->head].out = out;
    queue->c[queue->head].arg = flags;
    queue->c[queue->head].th = GetThreadId();
    if (size > 0) {
        if (seg != 0) {
            char *copyDst = (char *)(queue->base2 + off);
            for (i = 0; seg[i].len > 0; i++) {
                func_0041605C(copyDst, seg[i].ptr, seg[i].len);
                copyDst += seg[i].len;
            }
        } else {
            func_0041605C((char *)(queue->base2 + off), data, size);
        }
    }
    queue->a[queue->head].f0 = queue->base1 + off;
    queue->a[queue->head].f4 = size;
    queue->a[queue->head].pad8[0] = dst;
    queue->a[queue->head].pad8[1] = dstsize;
    queue->a[queue->head].pad8[2] = fno;
    queue->a[queue->head].pad8[3] = callArg;
    queue->a[queue->head].pad8[4] = cl->fC;
    queue->a[queue->head].key = queue->pad4DC8[1];
    if (wb) f4528_wb(dst, dstsize + 0x80);
    dma[0].data = queue->base2 + off;
    dma[0].addr = queue->base1 + off;
    dma[0].size = asize & ~0x3F;
    dma[0].mode = 0;
    dma[1].data = (int)&queue->a[queue->head];
    dma[1].addr = queue->pad4D98[1] + queue->head * sizeof(struct sSifReq);
    dma[1].size = 0x20;
    dma[1].mode = 0;
    ((int *)queue->pad4DB8[0])[queue->head] = dma[0].size != 0 ? sceSifSetDma(&dma[0], 2) : sceSifSetDma(&dma[1], 1);
    while (((int *)queue->pad4DB8[0])[queue->head] == 0) {
        tickNow = NetTick();
        while (tickNow == NetTick()) {
        }
        ((int *)queue->pad4DB8[0])[queue->head] = dma[0].size != 0 ? sceSifSetDma(&dma[0], 2) : sceSifSetDma(&dma[1], 1);
    }
    if (nowait) sceSifSendCmd(1, &D_00451300, D_00451300.b, 0, 0, 0);
    key = queue->pad4DC8[1];
    if (size > 0) queue->pad4DDC = off + (asize & ~0x3F);
    queue->pad4DC8[1] = (queue->pad4DC8[1] + 1) % *(int *)&queue->buf[0x4D84];
    queue->head = (queue->head + 1) % queue->cnt;
    SignalSema(queue->sema);
    if (queue->h4DEC < 0 && fn == 1) queue->h4DEC = SetAlarm(8, (void *)func_003F4448, queue);
    if (fn == 1) {
        D_004A5B08 = 2;
        SleepThread();
        D_004A5B08 = 0;
    }
    return key;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4A78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#define SYNC_2F41() __asm__ volatile("sync")

/* D_0051F0C0: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store f14 and wake th). */
extern struct sSifQueue D_0051F0C0;

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
int func_003F4A78(void) {
    struct sSifQueue *queue = &D_0051F0C0;
    int n = 0;
    while (queue->tail != queue->head && queue->b[queue->tail].key == queue->a[queue->tail].key) {
        int th, key, kind, reqFlags, replyWord10, replyResult, addr, len, replyWord8, pC;
        D_004A5B00 = 1;
        WaitSema(queue->sema);
        D_004A5B00 = 0;
        key = queue->b[queue->tail].key;
        if (key != queue->a[queue->tail].key) {
            SignalSema(queue->sema);
            return n;
        }
        th = 0;
        addr = queue->base2 + (queue->a[queue->tail].f0 - queue->base1);
        replyWord8 = queue->b[queue->tail].f8;
        pC = queue->b[queue->tail].fC;
        replyWord10 = queue->b[queue->tail].f10;
        replyResult = queue->b[queue->tail].f14;
        kind = (int)queue->c[queue->tail].fn;
        reqFlags = queue->c[queue->tail].arg;
        len = queue->a[queue->tail].f4;
        if (kind != 0) {
            if (kind == 1) {
                if (queue->c[queue->tail].out != 0) *queue->c[queue->tail].out = replyResult;
                th = queue->c[queue->tail].th;
            } else {
                queue->c[queue->tail].fn(key, reqFlags, replyWord10, replyResult, addr, len, replyWord8, pC, queue->c[queue->tail].out);
                SYNC_2F41();
            }
        }
        if (queue->a[queue->tail].f4 > 0) {
            queue->pos = queue->a[queue->tail].f0 - queue->base1;
            queue->pos += (queue->a[queue->tail].f4 + 0x3F) & ~0x3F;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F4C68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an unused `int unused[12]` gives the 0x60 frame (perhaps a compiled-out debug buffer); plain form 99.50%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int func_003F4528_4C68(int, int, int, int, int, int, int, int, int, int, int) __asm__("func_003F4528");
int func_003F4C68_impl(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) __asm__("func_003F4C68");
/**
 * Sends an IOP request: forwards its ten arguments to func_003F4528 (passing the call argument a second time as an
 * eleventh).
 *
 * @param handle  service handle (from func_003F4C98)
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
int func_003F4C68_impl(int handle, int flags, int objectId, int callArg, int payload, int payloadSize, int replyBuf, int replySize, int completion, int resultOut) {
    int stackPad[12];
    return func_003F4528_4C68(handle, flags, objectId, callArg, payload, payloadSize, replyBuf, replySize, completion, resultOut, callArg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4C98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0051F0C0: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store f14 and wake th). */
extern struct sSifQueue D_0051F0C0;

int f4C98impl(struct sSifEnt *c, int k1, int k2) __asm__("func_003F4C98");
/**
 * Resolves an IOP service by its module and function tags into a 16-byte handle, caching the result.
 *
 * When the handle already holds the same tags and its cached index still names a directory entry (index < 129) with the
 * same id and module tag, returns 0. Otherwise stores the tags and searches the service directory (+0x4D88 .. +0x4D8C
 * of D_0051F0C0, fetched by func_003F4D68) for the pair: found, it copies the entry's index and IOP id and returns 1;
 * not found, it clears both and returns -1.
 *
 * @param handle  the handle: index, module tag, function tag, IOP id
 * @param moduleTag  module tag ('netw', 'name', 'sess', ...)
 * @param funcTag  function tag ('crea', 'look', 'send', ...)
 * @return 0 cached, 1 found, -1 unknown service
 */
int f4C98impl(struct sSifEnt *handle, int moduleTag, int funcTag) {
    struct sSifQueue *queue = &D_0051F0C0;
    struct sSifEnt *dirEntry;
    if (moduleTag == handle->k1 && funcTag == handle->k2) {
        int i = handle->idx;
        if (i >= 0) {
            if (i < 129) {
                struct sSifEnt *ent = (struct sSifEnt *)(i * 16 + (int)queue->beg);
                if (ent->v == handle->v && ent->k1 == moduleTag) return 0;
            }
        }
    }
    handle->k2 = funcTag;
    handle->k1 = moduleTag;
    for (dirEntry = queue->beg; dirEntry != queue->end; dirEntry++) {
        if (dirEntry->k1 == handle->k1 && dirEntry->k2 == handle->k2) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F4D68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0051F0C0: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store f14 and wake th). */
extern struct sSifQueue D_0051F0C0;

extern int func_003F4C68d(int *, int, int, int, int, int, int, int, int, int) __asm__("func_003F4C68");
extern int func_003F4A78(void);
/**
 * Fetches the IOP's service directory into the queue and waits until every queued request has completed.
 *
 * Sends a blocking request to the fixed directory service (a handle whose IOP id is 1) with the directory area of
 * D_0051F0C0 (+0x4D88 .. +0x4D8C, 0x800 bytes) as the reply buffer, then pumps replies (func_003F4A78) until the ring
 * is empty. func_003F4C98 searches that directory. Called by NetConnStartup right after starting the queue.
 *
 * @return 0
 */
int func_003F4D68(void) {
    int dirHandle[4];
    dirHandle[3] = 1;
    func_003F4C68d(dirHandle, 1, 0, 0, 0, 0, (int)D_0051F0C0.beg, (char *)D_0051F0C0.end - (char *)D_0051F0C0.beg, 1, 0);
    while (D_0051F0C0.head != D_0051F0C0.tail) func_003F4A78();
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
 * interrupt handler func_003F4410 (re-reading while either changes), adds the Count cycles since that update and
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
        last = D_0051F0C0.last;
        total = D_0051F0C0.total;
        MFC0_COUNT_2F41(c);
        cnt = c;
        lastSeen = cnt;
        lastSeen = last;
    } while (lastSeen != D_0051F0C0.last || total != D_0051F0C0.total);
    sum = total + (unsigned)(cnt - last);
    return (int)__udivdi3_4DE8(sum, 0x48000);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4E80);
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
int func_003F4E80(int start, int size) { return SyncDCache(start, start + size); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4EA0);
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
int func_003F4EA0(unsigned blockAddr, int *out) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F4F40);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (read the Count register).
#define MFC0_COUNT_2F41(v) __asm__ volatile("mfc0 %0, $9" : "=r"(v))

/* D_0051F0C0: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store f14 and wake th). */
extern struct sSifQueue D_0051F0C0;

extern struct { unsigned char b; char pad[11]; int w; } D_00451340;
extern void FlushCache(int);
extern void sceSifSendCmd(int, void *, int, int, int, int);
/**
 * Checks that the IOP side of the request queue answers.
 *
 * Clears the flag byte at the start of D_0051F0C0, sends SIF command 1 (sceSifSendCmd) carrying the flag's address + 1,
 * and spins (flushing the cache) until the IOP sets the flag or 50,000,000 Count cycles (about 170 ms) pass.
 *
 * @return 1 when the IOP answered, 0 on timeout
 */
int func_003F4F40(void) {
    int answered;
    unsigned start;
    unsigned c;
    char *flag = D_0051F0C0.buf;
    unsigned limit = 0x2FAF080;
    *flag = 0;
    D_00451340.w = (int)flag + 1;
    FlushCache(0);
    sceSifSendCmd(1, &D_00451340, D_00451340.b, 0, 0, 0);
    answered = 1;
    MFC0_COUNT_2F41(start);
    while (*flag == 0) {
        MFC0_COUNT_2F41(c);
        if (c - start > limit) {
            answered = 0;
            break;
        }
        FlushCache(0);
    }
    return answered;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F5000);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_0051F0C0: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store f14 and wake th). */
extern struct sSifQueue D_0051F0C0;

extern int D_004A4BB0;
extern char D_00523F00[];   /* the queue thread's stack (0x2000 bytes) */
extern int _gp;   /* the linker-script $gp value (0x4A30F0, D_004A30F0); gas never addresses _gp off $gp */

/* What func_003F5000 DMAs to the IOP buffer's first 0x40 bytes: per region, its IOP start / end and EE address. */
struct sSifHdr_5000 { int f0; int f4; int v[14]; };
struct sSemaParam_5000 { int count; int max_count; int init_count; int wait_threads; unsigned int attr; unsigned int option; };
struct sThreadParam_5000 { int status; void (*func)(void); void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned int attr; unsigned int option; };

extern void f5000_6210(void *, int, int) __asm__("memset");
/* sceSifSendCmd (sceSifSendCmd) returns its packet id; the unit declares it void. */
extern int f5000_61B0(int, void *, int, int, int, int) __asm__("sceSifSendCmd");
extern int sceSifAllocIopHeap(int);
extern int sceSifDmaStat(int);
extern void sceSifInitCmd(void);
extern int CreateSema(struct sSemaParam_5000 *);
extern int CreateThread(struct sThreadParam_5000 *);
extern int AddIntcHandler2(int, int (*)(int, void *), int, void *);
extern int EnableIntc(int);
extern void StartThread(int, void *);
extern int func_003F4410(int, void *);

/**
 * Starts the EE-IOP request queue D_0051F0C0.
 *
 * Fails (-1) when the IOP does not answer (func_003F4F40). Zeroes the queue and lays out its uncached areas: service
 * directory (0x800 bytes), request ring and reply ring (0x200 each), payload area (0x4000), DMA-id words and the
 * completion ring. Allocates the IOP-side copy (sceSifAllocIopHeap, -2 on failure), DMAs a 0x40-byte header describing the
 * areas and then the request ring (sceSifSetDma, -3 / -4 on failure), initialises SIF commands (sceSifInitCmd) and
 * sends command 1 with the IOP address. Creates the queue semaphore and the queue thread (func_003F44F8, priority 10,
 * 0x2000-byte stack D_00523F00; -5 on failure), installs the Count handler func_003F4410 on interrupt 2 and enables it,
 * starts the thread and sets the started flag D_004A4BB0.
 *
 * @return 0, or -1 .. -5 on failure
 */
int func_003F5000(void) {
    struct sSifQueue *queue = &D_0051F0C0;
    struct sSifHdr_5000 hdr;
    struct sSifDma_4528 dma;
    struct sSemaParam_5000 sema;
    struct sThreadParam_5000 tp;
    int iop;
    int size;
    int id;

    if (func_003F4F40() == 0) return -1;
    f5000_6210(queue, 0, 20032);
    *(int *)&queue->buf[0x4D80] = 0x10;
    *(int *)&queue->buf[0x4D84] = 0x7FFFFFFF;
    queue->beg = (struct sSifEnt *)(((int)&queue->buf[0] & 0xFFFFFFF) | 0x20000000);
    queue->end = (struct sSifEnt *)(((int)&queue->buf[0x800] & 0xFFFFFFF) | 0x20000000);
    queue->a = (struct sSifReq *)(((int)&queue->buf[0x800] & 0xFFFFFFF) | 0x20000000);
    queue->pad4D98[0] = ((int)&queue->buf[0xA00] & 0xFFFFFFF) | 0x20000000;
    queue->b = (struct sSifRes *)(((int)&queue->buf[0xA00] & 0xFFFFFFF) | 0x20000000);
    queue->pad4DA4[0] = ((int)&queue->buf[0xC00] & 0xFFFFFFF) | 0x20000000;
    queue->base2 = ((int)&queue->buf[0xC00] & 0xFFFFFFF) | 0x20000000;
    queue->pad4DB0 = ((int)&queue->buf[0x4C00] & 0xFFFFFFF) | 0x20000000;
    queue->pad4DB8[0] = (int)&queue->buf[0x4C00];
    queue->pad4DB8[1] = (int)&queue->buf[0x4C40];
    queue->c = (struct sSifCb *)&queue->buf[0x4C40];
    queue->pad4DC8[0] = (int)&queue->buf[0x4D40];
    size = 0x40;
    size += (queue->end - queue->beg) * sizeof(struct sSifEnt);
    size += ((struct sSifReq *)queue->pad4D98[0] - queue->a) * sizeof(struct sSifReq);
    size += ((struct sSifRes *)queue->pad4DA4[0] - queue->b) * sizeof(struct sSifRes);
    size += queue->pad4DB0 - queue->base2;
    size += ((int *)queue->pad4DB8[1] - (int *)queue->pad4DB8[0]) * sizeof(int);
    queue->pad4DB8[2] = sceSifAllocIopHeap(size);
    if (queue->pad4DB8[2] == 0) return -2;
    iop = (queue->pad4DB8[2] & 0xFFFFFFF) | 0xA0000000;
    queue->pad4DB8[2] = iop;
    hdr.f0 = *(int *)&queue->buf[0x4D80];
    hdr.f4 = *(int *)&queue->buf[0x4D84];
    hdr.v[0] = iop + 0x40;
    hdr.v[1] = hdr.v[0] + (queue->end - queue->beg) * sizeof(struct sSifEnt);
    hdr.v[2] = (int)queue->beg;
    hdr.v[3] = hdr.v[1];
    hdr.v[4] = hdr.v[3] + ((struct sSifReq *)queue->pad4D98[0] - queue->a) * sizeof(struct sSifReq);
    hdr.v[5] = (int)queue->a;
    hdr.v[6] = hdr.v[4];
    hdr.v[7] = hdr.v[6] + ((struct sSifRes *)queue->pad4DA4[0] - queue->b) * sizeof(struct sSifRes);
    hdr.v[8] = (int)queue->b;
    hdr.v[9] = hdr.v[7];
    hdr.v[10] = hdr.v[9] + (queue->pad4DB0 - queue->base2);
    hdr.v[11] = queue->base2;
    hdr.v[12] = hdr.v[10];
    hdr.v[13] = hdr.v[12] + ((int *)queue->pad4DB8[1] - (int *)queue->pad4DB8[0]) * sizeof(int);
    queue->pad4D90 = hdr.v[0];
    queue->pad4D98[1] = hdr.v[3];
    queue->pad4DA4[1] = hdr.v[6];
    queue->base1 = hdr.v[9];
    FlushCache(0);
    dma.data = (int)&hdr;
    dma.addr = queue->pad4DB8[2];
    dma.size = 0x40;
    dma.mode = 0;
    id = sceSifSetDma(&dma, 1);
    if (id == 0) return -3;
    while (sceSifDmaStat(id) >= 0) {
    }
    dma.data = (int)queue->a;
    dma.addr = queue->pad4D98[1];
    dma.size = 0x200;
    dma.mode = 0;
    id = sceSifSetDma(&dma, 1);
    if (id == 0) return -4;
    while (sceSifDmaStat(id) >= 0) {
    }
    sceSifInitCmd();
    D_00451340.w = queue->pad4DB8[2];
    FlushCache(0);
    f5000_61B0(1, &D_00451340, D_00451340.b, 0, 0, 0);
    queue->pad4DC8[1] = 1;
    queue->cnt = ((struct sSifReq *)queue->pad4D98[0] - queue->a);
    queue->tail = 1;
    queue->head = 1;
    queue->pad4DE4 = queue->pad4DB0 - queue->base2;
    f5000_6210(&sema, 0, sizeof(sema));
    sema.init_count = 1;
    sema.max_count = 1;
    sema.option = 0;
    queue->sema = CreateSema(&sema);
    queue->h4DEC = -1;
    queue->h4DF0 = -1;
    tp.func = func_003F44F8;
    tp.initPriority = 10;
    tp.stack = D_00523F00;
    tp.stackSize = 0x2000;
    tp.gpReg = &_gp;
    tp.option = 0;
    queue->h4DE8 = CreateThread(&tp);
    if (queue->h4DE8 <= 0) return -5;
    queue->h4DF4 = AddIntcHandler2(2, func_003F4410, -1, queue);
    EnableIntc(2);
    StartThread(queue->h4DE8, queue);
    D_004A4BB0 = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F53F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0051F0C0: the SIF request queue (layout from every user in this unit). Requests a[] are sent, results b[] come
   back with the same key; c[] holds the completion callback (fn 0 = none, 1 = store f14 and wake th). */
extern struct sSifQueue D_0051F0C0;

extern int D_004A4BB0;

extern void ReleaseAlarm(int);
extern void RemoveIntcHandler(int, int);
extern void TerminateThread(int);
extern void DeleteThread(int);
extern void DeleteSema(int);
/**
 * Stops the EE-IOP request queue: releases its alarms, removes the interrupt handler, terminates and deletes the queue
 * thread, deletes the semaphore and clears the started flag D_004A4BB0.
 *
 * Nothing in retail calls it.
 *
 * @return 0
 */
int func_003F53F8(void) {
    struct sSifQueue *queue = &D_0051F0C0;
    if (queue->h4DEC >= 0) ReleaseAlarm(queue->h4DEC);
    if (queue->h4DF0 >= 0) ReleaseAlarm(queue->h4DF0);
    RemoveIntcHandler(2, queue->h4DF4);
    TerminateThread(queue->h4DE8);
    DeleteThread(queue->h4DE8);
    DeleteSema(queue->sema);
    D_004A4BB0 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5470);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BB0;

/**
 * Returns whether the EE-IOP request queue has been started (D_004A4BB0, set by func_003F5000).
 *
 * @return the started flag
 */
int func_003F5470(void) {
    return D_004A4BB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5478);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F00[];
extern int *RpcAlloc(int);
extern void *f6210h(void *, int, int) __asm__("memset");
extern int SyncDCache(int, int);
extern void FlushCache(int);
extern void RpcFree(int *);
extern int func_003F4C98(char *, int, int);
extern int func_003F4C68(char *, int, int, int, int, int, int, int, int, int);
/**
 * Creates a network packet session on the IOP ('sess'/'crea').
 *
 * Clamps @p size to 0x1000..0x2000 and allocates a 156-byte header plus the buffers with RpcAlloc: a 64-byte-aligned
 * 0x20-byte address block (+0x4) and, 0x40 bytes after it, the receive buffer (+0x18) of @p size bytes (+0x14). Zeroes
 * both and writes them back from the cache, then waits for the IOP session id into +0x0; with id 0 the block is freed
 * and 0 returned. BuddyApi (func_003F0710) creates its session with 0x8000, clamped to 0x2000.
 *
 * @param size  receive buffer size
 * @return the session, or 0
 */
int *func_003F5478(int size) {
    int *session;

    if (size < 0x1000) size = 0x1000;
    if (size > 0x2000) size = 0x2000;

    session = RpcAlloc(size + 156);
    if (session == 0) return 0;
    f6210h(session, 0, 156);
    session[1] = ((int)session + 0x5B) & ~0x3F;
    session[5] = size;
    session[6] = session[1] + 0x40;
    f6210h((void *)session[1], 0, 0x20);
    f6210h((void *)session[6], 0, session[5]);
    SyncDCache(session[1], session[1] + 0x20);
    SyncDCache(session[6], session[6] + session[5]);
    FlushCache(0);
    func_003F4C98(D_00525F00, 'sess', 'crea');
    func_003F4C68(D_00525F00, 1, 0, size, 0, 0, 0, 0, 1, (int)session);
    if (session[0] == 0) {
        RpcFree(session);
        session = 0;
    }
    return session;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F55B8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F10[];
extern int func_003F4C98(char *, int, int);
extern void RpcFree(int *);
/**
 * Destroys a network packet session: sends 'sess'/'dest' for its id and waits, then frees it (RpcFree).
 *
 * @param session  the session
 */
void func_003F55B8(int *session) {
    func_003F4C98(D_00525F10, 'sess', 'dest');
    func_003F4C68(D_00525F10, 1, *session, 0, 0, 0, 0, 0, 1, 0);
    RpcFree(session);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5638);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F20[];
extern int func_003F4C98(char *, int, int);
extern int func_003F4C68(char *, int, int, int, int, int, int, int, int, int);
extern int SyncDCache(int, int);
/* Network session handle (0x9C bytes, created by func_003F5478). The receive callback func_003F5A50 (an RPC
   completion) writes f8 / fC / f10, so they are volatile in every accessor. */
struct sSess { int f0; int f4; volatile int f8; volatile int fC; volatile int f10; int f14; unsigned char *f18; };
typedef struct { unsigned short family; unsigned short port; int addr; int z8; int zC; } SA5638;
/**
 * Connects a network packet session to an IPv4 address and port ('sess'/'conn', not waited for).
 *
 * Returns -1 when the session's address block (+0x4) is already in use (family set). Otherwise fills it as a
 * sockaddr_in (family 2, big-endian port and address), writes it back from the cache, sends it and clears the
 * received-byte count (+0x10). func_003F5738 disconnects.
 *
 * @param addr  IPv4 address (host order)
 * @param port  port
 * @return 0, or -1 when already connected
 */
int func_003F5638(struct sSess *self, unsigned addr, int port) {
    unsigned addrShift;
    if (((SA5638 *)self->f4)->family != 0) return -1;
    ((SA5638 *)self->f4)->family = 2;
    ((SA5638 *)self->f4)->port = 0;
    ((SA5638 *)self->f4)->addr = 0;
    ((SA5638 *)self->f4)->z8 = 0;
    ((SA5638 *)self->f4)->zC = 0;
    ((unsigned char *)self->f4)[2] = port >> 8;
    ((unsigned char *)self->f4)[3] = port;
    addrShift = addr;
    ((unsigned char *)self->f4)[7] = addrShift;
    addrShift >>= 8;
    ((unsigned char *)self->f4)[6] = addrShift;
    addrShift >>= 8;
    ((unsigned char *)self->f4)[5] = addrShift;
    addrShift >>= 8;
    ((unsigned char *)self->f4)[4] = addrShift;
    SyncDCache(self->f4, self->f4 + 0x20);
    func_003F4C98(D_00525F20, 'sess', 'conn');
    func_003F4C68(D_00525F20, 0, self->f0, 0, self->f4, 0x10, 0, 0, 0, 0);
    self->f10 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5738);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F30[];
extern int func_003F4C98(char *, int, int);
extern void func_00416210b(int, int, int) __asm__("memset");
/**
 * Disconnects a network packet session ('sess'/'disc', not waited for), zeroes its address block (+0x4) and clears the
 * received-byte count (+0x10).
 *
 * @return 0
 */
int func_003F5738(struct sSess *self) {
    func_003F4C98(D_00525F30, 'sess', 'disc');
    func_003F4C68(D_00525F30, 0, self->f0, 0, 0, 0, 0, 0, 0, 0);
    func_00416210b(self->f4, 0, 0x20);
    self->f10 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F57C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { unsigned char b[0x18]; } B57C8;
extern char D_00525F40[];
extern int func_003F4C98(char *, int, int);
extern int c57_4C68(char *, int, int, int, int, int, B57C8 *, int, int, int) __asm__("func_003F4C68");
int func_003F57C8_impl(struct sSess *self, int key) __asm__("func_003F57C8");
/**
 * Returns one field of a network packet session's status.
 *
 * Fetches the 0x20-byte status from the IOP ('sess'/'stat', waiting) into the address block (+0x4), then returns by
 * @p key: 'port' / 'addr' the peer's port and address (bytes 2-3 / 4-7, big-endian), 'lprt' / 'ladr' the local port and
 * address (bytes 0x12-0x13 / 0x14-0x17), 'ibuf' the bytes received and not yet read (+0x10); 0 for anything else.
 * Nothing in retail calls it.
 *
 * @param key  selector ('port', 'addr', 'lprt', 'ladr', 'ibuf')
 * @return the selected value
 */
int func_003F57C8_impl(struct sSess *self, int key) {
    func_003F4C98(D_00525F40, 'sess', 'stat');
    c57_4C68(D_00525F40, 0, self->f0, 0, 0, 0, (B57C8 *)self->f4, 0x20, 1, 0);
    if (key == 'port') {
        return (((B57C8 *)self->f4)->b[2] << 8) | ((B57C8 *)self->f4)->b[3];
    }
    if (key == 'addr') {
        return (((((B57C8 *)self->f4)->b[4] << 8 | ((B57C8 *)self->f4)->b[5]) << 8 | ((B57C8 *)self->f4)->b[6]) << 8) | ((B57C8 *)self->f4)->b[7];
    }
    if (key == 'ladr') {
        return (((((B57C8 *)self->f4)->b[0x14] << 8 | ((B57C8 *)self->f4)->b[0x15]) << 8 | ((B57C8 *)self->f4)->b[0x16]) << 8) | ((B57C8 *)self->f4)->b[0x17];
    }
    if (key == 'lprt') {
        return (((B57C8 *)self->f4)->b[0x12] << 8) | ((B57C8 *)self->f4)->b[0x13];
    }
    if (key == 'ibuf') {
        return self->f10;
    }
    return 0;
}
#endif

/**
 * Does nothing (an empty per-session update hook; func_003F17D8 and func_003F5910 call it).
 */
void func_003F5908(void) {
}

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5910);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int n0; unsigned char *p; int n; char *s; int z; } IO5910;
extern char D_00525F50[];
extern int func_003F4C98(char *, int, int);
extern int c59_4C68(char *, int, int, int, IO5910 *, int, int, int, int, int) __asm__("func_003F4C68");
extern void c59_5908(int *) __asm__("func_003F5908");
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
int func_003F5910(int *self, int packetKind, int packetArg, char *str, int len) {
    unsigned char hdr[12];
    IO5910 io;
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
    io.n0 = 0xC;
    io.p = hdr;
    io.n = len - 0xC;
    io.s = str;
    io.z = 0;
    func_003F4C98(D_00525F50, 'sess', 'send');
    c59_4C68(D_00525F50, 2, self[0], 0, &io, 0, 0, 0, 0, 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5A50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* A network session (func_003F5A70 / func_003F5C38 read from its receive buffer f18). */

int func_003F5A50_cb(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int len, struct sSess *self) __asm__("func_003F5A50");
/**
 * Completion callback of a session's 'sess'/'recv' request: records how many bytes arrived.
 *
 * Called by func_003F4A78 with the reply's words; stores the received length (the eighth argument, reply word +0xC) in
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
int func_003F5A50_cb(int requestKey, int requestFlags, int replyWord10, int replyResult, int payloadAddr, int payloadSize, int replyWord8, int len, struct sSess *self) {
    self->f10 = len;
    self->fC = 0;
    self->f8 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5A70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* A network session (func_003F5A70 / func_003F5C38 read from its receive buffer f18). */

extern char D_00525F60[];
extern char str_empty_l[];
extern int func_003F4C98(char *, int, int);
extern void a70_47D8(unsigned char *, unsigned char *) __asm__("InvalidDCache");
extern int a70_4C68(char *, int, int, int, int, int, unsigned char *, int, int, int) __asm__("func_003F4C68");
extern int func_003F5A50(void);
/**
 * Peeks at the next received packet of a network packet session, starting a new receive when all are read.
 *
 * When every received byte has been read (+0xC >= +0x10) and no receive is pending (+0x8), it marks one pending,
 * invalidates the cache over the receive buffer, resets the counts and sends 'sess'/'recv' with the receive buffer and
 * func_003F5A50 as completion; then returns -1. Otherwise it parses the 12-byte header at the read offset: kind and
 * argument (big-endian) go to *@p kindOut / *@p argOut, and *@p dataOut points at the data, NUL-terminated by overwriting its last
 * byte (or at an empty string when there is none). The packet stays unread; func_003F5C38 consumes it.
 *
 * @param kindOut  receives the packet kind (may be 0)
 * @param argOut  receives the packet argument (may be 0)
 * @param dataOut  receives the data pointer (may be 0)
 * @return the data length, or -1 when no packet is waiting
 */
int func_003F5A70(struct sSess *self, unsigned *kindOut, unsigned *argOut, char **dataOut) {
    int n;
    if (self->fC >= self->f10) {
        if (self->f8 == 0 && self->fC >= self->f10) {
            self->f8 = 1;
            if (self->f10 > 0) {
                a70_47D8(self->f18, self->f18 + self->f14 - 1);
                self->f10 = 0;
                self->fC = self->f10;
            }
            func_003F4C98(D_00525F60, 'sess', 'recv');
            a70_4C68(D_00525F60, 0, self->f0, 0, 0, 0, self->f18, self->f14, (int)func_003F5A50, (int)self);
        }
        return -1;
    } else {
        unsigned char *header = self->f18 + self->fC;
        char *payload = (char *)(self->f18 + self->fC) + 0xC;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F5C38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* A network session (func_003F5A70 / func_003F5C38 read from its receive buffer f18). */

extern char D_00525F70[];
extern int c38_5A70(struct sSess *, int, int, int) __asm__("func_003F5A70");
extern void c38_605C(char *, void *, int) __asm__("func_0041605C");
extern void c38_47D8(unsigned char *, unsigned char *) __asm__("InvalidDCache");
extern int func_003F4C98(char *, int, int);
extern int c38_4C68(char *, int, int, int, int, int, unsigned char *, int, int, int) __asm__("func_003F4C68");
extern int func_003F5A50(void);
int func_003F5C38_impl(struct sSess *self, int a, int b, char *buf, int max) __asm__("func_003F5C38");
/**
 * Reads and consumes the next received packet of a network packet session.
 *
 * Peeks with func_003F5A70 (kind and argument to @p kindOut / @p argOut), copies up to @p max data bytes into @p buf when given,
 * and advances the read offset past the packet. When that empties the buffer and no receive is pending it starts the
 * next 'sess'/'recv' as func_003F5A70 does. BuddyApi passes no outputs, just to drop the packet it peeked.
 *
 * @param kindOut  receives the packet kind (may be 0)
 * @param argOut  receives the packet argument (may be 0)
 * @param buf  data output (may be 0)
 * @param max  data output size
 * @return the data length, or -1 when no packet is waiting
 */
int func_003F5C38_impl(struct sSess *self, int kindOut, int argOut, char *buf, int max) {
    int dataStart;
    int n = c38_5A70(self, kindOut, argOut, 0);
    if (n >= 0) {
        if (n < max) max = n;
        if (buf) c38_605C(buf, self->f18 + self->fC + 0xC, max);
        dataStart = self->fC + 0xC;
        self->fC = dataStart + n;
        if (self->fC >= self->f10 && self->f8 == 0 && self->fC >= self->f10) {
            self->f8 = 1;
            if (self->f10 > 0) {
                c38_47D8(self->f18, self->f18 + self->f14 - 1);
                self->f10 = 0;
                self->fC = self->f10;
            }
            func_003F4C98(D_00525F70, 'sess', 'recv');
            c38_4C68(D_00525F70, 0, self->f0, 0, 0, 0, self->f18, self->f14, (int)func_003F5A50, (int)self);
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5D88);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetTick(void);
/**
 * Returns the millisecond tick (NetTick); the session argument its callers pass is ignored.
 *
 * @return milliseconds since the IOP queue's timer started
 */
int func_003F5D88(void) { return NetTick(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5DA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int strlen(const char *);

/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
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
    unsigned char *ents;                /* 0xA8 */
    int a;                              /* 0xAC */
    int b;
    int tick;                           /* 0xB4 */
    int period;
    void (*cb1)(int, int);              /* 0xBC */
    int (*cb2)(int);                    /* 0xC0 */
};

extern struct sKb D_00525F80;

extern char *strcpy(char *, const char *);
extern char* strcat(char *, const char *);
extern int fBA68(char *, int, const char *) __asm__("sceSifLoadModule");
extern char str_USBD_IRX_b[], str_USBKB_IRX_b[];
/**
 * Loads the USB driver modules USBD.IRX and USBKB.IRX from a directory.
 *
 * With @p name 0 it only marks both module ids in D_00525F80 as -1 and returns 0. Otherwise loads @p name + "USBD.IRX"
 * without arguments and @p name + "USBKB.IRX" with @p str as its argument text (sceSifLoadModule, the SIF module loader),
 * keeping the ids in D_00525F80.fd[0] and fd[1]. Called by the USB keyboard setup func_003F61D0.
 *
 * @param name  directory prefix of the IRX files, or 0
 * @param str  argument text for USBKB.IRX
 * @return 0, or -1 when @p str is 0 or a load fails
 */
int func_003F5DA8(char *name, char *str) {
    char path[0x100];
    if (name == 0) {
        D_00525F80.fd[0] = -1;
        D_00525F80.fd[1] = -1;
        return 0;
    }
    if (str == 0) goto fail;
    strcpy(path, name);
    strcat(path, str_USBD_IRX_b);
    D_00525F80.fd[0] = fBA68(path, 0, 0);
    if (D_00525F80.fd[0] < 0) goto fail;
    strcpy(path, name);
    strcat(path, str_USBKB_IRX_b);
    D_00525F80.fd[1] = fBA68(path, strlen(str) + 1, str); if (D_00525F80.fd[1] < 0) return -1; return 0;
fail:
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5E90);
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
int func_003F5E90(int handle, int errorValue) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F5F08);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

extern char D_00525F88[];
extern int sceUsbKbGetInfo(char *);
/**
 * Starts an asynchronous USB keyboard connection query.
 *
 * Copies each keyboard's connected flag (D_00525F80 +0x10) to the previous flags (+0x94), then asks for the keyboard
 * info (sceUsbKbGetInfo into +0x8); when the request is accepted sets the state (+0x9C) to 1 and the busy flag (+0xA0).
 * func_003F5F88 finishes it.
 */
void func_003F5F08(void) {
    int i;
    char *kbState;
    for (i = 0; i < D_00525F80.cnt; i++) D_00525F80.prev[i] = D_00525F80.v[i];
    if (sceUsbKbGetInfo(D_00525F88) == 0) {
        kbState = D_00525F88 - 8;
        *(int *)(kbState + 0x9C) = 1;
        kbState[0xA0] = 1;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5F88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

extern int f810(int, int *) __asm__("sceUsbKbSync");
/**
 * Finishes the USB keyboard connection query started by func_003F5F08.
 *
 * Polls the request without waiting (sceUsbKbSync with mode 1). When it has finished, calls the connect callback
 * (+0xBC) with (keyboard, connected) for every keyboard whose connected flag changed, and clears the busy flag and the
 * state.
 *
 * @return the poll result: nonzero while the query is still running
 */
int func_003F5F88(void) {
    int syncResult;
    int syncBusy;
    int i;
    syncBusy = f810(1, &syncResult);
    if (syncBusy != 0) return syncBusy;
    for (i = 0; i < D_00525F80.cnt; i++) {
        if (D_00525F80.prev[i] != D_00525F80.v[i]) D_00525F80.cb1(i, D_00525F80.v[i]);
    }
    D_00525F80.flag = 0;
    D_00525F80.st = 0;
    return syncBusy;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6040);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

/**
 * Finds the next connected USB keyboard, starting at @p i and wrapping, whose read buffer is not holding unread data
 * (ready flag at +0x88 of its 0x8C-byte buffer clear).
 *
 * @param i  keyboard to start from
 * @return the keyboard index, or -1 when there is none
 */
int func_003F6040(int i) {
    int checked;
    for (checked = 0; checked < D_00525F80.cnt; checked++) {
        if (i >= D_00525F80.cnt) i = 0;
        if (D_00525F80.v[i] != 0) {
            if (*(unsigned char *)(D_00525F80.ents + i * 0x8C + 0x88) == 0) return i;
        }
        i++;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F60B8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

extern int func_003F6040(int);
extern int sceUsbKbRead(int, int *);
/**
 * Starts reading the next USB keyboard that has room (func_003F6040 from the one after the current, +0xA4).
 *
 * Makes it current, remembers its buffer's first two words (LED and modifier state, +0xAC / +0xB0) and starts an
 * asynchronous read into the buffer (sceUsbKbRead); when accepted sets the state to 2 and the busy flag.
 */
void func_003F60B8(void) {
    int id = func_003F6040(D_00525F80.cur + 1);
    int *readBuf;
    if (id != -1) {
        D_00525F80.cur = id;
        readBuf = (int *)(D_00525F80.ents + id * 0x8C);
        D_00525F80.a = readBuf[0];
        D_00525F80.b = readBuf[1];
        if (sceUsbKbRead(id, readBuf) == 0) {
            D_00525F80.st = 2;
            D_00525F80.flag = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6138);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

extern int f810(int, int *) __asm__("sceUsbKbSync");
/**
 * Finishes the USB keyboard read started by func_003F60B8.
 *
 * Waits for the read to finish (sceUsbKbSync with mode 0, the blocking wait). Then marks the current keyboard's buffer
 * ready (+0x88 = 1) if it holds key codes or its LED / modifier words changed from the remembered ones, and clears the
 * busy flag and the state.
 *
 * @return 0, or sceUsbKbSync's error (-1 when the keyboard library is not running)
 */
int func_003F6138(void) {
    int syncResult;
    int syncStatus;
    syncStatus = f810(0, &syncResult);
    if (syncStatus != 0) return syncStatus;
    {
        int *readBuf = (int *)(D_00525F80.ents + D_00525F80.cur * 0x8C);
        if (readBuf[2] > 0 || readBuf[1] != D_00525F80.b || readBuf[0] != D_00525F80.a) ((unsigned char *)readBuf)[0x88] = 1;
    }
    D_00525F80.flag = 0;
    D_00525F80.st = 0;
    return syncStatus;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F61C8);
#ifdef SKIP_ASM
/**
 * Default idle hook of the USB keyboard reader: does nothing and returns 0 (never claims the update).
 *
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F61C8(void) { return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F61D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

extern int D_00526010[];

extern int func_003F5DA8(char *, char *);
extern int f0C028(int *) __asm__("sceUsbKbInit");
extern void f5F08(void) __asm__("func_003F5F08");
extern int f5F88(void) __asm__("func_003F5F88");
extern void f6358(int, int) __asm__("func_003F6358");
extern int f61C8(int) __asm__("func_003F61C8");
// The keyboard state block sits 0x90 bytes below the max-connect word passed to sceUsbKbInit.
static inline struct sKb *ctl(void) { return (struct sKb *)((char *)D_00526010 - 0x90); }
/**
 * Starts the USB keyboard reader.
 *
 * Loads the USB modules (func_003F5DA8 with @p name / @p str) and initialises the keyboard library (sceUsbKbInit with
 * the max-connect word D_00526010, capped at 8). Clears every keyboard's connected and previous flags and the ready
 * flag of each 0x8C-byte buffer in @p ents, resets the current keyboard, sets the info-refresh period and counter to 60
 * updates, installs the callbacks (defaults func_003F6358 and func_003F61C8) and runs one connection query to
 * completion (func_003F5F08, then func_003F5F88 until done). The USB keyboard reader object (cUsbKeyboard_cUsbKeyboard) calls it
 * with no module path, so the modules are expected to be loaded already (initOnline loads them).
 *
 * @param name  directory of USBD.IRX / USBKB.IRX, or 0 to skip loading
 * @param str  argument text for USBKB.IRX
 * @param ents  read buffers, 0x8C bytes per keyboard
 * @param cb1  called with (keyboard, connected) when a keyboard connects or disconnects (0 for func_003F6358)
 * @param cb2  idle hook polled by func_003F63F0 (0 for func_003F61C8)
 * @return 0, or -1 when a module or the library fails to start
 */
int func_003F61D0(char *name, char *str, unsigned char *ents, void (*cb1)(int, int), int (*cb2)(int)) {
    int i;
    if (func_003F5DA8(name, str) < 0) return -1;
    if (f0C028(D_00526010) == -1) return -1;
    if (ctl()->cnt > 8) ctl()->cnt = 8;
    ctl()->ents = ents;
    for (i = 0; i < ctl()->cnt; i++) {
        ctl()->v[i] = 0;
        ctl()->prev[i] = 0;
        ents[i * 140 + 136] = 0;
    }
    D_00525F80.cur = 0;
    D_00525F80.period = 0x3C;
    D_00525F80.tick = 0x3C;
    D_00525F80.flag = 0;
    D_00525F80.cb1 = cb1 ? cb1 : f6358;
    D_00525F80.cb2 = cb2 ? cb2 : f61C8;
    f5F08();
    while (f5F88()) ;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F62F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

extern int sceUsbKbEnd(void);
extern int func_003F5E90_62F8(int, int) __asm__("func_003F5E90");
/**
 * Shuts the USB keyboard reader down: ends the keyboard library (sceUsbKbEnd) and unloads USBKB.IRX and USBD.IRX
 * (func_003F5E90 with error values 1 and 2).
 *
 * @return 0, or the OR of the error values of the unloads that failed
 */
int func_003F62F8(void) {
    int unloadErrors;
    sceUsbKbEnd();
    unloadErrors = func_003F5E90_62F8(D_00525F80.fd[1], 1);
    return unloadErrors | func_003F5E90_62F8(D_00525F80.fd[0], 2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6348);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526010[];

/**
 * Returns the number of USB keyboards the reader handles (the max-connect word D_00526010).
 *
 * @return the keyboard count
 */
int func_003F6348(void) { return D_00526010[0]; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6358);
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
void func_003F6358(int keyboard, int on) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F63E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526038[];
/**
 * Sets how many reader updates pass between USB keyboard connection queries (D_00525F80 +0xB8).
 *
 * @param queryPeriod  the period in updates
 */
void func_003F63E0(int queryPeriod) { D_00526038[0] = queryPeriod; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F63F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

extern int func_003F5F88(void);
extern int func_003F6138(void);
extern void func_003F5F08(void);
extern void func_003F60B8(void);
void func_003F63F0_63F0(void) __asm__("func_003F63F0");
/**
 * Runs one step of the USB keyboard reader.
 *
 * By state (+0x9C): 1 finishes a connection query (func_003F5F88), 2 finishes a read (func_003F6138), 3 polls the idle
 * hook with 1 until it returns nonzero. When idle (state 0), a nonzero idle hook with 0 moves to state 3; otherwise the
 * counter (+0xB4) advances, and past the period (+0xB8) it restarts at 0 and queries the connections (func_003F5F08),
 * else it starts reading the next keyboard (func_003F60B8).
 */
void func_003F63F0_63F0(void) {
    switch (D_00525F80.st) {
    case 1: func_003F5F88(); break;
    case 0: break;
    case 2: func_003F6138(); break;
    case 3: if (D_00525F80.cb2(1) != 0) D_00525F80.st = 0; break;
    }
    if (D_00525F80.st == 0) {
        if (D_00525F80.cb2(0) != 0) D_00525F80.st = 3;
        if (D_00525F80.st == 0) {
            D_00525F80.tick = D_00525F80.tick + 1;
            if (D_00525F80.period < D_00525F80.tick) { D_00525F80.tick = 0; func_003F5F08(); } else { func_003F60B8(); }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F64E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

/**
 * Takes a USB keyboard's read buffer when it holds unread data.
 *
 * @param n  keyboard index
 * @return the 0x8C-byte buffer (its ready flag is cleared), or 0 when nothing new arrived
 */
unsigned char *func_003F64E0(int n) {
    int off = n * 140;
    if (D_00525F80.ents[off + 136] == 1) {
        D_00525F80.ents[off + 136] = 0;
        return D_00525F80.ents + off;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6520);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00525F80: the USB keyboard state. D_00525F88 (+8) is the sceUsbKbGetInfo block and D_00526010 (+0x90) the
   max-connect word handed to sceUsbKbInit; ents[] are 0x8C-byte read buffers with a ready flag at +0x88. */
extern struct sKb D_00525F80;

/**
 * Returns whether a USB keyboard is connected (its flag at D_00525F80 +0x10).
 *
 * @param keyboard  keyboard index
 * @return 1 when connected, else 0
 */
int func_003F6520(int keyboard) { return D_00525F80.v[keyboard] != 0; }
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F6538);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003F63F0(void);
extern int func_003F6520(int);
extern int func_003F6348(void);
/**
 * Runs the USB keyboard reader once (func_003F63F0) and returns 1 when any keyboard is connected, else 0.
 *
 * Nothing in retail calls it.
 *
 * @return 1 when a keyboard is connected
 */
int func_003F6538(void) {
    int i;
    func_003F63F0();
    for (i = 0; i < func_003F6348(); i++) {
        if (func_003F6520(i) == 1) return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F65A0);
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
short *func_003F65A0(unsigned int keyCode) {
    if (keyCode - 46 < 8) return &D_004513DC[keyCode];
    if (keyCode - 135 >= 5) return &D_00451350[keyCode];
    return &D_0045133A[keyCode];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F65F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern short D_00451420[], D_00451350[];
extern short *func_003F65A0(unsigned int);
/**
 * Translates a USB key code to its virtual-key byte (byte 1 of its key-map entry).
 *
 * With Num Lock on (bit 0 of the LED word at @p flags) the keypad codes 89..99 use D_00451420; otherwise map mode 0
 * uses the default map D_00451350 and any other mode the alternate one (func_003F65A0).
 *
 * @param out  receives the virtual-key byte
 * @param flags  the keyboard read buffer (LED word first)
 * @param mode  key-map mode
 * @param keyCode  USB HID key code
 */
void func_003F65F8(char *out, char *flags, int mode, int keyCode) {
    unsigned char *mapEntry;
    unsigned int keypadIndex;
    if ((*(int *)flags & 1) && (keypadIndex = keyCode - 89, keypadIndex < 11)) {
        mapEntry = (unsigned char *)&D_00451420[keypadIndex];
    } else if (mode == 0) {
        mapEntry = (unsigned char *)&D_00451350[keyCode];
    } else {
        mapEntry = (unsigned char *)func_003F65A0(keyCode);
    }
    *out = mapEntry[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6670);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_00451458[];
extern int f268(int, int, int, int) __asm__("sceUsbKbCnvRawCode");
void func_003F6670_impl(unsigned char *self, int *fl, int kb, unsigned code) __asm__("func_003F6670");
/**
 * Translates a USB key code to its character, stored in byte 1 of the output pair @p self.
 *
 * Keypad codes 84..99 come from D_00451458 (its first four are the "/", "*", "-" and "+" keys); the digit and period
 * keys (89 and up) give 0 unless Num Lock (bit 0 of @p fl[0]) is on. Other codes go through sceUsbKbCnvRawCode with
 * the layout @p kb, the modifier word @p fl[1] and the LED word; results of 0x100 or more give 0.
 *
 * @param fl  the keyboard read buffer: LED word, modifier word
 * @param kb  keyboard layout passed to sceUsbKbCnvRawCode
 * @param code  USB HID key code
 */
void func_003F6670_impl(unsigned char *self, int *fl, int kb, unsigned code) {
    unsigned keypadIndex = code - 84;
    if (keypadIndex < 0x10) {
        if (code < 89 || (fl[0] & 1)) self[1] = D_00451458[keypadIndex];
        else self[1] = 0;
    } else {
        int converted = f268(kb, fl[1], fl[0], code & 0xFFFF);
        if (converted < 0x100) self[1] = converted;
        else self[1] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6700);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F6700_impl(unsigned flags, unsigned key, unsigned *out) __asm__("func_003F6700");
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
int func_003F6700_impl(unsigned flags, unsigned key, unsigned *out) {
    int ok = 1;
    unsigned modifierBits = 0;
    switch (key) {
    case 91: modifierBits = flags & 0x8; break;
    case 163: modifierBits = flags & 0x10; break;
    case 161: modifierBits = flags & 0x20; break;
    case 160:
    case 0x10: modifierBits = flags & 0x2; break;
    case 18:
    case 164: modifierBits = flags & 0x4; break;
    case 17: modifierBits = flags & 0x11; break;
    case 92: modifierBits = flags & 0x80; break;
    case 162: modifierBits = flags & 0x1; break;
    case 165: modifierBits = flags & 0x40; break;
    default: ok = 0; break;
    }
    if (ok) *out = modifierBits;
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F67E0);
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
int func_003F67E0(int lockFlags, unsigned int mode, int *out) {
    int handled = 1;
    int lockState = 0;
    switch (mode) {
    case 144: lockState = lockFlags & 1; break;
    case 20: lockState = lockFlags & 2; break;
    case 145: lockState = lockFlags & 4; break;
    default: handled = 0; break;
    }
    if (handled) *out = lockState;
    return handled;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6838);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int a; int b; int pad; unsigned short t[1]; } S6838;
extern int f6700(int, int, int *) __asm__("func_003F6700");
extern int f67E0(int, int, int *) __asm__("func_003F67E0");
extern void f65F8(char *, S6838 *, int, int) __asm__("func_003F65F8");
/**
 * Returns whether a virtual key is down in a USB keyboard read buffer.
 *
 * Modifier keys are tested in the modifier word (func_003F6700) and lock keys in the LED word (func_003F67E0); any
 * other key compares the virtual-key byte of the buffer's @p idx-th key code (func_003F65F8) with @p virtKey. The
 * keyboard reader update (func_002668E8) asks it for VK_SHIFT (0x10).
 *
 * @param kbdState  the read buffer: LED word, modifier word, count, key codes
 * @param mapMode  key-map mode for func_003F65F8
 * @param idx  index of the key code to test
 * @param virtKey  virtual-key code
 * @return nonzero when the key is down
 */
int func_003F6838(S6838 *kbdState, int mapMode, int idx, int virtKey) {
    int res = 0;
    char buf[8];
    int key = kbdState->t[idx];
    if (f6700(kbdState->b, virtKey, &res) == 0) {
        if (f67E0(kbdState->a, virtKey, &res) == 0) {
            f65F8(buf, kbdState, mapMode, key);
            if ((unsigned char)buf[0] == virtKey) res = 1;
        }
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F68F0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004A5B10;
extern void func_003F65F8(char *, char *, int, int);
extern void func_003F6670(char *, char *, int, int);
/**
 * Translates one key code of a USB keyboard read buffer into the static pair D_004A5B10: virtual-key byte
 * (func_003F65F8) and character (func_003F6670).
 *
 * @param readBuf  the read buffer (key codes from +0xC)
 * @param mapMode  key-map mode / layout
 * @param keyIndex  index of the key code
 * @return D_004A5B10 (overwritten by the next call)
 */
char *func_003F68F0(char *readBuf, int mapMode, int keyIndex) {
    unsigned int keyCode = *(unsigned short *)(readBuf + (keyIndex << 1) + 12);
    func_003F65F8(&D_004A5B10, readBuf, mapMode, keyCode);
    func_003F6670(&D_004A5B10, readBuf, mapMode, keyCode);
    return &D_004A5B10;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6960);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* SIF RPC client (func_003F6A40 binds it): D_004A5B14 = bound, D_00526100 the client data, D_00526080 the
   send buffer. */
extern unsigned char D_004A5B14;

extern int D_00526100[];

struct sRpcSend { int a; int b; };

extern struct sRpcSend D_00526080[];

extern int f6F08(int *) __asm__("sceSifCheckStatRpc");
extern int f6D18(int *, int, int, struct sRpcSend *, int, int, int, int, int) __asm__("sceSifCallRpc");
void func_003F6960_impl(int a, int b, int c, int d, int e) __asm__("func_003F6960");
/**
 * Calls the voice-chat IOP module (VOIPF) through its SIF RPC client.
 *
 * Only when bound (D_004A5B14): waits for the previous call to finish (sceSifCheckStatRpc), puts two words in the send
 * buffer D_00526080 and calls RPC function @p rpcFunc (sceSifCallRpc on the client D_00526100, 0x80-byte send buffer),
 * retrying until the call is accepted.
 *
 * @param rpcFunc  RPC function number
 * @param argWord0  first argument word
 * @param argWord1  second argument word
 * @param recvBuf  receive buffer
 * @param recvSize  receive buffer size
 */
void func_003F6960_impl(int rpcFunc, int argWord0, int argWord1, int recvBuf, int recvSize) {
    struct sRpcSend *sendBuf = D_00526080;
    if (D_004A5B14) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F6A40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* SIF RPC client (func_003F6A40 binds it): D_004A5B14 = bound, D_00526100 the client data, D_00526080 the
   send buffer. */
extern unsigned char D_004A5B14;

extern int D_00526100[];

extern int D_005261C0[];

extern int D_00526140[];

extern int *D_004A5B18;

extern int sceSifBindRpc(int *, int, int);
extern void func_003F6960b(int, int, int, int *, int) __asm__("func_003F6960");
/**
 * Binds the voice-chat SIF RPC client (server id 567, sceSifBindRpc) and registers the status word with the module.
 *
 * Marks the client bound first, keeps an uncached pointer to D_005261C0 in D_004A5B18 and sends it with RPC function 0.
 * initOnline calls it when VOIPF.IRX loaded. Quirk: the bind result is not checked.
 */
void func_003F6A40(void) {
    int *statusWord;
    D_004A5B14 = 1;
    sceSifBindRpc(D_00526100, 567, 0);
    statusWord = (int *)(((int)D_005261C0 & 0xFFFFFFF) | 0x20000000);
    D_004A5B18 = statusWord;
    func_003F6960b(0, (int)statusWord, 0, D_00526140, 0x80);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6AB0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* SIF RPC client (func_003F6A40 binds it): D_004A5B14 = bound, D_00526100 the client data, D_00526080 the
   send buffer. */
extern unsigned char D_004A5B14;

/**
 * Marks the voice-chat RPC client unbound (D_004A5B14 = 0), which turns every later call into a no-op.
 */
void func_003F6AB0(void) { D_004A5B14 = 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6AB8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526140[];

extern int func_003F6960(int, int, int, int *, int);
/**
 * Connects voice chat to a peer: RPC function 2 with the address and the two 16-bit ports packed in one word.
 *
 * The voice manager (func_00266D58) passes ports 6000 / 6000.
 *
 * @param peerAddr  peer address
 * @param firstPort  first port (low half)
 * @param secondPort  second port (high half)
 * @return no meaningful value (func_003F6960 returns nothing)
 */
int func_003F6AB8(int peerAddr, unsigned short firstPort, int secondPort) { return func_003F6960(2, peerAddr, firstPort | (secondPort << 16), D_00526140, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6AF0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526140[];

extern int func_003F6960(int, int, int, int *, int);
/**
 * Stops voice chat: RPC function 3.
 *
 * @return no meaningful value (func_003F6960 returns nothing)
 */
int func_003F6AF0(void) { return func_003F6960(3, 0, 0, D_00526140, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6B20);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526140[];

extern int func_003F6960(int, int, int, int *, int);
/**
 * Turns the voice on: RPC function 4.
 */
void func_003F6B20() { func_003F6960(4, 0, 0, D_00526140, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6B50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526140[];

extern int func_003F6960(int, int, int, int *, int);
/**
 * Turns the voice off: RPC function 5.
 *
 * @return no meaningful value (func_003F6960 returns nothing)
 */
int func_003F6B50(void) { return func_003F6960(5, 0, 0, D_00526140, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6B80);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6C40(void);
/**
 * Returns bit 0 of the voice module's status word (func_003F6C40).
 *
 * @return the bit
 */
int func_003F6B80(void) { return func_003F6C40() & 1; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6BA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526140[];

extern void func_003F6960b(int, int, int, int *, int) __asm__("func_003F6960");
/**
 * Reads the voice level and its maximum from the voice module: RPC function 7, then the first two received words.
 *
 * @param levelOut  receives the level
 * @param maxOut  receives the maximum
 */
void func_003F6BA0(int *levelOut, int *maxOut) {
    func_003F6960b(7, 0, 0, D_00526140, 0x80);
    *levelOut = D_00526140[0];
    *maxOut = D_00526140[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6C10);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526140[];

extern int func_003F6960(int, int, int, int *, int);
/**
 * Sets the voice level: RPC function 6 with the level and the maximum.
 *
 * @param level  voice level
 * @param maxLevel  maximum level
 */
void func_003F6C10(int level, int maxLevel) { func_003F6960(6, level, maxLevel, D_00526140, 0x80); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6C40);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int *D_004A5B18;

/**
 * Returns the voice module's status word, which the IOP writes into D_005261C0 (read through the uncached pointer
 * D_004A5B18).
 *
 * @return the status word
 */
int func_003F6C40(void) { return *D_004A5B18; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6C50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004A4BC0;

/**
 * Sets the network-account library's started flag D_004A4BC0 (func_003F7EE0 calls it first).
 *
 * @return 0
 */
int func_003F6C50(void) { D_004A4BC0 = 1; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6C60);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004A4BC0;

/**
 * Clears the network-account library's started flag D_004A4BC0 (func_003F7F18 calls it last).
 *
 * @return 0
 */
int func_003F6C60(void) { D_004A4BC0 = 0; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6C70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sMcParams { int w[25]; };

/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
struct sNetHdr { int w[17]; };

struct sNetPhase {
    unsigned mode;                      /* +0x00 */
    void *fDC;
    void *fE0;
    void *fE4;
    void *fE8;
    int fEC;
    void *fF0;
    void *fF4;
    void *fF8;
    void *fFC;
    void *f100;
};

struct sNetAcct {
    struct sNetHdr hdr;                 /* 0x00 */
    struct sMcParams params;            /* 0x44 */
    char name[0x11];                    /* 0xA8 */
    char pass[0x11];                    /* 0xB9 */
    char *out;                          /* 0xCC */
    unsigned st;                        /* 0xD0 */
    void *fD4;
    struct sNetPhase cur;               /* 0xD8 */
    unsigned f104;
    int f108;
    struct sNetPhase saved;             /* 0x10C */
    int f138;
    int f13C;
    int heap;                           /* 0x140 */
    char *f144;
    int f148;
};

extern struct sNetAcct *D_004A4BC4;

extern struct sNetAcct *f6728(int, int, int) __asm__("func_00266728");
extern unsigned f6210x(struct sNetAcct *, int, int) __asm__("memset");
int func_003F6C70_impl(int *arg) __asm__("func_003F6C70");
/**
 * Creates the network-account session D_004A4BC4 (0x14C bytes).
 *
 * Allocates it from the heap @p initParams[1] with func_00266728 and zeroes it, keeps the heap (+0x140), copies the 17
 * memory-card callbacks from the table @p initParams[0] into its head, and sets it idle: operation 4 (+0xD0, +0x104), mode 10
 * (+0xD8), phase 5 (+0xEC), +0x148 and +0x138 0, +0x13C -1 (so +0x138 is the port list {0, -1} for func_003F6DC8).
 *
 * @param initParams  {callback table, heap}
 * @return 0
 */
int func_003F6C70_impl(int *initParams) {
    struct sNetAcct *session;
    struct sNetAcct *sessionReload;
    D_004A4BC4 = f6728(initParams[1], 332, 4);
    f6210x(D_004A4BC4, 0, 332);
    session = D_004A4BC4;
    session->heap = initParams[1];
    session->hdr = *(struct sNetHdr *)initParams[0];
    sessionReload = D_004A4BC4;
    sessionReload->st = 4;
    sessionReload->f104 = 4;
    sessionReload->f148 = 0;
    sessionReload->f138 = 0;
    sessionReload->cur.mode = 10;
    sessionReload->cur.fEC = 5;
    sessionReload->f13C = -1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6D98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_00266760(int, char *, int, int);
/**
 * Frees the network-account session D_004A4BC4 back to its heap (func_00266760) and clears the pointer.
 *
 * @return 0
 */
int func_003F6D98(void) {
    func_00266760(D_004A4BC4->heap, (char *)D_004A4BC4, 332, 4);
    D_004A4BC4 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6DC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

typedef struct { char b[0x1A]; } K6DC8;
extern K6DC8 D_00497060;
extern int fB800(void) __asm__("func_003FB800");
extern int fC0E8(void *) __asm__("func_003FC0E8");
extern int func_003F8178_6DC8(int) __asm__("func_003F8178");
typedef struct { int a; int *p; struct sNetAcct *d; K6DC8 *buf; int len; int r; int v; } R6DC8;
/**
 * Opens the save-record manager for the network-account session (func_003FC0E8).
 *
 * Passes: at most 2 records, the port list at +0x138 (port 0 only), the session's 17 card callbacks, the 26-byte cipher
 * key "Copyright Electronic Arts" (with its NUL), the XOR cipher interface (func_003FB800) and the session's heap. The
 * result goes through func_003F8178.
 *
 * @return the mapped error, 0 on success
 */
int func_003F6DC8(void) {
    K6DC8 buf;
    R6DC8 req;
    struct sNetAcct *session = D_004A4BC4;
    buf = D_00497060;
    req.a = 2;
    req.p = &session->f138;
    req.d = session;
    req.len = 26;
    req.buf = &buf;
    req.r = fB800();
    req.v = D_004A4BC4->heap;
    return func_003F8178_6DC8(fC0E8(&req));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6E70);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FC260(void);
extern int func_003F8178(int);
/**
 * Closes the save-record manager (func_003FC260) and returns the error mapped by func_003F8178.
 *
 * @return the mapped error, 0 on success
 */
int func_003F6E70(void) { return func_003F8178(func_003FC260()); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6E98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003F7990(int);
/**
 * Starts a network-account operation when the session is idle (operation word +0xD0 == 4), with func_003F7990.
 *
 * Operations: 0 create the account save, 1 save the name and password, 2 load them, 3 delete the save.
 *
 * @param operation  the operation
 * @return 0, or 22 when an operation is already running
 */
int func_003F6E98(int operation) {
    int errorCode = 0;
    if (D_004A4BC4->st == 4) {
        func_003F7990(operation);
    } else {
        errorCode = 22;
    }
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6EE0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/**
 * Advances the running network-account operation (installed at +0xD4 by func_003F7990, called by func_003F80E0).
 *
 * Returns 0xF when idle. In a card mode (below 9) it runs the step function (+0xDC, func_003F6F68); in mode 9 it calls
 * the dispatcher (+0xE8, func_003F7328) to choose the first mode. Likely bug: *@p out is cleared before @p out is
 * checked for 0.
 *
 * @param out  receives 1 while running, 2 when the operation has finished
 * @return 0 or an error code
 */
int func_003F6EE0(int *out) {
    int err = 0; struct sNetAcct *session;
    *out = 0;
    if (out == 0) err = 7;
    session = D_004A4BC4;
    if (session->st >= 4) err = 0xF;
    if (err == 0) {
        if (D_004A4BC4->cur.mode < 9) err = ((int (*)(int *, struct sNetAcct *))session->cur.fDC)(out, session);
        else ((void (*)(int *, int *))D_004A4BC4->cur.fE8)(out, &err);
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6F68);
#ifdef SKIP_ASM
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;
extern int func_003F7D98(void);
typedef struct { unsigned char b[0x18]; } B6F68;
/**
 * Runs one step of the current network-account mode through its phase (+0xEC).
 *
 * Phase 4 begins (saving the phase block to +0x10C) and moves to 0; phase 0 calls the start function (+0xF0), then goes
 * to 1, or on failure to 2, running the step once more for its cleanup and finishing (func_003F7D98); phase 1 polls
 * (+0xF4, func_003F7708) and finishes when the card work is done (@p out 2), failing like phase 0; phase 2 calls +0xF8;
 * phase 5 returns 0x10, anything else 17. Then a finished step calls the next-mode handler (+0xE0) and a failed one the
 * error handler (+0xE4). Likely bug: *@p out is written before @p out is checked for 0.
 *
 * @param out  receives 1 while running, 2 when the mode finished
 * @return 0 or an error code
 */
int func_003F6F68(int *out) {
    int startFailOut;
    int st;
    int pollFailOut;
    int result;
    result = 0;
    *out = 0;
    if (out == 0) result = 7;
    if (D_004A4BC4->cur.mode >= 9) {
        if (D_004A4BC4->cur.mode == 10) result = 0xC;
        else result = 13;
    }
    if (result == 0) {
        switch (D_004A4BC4->cur.fEC) {
        case 4:
            *(B6F68 *)&D_004A4BC4->saved.fEC = *(B6F68 *)&D_004A4BC4->cur.fEC;
            D_004A4BC4->cur.fEC = 0;
            *out = 1;
            break;
        case 0:
            result = ((int (*)(void))D_004A4BC4->cur.fF0)();
            if (result == 0) {
                *(B6F68 *)&D_004A4BC4->saved.fEC = *(B6F68 *)&D_004A4BC4->cur.fEC;
                D_004A4BC4->cur.fEC = 1;
                *out = 1;
            } else {
                *(B6F68 *)&D_004A4BC4->saved.fEC = *(B6F68 *)&D_004A4BC4->cur.fEC;
                D_004A4BC4->cur.fEC = 2;
                ((int (*)(int *))D_004A4BC4->cur.fDC)(&startFailOut);
                func_003F7D98();
            }
            break;
        case 1:
            result = ((int (*)(int *))D_004A4BC4->cur.fF4)(&st);
            if (result == 0) {
                *out = 1;
                if (st == 2) {
                    *out = 2;
                    func_003F7D98();
                }
            } else {
                *(B6F68 *)&D_004A4BC4->saved.fEC = *(B6F68 *)&D_004A4BC4->cur.fEC;
                D_004A4BC4->cur.fEC = 2;
                ((int (*)(int *))D_004A4BC4->cur.fDC)(&pollFailOut);
                func_003F7D98();
            }
            break;
        case 2:
            result = ((int (*)(void))D_004A4BC4->cur.fF8)();
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
        if (*out == 2) ((void (*)(int *, int *))D_004A4BC4->cur.fE0)(out, &result);
    } else {
        ((void (*)(int *, int *))D_004A4BC4->cur.fE4)(out, &result);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F71B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int f7A60c(int, int) __asm__("func_003F7A60");
/**
 * Error handler of network-account modes 0..5: keeps the error in +0x148, clears it and switches to mode 7 (close the
 * save) so the operation reports the error after closing.
 *
 * @param statusOut  receives 1 (still running)
 * @param errorInOut  the error; cleared
 * @return func_003F7A60's result
 */
int func_003F71B8(int *statusOut, int *errorInOut) {
    *statusOut = 1;
    D_004A4BC4->f148 = *errorInOut;
    *errorInOut = 0;
    return f7A60c(7, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F71F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7BD0(void);
/**
 * Error handler of network-account modes 6 and 8: returns the session to idle (func_003F7BD0).
 *
 * @return 0
 */
int func_003F71F8(void) { return func_003F7BD0(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7218);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int f7A60c(int, int) __asm__("func_003F7A60");
int func_003F7218_impl(int *a, int *b) __asm__("func_003F7218");
/**
 * Next-mode handler of network-account modes 0..5: picks the following card step of the operation.
 *
 * Mode 0 (create) goes to 1 (open); mode 1 goes to 2 (write ACCT) for operations 0 and 1 or to 4 (read ACCT) for
 * operation 2; 2 goes to 3 (write PASS), 4 to 5 (read PASS), and 3 and 5 to 6 (close). Enters the new mode with
 * func_003F7A60, saving the current phase block. Any other case fails with 0xC.
 *
 * @param statusOut  receives 1 (still running), or 0 on failure
 * @param errorOut  receives the error on failure
 * @return func_003F7A60's result, or 0xC
 */
int func_003F7218_impl(int *statusOut, int *errorOut) {
    int x; int err = 0xC;
    *statusOut = 1;
    switch (D_004A4BC4->cur.mode) {
    case 0: x = 1; break;
    case 1: { unsigned sel = D_004A4BC4->st;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F72C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7BD0(void);
/**
 * Next-mode handler of network-account modes 6 and 8: returns the session to idle and reports the operation finished.
 *
 * @param statusOut  receives 2 (finished)
 */
void func_003F72C8(int *statusOut) { func_003F7BD0(); *statusOut = 2; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F72F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003F7BD0(void);
/**
 * Handler of network-account mode 7 (closing after an error): hands back the error kept in +0x148, clears it and
 * returns the session to idle.
 *
 * @param statusOut  status output (not written)
 * @param errorOut  receives the kept error
 * @return func_003F7BD0's result (0)
 */
int func_003F72F8(int statusOut, int *errorOut) {
    *errorOut = D_004A4BC4->f148;
    D_004A4BC4->f148 = 0;
    return func_003F7BD0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7328);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int f7A60c(int, int) __asm__("func_003F7A60");
int func_003F7328_impl(int *a, int *b) __asm__("func_003F7328");
/**
 * Mode-9 dispatcher of the network-account session: enters the first card mode of the operation.
 *
 * Operation 0 starts with mode 0 (create the save), operations 1 and 2 with mode 1 (open it), operation 3 with mode 8
 * (delete it); entered with func_003F7A60, saving the phase block. Outside mode 9 or for another operation it fails
 * with 0xC.
 *
 * @param statusOut  receives 1 (still running), or 0 on failure
 * @param errorOut  receives the error on failure
 * @return func_003F7A60's result, or 0xC
 */
int func_003F7328_impl(int *statusOut, int *errorOut) {
    int ret;
    if (D_004A4BC4->cur.mode == 9) {
        *statusOut = 1;
        switch (D_004A4BC4->st) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F73A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern char str_slogin[];

extern int func_003F77F8(void);
extern int func_003FC390(char *, int, int);
extern int func_003F8178_73A8(int) __asm__("func_003F8178");
/**
 * Start function of network-account mode 0: creates the "slogin" save on any port (func_003FC390), with func_003F77F8
 * as completion.
 *
 * @return the error mapped by func_003F8178
 */
int func_003F73A8(void) {
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = func_003F77F8;
    session->cur.f100 = 0;
    return func_003F8178_73A8(func_003FC390(str_slogin, -1, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F73F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern char str_slogin[];

extern int func_003F7820_73F0(void) __asm__("func_003F7820");
extern int func_003FC4C8_73F0(int, int, int) __asm__("func_003FC4C8");
extern int func_003F8178_73F0(int) __asm__("func_003F8178");
/**
 * Start function of network-account mode 1: opens the "slogin" save on any port (func_003FC4C8), with func_003F7820 as
 * completion.
 *
 * @return the error mapped by func_003F8178
 */
int func_003F73F0(void) {
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = func_003F7820_73F0;
    session->cur.f100 = 0;
    return func_003F8178_73F0(func_003FC4C8_73F0((int)str_slogin, -1, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7438);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern char str_slogin[];

extern int func_003FC3C0(char *, int, int);
extern int func_003F8178_7438(int) __asm__("func_003F8178");
/**
 * Start function of network-account mode 8: deletes the "slogin" save on any port (func_003FC3C0), without a
 * completion.
 *
 * @return the error mapped by func_003F8178
 */
int func_003F7438(void) {
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = 0;
    session->cur.f100 = 0;
    return func_003F8178_7438(func_003FC3C0(str_slogin, -1, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7478);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/* A 0x11-byte name copied as a block. */
struct sName17 { char b[17]; };

extern char fn7920[] __asm__("func_003F7920");
extern char fn78E0[] __asm__("func_003F78E0");
extern int func_003FCE10(char *, int, int, int);
extern int fC548(struct sMcParams *, int, int, int, int) __asm__("func_003FC548");
extern int func_003F8178_7478(int) __asm__("func_003F8178");
/**
 * Start function of network-account mode 2: writes the user name as the save's 'ACCT' record.
 *
 * Allocates a 17-byte record buffer (func_003FCE10, into +0x144), copies the name (+0xA8) into it and starts the write
 * (func_003FC548 with the open save's parameters at +0x44). Completion func_003F78E0 and error cleanup func_003F7920
 * free the buffer.
 *
 * @return the error mapped by func_003F8178
 */
int func_003F7478(void) {
    int result;
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = fn78E0;
    session->cur.f100 = fn7920;
    result = func_003F8178_7478(func_003FCE10((char *)&session->f144, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = D_004A4BC4;
        struct sName17 *dst = (struct sName17 *)sessionReload->f144;
        *dst = *(struct sName17 *)sessionReload->name;
        result = func_003F8178_7478(fC548(&D_004A4BC4->params, 0x41434354, 0, (int)D_004A4BC4->f144, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7528);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/* A 0x11-byte name copied as a block. */
extern char fn7958[] __asm__("func_003F7958");
extern char fn7900[] __asm__("func_003F7900");
extern int func_003FCE10(char *, int, int, int);
extern int fC548(struct sMcParams *, int, int, int, int) __asm__("func_003FC548");
extern int func_003F8178_7528(int) __asm__("func_003F8178");
/**
 * Start function of network-account mode 3: writes the password as the save's 'PASS' record.
 *
 * Like func_003F7478 with the password (+0xB9); completion func_003F7900 and cleanup func_003F7958 free the buffer.
 *
 * @return the error mapped by func_003F8178
 */
int func_003F7528(void) {
    int result;
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = fn7900;
    session->cur.f100 = fn7958;
    result = func_003F8178_7528(func_003FCE10((char *)&session->f144, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = D_004A4BC4;
        struct sName17 *dst = (struct sName17 *)sessionReload->f144;
        *dst = *(struct sName17 *)sessionReload->pass;
        result = func_003F8178_7528(fC548(&D_004A4BC4->params, 0x50415353, 0, (int)D_004A4BC4->f144, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F75D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern char fn7920[] __asm__("func_003F7920");
extern char fn7840[] __asm__("func_003F7840");
extern int func_003FCE10(char *, int, int, int);
extern int func_003FC708(struct sMcParams *, int, int, int, int);
extern int func_003F8178_75D8(int) __asm__("func_003F8178");
/**
 * Start function of network-account mode 4: reads the save's 'ACCT' record into a new 17-byte buffer (+0x144) with
 * func_003FC708.
 *
 * Completion func_003F7840 copies it to the caller's output; cleanup func_003F7920 frees it.
 *
 * @return the error mapped by func_003F8178
 */
int func_003F75D8(void) {
    int result;
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = fn7840;
    session->cur.f100 = fn7920;
    result = func_003F8178_75D8(func_003FCE10((char *)&session->f144, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = D_004A4BC4;
        result = func_003F8178_75D8(func_003FC708(&sessionReload->params, 0x41434354, 0, (int)sessionReload->f144, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7658);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern char fn7958[] __asm__("func_003F7958");
extern char fn7890[] __asm__("func_003F7890");
extern int func_003FCE10(char *, int, int, int);
extern int func_003FC708(struct sMcParams *, int, int, int, int);
extern int func_003F8178_7658(int) __asm__("func_003F8178");
/**
 * Start function of network-account mode 5: reads the save's 'PASS' record into a new 17-byte buffer (+0x144) with
 * func_003FC708.
 *
 * Completion func_003F7890 copies it to the caller's output + 0x11; cleanup func_003F7958 frees it.
 *
 * @return the error mapped by func_003F8178
 */
int func_003F7658(void) {
    int result;
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = fn7890;
    session->cur.f100 = fn7958;
    result = func_003F8178_7658(func_003FCE10((char *)&session->f144, session->heap, 17, 0));
    if (result == 0) {
        struct sNetAcct *sessionReload = D_004A4BC4;
        result = func_003F8178_7658(func_003FC708(&sessionReload->params, 0x50415353, 0, (int)sessionReload->f144, 17));
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F76D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003FC4E8(char *);
extern int func_003F8178_76D8(int) __asm__("func_003F8178");
/**
 * Start function of network-account modes 6 and 7: closes the open save (func_003FC4E8 with its parameters at +0x44).
 *
 * @return the error mapped by func_003F8178
 */
int func_003F76D8(void) {
    struct sNetAcct *session = D_004A4BC4;
    session->cur.fFC = 0;
    session->cur.f100 = 0;
    return func_003F8178_76D8(func_003FC4E8((char *)&session->params));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7708);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003FCA40(int *, int *);
extern int func_003F8178_7708(int) __asm__("func_003F8178");
/**
 * Poll function of every network-account card mode: polls the save-record manager (func_003FCA40).
 *
 * Status 1 (busy) is passed on; status 2 (done) first runs the mode's completion (+0xFC) once; status 0 gives error 23
 * and anything else 18. When the poll itself fails, the mode's cleanup (+0x100) runs once instead.
 *
 * @param out  receives 1 while busy, 2 when done
 * @return 0 or the mapped error
 */
int func_003F7708(int *out) {
    int st[2];
    int err;
    unsigned pollStatus;
    st[0] = 0;
    err = out ? 0 : 7;
    if (err == 0)
        err = func_003F8178_7708(func_003FCA40(st, st + 1));
    if (err == 0) {
        pollStatus = st[0];
        *out = 0;
        switch (pollStatus) {
        case 0:
            err = 23;
            break;
        case 2:
            if (D_004A4BC4->cur.fFC) {
                err = ((int (*)(void))D_004A4BC4->cur.fFC)();
                D_004A4BC4->cur.fFC = 0;
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
        if (D_004A4BC4->cur.f100) {
            err = ((int (*)(void))D_004A4BC4->cur.f100)();
            D_004A4BC4->cur.f100 = 0;
        }
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F77F0);
#ifdef SKIP_ASM
/**
 * Phase-2 function of the network-account card modes: does nothing.
 *
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F77F0(void) { return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F77F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003FC990(struct sMcParams *);
extern int func_003F8178_77F8(int) __asm__("func_003F8178");
/**
 * Completion of network-account mode 0: copies the save-record manager's open-save parameters (func_003FC990) into the
 * session (+0x44).
 *
 * @return the error mapped by func_003F8178
 */
int func_003F77F8(void) { return func_003F8178_77F8(func_003FC990(&D_004A4BC4->params)); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7820);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F77F8(void);
/**
 * Completion of network-account mode 1: the same as func_003F77F8 (stores the open-save parameters).
 *
 * @return the error mapped by func_003F8178
 */
int func_003F7820(void) { return func_003F77F8(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7840);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/* A 0x11-byte name copied as a block. */
extern int func_003F7920(int);
/**
 * Completion of network-account mode 4: copies the 17 bytes read for 'ACCT' to the caller's output (+0xCC) and frees
 * the record buffer (func_003F7920).
 */
void func_003F7840(void) {
    struct sName17 *dst = (struct sName17 *)D_004A4BC4->out;
    *dst = *(struct sName17 *)D_004A4BC4->f144;
    func_003F7920(0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7890);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/* A 0x11-byte name copied as a block. */
extern int func_003F7958(int);
/**
 * Completion of network-account mode 5: copies the 17 bytes read for 'PASS' to the caller's output + 0x11 and frees the
 * record buffer (func_003F7958).
 */
void func_003F7890(void) {
    struct sName17 *dst = (struct sName17 *)(D_004A4BC4->out + 0x11);
    *dst = *(struct sName17 *)D_004A4BC4->f144;
    func_003F7958(0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F78E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7920(int);
/**
 * Completion of network-account mode 2: frees the record buffer (func_003F7920).
 *
 * @return the error mapped by func_003F8178
 */
int func_003F78E0(void) { return func_003F7920(0); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7900);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7958(int);
/**
 * Completion of network-account mode 3: frees the record buffer (func_003F7958).
 *
 * @return the error mapped by func_003F8178
 */
int func_003F7900(void) { return func_003F7958(0); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7920);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003FCF00(int, int, int, int);
extern int func_003F8178_7920(int) __asm__("func_003F8178");
/**
 * Frees the network-account record buffer (+0x144, 17 bytes) with func_003FCF00. Used for the 'ACCT' modes.
 *
 * @param unused  ignored (callers pass 0)
 * @return the error mapped by func_003F8178
 */
int func_003F7920(int unused) { return func_003F8178_7920(func_003FCF00((int)D_004A4BC4->f144, D_004A4BC4->heap, 17, 0)); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7958);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003FCF00(int, int, int, int);
extern int func_003F8178_7958(int) __asm__("func_003F8178");
/**
 * Frees the network-account record buffer (+0x144, 17 bytes) with func_003FCF00; identical to func_003F7920 and used
 * for the 'PASS' modes.
 *
 * @param unused  ignored (callers pass 0)
 * @return the error mapped by func_003F8178
 */
int func_003F7958(int unused) { return func_003F8178_7958(func_003FCF00((int)D_004A4BC4->f144, D_004A4BC4->heap, 17, 0)); }
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F7990);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern char fn6EE0[] __asm__("func_003F6EE0");
extern char fn7328[] __asm__("func_003F7328");
int f7990(unsigned a) __asm__("func_003F7990");
/**
 * Begins network-account operation @p operation.
 *
 * Stores it in +0xD0 and +0x104, installs the operation poll func_003F6EE0 at +0xD4 (only for operations below 4), and
 * sets mode 9 (choose the first card step) with phase 4, the dispatcher func_003F7328 at +0xE8 and every other handler
 * cleared.
 *
 * @param operation  operation (0 create, 1 save, 2 load, 3 delete)
 * @return 0, or 0xF for an unknown operation
 */
int f7990(unsigned operation) {
    int errorCode = 0;
    unsigned i;
    D_004A4BC4->st = operation;
    D_004A4BC4->f104 = operation;
    if (operation < 4) {
        D_004A4BC4->fD4 = fn6EE0;
    } else {
        errorCode = 0xF;
    }
    D_004A4BC4->cur.mode = 9;
    D_004A4BC4->cur.fEC = 4;
    D_004A4BC4->cur.fE8 = fn7328;
    D_004A4BC4->cur.fE0 = 0;
    D_004A4BC4->cur.fE4 = 0;
    D_004A4BC4->cur.fDC = 0;
    for (i = 0; i < 3; i++)
        ;
    D_004A4BC4->cur.fF0 = 0;
    D_004A4BC4->cur.fF4 = 0;
    D_004A4BC4->cur.fF8 = 0;
    D_004A4BC4->cur.fFC = 0;
    D_004A4BC4->cur.f100 = 0;
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7A30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003F7BD0(void);
/**
 * Ends the network-account operation: marks the session idle (operation 4), removes the operation poll and the
 * dispatcher and returns to mode 10 (func_003F7BD0).
 *
 * @return 0
 */
int func_003F7A30(void) {
    struct sNetAcct *session = D_004A4BC4;
    session->st = 4;
    session->fD4 = 0;
    session->cur.fE8 = 0;
    return func_003F7BD0();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F7A60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

// (prototype from their definition at line 1929)
int func_003F71F8_7A60(void) __asm__("func_003F71F8");

// (prototype from their definition at line 1917)
int func_003F71B8(int *a, int *b);

// (prototype from their definition at line 1960)
void func_003F72C8_7A60(int *a) __asm__("func_003F72C8");

// (prototype from their definition at line 1967)
int func_003F72F8(int a, int *b);

extern int func_003F6F68();
extern int func_003F7218();
extern int func_003F7C00(void);
/**
 * Enters a network-account card mode.
 *
 * Sets the mode (+0xD8), optionally saving the whole phase block to +0x10C, installs the step function func_003F6F68,
 * the next-mode handler (modes 0..5 func_003F7218, 6 and 8 func_003F72C8, 7 func_003F72F8) and the error handler (0..5
 * func_003F71B8, 6 and 8 func_003F71F8, 7 func_003F72F8), then the mode's card functions (func_003F7C00). Modes: 0
 * create the save, 1 open it, 2 / 3 write ACCT / PASS, 4 / 5 read them, 6 and 7 close, 8 delete.
 *
 * @param mode  the mode
 * @param copy  nonzero to save the phase block first
 * @return 0, or 13 for a mode of 9 or more
 */
int func_003F7A60(int mode, unsigned char copy) {
    int err = 0;
    D_004A4BC4->cur.mode = mode;
    if (copy)
        D_004A4BC4->saved = D_004A4BC4->cur;
    if (D_004A4BC4->cur.mode < 9)
        D_004A4BC4->cur.fDC = (void *)func_003F6F68;
    else
        err = 13;
    if (err == 0) {
        switch (D_004A4BC4->cur.mode) {
        case 0: case 1: case 2: case 3: case 4: case 5:
            D_004A4BC4->cur.fE0 = (void *)func_003F7218;
            break;
        case 6: case 8:
            D_004A4BC4->cur.fE0 = (void *)func_003F72C8_7A60;
            break;
        case 7:
            D_004A4BC4->cur.fE0 = (void *)func_003F72F8;
            break;
        default:
            err = 13;
            break;
        }
    }
    if (err == 0) {
        switch (D_004A4BC4->cur.mode) {
        case 0: case 1: case 2: case 3: case 4: case 5:
            D_004A4BC4->cur.fE4 = (void *)func_003F71B8;
            break;
        case 6: case 8:
            D_004A4BC4->cur.fE4 = (void *)func_003F71F8_7A60;
            break;
        case 7:
            D_004A4BC4->cur.fE4 = (void *)func_003F72F8;
            break;
        }
    }
    return func_003F7C00();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7BD0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003F7D98(void);
/**
 * Returns the network-account session to mode 10 (idle) with no step, next-mode or error handler, and finishes the
 * phase (func_003F7D98).
 *
 * @return 0
 */
int func_003F7BD0(void) {
    struct sNetAcct *session = D_004A4BC4;
    session->cur.mode = 10;
    session->cur.fDC = 0;
    session->cur.fE0 = 0;
    session->cur.fE4 = 0;
    return func_003F7D98();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7C00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

// (prototype from their definition at line 2049)
int func_003F7528(void);

// (prototype from their definition at line 1996)
int func_003F73F0(void);

// (prototype from their definition at line 2114)
int func_003F76D8(void);

// (prototype from their definition at line 2025)
int func_003F7478(void);

// (prototype from their definition at line 2169)
int func_003F77F0_7C00(void) __asm__("func_003F77F0");

// (prototype from their definition at line 1982)
int func_003F73A8(void);

// (prototype from their definition at line 2009)
int func_003F7438(void);

// (prototype from their definition at line 2127)
int func_003F7708(int *out);

// (prototype from their definition at line 2095)
int func_003F7658(void);

// (prototype from their definition at line 2073)
int func_003F75D8(void);

typedef struct { unsigned char b[0x18]; } B7C00;
/**
 * Installs the card functions of the current network-account mode and resets its phase to 4.
 *
 * Start function (+0xF0): mode 0 func_003F73A8, 1 func_003F73F0, 2 func_003F7478, 3 func_003F7528, 4 func_003F75D8, 5
 * func_003F7658, 6 and 7 func_003F76D8, 8 func_003F7438. Poll (+0xF4) func_003F7708, phase-2 function (+0xF8)
 * func_003F77F0; completion and cleanup (+0xFC, +0x100) cleared. Also copies the phase word to the saved block.
 *
 * @return 0, or 13 for a mode of 9 or more
 */
int func_003F7C00(void) {
    int errorCode;
    unsigned i;
    i = errorCode = 0;
    D_004A4BC4->cur.fEC = 4;
    *(B7C00 *)&D_004A4BC4->saved.fEC = *(B7C00 *)&D_004A4BC4->cur.fEC;
    for (i = 0; i < 3; i++) ;
    switch (D_004A4BC4->cur.mode) {
    case 0: D_004A4BC4->cur.fF0 = (void *)func_003F73A8; break;
    case 1: D_004A4BC4->cur.fF0 = (void *)func_003F73F0; break;
    case 2: D_004A4BC4->cur.fF0 = (void *)func_003F7478; break;
    case 3: D_004A4BC4->cur.fF0 = (void *)func_003F7528; break;
    case 4: D_004A4BC4->cur.fF0 = (void *)func_003F75D8; break;
    case 5: D_004A4BC4->cur.fF0 = (void *)func_003F7658; break;
    case 6:
    case 7: D_004A4BC4->cur.fF0 = (void *)func_003F76D8; break;
    case 8: D_004A4BC4->cur.fF0 = (void *)func_003F7438; break;
    default: errorCode = 13;
    }
    if (errorCode == 0) {
        if (D_004A4BC4->cur.mode < 9) D_004A4BC4->cur.fF4 = (void *)func_003F7708;
        else errorCode = 13;
        if (errorCode == 0) {
            if (D_004A4BC4->cur.mode < 9) D_004A4BC4->cur.fF8 = (void *)func_003F77F0_7C00;
            else errorCode = 13;
        }
    }
    D_004A4BC4->cur.fFC = 0;
    D_004A4BC4->cur.f100 = 0;
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7D98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/**
 * Finishes the current network-account mode's phase: phase 5, with the start, poll and phase-2 functions, the
 * completion and the cleanup cleared.
 *
 * @return 0
 */
int func_003F7D98(void) {
    unsigned int i;
    int *handlerSlot;
    struct sNetAcct *session = D_004A4BC4;
    struct sNetAcct *sessionReload;
    session->cur.fEC = 5;
    handlerSlot = (int *)&session->cur.fF0;
    for (i = 0; i < 3; i++) {
        *handlerSlot++ = 0;
    }
    sessionReload = D_004A4BC4;
    sessionReload->cur.fFC = 0;
    sessionReload->cur.f100 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7DE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int **p; } W7DE0;
typedef struct { char pad[0xC]; int (*f0)(void); int (*f1)(int, int); int (*f2)(W7DE0 **); } V7DE0;
extern V7DE0 *fB5E8(void) __asm__("func_003FB5E8");
/**
 * Checks the embedded save icon: its 10606 bytes must have the CRC-32 0x0836C71D.
 *
 * Runs the CRC interface from func_003FB5E8 (reset, update over @p iconData, final) and compares the digest.
 *
 * @param iconData  the icon data (view.ico)
 * @return 0 when it matches, 21 when the CRC differs, 0x18 when the CRC interface fails
 */
int func_003F7DE0(int iconData) {
    int err = 0;
    V7DE0 *crcIface = fB5E8();
    int crcStatus;
    W7DE0 *w;
    crcStatus = crcIface->f0();
    if (crcStatus == 0) {
        crcStatus = crcIface->f1(iconData, 10606);
        if (crcStatus == 0) {
            crcStatus = crcIface->f2(&w);
            if (crcStatus == 0) {
                err = 21;
                if (**(unsigned **)((char *)w + 4) == 0x836C71D) err = 0;
            }
        }
    }
    if (crcStatus != 0) return 0x18;
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7E90);
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
char *func_003F7E90(char *dst, char *src, int n) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F7EE0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6C50(void);
extern int func_003F6C70(int);
extern int func_003F6DC8(void);
/**
 * Starts the network-account library: sets its started flag (func_003F6C50), creates the session (func_003F6C70) and
 * opens the save-record manager (func_003F6DC8).
 *
 * The shared-login manager (func_00265950) passes {its 17 memory-card callbacks, 0}.
 *
 * @param initParams  {callback table, heap}
 */
void func_003F7EE0(int initParams) {
    func_003F6C50();
    func_003F6C70(initParams);
    func_003F6DC8();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F7F18);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6E70(void);
extern int func_003F6D98(void);
extern int func_003F6C60(void);
/**
 * Stops the network-account library: closes the save-record manager (func_003F6E70), frees the session (func_003F6D98)
 * and clears the started flag (func_003F6C60). The shared-login manager (func_002659C0) calls it.
 *
 * @return 0
 */
int func_003F7F18(void) {
    func_003F6E70();
    func_003F6D98();
    return func_003F6C60();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7F48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

// (prototype from their definition at line 1882)
int func_003F6E98_7F48(int a) __asm__("func_003F6E98");

extern char* f7E90(char *, int, int) __asm__("func_003F7E90");
extern int func_003F7DE0_7F48(int) __asm__("func_003F7DE0");
extern int f8258(int) __asm__("func_003F8258");
/**
 * Starts creating the network-account save (operation 0) with a user name and password.
 *
 * Copies the name (17 bytes) and password (17 bytes at +0x11) from @p accountBuf into the session, checks the icon data
 * (func_003F7DE0), builds the save description (func_003F8258) and starts the operation (func_003F6E98(0)). Likely bug:
 * it refuses (7) only when both arguments are 0, so a 0 @p accountBuf is copied from.
 *
 * @param iconData  the save icon data (view.ico, 10606 bytes)
 * @param accountBuf  the 34-byte account buffer: name, password at +0x11
 * @return 0 or an error code
 */
int func_003F7F48(int iconData, int accountBuf) {
    int err = 0;
    if (iconData == 0 && accountBuf == 0) {
        err = 7;
    } else {
        f7E90(D_004A4BC4->name, accountBuf, 17);
        f7E90(D_004A4BC4->pass, accountBuf + 17, 17);
    }
    if (err == 0) {
        err = func_003F7DE0_7F48(iconData);
        if (err == 0) {
            err = f8258(iconData);
            if (err == 0) err = func_003F6E98_7F48(0);
        }
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8000);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003F6E98(int);
/**
 * Starts loading the network-account save (operation 2) into a 34-byte buffer: name at +0, password at +0x11.
 *
 * @param accountBuf  the output buffer (kept at +0xCC)
 * @return 0, 7 when @p accountBuf is 0, or 22 when busy
 */
int func_003F8000(char *accountBuf) {
    int errorCode = 0;
    if (accountBuf == 0) {
        errorCode = 7;
    } else {
        D_004A4BC4->out = accountBuf;
    }
    if (errorCode == 0) {
        errorCode = func_003F6E98(2);
    }
    return errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8048);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern char *func_003F7E90_8048(char *, char *, int) __asm__("func_003F7E90");
extern int func_003F6E98(int);
/**
 * Starts saving a user name and password to the network-account save (operation 1).
 *
 * Copies the name and the password (at +0x11) from the buffer into the session first.
 *
 * @param name  the 34-byte account buffer
 * @return 0, 7 when @p name is 0, or 22 when busy
 */
int func_003F8048(char *name) {
    int errorCode = 0;
    if (name == 0) {
        errorCode = 7;
    } else {
        func_003F7E90_8048(D_004A4BC4->name, name, 17);
        func_003F7E90_8048(D_004A4BC4->pass, name + 0x11, 17);
    }
    if (errorCode == 0) errorCode = func_003F6E98(1);
    return errorCode;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F80C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6E98(int);
/**
 * Starts deleting the network-account save (operation 3).
 *
 * @return 0, or 22 when busy
 */
int func_003F80C0(void) { return func_003F6E98(3); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F80E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

extern int func_003F7A30(void);
/**
 * Polls the running network-account operation.
 *
 * Unless idle, runs the operation poll (+0xD4, func_003F6EE0); when it finishes (status 2) or fails, the session goes
 * back to idle (func_003F7A30). The shared-login manager (func_00265A68) calls it every frame.
 *
 * @param out  receives 0 when idle, 1 while running, 2 when finished
 * @return 0 or an error code (7 when @p out is 0)
 */
int func_003F80E0(int *out) {
    int err = 0;
    int status;
    if (out == 0) {
        err = 7;
    } else {
        *out = 0;
        if (D_004A4BC4->st != 4) {
            err = ((int (*)(int *))D_004A4BC4->fD4)(out);
            if (err == 0) {
                status = *out;
                if (status == 2) {
                    func_003F7A30();
                    *out = status;
                }
            } else {
                func_003F7A30();
            }
        }
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8178);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8178(int a);
/**
 * Maps a save-record manager error (card-layer errors are offset by 100) to a network-account error code.
 *
 * 0 to 0; 104 to 1; 103 to 2; 114 to 3; 108 to 4; 3, 7 and 110 to 5; 9 to 6; 1 to 19; 2 to 20; 4 to 7; 6 and 107 to 11;
 * anything else to 0x18.
 *
 * @param sysError  the manager's error
 * @return the account error
 */
int func_003F8178(int sysError) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F8200);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/**
 * Stores the last network-account operation started (+0x104) in *@p operationOut.
 *
 * @param operationOut  output
 * @return 0
 */
int func_003F8200(int *operationOut) { *operationOut = D_004A4BC4->f104; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8218);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BC4: the network account session (0x14C bytes, func_003F6C70). cur / saved are the current and the saved
   phase (mode + handler slots); handler slots hold untyped code addresses (callers cast them). */
extern struct sNetAcct *D_004A4BC4;

/**
 * Stores the mode kept in the saved phase block (+0x10C) in *@p modeOut.
 *
 * @param modeOut  output
 * @return 0
 */
int func_003F8218(int *modeOut) { *modeOut = D_004A4BC4->saved.mode; return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8230);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FCF98(int, int);
/**
 * Stores the on-card size of a 34-byte account record (func_003FCF98) through @p sizeOut.
 *
 * @param sizeOut  output pointer (declared int)
 * @return 0
 */
int func_003F8230(int sizeOut) { func_003FCF98(34, sizeOut); return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8258);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* The memory-card save's icon.sys (Sony libmc sceMcIconSys, 0x3C4 bytes). */
typedef int iconIVECTOR_8258[4];
typedef float iconFVECTOR_8258[4];
typedef struct {
    unsigned char Head[4];
    unsigned short Reserv1;
    unsigned short OffsLF;
    unsigned Reserv2;
    unsigned TransRate;
    iconIVECTOR_8258 BgColor[4];
    iconFVECTOR_8258 LightDir[3];
    iconFVECTOR_8258 LightColor[3];
    iconFVECTOR_8258 Ambient;
    unsigned short TitleName[34];
    unsigned char FnameView[64];
    unsigned char FnameCopy[64];
    unsigned char FnameDel[64];
    unsigned char Reserve3[512];
} sIconSys_8258;
typedef struct {
    int port;
    int id;
    sIconSys_8258 icon;
    int f3CC;
} sMcSaveParam_8258;

extern void *f8258_memset(void *, int, int) __asm__("memset");
/* Constant data this function copies from (string literals and local array initialisers). */
typedef struct { iconFVECTOR_8258 v[3]; } sIconVec3_8258;
typedef struct { iconFVECTOR_8258 v; } sIconVec1_8258;
extern sIconVec3_8258 D_00497320; /* light directions */
extern sIconVec3_8258 D_00497350; /* light colours */
extern sIconVec1_8258 D_00497380; /* ambient */
extern char D_004A4BD0[] __attribute__((aligned(8))); /* "PS2D" (.sdata) */
extern char str_EA_Login[];         /* "EA Login" */
extern char str_view_ico[];         /* "view.ico" */
extern void func_003FA380(char *src, unsigned short *dst);
extern int func_003FCDC8(sMcSaveParam_8258 *);

/**
 * Builds the network-account save's description and hands it to the save-record manager (func_003FCDC8).
 *
 * Fills the icon.sys header: magic "PS2D", title "EA Login" converted to Shift-JIS (func_003FA380), line break at 16,
 * transparency 0x60, two blue background corners, light directions, colours and ambient from D_00497320 / D_00497350 /
 * D_00497380, and "view.ico" as the list, copy and delete icon. Then stores the icon data pointer and size 10606 and
 * the record size 34.
 *
 * @param port  the icon data (not a port; named after the field it fills)
 * @return 0, 7 when @p port is 0, or the error mapped by func_003F8178
 */
int func_003F8258(int port) {
    sMcSaveParam_8258 saveDesc;
    iconIVECTOR_8258 bgcolor[4];
    sIconVec3_8258 lightdir;
    sIconVec3_8258 lightcol;
    sIconVec1_8258 ambient;
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
        func_003FA380(str_EA_Login, saveDesc.icon.TitleName);
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
        saveDesc.f3CC = 34;
        err = func_003F8178(func_003FCDC8(&saveDesc));
    }
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8550);
#ifdef SKIP_ASM
/**
 * Returns the index of the lowest of the low 8 bits set in @p mask (8 when none is).
 *
 * @param mask  slot bit mask
 * @return the slot index
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8550(unsigned mask) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F8588);
#ifdef SKIP_ASM
/**
 * Returns the index of the highest of the low 8 bits set in @p mask (-1 when none is).
 *
 * @param mask  slot bit mask
 * @return the slot index
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8588(unsigned mask) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F85C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
struct sMcReqBody { int port; int slot; int pad08[16]; char name[0x18]; };

struct sMcReq {
    int op;                             /* 0x00: request kind, 0 = none */
    int st;                             /* 0x04: state */
    int f08;
    struct sMcReqBody body;             /* 0x0C */
    int f6C;
    int f70;
};

extern struct sMcReq *D_004A4BD8;

/**
 * Returns the next usable memory-card slot after the request's current one (+0x10 of D_004A4BD8).
 *
 * @param slotMask  usable-slot bit mask
 * @param dir  0 to search upward, 1 downward
 * @return the slot, -1 when there is none, or 18 for another @p dir
 */
int func_003F85C0(unsigned short slotMask, int dir) {
    short i = D_004A4BD8->body.slot;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F86B8);
#ifdef SKIP_ASM
/**
 * Counts the bits set among the low 8 of a usable-unit mask (the number of memory-card units the request layer may
 * use).
 *
 * @param mask  unit bit mask
 * @return the number of units
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F86B8(unsigned short mask) {
    int count = 0; int i = 0;
    for (i = 0; i < 8; i++) { int bit = 1 << i; if (mask & bit) count++; }
    return count;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F86F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

void func_003F86F0_impl(int a) __asm__("func_003F86F0");
/**
 * Records the memory-card request's error (D_004A4BD8 +0x8), unless the request is already one of the cleanup
 * operations 11..13, which keep the error that caused them.
 *
 * @param errorCode  the error (0 clears it)
 */
void func_003F86F0_impl(int errorCode) {
    struct sMcReq *request = D_004A4BD8;
    if ((unsigned)(request->op - 11) >= 3) request->f08 = errorCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8710);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Returns the error recorded for the memory-card request (D_004A4BD8 +0x8, set by func_003F86F0).
 *
 * @return the recorded error
 */
int func_003F8710(void) { return D_004A4BD8->f08; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8720);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
struct sMcDrv {
    unsigned short ports;               /* 0x00: bit per usable slot */
    short pad02;
    int f04;
    int f08;
    int f0C;
    void (*f10)(int);                   /* 0x10 */
    void (*f14)(int);
    int f18;
    void (*f1C)(int);
    int f20;
    int f24;
    int f28;
    void (*f2C)(int, int, int);         /* 0x2C */
    void (*f30)(int, int, int);
    void (*f34)(int, int, int);
    void (*f38)(int);
    int f3C;
    int (*f40)(int *, int *);           /* 0x40 */
    int f44;
    int (**f48)(int, int *, int *);     /* 0x48: by op */
    int (**f4C)(int, int, int);         /* 0x4C: by state */
    char f50[0x3D0];
    int f420;
    int f424;
    int h;                              /* 0x428: heap */
};

extern struct sMcDrv *D_004A4BDC;

/**
 * Starts the cleanup of a failed memory-card request that has a file open: operation 11, state 24, flushing the file
 * (driver callback +0x38 with the descriptor in the request body).
 *
 * func_003F9130 then closes the file and releases the unit, and the request ends with the recorded error.
 *
 * @param reqBody  ignored (callers pass the request body)
 * @param out  receives 1 (still running)
 * @return 0
 */
int func_003F8720(int reqBody, int *out) {
    struct sMcReq *request;
    struct sMcDrv *driver;
    *out = 1;
    request = D_004A4BD8;
    driver = D_004A4BDC;
    request->op = 11;
    request->st = 0x18;
    driver->f38(request->body.port);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8768);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

/**
 * Starts the cleanup of a failed memory-card request at the close step: operation 12, state 25, closing the file
 * (driver callback +0x1C with the descriptor in the request body).
 *
 * @param reqBody  ignored (callers pass the request body)
 * @param out  receives 1 (still running)
 * @return 0
 */
int func_003F8768(int reqBody, int *out) {
    struct sMcReq *request;
    struct sMcDrv *driver;
    *out = 1;
    request = D_004A4BD8;
    driver = D_004A4BDC;
    request->op = 0xC;
    request->st = 25;
    driver->f1C(request->body.port);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F87B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

/**
 * Starts the cleanup of a failed memory-card request at the last step: operation 13, state 26, releasing the card unit
 * (driver callback +0x14).
 *
 * @param unitCode  the card unit code
 * @param out  receives 1 (still running)
 * @return 0
 */
int func_003F87B0(int unitCode, int *out) {
    struct sMcReq *request;
    struct sMcDrv *driver;
    *out = 1;
    request = D_004A4BD8;
    driver = D_004A4BDC;
    request->op = 13;
    request->st = 26;
    driver->f14(unitCode);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F87F8);
#ifdef SKIP_ASM
/**
 * Result check of memory-card request state 0 (entry 0 of the per-state table D_004514A0): accepts any result.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F87F8(int cardResult) { return cardResult; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8800);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 1 (unit check): accepts results 0, 3 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 when the request is not in state 1, or 0x10 for any other result
 */
int func_003F8800(int cardResult) {
    if (D_004A4BD8->st != 1) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8838);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 2 (directory search): accepts results 0, 3, 4, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 when the request is not in state 2, or 0x10 for any other result
 */
int func_003F8838(int cardResult) {
    if (D_004A4BD8->st != 2) return 18;
    if (cardResult != 0 && cardResult != 4 && cardResult != 3 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8880);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of the memory-card read, write and seek states (27, 28, 29 and the create steps 7 and 11): accepts
 * results 0, 3, 4, 9, 10, 13, 14 and 17.
 *
 * Quirk: the per-state table also maps state 15 here, which this check rejects (state 15 is never entered), and it
 * accepts state 24, whose table entry is func_003F8908.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8880(int cardResult) {
    int state = D_004A4BD8->st;
    if (state != 27 && state != 29 && state != 0x1C && state != 7 && state != 11 && state != 0x18) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 0xE && cardResult != 13 && cardResult != 17 && cardResult != 10) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8908);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of the memory-card flush states (8, 12, 16 and 24): accepts results 0, 3, 4, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8908(int cardResult) {
    int state = D_004A4BD8->st;
    if (state != 0x18 && state != 8 && state != 0xC && state != 0x10) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8968);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of the memory-card close states (9, 13, 17 and 25): accepts results 0, 3, 4, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8968(int cardResult) {
    int state = D_004A4BD8->st;
    if (state != 25 && state != 9 && state != 13 && state != 17) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F89C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 26 (unit released): accepts results 0, 3, 9 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F89C8(int cardResult) {
    if (D_004A4BD8->st != 26) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8A08);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 5 (directory created): accepts results 0, 3, 8, 9, 14 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8A08(int cardResult) {
    if (D_004A4BD8->st != 5) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 9 && cardResult != 0xE && cardResult != 8 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8A58);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of the memory-card file-open states (6, 10, 14 and 22): accepts results 0, 3, 4, 9, 13 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8A58(int cardResult) {
    int state = D_004A4BD8->st;
    if (state != 22 && state != 6 && state != 0xE && state != 10) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 13 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8AC0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of the memory-card file-delete states (19..21): accepts results 0, 3, 4, 5, 6, 9, 13 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8AC0(int cardResult) {
    if ((unsigned)(D_004A4BD8->st - 19) >= 3) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 13 && cardResult != 6 && cardResult != 5 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8B28);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 18 (directory deleted): accepts results 0, 3, 4, 9, 13 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8B28(int cardResult) {
    if (D_004A4BD8->st != 18) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 13 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8B78);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 3 (free space read): accepts results 0, 3 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8B78(int cardResult) {
    int state = D_004A4BD8->st;
    if (state != 3) return 18;
    if (cardResult != 0 && cardResult != state && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8BB0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 4 (second space query): accepts results 0, 3 and 17.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8BB0(int cardResult) {
    if (D_004A4BD8->st != 4) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8BE8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Result check of memory-card request state 23: accepts results 0, 3, 4, 9 and 17. No request step enters state 23.
 *
 * @param cardResult  result of the finished card call
 * @return @p cardResult, 18 for another state, or 0x10 for any other result
 */
int func_003F8BE8(int cardResult) {
    if (D_004A4BD8->st != 23) return 18;
    if (cardResult != 0 && cardResult != 3 && cardResult != 4 && cardResult != 9 && cardResult != 17) return 0x10;
    return cardResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8C30);
#ifdef SKIP_ASM
/**
 * Step handler of memory-card request operation 0 (entry 0 of the per-operation table D_00451468): passes the result
 * on.
 *
 * @param cardResult  the checked result
 * @return @p cardResult
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8C30(int cardResult) { return cardResult; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8C38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;
/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;
extern void f86F0(int) __asm__("func_003F86F0");
extern int func_003F85C0(unsigned short v, int dir);
extern int func_003F8710(void);
extern int func_003F87B0(int a, int *out);
extern char *func_003FA7A0(char *dst, int unused, char *src);
extern void fA818(char *, char *, int, char *) __asm__("func_003FA818");
/**
 * Step handler of the memory-card open operations (3: search the usable units, 6: one unit).
 *
 * Unit checked (state 1): searches the directory for "*" + the save name (state 2); with no card (3) operation 3 tries
 * the next usable unit in the search direction. Search done: when found, builds the directory and file names
 * ("PSCD10088" + name) and opens dir/file with mode 3 (state 22); when not found records error 4 and releases the unit
 * (state 26). Open done: keeps the descriptor in the request body and finishes (10 when it is negative). Unit released:
 * finishes with the recorded error, or for operation 3 with an error tries the next unit. Failures go to the release
 * cleanup (func_003F87B0).
 *
 * @param callResult  checked result of the finished card call
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (search: 1 found / 0 not; open: the descriptor)
 * @return 0 or an error code
 */
int func_003F8C38(int callResult, int *out, int *pollResult) {
    int op;
    int slot;
    char buf[80];
    if (out == 0) return 18;
    if (pollResult == 0) return 18;
    op = D_004A4BD8->op;
    if (op != 3 && op != 6) return 18;
    *out = 1;
    switch ((unsigned)D_004A4BD8->st) {
    case 1:
        if (callResult == 0) {
            func_003FA7A0(buf, (int)D_004A4BDC->f50, D_004A4BD8->body.name);
            D_004A4BD8->st = 2;
            ((void (*)(char *, int))D_004A4BDC->f04)(buf, D_004A4BD8->body.slot);
            break;
        }
        if (callResult != 3) {
            *out = 2;
            return callResult;
        }
        if (D_004A4BD8->op == 3) {
            slot = func_003F85C0(D_004A4BDC->ports, D_004A4BD8->f6C);
            D_004A4BD8->body.slot = slot;
            if (slot != -1) {
                D_004A4BD8->st = 1;
                D_004A4BDC->f10(slot);
                return 0;
            }
        }
        *out = 2;
        return callResult;
    case 2:
        if (callResult == 0) {
            if (*pollResult == 1) {
                fA818((char *)D_004A4BD8->body.pad08, (char *)&D_004A4BD8->body.pad08[8], (int)D_004A4BDC->f50, D_004A4BD8->body.name);
                D_004A4BD8->st = 22;
                ((void (*)(int *, int *, int, int))D_004A4BDC->f18)(D_004A4BD8->body.pad08, &D_004A4BD8->body.pad08[8], D_004A4BD8->body.slot, 3);
                break;
            }
            if (*pollResult == 0) {
                f86F0(4);
                D_004A4BD8->st = 26;
                D_004A4BDC->f14(D_004A4BD8->body.slot);
                break;
            }
            *out = 2;
            return 10;
        }
        f86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, out);
    case 22:
        if (callResult == 0) {
            if (*pollResult >= 0) {
                D_004A4BD8->body.port = *pollResult;
                *out = 2;
                return 0;
            }
            D_004A4BD8->body.port = *pollResult;
            *out = 2;
            return 10;
        }
        f86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, out);
    case 26:
        if (callResult == 0) {
            if (D_004A4BD8->op == 4 || D_004A4BD8->op == 6) {
                *out = 2;
                return func_003F8710();
            }
            if (func_003F8710() == 0) {
                *out = 2;
                break;
            }
            slot = func_003F85C0(D_004A4BDC->ports, D_004A4BD8->f6C);
            D_004A4BD8->body.slot = slot;
            if (slot != -1) {
                D_004A4BD8->st = 1;
                D_004A4BDC->f10(slot);
                return 0;
            }
            *out = 2;
            return func_003F8710();
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F8EF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

extern void f86F0(int) __asm__("func_003F86F0");
/**
 * Step handler of the memory-card seek, write and read operations (8, 9, 10).
 *
 * Seek (state 27) finishes on success; write and read (states 28, 29) finish with 0 when the byte count equals the
 * request's expected count (+0x70), else 10. A failed call records the error and starts the flush cleanup
 * (func_003F8720).
 *
 * @param callResult  checked result of the finished card call
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (bytes moved)
 * @return 0 or an error code
 */
int func_003F8EF8(int callResult, int *out, int *pollResult) {
    int op;
    if (out == 0) return 18;
    if (pollResult == 0) return 18;
    op = D_004A4BD8->op;
    if (op != 8 && op != 10 && op != 9) return 18;
    *out = 1;
    switch ((unsigned)D_004A4BD8->st) {
    case 27:
        if (callResult == 0) {
            *out = 2;
            return 0;
        }
        f86F0(callResult);
        return func_003F8720((int)&D_004A4BD8->body, out);
    case 0x1C:
    case 29:
        if (callResult == 0) {
            if (D_004A4BD8->f70 == *pollResult) {
                *out = 2;
                return 0;
            }
            *out = 2;
            return 10;
        }
        f86F0(callResult);
        return func_003F8720((int)&D_004A4BD8->body, out);
    }
    *out = 2;
    return 18;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8FF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;
/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern void f86F0(int) __asm__("func_003F86F0");
/**
 * Step handler of the memory-card close operation (7): flush done (state 24) closes the file (25), close done releases
 * the unit (26), release done finishes. A failed step records the error and continues with the matching cleanup
 * (func_003F8768, func_003F87B0).
 *
 * @param callResult  checked result of the finished card call
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (only checked for 0)
 * @return 0 or an error code
 */
int func_003F8FF0(int callResult, int *out, int pollResult) {
    if (out == 0) return 18;
    if (pollResult == 0) return 18;
    if (D_004A4BD8->op != 7) return 18;
    *out = 1;
    switch ((unsigned)D_004A4BD8->st) {
    case 0x18:
        if (callResult == 0) {
            D_004A4BD8->st = 25;
            D_004A4BDC->f1C(D_004A4BD8->body.port);
            break;
        }
        f86F0(callResult);
        return func_003F8768((int)&D_004A4BD8->body, out);
    case 25:
        if (callResult == 0) {
            D_004A4BD8->st = 26;
            D_004A4BDC->f14(D_004A4BD8->body.slot);
            break;
        }
        f86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, out);
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
INCLUDE_ASM("seg/seg_2F41F8", func_003F9130);
#ifdef SKIP_ASM
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F8710(void);
/**
 * Step handler of the memory-card cleanup operations (11..13): flush (state 24), then close (25), then release the unit
 * (26), and finish with the recorded error (func_003F8710), whatever each step returned.
 *
 * @param callResult  checked result of the finished card call (ignored)
 * @param out  receives 1 while running, 2 when finished
 * @param pollResult  the poll's result word (only checked for 0)
 * @return 0 while running, the recorded error when finished, or 18 for a bad state
 */
int func_003F9130(int callResult, int *out, int pollResult) {
    if (out == 0 || pollResult == 0 || (unsigned)(D_004A4BD8->op - 11) > 2) return 18;
    *out = 1;
    switch ((unsigned)D_004A4BD8->st) {
    case 0x18:
        D_004A4BD8->st = 25;
        D_004A4BDC->f1C(D_004A4BD8->body.port);
        break;
    case 25:
        D_004A4BD8->st = 26;
        D_004A4BDC->f14(D_004A4BD8->body.slot);
        break;
    case 26:
        *out = 2;
        return func_003F8710();
    default:
        *out = 2;
        return 18;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9208);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

/**
 * Returns whether the memory-card request layer is open (its driver block D_004A4BDC exists).
 *
 * @return 1 when open, else 0
 */
int func_003F9208(void) { return D_004A4BDC != 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9218);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

/* The caller's callback table (17 entries, copied to sMcDrv 0x04..0x44); every entry is required. */
struct sMcCallbacks { int f[17]; };

extern int func_003F9208(void);
extern int func_00266728(int, int, int);
extern int (*D_00451468[])(int, int *, int *);  /* per-op handler table */
extern int (*D_004514A0[])(int, int, int);      /* per-state handler table */

/* The unit's later caller declares this (int, int, int). */
int func_003F9218_impl(int *ports, struct sMcCallbacks *cb, int heap) __asm__("func_003F9218");
/**
 * Opens the memory-card request layer.
 *
 * Checks the arguments (every one of the 17 callbacks must be set), allocates the 1068-byte driver block D_004A4BDC and
 * the 116-byte request D_004A4BD8 from @p heap, builds the usable-unit mask from @p ports (a -1-terminated list of unit
 * codes 0..7, or 9 for all eight; at most 8 entries), copies the callbacks to driver +0x4..+0x44 and installs the
 * per-operation and per-state tables D_00451468 / D_004514A0. Quirk: on the second allocation failure the driver block
 * stays allocated and set.
 *
 * @param ports  the unit-code list
 * @param cb  the 17 memory-card callbacks
 * @param heap  heap for the blocks
 * @return 0; 1 when already open, 0xC for bad arguments, 9 for a bad unit code, 7 when an allocation fails
 */
int func_003F9218_impl(int *ports, struct sMcCallbacks *cb, int heap) {
    int *unitEntry;
    unsigned char n = 0;
    char i;

    if (func_003F9208() != 0) return 1;
    if (ports == 0) return 0xC;
    if (*ports == -1) return 0xC;
    if (cb == 0) return 0xC;
    if (cb->f[0] == 0 || cb->f[1] == 0 || cb->f[2] == 0 || cb->f[3] == 0 || cb->f[4] == 0 || cb->f[5] == 0 ||
        cb->f[6] == 0 || cb->f[7] == 0 || cb->f[8] == 0 || cb->f[9] == 0 || cb->f[10] == 0 || cb->f[11] == 0 ||
        cb->f[12] == 0 || cb->f[13] == 0 || cb->f[14] == 0 || cb->f[15] == 0 || cb->f[16] == 0)
        return 0xC;
    if (D_004A4BDC != 0 || D_004A4BD8 != 0) return 1;
    D_004A4BDC = (struct sMcDrv *)func_00266728(heap, 1068, 4);
    if (D_004A4BDC == 0) return 7;
    D_004A4BD8 = (struct sMcReq *)func_00266728(heap, 116, 4);
    if (D_004A4BD8 == 0) return 7;
    D_004A4BDC->h = heap;
    D_004A4BDC->ports = 0;
    for (unitEntry = ports; *unitEntry != -1; unitEntry++, n++) {
        if ((unsigned)*unitEntry >= 8 && *unitEntry != 9) return 9;
        if (n == 8) return 0xC;
        if (*unitEntry == 9) {
            for (i = 0; i < 8; i++) D_004A4BDC->ports |= 1 << i;
        } else {
            D_004A4BDC->ports |= 1 << (unsigned short)*unitEntry;
        }
    }
    D_004A4BDC->f04 = cb->f[0];
    D_004A4BDC->f08 = cb->f[1];
    D_004A4BDC->f0C = cb->f[2];
    D_004A4BDC->f10 = (void (*)(int))cb->f[3];
    D_004A4BDC->f14 = (void (*)(int))cb->f[4];
    D_004A4BDC->f18 = cb->f[5];
    D_004A4BDC->f1C = (void (*)(int))cb->f[6];
    D_004A4BDC->f20 = cb->f[7];
    D_004A4BDC->f24 = cb->f[8];
    D_004A4BDC->f28 = cb->f[9];
    D_004A4BDC->f2C = (void (*)(int, int, int))cb->f[10];
    D_004A4BDC->f30 = (void (*)(int, int, int))cb->f[11];
    D_004A4BDC->f34 = (void (*)(int, int, int))cb->f[12];
    D_004A4BDC->f38 = (void (*)(int))cb->f[13];
    D_004A4BDC->f3C = cb->f[14];
    D_004A4BDC->f40 = (int (*)(int *, int *))cb->f[15];
    D_004A4BDC->f44 = cb->f[16];
    D_004A4BDC->f48 = D_00451468;
    D_004A4BDC->f4C = D_004514A0;
    D_004A4BD8->op = 0;
    D_004A4BD8->st = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9530);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);
extern int func_00266760(int, char *, int, int);
/**
 * Closes the memory-card request layer: clears the 17 callbacks and the request, frees the request and the driver block
 * to the driver's heap and clears both pointers.
 *
 * @return 0, or 2 when the layer is not open
 */
int func_003F9530(void) {
    struct sMcDrv *driver;
    struct sMcReq *request;
    if (func_003F9208() == 0) return 2;
    driver = D_004A4BDC;
    request = D_004A4BD8;
    if (driver == 0 || request == 0) return 2;
    driver->f04 = 0;
    driver->f08 = 0;
    driver->f0C = 0;
    driver->f10 = 0;
    driver->f14 = 0;
    driver->f18 = 0;
    driver->f1C = 0;
    driver->f20 = 0;
    driver->f24 = 0;
    driver->f28 = 0;
    driver->f2C = 0;
    driver->f30 = 0;
    driver->f34 = 0;
    driver->f38 = 0;
    driver->f3C = 0;
    driver->f40 = 0;
    driver->f44 = 0;
    request->op = 0;
    request->st = 0;
    func_00266760(driver->h, (char *)request, 116, 4);
    D_004A4BD8 = 0;
    func_00266760(D_004A4BDC->h, (char *)D_004A4BDC, 1068, 4);
    D_004A4BDC = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F95E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/**
 * Copies the current memory-card request body (0x60 bytes: file descriptor, unit code, directory and file names, save
 * name) to @p dst.
 *
 * @param dst  destination
 * @return 0, or 0xC when @p dst is 0
 */
int func_003F95E8(char *dst) {
    if (dst == 0) return 0xC;
    __builtin_memcpy(dst, &D_004A4BD8->body, 0x60);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9698);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);

extern int c9698_86B8(int) __asm__("func_003F86B8");
extern int c9698_8550(int) __asm__("func_003F8550");
extern int c9698_8588(int) __asm__("func_003F8588");
extern void c9698_86F0(int) __asm__("func_003F86F0");
extern void c9698_B2F8(int) __asm__("func_003FB2F8");
extern unsigned c9698_6210(void *, int, int) __asm__("memset");
extern char *strcpy(char *, const char *);
int func_003F9698(char *str, int a, int b);
/**
 * Starts a memory-card request that deletes the save @p str.
 *
 * With unit -1 it searches the usable units (operation 2) from the lowest (@p searchDir 0) or the highest (@p searchDir 1), or uses the
 * only one (operation 5); otherwise it uses unit @p unitCode (operation 5). Clears the body and the recorded error, stores the
 * name and direction, calls the start hook func_003FB2F8 and the unit-check callback (driver +0x10), and enters state
 * 1. func_003FAE50 runs the steps.
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction (0 upward, 1 downward)
 * @return 0; 2 when the layer is not open, 0xC for a bad name or direction, 9 for a bad unit, 11 when the unit is not
 * usable, 13 when a request is running
 */
int func_003F9698(char *str, int unitCode, int searchDir) {
    int firstUnit;
    int t;
    struct sMcReq *request;
    struct sMcReq *request2;
    unsigned unitBit;
    if (func_003F9208() == 0) return 2;
    if (str == 0) return 0xC;
    if ((unsigned)(unitCode + 1) >= 9) return 9;
    if (unitCode != -1) {
        unitBit = 1 << unitCode;
        if (!(D_004A4BDC->ports & unitBit)) return 11;
    }
    if (D_004A4BD8->op != 0) return 13;
    c9698_6210(&D_004A4BD8->body, 0, 0x60);
    if (unitCode == -1) {
        if (c9698_86B8(D_004A4BDC->ports) == 1) {
            firstUnit = c9698_8550(D_004A4BDC->ports);
            request = D_004A4BD8;
            request->body.slot = firstUnit;
            request->op = 5;
        } else if (searchDir == 0) {
            firstUnit = c9698_8550(D_004A4BDC->ports);
            request = D_004A4BD8;
            request->body.slot = firstUnit;
            request->op = 2;
        } else if (searchDir == 1) {
            firstUnit = c9698_8588(D_004A4BDC->ports);
            request = D_004A4BD8;
            request->body.slot = firstUnit;
            request->op = 2;
        } else {
            return 0xC;
        }
    } else {
        request2 = D_004A4BD8;
        request2->body.slot = unitCode;
        request2->op = 5;
    }
    c9698_86F0(0);
    strcpy(D_004A4BD8->body.name, str);
    D_004A4BD8->f6C = searchDir;
    c9698_B2F8(1);
    D_004A4BDC->f10(D_004A4BD8->body.slot);
    D_004A4BD8->st = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9818);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int strlen(const char *);

/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);

extern int c9818_86B8(int) __asm__("func_003F86B8");
extern int c9818_8550(int) __asm__("func_003F8550");
extern int c9818_8588(int) __asm__("func_003F8588");
extern void c9818_86F0(int) __asm__("func_003F86F0");
extern void c9818_B2F8(int) __asm__("func_003FB2F8");
extern unsigned c9818_6210(void *, int, int) __asm__("memset");
extern char *strcpy(char *, const char *);
int func_003F9818(char *str, int a, int b);
/**
 * Starts a memory-card request that creates the save @p str (operations 1 / 4).
 *
 * Like func_003F9698, with a name limit: names of 0x18 characters or more give 0xC. func_003FA8F8 runs the steps.
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction (0 upward, 1 downward)
 * @return 0 or the error codes of func_003F9698
 */
int func_003F9818(char *str, int unitCode, int searchDir) {
    int firstUnit;
    int t;
    struct sMcReq *request;
    struct sMcReq *request2;
    unsigned unitBit;
    if (func_003F9208() == 0) return 2;
    if (str == 0) return 0xC;
    if (strlen(str) >= 0x18) return 0xC;
    if ((unsigned)(unitCode + 1) >= 9) return 9;
    if (unitCode != -1) {
        unitBit = 1 << unitCode;
        if (!(D_004A4BDC->ports & unitBit)) return 11;
    }
    if (D_004A4BD8->op != 0) return 13;
    c9818_6210(&D_004A4BD8->body, 0, 0x60);
    if (unitCode == -1) {
        if (c9818_86B8(D_004A4BDC->ports) == 1) {
            firstUnit = c9818_8550(D_004A4BDC->ports);
            request = D_004A4BD8;
            request->body.slot = firstUnit;
            request->op = 4;
        } else if (searchDir == 0) {
            firstUnit = c9818_8550(D_004A4BDC->ports);
            request2 = D_004A4BD8;
            request2->body.slot = firstUnit;
            request2->op = 1;
        } else if (searchDir == 1) {
            firstUnit = c9818_8588(D_004A4BDC->ports);
            request2 = D_004A4BD8;
            request2->body.slot = firstUnit;
            request2->op = searchDir;
        } else {
            return 0xC;
        }
    } else {
        request2 = D_004A4BD8;
        request2->body.slot = unitCode;
        request2->op = 4;
    }
    c9818_86F0(0);
    strcpy(D_004A4BD8->body.name, str);
    D_004A4BD8->f6C = searchDir;
    c9818_B2F8(1);
    D_004A4BDC->f10(D_004A4BD8->body.slot);
    D_004A4BD8->st = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F99B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);

extern int c99b8_86B8(int) __asm__("func_003F86B8");
extern int c99b8_8550(int) __asm__("func_003F8550");
extern int c99b8_8588(int) __asm__("func_003F8588");
extern void c99b8_86F0(int) __asm__("func_003F86F0");
extern void c99b8_B2F8(int) __asm__("func_003FB2F8");
extern unsigned c99b8_6210(void *, int, int) __asm__("memset");
extern char *strcpy(char *, const char *);
int func_003F99B8_impl(char *str, int a, int b) __asm__("func_003F99B8");
/**
 * Starts a memory-card request that opens the save @p str (operations 3 / 6).
 *
 * Like func_003F9698; func_003F8C38 runs the steps.
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction (0 upward, 1 downward)
 * @return 0 or the error codes of func_003F9698
 */
int func_003F99B8_impl(char *str, int unitCode, int searchDir) {
    int firstUnit;
    int t;
    struct sMcReq *request;
    struct sMcReq *request2;
    unsigned unitBit;
    if (func_003F9208() == 0) return 2;
    if (str == 0) return 0xC;
    if ((unsigned)(unitCode + 1) >= 9) return 9;
    if (unitCode != -1) {
        unitBit = 1 << unitCode;
        if (!(D_004A4BDC->ports & unitBit)) return 11;
    }
    if (D_004A4BD8->op != 0) return 13;
    c99b8_6210(&D_004A4BD8->body, 0, 0x60);
    if (unitCode == -1) {
        if (c99b8_86B8(D_004A4BDC->ports) == 1) {
            firstUnit = c99b8_8550(D_004A4BDC->ports);
            request = D_004A4BD8;
            request->body.slot = firstUnit;
            request->op = 6;
        } else if (searchDir == 0) {
            firstUnit = c99b8_8550(D_004A4BDC->ports);
            request = D_004A4BD8;
            request->body.slot = firstUnit;
            request->op = 3;
        } else if (searchDir == 1) {
            firstUnit = c99b8_8588(D_004A4BDC->ports);
            request = D_004A4BD8;
            request->body.slot = firstUnit;
            request->op = 3;
        } else {
            return 0xC;
        }
    } else {
        request2 = D_004A4BD8;
        request2->body.slot = unitCode;
        request2->op = 6;
    }
    c99b8_86F0(0);
    strcpy(D_004A4BD8->body.name, str);
    D_004A4BD8->f6C = searchDir;
    c99b8_B2F8(1);
    D_004A4BDC->f10(D_004A4BD8->body.slot);
    D_004A4BD8->st = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9B38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int f9208(void) __asm__("func_003F9208");
int func_003F9B38_impl(int *src) __asm__("func_003F9B38");
/**
 * Starts a memory-card request that closes an open save file (operation 7): copies the request body from @p src, enters
 * state 24 and flushes the file (driver +0x38 with the descriptor @p src[0]). func_003F8FF0 runs the steps.
 *
 * @param src  the 0x60-byte request body of the open file
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int func_003F9B38_impl(int *src) {
    if (f9208() == 0) return 2;
    if (src == 0) return 0xC;
    if (D_004A4BD8->op != 0) return 13;
    __builtin_memcpy(&D_004A4BD8->body, src, 0x60);
    D_004A4BD8->op = 7;
    D_004A4BD8->st = 0x18;
    D_004A4BDC->f38(src[0]);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9C48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);

extern void c9c48_86F0(int) __asm__("func_003F86F0");
extern int func_003FA6E8_9C48(void) __asm__("func_003FA6E8");
int func_003F9C48_impl(int *src, int a, int b) __asm__("func_003F9C48");
/**
 * Starts a memory-card request that seeks in an open save file (operation 8, state 27).
 *
 * Copies the body from @p src; with origin 0 seeks to @p offset plus the base offset (func_003FA6E8, 0) from the start,
 * otherwise passes @p offset and @p origin on. Keeps the target in +0x70.
 *
 * @param src  the 0x60-byte request body of the open file
 * @param offset  offset
 * @param origin  seek origin
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int func_003F9C48_impl(int *src, int offset, int origin) {
    struct sMcReq *request;
    struct sMcReq *requestAgain;
    int target;
    struct sMcDrv *driver;
    if (func_003F9208() == 0) return 2;
    if (src == 0) return 0xC;
    request = D_004A4BD8;
    if (request->op != 0) return 13;
    request->op = 8;
    request->st = 27;
    c9c48_86F0(0);
    __builtin_memcpy(&D_004A4BD8->body, (char *)src, 0x60);
    if (origin == 0) {
        target = offset + func_003FA6E8_9C48();
        requestAgain = D_004A4BD8;
        driver = D_004A4BDC;
        requestAgain->f70 = target;
        driver->f34(requestAgain->body.port, offset + func_003FA6E8_9C48(), 0);
    } else {
        struct sMcReq *requestInner = D_004A4BD8;
        struct sMcDrv *driverInner = D_004A4BDC;
        requestInner->f70 = offset;
        driverInner->f34(requestInner->body.port, offset, origin);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9DC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);

extern void f86F0(int) __asm__("func_003F86F0");
int func_003F9DC0_impl(int *src, int a, int b) __asm__("func_003F9DC0");
/**
 * Starts a memory-card request that reads from an open save file (operation 10, state 29): read callback (driver +0x2C)
 * with the descriptor, @p buffer and @p byteCount; @p byteCount is kept as the expected count.
 *
 * @param src  the 0x60-byte request body of the open file
 * @param buffer  destination buffer
 * @param byteCount  byte count
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int func_003F9DC0_impl(int *src, int buffer, int byteCount) {
    struct sMcReq *request;
    struct sMcReq *requestAgain;
    struct sMcDrv *driver;
    if (func_003F9208() == 0) return 2;
    if (src == 0) return 0xC;
    request = D_004A4BD8;
    if (request->op != 0) return 13;
    request->op = 10;
    request->st = 29;
    f86F0(0);
    __builtin_memcpy(&D_004A4BD8->body, (char *)src, 0x60);
    requestAgain = D_004A4BD8;
    driver = D_004A4BDC;
    requestAgain->f70 = byteCount;
    driver->f2C(src[0], buffer, byteCount);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F9F00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);
extern void f86F0(int) __asm__("func_003F86F0");
int func_003F9F00_impl(int *src, int a, int b) __asm__("func_003F9F00");
/**
 * Starts a memory-card request that writes to an open save file (operation 9, state 28): write callback (driver +0x30)
 * with the descriptor, @p buffer and @p byteCount; @p byteCount is kept as the expected count.
 *
 * @param src  the 0x60-byte request body of the open file
 * @param buffer  source buffer
 * @param byteCount  byte count
 * @return 0; 2 when the layer is not open, 0xC when @p src is 0, 13 when a request is running
 */
int func_003F9F00_impl(int *src, int buffer, int byteCount) {
    struct sMcReq *request;
    struct sMcReq *requestAgain;
    struct sMcDrv *driver;
    if (func_003F9208() == 0) return 2;
    if (src == 0) return 0xC;
    request = D_004A4BD8;
    if (request->op != 0) return 13;
    request->op = 9;
    request->st = 0x1C;
    f86F0(0);
    __builtin_memcpy(&D_004A4BD8->body, (char *)src, 0x60);
    requestAgain = D_004A4BD8;
    driver = D_004A4BDC;
    requestAgain->f70 = byteCount;
    driver->f30(src[0], buffer, byteCount);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA040);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);

extern void fB2F8w(int) __asm__("func_003FB2F8");
/**
 * Polls the memory-card request layer and advances the running request.
 *
 * Calls the poll callback (driver +0x40). When the card call has finished (status 2), checks its result with the
 * current state's entry of the per-state table (10 ends the request, 0x10 is returned) and runs the operation's step
 * handler. When that finishes the request, calls the hook func_003FB2F8 for operations 2, 5, 7 and 11..13 and clears
 * the operation and state.
 *
 * @param statusOut  receives the status: 0 idle, 1 running, 2 finished
 * @param resultOut  receives the card call's result word
 * @return the result; 2 when the layer is not open, 13 without a poll callback, 0xC for a 0 argument, 0xF when the card
 * is idle but a request is pending
 */
int func_003FA040(int *statusOut, int *resultOut) {
    int result;
    if (func_003F9208() == 0) return 2;
    if (D_004A4BDC->f40 == 0) return 13;
    if (statusOut == 0) return 0xC;
    if (resultOut == 0) return 0xC;
    result = D_004A4BDC->f40(statusOut, resultOut);
    if (statusOut[0] == 0) {
        if (D_004A4BD8->op != 0) return 0xF;
    }
    if (statusOut[0] != 2) return result;
    result = D_004A4BDC->f4C[D_004A4BD8->st](result, 2, resultOut[0]);
    if (result == 10) {
        struct sMcReq *request = D_004A4BD8;
        request->op = 0;
        request->st = 0;
        return 10;
    }
    if (result == 0x10) return 0x10;
    result = D_004A4BDC->f48[D_004A4BD8->op](result, statusOut, resultOut);
    if (statusOut[0] == 2) {
        int finishedOp = D_004A4BD8->op;
        struct sMcReq *request;
        if (finishedOp == 5 || finishedOp == 2 || finishedOp == 7 || finishedOp == 11 || finishedOp == 0xC || finishedOp == 13) fB2F8w(0);
        request = D_004A4BD8;
        request->op = 0;
        request->st = 0;
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003FA1D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F9208(void);
extern int func_003FA708(unsigned int, int);
/**
 * Decodes a memory-card unit code into its port and slot (func_003FA708). The game's card callbacks call it.
 *
 * @param unitIndex  unit code 0..7
 * @param portSlot  receives the port and slot bytes
 * @return 0; 12 when @p portSlot is 0, 2 when the layer is not open, 9 for a code of 8 or more
 */
int func_003FA1D0(unsigned int unitIndex, int portSlot) {
    if (portSlot == 0) return 12;
    if (func_003F9208() == 0) return 2;
    if (unitIndex >= 8) return 9;
    func_003FA708(unitIndex, portSlot);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA240);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern int func_003F9208(void);
extern void fA6A8(char *, int *, int *) __asm__("func_003FA6A8");
extern int fB308(char *) __asm__("func_003FB308");
int func_003FA240_impl(char *src) __asm__("func_003FA240");
/**
 * Stores the save description (976 bytes: icon data pointer and size, the icon.sys block, the record size at +0x3CC) in
 * the driver block (+0x50) and computes the space a new save needs into +0x420 / +0x424 (func_003FA6A8).
 *
 * @param src  the description
 * @return 0; 0xC when @p src is 0, 2 when the layer is not open
 */
int func_003FA240_impl(char *src) {
    if (src == 0) return 0xC;
    if (func_003F9208() == 0) return 2;
    __builtin_memcpy(D_004A4BDC->f50, src, 976);
    fA6A8(src, &D_004A4BDC->f420, &D_004A4BDC->f424);
    return fB308(src);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA360);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB278(void);
/**
 * Directory-entry filter the game's search callback uses: accepts every entry (func_003FB278).
 *
 * @param name  the entry name (ignored)
 * @return 1
 */
int func_003FA360(char* name) { return func_003FB278(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA380);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { unsigned char b[33]; } T_A380;
extern T_A380 D_004973B0;
/**
 * Converts an ASCII string to full-width Shift-JIS, as icon.sys titles need.
 *
 * Letters and digits map to 0x8260.. / 0x8281.. / 0x824F..; other printable characters to 0x81xx through the table
 * D_004973B0; Shift-JIS lead bytes 0x81..0x98 copy their two-byte character through. Each character is stored
 * byte-swapped (big-endian in memory). Quirk: no terminator is written; func_003F8258 converts into a zeroed buffer.
 *
 * @param src  ASCII text
 * @param dst  Shift-JIS output
 */
void func_003FA380(char *src, unsigned short *dst) {
    T_A380 tbl = D_004973B0;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FA4F0);
#ifdef SKIP_ASM
/**
 * Converts a full-width Shift-JIS string (byte-swapped, as func_003FA380 writes it) back to ASCII and NUL-terminates
 * it. Stops at the first character it cannot map. Nothing in retail calls it.
 *
 * @param src  Shift-JIS text
 * @param dst  ASCII output
 */
void func_003FA4F0(unsigned short *src, char *dst) {
    T_A380 tbl = D_004973B0;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FA6A8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB280(void);
/**
 * Forwards to func_003FB280 (the space a new save needs) with the caller's arguments.
 *
 * @return no meaningful value (func_003FB280 returns nothing)
 */
int func_003FA6A8(void) { return func_003FB280(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA6C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB2C8(void);
/**
 * Forwards to func_003FB2C8 (fetch the CRC interface) with the caller's argument.
 *
 * @return no meaningful value (func_003FB2C8 returns nothing)
 */
int func_003FA6C8(void) { return func_003FB2C8(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA6E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB300(void);
/**
 * Returns the base offset of seeks in a save file (func_003FB300, always 0).
 *
 * @return 0
 */
int func_003FA6E8(void) { return func_003FB300(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA708);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003FA708_impl(unsigned a, unsigned char *p) __asm__("func_003FA708");
/**
 * Splits a memory-card unit code into port and slot bytes: codes 0..3 are port 0 slots 0..3, codes 4..7 port 1 slots
 * 0..3. Other codes leave the output untouched.
 *
 * @param unitCode  unit code
 * @param portSlotOut  receives port, slot
 */
void func_003FA708_impl(unsigned unitCode, unsigned char *portSlotOut) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FA7A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_004A4BE0[];
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
char *func_003FA7A0(char *dst, int saveDesc, char *src) {
    char *nameDst = dst + 1;
    memset(dst, 0, 0x20);
    *dst = D_004A4BE0[0];
    func_0041605C(nameDst, src, strlen(src));
    return nameDst + strlen(src);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA818);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { char b[9]; } K9;
extern K9 D_00497400;
extern void f6210(char *, int, int) __asm__("memset");
extern void f605C(char *, char *, int) __asm__("func_0041605C");
void func_003FA818_impl(char *a, char *b, int unused, char *str) __asm__("func_003FA818");
/**
 * Builds a save's directory name and file name, both "PSCD10088" followed by the save name, each in a zeroed 32-byte
 * buffer.
 *
 * @param dirName  receives the directory name
 * @param fileName  receives the file name
 * @param saveDesc  ignored (callers pass the save description)
 * @param str  the save name
 */
void func_003FA818_impl(char *dirName, char *fileName, int saveDesc, char *str) {
    char *nameDst;
    f6210(dirName, 0, 0x20);
    *(K9 *)dirName = D_00497400;
    nameDst = dirName + 9;
    f605C(nameDst, str, strlen(str));
    nameDst = fileName + 9;
    strlen(str);
    f6210(fileName, 0, 0x20);
    *(K9 *)fileName = D_00497400;
    f605C(nameDst, str, strlen(str));
    strlen(str);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA8F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern char str_view_ico_b[];   /* "view.ico" */
extern char str_icon_sys_b[];   /* "icon.sys" */
extern void fA8F8_86F0(int) __asm__("func_003F86F0");
extern char *fA8F8_A7A0(char *, char *, char *) __asm__("func_003FA7A0");

/**
 * Step handler of the memory-card create operations (1: search the usable units, 4: one unit); the per-op handler
 * table D_00451468 holds it for ops 1 and 4.
 *
 * Unit check, then a search for the save (it must not exist: error 8), the free space (at least driver +0x420 bytes,
 * else 14), a second query against +0x424 and the directory ("PSCD10088" + name). Then creates the main file dir/dir
 * (mode 515), flushes and closes it; writes the icon data to view.ico and the 964-byte icon.sys block from the save
 * description, each opened, written, flushed and closed; and releases the unit. With no card (3) operation 1 tries the
 * next usable unit; when the unit is released with an error it does too. Failures go to the flush, close or release
 * cleanup (func_003F8720, func_003F8768, func_003F87B0).
 *
 * @param callResult  checked result of the finished card call
 * @param st  receives 1 while running, 2 when finished
 * @param res  the poll's result word
 * @return 0 or an error code
 */
int func_003FA8F8(int callResult, int *st, int *res) {
    char path[80];
    int pollValue;

    if (st == 0) return 18;
    if (res == 0) return 18;
    if (D_004A4BD8->op != 1 && D_004A4BD8->op != 4) return 18;
    *st = 1;
    switch (D_004A4BD8->st) {
    case 1:
        if (callResult == 0) {
            fA8F8_A7A0(path, D_004A4BDC->f50, D_004A4BD8->body.name);
            D_004A4BD8->st = 2;
            ((void (*)(char *, int))D_004A4BDC->f04)(path, D_004A4BD8->body.slot);
            break;
        }
        if (callResult == 3) {
            if (D_004A4BD8->op == 1) {
                D_004A4BD8->body.slot = func_003F85C0(D_004A4BDC->ports, D_004A4BD8->f6C);
                if (D_004A4BD8->body.slot != -1) {
                    D_004A4BD8->st = 1;
                    D_004A4BDC->f10(D_004A4BD8->body.slot);
                    return 0;
                }
                *st = 2;
                return func_003F8710();
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
                D_004A4BD8->st = 3;
                ((void (*)(int))D_004A4BDC->f08)(D_004A4BD8->body.slot);
                break;
            }
            if (pollValue == 1) {
                fA8F8_86F0(8);
                D_004A4BD8->st = 26;
                D_004A4BDC->f14(D_004A4BD8->body.slot);
                break;
            }
            *st = 2;
            return 10;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 3:
        if (callResult == 0) {
            pollValue = *res;
            if (pollValue < 0) {
                *st = 2;
                return 10;
            }
            if ((unsigned)pollValue >= (unsigned)D_004A4BDC->f420) {
                D_004A4BD8->st = 4;
                ((void (*)(int))D_004A4BDC->f0C)(D_004A4BD8->body.slot);
                break;
            }
            fA8F8_86F0(0xE);
            D_004A4BD8->st = 26;
            D_004A4BDC->f14(D_004A4BD8->body.slot);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 4:
        if (callResult == 0) {
            pollValue = *res;
            if (pollValue < 0) {
                *st = 2;
                return 10;
            }
            if ((unsigned)pollValue >= (unsigned)D_004A4BDC->f424) {
                func_003FA818_impl((char *)D_004A4BD8->body.pad08, (char *)&D_004A4BD8->body.pad08[8], (int)D_004A4BDC->f50,
                                   D_004A4BD8->body.name);
                D_004A4BD8->st = 5;
                ((void (*)(char *, int, int))D_004A4BDC->f20)((char *)D_004A4BD8->body.pad08, D_004A4BDC->f420,
                                                              D_004A4BD8->body.slot);
                break;
            }
            fA8F8_86F0(0xE);
            D_004A4BD8->st = 26;
            D_004A4BDC->f14(D_004A4BD8->body.slot);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 5:
        if (callResult == 0) {
            D_004A4BD8->st = 0xE;
            ((void (*)(char *, char *, int, int))D_004A4BDC->f18)((char *)D_004A4BD8->body.pad08,
                                                                  (char *)D_004A4BD8->body.pad08, D_004A4BD8->body.slot, 515);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 0xE:
        if (callResult == 0) {
            int openResult = *res;
            if (openResult >= 0) {
                D_004A4BD8->st = 0x10;
                D_004A4BD8->body.port = openResult;
                D_004A4BDC->f38(openResult);
                break;
            }
            fA8F8_86F0(0);
            return func_003F8720((int)&D_004A4BD8->body, st);
        }
        fA8F8_86F0(callResult);
        return func_003F8720((int)&D_004A4BD8->body, st);
    case 0x10:
        if (callResult == 0) {
            D_004A4BD8->st = 17;
            D_004A4BDC->f1C(D_004A4BD8->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F8768((int)&D_004A4BD8->body, st);
    case 17:
        if (callResult == 0) {
            D_004A4BD8->st = 6;
            ((void (*)(char *, char *, int, int))D_004A4BDC->f18)((char *)D_004A4BD8->body.pad08, str_view_ico_b,
                                                                  D_004A4BD8->body.slot, 515);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 6:
        if (callResult == 0) {
                int openResult = *res;
            D_004A4BD8->st = 7;
            D_004A4BD8->body.port = openResult;
            D_004A4BDC->f30(openResult, *(int *)&D_004A4BDC->f50[0], *(int *)&D_004A4BDC->f50[4]);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 7:
        if (callResult == 0) {
            D_004A4BD8->st = 8;
            D_004A4BDC->f38(D_004A4BD8->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F8720((int)&D_004A4BD8->body, st);
    case 8:
        if (callResult == 0) {
            D_004A4BD8->st = 9;
            D_004A4BDC->f1C(D_004A4BD8->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F8768((int)&D_004A4BD8->body, st);
    case 9:
        if (callResult == 0) {
            D_004A4BD8->st = 10;
            ((void (*)(char *, char *, int, int))D_004A4BDC->f18)((char *)D_004A4BD8->body.pad08, str_icon_sys_b,
                                                                  D_004A4BD8->body.slot, 515);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 10:
        if (callResult == 0) {
                int openResult = *res;
            D_004A4BD8->body.port = openResult;
            D_004A4BD8->st = 11;
            D_004A4BDC->f30(openResult, (int)&D_004A4BDC->f50[8], 964);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 11:
        if (callResult == 0) {
            D_004A4BD8->st = 0xC;
            D_004A4BDC->f38(D_004A4BD8->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F8720((int)&D_004A4BD8->body, st);
    case 0xC:
        if (callResult == 0) {
            D_004A4BD8->st = 13;
            D_004A4BDC->f1C(D_004A4BD8->body.port);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F8768((int)&D_004A4BD8->body, st);
    case 13:
        if (callResult == 0) {
            D_004A4BD8->st = 26;
            D_004A4BDC->f14(D_004A4BD8->body.slot);
            break;
        }
        fA8F8_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 26:
        if (callResult == 0) {
            if (D_004A4BD8->op == 4) {
                *st = 2;
                return func_003F8710();
            }
            if (func_003F8710() == 0) {
                *st = 2;
                return 0;
            }
            D_004A4BD8->body.slot = func_003F85C0(D_004A4BDC->ports, D_004A4BD8->f6C);
            if (D_004A4BD8->body.slot != -1) {
                D_004A4BD8->st = 1;
                D_004A4BDC->f10(D_004A4BD8->body.slot);
                return 0;
            }
            *st = 2;
            return func_003F8710();
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FAE50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A4BD8: the pending memory-card request (0x74 bytes). body is a copy of the caller's parameter block. */
extern struct sMcReq *D_004A4BD8;

/* D_004A4BDC: the memory-card driver (0x42C bytes): callbacks and per-op / per-state handler tables. */
extern struct sMcDrv *D_004A4BDC;

extern char str_view_ico_b[];   /* "view.ico" */
extern char str_icon_sys_b[];   /* "icon.sys" */
extern void fAE50_86F0(int) __asm__("func_003F86F0");
extern char *fAE50_A7A0(char *, char *, char *) __asm__("func_003FA7A0");

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
int func_003FAE50(int callResult, int *st, int *res) {
    char path[80];

    if (st == 0) return 18;
    if (res == 0) return 18;
    if (D_004A4BD8->op != 2 && D_004A4BD8->op != 5) return 18;
    *st = 1;
    switch (D_004A4BD8->st) {
    case 1:
        fAE50_86F0(0);
        if (callResult == 0) {
            fAE50_A7A0(path, D_004A4BDC->f50, D_004A4BD8->body.name);
            D_004A4BD8->st = 2;
            ((void (*)(char *, int))D_004A4BDC->f04)(path, D_004A4BD8->body.slot);
            break;
        }
        if (callResult == 3) {
            if (D_004A4BD8->op == 2) {
                D_004A4BD8->body.slot = func_003F85C0(D_004A4BDC->ports, D_004A4BD8->f6C);
                if (D_004A4BD8->body.slot != -1) {
                    D_004A4BD8->st = 1;
                    D_004A4BDC->f10(D_004A4BD8->body.slot);
                    return 0;
                }
                *st = 2;
                return func_003F8710();
            }
            *st = 2;
            return 3;
        }
        *st = 2;
        return callResult;
    case 2:
        if (callResult == 0) {
            if (*res == 1) {
                ((void (*)(char *))D_004A4BDC->f44)((char *)D_004A4BD8->body.pad08);
                D_004A4BD8->st = 19;
                ((void (*)(char *, char *, int))D_004A4BDC->f24)((char *)D_004A4BD8->body.pad08, str_icon_sys_b,
                                                                D_004A4BD8->body.slot);
                break;
            }
            if (*res == 0) {
                fAE50_86F0(4);
                return func_003F87B0(D_004A4BD8->body.slot, st);
            }
            *st = 2;
            return 10;
        }
        fAE50_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 19:
        if (callResult == 3) {
            if (func_003F8710() == 0) fAE50_86F0(callResult);
            return func_003F87B0(D_004A4BD8->body.slot, st);
        }
        if (callResult != 4) {
            if (func_003F8710() == 0) fAE50_86F0(callResult);
        }
        D_004A4BD8->st = 20;
        ((void (*)(char *, char *, int))D_004A4BDC->f24)((char *)D_004A4BD8->body.pad08, str_view_ico_b,
                                                        D_004A4BD8->body.slot);
        break;
    case 20:
        if (callResult == 3) {
            if (func_003F8710() == 0) fAE50_86F0(callResult);
            return func_003F87B0(D_004A4BD8->body.slot, st);
        }
        if (callResult != 4) {
            if (func_003F8710() == 0) fAE50_86F0(callResult);
        }
        D_004A4BD8->st = 21;
        ((void (*)(char *, char *, int))D_004A4BDC->f24)((char *)D_004A4BD8->body.pad08,
                                                        (char *)D_004A4BD8->body.pad08, D_004A4BD8->body.slot);
        break;
    case 21:
        if (callResult == 3) {
            if (func_003F8710() == 0) fAE50_86F0(callResult);
            return func_003F87B0(D_004A4BD8->body.slot, st);
        }
        if (callResult != 4) {
            if (func_003F8710() == 0) fAE50_86F0(callResult);
        }
        D_004A4BD8->st = 18;
        ((void (*)(char *, int))D_004A4BDC->f28)((char *)D_004A4BD8->body.pad08, D_004A4BD8->body.slot);
        break;
    case 18:
        if (callResult == 0) {
            if (func_003F8710() == 0) fAE50_86F0(0);
            D_004A4BD8->st = 26;
            D_004A4BDC->f14(D_004A4BD8->body.slot);
            break;
        }
        if (func_003F8710() == 0) fAE50_86F0(callResult);
        return func_003F87B0(D_004A4BD8->body.slot, st);
    case 26:
        if (callResult == 0) {
            if (D_004A4BD8->op == 5) {
                *st = 2;
                return func_003F8710();
            }
            if (func_003F8710() == 0) {
                *st = 2;
                return 0;
            }
            D_004A4BD8->body.slot = func_003F85C0(D_004A4BDC->ports, D_004A4BD8->f6C);
            if (D_004A4BD8->body.slot != -1) {
                D_004A4BD8->st = 1;
                D_004A4BDC->f10(D_004A4BD8->body.slot);
                return 0;
            }
            *st = 2;
            return func_003F8710();
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FB278);
#ifdef SKIP_ASM
/**
 * Returns 1 (the save-entry filter behind func_003FA360 accepts everything).
 *
 * @return 1
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FB278(void) { return 1; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB280);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
static inline unsigned roundup_kb(unsigned v) { if (v & 0x3FF) return ((v >> 10) + 1) << 10; return (v >> 10) << 10; }
/**
 * Computes the space a new save needs: the record size (+0x3CC) and the icon size (+0x4) of the save description, each
 * rounded up to 1 KB, plus 5120 bytes; @p kind is set to 3.
 *
 * @param p  the save description
 * @param out  receives the byte count
 * @param kind  receives 3 (compared with the second space query)
 */
void func_003FB280(char *p, int *out, int *kind) {
    unsigned a = roundup_kb(*(unsigned *)(p + 0x3CC));
    unsigned b = roundup_kb(*(unsigned *)(p + 4));
    *out = a + b + 5120;
    *kind = 3;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB2C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char *func_003FB5E8(void);
/**
 * Stores the CRC interface (func_003FB5E8, D_00497508) through @p a when it is not 0.
 *
 * @param a  output pointer
 */
void func_003FB2C8(char **a) {
    char *r = func_003FB5E8();
    if (a) *a = r;
}
#endif

/**
 * Empty hook of the memory-card request layer (called with 1 when a request starts and 0 when some finish).
 */
void func_003FB2F8(void) {
}

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB300);
#ifdef SKIP_ASM
/**
 * Returns 0 (the base offset of seeks in a save file).
 *
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FB300(void) { return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB308);
#ifdef SKIP_ASM
/**
 * Checks the save description pointer: 0 when set, 12 when 0.
 *
 * @param saveDesc  the description
 * @return 0 or 12
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FB308(int saveDesc) { return saveDesc ? 0 : 12; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB318);
#ifdef SKIP_ASM
/**
 * Computes one entry of the CRC-32 table: eight shifts of @p crc with the reflected polynomial 0xEDB88320.
 *
 * @param crc  the table index
 * @return the table entry
 */
// From peak3project's fork (github.com/peak3project/ssx3).
unsigned int func_003FB318(unsigned int crc) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FB358);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int func_003FB318(unsigned int);
/**
 * Fills a 256-entry CRC-32 table (func_003FB318 for each index).
 *
 * @param table  the table
 */
void func_003FB358(unsigned int *table) {
    unsigned int i;
    for (i = 0; i < 256; i++) {
        *table++ = func_003FB318(i);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB3A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00451518: the CRC-32 state (func_003FB3A0 allocates the 0x400-byte table). */
struct sCrc { unsigned *table; int f04; int *f08; unsigned crc; int heap; };

extern struct sCrc D_00451518;

extern int func_00266728(int, int, int);
extern void func_003FB358b(int) __asm__("func_003FB358");
/**
 * Starts the CRC interface D_00451518: allocates its 1 KB table from @p heapId, fills it, and resets the digest (all ones)
 * and the output block.
 *
 * @param heapId  heap
 * @return 0; 5 when already started, 1 when the allocation fails
 */
int func_003FB3A0(unsigned heapId) {
    int tableAddr;
    if (D_00451518.table != 0) return 5;
    tableAddr = func_00266728(heapId, 0x400, 4);
    D_00451518.table = (unsigned *)tableAddr;
    if (tableAddr == 0) return 1;
    D_00451518.heap = heapId;
    func_003FB358b(tableAddr);
    D_00451518.f04 = 4;
    D_00451518.crc = 0xFFFFFFFF;
    D_00451518.f08 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB430);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00451518: the CRC-32 state (func_003FB3A0 allocates the 0x400-byte table). */
extern struct sCrc D_00451518;

extern int func_00266760(int, char *, int, int);
/**
 * Stops the CRC interface: frees its table to the heap it came from.
 *
 * @return 0, or 6 when not started
 */
int func_003FB430(void) {
    struct sCrc *crcState = &D_00451518;
    if (D_00451518.table == 0) return 6;
    func_00266760(crcState->heap, (char *)crcState->table, 0x400, 4);
    D_00451518.table = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB480);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00451518: the CRC-32 state (func_003FB3A0 allocates the 0x400-byte table). */
extern struct sCrc D_00451518;

/**
 * Returns whether the CRC interface is started (its table exists). Slot 2 of the CRC interface D_00497508.
 *
 * @return 1 when started, else 0
 */
int func_003FB480(void) { return D_00451518.table != 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB490);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00451518: the CRC-32 state (func_003FB3A0 allocates the 0x400-byte table). */
extern struct sCrc D_00451518;

/**
 * Resets the CRC-32 digest to all ones. Slot 3 of the CRC interface.
 *
 * @return 0, or 6 when not started
 */
int func_003FB490(void) {
    unsigned long long initialCrc = 0xFFFFFFFF;
    if (D_00451518.table == 0) return 6;
    D_00451518.crc = initialCrc;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB4C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00451518: the CRC-32 state (func_003FB3A0 allocates the 0x400-byte table). */
extern struct sCrc D_00451518;

/**
 * Adds @p n bytes to the CRC-32 digest (table-driven, reflected). Slot 4 of the CRC interface.
 *
 * @param buf  the data
 * @param n  byte count
 * @return 0; 6 when not started, 2 when @p buf is 0
 */
int func_003FB4C0(unsigned char *buf, unsigned n) {
    unsigned crc = D_00451518.crc;
    unsigned i;
    if (D_00451518.table == 0) return 6;
    if (buf == 0) return 2;
    for (i = 0; i < n; i++)
        crc = ((crc >> 8) & 0xFFFFFF) ^ D_00451518.table[(crc ^ buf[i]) & 0xFF];
    D_00451518.crc = crc;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB558);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00451518: the CRC-32 state (func_003FB3A0 allocates the 0x400-byte table). */
extern struct sCrc D_00451518;

extern int D_004A5B1C;
/**
 * Finishes the CRC-32 digest. Slot 5 of the CRC interface.
 *
 * Stores the inverted digest in D_004A5B1C and points the output block (size 4, then that pointer, at +0x4 of
 * D_00451518) at it.
 *
 * @param out  receives the output block
 * @return 0; 6 when not started, 2 when @p out is 0
 */
int func_003FB558(int **out) {
    struct sCrc *crcState = &D_00451518;
    if (D_00451518.table == 0) return 6;
    if (out == 0) return 2;
    crcState->f08 = &D_004A5B1C;
    crcState->f04 = 4;
    D_004A5B1C = ~crcState->crc;
    *out = &crcState->f04;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB5B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_00451518: the CRC-32 state (func_003FB3A0 allocates the 0x400-byte table). */
extern struct sCrc D_00451518;

/**
 * Returns the digest size of the CRC interface (4 bytes). Slot 6 of the CRC interface.
 *
 * @param out  receives 4
 * @return 0; 6 when not started, 2 when @p out is 0
 */
int func_003FB5B0(int *out) {
    if (D_00451518.table == 0) return 6;
    if (out == 0) return 2;
    *out = 4;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB5E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00497508[];
/**
 * Returns the CRC interface D_00497508: start (func_003FB3A0), stop, started, reset, update, finish, digest size.
 *
 * @return the interface table
 */
char *func_003FB5E8(void) { return D_00497508; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB5F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BF0;

/**
 * Starts the XOR cipher interface (slot 0 of D_00497528): moves it from stopped (state 4) to started without a key (6).
 *
 * @return 0, or 3 when it is not stopped
 */
int func_003FB5F8(void) {
    if (D_004A4BF0 != 4) return 3;
    D_004A4BF0 = 6;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB620);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (func_003FB650): D_004A4BE4 .. D_004A4BE8 is the key, D_004A4BEC the next key byte, D_004A4BF0
   the cipher state (4 = idle). */
extern unsigned char *D_004A4BE4;

extern unsigned char *D_004A4BE8;

extern unsigned char *D_004A4BEC;

extern int D_004A4BF0;

/**
 * Stops the XOR cipher interface (slot 1): forgets the key and returns to state 4.
 *
 * @return 0
 */
int func_003FB620(void) {
    D_004A4BE4 = 0;
    D_004A4BE8 = 0;
    D_004A4BEC = 0;
    D_004A4BF0 = 4;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB640);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A4BF0;

/**
 * Returns whether the XOR cipher interface is started (state not 4). Slot 2.
 *
 * @return 1 when started
 */
int func_003FB640(void) { return D_004A4BF0 != 4; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB650);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (func_003FB650): D_004A4BE4 .. D_004A4BE8 is the key, D_004A4BEC the next key byte, D_004A4BF0
   the cipher state (4 = idle). */
extern unsigned char *D_004A4BE4;

extern unsigned char *D_004A4BE8;

extern unsigned char *D_004A4BEC;

extern int D_004A4BF0;

/**
 * Sets the XOR cipher key (slot 3): the key runs from @p keyBytes for @p keyLen bytes and the position restarts at its first byte
 * (state 5).
 *
 * @param keyBytes  key bytes (kept by pointer)
 * @param keyLen  key length
 * @return 0; 2 for a 0 argument, 4 when stopped
 */
int func_003FB650(unsigned char *keyBytes, int keyLen) {
    if (keyBytes == 0 || keyLen == 0) return 2;
    if (D_004A4BF0 == 4) return 4;
    D_004A4BE4 = keyBytes;
    D_004A4BE8 = keyBytes + keyLen - 1;
    D_004A4BEC = keyBytes;
    D_004A4BF0 = 5;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB6A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (func_003FB650): D_004A4BE4 .. D_004A4BE8 is the key, D_004A4BEC the next key byte, D_004A4BF0
   the cipher state (4 = idle). */
extern unsigned char *D_004A4BE4;

extern unsigned char *D_004A4BE8;

extern unsigned char *D_004A4BEC;

extern int D_004A4BF0;

/**
 * Encrypts @p n bytes in place (slot 4): XORs each with the next key byte, wrapping to the key's start, and continues
 * from the key position the previous call left (state 7).
 *
 * @param buf  the data
 * @param n  byte count
 * @return 0; 2 for a 0 argument, or the current state when it is neither keyed (5) nor encrypting (7)
 */
int func_003FB6A0(unsigned char *buf, unsigned n) {
    unsigned i;
    if (buf == 0 || n == 0) return 2;
    if (D_004A4BF0 != 5 && D_004A4BF0 != 7) return D_004A4BF0;
    for (i = 0; i < n; i++) {
        buf[i] ^= *D_004A4BEC;
        if (D_004A4BEC == D_004A4BE8) D_004A4BEC = D_004A4BE4;
        else D_004A4BEC = D_004A4BEC + 1;
    }
    D_004A4BF0 = 7;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB730);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (func_003FB650): D_004A4BE4 .. D_004A4BE8 is the key, D_004A4BEC the next key byte, D_004A4BF0
   the cipher state (4 = idle). */
extern unsigned char *D_004A4BE4;

extern unsigned char *D_004A4BE8;

extern unsigned char *D_004A4BEC;

extern int D_004A4BF0;

/**
 * Decrypts @p n bytes in place (slot 5): the same XOR as func_003FB6A0, allowed when keyed (5) or decrypting (8), and
 * leaves state 8.
 *
 * @param buf  the data
 * @param n  byte count
 * @return 0; 2 for a 0 argument, or the current state when it is neither 5 nor 8
 */
int func_003FB730(unsigned char *buf, unsigned n) {
    unsigned i;
    if (buf == 0 || n == 0) return 2;
    if (D_004A4BF0 != 5 && D_004A4BF0 != 8) return D_004A4BF0;
    for (i = 0; i < n; i++) {
        buf[i] ^= *D_004A4BEC;
        if (D_004A4BEC == D_004A4BE8) D_004A4BEC = D_004A4BE4;
        else D_004A4BEC = D_004A4BEC + 1;
    }
    D_004A4BF0 = 8;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB7C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* XOR key window (func_003FB650): D_004A4BE4 .. D_004A4BE8 is the key, D_004A4BEC the next key byte, D_004A4BF0
   the cipher state (4 = idle). */
extern unsigned char *D_004A4BE4;

extern unsigned char *D_004A4BEC;

extern int D_004A4BF0;

/**
 * Rewinds the XOR cipher to the key's first byte and back to state 5 (slot 6); does nothing unless a key is set.
 *
 * @return 0
 */
int func_003FB7C0(void) {
    if (D_004A4BF0 == 5 || D_004A4BF0 == 7 || D_004A4BF0 == 8) {
        D_004A4BF0 = 5;
        D_004A4BEC = D_004A4BE4;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB800);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00497528[];
/**
 * Returns the XOR cipher interface D_00497528: start, stop, started, set key, encrypt, decrypt, rewind.
 *
 * @return the interface table
 */
char *func_003FB800(void) { return D_00497528; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB810);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *func_003FB810_impl(char *p) __asm__("func_003FB810");
/**
 * Offsets an error of the memory-card request layer into the save-record manager's range: nonzero codes get 100 added.
 *
 * @param errorCode  the error (declared as a pointer)
 * @return 0, or @p errorCode + 100
 */
char *func_003FB810_impl(char *errorCode) {
    if (errorCode == 0) return 0;
    return errorCode + 100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB828);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *func_003FB828_impl(char *p) __asm__("func_003FB828");
/**
 * Offsets an XOR cipher error into the save-record manager's range: nonzero codes get 200 added.
 *
 * @param errorCode  the error (declared as a pointer)
 * @return 0, or @p errorCode + 200
 */
char *func_003FB828_impl(char *errorCode) {
    if (errorCode == 0) return 0;
    return errorCode + 200;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB840);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *func_003FB840_impl(char *p) __asm__("func_003FB840");
/**
 * Offsets a CRC interface error into the save-record manager's range: nonzero codes get 300 added.
 *
 * @param errorCode  the error (declared as a pointer)
 * @return 0, or @p errorCode + 300
 */
char *func_003FB840_impl(char *errorCode) {
    if (errorCode == 0) return 0;
    return errorCode + 300;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB858);
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
void func_003FB858(unsigned memType, int *x, int *y) {
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FB898);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003FB858b(int, int *, int *) __asm__("func_003FB858");
int func_003FB898(int a, int b);
/**
 * Returns the block size of a save record: @p size plus its 12-byte header, rounded up to the type's granularity
 * (func_003FB858, which is 1, so @p size + 12).
 *
 * @param size  record data size
 * @param memType  memory type
 * @return the block size
 */
int func_003FB898(int size, int memType) {
    int x = 4;
    unsigned y = 1;
    unsigned rawSize;
    unsigned pad;
    func_003FB858b(memType, &x, (int *)&y);
    rawSize = size + 0xC;
    if (rawSize < y) return y;
    pad = (y - rawSize % y) % y;
    return size + 0xC + pad;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB910);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL[];

extern int memcmp(int, char *, int);
/**
 * Returns the 12-byte header in front of a save-record buffer from func_003FCE10.
 *
 * @param userPtr  the record buffer
 * @param checkTag  nonzero to require the header to still hold "TAG_SENTINEL"
 * @return @p userPtr - 12, or 0 when the check fails
 */
int func_003FB910(int userPtr, int checkTag) {
    int header = userPtr - 0xC;
    if (checkTag & 0xFF) {
        if (memcmp(header, str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL, 0xC) != 0) header = 0;
    }
    return header;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB958);
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
void func_003FB958(W3FB958 *dst, int recordKey, int recordSize, W3FB958 *recordCrc) {
    V3FB958 packed;
    *(int *)&packed.x = recordKey;
    *(int *)&packed.y = recordSize;
    dst[0] = packed.x;
    dst[1] = packed.y;
    dst[2] = *recordCrc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB9A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL[];

/**
 * Writes "TAG_SENTINEL" (12 bytes) back into a save-record header area.
 *
 * @param dst  the header
 */
void func_003FB9A0(char *dst) {
    __builtin_memcpy(dst, str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL, 12);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB9D0);
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
void func_003FB9D0(char *src, char *outWord0, char *outWord1, char *outWord2) {
    if (outWord0) __builtin_memcpy(outWord0, src, 4);
    if (outWord1) __builtin_memcpy(outWord1, src + 4, 4);
    if (outWord2) __builtin_memcpy(outWord2, src + 8, 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBA20);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
struct sMcEnt { int key; int a; int b; int c; };

struct sMcKey { char b[4]; };

struct sMcIfA { int (*f00)(int); int (*f04)(void); int pad08; int (*f0C)(char *, int); int (*f10)(int, int); int (*f14)(int, int); int (*f18)(void); };

struct sMcIfB { int (*f00)(int); int (*f04)(void); int pad08; int (*f0C)(int, int, int); int (*f10)(int, int); int (*f14)(int); };

struct sMcState {
    struct sMcParams params;            /* 0x00 */
    char *f64;
    int f68;
    struct sMcIfA *f6C;
    struct sMcIfB *f70;
    int f74;
    int f78;                            /* current step */
    int f7C;
    int f80;
    int f84;
    struct sMcKey f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    struct sMcEnt *f9C;
    int fA0;
    int fA4;
    char fA8;
};

extern struct sMcState *D_004A5B20;

/**
 * Finds the @p n-th entry with key @p key in the open save's record table (D_004A5B20 +0x9C, +0x94 entries).
 *
 * @param key  record key
 * @param n  index among the entries with that key
 * @return the entry (key, offset, size, CRC), or 0
 */
struct sMcEnt *func_003FBA20(int key, int n) {
    struct sMcState *state = D_004A5B20;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FBA78);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

/**
 * Sets the CRC of the @p n-th record-table entry with key @p key.
 *
 * @param key  record key
 * @param n  index among the entries with that key
 * @param val  the CRC
 */
void func_003FBA78(int key, int n, int *val) {
    struct sMcState *state = D_004A5B20;
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
INCLUDE_ASM("seg/seg_2F41F8", func_003FBAD0);
#ifdef SKIP_ASM
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
#include "fakematch.h"
extern struct sMcState *D_004A5B20;

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
// FAKEMATCH: FM_OPAQUE(e) after the e->c store hides that e == t, so the pre-if `t = &f9C[f94]` (v0) is copied into e (a3) at the join as in retail; e->c is stored through an int view (retail reloads D_004A5B20 after that store, which a struct-field store lets sched hoist); plain form 67.84% (91.38% with the struct-field store).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int func_003FBAD0(int key, int size, int *val) {
    struct sMcEnt *e, *newEntry;
    int len;
    int end;
    if (D_004A5B20->f94 == D_004A5B20->f90) return D_004A5B20->f98;
    newEntry = D_004A5B20->f9C + D_004A5B20->f94;
    if (D_004A5B20->f94 == 0) D_004A5B20->f9C->a = 0;
    e = newEntry;
    e->key = key;
    e->b = size;
    len = size + 0xC;
    if (len == 0) len = 1;
    *(int *)&e->c = *val;
    FM_OPAQUE(e);
    D_004A5B20->f94++;
    end = e->a + len;
    D_004A5B20->f98 = end;
    if (D_004A5B20->f94 != D_004A5B20->f90) e[1].a = end;
    return e->a;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBB50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB840c(int) __asm__("func_003FB840");
int func_003FBB50_impl(int a, int b, int c) __asm__("func_003FBB50");
/**
 * Computes the CRC-32 of a buffer through the CRC interface (+0x70): reset, update, finish.
 *
 * @param data  the data
 * @param byteCount  byte count
 * @param digestOut  receives the output block (size, then a pointer to the digest)
 * @return 0, or the CRC error + 300
 */
int func_003FBB50_impl(int data, int byteCount, int digestOut) {
    struct sMcIfB *vt = D_004A5B20->f70;
    int result = vt->f0C(data, byteCount, digestOut);
    if (result == 0) {
        result = vt->f10(data, byteCount);
        if (result == 0) {
            result = vt->f14(digestOut);
            if (result == 0) return 0;
        }
    }
    return func_003FB840c(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBBE0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FBB50(void *, int, int **);
/**
 * Computes the CRC-32 of the open save's record table (16 bytes per entry) into @p out; 0 when the table is empty.
 *
 * @param out  receives the 4 digest bytes
 */
void func_003FBBE0(struct sMcKey *out) {
    struct sMcState *state = D_004A5B20;
    int *digestBlock;
    int sz = state->f94 << 4;
    if (sz == 0) {
        *(int *)out = 0;
    } else {
        func_003FBB50(state->f9C, sz, &digestBlock);
        *out = *(struct sMcKey *)digestBlock[1];
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBC40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int fB828b(int) __asm__("func_003FB828");
int func_003FBC40_impl(int x, int y) __asm__("func_003FBC40");
/**
 * Encrypts a buffer with the save cipher (+0x6C): sets the stored key (+0x64, +0x68), encrypts, rewinds. Does nothing
 * without a cipher.
 *
 * @param x  the data
 * @param y  byte count
 * @return 0, or the cipher error + 200
 */
int func_003FBC40_impl(int x, int y) {
    int result;
    if (D_004A5B20->f6C == 0) return 0;
    result = D_004A5B20->f6C->f0C(D_004A5B20->f64, D_004A5B20->f68);
    if (result == 0) {
        result = D_004A5B20->f6C->f10(x, y);
        if (result == 0) {
            result = D_004A5B20->f6C->f18();
            if (result == 0) return 0;
        }
    }
    return fB828b(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBCD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int fB828(int) __asm__("func_003FB828");
int func_003FBCD8_impl(int x, int y) __asm__("func_003FBCD8");
/**
 * Decrypts a buffer with the save cipher: as func_003FBC40 with the decrypt slot.
 *
 * @param x  the data
 * @param y  byte count
 * @return 0, or the cipher error + 200
 */
int func_003FBCD8_impl(int x, int y) {
    int result;
    if (D_004A5B20->f6C == 0) return 0;
    result = D_004A5B20->f6C->f0C(D_004A5B20->f64, D_004A5B20->f68);
    if (result == 0) {
        result = D_004A5B20->f6C->f14(x, y);
        if (result == 0) {
            result = D_004A5B20->f6C->f18();
            if (result == 0) return 0;
        }
    }
    return fB828(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBD70);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003F99B8(int, int, int);
/**
 * Starts opening a save (func_003F99B8) as save-record operation @p operation at step 4, with an empty record table.
 *
 * @param saveName  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @param operation  the operation (4 open, 5 reopen and verify)
 * @return 0, or the request error + 100
 */
int func_003FBD70(int saveName, int unitCode, int searchDir, int operation) {
    int result = func_003F99B8(saveName, unitCode, searchDir);
    struct sMcState *state = D_004A5B20;
    state->f74 = operation;
    state->f78 = 4;
    state->f94 = 0;
    state->f98 = 0;
    return func_003FB810(result);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBDB8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003F9C48(struct sMcState *, int, int);
extern int func_003FB810(int);
/**
 * Save-record step 9: seeks to the start of the open save file to read its record headers.
 *
 * @return 0, or the request error + 100
 */
int func_003FBDB8(void) {
    struct sMcState *state = D_004A5B20;
    state->f78 = 9;
    return func_003FB810(func_003F9C48(state, 0, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBDF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB898_BDF0(int, int) __asm__("func_003FB898");
extern int func_003F9DC0(struct sMcState *, int, int);
extern int func_003FB810(int);
/**
 * Save-record step 7: reads the next 12-byte record header of the open file into the header buffer (+0x80).
 *
 * @return 0, or the request error + 100
 */
int func_003FBDF0(void) {
    struct sMcState *state = D_004A5B20;
    state->f78 = 7;
    return func_003FB810(func_003F9DC0(D_004A5B20, state->f80, func_003FB898_BDF0(0, 1)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003FBE40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB810(int);

extern int fBCD8(int, int) __asm__("func_003FBCD8");
extern void fB9D0(int, int *, int *, char *) __asm__("func_003FB9D0");
extern int fBAD0(int, int, char *) __asm__("func_003FBAD0");
extern int f9C48(struct sMcState *, int, int) __asm__("func_003F9C48");
/**
 * Save-record step 11: decrypts the record header just read, adds the record to the table (func_003FBAD0) and seeks
 * past its data to the next header.
 *
 * @return 0, the cipher or request error, or 5 when the table is full
 */
int func_003FBE40(void) {
    int x[2];
    int result;
    int recordKey, recordSize;
    x[1] = 0;
    D_004A5B20->f78 = 11;
    result = fBCD8(D_004A5B20->f80, 0xC);
    if (result != 0) return result;
    fB9D0(D_004A5B20->f80, &x[0], &x[1], D_004A5B20->f88.b);
    recordSize = x[1];
    recordKey = x[0];
    result = fBAD0(recordKey, recordSize, D_004A5B20->f88.b);
    if (D_004A5B20->f98 == result) return 5;
    return func_003FB810(f9C48(D_004A5B20, D_004A5B20->f98, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBED0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB898_BED0(int, int) __asm__("func_003FB898");

extern int func_003FB810(int);

extern int func_003FB910(int, int);
extern void fB958(int, int, int, char *) __asm__("func_003FB958");
extern int fBC40(int, int) __asm__("func_003FBC40");
extern int f9F00(struct sMcState *, int, int) __asm__("func_003F9F00");
/**
 * Save-record step 6: writes a record.
 *
 * Fills the header in front of the record buffer (+0x7C; 4 when its sentinel is gone) with the key, size and CRC,
 * encrypts the header and the data, and writes both (func_003F9F00).
 *
 * @return 0, the cipher error, or the request error + 100
 */
int func_003FBED0(void) {
    int header;
    int result;
    D_004A5B20->f78 = 6;
    header = func_003FB910(D_004A5B20->f7C, 1);
    if (header == 0) return 4;
    fB958(header, D_004A5B20->f8C, D_004A5B20->f84, D_004A5B20->f88.b);
    result = fBC40(header, 0xC);
    if (result != 0) return result;
    result = fBC40(D_004A5B20->f7C, D_004A5B20->f84);
    if (result != 0) return result;
    result = func_003FB898_BED0(D_004A5B20->f84, 2);
    return func_003FB810(f9F00(D_004A5B20, header, result));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBF80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB910(int, int);

extern int func_003FB898_BF80(int, int) __asm__("func_003FB898");
extern int func_003F9DC0(struct sMcState *, int, int);
extern int func_003FB810(int);
/**
 * Save-record step 10: reads a record's header and data into the header in front of the record buffer.
 *
 * @return 0, or the request error + 100
 */
int func_003FBF80(void) {
    struct sMcState *state = D_004A5B20;
    int header;
    state->f78 = 10;
    header = func_003FB910(state->f7C, 0);
    return func_003FB810(func_003F9DC0(D_004A5B20, header, func_003FB898_BF80(D_004A5B20->f84, 1)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FBFE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

typedef struct { int tag; int p; } P3FBFE0;
typedef struct { int a; int b; P3FBFE0 *p; } L3FBFE0;
extern int func_003FB910(int, int);
extern int fBCD8(int, int) __asm__("func_003FBCD8");
extern void fB9D0(int, int *, int *, char *) __asm__("func_003FB9D0");
extern int fBB50(int, int, P3FBFE0 **) __asm__("func_003FBB50");
extern int f5FC8(int, char *, int) __asm__("memcmp");
/**
 * Verifies a record just read: decrypts its header, checks the key (11) and size (110), decrypts the data and compares
 * its CRC-32 with the header's.
 *
 * @return 0 when the record is valid, 10 when the CRC differs, 11 or 110, or the cipher / CRC error
 */
int func_003FBFE0(void) {
    L3FBFE0 x; int header; int result; int err = 10; int hh;
    x.p = 0; hh = D_004A5B20->f7C; x.b = 0;
    D_004A5B20->f78 = err;
    header = func_003FB910(hh, 0);
    result = fBCD8(header, 0xC);
    if (result != 0) return result;
    fB9D0(header, &x.a, &x.b, D_004A5B20->f88.b);
    if (x.a != D_004A5B20->f8C) return 11;
    if (x.b != D_004A5B20->f84) return 110;
    result = fBCD8(D_004A5B20->f7C, x.b);
    if (result != 0) return result;
    result = fBB50(D_004A5B20->f7C, D_004A5B20->f84, &x.p);
    if (result != 0) return result;
    if (x.p->tag != 4) return 11;
    if (f5FC8(x.p->p, D_004A5B20->f88.b, 4) == 0) err = 0;
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC0D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

/**
 * Returns whether the save-record manager D_004A5B20 is open.
 *
 * @return 1 when open
 */
int func_003FC0D8(void) { return D_004A5B20 != 0; }
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003FC0E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

typedef struct { int f0; int f4; int f8; char *fC; int f10; struct sMcIfA *f14; int f18; } P0E8;
extern int func_003FC0D8(void);
extern int func_003F9218(int, int, int);
extern int func_003FB810(int);
extern int func_003FB840c(int) __asm__("func_003FB840");
extern int fB828(int) __asm__("func_003FB828");
extern void *c0e8_6728(int, int, int) __asm__("func_00266728");
extern void c0e8_A6C8(struct sMcIfB **) __asm__("func_003FA6C8");
extern int func_003FCE10(char *, int, int, int);
extern void func_0041605C(char *, char *, int);
/**
 * Opens the save-record manager.
 *
 * @p openParams holds: +0x0 the record limit, +0x4 the card unit list, +0x8 the 17 card callbacks, +0xC / +0x10 the cipher key
 * and its length, +0x14 the cipher interface (may be 0), +0x18 the heap. Opens the request layer (func_003F9218),
 * allocates the 172-byte state, starts the CRC interface and the cipher, copies the key, allocates the record table (16
 * bytes per record) and the 12-byte header buffer (+0x80). Likely bug: the header buffer is allocated from +0xA0 before
 * +0xA0 is set to the heap.
 *
 * @param openParams  the open parameters
 * @return 0; 1 when already open, 6 when an allocation fails, or a layer error offset by 100 / 200 / 300
 */
int func_003FC0E8(P0E8 *openParams) {
    int result;
    struct sMcIfA *cb;
    if (func_003FC0D8() != 0) return 1;
    result = func_003F9218(openParams->f4, openParams->f8, openParams->f18);
    if (result != 0) return func_003FB810(result);
    D_004A5B20 = c0e8_6728(openParams->f18, 172, 4);
    if (D_004A5B20 == 0) return 6;
    c0e8_A6C8(&D_004A5B20->f70);
    result = D_004A5B20->f70->f00(openParams->f18);
    if (result != 0) return func_003FB840c(result);
    D_004A5B20->f6C = cb = openParams->f14;
    if (cb != 0) {
        int n;
        result = cb->f00(openParams->f18);
        if (result != 0) return fB828(result);
        n = openParams->f10;
        D_004A5B20->f68 = n;
        D_004A5B20->f64 = c0e8_6728(openParams->f18, n, 4);
        if (D_004A5B20->f64 == 0) return 6;
        func_0041605C(D_004A5B20->f64, openParams->fC, D_004A5B20->f68);
    }
    D_004A5B20->f90 = openParams->f0;
    D_004A5B20->f94 = 0;
    D_004A5B20->f98 = 0;
    D_004A5B20->f9C = c0e8_6728(openParams->f18, openParams->f0 << 4, 4);
    if (D_004A5B20->f9C == 0) return 6;
    result = func_003FCE10((char *)&D_004A5B20->f80, D_004A5B20->fA0, 0, 1);
    if (result != 0) return result;
    D_004A5B20->f80 -= 0xC;
    D_004A5B20->fA0 = openParams->f18;
    D_004A5B20->fA8 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC260);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FCF00(int, int, int, int);

// 99.47: only $2/$3 swap between D ptr and q (f6C) in blocks 3,7 (target D in $2). tried decl order sweep, q vs int v, permuter 4min nothing. Names prefixed c260_ in file to avoid clashes
extern int func_003FC0D8(void);
extern void c260_B9A0(int) __asm__("func_003FB9A0");
extern void c260_6760(int, void *, int, int) __asm__("func_00266760");
extern int c260_B828(int) __asm__("func_003FB828");
extern int c260_B840(int) __asm__("func_003FB840");
extern int c260_B810(int, struct sMcState *) __asm__("func_003FB810");
extern int c260_9530(void) __asm__("func_003F9530");
/**
 * Closes the save-record manager: frees the header buffer, the record table and the key, stops the cipher and the CRC
 * interface, closes the request layer (func_003F9530) and frees the state.
 *
 * @return 0; 2 when not open, or a layer error offset by 100 / 200 / 300
 */
int func_003FC260(void) {
    int result;
    int userBuf;
    struct sMcIfA *cipher;
    if (func_003FC0D8() == 0) return 2;
    c260_B9A0(D_004A5B20->f80);
    userBuf = D_004A5B20->f80 + 0xC;
    D_004A5B20->f80 = userBuf;
    result = func_003FCF00(userBuf, D_004A5B20->fA0, 0, 1);
    if (result != 0) return result;
    c260_6760(D_004A5B20->fA0, D_004A5B20->f9C, D_004A5B20->f90 * 16, 4);
    cipher = D_004A5B20->f6C;
    D_004A5B20->f9C = 0;
    if (cipher != 0) {
        result = cipher->f04();
        if (result != 0) return c260_B828(result);
        c260_6760(D_004A5B20->fA0, D_004A5B20->f64, D_004A5B20->f68, 4);
        D_004A5B20->f64 = 0;
    }
    D_004A5B20->f6C = 0;
    result = D_004A5B20->f70->f04();
    if (result != 0) return c260_B840(result);
    D_004A5B20->f70 = 0;
    result = c260_9530();
    if (result != 0) return c260_B810(result, D_004A5B20);
    c260_6760(D_004A5B20->fA0, D_004A5B20, 172, 4);
    D_004A5B20 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC390);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003F9818(char *str, int a, int b);
extern int func_003FB810(int);
/**
 * Starts creating a save (save-record operation 1, func_003F9818).
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @return 0, or the request error + 100
 */
int func_003FC390(char *str, int unitCode, int searchDir) {
    struct sMcState *state = D_004A5B20;
    state->f74 = 1;
    state->f78 = 1;
    return func_003FB810(func_003F9818(str, unitCode, searchDir));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC3C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003F9698(char *str, int a, int b);
extern int func_003FB810(int);
/**
 * Starts deleting a save (save-record operation 2, func_003F9698).
 *
 * @param str  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @return 0, or the request error + 100
 */
int func_003FC3C0(char *str, int unitCode, int searchDir) {
    struct sMcState *state = D_004A5B20;
    state->f74 = 2;
    state->f78 = 2;
    return func_003FB810(func_003F9698(str, unitCode, searchDir));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC3F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int fBD70(char *, int, int, int) __asm__("func_003FBD70");
int func_003FC3F0_impl(struct sMcParams *a) __asm__("func_003FC3F0");
/**
 * Starts reopening a save from parameters kept from an earlier open (operation 5): copies them, opens the save by its
 * name (+0x48) on its unit (+0x4); when the headers have been read, the record table's CRC must equal the kept one
 * (+0x60).
 *
 * @param savedParams  the kept 100-byte save parameters
 * @return 0, or the request error + 100
 */
int func_003FC3F0_impl(struct sMcParams *savedParams) {
    D_004A5B20->params = *savedParams;
    return fBD70((char *)savedParams + 0x48, savedParams->w[1], -1, 5);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003FC4C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FBD70(int, int, int, int);
/**
 * Starts opening a save and reading its record table (save-record operation 4, func_003FBD70).
 *
 * @param saveName  the save name
 * @param unitCode  card unit code, or -1 for any usable unit
 * @param searchDir  search direction
 * @return 0, or the request error + 100
 */
int func_003FC4C8(int saveName, int unitCode, int searchDir) { return func_003FBD70(saveName, unitCode, searchDir, 4); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC4E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003F9B38(char *);
extern int func_003FB810(int);
extern void func_003FBBE0b(char *) __asm__("func_003FBBE0");
/**
 * Starts closing an open save (save-record operation 3): stores the record table's CRC in the caller's parameters
 * (+0x60) and the manager's, clears the table and closes the file (func_003F9B38).
 *
 * @param x  the save parameters
 * @return 0, or the request error + 100
 */
int func_003FC4E8(char *x) {
    struct sMcState *state;
    func_003FBBE0b(x + 0x60);
    /* stored through a pointer, not params.w[24]: the target reloads D_004A5B20 after this store */
    *(D_004A5B20->params.w + 24) = *(int *)(x + 0x60);
    state = D_004A5B20;
    state->f74 = 3;
    state->f78 = 3;
    state->f98 = 0;
    state->f94 = 0;
    return func_003FB810(func_003F9B38(x));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003FC548);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB810(int);

// (prototype from their definition at line 3805)
struct sMcEnt *func_003FBA20(int key, int n);

typedef struct { int w[25]; } __attribute__((aligned(8))) S64C548;
extern int fBB50w(int, int, int *) __asm__("func_003FBB50");
extern int fBAD0w(int, int, void *) __asm__("func_003FBAD0");
extern void fBA78w(int, int, void *) __asm__("func_003FBA78");
extern void fBBE0w(void *) __asm__("func_003FBBE0");
extern int f9C48w(void *, int, int) __asm__("func_003F9C48");
/**
 * Starts writing a record to an open save (save-record operation 6).
 *
 * Copies the save parameters, computes the data's CRC and either updates the @p recordIndex-th record with key @p recordKey or appends a
 * new one at the end of the file; recomputes the table CRC into @p saveParams +0x60 and seeks to the record's offset.
 * func_003FBED0 then writes it. Quirk: when the CRC computation fails it reaches its end without a return value.
 *
 * @param saveParams  the open save's parameters
 * @param recordKey  record key
 * @param recordIndex  index among the records with that key
 * @param recordBuf  the record data (a buffer from func_003FCE10)
 * @param recordSize  data size
 * @return 0, or the request error + 100
 */
int func_003FC548(S64C548 *saveParams, int recordKey, int recordIndex, int recordBuf, int recordSize) {
    int *digestBlock[4];
    int writeOffset;
    void* k;
    digestBlock[0] = 0;
    D_004A5B20->f74 = 6;
    D_004A5B20->f78 = 5;
    __builtin_memcpy(D_004A5B20, saveParams, 100);
    D_004A5B20->f84 = recordSize;
    D_004A5B20->f7C = recordBuf;
    D_004A5B20->f8C = recordKey;
    k = func_003FBA20(recordKey, recordIndex);
    if (fBB50w(D_004A5B20->f7C, D_004A5B20->f84, (int *)digestBlock) == 0) {
        D_004A5B20->f88 = *(struct sMcKey *)((int *)digestBlock[0])[1];
        if (k == 0) {
            writeOffset = D_004A5B20->f98;
            fBAD0w(recordKey, D_004A5B20->f84, &D_004A5B20->f88);
        } else {
            writeOffset = ((int *)k)[1];
            fBA78w(recordKey, recordIndex, &D_004A5B20->f88);
        }
        fBBE0w((char *)saveParams + 0x60);
        return func_003FB810(f9C48w(saveParams, writeOffset, 0));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC708);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB810(int);

// (prototype from their definition at line 3805)
struct sMcEnt *func_003FBA20(int key, int n);

extern int c708_9C48(void *, int, int) __asm__("func_003F9C48");
int func_003FC708_impl(struct sMcParams *src, int a, int b, int c, int d) __asm__("func_003FC708");
/**
 * Starts reading a record of an open save (save-record operation 8): finds the @p recordIndex-th record with key @p recordKey and seeks
 * to it. func_003FBF80 then reads it and func_003FBFE0 checks it.
 *
 * @param src  the open save's parameters
 * @param recordKey  record key
 * @param recordIndex  index among the records with that key
 * @param recordBuf  the destination (a buffer from func_003FCE10)
 * @param recordSize  expected data size
 * @return 0; 7 when there is no such record, 110 when its size differs, or the request error + 100
 */
int func_003FC708_impl(struct sMcParams *src, int recordKey, int recordIndex, int recordBuf, int recordSize) {
    struct sMcState *state = D_004A5B20;
    struct sMcEnt *entry;
    int readOffset;
    state->f74 = 8;
    state->f78 = 5;
    state->params = *src;
    D_004A5B20->f7C = recordBuf;
    D_004A5B20->f8C = recordKey;
    D_004A5B20->f84 = recordSize;
    entry = func_003FBA20(recordKey, recordIndex);
    if (entry == 0) return 7;
    if (recordSize != entry->b) return 110;
    readOffset = entry->a;
    D_004A5B20->fA4 = readOffset;
    return func_003FB810(c708_9C48(src, readOffset, 0));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC850);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FC0D8(void);
int func_003FC850_impl(int a, int key, int *out) __asm__("func_003FC850");
/**
 * Counts the records with key @p key in the open save.
 *
 * @param saveParams  the save parameters (only checked for 0)
 * @param key  record key
 * @param out  receives the count
 * @return 0; 2 when the manager is not open, 4 for a 0 argument
 */
int func_003FC850_impl(int saveParams, int key, int *out) {
    struct sMcState *state;
    struct sMcEnt *entry, *end;
    if (!func_003FC0D8()) return 2;
    if (saveParams == 0) return 4;
    if (out == 0) return 4;
    *out = 0;
    state = D_004A5B20;
    end = state->f9C + state->f94;
    entry = state->f9C;
    for (; entry < end; entry++) {
        if (key == entry->key) *out = *out + 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC8F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
// (prototype from their definition at line 3805)
struct sMcEnt *func_003FBA20(int key, int n);

extern int func_003FC0D8(void);
int func_003FC8F8_impl(int a, int b, int c, int *out) __asm__("func_003FC8F8");
/**
 * Returns the size of the @p recordIndex-th record with key @p recordKey in the open save.
 *
 * @param saveParams  the save parameters (only checked for 0)
 * @param recordKey  record key
 * @param recordIndex  index among the records with that key
 * @param out  receives the size (0 when not found)
 * @return 0; 7 when not found, 2 when the manager is not open, 4 for a 0 argument
 */
int func_003FC8F8_impl(int saveParams, int recordKey, int recordIndex, int *out) {
    int err = 0;
    struct sMcEnt *entry;
    if (!func_003FC0D8()) return 2;
    if (saveParams == 0) return 4;
    if (out == 0) return 4;
    *out = 0;
    entry = func_003FBA20(recordKey, recordIndex);
    if (entry) *out = entry->b;
    else err = 7;
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FC990);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

int func_003FC990_impl(struct sMcParams *dst) __asm__("func_003FC990");
/**
 * Copies the open save's 100-byte parameters (request body and table CRC) to @p dst.
 *
 * @param dst  destination
 * @return 0, or 4 when @p dst is 0
 */
int func_003FC990_impl(struct sMcParams *dst) {
    if (dst == 0) return 4;
    *dst = D_004A5B20->params;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FCA40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* D_004A5B20: the memory-card session (0xAC bytes, allocated by func_003FC0E8). params is the caller's 0x64-byte
   block; f9C points to f94 entries; f6C / f70 are the two driver interfaces. Field names are offsets. */
extern struct sMcState *D_004A5B20;

extern int func_003FB810(int);
extern int func_003FB828(int);
extern int func_003FBCD8(int, int);
extern void fCA40_B9A0(int) __asm__("func_003FB9A0");
extern int fCA40_5FC8(void *, void *, int) __asm__("memcmp");

/**
 * Polls the save-record manager and runs the next step of its operation.
 *
 * Polls the request layer (func_003FA040). Write: after the seek writes the record (func_003FBED0); after the write
 * checks the count, decrypts the data back and recomputes the table CRC. Read: after the seek reads the record
 * (func_003FBF80); after the read checks the count and verifies it (func_003FBFE0). Open / reopen: after the open seeks
 * to the start; then reads header after header (func_003FBDF0, func_003FBE40) until the file or the table ends (a
 * just-created save has none), and finishes with the table CRC (reopen: 9 when it differs from the kept one). Other
 * operations finish when the request does.
 *
 * @param st  receives 1 while running, 2 when finished
 * @param res  the request's result word; receives the record size after a read or write
 * @return 0 or an error code (request errors + 100)
 */
int func_003FCA40(int *st, int *res) {
    int cardResult;
    int key;

    if (func_003FC0D8() == 0) return 2;
    if (st == 0) return 4;
    if (res == 0) return 4;
    cardResult = func_003FA040(st, res);
    if (st[0] == 0) {
        if (D_004A5B20->f74 != 0) return 8;
    }
    if (st[0] != 2) return func_003FB810(cardResult);
    if (cardResult != 0 && D_004A5B20->f74 != 4 && D_004A5B20->f74 != 5) return func_003FB810(cardResult);
    if (D_004A5B20->f74 == 1 && cardResult == 0) D_004A5B20->fA8 = 1;
    if (D_004A5B20->f74 != 1 && D_004A5B20->f74 != 4) D_004A5B20->fA8 = 0;
    if (D_004A5B20->f74 == 6) {
        if (D_004A5B20->f78 == 5) {
            *st = 1;
            return func_003FBED0();
        }
        if (D_004A5B20->f78 == 6) {
            int header = func_003FB910(D_004A5B20->f7C, 0);
            int stepError;
            fCA40_B9A0(header);
            if (func_003FB898(D_004A5B20->f84, 2) != *res) return 110;
            *res = D_004A5B20->f84;
            stepError = func_003FBCD8(D_004A5B20->f7C, D_004A5B20->f84);
            if (stepError != 0) return func_003FB828(stepError);
            if (cardResult == 0) func_003FBBE0((struct sMcKey *)&D_004A5B20->params.w[24]);
        }
    }
    if (D_004A5B20->f74 == 8) {
        if (D_004A5B20->f78 == 5) {
            *st = 1;
            return func_003FBF80();
        }
        if (D_004A5B20->f78 == 10) {
            int header = func_003FB910(D_004A5B20->f7C, 0);
            int stepError;
            if (func_003FB898(D_004A5B20->f84, 1) == *res) {
                *res = D_004A5B20->f84;
                stepError = func_003FBFE0();
            } else {
                stepError = 110;
            }
            fCA40_B9A0(header);
            return stepError;
        }
    }
    if (D_004A5B20->f74 == 4 || D_004A5B20->f74 == 5) {
        if (D_004A5B20->f78 == 4) {
            if (cardResult != 0) {
                D_004A5B20->fA8 = 0;
                return func_003FB810(cardResult);
            }
            func_003F95E8((char *)D_004A5B20);
            *st = 1;
            return func_003FBDB8();
        }
        if (D_004A5B20->f78 == 9) {
            if (cardResult != 0) {
                D_004A5B20->fA8 = 0;
                return func_003FB810(cardResult);
            }
            if ((unsigned char)D_004A5B20->fA8 != 0) {
                D_004A5B20->fA8 = 0;
                *st = 2;
                return 0;
            }
            D_004A5B20->fA8 = 0;
            *st = 1;
            return func_003FBDF0();
        }
        if (D_004A5B20->f78 == 7) {
            if (cardResult != 0 && cardResult != 10) {
                D_004A5B20->fA8 = 0;
                return func_003FB810(cardResult);
            }
            if (cardResult == 10 || D_004A5B20->f94 == D_004A5B20->f90) {
                *st = 2;
                fCA40_B9A0(D_004A5B20->f80);
                func_003FBBE0((struct sMcKey *)&key);
                if (D_004A5B20->f74 == 5) return fCA40_5FC8(&key, &D_004A5B20->params.w[24], 4) != 0 ? 9 : 0;
                D_004A5B20->params.w[24] = key;
                return 0;
            }
            *st = 1;
            return func_003FBE40();
        }
        if (cardResult == 0) {
            *st = 1;
            return func_003FBDF0();
        }
    } else if (*st == 2) {
        D_004A5B20->f74 = 0;
        D_004A5B20->f78 = 0;
    }
    return func_003FB810(cardResult);
}
#endif
