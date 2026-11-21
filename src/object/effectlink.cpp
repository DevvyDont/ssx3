#include "common.h"
#include "world/worldview.h"
#include "object/flexbridgenode.h"
#include "object/effectmodifier.h"
#include "render/ps2graphicsman.h"
#include "object/effectlink.h"

struct sEffectLinkView {  // this unit's view of cEffectLink (object/effectlink.h); typed members schedule differently
    sEffectLinkView* next; // 0x0
    sEffectLinkView* prev; // 0x4
};

//100%
INCLUDE_ASM("object/effectlink", cEffectLink_add__FP11cEffectLinkT0);
#ifdef SKIP_ASM
/**
 * Inserts an effect link into the chain directly after another.
 *
 * @param link   link to insert after
 * @param other  link to insert
 */
void cEffectLink_add(cEffectLink* link, cEffectLink* other)
{
    if (link->Next != 0) {
        link->Next->Prev = other;
    }
    other->Prev = link;
    other->Next = link->Next;
    link->Next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectLink_append);
#ifdef SKIP_ASM
/**
 * Appends a link at the end of an effect list.
 *
 * Walks Next from @p link to the last link, then sets @p other's Next to 0 and its Prev to that last link and makes
 * it the last link's Next. Callers pass a tModifierBlock's effect list head (+0x10).
 *
 * @param link   the list (any link in it; normally the head)
 * @param other  the link to append (On Tour effect)
 */
extern "C" void cEffectLink_append(cEffectLink* link, cEffectLink* other)
{
    while (link->Next != 0) {
        link = link->Next;
    }
    other->Next = 0;
    other->Prev = link;
    link->Next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectLink_unlink);
#ifdef SKIP_ASM
/**
 * Removes a link from its effect list.
 *
 * Points the next link's Prev at this link's Prev and the previous link's Next at this link's Next (each only when
 * present), then clears this link's Next and Prev.
 *
 * @param link  the link to remove
 */
extern "C" void cEffectLink_unlink(cEffectLink* link)
{
    if (link->Next != 0) {
        link->Next->Prev = link->Prev;
    }
    if (link->Prev != 0) {
        link->Prev->Next = link->Next;
    }
    link->Next = 0;
    link->Prev = 0;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectModifier_cEffectModifier);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* cEffectModifier_vtbl[];

struct sEffectLinkObj_5798 {
    sEffectLinkView link;  // 0x0
    void** vt;         // 0x8
    int value;         // 0xC
};

/**
 * Constructs a cEffectModifier for an instance (On Tour cEffectModifier(tInstanceRecord* inst)).
 *
 * Clears the effect-link Next/Prev, stores cEffectModifier_vtbl and the instance (Instance, +0xC), and increments the
 * live effect count D_004A3FF4 (On Tour cEffectModifier::EffectCount).
 *
 * @param inst  the tInstanceRecord the effect is attached to (declared int)
 * @return @p self
 */
extern "C" void* cEffectModifier_cEffectModifier(cEffectModifier* self, int inst)
{
    sEffectLinkObj_5798* modifier = (sEffectLinkObj_5798*)self;
    modifier->link.next = 0;
    modifier->link.prev = 0;
    modifier->vt = cEffectModifier_vtbl;
    modifier->value = inst;
    D_004A3FF4++;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectModifier_cEffectModifier1);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* cEffectModifier_vtbl[];

struct sEffectLinkObj_57C8 {
    sEffectLinkView link;  // 0x0
    void** vt;         // 0x8
    int value;         // 0xC
};

struct sEffVEntry57C8 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Constructs an effect modifier from a replay frame.
 *
 * Clears the effect-link Next/Prev, installs cEffectModifier_vtbl, reads Instance through the stream's vtable entry 3
 * (instance-pointer read) and increments the live effect count D_004A3FF4 (read by cEffectModifier_getEffectCount). The particle and
 * halo replay constructors (cParticleModifier_cParticleModifier1, cHaloModifier_cHaloModifier1) start with it.
 *
 * @param effectModifier  the modifier being constructed
 * @param obj  replay stream to read from
 * @return effectModifier
 */
extern "C" void* cEffectModifier_cEffectModifier1(cEffectModifier* effectModifier, void* obj)
{
    sEffectLinkObj_57C8* self = (sEffectLinkObj_57C8*)effectModifier;
    self->link.next = 0;
    self->link.prev = 0;
    self->vt = cEffectModifier_vtbl;
    sEffVEntry57C8* vt = *(sEffVEntry57C8**)obj;
    self->value = vt[3].fn((char*)obj + vt[3].delta);
    D_004A3FF4++;
    return ((void*)effectModifier);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectModifier__cEffectModifier);
#ifdef SKIP_ASM
extern int D_004A3FF4;
extern void* cEffectModifier_vtbl[];
extern void* cEffectLink_vtbl[];
void operator_delete(int*);


