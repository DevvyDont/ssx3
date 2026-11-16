#include "common.h"

//100%
INCLUDE_ASM("worldpainter/worldpainterquery", cWorldPainterQuery_reset);
#ifdef SKIP_ASM
struct sWPQuery {
    float* result;          // 0x0
    char pad04[0x10];
    sWPQuery* next;         // 0x14
};

extern sWPQuery* D_004A3880;

/**
 * Resets the result of every world-painter query.
 *
 * Walks the global query list (D_004A3880, linked through offset 0x14) and writes -99999.0 into the first
 * float of each query's implementation object, probably as a "no result yet" marker.
 */
extern "C" void cWorldPainterQuery_reset(void)
{
    for (sWPQuery* query = D_004A3880; query != 0; query = query->next) {
        *query->result = -99999.0f;
    }
}
#endif

