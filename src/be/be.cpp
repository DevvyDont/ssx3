#include "common.h"

//100%
INCLUDE_ASM("be/be", cBE_getBE);
#ifdef SKIP_ASM
struct sBEInterface;
extern sBEInterface* cBE_aBEState[];
extern int cBE_currentState;
extern void* cBE_library;
extern int* cBE_instance;
extern int cBE_BEInitialized;
extern int* cBEState_pBE;
extern char str_BE[];
extern char str_BELibrary[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBELibrary_cBELibrary(void* mem);
void* cBEPreRaceState_getState(void* self);
void* cBERaceState_getState(void* self);
void* cBEPostRaceState_getState(void* self);

class cBEStateV_getBE
{
public:
    char pad_0x00[0x4];
    virtual void v1();
    virtual void v2();
    virtual void leave();
    virtual void enter();
};

/**
 * Returns the back end singleton, creating it and its three game-flow states on first use.
 *
 * The first call allocates the 4-byte "BE" object (cBE_instance) and the "BELibrary", fetches the pre-race, race
 * and post-race states into cBE_aBEState[0..2], makes pre-race (0) the current state and enters it, and sets
 * cBE_BEInitialized and cBEState_pBE.
 *
 * @return the back end object
 */
extern "C" void* cBE_getBE()
{
    if (cBE_instance == 0)
    {
        cBE_instance = (int*)cMemMan_alloc(4, str_BE, 0, 0);
        cBE_library = cBELibrary_cBELibrary(cMemMan_alloc(1, str_BELibrary, 0, 0));
        cBE_aBEState[0] = (sBEInterface*)cBEPreRaceState_getState((void*)0);
        cBE_aBEState[1] = (sBEInterface*)cBERaceState_getState((void*)1);
        cBE_aBEState[2] = (sBEInterface*)cBEPostRaceState_getState((void*)2);
        cBE_currentState = 0;
        ((cBEStateV_getBE*)cBE_aBEState[0])->enter();
        cBE_BEInitialized = 1;
        cBEState_pBE = cBE_instance;
    }
    return cBE_instance;
}
#endif

struct sBEInterfaceVTable {
    char pad_0x00[0x8];
    short field_0x8;
    char pad_0xA[2];
    void* (*fn)(void*);
};

struct sBEInterface {
    char pad_0x00[0x4];
    sBEInterfaceVTable* vtable;
};

extern sBEInterface* cBE_aBEState[];
extern int cBE_currentState;

//100%
INCLUDE_ASM("be/be", func_0014DD48);
#ifdef SKIP_ASM
extern int cBE_BEInitialized;

extern "C" int func_0014DD48(void) {
    return cBE_BEInitialized != 0;
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_getInterface__Fv);
#ifdef SKIP_ASM
class cBEInterfaceV_getInterface
{
public:
    char pad_0x00[0x4];
    virtual void* getInterface(int kind);
};

// PORT: callers pass (be, kind); the mangled name __Fv is a guess. The kind is forwarded
// to the current state's virtual getInterface.
void* cBE_getInterface_impl(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Returns one of the back end's interfaces, as provided by the current game-flow state.
 *
 * @param be    the back end object; unused, the current state (cBE_aBEState[cBE_currentState]) is asked
 * @param kind  which interface to return (14 kinds, 0-13)
 * @return the current state's getInterface(kind) result (cBE_readFromReplayFrame() treats 0 as absent)
 */
void* cBE_getInterface_impl(void* be, int kind)
{
    return ((cBEInterfaceV_getInterface*)cBE_aBEState[cBE_currentState])->getInterface(kind);
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_purgeBE);
#ifdef SKIP_ASM
class cBEStateV_0014DD98
{
public:
    char pad_0x00[0x4];
    virtual void v1();
    virtual void shutdown();
};
extern void* cBE_library;
extern int* cBE_instance;
extern int cBE_BEInitialized;
extern "C" void cBELibrary__cBELibrary(void* p, int a);
void operator_delete(int* ptr);

/**
 * Tears down the back end: the BE library, the three game-flow states and the BE object.
 *
 * Destroys the library (cBE_library, flags 3) if there is one, calls virtual slot 2 on each state, frees the
 * 4-byte "BE" object and clears cBE_instance and the exists flag cBE_BEInitialized, so the next cBE_getBE() builds
 * everything again. cBE_library itself is not cleared.
 */
extern "C" void cBE_purgeBE(void)
{
    if (cBE_library)
        cBELibrary__cBELibrary(cBE_library, 3);
    ((cBEStateV_0014DD98*)cBE_aBEState[0])->shutdown();
    ((cBEStateV_0014DD98*)cBE_aBEState[1])->shutdown();
    ((cBEStateV_0014DD98*)cBE_aBEState[2])->shutdown();
    operator_delete(cBE_instance);
    cBE_instance = 0;
    cBE_BEInitialized = 0;
}
#endif

//100%
INCLUDE_ASM("be/be", func_0014DE28);
#ifdef SKIP_ASM
extern void* cBE_library;
extern "C" void func_00149A88(void* p, char c);
extern "C" void func_0014FC40(char c);

extern "C" void func_0014DE28(void* self, char playerIndex)
{
    if (cBE_library)
        func_00149A88(cBE_library, playerIndex);
    func_0014FC40(playerIndex);
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_setState);
#ifdef SKIP_ASM
class cBEStateV_setState
{
public:
    char pad_0x00[0x4];
    virtual void v1();
    virtual void v2();
    virtual void leave();
    virtual void enter();
};

/**
 * Switches the back end to another game-flow state, calling the old state's leave and the new state's enter.
 *
 * Does nothing when @p state is already current.
 *
 * @param state  0 pre-race, 1 race, 2 post-race
 */
extern "C" void cBE_setState(int state)
{
    if (cBE_currentState != state)
    {
        ((cBEStateV_setState*)cBE_aBEState[cBE_currentState])->leave();
        cBE_currentState = state;
        ((cBEStateV_setState*)cBE_aBEState[state])->enter();
    }
}
#endif

//100%
INCLUDE_ASM("be/be", func_0014DEE8);
#ifdef SKIP_ASM
extern "C" void func_0014A078(int);

extern "C" void func_0014DEE8(void) {
    func_0014A078((*(int*)&cBE_library));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/be", func_0014DF08);
#ifdef SKIP_ASM
class cBEReplayStream {
public:
    virtual void write(void* data, int size);
    virtual void read(void* data, int size);
};

class cBEReplayIface {
public:
    char pad_0x00[0xC];
    virtual void v1();
    virtual void v2();
    virtual void writeToReplayFrame(cBEReplayStream* s);
    virtual void readFromReplayFrame(cBEReplayStream* s);
};

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEReplayIface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" void func_0014DF08(void* self, cBEReplayStream* stream)
{
    stream->write(&cBE_currentState, 4);
    for (int i = 0; i < 14; i++)
    {
        if (cBE_getInterface_Fv(self, i))
            cBE_getInterface_Fv(self, i)->writeToReplayFrame(stream);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/be", cBE_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEReplayIface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Restores the back end from a replay frame: the current state index, then each interface's own data.
 *
 * The state index is read directly, with no leave or enter call. Each of the 14 interfaces that exists reads
 * its data through its virtual readFromReplayFrame.
 *
 * @param data  the replay stream to read from
 */
extern "C" void cBE_readFromReplayFrame(void* self, cBEReplayStream* data)
{
    data->read(&cBE_currentState, 4);
    for (int i = 0; i < 14; i++)
    {
        if (cBE_getInterface_Fv(self, i))
            cBE_getInterface_Fv(self, i)->readFromReplayFrame(data);
    }
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_getDataSize);
#ifdef SKIP_ASM
extern "C" int cBE_getDataSize()
{
    return 0xBB78;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/be", func_0014E050);
#ifdef SKIP_ASM
extern "C" void func_0014A188(void* buf);
// PORT: cBE_getDataSize is declared () in this unit but called with the BE object here.
int cBE_getDataSize_self(void* be) __asm__("cBE_getDataSize");
extern "C" int func_003E62D0(void* buf, int size, int seed);
extern "C" void* memcpy(void*, const void*, unsigned int);

extern "C" void func_0014E050(void* self, char* buf)
{
    func_0014A188(buf);
    int crc = func_003E62D0(buf, cBE_getDataSize_self(self) - 4, 0xFBEA);
    char* crcSlot = buf + (cBE_getDataSize_self(self) - 4);
    memcpy(crcSlot, &crc, 4);
}
#endif

extern "C" void* cBELibrary_readFromData(int);

//100%
INCLUDE_ASM("be/be", cBE_readFromData__FPvi);
#ifdef SKIP_ASM
void* cBE_readFromData(void* self, int a1)
{
    return cBELibrary_readFromData(a1);
}
#endif

