#include "common.h"

//100%
INCLUDE_ASM("dirtysock/tagsunk", cDirtysock_tag_TagFieldSetUnk);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldSetString(char* buf, int size, const char* key, const char* value);

// PORT: the unit declares value as int (callers pass a string pointer in it).
extern "C" int cDirtysock_tag_TagFieldSetUnk(char* buf, int size, const char* key, int value)
{
    char temp[256];
    int r;
    if (*(const char*)value == '\'') {
        temp[0] = '`';
        strcpy(temp + 1, (const char*)value);
        r = cDirtysock_tag_TagFieldSetString(buf, size, key, temp);
    } else {
        r = cDirtysock_tag_TagFieldSetString(buf, size, key, (const char*)value);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cDirtysock_tag_TagFieldGetUnk);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int cDirtysock_tag_TagFieldGetString(const char* tag, char* buf, int size);

// PORT: variable-length array (GNU extension in C++).
extern "C" void cDirtysock_tag_TagFieldGetUnk(const char* tag, char* dst, int size)
{
    char buf[size + 1];
    cDirtysock_tag_TagFieldGetString(tag, buf, size + 1);
    if (buf[0] == '`')
        strcpy(dst, buf + 1);
    else
        strcpy(dst, buf);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259390__FPv);
#ifdef SKIP_ASM
void func_00259390(void* self)
{
}
#endif

extern "C" void* func_0041AA88(void*, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259398__FPviT0T0);
#ifdef SKIP_ASM
void* func_00259398(void* self, int a1, void* a2, void* a3)
{
    return func_0041AA88((char*)a2 + 0x1c, (char*)a3 + 0x1c);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002593B8__FPviT0T0);
#ifdef SKIP_ASM
void* func_002593B8(void* self, int a1, void* a2, void* a3)
{
    return func_0041AA88((char*)a2 + 0x8, (char*)a3 + 0x8);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002594D0);

INCLUDE_ASM("dirtysock/tagsunk", func_00259628);

extern "C" void* func_003E8FD0(int);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002598A8__FPv);
#ifdef SKIP_ASM
void* func_002598A8(void* self)
{
    return func_003E8FD0(*(int*)((char*)self + 0x54));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002598C8);

INCLUDE_ASM("dirtysock/tagsunk", func_002599B0);

INCLUDE_ASM("dirtysock/tagsunk", func_00259A60);

INCLUDE_ASM("dirtysock/tagsunk", func_00259B10);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A1D0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A650);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A6F8);
#ifdef SKIP_ASM
extern "C" void func_003E6E70(void*);

extern "C" void func_0025A6F8(void* self)
{
    void* p = *(void**)((char*)self + 0xF4);
    if (p != 0) {
        func_003E6E70(p);
        *(void**)((char*)self + 0xF4) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A730);
#ifdef SKIP_ASM
extern char D_004813A0[];
extern "C" int func_003E7218(void* tags, const char* key, const char* def);

extern "C" int func_0025A730(void* self, const char* key, int def)
{
    void* tags = *(void**)((char*)self + 0xF4);
    if (tags != 0) {
        int r = func_003E7218(tags, D_004813A0, key);
        if (r != 0) {
            def = r;
        }
    }
    return def;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025A778);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A850);
#ifdef SKIP_ASM
extern "C" void func_003E8688(void* ctx, int kind, void* p);

extern "C" void func_0025A850(void* self)
{
    int i;
    void** arr = (void**)((char*)self + 0x1EC);
    for (i = 0; i < 29; i++) {
        if (arr[i] != 0) {
            func_003E8688(*(void**)((char*)self + 0x54), 4, arr[i]);
            arr[i] = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025A8B0);
#ifdef SKIP_ASM
struct sTagsUnkCallbackObj {
    char pad[0xC];
    void (*fn)(sTagsUnkCallbackObj* self);
};

extern "C" void func_0025A8B0(void* self)
{
    if (*(void**)((char*)self + 0x5C) != 0) {
        func_003E8688(*(void**)((char*)self + 0x54), 0, *(void**)((char*)self + 0x5C));
        *(void**)((char*)self + 0x5C) = 0;
    }
    if (*(void**)((char*)self + 0x60) != 0) {
        func_003E8688(*(void**)((char*)self + 0x54), 1, *(void**)((char*)self + 0x60));
        *(void**)((char*)self + 0x60) = 0;
    }
    if (*(void**)((char*)self + 0x64) != 0) {
        func_003E8688(*(void**)((char*)self + 0x54), 2, *(void**)((char*)self + 0x64));
        *(void**)((char*)self + 0x64) = 0;
    }
    func_0025A850(self);
    sTagsUnkCallbackObj* obj = *(sTagsUnkCallbackObj**)((char*)self + 0x58);
    if (obj != 0) {
        obj->fn(obj);
        *(void**)((char*)self + 0x58) = 0;
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025A948);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025AA98);
#ifdef SKIP_ASM
extern "C" void func_0025A8B0(void* self);
extern "C" void func_003E80C0(void* p);
extern "C" void func_003E81C8(void* p);

extern "C" void func_0025AA98(void* self)
{
    func_0025A8B0(self);
    if (*(void**)((char*)self + 0x54) != 0) {
        func_003E80C0(*(void**)((char*)self + 0x54));
        func_003E81C8(*(void**)((char*)self + 0x54));
        *(void**)((char*)self + 0x54) = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025AAE0);
#ifdef SKIP_ASM
extern char D_004A3010[];
extern "C" int cDirtysock_tag_TagFieldSetUnk(char* buf, int size, const char* key, int value);
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);

extern "C" void func_0025AAE0(void* self, int value)
{
    char buf[0x200];
    if (*(int*)self != 0) {
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetUnk(buf, 0x200, D_004A3010, value);
        func_003E8B10(*(void**)((char*)self + 0x54), 0x6D657367, buf, 0, 0);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025AB40);

INCLUDE_ASM("dirtysock/tagsunk", func_0025AC50);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025B608);
#ifdef SKIP_ASM
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);

extern "C" void func_0025B608(void* self, int a1, int tag, void* cb, int a4, float f)
{
    *(int*)self = a1;
    *(float*)((char*)self + 0xE0) = f;
    *(int*)((char*)self + 0x40) = func_003E8B10(*(void**)((char*)self + 0x54), tag, cb, a4, self);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025B650);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B688);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B6D8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025B800);
