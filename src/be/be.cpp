#include "vtable.h"
#include "common.h"
#include "be/be.h"
#include "be/bestate.h"
#include "be/beinterface.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("be/be", cBE_getBE);
#ifdef SKIP_ASM
extern cBE* cBEState_pBE;
extern char str_BE[];
extern char str_BELibrary[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" cBELibrary* cBELibrary_cBELibrary(void* mem);
void* cBEPreRaceState_getState(void* self);
void* cBERaceState_getState(void* self);
void* cBEPostRaceState_getState(void* self);

/**
 * Returns the back end singleton, creating it and its three game-flow states on first use.
 *
 * The first call allocates the 4-byte "BE" object (cBE_instance) and the "BELibrary", fetches the pre-race, race
 * and post-race states into cBE_aBEState[0..2], makes pre-race (0) the current state and enters it, and sets
 * cBE_BEInitialized and cBEState_pBE.
 *
 * @return the back end object
 */
extern "C" cBE* cBE_getBE()
{
    if (cBE_instance == 0)
    {
        cBE_instance = (cBE*)cMemMan_alloc(4, str_BE, 0, 0);
        cBE_library = cBELibrary_cBELibrary(cMemMan_alloc(1, str_BELibrary, 0, 0));
        cBE_aBEState[0] = (cBEState*)cBEPreRaceState_getState((void*)0);
        cBE_aBEState[1] = (cBEState*)cBERaceState_getState((void*)1);
        cBE_aBEState[2] = (cBEState*)cBEPostRaceState_getState((void*)2);
        cBE_currentState = 0;
        cBEState* state = cBE_aBEState[0];
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)state->mVtbl;
        vt[kBEStateVt_onEnter].fn(vthis(state, vt[kBEStateVt_onEnter]));
        cBE_BEInitialized = 1;
        cBEState_pBE = cBE_instance;
    }
    return cBE_instance;
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_IsBEInitialized);
#ifdef SKIP_ASM
/**
 * Returns whether the back end has been initialised: true when cBE_BEInitialized is nonzero.
 *
 * @return true when initialised
 */
extern "C" bool cBE_IsBEInitialized(void) {
    return cBE_BEInitialized != 0;
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_getInterface__Fv);
#ifdef SKIP_ASM
// PORT: callers pass (be, kind); the mangled name __Fv is a guess. The kind is forwarded
// to the current state's virtual getInterface.

/**
 * Returns one of the back end's interfaces, as provided by the current game-flow state.
 *
 * @param kind  which interface to return (14 kinds, 0-13)
 * @return the current state's getInterface(kind) result (cBE_readFromReplayFrame() treats 0 as absent)
 */
cBEInterface* cBE::getInterface(int kind)
{
    cBEState* state = cBE_aBEState[cBE_currentState];
    sVEntry<cBEInterface*(void*, int)>* vt = (sVEntry<cBEInterface*(void*, int)>*)state->mVtbl;
    return vt[kBEStateVt_getInterface].fn(vthis(state, vt[kBEStateVt_getInterface]), kind);
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_purgeBE);
#ifdef SKIP_ASM
extern "C" void cBELibrary__cBELibrary(cBELibrary* p, int a);
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
    cBEState* preRace = cBE_aBEState[0];
    sVEntry<void(void*)>* vt0 = (sVEntry<void(void*)>*)preRace->mVtbl;
    vt0[kBEStateVt_purgeState].fn(vthis(preRace, vt0[kBEStateVt_purgeState]));
    cBEState* race = cBE_aBEState[1];
    sVEntry<void(void*)>* vt1 = (sVEntry<void(void*)>*)race->mVtbl;
    vt1[kBEStateVt_purgeState].fn(vthis(race, vt1[kBEStateVt_purgeState]));
    cBEState* postRace = cBE_aBEState[2];
    sVEntry<void(void*)>* vt2 = (sVEntry<void(void*)>*)postRace->mVtbl;
    vt2[kBEStateVt_purgeState].fn(vthis(postRace, vt2[kBEStateVt_purgeState]));
    operator_delete((int*)cBE_instance);
    cBE_instance = 0;
    cBE_BEInitialized = 0;
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_newProfile);
#ifdef SKIP_ASM
extern "C" void cBELibrary_newProfile(cBELibrary* p, char c);
extern "C" void cBEBAGTInterface_reloadProfileTricks(char c);

/**
 * Starts a fresh profile in slot @p playerIndex: resets its library entry and its BAGT interface state.
 *
 * Calls cBELibrary_newProfile(cBE_library, @p playerIndex) when the library exists (clears the player entry, gives it the
 * default player name, re-inits its profile and resets the new-player, stat and BAGT interfaces), then
 * cBEBAGTInterface_reloadProfileTricks(@p playerIndex). fe/fereal resets slots 0-2 this way when the back end has to be cleared.
 *
 * @param playerIndex  the profile slot
 */
