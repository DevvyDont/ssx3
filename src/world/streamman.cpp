#include "common.h"

INCLUDE_ASM("world/streamman", cStreamMan_cStreamMan);

//100%
INCLUDE_ASM("world/streamman", func_003A6E20);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void func_003A6ED8(void* self);
extern "C" void func_003A6F38(void* self);

extern "C" void func_003A6E20(void* self, int flags)
{
    void* buf = *(void**)((char*)self + 0xCC);
    if (buf != 0 && *(int*)((char*)self + 0xC0) != 0 && *(unsigned char*)((char*)self + 0xBC) == 0xFF) {
        cMemMan_free(buf);
    }
    func_003A6ED8(self);
    func_003A6F38(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

//100%
INCLUDE_ASM("world/streamman", func_003A6ED8);
#ifdef SKIP_ASM
extern "C" void func_003A7058(void* self, int size);

extern "C" void func_003A6ED8(void* self)
{
    *(int*)((char*)self + 0x94) = -1;
    *(int*)((char*)self + 0xD4) = 1;
    *(int*)((char*)self + 0x98) = 0;
    *(char*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x90) = 0;
    *(int*)((char*)self + 0xC0) = 0;
    *(int*)((char*)self + 0xCC) = 0;
    *(int*)((char*)self + 0xD0) = 0;
    *(int*)((char*)self + 0xC8) = 0;
    *(int*)((char*)self + 0xC4) = 0;
    *(int*)((char*)self + 0xAC) = 0;
    *(int*)((char*)self + 0xB0) = 0;
    *(int*)((char*)self + 0xB4) = 0;
    func_003A7058(self, 0x19000);
}
#endif

//100%
INCLUDE_ASM("world/streamman", func_003A6F38);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void func_003E0A28(int h);

extern "C" void func_003A6F38(void* self)
{
    func_003E0A28(*(int*)((char*)self + 0x88));
    if (*(void**)((char*)self + 0x84) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x84));
    }
    if (*(void**)((char*)self + 0x8C) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8C));
    }
}
#endif

//100%
INCLUDE_ASM("world/streamman", func_003A6F88);
#ifdef SKIP_ASM
extern "C" int func_003E0C88(int h, void* p, int a, int tag);

extern "C" int func_003A6F88(void* self, int a1, int a2, int a3)
{
    if (*(int*)((char*)self + 0x90) == 0) {
        int r = func_003E0C88(*(int*)((char*)self + 0x88), (char*)self + 0x4, a3, 0x444E4543);
        *(int*)((char*)self + 0x9C) = r;
        if (r != 0) {
            *(int*)((char*)self + 0x94) = a1;
            *(int*)((char*)self + 0x90) = 1;
            *(int*)((char*)self + 0x98) = a2;
            *(int*)((char*)self + 0xA0) = 0;
            *(int*)((char*)self + 0xC4) = 0;
            *(int*)((char*)self + 0xE4) = 0;
            *(int*)((char*)self + 0xE0) = 0;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/streamman", func_003A7010);
#ifdef SKIP_ASM
extern "C" void func_003E0E90(int, int);

extern "C" void func_003A7010(void* self)
{
    if (*(int*)((char*)self + 0x90) != 0) {
        func_003E0E90(*(int*)((char*)self + 0x88), *(int*)((char*)self + 0x9C));
        *(int*)((char*)self + 0x90) = 0;
        *(int*)((char*)self + 0x94) = -1;
    }
}
#endif

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

