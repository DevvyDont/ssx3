#include "common.h"

INCLUDE_ASM("input/inputmap", cInputMap_init);

INCLUDE_ASM("input/inputmap", cInputMap_loadMapFile);

INCLUDE_ASM("input/inputmap", cInputMap_compileMap);

INCLUDE_ASM("input/inputmap", cInputMap_purgeMapFile);

INCLUDE_ASM("input/inputmap", func_00320FA8);

INCLUDE_ASM("input/inputmap", func_00321108);

INCLUDE_ASM("input/inputmap", func_00321298);

INCLUDE_ASM("input/inputmap", func_00321428);

INCLUDE_ASM("input/inputmap", func_00321500);

INCLUDE_ASM("input/inputmap", func_00321590);

//100%
INCLUDE_ASM("input/inputmap", func_003215F8);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

extern "C" void* func_003215F8(void* self)
{
    void* p = *(void**)((char*)self + 0xE0);
    *(void**)((char*)self + 0xE0) = *(void**)((char*)p + 4);
    func_00416210(p, 0, 0x10);
    return p;
}
#endif

INCLUDE_ASM("input/inputmap", func_00321638);

INCLUDE_ASM("input/inputmap", func_003216E8);

