#include "common.h"

//100%
INCLUDE_ASM("input/inputmap", cInputMap_init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_003E6448(void* dst, unsigned int value, int size);
extern char D_0048DD00[];

extern "C" void cInputMap_init(void* self, int count)
{
    *(int*)((char*)self + 0x0) = count;
    *(void**)((char*)self + 0x4) = operator_new_tag(count * 4, D_0048DD00, 0, 0);
    func_003E6448(*(void**)((char*)self + 0x4), 0xFFFFFFFF, *(int*)((char*)self + 0x0) * 4);
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_loadMapFile);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00321428(void* self, void* a1, void* a2);
extern char D_0048DD10[];

extern "C" void cInputMap_loadMapFile(void* self, void* a1, void* a2)
{
    *(void**)((char*)self + 0x8) = func_00321428(cMemMan_alloc(0x109EC, D_0048DD10, 0x100, 0), a1, a2);
}
#endif

INCLUDE_ASM("input/inputmap", cInputMap_compileMap);

INCLUDE_ASM("input/inputmap", cInputMap_purgeMapFile);

//100%
INCLUDE_ASM("input/inputmap", func_00320FA8);
#ifdef SKIP_ASM
void* func_00325250(void* self);
extern "C" float func_00325450(void* parser, void* a1, void* a2);

extern "C" float func_00320FA8(void* self, void* a1, int a2)
{
    char parser[0x190];
    func_00325250(parser);
    return func_00325450(parser, a1, (char*)*(void**)((char*)self + 0x14) + ((*(int**)((char*)self + 0x4))[a2] << 2));
}
#endif

//100%
INCLUDE_ASM("input/inputmap", func_00321108);
#ifdef SKIP_ASM
void* func_00325250(void* self);
extern "C" float func_00325450(void* parser, void* a1, void* a2);

extern "C" int func_00321108(void* self, void* a1, int a2)
{
    char parser[0x190];
    func_00325250(parser);
    return func_00325450(parser, a1, (char*)*(void**)((char*)self + 0x14) + ((*(int**)((char*)self + 0x4))[a2] << 2)) != 0.0f;
}
#endif

INCLUDE_ASM("input/inputmap", func_00321298);

INCLUDE_ASM("input/inputmap", func_00321428);

INCLUDE_ASM("input/inputmap", func_00321500);

//100%
INCLUDE_ASM("input/inputmap", func_00321590);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(void* self);

extern "C" void func_00321590(void* self)
{
    while (*(int*)((char*)self + 0x98) != 5 && *(int*)((char*)self + 0x98) != 0) {
        cInputMapParser_readToken(self);
    }
    cInputMapParser_readToken(self);
}
#endif

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

