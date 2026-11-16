#include "common.h"
#include "worldpainter/worldpainterquery.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery_cWorldPainterQuery);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char cWorldPainterQuery_vtbl[];
extern char D_00483CB8[];
extern char D_00483CD0[];
extern char D_00483CE0[];
extern char D_00483CF0[];
extern char D_00483D00[];
extern char D_00483D10[];
extern char D_00483D20[];
extern char D_00483D38[];
extern char D_00483D50[];
extern char D_00483D60[];
extern char D_00483D70[];
extern char D_00483D80[];
extern char D_00483D90[];
extern char D_00483DA0[];
extern "C" void* func_002BC6F8(void* p);
extern "C" void* func_002BC728(void* p);
extern "C" void* func_002BC750(void* p);
extern "C" void* func_002BC778(void* p);
extern "C" void* func_002BC7A0(void* p);
extern "C" void* func_002BC7C8(void* p);
extern "C" void* func_002BC830(void* p);
extern "C" void* func_002BC890(void* p);
extern "C" void* func_002BC8E8(void* p);
extern "C" void* func_002BC910(void* p);
extern "C" void* func_002BC980(void* p);
extern "C" void* func_002BC9B0(void* p);
extern "C" void* func_002BCAF8(void* p);
extern "C" void* func_002BCBD0(void* p);

struct sWPQueryType {
    unsigned int flags : 8;
    unsigned int kind : 24;
};

struct sWPQuery_0410 {
    void* impl;                     // 0x0
    void* vtbl;                     // 0x4
    float minDist;                  // 0x8
    float maxDist;                  // 0xC
    sWPQueryType type;              // 0x10
    sWPQuery_0410* next;            // 0x14
};

extern sWPQuery_0410* D_004A3880_0410 __asm__("D_004A3880");

/**
 * Constructs a world painter query for a region type, creates the matching query implementation and links the
 * query into the global query list.
 *
 * Kinds 0-13 get a new implementation at +0x0: tWPIGD_MusicTrigger, Mix, Ambience, Speech, Camera, Fog,
 * LightGlow, ScreenTint, SkyBox, Sun, Surface, Lighting, Weather, Danger; other kinds leave it unset. Both
 * distances (+0x8, +0xC) start at 4286578688.0, the integer 0xFF800000 (the bit pattern of -infinity)
 * converted to float. The query becomes the head of the list at D_004A3880.
 *
 * @param type  the region type (On Tour tWorldPainterRegionType): low byte kept as flags, the kind above it
 * @return @p self
 */
// Creates the query implementation for the requested kind and links the query into the global list.
extern "C" sWPQuery_0410* cWorldPainterQuery_cWorldPainterQuery(cWorldPainterQuery* self, unsigned int type)
{
    (*(sWPQueryType*)&self->m_regionType).flags = type;
    (*(sWPQueryType*)&self->m_regionType).kind = type >> 8;
    self->mVtbl = (void**)cWorldPainterQuery_vtbl;
    self->m_x = 4286578688.0f;
    self->m_y = 4286578688.0f;
    switch ((*(sWPQueryType*)&self->m_regionType).kind) {
    case 0:
        self->m_data = (tWPInGameData*)func_002BC6F8(cMemMan_alloc(0x18, D_00483CB8, 0, 0));
        break;
    case 1:
        self->m_data = (tWPInGameData*)func_002BC728(cMemMan_alloc(0x10, D_00483CD0, 0, 0));
        break;
    case 2:
        self->m_data = (tWPInGameData*)func_002BC750(cMemMan_alloc(0x10, D_00483CE0, 0, 0));
        break;
    case 3:
        self->m_data = (tWPInGameData*)func_002BC778(cMemMan_alloc(0x10, D_00483CF0, 0, 0));
        break;
    case 4:
        self->m_data = (tWPInGameData*)func_002BC7A0(cMemMan_alloc(0x10, D_00483D00, 0, 0));
        break;
    case 5:
        self->m_data = (tWPInGameData*)func_002BC7C8(cMemMan_alloc(0x38, D_00483D10, 0, 0));
        break;
    case 6:
        self->m_data = (tWPInGameData*)func_002BC830(cMemMan_alloc(0x40, D_00483D20, 0, 0));
        break;
    case 7:
        self->m_data = (tWPInGameData*)func_002BC890(cMemMan_alloc(0x38, D_00483D38, 0, 0));
        break;
    case 8:
        self->m_data = (tWPInGameData*)func_002BC8E8(cMemMan_alloc(0x10, D_00483D50, 0, 0));
        break;
    case 9:
        self->m_data = (tWPInGameData*)func_002BC910(cMemMan_alloc(0x50, D_00483D60, 0, 0));
        break;
    case 10:
        self->m_data = (tWPInGameData*)func_002BC980(cMemMan_alloc(0x10, D_00483D70, 0, 0));
        break;
    case 11:
        self->m_data = (tWPInGameData*)func_002BC9B0(cMemMan_alloc(0x58, D_00483D80, 0, 0));
        break;
    case 12:
        self->m_data = (tWPInGameData*)func_002BCAF8(cMemMan_alloc(0xA0, D_00483D90, 0, 0));
        break;
    case 13:
        self->m_data = (tWPInGameData*)func_002BCBD0(cMemMan_alloc(0x18, D_00483DA0, 0, 0));
        break;
    }
    self->m_next = (cWorldPainterQuery*)D_004A3880_0410;
    D_004A3880_0410 = ((sWPQuery_0410*)self);
    return ((sWPQuery_0410*)self);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery__cWorldPainterQuery);
