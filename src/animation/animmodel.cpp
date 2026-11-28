#include "common.h"
#include "animation/mdfarchive.h"
#include "animation/animdatabase.h"
#include "vtable.h"
#include "animation/adfdatasource.h"
#include "render/ps2graphicsman.h"
#include "animation/animmodel.h"
#include "replay/bxreplayframe.h"
#include "render/graphicsman.h"

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_addModelPartLOD);
#ifdef SKIP_ASM
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
// as operator new[] so the new-expressions below compute their destination before the call.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_ModelVariant[];
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Adds a model part (an MDF part header plus its data) to the model at one LOD and variant slot.
 *
 * Parts are matched by the header's signed byte at +0x52 (the part id). An existing part gets that LOD marked
 * used and refs[index] set to the header and its data (+0x60). A new part first gets four zeroed
 * "ModelVariant" ref arrays of numLodRefs entries, then its bone names, two more data tables, bone count, two
 * shorts from the header and refIndex = @p index; numParts is not bounds-checked.
 *
 * @param lod  LOD slot, 0-3
 * @param part  the part header (On Tour: tMdfModelHeader*), its data following at +0x60
 * @param index  variant slot in the LOD's ref array (On Tour: variant)
 */
void cAnimModel::addModelPartLOD(int lod, char* part, int index)
{
    char* data = part + 0x60;
    for (int i = 0; i < numParts; i++) {
        if (parts[i].id == *(signed char*)(part + 0x52)) {
            parts[i].lods[lod].used = 1;
            parts[i].lods[lod].refs[index].part = part;
            parts[i].lods[lod].refs[index].data = data;
            return;
        }
    }
    for (int k = 0; k < 4; k++) {
        parts[numParts].lods[k].refs = new (str_ModelVariant, 0, 0) sAnimModelLodRef[numLodRefs];
        MEM_fill(parts[numParts].lods[k].refs, 0, numLodRefs << 4);
    }
    parts[numParts].id = *(signed char*)(part + 0x52);
    parts[numParts].lods[lod].used = 1;
    parts[numParts].lods[lod].refs[index].part = part;
    parts[numParts].lods[lod].refs[index].data = data;
    parts[numParts].boneNames = data + *(int*)(part + 0x18);
    parts[numParts].unk_40 = data + *(int*)(part + 0x2C);
    parts[numParts].HotSpots = data + *(int*)(part + 0x1C);
    parts[numParts].numBones = *(short*)(part + 0x4A);
    parts[numParts].unk_48 = *(short*)(part + 0x4E);
    parts[numParts].unk_4C = *(short*)(part + 0x50);
    parts[numParts].refIndex = index;
    numParts++;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_addTextureFile__FPvi);
#ifdef SKIP_ASM
struct sAnimModelList {
    char pad_0x00[0x60];
    int count;     // 0x60
    int items[1];  // 0x64
};

/**
 * Appends a texture file to the model's list: TextureFiles[NumTextureFiles++] = @p item (On Tour's count and 50-entry
 * array, here at +0x60 and +0x64, just before NumTextureRefs; no bounds check).
 *
 * cRider_loadRider calls it on the rider's mAnimModel for each 0x108-byte entry of its load list (mLoadList), passing
 * the entry's word at +0x104, then calls cAnimModel_compile.
 *
 * @param item  the texture file to add
 */
void cAnimModel::addTextureFile(int item)
{
    sAnimModelList* list = (sAnimModelList*)this;
    list->items[list->count++] = item;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", compareAnimModelPartIds);
#ifdef SKIP_ASM
/**
 * Compares two records by their first int: returns *self - *other (qsort-style comparator).
 *
 * cAnimModel_compile passes it to qsort to sort the model's 0x58-byte parts by sAnimModelPart::id.
 *
 * @param other  the second record
 * @return negative, zero or positive as the first int of @p self is below, equal to or above that of @p other
 */
extern "C" int compareAnimModelPartIds(void* self, void* other)
{
    return *(int*)self - *(int*)other;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_setScale);
#ifdef SKIP_ASM
/**
 * Sets the model's scale vector unk_140 to (length, width, width, 1).
 *
 * cAnimModel_transform() scales the local bone positions by it, the skeleton build scales the second matrix
 * set by it and cAnimModel_render() passes its y to the draw call.
 *
 * @param lengthScale  scale along x
 * @param widthScale  scale along y and z
 */
void cAnimModel::setScale(float lengthScale, float widthScale)
{
    t4Vector scale;
    scale.x = lengthScale;
    scale.y = widthScale;
    scale.z = widthScale;
    scale.w = 1.0f;
    ScaleVect = scale;
}
#endif

INCLUDE_ASM("animation/animmodel", cAnimModel_compile);

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_enablePart);
#ifdef SKIP_ASM
/**
 * Enables model part number @p i: ORs the part's mask into the model's mask and marks LOD slot @p channel used.
 *
 * Does nothing when @p i > maxPartIndex, partIndex is null or partIndex[i] is negative (no such part). A negative
 * @p channel marks all four lods[] slots used. cAnimModel_disablePart is the reverse; ai/rider uses both to show and hide parts.
 *
 * @param i  the part number (index into partIndex)
 * @param channel  the LOD slot 0-3 to enable, or negative for all four
 */
