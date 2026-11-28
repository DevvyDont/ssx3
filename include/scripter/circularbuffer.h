#ifndef SCRIPTER_CIRCULARBUFFER_H
#define SCRIPTER_CIRCULARBUFFER_H

// cCircularBuffer: the NIS list's ring of five entries (scripter/videngine): head and tail indexes and a full flag.
// Methods: AddToTail, RemoveFromHead, operator[], IsEmpty / IsFull, GetNumEntries, Reset.
//
// Size 0x98 (On Tour's; retail's methods reach 0x94, the last member). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods
// falls on a member start (tools/gen/self_offsets.py). Enums and bools are words; members of class type are byte arrays
// (type in the comment).

#include "layout.h"


struct tNISListEntry;

struct cCircularBuffer {
    // The ring's size (name ours): [O] cCircularBuffer<5, tNISListEntry>; [U] IncBufferIndex wraps after
    // kNumEntries - 1, GetNumEntries counts modulo kNumEntries.
    enum { kNumEntries = 5 };

    char mBuffer[0x8C];             // 0x000 [O] tNISListEntry[kNumEntries]
    int mHeadIndex;                 // 0x08C [O]
    int mTailIndex;                 // 0x090 [O]
    int mIsFull;                    // 0x094 [O] bool

    // Methods bound to their retail symbols by asm labels.
    void Reset() __asm__("cCircularBuffer_Reset__FPv");
    int IsFull() __asm__("cCircularBuffer_IsFull__FPv");
    int IncBufferIndex(int index) __asm__("cCircularBuffer_IncBufferIndex");
    bool AddToTail(tNISListEntry* cmd) __asm__("cCircularBuffer_AddToTail");
    int GetNumEntries() __asm__("cCircularBuffer_GetNumEntries");
    void* operatorIndex(int index) __asm__("cCircularBuffer_operatorIndex");
    int IsEmpty() __asm__("cCircularBuffer_IsEmpty");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cCircularBuffer();
};

CHECK_OFFSET(cCircularBuffer, mBuffer, 0x000);
CHECK_OFFSET(cCircularBuffer, mHeadIndex, 0x08C);
CHECK_OFFSET(cCircularBuffer, mTailIndex, 0x090);
CHECK_OFFSET(cCircularBuffer, mIsFull, 0x094);
CHECK_SIZE(cCircularBuffer, 0x98);

#endif
