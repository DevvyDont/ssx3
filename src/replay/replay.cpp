#include "common.h"
#include "replay/frameptr.h"
#include "ai/ridermetrix.h"
#include "replay/replay.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("replay/replay", cReplay_restoreFrame);
#ifdef SKIP_ASM
class cStreamRF {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
};
class cStreamRB;
class cStreamRDB;
struct cReplayFramePtr;
extern "C" void cBucketMan_mergeBucket(void* mgr, void* bucket);
extern "C" void cBucketMan_purgeBucket(void* mgr, int bucket);
extern "C" void cWorld_resetMap(void* world);
void cReplayFramePtr_readRewind(cReplayFramePtr* p);
void AIsetseed(void* buf);
// PORT: cWorldPainterMan_reset__FPv is called without loading $a0 here.
void cWorldPainterMan_reset_0() __asm__("cWorldPainterMan_reset__FPv");
extern "C" void* cBE_getBE();
extern "C" void cBE_readFromReplayFrame(void* be, void* stream);
extern "C" void cAvalanche_readFromReplayFrame(char* stream);
extern "C" void cWScriptMan_readFromReplayFrame(void* self, void* stream);
extern "C" void func_00229E58(void* self, void* stream);
extern "C" void func_00344240(void* self, void* stream);
extern "C" void cMultiParticleMan_readFromReplayFrame(void* self, void* stream);
extern "C" void cReplay_restoreBucket(cReplay* self, void* bucket, cStreamRB* stream);
extern "C" void cReplay_restoreDeadBucket(cReplay* self, cStreamRDB* stream);
extern "C" void cFocusMan_readFromReplayFrame(void* self, void* stream);
extern "C" void cReplay_applyBucket(cReplay* self, int a);
extern "C" void cAI_updateOverlapList(void* self);
extern "C" void cGameViewMan_updateAll(void* self);
extern char gBucketMan;
extern void* cWScriptMan_TheWScriptMan;
extern void* D_004A2A00;
extern void* D_004A3FF0;
extern void* cMultiParticleMan_TheMultiParticleMan;
extern int cSortObjNode_ObjNodeSortIndex;
extern int cSortObjNode_ObjNodeNoSortIndex;

struct sVtS_DBF0 { short delta; short index; void (*fn)(void*, void*); };

/**
 * Restores the whole game state from a replay frame: world map, BE, game, AI, avalanche, scripts and the
 * object buckets.
 *
 * Clears the dynamic object buckets first, rewinds the frame stream and reads each system's part in recording
 * order, then rebuilds the AI overlap list and updates every view.
 *
 * @param stream  the frame's stream (On Tour cReplayFramePtr* framePtr)
 */
extern "C" void cReplay_restoreFrame(cReplay* self, cStreamRF* stream)
{
    void* bm = &gBucketMan;
    cBucketMan_mergeBucket(bm, (void*)1);
    cBucketMan_purgeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 8);
    cWorld_resetMap(cWorld_m_pTheWorld);
    cReplayFramePtr_readRewind((cReplayFramePtr*)stream);
    char buf[0x18];
    stream->read(buf, 0x18);
    AIsetseed(buf);
    int savedValue;
    stream->read(&savedValue, 4);
    cSortObjNode_ObjNodeSortIndex = savedValue;
    stream->read(&savedValue, 4);
    cSortObjNode_ObjNodeNoSortIndex = savedValue;
    cWorldPainterMan_reset_0();
    cBE_readFromReplayFrame(cBE_getBE(), stream);
    char* game = (char*)SSXApp->Game;
    sVtS_DBF0* vt = *(sVtS_DBF0**)game;
    vt[9].fn(game + vt[9].delta, stream);
    cAI* aiObj = SSXApp->Game->AI;
    sVtS_DBF0* vt2 = (sVtS_DBF0*)aiObj->mVtbl;
    vt2[8].fn(((char*)aiObj) + vt2[8].delta, stream);
    cAvalanche_readFromReplayFrame((char*)stream);
    cWScriptMan_readFromReplayFrame(cWScriptMan_TheWScriptMan, stream);
    func_00229E58(D_004A2A00, stream);
    func_00344240(D_004A3FF0, stream);
    cMultiParticleMan_readFromReplayFrame(cMultiParticleMan_TheMultiParticleMan, stream);
    cReplay_restoreBucket(self, (void*)1, (cStreamRB*)stream);
    cReplay_restoreDeadBucket(self, (cStreamRDB*)stream);
    cFocusMan_readFromReplayFrame(SSXApp->Game->AI->FocusMan, stream);
    cReplay_applyBucket(self, 1);
    cAI_updateOverlapList(SSXApp->Game->AI);
    cGameViewMan_updateAll(SSXApp->Game->ViewMan);
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplay_restoreBucket);
#ifdef SKIP_ASM
class cStreamRB {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
};