void cBE::newProfile(char playerIndex)
{
    if (cBE_library)
        cBELibrary_newProfile(cBE_library, playerIndex);
    cBEBAGTInterface_reloadProfileTricks(playerIndex);
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_setState);
#ifdef SKIP_ASM
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
        cBEState* from = cBE_aBEState[cBE_currentState];
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)from->mVtbl;
        vt[kBEStateVt_onExit].fn(vthis(from, vt[kBEStateVt_onExit]));
        cBE_currentState = state;
        cBEState* to = cBE_aBEState[state];
        sVEntry<void(void*)>* vt2 = (sVEntry<void(void*)>*)to->mVtbl;
        vt2[kBEStateVt_onEnter].fn(vthis(to, vt2[kBEStateVt_onEnter]));
    }
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_reloadData);
#ifdef SKIP_ASM
extern "C" void cBELibrary_debugReloadData(cBELibrary* library);

/**
 * Calls cBELibrary_debugReloadData with cBE_library; that function's body is empty in this build, so this does nothing.
 */
extern "C" void cBE_reloadData(void) {
    cBELibrary_debugReloadData(cBE_library);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/be", cBE_writeToReplayFrame);
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

/**
 * Writes the back end's replay state to @p stream: cBE_currentState (4 bytes), then every interface's own data.
 *
 * For each of the 14 interface kinds that cBE_getInterface returns, calls that interface's writeToReplayFrame
 * virtual (vtable slot 3) with @p stream. cBE_readFromReplayFrame reads it back.
 *
 * @param stream  the replay output stream
 */
void cBE::writeToReplayFrame(cBxReplayFrame* stream)
{
    stream->write((const char*)&cBE_currentState, 4);
    for (int i = 0; i < 14; i++)
    {
        if (cBE_getInterface_Fv(this, i))
            cBE_getInterface_Fv(this, i)->writeToReplayFrame((cBEReplayStream*)stream);
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
void cBE::readFromReplayFrame(cBxReplayFrame* data)
{
    data->read((char*)&cBE_currentState, 4);
    for (int i = 0; i < 14; i++)
    {
        if (cBE_getInterface_Fv(this, i))
            cBE_getInterface_Fv(this, i)->readFromReplayFrame((cBEReplayStream*)data);
    }
}
#endif

//100%
INCLUDE_ASM("be/be", cBE_getDataSize);
#ifdef SKIP_ASM
/**
 * Returns the size of the career save data the game keeps for a memory card or replay.
 *
 * Always 0xBB78 (47,992) bytes.
 *
 * @return 0xBB78
 */
extern "C" int cBE_getDataSize()
{
    return 47992;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/be", cBE_writeToData);
#ifdef SKIP_ASM
extern "C" void cBELibrary_writeToData(void* buf);
// PORT: cBE_getDataSize is declared () in this unit but called with the BE object here.
int cBE_getDataSize_self(void* be) __asm__("cBE_getDataSize");
extern "C" int MEM_crc16(void* buf, int size, int seed);
extern "C" void* memcpy(void*, const void*, unsigned int);

/**
 * Fills the save buffer @p buf with the back-end state and stores a checksum in its last 4 bytes.
 *
 * cBELibrary_writeToData copies the race database, player entries, profiles and other state blocks into @p buf; then
 * MEM_crc16 checksums the first cBE_getDataSize() - 4 bytes with seed 64490 and the result is copied to the end.
 * fe/festatetrophyroom saves the memory card buffer this way.
 *
 * @param buf  the save buffer, cBE_getDataSize() bytes
 */
void cBE::writeToData(char* buf)
{
    cBELibrary_writeToData(buf);
    int crc = MEM_crc16(buf, cBE_getDataSize_self(this) - 4, 64490);
    char* crcSlot = buf + (cBE_getDataSize_self(this) - 4);
    memcpy(crcSlot, &crc, 4);
}
#endif

extern "C" void* cBELibrary_readFromData(int);

//100%
INCLUDE_ASM("be/be", cBE_readFromData__FPvi);
#ifdef SKIP_ASM
/**
 * Loads the career data from a save buffer read off the memory card.
 *
 * Tail-calls cBELibrary_readFromData (On Tour cBELibrary::readFromData) with @p data.
 *
 * @param data  the save buffer (On Tour data)
 * @return cBELibrary_readFromData's result (On Tour returns nothing)
 */
void* cBE::readFromData(int data)
{
    return cBELibrary_readFromData(data);
}
#endif

