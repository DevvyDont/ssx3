#include "common.h"

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cFEStateCharEquip);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199148);

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onCreateScreen);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_00199350);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void* func_001A0548(void* self, int a1);
void* func_0039E4A0(void* self);

extern "C" void func_00199350(void* self)
{
    char* mgr = *(char**)((char*)D_004A28A8 + 0x7C);
    if (mgr != 0) {
        char* r = (char*)func_001A0548(mgr + 0xB0, *(signed char*)((char*)self + 0x44));
        *(unsigned int*)(r + 0xC2C) = 0xFFFFFFFF;
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_001993A0);
#ifdef SKIP_ASM
extern "C" void func_0019A308(void* self);
extern "C" void func_0019A238(void* self);
extern "C" void func_0019BC90(void* self);
extern "C" void func_0019BD48(void* self, int a1);
extern "C" void func_0019B618(void* self, int a1, int a2);
extern "C" void func_00186518(void* self, int a1);

extern "C" void func_001993A0(void* self, int a1)
{
    bool off = !*(bool*)((char*)self + 0xC8);
    if (!off) {
        func_0019A308(self);
    } else {
        func_0019A238(self);
    }
    func_0019BC90(self);
    func_0019BD48(self, 0);
    func_0019B618(self, 1, 1);
    func_00186518(self, a1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_00199420);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019E538(void* rider, int a1);

extern "C" void func_00199420(void* self)
{
    func_0019E538(func_001A0548(*(char**)((char*)D_004A28A8 + 0x7C) + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_00199870);
#ifdef SKIP_ASM
extern "C" void func_0019BE80(void* self);
extern "C" void func_0019BBA0(void* self);
struct sEquipDetail;
extern "C" int func_0019B458(sEquipDetail* self, int idx);
extern "C" void func_0019B618(void* self, int a1, int a2);

extern "C" void func_00199870(void* self, void* widget, int msg)
{
    switch (msg) {
    case 0x15:
        if (*(int*)((char*)widget + 0xC) != 7) {
            *(int*)((char*)self + 0x1C) &= ~8;
        }
        break;
    case 0x16:
        if (((*(int*)((char*)self + 0x1C) >> 3) & 1) == 0) {
            *(int*)((char*)self + 0x1C) |= 8;
        }
        if (*(int*)((char*)widget + 0xC) == 7 && *(int*)((char*)widget + 0x6C) != 0) {
            func_0019BE80(self);
            func_0019BBA0(self);
            if (func_0019B458((sEquipDetail*)self, *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x95))) {
                func_0019B618(self, 0, 1);
            }
        }
        break;
    }
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onUpdate);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199A38);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199C28);

INCLUDE_ASM("fe/festatecharequipdetail", func_00199F20);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A098);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A238);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A308);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A3D0);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019A498);
#ifdef SKIP_ASM
extern void* D_004A28A8;
struct sEquipTable_DC20;
extern "C" void func_0019DC20(sEquipTable_DC20* self, int bank, int i);

extern "C" void func_0019A498(void* self)
{
    int idx = *(int*)((char*)self + 0xA6C);
    if (idx >= 0) {
        func_0019DC20((sEquipTable_DC20*)(*(char**)((char*)D_004A28A8 + 0x7C) + 0x1A70), *(int*)((char*)self + 0xBC), idx);
    }
    *(char*)((char*)self + 0x960) = 0;
    *(int*)((char*)self + 0xA6C) = -1;
    *(int*)((char*)self + 0x95C) = -1;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A4E8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A638);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019A798);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019A9B8);
#ifdef SKIP_ASM
extern "C" void func_0019AB78(void* self);
extern "C" void func_0019AA08(void* self);
extern "C" void func_0019A798(void* self);

extern "C" void func_0019A9B8(void* self)
{
    bool b = *(int*)((char*)self + 0xC8) == 1;
    if (b) {
        func_0019AB78(self);
    } else {
        func_0019AA08(self);
    }
    func_0019A798(self);
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019AA08);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019AB78);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019ACA0);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" void func_0019FBE0(void* rider, void* item, int a2, int a3);

extern "C" void func_0019ACA0(void* self)
{
    void* rider = func_001A0548(*(char**)((char*)D_004A28A8 + 0x7C) + 0xB0, *(signed char*)((char*)self + 0x44));
    func_0019FBE0(rider, func_0014AD28(cBE_getInterface_Fv(cBE_getBE(), 9), *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xBC)), 0, 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019AFD0);
