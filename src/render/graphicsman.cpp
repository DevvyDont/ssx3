#include "vtable.h"
#include "common.h"
#include "render/bxps2tex.h"
#include "visualfx/transitionman.h"
#include "world/instancerecord.h"
#include "render/2drender.h"
#include "bx/appman.h"
#include "bx/execman.h"
#include "bx/math3d.h"
#include "render/graphicsman.h"
#include "ui/uithing.h"
#include "world/worldview.h"
#include "render/particle.h"
#include "render/ps2graphicsman.h"
#include "replay/bxreplayframe.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_AddBlendedMatrix);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_BlendedMatrixHistory[];

extern "C" bool areBlendedMatricesEqual(void* a, void* b, int tol0, int tol1, int tol2);

/**
 * Adds a skinning blend matrix to the shared history, or reuses a near-identical one already there.
 *
 * The 512-entry "BlendedMatrixHistory" is allocated on first use: 512 records, then 512 x 3 components, each record's
 * m_matrixList pointing at its three. A match within areBlendedMatricesEqual's tolerances copies the stored weights
 * into @p blendedMatrix; otherwise it is appended, with no check against the 512 entries.
 *
 * @param blendedMatrix  the bone weights to add
 */
void cGraphicsMan::AddBlendedMatrix(tMdfBlendedMatrix* blendedMatrix)
{
    if (m_blendedMatrixHistory == 0) {
        tMdfBlendedMatrix* mem = (tMdfBlendedMatrix*)operator_new_tag(0x3000, str_BlendedMatrixHistory, 0x100, 0);
        m_blendedMatrixHistory = mem;
        // the components follow the 512 records in the same block
        tMdfBlendedMatrixComponent* w = (tMdfBlendedMatrixComponent*)&mem[0x200];
        for (int k = 0; k < 0x200; k++) {
            m_blendedMatrixHistory[k].m_matrixList = w;
            w += 3;
        }
    }
    tMdfBlendedMatrix* existing = m_blendedMatrixHistory;
    for (int i = 0; i < m_blendedMatrixHistorySize; i++, existing++) {
        if (areBlendedMatricesEqual(existing, blendedMatrix, 10, 20, 20)) {
            for (int j = 0; j < existing->m_iNumMatrices; j++) {
                blendedMatrix->m_matrixList[j].m_iWeight = existing->m_matrixList[j].m_iWeight;
            }
            return;
        }
    }
    m_blendedMatrixHistory[m_blendedMatrixHistorySize].m_iNumMatrices = blendedMatrix->m_iNumMatrices;
    for (int j = 0; j < blendedMatrix->m_iNumMatrices; j++) {
        m_blendedMatrixHistory[m_blendedMatrixHistorySize].m_matrixList[j] = blendedMatrix->m_matrixList[j];
    }
    m_blendedMatrixHistorySize++;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", areBlendedMatricesEqual);
#ifdef SKIP_ASM
// A component's matrix id and part id as one halfword: retail compares the two bytes with a single lhu.
static inline unsigned short componentKey(const tMdfBlendedMatrixComponent* c)
{
    return *(const unsigned short*)&c->m_iMatrixId;
}

static inline int iabs_9A78(int v)
{
    return v < 0 ? -v : v;
}

/**
 * Returns 1 when two blended matrices name the same matrices with weights within the given tolerances, else 0.
 *
 * Each argument points at an 0xC-byte record laid out like On Tour's tMdfBlendedMatrix: a component count at +0 and a
 * pointer at +4 to up to three 4-byte components (a short weight, then the matrix id). The counts and the first ids
 * must be equal. Count 1 matches at once when the first weights are equal; count 2 when the first weights, second ids
 * and second weights are equal; counts other than 1 to 3 never match.
 *
 * Otherwise (count 3, or an inexact count 1 or 2) it compares second and third components: with the ids in the same
 * order every weight difference must be within @p tol0, @p tol1, @p tol2; with the second and third ids swapped the
 * weights are compared crosswise. Possible bug: an inexact count 1 or 2 falls into this test and reads components such
 * a record does not have (the third, and for count 1 also the second).
 *
 * Callers: cGraphicsMan_AddBlendedMatrix (10, 20, 20) and cPS2GraphicsMan_ResolveCharModel (0, 0, 0).
 *
 * @param ap  the first blended-matrix record
 * @param bp  the second blended-matrix record
 * @param tol0  largest allowed difference of the first weights
 * @param tol1  largest allowed difference of the second weights
 * @param tol2  largest allowed difference of the third weights
 * @return true when the two records are treated as the same blended matrix, else false
 */
extern "C" bool areBlendedMatricesEqual(void* ap, void* bp, int tol0, int tol1, int tol2)
{
    tMdfBlendedMatrix* lhs = (tMdfBlendedMatrix*)ap;
    tMdfBlendedMatrix* rhs = (tMdfBlendedMatrix*)bp;
    if (lhs->m_iNumMatrices != rhs->m_iNumMatrices)
        goto fail;
    if (componentKey(&lhs->m_matrixList[0]) != componentKey(&rhs->m_matrixList[0]))
        goto fail;
    switch (lhs->m_iNumMatrices) {
    case 1:
        if (lhs->m_matrixList[0].m_iWeight == rhs->m_matrixList[0].m_iWeight)
            return true;
        break;
    case 2:
        if (lhs->m_matrixList[0].m_iWeight == rhs->m_matrixList[0].m_iWeight
            && componentKey(&lhs->m_matrixList[1]) == componentKey(&rhs->m_matrixList[1])
            && lhs->m_matrixList[1].m_iWeight == rhs->m_matrixList[1].m_iWeight)
            return true;
        break;
    case 3:
        break;
    default:
        goto fail;
    }
    if (componentKey(&lhs->m_matrixList[1]) == componentKey(&rhs->m_matrixList[1])
        && componentKey(&lhs->m_matrixList[2]) == componentKey(&rhs->m_matrixList[2])) {
        if (iabs_9A78(lhs->m_matrixList[0].m_iWeight - rhs->m_matrixList[0].m_iWeight) > tol0
            || iabs_9A78(lhs->m_matrixList[1].m_iWeight - rhs->m_matrixList[1].m_iWeight) > tol1
            || iabs_9A78(lhs->m_matrixList[2].m_iWeight - rhs->m_matrixList[2].m_iWeight) > tol2)
            goto fail;
        return true;
    }
    if (componentKey(&lhs->m_matrixList[2]) == componentKey(&rhs->m_matrixList[1])
        && componentKey(&lhs->m_matrixList[1]) == componentKey(&rhs->m_matrixList[2])) {
        if (iabs_9A78(lhs->m_matrixList[0].m_iWeight - rhs->m_matrixList[0].m_iWeight) <= tol0
            && iabs_9A78(lhs->m_matrixList[2].m_iWeight - rhs->m_matrixList[1].m_iWeight) <= tol1
            && iabs_9A78(lhs->m_matrixList[1].m_iWeight - rhs->m_matrixList[2].m_iWeight) <= tol2)
            return true;
    }
fail:
    return false;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_FlushRenderer);
#ifdef SKIP_ASM
extern "C" void SYNCTASK_run(int a);
extern "C" void THREAD_yield(int a);


// cExecutionMan's vtable slots 11 / 12 (prepause / postpause; [R] cExecutionMan_vtbl, cPS2ExecutionMan_vtbl).
typedef sVEntry<void(void*)> tExecPauseSlot;

/**
 * Spins until the renderer is idle: while virtual slot 18 returns 0, runs the app's execution manager's slot 11
 * (prepause), SYNCTASK_run(0), THREAD_yield(0) and its slot 12 (postpause).
 */
void cGraphicsMan::FlushRenderer()
{
    for (;;) {
        if (IsRenderIdle() != 0) break;
        cExecutionMan* exec = cAppMan_TheAppMan->ExecutionMan;
        tExecPauseSlot* vt = (tExecPauseSlot*)exec->mVtbl;
        vt[11].fn(vthis(exec, vt[11]));
        SYNCTASK_run(0);
        THREAD_yield(0);
        cExecutionMan* execAfter = cAppMan_TheAppMan->ExecutionMan;
        tExecPauseSlot* vtAfter = (tExecPauseSlot*)execAfter->mVtbl;
        vtAfter[12].fn(vthis(execAfter, vtAfter[12]));
    }
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_RenderModel);
#ifdef SKIP_ASM
/**
 * Draws a model instance through vtable slot 95 (cPS2GraphicsMan_RenderModel) with a scale of 1.0.
 *
 * Passes @p obj, the word at @p obj + 0x94, the address @p obj + 0x10, then @p animMats, @p texMat, @p renderFlags and 1.0f. Slot 96 of
 * both cGraphicsMan_vtbl and cPS2GraphicsMan_vtbl.
 *
 * @param obj  the instance record handed to slot 95
 * @param animMats  passed through to slot 95 (animation matrices)
 * @param texMat  passed through to slot 95 (texture matrix)
 * @param renderFlags  passed through to slot 95 (render flags)
 */
void cGraphicsMan::RenderModel(tInstanceRecord* obj, int animMats, int texMat, int renderFlags)
{
    RenderModel(obj, obj->m_MaterialBlock, *(const t4Matrix*)&obj->m_fmTransform, 1.0f, (const t4Matrix*)animMats,
                (const t4Matrix*)texMat, renderFlags);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_RenderIndexedTriList);
#ifdef SKIP_ASM
// t3Vector / t4Vector with the On Tour inline constructors used below (t3Vector(float, float, float), t4Vector(),
// t4Vector(float, float, float, float), t4Vector(const t3vect&, float)), and t3vect's operator+ ([O] t3Vector
// operator+(const t3vect&) const).
struct t3VectorInit : t3Vector {
    t3VectorInit(float ax, float ay, float az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};

struct t4VectorInit : t4Vector {
    t4VectorInit() {}
    t4VectorInit(const float& ax, const float& ay, const float& az, const float& aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
    t4VectorInit(const t3Vector& v, const float& aw)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = aw;
    }
};

static inline t3VectorInit operator+(const t3Vector& a, const t3Vector& b)
{
    return t3VectorInit(a.x + b.x, a.y + b.y, a.z + b.z);
}

/**
 * Draws an indexed triangle list one triangle at a time through virtual slot 71 (three vertices per call).
 *
 * Each vertex is @p Positions[index] + *Offset with w 1, its colour Color[index] halved into a, r, g, b, and its texture
 * coordinates UV[index] (or 0, 0 when @p UV is null). Indices are bytes, so at most 256 vertices can be addressed.
 * @p NumVerts is not used.
 *
 * @param NumTris  the number of triangles
 * @param NumVerts  the number of vertices (unused)
 * @param Indices  three byte indices per triangle
 * @param Positions  the vertex positions (On Tour P)
 * @param UV  the vertex texture coordinates, or null for none
 * @param Color  the vertex colours (a, r, g, b floats)
 * @param Offset  a translation added to every position
 */
void cGraphicsMan::RenderIndexedTriList(int NumTris, int NumVerts, const unsigned char* Indices,
                                        const t3Vector* Positions, const t2Vector* UV, const tARGB* Color,
                                        const t3Vector& Offset)
{
    tBxVertex verts[3];
    (*(t4Vector*)&verts[2].u).z = 1.0f;
    (*(t4Vector*)&verts[1].u).z = 1.0f;
    (*(t4Vector*)&verts[0].u).z = 1.0f;
    if (UV) {
        for (int i = 0; i < NumTris; i++) {
            for (int j = 0; j < 3; j++) {
                int Index = Indices[j];
                verts[j].p = t4VectorInit(Positions[Index] + Offset, 1.0f);
                verts[j].a = (int)(Color[Index].A * 0.5f);
                verts[j].r = (int)(Color[Index].R * 0.5f);
                verts[j].g = (int)(Color[Index].G * 0.5f);
                verts[j].b = (int)(Color[Index].B * 0.5f);
                (*(t4Vector*)&verts[j].u).x = UV[Index].X();
                (*(t4Vector*)&verts[j].u).y = UV[Index].Y();
            }
            Indices += 3;
            RenderTriStrip(3, verts, 0);
        }
    } else {
        for (int i = 0; i < NumTris; i++) {
            for (int j = 0; j < 3; j++) {
                int Index = Indices[j];
                verts[j].p = t4VectorInit(Positions[Index] + Offset, 1.0f);
                verts[j].a = (int)(Color[Index].A * 0.5f);
                verts[j].r = (int)(Color[Index].R * 0.5f);
                verts[j].g = (int)(Color[Index].G * 0.5f);
                verts[j].b = (int)(Color[Index].B * 0.5f);
                (*(t4Vector*)&verts[j].u).z = 1.0f;
                (*(t4Vector*)&verts[j].u).x = 0.0f;
                (*(t4Vector*)&verts[j].u).y = 0.0f;
            }
            Indices += 3;
            RenderTriStrip(3, verts, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * cPS2GraphicsMan_update calls it.
 */
void cGraphicsMan_update(void* vself)
{
    cGraphicsMan* self = (cGraphicsMan*)vself;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_UploadTextures);
#ifdef SKIP_ASM
/**
 * Calls the graphics manager's virtual slot 121, PlatformUploadTextures (the platform texture upload).
 */
void cGraphicsMan::UploadTextures()
{
    PlatformUploadTextures();
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_PurgeTextures);
#ifdef SKIP_ASM
/**
 * Calls the graphics manager's virtual slot 120, PlatformPurgeTextures (the platform texture purge).
 */
void cGraphicsMan::PurgeTextures()
{
    PlatformPurgeTextures();
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_RenderSpline);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm (4x4 matrix times vector).
static inline t4VectorInit vu0MtxMul_A048(const t4Matrix* m, const t4VectorInit& v)
{
    t4VectorInit r;
    __asm__(
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, %2\n"
        "lqc2         $vf5, %3\n"
        "lqc2         $vf6, %4\n"
        "lqc2         $vf7, %5\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(r)
        : "m"(v), "m"(m->row[0]), "m"(m->row[1]), "m"(m->row[2]), "m"(m->row[3])
        : "memory");
    return r;
}

/**
 * Draws a cubic spline as a strip of @p n points through virtual slot 73.
 *
 * Point i is (t^3, t^2, t, 1) * @p BezMat at t = i / (n - 1); every point gets texture coordinates (0, 0, 1) and the
 * colour @p color scaled by 128 (its R, G, B and A). The vertices live in a fixed 20-entry
 * local array, so @p n above 20 overruns the stack.
 *
 * @param BezMat  the Bezier basis matrix of the curve
 * @param color  the colour (A, R, G, B in 0..1)
 * @param n  the number of points (On Tour NumPoints); at most 20
 */
void cGraphicsMan::RenderSpline(const t4Matrix& BezMat, const tARGB& color, int n)
{
    float step = 1.0f / ((float)n - 1.0f);
    int red = (int)(color.R * 128.0f);
    int green = (int)(color.G * 128.0f);
    int blue = (int)(color.B * 128.0f);
    int alpha = (int)(color.A * 128.0f);
    float curveParam = 0.0f;
    t4VectorInit pos;
    tBxVertex verts[20];
    for (int i = 0; i < n; i++) {
        float curveParamSq = curveParam * curveParam;
        pos = vu0MtxMul_A048(&BezMat, t4VectorInit(curveParam * curveParamSq, curveParamSq, curveParam, 1.0f));
        verts[i].u = 0.0f;
        verts[i].v = 0.0f;
        verts[i].one = 1.0f;
        verts[i].r = red;
        verts[i].g = green;
        verts[i].b = blue;
        verts[i].a = alpha;
        verts[i].p = pos;
        curveParam += step;
    }
    RenderLineStrip(n, verts, 0);
}
#endif

extern "C" void* cGarbageMan_Trash(void*);

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_DeleteGridMesh__FPvi);
#ifdef SKIP_ASM
/**
 * Queues a grid mesh for deferred deletion: calls cGarbageMan_Trash(@p gridMesh).
 *
 * Slot 77 of cPS2GraphicsMan_vtbl, after CreateGridMesh at slot 76.
 *
 * @param gridMesh  the grid mesh object (a garbage-can node) to queue
 */
void cGraphicsMan::DeleteGridMesh(cGridMesh* gridMesh)
{
    cGarbageMan_Trash(gridMesh);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_CreateScreenGrabTexture__FPvT0);
#ifdef SKIP_ASM
/**
 * The base cGraphicsMan::CreateScreenGrabTexture: marks the handle empty (no memory, texture id kNoScreenGrab).
 *
 * The width and height (On Tour's second and third parameters) are not used.
 *
 * @param screenGrabHandle  the handle to reset
 */
void cGraphicsMan::CreateScreenGrabTexture(tScreenGrabHandle& screenGrabHandle, unsigned int, unsigned int)
{
    screenGrabHandle.mMemory = 0;
    screenGrabHandle.mTextureId = tScreenGrabHandle::kNoScreenGrab;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGarbageMan_Trash);
#ifdef SKIP_ASM
extern int cGarbageMan_mCurCan;
extern void* cGarbageMan_mCan[];

/**
 * Pushes node @p n onto the current deferred-delete list cGarbageMan_mCan[cGarbageMan_mCurCan].
 *
 * Stores the old list head in the node's first word and makes the node the head. The node's second word is its vtable:
 * cGarbageMan_Update later deletes it through the virtual destructor. No return statement although declared to return void*.
 *
 * @param n  the node to queue
 * @return nothing defined (no return statement)
 */
// PORT: the unit declares this returning void*; the arg is a list node pointer.
extern "C" void* cGarbageMan_Trash(void* n)
{
    void** node = (void**)n;
    void** head = &cGarbageMan_mCan[cGarbageMan_mCurCan];
    *node = *head;
    *head = node;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGarbageMan_Update);
#ifdef SKIP_ASM
extern int cGarbageMan_mCurCan;
extern void* cGarbageMan_mCan[];

class cGmNode_A1E8 {
public:
    cGmNode_A1E8* next;
    virtual ~cGmNode_A1E8();
};

/**
 * Advances to the next of the four deferred-delete lists and deletes every object on it.
 *
 * cGarbageMan_mCurCan = (cGarbageMan_mCurCan + 1) % 4; each node of cGarbageMan_mCan[cGarbageMan_mCurCan] is deleted through its virtual destructor
 * (next pointer at +0, vtable at +4), then the list is cleared. An object queued by cGarbageMan_Trash is therefore deleted
 * on the fourth call after it was queued. cGame_render calls it once per frame.
 */
extern "C" void cGarbageMan_Update()
{
    cGarbageMan_mCurCan = (cGarbageMan_mCurCan + 1) % 4;
    cGmNode_A1E8* n = (cGmNode_A1E8*)cGarbageMan_mCan[cGarbageMan_mCurCan];
    while (n != 0) {
        cGmNode_A1E8* next = n->next;
        delete n;
        n = next;
    }
    cGarbageMan_mCan[cGarbageMan_mCurCan] = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/graphicsman", cGarbageMan_Purge);
#ifdef SKIP_ASM
extern "C" void cGarbageMan_Update();

/**
 * Empties all four deferred-delete lists by calling cGarbageMan_Update four times.
 *
 * cGame_purge calls it.
 */
extern "C" void cGarbageMan_Purge()
{
    int i;
    for (i = 0; i < 4; i++)
        cGarbageMan_Update();
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cGraphicsMan_DeformMeshMatrix);
#ifdef SKIP_ASM


extern "C" t4Matrix* cPlants_getDeformationMatrix(t4Vector* pos);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply out = a * b); the PC port needs a plain multiply.
static inline void gmMtxMul_A2C0(t4Matrix* out, t4Matrix* a, t4Matrix* b)
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
        : "r"(out), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (64-byte matrix copy); the PC port needs a plain copy.
static inline void gmMtxCopy_A2C0(t4Matrix* dst, t4Matrix* src)
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

/**
 * Applies the NIS deformation to a mesh matrix: when m_isInNIS[6] is set, multiplies @p mat by the deformation matrix
 * that cPlants_getDeformationMatrix returns for the matrix's translation.
 *
 * @param mat  the mesh matrix, updated in place
 */
void cGraphicsMan::DeformMeshMatrix(t4Matrix& mat)
{
    if (m_isInNIS[6] != 0) {
        t4Matrix result;
        t4Vector pos = mat.row[3];
        t4Matrix product;
        t4Matrix* deform = cPlants_getDeformationMatrix(&pos);
        gmMtxMul_A2C0(&product, &mat, deform);
        gmMtxCopy_A2C0(&result, &product);
        gmMtxCopy_A2C0(&mat, &result);
    }
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", morton_spreadByte);
#ifdef SKIP_ASM
/**
 * Spreads the low 8 bits of @p value to the even bit positions (bit i goes to bit 2i).
 *
 * @param value  the value whose low byte is spread
 * @return the spread value, at most 0x5555
 */
extern "C" int morton_spreadByte(int value) {
    int temp_2;
    int temp_2_2;
    int temp_2_3;

    temp_2_2 = value & 0xFF;
    temp_2_3 = (temp_2_2 | (temp_2_2 * 0x10)) & 0xF0F;
    temp_2 = (temp_2_3 | (temp_2_3 * 4)) & 0x3333;
    return (temp_2 | (temp_2 * 2)) & 0x5555;
}
#endif
//100%
INCLUDE_ASM("render/graphicsman", morton_interleaveBytes);
#ifdef SKIP_ASM
extern "C" int morton_spreadByte(int);

/**
 * Interleaves the low bytes of two values into a 16-bit Morton code.
 *
 * Returns morton_spreadByte(@p evenValue) | (morton_spreadByte(@p oddValue) << 1). No caller.
 *
 * @param evenValue  the value whose bits go to the even positions
 * @param oddValue  the value whose bits go to the odd positions
 * @return the interleaved 16-bit code
 */
extern "C" int morton_interleaveBytes(unsigned short evenValue, unsigned short oddValue)
{
    return morton_spreadByte(evenValue) | (morton_spreadByte(oddValue) << 1);
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036A428);

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_writeImageUpload16x16);
#ifdef SKIP_ASM
/**
 * Writes a DMA packet that uploads a 16x16 32-bit image from main memory into GS memory, then flushes the texture
 * cache.
 *
 * The packet (0xB0 bytes at *@p pp) is: a cnt DMA tag with a GIF A+D list setting BITBLTBUF (destination base @p y,
 * width 1 block, PSMCT32), TRXPOS 0, TRXREG 16x16 and TRXDIR 0 (host to local), followed by an IMAGE GIF tag of 64
 * qwords; a ref DMA tag pointing the 64 qwords at @p x; and a cnt tag with an A+D write of TEXFLUSH. Advances *@p pp by
 * 0xB0. @p self is unused. Caller: cPS2GraphicsMan_queueDepthFogPass.
 *
 * @param x  main-memory address of the 1 KB image (the DMA ref address)
 * @param y  GS destination base pointer (BITBLTBUF DBP)
 * @param pp  in/out: the packet write pointer
 */
// PORT: ulong is 64-bit here (GIF/DMA packet words); cQuad128 is a 128-bit GPR quadword.
void cPS2GraphicsMan::writeImageUpload16x16(int x, int y, char** pp)
{
    *(cQuad128*)(*pp + 0x00) = 0x10000006;
    *(ulong*)(*pp + 0x10) = ((ulong)0x10000000 << 32) | 4;
    *(ulong*)(*pp + 0x18) = 0xE;
    *(ulong*)(*pp + 0x20) = ((ulong)y << 32) | ((ulong)0x8000 << 33);
    *(ulong*)(*pp + 0x28) = 80;
    *(ulong*)(*pp + 0x30) = 0;
    *(ulong*)(*pp + 0x38) = 81;
    *(ulong*)(*pp + 0x40) = ((ulong)0x10 << 32) | 0x10;
    *(ulong*)(*pp + 0x48) = 82;
    *(ulong*)(*pp + 0x50) = 0;
    *(ulong*)(*pp + 0x58) = 83;
    *(cQuad128*)(*pp + 0x60) = ((cQuad128)0x08000000 << 32) | 0x40;
    *(ulong*)(*pp + 0x70) = ((ulong)x << 32) | 0x30000040;
    *(ulong*)(*pp + 0x78) = 0;
    *(cQuad128*)(*pp + 0x80) = 0x10000002;
    *(ulong*)(*pp + 0x90) = ((ulong)0x10000000 << 32) | 1;
    *(ulong*)(*pp + 0x98) = 0xE;
    *(ulong*)(*pp + 0xA0) = 0;
    *(ulong*)(*pp + 0xA8) = 0x3F;
    *pp += 0xB0;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_copyDepthFog);
#ifdef SKIP_ASM
extern float cDepthFog_m_NearPlane;
extern float cDepthFog_m_FarPlane;
extern int cDepthFog_m_Exponent;
extern float cDepthFog_m_Density;

struct sGmQuad_ABA0 {
    int v[4];
};
extern sGmQuad_ABA0 cDepthFog_m_Colour;

struct sGmEntry_ABA0 {
    float x;            // 0x0
    float y;            // 0x4
    int z;              // 0x8
    float w;            // 0xC
    sGmQuad_ABA0 q;     // 0x10
};

struct sGmOwner_ABA0 {
    char pad[0x6C64];
    sGmEntry_ABA0 entries[1];
};

/**
 * Copies the depth-fog tweak values into the graphics manager's fog entry for view @p idx.
 *
 * Writes cDepthFog_m_NearPlane, cDepthFog_m_FarPlane, cDepthFog_m_Exponent, cDepthFog_m_Density and the 16-byte
 * cDepthFog_m_Colour into the 0x20-byte entry at unk_6C64 + @p idx * 0x20. Callers: cPS2GraphicsMan_queueDepthFogPass (when
 * cDepthFog_m_OverrideWP is set) and cPS2GraphicsMan_UpdateFullScreenEffects.
 *
 * @param idx  the view index
 */
void cPS2GraphicsMan::copyDepthFog(int idx)
{
    char* entry = (char*)&((sGmOwner_ABA0*)this)->entries[idx];
    *(float*)(entry + 0x0) = cDepthFog_m_NearPlane;
    *(float*)(entry + 0x4) = cDepthFog_m_FarPlane;
    *(int*)(entry + 0x8) = cDepthFog_m_Exponent;
    *(float*)(entry + 0xC) = cDepthFog_m_Density;
    *(sGmQuad_ABA0*)(entry + 0x10) = cDepthFog_m_Colour;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_queueDepthFogPass);
#ifdef SKIP_ASM
extern int cDepthFog_m_Enable;
extern int cDepthFog_m_OverrideWP;
extern void* SPRMan;
extern char cPS2GraphicsMan_depthFogTableArray[];
extern "C" void cPS2GraphicsMan_copyDepthFog(void* self, int idx);
extern "C" void func_0036A428(void* self, void* mat, int idx);
extern "C" char* cPS2ScratchPad_claimBuffer(void* self, unsigned int addr, int size, int flags);
extern "C" int cPS2ScratchPad_flushBufferTo(void* self, char* end, int arg);
extern "C" int vramHeap_allocBlock(void* self, int size, int owner, int skipB, int level, int align);
extern "C" void cPS2GraphicsMan_writeImageUpload16x16(void* self, int x, int y, char** pp);
extern "C" void cPS2GraphicsMan_writeZToFrameCopy(cPS2GraphicsMan* self, int idx, int a, int b, int n, char** pp);
extern "C" void func_0036B158(void* self, int idx, int y, char** pp);
extern "C" void vramHeap_freeBlock(void* self, int id);
extern "C" void cTextureMan_appendFixedPacketRef(void* self, void* pkt);

struct sGmEntry_AC00 {
    float x;            // 0x0
    float y;            // 0x4
    int z;              // 0x8
    float w;            // 0xC
    int q[4];           // 0x10
};

struct sGmOwner_AC00 {
    char pad[0x6C64];
    sGmEntry_AC00 entries[1];
};

/**
 * Queues the depth-fog full-screen pass for view @p idx on the scratchpad DMA ring.
 *
 * Returns at once unless cDepthFog_m_Enable. When cDepthFog_m_OverrideWP is set it first refreshes the view's fog entry
 * (cPS2GraphicsMan_copyDepthFog); it skips the view when the entry's density is 0.0 and its exponent is non-zero. func_0036A428 fills
 * the 1 KB table at cPS2GraphicsMan_depthFogTableArray + @p idx * 0x400; vramHeap_allocBlock on mTexMan + 0x1F60 reserves 0x400 bytes of GS
 * memory, whose address is the record word at *(mTexMan + 0x3F50) + slot * 0x1C, shifted right 8.
 *
 * Three packets then go out through SPRMan (each opened with cPS2ScratchPad_claimBuffer and closed with cPS2ScratchPad_flushBufferTo, which
 * updates *@p handle): cPS2GraphicsMan_writeImageUpload16x16 uploads the table, cPS2GraphicsMan_writeZToFrameCopy runs over mCurrentZBufferPage and
 * mCurrentFrameBufferPage with twice the slot 11 GetScreenHeight value, and func_0036B158 finishes with the uploaded
 * table. Finally vramHeap_freeBlock releases the GS slot and cTextureMan_appendFixedPacketRef(mTexMan, @p pkt2) runs. Caller:
 * cPS2DeferManager_buildDmaChains.
 *
 * @param idx  the view index
 * @param pkt2  passed to cTextureMan_appendFixedPacketRef
 * @param handle  in/out: the DMA ring's current main-memory address
 */
void cPS2GraphicsMan::queueDepthFogPass(int idx, void* pkt2, int* handle)
{
    if (cDepthFog_m_Enable == 0) {
        return;
    }
    if (cDepthFog_m_OverrideWP != 0) {
        cPS2GraphicsMan_copyDepthFog(((char*)this), idx);
    }
    sGmEntry_AC00* fogEntry = &((sGmOwner_AC00*)this)->entries[idx];
    if (fogEntry->w == 0.0f && fogEntry->z != 0) {
        return;
    }
    char* mat = cPS2GraphicsMan_depthFogTableArray + (idx << 10);
    func_0036A428(((char*)this), mat, idx);
    void* dma = SPRMan;
    char* pkt = cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    int slot = vramHeap_allocBlock(mTexMan->mMainHeap, 0x400, 0x7FFFFFFF, 0, 0, 1);
    int* rec = (int*)(*(char**)((char*)mTexMan + 0x3F50) + slot * 0x1C);
    // PORT: the packet builder takes the matrix block's address as an int.
    cPS2GraphicsMan_writeImageUpload16x16(((char*)this), (int)mat, *rec >> 8, &pkt);
    *handle = cPS2ScratchPad_flushBufferTo(dma, pkt, 4);
    pkt = cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    int zBufPage = mCurrentZBufferPage;
    int frameBufPage = mCurrentFrameBufferPage;
    int n = GetScreenHeight();
    writeZToFrameCopy(idx, zBufPage, frameBufPage, n * 2, &pkt);
    *handle = cPS2ScratchPad_flushBufferTo(dma, pkt, 4);
    pkt = cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    func_0036B158(((char*)this), idx, *rec >> 8, &pkt);
    *handle = cPS2ScratchPad_flushBufferTo(dma, pkt, 4);
    vramHeap_freeBlock(mTexMan->mMainHeap, slot);
    cTextureMan_appendFixedPacketRef((char*)mTexMan, pkt2);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_writeZToFrameCopy);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here (GIF/DMA packet words).
struct sGmVE_AE20 {
    short delta;
    short index;
    unsigned int (*fn)(void*);
};

static inline unsigned int gmCall_AE20(char* vvself, int slot)
{
    cPS2GraphicsMan* vself = (cPS2GraphicsMan*)vvself;
    cPS2GraphicsMan* self = (cPS2GraphicsMan*)vself;
    sGmVE_AE20* vt = (sGmVE_AE20*)VPTR_OF(cGraphicsMan, self);
    return vt[slot].fn(vthis(self, vt[slot]));
}

/**
 * Appends a GS packet that reads the Z buffer as a 16-bit texture and copies it in 8-pixel strips into the frame
 * buffer, every 16 pixels.
 *
 * Sets FRAME_1 to @p frameBase (width = slot 10 GetScreenWidth / 64, PSMCT16, FBMSK 0x3FFF), ZBUF to
 * mCurrentZBufferPage (PSMZ24, no write), TEST to depth test ALWAYS, XYOFFSET to centre the screen in the 4096 space,
 * SCISSOR to mFBWidth x 2 * mFBHeight, then TEXFLUSH, TEXA (TA1 = 0x80) and TEX1 0. TEX0 is @p texBase << 5 as a PSMZ16
 * 1024x1024 texture with TCC and DECAL. With PRIM 0x116 (textured sprite, UV coordinates) and RGBAQ 0x80808080 (Q = 1)
 * it draws one sprite per 16 pixels: texels [16k, 16k + 8) to x [16k + 8, 16k + 16), @p n rows tall. @p idx is unused.
 * Caller: cPS2GraphicsMan_queueDepthFogPass.
 *
 * @param idx  the view index (unused)
 * @param texBase  the Z buffer page read as the texture
 * @param frameBase  the frame buffer page drawn into
 * @param n  the strip height in rows
 * @param pp  in/out: the packet write pointer
 */
void cPS2GraphicsMan::writeZToFrameCopy(int idx, int texBase, int frameBase, int n, char** pp)
{
    unsigned int full = 0x1000;
    int w = mFBWidth;
    int height = mFBHeight;
    int offX = ((full - gmCall_AE20(((char*)this), 10)) >> 1) << 4;
    int offY = ((full - gmCall_AE20(((char*)this), 11)) >> 1) << 4;
    int cols = w / 8;
    int zbp = mCurrentZBufferPage;
    ulong* packet = (ulong*)*pp;
    packet += 2;
    packet[0] = (cols * 2 + 11) | ((ulong)0x8000 << 45);
    packet[1] = 0xE;
    unsigned int fbw = gmCall_AE20(((char*)this), 10) >> 6;
    packet[2] = frameBase | ((ulong)fbw << 16) | ((ulong)0x3FFF0200 << 16);
    packet[3] = 76;
    packet[4] = (ulong)(unsigned int)(zbp | 0x1000000) | ((ulong)0x8000 << 17);
    packet[5] = 78;
    packet[6] = 0x30000;
    packet[7] = 71;
    packet[8] = offX | ((ulong)offY << 32);
    packet[9] = 0x18;
    packet[10] = ((ulong)(w - 1) << 16) | ((ulong)(height * 2 - 1) << 48);
    packet[11] = 0x40;
    packet[12] = 0;
    packet[13] = 0x3F;
    packet[14] = (ulong)0x8000 << 24;
    packet[15] = 59;
    packet[16] = 0;
    packet[17] = 20;
    unsigned int tbw = gmCall_AE20(((char*)this), 10) >> 6;
    packet[18] = (texBase << 5) | ((ulong)tbw << 14) | ((ulong)60082 << 20);
    packet[19] = 6;
    packet[20] = 278;
    packet[21] = 0;
    packet[22] = 0x3F80000080808080;
    packet[23] = 1;
    packet += 24;
    for (int i = 0; i < cols; i += 2) {
        int j = i * 0x80;
        int x0 = offX + 0x80 + j;
        int x1 = offX + 0x100 + j;
        int texU0 = 8 + j;
        int texU1 = 136 + j;
        packet[0] = texU0 | 0x80000;
        packet[1] = 3;
        packet[2] = x0 | ((ulong)offY << 16);
        packet[3] = 5;
        packet[4] = texU1 | ((ulong)(n * 16 + 8) << 16);
        packet[5] = 3;
        packet[6] = x1 | ((ulong)(offY + n * 16) << 16);
        packet[7] = 5;
        packet += 8;
    }
    *(ulong*)*pp = ((packet - (ulong*)*pp) / 2 - 1) | 0x10000000;
    ((ulong*)*pp)[1] = 0;
    *pp = (char*)packet;
}
#endif

INCLUDE_ASM("render/graphicsman", func_0036B158);

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_writeFogBlend);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here (GIF/DMA packet words).
struct sGmVE_B5E8 {
    short delta;
    short index;
    unsigned int (*fn)(void*);
};

// Saved scissor rectangle per view (self+0x59B0, copied from the current one at self+0x5900).
struct sGmRect_B5E8 {
    unsigned short x, y, w, h;
};

// Per-view fill colour (self+0x6C64, 0x20 bytes each): r, g, b at +0x14..+0x1C, 0..1.
struct sGmFill_B5E8 {
    char pad0[0x14];
    float r, g, b;          // 0x14
};

struct sGmMan_B5E8 {
    char pad0[0x10D8];
    sGmVE_B5E8* vt;             // 0x10D8
    char pad10DC[0x5900 - 0x10DC];
    sGmRect_B5E8 scissor;       // 0x5900: current
    char pad5908[0x59B0 - 0x5908];
    sGmRect_B5E8 saved[1];      // 0x59B0: per view
    char pad59B8[0x5A80 - 0x59B8];
    int zbp;                    // 0x5A80
    int fbp;                    // 0x5A84
    char pad5A88[0x6C64 - 0x5A88];
    sGmFill_B5E8 fill[1];       // 0x6C64: per view
};

static inline unsigned int gmVCall_B5E8(sGmMan_B5E8* self, int slot)
{
    sGmVE_B5E8* vt = self->vt;
    return vt[slot].fn(vthis(self, vt[slot]));
}

/**
 * Appends a GS packet that blends the depth-fog colour over view @p idx's rectangle where the depth test passes, then
 * restores the frame, offset and scissor.
 *
 * Sets FRAME_1 (32-bit), ZBUF without writes, TEST 0x70000 (depth test GREATER), XYOFFSET from the view, ALPHA_1 17
 * ((Cd - Cs) * Ad + Cs) and PRIM 70 (sprite with blending). RGBAQ is the fog entry's floats at +0x14, +0x18 and +0x1C
 * times 256 with alpha 0x80; a value of 1.0 gives 256 and spills into the next byte. It draws 32-pixel-wide strips at Z
 * 0x7FFF over the saved view rectangle mViewScissor[@p idx] (a width remainder below 32 is not drawn), then restores
 * FRAME, XYOFFSET and SCISSOR from mScissor. No caller.
 *
 * @param idx  the view index
 * @param pp  in/out: the packet write pointer
 */
extern "C" void cPS2GraphicsMan_writeFogBlend(sGmMan_B5E8* self, int idx, char** pp)
{
    sGmFill_B5E8* fill = &self->fill[idx];
    int x = self->saved[idx].x;
    int y = self->saved[idx].y;
    int w = self->saved[idx].w;
    int height = self->saved[idx].h;
    int offX = (((0x1000 - gmVCall_B5E8(self, 10)) >> 1) + x) << 4;
    int offY = (((0x1000 - gmVCall_B5E8(self, 11)) >> 1) + y) << 4;
    int cols = w / 32;
    int fbp = self->fbp;
    int zbp = self->zbp;
    ulong* packet = (ulong*)*pp;
    packet += 2;
    packet[0] = (cols * 2 + 7) | ((ulong)0x8000 << 45);
    packet[1] = 0xE;
    unsigned int fbw = gmVCall_B5E8(self, 10) >> 6;
    packet[2] = (ulong)(unsigned int)fbp | ((ulong)fbw << 16);
    packet[3] = 76;
    packet[4] = (ulong)(unsigned int)(zbp | 0x1000000) | ((ulong)0x8000 << 17);
    packet[5] = 78;
    packet[6] = 0x70000;
    packet[7] = 71;
    packet[8] = offX | ((ulong)offY << 32);
    packet[9] = 0x18;
    packet[10] = 17;
    packet[11] = 66;
    packet[12] = 70;
    packet[13] = 0;
    packet[14] = (ulong)(int)(fill->r * 256.0f) | ((ulong)(int)(fill->g * 256.0f) << 8)
          | ((ulong)(int)(fill->b * 256.0f) << 16) | 0x3F80000080000000;
    packet[15] = 1;
    packet += 16;
    int sx = 0;                 // strip's pixel offset from the left edge
    for (int i = 0; i < cols; i++) {
        int x0 = offX + sx * 16;
        packet[0] = x0 | ((ulong)offY << 16) | ((ulong)0x7FFF << 32);
        packet[1] = 5;
        packet[2] = (x0 + 0x200) | ((ulong)(offY + height * 16) << 16) | ((ulong)0x7FFF << 32);
        packet[3] = 5;
        packet += 4;
        sx += 32;
    }
    *(ulong*)*pp = ((packet - (ulong*)*pp) / 2 - 1) | 0x10000000;
    ((ulong*)*pp)[1] = 0;
    packet[0] = 0x10000004;
    packet[1] = 0;
    packet[2] = ((ulong)0x10000000 << 32) | 0x8003;
    packet[3] = 0xE;
    unsigned int fbp2 = self->fbp;
    unsigned int fbw2 = gmVCall_B5E8(self, 10) >> 6;
    packet[4] = (ulong)fbp2 | ((ulong)fbw2 << 16);
    packet[5] = 76;
    int offX2 = ((0x1000 - gmVCall_B5E8(self, 10)) >> 1) << 4;
    int offY2 = ((0x1000 - gmVCall_B5E8(self, 11)) >> 1) << 4;
    packet[6] = offX2 | ((ulong)offY2 << 32);
    packet[7] = 0x18;
    packet[8] = (ulong)self->scissor.x | ((ulong)(self->scissor.x + (self->scissor.w - 1)) << 16)
         | ((ulong)self->scissor.y << 32) | ((ulong)(self->scissor.y + (self->scissor.h - 1)) << 48);
    packet[9] = 0x40;
    packet += 10;
    *pp = (char*)packet;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_queueOffsetBlur);
#ifdef SKIP_ASM
/**
 * Queues a multi-pass offset blur: draws a texture region into a frame-buffer area once per pass, each pass shifted by
 * @p off and added to the previous ones.
 *
 * FRAME_1 is @p fbp >> 5 with width @p w / 64 (at least 1), PSMCT24, alpha masked; ZBUF without writes; TEST ALWAYS;
 * ALPHA 0x68 with FIX 0x80 (Cs + Cd); SCISSOR @p w x @p height; TEX1 bilinear; TEX0 @p tbp, width @p tw / 64, PSMCT24,
 * MODULATE; CLAMP REGION_CLAMP to [@p srcU, @p srcU + @p uw - 1] x [@p srcV, @p srcV + @p vh - 1]. The colour is @p intensity / @p
 * passes as grey. Pass 1 uses PRIM 0x116 (overwrite) and passes 2 to 4 PRIM 0x156 (blend), offset by (-off, -off),
 * (+off, +off), (+off, -off), (-off, +off) in 1/16 pixels.
 *
 * Likely bug: the GIF tag's NLOOP is cols * 4 * passes + 13, but only four offsets exist, so @p passes above 4 would
 * describe more data than is written. Caller: cPS2GraphicsMan_queueLightGlowPass (1 pass to downsample, 4 to blur).
 *
 * @param vself  the graphics manager (cPS2GraphicsMan)
 * @param tbp  the source texture's GS base pointer
 * @param tw  the source texture width
 * @param srcU  the source region's left texel
 * @param srcV  the source region's top texel
 * @param uw  the source region width
 * @param vh  the source region height
 * @param fbp  the destination frame-buffer address (>> 5 gives FBP)
 * @param intensity  the summed brightness
 * @param w  the destination width
 * @param height  the destination height
 * @param off  the per-pass offset in 1/16 pixels
 * @param passes  the number of passes, 1 to 4
 * @param handle  in/out: the DMA ring's current main-memory address
 */
// PORT: ulong is 64-bit here (GIF/DMA packet words).
// Multi-pass offset blur: draws the source region `passes` times as column sprites, each pass
// shifted diagonally by +-off texels, with the vertex colour scaled by intensity / passes.
void cPS2GraphicsMan::queueOffsetBlur(int tbp, int tw, int srcU, int srcV, int uw, int vh, int fbp, float intensity,
                                      int w, int height, int off, int passes, int* handle)
{
    int uStart = (srcU << 4) + 8 + off;
    int du = ((srcU + uw) << 4) + 8 - off - uStart;
    int vStart = (srcV << 4) + 8 + off;
    int vEnd = ((srcV + vh) << 4) + 8 - off;
    int cols = w / 32;
    void* dma = SPRMan;
    int zbp = mCurrentZBufferPage;
    int w16 = w << 4;
    int x0;
    int x1;
    ulong* gsPacket = (ulong*)cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    int grey = (int)(intensity / (float)passes);
    int fbw = w / 64;
    if (fbw <= 0) fbw = 1;
    int tbw = tw / 64;
    if (tbw <= 0) tbw = 1;
    gsPacket[0] = (cols * 4 * passes + 14) | 0x10000000;
    gsPacket[1] = 0;
    gsPacket += 2;
    gsPacket[0] = (cols * 4 * passes + 13) | ((ulong)0x8000 << 45);
    gsPacket[1] = 0xE;
    gsPacket[2] = (fbp >> 5) | ((ulong)fbw << 16) | 0xFF00000001000000;
    gsPacket[3] = 76;
    gsPacket[4] = (zbp | 0x1000000) | ((ulong)0x8000 << 17);
    gsPacket[5] = 78;
    gsPacket[6] = 0x30000;
    gsPacket[7] = 71;
    gsPacket[8] = 0;
    gsPacket[9] = 0x18;
    gsPacket[10] = 0x68 | ((ulong)0x80 << 32);
    gsPacket[11] = 66;
    gsPacket[12] = 0;
    gsPacket[13] = 0x3F;
    gsPacket[14] = ((ulong)(w - 1) << 16) | ((ulong)(height - 1) << 48);
    gsPacket[15] = 0x40;
    gsPacket[16] = 0x60;
    gsPacket[17] = 20;
    gsPacket[18] = tbp | ((ulong)tbw << 14) | ((ulong)54530 << 19);
    gsPacket[19] = 6;
    gsPacket[20] = 0xA | ((ulong)srcU << 4) | ((ulong)(srcU + uw - 1) << 14) | ((ulong)srcV << 24) | ((ulong)(srcV + vh - 1) << 34);
    gsPacket[21] = 8;
    gsPacket += 22;
    gsPacket[0] = 278;
    gsPacket[1] = 0;
    gsPacket[2] = grey | ((ulong)grey << 8) | ((ulong)grey << 16) | 0x3F80000080000000;
    gsPacket[3] = 1;
    gsPacket += 4;
    *handle = cPS2ScratchPad_flushBufferTo(dma, (char*)gsPacket, 4);

    gsPacket = (ulong*)cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    x1 = 0x200;
    x0 = 0;
    for (int i = 0; i < cols; i++) {
        int ua = uStart + x0 * du / w16;
        int ub = uStart + x1 * du / w16;
        gsPacket[0] = (ua - off) | ((ulong)(vStart - off) << 16);
        gsPacket[1] = 3;
        gsPacket[2] = x0;
        gsPacket[3] = 5;
        gsPacket[4] = (ub - off) | ((ulong)(vEnd - off) << 16);
        gsPacket[5] = 3;
        gsPacket[6] = x1 | ((ulong)(height << 4) << 16);
        gsPacket[7] = 5;
        x0 += 0x200;
        x1 = (x1 + 0x200 < w16) ? x1 + 0x200 : w16;
        gsPacket += 8;
    }
    gsPacket[0] = 342;
    gsPacket[1] = 0;
    gsPacket += 2;
    *handle = cPS2ScratchPad_flushBufferTo(dma, (char*)gsPacket, 4);
    if (passes < 2) {
        return;
    }

    gsPacket = (ulong*)cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    x1 = 0x200;
    x0 = 0;
    for (int i = 0; i < cols; i++) {
        int ua = uStart + x0 * du / w16;
        int ub = uStart + x1 * du / w16;
        gsPacket[0] = (ua + off) | ((ulong)(vStart + off) << 16);
        gsPacket[1] = 3;
        gsPacket[2] = x0;
        gsPacket[3] = 5;
        gsPacket[4] = (ub + off) | ((ulong)(vEnd + off) << 16);
        gsPacket[5] = 3;
        gsPacket[6] = x1 | ((ulong)(height << 4) << 16);
        gsPacket[7] = 5;
        x0 += 0x200;
        x1 = (x1 + 0x200 < w16) ? x1 + 0x200 : w16;
        gsPacket += 8;
    }
    *handle = cPS2ScratchPad_flushBufferTo(dma, (char*)gsPacket, 4);
    if (passes < 3) {
        return;
    }

    gsPacket = (ulong*)cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    x1 = 0x200;
    x0 = 0;
    for (int i = 0; i < cols; i++) {
        int ua = uStart + x0 * du / w16;
        int ub = uStart + x1 * du / w16;
        gsPacket[0] = (ua + off) | ((ulong)(vStart - off) << 16);
        gsPacket[1] = 3;
        gsPacket[2] = x0;
        gsPacket[3] = 5;
        gsPacket[4] = (ub + off) | ((ulong)(vEnd - off) << 16);
        gsPacket[5] = 3;
        gsPacket[6] = x1 | ((ulong)(height << 4) << 16);
        gsPacket[7] = 5;
        x0 += 0x200;
        x1 = (x1 + 0x200 < w16) ? x1 + 0x200 : w16;
        gsPacket += 8;
    }
    *handle = cPS2ScratchPad_flushBufferTo(dma, (char*)gsPacket, 4);
    if (passes < 4) {
        return;
    }

    gsPacket = (ulong*)cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    x0 = 0;
    x1 = 0x200;
    for (int i = 0; i < cols; i++) {
        int ua = uStart + x0 * du / w16;
        int ub = uStart + x1 * du / w16;
        gsPacket[0] = (ua - off) | ((ulong)(vStart + off) << 16);
        gsPacket[1] = 3;
        gsPacket[2] = x0;
        gsPacket[3] = 5;
        gsPacket[4] = (ub - off) | ((ulong)(vEnd + off) << 16);
        gsPacket[5] = 3;
        gsPacket[6] = x1 | ((ulong)(height << 4) << 16);
        gsPacket[7] = 5;
        x0 += 0x200;
        x1 = (x1 + 0x200 < w16) ? x1 + 0x200 : w16;
        gsPacket += 8;
    }
    *handle = cPS2ScratchPad_flushBufferTo(dma, (char*)gsPacket, 4);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_queueGreySubtract);
#ifdef SKIP_ASM
extern "C" void cPS2GraphicsMan_queueGreySubtract(void* self, int x, int w, int y, int grey, int alpha, int* handle);

/**
 * Queues a GS pass that subtracts a grey level from a frame-buffer area and scales the rest: (Cd - grey) * @p alpha /
 * 128.
 *
 * FRAME_1 is @p x >> 5, width @p w / 64 (at least 1), PSMCT24 with alpha masked; XYOFFSET 0; ALPHA 0xA1 ((Cd - Cs) *
 * FIX) with FIX = @p alpha; PRIM 0x146 (untextured sprites with blending) in colour @p grey. It draws @p w / 32 sprites
 * covering @p w x @p y. Likely bug: a width below 32 makes the strip count 0 and the step computation divides by zero;
 * cPS2GraphicsMan_queueLightGlowPass passes 1 << cLightGlow_m_PS2CaptureSize, which its menu lets drop to 16. Caller: cPS2GraphicsMan_queueLightGlowPass.
 *
 * @param x  the frame-buffer address of the area (>> 5 gives FBP)
 * @param w  the area width
 * @param y  the area height
 * @param grey  the grey level subtracted (0 to 255)
 * @param alpha  the scale, 128 = 1.0
 * @param handle  in/out: the DMA ring's current main-memory address
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: ulong is 64-bit here (GIF/DMA packet words).
void cPS2GraphicsMan::queueGreySubtract(int x, int w, int y, int grey, int alpha, int* handle)
{
    int cols = w / 32;
    int step = (w / cols) << 4;
    void* dma = SPRMan;
    ulong* gsPacket = (ulong*)cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    int fbw = w / 64;
    if (fbw <= 0) fbw = 1;
    gsPacket[0] = (cols * 2 + 7) | 0x10000000;
    gsPacket[1] = 0;
    gsPacket += 2;
    gsPacket[0] = (cols * 2 + 6) | ((ulong)0x8000 << 45);
    gsPacket[1] = 0xE;
    gsPacket[2] = (x >> 5) | ((ulong)fbw << 16) | 0xFF00000001000000;
    gsPacket[3] = 76;
    gsPacket[4] = 0;
    gsPacket[5] = 0x18;
    gsPacket[6] = 0xA1 | ((ulong)alpha << 32);
    gsPacket[7] = 66;
    gsPacket[8] = 0;
    gsPacket[9] = 0x3F;
    gsPacket += 10;
    gsPacket[0] = 326;
    gsPacket[1] = 0;
    gsPacket[2] = grey | ((ulong)grey << 8) | ((ulong)grey << 16) | 0x3F80000080000000;
    gsPacket[3] = 1;
    gsPacket += 4;
    int x0 = 0;
    int x1 = step;
    for (int i = 0; i < cols; i++) {
        gsPacket[0] = x0;
        gsPacket[1] = 5;
        gsPacket[2] = x1 | ((ulong)(y << 4) << 16);
        gsPacket[3] = 5;
        x0 += step;
        x1 += step;
        gsPacket += 4;
    }
    *handle = cPS2ScratchPad_flushBufferTo(dma, (char*)gsPacket, 4);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_queueTextureAdd);
#ifdef SKIP_ASM
/**
 * Queues a GS pass that adds a texture, scaled by @p alpha / 128, onto the view rectangle @p x, @p y, @p w, @p height.
 *
 * FRAME_1 is @p fbp >> 5 with the slot 10 screen width / 64 (width 1 when that is 0), PSMCT24, alpha masked; ZBUF
 * mCurrentZBufferPage without writes; TEST ALWAYS; ALPHA 0x29 (Cs + Cd * FIX) with FIX = @p alpha; SCISSOR to the
 * rectangle; TEX0 @p tbp, PSMCT24, with CLAMP REGION_CLAMP to @p tw x @p th; PRIM 0x156; colour @p grey in all four
 * channels. It draws @p w / 32 columns, each mapping a 1/cols slice of the texture. Caller: cPS2GraphicsMan_queueLightGlowPass.
 *
 * @param vself  the graphics manager (cPS2GraphicsMan)
 * @param tbp  the texture's GS base pointer
 * @param tw  the texture width
 * @param th  the texture height
 * @param fbp  the destination frame-buffer address (>> 5 gives FBP)
 * @param x  the rectangle's left edge
 * @param y  the rectangle's top edge
 * @param w  the rectangle width
 * @param height  the rectangle height
 * @param grey  the vertex colour (texture modulation)
 * @param alpha  the destination scale, 128 = 1.0
 * @param handle  in/out: the DMA ring's current main-memory address
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: ulong is 64-bit here (GIF/DMA packet words).
void cPS2GraphicsMan::queueTextureAdd(int tbp, int tw, int th, int fbp, int x, int y, int w, int height, int grey,
                                      int alpha, int* handle)
{
    int cols = w / 32;
    int step = (tw / cols) << 4;
    void* dma = SPRMan;
    int zbp = mCurrentZBufferPage;
    ulong* gsPacket = (ulong*)cPS2ScratchPad_claimBuffer(dma, *handle, -1, 0);
    gsPacket[0] = (cols * 4 + 13) | 0x10000000;
    gsPacket[1] = 0;
    gsPacket += 2;
    gsPacket[0] = (cols * 4 + 12) | ((ulong)0x8000 << 45);
    gsPacket[1] = 0xE;
    ulong fr = fbp >> 5;
    ulong* frameWord = gsPacket + 2;
    if (gmCall_AE20(((char*)this), 10) >> 6) {
        fr = fr | ((ulong)(gmCall_AE20(((char*)this), 10) >> 6) << 16) | 0xFF00000001000000;
    } else {
        fr = fr | 0xFF00000001010000;
    }
    *frameWord = fr;
    int tbw = tw / 64;
    if (tbw <= 0) tbw = 1;
    gsPacket[3] = 76;
    gsPacket[4] = (zbp | 0x1000000) | ((ulong)0x8000 << 17);
    gsPacket[5] = 78;
    gsPacket[6] = 0x30000;
    gsPacket[7] = 71;
    gsPacket[8] = 0;
    gsPacket[9] = 0x18;
    gsPacket[10] = 0x29 | ((ulong)alpha << 32);
    gsPacket[11] = 66;
    gsPacket[12] = 0;
    gsPacket[13] = 0x3F;
    gsPacket[14] = x | ((ulong)(x + w - 1) << 16) | ((ulong)y << 32) | ((ulong)(y + height - 1) << 48);
    gsPacket[15] = 0x40;
    gsPacket[16] = 0x60;
    gsPacket[17] = 20;
    gsPacket[18] = tbp | ((ulong)tbw << 14) | ((ulong)54530 << 19);
    gsPacket[19] = 6;
    gsPacket[20] = 0xA | ((ulong)(tw - 1) << 14) | ((ulong)(th - 1) << 34);
    gsPacket[21] = 8;
    gsPacket[22] = 342;
    gsPacket[23] = 0;
    gsPacket[24] = grey | ((ulong)grey << 8) | ((ulong)grey << 16) | ((ulong)grey << 24) | 0x3F80000000000000;
    gsPacket[25] = 1;
    gsPacket += 26;
    int texU0 = 8;
    int texU1 = step + 8;
    int x0 = x << 4;
    for (int i = 0; i < cols; i++) {
        gsPacket[0] = texU0;
        gsPacket[1] = 3;
        gsPacket[2] = x0 | ((ulong)(y << 4) << 16);
        gsPacket[3] = 5;
        gsPacket[4] = texU1 | ((ulong)(th << 4) << 16);
        gsPacket[5] = 3;
        gsPacket[6] = (x0 + 0x200) | ((ulong)((y + height) << 4) << 16);
        gsPacket[7] = 5;
        x0 += 0x200;
        texU0 += step;
        texU1 += step;
        gsPacket += 8;
    }
    *handle = cPS2ScratchPad_flushBufferTo(dma, (char*)gsPacket, 4);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_copyLightGlow);
#ifdef SKIP_ASM
extern float cLightGlow_m_PS2MinIntensity;
extern float cLightGlow_m_PS2ModIntensity;
extern float cLightGlow_m_PS2CopyIntensity;
extern float cLightGlow_m_PS2FrameSrcScale;
extern float cLightGlow_m_PS2FrameDstScale;
extern float cLightGlow_m_PS2IntensityTex2;
extern float cLightGlow_m_PS2IntensityTex3;

struct sGmEntry_C740 {
    float v[7];
};


extern "C" void cPS2GraphicsMan_copyLightGlow(void* self, int idx);

/**
 * Copies the seven cLightGlow PS2 tweak values into the graphics manager's glow entry for view @p idx.
 *
 * Writes cLightGlow_m_PS2MinIntensity, _m_PS2ModIntensity, _m_PS2CopyIntensity, _m_PS2FrameSrcScale,
 * _m_PS2FrameDstScale, _m_PS2IntensityTex2 and _m_PS2IntensityTex3 to unk_6CD4 + @p idx * 28. Caller: cPS2GraphicsMan_queueLightGlowPass
 * (when cLightGlow_m_OverrideWP is set).
 *
 * @param idx  the view index
 */
void cPS2GraphicsMan::copyLightGlow(int idx)
{
    char* entry = (char*)&((sGmEntry_C740*)&((cPS2GraphicsMan*)this)->unk_6CD4[0][0])[idx];
    *(float*)(entry + 0x0) = cLightGlow_m_PS2MinIntensity;
    *(float*)(entry + 0x4) = cLightGlow_m_PS2ModIntensity;
    *(float*)(entry + 0x8) = cLightGlow_m_PS2CopyIntensity;
    *(float*)(entry + 0xC) = cLightGlow_m_PS2FrameSrcScale;
    *(float*)(entry + 0x10) = cLightGlow_m_PS2FrameDstScale;
    *(float*)(entry + 0x14) = cLightGlow_m_PS2IntensityTex2;
    *(float*)(entry + 0x18) = cLightGlow_m_PS2IntensityTex3;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cPS2GraphicsMan_queueLightGlowPass);
#ifdef SKIP_ASM
extern int cLightGlow_m_Enable;
extern int cLightGlow_m_OverrideWP;
extern int cLightGlow_m_PS2CaptureSize;
extern float cLightGlow_m_PS2ModIntensity;
extern float cLightGlow_m_PS2IntensityTex0;
extern float cLightGlow_m_PS2IntensityTex1;
extern float cLightGlow_m_PS2JitterTex;

// Per-view bloom parameters (7 floats at gm+0x6CD4, see cPS2GraphicsMan_copyLightGlow).
struct sGmBloom_C790 {
    float v[7];
};

// Per-view viewport rectangle at gm+0x59B0.
struct sGmView_C790 {
    unsigned short x;
    unsigned short y;
    unsigned short w;
    unsigned short h;
};

struct sGmViews_C790 {
    char pad[0x59B0];
    sGmView_C790 views[2];
};

/**
 * Queues the light-glow pass for view @p idx: downsamples the view into a buffer in the Z-buffer area, cuts it off,
 * then blurs and composites it back at three levels.
 *
 * Returns at once unless cLightGlow_m_Enable; when cLightGlow_m_OverrideWP is set it first refreshes the view's entry
 * (cPS2GraphicsMan_copyLightGlow). The four composite strengths are cLightGlow_m_PS2IntensityTex0 and cLightGlow_m_PS2IntensityTex1 (menu "PS2 Blend Texture 0" and "1") and
 * the entry's IntensityTex2/IntensityTex3 values, each times 127.5; when all are 0 it does nothing. The buffers are at
 * mCurrentZBufferPage << 5 and 0x400 above it, 1 << cLightGlow_m_PS2CaptureSize ("PS2 Capture Size (2^x)") texels square.
 *
 * cPS2GraphicsMan_queueOffsetBlur downsamples the view (mViewScissor[@p idx]) from mCurrentFrameBufferPage with the FrameSrcScale value;
 * cPS2GraphicsMan_queueGreySubtract subtracts the MinIntensity value and scales by cLightGlow_m_PS2ModIntensity (the global, not the
 * entry). For three levels it composites the current buffer with cPS2GraphicsMan_queueTextureAdd when that level's strength is set
 * (FrameDstScale as the scale), then blurs it into the other buffer at half size with four passes of cPS2GraphicsMan_queueOffsetBlur
 * (CopyIntensity, offset cLightGlow_m_PS2JitterTex "PS2 Texture Jitter" * 16). The fourth strength composites the last level. It then
 * restores FRAME and XYOFFSET for the screen and calls cTextureMan_appendFixedPacketRef(mTexMan, @p pkt2). Caller: cPS2DeferManager_buildDmaChains.
 *
 * @param vself  the graphics manager (cPS2GraphicsMan)
 * @param idx  the view index
 * @param pkt2  passed to cTextureMan_appendFixedPacketRef
 * @param handle  in/out: the DMA ring's current main-memory address
 */
// PORT: ulong is 64-bit here (GIF packet words); the packet is written through the uncached mirror.
void cPS2GraphicsMan::queueLightGlowPass(int idx, void* pkt2, int* handle)
{
    if (cLightGlow_m_Enable == 0) {
        return;
    }
    if (cLightGlow_m_OverrideWP != 0) {
        cPS2GraphicsMan_copyLightGlow(((char*)this), idx);
    }
    sGmBloom_C790* glowParams = (sGmBloom_C790*)(&unk_6CD4[0]) + idx;
    int alpha[4] = {
        (int)(cLightGlow_m_PS2IntensityTex0 * 127.5f),
        (int)(cLightGlow_m_PS2IntensityTex1 * 127.5f),
        (int)(glowParams->v[5] * 127.5f),
        (int)(glowParams->v[6] * 127.5f),
    };
    if (alpha[3] == 0 && alpha[2] == 0 && alpha[1] == 0 && alpha[0] == 0) {
        return;
    }
    int buf0 = mCurrentZBufferPage << 5;
    int bufs[2];
    bufs[0] = buf0;
    bufs[1] = buf0 + 0x400;
    int offset = (int)(cLightGlow_m_PS2JitterTex * 16.0f);
    int src = buf0;
    int th = 1 << cLightGlow_m_PS2CaptureSize;
    int tw = th;
    int x0 = ((sGmViews_C790*)this)->views[idx].x;
    int y0 = ((sGmViews_C790*)this)->views[idx].y;
    int w = ((sGmViews_C790*)this)->views[idx].w;
    int drawFbp = mCurrentFrameBufferPage;
    int viewHeight = ((sGmViews_C790*)this)->views[idx].h;
    int width = gmCall_AE20(((char*)this), 10);
    int fbp32 = drawFbp << 5;
    queueOffsetBlur(fbp32, width, x0, y0, w, viewHeight, src, glowParams->v[3] * 127.5f, tw, th, 0, 1, handle);
    cPS2GraphicsMan_queueGreySubtract(((char*)this), src, tw, th, (int)(glowParams->v[0] * 127.5f),
                                      (int)(cLightGlow_m_PS2ModIntensity * 127.5f), handle);
    for (int i = 0; i < 3; i++) {
        if (alpha[i] != 0) {
            queueTextureAdd(src, tw, th, fbp32, x0, y0, w, viewHeight, alpha[i], (int)(glowParams->v[4] * 127.5f),
                            handle);
        }
        int psrc = src;
        int pw = tw;
        int ph = th;
        src = bufs[(i + 1) & 1];
        tw >>= 1;
        th >>= 1;
        queueOffsetBlur(psrc, pw, 0, 0, pw, ph, src, glowParams->v[2] * 127.5f, tw, th, offset, 4, handle);
    }
    if (alpha[3] != 0) {
        queueTextureAdd(src, tw, th, fbp32, x0, y0, w, viewHeight, alpha[3], (int)(glowParams->v[4] * 127.5f), handle);
    }
    ulong* gsPacket = (ulong*)(*handle | 0x30000000);
    unsigned int full = 0x1000;
    int offX = ((full - gmCall_AE20(((char*)this), 10)) >> 1) << 4;
    int offY = ((full - gmCall_AE20(((char*)this), 11)) >> 1) << 4;
    gsPacket[0] = 0x10000003;
    gsPacket[1] = 0;
    gsPacket[2] = 0x1000000000008002;
    gsPacket[3] = 0xE;
    gsPacket[4] = drawFbp | ((ulong)(gmCall_AE20(((char*)this), 10) >> 6) << 16);
    gsPacket[5] = 76;
    gsPacket[6] = offX | ((ulong)offY << 32);
    gsPacket[7] = 0x18;
    gsPacket += 8;
    *handle = (int)gsPacket & 0x0FFFFFFF;
    cTextureMan_appendFixedPacketRef((char*)mTexMan, pkt2);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_init);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero;

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4Vector vu0DivGM(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

/**
 * Initialises a particle emitter's counts, time step, force and blur (On Tour init(num, numblur, duration, blurstep,
 * force, damp), with damp and force swapped).
 *
 * TimeStep = @p x * @p damp / @p w, Force = @p force / @p damp / @p damp, Damp = @p damp, BlurAlpha = 1 / NumBlur, BlurStep = @p y *
 * @p damp, UpperUV = 0 and LowerUV = (1, 1, 0, 0). Damp acts as the time scale: setLife and setTime multiply by it too.
 *
 * @param w  the number of particles (On Tour num)
 * @param numblur  the number of blur copies per particle
 * @param x  the emission duration (On Tour duration)
 * @param y  the blur step (On Tour blurstep)
 * @param damp  the time scale (tParticleData::Damp)
 * @param force  the force vector, divided by damp squared
 */
void tParticleEmitter::init(int w, int numblur, float x, float y, float damp, t4Vector* force)
{
    NumParticles = w;
    NumBlur = numblur;
    TimeStep = x * damp / (float)w;
    Force = vu0DivGM(vu0DivGM(*force, damp), damp);
    Damp = damp;
    BlurAlpha = 1.0f / (float)NumBlur;
    BlurStep = y * damp;
    t4Vector one;
    one.x = 1.0f;
    one.y = 1.0f;
    one.z = 0.0f;
    one.w = 0.0f;
    UpperUV = t4vect_Zero;
    LowerUV = one;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_randomize);
#ifdef SKIP_ASM
int BXrand();

static inline float randf_0036CCB8(float a, float b)
{
    union { int i; float f; } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return a + (b - a) * (u.f - 1.0f);
}

/**
 * Gives the particle system new random seeds.
 *
 * Sets SeedX0, SeedX1, SeedV0, SeedV1, SeedV2, SeedC0, SeedC1, SeedSize and SeedLife each to randf_0036CCB8(1.0, 2.0).
 */
void tParticleEmitter::randomize()
{
    SeedX0 = randf_0036CCB8(1.0f, 2.0f);
    SeedX1 = randf_0036CCB8(1.0f, 2.0f);
    SeedV0 = randf_0036CCB8(1.0f, 2.0f);
    SeedV1 = randf_0036CCB8(1.0f, 2.0f);
    SeedV2 = randf_0036CCB8(1.0f, 2.0f);
    SeedC0 = randf_0036CCB8(1.0f, 2.0f);
    SeedC1 = randf_0036CCB8(1.0f, 2.0f);
    SeedSize = randf_0036CCB8(1.0f, 2.0f);
    SeedLife = randf_0036CCB8(1.0f, 2.0f);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setSize);
#ifdef SKIP_ASM
/**
 * Sets the particle size range: SizeR = @p sizerandomness, SizeFinalDelta = @p endsize - @p startsize and Size0 = @p startsize - 1.5 * @p sizerandomness.
 *
 * @param startsize  the size at birth
 * @param endsize  the size at death
 * @param sizerandomness  the random size spread
 */
void tParticleEmitter::setSize(float startsize, float endsize, float sizerandomness)
{
    SizeR = sizerandomness;
    SizeFinalDelta = endsize - startsize;
    Size0 = startsize - sizerandomness * 1.5f;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setLife);
#ifdef SKIP_ASM
/**
 * Sets the particle life range scaled by Damp: LifeR = (@p maxlife - @p minlife) * Damp and Life0 = @p minlife * Damp - LifeR.
 *
 * @param minlife  the shortest particle life
 * @param maxlife  the longest particle life
 */
void tParticleEmitter::setLife(float minlife, float maxlife)
{
    float range = maxlife - minlife;
    float dampFactor = Damp;
    minlife = minlife * dampFactor;
    range = range * dampFactor;
    LifeR = range;
    Life0 = minlife - range;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setEternal);
#ifdef SKIP_ASM
/**
 * Makes @p particleEmitter's particles live forever: LifeR = 0 and Life0 = 1e20.
 *
 * No caller.
 *
 */
void tParticleEmitter::setEternal()
{
    LifeR = 0.0f;
    Life0 = 1e20f;
}
#endif
//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setPoint);
#ifdef SKIP_ASM
/**
 * Makes @p particleEmitter emit from a single point: X0 = *@p point and Xr0 = Xr1 = t4vect_Zero.
 *
 * Copies the 16-byte vectors as 128-bit quadwords. No caller.
 *
 * @param particleEmitter  the emitter
 * @param point  the emission point (a 16-byte vector)
 */
void tParticleEmitter::setPoint(const t4Vector *point)
{
    X0 = *point;
    Xr0 = t4vect_Zero;
    Xr1 = t4vect_Zero;
}
#endif
//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setLine);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (a - b). Same body as vu0SubGM (defined later in the unit).
static inline t4Vector vu0Sub_CE80(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s). Same body as vu0ScaleGM (defined later in the unit).
static inline t4Vector vu0Scale_CE80(const t4Vector& v, float s)
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

/**
 * Makes particles start along a line segment.
 *
 * Sets X0 to 2 * @p start - @p end, Xr0 to @p end - @p start and Xr1 to zero, so the random emission offsets span the
 * segment (the same scheme as tParticleEmitter_setPlane() with one axis).
 *
 * @param start  one end of the segment
 * @param end  the other end
 */
void tParticleEmitter::setLine(t4Vector* start, t4Vector* end)
{
    X0 = vu0Sub_CE80(vu0Scale_CE80(*start, 2.0f), *end);
    Xr0 = vu0Sub_CE80(*end, *start);
    Xr1 = t4vect_Zero;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setPlane);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0AddGM(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0SubGM(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vu0ScaleGM(const t4Vector& v, float s)
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

/**
 * Makes particles start on a parallelogram given by a centre and two axes.
 *
 * Sets X0 to @p center - 3 * (@p axis0 + @p axis1), Xr0 to 2 * @p axis0 and Xr1 to 2 * @p axis1; the emitter adds
 * random multiples of Xr0 and Xr1 to X0.
 *
 * @param center  the centre
 * @param axis0  the first half axis
 * @param axis1  the second half axis
 */
void tParticleEmitter::setPlane(t4Vector* center, t4Vector* axis0, t4Vector* axis1)
{
    X0 = vu0SubGM(*center, vu0ScaleGM(vu0AddGM(*axis0, *axis1), 3.0f));
    Xr0 = vu0ScaleGM(*axis0, 2.0f);
    Xr1 = vu0ScaleGM(*axis1, 2.0f);
}
#endif

extern t4Vector t4vect_Zero;

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setMotion);
#ifdef SKIP_ASM
/**
 * Sets how far the emission point drifts over a time.
 *
 * Sets Xi to @p delta * TimeStep / (@p time * Damp), the displacement per particle step; tParticleEmitter_stopMotion()
 * clears it again.
 *
 * @param delta  the total displacement
 * @param time  the time it takes
 */
void tParticleEmitter::setMotion(t4Vector* delta, float time)
{
    Xi = vu0DivGM(vu0ScaleGM(*delta, TimeStep), time * Damp);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_stopMotion);
#ifdef SKIP_ASM
/**
 * Stops the emitter's own motion: zeroes Xi (On Tour stopMotion).
 */
void tParticleEmitter::stopMotion()
{
    Xi = t4vect_Zero;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setVelocity);
#ifdef SKIP_ASM

struct sGmFrustum_D020 {
    char pad_0x00[0x4C];
    float scale;            // 0x4C
    char pad_0x50[0x20];
    t4Vector v70;       // 0x70
    t4Vector v80;       // 0x80
    t4Vector v90;       // 0x90
    t4Vector vA0;       // 0xA0
};

extern t4Vector t4vect_XAxis;
extern t4Vector t4vect_YAxis;

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4Vector gmDivD020(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector gmAddD020(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector gmSubD020(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector gmScaleD020(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float gmLengthD020(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void gmSubFromD020(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector gmCrossD020(const t4Vector& a, const t4Vector& b)
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
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt; the result stays in $vf5).
static inline t4Vector gmNormalizeD020(const t4Vector& v)
{
    t4Vector r;
    __asm__ __volatile__(
        "lqc2      $vf3, %0\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        :
        : "m"(v));
    __asm__("sqc2      $vf5, %0\n" : "=m"(r));
    return r;
}

/**
 * Sets the emitter's velocity from a direction and a spread: a cone around @p dir.
 *
 * V0 = @p dir / Damp; with len = |V0| and n = normalize(@p dir x axis) (axis = t4vect_YAxis when dir.x != 0, else
 * t4vect_XAxis), Vr0 = n * spread * len, Vr1 = (V0 x n) * spread, Vr2 = 0, and finally V0 -= 1.5 * (Vr0 + Vr1 + Vr2),
 * so the three random terms are centred on the base velocity. On Tour's matching overload is setVelocity(const
 * t4Vector& v0, float spread). No caller here.
 *
 * @param dir  the base velocity (direction and speed)
 * @param spread  the cone spread, relative to the speed
 */
void tParticleEmitter::setVelocity(const t4Vector* dir, float spread)
{
    *(&V0) = gmDivD020(*dir, Damp);
    float len = gmLengthD020(*(&V0));
    const t4Vector* axis = (dir->x != 0.0f) ? &t4vect_YAxis : &t4vect_XAxis;
    t4Vector norm = gmNormalizeD020(gmCrossD020(*dir, *axis));
    *(&Vr0) = gmScaleD020(gmScaleD020(norm, spread), len);
    *(&Vr1) = gmScaleD020(gmCrossD020(*(&V0), norm), spread);
    Vr2 = t4vect_Zero;
    gmSubFromD020(*(&V0), gmScaleD020(gmAddD020(gmAddD020(*(&Vr0), *(&Vr1)), *(&Vr2)), 1.5f));
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setVelocity1);
#ifdef SKIP_ASM

struct sGmFrustum {
    char pad_0x00[0x4C];
    float scale;        // 0x4C
    char pad_0x50[0x20];
    t4Vector v70;        // 0x70
    t4Vector v80;        // 0x80
    t4Vector v90;        // 0x90
    t4Vector vA0;        // 0xA0
};

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4Vector gmDivD1F0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector gmAddD1F0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector gmSubD1F0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector gmScaleD1F0(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

/**
 * Sets the emitter's base velocity and its three random velocity terms.
 *
 * Each input is divided by Damp: Vr0..Vr2 = @p randVel0..2 / Damp, V0 = @p baseVel / Damp - 1.5 * (Vr0 + Vr1 + Vr2)
 * (centring the random terms). Called by cBoardSparkFX_update.
 *
 * @param baseVel  the base velocity (On Tour v0)
 * @param randVel0  random term 0 (vr0)
 * @param randVel1  random term 1 (vr1)
 * @param randVel2  random term 2 (vr2)
 */
void tParticleEmitter::setVelocity1(const t4Vector* baseVel, const t4Vector* randVel0, const t4Vector* randVel1,
                                    const t4Vector* randVel2)
{
    Vr0 = gmDivD1F0(*randVel0, Damp);
    Vr1 = gmDivD1F0(*randVel1, Damp);
    Vr2 = gmDivD1F0(*randVel2, Damp);
    V0 = gmSubD1F0(gmDivD1F0(*baseVel, Damp), gmScaleD1F0(gmAddD1F0(gmAddD1F0(Vr0, Vr1), Vr2), 1.5f));
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setColour);
#ifdef SKIP_ASM
/**
 * Sets the particles' colour over their life.
 *
 * Sets C0 to @p colStart - 1.5 * (@p colRand0 + @p colRand1), Ct to (@p colEnd - @p colStart) / (@p time * Damp), and
 * Cr0/Cr1 to the two random ranges.
 *
 * @param colStart  the start colour
 * @param colEnd  the colour after @p time
 * @param colRand0  the first random colour range
 * @param colRand1  the second random colour range
 * @param time  the time to go from start to end colour
 */
// PORT: PS2-only VU0 inline asm (vector add).
// PORT: PS2-only VU0 inline asm (vector subtract).
// PORT: PS2-only VU0 inline asm (vector times scalar).
// PORT: PS2-only VU0 inline asm (vector divided by scalar).
void tParticleEmitter::setColour(t4Vector* colStart, t4Vector* colEnd, t4Vector* colRand0, t4Vector* colRand1,
                                 float time)
{
    C0 = vu0SubGM(*colStart, vu0ScaleGM(vu0AddGM(*colRand0, *colRand1), 1.5f));
    Ct = vu0DivGM(vu0SubGM(*colEnd, *colStart), time * Damp);
    Cr0 = *colRand0;
    Cr1 = *colRand1;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setTime);
#ifdef SKIP_ASM
/**
 * Sets the emitter's end time: EndTime = @p val * Damp.
 *
 * @param val  the time in seconds (On Tour t)
 */
void tParticleEmitter::setTime(float val)
{
    EndTime = val * Damp;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_step);
#ifdef SKIP_ASM
/**
 * Advances the particle system's clock.
 *
 * Adds @p deltaTime * Damp to EndTime.
 *
 * @param deltaTime  the elapsed time
 */
void tParticleEmitter::step(float deltaTime)
{
    EndTime += deltaTime * Damp;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", vu0_advanceRandom);
#ifdef SKIP_ASM
/**
 * Advances the VU0 random generator seeded from *@p seed and stores the next value back.
 *
 * Runs vrinit then vrnext on the VU0 R register.
 *
 * @param seed  in/out: the seed
 */
// PORT: PS2-only VU0 random-number unit (R register); the PC port needs a C PRNG.
extern "C" void vu0_advanceRandom(float* seed)
{
    float rngState = *seed;
    __asm__ __volatile__(
        "qmtc2.ni  %0, $vf3\n"
        "vrinit    R, $vf3x\n"
        "vrnext.x  $vf3, R\n"
        "qmfc2.ni  %0, $vf3\n"
        : "=r"(rngState)
        : "0"(rngState));
    *seed = rngState;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_stepContinous);
#ifdef SKIP_ASM


extern "C" void tParticleEmitter_step(tParticleEmitter* self, float val);
// PORT: vu0_advanceRandom returns the new seed in $f0 (float); the unit defines it as void, so the
// float-returning view binds to the same symbol through an asm label. The unused call_value result
// is what keeps $f0 busy at the loop-exit test (product in $f1), so jump2 does not cross-jump the
// duplicated exit test.
extern "C" float vu0_advanceRandom_f(float* seed) __asm__("vu0_advanceRandom");

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (a += b, 4 floats).
static inline void gmVecAddEq_D428(t4Vector& a, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

/**
 * Steps a particle emitter by @p dt, then moves its emission point along its motion for each whole time step that has
 * passed.
 *
 * Calls tParticleEmitter_step; while NumParticles * TimeStep < EndTime it takes TimeStep off EndTime, adds Xi to X0 and
 * advances the seeds SeedX0, SeedX1, SeedV0, SeedV1, SeedV2, SeedC0, SeedC1, SeedSize and SeedLife with vu0_advanceRandom
 * (skipping Damp). Callers: cStaticEmitter_update and cBoardSparkFX_update.
 *
 * @param dt  the frame time
 */
void tParticleEmitter::stepContinous(float dt)
{
    this->step(dt);
    for (;;) {
        float step = TimeStep;
        float time = EndTime;
        if (!((float)NumParticles * step < time)) {
            break;
        }
        EndTime = time - step;
        gmVecAddEq_D428(*(&X0), *(&Xi));
        vu0_advanceRandom_f(&SeedX0);
        vu0_advanceRandom_f(&SeedX1);
        vu0_advanceRandom_f(&SeedV0);
        vu0_advanceRandom_f(&SeedV1);
        vu0_advanceRandom_f(&SeedV2);
        vu0_advanceRandom_f(&SeedC0);
        vu0_advanceRandom_f(&SeedC1);
        vu0_advanceRandom_f(&SeedSize);
        vu0_advanceRandom_f(&SeedLife);
    }
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", tParticleEmitter_calcBoundBox);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
static inline void boxAdd_D500(tBoundBox& b, const t4Vector& p)
{
    if (p.x < b.MinPoint.x) b.MinPoint.x = p.x;
    if (p.y < b.MinPoint.y) b.MinPoint.y = p.y;
    if (p.z < b.MinPoint.z) b.MinPoint.z = p.z;
    if (p.x > b.MaxPoint.x) b.MaxPoint.x = p.x;
    if (p.y > b.MaxPoint.y) b.MaxPoint.y = p.y;
    if (p.z > b.MaxPoint.z) b.MaxPoint.z = p.z;
}

static inline void boxInit_D500(tBoundBox& b, const t4Vector& p)
{
    b.MinPoint = b.MaxPoint = p;
}

// Grows box b by the four extreme trajectory points at time t: start corner + v * t + d * drag(td),
// drag(t) = t * (DRAG_A * t + DRAG_B).
#define ADD_TRAJECTORY_D500(b, start, v, d1, d2, t, td) \
    { \
        float drag = (td) * ((td) * DRAG_A_D500 + DRAG_B_D500); \
        boxAdd_D500(b, vu0AddGM(vu0AddGM(start.MinPoint, vu0ScaleGM(v, t)), vu0ScaleGM(d1, drag))); \
        boxAdd_D500(b, vu0AddGM(vu0AddGM(start.MinPoint, vu0ScaleGM(v, t)), vu0ScaleGM(d2, drag))); \
        boxAdd_D500(b, vu0AddGM(vu0AddGM(start.MaxPoint, vu0ScaleGM(v, t)), vu0ScaleGM(d1, drag))); \
        boxAdd_D500(b, vu0AddGM(vu0AddGM(start.MaxPoint, vu0ScaleGM(v, t)), vu0ScaleGM(d2, drag))); \
    }

#define DRAG_TMAX_D500 2.7f
#define DRAG_A_D500 0.113f
#define DRAG_B_D500 (-0.73f)

// Time at which the trajectory along one axis turns (d/dt = 0), v = velocity, b = velocity-box bound.
#define TURN_TIME_D500(v, b) (((v) / ((b) - (v)) - DRAG_B_D500) * (1.0f / (2.0f * DRAG_A_D500)))

// Bounding box of every particle the emitter can produce over its lifetime.
/**
 * Computes a bounding box for every particle the emitter can produce over its lifetime.
 *
 * life is the larger of Life0 + LifeR and Life0 + 2 * LifeR. The box starts as the spawn-position box (X0 + Xr0 * r1 +
 * Xr1 * r2 (+ Xi * NumParticles), r in [1, 2]); a second box bounds the velocities (V0 + Vr0 * r1 + Vr1 * r2 + Vr2 *
 * r3). Each trajectory corner is then added at time life (drag time clamped to 2.7) and at the six per-axis turning
 * times where the drag curve t * (0.113 * t - 0.73) stops the motion (On Tour's tcx0..tcz1), when positive. Callers:
 * tParticleEmitter_setupEmitter and cStaticEmitter_readFromReplayFrame (the static emitter's bounding box).
 *
 * @param bb  receives the bounding box
 */
void tParticleEmitter::calcBoundBox(tBoundBox* bb)
{
    float lifeA = Life0 + LifeR;
    float lifeB = Life0 + LifeR * 2.0f;
    float life = lifeA > lifeB ? lifeA : lifeB;

    // spawn positions: B0 + C0 * r1 + D0 * r2 (+ E0 * count), r in [1, 2]
    boxInit_D500(*bb, vu0AddGM(vu0AddGM(X0, Xr0), Xr1));
    boxAdd_D500(*bb, vu0AddGM(vu0AddGM(X0, vu0ScaleGM(Xr0, 2.0f)), Xr1));
    boxAdd_D500(*bb, vu0AddGM(vu0AddGM(X0, Xr0), vu0ScaleGM(Xr1, 2.0f)));
    boxAdd_D500(*bb, vu0AddGM(vu0AddGM(X0, vu0ScaleGM(Xr0, 2.0f)), vu0ScaleGM(Xr1, 2.0f)));
    boxAdd_D500(*bb, vu0AddGM(vu0AddGM(vu0AddGM(X0, vu0ScaleGM(Xr0, 2.0f)), Xr1),
        vu0ScaleGM(Xi, (float)NumParticles)));
    boxAdd_D500(*bb, vu0AddGM(vu0AddGM(vu0AddGM(X0, Xr0), vu0ScaleGM(Xr1, 2.0f)),
        vu0ScaleGM(Xi, (float)NumParticles)));
    boxAdd_D500(*bb, vu0AddGM(vu0AddGM(vu0AddGM(X0, vu0ScaleGM(Xr0, 2.0f)), vu0ScaleGM(Xr1, 2.0f)),
        vu0ScaleGM(Xi, (float)NumParticles)));

    tBoundBox start = *bb;

    // velocities: 70 + 80 * r1 + 90 * r2 + A0 * r3, r in [1, 2]
    tBoundBox vel;
    boxInit_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, Vr0), Vr1), Vr2));
    boxAdd_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, vu0ScaleGM(Vr0, 2.0f)), Vr1), Vr2));
    boxAdd_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, Vr0), vu0ScaleGM(Vr1, 2.0f)), Vr2));
    boxAdd_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, Vr0), Vr1), vu0ScaleGM(Vr2, 2.0f)));
    boxAdd_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, vu0ScaleGM(Vr0, 2.0f)), vu0ScaleGM(Vr1, 2.0f)),
        Vr2));
    boxAdd_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, vu0ScaleGM(Vr0, 2.0f)), Vr1),
        vu0ScaleGM(Vr2, 2.0f)));
    boxAdd_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, Vr0), vu0ScaleGM(Vr1, 2.0f)),
        vu0ScaleGM(Vr2, 2.0f)));
    boxAdd_D500(vel, vu0AddGM(vu0AddGM(vu0AddGM(V0, vu0ScaleGM(Vr0, 2.0f)), vu0ScaleGM(Vr1, 2.0f)),
        vu0ScaleGM(Vr2, 2.0f)));

    t4Vector deltaMax = vu0SubGM(Force, vel.MaxPoint);
    t4Vector deltaMin = vu0SubGM(Force, vel.MinPoint);

    ADD_TRAJECTORY_D500(*bb, start, Force, deltaMax, deltaMin, life, life <? DRAG_TMAX_D500);

    float tx0 = TURN_TIME_D500(Force.x, vel.MinPoint.x);
    float tx1 = TURN_TIME_D500(Force.x, vel.MaxPoint.x);
    float ty0 = TURN_TIME_D500(Force.y, vel.MinPoint.y);
    float ty1 = TURN_TIME_D500(Force.y, vel.MaxPoint.y);
    float tz0 = TURN_TIME_D500(Force.z, vel.MinPoint.z);
    float tz1 = TURN_TIME_D500(Force.z, vel.MaxPoint.z);

    if (tx0 > 0.0f) {
        tx0 = tx0 <? DRAG_TMAX_D500;
        ADD_TRAJECTORY_D500(*bb, start, Force, deltaMax, deltaMin, tx0, tx0);
    }
    if (tx1 > 0.0f) {
        tx1 = tx1 <? DRAG_TMAX_D500;
        ADD_TRAJECTORY_D500(*bb, start, Force, deltaMax, deltaMin, tx1, tx1);
    }
    if (ty0 > 0.0f) {
        ty0 = ty0 <? DRAG_TMAX_D500;
        ADD_TRAJECTORY_D500(*bb, start, Force, deltaMax, deltaMin, ty0, ty0);
    }
    if (ty1 > 0.0f) {
        ty1 = ty1 <? DRAG_TMAX_D500;
        ADD_TRAJECTORY_D500(*bb, start, Force, deltaMax, deltaMin, ty1, ty1);
    }
    if (tz0 > 0.0f) {
        tz0 = tz0 <? DRAG_TMAX_D500;
        ADD_TRAJECTORY_D500(*bb, start, Force, deltaMax, deltaMin, tz0, tz0);
    }
    if (tz1 > 0.0f) {
        tz1 = tz1 <? DRAG_TMAX_D500;
        ADD_TRAJECTORY_D500(*bb, start, Force, deltaMax, deltaMin, tz1, tz1);
    }
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cStaticEmitter_cStaticEmitter);
#ifdef SKIP_ASM
extern char cStaticEmitter_vtbl[];
void tParticleEmitter_tParticleEmitter(void*);

