#ifndef CAMERA_TRIGGER_CAMERATRIGGERVOLUMES_H
#define CAMERA_TRIGGER_CAMERATRIGGERVOLUMES_H

// Camera-trigger shapes (EA: d:/ssx/code/source/camera/trigger/cameratriggervolumes.cpp, from the July 2003 preview's
// assert strings). Every trigger shape is a cCTObject: a position, heading/pitch/roll and a scale, with a vtable whose
// slots 1-5 are the cCTObject accessors pos / heading / pitch / roll / scale (all inline in EA's headers; retail keeps
// out-of-line copies cCTObject_pos__FPv .. cCTObject_scale__FPv at 0x176440-0x176460).
//   cCTObject (0x28)                 Pos, Heading, Pitch, Roll, Scale; vptr at 0x24
//    +- cCTVolume (0x2C)             + Type (eCTVolumeType); vtable D_0045C3A0 (slots 6-7 __pure_virtual)
//    |   +- cCTVolumeEllipse (0x2C)  "Camera Sphere Trigger", Type 0 (eCTVolumeEllipse); vtable cCTVolumeEllipse_vtbl
//    |   +- cCTVolumeBox (0x2C)      "Camera Box Trigger", Type 1 (not in this header)
//    +- cCTBoundObj (0x2C) and its Ellipse/Box/Line/Point subclasses, cCTSpline (0x58) (not in this header)
// Retail vtables (builds/x/vtables/vtables.tsv): cCTObject_vtbl 0x0045C3F0 (5 slots, stored first by every shape ctor),
// cCTVolumeEllipse_vtbl 0x0045C350 (8 slots: the five accessors, 6 boundBox cCTVolumeEllipse_boundBox, 7 containsPoint
// cCTVolumeEllipse_containsPoint, 8 cCTVolume_getBoundaryPointClosestTo).
// Retail code: the camera/trigger/cameratriggerfactory segment (get_camvolume 0x172E18 builds the ellipse inline,
// get_cCTVolumeEllipse 0x172FE0 reads it; boundBox 0x1721C0, containsPoint 0x172278; ctor copy 0x1764E8, uncalled).
//
// FLAT layouts, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Member names, types and order are On Tour's (offsets equal retail's). Evidence tags:
//   [R] retail ctors: get_camvolume's inline construction (cMemMan_alloc(0x2C, "Camera Sphere Trigger")) and the
//       ctor copy cCTVolumeEllipse_cCTVolumeEllipse;
//   [P] not used: the preview's out-of-line pos/heading/pitch/roll/scale__9cCTObject return `this + N` (no load or
//       store), so tools/layout_xlate.py observes no offset for these classes;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cCTObject 0x28, cCTVolume 0x2C, cCTVolumeEllipse 0x2C).

#include "layout.h"
#include "ui/uithing.h"     // t3Vector

// cCTObject (On Tour: 0x28): the base of every camera-trigger shape. Never allocated on its own; 0x28 = its vptr
// (+0x24) + 4, and every derived class's first own member is at 0x28 ([R] cCTVolume/cCTBoundObj Type; [O] cCTSpline).
struct cCTObject {
    t3Vector Pos;               // 0x00 [O]; [R] = t3vect_Zero (ldl/ldr + lw, sdl/sdr + sw); [U] cCTObject_pos returns
                                //      this; the shapes' tests read .x/.y/.z (lwc1)
    float Heading;              // 0x0C [O]; [R] 0 (sw $0); [U] cCTObject_heading returns &Heading
    float Pitch;                // 0x10 [O]; [R] 0 (sw $0); [U] cCTObject_pitch returns &Pitch
    float Roll;                 // 0x14 [O]; [R] 0 (sw $0); [U] cCTObject_roll returns &Roll
    t3Vector Scale;             // 0x18 [O]; [R] (1, 1, 1) (sdl/sdr + sw); [U] cCTObject_scale returns &Scale
    void** mVtbl;               // 0x24 [R] cCTObject_vtbl, then the derived vtable (both sw); [U] get_cCTVolumeEllipse
                                //      calls slots 1-5 through it (lw +0x24)

    // Methods bound to their retail symbols by asm labels.
    void* pos() __asm__("cCTObject_pos__FPv");
    void* heading() __asm__("cCTObject_heading__FPv");
    void* pitch() __asm__("cCTObject_pitch__FPv");
    void* roll() __asm__("cCTObject_roll__FPv");
    void* scale() __asm__("cCTObject_scale__FPv");
};

CHECK_OFFSET(cCTObject, Pos, 0x00);
CHECK_OFFSET(cCTObject, Heading, 0x0C);
CHECK_OFFSET(cCTObject, Pitch, 0x10);
CHECK_OFFSET(cCTObject, Roll, 0x14);
CHECK_OFFSET(cCTObject, Scale, 0x18);
CHECK_OFFSET(cCTObject, mVtbl, 0x24);
CHECK_SIZE(cCTObject, 0x28);