#ifdef SKIP_ASM
struct sEquipEntry_AFD0 {
    unsigned short f0;
    unsigned short flags;
};
struct sEquipDb_AFD0 {
    char pad_0x0[0x288];
    short* map;     // 0x288
    char pad_0x28c[4];
    sEquipEntry_AFD0 entries[1];  // 0x290
};
struct sEquipItem_AFD0 {
    int f0;
    short id;       // 0x4
    char pad_0x6[0x34 - 0x6];
    int flags;      // 0x34
};
extern "C" int func_0014D7E8(int a0, int a1, int a2, void* out, int a4, int a5);

static inline sEquipEntry_AFD0* getEntry_AFD0(sEquipDb_AFD0* db, int id)
{
    short idx = db->map[id];
    if (idx >= 0) {
        return &db->entries[idx];
    }
    return 0;
}

extern "C" int func_0019AFD0(void* self, int a1, sEquipDb_AFD0* db, int a3)
{
    sEquipItem_AFD0* items[528];
    int n = func_0014D7E8(a1, *(int*)((char*)self + 0xBC), a3, items, -1, 1);
    int i;
    for (i = 0; i < n; i++) {
        sEquipItem_AFD0* it = items[i];
        if (it->flags & 4) {
            if (getEntry_AFD0(db, it->id)->flags & 2) {
                if (!(it->flags & 0x20)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B098);
#ifdef SKIP_ASM
struct sEquipEntry_B098 {
    unsigned short f0;
    unsigned short flags;
};
struct sEquipDb_B098 {
    char pad_0x0[0x288];
    short* map;     // 0x288
    char pad_0x28c[4];
    sEquipEntry_B098 entries[1];  // 0x290
};
struct sEquipItem_B098 {
    short f0;
    signed char slot;   // 0x2
    char f3;
    short id;       // 0x4
    char pad_0x6[0x34 - 0x6];
    int flags;      // 0x34
};
extern "C" int func_0014D7E8(int a0, int a1, int a2, void* out, int a4, int a5);

static inline sEquipEntry_B098* getEntry_B098(sEquipDb_B098* db, int id)
{
    short idx = db->map[id];
    if (idx >= 0) {
        return &db->entries[idx];
    }
    return 0;
}

extern "C" int func_0019B098(void* self, int a1, sEquipDb_B098* db, int a3)
{
    sEquipItem_B098* items[528];
    int n = func_0014D7E8(a1, *(int*)((char*)self + 0xBC), a3, items, -1, 1);
    int i;
    for (i = 0; i < n; i++) {
        sEquipItem_B098* it = items[i];
        if (it->flags & 4) {
            if (!(getEntry_B098(db, it->id)->flags & 2)) {
                if (it->slot == *(int*)((char*)self + 0xCC)) {
                    if (!(it->flags & 0x20)) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B518);
#ifdef SKIP_ASM
extern "C" void func_0019B7E0(void* self, int idx);

extern "C" int func_0019B518(void* self)
{
    int j = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x98);
    int i = 0;
    while (i < 6 && j < *(int*)((char*)self + 0x958)) {
        func_0019B7E0(self, i);
        i++;
        j++;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B598);
#ifdef SKIP_ASM
extern "C" void func_0019BA60(void* self, int idx);

extern "C" int func_0019B598(void* self)
{
    int j = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x98);
    int i = 0;
    while (i < 6 && j < *(int*)((char*)self + 0x958)) {
        func_0019BA60(self, i);
        i++;
        j++;
    }
    return i;
}
#endif

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

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019BE80);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014B560(void* iface, int a1, int a2, int a3, int a4);

extern "C" void func_0019BE80(void* self)
{
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x95);
    char* item = *(char**)((char*)self + (idx << 2) + 0x124);
    *(void**)((char*)self + 0xC0) = func_0014B560(cBE_getInterface_Fv(cBE_getBE(), 9),
        *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xBC), *(short*)(item + 4), 1);
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BEE8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019BFE8);

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C7E8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001CABA8(void* self, void* engine, void* owner, signed char idx);
extern "C" void cBuyPopupInfo_initBuyBolt(int* self, int a1, int a2, int a3);
extern "C" void func_0039F290(void* list, void* item);
extern char D_00460790[];

extern "C" void func_0019C7E8(void* self, void* item)
{
    void* popup = func_001CABA8(cMemMan_alloc(0x70, D_00460790, 0, 0), *(void**)((char*)self + 0x10), self, *(signed char*)((char*)self + 0x44));
    int price = *(short*)((char*)item + 0xE);
    int id = *(int*)((char*)item + 0x14);
    if (price > 0) {
        price *= 10;
    }
    cBuyPopupInfo_initBuyBolt((int*)((char*)popup + 0x48), id, price, *(int*)((char*)self + 0xC0));
    func_0039F290(*(char**)((char*)self + 0x10) + 0x18, popup);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C880);
#ifdef SKIP_ASM
extern "C" void* func_0019C8F8(void* self, void* item);

extern "C" void* func_0019C880(void* self)
{
    char* item = *(char**)((char*)self + 0x1F40);
    *(void**)((char*)self + 0x1F40) = func_0019C8F8(self, item);
    *(short*)(item + 0xE) = -1;
    char* head = *(char**)((char*)self + 0x1F40);
    if (head != 0) {
        *(short*)(head + 0xC) = -1;
    }
    return item;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CA70);
#ifdef SKIP_ASM
struct sNode_CA70 {
    int key;
    short f4;
    short f6;
    short f8;
    short fA;
    short prev;
    short next;
};
extern "C" void* func_0019C8D0(void* self, void* item);
extern "C" void* func_0019C8F8(void* self, void* item);
extern "C" int func_0019C920(void* self, int a1);

extern "C" void func_0019CA70(void* self, void* nodep)
{
    sNode_CA70* node = (sNode_CA70*)nodep;
    if (node != 0) {
        int h = node->key % 400;
        if (node->prev < 0) {
            *(void**)((char*)self + (h << 2) + 0x1900) = func_0019C8F8(self, node);
        } else {
            ((sNode_CA70*)func_0019C8D0(self, node))->next = node->next;
        }
        if (node->next >= 0) {
            ((sNode_CA70*)func_0019C8F8(self, node))->prev = node->prev;
        }
        node->key = -1;
        node->fA = -1;
        node->f8 = -1;
        node->prev = -1;
        // PORT: free-list head pointer passed as int (unit declares func_0019C920(void*, int))
        node->next = func_0019C920(self, *(int*)((char*)self + 0x1F40));
        *(sNode_CA70**)((char*)self + 0x1F40) = node;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CB60);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, unsigned int value, int size);

struct sNode0019CB60 {
    int field_0x0;
    short field_0x4;
    short field_0x6;
    int field_0x8;
    short prev;
    short next;
};

extern "C" void func_0019CB60(void* self)
{
    func_003E6448(self, 0xFFFFFFFF, 0x1900);
    func_003E6448((char*)self + 0x1900, 0, 0x640);
    *(void**)((char*)self + 0x1F40) = self;
    sNode0019CB60* n = (sNode0019CB60*)self;
    for (int i = 0; i < 400; i++) {
        n[i].next = i + 1;
        n[i].prev = i - 1;
        n[i].field_0x4 = 0;
        n[i].field_0x6 = 0;
    }
    n[399].next = -1;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CC30);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);

extern "C" void func_0019CA70(void* self, void* node);
struct sNode_CC30 {
    int key;
    short f4;
    short mask;
    short f8;
    short val;
    short prev;
    short next;
};

extern "C" void func_0019CC30(void* self, int bit, int a2, int val)
{
    sNode_CC30* p = (sNode_CC30*)func_0019C938(self, bit, a2, val != -1);
    if (p != 0) {
        if (val < 0) {
            if ((p->mask >> bit) & 1) {
                p->mask &= ~(1 << bit);
                if (p->mask == 0 && p->f8 < 0) {
                    func_0019CA70(self, p);
                }
            }
        } else {
            p->val = val;
            p->mask |= 1 << bit;
        }
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CD38);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);
extern "C" void func_0019CA70(void* self, void* node);
struct sNode_CD38 {
    int key;
    short mask;
    short f6;
    short val;
    short fA;
    short prev;
    short next;
};

extern "C" void func_0019CD38(void* self, int bit, int a2, int val)
{
    sNode_CD38* p = (sNode_CD38*)func_0019C938(self, bit, a2, val != -1);
    if (p != 0) {
        if (val < 0) {
            if ((p->mask >> bit) & 1) {
                p->mask &= ~(1 << bit);
                if (p->mask == 0 && p->fA < 0) {
                    func_0019CA70(self, p);
                }
            }
        } else {
            p->val = val;
            p->mask |= 1 << bit;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CDF0);
#ifdef SKIP_ASM
extern void* D_004A196C;
extern "C" void func_0019CE68(void* self);

extern "C" void* func_0019CDF0(void* self)
{
    D_004A196C = self;
    func_0019CE68(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CE20);
#ifdef SKIP_ASM
extern void* D_004A196C;
extern "C" void func_0019CF40(void* self);
void operator_delete(int* p);

extern "C" void func_0019CE20(void* self, int flags)
{
    D_004A196C = 0;
    func_0019CF40(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D428);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

struct sEquipSlot_D428 {
    int state;
    char name[0x100];
    int value;
    char pad108[0x18];
};
struct sEquipRow_D428 {
    sEquipSlot_D428 slots[0x100];
};
struct sEquipTable_D428 {
    sEquipRow_D428 rows[1];
};

extern "C" int func_0019D428(sEquipTable_D428* self, int bank, const char* name)
{
    for (int i = 0; i < 256; i++) {
        sEquipSlot_D428* s = &self->rows[bank].slots[i];
        if (s->name[0] != 0 && func_004165A8(self->rows[bank].slots[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D4B8);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
void cMemMan_free(void* ptr);
struct sEquipSlot_D4B8 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int f114;
    int size;       // 0x118
    int f11C;
};
struct sEquipRow_D4B8 {
    sEquipSlot_D4B8 slots[0x100];
};
struct sEquipTable_D4B8 {
    sEquipRow_D4B8 rows[1];
};

extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i)
{
    int status;
    sEquipSlot_D4B8* s = &self->rows[bank].slots[i];
    if (s->state == 2) {
        if (s->pending != 0) {
            ASYNCFILE_release(s->handle, 0, &status);
        }
    }
    *(int*)((char*)self + 0xB4000) -= s->size;
    if (s->data != 0) {
        cMemMan_free(s->data);
    }
    s->data = 0;
    s->size = 0;
    s->state = 0;
    s->name[0] = 0;
    s->handle = 0;
    s->pending = 0;
    s->f114 = 0;
    s->f110 = -1;
}
#endif

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D578);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D738);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019D8B8);

INCLUDE_ASM("fe/festatecharequipdetail", func_0019DA10);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019DC20);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
extern "C" int func_003DF980(int handle);
struct sEquipSlot_DC20 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
    int f11C;
};
struct sEquipRow_DC20 {
    sEquipSlot_DC20 slots[0x100];
};
struct sEquipTable_DC20 {
    sEquipRow_DC20 rows[1];
};
extern "C" void func_0019D000(sEquipTable_DC20* self, int bank, sEquipSlot_DC20* slot);
struct sEquipTable_D4B8;
extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i);

extern "C" void func_0019DC20(sEquipTable_DC20* self, int bank, int i)
{
    int status;
    sEquipSlot_DC20* s = &self->rows[bank].slots[i];
    if (--s->timer > 0) {
        return;
    }
    if (s->state == 2) {
        if (func_003DF980(s->handle) != 1) {
            return;
        }
        ASYNCFILE_release(s->handle, 0, &status);
        func_0019D000(self, bank, s);
        s->pending = 0;
        s->state = 0;
        *(int*)((char*)self + 0xB4004) -= 1;
    }
    if (s->data == 0) {
        func_0019D4B8((sEquipTable_D4B8*)self, bank, i);
    } else {
        s->state = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E498);
#ifdef SKIP_ASM
struct sVEntry0019E498 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sSlot0019E498 {
    int id;
    char flag;
    char pad[7];
};

extern void* D_004A289C;
extern "C" void func_0019EBA0(void* self);
void operator_delete(int* p);

extern "C" void func_0019E498(void* self, int flags)
{
    sSlot0019E498* s = (sSlot0019E498*)((char*)self + 0x1C);
    func_0019EBA0(self);
    int i;
    int none = -1;
    for (i = 0xFF; i >= 0; i--, s++) {
        if (s->id >= 0) {
            char* g = (char*)D_004A289C;
            sVEntry0019E498* vt = *(sVEntry0019E498**)(g + 0x10D8);
            vt[50].fn(g + vt[50].delta, s->id);
        }
        s->id = none;
        s->flag = 0;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

