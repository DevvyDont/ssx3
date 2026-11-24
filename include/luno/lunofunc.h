#ifndef LUNO_LUNOFUNC_H
#define LUNO_LUNOFUNC_H

// luno::cLunoFunc: a reference to a compiled Luno function: the code chunk it lives in and the function's number in
// it. Stored in a luno::cLunoVar's data when the var's Type is eLunoVar_Func (luno/lunovar.h); trivial destructor.
//
// Name: EA's luno::cLunoFunc (the July 2003 preview's linkonce init__Q24luno9cLunoFuncPCci = retail
// luno_cLunoFunc_init, and the assert "chunk->Signature == kLunoChunkSignature" from ../source/luno/lunofunc.h; On
// Tour's DWARF: class cLunoFunc, 0x8, {const tChunkHeader* Chunk; int FuncNum}, methods init(const char* data, int
// funcNum), writeToReplayFrame(cBxReplayFrame*, const char* base) const, readFromReplayFrame(cBxReplayFrame*, const
// char* base)). Flattened as luno_cLunoFunc. No vtable.
// Local definitions it replaces: the SKIP_ASM view struct cLunoFunc {const char* Code; int Index} in
// src/luno/lunovm.cpp (peak3project's guessed names; On Tour's are used here).
//
// FLAT layout. Evidence tags: [O] On Tour's DWARF; [P] the preview; [U] retail use.

#include "layout.h"

// The header of a compiled Luno code chunk (the block luno_cLunoFunc::Chunk points at; [O] On Tour struct tChunkHeader,
// 0x10, the type of cLunoFunc::Chunk). The instruction words follow it; the function table sits at FunctionOffset.
// cLunoVM::execute (src/luno/lunovm.cpp) reads FunctionOffset from it.
struct tChunkHeader {
    int Signature;                  // 0x0 [O]; [P] assert "chunk->Signature == kLunoChunkSignature" (luno_cLunoFunc_init)
    int FunctionOffset;             // 0x4 [O]; [U] execute: the function table is at chunk + FunctionOffset and the
                                    //     code runs from chunk + 0x10 up to it
    int unk_8;                      // 0x8 no retail access seen ([O] LiteralOffset)
    int unk_C;                      // 0xC no retail access seen ([O] ChunkSize)
};

CHECK_OFFSET(tChunkHeader, Signature, 0x0);
CHECK_OFFSET(tChunkHeader, FunctionOffset, 0x4);
CHECK_OFFSET(tChunkHeader, unk_8, 0x8);
CHECK_OFFSET(tChunkHeader, unk_C, 0xC);
CHECK_SIZE(tChunkHeader, 0x10);

struct luno_cLunoFunc {
    const char* Chunk;                  // 0x0 [O] const tChunkHeader* (no tChunkHeader in include/ yet); [U] init stores
                                        //     its data argument; writeToReplayFrame (luno_cLunoFunc_writeToReplayFrame) writes Chunk - base,
                                        //     readFromReplayFrame (luno_cLunoFunc_readFromReplayFrame) stores base + the value read
    int FuncNum;                        // 0x4 [O]; [U] init; written / read as is by the replay functions
};

CHECK_OFFSET(luno_cLunoFunc, Chunk, 0x0);
CHECK_OFFSET(luno_cLunoFunc, FuncNum, 0x4);
CHECK_SIZE(luno_cLunoFunc, 0x8);

#endif /* LUNO_LUNOFUNC_H */
