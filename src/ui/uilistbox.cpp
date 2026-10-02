#include "common.h"

INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByAsciiString);

INCLUDE_ASM("ui/uilistbox", func_0039A4B0);

INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByStringID);

//100%
INCLUDE_ASM("ui/uilistbox", func_0039A670);
#ifdef SKIP_ASM
void cMemMan_free(void*);

struct func_0039A670_sItem {
    void* data;
    char pad[0x10];
};

extern "C" void func_0039A670(void* self)
{
    if (*(int*)((char*)self + 0x74) & 0x10) {
        if (*(void**)((char*)self + 0x9C) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x9C));
        }
    } else {
        for (int i = 0; i < *(unsigned char*)((char*)self + 0x318); i++) {
            func_0039A670_sItem* it = (func_0039A670_sItem*)((char*)self + 0x9C) + i;
            if (it->data != 0) {
                cMemMan_free(it->data);
            }
        }
    }
    *(unsigned char*)((char*)self + 0x319) = 0;
    *(unsigned char*)((char*)self + 0x318) = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", func_0039A708);
#ifdef SKIP_ASM
extern "C" int func_0039A708(void* self)
{
    if (*(unsigned char*)((char*)self + 0x319) < *(unsigned char*)((char*)self + 0x318)) {
        return *(int*)((char*)self + *(unsigned char*)((char*)self + 0x319) * 0x14 + 0xA0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", func_0039A738);
#ifdef SKIP_ASM
extern "C" int func_0039A738(void* self)
{
    if (*(unsigned char*)((char*)self + 0x319) < *(unsigned char*)((char*)self + 0x318)) {
        return *(int*)((char*)self + *(unsigned char*)((char*)self + 0x319) * 0x14 + 0xA4);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", func_0039A768);
#ifdef SKIP_ASM
extern "C" int func_0039A768(void* self, int key, unsigned char* out)
{
    int i;
    for (i = 0; i < *(unsigned char*)((char*)self + 0x318); i++) {
        if (*(int*)((char*)self + i * 0x14 + 0xA4) == key) {
            *out = i;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", func_0039A7A8);
#ifdef SKIP_ASM
extern "C" void func_0039A7A8(void* self, int a1)
{
    unsigned char v = (unsigned char)a1;
    if (v < *(unsigned char*)((char*)self + 0x318)) {
        *(char*)((char*)self + 0x319) = v;
    }
}
#endif

INCLUDE_ASM("ui/uilistbox", cUIListBox_setEntryByAsciiString);

//100%
INCLUDE_ASM("ui/uilistbox", func_0039A8D8);
#ifdef SKIP_ASM
struct sVEntry39A8D8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0039A8D8(void* self, int count)
{
    *(char*)((char*)self + 0x318) = count;
    *(char*)((char*)self + 0x319) = 0;
    *(int*)((char*)self + 0x74) |= 0x10;
    void* obj = *(void**)(*(char**)((char*)self + 0x5C) + 0xD0);
    sVEntry39A8D8* vt = *(sVEntry39A8D8**)((char*)obj + 8);
    vt[0x13].fn((char*)obj + vt[0x13].delta, self, 9);
}
#endif

INCLUDE_ASM("ui/uilistbox", func_0039A928);

//100%
INCLUDE_ASM("ui/uilistbox", func_0039AAC0);
#ifdef SKIP_ASM
extern "C" void* func_0039FE00(void* self);
extern "C" char func_003979C0(void*);
extern "C" void func_0039AB00(void* self, char a1);

extern "C" void func_0039AAC0(void* self)
{
    func_0039FE00(self);
    char v = func_003979C0((char*)self + 0x74);
    *(char*)((char*)self + 0x97) = v;
    *(char*)((char*)self + 0x96) = v;
    func_0039AB00(self, 0);
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", func_0039AB00);
#ifdef SKIP_ASM
extern "C" void func_0039AB00(void* self, char a1)
{
    if (a1 == 0) {
        *(char*)((char*)self + 0x91) = 0;
        *(char*)((char*)self + 0x92) = 1;
        *(char*)((char*)self + 0x93) = 2;
        *(char*)((char*)self + 0x94) = 3;
    } else {
        *(char*)((char*)self + 0x91) = 4;
        *(char*)((char*)self + 0x92) = 5;
        *(char*)((char*)self + 0x93) = 6;
        *(char*)((char*)self + 0x94) = 7;
    }
}
#endif

INCLUDE_ASM("ui/uilistbox", func_0039AB50);

INCLUDE_ASM("ui/uilistbox", func_0039AC48);

