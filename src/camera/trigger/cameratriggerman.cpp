#include "common.h"
#include "camera/trigger/cameratriggerman.h"
#include "ai/rider.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_cleanupOnExit);
#ifdef SKIP_ASM
struct cActiveTriggerList;
struct cCameraTriggerList;
struct cCameraTriggerStack;
void cCameraTriggerStack_init(cCameraTriggerStack* self);
void cActiveTriggerList_purge(cActiveTriggerList* self);
void cCameraTriggerList_init(cCameraTriggerList* self);
extern "C" void cCameraTriggerMan_purge(cCameraTriggerMan* self);
extern "C" void cCamera_resetChaseControllerSwitches();

/**
 * Clears all camera trigger state when leaving a track.
 *
 * Purges both active trigger lists (+0x14, +0x18), resets both trigger stacks (+0x1C, +0x3C), purges the
 * manager, re-initialises its trigger list and resets the chase camera controller switches.
 */
extern "C" void cCameraTriggerMan_cleanupOnExit(cCameraTriggerMan* self)
{
    cActiveTriggerList_purge((cActiveTriggerList*)&self->ActiveTriggerList);
    cActiveTriggerList_purge((cActiveTriggerList*)(&self->ActiveTriggerList[1]));
    cCameraTriggerStack_init((cCameraTriggerStack*)&self->CameraTriggerStack);
    cCameraTriggerStack_init((cCameraTriggerStack*)(&self->CameraTriggerStack[1]));
    cCameraTriggerMan_purge(self);
    cCameraTriggerList_init((cCameraTriggerList*)self);
    cCamera_resetChaseControllerSwitches();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CA08);
#ifdef SKIP_ASM
/**
 * Does nothing; nothing calls it or stores its address.
 */
extern "C" void func_0016CA08(void) {

}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_loadTriggersIfNotStreaming__FPv);
#ifdef SKIP_ASM
/**
 * Would load the camera triggers from a file when the track is not streamed; empty in this build.
 *
 * cGame_loadTrack calls it on gCamTriggerMan with the track's ".cam" file name as a second argument, which the
 * declaration leaves out. The triggers arrive through the stream instead (cCameraTriggerMan_resolve, cCameraTriggerMan_streamIn).
 */
void cCameraTriggerMan_loadTriggersIfNotStreaming(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CA18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void operator_delete(int*);
extern "C" void cCameraTriggerList_reset(void* self);

struct sCtVec4_CA18 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sCtBox_CA18 {
    sCtVec4_CA18 min;
    sCtVec4_CA18 max;
};
struct sCtCell_CA18 {
    int level;
    int x, y, z;
};
struct sCtNode_CA18 {
    sCtNode_CA18* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sCtRoot_CA18 {
    sCtCell_CA18 cell;              // 0x00
    sCtNode_CA18* node;             // 0x10
};

extern "C" void nash_Nash_findkey_CA18(void* out, const void* box) __asm__("nash_Nash_findkey");
extern "C" void nash_Cell_remnode_CA18(void* node, int idx, void* item, const void* target, const void* cur) __asm__("nash_Cell_remnode");

class cCtShape_CA18 {
public:
    char pad[0x24];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual sCtBox_CA18 bounds();
};

struct sCtTrig_CA18 {
    int f0;
    unsigned int mask;              // 0x4
    cCtShape_CA18* shape;           // 0x8
    char padC[0x8];
    char item[0xC];                 // 0x14
    sCtTrig_CA18* reg;              // 0x20
};

struct sCtMan_CA18 {
    sCtTrig_CA18** trigs;           // 0x0
    int count;                      // 0x4
};

static inline void remove_CA18(char* w, void* item, const sCtBox_CA18* box)
{
    (*(int*)(w + 0xA0))++;
    sCtCell_CA18 c;
    nash_Nash_findkey_CA18(&c, box);
    sCtRoot_CA18* r = (sCtRoot_CA18*)w;
    sCtRoot_CA18* rx = c.x < 0 ? r + 4 : r;
    sCtRoot_CA18* ry = c.y < 0 ? rx + 2 : rx;
    sCtRoot_CA18* rz = c.z < 0 ? ry + 1 : ry;
    sCtRoot_CA18* q = rz;
    nash_Cell_remnode_CA18(q->node, 2, item, &c, q);
    if (q->node->empty()) {
        operator_delete((int*)q->node);
        q->node = 0;
    }
}

extern "C" void func_0016CA18_impl(void* vself) __asm__("func_0016CA18");
/**
 * Takes every camera trigger out of the world's space hash and resets the trigger list, both trigger stacks and the
 * chase camera switches.
 *
 * For each trigger in the list (array +0x0, count +0x4) it gets the bounds of the trigger's Volume (+0x8, virtual slot
 * 6), bumps the counter at +0xA0 of SSXApp->Game->BXNash, finds the box's cell key (nash_Nash_findkey), picks the root
 * octant by the signs of its x, y and z, removes the trigger's space node (+0x14) from node list 2 of that root cell
 * (nash_Cell_remnode) and frees the root cell when it is left empty. Then both trigger stacks (+0x1C, +0x3C) are
 * emptied (cCameraTriggerStack_init), the list is reset (cCameraTriggerList_reset; its array is kept) and
 * cCamera_resetChaseControllerSwitches runs.
 *
 * Unlike cCameraTriggerMan_purge it does not check the registration word (+0x20) before removing a trigger and leaves
 * the active lists alone. Its only caller is func_0016CEA8.
 *
 * @param vself  the camera trigger manager (this)
 */
extern "C" void func_0016CA18_impl(void* vself)
{
    sCtMan_CA18* self = (sCtMan_CA18*)vself;
    for (int i = 0; i < self->count; i++) {
        sCtTrig_CA18* trigger = self->trigs[i];
        char* w = (char*)SSXApp->Game->BXNash;
        sCtBox_CA18 box = trigger->shape->bounds();
        remove_CA18(w, trigger->item, &box);
    }
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)vself + 0x1C));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)vself + 0x3C));
    cCameraTriggerList_reset(vself);
    cCamera_resetChaseControllerSwitches();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_purge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void operator_delete(int*);
extern "C" void cCameraTriggerList_purge(void** self);

