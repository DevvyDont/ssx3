#ifndef REPLAY_FRAME_H
#define REPLAY_FRAME_H

// cReplayFrame: one recorded replay frame - a cReplayFramePtr data block (the saved world/rider state) plus its place
// in a cReplayFrameList (Next / Prev), its type (plain, highlight, chapter, instant replay; tReplayFrameType), the
// riders' rankings and highlight end ticks, and the tick range it covers. cReplay owns 16 of them (cReplay::Frames,
// replay/replay.h, built by cReplay_cReplay with cReplayFrame_cReplayFrame, 0x38 apart) and moves them between its
// FreeFrameList and FrameList; cPlaybackMan embeds one at +0x14 (its saved frame).
// EA's header: ../source/replay/frame.h (the July 2003 preview files inline asserts under it); EA's source
// d:/ssx/code/source/replay/frame.cpp. Retail code: unit replay/playbackman (0x0026D5E8 ctor .. 0x0026D730 setRanking)
// and the destructor in replay/replaycache (0x00272258).
//
// Size 0x38: [R] cReplay_cReplay constructs 16 frames 0x38 apart (replay/replay.h Frames[16][0x38]); the last member
// cReplayFrame_clearFrame / cReplay_allocReplayFrame write is 0x34; [O] On Tour's cReplayFrame is 0x48 (4 players:
// Ranking[4] / HighlightEndTick[4]; retail has 2 players: 2 + 2 words, 0x10 less).
// Base: cReplayFramePtr (0x00-0x14, replay/frameptr.h; its base cBxReplayFrame is a vptr only, at 0). Written out
// inline with that header's names. Vtable cReplayFrame_vtbl (0x00481850, 7 slots: 1..6 cReplayFramePtr's write / read /
// readReplayInstance / readReplayInstance1 / writeReplayInstance / writeReplayInstance1, 7 cReplayFrame__cReplayFrame).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] cReplayFrame_cReplayFrame (base ctor, vtable, clearFrame), cReplayFrame_clearFrame (stores 0x8..0x34),
//       cReplayFrame__cReplayFrame, cReplay_allocReplayFrame (src/replay/replaycache.cpp: type 0, -1 at 0x20 / 0x24 /
//       0x34, FrameStartTick 0);
//   [P] the preview names the class only (_vt$12cReplayFrame, its destructor); no accessors;
//   [U] how retail code uses the field (cReplayFrame_loadFrame, cReplayFrame_setRanking, cReplayFrameList_findFrame,
//       the cReplay save / load / highlight code in replay/replay.cpp, replay/replaycache.cpp, replay/playbackman.cpp);
//   [O] SSX On Tour's DWARF (class cReplayFrame, struct tReplayFrameType): names, types and order.

#include "layout.h"

// A frame's type (On Tour tReplayFrameType, 4 bytes; passed by value as one word). [R] clearFrame and
// cReplay_allocReplayFrame build {0, 0, 0} on the stack and store it as a word (the views sPlaybackKey /
// sFrameDesc26CBB8 / sFrameDesc270130 / sFrameDesc_F4A8 / sFrameDesc26F7F8: short + 2 chars).
struct tReplayFrameType {
    unsigned short Type;            // 0x0 [O]; [U] 0 plain, 1 = the playback manager's saved frame
                                    //     (cPlaybackMan_saveFrame); cReplay_purgeIdleFrames frees Type-0 frames
    unsigned char Highlights;       // 0x2 [O]; [U] cReplay_freeOneReplayFrame / purgeIdleFrames keep frames whose
                                    //     +0x1E (Highlights) or +0x1F (Chapter) byte is set
    unsigned char Chapter;          // 0x3 [O]; [U] ditto
};

CHECK_OFFSET(tReplayFrameType, Type, 0x0);
CHECK_OFFSET(tReplayFrameType, Highlights, 0x2);
CHECK_OFFSET(tReplayFrameType, Chapter, 0x3);
CHECK_SIZE(tReplayFrameType, 0x4);

struct cReplayFrame {
    // --- cReplayFramePtr (replay/frameptr.h); cBxReplayFrame is the vptr
    void** mVtbl;                   // 0x00 [R] cReplayFramePtr ctor's vtable, then cReplayFrame_vtbl (ctor);
                                    //      cReplayFramePtr_vtbl again in the dtor
    char* Block;                    // 0x04 [O] cReplayFramePtr::Block; [R] base ctor 0; [U] cReplayFramePtr_loadBlock
    char* WriteData;                // 0x08 [O]; [R] clearFrame 0
    char* ReadData;                 // 0x0C [O]; [R] clearFrame 0
    int Size;                       // 0x10 [O]; [R] clearFrame 0; [U] loadFrame restores it (header word 1)
    // --- cReplayFrame
    cReplayFrame* Next;             // 0x14 [O]; [U] cReplayFrameList_addFrame / remFrame / findFrame (view sReplayFrame
                                    //      next 0x14), cReplay_writeFrames (sRcFrame1380 next)
    cReplayFrame* Prev;             // 0x18 [O]; [U] cReplayFrameList_addFrame (frame->Prev = Head), remFrame
    tReplayFrameType FrameType;     // 0x1C [O]; [R] clearFrame / allocReplayFrame store {0, 0, 0} as a word;
                                    //      [U] loadFrame restores the word; cReplay_saveFrame1 copies its argument in
    int Ranking[2];                 // 0x20 [O] (On Tour [4]); [R] clearFrame / allocReplayFrame -1; [U] setRanking(i,
                                    //      value) stores word i; loadFrame / the save header carry 0x20..0x2C
    int HighlightEndTick[2];        // 0x28 [O] (On Tour [4]; placed by On Tour's order between Ranking and
                                    //      FrameStartTick); [R] clearFrame -1; [U] loadFrame / save header only
    int FrameStartTick;             // 0x30 [O]; [R] clearFrame / allocReplayFrame 0; [U] cReplay_saveFrame1 stores
                                    //      RecordTick, cReplayFrameList_findFrame compares it, playback seeks to it
    int FrameEndTick;               // 0x34 [O]; [R] clearFrame / allocReplayFrame -1; [U] loadFrame restores it
};

CHECK_OFFSET(cReplayFrame, mVtbl, 0x00);
CHECK_OFFSET(cReplayFrame, Block, 0x04);
CHECK_OFFSET(cReplayFrame, WriteData, 0x08);
CHECK_OFFSET(cReplayFrame, ReadData, 0x0C);
CHECK_OFFSET(cReplayFrame, Size, 0x10);
CHECK_OFFSET(cReplayFrame, Next, 0x14);
CHECK_OFFSET(cReplayFrame, Prev, 0x18);
CHECK_OFFSET(cReplayFrame, FrameType, 0x1C);
CHECK_OFFSET(cReplayFrame, Ranking, 0x20);
CHECK_OFFSET(cReplayFrame, HighlightEndTick, 0x28);
CHECK_OFFSET(cReplayFrame, FrameStartTick, 0x30);
CHECK_OFFSET(cReplayFrame, FrameEndTick, 0x34);
CHECK_SIZE(cReplayFrame, 0x38);

#endif /* REPLAY_FRAME_H */
