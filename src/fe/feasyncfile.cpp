#include "common.h"
#include "fe/festatelogin.h"
#include "fe/festatenetcreateroompopup.h"
#include "fe/festateauthdnas.h"
#include "fe/festatenetquickmessages.h"
#include "fe/festateonlineagreement.h"
#include "fe/festatereadmessagepopup.h"
#include "fe/festatenetquickplaypopup.h"
#include "fe/festateonlinecharacterselect.h"
#include "fe/festatecreateduplicate.h"
#include "fe/festatechatroom.h"
#include "fe/festatenetchallenge.h"
#include "fe/festatenetaccount.h"
#include "fe/popupinfo.h"
#include "fe/festatestats.h"
#include "fe/festatenetlobby.h"
#include "fe/festatenetcreate.h"
#include "fe/festatenetconfig.h"
#include "fe/festatenetmessenger.h"
#include "fe/festatenet.h"
#include "online/buddyman.h"
#include "online/lobbyman.h"
#include "render/ps2graphicsman.h"
#include "be/beintnetwork.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_Load3PeakPic);
#ifdef SKIP_ASM
struct cFEAsyncManager;
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_004A19F0;
extern char str_pct_spct_s[];
extern char str_ssh[];

struct sFE3PeakPicEntry {
    char mName[0x108];
    int mParam; // 0x108
    char pad[0x11C - 0x10C];
};

struct sFE3PeakPicFiles {
    sFE3PeakPicEntry mFiles[1];
};

/**
 * Requests the whole-mountain map picture ("|map_mtn.ssh") into an async file slot.
 *
 * Writes the name into the slot, stores @p useAsync with it and sets the slot's status to 1.
 *
 * @param slot   index of the async file slot
 * @param useAsync  value stored with the request (On Tour's LoadLocationPic calls it useAsync)
 */
extern "C" void cFEAsyncManager_Load3PeakPic(cFEAsyncManager* self, int slot, int useAsync)
{
    sFE3PeakPicFiles* files = (sFE3PeakPicFiles*)self;
    sprintf(files->mFiles[slot].mName, str_pct_spct_s, D_004A19F0, str_ssh);
    files->mFiles[slot].mParam = useAsync;
    cFEAsyncManager_SetFileStatus(self, slot, 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_Load1PeakPic);
#ifdef SKIP_ASM
struct cFEAsyncManager;
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_00441258[];
extern char str_pct_spct_s[];
extern char str_ssh[];

struct sFEPeakPicEntry {
    char mName[0x108];
    int mParam; // 0x108
    char pad[0x11C - 0x10C];
};

struct sFEPeakPicFiles {
    sFEPeakPicEntry mFiles[1];
};

/**
 * Requests one peak's map picture ("|map_peakA.ssh", B or C) into an async file slot.
 *
 * Writes the name into the slot, stores @p useAsync with it and sets the slot's status to 1.
 *
 * @param pic    the peak, 0 to 2
 * @param slot   index of the async file slot
 * @param useAsync  value stored with the request (On Tour's LoadLocationPic calls it useAsync)
 */
extern "C" void cFEAsyncManager_Load1PeakPic(cFEAsyncManager* self, int pic, int slot, int useAsync)
{
    sFEPeakPicFiles* files = (sFEPeakPicFiles*)self;
    sprintf(files->mFiles[slot].mName, str_pct_spct_s, D_00441258[pic], str_ssh);
    files->mFiles[slot].mParam = useAsync;
    cFEAsyncManager_SetFileStatus(self, slot, 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_LoadLocationPic);
#ifdef SKIP_ASM
extern char str_pct_spct_spct_s[];
extern char D_004A1A10[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBEMissionInterface_getPictureFileName(void* self, int id);

/**
 * Requests a location's picture into an async file slot.
 *
 * The name is "|" + cBEMissionInterface_getPictureFileName() + ".ssh". A negative location ID requests nothing.
 *
 * @param id     the location ID (On Tour eBELocationID locID)
 * @param slot   index of the async file slot (On Tour pos)
 * @param useAsync  value stored with the request (On Tour useAsync)
 */
extern "C" void cFEAsyncManager_LoadLocationPic(cFEAsyncManager* self, int id, int slot, int useAsync)
{
    if (id >= 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 10);
        sFEPeakPicFiles* files = (sFEPeakPicFiles*)self;
        sprintf(files->mFiles[slot].mName, str_pct_spct_spct_s, D_004A1A10, cBEMissionInterface_getPictureFileName(iface, id), str_ssh);
        files->mFiles[slot].mParam = useAsync;
        cFEAsyncManager_SetFileStatus(self, slot, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_UnloadFEAsyncFile);
#ifdef SKIP_ASM
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status);
void cMemMan_free(void* p);
extern "C" int ASYNCFILE_release(int handle, void** data, int* size);

struct sFEUnloadFile {
    char name[0x100];
    void* data;    // 0x100
    int handle;    // 0x104
    int async;     // 0x108
    int size;      // 0x10C
    int id;        // 0x110
    int status;    // 0x114
    int f118;      // 0x118
};

struct sFEUnloadFiles {
    sFEUnloadFile files[5];
};

struct sVEUnload {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Releases an async file slot at whatever stage its load is in and marks it free (status 0).
 *
 * Requested (1): only the status changes. Loading (2): releases the ASYNCFILE handle and frees the buffer.
 * Loaded (3): keeps the texture and buffer and sets +0x118 to the graphics frame count (GrMan vtable slot
 * 114) + 14, so cFEAsyncManager_Update() frees them once that frame has passed.
 *
 * @param idx  index of the file slot (On Tour fileNum)
 */
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(cFEAsyncManager* self, int idx)
{
    sFEUnloadFiles* asyncMan = (sFEUnloadFiles*)self;
    if (cFEAsyncManager_GetFileStatus(self, idx) == 1) {
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    } else if (cFEAsyncManager_GetFileStatus(self, idx) == 2) {
        ASYNCFILE_release(asyncMan->files[idx].handle, 0, 0);
        if (asyncMan->files[idx].data != 0) {
            cMemMan_free(asyncMan->files[idx].data);
        }
        asyncMan->files[idx].data = 0;
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    } else if (cFEAsyncManager_GetFileStatus(self, idx) == 3) {
        cPS2GraphicsMan* obj = GrMan;
        sVEUnload* vt = (sVEUnload*)obj->mVtbl;
        asyncMan->files[idx].f118 = vt[114].fn((char*)obj + vt[114].delta) + 0xE;
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    }
}
#endif

struct cFEAsyncFileEntry {
    char pad_0x00[0x114];
    int mStatus; // 0x114
    char pad_0x118[0x11C - 0x114 - 4];
};

struct cFEAsyncManager {
    cFEAsyncFileEntry mFiles[1];
};

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_GetFileStatus__FP15cFEAsyncManageri);
#ifdef SKIP_ASM
/**
 * Returns the status of one of the async manager's files.
 *
 * @param index  file slot, not range-checked
 * @return mFiles[index].mStatus
 */
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index)
{
    return self->mFiles[index].mStatus;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_SetFileStatus__FP15cFEAsyncManagerii);
#ifdef SKIP_ASM
/**
 * Sets the status of one of the async manager's files.
 *
 * @param index   file slot, not range-checked
 * @param status  value stored in mFiles[index].mStatus
 */
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status)
{
    self->mFiles[index].mStatus = status;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_Update);
#ifdef SKIP_ASM
struct sFE39F0File {
    char name[0x100];
    char* data;    // 0x100
    int handle;    // 0x104
    int async;     // 0x108
    int size;      // 0x10C
    int id;        // 0x110
    int status;    // 0x114
    int f118;      // 0x118
};

struct sFE39F0Files {
    sFE39F0File files[5];
};

struct sVE39F0 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sVE39F0L {
    short delta;
    short index;
    int (*fn)(void*, char*, const char*, int, int, int);
};

struct sVE39F0V {
    short delta;
    short index;
    void (*fn)(void*, int);
};

int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);
void cFEAsyncManager_SetFileStatus(cFEAsyncManager* self, int index, int status);
extern "C" void cFEAsyncManager_LoadDataFile(cFEAsyncManager* self, int idx);
extern "C" int ASYNCFILE_release(int handle, void** data, int* size);
extern "C" int ASYNCFILE_getstatus(int handle);
void cMemMan_free(void* p);
extern char str_map[];

/**
 * Advances the five async file slots by one frame: deferred frees, load starts and finished reads.
 *
 * A free or requested slot whose release frame (+0x118) is before the current frame count and which still
 * holds a buffer releases its texture (GrMan vtable slot 50) and buffer; a requested slot past that frame
 * then starts loading (cFEAsyncManager_LoadDataFile()). A loading slot whose ASYNCFILE read is done becomes
 * loaded (3): its handle is released and the image (file + its word at +0x14) becomes a texture named "map"
 * (GrMan slot 46), whose ID goes to +0x110.
 */
extern "C" void cFEAsyncManager_Update(cFEAsyncManager* self)
{
    sFE39F0Files* asyncMan = (sFE39F0Files*)self;
    int i;
    for (i = 0; i < 5; i++) {
        if (cFEAsyncManager_GetFileStatus(self, i) == 0 || cFEAsyncManager_GetFileStatus(self, i) == 1) {
            cPS2GraphicsMan* obj = GrMan;
            sVE39F0* vt = (sVE39F0*)obj->mVtbl;
            if (asyncMan->files[i].f118 < vt[114].fn((char*)obj + vt[114].delta)) {
                if (asyncMan->files[i].data != 0) {
                    cPS2GraphicsMan* obj2 = GrMan;
                    sVE39F0V* vt2 = (sVE39F0V*)obj2->mVtbl;
                    vt2[50].fn((char*)obj2 + vt2[50].delta, asyncMan->files[i].id);
                    if (asyncMan->files[i].data != 0) {
                        cMemMan_free(asyncMan->files[i].data);
                    }
                    asyncMan->files[i].data = 0;
                    asyncMan->files[i].f118 = 0;
                    asyncMan->files[i].id = -1;
                }
            }
        }
        if (cFEAsyncManager_GetFileStatus(self, i) == 1) {
            cPS2GraphicsMan* obj = GrMan;
            sVE39F0* vt = (sVE39F0*)obj->mVtbl;
            if (asyncMan->files[i].f118 < vt[114].fn((char*)obj + vt[114].delta)) {
                cFEAsyncManager_LoadDataFile(self, i);
            }
        }
        if (cFEAsyncManager_GetFileStatus(self, i) == 2) {
            if (ASYNCFILE_getstatus(asyncMan->files[i].handle) == 1) {
                cFEAsyncManager_SetFileStatus(self, i, 3);
                ASYNCFILE_release(asyncMan->files[i].handle, 0, 0);
                cPS2GraphicsMan* obj = GrMan;
                sVE39F0L* vt = (sVE39F0L*)obj->mVtbl;
                char* data = asyncMan->files[i].data;
                asyncMan->files[i].id = vt[46].fn((char*)obj + vt[46].delta, data + *(int*)(data + 0x14), str_map, 0, 0, -1);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_GetFreeAsyncFile);
#ifdef SKIP_ASM
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);

/**
 * Finds the first of the 5 async file slots that is idle.
 *
 * @return the slot index whose cFEAsyncManager_GetFileStatus() is 0, or -1 when all 5 are busy
 */
extern "C" int cFEAsyncManager_GetFreeAsyncFile(cFEAsyncManager* self)
{
    int i;
    for (i = 0; i < 5; i++) {
        if (cFEAsyncManager_GetFileStatus(self, i) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_FlushFEAsyncTextures);
#ifdef SKIP_ASM
struct sFE3C50File {
    char name[0x100];
    void* data;    // 0x100
    int handle;    // 0x104
    int async;     // 0x108
    int size;      // 0x10C
    int id;        // 0x110
    int status;    // 0x114
    int f118;      // 0x118
};

struct sFE3C50Files {
    sFE3C50File files[5];
};

struct sVE3C50 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

void cMemMan_free(void* p);

/**
 * Releases the front-end's five asynchronously loaded textures.
 *
 * For each slot with data: unloads its texture id through graphics manager slot 50, frees the data (cMemMan_free),
 * then clears the data pointer and the word at +0x118 and sets the id to -1.
 */
extern "C" void cFEAsyncManager_FlushFEAsyncTextures(cFEAsyncManager* self)
{
    sFE3C50Files* asyncMan = (sFE3C50Files*)self;
    int i;
    for (i = 0; i < 5; i++) {
        if (asyncMan->files[i].data != 0) {
            cPS2GraphicsMan* obj = GrMan;
            sVE3C50* vt = (sVE3C50*)obj->mVtbl;
            vt[50].fn((char*)obj + vt[50].delta, asyncMan->files[i].id);
            if (asyncMan->files[i].data != 0) {
                cMemMan_free(asyncMan->files[i].data);
            }
            asyncMan->files[i].data = 0;
            asyncMan->files[i].f118 = 0;
            asyncMan->files[i].id = -1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_LoadDataFile);
#ifdef SKIP_ASM
struct sFEAsyncFile {
    char name[0x100];
    void* data;    // 0x100
    int handle;    // 0x104
    int async;     // 0x108
    int size;      // 0x10C
    int pad110;
    int status;    // 0x114
    int pad118;
};

struct sFEAsyncFiles {
    sFEAsyncFile files[5];
};

extern char str_FE_Async_Texture[];
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int func_003DF748(void* file, void* data, int size);
extern "C" void FILE_loadatz(void* file, void* data, int size);

/**
 * Starts reading a slot's file into a new buffer of a fixed 0x109A0 bytes (tag "FE Async Texture").
 *
 * Sets the status to 2 (loading). With the slot's async flag (+0x108) it starts an asynchronous read
 * (func_003DF748) and keeps the handle at +0x104; otherwise it reads synchronously (FILE_loadatz) and sets
 * status 3 directly, skipping the texture step that cFEAsyncManager_Update() runs only for asynchronous
 * loads.
 *
 * @param idx  index of the file slot (On Tour fileNum)
 */
extern "C" void cFEAsyncManager_LoadDataFile(cFEAsyncManager* self, int idx)
{
    sFEAsyncFiles* asyncMan = (sFEAsyncFiles*)self;
    void** pData;
    char* base;
    asyncMan->files[idx].size = 0x109A0;
    base = (char*)self + 0x100;
    pData = (void**)(base + idx * 0x11C);
    *pData = operator_new_tag(0x109A0, str_FE_Async_Texture, 0x25000000, 0);
    cFEAsyncManager_SetFileStatus(self, idx, 2);
    if (asyncMan->files[idx].async != 0) {
        cFEAsyncManager_SetFileStatus(self, idx, 2);
        asyncMan->files[idx].handle = func_003DF748(&asyncMan->files[idx], *pData, asyncMan->files[idx].size);
    } else {
        FILE_loadatz(&asyncMan->files[idx], *pData, asyncMan->files[idx].size);
        cFEAsyncManager_SetFileStatus(self, idx, 3);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_cFEStateNetMessenger);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern void* D_00467F28[];
extern void* D_004A3E90_v __asm__("cBXString_EmptyString");

/**
 * Constructs the online messenger (buddy list) screen state.
 *
 * Runs the cFEStateNet constructor func_001A8500 with @p pEngine and @p pParentState, then stores the messenger vtable D_00467F28 and
 * sets mHashName to 70 (0x46). The cBXString members mszFoundName, mszSearchName and unk_6E0 start as the shared empty
 * string (cBXString_EmptyString), miSelectedIndex is -1, and mLastRefreshFrame, the mNames vector's three pointers and
 * miScrollOffset are zeroed. Its only caller, func_001A9B58, allocates the object with cMemMan_alloc(0x6FC,
 * "cFEStateNetMessenger").
 *
 * @param pEngine  the UI engine (stored in mpEngine by the base constructor)
 * @param pParentState  the parent state (mpParentState)
 * @return @p self
 */
extern "C" void* cFEStateNetMessenger_cFEStateNetMessenger(cFEStateNetMessenger* self, void* pEngine, void* pParentState)
{
    // An unused 16-byte local is needed to reproduce the 0x30-byte frame.
    int unused[4];
    func_001A8500_3(self, (int)pEngine, (int)pParentState);
    void* empty = D_004A3E90_v;
    self->mVtbl = D_00467F28;
    self->mszFoundName = (char*)empty;
    self->mszSearchName = (char*)empty;
    self->unk_6E0 = (char*)empty;
    self->miSelectedIndex = -1;
    self->mHashName = 70;
    self->mLastRefreshFrame = 0;
    self->mNames.mpStart = 0;
    self->mNames.mpFinish = 0;
    self->mNames.mpEndOfStorage = 0;
    self->miScrollOffset = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3E30);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

extern "C" void func_001A3E30()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00262768(BuddyMan_mBuddyMan, 1, 10, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger__cFEStateNetMessenger);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cBXString__cBXString(void* self, int flags);
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern void* D_00467F28[];
struct sOwner1A5450;
extern "C" void func_001A5450(cFEStateNetMessenger* self);
extern "C" void func_001A85D0_dtor3EA0(void* self, int flags) __asm__("func_001A85D0");

struct sVE1A3EA0 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline void Dealloc1A3EA0(void** p, int n)
{
    if (p != 0) {
        if (n == 1) {
            operator_delete((int*)p);
        } else {
            cMemMan_free(p);
        }
    }
}



/**
 * Destroys the online messenger screen state and its buddy-name list.
 *
 * Restores the messenger vtable, calls the parent state's virtual at vtable entry 32 (for a cFEStateNet parent
 * func_001A8830, which sets the player's buddy presence through func_00262768), destroys every name in mNames
 * (func_001A5450) and frees the vector's storage (operator_delete when its capacity is one pointer, cMemMan_free
 * otherwise), destroys the cBXStrings unk_6E0, mszSearchName and mszFoundName, and ends in the cFEStateNet destructor
 * func_001A85D0 with @p flags.
 *
 * @param flags  g++ destructor flags, passed to the base destructor (bit 0 set frees the object)
 */
extern "C" void cFEStateNetMessenger__cFEStateNetMessenger(cFEStateNetMessenger* self, int flags)
{
    self->mVtbl = (void**)D_00467F28;
    char* child = (char*)self->mpParentState;
    sVE1A3EA0* vt = *(sVE1A3EA0**)(child + 0x8);
    vt[32].fn(child + vt[32].delta);
    func_001A5450(self);
    tFEStringPtrVector* feStringPtrVector = &(*(&self->mNames));
    Dealloc1A3EA0(((void**)feStringPtrVector->mpStart), (void**)self->mNames.mpEndOfStorage - (void**)self->mNames.mpStart);
    cBXString__cBXString(((char*)&self->unk_6E0), 2);
    cBXString__cBXString(((char*)&self->mszSearchName), 2);
    cBXString__cBXString(((char*)&self->mszFoundName), 2);
    func_001A85D0_dtor3EA0((self), flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A5898(cFEStateNetMessenger* self);
extern "C" void func_001A6978(cFEStateNetMessenger* self);
extern char str_279EAMessenger[];

/**
 * Creates the messenger's UI screen and fills it.
 *
 * Adds the screen "279EAMessenger" (GetHashValue32 of str_279EAMessenger) through cUIEngine_addScreenByHashName with
 * this state as owner and stores it in mpScreen. When it exists, plays frame 0, loads the buddy names (func_001A5898)
 * and draws the list (func_001A6978).
 */
extern "C" void cFEStateNetMessenger_onCreateScreen(cFEStateNetMessenger* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_279EAMessenger), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001A5898(self);
        func_001A6978(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_onUpdate);
#ifdef SKIP_ASM
extern void* cAppMan_TheAppMan;
extern "C" void func_001A6978(cFEStateNetMessenger* self);
extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self);

/**
 * Per-frame update of the messenger screen; redraws the buddy list about once a second.
 *
 * Reads the frame counter (cAppMan_TheAppMan + 0x18). When 1 to 59 frames have passed since mLastRefreshFrame it only
 * runs cFEStateNet's update (cFEStateNet_onUpdate); otherwise it stores the frame in mLastRefreshFrame, redraws the list
 * (func_001A6978) and then runs cFEStateNet_onUpdate.
 */
extern "C" void cFEStateNetMessenger_onUpdate(cFEStateNetMessenger* self)
{
    int now = *(int*)((char*)cAppMan_TheAppMan + 0x18);
    int elapsed = now - self->mLastRefreshFrame;
    if (elapsed >= 1 && elapsed <= 59) {
        cFEStateNet_onUpdate((cFEStateNet*)self);
    } else {
        self->mLastRefreshFrame = now;
        func_001A6978(self);
        cFEStateNet_onUpdate((cFEStateNet*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4040);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_BTNEAMessenger[];
extern char str_kT_HELP279BuddiesOnline[];
extern char str_kT_HELP279ReadMessages[];
extern char str_kT_HELP279StoreBuddies[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELP279SelectBuddy[];
extern char str_kT_HELP279FindUser[];
extern char str_kT_HELPCmnPrevious[];
extern char str_kT_POPUPCMNNextChallenge[];

extern "C" void func_001A4040(cFEStateNetMessenger* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_BTNEAMessenger);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP279BuddiesOnline));
    func_001DA530(menu, 1, GetHashValue32(str_kT_HELP279ReadMessages));
    func_001DA530(menu, 2, GetHashValue32(str_kT_HELP279StoreBuddies));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELP279SelectBuddy), 0);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELP279FindUser), 2);
    func_001DA510(menu, 3, GetHashValue32(str_kT_HELPCmnPrevious), 1);
    func_001DA510(menu, 4, GetHashValue32(str_kT_POPUPCMNNextChallenge), 6);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_onInputBegin);
#ifdef SKIP_ASM
struct sVE1A4170 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001A6C40(cFEStateNetMessenger* self, int a1);

/**
 * Handles the start of an input on the messenger screen: L1 jumps to the buddy with something pending.
 *
 * Calls @p obj's virtual at vtable entry 27 (cFEInput_isLShift1 for a cFEInput). When it is set, selects entry
 * miSelectedIndex (set by func_001A52B0 to the buddy whose challenge or message waits first) in the "playerMenu" menu
 * (func_001A6C40) and reports the input handled.
 *
 * @param obj  the input (On Tour: cUIInput* pInput)
 * @return 1 when L1 was pressed, otherwise 0
 */
extern "C" int cFEStateNetMessenger_onInputBegin(cFEStateNetMessenger* self, void* obj)
{
    int handled;
    sVE1A4170* vt = *(sVE1A4170**)((char*)obj + 0x8);
    if (vt[27].fn((char*)obj + vt[27].delta) == 0) {
        handled = 0;
    } else {
        func_001A6C40(self, self->miSelectedIndex);
        handled = 1;
    }
    return handled;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_kT_BTNEAMessenger[];
extern char str_TextTitle[];
extern char str_TextFindUser[];
extern char str_kT_231FindPlayer[];
extern char str_TextBuddyLocation[];
extern char str_TextEqGoToNext[];
extern char str_kT_279GoToNext[];
extern char str_playerMenu[];

class cFEVObj_001A41C0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash1A41C0(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Sets up the messenger screen's widgets as the screen creates them.
 *
 * By the widget's hash name (+0x38): "TextTitle" gets the string kT_BTNEAMessenger and is shown (virtual setVisible,
 * entry 9); "TextFindUser" gets kT_231FindPlayer and is shown; "TextBuddyLocation" is shown and gets flag 0x80 in its
 * word +0x14; "TextEqGoToNext" is hidden and gets kT_279GoToNext; "playerMenu" gets flag 1 in +0x14.
 *
 * @param text  the widget being created (On Tour: cUIThing* pWidget)
 */
extern "C" void cFEStateNetMessenger_onWidgetCreate(cFEStateNetMessenger* self, cUIText* text)
{
    cFEVObj_001A41C0* obj = (cFEVObj_001A41C0*)text;
    if (IsHash1A41C0(*(int*)((char*)text + 0x38), str_TextTitle)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_BTNEAMessenger));
        obj->v09(1);
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), str_TextFindUser)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_231FindPlayer));
        obj->v09(1);
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), str_TextBuddyLocation)) {
        obj->v09(1);
        *(int*)((char*)text + 0x14) |= 0x80;
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), str_TextEqGoToNext)) {
        obj->v09(0);
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_279GoToNext));
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), str_playerMenu)) {
        *(int*)((char*)text + 0x14) |= 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void func_001A6978(cFEStateNetMessenger* self);

/**
 * Tracks the buddy list's scrolling: a menu scroll event stores the new top row and redraws the list.
 *
 * On @p event 4, stores @p value in miScrollOffset, redraws the list (func_001A6978) and returns 0. Any other event
 * returns 257 (0x101) and changes nothing.
 *
 * @param pMenu  the menu widget (unused)
 * @param event  the menu event
 * @param value  the event's value: the new first visible row for event 4
 * @return 0 for event 4, otherwise 257
 */
extern "C" int cFEStateNetMessenger_onWidgetMenu(cFEStateNetMessenger* self, int pMenu, int event, int value)
{
    if (event == 4) {
        self->miScrollOffset = value;
        func_001A6978(self);
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void* cUIScreen_getObjectByHashName_4330(void* self, int hash) __asm__("cUIScreen_getObjectByHashName");
extern char D_004A1408[];
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* func_002625C8_4330(void* session, int idx) __asm__("func_002625C8");
extern "C" void func_001A9890_4330(void* self, void* entry) __asm__("func_001A9890");
extern "C" void* func_001A98B0_4330(void* self) __asm__("func_001A98B0__FPv");
extern "C" void func_001AB370_4330(void* self, void* a1, int a2) __asm__("func_001AB370");
extern "C" void func_001A4750_4330(void* self) __asm__("func_001A4750");
extern "C" void func_001A6070_4330(void* self, int idx) __asm__("func_001A6070");
extern "C" void func_001A5FB8_4330(void* self, int idx) __asm__("func_001A5FB8");
extern "C" void func_001A5128_4330(void* self, int idx, int n) __asm__("func_001A5128");
extern "C" void func_001A66A8_4330(void* self, int idx, int on) __asm__("func_001A66A8");
extern char str_kT_231FindPlayer[];
extern char str_personapct_02d[];
extern char str_ps2Xpct_02d[];

struct cVec_4330 {
    float x, y, z, w;
    cVec_4330(float a, float b, float c, float e) { x = a; y = b; z = c; w = e; }
};

class cObjV_4330 {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void set(const cVec_4330& v);
};

struct sVE_4330 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

struct sObj_4330 {
    int pad[2];
    sVE_4330* vt;
};

/**
 * Handles the messenger screen's widget events: the buddy rows, the find-user keyboard and going back.
 *
 * By @p msg:
 * - 1 (focus): for the row "persona%02d" i (0..14) that @p pWidget is, selects buddy i + miScrollOffset (func_001A6070,
 *   func_001A5FB8, func_001A5128) and highlights the row (func_001A66A8(.., i, 1)), then runs cFEStateNet's handler
 *   cFEStateNet_onWidgetEvent.
 * - 2 (focus lost): hides that row's "ps2X%02d" icon, sets @p pWidget's colour (virtual setColour) to (1, 0, 0, 0) and
 *   recolours the row by the buddy's state (func_001A66A8(.., i, 0)), then cFEStateNet_onWidgetEvent.
 * - 5 (a row picked): takes the buddy at index @p pWidget +0x18 from the BuddyMan session (created and connected on demand),
 *   makes that name the current user (func_001A9890) and opens the user popup (func_001AB370).
 * - 6: reports event 20 to the parent state (its onPopupEvent, vtable entry 17).
 * - 8: opens the keyboard popup (func_001A9150: title kT_231FindPlayer, popup id 1, at most 16 characters), sets its
 *   words +0x438 and +0x58 to 1 and changes two keys (cKeyboardPopup_setIncludeKey 75 on, 53 off).
 * - 13 (the keyboard finished): func_001A4750 (find the typed user, or send to the current one).
 * - anything else: cFEStateNet_onWidgetEvent.
 *
 * @param pWidget  the widget (On Tour: cUIThing* pWidget)
 * @param msg  the widget event
 */
extern "C" void cFEStateNetMessenger_onWidgetEvent(cFEStateNetMessenger* self, sObj_4330* pWidget, int msg)
{
    char buf[0x20];
    switch (msg) {
    case 6: {
        sObj_4330* o = (sObj_4330*)self->mpParentState;
        o->vt[17].fn((char*)o + o->vt[17].delta, ((char*)self), 20);
        break;
    }
    case 8:
        func_001A9150((cFEStateNet*)self, 0, GetHashValue32(str_kT_231FindPlayer), (int)D_004A1408, 0, 1, 0x10);
        *(int*)((char*)self->mpKeyboard + 0x438) = 1;
        *(int*)((char*)self->mpKeyboard + 0x58) = 1;
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
        break;
    case 5: {
        int idx = *(int*)((char*)pWidget + 0x18);
        if (idx < 0) {
            break;
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80();
        }
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            func_002613C8(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800(LobbyMan_mLobbyMan);
        }
        void* e = func_002625C8_4330(BuddyMan_mBuddyMan, idx);
        if (e == 0) {
            break;
        }
        func_001A9890_4330(((char*)self), e);
        func_001AB370_4330(((char*)self), func_001A98B0_4330(((char*)self)), 0);
        break;
    }
    case 13:
        func_001A4750_4330(((char*)self));
        break;
    case 1:
        for (int i = 0; i < 15; i++) {
            sprintf(buf, str_personapct_02d, i);
            int id = *(int*)((char*)pWidget + 0x38);
            if (id == GetHashValue32(buf)) {
                func_001A6070_4330(((char*)self), i + self->miScrollOffset);
                func_001A5FB8_4330(((char*)self), i);
                func_001A5128_4330(((char*)self), i + self->miScrollOffset, (int*)self->mNames.mpFinish - (int*)self->mNames.mpStart);
                func_001A66A8_4330(((char*)self), i, 1);
                break;
            }
        }
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)pWidget, msg);
        break;
    case 2: {
        for (int i = 0; i < 15; i++) {
            sprintf(buf, str_personapct_02d, i);
            int id = *(int*)((char*)pWidget + 0x38);
            if (id == GetHashValue32(buf)) {
                sprintf(buf, str_ps2Xpct_02d, i);
                sObj_4330* o = (sObj_4330*)cUIScreen_getObjectByHashName_4330(self->mpScreen, GetHashValue32(buf));
                if (o != 0) {
                    o->vt[9].fn((char*)o + o->vt[9].delta, 0);
                }
                ((cObjV_4330*)pWidget)->set(cVec_4330(1.0f, 0.0f, 0.0f, 0.0f));
                func_001A66A8_4330(((char*)self), i, 0);
                break;
            }
        }
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)pWidget, msg);
        break;
    }
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4670);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262400(void* self, int a1);
extern "C" int func_00263338(void* self, int a1);

static inline void* GetSession_001A4670()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" int func_001A4670(cFEStateNetMessenger* self, int idx)
{
    if (func_00262400(GetSession_001A4670(), idx) != 0) {
        return 0;
    }
    if (func_00263338(GetSession_001A4670(), idx) != 0) {
        return 2;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4740);
#ifdef SKIP_ASM
extern "C" int func_001A4740(cFEStateNetMessenger* self, int row)
{
    return self->miScrollOffset + row;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4750);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_001A4810(cFEStateNetMessenger* self, void* a1);
extern "C" void func_00262690(void* self, int a1, void* a2);
int func_001A98B0(void* self);

static inline void* GetSession_001A4750()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001A4750(cFEStateNetMessenger* self)
{
    char* keyboard = (char*)self->mpKeyboard;
    switch (*(int*)(keyboard + 0x18)) {
    case 1:
        func_001A4810(self, keyboard + 0x74);
        break;
    case 2:
        func_00262690(GetSession_001A4750(), func_001A98B0(((cFEStateNet*)self)), (char*)self->mpKeyboard + 0x74);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4810);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025E6D0(void* self, const char* str);
extern char str_kT_POPUP220PersonaNameLength[];
extern char str_kT_BTNOK[];
extern char str_kT_POPUP231SubmittingFind[];

extern "C" void func_001A4810(cFEStateNetMessenger* self, void* text)
{
    const char* name = (const char*)text;
    if (strlen(name) < 3) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        int three = 3;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220PersonaNameLength);
        *(int*)((char*)popup + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x14C) = three;
        *(int*)((char*)popup + 0x150) = 2054;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 0;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231SubmittingFind);
        *(int*)((char*)popup + 0xC0) = 3;
        *(int*)((char*)popup + 0x150) = 2055;
        *(int*)((char*)popup + 0x14C) = 3;
        *(int*)((char*)popup + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, popup);
        cBXString_cBXString4((char*)self + 0x6D4, name);
        func_0025E6D0(LobbyMan_mLobbyMan, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4928);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" void func_001A9890(cFEStateNet* self, const char* str);
int func_001A98B0(void* self);
extern "C" void func_001AB370(cFEStateNet* self, int a1, int a2);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001A4928(cFEStateNetMessenger* self, void* popup, int event)
{
    if (event == 0xF) {
        if (strcasecmp(((const char*)LobbyMan_mLobbyMan->mPersona), (const char*)self->mszFoundName) != 0) {
            func_001A9890((cFEStateNet*)self, (const char*)self->mszFoundName);
            func_001AB370((cFEStateNet*)self, func_001A98B0(self), 0);
        }
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A49A0);
#ifdef SKIP_ASM
extern "C" void func_001A5898(cFEStateNetMessenger* self);
extern "C" void func_001A6978(cFEStateNetMessenger* self);
extern "C" void func_001ABB30(cFEStateNet* self, int a1, void* a2, int a3);
// PORT: the unit types the last parameter `int`; it is an out pointer passed through.
extern "C" void func_001A4B20_ABC50(void* self, int a1, int* out) __asm__("func_001ABC50");
extern "C" void func_001A4B20_ABE58(void* self, int a1, int* out) __asm__("func_001ABE58");
extern "C" void func_001ABED8(void* self);
extern "C" void func_001ABCD0(cFEStateNet* self, int a1, void* a2, int a3, int a4);

extern "C" void func_001A49A0(cFEStateNetMessenger* self, void* popup, unsigned int kind)
{
    int buddyErr;
    int buddyErr2;
    switch (kind) {
    case 15:
        func_001ABCD0((cFEStateNet*)self, func_001A98B0(self), popup, 15, 1);
        break;
    case 16:
        func_001A97B8(self, popup, 16);
        func_001ABB30((cFEStateNet*)self, func_001A98B0(self), popup, 16);
        break;
    case 17:
        func_001A4B20_ABE58(self, func_001A98B0(self), &buddyErr);
        if (buddyErr == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, popup, 17);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    case 18:
        func_001A4B20_ABC50(self, func_001A98B0(self), &buddyErr2);
        if (buddyErr2 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, popup, 18);
        }
        func_001A5898(self);
        break;
    default:
        func_001A97B8(self, popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4B20);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
int func_001A98B0(void* self);
extern "C" void func_001A5898(cFEStateNetMessenger* self);
extern "C" void func_001A6978(cFEStateNetMessenger* self);
extern "C" void func_001ABB30(cFEStateNet* self, int a1, void* a2, int a3);
// PORT: the unit types the last parameter `int`; it is an out pointer passed through.
extern "C" void func_001A4B20_ABC50(void* self, int a1, int* out) __asm__("func_001ABC50");
extern "C" void func_001A4B20_ABE58(void* self, int a1, int* out) __asm__("func_001ABE58");
extern "C" void func_001ABED8(void* self);

extern "C" void func_001A4B20(cFEStateNetMessenger* self, void* popup, unsigned int kind)
{
    int buddyErr;
    int buddyErr2;
    switch (kind) {
    case 15:
        func_001A97B8(self, popup, 15);
        func_001ABB30((cFEStateNet*)self, func_001A98B0(self), popup, 15);
        break;
    case 16:
        func_001A4B20_ABE58(self, func_001A98B0(self), &buddyErr);
        if (buddyErr == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, popup, 16);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    case 17:
        func_001A4B20_ABC50(self, func_001A98B0(self), &buddyErr2);
        if (buddyErr2 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, popup, 17);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    default:
        func_001A97B8(self, popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetMessenger_onPopupEvent);
#ifdef SKIP_ASM
class cFEVObj_001A4C58 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33(void* a1, int a2);
    virtual void v34(void* a1, int a2);
    virtual void v35(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

/**
 * Routes an event from one of the messenger's popups to the handler for that popup.
 *
 * Switches on the popup's miMiscData (@p pPopup +0x18, the id its creator gave it): 2057 goes to the messenger's own
 * virtual at vtable entry 33 (func_001A4928), 2059 to entry 34 (func_001A49A0) and 2060 to entry 35 (func_001A4B20),
 * each with (@p pPopup, @p popupEvent); any other popup goes to cFEStateNet's handler func_001A97B8.
 *
 * @param pPopup  the popup (On Tour: cUIState* pPopup)
 * @param popupEvent  the popup event
 */
extern "C" void cFEStateNetMessenger_onPopupEvent(cFEStateNetMessenger* self, void* pPopup, int popupEvent)
{
    switch (*(unsigned int*)((char*)pPopup + 0x18)) {
    case 2057:
        ((cFEVObj_001A4C58*)self)->v33(pPopup, popupEvent);
        break;
    case 2059:
        ((cFEVObj_001A4C58*)self)->v34(pPopup, popupEvent);
        break;
    case 2060:
        ((cFEVObj_001A4C58*)self)->v35(pPopup, popupEvent);
        break;
    default:
        func_001A97B8(((cFEVObj_001A4C58*)self), pPopup, popupEvent);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4CE8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C5780(void* self, int a1, int a2);
extern char str_kT_POPUP279UserFound[];

extern "C" void func_001A4CE8(cFEStateNetMessenger* self, void* src)
{
    cBXString_operatorE((char*)self + 0x6D0, (char*)src + 0x8);
    self->unk_6D8 = *(int*)((char*)src + 0x10);
    void* p = func_001A9C50((cFEStateNet*)self);
    int three = 3;
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xCC) = GetHashValue32(str_kT_POPUP279UserFound);
    *(int*)((char*)p + 0xC8) = three;
    func_001C5780(sub, 0, (int)self->mszFoundName);
    *(int*)((char*)p + 0x14C) = three;
    *(int*)((char*)p + 0x150) = 2057;
    func_001A9BC0((cFEStateNet*)self, p);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4D90);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char str_kT_279LookingForSelf[];
extern char str_kT_BTNOK[];

extern "C" void func_001A4D90(cFEStateNetMessenger* self)
{
    void* popup = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)popup + 0xBC) = 1;
    *(int*)((char*)popup + 0xCC) = GetHashValue32(str_kT_279LookingForSelf);
    *(int*)((char*)popup + 0xC8) = 3;
    func_001C57C0((char*)popup + 0xBC, 0, GetHashValue32(str_kT_BTNOK));
    *(int*)((char*)popup + 0x14C) = 3;
    *(int*)((char*)popup + 0x150) = 2056;
    func_001A9BC0((cFEStateNet*)self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4E28);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void sprintf1(void* str, int a1, char* buf);
// PORT: unit declares func_001A8E40 with one arg; the real handler takes (self, id, msg)
extern "C" void* func_001A8E40_msg(void* self, int id, void* msg) __asm__("func_001A8E40");
extern char str_kT_POPUP220PersonaNameLength[];
extern char str_kT_BTNOK[];
extern char str_kT_POPUP279InvalidName[];
extern char str_kT_POPUP279UserNotFound[];

struct sVEntry_001A4E28 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

extern "C" void func_001A4E28(cFEStateNetMessenger* self, void* msg)
{
    switch (*(int*)((char*)msg + 8)) {
    case 0x69706572: {
        char* buf = (char*)self + 0x9C;
        int one = 1;
        int three = 3;
        char* obj = (char*)SSXApp->FELocale;
        sVEntry_001A4E28* vt = *(sVEntry_001A4E28**)(obj + 4);
        sprintf1(buf, vt[4].fn(obj + vt[4].delta, GetHashValue32(str_kT_POPUP279InvalidName)), self->mszSearchName);
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = one;
        char* sub = (char*)popup + 0xBC;
        int textHash = GetHashValue32(str_kT_POPUP279UserNotFound);
        *(char**)((char*)popup + 0xC4) = buf;
        *(int*)((char*)popup + 0xC0) = one;
        *(int*)((char*)popup + 0xC8) = three;
        *(int*)((char*)popup + 0xCC) = textHash;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x14C) = three;
        *(int*)((char*)popup + 0x150) = 2058;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    case 0x696E616D: {
        void* popup = func_001A9C50((cFEStateNet*)self);
        int three = 3;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220PersonaNameLength);
        *(int*)((char*)popup + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x14C) = three;
        *(int*)((char*)popup + 0x150) = 2054;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    default:
        func_001A8E40_msg(self, *(int*)msg, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4FB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_001A4E28(cFEStateNetMessenger* self, void* msg);
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");

// While the popup is shown, routes its 0x807 result to func_001A4E28; otherwise to the base handler.
extern "C" void func_001A4FB0(cFEStateNetMessenger* self, int msgId, int data)
{
    if (self->mAttrState & 6) {
        if (*(int*)((char*)self->mpPopup + 0x18) == 2055) {
            func_001A4E28(self, (void*)data);
        } else {
            func_001A8E40_3(((cFEStateNet*)self), msgId, data);
        }
    } else {
        func_001A8E40_3(((cFEStateNet*)self), msgId, data);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5010);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void func_001A5898(cFEStateNetMessenger* self);
extern "C" void func_001A54D8(cFEStateNetMessenger* self);
extern "C" void func_001A52B0(cFEStateNetMessenger* self);
extern "C" void func_001A6978(cFEStateNetMessenger* self);
int func_001A98B0(void* self);
extern "C" void func_001AB3D0(cFEStateNet* self, int a1, int a2);
extern "C" void func_001A4D90(cFEStateNetMessenger* self);
extern "C" void func_001A4CE8(cFEStateNetMessenger* self, void* src);
extern "C" void func_001A4FB0(cFEStateNetMessenger* self, int a1, int a2);

// Message handler: refreshes the file list on most results, otherwise forwards to the base handler.
extern "C" void* func_001A5010(cFEStateNetMessenger* self, int a1, int a2)
{
    switch (a1) {
    case 263:
        func_001A5898(self);
    case 234: case 236: case 237: case 239: case 241: case 262:
    case 265: case 266: case 267: case 274: case 275:
        if (self->unk_6AC != 0) {
            break;
        }
        func_001A54D8(self);
        func_001A52B0(self);
        func_001A6978(self);
        func_001AB3D0(((cFEStateNet*)self), func_001A98B0(((cFEStateNet*)self)), 0);
        break;
    case 270:
        func_001A4D90(self);
        break;
    case 227:
        func_001A4CE8(self, (void*)a2);
        break;
    case 258:
        func_001A4FB0(self, a1, a2);
        break;
    default:
        func_001AB3D0(((cFEStateNet*)self), func_001A98B0(((cFEStateNet*)self)), 0);
        return func_001A8E40_3(((cFEStateNet*)self), a1, a2);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5128);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char str_ScrollVector[];
extern char str_ScrollButton[];

struct sVec3_001A5128 {
    float x;
    float y;
    float z;
};

struct cUIObj_001A5128 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20(void* out);
};

static inline sVec3_001A5128 GetPos_001A5128(cUIObj_001A5128* o)
{
    return *(sVec3_001A5128*)((char*)o + 0x44);
}

extern "C" void func_001A5128(cFEStateNetMessenger* self, int pos, int count)
{
    if (self->mpScreen == 0) return;
    if (pos < 0) return;
    if (count < pos) return;
    if (count <= 0) return;
    cUIObj_001A5128* track = (cUIObj_001A5128*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ScrollVector));
    cUIObj_001A5128* thumb = (cUIObj_001A5128*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ScrollButton));
    if (thumb == 0) return;
    if (track == 0) return;
    sVec3_001A5128 size;
    track->v20(&size);
    sVec3_001A5128 p = *(sVec3_001A5128*)((char*)track + 0x44);
    float fpos = (float)pos;
    if (count >= 2) count--;
    float step = (size.y + -15.0f) / (float)count;
    sVec3_001A5128 np = p;
    np.y += fpos * step;
    np.x += -2.0f;
    *(sVec3_001A5128*)((char*)thumb + 0x44) = np;
    thumb->v09(1);
    track->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A52B0);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void* func_00263308(void* self, int a1);
extern "C" int func_00262938(void* self);
extern "C" void* func_00263630(void* self, int a1);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
struct sOwner1A5800;
extern "C" int func_001A5800(cFEStateNetMessenger* self, char** name);

struct sSession1A52B0 {
    char pad[0xBC];
    void** mBegin;
    void** mEnd;
};

static inline int Count1A52B0(sSession1A52B0* s) { return s->mEnd - s->mBegin; }

static inline void* GetSession_001A52B0()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

struct sBXString1A52B0 {
    char* str;
    sBXString1A52B0() {}
    sBXString1A52B0(const sBXString1A52B0& o);
};

extern "C" void func_001A52B0(cFEStateNetMessenger* self)
{
    void* name;
    if ((Count1A52B0((sSession1A52B0*)GetSession_001A52B0()) != 0 && (name = func_00263308(GetSession_001A52B0(), 0)) != 0)
        || (func_00262938(GetSession_001A52B0()) > 0 && (name = func_00263630(GetSession_001A52B0(), 0)) != 0)) {
        sBXString1A52B0 nameStr;
        cBXString_cBXString1(&nameStr, name);
        self->miSelectedIndex = func_001A5800(self, &nameStr.str);
        cBXString__cBXString(&nameStr, 2);
    } else {
        self->miSelectedIndex = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5450);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* func_0041610C(void* dst, void* src, int n);

struct sPtrVec1A5450 {
    void** mBegin;
    void** mEnd;
};


extern "C" void func_001A5450(cFEStateNetMessenger* self)
{
    for (;;) {
        if ((*(sPtrVec1A5450*)&self->mNames.mpStart).mBegin == (*(sPtrVec1A5450*)&self->mNames.mpStart).mEnd) break;
        void* nameStr = (*(sPtrVec1A5450*)&self->mNames.mpStart).mEnd[-1];
        (*(sPtrVec1A5450*)&self->mNames.mpStart).mEnd--;
        if (nameStr) {
            cBXString__cBXString(nameStr, 3);
        }
    }
    void** first = (*(sPtrVec1A5450*)&self->mNames.mpStart).mBegin;
    void** last = (*(sPtrVec1A5450*)&self->mNames.mpStart).mEnd;
    void** finish = last;
    func_0041610C(first, last, (char*)finish - (char*)last);
    (*(sPtrVec1A5450*)&self->mNames.mpStart).mEnd -= last - first;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A54D8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262598(void* self);
extern "C" const char* func_002625C8(void* self, int idx);
extern "C" int func_00262438(void* self, const char* name);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
struct sOwner1A56D0;
extern "C" int func_001A56D0(cFEStateNetMessenger* self, const char* name);
extern "C" void func_001DB778(void* vec, void** pos, void* const* val);
extern char str_cBXString[];

static inline void* GetSession_001A54D8()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

struct sElem1A54D8 {
    void* v;
    sElem1A54D8(void* const& x) : v(x) {}
    void* operator new(unsigned int, void* p) throw() { return p; }
};

static inline void Construct1A54D8(sElem1A54D8* p, void* const& x) { new (p) sElem1A54D8(x); }



static inline void PushBack1A54D8(cFEStateNetMessenger* feStateNetMessenger, void* x)
{
    tFEStringPtrVector* feStringPtrVector = &(*(&feStateNetMessenger->mNames));
    if (((sElem1A54D8*)(*(&feStateNetMessenger->mNames)).mpFinish) != ((sElem1A54D8*)(*(&feStateNetMessenger->mNames)).mpEndOfStorage)) {
        Construct1A54D8(((sElem1A54D8*)feStringPtrVector->mpFinish), x);
        ++(*(sElem1A54D8**)&(*(&feStateNetMessenger->mNames)).mpFinish);
    } else {
        func_001DB778(feStringPtrVector, (void**)((sElem1A54D8*)(*(&feStateNetMessenger->mNames)).mpFinish), &x);
    }
}

extern "C" void func_001A54D8(cFEStateNetMessenger* self)
{
    if (*(int*)((char*)GetSession_001A54D8() + 0xE8) != 0) {
        int i;
        for (i = 0; i < func_00262598(GetSession_001A54D8()); i++) {
            const char* name = func_002625C8(GetSession_001A54D8(), i);
            if (name != 0 && func_00262438(GetSession_001A54D8(), name) != 0 && func_001A56D0(self, name) == 0) {
                void* nameStr = cBXString_cBXString2(cMemMan_alloc(4, str_cBXString, 0x20000000, 0), name);
                PushBack1A54D8(self, nameStr);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A56D0);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);


static inline unsigned int Count1A56D0(cFEStateNetMessenger* feStateNetMessenger) { return ((char***)feStateNetMessenger->mNames.mpFinish) - ((char***)feStateNetMessenger->mNames.mpStart); }

extern "C" int func_001A56D0(cFEStateNetMessenger* self, const char* name)
{
    unsigned int i;
    for (i = 0; i < Count1A56D0((self)); i++) {
        if (strcasecmp(**(char***)((char*)(char***)self->mNames.mpStart + (i << 2)), name) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5768);
#ifdef SKIP_ASM

static inline unsigned int Count1A5768(cFEStateNetMessenger* feStateNetMessenger) { return ((char***)feStateNetMessenger->mNames.mpFinish) - ((char***)feStateNetMessenger->mNames.mpStart); }

extern "C" int func_001A5768(cFEStateNetMessenger* self, const char* name)
{
    unsigned int i;
    for (i = 0; i < Count1A5768((self)); i++) {
        if (strcasecmp(**(char***)((char*)(char***)self->mNames.mpStart + (i << 2)), name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5800);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);


static inline unsigned int Count1A5800(cFEStateNetMessenger* feStateNetMessenger) { return ((char***)feStateNetMessenger->mNames.mpFinish) - ((char***)feStateNetMessenger->mNames.mpStart); }

extern "C" int func_001A5800(cFEStateNetMessenger* self, char** name)
{
    unsigned int i;
    for (i = 0; i < Count1A5800((self)); i++) {
        if (strcasecmp(**(char***)((char*)(char***)self->mNames.mpStart + (i << 2)), *name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5898);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262598(void* self);
extern "C" const char* func_002625C8(void* self, int idx);
extern "C" void* func_00263308(void* self, int a1);
extern "C" int func_00262468(void* self, const char* name);
extern "C" int func_00262400(void* self, int a1);
extern "C" int func_00263128(void* self, const char* name);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
struct sOwner1A5450;
extern "C" void func_001A5450(cFEStateNetMessenger* self);
extern "C" void func_001DB778(void* vec, void** pos, void* const* val);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern char str_cBXString[];
extern char str_playerMenu[];

static inline void* GetSession_001A5898()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

struct sSession1A5898 {
    char pad[0xBC];
    void** mBegin;
    void** mEnd;
    char padC4[0x24];
    int mEnabled;           // 0xE8
};

static inline int Count1A5898(sSession1A5898* s) { return s->mEnd - s->mBegin; }

struct sElem1A5898 {
    void* v;
    sElem1A5898(void* const& x) : v(x) {}
    void* operator new(unsigned int, void* p) throw() { return p; }
};

static inline void Construct1A5898(sElem1A5898* p, void* const& x) { new (p) sElem1A5898(x); }



static inline void PushBack1A5898(cFEStateNetMessenger* feStateNetMessenger, void* x)
{
    tFEStringPtrVector* feStringPtrVector = &(*(&feStateNetMessenger->mNames));
    void* const* px = &x;
    if (((sElem1A5898*)(*(&feStateNetMessenger->mNames)).mpFinish) != ((sElem1A5898*)(*(&feStateNetMessenger->mNames)).mpEndOfStorage)) {
        Construct1A5898(((sElem1A5898*)feStringPtrVector->mpFinish), x);
        ++(*(sElem1A5898**)&(*(&feStateNetMessenger->mNames)).mpFinish);
    } else {
        func_001DB778(feStringPtrVector, (void**)((sElem1A5898*)(*(&feStateNetMessenger->mNames)).mpFinish), px);
    }
}

extern "C" void func_001A5898(cFEStateNetMessenger* vself)
{
    cFEStateNetMessenger* self = vself;
    if (((sSession1A5898*)GetSession_001A5898())->mEnabled == 0) {
        return;
    }
    func_001A5450(self);
    int count = func_00262598(GetSession_001A5898());
    if (count == 0) {
        return;
    }
    int j;
    for (j = 0; j < Count1A5898((sSession1A5898*)GetSession_001A5898()); j++) {
        void** name = (void**)func_00263308(GetSession_001A5898(), j);
        if (name != 0 && func_00262468(GetSession_001A5898(), (const char*)*name) >= 0) {
            PushBack1A5898(self, cBXString_cBXString1(cMemMan_alloc(4, str_cBXString, 0x20000000, 0), name));
        }
    }
    int i;
    for (i = 0; i < count; i++) {
        const char* name = func_002625C8(GetSession_001A5898(), i);
        if (name != 0 && name[0xA7] != 0 && func_00263128(GetSession_001A5898(), name) == 0
            && func_00262400(GetSession_001A5898(), i) == 0) {
            PushBack1A5898(self, cBXString_cBXString2(cMemMan_alloc(4, str_cBXString, 0x20000000, 0), name));
        }
    }
    for (i = 0; i < count; i++) {
        const char* name = func_002625C8(GetSession_001A5898(), i);
        if (name != 0 && func_00262400(GetSession_001A5898(), i) != 0 && func_00263128(GetSession_001A5898(), name) == 0) {
            PushBack1A5898(self, cBXString_cBXString2(cMemMan_alloc(4, str_cBXString, 0x20000000, 0), name));
        }
    }
    for (i = 0; i < count; i++) {
        const char* name = func_002625C8(GetSession_001A5898(), i);
        if (name != 0 && name[0xA7] == 0 && func_00263128(GetSession_001A5898(), name) == 0
            && func_00262400(GetSession_001A5898(), i) == 0) {
            PushBack1A5898(self, cBXString_cBXString2(cMemMan_alloc(4, str_cBXString, 0x20000000, 0), name));
        }
    }
    if (((void*)self->mpScreen) != 0) {
        char* list = (char*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_playerMenu));
        int n = ((sElem1A5898*)(*(&self->mNames)).mpFinish) - ((sElem1A5898*)(*(&self->mNames)).mpStart);
        if (*(unsigned char*)(list + 0x96) != n) {
            cUIMenu_setScrollableMenu(list, n);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5FB8);
#ifdef SKIP_ASM
struct sVE1A5FB8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern char str_ps2Xpct_02d[];
extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A66A8(cFEStateNetMessenger* self, unsigned int index, int flag);

extern "C" void func_001A5FB8(cFEStateNetMessenger* self, int sel)
{
    char buf[32];
    int i;
    for (i = 0; i < 15; i++) {
        sprintf(buf, str_ps2Xpct_02d, i);
        void* obj = (void*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (obj != 0) {
            sVE1A5FB8* vt = *(sVE1A5FB8**)((char*)obj + 0x8);
            vt[9].fn((char*)obj + vt[9].delta, i == sel);
        }
        func_001A66A8(self, sel, i == sel);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6070);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_002624F8(void* self, int id);
extern "C" const char* func_002625C8(void* self, int idx);
extern "C" int func_00263338(void* self, int a1);
extern "C" int func_002633F8(void* self, int a1);
extern "C" void func_001A7390(cFEStateNet* self, const char* str, int a2);
extern char D_004A1390[];
extern char str_TextBuddyLocation[];
extern char str_TextBuddyName[];
extern char str_kT_279NoBuddies[];
extern char str_kT_CMNUpdateEAMsgr[];
extern char str_kT_279PresenceOffline[];

static inline void* GetSession_001A6070()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

class cFEVObj_001A6070 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" int func_001A6070(cFEStateNetMessenger* self, int idx)
{
    if (self->mpScreen == 0) {
        return 0;
    }
    cFEVObj_001A6070* name = (cFEVObj_001A6070*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextBuddyName));
    cFEVObj_001A6070* status = (cFEVObj_001A6070*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextBuddyLocation));
    if (status == 0 || name == 0) {
        return 0;
    }
    int n = (int*)self->mNames.mpFinish - (int*)self->mNames.mpStart;
    if (n == 0) {
        cUIText_setAsciiString((cUIText*)name, D_004A1390);
        if (*(int*)((char*)GetSession_001A6070() + 0xE8) != 0) {
            cUIText_setUnicodeStringByID((cUIText*)status, GetHashValue32(str_kT_279NoBuddies));
        } else {
            cUIText_setUnicodeStringByID((cUIText*)status, GetHashValue32(str_kT_CMNUpdateEAMsgr));
            GetSession_001A6070();
        }
        return 0;
    }
    if (idx >= n) {
        return 0;
    }
    int e = *(int*)((char*)self->mNames.mpStart + (idx << 2));
    int id = func_002624F8(GetSession_001A6070(), e);
    const char* str = func_002625C8(GetSession_001A6070(), id);
    if (str == 0) {
        return 0;
    }
    cBXString_cBXString4((char*)self + 0x6E0, str);
    cUIText_setAsciiString((cUIText*)name, str);
    name->v09(1);
    status->v09(1);
    if (func_00263338(GetSession_001A6070(), id) != 0) {
        func_001A7390((cFEStateNet*)self, str, func_002633F8(GetSession_001A6070(), id));
    } else {
        cUIText_setUnicodeStringByID((cUIText*)status, GetHashValue32(str_kT_279PresenceOffline));
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A63C8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_IconMessagepct_02d[];
extern char str_IconBlockedpct_02d[];
extern char str_IconChallengepct_02d[];

class cFEVObj_001A63C8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001A63C8(cFEStateNetMessenger* self, unsigned int index, int state)
{
    char name[32];
    if (self->mpScreen == 0) {
        return;
    }
    if (index >= 15) {
        return;
    }
    sprintf(name, str_IconMessagepct_02d, index);
    cFEVObj_001A63C8* a = (cFEVObj_001A63C8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    sprintf(name, str_IconBlockedpct_02d, index);
    cFEVObj_001A63C8* b = (cFEVObj_001A63C8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    sprintf(name, str_IconChallengepct_02d, index);
    cFEVObj_001A63C8* c = (cFEVObj_001A63C8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    switch (state) {
    case 0:
        if (a) a->v09(0);
        if (b) b->v09(1);
        if (c) c->v09(0);
        break;
    case 1:
        if (a) a->v09(1);
        if (b) b->v09(0);
        if (c) c->v09(0);
        break;
    case 2:
        if (a) a->v09(0);
        if (b) b->v09(0);
        if (c) c->v09(1);
        break;
    default:
        if (a) a->v09(0);
        if (b) b->v09(0);
        if (c) c->v09(0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6618);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char str_personapct_02d[];

extern "C" void func_001A6618(cFEStateNetMessenger* self, unsigned int index, const char* str, int a3)
{
    char buf[32];
    if (self->mpScreen != 0 && index < 0xF) {
        sprintf(buf, str_personapct_02d, index);
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (text != 0) {
            cUIText_setAsciiString(text, str);
            *(int*)((char*)text + 0x18) = a3;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A66A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_001A4670(cFEStateNetMessenger* self, int idx);
extern "C" int func_001A4740(cFEStateNetMessenger* self, int a1);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_002624F8(void* self, int id);
extern char str_personapct_02d[];
extern char str_overlaypct_02d[];

static inline void* GetSession_001A66A8()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

struct sCol_001A66A8 {
    float r, g, b, a;
    sCol_001A66A8() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
    sCol_001A66A8(const float& r_) : r(r_), g(0.0f), b(0.0f), a(0.0f) {}
    sCol_001A66A8(const float& r_, const float& g_, const float& b_, const float& a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cFEVObj_001A66A8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sCol_001A66A8& c);
};

extern "C" void func_001A66A8(cFEStateNetMessenger* self, unsigned int index, int flag)
{
    char name[32];
    if (self->mpScreen == 0 || index >= 15) {
        return;
    }
    sprintf(name, str_personapct_02d, index);
    cFEVObj_001A66A8* persona = (cFEVObj_001A66A8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    sprintf(name, str_overlaypct_02d, index);
    cFEVObj_001A66A8* overlay = (cFEVObj_001A66A8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    if (flag != 0) {
        overlay->v11(sCol_001A66A8(1.0f, 0.6078431606292725f, 0.24313727021217346f, 0.0f));
        persona->v11(sCol_001A66A8(1.0f, 1.0f, 1.0f, 1.0f));
        return;
    }
    unsigned int idx = func_001A4740(self, index);
    if (idx >= (unsigned int)((int*)self->mNames.mpFinish - (int*)self->mNames.mpStart)) {
        overlay->v11(sCol_001A66A8());
        return;
    }
    persona->v11(sCol_001A66A8(1.0f));
    switch (func_001A4670(self, func_002624F8(GetSession_001A66A8(), *(int*)((char*)self->mNames.mpStart + (idx << 2))))) {
    case 2:
        overlay->v11(sCol_001A66A8(1.0f, 0.5372549295425415f, 0.7411764860153198f, 0.874509871006012f));
        break;
    case 0:
        overlay->v11(sCol_001A66A8(1.0f, 0.501960813999176f, 0.501960813999176f, 0.501960813999176f));
        break;
    case 1:
    default:
        overlay->v11(sCol_001A66A8(1.0f, 0.30588236451148987f, 0.545098066329956f, 0.7019608020782471f));
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6978);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A5FB8(cFEStateNetMessenger* self, int sel);
extern "C" int func_001A6070(cFEStateNetMessenger* self, int sel);
extern "C" void func_001A66A8(cFEStateNetMessenger* self, unsigned int index, int flag);
extern "C" void func_001A6CA0(cFEStateNetMessenger* self);
struct sOwner1A6E70;
extern "C" void func_001A6E70(cFEStateNetMessenger* self);
extern "C" void func_001A71B0(cFEStateNetMessenger* self, unsigned int index, const char* name, int a3);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_002624F8(void* self, int id);
extern "C" const char* func_002625C8(void* self, int idx);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern char str_playerMenu[];

static inline void* GetSession_001A6978()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}



static inline unsigned int Size_001A6978(cFEStateNetMessenger* feStateNetMessenger) { return ((int*)(*(&feStateNetMessenger->mNames)).mpFinish) - ((int*)(*(&feStateNetMessenger->mNames)).mpStart); }

extern "C" void func_001A6978(cFEStateNetMessenger* vself)
{
    cFEStateNetMessenger* self = vself;
    if (((void*)self->mpScreen) == 0) {
        return;
    }
    func_001A6E70(self);
    func_001A6CA0((cFEStateNetMessenger*)self);
    unsigned int n = Size_001A6978(self);
    unsigned int first = ((unsigned int)self->miScrollOffset);
    if (n >= 16) {
        if (n - first < 15) {
            int top = n - 15;
            first = top;
        }
    }
    int count = 0;
    int row = 0;
    while (row < 15 && first < Size_001A6978(self)) {
        int id = *(int*)((char*)((int*)(*(&self->mNames)).mpStart) + (first << 2));
        int idx = func_002624F8(GetSession_001A6978(), id);
        if (idx < 0) {
            func_001A71B0((cFEStateNetMessenger*)self, row, 0, -1);
        } else {
            count++;
            func_001A71B0((cFEStateNetMessenger*)self, row, func_002625C8(GetSession_001A6978(), idx), idx);
            func_001A66A8((cFEStateNetMessenger*)self, row, 0);
        }
        row++;
        first++;
    }
    cUIText* menu = cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_playerMenu));
    if (menu != 0) {
        if (*(unsigned char*)((char*)menu + 0x96) != Size_001A6978(self)) {
            cUIMenu_setScrollableMenu(menu, Size_001A6978(self));
            if (Size_001A6978(self) == 0) {
                cUIMenu_setScrollableMenu(menu, 1);
            }
        }
        if (func_001A6070((cFEStateNetMessenger*)self, *(unsigned char*)((char*)menu + 0x95)) != 0 && Size_001A6978(self) != 0) {
            func_001A5FB8((cFEStateNetMessenger*)((cFEStateNet*)self), *(unsigned char*)((char*)menu + 0x95) - *(unsigned char*)((char*)menu + 0x98));
            func_001A66A8((cFEStateNetMessenger*)self, *(unsigned char*)((char*)menu + 0x95) - *(unsigned char*)((char*)menu + 0x98), 1);
        } else {
            func_001A5FB8((cFEStateNetMessenger*)((cFEStateNet*)self), -1);
        }
    }
    for (row = count; row < 15; row++) {
        func_001A71B0((cFEStateNetMessenger*)self, row, 0, -1);
        func_001A66A8((cFEStateNetMessenger*)self, row, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6C40);
#ifdef SKIP_ASM
extern char str_playerMenu[];
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" void func_001A6C40(cFEStateNetMessenger* self, int idx)
{
    if (self->mpScreen != 0 && idx >= 0) {
        void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_playerMenu));
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, idx);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6CA0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A52B0(cFEStateNetMessenger* self);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern char str_ps2L1[];
extern char str_TextEqGoToNext[];
extern char str_IconNextChallenge[];
extern char str_IconNextMessage[];

struct sSession1A6CA0 {
    char pad[0xBC];
    void** mBegin;
    void** mEnd;
};

static inline int Count1A6CA0(sSession1A6CA0* s) { return s->mEnd - s->mBegin; }

static inline void* GetSession_001A6CA0()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

class cFEVObj_001A6CA0 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001A6CA0(cFEStateNetMessenger* self)
{
    if (self->mpScreen == 0) {
        return;
    }
    func_001A52B0(self);
    int has = self->miSelectedIndex >= 0;
    cFEVObj_001A6CA0* o = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2L1));
    if (o != 0) {
        o->v09(has);
    }
    o = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextEqGoToNext));
    if (o != 0) {
        o->v09(has);
    }
    cFEVObj_001A6CA0* a = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_IconNextChallenge));
    cFEVObj_001A6CA0* b = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_IconNextMessage));
    if (a != 0 && b != 0) {
        if (!has) {
            a->v09(0);
            b->v09(0);
        } else if (Count1A6CA0((sSession1A6CA0*)GetSession_001A6CA0()) != 0) {
            a->v09(has);
            b->v09(0);
        } else {
            b->v09(has);
            a->v09(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6E70);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_002624F8(void* self, int id);
extern "C" void* func_0041610C(void* dst, void* src, int n);

static inline void* GetSession_001A6E70()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}


static inline int* Copy1A6E70(int* first, int* last, int* result)
{
    func_0041610C(result, first, (last - first) * sizeof(int));
    return result + (last - first);
}


extern "C" void func_001A6E70(cFEStateNetMessenger* self)
{
    int i;
    for (i = (((int*)(*(&self->mNames)).mpFinish) - ((int*)(*(&self->mNames)).mpStart)) - 1; i >= 0; i--) {
        tFEStringPtrVector* feStringPtrVector = &(*(&self->mNames));
        int id = *(int*)((char*)((int*)(*(&self->mNames)).mpStart) + (i << 2));
        if (func_002624F8(GetSession_001A6E70(), id) < 0) {
            // PORT: pointer arithmetic through int
            int* pos = (int*)(i * 4 + (int)((int*)(*(&self->mNames)).mpStart));
            if (pos + 1 != ((int*)(*(&self->mNames)).mpFinish)) {
                Copy1A6E70(pos + 1, ((int*)feStringPtrVector->mpFinish), pos);
            }
            (*(int**)&(*(&self->mNames)).mpFinish)--;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A6F70);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262598(void* self);
extern "C" const char* func_002625C8(void* self, int idx);
extern "C" int func_00262400(void* self, int a1);
extern "C" int func_00263128(void* self, const char* name);
// PORT: the unit declares func_00262A38 with an int name parameter; this caller passes a string
extern "C" int func_00262A38(void* self, int a1);
extern "C" void func_00261EF8(void* self, int a1, int a2, int a3);

static inline void* GetSession_001A6F70()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

// Walks the buddy list (BuddyMan_mBuddyMan, created on demand): for each named entry for which func_00262400 is set,
// calls func_00261EF8(name, 2, buf) when neither func_00263128 nor func_00262A38 knows the name yet.
extern "C" void func_001A6F70(void)
{
    char buf[0x10];
    int n = func_00262598(GetSession_001A6F70());
    for (int i = 0; i < n; i++) {
        const char* name = func_002625C8(GetSession_001A6F70(), i);
        if (name != 0 && func_00262400(GetSession_001A6F70(), i) != 0) {
            int hasEntry = func_00263128(GetSession_001A6F70(), name);
            int b = func_00262A38(GetSession_001A6F70(), (int)name);
            if (hasEntry == 0 && b == 0)
                func_00261EF8(GetSession_001A6F70(), (int)name, 2, (int)buf); // PORT: pointer passed as int (unit prototype)
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A71B0);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00263128(void* self, const char* name);
extern "C" int func_002623B0(void* self, const char* name);
// PORT: the unit declares func_00262A38 with an int name parameter; this caller passes a string
extern "C" int func_00262A38(void* self, int a1);
extern "C" void func_001A63C8(cFEStateNetMessenger* self, unsigned int index, int state);
extern "C" void func_001A6618(cFEStateNetMessenger* self, unsigned int index, const char* str, int a3);
extern "C" void func_001A66A8(cFEStateNetMessenger* self, unsigned int index, int flag);
extern char D_004A1390[];

static inline void* GetSession_001A71B0()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001A71B0(cFEStateNetMessenger* self, unsigned int index, const char* name, int a3)
{
    if (index >= 15) {
        return;
    }
    if (name == 0) {
        func_001A63C8(self, index, 3);
        func_001A6618(self, index, D_004A1390, a3);
        func_001A66A8(self, index, 1);
        return;
    }
    if (func_00263128(GetSession_001A71B0(), name)) {
        func_001A63C8(self, index, 2);
    } else if (func_002623B0(GetSession_001A71B0(), name)) {
        func_001A63C8(self, index, 0);
    } else if (func_00262A38(GetSession_001A71B0(), (int)name)) {
        func_001A63C8(self, index, 1);
    } else {
        func_001A63C8(self, index, 3);
    }
    func_001A6618(self, index, name, a3);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7390);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
int UIAsciiToUnicode_i(unsigned short* dst, const char* src) __asm__("UIAsciiToUnicode__FPUsPCc");
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" void func_001B25E0(const char* src, void* out);
extern "C" void strcpy1(void* str, int a1);
extern "C" void func_002C2688(void* str, unsigned short* s);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...); the unit declares a 3-arg view
extern "C" void sprintf1_1A7390(void* str, int fmt, ...) __asm__("sprintf1");
extern "C" unsigned short* func_00392680(void* f, float sx, float width, const unsigned short* str, unsigned short* dst, int max);
extern "C" char* cUIText_getFont(void* text);
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern "C" void func_003EFA48(unsigned short* dst, int n, const char* src);
extern "C" char* strcat(char*, const char*);
extern "C" char* strcpy(char* dst, const char* src);
extern char D_004A1390[];
extern char D_004A1408[];
extern char str_LOCID[];
extern char str_SSX3ID[];
extern char str_SZ1[];
extern char str_SZ2[];
extern char str_SZ3[];
extern char str_TextBuddyLocation[];
extern char str_TextBuddyName[];
extern char str_kT_279PresenceOffline[];
extern char str_kT_279PresenceOnline[];
extern char str_kT_279PresenceLobby[];
extern char str_kT_279PresenceNews[];
extern char str_kT_279PresenceStats[];
extern char str_kT_279PresenceMessenger[];
extern char str_kT_279PresenceSelecting[];
extern char str_kT_279PresenceRoom[];
extern char str_kT_279PresenceBeingChallenged[];
extern char str_kT_279PresenceChallenging[];
extern char str_kT_279PresenceRacing[];

struct sVEntry_001A7390 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sVEntryP_001A7390 {
    short delta;
    short index;
    int (*fn)(void*, float*);
};

struct sVec3_001A7390 {
    float x, y, z;
};

struct sChar2_001A7390 {
    char c[2];
};

static inline int Loc1A7390(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001A7390* vt = *(sVEntry_001A7390**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

extern "C" void func_001A7390(cFEStateNet* self, const char* name, int mode)
{
    unsigned short out[80];
    char s1[0x20];
    char s2[0x20];
    char s3[0x20];
    void* screen = self->mpScreen;
    if (screen == 0 || name == 0) {
        return;
    }
    cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextBuddyName));
    cUIText* t2 = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextBuddyLocation));
    if (t2 == 0) {
        return;
    }
    const char* tags = name + 0x54;
    unsigned int state = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(tags, str_LOCID), -1);
    cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(tags, str_SSX3ID), -1);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(tags, str_SZ1), s1, 0x20, D_004A1408);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(tags, str_SZ2), s2, 0x20, D_004A1408);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(tags, str_SZ3), s3, 0x20, D_004A1408);
    if (mode == 0) {
        char buf[0x60];
        unsigned short wide[0x60];
        unsigned short wrapped[0x60];
        float size[4];
        strcpy(buf, name);
        strcat(buf, D_004A1390);
        strcat(buf, tags);
        func_003EFA48(wide, 0x60, buf);
        sVEntryP_001A7390* vt = *(sVEntryP_001A7390**)((char*)uiText + 8);
        vt[20].fn((char*)uiText + vt[20].delta, size);
        sVec3_001A7390 scale = *(sVec3_001A7390*)((char*)uiText + 0x50);
        unsigned short* rest = func_00392680(cUIText_getFont(uiText), scale.x, size[0], wide, wrapped, 0x30);
        cUIText_setUnicodeString(uiText, (char*)wrapped);
        if (rest != 0) {
            cUIText_setUnicodeString(t2, (char*)rest);
        } else {
            cUIText_setAsciiString(t2, D_004A1390);
        }
        return;
    }
    switch (state) {
    case 1:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(str_kT_279PresenceOffline));
        break;
    case 0:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(str_kT_279PresenceOnline));
        break;
    case 2:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(str_kT_279PresenceLobby));
        break;
    case 4:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(str_kT_279PresenceNews));
        break;
    case 5:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(str_kT_279PresenceStats));
        break;
    case 10:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(str_kT_279PresenceMessenger));
        break;
    case 6:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(str_kT_279PresenceSelecting));
        break;
    case 3: {
        unsigned short w1[0x40];
        int str = Loc1A7390(str_kT_279PresenceRoom);
        sChar2_001A7390 sp = *(sChar2_001A7390*)D_004A1390;
        unsigned short wsp[8];
        UIAsciiToUnicode_i(wsp, sp.c);
        func_001B25E0(s1, w1);
        strcpy1(out, str);
        func_002C2688(out, wsp);
        func_002C2688(out, w1);
        cUIText_setUnicodeString(t2, (char*)out);
        break;
    }
    case 8:
        sprintf1_1A7390(out, Loc1A7390(str_kT_279PresenceBeingChallenged), s1);
        cUIText_setUnicodeString(t2, (char*)out);
        break;
    case 7:
        sprintf1_1A7390(out, Loc1A7390(str_kT_279PresenceChallenging), s1);
        cUIText_setUnicodeString(t2, (char*)out);
        break;
    case 9:
        sprintf1_1A7390(out, Loc1A7390(str_kT_279PresenceRacing), s1);
        cUIText_setUnicodeString(t2, (char*)out);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup_cFEStateReadMessagePopup);
#ifdef SKIP_ASM
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern char cFEStateReadMessagePopup_vtbl[];

struct sColor1A77A0 {
    float r, g, b, a;
};

struct sScreen1A77A0 {
    char pad0[0x8];
    void* vt;               // 0x8
    int id;                 // 0xC
    char pad10[0x6D0 - 0x10];
    char name[4];           // 0x6D0
    int f6D4;               // 0x6D4
    int f6D8;
    int f6DC;               // 0x6DC
    int f6E0;               // 0x6E0
    int f6E4;               // 0x6E4
    int f6E8;               // 0x6E8
    sColor1A77A0 color;     // 0x6EC
};

// PORT: the unit declares this as (void*, int, void*, int); the body uses typed arguments.
extern "C" void* cFEStateReadMessagePopup_cFEStateReadMessagePopup_impl(sScreen1A77A0* self, int a1, int a2, const char* name) __asm__("cFEStateReadMessagePopup_cFEStateReadMessagePopup");

/**
 * Constructs the private-message conversation popup for one sender.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with the engine and parent, stores cFEStateReadMessagePopup_vtbl,
 * builds mszSender from @p name, sets mbResetScroll 1, mHashName 0x49, unk_6D4 0, empties mMessages and sets
 * mTextColour to white (1, 1, 1, 1).
 *
 * @param engine  the UI engine (On Tour cUIState's pEngine)
 * @param owner  the parent state, the messenger (On Tour pParentState)
 * @param name  the other player's name
 * @return @p self
 */
extern "C" void* cFEStateReadMessagePopup_cFEStateReadMessagePopup_impl(sScreen1A77A0* self, int engine, int owner, const char* name)
{
    // An unused 16-byte local is needed to reproduce the 0x50-byte frame.
    int frameFiller[4];
    func_001A8500_3(self, engine, owner);
    self->vt = cFEStateReadMessagePopup_vtbl;
    cBXString_cBXString2(self->name, name);
    self->f6E8 = 1;
    self->id = 73;
    self->f6D4 = 0;
    self->f6DC = 0;
    self->f6E0 = 0;
    self->f6E4 = 0;
    sColor1A77A0 white;
    white.r = 1.0f;
    white.g = 1.0f;
    white.b = 1.0f;
    white.a = 1.0f;
    self->color = white;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup__cFEStateReadMessagePopup);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* func_0041610C(void* dst, void* src, int n);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern char cFEStateReadMessagePopup_vtbl[];

struct sVE1A7848 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj1A7848 {
    char pad[0xC];
    sVE1A7848* vt;
};

struct sObjVec1A7848 {
    int f0;
    sObj1A7848** mBegin;
    sObj1A7848** mEnd;
    sObj1A7848** mCap;
};

static inline void Dealloc1A7848(sObj1A7848** p, int n)
{
    if (p != 0) {
        if (n == 1) {
            operator_delete((int*)p);
        } else {
            cMemMan_free(p);
        }
    }
}

struct sOwner1A7848 {
    char pad0[0x8];
    void* vt;
    char padC[0x6C4];
    char str[8];
    sObjVec1A7848 mVec;
};

/**
 * Destroys the private-message popup: deletes every message, frees the vector and destroys mszSender.
 *
 * Pops each cPrivateMessage from the end of mMessages and deletes it through its slot 1 (flags 3), frees the vector's
 * storage, destroys the sender string and runs the cFEStateNet destructor (func_001A85D0) with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateReadMessagePopup__cFEStateReadMessagePopup(sOwner1A7848* self, int flags)
{
    self->vt = cFEStateReadMessagePopup_vtbl;
    for (;;) {
        if (self->mVec.mBegin == self->mVec.mEnd) break;
        sObj1A7848* message = self->mVec.mEnd[-1];
        self->mVec.mEnd--;
        if (message) {
            message->vt[1].fn((char*)message + message->vt[1].delta, 3);
        }
    }
    sObjVec1A7848* messages = &self->mVec;
    sObj1A7848** first = self->mVec.mBegin;
    sObj1A7848** last = self->mVec.mEnd;
    func_0041610C(first, last, (messages->mEnd - last) * sizeof(sObj1A7848*));
    self->mVec.mEnd -= last - first;
    Dealloc1A7848(messages->mBegin, self->mVec.mCap - self->mVec.mBegin);
    cBXString__cBXString(self->str, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUITextScroll_setInputMap(void* self, int a1);
extern char str_282ReadPrivateMessage[];
extern char str_AgreementSlider[];
extern char str_ChatUpArrow[];
extern char str_ChatDownArrow[];
extern char str_slideroutline2[];
extern char str_textScrollChat[];

struct sCol1A7970 {
    float r, g, b, a;
    sCol1A7970(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct sBlk1A7970 {
    int v[4];
};

class cFEVObj_001A7970 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sCol1A7970& c);
};

/**
 * Creates the conversation screen "282ReadPrivateMessage" and wires its chat scroller.
 *
 * Adds and plays the screen, sets bit 0 of "AgreementSlider"'s attributes (+0x14) and links it to "ChatUpArrow",
 * "ChatDownArrow" and "slideroutline2" (+0x7C..+0x84, mode 2 at +0x88). "textScrollChat", when found, gets bit 0 of
 * +0x74 and bit 7 of +0x14, cUITextScroll_setInputMap(1), its slot 9(1) and slot 11 colour (1, 0, 0, 0), a copy of mTextColour at
 * +0xA8 and the slider at +0xB8.
 */
extern "C" void cFEStateReadMessagePopup_onCreateScreen(cFEStateReadMessagePopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_282ReadPrivateMessage), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    char* scrollBar = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_AgreementSlider));
    *(int*)(scrollBar + 0x14) |= 1;
    void* upArrow = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatUpArrow));
    void* downArrow = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatDownArrow));
    void* sliderOutline = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_slideroutline2));
    *(void**)(scrollBar + 0x7C) = upArrow;
    *(void**)(scrollBar + 0x80) = downArrow;
    *(void**)(scrollBar + 0x84) = sliderOutline;
    *(int*)(scrollBar + 0x88) = 2;
    char* textScroll = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_textScrollChat));
    if (textScroll != 0) {
        cFEVObj_001A7970* obj = (cFEVObj_001A7970*)textScroll;
        *(int*)(textScroll + 0x74) |= 1;
        cUITextScroll_setInputMap(textScroll, 1);
        obj->v09(1);
        *(int*)(textScroll + 0x14) |= 0x80;
        obj->v11(sCol1A7970(1.0f, 0.0f, 0.0f, 0.0f));
        *(sBlk1A7970*)(textScroll + 0xA8) = *(sBlk1A7970*)&self->mTextColour;
        *(char**)(textScroll + 0xB8) = scrollBar;
    }
}
#endif

extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self);

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the private-message popup: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateReadMessagePopup_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7B40);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

extern "C" void func_001A7B40()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00262768(BuddyMan_mBuddyMan, 1, 10, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Widget-creation hook of the private-message popup: empty, the widgets are found in onCreateScreen instead.
 */
void cFEStateReadMessagePopup_onWidgetCreate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262690(void* self, int a1, void* a2);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern "C" void func_001A81F0(cFEStateReadMessagePopup* self);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A7ED8(cFEStateReadMessagePopup* self, const char* name);
extern "C" void func_001A9090(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern char D_004A1408[];

struct cFEObj_001A7BB8 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(void* a1, int a2);
};

/**
 * Handles the private-message popup's widget events: back, reply with the keyboard, and sending the typed reply.
 *
 * Event 6 tells the parent state (slot 17, onPopupEvent) the popup was backed out of (0x14). Event 5 opens the
 * keyboard (func_001A9090) titled with mszSender, empty, at most 63 characters, as a regular keyboard (meType 0) with
 * key 0x4B included and mbReferEventsToParent set. Event 13 (the keyboard finished) adds the typed text to the
 * conversation (func_001A7ED8), makes sure BuddyMan exists and is connected, sends the text to mszSender
 * (func_00262690) and refreshes the list (func_001A81F0). Event 7 is ignored; the rest go to the cFEStateNet handler
 * (cFEStateNet_onWidgetEvent).
 *
 * @param pWidget  the widget (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cFEStateReadMessagePopup_onWidgetEvent(cFEStateReadMessagePopup* self, int pWidget, unsigned int msg)
{
    switch (msg) {
    case 6:
        ((cFEObj_001A7BB8*)self->mpParentState)->v17(self, 20);
        break;
    case 5:
        func_001A9090((cFEStateNet*)self, 0, (int)self->mszSender, (int)D_004A1408, 0, 0, 0x3F);
        *(int*)((char*)self->mpKeyboard + 0x43C) = 0;
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
        func_001CE468(self->mpKeyboard, 1);
        *(int*)((char*)self->mpKeyboard + 0x438) = 1;
        break;
    case 7:
        break;
    case 13:
        func_001A7ED8(self, (char*)self->mpKeyboard + 0x74);
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80();
        }
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            func_002613C8(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800(LobbyMan_mLobbyMan);
        }
        func_00262690(BuddyMan_mBuddyMan, (int)self->mszSender, (char*)self->mpKeyboard + 0x74);
        func_001A81F0(self);
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void func_001A81F0(cFEStateReadMessagePopup* self);

/**
 * Handles menu events on the private-message popup: event 4 (the text scroller moved) stores the index and refreshes.
 *
 * The index goes to unk_6D4 and func_001A81F0 rebuilds the conversation.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (On Tour Event)
 * @param index  the new index (On Tour index)
 * @return 0 for event 4, else 0x101
 */
extern "C" int cFEStateReadMessagePopup_onWidgetMenu(cFEStateReadMessagePopup* self, int pMenu, int Event, int index)
{
    if (Event == 4) {
        self->unk_6D4 = index;
        func_001A81F0(self);
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7D40);
#ifdef SKIP_ASM
extern "C" void func_001A7F78(cFEStateReadMessagePopup* self);
extern "C" void func_001A81F0(cFEStateReadMessagePopup* self);
extern "C" void* func_001A8E40(void* self);
extern "C" void* func_001A7D40(cFEStateReadMessagePopup* self, int msg)
{
    if (msg == 265) {
        func_001A7F78(self);
        func_001A81F0(self);
        return 0;
    }
    return func_001A8E40(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateReadMessagePopup_onGainTransition);
#ifdef SKIP_ASM
class cFEVObj_001A7D90 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

extern "C" void func_001A7F78(cFEStateReadMessagePopup* self);
extern "C" void func_001A81F0(cFEStateReadMessagePopup* self);

/**
 * Prepares the private-message popup as it opens: refreshes BuddyMan, takes the sender's messages and lists them.
 *
 * Calls its slot 32 (func_001A7B40: BuddyMan get-or-create, reconnect, then func_00262768(1, 10)), moves BuddyMan's
 * messages from mszSender into mMessages (func_001A7F78) and refreshes the list (func_001A81F0).
 *
 * @return 1 (the transition is done)
 */
extern "C" int cFEStateReadMessagePopup_onGainTransition(cFEStateReadMessagePopup* self)
{
    ((cFEVObj_001A7D90*)self)->v32();
    func_001A7F78(self);
    func_001A81F0(self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7DD8);
#ifdef SKIP_ASM
extern "C" void* func_0041610C(void* dst, void* src, int n);

struct sVE1A7DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj1A7DD8 {
    char pad[0xC];
    sVE1A7DD8* vt;
};


extern "C" void func_001DB918(tFEPrivateMessageVector* vec, sObj1A7DD8** pos, sObj1A7DD8* const* x);

static inline sObj1A7DD8** Copy1A7DD8(sObj1A7DD8** first, sObj1A7DD8** last, sObj1A7DD8** result)
{
    func_0041610C(result, first, (last - first) * sizeof(sObj1A7DD8*));
    return result + (last - first);
}


extern "C" void func_001A7DD8(cFEStateReadMessagePopup* selfp, void* item)
{
    cFEStateReadMessagePopup* self = selfp;
    sObj1A7DD8* x = (sObj1A7DD8*)item;
    for (;;) {
        if ((unsigned int)(((sObj1A7DD8**)(*(&self->mMessages)).mpFinish) - ((sObj1A7DD8**)(*(&self->mMessages)).mpStart)) < 31) break;
        tFEPrivateMessageVector* messages = &(*(&self->mMessages));
        sObj1A7DD8** pos = ((sObj1A7DD8**)(*(&self->mMessages)).mpStart);
        sObj1A7DD8* oldest = *pos;
        if (oldest) {
            if (pos + 1 != ((sObj1A7DD8**)(*(&self->mMessages)).mpFinish)) {
                Copy1A7DD8(pos + 1, ((sObj1A7DD8**)messages->mpFinish), pos);
            }
            (*(sObj1A7DD8***)&(*(&self->mMessages)).mpFinish)--;
            oldest->vt[1].fn((char*)oldest + oldest->vt[1].delta, 3);
        }
    }
    tFEPrivateMessageVector* messageVec = &(*(&self->mMessages));
    if (((sObj1A7DD8**)(*(&self->mMessages)).mpFinish) != ((sObj1A7DD8**)(*(&self->mMessages)).mpEndOfStorage)) {
        sObj1A7DD8** slot = ((sObj1A7DD8**)messageVec->mpFinish);
        if (slot) *slot = x;
        (*(sObj1A7DD8***)&(*(&self->mMessages)).mpFinish)++;
    } else {
        func_001DB918(messageVec, ((sObj1A7DD8**)(*(&self->mMessages)).mpFinish), &x);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A7ED8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void func_001A7DD8(cFEStateReadMessagePopup* self, void* item);
extern char str_cPrivateMessage[];
extern void* D_0046D8C0[];
extern char D_004A1408[];

static inline void* Init_001A7ED8(char* p, const char* a, const char* b, const char* c)
{
    *(void***)(p + 0xC) = D_0046D8C0;
    cBXString_cBXString2(p, a);
    cBXString_cBXString2(p + 4, b);
    cBXString_cBXString2(p + 8, c);
    return p;
}

extern "C" void func_001A7ED8(cFEStateReadMessagePopup* self, const char* name)
{
    func_001A7DD8(self, Init_001A7ED8((char*)cMemMan_alloc(0x10, str_cPrivateMessage, 0x20000000, 0),
                                      ((const char*)LobbyMan_mLobbyMan->mPersona), name, D_004A1408));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7F78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void func_001A7DD8(cFEStateReadMessagePopup* self, void* item);
extern "C" void func_0025B800(void* self);
extern "C" void func_00260F80();
extern "C" void func_002613C8(void* self);
extern "C" int func_00261460(void* self);
extern "C" int func_00262938(void* self);
extern "C" void func_002635A8(void* self, int idx);
extern "C" void* func_00263630(void* self, int a1);
extern "C" int strcasecmp(const char* a, const char* b);
extern char str_cPrivateMessage[];
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern void* D_0046D8C0[];

static inline void* GetSession_001A7F78()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

static inline void* Init_001A7F78(char* p, char* src)
{
    *(void***)(p + 0xC) = D_0046D8C0;
    cBXString_cBXString1(p, src);
    cBXString_cBXString1(p + 4, src + 4);
    cBXString_cBXString1(p + 8, src + 8);
    return p;
}

extern "C" void func_001A7F78(cFEStateReadMessagePopup* self)
{
    for (int i = 0; i < func_00262938(GetSession_001A7F78()); i++) {
        char* item = (char*)func_00263630(GetSession_001A7F78(), i);
        if (item != 0 && strcasecmp(*(const char**)item, (const char*)self->mszSender) == 0) {
            func_001A7DD8(self, Init_001A7F78((char*)operator new(0x10, str_cPrivateMessage, 0x20000000, 0), item));
        }
    }
    for (int i = func_00262938(GetSession_001A7F78()) - 1; i >= 0; i--) {
        char* item = (char*)func_00263630(GetSession_001A7F78(), i);
        if (item != 0 && strcasecmp(*(const char**)item, (const char*)self->mszSender) == 0) {
            func_002635A8(GetSession_001A7F78(), i);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A81F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
struct sList1A83D8;
extern "C" void func_001A83D8(sList1A83D8* list, int keep, unsigned int sel, int a3, int pos, int a5, int a6, int trim);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...); the unit declares a 3-arg view
extern "C" void sprintf1_1A81F0(void* str, int fmt, ...) __asm__("sprintf1");
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern "C" void cUITextScroll_clearBuffer(void* list);
extern "C" void cUITextScroll_addAsciiString(cUIText* text, char* str);
extern "C" char* strcat(char*, const char*);
extern "C" char* strcpy(char* dst, const char* src);
extern char str_TextFromX[];
extern char str_kT_POPUP242PlayerID[];
extern char str_textScrollChat[];
extern char D_004A1408[];
extern char D_004A1A50[];

struct sVEntry_001A81F0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1A81F0(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001A81F0* vt = *(sVEntry_001A81F0**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

struct sEntry1A81F0 {
    char* name;
    char* ext;
    char* desc;
};


static inline unsigned int Count1A81F0(cFEStateReadMessagePopup* feStateReadMessagePopup) { return ((sEntry1A81F0**)feStateReadMessagePopup->mMessages.mpFinish) - ((sEntry1A81F0**)feStateReadMessagePopup->mMessages.mpStart); }

class cFEVObj_001A81F0 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001A81F0(cFEStateReadMessagePopup* self)
{
    char buf[0x100];
    if (self->mpScreen == 0) {
        return;
    }
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextFromX));
    if (text != 0 && Count1A81F0(self) != 0) {
        sEntry1A81F0* e = ((sEntry1A81F0**)(self)->mMessages.mpStart)[0];
        char* who = self->mszSender;
        sprintf1_1A81F0(buf, Loc1A81F0(str_kT_POPUP242PlayerID), who, e->desc, D_004A1408);
        cUIText_setUnicodeString(text, buf);
        ((cFEVObj_001A81F0*)text)->v09(1);
    }
    cUIText* list = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_textScrollChat));
    if (list == 0) {
        return;
    }
    unsigned short count = *(unsigned short*)((char*)list + 0xC8);
    unsigned short top = *(unsigned short*)((char*)list + 0xC4);
    unsigned short sel = *(unsigned short*)((char*)list + 0xC6);
    cUITextScroll_clearBuffer(list);
    unsigned int i;
    for (i = 0; i < Count1A81F0(self); i++) {
        sEntry1A81F0* e = *(sEntry1A81F0**)((char*)((sEntry1A81F0**)(self)->mMessages.mpStart) + (i << 2));
        strcpy(buf, e->name);
        strcat(buf, D_004A1A50);
        strcat(buf, e->ext);
        cUITextScroll_addAsciiString(list, buf);
    }
    func_001A83D8((sList1A83D8*)list, 40, 8, count, top, sel, self->mbResetScroll, 0);
    self->mbResetScroll = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A83D8);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_deleteLineByIndex(void* list, int a1);

struct sList1A83D8 {
    char pad[0xC4];
    short top;
    short sel;
    unsigned short count;
};

extern "C" void func_001A83D8(sList1A83D8* list, int keep, unsigned int sel, int total, int pos, int selIndex, int scrollToEnd, int trim)
{
    if (list == 0) return;
    int toEnd = 0;
    if (total == pos + 1 || scrollToEnd != 0) toEnd = 1;
    int n = list->count - keep;
    int atStart = pos == 0;
    int removed = 0;
    if (n > 0 && trim != 0) {
        for (removed = 0; removed < n; removed++) {
            cUITextScroll_deleteLineByIndex(list, 0);
        }
    }
    if (toEnd) {
        unsigned short lineCount = list->count;
        if (sel >= lineCount) {
            list->sel = 0;
        } else {
            list->sel = lineCount - sel;
        }
        list->top = list->count - 1;
    } else if (atStart) {
        list->sel = 0;
        list->top = 0;
    } else {
        int newTop = pos - removed;
        if (newTop <= 0) {
            list->top = 0;
            list->sel = 0;
        } else {
            int newSel = selIndex - removed;
            if (newSel <= 0) {
                list->sel = 0;
            } else {
                list->sel = newSel;
            }
            list->top = newTop;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8500);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern void* D_0046B808[];
extern char D_004A1390[];
extern char str_StlBlock[];

struct sListNode_8500 {
    sListNode_8500* next;   // 0x0
    sListNode_8500* prev;   // 0x4
    int data;               // 0x8
};

struct sList_8500 {
    int count;              // 0x0
    sListNode_8500* head;   // 0x4
};

// PORT: the unit declares func_001A8500 with one arg; the body takes (self, engine, owner).
extern "C" void* func_001A8500_impl(void* self, void* engine, void* owner) __asm__("func_001A8500");

extern "C" void* func_001A8500_impl(void* self, void* engine, void* owner)
{
    // An unused 16-byte local is needed to reproduce the 0x40-byte frame.
    int unused[4];
    char* s = (char*)self;
    func_0039E318(self, engine, owner);
    sList_8500* l = (sList_8500*)(s + 0x6C8);
    *(int*)(s + 0x48) = 0;
    *(int*)(s + 0x4C) = 0;
    *(int*)(s + 0x50) = 0;
    *(int*)(s + 0x54) = 0;
    *(int*)(s + 0x58) = 0;
    *(int*)(s + 0x69C) = 0;
    *(int*)(s + 0x6A0) = 0;
    *(int*)(s + 0x6A4) = 0;
    *(int*)(s + 0x6AC) = 0;
    *(int*)(s + 0x6B0) = 0;
    *(int*)(s + 0x6B4) = 0;
    *(int*)(s + 0x6B8) = 0;
    *(int*)(s + 0x6A8) = 1;
    *(int*)(s + 0x6BC) = 2;
    *(void***)(s + 0x8) = D_0046B808;
    cBXString_cBXString2(s + 0x6C0, D_004A1390);
    *(int*)(s + 0x6C4) = 0;
    *(sListNode_8500**)(s + 0x6CC) = (sListNode_8500*)cMemMan_alloc(0xC, str_StlBlock, 0x20000000, 0);
    l->head->next = *(sListNode_8500**)(s + 0x6CC);
    l->head->prev = *(sListNode_8500**)(s + 0x6CC);
    *(char*)(s + 0x44) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A85D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_001DB718(void* self);
extern "C" void func_0039E390(void* self, int flags);
void operator_delete(int*);

struct cObj_1A85D0 {
    int pad;
    virtual ~cObj_1A85D0();
};

struct sNode1A85D0 {
    sNode1A85D0* next;
    sNode1A85D0* prev;
    cObj_1A85D0* data;
};

struct sIter1A85D0 {
    sNode1A85D0* node;
    sIter1A85D0(sNode1A85D0* x) : node(x) {}
    sIter1A85D0(const sIter1A85D0& x) : node(x.node) {}
    cObj_1A85D0*& operator*() const { return node->data; }
};

struct sList1A85D0 {
    int alloc;
    sNode1A85D0* node;
    sIter1A85D0 begin() { return node->next; }
    cObj_1A85D0*& front() { return *begin(); }
    sIter1A85D0 erase(sIter1A85D0 pos)
    {
        sNode1A85D0* next = pos.node->next;
        sNode1A85D0* prev = pos.node->prev;
        prev->next = next;
        next->prev = prev;
        operator_delete((int*)pos.node);
        return sIter1A85D0(next);
    }
    void pop_front() { erase(begin()); }
    sNode1A85D0* first() { return node->next; }
};

extern "C" void f_1A85D0(char* self, int flags) __asm__("func_001A85D0");
extern "C" void f_1A85D0(char* self, int flags)
{
    *(void**)(self + 8) = (void*)D_0046B808;
    while (((sList1A85D0*)(self + 0x6C8))->first() != *(sNode1A85D0**)(self + 0x6CC)) {
        cObj_1A85D0* d = ((sList1A85D0*)(self + 0x6C8))->front();
        ((sList1A85D0*)(self + 0x6C8))->pop_front();
        if (d) {
            delete d;
        }
    }
    if (*(void**)(self + 0x6B8)) {
        cMemMan_free(*(void**)(self + 0x6B8));
        *(void**)(self + 0x6B8) = 0;
    }
    sList1A85D0* l = (sList1A85D0*)(self + 0x6C8);
    func_001DB718(l);
    operator_delete((int*)l->node);
    cBXString__cBXString(self + 0x6C0, 2);
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A86E8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern int D_004A1A70;

extern "C" void func_001A86E8(int on)
{
    D_004A1A70 = on;
    if (on == 0 && BuddyMan_mBuddyMan != 0 && func_00261460(BuddyMan_mBuddyMan) != 0) {
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80();
        }
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            func_002613C8(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800(LobbyMan_mLobbyMan);
        }
        func_00261408(BuddyMan_mBuddyMan);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8768__FPv);
#ifdef SKIP_ASM
void func_001A8768(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNet_onUpdate);
#ifdef SKIP_ASM
class cFEAsync1A8770 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
};

extern "C" void cUIState_onUpdate(void*);
extern "C" void* func_001A8B88(void* self, int size, int a2);

/**
 * Per-frame update shared by the online front-end states: polls the buddy session, blinks the hint icons, updates the
 * screens and delivers pending net events.
 *
 * Calls the state's own virtuals at vtable entries 27 (func_001AAB68: polls the session for new messages and challenges
 * at most once per 60 frames) and 28 (func_001AA9B0: blinks "IconMessageHint" / "TextMessageCount" /
 * "IconChallengeHint"), then cUIState_onUpdate, and returns handleEvent func_001A8B88(self, 276, 0).
 *
 * @return func_001A8B88's result
 */
extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self)
{
    ((cFEAsync1A8770*)self)->v27();
    ((cFEAsync1A8770*)self)->v28();
    cUIState_onUpdate(self);
    return func_001A8B88(self, 276, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNet_onGainFocus);
#ifdef SKIP_ASM
struct sVE1A87D0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void cUIState_onGainFocus(void* self, int a1);

/**
 * Runs when an online front-end state gets the focus: refreshes the buddy presence and enables event processing.
 *
 * Calls the state's own virtual at vtable entry 32 (cFEStateNet's func_001A8830 unless overridden, which sets the
 * player's buddy presence through func_00262768), then cUIState_onGainFocus with @p pPrevState, and sets unk_048 to 1
 * (func_001DE8A8 reports the state as processing events only while it is set).
 *
 * @param pPrevState  the state that had the focus (On Tour: cUIState* pPrevState)
 */
extern "C" void cFEStateNet_onGainFocus(cFEStateNet* self, int pPrevState)
{
    sVE1A87D0* vt = (sVE1A87D0*)self->mVtbl;
    vt[32].fn((char*)self + vt[32].delta);
    cUIState_onGainFocus(self, pPrevState);
    self->unk_048 = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8828__FPv);
#ifdef SKIP_ASM
int func_001A8828(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8830);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

class cFEVObj_001A8830 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual int v29();
};

static inline void* GetSession_001A8830()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001A8830(cFEStateNet* self)
{
    if (((cFEVObj_001A8830*)self)->v29() != 0) {
        func_00262768(GetSession_001A8830(), 1, 0, 0, 0, 0);
    } else {
        func_00262768(GetSession_001A8830(), 0, 1, 0, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNet_onWidgetEvent);
#ifdef SKIP_ASM
class cFEVObj_001A8918 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25(void* popup);
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001D9D48(void* mem, void* a1, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetHelpPopup[];

/**
 * Default widget-event handling for the online front-end states: event 7 opens the help popup.
 *
 * When @p pWidget is set and @p msg is 7, allocates a cFEStateNetHelpPopup (cMemMan_alloc(320, "cFEStateNetHelpPopup"),
 * constructed by func_001D9D48 with mpEngine and this state as parent), sets its word +0x50 to 1 (On Tour has
 * mbReferEventsToParent there), lets the state fill it through its own virtual at vtable entry 25 (the messenger's is
 * func_001A4040) and pushes it on the engine's state stack (cUIStateStack_pushPopup on mpEngine + 0x18). Other events
 * are ignored.
 *
 * @param pWidget  the widget (On Tour: cUIThing* pWidget)
 * @param msg  the widget event
 */
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int pWidget, int msg)
{
    if (pWidget != 0 && msg == 7) {
        void* p = func_001D9D48(cMemMan_alloc(320, str_cFEStateNetHelpPopup, 0x100, 0), self->mpEngine, self);
        *(int*)((char*)p + 0x50) = 1;
        ((cFEVObj_001A8918*)self)->v25(p);
        cUIStateStack_pushPopup((char*)self->mpEngine + 0x18, p);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A89B0);
#ifdef SKIP_ASM
class cFEMsg_001A89B0 {
public:
    int field_0x0;
    int field_0x4;
    virtual ~cFEMsg_001A89B0();
};

extern "C" void func_001A9930(cFEStateNet* self);

extern "C" void* func_001A89B0(cFEStateNet* self, cFEMsg_001A89B0* msg)
{
    int kind = *(int*)((char*)msg + 0xC);
    if (self->unk_6A8 != 0 && kind != 49 && kind != 51 && kind != 77 && kind != 78
        && LobbyMan_mLobbyMan->mState == 0) {
        delete msg;
        func_001A9930(self);
        return 0;
    }
    return msg;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8A40);
#ifdef SKIP_ASM
class cFEVObj_001A8A40 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void* v26(void* obj);
};

int GetHashValue32(char* str);
extern char str_TransitionOut[];
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIStateStack_pushCheck(void* list, void* item);
// PORT: the unit declares func_001A8A40 as returning void; the body returns 0/1
extern "C" int func_001A8A40_r(void* self, void* obj) __asm__("func_001A8A40");

extern "C" int func_001A8A40_r(void* self, void* obj)
{
    void* child = *(void**)((char*)self + 0x20);
    if (child != 0) {
        return func_001A8A40_r(child, obj);
    }
    void* item = ((cFEVObj_001A8A40*)self)->v26(obj);
    if (item != 0) {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(str_TransitionOut));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
        cUIStateStack_pushCheck(*(char**)((char*)self + 0x10) + 0x18, item);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8AE8);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is probably the game's overloaded operator new(size, tag, flags, align). Calling it as
// operator new gives gcc's malloc-like (REG_NOALIAS) aliasing that the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_StlBlock[];

class cFEVObj_001A8AE8 {
public:
    int field_0x0;
    virtual void* v01();
    virtual void* v02();
};

struct sListNode1A8AE8 {
    void* mNext;
    void* mPrev;
    void* mData;
};

struct sListIter1A8AE8 {
    sListNode1A8AE8* mNode;
    sListIter1A8AE8(sListNode1A8AE8* x) : mNode(x) {}
    sListIter1A8AE8(const sListIter1A8AE8& x) : mNode(x.mNode) {}
};

static inline sListIter1A8AE8 ListInsert1A8AE8(sListIter1A8AE8 pos, void* const& x)
{
    sListNode1A8AE8* tmp = (sListNode1A8AE8*)operator new(0xC, str_StlBlock, 0x20000000, 0);
    void** data = &tmp->mData;
    if (data != 0) {
        *data = x;
    }
    tmp->mNext = pos.mNode;
    tmp->mPrev = pos.mNode->mPrev;
    ((sListNode1A8AE8*)(pos.mNode->mPrev))->mNext = tmp;
    pos.mNode->mPrev = tmp;
    return tmp;
}

extern "C" void func_001A8AE8(cFEStateNet* self, cFEVObj_001A8AE8* obj)
{
    if (obj != 0) {
        void* item = obj->v02();
        if (item != 0) {
            ListInsert1A8AE8((sListNode1A8AE8*)self->mMsgList.mpNode, item);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8B88);
#ifdef SKIP_ASM
void func_00261058(void* self);
extern "C" void func_00261408(void* self);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void* p, int id);
void operator_delete(int* ptr);

struct sNode1A8B88 {
    sNode1A8B88* next;
    sNode1A8B88* prev;
    void* data;
};

struct sIter1A8B88 {
    sNode1A8B88* node;
    sIter1A8B88(sNode1A8B88* x) : node(x) {}
    sIter1A8B88(const sIter1A8B88& x) : node(x.node) {}
    void*& operator*() const { return node->data; }
};

struct sList1A8B88 {
    int alloc;
    sNode1A8B88* node;
    sIter1A8B88 begin() { return node->next; }
    void*& front() { return *begin(); }
    sIter1A8B88 erase(sIter1A8B88 pos)
    {
        sNode1A8B88* next = pos.node->next;
        sNode1A8B88* prev = pos.node->prev;
        prev->next = next;
        next->prev = prev;
        operator_delete((int*)pos.node);
        return sIter1A8B88(next);
    }
    void pop_front() { erase(begin()); }
};


class cFEState1A8B88 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual int v30();
    virtual int v31(int msg, void* data);
};

class cMsg1A8B88 {
public:
    int type;
    virtual ~cMsg1A8B88();
};

// PORT: the unit declares func_001A8B88 as (void*, int, int) returning void*.
extern "C" int func_001A8B88_impl(cFEState1A8B88* st, int msg, int* data) __asm__("func_001A8B88");

extern "C" int func_001A8B88_impl(cFEState1A8B88* st, int msg, int* data)
{
    if ((unsigned int)(msg - 200) < 79) {
        if ((unsigned int)(msg - 277) < 2 || *(int*)((char*)st + 0x6B4) != 0) {
            if (msg == 277 && BuddyMan_mBuddyMan != 0) {
                func_00261058(BuddyMan_mBuddyMan);
                func_00261408(BuddyMan_mBuddyMan);
            }
            if (st->v30() != 0) {
                if ((unsigned int)(msg - 277) < 2) {
                    st->v31(*data, data);
                } else {
                    st->v31(277, 0);
                }
                *(int*)((char*)st + 0x6B4) = 0;
            } else {
                *(int*)((char*)st + 0x6B4) = 1;
            }
            return 1;
        }
        if (msg != 276) {
            func_001A8AE8((cFEStateNet*)st, (cFEVObj_001A8AE8*)data);
        }
        sList1A8B88* first = (sList1A8B88*)((char*)st + 0x6C8);
        if (first->node->next != *(sNode1A8B88**)((char*)st + 0x6CC)) {
            if (st->v30() == 0) {
                return 0;
            }
            if (*(int*)((char*)st + 0x1C) & 6) {
                char* p = *(char**)((char*)st + 0x6A0);
                if (p != 0 && p + 0xBC != 0 && *(int*)(p + 0x170) != 0) {
                    return 0;
                }
            }
            sList1A8B88* list = (sList1A8B88*)((char*)st + 0x6C8);
            if (list->node->next == *(sNode1A8B88**)((char*)st + 0x6CC)) {
                return 0;
            }
            do {
                cMsg1A8B88* item = (cMsg1A8B88*)list->front();
                list->pop_front();
                st->v31(item->type, item);
                switch (item->type) {
                case 236:
                    cSSXAudio_MenuSound(func_0028B180(), 7);
                case 237:
                case 239:
                case 241:
                case 274:
                case 275:
                    st->v27();
                    break;
                case 265:
                    cSSXAudio_MenuSound(func_0028B180(), 8);
                    st->v27();
                    break;
                }
                delete item;
            } while (list->node->next != *(sNode1A8B88**)((char*)st + 0x6CC));
        }
        return 0;
    }
    st->v31(msg, data);
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8E40);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_001A9930(cFEStateNet* self);
extern "C" int func_0025DA90(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateLobbyMenu_cFEStateLobbyMenu_v(void* mem, int a1) __asm__("cFEStateLobbyMenu_cFEStateLobbyMenu");
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cFEStateNet* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A8F98(cFEStateNet* self);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9CC0(cFEStateNet* self, void** popup, void* owner, void* data);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern char str_cFEStateLobbyMenu[];
extern const char str_cFEStateOnlineMainMenu[];

struct sVE1A8E40 {
    short delta;
    short index;
    int (*fn)(void*, int, void*);
};

// PORT: the unit declares func_001A8E40 with one arg; defined under an asm label with its real type.
extern "C" int func_001A8E40_impl(void* self, int msg, void* data) __asm__("func_001A8E40");

// Message handler of the online screens (cFEStateNet): 277/278 -> func_001A9930; 220 opens cFEStateLobbyMenu when the
// lobby isn't busy (func_0025DA90), 239 cFEStateOnlineMainMenu; 258 with the tag 'time' (0x74696D65) ->
// func_001A8F98, else a popup; 221-225 go to the child state at +0x20 (its virtual slot 24).
extern "C" int func_001A8E40_impl(void* self, int msg, void* data)
{
    char* obj = (char*)self;
    switch (msg) {
    case 277:
    case 278:
        func_001A9930((cFEStateNet*)self);
        break;
    case 220:
        if (func_0025DA90(LobbyMan_mLobbyMan) == 0) {
            func_001A8A40(self, cFEStateLobbyMenu_cFEStateLobbyMenu_v(cMemMan_alloc(2948, str_cFEStateLobbyMenu, 0, 0), *(int*)(obj + 0x10)));
        }
        break;
    case 239:
        func_001A8A40(self, cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), *(int*)(obj + 0x10)));
        break;
    case 258:
        if (*(int*)((char*)data + 0x8) == 0x74696D65) {
            func_001A8F98((cFEStateNet*)self);
        } else {
            void* popup = func_001A9C50((cFEStateNet*)self);
            func_001A9CC0((cFEStateNet*)self, &popup, self, data);
            func_001A9BC0((cFEStateNet*)self, popup);
        }
        break;
    case 221:
    case 222:
    case 224:
    case 225: {
        char* child = *(char**)(obj + 0x20);
        if (child != 0) {
            sVE1A8E40* vt = *(sVE1A8E40**)(child + 0x8);
            vt[24].fn(child + vt[24].delta, msg, data);
        }
        break;
    }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A8F98);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern char str_kT_BTNOK[];
extern char str_kT_POPUPCMNNetInterrupted[];

static inline void* GetSession_001A8F98()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001A8F98(cFEStateNet* self)
{
    void* popup = func_001A9C50(self);
    *(int*)((char*)popup + 0xBC) = 1;
    func_001C57C0((char*)popup + 0xBC, 0, GetHashValue32(str_kT_BTNOK));
    int textHash = GetHashValue32(str_kT_POPUPCMNNetInterrupted);
    *(int*)((char*)popup + 0xC4) = textHash;
    *(int*)((char*)popup + 0xC0) = 3;
    *(int*)((char*)popup + 0x150) = 2110;
    *(int*)((char*)popup + 0x14C) = 6;
    func_001A9BC0(self, popup);
    func_00261408(GetSession_001A8F98());
    func_0025B800(LobbyMan_mLobbyMan);
    func_002636F0(func_002591B8(), 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9090);
#ifdef SKIP_ASM
extern "C" void func_001A91B0(cFEStateNet* self, int a1, int a2, int a3, int a4);
extern "C" void func_001CCF00(void* p, int a);
extern "C" void cKeyboardPopup_setInputText(void* p, int a);

extern "C" void func_001A9090(cFEStateNet* self, int textObj, int a2, int inputText, int password, int popupId, int maxChars)
{
    func_001A91B0(self, textObj, password, popupId, maxChars);
    func_001CCF00(self->mpKeyboard, a2);
    cKeyboardPopup_setInputText(self->mpKeyboard, inputText);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A90F0);
#ifdef SKIP_ASM
extern "C" void cKeyboardPopup_setStatic(void* popup, int a);
extern "C" void func_001A91B0(cFEStateNet* self, int a1, int a2, int a3, int a4);
extern "C" void cKeyboardPopup_setInputText(void* p, int a);

extern "C" void func_001A90F0(cFEStateNet* self, int textObj, int staticText, int inputText, int password, int popupId, int maxChars)
{
    func_001A91B0(self, textObj, password, popupId, maxChars);
    cKeyboardPopup_setStatic(self->mpKeyboard, staticText);
    cKeyboardPopup_setInputText(self->mpKeyboard, inputText);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9150);
#ifdef SKIP_ASM
extern "C" void func_001A91B0(cFEStateNet* self, int a1, int a2, int a3, int a4);
extern "C" void cKeyboardPopup_setStatic1(void* p, int a);
extern "C" void cKeyboardPopup_setInputText(void* p, int a);

extern "C" void func_001A9150(cFEStateNet* self, int textObj, int staticText, int inputText, int password, int popupId, int maxChars)
{
    func_001A91B0(self, textObj, password, popupId, maxChars);
    cKeyboardPopup_setStatic1(self->mpKeyboard, staticText);
    cKeyboardPopup_setInputText(self->mpKeyboard, inputText);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A91B0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* mem, void* engine, void* owner, int a3, unsigned char a4);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cKeyboardPopup_setMaxCharacters(void* self, int v);
extern char str_cKeyboardPopup[];

extern "C" void func_001A91B0(cFEStateNet* self, int textObj, int password, int popupId, int maxChars)
{
    void* keyboard = cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(1092, str_cKeyboardPopup, 0x100, 0), self->mpEngine, self, password, 0xF);
    self->mpKeyboard = (cKeyboardPopup*)keyboard;
    cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, keyboard);
    *(int*)((char*)self->mpKeyboard + 0x18) = popupId;
    cKeyboardPopup_setMaxCharacters(self->mpKeyboard, maxChars);
    self->mpKeyboardText = (cUIText*)textObj;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9268);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern const char str_cFEStateOnlineCharacterSelect[];

extern "C" void func_001A9268(cFEStateNet* self, int popup, int event)
{
    if (event == 0xF) {
        func_001A93D0_3(self, popup, 0xF);
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(1764, str_cFEStateOnlineCharacterSelect, 0x100, 0), (int)self->mpEngine));
    } else {
        func_001A93D0_3(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A92D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");

struct sVE1A92D8 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE1A92D8b {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001A92D8(cFEStateNet* self, int a1, unsigned int kind)
{
    switch (kind) {
    case 21:
        self->unk_6BC = 2;
        func_001A93D0_3(self, a1, 21);
        break;
    case 15:
        self->unk_6BC = 0;
        func_001A93D0_3(self, a1, 0xF);
        break;
    case 22: {
        sVE1A92D8* vt = (sVE1A92D8*)self->mVtbl;
        vt[32].fn((char*)self + vt[32].delta);
        if (self->unk_6BC == 0) {
            char* obj = (char*)self->mpParentState;
            sVE1A92D8b* vt2 = *(sVE1A92D8b**)(obj + 0x8);
            vt2[17].fn(obj + vt2[17].delta, self, 0xF);
        }
        func_001A93D0_3(self, a1, 22);
        break;
    }
    default:
        func_001A93D0_3(self, a1, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A93D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0039F190(void* self, int a1);

// PORT: the unit declares this with a 4th arg; defined under an asm label with its real type
extern "C" void func_001A93D0_impl(void* self, int id, int event) __asm__("func_001A93D0");
extern "C" void func_001A93D0_impl(void* self, int id, int event)
{
    switch (event) {
    case 15: case 16: case 17: case 18: case 20:
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    case 22:
        if (id == *(int*)((char*)self + 0x6A0)) {
            *(int*)((char*)self + 0x6A0) = 0;
        }
        break;
    case 19:
    case 21:
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9438);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cFEVObj_001A9438 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateLogin_cFEStateLogin(cFEStateLogin* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern const char str_cFEStateLogin_MemoryCard[];

extern "C" void func_001A9438(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 22) {
        ((cFEVObj_001A9438*)self)->v32();
        func_001A8A40(self, cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(1748, str_cFEStateLogin_MemoryCard, 0, 0), (int)self->mpEngine));
        func_001A93D0_3(self, a1, 22);
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A94D8__FPv);
#ifdef SKIP_ASM
void func_001A94D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A94E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cFEVObj_001A94E0 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");

extern "C" void func_001A94E0(cFEStateNet* self, int a1, int a2)
{
    switch (a2) {
    case 21:
        func_001A93D0_3(self, a1, 21);
        break;
    case 22:
        ((cFEVObj_001A94E0*)self)->v32();
        self->mpKeyboard = 0;
        func_001A93D0_3(self, a1, 22);
        break;
    default:
        func_001A93D0_3(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9578);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
struct sFEObj_001B1F88; // defined with cFEStateChatRoom_cFEStateChatRoom (its ctor)
extern "C" sFEObj_001B1F88* cFEStateChatRoom_cFEStateChatRoom(sFEObj_001B1F88* mem, int a1);
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cFEStateNet* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_0025DA90(void* self);
extern int D_004A4E88;
extern const char str_cFEStateChatRoom[];
extern const char str_cFEStateOnlineMainMenu[];

extern "C" void func_001A9578(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 20) {
        func_001A93D0_3(self, a1, a2);
        void* obj;
        if (func_0025DA90(LobbyMan_mLobbyMan) != 0 && D_004A4E88 == 0) {
            obj = cFEStateChatRoom_cFEStateChatRoom((sFEObj_001B1F88*)cMemMan_alloc(1852, str_cFEStateChatRoom, 0, 0), (int)self->mpEngine);
        } else {
            obj = cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), (int)self->mpEngine);
        }
        func_001A8A40(self, obj);
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9638);
#ifdef SKIP_ASM
extern "C" void func_001A93D0(void* self, int a1, int a2, int a3);

extern "C" void func_001A9638(cFEStateNet* self, int popup, int event, int a3)
{
    switch (event) {
    case 0xF:
        break;
    case 20:
        break;
    default:
        func_001A93D0(self, popup, event, a3);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9668);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateLogin_cFEStateLogin(cFEStateLogin* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_0039F698(void* p);
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern const char str_cFEStateLogin[];

extern "C" void func_001A9668(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 20) {
        func_001A93D0_3(self, a1, a2);
        func_0039F698((char*)self->mpEngine + 0x18);
        self->unk_058 = 1;
        void* st = cUIStateStack_getCurrentState((char*)self->mpEngine + 0x18);
        *(int*)((char*)st + 0x1C) |= 0x80;
        func_001A8A40(self, cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(1748, str_cFEStateLogin, 0, 0), (int)self->mpEngine));
        self->unk_6AC = 0;
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9710);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_001A93D0 really takes (self, a1, a2); the unit declares it with a 4th arg
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateNetConfig_cFEStateNetConfig(cFEStateNetConfig* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_0039F698(void* p);
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern const char str_cFEStateNetConfig[];

extern "C" void func_001A9710(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 20) {
        func_001A93D0_3(self, a1, a2);
        func_0039F698((char*)self->mpEngine + 0x18);
        self->unk_058 = 1;
        void* st = cUIStateStack_getCurrentState((char*)self->mpEngine + 0x18);
        *(int*)((char*)st + 0x1C) |= 0x80;
        func_001A8A40(self, cFEStateNetConfig_cFEStateNetConfig((cFEStateNetConfig*)cMemMan_alloc(2332, str_cFEStateNetConfig, 0, 0), (int)self->mpEngine));
        self->unk_6AC = 0;
    } else {
        func_001A93D0_3(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A97B8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_001A94E0(cFEStateNet* self, int a1, int a2);
extern "C" void func_001A9438(cFEStateNet* self, int a1, int a2);
extern "C" void func_001A9268(cFEStateNet* self, int a1, int a2);
extern "C" void func_001A92D8(cFEStateNet* self, int a1, unsigned int kind);
extern "C" void func_001A9578(cFEStateNet* self, int a1, int a2);
// PORT: func_001A9638 takes a 4th arg; this caller leaves $a3 untouched.
void func_001A9638_3(void* self, int a1, int a2) __asm__("func_001A9638");
extern "C" void func_001A9668(cFEStateNet* self, int a1, int a2);
extern "C" void func_001A9710(cFEStateNet* self, int a1, int a2);
void func_001A93D0_3(void* self, int a1, int a2) __asm__("func_001A93D0");

// PORT: the unit declares this as returning int with a pointer a1; the body returns nothing.
extern "C" void func_001A97B8_impl(void* self, int popup, int a2) __asm__("func_001A97B8");

// Routes a closed popup to the handler for its message id.
extern "C" void func_001A97B8_impl(void* self, int popup, int event)
{
    if (popup == *(int*)((char*)self + 0x69C)) {
        func_001A94E0((cFEStateNet*)self, popup, event);
        return;
    }
    switch (*(int*)(popup + 0x18)) {
    case 2098:
        func_001A9438((cFEStateNet*)self, popup, event);
        break;
    case 2099:
        func_001A9268((cFEStateNet*)self, popup, event);
        break;
    case 2100:
        func_001A92D8((cFEStateNet*)self, popup, event);
        break;
    case 2103:
        func_001A9578((cFEStateNet*)self, popup, event);
        break;
    case 2102:
        func_001A9638_3(self, popup, event);
        break;
    case 2109:
        func_001A9668((cFEStateNet*)self, popup, event);
        break;
    case 2110:
        func_001A9710((cFEStateNet*)self, popup, event);
        break;
    default:
        func_001A93D0_3(self, popup, event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9890);
#ifdef SKIP_ASM
extern "C" void* cBXString_cBXString4(void* self, const char* str);

extern "C" void func_001A9890(cFEStateNet* self, const char* str)
{
    cBXString_cBXString4((char*)self + 0x6C0, str);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A98B0__FPv);
#ifdef SKIP_ASM
int func_001A98B0(void* self)
{
    return *(int*)((char*)self + 0x6C0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A98B8);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);


extern "C" void func_001A98B8(cFEStateNet* self, const char* str)
{
    cFEStateNet* feStateNet = self;
    int len = strlen(str);
    int i;
    for (i = 0; i < len && i < 0x3F; i++) {
        feStateNet->mszMaskedText[i] = '*';
    }
    if (len == 0) {
        feStateNet->mszMaskedText[0] = ' ';
        len = 1;
    }
    feStateNet->mszMaskedText[len] = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9930);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" int* func_002591B8();
extern "C" int func_0039F698(void* p);
extern char str_kT_BTNOK[];
extern char str_kT_POPUPCMNNetInterrupted[];
extern char str_kT_POPUPCMNDisconnected[];

extern "C" void func_001A9930(cFEStateNet* self)
{
    if (self->unk_6AC == 0) {
        int one = 1;
        self->unk_6AC = one;
        func_0039F698((char*)self->mpEngine + 0x18);
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = one;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)p + 0x170) = one;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x14C) = 6;
        if (*func_002591B8() == 0) {
            *(int*)((char*)p + 0x150) = 2110;
            *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUPCMNNetInterrupted);
            *(int*)((char*)p + 0xC0) = 3;
        } else {
            *(int*)((char*)p + 0x150) = 2109;
            *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUPCMNDisconnected);
            *(int*)((char*)p + 0xC0) = 3;
        }
        func_001A9BC0(self, p);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9A18);
#ifdef SKIP_ASM
class cUIListBox_001A9A18 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

extern "C" void func_001A9A18(cFEStateNet* self, cUIListBox_001A9A18* box, int value)
{
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNYes), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNNo), 0);
    cUIListBox_setCurrentEntryByIndex(box, value == 0);
    box->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9AB8);
#ifdef SKIP_ASM
class cUIListBox_001A9AB8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_kT_CMNOn[];
extern char str_kT_CMNOff[];

extern "C" void func_001A9AB8(cFEStateNet* self, cUIListBox_001A9AB8* box, int value)
{
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNOn), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNOff), 0);
    cUIListBox_setCurrentEntryByIndex(box, value == 0);
    box->v09(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9B58);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateNetMessenger_cFEStateNetMessenger(cFEStateNetMessenger* mem, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern const char str_cFEStateNetMessenger[];

extern "C" void func_001A9B58(cFEStateNet* self)
{
    void* messenger = cFEStateNetMessenger_cFEStateNetMessenger((cFEStateNetMessenger*)cMemMan_alloc(1788, str_cFEStateNetMessenger, 0x100, 0), self->mpEngine, self);
    *(int*)((char*)messenger + 0x18) = 2099;
    cUIStateStack_pushPopup((char*)*(void**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, messenger);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9BC0);
#ifdef SKIP_ASM
extern "C" void* func_0039FAE8(void* list);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cScreenPopup_redrawPopup(void* self);

extern "C" void func_001A9BC0(cFEStateNet* self, void* popup)
{
    void* top = func_0039FAE8(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18);
    if (*(int*)((char*)popup + 0x2B8) == 0) {
        if (popup != top) {
            cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, popup);
        }
    } else {
        *(int*)((char*)popup + 0x1C) = (*(int*)((char*)popup + 0x1C) & ~0x3F00) | 0x500;
        cScreenPopup_redrawPopup(self->mpPopup);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9C50);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cScreenPopup_cScreenPopup(void* mem, void* engine, void* owner);
extern "C" void cPopupInfo_clear(cPopupInfo* self);
extern const char str_createPopup[];

extern "C" void* func_001A9C50(cFEStateNet* self)
{
    void* popup;
    void* cur = self->mpPopup;
    if (cur == 0) {
        popup = cScreenPopup_cScreenPopup(cMemMan_alloc(864, str_createPopup, 0x100, 0), self->mpEngine, self);
        self->mpPopup = (cScreenPopup*)popup;
        *(int*)((char*)popup + 0x178) = 0;
    } else {
        cPopupInfo_clear((cPopupInfo*)((char*)cur + 0xBC));
        *(int*)((char*)self->mpPopup + 0x178) = 0;
    }
    return self->mpPopup;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9CC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cBXString;
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" int USTR_length(void* s);
extern "C" int strlen(const char* s);
void cMemMan_free(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
// PORT: sprintf2 is a printf-style formatter; the unit declares it with four args
extern "C" void sprintf2_1A9CC0(void* dst, const char* fmt, void* a, const char* b, const char* c) __asm__("sprintf2");
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
int cBXString_FindLastOf(cBXString* self, char ch, int len);
extern "C" void cBXString_setAt(cBXString* self, int idx, char ch);
extern "C" void func_00255FA0(void* self);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_00261408(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern char str_kT_BTNOK[];
extern char str_kT_POPUPCMNNetInterrupted[];
extern char str_kT_POPUPTITLEInvalidAccount[];
extern char str_kT_POPUPCMNUserInvalid[];
extern char str_kT_POPUPCMNPasswordInvalid[];
extern char str_kT_POPUPCMNDatabaseError[];
extern char str_kT_POPUPCMNMissingParameter[];
extern char str_kT_POPUPCMNInvalidParameters[];
extern char str_kT_POPUPCMNDuplicate[];
extern char str_kT_POPUPCMNMaxPersona[];
extern char str_kT_POPUPCMNDefaultPersona[];
extern char str_kT_POPUPCMNMasterAuth[];
extern char str_kT_POPUPCMNPersonaAuth[];
extern char str_kT_POPUPCMNUnknownRoom[];
extern char str_kT_POPUPCMNUnknownRankingList[];
extern char str_kT_POPUPCMNUnknownUser[];
extern char str_kT_POPUPCMNUnknownGame[];
extern char str_kT_POPUPCMNUnknownMachine[];
extern char str_kT_POPUPCMNInvalidRange[];
extern char str_kT_POPUPCMNInvalidStartPoint[];
extern char str_kT_POPUPCMNUserDetention[];
extern char str_kT_POPUPCMNKicked[];
extern char str_kT_POPUPCMNHostNotExist[];
extern char str_kT_POPUPCMNUserInGame[];
extern char str_kT_POPUPCMNSelf[];
extern char str_kT_POPUPCMNRejectPrivate[];
extern char str_kT_POPUPCMNNoRoom[];
extern char str_kT_POPUPCMNNoGame[];
extern char str_kT_POPUPCMNInRoom[];
extern char str_kT_POPUPCMNPrivileges[];
extern char str_kT_POPUPCMNNewsStartIndex[];
extern char str_kT_POPUPCMNPersonaSet[];
extern char str_kT_POPUPCMNNotOwner[];
extern char str_kT_POPUPCMNAlreadyLoggedIn[];
extern char str_kT_POPUPCMNProfanity[];
extern char str_pct_s_pct_S_pct_S[];
extern char str_kT_POPUPCMNLocked[];
extern char str_EC_LOCKED[];
extern char str_kT_POPUPCMNReserved[];
extern char str_kT_POPUPCMNTOSA[];
extern char str_kT_POPUPCMNSameHost[];
extern char str_kT_POPUPCMNIgnore[];
extern char str_kT_POPUPCMNAlreadyChallenge[];
extern char str_kT_POPUPCMNNoGameParams[];
extern char str_kT_POPUPCMNNoSpace[];
extern char str_kT_POPUPCMNDisabled[];
extern char str_kT_POPUPCMNWolf[];
extern char str_kT_POPUPCMNKickHost[];
extern char str_kT_POPUPCMNNotRanked[];
extern char str_kT_POPUPCMNBogus[];
extern char str_kT_POPUPCMNGameQuit[];
extern char str_kT_POPUPCMNGameFinishedEarly[];
extern char str_kT_POPUPCMNTimeout[];
extern char str_kT_POPUPCMNOverflow[];
extern char str_kT_POPUPCMNBadCommandLine[];
extern char str_kT_POPUPCMNUserOnline[];
extern char str_kT_POPUPCMNUserUnranked[];
extern char str_kT_POPUPCMNUserNotKicked[];
extern char str_kT_POPUPCMNBlackListed[];
extern char str_kT_POPUPCMNInvalidEmail[];
extern char str_kT_POPUP211InvalidParentalEmail[];
extern char str_kT_POPUPCMNInvalidBirthdate[];
extern char str_kT_POPUPCMNInvalidGender[];
extern char str_kT_POPUPCMNInvalidSpam[];
extern char str_kT_POPUPCMNRoomAuth[];
extern char str_kT_POPUPCMNNoEmail[];
extern char str_kT_POPUPCMNTooMany[];
extern char str_kT_POPUPCMNClientNotConnected[];
extern char str_kT_POPUPCMNInvalidGame[];
extern char str_kT_POPUPCMNInvalidIPAddress[];
extern char str_kT_POPUPCMNNotPlayer[];
extern char str_kT_POPUPCMNCheating[];
extern char str_kT_POPUPCMNTooYoung[];
extern char str_kT_POPUPCMNNoServer[];
extern char str_kT_POPUP210AccountNameLength[];
extern char str_kT_POPUP210EmailLength[];
extern char str_kT_POPUPCMNInvalidTournamentLength[];
extern char str_kT_POPUPCMNNotJoined[];
extern char str_kT_POPUPCMNAlreadyJoined[];
extern char str_kT_POPUPCMNNotFound[];
extern char str_kT_POPUPCMNAlreadyExists[];
extern char str_kT_POPUPCMNInvalidTeam[];
extern char str_kT_POPUPCMNMaxTourn[];
extern char str_kT_POPUPCMNTournStarted[];
extern char str_kT_POPUPCMNInvalidNewPass[];
extern char str_kT_POPUPCMNCheatDevice[];
extern char str_kT_POPUP200Maintenance2[];
extern char str_kT_POPUPCMNUnknownServerError[];

struct sVEntry_1A9CC0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

struct sBXString1A9CC0 {
    char* str;
    sBXString1A9CC0() {}
    sBXString1A9CC0(const sBXString1A9CC0& o);
};

// Popup message line: type 3 = string id.
static inline void SetMsg_1A9CC0(char* msg, char* id)
{
    *(int*)(msg + 8) = GetHashValue32(id);
    *(int*)(msg + 4) = 3;
}

static inline void SetText_1A9CC0(char* popup, char* id)
{
    *(int*)(popup + 0xC4) = GetHashValue32(id);
    *(int*)(popup + 0xC0) = 3;
}

static inline void* GetSession_1A9CC0()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

// Builds the popup for an online error reply (four-character code at data+8).
extern "C" void func_001A9CC0(cFEStateNet* self, void** popup, void* owner, void* data)
{
    int unknown = 0;
    int flag = 0;
    char* p = (char*)func_001A9C50(self);
    *popup = p;
    char* sub = p + 0xBC;
    *(int*)(p + 0xBC) = 1;
    func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
    *(int*)(p + 0x150) = 258;
    switch (*(int*)((char*)data + 8)) {
    case 0x696D7374: // 'imst'
        SetMsg_1A9CC0(sub, str_kT_POPUPTITLEInvalidAccount);
        break;
    case 0x69706572: // 'iper'
        SetText_1A9CC0(p, str_kT_POPUPCMNUserInvalid);
        break;
    case 0x70617373: // 'pass'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNPasswordInvalid);
        break;
    case 0x64626572: // 'dber'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNDatabaseError);
        break;
    case 0x6D697373: // 'miss'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNMissingParameter);
        break;
    case 0x696E7670: // 'invp'
        SetText_1A9CC0(p, str_kT_POPUPCMNInvalidParameters);
        break;
    case 0x6475706C: // 'dupl'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNDuplicate);
        break;
    case 0x6D617870: // 'maxp'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNMaxPersona);
        break;
    case 0x64656670: // 'defp'
        SetText_1A9CC0(p, str_kT_POPUPCMNDefaultPersona);
        break;
    case 0x6D617574: // 'maut'
        SetText_1A9CC0(p, str_kT_POPUPCMNMasterAuth);
        *(int*)(p + 0x150) = 2109;
        break;
    case 0x70617574: // 'paut'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNPersonaAuth);
        break;
    case 0x75726F6D: // 'urom'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNUnknownRoom);
        break;
    case 0x756C7374: // 'ulst'
        SetText_1A9CC0(p, str_kT_POPUPCMNUnknownRankingList);
        break;
    case 0x75757372: // 'uusr'
        SetText_1A9CC0(p, str_kT_POPUPCMNUnknownUser);
        break;
    case 0x7567616D: // 'ugam'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNUnknownGame);
        break;
    case 0x756D6163: // 'umac'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNUnknownMachine);
        break;
    case 0x72616E67: // 'rang'
        SetText_1A9CC0(p, str_kT_POPUPCMNInvalidRange);
        break;
    case 0x73747274: // 'strt'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInvalidStartPoint);
        break;
    case 0x64657465: // 'dete'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNUserDetention);
        break;
    case 0x6B69636B: // 'kick'
        SetText_1A9CC0(p, str_kT_POPUPCMNKicked);
        break;
    case 0x686F7374: // 'host'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNHostNotExist);
        break;
    case 0x696E676D: // 'ingm'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNUserInGame);
        break;
    case 0x73656C66: // 'self'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNSelf);
        break;
    case 0x6E707269: // 'npri'
        SetText_1A9CC0(p, str_kT_POPUPCMNRejectPrivate);
        break;
    case 0x6E726F6D: // 'nrom'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNoRoom);
        break;
    case 0x6E67616D: // 'ngam'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNoGame);
        break;
    case 0x726F6F6D: // 'room'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInRoom);
        break;
    case 0x61757468: // 'auth'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNPrivileges);
        break;
    case 0x6E657730: // 'new0'
        SetText_1A9CC0(p, str_kT_POPUPCMNNewsStartIndex);
        break;
    case 0x70736574: // 'pset'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNPersonaSet);
        break;
    case 0x6E6F776E: // 'nown'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNotOwner);
        break;
    case 0x6C6F676E: // 'logn'
        SetText_1A9CC0(p, str_kT_POPUPCMNAlreadyLoggedIn);
        break;
    case 0x66696C74: // 'filt'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNProfanity);
        break;
    case 0x6C6F636B: // 'lock'
        {
            const char* fmt = str_pct_s_pct_S_pct_S;
            const char* who = *(const char**)((char*)data + 0x10);
            char* obj = (char*)SSXApp->FELocale;
            const char* name = *(const char**)((char*)data + 0xC);
            sVEntry_1A9CC0* vt = *(sVEntry_1A9CC0**)(obj + 4);
            void* text = vt[4].fn(obj + vt[4].delta, GetHashValue32(str_kT_POPUPCMNLocked));
            int len = USTR_length(text) + strlen(who) + strlen(name) + 7;
            if (self->unk_6B8 != 0) {
                cMemMan_free(self->unk_6B8);
                self->unk_6B8 = 0;
            }
            void* buf = operator_new_tag(len * 2, str_EC_LOCKED, 0, 0);
            self->unk_6B8 = buf;
            sprintf2_1A9CC0(buf, fmt, text, who, name);
            *(void**)(sub + 8) = self->unk_6B8;
            *(int*)(sub + 4) = 1;
        }
        break;
    case 0x72737276: // 'rsrv'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNReserved);
        break;
    case 0x746F7361: // 'tosa'
        SetText_1A9CC0(p, str_kT_POPUPCMNTOSA);
        break;
    case 0x73616D65: // 'same'
        SetText_1A9CC0(p, str_kT_POPUPCMNSameHost);
        break;
    case 0x69676E6F: // 'igno'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNIgnore);
        break;
    case 0x6368616C: // 'chal'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNAlreadyChallenge);
        break;
    case 0x67706172: // 'gpar'
        SetText_1A9CC0(p, str_kT_POPUPCMNNoGameParams);
        break;
    case 0x66756C6C: // 'full'
        SetText_1A9CC0(p, str_kT_POPUPCMNNoSpace);
        break;
    case 0x64697361: // 'disa'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNDisabled);
        break;
    case 0x776F6C66: // 'wolf'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNWolf);
        break;
    case 0x6B696B68: // 'kikh'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNKickHost);
        break;
    case 0x72616E6B: // 'rank'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNotRanked);
        break;
    case 0x626F6775: // 'bogu'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNBogus);
        break;
    case 0x71756974: // 'quit'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNGameQuit);
        break;
    case 0x736F6F6E: // 'soon'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNGameFinishedEarly);
        break;
    case 0x74696D65: // 'time'
        SetText_1A9CC0(p, str_kT_POPUPCMNTimeout);
        break;
    case 0x6F766572: // 'over'
        SetText_1A9CC0(p, str_kT_POPUPCMNOverflow);
        break;
    case 0x62616463: // 'badc'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNBadCommandLine);
        break;
    case 0x6F6E6C6E: // 'onln'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNUserOnline);
        break;
    case 0x61756E72: // 'aunr'
        SetText_1A9CC0(p, str_kT_POPUPCMNUserUnranked);
        break;
    case 0x6E6B696B: // 'nkik'
        SetText_1A9CC0(p, str_kT_POPUPCMNUserNotKicked);
        break;
    case 0x626C616B: // 'blak'
        SetText_1A9CC0(p, str_kT_POPUPCMNBlackListed);
        break;
    case 0x6D61696C: // 'mail'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInvalidEmail);
        break;
    case 0x706D616C: // 'pmal'
        SetText_1A9CC0(p, str_kT_POPUP211InvalidParentalEmail);
        break;
    case 0x626F726E: // 'born'
        SetText_1A9CC0(p, str_kT_POPUPCMNInvalidBirthdate);
        break;
    case 0x67656E64: // 'gend'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInvalidGender);
        break;
    case 0x7370616D: // 'spam'
        SetText_1A9CC0(p, str_kT_POPUPCMNInvalidSpam);
        break;
    case 0x72617574: // 'raut'
        SetText_1A9CC0(p, str_kT_POPUPCMNRoomAuth);
        break;
    case 0x6E656D6C: // 'neml'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNoEmail);
        break;
    case 0x6D616E79: // 'many'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNTooMany);
        break;
    case 0x65636F6E: // 'econ'
        SetText_1A9CC0(p, str_kT_POPUPCMNClientNotConnected);
        break;
    case 0x6967616D: // 'igam'
        SetText_1A9CC0(p, str_kT_POPUPCMNInvalidGame);
        break;
    case 0x69616472: // 'iadr'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInvalidIPAddress);
        break;
    case 0x6E706C61: // 'npla'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNotPlayer);
        break;
    case 0x63686574: // 'chet'
        SetText_1A9CC0(p, str_kT_POPUPCMNCheating);
        break;
    case 0x746F6F79: // 'tooy'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNTooYoung);
        break;
    case 0x6E737276: // 'nsrv'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNoServer);
        break;
    case 0x69706173: // 'ipas'
        SetMsg_1A9CC0(sub, str_kT_POPUP210AccountNameLength);
        break;
    case 0x696E616D: // 'inam'
        SetText_1A9CC0(p, str_kT_POPUP210AccountNameLength);
        break;
    case 0x656C656E: // 'elen'
        SetMsg_1A9CC0(sub, str_kT_POPUP210EmailLength);
        break;
    case 0x6974726E: // 'itrn'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInvalidTournamentLength);
        break;
    case 0x6A6F696E: // 'join'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNotJoined);
        break;
    case 0x616A6F69: // 'ajoi'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNAlreadyJoined);
        break;
    case 0x6E737063: // 'nspc'
        SetText_1A9CC0(p, str_kT_POPUPCMNNoSpace);
        break;
    case 0x6E666E64: // 'nfnd'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNNotFound);
        break;
    case 0x78697374: // 'xist'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNAlreadyExists);
        break;
    case 0x7465616D: // 'team'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInvalidTeam);
        break;
    case 0x6D617874: // 'maxt'
        SetText_1A9CC0(p, str_kT_POPUPCMNMaxTourn);
        break;
    case 0x74737461: // 'tsta'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNTournStarted);
        break;
    case 0x696E7073: // 'inps'
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNInvalidNewPass);
        break;
    case 0x63646576: // 'cdev'
        flag = 1;
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNCheatDevice);
        break;
    case 0x646F776E: // 'down'
        {
            sBXString1A9CC0 str;
            cBXString_cBXString1(&str, (char*)data + 0xC);
            int pos;
            while ((pos = cBXString_FindLastOf((cBXString*)&str, '\n', 0)) != -1) {
                int next = pos + 1;
                if (str.str[next] == '\n') {
                    cBXString_setAt((cBXString*)&str, pos, '\r');
                    cBXString_setAt((cBXString*)&str, next, '\r');
                } else {
                    cBXString_setAt((cBXString*)&str, pos, ' ');
                }
            }
            *(char**)(sub + 8) = str.str;
            *(int*)(sub + 4) = 2;
            *(int*)(sub + 0x10) = GetHashValue32(str_kT_POPUP200Maintenance2);
            *(int*)(sub + 0xC) = 3;
            cBXString__cBXString(&str, 2);
        }
        break;
    case 0x646E7366: // 'dnsf'
        unknown = 1;
        SetText_1A9CC0(p, str_kT_POPUPCMNNetInterrupted);
        break;
    default:
        unknown = 1;
        SetMsg_1A9CC0(sub, str_kT_POPUPCMNUnknownServerError);
        break;
    }
    if (!unknown) {
        if (!flag) {
            return;
        }
        *(int*)(sub + 0x94) = 2109;
    } else {
        func_00255FA0(cCommSystem_mCommSystem);
        *(int*)(sub + 0x94) = 2110;
    }
    func_00261408(GetSession_1A9CC0());
    func_0025B800(LobbyMan_mLobbyMan);
    func_002636F0(func_002591B8(), 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AA9B0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern void* cAppMan_TheAppMan;
extern char str_IconChallengeHint[];
extern char str_TextMessageCount[];
extern char str_IconMessageHint[];

class cFEVObj_001AA9B0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001AA9B0(cFEStateNet* self)
{
    unsigned int now = *(unsigned int*)((char*)cAppMan_TheAppMan + 0x18);
    cFEVObj_001AA9B0* a = (cFEVObj_001AA9B0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_IconChallengeHint));
    if (a != 0) {
        if (self->mChallengeHintFrame != 0) {
            a->v09((now - self->mChallengeHintFrame) % 60 > 30);
        } else {
            a->v09(0);
        }
    }
    cFEVObj_001AA9B0* b = (cFEVObj_001AA9B0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextMessageCount));
    a = (cFEVObj_001AA9B0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_IconMessageHint));
    if (a != 0) {
        if (self->mMessageHintFrame != 0) {
            a->v09((now - self->mMessageHintFrame) % 60 > 30);
            if (b != 0) {
                b->v09((now - self->mMessageHintFrame) % 60 > 30);
            }
        } else {
            a->v09(0);
            if (b != 0) {
                b->v09(0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AAB68);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262938(void* self);
extern void* cAppMan_TheAppMan;
extern char str_TextMessageCount[];
extern char str_pct_d_b[];
extern char D_004A1390[];

struct sSession1AAB68 {
    char pad[0xBC];
    void** mBegin;
    void** mEnd;
};

static inline int Count1AAB68(sSession1AAB68* s) { return s->mEnd - s->mBegin; }

static inline void* GetSession_001AAB68()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

class cFEVObj_001AAB68 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual int v29();
};


extern "C" void func_001AAB68(cFEStateNet* self)
{
    char buf[16];
    if (self->mpScreen == 0) {
        return;
    }
    if (((cFEVObj_001AAB68*)self)->v29() == 0) {
        return;
    }
    unsigned int now = *(unsigned int*)((char*)cAppMan_TheAppMan + 0x18);
    unsigned int d = now - (self)->mLastPollFrame;
    if (d > 0 && d < 60) {
        return;
    }
    (self)->mLastPollFrame = now;
    int n = func_00262938(GetSession_001AAB68());
    if ((self)->mMessageHintFrame != 0) {
        if (n <= 0) {
            (self)->mMessageHintFrame = 0;
        }
    } else {
        if (n > 0) {
            (self)->mMessageHintFrame = now;
        }
    }
    int has = Count1AAB68((sSession1AAB68*)GetSession_001AAB68()) != 0;
    if ((self)->mChallengeHintFrame != 0) {
        if (!has) {
            (self)->mChallengeHintFrame = 0;
        }
    } else {
        if (has) {
            (self)->mChallengeHintFrame = now;
        }
    }
    cFEVObj_001AAB68* text = (cFEVObj_001AAB68*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextMessageCount));
    if (text == 0) {
        return;
    }
    if (n > 0) {
        text->v09(1);
        sprintf(buf, str_pct_d_b, n);
        cUIText_setAsciiString((cUIText*)text, buf);
    } else {
        text->v09(0);
        cUIText_setAsciiString((cUIText*)text, D_004A1390);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AAD50);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_qualitypct_dpct_c[];

struct sCol1AAD50 {
    float r, g, b, a;
    sCol1AAD50(const float& r_, const float& g_, const float& b_, const float& a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cFEVObj_001AAD50 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sCol1AAD50& c);
};

extern "C" void func_001AAD50(void* screen, int slot, int ping)
{
    if (screen == 0) {
        return;
    }
    sCol1AAD50 col(1.0f, 0.2199999988079071f, 0.5400000214576721f, 0.10000000149011612f);
    int bars = 5;
    char name[32];
    cFEVObj_001AAD50* objs[5];
    unsigned char i;
    for (i = 0; i < 5; i++) {
        sprintf(name, str_qualitypct_dpct_c, slot + 1, i + 'a');
        cFEVObj_001AAD50* o = (cFEVObj_001AAD50*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        if (o != 0) {
            objs[i] = o;
        }
    }
    if (ping > 200 && ping < 300) {
        col = sCol1AAD50(1.0f, 0.2199999988079071f, 0.5400000214576721f, 0.10000000149011612f);
        bars = 4;
    } else if (ping > 300 && ping < 400) {
        col = sCol1AAD50(1.0f, 0.949999988079071f, 0.8199999928474426f, 0.20000000298023224f);
        bars = 3;
    } else if (ping > 400 && ping < 500) {
        col = sCol1AAD50(1.0f, 0.949999988079071f, 0.8199999928474426f, 0.20000000298023224f);
        bars = 2;
    } else if (ping > 500) {
        col = sCol1AAD50(1.0f, 0.800000011920929f, 0.1599999964237213f, 0.019999999552965164f);
        bars = 1;
    }
    int j;
    for (j = 0; j < 5; j++) {
        cFEVObj_001AAD50* o = objs[j];
        if (j < bars) {
            o->v09(1);
            o->v11(col);
        } else {
            o->v09(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AAF90);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_POPUP200DifferentPS2[];
extern char str_kT_POPUP200Locate[];
extern char str_kT_POPUP200NetConfig[];
extern char str_kT_POPUP200HardwareInit[];
extern char str_kT_POPUP200Dialtone[];
extern char str_kT_POPUP200Busy[];
extern char str_kT_POPUP200Connect[];
extern char str_kT_POPUP200ISPFail[];
extern char str_kT_POPUP200PPP[];

struct cLocStr_001AAF90 {
    int f0;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual int v04(int hash);
};

extern "C" int func_001AAF90(cFEStateNet* self, void* msg)
{
    int h;
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x2D646966:
        h = GetHashValue32(str_kT_POPUP200DifferentPS2);
        break;
    case 0x2D646576:
        h = GetHashValue32(str_kT_POPUP200Locate);
        break;
    case 0x2D636667:
        h = GetHashValue32(str_kT_POPUP200NetConfig);
        break;
    case 0x2D696E69:
        h = GetHashValue32(str_kT_POPUP200HardwareInit);
        break;
    case 0x2D746F6E:
        h = GetHashValue32(str_kT_POPUP200Dialtone);
        break;
    case 0x2D627379:
        h = GetHashValue32(str_kT_POPUP200Busy);
        break;
    case 0x2D636F6E:
        h = GetHashValue32(str_kT_POPUP200Connect);
        break;
    case 0x2D3F3F3F:
        h = GetHashValue32(str_kT_POPUP200ISPFail);
        break;
    case 0x2D617468:
        h = GetHashValue32(str_kT_POPUP200PPP);
        break;
    case 0x2D646E73:
        h = GetHashValue32(str_kT_POPUP200ISPFail);
        break;
    case 0x74696D65:
        h = GetHashValue32(str_kT_POPUP200ISPFail);
        break;
    default:
        return 0;
    }
    return ((cLocStr_001AAF90*)SSXApp->FELocale)->v04(h);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB108);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern char str_kT_OVRCMNWaitPlayer[];

extern "C" void func_001AB108(cFEStateNet* self)
{
    void* popup = func_001A9C50(self);
    *(int*)((char*)popup + 0xBC) = 0;
    *(int*)((char*)popup + 0x170) = 0;
    *(int*)((char*)popup + 0x150) = 2102;
    *(int*)((char*)popup + 0x14C) = 6;
    *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_OVRCMNWaitPlayer);
    *(int*)((char*)popup + 0xC0) = 3;
    *(int*)((char*)popup + 0x164) = 0;
    func_001A9BC0(self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB178);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_kT_POPUPCMNBadVersion[];
extern char str_kT_POPUPCMNConnectionLost[];
extern char str_kT_POPUPCMNNotEstablish[];
extern char str_kT_POPUPCMNIgnore[];
extern char str_kT_POPUP241DeclineChallenge[];
extern char str_kT_POPUPCMNExitedChallenge[];
// Shows an error popup with the message matching the error code.
extern "C" void func_001AB178(cFEStateNet* self, int err)
{
    void* popup = func_001A9C50(self);
    char* sub = (char*)popup + 0xBC;
    *(int*)((char*)popup + 0xBC) = 1;
    func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
    *(int*)((char*)popup + 0x150) = 2101;
    *(int*)((char*)popup + 0x14C) = 6;
    *(int*)((char*)popup + 0x170) = 0;
    switch (err) {
    case 242:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNBadVersion);
        *(int*)(sub + 0x4) = 3;
        break;
    case 241: case 268: case 275:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNConnectionLost);
        *(int*)(sub + 0x4) = 3;
        break;
    case 274:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNNotEstablish);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0xF0:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNIgnore);
        *(int*)(sub + 0x4) = 3;
        break;
    case 237:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUP241DeclineChallenge);
        *(int*)(sub + 0x4) = 3;
        break;
    case 238:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNExitedChallenge);
        *(int*)(sub + 0x4) = 3;
        break;
    }
    func_001A9BC0(self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB288);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char str_kT_BTNOK[];
extern char str_kT_POPUPCMNConnectionLost[];
extern char str_kT_POPUPCMNNotEstablish[];
extern char str_kT_POPUPCMNExitedChallenge[];
// Shows an error popup with the message matching the error code.
extern "C" void func_001AB288(cFEStateNet* self, int err)
{
    void* popup = func_001A9C50(self);
    char* sub = (char*)popup + 0xBC;
    *(int*)((char*)popup + 0xBC) = 1;
    func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
    *(int*)((char*)popup + 0x150) = 2103;
    *(int*)((char*)popup + 0x14C) = 6;
    *(int*)((char*)popup + 0x170) = 0;
    switch (err) {
    case 241: case 250: case 268: case 275:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNConnectionLost);
        *(int*)(sub + 0x4) = 3;
        break;
    case 274:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNNotEstablish);
        *(int*)(sub + 0x4) = 3;
        break;
    case 238: case 0xF8:
        *(int*)(sub + 0x8) = GetHashValue32(str_kT_POPUPCMNExitedChallenge);
        *(int*)(sub + 0x4) = 3;
        break;
    }
    func_001A9BC0(self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB370);
#ifdef SKIP_ASM
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001AB478(cFEStateNet* self, void* popup, int a1, int a2, int a3);

extern "C" void func_001AB370(cFEStateNet* self, int a1, int a2)
{
    void* p = func_001A9C50(self);
    self->unk_6C4 = 1;
    func_001AB478(self, p, a1, a2, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB3D0);
#ifdef SKIP_ASM
extern "C" void func_001AB478(cFEStateNet* self, void* popup, int a1, int a2, int a3);
extern "C" int cScreenPopup_getCurrentlySelected(void* popup);

extern "C" void func_001AB3D0(cFEStateNet* self, int a1, int a2)
{
    if ((self->mAttrState & 6) != 0) {
        void* popup = self->mpPopup;
        if (popup != 0 && self->unk_6C4 != 0) {
            int id = *(int*)((char*)popup + 0x18);
            if (id != 2059) {
                if (id != 2060) {
                    return;
                }
            }
            if ((*(unsigned char*)((char*)popup + 0x1D) & 0x3F) < 6) {
                func_001AB478(self, self->mpPopup, a1, a2, cScreenPopup_getCurrentlySelected(popup));
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB478);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00263490(void* self, const char* name);
extern "C" int func_002633C8(void* self, const char* name);
extern "C" int func_00263128(void* self, const char* name);
extern "C" int func_00262A38(void* self, int a1);
extern "C" int func_00262360(void* self, const char* name);
extern "C" int func_002623B0(void* self, const char* name);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cPopupInfo_clearPopupConfirmation(cPopupInfo* self, int a1);
extern "C" void func_001C5838(void* self, int a1, int a2, int a3);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void sprintf1(void* str, int a1, char* buf);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern char str_kT_TITLEChallengeReceived[];
extern char str_kT_TITLEChallenge[];
extern char str_kT_POPUP235ReadMessage[];
extern char str_kT_POPUP235SendMessage[];
extern char str_kT_POPUP279ConfirmRemoveMessenger[];
extern char str_kT_POPUP235RemoveMessenger[];
extern char str_kT_POPUP279ConfirmAddMessenger[];
extern char str_kT_POPUP235AddMessenger[];
extern char str_kT_POPUP235RemoveBlockConfirm[];
extern char str_kT_POPUP235UnblockUser[];
extern char str_kT_POPUP235BlockAllConfirm[];
extern char str_kT_POPUP235BlockUser[];
extern char str_kT_235ReportAbuse[];

struct sVEntry_001AB478 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline void* GetSession_001AB478()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

static inline int Text_001AB478(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001AB478* vt = *(sVEntry_001AB478**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

// Fills the popup message for one file: a status header, then one line per check.
// PORT: a1 carries the name (char*) through an int parameter, as the unit declares the function.
extern "C" void func_001AB478(cFEStateNet* self, void* popup, int namePtr, int a2, int a3)
{
    if (popup == 0) {
        return;
    }
    const char* name = (const char*)namePtr;
    char* popupData = (char*)popup;
    int stateA = func_00263490(GetSession_001AB478(), name);
    int stateB = func_002633C8(GetSession_001AB478(), name);
    int n = 0;
    char* sub = popupData + 0xBC;
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    char nameStr[4];
    cBXString_cBXString2(nameStr, name);
    cBXString_operatorE((char*)lobbyMan + 0x98, nameStr);
    cBXString__cBXString(nameStr, 2);
    *(int*)(popupData + 0xC8) = 2;
    *(int*)(popupData + 0x14C) = 3;
    *(const char**)(popupData + 0xCC) = name;
    if (a2) {
        *(int*)(popupData + 0xBC) = 5;
        *(int*)(popupData + 0x150) = 2059;
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        if (func_00263128(GetSession_001AB478(), name)) {
            func_001C57C0(sub, n, GetHashValue32(str_kT_TITLEChallengeReceived));
        } else {
            func_001C57C0(sub, n, GetHashValue32(str_kT_TITLEChallenge));
        }
        n++;
    } else if (stateB != 0 && (stateA != 0 || func_00263128(GetSession_001AB478(), name))) {
        *(int*)sub = 4;
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        if (func_00263128(GetSession_001AB478(), name)) {
            func_001C57C0(sub, n, GetHashValue32(str_kT_TITLEChallengeReceived));
        } else {
            func_001C57C0(sub, n, GetHashValue32(str_kT_TITLEChallenge));
        }
        n++;
        *(int*)(sub + 0x94) = 2059;
    } else {
        *(int*)sub = 3;
        *(int*)(sub + 0x94) = 2060;
    }
    if (func_00262A38(GetSession_001AB478(), namePtr)) {
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        func_001C57C0(sub, n++, GetHashValue32(str_kT_POPUP235ReadMessage));
    } else {
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        func_001C57C0(sub, n++, GetHashValue32(str_kT_POPUP235SendMessage));
    }
    if (func_00262360(GetSession_001AB478(), name)) {
        char* buf = (char*)self + 0x49C;
        sprintf1(buf, Text_001AB478(str_kT_POPUP279ConfirmRemoveMessenger), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(str_kT_POPUP235RemoveMessenger));
        n++;
    } else {
        char* buf = (char*)self + 0x49C;
        sprintf1(buf, Text_001AB478(str_kT_POPUP279ConfirmAddMessenger), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(str_kT_POPUP235AddMessenger));
        n++;
    }
    if (func_002623B0(GetSession_001AB478(), name)) {
        char* buf = (char*)self + 0x59C;
        sprintf1(buf, Text_001AB478(str_kT_POPUP235RemoveBlockConfirm), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(str_kT_POPUP235UnblockUser));
        n++;
    } else {
        char* buf = (char*)self + 0x59C;
        sprintf1(buf, Text_001AB478(str_kT_POPUP235BlockAllConfirm), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(str_kT_POPUP235BlockUser));
        n++;
    }
    if (a2) {
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        func_001C57C0(sub, n, GetHashValue32(str_kT_235ReportAbuse));
    }
    *(int*)(sub + 0xAC) = a3;
    *(int*)(sub + 0xB8) = 1;
    func_001A9BC0(self, popupData);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001ABB30);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00262A38(void* self, int a1);
extern "C" void* cFEStateReadMessagePopup_cFEStateReadMessagePopup(void* mem, int a1, void* owner, int a3);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern char D_004A1408[];
extern char str_cFEStateReadMessagePopup[];


// PORT: passes D_004A1408 through an int parameter.
extern "C" void func_001ABB30(cFEStateNet* feStateNet, int a1, void* a2, int a3)
{
    cFEStateNet* self = feStateNet;
    self->unk_6C4 = 0;
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    if (func_00262A38(BuddyMan_mBuddyMan, a1) != 0) {
        void* obj = cFEStateReadMessagePopup_cFEStateReadMessagePopup(cMemMan_alloc(1788, str_cFEStateReadMessagePopup, 0x100, 0), ((int)self->mpEngine), self, a1);
        cUIStateStack_pushPopup(*(char**)(*(char**)(((char*)self->mpScreen) + 0xD0) + 0x10) + 0x18, obj);
    } else {
        func_001A9090((cFEStateNet*)self, 0, a1, (int)D_004A1408, 0, 2, 0x3F);
        *(int*)(((char*)self->mpKeyboard) + 0x43C) = 0;
        cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 75, 1, 2);
        func_001CE468(((char*)self->mpKeyboard), 1);
        *(int*)(((char*)self->mpKeyboard) + 0x438) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABC50);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261EF8(void* self, int a1, int a2, int a3);

struct sFEObj_001ABC50 {
    char pad[0x6C4];
    int f6C4;
};

extern "C" void func_001ABC50(cFEStateNet* self, int a1, int a2)
{
    self->unk_6C4 = 0;
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00261EF8(BuddyMan_mBuddyMan, a1, 1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABCD0);
#ifdef SKIP_ASM
// PORT: the unit declares func_001A97B8 as returning int; this call site ignores the result
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_00263128(void* self, const char* name);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup(cFEStateChallengeReceivedPopup* self, int a1, int a2, const char* name);
extern void* cFEStateChallengePopup_cFEStateChallengePopup_BCD0(void* self, int a1, int a2) __asm__("cFEStateChallengePopup_cFEStateChallengePopup");
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void MEM_fill(void* dst, int c, int n);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
extern int D_004A4E88_v[2] __asm__("D_004A4E88");
extern char D_005308B8[];
extern char str_cFEStateChallengeReceivedPopup[];
extern char str_cFEStateChallengePopup[];

extern "C" void func_001ABCD0(cFEStateNet* self, int a1, void* a2, int a3, int a4)
{
    self->unk_6C4 = 0;
    func_001A97B8_v(self, a2, a3);
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    if (func_00263128(BuddyMan_mBuddyMan, (const char*)a1)) {
        void* p = cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup((cFEStateChallengeReceivedPopup*)cMemMan_alloc(1752, str_cFEStateChallengeReceivedPopup, 0x100, 0), (int)self->mpEngine, (int)self, (const char*)a1);
        if (a4) {
            *(int*)((char*)p + 0x18) = 2100;
        } else {
            *(int*)((char*)p + 0x18) = 2049;
        }
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    } else {
        MEM_fill(D_005308B8, 0, 0x20);
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        D_004A4E88_v[1] = 4;
        char s[4];
        cBXString_cBXString2(s, (const char*)a1);
        cBXString_operatorE((char*)lobbyMan + 0x98, s);
        cBXString__cBXString(s, 2);
        void* p = cFEStateChallengePopup_cFEStateChallengePopup_BCD0(cMemMan_alloc(1776, str_cFEStateChallengePopup, 0x100, 0), (int)self->mpEngine, (int)self);
        if (a4) {
            *(int*)((char*)p + 0x18) = 2100;
        } else {
            *(int*)((char*)p + 0x18) = 0x800;
        }
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABE58);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261EF8(void* self, int a1, int a2, int a3);

struct sFEObj_001ABE58 {
    char pad[0x6C4];
    int f6C4;
};

extern "C" void func_001ABE58(cFEStateNet* self, int a1, int a2)
{
    self->unk_6C4 = 0;
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00261EF8(BuddyMan_mBuddyMan, a1, 0, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABED8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char str_kT_BTNOK[];
extern char str_kT_POPUP235MaxBuddiesExceeded[];

extern "C" void func_001ABED8(void* self)
{
    void* popup = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)popup + 0xBC) = 1;
    func_001C57C0((char*)popup + 0xBC, 0, GetHashValue32(str_kT_BTNOK));
    int textHash = GetHashValue32(str_kT_POPUP235MaxBuddiesExceeded);
    //S
    *(int*)((char*)popup + 0xC4) = textHash;
    *(int*)((char*)popup + 0xC0) = 3;
    *(int*)((char*)popup + 0x150) = 2117;
    *(int*)((char*)popup + 0x14C) = 6;
    //E
    func_001A9BC0((cFEStateNet*)self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEBlankPopup_cFEBlankPopup);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void* self);
extern void* cFEBlankPopup_vtbl[];

extern "C" void* cFEBlankPopup_cFEBlankPopup(cFEStateNet* self)
{
    func_001A8500(self);
    self->mVtbl = cFEBlankPopup_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEBlankPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_Blank[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void* cFEBlankPopup_onCreateScreen(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_Blank), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount_cFEUseExistingAccount);
#ifdef SKIP_ASM
extern void* cFEUseExistingAccount_vtbl[];
extern int cBXString_EmptyString;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the "use existing account" screen state.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with no parent, stores cFEUseExistingAccount_vtbl and mHashName
 * 0x35, starts mszEmail as the empty string (cBXString_EmptyString), and clears unk_6D0, mpTextForgotPassword and unk_6A8 (so
 * net messages are not filtered on this screen).
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEUseExistingAccount_cFEUseExistingAccount(cFEUseExistingAccount* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    int emptyStr = cBXString_EmptyString;
    self->mVtbl = cFEUseExistingAccount_vtbl;
    self->mszEmail = (char*)emptyStr;
    self->mHashName = 53;
    self->unk_6D0 = 0;
    self->mpTextForgotPassword = 0;
    self->unk_6A8 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount__cFEUseExistingAccount);
#ifdef SKIP_ASM
extern void* cFEUseExistingAccount_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the "use existing account" state: destroys mszEmail, then runs the cFEStateNet destructor (func_001A85D0).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEUseExistingAccount__cFEUseExistingAccount(cFEUseExistingAccount* self, int flags)
{
    self->mVtbl = cFEUseExistingAccount_vtbl;
    cBXString__cBXString((char*)self + 0x6DC, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void func_001A8768(void* self);
extern char str_204use_existing_account[];
extern char D_004A1408[];

/**
 * Creates the "204use_existing_account" screen and clears LobbyMan's two login strings (+0x48 and +0x4C).
 *
 * Adds and plays the screen, assigns "" to both LobbyMan strings and runs the cFEStateNet hook (func_001A8768).
 */
extern "C" void cFEUseExistingAccount_onCreateScreen(cFEUseExistingAccount* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_204use_existing_account), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, D_004A1408);
    cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, D_004A1408);
    func_001A8768(((cFEStateNet*)self));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the "use existing account" state: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEUseExistingAccount_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char str_PairAccountName[];
extern char str_PairPassword[];
extern char str_TextForgotPassword[];
extern char str_TextForgotAccount[];
extern char str_EnterAccountName[];
extern char str_EnterPassword[];
extern char str_TextOK[];
extern char D_004A1408[];

struct cFEVObj_001AC170 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25(int on);
};

static inline int IsHash_001AC170(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Numbers the "use existing account" widgets as the screen creates them.
 *
 * Widget ids (mMiscData, +0x18): "PairAccountName" 0, "PairPassword" 1, "TextForgotPassword" 2 (also kept in
 * mpTextForgotPassword, then setUseStaticColour(1) and setStatic(1)), "TextForgotAccount" 3, "TextOK" 4.
 * "EnterAccountName" and "EnterPassword" start empty.
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEUseExistingAccount_onWidgetCreate(cFEUseExistingAccount* self, cUIText* text)
{
    if (IsHash_001AC170(*(int*)((char*)text + 0x38), str_PairAccountName)) {
        *(int*)((char*)text + 0x18) = 0;
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), str_PairPassword)) {
        *(int*)((char*)text + 0x18) = 1;
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), str_TextForgotPassword)) {
        self->mpTextForgotPassword = (cUIThing*)text;
        *(int*)((char*)text + 0x18) = 2;
        ((cFEVObj_001AC170*)self->mpTextForgotPassword)->v25(1);
        ((cFEVObj_001AC170*)self->mpTextForgotPassword)->v08(1);
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), str_TextForgotAccount)) {
        *(int*)((char*)text + 0x18) = 3;
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), str_EnterAccountName) || IsHash_001AC170(*(int*)((char*)text + 0x38), str_EnterPassword)) {
        cUIText_setAsciiString(text, D_004A1408);
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), str_TextOK)) {
        *(int*)((char*)text + 0x18) = 4;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC2B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char str_kT_POPUP210PasswordMissing[];
extern char str_kT_BTNOK[];

extern "C" int func_001AC2B8(cFEUseExistingAccount* self)
{
    if (strlen(((const char*)LobbyMan_mLobbyMan->mPassword)) == 0) {
    void* popup = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)popup + 0xBC;
    *(int*)((char*)popup + 0xBC) = 1;
    *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210PasswordMissing);
    *(int*)((char*)popup + 0xC0) = 3;
    func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
    *(int*)((char*)popup + 0x150) = 2061;
    func_001A9BC0((cFEStateNet*)self, popup);
    return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC360);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char str_kT_POPUP210AccountNameMissing[];
extern char str_kT_BTNOK[];

extern "C" int func_001AC360(cFEUseExistingAccount* self)
{
    if (strlen(((const char*)LobbyMan_mLobbyMan->mUserName)) == 0) {
    void* popup = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)popup + 0xBC;
    *(int*)((char*)popup + 0xBC) = 1;
    *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210AccountNameMissing);
    *(int*)((char*)popup + 0xC0) = 3;
    func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
    *(int*)((char*)popup + 0x150) = 2061;
    func_001A9BC0((cFEStateNet*)self, popup);
    return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC408);
#ifdef SKIP_ASM
extern "C" int func_001AC360(cFEUseExistingAccount* self);
extern "C" int func_001AC2B8(cFEUseExistingAccount* self);

extern "C" int func_001AC408(cFEUseExistingAccount* self)
{
    bool ok = true;
    ok = ok && func_001AC360(self);
    ok = ok && func_001AC2B8(self);
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC448);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern char str_kT_POPUP210EmailMissing[];
extern char str_kT_BTNOK[];

extern "C" int func_001AC448(cFEUseExistingAccount* self)
{
    // PORT: string header read through a pointer held in an int
    if (*(int*)((int)self->mszEmail - 8) == 0) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210EmailMissing);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC4F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP204UseAnotherAccount[];
extern char str_kT_HELP204UseAnotherAccountMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001AC4F0(cFEUseExistingAccount* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP204UseAnotherAccount);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP204UseAnotherAccountMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(popup, 2, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the "use existing account" screen: reports event 9 as handled (0x100), any other as not (0x101).
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (On Tour Event)
 * @return 0x100 for event 9, else 0x101
 */
extern "C" int cFEUseExistingAccount_onWidgetMenu(cFEUseExistingAccount* self, int pMenu, int Event)
{
    return Event != 9 ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern char str_kT_BTNOK[];
extern char str_kT_BTNCancel[];
extern char str_cFEStateNetAccountMem[];
extern "C" void* cUIScreen_getObjectByHashName_C5C0(void* self, int hash) __asm__("cUIScreen_getObjectByHashName");
extern "C" int func_001AC408_C5C0(void* self) __asm__("func_001AC408");
extern "C" int func_001AC360_C5C0(void* self) __asm__("func_001AC360");
extern "C" void func_001ACDD0_C5C0(void* self) __asm__("func_001ACDD0");
extern "C" void func_0025B6D8_C5C0(void* self, int a1) __asm__("func_0025B6D8");
extern char str_EnterAccountName[];
extern char str_EnterPassword[];
extern char str_kT_POPUP201Verify[];
extern char str_kT_POPUP204RequestPassword[];
extern char str_kT_POPUP204CheckAccount[];
extern char str_kT_210EmailAddress[];
extern char str_kT_204AccountName[];
extern char str_kT_210Password[];


/**
 * Handles the "use existing account" widgets: name and password entry, verify, forgotten password or account, back.
 *
 * Event 5 by widget id: 4 ("TextOK", once func_001AC408 accepts) shows a "kT_POPUP201Verify" popup with a cancel
 * option (id 0x80E), sets miRequest 4 and connects LobbyMan (func_0025B6D8); 2 (forgot password, once func_001AC360
 * accepts) shows "kT_POPUP204RequestPassword" (id 0x812), sets miRequest 2 and connects; 3 (forgot account) shows an
 * e-mail entry popup "kT_POPUP204CheckAccount" ("kT_210EmailAddress", at most 50 characters, OK; id 0x80F); 0 and 1
 * open the keyboard (func_001A9150) on "EnterAccountName" / "EnterPassword" with LobbyMan's mUserName / mPassword, at
 * most 16 characters. Event 6 pushes a cFEStateNetAccountMem popup (miMiscData 0x832); event 13 (keyboard done) calls
 * func_001ACDD0; the rest go to the cFEStateNet handler. The comment above it ("the network-config menu") is stale.
 *
 * @param pWidget  the widget (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
// Message handler for the network-config menu.
extern "C" void cFEUseExistingAccount_onWidgetEvent(cFEUseExistingAccount* self, void* pWidget, unsigned int msg)
{
    switch (msg) {
    case 5:
        switch (*(unsigned int*)((char*)pWidget + 0x18)) {
        case 4: {
            if (!func_001AC408_C5C0(((char*)self))) {
                break;
            }
            void* popup = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xBC) = 1;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP201Verify);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNCancel));
            *(int*)((char*)popup + 0x150) = 2062;
            func_001A9BC0((cFEStateNet*)self, popup);
            (self)->miRequest = 4;
            func_0025B6D8_C5C0(LobbyMan_mLobbyMan, 0);
            break;
        }
        case 2: {
            if (!func_001AC360_C5C0(((char*)self))) {
                break;
            }
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 0;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP204RequestPassword);
            *(int*)((char*)popup + 0xC0) = 3;
            *(int*)((char*)popup + 0x164) = 0;
            *(int*)((char*)popup + 0x150) = 2066;
            func_001A9BC0((cFEStateNet*)self, popup);
            (self)->miRequest = 2;
            func_0025B6D8_C5C0(LobbyMan_mLobbyMan, 0);
            break;
        }
        case 3: {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = -1;
            *(int*)((char*)popup + 0x180) = 50;
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP204CheckAccount);
            *(int*)((char*)popup + 0xC0) = 3;
            *(int*)((char*)popup + 0xD4) = GetHashValue32(str_kT_210EmailAddress);
            *(int*)((char*)popup + 0xD0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)popup + 0x150) = 2063;
            func_001A9BC0((cFEStateNet*)self, popup);
            break;
        }
        case 0: {
            void* obj = cUIScreen_getObjectByHashName_C5C0(self->mpScreen, GetHashValue32(str_EnterAccountName));
            if (obj == 0) {
                break;
            }
            int inputText = (int)LobbyMan_mLobbyMan->mUserName;
            func_001A9150((cFEStateNet*)self, (int)obj, GetHashValue32(str_kT_204AccountName), inputText, 0, 6, 0x10);
            *(int*)((char*)self->mpKeyboard + 0x58) = 1;
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            break;
        }
        case 1: {
            void* obj = cUIScreen_getObjectByHashName_C5C0(self->mpScreen, GetHashValue32(str_EnterPassword));
            if (obj == 0) {
                break;
            }
            int inputText = (int)LobbyMan_mLobbyMan->mPassword;
            func_001A9150((cFEStateNet*)self, (int)obj, GetHashValue32(str_kT_210Password), inputText, 1, 7, 0x10);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            break;
        }
        }
        break;
    case 6: {
        void* popup = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, ((char*)self), 0, 0, 0);
        *(int*)((char*)popup + 0x18) = 2098;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, popup);
        break;
    }
    case 13:
        func_001ACDD0_C5C0(((char*)self));
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEUseExistingAccount_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001AC9C0(cFEUseExistingAccount* self, void* a1, int a2);
extern "C" void func_001ACA88(cFEUseExistingAccount* self, void* a1, int a2);
extern "C" void func_001ACB48(cFEUseExistingAccount* self, void* a1, int a2);
extern "C" void func_001ACCD0(cFEUseExistingAccount* self, void* a1, int a2);

/**
 * Hands a popup's event on the "use existing account" screen to the handler for that popup's id.
 *
 * By the popup's miMiscData (+0x18): 0x80E (verify) func_001ACA88, 0x80F (forgot account) func_001AC9C0, 0x82C (the
 * account memory-card popup) func_001ACB48, 0x81E (save account) func_001ACCD0, any other the cFEStateNet handler
 * (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEUseExistingAccount_onPopupEvent(cFEUseExistingAccount* self, void* pPopup, int Event)
{
    switch (*(unsigned int*)((char*)pPopup + 0x18)) {
    case 2062:
        func_001ACA88(self, pPopup, Event);
        break;
    case 2063:
        func_001AC9C0(self, pPopup, Event);
        break;
    case 2092:
        func_001ACB48(self, pPopup, Event);
        break;
    case 2078:
        func_001ACCD0(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC9C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" int func_001AC448(cFEUseExistingAccount* self);
extern "C" void func_0025B6D8(void* self, int a1);
extern char str_kT_POPUP204RequestAccount[];

extern "C" void func_001AC9C0(cFEUseExistingAccount* self, void* popup, int event)
{
    if (event == 0xF) {
        int three = 3;
        func_001A97B8(self, popup, 0xF);
        cBXString_cBXString4((char*)self + 0x6DC, *(const char**)((char*)popup + 0x2B0));
        void* newPopup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)newPopup + 0xBC) = 0;
        *(int*)((char*)newPopup + 0xC4) = GetHashValue32(str_kT_POPUP204RequestAccount);
        *(int*)((char*)newPopup + 0xC0) = three;
        *(int*)((char*)newPopup + 0x164) = 0;
        *(int*)((char*)newPopup + 0x150) = 2064;
        func_001A9BC0((cFEStateNet*)self, newPopup);
        if (func_001AC448(self) != 0) {
            self->miRequest = three;
            func_0025B6D8(LobbyMan_mLobbyMan, 0);
        }
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACA88);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

static inline void* GetSession_001ACA88()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001ACA88(cFEUseExistingAccount* self, void* popup, int event)
{
    if (event == 0xF || event == 20) {
        func_00261408(GetSession_001ACA88());
        func_0025B800(LobbyMan_mLobbyMan);
        func_001A97B8(self, popup, event);
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACB48);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
void func_001C57C0(void* self, int a1, int a2);
extern const char str_cFEStateSelectPersona[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char str_kT_POPUP204SaveAccount[];

extern "C" void func_001ACB48(cFEUseExistingAccount* self, void* popup, int kind)
{
    switch ((unsigned int)kind) {
    case 15:
    case 16:
        self->mbAskSave = 0;
        func_001A97B8(self, popup, kind);
        func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        break;
    case 17:
        self->mbAskSave = 1;
        func_001A97B8(self, popup, 17);
        break;
    case 22:
        if (self->mbAskSave != 0) {
            void* newPopup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)newPopup + 0xBC) = 2;
            char* sub = (char*)newPopup + 0xBC;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            *(int*)((char*)newPopup + 0x150) = 2078;
            *(int*)((char*)newPopup + 0xC4) = GetHashValue32(str_kT_POPUP204SaveAccount);
            *(int*)((char*)newPopup + 0xC0) = 3;
            *(int*)((char*)newPopup + 0x168) = 0;
            *(int*)((char*)newPopup + 0x164) = 0;
            func_001A9BC0((cFEStateNet*)self, newPopup);
        }
        self->mbAskSave = 0;
        func_001A97B8(self, popup, 22);
        break;
    default:
        func_001A97B8(self, popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACCD0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];
extern const char str_cFEStateSelectPersona[];

extern "C" void func_001ACCD0(cFEUseExistingAccount* self, void* popup, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(((cFEStateNet*)self), popup, 15);
        void* accountMem = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 1,
                                ((int)LobbyMan_mLobbyMan->mUserName), ((int)LobbyMan_mLobbyMan->mPassword));
        *(int*)((char*)accountMem + 0x18) = 2092;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, accountMem);
        break;
    }
    case 16:
    case 20:
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        break;
    default:
        func_001A97B8(((cFEStateNet*)self), popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACDD0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001A98B8(cFEStateNet* self, const char* str);
extern char str_EnterAccountName[];
extern char str_EnterPassword[];

struct sVE1ACDD0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Show1ACDD0(char* o, int on)
{
    sVE1ACDD0* vt = *(sVE1ACDD0**)(o + 0x8);
    vt[8].fn(o + vt[8].delta, on);
}

extern "C" void func_001ACDD0(cFEUseExistingAccount* self)
{
    char* keyboard = (char*)self->mpKeyboard;
    switch (*(int*)(keyboard + 0x18)) {
    case 6: {
        const char* name = keyboard + 0x74;
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterAccountName));
        if (text != 0) {
            cUIText_setAsciiString(text, name);
        }
        if (strlen(name) != 0) {
            Show1ACDD0((char*)self->mpTextForgotPassword, 0);
        } else {
            Show1ACDD0((char*)self->mpTextForgotPassword, 1);
        }
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, name);
        break;
    }
    case 7: {
        const char* name = keyboard + 0x74;
        if (cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterPassword)) != 0) {
            func_001A98B8((cFEStateNet*)self, name);
            cUIText_setAsciiString(self->mpKeyboardText, (char*)self + 0x5C);
        }
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, name);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ACEF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025C1E0(void* self, int a1);
extern "C" void func_0025C3F8(void* self, int a1);
extern "C" void func_0025C610(void* self);
extern "C" int func_001A8E40_3i(void* self, int a1, int a2) __asm__("func_001A8E40");
extern char str_kT_POPUP204MailAccount[];
extern char str_kT_POPUP204MailPassword[];
extern char str_kT_POPUP204SaveAccount[];
extern char str_kT_BTNOK[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

extern "C" int func_001ACEF8(cFEUseExistingAccount* self, int id, int data)
{
    switch (id) {
    case 201:
        switch (self->miRequest) {
        case 2:
            func_0025C3F8(LobbyMan_mLobbyMan, ((int)LobbyMan_mLobbyMan->mUserName));
            break;
        case 3:
            func_0025C1E0(LobbyMan_mLobbyMan, (int)self->mszEmail);
            break;
        case 4:
            func_0025C610(LobbyMan_mLobbyMan);
            break;
        }
        break;
    case 207: {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP204MailAccount);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2065;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    case 210: {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP204MailPassword);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2067;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    case 212: {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 2;
        char* sub = (char*)popup + 0xBC;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
        *(int*)((char*)popup + 0x150) = 2078;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP204SaveAccount);
        *(int*)((char*)popup + 0xC0) = 3;
        *(int*)((char*)popup + 0x168) = 0;
        *(int*)((char*)popup + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    default:
        return func_001A8E40_3i(self, id, data);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateSelectPersona_cFEStateSelectPersona);
#ifdef SKIP_ASM
extern void* cFEStateSelectPersona_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the persona-select screen state.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with no parent, stores mHashName 0x38 and
 * cFEStateSelectPersona_vtbl, and clears mbExistingPersona.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mHashName = 0x38;
    self->mVtbl = cFEStateSelectPersona_vtbl;
    self->mbExistingPersona = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateSelectPersona_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char str_220select_persona[];

/**
 * Creates and plays the "220select_persona" screen, then runs the cFEStateNet hook (func_001A8768).
 */
extern "C" void cFEStateSelectPersona_onCreateScreen(cFEStateSelectPersona* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_220select_persona), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001A8768(((cFEStateNet*)self));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateSelectPersona_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the persona-select state: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateSelectPersona_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD208);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025D1B8(void* self, const char* str);
extern char str_kT_POPUP220PersonaNameLength[];
extern char str_kT_BTNOK[];
extern char str_kT_POPUP220SubmittingPersona[];

extern "C" void func_001AD208(cFEStateSelectPersona* self, const char* name)
{
    if (strlen(name) < 3) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220PersonaNameLength);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2068;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 0;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220SubmittingPersona);
        *(int*)((char*)popup + 0xC0) = 3;
        *(int*)((char*)popup + 0x150) = 2069;
        *(int*)((char*)popup + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, popup);
        func_0025D1B8(LobbyMan_mLobbyMan, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateSelectPersona_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001AD3E8(cFEStateSelectPersona* self, cUIText* text);
extern char str_PersonaMenu[];
extern char str_EditAccount[];
extern char str_TextAccountName[];
extern char str_TextESRBNotice[];
extern char str_kT_220ESRBNotice[];

static inline int IsHash1AD308(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Sets up the persona-select widgets as the screen creates them.
 *
 * "PersonaMenu" is filled by func_001AD3E8, "EditAccount" gets id -1, "TextAccountName" shows LobbyMan's mUserName
 * and "TextESRBNotice" the "kT_220ESRBNotice" text.
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateSelectPersona_onWidgetCreate(cFEStateSelectPersona* self, cUIText* text)
{
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    if (IsHash1AD308(*(int*)((char*)text + 0x38), str_PersonaMenu)) {
        func_001AD3E8(self, text);
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), str_EditAccount)) {
        *(int*)((char*)text + 0x18) = -1;
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), str_TextAccountName)) {
        cUIText_setAsciiString(text, ((const char*)lobbyMan->mUserName));
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), str_TextESRBNotice)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_220ESRBNotice));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD3E8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" cUIText* cList_getByIndex(void* list, int i);
extern char str_kT_220CreateNewPersona[];

class cFEVObj_001AD3E8 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

struct sFEGlobals1AD3E8 {
    char pad[0x6C];
    int count;
    const char* names[4];
};

extern "C" void func_001AD3E8(cFEStateSelectPersona* self, cUIText* text)
{
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int i = 0;
    for (; i < lobbyMan->mNumPersonas; i++) {
        cUIText* item = cList_getByIndex((char*)text + 0x74, i);
        *(int*)((char*)item + 0x18) = i;
        cUIText_setAsciiString(item, ((const char**)&lobbyMan->mPersonas[0])[i]);
        ((cFEVObj_001AD3E8*)item)->v09(1);
    }
    for (; i < 4; i++) {
        cUIText* item = cList_getByIndex((char*)text + 0x74, i);
        *(int*)((char*)item + 0x18) = i;
        cUIText_setUnicodeStringByID(item, GetHashValue32(str_kT_220CreateNewPersona));
        ((cFEVObj_001AD3E8*)item)->v09(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD4F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP220Persona[];
extern char str_kT_HELP220CreateName[];
extern char str_kT_HELP220SelectName[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELP220DeleteName[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001AD4F0(cFEStateSelectPersona* self, void* menu)
{
    func_001DA528(menu, 4);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP220Persona);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP220CreateName));
    func_001DA530(menu, 1, GetHashValue32(str_kT_HELP220SelectName));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELP220DeleteName), 2);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(menu, 3, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateSelectPersona_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_kT_BTNCancel[];
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1_D5E8(void* mem, int a1, int a2) __asm__("cFEStateCreateStep1_cFEStateCreateStep1");
extern "C" void func_001AD208_D5E8(void* self, const char* name) __asm__("func_001AD208");
extern "C" void func_001C5878_D5E8(void* self, int a1, int a2, int a3) __asm__("func_001C5878");
void func_0025CD28(void* self, int a1);
extern char str_cFEStateCreateStep1[];
extern char str_kT_220CreateNewPersona[];
extern char str_kT_POPUP220DeleteInitial[];
extern char str_kT_BTNDelete[];
extern char str_kT_POPUP220DeleteFinal[];

/**
 * Handles the persona-select widgets: choose, create or delete a persona, and back.
 *
 * Event 5 by the row's id: -1 ("EditAccount") switches to a new cFEStateCreateStep1 (func_001A8A40); a row past
 * LobbyMan's mNumPersonas opens the keyboard titled "kT_220CreateNewPersona" (at most 12 characters) and clears
 * mbExistingPersona; an existing row selects that persona (func_0025CD28) and sets mbExistingPersona. Event 6
 * disconnects BuddyMan (func_00261408) and LobbyMan (func_0025B800) and pushes a cFEStateNetAccountMem popup (0x832).
 * Event 8 on an existing row stores it in miDeletePersona and asks "kT_POPUP220DeleteInitial" (Delete / Cancel,
 * Delete confirmed by "kT_POPUP220DeleteFinal"; id 0x816). Event 13 (keyboard done) creates the persona from the
 * typed name (func_001AD208); event 14 is ignored; the rest go to the cFEStateNet handler.
 *
 * @param selfp  the persona-select state (this)
 * @param pWidget  the widget (On Tour pWidget)
 * @param Event  the widget event (On Tour Event)
 */
// Message handler for the persona-select screen.
extern "C" void cFEStateSelectPersona_onWidgetEvent(cFEStateSelectPersona* selfp, void* pWidget, int Event)
{
    switch (Event) {
    case 5: {
        int idx = *(int*)((char*)pWidget + 0x18);
        if (idx == -1) {
            func_001A8A40(((char*)selfp), cFEStateCreateStep1_cFEStateCreateStep1_D5E8(cMemMan_alloc(1780, str_cFEStateCreateStep1, 0, 0), (int)selfp->mpEngine, 0));
            break;
        }
        if (idx >= LobbyMan_mLobbyMan->mNumPersonas) {
            func_001A9150((cFEStateNet*)selfp, (int)pWidget, GetHashValue32(str_kT_220CreateNewPersona), (int)D_004A1408, 0, -1, 0xC);
            *(int*)((char*)selfp->mpKeyboard + 0x58) = 1;
            cKeyboardPopup_setIncludeKey(selfp->mpKeyboard, 75, 1, 2);
            cKeyboardPopup_setIncludeKey(selfp->mpKeyboard, 53, 0, 2);
            selfp->mbExistingPersona = 0;
        } else {
            func_0025CD28(LobbyMan_mLobbyMan, idx);
            selfp->mbExistingPersona = 1;
        }
        break;
    }
    case 6: {
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80();
        }
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            func_002613C8(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800(LobbyMan_mLobbyMan);
        }
        func_00261408(BuddyMan_mBuddyMan);
        func_0025B800(LobbyMan_mLobbyMan);
        void* popup = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)selfp->mpEngine, ((char*)selfp), 0, 0, 0);
        *(int*)((char*)popup + 0x18) = 2098;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)selfp->mpScreen + 0xD0) + 0x10) + 0x18, popup);
        break;
    }
    case 8: {
        int idx = *(int*)((char*)pWidget + 0x18);
        if (idx < 0) {
            break;
        }
        if (idx < LobbyMan_mLobbyMan->mNumPersonas) {
            selfp->miDeletePersona = idx;
            void* popup = func_001A9C50((cFEStateNet*)selfp);
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xBC) = 2;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220DeleteInitial);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNDelete));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_BTNCancel));
            func_001C5878_D5E8(sub, 0, GetHashValue32(str_kT_POPUP220DeleteFinal), 1);
            *(int*)((char*)popup + 0x168) = 1;
            *(int*)((char*)popup + 0x150) = 2070;
            func_001A9BC0((cFEStateNet*)selfp, popup);
        }
        break;
    }
    case 13:
        func_001AD208_D5E8(((char*)selfp), (char*)selfp->mpKeyboard + 0x74);
        break;
    case 14:
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)selfp, (int)pWidget, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateSelectPersona_onPopupEvent);
#ifdef SKIP_ASM
class cFEVObj_001AD8C0 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001AD930(cFEStateSelectPersona* self, void* a1, int a2);

/**
 * Hands a popup's event on the persona-select screen to the handler for that popup's id.
 *
 * By the popup's miMiscData (+0x18): 0x816 (delete persona) goes to its slot 33 (func_001AD9C8), 0x819 to
 * func_001AD930, any other to the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateSelectPersona_onPopupEvent(cFEStateSelectPersona* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2070:
        ((cFEVObj_001AD8C0*)self)->v33(pPopup, Event);
        break;
    case 2073:
        func_001AD930(self, pPopup, Event);
        break;
    default:
        func_001A97B8(((cFEVObj_001AD8C0*)self), pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD930);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];

extern "C" void func_001AD930(cFEStateSelectPersona* self, void* popup, int event)
{
    if (event == 0xF) {
        func_001A97B8(((cFEStateNet*)self), popup, 0xF);
        void* accountMem = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 0, 0, 0);
        *(int*)((char*)accountMem + 0x18) = 2098;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, accountMem);
    } else {
        func_001A97B8(((cFEStateNet*)self), popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD9C8);
#ifdef SKIP_ASM
extern "C" void func_0025D428(void* self, void* a1);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001AD9C8(void* self, void* popup, int event)
{
    if (event == 0xF) {
        func_0025D428(LobbyMan_mLobbyMan, *(void**)((char*)self + 0x6D4));
        func_001A97B8(self, popup, 0xF);
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ADA30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cFEStateNet* mem, int a1);
extern "C" void func_0039F190(void* self, int a1);
extern "C" void func_001ADC30(cFEStateSelectPersona* self, int a2);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern const char str_cFEStateOnlineMainMenu[];
extern char str_kT_BTNOK[];
extern char str_PersonaMenu[];
extern char str_kT_POPUP220SelectingPersona[];
extern char str_kT_POPUPCMNErrorEAMessenger[];

struct sVE1ADA30 {
    short delta;
    short index;
    void (*fn)(void*, cUIText*);
};

// Lobby/server message handler (cFEStateNet virtual slot 31) of the persona-selection screen (cFEStateSelectPersona):
// 217 shows the "selecting persona" popup, 260 an EA Messenger error popup; unknown ids go to cFEStateNet's handler
// func_001A8E40. Returns -1 when handled.
extern "C" int func_001ADA30(cFEStateSelectPersona* self, int msgId, int data)
{
    switch (msgId) {
    case 217: {
        void* popup = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)popup + 0xBC) = 0;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220SelectingPersona);
        *(int*)((char*)popup + 0xC0) = 3;
        *(int*)((char*)popup + 0x150) = 2071;
        *(int*)((char*)popup + 0x164) = 0;
        func_001A9BC0(((cFEStateNet*)self), popup);
        break;
    }
    case 260: {
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80();
        }
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            func_002613C8(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800(LobbyMan_mLobbyMan);
        }
        func_00261408(BuddyMan_mBuddyMan);
        func_0025B800(LobbyMan_mLobbyMan);
        void* popup = func_001A9C50(((cFEStateNet*)self));
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUPCMNErrorEAMessenger);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x164) = 0;
        *(int*)((char*)popup + 0x150) = 2073;
        func_001A9BC0(((cFEStateNet*)self), popup);
    }
    case 214:
        func_0039F190((char*)self->mpEngine + 0x18, 1);
    case 216:
        {
            sVE1ADA30* vt = (sVE1ADA30*)self->mVtbl;
            vt[18].fn((char*)self + vt[18].delta, cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_PersonaMenu)));
        }
        break;
    case 258:
        func_001ADC30(self, data);
        break;
    case 273:
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), (int)self->mpEngine));
        break;
    default:
        return (int)func_001A8E40_3(((cFEStateNet*)self), msgId, data);
    case 225:
        break;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ADC30);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateCreateDuplicate_cFEStateCreateDuplicate(cFEStateCreateDuplicate* self, int a1, int a2);
void func_001C57C0(void* self, int a1, int a2);
// PORT: unit declares func_001A8E40 with one arg; the real handler takes (self, id, msg)
extern "C" void* func_001A8E40_1ADC30(void* self, int id, void* msg) __asm__("func_001A8E40");
extern char str_kT_POPUP220PersonaNameLength[];
extern char str_kT_BTNOK[];
extern char str_kT_POPUPCMNProfanity[];
extern char str_cFEStateCreateDuplicate[];
extern char str_kT_POPUPCMNProfanityPersona[];

// PORT: the message pointer arrives as int
extern "C" void func_001ADC30(cFEStateSelectPersona* self, int msgPtr)
{
    void* msg = (void*)msgPtr;
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x6475706C:
        func_001A8A40(self, cFEStateCreateDuplicate_cFEStateCreateDuplicate((cFEStateCreateDuplicate*)cMemMan_alloc(1760, str_cFEStateCreateDuplicate, 0, 0), (int)self->mpEngine, 0));
        break;
    case 0x696E616D:
        {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 1;
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220PersonaNameLength);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)popup + 0x150) = 2072;
            func_001A9BC0((cFEStateNet*)self, popup);
        }
        break;
    case 0x66696C74:
        {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 1;
            char* sub = (char*)popup + 0xBC;
            if (self->mbExistingPersona == 0) {
                *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUPCMNProfanityPersona);
                *(int*)((char*)popup + 0xC0) = 3;
                *(int*)((char*)popup + 0x150) = 2072;
            } else {
                *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUPCMNProfanity);
                *(int*)((char*)popup + 0xC0) = 3;
                *(int*)((char*)popup + 0x150) = 2109;
            }
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            func_001A9BC0((cFEStateNet*)self, popup);
        }
        break;
    default:
        func_001A8E40_1ADC30(self, *(int*)msg, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_cFEStateChallengePopup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_001A8500_DDD0(void* self, void* engine, void* owner) __asm__("func_001A8500");
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_setGameMode_DDD0(void* iface, int mode) __asm__("cBENewRaceInterface_setGameMode");
extern "C" int cBENewRaceInterface_setGameEvent_DDD0(void* iface, int ev) __asm__("cBENewRaceInterface_setGameEvent");
extern int D_004A4E88;
extern int D_004A11B8;
extern int D_00535C08[];
extern char D_00441268[];
extern void* cFEStateChallengePopup_vtbl[];

class cRace_DDD0 {
public:
    int pad[3];
    virtual int reset();
};

/**
 * Constructs the "send a challenge" popup state and resets the new-race interface for it.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with @p engine and @p owner, stores cFEStateChallengePopup_vtbl,
 * points mpEventOptions at D_00441268, sets mHashName 0x3F, clears mbChallengeSent, mpLBRankedGame and D_004A4E88. On
 * the new-race interface (cBE interface 0) it sets D_004A11B8 to 1, clears D_00535C08[0], sets game mode 0 and event
 * 0 and resets it; finally sets mbFirstCreate.
 *
 * @param engine  the UI engine (On Tour cUIState's pEngine)
 * @param owner  the parent state (On Tour pParentState)
 * @return @p self
 */
// Derived state constructor: sets the vtable and defaults, then resets the new-race interface (mode 0, event 0).
extern "C" void* cFEStateChallengePopup_cFEStateChallengePopup(cFEStateChallengePopup* self, void* engine, void* owner)
{
    func_001A8500_DDD0(self, engine, owner);
    self->mVtbl = cFEStateChallengePopup_vtbl;
    self->mpEventOptions = (int*)D_00441268;
    self->mHashName = 0x3F;
    self->mbChallengeSent = 0;
    self->mpLBRankedGame = 0;
    D_004A4E88 = 0;
    cRace_DDD0* race = (cRace_DDD0*)cBE_getInterface_Fv(cBE_getBE(), 0);
    D_004A11B8 = 1;
    D_00535C08[0] = 0;
    cBENewRaceInterface_setGameMode_DDD0(race, 0);
    cBENewRaceInterface_setGameEvent_DDD0(race, 0);
    race->reset();
    self->mbFirstCreate = 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" void func_001AF728(cFEStateChallengePopup* self, void* iface, void* screen);
extern "C" void func_001AF688(cFEStateChallengePopup* self);
extern "C" void func_001AF930(void* screen);
extern "C" void func_0025E6D0(void* self, const char* str);
extern "C" void func_001A9930(cFEStateNet* self);
// D_004A4E88 is 8 bytes (config/symbol_addrs.txt); its second word was the separate symbol D_004A4E8C.
extern int D_004A4E88_w[2] __asm__("D_004A4E88");
extern char str_240challenge_popup[];
extern char str_MenuChallenge[];

struct sVE1ADE88 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE1ADE88i {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Creates the "240challenge_popup" screen and fills it from the current race setup.
 *
 * Calls its slot 32 (func_001AEBF0) first. When the screen is created: plays it, sets the new-race interface's AI
 * count to D_004A4E88's second word and calls that interface's slot 1, fills the race texts (func_001AF728), resets
 * the challenge state (func_001AF688), asks LobbyMan about mOpponent (func_0025E6D0) and runs func_001AF930 on the
 * screen. Re-selects "MenuChallenge" row 6 except on the first creation (mbFirstCreate, then cleared). Opens the
 * disconnected popup (func_001A9930) when LobbyMan's mState is 0.
 */
extern "C" void cFEStateChallengePopup_onCreateScreen(cFEStateChallengePopup* self)
{
    sVE1ADE88* vt = (sVE1ADE88*)self->mVtbl;
    vt[32].fn((char*)self + vt[32].delta);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_240challenge_popup), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
        cBENewRaceInterface_setNumberAI(iface, D_004A4E88_w[1]);
        sVE1ADE88i* ivt = *(sVE1ADE88i**)(iface + 0xC);
        ivt[1].fn(iface + ivt[1].delta);
        func_001AF728(self, iface, self->mpScreen);
        func_001AF688(self);
        func_0025E6D0(LobbyMan_mLobbyMan, ((const char*)LobbyMan_mLobbyMan->mOpponent));
        func_001AF930(self->mpScreen);
        if (self->mbFirstCreate == 0) {
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_MenuChallenge)), 6);
        } else {
            self->mbFirstCreate = 0;
        }
    }
    if (LobbyMan_mLobbyMan->mState == 0) {
        func_001A9930((cFEStateNet*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onGainTransition);
#ifdef SKIP_ASM
class cFEStateChallengePopup_onGainTransition_cObj {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

/**
 * Prepares the challenge popup as it opens: calls its slot 32 (func_001AEBF0).
 *
 * @return 1 (the transition is done)
 */
extern "C" int cFEStateChallengePopup_onGainTransition(cFEStateChallengePopup* self)
{
    ((cFEStateChallengePopup_onGainTransition_cObj*)self)->v32();
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onGainFocus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cFEStateNet_onGainFocus(cFEStateNet* self, int a1);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern "C" void func_001AF098(cFEStateChallengePopup* self, int a1);
extern "C" void func_001AEC60(cFEStateChallengePopup* self, int a1);
extern signed char D_00535C12[];
extern char D_00441268[];
extern char D_00441278[];
extern char D_00441288[];
extern char D_00441298[];
extern char D_004412A8[];
extern char D_004412B8[];

/**
 * Refreshes the challenge options when the popup regains focus.
 *
 * Runs the cFEStateNet onGainFocus (cFEStateNet_onGainFocus), shows the network interface's bIsRankedGame in
 * "ListBoxRankedGame" (row ranked ^ 1), picks the event's allowed-options table for mpEventOptions by the event kind
 * D_00535C12 (1 D_00441278, 2 D_00441298, 3 D_00441288, 4 D_004412B8, 5 D_004412A8, else D_00441268), then updates
 * the option widgets for ranked play (func_001AF098) and locks them while mbChallengeSent (func_001AEC60). The
 * comment above it ("picks the mode title") does not match the code.
 *
 * @param pPrevState  the state that had focus before (On Tour pPrevState)
 */
// Refreshes the race-options screen and picks the mode title for the current game mode.
extern "C" void cFEStateChallengePopup_onGainFocus(cFEStateChallengePopup* self, int pPrevState)
{
    cFEStateNet_onGainFocus((cFEStateNet*)self, pPrevState);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    cUIListBox_setCurrentEntryByIndex(self->mpLBRankedGame, cBENetworkInterface_networkOpt.bIsRankedGame ^ 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    switch (D_00535C12[0]) {
    case 3:
        self->mpEventOptions = (int*)D_00441288;
        break;
    case 1:
        self->mpEventOptions = (int*)D_00441278;
        break;
    case 2:
        self->mpEventOptions = (int*)D_00441298;
        break;
    case 5:
        self->mpEventOptions = (int*)D_004412A8;
        break;
    case 4:
        self->mpEventOptions = (int*)D_004412B8;
        break;
    case 0:
    default:
        self->mpEventOptions = (int*)D_00441268;
        break;
    }
    func_001AF098(self, cBENetworkInterface_networkOpt.bIsRankedGame);
    func_001AEC60(self, self->mbChallengeSent);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the challenge popup: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateChallengePopup_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the challenge popup: reports events 8 and 9 as handled (0x100), every other as not (0x101).
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param key  the menu event (On Tour Event)
 * @return 0x100 for events 8 and 9, else 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateChallengePopup_onWidgetMenu(cFEStateChallengePopup* self, int pMenu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onWidgetCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern unsigned char D_004A4E88_E128[8] __asm__("D_004A4E88");
extern char str_TextTitle[];
extern char str_TextPlayerName[];
extern char str_TextSelectPeakEvent[];
extern char str_Textselectevent[];
extern char str_PeakLabel[];
extern char str_kT_OVRCMNPeakColon[];
extern char str_CourseLabel[];
extern char str_kT_OVRCMNCourseColon[];
extern char str_EventLabel[];
extern char str_kT_OVRCMNEventColon[];
extern char str_TextRankedGame[];
extern char str_TextSubmit[];
extern char str_kT_CMNSubmit[];
extern char str_TextCancel[];
extern char str_kT_BTNCancel[];
extern char str_ListBoxRankedGame[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char str_ListBoxCPURiders[];
extern char str_kT_CMNHUDNone[];
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];
extern char D_004A1510[];
extern char str_ListBoxPowerUps[];
extern char str_ListBoxMultipliers[];
extern char str_ListBoxPointicons[];
extern char str_MenuChallenge[];

class cFEVObj_001AE128 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24(int on);
    virtual void v25(int on);
};

static inline int IsHash1AE128(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Sets up the challenge popup's widgets as the screen creates them.
 *
 * "TextTitle", "TextSelectPeakEvent" and "TextRankedGame" are made visible (slot 9, setVisible); "TextPlayerName"
 * shows LobbyMan's mOpponent; "Textselectevent" gets id 7 (setUseStaticColour(1)); the peak, course and event labels
 * get their "kT_OVRCMN..." texts. "TextSubmit" ("kT_CMNSubmit") gets id 9 and "TextCancel" ("kT_BTNCancel") id 10.
 * The list boxes are filled and kept: "ListBoxRankedGame" (yes 1 / no 0) mpLBRankedGame, "ListBoxCPURiders" (none,
 * 1-4, set to D_004A4E88's fifth byte) mpLBCPURiders, "ListBoxPowerUps", "ListBoxMultipliers" and "ListBoxPointicons"
 * (yes 0 / no 1) in their members. "MenuChallenge" gets attribute bit 7.
 *
 * @param item  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateChallengePopup_onWidgetCreate(cFEStateChallengePopup* self, cUIText* item)
{
    cFEVObj_001AE128* obj = (cFEVObj_001AE128*)item;
    if (IsHash1AE128(*(int*)((char*)item + 0x38), str_TextTitle)) {
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_TextPlayerName)) {
        cUIText_setAsciiString(item, ((const char*)LobbyMan_mLobbyMan->mOpponent));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_TextSelectPeakEvent)) {
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_Textselectevent)) {
        *(int*)((char*)item + 0x18) = 7;
        obj->v25(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_PeakLabel)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(str_kT_OVRCMNPeakColon));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_CourseLabel)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(str_kT_OVRCMNCourseColon));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_EventLabel)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(str_kT_OVRCMNEventColon));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_TextRankedGame)) {
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_TextSubmit)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(str_kT_CMNSubmit));
        *(int*)((char*)item + 0x18) = 9;
        obj->v25(1);
        obj->v09(1);
        obj->v25(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_TextCancel)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(str_kT_BTNCancel));
        *(int*)((char*)item + 0x18) = 10;
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_ListBoxRankedGame)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNYes), 1);
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNNo), 0);
        obj->v09(1);
        obj->v24(1);
        self->mpLBRankedGame = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_ListBoxCPURiders)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNHUDNone), 0);
        cUIListBox_addEntryByAsciiString(item, D_004A14F8, 1);
        cUIListBox_addEntryByAsciiString(item, D_004A1500, 2);
        cUIListBox_addEntryByAsciiString(item, D_004A1508, 3);
        cUIListBox_addEntryByAsciiString(item, D_004A1510, 4);
        obj->v09(1);
        obj->v24(1);
        self->mpLBCPURiders = (cUIListBox*)item;
        cUIListBox_setCurrentEntryByIndex(item, D_004A4E88_E128[4]);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_ListBoxPowerUps)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNYes), 0);
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNNo), 1);
        obj->v09(1);
        obj->v24(1);
        self->mpLBPowerUps = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_ListBoxMultipliers)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNYes), 0);
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNNo), 1);
        obj->v09(1);
        obj->v24(1);
        self->mpLBMultipliers = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_ListBoxPointicons)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNYes), 0);
        cUIListBox_addEntryByStringID(item, GetHashValue32(str_kT_CMNNo), 1);
        obj->v09(1);
        obj->v24(1);
        self->mpLBPointIcons = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), str_MenuChallenge)) {
        *(int*)((char*)item + 0x14) |= 0x80;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AE680);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" int cUIListBox_getCurrentEntryData(void* a);
// D_00534B34 is a splat label inside cBENetworkInterface_networkOpt
extern int D_00534B34[];
extern char D_005308B8[];

struct sVE1AE680 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE1AE680i {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sOpts1AE680 {
    char pad[0x18];
    unsigned int f0 : 6;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int b8 : 1;
};

extern "C" void func_001AE680(cFEStateChallengePopup* self)
{
    if (self->mpScreen != 0) {
        char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
        D_00534B34[0] = cUIListBox_getCurrentEntryData(self->mpLBRankedGame) != 0;
        sVE1AE680* vt = *(sVE1AE680**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        int v = cUIListBox_getCurrentEntryData(self->mpLBMultipliers);
        sOpts1AE680* o = (sOpts1AE680*)D_005308B8;
        o->b6 = v;
        o->b8 = cUIListBox_getCurrentEntryData(self->mpLBPowerUps);
        o->b7 = cUIListBox_getCurrentEntryData(self->mpLBPointIcons);
        char* nr = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
        cBENewRaceInterface_setNumberAI(nr, cUIListBox_getCurrentEntryData(self->mpLBCPURiders));
        sVE1AE680i* nvt = *(sVE1AE680i**)(nr + 0xC);
        nvt[1].fn(nr + nvt[1].delta);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cBENewRaceInterface_setNumberAI_E7A0(void* iface, int n) __asm__("cBENewRaceInterface_setNumberAI");
extern "C" void func_001AE680_E7A0(void* self) __asm__("func_001AE680");
extern "C" void func_001AF568_E7A0(void* self) __asm__("func_001AF568");
extern "C" int cUIListBox_getCurrentEntryData_E7A0(void* item) __asm__("cUIListBox_getCurrentEntryData");
extern "C" void func_0025F4C0_E7A0(void* self, int a1, int a2) __asm__("func_0025F4C0");
extern "C" void func_0025F628_E7A0(void* self, int a1, int a2) __asm__("func_0025F628");
extern "C" void func_0039F600_E7A0(void* list) __asm__("func_0039F600");
extern "C" void* func_00186610_E7A0(void* mem, int a1, int a2) __asm__("func_00186610");
extern "C" void cUIStateStack_pushSpecial_E7A0(void* stack, void* state, int a2, int a3) __asm__("cUIStateStack_pushSpecial");
extern "C" void func_001AF098_E7A0(void* self, int on) __asm__("func_001AF098");
extern "C" void func_001AEC60_E7A0(void* self, int v) __asm__("func_001AEC60");
extern "C" void func_001AF208_E7A0(void* self, void* item, int on) __asm__("func_001AF208");
extern int cUITemplate_MAP_mPeak;
extern int cUITemplate_MAP_mMapStyle;
extern int D_004A2598;
extern int D_00534B34[];
extern char str_cFEStateEventSelect[];

struct sRaceOpts_E7A0 {
    char pad0[0x18];
    unsigned int low : 6;
    unsigned int bit6 : 1;
    unsigned int bit7 : 1;
    unsigned int bit8 : 1;
    unsigned int high : 23;
};
extern sRaceOpts_E7A0 D_005308B8_E7A0 __asm__("D_005308B8");

struct sIVE_E7A0 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline void commit_E7A0(char* iface)
{
    sIVE_E7A0* ivt = *(sIVE_E7A0**)(iface + 0xC);
    ivt[1].fn(iface + ivt[1].delta);
}


/**
 * Handles the challenge popup's widgets: event selection, sending or cancelling the challenge and the option boxes.
 *
 * A null widget is ignored. Event 5 by id: 7 saves the options (func_001AE680), sets cUITemplate_MAP_mPeak, clears
 * cUITemplate_MAP_mMapStyle / D_004A2598 and pushes a cFEStateEventSelect (mode 1); 9 (submit), unless mbChallengeSent, saves the
 * options, marks the challenge sent (func_001AF568), stores the ranked flag in the network options, sets the race
 * options (all on with no AI riders when ranked, else from the multiplier, power-up, point-icon and CPU-rider boxes)
 * and sends the challenge to mOpponent (func_0025F4C0); 10 cancels a sent challenge (func_0025F628(0, 1)) and pops
 * the popup (func_0039F600). Event 6 cancels (func_0025F628(0, mbChallengeSent)) and pops. Event 9 (a list box
 * changed) applies that box's value at once; events 1 and 2 call func_001AF208 with 1 / 0; event 7 is ignored; the
 * rest go to the cFEStateNet handler.
 *
 * @param pWidget  the widget (On Tour pWidget); null is ignored
 * @param msg  the widget event (On Tour Event)
 */
// Message handler for the race-options screen.
extern "C" void cFEStateChallengePopup_onWidgetEvent(cFEStateChallengePopup* self, void* pWidget, int msg)
{
    if (pWidget == 0) {
        return;
    }
    switch (msg) {
    case 5:
        switch (*(int*)((char*)pWidget + 0x18)) {
        case 7: {
            func_001AE680_E7A0((self));
            cUITemplate_MAP_mPeak = 1;
            cUITemplate_MAP_mMapStyle = 0;
            D_004A2598 = 0;
            char* popup = (char*)func_00186610_E7A0(cMemMan_alloc(828, str_cFEStateEventSelect, 0x100, 0), *(int*)(char**)&self->mpEngine, 1);
            *(int*)(popup + 0x1C) = (*(int*)(popup + 0x1C) & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial_E7A0((char*)self->mpEngine + 0x18, popup, 0, 0);
            break;
        }
        case 9: {
            if (self->mbChallengeSent) {
                break;
            }
            func_001AE680_E7A0((self));
            func_001AF568_E7A0((self));
            char* opts = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
            D_00534B34[0] = cUIListBox_getCurrentEntryData_E7A0(self->mpLBRankedGame) != 0;
            commit_E7A0(opts);
            if (cUIListBox_getCurrentEntryData_E7A0(self->mpLBRankedGame)) {
                D_005308B8_E7A0.bit6 = 1;
                D_005308B8_E7A0.bit7 = 1;
                D_005308B8_E7A0.bit8 = 1;
                char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
                cBENewRaceInterface_setNumberAI_E7A0(race, 0);
                commit_E7A0(race);
            } else {
                D_005308B8_E7A0.bit6 = cUIListBox_getCurrentEntryData_E7A0(self->mpLBMultipliers);
                D_005308B8_E7A0.bit8 = cUIListBox_getCurrentEntryData_E7A0(self->mpLBPowerUps);
                D_005308B8_E7A0.bit7 = cUIListBox_getCurrentEntryData_E7A0(self->mpLBPointIcons);
                char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
                cBENewRaceInterface_setNumberAI_E7A0(race, cUIListBox_getCurrentEntryData_E7A0(self->mpLBCPURiders));
                commit_E7A0(race);
            }
            func_0025F4C0_E7A0(LobbyMan_mLobbyMan, ((int)LobbyMan_mLobbyMan->mOpponent), self->mCurrentCtrl);
            break;
        }
        case 10:
            if (self->mbChallengeSent) {
                func_0025F628_E7A0(LobbyMan_mLobbyMan, 0, 1);
            }
            func_0039F600_E7A0((char*)self->mpEngine + 0x18);
            break;
        }
        break;
    case 6:
        func_0025F628_E7A0(LobbyMan_mLobbyMan, 0, self->mbChallengeSent);
        func_0039F600_E7A0((char*)self->mpEngine + 0x18);
        break;
    case 9:
        if (pWidget == (void*)self->mpLBRankedGame) {
            char* opts = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
            D_00534B34[0] = cUIListBox_getCurrentEntryData_E7A0(self->mpLBRankedGame) != 0;
            commit_E7A0(opts);
            func_001AF098_E7A0((self), cUIListBox_getCurrentEntryData_E7A0(self->mpLBRankedGame) != 0);
            func_001AEC60_E7A0((self), self->mbChallengeSent);
        } else if (pWidget == (void*)self->mpLBMultipliers) {
            D_005308B8_E7A0.bit6 = cUIListBox_getCurrentEntryData_E7A0(pWidget);
        } else if (pWidget == (void*)self->mpLBPowerUps) {
            D_005308B8_E7A0.bit8 = cUIListBox_getCurrentEntryData_E7A0(pWidget);
        } else if (pWidget == (void*)self->mpLBPointIcons) {
            D_005308B8_E7A0.bit7 = cUIListBox_getCurrentEntryData_E7A0(pWidget);
        } else if (pWidget == (void*)self->mpLBCPURiders) {
            char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
            cBENewRaceInterface_setNumberAI_E7A0(race, cUIListBox_getCurrentEntryData_E7A0(self->mpLBCPURiders));
            D_004A4E88_w[1] = cUIListBox_getCurrentEntryData_E7A0(self->mpLBCPURiders);
            commit_E7A0(race);
        }
        break;
    case 1:
        func_001AF208_E7A0((self), pWidget, 1);
        break;
    case 2:
        func_001AF208_E7A0((self), pWidget, 0);
        break;
    case 7:
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AEBF0);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

extern "C" void func_001AEBF0()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00262768(BuddyMan_mBuddyMan, 1, 7, ((int)LobbyMan_mLobbyMan->mOpponent), 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AEC60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int D_00534B34[];
extern char str_PairRankedGame[];
extern char str_PairNumCPURiders[];
extern char str_PairPowerUps[];
extern char str_PairMultipliers[];
extern char str_PairPointIcons[];
extern char str_Textselectevent[];

class cFEVObj_001AEC60 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

struct sVE_001AEC60 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001AEC60(cFEStateChallengePopup* self, int on)
{
    if (self->mpScreen == 0) {
        return;
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    int locked = D_00534B34[0];
    cFEVObj_001AEC60* o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_PairRankedGame));
    if (o != 0) {
        o->v08(on);
    }
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_PairNumCPURiders));
    if (o != 0) {
        ((cFEVObj_001AEC60*)self->mpLBCPURiders)->v08(on);
        o->v08(on);
        if (on == 0) {
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)o + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)o + vt[8].delta, (self->mpEventOptions)[0] == 0 || locked != 0);
        }
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)((cFEVObj_001AEC60*)self->mpLBCPURiders) + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)((cFEVObj_001AEC60*)self->mpLBCPURiders) + vt[8].delta, (self->mpEventOptions)[0] == 0 || locked != 0);
        }
        }
    }
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_PairPowerUps));
    if (o != 0) {
        o->v08(on);
        ((cFEVObj_001AEC60*)self->mpLBPowerUps)->v08(on);
        if (on == 0) {
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)((cFEVObj_001AEC60*)self->mpLBPowerUps) + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)((cFEVObj_001AEC60*)self->mpLBPowerUps) + vt[8].delta, (self->mpEventOptions)[1] == 0 || locked != 0);
        }
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)o + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)o + vt[8].delta, (self->mpEventOptions)[1] == 0 || locked != 0);
        }
        }
    }
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_PairMultipliers));
    if (o != 0) {
        o->v08(on);
        ((cFEVObj_001AEC60*)self->mpLBMultipliers)->v08(on);
        if (on == 0) {
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)((cFEVObj_001AEC60*)self->mpLBMultipliers) + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)((cFEVObj_001AEC60*)self->mpLBMultipliers) + vt[8].delta, (self->mpEventOptions)[3] == 0 || locked != 0);
        }
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)o + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)o + vt[8].delta, (self->mpEventOptions)[3] == 0 || locked != 0);
        }
        }
    }
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_PairPointIcons));
    if (o != 0) {
        o->v08(on);
        ((cFEVObj_001AEC60*)self->mpLBPointIcons)->v08(on);
        if (on == 0) {
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)((cFEVObj_001AEC60*)self->mpLBPointIcons) + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)((cFEVObj_001AEC60*)self->mpLBPointIcons) + vt[8].delta, (self->mpEventOptions)[2] == 0 || locked != 0);
        }
            {
            sVE_001AEC60* vt = *(sVE_001AEC60**)((char*)o + 8);
            void (**pf)(void*, int) = &vt[8].fn;
            (*pf)((char*)o + vt[8].delta, (self->mpEventOptions)[2] == 0 || locked != 0);
        }
        }
    }
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Textselectevent));
    if (o != 0) {
        o->v08(on);
    }
    ((cFEVObj_001AEC60*)self->mpLBRankedGame)->v08(on);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF098);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int D_005308D0[];
extern signed char D_00535C12[];
// D_004A4E88 is 8 bytes; the byte read is its second word's low byte (+4).
extern unsigned char D_004A4E88_AF098[8] __asm__("D_004A4E88");
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);

extern "C" void func_001AF098(cFEStateChallengePopup* self, int all)
{
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
    if (all) {
        cUIListBox_setCurrentEntryByIndex(self->mpLBPowerUps, 1);
        cUIListBox_setCurrentEntryByIndex(self->mpLBMultipliers, 1);
        cUIListBox_setCurrentEntryByIndex(self->mpLBPointIcons, 1);
        cUIListBox_setCurrentEntryByIndex(self->mpLBCPURiders, 0);
        return;
    }
    if (*(int*)((char*)self->mpEventOptions + 0x4) != 0) {
        cUIListBox_setCurrentEntryByIndex(self->mpLBPowerUps, (D_005308D0[0] >> 8) & 1);
    } else {
        cUIListBox_setCurrentEntryByIndex(self->mpLBPowerUps, 1);
    }
    if (*(int*)((char*)self->mpEventOptions + 0xC) != 0) {
        cUIListBox_setCurrentEntryByIndex(self->mpLBMultipliers, (D_005308D0[0] >> 6) & 1);
    } else {
        cUIListBox_setCurrentEntryByIndex(self->mpLBMultipliers, 1);
    }
    if (*(int*)((char*)self->mpEventOptions + 0x8) != 0) {
        int f = D_005308D0[0] >> 7;
        cUIListBox_setCurrentEntryByIndex(self->mpLBPointIcons, f & 1);
    } else {
        cUIListBox_setCurrentEntryByIndex(self->mpLBPointIcons, 1);
    }
    if (D_00535C12[0] != 0) {
        cBENewRaceInterface_setNumberAI(iface, 0);
        sVE1ADE88i* ivt = *(sVE1ADE88i**)(iface + 0xC);
        ivt[1].fn(iface + ivt[1].delta);
        cUIListBox_setCurrentEntryByIndex(self->mpLBCPURiders, 0);
    } else {
        cUIListBox_setCurrentEntryByIndex(self->mpLBCPURiders, D_004A4E88_AF098[4]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF208);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char str_ListBoxRankedGame[];
extern char str_ListBoxCPURiders[];
extern char str_ListBoxPowerUps[];
extern char str_ListBoxMultipliers[];
extern char str_ListBoxPointicons[];
extern char str_InformRight1[];
extern char str_InformLeft1[];
extern char str_InformRight2[];
extern char str_InformLeft2[];
extern char str_InformRight3[];
extern char str_InformLeft3[];
extern char str_InformRight4[];
extern char str_InformLeft4[];
extern char str_InformRight5[];
extern char str_InformLeft5[];

class cFEVObj_001AF208 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash1AF208(int id, char* name) { return id == GetHashValue32(name); }

static inline cFEVObj_001AF208* Get1AF208(void* self, char* name)
{
    return (cFEVObj_001AF208*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

extern "C" void func_001AF208(cFEStateChallengePopup* self, cUIText* item, int on)
{
    if (item == 0) {
        return;
    }
    cFEVObj_001AF208* a = 0;
    cFEVObj_001AF208* b = 0;
    if (IsHash1AF208(*(int*)((char*)item + 0x38), str_ListBoxRankedGame)) {
        a = Get1AF208(((cFEStateNet*)self), str_InformRight1);
        b = Get1AF208(((cFEStateNet*)self), str_InformLeft1);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), str_ListBoxCPURiders)) {
        a = Get1AF208(((cFEStateNet*)self), str_InformRight2);
        b = Get1AF208(((cFEStateNet*)self), str_InformLeft2);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), str_ListBoxPowerUps)) {
        a = Get1AF208(((cFEStateNet*)self), str_InformRight3);
        b = Get1AF208(((cFEStateNet*)self), str_InformLeft3);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), str_ListBoxMultipliers)) {
        a = Get1AF208(((cFEStateNet*)self), str_InformRight4);
        b = Get1AF208(((cFEStateNet*)self), str_InformLeft4);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), str_ListBoxPointicons)) {
        a = Get1AF208(((cFEStateNet*)self), str_InformRight5);
        b = Get1AF208(((cFEStateNet*)self), str_InformLeft5);
    }
    if (a != 0) {
        a->v09(on);
    }
    if (b != 0) {
        b->v09(on);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF3E0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_0039F698(void* p);

extern "C" int func_001AF3E0(cFEStateChallengePopup* self, void* popup, int event)
{
    if (event == 0xF || event == 20) {
        return func_0039F698((char*)self->mpEngine + 0x18);
    }
    return func_001A97B8(((cFEStateNet*)self), popup, event);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengePopup_onPopupEvent);
#ifdef SKIP_ASM
class cFEVObj_001AF428 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

/**
 * Hands a popup's event on the challenge popup to its slot 33 (func_001AF3E0) for popup id 0x835.
 *
 * Any other popup goes to the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateChallengePopup_onPopupEvent(cFEStateChallengePopup* self, void* pPopup, int Event)
{
    if (*(int*)((char*)pPopup + 0x18) == 2101) {
        ((cFEVObj_001AF428*)self)->v33(pPopup, Event);
    } else {
        func_001A97B8(((cFEVObj_001AF428*)self), pPopup, Event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF470);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001AF688(cFEStateChallengePopup* self);
extern "C" void func_001AB178(cFEStateNet* self, int err);
extern "C" void func_001AB108(cFEStateNet* self);
extern "C" void func_001AFA60(void* screen, char* msg);
extern "C" void func_001AFD88(void* self, void* a1);
extern const char str_cFEStateOnlineCharacterSelect[];

// Message handler: opens the next screen, shows error popups, or forwards to the base handler.
extern "C" void* func_001AF470(cFEStateChallengePopup* self, int msgId, int data)
{
    switch (msgId) {
    case 243:
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(1764, str_cFEStateOnlineCharacterSelect, 0x100, 0), (int)self->mpEngine));
        break;
    case 237: case 238: case 0xF0: case 241: case 242: case 268: case 274: case 275:
        func_001AF688(self);
        func_001AB178(((cFEStateNet*)self), msgId);
        break;
    case 272:
        func_001AB108(((cFEStateNet*)self));
        break;
    case 227:
        func_001AFA60(self->mpScreen, (char*)data);
        break;
    case 229:
        func_001AFD88(self->mpScreen, (void*)data);
        break;
    default:
        return func_001A8E40_3(((cFEStateNet*)self), msgId, data);
    case 239:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF568);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void func_001AEC60(cFEStateChallengePopup* self, int a1);
extern "C" void sprintf1(void* str, int a1, char* buf);
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern char str_staticTextSetchallenge[];
extern char str_kT_240WaitingChallenge[];
extern char str_TextSubmit[];
extern char str_MenuChallenge[];

struct sVE_001AF568 {
    short delta;
    short index;
    int (*fn)(void* self, int hash);
};

struct sVE_001AF568b {
    short delta;
    short index;
    void (*fn)(void* self, int on);
};

extern "C" void func_001AF568(cFEStateChallengePopup* self)
{
    char buf[0x400];
    self->mbChallengeSent = 1;
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_staticTextSetchallenge));
    if (text != 0) {
        char* name = LobbyMan_mLobbyMan->mOpponent;
        char* locale = (char*)SSXApp->FELocale;
        sVE_001AF568* vt = *(sVE_001AF568**)(locale + 4);
        char* obj = locale + vt[4].delta;
        sprintf1(buf, vt[4].fn(obj, GetHashValue32(str_kT_240WaitingChallenge)), name);
        cUIText_setUnicodeString(text, buf);
        char* submitText = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextSubmit));
        sVE_001AF568b* vt2 = *(sVE_001AF568b**)(submitText + 8);
        vt2[8].fn(submitText + vt2[8].delta, 1);
        cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_MenuChallenge)), 7);
        func_001AEC60(self, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF688);
#ifdef SKIP_ASM
class cUIObj_001AF688 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};

int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001AEC60(cFEStateChallengePopup* self, int a1);
extern char str_staticTextSetchallenge[];
extern char str_kT_241SetChallengeOptions[];
extern char str_TextSubmit[];

extern "C" void func_001AF688(cFEStateChallengePopup* self)
{
    self->mbChallengeSent = 0;
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_staticTextSetchallenge));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_241SetChallengeOptions));
        ((cUIObj_001AF688*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextSubmit)))->v08(0);
        func_001AEC60(self, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF728);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" int func_00144C98(void* track);
extern "C" const char* func_00145340(void* track);
extern "C" const char* func_00144BF0(void* track);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern signed char D_00535C12[];
extern char str_TextPeak[];
extern char str_TextEvent[];
extern char str_TextCourse[];
extern char D_004A1390[];
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];

class cRaceIface_001AF728 {
public:
    int f0;
    int f4;
    int f8;
    // vptr at 0xC
    virtual int v01();
};

class cFEVObj_001AF728 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

extern "C" void func_001AF728(cFEStateChallengePopup* self, void* track, void* screen)
{
    if (screen == 0) {
        return;
    }
    cUIText* uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextPeak));
    if (track == 0) {
        cUIText_setAsciiString(uiText, D_004A1390);
    } else {
        switch (func_00144C98(track)) {
        case 0:
            cUIText_setAsciiString(uiText, D_004A14F8);
            break;
        case 1:
            cUIText_setAsciiString(uiText, D_004A1500);
            break;
        case 2:
            cUIText_setAsciiString(uiText, D_004A1508);
            break;
        }
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextEvent));
    if (track == 0) {
        cUIText_setAsciiString(uiText, D_004A1390);
    } else {
        if (D_00535C12[0] != 0) {
            cRaceIface_001AF728* iface = (cRaceIface_001AF728*)cBE_getInterface_Fv(cBE_getBE(), 0);
            cBENewRaceInterface_setNumberAI(iface, 0);
            iface->v01();
            cUIListBox_setCurrentEntryByIndex(self->mpLBCPURiders, 0);
            ((cFEVObj_001AF728*)self->mpLBCPURiders)->v08(1);
        }
        cUIText_setAsciiString(uiText, func_00145340(track));
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextCourse));
    if (track == 0) {
        cUIText_setAsciiString(uiText, D_004A1390);
    } else {
        cUIText_setAsciiString(uiText, func_00144BF0(track));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF930);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_004A1390[];
extern char str_TextPlayerName[];
extern char str_TextPlayerRank[];
extern char str_TextNumGames[];
extern char str_TextPlayerWinsRatio[];
extern char str_TextPlayerDisconnects[];
extern char str_TextPlayerConQuality[];

static inline void Clear1AF930(void* screen, char* name)
{
    cUIText* uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
    if (uiText != 0) {
        cUIText_setAsciiString(uiText, D_004A1390);
    }
}

extern "C" void func_001AF930(void* screen)
{
    if (screen != 0) {
        Clear1AF930(screen, str_TextPlayerName);
        Clear1AF930(screen, str_TextPlayerRank);
        Clear1AF930(screen, str_TextNumGames);
        Clear1AF930(screen, str_TextPlayerWinsRatio);
        Clear1AF930(screen, str_TextPlayerDisconnects);
        Clear1AF930(screen, str_TextPlayerConQuality);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AFA60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: usprintf is a varargs formatter (buf, fmt, ...); the unit declares a 3-arg view
extern "C" void usprintf_1AFA60(char* buf, const char* fmt, ...) __asm__("usprintf");
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
// PORT: SN soft-float float->double helper called by name
extern "C" double fptodp(float f);
extern "C" void func_0025DD08(void* self, const char* str, int a2);
extern char str_TextPlayerName[];
extern char str_TextPlayerRank[];
extern char str_TextNumGames[];
extern char str_TextPlayerWinsRatio[];
extern char str_TextPlayerDisconnects[];
extern char str_kT_OVRCMNRanking[];
extern char str_kT_CMNGamesPlayed[];
extern char str_kT_CMNWinRatio[];
extern char str_pct_s_pct_d_pct_d_pct_02_2fpct_pct[];
extern char str_kT_CMNDisconnects[];
extern char str_pct_s_pct_d_b[];

struct sVEntry_001AFA60 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1AFA60(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001AFA60* vt = *(sVEntry_001AFA60**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

struct sBXString1AFA60 {
    char* str;
    sBXString1AFA60() {}
    sBXString1AFA60(const sBXString1AFA60& o);
};

struct sStats1AFA60 {
    int a;
    int b;
    int c;
    int d;
    int e;
};

extern "C" void func_001AFA60(void* screen, char* msg)
{
    sBXString1AFA60 nameStr;
    char buf[0x100];
    sStats1AFA60* st = (sStats1AFA60*)(msg + 0x18);
    cBXString_cBXString1(&nameStr, (char*)LobbyMan_mLobbyMan + 0x98);
    (*(sStats1AFA60*)&LobbyMan_mLobbyMan->mOpponentStats) = *(sStats1AFA60*)(msg + 0x18);
    func_0025DD08(LobbyMan_mLobbyMan, nameStr.str, *(int*)(msg + 0x14));
    cUIText* uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextPlayerName));
    if (uiText != 0) {
        cUIText_setAsciiString(uiText, nameStr.str);
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextPlayerRank));
    if (uiText != 0) {
        int name = Loc1AFA60(str_kT_OVRCMNRanking);
        usprintf_1AFA60(buf, str_pct_s_pct_d_b, name, *(int*)(msg + 0x18));
        cUIText_setUnicodeString(uiText, buf);
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextNumGames));
    if (uiText != 0) {
        int name = Loc1AFA60(str_kT_CMNGamesPlayed);
        usprintf_1AFA60(buf, str_pct_s_pct_d_b, name, *(int*)(msg + 0x20));
        cUIText_setUnicodeString(uiText, buf);
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextPlayerWinsRatio));
    if (uiText != 0) {
        int gamesPlayed = *(int*)(msg + 0x20);
        int wins = *(int*)(msg + 0x1C);
        float pct = 0.0f;
        if (wins < gamesPlayed) {
            pct = (float)wins / (float)gamesPlayed * 100.0f;
        } else if (wins != 0) {
            pct = 100.0f;
        }
        int name = Loc1AFA60(str_kT_CMNWinRatio);
        usprintf_1AFA60(buf, str_pct_s_pct_d_pct_d_pct_02_2fpct_pct, name, st->b, st->c, fptodp(pct));
        cUIText_setUnicodeString(uiText, buf);
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextPlayerDisconnects));
    if (uiText != 0) {
        int name = Loc1AFA60(str_kT_CMNDisconnects);
        usprintf_1AFA60(buf, str_pct_s_pct_d_b, name, st->d);
        cUIText_setUnicodeString(uiText, buf);
    }
    cBXString__cBXString(&nameStr, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001AFD88);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001AAD50(void* self, int a1, int a2);
extern char str_TextPlayerConQuality[];
extern char str_kT_CMNConnection[];

extern "C" void func_001AFD88(void* self, void* msg)
{
    cUIText* text = cUIScreen_getObjectByHashName(self, GetHashValue32(str_TextPlayerConQuality));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNConnection));
        func_001AAD50(self, 0, *(int*)((char*)msg + 0xC));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup);
#ifdef SKIP_ASM
extern void* cFEStateChallengeReceivedPopup_vtbl[];
extern int D_004A4E88;
extern "C" void* cBXString_cBXString2(void* self, const char* str);
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the "challenge received" popup state for one challenger.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with the engine and parent, stores
 * cFEStateChallengeReceivedPopup_vtbl, builds mszChallenger from @p name, clears mbAccepted, sets mHashName 0x40 and
 * clears D_004A4E88.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @param pParentState  the parent state (On Tour pParentState)
 * @param name  the challenger's name
 * @return @p self
 */
extern "C" void* cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup(cFEStateChallengeReceivedPopup* self, int pEngine, int pParentState, const char* name)
{
    func_001A8500_3(self, pEngine, pParentState);
    self->mVtbl = cFEStateChallengeReceivedPopup_vtbl;
    cBXString_cBXString2((char*)self + 0x6D0, name);
    self->mbAccepted = 0;
    self->mHashName = 0x40;
    D_004A4E88 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv_FE68(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" char* func_00263270(void* self, int id);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void MEM_fill(void* dst, int c, int n);
extern "C" int cBENewRaceInterface_setGameMode_FE68(void* iface, int mode) __asm__("cBENewRaceInterface_setGameMode");
extern "C" int cBENewRaceInterface_setGameEvent_FE68(void* iface, int ev) __asm__("cBENewRaceInterface_setGameEvent");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" void func_001B0850(cFEStateChallengeReceivedPopup* self, void* iface, void* screen);
extern "C" void func_0025E6D0(void* self, const char* str);
extern "C" void func_001AF930(void* screen);
extern int D_004A11B8;
extern int D_00535C08[];
extern int D_00534B34[];
extern char str_241challenge_received_popup[];

struct sVE_FE68 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sRaceSetup_FE68 {
    int f0;
    char* name;                     // 0x4
    int f8;
    int mode;                       // 0xC
    int event;                      // 0x10
    int f14;
    int f18;
    int f1C;
    int numAI;                      // 0x20
    int f24;
    int info[5];                    // 0x28
};

struct sRaceFlags_FE68 {
    char pad0[0x18];
    unsigned int b0 : 6;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int b8 : 1;
    unsigned int b9 : 23;
    char pad1C[0x4];
};
extern sRaceFlags_FE68 D_005308B8_FE68 __asm__("D_005308B8");

struct sInfo_FE68 {
    int v[5];
};

static inline void vcall_FE68(void* p)
{
    sVE_FE68* vt = *(sVE_FE68**)((char*)p + 0xC);
    ((int (*)(void*))vt[1].fn)((char*)p + vt[1].delta);
}

/**
 * Creates the "241challenge_received_popup" screen and loads the received challenge's race settings.
 *
 * Calls its slot 32, adds and plays the screen (returns if none), makes sure BuddyMan exists and is connected and
 * looks the challenge up by mszChallenger (func_00263270). Copies the name to LobbyMan (+0x98) and the record's stats
 * to LobbyMan's mOpponentStats before checking the record for null, so a missing challenge is read through a null
 * pointer. Then copies the record's name to LobbyMan (+0x8C), rebuilds the race-option bits (D_005308B8, cleared
 * first), the race flag D_00535C08[0], game mode, event and AI count on the new-race interface and the ranked flag on
 * the network interface (committing both), and fills the screen (func_001B0850, func_0025E6D0, func_001AF930). The
 * comment above it ("race-setup screen") is loose.
 */
// Creates the race-setup screen and applies the selected online race settings.
extern "C" void cFEStateChallengeReceivedPopup_onCreateScreen(cFEStateChallengeReceivedPopup* self)
{
    sVE_FE68* vt = (sVE_FE68*)self->mVtbl;
    vt[32].fn(((char*)self) + vt[32].delta);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((char*)self), GetHashValue32(str_241challenge_received_popup), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen == 0)
        return;
    cUIScreen_playFrame(screen, 0, 0);
    if (BuddyMan_mBuddyMan == 0)
        func_00260F80();
    if (func_00261460(BuddyMan_mBuddyMan) == 0)
        func_002613C8(BuddyMan_mBuddyMan);
    if (BuddyMan_mBuddyMan == 0)
        func_0025B800((char*)LobbyMan_mLobbyMan);
    char* name = ((char*)self) + 0x6D0;
    sRaceSetup_FE68* challenge = (sRaceSetup_FE68*)func_00263270(BuddyMan_mBuddyMan, *(int*)name);
    cBXString_operatorE((char*)LobbyMan_mLobbyMan + 0x98, name);
    (*(sInfo_FE68*)&LobbyMan_mLobbyMan->mOpponentStats) = *(sInfo_FE68*)challenge->info;
    if (challenge == 0)
        return;
    cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x8C, challenge->name);
    MEM_fill(&D_005308B8_FE68, 0, 0x20);
    D_005308B8_FE68.b6 = challenge->f18;
    D_005308B8_FE68.b7 = challenge->f24;
    D_005308B8_FE68.b8 = challenge->f1C;
    void* be = cBE_getInterface_Fv_FE68(cBE_getBE(), 0);
    D_00535C08[0] = challenge->f8;
    D_004A11B8 = 1;
    cBENewRaceInterface_setGameMode_FE68(be, challenge->mode);
    cBENewRaceInterface_setGameEvent_FE68(be, challenge->event);
    cBENewRaceInterface_setNumberAI(be, challenge->numAI);
    vcall_FE68(be);
    void* net = cBE_getInterface_Fv_FE68(SSXApp->BE, 7);
    D_00534B34[0] = challenge->f14;
    vcall_FE68(net);
    func_001B0850(self, be, self->mpScreen);
    func_0025E6D0((char*)LobbyMan_mLobbyMan, LobbyMan_mLobbyMan->mOpponent);
    func_001AF930(self->mpScreen);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B00B8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

extern "C" void func_001B00B8(cFEStateChallengeReceivedPopup* self)
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00262768(BuddyMan_mBuddyMan, 1, 8, (int)self->mszChallenger, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the challenge-received popup: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateChallengeReceivedPopup_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_onWidgetCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int D_00535C04[];
extern int D_005308D0[];
extern char str_pct_d_b[];
extern char str_True[];
extern char str_TextTitle[];
extern char str_TextAccept[];
extern char str_kT_BTNAccept[];
extern char str_TextDecline[];
extern char str_TextPlayerName[];
extern char str_TextSelectPeakEvent[];
extern char str_PeakLabel[];
extern char str_kT_OVRCMNPeakColon[];
extern char str_CourseLabel[];
extern char str_kT_OVRCMNCourseColon[];
extern char str_EventLabel[];
extern char str_kT_OVRCMNEventColon[];
extern char str_TextRankedGame[];
extern char str_TextRankedGameResult[];
extern char str_TextCPURidersResult[];
extern char str_kT_CMNHUDNone[];
extern char str_TextPowerUpsResult[];
extern char str_TextMultipliersResult[];
extern char str_TextPointiconsResult[];
extern char str_TextEventResult[];
extern char str_TextCourseResult[];
extern char str_MenuChallenge[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

class cFEVObj_001B0150 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash_001B0150(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Sets up the challenge-received widgets as the screen creates them.
 *
 * "TextAccept" ("kT_BTNAccept") gets id 0xB, "TextDecline" id 0xC, "TextSelectPeakEvent" id 7; "TextPlayerName" shows
 * mOpponent; the peak, course and event labels get their texts. The result texts: "TextRankedGameResult" always shows
 * "True"; "TextCPURidersResult" the AI count (D_00535C04, "%d" or "kT_CMNHUDNone"); "TextMultipliers Result" and
 * "TextPointiconsResult" No / Yes from bits 6 / 7 of D_005308D0. "TextPowerUpsResult" gets No / Yes from bit 8 and is
 * then overwritten with "True". The last branches for "TextRankedGameResult" and "TextCPURidersResult" can never run
 * (they are matched earlier). "MenuChallenge" gets attribute bit 7.
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateChallengeReceivedPopup_onWidgetCreate(cFEStateChallengeReceivedPopup* self, cFEVObj_001B0150* obj)
{
    char buf[16];
    char* widget = (char*)obj;
    cUIText* uiText = (cUIText*)obj;
    if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextTitle)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextAccept)) {
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_BTNAccept));
        *(int*)(widget + 0x18) = 11;
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextDecline)) {
        *(int*)(widget + 0x18) = 0xC;
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextPlayerName)) {
        cUIText_setAsciiString(uiText, ((const char*)LobbyMan_mLobbyMan->mOpponent));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextSelectPeakEvent)) {
        *(int*)(widget + 0x18) = 7;
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_PeakLabel)) {
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNPeakColon));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_CourseLabel)) {
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNCourseColon));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_EventLabel)) {
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNEventColon));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextRankedGame)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextRankedGameResult)) {
        cUIText_setAsciiString(uiText, str_True);
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextCPURidersResult)) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        int n = D_00535C04[0];
        if (n == 0) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNHUDNone));
        } else {
            sprintf(buf, str_pct_d_b, n);
            cUIText_setAsciiString(uiText, buf);
        }
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextPowerUpsResult)) {
        if ((D_005308D0[0] >> 8) & 1) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNNo));
        } else {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNYes));
        }
        cUIText_setAsciiString(uiText, str_True);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextMultipliersResult)) {
        if ((D_005308D0[0] >> 6) & 1) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNNo));
        } else {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNYes));
        }
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextPointiconsResult)) {
        int pointIconsBit = D_005308D0[0] >> 7;
        if (pointIconsBit & 1) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNNo));
        } else {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNYes));
        }
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextEventResult)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextCourseResult)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextRankedGameResult)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_TextCPURidersResult)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(widget + 0x38), str_MenuChallenge)) {
        *(int*)(widget + 0x14) |= 0x80;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001B0C08(cFEStateChallengeReceivedPopup* self);
extern "C" void func_001B0E10(cFEStateChallengeReceivedPopup* self);
extern "C" void func_0025FB58(void* self, int a1, int a2);

/**
 * Handles the challenge-received widgets: accept or decline the challenge.
 *
 * A null widget is ignored. Event 5 on id 0xB, while mbAccepted is 0, accepts (func_001B0C08) and tells LobbyMan
 * (func_0025FB58 with mOpponent and mCurrentCtrl); on id 0xC declines (func_001B0E10). Event 7 is ignored; the rest
 * go to the cFEStateNet handler.
 *
 * @param obj  the widget (On Tour pWidget); null is ignored
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cFEStateChallengeReceivedPopup_onWidgetEvent(cFEStateChallengeReceivedPopup* self, void* obj, int msg)
{
    if (obj == 0) {
        return;
    }
    switch (msg) {
    case 5:
        switch (*(int*)((char*)obj + 0x18)) {
        case 11:
            if (self->mbAccepted == 0) {
                func_001B0C08(self);
                func_0025FB58(LobbyMan_mLobbyMan, ((int)LobbyMan_mLobbyMan->mOpponent), self->mCurrentCtrl);
            }
            break;
        case 0xC:
            func_001B0E10(self);
            break;
        }
        break;
    case 7:
        break;
    default:
        // PORT: the unit declares cFEStateNet_onWidgetEvent with an int a1; obj is passed through as int
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)obj, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_onPopupEvent);
#ifdef SKIP_ASM
class cFEVObj_001B05E0 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

/**
 * Hands a popup's event on the challenge-received popup to its slot 33 for popup id 0x835.
 *
 * Any other popup goes to the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateChallengeReceivedPopup_onPopupEvent(cFEStateChallengeReceivedPopup* self, void* pPopup, int Event)
{
    if (*(int*)((char*)pPopup + 0x18) == 2101) {
        ((cFEVObj_001B05E0*)self)->v33(pPopup, Event);
    } else {
        func_001A97B8(((cFEVObj_001B05E0*)self), pPopup, Event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0628);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_0039F698(void* p);

extern "C" int func_001B0628(cFEStateChallengeReceivedPopup* self, void* popup, int event)
{
    if (event == 0xF || event == 20) {
        return func_0039F698((char*)self->mpEngine + 0x18);
    }
    return func_001A97B8(((cFEStateNet*)self), popup, event);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_onGainTransition);
#ifdef SKIP_ASM
class cFEVObj_001B0670 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001B0850(cFEStateChallengeReceivedPopup* self, void* iface, void* screen);

/**
 * Prepares the challenge-received popup as it opens: calls its slot 32 and, the first time, fills the screen.
 *
 * The fill is func_001B0850 with the new-race interface and mpScreen.
 *
 * @param bFirstTime  nonzero the first time the state opens (On Tour bFirstTime)
 * @return 1 (the transition is done)
 */
extern "C" int cFEStateChallengeReceivedPopup_onGainTransition(cFEStateChallengeReceivedPopup* self, int bFirstTime)
{
    ((cFEVObj_001B0670*)self)->v32();
    if (bFirstTime != 0) {
        func_001B0850(self, cBE_getInterface_Fv(cBE_getBE(), 0), self->mpScreen);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChallengeReceivedPopup_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the challenge-received popup: reports events 8 and 9 as handled (0x100), every other as not (0x101).
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param key  the menu event (On Tour Event)
 * @return 0x100 for events 8 and 9, else 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateChallengeReceivedPopup_onWidgetMenu(cFEStateChallengeReceivedPopup* self, int pMenu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0708);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001B0C08(cFEStateChallengeReceivedPopup* self);
extern "C" void func_001AB178(cFEStateNet* self, int err);
extern "C" void func_001AB108(cFEStateNet* self);
extern "C" void func_001AFA60(void* screen, char* msg);
extern "C" void func_001AFD88(void* self, void* a1);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00263018(void* self, void* a1);
extern const char str_cFEStateOnlineCharacterSelect[];

// Message handler: starts the memory-card task, shows error popups, or forwards to the base handler.
extern "C" void* func_001B0708(cFEStateChallengeReceivedPopup* self, int msgId, int data)
{
    switch (msgId) {
    case 239:
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80();
        }
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            func_002613C8(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800(LobbyMan_mLobbyMan);
        }
        func_00263018(BuddyMan_mBuddyMan, self->mszChallenger);
        break;
    case 272:
        func_001AB108((cFEStateNet*)self);
        break;
    case 237: case 238: case 0xF0: case 241: case 268: case 274: case 275:
        func_001B0C08(self);
        func_001AB178((cFEStateNet*)self, msgId);
        break;
    case 243:
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(1764, str_cFEStateOnlineCharacterSelect, 0x100, 0), (int)self->mpEngine));
        break;
    case 227:
        func_001AFA60(self->mpScreen, (char*)data);
        break;
    case 229:
        func_001AFD88(self->mpScreen, (void*)data);
        break;
    default:
        return func_001A8E40_3(self, msgId, data);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0850);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" const char* func_00145340(void* track);
extern "C" const char* func_00144BF0(void* track);
extern int D_00534B34[];
extern int D_005308D0[];
extern int D_00535C04[];
extern char D_004A1390[];
extern char str_pct_d_b[];
extern char str_TextEventResult[];
extern char str_TextCourseResult[];
extern char str_TextRankedGameResult[];
extern char str_TextPowerupsResult[];
extern char str_TextMultipliersResult[];
extern char str_TextPointiconsResult[];
extern char str_TextCPURidersResult[];
extern char str_kT_CMNHUDNone[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

extern "C" void func_001B0850(cFEStateChallengeReceivedPopup* self, void* iface, void* screen)
{
    char buf[16];
    if (screen == 0) {
        return;
    }
    cUIText* uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextEventResult));
    if (uiText != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(uiText, D_004A1390);
        } else {
            cUIText_setAsciiString(uiText, func_00145340(iface));
        }
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextCourseResult));
    if (uiText != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(uiText, D_004A1390);
        } else {
            cUIText_setAsciiString(uiText, func_00144BF0(iface));
        }
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextRankedGameResult));
    if (uiText != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(uiText, D_004A1390);
        } else {
            cBE_getInterface_Fv(SSXApp->BE, 7);
            if (D_00534B34[0] != 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNYes));
            } else {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNNo));
            }
        }
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextPowerupsResult));
    if (uiText != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(uiText, D_004A1390);
        } else {
            if ((D_005308D0[0] >> 8) & 1) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNNo));
            } else {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNYes));
            }
        }
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextMultipliersResult));
    if (uiText != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(uiText, D_004A1390);
        } else {
            if ((D_005308D0[0] >> 6) & 1) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNNo));
            } else {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNYes));
            }
        }
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextPointiconsResult));
    if (uiText != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(uiText, D_004A1390);
        } else {
            int f = D_005308D0[0] >> 7;
            if (f & 1) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNNo));
            } else {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNYes));
            }
        }
    }
    uiText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_TextCPURidersResult));
    if (uiText != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(uiText, D_004A1390);
        } else {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            int n = D_00535C04[0];
            if (n <= 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_CMNHUDNone));
            } else {
                sprintf(buf, str_pct_d_b, n);
                cUIText_setAsciiString(uiText, buf);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0C08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_TextDecline[];
extern char str_MenuChallenge[];
extern char str_staticTextSetchallenge[];
extern char str_kT_OVRCMNWaitPlayer[];
extern char str_TextAccept[];

class cFEVObj_001B0C08 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08(int on);
    virtual void v09(int on);
};

extern "C" void func_001B0C08(cFEStateChallengeReceivedPopup* self)
{
    self->mbAccepted = 1;
    if (self->mpScreen != 0) {
        cFEVObj_001B0C08* o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextDecline));
        if (o != 0) {
            o->v07(1);
        }
        cUIText* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_MenuChallenge));
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, 1);
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_staticTextSetchallenge));
        if (o != 0) {
            o->v09(1);
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNWaitPlayer));
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextAccept));
        if (o != 0) {
            o->v08(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0D30);
#ifdef SKIP_ASM
extern char str_kT_BTNDecline[];

extern "C" void func_001B0D30(cFEStateChallengeReceivedPopup* self)
{
    self->mbAccepted = 0;
    if (self->mpScreen != 0) {
        cFEVObj_001B0C08* o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextAccept));
        if (o != 0) {
            o->v08(0);
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_staticTextSetchallenge));
        if (o != 0) {
            o->v09(0);
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextDecline));
        if (o != 0) {
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_BTNDecline));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0E10);
#ifdef SKIP_ASM
extern "C" void func_001B0C08(cFEStateChallengeReceivedPopup* self);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_0025F740(void* self, int a1);
extern "C" void func_00263018(void* self, void* a1);
extern "C" void* func_002591D0();
extern "C" void func_00263BA8(void* p, int a1);

static inline void* GetSession_001B0E10()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

struct sVE1B0E10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001B0E10(cFEStateChallengeReceivedPopup* self)
{
    func_001B0C08(self);
    if (self->mbAccepted != 0) {
        func_0025F740(LobbyMan_mLobbyMan, 0);
        func_00263018(GetSession_001B0E10(), LobbyMan_mLobbyMan->mOpponent);
        func_00263BA8(func_002591D0(), 2);
    } else {
        func_00263018(GetSession_001B0E10(), LobbyMan_mLobbyMan->mOpponent);
    }
    char* obj = (char*)self->mpParentState;
    sVE1B0E10* vt = *(sVE1B0E10**)(obj + 0x8);
    vt[17].fn(obj + vt[17].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0F28);
#ifdef SKIP_ASM
extern signed char D_00440F68[];

extern "C" int func_001B0F28(int c)
{
    int i;
    for (i = 0; i < 10; i++) {
        if (c == D_00440F68[i]) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect);
#ifdef SKIP_ASM
extern void* cFEStateOnlineCharacterSelect_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the online rider-select state.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with no parent, stores cFEStateOnlineCharacterSelect_vtbl, sets
 * mbConfirmed (input blocked until the screen opens), mHashName 0x41 and clears miConfirmCtrl.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mVtbl = cFEStateOnlineCharacterSelect_vtbl;
    self->mbConfirmed = 1;
    self->mHashName = 65;
    self->miConfirmCtrl = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001B1B98(cFEStateOnlineCharacterSelect* self, int index, int a2, int a3);
extern char str_245online_char_select[];

/**
 * Creates the "245online_char_select" screen: clears miCharID, adds and plays the screen and resets all 30 rows.
 *
 * Each row i is reset with func_001B1B98(self, i, 1, 0).
 */
extern "C" void cFEStateOnlineCharacterSelect_onCreateScreen(cFEStateOnlineCharacterSelect* self)
{
    int i;
    self->miCharID = 0;
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_245online_char_select), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    for (i = 0; i < 30; i++) {
        func_001B1B98(self, i, 1, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the online rider-select state: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateOnlineCharacterSelect_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_onGainTransition);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cUIListBox_getCurrentEntryData(void* a);
extern "C" void func_001B1B10(cFEStateOnlineCharacterSelect* self, int sel, int flag);
extern "C" void func_001B1760(cFEStateOnlineCharacterSelect* self, int index);
extern "C" void func_001B1808(cFEStateOnlineCharacterSelect* self);

/**
 * Prepares the rider-select screen the first time it opens: shows the current pick and unlocks input.
 *
 * Shows the rider picked in "LBChallengerSelect" (func_001B1B10(.., 0)); when the network options' word at +0x32C is
 * not negative, also applies and shows that rider (func_001B1760, func_001B1B10(.., 1)); then func_001B1808 and
 * clears mbConfirmed.
 *
 * @param bFirstTime  nonzero the first time the state opens (On Tour bFirstTime)
 * @return 0
 */
extern "C" int cFEStateOnlineCharacterSelect_onGainTransition(cFEStateOnlineCharacterSelect* self, int bFirstTime)
{
    if (bFirstTime != 0) {
        func_001B1B10(self, cUIListBox_getCurrentEntryData(self->mpLBChallengerSelect), 0);
        cBE_getInterface_Fv(SSXApp->BE, 7);
        char* net = ((char*)&cBENetworkInterface_networkOpt);
        if (*(int*)(net + 0x32C) >= 0) {
            func_001B1760(self, *(int*)(net + 0x32C));
            func_001B1B10(self, *(int*)(net + 0x32C), 1);
        }
        func_001B1808(self);
        self->mbConfirmed = 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern "C" void* func_0014EEC8(void* self, int player, int index);
extern "C" int func_001B0F28(int c);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern signed char D_00440F68[];
extern char str_LBChallengerSelect[];
extern char str_TextOpponentSelect[];
extern char D_004A1390[];

struct sVec4_001B10F8 {
    float x, y, z, w;
    sVec4_001B10F8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

class cFEVObj_001B10F8 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sVec4_001B10F8& c);
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24(int on);
};

static inline int IsHash_001B10F8(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Sets up the rider-select widgets as the screen creates them.
 *
 * "LBChallengerSelect" loses attribute bit 0, gets the ten riders' names (func_0014EEC8 on the character interface
 * for each id in D_00440F68), becomes visible, is preset to the player's current rider (stored in miCharID) and is
 * kept in mpLBChallengerSelect (then slot 24 with 1). "TextOpponentSelect" gets the D_004A1390 text and colour (1, 0,
 * 0, 0).
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateOnlineCharacterSelect_onWidgetCreate(cFEStateOnlineCharacterSelect* self, cUIText* text)
{
    cFEVObj_001B10F8* box = (cFEVObj_001B10F8*)text;
    if (IsHash_001B10F8(*(int*)((char*)text + 0x38), str_LBChallengerSelect)) {
        *(int*)((char*)text + 0x14) &= ~1;
        int i;
        for (i = 0; i < 10; i++) {
            cUIListBox_addEntryByAsciiString(text, (const char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), D_00440F68[i], 0), D_00440F68[i]);
        }
        *(int*)((char*)text + 0x14) &= ~1;
        box->v09(1);
        int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(SSXApp->BE, 1), 0);
        self->miCharID = id;
        cUIListBox_setCurrentEntryByIndex(text, (unsigned char)func_001B0F28(id));
        self->mpLBChallengerSelect = (cUIListBox*)text;
        box->v24(1);
    } else if (IsHash_001B10F8(*(int*)((char*)text + 0x38), str_TextOpponentSelect)) {
        cUIText_setAsciiString(text, D_004A1390);
        box->v11(sVec4_001B10F8(1.0f, 0.0f, 0.0f, 0.0f));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001474A8(void* iface, int a1, int a2);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001B1A08(cFEStateOnlineCharacterSelect* self, int on, int a2);
extern "C" void func_001B1B10(cFEStateOnlineCharacterSelect* self, int sel, int flag);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_00264F88(void* self, int a1);
extern "C" void func_00265018(void* self, int a1);
extern "C" void func_00265140(void* self);
extern "C" int cUIListBox_getCurrentEntryData(void* a);
extern void* D_004A33CC;
extern char str_LBChallengerSelect[];
extern char str_kT_POPUPCMNCancelChallenge[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

struct sVE1B1280 {
    short delta;
    short index;
    void (*fn)(void*);
};

class cFEVObj_001B1280 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

static inline int IsHash1B1280(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Handles the rider-select widgets: confirm the pick, back out or cancel, and preview a changed pick.
 *
 * A null widget is ignored. Event 5, unless already confirmed or without the online game D_004A33CC: resets the
 * player interface (func_001474A8, then its slot 1) when the pick differs from miCharID, sends the pick
 * (func_00265018), records mCurrentCtrl in miConfirmCtrl, makes the widget static, locks the screen (func_001B1A08(1,
 * 0)) and sets mbConfirmed. Event 6 asks "kT_POPUPCMNCancelChallenge" (yes / no, id 0x838) before a pick is
 * confirmed, else withdraws it (func_00265140), unlocks and clears mbConfirmed. Event 9 on "LBChallengerSelect"
 * previews the pick (func_00264F88, func_001B1B10) and then also falls through to the cFEStateNet handler like every
 * other event.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget); null is ignored
 * @param msg  the widget event (On Tour Event)
 */
// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void cFEStateOnlineCharacterSelect_onWidgetEvent(cFEStateOnlineCharacterSelect* self, int pWidget, unsigned int msg)
{
    char* obj = (char*)pWidget;
    if (obj == 0) {
        return;
    }
    switch (msg) {
    case 5:
        if (self->mbConfirmed != 0) {
            return;
        }
        if (D_004A33CC == 0) {
            return;
        }
        if (self->miCharID != cUIListBox_getCurrentEntryData(self->mpLBChallengerSelect)) {
            char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 1);
            func_001474A8(iface, 0, 0);
            sVE1B1280* vt = *(sVE1B1280**)(iface + 0xC);
            vt[1].fn(iface + vt[1].delta);
        }
        {
            void* mgr = D_004A33CC;
            func_00265018(mgr, cUIListBox_getCurrentEntryData(self->mpLBChallengerSelect));
        }
        self->miConfirmCtrl = self->mCurrentCtrl;
        ((cFEVObj_001B1280*)obj)->v08(1);
        func_001B1A08(self, 1, 0);
        self->mbConfirmed = 1;
        break;
    case 6:
        if (self->mbConfirmed == 0) {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 2;
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUPCMNCancelChallenge);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            *(int*)((char*)popup + 0x150) = 2104;
            *(int*)((char*)popup + 0x168) = 1;
            func_001A9BC0((cFEStateNet*)self, popup);
        } else {
            func_00265140(D_004A33CC);
            ((cFEVObj_001B1280*)obj)->v08(0);
            func_001B1A08(self, 0, 0);
            self->mbConfirmed = 0;
        }
        break;
    case 9:
        if (IsHash1B1280(*(int*)(obj + 0x38), str_LBChallengerSelect)) {
            void* mgr = D_004A33CC;
            func_00264F88(mgr, cUIListBox_getCurrentEntryData(self->mpLBChallengerSelect));
            func_001B1B10(self, cUIListBox_getCurrentEntryData(self->mpLBChallengerSelect), 0);
        }
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B14D8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP245SelectCharacter[];
extern char str_kT_HELP245SelectCharacterMessage[];
extern char str_kT_HELP245ChangeCharacter[];
extern char str_kT_HELP245SelectCharacterButton[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001B14D8(cFEStateOnlineCharacterSelect* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP245SelectCharacter);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP245SelectCharacterMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELP245ChangeCharacter), 4);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELP245SelectCharacterButton), 0);
    func_001DA510(popup, 2, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_onInputBegin);
#ifdef SKIP_ASM
class cFEVObj_001B1590 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual bool v06();
};

extern void* D_004A33CC;

/**
 * Filters input on the rider-select screen: once the pick is confirmed (or with no online game) only "previous"
 * passes.
 *
 * @param obj  the input (On Tour cUIInput* pInput)
 * @return 1 to swallow the input (confirmed or no D_004A33CC, and not isPrevious, slot 6), else 0
 */
extern "C" bool cFEStateOnlineCharacterSelect_onInputBegin(cFEStateOnlineCharacterSelect* self, cFEVObj_001B1590* obj)
{
    if (self->mbConfirmed != 0 || D_004A33CC == 0) {
        return !obj->v06();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B15D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct sFEObj_001B1F88;
extern "C" sFEObj_001B1F88* cFEStateChatRoom_cFEStateChatRoom(sFEObj_001B1F88* mem, int a1);
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cFEStateNet* mem, int a1);
extern "C" void* func_00192D00(void* mem, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_0025DA90(void* self);
extern "C" void func_001B1760(cFEStateOnlineCharacterSelect* self, int index);
extern "C" void func_001B1B10(cFEStateOnlineCharacterSelect* self, int sel, int flag);
extern "C" void func_001B1A08(cFEStateOnlineCharacterSelect* self, int on, int a2);
extern "C" void func_001AB288(cFEStateNet* self, int err);
extern int D_004A4E88;
extern const char str_cFEStateChatRoom[];
extern const char str_cFEStateOnlineMainMenu[];
extern const char str_cFEStateBraggingRights[];

// Message handler for the profile list: select/scroll, error popups, and opening the follow-up screens.
extern "C" void* func_001B15D8(cFEStateOnlineCharacterSelect* self, int a1, int a2)
{
    void* obj;
    switch (a1) {
    case 243:
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cBENetworkInterface_networkOpt.OpponentCharacterID >= 0) {
            func_001B1760(self, cBENetworkInterface_networkOpt.OpponentCharacterID);
            func_001B1B10(self, cBENetworkInterface_networkOpt.OpponentCharacterID, 1);
        }
        break;
    case 245:
        func_001B1A08(self, 1, 1);
        break;
    case 247:
        func_001B1A08(self, 0, 1);
        break;
    case 238: case 241: case 0xF8: case 250: case 274: case 275:
        func_001AB288(((cFEStateNet*)self), a1);
        break;
    case 246:
        func_001A8A40(((cFEStateNet*)self), func_00192D00(cMemMan_alloc(1852, str_cFEStateBraggingRights, 0, 0), (int)self->mpEngine));
        break;
    case 268:
        if (func_0025DA90(LobbyMan_mLobbyMan) != 0 && D_004A4E88 == 0) {
            obj = cFEStateChatRoom_cFEStateChatRoom((sFEObj_001B1F88*)cMemMan_alloc(1852, str_cFEStateChatRoom, 0, 0), (int)self->mpEngine);
        } else {
            obj = cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), (int)self->mpEngine);
        }
        func_001A8A40(((cFEStateNet*)self), obj);
        break;
    default:
        return func_001A8E40_3(((cFEStateNet*)self), a1, a2);
    case 0xE0:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1760);
#ifdef SKIP_ASM
class cFEVText_001B1760 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014EEC8(void* self, int player, int index);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char str_TextOpponentSelect[];

extern "C" void func_001B1760(cFEStateOnlineCharacterSelect* self, int player)
{
    if (self->mpScreen != 0) {
        const char* str = (const char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), player, 0);
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextOpponentSelect));
        ((cFEVText_001B1760*)text)->v09(1);
        cUIText_setAsciiString(text, str);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1808);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
// PORT: usprintf is a varargs formatter (buf, fmt, ...); the unit declares a 3-arg view
extern "C" void usprintf_1B1808(char* buf, const char* fmt, ...) __asm__("usprintf");
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern char str_TextPlayerName[];
extern char str_TextChallengerName[];
extern char str_TextPlayerRank[];
extern char str_TextChallengerRank[];
extern char str_kT_OVRCMNRanking[];
extern char str_pct_s_pct_d_b[];

struct sVEntry_001B1808 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1B1808(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B1808* vt = *(sVEntry_001B1808**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

class cFEVObj_001B1808 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001B1808(cFEStateOnlineCharacterSelect* self)
{
    char buf[0x100];
    if (self->mpScreen == 0) {
        return;
    }
    cFEVObj_001B1808* o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextPlayerName));
    if (o != 0) {
        cUIText_setAsciiString((cUIText*)o, ((const char*)LobbyMan_mLobbyMan->mPersona));
        o->v09(1);
    }
    o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextChallengerName));
    if (o != 0) {
        cUIText_setAsciiString((cUIText*)o, ((const char*)LobbyMan_mLobbyMan->mOpponent));
        o->v09(1);
    }
    o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextPlayerRank));
    if (o != 0) {
        LobbyMan* mgr = LobbyMan_mLobbyMan;
        int name = Loc1B1808(str_kT_OVRCMNRanking);
        usprintf_1B1808(buf, str_pct_s_pct_d_b, name, mgr->mStats.mRank);
        cUIText_setUnicodeString((cUIText*)o, buf);
        o->v09(1);
    }
    o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextChallengerRank));
    if (o != 0) {
        LobbyMan* mgr = LobbyMan_mLobbyMan;
        int name = Loc1B1808(str_kT_OVRCMNRanking);
        usprintf_1B1808(buf, str_pct_s_pct_d_b, name, mgr->mOpponentStats.mRank);
        cUIText_setUnicodeString((cUIText*)o, buf);
        o->v09(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1A08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char str_frameChallenger[];
extern char str_framePlayer[];
extern char str_TextOpponentSelect[];

struct sColor1B1A08 {
    float r, g, b, a;
    sColor1B1A08(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cFEVObj_001B1A08 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11(const sColor1B1A08& c);
};

// Packed flags word at 0x14 of the UI text object: bits 8..12 hold a 5-bit size.
struct sText1B1A08 {
    char pad[0x14];
    unsigned int f0 : 8;
    unsigned int size : 5;
};

extern "C" void func_001B1A08(cFEStateOnlineCharacterSelect* self, int up, int highlight)
{
    if (self->mpScreen != 0) {
        sText1B1A08* t;
        if (highlight != 0) {
            t = (sText1B1A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_frameChallenger));
            cFEVObj_001B1A08* o = (cFEVObj_001B1A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextOpponentSelect));
            o->v11(sColor1B1A08(1.0f, 0.0f, 0.0f, 0.0f));
        } else {
            t = (sText1B1A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_framePlayer));
        }
        // Step the text's 5-bit size field up or down by 2 (wraps within the field).
        if (t != 0) {
            int v = t->size;
            if (up != 0) {
                t->size = v + 2;
            } else {
                t->size = v - 2;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1B10);
#ifdef SKIP_ASM
extern "C" void func_001B1760(cFEStateOnlineCharacterSelect* self, int index);
extern "C" void func_001B1B98(cFEStateOnlineCharacterSelect* self, int index, int a2, int a3);

extern "C" void func_001B1B10(cFEStateOnlineCharacterSelect* self, int sel, int flag)
{
    int i;
    for (i = 0; i < 30; i++) {
        if (i == sel && flag) {
            func_001B1760(self, i);
        }
        func_001B1B98(self, i, flag, i == sel);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1B98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00148BC8(void* iface, int a1, int idx);
extern "C" int func_00148C68(void* iface, int a1, int idx);
extern "C" int func_00148B78(void* iface, int a1, int idx);
extern "C" int func_00148CB8(void* iface, int a1, int idx);
extern "C" int func_00148D58(void* iface, int a1, int idx);
extern "C" int func_00148D08(void* iface, int a1, int idx);
extern "C" int func_00148C18(void* iface, int a1, int idx);
extern "C" void cUIProgress_setValue(void* self, int v);
extern char* D_004412C8[];
extern char str_pct_spct_s[];
extern char D_004A1B18[];
extern char D_004A1B20[];
extern char str_pct_d_b[];
extern char str_0LeftValue[];
extern char str_0RightValue[];
extern char str_0LeftAttribute[];
extern char str_0RightAttribute[];
extern unsigned char D_00534FD8[];
extern int D_00534B34[];

class cFEVObj_001B1B98 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001B1B98(cFEStateOnlineCharacterSelect* self, int index, int a2, int a3)
{
    char buf[0x30];
    int stats[7];
    char num[0x10];
    char* name1;
    char* name2;
    if ((unsigned int)index >= 31) {
        return;
    }
    if (a2 != 0) {
        sprintf(buf, str_pct_spct_s, D_004412C8[index], D_004A1B18);
    } else {
        sprintf(buf, str_pct_spct_s, D_004412C8[index], D_004A1B20);
    }
    cFEVObj_001B1B98* o = (cFEVObj_001B1B98*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
    if (o != 0) {
        o->v09(a3);
    }
    if (a3 == 0) {
        return;
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 3);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (a2 != 0) {
        name1 = str_0RightValue;
        name2 = str_0RightAttribute;
        unsigned char* src = D_00534FD8;
        for (int j = 0; j < 7; j++) {
            stats[j] = src[j];
        }
    } else {
        name1 = str_0LeftValue;
        name2 = str_0LeftAttribute;
        if (D_00534B34[0] != 0) {
            int v = 40;
            for (int j = 6; j >= 0; j--) {
                stats[j] = v;
            }
        } else {
            stats[0] = func_00148BC8(iface, 0, index);
            stats[1] = func_00148C68(iface, 0, index);
            stats[2] = func_00148B78(iface, 0, index);
            stats[3] = func_00148CB8(iface, 0, index);
            stats[4] = func_00148D58(iface, 0, index);
            stats[5] = func_00148D08(iface, 0, index);
            stats[6] = func_00148C18(iface, 0, index);
        }
    }
    for (int i = 0; i < 7; i++) {
        name1[0] = '0' + i;
        cFEVObj_001B1B98* a = (cFEVObj_001B1B98*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name1));
        name2[0] = '0' + i;
        void* b = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name2));
        if (a != 0 && b != 0) {
            int v = stats[i];
            int q = v / 5;
            sprintf(num, str_pct_d_b, q);
            a->v09(1);
            cUIText_setAsciiString((cUIText*)a, num);
            *(int*)((char*)b + 0x80) = 55;
            cUIProgress_setValue(b, v);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineCharacterSelect_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B1F18(cFEStateOnlineCharacterSelect* self, void* a1, int a2);

/**
 * Hands a popup's event on the rider-select screen to the handler for that popup's id.
 *
 * 0x838 (cancel challenge) goes to func_001B1F18, 0x835 to func_001A94D8, any other to the cFEStateNet handler
 * (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateOnlineCharacterSelect_onPopupEvent(cFEStateOnlineCharacterSelect* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2104:
        func_001B1F18(self, pPopup, Event);
        break;
    case 2101:
        func_001A94D8(self);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1F18);
#ifdef SKIP_ASM
extern void* D_004A33CC;
extern "C" void func_00265768(void* self, int a1);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001B1F18(cFEStateOnlineCharacterSelect* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        if (D_004A33CC != 0) {
            func_00265768(D_004A33CC, 249);
        }
        func_001A97B8(self, a1, 0xF);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_cFEStateChatRoom);
#ifdef SKIP_ASM
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern "C" char* strcpy(char* dst, const char* src);
extern void* cFEStateChatRoom_vtbl[];

struct sCol_001B1F88 {
    float r, g, b, a;
};

struct sFEObj_001B1F88 {
    char pad0[0x8];
    void** vt;                     // 0x8
    int fC;                        // 0xC
    char pad10[0x6EC - 0x10];
    int f6EC;                      // 0x6EC
    int f6F0;                      // 0x6F0
    int f6F4;                      // 0x6F4
    int f6F8;                      // 0x6F8
    int items[2];                  // 0x6FC
    int items2[2];                 // 0x704
    char name[0x20];               // 0x70C
    sCol_001B1F88 color;           // 0x72C
};

/**
 * Constructs the chat-room state.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with no parent, stores cFEStateChatRoom_vtbl and mHashName 0x3D,
 * clears mbExiting, both mpMenu and miMenuScroll entries, mpSlider1, mpTextScrollChat and mpSlider2, sets mChatColour
 * to white and copies LobbyMan's mPersona into mszPlayerName.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" sFEObj_001B1F88* cFEStateChatRoom_cFEStateChatRoom(sFEObj_001B1F88* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->vt = cFEStateChatRoom_vtbl;
    self->fC = 61;
    self->f6F8 = 0;
    for (int i = 0; i < 2; i++) {
        self->items[i] = 0;
        self->items2[i] = 0;
    }
    self->f6EC = 0;
    self->f6F0 = 0;
    self->f6F4 = 0;
    sCol_001B1F88 colour;
    colour.r = 1.0f;
    colour.g = 1.0f;
    colour.b = 1.0f;
    colour.a = 1.0f;
    self->color = colour;
    strcpy(self->name, ((const char*)LobbyMan_mLobbyMan->mPersona));
    return self;
}
#endif

extern void* cFEStateChatRoom_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom__cFEStateChatRoom__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the chat-room state: restores cFEStateChatRoom_vtbl and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The destructor flags stay untouched in $a1 and reach func_001A85D0, which frees the object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateChatRoom__cFEStateChatRoom(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateChatRoom_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001B2378(cFEStateChatRoom* self, int a1);
extern "C" void func_001B3978(cFEStateChatRoom* self, void* a1);
extern "C" void cUITextScroll_setInputMap(void* self, int a1);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern char str_235chat_room[];
extern char str_ChatUpArrow[];
extern char str_ChatDownArrow[];
extern char str_slideroutline2[];
extern char str_hlu2[];
extern char str_hld2[];
extern char str_UpArrow[];
extern char str_DownArrow[];
extern char str_slideroutline1[];

struct sBlk1B2070 {
    int v[4];
};

/**
 * Creates the "235chat_room" screen, fills the player list and wires the two scroll bars.
 *
 * Returns when the screen cannot be added. Otherwise plays it, fills the player list (func_001B2378(self, 1)), shows
 * the player's own info (func_001B3978 with LobbyMan's mPersona), and gives mpTextScrollChat bit 0 of +0x74,
 * cUITextScroll_setInputMap(1), mChatColour (+0xA8) and mpSlider2 (+0xB8). mpSlider2 gets "ChatUpArrow", "ChatDownArrow",
 * "slideroutline2", the highlight names "hlu2" / "hld2" (+0x8C / +0xA5, copied when at most 24 characters) and mode
 * 2; mpSlider1 gets "UpArrow", "DownArrow", "slideroutline1" and mode 2.
 */
extern "C" void cFEStateChatRoom_onCreateScreen(cFEStateChatRoom* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_235chat_room), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen == 0) {
        return;
    }
    cUIScreen_playFrame(screen, 0, 0);
    func_001B2378(self, 1);
    func_001B3978(self, LobbyMan_mLobbyMan->mPersona);
    *(int*)((char*)self->mpTextScrollChat + 0x74) |= 1;
    cUITextScroll_setInputMap(self->mpTextScrollChat, 1);
    *(sBlk1B2070*)((char*)self->mpTextScrollChat + 0xA8) = *(sBlk1B2070*)&self->mChatColour;
    *(void**)((char*)self->mpTextScrollChat + 0xB8) = (void*)self->mpSlider2;
    cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatUpArrow));
    cUIText* downArrow = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatDownArrow));
    cUIText* hideOnEmpty = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_slideroutline2));
    *(cUIText**)((char*)self->mpSlider2 + 0x7C) = uiText;
    *(cUIText**)((char*)self->mpSlider2 + 0x80) = downArrow;
    {
        char* slider2 = (char*)self->mpSlider2;
        if ((unsigned int)strlen(str_hlu2) > 24) {
            slider2[0x8C] = 0;
        } else {
            strcpy(slider2 + 0x8C, str_hlu2);
        }
    }
    {
        char* slider2 = (char*)self->mpSlider2;
        if ((unsigned int)strlen(str_hld2) > 24) {
            slider2[0xA5] = 0;
        } else {
            strcpy(slider2 + 0xA5, str_hld2);
        }
    }
    int two = 2;
    *(cUIText**)((char*)self->mpSlider2 + 0x84) = hideOnEmpty;
    *(int*)((char*)self->mpSlider2 + 0x88) = two;
    cUIText* upArrow1 = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_UpArrow));
    cUIText* downArrow1 = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_DownArrow));
    cUIText* outline1 = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_slideroutline1));
    *(cUIText**)((char*)self->mpSlider1 + 0x7C) = upArrow1;
    *(cUIText**)((char*)self->mpSlider1 + 0x80) = downArrow1;
    *(cUIText**)((char*)self->mpSlider1 + 0x84) = outline1;
    *(int*)((char*)self->mpSlider1 + 0x88) = two;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B22A0);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" int func_0025DA90(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

static inline void* GetSession_001B22A0()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001B22A0()
{
    func_00262768(GetSession_001B22A0(), 1, 3, func_0025DA90(LobbyMan_mLobbyMan), 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onUpdate);
#ifdef SKIP_ASM
extern void* cAppMan_TheAppMan;
extern "C" void func_001B3BF0(cFEStateChatRoom* self);
extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self);

/**
 * Updates the chat room: refreshes the players' connection-quality icons (func_001B3BF0) every 10000th frame.
 *
 * The frame counter is cAppMan_TheAppMan's word at +0x18; then the cFEStateNet update (cFEStateNet_onUpdate) runs.
 */
extern "C" void cFEStateChatRoom_onUpdate(cFEStateChatRoom* self)
{
    if (*(int*)((char*)cAppMan_TheAppMan + 0x18) % 10000 == 0) {
        func_001B3BF0(self);
    }
    cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2378);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001B3978(cFEStateChatRoom* self, void* a1);
extern "C" void func_001B3BF0(cFEStateChatRoom* self);
extern "C" void func_001B3D20(void* self, int idx, int on);
extern "C" void func_001B25E0(const char* src, void* out);
extern "C" int func_0025F1B0(void* self);
extern "C" const char* func_0025DA90_s(void* self) __asm__("func_0025DA90");
extern "C" int func_0025DA90(void* self);
extern "C" const char* func_0025F1E0_p(void* self, int idx, int* flag) __asm__("func_0025F1E0");
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
struct sFEObj_001BCB58;
extern "C" sFEObj_001BCB58* cFEStateLobbyMenu_cFEStateLobbyMenu(sFEObj_001BCB58* self, int a1);
extern char* D_004A3E90_p __asm__("cBXString_EmptyString");
extern char str_PlayerListMenu[];
extern char str_PlayerListItempct_02d[];
extern char str_cFEStateLobbyMenu[];
extern char D_004A1390[];
struct sName_001B2378 { char c[17]; };
extern sName_001B2378 D_00463A48;

struct sBXString_001B2378 {
    char* p;
    sBXString_001B2378() { p = D_004A3E90_p; }
    sBXString_001B2378(const sBXString_001B2378& o);
};

struct sCol_001B2378 {
    float r, g, b, a;
    sCol_001B2378() {}
    sCol_001B2378(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cFEVObj_001B2378 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sCol_001B2378& c);
};

extern "C" void func_001B2378(cFEStateChatRoom* vself, int a1)
{
    if (vself->mpScreen == 0) {
        return;
    }
    int n = func_0025F1B0(LobbyMan_mLobbyMan);
    func_001B3D20(((char*)vself), -1, 0);
    cUIText* menu = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_PlayerListMenu));
    if (*(unsigned char*)((char*)menu + 0x96) != n) {
        cUIMenu_setScrollableMenu(menu, n);
    }
    if (n < 7) {
        vself->miMenuScroll[0] = 0;
    }
    sName_001B2378 name = D_00463A48;
    sBXString_001B2378 s;
    int flag = 0;
    for (int i = 5; i >= 0; i--) {
        int idx = vself->miMenuScroll[0] + i;
        sprintf(name.c, str_PlayerListItempct_02d, i);
        cFEVObj_001B2378* t = (cFEVObj_001B2378*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(name.c));
        t->v11(sCol_001B2378(1.0f, 0.0f, 0.0f, 0.0f));
        if (idx < n) {
            cBXString_cBXString4(&s, func_0025F1E0_p(LobbyMan_mLobbyMan, idx, &flag));
            if (flag != 0) {
                char buf[256];
                func_001B25E0(func_0025DA90_s(LobbyMan_mLobbyMan), buf);
                if (func_0025DA90(LobbyMan_mLobbyMan) == 0) {
                    func_001A8A40(((char*)vself), cFEStateLobbyMenu_cFEStateLobbyMenu((sFEObj_001BCB58*)cMemMan_alloc(2948, str_cFEStateLobbyMenu, 0, 0), (int)vself->mpEngine));
                }
                cUIText_setUnicodeString(vself->mpTextRoomName, buf);
            }
        } else {
            cBXString_cBXString4(&s, D_004A1390);
        }
        cUIText_setAsciiString((cUIText*)t, s.p);
        *(int*)((char*)t + 0x18) = idx;
    }
    if (a1 != 0) {
        func_001B3978(vself, ((char*)vself) + 0x70C);
    }
    func_001B3BF0(vself);
    cBXString__cBXString(&s, 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B25E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern "C" void strcpy1(void* str, int a1);
extern "C" void func_002C2540(void* str, const char* s);
extern "C" void func_002C2688(void* str, unsigned short* s);
extern "C" int func_004165A8(const char* a, const char* b);
extern const char* D_004A1A60;
extern const char* D_004A1A64;
extern const char* D_004A1A68;
extern const char* D_004A1A6C;
extern char str_kT_POPUP232Elite[];
extern char str_kT_POPUP232Advanced[];
extern char str_kT_POPUP232Voice[];
extern char str_kT_POPUP232Beginner[];

struct sVEntry_001B25E0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1B25E0(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B25E0* vt = *(sVEntry_001B25E0**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

extern "C" void func_001B25E0(const char* src, void* out)
{
    char name[128];
    char ext[128];
    unsigned short wext[128];
    if (src == 0) {
        return;
    }
    int i;
    for (i = 0; src[i] != '.' && i < 127; i++) {
        name[i] = src[i];
    }
    name[i] = 0;
    i++;
    ext[0] = ' ';
    int j;
    for (j = 1; src[i] != 0 && j < 128; i++, j++) {
        ext[j] = src[i];
    }
    ext[j] = 0;
    if (func_004165A8(name, D_004A1A60) == 0) {
        strcpy1(out, Loc1B25E0(str_kT_POPUP232Elite));
    } else if (func_004165A8(name, D_004A1A64) == 0) {
        strcpy1(out, Loc1B25E0(str_kT_POPUP232Advanced));
    } else if (func_004165A8(name, D_004A1A68) == 0) {
        strcpy1(out, Loc1B25E0(str_kT_POPUP232Voice));
    } else if (func_004165A8(name, D_004A1A6C) == 0) {
        strcpy1(out, Loc1B25E0(str_kT_POPUP232Beginner));
    } else {
        func_002C2540(out, name);
    }
    UIAsciiToUnicode(wext, ext);
    func_002C2688(out, wext);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B27B0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int func_0025F1B0(void* self);
extern "C" void func_0025F288(void* self, int idx, void* info);
// PORT: sprintf2 is a varargs formatter (buf, fmt, ...); the unit declares a 4-arg view later
extern "C" int sprintf2_v(char* buf, const char* fmt, ...) __asm__("sprintf2");
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
// PORT: SN soft-float float->double helper called by name
extern "C" double fptodp(float f);
extern const char* D_004A1B30;
extern const char* D_004A1B34;
extern const char* D_004A1B38;
extern const char* D_004A1B3C;
extern const char* D_004A1B40;
extern char D_004A1390[];
extern char D_004A1408[];
extern char str_ps2X[];
extern char str_TextPlayerName[];
extern char str_kT_CMNGamesPlayed[];
extern char str_kT_CMNWinRatio[];
extern char str_kT_CMNDisconnects[];
extern char str_Txt_Select[];
extern char str_TextRanking[];
extern char str_kT_CMNRaceRanking[];
extern char str_TextGamesPlayed[];
extern char str_TextWinRatio[];
extern char str_TextDisconnects[];
extern char str_TextOverallRanking[];
extern char str_kT_235OverallRanking[];

struct sBXString_1B27B0 {
    char* str;
    sBXString_1B27B0() {}
    sBXString_1B27B0(const sBXString_1B27B0& o);
};

struct sInfo_1B27B0 {
    int a;
    int b;
    int c;
    int d;
    int e;
};

extern "C" void func_001B27B0(cFEStateChatRoom* vself, int idx)
{
    sBXString_1B27B0 s;
    char buf[0x400];
    sInfo_1B27B0 info;
    int flag = 0;
    cBXString_cBXString2(&s, func_0025F1E0_p(LobbyMan_mLobbyMan, idx, &flag));
    strcpy(((char*)vself) + 0x70C, s.str);
    if (vself->mpScreen == 0) {
        cBXString__cBXString(&s, 2);
        return;
    }
    int valid = 0;
    if (func_0025F1B0(LobbyMan_mLobbyMan) > 0) {
        valid = idx < func_0025F1B0(LobbyMan_mLobbyMan);
    }
    cUIText* title = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_TextPlayerName));
    if (title != 0) {
        cFEVObj_001B2378* a = (cFEVObj_001B2378*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_Txt_Select));
        cFEVObj_001B2378* b = (cFEVObj_001B2378*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_ps2X));
        if (a != 0 && b != 0) {
            if (flag != 0) {
                b->v09(0);
                a->v09(0);
            } else {
                b->v09(1);
                a->v09(1);
            }
        }
        if (valid && *(int*)(s.str - 8) > 0) {
            cUIText_setAsciiString(title, s.str);
        } else {
            cUIText_setAsciiString(title, D_004A1390);
        }
    }
    func_0025F288(LobbyMan_mLobbyMan, idx, &info);
    cUIText* uiText = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_TextRanking));
    if (uiText != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B30, Loc1B25E0(str_kT_CMNRaceRanking), info.a);
            cUIText_setUnicodeString(uiText, buf);
        } else {
            cUIText_setAsciiString(uiText, D_004A1408);
        }
    }
    uiText = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_TextGamesPlayed));
    if (uiText != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B34, Loc1B25E0(str_kT_CMNGamesPlayed), info.c);
            cUIText_setUnicodeString(uiText, buf);
        } else {
            cUIText_setAsciiString(uiText, D_004A1408);
        }
    }
    uiText = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_TextWinRatio));
    if (uiText != 0) {
        if (valid) {
            int done = info.b;
            int total = info.c;
            float pct = 0.0f;
            if (done < total) {
                pct = (float)done / (float)total * 100.0f;
            } else if (done != 0) {
                pct = 100.0f;
            }
            sprintf2_v(buf, D_004A1B38, Loc1B25E0(str_kT_CMNWinRatio), info.b, info.c, fptodp(pct));
            cUIText_setUnicodeString(uiText, buf);
        } else {
            cUIText_setAsciiString(uiText, D_004A1408);
        }
    }
    uiText = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_TextDisconnects));
    if (uiText != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B3C, Loc1B25E0(str_kT_CMNDisconnects), info.d);
            cUIText_setUnicodeString(uiText, buf);
        } else {
            cUIText_setAsciiString(uiText, D_004A1408);
        }
    }
    uiText = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_TextOverallRanking));
    if (uiText != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B40, Loc1B25E0(str_kT_235OverallRanking), info.a);
            cUIText_setUnicodeString(uiText, buf);
        } else {
            cUIText_setAsciiString(uiText, D_004A1408);
        }
    }
    cBXString__cBXString(&s, 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2CD0);
#ifdef SKIP_ASM
struct cUIText;
struct sList1A83D8;
extern "C" void func_001A83D8(sList1A83D8* list, int keep, unsigned int sel, int a3, int pos, int a5, int a6, int trim);
extern "C" void cUITextScroll_addAsciiString(cUIText* text, char* str);
extern char D_004A1A50[];
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
struct cBXString;
extern "C" void* cBXString_Concat(void* self, const char* str);
extern "C" cBXString* func_003189A0(cBXString* self, char** other);

struct sBXString1B2CD0 {
    char* str;
};

struct sListInfo1B2CD0 {
    char pad[0xC4];
    unsigned short pos;     // 0xC4
    unsigned short a5;      // 0xC6
    unsigned short a3;      // 0xC8
};

// Shows the item's name and value text in the list and refreshes it.
// PORT: the unit declares the info argument as int.
extern "C" void func_001B2CD0(cFEStateChatRoom* self, int infov)
{
    char* info = (char*)infov;
    if ((int)self->mpScreen == 0) {
        return;
    }
    sListInfo1B2CD0* list = (sListInfo1B2CD0*)self->mpTextScrollChat;
    if (list == 0) {
        return;
    }
    int a3 = list->a3;
    int first = a3 == 0;
    int pos = list->pos;
    int a5 = list->a5;
    sBXString1B2CD0 text;
    cBXString_cBXString1(&text, info + 0x8);
    cBXString_Concat(&text, D_004A1A50);
    func_003189A0((cBXString*)&text, (char**)(info + 0xC));
    cUITextScroll_addAsciiString(self->mpTextScrollChat, text.str);
    func_001A83D8((sList1A83D8*)self->mpTextScrollChat, 0x40, 6, a3, pos, a5, first, 1);
    cBXString__cBXString(&text, 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2DA0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP235ChatRoom[];
extern char str_kT_HELP235InitiateChallenge[];
extern char str_kT_HELP235ChangePlayer[];
extern char str_kT_HELP235PlayerOptions[];
extern char str_kT_HELP235TypeMessages[];
extern char str_kT_BTNEAMessenger[];
extern char str_kT_HELPCmnPrevious[];
extern char str_kT_HELP235ScrollChat[];

extern "C" void func_001B2DA0(cFEStateChatRoom* self, void* menu)
{
    func_001DA528(menu, 6);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP235ChatRoom);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP235InitiateChallenge));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELP235ChangePlayer), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELP235PlayerOptions), 0);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELP235TypeMessages), 6);
    func_001DA510(menu, 3, GetHashValue32(str_kT_BTNEAMessenger), 2);
    func_001DA510(menu, 4, GetHashValue32(str_kT_HELPCmnPrevious), 1);
    func_001DA510(menu, 5, GetHashValue32(str_kT_HELP235ScrollChat), 8);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_0025F1B0(void* self);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern char D_004A1408[];
extern char str_TextPlayerName[];
extern char str_PlayerListMenu[];
extern char str_Txt_Select[];
extern char str_TextRoomName[];
extern char str_TextChatWindowTitle[];
extern char str_kT_235MessageHistory[];
extern char str_TextAvailablePlayersTitle[];
extern char str_kT_235AvailablePlayers[];
extern char str_MenuChat[];
extern char str_kT_BTNSelect[];
extern char str_Txt_Options[];
extern char str_kT_BTNOptions[];
extern char str_Txt_Previous[];
extern char str_kT_BTNPrevious[];
extern char str_Txt_Buddies[];
extern char str_kT_BTNBuddies[];
extern char str_TextScrollChat[];
extern char str_qualitypct_d[];
extern char str_slider1[];
extern char str_slider2[];

class cFEVObj_001B2EB8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08(int on);
    virtual void v09(int on);
};

/**
 * Binds the chat-room widgets as the screen creates them.
 *
 * "TextRoomName" becomes mpTextRoomName; the chat-window and available-players titles get "kT_235MessageHistory" and
 * "kT_235AvailablePlayers"; "TextPlayerName" starts empty; "PlayerListMenu" (bit 7, focus, scrollable by the LobbyMan
 * player count or 6) and "MenuChat" (focus) become mpMenu[0] and [1]; the button labels get "kT_BTNSelect",
 * "kT_BTNOptions", "kT_BTNPrevious" and "kT_BTNBuddies"; "slider1" and "slider2" (attribute bit 0) become mpSlider1 /
 * mpSlider2 and "TextScrollChat" mpTextScrollChat. Every widget is also checked against "quality1".."quality6" and
 * stored in mpQualityIcon.
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateChatRoom_onWidgetCreate(cFEStateChatRoom* self, cFEVObj_001B2EB8* obj)
{
    char buf[32];
    char* widget = (char*)obj;
    int id = *(int*)(widget + 0x38);
    if (id == GetHashValue32(str_TextRoomName)) {
        self->mpTextRoomName = (cUIText*)obj;
        obj->v09(1);
    } else if (id == GetHashValue32(str_TextChatWindowTitle)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(str_kT_235MessageHistory));
        obj->v09(1);
    } else if (id == GetHashValue32(str_TextPlayerName)) {
        obj->v09(1);
        cUIText_setAsciiString((cUIText*)obj, D_004A1408);
    } else if (id == GetHashValue32(str_TextAvailablePlayersTitle)) {
        obj->v09(1);
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(str_kT_235AvailablePlayers));
    } else if (id == GetHashValue32(str_PlayerListMenu)) {
        *(int*)(widget + 0x14) |= 0x80;
        obj->v07(1);
        int n = func_0025F1B0(LobbyMan_mLobbyMan);
        if (n <= 0) {
            cUIMenu_setScrollableMenu(obj, 6);
        } else {
            cUIMenu_setScrollableMenu(obj, n);
        }
        self->mpMenu[0] = (cUIMenu*)obj;
    } else if (id == GetHashValue32(str_MenuChat)) {
        obj->v07(1);
        self->mpMenu[1] = (cUIMenu*)obj;
    } else if (id == GetHashValue32(str_Txt_Select)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(str_kT_BTNSelect));
    } else if (id == GetHashValue32(str_Txt_Options)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(str_kT_BTNOptions));
    } else if (id == GetHashValue32(str_Txt_Previous)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(str_kT_BTNPrevious));
    } else if (id == GetHashValue32(str_Txt_Buddies)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(str_kT_BTNBuddies));
    } else if (id == GetHashValue32(str_slider1)) {
        self->mpSlider1 = (cUIThing*)obj;
        *(int*)(widget + 0x14) |= 1;
    } else if (id == GetHashValue32(str_TextScrollChat)) {
        self->mpTextScrollChat = (cUIText*)obj;
    } else if (id == GetHashValue32(str_slider2)) {
        self->mpSlider2 = (cUIThing*)obj;
        *(int*)(widget + 0x14) |= 1;
    }
    void** slots = (void**)(((char*)self) + 0x6D0);
    for (int i = 0; i < 6; i++) {
        sprintf(buf, str_qualitypct_d, i + 1);
        if (id == GetHashValue32(buf)) {
            *(void**)((char*)slots + (i << 2)) = obj;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onWidgetEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_0025F248(void* self, int idx);
extern "C" void func_0025D800(void* a, int b, const char* c);
extern "C" void func_001A9B58(cFEStateNet* self);
extern "C" void func_001B33D8(cFEStateChatRoom* self);
extern char str_PlayerListItempct_02d[];
extern char str_kT_POPUP231ExitingRoom[];

/**
 * Handles the chat-room widgets: picking a player, scrolling the list, leaving the room and sending chat.
 *
 * Event 5 on a player row (id = the row's player) does nothing for func_0025F248's players; otherwise makes that
 * player's name the selected user (func_001A9890) and opens their options (func_001AB370(.., 1)). Events 1 / 2 on a
 * "PlayerListItem%02d" row scroll the list (func_001B3D20 with 1 / 0). Event 6 sets mbExiting, shows
 * "kT_POPUP231ExitingRoom" (no buttons, id 0x802) and leaves the room (func_0025D800(-1, 0)). Event 8 calls
 * func_001A9B58 unless mAttrState bits 1-2 are set; event 13 (keyboard done) sends the typed chat (func_001B33D8);
 * the rest go to the cFEStateNet handler. The parameter names are off: @p msg is the widget and @p ev the event.
 *
 * @param msg  the widget (On Tour pWidget)
 * @param ev  the widget event (On Tour Event)
 */
extern "C" void cFEStateChatRoom_onWidgetEvent(cFEStateChatRoom* self, char* msg, int ev)
{
    char buf[32];
    int i;
    switch (ev) {
    case 5: {
        int id = *(int*)(msg + 0x18);
        if (func_0025F248(LobbyMan_mLobbyMan, id) != 0) {
            return;
        }
        func_001A9890((cFEStateNet*)self, func_0025F1E0_p(LobbyMan_mLobbyMan, id, 0));
        func_001AB370((cFEStateNet*)self, func_001A98B0(((char*)self)), 1);
        return;
    }
    case 1:
        for (i = 0; i < 6; i++) {
            sprintf(buf, str_PlayerListItempct_02d, i);
            int hash = *(int*)(msg + 0x38);
            if (hash == GetHashValue32(buf)) {
                func_001B3D20(((char*)self), i, 1);
                return;
            }
        }
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)msg, ev);
        return;
    case 2:
        for (i = 0; i < 6; i++) {
            sprintf(buf, str_PlayerListItempct_02d, i);
            int hash = *(int*)(msg + 0x38);
            if (hash == GetHashValue32(buf)) {
                func_001B3D20(((char*)self), i, 0);
                return;
            }
        }
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)msg, ev);
        return;
    case 6: {
        self->mbExiting = 1;
        char* popup = (char*)func_001A9C50((cFEStateNet*)self);
        *(int*)(popup + 0xBC) = 0;
        int hash = GetHashValue32(str_kT_POPUP231ExitingRoom);
        *(int*)(popup + 0x164) = 0;
        *(int*)(popup + 0xC4) = hash;
        *(int*)(popup + 0xC0) = 3;
        *(int*)(popup + 0x150) = 2050;
        func_001A9BC0((cFEStateNet*)self, popup);
        func_0025D800(LobbyMan_mLobbyMan, -1, 0);
        return;
    }
    case 8:
        if ((self->mAttrState & 6) == 0) {
            func_001A9B58((cFEStateNet*)self);
        }
        return;
    case 13:
        func_001B33D8(self);
        return;
    }
    cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)msg, ev);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B33D8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_0025AAE0(void* self, void* a1);
extern "C" void func_00262690(void* self, int a1, void* a2);
int func_001A98B0(void* self);

static inline void* GetSession_001B33D8()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001B33D8(cFEStateChatRoom* self)
{
    char* keyboard = (char*)self->mpKeyboard;
    switch (*(int*)(keyboard + 0x18)) {
    case 4:
        func_0025AAE0(LobbyMan_mLobbyMan, keyboard + 0x74);
        break;
    case 2:
        func_00262690(GetSession_001B33D8(), func_001A98B0(((cFEStateNet*)self)), (char*)self->mpKeyboard + 0x74);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onGainTransition);
#ifdef SKIP_ASM
class cFEVObj_001B3490 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int a1);
};

struct sFEObj_001B3490 {
    char pad[0x6EC];
    void* f6EC;                    // 0x6EC
    int pad6F0;
    void* f6F4;                    // 0x6F4
    int pad6F8;
    cFEVObj_001B3490* items[2];    // 0x6FC
};

extern "C" void cUIScrollBar_setScrollVisibile(void* a, int b);
extern "C" void func_001B3978(cFEStateChatRoom* self, void* a1);

/**
 * Prepares the chat room the first time it opens: hides both scroll bars, focuses both menus, shows own info.
 *
 * The info is func_001B3978 with LobbyMan's mPersona.
 *
 * @param bFirstTime  nonzero the first time the state opens (On Tour bFirstTime)
 * @return 0
 */
extern "C" int cFEStateChatRoom_onGainTransition(cFEStateChatRoom* self, int bFirstTime)
{
    if (bFirstTime != 0) {
        int i;
        cUIScrollBar_setScrollVisibile(self->mpSlider1, 0);
        cUIScrollBar_setScrollVisibile(self->mpSlider2, 0);
        for (i = 0; i < 2; i++) {
            if (((cFEVObj_001B3490**)&self->mpMenu[0])[i] != 0) {
                ((cFEVObj_001B3490**)&self->mpMenu[0])[i]->v07(1);
            }
        }
        func_001B3978(self, LobbyMan_mLobbyMan->mPersona);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onGainFocus);
#ifdef SKIP_ASM
extern "C" void func_001B3BF0(cFEStateChatRoom* self);
extern "C" void cFEStateNet_onGainFocus(cFEStateNet* self, int a1);

/**
 * Refreshes the connection-quality icons (func_001B3BF0), then runs the cFEStateNet onGainFocus (cFEStateNet_onGainFocus).
 *
 * @param pPrevState  the state that had focus before (On Tour pPrevState)
 */
extern "C" void cFEStateChatRoom_onGainFocus(cFEStateChatRoom* self, int pPrevState)
{
    func_001B3BF0(self);
    cFEStateNet_onGainFocus((cFEStateNet*)self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void func_001B2378(cFEStateChatRoom* self, int a1);

/**
 * Tracks the chat room's menu scrolling: event 4 stores the new first row of the player list or the chat menu.
 *
 * For mpMenu[0] the value goes to miMenuScroll[0] and the player list is refilled (func_001B2378(self, 0)); for
 * mpMenu[1] it goes to miMenuScroll[1].
 *
 * @param sender  the menu (On Tour pMenu)
 * @param event  the menu event (On Tour Event)
 * @param value  the new scroll index (On Tour index)
 * @return always 0x101
 */
extern "C" int cFEStateChatRoom_onWidgetMenu(cFEStateChatRoom* self, void* sender, int event, int value)
{
    if (event == 4) {
        if (sender == (void*)self->mpMenu[0]) {
            self->miMenuScroll[0] = value;
            func_001B2378(self, 0);
            return 257;
        }
        if (sender == (void*)self->mpMenu[1]) {
            self->miMenuScroll[1] = value;
        }
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B35B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
int func_001A98B0(void* self);
extern "C" void func_001ABB30(cFEStateNet* self, int a1, void* a2, int a3);
extern "C" void func_001A4B20_ABC50(void* self, int a1, int* out) __asm__("func_001ABC50");
extern "C" void func_001A4B20_ABE58(void* self, int a1, int* out) __asm__("func_001ABE58");
extern "C" void func_001ABED8(void* self);
extern "C" void func_001ABCD0(cFEStateNet* self, int a1, void* a2, int a3, int a4);
extern "C" void func_0025DF00(void* self, int a1);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern char str_kT_POPUP235SubmittingAbuse[];

// Memory-card result handler: runs the follow-up for each result; on 19 shows the failure popup.
extern "C" void func_001B35B0(cFEStateChatRoom* self, void* a1, int kind)
{
    int r17;
    int r18;
    switch (kind) {
    case 15:
        func_001ABCD0(((cFEStateNet*)self), func_001A98B0(((cFEStateNet*)self)), a1, kind, 0);
        break;
    case 16:
        func_001A97B8(((cFEStateNet*)self), a1, kind);
        func_001ABB30(((cFEStateNet*)self), func_001A98B0(((cFEStateNet*)self)), a1, kind);
        break;
    case 17:
        func_001A4B20_ABE58(((cFEStateNet*)self), func_001A98B0(((cFEStateNet*)self)), &r17);
        if (r17 == 1) {
            func_001ABED8(((cFEStateNet*)self));
        } else {
            func_001A97B8(((cFEStateNet*)self), a1, kind);
        }
        break;
    case 18:
        func_001A4B20_ABC50(((cFEStateNet*)self), func_001A98B0(((cFEStateNet*)self)), &r18);
        if (r18 == 1) {
            func_001ABED8(((cFEStateNet*)self));
        } else {
            func_001A97B8(((cFEStateNet*)self), a1, kind);
        }
        break;
    case 19: {
        self->unk_6C4 = 0;
        LobbyMan* mc = LobbyMan_mLobbyMan;
        func_0025DF00(mc, func_001A98B0(((cFEStateNet*)self)));
        void* p = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUP235SubmittingAbuse);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 2053;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0(((cFEStateNet*)self), p);
    }
    default:
        func_001A97B8(((cFEStateNet*)self), a1, kind);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3760);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_0039F600(void* p);
extern const char str_cFEStateOnlineCharacterSelect[];

extern "C" void func_001B3760(cFEStateChatRoom* self, void* popup, int event)
{
    if (event == 0xF) {
        func_0039F600((char*)self->mpEngine + 0x18);
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(1764, str_cFEStateOnlineCharacterSelect, 0x100, 0), (int)self->mpEngine));
    } else {
        func_001A97B8(((cFEStateNet*)self), popup, event);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B37D8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_0039F600(void* p);
extern const char str_cFEStateOnlineCharacterSelect[];

extern "C" void func_001B37D8(cFEStateChatRoom* self, void* popup, int event)
{
    if (event == 0xF) {
        func_0039F600((char*)self->mpEngine + 0x18);
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(1764, str_cFEStateOnlineCharacterSelect, 0x100, 0), (int)self->mpEngine));
    } else {
        func_001A97B8(((cFEStateNet*)self), popup, event);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B35B0(cFEStateChatRoom* self, void* a1, int a2);
extern "C" void func_001B3760(cFEStateChatRoom* self, void* a1, int a2);
extern "C" void func_001B37D8(cFEStateChatRoom* self, void* a1, int a2);

/**
 * Hands a popup's event in the chat room to the handler for that popup's id.
 *
 * 0x80B func_001B35B0, 0x800 func_001B3760, 0x801 func_001B37D8, any other the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateChatRoom_onPopupEvent(cFEStateChatRoom* self, void* pPopup, int Event)
{
    switch (*(unsigned int*)((char*)pPopup + 0x18)) {
    case 2059:
        func_001B35B0(self, pPopup, Event);
        break;
    case 0x800:
        func_001B3760(self, pPopup, Event);
        break;
    case 2049:
        func_001B37D8(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom_onInputBegin);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern char str_kT_POPUP235Chat[];
extern char D_004A1408[];

/**
 * Opens the chat keyboard when L1 is pressed in the chat room.
 *
 * When the input's slot 27 (isLShift1) is set, opens the keyboard (func_001A9150) titled "kT_POPUP235Chat", empty, at
 * most 63 characters, as a regular keyboard (meType 0) with key 0x4B included and mbReferEventsToParent set.
 *
 * @param obj  the input (On Tour cUIInput* pInput)
 * @return 1 when the keyboard was opened (input consumed), else 0
 */
extern "C" int cFEStateChatRoom_onInputBegin(cFEStateChatRoom* self, void* obj)
{
    sVE1A4170* vt = *(sVE1A4170**)((char*)obj + 0x8);
    if (vt[27].fn((char*)obj + vt[27].delta) != 0) {
        // PORT: string pointer passed through an int parameter (func_001A9150's a3)
        func_001A9150(((cFEStateNet*)self), 0, GetHashValue32(str_kT_POPUP235Chat), (int)D_004A1408, 0, 4, 0x3F);
        *(int*)((char*)self->mpKeyboard + 0x43C) = 0;
        func_001CE468(self->mpKeyboard, 1);
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
        *(int*)((char*)self->mpKeyboard + 0x438) = 1;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3978);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" int func_0025F1B0(void* self);
extern "C" const char* func_0025F1E0(void* self, int idx, int a2);
extern "C" int func_004165A8(const char* a, const char* b);
extern char str_PlayerListMenu[];

extern "C" void func_001B3978(cFEStateChatRoom* self, void* name)
{
    if (self->mpScreen != 0) {
        int sel = -1;
        int i = 0;
        int n = func_0025F1B0(LobbyMan_mLobbyMan);
        for (; i < n; i++) {
            if (func_004165A8(func_0025F1E0(LobbyMan_mLobbyMan, i, 0), (const char*)name) == 0) {
                sel = i;
                break;
            }
        }
        if (sel >= 0) {
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_PlayerListMenu)), sel);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3A30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void func_001B2378(cFEStateChatRoom* self, int a1);
extern "C" void func_001B3978(cFEStateChatRoom* self, void* a1);
struct sFEObj_001BCB58;
extern "C" sFEObj_001BCB58* cFEStateLobbyMenu_cFEStateLobbyMenu(sFEObj_001BCB58* self, int a1);
extern "C" void func_001B2CD0(cFEStateChatRoom* self, int a1);
extern "C" int func_0025DA90(void* self);
int func_001A98B0(void* self);
extern "C" void func_001AB3D0(cFEStateNet* self, int a1, int a2);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern char str_cFEStateLobbyMenu[];
extern char str_kT_POPUP235Abuse[];
extern char str_kT_BTNOK[];

// Lobby/server message handler (cFEStateNet virtual slot 31) of the chat room (cFEStateChatRoom): 220 opens the lobby
// menu (cFEStateLobbyMenu) unless the lobby is busy, 232 shows the abuse-report popup; unknown ids go to cFEStateNet's
// handler func_001A8E40.
extern "C" void* func_001B3A30(cFEStateChatRoom* self, int msgId, int data)
{
    switch (msgId) {
    case 0xE0: {
        char buf[32];
        strcpy(buf, (char*)self + 0x70C);
        func_001B2378(self, 1);
        func_001B3978(self, buf);
        break;
    }
    case 220:
        if (func_0025DA90(LobbyMan_mLobbyMan) == 0) {
            func_001A8A40(self, cFEStateLobbyMenu_cFEStateLobbyMenu((sFEObj_001BCB58*)cMemMan_alloc(2948, str_cFEStateLobbyMenu, 0, 0), (int)self->mpEngine));
            break;
        }
        func_001B2378(self, 1);
        break;
    case 232: {
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP235Abuse);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2052;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    case 222:
        if (self->mbExiting == 0) {
            func_001B2CD0(self, data);
        }
        break;
    case 225:
        func_001B2378(self, 1);
        break;
    case 234: case 235: case 236: case 237: case 238: case 239: case 0xF0:
    case 262: case 266: case 267:
        func_001AB3D0((cFEStateNet*)self, func_001A98B0(self), 1);
        break;
    default:
        func_001AB3D0((cFEStateNet*)self, func_001A98B0(self), 1);
        return func_001A8E40_3(self, msgId, data);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3BF0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_001AAD50(void* self, int a1, int a2);
extern "C" int func_0025F1B0(void* self);
extern "C" int func_0025F248(void* self, int idx);
extern "C" void func_0025F288(void* self, int idx, void* info);
extern char str_PlayerListItempct_02d[];

class cFEVObj_001B3BF0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

struct sInfo1B3BF0 {
    int f00;
    int f04;
    int f08;
    int f0C;
    int f10;
    int pad[3];
};

struct sOwner1B3BF0 {
    char pad0[0x40];
    void* screen;
    char pad44[0x68C];
    cFEVObj_001B3BF0* items[6];
    char pad6E8[0x1C];
    int first;
};

extern "C" void func_001B3BF0(cFEStateChatRoom* feStateChatRoom)
{
    sOwner1B3BF0* self = (sOwner1B3BF0*)feStateChatRoom;
    char buf[32];
    sInfo1B3BF0 info;
    int n = func_0025F1B0(LobbyMan_mLobbyMan);
    int i;
    for (i = 0; i < 6; i++) {
        int idx = self->first + i;
        sprintf(buf, str_PlayerListItempct_02d, i);
        if (idx < n) {
            int r = func_0025F248(LobbyMan_mLobbyMan, idx);
            self->items[i]->v09(1);
            if (r != 0) {
                self->items[i]->v09(0);
            } else {
                func_0025F288(LobbyMan_mLobbyMan, idx, &info);
                func_001AAD50(self->screen, i, info.f10);
            }
        } else {
            self->items[i]->v09(0);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3D20);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_001B27B0(cFEStateChatRoom* self, int a1);
extern "C" int func_0025F1B0(void* self);
extern "C" void cUIScrollBar_setValues(void* a0, void* a1, int a2, int a3);
extern "C" void func_003A0290(void* text, void* out);
extern char str_PlayerListItempct_02d[];
extern char str_whitebarpct_02d[];
extern char str_hl[];

struct sCol1B3D20 {
    float r, g, b, a;
    sCol1B3D20() {}
    sCol1B3D20(const float& r_, const float& g_, const float& b_, const float& a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct sVec3_1B3D20 {
    float x, y, z;
};

class cFEVObj_001B3D20 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sCol1B3D20& c);
};

static inline cFEVObj_001B3D20* Get1B3D20(void* self, char* name)
{
    return (cFEVObj_001B3D20*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

extern "C" void func_001B3D20(void* self, int idx, int on)
{
    char name[32];
    if (*(void**)((char*)self + 0x40) == 0) {
        return;
    }
    sprintf(name, str_PlayerListItempct_02d, idx);
    cFEVObj_001B3D20* sel = Get1B3D20(self, name);
    cUIScrollBar_setValues(*(void**)((char*)self + 0x6EC), *(void**)((char*)self + 0x704), 6, func_0025F1B0(LobbyMan_mLobbyMan));
    if (sel == 0) {
        if (idx >= 0) {
            if (idx < 6) {
                return;
            }
        }
    }
    int i;
    for (i = 0; i < 6; i++) {
        sprintf(name, str_PlayerListItempct_02d, i);
        Get1B3D20(self, name)->v11(sCol1B3D20(1.0f, 0.0f, 0.0f, 0.0f));
    }
    cFEVObj_001B3D20* hl = Get1B3D20(self, str_hl);
    if (hl == 0) {
        return;
    }
    if (on == 0) {
        hl->v09(0);
        return;
    }
    sprintf(name, str_whitebarpct_02d, idx);
    cFEVObj_001B3D20* t = Get1B3D20(self, name);
    if (t == 0) {
        return;
    }
    sCol1B3D20 pos;
    func_003A0290(t, &pos);
    pos.r += 2.0f;
    pos.g += 3.0f;
    *(sVec3_1B3D20*)((char*)hl + 0x44) = *(sVec3_1B3D20*)&pos;
    hl->v09(1);
    func_001B27B0((cFEStateChatRoom*)self, *(int*)((char*)sel + 0x18));
    sel->v11(sCol1B3D20(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_cFEStateNetConfig);
#ifdef SKIP_ASM
class cFEVObj_001B3F78 {
public:
    virtual void v01();
    virtual void v02();
};

extern void* cFEStateNetConfig_vtbl[];
extern cFEVObj_001B3F78* D_004A1B78;
extern "C" cFEVObj_001B3F78* func_002C22C8();
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the network-configuration screen state ("200net_config") and creates the network configuration object.
 *
 * Runs the cFEStateNet constructor (func_001A8500, no parent state), zeroes the selection, setting count, list
 * boxes, menu, widgets, popup text, auth-error texts and state, clears unk_6A8, stores cFEStateNetConfig_vtbl and
 * mHashName 0x31, then stores func_002C22C8()'s object in D_004A1B78 (the object the connect state machine drives;
 * its delete, cMemCard_DeleteInstance, matches On Tour's cMemCard::DeleteInstance) and calls its slot 2. miWaitFrames starts
 * at -1.
 *
 * @param mem  the 0x91C-byte block to construct in
 * @param pEngine  the UI engine (On Tour cUIState ctor pEngine), passed to the cFEStateNet constructor
 * @return @p mem
 */
extern "C" void* cFEStateNetConfig_cFEStateNetConfig(cFEStateNetConfig* mem, int pEngine)
{
    func_001A8500_3(mem, pEngine, 0);
    mem->miSelectedSetting = 0;
    mem->miNumSettings = 0;
    mem->unk_6D8 = 0;
    mem->mpLBAdapters = 0;
    mem->mpLBSettings = 0;
    mem->mpMenuConfig = 0;
    mem->mpConfigRectangle = 0;
    mem->mpNoConfiguration = 0;
    mem->unk_700 = 0;
    mem->mpustrPopupText = 0;
    mem->miState = 0;
    mem->mpustrAuthError[0] = 0;
    mem->mpustrAuthError[1] = 0;
    mem->mpustrAuthError[2] = 0;
    mem->unk_6A8 = 0;
    mem->mVtbl = cFEStateNetConfig_vtbl;
    mem->mHashName = 49;
    cFEVObj_001B3F78* memCard = func_002C22C8();
    D_004A1B78 = memCard;
    memCard->v02();
    mem->miWaitFrames = -1;
    return ((void*)mem);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig__cFEStateNetConfig);
#ifdef SKIP_ASM
extern void* cFEStateNetConfig_vtbl[];
void cMemMan_free(void* p);
extern "C" void cMemCard_DeleteInstance(void* p);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");
// D_004A1B78 is declared earlier in the unit (cFEVObj_001B3F78*, see cFEStateNetConfig_cFEStateNetConfig)

/**
 * Destroys the network-configuration screen state: frees its heap texts and the network configuration object.
 *
 * Restores cFEStateNetConfig_vtbl, frees the three mpustrAuthError texts and mpustrPopupText (cMemMan_free) and
 * deletes D_004A1B78 with cMemCard_DeleteInstance, then runs the cFEStateNet destructor (func_001A85D0) with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cFEStateNetConfig__cFEStateNetConfig(cFEStateNetConfig* self, int flags)
{
    self->mVtbl = cFEStateNetConfig_vtbl;
    if (self->mpustrAuthError[0] != 0) {
        cMemMan_free(self->mpustrAuthError[0]);
        self->mpustrAuthError[0] = 0;
    }
    if (self->mpustrAuthError[1] != 0) {
        cMemMan_free(self->mpustrAuthError[1]);
        self->mpustrAuthError[1] = 0;
    }
    if (self->mpustrAuthError[2] != 0) {
        cMemMan_free(self->mpustrAuthError[2]);
        self->mpustrAuthError[2] = 0;
    }
    if (self->mpustrPopupText != 0) {
        cMemMan_free(self->mpustrPopupText);
        self->mpustrPopupText = 0;
    }
    if (D_004A1B78 != 0) {
        cMemCard_DeleteInstance(D_004A1B78);
        D_004A1B78 = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_00256C50();
extern "C" void func_001B5268(cFEStateNetConfig* self, int a1);
extern "C" void* func_0028B180();
void func_0028FC38(void* self);
extern char str_200net_config[];

/**
 * Opens the "200net_config" screen, shuts down any game comm session and resets the configuration menu.
 *
 * Adds the screen by hash (cUIEngine_addScreenByHashName) into mpScreen and plays its frame 0; calls func_00256C50
 * (cGameComm destroy) when cGameComm_mGameComm is set; func_001B5268 selects menu row 0, clears D_004A1B80 and
 * calls func_0028FC58(gSSXAudio); finally func_0028FC38(gSSXAudio).
 */
extern "C" void cFEStateNetConfig_onCreateScreen(cFEStateNetConfig* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_200net_config), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    if (cGameComm_mGameComm != 0) {
        func_00256C50();
    }
    func_001B5268(self, 0);
    func_0028FC38(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onDestroyScreen);
#ifdef SKIP_ASM
void* cUIState_onDestroyScreen(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028FC58(void*);

/**
 * Closes the network-configuration screen: the cUIState handler, then func_0028FC58(gSSXAudio) (an audio bank
 * call).
 */
extern "C" void cFEStateNetConfig_onDestroyScreen(cFEStateNetConfig* self)
{
    cUIState_onDestroyScreen(self);
    func_0028FC58(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4198__FPv);
#ifdef SKIP_ASM
void func_001B4198(void* self)
{
    *(int*)((char*)self + 0x708) = 6;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onUpdate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self);
extern "C" void func_001B4888(cFEStateNetConfig* self, int a1, int a2, int a3);
extern "C" void func_001B5068(cFEStateNetConfig* self, int a1);
extern "C" void func_001B43A8(cFEStateNetConfig* self);
extern "C" void func_001B4470(cFEStateNetConfig* self);
extern "C" void func_001B45E8(void* self);
extern "C" int func_0039F698(void* p);
void func_002C67E8(void* self, int a1);
extern char* D_004A1B78_p __asm__("D_004A1B78");
extern int D_004A1B80;

struct sVEv1B41A8 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

struct sVE1B41A8 {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

static inline int VCall1B41A8(int slot)
{
    char* obj = D_004A1B78_p;
    sVE1B41A8* vt = *(sVE1B41A8**)obj;
    return vt[slot].fn(obj + vt[slot].delta);
}

/**
 * Runs one frame of the network-configuration state: updates the network object and steps the connect state
 * machine.
 *
 * Calls D_004A1B78's slot 3 each frame, then by miState: 1 -> 2; 2 (slot 11 idle) func_002C67E8(obj, D_004A1B80),
 * -> 3; 3 (idle) func_001B4470; 4 func_001B43A8; 5 counts miWaitFrames down and, when it runs out, either reports
 * error popup 0x842 (slot 53 returns 0: func_001B5068(self, -1), func_001B4888(self, 0x842, 0x18, D_004A1B80)) or
 * pops the state stack (func_0039F698 on mpEngine + 0x18), then returns to 0; while still waiting it falls through
 * to state 0, which (idle) calls the object's slot 5 with D_004A1B80. States 6 -> 7, 7 (idle) func_002C67E8, -> 8,
 * 8 (idle) func_001B45E8 (connect with the chosen setting). Ends with the cFEStateNet update (cFEStateNet_onUpdate).
 */
// Per-frame update for the online-login screen: steps the login state machine on the network object.
extern "C" void cFEStateNetConfig_onUpdate(cFEStateNetConfig* self)
{
    if (D_004A1B78_p != 0) {
        char* obj = D_004A1B78_p;
        sVEv1B41A8* vt = *(sVEv1B41A8**)obj;
        vt[3].fn(obj + vt[3].delta);
    }
    switch (self->miState) {
    case 1:
        self->miState = 2;
        break;
    case 2:
        if (VCall1B41A8(11) == 0) {
            func_002C67E8(D_004A1B78_p, D_004A1B80);
            self->miState = 3;
        }
        break;
    case 3:
        if (VCall1B41A8(11) == 0) {
            func_001B4470(self);
        }
        break;
    case 4:
        func_001B43A8(self);
        break;
    case 5:
        if ((self->miWaitFrames)-- < 0) {
            if (VCall1B41A8(53) == 0) {
                func_001B5068(self, -1);
                func_001B4888(self, 2114, 0x18, D_004A1B80);
            } else {
                func_0039F698((char*)self->mpEngine + 0x18);
            }
            self->miState = 0;
            break;
        }
    case 0:
        if (VCall1B41A8(11) == 0) {
            char* obj = D_004A1B78_p;
            sVEv1B41A8* vt = *(sVEv1B41A8**)obj;
            vt[5].fn(obj + vt[5].delta, D_004A1B80);
        }
        break;
    case 6:
        self->miState = 7;
        break;
    case 7:
        if (VCall1B41A8(11) == 0) {
            func_002C67E8(D_004A1B78_p, D_004A1B80);
            self->miState = 8;
        }
        break;
    case 8:
        if (VCall1B41A8(11) == 0) {
            func_001B45E8(self);
        }
        break;
    }
    cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B43A8);
#ifdef SKIP_ASM
// D_004A1B78 is declared earlier in the unit (cFEVObj_001B3F78*, see cFEStateNetConfig_cFEStateNetConfig)
extern "C" void func_001B4888(cFEStateNetConfig* self, int a1, int a2, int a3);
extern "C" void func_001B4CD8(cFEStateNetConfig* self);
extern "C" void func_001B5068(cFEStateNetConfig* self, int a1);

class cFEVObj_001B43A8 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual int v53();
    virtual void v54();
    virtual int v55();
};

extern "C" void func_001B43A8(cFEStateNetConfig* self)
{
    int a = ((cFEVObj_001B43A8*)D_004A1B78)->v53();
    if (((cFEVObj_001B43A8*)D_004A1B78)->v55() == 0) {
        self->miState = 0;
        func_001B5068(self, -1);
        func_001B4888(self, 2112, 20, -1);
    } else if (a != 0) {
        func_001B4CD8(self);
    } else {
        self->miState = 0;
        func_001B5068(self, -1);
        func_001B4888(self, 2112, 2, -1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4470);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A1B80;

extern "C" void func_001B47C0_4470(void* self, int a1) __asm__("func_001B47C0");

extern cFEVObj_001B3F78* D_004A1B78;

extern "C" void func_001B4FA0_4470(void* self, int err) __asm__("func_001B4FA0");

extern "C" void func_001B4470(cFEStateNetConfig* self_)
{
    int k;
    int e;
    if (((cFEVObj_001B43A8*)D_004A1B78)->v53() == 0) {
        k = D_004A1B80;
        if (k == 0) {
            D_004A1B80 = 1;
            func_001B47C0_4470(((char*)self_), 2);
            self_->miState = 2;
        } else {
            e = self_->miResult;
            self_->miState = 0;
            if (e != 1) {
                D_004A1B80 = (k + 1) % 2;
                func_001B4FA0_4470(((char*)self_), e);
            } else {
                func_001B5068(self_, -1);
                func_001B4888(self_, 2112, 2, -1);
            }
        }
    } else {
        if (((cFEVObj_001B43A8*)D_004A1B78)->v55() == 0) {
            k = D_004A1B80;
                if (k == 0) {
                self_->miResult = -200;
                D_004A1B80 = 1;
                func_001B47C0_4470(((char*)self_), 2);
                self_->miState = 2;
            } else {
                e = self_->miResult;
                self_->miState = 0;
                if (e != 1) {
                    D_004A1B80 = (k + 1) % 2;
                    func_001B4FA0_4470(((char*)self_), e);
                } else {
                    func_001B4888(self_, 2113, 20, 1);
                }
                func_001B5068(self_, -1);
            }
        } else {
            self_->miState = 4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B45E8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001B5068(cFEStateNetConfig* self, int a1);
void func_001C57C0(void* self, int a1, int a2);
extern "C" int* func_002591B8();
extern "C" void func_002636A8(void* self, void* a1);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...); the unit declares a 3-arg view
extern "C" void sprintf1_1B45E8(void* str, int fmt, ...) __asm__("sprintf1");
class cFEVObj_001B3F78;
extern cFEVObj_001B3F78* D_004A1B78;
extern int D_004A1B80;
extern char str_kT_POPUPCMNYourNetwork[];
extern char str_kT_MEMFENotDetectedFile[];
extern char str_kT_POPUP200ConnectingISP[];
extern char str_kT_CMNContinue[];
extern char str_kT_BTNCancel[];

struct sVEntry_001B45E8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1B45E8(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B45E8* vt = *(sVEntry_001B45E8**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

class cFEVObj_001B45E8 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual int v53();
};

extern "C" void func_001B45E8(void* self)
{
    if (((cFEVObj_001B45E8*)D_004A1B78)->v53() == 0) {
        *(int*)((char*)self + 0x708) = 0;
        func_001B5068((cFEStateNetConfig*)self, -1);
        char* buf = (char*)self + 0x71C;
        void* p = func_001A9C50((cFEStateNet*)self);
        int name = Loc1B45E8(str_kT_POPUPCMNYourNetwork);
        sprintf1_1B45E8(buf, Loc1B45E8(str_kT_MEMFENotDetectedFile), D_004A1B80 + 1, name);
        *(char**)((char*)p + 0xC4) = buf;
        *(int*)((char*)p + 0xC0) = 1;
        *(int*)((char*)p + 0xBC) = 1;
        func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(str_kT_CMNContinue));
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 2115;
        func_001A9BC0((cFEStateNet*)self, p);
    } else {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUP200ConnectingISP);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 2062;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNCancel));
        func_001A9BC0((cFEStateNet*)self, p);
        func_002636A8(func_002591B8(), *(void**)((char*)self + 0x6D0));
        *(int*)((char*)self + 0x708) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B47C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void usprintf(char* buf, const char* fmt, int a2);
extern "C" void sprintf1(void* str, int a1, char* buf);
extern char str_pct_d_b[];
extern char str_kT_MEMFECheckingCardPS2[];

struct sVE_001B47C0 {
    short delta;
    short index;
    int (*fn)(void* self, int hash);
};

extern "C" void func_001B47C0(cFEStateNetConfig* self, int cardSlot)
{
    char buf[32];
    char* str = (char*)self + 0x71C;
    usprintf(buf, str_pct_d_b, cardSlot);
    char* locale = (char*)SSXApp->FELocale;
    sVE_001B47C0* vt = *(sVE_001B47C0**)(locale + 4);
    char* obj = locale + vt[4].delta;
    sprintf1(str, vt[4].fn(obj, GetHashValue32(str_kT_MEMFECheckingCardPS2)), buf);
    void* popup = func_001A9C50((cFEStateNet*)self);
    // stores
    *(int*)((char*)popup + 0xBC) = 0;
    *(char**)((char*)popup + 0xC4) = str;
    *(int*)((char*)popup + 0xC0) = 1;
    *(int*)((char*)popup + 0x154) = 1;
    *(int*)((char*)popup + 0x150) = 2111;
    *(int*)((char*)popup + 0x164) = 0;
    *(int*)((char*)popup + 0x168) = 0;
    // end
    func_001A9BC0((cFEStateNet*)self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4888);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
// PORT: sprintf1 / usprintf are varargs formatters (dst, fmt, ...); the unit declares 3-arg views
extern "C" void sprintf1_1B4888(void* str, int fmt, ...) __asm__("sprintf1");
extern "C" void usprintf_1B4888(char* buf, const char* fmt, ...) __asm__("usprintf");
extern char str_pct_d_b[];
extern char str_kT_MEMOVNotFormattedPS2[];
extern char str_kT_MEMOVRetry[];
extern char str_kT_POPUPCMNYourNetwork[];
extern char str_kT_MEMFENotDetectedFile[];
extern char str_kT_MEMFECorruptPS2[];
extern char str_kT_POPUP200DifferentPS2[];
extern char str_kT_CMNContinue[];
extern char str_kT_BTNCancel[];

struct sVEntry_001B4888 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1B4888(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B4888* vt = *(sVEntry_001B4888**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

extern "C" void func_001B4888(cFEStateNetConfig* self, int popupId, int cardError, int cardSlot)
{
    char buf[0x20];
    int flag = 0;
    void* popup = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)popup + 0xBC;
    switch (cardError) {
    case 2:
        if (cardSlot == -1) {
            char* dst = (char*)self + 0x71C;
            cardSlot = 1;
            int name = Loc1B4888(str_kT_POPUPCMNYourNetwork);
            sprintf1_1B4888(dst, Loc1B4888(str_kT_MEMFENotDetectedFile), cardSlot, name);
            *(char**)((char*)popup + 0xC4) = dst;
            *(int*)((char*)popup + 0xC0) = cardSlot;
        } else {
            char* dst = (char*)self + 0x71C;
            int name = Loc1B4888(str_kT_POPUPCMNYourNetwork);
            sprintf1_1B4888(dst, Loc1B4888(str_kT_MEMFENotDetectedFile), cardSlot + 1, name);
            *(char**)((char*)popup + 0xC4) = dst;
            *(int*)((char*)popup + 0xC0) = 1;
            flag = 1;
        }
        break;
    case 20: {
        usprintf_1B4888(buf, str_pct_d_b, cardSlot + 1);
        char* dst = (char*)self + 0x71C;
        sprintf1_1B4888(dst, Loc1B4888(str_kT_MEMOVNotFormattedPS2), buf);
        *(char**)((char*)popup + 0xC4) = dst;
        *(int*)((char*)popup + 0xC0) = 1;
        break;
    }
    case 0x18: {
        usprintf_1B4888(buf, str_pct_d_b, cardSlot + 1);
        char* dst = (char*)self + 0x71C;
        int name = Loc1B4888(str_kT_POPUPCMNYourNetwork);
        sprintf1_1B4888(dst, Loc1B4888(str_kT_MEMFECorruptPS2), name, buf);
        *(char**)((char*)popup + 0xC4) = dst;
        *(int*)((char*)popup + 0xC0) = 1;
        break;
    }
    case 6:
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP200DifferentPS2);
        *(int*)((char*)popup + 0xC0) = 3;
        break;
    }
    *(int*)sub = 2;
    func_001C57C0(sub, 0, GetHashValue32(str_kT_MEMOVRetry));
    if (flag == 0) {
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNContinue));
    } else {
        func_001C57C0(sub, 1, GetHashValue32(str_kT_BTNCancel));
    }
    *(int*)(sub + 0x94) = popupId;
    *(int*)(sub + 0xA8) = 0;
    func_001A9BC0((cFEStateNet*)self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4C00);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_001B4C00(cFEStateNetConfig* self, void* popup, int event)
{
    int result;
    if (event == 0xF || event == 20) {
        result = func_001A97B8(self, popup, event);
    } else {
        result = func_001A97B8(self, popup, event);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4C40);
#ifdef SKIP_ASM
extern int D_004A1B80;
extern "C" void func_001B47C0(cFEStateNetConfig* self, int a1);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001B4C40(cFEStateNetConfig* self, void* a1, unsigned int a2)
{
    switch (a2) {
    case 15:
        D_004A1B80 = 0;
        self->miResult = 1;
        func_001B47C0(self, 1);
        self->miState = 1;
        break;
    case 16:
    case 20:
        func_001A97B8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4CD8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_001B47C0(cFEStateNetConfig* self, int a1);
extern "C" void func_001B4FA0(cFEStateNetConfig* self, int err);
extern "C" void func_001B5068(cFEStateNetConfig* self, int a1);
extern "C" int func_00255F20(void* self, int a1);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" void* cUIListBox_getFont(void* box);
extern "C" void cUIListBox_addEntryByUnicodeString(void* box, unsigned short* str, int id);
extern "C" void func_003EFA48(unsigned short* dst, int n, const char* src);
extern "C" int func_004165A8(const char* a, const char* b);
extern int D_004A1B80;
extern int D_004A1B90[2];
extern int cCommSystem_mNumNetConn;
extern char* D_004A1B7C;
extern char str_LBSettings[];
extern char str_pct_d_c[];
// the 'netw'/'list' reply buffer filled by func_00255F20 (10 entries of 0x100 bytes, count in cCommSystem_mNumNetConn)
struct sNetEntry_4CD8 { int f0; char name4[0x3E]; char name42[0xBE]; };
extern sNetEntry_4CD8 cCommSystem_mNetConns[10];

struct sVec3_001B4CD8 {
    float x, y, z;
};

struct sVec4_001B4CD8 {
    float x, y, z, w;
};

class cFEVObj_001B4CD8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20(void* out);
};

extern "C" void func_001B4CD8(cFEStateNetConfig* self)
{
    char buf[64];
    unsigned short name[64];
    unsigned short line[64];
    sVec4_001B4CD8 size;
    sVec3_001B4CD8 scale;
    sVec4_001B4CD8 pos;
    func_00255F20(cCommSystem_mCommSystem, D_004A1B90[D_004A1B80]);
    self->miNumSettings = cCommSystem_mNumNetConn;
    if (self->miNumSettings > 4) {
        self->miNumSettings = 4;
    }
    if (self->miNumSettings <= 0) {
        if (D_004A1B80 == 0) {
            D_004A1B80 = 1;
            func_001B47C0(self, 2);
            self->miState = 1;
            self->miResult = self->miNumSettings;
            return;
        }
        if (self->miNumSettings != 0) {
            if (self->miResult == 0) {
                D_004A1B80 = 0;
                self->miState = 5;
                self->miWaitFrames = 15;
                return;
            }
            func_001B4FA0(self, self->miNumSettings);
            self->miState = 0;
            return;
        }
    } else {
        ((cFEVObj_001B4CD8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBSettings)))->v20(&pos);
        scale = *(sVec3_001B4CD8*)((char*)self->mpLBSettings + 0x50);
        if (self->miNumSettings > 4) {
            self->miNumSettings = 4;
        }
        for (int i = 0; i < self->miNumSettings; i++) {
            char* nm = cCommSystem_mNetConns[i].name42;
            float w;
            int len;
            if (self->miNumSettings >= 2) {
                len = sprintf(buf, str_pct_d_c, i + 1);
                w = pos.x - cFont_textExtent(cUIListBox_getFont(self->mpLBSettings), buf, &size, 1, scale.x, scale.y);
            } else {
                w = pos.x;
                len = 0;
                buf[0] = 0;
            }
            char* rec = cCommSystem_mNetConns[i].name4;
            func_003EFA48(name, 0x40, nm);
            UIAsciiToUnicode(line, buf);
            func_00392680(cUIListBox_getFont(self->mpLBSettings), scale.x, w, name, line + len, 0x40 - len);
            cUIListBox_addEntryByUnicodeString(self->mpLBSettings, line, i + 20);
            if (func_004165A8(D_004A1B7C, rec) == 0) {
                cUIListBox_addEntryByAsciiString(self->mpLBAdapters, rec, i + 0x1E);
            } else {
                cUIListBox_addEntryByAsciiString(self->mpLBAdapters, rec, i + 0x1E);
            }
        }
        func_001B5068(self, self->miNumSettings);
    }
    self->miState = 5;
    self->miWaitFrames = 15;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4FA0);
#ifdef SKIP_ASM
extern "C" void func_001B4888(cFEStateNetConfig* self, int a1, int a2, int a3);
extern "C" void func_001B5068(cFEStateNetConfig* self, int a1);
extern "C" int func_0039F698(void* p);
extern int D_004A1B80;

extern "C" void func_001B4FA0(cFEStateNetConfig* self, int err)
{
    switch (err) {
    case 0:
        func_0039F698((char*)self->mpEngine + 0x18);
        break;
    // PORT: GNU case ranges
    case -21 ... -16:
    case -14 ... -1:
        func_001B4888(self, 2114, 0x18, D_004A1B80);
        break;
    case -15:
        func_001B4888(self, 2113, 6, -1);
        break;
    case -200:
        func_001B4888(self, 2113, 20, 0);
        break;
    }
    func_001B5068(self, -1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5068);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void func_0039A670(void* self);

struct sCol1B5068 {
    float r, g, b, a;
};
extern sCol1B5068 D_004C66C8;

class cFEVObj_001B5068 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sCol1B5068& c);
};

extern "C" void func_001B5068(cFEStateNetConfig* self, int a1)
{
    if (a1 <= 0) {
        func_0039A670(((cFEVObj_001B5068*)self->mpLBAdapters));
        func_0039A670(((cFEVObj_001B5068*)self->mpLBSettings));
        ((cFEVObj_001B5068*)self->mpLBSettings)->v08(1);
        ((cFEVObj_001B5068*)self->mpLBAdapters)->v08(1);
        ((cFEVObj_001B5068*)self->mpPairSettings)->v08(1);
        ((cFEVObj_001B5068*)self->mpLBSettings)->v09(0);
        ((cFEVObj_001B5068*)self->mpLBAdapters)->v09(0);
        ((cFEVObj_001B5068*)self->mpConfigRectangle)->v09(0);
        ((cFEVObj_001B5068*)self->mpNoConfiguration)->v09(1);
        ((cFEVObj_001B5068*)self->mpNoConfiguration)->v11(D_004C66C8);
        cUIMenu_setSelectedByIndex(self->mpMenuConfig, 0);
    } else {
        ((cFEVObj_001B5068*)self->mpPairSettings)->v08(0);
        ((cFEVObj_001B5068*)self->mpLBSettings)->v08(0);
        ((cFEVObj_001B5068*)self->mpLBAdapters)->v08(0);
        ((cFEVObj_001B5068*)self->mpLBSettings)->v09(1);
        ((cFEVObj_001B5068*)self->mpLBAdapters)->v09(1);
        ((cFEVObj_001B5068*)self->mpConfigRectangle)->v09(1);
        ((cFEVObj_001B5068*)self->mpNoConfiguration)->v09(0);
        cUIMenu_setSelectedByIndex(self->mpMenuConfig, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5268);
#ifdef SKIP_ASM
extern int D_004A1B80;
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void func_0039A670(void* self);
extern "C" void func_001B47C0(cFEStateNetConfig* self, int a1);


extern "C" void func_001B5268(cFEStateNetConfig* obj, int a1)
{
    cFEStateNetConfig* self = obj;
    cUIMenu_setSelectedByIndex(((void*)self->mpMenuConfig), 0);
    D_004A1B80 = 0;
    self->miResult = 1;
    func_0039A670(((void*)self->mpLBAdapters));
    func_0039A670(((void*)self->mpLBSettings));
    func_001B47C0(obj, 1);
    self->miState = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern char str_TextCreateConfig[];
extern char str_TextTitle[];
extern char str_TextSettings[];
extern char str_TextInstructions[];
extern char str_LBSettings[];
extern char str_configrectangle1[];
extern char str_NoConfiguration[];
extern char str_LBAdapters[];
extern char str_MenuConfig[];
extern char str_PairSettings[];

class cFEVObj_001B52C8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int on);
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24(int on);
};

static inline int IsHash1B52C8(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Binds the network-configuration screen's widgets as the screen creates them.
 *
 * By hash name: "TextCreateConfig" gets id 1 (slots 9 and 6 with 1), "TextTitle", "TextSettings" and
 * "TextInstructions" id -1; "LBSettings" (id 100) is mpLBSettings and "LBAdapters" (id 101) mpLBAdapters, both with
 * slot 24(1); "configrectangle1" is mpConfigRectangle, "NoConfiguration" mpNoConfiguration, "MenuConfig"
 * mpMenuConfig (id -1, attribute bit 0x80) and "PairSettings" mpPairSettings. The id is the widget's mMiscData
 * (+0x18).
 *
 * @param item  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateNetConfig_onWidgetCreate(cFEStateNetConfig* self, cUIText* item)
{
    cFEVObj_001B52C8* obj = (cFEVObj_001B52C8*)item;
    if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_TextCreateConfig)) {
        *(int*)((char*)item + 0x18) = 1;
        obj->v09(1);
        obj->v06(1);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_TextTitle)) {
        *(int*)((char*)item + 0x18) = -1;
        obj->v09(1);
        obj->v06(0);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_TextSettings)) {
        *(int*)((char*)item + 0x18) = -1;
        obj->v09(0);
        obj->v06(0);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_TextInstructions)) {
        *(int*)((char*)item + 0x18) = -1;
        obj->v09(1);
        obj->v06(0);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_LBSettings)) {
        *(int*)((char*)item + 0x18) = 100;
        self->mpLBSettings = (cUIListBox*)item;
        obj->v24(1);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_configrectangle1)) {
        self->mpConfigRectangle = (cUIThing*)item;
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_NoConfiguration)) {
        self->mpNoConfiguration = (cUIThing*)item;
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_LBAdapters)) {
        *(int*)((char*)item + 0x18) = 101;
        self->mpLBAdapters = (cUIListBox*)item;
        obj->v24(1);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_MenuConfig)) {
        self->mpMenuConfig = (cUIMenu*)item;
        *(int*)((char*)item + 0x18) = -1;
        *(int*)((char*)item + 0x14) |= 0x80;
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), str_PairSettings)) {
        self->mpPairSettings = (cUIThing*)item;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B54B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP200NetworkConfig[];
extern char str_kT_HELP200NetworkConfigMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELP200Refresh[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001B54B8(cFEStateNetConfig* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP200NetworkConfig);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP200NetworkConfigMessage));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(menu, 3, GetHashValue32(str_kT_HELP200Refresh), 2);
    func_001DA510(menu, 4, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B55B0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
struct sFEObj_001B6288;
extern "C" sFEObj_001B6288* cFEStateAuthDNAS_cFEStateAuthDNAS(cFEStateAuthDNAS* self, int a1, int a2);
extern char str_NetConfigAuthPopup[];

class cFEVObj_001B55B0 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

static inline void* GetSession_001B55B0()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001B55B0(cFEStateNetConfig* self, void* a1, unsigned int kind)
{
    switch (kind) {
    case 15:
    case 20: {
        cFEVObj_001B55B0* o = (cFEVObj_001B55B0*)self->mpPairSettings;
        self->unk_6D8 = 1;
        o->v08(0);
        func_00261408(GetSession_001B55B0());
        func_0025B800(LobbyMan_mLobbyMan);
        func_002636F0(func_002591B8(), 0);
        func_001A97B8(self, a1, kind);
        break;
    }
    case 22:
        if (self->unk_6D8 == 0) {
            char* p = (char*)cFEStateAuthDNAS_cFEStateAuthDNAS((cFEStateAuthDNAS*)((sFEObj_001B6288*)cMemMan_alloc(0x700, str_NetConfigAuthPopup, 0, 0)), (int)self->mpEngine, (int)self);
            *(int*)(p + 0x18) = 2074;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        }
        self->unk_6D8 = 0;
        func_001A97B8(self, a1, 22);
        break;
    default:
        func_001A97B8(self, a1, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5740);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);

class cFEVObj_001B5740 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};

static inline void* GetSession_001B5740()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001B5740(cFEStateNetConfig* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 20) {
        func_00261408(GetSession_001B5740());
        func_0025B800(LobbyMan_mLobbyMan);
        func_002636F0(func_002591B8(), 0);
        ((cFEVObj_001B5740*)self->mpPairSettings)->v08(0);
        func_001A97B8(self, a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5830);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
// PORT: cFEStateAuthDNASError_cFEStateAuthDNASError is declared later in the unit with an int owner parameter
extern "C" void* cFEStateAuthDNASError_cFEStateAuthDNASError(cFEStateAuthDNASError* mem, int a1, int a2, int a3, int a4, int a5);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern char str_cFEStateAuthDNASError[];
extern char str_cFEStateNetAccountMem[];

static inline void* GetSession_001B5830()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

class cFEVObj_001B5830 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

extern "C" void func_001B5830(cFEStateNetConfig* self, void* a1, unsigned int kind)
{
    switch (kind) {
    case 15:
        func_001A97B8_v(self, a1, 15);
        self->unk_700 = 0;
        break;
    case 16:
        self->unk_700 = 1;
        func_001A97B8_v(self, a1, 16);
        break;
    case 20:
        func_00261408(GetSession_001B5830());
        func_0025B800(LobbyMan_mLobbyMan);
        func_002636F0(func_002591B8(), 0);
        self->unk_6D8 = 1;
        ((cFEVObj_001B5830*)self->mpPairSettings)->v08(0);
        func_001A97B8_v(self, a1, 20);
        break;
    case 22:
        func_001A97B8_v(self, a1, 22);
        if (self->unk_700 != 0) {
            void* p = cFEStateAuthDNASError_cFEStateAuthDNASError((cFEStateAuthDNASError*)cMemMan_alloc(1768, str_cFEStateAuthDNASError, 0, 0), (int)self->mpEngine, (int)self,
                                    (int)self->mpustrAuthError[1], (int)self->mpustrAuthError[2], (int)self->mpustrAuthError[0]);
            *(int*)((char*)p + 0x18) = 2075;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        } else if (self->unk_6D8 == 0) {
            void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, self, 0, 0, 0);
            *(int*)((char*)p + 0x18) = 2092;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        }
        self->unk_700 = 0;
        self->unk_6D8 = 0;
        break;
    default:
        func_001A97B8_v(self, a1, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5A58);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
// PORT: the unit declares func_001A97B8 as returning int; this caller ignores the result.
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* cFEStateLogin_cFEStateLogin(cFEStateLogin* mem, int a1);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern const char str_cFEStateLogin[];

struct sVE1B5A58 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// Tracks a pending flag; on 0x10/0x14 restarts the memory-card task and resets the list.
extern "C" void func_001B5A58(cFEStateNetConfig* self, void* a1, int a2)
{
    switch (a2) {
    case 21:
        self->unk_6DC = 0;
        func_001A97B8_v(self, a1, a2);
        break;
    case 0xF:
        func_001A97B8_v(self, a1, a2);
        self->unk_6DC = 1;
        break;
    case 0x10: case 20: {
        func_001A97B8_v(self, a1, a2);
        if (BuddyMan_mBuddyMan == 0) {
            func_00260F80();
        }
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            func_002613C8(BuddyMan_mBuddyMan);
        }
        if (BuddyMan_mBuddyMan == 0) {
            func_0025B800(LobbyMan_mLobbyMan);
        }
        func_00261408(BuddyMan_mBuddyMan);
        func_0025B800(LobbyMan_mLobbyMan);
        func_002636F0(func_002591B8(), 0);
        char* obj = (char*)self->mpPairSettings;
        sVE1B5A58* vt = *(sVE1B5A58**)(obj + 8);
        vt[8].fn(obj + vt[8].delta, 0);
        break;
    }
    case 22:
        if (self->unk_6DC != 0) {
            func_001A8A40(self, cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(1748, str_cFEStateLogin, 0, 0), (int)self->mpEngine));
        }
        func_001A97B8_v(self, a1, a2);
        break;
    default:
        func_001A97B8_v(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5BD8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_FrontEndPurge(void* p);
extern "C" void func_00256088();
extern char str_TextCreateConfig[];

struct sColor1B5BD8 {
    float r, g, b, a;
    sColor1B5BD8(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cFEVObj_001B5BD8 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int on);
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11(const sColor1B5BD8& c);
};

struct sFlags1B5BD8 {
    unsigned int f0 : 8;
    unsigned int mode : 6;
};

extern "C" void func_001B5BD8(cFEStateNetConfig* self, void* a1, unsigned int kind)
{
    switch (kind) {
    case 15: {
        func_001A97B8(((cFEStateNet*)self), a1, 15);
        cSSXAudio_FrontEndPurge(func_0028B180());
        func_00256088();
        cFEVObj_001B5BD8* o = (cFEVObj_001B5BD8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextCreateConfig));
        if (o != 0) {
            o->v06(0);
            o->v11(sColor1B5BD8(1.0f, 0.5f, 0.5f, 0.5f));
        }
        ((sFlags1B5BD8*)((char*)self + 0x1C))->mode = 7;
        break;
    }
    case 16:
    case 20:
        func_001A97B8(((cFEStateNet*)self), a1, kind);
        break;
    default:
        func_001A97B8(((cFEStateNet*)self), a1, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5CF8);
#ifdef SKIP_ASM
class cFEVObj_001B5CF8 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001B5CF8(cFEStateNetConfig* self, void* a1, int a2)
{
    if (a2 == 22) {
        func_001A97B8(self, a1, a2);
        ((cFEVObj_001B5CF8*)self->mpPairSettings)->v08(0);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onPopupEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_001B5A58(cFEStateNetConfig* self, void* a1, int a2);
/**
 * Hands a popup's event to the handler for that popup's message id, else to the cFEStateNet handler.
 *
 * The id is the popup's miMiscData (+0x18): 0x81C func_001B5CF8, 0x80E func_001B55B0, 0x81B func_001B5740, 0x81A
 * func_001B5830, 0x82C func_001B5A58, 0x840-0x842 (the connect error popups) func_001B4C40, 0x843 func_001B4C00,
 * 0x844 (the create/edit question) func_001B5BD8; any other func_001A97B8.
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event; 0xF + i option i chosen, 0x14 back, 0x16 closed)
 */
// Dispatches a message to the matching handler, falling back to the base handler.
extern "C" void cFEStateNetConfig_onPopupEvent(cFEStateNetConfig* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2076:
        func_001B5CF8(self, pPopup, Event);
        break;
    case 2062:
        func_001B55B0(self, pPopup, Event);
        break;
    case 2075:
        func_001B5740(self, pPopup, Event);
        break;
    case 2074:
        func_001B5830(self, pPopup, Event);
        break;
    case 2092:
        func_001B5A58(self, pPopup, Event);
        break;
    case 2112: case 2113: case 2114:
        func_001B4C40(self, pPopup, Event);
        break;
    case 2115:
        func_001B4C00(self, pPopup, Event);
        break;
    case 2116:
        func_001B5BD8(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B5E28);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void USTR_copy(void* dst, void* src);
extern "C" int USTR_length(void* s);
void cMemMan_free(void* p);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" int func_001AAF90(cFEStateNet* self, void* msg);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void strcpy1(void* dst, int src);
extern "C" void func_0039F190(void* self, int a1);
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int func_001A8E40_3i(void* self, int a1, int a2) __asm__("func_001A8E40");
extern char str_kT_POPUP200WarningCommProblem[];
extern char str_kT_POPUP200WarningProblemISP[];
extern char str_disconnected_from_serverErrorString[];
extern char str_kT_BTNOK[];

struct sVEntry_001B5E28 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1B5E28(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B5E28* vt = *(sVEntry_001B5E28**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

// PORT: a2 is a message pointer passed as int
extern "C" int func_001B5E28(cFEStateNetConfig* self, int id, int msgPtr)
{
    char* msg = (char*)msgPtr;
    if (id == 261) {
        if (self->mAttrState & 6) {
            func_0039F190((char*)self->mpEngine + 0x18, 1);
        }
    } else {
        if (id >= 261) {
            if (id < 279) {
                if (id >= 277) {
                    int type = *(int*)(msg + 8);
                    if (type != 0) {
                        int str;
                        switch (type) {
                        case 0x2D646576:
                        case 0x2D617468:
                        case 0x2D636667:
                        case 0x2D696E69:
                        case 0x2D646966:
                        case 0x2D746F6E:
                            str = Loc1B5E28(str_kT_POPUP200WarningCommProblem);
                            break;
                        default:
                            str = Loc1B5E28(str_kT_POPUP200WarningProblemISP);
                            break;
                        }
                        if (self->mpustrPopupText != 0) {
                            cMemMan_free(self->mpustrPopupText);
                            self->mpustrPopupText = 0;
                        }
                        void* errText = (void*)func_001AAF90((cFEStateNet*)self, msg);
                        if (errText != 0) {
                            self->mpustrPopupText = (unsigned short*)operator_new_tag((USTR_length(errText) + 1) * 2, str_disconnected_from_serverErrorString, 0x100, 0);
                            USTR_copy(self->mpustrPopupText, errText);
                        } else {
                            self->mpustrPopupText = (unsigned short*)operator_new_tag((USTR_length((void*)str) + 1) * 2, str_disconnected_from_serverErrorString, 0x100, 0);
                            strcpy1(self->mpustrPopupText, str);
                        }
                        int one = 1;
                        void* popup = func_001A9C50((cFEStateNet*)self);
                        *(int*)((char*)popup + 0xBC) = one;
                        char* sub = (char*)popup + 0xBC;
                        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
                        *(void**)((char*)popup + 0xC4) = (void*)self->mpustrPopupText;
                        *(int*)((char*)popup + 0x150) = 2076;
                        *(int*)((char*)popup + 0xC0) = one;
                        func_001A9BC0((cFEStateNet*)self, popup);
                    }
                    return 0;
                }
            }
        }
        return func_001A8E40_3i(self, id, msgPtr);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onWidgetPair__FPv);
#ifdef SKIP_ASM
/**
 * Answers a pair widget's event on the network-configuration screen: always 0x101 (the default result).
 *
 * @return always 0x101
 */
int cFEStateNetConfig_onWidgetPair(void* self)
{
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onWidgetListBox);
#ifdef SKIP_ASM
/**
 * Answers a list box event on the network-configuration screen: blocks scrolling with fewer than 2 settings.
 *
 * @param pListBox  the list box (On Tour pListBox); not used
 * @param key  the list box event; 0 and 1 (scrolling) are refused when miNumSettings <= 1
 * @return 0 for a refused scroll, otherwise 0x101
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cFEStateNetConfig_onWidgetListBox(cFEStateNetConfig* self, int pListBox, unsigned int key)
{
    int result = 257;
    if (key < 2) {
        if (self->miNumSettings > 1) {
            result = 257;
        } else {
            return 0;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00194A60(void* mem, int a1);
extern "C" void func_001A86E8(int on);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001B4198(void* self);
extern "C" void func_001B5268(cFEStateNetConfig* self, int a1);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char str_kT_POPUP200CreateEdit[];
extern char str_LBSettings[];
extern const char str_cFEStateMainMenu[];

class cFEVObj_001B60A8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

static inline int IsHash1B60A8(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Handles a widget event on the network-configuration screen: connect, create/edit, pick a setting, refresh or
 * back.
 *
 * Event 5 (pressed) by widget id (+0x18): 0 enables mpPairSettings (slot 8) and starts the connect (func_001B4198,
 * miState 6); 1 ("TextCreateConfig") opens a Yes/No popup "kT_POPUP200CreateEdit" (id 0x844, default option 1).
 * Event 9 on "LBSettings" stores its row (+0x319) in miSelectedSetting and selects it in mpLBAdapters; event 8
 * calls func_001B5268 (menu row 0); event 6 sets the FE path offline (func_001A86E8(0)) and switches to a new
 * cFEStateMainMenu (func_00194A60). Others go to the cFEStateNet handler (cFEStateNet_onWidgetEvent). A null widget is ignored.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void cFEStateNetConfig_onWidgetEvent(cFEStateNetConfig* self, int pWidget, unsigned int msg)
{
    char* obj = (char*)pWidget;
    if (obj == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        int miscData = *(int*)(obj + 0x18);
        switch (miscData) {
        case 0:
            ((cFEVObj_001B60A8*)self->mpPairSettings)->v08(1);
            func_001B4198(self);
            break;
        case 1: {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 2;
            char* sub = (char*)popup + 0xBC;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP200CreateEdit);
            *(int*)((char*)popup + 0xC0) = 3;
            *(int*)((char*)popup + 0x150) = 2116;
            *(int*)((char*)popup + 0x168) = 1;
            func_001A9BC0((cFEStateNet*)self, popup);
            break;
        }
        }
        break;
    }
    case 9:
        if (IsHash1B60A8(*(int*)(obj + 0x38), str_LBSettings)) {
            self->miSelectedSetting = *(unsigned char*)(obj + 0x319);
            cUIListBox_setCurrentEntryByIndex(self->mpLBAdapters, (unsigned char)self->miSelectedSetting);
        }
        break;
    case 8:
        func_001B5268(self, 0);
        break;
    case 6:
        func_001A86E8(0);
        func_001A8A40(self, func_00194A60(cMemMan_alloc(88, str_cFEStateMainMenu, 0, 0), (int)self->mpEngine));
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_cFEStateAuthDNAS);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void cMemMan_free(void* p);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int USTR_length(void* s);
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern void* cFEStateAuthDNAS_vtbl[];
extern char str_kT_205Authenticating[];
extern char str_cFEStateAuthDNAS[];

struct sVE_001B6288 {
    short delta;
    short index;
    void* (*fn)(void* self, int hash);
};

struct sFEObj_001B6288 {
    char pad0[0x8];
    void** vt;          // 0x8
    int fC;             // 0xC
    char pad10[0x6A8 - 0x10];
    int f6A8;           // 0x6A8
    char pad6AC[0x6D0 - 0x6AC];
    int f6D0;           // 0x6D0
    int f6D4;           // 0x6D4
    char pad6D8[0x6E4 - 0x6D8];
    int f6E4;           // 0x6E4
    char pad6E8[0x6F4 - 0x6E8];
    int f6F4;           // 0x6F4
    int f6F8;           // 0x6F8
    void* f6FC;         // 0x6FC
};

/**
 * Constructs the DNAS authentication state.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with the engine and parent, sets unk_6F4 2, unk_6F8 0x1E,
 * cFEStateAuthDNAS_vtbl and mHashName 0x44, clears mpAuth, mpTextAuth, miState and mpustrText, then allocates
 * mpustrText (tag "cFEStateAuthDNAS") with room for the localised "kT_205Authenticating" text plus 5 characters, and
 * clears unk_6A8 (net messages are not filtered here).
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @param pParentState  the parent state that receives the result (On Tour pParentState)
 * @return @p self
 */
extern "C" sFEObj_001B6288* cFEStateAuthDNAS_cFEStateAuthDNAS(cFEStateAuthDNAS* self, int pEngine, int pParentState)
{
    func_001A8500_3(((sFEObj_001B6288*)self), pEngine, pParentState);
    self->unk_6F4 = 2;
    self->mVtbl = cFEStateAuthDNAS_vtbl;
    self->unk_6F8 = 0x1E;
    self->mHashName = 68;
    self->mpAuth = 0;
    self->mpTextAuth = 0;
    self->miState = 0;
    self->mpustrText = 0;
    char* locale = (char*)SSXApp->FELocale;
    sVE_001B6288* vt = *(sVE_001B6288**)(locale + 4);
    int n = USTR_length(vt[4].fn(locale + vt[4].delta, GetHashValue32(str_kT_205Authenticating)));
    if (self->mpustrText != 0) {
        cMemMan_free(self->mpustrText);
        self->mpustrText = 0;
    }
    self->mpustrText = (unsigned short*)operator_new_tag((n + 5) * 2, str_cFEStateAuthDNAS, 0x100, 0);
    self->unk_6A8 = 0;
    return ((sFEObj_001B6288*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS__cFEStateAuthDNAS);
#ifdef SKIP_ASM
extern void* cFEStateAuthDNAS_vtbl[];
void cMemMan_free(void* p);
extern "C" void func_002563A8(void* p);
extern "C" void func_002561E0(void* p, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the DNAS authentication state: shuts down and deletes the DNAS object and frees mpustrText.
 *
 * The DNAS object (mpAuth) is shut down with func_002563A8 and deleted through func_002561E0(.., 3); then the
 * cFEStateNet destructor (func_001A85D0) runs with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateAuthDNAS__cFEStateAuthDNAS(cFEStateAuthDNAS* self, int flags)
{
    self->mVtbl = cFEStateAuthDNAS_vtbl;
    if (self->mpAuth != 0) {
        func_002563A8(self->mpAuth);
        if (self->mpAuth != 0) {
            func_002561E0(self->mpAuth, 3);
        }
    }
    if (self->mpustrText != 0) {
        cMemMan_free(self->mpustrText);
        self->mpustrText = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* func_002561B8(void* mem);
extern "C" void func_00256258(void* self, int mode);
extern char str_205AuthenticatingDNAS[];
extern char str_dnasGroup[];
extern char str_cFEStateAuthDNASCreate[];

struct sProfile1B63E8 {
    unsigned int flags;
    char rest[0x284];
};
extern sProfile1B63E8 cBEOptionInterface_mOption;

class cFEVObj_001B63E8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

/**
 * Creates the "205AuthenticatingDNAS" screen and starts the DNAS authentication object.
 *
 * Hides "dnasGroup", creates mpAuth (func_002561B8 on a 0xA50-byte "cFEStateAuthDNASCreate" block), sets miState 1
 * and starts it (func_00256258) in the mode held in bits 22-24 of the profile word cBEOptionInterface_mOption (0-3; other values use
 * 0).
 */
extern "C" void cFEStateAuthDNAS_onCreateScreen(cFEStateAuthDNAS* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_205AuthenticatingDNAS), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        cFEVObj_001B63E8* widget = (cFEVObj_001B63E8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_dnasGroup));
        if (widget != 0) {
            widget->v09(0);
        }
    }
    self->mpAuth = func_002561B8(cMemMan_alloc(2640, str_cFEStateAuthDNASCreate, 0x100, 0));
    self->miState = 1;
    cBE_getInterface_Fv(cBE_getBE(), 4);
    int mode = 0;
    sProfile1B63E8 prof = cBEOptionInterface_mOption;
    switch ((prof.flags >> 22) & 7) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 1;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 3;
        break;
    }
    func_00256258(self->mpAuth, mode);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_TextCancel[];
extern char str_TextAuth[];
extern char str_kT_ovrcmnloading[];
extern char str_XButton[];
extern char str_submitrect[];

class cFEVObj_001B65C0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash1B65C0(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Binds the DNAS screen's widgets: "TextCancel", "XButton" and "submitrect" are kept and hidden.
 *
 * "TextAuth" becomes mpTextAuth and shows "kT_ovrcmnloading" until authentication starts.
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateAuthDNAS_onWidgetCreate(cFEStateAuthDNAS* self, cUIText* text)
{
    cFEVObj_001B65C0* obj = (cFEVObj_001B65C0*)text;
    if (IsHash1B65C0(*(int*)((char*)text + 0x38), str_TextCancel)) {
        self->mpTextCancel = text;
        obj->v09(0);
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), str_TextAuth)) {
        self->mpTextAuth = text;
        cUIText_setUnicodeStringByID(self->mpTextAuth, GetHashValue32(str_kT_ovrcmnloading));
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), str_XButton)) {
        self->mpXButton = (cUIThing*)text;
        obj->v09(0);
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), str_submitrect)) {
        self->mpSubmitRect = (cUIThing*)text;
        obj->v09(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_onInputBegin);
#ifdef SKIP_ASM
class cFEVObj_001B66B8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual int v06();
};

/**
 * Swallows all input on the DNAS screen; while authenticating (miState 2), "next" or "previous" cancels.
 *
 * A cancel sets the DNAS object's word at +0x14 and miState 4.
 *
 * @param obj  the input (On Tour cUIInput* pInput); slots 5 (isNext) and 6 (isPrevious) are read
 * @return always 1
 */
extern "C" int cFEStateAuthDNAS_onInputBegin(cFEStateAuthDNAS* self, cFEVObj_001B66B8* obj)
{
    if (self->miState == 2) {
        if (obj->v05() || obj->v06()) {
            *(int*)((char*)self->mpAuth + 0x14) = 1;
            self->miState = 4;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_handleEvent);
#ifdef SKIP_ASM
class cFEStateAuthDNAS_handleEvent_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

/**
 * Passes an event on to the DNAS popup's parent state instead of handling it.
 *
 * Calls handleEvent (vtable slot 24) on mpParentState, the cFEStateNetConfig that pushed the popup (func_001B55B0),
 * with both arguments unchanged, the same body as cScreenPopup_handleEvent. The cFEStateNet handler func_001A8B88,
 * which the other online states keep in this slot, never runs for the popup.
 *
 * @param msg  event message (On Tour: msg)
 * @param pParam  event parameter (On Tour: pParam)
 * @return the parent state's handleEvent result
 */
extern "C" int cFEStateAuthDNAS_handleEvent(cFEStateAuthDNAS* self, int msg, int pParam)
{
    return ((cFEStateAuthDNAS_handleEvent_cObj*)self->mpParentState)->v24(msg, pParam);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);

/**
 * Passes the DNAS screen's widget events to the cFEStateNet handler, except event 13, which is ignored.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cFEStateAuthDNAS_onWidgetEvent(cFEStateAuthDNAS* self, int pWidget, int msg)
{
    if (msg != 13) {
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_onUpdate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void USTR_copy(void* dst, void* src);
extern "C" int USTR_length(void* s);
void cMemMan_free(void* p);
extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self);
extern "C" void func_002562D0(void* self);
extern "C" void func_002563F8(void* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_kT_205Authenticating[];
extern char str_cFEStateAuthDNAS[];
extern char str_dnasGroup[];

class cFEVObj_001B6798 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(void* owner, int msg);
};

static inline int IsReady_001B6798(int* d) { return *d >= 3; }
static inline int IsDone_001B6798(int* d) { return *d == 5; }

/**
 * Steps the DNAS authentication and reports the result to the parent state.
 *
 * miState 1: once the DNAS object is ready, shows the cancel text, X button, submit rect and "dnasGroup", changes the
 * text to "kT_205Authenticating", starts the check (func_002562D0) and moves to 2. 2: once done, moves to 3. 3: on an
 * error (+0x1C) copies the object's three error texts (+0x450, +0x650, +0x850) into fresh buffers at the parent's
 * +0x710/+0x714/+0x718 and sends the parent event 0x10 (slot 17), else 0xF; the state is not advanced, so this
 * repeats each frame until the parent removes the popup. 4 (cancelled): once done, sends 0x14. Every frame then
 * updates the DNAS object (func_002563F8) and runs the cFEStateNet update.
 */
extern "C" void cFEStateAuthDNAS_onUpdate(cFEStateAuthDNAS* self)
{
    switch (self->miState) {
    case 1:
        if (IsReady_001B6798((int*)self->mpAuth)) {
            ((cFEVObj_001B6798*)self->mpTextCancel)->v09(1);
            ((cFEVObj_001B6798*)self->mpXButton)->v09(1);
            ((cFEVObj_001B6798*)self->mpSubmitRect)->v09(1);
            ((cFEVObj_001B6798*)self->mpTextAuth)->v09(1);
            cUIText_setUnicodeStringByID(self->mpTextAuth, GetHashValue32(str_kT_205Authenticating));
            cFEVObj_001B6798* widget = (cFEVObj_001B6798*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_dnasGroup));
            if (widget != 0) {
                widget->v09(1);
            }
            self->miState = 2;
            func_002562D0(self->mpAuth);
        }
        break;
    case 2:
        if (IsDone_001B6798((int*)self->mpAuth)) {
            self->miState = 3;
        }
        break;
    case 3: {
        char* auth = (char*)self->mpAuth;
        if (*(int*)(auth + 0x1C) != 0) {
            char* parentState = (char*)self->mpParentState;
            char* errorText1 = auth + 0x450;
            char* errorText2 = auth + 0x650;
            char* errorText3 = auth + 0x850;
            if (*(void**)(parentState + 0x710) != 0) {
                cMemMan_free(*(void**)(parentState + 0x710));
                *(void**)(parentState + 0x710) = 0;
            }
            *(void**)(parentState + 0x710) = operator_new_tag((USTR_length(errorText1) + 1) * 2, str_cFEStateAuthDNAS, 0x100, 0);
            if (*(void**)(parentState + 0x714) != 0) {
                cMemMan_free(*(void**)(parentState + 0x714));
                *(void**)(parentState + 0x714) = 0;
            }
            *(void**)(parentState + 0x714) = operator_new_tag((USTR_length(errorText2) + 1) * 2, str_cFEStateAuthDNAS, 0x100, 0);
            if (*(void**)(parentState + 0x718) != 0) {
                cMemMan_free(*(void**)(parentState + 0x718));
                *(void**)(parentState + 0x718) = 0;
            }
            *(void**)(parentState + 0x718) = operator_new_tag((USTR_length(errorText3) + 1) * 2, str_cFEStateAuthDNAS, 0x100, 0);
            USTR_copy(*(void**)(parentState + 0x710), errorText1);
            USTR_copy(*(void**)(parentState + 0x714), errorText2);
            USTR_copy(*(void**)(parentState + 0x718), errorText3);
            ((cFEVObj_001B6798*)self->mpParentState)->v17(self, 0x10);
        } else {
            ((cFEVObj_001B6798*)self->mpParentState)->v17(self, 0xF);
        }
        break;
    }
    case 4:
        if (IsDone_001B6798((int*)self->mpAuth)) {
            ((cFEVObj_001B6798*)self->mpParentState)->v17(self, 20);
        }
        break;
    }
    func_002563F8(self->mpAuth);
    cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNAS_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the DNAS screen: reports events 8 and 9 as handled (0x100), every other as not (0x101).
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param key  the menu event (On Tour Event)
 * @return 0x100 for events 8 and 9, else 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateAuthDNAS_onWidgetMenu(cFEStateAuthDNAS* self, int pMenu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_cFEStateAuthDNASError);
#ifdef SKIP_ASM
extern void* cFEStateAuthDNASError_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the DNAS error popup with its three texts.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with the engine and parent, keeps the body, footer and title
 * strings, and stores cFEStateAuthDNASError_vtbl and mHashName 0x45.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @param pParentState  the parent state (On Tour pParentState)
 * @param pustrBody  the body text (unicode), stored in mpustrBody
 * @param pustrFooter  the footer text (unicode), stored in mpustrFooter
 * @param pustrTitle  the title text (unicode), stored in mpustrTitle
 * @return @p self
 */
extern "C" void* cFEStateAuthDNASError_cFEStateAuthDNASError(cFEStateAuthDNASError* self, int pEngine, int pParentState, int pustrBody, int pustrFooter, int pustrTitle)
{
    func_001A8500_3(self, pEngine, pParentState);
    self->mpustrBody = (unsigned short*)pustrBody;
    self->mpustrFooter = (unsigned short*)pustrFooter;
    self->mpustrTitle = (unsigned short*)pustrTitle;
    self->mVtbl = cFEStateAuthDNASError_vtbl;
    self->mHashName = 69;
    return ((void*)self);
}
#endif

extern void* cFEStateAuthDNASError_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError__cFEStateAuthDNASError__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the DNAS error popup: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The destructor flags stay untouched in $a1 and reach func_001A85D0, which frees the object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateAuthDNASError__cFEStateAuthDNASError(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateAuthDNASError_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_207DNASerror[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates and plays the "207DNASerror" screen.
 *
 * @return cUIScreen_playFrame's result, or 0 when the screen could not be added
 */
extern "C" void* cFEStateAuthDNASError_onCreateScreen(cFEStateAuthDNASError* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_207DNASerror), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_onWidgetCreate);
#ifdef SKIP_ASM
class cFEVText_001B6BB8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern char str_Footer[];
extern char str_Body[];
extern char str_Title[];
extern char str_dnasGroup[];
extern char str_MenuDNAS[];

static inline int IsHash1B6BB8(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Binds the DNAS error popup's widgets: "Footer", "Body" and "Title" are kept in mpFooter / mpBody / mpTitle.
 *
 * "dnasGroup" is made visible and "MenuDNAS" gets bit 3 of its +0x90 flags.
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateAuthDNASError_onWidgetCreate(cFEStateAuthDNASError* self, void* obj)
{
    if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), str_Footer)) {
        self->mpFooter = (cUIText*)obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), str_Body)) {
        self->mpBody = (cUIText*)obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), str_Title)) {
        self->mpTitle = (cUIText*)obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), str_dnasGroup)) {
        ((cFEVText_001B6BB8*)obj)->v09(1);
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), str_MenuDNAS)) {
        *(int*)((char*)obj + 0x90) |= 8;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" int strlen1(void* p);
extern "C" void cUIText_setUnicodeStringWrap(void* text, void* str);
extern char str_short[];
extern char str_full[];
extern char str_Animation_Start[];

class cFEVObj_001B6C98 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline void PlayLabel_001B6C98(void* self, char* label)
{
    int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(label));
    if (frame != 0xFFFF) {
        cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
    }
}

/**
 * Fills the DNAS error popup the first time it opens.
 *
 * An empty footer plays the "short" layout and hides mpFooter; otherwise plays "full" and wraps the footer text in.
 * The body and title are wrapped into their widgets, or hidden when empty. Finally plays "Animation Start".
 *
 * @param on  nonzero the first time the state opens (On Tour bFirstTime)
 * @return 1 (the transition is done)
 */
extern "C" int cFEStateAuthDNASError_onGainTransition(cFEStateAuthDNASError* self, int on)
{
    if (on) {
        if (strlen1(self->mpustrFooter) == 0) {
            PlayLabel_001B6C98(self, str_short);
            ((cFEVObj_001B6C98*)self->mpFooter)->v09(0);
        } else {
            PlayLabel_001B6C98(self, str_full);
            cUIText_setUnicodeStringWrap(self->mpFooter, self->mpustrFooter);
        }
        if (strlen1(self->mpustrBody) == 0) {
            ((cFEVObj_001B6C98*)self->mpBody)->v09(0);
        } else {
            cUIText_setUnicodeStringWrap(self->mpBody, self->mpustrBody);
        }
        if (strlen1(self->mpustrTitle) == 0) {
            ((cFEVObj_001B6C98*)self->mpTitle)->v09(0);
        } else {
            cUIText_setUnicodeStringWrap(self->mpTitle, self->mpustrTitle);
        }
        PlayLabel_001B6C98(self, str_Animation_Start);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_handleEvent);
#ifdef SKIP_ASM
class cFEStateAuthDNASError_handleEvent_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

/**
 * Passes an event on to the DNAS error popup's parent state instead of handling it.
 *
 * Calls handleEvent (vtable slot 24) on mpParentState, the cFEStateNetConfig that pushed the popup when
 * authentication failed (func_001B5830), with both arguments unchanged, the same body as cScreenPopup_handleEvent and
 * cFEStateAuthDNAS_handleEvent; the cFEStateNet handler func_001A8B88 never runs for the popup.
 *
 * @param msg  event message (On Tour: msg)
 * @param pParam  event parameter (On Tour: pParam)
 * @return the parent state's handleEvent result
 */
extern "C" int cFEStateAuthDNASError_handleEvent(cFEStateAuthDNASError* self, int msg, int pParam)
{
    return ((cFEStateAuthDNASError_handleEvent_cObj*)self->mpParentState)->v24(msg, pParam);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_onWidgetEvent);
#ifdef SKIP_ASM
class cFEVObj_001B6E40 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(void* sender, int msg);
};

extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);

/**
 * Closes the DNAS error popup on select or back: tells the parent state (slot 17, onPopupEvent) event 0xF.
 *
 * Events 7 and 8 are ignored; the rest go to the cFEStateNet handler (cFEStateNet_onWidgetEvent).
 *
 * @param pWidget  the widget, as an int (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cFEStateAuthDNASError_onWidgetEvent(cFEStateAuthDNASError* self, int pWidget, unsigned int msg)
{
    switch (msg) {
    case 5:
    case 6:
        ((cFEVObj_001B6E40*)self->mpParentState)->v17(((cFEStateNet*)self), 0xF);
        break;
    case 7:
    case 8:
        break;
    default:
        cFEStateNet_onWidgetEvent(((cFEStateNet*)self), pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the DNAS error popup: reports events 8 and 9 as handled (0x100), every other as not (0x101).
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param key  the menu event (On Tour Event)
 * @return 0x100 for events 8 and 9, else 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateAuthDNASError_onWidgetMenu(cFEStateAuthDNASError* self, int pMenu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem_cFEStateNetAccountMem);
#ifdef SKIP_ASM
extern "C" void* cFEBlankPopup_cFEBlankPopup(cFEStateNet* self);
extern "C" void* func_001B70D0(void*, int);
// PORT: the unit declares func_001B70D0 as returning void*; it returns nothing useful
extern "C" void func_001B70D0_v(void* self, int state) __asm__("func_001B70D0");
extern "C" void func_00265880();
extern "C" char* strcpy(char* dst, const char* src);
extern void* cFEStateNetAccountMem_vtbl[];
extern char D_00536760[];

static inline void SetNames_001B6ED0(char* buf1, char* buf2, const char* a, const char* b)
{
    if (a) {
        strcpy(buf1, a);
    }
    if (b) {
        strcpy(buf2, b);
    }
}

/**
 * Constructs the online-account memory-card popup state and picks its first card step from the mode.
 *
 * Runs the cFEBlankPopup constructor (cFEBlankPopup_cFEBlankPopup; the C call passes only self, so @p pEngine and @p owner reach it
 * untouched in $a1/$a2 and a port must pass them), stores cFEStateNetAccountMem_vtbl, clears the name, password,
 * errors and flags, copies @p name and @p name2 into mszUserName / mszPassword, and starts func_001B70D0 at the
 * mode's step: 0 -> 0, 1 -> 5, 2 -> 0xB, 3 -> 0x13 (mode 3 copies the names from D_00536760 instead), stored in
 * miStartState. Finally creates the card object D_004A33D0 (func_00265880).
 *
 * @param pEngine  the UI engine (On Tour cUIState ctor pEngine), for the base constructor
 * @param owner  the parent state told of the result (mpParentState), for the base constructor
 * @param mode  which card operation to run (0, 1, 2 or 3; see above)
 * @param name  the account name, as an int (LobbyMan mUserName / tLobbyAccount mName at the call sites)
 * @param name2  the account password, as an int
 * @return @p self
 */
// PORT: the unit declares the two name pointers as int
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* self, int pEngine, void* owner, int mode, int name, int name2)
{
    char* buf1 = (char*)self + 0x6E0;
    cFEBlankPopup_cFEBlankPopup((cFEStateNet*)self);
    char* buf2 = (char*)self + 0x6F1;
    self->mVtbl = cFEStateNetAccountMem_vtbl;
    *((char*)self + 0x6E0) = 0;
    *((char*)self + 0x6F1) = 0;
    self->miError = 0;
    self->unk_6D8 = 0;
    self->mbCardBusy = 0;
    self->unk_B08 = 0;
    SetNames_001B6ED0(buf1, buf2, (const char*)name, (const char*)name2);
    switch (mode) {
    case 0:
        func_001B70D0_v(self, 0);
        self->miStartState = 0;
        break;
    case 1:
        func_001B70D0_v(self, 5);
        self->miStartState = 5;
        break;
    case 2:
        func_001B70D0_v(self, 11);
        self->miStartState = 11;
        break;
    case 3:
        SetNames_001B6ED0(buf1, buf2, D_00536760, D_00536760 + 0x11);
        func_001B70D0_v(self, 19);
        self->miStartState = 19;
        break;
    }
    func_00265880();
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem__cFEStateNetAccountMem);
#ifdef SKIP_ASM
extern void* cFEStateNetAccountMem_vtbl[];
extern void* cFEBlankPopup_vtbl[];
extern "C" void func_002658B8();
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the online-account memory-card popup: deletes the card object, then runs the base destructors.
 *
 * Restores cFEStateNetAccountMem_vtbl, deletes D_004A33D0 (func_002658B8), stores cFEBlankPopup's vtable cFEBlankPopup_vtbl
 * and runs the cFEStateNet destructor (func_001A85D0) with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cFEStateNetAccountMem__cFEStateNetAccountMem(cFEStateNetAccountMem* self, int flags)
{
    self->mVtbl = cFEStateNetAccountMem_vtbl;
    func_002658B8();
    self->mVtbl = cFEBlankPopup_vtbl;
    func_001A85D0_dtor(((cFEStateNet*)self), flags);
}
#endif

extern "C" void* cFEBlankPopup_onCreateScreen(cFEStateNet* self);

//99.29%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem_onCreateScreen__FPv);
#ifdef SKIP_ASM
/**
 * Creates the account popup's screen: the cFEBlankPopup handler (cFEBlankPopup_onCreateScreen) adds and plays its blank screen.
 *
 * @return cFEBlankPopup_onCreateScreen()'s result (its value is not meaningful; declared void* by the unit)
 */
void* cFEStateNetAccountMem_onCreateScreen(void* self)
{
    return cFEBlankPopup_onCreateScreen((cFEStateNet*)self);
}
#endif

extern "C" void* func_001B7778(cFEStateNetAccountMem*);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B70A8__FPv);
#ifdef SKIP_ASM
void* func_001B70A8(void* self)
{
    int zero = 0;
    *(signed char*)((char*)self + 0x6e0) = (signed char)zero;
    *(signed char*)((char*)self + 0x6f1) = (signed char)zero;
    return func_001B7778((cFEStateNetAccountMem*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B70D0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001B7650(cFEStateNetAccountMem* self, int a1, int a2, int b0, int b1, int a5, int a6);
// PORT: the unit declares func_001B7778 as returning void*; the result is unused here
extern "C" void func_001B7778_v(void* self) __asm__("func_001B7778");
extern "C" void func_001B7E40(char* self, unsigned int msg);
extern "C" int func_00265A38(void* p);
extern "C" void func_00265D08(void* p);
void func_00265DF0(void* p);
void func_00265E00(void* p);
void func_00265E10(void* p);
void func_00265E20(void* p);
void func_00265E30(void* p);
extern "C" void strcpy1(void* str, int a1);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...); the unit declares a 3-arg view
extern "C" void sprintf1_1B70D0(void* str, int fmt, ...) __asm__("sprintf1");
extern "C" void usprintf(char* buf, const char* fmt, int a2);
extern "C" void func_0039F190(void* self, int a1);
extern int* D_004A33D0;
extern char str_pct_d_b[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char str_kT_MEMFECheckingCardPS2[];
extern char str_kT_POPUPCMNEABigLogin[];
extern char str_kT_MEMOVOverWriteFile[];
extern char str_kT_MEMOVSaveSavingPS2NGC[];
extern char str_kT_MEMFormatCardPS2[];
extern char str_kT_MEMFormattingPS2[];
extern char str_kT_POPUPTITLEDeleteAccount[];
extern char str_kT_MEMDeletingPS2[];
extern char str_kT_MEMOVDamagedDataPS2[];

struct sVEntry_001B70D0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1B70D0(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B70D0* vt = *(sVEntry_001B70D0**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

struct sFEState_001B70D0 {
    char pad0[0x10];
    char* f10;          // 0x10
    char pad14[0x6D4 - 0x14];
    int f6D4;           // 0x6D4
    int f6D8;           // 0x6D8
    int f6DC;           // 0x6DC
    char pad6E0[0x702 - 0x6E0];
    char text[0xB04 - 0x702]; // 0x702
    int fB04;           // 0xB04
};

// PORT: the unit declares func_001B70D0 as returning void*; it returns nothing useful
extern "C" void func_001B70D0_v(void* self, int state) __asm__("func_001B70D0");

extern "C" void func_001B70D0_v(void* vself, int state)
{
    sFEState_001B70D0* self = (sFEState_001B70D0*)vself;
    char buf[0x20];
    self->f6DC = state;
    int str = Loc1B70D0(str_kT_POPUPCMNEABigLogin);
    usprintf(buf, str_pct_d_b, 1);
    switch ((unsigned int)state) {
    case 1: {
        char* text = self->text;
        func_00265D08(D_004A33D0);
        func_00265DF0(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMFECheckingCardPS2), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 6: {
        char* text = self->text;
        func_00265D08(D_004A33D0);
        func_00265DF0(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMFECheckingCardPS2), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 7: {
        char* text = self->text;
        self->f6D8 = 0;
        self->fB04 = 0;
        func_00265D08(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMOVOverWriteFile), str);
        int h1 = GetHashValue32(str_kT_CMNYes);
        int h2 = GetHashValue32(str_kT_CMNNo);
        func_001B7650((cFEStateNetAccountMem*)self, 8, 1, h1, h2, 0, (int)text);
        break;
    }
    case 8: {
        func_001B7778_v(self);
        self->fB04 = 1;
        if (self->f6D8 != 0) {
            func_001B70D0_v(self, 9);
            break;
        }
        char* text = self->text;
        func_00265E00(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMOVSaveSavingPS2NGC), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 12: {
        char* text = self->text;
        func_00265D08(D_004A33D0);
        func_00265DF0(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMFECheckingCardPS2), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 13: {
        char* text = self->text;
        self->fB04 = 0;
        self->f6D8 = 0;
        func_00265D08(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMFormatCardPS2), buf);
        int h1 = GetHashValue32(str_kT_CMNYes);
        int h2 = GetHashValue32(str_kT_CMNNo);
        func_001B7650((cFEStateNetAccountMem*)self, 0xE, 1, h1, h2, 0, (int)text);
        break;
    }
    case 14: {
        self->fB04 = 1;
        char* text = self->text;
        func_00265E30(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMFormattingPS2), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 16: {
        self->fB04 = 0;
        if (func_00265A38(D_004A33D0) < 17) {
            self->f6D4 = 3;
            func_001B70D0_v(self, 17);
            break;
        }
        self->fB04 = 1;
        char* text = self->text;
        func_001B7778_v(self);
        func_00265E10(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMOVSaveSavingPS2NGC), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 0:
    case 5:
    case 11:
    case 19:
        self->f6D8 = 0;
    case 2:
        self->fB04 = 0;
        break;
    case 20: {
        char* text = self->text;
        func_00265D08(D_004A33D0);
        func_00265DF0(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMFECheckingCardPS2), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 21: {
        char* text = self->text;
        self->fB04 = 0;
        self->f6D8 = 0;
        func_00265D08(D_004A33D0);
        strcpy1(text, Loc1B70D0(str_kT_POPUPTITLEDeleteAccount));
        int h1 = GetHashValue32(str_kT_CMNYes);
        int h2 = GetHashValue32(str_kT_CMNNo);
        func_001B7650((cFEStateNetAccountMem*)self, 22, 1, h1, h2, 0, (int)text);
        break;
    }
    case 22: {
        self->fB04 = 1;
        if (self->f6D8 != 0) {
            func_001B70D0_v(self, 23);
            break;
        }
        char* text = self->text;
        func_00265E20(D_004A33D0);
        strcpy1(text, Loc1B70D0(str_kT_MEMDeletingPS2));
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 23:
        func_001B7778_v(self);
    case 3:
    case 9:
    case 15:
    case 17:
        func_001B7E40((char*)self, self->f6D4);
        break;
    case 25: {
        char* text = self->text;
        self->fB04 = 0;
        self->f6D8 = 0;
        func_00265D08(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMOVDamagedDataPS2), buf);
        int h1 = GetHashValue32(str_kT_CMNYes);
        int h2 = GetHashValue32(str_kT_CMNNo);
        func_001B7650((cFEStateNetAccountMem*)self, 26, 1, h1, h2, 0, (int)text);
        break;
    }
    case 26: {
        self->fB04 = 1;
        if (self->f6D8 != 0) {
            func_001B70D0_v(self, 0x1C);
            break;
        }
        char* text = self->text;
        func_00265E20(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMOVSaveSavingPS2NGC), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 27: {
        self->fB04 = 1;
        char* text = self->text;
        func_001B7778_v(self);
        func_00265E10(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(str_kT_MEMOVSaveSavingPS2NGC), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 4:
    case 10:
    case 18:
    case 24:
    case 29:
        func_0039F190(self->f10 + 0x18, 1);
        break;
    case 28:
        func_001B7E40((char*)self, self->f6D4);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7650);
#ifdef SKIP_ASM
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);

extern "C" void func_001B7650(cFEStateNetAccountMem* self, int popupId, int defaultSel, int firstButton, int secondButton, int titleText, int bodyText)
{
    void* popup = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)popup + 0xBC;
    if (firstButton != 0) {
        if (secondButton != 0) {
            *(int*)((char*)popup + 0xBC) = 2;
            func_001C57C0(sub, 0, firstButton);
            func_001C57C0(sub, 1, secondButton);
            *(int*)((char*)popup + 0x168) = defaultSel;
        } else {
            *(int*)((char*)popup + 0xBC) = 1;
            func_001C57C0(sub, 0, firstButton);
            *(int*)((char*)popup + 0x168) = 0;
        }
    } else {
        *(int*)((char*)popup + 0xBC) = 0;
        *(int*)((char*)popup + 0x164) = 0;
        *(int*)((char*)popup + 0x168) = 0;
    }
    if (titleText != 0) {
        *(int*)(sub + 0x10) = titleText;
        *(int*)(sub + 0xC) = 1;
    }
    if (bodyText != 0) {
        *(int*)(sub + 0x8) = bodyText;
        *(int*)(sub + 0x4) = 1;
    }
    *(int*)(sub + 0x94) = popupId;
    *(int*)(sub + 0xA8) = 0;
    *(int*)(sub + 0x98) = 1;
    func_001A9BC0((cFEStateNet*)self, popup);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7778);
#ifdef SKIP_ASM
extern "C" void func_00265F18(void*);
extern "C" void* func_00265F68(void*);

extern "C" void* func_001B7778(cFEStateNetAccountMem* self)
{
    func_00265F18((char*)self + 0x6E0);
    return func_00265F68((char*)self + 0x6F1);
}
#endif

extern "C" void* func_001B70D0(void*, int);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B77A8__FPv);
#ifdef SKIP_ASM
void* func_001B77A8(void* self)
{
    return func_001B70D0(self, 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B77C8);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B77C8(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B77C8(cFEStateNetAccountMem* self)
{
    if (IsMode_001B77C8(4)) {
        func_001B70D0(self, 4);
    } else if (IsMode_001B77C8(5)) {
        if (self->miError == 1) {
            func_001B70D0(self, 4);
        } else {
            func_001B70D0(self, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7820);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);
extern "C" int func_00265CB0(void* p);

static inline bool IsMode_001B7820(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7820(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7820(4)) {
        func_001B70D0(self, 7);
    } else if (IsMode_001B7820(5)) {
        if (self->miError == 2 && func_00265CB0(D_004A33D0) != 0) {
            func_001B70D0(self, 13);
        } else if (self->miError == 1) {
            func_001B70D0(self, 0x10);
        } else if (self->miError == 5) {
            func_001B70D0(self, 25);
        } else if (self->miError == 6) {
            func_001B70D0(self, 25);
        } else {
            func_001B70D0(self, 9);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B78F0);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B78F0(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B78F0(cFEStateNetAccountMem* self)
{
    if (IsMode_001B78F0(8)) {
        func_001B70D0(self, 10);
    } else if (IsMode_001B78F0(9)) {
        func_001B70D0(self, 9);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7938);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);
extern "C" int func_00265CB0(void* p);

static inline bool IsMode_001B7938(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7938(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7938(4)) {
        func_001B70D0(self, 7);
    } else if (IsMode_001B7938(5)) {
        if (self->miError == 2 && func_00265CB0(D_004A33D0) != 0) {
            func_001B70D0(self, 13);
        } else if (self->miError == 1) {
            func_001B70D0(self, 0x10);
        } else if (self->miError == 5) {
            func_001B70D0(self, 25);
        } else if (self->miError == 6) {
            func_001B70D0(self, 25);
        } else {
            func_001B70D0(self, 17);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7A08);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7A08(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7A08(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7A08(21)) {
        func_001B70D0(self, 0x10);
    } else if (IsMode_001B7A08(22)) {
        func_001B70D0(self, 0xF);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7A50);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7A50(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7A50(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7A50(0xC)) {
        func_001B70D0(self, 18);
    } else if (IsMode_001B7A50(13)) {
        func_001B70D0(self, 17);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7A98);
#ifdef SKIP_ASM
extern "C" void* func_001B70D0(void*, int);
extern "C" int func_00265CB0(void* p);
extern "C" int func_004165A8(const char* a, const char* b);
extern int* D_004A33D0;
extern char D_00536760[];

static inline bool IsMode_001B7A98(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7A98(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7A98(4)) {
        if (func_004165A8(D_00536760, (char*)self + 0x6E0) != 0 || func_004165A8(D_00536760 + 0x11, (char*)self + 0x6F1) != 0) {
            self->miError = 0x18;
            self->unk_B08 = 1;
            self->unk_6D8 = 1;
            func_001B70D0(self, 23);
        } else {
            func_001B70D0(self, 21);
        }
    } else if (IsMode_001B7A98(5)) {
        int s = self->miError;
        if (s == 1) {
            self->unk_B08 = 1;
            self->unk_6D8 = 1;
            self->miError = 0x18;
            func_001B70D0(self, 23);
        } else if (s == 5) {
            func_001B70D0(self, 21);
        } else if (s == 6) {
            func_001B70D0(self, 21);
        } else if (s == 2) {
            if (func_00265CB0(D_004A33D0) != 0) {
                self->unk_B08 = 1;
                self->miError = 0x18;
            }
            func_001B70D0(self, 23);
            goto dflt;
        } else {
        dflt:
            func_001B70D0(self, 23);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7BC0);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);
void* func_001B70A8(void* self);

static inline bool IsMode_001B7BC0(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7BC0(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7BC0(0x10)) {
        func_001B70A8(self);
        func_001B70D0(self, 0x18);
    } else if (IsMode_001B7BC0(17)) {
        func_001B70D0(self, 23);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7C28);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7C28(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7C28(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7C28(0x10)) {
        func_001B70D0(self, 27);
    } else if (IsMode_001B7C28(17)) {
        func_001B70D0(self, 0x1C);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7C70);
#ifdef SKIP_ASM
extern int* D_004A33D0;
extern "C" void* func_001B70D0(void*, int);

static inline bool IsMode_001B7C70(int m)
{
    return *D_004A33D0 == m;
}

extern "C" void func_001B7C70(cFEStateNetAccountMem* self)
{
    if (IsMode_001B7C70(0xC)) {
        func_001B70D0(self, 29);
    } else if (IsMode_001B7C70(13)) {
        func_001B70D0(self, 0x1C);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem_onUpdate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int* D_004A33D0;
extern "C" void func_00265A68(void* self);
extern "C" int func_00265D38(void* self);
extern "C" void func_002659E8(void* self);
extern "C" void* func_001B70D0(void*, int);
void* func_001B77A8(void* self);
extern "C" void func_001B77C8(cFEStateNetAccountMem* self);
extern "C" void func_001B7820(cFEStateNetAccountMem* self);
extern "C" void func_001B78F0(cFEStateNetAccountMem* self);
extern "C" void func_001B7938(cFEStateNetAccountMem* self);
extern "C" void func_001B7A08(cFEStateNetAccountMem* self);
extern "C" void func_001B7A50(cFEStateNetAccountMem* self);
extern "C" void func_001B7A98(cFEStateNetAccountMem* self);
extern "C" void func_001B7BC0(cFEStateNetAccountMem* self);
extern "C" void func_001B7C28(cFEStateNetAccountMem* self);
extern "C" void func_001B7C70(cFEStateNetAccountMem* self);

/**
 * Runs one frame of the account memory-card popup: polls the card and steps the current card state.
 *
 * Updates the card object D_004A33D0 (func_00265A68) and copies its status word into miError; when func_00265D38
 * reports a card change it sets unk_6D8 (and error 2 if none). Then by miState: the start steps 0, 5, 11 and 19
 * advance to 1, 6, 12 and 20 with func_001B70D0; 1, 2, 6, 8, 12, 14, 16, 20, 22, 26 and 27 run their step helpers
 * (func_001B77A8 .. func_001B7C70); the popup-wait states 7, 13, 21 and 25 call func_002659E8 on the card; 28 and
 * 29 do nothing.
 */
// Polls the memory card, then runs the step handler for the current memory-card state.
extern "C" void cFEStateNetAccountMem_onUpdate(cFEStateNetAccountMem* self)
{
    func_00265A68(D_004A33D0);
    int* mc = D_004A33D0;
    self->miError = mc[1];
    if (func_00265D38(mc) != 0) {
        self->unk_6D8 = 1;
        if (self->miError == 0) {
            self->miError = 2;
        }
    }
    switch (self->miState) {
    case 0: func_001B70D0(self, 1); break;
    case 1: func_001B77A8(self); break;
    case 2: func_001B77C8(self); break;
    case 5: func_001B70D0(self, 6); break;
    case 6: func_001B7820(self); break;
    case 8: func_001B78F0(self); break;
    case 11: func_001B70D0(self, 0xC); break;
    case 12: func_001B7938(self); break;
    case 14: func_001B7A08(self); break;
    case 16: func_001B7A50(self); break;
    case 19: func_001B70D0(self, 20); break;
    case 20: func_001B7A98(self); break;
    case 22: func_001B7BC0(self); break;
    case 7: case 13: case 21: case 25: func_002659E8(D_004A33D0); break;
    case 26: func_001B7C28(self); break;
    case 27: func_001B7C70(self); break;
    case 28: case 29: break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B7E40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

int GetHashValue32(char* str);

extern char str_kT_BTNOK[];


extern char str_pct_d_b[];

extern char str_kT_CMNContinue[];

extern char str_kT_MEMOVNotFormattedPS2[];

extern char str_kT_MEMOVRetry[];

extern int* D_004A33D0;

extern char str_kT_POPUPCMNEABigLogin[];

extern "C" int func_00265CB0(void* p);

extern "C" int func_00265D68(void* p);

extern char str_kT_MEMOVNotDetectedPS2[];

extern char str_kT_MEMFEFormatFailed[];

// sprintf1 is a varargs formatter (dst, fmt, ...); the plain declaration above is a 3-arg view.
extern "C" void sprintf1_v(void* dst, int fmt, ...) __asm__("sprintf1");

// usprintf is a varargs formatter (buf, fmt, ...); the plain declaration above is a 3-arg view.
extern "C" void usprintf_v(char* buf, const char* fmt, ...) __asm__("usprintf");

extern "C" void func_001B8238_7E40(char* self) __asm__("func_001B8238");
extern "C" void func_001B83C0_7E40(char* self) __asm__("func_001B83C0");
extern char str_kT_MEMOnlineTooBigPS2[];
extern char str_kT_MEMOnlineCorruptPS2[];
extern char str_kT_POPUPCMNCardRemoved[];

static inline int Loc_B7E40(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry<int(void*, int)>* vt = *(sVEntry<int(void*, int)>**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

static inline bool IsMode_001B7E40(int m)
{
    return *D_004A33D0 == m;
}

struct sFEState_001B7E40 {
    char pad0[0x6D8];
    int f6D8;           // 0x6D8
    int f6DC;           // 0x6DC
    char pad6E0[0x702 - 0x6E0];
    char text[0xB04 - 0x702]; // 0x702
    int fB04;           // 0xB04
};

extern "C" void func_001B7E40_s(sFEState_001B7E40* self, unsigned int mode) __asm__("func_001B7E40");

extern "C" void func_001B7E40_s(sFEState_001B7E40* self, unsigned int mode)
{
    char buf[0x20];
    char* cur = 0;
    int h2 = 0;
    int flag = 1;
    int h1 = GetHashValue32(str_kT_BTNOK);
    usprintf_v(buf, str_pct_d_b, 1);
    switch (mode) {
    case 2:
        if (self->fB04 == 0) {
            if (func_00265CB0(D_004A33D0)) {
                sprintf1_v(self->text, Loc_B7E40(str_kT_MEMOVNotFormattedPS2), buf);
            } else {
                func_001B8238_7E40((char*)self);
            }
        } else {
            func_001B83C0_7E40((char*)self);
        }
        flag = 0;
        h1 = GetHashValue32(str_kT_MEMOVRetry);
        h2 = GetHashValue32(str_kT_CMNContinue);
        cur = self->text;
        break;
    case 3:
        sprintf1_v(self->text, Loc_B7E40(str_kT_MEMOnlineTooBigPS2), buf, 17);
        flag = 0;
        h1 = GetHashValue32(str_kT_MEMOVRetry);
        h2 = GetHashValue32(str_kT_CMNContinue);
        cur = self->text;
        break;
    case 5:
        if (self->f6DC == 3) {
            flag = 0;
            int a = Loc_B7E40(str_kT_POPUPCMNEABigLogin);
            int b = Loc_B7E40(str_kT_MEMOnlineCorruptPS2);
            sprintf1_v(self->text, b, a, buf);
        } else {
            func_001B83C0_7E40((char*)self);
        }
        h1 = GetHashValue32(str_kT_MEMOVRetry);
        h2 = GetHashValue32(str_kT_CMNContinue);
        cur = self->text;
        break;
    case 1:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
        func_001B83C0_7E40((char*)self);
        h1 = GetHashValue32(str_kT_MEMOVRetry);
        h2 = GetHashValue32(str_kT_CMNContinue);
        cur = self->text;
        break;
    case 0:
        if (IsMode_001B7E40(22)) {
            if (func_00265D68(D_004A33D0)) {
                sprintf1_v(self->text, Loc_B7E40(str_kT_MEMOVNotDetectedPS2), buf);
            } else {
                sprintf1_v(self->text, Loc_B7E40(str_kT_MEMFEFormatFailed), buf);
            }
            h1 = GetHashValue32(str_kT_MEMOVRetry);
            h2 = GetHashValue32(str_kT_CMNContinue);
            cur = self->text;
        }
        break;
    }
    if (flag && self->f6D8 != 0) {
        sprintf1_v(self->text, Loc_B7E40(str_kT_POPUPCMNCardRemoved), buf);
        h1 = GetHashValue32(str_kT_MEMOVRetry);
        h2 = GetHashValue32(str_kT_CMNContinue);
        cur = self->text;
    }
    int r = -1;
    if (h2 == 0)
        r = 0;
    if (self->f6DC == 23) {
        h2 = 0;
        h1 = GetHashValue32(str_kT_CMNContinue);
    }
    func_001B7650((cFEStateNetAccountMem*)self, r, 0, h1, h2, 0, (int)cur);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B8238);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: usprintf/sprintf1 are varargs formatters; the unit declares fixed-arg views
extern "C" void usprintf_1B8238(char* buf, const char* fmt, ...) __asm__("usprintf");
extern "C" void sprintf1_1B8238(void* dst, int fmt, ...) __asm__("sprintf1");
extern char str_pct_d_b[];
struct sVEntry_001B8238 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
static inline int Loc_001B8238(char* key)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B8238* vt = *(sVEntry_001B8238**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(key));
}
extern char str_kT_POPUPCMNEABigLogin[];
extern char str_kT_MEMFENotDetectedFile[];
extern char str_kT_MEMFENotDetectedSize[];
extern char str_kT_MEMOVNotDetectedPS2[];

extern "C" void func_001B8238(cFEStateNetAccountMem* self)
{
    char buf[32];
    usprintf_1B8238(buf, str_pct_d_b, 1);
    switch (self->miState) {
    case 3:
    case 23: {
        int name = Loc_001B8238(str_kT_POPUPCMNEABigLogin);
        int str = Loc_001B8238(str_kT_MEMFENotDetectedFile);
        sprintf1_1B8238(((char*)self) + 0x702, str, 1, name);
        break;
    }
    case 9:
    case 15:
    case 17: {
        int str = Loc_001B8238(str_kT_MEMFENotDetectedSize);
        sprintf1_1B8238(((char*)self) + 0x702, str, 1, 17);
        break;
    }
    case 28: {
        int str = Loc_001B8238(str_kT_MEMOVNotDetectedPS2);
        sprintf1_1B8238(((char*)self) + 0x702, str, buf);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B83C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void usprintf_1B83C0(char* buf, const char* fmt, ...) __asm__("usprintf");
extern "C" void sprintf1_1B83C0(void* dst, int fmt, ...) __asm__("sprintf1");
extern char str_pct_d_b[];
extern char str_kT_MEMOVLoadFailedPS2NGC[];
extern char str_kT_MEMOVSaveFailed[];
extern char str_kT_MEMOVDeleteFailedError[];
extern char str_kT_MEMFEFormatFailed[];

struct sVEntry_001B83C0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc_001B83C0(char* key)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001B83C0* vt = *(sVEntry_001B83C0**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(key));
}

// Formats the localized string for key into the dialog's text buffer at 0x702.
static inline void SetText_001B83C0(char* self, char* key, char* buf)
{
    int str = Loc_001B83C0(key);
    sprintf1_1B83C0(self + 0x702, str, buf);
}

extern "C" void func_001B83C0(cFEStateNetAccountMem* self)
{
    char buf[32];
    usprintf_1B83C0(buf, str_pct_d_b, 1);
    switch (self->miState) {
    case 3:
        SetText_001B83C0(((char*)self), str_kT_MEMOVLoadFailedPS2NGC, buf);
        break;
    case 9:
    case 17:
    case 28:
        SetText_001B83C0(((char*)self), str_kT_MEMOVSaveFailed, buf);
        break;
    case 23:
        SetText_001B83C0(((char*)self), str_kT_MEMOVDeleteFailedError, buf);
        break;
    case 15:
        SetText_001B83C0(((char*)self), str_kT_MEMFEFormatFailed, buf);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem_onPopupEvent);
#ifdef SKIP_ASM
// PORT: the unit declares func_001A97B8 as returning int; the result is unused here.
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" int func_001B8890(cFEStateNetAccountMem* self, int state);
extern "C" int func_00265D68(void* p);
extern int* D_004A33D0;

class cListener_001B84D8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(void* owner, int v);
};

/**
 * Handles the account popup's events per card state and tells the parent state the result when it closes.
 *
 * Results are reported to mpParentState's slot 17 on event 0x16 (closed). States 7/13/21/25 (card prompts): option
 * 0 (0xF) goes to func_001B8890's follow-up step for the state when the card changed (func_00265D68; error 2) or
 * unk_6D8 is set (error 6), else to the popup's next state (+0x18) when nonzero; 0x10 / 0x14 set miResult 0x11 (in
 * 7/13/25) or 0x10; 0x16 reports miResult. State 3: 0xF restarts from miStartState when miMiscData is set (else
 * result 0x10), 0x10 / 0x14 result 0xF. States 9/15/17/28: 0xF result 0x11, 0x10 / 0x14 result 0xF. State 23: 0xF
 * restarts from step 0 when unk_B08 is set, else result 0x10. States 4/10/18/24/29 report 0xF on close. Every event
 * also goes to the cFEStateNet handler (func_001A97B8).
 *
 * @param w  the popup state that sent the event (On Tour pPopup); its +0x18 names the next state
 * @param msg  the popup event (On Tour Event; 0xF + i option i, 0x14 back, 0x16 closed)
 */
extern "C" void cFEStateNetAccountMem_onPopupEvent(cFEStateNetAccountMem* self, void* w, unsigned int msg)
{
    int result;
    switch (self->miState) {
    case 7:
    case 13:
    case 21:
    case 25:
        switch (msg) {
        case 15:
            func_001A97B8_v(self, w, 15);
            if (func_00265D68(D_004A33D0) != 0) {
                self->miError = 2;
                func_001B70D0(self, func_001B8890(self, self->miState));
            } else if (self->unk_6D8 != 0) {
                self->miError = 6;
                func_001B70D0(self, func_001B8890(self, self->miState));
            } else {
                int x = *(int*)((char*)w + 0x18);
                if (x != 0) {
                    func_001B70D0(self, x);
                }
            }
            break;
        case 16:
        case 20: {
            func_001A97B8_v(self, w, msg);
            int st = self->miState;
            if (st == 25 || st == 13 || st == 7) {
                self->miResult = 17;
            } else {
                self->miResult = 0x10;
            }
            break;
        }
        case 22:
            func_001A97B8_v(self, w, 22);
            ((cListener_001B84D8*)self->mpParentState)->v17(self, self->miResult);
            break;
        default:
            func_001A97B8_v(self, w, msg);
            break;
        }
        break;
    case 3:
        switch (msg) {
        case 15:
            func_001A97B8_v(self, w, 15);
            if (self->miMiscData != 0) {
                self->miResult = 0;
                self->miError = 0;
                self->unk_6D8 = 0;
                func_001B70D0(self, self->miStartState);
            } else {
                self->miResult = 0x10;
            }
            break;
        case 16:
        case 20:
            func_001A97B8_v(self, w, msg);
            self->miResult = 0xF;
            break;
        case 22:
            func_001A97B8_v(self, w, 22);
            result = self->miResult;
            if (result != 0) {
                ((cListener_001B84D8*)self->mpParentState)->v17(self, result);
            }
            break;
        default:
            func_001A97B8_v(self, w, msg);
            break;
        }
        break;
    case 9:
    case 15:
    case 17:
    case 28:
        switch (msg) {
        case 15:
            func_001A97B8_v(self, w, 15);
            self->miResult = 17;
            break;
        case 16:
        case 20:
            func_001A97B8_v(self, w, msg);
            self->miResult = 0xF;
            break;
        case 22:
            func_001A97B8_v(self, w, 22);
            result = self->miResult;
            if (result != 0) {
                ((cListener_001B84D8*)self->mpParentState)->v17(self, result);
            }
            break;
        default:
            func_001A97B8_v(self, w, msg);
            break;
        }
        break;
    case 23:
        switch (msg) {
        case 15:
            func_001A97B8_v(self, w, 15);
            if (self->unk_B08 == 0) {
                self->miResult = 0x10;
            } else {
                self->miResult = 0;
                self->miError = 0;
                self->unk_6D8 = 0;
                self->unk_B08 = 0;
                self->miStartState = 0;
                func_001B70D0(self, 0);
            }
            break;
        case 22:
            func_001A97B8_v(self, w, 22);
            result = self->miResult;
            if (result != 0) {
                ((cListener_001B84D8*)self->mpParentState)->v17(self, result);
            }
            break;
        default:
            func_001A97B8_v(self, w, msg);
            break;
        }
        break;
    case 4:
    case 10:
    case 18:
    case 24:
    case 29:
        if (msg == 22) {
            func_001A97B8_v(self, w, 22);
            ((cListener_001B84D8*)self->mpParentState)->v17(self, 0xF);
        } else {
            func_001A97B8_v(self, w, msg);
        }
        break;
    default:
        func_001A97B8_v(self, w, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem_handleEvent);
#ifdef SKIP_ASM
class cFEStateNetAccountMem_handleEvent_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

/**
 * Passes an event on to the account memory-card popup's parent state instead of handling it.
 *
 * Calls handleEvent (vtable slot 24) on mpParentState, the state that pushed the popup (cFEStateLogin,
 * cFEStateNetConfig, cFEStateCreateStep1 ...), with both arguments unchanged, the same body as
 * cScreenPopup_handleEvent; the cFEStateNet handler func_001A8B88 never runs for the popup.
 *
 * @param msg  event message (On Tour: msg)
 * @param pParam  event parameter (On Tour: pParam)
 * @return the parent state's handleEvent result
 */
extern "C" int cFEStateNetAccountMem_handleEvent(cFEStateNetAccountMem* self, int msg, int pParam)
{
    return ((cFEStateNetAccountMem_handleEvent_cObj*)self->mpParentState)->v24(msg, pParam);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B8890);
#ifdef SKIP_ASM
extern "C" int func_001B8890(cFEStateNetAccountMem* self, int id)
{
    switch (id) {
    case 25:
        return 28;
    case 13:
        return 15;
    case 21:
        return 23;
    case 7:
        return 9;
    }
    return 28;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep1_cFEStateCreateStep1);
#ifdef SKIP_ASM
extern void* cFEStateCreateStep1_vtbl[];
extern int cBXString_EmptyString;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs step 1 of the online account creation (or editing) screen.
 *
 * Runs the cFEStateNet constructor (func_001A8500, no parent), stores cFEStateCreateStep1_vtbl, clears
 * mpStaticContinue and unk_6A8, points the saved password, confirmation and mail texts at cBXString_EmptyString (an empty
 * string), stores @p bNewAccount in mbEditAccount and mHashName 0x4D when it is set, else 0x50. Despite the field's name a
 * nonzero flag means a new account: the callers pass 1 from "create account" (cFEStateLogin_onWidgetEvent,
 * cFEStateOnlineAgreement id 0x14, Step2's back) and the screen then uses LobbyMan's mNewAccount form.
 *
 * @param pEngine  the UI engine (On Tour cUIState ctor pEngine)
 * @param bNewAccount  nonzero to create a new account, 0 to edit the logged-in one (stored in mbEditAccount)
 * @return @p self
 */
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1(cFEStateCreateStep1* self, int pEngine, int bNewAccount)
{
    func_001A8500_3(self, pEngine, 0);
    int emptyStr = cBXString_EmptyString;
    self->mpStaticContinue = 0;
    self->mVtbl = cFEStateCreateStep1_vtbl;
    self->mszSavedPassword = (char*)emptyStr;
    self->mszSavedConfirm = (char*)emptyStr;
    self->mszSavedMail = (char*)emptyStr;
    if (bNewAccount != 0) {
        self->mHashName = 77;
    } else {
        self->mHashName = 80;
    }
    self->mbEditAccount = bNewAccount;
    self->unk_6A8 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep1_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char str_210create_step1[];

/**
 * Opens the "210create_step1" screen on the account being edited or created.
 *
 * Points mpAccount at LobbyMan's mNewAccount form (+0x260) when mbEditAccount is set, else at the logged-in
 * mAccount (+0x2B0): a nonzero flag means creating (fe/festatenetcreate.h's note on mpAccount says the reverse).
 * Adds the screen by hash into mpScreen, plays its frame 0 and calls func_001A8768 (an empty cFEStateNet hook).
 */
extern "C" void cFEStateCreateStep1_onCreateScreen(cFEStateCreateStep1* self)
{
    if (self->mbEditAccount != 0) {
        self->mpAccount = (tLobbyAccount*)((char*)LobbyMan_mLobbyMan + 0x260);
    } else {
        self->mpAccount = (tLobbyAccount*)((char*)LobbyMan_mLobbyMan + 0x2B0);
    }
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_210create_step1), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep1_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Runs one frame of account creation step 1: only the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (the pending-event handler's value)
 */
void* cFEStateCreateStep1_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep1_onGainTransition);
#ifdef SKIP_ASM
struct sVE1B8A08 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" bool func_001B9558(cFEStateCreateStep1* self);
extern "C" void func_001B95C0(cFEStateCreateStep1* self);
extern "C" void func_001B9610(cFEStateCreateStep1* self);
extern "C" void func_001B9678(cFEStateCreateStep1* self);
extern "C" void func_001B96E0(cFEStateCreateStep1* self);
extern "C" void func_001B9740(cFEStateCreateStep1* self);
extern "C" void func_001B97B0(cFEStateCreateStep1* self);
extern "C" void func_001BA178(cFEStateCreateStep1* self);

/**
 * Fills account creation step 1's boxes the first time it gains focus and enables Continue when the form is
 * complete.
 *
 * On the first time it fills the name, password, confirmation, e-mail and the two yes/no boxes (func_001B95C0 ..
 * func_001B9740), saves the account's fields (func_001BA178) and sets mpStaticContinue's slot 8 with
 * !func_001B9558(self) (the "all four strings set" check).
 *
 * @param bFirstTime  nonzero on the first transition (On Tour bFirstTime)
 * @return always 0
 */
extern "C" int cFEStateCreateStep1_onGainTransition(cFEStateCreateStep1* self, int bFirstTime)
{
    if (bFirstTime != 0) {
        func_001B95C0(self);
        func_001B9610(self);
        func_001B9678(self);
        func_001B96E0(self);
        func_001B97B0(self);
        func_001B9740(self);
        func_001BA178(self);
        void* obj = self->mpStaticContinue;
        if (obj != 0) {
            sVE1B8A08* vt = *(sVE1B8A08**)((char*)obj + 0x8);
            void* target = (char*)obj + vt[8].delta;
            vt[8].fn(target, !func_001B9558(self));
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep1_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_TextTitle[];
extern char str_EnterAccountName[];
extern char str_kT_CMNSubmit[];
extern char str_kT_TITLEEditAccount[];
extern char str_StaticAccountName[];
extern char str_LBInformYesNo[];
extern char str_LBShareInfoYesNo[];
extern char str_StaticContinue[];

class cFEVObj_001B8AA0 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25(int on);
};

static inline int IsHash1B8AA0(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Sets up account creation step 1's widgets as the screen creates them.
 *
 * When mbEditAccount is 0 (editing, see below) "TextTitle" shows "kT_TITLEEditAccount" and "StaticAccountName" /
 * "EnterAccountName" are locked (slots 25 and 8 with 1), so the name cannot change. "LBInformYesNo" and
 * "LBShareInfoYesNo" become yes/no lists (func_001A9A18). "StaticContinue" is stored in mpStaticContinue: locked
 * the same way when mbEditAccount is set, else labelled "kT_CMNSubmit". The code thus treats a nonzero
 * mbEditAccount as creating a new account, the opposite of its name.
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateCreateStep1_onWidgetCreate(cFEStateCreateStep1* self, cUIText* text)
{
    cFEVObj_001B8AA0* obj = (cFEVObj_001B8AA0*)text;
    if (IsHash1B8AA0(*(int*)((char*)text + 0x38), str_TextTitle)) {
        if (self->mbEditAccount == 0) {
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_TITLEEditAccount));
        }
    } else if (IsHash1B8AA0(*(int*)((char*)text + 0x38), str_StaticAccountName) || IsHash1B8AA0(*(int*)((char*)text + 0x38), str_EnterAccountName)) {
        if (self->mbEditAccount == 0) {
            obj->v25(1);
            obj->v08(1);
        }
    } else if (IsHash1B8AA0(*(int*)((char*)text + 0x38), str_LBInformYesNo) || IsHash1B8AA0(*(int*)((char*)text + 0x38), str_LBShareInfoYesNo)) {
        func_001A9A18((cFEStateNet*)self, (cUIListBox_001A9A18*)text, 1);
    } else if (IsHash1B8AA0(*(int*)((char*)text + 0x38), str_StaticContinue)) {
        if (self->mbEditAccount != 0) {
            obj->v25(1);
            obj->v08(1);
        } else {
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNSubmit));
        }
        self->mpStaticContinue = text;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep1_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" bool func_001B9558(cFEStateCreateStep1* self);
extern "C" void func_001B9820(cFEStateCreateStep1* self);
extern "C" int func_001BA110(cFEStateCreateStep1* self);
extern "C" void func_001BA218(cFEStateCreateStep1* self);
extern "C" void func_001BA280(cFEStateCreateStep1* self);
extern "C" void* cFEStateCreateStep2_cFEStateCreateStep2(cFEStateCreateStep2* self, int a1);
extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement(cFEStateOnlineAgreement* self, int a1);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" int cUIListBox_getCurrentEntryData(void* a);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_LBInformYesNo[];
extern char str_LBShareInfoYesNo[];
extern char str_StaticAccountName[];
extern char str_EnterAccountName[];
extern char str_kT_210AccountName[];
extern char str_StaticPassword[];
extern char str_EnterPassword[];
extern char str_kT_210Password[];
extern char str_StaticConfirmPassword[];
extern char str_EnterConfirmPassword[];
extern char str_kT_210ConfirmPassword[];
extern char str_StaticEmail[];
extern char str_EnterEmail[];
extern char str_kT_210EmailAddress[];
extern char str_StaticContinue[];
extern char str_cFEStateCreateStep2[];
extern char str_cFEStateNetAccountMem[];
extern const char str_cFEStateSelectPersona[];
extern char str_cFEStateOnlineAgreement[];

static inline int NameHash1B8C38(char* w)
{
    return *(int*)(w + 0x38);
}

struct sVE_001B8C38 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Handles account creation step 1's widget events: the text-entry keyboards, the yes/no boxes, continue and back.
 *
 * Event 9: "LBInformYesNo" / "LBShareInfoYesNo" store (selected id == 1) in mpAccount->mSpam[0] / [1]. Event 5
 * opens the keyboard (func_001A9150) on the matching entry box: "StaticAccountName" -> "EnterAccountName" (mName,
 * 16 chars, keyboard +0x58 = 1, keys 0x4B and 0x35 included), "StaticPassword" (mPassword, masked),
 * "StaticConfirmPassword" (unk_08, masked), "StaticEmail" (mMail, 50 chars); "StaticContinue", once func_001BA110
 * accepts the form, goes on to a new cFEStateCreateStep2 when mbEditAccount is set, else submits with
 * func_001BA280. Event 6 (back) pushes a cFEStateNetAccountMem popup (id 0x832) when mbEditAccount is set, else
 * restores the saved fields (func_001BA218) and returns to cFEStateSelectPersona. Event 13 re-reads the boxes
 * (func_001B9820) and updates Continue; event 8 opens cFEStateOnlineAgreement (miMiscData 0x14 or 0x15); 14 is
 * ignored; the rest go to the cFEStateNet handler.
 *
 * @param w  the widget (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cFEStateCreateStep1_onWidgetEvent(cFEStateCreateStep1* self, char* w, int msg)
{
    switch (msg) {
    case 9:
        if (NameHash1B8C38(w) == GetHashValue32(str_LBInformYesNo) && w != 0) {
            *(int*)((char*)self->mpAccount + 0x38) = cUIListBox_getCurrentEntryData(w) == 1;
        }
        if (NameHash1B8C38(w) == GetHashValue32(str_LBShareInfoYesNo) && w != 0) {
            *(int*)((char*)self->mpAccount + 0x3C) = cUIListBox_getCurrentEntryData(w) == 1;
        }
        break;
    case 5:
        if (NameHash1B8C38(w) == GetHashValue32(str_StaticAccountName)) {
            cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterAccountName));
            if (uiText != 0) {
                func_001A9150((cFEStateNet*)self, (int)uiText, GetHashValue32(str_kT_210AccountName), *(int*)((char*)self->mpAccount + 0x0), 0, 10, 0x10);
                *(int*)((char*)self->mpKeyboard + 0x58) = 1;
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(str_StaticPassword)) {
            cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterPassword));
            if (uiText != 0) {
                func_001A9150((cFEStateNet*)self, (int)uiText, GetHashValue32(str_kT_210Password), *(int*)((char*)self->mpAccount + 0x4), 1, 11, 0x10);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(str_StaticConfirmPassword)) {
            cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterConfirmPassword));
            if (uiText != 0) {
                func_001A9150((cFEStateNet*)self, (int)uiText, GetHashValue32(str_kT_210ConfirmPassword), *(int*)((char*)self->mpAccount + 0x8), 1, 12, 0x10);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(str_StaticEmail)) {
            cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterEmail));
            if (uiText != 0) {
                func_001A9150((cFEStateNet*)self, (int)uiText, GetHashValue32(str_kT_210EmailAddress), *(int*)((char*)self->mpAccount + 0xC), 0, 13, 50);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(str_StaticContinue)) {
            if (func_001BA110(self) != 0) {
                if (self->mbEditAccount != 0) {
                    func_001A8A40(((char*)self), cFEStateCreateStep2_cFEStateCreateStep2((cFEStateCreateStep2*)((cFEStateNet*)cMemMan_alloc(1760, str_cFEStateCreateStep2, 0, 0)), (int)self->mpEngine));
                } else {
                    func_001BA280(self);
                }
            }
        }
        break;
    case 6:
        if (self->mbEditAccount != 0) {
            void* newState = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, ((char*)self), 0, 0, 0);
            *(int*)((char*)newState + 0x18) = 2098;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, newState);
        } else {
            func_001BA218(self);
            func_001A8A40(((char*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        }
        break;
    case 13: {
        func_001B9820(self);
        char* staticContinue = (char*)self->mpStaticContinue;
        if (staticContinue != 0) {
            sVE_001B8C38* vt = *(sVE_001B8C38**)(staticContinue + 8);
            vt[8].fn(staticContinue + vt[8].delta, !func_001B9558(self));
        }
        break;
    }
    case 8: {
        void* newState = cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(1756, str_cFEStateOnlineAgreement, 0x100, 0), (int)self->mpEngine);
        if (self->mbEditAccount != 0) {
            *(int*)((char*)newState + 0x18) = 20;
        } else {
            *(int*)((char*)newState + 0x18) = 21;
        }
        func_001A8A40(((char*)self), newState);
        break;
    }
    case 14:
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)w, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9088);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP211CreateNewAccount[];
extern char str_kT_HELP211UniqueName[];
extern char str_kT_HELP211Password[];
extern char str_kT_HELP211ValidEmail[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];
extern char str_kT_HELPCmnMembershipAgreement[];
extern char str_kT_HELP211Note[];

extern "C" void func_001B9088(cFEStateCreateStep1* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP211CreateNewAccount);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP211UniqueName));
    func_001DA530(menu, 1, GetHashValue32(str_kT_HELP211Password));
    func_001DA530(menu, 2, GetHashValue32(str_kT_HELP211ValidEmail));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(menu, 3, GetHashValue32(str_kT_HELPCmnPrevious), 1);
    func_001DA510(menu, 4, GetHashValue32(str_kT_HELPCmnMembershipAgreement), 2);
    *(int*)((char*)menu + 0x64) = GetHashValue32(str_kT_HELP211Note);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B91C8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];
extern const char str_cFEStateSelectPersona[];

extern "C" void func_001B91C8(cFEStateCreateStep1* self, void* popup, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(self, popup, 15);
        void* accountMem = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, self, 1,
                                ((int*)self->mpAccount)[0], ((int*)self->mpAccount)[1]);
        *(int*)((char*)accountMem + 0x18) = 2092;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, accountMem);
        break;
    }
    case 16:
    case 20:
        func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        break;
    default:
        func_001A97B8(self, popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B92C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* mem, int a1);
extern const char str_cFEStateSelectPersona[];
extern char str_kT_POPUP204SaveAccount[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

// Tracks a pending flag across messages; on 0x16 either asks for confirmation or opens the next screen.
extern "C" void func_001B92C8(cFEStateCreateStep1* self, void* a1, int a2)
{
    switch (a2) {
    case 0xF: case 0x10: case 20:
        self->unk_6E8 = 0;
        func_001A97B8(self, a1, a2);
        break;
    case 17:
        self->unk_6E8 = 1;
        func_001A97B8(self, a1, a2);
        break;
    case 22:
        if (self->unk_6E8 != 0) {
            func_001A97B8(self, a1, a2);
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUP204SaveAccount);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            *(int*)((char*)p + 0x150) = 2078;
            *(int*)((char*)p + 0x168) = 0;
            *(int*)((char*)p + 0x164) = 0;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        }
        self->unk_6E8 = 0;
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep1_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B91C8(cFEStateCreateStep1* self, void* a1, int a2);
extern "C" void func_001B92C8(cFEStateCreateStep1* self, void* a1, int a2);

/**
 * Hands a popup's event on account creation step 1 to the handler for that popup's message id.
 *
 * The id is the popup's miMiscData (+0x18): 0x81E func_001B91C8, 0x82C (the save-account question) func_001B92C8;
 * any other the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateCreateStep1_onPopupEvent(cFEStateCreateStep1* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2078:
        func_001B91C8(self, pPopup, Event);
        break;
    case 2092:
        func_001B92C8(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9488);
#ifdef SKIP_ASM
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" int func_001A8E40_3i(void* self, int a1, int a2) __asm__("func_001A8E40");
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern char str_kT_POPUP204SaveAccount[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

extern "C" int func_001B9488(cFEStateCreateStep1* self, int msgId, int data)
{
    if (msgId == 206) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 2;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP204SaveAccount);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
        *(int*)((char*)popup + 0x168) = 0;
        *(int*)((char*)popup + 0x164) = 0;
        *(int*)((char*)popup + 0x150) = 2078;
        func_001A9BC0((cFEStateNet*)self, popup);
        return -1;
    }
    return func_001A8E40_3i(self, msgId, data);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9558);
#ifdef SKIP_ASM
// The string length is stored 8 bytes before the character data.
static inline bool has_9558(char* s)
{
    return *(int*)(s - 8) != 0;
}

extern "C" bool func_001B9558(cFEStateCreateStep1* self)
{
    char** names = (char**)self->mpAccount;
    bool ok = true;
    ok = ok && has_9558(names[0]);
    ok = ok && has_9558(names[1]);
    ok = ok && has_9558(names[2]);
    ok = ok && has_9558(names[3]);
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B95C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_EnterAccountName[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);

extern "C" void func_001B95C0(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterAccountName));
    if (text != 0) {
        cUIText_setAsciiString(text, *(const char**)self->mpAccount);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B9610);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_EnterPassword[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A98B8(cFEStateNet* self, const char* str);

extern "C" void func_001B9610(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterPassword));
    if (text != 0) {
        func_001A98B8((cFEStateNet*)self, ((const char**)self->mpAccount)[1]);
        cUIText_setAsciiString(text, (char*)self + 0x5C);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B9678);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_EnterConfirmPassword[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A98B8(cFEStateNet* self, const char* str);

extern "C" void func_001B9678(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterConfirmPassword));
    if (text != 0) {
        func_001A98B8((cFEStateNet*)self, ((const char**)self->mpAccount)[2]);
        cUIText_setAsciiString(text, (char*)self + 0x5C);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B96E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_EnterEmail[];
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001B9950(cFEStateCreateStep1* self, cUIText* text, char* str);

extern "C" void func_001B96E0(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterEmail));
    if (text != 0) {
        char* email = ((char**)self->mpAccount)[3];
        if (*(int*)(email - 8) != 0) {
            func_001B9950(self, text, email);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9740);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_LBInformYesNo[];

extern "C" void func_001B9740(cFEStateCreateStep1* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBInformYesNo));
    if (box != 0) {
        unsigned char out = 0;
        cUIListBox_getEntryByData(box, *(int*)((char*)self->mpAccount + 0x38) != 0, &out);
        cUIListBox_setCurrentEntryByIndex(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B97B0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_LBShareInfoYesNo[];

extern "C" void func_001B97B0(cFEStateCreateStep1* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBShareInfoYesNo));
    if (box != 0) {
        unsigned char out = 0;
        cUIListBox_getEntryByData(box, *(int*)((char*)self->mpAccount + 0x3C) != 0, &out);
        cUIListBox_setCurrentEntryByIndex(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9820);
#ifdef SKIP_ASM
struct cUIText;
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001A98B8(cFEStateNet* self, const char* str);
extern "C" void func_001B9950(cFEStateCreateStep1* self, cUIText* text, char* str);

extern "C" void func_001B9820(cFEStateCreateStep1* self)
{
    char* keyboard = (char*)self->mpKeyboard;
    switch (*(unsigned int*)(keyboard + 0x18)) {
    case 11:
        if (self->mpKeyboardText != 0) {
            func_001A98B8((cFEStateNet*)self, keyboard + 0x74);
            cUIText_setAsciiString(self->mpKeyboardText, (char*)self + 0x5C);
        }
        cBXString_cBXString4((char*)self->mpAccount + 0x4, (char*)self->mpKeyboard + 0x74);
        break;
    case 12:
        if (self->mpKeyboardText != 0) {
            func_001A98B8((cFEStateNet*)self, keyboard + 0x74);
            cUIText_setAsciiString(self->mpKeyboardText, (char*)self + 0x5C);
        }
        cBXString_cBXString4((char*)self->mpAccount + 0x8, (char*)self->mpKeyboard + 0x74);
        break;
    case 10:
        if (self->mpKeyboardText != 0) {
            cUIText_setAsciiString(self->mpKeyboardText, keyboard + 0x74);
        }
        cBXString_cBXString4((char*)self->mpAccount, (char*)self->mpKeyboard + 0x74);
        break;
    case 13: {
        cBXString_cBXString4((char*)self->mpAccount + 0xC, keyboard + 0x74);
        char* email = *(char**)((char*)self->mpAccount + 0xC);
        if (((int*)email)[-2] != 0) {
            func_001B9950(self, self->mpKeyboardText, email);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9950);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" char* cUIText_getFont(void* text);
extern "C" char* strcat(char*, const char*);
extern char D_004A1BD0[];

struct sVec3_1B9950 {
    float x, y, z;
};

struct sVec2_1B9950 {
    float x, y;
    sVec2_1B9950(float x_, float y_) : x(x_), y(y_) {}
};

struct sRect1B9950 {
    float x, y, w, h;
};

class cFEVObj_001B9950 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20(sRect1B9950* out);
};

static inline void Measure1B9950(cUIText* text, const char* s, sRect1B9950* out)
{
    char* f = cUIText_getFont(text);
    cFont_textExtent(f, s, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
}

extern "C" void func_001B9950(cFEStateCreateStep1* self, cUIText* text, char* str)
{
    int cut = 0;
    sVec3_1B9950 scale = *(sVec3_1B9950*)((char*)text + 0x50);
    sRect1B9950 box;
    ((cFEVObj_001B9950*)text)->v20(&box);
    char* f = cUIText_getFont(text);
    float sx = scale.x;
    float sy = scale.y;
    *(float*)(f + 0x38) = *(float*)(f + 0x30) * sx;
    *(float*)(f + 0x3C) = *(float*)(f + 0x34) * sy;
    int i = strlen(str) - 1;
    char buf[80];
    strcpy(buf, str);
    const char* ell = D_004A1BD0;
    sRect1B9950 sz;
    sRect1B9950 dots;
    Measure1B9950(text, ell, &dots);
    Measure1B9950(text, buf, &sz);
    while (box.x - dots.w < sz.w) {
        cut = 1;
        buf[i--] = 0;
        Measure1B9950(text, buf, &sz);
    }
    if (cut) {
        strcat(buf, ell);
        cUIText_setAsciiString(text, buf);
    } else {
        cUIText_setAsciiString(text, str);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9B18);
#ifdef SKIP_ASM
struct sObj1B9B18 {
    char data[0xD0];
};

int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* cPopupInfo_cPopupInfo(cPopupInfo* self);
extern "C" void cPopupInfo__cPopupInfo(cPopupInfo* self, int flags);
extern "C" int func_001B9CD8(cFEStateCreateStep1* self, const char* str);
extern "C" int strlen(const char* s);
extern char str_kT_POPUP210EmailMissing[];
extern char str_kT_POPUP210EmailLength[];
extern char str_kT_POPUP210EmailInvalid[];
extern char str_kT_BTNOK[];

extern "C" int func_001B9B18(cFEStateCreateStep1* self)
{
    int ok = 1;
    sObj1B9B18 popupInfo;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&popupInfo));
    int len = strlen(*(const char**)((char*)self->mpAccount + 0xC));
    if (len == 0) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210EmailMissing);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    }
    if (len < 7) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210EmailLength);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else if (!func_001B9CD8(self, *(const char**)((char*)self->mpAccount + 0xC))) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210EmailInvalid);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9CD8);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

extern "C" int func_001B9CD8(cFEStateCreateStep1* self, const char* str)
{
    int hasAt = 0;
    int hasDot = 0;
    int len = strlen(str);
    int i;
    for (i = 0; i < len; i++) {
        if (str[i] == '@') {
            hasAt = 1;
        } else if (str[i] == '.') {
            hasDot = 1;
        }
    }
    return hasAt && hasDot;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9D68);
#ifdef SKIP_ASM
struct sObj1B9D68 {
    char data[0xD0];
};

int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* cPopupInfo_cPopupInfo(cPopupInfo* self);
extern "C" void cPopupInfo__cPopupInfo(cPopupInfo* self, int flags);
extern "C" int func_001B9F68(cFEStateCreateStep1* self, signed char val, signed char lo, signed char hi);
extern "C" int strlen(const char* s);
extern char str_kT_POPUP210AccountNameMissing[];
extern char str_kT_POPUP210AccountNameLength[];
extern char str_kT_POPUP210AccountNameInvalid[];
extern char str_kT_BTNOK[];

extern "C" int func_001B9D68(cFEStateCreateStep1* self)
{
    int ok = 1;
    sObj1B9D68 popupInfo;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&popupInfo));
    int len = strlen(*(const char**)self->mpAccount);
    if (len == 0) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210AccountNameMissing);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else if (len < 4 || len > 16) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210AccountNameLength);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else {
        signed char firstChar = (*(signed char**)self->mpAccount)[0];
        if (firstChar > '0') {
            if (firstChar < '9') {
                void* popup = func_001A9C50((cFEStateNet*)self);
                *(int*)((char*)popup + 0xBC) = 1;
                char* sub = (char*)popup + 0xBC;
                *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210AccountNameInvalid);
                ok = 0;
                *(int*)((char*)popup + 0xC0) = 3;
                func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
                *(int*)((char*)popup + 0x150) = 2061;
                func_001A9BC0((cFEStateNet*)self, popup);
                goto done;
            }
        }
        int i;
        for (i = 0; i < len; i++) {
            signed char chr = (*(signed char**)self->mpAccount)[i];
            if (chr != ' ' && !func_001B9F68(self, chr, '0', '9') && !func_001B9F68(self, chr, 'a', 'z') && !func_001B9F68(self, chr, 'A', 'Z')) {
                ok = 0;
                void* popup = func_001A9C50((cFEStateNet*)self);
                *(int*)((char*)popup + 0xBC) = 1;
                char* sub = (char*)popup + 0xBC;
                *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210AccountNameInvalid);
                *(int*)((char*)popup + 0xC0) = 3;
                func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
                *(int*)((char*)popup + 0x150) = 2061;
                func_001A9BC0((cFEStateNet*)self, popup);
                break;
            }
        }
    }
done:
    cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9F68);
#ifdef SKIP_ASM
extern "C" int func_001B9F68(cFEStateCreateStep1* self, signed char val, signed char lo, signed char hi)
{
    if (val <= hi && val >= lo) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B9FA0);
#ifdef SKIP_ASM
struct sObj1B9FA0 {
    char data[0xD0];
};

int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* cPopupInfo_cPopupInfo(cPopupInfo* self);
extern "C" void cPopupInfo__cPopupInfo(cPopupInfo* self, int flags);
extern "C" int func_004165A8(const char* a, const char* b);
extern "C" int strlen(const char* s);
extern char str_kT_POPUP210PasswordMissing[];
extern char str_kT_POPUP210PasswordLength[];
extern char str_kT_POPUP210PasswordMatch[];
extern char str_kT_BTNOK[];

extern "C" int func_001B9FA0(cFEStateCreateStep1* self)
{
    int ok = 1;
    sObj1B9FA0 popupInfo;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&popupInfo));
    unsigned int len = strlen(*(const char**)((char*)self->mpAccount + 4));
    if (len == 0) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210PasswordMissing);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else if (len - 4 > 12) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210PasswordLength);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else if (func_004165A8(*(const char**)((char*)self->mpAccount + 4), *(const char**)((char*)self->mpAccount + 8))) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP210PasswordMatch);
        ok = 0;
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA110);
#ifdef SKIP_ASM
extern "C" int func_001BA110(cFEStateCreateStep1* self)
{
    bool ok = true;
    ok = ok && func_001B9D68(self);
    ok = ok && func_001B9FA0(self);
    ok = ok && func_001B9B18(self);
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA178);
#ifdef SKIP_ASM
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void cBXString_Reset(void* self);

extern "C" void func_001BA178(cFEStateCreateStep1* self)
{
    if (self->mbEditAccount == 0) {
        cBXString_operatorE((char*)self + 0x6D4, (char*)self->mpAccount + 0x4);
        cBXString_operatorE((char*)self + 0x6D8, (char*)self->mpAccount + 0x8);
        cBXString_operatorE((char*)self + 0x6DC, (char*)self->mpAccount + 0xC);
        char* account = (char*)self->mpAccount;
        self->miSavedSpam[0] = *(int*)(account + 0x38);
        self->miSavedSpam[1] = *(int*)(account + 0x3C);
    } else {
        cBXString_Reset((char*)self + 0x6D4);
        cBXString_Reset((char*)self + 0x6D8);
        cBXString_Reset((char*)self + 0x6DC);
    }
    *(int*)((char*)self->mpAccount + 0x4C) = 0;
    *(int*)((char*)self->mpAccount + 0x44) = 0;
    *(int*)((char*)self->mpAccount + 0x48) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA218);
#ifdef SKIP_ASM
extern "C" void* cBXString_operatorE(void* self, void* other);

extern "C" void func_001BA218(cFEStateCreateStep1* self)
{
    cBXString_operatorE((char*)self->mpAccount + 0x4, (char*)self + 0x6D4);
    cBXString_operatorE((char*)self->mpAccount + 0x8, (char*)self + 0x6D8);
    cBXString_operatorE((char*)self->mpAccount + 0xC, (char*)self + 0x6DC);
    *(int*)((char*)self->mpAccount + 0x38) = self->miSavedSpam[0];
    *(int*)((char*)self->mpAccount + 0x3C) = self->miSavedSpam[1];
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA280);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
extern "C" void func_001BA358(cFEStateCreateStep1* self);
extern "C" void func_0025BF18(void* self, void* a1);
extern const char str_cFEStateSelectPersona[];
extern char str_kT_POPUP210EditingAccount[];

extern "C" void func_001BA280(cFEStateCreateStep1* self)
{
    func_001BA358(self);
    char* account = (char*)self->mpAccount;
    if (*(int*)(account + 0x4C) == 0 && *(int*)(account + 0x44) == 0 && *(int*)(account + 0x48) == 0) {
        func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
    } else {
        void* pp = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)pp + 0xBC) = 0;
        *(int*)((char*)pp + 0xC4) = GetHashValue32(str_kT_POPUP210EditingAccount);
        *(int*)((char*)pp + 0xC0) = 3;
        *(int*)((char*)pp + 0x150) = 2077;
        *(int*)((char*)pp + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, pp);
        func_0025BF18(LobbyMan_mLobbyMan, self->mpAccount);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA358);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const char* a, const char* b);

extern "C" void func_001BA358(cFEStateCreateStep1* self)
{
    if (func_004165A8(*(char**)((char*)self->mpAccount + 0x4), self->mszSavedPassword) != 0) {
        *(int*)((char*)self->mpAccount + 0x44) = 1;
    }
    if (func_004165A8(*(char**)((char*)self->mpAccount + 0xC), self->mszSavedMail) != 0) {
        *(int*)((char*)self->mpAccount + 0x4C) = 1;
    }
    char* account = (char*)self->mpAccount;
    if (*(int*)(account + 0x38) != self->miSavedSpam[0] || *(int*)(account + 0x3C) != self->miSavedSpam[1]) {
        *(int*)(account + 0x48) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep2_cFEStateCreateStep2);
#ifdef SKIP_ASM
extern void* cFEStateCreateStep2_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs step 2 of online account creation ("211create_step2": birth date, gender, legal box).
 *
 * Runs the cFEStateNet constructor (func_001A8500, no parent), stores mHashName 0x4E and cFEStateCreateStep2_vtbl
 * and clears unk_6A8.
 *
 * @param pEngine  the UI engine (On Tour cUIState ctor pEngine)
 * @return @p self
 */
extern "C" void* cFEStateCreateStep2_cFEStateCreateStep2(cFEStateCreateStep2* self, int pEngine)
{
    func_001A8500_3(((cFEStateNet*)self), pEngine, 0);
    self->mHashName = 78;
    self->mVtbl = cFEStateCreateStep2_vtbl;
    self->unk_6A8 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep2_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern "C" void func_001BB2D0(cFEStateCreateStep2* self);
extern "C" void func_001BB340(cFEStateCreateStep2* self);
extern "C" void func_001BB3B0(cFEStateCreateStep2* self);
extern "C" void func_001BB440(cFEStateCreateStep2* self);
extern "C" void func_001BB4B0(cFEStateCreateStep2* self);
extern char str_211create_step2[];


/**
 * Opens the "211create_step2" screen on LobbyMan's new-account form and fills its boxes.
 *
 * Clears mbScreenReady, points mpAccount at LobbyMan's mNewAccount (+0x260), adds and plays the screen, clears the
 * form's unk_40 and unk_6DC, fills the boxes (func_001BB2D0 .. func_001BB4B0), runs the empty hook func_001A8768
 * and sets mbScreenReady.
 */
extern "C" void cFEStateCreateStep2_onCreateScreen(cFEStateCreateStep2* self)
{
    self->mbScreenReady = 0;
    self->mpAccount = (tLobbyAccount*)((char*)LobbyMan_mLobbyMan + 0x260);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, (self), GetHashValue32(str_211create_step2), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self->mpAccount + 0x40) = 0;
    self->unk_6DC = 0;
    func_001BB2D0(self);
    func_001BB340(self);
    func_001BB3B0(self);
    func_001BB440(self);
    func_001BB4B0(self);
    func_001A8768((self));
    self->mbScreenReady = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep2_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Runs one frame of account creation step 2: only the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (the pending-event handler's value)
 */
void* cFEStateCreateStep2_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep2_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001BB520(cFEStateCreateStep2* self, void* box, int flag);
extern "C" void func_001BB5D0(cFEStateCreateStep2* self, void* box, int a2);
extern "C" void func_001BB798(cFEStateCreateStep2* self, void* box, int value);
extern "C" void func_001BB840(cFEStateCreateStep2* self, void* box, int a2, int a3, int a4);
extern char str_LBLegalYesNo[];
extern char str_LBMonth[];
extern char str_LBDay[];
extern char str_LBYear[];
extern char str_LBGender[];

static inline int IsHash1BA500(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Fills account creation step 2's list boxes as the screen creates them.
 *
 * "LBLegalYesNo" becomes a yes/no list (func_001A9A18, default 0), "LBMonth" the months (func_001BB5D0), "LBDay"
 * the days of month 1 of 1985 (func_001BB840), "LBYear" the years from 1985 (func_001BB798) and "LBGender" the
 * genders (func_001BB520).
 *
 * @param box  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateCreateStep2_onWidgetCreate(cFEStateCreateStep2* self, void* box)
{
    if (IsHash1BA500(*(int*)((char*)box + 0x38), str_LBLegalYesNo)) {
        func_001A9A18((cFEStateNet*)self, (cUIListBox_001A9A18*)box, 0);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), str_LBMonth)) {
        func_001BB5D0(self, box, 1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), str_LBDay)) {
        func_001BB840(self, box, 1, 1985, 1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), str_LBYear)) {
        func_001BB798(self, box, 1985);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), str_LBGender)) {
        func_001BB520(self, box, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep2_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cBXString_Reset(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIListBox_setEntryByAsciiString(void* box, int index, const char* str, int a3);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1(cFEStateCreateStep1* self, int a1, int a2);
extern "C" void func_001BA9F8(cFEStateCreateStep2* self, int a1);
extern "C" void func_001BB840(cFEStateCreateStep2* self, void* box, int a2, int a3, int a4);
int func_001BBAA8(void* self);
extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement(cFEStateOnlineAgreement* self, int a1);
extern "C" int cUIListBox_getCurrentEntryData(void* a);
extern char str_LBMonth[];
extern char str_LBDay[];
extern char str_LBYear[];
extern char str_pct_d_b[];
extern char str_LBLegalYesNo[];
extern char str_LBGender[];
extern char str_StaticSubmit[];
extern char str_cFEStateCreateStep1[];
extern char str_cFEStateOnlineAgreement[];

static inline int IsHash_001BA618(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Handles account creation step 2's widget events: the birth-date, gender and legal boxes, submit and back.
 *
 * Event 9 (only once mbScreenReady is set) copies the box's selected id into the form: "LBMonth" mBornMonth,
 * "LBYear" mBornYear (row + 1900, shown with "%d" in row 0; also resets mParentMail), both refilling "LBDay" for
 * the month and year (func_001BB840); "LBDay" mBornDay, "LBGender" mGender and "LBLegalYesNo" unk_40 (id == 1).
 * Event 5 on "StaticSubmit" submits (func_001BA9F8(self, 1)) once func_001BBAA8 accepts the form. Event 6 returns
 * to a new cFEStateCreateStep1 (flag 1); event 8 opens cFEStateOnlineAgreement with miMiscData 0x16; the rest go to
 * the cFEStateNet handler.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void cFEStateCreateStep2_onWidgetEvent(cFEStateCreateStep2* self, int pWidget, unsigned int msg)
{
    char* obj = (char*)pWidget;
    switch (msg) {
    case 9:
        if (self->mbScreenReady != 0) {
            if (IsHash_001BA618(*(int*)(obj + 0x38), str_LBMonth)) {
                *(int*)((char*)self->mpAccount + 0x28) = cUIListBox_getCurrentEntryData(obj);
                cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBDay));
                if (box != 0) {
                    char* account = (char*)self->mpAccount;
                    func_001BB840(self, box, *(int*)(account + 0x28), *(int*)(account + 0x30), *(int*)(account + 0x2C));
                }
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), str_LBYear)) {
                char buf[16];
                *(int*)((char*)self->mpAccount + 0x30) = *(unsigned char*)(obj + 0x319) + 1900;
                sprintf(buf, str_pct_d_b, *(int*)((char*)self->mpAccount + 0x30));
                cUIListBox_setEntryByAsciiString(obj, 0, buf, 0);
                cBXString_Reset((char*)self->mpAccount + 0x10);
                cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBDay));
                if (box != 0) {
                    char* account = (char*)self->mpAccount;
                    func_001BB840(self, box, *(int*)(account + 0x28), *(int*)(account + 0x30), *(int*)(account + 0x2C));
                }
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), str_LBDay)) {
                *(int*)((char*)self->mpAccount + 0x2C) = cUIListBox_getCurrentEntryData(obj);
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), str_LBGender)) {
                *(int*)((char*)self->mpAccount + 0x34) = cUIListBox_getCurrentEntryData(obj);
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), str_LBLegalYesNo)) {
                *(int*)((char*)self->mpAccount + 0x40) = cUIListBox_getCurrentEntryData(obj) == 1;
            }
        }
        break;
    case 5:
        if (IsHash_001BA618(*(int*)(obj + 0x38), str_StaticSubmit)) {
            if (func_001BBAA8(self) != 0) {
                func_001BA9F8(self, 1);
            }
        }
        break;
    case 6:
        func_001A8A40(self, cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(1780, str_cFEStateCreateStep1, 0, 0), (int)self->mpEngine, 1));
        break;
    case 8: {
        char* agreement = (char*)cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(1756, str_cFEStateOnlineAgreement, 0x100, 0), (int)self->mpEngine);
        *(int*)(agreement + 0x18) = 22;
        func_001A8A40(self, agreement);
        break;
    }
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA8B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP211CreateNewAccount[];
extern char str_kT_HELP211UniqueName[];
extern char str_kT_HELP211Password[];
extern char str_kT_HELP211ValidEmail[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];
extern char str_kT_HELPCmnMembershipAgreement[];
extern char str_kT_HELP211Note[];

extern "C" void func_001BA8B8(cFEStateCreateStep2* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP211CreateNewAccount);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP211UniqueName));
    func_001DA530(menu, 1, GetHashValue32(str_kT_HELP211Password));
    func_001DA530(menu, 2, GetHashValue32(str_kT_HELP211ValidEmail));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(menu, 3, GetHashValue32(str_kT_HELPCmnPrevious), 1);
    func_001DA510(menu, 4, GetHashValue32(str_kT_HELPCmnMembershipAgreement), 2);
    *(int*)((char*)menu + 0x64) = GetHashValue32(str_kT_HELP211Note);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BA9F8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void func_0025B6D8(void* self, int a1);
extern "C" void func_0025B9C8(void* self, void* a1);
extern char str_kT_POPUP211Submitting[];

extern "C" void func_001BA9F8(cFEStateCreateStep2* self, int a1)
{
    void* popup = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)popup + 0xBC) = 0;
    *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211Submitting);
    *(int*)((char*)popup + 0xC0) = 3;
    *(int*)((char*)popup + 0x164) = 0;
    *(int*)((char*)popup + 0x150) = 2080;
    func_001A9BC0((cFEStateNet*)self, popup);
    if (a1 != 0) {
        func_0025B6D8(LobbyMan_mLobbyMan, 0);
    } else {
        func_0025B9C8(LobbyMan_mLobbyMan, (char*)LobbyMan_mLobbyMan + 0x260);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateStep2_onPopupEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_001BB158(cFEStateCreateStep2* self, void* a1, unsigned int a2);
extern "C" void func_001BB0E8(cFEStateCreateStep2* self, void* a1, int a2);
extern "C" void func_001BB098(cFEStateCreateStep2* self, void* a1, int a2);
extern "C" void func_001BADE8(cFEStateCreateStep2* self, void* a1, int a2);
extern "C" void func_001BAED8(cFEStateCreateStep2* self, void* a1, int a2);
/**
 * Hands a popup's event on account creation step 2 to the handler for that popup's message id.
 *
 * The id is the popup's miMiscData (+0x18): 0x81F func_001BB158, 0x821 func_001BB0E8, 0x822 func_001BB098, 0x80E
 * func_001BADE8, 0x82C (save account) func_001BAED8; any other the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
// Dispatches a message to the matching handler, falling back to the base handler.
extern "C" void cFEStateCreateStep2_onPopupEvent(cFEStateCreateStep2* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2079:
        func_001BB158(self, pPopup, Event);
        break;
    case 2081:
        func_001BB0E8(self, pPopup, Event);
        break;
    case 2082:
        func_001BB098(self, pPopup, Event);
        break;
    case 2062:
        func_001BADE8(self, pPopup, Event);
        break;
    case 2092:
        func_001BAED8(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BAB28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025B9C8(void* self, void* a1);
extern "C" void func_00260F10(void* self);
extern "C" void func_001BAC90(cFEStateCreateStep2* self, void* msg);
extern "C" int func_001A8E40_3i(void* self, int a1, int a2) __asm__("func_001A8E40");
extern const char str_cFEStateSelectPersona[];
extern char str_kT_POPUP211Congratulations2[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

extern "C" int func_001BAB28(cFEStateCreateStep2* self, int id, int data)
{
    switch (id) {
    case 201:
        func_0025B9C8(LobbyMan_mLobbyMan, (char*)LobbyMan_mLobbyMan + 0x260);
        break;
    case 203: {
        void* popup = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)popup + 0xBC) = 2;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211Congratulations2);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
        *(int*)((char*)popup + 0x164) = 0;
        *(int*)((char*)popup + 0x150) = 2079;
        func_001A9BC0(((cFEStateNet*)self), popup);
        break;
    }
    case 212:
        func_00260F10(LobbyMan_mLobbyMan);
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        break;
    case 258:
        func_001BAC90(self, (void*)data);
        break;
    default:
        return func_001A8E40_3i(((cFEStateNet*)self), id, data);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BAC90);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_001BBAC8(void* self, int state);
extern "C" void* cFEStateCreateDuplicate_cFEStateCreateDuplicate(cFEStateCreateDuplicate* self, int a1, int a2);
// PORT: unit declares func_001A8E40 with one arg; the real handler takes (self, id, msg)
extern "C" void* func_001A8E40_msg(void* self, int id, void* msg) __asm__("func_001A8E40");
extern char str_cFEStateCreateDuplicate[];
extern char str_kT_POPUP211InvalidParentalEmail[];
extern char str_kT_BTNOK[];

extern "C" void func_001BAC90(cFEStateCreateStep2* self, void* msg)
{
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x6475706C:
        func_001A8A40(self, cFEStateCreateDuplicate_cFEStateCreateDuplicate((cFEStateCreateDuplicate*)cMemMan_alloc(1760, str_cFEStateCreateDuplicate, 0, 0), (int)self->mpEngine, 1));
        break;
    case 0x746F6F79:
        func_001BBAC8(self, *(int*)((char*)msg + 0xC));
        break;
    case 0x706D616C:
        // PORT: string length lives in a header before the character data
        if (*(int*)(*(int*)((char*)self->mpAccount + 0x10) - 8) != 0) {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 1;
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211InvalidParentalEmail);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)popup + 0x150) = 2082;
            func_001A9BC0((cFEStateNet*)self, popup);
        } else {
            func_001BBAC8(self, 17);
        }
        break;
    default:
        func_001A8E40_msg(self, *(int*)msg, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BADE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];

static inline void* GetSession_001BADE8()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001BADE8(cFEStateCreateStep2* self, void* popup, int event)
{
    if (event == 0xF || event == 20) {
        func_001A97B8_v(((cFEStateNet*)self), popup, event);
        func_00261408(GetSession_001BADE8());
        func_0025B800(LobbyMan_mLobbyMan);
        void* accountMem = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 0, 0, 0);
        *(int*)((char*)accountMem + 0x18) = 2098;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, accountMem);
    } else {
        func_001A97B8_v(((cFEStateNet*)self), popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BAED8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
// PORT: the unit declares func_001A97B8 as returning int; this caller ignores the result.
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_0025C610(void* net);
extern char str_kT_POPUP204SaveAccount[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char str_kT_POPUP201Verify[];
extern char str_kT_BTNCancel[];

// Tracks a pending flag; on 0x16 shows either the confirm popup or the network-connect popup.
extern "C" void func_001BAED8(cFEStateCreateStep2* self, void* a1, int a2)
{
    switch (a2) {
    case 0xF: case 0x10: case 20:
        self->unk_6D8 = 0;
        func_001A97B8_v(self, a1, a2);
        break;
    case 17:
        self->unk_6D8 = 1;
        func_001A97B8_v(self, a1, a2);
        break;
    case 22:
        func_001A97B8_v(self, a1, a2);
        if (self->unk_6D8 != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUP204SaveAccount);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            *(int*)((char*)p + 0x164) = 0;
            *(int*)((char*)p + 0x150) = 2079;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 1;
            *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUP201Verify);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNCancel));
            *(int*)((char*)p + 0x150) = 2062;
            func_001A9BC0((cFEStateNet*)self, p);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, *(char**)self->mpAccount);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, ((char**)self->mpAccount)[1]);
            func_0025C610(LobbyMan_mLobbyMan);
        }
        self->unk_6D8 = 0;
        break;
    default:
        func_001A97B8_v(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB098);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BBAC8(void* self, int state);

extern "C" void func_001BB098(cFEStateCreateStep2* self, void* popup, int event)
{
    if (event == 22) {
        func_001A97B8(self, popup, event);
        func_001BBAC8(self, 17);
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB0E8);
#ifdef SKIP_ASM
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BA9F8(cFEStateCreateStep2* self, int a1);

extern "C" void func_001BB0E8(cFEStateCreateStep2* self, void* popup, int event)
{
    if (event == 0xF) {
        func_001A97B8(self, popup, 0xF);
        cBXString_cBXString4((char*)self->mpAccount + 0x10, *(const char**)((char*)popup + 0x2B0));
        func_001BA9F8(self, 0);
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB158);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025C610(void* self);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];
extern char str_kT_POPUP201Verify[];
extern char str_kT_BTNCancel[];

extern "C" void func_001BB158(cFEStateCreateStep2* self, void* popup, unsigned int kind)
{
    switch (kind) {
    case 15: {
        func_001A97B8(self, popup, 15);
        void* newPopup = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, self, 1,
                                *(int*)self->mpAccount, *(int*)((char*)self->mpAccount + 4));
        *(int*)((char*)newPopup + 0x18) = 2092;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, newPopup);
        break;
    }
    case 16:
    case 20: {
        func_001A97B8(self, popup, kind);
        void* newPopup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)newPopup + 0xBC) = 1;
        char* sub = (char*)newPopup + 0xBC;
        *(int*)((char*)newPopup + 0xC4) = GetHashValue32(str_kT_POPUP201Verify);
        *(int*)((char*)newPopup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNCancel));
        *(int*)((char*)newPopup + 0x150) = 2062;
        func_001A9BC0((cFEStateNet*)self, newPopup);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, *(const char**)self->mpAccount);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, *(const char**)((char*)self->mpAccount + 4));
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    }
    default:
        func_001A97B8(self, popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB2D0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_LBMonth[];

extern "C" void func_001BB2D0(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBMonth));
    if (box != 0) {
        unsigned char out = 0;
        cUIListBox_getEntryByData(box, *(int*)((char*)self->mpAccount + 0x28), &out);
        cUIListBox_setCurrentEntryByIndex(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB340);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_LBDay[];

extern "C" void func_001BB340(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBDay));
    if (box != 0) {
        unsigned char out = 0;
        cUIListBox_getEntryByData(box, *(int*)((char*)self->mpAccount + 0x2C), &out);
        cUIListBox_setCurrentEntryByIndex(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB3B0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIListBox_setEntryByAsciiString(void* box, int index, const char* str, int a3);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_LBYear[];
extern char str_pct_d_b[];

extern "C" void func_001BB3B0(cFEStateCreateStep2* self)
{
    char buf[16];
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBYear));
    if (box != 0) {
        sprintf(buf, str_pct_d_b, *(int*)((char*)self->mpAccount + 0x30));
        cUIListBox_setEntryByAsciiString(box, 0, buf, 0);
        cUIListBox_setCurrentEntryByIndex(box, (unsigned char)(*(int*)((char*)self->mpAccount + 0x30) - 108));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB440);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_LBGender[];

extern "C" void func_001BB440(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBGender));
    if (box != 0) {
        unsigned char out = 0;
        cUIListBox_getEntryByData(box, *(int*)((char*)self->mpAccount + 0x34), &out);
        cUIListBox_setCurrentEntryByIndex(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB4B0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_LBLegalYesNo[];

extern "C" void func_001BB4B0(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_LBLegalYesNo));
    if (box != 0) {
        unsigned char out = 0;
        cUIListBox_getEntryByData(box, *(int*)((char*)self->mpAccount + 0x40) != 0, &out);
        cUIListBox_setCurrentEntryByIndex(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB520);
#ifdef SKIP_ASM
class cFEVBox_001BB520 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_kT_CMNMale[];
extern char str_kT_CMNFemale[];

extern "C" void func_001BB520(cFEStateCreateStep2* self, void* box, int flag)
{
    unsigned char out = 0;
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNMale), 0);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNFemale), 1);
    cUIListBox_getEntryByData(box, flag == 0, &out);
    cUIListBox_setCurrentEntryByIndex(box, out);
    ((cFEVBox_001BB520*)box)->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB5D0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_kT_CMNJanuary[];
extern char str_kT_CMNFebruary[];
extern char str_kT_CMNMarch[];
extern char str_kT_CMNApril[];
extern char str_kT_CMNMay[];
extern char str_kT_CMNJune[];
extern char str_kT_CMNJuly[];
extern char str_kT_CMNAugust[];
extern char str_kT_CMNSeptember[];
extern char str_kT_CMNOctober[];
extern char str_kT_CMNNovember[];
extern char str_kT_CMNDecember[];

class cFEVObj_001BB5D0 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001BB5D0(cFEStateCreateStep2* self, void* box, int a2)
{
    unsigned char out = 0;
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNJanuary), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNFebruary), 2);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNMarch), 3);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNApril), 4);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNMay), 5);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNJune), 6);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNJuly), 7);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNAugust), 8);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNSeptember), 9);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNOctober), 10);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNNovember), 11);
    cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNDecember), 12);
    cUIListBox_getEntryByData(box, 1, &out);
    cUIListBox_setCurrentEntryByIndex(box, out);
    cUIListBox_setCurrentEntryByIndex(box, 1);
    ((cFEVObj_001BB5D0*)box)->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB798);
#ifdef SKIP_ASM
class cFEVBox_001BB798 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIListBox_setEntryByAsciiString(void* box, int index, const char* str, int a3);
extern "C" void func_0039A670(void* self);
extern "C" void cUIListBox_setCallbackListBox(void* self, int count);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_pct_d_b[];

extern "C" void func_001BB798(cFEStateCreateStep2* self, void* box, int value)
{
    char buf[16];
    func_0039A670(box);
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_setCallbackListBox(box, 103);
    sprintf(buf, str_pct_d_b, value);
    cUIListBox_setEntryByAsciiString(box, 0, buf, 0);
    cUIListBox_setCurrentEntryByIndex(box, (unsigned char)(value - 108));
    ((cFEVBox_001BB798*)box)->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB840);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_001BB9B0(cFEStateCreateStep2* self, int month, int year);
extern "C" void func_0039A670(void* self);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern char str_pct_d_b[];

class cFEVObj_001BB840 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

extern "C" void func_001BB840(cFEStateCreateStep2* self, void* box, int month, int year, int sel)
{
    char buf[16];
    unsigned char idx;
    int n = func_001BB9B0(self, month, year);
    func_0039A670(box);
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    int i;
    for (i = 1; i < n + 1; i++) {
        sprintf(buf, str_pct_d_b, i);
        cUIListBox_addEntryByAsciiString(box, buf, i);
    }
    if (n < sel) {
        sel = n;
        *(int*)((char*)self->mpAccount + 0x2C) = sel;
    }
    idx = 0;
    cUIListBox_getEntryByData(box, sel, &idx);
    cUIListBox_setCurrentEntryByIndex(box, idx);
    ((cFEVObj_001BB840*)box)->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB968);
#ifdef SKIP_ASM
extern "C" int func_001BB968(cFEStateCreateStep2* self, int year)
{
    int leap = 0;
    if ((year & 3) == 0) {
        leap = 1;
        if (year % 100 == 0) {
            leap = (year % 400 == 0);
        }
    }
    return leap;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB9B0);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Days in a month (1-based); February uses the leap-year test.
extern "C" int func_001BB9B0(cFEStateCreateStep2* self, int month, int year)
{
    if (month == 2) {
        if (func_001BB968(self, year))
            return 29;
        return 28;
    }
    if (month >= 8)
        month++;
    if (month % 2 == 1)
        return 31;
    return 30;
}

extern "C" void* func_001BBC40(void* self);
#endif

extern "C" void* func_001BBC40(void* self);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBA18);
#ifdef SKIP_ASM
extern "C" int func_001BBA18(cFEStateCreateStep2* self, const char* email)
{
    int hasDot = 0;
    int hasAt = 0;
    int n = strlen(email);
    int i;
    for (i = 0; i < n; i++) {
        if (email[i] == '@')
            hasAt = 1;
        else if (email[i] == '.')
            hasDot = 1;
    }
    if (hasAt)
        return hasDot != 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBAA8__FPv);
#ifdef SKIP_ASM
int func_001BBAA8(void* self)
{
    return (func_001BBC40(self) != 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBAC8);
#ifdef SKIP_ASM
extern char str_kT_POPUP211UnderAge[];
extern char str_kT_POPUPParentEmail[];
extern char str_kT_POPUP211Age13to17[];

struct sObj1BBAC8 {
    char data[0xD0];
};

// Shows the error popup for a failed network operation; returns 1 if the code needs no popup.
// PORT: callers declare func_001BBAC8 as void; defined under an asm label with its int result.
extern "C" int func_001BBAC8_impl(void* self, int code) __asm__("func_001BBAC8");
extern "C" int func_001BBAC8_impl(void* self, int code)
{
    sObj1BBAC8 popupInfo;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&popupInfo));
    if (code < 13) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211UnderAge);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
        cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
        return 0;
    } else if (code < 18) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = -1;
        *(int*)((char*)popup + 0x180) = 50;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xCC) = GetHashValue32(str_kT_POPUPParentEmail);
        *(int*)((char*)popup + 0xC8) = 3;
        *(int*)((char*)popup + 0xD4) = GetHashValue32(str_kT_POPUPParentEmail);
        *(int*)((char*)popup + 0xD0) = 3;
        *(int*)((char*)popup + 0xDC) = *(int*)(*(char**)((char*)self + 0x6D0) + 0x10);
        *(int*)((char*)popup + 0xD8) = 2;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211Age13to17);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2081;
        func_001A9BC0((cFEStateNet*)self, popup);
        cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
        return 0;
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBC40);
#ifdef SKIP_ASM
struct sObj1BBC40 {
    char data[0xD0];
};

int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* cPopupInfo_cPopupInfo(cPopupInfo* self);
extern "C" void cPopupInfo__cPopupInfo(cPopupInfo* self, int flags);
extern char str_kT_POPUP211AcceptLegal[];
extern char str_kT_BTNOK[];
// PORT: the unit declares func_001BBC40 as returning void*; the body returns 0/1
extern "C" int func_001BBC40_r(void* self) __asm__("func_001BBC40");

extern "C" int func_001BBC40_r(void* self)
{
    sObj1BBC40 popupInfo;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&popupInfo));
    if (*(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x40) == 0) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211AcceptLegal);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2061;
        func_001A9BC0((cFEStateNet*)self, popup);
        cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
        return 0;
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&popupInfo), 2);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateDuplicate_cFEStateCreateDuplicate);
#ifdef SKIP_ASM
extern void* cFEStateCreateDuplicate_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the "name already taken" state for an account name or a persona name.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with no parent, stores the kind in mbAccount, the vtable and
 * mHashName 0x4F, and clears unk_6A8.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @param bAccount  nonzero for an account name, 0 for a persona name (stored in mbAccount)
 * @return @p self
 */
extern "C" void* cFEStateCreateDuplicate_cFEStateCreateDuplicate(cFEStateCreateDuplicate* self, int pEngine, int bAccount)
{
    func_001A8500_3(self, pEngine, 0);
    self->mbAccount = bAccount;
    self->mVtbl = cFEStateCreateDuplicate_vtbl;
    self->mHashName = 79;
    self->unk_6A8 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateDuplicate_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern "C" void func_001BC400(cFEStateCreateDuplicate* self);
extern char str_212create_duplicate[];

/**
 * Creates the "212create_duplicate" screen and shows the suggested names.
 *
 * Points mpAccount at LobbyMan's new-account form (+0x260), adds and plays the screen, lists the suggestions
 * (func_001BC400) and runs the cFEStateNet hook (func_001A8768).
 */
extern "C" void cFEStateCreateDuplicate_onCreateScreen(cFEStateCreateDuplicate* self)
{
    self->mpAccount = (tLobbyAccount*)((char*)LobbyMan_mLobbyMan + 0x260);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_212create_duplicate), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001BC400(self);
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateDuplicate_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the duplicate-name state: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateCreateDuplicate_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateDuplicate_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_SelectNew[];
extern char str_TextTitle[];
extern char str_kT_TITLEDuplicateUser[];
extern char str_StaticMenuName[];
extern char str_kT_212SuggestedUser[];
extern char str_SelectNamepct_d[];

static inline int IsHash1BBE00(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Numbers the duplicate-name widgets as the screen creates them.
 *
 * "SelectNew" gets id 4. For a persona name (mbAccount 0), "TextTitle" shows "kT_TITLEDuplicateUser" and
 * "StaticMenuName" "kT_212SuggestedUser". "SelectName0".."SelectName3" get ids 0-3 and, until func_001BC400 fills
 * them, their own widget name as text.
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateCreateDuplicate_onWidgetCreate(cFEStateCreateDuplicate* self, cUIText* text)
{
    char buf[0x40];
    if (IsHash1BBE00(*(int*)((char*)text + 0x38), str_SelectNew)) {
        *(int*)((char*)text + 0x18) = 4;
    } else if (self->mbAccount == 0 && IsHash1BBE00(*(int*)((char*)text + 0x38), str_TextTitle)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_TITLEDuplicateUser));
    } else if (self->mbAccount == 0 && IsHash1BBE00(*(int*)((char*)text + 0x38), str_StaticMenuName)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_212SuggestedUser));
    } else {
        int i;
        for (i = 0; i < 4; i++) {
            sprintf(buf, str_SelectNamepct_d, i);
            if (IsHash1BBE00(*(int*)((char*)text + 0x38), buf)) {
                cUIText_setAsciiString(text, buf);
                *(int*)((char*)text + 0x18) = i;
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateDuplicate_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void func_001BC290(cFEStateCreateDuplicate* self);
extern "C" void func_001BC300(cFEStateCreateDuplicate* self, const char* name);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_0025B800(void* self);
extern "C" void func_00260F80();
extern "C" void func_002613C8(void* self);
extern "C" void func_00261408(void* self);
extern "C" int func_00261460(void* self);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char D_004A1408[];
extern char str_cFEStateNetAccountMem[];
extern const char str_cFEStateSelectPersona[];
extern char str_kT_TITLEDuplicateUser[];
extern char str_kT_TITLEDuplicateAccount[];


static inline void* GetSession_001BBF18()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

/**
 * Handles the duplicate-name widgets: take a suggested name, type a new one, or back out.
 *
 * Event 5 on "SelectNew" (id 4) opens the keyboard titled "kT_TITLEDuplicateAccount" (16 characters) or
 * "kT_TITLEDuplicateUser" (12); on a suggestion (id 0-3) assigns it to the account's mName and submits
 * (func_001BC290), or for a persona passes the name to func_001BC300. Event 6 for an account disconnects BuddyMan and
 * LobbyMan and pushes a cFEStateNetAccountMem popup (0x832); for a persona switches back to a new
 * cFEStateSelectPersona. Event 13 (keyboard done) uses the typed name the same way. The rest go to the cFEStateNet
 * handler.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget); reused for its id under event 5
 * @param msg  the widget event (On Tour Event)
 */
// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void cFEStateCreateDuplicate_onWidgetEvent(cFEStateCreateDuplicate* self, int pWidget, unsigned int msg)
{
    switch (msg) {
    case 5: {
        pWidget = *(int*)((char*)pWidget + 0x18);
        if (pWidget == 4) {
            if (self->mbAccount != 0) {
                // PORT: string pointer passed through an int parameter (func_001A9150's a3)
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(str_kT_TITLEDuplicateAccount), (int)D_004A1408, 0, -1, 0x10);
                *(int*)((char*)self->mpKeyboard + 0x58) = 1;
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            } else {
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(str_kT_TITLEDuplicateUser), (int)D_004A1408, 0, -1, 0xC);
                *(int*)((char*)self->mpKeyboard + 0x58) = 1;
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            }
        } else if (self->mbAccount != 0) {
            tLobbyAccount* lobbyAccount = self->mpAccount;
            cBXString_operatorE(lobbyAccount, &((char**)&lobbyAccount->mNameOptions[0])[pWidget]);
            func_001BC290(self);
        } else {
            func_001BC300(self, ((char**)((char*)self->mpAccount + 0x14))[pWidget]);
        }
        break;
    }
    case 6:
        if (self->mbAccount != 0) {
            func_00261408(GetSession_001BBF18());
            func_0025B800(LobbyMan_mLobbyMan);
            void* popup = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, self, 0, 0, 0);
            *(int*)((char*)popup + 0x18) = 2098;
            cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, popup);
        } else {
            func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        }
        break;
    case 13:
        if (self->mbAccount != 0) {
            cBXString_cBXString4(self->mpAccount, (char*)self->mpKeyboard + 0x74);
            func_001BC290(self);
        } else {
            func_001BC300(self, (char*)self->mpKeyboard + 0x74);
        }
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC190);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP212DuplicateAccountName[];
extern char str_kT_HELP212DuplicateAccountNameMessage[];
extern char str_kT_HELP212DuplicatePersona[];
extern char str_kT_HELP212DuplicatePersonaMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001BC190(cFEStateCreateDuplicate* self, void* menu)
{
    func_001DA528(menu, 3);
    if (self->mbAccount != 0) {
        *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP212DuplicateAccountName);
        func_001DA530(menu, 0, GetHashValue32(str_kT_HELP212DuplicateAccountNameMessage));
    } else {
        *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP212DuplicatePersona);
        func_001DA530(menu, 0, GetHashValue32(str_kT_HELP212DuplicatePersonaMessage));
    }
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC290);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void func_0025B9C8(void* self, void* a1);
extern char str_kT_POPUP211Submitting[];

extern "C" void func_001BC290(cFEStateCreateDuplicate* self)
{
    void* popup = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)popup + 0xBC) = 0;
    *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211Submitting);
    *(int*)((char*)popup + 0xC0) = 3;
    *(int*)((char*)popup + 0x150) = 2080;
    *(int*)((char*)popup + 0x164) = 0;
    func_001A9BC0((cFEStateNet*)self, popup);
    func_0025B9C8(LobbyMan_mLobbyMan, (char*)LobbyMan_mLobbyMan + 0x260);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC300);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025D1B8(void* self, const char* str);
extern char str_kT_POPUP220PersonaNameLength[];
extern char str_kT_BTNOK[];
extern char str_kT_POPUP220SubmittingPersona[];

extern "C" void func_001BC300(cFEStateCreateDuplicate* self, const char* name)
{
    if (strlen(name) < 3) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220PersonaNameLength);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2068;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 0;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP220SubmittingPersona);
        *(int*)((char*)popup + 0xC0) = 3;
        *(int*)((char*)popup + 0x150) = 2069;
        *(int*)((char*)popup + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, popup);
        func_0025D1B8(LobbyMan_mLobbyMan, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC400);
#ifdef SKIP_ASM
class cFEVText_001BC400 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int a1);
};


extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char str_SelectNamepct_d[];
extern char D_004A1408[];

extern "C" void func_001BC400(cFEStateCreateDuplicate* self)
{
    char buf[64];
    int i;
    for (i = 0; i < 4; i++) {
        sprintf(buf, str_SelectNamepct_d, i);
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (text != 0) {
            tLobbyAccount* names = self->mpAccount;
            if (i < names->mNumNameOptions) {
                cUIText_setAsciiString(text, ((char**)((char*)names + 0x14))[i]);
                ((cFEVText_001BC400*)text)->v08(0);
            } else {
                cUIText_setAsciiString(text, D_004A1408);
                ((cFEVText_001BC400*)text)->v08(1);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC4F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
extern "C" void func_001BC400(cFEStateCreateDuplicate* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_00260F10(void* self);
extern "C" void func_0039F190(void* self, int a1);
extern "C" int func_001A8E40_3i(void* self, int a1, int a2) __asm__("func_001A8E40");
extern const char str_cFEStateSelectPersona[];
extern char str_kT_POPUP211Congratulations2[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

// PORT: a2 is a message pointer passed as int
extern "C" int func_001BC4F0(cFEStateCreateDuplicate* self, int id, int data)
{
    switch (id) {
    case 203: {
        void* popup = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)popup + 0xBC) = 2;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP211Congratulations2);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
        *(int*)((char*)popup + 0x164) = 0;
        *(int*)((char*)popup + 0x150) = 2079;
        func_001A9BC0(((cFEStateNet*)self), popup);
        break;
    }
    case 212:
        func_00260F10(LobbyMan_mLobbyMan);
    case 214:
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        break;
    case 258:
        if (*(unsigned int*)((char*)data + 8) == 0x6475706C) {
            char* openPopup = (char*)self->mpPopup;
            if (openPopup != 0) {
                int popupId = *(int*)(openPopup + 0x18);
                if (popupId == 2080 || popupId == 2069) {
                    func_0039F190((char*)self->mpEngine + 0x18, 1);
                }
            }
            func_001BC400(self);
        } else {
            func_001A8E40_3i(((cFEStateNet*)self), id, data);
        }
        break;
    default:
        return func_001A8E40_3i(((cFEStateNet*)self), id, data);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateDuplicate_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BC728(cFEStateCreateDuplicate* self, void* a1, int a2);
extern "C" void func_001BC818(cFEStateCreateDuplicate* self, void* a1, int a2);
extern "C" void func_001BC990(cFEStateCreateDuplicate* self, void* a1, int a2);

/**
 * Hands a popup's event on the duplicate-name screen to the handler for that popup's id.
 *
 * 0x81F (save account) func_001BC818, 0x80E (verify) func_001BC728, 0x82C (account memory card) func_001BC990, any
 * other the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateCreateDuplicate_onPopupEvent(cFEStateCreateDuplicate* self, void* pPopup, int Event)
{
    switch (*(unsigned int*)((char*)pPopup + 0x18)) {
    case 2079:
        func_001BC818(self, pPopup, Event);
        break;
    case 2062:
        func_001BC728(self, pPopup, Event);
        break;
    case 2092:
        func_001BC990(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC728);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];

static inline void* GetSession_001BC728()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001BC728(cFEStateCreateDuplicate* self, void* popup, int event)
{
    if (event == 0xF || event == 20) {
        func_001A97B8_v(((cFEStateNet*)self), popup, event);
        func_00261408(GetSession_001BC728());
        func_0025B800(LobbyMan_mLobbyMan);
        void* accountMem = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 0, 0, 0);
        *(int*)((char*)accountMem + 0x18) = 2098;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, accountMem);
    } else {
        func_001A97B8_v(((cFEStateNet*)self), popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC818);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025C610(void* self);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];
extern char str_kT_POPUP201Verify[];
extern char str_kT_BTNCancel[];

extern "C" void func_001BC818(cFEStateCreateDuplicate* self, void* popup, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(self, popup, 15);
        void* newPopup = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, self, 1,
                                *(int*)self->mpAccount, *(int*)((char*)self->mpAccount + 4));
        *(int*)((char*)newPopup + 0x18) = 2092;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, newPopup);
        break;
    }
    case 16:
    case 20: {
        func_001A97B8(self, popup, kind);
        void* newPopup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)newPopup + 0xBC) = 1;
        char* sub = (char*)newPopup + 0xBC;
        *(int*)((char*)newPopup + 0xC4) = GetHashValue32(str_kT_POPUP201Verify);
        *(int*)((char*)newPopup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNCancel));
        *(int*)((char*)newPopup + 0x150) = 2062;
        func_001A9BC0((cFEStateNet*)self, newPopup);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, *(const char**)self->mpAccount);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, *(const char**)((char*)self->mpAccount + 4));
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    }
    default:
        func_001A97B8(self, popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC990);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
// PORT: the unit declares func_001A97B8 as returning int; this caller ignores the result.
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_0025C610(void* net);
extern char str_kT_POPUP204SaveAccount[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char str_kT_POPUP201Verify[];
extern char str_kT_BTNCancel[];

// Tracks a pending flag; on 0x16 shows either the confirm popup or the network-connect popup.
extern "C" void func_001BC990(cFEStateCreateDuplicate* self, void* popup, int event)
{
    switch (event) {
    case 0xF: case 0x10: case 20:
        self->mbAskSave = 0;
        func_001A97B8_v(self, popup, event);
        break;
    case 17:
        self->mbAskSave = 1;
        func_001A97B8_v(self, popup, event);
        break;
    case 22:
        func_001A97B8_v(self, popup, event);
        if (self->mbAskSave != 0) {
            void* newPopup = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)newPopup + 0xBC;
            *(int*)((char*)newPopup + 0xBC) = 2;
            *(int*)((char*)newPopup + 0xC4) = GetHashValue32(str_kT_POPUP204SaveAccount);
            *(int*)((char*)newPopup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            *(int*)((char*)newPopup + 0x168) = 0;
            *(int*)((char*)newPopup + 0x164) = 0;
            *(int*)((char*)newPopup + 0x150) = 2079;
            func_001A9BC0((cFEStateNet*)self, newPopup);
        } else {
            void* newPopup = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)newPopup + 0xBC;
            *(int*)((char*)newPopup + 0xBC) = 1;
            *(int*)((char*)newPopup + 0xC4) = GetHashValue32(str_kT_POPUP201Verify);
            *(int*)((char*)newPopup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNCancel));
            *(int*)((char*)newPopup + 0x150) = 2062;
            func_001A9BC0((cFEStateNet*)self, newPopup);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, *(char**)self->mpAccount);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, ((char**)self->mpAccount)[1]);
            func_0025C610(LobbyMan_mLobbyMan);
        }
        self->mbAskSave = 0;
        break;
    default:
        func_001A97B8_v(self, popup, event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_cFEStateLobbyMenu);
#ifdef SKIP_ASM
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern void* cFEStateLobbyMenu_vtbl[];
extern int cBXString_EmptyString;

struct sFEObj_001BCB58 {
    char pad0[0x8];
    void** vt;          // 0x8
    int fC;             // 0xC
    char pad10[0x6D0 - 0x10];
    int f6D0;           // 0x6D0
    int f6D4;           // 0x6D4
    int f6D8[10];       // 0x6D8
    int f700;           // 0x700
    int f704;           // 0x704
    int f708[4];        // 0x708
    int f718[4];        // 0x718
    int f728;           // 0x728
    int f72C;           // 0x72C
    int f730[4];        // 0x730
    int f740[4];        // 0x740
    int f750[4];        // 0x750
    char pad760[0x76C - 0x760];
    int f76C;           // 0x76C
    char pad770[0x778 - 0x770];
    int f778;           // 0x778
    char pad77C[0xB80 - 0x77C];
    int fB80;           // 0xB80
};

/**
 * Constructs the online lobby screen state ("231lobby_menu"): empty names and no widgets bound yet.
 *
 * Runs the cFEStateNet constructor (func_001A8500, no parent), stores cFEStateLobbyMenu_vtbl and mHashName 0x3C,
 * points mszRoomName and mszChannelName at the empty string cBXString_EmptyString, and zeroes miFirstPlayer, both menus, the
 * ten player texts, mpText3InRoom, the four channel lists, pairs, arrows, locks and rows, and the player up / down
 * arrows.
 *
 * @param pEngine  the UI engine (On Tour cUIState ctor pEngine)
 * @return @p self
 */
extern "C" sFEObj_001BCB58* cFEStateLobbyMenu_cFEStateLobbyMenu(sFEObj_001BCB58* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->vt = cFEStateLobbyMenu_vtbl;
    self->fB80 = self->f76C = cBXString_EmptyString;
    self->fC = 0x3C;
    self->f778 = 0;
    self->f6D0 = 0;
    self->f6D4 = 0;
    for (int i = 9; i >= 0; i--) {
        self->f6D8[i] = 0;
    }
    self->f704 = 0;
    for (int j = 0; j < 4; j++) {
        self->f708[j] = 0;
        self->f718[j] = 0;
        self->f730[j] = 0;
        self->f740[j] = 0;
        self->f750[j] = 0;
    }
    self->f728 = 0;
    self->f72C = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onCreateScreen);
#ifdef SKIP_ASM
class cFEVObj_001BCC30 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
};

int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char str_231lobby_menu[];

/**
 * Opens the "231lobby_menu" screen and, once it exists, calls the state's virtual slot 28.
 */
extern "C" void cFEStateLobbyMenu_onCreateScreen(cFEStateLobbyMenu* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEVObj_001BCC30*)self), GetHashValue32(str_231lobby_menu), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        ((cFEVObj_001BCC30*)self)->v28();
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BCCA8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

extern "C" void func_001BCCA8()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00262768(BuddyMan_mBuddyMan, 1, 2, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
// PORT: func_001BDF60's own definition takes an sFESlots1BDF60* (defined later); bound by asm label.
void func_001BDF60_1BDC18(void* self, int id) __asm__("func_001BDF60");
extern "C" void func_001BE0A8(cFEStateLobbyMenu* self);
extern "C" void func_001BE3A8(cFEStateLobbyMenu* self, int id);
extern "C" void func_001BE720(cFEStateLobbyMenu*);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);

struct sVE1BCD18 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Refreshes the lobby's channel lists and player list when the screen gains focus.
 *
 * On the first time it calls virtual slot 32 (func_001BCCA8), clears mbRoomChanged and refreshes the channel lists
 * (func_001BE0A8), the player arrows (func_001BE720) and channel 0's pair (func_001BE3A8). Otherwise, after a room
 * change, it refreshes the lists, clamps miSelectedRoom to the selected channel's row count, reselects it, the
 * channel in MenuRoomMenus and its pair, and the player list (func_001BDF60).
 *
 * @param reset  nonzero on the first transition (On Tour bFirstTime)
 * @return always 0
 */
extern "C" int cFEStateLobbyMenu_onGainTransition(cFEStateLobbyMenu* self, int reset)
{
    if (reset != 0) {
        sVE1BCD18* vt = (sVE1BCD18*)self->mVtbl;
        vt[32].fn((char*)self + vt[32].delta);
        self->mbRoomChanged = 0;
        func_001BE0A8(self);
        func_001BE720(self);
        func_001BE3A8(self, (int)self->mpChannelPair[0]);
    } else if (self->mbRoomChanged != 0) {
        func_001BE0A8(self);
        int cur = self->miSelectedChannel;
        if (cur < 4) {
            char* list = *(char**)((char*)self + (cur << 2) + 0x708);
            int n = *(unsigned char*)(list + 0x318);
            if (self->miSelectedRoom >= n) {
                self->miSelectedRoom = n - 1;
            }
            cUIListBox_setCurrentEntryByIndex(list, *(unsigned char*)&self->miSelectedRoom);
        }
        char* ids = (char*)self + 0x718;
        cUIMenu_setSelectedByIndex(self->mpMenuRoomMenus, self->miSelectedChannel);
        func_001BE3A8(self, *(int*)(ids + (self->miSelectedChannel << 2)));
        func_001BE720(self);
        func_001BDF60_1BDC18(self, *(int*)(ids + (self->miSelectedChannel << 2)));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Runs one frame of the lobby screen: only the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (the pending-event handler's value)
 */
void* cFEStateLobbyMenu_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIMenu_setInputMap(void* self, int a1);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern char str_MenuRoomMenus[];
extern char str_MenuPlayers[];
extern char str_TextVoiceChannel[];
extern char str_kT_231VoiceChannel[];
extern char str_3inroom[];
extern char str_TextCreateRoom[];
extern char str_PairBeginner[];
extern char str_PairVoice[];
extern char str_PairAdvanced[];
extern char str_PairExpert[];
extern char str_ArrowBeginner[];
extern char str_ArrowVoice[];
extern char str_ArrowAdvanced[];
extern char str_ArrowElite[];
extern char str_player_up[];
extern char str_player_down[];
extern char str_ListBoxBeginnerChannel[];
extern char str_ListBoxVoiceChannel[];
extern char str_ListBoxAdvancedChannel[];
extern char str_ListBoxExpertChannel[];
extern char str_LockBeginner[];
extern char str_LockVoice[];
extern char str_LockAdvanced[];
extern char str_LockElite[];
extern char str_TextPlayerpct_d[];

class cFEVObj_001BCE38 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08();
    virtual void v09(int on);
};

/**
 * Binds the lobby screen's widgets as the screen creates them.
 *
 * By hash name: "MenuRoomMenus", "MenuPlayers" (input map 1, scrollable, attribute bit 0x80 cleared and bit 0 set),
 * "TextVoiceChannel" (shows "kT_231VoiceChannel"), "3inroom", "TextCreateRoom" (id 4), the four channel pairs
 * ("PairBeginner" ..; ids 0-3), arrows ("ArrowBeginner" ..; only the first shown), "player up" / "player down",
 * channel list boxes ("ListBoxBeginnerChannel" ..; attribute bit 0 cleared) and locks ("LockBeginner" ..; hidden).
 * Any other widget named "TextPlayer%d" (0-9) goes to mpTextPlayer[i] with id i. The id is the widget's mMiscData
 * (+0x18).
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateLobbyMenu_onWidgetCreate(cFEStateLobbyMenu* self, cFEVObj_001BCE38* obj)
{
    char buf[16];
    char* widget = (char*)obj;
    int id = *(int*)(widget + 0x38);
    if (id == GetHashValue32(str_MenuRoomMenus)) {
        self->mpMenuRoomMenus = (cUIMenu*)obj;
    } else if (id == GetHashValue32(str_MenuPlayers)) {
        self->mpMenuPlayers = (cUIMenu*)obj;
        cUIMenu_setInputMap(obj, 1);
        ((cFEVObj_001BCE38*)self->mpMenuPlayers)->v07(1);
        cUIMenu_setScrollableMenu(self->mpMenuPlayers, 1);
        *(int*)((char*)self->mpMenuPlayers + 0x14) &= ~0x80;
        *(int*)((char*)self->mpMenuPlayers + 0x14) |= 1;
    } else if (id == GetHashValue32(str_TextVoiceChannel)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(str_kT_231VoiceChannel));
    } else if (id == GetHashValue32(str_3inroom)) {
        self->mpText3InRoom = (cUIText*)obj;
    } else if (id == GetHashValue32(str_TextCreateRoom)) {
        self->mpTextCreateRoom = (cUIText*)obj;
        *(int*)(widget + 0x18) = 4;
    } else if (id == GetHashValue32(str_PairBeginner)) {
        self->mpChannelPair[0] = (cUIThing*)obj;
        *(int*)(widget + 0x18) = 0;
    } else if (id == GetHashValue32(str_PairVoice)) {
        self->mpChannelPair[1] = (cUIThing*)obj;
        *(int*)(widget + 0x18) = 1;
    } else if (id == GetHashValue32(str_PairAdvanced)) {
        self->mpChannelPair[2] = (cUIThing*)obj;
        *(int*)(widget + 0x18) = 2;
    } else if (id == GetHashValue32(str_PairExpert)) {
        self->mpChannelPair[3] = (cUIThing*)obj;
        *(int*)(widget + 0x18) = 3;
    } else if (id == GetHashValue32(str_ArrowBeginner)) {
        self->mpChannelArrow[0] = (cUIThing*)obj;
        obj->v09(1);
    } else if (id == GetHashValue32(str_ArrowVoice)) {
        self->mpChannelArrow[1] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(str_ArrowAdvanced)) {
        self->mpChannelArrow[2] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(str_ArrowElite)) {
        self->mpChannelArrow[3] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(str_player_up)) {
        self->mpPlayerUp = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_player_down)) {
        self->mpPlayerDown = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_ListBoxBeginnerChannel)) {
        self->mpChannelList[0] = (cUIListBox*)obj;
        *(int*)(widget + 0x14) &= ~1;
    } else if (id == GetHashValue32(str_ListBoxVoiceChannel)) {
        self->mpChannelList[1] = (cUIListBox*)obj;
        *(int*)(widget + 0x14) &= ~1;
    } else if (id == GetHashValue32(str_ListBoxAdvancedChannel)) {
        self->mpChannelList[2] = (cUIListBox*)obj;
        *(int*)(widget + 0x14) &= ~1;
    } else if (id == GetHashValue32(str_ListBoxExpertChannel)) {
        self->mpChannelList[3] = (cUIListBox*)obj;
        *(int*)(widget + 0x14) &= ~1;
    } else if (id == GetHashValue32(str_LockBeginner)) {
        self->mpChannelLock[0] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(str_LockVoice)) {
        self->mpChannelLock[1] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(str_LockAdvanced)) {
        self->mpChannelLock[2] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(str_LockElite)) {
        self->mpChannelLock[3] = (cUIThing*)obj;
        obj->v09(0);
    } else {
        void** playerTextSlot = (void**)(((char*)self) + 0x6D8);
        int i;
        for (i = 0; ; playerTextSlot++, i++) {
            if (i >= 10) {
                return;
            }
            sprintf(buf, str_TextPlayerpct_d, i);
            if (id == GetHashValue32(buf)) {
                break;
            }
        }
        *playerTextSlot = obj;
        *(int*)(widget + 0x18) = i;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup_D1B8(void* mem, int a1, void* owner) __asm__("cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup");
extern "C" int cUIListBox_getCurrentEntryData_D1B8(void* item) __asm__("cUIListBox_getCurrentEntryData");
extern "C" void func_001BD460_D1B8(void* self, int id, int a2) __asm__("func_001BD460");
extern "C" void func_001BD428_D1B8(void* self) __asm__("func_001BD428");
extern "C" int func_0025F180_D1B8(void* self, int id) __asm__("func_0025F180");
extern "C" void func_0025D770_D1B8(void* self, int id) __asm__("func_0025D770");
extern char str_cFEStateNetCreateRoomPopup[];

struct sVE_D1B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj_D1B8 {
    int pad[2];
    sVE_D1B8* vt;
};

struct cState_D1B8 {
    char pad0[0x10];
    int engine;              // 0x10
    char pad14[0x1C - 0x14];
    int flags;               // 0x1C
    char pad20[0x40 - 0x20];
    char* screen;            // 0x40
    char pad44[0x6D4 - 0x44];
    char* info;              // 0x6D4
    char pad6D8[0x700 - 0x6D8];
    void* backBtn;           // 0x700
    char pad704[0x708 - 0x704];
    char* items[4];          // 0x708
    void* btns[4];           // 0x718
    char pad728[0x740 - 0x728];
    sObj_D1B8* texts[4];     // 0x740
    int vals[4];             // 0x750
    int changed;             // 0x760
    int sel;                 // 0x764
    int selVal;              // 0x768
    char pad76C[0x774 - 0x76C];
    int curId;               // 0x774
    int busy;                // 0x778
};

/**
 * Handles the lobby's widget events: room selection, create room, entering a room, channel changes and back.
 *
 * The local view (cState_D1B8) misnames fields: its backBtn is mpTextCreateRoom, items mpChannelList, btns
 * mpChannelPair, texts mpChannelLock, vals miChannelRow, busy miFirstPlayer, info mpMenuRoomMenus; the
 * "profile-select screen" comment above it is stale. Event 9 on channel list i stores its row, resets
 * miFirstPlayer, makes the row's id miRoomId, shows lock i from func_0025F180(LobbyMan, id) and asks LobbyMan for
 * the room (func_0025D770). Event 5 on "TextCreateRoom" pushes a cFEStateNetCreateRoomPopup (id 0x823); on channel
 * pair i it enters that list's selected room (func_001BD460) when its id is not -1. Event 6 returns to
 * cFEStateOnlineMainMenu; event 7 sets mbRoomChanged with the RoomMenus channel and its row; event 8 calls
 * func_001A9B58 unless mAttrState bits 1-2 are set; event 13 func_001BD428; the rest go to the cFEStateNet handler.
 *
 * @param pWidget  the widget (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
// Message handler for the profile-select screen.
extern "C" void cFEStateLobbyMenu_onWidgetEvent(cState_D1B8* self, char* pWidget, int msg)
{
    switch (msg) {
    case 9:
        for (int i = 0; i < 4; i++) {
            if (pWidget == self->items[i]) {
                self->vals[i] = (unsigned char)pWidget[0x319];
                self->busy = 0;
                int id = cUIListBox_getCurrentEntryData_D1B8(pWidget);
                self->curId = id;
                int lockState = func_0025F180_D1B8(LobbyMan_mLobbyMan, id);
                sObj_D1B8* lockIcon = self->texts[i];
                lockIcon->vt[9].fn((char*)lockIcon + lockIcon->vt[9].delta, lockState);
                func_0025D770_D1B8(LobbyMan_mLobbyMan, self->curId);
                return;
            }
        }
        break;
    case 5: {
        char* item = 0;
        if (pWidget == self->backBtn) {
            void* popup = cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup_D1B8(cMemMan_alloc(1768, str_cFEStateNetCreateRoomPopup, 0, 0), self->engine, self);
            *(int*)((char*)popup + 0x18) = 2083;
            cUIStateStack_pushPopup(*(char**)(*(char**)(self->screen + 0xD0) + 0x10) + 0x18, popup);
        }
        for (int i = 0; i < 4; i++) {
            if (pWidget == self->btns[i]) {
                item = self->items[i];
                break;
            }
        }
        if (item == 0) {
            break;
        }
        if (cUIListBox_getCurrentEntryData_D1B8(item) == -1) {
            break;
        }
        func_001BD460_D1B8(self, cUIListBox_getCurrentEntryData_D1B8(item), 0);
        break;
    }
    case 6:
        func_001A8A40(self, cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), self->engine));
        break;
    case 7: {
        self->changed = 1;
        int n = *(int*)(*(char**)(self->info + 0xA0) + 0x18);
        self->sel = n;
        if (n < 4) {
            self->selVal = (unsigned char)self->items[n][0x319];
        }
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)pWidget, msg);
        break;
    }
    case 8:
        if ((self->flags & 6) == 0) {
            func_001A9B58((cFEStateNet*)self);
        }
        break;
    case 13:
        func_001BD428_D1B8(self);
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD428);
#ifdef SKIP_ASM
extern "C" void func_001BD540(cFEStateLobbyMenu* self, int a1, void* a2);

extern "C" void func_001BD428(cFEStateLobbyMenu* self)
{
    char* keyboard = (char*)self->mpKeyboard;
    if (*(int*)(keyboard + 0x18) == 8) {
        func_001BD540(self, (int)self->mszRoomName, keyboard + 0x74);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD460);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void func_0025D800(void* self, int a1, const char* a2);
extern "C" const char* func_0025F150(void* self, int a1);
extern char str_kT_POPUP231EnteringRoom[];

struct sFEObj_001BD460 {
    char pad0[0x770];
    int f770;           // 0x770
    char pad774[0xB80 - 0x774];
    char fB80[4];       // 0xB80 (string object)
};

extern "C" void func_001BD460(cFEStateLobbyMenu* self, int roomId, const char* str)
{
    void* popup = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)popup + 0xBC) = 0;
    *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231EnteringRoom);
    *(int*)((char*)popup + 0x164) = 0;
    *(int*)((char*)popup + 0xC0) = 3;
    *(int*)((char*)popup + 0x150) = 2089;
    func_001A9BC0((cFEStateNet*)self, popup);
    if (str == 0 || strlen(str) == 0) {
        self->mbRoomHasPassword = 0;
        func_0025D800(LobbyMan_mLobbyMan, roomId, 0);
    } else {
        self->mbRoomHasPassword = 1;
        func_0025D800(LobbyMan_mLobbyMan, roomId, str);
    }
    char* roomName = ((char*)&self->mszRoomName);
    cBXString_cBXString4(roomName, func_0025F150(LobbyMan_mLobbyMan, roomId));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD540);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void func_0025D860(void* self, const char* name, const char* str);
extern char str_kT_POPUP231EnteringRoom[];


extern "C" void func_001BD540(cFEStateLobbyMenu* self, int roomName, void* password)
{
    void* popup = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)popup + 0xBC) = 0;
    *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231EnteringRoom);
    *(int*)((char*)popup + 0x164) = 0;
    *(int*)((char*)popup + 0xC0) = 3;
    *(int*)((char*)popup + 0x150) = 2089;
    func_001A9BC0((cFEStateNet*)self, popup);
    if (password == 0 || strlen((const char*)password) == 0) {
        (self)->mbRoomHasPassword = 0;
        func_0025D860(LobbyMan_mLobbyMan, (const char*)roomName, 0);
    } else {
        (self)->mbRoomHasPassword = 1;
        func_0025D860(LobbyMan_mLobbyMan, (const char*)roomName, (const char*)password);
    }
    // PORT: the unit declares a1 as int; it is the name string
    cBXString_cBXString4(((char*)&(self)->mszRoomName), (const char*)roomName);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD610);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025DAC0(void* self, int a1, const char* a2, const char* a3);
extern char str_kT_POPUP231UniqueRoom[];
extern char str_kT_BTNOK[];
extern char str_kT_POPUP231SubmittingCreateRoom[];

// PORT: the unit declares this (void*, int, int, int); a1/a3 are string pointers passed as int.
extern "C" void func_001BD610(cFEStateLobbyMenu* self, int namePtr, int a2, int extraPtr)
{
    const char* name = (const char*)namePtr;
    const char* extra = (const char*)extraPtr;
    if (strlen(name) == 0) {
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231UniqueRoom);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2084;
        func_001A9BC0((cFEStateNet*)self, popup);
    } else {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 0;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231SubmittingCreateRoom);
        *(int*)((char*)popup + 0xC0) = 3;
        *(int*)((char*)popup + 0x150) = 2088;
        *(int*)((char*)popup + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, popup);
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        func_0025DAC0(lobbyMan, a2, name, strlen(extra) != 0 ? extra : 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD738);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_001A8A40(void* self, void* obj);
// PORT: func_001BE510 is defined as (self) in this unit, but this caller passes a second arg.
void func_001BE510_BD738(void* self, int a1) __asm__("func_001BE510");
extern "C" const char* cUIListBox_getCurrentAsciiText(void* a);
extern "C" void func_001BE0A8(cFEStateLobbyMenu* self);
// PORT: func_001BE038 is defined later with its own list type; bound by asm label.
void func_001BE038_BD738(void* self) __asm__("func_001BE038");
struct sFEObj_001B1F88;
extern "C" sFEObj_001B1F88* cFEStateChatRoom_cFEStateChatRoom(sFEObj_001B1F88* mem, int a1);
extern "C" void func_001BD848(cFEStateLobbyMenu* self, int a1, int a2);
extern const char str_cFEStateChatRoom[];

struct sFE1BD738 {
    char pad0[0x708];
    void* boxes[4];     // 0x708
};

// Lobby/server message handler (cFEStateNet virtual slot 31) of the lobby menu (cFEStateLobbyMenu): 223 picks the
// channel (miSelectedChannel) from the room list, 220/228 open the chat room (cFEStateChatRoom), 258 handles room
// results (func_001BD848); unknown ids go to cFEStateNet's handler func_001A8E40.
extern "C" void* func_001BD738(cFEStateLobbyMenu* self, int msgId, int data)
{
    switch (msgId) {
    case 0xE0:
        func_001BE510_BD738(((char*)self), self->miRoomId);
        break;
    case 223: {
        int n = *(int*)(*(char**)((char*)self->mpMenuRoomMenus + 0xA0) + 0x18);
        self->miSelectedChannel = n;
        if (n < 4) {
            cBXString_cBXString4(((char*)self) + 0x76C, cUIListBox_getCurrentAsciiText(((sFE1BD738*)self)->boxes[n]));
        }
        func_001BE0A8(self);
        func_001BE038_BD738(((char*)self));
        break;
    }
    case 220: case 228:
        func_001A8A40(((char*)self), cFEStateChatRoom_cFEStateChatRoom((sFEObj_001B1F88*)cMemMan_alloc(1852, str_cFEStateChatRoom, 0, 0), (int)self->mpEngine));
        break;
    case 258:
        func_001BD848(self, msgId, data);
        break;
    default:
        return func_001A8E40_3(((char*)self), msgId, data);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD848);
#ifdef SKIP_ASM
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void func_001BDA88(cFEStateLobbyMenu* self, int a2);
extern "C" void func_001BD8C8(cFEStateLobbyMenu* self, int a2);

extern "C" void func_001BD848(cFEStateLobbyMenu* self, int msgId, int data)
{
    if (self->mAttrState & 6) {
        switch (*(int*)((char*)self->mpPopup + 0x18)) {
        case 2088:
            func_001BDA88(self, data);
            break;
        case 2089:
            func_001BD8C8(self, data);
            break;
        default:
            func_001A8E40_3(((cFEStateNet*)self), msgId, data);
            break;
        }
    } else {
        func_001A8E40_3(((cFEStateNet*)self), msgId, data);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BD8C8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void sprintf1(void* str, int a1, char* buf);
// PORT: unit declares func_001A8E40 with one arg; the real handler takes (self, id, msg)
extern "C" void* func_001A8E40_msg(void* self, int id, void* msg) __asm__("func_001A8E40");
extern char str_kT_BTNOK[];
extern char str_kT_POPUP231InvalidRoom[];
extern char str_kT_POPUP231RoomPassword[];
extern char str_kT_POPUP231EnterPassword[];
extern char str_kT_BTNCancel[];

struct sVEntry_001BD8C8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// PORT: the unit declares the message pointer as int
extern "C" void func_001BD8C8(cFEStateLobbyMenu* self, int msgPtr)
{
    void* msg = (void*)msgPtr;
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x66756C6C:
    case 0x75726F6D: {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 1;
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231InvalidRoom);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x150) = 2091;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    case 0x70617373:
        if (self->mbRoomHasPassword == 0) {
            char* buf = (char*)self + 0x780;
            char* name = self->mszRoomName;
            char* obj = (char*)SSXApp->FELocale;
            sVEntry_001BD8C8* vt = *(sVEntry_001BD8C8**)(obj + 4);
            sprintf1(buf, vt[4].fn(obj + vt[4].delta, GetHashValue32(str_kT_POPUP231RoomPassword)), name);
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(char**)((char*)popup + 0xC4) = buf;
            *(int*)((char*)popup + 0xBC) = 2;
            *(int*)((char*)popup + 0xC0) = 1;
            char* sub = (char*)popup + 0xBC;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_POPUP231EnterPassword));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_BTNCancel));
            *(int*)((char*)popup + 0x150) = 2090;
            func_001A9BC0((cFEStateNet*)self, popup);
        } else {
            func_001A8E40_msg(self, *(int*)msg, msg);
        }
        break;
    default:
        func_001A8E40_msg(self, *(int*)msg, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDA88);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
// PORT: unit declares func_001A8E40 with one arg; the real handler takes (self, id, msg)
extern "C" void* func_001A8E40_msg(void* self, int id, void* msg) __asm__("func_001A8E40");
extern char str_kT_POPUP231MaxRooms[];
extern char str_kT_POPUP231AlreadyExists[];
extern char str_kT_POPUPCMNProfanityRoom[];
extern char str_kT_BTNOK[];

// PORT: the unit declares the message pointer as int
extern "C" void func_001BDA88(cFEStateLobbyMenu* self, int msgPtr)
{
    void* msg = (void*)msgPtr;
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x66756C6C:
        {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 1;
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231MaxRooms);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)popup + 0x150) = 2085;
            func_001A9BC0((cFEStateNet*)self, popup);
        }
        break;
    case 0x6475706C:
        {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 1;
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231AlreadyExists);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)popup + 0x150) = 2086;
            func_001A9BC0((cFEStateNet*)self, popup);
        }
        break;
    case 0x66696C74:
        {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 1;
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUPCMNProfanityRoom);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)popup + 0x150) = 2087;
            func_001A9BC0((cFEStateNet*)self, popup);
        }
        break;
    default:
        func_001A8E40_msg(self, *(int*)msg, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onWidgetMenu);
#ifdef SKIP_ASM
// PORT: func_001BDF60's own definition takes an sFESlots1BDF60* (defined later); bound by asm label.
void func_001BDF60_1BDC18(void* self, int id) __asm__("func_001BDF60");
extern "C" void func_001BE3A8(cFEStateLobbyMenu* self, int id);
// PORT: func_001BE510 is defined as (self) in this unit, but this caller passes a second arg.
void func_001BE510_2(void* self, int a1) __asm__("func_001BE510");

/**
 * Handles the lobby's menu events: a channel change in MenuRoomMenus and scrolling in MenuPlayers.
 *
 * Event 5 on MenuRoomMenus refreshes the player list and pair for its selected item (+0xA0) (func_001BDF60,
 * func_001BE3A8); event 4 on MenuPlayers stores @p index as miFirstPlayer and redraws the players of miRoomId
 * (func_001BE510).
 *
 * @param obj  the menu (On Tour pMenu)
 * @param msg  the menu event (On Tour Event)
 * @param index  the menu's new index (On Tour index): the first player row shown
 * @return always 0x101
 */
extern "C" int cFEStateLobbyMenu_onWidgetMenu(cFEStateLobbyMenu* self, void* obj, int msg, int index)
{
    if (obj == (void*)self->mpMenuRoomMenus) {
        if (msg == 5) {
            func_001BDF60_1BDC18(self, *(int*)((char*)obj + 0xA0));
            func_001BE3A8(self, *(int*)((char*)obj + 0xA0));
            return 257;
        }
        return 257;
    }
    if (obj == (void*)self->mpMenuPlayers) {
        if (msg == 4) {
            self->miFirstPlayer = index;
            func_001BE510_2(self, self->miRoomId);
            return 257;
        }
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onWidgetPair);
#ifdef SKIP_ASM
#include "fakematch.h"

struct sListBox_BDCA0 {
    char pad0[0x318];
    unsigned char f318;
};

struct sState_BDCA0 {
    char pad0[0x708];
    sListBox_BDCA0* boxes[4];   // 0x708
    int ids[4];                 // 0x718
};

/**
 * Answers a pair widget's event on the lobby screen: always 0x101.
 *
 * For event 0 it looks for @p id among mpChannelPair[4] and reads that channel list's row count (+0x318) without
 * using it (a dead read kept by the FAKEMATCH spelling).
 *
 * @param self_  the lobby screen state (this)
 * @param id  the pair widget, as an int (On Tour pPair)
 * @param msg  the pair event (On Tour Event)
 * @return always 0x101
 */
// FAKEMATCH: do-while(0) wrapper around the if/else, per-function volatile view of the dead boxes[i]->f318 read, and FM_BARRIER() after it (the wrapper keeps loop.c from moving the found arm out of the loop, so jump2 cross-jumps the msg==1 return into it); plain form 42.05%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" int cFEStateLobbyMenu_onWidgetPair(void* self_, int id, int msg)
{
    sState_BDCA0* self = (sState_BDCA0*)self_;
    do {
        if (msg != 1) {
            if (msg == 0) {
                for (int i = 0; i < 4; i++) {
                    if (id == self->ids[i]) {
                        *(volatile unsigned char*)&self->boxes[i]->f318;
                        FM_BARRIER();
                        return 257;
                    }
                }
            }
        } else {
            return 257;
        }
    } while (0);
    return 257;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001BDCF8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BD610(cFEStateLobbyMenu* self, int a1, int a2, int a3);

extern "C" void func_001BDCF8(cFEStateLobbyMenu* self, void* popup, int event)
{
    if (event == 0xF) {
        func_001A97B8(self, popup, event);
        func_001BD610(self, *(int*)((char*)popup + 0x6DC), *(int*)((char*)popup + 0x6E0), *(int*)((char*)popup + 0x6E4));
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDD60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern char str_kT_POPUP231EnterPassword[];
extern char D_004A1408[];

extern "C" void func_001BDD60(cFEStateLobbyMenu* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(((cFEStateNet*)self), a1, 0xF);
        // PORT: string pointer passed through an int parameter (func_001A9150's a3)
        func_001A9150(((cFEStateNet*)self), 0, GetHashValue32(str_kT_POPUP231EnterPassword), (int)D_004A1408, 0, 8, 0x3F);
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
    } else {
        func_001A97B8(((cFEStateNet*)self), a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BDCF8(cFEStateLobbyMenu* self, void* a1, int a2);
extern "C" void func_001BDD60(cFEStateLobbyMenu* self, void* a1, int a2);

/**
 * Hands a popup's event on the lobby screen to the handler for that popup's message id.
 *
 * The id is the popup's miMiscData (+0x18): 0x823 (the create-room popup) func_001BDCF8, 0x82A func_001BDD60; any
 * other the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateLobbyMenu_onPopupEvent(cFEStateLobbyMenu* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2083:
        func_001BDCF8(self, pPopup, Event);
        break;
    case 2090:
        func_001BDD60(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDE48);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP231MainLobby[];
extern char str_kT_HELP231LobbyMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_HELP231ScrollUserList[];
extern char str_kT_HELP231EnterRoom[];
extern char str_kT_BTNEAMessenger[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001BDE48(cFEStateLobbyMenu* self, void* menu)
{
    func_001DA528(menu, 6);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP231MainLobby);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP231LobbyMessage));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    func_001DA510(menu, 2, GetHashValue32(str_kT_HELP231ScrollUserList), 8);
    func_001DA510(menu, 3, GetHashValue32(str_kT_HELP231EnterRoom), 0);
    func_001DA510(menu, 4, GetHashValue32(str_kT_BTNEAMessenger), 2);
    func_001DA510(menu, 5, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDF60);
#ifdef SKIP_ASM
class cFEVObj_001BDF60 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

struct sFESlots1BDF60 {
    char pad[0x718];
    int ids[4];                     // 0x718
    char pad728[0x730 - 0x728];
    cFEVObj_001BDF60* objs[4];      // 0x730
};

extern "C" void func_001BDFD8(cFEStateLobbyMenu* self);

extern "C" void func_001BDF60(sFESlots1BDF60* self, int id)
{
    func_001BDFD8((cFEStateLobbyMenu*)self);
    for (int i = 0; i < 4; i++) {
        if (id == self->ids[i]) {
            self->objs[i]->v09(1);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BDFD8);
#ifdef SKIP_ASM
class cFEVObj_001BDFD8 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

extern "C" void func_001BDFD8(cFEStateLobbyMenu* self)
{
    int i;
    for (i = 0; i < 4; i++) {
        ((cFEVObj_001BDFD8**)((char*)self + 0x730))[i]->v09(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE038);
#ifdef SKIP_ASM
struct sListBox1BE038 {
    char pad[0x318];
    unsigned char count;    // 0x318
};

struct sFELists1BE038 {
    char pad[0x708];
    sListBox1BE038* boxes[4];       // 0x708
    char pad718[0x750 - 0x718];
    unsigned int sel[4];            // 0x750
};

extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);

extern "C" void func_001BE038(sFELists1BE038* self)
{
    for (unsigned int i = 0; i < 4; i++) {
        if (self->sel[i] >= self->boxes[i]->count) {
            self->sel[i] = self->boxes[i]->count - 1;
        }
        if (self->boxes[i]->count != 0) {
            cUIListBox_setCurrentEntryByIndex(self->boxes[i], (unsigned char)self->sel[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE0A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cBXString;
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cBXString_cBXString5(void* self, void* src, int len);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
int cBXString_FindLastOf(cBXString* self, char ch, int len);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
// PORT: the data argument is an unsigned/handle value here (-1 as 0xFFFFFFFF)
extern "C" int cUIListBox_addEntryByStringID_u(void* box, int id, unsigned int a2) __asm__("cUIListBox_addEntryByStringID");
struct sFELists1BE038;
extern "C" int func_0025F120(void* self);
extern "C" const char* func_0025F150(void* self, int a1);
extern "C" int func_0025F180(void* self, int idx);
extern "C" void cBXString_right(void* ret, void* src, int n);
extern "C" void func_0039A670(void* self);
extern "C" int func_004165A8(const char* a, const char* b);
extern char* D_004A3E90_p __asm__("cBXString_EmptyString");
extern const char* D_004A1A60;
extern const char* D_004A1A64;
extern const char* D_004A1A68;
extern const char* D_004A1A6C;
extern char str_kT_231NoAvailableRooms[];

struct sBXStr_001BE0A8 {
    char* p;
    sBXStr_001BE0A8() { p = D_004A3E90_p; }
    sBXStr_001BE0A8(const sBXStr_001BE0A8& o);
};

struct sBXTmp_001BE0A8 {
    char* p;
    sBXTmp_001BE0A8(const sBXTmp_001BE0A8& o);
    sBXTmp_001BE0A8(int) {}
};

class cFEVObj_001BE0A8 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
};

extern "C" void func_001BE0A8(cFEStateLobbyMenu* self)
{
    LobbyMan* mgr = LobbyMan_mLobbyMan;
    cFEVObj_001BE0A8** lists = (cFEVObj_001BE0A8**)(((char*)self) + 0x708);
    int n = func_0025F120(mgr);
    sBXStr_001BE0A8 name;
    sBXStr_001BE0A8 base;
    sBXStr_001BE0A8 ext;
    int counts[4];
    for (int k = 0; k < 4; k++) {
        func_0039A670(lists[k]);
    }
    for (int i = 0; i < n; i++) {
        cBXString_cBXString4(&name, func_0025F150(mgr, i));
        int dot = cBXString_FindLastOf((cBXString*)&name, '.', 0);
        {
            sBXTmp_001BE0A8 t(0);
            cBXString_cBXString5(&t, &name, dot);
            cBXString_operatorE(&base, &t);
            cBXString__cBXString(&t, 2);
        }
        {
            sBXTmp_001BE0A8 t(0);
            int len = *(int*)(name.p - 8) - 1;
            cBXString_right(&t, &name, len - dot);
            cBXString_operatorE(&ext, &t);
            cBXString__cBXString(&t, 2);
        }
        cFEVObj_001BE0A8* list;
        int type;
        if (func_004165A8(base.p, D_004A1A6C) == 0) {
            type = 0;
            list = (cFEVObj_001BE0A8*)self->mpChannelList[0];
            counts[0]++;
        } else if (func_004165A8(base.p, D_004A1A68) == 0) {
            type = 1;
            list = (cFEVObj_001BE0A8*)self->mpChannelList[1];
            counts[1]++;
        } else if (func_004165A8(base.p, D_004A1A64) == 0) {
            type = 2;
            list = (cFEVObj_001BE0A8*)self->mpChannelList[2];
            counts[2]++;
        } else if (func_004165A8(base.p, D_004A1A60) == 0) {
            type = 3;
            list = (cFEVObj_001BE0A8*)self->mpChannelList[3];
            counts[3]++;
        } else {
            continue;
        }
        if (counts[type] == 0) {
            int on = func_0025F180(LobbyMan_mLobbyMan, i);
            (*(cFEVObj_001BE0A8**)(((char*)self) + (type << 2) + 0x740))->v09(on);
        }
        cUIListBox_addEntryByAsciiString(list, ext.p, i);
        list->v08(0);
    }
    for (int k = 0; k < 4; k++) {
        if (counts[k] == 0) {
            cUIListBox_addEntryByStringID_u(lists[k], GetHashValue32(str_kT_231NoAvailableRooms), 0xFFFFFFFF);
            lists[k]->v08(1);
        }
    }
    func_001BE038((sFELists1BE038*)self);
    cBXString__cBXString(&ext, 2);
    cBXString__cBXString(&base, 2);
    cBXString__cBXString(&name, 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE3A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int cUIListBox_getCurrentEntryData(void* a);
extern "C" int func_0025F180(void* self, int idx);
extern "C" void func_0025D770(void* self, int idx);
// PORT: func_001BE510 is defined as (self) in this unit, but this caller passes a second arg.
void func_001BE510_2(void* self, int a1) __asm__("func_001BE510");
extern "C" void sprintf1(void* str, int a1, char* buf);
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern char str_kT_231PlayersInRoom[];

class cFEVObj_001BE3A8 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

struct sVE_001BE3A8 {
    short delta;
    short index;
    int (*fn)(void* self, int hash);
};

struct sFE_001BE3A8 {
    char pad0[0x704];
    cUIText* text;
    void* boxes[4];
    int ids[4];
    char pad728[0x740 - 0x728];
    cFEVObj_001BE3A8* objs[4];
    char pad750[0x774 - 0x750];
    int sel;
    int f778;
};

extern "C" void func_001BE3A8(cFEStateLobbyMenu* feStateLobbyMenu, int id)
{
    sFE_001BE3A8* self = (sFE_001BE3A8*)feStateLobbyMenu;
    int sel = -1;
    int i;
    for (i = 0; i < 4; i++) {
        if (id == self->ids[i]) {
            void* box = self->boxes[i];
            if (*((unsigned char*)box + 0x318) != 0) {
                sel = cUIListBox_getCurrentEntryData(box);
                int on = func_0025F180(LobbyMan_mLobbyMan, sel);
                self->objs[i]->v09(on);
            }
        } else {
            self->objs[i]->v09(0);
        }
    }
    if (sel != -1) {
        self->sel = sel;
        self->f778 = 0;
        func_001BE510_2(self, sel);
        func_0025D770(LobbyMan_mLobbyMan, sel);
    } else {
        char buf[80];
        char* o = (char*)SSXApp->FELocale;
        sVE_001BE3A8* vt = *(sVE_001BE3A8**)(o + 4);
        char* obj = o + vt[4].delta;
        sprintf1(buf, vt[4].fn(obj, GetHashValue32(str_kT_231PlayersInRoom)), 0);
        cUIText_setUnicodeString(self->text, buf);
        func_001BE510_2(self, -1);
        func_0025D770(LobbyMan_mLobbyMan, -1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE510);
#ifdef SKIP_ASM
extern "C" void func_001BE540(void*);
extern "C" void func_001BE720(cFEStateLobbyMenu*);

extern "C" void func_001BE510(cFEStateLobbyMenu* self)
{
    func_001BE540(self);
    func_001BE720(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE540);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int func_0025F1B0(void* self);
extern "C" const char* func_0025F1E0(void* self, int idx, int a2);
extern "C" void sprintf1(void* str, int a1, char* buf);
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern char str_kT_231PlayersInRoom[];

struct sVEntry_001BE540 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1BE540(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001BE540* vt = *(sVEntry_001BE540**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

class cFEVObj_001BE540 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
};

struct sSelf1BE540 {
    char pad[0x6D0];
    char* info;
    char pad6D4[0x6D8 - 0x6D4];
    cFEVObj_001BE540* items[10];
    char pad700[0x704 - 0x700];
    cUIText* text;
    char pad708[0x778 - 0x708];
    int first;
    int count;
};

// PORT: the unit declares func_001BE540(void*); the body takes (self, page) (func_001BE510 passes its a1 through)
extern "C" void func_001BE540_2(void* self, int page) __asm__("func_001BE540");

extern "C" void func_001BE540_2(void* self, int page)
{
    int i;
    if (page != -1) {
        LobbyMan* mgr = LobbyMan_mLobbyMan;
        ((sSelf1BE540*)self)->count = func_0025F1B0(mgr);
        char buf[80];
        sprintf1(buf, Loc1BE540(str_kT_231PlayersInRoom), (char*)((sSelf1BE540*)self)->count);
        cUIText_setUnicodeString(((sSelf1BE540*)self)->text, buf);
        unsigned char n = ((sSelf1BE540*)self)->count;
        *(char*)(((sSelf1BE540*)self)->info + 0x96) = n;
        for (i = 0; i < 10; i++) {
            int idx = ((sSelf1BE540*)self)->first + i;
            if (idx < ((sSelf1BE540*)self)->count) {
                cUIText_setAsciiString((cUIText*)((sSelf1BE540*)self)->items[i], func_0025F1E0(mgr, idx, 0));
                *(int*)((char*)((sSelf1BE540*)self)->items[i] + 0x18) = idx;
                ((sSelf1BE540*)self)->items[i]->v09(1);
                ((sSelf1BE540*)self)->items[i]->v08(0);
            } else {
                ((sSelf1BE540*)self)->items[i]->v09(0);
                ((sSelf1BE540*)self)->items[i]->v08(1);
            }
        }
    } else {
        ((sSelf1BE540*)self)->count = 0;
        for (int j = 0; j < 10; j++) {
            ((sSelf1BE540*)self)->items[j]->v09(0);
            ((sSelf1BE540*)self)->items[j]->v08(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE720);
#ifdef SKIP_ASM
class cFEVObj_001BE720 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

extern "C" void func_001BE720(cFEStateLobbyMenu* self)
{
    if (self->miFirstPlayer == 0) {
        ((cFEVObj_001BE720*)self->mpPlayerUp)->v09(0);
    } else {
        ((cFEVObj_001BE720*)self->mpPlayerUp)->v09(1);
    }
    if (self->miFirstPlayer + 10 >= self->miNumPlayers) {
        ((cFEVObj_001BE720*)self->mpPlayerDown)->v09(0);
    } else {
        ((cFEVObj_001BE720*)self->mpPlayerDown)->v09(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup);
#ifdef SKIP_ASM
extern void* cFEStateNetCreateRoomPopup_vtbl[];
extern int cBXString_EmptyString;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the create-room popup state.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with the engine and parent, stores the vtable, starts mszRoomName,
 * mszChannelName and mszPassword as the empty string (cBXString_EmptyString) and sets mHashName 0x47.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @param pParentState  the parent state, the lobby (On Tour pParentState)
 * @return @p self
 */
extern "C" void* cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup(cFEStateNetCreateRoomPopup* self, int pEngine, int pParentState)
{
    func_001A8500_3(self, pEngine, pParentState);
    int emptyStr = cBXString_EmptyString;
    self->mVtbl = cFEStateNetCreateRoomPopup_vtbl;
    self->mszRoomName = (char*)emptyStr;
    self->mszChannelName = (char*)emptyStr;
    self->mszPassword = (char*)emptyStr;
    self->mHashName = 71;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetCreateRoomPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_232CreateRoomPopup[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates and plays the "232CreateRoomPopup" screen.
 *
 * @return cUIScreen_playFrame's result, or 0 when the screen could not be added
 */
extern "C" void* cFEStateNetCreateRoomPopup_onCreateScreen(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_232CreateRoomPopup), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetCreateRoomPopup_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the create-room popup: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateNetCreateRoomPopup_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetCreateRoomPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern const char* D_004A1A6C;
extern char str_LBChannelName[];
extern char str_EnterRoomName[];
extern char str_EnterPassword[];
extern char str_PairChannelName[];
extern char str_PairRoomName[];
extern char str_PairPassword[];
extern char str_TextDone[];
extern char str_kT_POPUP232Beginner[];
extern char str_kT_POPUP232Advanced[];
extern char str_kT_POPUP232Elite[];
extern char str_kT_POPUP232Voice[];
extern char D_004A1408[];

/**
 * Sets up the create-room widgets as the screen creates them.
 *
 * "LBChannelName" gets the channel entries Beginner (id 2), Advanced (1), Elite (0) and Voice (3), mszChannelName
 * starts as "Beginner" and the box is kept in mpLBChannelName. "EnterRoomName" and "EnterPassword" start empty and
 * become mpTextRoomName / mpTextPassword. Ids: "PairChannelName" 0, "PairRoomName" 1, "PairPassword" 2, "TextDone" 3.
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateNetCreateRoomPopup_onWidgetCreate(cFEStateNetCreateRoomPopup* self, cUIText* text)
{
    int id = *(int*)((char*)text + 0x38);
    if (id == GetHashValue32(str_LBChannelName)) {
        cUIListBox_addEntryByStringID(text, GetHashValue32(str_kT_POPUP232Beginner), 2);
        cUIListBox_addEntryByStringID(text, GetHashValue32(str_kT_POPUP232Advanced), 1);
        cUIListBox_addEntryByStringID(text, GetHashValue32(str_kT_POPUP232Elite), 0);
        cUIListBox_addEntryByStringID(text, GetHashValue32(str_kT_POPUP232Voice), 3);
        cBXString_cBXString4((char*)self + 0x6E0, D_004A1A6C);
        self->mpLBChannelName = (cUIListBox*)text;
    } else if (id == GetHashValue32(str_EnterRoomName)) {
        cUIText_setAsciiString(text, D_004A1408);
        self->mpTextRoomName = text;
    } else if (id == GetHashValue32(str_EnterPassword)) {
        cUIText_setAsciiString(text, D_004A1408);
        self->mpTextPassword = text;
    } else if (id == GetHashValue32(str_PairChannelName)) {
        *(int*)((char*)text + 0x18) = 0;
    } else if (id == GetHashValue32(str_PairRoomName)) {
        *(int*)((char*)text + 0x18) = 1;
    } else if (id == GetHashValue32(str_PairPassword)) {
        *(int*)((char*)text + 0x18) = 2;
    } else if (id == GetHashValue32(str_TextDone)) {
        *(int*)((char*)text + 0x18) = 3;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetCreateRoomPopup_onWidgetPair__FPv);
#ifdef SKIP_ASM
/**
 * Pair-widget events on the create-room popup: none are handled.
 *
 * @return always 0x101
 */
int cFEStateNetCreateRoomPopup_onWidgetPair(void* self)
{
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetCreateRoomPopup_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" int func_001BEC78(cFEStateNetCreateRoomPopup* self);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" int cUIListBox_getCurrentEntryData(void* a);
extern const char* D_004A1A60;
extern const char* D_004A1A64;
extern const char* D_004A1A68;
extern const char* D_004A1A6C;
extern char str_kT_POPUP231NewRoom[];
extern char str_kT_POPUP231AddPassword[];

class cFEVObj_001BEA38 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(void* owner, int msg);
};

/**
 * Handles the create-room widgets: name and password entry, channel choice, done and back.
 *
 * Event 5 by id: 1 opens the keyboard on the room name ("kT_POPUP231NewRoom", keyboard id 1, 8 characters), 2 on the
 * password ("kT_POPUP231AddPassword", id 2, 8 characters), 3 (done) tells the parent 0xF (slot 17) once func_001BEC78
 * accepts the form. Event 6 tells the parent 0x14. Event 9 on mpLBChannelName sets mszChannelName from the entry id:
 * 0 "Elite", 1 "Advanced", 2 "Beginner", 3 "Intermediate" (the entry labelled Voice). Event 13 (keyboard done) stores
 * the typed text in mszRoomName or mszPassword by the keyboard's id and shows it, then falls through to the
 * cFEStateNet handler like the other events.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void cFEStateNetCreateRoomPopup_onWidgetEvent(cFEStateNetCreateRoomPopup* self, int pWidget, unsigned int msg)
{
    char* obj = (char*)pWidget;
    switch (msg) {
    case 5:
        switch (*(int*)(obj + 0x18)) {
        case 1:
            func_001A9150((cFEStateNet*)self, (int)self->mpTextRoomName, GetHashValue32(str_kT_POPUP231NewRoom), (int)self->mszRoomName, 0, 1, 8);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            break;
        case 2:
            func_001A9150((cFEStateNet*)self, (int)self->mpTextPassword, GetHashValue32(str_kT_POPUP231AddPassword), (int)self->mszPassword, 0, 2, 8);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 53, 0, 2);
            break;
        case 3:
            if (func_001BEC78(self)) {
                ((cFEVObj_001BEA38*)self->mpParentState)->v17(self, 0xF);
            }
            break;
        }
        break;
    case 6:
        ((cFEVObj_001BEA38*)self->mpParentState)->v17(self, 20);
        break;
    case 9:
        if (obj == (char*)self->mpLBChannelName) {
            switch (cUIListBox_getCurrentEntryData(obj)) {
            case 0:
                cBXString_cBXString4((char*)self + 0x6E0, D_004A1A60);
                break;
            case 1:
                cBXString_cBXString4((char*)self + 0x6E0, D_004A1A64);
                break;
            case 2:
                cBXString_cBXString4((char*)self + 0x6E0, D_004A1A6C);
                break;
            case 3:
                cBXString_cBXString4((char*)self + 0x6E0, D_004A1A68);
                break;
            }
        }
        break;
    case 13: {
        char* keyboard = (char*)self->mpKeyboard;
        switch (*(int*)(keyboard + 0x18)) {
        case 1:
            cBXString_cBXString4((char*)self + 0x6DC, keyboard + 0x74);
            cUIText_setAsciiString(self->mpTextRoomName, (const char*)self->mszRoomName);
            break;
        case 2:
            cBXString_cBXString4((char*)self + 0x6E4, keyboard + 0x74);
            cUIText_setAsciiString(self->mpTextPassword, (const char*)self->mszPassword);
            break;
        }
    }
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEC78);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" int strlen(const char* s);
extern char str_kT_POPUP231UniqueRoom[];
extern char str_kT_BTNOK[];

extern "C" int func_001BEC78(cFEStateNetCreateRoomPopup* self)
{
    if (strlen((const char*)self->mszRoomName) == 0) {
        int three = 3;
        void* popup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 1;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP231UniqueRoom);
        *(int*)((char*)popup + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
        *(int*)((char*)popup + 0x14C) = three;
        *(int*)((char*)popup + 0x150) = 2119;
        func_001A9BC0((cFEStateNet*)self, popup);
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BED28);
#ifdef SKIP_ASM
class func_001BED28_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

extern "C" int func_001BED28(cFEStateNet* self, int a1, int a2)
{
    return ((func_001BED28_cObj*)self->mpParentState)->v24(a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BED58);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP232CreateRoom[];
extern char str_kT_HELP232CreateRoomMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001BED58(cFEStateNetCreateRoomPopup* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP232CreateRoom);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP232CreateRoomMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    func_001DA510(popup, 2, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(popup, 3, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLogin_cFEStateLogin);
#ifdef SKIP_ASM
extern void* cFEStateLogin_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the online login-type state (use, choose, create or delete an account).
 *
 * Runs the cFEStateNet constructor (func_001A8500) with no parent, stores mHashName 0x33 and cFEStateLogin_vtbl,
 * clears mpustrDisconnect and unk_6A8 (net messages are not filtered here).
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateLogin_cFEStateLogin(cFEStateLogin* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mHashName = 51;
    self->mVtbl = cFEStateLogin_vtbl;
    self->mpustrDisconnect = 0;
    self->unk_6A8 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLogin__cFEStateLogin);
#ifdef SKIP_ASM
extern void* cFEStateLogin_vtbl[];
void cMemMan_free(void* p);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the login state: frees mpustrDisconnect, then runs the cFEStateNet destructor (func_001A85D0).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateLogin__cFEStateLogin(cFEStateLogin* self, int flags)
{
    self->mVtbl = cFEStateLogin_vtbl;
    if (self->mpustrDisconnect != 0) {
        cMemMan_free(self->mpustrDisconnect);
        self->mpustrDisconnect = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateLogin_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char str_201login_type[];

/**
 * Creates and plays the "201login_type" screen, then runs the cFEStateNet hook (func_001A8768).
 */
extern "C" void cFEStateLogin_onCreateScreen(cFEStateLogin* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_201login_type), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001A8768(((cFEStateNet*)self));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEF40__FPv);
#ifdef SKIP_ASM
void* func_001BEF40(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLogin_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern "C" int strlen(const char* s);
// PORT: usprintf is a varargs formatter (buf, fmt, ...); the unit declares a 3-arg view
extern "C" void usprintf_1BEF60(unsigned short* buf, const char* fmt, ...) __asm__("usprintf");
extern char str_pct_s_pct_S[];
extern char str_TextUseAccount[];
extern char str_kT_CMNUse[];
extern char str_1rectangle[];
extern char str_4rectangle[];
extern char str_TextCreateAccount[];
extern char str_TextExistingAccount[];
extern char str_TextDelectAccount[];
extern char str_kT_BTNDelete[];
extern char D_00536760[];

struct sVEntry_001BEF60 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc_001BEF60(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001BEF60* vt = *(sVEntry_001BEF60**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

static inline int IsHash_001BEF60(int id, char* name) { return id == GetHashValue32(name); }

struct sItem_001BEF60 {
    char pad[0x18];
    int mState;
    char pad1C[0x38 - 0x1C];
    int mId;
};

class cFEVObj_001BEF60 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25(int on);
};

/**
 * Sets up the login-type widgets from the account saved on the memory card (D_00536760).
 *
 * Ids (+0x18): "TextUseAccount" 0, "TextExistingAccount" 1, "TextCreateAccount" 2, "TextDelectAccount" 3 (sic).
 * "TextUseAccount" and "TextDelectAccount" read "kT_CMNUse" / "kT_BTNDelete" followed by the saved account name ("%s
 * %S"); with no saved name they are greyed (setUseStaticColour, setStatic) and hidden, as are "1rectangle" and
 * "4rectangle". The create and existing-account texts are always visible.
 *
 * @param pWidget  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateLogin_onWidgetCreate(cFEStateLogin* self, cFEVObj_001BEF60* pWidget)
{
    unsigned short buf[64];
    sItem_001BEF60* widget = (sItem_001BEF60*)pWidget;
    if (IsHash_001BEF60(widget->mId, str_TextUseAccount)) {
        widget->mState = 0;
        int locStr = Loc_001BEF60(str_kT_CMNUse);
        usprintf_1BEF60(buf, str_pct_s_pct_S, locStr, D_00536760);
        cUIText_setUnicodeString((cUIText*)pWidget, (char*)buf);
        pWidget->v09(1);
        if (strlen(D_00536760) == 0) {
            pWidget->v25(1);
            pWidget->v08(1);
            pWidget->v09(0);
        }
    } else if (IsHash_001BEF60(widget->mId, str_1rectangle)) {
        if (strlen(D_00536760) == 0) {
            pWidget->v09(0);
        } else {
            pWidget->v09(1);
        }
    } else if (IsHash_001BEF60(widget->mId, str_4rectangle)) {
        if (strlen(D_00536760) == 0) {
            pWidget->v09(0);
        } else {
            pWidget->v09(1);
        }
    } else if (IsHash_001BEF60(widget->mId, str_TextCreateAccount)) {
        widget->mState = 2;
        pWidget->v09(1);
    } else if (IsHash_001BEF60(widget->mId, str_TextExistingAccount)) {
        widget->mState = 1;
        pWidget->v09(1);
    } else if (IsHash_001BEF60(widget->mId, str_TextDelectAccount)) {
        widget->mState = 3;
        int locStr = Loc_001BEF60(str_kT_BTNDelete);
        usprintf_1BEF60(buf, str_pct_s_pct_S, locStr, D_00536760);
        cUIText_setUnicodeString((cUIText*)pWidget, (char*)buf);
        pWidget->v09(1);
        if (strlen(D_00536760) == 0) {
            pWidget->v25(1);
            pWidget->v08(1);
            pWidget->v09(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BF228);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025C610(void* self);
extern "C" int func_001A8E40_3i(void* self, int a1, int a2) __asm__("func_001A8E40");
extern const char str_cFEStateSelectPersona[];
extern char str_kT_POPUP201Verify[];
extern char str_kT_BTNCancel[];
extern char D_00536760[];

extern "C" int func_001BF228(cFEStateLogin* self, int id, int a2)
{
    switch (id) {
    case 200: {
        void* p = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUP201Verify);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNCancel));
        *(int*)((char*)p + 0x150) = 2062;
        func_001A9BC0(((cFEStateNet*)self), p);
        break;
    }
    case 201: {
        char* names = D_00536760;
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, names);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, names + 0x11);
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    }
    case 212:
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(1752, str_cFEStateSelectPersona, 0, 0), (int)self->mpEngine));
        break;
    default:
        return func_001A8E40_3i(((cFEStateNet*)self), id, a2);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BF378);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

static inline void* GetSession_001BF378()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001BF378(cFEStateLogin* self, void* popup, int event)
{
    if (event == 0xF || event == 20) {
        func_00261408(GetSession_001BF378());
        func_0025B800(LobbyMan_mLobbyMan);
        func_001A97B8(self, popup, event);
    } else {
        func_001A97B8(self, popup, event);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BF438);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern char str_TextUseAccount[];
extern char str_TextDelectAccount[];
extern char str_1rectangle[];
extern char str_4rectangle[];
extern char str_Menu0000[];

class cFEVObj_001BF438 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18(void* obj);
};

extern "C" void func_001BF438(cFEStateLogin* self, void* a1, int kind)
{
    if (kind == 15) {
        func_001A97B8(((cFEStateNet*)self), a1, 15);
        {
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextUseAccount));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        {
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextDelectAccount));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        {
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_1rectangle));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        {
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_4rectangle));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu0000));
        if (menu) {
            char* item = *(char**)((char*)menu + 0xA0);
            if (item && ((*(int*)(item + 0x14) >> 5) & 1)) {
                cUIMenu_setSelectedByIndex(menu, 1);
            }
        }
    } else {
        func_001A97B8(((cFEStateNet*)self), a1, kind);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BF5A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_001A8A40_r(void* self, void* obj) __asm__("func_001A8A40");
extern "C" void* cFEStateNetConfig_cFEStateNetConfig(cFEStateNetConfig* mem, int a1);
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern const char str_cFEStateNetConfig[];

static inline void* GetSession_001BF5A8()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001BF5A8(cFEStateLogin* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15:
        self->unk_6B0 = 1;
        if (func_001A8A40_r(((cFEStateNet*)self), cFEStateNetConfig_cFEStateNetConfig((cFEStateNetConfig*)cMemMan_alloc(2332, str_cFEStateNetConfig, 0, 0), (int)self->mpEngine)) != 0) {
            func_00261408(GetSession_001BF5A8());
            func_0025B800(LobbyMan_mLobbyMan);
            func_002636F0(func_002591B8(), 0);
        }
        break;
    case 16:
    case 20:
        func_001A97B8(((cFEStateNet*)self), a1, kind);
        break;
    default:
        func_001A97B8(((cFEStateNet*)self), a1, kind);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateLogin_onPopupEvent);
#ifdef SKIP_ASM
class cFEVObj_001BF6C0 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33(void* a1, int a2);
};

extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BF438(cFEStateLogin* self, void* a1, int a2);
extern "C" void func_001BF5A8(cFEStateLogin* self, void* a1, int a2);

/**
 * Hands a popup's event on the login screen to the handler for that popup's id.
 *
 * 0x80E goes to its slot 33, 0x82C (account memory card) to func_001BF438, 0x848 to func_001BF5A8, any other to the
 * cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateLogin_onPopupEvent(cFEStateLogin* self, void* pPopup, int Event)
{
    switch (*(unsigned int*)((char*)pPopup + 0x18)) {
    case 2062:
        ((cFEVObj_001BF6C0*)self)->v33(pPopup, Event);
        break;
    case 2092:
        func_001BF438(self, pPopup, Event);
        break;
    case 2120:
        func_001BF5A8(self, pPopup, Event);
        break;
    default:
        func_001A97B8(((cFEVObj_001BF6C0*)self), pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLogin_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int USTR_length(void* s);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void cMemMan_free(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEUseExistingAccount_cFEUseExistingAccount(cFEUseExistingAccount* self, int a1);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1(cFEStateCreateStep1* self, int a1, int a2);
extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement(cFEStateOnlineAgreement* self, int a1);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0025B6D8(void* self, int a1);
extern "C" void sprintf2(void* dst, const char* fmt, int a, int b);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEUseExistingAccount[];
extern char str_cFEStateCreateStep1[];
extern char str_cFEStateNetAccountMem[];
extern char str_cFEStateOnlineAgreement[];
extern char str_kT_POPUPCMNDisconnecting[];
extern char str_kT_POPUPCMNDisconnect[];
extern char str_pct_s_pct_s[];
extern const char str_cFEStateLogin[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

struct sVEntry_001BF758 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1BF758(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_001BF758* vt = *(sVEntry_001BF758**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

/**
 * Handles the online login screen: logs in with the saved account, creates or links one, manages it on the card, or
 * offers to disconnect.
 *
 * A null widget is ignored. Event 5 by the widget's id (+0x18): 0 copies SSXApp->UserName and Password into
 * LobbyMan_mLobbyMan's strings at +0x48 / +0x4C and calls func_0025B6D8(LobbyMan_mLobbyMan, 0); 2 switches
 * (func_001A8A40) to a new cFEStateCreateStep1 (mode 1); 1 to a new cFEUseExistingAccount; 3 pushes a
 * cFEStateNetAccountMem popup (mode 3, miMiscData 0x82C). Event 6 (back) frees mpustrDisconnect and rebuilds it as
 * "kT_POPUPCMNDisconnecting" + "kT_POPUPCMNDisconnect" ("%s%s"), then asks it with yes / no (id 0x848, default
 * no); when the allocation fails the popup shows "kT_POPUPCMNDisconnect" by id instead. Event 8 switches to a new
 * cFEStateOnlineAgreement (miMiscData 18); the rest go to the cFEStateNet handler cFEStateNet_onWidgetEvent. The subclass vtable
 * cFEStateRelogin_vtbl uses this handler too.
 *
 * @param popup  the widget (On Tour pWidget; not a popup despite the name); null is ignored
 * @param kind  the widget event (On Tour Event)
 */
extern "C" void cFEStateLogin_onWidgetEvent(cFEStateLogin* self, void* popup, unsigned int kind)
{
    if (popup == 0) {
        return;
    }
    switch (kind) {
    case 5:
        switch (*(int*)((char*)popup + 0x18)) {
        case 0:
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, (const char*)SSXApp->UserName);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, (const char*)SSXApp->Password);
            func_0025B6D8(LobbyMan_mLobbyMan, 0);
            break;
        case 2:
            func_001A8A40(self, cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(1780, str_cFEStateCreateStep1, 0, 0), (int)self->mpEngine, 1));
            break;
        case 1:
            func_001A8A40(self, cFEUseExistingAccount_cFEUseExistingAccount((cFEUseExistingAccount*)cMemMan_alloc(1764, str_cFEUseExistingAccount, 0, 0), (int)self->mpEngine));
            break;
        case 3: {
            void* newState = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, self, 3, 0, 0);
            *(int*)((char*)newState + 0x18) = 2092;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, newState);
            break;
        }
        }
        break;
    case 6: {
        if (self->mpustrDisconnect != 0) {
            cMemMan_free(self->mpustrDisconnect);
            self->mpustrDisconnect = 0;
        }
        int bodyText = Loc1BF758(str_kT_POPUPCMNDisconnecting);
        char* title = str_kT_POPUPCMNDisconnect;
        int titleText = Loc1BF758(title);
        char* sep = str_pct_s_pct_s;
        int len = USTR_length((void*)bodyText) + USTR_length((void*)titleText) + strlen(sep) + 1;
        self->mpustrDisconnect = (unsigned short*)operator_new_tag(len * 2, str_cFEStateLogin, 0x100, 0);
        void* newPopup = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)newPopup + 0xBC;
        if (self->mpustrDisconnect != 0) {
            sprintf2(self->mpustrDisconnect, sep, bodyText, titleText);
            *(void**)((char*)newPopup + 0xC4) = (void*)self->mpustrDisconnect;
            *(int*)((char*)newPopup + 0xC0) = 1;
        } else {
            *(int*)((char*)newPopup + 0xC4) = GetHashValue32(title);
            *(int*)((char*)newPopup + 0xC0) = 3;
        }
        *(int*)sub = 2;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
        *(int*)(sub + 0xAC) = 1;
        *(int*)(sub + 0x94) = 2120;
        func_001A9BC0((cFEStateNet*)self, newPopup);
        break;
    }
    case 8: {
        void* newState = cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(1756, str_cFEStateOnlineAgreement, 0x100, 0), (int)self->mpEngine);
        *(int*)((char*)newState + 0x18) = 18;
        func_001A8A40(self, newState);
        break;
    }
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)popup, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFAE0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP201SelectLoginType[];
extern char str_kT_HELP201SelectLoginTypeMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];
extern char str_kT_HELPCmnMembershipAgreement[];

extern "C" void func_001BFAE0(cFEStateLogin* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP201SelectLoginType);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP201SelectLoginTypeMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(popup, 2, GetHashValue32(str_kT_HELPCmnPrevious), 1);
    func_001DA510(popup, 3, GetHashValue32(str_kT_HELPCmnMembershipAgreement), 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineMainMenu_cFEStateOnlineMainMenu);
#ifdef SKIP_ASM
extern void* cFEStateOnlineMainMenu_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the online main-menu state: the cFEStateNet constructor with no parent, mHashName 0x3B and its vtable.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cFEStateNet* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mHashName = 59;
    self->mVtbl = cFEStateOnlineMainMenu_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineMainMenu_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A9930(cFEStateNet* self);
extern "C" void func_00256C50();
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* p, int a1);
extern char str_230online_main_menu[];

/**
 * Creates the "230online_main_menu" screen and tidies up any previous online session.
 *
 * Adds and plays the screen, opens the disconnected popup (func_001A9930) when LobbyMan's mState is 0, calls
 * func_00256C50 when a cGameComm exists, and calls func_0028F140(func_0028B180(), 1).
 */
extern "C" void cFEStateOnlineMainMenu_onCreateScreen(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_230online_main_menu), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    if (LobbyMan_mLobbyMan->mState == 0) {
        func_001A9930(self);
    }
    if (cGameComm_mGameComm != 0) {
        func_00256C50();
    }
    func_0028F140(func_0028B180(), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineMainMenu_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the online main menu: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateOnlineMainMenu_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineMainMenu_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_TextQuickMatchup[];
extern char str_TextLobby[];
extern char str_TextStats[];
extern char str_TextSetupCharacter[];
extern char str_TextOnlineAgreement[];
extern char str_TextLogoff[];

static inline int IsHash1BFCC0(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Numbers the online main-menu items as the screen creates them.
 *
 * Ids (+0x18): "TextSetupCharacter" 0, "TextQuickMatchup" 1, "TextLobby" 2, "TextStats" 3, "TextOnlineAgreement" 4,
 * "TextLogoff" 5.
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateOnlineMainMenu_onWidgetCreate(cFEStateNet* self, void* obj)
{
    if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), str_TextQuickMatchup)) {
        *(int*)((char*)obj + 0x18) = 1;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), str_TextLobby)) {
        *(int*)((char*)obj + 0x18) = 2;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), str_TextStats)) {
        *(int*)((char*)obj + 0x18) = 3;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), str_TextSetupCharacter)) {
        *(int*)((char*)obj + 0x18) = 0;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), str_TextOnlineAgreement)) {
        *(int*)((char*)obj + 0x18) = 4;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), str_TextLogoff)) {
        *(int*)((char*)obj + 0x18) = 5;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFD98);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP230OnlineMainMenu[];
extern char str_kT_HELP230OnlineMainMenuMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_BTNEAMessenger[];
extern char str_kT_HELP230ReturnToMainMenu[];

extern "C" void func_001BFD98(cFEStateNet* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP230OnlineMainMenu);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP230OnlineMainMenuMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(popup, 2, GetHashValue32(str_kT_BTNEAMessenger), 2);
    func_001DA510(popup, 3, GetHashValue32(str_kT_HELP230ReturnToMainMenu), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineMainMenu_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup_FE70(void* mem, int a1, void* owner) __asm__("cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup");
extern "C" void* cFEStateLobbyMenu_cFEStateLobbyMenu_FE70(void* mem, int a1) __asm__("cFEStateLobbyMenu_cFEStateLobbyMenu");
extern "C" void* cFEStateStats_cFEStateStats_FE70(void* mem, int a1) __asm__("cFEStateStats_cFEStateStats");
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld_FE70(void* mem, int a1) __asm__("cFEStateNetSSXWorld_cFEStateNetSSXWorld");
extern char str_cFEStateNetQuickPlayPopup[];
extern char str_cFEStateStats[];
extern const char str_cFEStateNetSSXWorld[];
extern char str_kT_POPUP230WarningLeaving[];
extern char str_kT_POPUPCMNDisconnectServer[];

class cFEFactory_001BFE70 {
public:
    int f0;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* create(void* owner, int a2);
};

/**
 * Handles the online main menu: opens the chosen online screen, or asks before logging off or disconnecting.
 *
 * A null widget is ignored. Event 5 by id: 1 sets D_004A4E88 and pushes a cFEStateNetQuickPlayPopup (miMiscData
 * 0x82E); 2, 3 and 4 switch (func_001A8A40) to a new cFEStateLobbyMenu, cFEStateStats or cFEStateNetSSXWorld (the
 * last from "TextOnlineAgreement"); 0 switches to the state the engine's factory creates for id 0, when any; 5 asks
 * "kT_POPUP230WarningLeaving" (OK / Cancel, id 0x82D). Event 6 asks "kT_POPUPCMNDisconnectServer" (yes / no, id
 * 0x846). Event 8 calls func_001A9B58 unless mAttrState bits 1-2 are set; the rest go to the cFEStateNet handler. The
 * comment above it ("memory-card async state") is stale.
 *
 * @param pWidget  the widget (On Tour pWidget); null is ignored
 * @param Event  the widget event (On Tour Event)
 */
// Memory-card async state message handler: creates the next screen or shows an error popup.
extern "C" void cFEStateOnlineMainMenu_onWidgetEvent(cFEStateNet* self, void* pWidget, unsigned int Event)
{
    if (pWidget == 0) {
        return;
    }
    switch (Event) {
    case 5:
        switch (*(unsigned int*)((char*)pWidget + 0x18)) {
        case 1: {
            D_004A4E88 = 1;
            void* popup = cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup_FE70(cMemMan_alloc(1844, str_cFEStateNetQuickPlayPopup, 0, 0), (int)self->mpEngine, self);
            *(int*)((char*)popup + 0x18) = 2094;
            cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, popup);
            break;
        }
        case 2:
            func_001A8A40(self, cFEStateLobbyMenu_cFEStateLobbyMenu_FE70(cMemMan_alloc(2948, str_cFEStateLobbyMenu, 0, 0), (int)self->mpEngine));
            break;
        case 3:
            func_001A8A40(self, cFEStateStats_cFEStateStats_FE70(cMemMan_alloc(1776, str_cFEStateStats, 0, 0), (int)self->mpEngine));
            break;
        case 0: {
            void* obj = (*(cFEFactory_001BFE70**)self->mpEngine)->create(self, 0);
            if (obj == 0) {
                break;
            }
            func_001A8A40(self, obj);
            break;
        }
        case 4:
            func_001A8A40(self, cFEStateNetSSXWorld_cFEStateNetSSXWorld_FE70(cMemMan_alloc(1744, str_cFEStateNetSSXWorld, 0, 0), (int)self->mpEngine));
            break;
        case 5: {
            void* popup = func_001A9C50(self);
            char* sub = (char*)popup + 0xBC;
            *(int*)((char*)popup + 0xBC) = 2;
            *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUP230WarningLeaving);
            *(int*)((char*)popup + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_BTNCancel));
            *(int*)((char*)popup + 0x150) = 2093;
            func_001A9BC0(self, popup);
            break;
        }
        }
        break;
    case 6: {
        void* popup = func_001A9C50(self);
        char* sub = (char*)popup + 0xBC;
        *(int*)((char*)popup + 0xBC) = 2;
        *(int*)((char*)popup + 0xC4) = GetHashValue32(str_kT_POPUPCMNDisconnectServer);
        *(int*)((char*)popup + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
        *(int*)((char*)popup + 0x150) = 2118;
        func_001A9BC0(self, popup);
        break;
    }
    case 8:
        if ((self->mAttrState & 6) == 0) {
            func_001A9B58(self);
        }
        break;
    default:
        cFEStateNet_onWidgetEvent(self, (int)pWidget, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0138);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
// PORT: the unit declares func_001A8A40 as returning void; the body returns 0/1
extern "C" int func_001A8A40_r(void* self, void* obj) __asm__("func_001A8A40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00194A60(void* mem, int a1);
extern "C" int* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern const char str_cFEStateMainMenu[];

static inline void* GetSession_001C0138()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    return BuddyMan_mBuddyMan;
}

extern "C" void func_001C0138(cFEStateNet* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8_v(self, a1, 0xF);
        self->unk_6B0 = 1;
        if (func_001A8A40_r(self, func_00194A60(cMemMan_alloc(88, str_cFEStateMainMenu, 0, 0), (int)self->mpEngine)) != 0) {
            func_00261408(GetSession_001C0138());
            func_0025B800(LobbyMan_mLobbyMan);
            func_002636F0(func_002591B8(), 0);
        }
    } else {
        func_001A97B8_v(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0218);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001C0218(cFEStateNet* self, void* popup, int kind)
{
    if (kind != 15) {
        func_001A97B8(self, popup, kind);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0240);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0025B800(void* self);
extern "C" void func_00261408(void* self);
extern "C" void func_00261070(void* self);
void func_00261058(void* self);
// func_001A97B8 returns void (its asm never sets $2); the unit declares it int
extern "C" void func_001A97B8_v(void* self, void* a1, int a2) __asm__("func_001A97B8");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEStateNetAccountMem[];

extern "C" void func_001C0240(cFEStateNet* self, void* popup, int event)
{
    if (event == 0xF) {
        func_001A97B8_v(self, popup, 0xF);
        func_00261408(BuddyMan_mBuddyMan);
        func_00261070(BuddyMan_mBuddyMan);
        func_00261058(BuddyMan_mBuddyMan);
        func_0025B800(LobbyMan_mLobbyMan);
        void* accountMem = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(2832, str_cFEStateNetAccountMem, 0, 0), (int)self->mpEngine, self, 0, 0, 0);
        *(int*)((char*)accountMem + 0x18) = 2098;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, accountMem);
    } else {
        func_001A97B8_v(self, popup, event);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineMainMenu_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C0138(cFEStateNet* self, void* a1, int a2);
extern "C" void func_001C0240(cFEStateNet* self, void* a1, int a2);

/**
 * Hands a popup's event on the online main menu to the handler for that popup's id.
 *
 * 0x82D (log off) func_001C0138, 0x846 (disconnect) func_001C0240, any other the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateOnlineMainMenu_onPopupEvent(cFEStateNet* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2093:
        func_001C0138((cFEStateNet*)self, pPopup, Event);
        break;
    case 2118:
        func_001C0240((cFEStateNet*)self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C0358);
#ifdef SKIP_ASM
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern const char str_cFEStateOnlineCharacterSelect[];

extern "C" void* func_001C0358(cFEStateNet* self, int msgId, int data)
{
    switch (msgId) {
    default:
        return func_001A8E40_3(self, msgId, data);
    case 243:
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(1764, str_cFEStateOnlineCharacterSelect, 0x100, 0), (int)self->mpEngine));
        break;
    case 239:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_cFEStateOnlineAgreement1);
#ifdef SKIP_ASM
extern void* cFEStateOnlineAgreement_vtbl[];
extern "C" void* func_001A8500(void* self);

extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement1(cFEStateOnlineAgreement* self)
{
    func_001A8500(self);
    self->mHashName = 67;
    self->mVtbl = cFEStateOnlineAgreement_vtbl;
    self->mpTOSLines = 0;
    self->miNextTOSLine = (unsigned int)0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_cFEStateOnlineAgreement);
#ifdef SKIP_ASM
extern void* cFEStateOnlineAgreement_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the online terms-of-service state: cFEStateNet constructor with no parent, mHashName 0x43, its vtable.
 *
 * Also clears mpTOSLines and miNextTOSLine. Where it goes afterwards is decided by the miMiscData its creator sets.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement(cFEStateOnlineAgreement* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mHashName = 67;
    self->mVtbl = cFEStateOnlineAgreement_vtbl;
    self->mpTOSLines = 0;
    self->miNextTOSLine = (unsigned int)0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_cFEStateOnlineAgreement2);
#ifdef SKIP_ASM
extern void* cFEStateOnlineAgreement_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement2(cFEStateOnlineAgreement* self)
{
    func_001A8500_3(self, 0, 0);
    self->mpTOSLines = 0;
    self->mVtbl = cFEStateOnlineAgreement_vtbl;
    self->miNextTOSLine = (unsigned int)0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001C0540(cFEStateOnlineAgreement* self);
extern char str_259Online_Agreement[];
extern char str_AgreementMenu[];

/**
 * Creates the "259Online_Agreement" screen and builds the terms-of-service line list.
 *
 * Adds the screen and, when it exists, plays it and fills mpTOSLines (func_001C0540). Then gives "AgreementMenu" bit
 * 3 of its +0x90 flags; that lookup runs on mpScreen even when the screen could not be added.
 */
extern "C" void cFEStateOnlineAgreement_onCreateScreen(cFEStateOnlineAgreement* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_259Online_Agreement), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001C0540(self);
    }
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_AgreementMenu));
    if (text != 0) {
        *(int*)((char*)text + 0x90) |= 8;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0540);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
int GetHashValue32(char* str);
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_cFEStateOnlineAgreement[];
extern char str_kT_TOSpct_03d[];
extern char str_kT_TOS115a[];
extern char str_kT_TOS117a[];
extern char str_kT_TOS117b[];
extern char str_kT_TOS117c[];
extern char str_kT_TOS117d[];
extern char str_kT_TOS117e[];
extern char str_kT_TOS124a[];
extern char str_kT_TOS125[];
extern char str_kT_TOS125a[];
extern char str_kT_TOS125b[];
extern char str_kT_TOS128a[];
extern char str_kT_TOS128b[];
extern char str_kT_TOS128c[];
extern char str_kT_TOS128d[];
extern char str_kT_TOS128e[];
extern char str_kT_TOS129[];
extern char str_kT_TOS130[];
extern char str_kT_TOS130a[];
extern char str_kT_TOS130b[];
extern char str_kT_TOS130c[];
extern char str_kT_TOS130d[];
extern char str_kT_TOS153a[];
extern char str_kT_TOS153b[];

extern "C" void func_001C0540(cFEStateOnlineAgreement* self)
{
    self->miNumTOSLines = 178;
    if (self->mpTOSLines != 0) {
        cMemMan_free(self->mpTOSLines);
        self->mpTOSLines = 0;
    }
    if (self->miNumTOSLines != 0) {
        char buf[32];
        int i;
        int n;
        self->mpTOSLines = (int*)operator_new_tag(self->miNumTOSLines * 4, str_cFEStateOnlineAgreement, 0x100, 0);
        n = 0;
        for (i = 0x0; i < 116; i++) {
            sprintf(buf, str_kT_TOSpct_03d, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS115a);
        for (i = 116; i < 118; i++) {
            sprintf(buf, str_kT_TOSpct_03d, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS117a);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS117b);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS117c);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS117d);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS117e);
        for (i = 122; i < 125; i++) {
            sprintf(buf, str_kT_TOSpct_03d, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS124a);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS125);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS125a);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS125b);
        for (i = 0x7E; i < 129; i++) {
            sprintf(buf, str_kT_TOSpct_03d, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS128a);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS128b);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS128c);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS128d);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS128e);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS129);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS130);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS130a);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS130b);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS130c);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS130d);
        for (i = 131; i < 154; i++) {
            sprintf(buf, str_kT_TOSpct_03d, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS153a);
        (self->mpTOSLines)[n++] = GetHashValue32(str_kT_TOS153b);
        for (i = 158; i < 166; i++) {
            sprintf(buf, str_kT_TOSpct_03d, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement__cFEStateOnlineAgreement);
#ifdef SKIP_ASM
extern void* cFEStateOnlineAgreement_vtbl[];
void cMemMan_free(void* p);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the terms-of-service state: frees mpTOSLines, then runs the cFEStateNet destructor (func_001A85D0).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateOnlineAgreement__cFEStateOnlineAgreement(cFEStateOnlineAgreement* self, int flags)
{
    self->mVtbl = cFEStateOnlineAgreement_vtbl;
    if (self->mpTOSLines != 0) {
        cMemMan_free(self->mpTOSLines);
        self->mpTOSLines = 0;
    }
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_onUpdate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self);
extern "C" void cUITextScroll_addUnicodeStringByID(cUIText* text, int a1);
extern char str_AgTextScroll[];

/**
 * Adds one terms-of-service line per frame to "AgTextScroll" until all miNumTOSLines are in.
 *
 * Each frame hands mpTOSLines[miNextTOSLine++] to the text scroll (cUITextScroll_addUnicodeStringByID), then runs the cFEStateNet update.
 */
extern "C" void cFEStateOnlineAgreement_onUpdate(cFEStateOnlineAgreement* self)
{
    if (self->miNextTOSLine < self->miNumTOSLines && self->mpTOSLines != 0) {
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_AgTextScroll));
        if (text != 0) {
            cUITextScroll_addUnicodeStringByID(text, (self->mpTOSLines)[(self->miNextTOSLine)++]);
        }
    }
    cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the terms-of-service screen: reports event 9 as handled (0x100), any other as not (0x101).
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (On Tour Event)
 * @return 0x100 for event 9, else 0x101
 */
extern "C" int cFEStateOnlineAgreement_onWidgetMenu(cFEStateOnlineAgreement* self, int pMenu, int Event)
{
    return Event != 9 ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" void cUITextScroll_setInputMap(void* self, int a1);
extern char str_AgreementSlider[];
extern char str_ChatUpArrow[];
extern char str_ChatDownArrow[];
extern char str_slideroutline2[];
extern char str_AgTextScroll[];
extern char str_top_arrow[];
extern char str_bot_arrow[];

class cFEVObj_001C0B20 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

/**
 * Wires the terms-of-service scroll bar to its text as the screen opens.
 *
 * Calls its slot 32, then gives "AgreementSlider" the arrows "ChatUpArrow" / "ChatDownArrow", the highlight names
 * "top arrow" / "bot arrow" (+0x8C / +0xA5, copied when at most 24 characters; the second is copied twice) and the
 * outline "slideroutline2"; "AgTextScroll" gets the slider (+0xB8), cUITextScroll_setInputMap(1) and focus. The slider is made
 * visible only when found, although it was already written to before that check.
 *
 * @return 1 (the transition is done)
 */
extern "C" int cFEStateOnlineAgreement_onGainTransition(cFEStateOnlineAgreement* self)
{
    ((cFEVObj_001C0B20*)self)->v32();
    char* scrollBar = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_AgreementSlider));
    cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatUpArrow));
    cUIText* downArrow = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatDownArrow));
    cUIText* sliderOutline = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_slideroutline2));
    *(cUIText**)(scrollBar + 0x7C) = uiText;
    *(cUIText**)(scrollBar + 0x80) = downArrow;
    if ((unsigned int)strlen(str_top_arrow) > 24) {
        scrollBar[0x8C] = 0;
    } else {
        strcpy(scrollBar + 0x8C, str_top_arrow);
    }
    if ((unsigned int)strlen(str_bot_arrow) > 24) {
        scrollBar[0xA5] = 0;
    } else {
        strcpy(scrollBar + 0xA5, str_bot_arrow);
    }
    if ((unsigned int)strlen(str_bot_arrow) > 24) {
        scrollBar[0xA5] = 0;
    } else {
        strcpy(scrollBar + 0xA5, str_bot_arrow);
    }
    *(cUIText**)(scrollBar + 0x84) = sliderOutline;
    char* x = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_AgTextScroll));
    *(char**)(x + 0xB8) = scrollBar;
    cUITextScroll_setInputMap(x, 1);
    ((cFEVObj_001C0B20*)x)->v07(1);
    if (scrollBar) {
        ((cFEVObj_001C0B20*)scrollBar)->v09(1);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_onWidgetCreate);
#ifdef SKIP_ASM
extern char str_AgreementSlider[];
int GetHashValue32(char* str);

/**
 * Sets attribute bit 0 of "AgreementSlider" as the screen creates it.
 *
 * @param item  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateOnlineAgreement_onWidgetCreate(cFEStateOnlineAgreement* self, void* item)
{
    int hash = *(int*)((char*)item + 0x38);
    if (hash == GetHashValue32(str_AgreementSlider)) {
        *(int*)((char*)item + 0x14) |= 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineAgreement_onWidgetEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void* cFEStateLogin_cFEStateLogin(cFEStateLogin* mem, int a1);
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1(cFEStateCreateStep1* self, int a1, int a2);
extern "C" void* cFEStateCreateStep2_cFEStateCreateStep2(cFEStateCreateStep2* self, int a1);
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int a1);
extern char str_cFEStateNetLogin[];
extern char str_cFEStateCreateStep1[];
extern char str_cFEStateCreateStep2[];
extern const char str_cFEStateNetSSXWorld[];

/**
 * Leaves the terms-of-service screen on select or back, for the screen chosen by its miMiscData.
 *
 * 0x12 a new cFEStateLogin, 0x14 a cFEStateCreateStep1 (flag 1), 0x15 a cFEStateCreateStep1 (flag 0), 0x16 a
 * cFEStateCreateStep2, 0x13 or anything else a cFEStateNetSSXWorld; the switch is func_001A8A40. A null widget is
 * ignored; other events go to the cFEStateNet handler.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget); null is ignored
 * @param msg  the widget event (On Tour Event)
 */
// On confirm (msg 5/6), opens the screen that matches this menu's id; other messages go to the base handler.
extern "C" void cFEStateOnlineAgreement_onWidgetEvent(cFEStateOnlineAgreement* self, int pWidget, unsigned int msg)
{
    void* obj;
    if (pWidget == 0) {
        return;
    }
    switch (msg) {
    case 5:
    case 6:
        switch (self->miMiscData) {
        case 18:
            obj = cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(1748, str_cFEStateNetLogin, 0, 0), (int)self->mpEngine);
            break;
        case 20:
            obj = cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(1780, str_cFEStateCreateStep1, 0, 0), (int)self->mpEngine, 1);
            break;
        case 21:
            obj = cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(1780, str_cFEStateCreateStep1, 0, 0), (int)self->mpEngine, 0);
            break;
        case 22:
            obj = cFEStateCreateStep2_cFEStateCreateStep2((cFEStateCreateStep2*)((cFEStateNet*)cMemMan_alloc(1760, str_cFEStateCreateStep2, 0, 0)), (int)self->mpEngine);
            break;
        case 19:
        default:
            obj = cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateNetSSXWorld, 0, 0), (int)self->mpEngine);
            break;
        }
        func_001A8A40(((cFEStateNet*)self), obj);
        break;
    default:
        cFEStateNet_onWidgetEvent(((cFEStateNet*)self), pWidget, msg);
        break;
    }
}
#endif

extern "C" void* func_001A8E40(void* self);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0E88__FPv);
#ifdef SKIP_ASM
void* func_001C0E88(void* self)
{
    return func_001A8E40(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0EA8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP259MembershipAgreement[];
extern char str_kT_HELP259MembershipAgreementMessage[];
extern char str_kT_HELP259ScrollAgreement[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001C0EA8(cFEStateOnlineAgreement* self, void* popup)
{
    func_001DA528(popup, 2);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP259MembershipAgreement);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP259MembershipAgreementMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELP259ScrollAgreement), 8);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup);
#ifdef SKIP_ASM
extern void* cFEStateNetQuickPlayPopup_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the quick-match popup state.
 *
 * Runs the cFEStateNet constructor (func_001A8500) with the engine and parent, stores the vtable and mHashName 0x48,
 * and clears mbSearching and the pair, static and list-box widget pointers.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @param pParentState  the parent state, the online main menu (On Tour pParentState)
 * @return @p self
 */
extern "C" void* cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup(cFEStateNetQuickPlayPopup* self, int pEngine, int pParentState)
{
    func_001A8500_3(self, pEngine, pParentState);
    self->mVtbl = cFEStateNetQuickPlayPopup_vtbl;
    self->mHashName = 72;
    self->mbSearching = 0;
    self->mpRankedPair = 0;
    self->mpRacePair = 0;
    self->mpBigAirPair = 0;
    self->mpSuperpipePair = 0;
    self->mpSlopestylePair = 0;
    self->mpStaticRanked = 0;
    self->mpStaticRace = 0;
    self->mpStaticBigAir = 0;
    self->mpStaticSuperpipe = 0;
    self->mpStaticSlopeStyle = 0;
    self->mpLBRanked = 0;
    self->mpLBRace = 0;
    self->mpLBBigAir = 0;
    self->mpLBSuperpipe = 0;
    self->mpLBSlopeStyle = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void MEM_fill(void* dst, int c, int n);
extern char str_233QuickPlayPopup[];
extern char D_005308B8[];

/**
 * Creates the "233QuickPlayPopup" screen; when it exists, plays it and clears the 32 race-option bytes D_005308B8.
 */
extern "C" void cFEStateNetQuickPlayPopup_onCreateScreen(cFEStateNetQuickPlayPopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_233QuickPlayPopup), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        MEM_fill(D_005308B8, 0, 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_onUpdate(cFEStateNet* self);
/**
 * Updates the quick-match popup: while searching, counts miBlinkFrames down, reloading 30 after it reaches 0.
 *
 * @return the cFEStateNet update's result (cFEStateNet_onUpdate; declared void* here)
 */
extern "C" void* cFEStateNetQuickPlayPopup_onUpdate(cFEStateNetQuickPlayPopup* self)
{
    if (self->mbSearching != 0) {
        int blinkFrames = (self->miBlinkFrames)--;
        if (blinkFrames == 0) {
            self->miBlinkFrames = 30;
        }
    }
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_TextOK[];
extern char str_LBRanked[];
extern char str_LBRace[];
extern char str_LBBigAir[];
extern char str_LBSuperpipe[];
extern char str_LBSlopeStyle[];
extern char str_StaticRanked[];
extern char str_StaticRace[];
extern char str_StaticBigAir[];
extern char str_StaticSuperpipe[];
extern char str_StaticSlopeStyle[];
extern char str_RankedPair[];
extern char str_RacePair[];
extern char str_BigAirPair[];
extern char str_SuperpipePair[];
extern char str_SlopestylePair[];
extern char str_TextSearching[];

class cFEVObj_001C1080 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24(int on);
    virtual void v25(int on);
};

/**
 * Binds the quick-match widgets as the screen creates them; every widget is made visible first.
 *
 * "TextOK" becomes mpTextOK; "LBRanked", "LBRace", "LBBigAir", "LBSuperpipe" and "LBSlopeStyle" become yes / no boxes
 * (func_001A9AB8) kept in their mpLB members; the "Static..." labels (setUseStaticColour) and the "...Pair" widgets
 * are kept in their members; "TextSearching" becomes mpTextSearching and is hidden.
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateNetQuickPlayPopup_onWidgetCreate(cFEStateNetQuickPlayPopup* self, cFEVObj_001C1080* obj)
{
    int id = *(int*)((char*)obj + 0x38);
    obj->v09(1);
    if (id == GetHashValue32(str_TextOK)) {
        self->mpTextOK = (cUIText*)obj;
        obj->v25(1);
    } else if (id == GetHashValue32(str_LBRanked)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBRanked = (cUIListBox*)obj;
    } else if (id == GetHashValue32(str_LBRace)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBRace = (cUIListBox*)obj;
    } else if (id == GetHashValue32(str_LBBigAir)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBBigAir = (cUIListBox*)obj;
    } else if (id == GetHashValue32(str_LBSuperpipe)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBSuperpipe = (cUIListBox*)obj;
    } else if (id == GetHashValue32(str_LBSlopeStyle)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBSlopeStyle = (cUIListBox*)obj;
    } else if (id == GetHashValue32(str_StaticRanked)) {
        obj->v25(1);
        self->mpStaticRanked = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_StaticRace)) {
        obj->v25(1);
        self->mpStaticRace = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_StaticBigAir)) {
        obj->v25(1);
        self->mpStaticBigAir = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_StaticSuperpipe)) {
        obj->v25(1);
        self->mpStaticSuperpipe = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_StaticSlopeStyle)) {
        obj->v25(1);
        self->mpStaticSlopeStyle = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_RankedPair)) {
        self->mpRankedPair = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_RacePair)) {
        self->mpRacePair = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_BigAirPair)) {
        self->mpBigAirPair = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_SuperpipePair)) {
        self->mpSuperpipePair = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_SlopestylePair)) {
        self->mpSlopestylePair = (cUIThing*)obj;
    } else if (id == GetHashValue32(str_TextSearching)) {
        self->mpTextSearching = (cUIText*)obj;
        obj->v09(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C13E0);
#ifdef SKIP_ASM
class cFEVText_001C13E0 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char str_Txt_Options[];
extern char str_ps2Square[];

extern "C" void func_001C13E0(cFEStateNetQuickPlayPopup* self, int show)
{
    if (self->mpScreen != 0) {
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Txt_Options));
        if (text != 0) {
            ((cFEVText_001C13E0*)text)->v09(show);
        }
        text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2Square));
        if (text != 0) {
            ((cFEVText_001C13E0*)text)->v09(show);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1488);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP233QuickMatchup[];
extern char str_kT_HELP233QuickMatchupMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001C1488(cFEStateNetQuickPlayPopup* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP233QuickMatchup);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP233QuickMatchupMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    func_001DA510(popup, 2, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(popup, 3, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001C13E0(cFEStateNetQuickPlayPopup* self, int show);
extern "C" void func_001C17F8(cFEStateNetQuickPlayPopup* self, int a1);
extern "C" void func_0025EE20(void* self, void* ids, int a2);
extern "C" void func_0025EED8(void* self);
extern "C" int cUIListBox_getCurrentEntryData(void* a);
// D_00534B34 is a splat label inside cBENetworkInterface_networkOpt
extern int D_00534B34[];
extern char str_kT_233AbortingQuickmatch[];
extern char str_kT_BTNCancel[];


struct sVE_001C1560 {
    short delta;
    short index;
    void (*fn)(void*);
};

class cFEVObj_001C1560 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(void* owner, int msg);
};

/**
 * Handles the quick-match popup: OK starts or aborts the search, back aborts or closes.
 *
 * Event 5 on mpTextOK: while searching, shows "kT_233AbortingQuickmatch", makes OK static and aborts
 * (func_001C13E0(.., 0), func_0025EED8); otherwise clears unk_6F0, sets mbSearching, stores the ranked choice in the
 * network options (committed), sets no AI riders, copies the five boxes into miSearchOptions and starts the search
 * (func_0025EE20 with mCurrentCtrl), shows "TextSearching", relabels OK "kT_BTNCancel", sets miBlinkFrames 30 and
 * locks the options (func_001C17F8(.., 1)). Event 6 aborts the same way while searching, else tells the parent 0x14.
 * Event 7 reaches the cFEStateNet handler only while not searching; the rest always do.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void cFEStateNetQuickPlayPopup_onWidgetEvent(cFEStateNetQuickPlayPopup* self, int pWidget, unsigned int msg)
{
    switch (msg) {
    case 7:
        if (self->mbSearching == 0) {
            cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, 7);
        }
        break;
    case 5:
        if ((int)self->mpTextOK == pWidget) {
            if (self->mbSearching != 0) {
                cUIText_setUnicodeStringByID(self->mpTextSearching, GetHashValue32(str_kT_233AbortingQuickmatch));
                ((cFEVObj_001C1560*)self->mpTextOK)->v08(1);
                func_001C13E0(self, self->mbSearching ^ 1);
                func_0025EED8(LobbyMan_mLobbyMan);
            } else {
                self->unk_6F0 = 0;
                self->mbSearching = 1;
                cFEStateNetQuickPlayPopup* feStateNetQuickPlayPopup = self;
                int* ids = feStateNetQuickPlayPopup->miSearchOptions;
                func_001C13E0(self, 0);
                char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
                D_00534B34[0] = cUIListBox_getCurrentEntryData(self->mpLBRanked) != 0;
                sVE_001C1560* vt = *(sVE_001C1560**)(iface + 0xC);
                vt[1].fn(iface + vt[1].delta);
                cBENewRaceInterface_setNumberAI(cBE_getInterface_Fv(SSXApp->BE, 0), 0);
                feStateNetQuickPlayPopup->miSearchOptions[0] = cUIListBox_getCurrentEntryData(self->mpLBRanked);
                feStateNetQuickPlayPopup->miSearchOptions[1] = cUIListBox_getCurrentEntryData(self->mpLBRace);
                feStateNetQuickPlayPopup->miSearchOptions[2] = cUIListBox_getCurrentEntryData(self->mpLBBigAir);
                feStateNetQuickPlayPopup->miSearchOptions[3] = cUIListBox_getCurrentEntryData(self->mpLBSuperpipe);
                feStateNetQuickPlayPopup->miSearchOptions[4] = cUIListBox_getCurrentEntryData(self->mpLBSlopeStyle);
                func_0025EE20(LobbyMan_mLobbyMan, ids, self->mCurrentCtrl);
                ((cFEVObj_001C1560*)self->mpTextSearching)->v09(1);
                cUIText_setUnicodeStringByID(self->mpTextOK, GetHashValue32(str_kT_BTNCancel));
                self->miBlinkFrames = 0x1E;
                func_001C17F8(self, 1);
            }
        }
        break;
    case 6:
        func_001C13E0(self, self->mbSearching ^ 1);
        if (self->mbSearching != 0) {
            cUIText_setUnicodeStringByID(self->mpTextSearching, GetHashValue32(str_kT_233AbortingQuickmatch));
            ((cFEVObj_001C1560*)self->mpTextOK)->v08(1);
            func_0025EED8(LobbyMan_mLobbyMan);
        } else {
            ((cFEVObj_001C1560*)self->mpParentState)->v17(self, 20);
        }
        break;
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, pWidget, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_onWidgetPair__FPv);
#ifdef SKIP_ASM
/**
 * Pair-widget events on the quick-match popup: none are handled.
 *
 * @return always 0x101
 */
int cFEStateNetQuickPlayPopup_onWidgetPair(void* self)
{
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the quick-match popup: reports event 9 as handled (0x100), any other as not (0x101).
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (On Tour Event)
 * @return 0x100 for event 9, else 0x101
 */
extern "C" int cFEStateNetQuickPlayPopup_onWidgetMenu(cFEStateNetQuickPlayPopup* self, int pMenu, int Event)
{
    return Event != 9 ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C17F8);
#ifdef SKIP_ASM
class cFEVObj_001C17F8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

extern "C" void func_001C17F8(cFEStateNetQuickPlayPopup* self, int on)
{
    ((cFEVObj_001C17F8*)self->mpStaticRanked)->v08(on);
    ((cFEVObj_001C17F8*)self->mpStaticRace)->v08(on);
    ((cFEVObj_001C17F8*)self->mpStaticBigAir)->v08(on);
    ((cFEVObj_001C17F8*)self->mpStaticSuperpipe)->v08(on);
    ((cFEVObj_001C17F8*)self->mpStaticSlopeStyle)->v08(on);
    ((cFEVObj_001C17F8*)self->mpLBRanked)->v08(on);
    ((cFEVObj_001C17F8*)self->mpLBRace)->v08(on);
    ((cFEVObj_001C17F8*)self->mpLBBigAir)->v08(on);
    ((cFEVObj_001C17F8*)self->mpLBSuperpipe)->v08(on);
    ((cFEVObj_001C17F8*)self->mpLBSlopeStyle)->v08(on);
    ((cFEVObj_001C17F8*)self->mpRankedPair)->v08(on);
    ((cFEVObj_001C17F8*)self->mpRacePair)->v08(on);
    ((cFEVObj_001C17F8*)self->mpBigAirPair)->v08(on);
    ((cFEVObj_001C17F8*)self->mpSuperpipePair)->v08(on);
    ((cFEVObj_001C17F8*)self->mpSlopestylePair)->v08(on);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C19C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001AB178(cFEStateNet* self, int err);
extern "C" int func_0039F698(void* p);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern const char str_cFEStateOnlineCharacterSelect[];
extern char str_kT_BTNOK[];
extern char str_kT_POPUP233QuickMatchTimeout[];

struct cFEObj_001C19C8 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(void* a1, int a2);
};

// Message handler: on 0xE7 confirms (if armed) or closes the screen; other ids as in the sibling handlers.
extern "C" void* func_001C19C8(cFEStateNetQuickPlayPopup* self, int a1, int a2)
{
    switch (a1) {
    case 230:
        self->unk_6F0 = 1;
        break;
    case 268: case 274:
        func_0039F698((char*)self->mpEngine + 0x18);
        func_001AB178((cFEStateNet*)self, a1);
        break;
    case 231:
        if (self->unk_6F0 != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 1;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)p + 0xC4) = GetHashValue32(str_kT_POPUP233QuickMatchTimeout);
            *(int*)((char*)p + 0xC0) = 3;
            *(int*)((char*)p + 0x150) = 2095;
            *(int*)((char*)p + 0x14C) = 6;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            self->mbSearching = 0;
            ((cFEObj_001C19C8*)self->mpParentState)->v17(self, 20);
        }
        break;
    case 243:
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(1764, str_cFEStateOnlineCharacterSelect, 0x100, 0), (int)self->mpEngine));
        break;
    default:
        return func_001A8E40_3(self, a1, a2);
    case 239:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001C1B88(cFEStateNetQuickPlayPopup* self, void* a1, int a2);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

/**
 * Hands a popup's event on the quick-match popup to func_001C1B88 for popup id 0x82F, else to the cFEStateNet
 * handler.
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 * @return the handler's result (On Tour's onPopupEvent returns void)
 */
extern "C" int cFEStateNetQuickPlayPopup_onPopupEvent(cFEStateNetQuickPlayPopup* self, void* pPopup, int Event)
{
    int result;
    if (*(int*)((char*)pPopup + 0x18) == 2095) {
        result = func_001C1B88(self, pPopup, Event);
    } else {
        result = func_001A97B8(self, pPopup, Event);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C1B88);
#ifdef SKIP_ASM
class cFEVText_001C1B88 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
};

int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C13E0(cFEStateNetQuickPlayPopup* self, int show);
extern "C" void func_001C17F8(cFEStateNetQuickPlayPopup* self, int a1);
extern char str_kT_BTNOK[];

extern "C" int func_001C1B88(cFEStateNetQuickPlayPopup* self, void* a1, int a2)
{
    int r;
    if (a2 == 0xF || a2 == 20) {
        self->mbSearching = 0;
        ((cFEVText_001C1B88*)self->mpTextSearching)->v09(0);
        func_001C13E0(self, self->mbSearching ^ 1);
        cUIText_setUnicodeStringByID(self->mpTextOK, GetHashValue32(str_kT_BTNOK));
        func_001C17F8(self, 0);
        r = func_001A97B8(self, a1, a2);
    } else {
        r = func_001A97B8(self, a1, a2);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetSSXWorld_cFEStateNetSSXWorld);
#ifdef SKIP_ASM
extern void* cFEStateNetSSXWorld_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the "SSX World" online hub state: cFEStateNet constructor with no parent, mHashName 0x4A, its vtable.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mHashName = 74;
    self->mVtbl = cFEStateNetSSXWorld_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetSSXWorld_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A9930(cFEStateNet* self);
extern char str_234SSXWorld[];

/**
 * Creates and plays the "234SSXWorld" screen; opens the disconnected popup (func_001A9930) when LobbyMan's mState is
 * 0.
 */
extern "C" void cFEStateNetSSXWorld_onCreateScreen(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_234SSXWorld), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    if (LobbyMan_mLobbyMan->mState == 0) {
        func_001A9930(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetSSXWorld_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the SSX World hub: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateNetSSXWorld_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1D30);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP234SSXWorld[];
extern char str_kT_HELP234SSXWorldMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnSelectHighlight[];
extern char str_kT_BTNEAMessenger[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001C1D30(cFEStateNet* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP234SSXWorld);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP234SSXWorldMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnSelectHighlight), 0);
    func_001DA510(popup, 2, GetHashValue32(str_kT_BTNEAMessenger), 2);
    func_001DA510(popup, 3, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetSSXWorld_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_TextServerNews[];
extern char str_TextQuickMessages[];
extern char str_TextOnline_Agreement[];
extern char str_TextOptions[];

static inline int IsHash1C1E08(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Numbers the SSX World items as the screen creates them.
 *
 * Ids (+0x18): "TextServerNews" 0, "TextQuickMessages" 1, "TextOnline Agreement" 2, "TextOptions" 3.
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateNetSSXWorld_onWidgetCreate(cFEStateNet* self, void* obj)
{
    if (IsHash1C1E08(*(int*)((char*)obj + 0x38), str_TextServerNews)) {
        *(int*)((char*)obj + 0x18) = 0;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), str_TextQuickMessages)) {
        *(int*)((char*)obj + 0x18) = 1;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), str_TextOnline_Agreement)) {
        *(int*)((char*)obj + 0x18) = 2;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), str_TextOptions)) {
        *(int*)((char*)obj + 0x18) = 3;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetSSXWorld_onWidgetEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cFEStateNetNews_cFEStateNetNews(cFEStateNet* self, int a1);
extern "C" void* cFEStateNetQuickMessages_cFEStateNetQuickMessages(cFEStateNetQuickMessages* self, int a1);
extern "C" void* func_001887A0(void* self, int engine, int reset);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* st, int a2, int a3);
extern const char str_cFEStateNetNews[];
extern const char str_cFEStateNetQuickMessages[];
extern char str_cFEStateOnlineAgreement[];
extern const char str_FEOptions[];

struct sColor_1C1EA8 {
    float r, g, b, a;
};

struct sTextVE_1C1EA8 {
    short delta;
    short index;
    void (*fn)(void*, sColor_1C1EA8*);
};

/**
 * Handles the SSX World hub: opens news, quick messages, the agreement or options, and goes back.
 *
 * A null widget is ignored. Event 5 by id switches (func_001A8A40) to a new cFEStateNetNews (0),
 * cFEStateNetQuickMessages (1) or cFEStateOnlineAgreement with miMiscData 0x13 (2, so it returns here), or (3, unless
 * mAttrState bits 1-2 are set) pushes the "FEOptions" state (func_001887A0). Event 6 switches back to a new
 * cFEStateOnlineMainMenu. Events 1 / 2 recolour the widget white / (1, 0, 0, 0) through its slot 11 (setColour).
 * Event 8 calls func_001A9B58 unless mAttrState bits 1-2 are set; the rest go to the cFEStateNet handler. The
 * parameter names are off: @p msg is the widget and @p ev the event.
 *
 * @param msg  the widget (On Tour pWidget); null is ignored
 * @param ev  the widget event (On Tour Event)
 */
extern "C" void cFEStateNetSSXWorld_onWidgetEvent(cFEStateNet* self, char* msg, int ev)
{
    if (msg == 0) {
        return;
    }
    switch (ev) {
    case 5:
        switch (*(int*)(msg + 0x18)) {
        case 0:
            func_001A8A40(((char*)self), cFEStateNetNews_cFEStateNetNews((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateNetNews, 0, 0), (int)self->mpEngine));
            break;
        case 1:
            func_001A8A40(((char*)self), cFEStateNetQuickMessages_cFEStateNetQuickMessages((cFEStateNetQuickMessages*)((cFEStateNet*)cMemMan_alloc(1788, str_cFEStateNetQuickMessages, 0, 0)), (int)self->mpEngine));
            break;
        case 2: {
            char* agreement = (char*)cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(1756, str_cFEStateOnlineAgreement, 0x100, 0), (int)self->mpEngine);
            *(int*)(agreement + 0x18) = 19;
            func_001A8A40(((char*)self), agreement);
            break;
        }
        case 3:
            if ((self->mAttrState & 6) == 0) {
                char* optionsState = (char*)func_001887A0(cMemMan_alloc(84, str_FEOptions, 0x100, 0), (int)self->mpEngine, 0);
                *(int*)(optionsState + 0x1C) = (*(int*)(optionsState + 0x1C) & ~0x3F00) | 0x100;
                cUIStateStack_pushSpecial((char*)self->mpEngine + 0x18, optionsState, 0, 0);
            }
            break;
        }
        break;
    case 6:
        func_001A8A40(((char*)self), cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), (int)self->mpEngine));
        break;
    case 1: {
        sColor_1C1EA8 color;
        sTextVE_1C1EA8* vt = *(sTextVE_1C1EA8**)(msg + 8);
        char* obj = msg + vt[11].delta;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        vt[11].fn(obj, &color);
        break;
    }
    case 2: {
        sColor_1C1EA8 color;
        sTextVE_1C1EA8* vt = *(sTextVE_1C1EA8**)(msg + 8);
        char* obj = msg + vt[11].delta;
        color.r = 1.0f;
        color.g = 0.0f;
        color.b = 0.0f;
        color.a = 0.0f;
        vt[11].fn(obj, &color);
        break;
    }
    case 8:
        if ((self->mAttrState & 6) == 0) {
            func_001A9B58(self);
        }
        break;
    default:
        cFEStateNet_onWidgetEvent(self, (int)msg, ev);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickMessages_cFEStateNetQuickMessages);
#ifdef SKIP_ASM
extern void* cFEStateNetQuickMessages_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the quick-messages editor state: cFEStateNet constructor with no parent, mHashName 0x4B, its vtable.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateNetQuickMessages_cFEStateNetQuickMessages(cFEStateNetQuickMessages* self, int pEngine)
{
    func_001A8500_3(((cFEStateNet*)self), pEngine, 0);
    self->mHashName = 75;
    self->mVtbl = cFEStateNetQuickMessages_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickMessages_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char str_206QuickMessages[];

/**
 * Creates and plays the "206QuickMessages" screen and clears mbChanged.
 */
extern "C" void cFEStateNetQuickMessages_onCreateScreen(cFEStateNetQuickMessages* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_206QuickMessages), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    self->mbChanged = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickMessages_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the quick-messages editor: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateNetQuickMessages_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C21C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP206QuickMessages[];
extern char str_kT_HELP206QuickMessagesMessage[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELP206EnterMessage[];
extern char str_kT_HELPCmnPrevious[];
extern char str_kT_206ResetMessages[];

extern "C" void func_001C21C0(cFEStateNetQuickMessages* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP206QuickMessages);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP206QuickMessagesMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELP206EnterMessage), 0);
    func_001DA510(popup, 2, GetHashValue32(str_kT_HELPCmnPrevious), 1);
    func_001DA510(popup, 3, GetHashValue32(str_kT_206ResetMessages), 7);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickMessages_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char str_username[];
extern char str_Menu[];
extern char str_messagepct_02d[];
extern char D_004A1390[];

class cFEVObj_001C2298 {
public:
    int field_0x0;
    int field_0x4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline int IsHash1C2298(int id, char* name) { return id == GetHashValue32(name); }

struct sOwner1C2298 {
    char pad[0x6D4];
    cUIText* slots[10];
};

/**
 * Binds the quick-messages widgets as the screen creates them.
 *
 * "username" gets the placeholder " " (D_004A1390) and is made visible; "Menu" gets attribute bit 0; each of
 * "message00".."message09" gets " ", is made visible, gets id i and is kept in mpMessage[i].
 *
 * @param text  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateNetQuickMessages_onWidgetCreate(cFEStateNetQuickMessages* self, cUIText* text)
{
    char buf[32];
    if (IsHash1C2298(*(int*)((char*)text + 0x38), str_username)) {
        cUIText_setAsciiString(text, D_004A1390);
        ((cFEVObj_001C2298*)text)->v09(1);
    } else if (IsHash1C2298(*(int*)((char*)text + 0x38), str_Menu)) {
        *(int*)((char*)text + 0x14) |= 1;
    } else {
        int i;
        for (i = 0; i < 10; i++) {
            sprintf(buf, str_messagepct_02d, i);
            if (IsHash1C2298(*(int*)((char*)text + 0x38), buf)) {
                cUIText_setAsciiString(text, D_004A1390);
                ((cFEVObj_001C2298*)text)->v09(1);
                *(int*)((char*)text + 0x18) = i;
                self->mpMessage[i] = text;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickMessages_onGainTransition);
#ifdef SKIP_ASM
class cFEVObj_001C23C0 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

extern "C" void func_001C2418(cFEStateNetQuickMessages* self);

/**
 * Prepares the quick-messages editor as it opens: calls its slot 32 and, the first time, fills the messages.
 *
 * The fill is func_001C2418.
 *
 * @param bFirstTime  nonzero the first time the state opens (On Tour bFirstTime)
 * @return 0
 */
extern "C" int cFEStateNetQuickMessages_onGainTransition(cFEStateNetQuickMessages* self, int bFirstTime)
{
    ((cFEVObj_001C23C0*)self)->v32();
    if (bFirstTime != 0) {
        func_001C2418(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2418);
#ifdef SKIP_ASM
extern "C" void func_001C2468(cFEStateNetQuickMessages* self, int i);

extern "C" void func_001C2418(cFEStateNetQuickMessages* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        func_001C2468(self, i);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2468);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int i);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_Concat(void* self, const char* str);
extern "C" char* func_0014F6A8(void* self, int i);
extern "C" void USTR_strcpyW(char* dst, void* src);
extern "C" void func_001C2578(cFEStateNetQuickMessages* self, void* text, char* str);
extern char str_Qpct_d[];

struct sBXString1C2468 {
    char* str;
    sBXString1C2468() {}
    sBXString1C2468(const sBXString1C2468& o);
};

struct sVE_001C2468 {
    short delta;
    short index;
    void* (*fn)(void* self, int hash);
};

extern "C" void func_001C2468(cFEStateNetQuickMessages* self, int idx)
{
    char buf[16];
    sBXString1C2468 label;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    sprintf(buf, str_Qpct_d, idx + 1);
    cBXString_cBXString2(&label, buf);
    char* msg = func_0014F6A8(iface, idx);
    if (msg == 0) {
        char wideText[0x40];
        char* locale = (char*)SSXApp->FELocale;
        sVE_001C2468* vt = *(sVE_001C2468**)(locale + 4);
        char* obj = locale + vt[4].delta;
        void* str = vt[4].fn(obj, cBEOptionInterface_getDefaultQuickKeyMessageHashValue(iface, idx));
        USTR_strcpyW(wideText, str);
        cBXString_Concat(&label, wideText);
    } else {
        cBXString_Concat(&label, msg);
    }
    func_001C2578(self, *(void**)((char*)self + (idx << 2) + 0x6D4), label.str);
    cBXString__cBXString(&label, 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2578);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" char* cUIText_getFont(void* text);
extern "C" char* strcat(char*, const char*);
extern char D_004A1BD0[];

struct sVec3_1C2578 {
    float x, y, z;
};

struct sVec2_1C2578 {
    float x, y;
    sVec2_1C2578(float x_, float y_) : x(x_), y(y_) {}
};

struct sRect1C2578 {
    float x, y, w, h;
};

class cFEVObj_001C2578 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20(sRect1C2578* out);
};

static inline void Measure1C2578(cUIText* text, const char* s, sRect1C2578* out)
{
    char* f = cUIText_getFont(text);
    cFont_textExtent(f, s, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
}

extern "C" void func_001C2578(cFEStateNetQuickMessages* self, void* vtext, char* str)
{
    cUIText* text = (cUIText*)vtext;
    int cut = 0;
    sVec3_1C2578 scale = *(sVec3_1C2578*)((char*)text + 0x50);
    sRect1C2578 box;
    ((cFEVObj_001C2578*)text)->v20(&box);
    char* f = cUIText_getFont(text);
    float sx = scale.x;
    float sy = scale.y;
    *(float*)(f + 0x38) = *(float*)(f + 0x30) * sx;
    *(float*)(f + 0x3C) = *(float*)(f + 0x34) * sy;
    int i = strlen(str) - 1;
    char buf[80];
    strcpy(buf, str);
    const char* ell = D_004A1BD0;
    sRect1C2578 sz;
    sRect1C2578 dots;
    Measure1C2578(text, ell, &dots);
    Measure1C2578(text, buf, &sz);
    while (box.x - dots.w < sz.w) {
        cut = 1;
        buf[i--] = 0;
        Measure1C2578(text, buf, &sz);
    }
    if (cut) {
        strcat(buf, ell);
        cUIText_setAsciiString(text, buf);
    } else {
        cUIText_setAsciiString(text, str);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickMessages_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int i);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" char* func_0014F6A8(void* self, int i);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void func_001A9B58(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int a1);
extern "C" void func_001C2A18(cFEStateNetQuickMessages* self);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void USTR_strcpyW(char* dst, void* src);
extern char str_kT_POPUP206EditQuickMessage[];
extern char str_kT_POPUP206SaveQuickMessage[];
extern char str_kT_POPUP206CheckResetMessages[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern const char str_cFEStateNetSSXWorld[];

struct sVE_001C2740 {
    short delta;
    short index;
    void* (*fn)(void* self, int hash);
};

/**
 * Handles the quick-messages editor: edit a message, reset them all, save on back.
 *
 * Event 5 on message row i (< 10) opens the keyboard titled "kT_POPUP206EditQuickMessage" (keyboard id i, 63
 * characters, key 0x4B included) with the player's message i (func_0014F6A8 on the option interface) or, when unset,
 * its localised default. Event 6 asks "kT_POPUP206SaveQuickMessage" (yes / no, default yes, id 0x83A) when mbChanged
 * is set, else switches back to a new cFEStateNetSSXWorld. Event 4 asks "kT_POPUP206CheckResetMessages" (yes / no,
 * default no, id 0x83B). Event 8 calls func_001A9B58 unless mAttrState bits 1-2 are set; event 13 (keyboard done)
 * stores the edit (func_001C2A18); the rest go to the cFEStateNet handler.
 *
 * @param w  the widget (On Tour pWidget)
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cFEStateNetQuickMessages_onWidgetEvent(cFEStateNetQuickMessages* self, void* w, int msg)
{
    switch (msg) {
    case 5: {
        unsigned int idx = *(unsigned int*)((char*)w + 0x18);
        if (idx < 10) {
            void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
            char* name = func_0014F6A8(iface, idx);
            if (name == 0) {
                char msgBuf[0x40];
                char* locale = (char*)SSXApp->FELocale;
                sVE_001C2740* vt = *(sVE_001C2740**)(locale + 4);
                char* obj = locale + vt[4].delta;
                void* str = vt[4].fn(obj, cBEOptionInterface_getDefaultQuickKeyMessageHashValue(iface, idx));
                USTR_strcpyW(msgBuf, str);
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(str_kT_POPUP206EditQuickMessage), (int)msgBuf, 0, idx, 0x3F);
            } else {
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(str_kT_POPUP206EditQuickMessage), (int)name, 0, idx, 0x3F);
            }
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 75, 1, 2);
        }
        break;
    }
    case 6:
        if (self->mbChanged != 0) {
            void* popup = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)popup + 0xBC) = 2;
            char* sub = (char*)popup + 0xBC;
            int hash = GetHashValue32(str_kT_POPUP206SaveQuickMessage);
            *(int*)((char*)popup + 0xC0) = 3;
            *(int*)((char*)popup + 0xC4) = hash;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            *(int*)((char*)popup + 0x168) = 0;
            *(int*)((char*)popup + 0x150) = 2106;
            func_001A9BC0((cFEStateNet*)self, popup);
        } else {
            func_001A8A40(self, cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateNetSSXWorld, 0, 0), (int)self->mpEngine));
        }
        break;
    case 8:
        if (!(self->mAttrState & 6)) {
            func_001A9B58((cFEStateNet*)self);
        }
        break;
    case 13:
        func_001C2A18(self);
        break;
    case 4: {
        void* popup = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)popup + 0xBC) = 2;
        char* sub = (char*)popup + 0xBC;
        int hash = GetHashValue32(str_kT_POPUP206CheckResetMessages);
        *(int*)((char*)popup + 0xC0) = 3;
        *(int*)((char*)popup + 0xC4) = hash;
        func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
        func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
        *(int*)((char*)popup + 0x150) = 2107;
        *(int*)((char*)popup + 0x168) = 1;
        func_001A9BC0((cFEStateNet*)self, popup);
        break;
    }
    default:
        cFEStateNet_onWidgetEvent((cFEStateNet*)self, (int)w, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2A18);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0014F6E8(void* iface, int i);
extern "C" void func_001C2468(cFEStateNetQuickMessages* self, int i);
extern char str_kT_POPUP206EmptyMessages[];
extern char str_kT_BTNOK[];
extern char D_00535617[];

struct sVE1C2A18 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001C2A18(cFEStateNetQuickMessages* self)
{
    char* p = (char*)self->mpKeyboard;
    unsigned int idx = *(unsigned int*)(p + 0x18);
    char* name = p + 0x74;
    if (idx < 10) {
        char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
        if (strlen(name) == 0) {
            void* pop = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)pop + 0xBC;
            *(int*)((char*)pop + 0xBC) = 1;
            *(int*)((char*)pop + 0xC4) = GetHashValue32(str_kT_POPUP206EmptyMessages);
            *(int*)((char*)pop + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_BTNOK));
            *(int*)((char*)pop + 0x150) = 2108;
            func_001A9BC0((cFEStateNet*)self, pop);
            func_0014F6E8(iface, idx);
        } else {
            char* src = (char*)self->mpKeyboard + 0x74;
            strcpy(D_00535617 + (idx << 6), src);
        }
        self->mbChanged = 1;
        sVE1C2A18* vt = *(sVE1C2A18**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        func_001C2468(self, idx);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickMessages_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C2C50(cFEStateNetQuickMessages* self, void* a1, int a2);
extern "C" void func_001C2BA8(cFEStateNetQuickMessages* self, void* a1, int a2);

/**
 * Hands a popup's event in the quick-messages editor to the handler for that popup's id.
 *
 * 0x83A (save) func_001C2C50, 0x83B (reset) func_001C2BA8, any other the cFEStateNet handler (func_001A97B8).
 *
 * @param pPopup  the popup state that sent the event (On Tour pPopup)
 * @param Event  the popup event (On Tour Event)
 */
extern "C" void cFEStateNetQuickMessages_onPopupEvent(cFEStateNetQuickMessages* self, void* pPopup, int Event)
{
    switch (*(int*)((char*)pPopup + 0x18)) {
    case 2106:
        func_001C2C50(self, pPopup, Event);
        break;
    case 2107:
        func_001C2BA8(self, pPopup, Event);
        break;
    default:
        func_001A97B8(self, pPopup, Event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2BA8);
#ifdef SKIP_ASM
class cBEVIface_001C2BA8 {
public:
    int f0;
    int f4;
    int f8;
    virtual void v01();
};

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0014F758(void* self);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C2418(cFEStateNetQuickMessages* self);

extern "C" void func_001C2BA8(cFEStateNetQuickMessages* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        cBEVIface_001C2BA8* iface = (cBEVIface_001C2BA8*)cBE_getInterface_Fv(cBE_getBE(), 4);
        func_0014F758(iface);
        self->mbChanged = 1;
        iface->v01();
        func_001C2418(self);
        func_001A97B8(self, a1, 0xF);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C2C50);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0018D7F0(void* mem, int engine, int a2, int a3, int a4);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int a1);
extern const char str_cFEStateOptionsDeviceSelect[];
extern const char str_cFEStateNetSSXWorld[];

extern "C" void func_001C2C50(cFEStateNetQuickMessages* self, void* popup, int event)
{
    switch (event) {
    case 0xF:
        func_001A97B8(((cFEStateNet*)self), popup, 0xF);
        func_001A8A40(((cFEStateNet*)self), func_0018D7F0(cMemMan_alloc(572, str_cFEStateOptionsDeviceSelect, 0, 0), (int)self->mpEngine, 2, 1, 1));
        break;
    case 0x10:
        func_001A97B8(((cFEStateNet*)self), popup, 0x10);
        func_001A8A40(((cFEStateNet*)self), cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateNetSSXWorld, 0, 0), (int)self->mpEngine));
        break;
    default:
        func_001A97B8(((cFEStateNet*)self), popup, event);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_cFEStateStats);
#ifdef SKIP_ASM
extern void* cFEStateStats_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");


/**
 * Constructs the online stats screen state ("249Stats"): overall mode, first result 0, no menus bound yet.
 *
 * Runs the cFEStateNet constructor (func_001A8500, no parent), stores cFEStateStats_vtbl, mbShowTime 1 and
 * mHashName 0x42, and zeroes miMode, miFocusMenu, miFirstResult and mpMenus[4].
 *
 * @param pEngine  the UI engine (On Tour cUIState ctor pEngine)
 * @return @p self
 */
extern "C" void* cFEStateStats_cFEStateStats(cFEStateStats* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mVtbl = cFEStateStats_vtbl;
    self->mbShowTime = 1;
    self->mHashName = 66;
    cFEStateStats* feStateStats = self;
    feStateStats->miMode = 0;
    feStateStats->miFocusMenu = 0;
    feStateStats->miFirstResult = 0;
    for (int i = 3; i >= 0; i--) {
        ((int*)&feStateStats->mpMenus[0])[i] = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_cFEStateStats1);
#ifdef SKIP_ASM
extern void* cFEStateStats_vtbl[];

extern "C" void* cFEStateStats_cFEStateStats1(cFEStateStats* self)
{
    func_001A8500_3(self, 0, 0);
    self->miMode = 0;
    self->miFocusMenu = (unsigned int)0;
    self->mVtbl = cFEStateStats_vtbl;
    self->miFirstResult = 0;
    for (int i = 0; i < 4; i++)
        *(int*)((char*)self + 0x6D0 + i * 4) = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001C3310(cFEStateStats* self);
extern char str_249Stats[];

/**
 * Opens the "249Stats" screen and, once it exists, fills the result rows (func_001C3310).
 */
extern "C" void cFEStateStats_onCreateScreen(cFEStateStats* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(str_249Stats), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001C3310(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2E70);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

extern "C" void func_001C2E70()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00262768(BuddyMan_mBuddyMan, 1, 5, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Runs one frame of the stats screen: only the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (the pending-event handler's value)
 */
void* cFEStateStats_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIMenu_setInputMap(void* self, int a1);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern char str_EventList[];
extern char str_CourseList[];
extern char str_EventLeftArrow[];
extern char str_rankhl[];
extern char str_EventRightArrow[];
extern char str_CourseLeftArrow[];
extern char str_CourseRightArrow[];
extern char str_sliderbox[];
extern char str_slider[];
extern char str_Txt_Overal[];
extern char str_Txt_Race[];
extern char str_Txt_FreeStyle[];
extern char str_Title[];
extern char str_HeadingsMenu[];
extern char str_EventsMenu[];
extern char str_CoursesMenu[];
extern char str_ResultsMenu[];
extern char str_resultsItem0pct_d[];
extern char str_pointsItem0pct_d[];
extern char str_personaItem0pct_d[];

struct sCol_001C2F00 {
    float r, g, b, a;
    sCol_001C2F00(const float& r_) : r(r_), g(0.0f), b(0.0f), a(0.0f) {}
};

class cFEVObj_001C2F00 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08();
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(const sCol_001C2F00& c);
};

static inline int IsHash_001C2F00(int id, char* name) { return id == GetHashValue32(name); }

/**
 * Sets up the stats screen's widgets as the screen creates them.
 *
 * "EventList" and "CourseList" lose attribute bit 0 and get colour slot 11 with 1.0; the arrows, "rankhl",
 * "sliderbox", "slider" and "Title" are hidden or shown (slot 9: "rankhl", "sliderbox", "slider", "Title" shown,
 * the four arrows hidden). "Txt_Overal", "Txt_Race" and "Txt_FreeStyle" get slot 7(1). "HeadingsMenu",
 * "EventsMenu", "CoursesMenu" and "ResultsMenu" become mpMenus[0..3] (the events and courses menus with bit 3 of
 * +0x90, the results menu scrollable by 10 rows on input map 1). Every "resultsItem0%d", "pointsItem0%d" and
 * "personaItem0%d" row (0-9) is shown.
 *
 * @param obj  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateStats_onWidgetCreate(cFEStateStats* self, cFEVObj_001C2F00* obj)
{
    char* widget = (char*)obj;
    if (IsHash_001C2F00(*(int*)(widget + 0x38), str_EventList) || IsHash_001C2F00(*(int*)(widget + 0x38), str_CourseList)) {
        *(int*)(widget + 0x14) &= ~1;
        obj->v11(sCol_001C2F00(1.0f));
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_EventLeftArrow)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_rankhl)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_EventRightArrow)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_CourseLeftArrow)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_CourseRightArrow)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_sliderbox)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_slider)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_Txt_Overal)) {
        obj->v07(1);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_Txt_Race)) {
        obj->v07(1);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_Txt_FreeStyle)) {
        obj->v07(1);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_Title)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_HeadingsMenu)) {
        *(int*)(widget + 0x14) &= ~1;
        obj->v07(1);
        self->mpMenus[0] = (cUIMenu*)obj;
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_EventsMenu)) {
        self->mpMenus[1] = (cUIMenu*)obj;
        *(int*)(widget + 0x90) |= 8;
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_CoursesMenu)) {
        self->mpMenus[2] = (cUIMenu*)obj;
        *(int*)(widget + 0x90) |= 8;
    } else if (IsHash_001C2F00(*(int*)(widget + 0x38), str_ResultsMenu)) {
        cUIMenu_setScrollableMenu(obj, 10);
        self->mpMenus[3] = (cUIMenu*)obj;
        cUIMenu_setInputMap(obj, 1);
        ((cFEVObj_001C2F00*)self->mpMenus[3])->v07(1);
    } else {
        char buf[32];
        for (int i = 0; i < 10; i++) {
            sprintf(buf, str_resultsItem0pct_d, i);
            if (IsHash_001C2F00(*(int*)(widget + 0x38), buf)) {
                obj->v09(1);
                break;
            }
            sprintf(buf, str_pointsItem0pct_d, i);
            if (IsHash_001C2F00(*(int*)(widget + 0x38), buf)) {
                obj->v09(1);
                break;
            }
            sprintf(buf, str_personaItem0pct_d, i);
            if (IsHash_001C2F00(*(int*)(widget + 0x38), buf)) {
                obj->v09(1);
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onGainTransition);
#ifdef SKIP_ASM
class cFEVObj_001C32B8 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
};

extern "C" void func_001C3EE8(cFEStateStats* self, int a1);

/**
 * Prepares the stats screen when it gains focus: virtual slot 32 (func_001C2E70), then mode 0 the first time.
 *
 * @param bFirstTime  nonzero on the first transition (On Tour bFirstTime); selects the overall mode (func_001C3EE8(self,
 *            0))
 * @return always 1
 */
extern "C" int cFEStateStats_onGainTransition(cFEStateStats* self, int bFirstTime)
{
    ((cFEVObj_001C32B8*)self)->v32();
    if (bFirstTime != 0) {
        func_001C3EE8(self, 0);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C3310);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct cUIText;
extern "C" int func_0025F338(void* self);
extern "C" char* func_0025F388(void* self, int idx, int* a, int* b);
extern "C" void func_001C4F58(cFEStateStats* self, char* buf, int v);
extern char str_personaItem0pct_i[];
extern char str_resultsItem0pct_i[];
extern char str_pointsItem0pct_i[];
extern char D_004A1390_s __asm__("D_004A1390");
extern char str_pct_3d[];

extern "C" void func_001C3310(cFEStateStats* self_)
{
    char name[32];
    char txt[64];
    int v0;
    int v1;
    if (self_->mpScreen != 0) {
        int n = func_0025F338(LobbyMan_mLobbyMan);
        char* obj = (char*)cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(str_ResultsMenu));
        if (obj != 0) {
            if (*(unsigned char*)(obj + 0x96) != n) {
                cUIMenu_setScrollableMenu(obj, n);
                if (n == 0) {
                    cUIMenu_setScrollableMenu(obj, 1);
                }
            }
        }
        int idx = self_->miFirstResult;
        if (n >= 11) {
            if (n - idx <= 10 - 1) {
                idx = n - 10;
            }
        }
        int i;
        int* pb;
        int* pa;
        pa = &v0;
        pb = &v1;
        for (i = 0; i < 10; i++, idx++) {
            sprintf(name, str_personaItem0pct_i, i);
            cUIText* uiText = cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(name));
            sprintf(name, str_resultsItem0pct_i, i);
            cUIText* t1 = cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(name));
            sprintf(name, str_pointsItem0pct_i, i);
            cUIText* t2 = cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(name));
            char* e = func_0025F388(LobbyMan_mLobbyMan, idx, pa, pb);
            if (e != 0 && n > 0) {
                if (t2 != 0) {
                    if (self_->mbShowTime != 0) {
                        func_001C4F58(self_, txt, v0);
                    } else {
                        sprintf(txt, str_pct_d_b, v0);
                    }
                    cUIText_setAsciiString(t2, txt);
                }
                if (uiText != 0) {
                    cUIText_setAsciiString(uiText, e + 8);
                }
                if (t1 != 0) {
                    sprintf(txt, str_pct_3d, *(int*)(e + 4));
                    cUIText_setAsciiString(t1, txt);
                }
            } else {
                cUIText_setAsciiString(t1, &D_004A1390_s);
                cUIText_setAsciiString(t2, &D_004A1390_s);
                cUIText_setAsciiString(uiText, &D_004A1390_s);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C3570);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP249HighScore[];
extern char str_kT_HELP249HighScoreMessages[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_BTNEAMessenger[];
extern char str_kT_HELP249ScrollScore[];
extern char str_kT_HELPCmnPrevious[];

extern "C" void func_001C3570(cFEStateStats* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(str_kT_HELP249HighScore);
    func_001DA530(menu, 0, GetHashValue32(str_kT_HELP249HighScoreMessages));
    func_001DA510(menu, 0, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    func_001DA510(menu, 1, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    func_001DA510(menu, 2, GetHashValue32(str_kT_BTNEAMessenger), 2);
    func_001DA510(menu, 3, GetHashValue32(str_kT_HELP249ScrollScore), 8);
    func_001DA510(menu, 4, GetHashValue32(str_kT_HELPCmnPrevious), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIScreen_getObjectByHashName_3668(void* self, int hash) __asm__("cUIScreen_getObjectByHashName");
extern "C" void cFEStateNet_onWidgetEvent_3668(void* self, void* w, int msg) __asm__("cFEStateNet_onWidgetEvent");
extern "C" void func_001A8A40_3668(void* self, void* obj) __asm__("func_001A8A40");
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu_3668(void* mem, void* a1) __asm__("cFEStateOnlineMainMenu_cFEStateOnlineMainMenu");
extern "C" void func_001C49C8_3668(void* self) __asm__("func_001C49C8");
extern "C" int cUIListBox_getCurrentEntryData_3668(void* w) __asm__("cUIListBox_getCurrentEntryData");
extern "C" void func_001C4B78_3668(void* self, int v) __asm__("func_001C4B78");
extern "C" void func_001C3310_3668(void* self) __asm__("func_001C3310");
extern "C" void func_001C3EE8_3668(void* self, int mode) __asm__("func_001C3EE8");
extern "C" void func_001A9B58_3668(void* self) __asm__("func_001A9B58");
extern "C" void func_003A0290_3668(void* w, void* pos) __asm__("func_003A0290");
extern "C" int func_0025F338_3668(void* p) __asm__("func_0025F338");
extern "C" void func_001C3D68_3668(void* self, int idx, int count) __asm__("func_001C3D68");
extern const char str_cFEStateOnlineMainMenu[];
extern char str_EventList[];
extern char str_CourseList[];
extern char str_EventLeftArrow[];
extern char str_EventRightArrow[];
extern char str_CourseLeftArrow[];
extern char str_CourseRightArrow[];
extern char str_Txt_Overal[];
extern char str_Txt_Race[];
extern char str_Txt_FreeStyle[];
extern char str_resultsItem0pct_d[];
extern char str_event_hl[];
extern char str_course_hl[];
extern char str_rankhl[];

struct cVec_3668 {
    float x, y, z, w;
    cVec_3668(float a, float b, float c, float e) { x = a; y = b; z = c; w = e; }
};

struct cPos_3668 {
    float x, y, z;
};

class cObj_3668 {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void setVisible(int on);
    virtual void v10();
    virtual void setColor(const cVec_3668& v);
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19();
    virtual void getSize(cPos_3668* out);
};

// Same slot-9 setVisible, declared with other return types so the two highlight-off arms are not cross-jumped.
class cObjB_3668 {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual float setVisible(int on);
};

class cObjC_3668 {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual double setVisible(int on);
};

static inline cObj_3668* getObj_3668(char* self, char* name)
{
    return (cObj_3668*)cUIScreen_getObjectByHashName_3668(*(void**)(self + 0x40), GetHashValue32(name));
}

static inline int isButton_3668(cObj_3668* w, char* name)
{
    int id = *(int*)((char*)w + 0x38);
    return id == GetHashValue32(name);
}

/**
 * Handles the stats screen's widget events: the event and course lists, the mode headings and the result rows.
 *
 * Event 6 returns to cFEStateOnlineMainMenu. Event 9 on "EventList" calls func_001C49C8, on "CourseList" picks the
 * course (func_001C4B78 with the selected id) and refills the rows; it then falls through into event 1. Event 1 on
 * "Txt_Overal" / "Txt_Race" / "Txt_FreeStyle" selects mode 0 / 1 / 2 (func_001C3EE8); on "EventList" / "CourseList"
 * whitens the list and shows its arrows and highlight ("event_hl" / "course_hl"); on "resultsItem0%d" row i moves
 * "rankhl" next to the row (x - 40, y - height + 14), shows it and shows result i + miFirstResult (func_001C3D68).
 * Event 2 restores white on the current mode's heading and, on the lists, sets colour (1, 0, 0, 0) and hides the
 * arrows and highlight. Event 8 calls func_001A9B58 unless mAttrState bits 1-2 are set; event 5 is ignored. Events
 * 9, 1, 2 and any other also go to the cFEStateNet handler (cFEStateNet_onWidgetEvent).
 *
 * @param w  the widget (On Tour pWidget); null is ignored
 * @param msg  the widget event (On Tour Event)
 */
// Widget events of the online leaderboard screen: page buttons, row highlight and row selection.
extern "C" void cFEStateStats_onWidgetEvent(cFEStateStats* self, cObj_3668* w, int msg)
{
    cObj_3668* shownObj;
    cObjB_3668* hiddenObj;
    if (w == 0) {
        return;
    }
    switch (msg) {
    case 6:
        func_001A8A40_3668(((char*)self), cFEStateOnlineMainMenu_cFEStateOnlineMainMenu_3668(cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), self->mpEngine));
        return;
    case 9:
        if (isButton_3668(w, str_EventList)) {
            func_001C49C8_3668(((char*)self));
        } else if (isButton_3668(w, str_CourseList)) {
            func_001C4B78_3668(((char*)self), cUIListBox_getCurrentEntryData_3668(w));
            func_001C3310_3668(((char*)self));
        }
    case 1:
        if (self->mpScreen == 0) {
            return;
        }
        if (isButton_3668(w, str_Txt_Overal)) {
            func_001C3EE8_3668(((char*)self), 0);
        } else if (isButton_3668(w, str_Txt_FreeStyle)) {
            func_001C3EE8_3668(((char*)self), 2);
        } else if (isButton_3668(w, str_Txt_Race)) {
            func_001C3EE8_3668(((char*)self), 1);
        } else if (isButton_3668(w, str_EventList)) {
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
            shownObj = getObj_3668(((char*)self), str_EventRightArrow);
            shownObj->setVisible(1);
            shownObj = getObj_3668(((char*)self), str_EventLeftArrow);
            shownObj->setVisible(1);
            shownObj = getObj_3668(((char*)self), str_event_hl);
            shownObj->setVisible(1);
        } else if (isButton_3668(w, str_CourseList)) {
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
            shownObj = getObj_3668(((char*)self), str_CourseRightArrow);
            shownObj->setVisible(1);
            shownObj = getObj_3668(((char*)self), str_CourseLeftArrow);
            shownObj->setVisible(1);
            shownObj = getObj_3668(((char*)self), str_course_hl);
            shownObj->setVisible(1);
        } else {
            char buf[0x20];
            for (int i = 0; i < 10; i++) {
                sprintf(buf, str_resultsItem0pct_d, i);
                if (isButton_3668(w, buf)) {
                    cPos_3668 pos;
                    cPos_3668 size;
                    cObj_3668* rankHighlight = getObj_3668(((char*)self), str_rankhl);
                    func_003A0290_3668(w, &pos);
                    w->getSize(&size);
                    pos.y = pos.y - size.y + 14.0f;
                    pos.x = pos.x + -40.0f;
                    *(cPos_3668*)((char*)rankHighlight + 0x44) = pos;
                    rankHighlight->setVisible(1);
                    func_001C3D68_3668(((char*)self), i + self->miFirstResult, func_0025F338_3668(LobbyMan_mLobbyMan));
                    break;
                }
            }
        }
        break;
    case 2:
        if (isButton_3668(w, str_Txt_Overal)) {
            if (self->miMode != 0) {
                break;
            }
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (isButton_3668(w, str_Txt_FreeStyle)) {
            if (self->miMode != 2) {
                break;
            }
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (isButton_3668(w, str_Txt_Race)) {
            if (self->miMode != 1) {
                break;
            }
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (isButton_3668(w, str_EventList)) {
            w->setColor(cVec_3668(1.0f, 0.0f, 0.0f, 0.0f));
            hiddenObj = (cObjB_3668*)getObj_3668(((char*)self), str_EventRightArrow);
            hiddenObj->setVisible(0);
            hiddenObj = (cObjB_3668*)getObj_3668(((char*)self), str_EventLeftArrow);
            hiddenObj->setVisible(0);
            hiddenObj = (cObjB_3668*)getObj_3668(((char*)self), str_event_hl);
            hiddenObj->setVisible(0);
        } else if (isButton_3668(w, str_CourseList)) {
            w->setColor(cVec_3668(1.0f, 0.0f, 0.0f, 0.0f));
            hiddenObj = (cObjB_3668*)getObj_3668(((char*)self), str_CourseRightArrow);
            ((cObjC_3668*)hiddenObj)->setVisible(0);
            hiddenObj = (cObjB_3668*)getObj_3668(((char*)self), str_CourseLeftArrow);
            ((cObjC_3668*)hiddenObj)->setVisible(0);
            hiddenObj = (cObjB_3668*)getObj_3668(((char*)self), str_course_hl);
            ((cObjC_3668*)hiddenObj)->setVisible(0);
        }
        break;
    case 8:
        if ((self->mAttrState & 6) == 0) {
            func_001A9B58_3668(((char*)self));
        }
        return;
    case 5:
        return;
    default:
        goto other;
    }
    cFEStateNet_onWidgetEvent_3668(((char*)self), w, msg);
    return;
other:
    cFEStateNet_onWidgetEvent_3668(((char*)self), w, msg);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C3D68);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char str_sliderbox[];
extern char str_slider[];

struct sVec3_001C3D68 {
    float x;
    float y;
    float z;
};

struct cUIObj_001C3D68 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a1);
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20(void* out);
};

static inline sVec3_001C3D68 GetPos_001C3D68(cUIObj_001C3D68* o)
{
    return *(sVec3_001C3D68*)((char*)o + 0x44);
}

extern "C" void func_001C3D68(cFEStateStats* self, int pos, int count)
{
    if (self->mpScreen == 0) return;
    if (pos < 0) return;
    if (count < pos) return;
    if (count <= 0) return;
    cUIObj_001C3D68* track = (cUIObj_001C3D68*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_sliderbox));
    cUIObj_001C3D68* thumb = (cUIObj_001C3D68*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_slider));
    if (thumb == 0) return;
    if (track == 0) return;
    sVec3_001C3D68 size;
    track->v20(&size);
    sVec3_001C3D68 p = *(sVec3_001C3D68*)((char*)track + 0x44);
    float fpos = (float)pos;
    if (count >= 2) count--;
    float step = (size.y + -17.0f) / (float)count;
    sVec3_001C3D68 np = *(sVec3_001C3D68*)((char*)thumb + 0x44);
    np.y = p.y + fpos * step + 20.0f;
    *(sVec3_001C3D68*)((char*)thumb + 0x44) = np;
    thumb->v09(1);
    track->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C3EE8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001C4538(cFEStateStats* self, int on);
extern "C" void func_001C47A8(cFEStateStats* self);
extern "C" void func_001C4B78(cFEStateStats* self, int idx);
extern char str_Txt_FreeStyle[];
extern char str_Txt_Overal[];
extern char str_Txt_Race[];
extern char str_Overal_hl[];
extern char str_Race_hl[];
extern char str_Freestyle_hl[];
extern char str_Txt_TimePoints[];
extern char str_kT_OVRCMNPoints[];
extern char str_kT_OVRCMNTime[];

struct sColor_001C3EE8 {
    float r, g, b, a;
    sColor_001C3EE8(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cFEVObj_001C3EE8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
    virtual void v10();
    virtual void setColor(const sColor_001C3EE8& color);
};

static inline cFEVObj_001C3EE8* Find_001C3EE8(void* self, char* name)
{
    return (cFEVObj_001C3EE8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

// Switches the screen between modes 0, 1 and 2: three button highlights, three widgets shown or hidden, the caption text.
extern "C" void func_001C3EE8(cFEStateStats* self, int mode)
{
    int changed = self->miMode != mode;
    self->miMode = mode;
    if (self->mpScreen == 0) {
        return;
    }
    cFEVObj_001C3EE8* widget;
    switch (mode) {
    case 2:
        self->mbShowTime = 0;
        func_001C4538(self, 1);
        widget = Find_001C3EE8(self, str_Txt_FreeStyle);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Txt_Overal);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Txt_Race);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Overal_hl);
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = Find_001C3EE8(self, str_Race_hl);
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = Find_001C3EE8(self, str_Freestyle_hl);
        if (widget != 0) {
            widget->setVisible(1);
        }
        {
            cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Txt_TimePoints));
            if (uiText != 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNPoints));
            }
        }
        if (changed) {
            func_001C47A8(self);
        }
        break;
    case 0:
        self->mbShowTime = 0;
        func_001C4538(self, 0);
        widget = Find_001C3EE8(self, str_Txt_Overal);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Txt_FreeStyle);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Txt_Race);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Overal_hl);
        if (widget != 0) {
            widget->setVisible(1);
        }
        widget = Find_001C3EE8(self, str_Race_hl);
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = Find_001C3EE8(self, str_Freestyle_hl);
        if (widget != 0) {
            widget->setVisible(0);
        }
        {
            cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Txt_TimePoints));
            if (uiText != 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNPoints));
            }
        }
        func_001C4B78(self, 0);
        break;
    case 1:
        self->mbShowTime = 1;
        func_001C4538(self, 1);
        widget = Find_001C3EE8(self, str_Txt_Race);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Txt_Overal);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Txt_FreeStyle);
        if (widget != 0) {
            widget->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        widget = Find_001C3EE8(self, str_Overal_hl);
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = Find_001C3EE8(self, str_Race_hl);
        if (widget != 0) {
            widget->setVisible(1);
        }
        widget = Find_001C3EE8(self, str_Freestyle_hl);
        if (widget != 0) {
            widget->setVisible(0);
        }
        {
            cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Txt_TimePoints));
            if (uiText != 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNTime));
            }
        }
        if (changed) {
            func_001C47A8(self);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4538);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char str_Txt_event[];
extern char str_EventsMenu[];
extern char str_EventList[];
extern char str_txt_Course[];
extern char str_CoursesMenu[];
extern char str_CourseList[];
extern char str_coursebox[];
extern char str_eventbox[];
extern char str_event_hl[];
extern char str_course_hl[];

class cFEVObj_001C4538 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};

static inline void Show_001C4538(void* self, char* name, int on)
{
    cFEVObj_001C4538* o = (cFEVObj_001C4538*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
    if (o != 0) {
        o->v09(on);
    }
}

extern "C" void func_001C4538(cFEStateStats* self, int on)
{
    if (self->mpScreen == 0) {
        return;
    }
    Show_001C4538(((cFEStateNet*)self), str_Txt_event, on);
    Show_001C4538(((cFEStateNet*)self), str_EventsMenu, on);
    Show_001C4538(((cFEStateNet*)self), str_EventList, on);
    Show_001C4538(((cFEStateNet*)self), str_txt_Course, on);
    Show_001C4538(((cFEStateNet*)self), str_CoursesMenu, on);
    Show_001C4538(((cFEStateNet*)self), str_CourseList, on);
    Show_001C4538(((cFEStateNet*)self), str_coursebox, on);
    Show_001C4538(((cFEStateNet*)self), str_eventbox, on);
    if (on == 0) {
        Show_001C4538(((cFEStateNet*)self), str_event_hl, 0);
        Show_001C4538(((cFEStateNet*)self), str_course_hl, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C47A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" const char* func_001453B8(void* iface, int id);
extern "C" void func_001C49C8(cFEStateStats* self);
extern "C" void func_0039A670(void* self);
extern char str_EventList[];
extern char str_kT_CMNBackCountry[];
extern int D_0046DB98[];
extern int D_0046DB88[];

class cFEVObj_001C47A8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
};

extern "C" void func_001C47A8(cFEStateStats* self)
{
    cFEVObj_001C47A8* box = (cFEVObj_001C47A8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EventList));
    if (box == 0) {
        return;
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    switch (self->miMode) {
    case 0:
        func_0039A670(box);
        box->v08(1);
        break;
    case 1:
        func_0039A670(box);
        box->v08(0);
        for (int i = 0; i < 2; i++) {
            int id = D_0046DB98[i];
            if (id == 4) {
                cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNBackCountry), 4);
            } else {
                cUIListBox_addEntryByAsciiString(box, func_001453B8(iface, id), id);
            }
        }
        func_001C49C8(self);
        break;
    case 2:
        func_0039A670(box);
        box->v08(0);
        for (int j = 0; j < 4; j++) {
            int id = D_0046DB88[j];
            if (id == 4) {
                cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_CMNBackCountry), 4);
            } else {
                cUIListBox_addEntryByAsciiString(box, func_001453B8(iface, id), id);
            }
        }
        func_001C49C8(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C49C8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" const char* cBENewRaceInterface_getLocationName(void* iface, int id);
extern "C" void func_001C3310(cFEStateStats* self);
extern "C" void func_001C4B78(cFEStateStats* self, int idx);
extern "C" void func_0039A670(void* self);
extern "C" int cUIListBox_getCurrentEntryData(void* a);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern char str_EventList[];
extern char str_CourseList[];
extern char str_ResultsMenu[];

struct sEntry1C49C8 {
    int data;
    int name;
    int key;
    int mode;
};
extern sEntry1C49C8 D_0046D9B8[];

extern "C" void func_001C49C8(cFEStateStats* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EventList));
    if (box == 0) {
        return;
    }
    int key = cUIListBox_getCurrentEntryData(box);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_CourseList));
    if (box == 0) {
        return;
    }
    func_0039A670(box);
    int found = 0;
    sEntry1C49C8 first;
    int i;
    for (i = 0; i < 29; i++) {
        sEntry1C49C8 e = D_0046D9B8[i];
        if (e.key == key && e.mode == self->miMode) {
            cUIListBox_addEntryByAsciiString(box, cBENewRaceInterface_getLocationName(iface, e.name), e.data);
            if (!found) {
                first = e;
                found = 1;
            }
        }
    }
    if (first.data >= 0) {
        if (first.data < 29) {
            func_001C4B78(self, first.data);
            cUIListBox_setCurrentEntryByIndex(box, 0);
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ResultsMenu)), 0);
        }
    }
    func_001C3310(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4B78);
#ifdef SKIP_ASM
extern "C" void func_0025E348(void* self, int slot);

extern "C" void func_001C4B78(cFEStateStats* self, int slot)
{
    self->miFirstResult = 0;
    func_0025E348(LobbyMan_mLobbyMan, slot);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void func_001C3310(cFEStateStats* self);

/**
 * Handles a menu event on the stats screen: scrolling the results refills the rows.
 *
 * Event 4 stores @p index in miFirstResult and refills the rows (func_001C3310); event 6 is refused.
 *
 * @param pMenu  the menu (On Tour pMenu); not used
 * @param Event  the menu event (On Tour Event)
 * @param index  the menu's new index (On Tour index): the first result row shown
 * @return 0x100 for event 6, otherwise 0x101
 */
extern "C" int cFEStateStats_onWidgetMenu(cFEStateStats* self, void* pMenu, int Event, int index)
{
    switch (Event) {
    case 6:
        return 0x100;
    case 4:
        self->miFirstResult = index;
        func_001C3310(self);
        break;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onInputAlways);
#ifdef SKIP_ASM
// PORT: cUIState_onInputAlways__FPv is called with (self, pad) here; bind the 2-arg form to that symbol.
void cUIState_onInputAlways_2(void* self, void* pad) __asm__("cUIState_onInputAlways__FPv");
extern "C" void func_001C4D28(cFEStateStats* self);
extern "C" void func_001C4E40(cFEStateStats* self);

struct sVE1C4BD8 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int Pad1C4BD8(char* pad, int slot)
{
    sVE1C4BD8* vt = *(sVE1C4BD8**)(pad + 0x8);
    return vt[slot].fn(pad + vt[slot].delta);
}

/**
 * Handles raw input on the stats screen: up and down move the focus between its menus.
 *
 * After the cUIState handler, when any of the input's slots 17-20 (isUp, isDown, isLeft, isRight in On Tour's
 * cUIInput order) is set and miFocusMenu < 3, up calls func_001C4E40 and down func_001C4D28; left and right are
 * tested but do nothing.
 *
 * @param pInput  the UI input (On Tour cUIInput* pInput)
 */
extern "C" void cFEStateStats_onInputAlways(cFEStateStats* self, char* pInput)
{
    cUIState_onInputAlways_2(self, pInput);
    if ((Pad1C4BD8(pInput, 17) || Pad1C4BD8(pInput, 18) || Pad1C4BD8(pInput, 20) || Pad1C4BD8(pInput, 19)) &&
        self->miFocusMenu < 3) {
        if (Pad1C4BD8(pInput, 17)) {
            func_001C4E40(self);
        } else if (Pad1C4BD8(pInput, 18)) {
            func_001C4D28(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4CD8);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" void* func_001C4CD8(cFEStateStats* self, int msg)
{
    if (msg == 225) {
        func_001C3310(self);
        cUIMenu_setSelectedByIndex(self->mpMenus[3], 0);
        return 0;
    }
    return func_001A8E40(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4D28);
#ifdef SKIP_ASM
extern char gUISoundNull;

struct sVE1C4D28 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Show1C4D28(char* o, int on)
{
    sVE1C4D28* vt = *(sVE1C4D28**)(o + 0x8);
    vt[7].fn(o + vt[7].delta, on);
}

extern "C" void func_001C4D28(cFEStateStats* self)
{
    char* snd = *(char**)((char*)self->mpEngine + 0x14);
    if (snd == 0) snd = &gUISoundNull;
    if (snd != 0) {
        sVE1C4D28* vt = *(sVE1C4D28**)snd;
        vt[2].fn(snd + vt[2].delta, 1);
    }
    if (self->miFocusMenu != 3) {
        Show1C4D28(*(char**)((char*)self + (self->miFocusMenu << 2) + 0x6D0), 0);
    }
    self->miFocusMenu += 1;
    if (self->miFocusMenu >= 4) {
        self->miFocusMenu = 0;
    }
    if (self->miFocusMenu == 3) {
        Show1C4D28((char*)self->mpMenus[3], 1);
        func_001C4D28(self);
    } else {
        char* item = *(char**)((char*)self + (self->miFocusMenu << 2) + 0x6D0);
        if ((*(int*)(item + 0x14) >> 6) & 1) {
            Show1C4D28(item, 1);
        } else {
            func_001C4D28(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4E40);
#ifdef SKIP_ASM
extern char gUISoundNull;

struct sVE1C4E40 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Show1C4E40(char* o, int on)
{
    sVE1C4E40* vt = *(sVE1C4E40**)(o + 0x8);
    vt[7].fn(o + vt[7].delta, on);
}

extern "C" void func_001C4E40(cFEStateStats* self)
{
    char* snd = *(char**)((char*)self->mpEngine + 0x14);
    if (snd == 0) snd = &gUISoundNull;
    if (snd != 0) {
        sVE1C4E40* vt = *(sVE1C4E40**)snd;
        vt[2].fn(snd + vt[2].delta, 1);
    }
    if (self->miFocusMenu != 3) {
        Show1C4E40(*(char**)((char*)self + (self->miFocusMenu << 2) + 0x6D0), 0);
    }
    self->miFocusMenu -= 1;
    if (self->miFocusMenu >= 4) {
        self->miFocusMenu = 3;
    }
    if (self->miFocusMenu == 3) {
        Show1C4E40((char*)self->mpMenus[3], 1);
        func_001C4E40(self);
    } else {
        char* item = *(char**)((char*)self + (self->miFocusMenu << 2) + 0x6D0);
        if ((*(int*)(item + 0x14) >> 6) & 1) {
            Show1C4E40(item, 1);
        } else {
            func_001C4E40(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C4F58);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_02d_pct_02d_pct_02d_pct_02d[];
extern char str_pct_02d_pct_02d_pct_02d[];

// Formats a frame count (60 fps) as hours:minutes:seconds.milliseconds, dropping the hours when zero.
extern "C" void func_001C4F58(cFEStateStats* self, char* buf, int frames)
{
    float seconds = frames * (1.0f / 60.0f);
    int ms = (int)((seconds - (int)seconds) * 1000.0f);
    int secs = (int)seconds;
    int mins = secs / 60;
    int hours = mins / 60;
    secs -= mins * 60;
    mins -= hours * 60;
    if (hours > 0) {
        sprintf(buf, str_pct_02d_pct_02d_pct_02d_pct_02d, hours, mins, secs, ms);
    } else {
        sprintf(buf, str_pct_02d_pct_02d_pct_02d, mins, secs, ms);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_cFEStateNetNews);
#ifdef SKIP_ASM
extern void* cFEStateNetNews_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs the server-news state: cFEStateNet constructor with no parent, mHashName 0x43 and its vtable.
 *
 * 0x43 is also cFEStateOnlineAgreement's mHashName.
 *
 * @param pEngine  the UI engine (On Tour cUIState's pEngine)
 * @return @p self
 */
extern "C" void* cFEStateNetNews_cFEStateNetNews(cFEStateNet* self, int pEngine)
{
    func_001A8500_3(self, pEngine, 0);
    self->mHashName = 67;
    self->mVtbl = cFEStateNetNews_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_cFEStateNetNews1);
#ifdef SKIP_ASM
extern void* cFEStateNetNews_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateNetNews_cFEStateNetNews1(cFEStateNet* self)
{
    func_001A8500_3(self, 0, 0);
    self->mVtbl = cFEStateNetNews_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0025E0C8(void* self, int a1);
extern char str_252News[];

/**
 * Creates the "252News" screen; when it exists, plays it and asks LobbyMan for the news (func_0025E0C8(.., 1)).
 */
extern "C" void cFEStateNetNews_onCreateScreen(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_252News), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_0025E0C8(LobbyMan_mLobbyMan, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5108);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262768(void* self, int a1, int a2, int a3, int a4, int a5);

extern "C" void func_001C5108()
{
    if (BuddyMan_mBuddyMan == 0) {
        func_00260F80();
    }
    if (func_00261460(BuddyMan_mBuddyMan) == 0) {
        func_002613C8(BuddyMan_mBuddyMan);
    }
    if (BuddyMan_mBuddyMan == 0) {
        func_0025B800(LobbyMan_mLobbyMan);
    }
    func_00262768(BuddyMan_mBuddyMan, 1, 4, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the server-news state: just the cFEStateNet update (cFEStateNet_onUpdate).
 *
 * @return cFEStateNet_onUpdate()'s result (declared void* here)
 */
void* cFEStateNetNews_onUpdate(void* self)
{
    return cFEStateNet_onUpdate((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern char str_NewsSlider[];
extern char str_ChatUpArrow[];
extern char str_ChatDownArrow[];
extern char str_slideroutline2[];
extern char str_TextScroll_News[];
extern char str_top_arrow[];
extern char str_bot_arrow[];

class cFEVObj_001C5198 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08();
    virtual void v09(int on);
};

/**
 * Wires the news scroll bar to its text the first time the screen opens.
 *
 * "NewsSlider" gets the arrows "ChatUpArrow" / "ChatDownArrow", the highlight names "top arrow" / "bot arrow" (+0x8C
 * / +0xA5, copied when at most 24 characters) and the outline "slideroutline2"; "TextScroll_News" gets the slider
 * (+0xB8) and focus. The slider is made visible only when found, although it was already written to before that
 * check.
 *
 * @param on  nonzero the first time the state opens (On Tour bFirstTime)
 * @return 0
 */
extern "C" int cFEStateNetNews_onGainTransition(cFEStateNet* self, int on)
{
    if (on) {
        char* scrollBar = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_NewsSlider));
        cUIText* uiText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatUpArrow));
        cUIText* downArrow = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatDownArrow));
        cUIText* hideOnEmpty = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_slideroutline2));
        *(cUIText**)(scrollBar + 0x7C) = uiText;
        *(cUIText**)(scrollBar + 0x80) = downArrow;
        if ((unsigned int)strlen(str_top_arrow) > 24) {
            scrollBar[0x8C] = 0;
        } else {
            strcpy(scrollBar + 0x8C, str_top_arrow);
        }
        if ((unsigned int)strlen(str_bot_arrow) > 24) {
            scrollBar[0xA5] = 0;
        } else {
            strcpy(scrollBar + 0xA5, str_bot_arrow);
        }
        *(cUIText**)(scrollBar + 0x84) = hideOnEmpty;
        char* x = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextScroll_News));
        *(char**)(x + 0xB8) = scrollBar;
        ((cFEVObj_001C5198*)x)->v07(1);
        if (scrollBar) {
            ((cFEVObj_001C5198*)scrollBar)->v09(1);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_onWidgetCreate);
#ifdef SKIP_ASM
extern char str_NewsMenu[];
int GetHashValue32(char* str);

class cFEVObj_001C5310 {
public:
    int field_0x0;
    int field_0x4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int a1);
};

/**
 * Gives "NewsMenu" the focus (slot 7, setFocus(1)) as the screen creates it.
 *
 * @param item  the widget being created (On Tour pWidget)
 */
extern "C" void cFEStateNetNews_onWidgetCreate(cFEStateNet* self, cFEVObj_001C5310* item)
{
    int hash = *(int*)((char*)item + 0x38);
    if (hash == GetHashValue32(str_NewsMenu)) {
        item->v07(1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cFEStateNet_onWidgetEvent(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9B58(cFEStateNet* self);
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int a1);
extern const char str_cFEStateNetSSXWorld[];

/**
 * Handles the news screen: back returns to a new cFEStateNetSSXWorld; select (5) is ignored.
 *
 * A null widget is ignored. Event 8 calls func_001A9B58 unless mAttrState bits 1-2 are set; the rest go to the
 * cFEStateNet handler.
 *
 * @param pWidget  the widget, as an int (On Tour pWidget); null is ignored
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cFEStateNetNews_onWidgetEvent(cFEStateNet* self, int pWidget, unsigned int msg)
{
    if (pWidget != 0) {
        switch (msg) {
        case 8:
            if ((self->mAttrState & 6) == 0) {
                func_001A9B58(self);
            }
            break;
        case 6:
            func_001A8A40(self, cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(1744, str_cFEStateNetSSXWorld, 0, 0), (int)self->mpEngine));
            break;
        case 5:
            break;
        default:
            cFEStateNet_onWidgetEvent(self, pWidget, msg);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu events on the news screen: event 4 (scrolled) returns 0, any other 0x101.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (On Tour Event)
 * @return 0 for event 4, else 0x101
 */
extern "C" int cFEStateNetNews_onWidgetMenu(cFEStateNet* self, void* pMenu, int Event)
{
    return Event == 4 ? 0 : 257;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5430);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
struct cBXString;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
int cBXString_FindLastOf(cBXString* self, char ch, int len);
extern "C" void cBXString_setAt(cBXString* self, int idx, char ch);
extern "C" void cUITextScroll_addAsciiString(cUIText* text, char* str);
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern char str_TextScroll_News[];

struct sBXString1C5430 {
    char* str;
    sBXString1C5430() {}
    sBXString1C5430(const sBXString1C5430& o);
};

// PORT: a2 is a message-data pointer passed as int
extern "C" void* func_001C5430(cFEStateNet* self, int msg, int data)
{
    if (msg == 226) {
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextScroll_News));
        if (text != 0) {
            sBXString1C5430 newsStr;
            cBXString_cBXString1(&newsStr, (char*)data + 8);
            int pos;
            while ((pos = cBXString_FindLastOf((cBXString*)&newsStr, '\n', 0)) != -1) {
                int next = pos + 1;
                if (newsStr.str[next] == '\n') {
                    cBXString_setAt((cBXString*)&newsStr, pos, '\r');
                    cBXString_setAt((cBXString*)&newsStr, next, '\r');
                } else {
                    cBXString_setAt((cBXString*)&newsStr, pos, ' ');
                }
            }
            cUITextScroll_addAsciiString(text, newsStr.str);
            cBXString__cBXString(&newsStr, 2);
            return 0;
        }
    } else {
        return func_001A8E40_3(self, msg, data);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5540);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char str_kT_HELP252ServerNews[];
extern char str_kT_HELP252ServerNewsMessage[];
extern char str_kT_HELP252ScrollNews[];
extern char str_kT_HELPCmnPrevious[];
extern char str_kT_BTNEAMessenger[];

extern "C" void func_001C5540(cFEStateNet* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELP252ServerNews);
    func_001DA530(popup, 0, GetHashValue32(str_kT_HELP252ServerNewsMessage));
    func_001DA510(popup, 0, GetHashValue32(str_kT_HELP252ScrollNews), 3);
    func_001DA510(popup, 1, GetHashValue32(str_kT_HELPCmnPrevious), 1);
    func_001DA510(popup, 2, GetHashValue32(str_kT_BTNEAMessenger), 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_cPopupInfo);
#ifdef SKIP_ASM
extern "C" void cPopupInfo_clear(cPopupInfo* self);
extern void* cPopupInfo_vtbl[];

/**
 * Constructs a popup description: stores cPopupInfo_vtbl and clears every field with cPopupInfo_clear().
 *
 * @return @p self
 */
extern "C" void* cPopupInfo_cPopupInfo(cPopupInfo* self)
{
    self->mVtbl = cPopupInfo_vtbl;
    cPopupInfo_clear(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_clear);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

struct sVec3_1C5630 {
    int x, y, z;
};

extern sVec3_1C5630 t3vect_Zero;

/**
 * Resets a popup description to a plain message popup with no texts, options or confirmations.
 *
 * Zeroes the message, title, entry-box title and initial text, the five option names and confirmations and the
 * post-pending text (memset); sets mbPassUpdate 0, mbReceivePrev 1, mbCenterText 1, miMiscData 0, mbCenterOptions
 * 0, mfMinSize 0, meType 0 (no buttons), miLayerOffset 0, mvOffset zero, miDefaultOption -1, meBackgroundType 0
 * (online) and mbDeferEventsFlag 0. miMaxChars is left as it was.
 */
extern "C" void cPopupInfo_clear(cPopupInfo* self)
{
    char* confirmEntry = (char*)self + 0x54;
    char* optionName = (char*)self + 0x24;
    func_00416210((char*)self + 0x4, 0, 8);
    func_00416210((char*)self + 0xC, 0, 8);
    func_00416210((char*)self + 0x14, 0, 8);
    func_00416210((char*)self + 0x1C, 0, 8);
    int i;
    for (i = 4; i >= 0; i--) {
        func_00416210(optionName, 0, 8);
        optionName += 8;
        func_00416210(confirmEntry, 0, 0xC);
        confirmEntry += 0xC;
    }
    func_00416210((char*)self + 0x4C, 0, 8);
    self->mbPassUpdate = 0;
    self->mbReceivePrev = 1;
    self->mbCenterText = 1;
    self->miMiscData = 0;
    self->mbCenterOptions = 0;
    *(int*)&self->mfMinSize = 0;
    self->meType = 0;
    self->miLayerOffset = 0;
    *(sVec3_1C5630*)&self->mvOffset[0] = t3vect_Zero;
    self->miDefaultOption = -1;
    self->meBackgroundType = 0;
    self->mbDeferEventsFlag = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo__cPopupInfo);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cPopupInfo_vtbl[];

/**
 * Destroys a popup description: restores cPopupInfo_vtbl and, if asked, frees the object.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPopupInfo__cPopupInfo(cPopupInfo* self, int flags)
{
    self->mVtbl = cPopupInfo_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5780__FPvii);
#ifdef SKIP_ASM
struct cFEAsyncSlot {
    int mState; // 0x0
    int mValue; // 0x4
};

struct cFEAsyncSlots {
    char pad_0x00[0x24];
    cFEAsyncSlot mSlots[1]; // 0x24
};

void func_001C5780(void* self, int index, int value)
{
    cFEAsyncSlots* slots = (cFEAsyncSlots*)self;
    slots->mSlots[index].mValue = value;
    slots->mSlots[index].mState = 2;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C57A0__FPvii);
#ifdef SKIP_ASM
void func_001C57A0(void* self, int a1, int a2)
{
    cFEAsyncSlots* s = (cFEAsyncSlots*)self;
    s->mSlots[a1].mValue = a2;
    s->mSlots[a1].mState = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C57C0__FPvii);
#ifdef SKIP_ASM
void func_001C57C0(void* self, int a1, int a2)
{
    cFEAsyncSlots* s = (cFEAsyncSlots*)self;
    s->mSlots[a1].mValue = a2;
    s->mSlots[a1].mState = 3;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C57E0);
#ifdef SKIP_ASM
extern "C" void func_001C57E0(void* self, int i, void* src)
{
    cFEAsyncSlots* slots = (cFEAsyncSlots*)self;
    slots->mSlots[i] = *(cFEAsyncSlot*)src;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_getPopupOptionNames);
#ifdef SKIP_ASM
/**
 * Returns option @p iEntryNum's name element (mtOptionNames[i], at +0x24 + 8 * i).
 *
 * @param iEntryNum  the option index, 0-4
 * @return the option's tPopupElement (declared void* here)
 */
extern "C" void* cPopupInfo_getPopupOptionNames(cPopupInfo* self, int iEntryNum)
{
    return (char*)self + ((iEntryNum << 3) + 0x24);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5810);
#ifdef SKIP_ASM
struct sFEAsyncReq_1C5810 {
    int mState; // 0x0
    int mValue; // 0x4
    int mExtra; // 0x8
};

struct sFEAsyncReqs_1C5810 {
    char pad_0x00[0x54];
    sFEAsyncReq_1C5810 mReqs[1]; // 0x54
};

extern "C" void func_001C5810(void* self, int i, int value, int extra)
{
    sFEAsyncReqs_1C5810* reqs = (sFEAsyncReqs_1C5810*)self;
    reqs->mReqs[i].mValue = value;
    reqs->mReqs[i].mState = 2;
    reqs->mReqs[i].mExtra = extra;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5838);
#ifdef SKIP_ASM
struct cFEAsyncReq {
    int mState; // 0x0
    int mValue; // 0x4
    int mExtra; // 0x8
};

struct cFEAsyncReqs {
    char pad_0x00[0x54];
    cFEAsyncReq mReqs[1]; // 0x54
};

extern "C" void func_001C5838(void* self, int index, int value, int extra)
{
    cFEAsyncReqs* reqs = (cFEAsyncReqs*)self;
    reqs->mReqs[index].mValue = value;
    reqs->mReqs[index].mState = 1;
    reqs->mReqs[index].mExtra = extra;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_clearPopupConfirmation);
#ifdef SKIP_ASM
/**
 * Removes option @p iEntryNum's confirmation question: sets mtConfirmation[i].eType to 0 (none).
 *
 * @param iEntryNum  the option index, 0-4
 */
extern "C" void cPopupInfo_clearPopupConfirmation(cPopupInfo* self, int iEntryNum)
{
    char* entry = (char*)self + iEntryNum * 0xc;
    *(int*)(entry + 0x54) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5878);
#ifdef SKIP_ASM
extern "C" void func_001C5878(void* self, int index, int value, int extra)
{
    cFEAsyncReqs* reqs = (cFEAsyncReqs*)self;
    reqs->mReqs[index].mValue = value;
    reqs->mReqs[index].mState = 3;
    reqs->mReqs[index].mExtra = extra;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C58A0);
#ifdef SKIP_ASM
extern "C" void func_001C58A0(void* self, int i, void* src)
{
    cFEAsyncReqs* reqs = (cFEAsyncReqs*)self;
    reqs->mReqs[i] = *(cFEAsyncReq*)src;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_getPopupConfirmation);
#ifdef SKIP_ASM
/**
 * Returns option @p iEntryNum's confirmation element (mtConfirmation[i], at +0x54 + 0xC * i).
 *
 * @param iEntryNum  the option index, 0-4
 * @return the option's tConfirmElement (declared void* here)
 */
extern "C" void* cPopupInfo_getPopupConfirmation(cPopupInfo* self, int iEntryNum)
{
    return (char*)self + (iEntryNum * 0xc + 0x54);
}
#endif

