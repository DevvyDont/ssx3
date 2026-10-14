#include "common.h"
#include "md5.h"

extern "C" int strlen(const char* str);

struct tHash128 {
    ulong word[2];
};

int GetHashValue32(char* param_1);
int tHashName32_getHashValue(uint*, char*);

//https://decomp.me/scratch/YWMcE
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

//https://decomp.me/scratch/VD967
int GetHashValue32(char* param_1) {
    char hash[4];
    return tHashName32_getHashValue((uint*)hash, param_1);
}

// First 8 bytes of the string's MD5
ulong tHashName64_getHashValue(ulong* out, char* str) {
    md5_state_t state;
    ulong digest[2];

    md5_init(&state);
    md5_append(&state, (unsigned char*)str, strlen(str));
    md5_finish(&state, (unsigned char*)digest);
    *out = digest[0];
    return *out;
}

//https://decomp.me/scratch/I4NLb
ulong GetHashValue64(char* str) {
    char hash[8];
    return tHashName64_getHashValue((ulong*)hash, str);
}

// Full 16-byte MD5 of the string; the 128-bit counterpart of
// tHashName64_getHashValue
tHash128 func_00317710(tHash128* out, char* str) {
    md5_state_t state;
    tHash128 digest;

    md5_init(&state);
    md5_append(&state, (unsigned char*)str, strlen(str));
    md5_finish(&state, (unsigned char*)&digest);
    *out = digest;
    return *out;
}

// The 128-bit counterpart of GetHashValue64
tHash128 func_00317798(char* str) {
    tHash128 hash;
    return func_00317710(&hash, str);
}
