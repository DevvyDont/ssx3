// MD5 (RFC 1321). The game's implementation has the same interface and
// structure as L. Peter Deutsch's freely licensed md5.c (Aladdin Enterprises);
// this file was reconstructed from the game's assembly.
#include "md5.h"

extern "C" void* memcpy(void* dst, const void* src, uint size);
extern "C" void md5_process(md5_state_t* pms, const unsigned char* data);

// 64-byte final padding block: 0x80 followed by zeros
extern const unsigned char D_0048DAF8[64];

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

#define SET(f, a, b, c, d, k, s, Ti) \
    t = a + f(b, c, d) + X[k] + Ti; \
    a = ROTATE_LEFT(t, s) + b

// Runs the MD5 compression function over one 64-byte block
void md5_process(md5_state_t* pms, const unsigned char* data)
{
    uint a = pms->abcd[0];
    uint b = pms->abcd[1];
    uint c = pms->abcd[2];
    uint d = pms->abcd[3];
    uint t;
    uint xbuf[16];
    const uint* X;

    // The block is read as little-endian words in place when aligned
    if (!((uint)data & 3)) {
        X = (const uint*)data;
    } else {
        memcpy(xbuf, data, 64);
        X = xbuf;
    }

    // Round 1
    SET(F, a, b, c, d,  0,  7, 0xD76AA478);
    SET(F, d, a, b, c,  1, 12, 0xE8C7B756);
    SET(F, c, d, a, b,  2, 17, 0x242070DB);
    SET(F, b, c, d, a,  3, 22, 0xC1BDCEEE);
    SET(F, a, b, c, d,  4,  7, 0xF57C0FAF);
    SET(F, d, a, b, c,  5, 12, 0x4787C62A);
    SET(F, c, d, a, b,  6, 17, 0xA8304613);
    SET(F, b, c, d, a,  7, 22, 0xFD469501);
    SET(F, a, b, c, d,  8,  7, 0x698098D8);
    SET(F, d, a, b, c,  9, 12, 0x8B44F7AF);
    SET(F, c, d, a, b, 10, 17, 0xFFFF5BB1);
    SET(F, b, c, d, a, 11, 22, 0x895CD7BE);
    SET(F, a, b, c, d, 12,  7, 0x6B901122);
    SET(F, d, a, b, c, 13, 12, 0xFD987193);
    SET(F, c, d, a, b, 14, 17, 0xA679438E);
    SET(F, b, c, d, a, 15, 22, 0x49B40821);

    // Round 2
    SET(G, a, b, c, d,  1,  5, 0xF61E2562);
    SET(G, d, a, b, c,  6,  9, 0xC040B340);
    SET(G, c, d, a, b, 11, 14, 0x265E5A51);
    SET(G, b, c, d, a,  0, 20, 0xE9B6C7AA);
    SET(G, a, b, c, d,  5,  5, 0xD62F105D);
    SET(G, d, a, b, c, 10,  9, 0x02441453);
    SET(G, c, d, a, b, 15, 14, 0xD8A1E681);
    SET(G, b, c, d, a,  4, 20, 0xE7D3FBC8);
    SET(G, a, b, c, d,  9,  5, 0x21E1CDE6);
    SET(G, d, a, b, c, 14,  9, 0xC33707D6);
    SET(G, c, d, a, b,  3, 14, 0xF4D50D87);
    SET(G, b, c, d, a,  8, 20, 0x455A14ED);
    SET(G, a, b, c, d, 13,  5, 0xA9E3E905);
    SET(G, d, a, b, c,  2,  9, 0xFCEFA3F8);
    SET(G, c, d, a, b,  7, 14, 0x676F02D9);
    SET(G, b, c, d, a, 12, 20, 0x8D2A4C8A);

    // Round 3
    SET(H, a, b, c, d,  5,  4, 0xFFFA3942);
    SET(H, d, a, b, c,  8, 11, 0x8771F681);
    SET(H, c, d, a, b, 11, 16, 0x6D9D6122);
    SET(H, b, c, d, a, 14, 23, 0xFDE5380C);
    SET(H, a, b, c, d,  1,  4, 0xA4BEEA44);
    SET(H, d, a, b, c,  4, 11, 0x4BDECFA9);
    SET(H, c, d, a, b,  7, 16, 0xF6BB4B60);
    SET(H, b, c, d, a, 10, 23, 0xBEBFBC70);
    SET(H, a, b, c, d, 13,  4, 0x289B7EC6);
    SET(H, d, a, b, c,  0, 11, 0xEAA127FA);
    SET(H, c, d, a, b,  3, 16, 0xD4EF3085);
    SET(H, b, c, d, a,  6, 23, 0x04881D05);
    SET(H, a, b, c, d,  9,  4, 0xD9D4D039);
    SET(H, d, a, b, c, 12, 11, 0xE6DB99E5);
    SET(H, c, d, a, b, 15, 16, 0x1FA27CF8);
    SET(H, b, c, d, a,  2, 23, 0xC4AC5665);

    // Round 4
    SET(I, a, b, c, d,  0,  6, 0xF4292244);
    SET(I, d, a, b, c,  7, 10, 0x432AFF97);
    SET(I, c, d, a, b, 14, 15, 0xAB9423A7);
    SET(I, b, c, d, a,  5, 21, 0xFC93A039);
    SET(I, a, b, c, d, 12,  6, 0x655B59C3);
    SET(I, d, a, b, c,  3, 10, 0x8F0CCC92);
    SET(I, c, d, a, b, 10, 15, 0xFFEFF47D);
    SET(I, b, c, d, a,  1, 21, 0x85845DD1);
    SET(I, a, b, c, d,  8,  6, 0x6FA87E4F);
    SET(I, d, a, b, c, 15, 10, 0xFE2CE6E0);
    SET(I, c, d, a, b,  6, 15, 0xA3014314);
    SET(I, b, c, d, a, 13, 21, 0x4E0811A1);
    SET(I, a, b, c, d,  4,  6, 0xF7537E82);
    SET(I, d, a, b, c, 11, 10, 0xBD3AF235);
    SET(I, c, d, a, b,  2, 15, 0x2AD7D2BB);
    SET(I, b, c, d, a,  9, 21, 0xEB86D391);

    pms->abcd[0] += a;
    pms->abcd[1] += b;
    pms->abcd[2] += c;
    pms->abcd[3] += d;
}

