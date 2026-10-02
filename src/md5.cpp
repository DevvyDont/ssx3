#include "md5.h"

struct md5_ctx {
    unsigned int lo; // 0x0
    unsigned int hi; // 0x4
    unsigned int a; // 0x8
    unsigned int b; // 0xC
    unsigned int c; // 0x10
    unsigned int d; // 0x14
};

INCLUDE_ASM("md5", md5_process);
#ifdef SKIP_ASM
#endif

//100%
INCLUDE_ASM("md5", md5_init__FP7md5_ctx);
#ifdef SKIP_ASM
void md5_init(md5_ctx* ctx)
{
    unsigned int a = 0x67452301;
    unsigned int b = 0xEFCDAB89;
    unsigned int c = 0x98BADCFE;
    unsigned int d = 0x10325476;
    ctx->a = a;
    ctx->d = d;
    ctx->b = b;
    ctx->c = c;
    ctx->lo = 0;
    ctx->hi = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("md5", md5_append);
#ifdef SKIP_ASM
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern "C" void md5_process(md5_ctx* ctx, const unsigned char* data);

// Same layout as md5_ctx, viewed as the reference md5.c's md5_state_t.
struct md5_state_append {
    unsigned int count[2];
    unsigned int abcd[4];
    unsigned char buf[64];
};

extern "C" void md5_append(md5_ctx* ctx, const unsigned char* data, int nbytes)
{
    md5_state_append* pms = (md5_state_append*)ctx;
    const unsigned char* p = data;
    int left = nbytes;
    int offset = (pms->count[0] >> 3) & 63;
    unsigned int nbits = (unsigned int)(nbytes << 3);

    if (nbytes <= 0) {
        return;
    }

    pms->count[1] += nbytes >> 29;
    pms->count[0] += nbits;
    if (pms->count[0] < nbits) {
        pms->count[1]++;
    }

    if (offset) {
        int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);

        // PORT: pointer-in-int; the target adds offset to the state pointer before the buf offset (pms->buf + offset)
        func_0041605C((unsigned char*)(offset + (int)pms) + 0x18, p, copy);
        if (offset + copy < 64) {
            return;
        }
        p += copy;
        left -= copy;
        md5_process(ctx, pms->buf);
    }

    for (; left >= 64; p += 64, left -= 64) {
        md5_process(ctx, p);
    }

    if (left) {
        func_0041605C(pms->buf, p, left);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("md5", md5_finish);
#ifdef SKIP_ASM
// Same layout as md5_ctx, viewed as the reference md5.c's md5_state_t (count[2], abcd[4]).
struct md5_state_K4 {
    unsigned int count[2];  // 0x0
    unsigned int abcd[4];   // 0x8
};

extern "C" void md5_append(md5_ctx* ctx, const unsigned char* data, int nbytes);
extern unsigned char D_0048DAF8[];

extern "C" void md5_finish(md5_ctx* ctx, unsigned char* digest)
{
    md5_state_K4* pms = (md5_state_K4*)ctx;
    unsigned char data[8];
    int i;

    for (i = 0; i < 8; ++i) {
        data[i] = (unsigned char)(pms->count[i >> 2] >> ((i & 3) << 3));
    }
    md5_append(ctx, D_0048DAF8, ((55 - (pms->count[0] >> 3)) & 63) + 1);
    md5_append(ctx, data, 8);
    for (i = 0; i < 16; ++i) {
        digest[i] = (unsigned char)(pms->abcd[i >> 2] >> ((i & 3) << 3));
    }
}
#endif
#ifdef SKIP_ASM
#endif