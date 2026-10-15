#include "common.h"

extern "C" void cWorldPainterQuery_reset(void* self);

//100%
void cWorldPainterMan_reset(void* self)
{
    cWorldPainterQuery_reset(self);
}