extern "C" void cReplay_restoreObject(cReplay* self, void* bucket, cStreamRB* stream);
extern "C" void cBucketMan_mergeBucket(void* mgr, void* bucket);
extern char gBucketMan;

/**
 * Restores one bucket of scene objects from a replay stream and merges it into the global bucket manager.
 *
 * Reads an 8-byte header whose first word is the object count, then calls cReplay_restoreObject() that many
 * times.
 *
 * @param bucket  the bucket the objects are restored into
 * @param stream  the replay stream to read from
 */
extern "C" void cReplay_restoreBucket(cReplay* self, void* bucket, cStreamRB* stream)
{
    struct {
        int count;
        int pad;
    } hdr;
    int i;
    stream->read(&hdr, 8);
    for (i = 0; i < hdr.count; i++) {
        cReplay_restoreObject(self, bucket, stream);
    }
    cBucketMan_mergeBucket(&gBucketMan, bucket);
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplay_applyBucket);
#ifdef SKIP_ASM
struct cBucketMan;
void* cBucketMan_first(cBucketMan* self, int index);
void* cBucketMan_next(cBucketMan* self, void* node, int index);
struct sBucketVEntryDE58 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Calls slot 3 (cMoveNode_apply for move nodes) on every node in one bucket of the global bucket manager.
 *
 * @param bucket  the bucket (On Tour const int bucket)
 */
