#include "common.h"
#include "render/ps2graphicsman.h"
#include "animation/animmodel.h"

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
extern "C" void cAnimModel_addModelPartLOD(cAnimModel* self, int lod, char* part, int index)
{
    char* data = part + 0x60;
    for (int i = 0; i < self->numParts; i++) {
        if (self->parts[i].id == *(signed char*)(part + 0x52)) {
            self->parts[i].lods[lod].used = 1;
            self->parts[i].lods[lod].refs[index].part = part;
            self->parts[i].lods[lod].refs[index].data = data;
            return;
        }
    }
    for (int k = 0; k < 4; k++) {
        self->parts[self->numParts].lods[k].refs = new (str_ModelVariant, 0, 0) sAnimModelLodRef[self->numLodRefs];
        MEM_fill(self->parts[self->numParts].lods[k].refs, 0, self->numLodRefs << 4);
    }
    self->parts[self->numParts].id = *(signed char*)(part + 0x52);
    self->parts[self->numParts].lods[lod].used = 1;
    self->parts[self->numParts].lods[lod].refs[index].part = part;
    self->parts[self->numParts].lods[lod].refs[index].data = data;
    self->parts[self->numParts].boneNames = data + *(int*)(part + 0x18);
    self->parts[self->numParts].unk_40 = data + *(int*)(part + 0x2C);
    self->parts[self->numParts].unk_3C = data + *(int*)(part + 0x1C);
    self->parts[self->numParts].numBones = *(short*)(part + 0x4A);
    self->parts[self->numParts].unk_48 = *(short*)(part + 0x4E);
    self->parts[self->numParts].unk_4C = *(short*)(part + 0x50);
    self->parts[self->numParts].refIndex = index;
    self->numParts++;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030DB70__FPvi);
#ifdef SKIP_ASM
struct sAnimModelList {
    char pad_0x00[0x60];
    int count;     // 0x60
    int items[1];  // 0x64
};

void func_0030DB70(void* self, int item)
{
    sAnimModelList* list = (sAnimModelList*)self;
    list->items[list->count++] = item;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030DB90);
#ifdef SKIP_ASM
extern "C" int func_0030DB90(void* self, void* other)
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
extern "C" void cAnimModel_setScale(cAnimModel* self, float lengthScale, float widthScale)
{
    t4Vector scale;
    scale.x = lengthScale;
    scale.y = widthScale;
    scale.z = widthScale;
    scale.w = 1.0f;
    self->unk_140 = scale;
}
#endif

INCLUDE_ASM("animation/animmodel", cAnimModel_compile);

