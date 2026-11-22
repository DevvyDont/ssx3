#include "common.h"
#include "world/worldrecords.h"
#include "vtable.h"
#include "object/spline3d.h"
#include "render/ps2graphicsman.h"

struct sSplinePoint {
    char pad_0x00[0xC];
    float m_fLength;
    char pad_0x10[0x64 - 0x10];
    sSplinePoint* m_NextSeg; // 0x64
    char pad_0x68[0x84 - 0x68];
    float m_fDistance;
};

struct sSplineHeader {
    char pad_0x00[0x20];
    int numPoints; // 0x20
    sSplinePoint* first; // 0x24
};



//100%
INCLUDE_ASM("object/spline3d", cSpline_calcLength__FP7cSpline);
#ifdef SKIP_ASM
/**
 * Returns the spline's total length, read from its last segment.
 *
 * Follows the next links from the first segment to the last one (numPoints - 1 steps) and adds that segment's
 * floats at 0x84 and 0xC (probably the distance to the segment's start and the segment's own length).
 *
 * @return the spline's length
 */
float cSpline_calcLength(cSpline* self)
{
    sSplineHeader* header = ((sSplineHeader*)(self->Seg)->m_Parent);
    sSplinePoint* cur = header->first;
    for (int i = 1; i < header->numPoints; i++) {
        cur = cur->m_NextSeg;
    }
    return cur->m_fDistance + cur->m_fLength;
}
#endif

//100%
INCLUDE_ASM("object/spline3d", cSpline_writeToReplayFrame);
#ifdef SKIP_ASM
class cSpline_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void write(void* buf, int size);
};

/**
 * Writes the spline's first 8 bytes to a replay frame through the stream's write virtual (slot 1).
 *
 * @param obj  replay stream to write to
 */
extern "C" void cSpline_writeToReplayFrame(cSpline* self, cSpline_writeToReplayFrame_cObj* obj)
{
    obj->write(self, 8);
}
#endif

//100%
INCLUDE_ASM("object/spline3d", cSpline_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream_5570 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
};

struct sTrackNode5570 {
    char pad_0x0[0x24];
    tSplineSegmentRecord* node;      // 0x24
};

struct sTrackSet5570 {
    char pad_0x0[0x44];
    unsigned int* refs;     // 0x44, (node >> 2) << 8 | low byte
};

struct sTrackWorld5570 {
    char pad_0x0[0x8];
    sTrackSet5570** sets;   // 0x8
};

extern "C" sTrackWorld5570** cObjectInterface_getWorld();

static inline sTrackNode5570* refToNode5570(unsigned int p)
{
    return (sTrackNode5570*)(p << 2);
}

struct sTrackRef5570 {
    unsigned int id;

    sTrackNode5570* get()
    {
        sTrackSet5570* set = (*cObjectInterface_getWorld())->sets[id & 0xFF];
        if (set != 0) {
            unsigned int p = set->refs[id >> 8] >> 8;
            if (p != 0) {
                return refToNode5570(p);
            }
        }
        return 0;
    }
};


/**
 * Restores a spline follower from a replay frame: re-finds its current node by index and recomputes its
 * length.
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cSpline_readFromReplayFrame(cSpline* self, cStream_5570* stream)
{
    stream->read((self), 8);
    self->Seg = (tSplineSegmentRecord*)(*(sTrackRef5570*)&self->SplineId).get()->node;
    for (int i = 0; i < self->SegIndex; i++) {
        self->Seg = self->Seg->m_NextSeg;
    }
    self->Length = cSpline_calcLength((cSpline*)self);
}
#endif

//100%
INCLUDE_ASM("object/spline3d", cSplineSegment_isVisible);
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
    virtual sM44_5638 GetNetMatrix();
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


struct sVEnt_5638 {
    short delta;
    short index;
    int (*fn)(void*, const sV4_5638&, const sV4_5638&, const sM44_5638&);
};

/**
 * Returns whether a spline segment's bounding box is not culled by the graphics manager's box test.
 *
 * Builds the box corners (x, y, z, 1) from the segment's m_fvMin (+0x6C) and m_fvMax (+0x78), gets the current view
 * matrix from the graphics manager's virtual slot 43 and passes all three to its box test (virtual slot 93). The result
 * is 1 unless that test returns 1 (the box is outside the view), the same test as cSpline_isVisible. cSpline_draw draws
 * only the segments for which this returns non-zero.
 *
 * @return 1 when the box is (at least partly) in view, 0 when the test culls it
 */
extern "C" int cSplineSegment_isVisible(void* self)
{
    tSplineSegmentRecord* nodeBytes = (tSplineSegmentRecord*)self;
    cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
    sVEnt_5638* vt = (sVEnt_5638*)w->mVtbl;
    return vt[93].fn(vthis(w, vt[93]), sV4_5638(nodeBytes->m_fvMin.m[0], nodeBytes->m_fvMin.m[1], nodeBytes->m_fvMin.m[2], 1.0f),
                  sV4_5638(nodeBytes->m_fvMax.m[0], nodeBytes->m_fvMax.m[1], nodeBytes->m_fvMax.m[2], 1.0f),
                  ((cWorld_5638*)w)->GetNetMatrix()) != 1;
}
#endif

