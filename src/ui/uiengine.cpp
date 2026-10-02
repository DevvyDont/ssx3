#include "common.h"

//100%
INCLUDE_ASM("ui/uiengine", UIAsciiToUnicode__FPUsPCc);
#ifdef SKIP_ASM
void UIAsciiToUnicode(unsigned short* dst, const char* src)
{
    for (; *src != 0; src++, dst++) {
        *dst = (char)*(unsigned char*)src;
    }
    *dst = 0;
}
#endif

INCLUDE_ASM("ui/uiengine", func_00397B08);

INCLUDE_ASM("ui/uiengine", func_00397B70);

INCLUDE_ASM("ui/uiengine", cUIEngine_loadFile);

INCLUDE_ASM("ui/uiengine", cUIEngine_addScreenByHashName);

INCLUDE_ASM("ui/uiengine", func_00397DF8);

void func_0039ECA8(void*);

//100%
INCLUDE_ASM("ui/uiengine", func_00398018);
#ifdef SKIP_ASM
extern "C" void func_00398018(void* self)
{
    func_0039ECA8((char*)self + 0x18);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", func_00398038);
#ifdef SKIP_ASM
struct sVEntry398038 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0039F100(void*);

extern "C" void func_00398038(void* self)
{
    void* obj = *(void**)((char*)self + 0xC);
    sVEntry398038* vt = *(sVEntry398038**)((char*)obj + 8);
    vt[0x2E].fn((char*)obj + vt[0x2E].delta);
    func_0039F100((char*)self + 0x18);
}
#endif

INCLUDE_ASM("ui/uiengine", func_00398078);

INCLUDE_ASM("ui/uiengine", cUITextureBank_setData);

//100%
INCLUDE_ASM("ui/uiengine", func_00398380);
#ifdef SKIP_ASM
struct s398380Item {
    char pad[0x1C];
    int id;
};

extern "C" s398380Item* func_00398380(void* self, int id)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x10); i++) {
        s398380Item* item = &(*(s398380Item**)((char*)self + 0xC))[i];
        if (item->id == id) {
            return item;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", func_003983F0);
#ifdef SKIP_ASM
struct s3983F0Entry {
    int key;
    int unk4;
    int value;
};

struct s3983F0 {
    int unk0;
    s3983F0Entry entries[4];
    signed char count;
};

extern "C" int func_003983F0(s3983F0* self, int key)
{
    signed char i;
    for (i = 0; i < self->count; i++) {
        s3983F0Entry* e = &self->entries[i];
        if (e->key == key) {
            return e->value;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("ui/uiengine", func_00398438);

INCLUDE_ASM("ui/uiengine", func_003984B0);

INCLUDE_ASM("ui/uiengine", cUIFontInterface_loadFonts);

extern "C" void* func_0039FE00(void* self);

//100%
INCLUDE_ASM("ui/uiengine", func_00398618__FPv);
#ifdef SKIP_ASM
void* func_00398618(void* self)
{
    return func_0039FE00(self);
}
#endif

INCLUDE_ASM("ui/uiengine", func_00398638);

INCLUDE_ASM("ui/uiengine", func_003986B0);

//100%
INCLUDE_ASM("ui/uiengine", func_00398738);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct func_00398738_sVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00398738(void* self)
{
    void* n = cList_first((cList*)((char*)self + 0x74));
    if (n != 0) {
        do {
            func_00398738_sVEntry* vt = *(func_00398738_sVEntry**)((char*)n + 8);
            vt[0x10].fn((char*)n + vt[0x10].delta);
            n = *(void**)((char*)n + 4);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", func_00398798);
#ifdef SKIP_ASM
extern void* D_00494BB8[];
extern void* D_00494CE8[];

struct func_00398798_sFunctor {
    void** vt;
    int arg;
};

extern "C" int func_003979F8(void* list, func_00398798_sFunctor* fn);

extern "C" int func_00398798(void* self, int a1)
{
    if ((*(int*)((char*)self + 0x14) >> 5) & 1) {
        return 0;
    }
    func_00398798_sFunctor f;
    f.vt = D_00494CE8;
    f.vt = D_00494BB8;
    f.arg = a1;
    int r = func_003979F8((char*)self + 0x74, &f);
    f.vt = D_00494CE8;
    return r != 0;
}
#endif

INCLUDE_ASM("ui/uiengine", func_003987F8);

INCLUDE_ASM("ui/uiengine", func_00398868);

INCLUDE_ASM("ui/uiengine", func_00398910);

INCLUDE_ASM("ui/uiengine", func_00398998);

//100%
INCLUDE_ASM("ui/uiengine", func_00398A60__FPvT0);
#ifdef SKIP_ASM
void func_00398A60(void* a, void* b)
{
    float t = *(float*)a;
    *(float*)a = *(float*)b;
    *(float*)b = t;
}
#endif

INCLUDE_ASM("ui/uiengine", func_00398A78);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uiengine", func_00399730);
#ifdef SKIP_ASM
extern "C" void func_00399730(void* self, int id)
{
    void* a = *(void**)((char*)self + 0x5C);
    void* b = *(void**)((char*)a + 0xD0);
    void* c = *(void**)((char*)b + 0x10);
    *(s398380Item**)((char*)self + 0x7C) = func_00398380((char*)c + 0x58, id);
}
#endif

INCLUDE_ASM("ui/uiengine", func_00399768);

INCLUDE_ASM("ui/uiengine", func_00399820);

//100%
INCLUDE_ASM("ui/uiengine", func_00399920);
#ifdef SKIP_ASM
void* func_0039FF50(void*);

struct func_00399920_sEntry {
    int a;
    int b;
    int c;
};

extern "C" int func_00399920(void* self)
{
    void* o = func_0039FF50(self);
    if (o != 0) {
        func_00399920_sEntry* t = *(func_00399920_sEntry**)((char*)o + 0x8);
        return t[(*(unsigned char*)((char*)self + 0x74) & 3) + 1].a;
    }
    return 0;
}
#endif

INCLUDE_ASM("ui/uiengine", func_00399970);

INCLUDE_ASM("ui/uiengine", func_00399D80);

INCLUDE_ASM("ui/uiengine", func_00399E28);

INCLUDE_ASM("ui/uiengine", func_00399F00);

