#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWorldTriggerManager_cWorldTriggerManager(void* self);
extern const char D_00483588[];
extern void* D_004A52D4;

//99.88%
INCLUDE_ASM("sound/icepick/worldtriggermanager", WORLDTRIGGERMANAGER_Init__Fv);
#ifdef SKIP_ASM
void WORLDTRIGGERMANAGER_Init()
{
    if (D_004A52D4 == 0) {
        void* mem = cMemMan_alloc(0x344, D_00483588, 0, 0);
        D_004A52D4 = cWorldTriggerManager_cWorldTriggerManager(mem);
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4B48);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4B70);
#ifdef SKIP_ASM
extern unsigned char D_004A35A8[];

extern "C" void* func_002B4B70(void* self)
{
    *(int*)((char*)self + 0x30) = -1;
    *(float*)((char*)self + 0x2C) = 1.0f;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x34) = 0;
    *(int*)((char*)self + 0x38) = 0;
    *(unsigned char*)((char*)self + 0x40) = D_004A35A8[0];
    *(float*)((char*)self + 0x68) = -1.0f;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x6C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4BE0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_002B4BE0(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4C08);
#ifdef SKIP_ASM
extern "C" int func_002B4C08(void* self, int a1, int a2, int a3)
{
    if (a1 == *(int*)((char*)self + 0x4)
        && a2 == *(int*)((char*)self + 0x8)
        && a3 == *(int*)((char*)self + 0x3c)) {
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4C38);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5758);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002A72D8(void* snd, int id, float t);
extern "C" void func_002AD5F0(void* self, int idx, int a2, float v);
extern unsigned char D_004A35A8[];
struct sWorldTrigger70;

extern "C" void func_002B5758(sWorldTrigger70* t)
{
    char* self = (char*)t;
    if (*(int*)((char*)self + 0x38) != 0) {
        if (*(int*)((char*)self + 0x30) != -9999) {
            func_002A72D8(func_0028B180(), *(int*)((char*)self + 0x30), 0.25f);
        }
    } else if (*(int*)((char*)self + 0x30) != -9999) {
        func_002AD5F0(**(char***)((char*)func_0028B180() + 0x118) + 0x1D8, *(int*)((char*)self + 0x30), 1, 0.25f);
    }
    *(int*)((char*)self + 0x30) = -1;
    *(unsigned char*)((char*)self + 0x40) = D_004A35A8[0];
    *(float*)((char*)self + 0x68) = -1.0f;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x6C) = 0;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5818);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5838);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5878);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5898);
#ifdef SKIP_ASM
extern unsigned char D_004A35A8[];

extern "C" void func_002B5898(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x30) = -1;
    *(int*)((char*)self + 0x34) = 0;
    *(unsigned char*)((char*)self + 0x40) = D_004A35A8[0];
    *(float*)((char*)self + 0x68) = -1.0f;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x6c) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_cWorldTriggerManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* func_002B4B70(void* self);
extern "C" void cWorldTriggerManager_LoadTriggerInfo(void* self);
extern const char D_004835A0[];

struct sWorldTrigger58D0 {
    char pad[0x70];
};

extern "C" void* cWorldTriggerManager_cWorldTriggerManager(void* self)
{
    *(int*)((char*)self + 0x324) = 0;
    *(int*)((char*)self + 0x330) = 0;
    *(int*)((char*)self + 0x328) = -1;
    *(int*)((char*)self + 0x32C) = -1;
    *(int*)((char*)self + 0x334) = 1;
    int* mem = (int*)operator_new_tag(0x1190, D_004835A0, 0, 0);
    mem[0] = 40;
    sWorldTrigger58D0* arr = (sWorldTrigger58D0*)((char*)mem + 0x10);
    sWorldTrigger58D0* p = arr;
    for (int i = 39; i != -1; i--, p++) {
        func_002B4B70(p);
    }
    *(sWorldTrigger58D0**)self = arr;
    *(int*)((char*)self + 0x338) = 0;
    cWorldTriggerManager_LoadTriggerInfo(self);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5988);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);

