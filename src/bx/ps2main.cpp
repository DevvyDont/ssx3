#include "common.h"
#include "bx/appman.h"
#include "bx/math3d.h"

//100%
INCLUDE_ASM("bx/ps2main", systemInit);
#ifdef SKIP_ASM
extern void* D_004A3E94;
extern char str_SIO2MAN_IRX[];
extern char str_PADMAN_IRX[];
extern char str_LIBSD_IRX[];
extern char str_SNDDRV_IRX[];
extern char str_MCMAN_IRX[];
extern char str_MCSERV_IRX[];
extern char str_cd[];
extern "C" void bxIop_initIop(void* io);
extern "C" void bxIop_loadModule(void* io, const char* module, int a, int b, int c);
extern "C" void sceMcInit(void);
extern "C" void scePadInit(int a);
extern "C" void sceCdReadClock(int* clock);
void BXsrand(unsigned int seed);
extern "C" void THREAD_init(void);
extern "C" void* malloc(int size);
extern "C" void free(void* p);
extern "C" char* EndOfHeap(void);
extern "C" void cMemMan_initialize(char* heap, int size);
extern "C" void TIMER_setHblankRate(int a);
extern "C" void TIMER_init(int hz);
extern "C" int FILESYS_getopts(void* cfg);
extern "C" int FILESYS_setopts(void* cfg);
extern "C" void FILESYS_init(int a, int b, int c);
extern "C" void ASYNCFILE_init(int a, int b);
extern "C" void CDVD_checkFailedRead(void);
extern "C" void cd_recoverReadError(void);
extern "C" void SYNCTASK_del(void (*fn)(void));
extern "C" void SYNCTASK_add(void (*fn)(void), int a, int b);
extern "C" void FILESYS_setcurrentpath(void* p);
extern "C" void MEM_alloc(void);
extern "C" void MEM_free(void);

struct sIoCfg_SysInit {
    int size;               // 0x0
    void (*fnA)(void);      // 0x4
    void (*fnB)(void);      // 0x8
    int mode;               // 0xC
    char pad_0x10[0x14];
};

/**
 * Boot-time setup of the PS2 side: IOP modules, random seed, heap, file system and the per-frame sync task.
 *
 * Loads SIO2MAN, PADMAN, LIBSD, SNDDRV, MCMAN and MCSERV.IRX, seeds BXsrand() from a clock reading, hands the
 * memory from just past a 64-byte-aligned scratch block (+0x800) up to EndOfHeap() to
 * cMemMan_initialize(), installs MEM_alloc/MEM_free as the file system's allocator, runs FILESYS_init(48,
 * 500, 32) and ASYNCFILE_init(16, 0), swaps sync task CDVD_checkFailedRead for cd_recoverReadError and finally passes
 * "cd:" to FILESYS_setcurrentpath() (probably selecting the default file device).
 */