#ifdef SKIP_ASM
extern "C" void func_0025A6F8(void* self);
extern "C" void func_0025AA98(void* self);

extern "C" void func_0025B800(void* self)
{
    func_0025A6F8(self);
    if (*(void**)((char*)self + 0x54) != 0) {
        func_0025AA98(self);
    }
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x0) = 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025B848);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B9C8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025BC48);

INCLUDE_ASM("dirtysock/tagsunk", func_0025BD30);

INCLUDE_ASM("dirtysock/tagsunk", func_0025BF18);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C0C0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C1E0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C2D8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C3F8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C4F0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C610);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C8A8);

extern "C" void* func_0025CD50(void*, int);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025CD28__FPvi);
#ifdef SKIP_ASM
void* func_0025CD28(void* self, int a1)
{
    return func_0025CD50(self, *(int*)((char*)((char*)self + a1 * 4) + 0x70));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025CD50);

INCLUDE_ASM("dirtysock/tagsunk", func_0025CEB0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025CFE0);
#ifdef SKIP_ASM
extern char D_004A3108[];
extern "C" int cDirtysock_tag_TagFieldSetString(char* buf, int size, const char* key, const char* value);
extern "C" void func_0025B608(void* self, int a1, int tag, void* cb, int a4, float f);
extern "C" void func_00268CC0(void* self, void* a1, int a2);

extern "C" void func_0025CFE0(void* self)
{
    char buf[0x200];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, D_004A3108, *(const char**)((char*)self + 0x80));
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 0x1B, 0x6F6E6C6E, buf, (int)func_00268CC0, 20.0f);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025D048);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D1B8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D2D8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D428);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D538);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D6C0);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025D770);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern char D_004A2F58[];
extern "C" void* func_0025F150(void* self);

extern "C" void func_0025D770(void* self, int idx)
{
    char buf[256];
    if (*(int*)self != 0) {
        const char* key = D_004A2F58;
        const char* name;
        buf[0] = 0;
        if (idx != -1) {
            name = (const char*)func_0025F150(self);
        } else {
            name = D_004A2F40;
        }
        cDirtysock_tag_TagFieldSetString(buf, 0x100, key, name);
        func_003E8B10(*(void**)((char*)self + 0x54), 0x7065656B, buf, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025D800);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern "C" void* func_0025F150(void* self);
extern "C" void func_0025D860(void* self, void* name, int a2);

extern "C" void func_0025D800(void* self, int idx, int a2)
{
    void* name;
    if (idx != -1) {
        name = func_0025F150(self);
    } else {
        name = D_004A2F40;
    }
    func_0025D860(self, name, a2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025D860);
#ifdef SKIP_ASM
extern char D_004A2F58[];
extern char D_004A3040[];
extern "C" void func_00268D38(void* self, void* a1, int a2);

// PORT: the unit declares a2 as int; it carries an optional string pointer.
extern "C" void func_0025D860(void* self, void* name, int a2)
{
    if (*(int*)self != 0) {
        char buf[0x100];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A2F58, (const char*)name);
        if (a2 != 0) {
            cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A3040, (const char*)a2);
        }
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 0xC, 0x6D6F7665, buf, (int)func_00268D38, 20.0f);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025D8F8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025DA90);
#ifdef SKIP_ASM
extern "C" char* func_0025DA90(void* self)
{
    char* s;
    if (*(int*)((char*)self + 0x54) == 0) {
        return 0;
    }
    s = *(char**)((char*)self + 0x84);
    if (s != 0) {
        if (*s != 0) {
            return s;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025DAC0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025DB80);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025DD08);
#ifdef SKIP_ASM
extern "C" int func_0025A730(void* self, const char* key, int def);
extern "C" void func_00268D88(void* self, void* a1, int a2);
extern "C" int func_003E8DC0(void* p, const char* key, int value, int timeout, void* cb, void* user);

extern "C" void func_0025DD08(void* self, const char* key, int def)
{
    if (*(int*)self != 0) {
        func_003E8DC0(*(void**)((char*)self + 0x54), key, func_0025A730(self, key, def), 0x1388, (void*)func_00268D88, self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025DDE0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025DF00);
#ifdef SKIP_ASM
extern char D_004A3108[];
extern char D_004A2FE0[];
extern "C" const char* func_00259198();
extern "C" void func_00268DB0(void* self, void* a1, int a2);

extern "C" void func_0025DF00(void* self, const char* name)
{
    if (*(int*)self != 0) {
        char buf[0x100];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A3108, name);
        const char* key = D_004A2FE0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, key, func_00259198());
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 0x10, 0x72657074, buf, (int)func_00268DB0, 20.0f);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025DF98);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025E0C8);
#ifdef SKIP_ASM
extern char D_004A2F58[];
extern "C" int cDirtysock_tag_TagFieldSetNumber(char* buf, int size, const char* key, int value);
extern "C" void func_0025B608(void* self, int a1, int tag, void* cb, int a4, float f);
extern "C" void func_00268DD8(void* self, void* a1, int a2);

extern "C" void func_0025E0C8(void* self, int value)
{
    if (*(int*)self != 0) {
        char buf[0x100];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetNumber(buf, 0x100, D_004A2F58, value);
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 0xE, 0x6E657773, buf, (int)func_00268DD8, 20.0f);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025E138);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E348);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E420);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E590);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E6D0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E7F0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025EE20);

INCLUDE_ASM("dirtysock/tagsunk", func_0025EED8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F030);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F120);
#ifdef SKIP_ASM
extern "C" int func_003EE558(void*);

extern "C" int func_0025F120(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return func_003EE558(*(void**)((char*)self + 0x5C));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F150);
#ifdef SKIP_ASM
extern "C" void* func_003EE560(void*);

extern "C" void* func_0025F150(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return (char*)func_003EE560(*(void**)((char*)self + 0x5C)) + 0x1C;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F180);
#ifdef SKIP_ASM
extern "C" void* func_003EE560(void*);

static inline short getFlags(void* p)
{
    return *(short*)((char*)p + 0xA);
}

extern "C" int func_0025F180(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return getFlags(func_003EE560(*(void**)((char*)self + 0x5C))) & 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F1B0);
#ifdef SKIP_ASM
extern "C" int func_003EE558(void*);