extern "C" void func_002B5988(char** self, int flags)
{
    char* base = *self;
    if (base != 0) {
        char* p = base + *(int*)(base - 0x10) * 0x70;
        while (*self != p) {
            p -= 0x70;
            func_002B4BE0((int*)p, 0);
        }
        cMemMan_free(*self - 0x10);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_LoadTriggerInfo);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5B90);
#ifdef SKIP_ASM
struct sWorldTrigger70 {
    int active;
    char pad[0x30];
    int unk34;
    char pad2[0x38];
};

extern "C" sWorldTrigger70* func_002B5B90(sWorldTrigger70** self)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 0) {
            return &(*self)[i];
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5BC0);
#ifdef SKIP_ASM
extern "C" int func_002B4C08(void* self, int a1, int a2, int a3);

extern "C" sWorldTrigger70* func_002B5BC0(sWorldTrigger70** self, int a, int b, int c)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active != 0 && func_002B4C08(&(*self)[i], a, b, c)) {
            return &(*self)[i];
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5C68);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5D78);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" int func_004165A8(const void* a, const void* b);
// PORT: func_0029B960__FPv is an empty stub whose caller passes (mgr, name).
void func_0029B960_2(void* mgr, void* name) __asm__("func_0029B960__FPv");
extern "C" void func_002B4C38(sWorldTrigger70* t);
extern "C" void func_002B5758(sWorldTrigger70* t);
extern unsigned char D_004A35A8[];

extern "C" void func_002B5D78(sWorldTrigger70** self)
{
    if (*(int*)((char*)func_0028B180() + 0x5FB0) != 0) return;
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1) {
            if ((*self)[i].unk34 == 0) {
                if (func_004165A8((char*)&(*self)[i] + 0x40, D_004A35A8)) {
                    // PORT: pointer arithmetic through int
                    func_0029B960_2(func_0028B180(), (char*)(i * 0x70 + *(int*)self) + 0x40);
                }
                (*self)[i].active = 0;
                func_002B5758(&(*self)[i]);
            } else {
                func_002B4C38(&(*self)[i]);
            }
            (*self)[i].unk34 = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5E68);
#ifdef SKIP_ASM
extern "C" void func_002B5758(sWorldTrigger70* t);

extern "C" void func_002B5E68(void* self)
{
    for (int i = 0; i < 40; i++) {
        if ((*(sWorldTrigger70**)self)[i].active == 1) {
            (*(sWorldTrigger70**)self)[i].active = 0;
            func_002B5758(&(*(sWorldTrigger70**)self)[i]);
        }
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5F60);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B63D0);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6550);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" int func_002A34D0(void* mgr, int a1, int a2, int a3);

extern "C" int func_002B6550(void* self, int type, int* a, int* b, int* c, int arg)
{
    int r = 0;
    *a = 0;
    *b = 0;
    *c = 0;
    if (type == 0x4D) {
        *c = 1;
        r = func_002A34D0(func_0028B180(), 0, 1, arg);
    }
    return r;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B65B0);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6628__FPv);
#ifdef SKIP_ASM
int func_002B6628(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6630);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);
extern unsigned char D_004A35A8[];

extern "C" int func_002B6630(sWorldTrigger70** self, int id)
{
    int n = 0;
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1 && func_004165A8((char*)&(*self)[i] + 0x40, D_004A35A8)) {
            if (*(int*)((char*)&(*self)[i] + 0xC) != id) n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B66E8);
