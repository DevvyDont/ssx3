#include "common.h"
#include "seg/dirtynet.h"
#include "seg/buddyapi.h"

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003EFF40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00496B58_FF40[] __asm__("D_00496B58");
extern char D_00496B70_FF40[] __asm__("D_00496B70");
extern char D_00496B80_FF40[] __asm__("D_00496B80");
extern char D_00496B90_FF40[] __asm__("D_00496B90");
extern char D_004A4AD0_FF40[] __asm__("D_004A4AD0");
extern void func_003EFED0(char *, const char *, ...);
struct I_FF40 { int f0; int f4; int f8; char *fC; };

void func_003EFF40_impl(char *self, char *x, struct I_FF40 *info) __asm__("func_003EFF40");
/**
 * Logs a buddy packet: its tags and a summary of its text.
 *
 * Prints @p x and two four-letter tags ("%s%c%c%c%c/%c%c%c%c", via func_003EFED0): @p info's [1] and [2]; a [1] of -1
 * prints "****" with [2] shown as "conn" (-1), "disc" (0xFEFEFEFE) or "----", and a [2] of 0 prints "----". Then the
 * text at @p info [3]: " [null data]" when empty, " [binary data]" when it has a control character other than tab, line
 * feed or carriage return, " [long data]" at 513 characters or more, else the text itself.
 *
 * @param x  the line prefix (such as "send: ")
 * @param info  the packet: [1] command tag, [2] kind, [3] text
 */
