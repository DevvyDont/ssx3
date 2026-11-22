#include "common.h"

int GetHashValue32(char* param_1);
int tHashName32_getHashValue(uint*, char*);


//100%
//https://decomp.me/scratch/YWMcE
INCLUDE_ASM("hashvalue", tHashName32_getHashValue__FPUiPc);
#ifdef SKIP_ASM
/**
 * Hashes a string to 32 bits with a PJW/ELF-style hash.
 *
 * Each character is added after shifting the hash left by 4; when the top nibble becomes nonzero it is folded in
 * with hash ^= top >> 23 and then cleared. The fold shift is 23 where the classic ELF hash uses 24, so results
 * differ from that hash.
 *
 * @param out  receives the hash
 * @param str  NUL-terminated string to hash
 * @return the hash (the value stored in @p out)
 */
int tHashName32_getHashValue(uint* out, char* str) {
    uint hash = 0;
    uint top;

    while (*str) {
        hash = (hash << 4) + *str++;
        top = hash & 0xF0000000;
        if (top != 0) {
            hash = (hash ^ (top >> 23)) ^ top;
        }
    }

    *out = hash;
    return *out;
}
#endif

//100%
//https://decomp.me/scratch/VD967
INCLUDE_ASM("hashvalue", GetHashValue32__FPc);
#ifdef SKIP_ASM
/**
 * Returns the 32-bit name hash of a string.
 *
 * Calls tHashName32_getHashValue() with a scratch output word.
 *
 * @param param_1  NUL-terminated string to hash
 * @return the hash
 */
int GetHashValue32(char* param_1) {
    char hash[4];
    return tHashName32_getHashValue((uint*)hash, param_1);
}
#endif

//https://decomp.me/scratch/ZrcdK
//100%
INCLUDE_ASM("hashvalue", tHashName64_getHashValue__FPUlPc);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct md5_ctx {
    unsigned int lo;    // 0x0
    unsigned int hi;    // 0x4
    unsigned int a;     // 0x8
    unsigned int b;     // 0xC
    unsigned int c;     // 0x10
    unsigned int d;     // 0x14
    unsigned char buffer[64]; // 0x18
};

void md5_init(md5_ctx* ctx);
extern "C" void md5_append(md5_ctx* ctx, char* data, unsigned int len);
extern "C" void md5_finish(md5_ctx* ctx, ulong* digest);
extern "C" unsigned int strlen(const char*);

/**
 * Hashes a string to 64 bits by taking the first half of its MD5 digest.
 *
 * @param out  receives the hash
 * @param str  NUL-terminated string to hash; its strlen() bytes are digested
 * @return the hash (the value stored in @p out)
 */
ulong tHashName64_getHashValue(ulong* out, char* str)
{
    md5_ctx ctx;
    ulong hash[2];

    md5_init(&ctx);
    md5_append(&ctx, str, strlen(str));
    md5_finish(&ctx, hash);
    *out = hash[0];
    return hash[0];
}
#endif


//100%
//https://decomp.me/scratch/I4NLb
INCLUDE_ASM("hashvalue", GetHashValue64__FPc);
#ifdef SKIP_ASM
/**
 * Returns the 64-bit (MD5-based) name hash of a string.
 *
 * Calls tHashName64_getHashValue() with a scratch output value.
 *
 * @param str  NUL-terminated string to hash
 * @return the hash
 */
ulong GetHashValue64(char* str) {
    char hash[8];
    return tHashName64_getHashValue((ulong*)hash, str);
}
#endif

//100%
INCLUDE_ASM("hashvalue", tHashName128_getHashValue);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sHash128_17710 {
    ulong lo;
    ulong hi;
};
// harness-only: sHash128 is defined later in the unit (GetHashValue128's block)
sHash128_17710 tHashName128_getHashValue_impl(sHash128_17710* out, char* str) __asm__("tHashName128_getHashValue");

/**
 * Hashes a string to 128 bits: its full MD5 digest.
 *
 * Runs md5_init, md5_append over the strlen() bytes of @p str and md5_finish, copies the 16-byte digest to *@p out and
 * returns it by value. The 128-bit counterpart of tHashName64_getHashValue, which keeps only the first 8 bytes. Only
 * GetHashValue128 calls it.
 *
 * @param out  receives the digest
 * @param str  NUL-terminated string to hash
 * @return the digest (the value stored in *@p out)
 */
sHash128_17710 tHashName128_getHashValue_impl(sHash128_17710* out, char* str)
{
    md5_ctx ctx;
    sHash128_17710 digest;

    md5_init(&ctx);
    md5_append(&ctx, str, strlen(str));
    md5_finish(&ctx, (ulong*)&digest);
    *out = digest;
    return *out;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("hashvalue", GetHashValue128);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sHash128 {
    ulong lo;
    ulong hi;
};

extern "C" sHash128 tHashName128_getHashValue(sHash128* out, char* str);

/**
 * Returns the 128-bit (MD5) hash of a string.
 *
 * Calls tHashName128_getHashValue with a scratch output, as GetHashValue64 does for the 64-bit hash. Nothing calls it.
 *
 * @param str  NUL-terminated string to hash
 * @return the 16-byte MD5 digest
 */
extern "C" sHash128 GetHashValue128(char* str)
{
    sHash128 digest;
    return tHashName128_getHashValue(&digest, str);
}
#endif