extern "C" int func_0025F1B0(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return func_003EE558(*(void**)((char*)self + 0x60));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F1E0);
#ifdef SKIP_ASM
extern char D_004A2ED0[];
// PORT: the unit declares func_003EE560 with one parameter; this caller passes (list, index).
void* func_003EE560_2(void* list, int idx) __asm__("func_003EE560");

extern "C" char* func_0025F1E0(void* self, int idx, int* outFlag)
{
    if (*(int*)self == 0) {
        return 0;
    }
    char* p = (char*)func_003EE560_2(*(void**)((char*)self + 0x60), idx);
    if (p != 0) {
        if (outFlag != 0) {
            *outFlag = (*(int*)(p + 0x4) >> 21) & 1;
        }
        return p + 8;
    }
    return D_004A2ED0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F248);
#ifdef SKIP_ASM
extern "C" int func_0025F248(void* self)
{
    void* p;
    if (*(int*)self == 0) {
        return 0;
    }
    p = func_003EE560(*(void**)((char*)self + 0x60));
    if (p != 0) {
        return (*(int*)((char*)p + 0x4) >> 21) & 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025F288);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F338);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F388);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F430);
#ifdef SKIP_ASM
extern "C" char* func_003E8A70(void* p, int tag);
extern "C" bool func_0025F430(void* self)
{
    char* s;
    if (*(int*)self == 0) {
        return false;
    }
    s = func_003E8A70(*(void**)((char*)self + 0x54), 0x726F6F6D);
    return s != 0 && *s != 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025F4C0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F628);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F740);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F858);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);

extern "C" void func_0025F858(void* self)
{
    func_003E8B10(*(void**)((char*)self + 0x54), 0x6368616C, D_004A2F40, 0, 0);
    *(int*)self = 1;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025F8A0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025FA48);
#ifdef SKIP_ASM
extern char D_004A3108[];
extern char D_004A2F68[];
extern char D_004A3240[];
extern "C" void func_00268EA0(void* self, void* a1, int a2);

extern "C" void func_0025FA48(void* self, const char* name)
{
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A3108, name);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A2F68, D_004A3240);
    *(int*)self = 1;
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 0x15, 0x6368616C, buf, (int)func_00268EA0, 20.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025FAD0);
#ifdef SKIP_ASM
extern char D_004A3108[];
extern char D_004A2F68[];
extern char D_004A3250[];
extern "C" void func_00268EA0(void* self, void* a1, int a2);

extern "C" void func_0025FAD0(void* self, const char* name)
{
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A3108, name);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A2F68, D_004A3250);
    *(int*)self = 1;
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 0x18, 0x6368616C, buf, (int)func_00268EA0, 20.0f);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FB58);
#ifdef SKIP_ASM
extern "C" void func_0025FC70(void* self, int a1, int a2);

extern "C" void func_0025FB58(void* self, int a1, int a2)
{
    if (*(int*)self != 0) {
        *(int*)((char*)self + 0xA4) = a2;
        func_0025FC70(self, a1, 0x17);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FB80);
#ifdef SKIP_ASM
extern char D_004A3258[];
extern char D_004A3010[];
extern char D_004A3260[];
extern "C" int cDirtysock_tag_TagFieldSetFlags(char* record, int len, const char* name, int value);
extern "C" void func_00268EC8(void* self, void* a1, int a2);

// PORT: a1 carries a string pointer in an int (the unit's declaration).
extern "C" void* func_0025FB80(void* self, int a1, void* a2)
{
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A3258, (const char*)a1);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, D_004A3010, (const char*)a2);
    cDirtysock_tag_TagFieldSetFlags(buf, 0x100, D_004A3260, 0x40004000);
    // PORT: function pointer passed through func_003E8B10's int parameter.
    return (void*)func_003E8B10(*(void**)((char*)self + 0x54), 0x6D657367, buf, (int)func_00268EC8, self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC20__FPv);
#ifdef SKIP_ASM
void func_0025FC20(void* self)
{
}
#endif

extern void* D_004A3270[];
extern "C" void* func_0025FB80(void*, int, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC28__FPvi);
#ifdef SKIP_ASM
void* func_0025FC28(void* self, int a1)
{
    return func_0025FB80(self, a1, (void*)D_004A3270);
}
#endif

extern void* D_004807F8[];
extern "C" void* func_0025FB80(void*, int, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC48__FPv);
#ifdef SKIP_ASM
void* func_0025FC48(void* self)
{
    return func_0025FB80(self, *(int*)((char*)self + 0x98), (void*)D_004807F8);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025FC70);

INCLUDE_ASM("dirtysock/tagsunk", func_0025FD50);

INCLUDE_ASM("dirtysock/tagsunk", func_00260C68);

INCLUDE_ASM("dirtysock/tagsunk", func_00260E50);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00260F10);
#ifdef SKIP_ASM
extern "C" void cBXString_Reset(void* self);

extern "C" void func_00260F10(void* self)
{
    cBXString_Reset((char*)self + 0x260);
    cBXString_Reset((char*)self + 0x264);
    cBXString_Reset((char*)self + 0x268);
    cBXString_Reset((char*)self + 0x26C);
    cBXString_Reset((char*)self + 0x270);
    *(int*)((char*)self + 0x298) = 1;
    *(int*)((char*)self + 0x29C) = 1;
    *(int*)((char*)self + 0x288) = 1;
    *(int*)((char*)self + 0x294) = 0;
    *(int*)((char*)self + 0x28C) = 1;
    *(int*)((char*)self + 0x290) = 0x7C1;
    *(int*)((char*)self + 0x2A0) = 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00260F80);

INCLUDE_ASM("dirtysock/tagsunk", func_00261008);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261058__FPv);
#ifdef SKIP_ASM
void func_00261058(void* self)
{
    *(char*)self = 0;
    *(int*)((char*)self + 0x40) = -1;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x64) = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261070);
#ifdef SKIP_ASM
struct sVE261070 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj261070 {
    char pad[0xC];
    sVE261070* vt;
};

struct sVec261070 {
    char pad[0xAC];
    sObj261070** begin;
    sObj261070** end;
};

