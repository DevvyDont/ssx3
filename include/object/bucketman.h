#ifndef OBJECT_BUCKETMAN_H
#define OBJECT_BUCKETMAN_H

// cBucketMan: the scene's bucket manager - an array of buckets (tBucketHeader), each a doubly linked list of scene
// nodes (cObjNode) between a Head and a Tail sentinel, plus a pending-add list (AddHead .. AddTail) that
// mergeBucket sorts into the main list when the bucket's sort flag is set. The game updates / applies / renders the
// buckets in order (updateBucket, applyBucket, renderBucket); cObjNode's ctor adds itself to a bucket and its dtor
// removes it. One instance: the global gBucketMan (0x004A5988). EA's source file is not known (no preview assert
// path; On Tour: object/all_in_one_object.cpp); retail: splat unit object/bucketman (init 0x003549E0 .. setsortflag
// 0x00355260) plus the ctor cBucketMan_cBucketMan__FPv at the end of object/objnode (0x003549D0).
//
// Size 0x8: [O] On Tour class cBucketMan is 0x8 (NumBuckets, Buckets); [R] cBucketMan_cBucketMan zeroes exactly the
// words at 0x0 and 0x4; [U] every method reads only 0x0 / 0x4. No vtable, no bases.
// tBucketHeader (0x44): [R] cBucketMan_init: operator new[](n * 0x44, "Buckets") (the array cookie at -0x10; purge
// frees Buckets - 0x10), each element a Flags word and four cObjNode sentinels whose ctor stores cObjNode_vtbl at
// their +0xC (+0x10, +0x20, +0x30, +0x40 of the bucket); cBucketMan_purge runs cObjNode__cObjNode(…, 2) on +0x34, +0x24,
// +0x14, +0x4. [O] On Tour struct tBucketHeader is 0x34 (its cObjNode is 0xC; retail's is 0x10).
//
// A FLAT layout, like main/game.h. Tags: [R] retail ctor / init / purge; [U] how retail code uses the field
// (add, remove, unlink, first/last/next/prev, addfirst/addnext, mergeBucket, setsortflag); [O] SSX On Tour's DWARF
// (class cBucketMan, struct tBucketHeader: member names and order). No preview evidence (no accessors, no pairs).

#include "layout.h"
#include "object/objnode.h"

struct tBucketHeader {
    int Flags;                      // 0x000 [O]; [R] init clears it; [U] bit 0 = the bucket wants sorting:
                                    //       setsortflag sets it, add puts new nodes on the add list while it is set,
                                    //       mergeBucket merges and clears it
    cObjNode Head;                  // 0x004 [O]; [R] init: Head.Next = &Tail, Head.Prev = 0; [U] first() returns
                                    //       Head.Next; add links after it
    cObjNode Tail;                  // 0x014 [O]; [R] init: Tail.Prev = &Head, Tail.Next = 0; [U] last() returns
                                    //       Tail.Prev; next() stops at it
    cObjNode AddHead;               // 0x024 [O]; [R] init: AddHead.Next = &AddTail; [U] add (sort flag set),
                                    //       addfirst / addnext, mergeBucket
    cObjNode AddTail;               // 0x034 [O]; [R] init: AddTail.Prev = &AddHead
};

CHECK_OFFSET(tBucketHeader, Flags, 0x000);
CHECK_OFFSET(tBucketHeader, Head, 0x004);
CHECK_OFFSET(tBucketHeader, Tail, 0x014);
CHECK_OFFSET(tBucketHeader, AddHead, 0x024);
CHECK_OFFSET(tBucketHeader, AddTail, 0x034);
CHECK_SIZE(tBucketHeader, 0x044);

struct cObjNode;

struct cBucketMan {
    int NumBuckets;                 // 0x000 [O]; [R] ctor zeroes it, init stores numBuckets, purge zeroes it; [U] the
                                    //       purge loop bound
    tBucketHeader* Buckets;         // 0x004 [O]; [R] ctor zeroes it; init: new tBucketHeader[n] ("Buckets"); purge
                                    //       destroys and frees it; [U] every bucket method indexes it (index * 0x44)

    // Methods bound to their retail symbols by asm labels.
    void init(int n) __asm__("cBucketMan_init");
    void purge() __asm__("cBucketMan_purge");
    void mergeBucket(int index) __asm__("cBucketMan_mergeBucket");
    void* first(int index) __asm__("cBucketMan_first__FP10cBucketMani");
    void* last(int index) __asm__("cBucketMan_last");
    void* prev(cObjNode* node, int index) __asm__("cBucketMan_prev");
    void* addfirst(int index) __asm__("cBucketMan_addfirst__FP10cBucketMani");
    void* addnext(cObjNode* node, int index) __asm__("cBucketMan_addnext");
    void updateBucket(int index) __asm__("cBucketMan_updateBucket");
    void applyBucket(int index) __asm__("cBucketMan_applyBucket");
    void renderBucket(int index) __asm__("cBucketMan_renderBucket");
    void purgeBucket(int index) __asm__("cBucketMan_purgeBucket");
    void purgeBucket1(int index, int key) __asm__("cBucketMan_purgeBucket1");
    void setsortflag(int bucket) __asm__("cBucketMan_setsortflag");
};

CHECK_OFFSET(cBucketMan, NumBuckets, 0x000);
CHECK_OFFSET(cBucketMan, Buckets, 0x004);
CHECK_SIZE(cBucketMan, 0x008);

#endif /* OBJECT_BUCKETMAN_H */
