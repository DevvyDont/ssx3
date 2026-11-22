#include "common.h"
#include "worldpainter/worldpainterman.h"
#include "vtable.h"
#include "worldpainter/worldpainterquery.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "world/world.h"
//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery_cWorldPainterQuery);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char cWorldPainterQuery_vtbl[];
extern char str_tWPIGD_MusicTrigger[];
extern char str_tWPIGD_Mix[];
extern char str_tWPIGD_Ambience[];
extern char str_tWPIGD_Speech[];
extern char str_tWPIGD_Camera[];
extern char str_tWPIGD_Fog[];
extern char str_tWPIGD_LightGlow[];
extern char str_tWPIGD_ScreenTint[];
extern char str_tWPIGD_SkyBox[];
extern char str_tWPIGD_Sun[];
extern char str_tWPIGD_Surface[];
extern char str_tWPIGD_Lighting[];
extern char str_tWPIGD_Weather[];
extern char str_tWPIGD_Danger[];
extern "C" void* tWPInGameData_MusicTrigger_tWPInGameData_MusicTrigger(void* p);
extern "C" void* tWPInGameData_Mix_tWPInGameData_Mix(void* p);
extern "C" void* tWPInGameData_Ambience_tWPInGameData_Ambience(void* p);
extern "C" void* tWPInGameData_Speech_tWPInGameData_Speech(void* p);
extern "C" void* tWPInGameData_Camera_tWPInGameData_Camera(void* p);
extern "C" void* tWPInGameData_Fog_tWPInGameData_Fog(void* p);
extern "C" void* tWPInGameData_LightGlow_tWPInGameData_LightGlow(void* p);
extern "C" void* tWPInGameData_ScreenTint_tWPInGameData_ScreenTint(void* p);
extern "C" void* tWPInGameData_SkyBox_tWPInGameData_SkyBox(void* p);
extern "C" void* tWPInGameData_Sun_tWPInGameData_Sun(void* p);
extern "C" void* tWPInGameData_Surface_tWPInGameData_Surface(void* p);
extern "C" void* tWPInGameData_Lighting_tWPInGameData_Lighting(void* p);
extern "C" void* tWPInGameData_Weather_tWPInGameData_Weather(void* p);
extern "C" void* tWPInGameData_Danger_tWPInGameData_Danger(void* p);

struct sWPQueryType {
    unsigned int flags : 8;
    unsigned int kind : 24;
};


extern cWorldPainterQuery* D_004A3880_0410 __asm__("cWorldPainterQuery_m_queryListStart");

/**
 * Constructs a world painter query for a region type, creates the matching query implementation and links the
 * query into the global query list.
 *
 * Kinds 0-13 get a new implementation at +0x0: tWPIGD_MusicTrigger, Mix, Ambience, Speech, Camera, Fog,
 * LightGlow, ScreenTint, SkyBox, Sun, Surface, Lighting, Weather, Danger; other kinds leave it unset. Both
 * distances (+0x8, +0xC) start at 4286578688.0, the integer 0xFF800000 (the bit pattern of -infinity)
 * converted to float. The query becomes the head of the list at cWorldPainterQuery_m_queryListStart.
 *
 * @param type  the region type (On Tour tWorldPainterRegionType): low byte kept as flags, the kind above it
 * @return @p self
 */
