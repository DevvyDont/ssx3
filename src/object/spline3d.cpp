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

//100%
INCLUDE_ASM("object/spline3d", func_00345638);
#ifdef SKIP_ASM
struct sV4_5638 {
    float x, y, z, w;
    sV4_5638(const float& a, const float& b, const float& c, const float& d) : x(a), y(b), z(c), w(d) {}
};
struct sM44_5638 {
    float m[16];
};

class cWorld_5638 {
public:
    char pad_0x0[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual sM44_5638 v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void v90();
    virtual void v91();
    virtual void v92();
    virtual void v93();
};

extern cWorld_5638* D_004A5B80;

struct sVEnt_5638 {
    short delta;
    short index;
    int (*fn)(void*, const sV4_5638&, const sV4_5638&, const sM44_5638&);
};

extern "C" int func_00345638(void* self)
{
    char* s = (char*)self;
    cWorld_5638* w = D_004A5B80;
    sVEnt_5638* vt = *(sVEnt_5638**)((char*)w + 0x10D8);
    return vt[93].fn((char*)w + vt[93].delta, sV4_5638(*(float*)(s + 0x6C), *(float*)(s + 0x70), *(float*)(s + 0x74), 1.0f),
                  sV4_5638(*(float*)(s + 0x78), *(float*)(s + 0x7C), *(float*)(s + 0x80), 1.0f),
                  w->v43()) != 1;
}
#endif