#ifdef SKIP_ASM
extern "C" int func_002B66E8(sWorldTrigger70** self, int a1, int a2, sWorldTrigger70* except)
{
    for (int i = 0; i < 40; i++) {
        sWorldTrigger70* t = &(*self)[i];
        if (t->active == 1 && t != except
            && *(int*)((char*)t + 0x4) == a1
            && *(int*)((char*)t + 0x8) == a2) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6740);
#ifdef SKIP_ASM
extern "C" void* func_0028B1C8(void);
extern "C" void func_002B8190(void* self, void* obj, int* o0, int* o1, int* o2, int* o3);
extern "C" int func_002B67D8(void* self, unsigned int i);

extern "C" int func_002B6740(void* self, void* obj)
{
    int ids[4];
    func_002B8190(*(void**)((char*)func_0028B1C8() + 0x4C), obj, &ids[0], &ids[1], &ids[2], &ids[3]);
    if (func_002B67D8(self, ids[0])) {
        return 1;
    }
    if (func_002B67D8(self, ids[1])) {
        return 1;
    }
    if (func_002B67D8(self, ids[2])) {
        return 1;
    }
    return func_002B67D8(self, ids[3]) != 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B67D8);
#ifdef SKIP_ASM
struct sTriggerRef {
    int id;
    void* obj;
};

extern "C" int func_002B67D8(void* self, unsigned int i)
{
    if (i < 2) {
        return 0;
    }
    sTriggerRef* refs = *(sTriggerRef**)((char*)self + 0x33c);
    return *(int*)((char*)refs[i].obj + 0x4) == 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6808);
#ifdef SKIP_ASM
struct sTriggerIdCache {
    int unk0;
    int ids[200];       // 0x4
    int count;          // 0x324
    int unk328;
    int lastId;         // 0x32C
    int lastResult;     // 0x330
};

extern "C" int func_002B6808(sTriggerIdCache* self, int id)
{
    if (id == self->lastId) {
        return self->lastResult;
    }
    int i = self->count - 1;
    self->lastId = id;
    for (; i >= 0; i--) {
        if (self->ids[i] == id) {
            self->lastResult = 1;
            return 1;
        }
    }
    self->lastResult = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6868);
#ifdef SKIP_ASM
extern "C" void func_002B6868(sTriggerIdCache* self, int id)
{
    if (func_002B6808(self, id) == 0) {
        if (self->count < 200) {
            self->ids[self->count] = id;
            self->count++;
        }
        self->lastId = -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B68D0);
#ifdef SKIP_ASM
extern "C" void func_002B5E68(void* self);
void func_002B6900(void* self);

extern "C" void func_002B68D0(void* self)
{
    func_002B5E68(self);
    func_002B6900(self);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6900__FPv);
#ifdef SKIP_ASM
void func_002B6900(void* self)
{
    *(int*)((char*)self + 0x330) = 0;
    *(int*)((char*)self + 0x324) = 0;
    *(int*)((char*)self + 0x328) = -1;
    *(int*)((char*)self + 0x32c) = -1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6988);
#ifdef SKIP_ASM
extern "C" int func_002B6988(void* self, unsigned int i)
{
    if (i < 2) {
        return 0;
    }
    sTriggerRef* refs = *(sTriggerRef**)((char*)self + 0x33c);
    return *(int*)((char*)refs[i].obj + 0x8) == 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B69B8);
#ifdef SKIP_ASM
struct sWtmVEntryF {
    short delta;
    short index;
    float (*fn)(void*);
};

extern "C" int func_002B69B8(void* self, void* src, void* state)
{
    void* obj = *(void**)((char*)src + 0xC);
    if (obj == 0) {
        return 0;
    }
    float prev = *(float*)((char*)state + 0x68);
    sWtmVEntryF* vt = *(sWtmVEntryF**)((char*)obj + 0xC);
    float cur = vt[30].fn((char*)obj + vt[30].delta);
    int below = cur < prev;
    int r = *(int*)((char*)state + 0x6C) != 0 && !below;
    *(float*)((char*)state + 0x68) = cur;
    *(int*)((char*)state + 0x6C) = below;
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6A68);
#ifdef SKIP_ASM
extern "C" void func_002B6A68(sWorldTrigger70** self)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1) {
            (*self)[i].unk34 = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6AA0);
#ifdef SKIP_ASM
struct sQuad6AA0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sQuad6AA0 D_004FF120;

struct sTriggerSlot120 {
    int unk0;
    char pad[0x10c];
    sQuad6AA0 q;        // 0x110
};

struct sTriggerSlots {
    sTriggerSlot120 slots[2];
};

extern "C" sTriggerSlots* func_002B6AA0(sTriggerSlots* self)
{
    sTriggerSlot120* p = self->slots;
    for (int i = 1; i != -1; i--, p++) {
        p->unk0 = 0;
        p->q = D_004FF120;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6AE0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_002B6AE0(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B08);
#ifdef SKIP_ASM
extern "C" void func_002B6B50(void*);

extern "C" int func_002B6B08(void* self, int a1)
{
    if (a1 != 0) {
        func_002B6B50(self);
    }
    return 1;
}
#endif

extern "C" void func_002B6C20(void*);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B30);
#ifdef SKIP_ASM
extern "C" void func_002B6B30(void* self, int a1)
{
    if (a1 != 0) {
        func_002B6C20(self);
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B50);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6C20);
#ifdef SKIP_ASM
struct sWtmEnt18 {
    char pad[0xC];
    int active;
    char pad2[0x8];
};
struct sWtmEntList {
    char pad[0xC];
    int count;
    sWtmEnt18 entries[1];
};
extern "C" void func_002B7410(void* self, sWtmEnt18* e);

// PORT: the unit declares func_002B6C20 as void(void*); its callers pass a1 through.
int func_002B6C20_impl(void* self, sWtmEntList* list) __asm__("func_002B6C20");

int func_002B6C20_impl(void* self, sWtmEntList* list)
{
    int n = list->count;
    sWtmEnt18* entries = list->entries;
    for (int i = 0; i < n; i++) {
        sWtmEnt18* e = &entries[i];
        if (e->active != 0) {
            func_002B7410(self, e);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6C90);
#ifdef SKIP_ASM
struct sTriggerEntry18 {
    int unk0;
    int unk4;
    int unk8;
    int active;
    void* unk10;
    int unk14;
};

extern "C" void func_002B7318(void* self, sTriggerEntry18* e);

extern "C" int func_002B6C90(void* self, int a1, sTriggerEntry18* entries, int n)
{
    for (int i = 0; i < n; i++) {
        if (entries[i].active != 0) {
            func_002B7318(self, &entries[i]);
        }
    }
    return 1;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6CF8);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6F40);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7318);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7410);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B75D0);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B77C0);
