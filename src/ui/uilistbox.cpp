#include "common.h"

INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByAsciiString);

INCLUDE_ASM("ui/uilistbox", func_0039A4B0);

INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByStringID);

INCLUDE_ASM("ui/uilistbox", func_0039A670);

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

INCLUDE_ASM("ui/uilistbox", func_0039A8D8);

INCLUDE_ASM("ui/uilistbox", func_0039A928);

INCLUDE_ASM("ui/uilistbox", func_0039AAC0);

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