void func_003EFF40_impl(char *self, char *x, struct I_FF40 *info) {
    int cmdTag, kindTag;
    int n, flag;
    int cr;
    char *scanPos;
    char *bodyText = info->fC;

    kindTag = info->f8;
    if (kindTag == 0) kindTag = 0x2D2D2D2D;
    cmdTag = info->f4;
    if (cmdTag == -1) {
        cmdTag = 0x2A2A2A2A;
        if (kindTag == -1) {
            kindTag = 0x636F6E6E;
        } else if (kindTag == (int)0xFEFEFEFE) {
            kindTag = 0x64697363;
        } else {
            kindTag = 0x2D2D2D2D;
        }
    }
    func_003EFED0(self, D_00496B58_FF40, x, cmdTag >> 24, cmdTag >> 16, cmdTag >> 8, cmdTag, kindTag >> 24, kindTag >> 16, kindTag >> 8, kindTag);
    n = 0;
    flag = 0;
    if (*bodyText != 0 && *bodyText < 0x7F) {
        cr = 13;
        scanPos = bodyText;        do {
            unsigned char curByte = *scanPos;
            char ch = *scanPos;
            if (ch < 0x20) {
                if ((unsigned)(curByte - 9) < 2) continue;
                if (ch != cr) {
                    flag = 1;
                    break;
                }
            }
        } while (scanPos++, n++, *scanPos != 0 && *scanPos < 0x7F);
    }
    if (n == 0) {
        func_003EFED0(self, D_00496B70_FF40);
    } else if (flag == 1) {
        func_003EFED0(self, D_00496B80_FF40);
    } else if (n >= 513) {
        func_003EFED0(self, D_00496B90_FF40);
    } else {
        func_003EFED0(self, D_004A4AD0_FF40, bodyText);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F00D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_send[];
extern int func_003F5910(int, int, int, int, int);
extern void func_003EFF40(int*, char*, int*);
/**
 * Sends a buddy command packet and logs it.
 *
 * Calls func_003F5910 on the connection at @p buddyApi [1] with the command tag and message, then, when a log callback
 * is set (+0x508), logs it with func_003EFF40 and the prefix "send: ".
 *
 * @param buddyApi  the buddy-API object
 * @param cmdTag  the four-letter command tag
 * @param message  the message text
 */
void func_003F00D8(int* buddyApi, int cmdTag, int message) {
    int logInfo[4];
    func_003F5910(buddyApi[1], cmdTag, 0, message, -1);
    if (buddyApi[1288 / 4]) {
        logInfo[1] = cmdTag;
        logInfo[3] = message;
        logInfo[2] = 0;
        logInfo[0] = 0;
        func_003EFF40(buddyApi, str_send, logInfo);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0150);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char* strchr(const char* s, int c);
/**
 * Cuts a buddy name at the first '@' and then at the first '/', in place.
 *
 * func_003F17D8 calls it.
 *
 * @param name  the name to shorten
 */
void func_003F0150(char* name) {
    char* sep = strchr(name, '@');
    if (sep) *sep = 0;
    sep = strchr(name, '/');
    if (sep) *sep = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0198);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_USER_b[];
extern int D_004A4AE8[];
extern char str_DOMN[];
extern char str_RSRC[];
extern char* strchr(const char* s, int c);
extern int cDirtysock_tag_TagFieldDupl(char* dst, int len, const char* src);
extern char* cDirtysock_tag_TagFieldFind(const char* tag, const char* key);
extern int cDirtysock_tag_TagFieldGetString(const char* data, char* buf, int len, const char* defval);
extern int cDirtysock_tag_TagFieldDelete(char* tag, const char* key);
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
/**
 * Splits the sender of a received buddy message into user, domain and resource fields and hands it to the message
 * callback as a 'chat' message.
 *
 * Copies the message's tag text (@p ev[3]) into a 0x800-byte local buffer, reads its USER field, and rewrites it:
 * "user@domain/resource" becomes USER=user, DOMN=domain and RSRC=resource (each part only when its separator is
 * present). Then points ev[3] at the rewritten buffer, sets the message kind ev[0] to 'chat' and calls the receive
 * callback at +0x4B0 with the user data at +0x4AC (both set by func_003F17C8). func_003F17D8 calls it for a 'RECV'
 * packet when that callback is set.
 *
 * @param ev  the message being dispatched: kind at [0], tag text at [3] (replaced by a pointer to a stack buffer)
 */
void func_003F0198(char* self, int* ev) {
    char tag[0x800];
    char name[0x100];
    char* nm;
    char* domainSep;
    char* resourceSep;
    cDirtysock_tag_TagFieldDupl(tag, 0x800, (char*)ev[3]);
    cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(tag, str_USER_b), (char*)name, 0x100, (char*)D_004A4AE8);
    nm = (char*)name;
    cDirtysock_tag_TagFieldDelete(tag, str_USER_b);
    domainSep = strchr(nm, '@');
    if (domainSep) *domainSep = 0;
    cDirtysock_tag_TagFieldSetString(tag, 0x800, str_USER_b, nm);
    if (domainSep) {
        nm = domainSep + 1;
        resourceSep = strchr(nm, '/');
        if (resourceSep) *resourceSep = 0;
        cDirtysock_tag_TagFieldSetString(tag, 0x800, str_DOMN, nm);
        if (resourceSep) {
            cDirtysock_tag_TagFieldSetString(tag, 0x800, str_RSRC, resourceSep + 1);
        }
    }
    ev[3] = (int)tag;
    ev[0] = 0x63686174;
    (*(void (**)(char*, int*, int))(self + 0x4B0))(self, ev, *(int*)(self + 0x4AC));
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F02D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Fills the output record from the event tag fields.
extern int D_004512C0[];
extern int D_004A4AE8[];
extern char str_SHOW[];
extern char str_PROD_b[];
extern char str_STAT_b[];
extern char D_004A4B18[];
extern char* cDirtysock_tag_TagFieldFind(const char* tag, const char* key);
extern int cDirtysock_tag_TagFieldGetString(const char* data, char* buf, int len, const char* defval);
extern int cDirtysock_tag_TagFieldGetFourCC(const char* data, int def);
extern int strcmp_a(const char*, const char*) __asm__("strcmp");
extern int func_003EFE20(char*, int, short*, int, char*, int);
/**
 * Fills a roster entry's presence (show state, product, presence text, language) from a 'PGET' presence packet.
 *
 * Entry layout used here: +0x40 product name (20 bytes), +0x54 presence text (80), +0xA4 two-letter language code plus
 * NUL, +0xA7 show index, +0xA8 flags. The show index is the position of the packet's SHOW text in D_004512C0 ("DISC",
 * "CHAT", "AWAY", "XA", "DND", "PASS"; 0 when none matches, via strcmp strcmp). A PROD field sets flag 0x2000
 * and its text is the presence text; then the product name comes from func_003EFE20 (the product-key/language string
 * table, empty in retail) and the language is our own first language (+0x30C). Without PROD, it reads the STAT field,
 * takes its 'P' sub-field as the product key, and looks for a sub-field named by each of our languages (+0x308 codes at
 * +0x30C) in turn: the first found gives the presence text and the language. When none is found the language is
 * whatever func_003EFE20 matches for all our languages (0 in retail).
 *
 * @param ev  the received packet (tag text at +0xC)
 * @param out  the roster entry to fill
 */
void func_003F02D0(BuddyApiRefT* self, char* ev, char* out) {
    char key[3];
    char buf[0x100];
    short langCode;
    int w;
    int i = 0;
    int fourcc;
    char* foundField;
    char* langSlot;
    out[0xA7] = 0;
    out[0xA4] = 0;
    out[0x54] = 0;
    out[0x40] = 0;
    cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(*(char**)(ev + 0xC), str_SHOW), buf, 0x100, (char*)D_004A4AE8);
    while (i < 6) {
        if (strcmp_a((const char*)D_004512C0[i], buf) == 0) {
            out[0xA7] = i;
            break;
        }
        i++;
    }
    foundField = cDirtysock_tag_TagFieldFind(*(char**)(ev + 0xC), str_PROD_b);
    if (foundField) {
        *(int*)(out + 0xA8) |= 0x2000;
    }
    cDirtysock_tag_TagFieldGetString(foundField, out + 0x54, 80, (char*)D_004A4AE8);
    if (out[0x54] != 0) {
        func_003EFE20(((char*)self), self->mProductKey, (short*)(&self->mLangs[0]), 1, out + 0x40, 20);
        out[0xA4] = *(unsigned short*)&self->mLangs[0] >> 8;
        w = self->mLangs[0];
        out[0xA6] = 0;
        out[0xA5] = w;
    } else {
        i = 0;
        cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(*(char**)(ev + 0xC), str_STAT_b), buf, 0x100, (char*)D_004A4AE8);
        fourcc = cDirtysock_tag_TagFieldGetFourCC(cDirtysock_tag_TagFieldFind(buf, D_004A4B18), 0);
        if (self->mNumLangs > 0) {
            langSlot = (char*)&self->mLangs[0];
            for (; i < self->mNumLangs; i++, langSlot += 2) {
                short w = *(short*)langSlot;
                key[0] = w >> 8;
                key[1] = w;
                key[2] = 0;
                foundField = cDirtysock_tag_TagFieldFind(buf, key);
                if (foundField) {
                    unsigned char langHigh, langLow;
                    cDirtysock_tag_TagFieldGetString(foundField, out + 0x54, 80, (char*)D_004A4AE8);
                    langLow = key[1];
                    langHigh = key[0];
                    out[0xA4] = langHigh;
                    out[0xA5] = langLow;
                    out[0xA6] = 0;
                    langCode = (short)((((int)(langLow << 24) >> 8) | (langHigh << 24)) >> 16);
                    func_003EFE20(((char*)self), fourcc, &langCode, 1, out + 0x40, 20);
                    break;
                }
            }
        }
        if (out[0xA4] == 0) {
            int matchedCode = func_003EFE20(((char*)self), fourcc, (short*)(&self->mLangs[0]), self->mNumLangs, out + 0x40, 20);
            short matchedShort = matchedCode;
            out[0xA6] = 0;
            langCode = matchedCode;
            out[0xA4] = matchedShort >> 8;
            out[0xA5] = matchedShort;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0568);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004A4B18[];
extern char str_STAT_b[];
extern char str_PROD_b[];
extern int cDirtysock_tag_TagFieldSetFourCC(char* dst, int len, const char* key, int val);
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
/**
 * Sends the buddy client's presence to the server as a 'PSET' packet.
 *
 * When a product key is set (+0x1C) it is stored as the 'P' field of the status text at +0x140. The packet is the base
 * presence at +0x240 (RSRC and SHOW, built by func_003F15C8) plus STAT (+0x140) and PROD (+0x40) when those are
 * non-empty, sent with func_003F00D8. Clears the pending-send time at +0x2C0.
 *
 * @param buddyRef  the BuddyApi state
 */
void func_003F0568(BuddyApiRefT* buddyRef) {
    char buf[0x200];
    if (buddyRef->mProductKey) {
        cDirtysock_tag_TagFieldSetFourCC(buddyRef->mStatText, 0x100, D_004A4B18, buddyRef->mProductKey);
    }
    cDirtysock_tag_TagFieldDupl(buf, 0x200, buddyRef->mPresence);
    if (buddyRef->mStatText[0]) {
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_STAT_b, buddyRef->mStatText);
    }
    if (buddyRef->mProdText[0]) {
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PROD_b, buddyRef->mProdText);
    }
    func_003F00D8((int*)buddyRef, 0x50534554, (int)buf);
    buddyRef->mPresenceSendTime = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0610);
#ifdef SKIP_ASM
/**
 * Sets the buddy client's connection state and reports it to the connect callback.
 *
 * Stores @p state at +0x8 and, when the connect callback at +0x4C4 is set, calls it with a {'conn', state, 0, 0}
 * message and the user data at +0x4C0. States used: 0 idle, 1 connecting, 2 host lookup failed, 3 connect timed out, 5
 * online, 6 connection lost.
 *
 * @param buddyApi  the BuddyApi state
 * @param state  the new state
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003F0610(int* buddyApi, int state) {
    int event[4];
    buddyApi[2] = state;
    if (buddyApi[1220 / 4]) {
        event[0] = 0x636F6E6E;
        event[1] = state;
        event[2] = 0;
        event[3] = 0;
        ((void (*)(int*, int*, int))buddyApi[1220 / 4])(buddyApi, event, buddyApi[1216 / 4]);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0658);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct S0658E { int f0, f4, f8, fC, f10; };
struct S0658 { int pad[5]; int f14; int pad2[(0x4C8-0x18)/4]; struct S0658E e[1]; };
extern int D_004512B0[];
extern int func_003F5D88(int);
/**
 * Arms one of the buddy client's three pending-request slots (0 add, 1 delete, 2 send).
 *
 * The slot is the 0x14-byte record at +0x4C8 + @p idx * 0x14: its kind becomes D_004512B0[idx] ('addu', 'delu',
 * 'send'), its sequence number the next value of the request counter at +0x14 (which starts at 100), and it keeps the
 * callback, its user data and a deadline of now (func_003F5D88) + @p ms * 1000 (0 when @p ms is not positive).
 * func_003F17D8 drops the connection when a deadline passes while the callback is still set. Callers pass 600, so
 * @p ms is in seconds.
 *
 * @param buddyRef  the BuddyApi state
 * @param idx  slot index (0 add, 1 delete, 2 send)
 * @param callback  callback for the request's reply
 * @param userData  user data passed to the callback
 * @param ms  timeout in seconds
 */
void func_003F0658(struct S0658* buddyRef, int idx, int callback, int userData, int ms) {
    int deadline = 0;
    if (ms > 0) {
        deadline = func_003F5D88(((int*)buddyRef)[1]) + ms * 1000;
    }
    buddyRef->e[idx].f0 = D_004512B0[idx];
    buddyRef->f14++;
    buddyRef->e[idx].f4 = buddyRef->f14;
    buddyRef->e[idx].fC = userData;
    buddyRef->e[idx].f10 = callback;
    buddyRef->e[idx].f8 = deadline;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0710);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void* func_002591E8(int);
extern void memset_a(void*, int, int) __asm__("memset");
extern int cDirtysock_tag_TagFieldDupl(char* dst, int len, const char* src);
extern int func_003F5478(int);
/**
 * Creates a buddy client (BuddyApi) state block.
 *
 * Allocates 0x50C bytes with func_002591E8 and zero-fills them, copies @p product to +0x314 and @p version to +0x354
 * (both sent in the 'AUTH' login), creates the packet session with func_003F5478(0x8000) at +0x4 and starts the request
 * counter (+0x14) at 100. BuddyMan's BuddyMan_createBuddyApi calls it with version "2003-10-20".
 *
 * @param product  product name sent as PROD
 * @param version  version text sent as VERS
 * @return the new state, or 0 when the allocation fails
 */
int* func_003F0710(const char* product, const char* version) {
    BuddyApiRefT* buddyApi = (BuddyApiRefT*)func_002591E8(1292);
    if (buddyApi) {
        memset_a(buddyApi, 0, 1292);
        cDirtysock_tag_TagFieldDupl(buddyApi->mProduct, 0x40, product);
        cDirtysock_tag_TagFieldDupl(buddyApi->mVersion, 0x40, version);
        buddyApi->mpSession = (void*)func_003F5478(0x8000);
        buddyApi->mRequestSeq = 100;
    }
    return (int*)buddyApi;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F07A0);
#ifdef SKIP_ASM
extern int func_003F0B50(struct BuddyApiRefT*);
extern void func_003F0BE0(struct BuddyApiRefT*);
extern void func_003F55B8(int);
extern void func_00259210(void*);
/**
 * Destroys a buddy client.
 *
 * Destroys a pending host lookup (the object at +0x0, through its slot +0xC), disconnects (func_003F0B50), frees the
 * roster (func_003F0BE0), destroys the packet session at +0x4 (func_003F55B8) and frees the state with func_00259210.
 *
 * @param buddyRef  the BuddyApi state
 */
void func_003F07A0(struct BuddyApiRefT* buddyRef) {
    if (((struct HostentT*)buddyRef->mpHostLookup)) {
        ((void (*)(struct S07A0Obj*))((struct HostentT*)buddyRef->mpHostLookup)->Free)(((struct HostentT*)buddyRef->mpHostLookup));
        (*(struct HostentT**)&buddyRef->mpHostLookup) = 0;
    }
    func_003F0B50(buddyRef);
    func_003F0BE0(buddyRef);
    func_003F55B8(((int)buddyRef->mpSession));
    func_00259210(buddyRef);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0800);
#ifdef SKIP_ASM
/**
 * Returns the buddy client's connection state (+0x8; see func_003F0610).
 *
 * @param buddyApi  the BuddyApi state
 * @return the state
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F0800(int* buddyApi) {
    return buddyApi[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0808);
#ifdef SKIP_ASM
/**
 * Installs the buddy client's debug-print hook.
 *
 * Stores the context at +0x504 and the print function at +0x508; func_003EFED0 and the "send: "/"recv: " traces call it
 * with the context and the formatted text when it is set. Nothing in retail calls this.
 *
 * @param buddyApi  the BuddyApi state
 * @param debugContext  first argument passed to the print function
 * @param debugFunc  the print function (0 disables tracing)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003F0808(int* buddyApi, int debugContext, int debugFunc) {
    buddyApi[1284 / 4] = debugContext;
    buddyApi[1288 / 4] = debugFunc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0818);
#ifdef SKIP_ASM
/**
 * Sets the buddy client's presence text, product key, languages and name policy before connecting.
 *
 * Copies @p src to +0x2C8 (0x40 bytes, sent as PRES at login) and stores @p key at +0x1C (the product key; BuddyMan
 * passes 'ssx3'). Parses up to four two-letter language codes from @p langCodes into the 16-bit codes at +0x30C,
 * stopping at the first character below 'a', and stores their count at +0x308. Stores @p x at +0x18: when it is 0,
 * buddy names containing '.' are rejected (func_003F0E38, func_003F11F8) or skipped (roster updates).
 *
 * @param buddyApi  the BuddyApi state
 * @param src  presence text
 * @param key  product key
 * @param langCodes  concatenated lower-case language codes ("en", ...)
 * @param x  nonzero to allow names that contain '.'
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F0818(BuddyApiRefT* buddyApi, const char* src, int key, char* langCodes, int x) {
    short* langSlot;
    int j;
    cDirtysock_tag_TagFieldDupl(buddyApi->mPresText, 0x40, src);
    buddyApi->mProductKey = key;
    j = 0;
    langSlot = (short*)(&buddyApi->mLangs[0]);
    do {
        if (langCodes[0] < 97 || langCodes[1] < 97) {
            break;
        }
        *langSlot = 0;
        j++;
        *langSlot = langCodes[0] << 8;
        *langSlot |= langCodes[1];
        langCodes += 2;
        langSlot++;
    } while (j < 4);
    buddyApi->unk_018 = x;
    buddyApi->mNumLangs = j;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F08E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_PASS_c[];
extern char str_USER_b[];
extern char str_PROD_b[];
extern char str_VERS_b[];
extern char str_PRES[];
extern char str_LKEY[];
extern char* strchr(const char* s, int c);
extern char* strncpy(char* d, const char* s, int n);
extern char* strcpy(char* d, const char* s);
extern int strlen(const char* s);
extern char* strcat(char*, const char*);
extern int cDirtysock_tag_TagFieldDupl(char* dst, int len, const char* src);
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
extern int SocketLookup(int, int);
extern int func_003F5D88(int);
/**
 * Starts connecting the buddy client to a buddy server and prepares the login packet.
 *
 * Disconnects first (func_003F0B50), stores the connect callback (+0x4C4), its user data (+0x4C0) and the port (+0x10),
 * and builds the 'AUTH' tag text at +0x394: PROD (+0x314), VERS (+0x354), PRES (+0x2C8), then the login. A login
 * without ':' is a login key: USER = @p nick, LKEY = login. Otherwise "user:secret" gives USER = user followed by
 * @p nick (when it fits in 0x100), and LKEY = secret without its leading '$' or PASS = secret. Copies @p nick into
 * +0x494 (0x10 bytes, the resource sent as LRSC), dropping a leading '/' (an overlapping strcpy) and cutting at the
 * next '/'. Then starts the host lookup (+0x0 = SocketLookup(@p serverName, @p ms * 1000)), reports state 1
 * (connecting), sets the connect deadline +0xC to now + @p ms * 1000 and the last show state +0x2C4 to -1.
 * func_003F17D8 finishes the connection.
 *
 * @param serverName  buddy server host name
 * @param port  buddy server port
 * @param ms  timeout in seconds (BuddyMan passes 30)
 * @param host  the login: "user:password", "user:$key" or a bare key
 * @param nick  the resource ("/cso/" + resource from BuddyMan)
 * @param cb  connect callback ('conn' state changes and the 'auth' reply)
 * @param ctx  user data for the callback
 * @return 0
 */
int func_003F08E8(BuddyApiRefT* self, int serverName, int port, int ms, char* host, char* nick, int cb, int ctx) {
    char tag[0x100];
    char loginCopy[0x100];
    char* sep;
    char* buf = self->mAuthText;
    char* nickBuf;
    func_003F0B50((struct BuddyApiRefT*)self);
    self->mpConnectData = (void*)ctx;
    self->mpConnectProc = (void (*)())cb;
    self->mPort = port;
    *buf = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PROD_b, self->mProduct);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_VERS_b, self->mVersion);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PRES, self->mPresText);
    if (strchr(host, ':') == 0) {
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, nick);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LKEY, host);
    } else {
        strncpy(loginCopy, host, 0xFF);
        loginCopy[0xFF] = 0;
        sep = strchr(loginCopy, ':');
        if (sep) {
            *sep = 0;
            cDirtysock_tag_TagFieldDupl(tag, 0x100, loginCopy);
            sep++;
            if ((unsigned int)(strlen(tag) + strlen(nick)) < 0x100) {
                strcat(tag, nick);
            }
            cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, tag);
            if (*sep == '$') {
                cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LKEY, sep + 1);
            } else {
                cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PASS_c, sep);
            }
        }
    }
    nickBuf = self->mResource;
    cDirtysock_tag_TagFieldDupl(nickBuf, 0x10, nick);
    if (*nickBuf == '/') {
        strcpy(nickBuf, (char*)&self->mResource[1]);
    }
    sep = strchr(nickBuf, '/');
    if (sep) *sep = 0;
    *(int*)self = SocketLookup(serverName, ms * 1000);
    func_003F0610((int*)self, 1);
    self->mConnectDeadline = func_003F5D88((int)self->mpSession) + ms * 1000;
    self->mLastShow = -1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0B50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// (prototype from their definition at line 249)
