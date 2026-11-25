#include "vtable.h"
#include "common.h"
#include "object/instancenode.h"
#include "world/instancerecord.h"
#include "replay/frame.h"
#include "sound/ssxaudio.h"
#include "main/gameviewman.h"
#include "ui/uiengine.h"
#include "camera/trigger/cameratriggerman.h"
#include "replay/frameptr.h"
#include "replay/replay.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "camera/camera.h"
#include "camera/race/racecontroller.h"
#include "replay/bxreplayframe.h"
//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_initBlock);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_Frame_Block[];

// same body as cReplayFramePtr_setBlock, inlined here
static inline void cReplayFramePtr_setBlockInl(void* vself, char* buf, int size)
{
    cReplayFramePtr* self = (cReplayFramePtr*)vself;
    self->Block = buf;
    self->WriteData = buf + size;
    self->ReadData = buf;
    self->Size = size;
}

/**
 * Allocates a replay frame block and points the frame pointer at it as an empty block.
 *
 * Allocates @p size bytes tagged "Frame Block", then sets the start and read position to it and the end and
 * used size to the start and 0.
 *
 * @param size  size of the block in bytes
 */
void cReplayFramePtr::initBlock(unsigned int size)
{
    cReplayFramePtr_setBlockInl(this, (char*)operator_new_tag(size, str_Frame_Block, 0, 0), 0);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_setBlock);
#ifdef SKIP_ASM
/**
 * Points the replay frame at an existing block of data: the read cursor at its start, the write cursor at its end.
 *
 * Sets Block and ReadData to @p data, WriteData to @p data + @p size and Size to @p size.
 *
 * @param data  the block's data (On Tour char* data)
 * @param size  the block's size in bytes (On Tour size)
 */
void cReplayFramePtr::setBlock(char* data, int size)
{
    Block = data;
    WriteData = data + size;
    ReadData = data;
    Size = size;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_purgeBlock);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void cReplayFramePtr_purgeBlock(cReplayFramePtr* self);

/**
 * Frees the frame's data block, when it has one, and resets the block and its cursors to empty (size 0).
 */
void cReplayFramePtr::purgeBlock()
{
    void* block = Block;
    if (block != 0) {
        cMemMan_free(block);
    }
    cReplayFramePtr_setBlockInl(this, 0, 0);
}
#endif


//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_readRewind__FP15cReplayFramePtr);
#ifdef SKIP_ASM
/**
 * Rewinds the frame pointer's read position (+0xC) to the start of its block (+0x4).
 */
void cReplayFramePtr::readRewind()
{
    *(int*)&ReadData = ((int)Block);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_writeRewind__FPv);
#ifdef SKIP_ASM
/**
 * Rewinds the frame pointer's write position: sets WriteData to Block and Size to 0, so the block is rewritten.
 *
 * cReplay_saveFrame calls it before writing a frame; cReplayFramePtr_readRewind is the read-side counterpart.
 *
 * @return the block start (Block)
 */
int cReplayFramePtr::writeRewind()
{
    int block = (int)Block;
    Size = 0;
    WriteData = (char*)block;
    return block;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_write);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int size);

/**
 * Appends @p n bytes to the frame: copies them to WriteData, then advances WriteData and grows Size by @p n.
 *
 * @param src  the bytes to write (On Tour const char* data)
 * @param n  how many (On Tour len)
 */
extern "C" void cReplayFramePtr_write(cReplayFramePtr* self, void* src, int n)
{
    MEM_copy(self->WriteData, src, n);
    self->WriteData += n;
    self->Size += n;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_read);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int size);

/**
 * Reads the next @p n bytes of the frame into @p dst and advances ReadData.
 *
 * @param dst  receives the bytes (On Tour char* data)
 * @param n  how many (On Tour len)
 */
extern "C" void cReplayFramePtr_read(cReplayFramePtr* self, void* dst, int n)
{
    MEM_copy(dst, self->ReadData, n);
    self->ReadData += n;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_peekObjNodeType);
#ifdef SKIP_ASM
/**
 * Reads the type of the next object record in the replay frame without consuming it.
 *
 * Reads the 4-byte record header (type, size) through the read virtual (slot 2), then puts ReadData back.
 *
 * @return the record's object node type (On Tour tObjNodeType)
 */
// Peek the next frame header's id without consuming it.
short cReplayFramePtr::peekObjNodeType()
{
    struct {
        short id;
        short size;
    } hdr;
    char* pos = ReadData;
    read((char*)&hdr, 4);
    ReadData = pos;
    return hdr.id;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_loadBlock);
#ifdef SKIP_ASM
extern "C" int HUFF_decode(void* src, int a, char* dst);

/**
 * Loads a saved replay frame block: unpacks the saved bytes into Block and rewinds the read cursor.
 *
 * Reads the 4-byte size at *@p pp and steps over it, unpacks the data that follows into Block (HUFF_decode, which
 * returns the unpacked size), steps *@p pp past the saved size, sets WriteData to Block + the unpacked size and
 * calls cReplayFramePtr_readRewind. The 4-byte tag copied from @p src is not used.
 *
 * @param pp  the read cursor in the saved data (On Tour char** d); advanced past the block
 * @param src  the frame's save record (On Tour tFramePtrSave* ptrsave)
 * @return always 1
 */
int cReplayFramePtr::loadBlock(char** pp, tFramePtrSave* src)
{
    struct {
        int tag;
        int size;
    } hdr;
    MEM_copy(&hdr.tag, src, 4);
    MEM_copy(&hdr.size, *pp, 4);
    *pp += 4;
    int n = HUFF_decode(*pp, 0, Block);
    *pp += hdr.size;
    WriteData = Block + n;
    readRewind();
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_readReplayInstance);
#ifdef SKIP_ASM

struct sRefSet_EB48 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};

struct sRefWorld_EB48 {
    char pad_0x0[0x8];
    sRefSet_EB48** sets;    // 0x8
};


extern "C" void* func_003512C0(void* table, unsigned int id);

