#include "common.h"
#include "object/instanceman.h"

//100%
INCLUDE_ASM("wscript/wscriptcompile", cInstanceMan_cInstanceMan);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_Instance[];

struct cWScriptKeyword {
    cWScriptKeyword* next;
    char pad04[0x74];
    unsigned int f78;
    unsigned char f7C;
    unsigned char f7D;
    short f7E;
    char pad80[0x30];
};

struct cWScriptCompileKw {
    int f0;
    int count;
    cWScriptKeyword* free;
    cWScriptKeyword* pool;
};

/**
 * Despite the name, constructs the instance manager: a pool of instance records on a free list.
 *
 * Its only caller (gameload.cpp) builds the game's InstanceMan with it, and On Tour's cInstanceMan
 * constructor has
 * the same (instanceSectionId, numInstances) parameters. Allocates @p n 0xB0-byte records ("Instance"),
 * resets
 * each and links them all into the free list.
 *
 * @param instanceSectionId  instance section ID, stored in the first word (the game passes 0x80)
 * @param n  number of instance records (On Tour numInstances)
 * @return @p self
 */
extern "C" cWScriptCompileKw* cInstanceMan_cInstanceMan(cWScriptCompileKw* self, int instanceSectionId, int n)
{
    int i;
    self->f0 = instanceSectionId;
    self->count = n;
    cWScriptKeyword** slot = &self->pool;
    cWScriptKeyword* mem = (cWScriptKeyword*)operator_new_tag(n * 176, str_Instance, 0x20000000, 0);
    cWScriptKeyword* keyword = mem;
    int j;
    for (j = n - 1; j != -1; j--, keyword++) {
        keyword->f78 = 0xFFFFFFFF;
        keyword->f7C = 0;
        keyword->f7D = 0xFF;
        keyword->f7E = -1;
    }
    *slot = mem;
    self->free = 0;
    for (i = 0; i < self->count; i++) {
        self->pool[i].next = self->free;
        self->free = &self->pool[i];
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptcompile", cInstanceMan__cInstanceMan);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void cMemMan_free(void* p);

/**
 * Destroys the world instance manager.
 *
 * Frees the array at +0xC with cMemMan_free if there is one, then calls operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cInstanceMan__cInstanceMan(cInstanceMan* self, int flags)
{
    void* instances = self->Instances;
    if (instances != 0) {
        cMemMan_free(instances);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptcompile", cInstanceMan_allocInstance1);
#ifdef SKIP_ASM
struct sWSNode {
    sWSNode* next;
    int pad4;
    unsigned int flags;
    char pad0C[0x78 - 0xC];
    unsigned int type : 8;
    unsigned int index : 24;
    char pad7C[0xB0 - 0x7C];
};

struct sWSPool {
    unsigned char type;
    char pad1[7];
    sWSNode* free;
    sWSNode* base;
};

/**
 * Takes an instance record off the instance manager's free list (+0x8) and stamps it.
 *
 * The record's type byte (+0x78) becomes the manager's section ID (low byte of +0x0) and its 24-bit index the record's
 * position in the record array (+0xC, 0xB0 bytes each); flag 0x2000 is set in its flags word (+0x8).
 *
 * Likely bug: an empty free list is not handled; the null record is written to.
 *
 * @return the record
 */
extern "C" sWSNode* cInstanceMan_allocInstance1(cInstanceMan* self)
{
    sWSNode* node = (sWSNode*)self->FreeInstances;
    if (node != 0) {
        self->FreeInstances = (tInstanceList*)node->next;
    }
    node->type = *(unsigned char*)&self->InstanceSectionId;
    node->index = node - (sWSNode*)self->Instances;
    node->flags |= 0x2000;
    return node;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptcompile", cInstanceMan_allocInstance);
#ifdef SKIP_ASM
/**
 * Takes a specific instance record out of the instance manager's free list.
 *
 * The record's index is @p id >> 8. When it is found on the free list it is unlinked, given the manager's type
 * (On Tour InstanceSectionId), flag 0x2000 and its index.
 *
 * @param id  the resource ID of the instance (On Tour tResourceID); bits 8 and up are the record index
 * @return the record, or 0 when it is not on the free list
 */
extern "C" sWSNode* cInstanceMan_allocInstance(cInstanceMan* self, unsigned int id)
{
    sWSNode** link = (sWSNode**)&self->FreeInstances;
    sWSNode* inst = (sWSNode*)self->FreeInstances;
    while (inst != 0) {
        if (inst - (sWSNode*)self->Instances == (id >> 8)) {
            *link = inst->next;
            inst->type = *(unsigned char*)&self->InstanceSectionId;
            int idx = inst - (sWSNode*)self->Instances;
            inst->flags |= 0x2000;
            inst->index = idx;
            return inst;
        }
        link = &inst->next;
        inst = inst->next;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptcompile", cInstanceMan_freeInstance__FPvT0);
#ifdef SKIP_ASM
/**
 * Puts an instance record back on the instance manager's free list (+0x8): the record's first word links to the old
 * head.
 *
 * @param node  the instance record
 * @return the previous list head
 */
int cInstanceMan_freeInstance(void* vself, void* node)
{
    cInstanceMan* self = (cInstanceMan*)vself;
    int oldHead = (int)self->FreeInstances;
    *(int*)node = oldHead;
    self->FreeInstances = (tInstanceList*)((int)node);
    return oldHead;
}
#endif

