#include "common.h"
#include "camera/trigger/triggeralgorithms.h"
#include "camera/trigger/cameratriggerman.h"
#include "camera/camera.h"
#include "fe/backgroundman.h"
#include "camera/race/allspinchasealgo.h"
#include "camera/race/directionchasealgo.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camaction);
#ifdef SKIP_ASM
extern "C" int get_uint(void* reader, void* dst);
extern "C" int get_cCTActionSwitchCam(void* reader, void* dst);
extern "C" int get_cCTActionBoundedCam(void* reader, char* obj);
extern "C" int get_cCTActionSpline(void* reader, void* dst);
void* get_cCTActionNone(void* unused, int* outType);
// PORT: the game's tagged allocator, bound as a placement operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

struct sCTActVec3A {
    float x, y, z;
};

extern sCTActVec3A D_004FF0D8_act __asm__("t3vect_Zero");     // zero vector
extern void* D_0045C2E8[];      // cCTAction vtable
extern void* D_0045C1D0[];      // bounded cam
extern void* D_0045C1B8[];      // spline
extern char str_Camera_Switch_Action[];
extern char str_Camera_Bound_Action[];
extern char str_Camera_Spline_Action[];
extern char str_Camera_Action_None[];

// PORT: raw vtable stores stand in for the real virtual classes (vptr at 0x4, after `type`).
// Classes with their own vtable set `type` in their own ctor, after the derived vptr store.
struct sCTActA {
    int type;           // 0x00
    void** vtable;      // 0x04
    sCTActA()
    {
        vtable = D_0045C2E8;
    }
    sCTActA(int t)
    {
        vtable = D_0045C2E8;
        type = t;
    }
};

struct sCTActSwitchA : sCTActA {
    int target;         // 0x08
    float blend;        // 0x0C
    sCTActSwitchA() : sCTActA(0)
    {
        target = 0;
        blend = 1.0f;
    }
};

struct sCTActBoundedA : sCTActA {
    int f08;            // 0x08
    float blend;        // 0x0C
    float f10;          // 0x10
    float f14;          // 0x14
    float f18;          // 0x18
    float f1C;          // 0x1C
    int f20;            // 0x20
    int f24;            // 0x24
    sCTActVec3A f28;    // 0x28
    sCTActBoundedA() : sCTActA()
    {
        vtable = D_0045C1D0;
        type = 1;
        f08 = 0;
        blend = 1.0f;
        f14 = 0.5235987901687622f;
        f10 = 400.0f;
        f1C = 25.0f;
        f18 = 70.0f;
        f24 = 0;
        f28 = D_004FF0D8_act;
        f20 = 0;
    }
};

struct sCTActSplineA : sCTActA {
    int f08;            // 0x08
    float blend;        // 0x0C
    float f10;          // 0x10
    int f14;            // 0x14
    float f18;          // 0x18
    int f1C;            // 0x1C
    sCTActSplineA() : sCTActA()
    {
        vtable = D_0045C1B8;
        type = 2;
        f08 = 0;
        blend = 1.0f;
        f10 = 0.5235987901687622f;
        f14 = 0;
        f18 = 4.0f;
        f1C = 0;
    }
};

struct sCTActNoneA : sCTActA {
    sCTActNoneA() : sCTActA(3) {}
};

/**
 * Reads a camera trigger action of any type from a trigger stream and allocates it.
 *
 * Reads the type word, constructs the matching action and reads its data: 0 "Camera Switch Action",
 * 1 "Camera Bound Action", 2 "Camera Spline Action", 3 "Camera Action None". An unknown type leaves
 * @p out unwritten.
 *
 * @param reader  pointer to the stream's read cursor, advanced past what is read
 * @param out     receives the new action
 * @return the number of bytes read
 */
extern "C" int get_camaction(void* reader, void** out)
{
    int type;
    int n = get_uint(reader, &type);
    switch (type) {
    case 0: {
        sCTActSwitchA* action = new (str_Camera_Switch_Action, 0, 0) sCTActSwitchA;
        int bytesRead = get_cCTActionSwitchCam(reader, action);
        *out = action;
        n += bytesRead;
        break;
    }
    case 1: {
        sCTActBoundedA* action = new (str_Camera_Bound_Action, 0, 0) sCTActBoundedA;
        int bytesRead = get_cCTActionBoundedCam(reader, (char*)action);
        *out = action;
        n += bytesRead;
        break;
    }
    case 2: {
        sCTActSplineA* action = new (str_Camera_Spline_Action, 0, 0) sCTActSplineA;
        int bytesRead = get_cCTActionSpline(reader, action);
        *out = action;
        n += bytesRead;
        break;
    }
    case 3: {
        sCTActNoneA* action = new (str_Camera_Action_None, 0, 0) sCTActNoneA;
        // PORT: get_cCTActionNone returns the byte count as void*.
        int bytesRead = (int)get_cCTActionNone(reader, (int*)action);
        *out = action;
        n += bytesRead;
        break;
    }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionBoundedCam);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int get_float(void* reader, void* dst);
extern "C" int get_uint(void* reader, void* dst);
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

/**
 * Reads a bounded-camera action's data from a trigger stream.
 *
 * Sets the action type to 1, then reads six floats into +0x0C to +0x20, a uint into +0x24 and a vector
 * into +0x28.
 *
 * @param reader  pointer to the stream's read cursor
 * @param obj     the bounded-camera action to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTActionBoundedCam(void* reader, char* obj)
{
    int n;
    *(int*)obj = 1;
    n = get_float(reader, obj + 0xC);
    n += get_float(reader, obj + 0x10);
    n += get_float(reader, obj + 0x14);
    n += get_float(reader, obj + 0x18);
    n += get_float(reader, obj + 0x1C);
    n += get_float(reader, obj + 0x20);
    n += get_uint(reader, obj + 0x24);
    n += get_t3Vector_n(reader, obj + 0x28);
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionSwitchCam);
#ifdef SKIP_ASM
extern "C" int get_float(void* reader, void* dst);
extern "C" int get_uint(void* reader, void* dst);

/**
 * Reads a switch-camera action's data from a trigger stream.
 *
 * Sets the action type to 0, then reads a float into +0x0C and a uint into +0x08 (the fields the factory
 * constructor sets to 1.0 and 0).
 *
 * @param reader  pointer to the stream's read cursor
 * @param dst     the switch-camera action to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTActionSwitchCam(void* reader, void* dst)
{
    *(int*)dst = 0;
    int n = get_float(reader, (char*)dst + 0xC);
    return n + get_uint(reader, (char*)dst + 0x8);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionSpline);
#ifdef SKIP_ASM
/**
 * Reads a spline-camera action's data from a trigger stream.
 *
 * Sets the action type to 2, then reads five floats into +0x0C to +0x1C.
 *
 * @param reader  pointer to the stream's read cursor
 * @param dst     the spline action to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTActionSpline(void* reader, void* dst)
{
    *(int*)dst = 2;
    int n = get_float(reader, (char*)dst + 0xC);
    n += get_float(reader, (char*)dst + 0x10);
    n += get_float(reader, (char*)dst + 0x14);
    n += get_float(reader, (char*)dst + 0x18);
    return n + get_float(reader, (char*)dst + 0x1C);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionNone__FPvPi);
#ifdef SKIP_ASM
/**
 * Fills in a no-op camera action, which has no stream data.
 *
 * Sets the action type to 3 and reads nothing.
 *
 * @param reader   the stream's read cursor, not touched
 * @param outType  the action's type field, set to 3
 * @return 0, the number of bytes read (typed as a pointer)
 */
void* get_cCTActionNone(void* reader, int* outType)
{
    *outType = 3;
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camboundobj);
#ifdef SKIP_ASM
// PORT: the game's tagged allocator, bound as a placement operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

struct sCTVec3G {
    float x, y, z;
    sCTVec3G() {}
    sCTVec3G(float a, float b, float c) : x(a), y(b), z(c) {}
};

extern sCTVec3G t3vect_Zero;     // zero vector
extern void* cCTObject_vtbl[];      // cCTObject vtable (the base class; the July preview's _vt$9cCTObject)
extern void* cCTBoundObjEllipse_vtbl[];      // ellipse
extern void* cCTBoundObjBox_vtbl[];      // box
extern void* cCTBoundObjLine_vtbl[];      // line
extern void* cCTBoundObjPoint_vtbl[];      // point
extern char str_Camera_Ellipse_BoundObj[];
extern char str_Camera_Box_BoundObj[];
extern char str_Camera_Line_BoundObj[];
extern char str_Camera_Point_BoundObj[];

// PORT: raw vtable stores stand in for the real virtual classes: the base ctor stores the
// cCTObject vtable first and the derived class's vtable last, where g++ 2.95 sets the
// derived vptr (after the base ctor, before the derived class's members are constructed).
struct sCTBoundObjG {
    sCTVec3G center;    // 0x00
    int f0C;            // 0x0C
    int f10;            // 0x10
    int f14;            // 0x14
    sCTVec3G size;      // 0x18
    void** vtable;      // 0x24
    sCTBoundObjG(const sCTVec3G& s, void** vt)
    {
        vtable = cCTObject_vtbl;
        center = t3vect_Zero;
        f0C = 0;
        f10 = 0;
        f14 = 0;
        size = s;
        vtable = vt;
    }
};

struct sCTBoundEllG : sCTBoundObjG {
    int type;           // 0x28
    sCTBoundEllG() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTBoundObjEllipse_vtbl) { type = 0; }
};

struct sCTBoundBoxG : sCTBoundObjG {
    int type;           // 0x28
    sCTBoundBoxG() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTBoundObjBox_vtbl) { type = 1; }
};

struct sCTBoundLineG : sCTBoundObjG {
    int type;           // 0x28
    sCTVec3G pts[2];    // 0x2C
    sCTBoundLineG() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTBoundObjLine_vtbl)
    {
        type = 2;
        pts[0] = t3vect_Zero;
        pts[1] = t3vect_Zero;
    }
};

struct sCTBoundPointG : sCTBoundObjG {
    int type;           // 0x28
    sCTBoundPointG() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTBoundObjPoint_vtbl) { type = 3; }
};

class cCTBoundObjEllK2;
class cCTBoundObjBoxK2;
class cCTBoundObjLineK2;
struct sBoundObjPoint;
extern "C" int get_cCTBoundObjEllipse(void* reader, cCTBoundObjEllK2* obj);
extern "C" int get_cCTBoundObjBox(void* reader, cCTBoundObjBoxK2* obj);
extern "C" int get_cCTBoundObjLine(void* reader, cCTBoundObjLineK2* obj);
void* get_cCTBoundObjPoint(void* self, sBoundObjPoint* obj);

/**
 * Reads a camera bounding object of any type from a trigger stream and allocates it.
 *
 * Reads the type word, constructs the matching shape (scale 1, position zero) and reads its data:
 * 0 "Camera Ellipse BoundObj", 1 "Camera Box BoundObj", 2 "Camera Line BoundObj", 3 "Camera Point
 * BoundObj". An unknown type leaves @p out unwritten.
 *
 * @param reader  pointer to the stream's read cursor, advanced past what is read
 * @param out     receives the new bounding object
 * @return the number of bytes read
 */
extern "C" int get_camboundobj(void* reader, void** out)
{
    int type;
    int n = get_uint(reader, &type);
    switch (type) {
    case 0: {
        sCTBoundEllG* boundObj = new (str_Camera_Ellipse_BoundObj, 0, 0) sCTBoundEllG;
        int bytesRead = get_cCTBoundObjEllipse(reader, (cCTBoundObjEllK2*)boundObj);
        *out = boundObj;
        n += bytesRead;
        break;
    }
    case 1: {
        sCTBoundBoxG* boundObj = new (str_Camera_Box_BoundObj, 0, 0) sCTBoundBoxG;
        int bytesRead = get_cCTBoundObjBox(reader, (cCTBoundObjBoxK2*)boundObj);
        *out = boundObj;
        n += bytesRead;
        break;
    }
    case 2: {
        sCTBoundLineG* boundObj = new (str_Camera_Line_BoundObj, 0, 0) sCTBoundLineG;
        int bytesRead = get_cCTBoundObjLine(reader, (cCTBoundObjLineK2*)boundObj);
        *out = boundObj;
        n += bytesRead;
        break;
    }
    case 3: {
        sCTBoundPointG* boundObj = new (str_Camera_Point_BoundObj, 0, 0) sCTBoundPointG;
        // PORT: get_cCTBoundObjPoint returns the byte count as void*.
        int bytesRead = (int)get_cCTBoundObjPoint(reader, (sBoundObjPoint*)boundObj);
        *out = boundObj;
        n += bytesRead;
        break;
    }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjEllipse);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTBoundObjEllK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

/**
 * Reads an ellipse bounding object's data from a trigger stream.
 *
 * Sets the shape type to 0, then reads through the object's cCTObject accessors: position and scale
 * vectors, then heading, pitch and roll.
 *
 * @param reader  pointer to the stream's read cursor
 * @param obj     the ellipse to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTBoundObjEllipse(void* reader, cCTBoundObjEllK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 0;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjBox);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTBoundObjBoxK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

/**
 * Reads a box bounding object's data from a trigger stream.
 *
 * Sets the shape type to 1, then reads through the object's cCTObject accessors: position and scale vectors,
 * then heading, pitch and roll.
 *
 * @param reader  pointer to the stream's read cursor
 * @param obj     the box to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTBoundObjBox(void* reader, cCTBoundObjBoxK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 1;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjLine);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTBoundObjLineK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

/**
 * Reads a line bounding object's data from a trigger stream.
 *
 * Sets the shape type to 2, reads position, scale, heading, pitch and roll like the other shapes, then the
 * line's two end points (vectors at +0x2C and +0x38).
 *
 * @param reader  pointer to the stream's read cursor
 * @param obj     the line to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTBoundObjLine(void* reader, cCTBoundObjLineK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 2;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    n += get_t3Vector_n(reader, (char*)obj + 0x2C);
    n += get_t3Vector_n(reader, (char*)obj + 0x38);
    return n;
}
#endif

struct sBoundObjVTable {
    char pad_0x00[0x8];
    short field_0x8;
    char pad_0xA[2];
    void* (*fn)(void*); // 0xC
};

struct sBoundObjPoint {
    char pad_0x00[0x24];
    sBoundObjVTable* vtable;
    int field_0x28;
};

extern "C" void* get_t3Vector(void* self, void* v);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjPoint__FPvP14sBoundObjPoint);
#ifdef SKIP_ASM
/**
 * Reads a point bounding object's data from a trigger stream: sets its shape type to 3 and reads its
 * position.
 *
 * The position vector is read through the object's first virtual accessor (vtable at +0x24), the one the
 * other shapes read their position through. The first parameter is the stream cursor, not a this pointer.
 *
 * @param self  pointer to the stream's read cursor (On Tour char** src)
 * @param obj  the point to fill
 * @return the number of bytes read (get_t3Vector's count, typed void* here)
 */
void* get_cCTBoundObjPoint(void* self, sBoundObjPoint* obj)
{
    obj->field_0x28 = 3;
    void* result = obj->vtable->fn((char*)obj + obj->vtable->field_0x8);
    return get_t3Vector(self, result);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camspline);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");
extern char str_Camera_Trigger_Spline[];

struct sCTSplineG : sCTBoundObjG {
    sCTVec3G pts[4];    // 0x28
    sCTSplineG() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTObject_vtbl)
    {
        pts[0] = t3vect_Zero;
        pts[1] = t3vect_Zero;
        pts[2] = t3vect_Zero;
        pts[3] = t3vect_Zero;
    }
};

class cCTSplineK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; control points at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

/**
 * Allocates a camera trigger spline and reads its data from a trigger stream.
 *
 * Constructs a "Camera Trigger Spline" (scale 1, position and control points zero; it keeps the cCTBoundObj
 * vtable cCTObject_vtbl), reads position, scale, heading, pitch and roll like the bounding shapes, then the four
 * control-point vectors at +0x28, +0x34, +0x40 and +0x4C. readAction calls it for a spline action.
 *
 * @param reader  pointer to the stream's read cursor, advanced past what is read
 * @param out  receives the new spline (On Tour cCTSpline*&)
 * @return the number of bytes read
 */
extern "C" int get_camspline(void* reader, cCTSplineK2** out)
{
    int n;
    *out = (cCTSplineK2*)new (str_Camera_Trigger_Spline, 0, 0) sCTSplineG;
    n = get_t3Vector_n(reader, (*out)->center());
    n += get_t3Vector_n(reader, (*out)->axis());
    n += get_float(reader, (*out)->sizeX());
    n += get_float(reader, (*out)->sizeY());
    n += get_float(reader, (*out)->sizeZ());
    n += get_t3Vector_n(reader, (char*)*out + 0x28);
    n += get_t3Vector_n(reader, (char*)*out + 0x34);
    n += get_t3Vector_n(reader, (char*)*out + 0x40);
    n += get_t3Vector_n(reader, (char*)*out + 0x4C);
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolume_getBoundaryPointClosestTo);
#ifdef SKIP_ASM
struct sVec4_171FA8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBox_171FA8 {
    sVec4_171FA8 lo;
    sVec4_171FA8 hi;
};

struct sVEbox_171FA8 {
    short delta;
    short index;
    sBox_171FA8 (*fn)(void*);
};

struct sVEin_171FA8 {
    short delta;
    short index;
    int (*fn)(void*, sVec4_171FA8*);
};

