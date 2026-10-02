#include "common.h"

INCLUDE_ASM("util/statemachine", cConsoleConfig_setTimeString);

//100%
INCLUDE_ASM("util/statemachine", func_002C7440);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002C7478(void* self);
extern char D_00486560[];

extern "C" void* func_002C7440(void)
{
    return func_002C7478(cMemMan_alloc(0x38, D_00486560, 0, 0));
}
#endif

INCLUDE_ASM("util/statemachine", func_002C7478);

INCLUDE_ASM("util/statemachine", func_002C76C0);