// Boot-time setup: IOP modules, timers, random seed, heap, file system and the per-frame sync task.
extern "C" void systemInit(void)
{
    bxIop_initIop(D_004A3E94);
    bxIop_loadModule(D_004A3E94, str_SIO2MAN_IRX, 1, 0, 0);
    bxIop_loadModule(D_004A3E94, str_PADMAN_IRX, 1, 0, 0);
    bxIop_loadModule(D_004A3E94, str_LIBSD_IRX, 1, 0, 0);
    bxIop_loadModule(D_004A3E94, str_SNDDRV_IRX, 1, 0, 0);
    bxIop_loadModule(D_004A3E94, str_MCMAN_IRX, 1, 0, 0);
    bxIop_loadModule(D_004A3E94, str_MCSERV_IRX, 1, 0, 0);
    sceMcInit();
    scePadInit(0);
    int clock[2];
    sceCdReadClock(clock);
    BXsrand(clock[0] ^ (clock[1] << 3));
    THREAD_init();
    char* heapProbe = (char*)malloc(0x400);
    free(heapProbe);
    char* heap = (char*)(((int)heapProbe + 0x3F) & ~0x3F) + 0x800;
    cMemMan_initialize(heap, EndOfHeap() - heap);
    TIMER_setHblankRate(1);
    TIMER_init(0x3C);
    sIoCfg_SysInit cfg;
    cfg.size = 36;
    FILESYS_getopts(&cfg);
    cfg.fnA = MEM_alloc;
    cfg.fnB = MEM_free;
    cfg.mode = 2;
    FILESYS_setopts(&cfg);
    FILESYS_init(0x30, 500, 0x20);
    ASYNCFILE_init(0x10, 0);
    SYNCTASK_del(CDVD_checkFailedRead);
    SYNCTASK_add(cd_recoverReadError, 0, 0);
    FILESYS_setcurrentpath(str_cd);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/ps2main", main);
#ifdef SKIP_ASM
struct cExecutionMan;
struct cExecutionManMainK {
    int field_0x0;
    int field_0x4;
    void* field_0x8; // vtable
};
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
cExecutionMan* cExecutionMan_cExecutionMan(cExecutionMan* self);
extern "C" void __main(void); // __main
extern "C" void systemInit(void);
extern "C" int cAppMan_run(void* self, cExecutionMan* exec, int argc, char** argv);
extern const char str_PS2ExecutionMan[];
extern void* cPS2ExecutionMan_vtbl[];

// PORT: bound to "main" by asm label so gcc doesn't emit its own __main call;
// the target calls __main (__main) explicitly. Off-PS2 this is plain main().
int main_impl(int argc, char** argv) __asm__("main");

/**
 * Program entry point: initialises the system, creates the PS2 execution manager and runs the application.
 *
 * Calls __main (__main) and systemInit(), allocates the 0x142C0-byte "PS2ExecutionMan", initialises it
 * with cExecutionMan_cExecutionMan() and gives it the PS2 vtable (cPS2ExecutionMan_vtbl), then hands it to cAppMan_run() on the
 * application object (cAppMan_TheAppMan).
 *
 * @param argc  the argument count, passed to cAppMan_run()
 * @param argv  the argument strings, passed to cAppMan_run()
 * @return cAppMan_run()'s result
 */
int main_impl(int argc, char** argv)
{
    __main();
    systemInit();
    cExecutionManMainK* exec = (cExecutionManMainK*)cMemMan_alloc(0x142C0, str_PS2ExecutionMan, 0, 0);
    cExecutionMan_cExecutionMan((cExecutionMan*)exec);
    exec->field_0x8 = cPS2ExecutionMan_vtbl;
    return cAppMan_run(cAppMan_TheAppMan, (cExecutionMan*)exec, argc, argv);
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", shutdownAndLoadExec);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cAppManB008 {
public:
    char pad_0x00[0x5C];
    virtual void v01();
    virtual void loadexecpurge();
};
// Typed view of cAppMan_TheAppMan (main() in this unit declares it as void*).
extern cAppManB008* cAppMan_TheAppMan_B008 __asm__("cAppMan_TheAppMan");
extern "C" void sceCdMmode(int);
extern "C" void sceCdInit(int);
extern "C" void sceSifExitCmd(void);
extern "C" void LoadExecPS2(int, int, int);

/**
 * Shuts the game's systems down and loads another executable with LoadExecPS2.
 *
 * Calls virtual slot 2 of cAppMan_TheAppMan (cAppMan_loadexecpurge in cAppMan_vtbl, which purges the execution
 * manager), then sceCdMmode(2), sceCdInit(0) and sceCdInit(5) (sceCdInit has the shape of libcdvd's
 * sceCdInit; mode 5 shuts the CD client down), sceSifExitCmd (removes the DMAC 5 handler), and finally passes the
 * arguments to LoadExecPS2, which packs them with execArgs_copyToKernel and calls _LoadExecPS2.
 *
 * @param path  path of the executable to load
 * @param argc  argument count
 * @param argv  argument strings
 */
extern "C" void shutdownAndLoadExec(int path, int argc, int argv)
{
    cAppMan_TheAppMan_B008->loadexecpurge();
    sceCdMmode(2);
    sceCdInit(0);
    sceCdInit(5);
    sceSifExitCmd();
    LoadExecPS2(path, argc, argv);
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", det2x2);
#ifdef SKIP_ASM
/**
 * Returns the determinant of a 2x2 matrix passed row by row: first * fourth - second * third.
 *
 * @param mat00  row 0, column 0
 * @param mat01  row 0, column 1
 * @param mat10  row 1, column 0
 * @param mat11  row 1, column 1
 * @return the determinant
 */
extern "C" float det2x2(float mat00, float mat01, float mat10, float mat11)
{
    return mat00 * mat11 - mat01 * mat10;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/ps2main", det3x3);
#ifdef SKIP_ASM
extern "C" float det2x2(float a, float b, float c, float d);

/**
 * Returns the determinant of a 3x3 matrix passed row by row, by cofactor expansion along the first column.
 *
 * The three 2x2 minors come from det2x2. det4x4 and adjoint pass 3x3 minors of a 4x4 matrix
 * column by column; the determinant is the same either way.
 *
 * @param mat00  row 0, column 0
 * @param mat01  row 0, column 1
 * @param mat02  row 0, column 2
 * @param mat10  row 1, column 0
 * @param mat11  row 1, column 1
 * @param mat12  row 1, column 2
 * @param mat20  row 2, column 0
 * @param mat21  row 2, column 1
 * @param i  row 2, column 2
 * @return the determinant
 */
// 3x3 determinant by cofactor expansion along the first column.
extern "C" float det3x3(float mat00, float mat01, float mat02, float mat10, float mat11, float mat12, float mat20, float mat21, float i)
{
    return mat00 * det2x2(mat11, mat12, mat21, i) - mat10 * det2x2(mat01, mat02, mat21, i) + mat20 * det2x2(mat01, mat02, mat11, mat12);
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", det4x4);
#ifdef SKIP_ASM
extern "C" float det3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i);

/**
 * Returns the determinant of a 4x4 matrix by cofactor expansion along the first row.
 *
 * Each cofactor is a 3x3 minor from det3x3. t4mat_invert divides the adjugate by it to invert the matrix.
 *
 * @param vm  the 4x4 float matrix (row-major, 16 floats)
 * @return the determinant
 */
// 4x4 determinant by cofactor expansion along the first row.
extern "C" float det4x4(void* vm)
{
    float (*mat)[4] = (float (*)[4])vm;
    float ans;
    float mat00, mat10, mat20, mat30, mat01, mat11, mat21, mat31;
    float mat02, mat12, mat22, mat32, mat03, mat13, mat23, mat33;

    mat00 = mat[0][0]; mat01 = mat[0][1];
    mat02 = mat[0][2]; mat03 = mat[0][3];

    mat10 = mat[1][0]; mat11 = mat[1][1];
    mat12 = mat[1][2]; mat13 = mat[1][3];

    mat20 = mat[2][0]; mat21 = mat[2][1];
    mat22 = mat[2][2]; mat23 = mat[2][3];

    mat30 = mat[3][0]; mat31 = mat[3][1];
    mat32 = mat[3][2]; mat33 = mat[3][3];

    ans = mat00 * det3x3(mat11, mat21, mat31, mat12, mat22, mat32, mat13, mat23, mat33)
        - mat01 * det3x3(mat10, mat20, mat30, mat12, mat22, mat32, mat13, mat23, mat33)
        + mat02 * det3x3(mat10, mat20, mat30, mat11, mat21, mat31, mat13, mat23, mat33)
        - mat03 * det3x3(mat10, mat20, mat30, mat11, mat21, mat31, mat12, mat22, mat32);
    return ans;
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", adjoint);
#ifdef SKIP_ASM
extern "C" float det3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i);

/**
 * Replaces a 4x4 matrix with its adjugate (the transposed matrix of cofactors), in place.
 *
 * Copies all 16 elements first, then writes each element [r][c] as the signed 3x3 minor (det3x3) of element
 * [c][r]. t4mat_invert scales the result by 1/determinant to get the inverse.
 *
 * @param vm  the 4x4 float matrix (row-major, 16 floats)
 */
// 4x4 adjoint (transposed cofactor matrix), in place.
extern "C" void adjoint(void* vm)
{
    float (*mat)[4] = (float (*)[4])vm;
    float mat00, mat10, mat20, mat30, mat01, mat11, mat21, mat31;
    float mat02, mat12, mat22, mat32, mat03, mat13, mat23, mat33;

    mat00 = mat[0][0]; mat01 = mat[0][1];
    mat02 = mat[0][2]; mat03 = mat[0][3];

    mat10 = mat[1][0]; mat11 = mat[1][1];
    mat12 = mat[1][2]; mat13 = mat[1][3];

    mat20 = mat[2][0]; mat21 = mat[2][1];
    mat22 = mat[2][2]; mat23 = mat[2][3];

    mat30 = mat[3][0]; mat31 = mat[3][1];
    mat32 = mat[3][2]; mat33 = mat[3][3];

    mat[0][0] =  det3x3(mat11, mat21, mat31, mat12, mat22, mat32, mat13, mat23, mat33);
    mat[1][0] = -det3x3(mat10, mat20, mat30, mat12, mat22, mat32, mat13, mat23, mat33);
    mat[2][0] =  det3x3(mat10, mat20, mat30, mat11, mat21, mat31, mat13, mat23, mat33);
    mat[3][0] = -det3x3(mat10, mat20, mat30, mat11, mat21, mat31, mat12, mat22, mat32);

    mat[0][1] = -det3x3(mat01, mat21, mat31, mat02, mat22, mat32, mat03, mat23, mat33);
    mat[1][1] =  det3x3(mat00, mat20, mat30, mat02, mat22, mat32, mat03, mat23, mat33);
    mat[2][1] = -det3x3(mat00, mat20, mat30, mat01, mat21, mat31, mat03, mat23, mat33);
    mat[3][1] =  det3x3(mat00, mat20, mat30, mat01, mat21, mat31, mat02, mat22, mat32);

    mat[0][2] =  det3x3(mat01, mat11, mat31, mat02, mat12, mat32, mat03, mat13, mat33);
    mat[1][2] = -det3x3(mat00, mat10, mat30, mat02, mat12, mat32, mat03, mat13, mat33);
    mat[2][2] =  det3x3(mat00, mat10, mat30, mat01, mat11, mat31, mat03, mat13, mat33);
    mat[3][2] = -det3x3(mat00, mat10, mat30, mat01, mat11, mat31, mat02, mat12, mat32);

    mat[0][3] = -det3x3(mat01, mat11, mat21, mat02, mat12, mat22, mat03, mat13, mat23);
    mat[1][3] =  det3x3(mat00, mat10, mat20, mat02, mat12, mat22, mat03, mat13, mat23);
    mat[2][3] = -det3x3(mat00, mat10, mat20, mat01, mat11, mat21, mat03, mat13, mat23);
    mat[3][3] =  det3x3(mat00, mat10, mat20, mat01, mat11, mat21, mat02, mat12, mat22);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/ps2main", t4mat_invert);
#ifdef SKIP_ASM
extern "C" float det4x4(void* m);
extern "C" void adjoint(void* m);

/**
 * Inverts a 4x4 matrix in place: its adjugate (adjoint) scaled by 1 / determinant (det4x4).
 *
 * The determinant is taken before the adjugate overwrites the matrix; the scale runs on VU0 (vmulx on the four rows).
 * No check for a zero determinant.
 *
 * @param mat  the 4x4 float matrix (16-byte aligned)
 */
// Matrix inverse: adjoint (adjoint) scaled by 1/determinant (det4x4).
// PORT: PS2-only VU0 inline asm (4x4 matrix times scalar, in place).
extern "C" void t4mat_invert(void* mat)
{
    float det = det4x4(mat);
    adjoint(mat);
    float invDet = 1.0f / det;
    int scratch;
    __asm__ __volatile__(
        "mfc1      %0, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "qmtc2.ni  %0, $vf3\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulx.xyzw $vf8, $vf4, $vf3x\n"
        "vmulx.xyzw $vf9, $vf5, $vf3x\n"
        "vmulx.xyzw $vf10, $vf6, $vf3x\n"
        "vmulx.xyzw $vf11, $vf7, $vf3x\n"
        "sqc2      $vf8, 0x0(%1)\n"
        "sqc2      $vf9, 0x10(%1)\n"
        "sqc2      $vf10, 0x20(%1)\n"
        "sqc2      $vf11, 0x30(%1)\n"
        : "=&r"(scratch)
        : "r"(mat), "f"(invDet)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", tPose_tPose);
#ifdef SKIP_ASM
// A t4Vector with EA's constructors (ours has none): the pose's position, built from the matrix's row 3.
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW() {}
    t4VectorXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

// PORT: tQuaternion_tQuaternion returns the quaternion by value (hidden result pointer).
extern "C" t4VectorXYZW tQuaternion_tQuaternion_v(t4Matrix* mat) __asm__("tQuaternion_tQuaternion");

/**
 * Splits a rigid transform matrix into a pose: its translation row and its rotation quaternion.
 *
 * Copies row 3 (x, y, z, w) to @p out's position P and stores the quaternion of the matrix's rotation
 * (tQuaternion_tQuaternion) in Q.
 *
 * @param out  the pose to fill
 * @param mat  the 4x4 transform matrix
 * @return @p out
 */
// Splits a rigid transform matrix into its translation row and rotation quaternion.
extern "C" tPose* tPose_tPose(tPose* out, t4Matrix* mat)
{
    out->P = t4VectorXYZW(mat->row[3].x, mat->row[3].y, mat->row[3].z, mat->row[3].w);
    out->Q = tQuaternion_tQuaternion_v(mat);
    return out;
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", tQuaternion_tQuaternion);
#ifdef SKIP_ASM
struct sMtx_B7A8 {
    float m[4][4];
};

struct sQuat_B7A8 {
    float x, y, z, w;
};

// PORT: sqrt.s (sqrtf without errno check)
static inline float sqrtB7A8(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

/**
 * Converts the rotation part of a 4x4 matrix to a quaternion (x, y, z, w).
 *
 * With a positive trace m00 + m11 + m22: s = sqrt(trace + 1), w = s / 2, and x, y, z are the differences
 * (m12 - m21), (m20 - m02), (m01 - m10) times 0.5 / s. Otherwise picks the largest diagonal element i (with j, k the
 * next two axes mod 3), s = sqrt(m[i][i] - m[j][j] - m[k][k] + 1), component i = s / 2, w = (m[j][k] - m[k][j])
 * times 0.5 / s, and components j and k from the sums (m[i][j] + m[j][i]) and (m[i][k] + m[k][i]); the 0.5 / s
 * scale is skipped when s is 0.
 *
 * @param qu  the quaternion to fill
 * @param mat  the matrix
 * @return @p qu
 */
// Rotation matrix to quaternion (Shoemake): from the trace when positive, else from the largest diagonal term.
extern "C" sQuat_B7A8* tQuaternion_tQuaternion(sQuat_B7A8* qu, sMtx_B7A8* mat)
{
    float tr = mat->m[0][0] + mat->m[1][1] + mat->m[2][2];
    if (0.0f < tr) {
        float scale = sqrtB7A8(tr + 1.0f);
        qu->w = scale * 0.5f;
        scale = 0.5f / scale;
        qu->x = (mat->m[1][2] - mat->m[2][1]) * scale;
        qu->y = (mat->m[2][0] - mat->m[0][2]) * scale;
        qu->z = (mat->m[0][1] - mat->m[1][0]) * scale;
    } else {
        float quat[4];
        int i = 0;
        if (mat->m[0][0] < mat->m[1][1]) {
            i = 1;
        }
        if (mat->m[i][i] < mat->m[2][2]) {
            i = 2;
        }
        int j = (i + 1) % 3;
        int k = (j + 1) % 3;
        float scale = sqrtB7A8(mat->m[i][i] - mat->m[j][j] - mat->m[k][k] + 1.0f);
        quat[i] = scale * 0.5f;
        if (scale != 0.0f) {
            scale = 0.5f / scale;
        }
        quat[3] = (mat->m[j][k] - mat->m[k][j]) * scale;
        quat[j] = (mat->m[i][j] + mat->m[j][i]) * scale;
        quat[k] = (mat->m[i][k] + mat->m[k][i]) * scale;
        qu->x = quat[0];
        qu->y = quat[1];
        qu->z = quat[2];
        qu->w = quat[3];
    }
    return qu;
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", tquat_rotateto);
#ifdef SKIP_ASM

extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" float func_0031C128(float x);

// w-first quaternion (the target stores w, then x, y, z).
struct sQuat_B9B0
{
    float x, y, z, w;
    sQuat_B9B0() {}
    sQuat_B9B0(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_B9B0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (r = a x b, w = 0).
static inline void CrossTo_B9B0(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector length).
static inline float Length_B9B0(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_B9B0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v /= s, in place, via vdiv).
static inline void DivScalar_B9B0(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %0, %2\n"
        "qmtc2.ni  %0, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %1\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %1\n"
        : "=&r"(t), "+m"(v)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4Vector QuatMul_B9B0(const t4Vector& a, const sQuat_B9B0& b)
{
    t4Vector r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

static inline sQuat_B9B0 AxisAngle_B9B0(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_B9B0(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Post-multiplies a quaternion by the rotation that turns one direction onto another.
 *
 * The axis is the cross product @p fromVec x @p toVec; when its length is 0 nothing changes. Otherwise the axis is
 * normalised, the length is clamped to 0.99999, and the angle is func_0031C128 (an arcsine) of it, or pi minus that
 * when the dot product of the two vectors is negative. The axis-angle quaternion (half-angle sine and cosine from
 * fSinCos) is multiplied on the right: *quat = *quat * rotation. tquat_prerotateto is the same with the rotation
 * on the left. The two directions are expected to be unit length.
 *
 * @param quat  the quaternion to rotate, in place
 * @param fromVec  the starting direction
 * @param toVec  the target direction
 */
// Rotates the quaternion *q by the shortest-arc rotation taking direction a onto b.
extern "C" void tquat_rotateto(t4Vector* quat, t4Vector* fromVec, t4Vector* toVec)
{
    t4Vector axis;
    t4Vector cr;
    CrossTo_B9B0(cr, *fromVec, *toVec);
    axis = cr;
    float len = Length_B9B0(axis);
    if (len == 0.0f)
        return;
    DivScalar_B9B0(axis, len);
    if (len > 0.9999899864196777f)
        len = 0.9999899864196777f;
    float ang;
    if (Dot_B9B0(*fromVec, *toVec) < 0.0f)
        ang = 3.1415927410125732f - func_0031C128(len);
    else
        ang = func_0031C128(len);
    *quat = QuatMul_B9B0(*quat, AxisAngle_B9B0(axis, ang));
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", tquat_prerotateto);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* sout, float* cout, float x);
extern "C" float func_0031C128(float x);

// A t4Vector with EA's constructors (ours has none); the 4-float one stores w first (the rotation quaternion's
// order in retail).
struct t4VectorWXYZ : t4Vector {
    t4VectorWXYZ() {}
    t4VectorWXYZ(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
};

// PORT: PS2-only VU0 inline asm (r = a x b, w = 0).
static inline void CrossTo_31BB30(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_31BB30(const t4Vector& v)
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
static inline void DivEq_31BB30(t4Vector& v, float s)
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
static inline float Dot_31BB30(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4VectorWXYZ QuatMul_31BB30(const t4Vector& a, const t4Vector& b)
{
    t4VectorWXYZ r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline t4VectorWXYZ AxisAngle_31BB30(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return t4VectorWXYZ(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Pre-multiplies a quaternion by the rotation that turns one direction onto another.
 *
 * Same as tquat_rotateto (axis = @p fromVec x @p toVec, nothing when it is zero length, angle from func_0031C128 of
 * the clamped length, pi minus it for a negative dot product, half-angle sine and cosine from fSinCos), but the
 * rotation goes on the left: *quat = rotation * *quat. cRider_alignRiderAxis uses it.
 *
 * @param quat  the quaternion to rotate, in place
 * @param fromVec  the starting direction
 * @param toVec  the target direction
 */
extern "C" void tquat_prerotateto(t4Vector* quat, const t4Vector* fromVec, const t4Vector* toVec)
{
    t4VectorWXYZ axis;
    t4VectorWXYZ cr;
    CrossTo_31BB30(cr, *fromVec, *toVec);
    axis = cr;
    float len = Length_31BB30(axis);
    if (len == 0.0f)
        return;
    DivEq_31BB30(axis, len);
    if (len > 0.9999899864196777f)
        len = 0.9999899864196777f;
    float ang;
    if (Dot_31BB30(*fromVec, *toVec) < 0.0f)
        ang = 3.1415927410125732f - func_0031C128(len);
    else
        ang = func_0031C128(len);
    *quat = QuatMul_31BB30(AxisAngle_31BB30(axis, ang), *quat);
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", tquat_slerp);
#ifdef SKIP_ASM
extern "C" float func_0031C128(float x);
extern "C" float fSin(float x);

// A tQuaternion with EA's constructors (ours has none): the result, built from its four components.
struct tQuaternionXYZW : tQuaternion {
    tQuaternionXYZW() {}
    tQuaternionXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

/**
 * Returns the spherical linear interpolation between two quaternions.
 *
 * The cosine is the 4D dot product; when negative, the target and the cosine are negated so the shorter arc is
 * taken. When 1 - cosine > 0.001, omega = pi/2 - func_0031C128(cosine) (an arccosine through the arcsine) and the
 * weights are sin((1 - @p blend) * omega) / sin(omega) and sin(@p blend * omega) / sin(omega) (fSin);
 * otherwise the weights are the linear 1 - @p blend and @p blend. The result is not renormalised.
 *
 * @param fromQuat  the quaternion at blend 0
 * @param toQuat  the quaternion at blend 1
 * @param blend  the interpolation fraction, 0 to 1
 * @return the interpolated quaternion
 */
// Quaternion slerp.
extern "C" tQuaternionXYZW tquat_slerp(const tQuaternion* fromQuat, const tQuaternion* toQuat, float blend)
{
    float cosom = fromQuat->x * toQuat->x + fromQuat->y * toQuat->y + fromQuat->z * toQuat->z + fromQuat->w * toQuat->w;
    tQuaternionXYZW to;
    if (cosom < 0.0f) {
        cosom = -cosom;
        to.x = -toQuat->x;
        to.y = -toQuat->y;
        to.z = -toQuat->z;
        to.w = -toQuat->w;
    } else {
        to.x = toQuat->x;
        to.y = toQuat->y;
        to.z = toQuat->z;
        to.w = toQuat->w;
    }
    float fromWeight, toWeight;
    if (1.0f - cosom > 0.0010000000474974513f) {
        float omega = 1.5707963705062866f - func_0031C128(cosom);
        float sinom = fSin(omega);
        fromWeight = fSin((1.0f - blend) * omega) / sinom;
        toWeight = fSin(blend * omega) / sinom;
    } else {
        fromWeight = 1.0f - blend;
        toWeight = blend;
    }
    tQuaternionXYZW result(fromWeight * fromQuat->x + toWeight * to.x, fromWeight * fromQuat->y + toWeight * to.y,
                           fromWeight * fromQuat->z + toWeight * to.z, fromWeight * fromQuat->w + toWeight * to.w);
    return result;
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", fSinCos);
#ifdef SKIP_ASM
// How many quarter turns (pi/2) the reduced angle is short of x, modulo a full turn (names ours): sin and cos of
// x = r + k * pi/2 are those of r swapped and negated by k.
enum eQuarterTurns { kNoTurn = 0, kQuarterTurn = 1, kHalfTurn = 2, kThreeQuarterTurns = 3 };

/**
 * Computes the sine and cosine of an angle.
 *
 * Reduces @p x by the nearest multiple of pi/2 (the quadrant, rounded half away from zero), takes the sine of the
 * remainder from a degree-9 odd polynomial and the cosine as sqrt(1 - sine^2), then swaps and negates them by
 * quadrant & 3.
 *
 * @param sout  receives sin(@p x)
 * @param cout  receives cos(@p x)
 * @param x  the angle in radians
 */
// sincos(x): quadrant reduction by pi/2, sin polynomial, cos = sqrt(1 - sin^2).
extern "C" void fSinCos(float* sout, float* cout, float x)
{
    float quadrantF = x * 0.6366197466850281f;
    if (x < 0.0f)
        quadrantF -= 0.5f;
    else
        quadrantF += 0.5f;
    int quadrant;
    float fq;
    // PORT: PS2 float->int->float round trip kept in the FPU (cvt.w.s / mfc1 / cvt.s.w).
    __asm__("cvt.w.s %0, %0\n\tmfc1 %1, %0\n\tcvt.s.w %0, %0" : "+f"(quadrantF), "=r"(quadrant));
    fq = quadrantF;
    x = x - fq * 1.5707963705062866f;
    float x2 = x * x;
    float sinPart;
    sinPart = x2 * 2.755732339210226e-06f;
    sinPart = (sinPart + -0.0001984127302421257f) * x2;
    sinPart = (sinPart + 0.008333333767950535f) * x2;
    sinPart = (sinPart + -0.1666666716337204f) * x2;
    sinPart = (sinPart + 1.0f) * x;
    float cosSq = 1.0f - sinPart * sinPart;
    float cosPart;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(cosPart) : "f"(cosSq));
    switch (quadrant & 3)
    {
    case kNoTurn:
        *sout = sinPart;
        *cout = cosPart;
        break;
    case kQuarterTurn:
        *sout = cosPart;
        *cout = -sinPart;
        break;
    case kHalfTurn:
        *sout = -sinPart;
        *cout = -cosPart;
        break;
    case kThreeQuarterTurns:
        *sout = -cosPart;
        *cout = sinPart;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", fSin);
#ifdef SKIP_ASM
/**
 * Returns the sine of an angle.
 *
 * Reduces @p x by the nearest multiple of pi/2 (rounded half away from zero), evaluates the cosine polynomial for an
 * odd quadrant or the sine polynomial for an even one, and negates the result when quadrant bit 1 is set.
 *
 * @param x  the angle in radians
 * @return sin(@p x)
 */
// sin(x): quadrant reduction by pi/2, then a cos or sin polynomial (Horner form).
extern "C" float fSin(float x)
{
    float quadrantF = x * 0.6366197466850281f;
    if (x < 0.0f)
        quadrantF -= 0.5f;
    else
        quadrantF += 0.5f;
    int quadrant;
    float fq;
    // PORT: PS2 float->int->float round trip kept in the FPU (cvt.w.s / mfc1 / cvt.s.w).
    __asm__("cvt.w.s %0, %0\n\tmfc1 %1, %0\n\tcvt.s.w %0, %0" : "+f"(quadrantF), "=r"(quadrant));
    fq = quadrantF;
    x = x - fq * 1.5707963705062866f;
    float x2 = x * x;
    float result;
    if (quadrant & 1)
    {
        result = x2 * 2.480159128026571e-05f;
        result = (result + -0.0013888890389353037f) * x2;
        result = (result + 0.0416666679084301f) * x2;
        result = (result + -0.5f) * x2;
        result = result + 1.0f;
    }
    else
    {
        result = x2 * 2.755732339210226e-06f;
        result = (result + -0.0001984127302421257f) * x2;
        result = (result + 0.008333333767950535f) * x2;
        result = (result + -0.1666666716337204f) * x2;
        result = result * x + x;
    }
    if (quadrant & 2)
        result = -result;
    return result;
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", fCos);
#ifdef SKIP_ASM
/**
 * Returns the cosine of an angle.
 *
 * Same reduction as fSin, with the polynomials swapped: the sine polynomial for an odd quadrant and the cosine
 * polynomial for an even one; negated when (quadrant + 1) & 2.
 *
 * @param x  the angle in radians
 * @return cos(@p x)
 */
// cos(x): quadrant reduction by pi/2, then a sin or cos polynomial (Horner form).
extern "C" float fCos(float x)
{
    float quadrantF = x * 0.6366197466850281f;
    if (x < 0.0f)
        quadrantF -= 0.5f;
    else
        quadrantF += 0.5f;
    int quadrant;
    float fq;
    // PORT: PS2 float->int->float round trip kept in the FPU (cvt.w.s / mfc1 / cvt.s.w).
    __asm__("cvt.w.s %0, %0\n\tmfc1 %1, %0\n\tcvt.s.w %0, %0" : "+f"(quadrantF), "=r"(quadrant));
    fq = quadrantF;
    x = x - fq * 1.5707963705062866f;
    float x2 = x * x;
    float result;
    if (quadrant & 1)
    {
        result = x2 * 2.755732339210226e-06f;
        result = (result + -0.0001984127302421257f) * x2;
        result = (result + 0.008333333767950535f) * x2;
        result = (result + -0.1666666716337204f) * x2;
        result = result * x + x;
    }
    else
    {
        result = x2 * 2.480159128026571e-05f;
        result = (result + -0.0013888890389353037f) * x2;
        result = (result + 0.0416666679084301f) * x2;
        result = (result + -0.5f) * x2;
        result = result + 1.0f;
    }
    if ((quadrant + 1) & 2)
        result = -result;
    return result;
}
#endif

INCLUDE_ASM("bx/ps2main", func_0031C128);

INCLUDE_ASM("bx/ps2main", func_0031C228);