void func_003F0610_0B50(int* p, int a) __asm__("func_003F0610");

// (prototype from their definition at line 70)
void func_003F00D8_0B50(int* p, int a, int b) __asm__("func_003F00D8");

extern int D_004A4AE8[];
extern int func_003F5738(int);
/**
 * Disconnects the buddy client.
 *
 * Destroys a pending host lookup object (+0x0), sends 'DISC' with empty data (func_003F00D8), closes the packet session
 * (func_003F5738 on +0x4), reports state 6 when the client was online (5) and then state 0, clears the connect callback
 * (+0x4C4) and frees the roster (func_003F0BE0).
 *
 * @param buddyRef  the BuddyApi state
 * @return 0
 */
int func_003F0B50(struct BuddyApiRefT* buddyRef) {
    if (((struct HostentT*)buddyRef->mpHostLookup)) {
        ((void (*)(struct S07A0Obj*))((struct HostentT*)buddyRef->mpHostLookup)->Free)(((struct HostentT*)buddyRef->mpHostLookup));
        (*(struct HostentT**)&buddyRef->mpHostLookup) = 0;
    }
    func_003F00D8_0B50((int*)buddyRef, 0x44495343, (int)D_004A4AE8);
    func_003F5738(((int)buddyRef->mpSession));
    if (buddyRef->mState == 5) {
        func_003F0610_0B50((int*)buddyRef, 6);
    }
    func_003F0610_0B50((int*)buddyRef, 0);
    buddyRef->mpConnectProc = 0;
    func_003F0BE0(buddyRef);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0BE0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EE9A0(int);
extern void* func_003EE480(int, int);
extern void func_003EE820(int);
extern void func_003EE260(int);
/**
 * Frees the buddy client's roster.
 *
 * When a roster list exists (+0x4A8), frees every entry (taking ids out of the name table +0x4A4 with func_003EE9A0 and
 * removing the entries from the list with func_003EE480), calls the roster callback (+0x4B8, user data +0x4B4) with a
 * {'rost', 0, 0, roster} message, destroys the name table (func_003EE820) and the list (func_003EE260), and clears both
 * pointers and the roster callback and its user data.
 *
 * @param sp  the BuddyApi state
 */
void func_003F0BE0(struct BuddyApiRefT* sp) {
    int* buddyApi = (int*)sp;
    int event[4];
    int x;
    if (buddyApi[1192 / 4]) {
        while ((x = func_003EE9A0(buddyApi[1188 / 4]))) {
            func_00259210(func_003EE480(buddyApi[1192 / 4], x));
        }
        if (buddyApi[1208 / 4]) {
            event[0] = 0x726F7374;
            event[1] = 0;
            event[2] = 0;
            event[3] = buddyApi[1192 / 4];
            ((void (*)(int*, int*, int))buddyApi[1208 / 4])(buddyApi, event, buddyApi[1204 / 4]);
        }
        func_003EE820(buddyApi[1188 / 4]);
        buddyApi[1188 / 4] = 0;
        func_003EE260(buddyApi[1192 / 4]);
        buddyApi[1192 / 4] = 0;
        buddyApi[1192 / 4] = 0;
        buddyApi[1204 / 4] = 0;
        buddyApi[1208 / 4] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0C90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_PRES[];
extern char str_LRSC[];
extern char str_LIST[];
extern char D_004A4B48[];
extern char D_004A4B50[];
extern char str_ID[];
extern char D_004A4B60[];
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
extern int cDirtysock_tag_TagFieldSetNumber(char* dst, int len, const char* key, int val);
extern int func_003EE778(int, int);
extern int func_003EE1E8(int, int, int);
extern int func_003EE600(int, int);
/**
 * Creates the buddy roster and requests the buddy and ignore lists, once.
 *
 * When no roster exists (+0x4A8): stores the roster callback at +0x4B8 and its user data at +0x4B4, creates the name
 * table +0x4A4 (func_003EE778(1000, 2000)) and the roster list (func_003EE1E8(100, 100, 0)), and sends two 'RGET'
 * packets with LRSC = the resource (+0x494) and PRES = "Y": LIST "B" (buddies) as ID 1 and LIST "I" (ignored users) as
 * ID 2. Marks the roster changed (func_003EE600). BuddyMan's onconnect calls it after a successful 'auth'.
 *
 * @param rosterCallback  roster callback ('rost' changes, 'endr' when the lists are complete)
 * @param rosterUserData  user data for the callback
 * @return the roster list
 */
void* func_003F0C90(BuddyApiRefT* self, int rosterCallback, int rosterUserData) {
    char buf[0x100];
    if ((int)self->mpRoster == 0) {
        char* resourceName = self->mResource;
        self->mpRosterProc = (void (*)())rosterCallback;
        self->mpRosterData = (void*)rosterUserData;
        self->mpNameTable = (void*)func_003EE778(1000, 2000);
        self->mpRoster = (void*)func_003EE1E8(100, 100, 0);
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LRSC, resourceName);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LIST, D_004A4B48);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PRES, D_004A4B50);
        cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_ID, 1);
        func_003F00D8((int*)self, 0x52474554, (int)buf);
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LRSC, resourceName);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LIST, D_004A4B60);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PRES, D_004A4B50);
        cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_ID, 2);
        func_003F00D8((int*)self, 0x52474554, (int)buf);
        func_003EE600((int)self->mpRoster, 1);
    }
    return self->mpRoster;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0E30);
