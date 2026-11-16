#include "common.h"
#include "inseq/scriptplayerchannel.h"
#include "inseq/scriptheap.h"
#include "replay/frameptr.h"
#include "inseq/inseq.h"
#include "inseq/scriptplayermain.h"
#include "inseq/scriptplayerentity.h"
#include "replay/replay.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("replay/replaycache", cReplay_addCache);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0026D0A0(void* self);
extern const char D_004817B0[];

/**
 * Adds a recorded-input cache to the replay: builds a new cReplayCache in the first free InputCache slot.
 *
 * Checks InputCache[0] and [1] (+0x48C); for the first empty one it allocates 0x10 bytes tagged "ReplayCache"
 * and runs the cache constructor func_0026D0A0 (which allocates its 0x8000-byte "CmdFrame" buffer and clears
 * its counters). Nothing is allocated when both slots are in use.
 *
 * @return the new cache, or 0 when both slots were already taken
 */
extern "C" void* cReplay_addCache(cReplay* self)
{
    void** slot = (void**)((char*)self + 0x48C);
    for (int i = 0; i < 2; i++, slot++) {
        if (*slot == 0) {
            return *slot = func_0026D0A0(cMemMan_alloc(0x10, D_004817B0, 0, 0));
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_removeCache);
#ifdef SKIP_ASM
extern "C" void func_0026D0E8(void* p, int mode);

extern "C" void cReplay_removeCache(cReplay* self, void* pCache)
{
    if (pCache != 0) {
        void** slot = (void**)((char*)self + 0x48C);
        for (int i = 0; i < 2; i++, slot++) {
            if (*slot == pCache) {
                if (*slot != 0) {
                    func_0026D0E8(*slot, 3);
                }
                *slot = 0;
                return;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_allocReplayFrame);
#ifdef SKIP_ASM
extern "C" void cReplay_freeOneReplayFrame(cReplay* self);
extern "C" void* func_0026E670(void* list);

struct sFrameDesc270130 {
    short type;
    char a;
    char b;
};

/**
 * Takes a frame off FreeFrameList, first freeing a recorded one (cReplay_freeOneReplayFrame) when the free list is empty.
 *
 * The frame (func_0026E670, the free list's oldest) gets frame type 0, -1 in the words at +0x20, +0x24 and +0x34, and
 * FrameStartTick (+0x30) 0.
 *
 * @return the frame (On Tour cReplayFrame*)
 */
extern "C" void* cReplay_allocReplayFrame(cReplay* self)
{
    void* list = (char*)self + 0x3BC;
    if (self->FreeFrameList.NumFrames <= 0) {
        cReplay_freeOneReplayFrame(self);
    }
    void* node = func_0026E670(list);
    sFrameDesc270130 desc;
    desc.type = 0;
    desc.a = 0;
    desc.b = 0;
    *(sFrameDesc270130*)((char*)node + 0x1C) = desc;
    *(int*)((char*)node + 0x20) = -1;
    *(int*)((char*)node + 0x24) = -1;
    *(int*)((char*)node + 0x34) = -1;
    *(int*)((char*)node + 0x30) = 0;
    return node;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_freeOneReplayFrame);
#ifdef SKIP_ASM
extern "C" void cReplay_freeReplayFrame(cReplay* self, void* node);

struct sRcNode1A0 {
    char pad_0x00[0x14];
    sRcNode1A0* next;       // 0x14
    char pad_0x18[0x6];
    unsigned char locked;   // 0x1E
    unsigned char used;     // 0x1F
};


extern "C" void cReplay_freeOneReplayFrame(cReplay* self)
{
    cReplay* replay = self;
    int i;
    for (i = 0; i < 2; i++) {
        while (((sRcNode1A0*)replay->NextCandidate) != 0 && ((sRcNode1A0*)replay->NextCandidate)->next != 0 && ((sRcNode1A0*)replay->NextCandidate)->next->next != 0) {
            sRcNode1A0* n = ((sRcNode1A0*)replay->NextCandidate)->next;
            if (n->used == 0) {
                if (n->locked == 0) {
                    (*(sRcNode1A0**)&replay->NextCandidate) = n->next;
                    cReplay_freeReplayFrame(self, n);
                    return;
                }
            }
            (*(sRcNode1A0**)&replay->NextCandidate) = ((sRcNode1A0*)replay->NextCandidate)->next;
        }
        (*(sRcNode1A0**)&replay->NextCandidate) = (*(sRcNode1A0**)&replay->FrameList);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_freeReplayFrame);
#ifdef SKIP_ASM
extern "C" void func_0026E608(void* list, void* node);
extern "C" void cReplayFrame_clearFrame(void* node);
extern "C" void func_0026E5D0(void* list, void* node);

/**
 * Gives a frame back: unlinks it from FrameList (func_0026E608), clears it (cReplayFrame_clearFrame) and adds it to
 * FreeFrameList (func_0026E5D0).
 *
 * @param node  the frame (On Tour cReplayFrame*)
 */
extern "C" void cReplay_freeReplayFrame(cReplay* self, void* node)
{
    func_0026E608((char*)self + 0x3B0, node);
    cReplayFrame_clearFrame(node);
    func_0026E5D0((char*)self + 0x3BC, node);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_isPauseAI);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sS270280 {
    int state;
    char pad4[0x3CC - 4];
    int f3CC;
    char pad3D0[0x484 - 0x3D0];
    int f484;
    char pad488[0x604 - 0x488];
    int f604;
};
/**
 * Says whether the AI should hold still for the current replay state.
 *
 * Never while recording or in states 2 and 10-15. Always in 3, 5, 6, 8 and 9. In 1 and 7 when NextFrame is clear or
 * playback has reached EndRecordTick; in 4 only at EndRecordTick.
 *
 * @return 1 to pause the AI, else 0 (On Tour returns bool)
 */
extern "C" int cReplay_isPauseAI(cReplay* self)
{
    int r = 0;
    switch (self->State) {
    case 0: case 2: case 10: case 11: case 12: case 13: case 14: case 15:
        break;
    case 1:
    case 7:
        if (self->NextFrame == 0)
            r = 1;
        if (self->PlaybackTick >= self->EndRecordTick)
            r = 1;
        break;
    case 4:
        r = self->PlaybackTick >= self->EndRecordTick;
        break;
    case 3: case 5: case 6: case 8: case 9:
        r = 1;
        break;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_rewind);
#ifdef SKIP_ASM
extern "C" int func_0026D4D8(void* stream, unsigned int pos);

struct sReplay002702F8 {
    char pad_0x000[0x3B0];
    void* head;
    char pad_0x3B4[0x484 - 0x3B4];
    int pos;
    void* frame;
    void* streams[2];
    char pad_0x494[0x604 - 0x494];
    int unk_0x604;
};

/**
 * Moves playback to the start of @p frame, or of the oldest recorded frame when it is null.
 *
 * Clears NextFrame, sets PlaybackFrame, takes PlaybackTick from the frame's FrameStartTick and moves each input cache
 * to it (func_0026D4D8, cReplayCache::moveTo).
 *
 * @param frame  the frame to start from, or 0 for the first
 */
extern "C" void cReplay_rewind(cReplay* self, void* frame)
{
    int i;
    self->NextFrame = 0;
    if (frame != 0) {
        self->PlaybackFrame = (cReplayFrame*)frame;
    } else {
        self->PlaybackFrame = (cReplayFrame*)*(void**)&self->FrameList;
    }
    self->PlaybackTick = *(int*)((char*)self->PlaybackFrame + 0x30);
    for (i = 0; i < 2; i++) {
        if (((void**)&self->InputCache[0])[i] != 0) {
            func_0026D4D8(((void**)&self->InputCache[0])[i], self->PlaybackTick);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_lastFrame);
#ifdef SKIP_ASM
extern "C" int func_0026D4D8(void* stream, unsigned int pos);
// PORT: func_0026E448 is declared elsewhere with one parameter, but this caller
// passes (self, pos); bind the 2-arg form to the symbol.
void* func_0026E448_2(void* self, int pos) __asm__("func_0026E448__FPv");


extern "C" void cReplay_lastFrame(cReplay* self)
{
    int i;
    self->NextFrame = 0;
    void* p = func_0026E448_2((self), 100000000);
    self->PlaybackFrame = (cReplayFrame*)p;
    self->PlaybackTick = *(int*)((char*)p + 0x30);
    for (i = 0; i < 2; i++) {
        if (((void**)&self->InputCache[0])[i] != 0) {
            func_0026D4D8(((void**)&self->InputCache[0])[i], self->PlaybackTick);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_stepFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cReplay_restoreReplayFrame(void* self, void* frame);
extern "C" void cChaseCameraController_switchCamera_Chapter(void* self);
extern "C" void cGameViewMan_updateAll(void* list);
void* func_0026E448_2(void* self, int pos) __asm__("func_0026E448__FPv");
void cReplay_setReplayState(void* self, int arg);

struct sRcVEntry03F0 {
    short delta;
    short index;
    void (*fn)(void*);
};


/**
 * Advances playback by one tick, or (@p reset) jumps to the frame holding PlaybackTick and restores it.
 *
 * With @p reset, PlaybackFrame becomes the frame for PlaybackTick (func_0026E448); SaveFrame is skipped (its +0x18
 * link); PlaybackTick moves to that frame's start, the input caches follow and the frame is restored. Otherwise the
 * tick advances until EndRecordTick: there an automatic replay rewinds, steps, switches the first view's chase camera
 * to its chapter camera (slot 4 of its controller) and updates every view; any other replay stops at EndRecordTick in
 * state 3 (kPlaybackInput).
 *
 * @param reset  nonzero to jump to and restore the frame for PlaybackTick (On Tour bool setframe)
 */
extern "C" void cReplay_stepFrame(cReplay* self, int reset)
{
    int i;
    if (reset) {
        self->PlaybackFrame = (cReplayFrame*)func_0026E448_2((self), self->PlaybackTick);
    }
    void* f = self->PlaybackFrame;
    if (f != 0 && f == (void*)self->SaveFrame) {
        self->PlaybackFrame = (cReplayFrame*)(*(void**)((char*)f + 0x18));
    }
    if (reset) {
        self->PlaybackTick = *(int*)((char*)self->PlaybackFrame + 0x30);
        for (i = 0; i < 2; i++) {
            if (((void**)&self->InputCache[0])[i] != 0) {
                func_0026D4D8(((void**)&self->InputCache[0])[i], self->PlaybackTick);
            }
        }
        cReplay_restoreReplayFrame((self), self->PlaybackFrame);
        return;
    }
    if (self->PlaybackTick >= self->EndRecordTick) {
        if (self->AutoReplay != 0) {
            cReplay_rewind(self, 0);
            cReplay_stepFrame(self, 1);
            void* list = SSXApp->Game->ViewMan;
            char* obj = *(char**)(*(char**)((char*)list + 0x4) + 0xA8);
            cChaseCameraController_switchCamera_Chapter(obj);
            sRcVEntry03F0* vt = *(sRcVEntry03F0**)(obj + 0x14);
            vt[4].fn(obj + vt[4].delta);
            cGameViewMan_updateAll(list);
        } else {
            self->PlaybackTick = self->EndRecordTick;
            cReplay_setReplayState((self), 3);
        }
    } else {
        self->PlaybackTick++;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_purgeIdleFrames);
#ifdef SKIP_ASM
struct sReplayNode270538 {
    char pad_0x00[0x14];
    sReplayNode270538* next;
    char pad_0x18[0x4];
    unsigned short type;
    unsigned char a;
    unsigned char b;
};

extern "C" void cReplay_purgeIdleFrames(cReplay* self)
{
    sReplayNode270538* node = *(sReplayNode270538**)&self->FrameList;
    while (node != 0) {
        sReplayNode270538* next = node->next;
        if (node->type != 0) {
        } else if (node->b != 0) {
        } else if (node->a != 0) {
        } else {
            cReplay_freeReplayFrame(self, node);
        }
        node = next;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002705A8__FPv);
#ifdef SKIP_ASM
void func_002705A8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_endMarkInstantReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_startInstantReplay(cReplay* self);

// PORT: the unit declares the object argument as int.
extern "C" void cReplay_endMarkInstantReplay(cReplay* self, int obj, int alt)
{
    char* o = (char*)obj;
    if ((int)self->InstantReplayFrame == 0) {
        return;
    }
    if (*(int*)((char*)SSXApp->Game->AI + 0x7C) != 1) {
        return;
    }
    int now;
    int start;
    if (alt != 0) {
        now = *(int*)(o + 0x9C);
        start = *(int*)(((char*)self) + 0x474);
    } else {
        now = *(int*)(o + 0xA4);
        start = *(int*)(((char*)self) + 0x47C);
    }
    now -= start;
    if (now >= 1000) {
        cReplay_startInstantReplay(self);
    } else {
        self->InstantReplayFrame = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_cancelMarkInstantReplay);
#ifdef SKIP_ASM

/**
 * Drops a marked instant replay (InstantReplayFrame) when the AI manager's +0x7C word is 1.
 */
// FAKEMATCH: the clear is written as three identical arms on the sign of v (`if (v > 0) {*on = 0;} else if (v < 0)
// {*on = 0;} else {*on = 0;}`), which keeps v a global pseudo with priority over self; plain form 97.92%.
// Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void cReplay_cancelMarkInstantReplay(cReplay* self)
{
    int* on = (int*)((char*)self + 0x3D4);
    int v = *on;
    if (v != 0) {
        int x = *(int*)((char*)SSXApp->Game->AI + 0x7C);
        if (x == 1) {
            if (v > 0) {
                *on = 0;
            } else if (v < 0) {
                *on = 0;
            } else {
                *on = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_startInstantReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_startInstantReplay(cReplay* self)
{
    if (self->InstantReplay == 0) {
        self->StartInstantReplay = 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_stopInstantReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_stopReplay(void* self);
extern "C" void cRiderMetrix_resetHUDfx(void* p);

extern "C" void cReplay_stopInstantReplay(cReplay* self)
{
    if (self->InstantReplay != 0) {
        cReplay_stopReplay(self);
        cRiderMetrix_resetHUDfx(*(void**)(*(char**)(*(char**)((char*)SSXApp->Game->AI + 0x40) + 0x18) + 0x790));
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_startAutoReplay);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cReplay_setReplayState_cpp(void* self, int arg) __asm__("cReplay_setReplayState__FPvi");

/**
 * Switches to state 9 (kStartAutoReplay) unless an automatic replay is already on or the state is already 9.
 */
extern "C" void cReplay_startAutoReplay(cReplay* self)
{
    if (self->AutoReplay == 0 && *(int*)self != 9) {
        cReplay_setReplayState_cpp(self, 9);
    }
}
#endif

extern "C" void cReplay_stopReplay(void* self);
void cReplay_setReplayState(void* self, int arg);

//100%
INCLUDE_ASM("replay/replaycache", cReplay_stopAutoReplay__FPv);
#ifdef SKIP_ASM
/**
 * Stops an auto replay and puts the replay into the kReplayEnd state (13).
 *
 * When AutoReplay (+0x61C) is set it first calls cReplay_stopReplay (probably stopReplay: it clears AutoReplay,
 * InstantReplay and StartInstantReplay and, unless Loaded, restores the saved frame). The state is then set
 * to 13 either way, overriding the one cReplay_stopReplay chose.
 */
void cReplay_stopAutoReplay(void* self)
{
    if (*(int*)((char*)self + 0x61C) != 0) {
        cReplay_stopReplay(self);
    }
    cReplay_setReplayState(self, 0xD);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00270730);
#ifdef SKIP_ASM

class cRcIdObj_0730 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int v07();
};

struct sRcRider_0730 {
    char pad_0x0[0x18];
    char* obj;                      // 0x18
};

struct sRcWorld_0730 {
    char pad_0x0[0x40];
    sRcRider_0730* riders[15];      // 0x40
    int numRiders;                  // 0x7C
};

static inline sRcWorld_0730* world_0730()
{
    return (sRcWorld_0730*)SSXApp->Game->AI;
}

extern "C" int func_00270730(int id)
{
    int i;
    for (i = 0; i < world_0730()->numRiders; i++) {
        if (((cRcIdObj_0730*)(world_0730()->riders[i]->obj + 0x6C0))->v07() == id) {
            return i;
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventJump);
#ifdef SKIP_ASM
extern "C" int func_00270730(int id);
extern "C" void func_00270B88(cReplay* self, int slot, int id);
// PORT: the unit defines func_002705A8(void*) (an empty stub), but this caller
// passes (self, id); bind the 2-arg call by asm label.
void func_002705A8_2(void* self, int id) __asm__("func_002705A8__FPv");

extern "C" void cReplay_eventJump(cReplay* self, int key, int id)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->ReplayError == 0 && self->EndReplay == 0 &&
        self->State == 0) {
        int slot = func_00270730(key);
        if (slot >= 0) {
            func_00270B88(self, slot, id);
            func_002705A8_2(self, id);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventLand);
#ifdef SKIP_ASM
extern "C" int func_00270730(int id);
extern "C" void func_00270CE8(cReplay* self, int slot, int id, int flag);
extern "C" void cReplay_endMarkInstantReplay(cReplay* self, int id, int flag);

extern "C" void cReplay_eventLand(cReplay* self, int key, int id)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->State == 0) {
        int slot = func_00270730(key);
        if (slot >= 0) {
            func_00270CE8(self, slot, id, 1);
            cReplay_endMarkInstantReplay(self, id, 1);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventWipeout);
#ifdef SKIP_ASM
extern "C" int func_00270730(int id);
extern "C" void func_00270CE8(cReplay* self, int slot, int id, int flag);
extern "C" void cReplay_endMarkInstantReplay(cReplay* self, int id, int flag);

extern "C" void cReplay_eventWipeout(cReplay* self, int key, int id)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->State == 0) {
        int slot = func_00270730(key);
        if (slot >= 0) {
            func_00270CE8(self, slot, id, 0);
            cReplay_endMarkInstantReplay(self, id, 0);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventReset);
#ifdef SKIP_ASM
extern "C" int func_00270730(int id);
struct sRcSlots;
extern "C" void cReplay_cancelMarkHighlight(cReplay* self, int i);
extern "C" void cReplay_cancelMarkInstantReplay(cReplay* self);

/**
 * Records a rider's reset during live recording: cancels that rider's marked highlight and any marked instant replay.
 *
 * Only while recording (state 0) outside automatic and instant replays, and when func_00270730 (findPlayerId) finds
 * the rider.
 *
 * @param key  the rider (On Tour int rider)
 */
extern "C" void cReplay_eventReset(cReplay* self, int key)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->State == 0) {
        int slot = func_00270730(key);
        if (slot >= 0) {
            cReplay_cancelMarkHighlight(self, slot);
            cReplay_cancelMarkInstantReplay(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_eventWScript);
#ifdef SKIP_ASM
extern "C" int func_00270730(int id);
extern "C" void func_00270B88(cReplay* self, int slot, int id);
// PORT: the unit defines func_002705A8(void*) (an empty stub), but this caller
// passes (self, id); bind the 2-arg call by asm label.
void func_002705A8_2(void* self, int id) __asm__("func_002705A8__FPv");

extern "C" void cReplay_eventWScript(cReplay* self, int key, int mode)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->ReplayError == 0 && self->EndReplay == 0 &&
        self->State == 0) {
        int slot = func_00270730(key);
        if (slot >= 0) {
            char* world = (char*)SSXApp->Game->AI;
            char* rider = *(char**)(world + (key << 2) + 0x28);
            switch (mode) {
            case 0:
                func_002705A8_2(self, *(int*)(rider + 0x790) + 0xFC);
                break;
            case 1:
                func_00270B88(self, slot, *(int*)(rider + 0x790) + 0xFC);
                break;
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventChapter);
#ifdef SKIP_ASM
static inline int cReplay_eventChapter_isClear(int v, int bit)
{
    return !((v >> bit) & 1);
}

/**
 * Marks a chapter (a race checkpoint) for a rider during live recording.
 *
 * Only while recording (state 0) with no automatic or instant replay, ReplayError or EndReplay. When the rider is
 * found (func_00270730) and @p bit is below MaxChapters and not yet set in Chapters[player], sets the player's bit in
 * the newest frame's byte +0x1F and the chapter bit in Chapters[player].
 *
 * @param key  the rider (On Tour int rider)
 * @param bit  the chapter (On Tour int pos)
 */
extern "C" void cReplay_eventChapter(cReplay* self, int key, int bit)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->ReplayError == 0 && self->EndReplay == 0 &&
        self->State == 0) {
        int slot = func_00270730(key);
        if (slot >= 0 && bit >= 0 && bit < self->MaxChapters) {
            int off = slot << 2;
            char* base = (char*)self + 0x5FC;
            int* p = (int*)(base + off);
            if (cReplay_eventChapter_isClear(*p, bit)) {
                unsigned char* q = (unsigned char*)self->FrameList.Head;
                q[0x1F] |= 1 << slot;
                *p |= 1 << bit;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00270B88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0026E6E0_3(void* slot, void* rider, int src) __asm__("func_0026E6E0");

struct sRcSlotB88 {
    char pad[0xB0];
    int busy;
    bool isFree() { return busy == 0; }
};


extern "C" void func_00270B88(cReplay* replay, int playerId, int id)
{
    cReplay* self = replay;
    if (self->State == 0 && ((sRcSlotB88*)&self->Highlight[0])[playerId].isFree()) {
        func_0026E6E0_3(&((sRcSlotB88*)&self->Highlight[0])[playerId], ((void*)self->FrameList.Head), id);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_countHighlights);
#ifdef SKIP_ASM
struct sReplayCacheNode {
    char pad[0x14];
    sReplayCacheNode* next;
    char pad2[0x1E - 0x18];
    unsigned char mask;
    char pad3;
    int values[1];
};

extern "C" int cReplay_countHighlights(cReplay* self, int i, int positive)
{
    int count = 0;
    sReplayCacheNode* n = *(sReplayCacheNode**)&self->FrameList;
    while (n != 0) {
        if ((n->mask >> i) & 1) {
            if (positive) {
                if (n->values[i] > 0) count++;
            } else if (n->values[i] < 0) {
                count++;
            }
        }
        n = n->next;
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_maxHighlights);
#ifdef SKIP_ASM
extern "C" int cReplay_maxHighlights(cReplay* self, int land)
{
    if (land != 0) {
        return self->MaxPlayerHighlights;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_compareHighlights);
#ifdef SKIP_ASM
struct sReplayCacheNode2 {
    char pad[0x14];
    sReplayCacheNode2* next;
    char pad2[0x1E - 0x18];
    unsigned char mask;
    unsigned char busy;
    int values[1];
};

extern "C" sReplayCacheNode2* cReplay_compareHighlights(cReplay* self, int i, sReplayCacheNode2* best, int positive)
{
    sReplayCacheNode2* result = best;
    sReplayCacheNode2* n = *(sReplayCacheNode2**)&self->FrameList;
    while (n != 0) {
        if (((n->mask >> i) & 1) && n->busy == 0) {
            if (positive) {
                if (n->values[i] > 0 && !(best->values[i] < n->values[i])) {
                    best = result = n;
                }
            } else {
                if (n->values[i] < 0 && !(n->values[i] < best->values[i])) {
                    best = result = n;
                }
            }
        }
        n = n->next;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00270CE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_0026E838_s(void* self, int id, int mode) __asm__("func_0026E838");
extern "C" void func_0026E800(void* p);
void func_0026E7F8(void* self);

struct sRcSlotCE8 {
    char pad[0xB0];
    sReplayCacheNode2* node;
    bool isActive() { return node != 0; }
    sReplayCacheNode2* getNode() { return node; }
};


extern "C" void func_00270CE8(cReplay* replay, int playerId, int id, int positive)
{
    cReplay* self = replay;
    if (((sRcSlotCE8*)&self->Highlight[0])[playerId].isActive()) {
        if (func_0026E838_s(&((sRcSlotCE8*)&self->Highlight[0])[playerId], id, positive)) {
            if (!(cReplay_countHighlights((cReplay*)self, playerId, positive) < cReplay_maxHighlights((cReplay*)self, positive))) {
                sReplayCacheNode2* n = cReplay_compareHighlights((cReplay*)self, playerId, ((sRcSlotCE8*)&self->Highlight[0])[playerId].getNode(), positive);
                n->mask &= ~(1 << playerId);
            }
        } else {
            func_0026E800(&((sRcSlotCE8*)&self->Highlight[0])[playerId]);
        }
        func_0026E7F8(&((sRcSlotCE8*)&self->Highlight[0])[playerId]);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_cancelMarkHighlight);
#ifdef SKIP_ASM
extern "C" void func_0026E800(void* p);


struct sRcSlots {
    int state;
    char pad[0x494 - 4];
    cReplayHighlight slots[1];
};

/**
 * Cancels a player's marked highlight (func_0026E800 on Highlight[i]), only while recording (state 0).
 *
 * @param i  the player (On Tour int playerId)
 */
extern "C" void cReplay_cancelMarkHighlight(cReplay* self, int i)
{
    if (self->State == 0) {
        func_0026E800(&((&self->Highlight[0]))[i]);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_cancelMarkHighlights);
#ifdef SKIP_ASM
extern "C" void cReplay_cancelMarkHighlights(cReplay* self)
{
    if (self->State == 0) {
        for (int i = 0; i < 2; i++) {
            func_0026E800(&((&self->Highlight[0]))[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_initSave);
#ifdef SKIP_ASM
extern "C" void cReplay_allocSaveSpace(cReplay* self);

extern "C" void cReplay_initSave(cReplay* self)
{
    cReplay_allocSaveSpace(self);
    self->WriteState = 0;
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteSize = 0;
    self->WriteData = 0;
    self->WriteIndex = 0;
    self->WriteIndexMax = 0;
    self->WriteFrame = 0;
    self->WriteBlockData = 0;
    self->WriteBlockDataSize = 0;
    self->WriteCmdFrame = 0;
    self->WriteCacheSave = 0;
    self->WriteSaveSize = 0;
    self->WriteRemaining = 0;
    self->WriteCheckSum.Size = 0;
    self->WriteCheckSum.CheckSum = (int)0xDEADBEEF;
}
#endif

extern "C" void* cReplay_purgeSaveSpace(cReplay* self);

//100%
INCLUDE_ASM("replay/replaycache", func_00270ED8__FPv);
#ifdef SKIP_ASM
void* func_00270ED8(void* self)
{
    return cReplay_purgeSaveSpace((cReplay*)self);
}
#endif

extern "C" void* cReplay_reset(void* self);

//100%
INCLUDE_ASM("replay/replaycache", func_00270EF8__FPv);
#ifdef SKIP_ASM
void* func_00270EF8(void* self)
{
    return cReplay_reset(self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00270F18__FPv);
#ifdef SKIP_ASM
void func_00270F18(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_allocSaveSpace);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char D_00481800[];
extern const char D_00481810[];

extern "C" void cReplay_allocSaveSpace(cReplay* self)
{
    self->WriteBlock = (tReplayMemCardBlock*)cMemMan_alloc(0x4000, D_00481800, 0x20000000, 0);
    self->WriteTmp = (char*)operator_new_tag(0xC800, D_00481810, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_purgeSaveSpace);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* p);

// The unit declares this as returning void* (line ~198, used by the tail-call
// wrapper func_00270ED8); nothing meaningful is returned.
extern "C" void* cReplay_purgeSaveSpace(cReplay* self)
{
    operator_delete((int*)self->WriteBlock);
    void* p = self->WriteTmp;
    if (p != 0) {
        cMemMan_free(p);
    }
    self->WriteBlock = 0;
    self->WriteTmp = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeBuffered);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int size);

struct sRcStream0FC0 {
    char pad_0x000[0x644];
    char* src;      // 0x644
    int left;       // 0x648
    int used;       // 0x64C
    char* dst;      // 0x650
};

extern "C" void cReplay_writeBuffered(cReplay* self)
{
    if (self->WriteSrc != 0) {
        if (self->WriteSrcSize > 0) {
            int space = 0x4000 - self->WriteSize;
            int n = space < self->WriteSrcSize ? space : self->WriteSrcSize;
            if (n > 0) {
                MEM_copy(self->WriteData, self->WriteSrc, n);
                self->WriteData += n;
                self->WriteSrc += n;
                self->WriteSrcSize -= n;
                self->WriteSize += n;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeSetup);
#ifdef SKIP_ASM
extern "C" void cReplay_writeSetup(cReplay* self, signed char* data, int size)
{
    self->WriteCheckSum.Size += size;
    for (int i = 0; i < size; i++) {
        self->WriteCheckSum.CheckSum += data[i];
    }
    self->WriteSrc = (char*)data;
    self->WriteSrcSize = size;
    self->WriteSaveSize += size;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_getSaveData);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cReplay_writeHeaderStart(cReplay* self);
extern "C" void cReplay_writeHeader(cReplay* self);
extern "C" void cReplay_writeFramesStart(cReplay* self);
struct sReplayCache1380;
extern "C" void func_00271380(cReplay* self);
extern "C" void cReplay_writeInputStart(cReplay* self);
struct sReplayCache1510;
extern "C" void func_00271510(sReplayCache1510* self);
extern "C" void cReplay_writePadStart(cReplay* self);
extern "C" void cReplay_writePad(cReplay* self);
void* cReplay_writeCheckSumStart(void* self);
extern "C" void cReplay_writeCheckSum(cReplay* self);


// Runs the replay-cache writer state machine until the 16 KB block is full or the stream is done (state 11);
// returns the block and its size, or 0 if there was nothing to write.
extern "C" int cReplay_getSaveData(cReplay* self, char** block, int* size)
{
    if (self->WriteState == 11) {
        *size = 0;
        return 0;
    }
    self->WriteSize = 0;
    self->WriteData = (char*)self->WriteBlock;
    while (self->WriteSize < 0x4000 && self->WriteState != 11) {
        cReplay_writeBuffered(self);
        if (self->WriteSize >= 0x4000) {
            goto done;
        }
        switch (self->WriteState) {
        case 0: cReplay_writeHeaderStart(self); break;
        case 1: cReplay_writeHeader(self); break;
        case 2: cReplay_writeFramesStart(self); break;
        case 3: func_00271380(self); break;
        case 4: cReplay_writeInputStart(self); break;
        case 5: func_00271510((sReplayCache1510*)self); break;
        case 6: cReplay_writePadStart(self); break;
        case 7: cReplay_writePad(self); break;
        case 8: cReplay_writeCheckSumStart((self)); break;
        case 9: cReplay_writeCheckSum(self); break;
        case 10: self->WriteState++; break;
        case 11: break;
        }
    }
done:
    *size = self->WriteSize;
    *block = (char*)self->WriteBlock;
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeHeaderStart);
#ifdef SKIP_ASM
struct sReplayCache;
extern "C" int cReplay_writeFrameCount(cReplay* self);
extern "C" void cAI_getPodiumActorList(void* self, int* dst);

struct sReplayHdr_1228 {
    int magic;   // 0x0
    int count;   // 0x4
    int a;       // 0x8
    int b;       // 0xC
    int c;       // 0x10
    int d;       // 0x14
    int mode;    // 0x18
    int pad;     // 0x1C
    int data[2]; // 0x20
};

extern "C" void cReplay_writeHeaderStart(cReplay* self)
{
    sReplayHdr_1228* hdr = (sReplayHdr_1228*)self->WriteTmp;
    hdr->count = cReplay_writeFrameCount(self);
    char* g = (char*)SSXApp;
    hdr->a = self->SaveFrameTicks;
    hdr->b = self->ReplayError;
    hdr->c = self->RecordTick;
    hdr->d = self->EndRecordTick;
    int mode = *(int*)(*(char**)(*(char**)(g + 0x84) + 0xC) + 0x7C);
    hdr->magic = 0xABCD0002;
    hdr->mode = mode;
    cAI_getPodiumActorList(*(void**)(*(char**)(g + 0x84) + 0xC), hdr->data);
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteState = self->WriteState + 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writeHeader);
#ifdef SKIP_ASM
extern "C" void cReplay_writeHeader(cReplay* self)
{
    cReplay_writeSetup(self, (signed char*)self->WriteTmp, 0x28);
    self->WriteState += 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeFrameCount);
#ifdef SKIP_ASM
struct sCacheNode {
    char pad[0x14];
    sCacheNode* next;
};


extern "C" int cReplay_writeFrameCount(cReplay* self)
{
    int count = 0;
    (*(sCacheNode**)&self->WriteFrame) = (*(sCacheNode**)&self->FrameList);
    while (((sCacheNode*)self->WriteFrame)) {
        if (((sCacheNode*)self->WriteFrame) == ((sCacheNode*)self->SaveFrame)) {
            (*(sCacheNode**)&self->WriteFrame) = ((sCacheNode*)self->WriteFrame)->next;
        } else {
            count++;
        }
        if (((sCacheNode*)self->WriteFrame)) {
            (*(sCacheNode**)&self->WriteFrame) = ((sCacheNode*)self->WriteFrame)->next;
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeFramesStart);
#ifdef SKIP_ASM
extern "C" void cReplay_writeFramesStart(cReplay* self)
{
    void* p = *(void**)&self->FrameList;
    self->WriteFrame = (cReplayFrame*)p;
    if (p == (void*)self->SaveFrame) {
        self->WriteFrame = (cReplayFrame*)(*(void**)((char*)p + 0x14));
    }
    self->WriteBlockData = 0;
    self->WriteState += 1;
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteFrameCount = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00271380);
#ifdef SKIP_ASM
extern "C" int HUFF_encode(void* src, int size, void* dst, int* state);
extern "C" void cReplay_writeSetup(cReplay* self, signed char* data, int size);
// PORT: cReplayFramePtr_getFrameBlock is mangled with no parameters (__Fv) but is
// called with (frame, n); bind the 2-arg form to the symbol.
void cReplayFramePtr_getFrameBlock_2(void* frame, int n) __asm__("cReplayFramePtr_getFrameBlock__Fv");

struct sRcFrame1380 {
    char pad_0x0[0x4];
    void* data;             // 0x4
    char pad_0x8[0x8];
    int size;               // 0x10
    sRcFrame1380* next;     // 0x14
    char pad_0x18[0x8];
    int f20;                // 0x20
    int f24;                // 0x24
    int f28;                // 0x28
    int f2C;                // 0x2C
    int f30;                // 0x30
    int f34;                // 0x34
};


extern "C" void func_00271380(cReplay* self)
{
    if (((void*)self->WriteBlockData) != 0) {
        int state = 0;
        int n = HUFF_encode(((void*)self->WriteBlockData), self->WriteBlockDataSize, ((int*)self->WriteTmp) + 1, &state);
        *((int*)self->WriteTmp) = n;
        cReplay_writeSetup((cReplay*)self, (signed char*)((int*)self->WriteTmp), n + 4);
        (*(void**)&self->WriteBlockData) = 0;
        return;
    }
    if (((sRcFrame1380*)self->WriteFrame) != 0) {
        self->WriteFrameCount++;
        cReplayFramePtr_getFrameBlock_2(((sRcFrame1380*)self->WriteFrame), 1);
        int* hdr = ((int*)self->WriteTmp);
        hdr[0] = 0x11111113;
        hdr[2] = ((sRcFrame1380*)self->WriteFrame)->f30;
        hdr[3] = ((sRcFrame1380*)self->WriteFrame)->f34;
        hdr[1] = ((sRcFrame1380*)self->WriteFrame)->size;
        hdr[9] = 0x11111111;
        hdr[5] = ((sRcFrame1380*)self->WriteFrame)->f20;
        hdr[6] = ((sRcFrame1380*)self->WriteFrame)->f24;
        hdr[7] = ((sRcFrame1380*)self->WriteFrame)->f28;
        hdr[8] = ((sRcFrame1380*)self->WriteFrame)->f2C;
        hdr[7] = ((sRcFrame1380*)self->WriteFrame)->f28;
        hdr[8] = ((sRcFrame1380*)self->WriteFrame)->f2C;
        cReplay_writeSetup((cReplay*)self, (signed char*)hdr, 0x28);
        sRcFrame1380* f = ((sRcFrame1380*)self->WriteFrame);
        (*(void**)&self->WriteBlockData) = f->data;
        self->WriteBlockDataSize = f->size;
        sRcFrame1380* next = f->next;
        (*(sRcFrame1380**)&self->WriteFrame) = next;
        if (next != 0 && next == ((sRcFrame1380*)self->SaveFrame)) {
            (*(sRcFrame1380**)&self->WriteFrame) = next->next;
        }
        return;
    }
    (*(int*)&self->WriteSrc) = 0;
    self->WriteSrcSize = 0;
    self->WriteState++;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeInputStart);
#ifdef SKIP_ASM
extern "C" void cReplay_writeInputStart(cReplay* self)
{
    self->WriteIndexMax = 2;
    self->WriteState += 1;
    self->WriteCacheSave = 0;
    self->WriteCmdFrame = 0;
    self->WriteCmdFrameSize = 0;
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteIndex = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00271510);
#ifdef SKIP_ASM
extern "C" int HUFF_encode(void* src, int size, void* dst, int* state);

struct sRcEntry1510 {
    int field_0x0;          // 0x0
    char pad_0x4[0x8];
    void* data;             // 0xC
};

struct sReplayCache1510 {
    char pad_0x0[0x48C];
    sRcEntry1510* entries[(0x638 - 0x48C) / 4];   // 0x48C
    int field_0x638;        // 0x638
    char pad_0x63C[0x4];
    int* out;               // 0x640
    int field_0x644;        // 0x644
    int field_0x648;        // 0x648
    char pad_0x64C[0x8];
    int cur;                // 0x654
    int count;              // 0x658
    char pad_0x65C[0x10];
    void* pending;          // 0x66C
    int pendingSize;        // 0x670
    int* header;            // 0x674
};

extern "C" void func_00271510(sReplayCache1510* self)
{
    if (self->header != 0) {
        cReplay_writeSetup((cReplay*)self, (signed char*)self->header, 8);
        self->header = 0;
        return;
    }
    if (self->pending != 0) {
        int state = 0;
        int n = HUFF_encode(self->pending, self->pendingSize, self->out + 1, &state);
        *self->out = n;
        cReplay_writeSetup((cReplay*)self, (signed char*)self->out, n + 4);
        self->pending = 0;
        return;
    }
    int cur = self->cur;
    if (cur < self->count) {
        self->pending = 0;
        self->header = 0;
        self->pendingSize = 0;
        sRcEntry1510* e = self->entries[cur];
        if (e != 0) {
            self->pending = e->data;
            self->header = self->out;
            self->pendingSize = 0x8000;
            *self->out = 0x11111112;
            self->header[1] = e->field_0x0;
            self->cur++;
        } else {
            self->cur = cur + 1;
            self->field_0x648 = 0;
            self->field_0x644 = 0;
        }
    } else {
        self->field_0x638++;
        self->field_0x648 = 0;
        self->field_0x644 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writePadStart);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);
extern "C" void* cBE_getBE();
extern "C" int func_0014E048(void* be);

extern "C" void cReplay_writePadStart(cReplay* self)
{
    MEM_fill(self->WriteTmp, 0, 0xC800);
    int used = func_0014E048(cBE_getBE()) + self->WriteSaveSize;
    self->WriteRemaining = 0x7FFF8 - used;
    self->WriteState += 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writePad);
#ifdef SKIP_ASM
extern "C" void cReplay_writePad(cReplay* self)
{
    int left = self->WriteRemaining;
    if (left > 0) {
        int n = (left < 0x4001) ? left : 0x4000;
        cReplay_writeSetup(self, (signed char*)self->WriteTmp, n);
        self->WriteRemaining -= n;
    } else {
        self->WriteState += 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeCheckSumStart__FPv);
#ifdef SKIP_ASM
void* cReplay_writeCheckSumStart(void* self)
{
    int t0 = 0;
    void* t1 = (char*)*(void**)((char*)self + 0x638) + 0x1;
    *(int*)((char*)self + 0x644) = t0;
    *(int*)((char*)self + 0x648) = t0;
    *(int*)((char*)self + 0x638) = (int)t1;
    return t1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writeCheckSum);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void cReplay_writeSetup(cReplay* self, signed char* data, int size);

extern "C" void cReplay_writeCheckSum(cReplay* self)
{
    MEM_copy(self->WriteTmp, (char*)self + 0x680, 8);
    cReplay_writeSetup(self, (signed char*)self->WriteTmp, 8);
    self->WriteState += 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_load);
#ifdef SKIP_ASM
extern "C" int func_0026D558(void* self, char** pp);
extern "C" int func_0026D678(void* self, char** pp);
void func_0026E5C0(void* self);
extern "C" void func_0026E5D0(void* list, void* node);

struct sRcSave1758 {
    int field_0x0;
    int frameCount;         // 0x4
    int f8;                 // 0x8
    int fC;                 // 0xC
    int f10;                // 0x10
    int f14;                // 0x14
    int streamCount;        // 0x18
};

struct sReplayCache1758 {
    int state;                  // 0x0
    char pad_0x4[0x14];
    int field_0x18;             // 0x18
    char pad_0x1C[0x3B0 - 0x1C];
    char frames[0xC];           // 0x3B0
    char pad_0x3BC[0x3C8 - 0x3BC];
    int field_0x3C8;            // 0x3C8
    int field_0x3CC;            // 0x3CC
    void* tail;                 // 0x3D0
    char pad_0x3D4[0x484 - 0x3D4];
    int pos;                    // 0x484
    char pad_0x488[0x4];
    void* streams[(0x608 - 0x48C) / 4]; // 0x48C
    int field_0x608;            // 0x608
    char pad_0x60C[0x4];
    int field_0x610;            // 0x610
    char pad_0x614[0x4];
    int field_0x618;            // 0x618
};

/**
 * Loads a saved replay from a memory-card buffer and starts playing it.
 *
 * Fails unless its rider count matches the race's AI manager. Sets Loaded, SaveFrameTicks, ReplayError (+0xC != 0),
 * RecordTick and EndRecordTick from the header, clears SaveFrame and PlaybackTick, empties FrameList, then loads each
 * frame (cReplayFrame::loadFrame, from offset 0x28) into a fresh frame added to FrameList and each input cache
 * (cReplayCache::load). Ends in state 8 (kPlaybackLoad) with Cmd 1.
 *
 * @param buf  the saved replay (On Tour char* data)
 * @return 1 when loaded, 0 when the rider count differs (On Tour returns bool)
 */
extern "C" int cReplay_load(cReplay* self, char* buf)
{
    sRcSave1758* data = (sRcSave1758*)buf;
    char** pp = &buf;
    if (data->streamCount != ((sRcWorld_0730*)((SSXApp)->Game)->AI)->numRiders) {
        return 0;
    }
    {
        self->Loaded = 1;
        self->SaveFrameTicks = data->f8;
        self->ReplayError = data->fC != 0;
        self->RecordTick = data->f10;
        self->EndRecordTick = data->f14;
        self->SaveFrame = 0;
        self->PlaybackTick = 0;
        func_0026E5C0(((char*)&self->FrameList));
        *pp = (char*)data + 0x28;
        for (int i = 0; i < data->frameCount; i++) {
            void* node = cReplay_allocReplayFrame(self);
            func_0026D678(node, pp);
            func_0026E5D0(((char*)&self->FrameList), node);
        }
        for (int i = 0; i < data->streamCount; i++) {
            if (((void**)&self->InputCache[0])[i] != 0) {
                func_0026D558(((void**)&self->InputCache[0])[i], pp);
            }
        }
        self->State = 8;
        self->Cmd = 1;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002718A8);
#ifdef SKIP_ASM
extern "C" void* func_002718A8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002718E8);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_18E8 {
    float x, y, z, w;
    sV4_18E8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern char D_004D33F0[], D_004D3400[], D_004D3410[], D_004D3420[], D_004D3430[], D_004D3440[], D_004D3450[], D_004D3460[], D_004D3470[], D_004D3480[], D_004D3490[], D_004D34A0[], D_004D34B0[], D_004D34C0[], D_004D34D0[], D_004D34E0[], D_004D34F0[], D_004D3500[], D_004D3510[], D_004D3520[], D_004D3530[], D_004D3540[], D_004D3550[], D_004D3560[], D_004D3570[], D_004D3580[], D_004D3590[], D_004D35A0[], D_004D35B0[], D_004D35C0[], D_004D35D0[], D_004D35E0[], D_004D35F0[], D_004D3600[], D_004D3610[], D_004D3620[], D_004D3630[], D_004D3640[], D_004D3650[], D_004D3660[], D_004D3670[], D_004D3680[], D_004D3690[], D_004D36A0[], D_004D36B0[], D_004D36C0[], D_004D36D0[], D_004D36E0[], D_004D36F0[], D_004D3700[], D_004D3710[], D_004D3720[], D_004D3730[], D_004D3740[], D_004D3750[], D_004D3760[];
struct sSlot_18E8 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_18E8(int n) { a = n; b = n; }
};
extern int D_004A5160;
extern int D_004A5164;
extern sSlot_18E8 D_004A5168, D_004A5170, D_004A5178, D_004A5180, D_004A5188, D_004A5190, D_004A5198, D_004A51A0, D_004A51A8, D_004A51B0, D_004A51B8, D_004A51C0, D_004A51C8, D_004A51D0;

// PORT: the unit declares func_002718E8 as void*(int, int); it returns nothing.
extern "C" void func_002718E8_impl(int initialize, int priority) __asm__("func_002718E8");
extern "C" void func_002718E8_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (D_004D33F0) sV4_18E8(1.0f, 1.0f, 1.0f, 0.5f);
        new (D_004D3400) sV4_18E8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (D_004D3410) sV4_18E8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (D_004D3420) sV4_18E8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (D_004D3430) sV4_18E8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (D_004D3440) sV4_18E8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (D_004D3450) sV4_18E8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (D_004D3460) sV4_18E8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (D_004D3470) sV4_18E8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (D_004D3480) sV4_18E8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (D_004D3490) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D34A0) sV4_18E8(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D34B0) sV4_18E8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (D_004D34C0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D34D0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D34E0) sV4_18E8(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D34F0) sV4_18E8(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D3500) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3510) sV4_18E8(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D3520) sV4_18E8(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D3530) sV4_18E8(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D3540) sV4_18E8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (D_004D3550) sV4_18E8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (D_004D3560) sV4_18E8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (D_004D3570) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3580) sV4_18E8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (D_004D3590) sV4_18E8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (D_004D35A0) sV4_18E8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (D_004D35B0) sV4_18E8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (D_004D35C0) sV4_18E8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (D_004D35D0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D35E0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D35F0) sV4_18E8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (D_004D3600) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3610) sV4_18E8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (D_004D3620) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3630) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3640) sV4_18E8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (D_004D3650) sV4_18E8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (D_004D3660) sV4_18E8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (D_004D3670) sV4_18E8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (D_004D3680) sV4_18E8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (D_004D3690) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D36A0) sV4_18E8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (D_004D36B0) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D36C0) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D36D0) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D36E0) sV4_18E8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (D_004D36F0) sV4_18E8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (D_004D3700) sV4_18E8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (D_004D3710) sV4_18E8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (D_004D3720) sV4_18E8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (D_004D3730) sV4_18E8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (D_004D3740) sV4_18E8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (D_004D3750) sV4_18E8(1.0f, 0.0f, 1.0f, 0.0f);
        new (D_004D3760) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        D_004A5164 = 0;
        D_004A5160 = 0;
        new (&D_004A5168) sSlot_18E8(0);
        new (&D_004A5170) sSlot_18E8(1);
        new (&D_004A5178) sSlot_18E8(2);
        new (&D_004A5180) sSlot_18E8(3);
        new (&D_004A5188) sSlot_18E8(4);
        new (&D_004A5190) sSlot_18E8(5);
        new (&D_004A5198) sSlot_18E8(6);
        new (&D_004A51A0) sSlot_18E8(7);
        new (&D_004A51A8) sSlot_18E8(8);
        new (&D_004A51B0) sSlot_18E8(9);
        new (&D_004A51B8) sSlot_18E8(10);
        new (&D_004A51C0) sSlot_18E8(11);
        new (&D_004A51C8) sSlot_18E8(12);
        new (&D_004A51D0) sSlot_18E8(13);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr__cReplayFramePtr);
#ifdef SKIP_ASM
void operator_delete(int* p);

extern void* cReplayFramePtr_vtbl[];

/**
 * Destroys a replay frame: restores cReplayFramePtr_vtbl and frees the object when bit 0 of @p flags is set (the
 * block itself is not freed here).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cReplayFramePtr__cReplayFramePtr(int* self, int flags)
{
    *(void**)self = cReplayFramePtr_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_clearBlock);
#ifdef SKIP_ASM
/**
 * Empties the frame without freeing its block: clears WriteData, ReadData and Size.
 */
extern "C" void cReplayFramePtr_clearBlock(cReplayFramePtr* self)
{
    self->WriteData = 0;
    self->ReadData = 0;
    self->Size = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272210);
#ifdef SKIP_ASM
extern "C" int func_00272210(cReplayFramePtr* replayFramePtr) {
    return ((int)replayFramePtr->Block);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272218);
#ifdef SKIP_ASM
extern "C" void func_00272218(cReplayFramePtr* replayFramePtr, int *arg1) {
    *arg1 = ((int)replayFramePtr->ReadData);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272228);
#ifdef SKIP_ASM
extern "C" void func_00272228(void *arg0, int *arg1) {
    (*(int *)((char*)(arg0) + (0xC))) = (int) *arg1;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272238);
#ifdef SKIP_ASM
extern "C" int func_00272238(cReplayFramePtr* replayFramePtr) {
    return (replayFramePtr->Size);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272240);
#ifdef SKIP_ASM
extern "C" int func_00272240(cReplayFramePtr* replayFramePtr) {
    return (replayFramePtr->Size);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272248);
#ifdef SKIP_ASM
extern "C" void func_00272248(int *arg0, int arg1) {
    *arg0 = arg1;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272250);
#ifdef SKIP_ASM
extern "C" void func_00272250(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (4))) = arg1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272258);
#ifdef SKIP_ASM
void operator_delete(int* p);

extern void* cReplayFramePtr_vtbl[];

extern "C" void func_00272258(int* self, int flags)
{
    *(void**)self = cReplayFramePtr_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272288);
#ifdef SKIP_ASM
extern "C" void func_00272288(int* self)
{
    self[0] = -1;
    self[0xB0 / 4] = 0;
    MEM_fill(self + 1, 0, 0xAC);
}
#endif

extern "C" void* func_002718E8(int, int);

//99.38%
INCLUDE_ASM("replay/replaycache", func_002722C0__FPv);
#ifdef SKIP_ASM
void* func_002722C0(void* self)
{
    return func_002718E8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002722E0__FPvN20);
#ifdef SKIP_ASM
void* func_002722E0(void* self, void* a1, void* a2)
{
    *(int*)((char*)self + 0x8) = (int)a1;
    *(int*)self = (int)a1;
    *(int*)((char*)self + 0x4) = (int)a2;
    *(int*)((char*)a1 + 0x4) = 0;
    *(int*)*(void**)((char*)self + 0x8) = (int)((char*)a2 - 0x8);
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272308);
#ifdef SKIP_ASM
struct sBlk_2308 {
    unsigned int size;
    sBlk_2308* next;
};
struct sHeap_2308 {
    int f0;
    int f4;
    sBlk_2308* free;
};

// First-fit allocation from the heap's free list (8-byte block headers); splits a block that has room to spare.
// PORT: the unit declares it as void*(void*, int); defined under its own name bound to the symbol.
extern "C" void* func_00272308_impl(sHeap_2308* h, unsigned int size) __asm__("func_00272308");
extern "C" void* func_00272308_impl(sHeap_2308* h, unsigned int size)
{
    size = (size + 3) & ~3;
    sBlk_2308* prev = 0;
    sBlk_2308* b = h->free;
    sBlk_2308* r = 0;
    unsigned int split = size + 16;
    if (b != 0) {
        do {
            if (b->size >= size) {
                if (split < b->size) {
                    sBlk_2308* nb = (sBlk_2308*)((char*)b + (size + 8));
                    nb->size = (b->size - size) - 8;
                    nb->next = b->next;
                    b->next = nb;
                    b->size = size;
                }
                r = b;
                if (prev == 0)
                    h->free = r->next;
                else
                    prev->next = r->next;
                b->next = 0;
                break;
            }
            prev = b;
            b = b->next;
        } while (b != 0);
    }
    return (char*)r + 8;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002723A8);
#ifdef SKIP_ASM
struct sRcBlock {
    int size;
    sRcBlock* next;
};

struct sRcHeap {
    int field_0x0;
    int field_0x4;
    sRcBlock* freeList;
};

extern "C" void func_002723A8(cScriptHeap* scriptHeap, void* p)
{
    if (p) {
        sRcBlock* b = (sRcBlock*)((char*)p - 8);
        if (!(sRcBlock*)scriptHeap->mpTopFreeBlock) {
            scriptHeap->mpTopFreeBlock = (tFreeBlock*)b;
            return;
        }
        sRcBlock* prev = 0;
        sRcBlock* cur = (sRcBlock*)scriptHeap->mpTopFreeBlock;
        do {
            if (b < cur) {
                int size = b->size + 8;
                if ((sRcBlock*)((char*)b + size) == cur) {
                    b->next = cur->next;
                    b->size = size + cur->size;
                    if (!prev)
                        scriptHeap->mpTopFreeBlock = (tFreeBlock*)b;
                    else
                        prev->next = b;
                    return;
                }
                if (!prev) {
                    b->next = (sRcBlock*)scriptHeap->mpTopFreeBlock;
                    scriptHeap->mpTopFreeBlock = (tFreeBlock*)b;
                } else {
                    b->next = prev->next;
                    prev->next = b;
                }
                return;
            }
            int size = cur->size + 8;
            if ((sRcBlock*)((char*)cur + size) == b) {
                cur->size = size + b->size;
                b = cur;
                cur = b->next;
                b->next = 0;
                if (!prev)
                    scriptHeap->mpTopFreeBlock = (tFreeBlock*)cur;
                else
                    prev->next = cur;
            } else {
                prev = cur;
                cur = cur->next;
            }
        } while (cur);
        if (!prev) {
            b->next = (sRcBlock*)scriptHeap->mpTopFreeBlock;
            scriptHeap->mpTopFreeBlock = (tFreeBlock*)b;
        } else {
            b->next = prev->next;
            prev->next = b;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", func_00272488);
#ifdef SKIP_ASM
extern "C" void* func_00272308(void* heap, int size);
extern int D_004A3488;

// PORT: real arity is (size, heap); the unit declares func_00272488(void*) for its callers.
extern "C" void* func_00272488_impl(int size, void* heap) __asm__("func_00272488");
extern "C" void* func_00272488_impl(int size, void* heap)
{
    void** p = (void**)func_00272308(heap, size + D_004A3488);
    *p = heap;
    return (char*)p + D_004A3488;
}
#endif

extern "C" void* func_00272488(void* self);

//99.29%
INCLUDE_ASM("replay/replaycache", func_002724C8__FPv);
#ifdef SKIP_ASM
void* func_002724C8(void* self)
{
    return func_00272488(self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002724E8);
#ifdef SKIP_ASM
extern int D_004A3488;

// PORT: returns void; the unit declares func_002724E8 as returning void*.
extern "C" void func_002724E8_impl(void* p) __asm__("func_002724E8");
extern "C" void func_002724E8_impl(void* p)
{
    char* h = (char*)p - D_004A3488;
    func_002723A8((cScriptHeap*)(*(sRcHeap**)h), h);
}
#endif

extern "C" void* func_002724E8(void* self);

//99.29%
INCLUDE_ASM("replay/replaycache", func_00272510__FPv);
#ifdef SKIP_ASM
void* func_00272510(void* self)
{
    return func_002724E8(self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272530);
#ifdef SKIP_ASM
extern "C" void func_00272530(void *arg0) {
    func_002724E8(arg0);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272550);
#ifdef SKIP_ASM
extern "C" void func_00272550(void *arg0) {
    func_002724E8(arg0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272570);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float func_00272570(float* p, float t)
{
    float r;
    if (t != 0.0f) {
        r = p[0];
        r = r * t + p[1];
        r = r * t + p[2];
        r = r * t + p[3];
    } else {
        r = p[3];
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_cScriptPlayerChannelSpline);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelSpline_vtbl[];

/**
 * Constructs a spline channel: stores its vtable and clears the data, owner, header, current-key and next-key fields.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelSpline_cScriptPlayerChannelSpline(cScriptPlayerChannelSpline* self)
{
    self->mVtbl = cScriptPlayerChannelSpline_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpCurrKey = 0;
    self->mCurrKeyIndex = (unsigned int)0;
    *(int*)&self->mCurrKeyValue = 0;
    self->mpCurrKeyCustom = 0;
    self->mpNextKey = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_Play);
#ifdef SKIP_ASM
extern "C" float func_00272570(float* coef, float t);

struct sRcTrack25F0 {
    char pad_0x00[0x8];
    unsigned int* data;     // 0x8
    int key;                // 0xC
    unsigned int* cur;      // 0x10
    char* body;             // 0x14
    int field_0x18;         // 0x18
    float value;            // 0x1C
    char* extra;            // 0x20
    char* next;             // 0x24
};

/**
 * Starts a spline channel of an in-game script sequence at its first key and evaluates its value at frame 0.
 *
 * Stores the data and owner, the header (key count in its first word) and the first key (data + 4), resets the key
 * index (+0x18) and points the next key at the first key plus its size (u16 at +0) when there are at least two
 * keys. The value is GetCubicSplineValue (func_00272570) of the key's coefficients at 0; keys larger than 0x14
 * bytes carry custom data at +0x14.
 *
 * @param data  the channel's script data (On Tour pScriptData)
 * @param key  the script player entity that owns the channel (On Tour pOwner)
 * @return 1 when there is a next key to reach, otherwise 0
 */
extern "C" int cScriptPlayerChannelSpline_Play(sRcTrack25F0* self, unsigned int* data, int key)
{
    char* body = (char*)data + 4;
    self->data = data;
    self->key = key;
    self->cur = data;
    self->body = body;
    self->field_0x18 = 0;
    if (data[0] >= 2) {
        self->next = body + *(unsigned short*)((char*)data + 4);
    } else {
        self->next = 0;
    }
    self->value = func_00272570((float*)(self->body + 4), 0.0f);
    char* b = self->body;
    if (*(unsigned short*)b >= 0x15) {
        self->extra = b + 0x14;
    } else {
        self->extra = 0;
    }
    return self->next != 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_Stop__FPv);
#ifdef SKIP_ASM
/**
 * Stops a spline channel: clears its data, owner, header, keys, key index, value and custom data (+0x8 .. +0x24).
 */
void cScriptPlayerChannelSpline_Stop(void* self)
{
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1c) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x24) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_Update);
#ifdef SKIP_ASM
/**
 * Advances a spline channel to a frame and evaluates the current key's spline there.
 *
 * When the next key's start frame (u16 at +2) has been reached it becomes the current key (with its custom data)
 * and the index grows; at the last key the value is evaluated at the key's start and 0 is returned. Otherwise the
 * value is GetCubicSplineValue (func_00272570) at @p currFrameNum minus the current key's start frame.
 *
 * @param currFrameNum  the sequence's current frame (On Tour currFrameNum)
 * @return 1 while the channel runs, 0 once it reached its last key (or has none to reach)
 */
extern "C" int cScriptPlayerChannelSpline_Update(sRcTrack25F0* self, float currFrameNum)
{
    char* n = self->next;
    if (n == 0) return 0;
    if ((float)*(unsigned short*)(n + 2) <= currFrameNum) {
        self->body = n;
        self->extra = 0;
        self->field_0x18++;
        if (*(unsigned short*)n >= 0x15) self->extra = n + 0x14;
        if (self->field_0x18 == self->cur[0] - 1) {
            self->next = 0;
            self->value = func_00272570((float*)(self->body + 4), 0.0f);
            return 0;
        }
        self->next = self->body + *(unsigned short*)self->body;
    }
    char* b = self->body;
    self->value = func_00272570((float*)(b + 4), currFrameNum - (float)*(unsigned short*)(b + 2));
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_cInSeq);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerMain_cScriptPlayerMain(cScriptPlayerMain* self);
// PORT: array new[] with a heap argument; same symbol as the unit's func_002724C8(void*).
void* operator new[](unsigned int size, void* heap) __asm__("func_002724C8__FPv");

struct sRcItem2788 {
    int field_0x0;              // 0x0
    unsigned int index;         // 0x4
    char body[0x48];            // 0x8
    sRcItem2788* next;          // 0x50
    sRcItem2788* prev;          // 0x54
    sRcItem2788() { cScriptPlayerMain_cScriptPlayerMain((cScriptPlayerMain*)body); }
    void operator delete[](void* p, unsigned int size);
};

struct sRcPool2788 {
    int tag;                    // 0x0
    unsigned int count;         // 0x4
    char heap[0xC];             // 0x8
    sRcItem2788* items;         // 0x14
    sRcItem2788* free;          // 0x18
    int used;                   // 0x1C
};

/**
 * Constructs the in-game sequence (InSeq) manager: a heap over the caller's buffer and a pool of script
 * players.
 *
 * Sets up the heap at +0x8 over @p pHeap (func_002722E0), stores @p tag at +0x0 and @p count at +0x4, then
 * allocates @p count 0x58-byte script nodes from that heap (each runs cScriptPlayerMain_cScriptPlayerMain()
 * on node + 8), numbers them and links them all into the free list (+0x18); the active list (+0x1C) starts
 * empty.
 *
 * @param pHeap     start of the memory the manager's heap manages
 * @param heapSize  size of that memory in bytes
 * @param count     number of script players to create (On Tour maxScripts)
 * @param tag       despite the name, the comm interface (On Tour pCommInterface):
 *                  cScriptPlayerMain_GetInSeqCommInterface() falls back to this word (+0x0)
 * @return @p self
 */
extern "C" sRcPool2788* cInSeq_cInSeq(sRcPool2788* self, void* pHeap, void* heapSize, unsigned int count, int tag)
{
    func_002722E0(self->heap, pHeap, heapSize);
    self->tag = tag;
    self->count = count;
    self->items = 0;
    self->free = 0;
    sRcItem2788** slot = &self->items;
    *slot = new (self->heap) sRcItem2788[count];
    for (unsigned int i = 0; i < self->count; i++) {
        self->items[i].index = i;
        self->items[i].next = &self->items[i + 1];
        self->items[i].prev = &self->items[i - 1];
    }
    self->items[0].prev = 0;
    self->items[self->count - 1].next = 0;
    self->used = 0;
    self->free = self->items;
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq__cInSeq);
#ifdef SKIP_ASM
void operator_delete(int* p);

/**
 * Destroys an input sequence; the only work is freeing the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// Deleting-destructor tail: free self when bit 0 of the g++ 2.95 in-charge flag is set.
extern "C" void cInSeq__cInSeq(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_UpdateFrame);
#ifdef SKIP_ASM
extern "C" void func_00274A30(cScriptPlayerMain* p);

/**
 * Updates every playing script: cScriptPlayerMain::UpdateFrame (func_00274A30) on each node of mpScriptUsed.
 */
extern "C" void cInSeq_UpdateFrame(cInSeq* self)
{
    char* n = (char*)self->mpScriptUsed;
    while (n != 0) {
        func_00274A30((cScriptPlayerMain*)(n + 8));
        n = *(char**)(n + 0x50);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_ReadyPlayer);
#ifdef SKIP_ASM
extern "C" void* func_002745C0(void* data, void* src, void* owner, int id);

struct sRcNode272938 {
    int unk_0x0;
    int id;
    char data[0x48];
    sRcNode272938* next;
    sRcNode272938* prev;
};


extern "C" int cInSeq_ReadyPlayer(cInSeq* self, void* src)
{
    sRcNode272938* n = ((sRcNode272938*)self->mpScriptUnused);
    (*(sRcNode272938**)&self->mpScriptUnused) = n->next;
    n->next = 0;
    if (((sRcNode272938*)self->mpScriptUnused) != 0) {
        ((sRcNode272938*)self->mpScriptUnused)->prev = 0;
    }
    func_002745C0(n->data, src, self, n->id);
    n->next = ((sRcNode272938*)self->mpScriptUsed);
    if (((sRcNode272938*)self->mpScriptUsed) != 0) {
        ((sRcNode272938*)self->mpScriptUsed)->prev = n;
    }
    (*(sRcNode272938**)&self->mpScriptUsed) = n;
    return n->id;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002729A8);
#ifdef SKIP_ASM
extern "C" void cInSeq_Stop(cInSeq* self, int a1);
extern "C" void func_002746F8(cScriptPlayerMain* p);

extern "C" void func_002729A8(cInSeq* self, int i)
{
    cInSeq_Stop((cInSeq*)self, i);
    char* base = *(char**)((char*)self + 0x14);
    sRcNode272938* n = (sRcNode272938*)(base + i * 0x58);
    func_002746F8((cScriptPlayerMain*)n->data);
    if (n->prev == 0) {
        (*(sRcNode272938**)&self->mpScriptUsed) = n->next;
        n->next = 0;
        if (((sRcNode272938*)self->mpScriptUsed) != 0) {
            ((sRcNode272938*)self->mpScriptUsed)->prev = 0;
        }
    } else {
        n->prev->next = n->next;
        if (n->next != 0) {
            n->next->prev = n->prev;
        }
        n->next = 0;
        n->prev = 0;
    }
    n->next = ((sRcNode272938*)self->mpScriptUnused);
    if (((sRcNode272938*)self->mpScriptUnused) != 0) {
        ((sRcNode272938*)self->mpScriptUnused)->prev = n;
    }
    (*(sRcNode272938**)&self->mpScriptUnused) = n;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_RecycleAll);
#ifdef SKIP_ASM
/**
 * Gives every playing script's player back to the free pool (func_002729A8, RecyclePlayer, on each used node).
 */
extern "C" void cInSeq_RecycleAll(cInSeq* self)
{
    sRcNode272938* n = (sRcNode272938*)self->mpScriptUsed;
    while (n != 0) {
        sRcNode272938* next = n->next;
        func_002729A8((self), n->id);
        n = next;
    }
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Preview);
#ifdef SKIP_ASM
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int a1);
extern "C" void cScriptPlayerMain_Preview(cScriptPlayerMain* self, int flag);

/**
 * Previews one script player (cScriptPlayerMain_Preview).
 *
 * On Tour's Preview takes a cInSeqComm* (pPreviewCommInterface) as its last argument; here it is an int, passed
 * through unchanged.
 *
 * @param playerID  the script player
 * @param flag  the preview argument (On Tour cInSeqComm* pPreviewCommInterface)
 */
extern "C" void cInSeq_Preview(cInSeq* self, int playerID, int flag)
{
    cScriptPlayerMain_Preview((cScriptPlayerMain*)((char*)cInSeq_GetPlayer(self, playerID)), flag);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Play);
#ifdef SKIP_ASM
extern "C" void* func_00274830(void*, int);
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int a1);

/**
 * Plays one script player (func_00274830, cScriptPlayerMain::Play).
 *
 * @param playerID  the script player
 * @param loopScript  nonzero to loop the script
 * @return func_00274830's result (On Tour's Play returns void)
 */
extern "C" void* cInSeq_Play(cInSeq* self, int playerID, int loopScript)
{
    return func_00274830(cInSeq_GetPlayer(self, playerID), loopScript);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272B00);
#ifdef SKIP_ASM
extern "C" void func_00272B00(cInSeq* inSeq, int arg1) {
    void *var_16;

    var_16 = ((void*)inSeq->mpScriptUsed);
    if (var_16 != 0) {
        do {
            cInSeq_Play(inSeq, (*(int *)((char*)(var_16) + (4))), arg1);
            var_16 = (*(void **)((char*)(var_16) + (0x50)));
        } while (var_16 != 0);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Stop);
#ifdef SKIP_ASM
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int a1);
extern "C" void cScriptPlayerMain_Stop(cScriptPlayerMain* p);

/**
 * Stops one script player (cScriptPlayerMain_Stop).
 *
 * @param playerID  the script player
 */
extern "C" void cInSeq_Stop(cInSeq* self, int playerID)
{
    cScriptPlayerMain_Stop((cScriptPlayerMain*)cInSeq_GetPlayer(self, playerID));
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_StopAll);
#ifdef SKIP_ASM
/**
 * Stops every playing script player (cInSeq_Stop on each node of mpScriptUsed).
 */
extern "C" void cInSeq_StopAll(cInSeq* self)
{
    sRcNode272938* n;
    for (n = (sRcNode272938*)self->mpScriptUsed; n != 0; n = n->next) {
        cInSeq_Stop(self, n->id);
    }
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Pause);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Pause(cScriptPlayerMain* self);

/**
 * Pauses one script player (cScriptPlayerMain_Pause).
 *
 * @param playerID  the script player
 */
extern "C" void cInSeq_Pause(cInSeq* self, int playerID)
{
    cScriptPlayerMain_Pause((cScriptPlayerMain*)cInSeq_GetPlayer(self, playerID));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_PauseAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_Pause(cInSeq* self, int a1);

/**
 * Pauses every playing script player (cInSeq_Pause on each node of mpScriptUsed).
 */
extern "C" void cInSeq_PauseAll(cInSeq* self)
{
    sRcNode272938* n;
    for (n = (sRcNode272938*)self->mpScriptUsed; n != 0; n = n->next) {
        cInSeq_Pause(self, n->id);
    }
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Resume);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Resume(cScriptPlayerMain* self);

/**
 * Resumes one script player (cScriptPlayerMain_Resume).
 *
 * @param playerID  the script player
 */
extern "C" void cInSeq_Resume(cInSeq* self, int playerID)
{
    cScriptPlayerMain_Resume((cScriptPlayerMain*)cInSeq_GetPlayer(self, playerID));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_ResumeAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_Resume(cInSeq* self, int a1);

/**
 * Resumes every playing script player (cInSeq_Resume on each node of mpScriptUsed).
 */
extern "C" void cInSeq_ResumeAll(cInSeq* self)
{
    sRcNode272938* n;
    for (n = (sRcNode272938*)self->mpScriptUsed; n != 0; n = n->next) {
        cInSeq_Resume(self, n->id);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_GetPlayer);
#ifdef SKIP_ASM
/**
 * Returns the script player of pool slot @p playerID (node + 8 in mpScriptHeap, 0x58 per node) when its node's word
 * at +0xC is set.
 *
 * @param playerID  the slot
 * @return the cScriptPlayerMain, or 0 when that slot's player is not set up
 */
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int playerID)
{
    char* base = (char*)self->mpScriptHeap;
    char* p = base + playerID * 0x58;
    return *(int*)(p + 0xc) != 0 ? p + 0x8 : 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272CE0);
#ifdef SKIP_ASM
extern "C" void func_00274C58();

extern "C" void func_00272CE0(void) {
    func_00274C58();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272D00);
#ifdef SKIP_ASM
extern "C" void func_00274C60();

extern "C" void func_00272D00(void) {
    func_00274C60();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272D20);
#ifdef SKIP_ASM
extern "C" void func_00274C68();

extern "C" void func_00272D20(void) {
    func_00274C68();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272D40);
#ifdef SKIP_ASM
extern "C" void func_00274398(int);
extern "C" int func_00274D90();

extern "C" void func_00272D40(void) {
    func_00274398(func_00274D90());
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_cScriptPlayerChannelBlock);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelBlock_vtbl[];

/**
 * Constructs a block channel: stores its vtable and clears the data, owner, header, next-key, block-array and
 * active-block fields.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelBlock_cScriptPlayerChannelBlock(cScriptPlayerChannelBlock* self)
{
    self->mVtbl = cScriptPlayerChannelBlock_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpNextKey = 0;
    self->mNextKeyIndex = 0;
    self->mpBlockArray = 0;
    self->mpActiveBlockList = 0;
    self->mNumActiveBlocks = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock__cScriptPlayerChannelBlock);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerChannelBlock_Stop(void* self);
extern "C" void* func_002724E8(void* self);
extern void* cScriptPlayerChannelBlock_vtbl[];

/**
 * Destroys a block channel: restores its vtable (+0x4), stops it (freeing its key buffer) and, when bit 0 of @p
 * flags is set, frees it with func_002724E8 (On Tour cScriptMemOp::operator delete).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerChannelBlock__cScriptPlayerChannelBlock(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = cScriptPlayerChannelBlock_vtbl;
    cScriptPlayerChannelBlock_Stop(self);
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_Play);
#ifdef SKIP_ASM
struct sCacheRecord;
struct sCacheEntry;
extern "C" void* func_00272308(void* heap, int size);
extern "C" sCacheEntry* cScriptPlayerChannelBlock_AddActiveKey(cScriptPlayerChannelBlock* self, sCacheRecord* rec);
extern "C" void cScriptPlayerChannelBlock_LoadNextScriptKey(cScriptPlayerChannelBlock* self);

/**
 * Starts a block channel of an in-game script sequence and activates every key that starts at frame 0.
 *
 * Stores the data and owner; when the channel has keys it allocates data[1] active-key records of 0x1C bytes from
 * the owner's script heap (func_00272308, On Tour cScriptHeap::AllocMem), resets the index and starts at the first
 * key (data + 4). Each key whose start frame (+4) is 0 sets 0x4 in @p flags and is activated (cScriptPlayerChannelBlock_AddActiveKey
 * AddActiveKey, cScriptPlayerChannelBlock_LoadNextScriptKey LoadNextScriptKey).
 *
 * @param data  the channel's script data (On Tour pScriptData): key count, active-key count, keys
 * @param owner  the script player entity that owns the channel (On Tour pOwner)
 * @param flags  the sequence's event bits (On Tour int& events)
 * @return 1 when the channel has keys, otherwise 0
 */
extern "C" int cScriptPlayerChannelBlock_Play(void* self, unsigned short* data, void* owner, int* flags)
{
    *(unsigned short**)((char*)self + 0x8) = data;
    *(void**)((char*)self + 0xC) = owner;
    *(unsigned short**)((char*)self + 0x10) = data;
    if (data[0] != 0) {
        char* heap = *(char**)(*(char**)((char*)owner + 0x8) + 0x8);
        *(void**)((char*)self + 0x20) = func_00272308(heap + 8, data[1] * 0x1C);
        *(int*)((char*)self + 0x18) = 0;
        char* base = *(char**)((char*)self + 0x10);
        *(char**)((char*)self + 0x14) = base + 4;
        if (base + 4 != 0 && *(unsigned short*)(base + 8) == 0) do {
            *flags |= 4;
            cScriptPlayerChannelBlock_AddActiveKey((cScriptPlayerChannelBlock*)self, *(sCacheRecord**)((char*)self + 0x14));
            cScriptPlayerChannelBlock_LoadNextScriptKey((cScriptPlayerChannelBlock*)self);
        } while (*(char**)((char*)self + 0x14) != 0 &&
               *(unsigned short*)(*(char**)((char*)self + 0x14) + 4) == 0);
    }
    return **(unsigned short**)((char*)self + 0x10) != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_Stop);
#ifdef SKIP_ASM
/**
 * Stops a block channel: frees its active-key buffer to the owner's script heap and clears its state (only when it
 * has an owner).
 */
extern "C" void cScriptPlayerChannelBlock_Stop(void* self)
{
    char* owner = *(char**)((char*)self + 0xC);
    if (owner != 0) {
        void* p = *(void**)((char*)self + 0x20);
        if (p != 0) {
            func_002723A8((cScriptHeap*)((sRcHeap*)(*(char**)(*(char**)(owner + 0x8) + 0x8) + 0x8)), p);
        }
        *(int*)((char*)self + 0x8) = 0;
        *(int*)((char*)self + 0xC) = 0;
        *(int*)((char*)self + 0x10) = 0;
        *(int*)((char*)self + 0x14) = 0;
        *(int*)((char*)self + 0x18) = 0;
        *(int*)((char*)self + 0x20) = 0;
        *(int*)((char*)self + 0x24) = 0;
        *(int*)((char*)self + 0x28) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_Update);
#ifdef SKIP_ASM
struct sCacheEvent_2F28 {
    char pad0[0xC];
    int end;                        // 0xC
    char pad10[0x18 - 0x10];
    sCacheEvent_2F28* next;         // 0x18
};

struct sCacheRec_2F28 {
    char pad0[4];
    unsigned short time;            // 0x4
};

struct sCacheTrack_2F28 {
    char pad0[0x14];
    sCacheRec_2F28* rec;            // 0x14
    char pad18[0x24 - 0x18];
    sCacheEvent_2F28* events;       // 0x24
    int numEvents;                  // 0x28
};

/**
 * Advances a block channel to a frame: drops active keys that have ended and activates every key that has started.
 *
 * Each active key whose end frame has passed is unlinked (0x8 in @p flags) and the active count drops; then every
 * pending key whose start frame is reached sets 0x4 and is activated (AddActiveKey, LoadNextScriptKey).
 *
 * @param flags  the sequence's event bits (On Tour int& events)
 * @param currFrameNum  the sequence's current frame (On Tour currFrameNum)
 * @return 0 when no key is active or pending, otherwise 1
 */
// Drops finished events and plays every record whose start time has been reached.
extern "C" int cScriptPlayerChannelBlock_Update(sCacheTrack_2F28* self, int* flags, float currFrameNum)
{
    float zero;
    sCacheEvent_2F28* prev = 0;
    sCacheEvent_2F28* e = self->events;
    while (e != 0) {
        if ((float)e->end <= currFrameNum) {
            *flags |= 8;
            if (prev == 0) {
                e = self->events = e->next;
            } else {
                e = e->next;
                prev->next = e;
            }
            self->numEvents--;
        } else {
            prev = e;
            e = e->next;
        }
    }
    zero = 0.0f;
loop:
    do {
        if (self->rec != 0 && currFrameNum - (float)self->rec->time >= zero) {
            *flags |= 4;
            cScriptPlayerChannelBlock_AddActiveKey((cScriptPlayerChannelBlock*)self, (sCacheRecord*)self->rec);
            cScriptPlayerChannelBlock_LoadNextScriptKey((cScriptPlayerChannelBlock*)self);
            goto loop;
        }
    } while (0);
    if (self->events == 0 && self->rec == 0)
        return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetNextKeyFrame);
#ifdef SKIP_ASM
struct sCacheTimeNode {
    char pad[0xC];
    int time;
    char pad2[0x18 - 0x10];
    sCacheTimeNode* next;
};

/**
 * Returns the earliest frame at which the block channel changes: the soonest end of an active key or the next key's
 * start.
 *
 * @return that frame, or -1 when the channel is not playing, has no keys or nothing is left
 */
extern "C" int cScriptPlayerChannelBlock_GetNextKeyFrame(void* self)
{
    if (*(int*)((char*)self + 0x8) == 0) {
        return -1;
    }
    if (**(unsigned short**)((char*)self + 0x10) == 0) {
        return -1;
    }
    int best = -1;
    sCacheTimeNode* n = *(sCacheTimeNode**)((char*)self + 0x24);
    unsigned short* cur = *(unsigned short**)((char*)self + 0x14);
    for (; n != 0; n = n->next) {
        int t = n->time;
        if (best < 0 || t < best) {
            best = t;
        }
    }
    if (cur != 0) {
        int t = cur[2];
        if (best < 0 || t < best) {
            best = t;
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_AddActiveKey);
#ifdef SKIP_ASM
struct sCacheRecord {
    unsigned short size;
    unsigned short id;
    unsigned short a;
    unsigned short b;
    int x;
    int y;
};

struct sCacheEntry {
    int x;
    int id;
    int a;
    int b;
    int y;
    sCacheRecord* data;
    sCacheEntry* next;
};

/**
 * Starts a block key: fills mpBlockArray[key id] from the key and pushes it on mpActiveBlockList.
 *
 * Copies the key's fields, points the entry's data at the bytes after the key's 0x10-byte header when the key is
 * larger, and counts mNumActiveBlocks up.
 *
 * @param rec  the key (On Tour tInSeqBlockChannelKey* pKey)
 * @return the entry (On Tour tBlockEntry*)
 */
extern "C" sCacheEntry* cScriptPlayerChannelBlock_AddActiveKey(cScriptPlayerChannelBlock* self, sCacheRecord* rec)
{
    sCacheEntry* e = (sCacheEntry*)((char*)self->mpBlockArray + rec->id * 0x1C);
    e->x = rec->x;
    e->id = rec->id;
    e->y = rec->y;
    e->a = rec->a;
    e->b = rec->b;
    e->data = 0;
    if (rec->size > 0x10) {
        e->data = rec + 1;
    }
    e->next = (sCacheEntry*)self->mpActiveBlockList;
    self->mpActiveBlockList = (tBlockEntry*)e;
    (self->mNumActiveBlocks)++;
    return e;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_LoadNextScriptKey);
#ifdef SKIP_ASM
/**
 * Steps mpNextKey to the following key (each key starts with its size), or to 0 after the header's last key.
 */
extern "C" void cScriptPlayerChannelBlock_LoadNextScriptKey(cScriptPlayerChannelBlock* self)
{
    int idx = self->mNextKeyIndex;
    if (idx == *(unsigned short*)self->mpHeader - 1) {
        self->mpNextKey = 0;
    } else {
        char* cur = (char*)self->mpNextKey;
        self->mpNextKey = (tInSeqBlockChannelKey*)(cur + *(unsigned short*)cur);
        self->mNextKeyIndex = idx + 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_cScriptPlayerChannelEvent);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelEvent_vtbl[];

/**
 * Constructs an event channel: stores its vtable and clears the data, owner, header, next-key, event-array fields,
 * mNumActiveEvents and mEventFrame.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelEvent_cScriptPlayerChannelEvent(cScriptPlayerChannelEvent* self)
{
    self->mVtbl = cScriptPlayerChannelEvent_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpNextKey = 0;
    self->mNextKeyIndex = 0;
    self->mpEventArray = 0;
    self->mNumActiveEvents = 0;
    *(int*)&self->mEventFrame = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent__cScriptPlayerChannelEvent);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerChannelEvent_Stop(void* self);
extern "C" void* func_002724E8(void* self);
extern void* cScriptPlayerChannelEvent_vtbl[];

/**
 * Destroys an event channel: restores its vtable (+0x4), stops it (freeing its key buffer) and, when bit 0 of @p
 * flags is set, frees it with func_002724E8 (On Tour cScriptMemOp::operator delete).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerChannelEvent__cScriptPlayerChannelEvent(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = cScriptPlayerChannelEvent_vtbl;
    cScriptPlayerChannelEvent_Stop(self);
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_Play);
#ifdef SKIP_ASM
struct sCacheRec;
struct sCacheRef;
extern "C" void* func_00272308(void* heap, int size);
extern "C" sCacheRef* cScriptPlayerChannelEvent_AddActiveKey(cScriptPlayerChannelEvent* self, sCacheRec* rec);
extern "C" void cScriptPlayerChannelEvent_LoadNextScriptKey(cScriptPlayerChannelEvent* self);

/**
 * Starts an event channel of an in-game script sequence and fires every event key at frame 0.
 *
 * Stores the data and owner; when the channel has keys it allocates data[1] records of 8 bytes from the owner's
 * script heap (func_00272308), resets the index and starts at the first key (data + 4). Each key whose frame (+2)
 * is 0 sets 0x10 in @p flags and is fired (cScriptPlayerChannelEvent_AddActiveKey AddActiveKey, cScriptPlayerChannelEvent_LoadNextScriptKey LoadNextScriptKey); the
 * last-fired frame (+0x24) is reset each time.
 *
 * @param data  the channel's script data (On Tour pScriptData): key count, buffer count, keys
 * @param owner  the script player entity that owns the channel (On Tour pOwner)
 * @param flags  the sequence's event bits (On Tour int& events)
 * @return 1 while a key is still pending, otherwise 0
 */
extern "C" int cScriptPlayerChannelEvent_Play(void* self, unsigned short* data, void* owner, int* flags)
{
    *(unsigned short**)((char*)self + 0x8) = data;
    *(void**)((char*)self + 0xC) = owner;
    *(unsigned short**)((char*)self + 0x10) = data;
    if (data[0] != 0) {
        char* heap = *(char**)(*(char**)((char*)owner + 0x8) + 0x8);
        *(void**)((char*)self + 0x1C) = func_00272308(heap + 8, data[1] * 8);
        *(int*)((char*)self + 0x18) = 0;
        char* base = *(char**)((char*)self + 0x10);
        *(char**)((char*)self + 0x14) = base + 4;
        if (base + 4 != 0 && *(unsigned short*)(base + 6) == 0) do {
            *flags |= 0x10;
            cScriptPlayerChannelEvent_AddActiveKey((cScriptPlayerChannelEvent*)self, *(sCacheRec**)((char*)self + 0x14));
            cScriptPlayerChannelEvent_LoadNextScriptKey((cScriptPlayerChannelEvent*)self);
            *(int*)((char*)self + 0x24) = 0;
        } while (*(char**)((char*)self + 0x14) != 0 &&
               *(unsigned short*)(*(char**)((char*)self + 0x14) + 2) == 0);
    }
    return *(char**)((char*)self + 0x14) != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_Stop);
#ifdef SKIP_ASM
/**
 * Stops an event channel: frees its key buffer to the owner's script heap and clears its state (only when it has an
 * owner).
 */
extern "C" void cScriptPlayerChannelEvent_Stop(void* self)
{
    char* owner = *(char**)((char*)self + 0xC);
    if (owner != 0) {
        void* p = *(void**)((char*)self + 0x1C);
        if (p != 0) {
            func_002723A8((cScriptHeap*)((sRcHeap*)(*(char**)(*(char**)(owner + 0x8) + 0x8) + 0x8)), p);
        }
        *(int*)((char*)self + 0x8) = 0;
        *(int*)((char*)self + 0xC) = 0;
        *(int*)((char*)self + 0x10) = 0;
        *(int*)((char*)self + 0x14) = 0;
        *(int*)((char*)self + 0x18) = 0;
        *(int*)((char*)self + 0x1C) = 0;
        *(int*)((char*)self + 0x20) = 0;
        *(int*)((char*)self + 0x24) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_Update);
#ifdef SKIP_ASM
struct sCacheRec;
struct sCacheRef;
extern "C" sCacheRef* cScriptPlayerChannelEvent_AddActiveKey(cScriptPlayerChannelEvent* self, sCacheRec* rec);
extern "C" void cScriptPlayerChannelEvent_LoadNextScriptKey(cScriptPlayerChannelEvent* self);

/**
 * Advances an event channel to a frame and fires every event key that has been reached.
 *
 * When @p currFrameNum differs from the last fired frame (+0x24) the per-frame count (+0x20) is cleared. Each pending key
 * whose frame is reached sets 0x10 in @p flags, is fired (AddActiveKey), records its frame and loads the next key.
 *
 * @param flags  the sequence's event bits (On Tour int& events)
 * @param currFrameNum  the sequence's current frame (On Tour currFrameNum)
 * @return 1 while a key is still pending, otherwise 0
 */
extern "C" int cScriptPlayerChannelEvent_Update(void* self, int* flags, float currFrameNum)
{
    if (*(float*)((char*)self + 0x24) != currFrameNum) {
        *(int*)((char*)self + 0x20) = 0;
    }
    while (*(char**)((char*)self + 0x14) != 0) {
        float v = (float)*(unsigned short*)(*(char**)((char*)self + 0x14) + 2);
        if (currFrameNum < v) break;
        *flags |= 0x10;
        cScriptPlayerChannelEvent_AddActiveKey((cScriptPlayerChannelEvent*)self, *(sCacheRec**)((char*)self + 0x14));
        *(float*)((char*)self + 0x24) = v;
        cScriptPlayerChannelEvent_LoadNextScriptKey((cScriptPlayerChannelEvent*)self);
    }
    return *(char**)((char*)self + 0x14) != 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetNextKeyFrame);
#ifdef SKIP_ASM
/**
 * Returns the frame of the event channel's next pending key.
 *
 * @return that frame, or -1 when the channel is not playing, has no keys or none is pending
 */
extern "C" int cScriptPlayerChannelEvent_GetNextKeyFrame(void* self)
{
    if (*(int*)((char*)self + 0x8) == 0) {
        return -1;
    }
    if (**(unsigned short**)((char*)self + 0x10) == 0) {
        return -1;
    }
    unsigned short* cur = *(unsigned short**)((char*)self + 0x14);
    int r = -1;
    if (cur) {
        r = cur[1];
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_AddActiveKey);
#ifdef SKIP_ASM
struct sCacheRef {
    int key;
    void* data;
};

struct sCacheRec {
    unsigned short size;
    unsigned short pad;
    int key;
};

/**
 * Fires an event key: fills mpEventArray[mNumActiveEvents] with the key's id and its payload (the bytes after the
 * 8-byte header, when any), and counts mNumActiveEvents up.
 *
 * @param rec  the key (On Tour tInSeqEventChannelKey* pKey)
 * @return the entry (On Tour tEventEntry*)
 */
extern "C" sCacheRef* cScriptPlayerChannelEvent_AddActiveKey(cScriptPlayerChannelEvent* self, sCacheRec* rec)
{
    sCacheRef* e = &((sCacheRef*)self->mpEventArray)[self->mNumActiveEvents];
    e->key = rec->key;
    e->data = 0;
    if (rec->size > 8) {
        e->data = (char*)rec + 8;
    }
    self->mNumActiveEvents += 1;
    return e;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_LoadNextScriptKey);
#ifdef SKIP_ASM
/**
 * Steps mpNextKey to the following key (each key starts with its size), or to 0 after the header's last key.
 */
extern "C" void cScriptPlayerChannelEvent_LoadNextScriptKey(cScriptPlayerChannelEvent* self)
{
    int idx = self->mNextKeyIndex;
    if (idx == *(unsigned short*)self->mpHeader - 1) {
        self->mpNextKey = 0;
    } else {
        char* cur = (char*)self->mpNextKey;
        self->mpNextKey = (tInSeqEventChannelKey*)(cur + *(unsigned short*)cur);
        self->mNextKeyIndex = idx + 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_cScriptPlayerChannelObject);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelObject_vtbl[];

/**
 * Constructs an object channel: stores its vtable and clears the data, owner, header, next-key, object-array and
 * active-object fields.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelObject_cScriptPlayerChannelObject(cScriptPlayerChannelObject* self)
{
    self->mVtbl = cScriptPlayerChannelObject_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpNextKey = 0;
    self->mNextKeyIndex = 0;
    self->mpObjectArray = 0;
    self->mpActiveObjectList = 0;
    self->mNumActiveObjects = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject__cScriptPlayerChannelObject);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerChannelObject_Stop(void* self);
extern "C" void* func_002724E8(void* self);
extern void* cScriptPlayerChannelObject_vtbl[];

/**
 * Destroys an object channel: restores its vtable (+0x4), stops it (freeing its key buffer) and, when bit 0 of @p
 * flags is set, frees it with func_002724E8 (On Tour cScriptMemOp::operator delete).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerChannelObject__cScriptPlayerChannelObject(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = cScriptPlayerChannelObject_vtbl;
    cScriptPlayerChannelObject_Stop(self);
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_Play);
#ifdef SKIP_ASM
extern "C" void* func_00272308(void* heap, int size);
struct sCacheRecord2;
struct sCacheEntry2;
extern "C" sCacheEntry2* cScriptPlayerChannelObject_AddActiveKey(cScriptPlayerChannelObject* self, sCacheRecord2* rec);
extern "C" void cScriptPlayerChannelObject_LoadNextScriptKey(cScriptPlayerChannelObject* self);

/**
 * Starts an object channel of an in-game script sequence and activates every key that starts at frame 0.
 *
 * Stores the data and owner; when the channel has keys it allocates data[1] active-key records of 0x24 bytes from
 * the owner's script heap (func_00272308; an empty loop over them follows), resets the index and starts at the
 * first key. Each key whose start frame (+4) is 0 sets 0x1 in @p flags and is activated (cScriptPlayerChannelObject_AddActiveKey
 * AddActiveKey, cScriptPlayerChannelObject_LoadNextScriptKey LoadNextScriptKey).
 *
 * @param data  the channel's script data (On Tour pScriptData): key count, active-key count, keys
 * @param owner  the script player entity that owns the channel (On Tour pOwner)
 * @param flags  the sequence's event bits (On Tour int& events)
 * @return 1 when the channel has keys, otherwise 0
 */
extern "C" int cScriptPlayerChannelObject_Play(void* self, unsigned short* data, void* owner, int* flags)
{
    *(unsigned short**)((char*)self + 0x8) = data;
    *(void**)((char*)self + 0xC) = owner;
    *(unsigned short**)((char*)self + 0x10) = data;
    if (data[0] != 0) {
        char* heap = *(char**)(*(char**)((char*)owner + 0x8) + 0x8);
        *(void**)((char*)self + 0x1C) = func_00272308(heap + 8, data[1] * 0x24);
        for (int i = 0; i < (*(unsigned short**)((char*)self + 0x10))[1]; i++) {
        }
        *(int*)((char*)self + 0x18) = 0;
        char* base = *(char**)((char*)self + 0x10);
        *(char**)((char*)self + 0x14) = base + 4;
        if (base + 4 != 0 && *(unsigned short*)(base + 8) == 0) do {
            *flags |= 1;
            cScriptPlayerChannelObject_AddActiveKey((cScriptPlayerChannelObject*)self, *(sCacheRecord2**)((char*)self + 0x14));
            cScriptPlayerChannelObject_LoadNextScriptKey((cScriptPlayerChannelObject*)self);
        } while (*(char**)((char*)self + 0x14) != 0 &&
               *(unsigned short*)(*(char**)((char*)self + 0x14) + 4) == 0);
    }
    return **(unsigned short**)((char*)self + 0x10) != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_Stop);
#ifdef SKIP_ASM
/**
 * Stops an object channel: frees its active-key buffer to the owner's script heap and clears its state (only when
 * it has an owner).
 */
extern "C" void cScriptPlayerChannelObject_Stop(void* self)
{
    char* owner = *(char**)((char*)self + 0xC);
    if (owner != 0) {
        void* p = *(void**)((char*)self + 0x1C);
        if (p != 0) {
            func_002723A8((cScriptHeap*)((sRcHeap*)(*(char**)(*(char**)(owner + 0x8) + 0x8) + 0x8)), p);
        }
        *(int*)((char*)self + 0x8) = 0;
        *(int*)((char*)self + 0xC) = 0;
        *(int*)((char*)self + 0x10) = 0;
        *(int*)((char*)self + 0x14) = 0;
        *(int*)((char*)self + 0x18) = 0;
        *(int*)((char*)self + 0x1C) = 0;
        *(int*)((char*)self + 0x20) = 0;
        *(int*)((char*)self + 0x24) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_Update);
#ifdef SKIP_ASM
struct sCacheEvent_3698 {
    char pad0[0x8];
    int end;                        // 0x8
    char padC[0x10 - 0xC];
    float value;                    // 0x10
    float speed;                    // 0x14
    char pad18[0x20 - 0x18];
    sCacheEvent_3698* next;         // 0x20
};

struct sCacheRec_3698 {
    char pad0[4];
    unsigned short time;            // 0x4
};

struct sCacheTrack_3698 {
    char pad0[0x14];
    sCacheRec_3698* rec;            // 0x14
    char pad18[0x20 - 0x18];
    sCacheEvent_3698* events;       // 0x20
    int numEvents;                  // 0x24
};

/**
 * Advances an object channel to a frame: moves active keys on, drops ended ones and activates every key that has
 * started.
 *
 * Active keys that have ended are unlinked (0x2 in @p flags); the others advance their value by speed * @p dt. Each
 * pending key whose start frame is reached sets 0x1, is activated and has its value advanced by the frames it is
 * late (t - start) times its speed.
 *
 * @param flags  the sequence's event bits (On Tour int& events)
 * @param frameDelta  the sequence's current frame (On Tour currFrameNum)
 * @param dt  the frames since the last update (On Tour frameDelta)
 * @return 0 when no key is active or pending, otherwise 1
 */
// Advances running events by dt, drops finished ones and starts every record that is due.
extern "C" int cScriptPlayerChannelObject_Update(sCacheTrack_3698* self, int* flags, float frameDelta, float dt)
{
    float zero;
    sCacheEvent_3698* prev = 0;
    sCacheEvent_3698* e = self->events;
    while (e != 0) {
        if ((float)e->end <= frameDelta) {
            *flags |= 2;
            if (prev == 0) {
                e = self->events = e->next;
            } else {
                e = e->next;
                prev->next = e;
            }
            self->numEvents--;
        } else {
            prev = e;
            e->value += e->speed * dt;
            e = e->next;
        }
    }
    zero = 0.0f;
loop:
    do {
        if (self->rec != 0) {
            float d = frameDelta - (float)self->rec->time;
            if (d >= zero) {
                *flags |= 1;
                sCacheEvent_3698* n = (sCacheEvent_3698*)cScriptPlayerChannelObject_AddActiveKey((cScriptPlayerChannelObject*)self, (sCacheRecord2*)self->rec);
                n->value += d * n->speed;
                cScriptPlayerChannelObject_LoadNextScriptKey((cScriptPlayerChannelObject*)self);
                goto loop;
            }
        }
    } while (0);
    if (self->events == 0 && self->rec == 0)
        return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetNextKeyFrame);
#ifdef SKIP_ASM
struct sCacheTimeNode2 {
    char pad[0x8];
    int time;
    char pad2[0x20 - 0xC];
    sCacheTimeNode2* next;
};

/**
 * Returns the earliest frame at which the object channel changes: the soonest end of an active key or the next
 * key's start.
 *
 * @return that frame, or -1 when the channel is not playing, has no keys or nothing is left
 */
extern "C" int cScriptPlayerChannelObject_GetNextKeyFrame(void* self)
{
    if (*(int*)((char*)self + 0x8) == 0) {
        return -1;
    }
    if (**(unsigned short**)((char*)self + 0x10) == 0) {
        return -1;
    }
    int best = -1;
    sCacheTimeNode2* n = *(sCacheTimeNode2**)((char*)self + 0x20);
    unsigned short* cur = *(unsigned short**)((char*)self + 0x14);
    for (; n != 0; n = n->next) {
        int t = n->time;
        if (best < 0 || t < best) {
            best = t;
        }
    }
    if (cur != 0) {
        int t = cur[2];
        if (best < 0 || t < best) {
            best = t;
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_AddActiveKey);
#ifdef SKIP_ASM
struct sCacheRecord2 {
    unsigned short size;
    unsigned short id;
    unsigned short a;
    unsigned short b;
    int x;
    int ix;
    float f;
    int y;
};

struct sCacheEntry2 {
    int x;
    int a;
    int b;
    int id;
    float fx;
    float f;
    int y;
    sCacheRecord2* data;
    sCacheEntry2* next;
};

/**
 * Starts an object key: fills mpObjectArray[key id] from the key and pushes it on mpActiveObjectList.
 *
 * Copies the key's fields, points the entry's data at the bytes after the key's 0x18-byte header when the key is
 * larger, and counts mNumActiveObjects up.
 *
 * @param rec  the key (On Tour tInSeqObjectChannelKey* pKey)
 * @return the entry (On Tour tObjectEntry*)
 */
extern "C" sCacheEntry2* cScriptPlayerChannelObject_AddActiveKey(cScriptPlayerChannelObject* self, sCacheRecord2* rec)
{
    sCacheEntry2* e = (sCacheEntry2*)((char*)self->mpObjectArray + rec->id * 0x24);
    e->x = rec->x;
    e->y = rec->y;
    e->fx = rec->ix;
    e->f = rec->f;
    e->id = rec->id;
    e->a = rec->a;
    e->b = rec->b;
    e->data = 0;
    if (rec->size > 0x18) {
        e->data = rec + 1;
    }
    e->next = (sCacheEntry2*)self->mpActiveObjectList;
    self->mpActiveObjectList = (tObjectEntry*)e;
    (self->mNumActiveObjects)++;
    return e;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_LoadNextScriptKey);
#ifdef SKIP_ASM
/**
 * Steps mpNextKey to the following key (each key starts with its size), or to 0 after the header's last key.
 */
extern "C" void cScriptPlayerChannelObject_LoadNextScriptKey(cScriptPlayerChannelObject* self)
{
    int idx = self->mNextKeyIndex;
    if (idx == *(unsigned short*)self->mpHeader - 1) {
        self->mpNextKey = 0;
    } else {
        char* cur = (char*)self->mpNextKey;
        self->mpNextKey = (tInSeqObjectChannelKey*)(cur + *(unsigned short*)cur);
        self->mNextKeyIndex = idx + 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_cScriptPlayerEntity);
#ifdef SKIP_ASM
/**
 * Constructs an InSeq script entity player with no script, channels or entity comm interface.
 *
 * Zeroes mpData through mpEntityComm (+0x4 to +0x2C); unk_00 and the three setup flags (+0x30 to +0x38) are
 * left alone.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerEntity_cScriptPlayerEntity(cScriptPlayerEntity* self)
{
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpUserData = 0;
    self->mpBlockHeader = 0;
    self->mpVariationTable = 0;
    self->mCurrVariation = 0;
    self->mpSystemHeader = 0;
    self->mpHeader = 0;
    self->mpChannelPlayers = 0;
    self->mIsPlaying = 0;
    self->mpEntityComm = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity__cScriptPlayerEntity);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_DeInit(cScriptPlayerEntity* self);

/**
 * Destroys an entity player: runs cScriptPlayerEntity_DeInit(), then frees the object when asked to.
 *
 * The memory goes back to the cInSeq heap it came from, through func_002724E8.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerEntity__cScriptPlayerEntity(cScriptPlayerEntity* self, int flags)
{
    cScriptPlayerEntity_DeInit(self);
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_Init);
#ifdef SKIP_ASM
/**
 * Binds the entity player to its block of script data and selects variation 0.
 *
 * Stores @p data as mpData and mpBlockHeader, the owner, the variation table (data + 8) and the first
 * variation's header (data + table[0]); mpHeader points at that variation's custom header when its headerSize
 * is above 8, else 0.
 *
 * @param data  the entity's script block (a tInSeqScriptEntityBlockHeader first)
 * @param size  despite the name, the owning cScriptPlayerMain (On Tour pOwner), stored in mpOwner
 * @return the block's blockSize, so the caller can step to the next entity block
 */
extern "C" int cScriptPlayerEntity_Init(cScriptPlayerEntity* self, char* data, int size)
{
    self->mpData = (const char*)data;
    self->mpBlockHeader = (tInSeqScriptEntityBlockHeader*)data;
    self->mpOwner = (cScriptPlayerMain*)size;
    self->mpVariationTable = (unsigned int*)(data + 8);
    self->mCurrVariation = 0;
    char* rec = data + *(int*)(data + 8);
    self->mpSystemHeader = (tInSeqEntityHeader*)rec;
    if (*(unsigned short*)rec > 8) {
        self->mpHeader = (const char*)(rec + 8);
    } else {
        self->mpHeader = 0;
    }
    return *(int*)self->mpBlockHeader;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_DeInit);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_Stop(cScriptPlayerEntity* self);

/**
 * Unbinds the entity player from its script data, stopping it first.
 *
 * Only acts when mpData is set: calls cScriptPlayerEntity_Stop(), then clears the data, owner, user data,
 * header and variation fields and the three setup flags (mSetupScriptHasKeysLeft,
 * mSetupScriptInitialEventBits, mWaitingPlayExec).
 */
extern "C" void cScriptPlayerEntity_DeInit(cScriptPlayerEntity* self)
{
    if ((int)self->mpData != 0) {
        cScriptPlayerEntity_Stop(self);
        self->mpData = 0;
        self->mpOwner = 0;
        self->mpUserData = 0;
        self->mpBlockHeader = 0;
        self->mpVariationTable = 0;
        self->mCurrVariation = 0;
        self->mpSystemHeader = 0;
        self->mpHeader = 0;
        self->mSetupScriptHasKeysLeft = 0;
        self->mSetupScriptInitialEventBits = 0;
        self->mWaitingPlayExec = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_SetEntityVariation);
#ifdef SKIP_ASM
/**
 * Selects which variation of the entity's script is played.
 *
 * Stores @p idx in mCurrVariation and points mpSystemHeader at mpData + mpVariationTable[idx]; mpHeader is
 * the variation's custom header when its headerSize is above 8, else 0. The index is not range-checked.
 *
 * @param idx  variation index (On Tour varIndex)
 */
extern "C" void cScriptPlayerEntity_SetEntityVariation(cScriptPlayerEntity* self, int idx)
{
    self->mCurrVariation = idx;
    char* rec = (char*)self->mpData + ((int*)self->mpVariationTable)[idx];
    self->mpSystemHeader = (tInSeqEntityHeader*)rec;
    if (*(unsigned short*)rec > 8) {
        self->mpHeader = (const char*)(rec + 8);
    } else {
        self->mpHeader = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", func_00273AA8);
#ifdef SKIP_ASM
extern "C" void* func_00272308(void* heap, int size);
// PORT: real arity is (size, heap); the unit declares func_00272488(void*) for its callers.
extern "C" void* func_00272488_impl(int size, void* heap) __asm__("func_00272488");
extern "C" void* cScriptPlayerChannelSpline_cScriptPlayerChannelSpline(cScriptPlayerChannelSpline* self);
extern "C" void* cScriptPlayerChannelBlock_cScriptPlayerChannelBlock(cScriptPlayerChannelBlock* self);
extern "C" void* cScriptPlayerChannelEvent_cScriptPlayerChannelEvent(cScriptPlayerChannelEvent* self);
extern "C" void* cScriptPlayerChannelObject_cScriptPlayerChannelObject(cScriptPlayerChannelObject* self);
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self);

struct sRcVE_273AA8 {
    short delta;
    short index;
    int (*fn)(void*, void*, void*, int*);
};

struct sRcVE2_273AA8 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sRcVE3_273AA8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sRcHdr_273AA8 {
    unsigned short first;   // 0x0
    unsigned char count;    // 0x2
    unsigned char kind;     // 0x3
};

struct sRcRec_273AA8 {
    unsigned short type;    // 0x0
    unsigned short size;    // 0x2
};

struct sRcObj_273AA8 {
    int f0;
    sRcVE_273AA8* vt;       // 0x4
};

struct sRcCache_273AA8 {
    char pad0[0x8];
    char* owner;            // 0x8
    char padC[0x1C - 0xC];
    sRcHdr_273AA8* hdr;     // 0x1C
    char pad20[0x24 - 0x20];
    sRcObj_273AA8** objs;   // 0x24
    int ready;              // 0x28
    char* player;           // 0x2C
    int done;               // 0x30
    int flags;              // 0x34
    int started;            // 0x38
};

extern "C" int func_00273AA8(sRcCache_273AA8* self)
{
    self->done = 0;
    self->flags = 0;
    if (self->hdr->count) {
        void* heap = *(char**)(self->owner + 8) + 8;
        self->objs = (sRcObj_273AA8**)func_00272308(heap, self->hdr->count * 4);
        char* p = (char*)self->hdr + self->hdr->first;
        for (int i = 0; i < self->hdr->count; i++) {
            sRcRec_273AA8* e = (sRcRec_273AA8*)p;
            p += 4;
            switch (e->type) {
            case 0: {
                sRcObj_273AA8** slot = (sRcObj_273AA8**)((i << 2) + (int)self->objs);
                *slot = (sRcObj_273AA8*)cScriptPlayerChannelSpline_cScriptPlayerChannelSpline((cScriptPlayerChannelSpline*)func_00272488_impl(0x28, heap));
                break;
            }
            case 1: {
                sRcObj_273AA8** slot = (sRcObj_273AA8**)((i << 2) + (int)self->objs);
                *slot = (sRcObj_273AA8*)cScriptPlayerChannelObject_cScriptPlayerChannelObject((cScriptPlayerChannelObject*)func_00272488_impl(0x28, heap));
                break;
            }
            case 2: {
                sRcObj_273AA8** slot = (sRcObj_273AA8**)((i << 2) + (int)self->objs);
                *slot = (sRcObj_273AA8*)cScriptPlayerChannelBlock_cScriptPlayerChannelBlock((cScriptPlayerChannelBlock*)func_00272488_impl(0x2C, heap));
                break;
            }
            case 3: {
                sRcObj_273AA8** slot = (sRcObj_273AA8**)((i << 2) + (int)self->objs);
                *slot = (sRcObj_273AA8*)cScriptPlayerChannelEvent_cScriptPlayerChannelEvent((cScriptPlayerChannelEvent*)func_00272488_impl(0x28, heap));
                break;
            }
            }
            int flag = 0;
            int* pd = &self->done;
            int r = 0;
            sRcObj_273AA8* o = self->objs[i];
            if (o->vt[1].fn((char*)o + o->vt[1].delta, p, self, &flag) || self->done)
                r = 1;
            *pd = r;
            self->flags |= flag;
            p += e->size;
        }
    }
    self->ready = 1;
    char* x = (char*)cScriptPlayerMain_GetInSeqCommInterface((cScriptPlayerMain*)self->owner);
    if (x) {
        sRcVE2_273AA8* vt = *(sRcVE2_273AA8**)x;
        self->player = (char*)vt[6].fn(x + vt[6].delta, self, self->hdr->kind);
    }
    if (self->player) {
        char* pl = self->player;
        sRcVE3_273AA8* vt = *(sRcVE3_273AA8**)pl;
        vt[1].fn(pl + vt[1].delta, self, self->hdr->kind);
    }
    self->started = 1;
    return self->ready;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_ExecutePlay);
#ifdef SKIP_ASM
class cRcListener3D20 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03(void* src, int id, int h, float t);
    virtual void v04(void* src, int id, int h, float t0, float t1);
};

/**
 * Finishes a pending play: reports the setup's initial events to the entity comm interface and clears the
 * setup state.
 *
 * With an entity comm interface (mpEntityComm) it calls OnInSeqKeyEvent (slot 3) when
 * mSetupScriptInitialEventBits is nonzero, then OnInSeqUpdate (slot 4), with the entity type and frame 0.
 * Then clears mSetupScriptHasKeysLeft, mSetupScriptInitialEventBits and mWaitingPlayExec.
 *
 * @return mSetupScriptHasKeysLeft as it was: nonzero when a channel still has keys to play
 */
extern "C" int cScriptPlayerEntity_ExecutePlay(cScriptPlayerEntity* self)
{
    cRcListener3D20* obj = (cRcListener3D20*)self->mpEntityComm;
    int ret = self->mSetupScriptHasKeysLeft;
    if (obj != 0) {
        int h = self->mSetupScriptInitialEventBits;
        if (h != 0) {
            obj->v03(self, *(unsigned char*)((char*)self->mpSystemHeader + 3), h, 0.0f);
        }
        ((cRcListener3D20*)self->mpEntityComm)->v04(self,
            *(unsigned char*)((char*)self->mpSystemHeader + 3),
            self->mSetupScriptInitialEventBits, 0.0f, 0.0f);
    }
    self->mSetupScriptHasKeysLeft = 0;
    self->mSetupScriptInitialEventBits = 0;
    self->mWaitingPlayExec = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_Stop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_002724E8_impl(void* p) __asm__("func_002724E8");
extern "C" void func_002723A8(cScriptHeap* h, void* p);

class cRcObj_3DC0 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int v07();
    virtual void v08();
    virtual void v09();
    virtual void v10(int);
};

class cRcListener_3DC0 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* src, int id);
    virtual void v02(void* src, int id);
};

/**
 * Stops the entity's playback: releases its comm interface and deletes all of its channel players.
 *
 * Does nothing unless mIsPlaying. Calls the comm interface's OnInSeqRelease (slot 2) and clears mpEntityComm;
 * each channel then gets Stop (slot 2) and is deleted by type: spline channels (no destructor) through
 * func_002724E8, the others through their virtual destructor (slot 10, flags 3). The channel array goes back
 * to the owner's cInSeq heap (func_002723A8) and mIsPlaying is cleared.
 */
extern "C" void cScriptPlayerEntity_Stop(cScriptPlayerEntity* self)
{
    if (self->mIsPlaying == 0)
        return;
    cRcListener_3DC0* l = (cRcListener_3DC0*)self->mpEntityComm;
    if (l != 0)
        l->v02(self, *(unsigned char*)((char*)self->mpSystemHeader + 3));
    self->mpEntityComm = 0;
    if (*(unsigned char*)((char*)self->mpSystemHeader + 2) != 0)
    {
        for (int i = 0; i < *(unsigned char*)((char*)self->mpSystemHeader + 2); i++)
        {
            cRcObj_3DC0** objs = (cRcObj_3DC0**)self->mpChannelPlayers;
            objs[i]->v02();
            switch (((cRcObj_3DC0**)self->mpChannelPlayers)[i]->v07())
            {
            case 0:
                func_002724E8_impl(((cRcObj_3DC0**)self->mpChannelPlayers)[i]);
                break;
            case 1:
            {
                cRcObj_3DC0* o = ((cRcObj_3DC0**)self->mpChannelPlayers)[i];
                if (o)
                    o->v10(3);
                break;
            }
            case 2:
            {
                cRcObj_3DC0* o = ((cRcObj_3DC0**)self->mpChannelPlayers)[i];
                if (o)
                    o->v10(3);
                break;
            }
            case 3:
            {
                cRcObj_3DC0* o = ((cRcObj_3DC0**)self->mpChannelPlayers)[i];
                if (o)
                    o->v10(3);
                break;
            }
            }
        }
        func_002723A8((cScriptHeap*)((sRcHeap*)(*(char**)((char*)self->mpOwner + 0x8) + 8)), self->mpChannelPlayers);
        self->mpChannelPlayers = 0;
    }
    self->mIsPlaying = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_Restart);
#ifdef SKIP_ASM
class cRcObj_3F68 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual int v01(void* data, void* cache, int* mask);
    virtual void v02();
};

/**
 * Restarts every channel of a playing entity from the start of its data and reports the initial events.
 *
 * Walks the channel records after the variation header (4-byte record header, size at +2): each channel gets
 * Stop (slot 2), then Play(record + 4, self, &bits) (slot 1), and the event bits are ORed. With an entity
 * comm interface it then calls OnInSeqKeyEvent (slot 3, only when bits were raised) and OnInSeqUpdate (slot
 * 4) at frame 0.
 *
 * @return nonzero when any channel's Play returned nonzero (still has keys to play); 0 when not playing
 */
extern "C" int cScriptPlayerEntity_Restart(cScriptPlayerEntity* self)
{
    if (self->mIsPlaying == 0)
        return 0;
    unsigned char* hdr = (unsigned char*)self->mpSystemHeader;
    int mask = 0;
    int i = 0;
    int changed = 0;
    unsigned char* p = hdr + *(unsigned short*)hdr;
    if (hdr[2] != 0)
    {
        do
        {
            unsigned char* rec = p;
            int ch = 0;
            p += 4;
            ((cRcObj_3F68**)self->mpChannelPlayers)[i]->v02();
            int m = 0;
            if (((cRcObj_3F68**)self->mpChannelPlayers)[i]->v01(p, self, &m) != 0 || changed)
                ch = 1;
            i++;
            p += *(unsigned short*)(rec + 2);
            changed = ch;
            mask |= m;
        } while (i < ((unsigned char*)self->mpSystemHeader)[2]);
    }
    cRcListener3D20* l = (cRcListener3D20*)self->mpEntityComm;
    if (l == 0)
        return changed;
    if (mask)
        l->v03(self, *(unsigned char*)((char*)self->mpSystemHeader + 3), mask, 0.0f);
    ((cRcListener3D20*)self->mpEntityComm)->v04(self, *(unsigned char*)((char*)self->mpSystemHeader + 3), mask, 0.0f, 0.0f);
    return changed;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274100);
#ifdef SKIP_ASM
extern "C" int cScriptPlayerEntity_DoFrameUpdate(cScriptPlayerEntity* self, float t, float dt);

class cChan_274100 {
public:
    int f0;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual int nextTime();
};



// Plays the track from prev to time; when jumping, stops at every channel event in between.
extern "C" int func_00274100(cScriptPlayerEntity* self_, float prev, float time, float speed, int jump)
{
    cScriptPlayerEntity* self = self_;
    if (self->mIsPlaying == 0) {
        return 1;
    }
    if (jump) {
        for (;;) {
            float next = time;
            for (int i = 0; i < (self->mpSystemHeader)->numChannels; i++) {
                float f = (float)((cChan_274100**)self->mpChannelPlayers)[i]->nextTime();
                if (f >= 0.0f && f < next) {
                    next = f;
                }
            }
            if (next == time) {
                return cScriptPlayerEntity_DoFrameUpdate((cScriptPlayerEntity*)self, time, time - prev);
            }
            float d = next - prev;
            prev = next;
            if (!cScriptPlayerEntity_DoFrameUpdate((cScriptPlayerEntity*)self, prev, d)) {
                return 0;
            }
        }
    }
    return cScriptPlayerEntity_DoFrameUpdate((cScriptPlayerEntity*)self, time, speed);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274230);
#ifdef SKIP_ASM
extern "C" int func_00274230(void *arg0, int arg1) {
    int temp_2;

    temp_2 = (*(int *)((char*)(arg0) + (0xC)));
    (*(int *)((char*)(arg0) + (0xC))) = arg1;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_SetEntityComm);
#ifdef SKIP_ASM
class cRcListener4240 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* src, int id);
    virtual void v02(void* src, int id);
};

/**
 * Replaces the entity's comm interface, releasing the old one and locking the new one.
 *
 * Calls OnInSeqRelease (slot 2) on the old interface and OnInSeqLock (slot 1) on the new one, each with the
 * entity and its type (mpSystemHeader->type).
 *
 * @param pEntityComm  the new entity comm interface (On Tour cInSeqCommEntity*), or 0 for none
 * @return the previous interface
 */
extern "C" cRcListener4240* cScriptPlayerEntity_SetEntityComm(cScriptPlayerEntity* self, cRcListener4240* pEntityComm)
{
    cRcListener4240* old = (cRcListener4240*)self->mpEntityComm;
    if (old != 0) {
        old->v02(self, *(unsigned char*)((char*)self->mpSystemHeader + 3));
    }
    self->mpEntityComm = (cInSeqCommEntity*)pEntityComm;
    if (pEntityComm != 0) {
        pEntityComm->v01(self, *(unsigned char*)((char*)self->mpSystemHeader + 3));
    }
    return old;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002742C8__FPvi);
#ifdef SKIP_ASM
int func_002742C8(void* self, int a1)
{
    return *(int*)((char*)*(void**)((char*)self + 0x24) + a1 * 4);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002742E0);
#ifdef SKIP_ASM
extern "C" void* func_002742E0(cScriptPlayerEntity* self, int i)
{
    int* table = (int*)self->mpVariationTable;
    char* p = (char*)self->mpData + table[i];
    if (*(unsigned short*)p > 8) {
        return p + 8;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274318);
#ifdef SKIP_ASM
extern "C" void* func_00274318(char* base, int idx)
{
    char* p = base + *(int*)(base + (idx << 2) + 8);
    if (*(unsigned short*)p > 8)
        return p + 8;
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274348__FPv);
#ifdef SKIP_ASM
int func_00274348(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x1c) + 0x4);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274358);
#ifdef SKIP_ASM
extern "C" int func_00274358(cScriptPlayerEntity* self)
{
    char* base = (char*)self->mpData;
    int* tab = (int*)self->mpVariationTable;
    int i = self->mCurrVariation;
    return *(int*)(base + tab[i] + 4);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00274380);
#ifdef SKIP_ASM
extern "C" int func_00274380(cScriptPlayerEntity* scriptPlayerEntity) {
    return (scriptPlayerEntity->mCurrVariation);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274388__FPv);
#ifdef SKIP_ASM
int func_00274388(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x10) + 0x4);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274398);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00274398_impl(void *arg0) __asm__("func_00274398");
extern "C" int func_00274398_impl(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_002743A0);
#ifdef SKIP_ASM
extern "C" int func_002743A0(cScriptPlayerEntity* scriptPlayerEntity) {
    return (*(int *)((char*)((*(void **)((char*)((scriptPlayerEntity->mpOwner)) + (0x10)))) + (4)));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_002743B0);
#ifdef SKIP_ASM
extern "C" int func_002743B0(cScriptPlayerEntity* scriptPlayerEntity) {
    return *((int*)scriptPlayerEntity->mpBlockHeader);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_002743C0);
#ifdef SKIP_ASM
extern "C" int func_002743C0(int *arg0) {
    return *arg0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002743C8__FPv);
#ifdef SKIP_ASM
unsigned char func_002743C8(void* self)
{
    return *(unsigned char*)((char*)*(void**)((char*)self + 0x1c) + 0x3);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002743D8);
#ifdef SKIP_ASM
extern "C" unsigned char func_002743D8(void *arg0) {
    return (*(unsigned char *)(((char*)(arg0) + (*(int *)((char*)(arg0) + (8)))) + (3)));
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_DoFrameUpdate);
#ifdef SKIP_ASM
class cA2743E8 {
public:
    int pad0;
    virtual void a1();
    virtual void a2();
    virtual bool a3(float t, float dt, int* out);
};
struct sSelf2743E8;
class cB2743E8 {
public:
    virtual void b1();
    virtual void b2();
    virtual void b3(cScriptPlayerEntity* s, float t, unsigned char b, int flags);
    virtual void b4(cScriptPlayerEntity* s, float t, float dt, unsigned char b, int flags);
};

/**
 * Advances every channel of the entity to a frame and passes the resulting events to the entity comm
 * interface.
 *
 * Calls each channel's Update (slot 3) with @p currFrameNum and @p dt, ORing the results and the event bits
 * they raise. With an entity comm interface it then calls OnInSeqKeyEvent (slot 3, only when bits were
 * raised) and OnInSeqUpdate (slot 4) with the frame, the step and the entity type.
 *
 * @param p             the entity player (this)
 * @param currFrameNum  script frame to update to
 * @param dt            frame step since the last update (On Tour frameDelta)
 * @return nonzero while any channel is still playing (its Update returned true)
 */
extern "C" int cScriptPlayerEntity_DoFrameUpdate(cScriptPlayerEntity* p, float currFrameNum, float dt)
{
    cScriptPlayerEntity* self = p;
    int flags = 0;
    bool any = false;
    for (int i = 0; i < (self->mpSystemHeader)->numChannels; i++) {
        int out = 0;
        any = ((cA2743E8**)self->mpChannelPlayers)[i]->a3(currFrameNum, dt, &out) | any;
        flags |= out;
    }
    cB2743E8* s = ((cB2743E8*)self->mpEntityComm);
    if (s != 0) {
        if (flags != 0) {
            s->b3(self, currFrameNum, (self->mpSystemHeader)->type, flags);
        }
        ((cB2743E8*)self->mpEntityComm)->b4(self, currFrameNum, dt, (self->mpSystemHeader)->type, flags);
    }
    return any;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_cScriptPlayerMain);
#ifdef SKIP_ASM
/**
 * Constructs a stopped InSeq script player with no script loaded.
 *
 * mID, mScriptLength and mJumpFrame start at -1 and mPlaySpeed at 1.0; every other field (data, manager,
 * entity players, play state, frame, pause count, update flags, preview interface) is 0.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerMain_cScriptPlayerMain(cScriptPlayerMain* self)
{
    self->mID = -1;
    self->mpData = 0;
    self->mpManager = 0;
    self->mpUserData = 0;
    self->mpSystemHeader = 0;
    self->mpDataHeader = 0;
    self->mpEntityPlayers = 0;
    self->mLoopScript = 0;
    self->mPlayState = 0;
    self->mPlaySpeed = 1.0f;
    *(int*)&self->mCurrFrame = 0;
    self->mNumPaused = 0;
    self->mScriptLength = -1;
    self->mJumpFrame = -1;
    self->mNumSteps = 0;
    self->mEntityUpdateInProgress = 0;
    self->mDoStopInEntityUpdate = 0;
    self->mpPreviewCommInterface = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274578);
#ifdef SKIP_ASM
extern "C" void func_00274578(int* self, int flags)
{
    func_002746F8((cScriptPlayerMain*)self);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002745C0);
#ifdef SKIP_ASM
// PORT: array new[] with a heap argument; same symbol as the unit's func_002724C8(void*).
void* operator new[](unsigned int size, void* heap) __asm__("func_002724C8__FPv");

struct sRcElem45C0 {
    char pad_0x0[0x3C];
    sRcElem45C0() { cScriptPlayerEntity_cScriptPlayerEntity((cScriptPlayerEntity*)(this)); }
    void operator delete[](void* p, unsigned int size);
};

struct sRcHdr45C0 {
    unsigned char field_0x0;
    unsigned char count;    // 0x1
    unsigned short size;    // 0x2
    int field_0x4;
    int result;             // 0x8
};

struct sRcStream45C0 {
    int field_0x0;          // 0x0
    sRcHdr45C0* data;       // 0x4
    char* owner;            // 0x8
    char pad_0xC[0x4];
    sRcHdr45C0* hdr;        // 0x10
    char* ext;              // 0x14
    sRcElem45C0* elems;     // 0x18
    int field_0x1C;         // 0x1C
    char pad_0x20[0x14];
    int field_0x34;         // 0x34
    int field_0x38;         // 0x38
};

// PORT: cScriptPlayerEntity_Init's third argument receives the owning stream pointer as an int.
// PORT: the unit declares func_002745C0 as returning void*; it returns the header word.
extern "C" int func_002745C0_impl(sRcStream45C0* self, sRcHdr45C0* data, char* owner, int a3) __asm__("func_002745C0");
extern "C" int func_002745C0_impl(sRcStream45C0* self, sRcHdr45C0* data, char* owner, int a3)
{
    self->field_0x0 = a3;
    self->owner = owner;
    self->data = data;
    self->hdr = data;
    self->field_0x34 = -1;
    self->field_0x38 = 0;
    self->field_0x1C = 0;
    if (data->size >= 0xD) {
        self->ext = (char*)data + 0xC;
    }
    char* p = (char*)data + self->hdr->size;
    if (self->hdr->count != 0) {
        sRcElem45C0** slot = &self->elems;
        *slot = new (owner + 8) sRcElem45C0[self->hdr->count];
        for (int i = 0; i < self->hdr->count; i++) {
            p += cScriptPlayerEntity_Init((cScriptPlayerEntity*)(&self->elems[i]), p, (int)self);
        }
    }
    return self->hdr->result;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002746F8);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity__cScriptPlayerEntity(cScriptPlayerEntity* self, int flags);
void* func_00272510(void* self);




extern "C" void func_002746F8(cScriptPlayerMain* scriptPlayerMain)
{
    cScriptPlayerMain* self = scriptPlayerMain;
    if (((void*)self->mpData) == 0) {
        return;
    }
    cScriptPlayerMain_Stop((cScriptPlayerMain*)self);
    for (int i = 0; i < (self->mpSystemHeader)->numEntities; i++) {
        cScriptPlayerEntity_DeInit((cScriptPlayerEntity*)(&(self->mpEntityPlayers)[i]));
    }
    // delete[] self->elems (element count in the 16-byte array cookie)
    cScriptPlayerEntity* scriptPlayerEntity = (self->mpEntityPlayers);
    if (scriptPlayerEntity != 0) {
        cScriptPlayerEntity* q = scriptPlayerEntity + *(int*)((char*)scriptPlayerEntity - 0x10);
        while ((self->mpEntityPlayers) != q) {
            q--;
            cScriptPlayerEntity__cScriptPlayerEntity((cScriptPlayerEntity*)q, 0);
        }
        func_00272510((char*)(self->mpEntityPlayers) - 0x10);
    }
    self->mNumSteps = 0;
    self->mID = -1;
    (*(void**)&self->mpData) = 0;
    (*(void**)&self->mpManager) = 0;
    (*(int*)&self->mpUserData) = 0;
    (*(tInSeqScriptHeader**)&self->mpSystemHeader) = 0;
    (*(char**)&self->mpDataHeader) = 0;
    (*(cScriptPlayerEntity**)&self->mpEntityPlayers) = 0;
    self->mLoopScript = 0;
    self->mPlayState = 0;
    (*(int*)&self->mCurrFrame) = 0;
    self->mNumPaused = 0;
    self->mScriptLength = -1;
    self->mPlaySpeed = 1.0f;
    self->mJumpFrame = -1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274808__FPvf);
#ifdef SKIP_ASM
void func_00274808(void* self, float val)
{
    *(float*)((char*)self + 0x24) = val;
}
#endif

extern "C" void* func_00274830(void*, int);

//100%
INCLUDE_ASM("replay/replaycache", func_00274810__FPv);
#ifdef SKIP_ASM
void* func_00274810(void* self)
{
    return func_00274830(self, *(int*)((char*)self + 0x1c));
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274830);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// the unit declares this as returning void*; the body returns nothing,
// so bind a void body to the symbol
void func_00274830_impl(void* self, int a1) __asm__("func_00274830");

void func_00274830_impl(void* self, int a1)
{
    if (*(int*)((char*)self + 0x20) != 0) {
        if (*(int*)((char*)self + 0x3c) != 0) {
            *(int*)((char*)self + 0x40) = 0;
        }
    } else {
        *(int*)((char*)self + 0x1c) = a1;
        *(int*)((char*)self + 0x20) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274860);
#ifdef SKIP_ASM
extern "C" void func_00274860(cScriptPlayerMain* scriptPlayerMain, int arg1) {
    (scriptPlayerMain->mLoopScript) = arg1;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Preview);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00274830_v(void*, int) __asm__("func_00274830");
extern "C" void func_00274A30(cScriptPlayerMain* p);

/**
 * Runs the whole script at once through a preview comm interface, then restores the player's speed and loop
 * setting.
 *
 * Sets mpPreviewCommInterface, starts playback without looping (func_00274830, which only starts a stopped
 * player) and calls the frame update func_00274A30 at speed 1.0 until the play state is back to stopped; then
 * restores mPlaySpeed and mLoopScript and clears the preview interface.
 *
 * @param flag  despite the name, the comm interface to preview through (On Tour pPreviewCommInterface)
 */
extern "C" void cScriptPlayerMain_Preview(cScriptPlayerMain* self, int flag)
{
    float savedScale = self->mPlaySpeed;
    self->mpPreviewCommInterface = (cInSeqComm*)flag;
    self->mPlaySpeed = 1.0f;
    int savedIdx = self->mLoopScript;
    func_00274830_v(((char*)self), 0);
    while (self->mPlayState != 0) {
        self->mPlaySpeed = 1.0f;
        func_00274A30(self);
    }
    self->mPlaySpeed = savedScale;
    self->mLoopScript = savedIdx;
    self->mpPreviewCommInterface = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_002748F8);
#ifdef SKIP_ASM
extern "C" void func_002748F8(cScriptPlayerMain* scriptPlayerMain, int arg1) {
    (scriptPlayerMain->mJumpFrame) = arg1;
    (scriptPlayerMain->mNumSteps) = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00274908);
#ifdef SKIP_ASM
extern "C" void func_00274908(cScriptPlayerMain* scriptPlayerMain) {
    (scriptPlayerMain->mNumSteps) = (int) ((scriptPlayerMain->mNumSteps) + 1);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Stop);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_Stop(cScriptPlayerEntity* self);
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self);

class cRcListener4918 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(void* src);
};

/**
 * Stops the script: stops every entity and resets the play state, frame and cached length.
 *
 * Does nothing when already stopped. During an entity update (mEntityUpdateInProgress) it only sets
 * mDoStopInEntityUpdate, so the update stops afterwards. Otherwise it stops each entity
 * (cScriptPlayerEntity_Stop), zeroes mPlayState, mCurrFrame, mNumPaused and mNumSteps, sets mScriptLength and
 * mJumpFrame to -1 and calls the comm interface's slot 4 (OnInSeqStop by On Tour's order).
 */
extern "C" void cScriptPlayerMain_Stop(cScriptPlayerMain* self)
{
    if (self->mPlayState == 0) return;
    if (self->mEntityUpdateInProgress != 0) {
        self->mDoStopInEntityUpdate = 1;
        return;
    }
    for (int i = 0; i < *(unsigned char*)((char*)self->mpSystemHeader + 1); i++) {
        cScriptPlayerEntity_Stop((cScriptPlayerEntity*)((char*)self->mpEntityPlayers + i * 0x3C));
    }
    self->mPlayState = 0;
    *(int*)&self->mCurrFrame = 0;
    self->mNumPaused = 0;
    self->mNumSteps = 0;
    self->mScriptLength = -1;
    self->mJumpFrame = -1;
    cRcListener4918* l = (cRcListener4918*)cScriptPlayerMain_GetInSeqCommInterface(self);
    if (l != 0) {
        l->v04(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Pause);
#ifdef SKIP_ASM
/**
 * Pauses a playing script by raising its pause count (mNumPaused).
 *
 * Pauses nest: each needs a matching cScriptPlayerMain_Resume(). Nothing happens while stopped.
 */
extern "C" void cScriptPlayerMain_Pause(cScriptPlayerMain* self)
{
    if (self->mPlayState != 0) {
        self->mNumPaused += 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Resume);
#ifdef SKIP_ASM
/**
 * Undoes one pause of a playing script: lowers mNumPaused, not below 0.
 *
 * Nothing happens while stopped.
 */
extern "C" void cScriptPlayerMain_Resume(cScriptPlayerMain* self)
{
    if (self->mPlayState != 0) {
        int v = self->mNumPaused - 1;
        self->mNumPaused = v;
        if (v < 0) {
            self->mNumPaused = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", func_00274A30);
#ifdef SKIP_ASM
extern "C" int cScriptPlayerMain_GetScriptLength(cScriptPlayerMain* self);
extern "C" void cScriptPlayerMain_Stop(cScriptPlayerMain* p);
void* func_00274810(void* self);
extern "C" void func_00274DE0(void* self);
extern "C" void cScriptPlayerMain_Pause(cScriptPlayerMain* self);


extern "C" void func_00274A30(cScriptPlayerMain* scriptPlayerMain)
{
    cScriptPlayerMain* self = scriptPlayerMain;
    int need = 1;
    if (self->mJumpFrame >= 0)
    {
        int target = self->mJumpFrame;
        self->mJumpFrame = -1;
        int len = cScriptPlayerMain_GetScriptLength((cScriptPlayerMain*)self);
        int saved = self->mNumPaused;
        self->mNumPaused = 0;
        if (self->mLoopScript != 0)
            target = target % len;
        float ft = (float)target;
        if (self->mPlayState == 1)
            func_00274DE0(self);
        if (ft != self->mCurrFrame)
        {
            if (ft < self->mCurrFrame)
            {
                cScriptPlayerMain_Stop((cScriptPlayerMain*)self);
                func_00274810(self);
                func_00274DE0(self);
            }
            float one = 1.0f;
            float pos = self->mCurrFrame;
            float frac = (float)((int)pos + 1) - pos;
            float speed = self->mPlaySpeed;
            if (frac < one)
            {
                self->mPlaySpeed = frac;
                func_00274DE0(self);
            }
            self->mPlaySpeed = one;
            while (self->mPlayState != 0 && self->mCurrFrame < ft)
                func_00274DE0(self);
            self->mPlaySpeed = speed;
        }
        if (self->mPlayState != 0)
            self->mNumPaused = saved;
        need = 0;
    }
    if (self->mNumSteps != 0)
    {
        int steps = self->mNumSteps;
        int saved2 = self->mNumPaused;
        self->mNumSteps = 0;
        self->mNumPaused = 0;
        if (self->mPlayState != 0)
        {
            do
            {
                func_00274DE0(self);
                steps--;
                if (self->mPlayState == 0)
                    goto skip;
            } while (steps != 0);
            self->mNumPaused = saved2;
        }
    skip:
        if (self->mNumPaused <= 0)
            cScriptPlayerMain_Pause((cScriptPlayerMain*)self);
        need = 0;
    }
    if (need)
        func_00274DE0(self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274C10__FPvi);
#ifdef SKIP_ASM
int func_00274C10(void* self, int a1)
{
    int old = *(int*)((char*)self + 0xc);
    *(int*)((char*)self + 0xc) = a1;
    return old;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetEntity);
#ifdef SKIP_ASM
/**
 * Returns one of the script's entity players: mpEntityPlayers + index * 0x3C.
 *
 * @param entityIndex  entity index (On Tour entityIndex)
 * @return the entity player at that index (not range-checked)
 */
extern "C" void* cScriptPlayerMain_GetEntity(cScriptPlayerMain* self, int entityIndex)
{
    return (char*)self->mpEntityPlayers + entityIndex * 0x3c;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274C38);
#ifdef SKIP_ASM
extern "C" void* func_00274C38(void* self)
{
    if (*(unsigned short*)((char*)self + 0x2) >= 0xd) {
        return (char*)self + 0xc;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274C58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00274C58_impl(void *arg0) __asm__("func_00274C58");
extern "C" int func_00274C58_impl(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00274C60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00274C60_impl(void *arg0) __asm__("func_00274C60");
extern "C" int func_00274C60_impl(void *arg0) {
    return (*(int *)((char*)(arg0) + (8)));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00274C68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" unsigned char func_00274C68_impl(void *arg0) __asm__("func_00274C68");
extern "C" unsigned char func_00274C68_impl(void *arg0) {
    return (*(unsigned char *)((char*)(arg0) + (1)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetScriptLength);
#ifdef SKIP_ASM

/**
 * Returns the script's length in frames: the longest numFrames among its entities' current variations.
 *
 * Uses mScriptLength when it is not negative; otherwise computes the maximum with func_00274348 over the
 * entities and caches it only while the script is playing.
 *
 * @return the length in frames
 */
extern "C" int cScriptPlayerMain_GetScriptLength(cScriptPlayerMain* self)
{
    int r = self->mScriptLength;
    if (r < 0) {
        int best = 0;
        int i;
        for (i = 0; i < *(unsigned char*)((char*)self->mpSystemHeader + 1); i++) {
            int v = func_00274348(&(self->mpEntityPlayers)[i]);
            if (best < v) {
                best = v;
            }
        }
        if (self->mPlayState != 0) {
            self->mScriptLength = best;
        }
        r = best;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetNumUpdatesToEndLeft);
#ifdef SKIP_ASM
extern "C" int cScriptPlayerMain_GetScriptLength(cScriptPlayerMain* self);

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ftrunc_00274D08(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

/**
 * Returns how many more updates the script needs to reach its end at the current play speed.
 *
 * Computes (cScriptPlayerMain_GetScriptLength() - mCurrFrame) / mPlaySpeed and rounds it up.
 *
 * @return the number of updates left, rounded up
 */
extern "C" int cScriptPlayerMain_GetNumUpdatesToEndLeft(cScriptPlayerMain* self)
{
    float x = ((float)cScriptPlayerMain_GetScriptLength(self) - self->mCurrFrame) / self->mPlaySpeed;
    float c = ftrunc_00274D08(x);
    if (c < x) {
        c += 1.0f;
    }
    return (int)c;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetInSeqCommInterface);
#ifdef SKIP_ASM
/**
 * Returns the comm interface the script reports to: the preview interface when set, else the manager's.
 *
 * The manager's interface is the first word of the cInSeq (mpManager + 0x0).
 *
 * @return the cInSeqComm interface, as an int
 */
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self)
{
    int v = (int)self->mpPreviewCommInterface;
    if (v != 0) {
        return v;
    }
    return *(int*)self->mpManager;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274D90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// LANDING: unit declares `extern "C" int func_00274D90();` at line ~1887 (func_00272D40 caller); alias used here.
extern "C" char* func_00274D90_impl(char* self, int count) __asm__("func_00274D90");

extern "C" char* func_00274D90_impl(char* self, int count)
{
    char* p = self + *(unsigned short*)(self + 2);
    while (count != 0) {
        count--;
        p += func_002743C0((int*)p);
    }
    return p;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274DE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Advances playback by one step: plays tracks up to the new time, handles looping and stop/finish events.
// Like func_002751A0, it collects the tracks' "changed" results; this build never reads them.
extern "C" void func_00273AA8_4DE0(void* p) __asm__("func_00273AA8");
extern "C" int cScriptPlayerEntity_ExecutePlay_4DE0(void* p) __asm__("cScriptPlayerEntity_ExecutePlay");
extern "C" int cScriptPlayerEntity_Restart_4DE0(void* p) __asm__("cScriptPlayerEntity_Restart");
extern "C" int func_002751A0_4DE0(void* self, float a, float b, float c) __asm__("func_002751A0");
extern "C" int cScriptPlayerMain_GetScriptLength_4DE0(void* self) __asm__("cScriptPlayerMain_GetScriptLength");
extern "C" void cScriptPlayerMain_Stop_4DE0(void* p) __asm__("cScriptPlayerMain_Stop");
extern "C" void* cScriptPlayerMain_GetInSeqCommInterface_p(void) __asm__("cScriptPlayerMain_GetInSeqCommInterface");

class cRcListener4DE0 {
public:
    virtual void v01(void* src);
    virtual void v02(void* src, float a, float b);
    virtual void v03(void* src);
    virtual void v04(void* src);
    virtual int v05(void* src);
};

struct sRcPlayHdr_4DE0 {
    char pad[1];
    unsigned char count;
};

extern "C" void func_00274DE0_impl(char* self) __asm__("func_00274DE0");
extern "C" void func_00274DE0_impl(char* self)
{
    if (*(int*)(self + 0x20) == 0 || *(int*)(self + 0x2C) > 0) {
        return;
    }
    cRcListener4DE0* l = (cRcListener4DE0*)cScriptPlayerMain_GetInSeqCommInterface_p();
    if (*(int*)(self + 0x20) == 1) {
        *(int*)(self + 0x20) = 2;
        if (l != 0) {
            l->v01(self);
        }
    }
    if (*(int*)(self + 0x2C) > 0 || *(int*)(self + 0x20) == 0) {
        return;
    }
    int changed = 0;
    float t = (float)cScriptPlayerMain_GetScriptLength_4DE0(self);
    int hit = 0;
    float rem = 0.0f;
    if (*(int*)(self + 0x20) == 4 && t <= *(float*)(self + 0x28)) {
        cScriptPlayerMain_Stop_4DE0(self);
        return;
    }
    if ((unsigned)(*(int*)(self + 0x20) - 3) < 2) {
        float a = *(float*)(self + 0x24);
        float b = *(float*)(self + 0x28);
        float c = a;
        float lim = b + a;
        if (*(int*)(self + 0x20) == 4) {
            lim = t;
        }
        if (t <= lim) {
            a = lim - b;
            hit = 1;
            lim = t;
            rem = c - a;
        }
        *(int*)(self + 0x3C) = 1;
        changed = func_002751A0_4DE0(self, lim, b, a);
        *(int*)(self + 0x3C) = 0;
    } else {
        if (t == 0.0f) {
            hit = 1;
        }
        *(int*)(self + 0x3C) = 1;
        for (int i = 0; i < (*(sRcPlayHdr_4DE0**)(self + 0x10))->count; i++) {
            func_00273AA8_4DE0(*(char**)(self + 0x18) + i * 0x3C);
        }
        for (int i = 0; i < (*(sRcPlayHdr_4DE0**)(self + 0x10))->count; i++) {
            int r = 0;
            if (cScriptPlayerEntity_ExecutePlay_4DE0(*(char**)(self + 0x18) + i * 0x3C) != 0 || changed != 0) {
                r = 1;
            }
            changed = r;
        }
        *(int*)(self + 0x3C) = 0;
        *(int*)(self + 0x20) = 3;
        if (l != 0) {
            l->v02(self, 0.0f, 0.0f);
        }
    }
    if (*(int*)(self + 0x40) != 0 || *(int*)(self + 0x20) == 4) {
        cScriptPlayerMain_Stop_4DE0(self);
        return;
    }
    if (hit != 0 && *(int*)(self + 0x1C) != 0) {
        int changed2 = 0;
        for (int i = 0; i < (*(sRcPlayHdr_4DE0**)(self + 0x10))->count; i++) {
            int r = 0;
            if (cScriptPlayerEntity_Restart_4DE0(*(char**)(self + 0x18) + i * 0x3C) != 0 || changed2 != 0) {
                r = 1;
            }
            changed2 = r;
        }
        *(int*)(self + 0x28) = 0;
        while (t < rem) {
            rem -= t;
        }
        if (rem != 0.0f) {
            *(int*)(self + 0x3C) = 1;
            changed2 = func_002751A0_4DE0(self, *(float*)(self + 0x28) + rem, *(float*)(self + 0x28), rem);
            *(int*)(self + 0x3C) = 0;
        }
        if (*(int*)(self + 0x40) != 0) {
            cScriptPlayerMain_Stop_4DE0(self);
        }
    } else {
        if (t <= *(float*)(self + 0x28) + *(float*)(self + 0x24)) {
            int v = *(int*)(self + 0x1C);
            if (v != 0) {
                if (l != 0) {
                    v = l->v05(self);
                }
                if (*(int*)(self + 0x20) == 0 || v != 0) {
                    return;
                }
            }
            *(int*)(self + 0x20) = 4;
            if (l != 0) {
                l->v03(self);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", func_002751A0);
#ifdef SKIP_ASM
extern "C" int func_00274100(cScriptPlayerEntity* track, float prev, float time, float speed, int jump);
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self);

class cRcListener51A0 {
public:
    virtual void v01();
    virtual void v02(void* src, float time, float speed);
};

struct sRcAnim51A0 {
    char pad_0x0[0x10];
    unsigned char* hdr;     // 0x10, hdr[1] = track count
    char pad_0x14[0x4];
    char* tracks;           // 0x18, 0x3C bytes each
    char pad_0x1C[0xC];
    float time;             // 0x28
};

extern "C" int func_002751A0(sRcAnim51A0* self, float time, float prev, float speed)
{
    int changed = 0;
    int jump = 0;
    self->time = time;
    if (speed > 1.0f) {
        jump = 1;
    } else {
        int ti = (int)time;
        int pi = (int)prev;
        if ((float)ti != time) {
            jump = pi != ti;
        }
    }
    for (int i = 0; i < self->hdr[1]; i++) {
        int c = 0;
        if (func_00274100((cScriptPlayerEntity*)(self->tracks + i * 0x3C), prev, self->time, speed, jump) != 0 || changed != 0) {
            c = 1;
        }
        changed = c;
    }
    cRcListener51A0* l = (cRcListener51A0*)cScriptPlayerMain_GetInSeqCommInterface((cScriptPlayerMain*)self);
    if (l != 0) {
        l->v02(self, self->time, speed);
    }
    return changed;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the spline channel's value storage at +0x1C.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelSpline_GetValue(void* self)
{
    return (char*)self + 0x1C;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the spline channel's custom data word at +0x20.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelSpline_GetCustom(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetNextKeyFrame__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a spline channel has no next key frame.
 *
 * @return always -1
 */
int cScriptPlayerChannelSpline_GetNextKeyFrame(void* self)
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of a spline channel.
 *
 * @return always 0 (object channels return 1, block channels 2, event channels 3)
 */
int cScriptPlayerChannelSpline_GetType(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the spline channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelSpline_GetOwner(void* self)
{
    return *(int*)((char*)self + 0xC);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the spline channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelSpline_IsPlaying(void* self)
{
    return (*(int*)((char*)self + 0x8) != 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the block channel's value storage at +0x28.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelBlock_GetValue(void* self)
{
    return (char*)self + 0x28;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the block channel's custom data word at +0x24.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelBlock_GetCustom(void* self)
{
    return *(int*)((char*)self + 0x24);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of a block channel.
 *
 * @return always 2 (spline channels return 0, object channels 1, event channels 3)
 */
int cScriptPlayerChannelBlock_GetType(void* self)
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the block channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelBlock_GetOwner(void* self)
{
    return *(int*)((char*)self + 0xC);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the block channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelBlock_IsPlaying(void* self)
{
    return (*(int*)((char*)self + 0x8) != 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the event channel's value storage at +0x20.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelEvent_GetValue(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the event channel's custom data word at +0x1C.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelEvent_GetCustom(void* self)
{
    return *(int*)((char*)self + 0x1C);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of an event channel.
 *
 * @return always 3 (spline channels return 0, object channels 1, block channels 2)
 */
int cScriptPlayerChannelEvent_GetType(void* self)
{
    return 0x3;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the event channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelEvent_GetOwner(void* self)
{
    return *(int*)((char*)self + 0xC);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the event channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelEvent_IsPlaying(void* self)
{
    return (*(int*)((char*)self + 0x8) != 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the object channel's value storage at +0x24.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelObject_GetValue(void* self)
{
    return (char*)self + 0x24;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the object channel's custom data word at +0x20.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelObject_GetCustom(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of an object channel.
 *
 * @return always 1 (spline channels return 0, block channels 2, event channels 3)
 */
int cScriptPlayerChannelObject_GetType(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the object channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelObject_GetOwner(void* self)
{
    return *(int*)((char*)self + 0xC);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the object channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelObject_IsPlaying(void* self)
{
    return (*(int*)((char*)self + 0x8) != 0);
}
#endif

