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

//100%
INCLUDE_ASM("input/inputmap", cInputMap_compileMap);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void func_003E6574(void* dst, void* src, int size);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int cInputMapParser_compileStatement(void* parser, char* src, int* dst, int room, int a4);
extern char D_0048DD20[];

struct sInputMap_0E18 {
    int f0;
    int* offsets;   // 0x4
    void* parser;   // 0x8
    int cap;        // 0xC
    int used;       // 0x10
    int* code;      // 0x14
};

extern "C" void cInputMap_compileMap(sInputMap_0E18* self, int idx, char* src)
{
    if (self->cap - self->used < 0x100) {
        int newCap = self->cap + 0x400;
        int* buf = (int*)operator_new_tag(newCap * 4, D_0048DD20, 0x100, 0);
        if (self->code != 0) {
            func_003E6574(buf, self->code, self->cap * 4);
            if (self->code != 0) {
                cMemMan_free(self->code);
            }
        }
        self->code = buf;
        self->cap = newCap;
    }
    self->offsets[idx] = self->used;
    self->used += cInputMapParser_compileStatement(self->parser, src, self->code + self->used, self->cap - self->used, 5);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_purgeMapFile);
#ifdef SKIP_ASM
// PORT: operator new with tag args bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void operator_delete(int* p);
void cMemMan_free(void* p);
extern "C" void func_003E6574(void* dst, void* src, int size);
extern char D_0048DD30[];

extern "C" void cInputMap_purgeMapFile(void* self)
{
    if (*(int**)((char*)self + 0x8) != 0) {
        operator_delete(*(int**)((char*)self + 0x8));
    }
    *(int**)((char*)self + 0x8) = 0;
    void* p = operator_new_tag(*(int*)((char*)self + 0x10) << 2, D_0048DD30, 0, 0);
    func_003E6574(p, *(void**)((char*)self + 0x14), *(int*)((char*)self + 0x10) << 2);
    if (*(void**)((char*)self + 0x14) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x14));
    }
    *(void**)((char*)self + 0x14) = p;
    *(int*)((char*)self + 0xC) = *(int*)((char*)self + 0x10);
}
#endif

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

//100%
INCLUDE_ASM("input/inputmap", func_00321500);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* func_00321500_va_list;
#define func_00321500_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" int func_004186C8(char* dst, const char* fmt, char* ap);
extern char D_0048DD40[];

extern "C" void func_00321500(void* self, const char* fmt, ...)
{
    char buf[0x400];
    func_00321500_va_list ap;
    func_00321500_va_start(ap);
    func_004186C8(buf + sprintf(buf, D_0048DD40, (char*)self + 0xC, *(int*)((char*)self + 0x8C)), fmt, ap);
    *(int*)((char*)self + 0x94) = 1;
}
#endif

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

//100%
INCLUDE_ASM("input/inputmap", func_003216E8);
#ifdef SKIP_ASM
extern "C" int func_0041AB78(const char* a, const char* b, int n);
extern char D_004A3EF0[];

extern "C" int func_003216E8(void* self, char* name)
{
    if (func_0041AB78(name, D_004A3EF0, 6) != 0) {
        return -1;
    }
    int d = name[6] - '0';
    if ((unsigned char)d >= 10) {
        return -1;
    }
    int n = d;
    char* p = name + 7;
    while ((unsigned)(*p - '0') < 10) {
        n = n * 10 + (*p++ - '0');
    }
    int alpha = 0;
    if ((unsigned)(*p - 'a') < 26 || (unsigned)(*p - 'A') < 26) {
        alpha = 1;
    }
    if (alpha) {
        return -1;
    }
    if (*p != '_' && n < 0x39C) {
        return n;
    }
    return -1;
}
#endif