void md5_init(md5_state_t* pms)
{
    pms->count[0] = pms->count[1] = 0;
    pms->abcd[0] = 0x67452301;
    pms->abcd[1] = 0xEFCDAB89;
    pms->abcd[2] = 0x98BADCFE;
    pms->abcd[3] = 0x10325476;
}

void md5_append(md5_state_t* pms, const unsigned char* data, int nbytes)
{
    const unsigned char* p = data;
    int left = nbytes;
    int offset = (pms->count[0] >> 3) & 63;
    uint nbits = (uint)(nbytes << 3);

    if (nbytes <= 0)
        return;

    // Message length in bits, as a 64-bit count
    pms->count[1] += nbytes >> 29;
    pms->count[0] += nbits;
    if (pms->count[0] < nbits)
        pms->count[1]++;

    // Fill out a partially filled block first
    if (offset) {
        int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);

        memcpy(pms->buf + offset, p, copy);
        if (offset + copy < 64)
            return;
        p += copy;
        left -= copy;
        md5_process(pms, pms->buf);
    }

    for (; left >= 64; p += 64, left -= 64)
        md5_process(pms, p);

    // Keep the remainder for next time
    if (left)
        memcpy(pms->buf, p, left);
}

void md5_finish(md5_state_t* pms, unsigned char digest[16])
{
    unsigned char data[8];
    int i;

    // Save the length before padding changes it
    for (i = 0; i < 8; ++i)
        data[i] = (unsigned char)(pms->count[i >> 2] >> ((i & 3) << 3));
    // Pad to 56 bytes mod 64, then append the length
    md5_append(pms, D_0048DAF8, ((55 - (pms->count[0] >> 3)) & 63) + 1);
    md5_append(pms, data, 8);
    for (i = 0; i < 16; ++i)
        digest[i] = (unsigned char)(pms->abcd[i >> 2] >> ((i & 3) << 3));
}
