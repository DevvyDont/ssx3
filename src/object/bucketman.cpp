#include "common.h"

struct cBucketMan {
    char pad_0x00[0x4];
    char* mBuckets; // 0x4, element size 0x44
};

INCLUDE_ASM("object/bucketman", cBucketMan_init);

INCLUDE_ASM("object/bucketman", func_00354B38);

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_add);
#ifdef SKIP_ASM
struct sBucketLink {
    int field_0x0;
    sBucketLink* prev; // 0x4
    sBucketLink* next; // 0x8
};

struct sBucketLists {
    int flags;              // 0x00
    sBucketLink head0;      // 0x04
    char pad_0x10[0x14];    // 0x10
    sBucketLink head1;      // 0x24
    char pad_0x30[0x14];    // 0x30
};

extern "C" void cBucketMan_add(cBucketMan* self, sBucketLink* node, int index)
{
    if (((sBucketLists*)self->mBuckets)[index].flags & 1) {
        node->prev = ((sBucketLists*)self->mBuckets)[index].head1.prev;
        ((sBucketLists*)self->mBuckets)[index].head1.prev->next = node;
        node->next = (sBucketLink*)(self->mBuckets + index * 0x44 + 0x24);
        ((sBucketLists*)self->mBuckets)[index].head1.prev = node;
    } else {
        node->prev = ((sBucketLists*)self->mBuckets)[index].head0.prev;
        ((sBucketLists*)self->mBuckets)[index].head0.prev->next = node;
        node->next = (sBucketLink*)(self->mBuckets + index * 0x44 + 0x4);
        ((sBucketLists*)self->mBuckets)[index].head0.prev = node;
    }
}
#endif

INCLUDE_ASM("object/bucketman", func_00354C98);

//100%
INCLUDE_ASM("object/bucketman", func_00354E48);
#ifdef SKIP_ASM
struct sBucketNode {
    int field_0x0;
    sBucketNode* prev; // 0x4
    sBucketNode* next; // 0x8
};

extern "C" void func_00354E48(cBucketMan* self, sBucketNode* node)
{
    if (node->prev != 0 && node->next != 0) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_first__FP10cBucketMani);
#ifdef SKIP_ASM
// PORT: holds a pointer in an int (needed to match); not 64-bit safe.
void* cBucketMan_first(cBucketMan* self, int index)
{
    index *= 0x44;
    index += (int)self->mBuckets;
    void* head = *(void**)(index + 0x8);
    return (head == (void*)(index + 0x14)) ? 0 : head;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", func_00354ED0);
#ifdef SKIP_ASM
// PORT: holds a pointer in an int (same shape as cBucketMan_first); not 64-bit safe.
extern "C" void* func_00354ED0(cBucketMan* self, int index)
{
    index *= 0x44;
    index += (int)self->mBuckets;
    void* tail = *(void**)(index + 0x1C);
    return (tail == (void*)(index + 0x4)) ? 0 : tail;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_next__FP10cBucketManPvi);
#ifdef SKIP_ASM
void* cBucketMan_next(cBucketMan* self, void* node, int index)
{
    char* bucket = self->mBuckets + index * 0x44;
    void* next = *(void**)((char*)node + 0x4);
    return (next == (void*)(bucket + 0x14)) ? 0 : next;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", func_00354F20);
#ifdef SKIP_ASM
extern "C" void* func_00354F20(cBucketMan* self, void* node, int index)
{
    void* next = *(void**)((char*)node + 0x8);
    return (next == (void*)(self->mBuckets + index * 0x44 + 0x4)) ? 0 : next;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", cBucketMan_addfirst__FP10cBucketMani);
#ifdef SKIP_ASM
// PORT: holds a pointer in an int (needed to match); not 64-bit safe.
void* cBucketMan_addfirst(cBucketMan* self, int index)
{
    index *= 0x44;
    index += (int)self->mBuckets;
    void* head = *(void**)(index + 0x28);
    return (head == (void*)(index + 0x34)) ? 0 : head;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", func_00354F70);
#ifdef SKIP_ASM
extern "C" void* func_00354F70(cBucketMan* self, void* node, int index)
{
    void* next = *(void**)((char*)node + 0x4);
    return (next == (void*)(self->mBuckets + index * 0x44 + 0x34)) ? 0 : next;
}
#endif

INCLUDE_ASM("object/bucketman", func_00354F98);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", func_00355028);
#ifdef SKIP_ASM
struct sBucketVEntry5028 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00355028(cBucketMan* self, int index)
{
    void* node = cBucketMan_first(self, index);
    while (node != 0) {
        sBucketVEntry5028* vt = *(sBucketVEntry5028**)((char*)node + 0xC);
        vt[3].fn((char*)node + vt[3].delta);
        node = cBucketMan_next(self, node, index);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/bucketman", func_003550A0);
#ifdef SKIP_ASM
struct sBucketVEntry50A0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_003550A0(cBucketMan* self, int index)
{
    void* node = cBucketMan_first(self, index);
    while (node != 0) {
        sBucketVEntry50A0* vt = *(sBucketVEntry50A0**)((char*)node + 0xC);
        vt[4].fn((char*)node + vt[4].delta);
        node = cBucketMan_next(self, node, index);
    }
}
#endif

INCLUDE_ASM("object/bucketman", cBucketMan_purgeBucket);

INCLUDE_ASM("object/bucketman", func_003551A8);

// 0x44-byte elements reached through a pointer at self+0x4
struct sBucketEntry {
    int flags;
    char pad_0x04[0x40];
};

//100%
INCLUDE_ASM("object/bucketman", func_00355260);
#ifdef SKIP_ASM
extern "C" void func_00355260(void* self, int a1)
{
    sBucketEntry* p = *(sBucketEntry**)((char*)self + 0x4);
    p[a1].flags |= 1;
}
#endif

//100%
INCLUDE_ASM("object/bucketman", func_00355280);
#ifdef SKIP_ASM
extern void* D_00491028[];
extern "C" void* func_0034FB00(void* self, void* a1, int type, void* a3);

extern "C" void* func_00355280(void* self, void* a1, int type, void* a3)
{
    func_0034FB00(self, a1, type, a3);
    *(void***)((char*)self + 0xC) = D_00491028;
    *(float*)((char*)self + 0x24) = 1.0f;
    *(unsigned short*)((char*)self + 0x12) |= 1;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x28) = 0;
    return self;
}
#endif