extern "C" void func_00261070(sVec261070* self)
{
    while (self->begin != self->end) {
        sObj261070* p = self->end[-1];
        self->end--;
        if (p != 0) {
            p->vt[1].fn((char*)p + p->vt[1].delta, 3);
        }
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002610D0);

INCLUDE_ASM("dirtysock/tagsunk", func_00261248);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002613C8);
#ifdef SKIP_ASM
extern "C" void func_00261248(void* self, void* self2, int idx, void* a, void* b, int c);
extern "C" void func_002613C8(void* self)
{
    int idx = *(int*)((char*)self + 0x40);
    if (idx >= 0) {
        bool ok = !*(bool*)((char*)self + 0xD8);
        if (ok) {
            func_00261248(self, self, idx, (char*)self + 0x44, (char*)self + 0x64, *(int*)((char*)self + 0xEC));
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261408);
#ifdef SKIP_ASM
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);
extern "C" void func_00261530(void* self);

extern "C" void func_00261408(void* self)
{
    func_00262768(self, 0, 1, 0, 0, 0);
    func_00261530(self);
    *(int*)((char*)self + 0xD8) = 0;
    *(int*)((char*)self + 0xD0) = 0;
    *(int*)((char*)self + 0xDC) = 0;
    *(int*)((char*)self + 0xE0) = 0;
    *(int*)((char*)self + 0xE8) = 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00261460);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261490);
#ifdef SKIP_ASM
extern "C" void func_003F17D8(void* p);
extern "C" void func_00261408(void* self);

