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

INCLUDE_ASM("ui/uiengine", func_00398038);

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

INCLUDE_ASM("ui/uiengine", func_00398738);

INCLUDE_ASM("ui/uiengine", func_00398798);

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

INCLUDE_ASM("ui/uiengine", func_00399920);

INCLUDE_ASM("ui/uiengine", func_00399970);

INCLUDE_ASM("ui/uiengine", func_00399D80);

INCLUDE_ASM("ui/uiengine", func_00399E28);

INCLUDE_ASM("ui/uiengine", func_00399F00);

