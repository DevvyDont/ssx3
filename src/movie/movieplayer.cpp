#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00242288(void* self);
extern const char D_0047C128[];
extern void* D_004A2C74;

//99.53%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getManager__Fv);
#ifdef SKIP_ASM
void* cMCOverlayManager_getManager()
{
    if (D_004A2C74 == 0) {
        void* mem = cMemMan_alloc(0x74C, D_0047C128, 0, 0);
        D_004A2C74 = func_00242288(mem);
    }
    return D_004A2C74;
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023C860);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C898);
#ifdef SKIP_ASM
extern "C" int func_0023C898(void* self)
{
    return *(int*)((char*)self + 0x130) == 1;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C8D0);
#ifdef SKIP_ASM
extern "C" int func_0023C8D0(void* self)
{
    // masks applied as separate statements; folding them into one
    // expression collapses the two `and` instructions into one
    int v = *(int*)((char*)self + 0x43c);
    v &= -5;
    v &= -481;
    return v != 0;
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023C8F0);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CA28);
#ifdef SKIP_ASM
struct sMovieVEntryCA28 {
    short delta;
    short index;
    void (*fn)(void*, int, int, int);
};

extern "C" void func_0023CA28(void* self, int a, int b, int c)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntryCA28* vt = *(sMovieVEntryCA28**)obj;
    vt[29].fn((char*)obj + vt[29].delta, a, b, c);
    *(int*)((char*)self + 0xE8) = c;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CA70);
#ifdef SKIP_ASM
struct sMovieVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023CA70(void* self)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry* vt = *(sMovieVEntry**)obj;
    vt[32].fn((char*)obj + vt[32].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CAA0);
#ifdef SKIP_ASM
struct sMovieVEntryCAA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// PORT: the unit declares func_0023CAA0(void* self), but the body takes a
// second argument (callers pass it in $5); the real body is bound by asm label.
void func_0023CAA0_impl(void* self, int a) __asm__("func_0023CAA0");

void func_0023CAA0_impl(void* self, int a)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntryCAA0* vt = *(sMovieVEntryCAA0**)obj;
    vt[19].fn((char*)obj + vt[19].delta, a);
    *(int*)((char*)self + 0xF0) = a;
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023CAE8);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CC20);
#ifdef SKIP_ASM
extern "C" void func_0023CC20(void* self)
{
    if (*(int*)((char*)self + 0x118) != 0) {
        void* obj = *(void**)((char*)self + 0x434);
        sMovieVEntry* vt = *(sMovieVEntry**)obj;
        vt[43].fn((char*)obj + vt[43].delta);
    }
}
#endif

extern "C" void* func_0023CAA0(void* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", func_0023CC58__FPv);
#ifdef SKIP_ASM
void* func_0023CC58(void* self)
{
    return func_0023CAA0(self);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023CC78);

INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_setTitleString);

INCLUDE_ASM("movie/movieplayer", func_0023CF38);

INCLUDE_ASM("movie/movieplayer", func_0023D570);

INCLUDE_ASM("movie/movieplayer", func_0023D5A8);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D5E8);
#ifdef SKIP_ASM
extern "C" void func_002C2300(void*);

extern "C" void func_0023D5E8(void* self)
{
    func_002C2300(*(void**)((char*)self + 0x434));
    *(void**)((char*)self + 0x434) = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D618);
#ifdef SKIP_ASM
extern "C" void func_0023D7D8(void* self);
extern "C" void func_0023D660(void* self);

extern "C" void func_0023D618(void* self)
{
    switch (*(int*)((char*)self + 0xB8)) {
    case 0:
        func_0023D7D8(self);
        break;
    case 1:
        func_0023D660(self);
        break;
    }
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023D660);

INCLUDE_ASM("movie/movieplayer", func_0023D7D8);

INCLUDE_ASM("movie/movieplayer", func_0023E268);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E2C0__FPv);
#ifdef SKIP_ASM
void func_0023E2C0(void* self)
{
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023E2C8);

INCLUDE_ASM("movie/movieplayer", func_0023E320);

INCLUDE_ASM("movie/movieplayer", func_0023E3C0);

INCLUDE_ASM("movie/movieplayer", func_0023E428);

INCLUDE_ASM("movie/movieplayer", func_0023E498);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E4F0__FPv);
#ifdef SKIP_ASM
void func_0023E4F0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E4F8);
#ifdef SKIP_ASM
struct sMovieVEntryE4F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023E4F8(void* self)
{
    if ((*(int*)((char*)self + 0x108))-- <= 0) {
        sMovieVEntryE4F8* vt = *(sMovieVEntryE4F8**)((char*)self + 0x748);
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023E540);

INCLUDE_ASM("movie/movieplayer", func_0023E820);

INCLUDE_ASM("movie/movieplayer", func_0023EA30);

INCLUDE_ASM("movie/movieplayer", func_0023EA90);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023EB50);
#ifdef SKIP_ASM
struct sMovieVEntry2 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023EB50(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 4);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023EB80);