#ifdef SKIP_ASM
struct sTriggerIdSlot120 {
    int count;
    int ids[71];
};

static inline int getTriggerId(sTriggerIdSlot120* s, int j)
{
    return s->ids[j];
}

static inline void clearTriggerId(sTriggerIdSlot120* s, int j)
{
    s->ids[j] = 0;
}

extern "C" void func_002B77C0(sTriggerIdSlot120* self, int id)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < self[i].count; j++) {
            if (getTriggerId(&self[i], j) == id) {
                clearTriggerId(&self[i], j);
            }
        }
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7848);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7908);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B8190);
#ifdef SKIP_ASM
extern "C" void func_002B8190(void* self, void* obj, int* o0, int* o1, int* o2, int* o3)
{
    int* hdr = *(int**)(*(char**)((char*)obj + 0x8C) + 0x10);
    char* base = (char*)hdr + 8;
    char* p = base + (hdr[0] << 2);
    int n = hdr[1];
    int* recs[4];
    int i;
    int j;

    for (i = 0; i < n; i++) {
        recs[i] = (int*)p;
        switch (*(int*)p) {
        case 0:
            p += 0x1C;
            break;
        case 1:
            p += 0x30;
            break;
        case 2:
            p += 0x30;
            break;
        case 3:
            p += 0x18;
            break;
        }
    }

    *o0 = 0;
    *o1 = 0;
    *o2 = 0;
    *o3 = 0;

    for (j = 0; j < n; j++) {
        int v;
        switch (*recs[0]) {
        case 0:
            v = recs[j][1];
            break;
        case 1:
            v = recs[j][1];
            break;
        case 2:
            v = recs[j][1];
            break;
        case 3:
            v = recs[j][1];
            break;
        default:
            v = recs[j][1];
            break;
        }
        switch (j) {
        case 0:
            *o0 = v;
            break;
        case 1:
            *o1 = v;
            break;
        case 2:
            *o2 = v;
            break;
        case 3:
            *o3 = v;
            break;
        }
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B82B8);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B83F8);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B8818);

