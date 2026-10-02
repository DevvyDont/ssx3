#include "common.h"

int GetHashValue32(char* param_1);
int tHashName32_getHashValue(uint*, char*);


//100%
//https://decomp.me/scratch/YWMcE
INCLUDE_ASM("hashvalue", tHashName32_getHashValue__FPUiPc);
#ifdef SKIP_ASM
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
ulong GetHashValue64(char* str) {
    char hash[8];
    return tHashName64_getHashValue((ulong*)hash, str);
}
#endif

//100%
INCLUDE_ASM("hashvalue", func_00317710);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sHash128_17710 {
    ulong lo;
    ulong hi;
};
// harness-only: sHash128 is defined later in the unit (func_00317798's block)
sHash128_17710 func_00317710_impl(sHash128_17710* out, char* str) __asm__("func_00317710");

sHash128_17710 func_00317710_impl(sHash128_17710* out, char* str)
{
    md5_ctx ctx;
    sHash128_17710 h;

    md5_init(&ctx);
    md5_append(&ctx, str, strlen(str));
    md5_finish(&ctx, (ulong*)&h);
    *out = h;
    return *out;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("hashvalue", func_00317798);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sHash128 {
    ulong lo;
    ulong hi;
};

extern "C" sHash128 func_00317710(sHash128* out, char* str);

extern "C" sHash128 func_00317798(char* str)
{
    sHash128 tmp;
    return func_00317710(&tmp, str);
}
#endif
