#include "common.h"

//100%
struct sWPQuery {
    float* result;          // 0x0
    char pad04[0x10];
    sWPQuery* next;         // 0x14
};

extern sWPQuery* D_004A3880;

extern "C" void cWorldPainterQuery_reset(void)
{
    for (sWPQuery* q = D_004A3880; q != 0; q = q->next) {
        *q->result = -99999.0f;
    }
}

