#ifndef BX_MATH3D_H
#define BX_MATH3D_H

#include "layout.h"

// EA's vector/matrix types (names from the July 2003 preview build's mangled symbols, e.g.
// setVelocity__6cRiderRC8t4Vector; header path from its assert strings, ../source/bx/math3d.h).
// Members are our guess: 4 floats, 16-byte aligned (they are copied with lq/sq and loaded with lqc2).

struct t4Vector {
    float x, y, z, w;
} __attribute__((aligned(16)));

// EA's quaternion (On Tour: struct tQuaternion : tquat, 0x10; the preview mangles
// setOrientation__13cSimpleCameraRC11tQuaternion; On Tour cRider::EulerToQuat(tQuaternion& quat, heading, pitch, roll)).
// x, y, z, w as in t4Vector (retail cRider_EulerToQuat stores w = cos-product, x / y / z the sine terms; copied with
// lq/sq, so 16-byte aligned). [Staged by views group A.]
struct tQuaternion {
    float x, y, z, w;
} __attribute__((aligned(16)));

// EA's 3-float vector (type name from the preview's setSize__8cUIThingRC8t3Vector; [O] On Tour's t3Vector : t3vect,
// 0xC). 12 bytes, 4-byte aligned: retail copies it with ldl/ldr + lw. Member names are our guess (On Tour: x()/y()/z()
// accessors over an anonymous union).
struct t3Vector {
    float x, y, z;
};

// Row-major; for an object transform the rows are side, front, up and position.
struct t4Matrix {
    t4Vector row[4];
};

// EA's 2-float vector (8 bytes, 4-byte aligned). [P] the preview mangles it (setTileUV__7cUIIconG8t2Vector,
// __8t2Vectorff ...); [O] On Tour struct t2Vector (size 0x8, base t2vect; ctors t2Vector() and
// t2Vector(float ix, float iy)). Uses: cFont's Offset / UVScale / ShadowOffset / BaseScale / Scale ([O] t2Vector members;
// render/font.h keeps them as float[2]), the UV argument of cGraphicsMan::RenderIndexedTriList ([O] const t2Vector*).
// Both constructors are EA's: [R] cFont::cFont / cFont_cFont1 build (0, 0) and (1, 1) temporaries through the 2-float
// one. Members x, y as in the units' local copies. On Tour's t2Vector derives from t2vect, whose float& x() / y()
// accessors its 2-float ctor assigns through; X() / Y() stand in for them (the flat struct keeps the field names).
// This is src/fe/ovstatehud.cpp's local definition verbatim (matched code: onRender2D needs the accessor form).
// NOTE: src/fe/all_in_one_fe.cpp (POD {x, y}) and src/fe/ovstatehud.cpp define their own struct t2Vector and include
// this header: both must drop theirs when this one lands (the scratch compile shows the redefinition errors), and
// all_in_one_fe's 42 uses must be re-checked byte for byte (its copy has no constructors).
struct t2Vector {
    float x, y;
    float& X() { return x; }
    float& Y() { return y; }
    float X() const { return x; }   // [O] t2vect's float x() const / y() const
    float Y() const { return y; }
    t2Vector() {}
    t2Vector(float x_, float y_) { X() = x_; Y() = y_; }
};

// EA's pose type (On Tour: struct tPose {t4Vector P; tQuaternion Q;}, size 0x20; the preview mangles
// setOrientationOffset__14cRiderAnimBaseRC5tPose; cRiderAnimBase and the cameras use it).
struct tPose {
    t4Vector P;         // 0x000 [O] position
    t4Vector Q;         // 0x010 [O] rotation quaternion (tQuaternion in On Tour; x, y, z, w as in t4Vector)
};

CHECK_OFFSET(tQuaternion, x, 0x0);
CHECK_OFFSET(tQuaternion, y, 0x4);
CHECK_OFFSET(tQuaternion, z, 0x8);
CHECK_OFFSET(tQuaternion, w, 0xC);
CHECK_SIZE(tQuaternion, 0x10);

CHECK_OFFSET(t2Vector, x, 0x0);
CHECK_OFFSET(t2Vector, y, 0x4);
CHECK_SIZE(t2Vector, 0x8);

// EA's 3x3 matrix (On Tour: struct t3Matrix : t3mat, size 0x24; t3mat holds the nine floats in an anonymous union whose
// members the dump does not list). [O] cTextureElement::renderRot(tRect pos, t3Matrix mat, tARGB clr, unsigned int
// flags) and c2DRender::drawRect(tBxVertex*, const tRect&, const t3Matrix&); [R] retail cTextureElement_renderRot takes
// the nine floats by value and hands their address to c2DRender_drawRect1 / drawRotRect1 (ui/uitext's view sMat3_2A00).
// 4-byte aligned (no quadword copies seen). The member name is ours, like t3Vector's / t4Vector's.
struct t3Matrix {
    float m[9];                     // 0x00 [O] t3mat (anonymous union)
};

CHECK_SIZE(t3Matrix, 0x24);

#endif /* BX_MATH3D_H */