INCLUDE_ASM("movie/movieplayer", func_0023EC00);

INCLUDE_ASM("movie/movieplayer", func_0023EF68);

INCLUDE_ASM("movie/movieplayer", func_0023F050);

INCLUDE_ASM("movie/movieplayer", func_0023F0F8);

INCLUDE_ASM("movie/movieplayer", func_0023F150);

INCLUDE_ASM("movie/movieplayer", func_0023F1D8);

INCLUDE_ASM("movie/movieplayer", func_0023F258);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023F2D8);
#ifdef SKIP_ASM
extern "C" void func_0023F2D8(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023F308);

INCLUDE_ASM("movie/movieplayer", func_0023F3A0);

INCLUDE_ASM("movie/movieplayer", func_0023F438);

INCLUDE_ASM("movie/movieplayer", func_0023F4F8);

INCLUDE_ASM("movie/movieplayer", func_0023F578);

INCLUDE_ASM("movie/movieplayer", func_0023F698);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FAE0);
#ifdef SKIP_ASM
extern "C" void func_0023FB20(void* self, int v);

extern "C" void func_0023FAE0(void* self)
{
    int cur = *(int*)((char*)self + 0x428);
    if (*(int*)((char*)self + 0x444) != cur) {
        *(int*)((char*)self + 0x444) = cur;
        *(int*)((char*)self + 0x43C) = 0;
        *(int*)((char*)self + 0x440) = 0;
    }
    func_0023FB20(self, *(int*)((char*)self + 0x428));
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FB18__FPvi);
#ifdef SKIP_ASM
void func_0023FB18(void* self, int val)
{
    *(int*)((char*)self + 0x428) = val;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FB20);
#ifdef SKIP_ASM
extern "C" void func_0023FB20(void* self, int v)
{
    *(int*)((char*)self + 0x42C) = v;
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry2* vt = *(sMovieVEntry2**)obj;
    vt[5].fn((char*)obj + vt[5].delta, v);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023FB58);

INCLUDE_ASM("movie/movieplayer", func_0023FBB8);

INCLUDE_ASM("movie/movieplayer", func_0023FC70);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FD08);
#ifdef SKIP_ASM
extern "C" void func_0023EA30(void* self, int v);

extern "C" void func_0023FD08(void* self)
{
    func_0023EA30(self, 1);
    (*(void (**)())((char*)self + 0x38))();
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023FD40);

INCLUDE_ASM("movie/movieplayer", func_0023FE70);

INCLUDE_ASM("movie/movieplayer", func_0023FF00);

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FFC0);
#ifdef SKIP_ASM
extern "C" void func_0023FFC0(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_0023FFF0);

INCLUDE_ASM("movie/movieplayer", func_002400B0);

INCLUDE_ASM("movie/movieplayer", func_00240130);

INCLUDE_ASM("movie/movieplayer", func_002402D0);

INCLUDE_ASM("movie/movieplayer", func_002405D0);

INCLUDE_ASM("movie/movieplayer", func_00240688);

INCLUDE_ASM("movie/movieplayer", func_00240768);

INCLUDE_ASM("movie/movieplayer", func_00240800);

INCLUDE_ASM("movie/movieplayer", func_00240860);

INCLUDE_ASM("movie/movieplayer", func_00240960);

INCLUDE_ASM("movie/movieplayer", func_002409C8);

INCLUDE_ASM("movie/movieplayer", func_00240AB0);

INCLUDE_ASM("movie/movieplayer", func_00240B88);

INCLUDE_ASM("movie/movieplayer", func_00240C30);

//100%
INCLUDE_ASM("movie/movieplayer", func_00240C90);
#ifdef SKIP_ASM
extern "C" void func_00240C90(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 6);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00240CC0);

INCLUDE_ASM("movie/movieplayer", func_00240D90);

//100%
INCLUDE_ASM("movie/movieplayer", func_00240EB0);
#ifdef SKIP_ASM
extern "C" void func_00240EB0(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240EE0);
#ifdef SKIP_ASM
extern "C" void func_00240EE0(void* self)
{
    sMovieVEntry2* vt = *(sMovieVEntry2**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, 6);
}
#endif

extern "C" void* func_0023FB58(void* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", func_00240F10__FPv);
#ifdef SKIP_ASM
void* func_00240F10(void* self)
{
    return func_0023FB58(self);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00240F30);

INCLUDE_ASM("movie/movieplayer", func_00241000);

//100%
INCLUDE_ASM("movie/movieplayer", func_002410A0);
#ifdef SKIP_ASM
extern "C" void func_002410A0(void* self)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry* vt = *(sMovieVEntry**)obj;
    vt[12].fn((char*)obj + vt[12].delta);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00241138);

