#include "common.h"

INCLUDE_ASM("ui/uitext", cUIText_render2D);

//100%
INCLUDE_ASM("ui/uitext", cUIText_deleteText);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void cUIText_deleteText(void* self)
{
    *(int*)((char*)self + 0xA8) = 0;
    if (*(void**)((char*)self + 0xAC) != 0) {
        cMemMan_free(*(void**)((char*)self + 0xAC));
        *(void**)((char*)self + 0xAC) = 0;
    }
    if (*(void**)((char*)self + 0xB4) != 0) {
        cMemMan_free(*(void**)((char*)self + 0xB4));
        *(void**)((char*)self + 0xB4) = 0;
    }
}
#endif

INCLUDE_ASM("ui/uitext", cUIText_getNumTextLines);

//100%
INCLUDE_ASM("ui/uitext", func_003A0C00);
#ifdef SKIP_ASM
extern "C" int func_003A0C00(void* self)
{
    if (((*(int*)((char*)self + 0x74) >> 3) & 1) == 0) {
        return *(int*)((char*)self + 0xb4);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A0C28);
#ifdef SKIP_ASM
extern "C" int func_003A0C28(void* self)
{
    if (((*(int*)((char*)self + 0x74) >> 3) & 1) != 0) {
        return *(int*)((char*)self + 0xb4);
    }
    return 0;
}
#endif

struct cUITextManager {
    char pad_0x00[0x28];
    short field_0x28;
    char pad_0x2A[2];
    void (*fn)(void*, int); // 0x2C
};

struct cUIText {
    char pad_0x00[0x8];
    cUITextManager* mMgr; // 0x8
    char pad_0xC[0xB0 - 0xC];
    int field_0xB0;
};

extern "C" void cUIText_setAsciiStringPrivate(void* self, const char* str);

//100%
INCLUDE_ASM("ui/uitext", cUIText_setAsciiString__FP7cUITextPCc);
#ifdef SKIP_ASM
void cUIText_setAsciiString(cUIText* self, const char* str)
{
    self->field_0xB0 = 0;
    cUIText_setAsciiStringPrivate(self, str);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_setAsciiStringPrivate);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern "C" void cUIText_setUnicodeStringPrivate(void* self, const unsigned short* str);

class cUIText_3A0C70 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24(int);
};

extern "C" void cUIText_setAsciiStringPrivate(void* self, const char* str)
{
    unsigned short buf[0x200];
    ((cUIText_3A0C70*)self)->v05(1);
    ((cUIText_3A0C70*)self)->v24(0);
    if (strlen(str) + 1 < 0x200) {
        UIAsciiToUnicode(buf, str);
        cUIText_setUnicodeStringPrivate(self, buf);
    }
}
#endif

INCLUDE_ASM("ui/uitext", func_003A0D00);

//100%
INCLUDE_ASM("ui/uitext", func_003A0E90);
#ifdef SKIP_ASM
extern "C" void cUIText_setUnicodeStringPrivate(void* self, const unsigned short* str);

extern "C" void func_003A0E90(void* self, const unsigned short* str)
{
    *(int*)((char*)self + 0xB0) = 0;
    cUIText_setUnicodeStringPrivate(self, str);
}
#endif

INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringPrivate);

//100%
INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringByID__FP7cUITexti);
#ifdef SKIP_ASM
void cUIText_setUnicodeStringByID(cUIText* self, int id)
{
    self->field_0xB0 = id;
    cUITextManager* mgr = self->mMgr;
    mgr->fn((char*)self + mgr->field_0x28, 0);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1030);
#ifdef SKIP_ASM
extern "C" void* func_003A04F0(void* self);
extern "C" float func_003921F0(void* font, int str, void* out, int flags, float sx, float sy);

struct func_003A1030_sVec3 {
    float x;
    float y;
    float z;
};
extern func_003A1030_sVec3 D_004FF0D8;

struct func_003A1030_sRect {
    float x0;
    float y0;
    float x1;
    float y1;
};