#ifdef SKIP_ASM
/**
 * Returns the buddy client's roster list (+0x4A8; 0 before func_003F0C90).
 *
 * @param buddyApi  the BuddyApi state
 * @return the roster list
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F0E30(int* buddyApi) {
    return buddyApi[1192 / 4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F0E38);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char* strchr(const char* s, int c);
extern void* func_002591E8(int);
extern int func_003EEC88(int, int);
extern int func_003EE468(int, int);
extern int func_003EE320(int, int);
extern int func_003EEB30(int, int, int);
extern char str_USER_b[];
extern char str_PRES[];
extern char str_LRSC[];
extern char str_LIST[];
extern char D_004A4B48[];
extern char D_004A4B50[];
extern char str_ID[];
extern char D_004A4B60[];
extern char str_GROUP[];
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
extern int cDirtysock_tag_TagFieldSetNumber(char* dst, int len, const char* key, int val);
typedef struct { char b[0xB8]; } Ent_0E38;
/**
 * Adds a user to the buddy list, the ignore list and/or the presence subscriptions.
 *
 * Returns -3 for a missing record, no flags (+0xA8), an empty name or a name containing '/' or '@' (or '.', unless
 * +0x18 allows it). Arms request slot 0 ('addu') with func_003F0658. Without a roster it stops there (returns 0).
 * Otherwise finds the entry by name (func_003EEC88) or adds a 0xB8-byte copy of @p buddyRecord to the roster and name table, ORs
 * the flags in and clears +0xAC. Flag 4 sends 'RADD' with LIST "B", PRES "Y", USER and GROUP (+0x20); flag 0x200 sends
 * 'RADD' with LIST "I"; both carry LRSC and ID = the slot's sequence number (+0x4CC), mark the entry pending (flag
 * 0x10000) and store that number in +0xAC for the reply. Flag 0x800000 sends 'PADD' with USER.
 *
 * @param buddyRecord  the entry to add: name at +0x0, group at +0x20, flags at +0xA8
 * @param callback  callback for the 'addu' reply
 * @param userData  user data for the callback
 * @param timeoutSecs  timeout in seconds
 * @return 0, or -3 when the record is refused
 */