#ifdef SKIP_ASM
void operator_delete(int* p);
extern char cWorldPainterQuery_vtbl[];
extern char D_00485D30[];

struct sWPQuery_06C0 {
    int* buf;
    void* vtbl;
    char pad8[0xC];
    sWPQuery_06C0* next;
};

extern sWPQuery_06C0* D_004A3880;

extern "C" void cWorldPainterQuery__cWorldPainterQuery(cWorldPainterQuery* self, int flags)
{
    self->mVtbl = (void**)cWorldPainterQuery_vtbl;
    operator_delete((int*)self->m_data);
    sWPQuery_06C0* p = D_004A3880;
    if (p == ((sWPQuery_06C0*)self)) {
        D_004A3880 = (sWPQuery_06C0*)self->m_next;
    } else {
        while (p->next != ((sWPQuery_06C0*)self)) {
            p = p->next;
        }
        if (p != 0) {
            p->next = (sWPQuery_06C0*)self->m_next;
        }
    }
    self->mVtbl = (void**)D_00485D30;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery_queryPosition);
#ifdef SKIP_ASM
extern int D_004A3860;

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_0778(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct sHit_0778 {
    unsigned int key;
    float* data;
};

// PORT: func_002C0A10 takes (self, list); this caller also passes the query point in $f12/$f13.
extern "C" sHit_0778* func_002C0A10_q(void* self, void* list, float x, float z) __asm__("func_002C0A10");

struct sOutVt_0778 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tEnd_0778)(void*);
typedef int (*tTest_0778)(void*, float*);
typedef void (*tSet_0778)(void*, float*, float);

struct sOut_0778 {
    float value;
    sOutVt_0778* vt;
};

struct sLayer_0778 {
    char pad_0x0[0x4];
    int count;              // 0x4
    void* cells[1];         // 0x8
};

struct sEntry_0778 {
    short type;
    short pad;
    int f4;
    sLayer_0778* layer;     // 0x8
};

struct sQuery_0778 {
    sOut_0778* out;         // 0x0
    int f4;
    float x;                // 0x8
    float z;                // 0xC
    unsigned int id;        // 0x10
};

static inline int valid_0778(sEntry_0778* e)
{
    return e->type == 3 && e->layer != 0 && e->layer->count > 0;
}

static inline void end_0778(sOut_0778* o)
{
    ((tEnd_0778)o->vt[69].fn)((char*)o + o->vt[69].delta);
}

static inline void set_0778(sOut_0778* o, float* d, float v)
{
    ((tSet_0778)o->vt[66].fn)((char*)o + o->vt[66].delta, d, v);
}

extern "C" sOut_0778* cWorldPainterQuery_queryPosition(sQuery_0778* self, float x, float z, float h)
{
    int first = 1;
    char* map = *(char**)((char*)SSXApp->Game->World + 0x8);
    if (self->out->value != -99999.0f) {
        first = 0;
    }
    if (!first) {
        float dx = self->x - x;
        float dz = self->z - z;
        self->out->value += Sqrt_0778(dx * dx + dz * dz);
    }
    int idx = D_004A3860;
    self->x = x;
    self->z = z;
    while (valid_0778(&(*(sEntry_0778**)(map + 4))[idx])) {
        sEntry_0778* e = (sEntry_0778*)(*(char**)(map + 4) + idx * (int)sizeof(sEntry_0778));
        void* cell = e->layer->cells[*(unsigned char*)&self->id];
        if (cell == 0) {
            if (first) {
                end_0778(self->out);
            }
            break;
        }
        sHit_0778* hit = func_002C0A10_q(self, cell, self->x, self->z);
        if (hit == 0) {
            break;
        }
        if (hit->key != (self->id >> 8)) {
            return self->out;
        }
        if (((tTest_0778)self->out->vt[68].fn)((char*)self->out + self->out->vt[68].delta, hit->data)) {
            self->out->value = 0.0f;
        }
        if (first) {
            set_0778(self->out, hit->data, -1.0f);
            self->out->value = 0.0f;
            return self->out;
        }
        if (h == -99999.0f) {
            h = 0.0f;
            float v = *hit->data;
            if (h <= v && v <= self->out->value) {
                set_0778(self->out, hit->data, 1.0f);
                self->out->value = h;
            } else {
                set_0778(self->out, hit->data, -*hit->data);
            }
        } else {
            set_0778(self->out, hit->data, h);
        }
        return self->out;
    }
    end_0778(self->out);
    return self->out;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", func_002C0A10);
#ifdef SKIP_ASM
struct func_002C0A10_sItem {
    int a;
    int b;
};

struct func_002C0A10_sList {
    int field_0x0;
    int field_0x4;
    func_002C0A10_sItem* items;
};

extern "C" int func_002BAF90(func_002C0A10_sList* list);

extern "C" func_002C0A10_sItem* func_002C0A10(cWorldPainterQuery* self, func_002C0A10_sList* list)
{
    int i = func_002BAF90(list);
    return (i != -1) ? &list->items[i] : 0;
}
#endif