struct func_003A1030_sVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_003A1030(void* self, func_003A1030_sVec3* out)
{
    if (*(int*)((char*)self + 0xB4) == 0 || ((*(int*)((char*)self + 0x14) >> 2) & 1) == 0) {
        func_003A1030_sVEntry* vt = *(func_003A1030_sVEntry**)((char*)self + 8);
        vt[16].fn((char*)self + vt[16].delta);
    }
    void* font = func_003A04F0(self);
    int str = *(int*)((char*)self + 0xB4);
    if (str != 0 && font != 0) {
        func_003A1030_sRect r;
        func_003921F0(font, str, &r, 0, *(float*)((char*)self + 0x50), *(float*)((char*)self + 0x54));
        out->x = r.x1;
        out->y = r.y1;
    } else {
        *out = D_004FF0D8;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1148);
#ifdef SKIP_ASM
extern "C" void* func_0039FB30(void* self, int a1, int a2);
extern "C" void* func_003977E8(void* self);
extern "C" void func_003A1310(void* self, char a1);
extern void* D_00494278[];

extern "C" void* func_003A1148(void* self, int a1, int a2)
{
    char* s = (char*)self;
    func_0039FB30(self, a1, a2);
    *(void***)(s + 0x8) = D_00494278;
    *(unsigned char*)(s + 0x74) &= 0xC6;
    func_003977E8(s + 0x8C);
    *(int*)(s + 0xB8) = 0;
    *(int*)(s + 0xBC) = 0;
    *(int*)(s + 0xC0) = 0;
    *(short*)(s + 0xC4) = 0;
    *(short*)(s + 0xC6) = 0;
    *(short*)(s + 0xC8) = 0;
    *(int*)(s + 0xCC) = 0;
    func_003A1310(self, 0);
    return self;
}
#endif

INCLUDE_ASM("ui/uitext", func_003A11B8);

//100%
INCLUDE_ASM("ui/uitext", func_003A12D0);
#ifdef SKIP_ASM
extern "C" int func_003A12D0(void* self)
{
    void* p = *(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x5c) + 0xd0) + 0x10);
    if (p != 0) {
        return *(int*)((char*)*(void**)((char*)p + 0x8) + ((*(unsigned int*)((char*)self + 0x74) >> 1) & 3) * 12 + 0xC);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1310);
#ifdef SKIP_ASM
extern "C" void func_003A1310(void* self, char a1)
{
    if (a1 == 0) {
        *(char*)((char*)self + 0x88) = 0;
        *(char*)((char*)self + 0x89) = 1;
        *(char*)((char*)self + 0x8A) = 2;
        *(char*)((char*)self + 0x8B) = 3;
    } else {
        *(char*)((char*)self + 0x88) = 4;
        *(char*)((char*)self + 0x89) = 5;
        *(char*)((char*)self + 0x8A) = 6;
        *(char*)((char*)self + 0x8B) = 7;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1360);
#ifdef SKIP_ASM
extern "C" void func_003A1CF0(void* self, void* a1);

class func_003A1360_cVirtA {
public:
    char pad[0x4];
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* v04(int);
};

class func_003A1360_cVirtB {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};

extern "C" void func_003A1360(void* self)
{
    int id = *(int*)((char*)self + 0xC0);
    if (id != 0) {
        func_003A1360_cVirtA* a = *(func_003A1360_cVirtA**)((char*)*(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x5C) + 0xD0) + 0x10) + 0x10);
        if (a != 0) {
            void* r = a->v04(id);
            if (r != 0) {
                func_003A1CF0(self, r);
            }
            ((func_003A1360_cVirtB*)self)->v05(1);
        }
    }
}
#endif

INCLUDE_ASM("ui/uitext", func_003A13E0);

INCLUDE_ASM("ui/uitext", func_003A1588);

//100%
INCLUDE_ASM("ui/uitext", func_003A18C0);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void func_003A18C0(void* self, void* a1)
{
    if (*(void***)((char*)self + 0xBC) != 0) {
        unsigned short i;
        for (i = 0; i < *(unsigned short*)((char*)self + 0xC8); i++) {
            if ((*(void***)((char*)self + 0xBC))[i] != 0) {
                cMemMan_free((*(void***)((char*)self + 0xBC))[i]);
            }
        }
        if (*(void***)((char*)self + 0xBC) != 0) {
            cMemMan_free(*(void***)((char*)self + 0xBC));
        }
        *(void***)((char*)self + 0xBC) = 0;
    }
    *(unsigned short*)((char*)self + 0xC6) = 0;
    *(unsigned short*)((char*)self + 0xC8) = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1958);
#ifdef SKIP_ASM
extern "C" void* func_0041605C(void* dst, const void* src, int n);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004940A8[];

extern "C" int func_003A1958(void* self)
{
    unsigned short old = *(unsigned short*)((char*)self + 0xC8);
    *(unsigned short*)((char*)self + 0xC8) = old + 1;
    void** arr = (void**)operator_new_tag(*(unsigned short*)((char*)self + 0xC8) * 4, D_004940A8, 0x100, 0);
    if (*(void***)((char*)self + 0xBC) != 0) {
        func_0041605C(arr, *(void***)((char*)self + 0xBC), old * 4);
        void** prev = *(void***)((char*)self + 0xBC);
        arr[old] = 0;
        if (prev != 0) {
            cMemMan_free(prev);
        }
    }
    *(void***)((char*)self + 0xBC) = arr;
    return old;
}
#endif

INCLUDE_ASM("ui/uitext", func_003A19F8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uitext", cUITextScroll_addUnicodeString);
#ifdef SKIP_ASM
extern "C" int func_003A1958(void* self);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004940B8[];

extern "C" void cUITextScroll_addUnicodeString(void* self, unsigned short* str)
{
    int idx = func_003A1958(self);
    unsigned short* copy = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, D_004940B8, 0x100, 0);
    USTR_copy(copy, str);
    (*(unsigned short***)((char*)self + 0xBC))[idx] = copy;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1CF0);