// Calls virtual slot 3 on every object in the given bucket of the global bucket manager.
extern "C" void cReplay_applyBucket(cReplay* self, int bucket)
{
    void* node = cBucketMan_first((cBucketMan*)&gBucketMan, bucket);
    cBucketMan* bm = (cBucketMan*)&gBucketMan;
    while (node != 0) {
        sBucketVEntryDE58* vt = *(sBucketVEntryDE58**)((char*)node + 0xC);
        vt[3].fn((char*)node + vt[3].delta);
        node = cBucketMan_next(bm, node, bucket);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplay_restoreObject);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
class cStreamRB;
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern "C" int cReplayFramePtr_peekObjNodeType(void* stream);
extern char str_Debounce[];
extern "C" void* cDebounceNode_cDebounceNode_RO(void* self, void* bucket, void* stream) __asm__("cDebounceNode_cDebounceNode");
extern char str_AnimObject[];
extern "C" void* cAnimObjectNode_cAnimObjectNode_RO(void* self, void* bucket, void* stream) __asm__("cAnimObjectNode_cAnimObjectNode");
extern char str_AnimDelta[];
extern "C" void* cAnimDeltaNode_cAnimDeltaNode_RO(void* self, void* bucket, void* stream) __asm__("cAnimDeltaNode_cAnimDeltaNode");
extern char str_AnimCombo[];
extern "C" void* cAnimComboNode_cAnimComboNode_RO(void* self, void* bucket, void* stream) __asm__("cAnimComboNode_cAnimComboNode");
extern char str_AnimTeeter[];
extern "C" void* cAnimTeeterNode_cAnimTeeterNode_RO(void* self, void* bucket, void* stream) __asm__("cAnimTeeterNode_cAnimTeeterNode");
extern char str_FlexBridge[];
extern "C" void* cFlexBridgeNode_cFlexBridgeNode1_RO(void* self, void* bucket, void* stream) __asm__("cFlexBridgeNode_cFlexBridgeNode1");
extern char str_Counter[];
extern "C" void* cCounterNode_cCounterNode_RO(void* self, void* bucket, void* stream) __asm__("cCounterNode_cCounterNode");
extern char str_Boost[];
extern "C" void* cBoostNode_cBoostNode_RO(void* self, void* bucket, void* stream) __asm__("cBoostNode_cBoostNode");
extern char str_Fence[];
extern "C" void* cFenceNode_cFenceNode_RO(void* self, void* bucket, void* stream) __asm__("cFenceNode_cFenceNode");
extern char str_Flag[];
extern "C" void* cFlagNode_cFlagNode_RO(void* self, void* bucket, void* stream) __asm__("cFlagNode_cFlagNode");
extern char str_MeshAnim[];
extern "C" void* cMeshAnimNode_cMeshAnimNode_RO(void* self, void* bucket, void* stream) __asm__("cMeshAnimNode_cMeshAnimNode");
extern char str_Particle[];
extern "C" void* cParticleNode_cParticleNode_RO(void* self, void* bucket, void* stream) __asm__("cParticleNode_cParticleNode");
extern char str_RailMan[];
extern "C" void* cRailManNode_cRailManNode_RO(void* self, void* bucket, void* stream) __asm__("cRailManNode_cRailManNode");
extern char str_Restore[];
extern "C" void* cRestoreNode_cRestoreNode1_RO(void* self, void* bucket, void* stream) __asm__("cRestoreNode_cRestoreNode1");
extern char str_Floating[];
extern "C" void* cFloatingNode_cFloatingNode1_RO(void* self, void* bucket, void* stream) __asm__("cFloatingNode_cFloatingNode1");
extern char str_Conveyor[];
extern "C" void* cConveyorNode_cConveyorNode1_RO(void* self, void* bucket, void* stream) __asm__("cConveyorNode_cConveyorNode1");
extern char str_Hide[];
extern "C" void* cHideNode_cHideNode1_RO(void* self, void* bucket, void* stream) __asm__("cHideNode_cHideNode1");
extern char str_Object[];
extern "C" void* cObjectNode_cObjectNode1_RO(void* self, void* bucket, void* stream) __asm__("cObjectNode_cObjectNode1");
extern char str_FloatRail[];
extern "C" void* cFloatRailNode_cFloatRailNode1_RO(void* self, void* bucket, void* stream) __asm__("cFloatRailNode_cFloatRailNode1");
extern char str_SpringRail[];
extern "C" void* cSpringRailNode_cSpringRailNode1_RO(void* self, void* bucket, void* stream) __asm__("cSpringRailNode_cSpringRailNode1");
extern char str_DeadFade[];
struct sVE_RO { short delta; short index; void (*fn)(void*, int); };
struct sObj_RO { int pad[3]; sVE_RO* vt; };
extern "C" sObj_RO* cDeadFadeNode_cDeadFadeNode1_RO(void* self, void* bucket, void* stream) __asm__("cDeadFadeNode_cDeadFadeNode1");

/**
 * Recreates one recorded world object of the stream's next node type in a bucket.
 *
 * Types 6 and 12 have no case and are not read. Type 22 is constructed and then deleted at once (virtual slot
 * 1
 * with flags 3), which looks like a leftover.
 *
 * @param bucket  the bucket to link the object into
 * @param stream  the frame's stream, positioned at the object (On Tour cReplayFramePtr* data)
 */
// Recreates a replay object of the recorded type and links it into the bucket.
extern "C" void cReplay_restoreObject(cReplay* self, void* bucket, cStreamRB* stream)
{
    switch (cReplayFramePtr_peekObjNodeType(stream)) {
    case 0:
        cDebounceNode_cDebounceNode_RO(cMemMan_alloc(52, str_Debounce, 0x20000000, 0), bucket, stream);
        break;
    case 1:
        cAnimObjectNode_cAnimObjectNode_RO(cMemMan_alloc(108, str_AnimObject, 0x20000000, 0), bucket, stream);
        break;
    case 2:
        cAnimDeltaNode_cAnimDeltaNode_RO(cMemMan_alloc(0x70, str_AnimDelta, 0x20000000, 0), bucket, stream);
        break;
    case 3:
        cAnimComboNode_cAnimComboNode_RO(cMemMan_alloc(136, str_AnimCombo, 0x20000000, 0), bucket, stream);
        break;
    case 4:
        cAnimTeeterNode_cAnimTeeterNode_RO(cMemMan_alloc(0x80, str_AnimTeeter, 0x20000000, 0), bucket, stream);
        break;
    case 5:
        cFlexBridgeNode_cFlexBridgeNode1_RO(cMemMan_alloc(176, str_FlexBridge, 0x20000000, 0), bucket, stream);
        break;
    case 7:
        cCounterNode_cCounterNode_RO(cMemMan_alloc(36, str_Counter, 0x20000000, 0), bucket, stream);
        break;
    case 8:
        cBoostNode_cBoostNode_RO(cMemMan_alloc(80, str_Boost, 0x20000000, 0), bucket, stream);
        break;
    case 9:
        cFenceNode_cFenceNode_RO(cMemMan_alloc(268, str_Fence, 0x20000000, 0), bucket, stream);
        break;
    case 10:
        cFlagNode_cFlagNode_RO(cMemMan_alloc(0x1C, str_Flag, 0x20000000, 0), bucket, stream);
        break;
    case 11:
        cMeshAnimNode_cMeshAnimNode_RO(cMemMan_alloc(1728, str_MeshAnim, 0x20000000, 0), bucket, stream);
        break;
    case 13:
        cParticleNode_cParticleNode_RO(cMemMan_alloc(44, str_Particle, 0x20000000, 0), bucket, stream);
        break;
    case 18:
        cRailManNode_cRailManNode_RO(cMemMan_alloc(536, str_RailMan, 0x20000000, 0), bucket, stream);
        break;
    case 19:
        cRestoreNode_cRestoreNode1_RO(cMemMan_alloc(0x1C, str_Restore, 0x20000000, 0), bucket, stream);
        break;
    case 14:
        cFloatingNode_cFloatingNode1_RO(cMemMan_alloc(0x7C, str_Floating, 0x20000000, 0), bucket, stream);
        break;
    case 15:
        cConveyorNode_cConveyorNode1_RO(cMemMan_alloc(72, str_Conveyor, 0x20000000, 0), bucket, stream);
        break;
    case 16:
        cHideNode_cHideNode1_RO(cMemMan_alloc(0x1C, str_Hide, 0x20000000, 0), bucket, stream);
        break;
    case 17:
        cObjectNode_cObjectNode1_RO(cMemMan_alloc(44, str_Object, 0x20000000, 0), bucket, stream);
        break;
    case 20:
        cFloatRailNode_cFloatRailNode1_RO(cMemMan_alloc(0x80, str_FloatRail, 0x20000000, 0), bucket, stream);
        break;
    case 21:
        cSpringRailNode_cSpringRailNode1_RO(cMemMan_alloc(0x80, str_SpringRail, 0x20000000, 0), bucket, stream);
        break;
    case 22: {
        sObj_RO* deadFadeNode = cDeadFadeNode_cDeadFadeNode1_RO(cMemMan_alloc(0x3C, str_DeadFade, 0x20000000, 0), bucket, stream);
        if (deadFadeNode != 0) {
            deadFadeNode->vt[1].fn((char*)deadFadeNode + deadFadeNode->vt[1].delta, 3);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplay_restoreDeadBucket);
#ifdef SKIP_ASM
class cStreamRDB {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
};

struct sRdbModelSet {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};

struct sRdbWorld {
    char pad_0x0[0x8];
    sRdbModelSet** sets;    // 0x8
};

extern char str_Dead_Object[];
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern "C" void* cDeadNode_cDeadNode(void* self, void* model);

static inline void* refToPtrRDB(unsigned int p)
{
    return (void*)(p << 2);
}

struct sRdbRef {
    unsigned int id;
    sRdbRef() : id(0xFFFFFFFF) {}

    void* get()
    {
        sRdbModelSet* set = ((sRdbWorld*)cWorld_m_pTheWorld->m_pWorldCache)->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        unsigned int p = set->refs[id >> 8] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtrRDB(p);
    }
};

/**
 * Recreates the dead (removed) objects recorded in a replay stream.
 *
 * Reads an 8-byte header with the count, then for each a 4-byte world cache reference (low byte model set,
 * upper bits index); allocates a 0x1C-byte "Dead Object" and builds a cDeadNode (cDeadNode_cDeadNode) for the
 * referenced instance.
 *
 * @param stream  the replay stream to read from
 */
extern "C" void cReplay_restoreDeadBucket(cReplay* self, cStreamRDB* stream)
{
    struct {
        int count;
        int pad;
    } hdr;
    sRdbRef ref;
    stream->read(&hdr, 8);
    for (int i = 0; i < hdr.count; i++) {
        stream->read(&ref, 4);
        void* mem = cMemMan_alloc(0x1C, str_Dead_Object, 0x20000000, 0);
        cDeadNode_cDeadNode(mem, ref.get());
    }
}
#endif

extern "C" void* cReplayFrameList_findFrame(void*);

//100%
INCLUDE_ASM("replay/replay", cReplay_findFrame__FPv);
#ifdef SKIP_ASM
/**
 * Returns the recorded frame that holds a tick, by searching FrameList (+0x3B0) with cReplayFrameList_findFrame.
 *
 * Declared with only self: the tick the caller passes stays in $a1 and reaches cReplayFrameList_findFrame. cReplay_findNextFrame,
 * cReplay_findPrevFrame and cReplay_lastFrame use it.
 *
 * @return the frame containing the tick (see cReplayFrameList_findFrame)
 */
void* cReplay_findFrame(void* vself)
{
    cReplay* self = (cReplay*)vself;
    return cReplayFrameList_findFrame(&self->FrameList);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", cReplay_findNextFrame);
#ifdef SKIP_ASM
/**
 * Returns the recorded frame after the one containing a tick, or that frame itself when it is the newest (On Tour
 * findNextFrame(int tick)).
 *
 * Declared with only self: the tick stays in $a1 and reaches cReplay_findFrame, whose frame's Next (+0x14) is
 * taken when non-null.
 *
 * @return the next cReplayFrame
 */
extern "C" void* cReplay_findNextFrame(cReplay* self)
{
    void* frame = cReplay_findFrame(self);
    void* nextFrame = *(void**)((char*)frame + 0x14);
    if (nextFrame != 0) {
        frame = nextFrame;
    }
    return frame;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", cReplay_findPrevFrame);
#ifdef SKIP_ASM
/**
 * Returns the recorded frame before the one containing a tick, or that frame itself when it is the oldest (On Tour
 * findPrevFrame(int tick)).
 *
 * Declared with only self: the tick stays in $a1 and reaches cReplay_findFrame, whose frame's Prev (+0x18) is
 * taken when non-null.
 *
 * @return the previous cReplayFrame
 */
extern "C" void* cReplay_findPrevFrame(cReplay* self)
{
    void* frame = cReplay_findFrame(self);
    void* prevFrame = *(void**)((char*)frame + 0x18);
    if (prevFrame != 0) {
        frame = prevFrame;
    }
    return frame;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", cReplay_rewindFrame);
#ifdef SKIP_ASM
// PORT: cReplay_findFrame is declared in this unit with one parameter, but this caller
// passes (self, index); bind the 2-arg form to the symbol.
void* cReplay_findFrame_2(void* self, int idx) __asm__("cReplay_findFrame__FPv");
extern "C" int cReplayCache_moveTo(void* stream, unsigned int pos);


/**
 * Moves the playback cursor back to the start of its frame: PlaybackFrame = the frame containing PlaybackTick,
 * PlaybackTick = its FrameStartTick (+0x30), and each rider input cache moves there (cReplayCache_moveTo,
 * cReplayCache::moveTo).
 *
 * @param self_  the replay (this; copied into a local self)
 */
extern "C" void cReplay_rewindFrame(cReplay* self_)
{
    cReplay* self = self_;
    int i;
    void* frame = cReplay_findFrame_2(self, self->PlaybackTick);
    (*(void**)&self->PlaybackFrame) = frame;
    self->PlaybackTick = *(int*)((char*)frame + 0x30);
    for (i = 0; i < 2; i++) {
        if (((void**)&self->InputCache[0])[i] != 0) {
            cReplayCache_moveTo(((void**)&self->InputCache[0])[i], self->PlaybackTick);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", cReplay_nextFrame);
#ifdef SKIP_ASM
// PORT: cReplay_findNextFrame really takes (self, index); the unit declares it with 1 arg
void* cReplay_findNextFrame_2(void* self, int idx) __asm__("cReplay_findNextFrame");
extern "C" void cReplay_rewindFrame(cReplay* self);

/**
 * Moves the playback cursor to the start of the next recorded frame (findNextFrame of PlaybackTick), then rewindFrame
 * re-syncs the frame and the input caches.
 */
extern "C" void cReplay_nextFrame(cReplay* self)
{
    void* frame = cReplay_findNextFrame_2(self, self->PlaybackTick);
    self->PlaybackFrame = (cReplayFrame*)frame;
    self->PlaybackTick = *(int*)((char*)frame + 0x30);
    cReplay_rewindFrame(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", cReplay_prevFrame);
#ifdef SKIP_ASM
// PORT: cReplay_findPrevFrame really takes (self, index); the unit declares it with 1 arg
void* cReplay_findPrevFrame_2(void* self, int idx) __asm__("cReplay_findPrevFrame");
extern "C" void cReplay_rewindFrame(cReplay* self);

/**
 * Moves the playback cursor to the start of the previous recorded frame (findPrevFrame of PlaybackTick), then
 * rewindFrame re-syncs the frame and the input caches.
 */
extern "C" void cReplay_prevFrame(cReplay* self)
{
    void* frame = cReplay_findPrevFrame_2(self, self->PlaybackTick);
    self->PlaybackFrame = (cReplayFrame*)frame;
    self->PlaybackTick = *(int*)((char*)frame + 0x30);
    cReplay_rewindFrame(self);
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayFrameList_cReplayFrameList__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty replay frame list: zeroes Tail (+0x0), Head (+0x4) and NumFrames (+0x8).
 *
 * cReplay_cReplay builds FrameList and FreeFrameList with it.
 *
 * @return @p self
 */
void* cReplayFrameList_cReplayFrameList(void* vself)
{
    cReplayFrameList* self = (cReplayFrameList*)vself;
    int zero = 0;
    self->Head = (cReplayFrame*)zero;
    *(int*)self = zero;
    self->NumFrames = zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayFrameList_reset__FPv);
#ifdef SKIP_ASM
/**
 * Empties a replay frame list.
 *
 * Clears the head (+0x0), tail (+0x4) and count (+0x8) without touching the frames. cReplay_reset and cReplay_purge
 * reset FrameList and FreeFrameList with it, cReplay_load FrameList.
 */
void cReplayFrameList_reset(void* vself)
{
    cReplayFrameList* self = (cReplayFrameList*)vself;
    self->Head = 0;
    self->Tail = 0;
    self->NumFrames = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayFrameList_addFrame);
#ifdef SKIP_ASM
struct sReplayNode {
    char pad[0x14];
    sReplayNode* next;
    sReplayNode* prev;
};

struct sReplayList {
    sReplayNode* head;
    sReplayNode* tail;
    int count;
};

/**
 * Appends a frame at the tail of a replay frame list.
 *
 * Links @p node after the current tail (its prev +0x18 = tail, next +0x14 = 0), makes it the tail (and the head of an
 * empty list) and increments the count.
 *
 * @param list  the frame list (this)
 * @param node  the frame to append
 */
extern "C" void cReplayFrameList_addFrame(sReplayList* list, sReplayNode* node)
{
    node->prev = list->tail;
    node->next = 0;
    if (list->tail) {
        list->tail->next = node;
    }
    list->tail = node;
    if (!list->head) {
        list->head = node;
    }
    list->count++;
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayFrameList_remFrame);
#ifdef SKIP_ASM
/**
 * Unlinks frame @p node from the replay frame list and decrements its NumFrames.
 *
 * If the frame is the list's first (oldest, +0x0) or last (newest, +0x4) entry that end moves to its neighbour; the
 * neighbours' Next (+0x14) and Prev (+0x18) are joined and the frame's own links cleared. The frame must be in the
 * list: nothing is checked. cReplay_freeReplayFrame calls it on FrameList.
 *
 * @param list  the frame list
 * @param node  the frame to remove
 */
extern "C" void cReplayFrameList_remFrame(sReplayList* list, sReplayNode* node)
{
    if (node == list->head) {
        list->head = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    if (node->next != 0) {
        node->next->prev = node->prev;
    }
    if (node->prev != 0) {
        node->prev->next = node->next;
    }
    node->next = 0;
    node->prev = 0;
    list->count--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", cReplayFrameList_remTail);
#ifdef SKIP_ASM
/**
 * Removes the oldest frame (Tail, +0x0) from the replay frame list and returns it.
 *
 * Unlinks it with cReplayFrameList_remFrame. The list must not be empty: a null Tail is passed on unchecked.
 * cReplay_allocReplayFrame takes free frames from FreeFrameList with it.
 *
 * @param list  the frame list
 * @return the removed frame
 */
extern "C" sReplayNode* cReplayFrameList_remTail(sReplayList* list)
{
    sReplayNode* node = list->head;
    cReplayFrameList_remFrame(list, node);
    return node;
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayFrameList_findFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sReplayFrame {
    char pad[0x14];
    sReplayFrame* next;
    char pad2[0x30 - 0x18];
    int time;
};

struct sReplayFrameList {
    sReplayFrame* head;
    sReplayFrame* tail;
    int count;
};

// the unit declares this with one arg; bind the real 2-arg body to the symbol
sReplayFrame* cReplayFrameList_findFrame_impl(sReplayFrameList* list, int time) __asm__("cReplayFrameList_findFrame");

/**
 * Returns the frame of the list that holds tick @p time: the newest frame starting at or before it.
 *
 * Walks from the oldest frame (+0x0) along Next (+0x14) and stops at a frame whose FrameStartTick (+0x30) equals
 * @p time or lies below it with the next frame starting above it; with no later frame the newest frame is returned, so
 * a tick before the first frame also gives the oldest one only if it matches. The list must not be empty.
 *
 * @param list  the frame list
 * @param time  the tick to look for
 * @return the frame containing the tick (the newest frame when none matches)
 */
sReplayFrame* cReplayFrameList_findFrame_impl(sReplayFrameList* list, int time)
{
    sReplayFrame* frame = list->head;
    while (frame->next) {
        if (frame->time == time) {
            break;
        }
        if (frame->time < time && time < frame->next->time) {
            break;
        }
        frame = frame->next;
    }
    return frame;
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayHighlight_begin);
#ifdef SKIP_ASM
void cReplayFrame_setRanking(void*, int, int);

struct sReplayDataE6E0 {
    int v[0xAC / 4];
};

struct sReplayObjE6E0 {
    int index;                  // 0x0
    sReplayDataE6E0 data;       // 0x4
    void* rider;                // 0xB0
};

/**
 * Starts marking a highlight for this player on frame @p rider: flags the frame and keeps a copy of the rider stats.
 *
 * Sets bit PlayerId in the frame's highlight byte (+0x1E), resets the player's ranking on the frame to 0
 * (cReplayFrame_setRanking), stores the frame in Frame (+0xB0) and copies the 0xAC bytes of @p src into Stats (+0x4).
 *
 * @param rider  the replay frame the highlight starts on
 * @param src  the rider stats at the start of the highlight
 */
extern "C" void cReplayHighlight_begin(sReplayObjE6E0* self, char* rider, sReplayDataE6E0* src)
{
    *(unsigned char*)(rider + 0x1E) |= 1 << self->index;
    cReplayFrame_setRanking(rider, self->index, 0);
    self->rider = rider;
    self->data = *src;
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayHighlight_end__FPv);
#ifdef SKIP_ASM
/**
 * Ends this player's highlight mark: clears Frame (+0xB0) and leaves the frame's highlight flag and ranking in place.
 *
 * cReplay_endMarkHighlight calls it on Highlight[playerId] once the highlight is kept.
 */
void cReplayHighlight_end(void* vself)
{
    cReplayHighlight* self = (cReplayHighlight*)vself;
    self->Frame = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayHighlight_cancel);
#ifdef SKIP_ASM
/**
 * Cancels this player's highlight mark: clears bit PlayerId in the marked frame's highlight byte (+0x1E) and clears
 * Frame.
 *
 * Does nothing when no frame is marked (Frame, +0xB0, is null).
 */
extern "C" void cReplayHighlight_cancel(void* self)
{
    unsigned char* cache = *(unsigned char**)((char*)self + 0xb0);
    if (cache) {
        cache[0x1e] &= ~(1 << *(int*)self);
        *(int*)((char*)self + 0xb0) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replay", cReplayHighlight_setRanking);
#ifdef SKIP_ASM
void cReplayFrame_setRanking(void*, int, int);

/**
 * Ranks this player's highlight from the rider stats at its end and stores the ranking on the marked frame.
 *
 * With @p mode set (a landing): the points gained since the highlight began (the stats word at +0x9C minus the kept
 * copy) give the ranking when they are at least 1000; otherwise an air time (float at +0x44) of at least 5.0 gives air
 * time * 1000. With no gain at all the result is 0. Without @p mode, or when neither rule applied, the ranking is minus
 * the increase of the word at +0xA4 (On Tour TotalPointsLostToWipeouts), and 0 is returned when it did not change. The
 * ranking is stored with cReplayFrame_setRanking on Frame (+0xB0) for PlayerId.
 *
 * @param stats  the rider stats now (On Tour tRiderStats)
 * @param mode  non-zero when the highlight ended with a landing
 * @return 1 when a ranking was stored, 0 when nothing changed
 */
extern "C" int cReplayHighlight_setRanking(void* self, tRiderStats* stats, int mode)
{
    if (mode) {
        int pointsGained = stats->TotalPoints - *(int*)((char*)self + 0xA0);
        if (pointsGained == 0) return 0;
        if (pointsGained >= 1000) {
            cReplayFrame_setRanking(*(void**)((char*)self + 0xB0), *(int*)self, pointsGained);
            return 1;
        }
        float airTime = stats->AirTime;
        if (airTime >= 5.0f) {
            cReplayFrame_setRanking(*(void**)((char*)self + 0xB0), *(int*)self, (int)(airTime * 1000.0f));
            return 1;
        }
    }
    int pointsLost = stats->TotalPointsLostToWipeouts - *(int*)((char*)self + 0xA8);
    if (pointsLost == 0) return 0;
    cReplayFrame_setRanking(*(void**)((char*)self + 0xB0), *(int*)self, -pointsLost);
    return 1;
}
#endif

extern void* cReplayFramePtr_vtbl[];

//100%
INCLUDE_ASM("replay/replay", cReplayFramePtr_cReplayFramePtr__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty replay frame: stores cReplayFramePtr_vtbl and clears Block, WriteData, ReadData and Size.
 *
 * @return @p self
 */
void* cReplayFramePtr_cReplayFramePtr(void* vself)
{
    cReplayFramePtr* self = (cReplayFramePtr*)vself;
    int zeroValue = 0;
    self->Block = (char*)zeroValue;
    *(int*)self = (int)(void*)cReplayFramePtr_vtbl;
    self->WriteData = (char*)zeroValue;
    self->ReadData = (char*)zeroValue;
    self->Size = zeroValue;
    return ((void*)self);
}
#endif