int func_003F0E38(BuddyApiRefT* self, char* buddyRecord, int callback, int userData, int timeoutSecs) {
    char buf[0x100];
    char* ent;
    int x;
    if (buddyRecord == 0 || *(int*)(buddyRecord + 0xA8) == 0 || buddyRecord[0] == 0 || strchr(buddyRecord, '/') != 0)
        return -3;
    if (strchr(buddyRecord, '@') != 0)
        return -3;
    if (self->unk_018 == 0) {
        if (strchr(buddyRecord, '.') != 0)
            return -3;
    }
    func_003F0658((struct S0658*)self, 0, callback, userData, timeoutSecs);
    if ((int)self->mpRoster == 0)
        return 0;
    x = func_003EEC88((int)self->mpNameTable, (int)buddyRecord);
    if (x == 0) {
        ent = (char*)func_002591E8(184);
        *(Ent_0E38*)ent = *(Ent_0E38*)buddyRecord;
        func_003EEB30((int)self->mpNameTable, (int)ent, func_003EE320((int)self->mpRoster, (int)ent));
    } else {
        ent = (char*)func_003EE468((int)self->mpRoster, x);
        *(int*)(ent + 0xA8) |= *(int*)(buddyRecord + 0xA8);
    }
    *(int*)(ent + 0xAC) = 0;
    if (*(int*)(buddyRecord + 0xA8) & 4) {
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LRSC, self->mResource);
        cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_ID, self->mRequests[0][1]);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LIST, D_004A4B48);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PRES, D_004A4B50);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, buddyRecord);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_GROUP, buddyRecord + 0x20);
        func_003F00D8((int*)self, 0x52414444, (int)buf);
        *(int*)(ent + 0xA8) |= 0x10000;
        *(int*)(ent + 0xAC) = self->mRequestSeq;
    }
    if (*(int*)(buddyRecord + 0xA8) & 0x200) {
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LRSC, self->mResource);
        cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_ID, self->mRequests[0][1]);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LIST, D_004A4B60);
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, buddyRecord);
        func_003F00D8((int*)self, 0x52414444, (int)buf);
        *(int*)(ent + 0xA8) |= 0x10000;
        *(int*)(ent + 0xAC) = self->mRequestSeq;
    }
    if (*(int*)(buddyRecord + 0xA8) & 0x800000) {
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, buddyRecord);
        func_003F00D8((int*)self, 0x50414444, (int)buf);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F11F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_USER_b[];
extern char str_PRES[];
extern char str_LRSC[];
extern char str_LIST[];
extern char D_004A4B48[];
extern char D_004A4B50[];
extern char str_ID[];
extern char D_004A4B60[];
extern char str_GROUP[];
extern char* strchr(const char* s, int c);
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
extern int cDirtysock_tag_TagFieldSetNumber(char* dst, int len, const char* key, int val);
extern int func_003EEC88(int, int);
extern int func_003EE468(int, int);
extern void func_003EED08(int, char*);
extern void* func_003EE480(int, int);
/**
 * Removes a user from the buddy list, the ignore list and/or the presence subscriptions.
 *
 * Returns -2 without a record or a roster, -3 for a name containing '/' or '@' (or '.', unless +0x18 allows it). Arms
 * request slot 1 ('delu') with func_003F0658. When the name is in the roster, clears from its entry the flags that
 * @p name's +0xA8 also has: flag 4 sends 'RDEL' with ID = the slot's sequence number (+0x4E0), LRSC, LIST "B", PRES
 * "Y", USER and GROUP; flag 0x200 sends 'RDEL' with LIST "I"; flag 0x800000 sends 'PDEL'. An entry left without any of
 * the flags 0x100204 is taken out of the name table and freed.
 *
 * @param name  the record: name at +0x0, group at +0x20, flags to remove at +0xA8
 * @param callback  callback for the 'delu' reply
 * @param userData  user data for the callback
 * @param ms  timeout in seconds
 * @return 0 when the entry was found, else -2 or -3
 */
int func_003F11F8(BuddyApiRefT* self, char* name, int callback, int userData, int ms) {
    char buf[0x100];
    int ret = -2;
    int id;
    char* ent;
    int removedFlags;
    if (name) {
        if ((int)self->mpRoster != 0) {
            if (strchr(name, '/')) return -3;
            if (strchr(name, '@')) return -3;
            if (self->unk_018 == 0) {
                if (strchr(name, '.')) return -3;
            }
            func_003F0658((struct S0658*)self, 1, callback, userData, ms);
            id = func_003EEC88((int)self->mpNameTable, (int)name);
            if (id != 0) {
                ent = (char*)func_003EE468((int)self->mpRoster, id);
                removedFlags = *(int*)(ent + 0xA8) & *(int*)(name + 0xA8);
                *(int*)(ent + 0xA8) ^= removedFlags;
                if (removedFlags & 4) {
                    buf[0] = 0;
                    cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_ID, self->mRequests[1][1]);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LRSC, self->mResource);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LIST, D_004A4B48);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PRES, D_004A4B50);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, name);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_GROUP, name + 0x20);
                    func_003F00D8((int*)self, 0x5244454C, (int)buf);
                }
                if (removedFlags & 0x200) {
                    buf[0] = 0;
                    cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_ID, self->mRequests[1][1]);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LRSC, self->mResource);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LIST, D_004A4B60);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, name);
                    func_003F00D8((int*)self, 0x5244454C, (int)buf);
                }
                if (removedFlags & 0x800000) {
                    buf[0] = 0;
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_LRSC, self->mResource);
                    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_USER_b, name);
                    func_003F00D8((int*)self, 0x5044454C, (int)buf);
                }
                if ((*(int*)(ent + 0xA8) & 0x100204) == 0) {
                    func_003EED08((int)self->mpNameTable, name);
                    func_00259210(func_003EE480((int)self->mpRoster, id));
                }
                ret = 0;
            }
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F14D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EEC88(int, int);
extern int func_003EE468(int, int);
/**
 * Finds a roster entry by user name.
 *
 * @param buddyApi  the BuddyApi state
 * @param buddyName  the user name
 * @return the entry (0xB8 bytes), or 0 when there is no roster or no such name
 */
int func_003F14D8(int* buddyApi, int buddyName) {
    int rosterEntry = 0;
    if (buddyApi[1188 / 4] && buddyName) {
        int x = func_003EEC88(buddyApi[1188 / 4], buddyName);
        if (x) {
            rosterEntry = func_003EE468(buddyApi[1192 / 4], x);
        }
    }
    return rosterEntry;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F1538);
#ifdef SKIP_ASM
/**
 * Clears the buddy client's presence product text (+0x40) and status text (+0x140).
 *
 * @param buddyApi  the BuddyApi state
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003F1538(char* buddyApi) {
    buddyApi[0x40] = 0;
    buddyApi[0x140] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F1548);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int cDirtysock_tag_TagFieldDupl(char* dst, int len, const char* src);
/**
 * Sets the product text sent as PROD in the presence (+0x40, 0x100 bytes), when a presence text was set (+0x2C8).
 *
 * @param buddyApi  the BuddyApi state
 * @param src  the product text
 */