#ifdef SKIP_ASM
extern "C" void func_003A18C0(void*, void*);
extern "C" void func_003A1D30(void*, void*);

extern "C" void func_003A1CF0(void* self, void* a1)
{
    func_003A18C0(self, a1);
    func_003A1D30(self, a1);
}
#endif

INCLUDE_ASM("ui/uitext", func_003A1D30);

//100%
INCLUDE_ASM("ui/uitext", func_003A1F18);
#ifdef SKIP_ASM
extern "C" void func_003A1D30(void*, void*);

class func_003A1F18_cVirtA {
public:
    char pad[0x4];
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* v04(int);
};

class func_003A1F18_cVirtB {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};

extern "C" void func_003A1F18(void* self, int id)
{
    *(int*)((char*)self + 0xC0) = id;
    func_003A1F18_cVirtA* a = *(func_003A1F18_cVirtA**)((char*)*(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x5C) + 0xD0) + 0x10) + 0x10);
    if (a != 0) {
        void* r = a->v04(id);
        if (r != 0) {
            func_003A1D30(self, r);
        }
        ((func_003A1F18_cVirtB*)self)->v05(1);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1F90);
#ifdef SKIP_ASM
extern "C" void func_003A1F90(void* self, int idx)
{
    if (*(void***)((char*)self + 0xBC) != 0) {
        unsigned short n = *(unsigned short*)((char*)self + 0xC8);
        if (idx < n) {
            void** arr = (void**)operator_new_tag((n - 1) * 4, D_004940A8, 0x100, 0);
            if (idx > 0) {
                func_0041605C(arr, *(void***)((char*)self + 0xBC), idx * 4);
            }
            if (idx < *(unsigned short*)((char*)self + 0xC8) - 1) {
                // PORT: pointer held in int (only spelling found that gives idx-first addu)
                func_0041605C((void*)(idx * 4 + (int)arr), (void*)(idx * 4 + *(int*)((char*)self + 0xBC) + 4), (*(unsigned short*)((char*)self + 0xC8) - idx - 1) * 4);
            }
            if (*(void***)((char*)self + 0xBC) != 0) {
                cMemMan_free(*(void***)((char*)self + 0xBC));
            }
            *(void***)((char*)self + 0xBC) = arr;
            (*(unsigned short*)((char*)self + 0xC8))--;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A2068);
#ifdef SKIP_ASM
extern "C" int func_003A2068(void* self, int i)
{
    int* arr = *(int**)((char*)self + 0xBC);
    if (arr != 0 && i >= 0 && i < *(unsigned short*)((char*)self + 0xC8)) {
        return arr[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A3280);
#ifdef SKIP_ASM
extern "C" void* func_0039FB30(void* self, int a1, int a2);
extern void* D_004941B8[];

struct func_003A3280_s {
    char pad0[0x8];
    void** vtbl; // 0x8
    char padC[0x74 - 0xC];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int mid : 15;
    unsigned int hi : 15;
    int unk78;
    int unk7C;
    int unk80;
    int unk84;
    int unk88;
    int unk8C;
};

extern "C" func_003A3280_s* func_003A3280(func_003A3280_s* self, int a1, int a2)
{
    func_0039FB30(self, a1, a2);
    self->vtbl = D_004941B8;
    self->b0 = 0;
    self->b1 = 0;
    self->hi = 0;
    self->unk78 = 0;
    self->unk7C = 0;
    self->unk80 = 0;
    self->unk84 = 0;
    self->unk88 = 0;
    self->unk8C = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A32F0);
#ifdef SKIP_ASM
extern "C" void func_0039FC48(void* self, int flags);
extern void* D_004941B8[];

extern "C" void func_003A32F0(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004941B8;
    if (*(void**)((char*)self + 0x7C) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x7C));
    }
    if (*(void**)((char*)self + 0x80) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x80));
    }
    if (*(void**)((char*)self + 0x84) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x84));
    }
    if (*(void**)((char*)self + 0x88) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x88));
    }
    if (*(void**)((char*)self + 0x8C) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8C));
    }
    func_0039FC48(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A3398);
#ifdef SKIP_ASM
struct func_003A3398_sColor {
    float r;
    float g;
    float b;
    float a;
};

extern "C" void func_003A3398(void* self, func_003A3398_sColor* c)
{
    func_003A3398_sColor tmp = *c;
    unsigned int i;
    *(func_003A3398_sColor*)((char*)self + 0x1C) = *c;
    if (*(func_003A3398_sColor**)((char*)self + 0x7C) == 0) {
        return;
    }
    tmp.r = (int)(c->r * 255.0f);
    tmp.g = (int)(c->g * 255.0f);
    tmp.b = (int)(c->b * 255.0f);
    tmp.a = (int)(c->a * 255.0f);
    for (i = 0; i < *(unsigned int*)((char*)self + 0x74) >> 17; i++) {
        (*(func_003A3398_sColor**)((char*)self + 0x7C))[i] = tmp;
    }
}
#endif