/**
 * Destroys an effect modifier: decrements the live count (D_004A3FF4), unlinks it from its effect chain, then
 * frees it when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cEffectModifier__cEffectModifier(cEffectModifier* self, int flags)
{
    cEffectModifier* effectLink = self;
    effectLink->mVtbl = cEffectModifier_vtbl;
    D_004A3FF4--;
    effectLink->mVtbl = cEffectLink_vtbl;
    cEffectLink_unlink((cEffectLink*)(sEffectLinkView*)&effectLink->Next);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cEffectModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

/**
 * Writes an effect modifier's value (offset 0xC) to a replay frame.
 *
 * Passes the value to the stream's slot-5 virtual; the stream constructor reads it back through slot 3.
 *
 * @param obj  replay stream to write to
 */
extern "C" void cEffectModifier_writeToReplayFrame(cEffectModifier* self, void* obj)
{
    sEffVEntry* vt = *(sEffVEntry**)obj;
    vt[5].fn((char*)obj + vt[5].delta, self->Instance);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier_cParticleModifier);
#ifdef SKIP_ASM
extern "C" void* cEffectModifier_cEffectModifier(cEffectModifier* self, int a1);
extern "C" void* cStaticEmitter_cStaticEmitter(void* self);
extern "C" void cStaticEmitter_setupEmitter(void* self, void* mat, int a2);
extern int D_004A3FF8;
extern void* cParticleModifier_vtbl[];

struct sVEntry003458C0 {
    short delta;
    short index;
    void* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (4x4 matrix copy); returns dst.
static inline void* vu0CopyMatrix003458C0(void* dst, void* src)
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
        : "r"(dst), "r"(src)
        : "memory");
    return dst;
}


/**
 * Constructs a static particle effect on an instance (On Tour cParticleModifier(tParticleData* pdata,
 * tInstanceRecord* inst)).
 *
 * Runs the cEffectModifier constructor with @p owner, stores cParticleModifier_vtbl, constructs the cStaticEmitter at
 * +0x50 and keeps the particle data. The transform (+0x10) is copied from the instance node's (Instance + 0xC)
 * virtual slot 24 matrix, or from the instance record's own matrix (Instance + 0x10) when there is no node. Then
 * cStaticEmitter_setupEmitter(+0x50, transform, @p pdata), Complete (+0x1E4) = 0 and the particle count D_004A3FF8 is
 * incremented.
 *
 * @param pdata  the tParticleData describing the emitter (declared int)
 * @param owner  the tInstanceRecord the effect is attached to (On Tour inst; declared int)
 * @return @p self
 */
extern "C" void* cParticleModifier_cParticleModifier(cParticleModifier* self, int pdata, int owner)
{
    cEffectModifier_cEffectModifier((cEffectModifier*)self, owner);
    self->mVtbl = cParticleModifier_vtbl;
    cStaticEmitter_cStaticEmitter(&self->Emitter);
    self->ParticleData = (tParticleData*)pdata;
    char* own = (char*)self->Instance;
    void* obj = *(void**)(own + 0xC);
    void* transform;
    if (obj != 0) {
        sVEntry003458C0* vt = *(sVEntry003458C0**)((char*)obj + 0xC);
        transform = vu0CopyMatrix003458C0(&self->Transform, vt[24].fn((char*)obj + vt[24].delta));
    } else {
        transform = vu0CopyMatrix003458C0(&self->Transform, own + 0x10);
    }
    cStaticEmitter_setupEmitter(&self->Emitter, transform, (int)self->ParticleData);
    (self)->Complete = 0;
    D_004A3FF8++;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier_cParticleModifier1);
#ifdef SKIP_ASM
void* operator_new1(int size);
extern "C" void cStaticEmitter_readFromReplayFrame(void* self, void* stream);
extern char t4mat_Identity[];

struct sEffectLinkVEntry_3459A8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Constructs a particle modifier from a replay frame.
 *
 * Runs the effect-modifier replay constructor cEffectModifier_cEffectModifier1, installs cParticleModifier_vtbl, constructs the Emitter
 * (cStaticEmitter_cStaticEmitter) and reads Complete (+0x1E4). While not complete it allocates and reads the 0xD8-byte
 * ParticleData, reads Transform (0x40 bytes), sets the emitter up (cStaticEmitter_setupEmitter) and lets it read its
 * own state (cStaticEmitter_readFromReplayFrame); a completed modifier gets the identity Transform and no ParticleData.
 * Increments the particle-modifier count D_004A3FF8.
 *
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cParticleModifier_cParticleModifier1(cParticleModifier* self, void* stream)
{
    cEffectModifier_cEffectModifier1((cEffectModifier*)self, stream);
    self->mVtbl = cParticleModifier_vtbl;
    cStaticEmitter_cStaticEmitter(&self->Emitter);
    sEffectLinkVEntry_3459A8* readSlot = &(*(sEffectLinkVEntry_3459A8**)stream)[2];
    readSlot->fn((char*)stream + readSlot->delta, (char*)&self->Complete, 4);
    if (self->Complete == 0) {
        void* buf = operator_new1(216);
        self->ParticleData = (tParticleData*)buf;
        readSlot = &(*(sEffectLinkVEntry_3459A8**)stream)[2];
        readSlot->fn((char*)stream + readSlot->delta, buf, 216);
        readSlot = &(*(sEffectLinkVEntry_3459A8**)stream)[2];
        readSlot->fn((char*)stream + readSlot->delta, (char*)&self->Transform, 0x40);
        cStaticEmitter_setupEmitter(&self->Emitter, &self->Transform, (int)self->ParticleData);
        cStaticEmitter_readFromReplayFrame(&self->Emitter, stream);
    } else {
        vu0CopyMatrix003458C0(&self->Transform, t4mat_Identity);
        self->ParticleData = 0;
    }
    D_004A3FF8++;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier__cParticleModifier);