// Creates the query implementation for the requested kind and links the query into the global list.
extern "C" cWorldPainterQuery* cWorldPainterQuery_cWorldPainterQuery(cWorldPainterQuery* self, unsigned int type)
{
    (*(sWPQueryType*)&self->m_regionType).flags = type;
    (*(sWPQueryType*)&self->m_regionType).kind = type >> 8;
    self->mVtbl = (void**)cWorldPainterQuery_vtbl;
    self->m_x = 4286578688.0f;
    self->m_y = 4286578688.0f;
    switch ((*(sWPQueryType*)&self->m_regionType).kind) {
    case 0:
        self->m_data = (tWPInGameData*)tWPInGameData_MusicTrigger_tWPInGameData_MusicTrigger(cMemMan_alloc(0x18, str_tWPIGD_MusicTrigger, 0, 0));
        break;
    case 1:
        self->m_data = (tWPInGameData*)tWPInGameData_Mix_tWPInGameData_Mix(cMemMan_alloc(0x10, str_tWPIGD_Mix, 0, 0));
        break;
    case 2:
        self->m_data = (tWPInGameData*)tWPInGameData_Ambience_tWPInGameData_Ambience(cMemMan_alloc(0x10, str_tWPIGD_Ambience, 0, 0));
        break;
    case 3:
        self->m_data = (tWPInGameData*)tWPInGameData_Speech_tWPInGameData_Speech(cMemMan_alloc(0x10, str_tWPIGD_Speech, 0, 0));
        break;
    case 4:
        self->m_data = (tWPInGameData*)tWPInGameData_Camera_tWPInGameData_Camera(cMemMan_alloc(0x10, str_tWPIGD_Camera, 0, 0));
        break;
    case 5:
        self->m_data = (tWPInGameData*)tWPInGameData_Fog_tWPInGameData_Fog(cMemMan_alloc(0x38, str_tWPIGD_Fog, 0, 0));
        break;
    case 6:
        self->m_data = (tWPInGameData*)tWPInGameData_LightGlow_tWPInGameData_LightGlow(cMemMan_alloc(0x40, str_tWPIGD_LightGlow, 0, 0));
        break;
    case 7:
        self->m_data = (tWPInGameData*)tWPInGameData_ScreenTint_tWPInGameData_ScreenTint(cMemMan_alloc(0x38, str_tWPIGD_ScreenTint, 0, 0));
        break;
    case 8:
        self->m_data = (tWPInGameData*)tWPInGameData_SkyBox_tWPInGameData_SkyBox(cMemMan_alloc(0x10, str_tWPIGD_SkyBox, 0, 0));
        break;
    case 9:
        self->m_data = (tWPInGameData*)tWPInGameData_Sun_tWPInGameData_Sun(cMemMan_alloc(80, str_tWPIGD_Sun, 0, 0));
        break;
    case 10:
        self->m_data = (tWPInGameData*)tWPInGameData_Surface_tWPInGameData_Surface(cMemMan_alloc(0x10, str_tWPIGD_Surface, 0, 0));
        break;
    case 11:
        self->m_data = (tWPInGameData*)tWPInGameData_Lighting_tWPInGameData_Lighting(cMemMan_alloc(88, str_tWPIGD_Lighting, 0, 0));
        break;
    case 12:
        self->m_data = (tWPInGameData*)tWPInGameData_Weather_tWPInGameData_Weather(cMemMan_alloc(160, str_tWPIGD_Weather, 0, 0));
        break;
    case 13:
        self->m_data = (tWPInGameData*)tWPInGameData_Danger_tWPInGameData_Danger(cMemMan_alloc(0x18, str_tWPIGD_Danger, 0, 0));
        break;
    }
    self->m_next = (cWorldPainterQuery*)D_004A3880_0410;
    D_004A3880_0410 = (self);
    return (self);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery__cWorldPainterQuery);
#ifdef SKIP_ASM
void operator_delete(int* p);
extern char cWorldPainterQuery_vtbl[];
extern char tWorldPainterQuery_vtbl[];

struct sWPQuery_06C0 {
    int* buf;
    void* vtbl;
    char pad8[0xC];
    sWPQuery_06C0* next;
};

extern sWPQuery_06C0* cWorldPainterQuery_m_queryListStart;

/**
 * Destroys a world painter query: frees its data and unlinks it from the global query list (cWorldPainterQuery_m_queryListStart).
 *
 * When the query is not the list's head the list is walked until the entry before it; the null test after that walk
 * is useless (the walk dereferences each entry first). Restores the base vtable (tWorldPainterQuery_vtbl) and frees the object
 * when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorldPainterQuery__cWorldPainterQuery(cWorldPainterQuery* self, int flags)
{
    self->mVtbl = (void**)cWorldPainterQuery_vtbl;
    operator_delete((int*)self->m_data);
    sWPQuery_06C0* query = cWorldPainterQuery_m_queryListStart;
    if (query == ((sWPQuery_06C0*)self)) {
        cWorldPainterQuery_m_queryListStart = (sWPQuery_06C0*)self->m_next;
    } else {
        while (query->next != ((sWPQuery_06C0*)self)) {
            query = query->next;
        }
        if (query != 0) {
            query->next = (sWPQuery_06C0*)self->m_next;
        }
    }
    self->mVtbl = (void**)tWorldPainterQuery_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery_queryPosition);
#ifdef SKIP_ASM
extern int cWorldPainterMan_m_iCurrSection;

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

// PORT: cWorldPainterQuery_lookupRegion takes (self, list); this caller also passes the query point in $f12/$f13.
extern "C" sHit_0778* cWorldPainterQuery_lookupRegion_q(void* self, void* list, float x, float z) __asm__("cWorldPainterQuery_lookupRegion");

struct sOutVt_0778 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tEnd_0778)(void*);
typedef int (*tTest_0778)(void*, float*);
typedef void (*tSet_0778)(void*, float*, float);


struct sLayer_0778 {
    char pad_0x0[0x4];
    int count;              // 0x4
    void* cells[1];         // 0x8
};



static inline int valid_0778(cWorldPainterSection* e)
{
    return e->m_iState == 3 && ((sLayer_0778*)e->m_pWPHeader) != 0 && ((sLayer_0778*)e->m_pWPHeader)->count > 0;
}

static inline void end_0778(tWPInGameData* o)
{
    ((tEnd_0778)((sOutVt_0778*)o->mVtbl)[69].fn)(vthis(o, ((sOutVt_0778*)o->mVtbl)[69]));
}

static inline void set_0778(tWPInGameData* o, float* d, float v)
{
    ((tSet_0778)((sOutVt_0778*)o->mVtbl)[66].fn)(vthis(o, ((sOutVt_0778*)o->mVtbl)[66]), d, v);
}

/**
 * Looks up the world painter layer value under a position and blends it into the query's result, tracking the
 * distance travelled.
 *
 * The result's value is -99999 before the first query; later queries add the distance from the previous (x, z). The
 * cell of the query's layer (id low byte) in the painter map (World +0x8) at cWorldPainterMan_m_iCurrSection is searched for (x, z)
 * (cWorldPainterQuery_lookupRegion); a hit with another key (id >> 8) returns the result unchanged, a missing cell or hit ends the
 * result (end_0778). The first hit sets the data with weight -1 and the distance 0. Later hits use @p blend as the
 * weight; with @p blend == -99999 the weight is 1 (and the distance resets) when the hit's first value lies between 0
 * and the distance travelled, else minus that value. The while loop never repeats (cWorldPainterMan_m_iCurrSection is not advanced).
 *
 * @param x  the world x
 * @param z  the world z (On Tour fy)
 * @param blend  the blend weight, or -99999 for the distance rule (On Tour blend)
 * @return the query's result (On Tour const tWPInGameData*)
 */