// PORT: ulong is 64-bit here
void cAnimModel::enablePart(int i, int channel)
{
    if (i > maxPartIndex)
    {
        return;
    }
    if (partIndex == 0)
    {
        return;
    }
    int idx = partIndex[i];
    if (idx < 0)
    {
        return;
    }
    sAnimModelPart* animModelPart = &parts[idx];
    mask |= animModelPart->mask;
    if (channel < 0)
    {
        for (int k = 0; k < 4; k++)
        {
            animModelPart->lods[k].used = 1;
        }
    }
    else
    {
        animModelPart->lods[channel].used = 1;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_disablePart);
#ifdef SKIP_ASM
/**
 * Disables model part number @p i: clears the part's mask bits from the model's mask and its LOD slot @p channel.
 *
 * Does nothing when @p i > maxPartIndex, partIndex is null or partIndex[i] is negative. A negative @p channel clears
 * used on all four lods[] slots; a single slot is cleared alone, and if any other slot of the part is still used the
 * part's mask is OR'd back into the model's mask. Reverse of cAnimModel_enablePart.
 *
 * @param i  the part number (index into partIndex)
 * @param channel  the LOD slot 0-3 to disable, or negative for all four
 */
// PORT: ulong is 64-bit here
void cAnimModel::disablePart(int i, int channel)
{
    if (i > maxPartIndex)
    {
        return;
    }
    if (partIndex == 0)
    {
        return;
    }
    int idx = partIndex[i];
    if (idx < 0)
    {
        return;
    }
    sAnimModelPart* animModelPart = &parts[idx];
    mask &= ~animModelPart->mask;
    if (channel < 0)
    {
        for (int k = 0; k < 4; k++)
        {
            animModelPart->lods[k].used = 0;
        }
    }
    else
    {
        animModelPart->lods[channel].used = 0;
        for (int k = 0; k < 4; k++)
        {
            if (animModelPart->lods[k].used)
            {
                mask |= animModelPart->mask;
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_disableAllParts);
#ifdef SKIP_ASM
/**
 * Disables every part of the model: clears used on all four lods[] slots of each of the numParts parts and zeroes mask.
 */
// PORT: ulong is 64-bit here
void cAnimModel::disableAllParts()
{
    for (int j = 0; j < numParts; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            parts[j].lods[k].used = 0;
        }
    }
    mask = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_isPartEnabled);
#ifdef SKIP_ASM
/**
 * Tells whether a model part is drawn at a given level of detail.
 *
 * @param i  the part id, looked up through partIndex (On Tour partID)
 * @param channel  the level of detail, 0-3 (On Tour lod)
 * @return the part's lods[channel].used flag, or 0 when the id is above maxPartIndex, there is no partIndex table or
 *         the id maps to no part
 */
int cAnimModel::isPartEnabled(int i, int channel)
{
    int idx;
    if (i > maxPartIndex || partIndex == 0 || (idx = partIndex[i]) < 0)
    {
        return 0;
    }
    return parts[idx].lods[channel].used;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_setModelPartVariant);
#ifdef SKIP_ASM
/**
 * Picks which variant a model part shows and enables it at every level of detail.
 *
 * Ignores ids above maxPartIndex, a missing partIndex table or an unmapped id. Otherwise stores the variant in the
 * part's refIndex, ORs the part's mask into the model's mask and sets used on all four lods.
 *
 * @param i  the part id, looked up through partIndex (On Tour partID)
 * @param variant  the variant index (On Tour variant)
 */
// PORT: ulong is 64-bit here
void cAnimModel::setModelPartVariant(int i, int variant)
{
    if (i > maxPartIndex)
    {
        return;
    }
    if (partIndex == 0)
    {
        return;
    }
    int idx = partIndex[i];
    if (idx < 0)
    {
        return;
    }
    sAnimModelPart* animModelPart = &parts[idx];
    animModelPart->refIndex = variant;
    mask |= animModelPart->mask;
    for (int k = 0; k < 4; k++)
    {
        animModelPart->lods[k].used = 1;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_addAnimMinPart);
#ifdef SKIP_ASM
/**
 * Adds a part's bone mask to the model's second part mask (unk_158).
 *
 * The part number goes through partIndex without a range check. On Tour calls this addAnimMinPart, and its
 * blend() takes an animMinParts flag, so this is probably the set of parts blended in that mode.
 *
 * @param i  part number (On Tour: partID), mapped through partIndex
 */
// PORT: ulong is 64-bit here
void cAnimModel::addAnimMinPart(int i)
{
    sAnimModelPart* animModelPart = &parts[partIndex[i]];
    AnimMinPartsMask |= animModelPart->mask;
}
#endif

INCLUDE_ASM("animation/animmodel", func_0030ECD8);

INCLUDE_ASM("animation/animmodel", func_0030F2B0);

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_setSkelMatrix);
#ifdef SKIP_ASM
/**
 * Builds one bone's skeleton matrices from its pose with VU0 macro code.
 *
 * Converts the pose's rotation quaternion (+0x00) into rows 0-2 and its position (+0x10) into row 3 of unk_030[@p i],
 * then writes unk_034[@p i] as that matrix with each row scaled by the matching component of the model's +0x140 scale
 * vector.
 *
 * @param i  the bone index (On Tour boneidx)
 * @param animModelBone  the bone's pose: rotation quaternion then position (On Tour const tPose& pose)
 */
// PORT: PS2-only VU0 macro-mode asm. Builds bone matrix unk_030[i] from the bone's quaternion
// (rot, rows 0-2) and position (pos, row 3), then unk_034[i] = unk_030[i] with each row scaled
// by the matching component of unk_140.
// PORT: index-first (int) address arithmetic (pointer in int).
void cAnimModel::setSkelMatrix(int i, sAnimModelBone* animModelBone)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2       $vf4, 0x10(%0)\n"
        "lqc2       $vf3, 0x0(%0)\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf3, 0x30(%1)\n"
        "sqc2       $vf10, 0x0(%1)\n"
        "sqc2       $vf11, 0x10(%1)\n"
        "sqc2       $vf12, 0x20(%1)\n"
        ".set pop\n"
        :
        : "r"(animModelBone), "r"((t4Matrix*)((i << 6) + (int)SkelMatrices))
        : "memory");
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2       $vf1, 0x140(%2)\n"
        "lqc2       $vf4, 0x0(%0)\n"
        "lqc2       $vf5, 0x10(%0)\n"
        "lqc2       $vf6, 0x20(%0)\n"
        "lqc2       $vf7, 0x30(%0)\n"
        "vmulx.xyzw $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw $vf11, $vf7, $vf1w\n"
        "sqc2       $vf8, 0x0(%1)\n"
        "sqc2       $vf9, 0x10(%1)\n"
        "sqc2       $vf10, 0x20(%1)\n"
        "sqc2       $vf11, 0x30(%1)\n"
        ".set pop\n"
        :
        : "r"((t4Matrix*)((i << 6) + (int)SkelMatrices)), "r"((t4Matrix*)((i << 6) + (int)SkelMatricesScaled)),
                "r"(this)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_transform);
#ifdef SKIP_ASM
// Vector/quaternion with constructors (the result temps go straight into the return slot).
struct sAmQuat_0200
{
    float x, y, z, w;
    sAmQuat_0200() {}
    sAmQuat_0200(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));


extern t4Vector t4vect_Origin;

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sAmQuat_0200 amQuatMul_0200(const sAmQuat_0200& a, const sAmQuat_0200& b)
{
    sAmQuat_0200 r;
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline sAmQuat_0200 amSub_0200(const sAmQuat_0200& a, const sAmQuat_0200& b)
{
    sAmQuat_0200 r;
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
static inline sAmQuat_0200 amAdd_0200(const sAmQuat_0200& a, const sAmQuat_0200& b)
{
    sAmQuat_0200 r;
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

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sAmQuat_0200 amRot_0200(const sAmQuat_0200& q, const sAmQuat_0200& v)
{
    sAmQuat_0200 r;
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

// Child transform = parent transform a composed with local transform b.
static inline sAnimModelBone amCombine_0200(const sAnimModelBone& a, const sAnimModelBone& b)
{
    sAnimModelBone r;
    *(sAmQuat_0200*)&r.rot = amQuatMul_0200(*(sAmQuat_0200*)&a.rot, *(sAmQuat_0200*)&b.rot);
    *(sAmQuat_0200*)&r.pos = amAdd_0200(amRot_0200(*(sAmQuat_0200*)&a.rot, amSub_0200(*(sAmQuat_0200*)&b.pos, *(const sAmQuat_0200*)&t4vect_Origin)), *(sAmQuat_0200*)&a.pos);
    return r;
}


/**
 * Rebuilds every bone's world transform from the model's local pose.
 *
 * For each part's bones in order: the parent is bones[parent index], or the next entry of @p roots for a root
 * bone; the local position is localPos[k] scaled componentwise by unk_140 and the local rotation is
 * localRot[k]; bones[k] = parent combined with local (rotate, then translate).
 *
 * @param roots  root transforms (position, rotation), one per root bone in part order (On Tour also passes
 *               their count)
 */
// Rebuilds the world bones from the local pose: bone k = parent (or the next root transform) * local k.
void cAnimModel::transform(sAnimModelBone* roots)
{
    for (int i = 0; i < numParts; i++)
    {
        // PORT: pointer arithmetic through int (offset-first addu).
        tMdfBone* info = (tMdfBone*)((sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)parts))->boneNames;
        for (int j = 0; j < ((sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)parts))->numBones; j++)
        {
            int k = j + ((sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)parts))->firstBone;
            sAnimModelBone* parent;
            tMdfBone* boneInfo = &info[j];
            short parentIndex = boneInfo->mParentId;
            if (parentIndex < 0)
                parent = roots++;
            else
                parent = &bones[parentIndex];
            t4Vector* lp = (t4Vector*)((k << 4) + (int)localPos);
            sAnimModelBone local;
            *(sAmQuat_0200*)&local.pos = sAmQuat_0200(lp->x * ScaleVect.x, lp->y * ScaleVect.y, lp->z * ScaleVect.z,
                    lp->w * ScaleVect.w);
            *(sAmQuat_0200*)&local.rot = *(sAmQuat_0200*)((k << 4) + (int)localRot);
            *(sAnimModelBone*)((k << 5) + (int)bones) = amCombine_0200(*parent, local);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animmodel", cAnimModel_buildSkelMatrices);
#ifdef SKIP_ASM
extern "C" void cAnimModel_setSkelMatrix(cAnimModel* self, int i, sAnimModelBone* p);

/**
 * Builds the skeleton matrices of every bone from its world transform.
 *
 * cAnimModel_setSkelMatrix writes unk_030[i] from the bone's rotation and position, and unk_034[i] as that matrix with
 * its rows scaled by unk_140 (probably On Tour's getSkelMatrices and getSkelMatricesScaled).
 */
void cAnimModel::buildSkelMatrices()
{
    int i;
    for (i = 0; i < numBones; i++)
    {
        this->setSkelMatrix(i, &bones[i]);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_transformHotSpot);
#ifdef SKIP_ASM
extern t4Vector t4vect_Origin;

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline t4Vector amMulMat_0458(const t4Matrix* m, const t4Vector& v)
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


/**
 * Returns point @p index of the part whose id is @p id, transformed by its bone matrix, or t4vect_Origin if none.
 *
 * The part's point table (unk_3C) holds 16-byte entries: x, y, z and a bone number relative to the part's firstBone.
 * The point (x, y, z, 1) is multiplied on VU0 by the matrix of bone firstBone + bone in unk_034.
 *
 * @param model  the animated model
 * @param id  the sAnimModelPart::id to look for
 * @param index  the point's index in the part's table
 * @return the transformed point, or t4vect_Origin when no part has @p id
 */
// Returns point `index` of the part with this id, transformed by its bone matrix (unk_034),
// or t4vect_Origin when no part has the id.
extern "C" t4Vector cAnimModel_transformHotSpot(cAnimModel* model, int id, int index)
{
    for (int i = 0; i < model->numParts; i++)
    {
        // PORT: pointer arithmetic through int (offset-first addu).
        sAnimModelPart* part = (sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)model->parts);
        if (part->id == id)
        {
            tMdfHotSpot* pt = (tMdfHotSpot*)((index << 4) + (int)part->HotSpots);
            t4Matrix* boneMatrix = (t4Matrix*)(((pt->mBoneId + part->firstBone) << 6) + (int)model->SkelMatricesScaled);
            t4Vector localPoint;
            localPoint.x = pt->mOffset[0];
            localPoint.y = pt->mOffset[1];
            localPoint.z = pt->mOffset[2];
            localPoint.w = 1.0f;
            return amMulMat_0458(boneMatrix, localPoint);
        }
    }
    return t4vect_Origin;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_applyOffset);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void amVecAddEq(t4Vector* dst, t4Vector* v)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(*dst)
        : "m"(*dst), "m"(*v));
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector amVecAdd(const t4Vector& src, t4Vector* b)
{
    t4Vector a = src;
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(*b)
        : "memory");
    return r;
}

/**
 * Moves the whole posed model by an offset.
 *
 * Adds @p offset to every part bone's position and to row 3 (the translation) of its unk_030 and unk_034
 * matrices.
 *
 * @param offset  translation to add
 */
void cAnimModel::applyOffset(t4Vector* offset)
{
    for (int i = 0; i < numParts; i++) {
        for (int j = 0; j < parts[i].numBones; j++) {
            int k = j + parts[i].firstBone;
            amVecAddEq(&bones[k].pos, offset);
            {
                t4Vector shiftedPos = amVecAdd(SkelMatrices[k].row[3], offset);
                SkelMatrices[k].row[3] = shiftedPos;
            }
            {
                t4Vector shiftedPos = amVecAdd(SkelMatricesScaled[k].row[3], offset);
                SkelMatricesScaled[k].row[3] = shiftedPos;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_render);
#ifdef SKIP_ASM
struct sAmMat_0640;
// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_0640(void* d, const void* s)
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

struct sAmMat_0640 {
    float m[16];
    sAmMat_0640() {}
    sAmMat_0640(const sAmMat_0640& s) { vu0CopyMat_0640(this, &s); }
    sAmMat_0640& operator=(const sAmMat_0640& s)
    {
        vu0CopyMat_0640(this, &s);
        return *this;
    }
    // placement array new for the one-time construction of the matrix palette.
    void* operator new[](unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));


// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_0640(sAmMat_0640* d, const sAmMat_0640* a, const sAmMat_0640* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
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
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

static inline sAmMat_0640 mulMat_0640(const sAmMat_0640& a, const sAmMat_0640& b)
{
    sAmMat_0640 t;
    vu0MulMat_0640(&t, &a, &b);
    return t;
}

struct sAmVEnt_0640 {
    short delta;
    short index;
    void* fn;
};

struct sAmCtx_0640 {
    char pad[0x10D8];
    sAmVEnt_0640* vt;           // 0x10D8
};

typedef void (*tSetMats_0640)(void*, sAmMat_0640*, int, int, int, int);
typedef void (*tDraw_0640)(void*, char*, char*, int, int, float);

extern int D_004A5948;
extern sAmMat_0640 D_004FC420[];

/**
 * Draws the model at one LOD with its skinning palette.
 *
 * Builds the static 64-matrix palette D_004FC420 (constructed once) as unk_034[i] times unk_038[i]; the bone
 * count is not checked against 64. With per-part refs (unk_018 set), each used part's current variant ref
 * gets a set-matrices call (graphics virtual slot 111) and a draw (slot 99); otherwise one slot-111 call uses
 * lodInfo[lod] and each used part's first ref is drawn. Each draw also gets unk_140.y.
 *
 * @param lod  LOD slot, 0-3
 * @param x  draw flag for the board part (id sAnimModelPart::kPartBoard; On Tour: additiveBoard)
 * @param y  draw flag for every other part (On Tour: additivePlayer)
 */
void cAnimModel::render(int lod, int x, int y)
{
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (D_004A5948 == 0) {
        new ((void*)D_004FC420) sAmMat_0640[64];
        D_004A5948 = 1;
    }
    for (int i = 0; i < numBones; i++) {
        D_004FC420[i] = mulMat_0640(((sAmMat_0640*)SkelMatricesScaled)[i], ((sAmMat_0640*)InvDefaultMatrices)[i]);
    }
    if (unk_018 != 0) {
        for (int j = 0; j < numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = &animModelPart->lods[lod].refs[animModelPart->refIndex];
                if (animModelLodRef->part != 0) {
                    ctx->GenerateBlendedMatrices((const t4Matrix*)D_004FC420, 0, (const tMdfBlendedMatrix*)animModelLodRef->unk_0C, animModelLodRef->unk_08, 0);
                    int additive = parts[j].id == sAnimModelPart::kPartBoard ? x : y;
                    ctx->DrawCharModel((const tMdfModelHeader*)animModelLodRef->part, animModelLodRef->data,
                            ScaleVect.y, additive, 0);
                }
            }
        }
    } else {
        ctx->GenerateBlendedMatrices((const t4Matrix*)D_004FC420, 0, (const tMdfBlendedMatrix*)lodInfo[lod].unk_04,
                lodInfo[lod].unk_00, 0);
        for (int j = 0; j < numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = animModelPart->lods[lod].refs;
                if (animModelLodRef->part != 0) {
                    int additive = animModelPart->id == sAnimModelPart::kPartBoard ? x : y;
                    ctx->DrawCharModel((const tMdfModelHeader*)animModelLodRef->part, animModelLodRef->data,
                            ScaleVect.y, additive, 0);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_rendershadow);
#ifdef SKIP_ASM
struct sAmMat_0948;
// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_0948(void* d, const void* s)
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

struct sAmMat_0948 {
    float m[16];
    sAmMat_0948() {}
    sAmMat_0948(const sAmMat_0948& s) { vu0CopyMat_0948(this, &s); }
    sAmMat_0948& operator=(const sAmMat_0948& s)
    {
        vu0CopyMat_0948(this, &s);
        return *this;
    }
    // placement array new for the one-time construction of the matrix palette.
    void* operator new[](unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));


// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_0948(sAmMat_0948* d, const sAmMat_0948* a, const sAmMat_0948* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
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
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

static inline sAmMat_0948 mulMat_0948(const sAmMat_0948& a, const sAmMat_0948& b)
{
    sAmMat_0948 t;
    vu0MulMat_0948(&t, &a, &b);
    return t;
}

struct sAmVEnt_0948 {
    short delta;
    short index;
    void* fn;
};

struct sAmCtx_0948 {
    char pad[0x10D8];
    sAmVEnt_0948* vt;           // 0x10D8
};

typedef void (*tSetMats_0948)(void*, sAmMat_0948*, int, int, int, int);
typedef void (*tDraw_0948)(void*, char*, char*, int);

extern int D_004A594C;
extern sAmMat_0948 D_004FD420[];

/**
 * Draws the model's shadow pass with one colour for board parts and another for the rest.
 *
 * First builds D_004FD420 (64 matrices, constructed once) as unk_030[i] * unk_038[i] for each bone. With unk_018 set,
 * each part used at @p lod uploads the matrices for its selected variant (graphics manager slot 111) and draws it
 * (slot 100); otherwise the lod's matrices from lodInfo are uploaded once and every used part's first variant is
 * drawn. Parts with id 1 or 2 are drawn with @p x, the others with @p y.
 *
 * @param lod  the level of detail to draw
 * @param x  colour for parts with id 1 or 2 (On Tour const tARGB& boardcol)
 * @param y  colour for all other parts (On Tour const tARGB& shdwcol)
 */
void cAnimModel::rendershadow(int lod, int x, int y)
{
    if (D_004A594C == 0) {
        new ((void*)D_004FD420) sAmMat_0948[64];
        D_004A594C = 1;
    }
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    for (int i = 0; i < numBones; i++) {
        D_004FD420[i] = mulMat_0948(((sAmMat_0948*)SkelMatrices)[i], ((sAmMat_0948*)InvDefaultMatrices)[i]);
    }
    if (unk_018 != 0) {
        for (int j = 0; j < numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = &animModelPart->lods[lod].refs[animModelPart->refIndex];
                if (animModelLodRef->part != 0) {
                    ctx->GenerateBlendedMatrices((const t4Matrix*)D_004FD420, 0, (const tMdfBlendedMatrix*)animModelLodRef->unk_0C, animModelLodRef->unk_08, 0);
                    int ok = (unsigned)(parts[j].id - 1) < 2;
                    int colour = ok ? x : y;
                    ctx->DrawCharShadowModel((const tMdfModelHeader*)animModelLodRef->part, animModelLodRef->data, *(const tARGB*)colour);
                }
            }
        }
    } else {
        ctx->GenerateBlendedMatrices((const t4Matrix*)D_004FD420, 0, (const tMdfBlendedMatrix*)lodInfo[lod].unk_04,
                lodInfo[lod].unk_00, 0);
        for (int j = 0; j < numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = animModelPart->lods[lod].refs;
                if (animModelLodRef->part != 0) {
                    int ok = (unsigned)(animModelPart->id - 1) < 2;
                    int colour = ok ? x : y;
                    ctx->DrawCharShadowModel((const tMdfModelHeader*)animModelLodRef->part, animModelLodRef->data, *(const tARGB*)colour);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_getBoneSkelIndex);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);

struct sAmItem50 {
    char name[0x50];
};

/**
 * Finds a bone by name in one part and returns its skeleton index.
 *
 * Compares the part's 0x50-byte bone names with strcmp (strcmp); the part number is not range-checked.
 *
 * @param idx  part number (On Tour: partID), mapped through partIndex
 * @param name  bone name
 * @return the part's firstBone plus the bone's position in the part, or -1 when no bone has that name
 */
int cAnimModel::getBoneSkelIndex(int idx, const char* name)
{
    sAnimModelPart* animModelPart = &parts[partIndex[idx]];
    sAmItem50* items = (sAmItem50*)animModelPart->boneNames;
    for (int i = 0; i < animModelPart->numBones; i++)
    {
        if (strcmp(&items[i], name) == 0)
            return animModelPart->firstBone + i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_getGraftMask);
#ifdef SKIP_ASM
extern "C" int strncmp(const char* a, const char* b, int n);
extern char str_morph[];

/**
 * Builds the 64-bit bone mask for a comma-separated list of bone names in one part.
 *
 * Each name sets bit firstBone + j of the part's bone with exactly that name. When the part has a channel bit
 * (unk_0C >= 0), "morph" sets bit numBones + unk_0C instead; that test compares only the name's own length,
 * so a prefix of "morph" matches too.
 *
 * @param idx  part number (On Tour: partID); an out-of-range or unmapped part gives 0
 * @param names  comma-separated bone names
 * @return the mask; unknown names add nothing
 */
// PORT: returns a 64-bit mask (long is 8 bytes on the EE).
ulong cAnimModel::getGraftMask(int idx, char* names)
{
    if (maxPartIndex < idx)
        return 0;
    int* tbl = partIndex;
    if (tbl == 0 || tbl[idx] < 0)
        return 0;
    char* token = names;
    ulong mask = 0;
    sAnimModelPart* part = &parts[tbl[idx]];
    while (*token) {
        char* tokenEnd = token;
        if (*tokenEnd != 0 && *tokenEnd != ',') {
            do
                tokenEnd++;
            while (*tokenEnd != 0 && *tokenEnd != ',');
        }
        int len = tokenEnd - token;
        if (part->unk_0C >= 0 && strncmp(token, str_morph, len) == 0) {
            mask |= (ulong)1 << (part->unk_0C + numBones);
        } else {
            char* list = part->boneNames;
            int j = 0;
            ulong one = 1;
            for (; j < part->numBones; j++) {
                if (strncmp(list + j * 80, token, len) == 0 && *(list + len + j * 80) == 0) {
                    mask |= one << (part->firstBone + j);
                    break;
                }
            }
        }
        token = tokenEnd + (*tokenEnd == ',');
    }
    return mask;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_writeToReplayFrame);
#ifdef SKIP_ASM
class cAmStream {
public:
    virtual void read(void* data, int size);
    virtual void write(void* data, int size);
};

/**
 * Writes the model's bone transforms (numBones x 0x20 bytes) to a replay frame.
 *
 * Calls the stream's first virtual, which is the replay stream's write (as in cReplay_saveFrame()); the local
 * cAmStream class has the two names swapped and calls it read.
 *
 * @param data  the replay frame stream (On Tour: cBxReplayFrame*)
 */
void cAnimModel::writeToReplayFrame(cBxReplayFrame* data)
{
    data->write((const char*)bones, numBones << 5);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads the model's bone transforms (numBones x 0x20 bytes) back from a replay frame.
 *
 * Calls the stream's second virtual, which is the replay stream's read (as in cReplay_restoreFrame()); the
 * local cAmStream class has the two names swapped and calls it write.
 *
 * @param data  the replay frame stream (On Tour: cBxReplayFrame*)
 */
void cAnimModel::readFromReplayFrame(cBxReplayFrame* data)
{
    data->read((char*)bones, numBones << 5);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", gGenCacheBucket);
#ifdef SKIP_ASM
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
// as operator new[] so the new-expressions below compute their destination before the call.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_AnimChanCache[];



// Typed views of the pool tables (declared as int[] later in this unit).
extern tCmpChanCache* gChanCache_310F18[] __asm__("gChanCache");
extern tCmpChanCache* gChanCacheLRU_310F18[] __asm__("gChanCacheLRU");

/**
 * Allocates one channel-cache bucket: 20 nodes in a circular list, each with room for n channels.
 *
 * The 0x24-byte nodes and their n x 0x14-byte element buffers are tagged "AnimChanCache"; every node starts
 * unowned. The ring is stored in gChanCache[bank] and its head (the next node to reuse) in gChanCacheLRU[bank].
 *
 * @param bank  bucket number (On Tour: partID; cAdfDataSource_getFramePartData passes a block's kind byte)
 * @param n  number of channels per node (On Tour: numChan)
 * @return the first node, which is also the new head
 */
extern "C" tCmpChanCache* gGenCacheBucket(int bank, int n)
{
    gChanCache_310F18[bank] = new (str_AnimChanCache, 0x20000000, 0) tCmpChanCache[20];
    for (int i = 0; i < 20; i++)
    {
        gChanCache_310F18[bank][i].next = &gChanCache_310F18[bank][i] + 1;
        gChanCache_310F18[bank][i].prev = &gChanCache_310F18[bank][i] - 1;
        gChanCache_310F18[bank][i].animChunk = 0;
        gChanCache_310F18[bank][i].cmpAccess.mChannels = new (str_AnimChanCache, 0x20000000, 0) tCmpChannel[n];
    }
    gChanCache_310F18[bank][0].prev = &gChanCache_310F18[bank][19];
    gChanCache_310F18[bank][19].next = gChanCache_310F18[bank];
    return gChanCacheLRU_310F18[bank] = gChanCache_310F18[bank];
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", __13cAnimDataBase);
#ifdef SKIP_ASM
extern "C" void memset(void* dst, int c, int n);
// PORT: prototype mismatch. cAnimDataBase_initChanCache is defined later in the unit as (void), but this
// caller passes the model in $4.
void cAnimDataBase_initChanCache_1(void* self) __asm__("cAnimDataBase_initChanCache");

/**
 * Constructs the animation database: makes it the global instance, zeroes it and clears the channel cache.
 *
 * Stores the object in cAnimDataBase_TheAnimDataBase (whose start is the bank table cScriptAnimBankManager_LinkBank() installs banks
 * into), zero-fills its 0x3FC bytes (memset) and calls cAnimDataBase_initChanCache().
 */
cAnimDataBase::cAnimDataBase()
{
    cAnimDataBase_TheAnimDataBase = (cAnimDataBase*)(this);
    memset(this, 0, 0x3FC);
    cAnimDataBase_initChanCache_1(this);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", _$_13cAnimDataBase);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void operator_delete(int*);
// cAnimDataBase_purgeChanCache ignores its argument (matched as (void)); the call here passes self in a0.
extern "C" void cAnimDataBase_purgeChanCache_self(void* self) __asm__("cAnimDataBase_purgeChanCache");

/**
 * Destroys the animation database: clears cAnimDataBase_TheAnimDataBase and frees the channel cache.
 *
 * Calls cAnimDataBase_purgeChanCache (passing the object, which it ignores) and frees the object with operator_delete when bit 0 of
 * g++'s delete flag is set. Reverse of cAnimDataBase::cAnimDataBase.
 */
cAnimDataBase::~cAnimDataBase()
{
    cAnimDataBase_TheAnimDataBase = 0;
    cAnimDataBase_purgeChanCache_self(this);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimDataBase_initChanCache);
#ifdef SKIP_ASM
extern int gChanCache[];
extern int gChanCacheLRU[];

/**
 * Clears the 48 bucket pointers in both tables of the animation channel cache.
 *
 * Zeroes gChanCache[i] and gChanCacheLRU[i] for i 0-47: the bucket node rings gGenCacheBucket() allocates and their
 * current nodes. It takes no parameters here, though On Tour declares it as a cAnimDataBase method.
 */
extern "C" void cAnimDataBase_initChanCache(void)
{
    for (int i = 0; i < 48; i++)
    {
        gChanCache[i] = 0;
        gChanCacheLRU[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimDataBase_purgeChanCache);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cMemMan_free(void*);

struct sAnimNode_311110
{
    unsigned char* owner;       // 0x00
    void* data;                 // 0x04
    char pad8[0x14];
    sAnimNode_311110* prev;     // 0x1C
    sAnimNode_311110* next;     // 0x20
};

// Typed views of the pool tables (declared as int[] in this unit).
extern sAnimNode_311110* gChanCache_311110[] __asm__("gChanCache");
extern sAnimNode_311110* gChanCacheLRU_311110[] __asm__("gChanCacheLRU");

/**
 * Frees the animation channel cache: every allocated bucket of gChanCache, its nodes' data and the bucket itself.
 *
 * For each of the 48 buckets that is non-null, each of its 20 nodes (0x24 bytes) gets its owner's byte at +0x5 set to
 * 0xFF (no cached node) and its data freed with cMemMan_free; then the bucket array is freed and gChanCache[bank] and
 * gChanCacheLRU[bank] (the bucket's current node) are zeroed. Called by cAnimDataBase::~cAnimDataBase and before animation data is
 * unloaded (scripter/scanimmgr, fe/fereal, ai/ai).
 */
extern "C" void cAnimDataBase_purgeChanCache(void)
{
    for (int bank = 0; bank < 48; bank++)
    {
        if (gChanCache_311110[bank] != 0)
        {
            for (int i = 0; i < 20; i++)
            {
                if (gChanCache_311110[bank][i].owner != 0)
                {
                    gChanCache_311110[bank][i].owner[5] = 0xFF;
                }
                if (gChanCache_311110[bank][i].data != 0)
                {
                    cMemMan_free(gChanCache_311110[bank][i].data);
                }
            }
            if (gChanCache_311110[bank] != 0)
            {
                cMemMan_free(gChanCache_311110[bank]);
            }
            gChanCache_311110[bank] = 0;
            gChanCacheLRU_311110[bank] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAdfDataSource_cAdfDataSource1);
#ifdef SKIP_ASM
extern "C" void cAdfDataSource_populateFromData(cAdfDataSource* self, tAdfArchive* data);

/**
 * Constructs an animation data source over @p data that is already in memory, and returns @p self.
 *
 * Zeroes the first word (the loaded-file pointer that cAdfDataSource::~cAdfDataSource frees, so the caller keeps ownership of @p data)
 * and calls cAdfDataSource_populateFromData(@p self, @p data). scripter/scanimmgr uses it for banks it has loaded.
 *
 * @param data  the animation file data
 * @return @p self
 */
extern "C" void* cAdfDataSource_cAdfDataSource1(cAdfDataSource* self, char* data)
{
    *(int*)self = 0;
    self->populateFromData((tAdfArchive*)data);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAdfDataSource_cAdfDataSource);
#ifdef SKIP_ASM
extern "C" char* FILE_loadpack(void* src, int flags);

/**
 * Constructs an animation data source by loading file @p src, and returns @p self.
 *
 * Loads the file with FILE_loadpack(@p src, 0), keeps the buffer in the first word (freed by cAdfDataSource::~cAdfDataSource) and calls
 * cAdfDataSource_populateFromData on it. ai/ai and fe/fereal create their animation banks with it.
 *
 * @param src  the file name of the animation bank
 * @return @p self
 */
extern "C" void* cAdfDataSource_cAdfDataSource(char** self, void* src)
{
    char* data = FILE_loadpack(src, 0);
    *self = data;
    ((cAdfDataSource*)self)->populateFromData((tAdfArchive*)data);
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAdfDataSource_populateFromData);
#ifdef SKIP_ASM
/**
 * Points an animation data source at the sections of a loaded animation archive.
 *
 * Stores @p data (+0x4) and its section pointers: data + 0x10 (+0x8), and data plus the offsets in header words +0x4,
 * +0x8 and +0xC (+0xC, +0x10, +0x14). The release build drops On Tour's cookie and header checks.
 *
 * @param data  the loaded animation archive
 */
void cAdfDataSource::populateFromData(tAdfArchive* data)
{
    // ORIGINAL-PTR: a file blob: the loaded archive's header is followed by the animation headers and holds the byte
    // offsets of its other sections
    Archive = data;
    Anims = (tAdfAnimHeader*)(data + 1);
    Chunks = (tAdfAnimChunk*)((char*)data + data->mAnimChunkOffset);
    Tags = (tAdfTag*)((char*)data + data->mTagOffset);
    ChannelData = (unsigned char*)((char*)data + data->mChannelOffset);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", _$_14cAdfDataSource);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys an animation data source: frees its loaded file buffer (the first word) when there is one.
 *
 * Frees the object with operator_delete when bit 0 of g++'s delete flag is set; callers pass 3.
 */
cAdfDataSource::~cAdfDataSource()
{
    void* data = *(void**)this;
    if (data != 0)
    {
        cMemMan_free(data);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAdfDataSource_getFramePartData);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSeqAM {
    char pad_0x0[0x4];
    int base;                       // 0x4
    char pad_0x8[0x2];
    unsigned short a;               // 0xA
    unsigned short n;               // 0xC
    unsigned short b;               // 0xE
    char pad_0x10[4];
};
struct sEntAM {
    // slot when the chunk has no channel-cache node (name ours). [U] getFramePartData reuses the node while slot is
    // another value and otherwise takes one, storing this in the evicted owner's slot.
    enum { kNoSlot = 0xFF };

    int off;                        // 0x0
    unsigned char kind;             // 0x4
    unsigned char slot;             // 0x5
    unsigned short len;             // 0x6
};
struct sNodeAM {
    sEntAM* owner;                  // 0x00
    void* data;                     // 0x04
    char pad_0x8[0x14];
    sNodeAM* prev;                  // 0x1C
    sNodeAM* next;                  // 0x20
};
extern sNodeAM* D_pool_AM[] __asm__("gChanCache");
extern sNodeAM* D_head_AM[] __asm__("gChanCacheLRU");
sNodeAM* gGenCacheBucket_AM(int kind, long n) __asm__("gGenCacheBucket");
extern "C" void dcmpInitFrameAccessor(void* dst, void* src, int len);
extern "C" void dcmpGetFrame(void* node, float t, void* a, long b, void* c);

/**
 * Decodes one animation frame's channels for a part, through a cache of unpacked channel blocks.
 *
 * The frame index is the time x 30; the animation's 0x14-byte sequence record gives the block (frames per
 * block at +0xE) and the block entry for the part. A cached block's node moves to the most-recently-used end
 * of its kind's ring; otherwise the ring's head node is taken from its previous owner (the bucket is created
 * on first use), set up with dcmpInitFrameAccessor() and claimed. dcmpGetFrame (dcmpGetFrame) then writes
 * the channels.
 *
 * @param dest  destination float buffer (On Tour: dest)
 * @param frame  the source frame: word 0 >> 8 = animation number, word 1 = time in seconds (On Tour:
 *            tAnimSourceFrame*)
 * @param part  part number (On Tour: part)
 * @param startChan  first channel to decode (On Tour: start)
 * @param numChans  number of channels, clamped to the block's channel count (On Tour: len)
 */
void cAdfDataSource::getFramePartData(void* dest, unsigned int* frame, int part, void* startChan, long numChans)
{
    float findex = *(float*)(frame + 1) * 30.0f;
    sSeqAM* seq = (sSeqAM*)&Anims[frame[0] >> 8];
    float lim = (float)(seq->n - 1);
    int idx;
    int numChunks;
    int chunkFrames;
    if (lim <= findex) {
        idx = seq->a - 1;
        findex = lim;
        chunkFrames = seq->b;
        numChunks = seq->a;
    } else {
        idx = (int)findex / (seq->b - 1);
        chunkFrames = seq->b;
        numChunks = seq->a;
    }
    sNodeAM* node;
    sEntAM* ent = (sEntAM*)((char*)&Chunks[seq->base] + part * numChunks * 8 + idx * 8);
    findex = findex - (float)(idx * (chunkFrames - 1));
    long chunkChans = *(unsigned char*)(((char*)ChannelData) + ent->off);
    int kind = ent->kind;
    if (chunkChans < numChans) {
        numChans = chunkChans;
    }
    if (ent->slot != sEntAM::kNoSlot) {
        node = D_pool_AM[kind] + ent->slot;
        sNodeAM** head = &D_head_AM[kind];
        if (node == *head) {
            *head = node->next;
        } else if ((*head)->prev != node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->prev = (*head)->prev;
            node->next = *head;
            (*head)->prev->next = node;
            (*head)->prev = node;
        }
        dcmpGetFrame((char*)node + 4, findex, startChan, numChans, dest);
    } else {
        int slot;
        node = D_head_AM[kind];
        if (node == 0) {
            node = gGenCacheBucket_AM(kind, chunkChans);
        } else if (node->owner != 0) {
            slot = node->owner->slot;
            node->owner->slot = sEntAM::kNoSlot;
            goto have_slot;
        }
        slot = (node - D_pool_AM[kind]);
have_slot:
        ent->slot = slot;
        node->owner = ent;
        D_head_AM[kind] = node->next;
        char* np = (char*)node + 4;
        dcmpInitFrameAccessor(np, ((char*)ChannelData) + ent->off, ent->len);
        dcmpGetFrame(np, findex, startChan, numChans, dest);
    }
}
#endif

