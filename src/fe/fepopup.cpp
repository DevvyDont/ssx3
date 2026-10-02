#include "common.h"

INCLUDE_ASM("fe/fepopup", cScreenPopup_cScreenPopup);

//100%
INCLUDE_ASM("fe/fepopup", func_001C5A90);
#ifdef SKIP_ASM
extern void* D_0046CBD8[];
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void func_001C5750(void* self, int flags);
extern "C" void func_0039E390(void* self, int flags);

extern "C" void func_001C5A90(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046CBD8;
    cBXString__cBXString((char*)self + 0x2B4, 2);
    cBXString__cBXString((char*)self + 0x2B0, 2);
    func_001C5750((char*)self + 0xBC, 2);
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A1C40[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cScreenPopup_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A1C40), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        *(int*)((char*)self + 0x2B8) = 1;
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

void* func_0039E4A0(void*);

//100%
INCLUDE_ASM("fe/fepopup", func_001C5B68__FPv);
#ifdef SKIP_ASM
void* func_001C5B68(void* self)
{
    *(int*)((char*)self + 0x2b8) = 0;
    return func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C5B88);
#ifdef SKIP_ASM
struct sVE5B88 {
    short delta;
    short index;
    void* fn;
};
extern "C" void func_001C5F20(void* self, float t);
extern "C" void func_001C75C8(void* self, void* menu, int idx, int max);

extern "C" void func_001C5B88(void* self)
{
    if (*(void**)((char*)self + 0x6C) != 0) {
        func_001C5F20(self, 1.0f);
        {
            char* o = *(char**)((char*)self + 0x6C);
            sVE5B88* vt = *(sVE5B88**)(o + 8);
            ((void (*)(void*, int))vt[8].fn)(o + vt[8].delta, 0);
        }
        func_001C75C8(self, *(void**)((char*)self + 0x6C), *(int*)((char*)self + 0x2C4), *(int*)((char*)self + 0x30C));
        {
            char* o = *(char**)((char*)self + 0x6C);
            sVE5B88* vt = *(sVE5B88**)(o + 8);
            ((void (*)(void*, int))vt[7].fn)(o + vt[7].delta, 1);
        }
        {
            sVE5B88* vt = *(sVE5B88**)((char*)self + 8);
            ((int (*)(void*, void*, int, int))vt[20].fn)((char*)self + vt[20].delta, *(void**)((char*)self + 0x6C), 5, 0);
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopup", cScreenPopup_onGainTransition);

//100%
INCLUDE_ASM("fe/fepopup", func_001C5DD0__FPv);
#ifdef SKIP_ASM
int func_001C5DD0(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C5DD8);
#ifdef SKIP_ASM
struct sColor_7738;
struct sColor5DD8 { int r, g, b; };
struct sVEntry001C5DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
extern "C" void func_001C8A38(void* self);
extern "C" void func_001C6B08(void* self, void* p);
extern "C" void func_001C7738(void* self, sColor_7738* color);

extern "C" void func_001C5DD8(void* self)
{
    if (*(int*)((char*)self + 0x2BC) != 0) {
        func_001C8A38(self);
        func_001C6B08(self, (char*)self + 0xBC);
        sColor5DD8 c = *(sColor5DD8*)((char*)self + 0x278);
        func_001C7738(self, (sColor_7738*)&c);
        sVEntry001C5DD8* vt = *(sVEntry001C5DD8**)((char*)self + 8);
        vt[6].fn((char*)self + vt[6].delta, 0);
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C5F20);

INCLUDE_ASM("fe/fepopup", cScreenPopup_FillObjectPointers);

INCLUDE_ASM("fe/fepopup", func_001C66E8);

INCLUDE_ASM("fe/fepopup", func_001C6B08);

INCLUDE_ASM("fe/fepopup", func_001C6D30);

//100%
INCLUDE_ASM("fe/fepopup", func_001C7040);
#ifdef SKIP_ASM
struct sVec3_7040 { float x, y, z; };
extern "C" void func_001C97E0(void* self, void* a, void* b, int which, float t);
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);

extern "C" void func_001C7040(void* self, void* a1)
{
    char* o = *(char**)((char*)self + 0x78);
    if (o != 0) {
        if (*(int*)((char*)a1 + 0x4C) != 0) {
            sVec3_7040 v = *(sVec3_7040*)((char*)self + 0x1B8);
            v.y += *(float*)((char*)self + 0x264);
            *(sVec3_7040*)(o + 0x44) = v;
            func_001C97E0(self, (char*)a1 + 0x4C, (char*)self + 0x260, 0, 10.0f);
            if (*(int*)((char*)self + 0x2CC) != 0) {
                func_001C8930(self, *(void**)((char*)self + 0x78), 0x11, 0.0f);
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C70F0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001C7258);
#ifdef SKIP_ASM
struct sVec3_7258 { float x, y, z; };
extern sVec3_7258 D_004FF0D8;
extern "C" void func_001C70F0(void* self, sVec3_7258* v);
extern "C" void func_001C93B0(void* self, int* msg, void* out);
extern "C" void func_001C97E0(void* self, void* a, void* b, int which, float t);
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);

extern "C" void func_001C7258(void* self, void* a1)
{
    char* o = *(char**)((char*)self + 0x74);
    if (o != 0) {
        sVec3_7258 v = *(sVec3_7258*)((char*)self + 0x1B8);
        v.y += *(float*)((char*)self + 0x264);
        *(sVec3_7258*)(o + 0x44) = v;
        char* p = (char*)a1 + 4;
        if (*(int*)((char*)self + 0x2CC) != 0) {
            sVec3_7258 t = D_004FF0D8;
            func_001C93B0(self, (int*)p, &t);
            if (t.x > *(float*)((char*)self + 0x260))
                func_001C70F0(self, &t);
        }
        func_001C97E0(self, p, (char*)self + 0x260, 1, 10.0f);
        if (*(int*)((char*)self + 0x2CC) != 0)
            func_001C8930(self, *(void**)((char*)self + 0x74), 0x11, 0.0f);
    }
    *(sVec3_7258*)((char*)self + 0x284) = *(sVec3_7258*)((char*)self + 0x260);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C7388);

//100%
INCLUDE_ASM("fe/fepopup", func_001C75C8);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" void func_001C75C8(void* self, void* menu, int idx, int max)
{
    int lo;
    idx = idx + 1;
    if (max == -1) {
        lo = 0;
        max = 1;
    } else {
        lo = 1;
    }
    if (idx < lo) {
        idx = lo;
    } else if (idx > max) {
        idx = max;
    }
    cUIMenu_setSelectedByIndex(menu, idx);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C7620);

//100%
INCLUDE_ASM("fe/fepopup", func_001C7738);
#ifdef SKIP_ASM
struct sColor_7738 { int r, g, b; };

extern "C" void func_001C7738(void* self, sColor_7738* color)
{
    sColor_7738 c = *color;
    switch (*(int*)((char*)self + 0x31C)) {
    case 0:
        {
            void* p = *(void**)((char*)self + 0x48);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x4C);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    case 1:
        {
            void* p = *(void**)((char*)self + 0x58);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x5C);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x50);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x54);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x60);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    case 2:
        {
            void* p = *(void**)((char*)self + 0x68);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C78C0);

//100%
INCLUDE_ASM("fe/fepopup", func_001C7BE0);
#ifdef SKIP_ASM
extern "C" float func_001C7BE0(void* self)
{
    if (*(int*)((char*)self + 0x2CC) != 0) {
        return 0.0f;
    }
    return (640.0f - *(float*)((char*)self + 0x260)) * 0.5f - *(float*)((char*)self + 0x1B8);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C7C20);

INCLUDE_ASM("fe/fepopup", func_001C7EB0);

//100%
INCLUDE_ASM("fe/fepopup", func_001C8050);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void func_003A0E90(cUIText* text, void* p);
extern char D_004A1408[];

extern "C" void func_001C8050(void* self, cUIText* text, int* info)
{
    switch (info[0]) {
    case 1:
        func_003A0E90(text, (void*)info[1]);
        break;
    case 2:
        cUIText_setAsciiString(text, (const char*)info[1]);
        break;
    case 3:
        cUIText_setUnicodeStringByID(text, info[1]);
        break;
    case 0:
        cUIText_setAsciiString(text, D_004A1408);
        break;
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C80E8);

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setTextString);
#ifdef SKIP_ASM
struct cUIText;
extern "C" char* func_003A0C00(cUIText* text);
extern "C" unsigned short* func_003A0C28(cUIText* text);
extern "C" int USTR_length(unsigned short* s);
extern "C" char* func_002C2580(char* dst, unsigned short* src);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004660B8[];
extern char D_004A1408[];

extern "C" void cScreenPopup_setTextString(void* self, void* dst, cUIText* text)
{
    char* s = func_003A0C00(text);
    if (s != 0) {
        cBXString_cBXString4(dst, s);
        return;
    }
    unsigned short* u = func_003A0C28(text);
    if (u != 0) {
        char* buf = (char*)operator_new_tag(USTR_length(u) + 1, D_004660B8, 0x100, 0);
        func_002C2580(buf, u);
        cBXString_cBXString4(dst, buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    } else {
        cBXString_cBXString4(dst, D_004A1408);
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C8320);

//100%
INCLUDE_ASM("fe/fepopup", func_001C84A8);
#ifdef SKIP_ASM
struct sVec3_84A8 { float x, y, z; };
extern "C" void* func_003A04F0(void* self);
extern "C" float func_00391FB0(void* self, const char* str, void* out, int n, float sx, float sy);

extern "C" void func_001C84A8(void* self, void* text, const char* str, void* out)
{
    sVec3_84A8 v = *(sVec3_84A8*)((char*)text + 0x50);
    char* f = (char*)func_003A04F0(text);
    float sx = *(float*)(f + 0x30) * v.x;
    float sy = *(float*)(f + 0x34) * v.y;
    *(float*)(f + 0x38) = sx;
    *(float*)(f + 0x3C) = sy;
    f = (char*)func_003A04F0(text);
    func_00391FB0(f, str, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
    f = (char*)func_003A04F0(text);
    *(float*)((char*)out + 0xC) = v.y * ((float)*(int*)(f + 0x14) * *(float*)(f + 0x34));
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8568);
#ifdef SKIP_ASM
struct sVec3_8568 { float x, y, z; };
extern "C" void* func_003A04F0(void* self);
extern "C" float func_003921F0(void* self, const char* str, void* out, int n, float sx, float sy);

extern "C" void func_001C8568(void* self, void* text, const char* str, void* out)
{
    sVec3_8568 v = *(sVec3_8568*)((char*)text + 0x50);
    char* f = (char*)func_003A04F0(text);
    float sx = *(float*)(f + 0x30) * v.x;
    float sy = *(float*)(f + 0x34) * v.y;
    *(float*)(f + 0x38) = sx;
    *(float*)(f + 0x3C) = sy;
    f = (char*)func_003A04F0(text);
    func_003921F0(f, str, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
    f = (char*)func_003A04F0(text);
    *(float*)((char*)out + 0xC) = v.y * ((float)*(int*)(f + 0x14) * *(float*)(f + 0x34));
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8628);
#ifdef SKIP_ASM
struct sVec2_8628 { float x, y; };

struct sVEntry001C8628 {
    short delta;
    short index;
    void (*fn)(void*, sVec2_8628*);
};

extern "C" void func_001C8628(void* self, void* obj, void* out)
{
    sVec2_8628 v;
    sVEntry001C8628* vt = *(sVEntry001C8628**)((char*)obj + 8);
    vt[20].fn((char*)obj + vt[20].delta, &v);
    *(float*)((char*)out + 0x8) = v.x;
    *(float*)((char*)out + 0xC) = v.y;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8678);
#ifdef SKIP_ASM
struct cUIText;
struct sVE8678 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
extern "C" void func_001C8050(void* self, cUIText* text, int* info);
extern "C" void func_001C8320(void* self, cUIText* text, int* info, float* out);
extern "C" void func_001C9B68(void* self, float* size, float* item);
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);

extern "C" void func_001C8678(void* self, void* a1)
{
    cUIText* text = *(cUIText**)((char*)self + 0x7C);
    if (text != 0) {
        int* info = (int*)((char*)a1 + 0xC);
        if (*(int*)((char*)a1 + 0xC) != 0) {
            float item[4];
            func_001C8050(self, text, info);
            {
                char* o = *(char**)((char*)self + 0x7C);
                sVE8678* vt = *(sVE8678**)(o + 8);
                vt[9].fn(o + vt[9].delta, 1);
            }
            func_001C8320(self, *(cUIText**)((char*)self + 0x7C), info, item);
            func_001C9B68(self, (float*)((char*)self + 0x260), item);
            *(float*)((char*)self + 0x264) += 5.0f;
            if (*(int*)((char*)self + 0x2CC) != 0) {
                func_001C8930(self, *(void**)((char*)self + 0x7C), 0x11, 0.0f);
            }
        } else {
            sVE8678* vt = *(sVE8678**)((char*)text + 8);
            vt[9].fn((char*)text + vt[9].delta, 0);
        }
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C8758);

//100%
INCLUDE_ASM("fe/fepopup", func_001C88C8);
#ifdef SKIP_ASM
struct sVec3_88C8 { float x, y, z; };

extern "C" void func_001C88C8(void* self)
{
    void* p = *(void**)((char*)self + 0x70);
    if (p != 0) {
        sVec3_88C8 v = *(sVec3_88C8*)((char*)self + 0x1AC);
        v.y += *(float*)((char*)self + 0x294);
        v.x += *(float*)((char*)self + 0x290);
        *(sVec3_88C8*)((char*)p + 0x44) = v;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8930);
#ifdef SKIP_ASM
struct sObjBitsK8930 {
    unsigned lo : 13;
    unsigned state : 6;
    unsigned hi : 13;
};
struct sVec2K8930 { float x, y; };
struct sVec3K8930 { float x, y, z; };

struct sVEntryK8930 {
    short delta;
    short index;
    void (*fn)(void*, sVec2K8930*);
};

extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t)
{
    sObjBitsK8930* b = (sObjBitsK8930*)((char*)obj + 0x14);
    if (b->state == state)
        return;
    sVec2K8930 size;
    sVEntryK8930* vt = *(sVEntryK8930**)((char*)obj + 8);
    vt[20].fn((char*)obj + vt[20].delta, &size);
    sVec3K8930 pos = *(sVec3K8930*)((char*)obj + 0x44);
    b->state = state;
    if (t == 0.0f)
        t = size.x * 0.5f;
    switch (state) {
    case 17:
    case 18:
        pos.x += t;
        break;
    case 9:
    case 10:
        pos.x -= t;
        break;
    }
    *(sVec3K8930*)((char*)obj + 0x44) = pos;
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C8A38);

INCLUDE_ASM("fe/fepopup", func_001C9038);

INCLUDE_ASM("fe/fepopup", func_001C9210);

//100%
INCLUDE_ASM("fe/fepopup", func_001C93B0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cScreenPopup_getMessageLineWrapSize(void* self, void* text, const char* str, void* out);
extern "C" void func_001C94B0(void* self, void* text, void* str, void* out);
extern void* D_004A28A8;
extern char D_004660A8[];

struct sVEntryK93B0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern "C" void func_001C93B0(void* self, int* msg, void* out)
{
    if (*(void**)((char*)self + 0x74) == 0)
        return;
    switch (msg[0]) {
    case 0:
        break;
    case 2:
        cScreenPopup_getMessageLineWrapSize(self, *(void**)((char*)self + 0x74), (const char*)msg[1], out);
        break;
    case 1:
        func_001C94B0(self, *(void**)((char*)self + 0x74), (void*)msg[1], out);
        break;
    case 3: {
        char* o = *(char**)((char*)D_004A28A8 + 0x8C);
        sVEntryK93B0* vt = *(sVEntryK93B0**)(o + 4);
        void* str = vt[4].fn(o + vt[4].delta, msg[1]);
        if (str) {
            func_001C94B0(self, *(void**)((char*)self + 0x74), str, out);
        } else {
            char buf[0x40];
            sprintf(buf, D_004660A8, msg[1]);
            cScreenPopup_getMessageLineWrapSize(self, *(void**)((char*)self + 0x74), buf, out);
        }
        break;
    }
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C94B0);

INCLUDE_ASM("fe/fepopup", cScreenPopup_getMessageLineWrapSize);

INCLUDE_ASM("fe/fepopup", func_001C97E0);

INCLUDE_ASM("fe/fepopup", func_001C9938);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001C9B28);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001C9210(void* self, cUIText* text, const char* str, int a3);

extern "C" void func_001C9B28(void* self, cUIText* text, const char* str, int a3)
{
    func_001C9210(self, text, str, a3);
    cUIText_setAsciiString(text, str);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C9B68);
#ifdef SKIP_ASM
extern "C" void func_001C9B68(void* self, float* size, float* item)
{
    float w = item[2];
    if (size[0] < w) {
        size[0] = w;
    }
    size[1] += item[3];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cScreenPopup_createConfirmationPopup);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cScreenPopup_cScreenPopup(void* mem, void* engine, void* owner);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00466110[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

struct sVec3K1C9B98 {
    float x, y, z;
    sVec3K1C9B98(float a, float b, float c) : x(a), y(b), z(c) {}
};
struct sPairK1C9B98 { int a, b; };
struct sEntryK1C9B98 { sPairK1C9B98 p; int c; };
struct sSelfK1C9B98 { char pad[0x2D0]; sEntryK1C9B98 entries[4]; };

extern "C" void cScreenPopup_createConfirmationPopup(void* self, int idx)
{
    char* popup = (char*)cScreenPopup_cScreenPopup(cMemMan_alloc(0x360, D_00466110, 0x100, 0), *(void**)((char*)self + 0x10), self);
    *(void**)((char*)self + 0x2AC) = popup;
    char* slots = popup + 0xBC;
    *(int*)(popup + 0xBC) = 2;
    func_001C57C0(slots, 0, GetHashValue32(D_0045DCC8));
    func_001C57C0(slots, 1, GetHashValue32(D_0045FCD8));
    sVec3K1C9B98 v(30.0f, 40.0f, 0.0f);
    *(sVec3K1C9B98*)(popup + 0x158) = v;
    *(int*)(popup + 0x14C) = *(int*)((char*)self + 0x314) + 3;
    *(int*)(popup + 0x168) = ((sSelfK1C9B98*)self)->entries[idx].c;
    *(sPairK1C9B98*)(popup + 0xC0) = ((sSelfK1C9B98*)self)->entries[idx].p;
    *(int*)((char*)self + 0x324) = idx;
    func_0039F290((char*)*(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x18,
                  *(void**)((char*)self + 0x2AC));
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C9CD0);

//100%
INCLUDE_ASM("fe/fepopup", func_001CA150);
#ifdef SKIP_ASM
extern "C" int func_001CA150(void* self)
{
    void* p = *(void**)((char*)self + 0x6C);
    if (p == 0) {
        return *(int*)((char*)self + 0x168);
    }
    int n = *(unsigned char*)((char*)p + 0x95);
    if (*(int*)((char*)self + 0x30C) == -1) {
        return n;
    }
    return n - 1;
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CA180);

INCLUDE_ASM("fe/fepopup", func_001CA298);

//100%
INCLUDE_ASM("fe/fepopup", func_001CA488);
#ifdef SKIP_ASM
struct sVEntry_func_001CA488 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CA488(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001CA488* vt = *(sVEntry_func_001CA488**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA4C0);
#ifdef SKIP_ASM
struct sVEntry_func_001CA4C0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CA4C0(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001CA4C0* vt = *(sVEntry_func_001CA4C0**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA4F8);
#ifdef SKIP_ASM
class func_001CA4F8_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

extern "C" int func_001CA4F8(void* self, int a1, int a2)
{
    return (*(func_001CA4F8_cObj**)((char*)self + 0x20))->v24(a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA528);
#ifdef SKIP_ASM
struct sVEntry001CA528 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0039E510(void*);

extern "C" void func_001CA528(void* self)
{
    if (*(int*)((char*)self + 0x2C0) != 0) {
        void* obj = *(void**)((char*)self + 0x20);
        sVEntry001CA528* vt = *(sVEntry001CA528**)((char*)obj + 8);
        vt[12].fn((char*)obj + vt[12].delta);
    }
    func_0039E510(self);
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/fepopup", func_001CA578__FPv);
#ifdef SKIP_ASM
void* func_001CA578(void* self)
{
    return func_0039E6B8(self);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CA598);

INCLUDE_ASM("fe/fepopup", cScreenPopup_checkAndFixTextEntry);

//100%
INCLUDE_ASM("fe/fepopup", func_001CA8A0);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int value, int size);

extern "C" void func_001CA8A0(void* self)
{
    func_003E6448((char*)self + 0x14, 0, 8);
    func_003E6448((char*)self + 0xC, 0, 8);
    func_003E6448((char*)self + 0x1C, 0, 8);
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySongByCredit);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004661B8[];
extern char D_004661C8[];

extern "C" void cBuyPopupInfo_initBuySongByCredit(void* self, int song, int credit)
{
    int three = 3;
    int h = GetHashValue32(D_004661B8);
    *(int*)((char*)self + 0x10) = song;
    *(int*)((char*)self + 0x14) = three;
    *(int*)((char*)self + 0xC) = 2;
    *(int*)((char*)self + 0x18) = h;
    h = GetHashValue32(D_004661C8);
    *(int*)((char*)self + 0x8) = credit;
    *(int*)((char*)self + 0x1C) = three;
    *(int*)((char*)self + 0x20) = h;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySong);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004661B8[];
extern char D_004661E0[];

extern "C" void cBuyPopupInfo_initBuySong(int* self, int a1, int a2, int a3)
{
    int three = 3;
    int h = GetHashValue32(D_004661B8);
    self[4] = a1;
    self[5] = three;
    self[3] = 2;
    self[6] = h;
    h = GetHashValue32(D_004661E0);
    self[1] = a3;
    self[7] = three;
    self[0] = a2;
    self[2] = -1;
    self[8] = h;
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CAA18);

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuyBolt);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004662B8[];
extern char D_004661F8[];

extern "C" void cBuyPopupInfo_initBuyBolt(int* self, int a1, int a2, int a3)
{
    int three = 3;
    int h = GetHashValue32(D_004662B8);
    self[4] = a1;
    self[5] = three;
    self[3] = 2;
    self[6] = h;
    h = GetHashValue32(D_004661F8);
    self[1] = a3;
    self[7] = three;
    self[0] = a2;
    self[2] = -1;
    self[8] = h;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001CABA8);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" void func_001CA8A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern void* D_0046CB08[];

extern "C" void* func_001CABA8(void* self, void* engine, void* owner, signed char idx)
{
    func_0039E318(self, engine, owner);
    *(void***)((char*)self + 0x8) = D_0046CB08;
    func_001CA8A0((char*)self + 0x48);
    *(signed char*)((char*)self + 0x44) = idx;
    *(int*)((char*)self + 0xC) = 7;
    void* p = **(void***)((char*)self + 0x10);
    if (p != 0) {
        *(char*)((char*)self + 0x15) = func_001A1CD0(p, idx);
    }
    func_001CA8A0((char*)self + 0x48);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CAC30);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004662C8[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void func_001CAC30(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004662C8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onWidgetCreate);

INCLUDE_ASM("fe/fepopup", func_001CAED8);

//100%
INCLUDE_ASM("fe/fepopup", func_001CAF00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A14E8[];

extern "C" void func_001CAF00(void* self, void* item, int event)
{
    if (item != 0) {
        if (event == 5) {
            int h = *(int*)((char*)item + 0x38);
            if (h == GetHashValue32(D_004A14E8)) {
                *(int*)((char*)self + 0x6C) = 1;
            } else {
                *(int*)((char*)self + 0x6C) = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_initBuyTrick);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466338[];
extern char D_00466350[];

extern "C" void cUIStateBuyPopup_initBuyTrick(void* self, int trick, int a2, int a3)
{
    int three = 3;
    *(int*)((char*)self + 0x48) = a2;
    *(int*)((char*)self + 0x4C) = a3;
    *(int*)((char*)self + 0x58) = trick;
    *(int*)((char*)self + 0x54) = 2;
    int h = GetHashValue32(D_00466338);
    *(int*)((char*)self + 0x5C) = three;
    *(int*)((char*)self + 0x60) = h;
    h = GetHashValue32(D_00466350);
    *(int*)((char*)self + 0x64) = three;
    *(int*)((char*)self + 0x68) = h;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CAFC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466368[];
extern char D_004A1408[];

extern "C" void func_001CAFC0(void* self, int a1, int a2, int a3)
{
    char* s = D_004A1408;
    int two = 2;
    *(char**)((char*)self + 0x58) = s;
    *(int*)((char*)self + 0x54) = two;
    *(int*)((char*)self + 0x48) = a2;
    *(int*)((char*)self + 0x4C) = a3;
    int h = GetHashValue32(D_00466368);
    *(int*)((char*)self + 0x64) = two;
    *(char**)((char*)self + 0x68) = s;
    *(int*)((char*)self + 0x5C) = 3;
    *(int*)((char*)self + 0x60) = h;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CB030);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" void cBXString_Reset(void* self);
extern "C" void func_001CE2A8(void* self);
extern int D_004A3E90;
extern void* D_004A33F0;
extern void* D_0046BEC8[];

extern "C" void* func_001CB030(void* self, void* engine, void* owner, int a3, unsigned char a4)
{
    func_0039E318(self, engine, owner);
    int d = D_004A3E90;
    *(unsigned char*)((char*)self + 0x15) = a4;
    *(int*)((char*)self + 0x70) = a3;
    *(int*)((char*)self + 0x418) = 0;
    *(int*)((char*)self + 0x41C) = 0;
    *(int*)((char*)self + 0x420) = 1;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x6C) = 0;
    *(int*)((char*)self + 0x68) = 1;
    *(char*)((char*)self + 0x74) = 0;
    *(int*)((char*)self + 0x134) = 0;
    *(int*)((char*)self + 0x138) = 0;
    *(int*)((char*)self + 0x13C) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(void***)((char*)self + 0x8) = D_0046BEC8;
    *(int*)((char*)self + 0x140) = 0x1D;
    *(int*)((char*)self + 0x424) = d;
    *(int*)((char*)self + 0x60) = 0x3F;
    cBXString_Reset((char*)self + 0x424);
    void* bx = D_004A33F0;
    *(int*)((char*)self + 0x144) = 2;
    *(int*)((char*)self + 0x434) = 0x2B;
    *(int*)((char*)self + 0x43C) = 1;
    *(int*)((char*)self + 0x428) = 0;
    *(int*)((char*)self + 0x42C) = 0;
    *(int*)((char*)self + 0x430) = 0;
    *(int*)((char*)self + 0x438) = 0;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x50) = 0;
    if (bx != 0)
        *(int*)((char*)self + 0x68) = (*(unsigned int*)((char*)bx + 0x88) >> 1) & 1;
    func_001CE2A8(self);
    *(int*)((char*)self + 0x440) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CB138);
#ifdef SKIP_ASM
extern void* D_0046BEC8[];
void cMemMan_free(void* ptr);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void func_0039E390(void* self, int flags);

extern "C" void func_001CB138(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046BEC8;
    void* p = *(void**)((char*)self + 0x428);
    if (p != 0) {
        cMemMan_free(p);
        *(void**)((char*)self + 0x428) = 0;
    }
    cBXString__cBXString((char*)self + 0x424, 2);
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466388[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cKeyboardPopup_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00466388), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CB208);
#ifdef SKIP_ASM
struct sVEntry001CB208 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* D_004A2EEC;
extern void* D_004A28A8;
extern "C" void func_0039E510(void*);
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001CB208(void* self)
{
    func_0039E510(self);
    if (D_004A2EEC != 0 && *(int*)((char*)D_004A2EEC + 0x64) == 0 && *(int*)((char*)D_004A28A8 + 0x84) != 0) {
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
    }
    if (*(int*)((char*)self + 0x440) != 0) {
        void* obj = *(void**)((char*)self + 0x20);
        if (obj != 0) {
            sVEntry001CB208* vt = *(sVEntry001CB208**)((char*)obj + 8);
            vt[12].fn((char*)obj + vt[12].delta);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CB290);
#ifdef SKIP_ASM
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001CB290(void* self, void* a1, unsigned int a2)
{
    switch (a2) {
    case 5:
    case 6:
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern int D_004A1A70;
extern void* D_004A28A8;
extern char D_00466398[];
extern char D_004663A8[];
extern char D_004663B8[];
extern char D_004663C8[];

struct sVEntryK1CB2C8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int isOnK1CB2C8()
{
    return D_004A1A70 == 1;
}

static inline int IsK1CB2C8(int id, char* s)
{
    return id == GetHashValue32(s);
}

extern "C" void cKeyboardPopup_onWidgetCreate(void* self, void* widget)
{
    if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_00466398)) {
        sVEntryK1CB2C8* vt = *(sVEntryK1CB2C8**)((char*)widget + 8);
        vt[9].fn((char*)widget + vt[9].delta, 0);
        if (isOnK1CB2C8()) {
            sVEntryK1CB2C8* vt2 = *(sVEntryK1CB2C8**)((char*)widget + 8);
            vt2[9].fn((char*)widget + vt2[9].delta, 1);
        }
        if (*(int*)((char*)D_004A28A8 + 0x84) != 0) {
            sVEntryK1CB2C8* vt3 = *(sVEntryK1CB2C8**)((char*)widget + 8);
            vt3[9].fn((char*)widget + vt3[9].delta, 0);
        }
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_004663A8)) {
        *(void**)((char*)self + 0x48) = widget;
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_004663B8)) {
        *(void**)((char*)self + 0x50) = widget;
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_004663C8)) {
        *(void**)((char*)self + 0x4C) = widget;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CB3D0);
#ifdef SKIP_ASM
struct sVEntry001CB3D0 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_001CB3D0(void* self, int a1, int a2)
{
    if (*(int*)((char*)self + 0x438) != 0) {
        void* obj = *(void**)((char*)self + 0x20);
        if (obj != 0) {
            sVEntry001CB3D0* vt = *(sVEntry001CB3D0**)((char*)obj + 8);
            return vt[24].fn((char*)obj + vt[24].delta, a1, a2);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CB418);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" void func_001CE3C8(void* self, int i, int v, int mode);
extern "C" void cKeyboardPopup_redrawScreen(void* self);
extern "C" void func_001CC848(void* self, int a1);

extern "C" void func_001CB418(void* self, const char* str)
{
    int one = 1;
    char* buf = (char*)self + 0x74;
    *(int*)((char*)self + 0x418) = one;
    strcpy(buf, str);
    *(int*)((char*)self + 0x134) = strlen(buf);
    *(int*)((char*)self + 0x138) = 0;
    *(int*)((char*)self + 0x13C) = strlen(buf);
    if (*(int*)((char*)self + 0x58) != 0) {
        func_001CE3C8(self, 0x4B, strlen(buf) ? one : 0, 2);
    }
    if (strlen(buf) < *(int*)((char*)self + 0x64)) {
        func_001CE3C8(self, 0x37, 0, 2);
    } else {
        func_001CE3C8(self, 0x37, 1, 2);
    }
    cKeyboardPopup_redrawScreen(self);
    if (*(int*)((char*)self + 0x41C) != 0) {
        func_001CC848(self, 0);
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CB500);

INCLUDE_ASM("fe/fepopup", func_001CB650);

INCLUDE_ASM("fe/fepopup", func_001CB7F0);

INCLUDE_ASM("fe/fepopup", func_001CB9B0);

//100%
INCLUDE_ASM("fe/fepopup", func_001CBB70);
#ifdef SKIP_ASM
// PORT: func_001CE560's third parameter is an int* out-param; the unit declares it (void*, int, int).
int func_001CE560_out(void* self, int idx, int* out) __asm__("func_001CE560");
extern "C" void func_001CC0E0(void* self, int a1);

extern "C" int func_001CBB70(void* self, int a1)
{
    int val;
    int state = *(int*)((char*)self + 0x140);
    if (state >= 5 && state <= 8) {
        if (func_001CE560_out(self, 0x4B, &val)) {
            *(int*)((char*)self + 0x140) = 0x4B;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC0E0(self, -1);
        }
        return 1;
    } else if (state >= 9 && state <= 10) {
        if (func_001CE560_out(self, 0x4C, &val)) {
            *(int*)((char*)self + 0x140) = 0x4C;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC0E0(self, -1);
        }
        return 1;
    } else if (state >= 11 && state <= 12) {
        if (func_001CE560_out(self, 0x4F, &val)) {
            *(int*)((char*)self + 0x140) = 0x4F;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC0E0(self, -1);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CBC48);
#ifdef SKIP_ASM
// PORT: func_001CE560's third parameter is an int* out-param; the unit declares it (void*, int, int).
int func_001CE560_out(void* self, int idx, int* out) __asm__("func_001CE560");
extern "C" void func_001CC190(void* self);

extern "C" int func_001CBC48(void* self, int a1)
{
    int val;
    int state = *(int*)((char*)self + 0x140);
    if (state == 4) {
        if (func_001CE560_out(self, 0x10, &val)) {
            *(int*)((char*)self + 0x140) = 0x10;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC190(self);
        }
        return 1;
    } else if (state == 5) {
        if (func_001CE560_out(self, 0x12, &val)) {
            *(int*)((char*)self + 0x140) = 0x12;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC190(self);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CBCE8);
#ifdef SKIP_ASM
// PORT: func_001CE560's third parameter is an int* out-param; the unit declares it (void*, int, int).
int func_001CE560_out(void* self, int idx, int* out) __asm__("func_001CE560");
extern "C" void func_001CC090(void* self);

extern "C" int func_001CBCE8(void* self, int a1)
{
    int val;
    int val2;
    int state = *(int*)((char*)self + 0x140);
    if (state == 0x4C) {
        if (func_001CE560_out(self, 0, &val)) {
            *(int*)((char*)self + 0x140) = 9;
        } else if (func_001CE560_out(self, 0x17, &val)) {
            *(int*)((char*)self + 0x140) = 0x17;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC090(self);
        }
        return 1;
    } else if (state == 0x4B) {
        if (func_001CE560_out(self, 0x13, &val2)) {
            *(int*)((char*)self + 0x140) = 0x13;
        } else {
            if (val2) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC090(self);
        }
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CBDB0);

//100%
INCLUDE_ASM("fe/fepopup", func_001CBF20);
#ifdef SKIP_ASM
// PORT: func_001CE560's third parameter is an int* out-param; the unit declares it (void*, int, int).
int func_001CE560_out(void* self, int idx, int* out) __asm__("func_001CE560");
extern "C" void func_001CC140(void* self);

extern "C" int func_001CBF20(void* self, int a1)
{
    int val;
    int state = *(int*)((char*)self + 0x140);
    if (state == 0x4C) {
        if (func_001CE560_out(self, 0x42, &val)) {
            *(int*)((char*)self + 0x140) = 0x42;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC140(self);
        }
        return 1;
    } else if (state == 0x4F) {
        if (func_001CE560_out(self, 0x42, &val)) {
            *(int*)((char*)self + 0x140) = 0x42;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC140(self);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CBFC0);
#ifdef SKIP_ASM
// PORT: func_001CE560's third parameter is an int* out-param; the unit declares it (void*, int, int).
int func_001CE560_out(void* self, int idx, int* out) __asm__("func_001CE560");
extern "C" void func_001CC090(void* self);

extern "C" int func_001CBFC0(void* self, int a1)
{
    int val;
    int state = *(int*)((char*)self + 0x140);
    if (state == 0x39) {
        if (func_001CE560_out(self, 0x4B, &val)) {
            *(int*)((char*)self + 0x140) = 0x4B;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC090(self);
        }
        return 1;
    } else if (state >= 0x3A && state <= 0x41) {
        if (func_001CE560_out(self, 0x4B, &val)) {
            *(int*)((char*)self + 0x140) = 0x4B;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC090(self);
        }
        return 1;
    } else if (state == 0x42) {
        if (func_001CE560_out(self, 0x4C, &val)) {
            *(int*)((char*)self + 0x140) = 0x4C;
        } else {
            if (val) {
                *(int*)((char*)self + 0x144) = a1 == 0;
            }
            func_001CC090(self);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC090);
#ifdef SKIP_ASM
extern "C" int func_001CE560(void*, int, int);

extern "C" void func_001CC090(void* self)
{
    int i = *(int*)((char*)self + 0x140) - 0x38;
    while (!func_001CE560(self, i, 0)) {
        i += 0xE;
    }
    *(int*)((char*)self + 0x140) = i;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC0E0);
#ifdef SKIP_ASM
extern "C" int func_001CE560(void*, int, int);

extern "C" void func_001CC0E0(void* self, int start)
{
    int i;
    if (start == -1) {
        i = *(int*)((char*)self + 0x140) + 0x38;
    } else {
        i = start;
    }
    while (!func_001CE560(self, i, 0)) {
        i -= 0xE;
    }
    *(int*)((char*)self + 0x140) = i;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC140);
#ifdef SKIP_ASM
extern "C" int func_001CE560(void*, int, int);

extern "C" void func_001CC140(void* self)
{
    int i = *(int*)((char*)self + 0x140) - 0xE;
    while (!func_001CE560(self, i, 0)) {
        i -= 0xE;
    }
    *(int*)((char*)self + 0x140) = i;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC190);
#ifdef SKIP_ASM
extern "C" int func_001CE560(void*, int, int);

extern "C" void func_001CC190(void* self)
{
    int i = *(int*)((char*)self + 0x140) + 0xE;
    while (!func_001CE560(self, i, 0)) {
        i += 0xE;
    }
    *(int*)((char*)self + 0x140) = i;
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CC1E0);

//100%
INCLUDE_ASM("fe/fepopup", func_001CC620);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

extern "C" void func_001CC620(void* self, int value)
{
    char c = value;
    char* buf = (char*)self + 0x74;
    int len = strlen(buf);
    int cur = *(int*)((char*)self + 0x134);
    buf[cur] = c;
    if (cur == len) {
        if (*(int*)((char*)self + 0x134) == *(int*)((char*)self + 0x60) - 1) {
            buf[*(int*)((char*)self + 0x60)] = 0;
            *(int*)((char*)self + 0x13C) = *(int*)((char*)self + 0x134) + 1;
        } else {
            if (*(int*)((char*)self + 0x134) < *(int*)((char*)self + 0x60) - 1) {
                *(int*)((char*)self + 0x134) += 1;
            }
            buf[*(int*)((char*)self + 0x134)] = 0;
            *(int*)((char*)self + 0x13C) += 1;
        }
    } else {
        if (*(int*)((char*)self + 0x134) < *(int*)((char*)self + 0x60) - 1) {
            *(int*)((char*)self + 0x134) += 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC6E8);
#ifdef SKIP_ASM
extern "C" int func_001CE560(void*, int, int);
extern signed char D_00441390[];
extern signed char D_004413E8[];

extern "C" int func_001CC6E8(void* self, int value)
{
    char c = value;
    int i;
    if (c == 0x60) {
        *(int*)((char*)self + 0x140) = 0xE;
        return 1;
    }
    for (i = 0; i < 0x54; i++) {
        if ((D_00441390[i] == c || D_004413E8[i] == c) && D_00441390[i] != '*'
            && (D_00441390[i] != '@' || D_004413E8[i] != D_00441390[i])) {
            if (func_001CE560(self, i, 0)) {
                *(int*)((char*)self + 0x140) = i;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001CC7C8);
#ifdef SKIP_ASM
extern "C" void func_001CC620(void* self, int value);
extern signed char D_00441390[];
extern signed char D_004413E8[];

extern "C" void func_001CC7C8(void* self)
{
    bool b = *(int*)((char*)self + 0x68) != 0;
    int flag = *(int*)((char*)self + 0x6C);
    if (flag) {
        flag = !b;
    } else {
        flag = b != 0;
    }
    int idx = *(int*)((char*)self + 0x140);
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)((char*)self + 0x6C);
    }
    if (flag) {
        func_001CC620(self, D_00441390[idx]);
    } else {
        func_001CC620(self, D_004413E8[idx]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC848);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
struct sPwd_CCE98;
extern "C" void func_001CCE98(sPwd_CCE98* self, char* str);
extern "C" void func_001CC8D8(void* self, int a1);
extern "C" void func_001CCC30(void* self);
extern char D_00466530[];

extern "C" void func_001CC848(void* self, int a1)
{
    func_001CC8D8(self, a1);
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00466530));
    if (text != 0) {
        if (*(int*)((char*)self + 0x70) != 0) {
            func_001CCE98((sPwd_CCE98*)self, (char*)self + 0xB4);
            cUIText_setAsciiString(text, (char*)self + 0xF4);
        } else {
            cUIText_setAsciiString(text, (char*)self + 0xB4);
        }
    }
    func_001CCC30(self);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CC8D8);

INCLUDE_ASM("fe/fepopup", func_001CCC30);

//100%
INCLUDE_ASM("fe/fepopup", func_001CCE98);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

struct sPwd_CCE98 {
    char pad[0xF4];
    char buf[1];
};

extern "C" void func_001CCE98(sPwd_CCE98* self, char* str)
{
    int n = strlen(str);
    int i;
    for (i = 0; i < n; i++) {
        self->buf[i] = '*';
    }
    self->buf[n] = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CCF00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466580[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBXString_cBXString4(void* self, const char* str);

extern "C" void func_001CCF00(void* self, const char* str)
{
    if (*(void**)((char*)self + 0x40) == 0) {
        cBXString_cBXString4((char*)self + 0x424, str);
        return;
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00466580));
    if (text != 0) {
        cUIText_setAsciiString(text, str);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setStatic);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_003A0E90(cUIText* text, void* p);
void cMemMan_free(void* ptr);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00466580[];
extern char D_00466590[];

extern "C" void cKeyboardPopup_setStatic(void* self, unsigned short* str)
{
    if (*(void**)((char*)self + 0x40) == 0) {
        void* p = *(void**)((char*)self + 0x428);
        if (p != 0) {
            cMemMan_free(p);
            *(void**)((char*)self + 0x428) = 0;
        }
        unsigned short* copy = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, D_00466590, 0x100, 0);
        *(unsigned short**)((char*)self + 0x428) = copy;
        USTR_copy(copy, str);
    } else {
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00466580));
        if (text != 0) {
            func_003A0E90(text, str);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CD020);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_00466580[];

extern "C" void func_001CD020(void* self, int id)
{
    if (*(void**)((char*)self + 0x40) == 0) {
        *(int*)((char*)self + 0x42C) = id;
    } else {
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00466580));
        if (text != 0) {
            cUIText_setUnicodeStringByID(text, id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CD088);
#ifdef SKIP_ASM
extern "C" void func_001CD088(void* self, int v)
{
    if (v < 0) {
        *(int*)((char*)self + 0x60) = 0;
        return;
    }
    if (v >= 0x40) {
        *(int*)((char*)self + 0x60) = 0x3f;
        return;
    }
    *(int*)((char*)self + 0x60) = v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CD0B0);
#ifdef SKIP_ASM
extern "C" void func_001CD0B0(void* self, int v)
{
    if (v < 0) {
        *(int*)((char*)self + 0x64) = 0;
        return;
    }
    if (v >= 0x40) {
        *(int*)((char*)self + 0x64) = 0x3f;
        return;
    }
    *(int*)((char*)self + 0x64) = v;
}
#endif

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_getObjName);

//100%
INCLUDE_ASM("fe/fepopup", func_001CD2F0);
#ifdef SKIP_ASM
extern "C" void func_001CCC30(void* self);
extern "C" void func_0039E4C0(void* self, int a1);

extern "C" void func_001CD2F0(void* self, int a1)
{
    if (*(int*)((char*)self + 0x134) == *(int*)((char*)self + 0x60)) {
        *(int*)((char*)self + 0x134) = *(int*)((char*)self + 0x134) - 1;
    }
    func_001CCC30(self);
    func_0039E4C0(self, a1);
}
#endif

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onGainTransition);

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_redrawScreen);

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_SetNameForKey);

INCLUDE_ASM("fe/fepopup", func_001CDB98);

//100%
INCLUDE_ASM("fe/fepopup", func_001CE2A8);
#ifdef SKIP_ASM
extern "C" void func_001CE3C8(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int a1);
extern "C" int strlen(const char* s);
extern signed char D_00441390[];

extern "C" void func_001CE2A8(void* self)
{
    int i;
    for (i = 0; i < 0x54; i++) {
        if (D_00441390[i] == '*' || i == 0x1C) {
            func_001CE3C8(self, i, 0, 2);
        } else if (i == 0x45 || i == 0x4F || i == 0x29) {
            func_001CE3C8(self, i, 0, 2);
        } else if (i == 0x4B) {
            if (strlen((char*)self + 0x74) != 0 && *(int*)((char*)self + 0x58) != 0)
                func_001CE3C8(self, i, 1, 2);
            else
                func_001CE3C8(self, i, 0, 2);
        } else {
            func_001CE3C8(self, i, 1, 2);
        }
    }
    func_001CE468(self, 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE3C8);
#ifdef SKIP_ASM
extern "C" void func_001CE3C8(void* self, int i, int v, int mode)
{
    if (mode == 0 || mode == 2) {
        *(int*)((char*)self + (i << 2) + 0x148) = v;
    }
    if (mode == 1 || mode == 2) {
        *(int*)((char*)self + (i << 2) + 0x298) = v;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE408);
#ifdef SKIP_ASM
extern "C" void func_001CE408(void* self, bool on)
{
    int i;
    for (i = 0xF; i < 0x19; i++) {
        func_001CE3C8(self, i, !on, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE468);
#ifdef SKIP_ASM
extern "C" void func_001CE468(void* self, int v)
{
    int i;
    for (i = 0; i < 0xE; i++) {
        func_001CE3C8(self, i, v, 2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE4C8);
#ifdef SKIP_ASM
struct sVec3_CE4C8 { float x, y, z; };

extern "C" void* func_003A04F0(void* self);

extern "C" void func_001CE4C8(void* self, void* obj)
{
    sVec3_CE4C8 v = *(sVec3_CE4C8*)((char*)obj + 0x50);
    char* p = (char*)func_003A04F0(obj);
    float x = *(float*)(p + 0x30) * v.x;
    float y = *(float*)(p + 0x34) * v.y;
    *(float*)(p + 0x38) = x;
    *(float*)(p + 0x3C) = y;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001CE520);
#ifdef SKIP_ASM
extern "C" int func_001CE560(void*, int, int);
extern "C" void func_001CB9B0(void*, int);

extern "C" void func_001CE520(void* self)
{
    if (func_001CE560(self, *(int*)((char*)self + 0x140), 0) == 0) {
        func_001CB9B0(self, 0);
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CE560);

//100%
INCLUDE_ASM("fe/fepopup", func_001CE5E0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* func_0014F6A8(void* self, int i);
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int value);
extern "C" void func_003DCB20(char* dst, void* src);
extern "C" char* func_004162D0(char* dst, const char* src);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern void* D_004A28A8;

struct sVEntryK1CE5E0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern "C" void func_001CE5E0(void* self, int key)
{
    char buf[0x40];
    int room = *(int*)((char*)self + 0x60) - strlen((char*)self + 0x74);
    void* opt = cBE_getInterface_Fv(cBE_getBE(), 4);
    char* msg = func_0014F6A8(opt, key);
    if (msg == 0) {
        char* o = *(char**)((char*)D_004A28A8 + 0x8C);
        sVEntryK1CE5E0* vt = *(sVEntryK1CE5E0**)(o + 4);
        void* str = vt[4].fn(o + vt[4].delta, cBEOptionInterface_getDefaultQuickKeyMessageHashValue(opt, key));
        if (str)
            func_003DCB20(buf, str);
        else
            buf[0] = 0;
    } else {
        strcpy(buf, msg);
    }
    buf[room] = 0;
    func_004162D0((char*)self + 0x74, buf);
    int len = strlen((char*)self + 0x74);
    int n = len - 1;
    *(int*)((char*)self + 0x134) = n;
    if (n < *(int*)((char*)self + 0x60) - 1)
        *(int*)((char*)self + 0x134) = len;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE6F0);
#ifdef SKIP_ASM
struct sPopupSlot_E6F0 {
    signed char id;
    signed char pad;
};

extern sPopupSlot_E6F0 D_0045AAD8[][4][5];

extern "C" int func_001CE6F0(int a, int b)
{
    int n = 0;
    for (int i = 0; i < 5; i++) {
        if (D_0045AAD8[a][b][i].id >= 0) {
            n++;
        } else {
            break;
        }
    }
    return n;
}
#endif