/**
 * Constructs a static particle emitter: the embedded tParticleEmitter at +0x10 (tParticleEmitter_tParticleEmitter),
 * then m_initialized 0 and mCurrentFlipTexture 0.
 */
cStaticEmitter::cStaticEmitter()
{
    m_initialized = 0;
    mCurrentFlipTexture = 0.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/graphicsman", tParticleEmitter_setupEmitter);
#ifdef SKIP_ASM
extern "C" void tParticleEmitter_setSize(tParticleEmitter* self, float a, float b, float c);
extern "C" void tParticleEmitter_setLife(tParticleEmitter* self, float a, float b);
extern "C" void tParticleEmitter_randomize(tParticleEmitter* self);
extern "C" void tParticleEmitter_stopMotion(tParticleEmitter* self);
extern "C" void tParticleEmitter_calcBoundBox(void* a, void* b);
// t4Vector with On Tour's inline constructors t4Vector(), t4Vector(float, float, float, float) and t4Vector(const
// t4vect&); the copy constructor makes the by-value returns below go through memory, as in retail.
struct t4VectorCtor : t4Vector {
    t4VectorCtor() {}
    t4VectorCtor(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
    // PORT: 128-bit integer copy (one lq/sq pair).
    t4VectorCtor(const t4VectorCtor& o) { *(cQuad128*)this = *(const cQuad128*)&o; }
};

// The unit's tParticleEmitter_setColour takes pointers; this caller passes the four corners as references to temporaries.
extern "C" void tParticleEmitter_setColour_0058(void* self, const t4Vector& a, const t4Vector& b, const t4Vector& c,
                                   const t4Vector& d, float t) __asm__("tParticleEmitter_setColour");


// PORT: PS2-only VU0 inline asm (a + b).
static inline t4VectorCtor vu0Add_0058(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
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
static inline t4VectorCtor vu0Scale_0058(const t4Vector& v, float s)
{
    t4VectorCtor r;
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

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4VectorCtor vu0Div_0058(const t4Vector& v, float s)
{
    t4VectorCtor r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (m * v).
static inline t4VectorCtor vu0ApplyGM_0058(const t4Matrix* m, const t4Vector& v)
{
    t4VectorCtor r;
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

// PORT: PS2-only VU0 inline asm (d -= b).
static inline void vu0SubInGM_0058(t4Vector& d, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(d)
        : "m"(d), "m"(b)
        : "memory");
}

/**
 * Sets a particle emitter up from particle data placed by a transform (On Tour setupEmitter(transform, particleData,
 * boundBox, seed)).
 *
 * From the tParticleData: setSize(Size, SizeFinal, SizeR) and setLife(Life -/+ LifeR / 2); a negative @p seed randomises
 * the emitter (tParticleEmitter_randomize), otherwise SeedX0 = @p seed. After stopMotion, X0 is the transformed offset (OffX.. OffZ,
 * w 1) and Xr0 / Xr1 the transformed position spreads R0 / R1, with X0 moved back by 1.5 * (Xr0 + Xr1); V0 and
 * Vr0..Vr2 are the transformed velocity and its three spreads divided by Damp, V0 moved back by 1.5 * (Vr0 + Vr1 +
 * Vr2). The start, end and two random colours (scaled by 128) go to tParticleEmitter_setColour with the maximum life, then
 * setTime(Duration < 0 ? maximum life : 0) and calcBoundBox (tParticleEmitter_calcBoundBox) into @p boundBox.
 *
 * The source comment above the function ("camera parameters ... frustum planes") does not describe this code.
 *
 * @param transform  the emitter's world transform (On Tour transform)
 * @param particleData  the tParticleData to read
 * @param boundBox  receives the emitter's bounding box
 * @param seed  the random seed; negative picks a random one
 */
void tParticleEmitter::setupEmitter(t4Matrix* transform, tParticleData* particleData, tBoundBox* boundBox, float seed)
{
    setSize(particleData->Size, particleData->SizeFinal, particleData->SizeR);
    float halfSpread = particleData->LifeR * 0.5f;
    setLife(particleData->Life - halfSpread, particleData->Life + halfSpread);
    if (seed < 0.0f) {
        randomize();
    } else {
        SeedX0 = seed;
    }
    stopMotion();
    X0 = vu0ApplyGM_0058(transform, t4VectorCtor(particleData->OffX, particleData->OffY, particleData->OffZ,
        1.0f));
    Xr0 = vu0ApplyGM_0058(transform, t4VectorCtor(particleData->R0X, particleData->R0Y, particleData->R0Z, 0.0f));
    Xr1 = vu0ApplyGM_0058(transform, t4VectorCtor(particleData->R1X, particleData->R1Y, particleData->R1Z, 0.0f));
    vu0SubInGM_0058(X0, vu0Scale_0058(vu0Add_0058(Xr0, Xr1), 1.5f));
    V0 = vu0Div_0058(vu0ApplyGM_0058(transform, t4VectorCtor(particleData->VelX, particleData->VelY,
        particleData->VelZ, 0.0f)), particleData->Damp);
    Vr0 = vu0Div_0058(vu0ApplyGM_0058(transform, t4VectorCtor(particleData->R0VX, particleData->R0VY,
        particleData->R0VZ, 0.0f)), particleData->Damp);
    Vr1 = vu0Div_0058(vu0ApplyGM_0058(transform, t4VectorCtor(particleData->R1VX, particleData->R1VY,
        particleData->R1VZ, 0.0f)), particleData->Damp);
    Vr2 = vu0Div_0058(vu0ApplyGM_0058(transform, t4VectorCtor(particleData->R2VX, particleData->R2VY,
        particleData->R2VZ, 0.0f)), particleData->Damp);
    vu0SubInGM_0058(V0, vu0Scale_0058(vu0Add_0058(vu0Add_0058(Vr0, Vr1), Vr2), 1.5f));
    tParticleEmitter_setColour_0058(this,
                       vu0Scale_0058(t4VectorCtor(particleData->StartColR, particleData->StartColG,
                           particleData->StartColB, particleData->StartColA), 128.0f),
                       vu0Scale_0058(t4VectorCtor(particleData->EndColR, particleData->EndColG, particleData->EndColB,
                           particleData->EndColA), 128.0f),
                       vu0Scale_0058(t4VectorCtor(particleData->R0R, particleData->R0G, particleData->R0B,
                           particleData->R0A), 128.0f),
                       vu0Scale_0058(t4VectorCtor(particleData->R1R, particleData->R1G, particleData->R1B,
                           particleData->R1A), 128.0f),
                       particleData->Life + particleData->LifeR * 0.5f);
    setTime(particleData->Duration < 0.0f ? particleData->Life + particleData->LifeR * 0.5f : 0.0f);
    tParticleEmitter_calcBoundBox(this, boundBox);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/graphicsman", cStaticEmitter_setupEmitter);
#ifdef SKIP_ASM
extern "C" void tParticleEmitter_setupEmitter(tParticleEmitter* gfx, t4Matrix* a1, tParticleData* desc, tBoundBox* out, float f);

/**
 * Sets a static emitter up from particle data (On Tour setupEmitter(const t4Matrix& transform, const tParticleData*
 * particleData)).
 *
 * Copies NumFlipTextures, FlipTextureRate, TextureId and BlendMode; Duration is the data's Duration when negative
 * (endless), otherwise Duration + Life + LifeR / 2. Runs tParticleEmitter_init on the embedded emitter (+0x10) with
 * NumParticles, NumBlur, the duration (Life + LifeR / 2 for an endless effect), BlurStep, Damp and the force, then
 * tParticleEmitter_setupEmitter with a random seed (-1), writing the bounding box at +0x160.
 *
 * @param transform  the emitter's world transform
 * @param particleData  the tParticleData to read
 */
void cStaticEmitter::setupEmitter(t4Matrix* transform, tParticleData* particleData)
{
    mNumFlipTextures = particleData->NumFlipTextures;
    mFlipTextureRate = particleData->FlipTextureRate;
    TextureId = particleData->TextureId;
    BlendMode = particleData->BlendMode;
    float dataDuration = particleData->Duration;
    float totalDuration;
    if (dataDuration < 0.0f) {
        totalDuration = dataDuration;
    } else {
        totalDuration = dataDuration + particleData->Life + particleData->LifeR * 0.5f;
    }
    Duration = totalDuration;
    float x = particleData->Duration >= 0.0f ? particleData->Duration : particleData->Life + particleData->LifeR * 0.5f;
    t4Vector force;
    force.x = particleData->ForceX;
    force.y = particleData->ForceY;
    force.z = particleData->ForceZ;
    force.w = 0.0f;
    Emitter.init(particleData->NumParticles, particleData->NumBlur, x, particleData->BlurStep, particleData->Damp,
            &force);
    Emitter.setupEmitter(transform, particleData, (tBoundBox*)(&BoundBox), -1.0f);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cStaticEmitter_setTime);
#ifdef SKIP_ASM
extern "C" void tParticleEmitter_setTime(tParticleEmitter*, float);

/**
 * Sets the emitter's Duration to @p duration (unless it is 0) and its particle system's time with
 * tParticleEmitter_setTime.
 *
 * A zero @p duration leaves Duration as it is and calls tParticleEmitter_setTime(&Emitter, 0).
 *
 * @param duration  the new duration; 0 resets only the particle time
 */
void cStaticEmitter::setTime(float duration)
{
    if (duration == 0.0f) {
        Emitter.setTime(0.0f);
        return;
    }
    Duration = duration;
    Emitter.setTime(duration);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", _$_14cStaticEmitter);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cStaticEmitter_vtbl[];

/**
 * Destroys a static particle emitter; it owns nothing, so this only frees the object when asked.
 *
 * ~cParticleModifier destroys its embedded emitter (+0x50, On Tour cParticleModifier::Emitter) in place.
 */
cStaticEmitter::~cStaticEmitter()
{
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/graphicsman", cStaticEmitter_update);
#ifdef SKIP_ASM
extern "C" void tParticleEmitter_stepContinous(void* self, float val);

/**
 * Advances a static particle emitter: emits while its timer runs (or forever when negative) and steps its
 * texture animation.
 *
 * The timer counts down by one app tick; at 0 the emitter stops. The animation frame advances by rate times
 * @p dt
 * and wraps at the frame count. Marks the emitter updated (+0xC).
 *
 * @param dt  time step in seconds
 */
void cStaticEmitter::update(float dt)
{
    float timer = Duration;
    if (timer == 0.0f) {
        return;
    }
    if (timer < 0.0f) {
        tParticleEmitter_stepContinous(&Emitter, 1.0f / (float)cAppMan_TheAppMan->TimerHz);
    } else {
        Emitter.step(1.0f / (float)cAppMan_TheAppMan->TimerHz);
        Duration -= 1.0f / (float)cAppMan_TheAppMan->TimerHz;
        if (Duration < 0.0f) {
            Duration = 0.0f;
        }
    }
    float animFrame = mCurrentFlipTexture + mFlipTextureRate * dt;
    mCurrentFlipTexture = animFrame;
    if ((int)animFrame >= mNumFlipTextures) {
        mCurrentFlipTexture = 0.0f;
    }
    m_initialized = 1;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cStaticEmitter_render);
#ifdef SKIP_ASM


extern "C" void cStaticEmitter_render1(cStaticEmitter* self);

/**
 * Stores the emitter's bounding box and position, then renders it.
 *
 * @param bbox  bounding box to cull against (On Tour const tBoundBox&)
 * @param pos  emitter position (On Tour const t4Vector&)
 */
void cStaticEmitter::render(const tBoundBox* bbox, const t4Vector* pos)
{
    *(&BoundBox) = *bbox;
    Emitter.X0 = *pos;
    render1();
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cStaticEmitter_render1);
#ifdef SKIP_ASM
extern int gBlendTable[];

// The graphics-manager slots render1 calls by hand: 43 GetNetMatrix, 93 ClipBoundBox, 82 RenderParticleEmitter (real
// virtual calls keep the slot address live, retail reads the vtable entries directly).
typedef sVEntry<t4Matrix(void*)> tNetMatrixSlot;
typedef sVEntry<int(void*, const t4Vector*, const t4Vector*, const t4Matrix&)> tClipBoundBoxSlot;
typedef sVEntry<void(void*, const tParticleEmitter*)> tRenderEmitterSlot;

/**
 * Draws a static emitter's particles with its texture-animation frame, unless it is culled or hidden.
 *
 * Culls BoundBox against the net matrix (GrMan slots 93 ClipBoundBox and 43 GetNetMatrix), then pushes a render state
 * with the layer for its kind (gBlendTable) and blend flags, draws through GrMan slot 82 (RenderParticleEmitter) and
 * pops the state.
 *
 */
void cStaticEmitter::render1()
{
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    tClipBoundBoxSlot* vt = (tClipBoundBoxSlot*)VPTR_OF(cGraphicsMan, gfx);
    tNetMatrixSlot* vtMat = (tNetMatrixSlot*)vt;
    if (vt[93].fn(vthis(gfx, vt[93]), &BoundBox.MinPoint, &BoundBox.MaxPoint,
                  vtMat[43].fn(vthis(gfx, vtMat[43]))) == GM_CLIPCODE_OUTSIDE) {
        return;
    }
    if (m_initialized == 0) {
        return;
    }
    int layer = gBlendTable[BlendMode];
    gfx->m_pCurrentDrawState[1] = gfx->m_pCurrentDrawState[0];
    sDrawState* prev = gfx->m_pCurrentDrawState;
    gfx->m_pCurrentDrawState = prev + 1;
    prev[1].m_bitGroups[2] = ((int)prev[1].m_bitGroups[2] & ~0x3E0) | 0xE0;
    gfx->m_pCurrentDrawState->m_bitGroups[1] = ((int)gfx->m_pCurrentDrawState->m_bitGroups[1] & 0xFFBFFFFF) | 0x400000;
    gfx->m_pCurrentDrawState->m_bitGroups[1] = (int)gfx->m_pCurrentDrawState->m_bitGroups[1] & 0xFE7FFFFF;
    gfx->m_pCurrentDrawState->m_bitGroups[1] = (int)gfx->m_pCurrentDrawState->m_bitGroups[1] & 0xFFCFFFFF;
    gfx->m_pCurrentDrawState->m_bitGroups[1] = ((int)gfx->m_pCurrentDrawState->m_bitGroups[1] & 0xFFF00FFF) | 0x14000;
    gfx->m_pCurrentDrawState->m_bitGroups[1] = ((int)gfx->m_pCurrentDrawState->m_bitGroups[1] & ~0x7C)
        | ((layer << 2) & 0x7C);
    *gfx->m_pCurrentDrawState->m_textureState.m_texid =
        cGraphicsMan_TheGraphicsMan->getEffectTexMap(TextureId + (int)mCurrentFlipTexture);
    tRenderEmitterSlot* vtDraw = (tRenderEmitterSlot*)VPTR_OF(cGraphicsMan, gfx);
    vtDraw[82].fn(vthis(gfx, vtDraw[82]), &Emitter);
    gfx->m_pCurrentDrawState--;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cStaticEmitter_writeToReplayFrame);
#ifdef SKIP_ASM
struct sGmSerVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sGmSerObj {
    sGmSerVEntry* vt;
};

struct sGmSer12 {
    float a;
    float b;
    int c;
};

/**
 * Writes a static emitter's replay state as one 12-byte record.
 *
 * Packs the float at offset 0 (the emitter's remaining time, which cParticleModifier_update watches), the float
 * at 0x18 and the int at 0xC, and writes them through the stream's write virtual (slot 1).
 *
 * @param out  replay stream to write to
 */
void cStaticEmitter::writeToReplayFrame(cBxReplayFrame* out)
{
    sGmSer12 buf;
    buf.a = Duration;
    buf.b = Emitter.EndTime;
    buf.c = m_initialized;
    out->write((const char*)&buf, 12);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/graphicsman", cStaticEmitter_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void tParticleEmitter_calcBoundBox(void* a, void* b);

/**
 * Restores a static emitter from a replay frame: reads 12 bytes (Duration, the emitter's EndTime, m_initialized) with
 * the stream's slot 2 and recomputes the bounding box at +0x160 (tParticleEmitter_calcBoundBox, calcBoundBox).
 *
 * @param stream  the replay frame to read from (On Tour data)
 */
void cStaticEmitter::readFromReplayFrame(cBxReplayFrame* stream)
{
    struct {
        float a;
        float b;
        int c;
    } buf;
    stream->read((char*)&buf, 0xC);
    Duration = buf.a;
    Emitter.EndTime = buf.b;
    tParticleEmitter_calcBoundBox((char*)&Emitter, (char*)&BoundBox);
    m_initialized = buf.c;
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter);
#ifdef SKIP_ASM
void tParticleEmitter_tParticleEmitter(void*);
extern "C" void* memset(void* dst, int c, int n);
extern void* cBaseClass_DynamicEmitter_vtbl[];


/**
 * Constructs the dynamic particle emitter base: no flip textures, cBaseClass_DynamicEmitter_vtbl, the embedded
 * tParticleEmitter at +0x20 (tParticleEmitter_tParticleEmitter), m_initialized 0, m_enabled 1, m_numSubEmitters -1, m_subEmitterId 0, no
 * position or velocity buffers, m_enabledCount 0, and a draw state whose 16 flag bytes are zeroed (memset) and whose
 * two texture ids (m_textureState, +0x10) are -1.
 *
 * @return @p self
 */
extern "C" cBaseClass_DynamicEmitter* cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter(cBaseClass_DynamicEmitter* self)
{
    self->mNumFlipTextures = 0;
    self->mCurrentFlipTexture = 0.0f;
    self->mFlipTextureRate = 0.0f;
    self->mVtbl = cBaseClass_DynamicEmitter_vtbl;
    tParticleEmitter_tParticleEmitter((char*)&self->m_emitter);
    self->m_initialized = 0;
    self->m_enabled = 1;
    self->m_numSubEmitters = -1;
    self->m_subEmitterId = 0;
    self->m_positions = 0;
    self->m_velocities = 0;
    self->m_enabledCount = 0;
    char* blk = ((char*)&self->mDrawState);
    for (unsigned int i = 0; i < 2; i++) {
        self->mDrawState.m_textureState.m_texid[i] = -1;
    }
    memset(blk, 0, 0x10);
    return (self);
}
#endif

//100%
INCLUDE_ASM("render/graphicsman", cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter);
#ifdef SKIP_ASM
extern "C" void cBaseClass_DynamicEmitter_purge(void* self, int flag);
void operator_delete(int* ptr);
extern void* cBaseClass_DynamicEmitter_vtbl[];

/**
 * Destroys a dynamic emitter: restores its base vtable and releases its particle buffers.
 *
 * cBaseClass_DynamicEmitter_purge(self, 1) frees the two blocks at offsets 0x1A0 and 0x1A4 and resets the emitter.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(cBaseClass_DynamicEmitter* self, int flags)
{
    self->mVtbl = cBaseClass_DynamicEmitter_vtbl;
    cBaseClass_DynamicEmitter_purge(self, 1);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