extern "C" void func_00261490(void* self)
{
    void* p = *(void**)((char*)self + 0xC8);
    if (p != 0) {
        func_003F17D8(p);
        if (*(int*)((char*)self + 0xDC) != 0) {
            func_00261408(self);
            *(int*)((char*)self + 0xDC) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002614E8);
#ifdef SKIP_ASM
extern char D_00480E58[];
extern "C" void func_00268F40(void* self);
extern "C" void* func_003F0710(void* a, const char* name);
extern "C" void func_003F17C8(void* h, void (*cb)(void*), void* user);

extern "C" void func_002614E8(void* self, void* a1)
{
    void* h = func_003F0710(a1, D_00480E58);
    *(void**)((char*)self + 0xC8) = h;
    func_003F17C8(h, func_00268F40, self);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00261530);

INCLUDE_ASM("dirtysock/tagsunk", func_002615C8);

INCLUDE_ASM("dirtysock/tagsunk", func_00261770);

INCLUDE_ASM("dirtysock/tagsunk", func_002618C0);

INCLUDE_ASM("dirtysock/tagsunk", func_00261970);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261A20__FPv);
#ifdef SKIP_ASM
void func_00261A20(void* self)
{
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00261A28);

INCLUDE_ASM("dirtysock/tagsunk", func_00261CD0);

INCLUDE_ASM("dirtysock/tagsunk", func_00261EF8);

INCLUDE_ASM("dirtysock/tagsunk", func_002620D8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262280);
#ifdef SKIP_ASM
extern "C" int func_00262400(void* self, int a1);
extern "C" int func_00262598(void* self);

extern "C" int func_00262280(void* self)
{
    int n = 0;
    for (int i = 0; i < func_00262598(self); i++) {
        if (func_00262400(self, i) == 0)
            n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002622F0);
#ifdef SKIP_ASM
extern "C" int func_00262400(void* self, int a1);
extern "C" int func_00262598(void* self);

extern "C" int func_002622F0(void* self)
{
    int n = 0;
    for (int i = 0; i < func_00262598(self); i++) {
        if (func_00262400(self, i) != 0)
            n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262360);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
// PORT: the unit declares func_00262390 as `int (void*)`; this caller passes a
// second argument (it reaches func_00262620's callee in $a1). Bound by asm label.
int func_00262390_2(void* self, int a1) __asm__("func_00262390__FPv");

extern "C" int func_00262360(void* self, int a1)
{
    return func_00262390_2(self, func_00262468(self, a1));
}
#endif

extern "C" void* func_00262620(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262390__FPv);
#ifdef SKIP_ASM
int func_00262390(void* self)
{
    return (func_00262620(self) != 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002623B0);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
// PORT: the unit declares func_002623E0 as `int (void*)`; this caller passes a
// second argument (it reaches func_00262658's callee in $a1). Bound by asm label.
int func_002623E0_2(void* self, int a1) __asm__("func_002623E0__FPv");

extern "C" int func_002623B0(void* self, int a1)
{
    return func_002623E0_2(self, func_00262468(self, a1));
}
#endif

extern "C" void* func_00262658(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002623E0__FPv);
#ifdef SKIP_ASM
int func_002623E0(void* self)
{
    return (func_00262658(self) != 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262400);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(void* self, int idx);

extern "C" int func_00262400(void* self, int a1)
{
    void* p = func_002625C8(self, a1);
    if (p != 0) {
        return (*(int*)((char*)p + 0xA8) >> 20) & 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262438);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_00262400(void* self, int a1);

extern "C" int func_00262438(void* self, int a1)
{
    return func_00262400(self, func_00262468(self, a1));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262468);
#ifdef SKIP_ASM
extern "C" int func_004165A8(void*, void*);
extern "C" void* func_002625C8(void* self, int idx);

// PORT: a1 carries a name pointer in an int (the unit's declaration).
extern "C" int func_00262468(void* self, int a1)
{
    int i;
    int n = func_00262598(self);
    for (i = 0; i < n; i++) {
        void* e = func_002625C8(self, i);
        if (e != 0 && func_004165A8(e, (void*)a1) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002624F8);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(void* self, int idx);

extern "C" int func_002624F8(void* self, void** key)
{
    int i;
    int n;
    if (key == 0) {
        return -1;
    }
    n = func_00262598(self);
    for (i = 0; i < n; i++) {
        void* e = func_002625C8(self, i);
        if (e != 0 && func_0041AA88(*key, e) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262598);
#ifdef SKIP_ASM
extern "C" int func_003EE550(void*);

extern "C" int func_00262598(void* self)
{
    void* p = *(void**)((char*)self + 0xCC);
    if (p == 0) {
        return 0;
    }
    return func_003EE550(p);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002625C8);
#ifdef SKIP_ASM
extern "C" int func_00262598(void* self);
// PORT: the unit declares func_003EE560 with one parameter; this caller passes (list, index).
void* func_003EE560_2(void* list, int idx) __asm__("func_003EE560");

extern "C" void* func_002625C8(void* self, int idx)
{
    void* r;
    if (idx < 0 || func_00262598(self) < idx) {
        r = 0;
    } else {
        r = func_003EE560_2(*(void**)((char*)self + 0xCC), idx);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262620);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(void* self, int idx);
// PORT: the unit declares func_00262620 as `void* (void*)`; the body passes a second
// argument through to func_002625C8 in $a1. Bound by asm label.
void* func_00262620_2(void* self, int idx) __asm__("func_00262620");

void* func_00262620_2(void* self, int idx)
{
    void* p = func_002625C8(self, idx);
    if ((p != 0) && (*(int*)((char*)p + 0xA8) & 0x4)) {
        return p;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262658);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(void* self, int idx);
// PORT: the unit declares func_00262658 as `void* (void*)`; the body passes a second
// argument through to func_002625C8 in $a1. Bound by asm label.
void* func_00262658_2(void* self, int idx) __asm__("func_00262658");

void* func_00262658_2(void* self, int idx)
{
    void* p = func_002625C8(self, idx);
    if ((p != 0) && (*(int*)((char*)p + 0xA8) & 0x200)) {
        return p;
    }
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262690);

INCLUDE_ASM("dirtysock/tagsunk", func_00262768);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002628C0);
#ifdef SKIP_ASM
extern "C" int func_002628C0(void* self, int sel)
{
    if (*(int*)((char*)self + 0xC8) == 0) {
        return -1;
    }
    switch (sel) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262938);
#ifdef SKIP_ASM
extern "C" int func_00262938(void* self)
{
    return (*(int*)((char*)self + 0xb0) - *(int*)((char*)self + 0xac)) >> 2;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262950);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00262A08);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_00262950(void* self, int a1);

extern "C" int func_00262A08(void* self, int a1)
{
    return func_00262950(self, func_00262468(self, a1));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262A38);
#ifdef SKIP_ASM
extern "C" int func_004165A8(void*, void*);

extern "C" int func_00262A38(void* self, void* name)
{
    for (void*** it = *(void****)((char*)self + 0xAC); it != *(void****)((char*)self + 0xB0); it++) {
        if (func_004165A8(**it, name) == 0)
            return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262AF0);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262E20);
#ifdef SKIP_ASM
static inline unsigned int func_00262E20_count(void* self)
{
    return *(void***)((char*)self + 0xC0) - *(void***)((char*)self + 0xBC);
}

static inline void** func_00262E20_at(void* self, unsigned int i)
{
    return *(void***)((char*)*(void***)((char*)self + 0xBC) + (i << 2));
}

extern "C" int func_00262E20(void* self, void* name)
{
    for (unsigned int i = 0; i < func_00262E20_count(self); i++) {
        void** e = func_00262E20_at(self, i);
        if (e != 0 && func_0041AA88(*e, name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262EC0);

INCLUDE_ASM("dirtysock/tagsunk", func_00263018);

INCLUDE_ASM("dirtysock/tagsunk", func_00263128);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263270);
#ifdef SKIP_ASM
extern "C" void* func_00263270(void* self, void* name)
{
    if ((unsigned int)(*(void***)((char*)self + 0xC0) - *(void***)((char*)self + 0xBC)) == 0) {
        return 0;
    }
    for (void** it = *(void***)((char*)self + 0xBC); it != *(void***)((char*)self + 0xC0); it++) {
        void* e = *it;
        if (func_004165A8(*(void**)e, name) == 0) {
            return e;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263308);
#ifdef SKIP_ASM
extern "C" void* func_00263308(void* self, unsigned int i)
{
    void** begin = *(void***)((char*)self + 0xBC);
    void** end = *(void***)((char*)self + 0xC0);
    if (i >= (unsigned int)(end - begin)) {
        return 0;
    }
    return *(void**)((char*)begin + (i << 2));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263338);
#ifdef SKIP_ASM
// PORT: the target calls func_00262598 without setting $4 (self is still there from entry);
// bound here with a no-arg prototype.
extern "C" int func_00262598_noarg() __asm__("func_00262598");

extern "C" int func_00263338(void* self, int idx)
{
    if (idx < 0) {
        return 0;
    }
    if (func_00262598_noarg() < idx) {
        return 0;
    }
    char* e = (char*)func_002625C8(self, idx);
    if (e == 0) {
        return 0;
    }
    int r = 0;
    if ((*(unsigned char*)(e + 0xA7) & 1) || (*(unsigned int*)(e + 0xA8) & 0x100000)) {
        r = 1;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_002633C8);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_00263338(void* self, int a1);

extern "C" int func_002633C8(void* self, int a1)
{
    return func_00263338(self, func_00262468(self, a1));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002633F8);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern char D_004A31F8[];
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
// PORT: the target calls func_00262598 without setting $4 (self is still there from entry);
// bound here with a no-arg prototype.
extern "C" int func_00262598_noarg() __asm__("func_00262598");
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");

extern "C" int func_002633F8(void* self, int idx)
{
    if (idx < 0) {
        return 0;
    }
    if (func_00262598_noarg() < idx) {
        return 0;
    }
    char* e = (char*)func_002625C8(self, idx);
    if (e == 0) {
        return 0;
    }
    char buf[16];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(e + 0x54, D_004A31F8), buf, 0x10, D_004A2F40);
    return func_004165A8(*(void**)((char*)self + 0xEC), buf) == 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00263490);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_002633F8(void* self, int a1);

extern "C" int func_00263490(void* self, int a1)
{
    return func_002633F8(self, func_00262468(self, a1));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002634C0);
#ifdef SKIP_ASM
extern "C" int func_002634C0(void* self, int a1, void* a, void* b)
{
    if (a == 0) {
        return b != 0;
    }
    if (b == 0) {
        return -1;
    }
    // PORT: func_0041AA88 (strcmp-like) is declared void* in this unit but returns int
    return (int)func_0041AA88(a, b);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002635A8);
#ifdef SKIP_ASM
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);

struct sTagsUnkVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sTagsUnkObjVec {
    void* alloc;
    void** mStart;
    void** mFinish;
};

static inline void** sTagsUnkObjVec_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

extern "C" void func_002635A8(void* self, int idx)
{
    sTagsUnkObjVec* v = (sTagsUnkObjVec*)((char*)self + 0xA8);
    void** pos = *(void***)((char*)self + 0xAC) + idx;
    char* obj = (char*)*pos;
    if (pos + 1 != *(void***)((char*)self + 0xB0)) {
        sTagsUnkObjVec_copy(pos + 1, v->mFinish, pos);
    }
    --*(void***)((char*)self + 0xB0);
    if (obj != 0) {
        sTagsUnkVEntry* vt = *(sTagsUnkVEntry**)(obj + 0xC);
        vt[1].fn(obj + vt[1].delta, 3);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263630);
#ifdef SKIP_ASM
extern "C" void* func_00263630(void* self, unsigned int i)
{
    void** begin = *(void***)((char*)self + 0xAC);
    void** end = *(void***)((char*)self + 0xB0);
    if (i >= (unsigned int)(end - begin)) {
        return 0;
    }
    return *(void**)((char*)begin + (i << 2));
}
#endif

extern void* D_004810C8[];

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263660__FPv);
#ifdef SKIP_ASM
void* func_00263660(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x8) = (int)(void*)D_004810C8;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263678);
#ifdef SKIP_ASM
extern void* D_004810C8[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00263678(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004810C8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002636A8);

INCLUDE_ASM("dirtysock/tagsunk", func_002636F0);

INCLUDE_ASM("dirtysock/tagsunk", func_00263828);

extern void* D_004810B0[];

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263998__FPv);
#ifdef SKIP_ASM
void* func_00263998(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x14) = (int)(void*)D_004810B0;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002639B0);
#ifdef SKIP_ASM
extern void* D_004810B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_002639B0(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = D_004810B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002639E0);

INCLUDE_ASM("dirtysock/tagsunk", func_00263BA8);

INCLUDE_ASM("dirtysock/tagsunk", func_00263CF0);

INCLUDE_ASM("dirtysock/tagsunk", func_00264030);

INCLUDE_ASM("dirtysock/tagsunk", func_00264098);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002640C0);
#ifdef SKIP_ASM
extern "C" void* func_003E6448(void* dst, int value, int size);

extern "C" void* func_002640C0(void* self, int a1, int a2, int a3)
{
    *(int*)((char*)self + 0x0) = 1;
    *(int*)((char*)self + 0x120) = a1;
    *(int*)((char*)self + 0x124) = a2;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x118) = 0;
    *(int*)((char*)self + 0x11C) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x128) = a3;
    func_003E6448((char*)self + 0x10, 0, 0x80);
    *(int*)((char*)self + 0x90) = 0;
    *(int*)((char*)self + 0x94) = 0;
    func_003E6448((char*)self + 0x98, 0, 0x80);
    return self;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00264138);

INCLUDE_ASM("dirtysock/tagsunk", func_002642B8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264960);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_004805A8[];

extern "C" void* func_00264960(void)
{
    return cMemMan_alloc(0xF0, D_004805A8, 0x20000000, 0);
}
#endif

void operator_delete(int*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264990);
#ifdef SKIP_ASM
extern "C" void func_00264990(void* self, int* p)
{
    operator_delete(p);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002649B0);

INCLUDE_ASM("dirtysock/tagsunk", func_00264A58);

INCLUDE_ASM("dirtysock/tagsunk", func_00264B20);

INCLUDE_ASM("dirtysock/tagsunk", func_00264BE8);

INCLUDE_ASM("dirtysock/tagsunk", func_00264CB8);

INCLUDE_ASM("dirtysock/tagsunk", func_00264CF0);

INCLUDE_ASM("dirtysock/tagsunk", func_00264D80);

INCLUDE_ASM("dirtysock/tagsunk", func_00264E50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00264F88);
#ifdef SKIP_ASM
extern "C" void func_00264CF0(void* self, int a1);
extern "C" void func_00264E50(void* self, int event, int a2, int state);

extern "C" void func_00264F88(void* self, int a1)
{
    int state = *(int*)self;
    if (state == 0xC) {
        return;
    }
    if (*(int*)((char*)self + 0x124) != 0) {
        if (state == 1) {
            *(int*)self = 2;
        }
        func_00264CF0(self, a1);
        func_00264E50(self, 0, a1, *(int*)self);
    } else {
        func_00264E50(self, 1, a1, 0);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265018);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00265140);
#ifdef SKIP_ASM
extern "C" void func_00264E50(void* self, int event, int a2, int state);

extern "C" void func_00265140(int* self)
{
    int event;
    if (*self != 0xC) {
        if (*(int*)((char*)self + 0x124) != 0) {
            if (*self == 4) {
                *self = 2;
            }
            event = 4;
        } else {
            event = 5;
        }
        func_00264E50(self, event, -1, *self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265190);

INCLUDE_ASM("dirtysock/tagsunk", func_00265290);

INCLUDE_ASM("dirtysock/tagsunk", func_00265398);

INCLUDE_ASM("dirtysock/tagsunk", func_00265458);

INCLUDE_ASM("dirtysock/tagsunk", func_00265568);

INCLUDE_ASM("dirtysock/tagsunk", func_00265688);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265700);
#ifdef SKIP_ASM
struct sTagMsg265700 {
    int type;          // 0x0
    int state;         // 0x4
    unsigned int id;   // 0x8
    int pad[2];
};

extern "C" void func_00264BE8(void* self, void* msg, int size);

extern "C" void func_00265700(void* self)
{
    sTagMsg265700 msg;
    int type;
    int state = *(int*)self;
    if (state == 12)
        return;
    if (*(int*)((char*)self + 0x124) != 0) {
        if (state == 9)
            *(int*)self = 7;
        type = 12;
    } else {
        type = 13;
    }
    msg.type = type;
    msg.state = *(int*)self;
    msg.id = 0xFFFFFFFF;
    func_00264BE8(self, &msg, sizeof(msg));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265768);

INCLUDE_ASM("dirtysock/tagsunk", func_00265880);

INCLUDE_ASM("dirtysock/tagsunk", func_002658B8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002658E0);
#ifdef SKIP_ASM
extern "C" void func_00265950(void* self);

extern "C" void* func_002658E0(void* self)
{
    func_00265950(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265908);
#ifdef SKIP_ASM
extern "C" void func_002659C0(void* self);
void operator_delete(int*);

extern "C" void func_00265908(void* self, int flags)
{
    func_002659C0(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265950);

INCLUDE_ASM("dirtysock/tagsunk", func_002659C0);

INCLUDE_ASM("dirtysock/tagsunk", func_002659E8);

INCLUDE_ASM("dirtysock/tagsunk", func_00265A38);

INCLUDE_ASM("dirtysock/tagsunk", func_00265A68);

INCLUDE_ASM("dirtysock/tagsunk", func_00265CB0);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D08);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D38);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D68);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D98);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265DF0__FPv);
#ifdef SKIP_ASM
void func_00265DF0(void* self)
{
    *(int*)self = 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E00__FPv);
#ifdef SKIP_ASM
void func_00265E00(void* self)
{
    *(int*)self = 6;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E10__FPv);
#ifdef SKIP_ASM
void func_00265E10(void* self)
{
    *(int*)self = 10;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E20__FPv);
#ifdef SKIP_ASM
void func_00265E20(void* self)
{
    *(int*)self = 14;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E30__FPv);
#ifdef SKIP_ASM
void func_00265E30(void* self)
{
    *(int*)self = 18;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265E40);

INCLUDE_ASM("dirtysock/tagsunk", func_00265E80);

INCLUDE_ASM("dirtysock/tagsunk", func_00265EC8);

INCLUDE_ASM("dirtysock/tagsunk", func_00265F00);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265F18);
#ifdef SKIP_ASM
extern char D_00536760[0x11];
extern "C" void* func_003E6448(void* dst, int value, int size);
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_00265F18(const char* name)
{
    func_003E6448(D_00536760, 0, 0x11);
    strcpy(D_00536760, name);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265F68);
#ifdef SKIP_ASM
extern char D_00536771[0x11];
extern "C" void* func_003E6448(void* dst, int value, int size);
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_00265F68(const char* name)
{
    func_003E6448(D_00536771, 0, 0x11);
    strcpy(D_00536771, name);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265FB8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266010);

INCLUDE_ASM("dirtysock/tagsunk", func_00266038);

INCLUDE_ASM("dirtysock/tagsunk", func_00266060);

INCLUDE_ASM("dirtysock/tagsunk", func_002660A0);

INCLUDE_ASM("dirtysock/tagsunk", func_002660C8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266148);

INCLUDE_ASM("dirtysock/tagsunk", func_00266170);

INCLUDE_ASM("dirtysock/tagsunk", func_002661D8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266240);

INCLUDE_ASM("dirtysock/tagsunk", func_00266298);

INCLUDE_ASM("dirtysock/tagsunk", func_002662C0);

INCLUDE_ASM("dirtysock/tagsunk", func_002662E8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266320);

INCLUDE_ASM("dirtysock/tagsunk", func_00266348);

INCLUDE_ASM("dirtysock/tagsunk", func_00266358);

INCLUDE_ASM("dirtysock/tagsunk", func_00266550);

INCLUDE_ASM("dirtysock/tagsunk", func_00266578);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266728);
#ifdef SKIP_ASM
extern char D_00481080[];
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

extern "C" void* func_00266728(void* self, int size, int kind)
{
    int flags = 0x20000000;
    if (kind == 0x20) {
        flags = 0x23000000;
    }
    return operator_new_tag(size + 0x400, D_00481080, flags, 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266760);
#ifdef SKIP_ASM
void* cMemMan_free(void* ptr);

extern "C" void func_00266760(void* self, void* ptr)
{
    if (ptr != 0) {
        cMemMan_free(ptr);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266788);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002667E8);
#ifdef SKIP_ASM
extern char D_005371D0[];
extern "C" void func_002668A8(void* self);
extern "C" int func_003F61D0(int a0, int a1, void* name, void* cb, int a4);
extern "C" void* func_003E6448(void* dst, int value, int size);

extern "C" void* func_002667E8(void* self)
{
    *(int*)((char*)self + 0x460) = 0;
    if (func_003F61D0(0, 0, D_005371D0, (void*)func_002668A8, 0) >= 0) {
        *(int*)((char*)self + 0x460) = 1;
    }
    func_003E6448(self, 0, 0x460);
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002668A8);
#ifdef SKIP_ASM
extern "C" void func_003F6358(void* self);
extern "C" void func_0040C7C0(void* self, int a1);
extern "C" void func_0040D430(void* self, int a1);

extern "C" void func_002668A8(void* self)
{
    func_003F6358(self);
    func_0040C7C0(self, 0);
    func_0040D430(self, 0);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002668E8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266BA8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266CA0);

INCLUDE_ASM("dirtysock/tagsunk", func_00266CD8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266D00__FPv);
#ifdef SKIP_ASM
void* func_00266D00(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0x4) = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266D10);
#ifdef SKIP_ASM
extern "C" void func_00266DF8(void* self);
void operator_delete(int*);

extern "C" void func_00266D10(void* self, int flags)
{
    func_00266DF8(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266D58);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266DF8);
#ifdef SKIP_ASM
extern "C" void func_003F6AF0(void* self);

extern "C" void func_00266DF8(void* self)
{
    if (*(void**)self != 0) {
        func_003F6AF0(self);
        *(void**)self = 0;
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266E30);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266E88);
#ifdef SKIP_ASM
extern "C" void func_003F6B50(void* self);

extern "C" void func_00266E88(void* self)
{
    if (*(int*)self != 0 && *(int*)((char*)self + 0x4) != 0) {
        func_003F6B50(self);
        *(int*)((char*)self + 0x4) = 0;
    }
}
#endif

extern "C" void* func_003F6C40(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F38__FPv);
#ifdef SKIP_ASM
void* func_00266F38(void* self)
{
    return func_003F6C40(self);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266F58);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F90);
#ifdef SKIP_ASM
extern "C" void func_003F6BA0(void* a, void* b);

extern "C" void func_00266F90(void* self, void* a, void* b)
{
    if (*(int*)self != 0) {
        func_003F6BA0(a, b);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267128);

INCLUDE_ASM("dirtysock/tagsunk", func_002672C8);

INCLUDE_ASM("dirtysock/tagsunk", func_00267468);

extern "C" void* func_00255840(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DA8__FPv);
#ifdef SKIP_ASM
void* func_00267DA8(void* self)
{
    return func_00255840(self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DC8__FPv);
#ifdef SKIP_ASM
void func_00267DC8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DD0__FPv);
#ifdef SKIP_ASM
int func_00267DD0(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267DD8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DE8__FPv);
#ifdef SKIP_ASM
int func_00267DE8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DF0__FPv);
#ifdef SKIP_ASM
int func_00267DF0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DF8__FPv);
#ifdef SKIP_ASM
int func_00267DF8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E00);
#ifdef SKIP_ASM
extern "C" int func_00267E00(void* self)
{
    return *(int*)((char*)self + 0xc) == 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E10__FPv);
#ifdef SKIP_ASM
int func_00267E10(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E18);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267E18(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E48);
#ifdef SKIP_ASM
extern char D_00480488[];
extern void* D_004812B0[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void* func_00267E48(int* self)
{
    int* p = (int*)cMemMan_alloc(8, D_00480488, 0x20000000, 0);
    int v = *self;
    *(void***)((char*)p + 0x4) = D_004812B0;
    p[0] = v;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E98);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267E98(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267EC8);
#ifdef SKIP_ASM
extern char D_00480488[];
extern void* D_00481290[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void* func_00267EC8(void* self)
{
    int* p = (int*)cMemMan_alloc(0xC, D_00480488, 0x20000000, 0);
    int v = *(int*)((char*)self + 0x8);
    p[0] = 0x102;
    *(void***)((char*)p + 0x4) = D_00481290;
    p[2] = v;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267F28);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267F28(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267F58);
#ifdef SKIP_ASM
extern char D_00480488[];
extern void* D_00481290[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void* func_00267F58(void* self)
{
    int* p = (int*)cMemMan_alloc(0xC, D_00480488, 0x20000000, 0);
    int v = *(int*)((char*)self + 0x8);
    p[0] = 0x102;
    *(void***)((char*)p + 0x4) = D_00481290;
    p[2] = v;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267FB8);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267FB8(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267FE8);
#ifdef SKIP_ASM
extern char D_00480488[];
extern void* D_00481250[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void* func_00267FE8(void* self)
{
    int* p = (int*)cMemMan_alloc(0xC, D_00480488, 0x20000000, 0);
    int v = *(int*)((char*)self + 0x8);
    p[0] = 0x116;
    *(void***)((char*)p + 0x4) = D_00481250;
    p[2] = v;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268048);
#ifdef SKIP_ASM
extern void* D_004812B0[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

extern "C" void func_00268048(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0xC, 2);
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002680A8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268158);
#ifdef SKIP_ASM
extern void* D_004812B0[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

extern "C" void func_00268158(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002681B0);
#ifdef SKIP_ASM
extern char D_00480488[];
extern void* D_00481210[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* s);

extern "C" void* func_002681B0(void* self)
{
    int* p = (int*)cMemMan_alloc(0xC, D_00480488, 0x20000000, 0);
    const char* s = *(const char**)((char*)self + 0x8);
    *(void***)((char*)p + 0x4) = D_00481210;
    p[0] = 0xE2;
    cBXString_cBXString2((char*)p + 0x8, s);
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268210);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268210(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268240);
#ifdef SKIP_ASM
extern char D_00480488[];
extern void* D_004811F0[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void* func_00268240(void* self)
{
    int* p = (int*)cMemMan_alloc(0x10, D_00480488, 0x20000000, 0);
    int b = *(int*)((char*)self + 0xC);
    p[0] = 0x102;
    p[2] = 0x746F6F79;
    *(void***)((char*)p + 0x4) = D_004811F0;
    p[3] = b;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002682A8);
#ifdef SKIP_ASM
extern void* D_004812B0[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

extern "C" void func_002682A8(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0x10, 2);
    cBXString__cBXString((char*)self + 0xC, 2);
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00268308);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268398);
#ifdef SKIP_ASM
extern void* D_004812B0[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

extern "C" void func_00268398(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0xC, 2);
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002683F0);
#ifdef SKIP_ASM
extern void* D_004811B0[];

extern "C" void* func_002683F0(void* self)
{
    int* p = (int*)cMemMan_alloc(0x10, D_00480488, 0x20000000, 0);
    const char* s = *(const char**)((char*)self + 0xC);
    p[2] = 0x646F776E;
    *(void***)((char*)p + 0x4) = D_004811B0;
    p[0] = 0x102;
    cBXString_cBXString2((char*)p + 0xC, s);
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268460);
#ifdef SKIP_ASM
extern void* D_004812B0[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

extern "C" void func_00268460(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0xC, 2);
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002684C0);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268588);
#ifdef SKIP_ASM
extern void* D_004812B0[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

extern "C" void func_00268588(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002685E0);
#ifdef SKIP_ASM
extern void* D_00481170[];

extern "C" void* func_002685E0(void* self)
{
    int* p = (int*)cMemMan_alloc(0x10, D_00480488, 0x20000000, 0);
    int b = *(int*)((char*)self + 0xC);
    const char* s = *(const char**)((char*)self + 0x8);
    *(void***)((char*)p + 0x4) = D_00481170;
    p[0] = 0xE5;
    cBXString_cBXString2((char*)p + 0x8, s);
    p[3] = b;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268658);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268658(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268688);
#ifdef SKIP_ASM
extern char D_00480488[];
extern void* D_00481150[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void* func_00268688(void* self)
{
    int* p = (int*)cMemMan_alloc(0x10, D_00480488, 0x20000000, 0);
    int a = *(int*)((char*)self + 0x8);
    int b = *(int*)((char*)self + 0xC);
    p[0] = 0xFB;
    *(void***)((char*)p + 0x4) = D_00481150;
    p[2] = a;
    p[3] = b;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002686F0);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_002686F0(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268720);
#ifdef SKIP_ASM
extern void* D_00481130[];

extern "C" void* func_00268720(void* self)
{
    int* p = (int*)cMemMan_alloc(0x14, D_00480488, 0x20000000, 0);
    int a = *(int*)((char*)self + 0x8);
    int b = *(int*)((char*)self + 0xC);
    int c = *(int*)((char*)self + 0x10);
    p[0] = 0xFC;
    *(void***)((char*)p + 0x4) = D_00481130;
    p[2] = a;
    p[3] = b;
    p[4] = c;
    return p;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268790);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268790(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002687C0);

