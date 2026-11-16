#include "common.h"

extern "C" void cWorldPainterQuery_reset(void* self);

//100%
INCLUDE_ASM("worldpainter/worldpainterman", cWorldPainterMan_reset__FPv);
#ifdef SKIP_ASM
/**
 * Resets the world painter manager by resetting its query with cWorldPainterQuery_reset().
 */
void cWorldPainterMan_reset(void* self)
{
    cWorldPainterQuery_reset(self);
}
#endif

