#ifndef MD5_H
#define MD5_H

#include "common.h"

// 0x58 bytes: message length in bits, the four chaining words, and the
// pending 64-byte input block
typedef struct md5_state_s {
    uint count[2];
    uint abcd[4];
    unsigned char buf[64];
} md5_state_t;

extern "C" {
void md5_init(md5_state_t* state);
void md5_append(md5_state_t* state, const unsigned char* data, int size);
void md5_finish(md5_state_t* state, unsigned char digest[16]);
}

#endif