void func_003F1548(char* buddyApi, const char* src) {
    if (buddyApi[0x2C8]) {
        cDirtysock_tag_TagFieldDupl(buddyApi + 0x40, 0x100, src);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F1578);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
/**
 * Sets one language's status text in the presence status (STAT, +0x140).
 *
 * Only when @p key is exactly two characters and @p val is not empty. BuddyMan passes ("is playing SSX 3", "en").
 *
 * @param buddyApi  the BuddyApi state
 * @param val  the status text
 * @param key  two-letter language code
 */
void func_003F1578(char* buddyApi, const char* val, const char* key) {
    if (key[0] && key[1] && !key[2] && val[0]) {
        cDirtysock_tag_TagFieldSetString(buddyApi + 0x140, 0x100, key, val);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F15C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004512C0[];
extern char str_RSRC[];
extern char str_SHOW[];
extern int cDirtysock_tag_TagFieldSetString(char* dst, int len, const char* key, const char* val);
extern int func_003F5D88(int);
extern void func_003F0568(BuddyApiRefT*);
/**
 * Sets the buddy client's resource and show state and sends the presence now or after a delay.
 *
 * Rebuilds the base presence at +0x240 (0x80 bytes): RSRC = @p resourceText and SHOW = D_004512C0[@p st] ("CHAT", "AWAY", "XA",
 * "DND", "PASS" for 1..5, else "DISC"). With no send pending (+0x2C0 == 0) schedules one at now + @p sendDelay; a show state
 * different from the last one (+0x2C4) is recorded and sent at once. Sends with func_003F0568 when the time has come;
 * otherwise func_003F17D8 sends it later.
 *
 * @param buddyRef  the BuddyApi state
 * @param resourceText  resource text
 * @param st  show state index
 * @param sendDelay  delay in ticks of func_003F5D88 (BuddyMan passes 2000)
 * @return 0
 */
int func_003F15C8(BuddyApiRefT* buddyRef, char* resourceText, int st, int sendDelay) {
    unsigned int now = func_003F5D88((int)buddyRef->mpSession);
    int showName;
    char* buf;
    if ((unsigned int)(st - 1) < 5) {
        showName = D_004512C0[st];
    } else {
        showName = D_004512C0[0];
    }
    buf = buddyRef->mPresence;
    *buf = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x80, str_RSRC, resourceText);
    cDirtysock_tag_TagFieldSetString(buf, 0x80, str_SHOW, (char*)showName);
    if ((unsigned int)buddyRef->mPresenceSendTime == 0) {
        buddyRef->mPresenceSendTime = now + sendDelay;
    }
    if (buddyRef->mLastShow != st) {
        buddyRef->mLastShow = st;
        buddyRef->mPresenceSendTime = now;
    }
    if (now >= (unsigned int)buddyRef->mPresenceSendTime) {
        func_003F0568(buddyRef);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F16D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_BODY_b[];
extern char str_SUBJ_b[];
extern int D_004A4AE8[];
extern char* cDirtysock_tag_TagFieldFind(const char* tag, const char* key);
extern int cDirtysock_tag_TagFieldGetString(const char* data, char* buf, int len, const char* defval);
/**
 * Sends a private message to the buddy server ('SEND').
 *
 * Refused (-3) when the BODY field is 0x40 characters or longer or a SUBJ field has text. Otherwise arms request slot 2
 * ('send') with func_003F0658 and sends @p tag as a 'SEND' packet.
 *
 * @param buddyRef  the BuddyApi state
 * @param tag  the message tag text (BODY, USER, ...)
 * @param callback  callback for the 'send' reply
 * @param userData  user data for the callback
 * @param timeoutSecs  timeout in seconds
 * @return 0, or -3 when refused
 */
int func_003F16D8(int* buddyRef, char* tag, int callback, int userData, int timeoutSecs) {
    if (cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(tag, str_BODY_b), 0, 0, (char*)D_004A4AE8) < 0x40) {
        if (cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(tag, str_SUBJ_b), 0, 0, (char*)D_004A4AE8) <= 0) {
            func_003F0658(buddyRef, 2, callback, userData, timeoutSecs);
            func_003F00D8(buddyRef, 0x53454E44, (int)tag);
            return 0;
        }
    }
    return -3;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F17C8);
#ifdef SKIP_ASM
/**
 * Installs the buddy client's receive callback (+0x4B0) and its user data (+0x4AC).
 *
 * The callback gets 'chat' messages (func_003F0198) and 'admn' messages from func_003F17D8.
 *
 * @param buddyApi  the BuddyApi state
 * @param callback  the callback
 * @param userData  its user data
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003F17C8(int* buddyApi, int callback, int userData) {
    buddyApi[1200 / 4] = callback;
    buddyApi[1196 / 4] = userData;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F17D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00496BA0[];
extern char D_00496BB8[];
extern char str_recv[];
extern char str_ID[];
extern char str_SIZE[];
extern char str_USER_b[];
extern int D_004A4AE8[];
extern char str_GROUP[];
extern char str_FUSR[];
extern int func_003F5D88(int);
extern void f17_5908(int) __asm__("func_003F5908");
extern int func_003F5638(int, int, int);
extern int func_003F5A70(int, int*, int*, char**);
extern int func_003F5C38(int, int, int, int, int);
extern int func_003F5738(int);
extern int func_003F5910(int, int, int, int, int);
extern int func_003EE560(int, int);
extern int func_003EE620(int);
extern int f17_EED08(int, char*) __asm__("func_003EED08");
extern int memcmp_a(void*, void*, int) __asm__("memcmp");
extern int cDirtysock_tag_TagFieldGetNumber(const char* data, int defval);
extern char* strcpy(char* d, const char* s);
extern int strlen(const char* s);
extern void* memset(void*, int, unsigned int);
struct Ev_17D8 {
    int kind;
    int type;
    int arg;
    char* data;
};
struct Obj_17D8 {
    int f0;
    int addr;
    int (*poll)(struct Obj_17D8*);
    void (*destroy)(struct Obj_17D8*);
};
struct Conn_17D8;
typedef void (*Cb_17D8)(struct Conn_17D8*, struct Ev_17D8*, int);
struct Req_17D8 {
    int kind;
    int seq;
    unsigned int timeout;
    int ctx;
    Cb_17D8 cb;
};
struct Conn_17D8 {
    struct Obj_17D8* obj;
    int sess;
    int state;
    unsigned int timeout;
    int port;
    int f14;
    int f18;
    char pad1C[0x2C0 - 0x1C];
    unsigned int f2C0;
    char pad2C4[0x394 - 0x2C4];
    char auth[0x4A4 - 0x394];
    int names;
    int roster;
    int admnCtx;
    Cb_17D8 admnCb;
    int rostCtx;
    Cb_17D8 rostCb;
    int rostCount;
    int stateCtx;
    Cb_17D8 stateCb;
    struct Req_17D8 req[3];
    int f504;
    int f508;
};
typedef struct { char b[0xB8]; } Ent_17D8;
/**
 * Runs the buddy client: finishes connecting, enforces timeouts, and dispatches every received packet.
 *
 * While connecting, polls the host lookup object (+0x0): on success connects the session (func_003F5638 with its
 * address and the port), traces "connecting to %08x:%d" and sends the 'AUTH' login (+0x394); on failure reports state
 * 2; either way destroys the object. A passed connect deadline (+0xC) reports state 3. Online (state 5) it sends a
 * delayed presence when due (+0x2C0, func_003F0568) and drops the connection (func_003F5738, state 6) when a pending
 * request's deadline passes. In states 3 and 6 every pending request's callback gets its kind with result -4.
 *
 * Then reads packets with func_003F5A70 until none is left (each released with func_003F5C38): type -1 reports state 5
 * (result -1) or 6 (0xFEFEFEFE); 'PING' is echoed; 'AUTH' goes to the connect callback as 'auth' (clearing the
 * deadline; an error disconnects with func_003F0B50); 'RECV' goes to func_003F0198 and 'ADMN' to the receive callback
 * as 'admn'; 'SEND' completes request slot 2. 'RGET' with ID 2 gives the expected ignore-list size; 'ROST' adds or
 * updates a roster entry (ID 1 flag 4, ID 2 flag 0x200, clears 0x100000, copies GROUP), counting down the ignore list
 * to send 'endr' to the roster callback. 'PGET' refreshes an entry's presence (func_003F02D0) and traces "PRES: ..."
 * when it changed. 'RADD' completes the pending add whose ID matches the entry's +0xAC: an error frees the entry, else
 * the name is replaced by FUSR when given; the slot 0 callback gets 'addu' either way. 'RDEL' with slot 1's ID
 * completes it as 'delu'. Finally, when the roster changed (func_003EE620), the roster callback gets 'rost'.
 */
void func_003F17D8(struct Conn_17D8* self) {
    char name[0x20];
    struct Ev_17D8 ev;
    Ent_17D8 old;
    char* data;
    int done;
    unsigned int now;
    int i;
    int flags;
    int id;
    char* ent;

    now = func_003F5D88(self->sess);
    f17_5908(self->sess);
    memset(&ev, 0, sizeof(ev));
    if (self->stateCb != 0 && self->obj != 0) {
        i = self->obj->poll(self->obj);
        if (i != 0) {
            if (i > 0) {
                func_003F5638(self->sess, self->obj->addr, self->port);
                func_003EFED0((char*)self, D_00496BA0, self->obj->addr, self->port);
                func_003F00D8((int*)self, 0x41555448, (int)self->auth);
            }
            if (i < 0) {
                func_003F0610((int*)self, 2);
                self->stateCb = 0;
            }
            self->obj->destroy(self->obj);
            self->obj = 0;
        }
    }
    if (self->timeout != 0 && self->timeout < now) {
        func_003F0610((int*)self, 3);
        self->stateCb = 0;
    }
    if (self->state == 5) {
        if (self->f2C0 != 0 && now >= self->f2C0) {
            func_003F0568((BuddyApiRefT*)((char*)self));
        }
    }
    if (self->state == 5) {
        for (i = 0; i < 3; i++) {
            if (self->req[i].cb != 0 && self->req[i].timeout != 0 && self->req[i].timeout < now) {
                func_003F5738(self->sess);
                func_003F0610((int*)self, 6);
                break;
            }
        }
    }
    if (self->state == 6 || self->state == 3) {
        for (i = 0; i < 3; i++) {
            if (self->req[i].cb != 0) {
                ev.kind = self->req[i].kind;
                ev.arg = -4;
                self->req[i].cb(self, &ev, self->req[i].ctx);
                self->req[i].cb = 0;
            }
        }
    }
    while (func_003F5A70(self->sess, &ev.type, &ev.arg, &data) >= 0) {
        done = 0;
        ev.data = data;
        if (self->f508 != 0) {
            func_003EFF40((int*)self, str_recv, (int*)&ev);
        }
        if (ev.type == -1) {
            if (ev.arg == -1) {
                func_003F0610((int*)self, 5);
            }
            if (ev.type == -1 && ev.arg == (int)0xFEFEFEFE) {
                func_003F0610((int*)self, 6);
            }
        }
        if (ev.type == 0x50494E47) {
            func_003F5910(self->sess, 0x50494E47, ev.arg, (int)ev.data, -1);
        }
        if (ev.type == 0x41555448) {
            ev.kind = 0x61757468;
            self->stateCb(self, &ev, self->stateCtx);
            self->timeout = 0;
            if (ev.arg != 0) {
                func_003F0B50((struct BuddyApiRefT*)self);
            }
        }
        if (ev.type == 0x52454356 && self->admnCb != 0) {
            func_003F0198((char*)self, (int*)&ev);
        }
        if (ev.type == 0x53454E44 && self->req[2].cb != 0) {
            ev.kind = 0x73656E64;
            self->req[2].cb(self, &ev, self->req[2].ctx);
            self->req[2].cb = 0;
        }
        if (ev.type == 0x41444D4E && self->admnCb != 0) {
            ev.kind = 0x61646D6E;
            self->admnCb(self, &ev, self->admnCtx);
        }
        if (ev.type == 0x52474554) {
            if (cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(ev.data, str_ID), 0) == 2) {
                self->rostCount = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(ev.data, str_SIZE), 0);
                done = self->rostCount == 0;
            }
        }
        if (ev.type == 0x524F5354 && self->roster != 0) {
            flags = 4;
            id = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(ev.data, str_ID), 0);
            if (id != 1) flags = 0;
            if (id == 2) flags = 0x200;
            if (flags == 0x200) {
                if (--self->rostCount == 0) done = 1;
            }
            cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(ev.data, str_USER_b), name, 0x20, (char*)D_004A4AE8);
            func_003F0150(name);
            if (self->f18 != 0 || strchr(name, '.') == 0) {
                id = func_003EEC88(self->names, (int)name);
                if (id == 0) {
                    if ((unsigned int)strlen(name) < 0x20) {
                        ent = (char*)func_002591E8(184);
                        memset_a(ent, 0, 184);
                        strcpy(ent, name);
                        id = func_003EE320(self->roster, (int)ent);
                        func_003EEB30(self->names, (int)ent, id);
                    }
                }
                if (id != 0) {
                    ent = (char*)func_003EE468(self->roster, id);
                    if (ent != 0) {
                        cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(ev.data, str_GROUP), ent + 0x20, 0x20, (char*)D_004A4AE8);
                        if (flags != 0) {
                            *(int*)(ent + 0xA8) = (*(int*)(ent + 0xA8) | flags) & ~0x100000;
                        }
                    }
                }
            }
        }
        if (ev.type == 0x50474554 && self->roster != 0) {
            cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(ev.data, str_USER_b), name, 0x20, (char*)D_004A4AE8);
            func_003F0150(name);
            id = func_003EEC88(self->names, (int)name);
            if (id != 0) {
                ent = (char*)func_003EE468(self->roster, id);
                old = *(Ent_17D8*)ent;
                func_003F02D0((BuddyApiRefT*)((char*)self), (char*)&ev, ent);
                if (memcmp_a(&old, ent, 184) != 0) {
                    func_003EE600(self->roster, 1);
                    func_003EFED0((char*)self, D_00496BB8, ent, ent[0xA7], ent + 0x40, ent + 0x54, ent + 0xA4);
                }
            }
        }
        if (ev.type == 0x52414444 && self->roster != 0) {
            ev.kind = 0x61646475;
            id = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(ev.data, str_ID), 0);
            for (i = 0; (ent = (char*)func_003EE560(self->roster, i)) != 0; i++) {
                if ((*(int*)(ent + 0xA8) & 0x10000) && *(int*)(ent + 0xAC) == id) {
                    if (ev.arg != 0) {
                        if (self->req[0].cb != 0) {
                            ev.arg = -2;
                            ev.data = ent;
                            self->req[0].cb(self, &ev, self->req[0].ctx);
                        }
                        func_003EE480(self->roster, f17_EED08(self->names, ent));
                        func_00259210(ent);
                    } else {
                        cDirtysock_tag_TagFieldGetString(cDirtysock_tag_TagFieldFind(ev.data, str_FUSR), name, 0x20, (char*)D_004A4AE8);
                        func_003F0150(name);
                        if (name[0] != 0 && (unsigned int)strlen(name) < 0x20) {
                            int k = f17_EED08(self->names, ent);
                            cDirtysock_tag_TagFieldDupl(ent, 0x20, name);
                            func_003EEB30(self->names, (int)ent, k);
                        }
                        if (self->req[0].cb != 0) {
                            ev.arg = 0;
                            ev.data = ent;
                            self->req[0].cb(self, &ev, self->req[0].ctx);
                        }
                        *(int*)(ent + 0xAC) = 0;
                        *(int*)(ent + 0xA8) ^= 0x10000;
                    }
                    break;
                }
            }
            func_003EE600(self->roster, 1);
        }
        if (ev.type == 0x5244454C && self->req[1].cb != 0) {
            if (cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(ev.data, str_ID), 0) == self->req[1].seq) {
                ev.kind = 0x64656C75;
                self->req[1].cb(self, &ev, self->req[1].ctx);
                self->req[1].cb = 0;
            }
        }
        if (done != 0) {
            if (self->rostCb != 0) {
                ev.kind = 0x656E6472;
                ev.type = 0;
                ev.arg = 0;
                ev.data = 0;
                self->rostCb(self, &ev, self->rostCtx);
            }
        }
        func_003F5C38(self->sess, 0, 0, 0, 0);
    }
    if (self->rostCb != 0 && func_003EE620(self->roster) > 0) {
        ev.kind = 0x726F7374;
        ev.type = 0;
        ev.arg = 0;
        ev.data = (char*)self->roster;
        self->rostCb(self, &ev, self->rostCtx);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F2220);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

