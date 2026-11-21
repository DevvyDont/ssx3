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

// Row-major; for an object transform the rows are side, front, up and position.
struct t4Matrix {
    t4Vector row[4];
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

#endif /* BX_MATH3D_H */