// cCTVolume (retail and On Tour 0x2C): base cCTObject. The abstract trigger volume: a cCTObject plus its shape type.
// cCTVolume::Type. [O] On Tour's enum eCTVolumeType, names and values; [R] get_camvolume builds a "Camera Sphere
// Trigger" for 0 and a "Camera Box Trigger" for 1.
enum eCTVolumeType { eCTVolumeEllipse = 0, eCTVolumeBox = 1, eCTVolumeNum = 2 };
// cCTBoundObj::Type. [O] On Tour's enum eCTBoundObjType, names and values; [R] get_camboundobj builds a "Camera
// Ellipse BoundObj", "Camera Box BoundObj", "Camera Line BoundObj" or "Camera Point BoundObj" for 0..3.
enum eCTBoundObjType {
    eCTBoundObjEllipse = 0, eCTBoundObjBox = 1, eCTBoundObjLine = 2, eCTBoundObjPoint = 3, eCTBoundObjNum = 4
};

// Vtable D_0045C3A0 (slots 6 boundBox and 7 containsPoint __pure_virtual, slot 8 cCTVolume_getBoundaryPointClosestTo).
// [R] cCTVolume_cCTVolume (uncalled ctor copy): the cCTObject defaults, cCTObject_vtbl then D_0045C3A0; it leaves Type
// unset. [Staged by views group A.]
struct cCTVolume {
    // --- cCTObject (0x28)
    t3Vector Pos;               // 0x00 [O]; [R] = t3vect_Zero
    float Heading;              // 0x0C [O]; [R] 0
    float Pitch;                // 0x10 [O]; [R] 0
    float Roll;                 // 0x14 [O]; [R] 0
    t3Vector Scale;             // 0x18 [O]; [R] (1, 1, 1)
    void** mVtbl;               // 0x24 [R] cCTObject_vtbl, then D_0045C3A0 (cCTVolume_cCTVolume)
    // --- cCTVolume
    int Type;                   // 0x28 [O] eCTVolumeType; [R] the subclass ctors store it (ellipse 0, box 1)
};

CHECK_OFFSET(cCTVolume, Pos, 0x00);
CHECK_OFFSET(cCTVolume, Heading, 0x0C);
CHECK_OFFSET(cCTVolume, Pitch, 0x10);
CHECK_OFFSET(cCTVolume, Roll, 0x14);
CHECK_OFFSET(cCTVolume, Scale, 0x18);
CHECK_OFFSET(cCTVolume, mVtbl, 0x24);
CHECK_OFFSET(cCTVolume, Type, 0x28);
CHECK_SIZE(cCTVolume, 0x2C);

// cCTVolumeEllipse (retail and On Tour 0x2C): base cCTVolume <- cCTObject. An ellipsoid trigger volume: the unit
// sphere scaled by Scale, turned by Heading/Roll/Pitch and moved to Pos. No members of its own.
// [R] get_camvolume case 0: cMemMan_alloc(0x2C, "Camera Sphere Trigger"), the cCTObject defaults,
// cCTVolumeEllipse_vtbl and Type 0.
struct t4Vector;

struct cCTVolumeEllipse {
    // --- cCTObject (0x28)
    t3Vector Pos;               // 0x00 [O]; [R] = t3vect_Zero; [U] get_cCTVolumeEllipse reads it through slot 1;
                                //      boundBox: Pos -/+ the largest scale component (lwc1 0x0/0x4/0x8);
                                //      containsPoint translates by -Pos (lwc1)
    float Heading;              // 0x0C [O]; [R] 0; [U] get (slot 2); containsPoint rotates about Z by -Heading (lwc1)
    float Pitch;                // 0x10 [O]; [R] 0; [U] get (slot 3); containsPoint rotates about X by -Pitch (lwc1)
    float Roll;                 // 0x14 [O]; [R] 0; [U] get (slot 4); containsPoint rotates about Y by -Roll (lwc1)
    t3Vector Scale;             // 0x18 [O]; [R] (1, 1, 1); [U] get (slot 5); boundBox passes &Scale to GetMaxElement;
                                //      containsPoint scales by 1 / Scale.x, .y, .z (lwc1 0x18/0x1C/0x20)
    void** mVtbl;               // 0x24 [R] cCTObject_vtbl, then cCTVolumeEllipse_vtbl (sw); [U] get_cCTVolumeEllipse (lw)
    // --- cCTVolume (0x2C)
    int Type;                   // 0x28 [O] eCTVolumeType; [R] 0 = eCTVolumeEllipse (sw $0); [U] get_cCTVolumeEllipse
                                //      stores 0 again