struct sCtVec4_P {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sCtBox_P {
    sCtVec4_P min;
    sCtVec4_P max;
};
struct sCtCell_P {
    int level;
    int x, y, z;
};
struct sCtNode_P {
    sCtNode_P* child[2][2][2];      // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sCtRoot_P {
    sCtCell_P cell;                 // 0x00
    sCtNode_P* node;                // 0x10
};

extern "C" void nash_Nash_findkey_P(void* out, const void* box) __asm__("nash_Nash_findkey");
extern "C" void nash_Cell_remnode_P(void* node, int idx, void* item, const void* target, const void* cur) __asm__("nash_Cell_remnode");

class cCtShape_P {
public:
    char pad[0x24];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual sCtBox_P bounds();
};

struct sCtTrig_P {
    int f0;
    unsigned int mask;              // 0x4
    cCtShape_P* shape;              // 0x8
    char padC[0x8];
    char item[0xC];                 // 0x14
    sCtTrig_P* reg;                 // 0x20
};

struct sCtMan_P {
    sCtTrig_P** trigs;              // 0x0
    int count;                      // 0x4
};

static inline void remove_P(char* w, void* item, const sCtBox_P* box)
{
    (*(int*)(w + 0xA0))++;
    sCtCell_P c;
    nash_Nash_findkey_P(&c, box);
    sCtRoot_P* r = (sCtRoot_P*)w;
    sCtRoot_P* rx = c.x < 0 ? r + 4 : r;
    sCtRoot_P* ry = c.y < 0 ? rx + 2 : rx;
    sCtRoot_P* rz = c.z < 0 ? ry + 1 : ry;
    sCtRoot_P* q = rz;
    nash_Cell_remnode_P(q->node, 2, item, &c, q);
    if (q->node->empty()) {
        operator_delete((int*)q->node);
        q->node = 0;
    }
}

/**
 * Takes every registered camera trigger out of the world's space hash, then frees the trigger list and clears
 * the trigger stacks and active lists.
 *
 * Each trigger with a registration (+0x20) is removed, with its shape's bounds, from SSXApp->Game->BXNash;
 * the registration word is left as it was. Then both trigger stacks (+0x1C, +0x3C) are reset, the trigger
 * list is reset and its array freed (cCameraTriggerList_purge), both active lists (+0x14, +0x18) are purged and the
 * chase controller switches are reset.
 *
 * @param vself  the camera trigger manager (this)
 */
// Takes every registered trigger out of the world's space hash and clears the trigger stacks and active lists.
extern "C" void cCameraTriggerMan_purge(cCameraTriggerMan* vself)
{
    sCtMan_P* self = (sCtMan_P*)vself;
    for (int i = 0; i < self->count; i++) {
        sCtTrig_P* trigger = self->trigs[i];
        if (trigger->reg != 0) {
            char* w = (char*)SSXApp->Game->BXNash;
            sCtBox_P box = trigger->shape->bounds();
            remove_P(w, trigger->item, &box);
        }
    }
    cCameraTriggerStack_init((cCameraTriggerStack*)&vself->CameraTriggerStack);
    cCameraTriggerStack_init((cCameraTriggerStack*)(&vself->CameraTriggerStack[1]));
    cCameraTriggerList_purge((void**)vself);
    cActiveTriggerList_purge((cActiveTriggerList*)&vself->ActiveTriggerList);
    cActiveTriggerList_purge((cActiveTriggerList*)(&vself->ActiveTriggerList[1]));
    cCamera_resetChaseControllerSwitches();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_loadTriggers);
#ifdef SKIP_ASM
struct sCamTrigMan {
    char** triggers; // 0x0
    int count;       // 0x4
};

extern "C" int cCameraTriggerList_loadFromFile(void* self, const char* file);

/**
 * Loads the camera triggers from a file and clears the word at +0x20 of each loaded trigger.
 *
 * @param file  the trigger file name
 * @return 0 on success, otherwise cCameraTriggerList_loadFromFile()'s error code
 */
extern "C" int cCameraTriggerMan_loadTriggers(sCamTrigMan* self, const char* file)
{
    int err = cCameraTriggerList_loadFromFile(self, file);
    if (err != 0)
        return err;
    for (int i = 0; i < self->count; i++)
        *(int*)(self->triggers[i] + 0x20) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_streamIn);
#ifdef SKIP_ASM
struct sCamTrigManB {
    char** triggers; // 0x0
    int count;       // 0x4
};

extern "C" int cCameraTriggerList_loadFromBuffer(void* self, void* buf);
void cCameraTriggerMan_setInGameTriggers(void* self);

/**
 * Replaces the camera triggers with the ones in a streamed-in trigger file and activates the in-game set.
 *
 * Clears all trigger state (cCameraTriggerMan_cleanupOnExit), loads the triggers from the buffer
 * (cCameraTriggerList_loadFromBuffer; its result is ignored), clears each trigger's registration word (+0x20)
 * and calls cCameraTriggerMan_setInGameTriggers().
 *
 * @param stream  the trigger file's data in memory (On Tour fileBuffer)
 */
extern "C" void cCameraTriggerMan_streamIn(cCameraTriggerMan* self, void* stream)
{
    cCameraTriggerMan_cleanupOnExit(self);
    cCameraTriggerList_loadFromBuffer(self, stream);
    sCamTrigManB* manager = (sCamTrigManB*)self;
    for (int i = 0; i < manager->count; i++)
        *(int*)(manager->triggers[i] + 0x20) = 0;
    cCameraTriggerMan_setInGameTriggers(self);
}
#endif

extern "C" void* cCameraTriggerMan_resolve(void*, void*, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CEA8);
#ifdef SKIP_ASM
extern "C" void func_0016CA18();

/**
 * Resets the camera trigger manager in its argument register: a plain call of func_0016CA18.
 *
 * The manager pointer passes through untouched in $a0 (the declaration leaves it out). Nothing calls it or stores its
 * address.
 */
extern "C" void func_0016CEA8(void) {
    func_0016CA18();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", gCamTriggerMan_resolve__FPvi);
#ifdef SKIP_ASM
// PORT: the unit declares cCameraTriggerMan_resolve as returning void*; this caller treats it as void.
void cCameraTriggerMan_resolve_CEC8(void*, void*, int) __asm__("cCameraTriggerMan_resolve");
/**
 * Stream-resolve hook for camera trigger chunks: hands a loaded buffer to the global camera trigger manager.
 *
 * Calls cCameraTriggerMan_resolve(gCamTriggerMan, @p self, @p pFileBuffer). Despite its name, @p self is not an object: it is the
 * chunk's resource id (On Tour rid).
 *
 * @param pFileBuffer  the loaded chunk data
 */
void gCamTriggerMan_resolve(void* self, int pFileBuffer)
{
    cCameraTriggerMan_resolve_CEC8((void*)((void**)&gCamTriggerMan), self, pFileBuffer);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_resolve);
#ifdef SKIP_ASM
extern "C" void cCameraTriggerMan_streamIn(cCameraTriggerMan* self, void* stream);

// PORT: the unit declares cCameraTriggerMan_resolve as returning void*, but the body returns nothing.
void cCameraTriggerMan_resolve_impl(void* self, void* unused, void* stream) __asm__("cCameraTriggerMan_resolve");
/**
 * Resolves a streamed camera trigger chunk: reads the triggers out of the buffer.
 *
 * Calls cCameraTriggerMan_streamIn with @p stream when it is not null; the resource id is ignored.
 *
 * @param rid  the chunk's resource id (On Tour rid), not used
 * @param stream  the loaded chunk data
 */
void cCameraTriggerMan_resolve_impl(void* self, void* rid, void* stream)
{
    if (stream != 0) {
        cCameraTriggerMan_streamIn((cCameraTriggerMan*)self, stream);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CF18);
#ifdef SKIP_ASM
// PORT: cCameraTriggerMan_unresolve__FPv is mangled as (void*), but this caller passes two args.
void cCameraTriggerMan_unresolve_2(void* mgr, void* self) __asm__("cCameraTriggerMan_unresolve__FPv");

/**
 * Stream-unresolve hook for camera trigger chunks: hands the chunk to the global camera trigger manager.
 *
 * Calls cCameraTriggerMan_unresolve (an empty method) with gCamTriggerMan and @p self. Despite its name, @p self is not an object:
 * cPS2ChunkResolve_unresolve passes the chunk's resource id for stream data type 17, the counterpart of
 * gCamTriggerMan_resolve.
 */
extern "C" void func_0016CF18(void* self)
{
    cCameraTriggerMan_unresolve_2((void*)((void**)&gCamTriggerMan), self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_unresolve__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the camera trigger manager's handler for a trigger chunk being unloaded.
 *
 * func_0016CF18 calls it with gCamTriggerMan and the chunk's resource id (a second argument the declaration leaves
 * out); the triggers are only cleared when new ones stream in (cCameraTriggerMan_streamIn).
 */
void cCameraTriggerMan_unresolve(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_setTriggers);
#ifdef SKIP_ASM
void operator_delete(int*);

struct sCtVec4_CF48 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sCtBox_CF48 {
    sCtVec4_CF48 min;
    sCtVec4_CF48 max;
};
struct sCtCell_CF48 {
    int level;
    int x, y, z;
};
struct sCtNode_CF48 {
    sCtNode_CF48* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sCtRoot_CF48 {
    sCtCell_CF48 cell;              // 0x00
    sCtNode_CF48* node;             // 0x10
};

extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void nash_Cell_remnode(void* node, int idx, void* item, const void* target, const void* cur);
extern "C" void nash_Octant_addnode(void* root, int idx, void* item, const void* target);

class cCtShape_CF48 {
public:
    char pad[0x24];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual sCtBox_CF48 bounds();
};

struct sCtTrig_CF48 {
    int f0;
    unsigned int mask;              // 0x4
    cCtShape_CF48* shape;           // 0x8
    char padC[0x8];
    char item[0xC];                 // 0x14
    sCtTrig_CF48* reg;              // 0x20
};

struct sCtMan_CF48 {
    sCtTrig_CF48** trigs;           // 0x0
    int count;                      // 0x4
};

static inline sCtRoot_CF48* findRoot_CF48(char* w, int x, int y, int z)
{
    sCtRoot_CF48* r = (sCtRoot_CF48*)w;
    sCtRoot_CF48* rx = x >= 0 ? r : r + 4;
    sCtRoot_CF48* ry = y >= 0 ? rx : rx + 2;
    sCtRoot_CF48* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void remove_CF48(char* w, void* item, const sCtBox_CF48* box)
{
    (*(int*)(w + 0xA0))++;
    sCtCell_CF48 c;
    nash_Nash_findkey(&c, box);
    sCtRoot_CF48* r = findRoot_CF48(w, c.x, c.y, c.z);
    nash_Cell_remnode(r->node, 2, item, &c, &r->cell);
    if (r->node->empty()) {
        operator_delete((int*)r->node);
        r->node = 0;
    }
}

static inline void insert_CF48(char* w, void* item, const sCtBox_CF48* box)
{
    sCtCell_CF48 c;
    nash_Nash_findkey(&c, box);
    nash_Octant_addnode(findRoot_CF48(w, c.x, c.y, c.z), 2, item, &c);
}

// PORT: the unit declares cCameraTriggerMan_setTriggers as returning void*; it returns nothing.
extern "C" void cCameraTriggerMan_setTriggers_impl(void* vself, int mask) __asm__("cCameraTriggerMan_setTriggers");

/**
 * Activates the camera triggers that belong to a game mode and deactivates the rest.
 *
 * For each trigger whose mode mask shares a bit with @p mask and is not yet registered, registers it and adds its
 * node to SSXApp->Game's BXNash grid using its volume's bounding box; a registered trigger outside the mask is
 * removed from the grid and unregistered.
 *
 * @param vself  the trigger manager (this)
 * @param mask   the game-mode bits to enable (On Tour eCTGameMode: 1 replay, otherwise in-game)
 */
extern "C" void cCameraTriggerMan_setTriggers_impl(void* vself, int mask)
{
    sCtMan_CF48* self = (sCtMan_CF48*)vself;
    for (int i = 0; i < self->count; i++) {
        sCtTrig_CF48* trigger = self->trigs[i];
        if (trigger->mask & mask) {
            if (trigger->reg == 0) {
                trigger->reg = trigger;
                char* w = (char*)SSXApp->Game->BXNash;
                sCtBox_CF48 box = trigger->shape->bounds();
                insert_CF48(w, trigger->item, &box);
            }
        } else if (trigger->reg != 0) {
            char* w = (char*)SSXApp->Game->BXNash;
            sCtBox_CF48 box = trigger->shape->bounds();
            remove_CF48(w, trigger->item, &box);
            trigger->reg = 0;
        }
    }
}
#endif

extern "C" void* cCameraTriggerMan_setTriggers(void* self, int type);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_setInGameTriggers__FPv);
#ifdef SKIP_ASM
/**
 * Activates the in-game camera triggers.
 *
 * Calls cCameraTriggerMan_setTriggers(self, 2): every trigger whose mask has bit 1 set is registered with its bounds in
 * SSXApp->Game->BXNash (if it is not already), and every other registered trigger is removed from it.
 */
void cCameraTriggerMan_setInGameTriggers(void* self)
{
    cCameraTriggerMan_setTriggers(self, 2);
}
#endif

//99.29%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_setReplayTriggers__FPv);
#ifdef SKIP_ASM
/**
 * Switches the camera triggers to the replay set.
 *
 * Calls cCameraTriggerMan_setTriggers(self, 1): every trigger whose mode mask has bit 0 set is registered in
 * SSXApp->Game->BXNash (if it is not already) and every other registered trigger is removed from it.
 *
 * @return no meaningful value (cCameraTriggerMan_setTriggers returns nothing)
 */
void* cCameraTriggerMan_setReplayTriggers(void* self)
{
    return cCameraTriggerMan_setTriggers(self, 1);
}
#endif


//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_getTriggerList);
#ifdef SKIP_ASM
/**
 * Returns its argument: the camera trigger manager's trigger list, which sits at offset 0.
 *
 * Nothing calls it.
 *
 * @param triggerMan  the camera trigger manager (this)
 * @return @p triggerMan, the address of the trigger list
 */
extern "C" int cCameraTriggerMan_getTriggerList(int triggerMan) {
    return triggerMan;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerStack_init__FP19cCameraTriggerStack);
#ifdef SKIP_ASM
/**
 * Empties a camera trigger stack by zeroing its first two words.
 */
void cCameraTriggerStack_init(cCameraTriggerStack* self)
{
    self->NumTriggers = 0;
    self->LastEnterPlayed = 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerStack_cCameraTriggerStack);
#ifdef SKIP_ASM
void cCameraTriggerStack_init(cCameraTriggerStack* self);

/**
 * Constructs a camera trigger stack: runs cCameraTriggerStack_init on it.
 *
 * @return @p self
 */
extern "C" cCameraTriggerStack* cCameraTriggerStack_cCameraTriggerStack(cCameraTriggerStack* self)
{
    cCameraTriggerStack_init(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerStack_recordEnter__FPvi);
#ifdef SKIP_ASM
/**
 * Pushes a trigger id onto a camera trigger stack and records it as the last entered trigger.
 *
 * Stores @p triggerId at TriggerIDStack[count] (+0x8 + 4 x count), in the last-entered word (+0x4) and increments the
 * count (+0x0). There is no check against the six slots. Called by cCameraTriggerMan_checkCollisions when the rider enters a trigger.
 *
 * @param triggerId  the entered trigger's id (On Tour triggerID)
 */
void cCameraTriggerStack_recordEnter(void* vself, int triggerId)
{
    cCameraTriggerStack* self = (cCameraTriggerStack*)vself;
    *(int*)((char*)self + (*(int*)self << 2) + 0x8) = triggerId;
    self->LastEnterPlayed = triggerId;
    *(int*)self = *(int*)self + 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerStack_recordExitGetNext);
#ifdef SKIP_ASM
struct sCamTrigStack {
    unsigned int count; // 0x0
    int top; // 0x4
    int items[1]; // 0x8
};

/**
 * Removes an exited trigger from a camera trigger stack and says which trigger, if any, should now take over.
 *
 * Deletes the first entry equal to @p key, closing the gap. If entries remain, the top one is compared with the last
 * entered trigger (+0x4): when they differ *@p changed = 1 and *@p out = the top id, otherwise *@p changed = 0; either
 * way it returns 0. When the stack is empty *@p changed = 0 and it returns 1.
 *
 * cCameraTriggerMan_checkCollisions runs the exited trigger's exit action on 1, and the new top trigger's enter action when @p changed is
 * set.
 *
 * @param key  the exited trigger's id (On Tour triggerID)
 * @param changed  receives 1 when another trigger's enter action should be played (On Tour playEnter_outArg)
 * @param out  receives that trigger's id (On Tour enterToPlay_outArg)
 * @return 1 when the stack is now empty, else 0
 */
extern "C" int cCameraTriggerStack_recordExitGetNext(sCamTrigStack* self, int key, int* changed, int* out)
{
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        if (self->items[i] == key) {
            unsigned int j;
            for (j = i + 1; j < self->count; j++) {
                self->items[j - 1] = self->items[j];
            }
            self->count--;
            break;
        }
    }
    if (self->count != 0) {
        int newTop = self->items[self->count - 1];
        if (newTop == self->top) {
            *changed = 0;
        } else {
            *changed = 1;
            *out = newTop;
        }
        return 0;
    }
    *changed = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_checkCollisions);
#ifdef SKIP_ASM
extern "C" void cActiveTriggerList_clearFlags(void* list);
extern "C" void* cActiveTriggerList_find(void* list, void* key);
extern "C" void* cActiveTriggerList_find1(void* list, int key);
extern "C" void cActiveTriggerList_add(void* list, void* item);
extern "C" void* cActiveTriggerList_remove(void* list, void* node);
extern "C" void descend(void* node, void* ctx);
extern "C" void nash_privy_QueryHelper_helpX(char* hit, void* ctx, void* ctx2, void* box, void* node, void* root);
extern "C" int nash_CellKey_proximitytest(void* root, void* box);
// PORT: the unit defines doTriggerAction later with typed params; bind this call by asm label.
extern "C" void doTriggerAction_D320(void* evt, char* rider) __asm__("doTriggerAction");

struct sVEnt_D320 {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct sCtx_D320 {
    void* list;
    char* rider;
};

struct sQuad_D320 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBox_D320 {
    sQuad_D320 min;
    sQuad_D320 max;
};

struct sRoot_D320 {
    int level;
    int x, y, z;
    void* node;
};

static inline void walk_D320(void* node, sCtx_D320* ctx)
{
    char* e;
    for (e = *(char**)((char*)node + 0x28); e != 0; e = *(char**)e) {
        if (*(int*)(e + 8) == 4) {
            char* trig = *(char**)(e + 0xC);
            char* sub = ctx->rider + 0x6C0;
            char* bound = *(char**)(trig + 8);
            sVEnt_D320* cvt = *(sVEnt_D320**)sub;
            sVEnt_D320* bvt = *(sVEnt_D320**)(bound + 0x24);
            char* bthis = bound + bvt[7].delta;
            int r = cvt[5].fn(sub + cvt[5].delta);
            if (bvt[7].fn(bthis, r)) {
                void* f = cActiveTriggerList_find(ctx->list, trig);
                if (f) {
                    *(int*)((char*)f + 0xC) = 1;
                } else {
                    cActiveTriggerList_add(ctx->list, trig);
                }
            }
        }
    }
    if (*(void**)((char*)node + 0x0)) descend(*(void**)((char*)node + 0x0), ctx);
    if (*(void**)((char*)node + 0x4)) descend(*(void**)((char*)node + 0x4), ctx);
    if (*(void**)((char*)node + 0x8)) descend(*(void**)((char*)node + 0x8), ctx);
    if (*(void**)((char*)node + 0xC)) descend(*(void**)((char*)node + 0xC), ctx);
    if (*(void**)((char*)node + 0x10)) descend(*(void**)((char*)node + 0x10), ctx);
    if (*(void**)((char*)node + 0x14)) descend(*(void**)((char*)node + 0x14), ctx);
    if (*(void**)((char*)node + 0x18)) descend(*(void**)((char*)node + 0x18), ctx);
    if (*(void**)((char*)node + 0x1C)) descend(*(void**)((char*)node + 0x1C), ctx);
}

static inline void collect_D320(sRoot_D320* r, char* hits, int k, sCtx_D320* ctx, sBox_D320* box)
{
    void* node = r->node;
    char* hit = hits + k;
    int c = nash_CellKey_proximitytest(r, box);
    if (c == 0) {
        walk_D320(node, ctx);
    } else if (c == 2) {
        nash_privy_QueryHelper_helpX(hit, ctx, ctx, box, node, r);
    }
}

static inline sQuad_D320* getPos_D320(char* sub)
{
    sVEnt_D320* vt = *(sVEnt_D320**)sub;
    return (sQuad_D320*)vt[5].fn(sub + vt[5].delta);
}

static inline int active_D320(char* rider)
{
    return *(int*)(rider + 0x874) != 0 && *(int*)(rider + 0x87C) != 0;
}

/**
 * Updates which camera triggers a human rider is inside and plays the enter and exit camera actions.
 *
 * Only for riders with mIsHuman (+0x874) and mIsLocal (+0x87C) set. Uses the active trigger list and trigger stack of
 * the rider's view (mViewIndex +0x870: lists at +0x14, stacks at +0x1C, 0x20 bytes each). Clears the list's flags
 * (cActiveTriggerList_clearFlags), then queries the eight root cells of SSXApp->Game->BXNash with a box at the rider's
 * position (rider +0x6C0 slot 5): nash_CellKey_proximitytest decides per root whether the cell lies inside the box (0: walk it all
 * with the collision callback and descend) or crosses it (2: nash_privy_QueryHelper_helpX; because of that test's quirks it
 * practically always answers 2 here); each trigger whose volume contains the position is flagged or added.
 *
 * Then every node not flagged this frame is a trigger the rider left: cCameraTriggerStack_recordExitGetNext updates the stack, and its exit
 * action (+0x10) runs through doTriggerAction when the stack became empty, or the enter action (+0xC) of the new top
 * trigger (found with cActiveTriggerList_find1) when the top changed; the node is removed (cActiveTriggerList_remove). Finally
 * each node still marked new (+0x8) is pushed onto the stack (cCameraTriggerStack_recordEnter) and its enter action runs.
 *
 * Called by cRider_checkObjectCollisions on gCamTriggerMan.
 *
 * @param rider  the rider (On Tour cRider&)
 */
extern "C" void cCameraTriggerMan_checkCollisions(cCameraTriggerMan* self, cRider* rider)
{
    if (active_D320(((char*)rider))) {
        int idx = rider->mViewIndex;
        cActiveTriggerList_clearFlags(&self->ActiveTriggerList[idx]);
        sCtx_D320 ctx;
        sRoot_D320* w = (sRoot_D320*)SSXApp->Game->BXNash;
        ctx.list = (char*)(idx * 4 + (int)self) + 0x14;
        ctx.rider = ((char*)rider);
        char* sub = (char*)&rider->mVtbl;
        sQuad_D320* riderPosMin = getPos_D320(sub);
        sQuad_D320* riderPosMax = getPos_D320(sub);
        sBox_D320 box;
        box.min = *riderPosMin;
        box.max = *riderPosMax;
        // PORT: eight 1-byte hit flags kept in one 8-byte slot (relies on sizeof(long) == 8).
        long hits;
        sCtx_D320* pc = &ctx;
        sRoot_D320* root0 = &w[0];
        if (w[0].node) collect_D320(root0, (char*)&hits, 0, pc, &box);
        sRoot_D320* root1 = &w[1];
        if (w[1].node) collect_D320(root1, (char*)&hits, 1, pc, &box);
        sRoot_D320* root2 = &w[2];
        if (w[2].node) collect_D320(root2, (char*)&hits, 2, pc, &box);
        sRoot_D320* root3 = &w[3];
        if (w[3].node) collect_D320(root3, (char*)&hits, 3, pc, &box);
        sRoot_D320* root4 = &w[4];
        if (w[4].node) collect_D320(root4, (char*)&hits, 4, pc, &box);
        sRoot_D320* root5 = &w[5];
        if (w[5].node) collect_D320(root5, (char*)&hits, 5, pc, &box);
        sRoot_D320* root6 = &w[6];
        if (w[6].node) collect_D320(root6, (char*)&hits, 6, pc, &box);
        sRoot_D320* root7 = &w[7];
        if (w[7].node) collect_D320(root7, (char*)&hits, 7, pc, &box);

        int changed;
        int top;
        char* n = *(char**)((char*)(idx * 4 + (int)self) + 0x14);
        while (n != 0) {
            if (*(int*)(n + 0xC) == 0) {
                char* trigger = *(char**)n;
                if (cCameraTriggerStack_recordExitGetNext((sCamTrigStack*)&self->CameraTriggerStack[idx], *(int*)trigger, &changed, &top)) {
                    doTriggerAction_D320(*(void**)(trigger + 0x10), ((char*)rider));
                } else if (changed) {
                    char* topActive = (char*)cActiveTriggerList_find1(&self->ActiveTriggerList[idx], top);
                    if (topActive) {
                        doTriggerAction_D320(*(void**)(topActive + 0xC), ((char*)rider));
                    }
                }
                n = (char*)cActiveTriggerList_remove(&self->ActiveTriggerList[idx], n);
            } else {
                n = *(char**)(n + 4);
            }
        }
        for (char* listNode = *(char**)((char*)(idx * 4 + (int)self) + 0x14); listNode != 0; listNode = *(char**)(listNode + 4)) {
            if (*(int*)(listNode + 8)) {
                cCameraTriggerStack_recordEnter(&self->CameraTriggerStack[idx], **(int**)listNode);
                doTriggerAction_D320(*(void**)(*(char**)listNode + 0xC), ((char*)rider));
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", doTriggerAction);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cChaseCameraController_switchCamera_Volume(void* self, int mode, int arg, float blend);

struct sCamEvt_E1D8 {
    int type;
    int pad4;
    unsigned int id;
    float dur;
};

struct sCams_E1D8 {
    int count;
    char* cam[1];
};

/**
 * Plays a camera trigger action for a rider: switches the rider's chase camera to the camera the action names.
 *
 * The chase controller is +0xA8 of the camera SSXApp->Game->ViewMan lists for the rider's mViewIndex (+0x870). The
 * blend rate is 1 / (BlendTime (+0xC) x 60) per frame, or 1 (a cut) when BlendTime is 0. By the action's Type (+0x0, On
 * Tour eCTActionType): 0 (switch) maps the action's SwitchType (+0x8, eCTSwitchType 1..75) to a camera algorithm type
 * (mostly the same number; 0, 43 and unknown values give 0) and calls cChaseCameraController_switchCamera_Volume(type,
 * 0, rate); 1 (bounded) and 2 (spline) call it with type 91 or 92 and the action itself. Type 3 (none) does nothing.
 *
 * Called by cCameraTriggerMan_checkCollisions with a trigger's enter or exit action.
 *
 * @param evt  the camera trigger action (On Tour cCTAction*)
 * @param rider  the rider whose camera to switch (On Tour cRider&)
 */
extern "C" void doTriggerAction(sCamEvt_E1D8* evt, cRider* rider)
{
    sCams_E1D8* cams = (sCams_E1D8*)SSXApp->Game->ViewMan;
    void* cam = *(void**)(cams->cam[rider->mViewIndex] + 0xA8);
    switch (evt->type) {
    case 2: {
        float rate;
        if (evt->dur != 0.0f)
            rate = 1.0f / evt->dur * 0.016666668f;
        else
            rate = 1.0f;
        cChaseCameraController_switchCamera_Volume(cam, 92, (int)evt, rate);
        break;
    }
    case 1: {
        float rate;
        if (evt->dur != 0.0f)
            rate = 1.0f / evt->dur * 0.016666668f;
        else
            rate = 1.0f;
        cChaseCameraController_switchCamera_Volume(cam, 91, (int)evt, rate);
        break;
    }
    case 0: {
        float rate = 0.0f;
        if (evt->dur == rate)
            rate = 1.0f;
        else
            rate = 1.0f / evt->dur * 0.016666668f;
        int mode;
        switch (evt->id) {
        case 3: mode = 4; break;
        case 7: mode = 3; break;
        case 1: mode = 2; break;
        case 2: mode = 1; break;
        case 4: mode = 7; break;
        case 5: mode = 5; break;
        case 6: mode = 6; break;
        case 8: mode = 8; break;
        case 10: mode = 9; break;
        case 9: mode = 10; break;
        case 11: mode = 11; break;
        case 12: mode = 12; break;
        case 13: mode = 13; break;
        case 14: mode = 14; break;
        case 15: mode = 15; break;
        case 16: mode = 16; break;
        case 17: mode = 17; break;
        case 18: mode = 18; break;
        case 19: mode = 19; break;
        case 20: mode = 20; break;
        case 21: mode = 21; break;
        case 22: mode = 22; break;
        case 23: mode = 23; break;
        case 24: mode = 24; break;
        case 25: mode = 25; break;
        case 26: mode = 26; break;
        case 27: mode = 27; break;
        case 28: mode = 28; break;
        case 29: mode = 29; break;
        case 30: mode = 30; break;
        case 31: mode = 31; break;
        case 32: mode = 32; break;
        case 33: mode = 41; break;
        case 34: case 35: case 36: case 37: case 38: case 39: case 40: case 41: mode = evt->id - 1; break;
        case 44: mode = 42; break;
        case 45: mode = 43; break;
        case 46: mode = 44; break;
        case 47: mode = 45; break;
        case 48: mode = 46; break;
        case 49: mode = 47; break;
        case 50: mode = 0x30; break;
        case 51: mode = 49; break;
        case 52: mode = 50; break;
        case 53: mode = 51; break;
        case 54: mode = 52; break;
        case 55: mode = 53; break;
        case 56: mode = 54; break;
        case 57: mode = 55; break;
        case 58: mode = 0x38; break;
        case 59: mode = 57; break;
        case 60: mode = 58; break;
        case 61: mode = 59; break;
        case 62: mode = 0x3C; break;
        case 63: mode = 61; break;
        case 64: mode = 0x3E; break;
        case 65: mode = 0x3F; break;
        case 66: mode = 0x40; break;
        case 67: mode = 65; break;
        case 68: mode = 66; break;
        case 69: mode = 67; break;
        case 70: mode = 68; break;
        case 71: mode = 69; break;
        case 72: mode = 70; break;
        case 73: mode = 71; break;
        case 74: mode = 72; break;
        case 75: mode = 73; break;
        case 42: mode = 76; break;
        case 0:
        case 43:
        default: mode = 0; break;
        }
        cChaseCameraController_switchCamera_Volume(cam, mode, 0, rate);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCTBoundObjEllipse_limitPoint);
#ifdef SKIP_ASM
struct sCtmV3_E560 {
    float x, y, z;
    sCtmV3_E560() {}
    sCtmV3_E560(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

inline sCtmV3_E560 operator-(const sCtmV3_E560& a)
{
    return sCtmV3_E560(-a.x, -a.y, -a.z);
}

struct sCtmV_E560 {
    float x, y, z, w;
    sCtmV_E560() {}
    sCtmV_E560(const sCtmV3_E560& v, float aw) : x(v.x), y(v.y), z(v.z), w(aw) {}
    sCtmV_E560(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCtmM_E560 {
    float m[4][4];
    sCtmM_E560() {}
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_E560(const sCtmM_E560& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_E560& operator=(const sCtmM_E560& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

extern sCtmM_E560 D_004FF1A0_E560 __asm__("t4mat_Identity");
extern sCtmV_E560 D_004FF140_E560 __asm__("t4vect_XAxis");
extern sCtmV_E560 D_004FF150_E560 __asm__("t4vect_YAxis");
extern sCtmV_E560 D_004FF160_E560 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void ctmMulMtx_E560(sCtmM_E560* dst, const sCtmM_E560* a, const sCtmM_E560* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void ctmTransMtx_E560(sCtmM_E560* m, const sCtmV_E560& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
static inline sCtmM_E560 ctmScaleMtx_E560(const sCtmM_E560* m, const sCtmV_E560& v)
{
    sCtmM_E560 d;
    sCtmM_E560* dst = &d;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf1, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulx.xyzw   $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw   $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw   $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw   $vf11, $vf7, $vf1w\n"
        "sqc2         $vf8, 0x0(%0)\n"
        "sqc2         $vf9, 0x10(%0)\n"
        "sqc2         $vf10, 0x20(%0)\n"
        "sqc2         $vf11, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(m), "m"(v)
        : "memory");
    return d;
}

// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sCtmV_E560 ctmApplyMtx_E560(const sCtmM_E560* m, const sCtmV_E560& v)
{
    sCtmV_E560 out;
    __asm__(
        ".set noreorder\n"
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set reorder\n"
        : "=m"(out)
        : "r"(m), "m"(v)
        : "memory");
    return out;
}

static inline void ctmRotAxis_E560(sCtmM_E560* m, const sCtmV_E560& axis, float angle)
{
    float s;
    float c;
    float one = 1.0f;
    fSinCos(&s, &c, angle);
    sCtmM_E560 r;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    ctmMulMtx_E560(m, m, &r);
}


// PORT: PS2-only VU0 inline asm (normalize xyzw with vrsqrt).
static inline sCtmV_E560 ctmNormalize_E560(const sCtmV_E560& v)
{
    sCtmV_E560 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

/**
 * Keeps a point inside an ellipse camera bound object: a point outside is pulled onto the ellipsoid's surface along the
 * line to its centre.
 *
 * Maps the point into unit-sphere space with the inverse transform (scale by 1 / scale +0x18..+0x20, rotate by -heading
 * about Z, -roll about Y, -pitch about X, translate by -position). If its length is below 1 the point is left alone;
 * otherwise it is normalised, mapped back with the forward transform (translate, pitch, roll, heading, scale) and
 * written back with w = 1. This is a radial projection, not the closest surface point. Slot 6 (limitPoint) of the
 * ellipse bound-object vtable cCTBoundObjEllipse_vtbl.
 *
 * @param src  the ellipse bound object (this)
 * @param point  the world point to limit; overwritten when outside
 */
// Pushes *p out to the surface of the trigger's ellipsoid when it lies inside: into trigger space
// (inverse of cCTBoundObjLine_worldPoint1's transform), normalize, and back to world space.
extern "C" void cCTBoundObjEllipse_limitPoint(char* src, sCtmV_E560* point)
{
    sCtmM_E560 xform(D_004FF1A0_E560);
    float one = 1.0f;
    xform = ctmScaleMtx_E560(&xform, sCtmV_E560(one / *(float*)(src + 0x18), one / *(float*)(src + 0x1C), one / *(float*)(src + 0x20), one));
    ctmRotAxis_E560(&xform, D_004FF160_E560, -*(float*)(src + 0xC));
    ctmRotAxis_E560(&xform, D_004FF150_E560, -*(float*)(src + 0x14));
    ctmRotAxis_E560(&xform, D_004FF140_E560, -*(float*)(src + 0x10));
    ctmTransMtx_E560(&xform, sCtmV_E560(-*(sCtmV3_E560*)(src + 0x0), one));
    sCtmV_E560 localPt = ctmApplyMtx_E560(&xform, *point);
    localPt.w = 0.0f;
    if (localPt.x * localPt.x + localPt.y * localPt.y + localPt.z * localPt.z < one) {
        return;
    }
    localPt = ctmNormalize_E560(localPt);
    localPt.w = one;
    xform = D_004FF1A0_E560;
    ctmTransMtx_E560(&xform, sCtmV_E560(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis_E560(&xform, D_004FF140_E560, *(float*)(src + 0x10));
    ctmRotAxis_E560(&xform, D_004FF150_E560, *(float*)(src + 0x14));
    ctmRotAxis_E560(&xform, D_004FF160_E560, *(float*)(src + 0xC));
    xform = ctmScaleMtx_E560(&xform, sCtmV_E560(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    *point = ctmApplyMtx_E560(&xform, localPt);
    point->w = one;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCTBoundObjBox_limitPoint);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern "C" void fSinCos(float* sout, float* cout, float x);

extern t4Vector D_004FF140_F068 __asm__("t4vect_XAxis");
extern t4Vector D_004FF150_F068 __asm__("t4vect_YAxis");
extern t4Vector D_004FF160_F068 __asm__("t4vect_ZAxis");
extern t4Matrix D_004FF1A0_F068 __asm__("t4mat_Identity");

struct sVec3_F068 {
    float x, y, z;
    sVec3_F068() {}
    sVec3_F068(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct sVec4_F068 : t4Vector
{
    sVec4_F068() {}
    sVec4_F068(const float& ax, const float& ay, const float& az, const float& aw) { x = ax; y = ay; z = az; w = aw; }
    sVec4_F068(const sVec3_F068& v, const float& aw) { x = v.x; y = v.y; z = v.z; w = aw; }
} __attribute__((aligned(16)));

struct sMtx_F068;

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, d = b * d).
static inline void vu0MulMatEq_F068(sMtx_F068* d, const sMtx_F068* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0Copy_F068(sMtx_F068* d, const sMtx_F068* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

struct sMtx_F068 {
    float m[4][4];
    sMtx_F068() {}
    sMtx_F068(const sMtx_F068& o) { vu0Copy_F068(this, &o); }
    sMtx_F068& operator=(const sMtx_F068& o)
    {
        vu0Copy_F068(this, &o);
        return *this;
    }
    sMtx_F068& operator*=(const sMtx_F068& b)
    {
        vu0MulMatEq_F068(this, &b);
        return *this;
    }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector into the last matrix row).
static inline void vu0MulVecRow3_F068(sMtx_F068* m, const t4Vector& v)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, %1\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, 0x30(%0)\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector, vector loaded first).
static inline t4Vector vu0MulVec2_F068(const sMtx_F068* m, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_F068(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (d -= b).
static inline void SubEq_F068(t4Vector& d, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(d)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_F068(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_F068(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

struct sXform_F068 {
    sVec3_F068 pos;     // 0x0
    float rot[3];       // 0xC
    sVec3_F068 size;    // 0x18
};

// Multiply by a rotation of `angle` about the unit axis `a` (Rodrigues).
#define ROT_F068(R, a, angle) \
    { \
        const float* ax = a; \
        float s, c; \
        fSinCos(&s, &c, angle); \
        float t = 1.0f - c; \
        float x = ax[0]; \
        float y = ax[1]; \
        float z = ax[2]; \
        float tx = t * x; \
        float ty = t * y; \
        float tz = t * z; \
        float sx = s * x; \
        float sy = s * y; \
        float sz = s * z; \
        R.m[0][0] = tx * x + c; \
        R.m[1][0] = tx * y + sz; \
        R.m[2][0] = tx * z - sy; \
        R.m[0][1] = ty * x - sz; \
        R.m[1][1] = ty * y + c; \
        R.m[2][1] = ty * z + sx; \
        R.m[0][2] = tz * x + sy; \
        R.m[1][2] = tz * y - sx; \
        R.m[2][2] = tz * z + c; \
        R.m[3][0] = 0.0f; \
        R.m[3][1] = 0.0f; \
        R.m[3][2] = 0.0f; \
        R.m[0][3] = 0.0f; \
        R.m[1][3] = 0.0f; \
        R.m[2][3] = 0.0f; \
        R.m[3][3] = 1.0f; \
    }

extern int D_004A4DE8;              // guard of the static axis table below
extern t4Vector D_004C5450[3];      // static local: the three box axes

/**
 * Keeps a point inside a box camera bound object: a point outside is moved to the nearest point of the box.
 *
 * Starts from t4mat_Identity, multiplies in rotations about Z by -heading (rot[0]), about Y by -roll (rot[2]) and about
 * X by -pitch (rot[1]) and a translation by -position, and maps @p point into box space (w = 1). On each axis of the static
 * table D_004C5450 (X, Y and Z axes, filled once under the guard D_004A4DE8) the coordinate is clamped to [-size,
 * +size] (the scale at +0x18 is a half-extent). The result is mapped back (translate, pitch, roll, heading) and stored
 * in @p point with w = 1. Slot 6 (limitPoint) of the box bound-object vtable cCTBoundObjBox_vtbl.
 *
 * @param t  the box bound object (this): position, heading/pitch/roll and half-extents
 * @param point  the world point to limit; overwritten
 */
// cCTBoundObjBox: move p to the closest point of the (rotated) box.
extern "C" void cCTBoundObjBox_limitPoint(const sXform_F068* t, t4Vector* point)
{
    sMtx_F068 xform = (const sMtx_F068&)D_004FF1A0_F068;
    {
        sMtx_F068* xformPtr = &xform;
        sMtx_F068 R;
        ROT_F068(R, &D_004FF160_F068.x, -t->rot[0]);
        *xformPtr *= R;
    }
    {
        sMtx_F068* xformPtr = &xform;
        sMtx_F068 R;
        ROT_F068(R, &D_004FF150_F068.x, -t->rot[2]);
        *xformPtr *= R;
    }
    {
        sMtx_F068* xformPtr = &xform;
        sMtx_F068 R;
        ROT_F068(R, &D_004FF140_F068.x, -t->rot[1]);
        *xformPtr *= R;
    }
    vu0MulVecRow3_F068(&xform, sVec4_F068(sVec3_F068(-t->pos.x, -t->pos.y, -t->pos.z), 1.0f));
    t4Vector localPt = vu0MulVec2_F068(&xform, *point);
    localPt.w = 1.0f;
    t4Vector dmin = Sub_F068(localPt, sVec4_F068(sVec3_F068(-t->size.x, -t->size.y, -t->size.z), 1.0f));
    t4Vector dmax = Sub_F068(localPt, sVec4_F068(t->size.x, t->size.y, t->size.z, 1.0f));
    if (D_004A4DE8 == 0) {
        D_004C5450[0] = D_004FF140_F068;
        D_004C5450[1] = D_004FF150_F068;
        D_004C5450[2] = D_004FF160_F068;
        D_004A4DE8 = 1;
    }
    for (int i = 0; i < 3; i++) {
        t4Vector n = Scale_F068(D_004C5450[i], -1.0f);
        float planeDist = Dot_F068(n, dmin);
        if (planeDist > 0.0f) {
            SubEq_F068(localPt, Scale_F068(n, planeDist));
        }
        planeDist = Dot_F068(D_004C5450[i], dmax);
        if (planeDist > 0.0f) {
            SubEq_F068(localPt, Scale_F068(D_004C5450[i], planeDist));
        }
    }
    xform = (const sMtx_F068&)D_004FF1A0_F068;
    vu0MulVecRow3_F068(&xform, sVec4_F068(t->pos, 1.0f));
    {
        sMtx_F068* xformPtr = &xform;
        sMtx_F068 R;
        ROT_F068(R, &D_004FF140_F068.x, t->rot[1]);
        *xformPtr *= R;
    }
    {
        sMtx_F068* xformPtr = &xform;
        sMtx_F068 R;
        ROT_F068(R, &D_004FF150_F068.x, t->rot[2]);
        *xformPtr *= R;
    }
    {
        sMtx_F068* xformPtr = &xform;
        sMtx_F068 R;
        ROT_F068(R, &D_004FF160_F068.x, t->rot[0]);
        *xformPtr *= R;
    }
    *point = vu0MulVec2_F068(&xform, localPt);
    point->w = 1.0f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCTBoundObjLine_limitPoint);
#ifdef SKIP_ASM
struct sCtmV_FBB8 {
    float x, y, z, w;
    sCtmV_FBB8() {}
    sCtmV_FBB8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCtmM_FBB8 {
    float m[4][4];
    sCtmM_FBB8() {}
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_FBB8(const sCtmM_FBB8& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_FBB8& operator=(const sCtmM_FBB8& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

extern sCtmM_FBB8 D_004FF1A0_FBB8 __asm__("t4mat_Identity");
extern sCtmV_FBB8 D_004FF140_FBB8 __asm__("t4vect_XAxis");
extern sCtmV_FBB8 D_004FF150_FBB8 __asm__("t4vect_YAxis");
extern sCtmV_FBB8 D_004FF160_FBB8 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void ctmMulMtx_FBB8(sCtmM_FBB8* dst, const sCtmM_FBB8* a, const sCtmM_FBB8* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void ctmTransMtx_FBB8(sCtmM_FBB8* m, const sCtmV_FBB8& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
static inline sCtmM_FBB8 ctmScaleMtx_FBB8(const sCtmM_FBB8* m, const sCtmV_FBB8& v)
{
    sCtmM_FBB8 d;
    sCtmM_FBB8* dst = &d;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf1, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulx.xyzw   $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw   $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw   $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw   $vf11, $vf7, $vf1w\n"
        "sqc2         $vf8, 0x0(%0)\n"
        "sqc2         $vf9, 0x10(%0)\n"
        "sqc2         $vf10, 0x20(%0)\n"
        "sqc2         $vf11, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(m), "m"(v)
        : "memory");
    return d;
}

// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sCtmV_FBB8 ctmApplyMtx_FBB8(const sCtmM_FBB8* m, const sCtmV_FBB8& v)
{
    sCtmV_FBB8 out;
    __asm__(
        ".set noreorder\n"
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set reorder\n"
        : "=m"(out)
        : "r"(m), "m"(v)
        : "memory");
    return out;
}

static inline void ctmRotAxis_FBB8(sCtmM_FBB8* m, const sCtmV_FBB8& axis, float angle)
{
    float s;
    float c;
    float one = 1.0f;
    fSinCos(&s, &c, angle);
    sCtmM_FBB8 r;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    ctmMulMtx_FBB8(m, m, &r);
}


// PORT: PS2-only VU0 inline asm (a - b).
static inline sCtmV_FBB8 ctmSub_FBB8(const sCtmV_FBB8& a, const sCtmV_FBB8& b)
{
    sCtmV_FBB8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sCtmV_FBB8 ctmAdd_FBB8(const sCtmV_FBB8& a, const sCtmV_FBB8& b)
{
    sCtmV_FBB8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float ctmDot_FBB8(const sCtmV_FBB8& a, const sCtmV_FBB8& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sCtmV_FBB8 ctmScale_FBB8(const sCtmV_FBB8& v, float s)
{
    sCtmV_FBB8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

/**
 * Keeps a point on a line camera bound object: moves it to its perpendicular projection onto the line.
 *
 * Transforms both end points (+0x2C, +0x38) to world space (translate by position, rotate by pitch about X, roll about
 * Y and heading about Z, scale), then sets @p point = end1 + d x t with d = end2 - end1 and t = dot(@p point - end1, d) /
 * dot(d, d). t is not clamped, so the point can land beyond either end (and a zero-length line divides by zero). Slot 6
 * (limitPoint) of the line bound-object vtable cCTBoundObjLine_vtbl.
 *
 * @param src  the line bound object (this)
 * @param point  the world point to limit; overwritten
 */
// Projects *p onto the line through the transformed points src+0x2C and src+0x38
// (transform as in cCTBoundObjLine_worldPoint1).
extern "C" void cCTBoundObjLine_limitPoint(char* src, sCtmV_FBB8* point)
{
    sCtmM_FBB8 xform(D_004FF1A0_FBB8);
    float one = 1.0f;
    ctmTransMtx_FBB8(&xform, sCtmV_FBB8(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis_FBB8(&xform, D_004FF140_FBB8, *(float*)(src + 0x10));
    ctmRotAxis_FBB8(&xform, D_004FF150_FBB8, *(float*)(src + 0x14));
    ctmRotAxis_FBB8(&xform, D_004FF160_FBB8, *(float*)(src + 0xC));
    xform = ctmScaleMtx_FBB8(&xform, sCtmV_FBB8(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    sCtmV_FBB8 pts[2];
    float w1 = 1.0f;
    pts[0] = ctmApplyMtx_FBB8(&xform, sCtmV_FBB8(*(float*)(src + 0x2C), *(float*)(src + 0x30), *(float*)(src + 0x34), w1));
    pts[1] = ctmApplyMtx_FBB8(&xform, sCtmV_FBB8(*(float*)(src + 0x38), *(float*)(src + 0x3C), *(float*)(src + 0x40), w1));
    sCtmV_FBB8 lineDir = ctmSub_FBB8(pts[1], pts[0]);
    float lineParam = ctmDot_FBB8(ctmSub_FBB8(*point, pts[0]), lineDir) / ctmDot_FBB8(lineDir, lineDir);
    *point = ctmAdd_FBB8(pts[0], ctmScale_FBB8(lineDir, lineParam));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCTBoundObjLine_worldPoint1);
#ifdef SKIP_ASM
struct sCtmV {
    float x, y, z, w;
    sCtmV() {}
    sCtmV(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCtmM {
    float m[4][4];
    sCtmM() {}
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM(const sCtmM& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM& operator=(const sCtmM& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

extern sCtmM t4mat_Identity;
extern sCtmV t4vect_XAxis;
extern sCtmV t4vect_YAxis;
extern sCtmV t4vect_ZAxis;
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void ctmMulMtx(sCtmM* dst, const sCtmM* a, const sCtmM* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void ctmTransMtx(sCtmM* m, const sCtmV& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
static inline sCtmM ctmScaleMtx(const sCtmM* m, const sCtmV& v)
{
    sCtmM d;
    sCtmM* dst = &d;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf1, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulx.xyzw   $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw   $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw   $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw   $vf11, $vf7, $vf1w\n"
        "sqc2         $vf8, 0x0(%0)\n"
        "sqc2         $vf9, 0x10(%0)\n"
        "sqc2         $vf10, 0x20(%0)\n"
        "sqc2         $vf11, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(m), "m"(v)
        : "memory");
    return d;
}

// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sCtmV ctmApplyMtx(const sCtmM* m, const sCtmV& v)
{
    sCtmV out;
    __asm__(
        ".set noreorder\n"
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set reorder\n"
        : "=m"(out)
        : "r"(m), "m"(v)
        : "memory");
    return out;
}

static inline void ctmRotAxis(sCtmM* m, const sCtmV& axis, float angle)
{
    float s;
    float c;
    float one = 1.0f;
    fSinCos(&s, &c, angle);
    sCtmM r;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    ctmMulMtx(m, m, &r);
}

/**
 * Returns a line bound object's first end point in world space.
 *
 * Transforms Point[0] (+0x2C, w = 1) by translate(position +0x0), rotations by pitch (+0x10) about X, roll (+0x14)
 * about Y and heading (+0xC) about Z, and scale (+0x18). cChaseCameraController_createChaseAlgorithmBlend uses it with
 * cCTBoundObjLine_worldPoint2 to build the superpipe camera from a line bound object.
 *
 * @param src  the line bound object (this)
 * @return the world-space point, returned by value
 */
// World position of the point at src+0x2C: translate(src+0x0) * rotations(0x10, 0x14, 0xC) * scale(src+0x18).
extern "C" sCtmV cCTBoundObjLine_worldPoint1(char* src)
{
    sCtmM transform(t4mat_Identity);
    float one = 1.0f;
    ctmTransMtx(&transform, sCtmV(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis(&transform, t4vect_XAxis, *(float*)(src + 0x10));
    ctmRotAxis(&transform, t4vect_YAxis, *(float*)(src + 0x14));
    ctmRotAxis(&transform, t4vect_ZAxis, *(float*)(src + 0xC));
    transform = ctmScaleMtx(&transform, sCtmV(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    return ctmApplyMtx(&transform, sCtmV(*(float*)(src + 0x2C), *(float*)(src + 0x30), *(float*)(src + 0x34), one));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCTBoundObjLine_worldPoint2);
#ifdef SKIP_ASM
extern sCtmM t4mat_Identity;
extern sCtmV t4vect_XAxis;
extern sCtmV t4vect_YAxis;
extern sCtmV t4vect_ZAxis;
extern "C" void fSinCos(float* s, float* c, float angle);

/**
 * Returns a line bound object's second end point in world space.
 *
 * Same transform as cCTBoundObjLine_worldPoint1, applied to Point[1] (+0x38). Used by
 * cChaseCameraController_createChaseAlgorithmBlend for the superpipe camera.
 *
 * @param src  the line bound object (this)
 * @return the world-space point, returned by value
 */
// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
// PORT: PS2-only VU0 inline asm (out = m * v).
// World position of the point at src+0x38: translate(src+0x0) * rotations(0x10, 0x14, 0xC) * scale(src+0x18).
extern "C" sCtmV cCTBoundObjLine_worldPoint2(char* src)
{
    sCtmM transform(t4mat_Identity);
    float one = 1.0f;
    ctmTransMtx(&transform, sCtmV(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis(&transform, t4vect_XAxis, *(float*)(src + 0x10));
    ctmRotAxis(&transform, t4vect_YAxis, *(float*)(src + 0x14));
    ctmRotAxis(&transform, t4vect_ZAxis, *(float*)(src + 0xC));
    transform = ctmScaleMtx(&transform, sCtmV(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    return ctmApplyMtx(&transform, sCtmV(*(float*)(src + 0x38), *(float*)(src + 0x3C), *(float*)(src + 0x40), one));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCTSpline_worldPoint);
#ifdef SKIP_ASM
struct sCtmV3_0D20 {
    float x, y, z;
};

struct sCtmV_0D20 {
    float x, y, z, w;
    sCtmV_0D20() {}
    sCtmV_0D20(const sCtmV3_0D20& v, float aw) : x(v.x), y(v.y), z(v.z), w(aw) {}
    sCtmV_0D20(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCtmM_0D20 {
    float m[4][4];
    sCtmM_0D20() {}
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_0D20(const sCtmM_0D20& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_0D20& operator=(const sCtmM_0D20& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

extern sCtmM_0D20 D_004FF1A0_0D20 __asm__("t4mat_Identity");
extern sCtmV_0D20 D_004FF140_0D20 __asm__("t4vect_XAxis");
extern sCtmV_0D20 D_004FF150_0D20 __asm__("t4vect_YAxis");
extern sCtmV_0D20 D_004FF160_0D20 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void ctmMulMtx_0D20(sCtmM_0D20* dst, const sCtmM_0D20* a, const sCtmM_0D20* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void ctmTransMtx_0D20(sCtmM_0D20* m, const sCtmV_0D20& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
static inline sCtmM_0D20 ctmScaleMtx_0D20(const sCtmM_0D20* m, const sCtmV_0D20& v)
{
    sCtmM_0D20 d;
    sCtmM_0D20* dst = &d;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf1, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulx.xyzw   $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw   $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw   $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw   $vf11, $vf7, $vf1w\n"
        "sqc2         $vf8, 0x0(%0)\n"
        "sqc2         $vf9, 0x10(%0)\n"
        "sqc2         $vf10, 0x20(%0)\n"
        "sqc2         $vf11, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(m), "m"(v)
        : "memory");
    return d;
}

// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sCtmV_0D20 ctmApplyMtx_0D20(const sCtmM_0D20* m, const sCtmV_0D20& v)
{
    sCtmV_0D20 out;
    __asm__(
        ".set noreorder\n"
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set reorder\n"
        : "=m"(out)
        : "r"(m), "m"(v)
        : "memory");
    return out;
}

static inline void ctmRotAxis_0D20(sCtmM_0D20* m, const sCtmV_0D20& axis, float angle)
{
    float s;
    float c;
    float one = 1.0f;
    fSinCos(&s, &c, angle);
    sCtmM_0D20 r;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    ctmMulMtx_0D20(m, m, &r);
}

struct sCtmSrc_0D20 {
    char pad_0x0[0x28];
    sCtmV3_0D20 corners[4];      // 0x28 (count from the callers: corners 0..3)
};


/**
 * Returns one of a camera spline's four control points in world space.
 *
 * Transforms Point[@p corner] (+0x28 + 12 x @p corner, w = 1) by translate(position), rotations by pitch about X, roll
 * about Y and heading about Z, and scale. cChaseCameraController_createChaseAlgorithmBlend calls it for points 0..3 of
 * a spline action's spline to build a cSplineCamAlgorithm.
 *
 * @param src  the camera spline (this, On Tour cCTSpline)
 * @param corner  which control point, 0..3 (On Tour i)
 * @return the world-space point, returned by value
 */
// World position of corner `corner` (the 3-float points at src+0x28): translate(src+0x0) * rotations(0x10, 0x14, 0xC) * scale(src+0x18).
extern "C" sCtmV_0D20 cCTSpline_worldPoint(char* src, int corner)
{
    sCtmM_0D20 xform(D_004FF1A0_0D20);
    float one = 1.0f;
    ctmTransMtx_0D20(&xform, sCtmV_0D20(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis_0D20(&xform, D_004FF140_0D20, *(float*)(src + 0x10));
    ctmRotAxis_0D20(&xform, D_004FF150_0D20, *(float*)(src + 0x14));
    ctmRotAxis_0D20(&xform, D_004FF160_0D20, *(float*)(src + 0xC));
    xform = ctmScaleMtx_0D20(&xform, sCtmV_0D20(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    return ctmApplyMtx_0D20(&xform, sCtmV_0D20(((sCtmSrc_0D20*)src)->corners[corner], one));
}
#endif