#define MD5_F(x, y, z) ((z) ^ (((y) ^ (z)) & (x)))
#define MD5_G(x, y, z) ((y) ^ (((x) ^ (y)) & (z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | ~(z)))
#define MD5_STEP(f, a, b, c, d, x, s, ac)     { (a) += f((b), (c), (d)) + (x) + (unsigned int)(ac);       (a) = ((a) << (s)) | ((a) >> (32 - (s)));       (a) += (b); }
struct Md5Ctx { unsigned int count; unsigned int state[4]; unsigned char buf[64]; };
/**
 * Runs the MD5 compression function on the 64-byte block buffered in an MD5 context.
 *
 * Loads the block (+0x14) as sixteen little-endian words, runs the four MD5 rounds of sixteen steps with the standard
 * constants and shifts, and adds the result into the state words (+0x4..+0x10). func_003F3128 calls it each time the
 * buffer fills.
 *
 * @param ctx  the MD5 context: byte count, four state words, 64-byte block
 */
void func_003F2220(struct Md5Ctx* ctx) {
    struct Md5Ctx* st = ctx;
    unsigned int x[16];
    unsigned int a, b, c, d;
    unsigned char* srcByte;
    unsigned int* dstWord;
    dstWord = x + 16;
    a = 16;
    srcByte = st->buf + 64;
    do {
        dstWord--;
        b = *--srcByte;
        a--;
        b = (b << 8) + *--srcByte;
        b = (b << 8) + *--srcByte;
        b = (b << 8) + *--srcByte;
        *dstWord = b;
    } while (a);
    a = st->state[0];
    b = st->state[1];
    c = st->state[2];
    d = st->state[3];
    MD5_STEP(MD5_F, a, b, c, d, x[0], 7, 0xD76AA478U);
    MD5_STEP(MD5_F, d, a, b, c, x[1], 12, 0xE8C7B756U);
    MD5_STEP(MD5_F, c, d, a, b, x[2], 17, 0x242070DBU);
    MD5_STEP(MD5_F, b, c, d, a, x[3], 22, 0xC1BDCEEEU);
    MD5_STEP(MD5_F, a, b, c, d, x[4], 7, 0xF57C0FAFU);
    MD5_STEP(MD5_F, d, a, b, c, x[5], 12, 0x4787C62AU);
    MD5_STEP(MD5_F, c, d, a, b, x[6], 17, 0xA8304613U);
    MD5_STEP(MD5_F, b, c, d, a, x[7], 22, 0xFD469501U);
    MD5_STEP(MD5_F, a, b, c, d, x[8], 7, 0x698098D8U);
    MD5_STEP(MD5_F, d, a, b, c, x[9], 12, 0x8B44F7AFU);
    MD5_STEP(MD5_F, c, d, a, b, x[10], 17, 0xFFFF5BB1U);
    MD5_STEP(MD5_F, b, c, d, a, x[11], 22, 0x895CD7BEU);
    MD5_STEP(MD5_F, a, b, c, d, x[12], 7, 0x6B901122U);
    MD5_STEP(MD5_F, d, a, b, c, x[13], 12, 0xFD987193U);
    MD5_STEP(MD5_F, c, d, a, b, x[14], 17, 0xA679438EU);
    MD5_STEP(MD5_F, b, c, d, a, x[15], 22, 0x49B40821U);
    MD5_STEP(MD5_G, a, b, c, d, x[1], 5, 0xF61E2562U);
    MD5_STEP(MD5_G, d, a, b, c, x[6], 9, 0xC040B340U);
    MD5_STEP(MD5_G, c, d, a, b, x[11], 14, 0x265E5A51U);
    MD5_STEP(MD5_G, b, c, d, a, x[0], 20, 0xE9B6C7AAU);
    MD5_STEP(MD5_G, a, b, c, d, x[5], 5, 0xD62F105DU);
    MD5_STEP(MD5_G, d, a, b, c, x[10], 9, 0x02441453U);
    MD5_STEP(MD5_G, c, d, a, b, x[15], 14, 0xD8A1E681U);
    MD5_STEP(MD5_G, b, c, d, a, x[4], 20, 0xE7D3FBC8U);
    MD5_STEP(MD5_G, a, b, c, d, x[9], 5, 0x21E1CDE6U);
    MD5_STEP(MD5_G, d, a, b, c, x[14], 9, 0xC33707D6U);
    MD5_STEP(MD5_G, c, d, a, b, x[3], 14, 0xF4D50D87U);
    MD5_STEP(MD5_G, b, c, d, a, x[8], 20, 0x455A14EDU);
    MD5_STEP(MD5_G, a, b, c, d, x[13], 5, 0xA9E3E905U);
    MD5_STEP(MD5_G, d, a, b, c, x[2], 9, 0xFCEFA3F8U);
    MD5_STEP(MD5_G, c, d, a, b, x[7], 14, 0x676F02D9U);
    MD5_STEP(MD5_G, b, c, d, a, x[12], 20, 0x8D2A4C8AU);
    MD5_STEP(MD5_H, a, b, c, d, x[5], 4, 0xFFFA3942U);
    MD5_STEP(MD5_H, d, a, b, c, x[8], 11, 0x8771F681U);
    MD5_STEP(MD5_H, c, d, a, b, x[11], 16, 0x6D9D6122U);
    MD5_STEP(MD5_H, b, c, d, a, x[14], 23, 0xFDE5380CU);
    MD5_STEP(MD5_H, a, b, c, d, x[1], 4, 0xA4BEEA44U);
    MD5_STEP(MD5_H, d, a, b, c, x[4], 11, 0x4BDECFA9U);
    MD5_STEP(MD5_H, c, d, a, b, x[7], 16, 0xF6BB4B60U);
    MD5_STEP(MD5_H, b, c, d, a, x[10], 23, 0xBEBFBC70U);
    MD5_STEP(MD5_H, a, b, c, d, x[13], 4, 0x289B7EC6U);
    MD5_STEP(MD5_H, d, a, b, c, x[0], 11, 0xEAA127FAU);
    MD5_STEP(MD5_H, c, d, a, b, x[3], 16, 0xD4EF3085U);
    MD5_STEP(MD5_H, b, c, d, a, x[6], 23, 0x04881D05U);
    MD5_STEP(MD5_H, a, b, c, d, x[9], 4, 0xD9D4D039U);
    MD5_STEP(MD5_H, d, a, b, c, x[12], 11, 0xE6DB99E5U);
    MD5_STEP(MD5_H, c, d, a, b, x[15], 16, 0x1FA27CF8U);
    MD5_STEP(MD5_H, b, c, d, a, x[2], 23, 0xC4AC5665U);
    MD5_STEP(MD5_I, a, b, c, d, x[0], 6, 0xF4292244U);
    MD5_STEP(MD5_I, d, a, b, c, x[7], 10, 0x432AFF97U);
    MD5_STEP(MD5_I, c, d, a, b, x[14], 15, 0xAB9423A7U);
    MD5_STEP(MD5_I, b, c, d, a, x[5], 21, 0xFC93A039U);
    MD5_STEP(MD5_I, a, b, c, d, x[12], 6, 0x655B59C3U);
    MD5_STEP(MD5_I, d, a, b, c, x[3], 10, 0x8F0CCC92U);
    MD5_STEP(MD5_I, c, d, a, b, x[10], 15, 0xFFEFF47DU);
    MD5_STEP(MD5_I, b, c, d, a, x[1], 21, 0x85845DD1U);
    MD5_STEP(MD5_I, a, b, c, d, x[8], 6, 0x6FA87E4FU);
    MD5_STEP(MD5_I, d, a, b, c, x[15], 10, 0xFE2CE6E0U);
    MD5_STEP(MD5_I, c, d, a, b, x[6], 15, 0xA3014314U);
    MD5_STEP(MD5_I, b, c, d, a, x[13], 21, 0x4E0811A1U);
    MD5_STEP(MD5_I, a, b, c, d, x[4], 6, 0xF7537E82U);
    MD5_STEP(MD5_I, d, a, b, c, x[11], 10, 0xBD3AF235U);
    MD5_STEP(MD5_I, c, d, a, b, x[2], 15, 0x2AD7D2BBU);
    MD5_STEP(MD5_I, b, c, d, a, x[9], 21, 0xEB86D391U);
    st->state[0] += a;
    st->state[1] += b;
    st->state[2] += c;
    st->state[3] += d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F30F0);
