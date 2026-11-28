#ifndef REPLAY_REPLAYCACHE_H
#define REPLAY_REPLAYCACHE_H

// cReplayCache: one rider's recorded control commands for the replay - a run-length list of 8-byte tControlCmds
// (low 12 bits of the first word = how many ticks the command repeats). While recording, updateControlCmd adds the
// rider's command each tick (addControlCmd merges a repeat into the last entry, addCmdFrame appends); during playback
// it returns them (getControlCmd), moveTo seeks to a tick, truncate cuts the list, load decodes a saved one.
// cReplay owns up to two (cReplay::InputCache[2], replay/replay.h): cReplay_addCache does cMemMan_alloc(0x10,
// "ReplayCache", 0, 0) + cReplayCache::cReplayCache; each cPlayer keeps the one it gets in cPlayer::ReplayCache
// (ai/player.h, 0xDF8). EA's source: d:/ssx/code/source/replay/replaycache.cpp (every reference to that assert path in
// the preview, 0x00239310 .. 0x0023983C, is inside its cReplayCache methods, 0x00239220 .. 0x002399B8). Retail code is
// in the decomp's unit replay/playbackman.cpp (0x0026D0A0 .. 0x0026D5E0); the decomp's unit replay/replaycache.cpp
// holds cReplay methods instead, and replay/playbackman.h is cPlaybackMan's header, so this is a header of its own.
// Size 0x10: [R] the allocation size in cReplay_addCache. No vtable (the destructor is called directly), no bases in
// retail code.
//
// A FLAT layout, like main/game.h. Tags: [R] cReplayCache::cReplayCache (operator new(0x8000, "CmdFrame") -> 0xC) and the
// allocation above; [U] how retail code uses the field (replay/playbackman.cpp, replay/replaycache.cpp's cReplay_writeInput,
// the asm); [O] SSX On Tour's DWARF: class cReplayCache (size 0x10, CmdFrame at 0xC; its first 0xC bytes are not
// named there) and class cReplayCacheState (size 0xC: RecordCursor, PlaybackCursor, PlaybackLeft), whose members fit
// the first 0xC bytes by use. No preview evidence (no accessors; layout_xlate has no pairs).

#include "layout.h"
#include "ai/controlcmd.h"

// tControlCmd (On Tour union tControlCmd, 8 bytes) is in ai/controlcmd.h: one recorded rider command. [R] the cache
// allocates 0x8000 bytes = 0x1000 commands; [O] cReplayCache::getControlCmd(tControlCmd&), addCmdFrame(const
// tControlCmd&), getCmdFrame() returns tControlCmd*.

union tControlCmd;

void operator_delete(int* p);

struct cReplayCache {
    // --- cReplayCacheState (On Tour: size 0xC; [O] names, placed by [U])
    int RecordCursor;               // 0x000 [O]; [U] the number of CmdFrame entries: addCmdFrame appends at it and
                                    //       increments it (cap 0x1000 = 0x8000 / 8), addControlCmd looks at entry
                                    //       RecordCursor - 1, truncate sets it, getControlCmd / moveTo stop at it,
                                    //       cReplay_writeInput saves it in the cache header
    int PlaybackCursor;             // 0x004 [O]; [U] getControlCmd's current entry (advanced when PlaybackLeft runs out);
                                    //       moveTo sets it
    int PlaybackLeft;               // 0x008 [O]; [U] ticks left in the current entry (getControlCmd reloads it from the
                                    //       entry's 12-bit count and decrements it; moveTo sets it)
    // --- cReplayCache
    tControlCmd* CmdFrame;          // 0x00C [O]; [R] ctor: operator new(0x8000, "CmdFrame", 0, 0); [U] purge cMemMan_frees
                                    //       it; load HUFF_decodes into it; updateControlCmd returns 0 while it is 0;
                                    //       cReplay_writeInput saves 0x8000 bytes of it

    // Methods bound to their retail symbols by asm labels.
    void purge() __asm__("cReplayCache_purge");
    void reset() __asm__("cReplayCache_reset__FPv");
    int updateControlCmd(tControlCmd* controlCmd) __asm__("cReplayCache_updateControlCmd");
    void addCmdFrame(tControlCmd* chunkp) __asm__("cReplayCache_addCmdFrame");
    int addControlCmd(tControlCmd* chunk) __asm__("cReplayCache_addControlCmd");
    void truncate(unsigned int pos) __asm__("cReplayCache_truncate");
    bool moveTo(unsigned int pos) __asm__("cReplayCache_moveTo");
    int load(char** pp) __asm__("cReplayCache_load");
    bool getControlCmd(tControlCmd* out) __asm__("cReplayCache_getControlCmd");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cReplayCache();
    ~cReplayCache();
    static void operator delete(void* p) { operator_delete((int*)p); }
};
CHECK_OFFSET(cReplayCache, RecordCursor, 0x0);
CHECK_OFFSET(cReplayCache, PlaybackCursor, 0x4);
CHECK_OFFSET(cReplayCache, PlaybackLeft, 0x8);
CHECK_OFFSET(cReplayCache, CmdFrame, 0xC);
CHECK_SIZE(cReplayCache, 0x10);

#endif /* REPLAY_REPLAYCACHE_H */