static inline void* refToPtr_EB48(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* lookup_EB48(unsigned int id)
{
    sRefSet_EB48* set = ((sRefWorld_EB48*)cWorld_m_pTheWorld->m_pWorldCache)->sets[id & 0xFF];
    if (set == 0) {
        return 0;
    }
    unsigned int p = set->refs[id >> 8] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr_EB48(p);
}

struct sRefH_EB48 {
    unsigned int id;
    sRefH_EB48() : id(0xFFFFFFFF) {}
};

/**
 * Reads a 4-byte instance id from the frame and returns the instance record it names, or 0.
 *
 * The id is read through the stream's read slot (virtual slot 2). An id of 0xFFFFFFFF gives 0. Otherwise the instance
 * manager is asked first (func_003512C0 on SSXApp->Game->InstanceMan); if it has no record, the world cache of
 * cWorld_m_pTheWorld is used: set (id & 0xFF), entry (id >> 8), whose upper 24 bits are the record address / 4.
 * cReplayFramePtr_writeReplayInstance writes what this reads.
 *
 * @param stream  the frame pointer to read from
 * @return the instance record, or 0 for no id or an unknown one
 */
extern "C" void* cReplayFramePtr_readReplayInstance(cBxReplayFrame* stream)
{
    void* instRecord = 0;
    sRefH_EB48 savedId;
    stream->read((char*)&savedId, 4);
    if (~savedId.id != 0) {
        instRecord = func_003512C0(SSXApp->Game->InstanceMan, savedId.id);
        if (instRecord == 0) {
            void* cachedRecord = lookup_EB48(savedId.id);
            instRecord = cachedRecord;
        }
    }
    return instRecord;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_readReplayInstance1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRefSet_EC08 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};

struct sRefWorld_EC08 {
    char pad_0x0[0x8];
    sRefSet_EC08** sets;    // 0x8
};

// asm-label view: cReplayFramePtr_readReplayInstance declares cWorld_m_pTheWorld with its own struct type in this unit

struct sRec_EC08 {
    unsigned int id;
    unsigned short type;
    sRec_EC08() : id(0xFFFFFFFF) {}
};

struct sNode_EC08 {
    char pad_0x0[0x8];
    unsigned int flags;     // 0x8
    void* owner;            // 0xC
};

static inline void* refToPtr_EC08(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* lookup_EC08(unsigned int id)
{
    sRefSet_EC08* set = ((sRefWorld_EC08*)cWorld_m_pTheWorld->m_pWorldCache)->sets[id & 0xFF];
    if (set == 0) {
        return 0;
    }
    unsigned int p = set->refs[id >> 8] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr_EC08(p);
}

/**
 * Reads an 8-byte instance reference (id, type) from the frame, binds the record it names to @p owner and returns it.
 *
 * The id and a 16-bit type are read through the stream's read slot (virtual slot 2). An id of 0xFFFFFFFF gives 0;
 * otherwise the record comes from the world cache of cWorld_m_pTheWorld (set id & 0xFF, entry id >> 8, upper 24 bits =
 * address / 4), with no null check. The record's word at +0xC is set to @p owner and the low 16 bits of its flags
 * (+0x8) are replaced by the saved type, keeping bits 8 and 9. cReplayFramePtr_writeReplayInstance1 writes what this reads.
 *
 * @param stream  the frame pointer to read from
 * @param owner  the node that becomes the record's owner (stored at +0xC)
 * @return the instance record, or 0 for no id
 */
extern "C" void* cReplayFramePtr_readReplayInstance1(cBxReplayFrame* stream, void* owner)
{
    sNode_EC08* instRecord = 0;
    sRec_EC08 rec;
    stream->read((char*)&rec, 8);
    if (~rec.id != 0) {
        void* cachedRecord = lookup_EC08(rec.id);
        instRecord = (sNode_EC08*)cachedRecord;
        instRecord->owner = owner;
        instRecord->flags = (instRecord->flags & 0xFFFF0300) | (rec.type & 0xFCFF);
    }
    return instRecord;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_writeReplayInstance);
#ifdef SKIP_ASM
struct sFpSerVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sFpSerObj {
    sFpSerVEntry* vt;
};

/**
 * Writes the 4-byte id of instance record @p obj (its word at +0x78) to the frame, or 0xFFFFFFFF when @p obj is null.
 *
 * The id goes through the stream's write slot (virtual slot 1); cReplayFramePtr_readReplayInstance reads it back.
 *
 * @param out  the frame pointer to write to
 * @param obj  the instance record, or null
 */
extern "C" void cReplayFramePtr_writeReplayInstance(cBxReplayFrame* out, tInstanceRecord* obj)
{
    unsigned int id = 0xFFFFFFFF;
    if (((void*)obj) != 0) {
        id = obj->m_RID;
    }
    out->write((const char*)&id, 4);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_writeReplayInstance1);
#ifdef SKIP_ASM
/**
 * Writes an 8-byte reference to the instance record of node @p obj (its pointer at +0x18) to the frame.
 *
 * The record holds the instance's id (+0x78) and the 16-bit type from its flags (+0x8); with no instance it is id
 * 0xFFFFFFFF and type 0. It goes through the stream's write slot (virtual slot 1); cReplayFramePtr_readReplayInstance1 reads it back. @p obj
 * itself must not be null.
 *
 * @param out  the frame pointer to write to
 * @param obj  the node whose instance is written
 */
extern "C" void cReplayFramePtr_writeReplayInstance1(cBxReplayFrame* out, cInstanceNode* obj)
{
    struct {
        unsigned int id;
        unsigned short type;
    } rec;
    rec.id = 0xFFFFFFFF;
    tInstanceRecord* inst = (tInstanceRecord*)(obj->Instance);
    if (((void*)inst) != 0) {
        rec.id = inst->m_RID;
        rec.type = *(unsigned short*)&inst->m_iFlags;
    } else {
        rec.type = 0;
    }
    out->write((const char*)&rec, 8);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_putFrameBlock__FPv);
#ifdef SKIP_ASM
/**
 * Hands a frame's data block back to storage: a no-op on the PS2.
 *
 * An empty stub, as in On Tour. cReplayFrame_loadFrame (cReplayFrame_loadFrame) calls it with (frame, 1) after
 * cReplayFramePtr_loadBlock; the wait flag in $a1 is ignored.
 */
void cReplayFramePtr_putFrameBlock(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_getFrameBlock__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing and returns nothing, despite the name: the body is empty.
 */
void cReplayFramePtr_getFrameBlock()
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_cycleReplayCameraAlgorithm);
#ifdef SKIP_ASM
extern "C" void cReplay_setCurrentReplayCamera(cReplay* self);

/**
 * Steps the replay camera to the next of the 9 (CurrentReplayCamIndex + 1, mod 9) and applies it to every view.
 */
void cReplay::cycleReplayCameraAlgorithm()
{
    CurrentReplayCamIndex = (CurrentReplayCamIndex + 1) % 9;
    setCurrentReplayCamera();
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_setCurrentReplayCamera);
#ifdef SKIP_ASM
extern int camSubset[];
extern "C" void cChaseCameraController_switchCamera_ReplayToggle(void* self, int mode);

struct sRiderList_EDD8 {
    int count;
    char* riders[1];
};

/**
 * Switches every view's chase camera (rider + 0xA8) to replay camera camSubset[CurrentReplayCamIndex].
 *
 * The views are the game's ViewMan list; each switch is cChaseCameraController_switchCamera_ReplayToggle.
 */
void cReplay::setCurrentReplayCamera()
{
    sRiderList_EDD8* list = (sRiderList_EDD8*)SSXApp->Game->ViewMan;
    int n = list->count;
    int i;
    for (i = 0; i < n; i++) {
        cChaseCameraController_switchCamera_ReplayToggle(((cCamera*)list->riders[i])->ChaseController,
                camSubset[CurrentReplayCamIndex]);
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_setupReplayCameras);
#ifdef SKIP_ASM
void* cCameraTriggerMan_setReplayTriggers(void* p);
extern "C" void cReplay_setCurrentReplayCamera(cReplay* self);

/**
 * Puts the cameras into replay mode: the camera triggers' replay set (cCameraTriggerMan_setReplayTriggers, setReplayTriggers), then the
 * current replay camera.
 */
void cReplay::setupReplayCameras()
{
    cCameraTriggerMan_setReplayTriggers(((void**)&gCamTriggerMan));
    setCurrentReplayCamera();
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_setupGameCamerasOnExitReplay);
#ifdef SKIP_ASM
void cCameraTriggerMan_setInGameTriggers(void* self);
extern "C" void cCamera_setCurrentController(void* self, int mode);
extern "C" void cChaseCameraController_switchCamera_EndReplay(void* cam);

struct sRiderList_EEA0 {
    int count;
    char* riders[1];
};

/**
 * Gives the cameras back to the game after a replay.
 *
 * Restores the in-game camera triggers, sets each view's current controller to 2 and ends its replay camera
 * (cChaseCameraController_switchCamera_EndReplay), and resets CurrentReplayCamIndex to 0.
 */
void cReplay::setupGameCamerasOnExitReplay()
{
    cCameraTriggerMan_setInGameTriggers(((void**)&gCamTriggerMan));
    sRiderList_EEA0* list = (sRiderList_EEA0*)SSXApp->Game->ViewMan;
    int n = list->count;
    int i;
    for (i = 0; i < n; i++) {
        cCamera* camera = (cCamera*)list->riders[i];
        void* cam = camera->ChaseController;
        cCamera_setCurrentController(camera, 2);
        cChaseCameraController_switchCamera_EndReplay(cam);
    }
    CurrentReplayCamIndex = 0;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_setupInstantReplayCameras);
#ifdef SKIP_ASM
/**
 * Sets up the instant-replay cameras: installs the replay camera triggers (cCameraTriggerMan_setReplayTriggers, cCameraTriggerMan::
 * setReplayTriggers, on gCamTriggerMan) and steps CurrentInstantReplayCamIndex to the next of 9.
 */
void cReplay::setupInstantReplayCameras()
{
    cCameraTriggerMan_setReplayTriggers(((void**)&gCamTriggerMan));
    CurrentInstantReplayCamIndex = (CurrentInstantReplayCamIndex + 1) % 9;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_setupGameCamerasOnExitInstantReplay);
#ifdef SKIP_ASM
void cCameraTriggerMan_setInGameTriggers(void* self);
extern "C" void cCamera_setCurrentController(void* self, int mode);

/**
 * Gives the cameras back to the game after an instant replay.
 *
 * Restores the in-game camera triggers (cCameraTriggerMan_setInGameTriggers on gCamTriggerMan) and sets the current
 * controller of the view manager's first camera (+0x4) to 2. cReplay_stopReplay calls it instead of
 * cReplay_setupGameCamerasOnExitReplay when InstantReplay is set after the save frame is restored.
 */
extern "C" void cReplay_setupGameCamerasOnExitInstantReplay()
{
    cCameraTriggerMan_setInGameTriggers(((void**)&gCamTriggerMan));
    cCamera_setCurrentController(SSXApp->Game->ViewMan->PlayerCamera[0], 2);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_cReplay);
#ifdef SKIP_ASM
extern "C" void* cReplayFrame_cReplayFrame(void* self);
void* cReplayFrameList_cReplayFrameList(void* self);
extern "C" void* cReplayHighlight_reset(void* self);
void cReplay_setReplayState(void* self, int state);

/**
 * Constructs the replay manager.
 *
 * Constructs the 16 frames (cReplayFrame_cReplayFrame, cReplayFrame), FrameList and FreeFrameList (cReplayFrameList_cReplayFrameList) and the two
 * Highlight records (cReplayHighlight_reset), sets the state to 15 (kReplayInit) and clears both InputCache pointers.
 *
 * @return @p self
 */
extern "C" void* cReplay_cReplay(cReplay* self)
{
    cReplayFrame* frame = self->Frames;
    for (int i = 15; i != -1; i--, frame++) {
        cReplayFrame_cReplayFrame(frame);
    }
    cReplayHighlight* highlight = self->Highlight;
    cReplayFrameList_cReplayFrameList(&self->FrameList);
    cReplayFrameList_cReplayFrameList(&self->FreeFrameList);
    for (int i = 1; i != -1; i--, highlight++) {
        cReplayHighlight_reset(highlight);
    }
    self->setReplayState(15);
    for (int i = 1; i >= 0; i--) {
        self->InputCache[i] = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay__cReplay);
#ifdef SKIP_ASM
struct sFpOwner26F428;
struct cReplayFramePtr_F980;
extern "C" void cReplay_purge(cReplay* self);
extern "C" void cReplay_stopReplay(cReplay* self);
void cReplay_frameSync(void* self);
void operator_delete(int* ptr);




/**
 * Destroys the replay system: stops any playback, purges it, destroys its frames and frees it if asked.
 *
 * Calls cReplay_stopReplay when State is 1..9, then cReplay_frameSync (cReplay_frameSync__FPv) and cReplay_purge. Highlight[2]
 * (+0x494, 0xB4 each) have trivial destructors; Frames[16] (+0x2C, 0x38 each) are destroyed last to first through their
 * vtable slot 7 (the cReplayFrame destructor) with flags 0. Then operator_delete on @p flags bit 0 (cGame_purge passes
 * 3).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cReplay__cReplay(int* self, int flags)
{
    char* replay = (char*)self;
    int ok = 0;
    int state = *self;
    if (state != 0) {
        ok = state;
        ok = ok < 10;
    }
    if (ok) {
        ((cReplay*)self)->stopReplay();
    }
    cReplay_frameSync(self);
    ((cReplay*)self)->purge();
    if (replay + 0x494 != 0) {
        char* element = replay + 0x494 + 0x168;
        while (replay + 0x494 != element) {
            element -= 0xB4;
            ((cReplayHighlight*)element)->~cReplayHighlight();
        }
    }
    if (replay + 0x2C != 0) {
        char* element = replay + 0x2C + 0x380;
        while (replay + 0x2C != element) {
            element -= 0x38;
            sVEntry<void(void*, int)>* vt = ((sVEntry<void (void*, int)>*)((cReplayFrame*)element)->mVtbl);
            vt[7].fn(vthis(element, vt[7]), 0);
        }
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_init);
#ifdef SKIP_ASM


extern "C" void cReplayFramePtr_initBlock(cReplayFramePtr* self, unsigned int size);
extern "C" void cReplay_reset(cReplay* self);
void cReplay_setReplayState(void* self, int state);

/**
 * Sets up the replay system: allocates its frame blocks, enters the pre-race state and resets.
 *
 * Uses 0x14000-byte blocks, as many as fit in 0xC9800 bytes (10), clears SaveFrameTicks, sets the state to
 * kPreRace (10) and calls cReplay_reset().
 */
void cReplay::init()
{
    FrameBlockSize = 0x14000;
    MaxFrames = 0xC9800 / FrameBlockSize;
    SaveFrameTicks = 0;
    int i = 0;
    do {
        cReplayFramePtr_initBlock((cReplayFramePtr*)(&((&Frames[0]))[i]), FrameBlockSize);
        i++;
    } while (i < MaxFrames);
    setReplayState(10);
    reset();
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_reset);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
void cReplayCache_reset(void* self);
void cReplayFrameList_reset(void* self);
extern "C" void cReplayFrameList_addFrame(void* list, void* block);
extern "C" void MEM_fill(void* dst, int c, int n);


/**
 * Resets the replay for a new race: clears the recording, playback and save state and refills the free frame
 * list.
 *
 * Does nothing in kReplayPaused (14) or kReplayInit (15). Otherwise sets SaveState 15 and State kPreRace
 * (10), zeroes the playback, record and writer fields, resets both input caches (cReplayCache_reset) and
 * highlights (PlayerId 0 / 1, no frame, stats cleared), empties FrameList and FreeFrameList and adds all
 * MaxFrames frames to the free list. With one human player (cAI NumberOfPlayersAllocated == 1) it keeps 3
 * highlights and 3 chapters, else 1 and 0.
 */
void cReplay::reset()
{
    unsigned int st = State - 14;
    bool resettable = !(st < 2);
    if (!resettable) return;
    cReplay_frameSync(this);
    int i = 0;
    char* frameList = (char*)&FrameList;
    char* freeList = (char*)&FreeFrameList;
    int none = -1;
    SaveState = 15;
    setReplayState(10);
    EndReplay = 0;
    ReplayError = 0;
    ShowError = 0;
    PauseRecord = 0;
    PauseRecordTick = 0;
    PadNum = 0;
    Loaded = 0;
    PlaybackTick = 0;
    RecordTick = 0;
    EndRecordTick = 0;
    PlaybackFrame = 0;
    InstantReplayFrame = 0;
    NextCandidate = 0;
    SaveFrame = 0;
    SkipFrameTicks = 0;
    SkipFrame = 0;
    Cmd = 0;
    AutoReplay = 0;
    InstantReplay = 0;
    StartInstantReplay = 0;
    WriteBlock = 0;
    WriteData = 0;
    WriteTmp = 0;
    WriteSrc = 0;
    WriteSrcSize = 0;
    WriteState = 0;
    WriteSize = 0;
    WriteIndex = 0;
    WriteIndexMax = 0;
    WriteFrame = 0;
    WriteBlockData = 0;
    WriteBlockDataSize = 0;
    WriteCmdFrame = 0;
    WriteCacheSave = 0;
    WriteSaveSize = 0;
    WriteRemaining = 0;
    WriteFrameCount = 0;
    cReplayHighlight* highlight = ((&Highlight[0]));
    char* highlightData = (char*)&Highlight[0].Stats;
    void** pp = (void**)(&InputCache[0]);
    do {
        if (*pp) cReplayCache_reset(*pp);
        *(int*)&highlight->Frame = 0;
        highlight->PlayerId = none;
        MEM_fill(highlightData, 0, 172);
        highlightData += 0xB4;
        pp++;
        highlight->PlayerId = i;
        i++;
        highlight++;
    } while (i < 2);
    cReplayFrameList_reset(frameList);
    cReplayFrameList_reset(freeList);
    for (i = 0; i < MaxFrames; i++) {
        cReplayFrameList_addFrame(freeList, &Frames[i]);
    }
    int three = 3;
    CurrentReplayCamIndex = 0;
    CurrentInstantReplayCamIndex = 0;
    if (SSXApp->Game->AI->NumberOfPlayersAllocated == 1) {
        MaxPlayerHighlights = three;
        MaxChapters = three;
    } else {
        MaxPlayerHighlights = 1;
        MaxChapters = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_purge);
#ifdef SKIP_ASM
void cReplayFrameList_reset(void* self);



/**
 * Frees the data block of each of the MaxFrames frames and empties FrameList and FreeFrameList.
 */
void cReplay::purge()
{
    int i;
    for (i = 0; i < MaxFrames; i++) {
        cReplayFramePtr_purgeBlock((cReplayFramePtr*)(&((&Frames[0]))[i]));
    }
    cReplayFrameList_reset((char*)&FrameList);
    cReplayFrameList_reset((char*)&FreeFrameList);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_frameSync__FPv);
#ifdef SKIP_ASM
/**
 * Waits for pending frame transfers: a no-op on the PS2.
 *
 * An empty stub, as in On Tour. cReplay_reset and the destructor cReplay__cReplay call it before touching the frames.
 */
void cReplay_frameSync(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_setReplayState__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the replay state (State, +0x0).
 *
 * @param val  the new state (On Tour tReplayState)
 */
void cReplay::setReplayState(int val)
{
    State = val;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void cReplay_setReplayState(void* self, int state);
struct sFrameDesc_F4A8 {
    short type;
    char a;
    char b;
};
// PORT: local views of callees declared with other parameter types later in the unit
extern "C" void* cReplay_saveReplayFrame_F4A8(void* self, sFrameDesc_F4A8* desc) __asm__("cReplay_saveReplayFrame");
// PORT: cReplay_addReplayFrame is mangled with one parameter (__FPv) but is called here
// with (self, block); bind the 2-arg form to the symbol.
void cReplay_addReplayFrame_F4A8(void* self, void* block) __asm__("cReplay_addReplayFrame__FPv");
extern "C" void cReplay_startReplay_F4A8(void* self, int a1, int a2, int a3) __asm__("cReplay_startReplay");
extern "C" void cReplay_updateInput(void* self, int a1);
extern "C" void cReplay_processInput(cReplay* self);
extern "C" void cReplay_purgeIdleFrames(void* self);
extern "C" void cReplay_cancelMarkInstantReplay(void* self);
extern "C" void cReplay_stopInstantReplay(void* self);
extern "C" void cReplay_cancelMarkHighlights(void* self);
int cInput_getState(void* obj, int id);
struct sApp_F4A8 {
    char pad0[0xB0];
    void* players[4];       // 0xB0
};
// PORT: typed view of the app global
extern sApp_F4A8* SSXApp_F4A8 __asm__("SSXApp");


static inline void addFrame_F4A8(cReplay* self, short type)
{
    sFrameDesc_F4A8 desc;
    desc.type = type;
    desc.a = 0;
    desc.b = 0;
    cReplay_addReplayFrame_F4A8(self, cReplay_saveReplayFrame_F4A8(self, &desc));
}

/**
 * Runs one frame of the replay state machine: records the race, plays instant replays and drives replay playback (On
 * Tour cReplay::update).
 *
 * Does nothing while paused or uninitialised (State 14 / 15). Counts ShowError down, clears NextFrame, turns Cmd 7
 * into kPlaybackInput (3) and clears SkipFrame outside states 3 / 4. Then by State:
 * - 11 (kStartRace) moves to 12; 12 and 0 (recording) set state 0 and clear SaveFrame. On EndReplay (ignored once a
 *   replay is Loaded) it cancels the instant-replay and highlight marks, purges idle frames, saves a final full frame
 *   and enters kReplayEnd (13). On StartInstantReplay it starts the replay at InstantReplayFrame and enters
 *   kInstantPlayback (2). Otherwise every 60 ticks without ReplayError it purges idle frames and saves a frame, a
 *   full one (type 1) at tick 0 and every SaveFrameTicks ticks; EndRecordTick (without ReplayError) and RecordTick
 *   advance.
 * - 2 (instant replay) reads input (cReplay_updateInput, updateInput) and runs processInput until PlaybackTick reaches
 *   RecordTick minus 1, then stops the instant replay and falls into recording.
 * - 3 (kPlaybackInput) reads input when the replay pad (PadNum) has button state 0x6B, then processInput.
 * - 9 starts an auto replay (startReplay(0, 0, 1)), enters kGamePlayback (1) and sets AutoReplay; 8 starts a replay
 *   from the beginning and, like 4..7, enters kPlaybackInput.
 * - 10 and 13 do nothing; any other state (1) runs processInput.
 */
// Replay recorder/player: per-frame update of the replay state machine.
void cReplay::update()
{
    int state = State;
    bool ok = !(state == 14 || state == 15);
    if (!ok)
        return;
    if (ShowError > 0)
        ShowError--;
    NextFrame = 0;
    if (Cmd == 7)
        setReplayState(3);
    if (!(State == 3 || State == 4))
        SkipFrame = 0;
    switch (State) {
    case 11:
        setReplayState(12);
        return;
    case 2:
        if (PlaybackTick < RecordTick - 1) {
            cReplay_updateInput((this), 0);
            processInput();
            return;
        }
        cReplay_stopInstantReplay((this));
    case 0:
    case 12:
        setReplayState(0);
        SaveFrame = 0;
        if (EndReplay != 0) {
            if (Loaded != 0)
                return;
            cReplay_cancelMarkInstantReplay((this));
            cReplay_cancelMarkHighlights((this));
            cReplay_purgeIdleFrames((this));
            addFrame_F4A8((this), 1);
            setReplayState(13);
            return;
        }
        if (StartInstantReplay != 0) {
            cReplay_startReplay_F4A8((this), (int)InstantReplayFrame, 1, 0);
            setReplayState(2);
            StartInstantReplay = 0;
            InstantReplay = 1;
            return;
        }
        if (ReplayError == 0 && RecordTick % 60 == 0) {
            cReplay_purgeIdleFrames((this));
            if (RecordTick == 0 || (SaveFrameTicks != 0 && RecordTick % SaveFrameTicks == 0))
                addFrame_F4A8((this), 1);
            else
                addFrame_F4A8((this), 0);
        }
        if (ReplayError == 0)
            EndRecordTick++;
        RecordTick++;
        return;
    case 3: {
        void* input = SSXApp_F4A8->players[PadNum];
        if (input != 0 && cInput_getState(input, 107))
            cReplay_updateInput((this), 0);
        processInput();
        return;
    }
    case 9:
        cReplay_startReplay_F4A8((this), 0, 0, 1);
        setReplayState(1);
        AutoReplay = 1;
        return;
    case 8:
        cReplay_startReplay_F4A8((this), 0, 0, 0);
    case 4:
    case 5:
    case 6:
    case 7:
        setReplayState(3);
        return;
    default:
        processInput();
    case 10:
    case 13:
        return;
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_startRace);
#ifdef SKIP_ASM
void cReplay_setReplayState(void* self, int state);

/**
 * Marks the race start for the replay: moves the state from kPreRace (10) to kStartRace (11).
 *
 * Any other state is left alone.
 */
extern "C" void cReplay_startRace(int* self)
{
    int replayState = *self;
    bool ok = !(replayState == 14 || replayState == 15);
    if (ok) {
        if (replayState == 10) {
            cReplay_setReplayState(self, 11);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_pushReplaySaveFrame);
#ifdef SKIP_ASM

extern "C" void* cReplay_saveReplayFrame(void* self, tReplayFrameType* desc);
// PORT: cReplay_addReplayFrame is mangled with one parameter (__FPv) but is called here
// with (self, block); bind the 2-arg form to the symbol.
void cReplay_addReplayFrame_2(void* self, void* block) __asm__("cReplay_addReplayFrame__FPv");

/**
 * Saves the live game into SaveFrame (a full, type 1 frame) so it can be restored after a replay; does nothing when
 * one is already saved.
 *
 * The frame is also added to the replay (cReplay_addReplayFrame, addReplayFrame).
 */
void cReplay::pushReplaySaveFrame()
{
    if (SaveFrame == 0) {
        tReplayFrameType desc;
        desc.Type = 1;
        desc.Highlights = 0;
        desc.Chapter = 0;
        void* block = cReplay_saveReplayFrame(this, &desc);
        SaveFrame = (cReplayFrame*)block;
        cReplay_addReplayFrame_2(this, block);
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_popReplaySaveFrame);
#ifdef SKIP_ASM
extern "C" void cReplay_restoreReplayFrame(void* self, void* p);
extern "C" void cReplay_freeReplayFrame(void* self, void* p);

/**
 * Restores the live game from SaveFrame and frees that frame, when one was saved.
 */
void cReplay::popReplaySaveFrame()
{
    void* frame = SaveFrame;
    if (frame != 0) {
        cReplay_restoreReplayFrame(this, frame);
        cReplay_freeReplayFrame(this, SaveFrame);
        SaveFrame = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_setReplayPadNum__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the replay's PadNum, the controller whose pad drives the replay controls.
 *
 * @param val  the pad number
 */
void cReplay::setReplayPadNum(int val)
{
    PadNum = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/frameptr", cReplay_startReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_setupReplayCameras(cReplay* self);
extern "C" void cReplay_setupInstantReplayCameras(cReplay* self);
extern "C" void cReplay_pushReplaySaveFrame(cReplay* self);
void cReplay_setReplayState(void* self, int state);
extern "C" void cReplay_rewind(void* self, void* a1);
extern "C" void cReplay_stepFrame(void* self, int a1);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F200(void* self);


/**
 * Starts playing back the replay from @p frame, unless the replay is paused (14) or not set up (15).
 *
 * Saves the live game first unless a loaded replay is playing (cReplay_pushReplaySaveFrame), sets state 7
 * (kPlaybackStart), clears Cmd and AutoReplay, stores @p instantReplay, rewinds to @p frame and steps one frame. An
 * instant replay sets up its cameras with cReplay_setupInstantReplayCameras, any other with cReplay_setupReplayCameras. A replay that is
 * neither instant nor automatic also calls the sound bank manager's func_0028F200. AutoReplay is cleared even when @p
 * autoReplay is set.
 *
 * @param frame  the frame to start from (On Tour cReplayFrame*)
 * @param instantReplay  nonzero for an instant replay
 * @param autoReplay  nonzero for an automatic (attract) replay; only decides the sound call
 */
void cReplay::startReplay(void* frame, int instantReplay, int autoReplay)
{
    int ok = !(State == 14 || State == 15);
    if (ok) {
        if (Loaded == 0) {
            pushReplaySaveFrame();
        }
        setReplayState(7);
        Cmd = 0;
        AutoReplay = 0;
        InstantReplay = instantReplay;
        cReplay_rewind((this), frame);
        cReplay_stepFrame((this), 1);
        if (instantReplay) {
            setupInstantReplayCameras();
        } else {
            setupReplayCameras();
        }
        if (instantReplay == 0 && autoReplay == 0) {
            func_0028F200(cAudioInterface_bxAudio());
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/frameptr", cReplay_stopReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_setupGameCamerasOnExitReplay(cReplay* self);
extern "C" void cReplay_setupGameCamerasOnExitInstantReplay(void* self);
void cReplay_setReplayState(void* self, int state);
extern "C" void cReplay_popReplaySaveFrame(cReplay* self);
extern "C" void cReplay_cancelMarkInstantReplay(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F2C0(void* self);


/**
 * Stops playback, unless the replay is paused (14) or not set up (15).
 *
 * Cancels a marked instant replay, calls the sound bank manager's func_0028F2C0 unless the replay was automatic or
 * instant, and clears AutoReplay, InstantReplay and StartInstantReplay. Unless a loaded replay is playing, restores
 * the live game (cReplay_popReplaySaveFrame), gives the cameras back and sets state 13 (kReplayEnd) when EndReplay is
 * set, else 0 (kGameRecording). InstantReplay is cleared before it is tested, so cReplay_setupGameCamerasOnExitInstantReplay (the instant-replay
 * camera exit) runs only if restoring the save frame sets InstantReplay again.
 */
void cReplay::stopReplay()
{
    int ok = !(State == 14 || State == 15);
    if (ok) {
        cReplay_cancelMarkInstantReplay((this));
        if (AutoReplay == 0 && InstantReplay == 0) {
            func_0028F2C0(cAudioInterface_bxAudio());
        }
        AutoReplay = 0;
        InstantReplay = 0;
        StartInstantReplay = 0;
        if (Loaded == 0) {
            popReplaySaveFrame();
            if (InstantReplay) {
                cReplay_setupGameCamerasOnExitInstantReplay((this));
            } else {
                setupGameCamerasOnExitReplay();
            }
            if (EndReplay) {
                setReplayState(13);
            } else {
                setReplayState(0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_endReplay);
#ifdef SKIP_ASM
/**
 * Requests the end of the recording: sets EndReplay unless the replay is paused or uninitialised (State 14 / 15).
 * cReplay_update acts on it.
 */
void cReplay::endReplay()
{
    int curState = State;
    bool ok = !(curState == 0xE || curState == 0xF);
    if (ok) {
        EndReplay = 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_updateCameraSelect);
#ifdef SKIP_ASM
extern int camSubset[];
int cInput_getState(void* pad, int button);
// PORT: cInput_getValue returns the input axis value as a float ($f0).
extern "C" float cInput_getValue(void* pad, int axis);
extern "C" void cGameViewMan_updateAll(void* list);

static inline bool isStateFA78(int* self, int s)
{
    return *self == s;
}

/**
 * Lets the replay viewer change the camera while a replay plays.
 *
 * Only in state 3. Reads the replay pad (SSXApp's input at +0xB0 + PadNum * 4): input state 0x6C refreshes every game
 * view (cGameViewMan_updateAll). When the current replay camera's algorithm (camSubset[CurrentReplayCamIndex], On Tour
 * camSubset) is 0xB, any movement on axes 0x30..0x33 (cInput_getValue) refreshes the views too.
 */
extern "C" void cReplay_updateCameraSelect(int* self)
{
    if (!isStateFA78(self, 3)) {
        return;
    }
    void* input = (void*)SSXApp->Inputs[self[0x28 / 4]];
    if (cInput_getState(input, 108) != 0) {
        cGameViewMan_updateAll(SSXApp->Game->ViewMan);
    }
    if (camSubset[self[1584 / 4]] != 11) {
        return;
    }
    if (cInput_getValue(input, 49) != 0.0f || cInput_getValue(input, 0x30) != 0.0f ||
        cInput_getValue(input, 50) != 0.0f || cInput_getValue(input, 51) != 0.0f) {
        cGameViewMan_updateAll(SSXApp->Game->ViewMan);
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_updateInput);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char gUISoundNull;
extern "C" void cGame_stopReplay(void* self);

struct sFpViewVEntry_FB88 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sFpView_FB88 {
    sFpViewVEntry_FB88* vt;
};

static inline bool isStateFB88(char* s, int st)
{
    return *(int*)s == st;
}

static inline bool isActiveFB88(char* s)
{
    int st = *(int*)s;
    return st != 0 && st < 10;
}

static inline bool isIdleFB88(char* vvs)
{
    cReplay* vs = (cReplay*)vvs;
    cReplay* s = (cReplay*)vs;
    bool r = false;
    if (isActiveFB88(((char*)s))) r = s->PlaybackTick == s->EndRecordTick;
    return r;
}

static inline bool isIdleNoCamFB88(char* s)
{
    bool r = false;
    if (isActiveFB88(s)) r = *(int*)(s + 0x484) == 0;
    return r;
}

// PORT: returns whether a menu action was queued; the unit declares it
// void earlier, so the definition goes through an asm label.
extern "C" int cReplay_updateInput_FB88(void* self, int a1) __asm__("cReplay_updateInput");
extern "C" int cReplay_updateInput_impl(void* self, int a1) __asm__("cReplay_updateInput");
/**
 * Reads the replay menu buttons into the next replay command.
 *
 * Does nothing while a command is pending (Cmd, +0x618). Outside auto and instant replays (+0x61C/+0x620): input 0x6A
 * (SkipFrameTicks = 10, SkipFrame = 0) and 0x6B (counts SkipFrameTicks down and toggles SkipFrame, acting once both are
 * 0) give command 5, 0x67 command 6, 0x68 command 4 and 0x69 command 3; each gives 7 outside state 3 and none when the
 * playback cursor is already at the end (0x69: at the start). 0x66 stops the replay through cGame (cGame_stopReplay, Cmd
 * 8) when @p debug is set, else queues 7 outside state 3, and returns 0; 0x6C cycles the camera algorithm
 * (cReplay_cycleReplayCameraAlgorithm). Menu sounds go through the front end's sound interface (gUISoundNull when there
 * is none), vtable slot 2. Stores the command in Cmd.
 *
 * @param debug  nonzero to stop the replay at once on input 0x66 (On Tour: bool debug)
 * @return nonzero when a command was queued
 */
extern "C" int cReplay_updateInput_impl(void* self, int debug)
{
    cReplay* replay = (cReplay*)self;
    cSSXApp* app = SSXApp;
    sFpView_FB88* uiSound = (sFpView_FB88*)&gUISoundNull;
    sFpView_FB88* cur = (sFpView_FB88*)app->Game->UI->mpSound;
    if (cur != 0) uiSound = cur;
    if (replay->Cmd != 0) return 0;
    int command = 0;
    void* input = (void*)app->Inputs[replay->PadNum];
    if (replay->AutoReplay == 0 && replay->InstantReplay == 0) {
        if (cInput_getState(input, 106)) {
            replay->SkipFrameTicks = 10;
            replay->SkipFrame = 0;
            if (!isStateFB88(((char*)replay), 3)) command = 7;
            else command = isIdleFB88(((char*)replay)) ? 0 : 5;
        } else if (cInput_getState(input, 107)) {
            if (replay->SkipFrameTicks > 0) replay->SkipFrameTicks -= 1;
            replay->SkipFrame ^= 1;
            if (replay->SkipFrameTicks <= 0 && replay->SkipFrame == 0) {
                if (!isStateFB88(((char*)replay), 3)) command = 7;
                else command = isIdleFB88(((char*)replay)) ? 0 : 5;
            }
        } else if (cInput_getState(input, 103)) {
            if (!isStateFB88(((char*)replay), 3)) command = 7;
            else command = isIdleFB88(((char*)replay)) ? 0 : 6;
        } else if (cInput_getState(input, 102)) {
            if (debug) {
                cGame_stopReplay(SSXApp->Game);
                replay->Cmd = 8;
            } else if (!isStateFB88(((char*)replay), 3)) {
                replay->Cmd = 7;
            }
            return 0;
        } else if (cInput_getState(input, 104)) {
            if (!isStateFB88(((char*)replay), 3)) command = 7;
            else if (!isIdleFB88(((char*)replay))) {
                uiSound->vt[2].fn(vthis(uiSound, uiSound->vt[2]), 2);
                command = 4;
            } else {
                uiSound->vt[2].fn(vthis(uiSound, uiSound->vt[2]), 4);
            }
        } else if (cInput_getState(input, 105)) {
            if (isStateFB88(((char*)replay), 3)) {
                if (!isIdleNoCamFB88(((char*)replay))) {
                    uiSound->vt[2].fn(vthis(uiSound, uiSound->vt[2]), 2);
                    command = 3;
                } else {
                    uiSound->vt[2].fn(vthis(uiSound, uiSound->vt[2]), 4);
                }
            } else {
                command = 7;
            }
        } else if (cInput_getState(input, 108)) {
            uiSound->vt[2].fn(vthis(uiSound, uiSound->vt[2]), 2);
            ((cReplay*)self)->cycleReplayCameraAlgorithm();
        }
    }
    replay->Cmd = command;
    return command != 0;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplay_processInput);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cGameViewMan_updateAll(void* list);
// PORT: cReplay_findFrame is declared (void*) in replay.cpp but takes the frame index too
extern "C" void cReplay_lastFrame(void* self);
void* cReplay_findFrame_2(void* self, int frame) __asm__("cReplay_findFrame__FPv");
extern "C" void cReplay_prevFrame(void* self);
extern "C" void cReplay_nextFrame(void* self);
extern "C" void cChaseCameraController_switchCamera_Chapter(void* p);


/**
 * Applies the pending replay-menu command (Cmd, set by updateInput) and clears it (On Tour processInput).
 *
 * - 1: rewinds to the start; 2: jumps to the last frame; 3: goes back to the previous key frame (to the start of the
 *   frame PlaybackTick is in, or the one before when already at a frame start). Each then steps once with setframe
 *   and enters kPlaybackStepUpdate (6) at tick 0, else kPlaybackUpdate (5).
 * - 4: when PlaybackFrame has a next frame, moves to it, steps with setframe and enters kPlaybackUpdate.
 * - 5: steps one frame and enters kPlaybackStep (4).
 * - 6: toggles pause: kPlaybackInput (3) goes to kGamePlayback (1), any other state to kPlaybackInput.
 * - 7: enters kPlaybackInput; 8: nothing.
 * - 0 (and any other value): unless paused (State 3), sets NextFrame and steps one frame.
 *
 * Commands 1 to 4 also redraw: they switch the first view's chase camera (cChaseCameraController_switchCamera_Chapter
 * on ViewMan's PlayerCamera[0]->ChaseController), call its snapCurrent (virtual slot 4) and update all views
 * (cGameViewMan_updateAll).
 */
void cReplay::processInput()
{
    int redraw = 0;
    switch (Cmd) {
    case 1:
        cReplay_rewind(((char*)this), 0);
        redraw = 1;
        cReplay_stepFrame(((char*)this), 1);
        if (PlaybackTick == 0) {
            cReplay_setReplayState(((char*)this), 6);
        } else {
            cReplay_setReplayState(((char*)this), 5);
        }
        break;
    case 2:
        cReplay_lastFrame(((char*)this));
        redraw = 1;
        cReplay_stepFrame(((char*)this), 1);
        if (PlaybackTick == 0) {
            cReplay_setReplayState(((char*)this), 6);
        } else {
            cReplay_setReplayState(((char*)this), 5);
        }
        break;
    case 3: {
        redraw = 1;
        cReplayFrame* frame = (cReplayFrame*)((char*)cReplay_findFrame_2(((char*)this), PlaybackTick));
        PlaybackFrame = (cReplayFrame*)frame;
        if (PlaybackTick == frame->FrameStartTick) {
            cReplay_prevFrame(((char*)this));
        } else {
            cReplayFrame* n = frame->Next;
            if (((char*)n) != 0 && n->FrameStartTick == PlaybackTick) {
                cReplay_prevFrame(((char*)this));
            }
        }
        cReplay_stepFrame(((char*)this), 1);
        if (PlaybackTick == 0) {
            cReplay_setReplayState(((char*)this), 6);
        } else {
            cReplay_setReplayState(((char*)this), 5);
        }
        break;
    }
    case 4:
        redraw = 1;
        if ((int)PlaybackFrame->Next != 0) {
            cReplay_nextFrame(((char*)this));
            cReplay_stepFrame(((char*)this), 1);
            cReplay_setReplayState(((char*)this), 5);
        }
        break;
    case 5:
        cReplay_stepFrame(((char*)this), 0);
        cReplay_setReplayState(((char*)this), 4);
        break;
    case 6:
        if (State == 3) {
            cReplay_setReplayState(((char*)this), 1);
        } else {
            cReplay_setReplayState(((char*)this), 3);
        }
        break;
    case 7:
        cReplay_setReplayState(((char*)this), 3);
        break;
    case 8:
        break;
    default:
        if (State != 3) {
            NextFrame = 1;
            cReplay_stepFrame(((char*)this), 0);
        }
        break;
    }
    Cmd = 0;
    if (redraw) {
        cGameViewMan* views = SSXApp->Game->ViewMan;
        cChaseCameraController* controller = views->PlayerCamera[0]->ChaseController;
        cChaseCameraController_switchCamera_Chapter(controller);
        // slot 4: snapCurrent (cCameraController_snapCurrent)
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)controller->mVtbl;
        vt[4].fn(vthis(controller, vt[4]));
        cGameViewMan_updateAll(views);
    }
}
#endif