#ifdef SKIP_ASM
extern int D_004A3FF8;
extern void* cParticleModifier_vtbl[];
void operator_delete(int*);
extern "C" void cStaticEmitter__cStaticEmitter(int* self, int flags);
extern "C" void cEffectModifier__cEffectModifier(cEffectModifier* self, int flags);

/**
 * Destroys a particle modifier: frees its 0xD8-byte buffer (+0x1E0), decrements the live count (D_004A3FF8),
 * destroys the embedded static emitter (+0x50), then the cEffectModifier part.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cParticleModifier__cParticleModifier(cParticleModifier* self, int flags)
{
    self->mVtbl = cParticleModifier_vtbl;
    int* buf = (int*)self->ParticleData;
    if (buf != 0) {
        operator_delete(buf);
    }
    D_004A3FF8--;
    cStaticEmitter__cStaticEmitter((int*)(&self->Emitter), 2);
    cEffectModifier__cEffectModifier((cEffectModifier*)self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier_update);
#ifdef SKIP_ASM
extern "C" void cStaticEmitter_update(void* self, float dt);

/**
 * Advances a particle modifier's static emitter by one 1/60 s step until the effect has finished.
 *
 * An emitter whose life (+0x50) starts negative runs forever; otherwise the effect is marked finished
 * (+0x1E4) once the life has run down to 0.
 */
