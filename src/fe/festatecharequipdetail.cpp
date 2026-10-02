#include "common.h"

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cFEStateCharEquip);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199148);

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onCreateScreen);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199350);

INCLUDE_ASM("fe/festatecharequipdetail", func_001993A0);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199420);

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onWidgetCreate);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199870);

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onUpdate);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199A38);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199C28);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199F20);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A098);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A238);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A308);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A3D0);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A498);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A4E8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A638);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A798);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A9B8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019AA08);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019AB78);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019ACA0);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019AFD0);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019B098);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019B180);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B458);
#ifdef SKIP_ASM
struct sEquipListWidget {
    char pad_0x0[0x95];
    unsigned char sel;   // 0x95
    char pad_0x96[2];
    unsigned char top;   // 0x98
};

struct sEquipDetail {
    char pad_0x0[0x5C];
    sEquipListWidget* list;   // 0x5C
    char pad_0x60[0x124 - 0x60];
    int items[(0x958 - 0x124) / 4];  // 0x124
    int count;   // 0x958
};

extern "C" int func_0019B458(sEquipDetail* self, int idx)
{
    int top, sel, n;
    if (idx >= self->count) {
        return 0;
    }
    for (; idx < self->count - 1; idx++) {
        self->items[idx] = self->items[idx + 1];
    }
    self->count--;
    self->items[self->count] = 0;
    n = self->count;
    top = self->list->top;
    sel = self->list->sel;
    if (n < top + 6) {
        top = n - 6;
    }
    if (sel >= n) {
        sel = n - 1;
    }
    if (top < 0) {
        top = 0;
    }
    self->list->top = top;
    if (sel < 0) {
        sel = 0;
    }
    self->list->sel = sel;
    return 1;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019B518);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019B598);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019B618);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019B7E0);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BA60);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019BBA0);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* func_00198AF0(void* a0);

extern "C" void func_0019BBA0(void* self)
{
    if (*(cUIText**)((char*)self + 0x64) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x64), func_00198AF0(*(void**)((char*)self + 0xC0)));
    }
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateHeading);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BC90);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BD48);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BE80);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BEE8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BFE8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019C7E8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019C880);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C8D0);
#ifdef SKIP_ASM
extern "C" void* func_0019C8D0(void* self, void* item)
{
    if (item != 0) {
        short idx = *(short*)((char*)item + 0xc);
        if (idx >= 0) {
            return (char*)self + (idx << 4);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C8F8);
#ifdef SKIP_ASM
extern "C" void* func_0019C8F8(void* self, void* item)
{
    if (item != 0) {
        short idx = *(short*)((char*)item + 0xe);
        if (idx >= 0) {
            return (char*)self + (idx << 4);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C920);
#ifdef SKIP_ASM
extern "C" int func_0019C920(void* self, int a1)
{
    int diff = a1 - (int)self;
    if (a1 != 0) return diff >> 4;
    return -1;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019C938);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CA70);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CB38);
#ifdef SKIP_ASM
extern "C" void func_0019CB60(void* self);

extern "C" void* func_0019CB38(void* self)
{
    func_0019CB60(self);
    return self;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CB60);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CBE0);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);

extern "C" int func_0019CBE0(void* self, int a1, int a2)
{
    void* p = func_0019C938(self, a1, a2, 0);
    if (p != 0) {
        if ((*(short*)((char*)p + 6) >> a1) & 1) {
            return *(short*)((char*)p + 0xA);
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CC30);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CCE8);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);

extern "C" int func_0019CCE8(void* self, int a1, int a2)
{
    void* p = func_0019C938(self, a1, a2, 0);
    if (p != 0) {
        if ((*(short*)((char*)p + 4) >> a1) & 1) {
            return *(short*)((char*)p + 0x8);
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CD38);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CDF0);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CE20);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CE68);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019CF40);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D000);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D140);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D250);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D3B0);
#ifdef SKIP_ASM
struct sEquipSlot_D3B0 {
    int used;               // 0x000
    char pad_0x4[0x100];
    int valid;              // 0x104
    char pad_0x108[0x14];
    unsigned int stamp;     // 0x11C
};

extern "C" int func_0019D3B0(void* self, int bank)
{
    unsigned int best = 0;
    int bestIdx = -1;
    sEquipSlot_D3B0* p = (sEquipSlot_D3B0*)((char*)self + bank * 0x12000);
    for (int i = 0; i < 256; i++, p++) {
        if (p->used == 0) {
            if (p->valid == 0) {
                return i;
            }
            if (bestIdx < 0 || p->stamp < best) {
                best = p->stamp;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D428);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D4B8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D578);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D738);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D8B8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019DA10);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019DC20);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019DD10);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019DE18);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019DF20);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019E070);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E238);
#ifdef SKIP_ASM
struct sEquipSlot120 {
    int state;
    char pad04[0x100];
    int value;
    char pad108[0x18];
};
struct sEquipRow12000 {
    sEquipSlot120 slots[0x100];
};
struct sEquipTable {
    sEquipRow12000 rows[1];
};

extern "C" int func_0019E238(sEquipTable* self, int a1, int a2)
{
    return self->rows[a1].slots[a2].state == 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E2B0);
#ifdef SKIP_ASM
extern "C" int func_0019E2B0(sEquipTable* self, int a1, int a2)
{
    if (self->rows[a1].slots[a2].state != 1) {
        return 0;
    }
    return self->rows[a1].slots[a2].value;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019E3D0);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019E498);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E538);
#ifdef SKIP_ASM
extern "C" void func_0019E538(void* self, int a1)
{
    if (a1 != 0) {
        if (*(int*)((char*)self + 0xcc0) != 0 && *(int*)((char*)self + 0xcb8) != 0 &&
            *(int*)((char*)self + 0xcb4) != 0 && *(int*)((char*)self + 0xcd4) < 0) {
            *(int*)((char*)self + 0xcc8) = 1;
        } else {
            *(int*)((char*)self + 0xcc4) = 1;
        }
    } else {
        *(int*)((char*)self + 0xcc8) = 0;
        *(int*)((char*)self + 0xcc4) = 0;
    }
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019E588);

