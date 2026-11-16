#ifndef BX_MATH3D_H
#define BX_MATH3D_H

// EA's vector/matrix types (names from the July 2003 preview build's mangled symbols, e.g.
// setVelocity__6cRiderRC8t4Vector; header path from its assert strings, ../source/bx/math3d.h).
// Members are our guess: 4 floats, 16-byte aligned (they are copied with lq/sq and loaded with lqc2).

struct t4Vector {
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

#endif /* BX_MATH3D_H */
