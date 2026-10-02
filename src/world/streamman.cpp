#include "common.h"

INCLUDE_ASM("world/streamman", cStreamMan_cStreamMan);

INCLUDE_ASM("world/streamman", func_003A6E20);

//100%
INCLUDE_ASM("world/streamman", func_003A6E98);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern const char D_004A47D8[];
extern const char D_004A47E0[];

extern "C" int func_003A6E98(void* self, const char* name, int id)
{
    *(int*)self = id;
    sprintf((char*)self + 4, D_004A47D8, name, D_004A47E0);
    return 1;
}
#endif

INCLUDE_ASM("world/streamman", func_003A6ED8);

INCLUDE_ASM("world/streamman", func_003A6F38);

INCLUDE_ASM("world/streamman", func_003A6F88);

INCLUDE_ASM("world/streamman", func_003A7010);

INCLUDE_ASM("world/streamman", func_003A7058);

INCLUDE_ASM("world/streamman", func_003A7098);

extern "C" void* func_003B47F8(int);

//100%
INCLUDE_ASM("world/streamman", func_003A7218__FPviii);
#ifdef SKIP_ASM
void* func_003A7218(void* self, int a1, int a2, int a3)
{
    return func_003B47F8(a3);
}
#endif

INCLUDE_ASM("world/streamman", func_003A7238);