    // Methods bound to their retail symbols by asm labels.
    bool containsPoint(t4Vector* point) __asm__("cCTVolumeEllipse_containsPoint");
};

CHECK_OFFSET(cCTVolumeEllipse, Pos, 0x00);
CHECK_OFFSET(cCTVolumeEllipse, Heading, 0x0C);
CHECK_OFFSET(cCTVolumeEllipse, Pitch, 0x10);
CHECK_OFFSET(cCTVolumeEllipse, Roll, 0x14);
CHECK_OFFSET(cCTVolumeEllipse, Scale, 0x18);
CHECK_OFFSET(cCTVolumeEllipse, mVtbl, 0x24);
CHECK_OFFSET(cCTVolumeEllipse, Type, 0x28);
CHECK_SIZE(cCTVolumeEllipse, 0x2C);

// cCTBoundObj (retail and On Tour 0x2C): base cCTObject. The shape a bounded trigger camera (cCTActionBoundedCam::
// BoundObj) is kept inside. [R] get_camboundobj (camera/trigger/cameratriggerfactory) allocates each kind with its tag
// ("Camera Ellipse BoundObj" 0x2C Type 0, "Camera Box BoundObj" 0x2C Type 1, "Camera Line BoundObj" 0x44,
// "Camera Point BoundObj"), the cCTObject defaults and cCTObject_vtbl then the kind's vtable (cCTBoundObjEllipse_vtbl,
// cCTBoundObjBox_vtbl, cCTBoundObjLine_vtbl); [U] createChaseAlgorithmBlend tests Type == 2 for a line.
struct cCTBoundObj {
    // --- cCTObject (0x28)
    t3Vector Pos;               // 0x00 [O]; [R] = t3vect_Zero (get_camboundobj); [U] limitPoint / worldPoint translate by it
    float Heading;              // 0x0C [O]; [R] 0; [U] worldPoint rotates by it
    float Pitch;                // 0x10 [O]; [R] 0; [U] worldPoint rotates by it
    float Roll;                 // 0x14 [O]; [R] 0; [U] worldPoint rotates by it
    t3Vector Scale;             // 0x18 [O]; [R] (1, 1, 1); [U] worldPoint scales by it
    void** mVtbl;               // 0x24 [R] cCTObject_vtbl, then the derived vtable
    // --- cCTBoundObj
    int Type;                   // 0x28 [O] eCTBoundObjType; [R] get_camboundobj stores the kind (ellipse 0, box 1);
                                //      [U] createChaseAlgorithmBlend: 2 = line
};

CHECK_OFFSET(cCTBoundObj, Pos, 0x00);
CHECK_OFFSET(cCTBoundObj, Heading, 0x0C);
CHECK_OFFSET(cCTBoundObj, Pitch, 0x10);
CHECK_OFFSET(cCTBoundObj, Roll, 0x14);
CHECK_OFFSET(cCTBoundObj, Scale, 0x18);
CHECK_OFFSET(cCTBoundObj, mVtbl, 0x24);
CHECK_OFFSET(cCTBoundObj, Type, 0x28);
CHECK_SIZE(cCTBoundObj, 0x2C);

// cCTBoundObjEllipse (retail and On Tour 0x2C): base cCTBoundObj <- cCTObject; no members of its own.
// [R] get_camboundobj: cMemMan_alloc(0x2C, "Camera Ellipse BoundObj"), Type 0; [O] limitPoint(cCTBoundObjEllipse*,
// t4Vector&) = retail cCTBoundObjEllipse_limitPoint (reads Pos, Heading/Pitch/Roll, Scale).
struct cCTBoundObjEllipse {
    // --- cCTObject (0x28)
    t3Vector Pos;               // 0x00 [O]; [R] = t3vect_Zero (get_camboundobj); [U] limitPoint / worldPoint translate by it
    float Heading;              // 0x0C [O]; [R] 0; [U] worldPoint rotates by it
    float Pitch;                // 0x10 [O]; [R] 0; [U] worldPoint rotates by it
    float Roll;                 // 0x14 [O]; [R] 0; [U] worldPoint rotates by it
    t3Vector Scale;             // 0x18 [O]; [R] (1, 1, 1); [U] worldPoint scales by it
    void** mVtbl;               // 0x24 [R] cCTObject_vtbl, then the derived vtable
    // --- cCTBoundObj (0x2C)
    int Type;                   // 0x28 [O] eCTBoundObjType; [R] 0 (sw $0)

    // Methods bound to their retail symbols by asm labels.
    void limitPoint(t4Vector* point) __asm__("cCTBoundObjEllipse_limitPoint");
};

