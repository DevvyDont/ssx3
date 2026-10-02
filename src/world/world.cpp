#include "common.h"

INCLUDE_ASM("world/world", cWorld_cWorld);

INCLUDE_ASM("world/world", func_003A6740);

extern "C" void cWorld_resetMap(void*);

//100%
INCLUDE_ASM("world/world", func_003A67E0);
#ifdef SKIP_ASM
extern "C" void func_003A67E0(void* self)
{
    cWorld_resetMap(self);
}
#endif

INCLUDE_ASM("world/world", cWorld_resetMap);

extern "C" void* func_003A8290(int);

//100%
INCLUDE_ASM("world/world", func_003A6928__FPv);
#ifdef SKIP_ASM
void* func_003A6928(void* self)
{
    return func_003A8290(*(int*)self);
}
#endif

INCLUDE_ASM("world/world", func_003A6948);

//100%
INCLUDE_ASM("world/world", func_003A6AE0);
#ifdef SKIP_ASM
struct cWorldView;
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void func_003A7F90(void* cache);
extern "C" void cWorldCache_init(void* cache, const char* name, int a2);
int cWorldView_getNumSections(cWorldView* view);
extern "C" void cWScriptCache_init(void* cache, int n);
extern "C" void func_002BB1D0(void* p, int n);

struct func_003A6AE0_sBXString {
    char* str;
};

extern "C" void func_003A6AE0(void** self, const char* name, int a2)
{
    func_003A6AE0_sBXString s;
    cBXString_cBXString2(&s, name);
    func_003A7F90(self[0]);
    cWorldCache_init(self[0], name, a2);
    cWScriptCache_init(self[1], cWorldView_getNumSections((cWorldView*)((char*)self[0] + 0x10)));
    func_002BB1D0(self[2], cWorldView_getNumSections((cWorldView*)((char*)self[0] + 0x10)));
    cBXString__cBXString(&s, 2);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6B78);
#ifdef SKIP_ASM
struct func_003A6B78_sEntry {
    short type;
    char pad2[6];
    void* data;
};

struct func_003A6B78_sTable {
    char pad[4];
    func_003A6B78_sEntry* entries;
};

extern "C" int func_003AD120(func_003A6B78_sEntry* e, void* obj, int i);

extern "C" int func_003A6B78(void* self, void* obj, int i)
{
    return func_003AD120(&(*(func_003A6B78_sTable**)((char*)self + 0x4))->entries[*(unsigned char*)((char*)obj + 0x78)], obj, i);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6BA8);
#ifdef SKIP_ASM
struct func_003A6BA8_sEntry {
    short type;
    char pad2[6];
    void* data;
};

struct func_003A6BA8_sTable {
    char pad[4];
    func_003A6BA8_sEntry* entries;
};

extern "C" int func_003ADC80(func_003A6BA8_sEntry* e);

extern "C" int func_003A6BA8(void* self, int i)
{
    return func_003ADC80(&(*(func_003A6BA8_sTable**)((char*)self + 0x4))->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6BD8);
#ifdef SKIP_ASM
extern "C" int func_003ADCB0(func_003A6BA8_sEntry* e);

extern "C" int func_003A6BD8(void* self, int i)
{
    return func_003ADCB0(&(*(func_003A6BA8_sTable**)((char*)self + 0x4))->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6C08);
#ifdef SKIP_ASM
extern "C" int func_003ADCE0(func_003A6BA8_sEntry* e);

extern "C" int func_003A6C08(void* self, int i)
{
    return func_003ADCE0(&(*(func_003A6BA8_sTable**)((char*)self + 0x4))->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6C38);
#ifdef SKIP_ASM
extern "C" int func_003ADD10(func_003A6BA8_sEntry* e);

extern "C" int func_003A6C38(void* self, int i)
{
    return func_003ADD10(&(*(func_003A6BA8_sTable**)((char*)self + 0x4))->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6C68);
#ifdef SKIP_ASM
extern "C" int func_003ADD40(func_003A6BA8_sEntry* e);

extern "C" int func_003A6C68(void* self, int i)
{
    return func_003ADD40(&(*(func_003A6BA8_sTable**)((char*)self + 0x4))->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6C98);
#ifdef SKIP_ASM
extern "C" int func_003ADD70(func_003A6BA8_sEntry* e);

extern "C" int func_003A6C98(void* self, int i)
{
    return func_003ADD70(&(*(func_003A6BA8_sTable**)((char*)self + 0x4))->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6CC8__FPvT0);
#ifdef SKIP_ASM
int func_003A6CC8(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)*(void**)((char*)a1 + 0x88) + 0xc) + 0x8);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6CD8__FPvT0);
#ifdef SKIP_ASM
int func_003A6CD8(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)*(void**)((char*)a1 + 0x88) + 0xc) + 0x8);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6CE8);
#ifdef SKIP_ASM
extern "C" void* func_003A6CE8(void* self, int a1)
{
    return (char*)(*(void**)self) + a1 * 0x140;
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6D00__FPvi);
#ifdef SKIP_ASM
int func_003A6D00(void* self, int a1)
{
    return *(int*)((char*)*(void**)self + a1 * 4);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6D18);
#ifdef SKIP_ASM
extern "C" int func_003ADC50(func_003A6BA8_sEntry* e, unsigned int i);

extern "C" int func_003A6D18(void* self, unsigned int* handle)
{
    unsigned int h = *handle;
    return func_003ADC50(&(*(func_003A6BA8_sTable**)((char*)self + 0x4))->entries[h & 0xFF], h >> 8);
}
#endif