//100%
INCLUDE_ASM("animation/animmodel", func_0030E9E0);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_0030E9E0(cAnimModel* self, int i, int channel)
{
    if (i > self->maxPartIndex)
    {
        return;
    }
    if (self->partIndex == 0)
    {
        return;
    }
    int idx = self->partIndex[i];
    if (idx < 0)
    {
        return;
    }
    sAnimModelPart* animModelPart = &self->parts[idx];
    self->mask |= animModelPart->mask;
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
INCLUDE_ASM("animation/animmodel", func_0030EA80);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_0030EA80(cAnimModel* self, int i, int channel)
{
    if (i > self->maxPartIndex)
    {
        return;
    }
    if (self->partIndex == 0)
    {
        return;
    }
    int idx = self->partIndex[i];
    if (idx < 0)
    {
        return;
    }
    sAnimModelPart* animModelPart = &self->parts[idx];
    self->mask &= ~animModelPart->mask;
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
                self->mask |= animModelPart->mask;
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030EB60);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_0030EB60(cAnimModel* self)
{
    for (int j = 0; j < self->numParts; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            self->parts[j].lods[k].used = 0;
        }
    }
    self->mask = 0;
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
extern "C" int cAnimModel_isPartEnabled(cAnimModel* self, int i, int channel)
{
    int idx;
    if (i > self->maxPartIndex || self->partIndex == 0 || (idx = self->partIndex[i]) < 0)
    {
        return 0;
    }
    return self->parts[idx].lods[channel].used;
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
extern "C" void cAnimModel_setModelPartVariant(cAnimModel* self, int i, int variant)
{
    if (i > self->maxPartIndex)
    {
        return;
    }
    if (self->partIndex == 0)
    {
        return;
    }
    int idx = self->partIndex[i];
    if (idx < 0)
    {
        return;
    }
    sAnimModelPart* animModelPart = &self->parts[idx];
    animModelPart->refIndex = variant;
    self->mask |= animModelPart->mask;
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
extern "C" void cAnimModel_addAnimMinPart(cAnimModel* self, int i)
{
    sAnimModelPart* animModelPart = &self->parts[self->partIndex[i]];
    self->unk_158 |= animModelPart->mask;
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
extern "C" void cAnimModel_setSkelMatrix(cAnimModel* self, int i, sAnimModelBone* animModelBone)
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
        : "r"(animModelBone), "r"((t4Matrix*)((i << 6) + (int)self->unk_030))
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
        : "r"((t4Matrix*)((i << 6) + (int)self->unk_030)), "r"((t4Matrix*)((i << 6) + (int)self->unk_034)), "r"(self)
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

struct sAmXform_0200
{
    sAmQuat_0200 pos;
    sAmQuat_0200 rot;
};

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
static inline sAmXform_0200 amCombine_0200(const sAmXform_0200& a, const sAmXform_0200& b)
{
    sAmXform_0200 r;
    r.rot = amQuatMul_0200(a.rot, b.rot);
    r.pos = amAdd_0200(amRot_0200(a.rot, amSub_0200(b.pos, *(const sAmQuat_0200*)&t4vect_Origin)), a.pos);
    return r;
}

// One boneNames entry (0x50 bytes): the name, then the parent bone (< 0 = root).
struct sAmBoneInfo_0200 {
    char name[0x12];
    short parent;
    char pad[0x3C];
};

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
extern "C" void cAnimModel_transform(cAnimModel* self, sAnimModelBone* roots)
{
    for (int i = 0; i < self->numParts; i++)
    {
        // PORT: pointer arithmetic through int (offset-first addu).
        sAmBoneInfo_0200* info = (sAmBoneInfo_0200*)((sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)self->parts))->boneNames;
        for (int j = 0; j < ((sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)self->parts))->numBones; j++)
        {
            int k = j + ((sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)self->parts))->firstBone;
            sAnimModelBone* parent;
            sAmBoneInfo_0200* boneInfo = &info[j];
            short parentIndex = boneInfo->parent;
            if (parentIndex < 0)
                parent = roots++;
            else
                parent = &self->bones[parentIndex];
            t4Vector* lp = (t4Vector*)((k << 4) + (int)self->localPos);
            sAmXform_0200 local;
            local.pos = sAmQuat_0200(lp->x * self->unk_140.x, lp->y * self->unk_140.y, lp->z * self->unk_140.z, lp->w * self->unk_140.w);
            local.rot = *(sAmQuat_0200*)((k << 4) + (int)self->localRot);
            *(sAmXform_0200*)((k << 5) + (int)self->bones) = amCombine_0200(*(sAmXform_0200*)parent, local);
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
extern "C" void cAnimModel_buildSkelMatrices(cAnimModel* self)
{
    int i;
    for (i = 0; i < self->numBones; i++)
    {
        cAnimModel_setSkelMatrix(self, i, &self->bones[i]);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00310458);
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

struct sAmPoint_0458 {
    float x, y, z;
    int bone;
};

// Returns point `index` of the part with this id, transformed by its bone matrix (unk_034),
// or t4vect_Origin when no part has the id.
extern "C" t4Vector func_00310458(cAnimModel* model, int id, int index)
{
    for (int i = 0; i < model->numParts; i++)
    {
        // PORT: pointer arithmetic through int (offset-first addu).
        sAnimModelPart* part = (sAnimModelPart*)(i * (int)sizeof(sAnimModelPart) + (int)model->parts);
        if (part->id == id)
        {
            sAmPoint_0458* pt = (sAmPoint_0458*)((index << 4) + (int)part->unk_3C);
            t4Matrix* m = (t4Matrix*)(((pt->bone + part->firstBone) << 6) + (int)model->unk_034);
            t4Vector v;
            v.x = pt->x;
            v.y = pt->y;
            v.z = pt->z;
            v.w = 1.0f;
            return amMulMat_0458(m, v);
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
extern "C" void cAnimModel_applyOffset(cAnimModel* self, t4Vector* offset)
{
    for (int i = 0; i < self->numParts; i++) {
        for (int j = 0; j < self->parts[i].numBones; j++) {
            int k = j + self->parts[i].firstBone;
            amVecAddEq(&self->bones[k].pos, offset);
            {
                t4Vector shiftedPos = amVecAdd(self->unk_030[k].row[3], offset);
                self->unk_030[k].row[3] = shiftedPos;
            }
            {
                t4Vector shiftedPos = amVecAdd(self->unk_034[k].row[3], offset);
                self->unk_034[k].row[3] = shiftedPos;
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
 * @param x  draw flag for the part with id 2, probably the board (On Tour: additiveBoard)
 * @param y  draw flag for every other part (On Tour: additivePlayer)
 */
extern "C" void cAnimModel_render(cAnimModel* self, int lod, int x, int y)
{
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (D_004A5948 == 0) {
        new ((void*)D_004FC420) sAmMat_0640[64];
        D_004A5948 = 1;
    }
    for (int i = 0; i < self->numBones; i++) {
        D_004FC420[i] = mulMat_0640(((sAmMat_0640*)self->unk_034)[i], ((sAmMat_0640*)self->unk_038)[i]);
    }
    if (self->unk_018 != 0) {
        for (int j = 0; j < self->numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)self->parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = &animModelPart->lods[lod].refs[animModelPart->refIndex];
                if (animModelLodRef->part != 0) {
                    ((tSetMats_0640)((sAmVEnt_0640*)ctx->mVtbl)[111].fn)((char*)ctx + ((sAmVEnt_0640*)ctx->mVtbl)[111].delta, D_004FC420, 0, animModelLodRef->unk_0C, animModelLodRef->unk_08, 0);
                    int additive = self->parts[j].id == 2 ? x : y;
                    ((tDraw_0640)((sAmVEnt_0640*)ctx->mVtbl)[99].fn)((char*)ctx + ((sAmVEnt_0640*)ctx->mVtbl)[99].delta, animModelLodRef->part, animModelLodRef->data, additive, 0, self->unk_140.y);
                }
            }
        }
    } else {
        ((tSetMats_0640)((sAmVEnt_0640*)ctx->mVtbl)[111].fn)((char*)ctx + ((sAmVEnt_0640*)ctx->mVtbl)[111].delta, D_004FC420, 0, self->lodInfo[lod].unk_04, self->lodInfo[lod].unk_00, 0);
        for (int j = 0; j < self->numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)self->parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = animModelPart->lods[lod].refs;
                if (animModelLodRef->part != 0) {
                    int additive = animModelPart->id == 2 ? x : y;
                    ((tDraw_0640)((sAmVEnt_0640*)ctx->mVtbl)[99].fn)((char*)ctx + ((sAmVEnt_0640*)ctx->mVtbl)[99].delta, animModelLodRef->part, animModelLodRef->data, additive, 0, self->unk_140.y);
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
extern "C" void cAnimModel_rendershadow(cAnimModel* self, int lod, int x, int y)
{
    if (D_004A594C == 0) {
        new ((void*)D_004FD420) sAmMat_0948[64];
        D_004A594C = 1;
    }
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    for (int i = 0; i < self->numBones; i++) {
        D_004FD420[i] = mulMat_0948(((sAmMat_0948*)self->unk_030)[i], ((sAmMat_0948*)self->unk_038)[i]);
    }
    if (self->unk_018 != 0) {
        for (int j = 0; j < self->numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)self->parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = &animModelPart->lods[lod].refs[animModelPart->refIndex];
                if (animModelLodRef->part != 0) {
                    ((tSetMats_0948)((sAmVEnt_0948*)ctx->mVtbl)[111].fn)((char*)ctx + ((sAmVEnt_0948*)ctx->mVtbl)[111].delta, D_004FD420, 0, animModelLodRef->unk_0C, animModelLodRef->unk_08, 0);
                    int ok = (unsigned)(self->parts[j].id - 1) < 2;
                    int colour = ok ? x : y;
                    ((tDraw_0948)((sAmVEnt_0948*)ctx->mVtbl)[100].fn)((char*)ctx + ((sAmVEnt_0948*)ctx->mVtbl)[100].delta, animModelLodRef->part, animModelLodRef->data, colour);
                }
            }
        }
    } else {
        ((tSetMats_0948)((sAmVEnt_0948*)ctx->mVtbl)[111].fn)((char*)ctx + ((sAmVEnt_0948*)ctx->mVtbl)[111].delta, D_004FD420, 0, self->lodInfo[lod].unk_04, self->lodInfo[lod].unk_00, 0);
        for (int j = 0; j < self->numParts; j++) {
            // PORT: pointer arithmetic through int (offset-first addu).
            sAnimModelPart* animModelPart = (sAnimModelPart*)(j * (int)sizeof(sAnimModelPart) + (int)self->parts);
            if (animModelPart->lods[lod].used != 0) {
                sAnimModelLodRef* animModelLodRef = animModelPart->lods[lod].refs;
                if (animModelLodRef->part != 0) {
                    int ok = (unsigned)(animModelPart->id - 1) < 2;
                    int colour = ok ? x : y;
                    ((tDraw_0948)((sAmVEnt_0948*)ctx->mVtbl)[100].fn)((char*)ctx + ((sAmVEnt_0948*)ctx->mVtbl)[100].delta, animModelLodRef->part, animModelLodRef->data, colour);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_getBoneSkelIndex);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

struct sAmItem50 {
    char name[0x50];
};

/**
 * Finds a bone by name in one part and returns its skeleton index.
 *
 * Compares the part's 0x50-byte bone names with func_004165A8 (strcmp); the part number is not range-checked.
 *
 * @param idx  part number (On Tour: partID), mapped through partIndex
 * @param name  bone name
 * @return the part's firstBone plus the bone's position in the part, or -1 when no bone has that name
 */
extern "C" int cAnimModel_getBoneSkelIndex(cAnimModel* self, int idx, const char* name)
{
    sAnimModelPart* animModelPart = &self->parts[self->partIndex[idx]];
    sAmItem50* items = (sAmItem50*)animModelPart->boneNames;
    for (int i = 0; i < animModelPart->numBones; i++)
    {
        if (func_004165A8(&items[i], name) == 0)
            return animModelPart->firstBone + i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimModel_getGraftMask);
#ifdef SKIP_ASM
extern "C" int func_00416B18(const char* a, const char* b, int n);
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
extern "C" ulong cAnimModel_getGraftMask(cAnimModel* self, int idx, char* names)
{
    if (self->maxPartIndex < idx)
        return 0;
    int* tbl = self->partIndex;
    if (tbl == 0 || tbl[idx] < 0)
        return 0;
    char* token = names;
    ulong mask = 0;
    sAnimModelPart* part = &self->parts[tbl[idx]];
    while (*token) {
        char* tokenEnd = token;
        if (*tokenEnd != 0 && *tokenEnd != ',') {
            do
                tokenEnd++;
            while (*tokenEnd != 0 && *tokenEnd != ',');
        }
        int len = tokenEnd - token;
        if (part->unk_0C >= 0 && func_00416B18(token, str_morph, len) == 0) {
            mask |= (ulong)1 << (part->unk_0C + self->numBones);
        } else {
            char* list = part->boneNames;
            int j = 0;
            ulong one = 1;
            for (; j < part->numBones; j++) {
                if (func_00416B18(list + j * 0x50, token, len) == 0 && *(list + len + j * 0x50) == 0) {
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
extern "C" void cAnimModel_writeToReplayFrame(cAnimModel* self, cAmStream* data)
{
    data->read(self->bones, self->numBones << 5);
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
extern "C" void cAnimModel_readFromReplayFrame(cAnimModel* self, cAmStream* data)
{
    data->write(self->bones, self->numBones << 5);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", gGenCacheBucket);
#ifdef SKIP_ASM
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
// as operator new[] so the new-expressions below compute their destination before the call.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_AnimChanCache[];

struct sAnimElem_310F18
{
    int a[5];
};

struct sAnimNode_310F18
{
    int used;                   // 0x00
    sAnimElem_310F18* data;     // 0x04
    char pad8[0x14];
    sAnimNode_310F18* prev;     // 0x1C
    sAnimNode_310F18* next;     // 0x20
};

// Typed views of the pool tables (declared as int[] later in this unit).
extern sAnimNode_310F18* D_004FE7A0_310F18[] __asm__("D_004FE7A0");
extern sAnimNode_310F18* D_004FE860_310F18[] __asm__("D_004FE860");

/**
 * Allocates one channel-cache bucket: 20 nodes in a circular list, each with room for n channels.
 *
 * The 0x24-byte nodes and their n x 0x14-byte element buffers are tagged "AnimChanCache"; every node starts
 * unowned. The ring is stored in D_004FE7A0[bank] and its head (the next node to reuse) in D_004FE860[bank].
 *
 * @param bank  bucket number (On Tour: partID; cAdfDataSource_getFramePartData passes a block's kind byte)
 * @param n  number of channels per node (On Tour: numChan)
 * @return the first node, which is also the new head
 */
extern "C" sAnimNode_310F18* gGenCacheBucket(int bank, int n)
{
    D_004FE7A0_310F18[bank] = new (str_AnimChanCache, 0x20000000, 0) sAnimNode_310F18[20];
    for (int i = 0; i < 20; i++)
    {
        D_004FE7A0_310F18[bank][i].next = &D_004FE7A0_310F18[bank][i] + 1;
        D_004FE7A0_310F18[bank][i].prev = &D_004FE7A0_310F18[bank][i] - 1;
        D_004FE7A0_310F18[bank][i].used = 0;
        D_004FE7A0_310F18[bank][i].data = new (str_AnimChanCache, 0x20000000, 0) sAnimElem_310F18[n];
    }
    D_004FE7A0_310F18[bank][0].prev = &D_004FE7A0_310F18[bank][19];
    D_004FE7A0_310F18[bank][19].next = D_004FE7A0_310F18[bank];
    return D_004FE860_310F18[bank] = D_004FE7A0_310F18[bank];
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimDataBase_cAnimDataBase);
#ifdef SKIP_ASM
extern void* cAnimDataBase_TheAnimDataBase;
extern "C" void func_00416210(void* dst, int c, int n);
// PORT: prototype mismatch. cAnimDataBase_initChanCache is defined later in the unit as (void), but this
// caller passes the model in $4.
void cAnimDataBase_initChanCache_1(void* self) __asm__("cAnimDataBase_initChanCache");

/**
 * Constructs the animation database: makes it the global instance, zeroes it and clears the channel cache.
 *
 * Stores @p self in cAnimDataBase_TheAnimDataBase (whose start is the bank table cScriptAnimBankManager_LinkBank() installs banks
 * into), zero-fills its 0x3FC bytes (func_00416210) and calls cAnimDataBase_initChanCache().
 *
 * @return @p self
 */
extern "C" void* cAnimDataBase_cAnimDataBase(void* self)
{
    cAnimDataBase_TheAnimDataBase = self;
    func_00416210(self, 0, 0x3FC);
    cAnimDataBase_initChanCache_1(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00311088);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void operator_delete(int*);
// func_00311110 ignores its argument (matched as (void)); the call here passes self in a0.
extern "C" void func_00311110_self(void* self) __asm__("func_00311110");

extern "C" void func_00311088(void* self, int flags)
{
    cAnimDataBase_TheAnimDataBase = 0;
    func_00311110_self(self);
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", cAnimDataBase_initChanCache);
#ifdef SKIP_ASM
extern int D_004FE7A0[];
extern int D_004FE860[];

/**
 * Clears the 48 bucket pointers in both tables of the animation channel cache.
 *
 * Zeroes D_004FE7A0[i] and D_004FE860[i] for i 0-47: the bucket node rings gGenCacheBucket() allocates and their
 * current nodes. It takes no parameters here, though On Tour declares it as a cAnimDataBase method.
 */
extern "C" void cAnimDataBase_initChanCache(void)
{
    for (int i = 0; i < 48; i++)
    {
        D_004FE7A0[i] = 0;
        D_004FE860[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00311110);
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
extern sAnimNode_311110* D_004FE7A0_311110[] __asm__("D_004FE7A0");
extern sAnimNode_311110* D_004FE860_311110[] __asm__("D_004FE860");

extern "C" void func_00311110(void)
{
    for (int bank = 0; bank < 48; bank++)
    {
        if (D_004FE7A0_311110[bank] != 0)
        {
            for (int i = 0; i < 20; i++)
            {
                if (D_004FE7A0_311110[bank][i].owner != 0)
                {
                    D_004FE7A0_311110[bank][i].owner[5] = 0xFF;
                }
                if (D_004FE7A0_311110[bank][i].data != 0)
                {
                    cMemMan_free(D_004FE7A0_311110[bank][i].data);
                }
            }
            if (D_004FE7A0_311110[bank] != 0)
            {
                cMemMan_free(D_004FE7A0_311110[bank]);
            }
            D_004FE7A0_311110[bank] = 0;
            D_004FE860_311110[bank] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00311220);
#ifdef SKIP_ASM
extern "C" void cAdfDataSource_populateFromData(void* self, char* data);

extern "C" void* func_00311220(void* self, char* data)
{
    *(int*)self = 0;
    cAdfDataSource_populateFromData(self, data);
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00311250);
#ifdef SKIP_ASM
extern "C" char* FILE_loadpack(void* src, int flags);

extern "C" void* func_00311250(char** self, void* src)
{
    char* data = FILE_loadpack(src, 0);
    *self = data;
    cAdfDataSource_populateFromData(self, data);
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
extern "C" void cAdfDataSource_populateFromData(void* self, char* data)
{
    *(char**)((char*)self + 0x4) = data;
    *(char**)((char*)self + 0x8) = data + 0x10;
    *(char**)((char*)self + 0xC) = data + *(int*)(data + 0x4);
    *(char**)((char*)self + 0x10) = data + *(int*)(data + 0x8);
    *(char**)((char*)self + 0x14) = data + *(int*)(data + 0xC);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_003112C8);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

extern "C" void func_003112C8(void* self, int flags)
{
    void* data = *(void**)self;
    if (data != 0)
    {
        cMemMan_free(data);
    }
    if (flags & 1)
    {
        operator_delete((int*)self);
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
struct sModelAM {
    char pad_0x0[0x8];
    char* seqs;                     // 0x8
    char* ents;                     // 0xC
    char pad_0x10[4];
    char* data;                     // 0x14
};
extern sNodeAM* D_pool_AM[] __asm__("D_004FE7A0");
extern sNodeAM* D_head_AM[] __asm__("D_004FE860");
sNodeAM* gGenCacheBucket_AM(int kind, long n) __asm__("gGenCacheBucket");
extern "C" void dcmpInitFrameAccessor(void* dst, void* src, int len);
extern "C" void func_00312C20(void* node, float t, void* a, long b, void* c);

void cAdfDataSource_getFramePartData_AM(sModelAM* self, void* p2, unsigned int* p3, int p4, void* p5, long p6) __asm__("cAdfDataSource_getFramePartData");
/**
 * Decodes one animation frame's channels for a part, through a cache of unpacked channel blocks.
 *
 * The frame index is the time x 30; the animation's 0x14-byte sequence record gives the block (frames per
 * block at +0xE) and the block entry for the part. A cached block's node moves to the most-recently-used end
 * of its kind's ring; otherwise the ring's head node is taken from its previous owner (the bucket is created
 * on first use), set up with dcmpInitFrameAccessor() and claimed. dcmpGetFrame (func_00312C20) then writes
 * the channels.
 *
 * @param dest  destination float buffer (On Tour: dest)
 * @param frame  the source frame: word 0 >> 8 = animation number, word 1 = time in seconds (On Tour:
 *            tAnimSourceFrame*)
 * @param part  part number (On Tour: part)
 * @param startChan  first channel to decode (On Tour: start)
 * @param numChans  number of channels, clamped to the block's channel count (On Tour: len)
 */
void cAdfDataSource_getFramePartData_AM(sModelAM* self, void* dest, unsigned int* frame, int part, void* startChan, long numChans)
{
    float findex = *(float*)(frame + 1) * 30.0f;
    sSeqAM* seq = (sSeqAM*)(self->seqs + (frame[0] >> 8) * 0x14);
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
    sEntAM* ent = (sEntAM*)(self->ents + seq->base * 8 + part * numChunks * 8 + idx * 8);
    findex = findex - (float)(idx * (chunkFrames - 1));
    long chunkChans = *(unsigned char*)(self->data + ent->off);
    int kind = ent->kind;
    if (chunkChans < numChans) {
        numChans = chunkChans;
    }
    if (ent->slot != 0xFF) {
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
        func_00312C20((char*)node + 4, findex, startChan, numChans, dest);
    } else {
        int slot;
        node = D_head_AM[kind];
        if (node == 0) {
            node = gGenCacheBucket_AM(kind, chunkChans);
        } else if (node->owner != 0) {
            slot = node->owner->slot;
            node->owner->slot = 0xFF;
            goto have_slot;
        }
        slot = (node - D_pool_AM[kind]);
have_slot:
        ent->slot = slot;
        node->owner = ent;
        D_head_AM[kind] = node->next;
        char* np = (char*)node + 4;
        dcmpInitFrameAccessor(np, self->data + ent->off, ent->len);
        func_00312C20(np, findex, startChan, numChans, dest);
    }
}
#endif

