#include "common.h"
#include "replay/replay.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("replay/replay", cReplay_restoreFrame);
#ifdef SKIP_ASM
class cStreamRF {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
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
    stream->v02(buf, 0x18);
    AIsetseed(buf);
    int savedValue;
    stream->v02(&savedValue, 4);
    cSortObjNode_ObjNodeSortIndex = savedValue;
    stream->v02(&savedValue, 4);
    cSortObjNode_ObjNodeNoSortIndex = savedValue;
    cWorldPainterMan_reset_0();
    cBE_readFromReplayFrame(cBE_getBE(), stream);
    char* game = (char*)SSXApp->Game;
    sVtS_DBF0* vt = *(sVtS_DBF0**)game;
    vt[9].fn(game + vt[9].delta, stream);
    char* aiObj = (char*)SSXApp->Game->AI;
    sVtS_DBF0* vt2 = *(sVtS_DBF0**)(aiObj + 0xCC);
    vt2[8].fn(aiObj + vt2[8].delta, stream);
    cAvalanche_readFromReplayFrame((char*)stream);
    cWScriptMan_readFromReplayFrame(cWScriptMan_TheWScriptMan, stream);
    func_00229E58(D_004A2A00, stream);
    func_00344240(D_004A3FF0, stream);
    cMultiParticleMan_readFromReplayFrame(cMultiParticleMan_TheMultiParticleMan, stream);
    cReplay_restoreBucket(self, (void*)1, (cStreamRB*)stream);
    cReplay_restoreDeadBucket(self, (cStreamRDB*)stream);
    cFocusMan_readFromReplayFrame(*(void**)((char*)SSXApp->Game->AI + 0xA4), stream);
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
    virtual void v02(void* buf, int size);
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
    stream->v02(&hdr, 8);
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
extern "C" void* func_00350FD8_RO(void* self, void* bucket, void* stream) __asm__("func_00350FD8");
extern char str_Floating[];
extern "C" void* func_0034ED88_RO(void* self, void* bucket, void* stream) __asm__("func_0034ED88");
extern char str_Conveyor[];
extern "C" void* func_003430D0_RO(void* self, void* bucket, void* stream) __asm__("func_003430D0");
extern char str_Hide[];
extern "C" void* func_00350F08_RO(void* self, void* bucket, void* stream) __asm__("func_00350F08");
extern char str_Object[];
extern "C" void* func_00356E60_RO(void* self, void* bucket, void* stream) __asm__("func_00356E60");
extern char str_FloatRail[];
extern "C" void* func_00348FA0_RO(void* self, void* bucket, void* stream) __asm__("func_00348FA0");
extern char str_SpringRail[];
extern "C" void* func_00349DB0_RO(void* self, void* bucket, void* stream) __asm__("func_00349DB0");
extern char str_DeadFade[];
struct sVE_RO { short delta; short index; void (*fn)(void*, int); };
struct sObj_RO { int pad[3]; sVE_RO* vt; };
extern "C" sObj_RO* func_00350B98_RO(void* self, void* bucket, void* stream) __asm__("func_00350B98");

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
        cDebounceNode_cDebounceNode_RO(cMemMan_alloc(0x34, str_Debounce, 0x20000000, 0), bucket, stream);
        break;
    case 1:
        cAnimObjectNode_cAnimObjectNode_RO(cMemMan_alloc(0x6C, str_AnimObject, 0x20000000, 0), bucket, stream);
        break;
    case 2:
        cAnimDeltaNode_cAnimDeltaNode_RO(cMemMan_alloc(0x70, str_AnimDelta, 0x20000000, 0), bucket, stream);
        break;
    case 3:
        cAnimComboNode_cAnimComboNode_RO(cMemMan_alloc(0x88, str_AnimCombo, 0x20000000, 0), bucket, stream);
        break;
    case 4:
        cAnimTeeterNode_cAnimTeeterNode_RO(cMemMan_alloc(0x80, str_AnimTeeter, 0x20000000, 0), bucket, stream);
        break;
    case 5:
        cFlexBridgeNode_cFlexBridgeNode1_RO(cMemMan_alloc(0xB0, str_FlexBridge, 0x20000000, 0), bucket, stream);
        break;
    case 7:
        cCounterNode_cCounterNode_RO(cMemMan_alloc(0x24, str_Counter, 0x20000000, 0), bucket, stream);
        break;
    case 8:
        cBoostNode_cBoostNode_RO(cMemMan_alloc(0x50, str_Boost, 0x20000000, 0), bucket, stream);
        break;
    case 9:
        cFenceNode_cFenceNode_RO(cMemMan_alloc(0x10C, str_Fence, 0x20000000, 0), bucket, stream);
        break;
    case 10:
        cFlagNode_cFlagNode_RO(cMemMan_alloc(0x1C, str_Flag, 0x20000000, 0), bucket, stream);
        break;
    case 11:
        cMeshAnimNode_cMeshAnimNode_RO(cMemMan_alloc(0x6C0, str_MeshAnim, 0x20000000, 0), bucket, stream);
        break;
    case 13:
        cParticleNode_cParticleNode_RO(cMemMan_alloc(0x2C, str_Particle, 0x20000000, 0), bucket, stream);
        break;
    case 18:
        cRailManNode_cRailManNode_RO(cMemMan_alloc(0x218, str_RailMan, 0x20000000, 0), bucket, stream);
        break;
    case 19:
        func_00350FD8_RO(cMemMan_alloc(0x1C, str_Restore, 0x20000000, 0), bucket, stream);
        break;
    case 14:
        func_0034ED88_RO(cMemMan_alloc(0x7C, str_Floating, 0x20000000, 0), bucket, stream);
        break;
    case 15:
        func_003430D0_RO(cMemMan_alloc(0x48, str_Conveyor, 0x20000000, 0), bucket, stream);
        break;
    case 16:
        func_00350F08_RO(cMemMan_alloc(0x1C, str_Hide, 0x20000000, 0), bucket, stream);
        break;
    case 17:
        func_00356E60_RO(cMemMan_alloc(0x2C, str_Object, 0x20000000, 0), bucket, stream);
        break;
    case 20:
        func_00348FA0_RO(cMemMan_alloc(0x80, str_FloatRail, 0x20000000, 0), bucket, stream);
        break;
    case 21:
        func_00349DB0_RO(cMemMan_alloc(0x80, str_SpringRail, 0x20000000, 0), bucket, stream);
        break;
    case 22: {
        sObj_RO* deadFadeNode = func_00350B98_RO(cMemMan_alloc(0x3C, str_DeadFade, 0x20000000, 0), bucket, stream);
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
    virtual void v02(void* buf, int size);
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
    stream->v02(&hdr, 8);
    for (int i = 0; i < hdr.count; i++) {
        stream->v02(&ref, 4);
        void* mem = cMemMan_alloc(0x1C, str_Dead_Object, 0x20000000, 0);
        cDeadNode_cDeadNode(mem, ref.get());
    }
}
#endif

extern "C" void* func_0026E6A0(void*);

//100%
INCLUDE_ASM("replay/replay", func_0026E448__FPv);
#ifdef SKIP_ASM
void* func_0026E448(void* self)
{
    return func_0026E6A0((char*)self + 0x3b0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", cReplay_findNextFrame);
#ifdef SKIP_ASM
/**
 * Returns the recorded frame after the one containing a tick, or that frame itself when it is the newest (On Tour
 * findNextFrame(int tick)).
 *
 * Declared with only self: the tick stays in $a1 and reaches func_0026E448 (findFrame), whose frame's Next (+0x14) is
 * taken when non-null.
 *
 * @return the next cReplayFrame
 */
extern "C" void* cReplay_findNextFrame(cReplay* self)
{
    void* frame = func_0026E448(self);
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
 * Declared with only self: the tick stays in $a1 and reaches func_0026E448 (findFrame), whose frame's Prev (+0x18) is
 * taken when non-null.
 *
 * @return the previous cReplayFrame
 */
extern "C" void* cReplay_findPrevFrame(cReplay* self)
{
    void* frame = func_0026E448(self);
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
// PORT: func_0026E448 is declared in this unit with one parameter, but this caller
// passes (self, index); bind the 2-arg form to the symbol.
void* func_0026E448_2(void* self, int idx) __asm__("func_0026E448__FPv");
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
    void* frame = func_0026E448_2(self, self->PlaybackTick);
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
INCLUDE_ASM("replay/replay", func_0026E5A8__FPv);
#ifdef SKIP_ASM
void* func_0026E5A8(void* self)
{
    int zero = 0;
    *(int*)((char*)self + 0x4) = zero;
    *(int*)self = zero;
    *(int*)((char*)self + 0x8) = zero;
    return self;
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
void cReplayFrameList_reset(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x8) = 0;
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
INCLUDE_ASM("replay/replay", func_0026E608);
#ifdef SKIP_ASM
extern "C" void func_0026E608(sReplayList* list, sReplayNode* node)
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
INCLUDE_ASM("replay/replay", func_0026E670);
#ifdef SKIP_ASM
extern "C" sReplayNode* func_0026E670(sReplayList* list)
{
    sReplayNode* node = list->head;
    func_0026E608(list, node);
    return node;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E6A0);
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
sReplayFrame* func_0026E6A0_impl(sReplayFrameList* list, int time) __asm__("func_0026E6A0");

sReplayFrame* func_0026E6A0_impl(sReplayFrameList* list, int time)
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
INCLUDE_ASM("replay/replay", func_0026E6E0);
#ifdef SKIP_ASM
void func_0026D730(void*, int, int);

struct sReplayDataE6E0 {
    int v[0xAC / 4];
};

struct sReplayObjE6E0 {
    int index;                  // 0x0
    sReplayDataE6E0 data;       // 0x4
    void* rider;                // 0xB0
};

extern "C" void func_0026E6E0(sReplayObjE6E0* self, char* rider, sReplayDataE6E0* src)
{
    *(unsigned char*)(rider + 0x1E) |= 1 << self->index;
    func_0026D730(rider, self->index, 0);
    self->rider = rider;
    self->data = *src;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E7F8__FPv);
#ifdef SKIP_ASM
void func_0026E7F8(void* self)
{
    *(int*)((char*)self + 0xB0) = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E800);
#ifdef SKIP_ASM
extern "C" void func_0026E800(void* self)
{
    unsigned char* cache = *(unsigned char**)((char*)self + 0xb0);
    if (cache) {
        cache[0x1e] &= ~(1 << *(int*)self);
        *(int*)((char*)self + 0xb0) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E838);
#ifdef SKIP_ASM
void func_0026D730(void*, int, int);

extern "C" int func_0026E838(void* self, void* stats, int mode)
{
    if (mode) {
        int d = *(int*)((char*)stats + 0x9C) - *(int*)((char*)self + 0xA0);
        if (d == 0) return 0;
        if (d >= 1000) {
            func_0026D730(*(void**)((char*)self + 0xB0), *(int*)self, d);
            return 1;
        }
        float v = *(float*)((char*)stats + 0x44);
        if (v >= 5.0f) {
            func_0026D730(*(void**)((char*)self + 0xB0), *(int*)self, (int)(v * 1000.0f));
            return 1;
        }
    }
    int e = *(int*)((char*)stats + 0xA4) - *(int*)((char*)self + 0xA8);
    if (e == 0) return 0;
    func_0026D730(*(void**)((char*)self + 0xB0), *(int*)self, -e);
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
void* cReplayFramePtr_cReplayFramePtr(void* self)
{
    int zeroValue = 0;
    *(int*)((char*)self + 0x4) = zeroValue;
    *(int*)self = (int)(void*)cReplayFramePtr_vtbl;
    *(int*)((char*)self + 0x8) = zeroValue;
    *(int*)((char*)self + 0xc) = zeroValue;
    *(int*)((char*)self + 0x10) = zeroValue;
    return self;
}
#endif