extern "C" void cParticleModifier_update(cParticleModifier* self)
{
    if (self->Complete == 0) {
        void* emitter = (char*)&self->Emitter;
        if (self->Emitter.Duration < 0.0f) {
            cStaticEmitter_update(emitter, 0.01666666753590107f);
        } else {
            cStaticEmitter_update(emitter, 0.01666666753590107f);
            if (self->Emitter.Duration <= 0.0f) {
                self->Complete = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cParticleModifier_render);
#ifdef SKIP_ASM
extern "C" void cStaticEmitter_render1(void*);

/**
 * Renders the particle modifier's static emitter unless the effect has finished.
 *
 * Calls cStaticEmitter_render1() on the emitter embedded at offset 0x50 while the finished flag (offset 0x1E4,
 * set by cParticleModifier_update when the emitter's time runs out) is clear.
 */
extern "C" void cParticleModifier_render(cParticleModifier* self)
{
    if (self->Complete == 0) {
        cStaticEmitter_render1((char*)&self->Emitter);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", cParticleModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffectLinkVEntry5BF0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cStaticEmitter_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a particle modifier's state to a replay frame.
 *
 * Writes the cEffectModifier part and the finished flag (+0x1E4); while not finished, also the 0xD8-byte
 * buffer (+0x1E0), the 0x40 bytes at +0x10 and the static emitter's own state.
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cParticleModifier_writeToReplayFrame(cParticleModifier* self, void* stream)
{
    cEffectModifier_writeToReplayFrame((cEffectModifier*)self, stream);
    sEffectLinkVEntry5BF0* writeEntry = &(*(sEffectLinkVEntry5BF0**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)&self->Complete, 4);
    if (self->Complete == 0) {
        writeEntry = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        writeEntry->fn((char*)stream + writeEntry->delta, self->ParticleData, 216);
        writeEntry = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        writeEntry->fn((char*)stream + writeEntry->delta, (char*)&self->Transform, 0x40);
        cStaticEmitter_writeToReplayFrame((char*)&self->Emitter, stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_cDynamicParticleModifier1);
#ifdef SKIP_ASM
extern "C" void* cEffectModifier_cEffectModifier(cEffectModifier* self, int a1);
extern "C" void* cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter(void* self);
extern "C" void cBaseClass_DynamicEmitter_setupEmitter(void* self, void* target, float t);
struct sEffectLink5EF8;
extern "C" void cDynamicParticleModifier_updatePosition(cDynamicParticleModifier* self);
extern int D_004A3FFC;
extern void* cDynamicParticleModifier_vtbl[];
extern void* cDynamicEmitter_vtbl[];

struct sVec4_00345C90 {
    float x, y, z, w;
    sVec4_00345C90(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
} __attribute__((aligned(16)));

struct sTarget00345C90 {
    char pad0[0x48];
    float x;
    float y;
    float z;
};


/**
 * Constructs a dynamic particle modifier attached to mesh @p bone of @p owner.
 *
 * Runs cEffectModifier_cEffectModifier(self, owner), installs cDynamicParticleModifier_vtbl, constructs the embedded
 * Emitter (cDynamicEmitter_vtbl), stores ParticleData = @p particleDesc, Mesh = @p bone and Offset = *@p pos, copies the data's
 * velocity (+0x48..+0x50) into InitVel and then zeroes it in @p particleDesc itself, clears Fade, sets up the emitter
 * (cBaseClass_DynamicEmitter_setupEmitter with -1), places it (cDynamicParticleModifier_updatePosition), counts it in
 * D_004A3FFC and clears Complete. cMoveNode_addEffectModifier's caller allocates it (624 bytes,
 * "DynamicParticleModifier").
 *
 * @param particleDesc  the particle data record (its velocity is consumed)
 * @param owner  the owning instance
 * @param bone  the mesh to follow
 * @param pos  offset from that mesh
 * @return @p self
 */
extern "C" cDynamicParticleModifier* cDynamicParticleModifier_cDynamicParticleModifier1(cDynamicParticleModifier* self, sTarget00345C90* particleDesc, int owner, int bone, sVec4_00345C90* pos)
{
    cEffectModifier_cEffectModifier((cEffectModifier*)self, owner);
    self->mVtbl = cDynamicParticleModifier_vtbl;
    cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter(((char*)&self->Emitter));
    self->Emitter.mVtbl = cDynamicEmitter_vtbl;
    self->ParticleData = (tParticleData*)particleDesc;
    self->Mesh = bone;
    *(sVec4_00345C90*)&self->Offset = *pos;
    *(sVec4_00345C90*)&self->InitVel = sVec4_00345C90(particleDesc->x, particleDesc->y, particleDesc->z, 0.0f);
    self->Fade = 0;
    particleDesc->x = 0.0f;
    particleDesc->y = 0.0f;
    particleDesc->z = 0.0f;
    cBaseClass_DynamicEmitter_setupEmitter(((char*)&self->Emitter), (sTarget00345C90*)self->ParticleData, -1.0f);
    cDynamicParticleModifier_updatePosition(self);
    D_004A3FFC++;
    self->Complete = 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_cDynamicParticleModifier);
#ifdef SKIP_ASM
void* operator_new1(int size);
struct cStream00371318;
extern "C" void cBaseClass_DynamicEmitter_readFromReplayFrame(void* self, cStream00371318* stream);
extern "C" void cBaseClass_DynamicEmitter_updateBASE(void* sub, void* a, void* b, int flag, float dt);

struct sEffectLinkVEntry_345D80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Rebuilds a dynamic (moving) particle effect from a replay frame (On Tour cDynamicParticleModifier(cBxReplayFrame*
 * data)).
 *
 * Runs the cEffectModifier replay constructor (cEffectModifier_cEffectModifier1), stores cDynamicParticleModifier_vtbl, constructs the
 * embedded emitter (cBaseClass_DynamicEmitter constructor, then cDynamicEmitter_vtbl) and reads the 0x50-byte state
 * block starting at Mesh through the stream's read (slot 2). If the effect is not Complete it allocates a 0xD8-byte
 * tParticleData with operator_new1, reads it from the stream, sets the emitter up from it with a random seed
 * (cBaseClass_DynamicEmitter_setupEmitter, cBaseClass_DynamicEmitter::setupEmitter, seed -1), restores the emitter's own replay state and
 * steps it once at Pos/Vel for 1/60 s (cBaseClass_DynamicEmitter_updateBASE). A Complete effect gets no ParticleData. Increments D_004A3FFC
 * (the dynamic particle count).
 *
 * @param stream  the replay frame to read from (On Tour data)
 * @return @p self
 */
extern "C" cDynamicParticleModifier* cDynamicParticleModifier_cDynamicParticleModifier(cDynamicParticleModifier* self, void* stream)
{
    cEffectModifier_cEffectModifier1((cEffectModifier*)self, stream);
    self->mVtbl = cDynamicParticleModifier_vtbl;
    void* data = &self->Mesh;
    cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter(((char*)&self->Emitter));
    self->Emitter.mVtbl = cDynamicEmitter_vtbl;
    sEffectLinkVEntry_345D80* vt = *(sEffectLinkVEntry_345D80**)stream;
    vt[2].fn((char*)stream + vt[2].delta, data, 80);
    if (self->Complete == 0) {
        void* particleData = operator_new1(216);
        self->ParticleData = (tParticleData*)((sTarget00345C90*)particleData);
        sEffectLinkVEntry_345D80* vt2 = *(sEffectLinkVEntry_345D80**)stream;
        vt2[2].fn((char*)stream + vt2[2].delta, particleData, 216);
        cBaseClass_DynamicEmitter_setupEmitter(((char*)&self->Emitter), (sTarget00345C90*)self->ParticleData, -1.0f);
        cBaseClass_DynamicEmitter_readFromReplayFrame(((char*)&self->Emitter), (cStream00371318*)stream);
        cBaseClass_DynamicEmitter_updateBASE(((char*)&self->Emitter), (sVec4_00345C90*)&self->Pos, (sVec4_00345C90*)&self->Vel, 1, 0.01666666753590107f);
    } else {
        self->ParticleData = 0;
    }
    D_004A3FFC++;
    return (self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier__cDynamicParticleModifier);
#ifdef SKIP_ASM
extern int D_004A3FFC;
extern void* cDynamicParticleModifier_vtbl[];
void operator_delete(int*);
extern "C" void cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(void* self, int flags);
extern "C" void cEffectModifier__cEffectModifier(cEffectModifier* self, int flags);

/**
 * Destroys a dynamic particle modifier: frees its 0xD8-byte buffer (+0x260), decrements the live count
 * (D_004A3FFC), destroys the embedded dynamic emitter (+0x60), then the cEffectModifier part.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cDynamicParticleModifier__cDynamicParticleModifier(cDynamicParticleModifier* self, int flags)
{
    self->mVtbl = cDynamicParticleModifier_vtbl;
    int* buf = (int*)self->ParticleData;
    if (buf != 0) {
        operator_delete(buf);
    }
    D_004A3FFC--;
    cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(&self->Emitter, 2);
    cEffectModifier__cEffectModifier((cEffectModifier*)self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_updatePosition);
#ifdef SKIP_ASM
struct sEffVec4_5EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEffMtx_5EF8 {
    sEffVec4_5EF8 r[4];
};

struct sEffMtx_62A0;
extern "C" void cInstanceNode_buildMeshMat(void* a, int b, sEffMtx_62A0* out);

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline sEffVec4_5EF8 effMtxApply5EF8(sEffMtx_5EF8* m, sEffVec4_5EF8* v)
{
    sEffVec4_5EF8 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(*v), "r"(m)
        : "memory");
    return out;
}

struct sEffectLink5EF8 {
    char pad_0x0[0xC];
    void* model;            // 0xC
    int bone;               // 0x10
    char pad_0x14[0xC];
    sEffVec4_5EF8 localA;   // 0x20
    sEffVec4_5EF8 worldA;   // 0x30
    sEffVec4_5EF8 worldB;   // 0x40
    sEffVec4_5EF8 localB;   // 0x50
};

/**
 * Moves the effect to follow the mesh it is attached to.
 *
 * Builds the mesh matrix of Instance/Mesh with cInstanceNode_buildMeshMat() and sets Pos to that matrix applied to
 * Offset and Vel to it applied to InitVel.
 */
extern "C" void cDynamicParticleModifier_updatePosition(cDynamicParticleModifier* self)
{
    sEffMtx_5EF8 meshMat;
    cInstanceNode_buildMeshMat(self->Instance, self->Mesh, (sEffMtx_62A0*)&meshMat);
    *(sEffVec4_5EF8*)&self->Pos = effMtxApply5EF8(&meshMat, (sEffVec4_5EF8*)&self->Offset);
    *(sEffVec4_5EF8*)&self->Vel = effMtxApply5EF8(&meshMat, (sEffVec4_5EF8*)&self->InitVel);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_update);
#ifdef SKIP_ASM
struct sEffectLink5EF8;
extern "C" void cDynamicParticleModifier_updatePosition(cDynamicParticleModifier* self);
extern "C" void cBaseClass_DynamicEmitter_updateBASE(void* sub, void* a, void* b, int flag, float dt);

/**
 * Advances a dynamic particle effect by one 1/60 s step and marks it finished when it is done.
 *
 * Follows its link first (cDynamicParticleModifier_updatePosition), then updates the emitter at +0x60 with the positions at
 * +0x30/+0x40. While fading (+0x14) it stops emitting and finishes once no particles are left (+0x240);
 * otherwise it keeps emitting and finishes when a non-negative life (+0x60) runs out. Does nothing once
 * finished (+0x18).
 */
extern "C" void cDynamicParticleModifier_update(cDynamicParticleModifier* self)
{
    if (self->Complete != 0) {
        return;
    }
    cDynamicParticleModifier_updatePosition(self);
    if (self->Fade > 0) {
        float* sub = (float*)(&self->Emitter);
        if (self->Emitter.m_enabledCount > 0) {
            cBaseClass_DynamicEmitter_updateBASE(sub, &self->Pos, &self->Vel, 0, 0.01666666753590107f);
            return;
        }
    } else {
        float* sub = (float*)(&self->Emitter);
        if (self->Emitter.Duration < 0.0f) {
            cBaseClass_DynamicEmitter_updateBASE(sub, &self->Pos, &self->Vel, 1, 0.01666666753590107f);
            return;
        }
        cBaseClass_DynamicEmitter_updateBASE(sub, &self->Pos, &self->Vel, 1, 0.01666666753590107f);
        if (!(self->Emitter.Duration <= 0.0f)) {
            return;
        }
    }
    self->Complete = 1;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_fadeEffect__FPv);
#ifdef SKIP_ASM
/**
 * Makes a dynamic particle effect fade out instead of stopping at once.
 *
 * Sets the fade flag (offset 0x14). cDynamicParticleModifier_update then stops emitting and marks the effect
 * finished once the emitter's count at offset 0x240 (probably its live particles) reaches 0.
 */
void cDynamicParticleModifier_fadeEffect(void* vself)
{
    cDynamicParticleModifier* self = (cDynamicParticleModifier*)vself;
    self->Fade = 1;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_render);
#ifdef SKIP_ASM
extern "C" void cDynamicEmitter_render(void*, int);

/**
 * Renders the dynamic particle modifier's emitter unless the effect has finished.
 *
 * Calls cDynamicEmitter_render() with 7 on the emitter embedded at offset 0x60 while the finished flag
 * (offset 0x18) is clear.
 */
extern "C" void cDynamicParticleModifier_render(cDynamicParticleModifier* self)
{
    if (self->Complete == 0) {
        cDynamicEmitter_render((char*)&self->Emitter, 7);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", cDynamicParticleModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffectLinkVEntry60A0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cBaseClass_DynamicEmitter_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a dynamic particle modifier's state to a replay frame.
 *
 * Writes the cEffectModifier part and the 0x50 bytes at +0x10 (which include the fade and finished flags);
 * while not finished, also the 0xD8-byte buffer (+0x260) and the emitter's state (cBaseClass_DynamicEmitter_writeToReplayFrame).
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cDynamicParticleModifier_writeToReplayFrame(cDynamicParticleModifier* self, void* stream)
{
    cEffectModifier_writeToReplayFrame((cEffectModifier*)self, stream);
    sEffectLinkVEntry60A0* writeEntry = &(*(sEffectLinkVEntry60A0**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)&self->Mesh, 80);
    if (self->Complete == 0) {
        writeEntry = &(*(sEffectLinkVEntry60A0**)stream)[1];
        writeEntry->fn((char*)stream + writeEntry->delta, self->ParticleData, 216);
        cBaseClass_DynamicEmitter_writeToReplayFrame((char*)&self->Emitter, stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier_cHaloModifier);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* cHaloModifier_vtbl[];
extern "C" void* cEffectModifier_cEffectModifier(cEffectModifier* self, int a1);

struct sEffQuat_6120 {
    float x, y, z, w;
    sEffQuat_6120(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
} __attribute__((aligned(16)));

struct sEffObj_6120 {
    sEffectLinkView link;   // 0x0
    void** vt;          // 0x8
    int value;          // 0xC
    sEffQuat_6120 q;    // 0x10
    float f20;          // 0x20
    int f24;            // 0x24
    int f28;            // 0x28
    int f2C;            // 0x2C
    int f30;            // 0x30
};

struct sEffSrc_6120 {
    int f0;     // 0x0
    int f4;     // 0x4
    int f8;     // 0x8
    float w;    // 0xC
    float x;    // 0x10
    float y;    // 0x14
    float z;    // 0x18
    float f1C;  // 0x1C
    int f20;    // 0x20
};

/**
 * Constructs a halo modifier for an instance from halo make-data.
 *
 * Runs cEffectModifier_cEffectModifier, installs cHaloModifier_vtbl and copies from src: Type (+0x4) to +0x24, Mesh
 * (+0x8) to +0x28, the colour (ColourR/G/B/A at +0x10..+0x18 and +0xC) to +0x10 as (R, G, B, A), Radius (+0x1C) to
 * +0x20 and Speed (+0x20) to +0x2C; zeroes +0x30 and increments the halo count D_004A4000.
 *
 * @param src  halo make-data (tAddHaloData layout)
 * @param instRecord  instance record the halo belongs to
 * @return self
 */
extern "C" void* cHaloModifier_cHaloModifier(sEffObj_6120* self, sEffSrc_6120* src, int instRecord)
{
    cEffectModifier_cEffectModifier((cEffectModifier*)self, instRecord);
    self->vt = cHaloModifier_vtbl;
    self->f24 = src->f4;
    self->f28 = src->f8;
    self->q = sEffQuat_6120(src->x, src->y, src->z, src->w);
    self->f20 = src->f1C;
    self->f2C = src->f20;
    self->f30 = 0;
    D_004A4000++;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier_cHaloModifier1);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* cHaloModifier_vtbl[];
extern "C" void* cEffectModifier_cEffectModifier1(cEffectModifier* p, void* obj);

struct sEffVEntry61C0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Constructs a halo modifier from a replay frame.
 *
 * Runs the effect-modifier replay constructor cEffectModifier_cEffectModifier1, installs cHaloModifier_vtbl, reads the 0x30-byte halo
 * state at +0x10 through the stream's read slot (vtable entry 2) and increments the halo count D_004A4000.
 *
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cHaloModifier_cHaloModifier1(cHaloModifier* self, void* stream)
{
    cEffectModifier_cEffectModifier1((cEffectModifier*)self, stream);
    self->mVtbl = cHaloModifier_vtbl;
    sEffVEntry61C0* vt = *(sEffVEntry61C0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)&self->Color, 0x30);
    D_004A4000++;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier__cHaloModifier);
#ifdef SKIP_ASM
extern int D_004A4000;
extern void* cHaloModifier_vtbl[];
extern "C" void cEffectModifier__cEffectModifier(cEffectModifier* self, int flags);


/**
 * Destroys a halo modifier: decrements the live count (D_004A4000), then runs the cEffectModifier destructor.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cHaloModifier__cHaloModifier(cHaloModifier* self, int flags)
{
    (self)->mVtbl = cHaloModifier_vtbl;
    D_004A4000--;
    cEffectModifier__cEffectModifier((cEffectModifier*)self, flags);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier_update);
#ifdef SKIP_ASM
/**
 * Spins a halo: adds the integer step (+0x2C) to its angle in degrees (+0x30), resetting it to 0 at +/-360.
 */
extern "C" void cHaloModifier_update(cHaloModifier* self)
{
    float angle = self->Angle + (float)self->Speed;
    self->Angle = angle;
    if (angle >= 360.0f || angle <= -360.0f) {
        self->Angle = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cHaloModifier_render);
#ifdef SKIP_ASM
struct sEffVec4_62A0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEffMtx_62A0 {
    sEffVec4_62A0 r[4];
};

extern "C" void cInstanceNode_buildMeshMat(void* a, int b, sEffMtx_62A0* out);
extern "C" void cObjectInterface_renderSimpleLightHalo(sEffVec4_62A0* pos, void* p, int n, float a, float b);

/**
 * Draws a halo at the position of its instance's mesh.
 *
 * Gets the world matrix of mesh +0x28 of the instance (+0xC) and draws the halo sprite there with
 * cObjectInterface_renderSimpleLightHalo(), passing the halo data (+0x10), +0x24, the size (+0x20) and the current angle (+0x30).
 */
extern "C" void cHaloModifier_render(cHaloModifier* self)
{
    sEffMtx_62A0 matrix;
    void* colour = (char*)&self->Color;
    cInstanceNode_buildMeshMat(self->Instance, self->Mesh, &matrix);
    sEffVec4_62A0 origin = matrix.r[3];
    cObjectInterface_renderSimpleLightHalo(&origin, colour, self->Type,
                  self->Radius, self->Angle);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", cHaloModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sEffectLinkVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cEffectModifier_writeToReplayFrame(cEffectModifier* self, void* stream);

/**
 * Writes a halo modifier's state to a replay frame.
 *
 * Writes the effect-modifier part with cEffectModifier_writeToReplayFrame(), then the 0x30 bytes at offset 0x10
 * through the stream's write virtual (slot 1).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cHaloModifier_writeToReplayFrame(cHaloModifier* self, void* stream)
{
    cEffectModifier_writeToReplayFrame((cEffectModifier*)self, stream);
    sEffectLinkVEntry* vt = *(sEffectLinkVEntry**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)&self->Color, 0x30);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cFlexBridgeNode_cFlexBridgeNode);
#ifdef SKIP_ASM
struct sEffV3_346350 {
    float x, y, z;
    sEffV3_346350() {}
};
struct sEffV2_346350 {
    float u, v;
    sEffV2_346350() {}
};
struct sEffV4_346350 {
    float x, y, z, w;
    sEffV4_346350() {}
};

struct sEffVE_346350 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, sEffV3_346350*, sEffV2_346350*, sEffV4_346350*);
};

extern "C" void* cInstanceNode_cInstanceNode1(void* self, int a1, int a2, int a3);
void cSpring_cSpring(void* self);
extern "C" void cSpring_setupSpring(void* self, int a1, float len);
extern "C" void cFlexBridgeNode_initSpringNodes(void* self);
extern "C" void cFlexBridgeNode_setupGrid(void* self);
extern "C" void cFlexBridgeNode_updateGrid(void*);
extern void* cFlexBridgeNode_vtbl[];

// PORT: SN sqrt.s asm helper (PS2 FPU).
static inline float sqrtf_346350(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct sEffFlags_346350 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
};

/**
 * Constructs a flexible bridge node for a world instance.
 *
 * Builds the spring chain (+0x50) from the bridge data and the length of the instance's first edge (the first
 * two mesh vertices, through graphics manager slot 97), then sets up the strip mesh, the spring nodes and the
 * first pose, and updates the instance's flag bits. Also sets +0x40 = 100 and +0x44 = 12 (meaning unknown).
 *
 * @param bucket  update bucket
 * @param pInst   the world instance (On Tour tInstanceRecord*)
 * @param tdata   the bridge data (On Tour tMakeFlexBridgeData*)
 * @return self
 */
extern "C" void* cFlexBridgeNode_cFlexBridgeNode(cFlexBridgeNode* self, int bucket, int pInst, int tdata)
{
    cInstanceNode_cInstanceNode1(self, bucket, 5, pInst);
    self->mVtbl = cFlexBridgeNode_vtbl;
    cSpring_cSpring((char*)&self->Mesh);
    self->TextureId = 12;
    self->RailHeight = 100.0f;
    sEffV3_346350 pts[4];
    sEffV2_346350 uv[4];
    sEffV4_346350 col[4];
    sEffVE_346350* vt = (sEffVE_346350*)cGraphicsMan_TheGraphicsMan->mVtbl;
    vt[97].fn((char*)cGraphicsMan_TheGraphicsMan + vt[97].delta, self->Instance, 0, pts, uv, col);
    sEffV3_346350 span;
    span.x = pts[1].x - pts[0].x;
    span.y = pts[1].y - pts[0].y;
    span.z = pts[1].z - pts[0].z;
    cSpring_setupSpring((char*)&self->Mesh, tdata, sqrtf_346350(span.x * span.x + span.y * span.y + span.z * span.z));
    cFlexBridgeNode_setupGrid(self);
    cFlexBridgeNode_initSpringNodes(self);
    cFlexBridgeNode_updateGrid(self);
    unsigned int* flags = (unsigned int*)((char*)self->Instance + 8);
    *flags = (*flags & 0xFFFFFFFD) | 4;
    flags = (unsigned int*)((char*)self->Instance + 8);
    *flags = (*flags & 0xFFFFFFDF) | 0x40;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cFlexBridgeNode_cFlexBridgeNode1);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern "C" void* cSpring_cSpring1(void* self, void* stream);
extern "C" void cFlexBridgeNode_setupGrid(void* self);
extern "C" void cFlexBridgeNode_updateGrid(void*);
extern void* cFlexBridgeNode_vtbl[];

struct sEffectLinkVEntry64E0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Constructs a flexible bridge node from a replay frame.
 *
 * Runs the cInstanceNode replay constructor, reads the spring chain (cSpring_cSpring1), sets up the strip mesh,
 * reads 0x30 bytes at +0x20 and poses the strip.
 *
 * @param bucket  update bucket
 * @param stream  the replay frame to read (On Tour cBxReplayFrame*)
 * @return self
 */
extern "C" void* cFlexBridgeNode_cFlexBridgeNode1(cFlexBridgeNode* self, void* bucket, void* stream)
{
    void* payload = (char*)&self->BottomLeftOffset;
    cInstanceNode_cInstanceNode(self, bucket, stream);
    self->mVtbl = cFlexBridgeNode_vtbl;
    cSpring_cSpring1((char*)&self->Mesh, stream);
    cFlexBridgeNode_setupGrid(self);
    sEffectLinkVEntry64E0* readEntry = &(*(sEffectLinkVEntry64E0**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, payload, 0x30);
    cFlexBridgeNode_updateGrid(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/effectlink", cFlexBridgeNode__cFlexBridgeNode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cMemMan_free(void* ptr);
extern "C" void* cObjectInterface_getNash();
extern "C" void cSpring__cSpring(void* self, int flags);
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);

struct sVec4_346568 {
    float x, y, z, w;
} __attribute__((aligned(16)));


struct sInst_346568 {
    char pad_0x00[0x60];
    float min[3];
    float max[3];
};

extern "C" void nash_Nash_movenode(void* world, int type, void* id, tBoundBox* box, tBoundBox* old);

struct sRVEntry_346568 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
struct sRCtx_346568 {
    char pad[0x10D8];
    sRVEntry_346568* vt;
};

/**
 * Destroys a flexible bridge node and puts its instance back into the space hash with its own bounding box.
 *
 * Frees the face, normal and vertex arrays, releases the strip mesh (graphics manager slot 77), moves the
 * instance in the Nash grid from the bridge's box to the instance's own box, destroys the spring chain and
 * runs the cInstanceNode destructor.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cFlexBridgeNode__cFlexBridgeNode(cFlexBridgeNode* self, int flags)
{
    self->mVtbl = cFlexBridgeNode_vtbl;
    if (self->Faces != 0) {
        cMemMan_free(self->Faces);
    }
    if (self->FaceNorms != 0) {
        cMemMan_free(self->FaceNorms);
    }
    if (self->Verts != 0) {
        cMemMan_free(self->Verts);
    }
    tBoundBox* box = ((&self->BoundBox));
    ((sRVEntry_346568*)cGraphicsMan_TheGraphicsMan->mVtbl)[77].fn((char*)cGraphicsMan_TheGraphicsMan + ((sRVEntry_346568*)cGraphicsMan_TheGraphicsMan->mVtbl)[77].delta, self->GridMesh);
    tBoundBox old = *(&self->BoundBox);
    sInst_346568* inst = (sInst_346568*)self->Instance;
    sVec4_346568 corner;
    corner.x = inst->min[0];
    corner.y = inst->min[1];
    corner.z = inst->min[2];
    corner.w = 1.0f;
    *(sVec4_346568*)&self->BoundBox = corner;
    corner.x = inst->max[0];
    corner.y = inst->max[1];
    corner.z = inst->max[2];
    corner.w = 1.0f;
    *(sVec4_346568*)&self->BoundBox.MaxPoint = corner;
    nash_Nash_movenode(cObjectInterface_getNash(), 0, inst, box, &old);
    cSpring__cSpring(&self->Mesh, 2);
    cInstanceNode__cInstanceNode(((char*)self), flags);
}
#endif