INCLUDE_ASM("movie/movieplayer", func_00241180);

//100%
INCLUDE_ASM("movie/movieplayer", func_00241200);
#ifdef SKIP_ASM
struct sMovieVEntry3 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern "C" void func_00241200(void* self, int a, int n)
{
    *(int*)((char*)self + 0xE8) += n;
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry3* vt = *(sMovieVEntry3**)obj;
    vt[30].fn((char*)obj + vt[30].delta, a, n);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00241240);

INCLUDE_ASM("movie/movieplayer", func_002412A0);

INCLUDE_ASM("movie/movieplayer", func_00241380);

INCLUDE_ASM("movie/movieplayer", func_00241400);

INCLUDE_ASM("movie/movieplayer", func_00241540);

INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceDisplayString);

//100%
INCLUDE_ASM("movie/movieplayer", func_002419D8);
#ifdef SKIP_ASM
extern "C" int func_002419D8(void* self, int a1)
{
    char* p = (char*)self + a1 * 0xe0;
    return *(int*)(p + 0x354);
}
#endif

INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceTotalString);

struct sPad16 { char x; int pad[3]; };
extern sPad16 D_0047C3A8;
extern "C" void func_002C2540(void*, void*);

//100%
INCLUDE_ASM("movie/movieplayer", func_00241AA0);
#ifdef SKIP_ASM
extern "C" void func_00241AA0(void* self)
{
    func_002C2540(self, &D_0047C3A8);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00241AC0);

INCLUDE_ASM("movie/movieplayer", func_00241B20);

INCLUDE_ASM("movie/movieplayer", func_00241CD8);

INCLUDE_ASM("movie/movieplayer", func_00241D40);

//100%
INCLUDE_ASM("movie/movieplayer", func_00241DC8);
#ifdef SKIP_ASM
extern "C" void func_002C26D0(void* dst, void* src, int n);
extern char D_004A2C28[];

extern "C" void func_00241DC8(void* self, void* dst, int n)
{
    unsigned short buf[256];
    func_002C2540(buf, D_004A2C28);
    func_002C26D0(dst, buf, n + 1);
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00241E18);

extern "C" void* func_002420C8(void*, int);

//100%
INCLUDE_ASM("movie/movieplayer", func_00241FB0__FPv);
#ifdef SKIP_ASM
void* func_00241FB0(void* self)
{
    return func_002420C8(self, *(int*)((char*)self + 0x42c));
}
#endif

INCLUDE_ASM("movie/movieplayer", func_00241FD0);

INCLUDE_ASM("movie/movieplayer", func_00242050);

INCLUDE_ASM("movie/movieplayer", func_002420C8);

INCLUDE_ASM("movie/movieplayer", func_00242288);

INCLUDE_ASM("movie/movieplayer", func_002424C8);

//100%
INCLUDE_ASM("movie/movieplayer", func_00242500__FPv);
#ifdef SKIP_ASM
void* func_00242500(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)((char*)self + 0x108) = t0;
    *(int*)((char*)self + 0x10c) = t0;
    *(int*)self = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242518);
#ifdef SKIP_ASM
void operator_delete(int*);

extern "C" void func_00242518(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242540);
#ifdef SKIP_ASM
struct sMovieNode {
    char pad_0x0[0xbc];
    sMovieNode* next; // 0xbc
    sMovieNode* prev; // 0xc0
};

struct sMovieList {
    sMovieNode* head; // 0x0
    int count;        // 0x4
};

extern "C" void func_00242540(sMovieList* list, sMovieNode* node)
{
    node->next = list->head;
    node->prev = 0;
    if (list->head != 0) {
        list->head->prev = node;
    }
    list->head = node;
    list->count++;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242570);
#ifdef SKIP_ASM
extern "C" void func_00242570(sMovieList* list, sMovieNode* node)
{
    if (node->prev != 0) {
        node->prev->next = node->next;
    }
    if (node->next != 0) {
        node->next->prev = node->prev;
    }
    if (node == list->head) {
        list->head = node->next;
    }
    list->count--;
}
#endif

INCLUDE_ASM("movie/movieplayer", func_002425C0);

//100%
INCLUDE_ASM("movie/movieplayer", func_00242978);
#ifdef SKIP_ASM
// qsort-style comparator: orders by the float at +0x4, ascending.
extern "C" int func_00242978(const void* a, const void* b)
{
    float fa = *(float*)((char*)a + 0x4);
    float fb = *(float*)((char*)b + 0x4);
    if (fa < fb) {
        return -1;
    }
    if (fb < fa) {
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("movie/movieplayer", func_002429B0);