extern "C" float logf(float x);

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec4_171FA8 Add_171FA8(const sVec4_171FA8& a, const sVec4_171FA8& b)
{
    sVec4_171FA8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4_171FA8 Sub_171FA8(const sVec4_171FA8& a, const sVec4_171FA8& b)
{
    sVec4_171FA8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec4_171FA8 Scale_171FA8(const sVec4_171FA8& v, float s)
{
    sVec4_171FA8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_171FA8(const sVec4_171FA8& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

static inline sBox_171FA8 GetBox_171FA8(char* o)
{
    sVEbox_171FA8* e = &(*(sVEbox_171FA8**)(o + 0x24))[6];
    return e->fn(o + e->delta);
}

static inline int Inside_171FA8(char* o, sVec4_171FA8* p)
{
    sVEin_171FA8* e = &(*(sVEin_171FA8**)(o + 0x24))[7];
    return e->fn(o + e->delta, p);
}

static inline sVec4_171FA8 Center_171FA8(const sBox_171FA8& b)
{
    return Scale_171FA8(Add_171FA8(b.lo, b.hi), 0.5f);
}

/**
 * Returns the point where the ray from a trigger volume's centre through @p pos leaves the volume, found by bisection.
 *
 * Gets the volume's bounding box from its own virtual slot 6 (boundBox; the vtable sits at +0x24 of the cCTObject part)
 * and starts from the box centre. While the volume still contains the test point (slot 7, containsPoint) it moves the
 * test point out along centre-to-@p pos at 2, 4, 8, ... times that offset, remembering the last point that was inside.
 * It then halves the inside/outside bracket (int)logf(length / @p step) + 1 times and returns the outside end.
 * When @p pos is already outside, the bracket is simply the centre and @p pos.
 *
 * It fills slot 8 of the cCTVolume vtable D_0045C3A0 and of both concrete volume vtables (ellipse cCTVolumeEllipse_vtbl, box
 * cCTVolumeBox_vtbl). Likely bug: logf is the natural logarithm, so the step count is ln(length / step) + 1 rather
 * than log2, and the final bracket can stay wider than @p step.
 *
 * @param obj  the trigger volume (this)
 * @param pos  the world point to project onto the boundary
 * @param step  the wanted accuracy in cm (On Tour tolerance_cm)
 * @return the boundary point (the last tested point outside the volume), returned by value
 */
extern "C" sVec4_171FA8 cCTVolume_getBoundaryPointClosestTo(char* obj, sVec4_171FA8* pos, float step)
{
    sVec4_171FA8 center = Center_171FA8(GetBox_171FA8(obj));
    sVec4_171FA8 dir = Sub_171FA8(*pos, center);
    sVec4_171FA8 outsidePos = *pos;
    sVec4_171FA8 lastInsidePos = center;
    float k = 2.0f;
    sVEin_171FA8* containsEntry;
    while (containsEntry = &(*(sVEin_171FA8**)(obj + 0x24))[7], containsEntry->fn(obj + containsEntry->delta, &outsidePos)) {
        lastInsidePos = outsidePos;
        sVec4_171FA8 off = Scale_171FA8(dir, k);
        outsidePos = Add_171FA8(center, off);
        k += k;
    }
    float len = Length_171FA8(Sub_171FA8(outsidePos, lastInsidePos));
    int n = (int)logf(len / step) + 1;
    for (int i = 0; i < n; i++) {
        sVec4_171FA8 testPos = Scale_171FA8(Add_171FA8(outsidePos, lastInsidePos), 0.5f);
        containsEntry = &(*(sVEin_171FA8**)(obj + 0x24))[7];
        if (containsEntry->fn(obj + containsEntry->delta, &testPos)) {
            lastInsidePos = testPos;
        } else {
            outsidePos = testPos;
        }
    }
    return outsidePos;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolumeEllipse_boundBox);
#ifdef SKIP_ASM
extern "C" float GetMaxElement(float* v);

struct sCTVec3K2 {
    float x, y, z;
    sCTVec3K2() {}
    sCTVec3K2(float a, float b, float c) : x(a), y(b), z(c) {}
};

static inline sCTVec3K2 operator-(const sCTVec3K2& a, float s)
{
    return sCTVec3K2(a.x - s, a.y - s, a.z - s);
}

static inline sCTVec3K2 operator+(const sCTVec3K2& a, float s)
{
    return sCTVec3K2(a.x + s, a.y + s, a.z + s);
}

struct sCTVec4K2 {
    float x, y, z, w;
    sCTVec4K2() {}
    sCTVec4K2(const sCTVec3K2& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {}
} __attribute__((aligned(16)));

struct sCTBoxK2 {
    sCTVec4K2 min;
    sCTVec4K2 max;
    sCTBoxK2(const sCTVec4K2& a, const sCTVec4K2& b) : min(a), max(b) {}
};

/**
 * Returns the bounding box of an ellipse trigger volume: its position plus or minus its largest scale component.
 *
 * The box is Pos (+0x0) - m and Pos + m, both with w = 1, where m = GetMaxElement (the largest of the three scale
 * components at +0x18). This encloses the ellipsoid whatever its rotation. Slot 6 of the ellipse volume vtable
 * cCTVolumeEllipse_vtbl.
 *
 * @param obj  the ellipse volume (this)
 * @return the bounding box (min, max), returned by value
 */
// Bounding box of a trigger volume: centre +- the largest half-extent.
extern "C" sCTBoxK2 cCTVolumeEllipse_boundBox(char* obj)
{
    float maxDist = GetMaxElement((float*)(obj + 0x18));
    sCTVec3K2 rv(maxDist, maxDist, maxDist);
    const sCTVec3K2& center = *(sCTVec3K2*)obj;
    return sCTBoxK2(sCTVec4K2(center - maxDist, 1.0f), sCTVec4K2(center + maxDist, 1.0f));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolumeEllipse_containsPoint);
#ifdef SKIP_ASM
struct sV3_172278 {
    float x, y, z;
    sV3_172278() {}
    sV3_172278(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

struct sV_172278 {
    float x, y, z, w;
    sV_172278() {}
    sV_172278(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
    sV_172278(const sV3_172278& v, float aw) { x = v.x; y = v.y; z = v.z; w = aw; }
} __attribute__((aligned(16)));

static inline sV3_172278 operator-(const sV3_172278& a)
{
    return sV3_172278(-a.x, -a.y, -a.z);
}

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_172278(void* dst, const void* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}


struct sM_172278 {
    float m[4][4];
    sM_172278() {}
    sM_172278(const sM_172278& o) { vu0CopyMtx_172278(this, &o); }
    sM_172278& operator=(const sM_172278& o) { vu0CopyMtx_172278(this, &o); return *this; }
} __attribute__((aligned(16)));

extern sM_172278 D_004FF1A0_v172278 __asm__("t4mat_Identity");
extern sV_172278 D_004FF140_v172278 __asm__("t4vect_XAxis");
extern sV_172278 D_004FF150_v172278 __asm__("t4vect_YAxis");
extern sV_172278 D_004FF160_v172278 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* sout, float* cout, float x);

// PORT: PS2-only VU0 inline asm (scale the rows of m by s.x, s.y, s.z, s.w).
static inline sM_172278 vu0ScaleMtx_172278(const sM_172278& m, const sV_172278& s)
{
    sM_172278 r;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulx.xyzw $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw $vf11, $vf7, $vf1w\n"
        "sqc2      $vf8, 0x0(%0)\n"
        "sqc2      $vf9, 0x10(%0)\n"
        "sqc2      $vf10, 0x20(%0)\n"
        "sqc2      $vf11, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(&r), "r"(&m), "m"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_172278(sM_172278* dst, const sM_172278* a, const sM_172278* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_172278(sM_172278* m, const sV_172278& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (r = m * v).
static inline sV_172278 vu0MulVec_172278(const sM_172278& m, const sV_172278& v)
{
    sV_172278 r;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set reorder\n"
        : "=m"(r)
        : "r"(&m), "m"(v)
        : "memory");
    return r;
}

static inline void SinCos_172278(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void RotAxis_172278(sM_172278* m, const sV_172278& axis, float angle)
{
    sM_172278 r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_172278(&s, &c, angle);
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    vu0MulMtx_172278(m, m, &r);
}

/**
 * Tells whether a world point lies inside an ellipse (ellipsoid) trigger volume.
 *
 * Builds the inverse of the volume's transform: t4mat_Identity scaled by 1 / scale (+0x18, +0x1C, +0x20), rotated about
 * t4vect_ZAxis by -heading (+0xC), about t4vect_YAxis by -roll (+0x14) and about t4vect_XAxis by -pitch (+0x10), then
 * translated by -position (+0x0). The point mapped into that unit-sphere space is inside when x*x + y*y + z*z <= 1.
 * Slot 7 of the ellipse volume vtable cCTVolumeEllipse_vtbl.
 *
 * @param point  the world point to test
 * @return 1 when the point is inside (or on) the ellipsoid, else 0
 */
// Ellipsoid volume test: is p inside the unit sphere after the inverse transform?
extern "C" int cCTVolumeEllipse_containsPoint(char* self, sV_172278* point)
{
    sM_172278 toLocal(D_004FF1A0_v172278);
    float one = 1.0f;
    toLocal = vu0ScaleMtx_172278(toLocal, sV_172278(one / *(float*)(self + 0x18), one / *(float*)(self + 0x1C), one / *(float*)(self + 0x20), one));
    RotAxis_172278(&toLocal, D_004FF160_v172278, -*(float*)(self + 0xC));
    RotAxis_172278(&toLocal, D_004FF150_v172278, -*(float*)(self + 0x14));
    RotAxis_172278(&toLocal, D_004FF140_v172278, -*(float*)(self + 0x10));
    vu0TransMtx_172278(&toLocal, sV_172278(-*(sV3_172278*)self, one));
    sV_172278 localPt = vu0MulVec_172278(toLocal, *point);
    return localPt.x * localPt.x + localPt.y * localPt.y + localPt.z * localPt.z <= one;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolumeBox_boundBox);
#ifdef SKIP_ASM
extern "C" float GetMaxElement(float* v);

struct sCTVec3K2b {
    float x, y, z;
    sCTVec3K2b() {}
    sCTVec3K2b(float a, float b, float c) : x(a), y(b), z(c) {}
};

static inline sCTVec3K2b operator-(const sCTVec3K2b& a, float s)
{
    return sCTVec3K2b(a.x - s, a.y - s, a.z - s);
}

static inline sCTVec3K2b operator+(const sCTVec3K2b& a, float s)
{
    return sCTVec3K2b(a.x + s, a.y + s, a.z + s);
}

struct sCTVec4K2b {
    float x, y, z, w;
    sCTVec4K2b() {}
    sCTVec4K2b(const sCTVec3K2b& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {}
} __attribute__((aligned(16)));

struct sCTBoxK2b {
    sCTVec4K2b min;
    sCTVec4K2b max;
    sCTBoxK2b(const sCTVec4K2b& a, const sCTVec4K2b& b) : min(a), max(b) {}
};

/**
 * Returns the bounding box of a box trigger volume: its position plus or minus its largest scale component.
 *
 * The same body as the ellipse version (cCTVolumeEllipse_boundBox): Pos (+0x0) -/+ GetMaxElement of the scale (+0x18), w = 1. Slot
 * 6 of the box volume vtable cCTVolumeBox_vtbl. For a box turned by its heading or pitch the corners reach up to sqrt(3) times
 * further than the largest half-extent, so such a box is not fully enclosed (likely bug).
 *
 * @param obj  the box volume (this)
 * @return the bounding box (min, max), returned by value
 */
// Bounding box of a trigger volume: centre +- the largest half-extent.
extern "C" sCTBoxK2b cCTVolumeBox_boundBox(char* obj)
{
    float maxDist = GetMaxElement((float*)(obj + 0x18));
    sCTVec3K2b rv(maxDist, maxDist, maxDist);
    const sCTVec3K2b& center = *(sCTVec3K2b*)obj;
    return sCTBoxK2b(sCTVec4K2b(center - maxDist, 1.0f), sCTVec4K2b(center + maxDist, 1.0f));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolumeBox_containsPoint);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector t4vect_XAxis;

extern t4Vector D_004FF150_28F8 __asm__("t4vect_YAxis");

extern t4Vector D_004FF160_28F8 __asm__("t4vect_ZAxis");

extern t4Matrix D_004FF1A0_28F8 __asm__("t4mat_Identity");

extern "C" void fSinCos(float* sout, float* cout, float x);


struct sVec3_28F8 {
    float x, y, z;
    sVec3_28F8() {}
    sVec3_28F8(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct sVec4_28F8 : t4Vector
{
    sVec4_28F8() {}
    sVec4_28F8(const float& ax, const float& ay, const float& az, const float& aw) { x = ax; y = ay; z = az; w = aw; }
    sVec4_28F8(const sVec3_28F8& v, const float& aw) { x = v.x; y = v.y; z = v.z; w = aw; }
} __attribute__((aligned(16)));

struct sMtx_28F8;

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, d = b * d).
static inline void vu0MulMatEq_28F8(sMtx_28F8* d, const sMtx_28F8* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0Copy_28F8(sMtx_28F8* d, const sMtx_28F8* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

struct sMtx_28F8 {
    float m[4][4];
    sMtx_28F8() {}
    sMtx_28F8(const sMtx_28F8& o) { vu0Copy_28F8(this, &o); }
    sMtx_28F8& operator=(const sMtx_28F8& o)
    {
        vu0Copy_28F8(this, &o);
        return *this;
    }
    sMtx_28F8& operator*=(const sMtx_28F8& b)
    {
        vu0MulMatEq_28F8(this, &b);
        return *this;
    }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector into the last matrix row).
static inline void vu0MulVecRow3_28F8(sMtx_28F8* m, const t4Vector& v)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, %1\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, 0x30(%0)\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector, vector loaded first).
static inline t4Vector vu0MulVec2_28F8(const sMtx_28F8* m, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

struct sXform_28F8 {
    sVec3_28F8 pos;     // 0x0
    float rot[3];       // 0xC
    sVec3_28F8 size;    // 0x18
};

// Multiply by a rotation of `angle` about the unit axis `a` (Rodrigues).
#define ROT_28F8(R, a, angle) \
    { \
        const float* ax = a; \
        float s, c; \
        fSinCos(&s, &c, angle); \
        float t = 1.0f - c; \
        float x = ax[0]; \
        float y = ax[1]; \
        float z = ax[2]; \
        float tx = t * x; \
        float ty = t * y; \
        float tz = t * z; \
        float sx = s * x; \
        float sy = s * y; \
        float sz = s * z; \
        R.m[0][0] = tx * x + c; \
        R.m[1][0] = tx * y + sz; \
        R.m[2][0] = tx * z - sy; \
        R.m[0][1] = ty * x - sz; \
        R.m[1][1] = ty * y + c; \
        R.m[2][1] = ty * z + sx; \
        R.m[0][2] = tz * x + sy; \
        R.m[1][2] = tz * y - sx; \
        R.m[2][2] = tz * z + c; \
        R.m[3][0] = 0.0f; \
        R.m[3][1] = 0.0f; \
        R.m[3][2] = 0.0f; \
        R.m[0][3] = 0.0f; \
        R.m[1][3] = 0.0f; \
        R.m[2][3] = 0.0f; \
        R.m[3][3] = 1.0f; \
    }

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_28F8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_28F8(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_28F8(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

extern int D_004A4DEC;              // guard of the static axis table below
extern t4Vector D_004C5480[3];      // static local: the three box axes

/**
 * Tells whether a world point lies inside a box trigger volume.
 *
 * Starts from t4mat_Identity, multiplies in a rotation about t4vect_XAxis by -pitch (rot[1], +0x10) and one about
 * t4vect_ZAxis by -heading (rot[0], +0xC), then a translation by -position, and maps @p pt into the box's space (w
 * forced to 1). The point is inside when, on each of the three axes (a static table D_004C5480 filled once with the X,
 * Y and Z axes, guarded by D_004A4DEC), it lies between -size and +size (the scale at +0x18 is a half-extent).
 *
 * Unlike the ellipse test (cCTVolumeEllipse_containsPoint) and the box bound object's limiter (cCTBoundObjBox_limitPoint), the roll (+0x14) is
 * ignored. Slot 7 of the box volume vtable cCTVolumeBox_vtbl.
 *
 * @param t  the box volume (this): position, heading/pitch/roll and half-extents
 * @param pt  the world point to test
 * @return 1 when the point is inside the box, else 0
 */
// cCTVolumeBox: is pt inside the (rotated) box?
extern "C" int cCTVolumeBox_containsPoint(const sXform_28F8* t, const t4Vector* pt)
{
    sMtx_28F8 toLocal = (const sMtx_28F8&)D_004FF1A0_28F8;
    {
        sMtx_28F8* toLocalPtr = &toLocal;
        sMtx_28F8 R;
        ROT_28F8(R, &t4vect_XAxis.x, -t->rot[1]);
        *toLocalPtr *= R;
    }
    {
        sMtx_28F8* toLocalPtr = &toLocal;
        sMtx_28F8 R;
        ROT_28F8(R, &D_004FF160_28F8.x, -t->rot[0]);
        *toLocalPtr *= R;
    }
    vu0MulVecRow3_28F8(&toLocal, sVec4_28F8(sVec3_28F8(-t->pos.x, -t->pos.y, -t->pos.z), 1.0f));
    t4Vector localPt = vu0MulVec2_28F8(&toLocal, *pt);
    localPt.w = 1.0f;
    t4Vector dmin = Sub_28F8(localPt, sVec4_28F8(sVec3_28F8(-t->size.x, -t->size.y, -t->size.z), 1.0f));
    t4Vector dmax = Sub_28F8(localPt, sVec4_28F8(t->size.x, t->size.y, t->size.z, 1.0f));
    if (D_004A4DEC == 0) {
        D_004C5480[0] = t4vect_XAxis;
        D_004C5480[1] = D_004FF150_28F8;
        D_004C5480[2] = D_004FF160_28F8;
        D_004A4DEC = 1;
    }
    for (int i = 0; i < 3; i++) {
        t4Vector n = Scale_28F8(D_004C5480[i], -1.0f);
        float planeDist = Dot_28F8(n, dmin);
        if (planeDist > 0.0f) {
            return 0;
        }
        planeDist = Dot_28F8(D_004C5480[i], dmax);
        if (planeDist > 0.0f) {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camvolume);
#ifdef SKIP_ASM
class cCTVolumeEllK2;
class cCTVolumeBoxK2;
extern "C" int get_cCTVolumeEllipse(void* reader, cCTVolumeEllK2* obj);
extern "C" int get_cCTVolumeBox(void* reader, cCTVolumeBoxK2* obj);
extern void* cCTVolumeEllipse_vtbl[];      // volume ellipse vtable
extern void* cCTVolumeBox_vtbl[];      // volume box vtable
extern char str_Camera_Sphere_Trigger[];
extern char str_Camera_Box_Trigger[];

struct sCTVolEllG : sCTBoundObjG {
    int type;           // 0x28
    sCTVolEllG() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTVolumeEllipse_vtbl) { type = 0; }
};

struct sCTVolBoxG : sCTBoundObjG {
    int type;           // 0x28
    sCTVolBoxG() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTVolumeBox_vtbl) { type = 1; }
};

/**
 * Reads a camera trigger volume from a trigger stream and allocates it.
 *
 * Reads the type word and constructs the matching volume: 0 "Camera Sphere Trigger" (an ellipse) or
 * 1 "Camera Box Trigger". For any other type it moves the cursor back over the type word and returns 0.
 *
 * @param reader  pointer to the stream's read cursor, advanced past what is read
 * @param out     receives the new volume
 * @return the number of bytes read, or 0 for an unknown type
 */
extern "C" int get_camvolume(void* reader, void** out)
{
    int type;
    int n = get_uint(reader, &type);
    switch (type) {
    case 0: {
        sCTVolEllG* volume = new (str_Camera_Sphere_Trigger, 0, 0) sCTVolEllG;
        int bytesRead = get_cCTVolumeEllipse(reader, (cCTVolumeEllK2*)volume);
        *out = volume;
        n += bytesRead;
        break;
    }
    case 1: {
        sCTVolBoxG* volume = new (str_Camera_Box_Trigger, 0, 0) sCTVolBoxG;
        int bytesRead = get_cCTVolumeBox(reader, (cCTVolumeBoxK2*)volume);
        *out = volume;
        n += bytesRead;
        break;
    }
    default:
        *(int*)reader -= n;
        return 0;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTVolumeEllipse);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTVolumeEllK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

/**
 * Reads an ellipse (sphere) trigger volume's data from a trigger stream.
 *
 * Sets the shape type to 0, then reads through the object's cCTObject accessors: position and scale vectors,
 * then heading, pitch and roll.
 *
 * @param reader  pointer to the stream's read cursor
 * @param obj     the volume to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTVolumeEllipse(void* reader, cCTVolumeEllK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 0;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTVolumeBox);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTVolumeBoxK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

/**
 * Reads a box trigger volume's data from a trigger stream.
 *
 * Sets the shape type to 1, then reads through the object's cCTObject accessors: position and scale vectors,
 * then heading, pitch and roll.
 *
 * @param reader  pointer to the stream's read cursor
 * @param obj     the volume to fill
 * @return the number of bytes read
 */
extern "C" int get_cCTVolumeBox(void* reader, cCTVolumeBoxK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 1;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", GetMaxElement);
#ifdef SKIP_ASM
/**
 * Returns the largest of three floats.
 *
 * Used on a trigger shape's scale (+0x18) by the volume bounding boxes cCTVolumeEllipse_boundBox and cCTVolumeBox_boundBox.
 *
 * @param halfExtents  the three components (x, y, z)
 * @return the largest component
 */
extern "C" float GetMaxElement(float* halfExtents)
{
    float x = halfExtents[0];
    float y = halfExtents[1];
    float largest = halfExtents[2];
    if (y < x) {
        if (largest < x) {
            return x;
        }
    } else if (largest < y) {
        largest = y;
    }
    return largest;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTriggerAlgorithm_cTriggerAlgorithm);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cChaseAlgorithm_cChaseAlgorithm(void* self);
extern void* cTriggerAlgorithm_vtbl[];
struct sQuad_00173208 {
    int v[4];
} __attribute__((aligned(16)));

extern sQuad_00173208 D_004FF130_00173208 __asm__("t4vect_Origin");

/**
 * Constructs a trigger camera algorithm: a chase camera that follows the rider from a configurable distance and angle.
 *
 * Runs the cChaseAlgorithm_cChaseAlgorithm base constructor, installs the vtable cTriggerAlgorithm_vtbl and sets AlgorithmType
 * (+0xC) to 90 (On Tour CAMERA_ALGORITHM_TRIGGER_GENERIC). The On Tour cTriggerAlgorithm members get their defaults:
 * Position (+0x40) = t4vect_Origin, Heading (+0x50) and Pitch (+0x54) 0, PitchMultiplier (+0x58) 0.8, HeadingAdjustRate
 * (+0x5C) 10.3, PitchAdjustRate (+0x60) 1.1, HeadingLookRate (+0x64) 5, PitchLookRate (+0x68) 10, HeightOffset (+0x6C)
 * 70, IdealDist (+0x70) 400, LeadDistance (+0x74) 0 and PitchOffset (+0x78) 20 (degrees).
 *
 * The bounded-camera constructor cBoundedAlgorithm_cBoundedAlgorithm builds on it.
 *
 * @return @p self
 */
extern "C" void* cTriggerAlgorithm_cTriggerAlgorithm(cTriggerAlgorithm* self)
{
    cChaseAlgorithm_cChaseAlgorithm(self);
    self->mVtbl = cTriggerAlgorithm_vtbl;
    self->PitchMultiplier = 0.800000011920929f;
    self->HeadingAdjustRate = 10.300000190734863f;
    self->PitchAdjustRate = 1.100000023841858f;
    self->HeadingLookRate = 5.0f;
    self->PitchLookRate = 10.0f;
    self->HeightOffset = 70.0f;
    self->IdealDist = 400.0f;
    *(int*)&self->LeadDistance = 0;
    self->PitchOffset = 20.0f;
    *(sQuad_00173208*)&self->Position = D_004FF130_00173208;
    self->AlgorithmType = 90;
    *(int*)&self->Pitch = 0;
    *(int*)&self->Heading = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTriggerAlgorithm__cTriggerAlgorithm);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// PORT: the unit declares cTriggerAlgorithm__cTriggerAlgorithm as `void* (void*)` (callers pass the
// flags through in $a1); the real body takes (self, flags). Bound by asm label.
void cTriggerAlgorithm__cTriggerAlgorithm_impl(void* self, int flags) __asm__("cTriggerAlgorithm__cTriggerAlgorithm");

/**
 * Destroys a trigger camera algorithm: puts back the base camera-algorithm vtable and frees the object when asked.
 *
 * Stores cCameraAlgorithm's vtable D_0045BDE0 at +0x10 and calls operator_delete when bit 0 of @p flags is set. Slot 1
 * of the trigger algorithm vtable cTriggerAlgorithm_vtbl; the bounded algorithm's destructor cBoundedAlgorithm__cBoundedAlgorithm chains to it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
void cTriggerAlgorithm__cTriggerAlgorithm_impl(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTriggerAlgorithm_snap);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

#include "vtable.h"

// 2-float vector (the horizontal part of a direction, for pitch = atan2(z, |xy|)).
struct sVec2 {
    float x, y;
    sVec2(float ax, float ay) : x(ax), y(ay) {}
};

extern t4Vector D_004FF160_32E8 __asm__("t4vect_ZAxis");

extern "C" float func_0031C228(float x);
extern "C" void fSinCos(float* sout, float* cout, float x);

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddEq_1732E8(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1732E8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_1732E8(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_1732E8(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_1732E8(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float wrap_1732E8(float x)
{
    return x - ffloor_1732E8(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

static inline float Len2_1732E8(const sVec2& v)
{
    return Sqrt_1732E8(v.x * v.x + v.y * v.y);
}

static inline float Atan2_1732E8(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

extern "C" void cTriggerAlgorithm_snap_x(char* self, long key) __asm__("cTriggerAlgorithm_snap");
/**
 * Snaps a trigger camera into place behind the rider it follows, when @p key names that rider.
 *
 * Does nothing unless @p key equals slot 7 (getAIIndex) of the target rider's cBeing part (Target +0x30, its Rider
 * +0x4, then +0x6C0). The focus point is Target's getPosition (slot 1) raised by HeightOffset (+0x6C) along
 * t4vect_ZAxis and pushed LeadDistance (+0x74) along getFront (slot 4). Heading (+0x50) becomes the wrapped yaw of the
 * front vector and Pitch (+0x54) the wrapped -PitchMultiplier (+0x58) x the front vector's pitch minus PitchOffset
 * (+0x78, degrees); Position (+0x40) is put IdealDist (+0x70) back from the focus along the direction given by that
 * heading and pitch. Finally it calls its own virtual update (slot 5, cTriggerAlgorithm_update for this class).
 *
 * @param key  the subject to snap for: the rider's AI index (On Tour subjectID)
 */
extern "C" void cTriggerAlgorithm_snap_x(char* self, long key)
{
    char* riderTarget = *(char**)(self + 0x30);
    char* riderPart = *(char**)(riderTarget + 4) + 0x6C0;
    sVEntry<long(void*)>* vEntry = &(*(sVEntry<long(void*)>**)riderPart)[7];
    if (key != vEntry->fn(riderPart + vEntry->delta))
        return;
    t4Vector focusPos, riderFront, dir;
    {
        char* posTarget = *(char**)(self + 0x30);
        sVEntry<t4Vector(void*)>* posSlot = &(*(sVEntry<t4Vector(void*)>**)posTarget)[1];
        focusPos = posSlot->fn(posTarget + posSlot->delta);
    }
    {
        char* frontTarget = *(char**)(self + 0x30);
        sVEntry<t4Vector(void*)>* frontSlot = &(*(sVEntry<t4Vector(void*)>**)frontTarget)[4];
        riderFront = frontSlot->fn(frontTarget + frontSlot->delta);
    }
    AddEq_1732E8(focusPos, Scale_1732E8(D_004FF160_32E8, *(float*)(self + 0x6C)));
    AddEq_1732E8(focusPos, Scale_1732E8(riderFront, *(float*)(self + 0x74)));
    *(float*)(self + 0x50) = wrap_1732E8(Atan2_1732E8(riderFront.y, riderFront.x));
    float frontPitch = Atan2_1732E8(riderFront.z, Len2_1732E8(sVec2(riderFront.x, riderFront.y)));
    float mm = -*(float*)(self + 0x78) * 0.01745329424738884f;
    *(float*)(self + 0x54) = wrap_1732E8(frontPitch * -*(float*)(self + 0x58) + mm);
    float sinHeading, cosHeading, sinPitch, cosPitch;
    fSinCos(&sinHeading, &cosHeading, *(float*)(self + 0x50));
    fSinCos(&sinPitch, &cosPitch, -*(float*)(self + 0x54));
    dir.x = cosPitch * cosHeading;
    dir.y = cosPitch * sinHeading;
    dir.z = -sinPitch;
    dir.w = 0.0f;
    *(t4Vector*)(self + 0x40) = Sub_1732E8(focusPos, Scale_1732E8(dir, *(float*)(self + 0x70)));
    sVEntry<void(void*)>* updateSlot = &(*(sVEntry<void(void*)>**)(self + 0x10))[5];
    updateSlot->fn(self + updateSlot->delta);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTriggerAlgorithm_snapCurrent);
#ifdef SKIP_ASM
struct sVE173678a {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sVE173678b {
    short delta;
    short index;
    void* (*fn)(void*, void*);
};

/**
 * Snaps a trigger camera for the rider it currently follows.
 *
 * Calls its own virtual snap (slot 3) with that rider's AI index (slot 7 of the rider's +0x6C0 part, reached through
 * Target +0x30 and its Rider +0x4).
 */
extern "C" void* cTriggerAlgorithm_snapCurrent(cTriggerAlgorithm* self)
{
    sVE173678b* vt = (sVE173678b*)self->mVtbl;
    char* riderIface = *(char**)((char*)self->Target + 0x4) + 0x6C0;
    sVE173678a* vt2 = *(sVE173678a**)riderIface;
    return vt[3].fn((char*)self + vt[3].delta, vt2[7].fn(riderIface + vt2[7].delta));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTriggerAlgorithm_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// 2-float vector (the horizontal part of a direction, for pitch = atan2(z, |xy|)).


extern t4Vector D_004FF160_36E0 __asm__("t4vect_ZAxis");

extern "C" float func_0031C228(float x);
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (d += b).
static inline void AddEq_1736E0(t4Vector& d, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(d)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (d = v * s).
static inline void ScaleTo_1736E0(t4Vector& d, const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    d = r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1736E0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b); g++ named-return-value extension.
static inline t4Vector SubN_1736E0(const t4Vector& a, const t4Vector& b) return r
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1736E0(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_1736E0(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only inline asm (float absolute value).
static inline float Abs_1736E0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_1736E0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float Len2_1736E0(const sVec2& v)
{
    return Sqrt_1736E0(v.x * v.x + v.y * v.y);
}

static inline float Atan2_1736E0(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

static inline float Wrap_1736E0(float v)
{
    return v - ffloor_1736E0(v * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

/**
 * Updates a trigger camera each frame: eases its position toward the wanted distance and angle behind the rider, then
 * turns it to look at the rider.
 *
 * The focus point is Target's getPosition (slot 1) + HeightOffset (+0x6C) along t4vect_ZAxis + LeadDistance (+0x74)
 * along getFront (slot 4); it becomes CamTargetPos (+0x20). The new heading moves the current camera-to-focus yaw
 * toward the rider's front yaw by HeadingAdjustRate (+0x5C) / 60 of the wrapped difference, and the new pitch moves
 * toward -PitchMultiplier (+0x58) x the front pitch + PitchOffset (+0x78, degrees) by PitchAdjustRate (+0x60) / 60. The
 * distance moves toward IdealDist (+0x70): at once when more than 5000 off, otherwise by a third of the error, or by
 * error x k / 60 when k = |getVelocity (slot 3)| x 0.0072 + 8 reaches 20.
 *
 * Position (+0x40) is set from those, then Heading (+0x50) and Pitch (+0x54) turn toward the new camera-to-focus
 * direction by HeadingLookRate (+0x64) / 60 and PitchLookRate (+0x68) / 60 (the pitch step is not wrapped).
 */
extern "C" void cTriggerAlgorithm_update(cTriggerAlgorithm* self)
{
    t4Vector focusPos, riderVel, riderFront;
    char* riderTarget = (char*)self->Target;
    sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)riderTarget)[1];
    ((void (*)(void*, void*))vEntry->fn)(&focusPos, riderTarget + vEntry->delta);
    riderTarget = (char*)self->Target;
    vEntry = &(*(sVEntry<t4Vector(void*)>**)riderTarget)[3];
    ((void (*)(void*, void*))vEntry->fn)(&riderVel, riderTarget + vEntry->delta);
    riderTarget = (char*)self->Target;
    vEntry = &(*(sVEntry<t4Vector(void*)>**)riderTarget)[4];
    ((void (*)(void*, void*))vEntry->fn)(&riderFront, riderTarget + vEntry->delta);
    {
        t4Vector scaledVec;
        ScaleTo_1736E0(scaledVec, D_004FF160_36E0, self->HeightOffset);
        AddEq_1736E0(focusPos, scaledVec);
        ScaleTo_1736E0(scaledVec, riderFront, self->LeadDistance);
        AddEq_1736E0(focusPos, scaledVec);
    }
    self->CamTargetPos = focusPos;
    t4Vector toFocus = Sub_1736E0(focusPos, self->Position);
    float yawD = Atan2_1736E0(toFocus.y, toFocus.x);
    float yawC = Atan2_1736E0(riderFront.y, riderFront.x);
    float diff = Wrap_1736E0(yawC) - yawD;
    float wd = Wrap_1736E0(diff);
    float len3 = Length_1736E0(toFocus);
    float dd = self->IdealDist - len3;
    float yawN = yawD + wd * self->HeadingAdjustRate * 0.01666666753590107f;
    float dist;
    if (Abs_1736E0(dd) > 5000.0f) {
        dist = len3 + dd;
    } else {
        float k = Length_1736E0(riderVel) * 0.007199999410659075f + 8.0f;
        float x;
        if (k >= 20.0f) {
            x = dd * k * 0.01666666753590107f;
        } else {
            x = dd * 0.3333333432674408f;
        }
        dist = len3 + x;
    }
    float pitchD = Atan2_1736E0(-toFocus.z, Len2_1736E0(sVec2(toFocus.x, toFocus.y)));
    float pitchC = Atan2_1736E0(riderFront.z, Len2_1736E0(sVec2(riderFront.x, riderFront.y)));
    float tp = pitchC * -self->PitchMultiplier + self->PitchOffset * 0.01745329424738884f;
    float pitchN = pitchD + (Wrap_1736E0(tp) - pitchD) * self->PitchAdjustRate * 0.01666666753590107f;
    t4Vector dir;
    t4Vector n;
    float sc0, sc1, sc2, sc3;
    fSinCos(&sc0, &sc1, yawN);
    fSinCos(&sc2, &sc3, pitchN);
    dir.x = sc3 * sc1;
    dir.y = sc3 * sc0;
    dir.z = -sc2;
    dir.w = 0.0f;
    {
        t4Vector scaledVec;
        ScaleTo_1736E0(scaledVec, dir, dist);
        n = SubN_1736E0(focusPos, scaledVec);
    }
    self->Position = n;
    n = SubN_1736E0(focusPos, self->Position);
    float yawE = Atan2_1736E0(n.y, n.x);
    float oy = self->Heading;
    self->Heading = oy + Wrap_1736E0(yawE - oy) * self->HeadingLookRate * 0.01666666753590107f;
    float pitchE = Atan2_1736E0(n.z, Len2_1736E0(sVec2(n.x, n.y)));
    float op = self->Pitch;
    self->Pitch = op + (pitchE - op) * self->PitchLookRate * 0.01666666753590107f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTriggerAlgorithm_getPose);
#ifdef SKIP_ASM
struct sV_173E40 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_173E40 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPair_173E40 {
    sV_173E40 a;
    sV_173E40 b;
};

extern sM_173E40 t4mat_Identity;
extern sV_173E40 t4vect_YAxis;
extern sV_173E40 D_004FF160_v173E40 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* sout, float* cout, float x);
extern "C" sPair_173E40 tPose_tPose(sM_173E40* m);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_173E40(sM_173E40* dst, const sM_173E40* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_173E40(sM_173E40* dst, const sM_173E40* a, const sM_173E40* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_173E40(sM_173E40* m, const sV_173E40& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

static inline void SinCos_173E40(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void RotAxis_173E40(sM_173E40* m, const sV_173E40& axis, float angle)
{
    sM_173E40 r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_173E40(&s, &c, angle);
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    vu0MulMtx_173E40(m, m, &r);
}

/**
 * Returns a trigger camera's pose: Position turned by Pitch and Heading.
 *
 * Starts from t4mat_Identity translated to Position (+0x40), rotates it about t4vect_YAxis by -Pitch (+0x54) and about
 * t4vect_ZAxis by Heading (+0x50), and splits the matrix into a position and a rotation quaternion with tPose_tPose.
 * Slot 2 of both the trigger (cTriggerAlgorithm_vtbl) and the bounded (cBoundedAlgorithm_vtbl) algorithm vtables.
 *
 * @param src  the trigger camera algorithm (this)
 * @return the pose: position and rotation quaternion (On Tour tPose), returned by value
 */
extern "C" sPair_173E40 cTriggerAlgorithm_getPose(char* src)
{
    sM_173E40 poseMtx;
    vu0CopyMtx_173E40(&poseMtx, &t4mat_Identity);
    vu0TransMtx_173E40(&poseMtx, *(sV_173E40*)(src + 0x40));
    RotAxis_173E40(&poseMtx, t4vect_YAxis, -*(float*)(src + 0x54));
    RotAxis_173E40(&poseMtx, D_004FF160_v173E40, *(float*)(src + 0x50));
    return tPose_tPose(&poseMtx);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBoundedAlgorithm_cBoundedAlgorithm);
#ifdef SKIP_ASM
extern void* cBoundedAlgorithm_vtbl[];
extern "C" void* cTriggerAlgorithm_cTriggerAlgorithm(cTriggerAlgorithm* self);

/**
 * Constructs a bounded trigger camera: a trigger camera whose position is kept inside a bound object.
 *
 * Runs the trigger algorithm constructor cTriggerAlgorithm_cTriggerAlgorithm, then sets AlgorithmType (+0xC) to 91 (On Tour
 * CAMERA_ALGORITHM_TRIGGER_BOUNDED), installs the vtable cBoundedAlgorithm_vtbl and zeroes BoundAction (+0x80) and LeadDistance
 * (+0x74). cChaseCameraController_createChaseAlgorithmBlend allocates it (0x90 bytes) for a camera type 91 action and
 * stores the action in BoundAction.
 *
 * @return @p self
 */
extern "C" void* cBoundedAlgorithm_cBoundedAlgorithm(cBoundedAlgorithm* self)
{
    cTriggerAlgorithm_cTriggerAlgorithm((cTriggerAlgorithm*)self);
    self->AlgorithmType = 91;
    self->mVtbl = cBoundedAlgorithm_vtbl;
    self->BoundAction = 0;
    *(int*)&self->LeadDistance = 0;
    return ((void*)self);
}
#endif

extern void* cBoundedAlgorithm_vtbl[];
extern "C" void* cTriggerAlgorithm__cTriggerAlgorithm(void*);

//99.5%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBoundedAlgorithm__cBoundedAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a bounded trigger camera: re-installs its vtable cBoundedAlgorithm_vtbl and chains to the trigger algorithm destructor.
 *
 * Stores cBoundedAlgorithm_vtbl at +0x10 and calls cTriggerAlgorithm__cTriggerAlgorithm, which puts back the base vtable and frees the object when bit 0
 * of the g++ destructor flags (passed through in $a1, not declared here) is set.
 *
 * @return cTriggerAlgorithm__cTriggerAlgorithm's result (the destructor returns nothing)
 */
void* cBoundedAlgorithm__cBoundedAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cBoundedAlgorithm_vtbl;
    return cTriggerAlgorithm__cTriggerAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBoundedAlgorithm_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// 2-float vector (the horizontal part of a direction, for pitch = atan2(z, |xy|)).


extern t4Vector D_004FF160_4200 __asm__("t4vect_ZAxis");

extern "C" float func_0031C228(float x);

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_174200(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (d = a + b).
static inline void AddTo_174200(t4Vector& d, const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    d = r;
}

// PORT: PS2-only VU0 inline asm (d = v * s).
static inline void ScaleTo_174200(t4Vector& d, const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    d = r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_174200(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_174200(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_174200(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Len2_174200(const sVec2& v)
{
    return Sqrt_174200(v.x * v.x + v.y * v.y);
}

static inline float Atan2_174200(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

/**
 * Updates a bounded trigger camera: takes its settings from the bound action, keeps Position inside the bound object
 * and aims at the target.
 *
 * Copies the action's (BoundAction +0x80, On Tour cCTActionBoundedCam) IdealDist (+0x10), HeightOffset (+0x18),
 * PitchOffset (+0x1C), FieldOfView (+0x14, into View +0x0) and LeadDistance (+0x20) into the algorithm. The look target
 * is the action's TargetPoint (+0x28, w = 1) when its TargetType (+0x24) is 1 (On Tour eCTTargetPoint), otherwise
 * Target's getPosition (slot 1) + LeadDistance along getFront (slot 4) + HeightOffset along t4vect_ZAxis; it becomes
 * CamTargetPos (+0x20). The action's BoundObj (+0x8) then limits Position (+0x40) through its virtual slot 6
 * (limitPoint), and Heading (+0x50) and Pitch (+0x54) are set to look straight from Position at the target.
 */
extern "C" void cBoundedAlgorithm_update(cBoundedAlgorithm* self)
{
    char* act = (char*)self->BoundAction;
    self->IdealDist = *(float*)(act + 0x10);
    self->HeightOffset = *(float*)(act + 0x18);
    self->PitchOffset = *(float*)(act + 0x1C);
    self->View = *(float*)(act + 0x14);
    self->LeadDistance = *(float*)(act + 0x20);
    t4Vector lookTarget;
    if (*(int*)(act + 0x24) == 1) {
        lookTarget.x = *(float*)(act + 0x28);
        lookTarget.y = *(float*)(act + 0x2C);
        lookTarget.z = *(float*)(act + 0x30);
        lookTarget.w = 1.0f;
    } else {
        t4Vector leadPos, riderPos, leadOffset, riderFront;
        char* calleeObj = (char*)self->Target;
        sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)calleeObj)[1];
        ((void (*)(void*, void*))vEntry->fn)(&riderPos, calleeObj + vEntry->delta);
        float k = self->LeadDistance;
        calleeObj = (char*)self->Target;
        vEntry = &(*(sVEntry<t4Vector(void*)>**)calleeObj)[4];
        ((void (*)(void*, void*))vEntry->fn)(&riderFront, calleeObj + vEntry->delta);
        ScaleTo_174200(leadOffset, riderFront, k);
        AddTo_174200(leadPos, riderPos, leadOffset);
        t4Vector heightLift;
        ScaleTo_174200(heightLift, D_004FF160_4200, self->HeightOffset);
        AddTo_174200(lookTarget, leadPos, heightLift);
    }
    self->CamTargetPos = lookTarget;
    {
        char* calleeObj = *(char**)((char*)self->BoundAction + 8);
        sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)(calleeObj + 0x24))[6];
        ((void (*)(void*, void*))vEntry->fn)(calleeObj + vEntry->delta, ((char*)self) + 0x40);
    }
    t4Vector toTarget = Sub_174200(lookTarget, self->Position);
    self->Heading = Atan2_174200(toTarget.y, toTarget.x);
    self->Pitch = Atan2_174200(toTarget.z, Len2_174200(sVec2(toTarget.x, toTarget.y)));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBoundedAlgorithm_snap);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// 2-float vector (the horizontal part of a direction, for pitch = atan2(z, |xy|)).


extern t4Vector D_004FF160_44B0 __asm__("t4vect_ZAxis");

extern "C" float func_0031C228(float x);

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_1744B0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (d = a + b).
static inline void AddTo_1744B0(t4Vector& d, const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    d = r;
}

// PORT: PS2-only VU0 inline asm (d = v * s).
static inline void ScaleTo_1744B0(t4Vector& d, const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    d = r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1744B0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_1744B0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_1744B0(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Len2_1744B0(const sVec2& v)
{
    return Sqrt_1744B0(v.x * v.x + v.y * v.y);
}

static inline float Atan2_1744B0(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

extern "C" void cTriggerAlgorithm_snap(void* self, long a1);

/**
 * Snaps a bounded trigger camera into place for the rider @p subjectId names: the trigger camera snap, then the bound object
 * limit.
 *
 * Does nothing unless @p subjectId equals slot 7 (getAIIndex) of the target rider's +0x6C0 part. Computes the look target like
 * cBoundedAlgorithm_update (the action's TargetPoint for TargetType 1, else rider position + lead + height), but with the
 * LeadDistance (+0x74) and HeightOffset (+0x6C) the algorithm held before; only then copies IdealDist, HeightOffset,
 * PitchOffset, FieldOfView and LeadDistance from BoundAction (+0x80). Runs the trigger algorithm snap cTriggerAlgorithm_snap,
 * lets the action's BoundObj limit Position (slot 6), and aims Heading (+0x50) and Pitch (+0x54) from Position at the
 * look target.
 *
 * Quirk: on the first snap after a new action the look target uses the previous action's lead and height.
 *
 * @param subjectId  the subject to snap for: the rider's AI index (On Tour subjectID)
 */
extern "C" void cBoundedAlgorithm_snap(cBoundedAlgorithm* self, long subjectId)
{
    {
        char* calleeObj = *(char**)((char*)self->Target + 4) + 0x6C0;
        sVEntry<long(void*)>* vEntry = &(*(sVEntry<long(void*)>**)calleeObj)[7];
        if (vEntry->fn(calleeObj + vEntry->delta) != subjectId)
            return;
    }
    char* act = (char*)self->BoundAction;
    t4Vector lookTarget;
    if (*(int*)(act + 0x24) == 1) {
        lookTarget.x = *(float*)(act + 0x28);
        lookTarget.y = *(float*)(act + 0x2C);
        lookTarget.z = *(float*)(act + 0x30);
        lookTarget.w = 1.0f;
    } else {
        t4Vector leadPos, riderPos, leadOffset, riderFront;
        char* calleeObj = (char*)self->Target;
        sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)calleeObj)[1];
        ((void (*)(void*, void*))vEntry->fn)(&riderPos, calleeObj + vEntry->delta);
        float k = self->LeadDistance;
        calleeObj = (char*)self->Target;
        vEntry = &(*(sVEntry<t4Vector(void*)>**)calleeObj)[4];
        ((void (*)(void*, void*))vEntry->fn)(&riderFront, calleeObj + vEntry->delta);
        ScaleTo_1744B0(leadOffset, riderFront, k);
        AddTo_1744B0(leadPos, riderPos, leadOffset);
        t4Vector heightLift;
        ScaleTo_1744B0(heightLift, D_004FF160_44B0, self->HeightOffset);
        AddTo_1744B0(lookTarget, leadPos, heightLift);
    }
    char* act2 = (char*)self->BoundAction;
    self->IdealDist = *(float*)(act2 + 0x10);
    self->HeightOffset = *(float*)(act2 + 0x18);
    self->PitchOffset = *(float*)(act2 + 0x1C);
    self->View = *(float*)(act2 + 0x14);
    self->LeadDistance = *(float*)(act2 + 0x20);
    cTriggerAlgorithm_snap(((char*)self), subjectId);
    {
        char* calleeObj = *(char**)((char*)self->BoundAction + 8);
        sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)(calleeObj + 0x24))[6];
        ((void (*)(void*, void*))vEntry->fn)(calleeObj + vEntry->delta, ((char*)self) + 0x40);
    }
    t4Vector toTarget = Sub_1744B0(lookTarget, self->Position);
    self->Heading = Atan2_1744B0(toTarget.y, toTarget.x);
    self->Pitch = Atan2_1744B0(toTarget.z, Len2_1744B0(sVec2(toTarget.x, toTarget.y)));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBoundedAlgorithm_snapCurrent);
#ifdef SKIP_ASM
struct sVE1747A0a {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sVE1747A0b {
    short delta;
    short index;
    void* (*fn)(void*, void*);
};

/**
 * Snaps a bounded trigger camera for the rider it currently follows.
 *
 * Calls its own virtual snap (slot 3, cBoundedAlgorithm_snap) with that rider's AI index (slot 7 of the rider's +0x6C0 part,
 * through Target +0x30 and its Rider +0x4).
 */
extern "C" void* cBoundedAlgorithm_snapCurrent(cBoundedAlgorithm* self)
{
    sVE1747A0b* vt = (sVE1747A0b*)self->mVtbl;
    char* riderIface = *(char**)((char*)self->Target + 0x4) + 0x6C0;
    sVE1747A0a* vt2 = *(sVE1747A0a**)riderIface;
    return vt[3].fn((char*)self + vt[3].delta, vt2[7].fn(riderIface + vt2[7].delta));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00174808);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest (variadic).
 *
 * The body only spills the other argument registers. Nothing in the ELF calls it or stores its address; each of the
 * following units has one such stub too (func_00177610, func_00178F18, func_00179758).
 *
 * @return the first argument
 */
extern "C" void* func_00174808(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", nash_privy_QueryHelper_helpX);
#ifdef SKIP_ASM
// Camera-trigger octree query (REF crates/ssx3-game/src/game/camera/trigger/cameratriggerfactory.rs,
// "CollectOctreeTriggersInBox"): adds the node's triggers, then visits the eight children whose
// cell (grown by 0.2) overlaps the box; children wholly inside take the plain subtree walk.
struct sVEnt_4848 {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct sOctNode_4848 {
    sOctNode_4848* child[8];        // 0x00
    char pad_0x20[0x8];
    char* list;                     // 0x28 trigger entries
};

struct sCell_4848 {
    int level;
    int u;
    int v;
    int w;
};

struct sBox_4848 {
    float minx, miny, minz, pad0;
    float maxx, maxy, maxz, pad1;
};

extern "C" void* cActiveTriggerList_find(void*, void*);
extern "C" void cActiveTriggerList_add(void*, void*);
extern "C" void descend(void* node, void* ctx);

static inline void WalkList_4848(char* e, void* ctx)
{
    for (; e != 0; e = *(char**)e) {
        if (*(int*)(e + 8) == 4) {
            char* trig = *(char**)(e + 0xC);
            char* sub = *(char**)((char*)ctx + 4) + 0x6C0;
            char* bound = *(char**)(trig + 8);
            sVEnt_4848* cvt = *(sVEnt_4848**)sub;
            sVEnt_4848* bvt = *(sVEnt_4848**)(bound + 0x24);
            char* bthis = bound + bvt[7].delta;
            int r = cvt[5].fn(sub + cvt[5].delta);
            if (bvt[7].fn(bthis, r)) {
                void* f = cActiveTriggerList_find(*(void**)ctx, trig);
                if (f) {
                    *(int*)((char*)f + 0xC) = 1;
                } else {
                    cActiveTriggerList_add(*(void**)ctx, trig);
                }
            }
        }
    }
}

static inline void Subtree_4848(sOctNode_4848* node, void* ctx)
{
    WalkList_4848(node->list, ctx);
    if (node->child[0]) descend(node->child[0], ctx);
    if (node->child[1]) descend(node->child[1], ctx);
    if (node->child[2]) descend(node->child[2], ctx);
    if (node->child[3]) descend(node->child[3], ctx);
    if (node->child[4]) descend(node->child[4], ctx);
    if (node->child[5]) descend(node->child[5], ctx);
    if (node->child[6]) descend(node->child[6], ctx);
    if (node->child[7]) descend(node->child[7], ctx);
}

/**
 * Collects the camera triggers whose volume holds the rider from an octree cell that crosses the query box, recursing
 * into its children.
 *
 * First runs the crossing callback (@p ctx1, inlined) on the cell's own node list (+0x28): every entry of type 4 is a
 * camera trigger, and when the trigger's Volume (+0x8) contains the rider's position (volume slot 7 containsPoint,
 * rider +0x6C0 slot 5 getPosition) the trigger is flagged in the active list (node +0xC = 1, found with cActiveTriggerList_find)
 * or added to it (cActiveTriggerList_add). Cells at level 11 stop there.
 *
 * Otherwise the children are 2^(level - 1) units wide; for each axis it checks which of the two child spans, widened by
 * 0.2 cell on both sides, overlap the box (returning when neither does) and which lie wholly inside it. It then visits
 * the eight children in the order 0, 1, 3, 2, 6, 7, 5, 4 with the child key kept up to date: an overlapping child
 * wholly inside the box gets the inside callback (@p ctx2) on its own list and descend on each of its children,
 * any other overlapping child is handled by a recursive call.
 *
 * Called by cCameraTriggerMan_checkCollisions for the root cells of the world's octree (SSXApp->Game->BXNash) that cross the rider's box.
 *
 * @param hit  the query helper object (this); unused, the caller passes a byte of a local flag array
 * @param ctx1  the crossing callback (On Tour CameraCollisionCallback: the active trigger list and the rider)
 * @param ctx2  the callback for cells wholly inside the box (the same kind of object)
 * @param box  the query box (min, max)
 * @param node  the octree cell (On Tour Cell: children[2][2][2], node lists from +0x20)
 * @param cell  the cell's key: level and x, y, z cell coordinates (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_helpX(int hit, void* ctx1, void* ctx2, sBox_4848* box, sOctNode_4848* node, sCell_4848* cell)
{
    sCell_4848 childCell;
    sOctNode_4848* ch;

    WalkList_4848(node->list, ctx1);
    if (cell->level == 11) {
        return;
    }
    int bits = (cell->level + 0x7E) << 23;
    float size = *(float*)&bits;
    childCell.level = cell->level - 1;
    childCell.u = cell->u * 2;
    childCell.v = cell->v * 2;
    childCell.w = cell->w * 2;

    float ulo0 = ((float)childCell.u - 0.2f) * size;
    float uhi0 = ((float)(childCell.u + 1) + 0.2f) * size;
    int fu0 = 0;
    if (box->minx < uhi0 && ulo0 < box->maxx) fu0 = 1;
    float ulo1 = ((float)(childCell.u + 1) - 0.2f) * size;
    float uhi1 = ((float)(childCell.u + 2) + 0.2f) * size;
    int fu1 = 0;
    if (box->minx < uhi1 && ulo1 < box->maxx) fu1 = 1;
    if (!fu0 && !fu1) return;

    float vlo0 = ((float)childCell.v - 0.2f) * size;
    float vhi0 = ((float)(childCell.v + 1) + 0.2f) * size;
    int fv0 = 0;
    if (box->miny < vhi0 && vlo0 < box->maxy) fv0 = 1;
    float vlo1 = ((float)(childCell.v + 1) - 0.2f) * size;
    float vhi1 = ((float)(childCell.v + 2) + 0.2f) * size;
    int fv1 = 0;
    if (box->miny < vhi1 && vlo1 < box->maxy) fv1 = 1;
    if (!fv0 && !fv1) return;

    float wlo0 = ((float)childCell.w - 0.2f) * size;
    float whi0 = ((float)(childCell.w + 1) + 0.2f) * size;
    int fw0 = 0;
    if (box->minz < whi0 && wlo0 < box->maxz) fw0 = 1;
    float wlo1 = ((float)(childCell.w + 1) - 0.2f) * size;
    float whi1 = ((float)(childCell.w + 2) + 0.2f) * size;
    int fw1 = 0;
    if (box->minz < whi1 && wlo1 < box->maxz) fw1 = 1;
    if (!fw0 && !fw1) return;

    int iu0 = 0;
    if (box->minx < ulo0 && uhi0 < box->maxx) iu0 = 1;
    int iu1 = 0;
    if (box->minx < ulo1 && uhi1 < box->maxx) iu1 = 1;
    int iv0 = 0;
    if (box->miny < vlo0 && vhi0 < box->maxy) iv0 = 1;
    int iv1 = 0;
    if (box->miny < vlo1 && vhi1 < box->maxy) iv1 = 1;
    int iw0 = 0;
    if (box->minz < wlo0 && whi0 < box->maxz) iw0 = 1;
    int iw1 = 0;
    if (box->minz < wlo1 && whi1 < box->maxz) iw1 = 1;

    ch = node->child[0];
    if (ch != 0 && fu0 && fv0 && fw0) {
        if (iu0 && iv0 && iw0) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
    childCell.w++;
    ch = node->child[1];
    if (ch != 0 && fu0 && fv0 && fw1) {
        if (iu0 && iv0 && iw1) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
    childCell.v++;
    ch = node->child[3];
    if (ch != 0 && fu0 && fv1 && fw1) {
        if (iu0 && iv1 && iw1) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
    childCell.w--;
    ch = node->child[2];
    if (ch != 0 && fu0 && fv1 && fw0) {
        if (iu0 && iv1 && iw0) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
    childCell.u++;
    ch = node->child[6];
    if (ch != 0 && fu1 && fv1 && fw0) {
        if (iu1 && iv1 && iw0) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
    childCell.w++;
    ch = node->child[7];
    if (ch != 0 && fu1 && fv1 && fw1) {
        if (iu1 && iv1 && iw1) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
    childCell.v--;
    ch = node->child[5];
    if (ch != 0 && fu1 && fv0 && fw1) {
        if (iu1 && iv0 && iw1) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
    childCell.w--;
    ch = node->child[4];
    if (ch != 0 && fu1 && fv0 && fw0) {
        if (iu1 && iv0 && iw0) {
            Subtree_4848(ch, ctx2);
        } else {
            nash_privy_QueryHelper_helpX(hit, ctx1, ctx2, box, ch, &childCell);
        }
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00175A20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern float D_004C54B0[4];
extern float D_004C54C0[4];
extern float D_004C54D0[4];
extern float D_004C54E0[4];
extern float D_004C54F0[4];
extern float D_004C5500[4];
extern float D_004C5510[4];
extern float D_004C5520[4];
extern float D_004C5530[4];
extern float D_004C5540[4];
extern float D_004C5550[4];
extern float D_004C5560[4];
extern float D_004C5570[4];
extern float D_004C5580[4];
extern float D_004C5590[4];
extern float D_004C55A0[4];
extern float D_004C55B0[4];
extern float D_004C55C0[4];
extern float D_004C55D0[4];
extern float D_004C55E0[4];
extern float D_004C55F0[4];
extern float D_004C5600[4];
extern float D_004C5610[4];
extern float D_004C5620[4];
extern float D_004C5630[4];
extern float D_004C5640[4];
extern float D_004C5650[4];
extern float D_004C5660[4];
extern float D_004C5670[4];
extern float D_004C5680[4];
extern float D_004C5690[4];
extern float D_004C56A0[4];
extern float D_004C56B0[4];
extern float D_004C56C0[4];
extern float D_004C56D0[4];
extern float D_004C56E0[4];
extern float D_004C56F0[4];
extern float D_004C5700[4];
extern float D_004C5710[4];
extern float D_004C5720[4];
extern float D_004C5730[4];
extern float D_004C5740[4];
extern float D_004C5750[4];
extern float D_004C5760[4];
extern float D_004C5770[4];
extern float D_004C5780[4];
extern float D_004C5790[4];
extern float D_004C57A0[4];
extern float D_004C57B0[4];
extern float D_004C57C0[4];
extern float D_004C57D0[4];
extern float D_004C57E0[4];
extern float D_004C57F0[4];
extern float D_004C5800[4];
extern float D_004C5810[4];
extern float D_004C5820[4];
extern "C" void* cCameraTriggerMan_cCameraTriggerMan(void*);
extern "C" void cCameraTriggerMan__cCameraTriggerMan(void*, int);
extern int D_004A4DF0;
void sti_00175A20(int init, int prio) __asm__("func_00175A20");
/**
 * Runs the static constructors or destructors of this unit's globals (g++'s static initialization and destruction).
 *
 * Only priority 0xFFFF does anything. With @p init set it fills the 56 four-float constants D_004C54B0..D_004C5820 (the
 * colour and blend vectors every unit of this kind sets up; nothing else in the ELF reads these copies), sets the word
 * D_004A4DF0 to all ones (its low byte, then the upper 24 bits) and constructs the camera trigger manager
 * gCamTriggerMan with cCameraTriggerMan_cCameraTriggerMan. With @p init clear it destroys gCamTriggerMan
 * (cCameraTriggerMan__cCameraTriggerMan, flags 2). func_00176A28 calls it with (1, 0xFFFF) and func_00176A48 with (0,
 * 0xFFFF).
 *
 * @param init  1 to construct, 0 to destroy
 * @param prio  the init priority; only 0xFFFF is handled
 */
void sti_00175A20(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init) {
            D_004C54B0[0] = 1.0f;
            D_004C54B0[1] = 1.0f;
            D_004C54B0[2] = 1.0f;
            D_004C54B0[3] = 0.5f;
            D_004C54C0[0] = 0.4998680055141449f;
            D_004C54C0[1] = 1.0f;
            D_004C54C0[2] = 1.0f;
            D_004C54C0[3] = 0.0f;
            D_004C54D0[0] = 0.12951700389385223f;
            D_004C54D0[1] = 1.0f;
            D_004C54D0[2] = 1.0f;
            D_004C54D0[3] = 0.0f;
            D_004C54E0[0] = 0.22426900267601013f;
            D_004C54E0[1] = 0.9402310252189636f;
            D_004C54E0[2] = 0.5425059795379639f;
            D_004C54E0[3] = 0.0004030000127386302f;
            D_004C54F0[0] = 0.4652079939842224f;
            D_004C54F0[1] = 0.32170501351356506f;
            D_004C54F0[2] = 0.9943439960479736f;
            D_004C54F0[3] = 0.0f;
            D_004C5500[0] = 0.17792600393295288f;
            D_004C5500[1] = 1.0f;
            D_004C5500[2] = 1.0f;
            D_004C5500[3] = 0.0f;
            D_004C5510[0] = 0.4411740005016327f;
            D_004C5510[1] = 0.41388601064682007f;
            D_004C5510[2] = 0.3211260139942169f;
            D_004C5510[3] = 0.4398239850997925f;
            D_004C5520[0] = 0.500997006893158f;
            D_004C5520[1] = 0.24849599599838257f;
            D_004C5520[2] = 1.0f;
            D_004C5520[3] = 1.0f;
            D_004C5530[0] = 0.27687299251556396f;
            D_004C5530[1] = 0.6395620107650757f;
            D_004C5530[2] = 0.13423900306224823f;
            D_004C5530[3] = 0.0f;
            D_004C5540[0] = 0.8936439752578735f;
            D_004C5540[1] = 0.5263469815254211f;
            D_004C5540[2] = 0.5051450133323669f;
            D_004C5540[3] = 0.8687300086021423f;
            D_004C5550[0] = 1.0f;
            D_004C5550[1] = 1.0f;
            D_004C5550[2] = 1.0f;
            D_004C5550[3] = 1.0f;
            D_004C5560[0] = 1.0f;
            D_004C5560[1] = 0.0f;
            D_004C5560[2] = 0.0f;
            D_004C5560[3] = 0.0f;
            D_004C5570[0] = 1.0f;
            D_004C5570[1] = 0.550000011920929f;
            D_004C5570[2] = 0.5350000262260437f;
            D_004C5570[3] = 0.550000011920929f;
            D_004C5580[0] = 1.0f;
            D_004C5580[1] = 1.0f;
            D_004C5580[2] = 1.0f;
            D_004C5580[3] = 1.0f;
            D_004C5590[0] = 1.0f;
            D_004C5590[1] = 1.0f;
            D_004C5590[2] = 1.0f;
            D_004C5590[3] = 1.0f;
            D_004C55A0[0] = 1.0f;
            D_004C55A0[1] = 0.0f;
            D_004C55A0[2] = 0.0f;
            D_004C55A0[3] = 0.0f;
            D_004C55B0[0] = 1.0f;
            D_004C55B0[1] = 0.0f;
            D_004C55B0[2] = 0.0f;
            D_004C55B0[3] = 0.0f;
            D_004C55C0[0] = 1.0f;
            D_004C55C0[1] = 1.0f;
            D_004C55C0[2] = 1.0f;
            D_004C55C0[3] = 1.0f;
            D_004C55D0[0] = 1.0f;
            D_004C55D0[1] = 0.5f;
            D_004C55D0[2] = 1.0f;
            D_004C55D0[3] = 0.5f;
            D_004C55E0[0] = 1.0f;
            D_004C55E0[1] = 0.5f;
            D_004C55E0[2] = 1.0f;
            D_004C55E0[3] = 0.5f;
            D_004C55F0[0] = 1.0f;
            D_004C55F0[1] = 0.5f;
            D_004C55F0[2] = 1.0f;
            D_004C55F0[3] = 0.5f;
            D_004C5600[0] = 1.0f;
            D_004C5600[1] = 0.7828119993209839f;
            D_004C5600[2] = 0.06171900033950806f;
            D_004C5600[3] = 0.03984399884939194f;
            D_004C5610[0] = 1.0f;
            D_004C5610[1] = 1.0f;
            D_004C5610[2] = 0.7988259792327881f;
            D_004C5610[3] = 0.0f;
            D_004C5620[0] = 0.9886710047721863f;
            D_004C5620[1] = 0.9230499863624573f;
            D_004C5620[2] = 0.34311699867248535f;
            D_004C5620[3] = 0.0f;
            D_004C5630[0] = 1.0f;
            D_004C5630[1] = 1.0f;
            D_004C5630[2] = 1.0f;
            D_004C5630[3] = 1.0f;
            D_004C5640[0] = 1.0f;
            D_004C5640[1] = 1.0f;
            D_004C5640[2] = 0.7976430058479309f;
            D_004C5640[3] = 0.0f;
            D_004C5650[0] = 1.0f;
            D_004C5650[1] = 0.9222670197486877f;
            D_004C5650[2] = 0.34179699420928955f;
            D_004C5650[3] = 0.0f;
            D_004C5660[0] = 1.0f;
            D_004C5660[1] = 0.7847639918327332f;
            D_004C5660[2] = 0.06054700165987015f;
            D_004C5660[3] = 0.04062499850988388f;
            D_004C5670[0] = 1.0f;
            D_004C5670[1] = 0.800000011920929f;
            D_004C5670[2] = 0.44999998807907104f;
            D_004C5670[3] = 1.0f;
            D_004C5680[0] = 1.0f;
            D_004C5680[1] = 0.3281260132789612f;
            D_004C5680[2] = 1.0f;
            D_004C5680[3] = 0.3539060056209564f;
            D_004C5690[0] = 1.0f;
            D_004C5690[1] = 1.0f;
            D_004C5690[2] = 1.0f;
            D_004C5690[3] = 1.0f;
            D_004C56A0[0] = 1.0f;
            D_004C56A0[1] = 1.0f;
            D_004C56A0[2] = 1.0f;
            D_004C56A0[3] = 1.0f;
            D_004C56B0[0] = 1.0f;
            D_004C56B0[1] = 0.8613290190696716f;
            D_004C56B0[2] = 0.38124799728393555f;
            D_004C56B0[3] = 0.0f;
            D_004C56C0[0] = 1.0f;
            D_004C56C0[1] = 1.0f;
            D_004C56C0[2] = 1.0f;
            D_004C56C0[3] = 1.0f;
            D_004C56D0[0] = 1.0f;
            D_004C56D0[1] = 0.3244380056858063f;
            D_004C56D0[2] = 1.0f;
            D_004C56D0[3] = 0.0f;
            D_004C56E0[0] = 1.0f;
            D_004C56E0[1] = 1.0f;
            D_004C56E0[2] = 1.0f;
            D_004C56E0[3] = 1.0f;
            D_004C56F0[0] = 1.0f;
            D_004C56F0[1] = 1.0f;
            D_004C56F0[2] = 0.0f;
            D_004C56F0[3] = 0.0f;
            D_004C5700[0] = 0.7007060050964355f;
            D_004C5700[1] = 0.0f;
            D_004C5700[2] = 0.0f;
            D_004C5700[3] = 0.0f;
            D_004C5710[0] = 1.0f;
            D_004C5710[1] = 0.7828530073165894f;
            D_004C5710[2] = 0.06140600144863129f;
            D_004C5710[3] = 0.04146299883723259f;
            D_004C5720[0] = 1.0f;
            D_004C5720[1] = 0.32487601041793823f;
            D_004C5720[2] = 1.0f;
            D_004C5720[3] = 0.35280099511146545f;
            D_004C5730[0] = 1.0f;
            D_004C5730[1] = 1.0f;
            D_004C5730[2] = 0.7977359890937805f;
            D_004C5730[3] = 0.0f;
            D_004C5740[0] = 1.0f;
            D_004C5740[1] = 0.4937489926815033f;
            D_004C5740[2] = 1.0f;
            D_004C5740[3] = 0.49179700016975403f;
            D_004C5750[0] = 1.0f;
            D_004C5750[1] = 1.0f;
            D_004C5750[2] = 1.0f;
            D_004C5750[3] = 1.0f;
            D_004C5760[0] = 1.0f;
            D_004C5760[1] = 0.800000011920929f;
            D_004C5760[2] = 1.0f;
            D_004C5760[3] = 0.800000011920929f;
            D_004C5770[0] = 1.0f;
            D_004C5770[1] = 1.0f;
            D_004C5770[2] = 0.0f;
            D_004C5770[3] = 0.0f;
            D_004C5780[0] = 1.0f;
            D_004C5780[1] = 1.0f;
            D_004C5780[2] = 0.0f;
            D_004C5780[3] = 0.0f;
            D_004C5790[0] = 1.0f;
            D_004C5790[1] = 1.0f;
            D_004C5790[2] = 0.0f;
            D_004C5790[3] = 0.0f;
            D_004C57A0[0] = 1.0f;
            D_004C57A0[1] = 0.3273420035839081f;
            D_004C57A0[2] = 1.0f;
            D_004C57A0[3] = 0.35390499234199524f;
            D_004C57B0[0] = 1.0f;
            D_004C57B0[1] = 1.0f;
            D_004C57B0[2] = 0.061000000685453415f;
            D_004C57B0[3] = 0.039000000804662704f;
            D_004C57C0[0] = 1.0f;
            D_004C57C0[1] = 1.0f;
            D_004C57C0[2] = 0.7999979853630066f;
            D_004C57C0[3] = 0.0f;
            D_004C57D0[0] = 1.0f;
            D_004C57D0[1] = 0.9589840173721313f;
            D_004C57D0[2] = 0.6143789887428284f;
            D_004C57D0[3] = 0.0f;
            D_004C57E0[0] = 1.0f;
            D_004C57E0[1] = 0.9285150170326233f;
            D_004C57E0[2] = 0.3399839997291565f;
            D_004C57E0[3] = 0.0f;
            D_004C57F0[0] = 1.0f;
            D_004C57F0[1] = 0.8738250136375427f;
            D_004C57F0[2] = 0.24931700527668f;
            D_004C57F0[3] = 0.017578000202775f;
            D_004C5800[0] = 1.0f;
            D_004C5800[1] = 0.7843739986419678f;
            D_004C5800[2] = 0.06132800132036209f;
            D_004C5800[3] = 0.03945299983024597f;
            D_004C5810[0] = 1.0f;
            D_004C5810[1] = 0.0f;
            D_004C5810[2] = 1.0f;
            D_004C5810[3] = 0.0f;
            D_004C5820[0] = 1.0f;
            D_004C5820[1] = 1.0f;
            D_004C5820[2] = 0.0f;
            D_004C5820[3] = 0.0f;
            *(char*)&D_004A4DF0 = -1;
            D_004A4DF0 = (D_004A4DF0 & 0xFF) | 0xFFFFFF00u;
            cCameraTriggerMan_cCameraTriggerMan(((char*)&gCamTriggerMan));
        } else {
            cCameraTriggerMan__cCameraTriggerMan(((char*)&gCamTriggerMan), 2);
        }
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176270);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];

/**
 * Destroys a camera algorithm base object: an unreferenced copy of cCameraAlgorithm__cCameraAlgorithm.
 *
 * Restores cCameraAlgorithm's vtable D_0045BDE0 at +0x10 and frees the object when bit 0 of @p flags is set. Nothing
 * calls it or stores its address; g++ emitted this unit's copy of the inline destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void func_00176270(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762A0);
#ifdef SKIP_ASM
/**
 * Does nothing: an unreferenced copy of the base camera algorithm's empty snap (slot 3; cCameraAlgorithm_snap in the vtable).
 */
extern "C" void func_001762A0(void) {

}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762A8);
#ifdef SKIP_ASM
/**
 * Does nothing: an unreferenced copy of the base camera algorithm's empty snapCurrent (slot 4; cCameraAlgorithm_snapCurrent in the
 * vtable).
 */
extern "C" void func_001762A8(void) {

}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762B0);
#ifdef SKIP_ASM
/**
 * Does nothing: an unreferenced copy of the base camera algorithm's empty update (slot 5; cCameraAlgorithm_update in the vtable).
 */
extern "C" void func_001762B0(void) {

}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762B8);
#ifdef SKIP_ASM
/**
 * Returns pi/4, the default field of view: an unreferenced copy of cCameraAlgorithm_getDefaultView.
 *
 * @return pi/4 (45 degrees in radians)
 */
extern "C" float func_001762B8(void) {
    return 0.7853982f;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762C8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long t4vect_Zero;

/**
 * Returns the zero vector as the target position: an unreferenced copy of the base camera algorithm's getTargetPos
 * (cCameraAlgorithm_getTargetPos).
 *
 * Copies t4vect_Zero into @p result.
 *
 * @param result  receives the vector (hidden return slot)
 * @return @p result
 */
extern "C" long long *func_001762C8(long long *result) {
    *result = t4vect_Zero;
    return result;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762E0);
#ifdef SKIP_ASM
/**
 * Does nothing: an unreferenced copy of the base camera algorithm's empty getRaceCamFinalPhaseData (cCameraAlgorithm_getRaceCamFinalPhaseData).
 */
extern "C" void func_001762E0(void) {

}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762E8);
#ifdef SKIP_ASM
/**
 * Does nothing: an unreferenced copy of the base camera algorithm's empty doRaceCamFinalPhase (cCameraAlgorithm_doRaceCamFinalPhase).
 */
extern "C" void func_001762E8(void) {

}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001762F0);
#ifdef SKIP_ASM
class cTriggerVirt5 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xC;
    // vptr at 0x10; slot N lives at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void update();
};

/**
 * Calls the algorithm's own update (virtual slot 5): an unreferenced copy of cCameraAlgorithm_updateAlgorithm.
 */
extern "C" void func_001762F0(cTriggerVirt5* self)
{
    self->update();
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176318);
#ifdef SKIP_ASM
/**
 * Does nothing: an unreferenced copy of cCameraAlgorithm_notifyMoveZ.
 */
extern "C" void func_00176318(void) {

}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176320);
#ifdef SKIP_ASM
/**
 * Returns 0: an unreferenced copy of the base camera algorithm's shouldSnapOnUpdate (cCameraAlgorithm_shouldSnapOnUpdate).
 *
 * @return 0 (false)
 */
extern "C" int func_00176320(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cScriptAlgorithm__cScriptAlgorithm);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

/**
 * Destroys a script camera algorithm base: restores cCameraAlgorithm's vtable and frees the object when asked.
 *
 * Stores D_0045BDE0 at +0x10 and calls operator_delete when bit 0 of @p flags is set. Slot 1 of the cScriptAlgorithm
 * vtable cScriptAlgorithm_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cScriptAlgorithm__cScriptAlgorithm(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cScriptAlgorithm_getTargetPos);
#ifdef SKIP_ASM
/**
 * Returns a script camera algorithm's target position.
 *
 * Copies the 16-byte vector at +0x20 of @p scriptAlgo (On Tour cScriptAlgorithm::Target) into @p self, the hidden
 * result slot. Slot 6 (getTargetPos) of cScriptAlgorithm's vtable cScriptAlgorithm_vtbl and of the four script algorithm vtables
 * (cScriptRelativeAlgorithm_vtbl, cScriptTargetAlgorithm_vtbl, cScriptPointAlgorithm_vtbl,
 * cScriptManualAlgorithm_vtbl).
 *
 * @param scriptAlgo  the script camera algorithm (this)
 * @return @p self, holding the target position
 */
extern "C" void* cScriptAlgorithm_getTargetPos(void* self, void* scriptAlgo)
{
    *(cQuad128*)self = *(cQuad128*)((char*)scriptAlgo + 0x20);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cScriptManualAlgorithm__cScriptManualAlgorithm);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

/**
 * Destroys a manual script camera algorithm, freeing it when asked to.
 *
 * Stores the cCameraAlgorithm vtable D_0045BDE0 at +0x10 (the last vtable store of the inlined destructor chain; no
 * level tears anything else down), then calls operator_delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cScriptManualAlgorithm__cScriptManualAlgorithm(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cScriptPointAlgorithm__cScriptPointAlgorithm);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

/**
 * Destroys a point script camera algorithm, freeing it when asked to.
 *
 * Stores the cCameraAlgorithm vtable D_0045BDE0 at +0x10 (the last vtable store of the inlined destructor chain; no
 * level tears anything else down), then calls operator_delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cScriptPointAlgorithm__cScriptPointAlgorithm(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cScriptTargetAlgorithm__cScriptTargetAlgorithm);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

/**
 * Destroys a target script camera algorithm, freeing it when asked to.
 *
 * Stores the cCameraAlgorithm vtable D_0045BDE0 at +0x10 (the last vtable store of the inlined destructor chain; no
 * level tears anything else down), then calls operator_delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cScriptTargetAlgorithm__cScriptTargetAlgorithm(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cScriptRelativeAlgorithm__cScriptRelativeAlgorithm);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

/**
 * Destroys a relative script camera algorithm, freeing it when asked to.
 *
 * Stores the cCameraAlgorithm vtable D_0045BDE0 at +0x10 (the last vtable store of the inlined destructor chain; no
 * level tears anything else down), then calls operator_delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cScriptRelativeAlgorithm__cScriptRelativeAlgorithm(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBoundedAlgorithm_setBoundAction);
#ifdef SKIP_ASM
/**
 * Sets a bounded camera algorithm's bound action.
 *
 * Stores @p boundAction at +0x80 (On Tour cBoundedAlgorithm::BoundAction). Nothing calls it;
 * cChaseCameraController_createChaseAlgorithmBlend writes the field directly.
 *
 * @param boundedAlgo  the bounded camera algorithm (this)
 * @param boundAction  the cCTActionBoundedCam to follow
 */
extern "C" void cBoundedAlgorithm_setBoundAction(void *boundedAlgo, int boundAction) {
    (*(int *)((char*)(boundedAlgo) + (0x80))) = boundAction;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBoundedAlgorithm_getBoundAction);
#ifdef SKIP_ASM
/**
 * Returns a bounded camera algorithm's bound action.
 *
 * Loads +0x80 (On Tour cBoundedAlgorithm::BoundAction). Nothing calls it.
 *
 * @param boundedAlgo  the bounded camera algorithm (this)
 * @return the cCTActionBoundedCam pointer at +0x80
 */
extern "C" int cBoundedAlgorithm_getBoundAction(cBoundedAlgorithm* boundedAlgo) {
    return ((int)boundedAlgo->BoundAction);
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176438);
#ifdef SKIP_ASM
/**
 * Returns the word at +0x18 of @p object.
 *
 * An inline accessor emitted with this unit's other copies; nothing calls it or stores its address.
 *
 * @param object  the object to read
 * @return the 32-bit word at +0x18
 */
extern "C" int func_00176438(void *object) {
    return (*(int *)((char*)(object) + (0x18)));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTObject_pos__FPv);
#ifdef SKIP_ASM
/**
 * Returns the camera-trigger object's position, which sits at the start of the object.
 *
 * @return self
 */
void* cCTObject_pos(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTObject_heading__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of a camera trigger object's heading.
 *
 * @return a pointer to the heading, the float at +0x0C
 */
void* cCTObject_heading(void* self)
{
    return (char*)self + 0xC;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTObject_pitch__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of a camera trigger object's pitch.
 *
 * @return a pointer to the pitch, the float at +0x10
 */
void* cCTObject_pitch(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTObject_roll__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of a camera trigger object's roll.
 *
 * @return a pointer to the roll, the float at +0x14
 */
void* cCTObject_roll(void* self)
{
    return (char*)self + 0x14;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTObject_scale__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of a camera trigger object's scale vector.
 *
 * @return a pointer to the scale, the vector at +0x18 (the factory constructors set it to 1, 1, 1)
 */
void* cCTObject_scale(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolume_cCTVolume);
#ifdef SKIP_ASM
extern void* D_0045C3A0[];
inline void* operator new(unsigned int, void* p) { return p; }

struct sCTBoundK_6468 : sCTBoundObjG {
    sCTBoundK_6468() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), D_0045C3A0) {}
};

/**
 * Constructs a trigger volume base object: the cCTObject defaults plus the abstract volume vtable.
 *
 * Stores the cCTObject vtable cCTObject_vtbl at +0x24, zeroes the position (+0x0, t3vect_Zero) and heading, pitch and roll
 * (+0xC..+0x14), sets the scale (+0x18) to (1, 1, 1) and then installs D_0045C3A0, whose slots 6 and 7 (boundBox,
 * containsPoint) are empty and slot 8 is cCTVolume_getBoundaryPointClosestTo. Type (+0x28) is not set. Nothing calls it.
 *
 * @return @p self
 */
extern "C" void* cCTVolume_cCTVolume(void* self)
{
    return new (self) sCTBoundK_6468;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolumeEllipse_cCTVolumeEllipse);
#ifdef SKIP_ASM
/**
 * Constructs an ellipse trigger volume with default values.
 *
 * The cCTObject defaults (position and angles 0, scale 1), the ellipse volume vtable cCTVolumeEllipse_vtbl at +0x24 and Type
 * (+0x28) 0 (On Tour eCTVolumeEllipse). get_camvolume builds the same object inline for "Camera Sphere Trigger".
 * Nothing calls this copy.
 *
 * @return @p self
 */
extern "C" void* cCTVolumeEllipse_cCTVolumeEllipse(void* self)
{
    return new (self) sCTVolEllG;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTVolumeBox_cCTVolumeBox);
#ifdef SKIP_ASM
/**
 * Constructs a box trigger volume with default values.
 *
 * The cCTObject defaults (position and angles 0, scale 1), the box volume vtable cCTVolumeBox_vtbl at +0x24 and Type (+0x28) 1
 * (On Tour eCTVolumeBox). get_camvolume builds the same object inline for "Camera Box Trigger". Nothing calls this
 * copy.
 *
 * @return @p self
 */
extern "C" void* cCTVolumeBox_cCTVolumeBox(void* self)
{
    return new (self) sCTVolBoxG;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTAction_purge__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: a camera trigger action's default purge.
 *
 * Slot 1 of the cCTAction vtable D_0045C2E8 (the plain switch and none actions). The bounded and spline actions
 * override it with cCTActionBoundedCam_purge and cCTActionSpline_purge, which free their bound object or spline.
 */
void cCTAction_purge(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTBoundObjEllipse_cCTBoundObjEllipse);
#ifdef SKIP_ASM
/**
 * Constructs an ellipse camera bound object with default values.
 *
 * The cCTObject defaults (position and angles 0, scale 1), the ellipse bound-object vtable cCTBoundObjEllipse_vtbl at +0x24 and Type
 * (+0x28) 0 (On Tour eCTBoundObjEllipse). get_camboundobj builds the same object inline for "Camera Ellipse BoundObj";
 * nothing calls this copy.
 *
 * @return @p self
 */
extern "C" void* cCTBoundObjEllipse_cCTBoundObjEllipse(void* self)
{
    return new (self) sCTBoundEllG;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTBoundObjBox_cCTBoundObjBox);
#ifdef SKIP_ASM
/**
 * Constructs a box camera bound object with default values.
 *
 * The cCTObject defaults (position and angles 0, scale 1), the box bound-object vtable cCTBoundObjBox_vtbl at +0x24 and Type
 * (+0x28) 1 (On Tour eCTBoundObjBox). get_camboundobj builds the same object inline for "Camera Box BoundObj"; nothing
 * calls this copy.
 *
 * @return @p self
 */
extern "C" void* cCTBoundObjBox_cCTBoundObjBox(void* self)
{
    return new (self) sCTBoundBoxG;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTBoundObjLine_cCTBoundObjLine);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sCTBoundLineK : sCTBoundObjG {
    int type;
    sCTVec3G pts[2];
    sCTBoundLineK() __asm__("cCTBoundObjLine_cCTBoundObjLine");
};

/**
 * Constructs a line camera bound object with default values.
 *
 * The cCTObject defaults (position and angles 0, scale 1), the line bound-object vtable cCTBoundObjLine_vtbl at +0x24, Type
 * (+0x28) 2 (On Tour eCTBoundObjLine) and both end points (+0x2C, +0x38) set to t3vect_Zero. get_camboundobj builds the
 * same object inline for "Camera Line BoundObj"; nothing calls this copy.
 */
sCTBoundLineK::sCTBoundLineK() : sCTBoundObjG(sCTVec3G(1.0f, 1.0f, 1.0f), cCTBoundObjLine_vtbl)
{
    type = 2;
    pts[0] = t3vect_Zero;
    pts[1] = t3vect_Zero;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTBoundObjLine_point1);
#ifdef SKIP_ASM
/**
 * Returns the address of a line bound object's first end point.
 *
 * That is +0x2C (On Tour cCTBoundObjLine::Point[0], in object space). Nothing calls it.
 *
 * @param boundLine  the line bound object (this)
 * @return @p boundLine + 0x2C
 */
extern "C" int cCTBoundObjLine_point1(int boundLine) {
    return boundLine + 44;
}
#endif
//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTBoundObjLine_point2);
#ifdef SKIP_ASM
/**
 * Returns the address of a line bound object's second end point.
 *
 * That is +0x38 (On Tour cCTBoundObjLine::Point[1], in object space). Nothing calls it.
 *
 * @param boundLine  the line bound object (this)
 * @return @p boundLine + 0x38
 */
extern "C" int cCTBoundObjLine_point2(int boundLine) {
    return boundLine + 0x38;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTBoundObjPoint_limitPoint);
#ifdef SKIP_ASM
struct sVec4A16 {
    float x, y, z, w;
} __attribute__((aligned(16)));

/**
 * Limits a world point to a point shape, which means replacing it with the shape's position.
 *
 * Writes the shape's x, y and z (+0x0, +0x4, +0x8) with w = 1 into @p out, whatever it held before.
 *
 * @param out  the world point to limit (On Tour t4Vector& worldP); overwritten
 */
extern "C" void cCTBoundObjPoint_limitPoint(float* self, sVec4A16* out)
{
    sVec4A16 worldPoint;
    worldPoint.x = self[0];
    worldPoint.y = self[1];
    worldPoint.z = self[2];
    worldPoint.w = 1.0f;
    *out = worldPoint;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTActionBoundedCam_purge);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees a bounded-camera trigger action's bound object.
 *
 * Calls operator_delete on the pointer at +0x8 (On Tour cCTActionBoundedCam::BoundObj) when it is set; the pointer
 * itself is not cleared. Slot 1 of the bounded action vtable D_0045C1D0.
 */
extern "C" void cCTActionBoundedCam_purge(void* self)
{
    int* buffer = *(int**)((char*)self + 0x8);
    if (buffer != 0) {
        operator_delete(buffer);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cCTActionSpline_purge);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees a spline trigger action's spline.
 *
 * Calls operator_delete on the pointer at +0x8 (On Tour cCTActionSpline::Spline) when it is set; the pointer itself is
 * not cleared. Slot 1 of the spline action vtable D_0045C1B8.
 */
extern "C" void cCTActionSpline_purge(void* self)
{
    int* buffer = *(int**)((char*)self + 0x8);
    if (buffer != 0) {
        operator_delete(buffer);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", descend);
#ifdef SKIP_ASM
struct sVEnt176890 {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

extern "C" void* cActiveTriggerList_find(void*, void*);
extern "C" void cActiveTriggerList_add(void*, void*);

/**
 * Runs the camera-trigger collision callback on an octree cell and, recursively, on all its children.
 *
 * For each entry of type 4 (a camera trigger) in the cell's node list (+0x28) whose trigger Volume (+0x8) contains the
 * rider's position (volume slot 7 containsPoint; rider +0x6C0 slot 5 getPosition), flags the trigger's node in the
 * active list (+0xC = 1, found with cActiveTriggerList_find) or adds it with cActiveTriggerList_add. Then recurses into each
 * non-null child (+0x0..+0x1C). Used for cells that lie wholly inside the query box (nash_privy_QueryHelper_helpX, cCameraTriggerMan_checkCollisions).
 *
 * @param node  the octree cell (On Tour Cell)
 * @param ctx  the callback: active trigger list at +0x0, rider at +0x4 (On Tour CameraCollisionCallback)
 */
extern "C" void descend(void* node, void* ctx)
{
    char* entry;
    for (entry = *(char**)((char*)node + 0x28); entry != 0; entry = *(char**)entry) {
        if (*(int*)(entry + 8) == 4) {
            char* trig = *(char**)(entry + 0xC);
            char* sub = *(char**)((char*)ctx + 4) + 0x6C0;
            char* bound = *(char**)(trig + 8);
            sVEnt176890* cvt = *(sVEnt176890**)sub;
            sVEnt176890* bvt = *(sVEnt176890**)(bound + 0x24);
            char* bthis = bound + bvt[7].delta;
            int riderPos = cvt[5].fn(sub + cvt[5].delta);
            if (bvt[7].fn(bthis, riderPos)) {
                void* existing = cActiveTriggerList_find(*(void**)ctx, trig);
                if (existing) {
                    *(int*)((char*)existing + 0xC) = 1;
                } else {
                    cActiveTriggerList_add(*(void**)ctx, trig);
                }
            }
        }
    }
    if (*(void**)((char*)node + 0x0)) descend(*(void**)((char*)node + 0x0), ctx);
    if (*(void**)((char*)node + 0x4)) descend(*(void**)((char*)node + 0x4), ctx);
    if (*(void**)((char*)node + 0x8)) descend(*(void**)((char*)node + 0x8), ctx);
    if (*(void**)((char*)node + 0xC)) descend(*(void**)((char*)node + 0xC), ctx);
    if (*(void**)((char*)node + 0x10)) descend(*(void**)((char*)node + 0x10), ctx);
    if (*(void**)((char*)node + 0x14)) descend(*(void**)((char*)node + 0x14), ctx);
    if (*(void**)((char*)node + 0x18)) descend(*(void**)((char*)node + 0x18), ctx);
    if (*(void**)((char*)node + 0x1C)) descend(*(void**)((char*)node + 0x1C), ctx);
}
#endif

extern "C" void* func_00175A20(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176A28__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static constructors: calls func_00175A20 with (1, 0xFFFF).
 *
 * Listed in the global constructor table (word at 0x0043CE48).
 *
 * @return func_00175A20's result
 */
void* func_00176A28(void* self)
{
    return func_00175A20(1, 0xffff);
}
#endif

extern "C" void* func_00175A20(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176A48__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static destructors: calls func_00175A20 with (0, 0xFFFF), which destroys gCamTriggerMan.
 *
 * Listed in the global destructor table (word at 0x0043CEE8).
 *
 * @return func_00175A20's result
 */
void* func_00176A48(void* self)
{
    return func_00175A20(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(void* self);
extern void* cDEFAULT_2ChaseAlgorithm_vtbl[];

/**
 * Constructs the DEFAULT_2 chase camera algorithm (the near chase camera).
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x3C (On Tour CAMERA_ALGORITHM_CHASE_DEFAULT_2 = 60)
 * and cDEFAULT_2ChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm(cDEFAULT_2ChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(self);
    self->AlgorithmType = 0x3C;
    self->mVtbl = cDEFAULT_2ChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cDEFAULT_2ChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_2ChaseAlgorithm__cDEFAULT_2ChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a DEFAULT_2 chase camera algorithm: stores cDEFAULT_2ChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cDEFAULT_2ChaseAlgorithm__cDEFAULT_2ChaseAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cDEFAULT_2ChaseAlgorithm_vtbl;
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_2ChaseAlgorithm_getRestDrop);
#ifdef SKIP_ASM
/**
 * Returns the DEFAULT_2 chase camera's rest drop (vtable slot 13, On Tour getRestDrop): 61.688.
 *
 * @return 61.688
 */
extern "C" float cDEFAULT_2ChaseAlgorithm_getRestDrop(cDEFAULT_2ChaseAlgorithm* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_2ChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the DEFAULT_2 chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cDEFAULT_2ChaseAlgorithm_getRaceCamFinalPhaseData(cDEFAULT_2ChaseAlgorithm* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_2ChaseAlgorithm_update);
#ifdef SKIP_ASM
struct sCamInfo_00178BB0;
extern "C" void cLookAtChaseAlgorithm_setupChaseData(void* self, sCamInfo_00178BB0* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);
extern "C" void cLookAtChaseAlgorithm_setupHandplantBlend(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupHalfPipeCam(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateCamTarget(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupCamDirection(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateFOVWithSpeed(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateAirCam(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateHalfPipeCam(void* self, void* info, float a);
extern "C" void cLookAtChaseAlgorithm_updateRailCam(void* self, void* info, float a, float b);
// PORT: the camera.cpp definition takes 7 floats; this caller passes an 8th in $f19.
extern "C" void cLookAtChaseAlgorithm_updateHandplant(void* self, void* info, float a, float b, float c, float d, float e, float f,
                              float g, float h);
extern "C" void cLookAtChaseAlgorithm_updateViewCam(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateSlowSpeedCam(void* self, void* info, float a, float b, float c);

struct sCamInfo_176B10
{
    char data[0x68];
} __attribute__((aligned(16)));

/**
 * Runs one frame of the DEFAULT_2 (near) chase camera through the full look-at chase pipeline.
 *
 * Runs setupChaseData (cLookAtChaseAlgorithm_setupChaseData), setupHandplantBlend, setupHalfPipeCam, updateCamTarget, setupCamDirection,
 * updateFollowDistance (222.8 stopped, 5.92 per speed, boost 5.25), updateDropWithSpeed, updateFOVWithSpeed (0.965 *
 * pi/4), updateHandplant, updateCamTargetLate (base height 57.0), updateAirCam, updateHalfPipeCam (1.53 s),
 * updateRailCam (rise 120.9), updateViewCam (600, 300, 100) and updateSlowSpeedCam (15 km/h, 1.5 s each way).
 */
extern "C" void cDEFAULT_2ChaseAlgorithm_update(cDEFAULT_2ChaseAlgorithm* self)
{
    sCamInfo_176B10 info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), (sCamInfo_00178BB0*)&info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    cLookAtChaseAlgorithm_setupHandplantBlend(((char*)self), &info);
    cLookAtChaseAlgorithm_setupHalfPipeCam(((char*)self), &info);
    cLookAtChaseAlgorithm_updateCamTarget(((char*)self), &info);
    cLookAtChaseAlgorithm_setupCamDirection(((char*)self), &info);
    cLookAtChaseAlgorithm_updateFollowDistance(((char*)self), &info, 222.8457794189453f, 5.91639518737793f, 76.68663024902344f, 5.25f,
                  1.7668397426605225f, 0.9239780306816101f, 0.907414972782135f);
    cLookAtChaseAlgorithm_updateDropWithSpeed(((char*)self), &info, 5.74874210357666f, 27.03497314453125f, 8.82034969329834f, 7.998477935791016f,
                  0.9783917665481567f);
    cLookAtChaseAlgorithm_updateFOVWithSpeed(((char*)self), &info, 0.0f, 0.9651793837547302f, 1.0f, 0.949999988079071f);
    cLookAtChaseAlgorithm_updateHandplant(((char*)self), &info, 348.64111328125f, 66.33821868896484f, 162.2997283935547f, 2.00368070602417f, 0.0f,
                  0.9800000190734863f, 0.9599999785423279f, 0.4552607834339142f);
    cLookAtChaseAlgorithm_updateCamTargetLate(((char*)self), &info, 57.0410041809082f, 1.552131175994873f, 10.378137588500977f, 10.066482543945312f,
                  0.9241908192634583f);
    cLookAtChaseAlgorithm_updateAirCam(((char*)self), &info, 1.840967059135437f, 2.494720935821533f, 0.0949358195066452f, 0.8546590209007263f);
    cLookAtChaseAlgorithm_updateHalfPipeCam(((char*)self), &info, 1.5269116163253784f);
    cLookAtChaseAlgorithm_updateRailCam(((char*)self), &info, 120.85977172851562f, 0.9706981778144836f);
    cLookAtChaseAlgorithm_updateViewCam(((char*)self), &info, 600.0f, 300.0f, 100.0f, 0.9800000190734863f);
    cLookAtChaseAlgorithm_updateSlowSpeedCam(((char*)self), &info, 15.0f, 1.5f, 1.5f);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_2ChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(void* self, float a, float b);
class cCamTargetK8940 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int getAIIndex();
};
class cCamCtrlK8940 {
public:
    char pad_0x00[0x10];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void update();
};

/**
 * Snaps the DEFAULT_2 chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cDEFAULT_2ChaseAlgorithm_snap(cDEFAULT_2ChaseAlgorithm* self, int id)
{
    char* rider = *(char**)((char*)self->Target + 4);
    if (id == ((cCamTargetK8940*)(rider + 0x6C0))->getAIIndex()) {
        cLookAtChaseAlgorithm_snap(((cCamCtrlK8940*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cCamCtrlK8940*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrlK8940*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(void* self);
extern void* cDEFAULT_3ChaseAlgorithm_vtbl[];

/**
 * Constructs the DEFAULT_3 chase camera algorithm (the default race camera; cCamera embeds one at +0xC0).
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x3D (61) and cDEFAULT_3ChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(cDEFAULT_3ChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(self);
    self->AlgorithmType = 61;
    self->mVtbl = cDEFAULT_3ChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cDEFAULT_3ChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a DEFAULT_3 chase camera algorithm: stores cDEFAULT_3ChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cDEFAULT_3ChaseAlgorithm_vtbl;
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_3ChaseAlgorithm_getRestDrop);
#ifdef SKIP_ASM
/**
 * Returns the DEFAULT_3 chase camera's rest drop (vtable slot 13, On Tour getRestDrop): 61.688.
 *
 * @return 61.688
 */
extern "C" float cDEFAULT_3ChaseAlgorithm_getRestDrop(cDEFAULT_3ChaseAlgorithm* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_3ChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the DEFAULT_3 chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cDEFAULT_3ChaseAlgorithm_getRaceCamFinalPhaseData(cDEFAULT_3ChaseAlgorithm* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_3ChaseAlgorithm_update);
#ifdef SKIP_ASM
struct sCamInfo_00178BB0;
extern "C" void cLookAtChaseAlgorithm_setupChaseData(void* self, sCamInfo_00178BB0* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);
extern "C" void cLookAtChaseAlgorithm_setupHandplantBlend(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupHalfPipeCam(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateCamTarget(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupCamDirection(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateFOVWithSpeed(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateAirCam(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateHalfPipeCam(void* self, void* info, float a);
extern "C" void cLookAtChaseAlgorithm_updateRailCam(void* self, void* info, float a, float b);
// PORT: the camera.cpp definition takes 7 floats; this caller passes an 8th in $f19.
extern "C" void cLookAtChaseAlgorithm_updateHandplant(void* self, void* info, float a, float b, float c, float d, float e, float f,
                              float g, float h);
extern "C" void cLookAtChaseAlgorithm_updateViewCam(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateSlowSpeedCam(void* self, void* info, float a, float b, float c);

struct sCamInfo_176E10
{
    char data[0x68];
} __attribute__((aligned(16)));

/**
 * Runs one frame of the DEFAULT_3 (default) chase camera through the full look-at chase pipeline.
 *
 * The same steps as cDEFAULT_2ChaseAlgorithm_update with its own tuning: updateFollowDistance (300.9 stopped, 11.55
 * per speed, boost 10.76), updateFOVWithSpeed (0.960 * pi/4), updateCamTargetLate (base height 31.1), updateAirCam
 * (amplitude 0.136) and updateRailCam (rise 146.4).
 */
extern "C" void cDEFAULT_3ChaseAlgorithm_update(cDEFAULT_3ChaseAlgorithm* self)
{
    sCamInfo_176E10 info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), (sCamInfo_00178BB0*)&info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    cLookAtChaseAlgorithm_setupHandplantBlend(((char*)self), &info);
    cLookAtChaseAlgorithm_setupHalfPipeCam(((char*)self), &info);
    cLookAtChaseAlgorithm_updateCamTarget(((char*)self), &info);
    cLookAtChaseAlgorithm_setupCamDirection(((char*)self), &info);
    cLookAtChaseAlgorithm_updateFollowDistance(((char*)self), &info, 300.9249267578125f, 11.549837112426758f, 76.68663024902344f, 10.755208015441895f,
                  1.7668397426605225f, 0.9239780306816101f, 0.907414972782135f);
    cLookAtChaseAlgorithm_updateDropWithSpeed(((char*)self), &info, 11.678784370422363f, 31.225603103637695f, 11.392387390136719f, 9.33469009399414f,
                  0.9783917665481567f);
    cLookAtChaseAlgorithm_updateFOVWithSpeed(((char*)self), &info, 0.0f, 0.9599078297615051f, 1.0f, 0.949999988079071f);
    cLookAtChaseAlgorithm_updateHandplant(((char*)self), &info, 348.64111328125f, 66.33821868896484f, 162.2997283935547f, 2.00368070602417f, 0.0f,
                  0.9800000190734863f, 0.9599999785423279f, 0.4552607834339142f);
    cLookAtChaseAlgorithm_updateCamTargetLate(((char*)self), &info, 31.10257339477539f, 0.37469127774238586f, 12.332656860351562f, 10.589057922363281f,
                  0.9241908192634583f);
    cLookAtChaseAlgorithm_updateAirCam(((char*)self), &info, 1.8110172748565674f, 2.494720935821533f, 0.13641449809074402f, 0.8536682724952698f);
    cLookAtChaseAlgorithm_updateHalfPipeCam(((char*)self), &info, 1.5269116163253784f);
    cLookAtChaseAlgorithm_updateRailCam(((char*)self), &info, 146.38900756835938f, 0.9706981778144836f);
    cLookAtChaseAlgorithm_updateViewCam(((char*)self), &info, 600.0f, 300.0f, 100.0f, 0.9800000190734863f);
    cLookAtChaseAlgorithm_updateSlowSpeedCam(((char*)self), &info, 15.0f, 1.5f, 1.5f);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_3ChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(void* self, float a, float b);
/**
 * Snaps the DEFAULT_3 chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cDEFAULT_3ChaseAlgorithm_snap(cDEFAULT_3ChaseAlgorithm* self, int id)
{
    char* rider = *(char**)((char*)self->Target + 4);
    if (id == ((cCamTargetK8940*)(rider + 0x6C0))->getAIIndex()) {
        cLookAtChaseAlgorithm_snap(((cCamCtrlK8940*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cCamCtrlK8940*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrlK8940*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(void* self);
extern void* cDEFAULT_4ChaseAlgorithm_vtbl[];

/**
 * Constructs the DEFAULT_4 chase camera algorithm (the far chase camera).
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x3E (62) and cDEFAULT_4ChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm(cDEFAULT_4ChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(self);
    self->AlgorithmType = 0x3E;
    self->mVtbl = cDEFAULT_4ChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cDEFAULT_4ChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_4ChaseAlgorithm__cDEFAULT_4ChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a DEFAULT_4 chase camera algorithm: stores cDEFAULT_4ChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cDEFAULT_4ChaseAlgorithm__cDEFAULT_4ChaseAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cDEFAULT_4ChaseAlgorithm_vtbl;
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_4ChaseAlgorithm_getRestDrop);
#ifdef SKIP_ASM
/**
 * Returns the DEFAULT_4 chase camera's rest drop (vtable slot 13, On Tour getRestDrop): 61.688.
 *
 * @return 61.688
 */
extern "C" float cDEFAULT_4ChaseAlgorithm_getRestDrop(cDEFAULT_4ChaseAlgorithm* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_4ChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the DEFAULT_4 chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cDEFAULT_4ChaseAlgorithm_getRaceCamFinalPhaseData(cDEFAULT_4ChaseAlgorithm* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_4ChaseAlgorithm_update);
#ifdef SKIP_ASM
struct sCamInfo_00178BB0;
extern "C" void cLookAtChaseAlgorithm_setupChaseData(void* self, sCamInfo_00178BB0* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);
extern "C" void cLookAtChaseAlgorithm_setupHandplantBlend(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupHalfPipeCam(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateCamTarget(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupCamDirection(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateFOVWithSpeed(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateAirCam(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateHalfPipeCam(void* self, void* info, float a);
extern "C" void cLookAtChaseAlgorithm_updateRailCam(void* self, void* info, float a, float b);
// PORT: the camera.cpp definition takes 7 floats; this caller passes an 8th in $f19.
extern "C" void cLookAtChaseAlgorithm_updateHandplant(void* self, void* info, float a, float b, float c, float d, float e, float f,
                              float g, float h);
extern "C" void cLookAtChaseAlgorithm_updateViewCam(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateSlowSpeedCam(void* self, void* info, float a, float b, float c);

struct sCamInfo_177110
{
    char data[0x68];
} __attribute__((aligned(16)));

/**
 * Runs one frame of the DEFAULT_4 (far) chase camera through the full look-at chase pipeline.
 *
 * The same steps as cDEFAULT_2ChaseAlgorithm_update with its own tuning: updateFollowDistance (505.8 stopped, 11.55
 * per speed, boost 11.38), updateDropWithSpeed (rise 98.8), updateFOVWithSpeed (0.965 * pi/4), updateCamTargetLate
 * (base height 50.0), updateAirCam (amplitude 0.102) and updateRailCam (rise 167.9).
 */
extern "C" void cDEFAULT_4ChaseAlgorithm_update(cDEFAULT_4ChaseAlgorithm* self)
{
    sCamInfo_177110 info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), (sCamInfo_00178BB0*)&info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    cLookAtChaseAlgorithm_setupHandplantBlend(((char*)self), &info);
    cLookAtChaseAlgorithm_setupHalfPipeCam(((char*)self), &info);
    cLookAtChaseAlgorithm_updateCamTarget(((char*)self), &info);
    cLookAtChaseAlgorithm_setupCamDirection(((char*)self), &info);
    cLookAtChaseAlgorithm_updateFollowDistance(((char*)self), &info, 505.79815673828125f, 11.549837112426758f, 76.68663024902344f, 11.38194465637207f,
                  1.7668397426605225f, 0.9239780306816101f, 0.907414972782135f);
    cLookAtChaseAlgorithm_updateDropWithSpeed(((char*)self), &info, 13.021197319030762f, 98.84776306152344f, 12.293619155883789f, 13.058300971984863f,
                  0.9783917665481567f);
    cLookAtChaseAlgorithm_updateFOVWithSpeed(((char*)self), &info, 0.0f, 0.9653480052947998f, 1.0f, 0.949999988079071f);
    cLookAtChaseAlgorithm_updateHandplant(((char*)self), &info, 348.64111328125f, 66.33821868896484f, 162.2997283935547f, 2.00368070602417f, 0.0f,
                  0.9800000190734863f, 0.9599999785423279f, 0.4552607834339142f);
    cLookAtChaseAlgorithm_updateCamTargetLate(((char*)self), &info, 50.02703094482422f, 2.5781052112579346f, 13.97744369506836f, 10.748767852783203f,
                  0.9241908192634583f);
    cLookAtChaseAlgorithm_updateAirCam(((char*)self), &info, 1.878627061843872f, 2.494720935821533f, 0.10213389247655869f, 0.8599911332130432f);
    cLookAtChaseAlgorithm_updateHalfPipeCam(((char*)self), &info, 1.5269116163253784f);
    cLookAtChaseAlgorithm_updateRailCam(((char*)self), &info, 167.9465789794922f, 0.9706981778144836f);
    cLookAtChaseAlgorithm_updateViewCam(((char*)self), &info, 600.0f, 300.0f, 100.0f, 0.9800000190734863f);
    cLookAtChaseAlgorithm_updateSlowSpeedCam(((char*)self), &info, 15.0f, 1.5f, 1.5f);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDEFAULT_4ChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(void* self, float a, float b);
/**
 * Snaps the DEFAULT_4 chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cDEFAULT_4ChaseAlgorithm_snap(cDEFAULT_4ChaseAlgorithm* self, int id)
{
    char* rider = *(char**)((char*)self->Target + 4);
    if (id == ((cCamTargetK8940*)(rider + 0x6C0))->getAIIndex()) {
        cLookAtChaseAlgorithm_snap(((cCamCtrlK8940*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cCamCtrlK8940*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrlK8940*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDirectionChaseAlgorithm_update);
#ifdef SKIP_ASM
struct sCamInfo_00178BB0;
extern "C" void cLookAtChaseAlgorithm_setupChaseData(void* self, sCamInfo_00178BB0* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);
extern "C" void cLookAtChaseAlgorithm_setupHandplantBlend(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupHalfPipeCam(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateCamTarget(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_setupCamDirection(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateFOVWithSpeed(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateAirCam(void* self, void* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateHalfPipeCam(void* self, void* info, float a);
extern "C" void cLookAtChaseAlgorithm_updateRailCam(void* self, void* info, float a, float b);
extern "C" void cLookAtChaseAlgorithm_updateRotation(void* self, float ang);

struct sCamInfo_177368
{
    char data[0x68];
} __attribute__((aligned(16)));

/**
 * Runs one frame of a direction chase camera: a chase pose whose eye always looks along one compass heading.
 *
 * Runs setupChaseData (cLookAtChaseAlgorithm_setupChaseData), setupHandplantBlend, setupHalfPipeCam, updateCamTarget, setupCamDirection,
 * updateFollowDistance (356.4, no speed or boost terms), updateDropWithSpeed (only a constant 168.6 drop),
 * updateFOVWithSpeed (0.971 * pi/4), updateCamTargetLate, updateAirCam, updateHalfPipeCam (1.53 s) and updateRailCam
 * (rise 0), then updateRotation with Angle to put the eye on the fixed heading. There is no handplant, view or
 * slow-speed step.
 */
extern "C" void cDirectionChaseAlgorithm_update(cDirectionChaseAlgorithm* self)
{
    sCamInfo_177368 info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), (sCamInfo_00178BB0*)&info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    cLookAtChaseAlgorithm_setupHandplantBlend(((char*)self), &info);
    cLookAtChaseAlgorithm_setupHalfPipeCam(((char*)self), &info);
    cLookAtChaseAlgorithm_updateCamTarget(((char*)self), &info);
    cLookAtChaseAlgorithm_setupCamDirection(((char*)self), &info);
    cLookAtChaseAlgorithm_updateFollowDistance(((char*)self), &info, 356.4251708984375f, 0.0f, 76.68663024902344f, 0.0f, 1.6834967136383057f,
                  0.9239780306816101f, 0.907414972782135f);
    cLookAtChaseAlgorithm_updateDropWithSpeed(((char*)self), &info, 0.0f, -168.60406494140625f, 0.0f, 0.0f, 0.9783917665481567f);
    cLookAtChaseAlgorithm_updateFOVWithSpeed(((char*)self), &info, 0.0f, 0.9709749817848206f, 1.0f, 0.949999988079071f);
    cLookAtChaseAlgorithm_updateCamTargetLate(((char*)self), &info, -0.24752351641654968f, 0.008603549562394619f, 0.12915411591529846f,
                  0.22476252913475037f, 0.9241908192634583f);
    cLookAtChaseAlgorithm_updateAirCam(((char*)self), &info, 1.878627061843872f, 2.494720935821533f, 0.19890090823173523f, 0.8672914505004883f);
    cLookAtChaseAlgorithm_updateHalfPipeCam(((char*)self), &info, 1.5269116163253784f);
    cLookAtChaseAlgorithm_updateRailCam(((char*)self), &info, 0.0f, 0.9706981778144836f);
    cLookAtChaseAlgorithm_updateRotation(((char*)self), self->Angle);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDirectionChaseAlgorithm_cDirectionChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(void* self);
extern void* cDirectionChaseAlgorithm_vtbl[];

/**
 * Constructs a direction chase camera for compass direction @p n.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 18 + n (On Tour CAMERA_ALGORITHM_CHASE_DIRECTION_N ..
 * _NE), cDirectionChaseAlgorithm_vtbl and Angle = n * pi/4.
 *
 * @param n  the direction index 0..7: N, NW, W, SW, S, SE, E, NE by On Tour's enum (On Tour eCamDirection dir)
 * @return @p self
 */
extern "C" void* cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cDirectionChaseAlgorithm* self, int n)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(self);
    self->AlgorithmType = n + 18;
    self->mVtbl = cDirectionChaseAlgorithm_vtbl;
    self->Angle = (float)n * 0.7853981852531433f;
    return ((void*)self);
}
#endif

extern void* cDirectionChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDirectionChaseAlgorithm__cDirectionChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a direction chase camera algorithm: stores cDirectionChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cDirectionChaseAlgorithm__cDirectionChaseAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cDirectionChaseAlgorithm_vtbl;
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDirectionChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the direction chase camera's rest drop (vtable slot 13, On Tour getRestDrop): always 0.
 *
 * @return 0
 */
float cDirectionChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDirectionChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the direction chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cDirectionChaseAlgorithm_getRaceCamFinalPhaseData(cDirectionChaseAlgorithm* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cDirectionChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(void* self, float a, float b);
/**
 * Snaps the direction chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cDirectionChaseAlgorithm_snap(cDirectionChaseAlgorithm* self, int id)
{
    char* rider = *(char**)((char*)self->Target + 4);
    if (id == ((cCamTargetK8940*)(rider + 0x6C0))->getAIIndex()) {
        cLookAtChaseAlgorithm_snap(((cCamCtrlK8940*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cCamCtrlK8940*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrlK8940*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177610);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest (variadic).
 *
 * The body only spills the other argument registers. Nothing in the ELF calls it or stores its address; the unit has
 * one such stub, like func_00174808 in the camera trigger unit, placed just before its static initializer
 * (func_00177650).
 *
 * @return the first argument
 */
extern "C" void* func_00177610(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177650);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColor_177650 {
    float r, g, b, a;
    sColor_177650(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

extern sColor_177650 D_004C5890;
extern sColor_177650 D_004C58A0;
extern sColor_177650 D_004C58B0;
extern sColor_177650 D_004C58C0;
extern sColor_177650 D_004C58D0;
extern sColor_177650 D_004C58E0;
extern sColor_177650 D_004C58F0;
extern sColor_177650 D_004C5900;
extern sColor_177650 D_004C5910;
extern sColor_177650 D_004C5920;
extern sColor_177650 D_004C5930;
extern sColor_177650 D_004C5940;
extern sColor_177650 D_004C5950;
extern sColor_177650 D_004C5960;
extern sColor_177650 D_004C5970;
extern sColor_177650 D_004C5980;
extern sColor_177650 D_004C5990;
extern sColor_177650 D_004C59A0;
extern sColor_177650 D_004C59B0;
extern sColor_177650 D_004C59C0;
extern sColor_177650 D_004C59D0;
extern sColor_177650 D_004C59E0;
extern sColor_177650 D_004C59F0;
extern sColor_177650 D_004C5A00;
extern sColor_177650 D_004C5A10;
extern sColor_177650 D_004C5A20;
extern sColor_177650 D_004C5A30;
extern sColor_177650 D_004C5A40;
extern sColor_177650 D_004C5A50;
extern sColor_177650 D_004C5A60;
extern sColor_177650 D_004C5A70;
extern sColor_177650 D_004C5A80;
extern sColor_177650 D_004C5A90;
extern sColor_177650 D_004C5AA0;
extern sColor_177650 D_004C5AB0;
extern sColor_177650 D_004C5AC0;
extern sColor_177650 D_004C5AD0;
extern sColor_177650 D_004C5AE0;
extern sColor_177650 D_004C5AF0;
extern sColor_177650 D_004C5B00;
extern sColor_177650 D_004C5B10;
extern sColor_177650 D_004C5B20;
extern sColor_177650 D_004C5B30;
extern sColor_177650 D_004C5B40;
extern sColor_177650 D_004C5B50;
extern sColor_177650 D_004C5B60;
extern sColor_177650 D_004C5B70;
extern sColor_177650 D_004C5B80;
extern sColor_177650 D_004C5B90;
extern sColor_177650 D_004C5BA0;
extern sColor_177650 D_004C5BB0;
extern sColor_177650 D_004C5BC0;
extern sColor_177650 D_004C5BD0;
extern sColor_177650 D_004C5BE0;
extern sColor_177650 D_004C5BF0;
extern sColor_177650 D_004C5C00;

// The unit declares func_00177650 later as returning void*; bind this body by asm label.
extern "C" void func_00177650_impl(int initialize, int priority) __asm__("func_00177650");

/**
 * Runs the static constructors of this unit's globals (g++'s static initialization; there is nothing to destroy).
 *
 * Only priority 0xFFFF with @p initialize set does anything: it constructs the 56 four-float constants
 * D_004C5890..D_004C5C00 (the colour and blend vectors every unit of this kind sets up; nothing else in the ELF reads
 * these copies). func_00177E30 calls it with (1, 0xFFFF).
 *
 * @param initialize  1 to construct; 0 does nothing
 * @param priority  the init priority; only 0xFFFF is handled
 */
extern "C" void func_00177650_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004C5890) sColor_177650(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004C58A0) sColor_177650(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004C58B0) sColor_177650(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004C58C0) sColor_177650(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004C58D0) sColor_177650(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004C58E0) sColor_177650(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004C58F0) sColor_177650(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004C5900) sColor_177650(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004C5910) sColor_177650(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004C5920) sColor_177650(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004C5930) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5940) sColor_177650(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5950) sColor_177650(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004C5960) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5970) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5980) sColor_177650(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5990) sColor_177650(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C59A0) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C59B0) sColor_177650(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C59C0) sColor_177650(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C59D0) sColor_177650(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C59E0) sColor_177650(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004C59F0) sColor_177650(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004C5A00) sColor_177650(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004C5A10) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5A20) sColor_177650(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004C5A30) sColor_177650(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004C5A40) sColor_177650(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004C5A50) sColor_177650(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004C5A60) sColor_177650(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004C5A70) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5A80) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5A90) sColor_177650(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004C5AA0) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5AB0) sColor_177650(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004C5AC0) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5AD0) sColor_177650(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5AE0) sColor_177650(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5AF0) sColor_177650(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004C5B00) sColor_177650(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004C5B10) sColor_177650(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004C5B20) sColor_177650(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004C5B30) sColor_177650(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5B40) sColor_177650(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004C5B50) sColor_177650(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5B60) sColor_177650(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5B70) sColor_177650(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5B80) sColor_177650(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004C5B90) sColor_177650(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004C5BA0) sColor_177650(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004C5BB0) sColor_177650(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004C5BC0) sColor_177650(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004C5BD0) sColor_177650(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004C5BE0) sColor_177650(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004C5BF0) sColor_177650(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004C5C00) sColor_177650(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

extern "C" void* func_00177650(int, int);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177E28);
#ifdef SKIP_ASM
/**
 * Does nothing; nothing calls it or stores its address.
 */
extern "C" void func_00177E28(void) {

}
#endif

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177E30__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static constructors: calls func_00177650 with (1, 0xFFFF).
 *
 * Listed in the global constructor table (word at 0x0043CE4C).
 *
 * @return func_00177650's result
 */
void* func_00177E30(void* self)
{
    return func_00177650(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(void* self);
extern void* cSPOKEChaseAlgorithm_vtbl[];

/**
 * Constructs the SPOKE chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x42 (On Tour CAMERA_ALGORITHM_CHASE_SPOKE = 66) and
 * cSPOKEChaseAlgorithm_vtbl. LastLookDir is left for snap to set.
 *
 * @return @p self
 */
extern "C" void* cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm(cSPOKEChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(self);
    self->AlgorithmType = 66;
    self->mVtbl = cSPOKEChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cSPOKEChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm__cSPOKEChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a SPOKE chase camera algorithm: stores cSPOKEChaseAlgorithm_vtbl and chains to the chase destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cSPOKEChaseAlgorithm__cSPOKEChaseAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cSPOKEChaseAlgorithm_vtbl;
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm_getRestDrop);
#ifdef SKIP_ASM
/**
 * Returns the SPOKE chase camera's rest drop (vtable slot 13, On Tour getRestDrop): 61.688.
 *
 * @return 61.688
 */
extern "C" float cSPOKEChaseAlgorithm_getRestDrop(cSPOKEChaseAlgorithm* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the SPOKE chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cSPOKEChaseAlgorithm_getRaceCamFinalPhaseData(cSPOKEChaseAlgorithm* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm_getRiderPosition);
#ifdef SKIP_ASM
struct sVec_00177EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEa_00177EF8 {
    short delta;
    short index;
    sVec_00177EF8* (*fn)(void*);
};

struct sVEb_00177EF8 {
    short delta;
    short index;
    sVec_00177EF8 (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_00177EF8 ctScale_00177EF8(const sVec_00177EF8& v, float s)
{
    sVec_00177EF8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_00177EF8 ctAdd_00177EF8(const sVec_00177EF8& a, const sVec_00177EF8& b)
{
    sVec_00177EF8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline sVec_00177EF8 ctGetB_00177EF8(void* self)
{
    char* o2 = *(char**)((char*)self + 0x30);
    sVEb_00177EF8* e2 = &(*(sVEb_00177EF8**)o2)[1];
    return e2->fn(o2 + e2->delta);
}

/**
 * Returns the point the SPOKE camera follows: 20 % of the rider's slot-5 vector and 80 % of the target position.
 *
 * Calls slot 5 of the rider's +0x6C0 interface (Target->Rider), which returns a vector pointer, and the target's
 * position (slot 1), and returns 0.2 * the first + 0.8 * the second.
 *
 * @return the blended rider position
 */
extern "C" sVec_00177EF8 cSPOKEChaseAlgorithm_getRiderPosition(cSPOKEChaseAlgorithm* self)
{
    char* riderIface = *(char**)((char*)self->Target + 4) + 0x6C0;
    sVEa_00177EF8* posEntry = &(*(sVEa_00177EF8**)riderIface)[5];
    sVec_00177EF8* riderPos = posEntry->fn(riderIface + posEntry->delta);
    return ctAdd_00177EF8(ctScale_00177EF8(*riderPos, 0.19999998807907104f), ctScale_00177EF8(ctGetB_00177EF8(self), 0.800000011920929f));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm_getDesiredPosition);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_7FC0 __asm__("t4vect_ZAxis");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector ctScale_00177EF8_7FC0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector ctAdd_00177EF8_7FC0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// (prototype from their definition at line 4013)
extern "C" t4Vector cSPOKEChaseAlgorithm_getRiderPosition_7FC0(void* self) __asm__("cSPOKEChaseAlgorithm_getRiderPosition");

int cRider_getMotionState_7FC0(void*) __asm__("cRider_getMotionState__FPv");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector ctSub_7FC0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float ctLength_7FC0(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector *= 1/len).
static inline void ctDivEq_7FC0(t4Vector& v, float len)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(len));
}

static inline t4Vector* ctGetA_7FC0(void* self)
{
    char* o = *(char**)((char*)self + 0x30);
    sVEntry<t4Vector*(void*)>* vEntry = &(*(sVEntry<t4Vector*(void*)>**)o)[10];
    return vEntry->fn(o + vEntry->delta);
}

static inline t4Vector ctGetC_7FC0(void* self)
{
    char* o = *(char**)((char*)self + 0x30);
    sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)o)[4];
    return vEntry->fn(o + vEntry->delta);
}

// PORT: PS2-only VU0 inline asm (vector times scalar, vector by address).
static inline t4Vector ctScaleP_7FC0(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

/**
 * Returns where the SPOKE camera wants its eye: 250 units behind the rider along a slowly turning look direction, 30
 * up.
 *
 * The direction is @p in, or the target's handplant spline tangent (slot 10) while the rider's motion state
 * (cRider_getMotionState) is 5 (MS_Handplant); it is normalised, falling back to the target's front (slot 4) when shorter
 * than 0.01. LastLookDir = normalise(0.96 * LastLookDir + 0.04 * direction), or the direction itself when that sum is
 * under 0.02. The result is getRiderPosition + Z * 30 - LastLookDir * 250.
 *
 * @param in  the rider's velocity from the chase setup (On Tour vel)
 * @return the desired eye position
 */
extern "C" t4Vector cSPOKEChaseAlgorithm_getDesiredPosition(cSPOKEChaseAlgorithm* self, const t4Vector& in)
{
    t4Vector dir;
    if (cRider_getMotionState_7FC0(*(void**)((char*)self->Target + 4)) == 5)
        dir = *ctGetA_7FC0(self);
    else
        dir = in;
    float len = ctLength_7FC0(dir);
    if (len < 0.009999999776482582f)
        dir = ctGetC_7FC0(self);
    else
        ctDivEq_7FC0(dir, len);
    t4Vector lookDir = ctAdd_00177EF8_7FC0(ctScale_00177EF8_7FC0(self->LastLookDir, 0.9599999785423279f), ctScale_00177EF8_7FC0(dir, 0.04000002145767212f));
    len = ctLength_7FC0(lookDir);
    if (len < 0.019999999552965164f)
        lookDir = dir;
    else
        ctDivEq_7FC0(lookDir, len);
    self->LastLookDir = lookDir;
    t4Vector desiredPos = ctSub_7FC0(ctAdd_00177EF8_7FC0(cSPOKEChaseAlgorithm_getRiderPosition_7FC0(self), ctScaleP_7FC0(&D_004FF160_7FC0, 30.0f)), ctScale_00177EF8_7FC0(lookDir, 250.0f));
    return desiredPos;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector D_004FF160_8208 __asm__("t4vect_ZAxis");

// The per-frame camera input record (0x70 bytes) that cLookAtChaseAlgorithm_setupChaseData_8208 (camera/camera) fills from the target.
struct sCamInfo {
    t4Vector f00;               // 0x00
    t4Vector pos;               // 0x10
    t4Vector dir;               // 0x20
    t4Vector out;               // 0x30
    int mode;                   // 0x40
    float speed;                // 0x44: length of dir
    char pad48[4];
    float f4C;                  // 0x4C
    float f50;                  // 0x50
    float f54;                  // 0x54
    int f58;                    // 0x58
    int f5C;                    // 0x5C
    int f60;                    // 0x60
    int f64;                    // 0x64
};

extern "C" void cLookAtChaseAlgorithm_setupChaseData_8208(void* self, sCamInfo* out, float a, float b, float c, float d,
                              float e, float f, float g, float h) __asm__("cLookAtChaseAlgorithm_setupChaseData");

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_8208(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (*v * s).
static inline t4Vector ScaleP_8208(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_8208(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}
// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_8208(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_8208(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (dst = a + b).
static inline void AddD_8208(t4Vector& dst, const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    dst = r;
}

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_8208(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" t4Vector f_177FC0_8208(void* self, const t4Vector& v) __asm__("cSPOKEChaseAlgorithm_getDesiredPosition");
extern "C" t4Vector f_177EF8_8208(void* self) __asm__("cSPOKEChaseAlgorithm_getRiderPosition");
int cRider_getMotionState(void* self);

extern "C" void cSPOKEChaseAlgorithm_update_v(char* self) __asm__("cSPOKEChaseAlgorithm_update");

/**
 * Runs one frame of the SPOKE camera: eases the eye toward getDesiredPosition and aims 37 units above the followed
 * point.
 *
 * After setupChaseData (cLookAtChaseAlgorithm_setupChaseData) sets View to pi/4. The weight kept on the old eye falls from 0.988 at an eye
 * distance of 150 to 0 at 1500 (its square root while handplanting, motion state 5); Position = f * Position + (1 -
 * f) * getDesiredPosition(setup velocity), and CamTargetPos = getRiderPosition + Z * 37.
 */
extern "C" void cSPOKEChaseAlgorithm_update_v(char* self)
{
    sCamInfo info;
    cLookAtChaseAlgorithm_setupChaseData_8208(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    *(float*)self = 0.7853981852531433f;
    t4Vector scratchVec = Sub_8208(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40));
    float len = Length_8208(scratchVec);
    float distFrac;
    if (len >= 150.0f) {
        if (len <= 1500.0f) {
            distFrac = (len - 150.0f) / 1350.0f;
        } else {
            distFrac = 1.0f;
        }
    } else {
        distFrac = 0.0f;
    }
    float keepWeight = (1.0f - distFrac) * 0.9879999756813049f;
    if (cRider_getMotionState(*(void**)(*(char**)(self + 0x30) + 4)) == 5) {
        keepWeight = Sqrt_8208(keepWeight);
    }
    {
        t4Vector keptEye = Scale_8208(*(t4Vector*)(self + 0x40), keepWeight);
        float newWeight = 1.0f - keepWeight;
        AddD_8208(scratchVec, keptEye, Scale_8208(f_177FC0_8208(self, info.dir), newWeight));
        *(t4Vector*)(self + 0x40) = scratchVec;
    }
    AddD_8208(scratchVec, f_177EF8_8208(self), ScaleP_8208(&D_004FF160_8208, 37.0f));
    *(t4Vector*)(self + 0x20) = scratchVec;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSPOKEChaseAlgorithm_snap);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_83E0 __asm__("t4vect_ZAxis");

// A being: the script/AI-facing interface of a rider, at rider + 0x6C0 (seg/seg_1FBE38; slot order of the
// prototype's vtable _vt$6cBeing).
struct tInstanceRecord;

class cBeing {
public:
    virtual ~cBeing();
    virtual const t4Vector* getVelocity() const;
    virtual int isIntersecting(tInstanceRecord* rec) const;
    virtual void applyBoost(const t4Vector& impulse);
    virtual const t4Vector* getPosition() const;
    virtual const t4Matrix* getTransform() const;
    virtual int getAIIndex() const;
    virtual bool isHuman() const;
    virtual bool isComputer() const;
    virtual void transport(const t4Matrix& m);
    virtual void resetToPath();
    virtual void initForMission(const t4Matrix& m);
};

// A virtual-call view of a camera algorithm (cCameraAlgorithm, camera/cameraalgorithms.h): the vptr follows the base's own data at 0x10; slots by
// position.
class cCameraAlgorithm_cVirt {
public:
    char pad_0x00[0xC];
    int type;                   // 0xC
    virtual void v01();
    virtual void v02();
    virtual void v03(int on);
    virtual void v04();
    virtual void update();
};

extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(void* self, float a, float b);

extern "C" void f_166C60_1783E0(void* self, long id) __asm__("cLookAtChaseAlgorithm_snap");
extern "C" t4Vector f_177FC0_1783E0(void* self, const t4Vector& v) __asm__("cSPOKEChaseAlgorithm_getDesiredPosition");
extern "C" t4Vector f_177EF8_1783E0(void* self) __asm__("cSPOKEChaseAlgorithm_getRiderPosition");


// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_1783E0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (*v * s).
static inline t4Vector ScaleP_1783E0(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

/**
 * Snaps the SPOKE chase camera straight to its subject when @p id is the AI index of the rider it follows.
 *
 * Takes the target's (+0x30) slot-4 vector into +0x390 and the desired position for its slot-3 vector
 * (cSPOKEChaseAlgorithm_getDesiredPosition, probably getDesiredPosition) into +0x40, and sets +0x20 to the rider position
 * (cSPOKEChaseAlgorithm_getRiderPosition) plus 37 along the Z axis (t4vect_ZAxis). Then runs the look-at chase snap (cLookAtChaseAlgorithm_snap),
 * setupInitialCamDir (cLookAtChaseAlgorithm_setupInitialCamDir, 559.744 and 300) and the algorithm's virtual slot 5.
 *
 * @param id  the subject's AI index (On Tour subjectID); any other rider is ignored
 */
extern "C" void cSPOKEChaseAlgorithm_snap(cSPOKEChaseAlgorithm* self, long id)
{
    char* rider = *(char**)((char*)self->Target + 4);
    if (id == ((cBeing*)(rider + 0x6C0))->getAIIndex()) {
        char* subject = (char*)self->Target;
        sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)subject)[4];
        self->LastLookDir = vEntry->fn(subject + vEntry->delta);
        subject = (char*)self->Target;
        vEntry = &(*(sVEntry<t4Vector(void*)>**)subject)[3];
        self->Position = f_177FC0_1783E0(((cCameraAlgorithm_cVirt*)self), vEntry->fn(subject + vEntry->delta));
        self->CamTargetPos = Add_1783E0(f_177EF8_1783E0(((cCameraAlgorithm_cVirt*)self)), ScaleP_1783E0(&D_004FF160_83E0, 37.0f));
        f_166C60_1783E0(((cCameraAlgorithm_cVirt*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cCameraAlgorithm_cVirt*)self), 559.7440185546875f, 300.0f);
        ((cCameraAlgorithm_cVirt*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm);
#ifdef SKIP_ASM
struct cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));
extern void* cSUPERPIPEChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(void* self);

/**
 * Constructs the SUPERPIPE chase camera for one superpipe, given two points on its axis.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x43 (On Tour CAMERA_ALGORITHM_CHASE_SUPERPIPE = 67),
 * cSUPERPIPEChaseAlgorithm_vtbl, Point1 and Point2.
 *
 * @param point1  the first axis point, copied into Point1 (On Tour const t4Vector&)
 * @param point2  the second axis point, copied into Point2
 * @return @p self
 */
extern "C" void* cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(cSUPERPIPEChaseAlgorithm* self, cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm_sVec4* point1, cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm_sVec4* point2)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(self);
    self->AlgorithmType = 67;
    self->mVtbl = cSUPERPIPEChaseAlgorithm_vtbl;
    *(cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm_sVec4*)&self->Point1 = *point1;
    *(cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm_sVec4*)&self->Point2 = *point2;
    return ((void*)self);
}
#endif

extern void* cSUPERPIPEChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm__cSUPERPIPEChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a SUPERPIPE chase camera algorithm: stores cSUPERPIPEChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cSUPERPIPEChaseAlgorithm__cSUPERPIPEChaseAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cSUPERPIPEChaseAlgorithm_vtbl;
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm_getRestDrop);
#ifdef SKIP_ASM
/**
 * Returns the SUPERPIPE chase camera's rest drop (vtable slot 13, On Tour getRestDrop): 61.688.
 *
 * @return 61.688
 */
extern "C" float cSUPERPIPEChaseAlgorithm_getRestDrop(cSUPERPIPEChaseAlgorithm* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the SUPERPIPE chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cSUPERPIPEChaseAlgorithm_getRaceCamFinalPhaseData(cSUPERPIPEChaseAlgorithm* self, void* out)
{
    *(float*)((char*)out + 0x4) = 6.654887676239014f;
    *(float*)((char*)out + 0x0) = 16.140756607055664f;
    *(float*)((char*)out + 0xC) = 0.8425687551498413f;
    *(float*)((char*)out + 0x8) = 0.9264262914657593f;
    *(int*)((char*)out + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm_getIdealPosition);
#ifdef SKIP_ASM
struct sVec4_1785F0
{
    float x, y, z, w;
} __attribute__((aligned(16)));


struct sVEntry_1785F0 { short delta; short index; sVec4_1785F0 (*fn)(void*); };

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4_1785F0 Sub_1785F0(const sVec4_1785F0& a, const sVec4_1785F0& b)
{
    sVec4_1785F0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec4_1785F0 Add_1785F0(const sVec4_1785F0& a, const sVec4_1785F0& b)
{
    sVec4_1785F0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec4_1785F0 Scale_1785F0(const sVec4_1785F0& v, float s)
{
    sVec4_1785F0 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4_1785F0 Normalize_1785F0(const sVec4_1785F0& v)
{
    sVec4_1785F0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_1785F0(const sVec4_1785F0& a, const sVec4_1785F0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline sVec4_1785F0 GetPos_1785F0(char* self)
{
    char* o = *(char**)(self + 0x30);
    sVEntry_1785F0* e = &(*(sVEntry_1785F0**)o)[1];
    return e->fn(o + e->delta);
}

// The unit declares cSUPERPIPEChaseAlgorithm_getIdealPosition later with its own vector type; bind this body by asm label.
extern "C" sVec4_1785F0 cSUPERPIPEChaseAlgorithm_getIdealPosition_impl(char* self) __asm__("cSUPERPIPEChaseAlgorithm_getIdealPosition");

/**
 * Returns the SUPERPIPE camera's ideal eye: on the pipe's axis level with the rider, 1500 units back toward Point2.
 *
 * With dir = normalise(Point1 - Point2), projects the target position (slot 1) onto the axis through Point1 and
 * returns that point - dir * 1500.
 *
 * @return the ideal eye position
 */
extern "C" sVec4_1785F0 cSUPERPIPEChaseAlgorithm_getIdealPosition_impl(char* self)
{
    sVec4_1785F0 dir = Normalize_1785F0(Sub_1785F0(*(sVec4_1785F0*)(self + 0x390), *(sVec4_1785F0*)(self + 0x3A0)));
    sVec4_1785F0 rel = Sub_1785F0(GetPos_1785F0(self), *(sVec4_1785F0*)(self + 0x390));
    sVec4_1785F0 off = Scale_1785F0(dir, Dot_1785F0(rel, dir) / Dot_1785F0(dir, dir));
    sVec4_1785F0 res = Sub_1785F0(Add_1785F0(*(sVec4_1785F0*)(self + 0x390), off), Scale_1785F0(dir, 1500.0f));
    return res;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm_getIdealTargetPos);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector D_004FF160_8758 __asm__("t4vect_ZAxis");


// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_178758(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (*v * s).
static inline t4Vector ScaleP_178758(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

static inline t4Vector CallA_178758(char* self)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)o)[1];
    return vEntry->fn(o + vEntry->delta);
}

extern "C" t4Vector f_178758(char* self) __asm__("cSUPERPIPEChaseAlgorithm_getIdealTargetPos");
/**
 * Returns the SUPERPIPE camera's ideal look-at point: the target position (slot 1) 100 units lower.
 *
 * @return the ideal look-at point
 */
extern "C" t4Vector f_178758(char* self)
{
    return Add_178758(CallA_178758(self), ScaleP_178758(&D_004FF160_8758, -100.0f));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector t4vect_XAxis;

// The per-frame camera input record (0x70 bytes) that cLookAtChaseAlgorithm_setupChaseData_87E0 (camera/camera) fills from the target.
extern "C" void cLookAtChaseAlgorithm_setupChaseData_87E0(void* self, sCamInfo* out, float a, float b, float c, float d,
                              float e, float f, float g, float h) __asm__("cLookAtChaseAlgorithm_setupChaseData");
extern "C" t4Vector f_1785F0_1787E0(void* self) __asm__("cSUPERPIPEChaseAlgorithm_getIdealPosition");
extern "C" t4Vector f_178758_1787E0(void* self) __asm__("cSUPERPIPEChaseAlgorithm_getIdealTargetPos");

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1787E0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (*v * s).
static inline t4Vector ScaleP_1787E0(const t4Vector* v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void ScaleEq_1787E0(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1787E0(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

/**
 * Runs one frame of the SUPERPIPE camera: eye on the pipe's axis, aimed just below the rider, at most 600 away.
 *
 * After setupChaseData (cLookAtChaseAlgorithm_setupChaseData) sets View to pi/4, Position to getIdealPosition and CamTargetPos to
 * getIdealTargetPos. When the two are over 600 apart the eye is pulled in to 600. When they are under 1 apart it
 * computes a fallback offset (X axis * 600) but never applies it, so the degenerate pose stays.
 */
extern "C" void cSUPERPIPEChaseAlgorithm_update(cSUPERPIPEChaseAlgorithm* self)
{
    sCamInfo info;
    cLookAtChaseAlgorithm_setupChaseData_87E0(((char*)self), &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    *(float*)self = 0.7853981852531433f;
    self->Position = f_1785F0_1787E0(((char*)self));
    self->CamTargetPos = f_178758_1787E0(((char*)self));
    t4Vector look = Sub_1787E0(self->CamTargetPos, self->Position);
    float len = Length_1787E0(look);
    if (len < 1.0f) {
        look = ScaleP_1787E0(&t4vect_XAxis, 600.0f);
    } else if (len > 600.0f) {
        ScaleEq_1787E0(look, 600.0f / len);
        self->Position = Sub_1787E0(self->CamTargetPos, look);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cSUPERPIPEChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(void* self, float a, float b);

struct sVec4_00178938 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" sVec4_00178938 cSUPERPIPEChaseAlgorithm_getIdealPosition(void* self);
extern "C" sVec4_00178938 cSUPERPIPEChaseAlgorithm_getIdealTargetPos(void* self);

class cCamTarget_00178938 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int getAIIndex();
};
class cCamCtrl_00178938 {
public:
    char pad_0x00[0x10];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void update();
};

/**
 * Snaps the SUPERPIPE chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match sets Position and
 * CamTargetPos to getIdealPosition and getIdealTargetPos, then runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cSUPERPIPEChaseAlgorithm_snap(cSUPERPIPEChaseAlgorithm* self, int id)
{
    char* rider = *(char**)((char*)self->Target + 4);
    if (id == ((cCamTarget_00178938*)(rider + 0x6C0))->getAIIndex()) {
        *(sVec4_00178938*)&self->Position = cSUPERPIPEChaseAlgorithm_getIdealPosition(((cCamCtrl_00178938*)self));
        *(sVec4_00178938*)&self->CamTargetPos = cSUPERPIPEChaseAlgorithm_getIdealTargetPos(((cCamCtrl_00178938*)self));
        cLookAtChaseAlgorithm_snap(((cCamCtrl_00178938*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cCamCtrl_00178938*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrl_00178938*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm);
#ifdef SKIP_ASM
struct sVec4_1789E8
{
    float x, y, z, w;
} __attribute__((aligned(16)));


struct sVEntry_1789E8 { short delta; short index; sVec4_1789E8 (*fn)(void*); };

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4_1789E8 Sub_1789E8(const sVec4_1789E8& a, const sVec4_1789E8& b)
{
    sVec4_1789E8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec4_1789E8 Add_1789E8(const sVec4_1789E8& a, const sVec4_1789E8& b)
{
    sVec4_1789E8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec4_1789E8 Scale_1789E8(const sVec4_1789E8& v, float s)
{
    sVec4_1789E8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4_1789E8 Normalize_1789E8(const sVec4_1789E8& v)
{
    sVec4_1789E8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1789E8(const sVec4_1789E8& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void DivEq_1789E8(sVec4_1789E8& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_1789E8(const sVec4_1789E8& a, const sVec4_1789E8& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

struct sQuad_1789E8
{
    float v[4];
} __attribute__((aligned(16)));
extern sQuad_1789E8 D_004FF140_v1789E8 __asm__("t4vect_XAxis");
extern void* cPOST_RACE_1ChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(void* self);

static inline sVec4_1789E8 Get_1789E8(char* o, int slot)
{
    sVEntry_1789E8* e = &(*(sVEntry_1789E8**)o)[slot];
    return e->fn(o + e->delta);
}

/**
 * Constructs the post-race chase camera, fixing its start direction from the rider's motion.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x44 (On Tour CAMERA_ALGORITHM_CHASE_POST_RACE_1 = 68),
 * cPOST_RACE_1ChaseAlgorithm_vtbl and AgeInFrames 0. StartDir is the target's velocity (slot 3) normalised when
 * longer than 1, else its front (slot 4); z is then cleared and it is renormalised, or set to the X axis (t4vect_XAxis)
 * when under 0.01.
 *
 * @param src  the rider camera target (On Tour cRiderCameraTarget* target)
 * @return @p self
 */
extern "C" void* cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm(cPOST_RACE_1ChaseAlgorithm* self, cRiderCameraTarget* src)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(((char*)self));
    sVec4_1789E8* startDir = (sVec4_1789E8*)(((char*)self) + 0x390);
    self->AlgorithmType = 68;
    self->mVtbl = cPOST_RACE_1ChaseAlgorithm_vtbl;
    self->AgeInFrames = 0;
    sVec4_1789E8 dir = (Length_1789E8(Get_1789E8(((char*)src), 3)) > 1.0f) ? Normalize_1789E8(Get_1789E8(((char*)src), 3)) : Get_1789E8(((char*)src), 4);
    *startDir = dir;
    self->StartDir.z = 0.0f;
    float len = Length_1789E8(*(sVec4_1789E8*)&self->StartDir);
    if (len > 0.009999999776482582f)
        DivEq_1789E8(*(sVec4_1789E8*)&self->StartDir, len);
    else
        *(sQuad_1789E8*)&self->StartDir = D_004FF140_v1789E8;
    return ((char*)self);
}
#endif

extern void* cPOST_RACE_1ChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cPOST_RACE_1ChaseAlgorithm__cPOST_RACE_1ChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a post-race chase camera algorithm: stores cPOST_RACE_1ChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cPOST_RACE_1ChaseAlgorithm__cPOST_RACE_1ChaseAlgorithm(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)cPOST_RACE_1ChaseAlgorithm_vtbl;
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cPOST_RACE_1ChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the post-race chase camera's rest drop (vtable slot 13, On Tour getRestDrop): always 0.
 *
 * @return 0
 */
float cPOST_RACE_1ChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cPOST_RACE_1ChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the post-race chase camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cPOST_RACE_1ChaseAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cPOST_RACE_1ChaseAlgorithm_update);
#ifdef SKIP_ASM
extern "C" float fCos(float x);
extern "C" void fSinCos(float* s, float* c, float angle);

struct sVec_00178BB0 {
    float x, y, z, w;
    sVec_00178BB0() {}
    sVec_00178BB0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCamInfo_00178BB0 {
    char data[0x68];
} __attribute__((aligned(16)));

extern sVec_00178BB0 t4vect_ZAxis;
extern "C" void cLookAtChaseAlgorithm_setupChaseData(void* self, sCamInfo_00178BB0* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);

struct sVEv_00178BB0 {
    short delta;
    short index;
    sVec_00178BB0 (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_00178BB0 ctScale_00178BB0(const sVec_00178BB0& v, float s)
{
    sVec_00178BB0 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_00178BB0 ctAdd_00178BB0(const sVec_00178BB0& a, const sVec_00178BB0& b)
{
    sVec_00178BB0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_00178BB0 ctSub_00178BB0(const sVec_00178BB0& a, const sVec_00178BB0& b)
{
    sVec_00178BB0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sVec_00178BB0 ctRot_00178BB0(const sVec_00178BB0& q, const sVec_00178BB0& v)
{
    sVec_00178BB0 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_00178BB0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float wrap_00178BB0(float x)
{
    return x - ffloor_00178BB0(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

static inline sVec_00178BB0 AxisAngle_00178BB0(const sVec_00178BB0& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    sVec_00178BB0 q;
    q.w = c;
    q.x = s * axis.x;
    q.y = s * axis.y;
    q.z = s * axis.z;
    return q;
}

static inline sVec_00178BB0 getPos_00178BB0(char* self)
{
    char* o = *(char**)(self + 0x30);
    sVEv_00178BB0* e = &(*(sVEv_00178BB0**)o)[1];
    return e->fn(o + e->delta);
}

/**
 * Runs one frame of the post-race camera: a slow side-to-side swing of the eye behind the rider's start direction.
 *
 * After setupChaseData (cLookAtChaseAlgorithm_setupChaseData) AgeInFrames advances modulo 1100 (18.3 s). The swing angle is (1 - cos(2pi *
 * age / 1100)) * 0.748 - 0.748, wrapped into [-pi, pi), and StartDir rotated by it about Z gives v. CamTargetPos is
 * the target position (slot 1) - Z * 3; Position = CamTargetPos - v * 126 - Z * 27, or v * 326 back and 123 up when
 * two or more riders are allocated (cAI::NumberOfRidersAllocated, AI + 0x78).
 */
extern "C" void cPOST_RACE_1ChaseAlgorithm_update(cPOST_RACE_1ChaseAlgorithm* self)
{
    sCamInfo_00178BB0 info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    (self->AgeInFrames)++;
    self->AgeInFrames %= 1100;
    float ang = wrap_00178BB0((float)self->AgeInFrames * 0.0009090909152291715f * 6.2831854820251465f);
    float angle = (1.0f - fCos(ang)) * 0.7482788562774658f + -0.7482788562774658f;
    angle -= ffloor_00178BB0(angle * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
    sVec_00178BB0 rotation = AxisAngle_00178BB0(t4vect_ZAxis, angle);
    sVec_00178BB0 look = ctRot_00178BB0(rotation, *(sVec_00178BB0*)&self->StartDir);
    *(sVec_00178BB0*)&self->CamTargetPos = ctAdd_00178BB0(getPos_00178BB0(((char*)self)), ctScale_00178BB0(t4vect_ZAxis, -3.0f));
    int multi = SSXApp->Game->AI->NumberOfRidersAllocated >= 2;
    float multiplayerHeight = 0.0f;
    if (multi) {
        multiplayerHeight = 150.0f;
    }
    float multiplayerDistance = 0.0f;
    if (multi) {
        multiplayerDistance = 200.0f;
    }
    *(sVec_00178BB0*)&self->Position =
        ctAdd_00178BB0(ctSub_00178BB0(*(sVec_00178BB0*)&self->CamTargetPos, ctScale_00178BB0(look, multiplayerDistance + 126.0f)),
                       ctScale_00178BB0(t4vect_ZAxis, multiplayerHeight + -27.0f));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cPOST_RACE_1ChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(void* self, float a, float b);

struct sCamVEntryIntA {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoidA {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Snaps the post-race chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(100, 100) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cPOST_RACE_1ChaseAlgorithm_snap(cPOST_RACE_1ChaseAlgorithm* self, int id)
{
    char* obj = *(char**)((char*)self->Target + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        cLookAtChaseAlgorithm_snap(self, id);
        cLookAtChaseAlgorithm_setupInitialCamDir(self, 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = (sCamVEntryVoidA*)self->mVtbl;
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178F18);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest (variadic).
 *
 * The body only spills the other argument registers. Nothing in the ELF calls it or stores its address; the unit has
 * one such stub, like func_00174808 in the camera trigger unit, placed just before its static initializer
 * (func_00178F58).
 *
 * @return the first argument
 */
extern "C" void* func_00178F18(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178F58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColor_178F58 {
    float r, g, b, a;
    sColor_178F58(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

extern sColor_178F58 D_004C5C10;
extern sColor_178F58 D_004C5C20;
extern sColor_178F58 D_004C5C30;
extern sColor_178F58 D_004C5C40;
extern sColor_178F58 D_004C5C50;
extern sColor_178F58 D_004C5C60;
extern sColor_178F58 D_004C5C70;
extern sColor_178F58 D_004C5C80;
extern sColor_178F58 D_004C5C90;
extern sColor_178F58 D_004C5CA0;
extern sColor_178F58 D_004C5CB0;
extern sColor_178F58 D_004C5CC0;
extern sColor_178F58 D_004C5CD0;
extern sColor_178F58 D_004C5CE0;
extern sColor_178F58 D_004C5CF0;
extern sColor_178F58 D_004C5D00;
extern sColor_178F58 D_004C5D10;
extern sColor_178F58 D_004C5D20;
extern sColor_178F58 D_004C5D30;
extern sColor_178F58 D_004C5D40;
extern sColor_178F58 D_004C5D50;
extern sColor_178F58 D_004C5D60;
extern sColor_178F58 D_004C5D70;
extern sColor_178F58 D_004C5D80;
extern sColor_178F58 D_004C5D90;
extern sColor_178F58 D_004C5DA0;
extern sColor_178F58 D_004C5DB0;
extern sColor_178F58 D_004C5DC0;
extern sColor_178F58 D_004C5DD0;
extern sColor_178F58 D_004C5DE0;
extern sColor_178F58 D_004C5DF0;
extern sColor_178F58 D_004C5E00;
extern sColor_178F58 D_004C5E10;
extern sColor_178F58 D_004C5E20;
extern sColor_178F58 D_004C5E30;
extern sColor_178F58 D_004C5E40;
extern sColor_178F58 D_004C5E50;
extern sColor_178F58 D_004C5E60;
extern sColor_178F58 D_004C5E70;
extern sColor_178F58 D_004C5E80;
extern sColor_178F58 D_004C5E90;
extern sColor_178F58 D_004C5EA0;
extern sColor_178F58 D_004C5EB0;
extern sColor_178F58 D_004C5EC0;
extern sColor_178F58 D_004C5ED0;
extern sColor_178F58 D_004C5EE0;
extern sColor_178F58 D_004C5EF0;
extern sColor_178F58 D_004C5F00;
extern sColor_178F58 D_004C5F10;
extern sColor_178F58 D_004C5F20;
extern sColor_178F58 D_004C5F30;
extern sColor_178F58 D_004C5F40;
extern sColor_178F58 D_004C5F50;
extern sColor_178F58 D_004C5F60;
extern sColor_178F58 D_004C5F70;
extern sColor_178F58 D_004C5F80;

// The unit declares func_00178F58 later as returning void*; bind this body by asm label.
extern "C" void func_00178F58_impl(int initialize, int priority) __asm__("func_00178F58");

/**
 * Runs the static constructors of this unit's globals (g++'s static initialization; there is nothing to destroy).
 *
 * Only priority 0xFFFF with @p initialize set does anything: it constructs the 56 four-float constants
 * D_004C5C10..D_004C5F80 (the colour and blend vectors every unit of this kind sets up; nothing else in the ELF reads
 * these copies). func_00179738 calls it with (1, 0xFFFF).
 *
 * @param initialize  1 to construct; 0 does nothing
 * @param priority  the init priority; only 0xFFFF is handled
 */
extern "C" void func_00178F58_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004C5C10) sColor_178F58(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004C5C20) sColor_178F58(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5C30) sColor_178F58(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5C40) sColor_178F58(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004C5C50) sColor_178F58(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004C5C60) sColor_178F58(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5C70) sColor_178F58(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004C5C80) sColor_178F58(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004C5C90) sColor_178F58(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004C5CA0) sColor_178F58(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004C5CB0) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5CC0) sColor_178F58(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5CD0) sColor_178F58(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004C5CE0) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5CF0) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5D00) sColor_178F58(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5D10) sColor_178F58(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5D20) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5D30) sColor_178F58(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5D40) sColor_178F58(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5D50) sColor_178F58(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5D60) sColor_178F58(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004C5D70) sColor_178F58(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004C5D80) sColor_178F58(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004C5D90) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5DA0) sColor_178F58(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004C5DB0) sColor_178F58(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004C5DC0) sColor_178F58(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004C5DD0) sColor_178F58(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004C5DE0) sColor_178F58(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004C5DF0) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5E00) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5E10) sColor_178F58(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004C5E20) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5E30) sColor_178F58(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004C5E40) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5E50) sColor_178F58(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5E60) sColor_178F58(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5E70) sColor_178F58(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004C5E80) sColor_178F58(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004C5E90) sColor_178F58(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004C5EA0) sColor_178F58(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004C5EB0) sColor_178F58(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5EC0) sColor_178F58(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004C5ED0) sColor_178F58(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5EE0) sColor_178F58(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5EF0) sColor_178F58(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5F00) sColor_178F58(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004C5F10) sColor_178F58(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004C5F20) sColor_178F58(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004C5F30) sColor_178F58(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004C5F40) sColor_178F58(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004C5F50) sColor_178F58(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004C5F60) sColor_178F58(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004C5F70) sColor_178F58(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004C5F80) sColor_178F58(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

extern "C" void* func_00178F58(int, int);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179730);
#ifdef SKIP_ASM
/**
 * Does nothing; nothing calls it or stores its address.
 */
extern "C" void func_00179730(void) {

}
#endif

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179738__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static constructors: calls func_00178F58 with (1, 0xFFFF).
 *
 * Listed in the global constructor table (word at 0x0043CE50).
 *
 * @return func_00178F58's result
 */
void* func_00179738(void* self)
{
    return func_00178F58(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179758);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest (variadic).
 *
 * The body only spills the other argument registers. Nothing in the ELF calls it or stores its address; the unit has
 * one such stub, like func_00174808 in the camera trigger unit, placed just before its static initializer
 * (func_00179798).
 *
 * @return the first argument
 */
extern "C" void* func_00179758(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179798);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColor_179798 {
    float r, g, b, a;
    sColor_179798(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sFlags_179798 {
    unsigned char a;
    unsigned int b : 24;
    sFlags_179798(unsigned char aa, unsigned int ab) : a(aa), b(ab) {}
};

extern sFlags_179798 D_004A4DF8;
extern sColor_179798 D_004C5F90;
extern sColor_179798 D_004C5FA0;
extern sColor_179798 D_004C5FB0;
extern sColor_179798 D_004C5FC0;
extern sColor_179798 D_004C5FD0;
extern sColor_179798 D_004C5FE0;
extern sColor_179798 D_004C5FF0;
extern sColor_179798 D_004C6000;
extern sColor_179798 D_004C6010;
extern sColor_179798 D_004C6020;
extern sColor_179798 D_004C6030;
extern sColor_179798 D_004C6040;
extern sColor_179798 D_004C6050;
extern sColor_179798 D_004C6060;
extern sColor_179798 D_004C6070;
extern sColor_179798 D_004C6080;
extern sColor_179798 D_004C6090;
extern sColor_179798 D_004C60A0;
extern sColor_179798 D_004C60B0;
extern sColor_179798 D_004C60C0;
extern sColor_179798 D_004C60D0;
extern sColor_179798 D_004C60E0;
extern sColor_179798 D_004C60F0;
extern sColor_179798 D_004C6100;
extern sColor_179798 D_004C6110;
extern sColor_179798 D_004C6120;
extern sColor_179798 D_004C6130;
extern sColor_179798 D_004C6140;
extern sColor_179798 D_004C6150;
extern sColor_179798 D_004C6160;
extern sColor_179798 D_004C6170;
extern sColor_179798 D_004C6180;
extern sColor_179798 D_004C6190;
extern sColor_179798 D_004C61A0;
extern sColor_179798 D_004C61B0;
extern sColor_179798 D_004C61C0;
extern sColor_179798 D_004C61D0;
extern sColor_179798 D_004C61E0;
extern sColor_179798 D_004C61F0;
extern sColor_179798 D_004C6200;
extern sColor_179798 D_004C6210;
extern sColor_179798 D_004C6220;
extern sColor_179798 D_004C6230;
extern sColor_179798 D_004C6240;
extern sColor_179798 D_004C6250;
extern sColor_179798 D_004C6260;
extern sColor_179798 D_004C6270;
extern sColor_179798 D_004C6280;
extern sColor_179798 D_004C6290;
extern sColor_179798 D_004C62A0;
extern sColor_179798 D_004C62B0;
extern sColor_179798 D_004C62C0;
extern sColor_179798 D_004C62D0;
extern sColor_179798 D_004C62E0;
extern sColor_179798 D_004C62F0;
extern sColor_179798 D_004C6300;

// The unit declares func_00179798 later as returning void*; bind this body by asm label.
extern "C" void func_00179798_impl(int initialize, int priority) __asm__("func_00179798");

/**
 * Runs the static constructors of this unit's globals (g++'s static initialization; there is nothing to destroy).
 *
 * Only priority 0xFFFF with @p initialize set does anything: it constructs the 56 four-float constants
 * D_004C5F90..D_004C6300 (the colour and blend vectors every unit of this kind sets up; nothing else in the ELF reads
 * these copies), then the two-word slot D_004A4DF8 (a byte 0xFF followed by the 24-bit value 0xFFFFFF). func_00179FA8
 * calls it with (1, 0xFFFF).
 *
 * @param initialize  1 to construct; 0 does nothing
 * @param priority  the init priority; only 0xFFFF is handled
 */
extern "C" void func_00179798_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004C5F90) sColor_179798(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004C5FA0) sColor_179798(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5FB0) sColor_179798(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5FC0) sColor_179798(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004C5FD0) sColor_179798(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004C5FE0) sColor_179798(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5FF0) sColor_179798(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004C6000) sColor_179798(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004C6010) sColor_179798(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004C6020) sColor_179798(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004C6030) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C6040) sColor_179798(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C6050) sColor_179798(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004C6060) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C6070) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C6080) sColor_179798(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C6090) sColor_179798(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C60A0) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C60B0) sColor_179798(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C60C0) sColor_179798(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C60D0) sColor_179798(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C60E0) sColor_179798(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004C60F0) sColor_179798(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004C6100) sColor_179798(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004C6110) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C6120) sColor_179798(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004C6130) sColor_179798(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004C6140) sColor_179798(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004C6150) sColor_179798(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004C6160) sColor_179798(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004C6170) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C6180) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C6190) sColor_179798(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004C61A0) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C61B0) sColor_179798(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004C61C0) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C61D0) sColor_179798(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C61E0) sColor_179798(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004C61F0) sColor_179798(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004C6200) sColor_179798(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004C6210) sColor_179798(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004C6220) sColor_179798(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004C6230) sColor_179798(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C6240) sColor_179798(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004C6250) sColor_179798(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C6260) sColor_179798(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C6270) sColor_179798(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C6280) sColor_179798(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004C6290) sColor_179798(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004C62A0) sColor_179798(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004C62B0) sColor_179798(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004C62C0) sColor_179798(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004C62D0) sColor_179798(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004C62E0) sColor_179798(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004C62F0) sColor_179798(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004C6300) sColor_179798(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004A4DF8) sFlags_179798(0xFF, 0xFFFFFF);
    }
}
#endif

extern "C" void* func_00179798(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179FA8__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static constructors: calls func_00179798 with (1, 0xFFFF).
 *
 * Listed in the global constructor table (word at 0x0043CE54).
 *
 * @return func_00179798's result
 */
void* func_00179FA8(void* self)
{
    return func_00179798(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_testWCBAMStartLocation);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationType(void* iface, int id);

/**
 * Tells whether a location can be a World Championship backcountry (WC BAM) start location.
 *
 * True when cBENewRaceInterface_getLocationType (On Tour cBENewRaceInterface::getLocationType) on the new-race interface
 * (cBE_getInterface 0) returns 1 for @p id, or when @p id is 14, 15 or 16.
 *
 * @param id  the location (On Tour eBELocationID)
 * @return 1 for a WC BAM start location, otherwise 0
 */
extern "C" int cTrackFileMan_testWCBAMStartLocation(void* self, int id)
{
    if (cBENewRaceInterface_getLocationType(cBE_getInterface_Fv(cBE_getBE(), 0), id) == 1) {
        return 1;
    }
    if (id >= 0xE && id <= 0x10) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_getNextWCBAMStartLocation);
#ifdef SKIP_ASM
struct cTrackFileMan_getNextWCBAMStartLocation_sEntry {
    int active;
    char pad_0x4[0x88];
};

struct cTrackFileMan_getNextWCBAMStartLocation_sMgr {
    int count;
    char pad_0x4[0x88];
    cTrackFileMan_getNextWCBAMStartLocation_sEntry entries[1];
};

/**
 * Finds the next location after @p i that is a WC BAM start location, wrapping around.
 *
 * Returns -1 when the manager has no such locations (+0x8C08, On Tour NumWCBAMStartLocations); otherwise steps
 * forward modulo NumLocations (+0x0) until an entry's WC-BAM-start flag (entry +0x88) is set. Loops forever if the
 * count is nonzero but no flag is set.
 *
 * @param i  the current location index
 * @return the next WC BAM start location index, or -1
 */
extern "C" int cTrackFileMan_getNextWCBAMStartLocation(cTrackFileMan_getNextWCBAMStartLocation_sMgr* self, int i)
{
    if (*(int*)((char*)self + 0x8C08) == 0) {
        return -1;
    }
    i = (i + 1) % self->count;
    while (self->entries[i].active == 0) {
        i = (i + 1) % self->count;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_getPrevWCBAMStartLocation);
#ifdef SKIP_ASM
/**
 * Finds the previous location before @p i that is a WC BAM start location, wrapping around.
 *
 * Returns -1 when the manager has no such locations (+0x8C08); otherwise steps backward modulo NumLocations until
 * an entry's WC-BAM-start flag (entry +0x88) is set.
 *
 * @param i  the current location index
 * @return the previous WC BAM start location index, or -1
 */
extern "C" int cTrackFileMan_getPrevWCBAMStartLocation(cTrackFileMan_getNextWCBAMStartLocation_sMgr* self, int i)
{
    if (*(int*)((char*)self + 0x8C08) == 0) {
        return -1;
    }
    i = (i + self->count - 1) % self->count;
    while (self->entries[i].active == 0) {
        i = (i + self->count - 1) % self->count;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_legalTrackMode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_A158(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationType(void* iface, int id);

// PORT: the unit also declares this with a pointer third argument; defined under an asm label with its real type
extern "C" int cTrackFileMan_legalTrackMode_impl(int* list, int idx, int kind) __asm__("cTrackFileMan_legalTrackMode");
/**
 * Tells whether a location may be raced in a game mode (debug menu track picker).
 *
 * Out-of-range @p idx gives 0. Otherwise the entry's location id (entry +0x80) must fall in the mode's peak range:
 * mode 0 ids 0..4, 1 ids 5..7, 2 ids 8..10, 3 ids 11..13, 5 and 6 ids 14..16; mode 4 accepts any location for
 * which cBENewRaceInterface_getLocationType (On Tour cBENewRaceInterface::getLocationType) returns 0. Other modes give 0.
 *
 * @param list  the track file manager (this)
 * @param idx   the location index
 * @param kind  the game mode (On Tour eBEGameMode)
 * @return 1 when the location is legal for the mode, otherwise 0
 */
extern "C" int cTrackFileMan_legalTrackMode_impl(int* list, int idx, int kind)
{
    if (idx >= 0 && idx < list[0]) {
        unsigned int locationId = *(unsigned int*)((char*)list + idx * 0x8C + 0x84);
        switch (kind) {
        case 0:
            if (locationId < 5) return 1;
            break;
        case 1:
            if (locationId >= 5 && locationId < 8) return 1;
            break;
        case 2:
            if (locationId >= 8 && locationId < 11) return 1;
            break;
        case 3:
            if (locationId >= 11 && locationId < 14) return 1;
            break;
        case 5:
        case 6:
            if (locationId >= 14 && locationId < 17) return 1;
            break;
        case 4:
            if (!cBENewRaceInterface_getLocationType(cBE_getInterface_A158(cBE_getBE(), 0), locationId)) return 1;
            break;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_getNextLocation);
#ifdef SKIP_ASM
extern "C" int cTrackFileMan_legalTrackMode(int* self, int i, void* key);

/**
 * Finds the next location after @p start that is legal for a game mode, wrapping around.
 *
 * Tries each index once, starting at (@p start + 1) mod NumLocations, with cTrackFileMan_legalTrackMode.
 *
 * @param start  the current location index
 * @param key    the game mode (On Tour eBEGameMode)
 * @return the next legal location index, or -1 when none is legal
 */
// Circular search: first index after `start` for which cTrackFileMan_legalTrackMode matches.
extern "C" int cTrackFileMan_getNextLocation(int* self, int start, void* key)
{
    int n;
    int i = (start + 1) % self[0];
    for (n = 0; n < self[0]; n++) {
        if (cTrackFileMan_legalTrackMode(self, i, key))
            return i;
        i = (i + 1) % self[0];
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_getPrevLocation);
#ifdef SKIP_ASM
extern "C" int cTrackFileMan_legalTrackMode(int* self, int i, void* key);

/**
 * Finds the previous location before @p start that is legal for a game mode, wrapping around.
 *
 * Tries each index once, starting at (@p start + NumLocations - 1) mod NumLocations, with cTrackFileMan_legalTrackMode.
 *
 * @param start  the current location index
 * @param key    the game mode (On Tour eBEGameMode)
 * @return the previous legal location index, or -1 when none is legal
 */
// Circular search backwards: first index before `start` for which cTrackFileMan_legalTrackMode matches.
extern "C" int cTrackFileMan_getPrevLocation(int* self, int start, void* key)
{
    int n;
    int i = (start + self[0] - 1) % self[0];
    for (n = 0; n < self[0]; n++) {
        if (cTrackFileMan_legalTrackMode(self, i, key))
            return i;
        i = (i + self[0] - 1) % self[0];
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_scanTrackIndex);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* FILE_loadsizez(const char* name, int* size, int align);
extern "C" void cBXString_cBXString3(void* self, const char* s, int len);
extern "C" void* MEM_free(void* self);
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: alias; the unit defines cTrackFileMan_checkLocation later with a typed self (sCamMgr_A830*).
extern "C" int cTrackFileMan_checkLocation_17A378(void* selfp, char* name, char* tag) __asm__("cTrackFileMan_checkLocation");
extern "C" char* strcpy(char* d, const char* s);

struct sCamEntry_17A378 {
    char name[0x40];    // 0x00
    char tag[0x40];     // 0x40
    int type;           // 0x80
    int f84;            // 0x84
    int f88;            // 0x88
};

struct sCamMgr_17A378 {
    int count;                          // 0x0000
    sCamEntry_17A378 entries[0x100];    // 0x0004
};

struct sStr_17A378 {
    char* s;
    sStr_17A378() {}
    sStr_17A378(const sStr_17A378&);
};

/**
 * Adds the track files named in a text index file to the location table, skipping duplicates and files not on disc.
 *
 * Loads @p file whole (FILE_loadsizez, 0x100 alignment) and splits it into names: whitespace separates them, a leading
 * quote and a trailing quote are skipped, a name runs over letters, digits and "_.-/", a "\" or ":" restarts it
 * (dropping any path), and everything from the first "." on is dropped. For each non-empty name, made into a cBXString
 * (cBXString_cBXString3), cTrackFileMan_checkLocation is asked with the name as both location file and big file; when it says yes the name
 * is copied into the next entry's File (+0x0) and BigFile (+0x40) fields (0x8C-byte entries from +0x4), the entry's
 * location id (+0x80) is set to 22 (one past the built-in locations) and its BAM flag (+0x84) to 0, and the count
 * (+0x0) grows. There is no check against the 256-entry limit. The buffer is freed with MEM_free.
 *
 * Nothing in the ELF calls it.
 *
 * @param selfp  the track file manager (this, On Tour cTrackFileMan)
 * @param file  path of the index file
 */
extern "C" void cTrackFileMan_scanTrackIndex(void* selfp, const char* file)
{
    sCamMgr_17A378* self = (sCamMgr_17A378*)selfp;
    int size;
    char* buf = (char*)FILE_loadsizez(file, &size, 0x100);
    if (buf == 0)
        return;
    char* cursor = buf;
    for (;;) {
        while (cursor < buf + size) {
            int ws = 0;
            char curChar = *cursor;
            if (curChar == ' ' || curChar == '\t' || curChar == '\n' || curChar == '\r')
                ws = 1;
            if (!ws)
                break;
            cursor++;
        }
        if (!(cursor < buf + size))
            break;
        if (*cursor == '"' || *cursor == '\'')
            cursor++;
        if (!(cursor < buf + size))
            break;
        char* start = cursor;
        char* stop = 0;
        for (;;) {
            while (cursor < buf + size) {
                int ok = 0;
                char curChar = *cursor;
                if ((curChar >= 'a' && curChar <= 'z') || (curChar >= 'A' && curChar <= 'Z') || (curChar >= '0' && curChar <= '9') ||
                    curChar == '_' || curChar == '.' || curChar == '-' || curChar == '/')
                    ok = 1;
                if (!ok)
                    break;
                if (stop == 0 && *cursor == '.')
                    stop = cursor;
                cursor++;
            }
            if ((cursor < buf + size && *cursor == '\\') || *cursor == ':') {
                start = cursor + 1;
                stop = 0;
                cursor = start;
                continue;
            }
            break;
        }
        if (stop == 0)
            stop = cursor;
        if (cursor < buf + size && (*cursor == '"' || *cursor == '\''))
            cursor++;
        if (stop == start)
            continue;
        sStr_17A378 str;
        cBXString_cBXString3(&str, start, stop - start);
        if (cTrackFileMan_checkLocation_17A378(self, str.s, str.s)) {
            strcpy(self->entries[self->count].name, str.s);
            strcpy(self->entries[self->count].tag, str.s);
            self->entries[self->count].type = 22;
            self->entries[self->count].f84 = 0;
            self->count++;
        }
        cBXString__cBXString(&str, 2);
    }
    MEM_free(buf);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_scanLocations);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// The track file manager's location table (0x8C0C bytes; the structs keep their old sCam* names): up to 0x100 named
// track locations with their tag, index and flags; cTrackFileMan_scanLocations builds it, cTrackFileMan_getNextWCBAMStartLocation / cTrackFileMan_getPrevWCBAMStartLocation step through the valid ones.
struct sCamEntry {
    char name[0x40];    // 0x00
    char tag[4];        // 0x40
    char pad44[0x3C];
    int idx;            // 0x80
    int active;         // 0x84
    int valid;          // 0x88
};

struct sCamMgr {
    int count;                  // 0x0000
    sCamEntry entries[0x100];   // 0x0004
    int numActive;              // 0x8C04
    int numValid;               // 0x8C08
};

// One name looked up in the bigfile by func_003A6948 (found flag written back).
struct sCamName {
    char* name;
    int ok;
};

extern "C" void* MEM_fill(void* dst, int v, int n);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_17A638(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cBENewRaceInterface_getLocationFile(void* iface, int id);
extern "C" int cTrackFileMan_checkLocation1_A638(void* self, int n, void* names, char* tag) __asm__("cTrackFileMan_checkLocation1");
extern "C" int cTrackFileMan_testWCBAMStartLocation_A638(void* self, int id) __asm__("cTrackFileMan_testWCBAMStartLocation");
extern "C" char* strcpy(char* d, const char* s);
extern char* str_BAM_b[];

/**
 * Builds the debug menu's location table from the race locations present on disc.
 *
 * Zeroes NumLocations (+0x0), the BAM and WC-BAM-start counts and the 256-entry table (0x8C bytes each), asks
 * cTrackFileMan_checkLocation1 (On Tour checkLocation) which of the 0x16 location files (cBENewRaceInterface_getLocationFile)
 * exist, and for each one found copies its file name, records its location id, marks it a BAM location and,
 * when cTrackFileMan_testWCBAMStartLocation says so, a WC BAM start. On Tour's second pass for separate "%sBAM" files is not here.
 */
extern "C" void cTrackFileMan_scanLocations(sCamMgr* self)
{
    sCamName names[0x18];
    int i;
    self->count = 0;
    self->numActive = 0;
    self->numValid = 0;
    MEM_fill(self->entries, 0, 35840);
    void* iface = cBE_getInterface_17A638(cBE_getBE(), 0);
    for (i = 0; i < 22; i++)
        names[i].name = cBENewRaceInterface_getLocationFile(iface, i);
    if (cTrackFileMan_checkLocation1_A638(self, 22, names, (char*)str_BAM_b)) {
        int j;
        for (j = 0; j < 22 && self->count < 0x100; j++) {
            if (names[j].ok) {
                int* ip = &self->entries[0].idx;
                int* ap = &self->entries[0].active;
                strcpy(self->entries[self->count].name, cBENewRaceInterface_getLocationFile(iface, j));
                *(char**)((char*)self + 0x44 + self->count * 0x8C) = str_BAM_b[0];
                ip[self->count * 35] = j;
                ap[self->count * 35] = 1;
                self->numActive++;
                if (cTrackFileMan_testWCBAMStartLocation_A638(self, ip[self->count * 35])) {
                    *(int*)((char*)self + 0x8C + self->count * 0x8C) = 1;
                    self->numValid++;
                }
                self->count++;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_checkLocation);
#ifdef SKIP_ASM
struct cBXString;
struct cBigFile;
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" cBXString* func_00318630(cBXString* self, cBXString* a, const char* str);
extern "C" cBXString* func_003186D0(cBXString* self, const char* str, cBXString* b);
// PORT: the bigfile ctor forwards (path, flags) to cBigFile_open; bind the 3-arg form.
cBigFile* cBigFile_cBigFile1_A830(cBigFile* self, const char* path, int flags) __asm__("cBigFile_cBigFile1__FP8cBigFile");
void cBigFile__cBigFile(cBigFile* self, int flags);
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int func_003A6948(const char* path, int n, void* names);
extern char str_data_worlds[];
extern char str_big[];

struct sCamEntry_A830 {
    char name[0x40];    // 0x00
    char tag[4];        // 0x40
    char pad_44[0x48];
};

struct sCamMgr_A830 {
    int count;                          // 0x0000
    sCamEntry_A830 entries[0x100];    // 0x0004
};

struct sCamName_A830 {
    char* name;
    int ok;
};

// Non-POD (user-declared copy ctor, never defined) so locals get the 16-byte slots g++ gives classes.
struct sStr_A830 {
    char* s;
    sStr_A830() {}
    sStr_A830(const sStr_A830&);
};

struct sBigFile_A830 {
    int f0;
    int f4;
    sBigFile_A830() {}
    sBigFile_A830(const sBigFile_A830&);
};

/**
 * Tells whether a track location file can be added to the location table: it is not listed yet and its big file has
 * that section.
 *
 * Returns 0 when an entry already has File == @p name and BigFile == @p tag (strcasecmp on +0x0 and +0x40 of each
 * 0x8C-byte entry). Otherwise opens the big file "data/worlds/<tag>.big" (str_data_worlds + @p tag + str_big, cBigFile
 * flags 0x100) and asks func_003A6948 whether section @p name exists in "data/worlds/<tag>"; the big file and the
 * strings are destroyed again before returning.
 *
 * Called by cTrackFileMan_scanTrackIndex.
 *
 * @param name  the location (section) file name (On Tour sLocationFile)
 * @param tag  the big file name (On Tour sBigFile)
 * @return 1 when the section exists and is not in the table yet, else 0
 */
extern "C" int cTrackFileMan_checkLocation(sCamMgr_A830* self, char* name, char* tag)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (strcasecmp(name, self->entries[i].name) == 0 &&
            strcasecmp(tag, self->entries[i].tag) == 0) {
            return 0;
        }
    }
    sStr_A830 base;
    sStr_A830 workStr;
    sBigFile_A830 bf;
    sCamName_A830 nm;
    char* pre = str_data_worlds;
    cBXString_cBXString2(&workStr, tag);
    func_003186D0((cBXString*)&base, pre, (cBXString*)&workStr);
    cBXString__cBXString(&workStr, 2);
    cBigFile* pbf = (cBigFile*)&bf;
    func_00318630((cBXString*)&workStr, (cBXString*)&base, str_big);
    cBigFile_cBigFile1_A830(pbf, workStr.s, 0x100);
    cBXString__cBXString(&workStr, 2);
    nm.name = name;
    int anyFound = func_003A6948(base.s, 1, &nm);
    int result = anyFound == 0 ? 0 : nm.ok;
    cBigFile__cBigFile((cBigFile*)&bf, 2);
    cBXString__cBXString(&base, 2);
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cTrackFileMan_checkLocation1);
#ifdef SKIP_ASM
struct cBXString;
struct cBigFile;
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" cBXString* func_00318630(cBXString* self, cBXString* a, const char* str);
extern "C" cBXString* func_003186D0(cBXString* self, const char* str, cBXString* b);
// PORT: the bigfile ctor forwards (path, flags) to cBigFile_open; bind the 3-arg form.
cBigFile* cBigFile_cBigFile1_17A978(cBigFile* self, const char* path, int flags) __asm__("cBigFile_cBigFile1__FP8cBigFile");
void cBigFile__cBigFile(cBigFile* self, int flags);
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int func_003A6948(const char* path, int n, void* names);
extern char str_data_worlds[];
extern char str_big[];

struct sCamEntry_17A978 {
    char name[0x40];    // 0x00
    char tag[4];        // 0x40
    char pad_44[0x48];
};

struct sCamMgr_17A978 {
    int count;                          // 0x0000
    sCamEntry_17A978 entries[0x100];    // 0x0004
};

struct sCamName_17A978 {
    char* name;
    int ok;
};

// Non-POD (user-declared copy ctor, never defined) so locals get the 16-byte slots g++ gives classes.
struct sStr_17A978 {
    char* s;
    sStr_17A978() {}
    sStr_17A978(const sStr_17A978&);
};

struct sBigFile_17A978 {
    int f0;
    int f4;
    sBigFile_17A978() {}
    sBigFile_17A978(const sBigFile_17A978&);
};

/**
 * Checks which of several track sections exist in a big file, unless all of them are already in the location table.
 *
 * If every one of the @p n names is already listed with BigFile == @p tag (strcasecmp), returns 0 without touching the
 * disc. Otherwise opens "data/worlds/<tag>.big" (cBigFile flags 0x100) and lets func_003A6948 set each name's found
 * flag from the "data/worlds/<tag>" section table; when none is found it returns 0. Names that are found but already
 * listed get their flag cleared again. Called by cTrackFileMan_scanLocations with the 22 built-in location files and
 * the big file "BAM".
 *
 * Quirk: it still returns 1 when every found name was already listed (all flags cleared).
 *
 * @param selfp  the track file manager (this, On Tour cTrackFileMan)
 * @param n  how many names to check (On Tour iNumSectionScan)
 * @param namesp  the names, each with a found flag (On Tour tWorldSectionScan: sectionName, bFound)
 * @param tag  the big file name (On Tour sBigFile)
 * @return 1 when at least one name was found in the big file, else 0
 */
extern "C" int cTrackFileMan_checkLocation1(void* selfp, int n, void* namesp, char* tag)
{
    sCamMgr_17A978* self = (sCamMgr_17A978*)selfp;
    sCamName_17A978* names = (sCamName_17A978*)namesp;
    int fail = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        int found = 0;
        for (j = 0; j < self->count; j++) {
            if (strcasecmp(names[i].name, self->entries[j].name) == 0 &&
                strcasecmp(tag, self->entries[j].tag) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            fail = 1;
            break;
        }
    }
    if (!fail) {
        return 0;
    }
    sStr_17A978 base;
    sStr_17A978 workStr;
    sBigFile_17A978 bf;
    char* pre = str_data_worlds;
    cBXString_cBXString2(&workStr, tag);
    func_003186D0((cBXString*)&base, pre, (cBXString*)&workStr);
    cBXString__cBXString(&workStr, 2);
    cBigFile* pbf = (cBigFile*)&bf;
    func_00318630((cBXString*)&workStr, (cBXString*)&base, str_big);
    cBigFile_cBigFile1_17A978(pbf, workStr.s, 0x100);
    cBXString__cBXString(&workStr, 2);
    int anyFound = func_003A6948(base.s, n, names);
    if (anyFound == 0) {
        cBigFile__cBigFile((cBigFile*)&bf, 2);
        cBXString__cBXString(&base, 2);
        return 0;
    }
    for (int k = 0; k < n; k++) {
        if (names[k].ok) {
            for (int entryIndex = 0; entryIndex < self->count; entryIndex++) {
                if (strcasecmp(names[k].name, self->entries[entryIndex].name) == 0 &&
                    strcasecmp(tag, self->entries[entryIndex].tag) == 0) {
                    names[k].ok = 0;
                    break;
                }
            }
        }
    }
    cBigFile__cBigFile((cBigFile*)&bf, 2);
    cBXString__cBXString(&base, 2);
    return anyFound;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBackgroundMan_cBackgroundMan__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty background-image manager.
 *
 * Zeroes the texture id array (+0x0) and image count (+0x4) and sets the active image (+0x8) to -1 (none).
 *
 * @return @p self
 */
void* cBackgroundMan_cBackgroundMan(void* self)
{
    int zero = 0;
    *(int*)((char*)self + 0x4) = zero;
    *(int*)((char*)self + 0x8) = -1;
    *(int*)self = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBackgroundMan__cBackgroundMan);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void cMemMan_free(void* p);

struct sVE_0017ABE0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sIdList_0017ABE0 {
    int* ids;
    int count;
};

/**
 * Destroys a background-image manager: releases its textures and the id array.
 *
 * For each of the count (+0x4) texture ids (+0x0) that is not negative, calls GrMan's virtual slot 50 (On Tour
 * RemoveTexture) with it, then frees the array with cMemMan_free and the object itself when @p flags bit 0 is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cBackgroundMan__cBackgroundMan(cBackgroundMan* self, int flags)
{
    if (self->mpiImageTID != 0)
    {
        for (int i = 0; i < self->miNumImage; i++)
        {
            int id = self->mpiImageTID[i];
            if (id >= 0)
            {
                cPS2GraphicsMan* obj = GrMan;
                sVE_0017ABE0* vt = (sVE_0017ABE0*)obj->mVtbl;
                vt[50].fn((char*)obj + vt[50].delta, id);
            }
        }
        if (self->mpiImageTID != 0)
            cMemMan_free(self->mpiImageTID);
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", cBackgroundMan_setActiveImage);
#ifdef SKIP_ASM
/**
 * Selects which background image to show.
 *
 * Sets the active image (+0x8) to @p i when 0 <= @p i < the image count (+0x4), otherwise to -1 (none). Called from
 * main/loadscreens.cpp.
 *
 * @param i  the image index
 * @return 1, always
 */
extern "C" int cBackgroundMan_setActiveImage(cBackgroundMan* self, int i)
{
    if (i >= 0 && i < self->miNumImage) {
        self->miActiveImage = i;
    } else {
        self->miActiveImage = -1;
    }
    return 1;
}
#endif

