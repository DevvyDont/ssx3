#ifndef REPLAY_FRAMEPTR_H
#define REPLAY_FRAMEPTR_H

// cReplayFramePtr: a replay data block with a write cursor and a read cursor: the stream every replayable object
// writes itself to and reads itself back from (writeToReplayFrame / readFromReplayFrame in On Tour). Derived from
// the interface cBxReplayFrame (vptr only; On Tour size 4), whose virtuals it implements: vtable cReplayFramePtr_vtbl, slots
// 1..7 = write (cReplayFramePtr_write), read (cReplayFramePtr_read), readReplayInstance() (cReplayFramePtr_readReplayInstance),
// readReplayInstance(cInstanceNode*) (cReplayFramePtr_readReplayInstance1), writeReplayInstance(const tInstanceRecord*) (cReplayFramePtr_writeReplayInstance),
// writeReplayInstance(cInstanceNode*) (cReplayFramePtr_writeReplayInstance1), destructor (cReplayFramePtr__cReplayFramePtr); the order is On Tour
// cBxReplayFrame's. cReplayFrame (replay/frame.h, vtable cReplayFrame_vtbl, ctor cReplayFrame_cReplayFrame) derives from it;
// cPlaybackMan embeds one at +0x14 (its destructor cPlaybackMan__cPlaybackMan stores cReplayFramePtr_vtbl there).
// Size 0x14 [R]: the constructor cReplayFramePtr_cReplayFramePtr stores the vptr and zeroes 0x4..0x10, and cReplayFrame's first own
// member (Next) is at 0x14 (On Tour cReplayFrame, same offsets as here). Same size, order and offsets as On Tour's
// class cReplayFramePtr.
// EA's header is ../source/replay/frameptr.h: the July 2003 preview files the inline asserts "Block && ReadData" and
// "Block && pos.Data" under it. Retail code: cReplayFramePtr_cReplayFramePtr (last function of unit replay/replay), unit
// replay/frameptr 0x0026E908-0x0026ED98, and the out-of-line inline copies in unit replay/replaycache
// 0x002721D0-0x00272248.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] the constructor cReplayFramePtr_cReplayFramePtr and the destructor cReplayFramePtr__cReplayFramePtr;
//   [P] the preview's inline accessors cReplayFramePtr::getBlock (lw 0x4), GetSize / getBlockSaveSize (lw 0x10)
//       (tools/layout_xlate.py; retail keeps identical out-of-line copies at the same offsets) and the asserts above;
//   [U] how retail code uses the field (asm of the methods listed in methods.txt);
//   [O] SSX On Tour's DWARF (class cReplayFramePtr, struct tFramePos): member names, types and order.

#include "layout.h"

struct tFramePtrSave;       // On Tour: 0x4 (int Type); loadBlock's second argument

// cBxReplayFrame: the replay stream interface every writeToReplayFrame / readFromReplayFrame takes ([P] the preview's
// linkonce names, e.g. writeToReplayFrame__8cObjNodeP14cBxReplayFrame; [O] On Tour class cBxReplayFrame, size 0x4: a
// vptr only, virtuals write(const char* data, int len), read(char* data, int len), readReplayInstance() /
// readReplayInstance(cInstanceNode*), writeReplayInstance(const tInstanceRecord*) / writeReplayInstance(cInstanceNode*)).
// g++ 2.95 vtable slots (8-byte entries, sVEntry in vtable.h; entry 0 unused): 1 write, 2 read, 3 / 4
// readReplayInstance, 5 / 6 writeReplayInstance ([U] the Luno, emitter and static-emitter replay functions call slot 1
// to write and slot 2 to read; cReplayFramePtr_vtbl implements them in that order). No data, so the vptr is at 0.
struct cBxReplayFrame {
    void** mVtbl;           // 0x00 [O] (vptr only); [U] writeToReplayFrame calls slot 1, readFromReplayFrame slot 2
};

// A saved read position (On Tour tFramePos, 0x4); getReadPos/setReadPos copy it to/from ReadData.
struct tFramePos {
    char* Data;             // 0x0 [O]; [P] preview assert "Block && pos.Data" (replay/frameptr.h); [U]
                            //     cReplayFramePtr_getReadPos stores ReadData here, cReplayFramePtr_setReadPos loads ReadData from it
};

struct cReplayFramePtr {
    // --- cBxReplayFrame (interface: a vptr only)
    void** mVtbl;           // 0x00 [R] ctor cReplayFramePtr_cReplayFramePtr and dtor cReplayFramePtr__cReplayFramePtr store cReplayFramePtr_vtbl (cReplayFrame's
                            //      ctor then stores cReplayFrame_vtbl); [U] peekObjNodeType and the readReplayInstance /
                            //      writeReplayInstance bodies call slots 1/2 (write/read) through it
    // --- cReplayFramePtr
    char* Block;            // 0x04 [O]; [P] getBlock (cReplayFramePtr_getBlock) lw 0x4; assert "Block && ReadData"; [R] ctor 0;
                            //      [U] initBlock stores the new "Frame Block" (operator new), setBlock stores data,
                            //      purgeBlock frees it; readRewind / writeRewind reset the cursors to it
    char* WriteData;        // 0x08 [O]; [R] ctor 0; [U] write (cReplayFramePtr_write) copies to it and advances it;
                            //      setBlock = data + size; loadBlock = Block + unpacked size; writeRewind = Block
    char* ReadData;         // 0x0C [O]; [P] assert "Block && ReadData"; [R] ctor 0; [U] read (cReplayFramePtr_read) copies
                            //      from it and advances it; peekObjNodeType restores it; readRewind = Block
    int Size;               // 0x10 [O]; [P] GetSize / getBlockSaveSize (cReplayFramePtr_getBlockSaveSize / cReplayFramePtr_GetSize) lw 0x10; [R]
                            //      ctor 0; [U] write adds len; setBlock stores size; writeRewind / purgeBlock 0
};

CHECK_OFFSET(cBxReplayFrame, mVtbl, 0x00);
CHECK_SIZE(cBxReplayFrame, 0x4);

CHECK_OFFSET(tFramePos, Data, 0x0);
CHECK_SIZE(tFramePos, 0x4);

CHECK_OFFSET(cReplayFramePtr, mVtbl, 0x00);
CHECK_OFFSET(cReplayFramePtr, Block, 0x04);
CHECK_OFFSET(cReplayFramePtr, WriteData, 0x08);
CHECK_OFFSET(cReplayFramePtr, ReadData, 0x0C);
CHECK_OFFSET(cReplayFramePtr, Size, 0x10);
CHECK_SIZE(cReplayFramePtr, 0x14);

#endif /* REPLAY_FRAMEPTR_H */