CHECK_OFFSET(cCTBoundObjEllipse, Pos, 0x00);
CHECK_OFFSET(cCTBoundObjEllipse, Heading, 0x0C);
CHECK_OFFSET(cCTBoundObjEllipse, Pitch, 0x10);
CHECK_OFFSET(cCTBoundObjEllipse, Roll, 0x14);
CHECK_OFFSET(cCTBoundObjEllipse, Scale, 0x18);
CHECK_OFFSET(cCTBoundObjEllipse, mVtbl, 0x24);
CHECK_OFFSET(cCTBoundObjEllipse, Type, 0x28);
CHECK_SIZE(cCTBoundObjEllipse, 0x2C);

// cCTBoundObjLine (retail and On Tour 0x44): base cCTBoundObj <- cCTObject. A segment between two local points.
// [R] get_camboundobj: cMemMan_alloc(0x44, "Camera Line BoundObj"); [O] worldPoint1 / worldPoint2 / limitPoint
// (cCTBoundObjLine*) = retail cCTBoundObjLine_worldPoint1 / _worldPoint2 / _limitPoint (Point[0] at 0x2C, Point[1] at 0x38).
struct cCTBoundObjLine {
    // --- cCTObject (0x28)
    t3Vector Pos;               // 0x00 [O]; [R] = t3vect_Zero (get_camboundobj); [U] limitPoint / worldPoint translate by it
    float Heading;              // 0x0C [O]; [R] 0; [U] worldPoint rotates by it
    float Pitch;                // 0x10 [O]; [R] 0; [U] worldPoint rotates by it
    float Roll;                 // 0x14 [O]; [R] 0; [U] worldPoint rotates by it
    t3Vector Scale;             // 0x18 [O]; [R] (1, 1, 1); [U] worldPoint scales by it
    void** mVtbl;               // 0x24 [R] cCTObject_vtbl, then the derived vtable
    // --- cCTBoundObj (0x2C)
    int Type;                   // 0x28 [O] eCTBoundObjType; [U] createChaseAlgorithmBlend: 2 = line
    // --- cCTBoundObjLine
    t3Vector Point[2];          // 0x2C [O]; [U] worldPoint1 transforms Point[0] (lwc1 0x2C/0x30/0x34), worldPoint2
                                //      Point[1] (0x38/0x3C/0x40)
};

CHECK_OFFSET(cCTBoundObjLine, Pos, 0x00);
CHECK_OFFSET(cCTBoundObjLine, Heading, 0x0C);
CHECK_OFFSET(cCTBoundObjLine, Pitch, 0x10);
CHECK_OFFSET(cCTBoundObjLine, Roll, 0x14);
CHECK_OFFSET(cCTBoundObjLine, Scale, 0x18);
CHECK_OFFSET(cCTBoundObjLine, mVtbl, 0x24);
CHECK_OFFSET(cCTBoundObjLine, Type, 0x28);
CHECK_OFFSET(cCTBoundObjLine, Point, 0x2C);
CHECK_SIZE(cCTBoundObjLine, 0x44);

// cCTSpline (retail and On Tour 0x58): base cCTObject. The 4 control points of a spline trigger camera
// (cCTActionSpline::Spline). [R] get_camspline: cMemMan_alloc(0x58, ...); [O] worldPoint(cCTSpline*, int i) = retail
// cCTSpline_worldPoint (transforms Point[i]).
struct cCTSpline {
    // --- cCTObject (0x28)
    t3Vector Pos;               // 0x00 [O]; [R] = t3vect_Zero (get_camboundobj); [U] limitPoint / worldPoint translate by it
    float Heading;              // 0x0C [O]; [R] 0; [U] worldPoint rotates by it
    float Pitch;                // 0x10 [O]; [R] 0; [U] worldPoint rotates by it
    float Roll;                 // 0x14 [O]; [R] 0; [U] worldPoint rotates by it
    t3Vector Scale;             // 0x18 [O]; [R] (1, 1, 1); [U] worldPoint scales by it
    void** mVtbl;               // 0x24 [R] cCTObject_vtbl, then the derived vtable
    // --- cCTSpline
    t3Vector Point[4];          // 0x28 [O]; [U] cCTSpline_worldPoint reads Point[i] (createChaseAlgorithmBlend asks for 0..3)
};

CHECK_OFFSET(cCTSpline, Pos, 0x00);
CHECK_OFFSET(cCTSpline, Heading, 0x0C);
CHECK_OFFSET(cCTSpline, Pitch, 0x10);
CHECK_OFFSET(cCTSpline, Roll, 0x14);
CHECK_OFFSET(cCTSpline, Scale, 0x18);
CHECK_OFFSET(cCTSpline, mVtbl, 0x24);
CHECK_OFFSET(cCTSpline, Point, 0x28);
CHECK_SIZE(cCTSpline, 0x58);

#endif /* CAMERA_TRIGGER_CAMERATRIGGERVOLUMES_H */