#ifdef SKIP_ASM
/**
 * Initialises an MD5 context: byte count 0 and the standard state words 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476.
 *
 * @param md5Ctx  the context
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003F30F0(unsigned int* md5Ctx) {
    md5Ctx[0] = 0;
    md5Ctx[1] = 0x67452301;
    md5Ctx[2] = 0xEFCDAB89;
    md5Ctx[3] = 0x98BADCFE;
    md5Ctx[4] = 0x10325476;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F0F40", func_003F3128);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).





extern void func_003F2220(struct Md5Ctx*);
/**
 * Feeds bytes into an MD5 context.
 *
 * A negative @p len means @p data is NUL-terminated (its length is counted). Bytes are appended to the block buffer one
 * at a time, counting them in +0x0, and func_003F2220 hashes each full 64-byte block.
 *
 * @param ctx  the MD5 context
 * @param data  bytes to hash
 * @param len  byte count, or negative for a C string
 */
void func_003F3128(struct Md5Ctx* ctx, unsigned char* data, int len) {
    unsigned int idx;
    unsigned char* cursor;
    cursor = data;
    if (len < 0) {
        len = 0;
        while (data[len]) {
            len++;
        }
    }
    idx = ctx->count & 0x3F;
    while (len > 0) {
        ctx->buf[idx] = *cursor++;
        ctx->count++;
        idx++;
        if (idx == 0x40) {
            func_003F2220(ctx);
            idx = 0;
        }
        len--;
    }
}
#endif