extern "C" tWPInGameData* cWorldPainterQuery_queryPosition(cWorldPainterQuery* self, float x, float z, float blend)
{
    int first = 1;
    cWorldPainterMan* map = SSXApp->Game->World->m_pWorldPainterMan;
    if ((self->m_data)->m_wpTravelDistance != -99999.0f) {
        first = 0;
    }
    if (!first) {
        float dx = self->m_x - x;
        float dz = self->m_y - z;
        (self->m_data)->m_wpTravelDistance += Sqrt_0778(dx * dx + dz * dz);
    }
    int idx = cWorldPainterMan_m_iCurrSection;
    self->m_x = x;
    self->m_y = z;
    while (valid_0778(&(map->m_pCache)[idx])) {
        cWorldPainterSection* entry = (cWorldPainterSection*)((char*)map->m_pCache + idx * (int)sizeof(cWorldPainterSection));
        void* cell = ((sLayer_0778*)entry->m_pWPHeader)->cells[*(unsigned char*)&(*(unsigned int*)&self->m_regionType)];
        if (cell == 0) {
            if (first) {
                end_0778((self->m_data));
            }
            break;
        }
        sHit_0778* hit = cWorldPainterQuery_lookupRegion_q(self, cell, self->m_x, self->m_y);
        if (hit == 0) {
            break;
        }
        if (hit->key != ((*(unsigned int*)&self->m_regionType) >> 8)) {
            return (self->m_data);
        }
        if (((tTest_0778)((sOutVt_0778*)(self->m_data)->mVtbl)[68].fn)((char*)(self->m_data) + ((sOutVt_0778*)(self->m_data)->mVtbl)[68].delta, hit->data)) {
            (self->m_data)->m_wpTravelDistance = 0.0f;
        }
        if (first) {
            set_0778((self->m_data), hit->data, -1.0f);
            (self->m_data)->m_wpTravelDistance = 0.0f;
            return (self->m_data);
        }
        if (blend == -99999.0f) {
            blend = 0.0f;
            float layerValue = *hit->data;
            if (blend <= layerValue && layerValue <= (self->m_data)->m_wpTravelDistance) {
                set_0778((self->m_data), hit->data, 1.0f);
                (self->m_data)->m_wpTravelDistance = blend;
            } else {
                set_0778((self->m_data), hit->data, -*hit->data);
            }
        } else {
            set_0778((self->m_data), hit->data, blend);
        }
        return (self->m_data);
    }
    end_0778((self->m_data));
    return (self->m_data);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpainterqueryctor", cWorldPainterQuery_lookupRegion);
#ifdef SKIP_ASM
struct cWorldPainterQuery_lookupRegion_sItem {
    int a;
    int b;
};

struct cWorldPainterQuery_lookupRegion_sList {
    int field_0x0;
    int field_0x4;
    cWorldPainterQuery_lookupRegion_sItem* items;
};

extern "C" int cWorldPainterLayer_getQuadTreeContents(cWorldPainterQuery_lookupRegion_sList* list);

/**
 * Returns the region a layer paints at a position: m_pRegions[i] for the index its quad tree's leaf holds.
 *
 * On Tour's lookupRegion takes (pWPLayer, fx, fy); here the position is not declared and stays in $f12/$f13 for
 * cWorldPainterLayer_getQuadTreeContents, which reads the leaf's second word.
 *
 * @param list  the world-painter layer (On Tour cWorldPainterLayer* pWPLayer)
 * @return the region (On Tour const tWPRegion*), or 0 when the index is -1
 */
extern "C" cWorldPainterQuery_lookupRegion_sItem* cWorldPainterQuery_lookupRegion(cWorldPainterQuery* self, cWorldPainterQuery_lookupRegion_sList* list)
{
    int i = cWorldPainterLayer_getQuadTreeContents(list);
    return (i != -1) ? &list->items[i] : 0;
}
#endif

