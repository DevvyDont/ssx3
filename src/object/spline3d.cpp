#include "common.h"

struct sSplinePoint {
    char pad_0x00[0xC];
    float field_0xC;
    char pad_0x10[0x64 - 0x10];
    sSplinePoint* next; // 0x64
    char pad_0x68[0x84 - 0x68];
    float field_0x84;
};

struct sSplineHeader {
    char pad_0x00[0x20];
    int numPoints; // 0x20
    sSplinePoint* first; // 0x24
};

struct sSplineNode {
    char pad_0x00[0x68];
    sSplineHeader* header; // 0x68
};

struct cSpline {
    char pad_0x00[0x8];
    sSplineNode* node; // 0x8
};

//100%
INCLUDE_ASM("object/spline3d", cSpline_calcLength__FP7cSpline);
#ifdef SKIP_ASM
float cSpline_calcLength(cSpline* self)
{
    sSplineHeader* header = self->node->header;
    sSplinePoint* cur = header->first;
    for (int i = 1; i < header->numPoints; i++) {
        cur = cur->next;
    }
    return cur->field_0x84 + cur->field_0xC;
}
#endif

//100%
INCLUDE_ASM("object/spline3d", func_00345538);
#ifdef SKIP_ASM
class func_00345538_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_00345538(void* self, func_00345538_cObj* obj)
{
    obj->v01(self, 8);
}
#endif

//100%
INCLUDE_ASM("object/spline3d", cSpline_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream_5570 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

struct sTrackNode5570 {
    char pad_0x0[0x24];
    sSplineNode* node;      // 0x24
};

struct sTrackSet5570 {
    char pad_0x0[0x44];
    unsigned int* refs;     // 0x44, (node >> 2) << 8 | low byte
};

struct sTrackWorld5570 {
    char pad_0x0[0x8];
    sTrackSet5570** sets;   // 0x8
};

extern "C" sTrackWorld5570** func_002D1BD8();

static inline sTrackNode5570* refToNode5570(unsigned int p)
{
    return (sTrackNode5570*)(p << 2);
}

struct sTrackRef5570 {
    unsigned int id;

    sTrackNode5570* get()
    {
        sTrackSet5570* set = (*func_002D1BD8())->sets[id & 0xFF];
        if (set != 0) {
            unsigned int p = set->refs[id >> 8] >> 8;
            if (p != 0) {
                return refToNode5570(p);
            }
        }
        return 0;
    }
};

struct sSplineFollow5570 {
    sTrackRef5570 ref;      // 0x0
    int count;              // 0x4
    sSplineNode* node;      // 0x8
    float length;           // 0xC
};

extern "C" void cSpline_readFromReplayFrame(sSplineFollow5570* self, cStream_5570* stream)
{
    stream->v02(self, 8);
    self->node = self->ref.get()->node;
    for (int i = 0; i < self->count; i++) {
        self->node = *(sSplineNode**)((char*)self->node + 0x64);
    }
    self->length = cSpline_calcLength((cSpline*)self);
}
#endif

INCLUDE_ASM("object/spline3d", func_00345638);

