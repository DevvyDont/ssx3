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
extern char D_004A19F8[];
extern char D_004A1A00[];

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
    sprintf(files->mFiles[slot].mName, D_004A19F8, D_004A19F0, D_004A1A00);
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
extern char D_004A19F8[];
extern char D_004A1A00[];

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
    sprintf(files->mFiles[slot].mName, D_004A19F8, D_00441258[pic], D_004A1A00);
    files->mFiles[slot].mParam = useAsync;
    cFEAsyncManager_SetFileStatus(self, slot, 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEAsyncManager_LoadLocationPic);
#ifdef SKIP_ASM
extern char D_004A1A08[];
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
        sprintf(files->mFiles[slot].mName, D_004A1A08, D_004A1A10, cBEMissionInterface_getPictureFileName(iface, id), D_004A1A00);
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
    sFEUnloadFiles* m = (sFEUnloadFiles*)self;
    if (cFEAsyncManager_GetFileStatus(self, idx) == 1) {
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    } else if (cFEAsyncManager_GetFileStatus(self, idx) == 2) {
        ASYNCFILE_release(m->files[idx].handle, 0, 0);
        if (m->files[idx].data != 0) {
            cMemMan_free(m->files[idx].data);
        }
        m->files[idx].data = 0;
        cFEAsyncManager_SetFileStatus(self, idx, 0);
    } else if (cFEAsyncManager_GetFileStatus(self, idx) == 3) {
        cPS2GraphicsMan* obj = GrMan;
        sVEUnload* vt = (sVEUnload*)obj->mVtbl;
        m->files[idx].f118 = vt[114].fn((char*)obj + vt[114].delta) + 0xE;
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
extern char D_004A1A18[];

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
    sFE39F0Files* m = (sFE39F0Files*)self;
    int i;
    for (i = 0; i < 5; i++) {
        if (cFEAsyncManager_GetFileStatus(self, i) == 0 || cFEAsyncManager_GetFileStatus(self, i) == 1) {
            cPS2GraphicsMan* obj = GrMan;
            sVE39F0* vt = (sVE39F0*)obj->mVtbl;
            if (m->files[i].f118 < vt[114].fn((char*)obj + vt[114].delta)) {
                if (m->files[i].data != 0) {
                    cPS2GraphicsMan* obj2 = GrMan;
                    sVE39F0V* vt2 = (sVE39F0V*)obj2->mVtbl;
                    vt2[50].fn((char*)obj2 + vt2[50].delta, m->files[i].id);
                    if (m->files[i].data != 0) {
                        cMemMan_free(m->files[i].data);
                    }
                    m->files[i].data = 0;
                    m->files[i].f118 = 0;
                    m->files[i].id = -1;
                }
            }
        }
        if (cFEAsyncManager_GetFileStatus(self, i) == 1) {
            cPS2GraphicsMan* obj = GrMan;
            sVE39F0* vt = (sVE39F0*)obj->mVtbl;
            if (m->files[i].f118 < vt[114].fn((char*)obj + vt[114].delta)) {
                cFEAsyncManager_LoadDataFile(self, i);
            }
        }
        if (cFEAsyncManager_GetFileStatus(self, i) == 2) {
            if (ASYNCFILE_getstatus(m->files[i].handle) == 1) {
                cFEAsyncManager_SetFileStatus(self, i, 3);
                ASYNCFILE_release(m->files[i].handle, 0, 0);
                cPS2GraphicsMan* obj = GrMan;
                sVE39F0L* vt = (sVE39F0L*)obj->mVtbl;
                char* data = m->files[i].data;
                m->files[i].id = vt[46].fn((char*)obj + vt[46].delta, data + *(int*)(data + 0x14), D_004A1A18, 0, 0, -1);
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

extern "C" void cFEAsyncManager_FlushFEAsyncTextures(cFEAsyncManager* self)
{
    sFE3C50Files* m = (sFE3C50Files*)self;
    int i;
    for (i = 0; i < 5; i++) {
        if (m->files[i].data != 0) {
            cPS2GraphicsMan* obj = GrMan;
            sVE3C50* vt = (sVE3C50*)obj->mVtbl;
            vt[50].fn((char*)obj + vt[50].delta, m->files[i].id);
            if (m->files[i].data != 0) {
                cMemMan_free(m->files[i].data);
            }
            m->files[i].data = 0;
            m->files[i].f118 = 0;
            m->files[i].id = -1;
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

extern char D_00461430[];
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int func_003DF748(void* file, void* data, int size);
extern "C" void func_003E1BB8(void* file, void* data, int size);

/**
 * Starts reading a slot's file into a new buffer of a fixed 0x109A0 bytes (tag "FE Async Texture").
 *
 * Sets the status to 2 (loading). With the slot's async flag (+0x108) it starts an asynchronous read
 * (func_003DF748) and keeps the handle at +0x104; otherwise it reads synchronously (func_003E1BB8) and sets
 * status 3 directly, skipping the texture step that cFEAsyncManager_Update() runs only for asynchronous
 * loads.
 *
 * @param idx  index of the file slot (On Tour fileNum)
 */
extern "C" void cFEAsyncManager_LoadDataFile(cFEAsyncManager* self, int idx)
{
    sFEAsyncFiles* m = (sFEAsyncFiles*)self;
    void** pData;
    char* base;
    m->files[idx].size = 0x109A0;
    base = (char*)self + 0x100;
    pData = (void**)(base + idx * 0x11C);
    *pData = operator_new_tag(0x109A0, D_00461430, 0x25000000, 0);
    cFEAsyncManager_SetFileStatus(self, idx, 2);
    if (m->files[idx].async != 0) {
        cFEAsyncManager_SetFileStatus(self, idx, 2);
        m->files[idx].handle = func_003DF748(&m->files[idx], *pData, m->files[idx].size);
    } else {
        func_003E1BB8(&m->files[idx], *pData, m->files[idx].size);
        cFEAsyncManager_SetFileStatus(self, idx, 3);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3DC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");
extern void* D_00467F28[];
extern void* D_004A3E90_v __asm__("D_004A3E90");

// Constructor for the memory-card screen: base init, vtable and empty-string fields.
extern "C" void* func_001A3DC8(cFEStateNetMessenger* self, void* a1, void* a2)
{
    // An unused 16-byte local is needed to reproduce the 0x30-byte frame.
    int unused[4];
    func_001A8500_3(self, (int)a1, (int)a2);
    void* empty = D_004A3E90_v;
    self->mVtbl = D_00467F28;
    self->mszFoundName = (char*)empty;
    self->mszSearchName = (char*)empty;
    self->unk_6E0 = (char*)empty;
    self->miSelectedIndex = -1;
    self->mHashName = 0x46;
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
INCLUDE_ASM("fe/feasyncfile", func_001A3EA0);
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



// Destructor for the memory-card screen.
extern "C" void func_001A3EA0(cFEStateNetMessenger* self, int flags)
{
    self->mVtbl = (void**)D_00467F28;
    char* child = (char*)self->mpParentState;
    sVE1A3EA0* vt = *(sVE1A3EA0**)(child + 0x8);
    vt[32].fn(child + vt[32].delta);
    func_001A5450(self);
    tFEStringPtrVector* v = &(*(&self->mNames));
    Dealloc1A3EA0(((void**)v->mpStart), (void**)self->mNames.mpEndOfStorage - (void**)self->mNames.mpStart);
    cBXString__cBXString(((char*)&self->unk_6E0), 2);
    cBXString__cBXString(((char*)&self->mszSearchName), 2);
    cBXString__cBXString(((char*)&self->mszFoundName), 2);
    func_001A85D0_dtor3EA0((self), flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3F68);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A5898(cFEStateNetMessenger* self);
extern "C" void func_001A6978(cFEStateNetMessenger* self);
extern char D_00461448[];

extern "C" void func_001A3F68(cFEStateNetMessenger* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_00461448), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001A5898(self);
        func_001A6978(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A3FE0);
#ifdef SKIP_ASM
extern void* D_004A5B64;
extern "C" void func_001A6978(cFEStateNetMessenger* self);
extern "C" void* func_001A8770(cFEStateNet* self);

extern "C" void func_001A3FE0(cFEStateNetMessenger* self)
{
    int now = *(int*)((char*)D_004A5B64 + 0x18);
    int d = now - self->mLastRefreshFrame;
    if (d >= 1 && d <= 59) {
        func_001A8770((cFEStateNet*)self);
    } else {
        self->mLastRefreshFrame = now;
        func_001A6978(self);
        func_001A8770((cFEStateNet*)self);
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
extern char D_00461458[];
extern char D_00461470[];
extern char D_00461488[];
extern char D_004614A0[];
extern char D_0045FE60[];
extern char D_004614B8[];
extern char D_004614D0[];
extern char D_004614E8[];
extern char D_00461500[];

extern "C" void func_001A4040(cFEStateNetMessenger* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00461458);
    func_001DA530(menu, 0, GetHashValue32(D_00461470));
    func_001DA530(menu, 1, GetHashValue32(D_00461488));
    func_001DA530(menu, 2, GetHashValue32(D_004614A0));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_004614B8), 0);
    func_001DA510(menu, 2, GetHashValue32(D_004614D0), 2);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 4, GetHashValue32(D_00461500), 6);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4170);
#ifdef SKIP_ASM
struct sVE1A4170 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001A6C40(cFEStateNetMessenger* self, int a1);

extern "C" int func_001A4170(cFEStateNetMessenger* self, void* obj)
{
    int r;
    sVE1A4170* vt = *(sVE1A4170**)((char*)obj + 0x8);
    if (vt[27].fn((char*)obj + vt[27].delta) == 0) {
        r = 0;
    } else {
        func_001A6C40(self, self->miSelectedIndex);
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A41C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_00461458[];
extern char D_00461520[];
extern char D_00461530[];
extern char D_00461540[];
extern char D_00461558[];
extern char D_00461570[];
extern char D_00461580[];
extern char D_00461590[];

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

extern "C" void func_001A41C0(cFEStateNetMessenger* self, cUIText* text)
{
    cFEVObj_001A41C0* obj = (cFEVObj_001A41C0*)text;
    if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461520)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00461458));
        obj->v09(1);
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461530)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00461540));
        obj->v09(1);
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461558)) {
        obj->v09(1);
        *(int*)((char*)text + 0x14) |= 0x80;
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461570)) {
        obj->v09(0);
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00461580));
    } else if (IsHash1A41C0(*(int*)((char*)text + 0x38), D_00461590)) {
        *(int*)((char*)text + 0x14) |= 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4300);
#ifdef SKIP_ASM
extern "C" void func_001A6978(cFEStateNetMessenger* self);

extern "C" int func_001A4300(cFEStateNetMessenger* self, int a1, int a2, int a3)
{
    if (a2 == 4) {
        self->miScrollOffset = a3;
        func_001A6978(self);
        return 0;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4330);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
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
extern char D_00461540[];
extern char D_004615A0[];
extern char D_004615B0[];

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

// Message handler for the slot-list screen: picks a slot by its button hash, or highlights it.
extern "C" void func_001A4330(cFEStateNetMessenger* self, sObj_4330* w, int msg)
{
    char buf[0x20];
    switch (msg) {
    case 6: {
        sObj_4330* o = (sObj_4330*)self->mpParentState;
        o->vt[17].fn((char*)o + o->vt[17].delta, ((char*)self), 0x14);
        break;
    }
    case 8:
        func_001A9150((cFEStateNet*)self, 0, GetHashValue32(D_00461540), (int)D_004A1408, 0, 1, 0x10);
        *(int*)((char*)self->mpKeyboard + 0x438) = 1;
        *(int*)((char*)self->mpKeyboard + 0x58) = 1;
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
        break;
    case 5: {
        int idx = *(int*)((char*)w + 0x18);
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
            sprintf(buf, D_004615A0, i);
            int id = *(int*)((char*)w + 0x38);
            if (id == GetHashValue32(buf)) {
                func_001A6070_4330(((char*)self), i + self->miScrollOffset);
                func_001A5FB8_4330(((char*)self), i);
                func_001A5128_4330(((char*)self), i + self->miScrollOffset, (int*)self->mNames.mpFinish - (int*)self->mNames.mpStart);
                func_001A66A8_4330(((char*)self), i, 1);
                break;
            }
        }
        func_001A8918((cFEStateNet*)self, (int)w, msg);
        break;
    case 2: {
        for (int i = 0; i < 15; i++) {
            sprintf(buf, D_004615A0, i);
            int id = *(int*)((char*)w + 0x38);
            if (id == GetHashValue32(buf)) {
                sprintf(buf, D_004615B0, i);
                sObj_4330* o = (sObj_4330*)cUIScreen_getObjectByHashName_4330(self->mpScreen, GetHashValue32(buf));
                if (o != 0) {
                    o->vt[9].fn((char*)o + o->vt[9].delta, 0);
                }
                ((cObjV_4330*)w)->set(cVec_4330(1.0f, 0.0f, 0.0f, 0.0f));
                func_001A66A8_4330(((char*)self), i, 0);
                break;
            }
        }
        func_001A8918((cFEStateNet*)self, (int)w, msg);
        break;
    }
    default:
        func_001A8918((cFEStateNet*)self, (int)w, msg);
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
extern "C" int func_001A4740(cFEStateNetMessenger* self, int a1)
{
    return self->miScrollOffset + a1;
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
    char* p = (char*)self->mpKeyboard;
    switch (*(int*)(p + 0x18)) {
    case 1:
        func_001A4810(self, p + 0x74);
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
extern char D_004615F8[];
extern char D_00461618[];
extern char D_00461628[];

extern "C" void func_001A4810(cFEStateNetMessenger* self, void* a1)
{
    const char* name = (const char*)a1;
    if (strlen(name) < 3) {
        void* p = func_001A9C50((cFEStateNet*)self);
        int three = 3;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
        *(int*)((char*)p + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x14C) = three;
        *(int*)((char*)p + 0x150) = 0x806;
        func_001A9BC0((cFEStateNet*)self, p);
    } else {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00461628);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x807;
        *(int*)((char*)p + 0x14C) = 3;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, p);
        cBXString_cBXString4((char*)self + 0x6D4, name);
        func_0025E6D0(LobbyMan_mLobbyMan, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4928);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);
extern "C" void func_001A9890(cFEStateNet* self, const char* str);
int func_001A98B0(void* self);
extern "C" void func_001AB370(cFEStateNet* self, int a1, int a2);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001A4928(cFEStateNetMessenger* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        if (func_0041AA88(((const char*)LobbyMan_mLobbyMan->mPersona), (const char*)self->mszFoundName) != 0) {
            func_001A9890((cFEStateNet*)self, (const char*)self->mszFoundName);
            func_001AB370((cFEStateNet*)self, func_001A98B0(self), 0);
        }
    } else {
        func_001A97B8(self, a1, a2);
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

extern "C" void func_001A49A0(cFEStateNetMessenger* self, void* a1, unsigned int kind)
{
    int r17;
    int r18;
    switch (kind) {
    case 15:
        func_001ABCD0((cFEStateNet*)self, func_001A98B0(self), a1, 15, 1);
        break;
    case 16:
        func_001A97B8(self, a1, 16);
        func_001ABB30((cFEStateNet*)self, func_001A98B0(self), a1, 16);
        break;
    case 17:
        func_001A4B20_ABE58(self, func_001A98B0(self), &r17);
        if (r17 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, a1, 17);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    case 18:
        func_001A4B20_ABC50(self, func_001A98B0(self), &r18);
        if (r18 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, a1, 18);
        }
        func_001A5898(self);
        break;
    default:
        func_001A97B8(self, a1, kind);
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

extern "C" void func_001A4B20(cFEStateNetMessenger* self, void* a1, unsigned int kind)
{
    int r16;
    int r17;
    switch (kind) {
    case 15:
        func_001A97B8(self, a1, 15);
        func_001ABB30((cFEStateNet*)self, func_001A98B0(self), a1, 15);
        break;
    case 16:
        func_001A4B20_ABE58(self, func_001A98B0(self), &r16);
        if (r16 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, a1, 16);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    case 17:
        func_001A4B20_ABC50(self, func_001A98B0(self), &r17);
        if (r17 == 1) {
            func_001ABED8(self);
        } else {
            func_001A97B8(self, a1, 17);
        }
        func_001A5898(self);
        func_001A6978(self);
        break;
    default:
        func_001A97B8(self, a1, kind);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A4C58);
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

extern "C" void func_001A4C58(cFEStateNetMessenger* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x809:
        ((cFEVObj_001A4C58*)self)->v33(a1, a2);
        break;
    case 0x80B:
        ((cFEVObj_001A4C58*)self)->v34(a1, a2);
        break;
    case 0x80C:
        ((cFEVObj_001A4C58*)self)->v35(a1, a2);
        break;
    default:
        func_001A97B8(((cFEVObj_001A4C58*)self), a1, a2);
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
extern char D_00461648[];

extern "C" void func_001A4CE8(cFEStateNetMessenger* self, void* src)
{
    cBXString_operatorE((char*)self + 0x6D0, (char*)src + 0x8);
    self->unk_6D8 = *(int*)((char*)src + 0x10);
    void* p = func_001A9C50((cFEStateNet*)self);
    int three = 3;
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xCC) = GetHashValue32(D_00461648);
    *(int*)((char*)p + 0xC8) = three;
    func_001C5780(sub, 0, (int)self->mszFoundName);
    *(int*)((char*)p + 0x14C) = three;
    *(int*)((char*)p + 0x150) = 0x809;
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
extern char D_00461660[];
extern char D_00461618[];

extern "C" void func_001A4D90(cFEStateNetMessenger* self)
{
    void* p = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xCC) = GetHashValue32(D_00461660);
    *(int*)((char*)p + 0xC8) = 3;
    func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x14C) = 3;
    *(int*)((char*)p + 0x150) = 0x808;
    func_001A9BC0((cFEStateNet*)self, p);
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
extern char D_004615F8[];
extern char D_00461618[];
extern char D_00461678[];
extern char D_00461690[];

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
        sprintf1(buf, vt[4].fn(obj + vt[4].delta, GetHashValue32(D_00461678)), self->mszSearchName);
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = one;
        char* sub = (char*)p + 0xBC;
        int h = GetHashValue32(D_00461690);
        *(char**)((char*)p + 0xC4) = buf;
        *(int*)((char*)p + 0xC0) = one;
        *(int*)((char*)p + 0xC8) = three;
        *(int*)((char*)p + 0xCC) = h;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x14C) = three;
        *(int*)((char*)p + 0x150) = 0x80A;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    case 0x696E616D: {
        void* p = func_001A9C50((cFEStateNet*)self);
        int three = 3;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
        *(int*)((char*)p + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x14C) = three;
        *(int*)((char*)p + 0x150) = 0x806;
        func_001A9BC0((cFEStateNet*)self, p);
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
extern "C" void func_001A4FB0(cFEStateNetMessenger* self, int a1, int a2)
{
    if (self->mAttrState & 6) {
        if (*(int*)((char*)self->mpPopup + 0x18) == 0x807) {
            func_001A4E28(self, (void*)a2);
        } else {
            func_001A8E40_3(((cFEStateNet*)self), a1, a2);
        }
    } else {
        func_001A8E40_3(((cFEStateNet*)self), a1, a2);
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
    case 0x107:
        func_001A5898(self);
    case 0xEA: case 0xEC: case 0xED: case 0xEF: case 0xF1: case 0x106:
    case 0x109: case 0x10A: case 0x10B: case 0x112: case 0x113:
        if (self->unk_6AC != 0) {
            break;
        }
        func_001A54D8(self);
        func_001A52B0(self);
        func_001A6978(self);
        func_001AB3D0(((cFEStateNet*)self), func_001A98B0(((cFEStateNet*)self)), 0);
        break;
    case 0x10E:
        func_001A4D90(self);
        break;
    case 0xE3:
        func_001A4CE8(self, (void*)a2);
        break;
    case 0x102:
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
extern char D_00461778[];
extern char D_00461788[];

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
    cUIObj_001A5128* track = (cUIObj_001A5128*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461778));
    cUIObj_001A5128* thumb = (cUIObj_001A5128*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461788));
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
        sBXString1A52B0 s;
        cBXString_cBXString1(&s, name);
        self->miSelectedIndex = func_001A5800(self, &s.str);
        cBXString__cBXString(&s, 2);
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
        void* p = (*(sPtrVec1A5450*)&self->mNames.mpStart).mEnd[-1];
        (*(sPtrVec1A5450*)&self->mNames.mpStart).mEnd--;
        if (p) {
            cBXString__cBXString(p, 3);
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
extern char D_00461798[];

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
    tFEStringPtrVector* v = &(*(&feStateNetMessenger->mNames));
    if (((sElem1A54D8*)(*(&feStateNetMessenger->mNames)).mpFinish) != ((sElem1A54D8*)(*(&feStateNetMessenger->mNames)).mpEndOfStorage)) {
        Construct1A54D8(((sElem1A54D8*)v->mpFinish), x);
        ++(*(sElem1A54D8**)&(*(&feStateNetMessenger->mNames)).mpFinish);
    } else {
        func_001DB778(v, (void**)((sElem1A54D8*)(*(&feStateNetMessenger->mNames)).mpFinish), &x);
    }
}

extern "C" void func_001A54D8(cFEStateNetMessenger* self)
{
    if (*(int*)((char*)GetSession_001A54D8() + 0xE8) != 0) {
        int i;
        for (i = 0; i < func_00262598(GetSession_001A54D8()); i++) {
            const char* name = func_002625C8(GetSession_001A54D8(), i);
            if (name != 0 && func_00262438(GetSession_001A54D8(), name) != 0 && func_001A56D0(self, name) == 0) {
                void* s = cBXString_cBXString2(cMemMan_alloc(4, D_00461798, 0x20000000, 0), name);
                PushBack1A54D8(self, s);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A56D0);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);


static inline unsigned int Count1A56D0(cFEStateNetMessenger* feStateNetMessenger) { return ((char***)feStateNetMessenger->mNames.mpFinish) - ((char***)feStateNetMessenger->mNames.mpStart); }

extern "C" int func_001A56D0(cFEStateNetMessenger* self, const char* name)
{
    unsigned int i;
    for (i = 0; i < Count1A56D0((self)); i++) {
        if (func_0041AA88(**(char***)((char*)(char***)self->mNames.mpStart + (i << 2)), name) == 0) {
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
        if (func_0041AA88(**(char***)((char*)(char***)self->mNames.mpStart + (i << 2)), name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A5800);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);


static inline unsigned int Count1A5800(cFEStateNetMessenger* feStateNetMessenger) { return ((char***)feStateNetMessenger->mNames.mpFinish) - ((char***)feStateNetMessenger->mNames.mpStart); }

extern "C" int func_001A5800(cFEStateNetMessenger* self, char** name)
{
    unsigned int i;
    for (i = 0; i < Count1A5800((self)); i++) {
        if (func_0041AA88(**(char***)((char*)(char***)self->mNames.mpStart + (i << 2)), *name) == 0) {
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
extern char D_00461798[];
extern char D_00461590[];

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
    tFEStringPtrVector* v = &(*(&feStateNetMessenger->mNames));
    void* const* px = &x;
    if (((sElem1A5898*)(*(&feStateNetMessenger->mNames)).mpFinish) != ((sElem1A5898*)(*(&feStateNetMessenger->mNames)).mpEndOfStorage)) {
        Construct1A5898(((sElem1A5898*)v->mpFinish), x);
        ++(*(sElem1A5898**)&(*(&feStateNetMessenger->mNames)).mpFinish);
    } else {
        func_001DB778(v, (void**)((sElem1A5898*)(*(&feStateNetMessenger->mNames)).mpFinish), px);
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
            PushBack1A5898(self, cBXString_cBXString1(cMemMan_alloc(4, D_00461798, 0x20000000, 0), name));
        }
    }
    int i;
    for (i = 0; i < count; i++) {
        const char* name = func_002625C8(GetSession_001A5898(), i);
        if (name != 0 && name[0xA7] != 0 && func_00263128(GetSession_001A5898(), name) == 0
            && func_00262400(GetSession_001A5898(), i) == 0) {
            PushBack1A5898(self, cBXString_cBXString2(cMemMan_alloc(4, D_00461798, 0x20000000, 0), name));
        }
    }
    for (i = 0; i < count; i++) {
        const char* name = func_002625C8(GetSession_001A5898(), i);
        if (name != 0 && func_00262400(GetSession_001A5898(), i) != 0 && func_00263128(GetSession_001A5898(), name) == 0) {
            PushBack1A5898(self, cBXString_cBXString2(cMemMan_alloc(4, D_00461798, 0x20000000, 0), name));
        }
    }
    for (i = 0; i < count; i++) {
        const char* name = func_002625C8(GetSession_001A5898(), i);
        if (name != 0 && name[0xA7] == 0 && func_00263128(GetSession_001A5898(), name) == 0
            && func_00262400(GetSession_001A5898(), i) == 0) {
            PushBack1A5898(self, cBXString_cBXString2(cMemMan_alloc(4, D_00461798, 0x20000000, 0), name));
        }
    }
    if (((void*)self->mpScreen) != 0) {
        char* list = (char*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(D_00461590));
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

extern char D_004615B0[];
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
        sprintf(buf, D_004615B0, i);
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
extern char D_00461558[];
extern char D_004617A8[];
extern char D_004617B8[];
extern char D_004617C8[];
extern char D_004617E0[];

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
    cFEVObj_001A6070* name = (cFEVObj_001A6070*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004617A8));
    cFEVObj_001A6070* status = (cFEVObj_001A6070*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461558));
    if (status == 0 || name == 0) {
        return 0;
    }
    int n = (int*)self->mNames.mpFinish - (int*)self->mNames.mpStart;
    if (n == 0) {
        cUIText_setAsciiString((cUIText*)name, D_004A1390);
        if (*(int*)((char*)GetSession_001A6070() + 0xE8) != 0) {
            cUIText_setUnicodeStringByID((cUIText*)status, GetHashValue32(D_004617B8));
        } else {
            cUIText_setUnicodeStringByID((cUIText*)status, GetHashValue32(D_004617C8));
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
        cUIText_setUnicodeStringByID((cUIText*)status, GetHashValue32(D_004617E0));
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
extern char D_004617F8[];
extern char D_00461808[];
extern char D_00461818[];

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
    sprintf(name, D_004617F8, index);
    cFEVObj_001A63C8* a = (cFEVObj_001A63C8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    sprintf(name, D_00461808, index);
    cFEVObj_001A63C8* b = (cFEVObj_001A63C8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    sprintf(name, D_00461818, index);
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
extern char D_004615A0[];

extern "C" void func_001A6618(cFEStateNetMessenger* self, unsigned int index, const char* str, int a3)
{
    char buf[32];
    if (self->mpScreen != 0 && index < 0xF) {
        sprintf(buf, D_004615A0, index);
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
extern char D_004615A0[];
extern char D_00461830[];

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
    sprintf(name, D_004615A0, index);
    cFEVObj_001A66A8* persona = (cFEVObj_001A66A8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    sprintf(name, D_00461830, index);
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
extern char D_00461590[];

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
    cUIText* menu = cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(D_00461590));
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
extern char D_00461590[];
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" void func_001A6C40(cFEStateNetMessenger* self, int idx)
{
    if (self->mpScreen != 0 && idx >= 0) {
        void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461590));
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
extern char D_004A1A20[];
extern char D_00461570[];
extern char D_00461840[];
extern char D_00461858[];

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
    cFEVObj_001A6CA0* o = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1A20));
    if (o != 0) {
        o->v09(has);
    }
    o = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461570));
    if (o != 0) {
        o->v09(has);
    }
    cFEVObj_001A6CA0* a = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461840));
    cFEVObj_001A6CA0* b = (cFEVObj_001A6CA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461858));
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

struct sIntVec1A6E70 {
    int f0;
    int* mBegin;
    int* mEnd;
};

static inline int* Copy1A6E70(int* first, int* last, int* result)
{
    func_0041610C(result, first, (last - first) * sizeof(int));
    return result + (last - first);
}


extern "C" void func_001A6E70(cFEStateNetMessenger* self)
{
    int i;
    for (i = ((*(sIntVec1A6E70*)&self->mNames).mEnd - (*(sIntVec1A6E70*)&self->mNames).mBegin) - 1; i >= 0; i--) {
        sIntVec1A6E70* v = &(*(sIntVec1A6E70*)&self->mNames);
        int id = *(int*)((char*)(*(sIntVec1A6E70*)&self->mNames).mBegin + (i << 2));
        if (func_002624F8(GetSession_001A6E70(), id) < 0) {
            // PORT: pointer arithmetic through int
            int* pos = (int*)(i * 4 + (int)(*(sIntVec1A6E70*)&self->mNames).mBegin);
            if (pos + 1 != (*(sIntVec1A6E70*)&self->mNames).mEnd) {
                Copy1A6E70(pos + 1, v->mEnd, pos);
            }
            (*(sIntVec1A6E70*)&self->mNames).mEnd--;
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

// For every save slot that is in use, starts an operation (2) on the files that are neither busy nor present.
extern "C" void func_001A6F70(void)
{
    char buf[0x10];
    int n = func_00262598(GetSession_001A6F70());
    for (int i = 0; i < n; i++) {
        const char* name = func_002625C8(GetSession_001A6F70(), i);
        if (name != 0 && func_00262400(GetSession_001A6F70(), i) != 0) {
            int a = func_00263128(GetSession_001A6F70(), name);
            int b = func_00262A38(GetSession_001A6F70(), (int)name);
            if (a == 0 && b == 0)
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
extern "C" char* func_004162D0(char*, const char*);
extern "C" char* strcpy(char* dst, const char* src);
extern char D_004A1390[];
extern char D_004A1408[];
extern char D_004A1A28[];
extern char D_004A1A30[];
extern char D_004A1A38[];
extern char D_004A1A40[];
extern char D_004A1A48[];
extern char D_00461558[];
extern char D_004617A8[];
extern char D_004617E0[];
extern char D_00461868[];
extern char D_00461880[];
extern char D_00461898[];
extern char D_004618B0[];
extern char D_004618C8[];
extern char D_004618E0[];
extern char D_004618F8[];
extern char D_00461910[];
extern char D_00461930[];
extern char D_00461950[];

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
    unsigned short out[0x50];
    char s1[0x20];
    char s2[0x20];
    char s3[0x20];
    void* screen = self->mpScreen;
    if (screen == 0 || name == 0) {
        return;
    }
    cUIText* t1 = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004617A8));
    cUIText* t2 = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461558));
    if (t2 == 0) {
        return;
    }
    const char* tags = name + 0x54;
    unsigned int state = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(tags, D_004A1A28), -1);
    cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(tags, D_004A1A30), -1);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(tags, D_004A1A38), s1, 0x20, D_004A1408);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(tags, D_004A1A40), s2, 0x20, D_004A1408);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(tags, D_004A1A48), s3, 0x20, D_004A1408);
    if (mode == 0) {
        char buf[0x60];
        unsigned short wide[0x60];
        unsigned short wrapped[0x60];
        float size[4];
        strcpy(buf, name);
        func_004162D0(buf, D_004A1390);
        func_004162D0(buf, tags);
        func_003EFA48(wide, 0x60, buf);
        sVEntryP_001A7390* vt = *(sVEntryP_001A7390**)((char*)t1 + 8);
        vt[20].fn((char*)t1 + vt[20].delta, size);
        sVec3_001A7390 scale = *(sVec3_001A7390*)((char*)t1 + 0x50);
        unsigned short* rest = func_00392680(cUIText_getFont(t1), scale.x, size[0], wide, wrapped, 0x30);
        cUIText_setUnicodeString(t1, (char*)wrapped);
        if (rest != 0) {
            cUIText_setUnicodeString(t2, (char*)rest);
        } else {
            cUIText_setAsciiString(t2, D_004A1390);
        }
        return;
    }
    switch (state) {
    case 1:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(D_004617E0));
        break;
    case 0:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(D_00461868));
        break;
    case 2:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(D_00461880));
        break;
    case 4:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(D_00461898));
        break;
    case 5:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(D_004618B0));
        break;
    case 10:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(D_004618C8));
        break;
    case 6:
        cUIText_setUnicodeStringByID(t2, GetHashValue32(D_004618E0));
        break;
    case 3: {
        unsigned short w1[0x40];
        int str = Loc1A7390(D_004618F8);
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
        sprintf1_1A7390(out, Loc1A7390(D_00461910), s1);
        cUIText_setUnicodeString(t2, (char*)out);
        break;
    case 7:
        sprintf1_1A7390(out, Loc1A7390(D_00461930), s1);
        cUIText_setUnicodeString(t2, (char*)out);
        break;
    case 9:
        sprintf1_1A7390(out, Loc1A7390(D_00461950), s1);
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

extern "C" void* cFEStateReadMessagePopup_cFEStateReadMessagePopup_impl(sScreen1A77A0* self, int a1, int a2, const char* name)
{
    // An unused 16-byte local is needed to reproduce the 0x50-byte frame.
    int unused[4];
    func_001A8500_3(self, a1, a2);
    self->vt = cFEStateReadMessagePopup_vtbl;
    cBXString_cBXString2(self->name, name);
    self->f6E8 = 1;
    self->id = 0x49;
    self->f6D4 = 0;
    self->f6DC = 0;
    self->f6E0 = 0;
    self->f6E4 = 0;
    sColor1A77A0 c;
    c.r = 1.0f;
    c.g = 1.0f;
    c.b = 1.0f;
    c.a = 1.0f;
    self->color = c;
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

extern "C" void cFEStateReadMessagePopup__cFEStateReadMessagePopup(sOwner1A7848* self, int flags)
{
    self->vt = cFEStateReadMessagePopup_vtbl;
    for (;;) {
        if (self->mVec.mBegin == self->mVec.mEnd) break;
        sObj1A7848* p = self->mVec.mEnd[-1];
        self->mVec.mEnd--;
        if (p) {
            p->vt[1].fn((char*)p + p->vt[1].delta, 3);
        }
    }
    sObjVec1A7848* v = &self->mVec;
    sObj1A7848** first = self->mVec.mBegin;
    sObj1A7848** last = self->mVec.mEnd;
    func_0041610C(first, last, (v->mEnd - last) * sizeof(sObj1A7848*));
    self->mVec.mEnd -= last - first;
    Dealloc1A7848(v->mBegin, self->mVec.mCap - self->mVec.mBegin);
    cBXString__cBXString(self->str, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7970);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_003A1310(void* self, int a1);
extern char D_004619A0[];
extern char D_004619B8[];
extern char D_004619C8[];
extern char D_004619D8[];
extern char D_004619E8[];
extern char D_004619F8[];

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

extern "C" void func_001A7970(cFEStateReadMessagePopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004619A0), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    char* a = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619B8));
    *(int*)(a + 0x14) |= 1;
    void* b = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619C8));
    void* c = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619D8));
    void* d = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619E8));
    *(void**)(a + 0x7C) = b;
    *(void**)(a + 0x80) = c;
    *(void**)(a + 0x84) = d;
    *(int*)(a + 0x88) = 2;
    char* o = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619F8));
    if (o != 0) {
        cFEVObj_001A7970* obj = (cFEVObj_001A7970*)o;
        *(int*)(o + 0x74) |= 1;
        func_003A1310(o, 1);
        obj->v09(1);
        *(int*)(o + 0x14) |= 0x80;
        obj->v11(sCol1A7970(1.0f, 0.0f, 0.0f, 0.0f));
        *(sBlk1A7970*)(o + 0xA8) = *(sBlk1A7970*)&self->mTextColour;
        *(char**)(o + 0xB8) = a;
    }
}
#endif

extern "C" void* func_001A8770(cFEStateNet* self);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7B20__FPv);
#ifdef SKIP_ASM
void* func_001A7B20(void* self)
{
    return func_001A8770((cFEStateNet*)self);
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
INCLUDE_ASM("fe/feasyncfile", func_001A7BB0__FPv);
#ifdef SKIP_ASM
void func_001A7BB0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7BB8);
#ifdef SKIP_ASM
extern "C" void func_00260F80();
extern "C" int func_00261460(void* self);
extern "C" void func_002613C8(void* self);
extern "C" void func_0025B800(void* self);
extern "C" void func_00262690(void* self, int a1, void* a2);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern "C" void func_001A81F0(cFEStateReadMessagePopup* self);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
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

extern "C" void func_001A7BB8(cFEStateReadMessagePopup* self, int a1, unsigned int msg)
{
    switch (msg) {
    case 6:
        ((cFEObj_001A7BB8*)self->mpParentState)->v17(self, 0x14);
        break;
    case 5:
        func_001A9090((cFEStateNet*)self, 0, (int)self->mszSender, (int)D_004A1408, 0, 0, 0x3F);
        *(int*)((char*)self->mpKeyboard + 0x43C) = 0;
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
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
        func_001A8918((cFEStateNet*)self, a1, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7D10);
#ifdef SKIP_ASM
extern "C" void func_001A81F0(cFEStateReadMessagePopup* self);

extern "C" int func_001A7D10(cFEStateReadMessagePopup* self, int a1, int a2, int a3)
{
    if (a2 == 4) {
        self->unk_6D4 = a3;
        func_001A81F0(self);
        return 0;
    }
    return 0x101;
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
    if (msg == 0x109) {
        func_001A7F78(self);
        func_001A81F0(self);
        return 0;
    }
    return func_001A8E40(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A7D90);
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

extern "C" int func_001A7D90(cFEStateReadMessagePopup* self)
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

struct sObjVec1A7DD8 {
    int f0;
    sObj1A7DD8** mBegin;
    sObj1A7DD8** mEnd;
    sObj1A7DD8** mCap;
};

extern "C" void func_001DB918(sObjVec1A7DD8* vec, sObj1A7DD8** pos, sObj1A7DD8* const* x);

static inline sObj1A7DD8** Copy1A7DD8(sObj1A7DD8** first, sObj1A7DD8** last, sObj1A7DD8** result)
{
    func_0041610C(result, first, (last - first) * sizeof(sObj1A7DD8*));
    return result + (last - first);
}

struct sOwner1A7DD8 {
    char pad0[0x6D8];
    sObjVec1A7DD8 mVec;
};

extern "C" void func_001A7DD8(cFEStateReadMessagePopup* selfp, void* item)
{
    sOwner1A7DD8* self = (sOwner1A7DD8*)selfp;
    sObj1A7DD8* x = (sObj1A7DD8*)item;
    for (;;) {
        if ((unsigned int)(self->mVec.mEnd - self->mVec.mBegin) < 31) break;
        sObjVec1A7DD8* v = &self->mVec;
        sObj1A7DD8** pos = self->mVec.mBegin;
        sObj1A7DD8* p = *pos;
        if (p) {
            if (pos + 1 != self->mVec.mEnd) {
                Copy1A7DD8(pos + 1, v->mEnd, pos);
            }
            self->mVec.mEnd--;
            p->vt[1].fn((char*)p + p->vt[1].delta, 3);
        }
    }
    sObjVec1A7DD8* v2 = &self->mVec;
    if (self->mVec.mEnd != self->mVec.mCap) {
        sObj1A7DD8** e = v2->mEnd;
        if (e) *e = x;
        self->mVec.mEnd++;
    } else {
        func_001DB918(v2, self->mVec.mEnd, &x);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A7ED8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void func_001A7DD8(cFEStateReadMessagePopup* self, void* item);
extern char D_00461A08[];
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
    func_001A7DD8(self, Init_001A7ED8((char*)cMemMan_alloc(0x10, D_00461A08, 0x20000000, 0),
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
extern "C" int func_0041AA88(const char* a, const char* b);
extern char D_00461A08[];
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
        if (item != 0 && func_0041AA88(*(const char**)item, (const char*)self->mszSender) == 0) {
            func_001A7DD8(self, Init_001A7F78((char*)operator new(0x10, D_00461A08, 0x20000000, 0), item));
        }
    }
    for (int i = func_00262938(GetSession_001A7F78()) - 1; i >= 0; i--) {
        char* item = (char*)func_00263630(GetSession_001A7F78(), i);
        if (item != 0 && func_0041AA88(*(const char**)item, (const char*)self->mszSender) == 0) {
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
extern "C" void func_003A18C0(void* list);
extern "C" void func_003A19F8(cUIText* text, char* str);
extern "C" char* func_004162D0(char*, const char*);
extern "C" char* strcpy(char* dst, const char* src);
extern char D_00461A18[];
extern char D_00461A28[];
extern char D_004619F8[];
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

struct sOwner1A81F0 {
    char pad[0x6DC];
    sEntry1A81F0** mBegin;
    sEntry1A81F0** mEnd;
};

static inline unsigned int Count1A81F0(sOwner1A81F0* s) { return s->mEnd - s->mBegin; }

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
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00461A18));
    if (text != 0 && Count1A81F0((sOwner1A81F0*)self) != 0) {
        sEntry1A81F0* e = ((sOwner1A81F0*)self)->mBegin[0];
        char* who = self->mszSender;
        sprintf1_1A81F0(buf, Loc1A81F0(D_00461A28), who, e->desc, D_004A1408);
        cUIText_setUnicodeString(text, buf);
        ((cFEVObj_001A81F0*)text)->v09(1);
    }
    cUIText* list = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619F8));
    if (list == 0) {
        return;
    }
    unsigned short count = *(unsigned short*)((char*)list + 0xC8);
    unsigned short top = *(unsigned short*)((char*)list + 0xC4);
    unsigned short sel = *(unsigned short*)((char*)list + 0xC6);
    func_003A18C0(list);
    unsigned int i;
    for (i = 0; i < Count1A81F0((sOwner1A81F0*)self); i++) {
        sEntry1A81F0* e = *(sEntry1A81F0**)((char*)((sOwner1A81F0*)self)->mBegin + (i << 2));
        strcpy(buf, e->name);
        func_004162D0(buf, D_004A1A50);
        func_004162D0(buf, e->ext);
        func_003A19F8(list, buf);
    }
    func_001A83D8((sList1A83D8*)list, 0x28, 8, count, top, sel, self->mbResetScroll, 0);
    self->mbResetScroll = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A83D8);
#ifdef SKIP_ASM
extern "C" void func_003A1F90(void* list, int a1);

struct sList1A83D8 {
    char pad[0xC4];
    short top;
    short sel;
    unsigned short count;
};

extern "C" void func_001A83D8(sList1A83D8* list, int keep, unsigned int sel, int a3, int pos, int a5, int a6, int trim)
{
    if (list == 0) return;
    int toEnd = 0;
    if (a3 == pos + 1 || a6 != 0) toEnd = 1;
    int n = list->count - keep;
    int atStart = pos == 0;
    int removed = 0;
    if (n > 0 && trim != 0) {
        for (removed = 0; removed < n; removed++) {
            func_003A1F90(list, 0);
        }
    }
    if (toEnd) {
        unsigned short c = list->count;
        if (sel >= c) {
            list->sel = 0;
        } else {
            list->sel = c - sel;
        }
        list->top = list->count - 1;
    } else if (atStart) {
        list->sel = 0;
        list->top = 0;
    } else {
        int d = pos - removed;
        if (d <= 0) {
            list->top = 0;
            list->sel = 0;
        } else {
            int e = a5 - removed;
            if (e <= 0) {
                list->sel = 0;
            } else {
                list->sel = e;
            }
            list->top = d;
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
extern char D_0045CDE8[];

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
    *(sListNode_8500**)(s + 0x6CC) = (sListNode_8500*)cMemMan_alloc(0xC, D_0045CDE8, 0x20000000, 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001A8770);
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

extern "C" void func_0039E510(void*);
extern "C" void* func_001A8B88(void* self, int size, int a2);

extern "C" void* func_001A8770(cFEStateNet* self)
{
    ((cFEAsync1A8770*)self)->v27();
    ((cFEAsync1A8770*)self)->v28();
    func_0039E510(self);
    return func_001A8B88(self, 0x114, 0);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A87D0);
#ifdef SKIP_ASM
struct sVE1A87D0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void cUIState_onGainFocus(void* self, int a1);

extern "C" void func_001A87D0(cFEStateNet* self, int a1)
{
    sVE1A87D0* vt = (sVE1A87D0*)self->mVtbl;
    vt[32].fn((char*)self + vt[32].delta);
    cUIState_onGainFocus(self, a1);
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
INCLUDE_ASM("fe/feasyncfile", func_001A8918);
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
extern char D_00461A70[];

extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg)
{
    if (a1 != 0 && msg == 7) {
        void* p = func_001D9D48(cMemMan_alloc(0x140, D_00461A70, 0x100, 0), self->mpEngine, self);
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
    if (self->unk_6A8 != 0 && kind != 0x31 && kind != 0x33 && kind != 0x4D && kind != 0x4E
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
extern char D_0045DC60[];
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0039F4C0(void* list, void* item);
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
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_0045DC60));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
        func_0039F4C0(*(char**)((char*)self + 0x10) + 0x18, item);
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
extern char D_0045CDE8[];

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
    sListNode1A8AE8* tmp = (sListNode1A8AE8*)operator new(0xC, D_0045CDE8, 0x20000000, 0);
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
extern "C" void func_00294F78(void* p, int id);
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
    if ((unsigned int)(msg - 0xC8) < 0x4F) {
        if ((unsigned int)(msg - 0x115) < 2 || *(int*)((char*)st + 0x6B4) != 0) {
            if (msg == 0x115 && BuddyMan_mBuddyMan != 0) {
                func_00261058(BuddyMan_mBuddyMan);
                func_00261408(BuddyMan_mBuddyMan);
            }
            if (st->v30() != 0) {
                if ((unsigned int)(msg - 0x115) < 2) {
                    st->v31(*data, data);
                } else {
                    st->v31(0x115, 0);
                }
                *(int*)((char*)st + 0x6B4) = 0;
            } else {
                *(int*)((char*)st + 0x6B4) = 1;
            }
            return 1;
        }
        if (msg != 0x114) {
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
                case 0xEC:
                    func_00294F78(func_0028B180(), 7);
                case 0xED:
                case 0xEF:
                case 0xF1:
                case 0x112:
                case 0x113:
                    st->v27();
                    break;
                case 0x109:
                    func_00294F78(func_0028B180(), 8);
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
extern char D_00461B30[];
extern const char D_0045FF28[];

struct sVE1A8E40 {
    short delta;
    short index;
    int (*fn)(void*, int, void*);
};

// PORT: the unit declares func_001A8E40 with one arg; defined under an asm label with its real type.
extern "C" int func_001A8E40_impl(void* self, int msg, void* data) __asm__("func_001A8E40");

// Message handler for the memory-card screen.
extern "C" int func_001A8E40_impl(void* self, int msg, void* data)
{
    char* s = (char*)self;
    switch (msg) {
    case 0x115:
    case 0x116:
        func_001A9930((cFEStateNet*)self);
        break;
    case 0xDC:
        if (func_0025DA90(LobbyMan_mLobbyMan) == 0) {
            func_001A8A40(self, cFEStateLobbyMenu_cFEStateLobbyMenu_v(cMemMan_alloc(0xB84, D_00461B30, 0, 0), *(int*)(s + 0x10)));
        }
        break;
    case 0xEF:
        func_001A8A40(self, cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), *(int*)(s + 0x10)));
        break;
    case 0x102:
        if (*(int*)((char*)data + 0x8) == 0x74696D65) {
            func_001A8F98((cFEStateNet*)self);
        } else {
            void* popup = func_001A9C50((cFEStateNet*)self);
            func_001A9CC0((cFEStateNet*)self, &popup, self, data);
            func_001A9BC0((cFEStateNet*)self, popup);
        }
        break;
    case 0xDD:
    case 0xDE:
    case 0xE0:
    case 0xE1: {
        char* child = *(char**)(s + 0x20);
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
extern char D_00461618[];
extern char D_00461C40[];

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
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 1;
    func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(D_00461618));
    int h = GetHashValue32(D_00461C40);
    *(int*)((char*)p + 0xC4) = h;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x83E;
    *(int*)((char*)p + 0x14C) = 6;
    func_001A9BC0(self, p);
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

extern "C" void func_001A9090(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    func_001A91B0(self, a1, a4, a5, a6);
    func_001CCF00(self->mpKeyboard, a2);
    cKeyboardPopup_setInputText(self->mpKeyboard, a3);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A90F0);
#ifdef SKIP_ASM
extern "C" void cKeyboardPopup_setStatic(void* popup, int a);
extern "C" void func_001A91B0(cFEStateNet* self, int a1, int a2, int a3, int a4);
extern "C" void cKeyboardPopup_setInputText(void* p, int a);

extern "C" void func_001A90F0(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    func_001A91B0(self, a1, a4, a5, a6);
    cKeyboardPopup_setStatic(self->mpKeyboard, a2);
    cKeyboardPopup_setInputText(self->mpKeyboard, a3);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A9150);
#ifdef SKIP_ASM
extern "C" void func_001A91B0(cFEStateNet* self, int a1, int a2, int a3, int a4);
extern "C" void cKeyboardPopup_setStatic1(void* p, int a);
extern "C" void cKeyboardPopup_setInputText(void* p, int a);

extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    func_001A91B0(self, a1, a4, a5, a6);
    cKeyboardPopup_setStatic1(self->mpKeyboard, a2);
    cKeyboardPopup_setInputText(self->mpKeyboard, a3);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001A91B0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* mem, void* engine, void* owner, int a3, unsigned char a4);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cKeyboardPopup_setMaxCharacters(void* self, int v);
extern char D_0045D8D8[];

extern "C" void func_001A91B0(cFEStateNet* self, int a1, int a2, int a3, int a4)
{
    void* p = cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(0x444, D_0045D8D8, 0x100, 0), self->mpEngine, self, a2, 0xF);
    self->mpKeyboard = (cKeyboardPopup*)p;
    cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    *(int*)((char*)self->mpKeyboard + 0x18) = a3;
    cKeyboardPopup_setMaxCharacters(self->mpKeyboard, a4);
    self->mpKeyboardText = (cUIText*)a1;
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
extern const char D_00461C60[];

extern "C" void func_001A9268(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0xF) {
        func_001A93D0_3(self, a1, 0xF);
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), (int)self->mpEngine));
    } else {
        func_001A93D0_3(self, a1, a2);
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
        func_001A93D0_3(self, a1, 0x15);
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
        func_001A93D0_3(self, a1, 0x16);
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
extern const char D_00461CA0[];

extern "C" void func_001A9438(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0x16) {
        ((cFEVObj_001A9438*)self)->v32();
        func_001A8A40(self, cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(0x6D4, D_00461CA0, 0, 0), (int)self->mpEngine));
        func_001A93D0_3(self, a1, 0x16);
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
    case 0x15:
        func_001A93D0_3(self, a1, 0x15);
        break;
    case 0x16:
        ((cFEVObj_001A94E0*)self)->v32();
        self->mpKeyboard = 0;
        func_001A93D0_3(self, a1, 0x16);
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
extern const char D_0045FF10[];
extern const char D_0045FF28[];

extern "C" void func_001A9578(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A93D0_3(self, a1, a2);
        void* obj;
        if (func_0025DA90(LobbyMan_mLobbyMan) != 0 && D_004A4E88 == 0) {
            obj = cFEStateChatRoom_cFEStateChatRoom((sFEObj_001B1F88*)cMemMan_alloc(0x73C, D_0045FF10, 0, 0), (int)self->mpEngine);
        } else {
            obj = cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), (int)self->mpEngine);
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

extern "C" void func_001A9638(cFEStateNet* self, int a1, int a2, int a3)
{
    switch (a2) {
    case 0xF:
        break;
    case 0x14:
        break;
    default:
        func_001A93D0(self, a1, a2, a3);
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
extern const char D_00461CD8[];

extern "C" void func_001A9668(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A93D0_3(self, a1, a2);
        func_0039F698((char*)self->mpEngine + 0x18);
        self->unk_058 = 1;
        void* st = cUIStateStack_getCurrentState((char*)self->mpEngine + 0x18);
        *(int*)((char*)st + 0x1C) |= 0x80;
        func_001A8A40(self, cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(0x6D4, D_00461CD8, 0, 0), (int)self->mpEngine));
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
extern const char D_00460BF0[];

extern "C" void func_001A9710(cFEStateNet* self, int a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A93D0_3(self, a1, a2);
        func_0039F698((char*)self->mpEngine + 0x18);
        self->unk_058 = 1;
        void* st = cUIStateStack_getCurrentState((char*)self->mpEngine + 0x18);
        *(int*)((char*)st + 0x1C) |= 0x80;
        func_001A8A40(self, cFEStateNetConfig_cFEStateNetConfig((cFEStateNetConfig*)cMemMan_alloc(0x91C, D_00460BF0, 0, 0), (int)self->mpEngine));
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
extern "C" void func_001A97B8_impl(void* self, int popup, int a2)
{
    if (popup == *(int*)((char*)self + 0x69C)) {
        func_001A94E0((cFEStateNet*)self, popup, a2);
        return;
    }
    switch (*(int*)(popup + 0x18)) {
    case 0x832:
        func_001A9438((cFEStateNet*)self, popup, a2);
        break;
    case 0x833:
        func_001A9268((cFEStateNet*)self, popup, a2);
        break;
    case 0x834:
        func_001A92D8((cFEStateNet*)self, popup, a2);
        break;
    case 0x837:
        func_001A9578((cFEStateNet*)self, popup, a2);
        break;
    case 0x836:
        func_001A9638_3(self, popup, a2);
        break;
    case 0x83D:
        func_001A9668((cFEStateNet*)self, popup, a2);
        break;
    case 0x83E:
        func_001A9710((cFEStateNet*)self, popup, a2);
        break;
    default:
        func_001A93D0_3(self, popup, a2);
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
    cFEStateNet* s = self;
    int len = strlen(str);
    int i;
    for (i = 0; i < len && i < 0x3F; i++) {
        s->mszMaskedText[i] = '*';
    }
    if (len == 0) {
        s->mszMaskedText[0] = ' ';
        len = 1;
    }
    s->mszMaskedText[len] = 0;
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
extern char D_00461618[];
extern char D_00461C40[];
extern char D_00461D28[];

extern "C" void func_001A9930(cFEStateNet* self)
{
    if (self->unk_6AC == 0) {
        int one = 1;
        self->unk_6AC = one;
        func_0039F698((char*)self->mpEngine + 0x18);
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = one;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x170) = one;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x14C) = 6;
        if (*func_002591B8() == 0) {
            *(int*)((char*)p + 0x150) = 0x83E;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00461C40);
            *(int*)((char*)p + 0xC0) = 3;
        } else {
            *(int*)((char*)p + 0x150) = 0x83D;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00461D28);
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
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_0045DCC8[];
extern char D_0045FCD8[];

extern "C" void func_001A9A18(cFEStateNet* self, cUIListBox_001A9A18* box, int value)
{
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045DCC8), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045FCD8), 0);
    func_0039A7A8(box, value == 0);
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
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_0045E028[];
extern char D_0045DF40[];

extern "C" void func_001A9AB8(cFEStateNet* self, cUIListBox_001A9AB8* box, int value)
{
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045E028), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_0045DF40), 0);
    func_0039A7A8(box, value == 0);
    box->v09(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001A9B58);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001A3DC8(cFEStateNetMessenger* mem, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern const char D_00461D40[];

extern "C" void func_001A9B58(cFEStateNet* self)
{
    void* p = func_001A3DC8((cFEStateNetMessenger*)cMemMan_alloc(0x6FC, D_00461D40, 0x100, 0), self->mpEngine, self);
    *(int*)((char*)p + 0x18) = 0x833;
    cUIStateStack_pushPopup((char*)*(void**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
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
extern const char D_00461D58[];

extern "C" void* func_001A9C50(cFEStateNet* self)
{
    void* popup;
    void* cur = self->mpPopup;
    if (cur == 0) {
        popup = cScreenPopup_cScreenPopup(cMemMan_alloc(0x360, D_00461D58, 0x100, 0), self->mpEngine, self);
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
extern char D_00461618[];
extern char D_00461C40[];
extern char D_00461D68[];
extern char D_00461D88[];
extern char D_00461DA0[];
extern char D_00461DC0[];
extern char D_00461DE0[];
extern char D_00461E00[];
extern char D_00461E20[];
extern char D_00461E38[];
extern char D_00461E50[];
extern char D_00461E70[];
extern char D_00461E88[];
extern char D_00461EA0[];
extern char D_00461EB8[];
extern char D_00461ED8[];
extern char D_00461EF0[];
extern char D_00461F08[];
extern char D_00461F28[];
extern char D_00461F40[];
extern char D_00461F60[];
extern char D_00461F80[];
extern char D_00461F98[];
extern char D_00461FB0[];
extern char D_00461FC8[];
extern char D_00461FD8[];
extern char D_00461FF8[];
extern char D_00462010[];
extern char D_00462028[];
extern char D_00462040[];
extern char D_00462058[];
extern char D_00462078[];
extern char D_00462090[];
extern char D_004620A8[];
extern char D_004620C8[];
extern char D_004620E0[];
extern char D_004620F0[];
extern char D_00462108[];
extern char D_00462118[];
extern char D_00462130[];
extern char D_00462140[];
extern char D_00462158[];
extern char D_00462170[];
extern char D_00462190[];
extern char D_004621A8[];
extern char D_004621C0[];
extern char D_004621D8[];
extern char D_004621E8[];
extern char D_00462200[];
extern char D_00462218[];
extern char D_00462230[];
extern char D_00462248[];
extern char D_00462268[];
extern char D_00462280[];
extern char D_00462298[];
extern char D_004622B8[];
extern char D_004622D0[];
extern char D_004622E8[];
extern char D_00462308[];
extern char D_00462320[];
extern char D_00462338[];
extern char D_00462358[];
extern char D_00462378[];
extern char D_00462398[];
extern char D_004623B0[];
extern char D_004623C8[];
extern char D_004623E0[];
extern char D_004623F8[];
extern char D_00462418[];
extern char D_00462430[];
extern char D_00462450[];
extern char D_00462468[];
extern char D_00462480[];
extern char D_00462498[];
extern char D_004624B0[];
extern char D_004624D0[];
extern char D_004624E8[];
extern char D_00462510[];
extern char D_00462528[];
extern char D_00462548[];
extern char D_00462560[];
extern char D_00462580[];
extern char D_00462598[];
extern char D_004625B0[];
extern char D_004625C8[];
extern char D_004625E8[];
extern char D_00462600[];
extern char D_00462618[];

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
    func_001C57C0(sub, 0, GetHashValue32(D_00461618));
    *(int*)(p + 0x150) = 0x102;
    switch (*(int*)((char*)data + 8)) {
    case 0x696D7374: // 'imst'
        SetMsg_1A9CC0(sub, D_00461D68);
        break;
    case 0x69706572: // 'iper'
        SetText_1A9CC0(p, D_00461D88);
        break;
    case 0x70617373: // 'pass'
        SetMsg_1A9CC0(sub, D_00461DA0);
        break;
    case 0x64626572: // 'dber'
        SetMsg_1A9CC0(sub, D_00461DC0);
        break;
    case 0x6D697373: // 'miss'
        SetMsg_1A9CC0(sub, D_00461DE0);
        break;
    case 0x696E7670: // 'invp'
        SetText_1A9CC0(p, D_00461E00);
        break;
    case 0x6475706C: // 'dupl'
        SetMsg_1A9CC0(sub, D_00461E20);
        break;
    case 0x6D617870: // 'maxp'
        SetMsg_1A9CC0(sub, D_00461E38);
        break;
    case 0x64656670: // 'defp'
        SetText_1A9CC0(p, D_00461E50);
        break;
    case 0x6D617574: // 'maut'
        SetText_1A9CC0(p, D_00461E70);
        *(int*)(p + 0x150) = 0x83D;
        break;
    case 0x70617574: // 'paut'
        SetMsg_1A9CC0(sub, D_00461E88);
        break;
    case 0x75726F6D: // 'urom'
        SetMsg_1A9CC0(sub, D_00461EA0);
        break;
    case 0x756C7374: // 'ulst'
        SetText_1A9CC0(p, D_00461EB8);
        break;
    case 0x75757372: // 'uusr'
        SetText_1A9CC0(p, D_00461ED8);
        break;
    case 0x7567616D: // 'ugam'
        SetMsg_1A9CC0(sub, D_00461EF0);
        break;
    case 0x756D6163: // 'umac'
        SetMsg_1A9CC0(sub, D_00461F08);
        break;
    case 0x72616E67: // 'rang'
        SetText_1A9CC0(p, D_00461F28);
        break;
    case 0x73747274: // 'strt'
        SetMsg_1A9CC0(sub, D_00461F40);
        break;
    case 0x64657465: // 'dete'
        SetMsg_1A9CC0(sub, D_00461F60);
        break;
    case 0x6B69636B: // 'kick'
        SetText_1A9CC0(p, D_00461F80);
        break;
    case 0x686F7374: // 'host'
        SetMsg_1A9CC0(sub, D_00461F98);
        break;
    case 0x696E676D: // 'ingm'
        SetMsg_1A9CC0(sub, D_00461FB0);
        break;
    case 0x73656C66: // 'self'
        SetMsg_1A9CC0(sub, D_00461FC8);
        break;
    case 0x6E707269: // 'npri'
        SetText_1A9CC0(p, D_00461FD8);
        break;
    case 0x6E726F6D: // 'nrom'
        SetMsg_1A9CC0(sub, D_00461FF8);
        break;
    case 0x6E67616D: // 'ngam'
        SetMsg_1A9CC0(sub, D_00462010);
        break;
    case 0x726F6F6D: // 'room'
        SetMsg_1A9CC0(sub, D_00462028);
        break;
    case 0x61757468: // 'auth'
        SetMsg_1A9CC0(sub, D_00462040);
        break;
    case 0x6E657730: // 'new0'
        SetText_1A9CC0(p, D_00462058);
        break;
    case 0x70736574: // 'pset'
        SetMsg_1A9CC0(sub, D_00462078);
        break;
    case 0x6E6F776E: // 'nown'
        SetMsg_1A9CC0(sub, D_00462090);
        break;
    case 0x6C6F676E: // 'logn'
        SetText_1A9CC0(p, D_004620A8);
        break;
    case 0x66696C74: // 'filt'
        SetMsg_1A9CC0(sub, D_004620C8);
        break;
    case 0x6C6F636B: // 'lock'
        {
            const char* fmt = D_004620E0;
            const char* who = *(const char**)((char*)data + 0x10);
            char* obj = (char*)SSXApp->FELocale;
            const char* name = *(const char**)((char*)data + 0xC);
            sVEntry_1A9CC0* vt = *(sVEntry_1A9CC0**)(obj + 4);
            void* text = vt[4].fn(obj + vt[4].delta, GetHashValue32(D_004620F0));
            int len = USTR_length(text) + strlen(who) + strlen(name) + 7;
            if (self->unk_6B8 != 0) {
                cMemMan_free(self->unk_6B8);
                self->unk_6B8 = 0;
            }
            void* buf = operator_new_tag(len * 2, D_00462108, 0, 0);
            self->unk_6B8 = buf;
            sprintf2_1A9CC0(buf, fmt, text, who, name);
            *(void**)(sub + 8) = self->unk_6B8;
            *(int*)(sub + 4) = 1;
        }
        break;
    case 0x72737276: // 'rsrv'
        SetMsg_1A9CC0(sub, D_00462118);
        break;
    case 0x746F7361: // 'tosa'
        SetText_1A9CC0(p, D_00462130);
        break;
    case 0x73616D65: // 'same'
        SetText_1A9CC0(p, D_00462140);
        break;
    case 0x69676E6F: // 'igno'
        SetMsg_1A9CC0(sub, D_00462158);
        break;
    case 0x6368616C: // 'chal'
        SetMsg_1A9CC0(sub, D_00462170);
        break;
    case 0x67706172: // 'gpar'
        SetText_1A9CC0(p, D_00462190);
        break;
    case 0x66756C6C: // 'full'
        SetText_1A9CC0(p, D_004621A8);
        break;
    case 0x64697361: // 'disa'
        SetMsg_1A9CC0(sub, D_004621C0);
        break;
    case 0x776F6C66: // 'wolf'
        SetMsg_1A9CC0(sub, D_004621D8);
        break;
    case 0x6B696B68: // 'kikh'
        SetMsg_1A9CC0(sub, D_004621E8);
        break;
    case 0x72616E6B: // 'rank'
        SetMsg_1A9CC0(sub, D_00462200);
        break;
    case 0x626F6775: // 'bogu'
        SetMsg_1A9CC0(sub, D_00462218);
        break;
    case 0x71756974: // 'quit'
        SetMsg_1A9CC0(sub, D_00462230);
        break;
    case 0x736F6F6E: // 'soon'
        SetMsg_1A9CC0(sub, D_00462248);
        break;
    case 0x74696D65: // 'time'
        SetText_1A9CC0(p, D_00462268);
        break;
    case 0x6F766572: // 'over'
        SetText_1A9CC0(p, D_00462280);
        break;
    case 0x62616463: // 'badc'
        SetMsg_1A9CC0(sub, D_00462298);
        break;
    case 0x6F6E6C6E: // 'onln'
        SetMsg_1A9CC0(sub, D_004622B8);
        break;
    case 0x61756E72: // 'aunr'
        SetText_1A9CC0(p, D_004622D0);
        break;
    case 0x6E6B696B: // 'nkik'
        SetText_1A9CC0(p, D_004622E8);
        break;
    case 0x626C616B: // 'blak'
        SetText_1A9CC0(p, D_00462308);
        break;
    case 0x6D61696C: // 'mail'
        SetMsg_1A9CC0(sub, D_00462320);
        break;
    case 0x706D616C: // 'pmal'
        SetText_1A9CC0(p, D_00462338);
        break;
    case 0x626F726E: // 'born'
        SetText_1A9CC0(p, D_00462358);
        break;
    case 0x67656E64: // 'gend'
        SetMsg_1A9CC0(sub, D_00462378);
        break;
    case 0x7370616D: // 'spam'
        SetText_1A9CC0(p, D_00462398);
        break;
    case 0x72617574: // 'raut'
        SetText_1A9CC0(p, D_004623B0);
        break;
    case 0x6E656D6C: // 'neml'
        SetMsg_1A9CC0(sub, D_004623C8);
        break;
    case 0x6D616E79: // 'many'
        SetMsg_1A9CC0(sub, D_004623E0);
        break;
    case 0x65636F6E: // 'econ'
        SetText_1A9CC0(p, D_004623F8);
        break;
    case 0x6967616D: // 'igam'
        SetText_1A9CC0(p, D_00462418);
        break;
    case 0x69616472: // 'iadr'
        SetMsg_1A9CC0(sub, D_00462430);
        break;
    case 0x6E706C61: // 'npla'
        SetMsg_1A9CC0(sub, D_00462450);
        break;
    case 0x63686574: // 'chet'
        SetText_1A9CC0(p, D_00462468);
        break;
    case 0x746F6F79: // 'tooy'
        SetMsg_1A9CC0(sub, D_00462480);
        break;
    case 0x6E737276: // 'nsrv'
        SetMsg_1A9CC0(sub, D_00462498);
        break;
    case 0x69706173: // 'ipas'
        SetMsg_1A9CC0(sub, D_004624B0);
        break;
    case 0x696E616D: // 'inam'
        SetText_1A9CC0(p, D_004624B0);
        break;
    case 0x656C656E: // 'elen'
        SetMsg_1A9CC0(sub, D_004624D0);
        break;
    case 0x6974726E: // 'itrn'
        SetMsg_1A9CC0(sub, D_004624E8);
        break;
    case 0x6A6F696E: // 'join'
        SetMsg_1A9CC0(sub, D_00462510);
        break;
    case 0x616A6F69: // 'ajoi'
        SetMsg_1A9CC0(sub, D_00462528);
        break;
    case 0x6E737063: // 'nspc'
        SetText_1A9CC0(p, D_004621A8);
        break;
    case 0x6E666E64: // 'nfnd'
        SetMsg_1A9CC0(sub, D_00462548);
        break;
    case 0x78697374: // 'xist'
        SetMsg_1A9CC0(sub, D_00462560);
        break;
    case 0x7465616D: // 'team'
        SetMsg_1A9CC0(sub, D_00462580);
        break;
    case 0x6D617874: // 'maxt'
        SetText_1A9CC0(p, D_00462598);
        break;
    case 0x74737461: // 'tsta'
        SetMsg_1A9CC0(sub, D_004625B0);
        break;
    case 0x696E7073: // 'inps'
        SetMsg_1A9CC0(sub, D_004625C8);
        break;
    case 0x63646576: // 'cdev'
        flag = 1;
        SetMsg_1A9CC0(sub, D_004625E8);
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
            *(int*)(sub + 0x10) = GetHashValue32(D_00462600);
            *(int*)(sub + 0xC) = 3;
            cBXString__cBXString(&str, 2);
        }
        break;
    case 0x646E7366: // 'dnsf'
        unknown = 1;
        SetText_1A9CC0(p, D_00461C40);
        break;
    default:
        unknown = 1;
        SetMsg_1A9CC0(sub, D_00462618);
        break;
    }
    if (!unknown) {
        if (!flag) {
            return;
        }
        *(int*)(sub + 0x94) = 0x83D;
    } else {
        func_00255FA0(cCommSystem_mCommSystem);
        *(int*)(sub + 0x94) = 0x83E;
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
extern void* D_004A5B64;
extern char D_00462638[];
extern char D_00462650[];
extern char D_00462668[];

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
    unsigned int now = *(unsigned int*)((char*)D_004A5B64 + 0x18);
    cFEVObj_001AA9B0* a = (cFEVObj_001AA9B0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462638));
    if (a != 0) {
        if (self->mChallengeHintFrame != 0) {
            a->v09((now - self->mChallengeHintFrame) % 60 > 30);
        } else {
            a->v09(0);
        }
    }
    cFEVObj_001AA9B0* b = (cFEVObj_001AA9B0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462650));
    a = (cFEVObj_001AA9B0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462668));
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
extern void* D_004A5B64;
extern char D_00462650[];
extern char D_004A12C0[];
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
    unsigned int now = *(unsigned int*)((char*)D_004A5B64 + 0x18);
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
    cFEVObj_001AAB68* text = (cFEVObj_001AAB68*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462650));
    if (text == 0) {
        return;
    }
    if (n > 0) {
        text->v09(1);
        sprintf(buf, D_004A12C0, n);
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
extern char D_00462678[];

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
        sprintf(name, D_00462678, slot + 1, i + 'a');
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
extern char D_00462688[];
extern char D_004626A0[];
extern char D_004626B8[];
extern char D_004626D0[];
extern char D_004626E8[];
extern char D_00462700[];
extern char D_00462710[];
extern char D_00462728[];
extern char D_00462740[];

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
        h = GetHashValue32(D_00462688);
        break;
    case 0x2D646576:
        h = GetHashValue32(D_004626A0);
        break;
    case 0x2D636667:
        h = GetHashValue32(D_004626B8);
        break;
    case 0x2D696E69:
        h = GetHashValue32(D_004626D0);
        break;
    case 0x2D746F6E:
        h = GetHashValue32(D_004626E8);
        break;
    case 0x2D627379:
        h = GetHashValue32(D_00462700);
        break;
    case 0x2D636F6E:
        h = GetHashValue32(D_00462710);
        break;
    case 0x2D3F3F3F:
        h = GetHashValue32(D_00462728);
        break;
    case 0x2D617468:
        h = GetHashValue32(D_00462740);
        break;
    case 0x2D646E73:
        h = GetHashValue32(D_00462728);
        break;
    case 0x74696D65:
        h = GetHashValue32(D_00462728);
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
extern char D_00462750[];

extern "C" void func_001AB108(cFEStateNet* self)
{
    void* p = func_001A9C50(self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0x170) = 0;
    *(int*)((char*)p + 0x150) = 0x836;
    *(int*)((char*)p + 0x14C) = 6;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462750);
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x164) = 0;
    func_001A9BC0(self, p);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AB178);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00462768[];
extern char D_00462780[];
extern char D_004627A0[];
extern char D_00462158[];
extern char D_004627B8[];
extern char D_004627D8[];
// Shows an error popup with the message matching the error code.
extern "C" void func_001AB178(cFEStateNet* self, int err)
{
    void* p = func_001A9C50(self);
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    func_001C57C0(sub, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x150) = 0x835;
    *(int*)((char*)p + 0x14C) = 6;
    *(int*)((char*)p + 0x170) = 0;
    switch (err) {
    case 0xF2:
        *(int*)(sub + 0x8) = GetHashValue32(D_00462768);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0xF1: case 0x10C: case 0x113:
        *(int*)(sub + 0x8) = GetHashValue32(D_00462780);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0x112:
        *(int*)(sub + 0x8) = GetHashValue32(D_004627A0);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0xF0:
        *(int*)(sub + 0x8) = GetHashValue32(D_00462158);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0xED:
        *(int*)(sub + 0x8) = GetHashValue32(D_004627B8);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0xEE:
        *(int*)(sub + 0x8) = GetHashValue32(D_004627D8);
        *(int*)(sub + 0x4) = 3;
        break;
    }
    func_001A9BC0(self, p);
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
extern char D_00461618[];
extern char D_00462780[];
extern char D_004627A0[];
extern char D_004627D8[];
// Shows an error popup with the message matching the error code.
extern "C" void func_001AB288(cFEStateNet* self, int err)
{
    void* p = func_001A9C50(self);
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    func_001C57C0(sub, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x150) = 0x837;
    *(int*)((char*)p + 0x14C) = 6;
    *(int*)((char*)p + 0x170) = 0;
    switch (err) {
    case 0xF1: case 0xFA: case 0x10C: case 0x113:
        *(int*)(sub + 0x8) = GetHashValue32(D_00462780);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0x112:
        *(int*)(sub + 0x8) = GetHashValue32(D_004627A0);
        *(int*)(sub + 0x4) = 3;
        break;
    case 0xEE: case 0xF8:
        *(int*)(sub + 0x8) = GetHashValue32(D_004627D8);
        *(int*)(sub + 0x4) = 3;
        break;
    }
    func_001A9BC0(self, p);
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
            if (id != 0x80B) {
                if (id != 0x80C) {
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
extern char D_00462938[];
extern char D_00462958[];
extern char D_00462970[];
extern char D_00462988[];
extern char D_004629A0[];
extern char D_004629C8[];
extern char D_004629E8[];
extern char D_00462A08[];
extern char D_00462A20[];
extern char D_00462A40[];
extern char D_00462A58[];
extern char D_00462A78[];
extern char D_00462A90[];

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
extern "C" void func_001AB478(cFEStateNet* self, void* popup, int a1, int a2, int a3)
{
    if (popup == 0) {
        return;
    }
    const char* name = (const char*)a1;
    char* p = (char*)popup;
    int stateA = func_00263490(GetSession_001AB478(), name);
    int stateB = func_002633C8(GetSession_001AB478(), name);
    int n = 0;
    char* sub = p + 0xBC;
    LobbyMan* g = LobbyMan_mLobbyMan;
    char s[4];
    cBXString_cBXString2(s, name);
    cBXString_operatorE((char*)g + 0x98, s);
    cBXString__cBXString(s, 2);
    *(int*)(p + 0xC8) = 2;
    *(int*)(p + 0x14C) = 3;
    *(const char**)(p + 0xCC) = name;
    if (a2) {
        *(int*)(p + 0xBC) = 5;
        *(int*)(p + 0x150) = 0x80B;
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        if (func_00263128(GetSession_001AB478(), name)) {
            func_001C57C0(sub, n, GetHashValue32(D_00462938));
        } else {
            func_001C57C0(sub, n, GetHashValue32(D_00462958));
        }
        n++;
    } else if (stateB != 0 && (stateA != 0 || func_00263128(GetSession_001AB478(), name))) {
        *(int*)sub = 4;
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        if (func_00263128(GetSession_001AB478(), name)) {
            func_001C57C0(sub, n, GetHashValue32(D_00462938));
        } else {
            func_001C57C0(sub, n, GetHashValue32(D_00462958));
        }
        n++;
        *(int*)(sub + 0x94) = 0x80B;
    } else {
        *(int*)sub = 3;
        *(int*)(sub + 0x94) = 0x80C;
    }
    if (func_00262A38(GetSession_001AB478(), a1)) {
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        func_001C57C0(sub, n++, GetHashValue32(D_00462970));
    } else {
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        func_001C57C0(sub, n++, GetHashValue32(D_00462988));
    }
    if (func_00262360(GetSession_001AB478(), name)) {
        char* buf = (char*)self + 0x49C;
        sprintf1(buf, Text_001AB478(D_004629A0), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(D_004629C8));
        n++;
    } else {
        char* buf = (char*)self + 0x49C;
        sprintf1(buf, Text_001AB478(D_004629E8), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(D_00462A08));
        n++;
    }
    if (func_002623B0(GetSession_001AB478(), name)) {
        char* buf = (char*)self + 0x59C;
        sprintf1(buf, Text_001AB478(D_00462A20), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(D_00462A40));
        n++;
    } else {
        char* buf = (char*)self + 0x59C;
        sprintf1(buf, Text_001AB478(D_00462A58), (char*)name);
        func_001C5838(sub, n, (int)buf, 0);
        func_001C57C0(sub, n, GetHashValue32(D_00462A78));
        n++;
    }
    if (a2) {
        cPopupInfo_clearPopupConfirmation((cPopupInfo*)sub, n);
        func_001C57C0(sub, n, GetHashValue32(D_00462A90));
    }
    *(int*)(sub + 0xAC) = a3;
    *(int*)(sub + 0xB8) = 1;
    func_001A9BC0(self, p);
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
extern char D_00462AA8[];


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
        void* obj = cFEStateReadMessagePopup_cFEStateReadMessagePopup(cMemMan_alloc(0x6FC, D_00462AA8, 0x100, 0), ((int)self->mpEngine), self, a1);
        cUIStateStack_pushPopup(*(char**)(*(char**)(((char*)self->mpScreen) + 0xD0) + 0x10) + 0x18, obj);
    } else {
        func_001A9090((cFEStateNet*)self, 0, a1, (int)D_004A1408, 0, 2, 0x3F);
        *(int*)(((char*)self->mpKeyboard) + 0x43C) = 0;
        cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 0x4B, 1, 2);
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
extern char D_00462AC8[];
extern char D_00462AE8[];

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
        void* p = cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup((cFEStateChallengeReceivedPopup*)cMemMan_alloc(0x6D8, D_00462AC8, 0x100, 0), (int)self->mpEngine, (int)self, (const char*)a1);
        if (a4) {
            *(int*)((char*)p + 0x18) = 0x834;
        } else {
            *(int*)((char*)p + 0x18) = 0x801;
        }
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    } else {
        MEM_fill(D_005308B8, 0, 0x20);
        LobbyMan* g = LobbyMan_mLobbyMan;
        D_004A4E88_v[1] = 4;
        char s[4];
        cBXString_cBXString2(s, (const char*)a1);
        cBXString_operatorE((char*)g + 0x98, s);
        cBXString__cBXString(s, 2);
        void* p = cFEStateChallengePopup_cFEStateChallengePopup_BCD0(cMemMan_alloc(0x6F0, D_00462AE8, 0x100, 0), (int)self->mpEngine, (int)self);
        if (a4) {
            *(int*)((char*)p + 0x18) = 0x834;
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
extern char D_00461618[];
extern char D_00462B00[];

extern "C" void func_001ABED8(void* self)
{
    void* p = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)p + 0xBC) = 1;
    func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(D_00461618));
    int h = GetHashValue32(D_00462B00);
    //S
    *(int*)((char*)p + 0xC4) = h;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x845;
    *(int*)((char*)p + 0x14C) = 6;
    //E
    func_001A9BC0((cFEStateNet*)self, p);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABF70);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void* self);
extern void* D_0046B6F8[];

extern "C" void* func_001ABF70(cFEStateNet* self)
{
    func_001A8500(self);
    self->mVtbl = D_0046B6F8;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ABFA8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A1A78[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void* func_001ABFA8(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A1A78), 0);
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
extern int D_004A3E90;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEUseExistingAccount_cFEUseExistingAccount(cFEUseExistingAccount* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    int d = D_004A3E90;
    self->mVtbl = cFEUseExistingAccount_vtbl;
    self->mszEmail = (char*)d;
    self->mHashName = 0x35;
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

extern "C" void cFEUseExistingAccount__cFEUseExistingAccount(cFEUseExistingAccount* self, int flags)
{
    self->mVtbl = cFEUseExistingAccount_vtbl;
    cBXString__cBXString((char*)self + 0x6DC, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC0B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void func_001A8768(void* self);
extern char D_00462B20[];
extern char D_004A1408[];

extern "C" void func_001AC0B8(cFEUseExistingAccount* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_00462B20), 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001AC150__FPv);
#ifdef SKIP_ASM
void* func_001AC150(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC170);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_00462B38[];
extern char D_00462B48[];
extern char D_00462B58[];
extern char D_00462B70[];
extern char D_00462B88[];
extern char D_00462BA0[];
extern char D_004A1A80[];
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

extern "C" void func_001AC170(cFEUseExistingAccount* self, cUIText* text)
{
    if (IsHash_001AC170(*(int*)((char*)text + 0x38), D_00462B38)) {
        *(int*)((char*)text + 0x18) = 0;
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), D_00462B48)) {
        *(int*)((char*)text + 0x18) = 1;
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), D_00462B58)) {
        self->mpTextForgotPassword = (cUIThing*)text;
        *(int*)((char*)text + 0x18) = 2;
        ((cFEVObj_001AC170*)self->mpTextForgotPassword)->v25(1);
        ((cFEVObj_001AC170*)self->mpTextForgotPassword)->v08(1);
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), D_00462B70)) {
        *(int*)((char*)text + 0x18) = 3;
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), D_00462B88) || IsHash_001AC170(*(int*)((char*)text + 0x38), D_00462BA0)) {
        cUIText_setAsciiString(text, D_004A1408);
    } else if (IsHash_001AC170(*(int*)((char*)text + 0x38), D_004A1A80)) {
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
extern char D_00462BB0[];
extern char D_00461618[];

extern "C" int func_001AC2B8(cFEUseExistingAccount* self)
{
    if (strlen(((const char*)LobbyMan_mLobbyMan->mPassword)) == 0) {
    void* p = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BB0);
    *(int*)((char*)p + 0xC0) = 3;
    func_001C57C0(sub, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x150) = 0x80D;
    func_001A9BC0((cFEStateNet*)self, p);
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
extern char D_00462BD0[];
extern char D_00461618[];

extern "C" int func_001AC360(cFEUseExistingAccount* self)
{
    if (strlen(((const char*)LobbyMan_mLobbyMan->mUserName)) == 0) {
    void* p = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)p + 0xBC;
    *(int*)((char*)p + 0xBC) = 1;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BD0);
    *(int*)((char*)p + 0xC0) = 3;
    func_001C57C0(sub, 0, GetHashValue32(D_00461618));
    *(int*)((char*)p + 0x150) = 0x80D;
    func_001A9BC0((cFEStateNet*)self, p);
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
extern char D_00462BF0[];
extern char D_00461618[];

extern "C" int func_001AC448(cFEUseExistingAccount* self)
{
    // PORT: string header read through a pointer held in an int
    if (*(int*)((int)self->mszEmail - 8) == 0) {
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BF0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
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
extern char D_00462C08[];
extern char D_00462C28[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001AC4F0(cFEUseExistingAccount* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00462C08);
    func_001DA530(popup, 0, GetHashValue32(D_00462C28));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC5A8);
#ifdef SKIP_ASM
extern "C" int func_001AC5A8(cFEUseExistingAccount* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC5C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void* cFEStateNetAccountMem_cFEStateNetAccountMem(cFEStateNetAccountMem* mem, int a1, void* owner, int a3, int a4, int a5);
extern char D_00461618[];
extern char D_00462C88[];
extern char D_00462D10[];
extern "C" void* cUIScreen_getObjectByHashName_C5C0(void* self, int hash) __asm__("cUIScreen_getObjectByHashName");
extern "C" int func_001AC408_C5C0(void* self) __asm__("func_001AC408");
extern "C" int func_001AC360_C5C0(void* self) __asm__("func_001AC360");
extern "C" void func_001ACDD0_C5C0(void* self) __asm__("func_001ACDD0");
extern "C" void func_0025B6D8_C5C0(void* self, int a1) __asm__("func_0025B6D8");
extern char D_00462B88[];
extern char D_00462BA0[];
extern char D_00462C70[];
extern char D_00462C98[];
extern char D_00462CB8[];
extern char D_00462CD0[];
extern char D_00462CE8[];
extern char D_00462D00[];

struct sState_C5C0 {
    char pad0[0x6D8];
    int mode;   // 0x6D8
};

// Message handler for the network-config menu.
extern "C" void func_001AC5C0(cFEUseExistingAccount* self, void* a1, unsigned int msg)
{
    switch (msg) {
    case 5:
        switch (*(unsigned int*)((char*)a1 + 0x18)) {
        case 4: {
            if (!func_001AC408_C5C0(((char*)self))) {
                break;
            }
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 1;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462C70);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00462C88));
            *(int*)((char*)p + 0x150) = 0x80E;
            func_001A9BC0((cFEStateNet*)self, p);
            ((sState_C5C0*)self)->mode = 4;
            func_0025B6D8_C5C0(LobbyMan_mLobbyMan, 0);
            break;
        }
        case 2: {
            if (!func_001AC360_C5C0(((char*)self))) {
                break;
            }
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 0;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462C98);
            *(int*)((char*)p + 0xC0) = 3;
            *(int*)((char*)p + 0x164) = 0;
            *(int*)((char*)p + 0x150) = 0x812;
            func_001A9BC0((cFEStateNet*)self, p);
            ((sState_C5C0*)self)->mode = 2;
            func_0025B6D8_C5C0(LobbyMan_mLobbyMan, 0);
            break;
        }
        case 3: {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = -1;
            *(int*)((char*)p + 0x180) = 0x32;
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462CB8);
            *(int*)((char*)p + 0xC0) = 3;
            *(int*)((char*)p + 0xD4) = GetHashValue32(D_00462CD0);
            *(int*)((char*)p + 0xD0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)p + 0x150) = 0x80F;
            func_001A9BC0((cFEStateNet*)self, p);
            break;
        }
        case 0: {
            void* obj = cUIScreen_getObjectByHashName_C5C0(self->mpScreen, GetHashValue32(D_00462B88));
            if (obj == 0) {
                break;
            }
            int v = (int)LobbyMan_mLobbyMan->mUserName;
            func_001A9150((cFEStateNet*)self, (int)obj, GetHashValue32(D_00462CE8), v, 0, 6, 0x10);
            *(int*)((char*)self->mpKeyboard + 0x58) = 1;
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            break;
        }
        case 1: {
            void* obj = cUIScreen_getObjectByHashName_C5C0(self->mpScreen, GetHashValue32(D_00462BA0));
            if (obj == 0) {
                break;
            }
            int v = (int)LobbyMan_mLobbyMan->mPassword;
            func_001A9150((cFEStateNet*)self, (int)obj, GetHashValue32(D_00462D00), v, 1, 7, 0x10);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            break;
        }
        }
        break;
    case 6: {
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, ((char*)self), 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 13:
        func_001ACDD0_C5C0(((char*)self));
        break;
    default:
        func_001A8918((cFEStateNet*)self, (int)a1, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AC920);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001AC9C0(cFEUseExistingAccount* self, void* a1, int a2);
extern "C" void func_001ACA88(cFEUseExistingAccount* self, void* a1, int a2);
extern "C" void func_001ACB48(cFEUseExistingAccount* self, void* a1, int a2);
extern "C" void func_001ACCD0(cFEUseExistingAccount* self, void* a1, int a2);

extern "C" void func_001AC920(cFEUseExistingAccount* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x80E:
        func_001ACA88(self, a1, a2);
        break;
    case 0x80F:
        func_001AC9C0(self, a1, a2);
        break;
    case 0x82C:
        func_001ACB48(self, a1, a2);
        break;
    case 0x81E:
        func_001ACCD0(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
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
extern char D_00462D48[];

extern "C" void func_001AC9C0(cFEUseExistingAccount* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        int three = 3;
        func_001A97B8(self, a1, 0xF);
        cBXString_cBXString4((char*)self + 0x6DC, *(const char**)((char*)a1 + 0x2B0));
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D48);
        *(int*)((char*)p + 0xC0) = three;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x810;
        func_001A9BC0((cFEStateNet*)self, p);
        if (func_001AC448(self) != 0) {
            self->miRequest = three;
            func_0025B6D8(LobbyMan_mLobbyMan, 0);
        }
    } else {
        func_001A97B8(self, a1, a2);
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

extern "C" void func_001ACA88(cFEUseExistingAccount* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_00261408(GetSession_001ACA88());
        func_0025B800(LobbyMan_mLobbyMan);
        func_001A97B8(self, a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
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
extern const char D_00462D68[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];
extern char D_00462D80[];

extern "C" void func_001ACB48(cFEUseExistingAccount* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15:
    case 16:
        self->mbAskSave = 0;
        func_001A97B8(self, a1, kind);
        func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
        break;
    case 17:
        self->mbAskSave = 1;
        func_001A97B8(self, a1, 17);
        break;
    case 22:
        if (self->mbAskSave != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 2;
            char* sub = (char*)p + 0xBC;
            func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
            func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
            *(int*)((char*)p + 0x150) = 0x81E;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D80);
            *(int*)((char*)p + 0xC0) = 3;
            *(int*)((char*)p + 0x168) = 0;
            *(int*)((char*)p + 0x164) = 0;
            func_001A9BC0((cFEStateNet*)self, p);
        }
        self->mbAskSave = 0;
        func_001A97B8(self, a1, 22);
        break;
    default:
        func_001A97B8(self, a1, kind);
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
extern char D_00462D10[];
extern const char D_00462D68[];

extern "C" void func_001ACCD0(cFEUseExistingAccount* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(((cFEStateNet*)self), a1, 15);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 1,
                                ((int)LobbyMan_mLobbyMan->mUserName), ((int)LobbyMan_mLobbyMan->mPassword));
        *(int*)((char*)p + 0x18) = 0x82C;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 16:
    case 20:
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
        break;
    default:
        func_001A97B8(((cFEStateNet*)self), a1, kind);
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
extern char D_00462B88[];
extern char D_00462BA0[];

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
    char* p = (char*)self->mpKeyboard;
    switch (*(int*)(p + 0x18)) {
    case 6: {
        const char* name = p + 0x74;
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462B88));
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
        const char* name = p + 0x74;
        if (cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462BA0)) != 0) {
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
extern char D_00462D98[];
extern char D_00462DB0[];
extern char D_00462D80[];
extern char D_00461618[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

extern "C" int func_001ACEF8(cFEUseExistingAccount* self, int id, int a2)
{
    switch (id) {
    case 0xC9:
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
    case 0xCF: {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D98);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x811;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    case 0xD2: {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462DB0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x813;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    case 0xD4: {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 2;
        char* sub = (char*)p + 0xBC;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)((char*)p + 0x150) = 0x81E;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D80);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x168) = 0;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    default:
        return func_001A8E40_3i(self, id, a2);
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

extern "C" void* cFEStateSelectPersona_cFEStateSelectPersona(cFEStateSelectPersona* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->mHashName = 0x38;
    self->mVtbl = cFEStateSelectPersona_vtbl;
    self->mbExistingPersona = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001AD178);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char D_00462DC8[];

extern "C" void func_001AD178(cFEStateSelectPersona* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_00462DC8), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001A8768(((cFEStateNet*)self));
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD1E8__FPv);
#ifdef SKIP_ASM
void* func_001AD1E8(void* self)
{
    return func_001A8770((cFEStateNet*)self);
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
extern char D_004615F8[];
extern char D_00461618[];
extern char D_00462DE0[];

extern "C" void func_001AD208(cFEStateSelectPersona* self, const char* name)
{
    if (strlen(name) < 3) {
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x814;
        func_001A9BC0((cFEStateNet*)self, p);
    } else {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462DE0);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x815;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, p);
        func_0025D1B8(LobbyMan_mLobbyMan, name);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD308);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void func_001AD3E8(cFEStateSelectPersona* self, cUIText* text);
extern char D_00462E00[];
extern char D_00462E10[];
extern char D_00462E20[];
extern char D_00462E30[];
extern char D_00462E40[];

static inline int IsHash1AD308(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001AD308(cFEStateSelectPersona* self, cUIText* text)
{
    LobbyMan* g = LobbyMan_mLobbyMan;
    if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E00)) {
        func_001AD3E8(self, text);
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E10)) {
        *(int*)((char*)text + 0x18) = -1;
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E20)) {
        cUIText_setAsciiString(text, ((const char*)g->mUserName));
    } else if (IsHash1AD308(*(int*)((char*)text + 0x38), D_00462E30)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00462E40));
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
extern "C" cUIText* func_00397870(void* list, int i);
extern char D_00462E58[];

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
    LobbyMan* g = LobbyMan_mLobbyMan;
    int i = 0;
    for (; i < g->mNumPersonas; i++) {
        cUIText* item = func_00397870((char*)text + 0x74, i);
        *(int*)((char*)item + 0x18) = i;
        cUIText_setAsciiString(item, ((const char**)&g->mPersonas[0])[i]);
        ((cFEVObj_001AD3E8*)item)->v09(1);
    }
    for (; i < 4; i++) {
        cUIText* item = func_00397870((char*)text + 0x74, i);
        *(int*)((char*)item + 0x18) = i;
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_00462E58));
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
extern char D_00462E70[];
extern char D_00462E88[];
extern char D_00462EA0[];
extern char D_0045FE60[];
extern char D_00462EB8[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001AD4F0(cFEStateSelectPersona* self, void* menu)
{
    func_001DA528(menu, 4);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00462E70);
    func_001DA530(menu, 0, GetHashValue32(D_00462E88));
    func_001DA530(menu, 1, GetHashValue32(D_00462EA0));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_00462EB8), 2);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD5E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00462C88[];
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1_D5E8(void* mem, int a1, int a2) __asm__("cFEStateCreateStep1_cFEStateCreateStep1");
extern "C" void func_001AD208_D5E8(void* self, const char* name) __asm__("func_001AD208");
extern "C" void func_001C5878_D5E8(void* self, int a1, int a2, int a3) __asm__("func_001C5878");
void func_0025CD28(void* self, int a1);
extern char D_00462ED0[];
extern char D_00462E58[];
extern char D_00462EE8[];
extern char D_00462F08[];
extern char D_00462F18[];

// Message handler for the persona-select screen.
extern "C" void func_001AD5E8(cFEStateSelectPersona* selfp, void* a1, int a2)
{
    switch (a2) {
    case 5: {
        int idx = *(int*)((char*)a1 + 0x18);
        if (idx == -1) {
            func_001A8A40(((char*)selfp), cFEStateCreateStep1_cFEStateCreateStep1_D5E8(cMemMan_alloc(0x6F4, D_00462ED0, 0, 0), (int)selfp->mpEngine, 0));
            break;
        }
        if (idx >= LobbyMan_mLobbyMan->mNumPersonas) {
            func_001A9150((cFEStateNet*)selfp, (int)a1, GetHashValue32(D_00462E58), (int)D_004A1408, 0, -1, 0xC);
            *(int*)((char*)selfp->mpKeyboard + 0x58) = 1;
            cKeyboardPopup_setIncludeKey(selfp->mpKeyboard, 0x4B, 1, 2);
            cKeyboardPopup_setIncludeKey(selfp->mpKeyboard, 0x35, 0, 2);
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
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)selfp->mpEngine, ((char*)selfp), 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)selfp->mpScreen + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 8: {
        int idx = *(int*)((char*)a1 + 0x18);
        if (idx < 0) {
            break;
        }
        if (idx < LobbyMan_mLobbyMan->mNumPersonas) {
            selfp->miDeletePersona = idx;
            void* p = func_001A9C50((cFEStateNet*)selfp);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462EE8);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00462F08));
            func_001C57C0(sub, 1, GetHashValue32(D_00462C88));
            func_001C5878_D5E8(sub, 0, GetHashValue32(D_00462F18), 1);
            *(int*)((char*)p + 0x168) = 1;
            *(int*)((char*)p + 0x150) = 0x816;
            func_001A9BC0((cFEStateNet*)selfp, p);
        }
        break;
    }
    case 13:
        func_001AD208_D5E8(((char*)selfp), (char*)selfp->mpKeyboard + 0x74);
        break;
    case 14:
        break;
    default:
        func_001A8918((cFEStateNet*)selfp, (int)a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD8C0);
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

extern "C" void func_001AD8C0(cFEStateSelectPersona* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x816:
        ((cFEVObj_001AD8C0*)self)->v33(a1, a2);
        break;
    case 0x819:
        func_001AD930(self, a1, a2);
        break;
    default:
        func_001A97B8(((cFEVObj_001AD8C0*)self), a1, a2);
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
extern char D_00462D10[];

extern "C" void func_001AD930(cFEStateSelectPersona* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(((cFEStateNet*)self), a1, 0xF);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8(((cFEStateNet*)self), a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AD9C8);
#ifdef SKIP_ASM
extern "C" void func_0025D428(void* self, void* a1);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" void func_001AD9C8(void* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_0025D428(LobbyMan_mLobbyMan, *(void**)((char*)self + 0x6D4));
        func_001A97B8(self, a1, 0xF);
    } else {
        func_001A97B8(self, a1, a2);
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
extern const char D_0045FF28[];
extern char D_00461618[];
extern char D_00462E00[];
extern char D_00462F58[];
extern char D_00462F78[];

struct sVE1ADA30 {
    short delta;
    short index;
    void (*fn)(void*, cUIText*);
};

// Message handler for the network-settings screen; returns -1 when handled.
extern "C" int func_001ADA30(cFEStateSelectPersona* self, int a1, int a2)
{
    switch (a1) {
    case 0xD9: {
        void* p = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462F58);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x817;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0(((cFEStateNet*)self), p);
        break;
    }
    case 0x104: {
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
        void* p = func_001A9C50(((cFEStateNet*)self));
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462F78);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x819;
        func_001A9BC0(((cFEStateNet*)self), p);
    }
    case 0xD6:
        func_0039F190((char*)self->mpEngine + 0x18, 1);
    case 0xD8:
        {
            sVE1ADA30* vt = (sVE1ADA30*)self->mVtbl;
            vt[18].fn((char*)self + vt[18].delta, cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462E00)));
        }
        break;
    case 0x102:
        func_001ADC30(self, a2);
        break;
    case 0x111:
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), (int)self->mpEngine));
        break;
    default:
        return (int)func_001A8E40_3(((cFEStateNet*)self), a1, a2);
    case 0xE1:
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
extern char D_004615F8[];
extern char D_00461618[];
extern char D_004620C8[];
extern char D_00463090[];
extern char D_004630A8[];

// PORT: the message pointer arrives as int
extern "C" void func_001ADC30(cFEStateSelectPersona* self, int a2)
{
    void* msg = (void*)a2;
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x6475706C:
        func_001A8A40(self, cFEStateCreateDuplicate_cFEStateCreateDuplicate((cFEStateCreateDuplicate*)cMemMan_alloc(0x6E0, D_00463090, 0, 0), (int)self->mpEngine, 0));
        break;
    case 0x696E616D:
        {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 1;
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)p + 0x150) = 0x818;
            func_001A9BC0((cFEStateNet*)self, p);
        }
        break;
    case 0x66696C74:
        {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 1;
            char* sub = (char*)p + 0xBC;
            if (self->mbExistingPersona == 0) {
                *(int*)((char*)p + 0xC4) = GetHashValue32(D_004630A8);
                *(int*)((char*)p + 0xC0) = 3;
                *(int*)((char*)p + 0x150) = 0x818;
            } else {
                *(int*)((char*)p + 0xC4) = GetHashValue32(D_004620C8);
                *(int*)((char*)p + 0xC0) = 3;
                *(int*)((char*)p + 0x150) = 0x83D;
            }
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            func_001A9BC0((cFEStateNet*)self, p);
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
INCLUDE_ASM("fe/feasyncfile", func_001ADE88);
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
extern char D_004630C8[];
extern char D_004630E0[];

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

extern "C" void func_001ADE88(cFEStateChallengePopup* self)
{
    sVE1ADE88* vt = (sVE1ADE88*)self->mVtbl;
    vt[32].fn((char*)self + vt[32].delta);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004630C8), 0);
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
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004630E0)), 6);
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
INCLUDE_ASM("fe/feasyncfile", func_001ADFB8);
#ifdef SKIP_ASM
class func_001ADFB8_cObj {
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

extern "C" int func_001ADFB8(cFEStateChallengePopup* self)
{
    ((func_001ADFB8_cObj*)self)->v32();
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001ADFE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001A87D0(cFEStateNet* self, int a1);
extern "C" void func_0039A7A8(void* self, int a1);
extern "C" void func_001AF098(cFEStateChallengePopup* self, int a1);
extern "C" void func_001AEC60(cFEStateChallengePopup* self, int a1);
extern signed char D_00535C12[];
extern char D_00441268[];
extern char D_00441278[];
extern char D_00441288[];
extern char D_00441298[];
extern char D_004412A8[];
extern char D_004412B8[];

// Refreshes the race-options screen and picks the mode title for the current game mode.
extern "C" void func_001ADFE8(cFEStateChallengePopup* self, int a1)
{
    func_001A87D0((cFEStateNet*)self, a1);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    func_0039A7A8(self->mpLBRankedGame, cBENetworkInterface_networkOpt.bIsRankedGame ^ 1);
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
INCLUDE_ASM("fe/feasyncfile", func_001AE0E0__FPv);
#ifdef SKIP_ASM
void* func_001AE0E0(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AE100);
#ifdef SKIP_ASM
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int func_001AE100(cFEStateChallengePopup* self, int a1, unsigned int key)
{
    int r = 0x101;
    if (key < 10) {
        if (key < 8) {
            r = 0x101;
        } else {
            return 0x100;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AE128);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern "C" void func_0039A7A8(void* self, int a1);
extern unsigned char D_004A4E88_E128[8] __asm__("D_004A4E88");
extern char D_00461520[];
extern char D_00463108[];
extern char D_00463118[];
extern char D_00463130[];
extern char D_00463140[];
extern char D_00463150[];
extern char D_00463168[];
extern char D_00463178[];
extern char D_00463190[];
extern char D_004631A0[];
extern char D_004631B8[];
extern char D_004631C8[];
extern char D_004631D8[];
extern char D_004631E8[];
extern char D_00462C88[];
extern char D_004631F8[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];
extern char D_00463210[];
extern char D_00463228[];
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];
extern char D_004A1510[];
extern char D_00463238[];
extern char D_00463248[];
extern char D_00463260[];
extern char D_004630E0[];

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

extern "C" void func_001AE128(cFEStateChallengePopup* self, cUIText* item)
{
    cFEVObj_001AE128* obj = (cFEVObj_001AE128*)item;
    if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00461520)) {
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463108)) {
        cUIText_setAsciiString(item, ((const char*)LobbyMan_mLobbyMan->mOpponent));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463118)) {
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463130)) {
        *(int*)((char*)item + 0x18) = 7;
        obj->v25(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463140)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_00463150));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463168)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_00463178));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463190)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_004631A0));
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_004631B8)) {
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_004631C8)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_004631D8));
        *(int*)((char*)item + 0x18) = 9;
        obj->v25(1);
        obj->v09(1);
        obj->v25(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_004631E8)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(D_00462C88));
        *(int*)((char*)item + 0x18) = 0xA;
        obj->v09(1);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_004631F8)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045DCC8), 1);
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045FCD8), 0);
        obj->v09(1);
        obj->v24(1);
        self->mpLBRankedGame = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463210)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_00463228), 0);
        cUIListBox_addEntryByAsciiString(item, D_004A14F8, 1);
        cUIListBox_addEntryByAsciiString(item, D_004A1500, 2);
        cUIListBox_addEntryByAsciiString(item, D_004A1508, 3);
        cUIListBox_addEntryByAsciiString(item, D_004A1510, 4);
        obj->v09(1);
        obj->v24(1);
        self->mpLBCPURiders = (cUIListBox*)item;
        func_0039A7A8(item, D_004A4E88_E128[4]);
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463238)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045DCC8), 0);
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045FCD8), 1);
        obj->v09(1);
        obj->v24(1);
        self->mpLBPowerUps = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463248)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045DCC8), 0);
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045FCD8), 1);
        obj->v09(1);
        obj->v24(1);
        self->mpLBMultipliers = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_00463260)) {
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045DCC8), 0);
        cUIListBox_addEntryByStringID(item, GetHashValue32(D_0045FCD8), 1);
        obj->v09(1);
        obj->v24(1);
        self->mpLBPointIcons = (cUIListBox*)item;
    } else if (IsHash1AE128(*(int*)((char*)item + 0x38), D_004630E0)) {
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
extern "C" int func_0039A738(void* a);
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
        D_00534B34[0] = func_0039A738(self->mpLBRankedGame) != 0;
        sVE1AE680* vt = *(sVE1AE680**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        int v = func_0039A738(self->mpLBMultipliers);
        sOpts1AE680* o = (sOpts1AE680*)D_005308B8;
        o->b6 = v;
        o->b8 = func_0039A738(self->mpLBPowerUps);
        o->b7 = func_0039A738(self->mpLBPointIcons);
        char* nr = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
        cBENewRaceInterface_setNumberAI(nr, func_0039A738(self->mpLBCPURiders));
        sVE1AE680i* nvt = *(sVE1AE680i**)(nr + 0xC);
        nvt[1].fn(nr + nvt[1].delta);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AE7A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cBENewRaceInterface_setNumberAI_E7A0(void* iface, int n) __asm__("cBENewRaceInterface_setNumberAI");
extern "C" void func_001AE680_E7A0(void* self) __asm__("func_001AE680");
extern "C" void func_001AF568_E7A0(void* self) __asm__("func_001AF568");
extern "C" int func_0039A738_E7A0(void* item) __asm__("func_0039A738");
extern "C" void func_0025F4C0_E7A0(void* self, int a1, int a2) __asm__("func_0025F4C0");
extern "C" void func_0025F628_E7A0(void* self, int a1, int a2) __asm__("func_0025F628");
extern "C" void func_0039F600_E7A0(void* list) __asm__("func_0039F600");
extern "C" void* func_00186610_E7A0(void* mem, int a1, int a2) __asm__("func_00186610");
extern "C" void cUIStateStack_pushSpecial_E7A0(void* stack, void* state, int a2, int a3) __asm__("cUIStateStack_pushSpecial");
extern "C" void func_001AF098_E7A0(void* self, int on) __asm__("func_001AF098");
extern "C" void func_001AEC60_E7A0(void* self, int v) __asm__("func_001AEC60");
extern "C" void func_001AF208_E7A0(void* self, void* item, int on) __asm__("func_001AF208");
extern int cUITemplate_MAP_mPeak;
extern int D_004A2594;
extern int D_004A2598;
extern int D_00534B34[];
extern char D_00460C38[];

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

struct cState_E7A0 {
    char pad0[0x10];
    char* engine;            // 0x10
    signed char f14;         // 0x14
    char pad15[0x6D0 - 0x15];
    int f6D0;                // 0x6D0
    char pad6D4[0x6DC - 0x6D4];
    void* aiOn;              // 0x6DC
    void* numAI;             // 0x6E0
    void* opt8;              // 0x6E4
    void* opt6;              // 0x6E8
    void* opt7;              // 0x6EC
};

// Message handler for the race-options screen.
extern "C" void func_001AE7A0(cFEStateChallengePopup* self, void* a1, int msg)
{
    if (a1 == 0) {
        return;
    }
    switch (msg) {
    case 5:
        switch (*(int*)((char*)a1 + 0x18)) {
        case 7: {
            func_001AE680_E7A0(((cState_E7A0*)self));
            cUITemplate_MAP_mPeak = 1;
            D_004A2594 = 0;
            D_004A2598 = 0;
            char* p = (char*)func_00186610_E7A0(cMemMan_alloc(0x33C, D_00460C38, 0x100, 0), *(int*)(char**)&self->mpEngine, 1);
            *(int*)(p + 0x1C) = (*(int*)(p + 0x1C) & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial_E7A0((char*)self->mpEngine + 0x18, p, 0, 0);
            break;
        }
        case 9: {
            if (self->mbChallengeSent) {
                break;
            }
            func_001AE680_E7A0(((cState_E7A0*)self));
            func_001AF568_E7A0(((cState_E7A0*)self));
            char* opts = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
            D_00534B34[0] = func_0039A738_E7A0(self->mpLBRankedGame) != 0;
            commit_E7A0(opts);
            if (func_0039A738_E7A0(self->mpLBRankedGame)) {
                D_005308B8_E7A0.bit6 = 1;
                D_005308B8_E7A0.bit7 = 1;
                D_005308B8_E7A0.bit8 = 1;
                char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
                cBENewRaceInterface_setNumberAI_E7A0(race, 0);
                commit_E7A0(race);
            } else {
                D_005308B8_E7A0.bit6 = func_0039A738_E7A0(self->mpLBMultipliers);
                D_005308B8_E7A0.bit8 = func_0039A738_E7A0(self->mpLBPowerUps);
                D_005308B8_E7A0.bit7 = func_0039A738_E7A0(self->mpLBPointIcons);
                char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
                cBENewRaceInterface_setNumberAI_E7A0(race, func_0039A738_E7A0(self->mpLBCPURiders));
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
        if (a1 == (void*)self->mpLBRankedGame) {
            char* opts = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
            D_00534B34[0] = func_0039A738_E7A0(self->mpLBRankedGame) != 0;
            commit_E7A0(opts);
            func_001AF098_E7A0(((cState_E7A0*)self), func_0039A738_E7A0(self->mpLBRankedGame) != 0);
            func_001AEC60_E7A0(((cState_E7A0*)self), self->mbChallengeSent);
        } else if (a1 == (void*)self->mpLBMultipliers) {
            D_005308B8_E7A0.bit6 = func_0039A738_E7A0(a1);
        } else if (a1 == (void*)self->mpLBPowerUps) {
            D_005308B8_E7A0.bit8 = func_0039A738_E7A0(a1);
        } else if (a1 == (void*)self->mpLBPointIcons) {
            D_005308B8_E7A0.bit7 = func_0039A738_E7A0(a1);
        } else if (a1 == (void*)self->mpLBCPURiders) {
            char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
            cBENewRaceInterface_setNumberAI_E7A0(race, func_0039A738_E7A0(self->mpLBCPURiders));
            D_004A4E88_w[1] = func_0039A738_E7A0(self->mpLBCPURiders);
            commit_E7A0(race);
        }
        break;
    case 1:
        func_001AF208_E7A0(((cState_E7A0*)self), a1, 1);
        break;
    case 2:
        func_001AF208_E7A0(((cState_E7A0*)self), a1, 0);
        break;
    case 7:
        break;
    default:
        func_001A8918((cFEStateNet*)self, (int)a1, msg);
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
extern char D_004632A8[];
extern char D_004632B8[];
extern char D_004632D0[];
extern char D_004632E0[];
extern char D_004632F0[];
extern char D_00463130[];

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
    cFEVObj_001AEC60* o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004632A8));
    if (o != 0) {
        o->v08(on);
    }
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004632B8));
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
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004632D0));
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
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004632E0));
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
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004632F0));
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
    o = (cFEVObj_001AEC60*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463130));
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
extern "C" void func_0039A7A8(void* self, int a1);

extern "C" void func_001AF098(cFEStateChallengePopup* self, int all)
{
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
    if (all) {
        func_0039A7A8(self->mpLBPowerUps, 1);
        func_0039A7A8(self->mpLBMultipliers, 1);
        func_0039A7A8(self->mpLBPointIcons, 1);
        func_0039A7A8(self->mpLBCPURiders, 0);
        return;
    }
    if (*(int*)((char*)self->mpEventOptions + 0x4) != 0) {
        func_0039A7A8(self->mpLBPowerUps, (D_005308D0[0] >> 8) & 1);
    } else {
        func_0039A7A8(self->mpLBPowerUps, 1);
    }
    if (*(int*)((char*)self->mpEventOptions + 0xC) != 0) {
        func_0039A7A8(self->mpLBMultipliers, (D_005308D0[0] >> 6) & 1);
    } else {
        func_0039A7A8(self->mpLBMultipliers, 1);
    }
    if (*(int*)((char*)self->mpEventOptions + 0x8) != 0) {
        int f = D_005308D0[0] >> 7;
        func_0039A7A8(self->mpLBPointIcons, f & 1);
    } else {
        func_0039A7A8(self->mpLBPointIcons, 1);
    }
    if (D_00535C12[0] != 0) {
        cBENewRaceInterface_setNumberAI(iface, 0);
        sVE1ADE88i* ivt = *(sVE1ADE88i**)(iface + 0xC);
        ivt[1].fn(iface + ivt[1].delta);
        func_0039A7A8(self->mpLBCPURiders, 0);
    } else {
        func_0039A7A8(self->mpLBCPURiders, D_004A4E88_AF098[4]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF208);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char D_004631F8[];
extern char D_00463210[];
extern char D_00463238[];
extern char D_00463248[];
extern char D_00463260[];
extern char D_00463300[];
extern char D_00463310[];
extern char D_00463320[];
extern char D_00463330[];
extern char D_00463340[];
extern char D_00463350[];
extern char D_00463360[];
extern char D_00463370[];
extern char D_00463380[];
extern char D_00463390[];

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
    if (IsHash1AF208(*(int*)((char*)item + 0x38), D_004631F8)) {
        a = Get1AF208(((cFEStateNet*)self), D_00463300);
        b = Get1AF208(((cFEStateNet*)self), D_00463310);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), D_00463210)) {
        a = Get1AF208(((cFEStateNet*)self), D_00463320);
        b = Get1AF208(((cFEStateNet*)self), D_00463330);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), D_00463238)) {
        a = Get1AF208(((cFEStateNet*)self), D_00463340);
        b = Get1AF208(((cFEStateNet*)self), D_00463350);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), D_00463248)) {
        a = Get1AF208(((cFEStateNet*)self), D_00463360);
        b = Get1AF208(((cFEStateNet*)self), D_00463370);
    } else if (IsHash1AF208(*(int*)((char*)item + 0x38), D_00463260)) {
        a = Get1AF208(((cFEStateNet*)self), D_00463380);
        b = Get1AF208(((cFEStateNet*)self), D_00463390);
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

extern "C" int func_001AF3E0(cFEStateChallengePopup* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        return func_0039F698((char*)self->mpEngine + 0x18);
    }
    return func_001A97B8(((cFEStateNet*)self), a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AF428);
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

extern "C" void func_001AF428(cFEStateChallengePopup* self, void* a1, int a2)
{
    if (*(int*)((char*)a1 + 0x18) == 0x835) {
        ((cFEVObj_001AF428*)self)->v33(a1, a2);
    } else {
        func_001A97B8(((cFEVObj_001AF428*)self), a1, a2);
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
extern const char D_00461C60[];

// Message handler: opens the next screen, shows error popups, or forwards to the base handler.
extern "C" void* func_001AF470(cFEStateChallengePopup* self, int a1, int a2)
{
    switch (a1) {
    case 0xF3:
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), (int)self->mpEngine));
        break;
    case 0xED: case 0xEE: case 0xF0: case 0xF1: case 0xF2: case 0x10C: case 0x112: case 0x113:
        func_001AF688(self);
        func_001AB178(((cFEStateNet*)self), a1);
        break;
    case 0x110:
        func_001AB108(((cFEStateNet*)self));
        break;
    case 0xE3:
        func_001AFA60(self->mpScreen, (char*)a2);
        break;
    case 0xE5:
        func_001AFD88(self->mpScreen, (void*)a2);
        break;
    default:
        return func_001A8E40_3(((cFEStateNet*)self), a1, a2);
    case 0xEF:
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
extern char D_00463468[];
extern char D_00463480[];
extern char D_004631C8[];
extern char D_004630E0[];

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
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463468));
    if (text != 0) {
        char* name = LobbyMan_mLobbyMan->mOpponent;
        char* o = (char*)SSXApp->FELocale;
        sVE_001AF568* vt = *(sVE_001AF568**)(o + 4);
        char* obj = o + vt[4].delta;
        sprintf1(buf, vt[4].fn(obj, GetHashValue32(D_00463480)), name);
        cUIText_setUnicodeString(text, buf);
        char* o2 = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004631C8));
        sVE_001AF568b* vt2 = *(sVE_001AF568b**)(o2 + 8);
        vt2[8].fn(o2 + vt2[8].delta, 1);
        cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004630E0)), 7);
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
extern char D_00463468[];
extern char D_00463498[];
extern char D_004631C8[];

extern "C" void func_001AF688(cFEStateChallengePopup* self)
{
    self->mbChallengeSent = 0;
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463468));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00463498));
        ((cUIObj_001AF688*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004631C8)))->v08(0);
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
extern "C" void func_0039A7A8(void* self, int a1);
extern signed char D_00535C12[];
extern char D_004634B8[];
extern char D_004634C8[];
extern char D_004634D8[];
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
    cUIText* t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004634B8));
    if (track == 0) {
        cUIText_setAsciiString(t, D_004A1390);
    } else {
        switch (func_00144C98(track)) {
        case 0:
            cUIText_setAsciiString(t, D_004A14F8);
            break;
        case 1:
            cUIText_setAsciiString(t, D_004A1500);
            break;
        case 2:
            cUIText_setAsciiString(t, D_004A1508);
            break;
        }
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004634C8));
    if (track == 0) {
        cUIText_setAsciiString(t, D_004A1390);
    } else {
        if (D_00535C12[0] != 0) {
            cRaceIface_001AF728* iface = (cRaceIface_001AF728*)cBE_getInterface_Fv(cBE_getBE(), 0);
            cBENewRaceInterface_setNumberAI(iface, 0);
            iface->v01();
            func_0039A7A8(self->mpLBCPURiders, 0);
            ((cFEVObj_001AF728*)self->mpLBCPURiders)->v08(1);
        }
        cUIText_setAsciiString(t, func_00145340(track));
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004634D8));
    if (track == 0) {
        cUIText_setAsciiString(t, D_004A1390);
    } else {
        cUIText_setAsciiString(t, func_00144BF0(track));
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
extern char D_00463108[];
extern char D_004634E8[];
extern char D_004634F8[];
extern char D_00463508[];
extern char D_00463520[];
extern char D_00463538[];

static inline void Clear1AF930(void* screen, char* name)
{
    cUIText* t = cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
    if (t != 0) {
        cUIText_setAsciiString(t, D_004A1390);
    }
}

extern "C" void func_001AF930(void* screen)
{
    if (screen != 0) {
        Clear1AF930(screen, D_00463108);
        Clear1AF930(screen, D_004634E8);
        Clear1AF930(screen, D_004634F8);
        Clear1AF930(screen, D_00463508);
        Clear1AF930(screen, D_00463520);
        Clear1AF930(screen, D_00463538);
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
// PORT: func_002C2718 is a varargs formatter (buf, fmt, ...); the unit declares a 3-arg view
extern "C" void func_002C2718_1AFA60(char* buf, const char* fmt, ...) __asm__("func_002C2718");
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
// PORT: SN soft-float float->double helper called by name
extern "C" double fptodp(float f);
extern "C" void func_0025DD08(void* self, const char* str, int a2);
extern char D_00463108[];
extern char D_004634E8[];
extern char D_004634F8[];
extern char D_00463508[];
extern char D_00463520[];
extern char D_00463550[];
extern char D_00463568[];
extern char D_00463580[];
extern char D_00463590[];
extern char D_004635A8[];
extern char D_004A1A88[];

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
    sBXString1AFA60 s;
    char buf[0x100];
    sStats1AFA60* st = (sStats1AFA60*)(msg + 0x18);
    cBXString_cBXString1(&s, (char*)LobbyMan_mLobbyMan + 0x98);
    (*(sStats1AFA60*)&LobbyMan_mLobbyMan->mOpponentStats) = *(sStats1AFA60*)(msg + 0x18);
    func_0025DD08(LobbyMan_mLobbyMan, s.str, *(int*)(msg + 0x14));
    cUIText* t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463108));
    if (t != 0) {
        cUIText_setAsciiString(t, s.str);
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004634E8));
    if (t != 0) {
        int name = Loc1AFA60(D_00463550);
        func_002C2718_1AFA60(buf, D_004A1A88, name, *(int*)(msg + 0x18));
        cUIText_setUnicodeString(t, buf);
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004634F8));
    if (t != 0) {
        int name = Loc1AFA60(D_00463568);
        func_002C2718_1AFA60(buf, D_004A1A88, name, *(int*)(msg + 0x20));
        cUIText_setUnicodeString(t, buf);
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463508));
    if (t != 0) {
        int b = *(int*)(msg + 0x20);
        int a = *(int*)(msg + 0x1C);
        float pct = 0.0f;
        if (a < b) {
            pct = (float)a / (float)b * 100.0f;
        } else if (a != 0) {
            pct = 100.0f;
        }
        int name = Loc1AFA60(D_00463580);
        func_002C2718_1AFA60(buf, D_00463590, name, st->b, st->c, fptodp(pct));
        cUIText_setUnicodeString(t, buf);
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463520));
    if (t != 0) {
        int name = Loc1AFA60(D_004635A8);
        func_002C2718_1AFA60(buf, D_004A1A88, name, st->d);
        cUIText_setUnicodeString(t, buf);
    }
    cBXString__cBXString(&s, 2);
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
extern char D_00463538[];
extern char D_004635C0[];

extern "C" void func_001AFD88(void* self, void* a1)
{
    cUIText* text = cUIScreen_getObjectByHashName(self, GetHashValue32(D_00463538));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004635C0));
        func_001AAD50(self, 0, *(int*)((char*)a1 + 0xC));
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

extern "C" void* cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup(cFEStateChallengeReceivedPopup* self, int a1, int a2, const char* name)
{
    func_001A8500_3(self, a1, a2);
    self->mVtbl = cFEStateChallengeReceivedPopup_vtbl;
    cBXString_cBXString2((char*)self + 0x6D0, name);
    self->mbAccepted = 0;
    self->mHashName = 0x40;
    D_004A4E88 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001AFE68);
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
extern char D_004635D8[];

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

// Creates the race-setup screen and applies the selected online race settings.
extern "C" void func_001AFE68(cFEStateChallengeReceivedPopup* self)
{
    sVE_FE68* vt = (sVE_FE68*)self->mVtbl;
    vt[32].fn(((char*)self) + vt[32].delta);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((char*)self), GetHashValue32(D_004635D8), 0);
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
    sRaceSetup_FE68* r = (sRaceSetup_FE68*)func_00263270(BuddyMan_mBuddyMan, *(int*)name);
    cBXString_operatorE((char*)LobbyMan_mLobbyMan + 0x98, name);
    (*(sInfo_FE68*)&LobbyMan_mLobbyMan->mOpponentStats) = *(sInfo_FE68*)r->info;
    if (r == 0)
        return;
    cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x8C, r->name);
    MEM_fill(&D_005308B8_FE68, 0, 0x20);
    D_005308B8_FE68.b6 = r->f18;
    D_005308B8_FE68.b7 = r->f24;
    D_005308B8_FE68.b8 = r->f1C;
    void* be = cBE_getInterface_Fv_FE68(cBE_getBE(), 0);
    D_00535C08[0] = r->f8;
    D_004A11B8 = 1;
    cBENewRaceInterface_setGameMode_FE68(be, r->mode);
    cBENewRaceInterface_setGameEvent_FE68(be, r->event);
    cBENewRaceInterface_setNumberAI(be, r->numAI);
    vcall_FE68(be);
    void* net = cBE_getInterface_Fv_FE68(SSXApp->BE, 7);
    D_00534B34[0] = r->f14;
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
INCLUDE_ASM("fe/feasyncfile", func_001B0130__FPv);
#ifdef SKIP_ASM
void* func_001B0130(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0150);
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
extern char D_004A12C0[];
extern char D_004A1A90[];
extern char D_00461520[];
extern char D_004635F8[];
extern char D_00463608[];
extern char D_00463618[];
extern char D_00463108[];
extern char D_00463118[];
extern char D_00463140[];
extern char D_00463150[];
extern char D_00463168[];
extern char D_00463178[];
extern char D_00463190[];
extern char D_004631A0[];
extern char D_004631B8[];
extern char D_00463628[];
extern char D_00463640[];
extern char D_00463228[];
extern char D_00463658[];
extern char D_00463670[];
extern char D_00463688[];
extern char D_004636A0[];
extern char D_004636B0[];
extern char D_004630E0[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

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

extern "C" void func_001B0150(cFEStateChallengeReceivedPopup* self, cFEVObj_001B0150* obj)
{
    char buf[16];
    char* o = (char*)obj;
    cUIText* t = (cUIText*)obj;
    if (IsHash_001B0150(*(int*)(o + 0x38), D_00461520)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_004635F8)) {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00463608));
        *(int*)(o + 0x18) = 0xB;
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463618)) {
        *(int*)(o + 0x18) = 0xC;
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463108)) {
        cUIText_setAsciiString(t, ((const char*)LobbyMan_mLobbyMan->mOpponent));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463118)) {
        *(int*)(o + 0x18) = 7;
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463140)) {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00463150));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463168)) {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00463178));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463190)) {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_004631A0));
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_004631B8)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463628)) {
        cUIText_setAsciiString(t, D_004A1A90);
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463640)) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        int n = D_00535C04[0];
        if (n == 0) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_00463228));
        } else {
            sprintf(buf, D_004A12C0, n);
            cUIText_setAsciiString(t, buf);
        }
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463658)) {
        if ((D_005308D0[0] >> 8) & 1) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045FCD8));
        } else {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045DCC8));
        }
        cUIText_setAsciiString(t, D_004A1A90);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463670)) {
        if ((D_005308D0[0] >> 6) & 1) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045FCD8));
        } else {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045DCC8));
        }
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463688)) {
        int f = D_005308D0[0] >> 7;
        if (f & 1) {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045FCD8));
        } else {
            cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045DCC8));
        }
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_004636A0)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_004636B0)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463628)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_00463640)) {
        obj->v09(1);
    } else if (IsHash_001B0150(*(int*)(o + 0x38), D_004630E0)) {
        *(int*)(o + 0x14) |= 0x80;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0538);
#ifdef SKIP_ASM
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001B0C08(cFEStateChallengeReceivedPopup* self);
extern "C" void func_001B0E10(cFEStateChallengeReceivedPopup* self);
extern "C" void func_0025FB58(void* self, int a1, int a2);

extern "C" void func_001B0538(cFEStateChallengeReceivedPopup* self, void* obj, int msg)
{
    if (obj == 0) {
        return;
    }
    switch (msg) {
    case 5:
        switch (*(int*)((char*)obj + 0x18)) {
        case 0xB:
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
        // PORT: the unit declares func_001A8918 with an int a1; obj is passed through as int
        func_001A8918((cFEStateNet*)self, (int)obj, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B05E0);
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

extern "C" void func_001B05E0(cFEStateChallengeReceivedPopup* self, void* a1, int a2)
{
    if (*(int*)((char*)a1 + 0x18) == 0x835) {
        ((cFEVObj_001B05E0*)self)->v33(a1, a2);
    } else {
        func_001A97B8(((cFEVObj_001B05E0*)self), a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0628);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_0039F698(void* p);

extern "C" int func_001B0628(cFEStateChallengeReceivedPopup* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        return func_0039F698((char*)self->mpEngine + 0x18);
    }
    return func_001A97B8(((cFEStateNet*)self), a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0670);
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

extern "C" int func_001B0670(cFEStateChallengeReceivedPopup* self, int a1)
{
    ((cFEVObj_001B0670*)self)->v32();
    if (a1 != 0) {
        func_001B0850(self, cBE_getInterface_Fv(cBE_getBE(), 0), self->mpScreen);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B06E0);
#ifdef SKIP_ASM
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int func_001B06E0(cFEStateChallengeReceivedPopup* self, int a1, unsigned int key)
{
    int r = 0x101;
    if (key < 10) {
        if (key < 8) {
            r = 0x101;
        } else {
            return 0x100;
        }
    }
    return r;
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
extern const char D_00461C60[];

// Message handler: starts the memory-card task, shows error popups, or forwards to the base handler.
extern "C" void* func_001B0708(cFEStateChallengeReceivedPopup* self, int a1, int a2)
{
    switch (a1) {
    case 0xEF:
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
    case 0x110:
        func_001AB108((cFEStateNet*)self);
        break;
    case 0xED: case 0xEE: case 0xF0: case 0xF1: case 0x10C: case 0x112: case 0x113:
        func_001B0C08(self);
        func_001AB178((cFEStateNet*)self, a1);
        break;
    case 0xF3:
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), (int)self->mpEngine));
        break;
    case 0xE3:
        func_001AFA60(self->mpScreen, (char*)a2);
        break;
    case 0xE5:
        func_001AFD88(self->mpScreen, (void*)a2);
        break;
    default:
        return func_001A8E40_3(self, a1, a2);
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
extern char D_004A12C0[];
extern char D_004636A0[];
extern char D_004636B0[];
extern char D_00463628[];
extern char D_00463798[];
extern char D_00463670[];
extern char D_00463688[];
extern char D_00463640[];
extern char D_00463228[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

extern "C" void func_001B0850(cFEStateChallengeReceivedPopup* self, void* iface, void* screen)
{
    char buf[16];
    if (screen == 0) {
        return;
    }
    cUIText* t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004636A0));
    if (t != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(t, D_004A1390);
        } else {
            cUIText_setAsciiString(t, func_00145340(iface));
        }
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_004636B0));
    if (t != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(t, D_004A1390);
        } else {
            cUIText_setAsciiString(t, func_00144BF0(iface));
        }
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463628));
    if (t != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(t, D_004A1390);
        } else {
            cBE_getInterface_Fv(SSXApp->BE, 7);
            if (D_00534B34[0] != 0) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045DCC8));
            } else {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045FCD8));
            }
        }
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463798));
    if (t != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(t, D_004A1390);
        } else {
            if ((D_005308D0[0] >> 8) & 1) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045FCD8));
            } else {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045DCC8));
            }
        }
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463670));
    if (t != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(t, D_004A1390);
        } else {
            if ((D_005308D0[0] >> 6) & 1) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045FCD8));
            } else {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045DCC8));
            }
        }
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463688));
    if (t != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(t, D_004A1390);
        } else {
            int f = D_005308D0[0] >> 7;
            if (f & 1) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045FCD8));
            } else {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_0045DCC8));
            }
        }
    }
    t = cUIScreen_getObjectByHashName(screen, GetHashValue32(D_00463640));
    if (t != 0) {
        if (iface == 0) {
            cUIText_setAsciiString(t, D_004A1390);
        } else {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            int n = D_00535C04[0];
            if (n <= 0) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_00463228));
            } else {
                sprintf(buf, D_004A12C0, n);
                cUIText_setAsciiString(t, buf);
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
extern char D_00463618[];
extern char D_004630E0[];
extern char D_00463468[];
extern char D_00462750[];
extern char D_004635F8[];

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
        cFEVObj_001B0C08* o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463618));
        if (o != 0) {
            o->v07(1);
        }
        cUIText* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004630E0));
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, 1);
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463468));
        if (o != 0) {
            o->v09(1);
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(D_00462750));
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004635F8));
        if (o != 0) {
            o->v08(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0D30);
#ifdef SKIP_ASM
extern char D_004637B0[];

extern "C" void func_001B0D30(cFEStateChallengeReceivedPopup* self)
{
    self->mbAccepted = 0;
    if (self->mpScreen != 0) {
        cFEVObj_001B0C08* o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004635F8));
        if (o != 0) {
            o->v08(0);
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463468));
        if (o != 0) {
            o->v09(0);
        }
        o = (cFEVObj_001B0C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463618));
        if (o != 0) {
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(D_004637B0));
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
    vt[17].fn(obj + vt[17].delta, self, 0x14);
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

extern "C" void* cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(cFEStateOnlineCharacterSelect* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->mVtbl = cFEStateOnlineCharacterSelect_vtbl;
    self->mbConfirmed = 1;
    self->mHashName = 0x41;
    self->miConfirmCtrl = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B0FB0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001B1B98(cFEStateOnlineCharacterSelect* self, int index, int a2, int a3);
extern char D_00463800[];

extern "C" void func_001B0FB0(cFEStateOnlineCharacterSelect* self)
{
    int i;
    self->miCharID = 0;
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00463800), 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001B1048__FPv);
#ifdef SKIP_ASM
void* func_001B1048(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1068);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_0039A738(void* a);
extern "C" void func_001B1B10(cFEStateOnlineCharacterSelect* self, int sel, int flag);
extern "C" void func_001B1760(cFEStateOnlineCharacterSelect* self, int index);
extern "C" void func_001B1808(cFEStateOnlineCharacterSelect* self);

extern "C" int func_001B1068(cFEStateOnlineCharacterSelect* self, int a1)
{
    if (a1 != 0) {
        func_001B1B10(self, func_0039A738(self->mpLBChallengerSelect), 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001B10F8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern "C" void* func_0014EEC8(void* self, int player, int index);
extern "C" int func_001B0F28(int c);
extern "C" void func_0039A7A8(void* self, int a1);
extern signed char D_00440F68[];
extern char D_00463818[];
extern char D_00463830[];
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

extern "C" void func_001B10F8(cFEStateOnlineCharacterSelect* self, cUIText* text)
{
    cFEVObj_001B10F8* box = (cFEVObj_001B10F8*)text;
    if (IsHash_001B10F8(*(int*)((char*)text + 0x38), D_00463818)) {
        *(int*)((char*)text + 0x14) &= ~1;
        int i;
        for (i = 0; i < 10; i++) {
            cUIListBox_addEntryByAsciiString(text, (const char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), D_00440F68[i], 0), D_00440F68[i]);
        }
        *(int*)((char*)text + 0x14) &= ~1;
        box->v09(1);
        int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(SSXApp->BE, 1), 0);
        self->miCharID = id;
        func_0039A7A8(text, (unsigned char)func_001B0F28(id));
        self->mpLBChallengerSelect = (cUIListBox*)text;
        box->v24(1);
    } else if (IsHash_001B10F8(*(int*)((char*)text + 0x38), D_00463830)) {
        cUIText_setAsciiString(text, D_004A1390);
        box->v11(sVec4_001B10F8(1.0f, 0.0f, 0.0f, 0.0f));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1280);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001474A8(void* iface, int a1, int a2);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001B1A08(cFEStateOnlineCharacterSelect* self, int on, int a2);
extern "C" void func_001B1B10(cFEStateOnlineCharacterSelect* self, int sel, int flag);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_00264F88(void* self, int a1);
extern "C" void func_00265018(void* self, int a1);
extern "C" void func_00265140(void* self);
extern "C" int func_0039A738(void* a);
extern void* D_004A33CC;
extern char D_00463818[];
extern char D_0045FD98[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

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

// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void func_001B1280(cFEStateOnlineCharacterSelect* self, int a1, unsigned int msg)
{
    char* obj = (char*)a1;
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
        if (self->miCharID != func_0039A738(self->mpLBChallengerSelect)) {
            char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 1);
            func_001474A8(iface, 0, 0);
            sVE1B1280* vt = *(sVE1B1280**)(iface + 0xC);
            vt[1].fn(iface + vt[1].delta);
        }
        {
            void* mgr = D_004A33CC;
            func_00265018(mgr, func_0039A738(self->mpLBChallengerSelect));
        }
        self->miConfirmCtrl = self->mCurrentCtrl;
        ((cFEVObj_001B1280*)obj)->v08(1);
        func_001B1A08(self, 1, 0);
        self->mbConfirmed = 1;
        break;
    case 6:
        if (self->mbConfirmed == 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 2;
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_0045FD98);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
            func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
            *(int*)((char*)p + 0x150) = 0x838;
            *(int*)((char*)p + 0x168) = 1;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            func_00265140(D_004A33CC);
            ((cFEVObj_001B1280*)obj)->v08(0);
            func_001B1A08(self, 0, 0);
            self->mbConfirmed = 0;
        }
        break;
    case 9:
        if (IsHash1B1280(*(int*)(obj + 0x38), D_00463818)) {
            void* mgr = D_004A33CC;
            func_00264F88(mgr, func_0039A738(self->mpLBChallengerSelect));
            func_001B1B10(self, func_0039A738(self->mpLBChallengerSelect), 0);
        }
    default:
        func_001A8918((cFEStateNet*)self, a1, msg);
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
extern char D_00463848[];
extern char D_00463868[];
extern char D_00463890[];
extern char D_004638B0[];
extern char D_004614E8[];

extern "C" void func_001B14D8(cFEStateOnlineCharacterSelect* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00463848);
    func_001DA530(popup, 0, GetHashValue32(D_00463868));
    func_001DA510(popup, 0, GetHashValue32(D_00463890), 4);
    func_001DA510(popup, 1, GetHashValue32(D_004638B0), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B1590);
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

extern "C" bool func_001B1590(cFEStateOnlineCharacterSelect* self, cFEVObj_001B1590* obj)
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
extern const char D_0045FF10[];
extern const char D_0045FF28[];
extern const char D_00460CC8[];

// Message handler for the profile list: select/scroll, error popups, and opening the follow-up screens.
extern "C" void* func_001B15D8(cFEStateOnlineCharacterSelect* self, int a1, int a2)
{
    void* obj;
    switch (a1) {
    case 0xF3:
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cBENetworkInterface_networkOpt.OpponentCharacterID >= 0) {
            func_001B1760(self, cBENetworkInterface_networkOpt.OpponentCharacterID);
            func_001B1B10(self, cBENetworkInterface_networkOpt.OpponentCharacterID, 1);
        }
        break;
    case 0xF5:
        func_001B1A08(self, 1, 1);
        break;
    case 0xF7:
        func_001B1A08(self, 0, 1);
        break;
    case 0xEE: case 0xF1: case 0xF8: case 0xFA: case 0x112: case 0x113:
        func_001AB288(((cFEStateNet*)self), a1);
        break;
    case 0xF6:
        func_001A8A40(((cFEStateNet*)self), func_00192D00(cMemMan_alloc(0x73C, D_00460CC8, 0, 0), (int)self->mpEngine));
        break;
    case 0x10C:
        if (func_0025DA90(LobbyMan_mLobbyMan) != 0 && D_004A4E88 == 0) {
            obj = cFEStateChatRoom_cFEStateChatRoom((sFEObj_001B1F88*)cMemMan_alloc(0x73C, D_0045FF10, 0, 0), (int)self->mpEngine);
        } else {
            obj = cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), (int)self->mpEngine);
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
extern char D_00463830[];

extern "C" void func_001B1760(cFEStateOnlineCharacterSelect* self, int player)
{
    if (self->mpScreen != 0) {
        const char* str = (const char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), player, 0);
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463830));
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
// PORT: func_002C2718 is a varargs formatter (buf, fmt, ...); the unit declares a 3-arg view
extern "C" void func_002C2718_1B1808(char* buf, const char* fmt, ...) __asm__("func_002C2718");
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern char D_00463108[];
extern char D_004639A0[];
extern char D_004634E8[];
extern char D_004639B8[];
extern char D_00463550[];
extern char D_004A1A88[];

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
    cFEVObj_001B1808* o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463108));
    if (o != 0) {
        cUIText_setAsciiString((cUIText*)o, ((const char*)LobbyMan_mLobbyMan->mPersona));
        o->v09(1);
    }
    o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004639A0));
    if (o != 0) {
        cUIText_setAsciiString((cUIText*)o, ((const char*)LobbyMan_mLobbyMan->mOpponent));
        o->v09(1);
    }
    o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004634E8));
    if (o != 0) {
        LobbyMan* mgr = LobbyMan_mLobbyMan;
        int name = Loc1B1808(D_00463550);
        func_002C2718_1B1808(buf, D_004A1A88, name, mgr->mStats.mRank);
        cUIText_setUnicodeString((cUIText*)o, buf);
        o->v09(1);
    }
    o = (cFEVObj_001B1808*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004639B8));
    if (o != 0) {
        LobbyMan* mgr = LobbyMan_mLobbyMan;
        int name = Loc1B1808(D_00463550);
        func_002C2718_1B1808(buf, D_004A1A88, name, mgr->mOpponentStats.mRank);
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
extern char D_004639D0[];
extern char D_004639E0[];
extern char D_00463830[];

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
            t = (sText1B1A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004639D0));
            cFEVObj_001B1A08* o = (cFEVObj_001B1A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463830));
            o->v11(sColor1B1A08(1.0f, 0.0f, 0.0f, 0.0f));
        } else {
            t = (sText1B1A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004639E0));
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
extern "C" void func_0039C428(void* self, int v);
extern char* D_004412C8[];
extern char D_004A19F8[];
extern char D_004A1B18[];
extern char D_004A1B20[];
extern char D_004A12C0[];
extern char D_00441340[];
extern char D_00441350[];
extern char D_00441360[];
extern char D_00441378[];
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
        sprintf(buf, D_004A19F8, D_004412C8[index], D_004A1B18);
    } else {
        sprintf(buf, D_004A19F8, D_004412C8[index], D_004A1B20);
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
        name1 = D_00441350;
        name2 = D_00441378;
        unsigned char* src = D_00534FD8;
        for (int j = 0; j < 7; j++) {
            stats[j] = src[j];
        }
    } else {
        name1 = D_00441340;
        name2 = D_00441360;
        if (D_00534B34[0] != 0) {
            int v = 0x28;
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
            sprintf(num, D_004A12C0, q);
            a->v09(1);
            cUIText_setAsciiString((cUIText*)a, num);
            *(int*)((char*)b + 0x80) = 0x37;
            func_0039C428(b, v);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B1EC0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B1F18(cFEStateOnlineCharacterSelect* self, void* a1, int a2);

extern "C" void func_001B1EC0(cFEStateOnlineCharacterSelect* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x838:
        func_001B1F18(self, a1, a2);
        break;
    case 0x835:
        func_001A94D8(self);
        break;
    default:
        func_001A97B8(self, a1, a2);
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
            func_00265768(D_004A33CC, 0xF9);
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

extern "C" sFEObj_001B1F88* cFEStateChatRoom_cFEStateChatRoom(sFEObj_001B1F88* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->vt = cFEStateChatRoom_vtbl;
    self->fC = 0x3D;
    self->f6F8 = 0;
    for (int i = 0; i < 2; i++) {
        self->items[i] = 0;
        self->items2[i] = 0;
    }
    self->f6EC = 0;
    self->f6F0 = 0;
    self->f6F4 = 0;
    sCol_001B1F88 c;
    c.r = 1.0f;
    c.g = 1.0f;
    c.b = 1.0f;
    c.a = 1.0f;
    self->color = c;
    strcpy(self->name, ((const char*)LobbyMan_mLobbyMan->mPersona));
    return self;
}
#endif

extern void* cFEStateChatRoom_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateChatRoom__cFEStateChatRoom__FPv);
#ifdef SKIP_ASM
void* cFEStateChatRoom__cFEStateChatRoom(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateChatRoom_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2070);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001B2378(cFEStateChatRoom* self, int a1);
extern "C" void func_001B3978(cFEStateChatRoom* self, void* a1);
extern "C" void func_003A1310(void* self, int a1);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern char D_00463A08[];
extern char D_004619C8[];
extern char D_004619D8[];
extern char D_004619E8[];
extern char D_004A1B48[];
extern char D_004A1B50[];
extern char D_004A1B58[];
extern char D_00463A18[];
extern char D_00463A28[];

struct sBlk1B2070 {
    int v[4];
};

extern "C" void func_001B2070(cFEStateChatRoom* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00463A08), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen == 0) {
        return;
    }
    cUIScreen_playFrame(screen, 0, 0);
    func_001B2378(self, 1);
    func_001B3978(self, LobbyMan_mLobbyMan->mPersona);
    *(int*)((char*)self->mpTextScrollChat + 0x74) |= 1;
    func_003A1310(self->mpTextScrollChat, 1);
    *(sBlk1B2070*)((char*)self->mpTextScrollChat + 0xA8) = *(sBlk1B2070*)&self->mChatColour;
    *(void**)((char*)self->mpTextScrollChat + 0xB8) = (void*)self->mpSlider2;
    cUIText* a = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619C8));
    cUIText* b = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619D8));
    cUIText* c = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619E8));
    *(cUIText**)((char*)self->mpSlider2 + 0x7C) = a;
    *(cUIText**)((char*)self->mpSlider2 + 0x80) = b;
    {
        char* o = (char*)self->mpSlider2;
        if ((unsigned int)strlen(D_004A1B48) > 24) {
            o[0x8C] = 0;
        } else {
            strcpy(o + 0x8C, D_004A1B48);
        }
    }
    {
        char* o = (char*)self->mpSlider2;
        if ((unsigned int)strlen(D_004A1B50) > 24) {
            o[0xA5] = 0;
        } else {
            strcpy(o + 0xA5, D_004A1B50);
        }
    }
    int two = 2;
    *(cUIText**)((char*)self->mpSlider2 + 0x84) = c;
    *(int*)((char*)self->mpSlider2 + 0x88) = two;
    cUIText* d = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1B58));
    cUIText* e = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463A18));
    cUIText* f = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463A28));
    *(cUIText**)((char*)self->mpSlider1 + 0x7C) = d;
    *(cUIText**)((char*)self->mpSlider1 + 0x80) = e;
    *(cUIText**)((char*)self->mpSlider1 + 0x84) = f;
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
INCLUDE_ASM("fe/feasyncfile", func_001B2320);
#ifdef SKIP_ASM
extern void* D_004A5B64;
extern "C" void func_001B3BF0(cFEStateChatRoom* self);
extern "C" void* func_001A8770(cFEStateNet* self);

extern "C" void func_001B2320(cFEStateChatRoom* self)
{
    if (*(int*)((char*)D_004A5B64 + 0x18) % 10000 == 0) {
        func_001B3BF0(self);
    }
    func_001A8770((cFEStateNet*)self);
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
extern char* D_004A3E90_p __asm__("D_004A3E90");
extern char D_00463A38[];
extern char D_00463A60[];
extern char D_00461B30[];
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
    cUIText* menu = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463A38));
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
        sprintf(name.c, D_00463A60, i);
        cFEVObj_001B2378* t = (cFEVObj_001B2378*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(name.c));
        t->v11(sCol_001B2378(1.0f, 0.0f, 0.0f, 0.0f));
        if (idx < n) {
            cBXString_cBXString4(&s, func_0025F1E0_p(LobbyMan_mLobbyMan, idx, &flag));
            if (flag != 0) {
                char buf[256];
                func_001B25E0(func_0025DA90_s(LobbyMan_mLobbyMan), buf);
                if (func_0025DA90(LobbyMan_mLobbyMan) == 0) {
                    func_001A8A40(((char*)vself), cFEStateLobbyMenu_cFEStateLobbyMenu((sFEObj_001BCB58*)cMemMan_alloc(0xB84, D_00461B30, 0, 0), (int)vself->mpEngine));
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
extern char D_00463A78[];
extern char D_00463A90[];
extern char D_00463AA8[];
extern char D_00463AC0[];

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
        strcpy1(out, Loc1B25E0(D_00463A78));
    } else if (func_004165A8(name, D_004A1A64) == 0) {
        strcpy1(out, Loc1B25E0(D_00463A90));
    } else if (func_004165A8(name, D_004A1A68) == 0) {
        strcpy1(out, Loc1B25E0(D_00463AA8));
    } else if (func_004165A8(name, D_004A1A6C) == 0) {
        strcpy1(out, Loc1B25E0(D_00463AC0));
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
extern char D_004A1B60[];
extern char D_00463108[];
extern char D_00463568[];
extern char D_00463580[];
extern char D_004635A8[];
extern char D_00463AD8[];
extern char D_00463AE8[];
extern char D_00463AF8[];
extern char D_00463B10[];
extern char D_00463B20[];
extern char D_00463B30[];
extern char D_00463B40[];
extern char D_00463B58[];

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
    cUIText* title = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463108));
    if (title != 0) {
        cFEVObj_001B2378* a = (cFEVObj_001B2378*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463AD8));
        cFEVObj_001B2378* b = (cFEVObj_001B2378*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_004A1B60));
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
    cUIText* t = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463AE8));
    if (t != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B30, Loc1B25E0(D_00463AF8), info.a);
            cUIText_setUnicodeString(t, buf);
        } else {
            cUIText_setAsciiString(t, D_004A1408);
        }
    }
    t = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463B10));
    if (t != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B34, Loc1B25E0(D_00463568), info.c);
            cUIText_setUnicodeString(t, buf);
        } else {
            cUIText_setAsciiString(t, D_004A1408);
        }
    }
    t = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463B20));
    if (t != 0) {
        if (valid) {
            int done = info.b;
            int total = info.c;
            float pct = 0.0f;
            if (done < total) {
                pct = (float)done / (float)total * 100.0f;
            } else if (done != 0) {
                pct = 100.0f;
            }
            sprintf2_v(buf, D_004A1B38, Loc1B25E0(D_00463580), info.b, info.c, fptodp(pct));
            cUIText_setUnicodeString(t, buf);
        } else {
            cUIText_setAsciiString(t, D_004A1408);
        }
    }
    t = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463B30));
    if (t != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B3C, Loc1B25E0(D_004635A8), info.d);
            cUIText_setUnicodeString(t, buf);
        } else {
            cUIText_setAsciiString(t, D_004A1408);
        }
    }
    t = cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(D_00463B40));
    if (t != 0) {
        if (valid) {
            sprintf2_v(buf, D_004A1B40, Loc1B25E0(D_00463B58), info.a);
            cUIText_setUnicodeString(t, buf);
        } else {
            cUIText_setAsciiString(t, D_004A1408);
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
extern "C" void func_003A19F8(cUIText* text, char* str);
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
    func_003A19F8(self->mpTextScrollChat, text.str);
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
extern char D_00463B70[];
extern char D_00463B88[];
extern char D_00463BA8[];
extern char D_00463BC0[];
extern char D_00463BD8[];
extern char D_00461458[];
extern char D_004614E8[];
extern char D_00463BF0[];

extern "C" void func_001B2DA0(cFEStateChatRoom* self, void* menu)
{
    func_001DA528(menu, 6);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00463B70);
    func_001DA530(menu, 0, GetHashValue32(D_00463B88));
    func_001DA510(menu, 0, GetHashValue32(D_00463BA8), 3);
    func_001DA510(menu, 1, GetHashValue32(D_00463BC0), 0);
    func_001DA510(menu, 2, GetHashValue32(D_00463BD8), 6);
    func_001DA510(menu, 3, GetHashValue32(D_00461458), 2);
    func_001DA510(menu, 4, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 5, GetHashValue32(D_00463BF0), 8);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B2EB8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_0025F1B0(void* self);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern char D_004A1408[];
extern char D_00463108[];
extern char D_00463A38[];
extern char D_00463AD8[];
extern char D_00463C08[];
extern char D_00463C18[];
extern char D_00463C30[];
extern char D_00463C48[];
extern char D_00463C68[];
extern char D_00463C80[];
extern char D_00463C90[];
extern char D_00463CA0[];
extern char D_00463CB0[];
extern char D_00463CC0[];
extern char D_00463CD0[];
extern char D_00463CE0[];
extern char D_00463CF0[];
extern char D_00463D00[];
extern char D_00463D10[];
extern char D_004A1B68[];
extern char D_004A1B70[];

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

extern "C" void func_001B2EB8(cFEStateChatRoom* self, cFEVObj_001B2EB8* obj)
{
    char buf[32];
    char* o = (char*)obj;
    int id = *(int*)(o + 0x38);
    if (id == GetHashValue32(D_00463C08)) {
        self->mpTextRoomName = (cUIText*)obj;
        obj->v09(1);
    } else if (id == GetHashValue32(D_00463C18)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(D_00463C30));
        obj->v09(1);
    } else if (id == GetHashValue32(D_00463108)) {
        obj->v09(1);
        cUIText_setAsciiString((cUIText*)obj, D_004A1408);
    } else if (id == GetHashValue32(D_00463C48)) {
        obj->v09(1);
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(D_00463C68));
    } else if (id == GetHashValue32(D_00463A38)) {
        *(int*)(o + 0x14) |= 0x80;
        obj->v07(1);
        int n = func_0025F1B0(LobbyMan_mLobbyMan);
        if (n <= 0) {
            cUIMenu_setScrollableMenu(obj, 6);
        } else {
            cUIMenu_setScrollableMenu(obj, n);
        }
        self->mpMenu[0] = (cUIMenu*)obj;
    } else if (id == GetHashValue32(D_00463C80)) {
        obj->v07(1);
        self->mpMenu[1] = (cUIMenu*)obj;
    } else if (id == GetHashValue32(D_00463AD8)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(D_00463C90));
    } else if (id == GetHashValue32(D_00463CA0)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(D_00463CB0));
    } else if (id == GetHashValue32(D_00463CC0)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(D_00463CD0));
    } else if (id == GetHashValue32(D_00463CE0)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(D_00463CF0));
    } else if (id == GetHashValue32(D_004A1B68)) {
        self->mpSlider1 = (cUIThing*)obj;
        *(int*)(o + 0x14) |= 1;
    } else if (id == GetHashValue32(D_00463D00)) {
        self->mpTextScrollChat = (cUIText*)obj;
    } else if (id == GetHashValue32(D_004A1B70)) {
        self->mpSlider2 = (cUIThing*)obj;
        *(int*)(o + 0x14) |= 1;
    }
    void** slots = (void**)(((char*)self) + 0x6D0);
    for (int i = 0; i < 6; i++) {
        sprintf(buf, D_00463D10, i + 1);
        if (id == GetHashValue32(buf)) {
            *(void**)((char*)slots + (i << 2)) = obj;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B31A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_0025F248(void* self, int idx);
extern "C" void func_0025D800(void* a, int b, const char* c);
extern "C" void func_001A9B58(cFEStateNet* self);
extern "C" void func_001B33D8(cFEStateChatRoom* self);
extern char D_00463A60[];
extern char D_00463D20[];

extern "C" void func_001B31A0(cFEStateChatRoom* self, char* msg, int ev)
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
            sprintf(buf, D_00463A60, i);
            int h = *(int*)(msg + 0x38);
            if (h == GetHashValue32(buf)) {
                func_001B3D20(((char*)self), i, 1);
                return;
            }
        }
        func_001A8918((cFEStateNet*)self, (int)msg, ev);
        return;
    case 2:
        for (i = 0; i < 6; i++) {
            sprintf(buf, D_00463A60, i);
            int h = *(int*)(msg + 0x38);
            if (h == GetHashValue32(buf)) {
                func_001B3D20(((char*)self), i, 0);
                return;
            }
        }
        func_001A8918((cFEStateNet*)self, (int)msg, ev);
        return;
    case 6: {
        self->mbExiting = 1;
        char* p = (char*)func_001A9C50((cFEStateNet*)self);
        *(int*)(p + 0xBC) = 0;
        int h = GetHashValue32(D_00463D20);
        *(int*)(p + 0x164) = 0;
        *(int*)(p + 0xC4) = h;
        *(int*)(p + 0xC0) = 3;
        *(int*)(p + 0x150) = 0x802;
        func_001A9BC0((cFEStateNet*)self, p);
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
    func_001A8918((cFEStateNet*)self, (int)msg, ev);
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
    char* p = (char*)self->mpKeyboard;
    switch (*(int*)(p + 0x18)) {
    case 4:
        func_0025AAE0(LobbyMan_mLobbyMan, p + 0x74);
        break;
    case 2:
        func_00262690(GetSession_001B33D8(), func_001A98B0(((cFEStateNet*)self)), (char*)self->mpKeyboard + 0x74);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3490);
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

extern "C" void func_0039DE68(void* a, int b);
extern "C" void func_001B3978(cFEStateChatRoom* self, void* a1);

extern "C" int func_001B3490(cFEStateChatRoom* self, int a1)
{
    if (a1 != 0) {
        int i;
        func_0039DE68(self->mpSlider1, 0);
        func_0039DE68(self->mpSlider2, 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001B3528);
#ifdef SKIP_ASM
extern "C" void func_001B3BF0(cFEStateChatRoom* self);
extern "C" void func_001A87D0(cFEStateNet* self, int a1);

extern "C" void func_001B3528(cFEStateChatRoom* self, int a1)
{
    func_001B3BF0(self);
    func_001A87D0((cFEStateNet*)self, a1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B3568);
#ifdef SKIP_ASM
extern "C" void func_001B2378(cFEStateChatRoom* self, int a1);

extern "C" int func_001B3568(cFEStateChatRoom* self, void* sender, int event, int value)
{
    if (event == 4) {
        if (sender == (void*)self->mpMenu[0]) {
            self->miMenuScroll[0] = value;
            func_001B2378(self, 0);
            return 0x101;
        }
        if (sender == (void*)self->mpMenu[1]) {
            self->miMenuScroll[1] = value;
        }
    }
    return 0x101;
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
extern char D_00463D78[];

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
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00463D78);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x805;
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
extern const char D_00461C60[];

extern "C" void func_001B3760(cFEStateChatRoom* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_0039F600((char*)self->mpEngine + 0x18);
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), (int)self->mpEngine));
    } else {
        func_001A97B8(((cFEStateNet*)self), a1, a2);
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
extern const char D_00461C60[];

extern "C" void func_001B37D8(cFEStateChatRoom* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_0039F600((char*)self->mpEngine + 0x18);
        func_001A8A40(((cFEStateNet*)self), cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), (int)self->mpEngine));
    } else {
        func_001A97B8(((cFEStateNet*)self), a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B3850);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001B35B0(cFEStateChatRoom* self, void* a1, int a2);
extern "C" void func_001B3760(cFEStateChatRoom* self, void* a1, int a2);
extern "C" void func_001B37D8(cFEStateChatRoom* self, void* a1, int a2);

extern "C" void func_001B3850(cFEStateChatRoom* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x80B:
        func_001B35B0(self, a1, a2);
        break;
    case 0x800:
        func_001B3760(self, a1, a2);
        break;
    case 0x801:
        func_001B37D8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B38D0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int v);
extern char D_00463DB8[];
extern char D_004A1408[];

extern "C" int func_001B38D0(cFEStateChatRoom* self, void* obj)
{
    sVE1A4170* vt = *(sVE1A4170**)((char*)obj + 0x8);
    if (vt[27].fn((char*)obj + vt[27].delta) != 0) {
        // PORT: string pointer passed through an int parameter (func_001A9150's a3)
        func_001A9150(((cFEStateNet*)self), 0, GetHashValue32(D_00463DB8), (int)D_004A1408, 0, 4, 0x3F);
        *(int*)((char*)self->mpKeyboard + 0x43C) = 0;
        func_001CE468(self->mpKeyboard, 1);
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
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
extern char D_00463A38[];

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
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463A38)), sel);
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
extern char D_00461B30[];
extern char D_00463DC8[];
extern char D_00461618[];

// Message handler for the save/load screen; unknown ids refresh the list and go to the base handler.
extern "C" void* func_001B3A30(cFEStateChatRoom* self, int a1, int a2)
{
    switch (a1) {
    case 0xE0: {
        char buf[32];
        strcpy(buf, (char*)self + 0x70C);
        func_001B2378(self, 1);
        func_001B3978(self, buf);
        break;
    }
    case 0xDC:
        if (func_0025DA90(LobbyMan_mLobbyMan) == 0) {
            func_001A8A40(self, cFEStateLobbyMenu_cFEStateLobbyMenu((sFEObj_001BCB58*)cMemMan_alloc(0xB84, D_00461B30, 0, 0), (int)self->mpEngine));
            break;
        }
        func_001B2378(self, 1);
        break;
    case 0xE8: {
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00463DC8);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x804;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    case 0xDE:
        if (self->mbExiting == 0) {
            func_001B2CD0(self, a2);
        }
        break;
    case 0xE1:
        func_001B2378(self, 1);
        break;
    case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE: case 0xEF: case 0xF0:
    case 0x106: case 0x10A: case 0x10B:
        func_001AB3D0((cFEStateNet*)self, func_001A98B0(self), 1);
        break;
    default:
        func_001AB3D0((cFEStateNet*)self, func_001A98B0(self), 1);
        return func_001A8E40_3(self, a1, a2);
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
extern char D_00463A60[];

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

extern "C" void func_001B3BF0(cFEStateChatRoom* p)
{
    sOwner1B3BF0* self = (sOwner1B3BF0*)p;
    char buf[32];
    sInfo1B3BF0 info;
    int n = func_0025F1B0(LobbyMan_mLobbyMan);
    int i;
    for (i = 0; i < 6; i++) {
        int idx = self->first + i;
        sprintf(buf, D_00463A60, i);
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
extern "C" void func_0039DA20(void* a0, void* a1, int a2, int a3);
extern "C" void func_003A0290(void* text, void* out);
extern char D_00463A60[];
extern char D_00463EA0[];
extern char D_004A17E0[];

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
    sprintf(name, D_00463A60, idx);
    cFEVObj_001B3D20* sel = Get1B3D20(self, name);
    func_0039DA20(*(void**)((char*)self + 0x6EC), *(void**)((char*)self + 0x704), 6, func_0025F1B0(LobbyMan_mLobbyMan));
    if (sel == 0) {
        if (idx >= 0) {
            if (idx < 6) {
                return;
            }
        }
    }
    int i;
    for (i = 0; i < 6; i++) {
        sprintf(name, D_00463A60, i);
        Get1B3D20(self, name)->v11(sCol1B3D20(1.0f, 0.0f, 0.0f, 0.0f));
    }
    cFEVObj_001B3D20* hl = Get1B3D20(self, D_004A17E0);
    if (hl == 0) {
        return;
    }
    if (on == 0) {
        hl->v09(0);
        return;
    }
    sprintf(name, D_00463EA0, idx);
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
 * its delete, func_002C2300, matches On Tour's cMemCard::DeleteInstance) and calls its slot 2. miWaitFrames starts
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
    mem->mHashName = 0x31;
    cFEVObj_001B3F78* o = func_002C22C8();
    D_004A1B78 = o;
    o->v02();
    mem->miWaitFrames = -1;
    return ((void*)mem);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig__cFEStateNetConfig);
#ifdef SKIP_ASM
extern void* cFEStateNetConfig_vtbl[];
void cMemMan_free(void* p);
extern "C" void func_002C2300(void* p);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");
// D_004A1B78 is declared earlier in the unit (cFEVObj_001B3F78*, see cFEStateNetConfig_cFEStateNetConfig)

/**
 * Destroys the network-configuration screen state: frees its heap texts and the network configuration object.
 *
 * Restores cFEStateNetConfig_vtbl, frees the three mpustrAuthError texts and mpustrPopupText (cMemMan_free) and
 * deletes D_004A1B78 with func_002C2300, then runs the cFEStateNet destructor (func_001A85D0) with @p flags.
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
        func_002C2300(D_004A1B78);
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
extern char D_00463ED0[];

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
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_00463ED0), 0);
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
extern "C" void* func_001A8770(cFEStateNet* self);
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
 * 8 (idle) func_001B45E8 (connect with the chosen setting). Ends with the cFEStateNet update (func_001A8770).
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
                func_001B4888(self, 0x842, 0x18, D_004A1B80);
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
    func_001A8770((cFEStateNet*)self);
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
        func_001B4888(self, 0x840, 0x14, -1);
    } else if (a != 0) {
        func_001B4CD8(self);
    } else {
        self->miState = 0;
        func_001B5068(self, -1);
        func_001B4888(self, 0x840, 2, -1);
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
                func_001B4888(self_, 0x840, 2, -1);
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
                    func_001B4888(self_, 0x841, 0x14, 1);
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
extern char D_00463F08[];
extern char D_00463F20[];
extern char D_00463F38[];
extern char D_0045F7E0[];
extern char D_00462C88[];

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
        int name = Loc1B45E8(D_00463F08);
        sprintf1_1B45E8(buf, Loc1B45E8(D_00463F20), D_004A1B80 + 1, name);
        *(char**)((char*)p + 0xC4) = buf;
        *(int*)((char*)p + 0xC0) = 1;
        *(int*)((char*)p + 0xBC) = 1;
        func_001C57C0((char*)p + 0xBC, 0, GetHashValue32(D_0045F7E0));
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x843;
        func_001A9BC0((cFEStateNet*)self, p);
    } else {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00463F38);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x80E;
        func_001C57C0(sub, 0, GetHashValue32(D_00462C88));
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
extern "C" void func_002C2718(char* buf, const char* fmt, int a2);
extern "C" void sprintf1(void* str, int a1, char* buf);
extern char D_004A12C0[];
extern char D_00463F58[];

struct sVE_001B47C0 {
    short delta;
    short index;
    int (*fn)(void* self, int hash);
};

extern "C" void func_001B47C0(cFEStateNetConfig* self, int a1)
{
    char buf[32];
    char* str = (char*)self + 0x71C;
    func_002C2718(buf, D_004A12C0, a1);
    char* o = (char*)SSXApp->FELocale;
    sVE_001B47C0* vt = *(sVE_001B47C0**)(o + 4);
    char* obj = o + vt[4].delta;
    sprintf1(str, vt[4].fn(obj, GetHashValue32(D_00463F58)), buf);
    void* p = func_001A9C50((cFEStateNet*)self);
    // stores
    *(int*)((char*)p + 0xBC) = 0;
    *(char**)((char*)p + 0xC4) = str;
    *(int*)((char*)p + 0xC0) = 1;
    *(int*)((char*)p + 0x154) = 1;
    *(int*)((char*)p + 0x150) = 0x83F;
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0x168) = 0;
    // end
    func_001A9BC0((cFEStateNet*)self, p);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4888);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
// PORT: sprintf1 / func_002C2718 are varargs formatters (dst, fmt, ...); the unit declares 3-arg views
extern "C" void sprintf1_1B4888(void* str, int fmt, ...) __asm__("sprintf1");
extern "C" void func_002C2718_1B4888(char* buf, const char* fmt, ...) __asm__("func_002C2718");
extern char D_004A12C0[];
extern char D_0045DC90[];
extern char D_0045DCD8[];
extern char D_00463F08[];
extern char D_00463F20[];
extern char D_00463F70[];
extern char D_00462688[];
extern char D_0045F7E0[];
extern char D_00462C88[];

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

extern "C" void func_001B4888(cFEStateNetConfig* self, int a1, int a2, int a3)
{
    char buf[0x20];
    int flag = 0;
    void* p = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)p + 0xBC;
    switch (a2) {
    case 2:
        if (a3 == -1) {
            char* dst = (char*)self + 0x71C;
            a3 = 1;
            int name = Loc1B4888(D_00463F08);
            sprintf1_1B4888(dst, Loc1B4888(D_00463F20), a3, name);
            *(char**)((char*)p + 0xC4) = dst;
            *(int*)((char*)p + 0xC0) = a3;
        } else {
            char* dst = (char*)self + 0x71C;
            int name = Loc1B4888(D_00463F08);
            sprintf1_1B4888(dst, Loc1B4888(D_00463F20), a3 + 1, name);
            *(char**)((char*)p + 0xC4) = dst;
            *(int*)((char*)p + 0xC0) = 1;
            flag = 1;
        }
        break;
    case 0x14: {
        func_002C2718_1B4888(buf, D_004A12C0, a3 + 1);
        char* dst = (char*)self + 0x71C;
        sprintf1_1B4888(dst, Loc1B4888(D_0045DC90), buf);
        *(char**)((char*)p + 0xC4) = dst;
        *(int*)((char*)p + 0xC0) = 1;
        break;
    }
    case 0x18: {
        func_002C2718_1B4888(buf, D_004A12C0, a3 + 1);
        char* dst = (char*)self + 0x71C;
        int name = Loc1B4888(D_00463F08);
        sprintf1_1B4888(dst, Loc1B4888(D_00463F70), name, buf);
        *(char**)((char*)p + 0xC4) = dst;
        *(int*)((char*)p + 0xC0) = 1;
        break;
    }
    case 6:
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462688);
        *(int*)((char*)p + 0xC0) = 3;
        break;
    }
    *(int*)sub = 2;
    func_001C57C0(sub, 0, GetHashValue32(D_0045DCD8));
    if (flag == 0) {
        func_001C57C0(sub, 1, GetHashValue32(D_0045F7E0));
    } else {
        func_001C57C0(sub, 1, GetHashValue32(D_00462C88));
    }
    *(int*)(sub + 0x94) = a1;
    *(int*)(sub + 0xA8) = 0;
    func_001A9BC0((cFEStateNet*)self, p);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B4C00);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" int func_001B4C00(cFEStateNetConfig* self, void* a1, int a2)
{
    int r;
    if (a2 == 0xF || a2 == 0x14) {
        r = func_001A97B8(self, a1, a2);
    } else {
        r = func_001A97B8(self, a1, a2);
    }
    return r;
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
extern "C" void* func_00399920(void* box);
extern "C" void func_0039A4B0(void* box, unsigned short* str, int id);
extern "C" void func_003EFA48(unsigned short* dst, int n, const char* src);
extern "C" int func_004165A8(const char* a, const char* b);
extern int D_004A1B80;
extern int D_004A1B90[2];
extern int cCommSystem_mNumNetConn;
extern char* D_004A1B7C;
extern char D_00463F88[];
extern char D_004A1B98[];
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
        ((cFEVObj_001B4CD8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463F88)))->v20(&pos);
        scale = *(sVec3_001B4CD8*)((char*)self->mpLBSettings + 0x50);
        if (self->miNumSettings > 4) {
            self->miNumSettings = 4;
        }
        for (int i = 0; i < self->miNumSettings; i++) {
            char* nm = cCommSystem_mNetConns[i].name42;
            float w;
            int len;
            if (self->miNumSettings >= 2) {
                len = sprintf(buf, D_004A1B98, i + 1);
                w = pos.x - cFont_textExtent(func_00399920(self->mpLBSettings), buf, &size, 1, scale.x, scale.y);
            } else {
                w = pos.x;
                len = 0;
                buf[0] = 0;
            }
            char* rec = cCommSystem_mNetConns[i].name4;
            func_003EFA48(name, 0x40, nm);
            UIAsciiToUnicode(line, buf);
            func_00392680(func_00399920(self->mpLBSettings), scale.x, w, name, line + len, 0x40 - len);
            func_0039A4B0(self->mpLBSettings, line, i + 0x14);
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
        func_001B4888(self, 0x842, 0x18, D_004A1B80);
        break;
    case -15:
        func_001B4888(self, 0x841, 6, -1);
        break;
    case -200:
        func_001B4888(self, 0x841, 0x14, 0);
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
extern char D_00463F98[];
extern char D_00461520[];
extern char D_00463FB0[];
extern char D_00463FC0[];
extern char D_00463F88[];
extern char D_00463FD8[];
extern char D_00463FF0[];
extern char D_00464000[];
extern char D_00464010[];
extern char D_00464020[];

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
    if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00463F98)) {
        *(int*)((char*)item + 0x18) = 1;
        obj->v09(1);
        obj->v06(1);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00461520)) {
        *(int*)((char*)item + 0x18) = -1;
        obj->v09(1);
        obj->v06(0);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00463FB0)) {
        *(int*)((char*)item + 0x18) = -1;
        obj->v09(0);
        obj->v06(0);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00463FC0)) {
        *(int*)((char*)item + 0x18) = -1;
        obj->v09(1);
        obj->v06(0);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00463F88)) {
        *(int*)((char*)item + 0x18) = 100;
        self->mpLBSettings = (cUIListBox*)item;
        obj->v24(1);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00463FD8)) {
        self->mpConfigRectangle = (cUIThing*)item;
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00463FF0)) {
        self->mpNoConfiguration = (cUIThing*)item;
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00464000)) {
        *(int*)((char*)item + 0x18) = 101;
        self->mpLBAdapters = (cUIListBox*)item;
        obj->v24(1);
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00464010)) {
        self->mpMenuConfig = (cUIMenu*)item;
        *(int*)((char*)item + 0x18) = -1;
        *(int*)((char*)item + 0x14) |= 0x80;
    } else if (IsHash1B52C8(*(int*)((char*)item + 0x38), D_00464020)) {
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
extern char D_00464030[];
extern char D_00464048[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_00464068[];
extern char D_004614E8[];

extern "C" void func_001B54B8(cFEStateNetConfig* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00464030);
    func_001DA530(menu, 0, GetHashValue32(D_00464048));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_00464068), 2);
    func_001DA510(menu, 4, GetHashValue32(D_004614E8), 1);
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
extern char D_00464080[];

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
            char* p = (char*)cFEStateAuthDNAS_cFEStateAuthDNAS((cFEStateAuthDNAS*)((sFEObj_001B6288*)cMemMan_alloc(0x700, D_00464080, 0, 0)), (int)self->mpEngine, (int)self);
            *(int*)(p + 0x18) = 0x81A;
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
    if (a2 == 0xF || a2 == 0x14) {
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
extern char D_00464098[];
extern char D_00462D10[];

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
            void* p = cFEStateAuthDNASError_cFEStateAuthDNASError((cFEStateAuthDNASError*)cMemMan_alloc(0x6E8, D_00464098, 0, 0), (int)self->mpEngine, (int)self,
                                    (int)self->mpustrAuthError[1], (int)self->mpustrAuthError[2], (int)self->mpustrAuthError[0]);
            *(int*)((char*)p + 0x18) = 0x81B;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        } else if (self->unk_6D8 == 0) {
            void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, self, 0, 0, 0);
            *(int*)((char*)p + 0x18) = 0x82C;
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
extern const char D_00461CD8[];

struct sVE1B5A58 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// Tracks a pending flag; on 0x10/0x14 restarts the memory-card task and resets the list.
extern "C" void func_001B5A58(cFEStateNetConfig* self, void* a1, int a2)
{
    switch (a2) {
    case 0x15:
        self->unk_6DC = 0;
        func_001A97B8_v(self, a1, a2);
        break;
    case 0xF:
        func_001A97B8_v(self, a1, a2);
        self->unk_6DC = 1;
        break;
    case 0x10: case 0x14: {
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
    case 0x16:
        if (self->unk_6DC != 0) {
            func_001A8A40(self, cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(0x6D4, D_00461CD8, 0, 0), (int)self->mpEngine));
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
extern char D_00463F98[];

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
        cFEVObj_001B5BD8* o = (cFEVObj_001B5BD8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463F98));
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
    if (a2 == 0x16) {
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
    case 0x81C:
        func_001B5CF8(self, pPopup, Event);
        break;
    case 0x80E:
        func_001B55B0(self, pPopup, Event);
        break;
    case 0x81B:
        func_001B5740(self, pPopup, Event);
        break;
    case 0x81A:
        func_001B5830(self, pPopup, Event);
        break;
    case 0x82C:
        func_001B5A58(self, pPopup, Event);
        break;
    case 0x840: case 0x841: case 0x842:
        func_001B4C40(self, pPopup, Event);
        break;
    case 0x843:
        func_001B4C00(self, pPopup, Event);
        break;
    case 0x844:
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
extern char D_004641B0[];
extern char D_004641D0[];
extern char D_00460688[];
extern char D_00461618[];

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
extern "C" int func_001B5E28(cFEStateNetConfig* self, int id, int a2)
{
    char* msg = (char*)a2;
    if (id == 0x105) {
        if (self->mAttrState & 6) {
            func_0039F190((char*)self->mpEngine + 0x18, 1);
        }
    } else {
        if (id >= 0x105) {
            if (id < 0x117) {
                if (id >= 0x115) {
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
                            str = Loc1B5E28(D_004641B0);
                            break;
                        default:
                            str = Loc1B5E28(D_004641D0);
                            break;
                        }
                        if (self->mpustrPopupText != 0) {
                            cMemMan_free(self->mpustrPopupText);
                            self->mpustrPopupText = 0;
                        }
                        void* u = (void*)func_001AAF90((cFEStateNet*)self, msg);
                        if (u != 0) {
                            self->mpustrPopupText = (unsigned short*)operator_new_tag((USTR_length(u) + 1) * 2, D_00460688, 0x100, 0);
                            USTR_copy(self->mpustrPopupText, u);
                        } else {
                            self->mpustrPopupText = (unsigned short*)operator_new_tag((USTR_length((void*)str) + 1) * 2, D_00460688, 0x100, 0);
                            strcpy1(self->mpustrPopupText, str);
                        }
                        int one = 1;
                        void* p = func_001A9C50((cFEStateNet*)self);
                        *(int*)((char*)p + 0xBC) = one;
                        char* sub = (char*)p + 0xBC;
                        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
                        *(void**)((char*)p + 0xC4) = (void*)self->mpustrPopupText;
                        *(int*)((char*)p + 0x150) = 0x81C;
                        *(int*)((char*)p + 0xC0) = one;
                        func_001A9BC0((cFEStateNet*)self, p);
                    }
                    return 0;
                }
            }
        }
        return func_001A8E40_3i(self, id, a2);
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
    return 0x101;
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
    int r = 0x101;
    if (key < 2) {
        if (self->miNumSettings > 1) {
            r = 0x101;
        } else {
            return 0;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetConfig_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00194A60(void* mem, int a1);
extern "C" void func_001A86E8(int on);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void* func_001A9C50(cFEStateNet* self);
void func_001B4198(void* self);
extern "C" void func_001B5268(cFEStateNetConfig* self, int a1);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_0045DCC8[];
extern char D_0045FCD8[];
extern char D_004641F0[];
extern char D_00463F88[];
extern const char D_00460BA8[];

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
 * cFEStateMainMenu (func_00194A60). Others go to the cFEStateNet handler (func_001A8918). A null widget is ignored.
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
        int t = *(int*)(obj + 0x18);
        switch (t) {
        case 0:
            ((cFEVObj_001B60A8*)self->mpPairSettings)->v08(1);
            func_001B4198(self);
            break;
        case 1: {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 2;
            char* sub = (char*)p + 0xBC;
            func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
            func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_004641F0);
            *(int*)((char*)p + 0xC0) = 3;
            *(int*)((char*)p + 0x150) = 0x844;
            *(int*)((char*)p + 0x168) = 1;
            func_001A9BC0((cFEStateNet*)self, p);
            break;
        }
        }
        break;
    }
    case 9:
        if (IsHash1B60A8(*(int*)(obj + 0x38), D_00463F88)) {
            self->miSelectedSetting = *(unsigned char*)(obj + 0x319);
            func_0039A7A8(self->mpLBAdapters, (unsigned char)self->miSelectedSetting);
        }
        break;
    case 8:
        func_001B5268(self, 0);
        break;
    case 6:
        func_001A86E8(0);
        func_001A8A40(self, func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), (int)self->mpEngine));
        break;
    default:
        func_001A8918((cFEStateNet*)self, pWidget, msg);
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
extern char D_00464208[];
extern char D_00464220[];

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

extern "C" sFEObj_001B6288* cFEStateAuthDNAS_cFEStateAuthDNAS(cFEStateAuthDNAS* self, int a1, int a2)
{
    func_001A8500_3(((sFEObj_001B6288*)self), a1, a2);
    self->unk_6F4 = 2;
    self->mVtbl = cFEStateAuthDNAS_vtbl;
    self->unk_6F8 = 0x1E;
    self->mHashName = 0x44;
    self->mpAuth = 0;
    self->mpTextAuth = 0;
    self->miState = 0;
    self->mpustrText = 0;
    char* o = (char*)SSXApp->FELocale;
    sVE_001B6288* vt = *(sVE_001B6288**)(o + 4);
    int n = USTR_length(vt[4].fn(o + vt[4].delta, GetHashValue32(D_00464208)));
    if (self->mpustrText != 0) {
        cMemMan_free(self->mpustrText);
        self->mpustrText = 0;
    }
    self->mpustrText = (unsigned short*)operator_new_tag((n + 5) * 2, D_00464220, 0x100, 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001B63E8);
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
extern char D_00464238[];
extern char D_00464250[];
extern char D_00464260[];

struct sProfile1B63E8 {
    unsigned int flags;
    char rest[0x284];
};
extern sProfile1B63E8 D_00535610;

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

extern "C" void func_001B63E8(cFEStateAuthDNAS* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00464238), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        cFEVObj_001B63E8* o = (cFEVObj_001B63E8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464250));
        if (o != 0) {
            o->v09(0);
        }
    }
    self->mpAuth = func_002561B8(cMemMan_alloc(0xA50, D_00464260, 0x100, 0));
    self->miState = 1;
    cBE_getInterface_Fv(cBE_getBE(), 4);
    int mode = 0;
    sProfile1B63E8 prof = D_00535610;
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
INCLUDE_ASM("fe/feasyncfile", func_001B65C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_004631E8[];
extern char D_00464278[];
extern char D_00464288[];
extern char D_004A1BA0[];
extern char D_004642A0[];

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

extern "C" void func_001B65C0(cFEStateAuthDNAS* self, cUIText* text)
{
    cFEVObj_001B65C0* obj = (cFEVObj_001B65C0*)text;
    if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_004631E8)) {
        self->mpTextCancel = text;
        obj->v09(0);
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_00464278)) {
        self->mpTextAuth = text;
        cUIText_setUnicodeStringByID(self->mpTextAuth, GetHashValue32(D_00464288));
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_004A1BA0)) {
        self->mpXButton = (cUIThing*)text;
        obj->v09(0);
    } else if (IsHash1B65C0(*(int*)((char*)text + 0x38), D_004642A0)) {
        self->mpSubmitRect = (cUIThing*)text;
        obj->v09(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B66B8);
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

extern "C" int func_001B66B8(cFEStateAuthDNAS* self, cFEVObj_001B66B8* obj)
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
INCLUDE_ASM("fe/feasyncfile", func_001B6740);
#ifdef SKIP_ASM
class func_001B6740_cObj {
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

extern "C" int func_001B6740(cFEStateAuthDNAS* self, int a1, int a2)
{
    return ((func_001B6740_cObj*)self->mpParentState)->v24(a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6770);
#ifdef SKIP_ASM
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);

extern "C" void func_001B6770(cFEStateAuthDNAS* self, int a1, int msg)
{
    if (msg != 0xD) {
        func_001A8918((cFEStateNet*)self, a1, msg);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6798);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void USTR_copy(void* dst, void* src);
extern "C" int USTR_length(void* s);
void cMemMan_free(void* p);
extern "C" void* func_001A8770(cFEStateNet* self);
extern "C" void func_002562D0(void* self);
extern "C" void func_002563F8(void* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00464208[];
extern char D_00464220[];
extern char D_00464250[];

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

extern "C" void func_001B6798(cFEStateAuthDNAS* self)
{
    switch (self->miState) {
    case 1:
        if (IsReady_001B6798((int*)self->mpAuth)) {
            ((cFEVObj_001B6798*)self->mpTextCancel)->v09(1);
            ((cFEVObj_001B6798*)self->mpXButton)->v09(1);
            ((cFEVObj_001B6798*)self->mpSubmitRect)->v09(1);
            ((cFEVObj_001B6798*)self->mpTextAuth)->v09(1);
            cUIText_setUnicodeStringByID(self->mpTextAuth, GetHashValue32(D_00464208));
            cFEVObj_001B6798* g = (cFEVObj_001B6798*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464250));
            if (g != 0) {
                g->v09(1);
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
        char* d = (char*)self->mpAuth;
        if (*(int*)(d + 0x1C) != 0) {
            char* o = (char*)self->mpParentState;
            char* a = d + 0x450;
            char* b = d + 0x650;
            char* c = d + 0x850;
            if (*(void**)(o + 0x710) != 0) {
                cMemMan_free(*(void**)(o + 0x710));
                *(void**)(o + 0x710) = 0;
            }
            *(void**)(o + 0x710) = operator_new_tag((USTR_length(a) + 1) * 2, D_00464220, 0x100, 0);
            if (*(void**)(o + 0x714) != 0) {
                cMemMan_free(*(void**)(o + 0x714));
                *(void**)(o + 0x714) = 0;
            }
            *(void**)(o + 0x714) = operator_new_tag((USTR_length(b) + 1) * 2, D_00464220, 0x100, 0);
            if (*(void**)(o + 0x718) != 0) {
                cMemMan_free(*(void**)(o + 0x718));
                *(void**)(o + 0x718) = 0;
            }
            *(void**)(o + 0x718) = operator_new_tag((USTR_length(c) + 1) * 2, D_00464220, 0x100, 0);
            USTR_copy(*(void**)(o + 0x710), a);
            USTR_copy(*(void**)(o + 0x714), b);
            USTR_copy(*(void**)(o + 0x718), c);
            ((cFEVObj_001B6798*)self->mpParentState)->v17(self, 0x10);
        } else {
            ((cFEVObj_001B6798*)self->mpParentState)->v17(self, 0xF);
        }
        break;
    }
    case 4:
        if (IsDone_001B6798((int*)self->mpAuth)) {
            ((cFEVObj_001B6798*)self->mpParentState)->v17(self, 0x14);
        }
        break;
    }
    func_002563F8(self->mpAuth);
    func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6A90);
#ifdef SKIP_ASM
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int func_001B6A90(cFEStateAuthDNAS* self, int a1, unsigned int key)
{
    int r = 0x101;
    if (key < 10) {
        if (key < 8) {
            r = 0x101;
        } else {
            return 0x100;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError_cFEStateAuthDNASError);
#ifdef SKIP_ASM
extern void* cFEStateAuthDNASError_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateAuthDNASError_cFEStateAuthDNASError(cFEStateAuthDNASError* self, int a1, int a2, int a3, int a4, int a5)
{
    func_001A8500_3(self, a1, a2);
    self->mpustrBody = (unsigned short*)a3;
    self->mpustrFooter = (unsigned short*)a4;
    self->mpustrTitle = (unsigned short*)a5;
    self->mVtbl = cFEStateAuthDNASError_vtbl;
    self->mHashName = 0x45;
    return ((void*)self);
}
#endif

extern void* cFEStateAuthDNASError_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateAuthDNASError__cFEStateAuthDNASError__FPv);
#ifdef SKIP_ASM
void* cFEStateAuthDNASError__cFEStateAuthDNASError(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateAuthDNASError_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6B50);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004642B0[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void* func_001B6B50(cFEStateAuthDNASError* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_004642B0), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6BB8);
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
extern char D_004A1BA8[];
extern char D_004A1BB0[];
extern char D_004A1BB8[];
extern char D_00464250[];
extern char D_004642C0[];

static inline int IsHash1B6BB8(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001B6BB8(cFEStateAuthDNASError* self, void* obj)
{
    if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004A1BA8)) {
        self->mpFooter = (cUIText*)obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004A1BB0)) {
        self->mpBody = (cUIText*)obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004A1BB8)) {
        self->mpTitle = (cUIText*)obj;
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_00464250)) {
        ((cFEVText_001B6BB8*)obj)->v09(1);
    } else if (IsHash1B6BB8(*(int*)((char*)obj + 0x38), D_004642C0)) {
        *(int*)((char*)obj + 0x90) |= 8;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6C98);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" int strlen1(void* p);
extern "C" void cUIText_setUnicodeStringWrap(void* text, void* str);
extern char D_004A1BC0[];
extern char D_004A1BC8[];
extern char D_004642D0[];

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

extern "C" int func_001B6C98(cFEStateAuthDNASError* self, int on)
{
    if (on) {
        if (strlen1(self->mpustrFooter) == 0) {
            PlayLabel_001B6C98(self, D_004A1BC0);
            ((cFEVObj_001B6C98*)self->mpFooter)->v09(0);
        } else {
            PlayLabel_001B6C98(self, D_004A1BC8);
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
        PlayLabel_001B6C98(self, D_004642D0);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6E10);
#ifdef SKIP_ASM
class func_001B6E10_cObj {
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

extern "C" int func_001B6E10(cFEStateAuthDNASError* self, int a1, int a2)
{
    return ((func_001B6E10_cObj*)self->mpParentState)->v24(a1, a2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6E40);
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

extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);

extern "C" void func_001B6E40(cFEStateAuthDNASError* self, int a1, unsigned int msg)
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
        func_001A8918(((cFEStateNet*)self), a1, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B6EA8);
#ifdef SKIP_ASM
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int func_001B6EA8(cFEStateAuthDNASError* self, int a1, unsigned int key)
{
    int r = 0x101;
    if (key < 10) {
        if (key < 8) {
            r = 0x101;
        } else {
            return 0x100;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem_cFEStateNetAccountMem);
#ifdef SKIP_ASM
extern "C" void* func_001ABF70(cFEStateNet* self);
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
 * Runs the cFEBlankPopup constructor (func_001ABF70; the C call passes only self, so @p pEngine and @p owner reach it
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
    func_001ABF70((cFEStateNet*)self);
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
        func_001B70D0_v(self, 0xB);
        self->miStartState = 0xB;
        break;
    case 3:
        SetNames_001B6ED0(buf1, buf2, D_00536760, D_00536760 + 0x11);
        func_001B70D0_v(self, 0x13);
        self->miStartState = 0x13;
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
extern void* D_0046B6F8[];
extern "C" void func_002658B8();
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the online-account memory-card popup: deletes the card object, then runs the base destructors.
 *
 * Restores cFEStateNetAccountMem_vtbl, deletes D_004A33D0 (func_002658B8), stores cFEBlankPopup's vtable D_0046B6F8
 * and runs the cFEStateNet destructor (func_001A85D0) with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cFEStateNetAccountMem__cFEStateNetAccountMem(cFEStateNetAccountMem* self, int flags)
{
    self->mVtbl = cFEStateNetAccountMem_vtbl;
    func_002658B8();
    self->mVtbl = D_0046B6F8;
    func_001A85D0_dtor(((cFEStateNet*)self), flags);
}
#endif

extern "C" void* func_001ABFA8(cFEStateNet* self);

//99.29%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetAccountMem_onCreateScreen__FPv);
#ifdef SKIP_ASM
/**
 * Creates the account popup's screen: the cFEBlankPopup handler (func_001ABFA8) adds and plays its blank screen.
 *
 * @return func_001ABFA8()'s result (its value is not meaningful; declared void* by the unit)
 */
void* cFEStateNetAccountMem_onCreateScreen(void* self)
{
    return func_001ABFA8((cFEStateNet*)self);
}
#endif

extern "C" void* func_001B7778(cFEStateNetAccountMem*);

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B70A8__FPv);
#ifdef SKIP_ASM
void* func_001B70A8(void* self)
{
    int t0 = 0;
    *(signed char*)((char*)self + 0x6e0) = (signed char)t0;
    *(signed char*)((char*)self + 0x6f1) = (signed char)t0;
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
extern "C" void func_002C2718(char* buf, const char* fmt, int a2);
extern "C" void func_0039F190(void* self, int a1);
extern int* D_004A33D0;
extern char D_004A12C0[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];
extern char D_00463F58[];
extern char D_004642E0[];
extern char D_004642F8[];
extern char D_00464310[];
extern char D_00464330[];
extern char D_00464348[];
extern char D_00464360[];
extern char D_00464380[];
extern char D_00464398[];

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
    int str = Loc1B70D0(D_004642E0);
    func_002C2718(buf, D_004A12C0, 1);
    switch ((unsigned int)state) {
    case 1: {
        char* text = self->text;
        func_00265D08(D_004A33D0);
        func_00265DF0(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(D_00463F58), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 6: {
        char* text = self->text;
        func_00265D08(D_004A33D0);
        func_00265DF0(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(D_00463F58), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 7: {
        char* text = self->text;
        self->f6D8 = 0;
        self->fB04 = 0;
        func_00265D08(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(D_004642F8), str);
        int h1 = GetHashValue32(D_0045DCC8);
        int h2 = GetHashValue32(D_0045FCD8);
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
        sprintf1_1B70D0(text, Loc1B70D0(D_00464310), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 12: {
        char* text = self->text;
        func_00265D08(D_004A33D0);
        func_00265DF0(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(D_00463F58), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 13: {
        char* text = self->text;
        self->fB04 = 0;
        self->f6D8 = 0;
        func_00265D08(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(D_00464330), buf);
        int h1 = GetHashValue32(D_0045DCC8);
        int h2 = GetHashValue32(D_0045FCD8);
        func_001B7650((cFEStateNetAccountMem*)self, 0xE, 1, h1, h2, 0, (int)text);
        break;
    }
    case 14: {
        self->fB04 = 1;
        char* text = self->text;
        func_00265E30(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(D_00464348), buf);
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
        sprintf1_1B70D0(text, Loc1B70D0(D_00464310), buf);
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
        sprintf1_1B70D0(text, Loc1B70D0(D_00463F58), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 21: {
        char* text = self->text;
        self->fB04 = 0;
        self->f6D8 = 0;
        func_00265D08(D_004A33D0);
        strcpy1(text, Loc1B70D0(D_00464360));
        int h1 = GetHashValue32(D_0045DCC8);
        int h2 = GetHashValue32(D_0045FCD8);
        func_001B7650((cFEStateNetAccountMem*)self, 0x16, 1, h1, h2, 0, (int)text);
        break;
    }
    case 22: {
        self->fB04 = 1;
        if (self->f6D8 != 0) {
            func_001B70D0_v(self, 0x17);
            break;
        }
        char* text = self->text;
        func_00265E20(D_004A33D0);
        strcpy1(text, Loc1B70D0(D_00464380));
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
        sprintf1_1B70D0(text, Loc1B70D0(D_00464398), buf);
        int h1 = GetHashValue32(D_0045DCC8);
        int h2 = GetHashValue32(D_0045FCD8);
        func_001B7650((cFEStateNetAccountMem*)self, 0x1A, 1, h1, h2, 0, (int)text);
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
        sprintf1_1B70D0(text, Loc1B70D0(D_00464310), buf);
        func_001B7650((cFEStateNetAccountMem*)self, 0, 0, 0, 0, 0, (int)text);
        break;
    }
    case 27: {
        self->fB04 = 1;
        char* text = self->text;
        func_001B7778_v(self);
        func_00265E10(D_004A33D0);
        sprintf1_1B70D0(text, Loc1B70D0(D_00464310), buf);
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

extern "C" void func_001B7650(cFEStateNetAccountMem* self, int a1, int a2, int b0, int b1, int a5, int a6)
{
    void* p = func_001A9C50((cFEStateNet*)self);
    char* sub = (char*)p + 0xBC;
    if (b0 != 0) {
        if (b1 != 0) {
            *(int*)((char*)p + 0xBC) = 2;
            func_001C57C0(sub, 0, b0);
            func_001C57C0(sub, 1, b1);
            *(int*)((char*)p + 0x168) = a2;
        } else {
            *(int*)((char*)p + 0xBC) = 1;
            func_001C57C0(sub, 0, b0);
            *(int*)((char*)p + 0x168) = 0;
        }
    } else {
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x168) = 0;
    }
    if (a5 != 0) {
        *(int*)(sub + 0x10) = a5;
        *(int*)(sub + 0xC) = 1;
    }
    if (a6 != 0) {
        *(int*)(sub + 0x8) = a6;
        *(int*)(sub + 0x4) = 1;
    }
    *(int*)(sub + 0x94) = a1;
    *(int*)(sub + 0xA8) = 0;
    *(int*)(sub + 0x98) = 1;
    func_001A9BC0((cFEStateNet*)self, p);
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
            func_001B70D0(self, 0xD);
        } else if (self->miError == 1) {
            func_001B70D0(self, 0x10);
        } else if (self->miError == 5) {
            func_001B70D0(self, 0x19);
        } else if (self->miError == 6) {
            func_001B70D0(self, 0x19);
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
            func_001B70D0(self, 0xD);
        } else if (self->miError == 1) {
            func_001B70D0(self, 0x10);
        } else if (self->miError == 5) {
            func_001B70D0(self, 0x19);
        } else if (self->miError == 6) {
            func_001B70D0(self, 0x19);
        } else {
            func_001B70D0(self, 0x11);
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
    if (IsMode_001B7A08(0x15)) {
        func_001B70D0(self, 0x10);
    } else if (IsMode_001B7A08(0x16)) {
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
        func_001B70D0(self, 0x12);
    } else if (IsMode_001B7A50(0xD)) {
        func_001B70D0(self, 0x11);
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
            func_001B70D0(self, 0x17);
        } else {
            func_001B70D0(self, 0x15);
        }
    } else if (IsMode_001B7A98(5)) {
        int s = self->miError;
        if (s == 1) {
            self->unk_B08 = 1;
            self->unk_6D8 = 1;
            self->miError = 0x18;
            func_001B70D0(self, 0x17);
        } else if (s == 5) {
            func_001B70D0(self, 0x15);
        } else if (s == 6) {
            func_001B70D0(self, 0x15);
        } else if (s == 2) {
            if (func_00265CB0(D_004A33D0) != 0) {
                self->unk_B08 = 1;
                self->miError = 0x18;
            }
            func_001B70D0(self, 0x17);
            goto dflt;
        } else {
        dflt:
            func_001B70D0(self, 0x17);
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
    } else if (IsMode_001B7BC0(0x11)) {
        func_001B70D0(self, 0x17);
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
        func_001B70D0(self, 0x1B);
    } else if (IsMode_001B7C28(0x11)) {
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
        func_001B70D0(self, 0x1D);
    } else if (IsMode_001B7C70(0xD)) {
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
    case 19: func_001B70D0(self, 0x14); break;
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

extern char D_00461618[];


extern char D_004A12C0[];

extern char D_0045F7E0[];

extern char D_0045DC90[];

extern char D_0045DCD8[];

extern int* D_004A33D0;

extern char D_004642E0[];

extern "C" int func_00265CB0(void* p);

extern "C" int func_00265D68(void* p);

extern char D_004644D8[];

extern char D_004644F0[];

// sprintf1 is a varargs formatter (dst, fmt, ...); the plain declaration above is a 3-arg view.
extern "C" void sprintf1_v(void* dst, int fmt, ...) __asm__("sprintf1");

// func_002C2718 is a varargs formatter (buf, fmt, ...); the plain declaration above is a 3-arg view.
extern "C" void func_002C2718_v(char* buf, const char* fmt, ...) __asm__("func_002C2718");

extern "C" void func_001B8238_7E40(char* self) __asm__("func_001B8238");
extern "C" void func_001B83C0_7E40(char* self) __asm__("func_001B83C0");
extern char D_004644A8[];
extern char D_004644C0[];
extern char D_00464508[];

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
    int h1 = GetHashValue32(D_00461618);
    func_002C2718_v(buf, D_004A12C0, 1);
    switch (mode) {
    case 2:
        if (self->fB04 == 0) {
            if (func_00265CB0(D_004A33D0)) {
                sprintf1_v(self->text, Loc_B7E40(D_0045DC90), buf);
            } else {
                func_001B8238_7E40((char*)self);
            }
        } else {
            func_001B83C0_7E40((char*)self);
        }
        flag = 0;
        h1 = GetHashValue32(D_0045DCD8);
        h2 = GetHashValue32(D_0045F7E0);
        cur = self->text;
        break;
    case 3:
        sprintf1_v(self->text, Loc_B7E40(D_004644A8), buf, 0x11);
        flag = 0;
        h1 = GetHashValue32(D_0045DCD8);
        h2 = GetHashValue32(D_0045F7E0);
        cur = self->text;
        break;
    case 5:
        if (self->f6DC == 3) {
            flag = 0;
            int a = Loc_B7E40(D_004642E0);
            int b = Loc_B7E40(D_004644C0);
            sprintf1_v(self->text, b, a, buf);
        } else {
            func_001B83C0_7E40((char*)self);
        }
        h1 = GetHashValue32(D_0045DCD8);
        h2 = GetHashValue32(D_0045F7E0);
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
        h1 = GetHashValue32(D_0045DCD8);
        h2 = GetHashValue32(D_0045F7E0);
        cur = self->text;
        break;
    case 0:
        if (IsMode_001B7E40(0x16)) {
            if (func_00265D68(D_004A33D0)) {
                sprintf1_v(self->text, Loc_B7E40(D_004644D8), buf);
            } else {
                sprintf1_v(self->text, Loc_B7E40(D_004644F0), buf);
            }
            h1 = GetHashValue32(D_0045DCD8);
            h2 = GetHashValue32(D_0045F7E0);
            cur = self->text;
        }
        break;
    }
    if (flag && self->f6D8 != 0) {
        sprintf1_v(self->text, Loc_B7E40(D_00464508), buf);
        h1 = GetHashValue32(D_0045DCD8);
        h2 = GetHashValue32(D_0045F7E0);
        cur = self->text;
    }
    int r = -1;
    if (h2 == 0)
        r = 0;
    if (self->f6DC == 0x17) {
        h2 = 0;
        h1 = GetHashValue32(D_0045F7E0);
    }
    func_001B7650((cFEStateNetAccountMem*)self, r, 0, h1, h2, 0, (int)cur);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B8238);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: func_002C2718/sprintf1 are varargs formatters; the unit declares fixed-arg views
extern "C" void func_002C2718_1B8238(char* buf, const char* fmt, ...) __asm__("func_002C2718");
extern "C" void sprintf1_1B8238(void* dst, int fmt, ...) __asm__("sprintf1");
extern char D_004A12C0[];
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
extern char D_004642E0[];
extern char D_00463F20[];
extern char D_00464588[];
extern char D_004644D8[];

extern "C" void func_001B8238(cFEStateNetAccountMem* self)
{
    char buf[32];
    func_002C2718_1B8238(buf, D_004A12C0, 1);
    switch (self->miState) {
    case 3:
    case 23: {
        int name = Loc_001B8238(D_004642E0);
        int str = Loc_001B8238(D_00463F20);
        sprintf1_1B8238(((char*)self) + 0x702, str, 1, name);
        break;
    }
    case 9:
    case 15:
    case 17: {
        int str = Loc_001B8238(D_00464588);
        sprintf1_1B8238(((char*)self) + 0x702, str, 1, 0x11);
        break;
    }
    case 28: {
        int str = Loc_001B8238(D_004644D8);
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
extern "C" void func_002C2718_1B83C0(char* buf, const char* fmt, ...) __asm__("func_002C2718");
extern "C" void sprintf1_1B83C0(void* dst, int fmt, ...) __asm__("sprintf1");
extern char D_004A12C0[];
extern char D_00464608[];
extern char D_00464628[];
extern char D_00464640[];
extern char D_004644F0[];

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
    func_002C2718_1B83C0(buf, D_004A12C0, 1);
    switch (self->miState) {
    case 3:
        SetText_001B83C0(((char*)self), D_00464608, buf);
        break;
    case 9:
    case 17:
    case 28:
        SetText_001B83C0(((char*)self), D_00464628, buf);
        break;
    case 23:
        SetText_001B83C0(((char*)self), D_00464640, buf);
        break;
    case 15:
        SetText_001B83C0(((char*)self), D_004644F0, buf);
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
    int v;
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
                self->miResult = 0x11;
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
            v = self->miResult;
            if (v != 0) {
                ((cListener_001B84D8*)self->mpParentState)->v17(self, v);
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
            self->miResult = 0x11;
            break;
        case 16:
        case 20:
            func_001A97B8_v(self, w, msg);
            self->miResult = 0xF;
            break;
        case 22:
            func_001A97B8_v(self, w, 22);
            v = self->miResult;
            if (v != 0) {
                ((cListener_001B84D8*)self->mpParentState)->v17(self, v);
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
            v = self->miResult;
            if (v != 0) {
                ((cListener_001B84D8*)self->mpParentState)->v17(self, v);
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
INCLUDE_ASM("fe/feasyncfile", func_001B8860);
#ifdef SKIP_ASM
class func_001B8860_cObj {
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

extern "C" int func_001B8860(cFEStateNetAccountMem* self, int a1, int a2)
{
    return ((func_001B8860_cObj*)self->mpParentState)->v24(a1, a2);
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
extern int D_004A3E90;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

/**
 * Constructs step 1 of the online account creation (or editing) screen.
 *
 * Runs the cFEStateNet constructor (func_001A8500, no parent), stores cFEStateCreateStep1_vtbl, clears
 * mpStaticContinue and unk_6A8, points the saved password, confirmation and mail texts at D_004A3E90 (an empty
 * string), stores @p bNewAccount in mbEditAccount and mHashName 0x4D when it is set, else 0x50. Despite the field's name a
 * nonzero flag means a new account: the callers pass 1 from "create account" (func_001BF758,
 * cFEStateOnlineAgreement id 0x14, Step2's back) and the screen then uses LobbyMan's mNewAccount form.
 *
 * @param pEngine  the UI engine (On Tour cUIState ctor pEngine)
 * @param bNewAccount  nonzero to create a new account, 0 to edit the logged-in one (stored in mbEditAccount)
 * @return @p self
 */
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1(cFEStateCreateStep1* self, int pEngine, int bNewAccount)
{
    func_001A8500_3(self, pEngine, 0);
    int d = D_004A3E90;
    self->mpStaticContinue = 0;
    self->mVtbl = cFEStateCreateStep1_vtbl;
    self->mszSavedPassword = (char*)d;
    self->mszSavedConfirm = (char*)d;
    self->mszSavedMail = (char*)d;
    if (bNewAccount != 0) {
        self->mHashName = 0x4D;
    } else {
        self->mHashName = 0x50;
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
extern char D_00464740[];

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
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00464740), 0);
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
 * Runs one frame of account creation step 1: only the cFEStateNet update (func_001A8770).
 *
 * @return func_001A8770()'s result (the pending-event handler's value)
 */
void* cFEStateCreateStep1_onUpdate(void* self)
{
    return func_001A8770((cFEStateNet*)self);
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
            void* o = (char*)obj + vt[8].delta;
            vt[8].fn(o, !func_001B9558(self));
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
extern char D_00461520[];
extern char D_00462B88[];
extern char D_004631D8[];
extern char D_00464750[];
extern char D_00464768[];
extern char D_00464780[];
extern char D_00464790[];
extern char D_004647A8[];

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
    if (IsHash1B8AA0(*(int*)((char*)text + 0x38), D_00461520)) {
        if (self->mbEditAccount == 0) {
            cUIText_setUnicodeStringByID(text, GetHashValue32(D_00464750));
        }
    } else if (IsHash1B8AA0(*(int*)((char*)text + 0x38), D_00464768) || IsHash1B8AA0(*(int*)((char*)text + 0x38), D_00462B88)) {
        if (self->mbEditAccount == 0) {
            obj->v25(1);
            obj->v08(1);
        }
    } else if (IsHash1B8AA0(*(int*)((char*)text + 0x38), D_00464780) || IsHash1B8AA0(*(int*)((char*)text + 0x38), D_00464790)) {
        func_001A9A18((cFEStateNet*)self, (cUIListBox_001A9A18*)text, 1);
    } else if (IsHash1B8AA0(*(int*)((char*)text + 0x38), D_004647A8)) {
        if (self->mbEditAccount != 0) {
            obj->v25(1);
            obj->v08(1);
        } else {
            cUIText_setUnicodeStringByID(text, GetHashValue32(D_004631D8));
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
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
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
extern "C" int func_0039A738(void* a);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char D_00464780[];
extern char D_00464790[];
extern char D_00464768[];
extern char D_00462B88[];
extern char D_004647B8[];
extern char D_004647D0[];
extern char D_00462BA0[];
extern char D_00462D00[];
extern char D_004647E0[];
extern char D_004647F8[];
extern char D_00464810[];
extern char D_00464828[];
extern char D_00464838[];
extern char D_00462CD0[];
extern char D_004647A8[];
extern char D_00464848[];
extern char D_00462D10[];
extern const char D_00462D68[];
extern char D_00464860[];

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
        if (NameHash1B8C38(w) == GetHashValue32(D_00464780) && w != 0) {
            *(int*)((char*)self->mpAccount + 0x38) = func_0039A738(w) == 1;
        }
        if (NameHash1B8C38(w) == GetHashValue32(D_00464790) && w != 0) {
            *(int*)((char*)self->mpAccount + 0x3C) = func_0039A738(w) == 1;
        }
        break;
    case 5:
        if (NameHash1B8C38(w) == GetHashValue32(D_00464768)) {
            cUIText* t = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462B88));
            if (t != 0) {
                func_001A9150((cFEStateNet*)self, (int)t, GetHashValue32(D_004647B8), *(int*)((char*)self->mpAccount + 0x0), 0, 10, 0x10);
                *(int*)((char*)self->mpKeyboard + 0x58) = 1;
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(D_004647D0)) {
            cUIText* t = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462BA0));
            if (t != 0) {
                func_001A9150((cFEStateNet*)self, (int)t, GetHashValue32(D_00462D00), *(int*)((char*)self->mpAccount + 0x4), 1, 11, 0x10);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(D_004647E0)) {
            cUIText* t = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004647F8));
            if (t != 0) {
                func_001A9150((cFEStateNet*)self, (int)t, GetHashValue32(D_00464810), *(int*)((char*)self->mpAccount + 0x8), 1, 12, 0x10);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(D_00464828)) {
            cUIText* t = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464838));
            if (t != 0) {
                func_001A9150((cFEStateNet*)self, (int)t, GetHashValue32(D_00462CD0), *(int*)((char*)self->mpAccount + 0xC), 0, 13, 0x32);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            }
        } else if (NameHash1B8C38(w) == GetHashValue32(D_004647A8)) {
            if (func_001BA110(self) != 0) {
                if (self->mbEditAccount != 0) {
                    func_001A8A40(((char*)self), cFEStateCreateStep2_cFEStateCreateStep2((cFEStateCreateStep2*)((cFEStateNet*)cMemMan_alloc(0x6E0, D_00464848, 0, 0)), (int)self->mpEngine));
                } else {
                    func_001BA280(self);
                }
            }
        }
        break;
    case 6:
        if (self->mbEditAccount != 0) {
            void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, ((char*)self), 0, 0, 0);
            *(int*)((char*)p + 0x18) = 0x832;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        } else {
            func_001BA218(self);
            func_001A8A40(((char*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
        }
        break;
    case 13: {
        func_001B9820(self);
        char* o = (char*)self->mpStaticContinue;
        if (o != 0) {
            sVE_001B8C38* vt = *(sVE_001B8C38**)(o + 8);
            vt[8].fn(o + vt[8].delta, !func_001B9558(self));
        }
        break;
    }
    case 8: {
        void* p = cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(0x6DC, D_00464860, 0x100, 0), (int)self->mpEngine);
        if (self->mbEditAccount != 0) {
            *(int*)((char*)p + 0x18) = 0x14;
        } else {
            *(int*)((char*)p + 0x18) = 0x15;
        }
        func_001A8A40(((char*)self), p);
        break;
    }
    case 14:
        break;
    default:
        func_001A8918((cFEStateNet*)self, (int)w, msg);
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
extern char D_004648A8[];
extern char D_004648C8[];
extern char D_004648E0[];
extern char D_004648F8[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];
extern char D_00464910[];
extern char D_00464930[];

extern "C" void func_001B9088(cFEStateCreateStep1* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_004648A8);
    func_001DA530(menu, 0, GetHashValue32(D_004648C8));
    func_001DA530(menu, 1, GetHashValue32(D_004648E0));
    func_001DA530(menu, 2, GetHashValue32(D_004648F8));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 4, GetHashValue32(D_00464910), 2);
    *(int*)((char*)menu + 0x64) = GetHashValue32(D_00464930);
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
extern char D_00462D10[];
extern const char D_00462D68[];

extern "C" void func_001B91C8(cFEStateCreateStep1* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(self, a1, 15);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, self, 1,
                                ((int*)self->mpAccount)[0], ((int*)self->mpAccount)[1]);
        *(int*)((char*)p + 0x18) = 0x82C;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 16:
    case 20:
        func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
        break;
    default:
        func_001A97B8(self, a1, kind);
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
extern const char D_00462D68[];
extern char D_00462D80[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

// Tracks a pending flag across messages; on 0x16 either asks for confirmation or opens the next screen.
extern "C" void func_001B92C8(cFEStateCreateStep1* self, void* a1, int a2)
{
    switch (a2) {
    case 0xF: case 0x10: case 0x14:
        self->unk_6E8 = 0;
        func_001A97B8(self, a1, a2);
        break;
    case 0x11:
        self->unk_6E8 = 1;
        func_001A97B8(self, a1, a2);
        break;
    case 0x16:
        if (self->unk_6E8 != 0) {
            func_001A97B8(self, a1, a2);
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D80);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
            func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
            *(int*)((char*)p + 0x150) = 0x81E;
            *(int*)((char*)p + 0x168) = 0;
            *(int*)((char*)p + 0x164) = 0;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
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
    case 0x81E:
        func_001B91C8(self, pPopup, Event);
        break;
    case 0x82C:
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
extern char D_00462D80[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

extern "C" int func_001B9488(cFEStateCreateStep1* self, int a1, int a2)
{
    if (a1 == 0xCE) {
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 2;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D80);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)((char*)p + 0x168) = 0;
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x81E;
        func_001A9BC0((cFEStateNet*)self, p);
        return -1;
    }
    return func_001A8E40_3i(self, a1, a2);
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
extern char D_00462B88[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);

extern "C" void func_001B95C0(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462B88));
    if (text != 0) {
        cUIText_setAsciiString(text, *(const char**)self->mpAccount);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001B9610);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00462BA0[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A98B8(cFEStateNet* self, const char* str);

extern "C" void func_001B9610(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00462BA0));
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
extern char D_004647F8[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001A98B8(cFEStateNet* self, const char* str);

extern "C" void func_001B9678(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004647F8));
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
extern char D_00464838[];
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001B9950(cFEStateCreateStep1* self, cUIText* text, char* str);

extern "C" void func_001B96E0(cFEStateCreateStep1* self)
{
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464838));
    if (text != 0) {
        char* s = ((char**)self->mpAccount)[3];
        if (*(int*)(s - 8) != 0) {
            func_001B9950(self, text, s);
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
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464780[];

extern "C" void func_001B9740(cFEStateCreateStep1* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464780));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)self->mpAccount + 0x38) != 0, &out);
        func_0039A7A8(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001B97B0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464790[];

extern "C" void func_001B97B0(cFEStateCreateStep1* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464790));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)self->mpAccount + 0x3C) != 0, &out);
        func_0039A7A8(box, out);
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
    char* p = (char*)self->mpKeyboard;
    switch (*(unsigned int*)(p + 0x18)) {
    case 11:
        if (self->mpKeyboardText != 0) {
            func_001A98B8((cFEStateNet*)self, p + 0x74);
            cUIText_setAsciiString(self->mpKeyboardText, (char*)self + 0x5C);
        }
        cBXString_cBXString4((char*)self->mpAccount + 0x4, (char*)self->mpKeyboard + 0x74);
        break;
    case 12:
        if (self->mpKeyboardText != 0) {
            func_001A98B8((cFEStateNet*)self, p + 0x74);
            cUIText_setAsciiString(self->mpKeyboardText, (char*)self + 0x5C);
        }
        cBXString_cBXString4((char*)self->mpAccount + 0x8, (char*)self->mpKeyboard + 0x74);
        break;
    case 10:
        if (self->mpKeyboardText != 0) {
            cUIText_setAsciiString(self->mpKeyboardText, p + 0x74);
        }
        cBXString_cBXString4((char*)self->mpAccount, (char*)self->mpKeyboard + 0x74);
        break;
    case 13: {
        cBXString_cBXString4((char*)self->mpAccount + 0xC, p + 0x74);
        char* s = *(char**)((char*)self->mpAccount + 0xC);
        if (((int*)s)[-2] != 0) {
            func_001B9950(self, self->mpKeyboardText, s);
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
extern "C" char* func_004162D0(char*, const char*);
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
    char buf[0x50];
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
        func_004162D0(buf, ell);
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
extern char D_00462BF0[];
extern char D_004624D0[];
extern char D_00464960[];
extern char D_00461618[];

extern "C" int func_001B9B18(cFEStateCreateStep1* self)
{
    int ok = 1;
    sObj1B9B18 o;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&o));
    int len = strlen(*(const char**)((char*)self->mpAccount + 0xC));
    if (len == 0) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BF0);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    }
    if (len < 7) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004624D0);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    } else if (!func_001B9CD8(self, *(const char**)((char*)self->mpAccount + 0xC))) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464960);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
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
extern char D_00462BD0[];
extern char D_004624B0[];
extern char D_00464978[];
extern char D_00461618[];

extern "C" int func_001B9D68(cFEStateCreateStep1* self)
{
    int ok = 1;
    sObj1B9D68 o;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&o));
    int len = strlen(*(const char**)self->mpAccount);
    if (len == 0) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BD0);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    } else if (len < 4 || len > 16) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004624B0);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    } else {
        signed char c0 = (*(signed char**)self->mpAccount)[0];
        if (c0 > '0') {
            if (c0 < '9') {
                void* p = func_001A9C50((cFEStateNet*)self);
                *(int*)((char*)p + 0xBC) = 1;
                char* sub = (char*)p + 0xBC;
                *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464978);
                ok = 0;
                *(int*)((char*)p + 0xC0) = 3;
                func_001C57C0(sub, 0, GetHashValue32(D_00461618));
                *(int*)((char*)p + 0x150) = 0x80D;
                func_001A9BC0((cFEStateNet*)self, p);
                goto done;
            }
        }
        int i;
        for (i = 0; i < len; i++) {
            signed char c = (*(signed char**)self->mpAccount)[i];
            if (c != ' ' && !func_001B9F68(self, c, '0', '9') && !func_001B9F68(self, c, 'a', 'z') && !func_001B9F68(self, c, 'A', 'Z')) {
                ok = 0;
                void* p = func_001A9C50((cFEStateNet*)self);
                *(int*)((char*)p + 0xBC) = 1;
                char* sub = (char*)p + 0xBC;
                *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464978);
                *(int*)((char*)p + 0xC0) = 3;
                func_001C57C0(sub, 0, GetHashValue32(D_00461618));
                *(int*)((char*)p + 0x150) = 0x80D;
                func_001A9BC0((cFEStateNet*)self, p);
                break;
            }
        }
    }
done:
    cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
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
extern char D_00462BB0[];
extern char D_00464998[];
extern char D_004649B8[];
extern char D_00461618[];

extern "C" int func_001B9FA0(cFEStateCreateStep1* self)
{
    int ok = 1;
    sObj1B9FA0 o;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&o));
    unsigned int len = strlen(*(const char**)((char*)self->mpAccount + 4));
    if (len == 0) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462BB0);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    } else if (len - 4 > 12) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464998);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    } else if (func_004165A8(*(const char**)((char*)self->mpAccount + 4), *(const char**)((char*)self->mpAccount + 8))) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004649B8);
        ok = 0;
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
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
        char* d = (char*)self->mpAccount;
        self->miSavedSpam[0] = *(int*)(d + 0x38);
        self->miSavedSpam[1] = *(int*)(d + 0x3C);
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
extern const char D_00462D68[];
extern char D_004649D8[];

extern "C" void func_001BA280(cFEStateCreateStep1* self)
{
    func_001BA358(self);
    char* p = (char*)self->mpAccount;
    if (*(int*)(p + 0x4C) == 0 && *(int*)(p + 0x44) == 0 && *(int*)(p + 0x48) == 0) {
        func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
    } else {
        void* pp = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)pp + 0xBC) = 0;
        *(int*)((char*)pp + 0xC4) = GetHashValue32(D_004649D8);
        *(int*)((char*)pp + 0xC0) = 3;
        *(int*)((char*)pp + 0x150) = 0x81D;
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
    char* d = (char*)self->mpAccount;
    if (*(int*)(d + 0x38) != self->miSavedSpam[0] || *(int*)(d + 0x3C) != self->miSavedSpam[1]) {
        *(int*)(d + 0x48) = 1;
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
    self->mHashName = 0x4E;
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
extern char D_004649F8[];


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
    void* screen = cUIEngine_addScreenByHashName(engine, (self), GetHashValue32(D_004649F8), 0);
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
 * Runs one frame of account creation step 2: only the cFEStateNet update (func_001A8770).
 *
 * @return func_001A8770()'s result (the pending-event handler's value)
 */
void* cFEStateCreateStep2_onUpdate(void* self)
{
    return func_001A8770((cFEStateNet*)self);
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
extern char D_00464A08[];
extern char D_004A1BD8[];
extern char D_004A1BE0[];
extern char D_004A1BE8[];
extern char D_00464A18[];

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
    if (IsHash1BA500(*(int*)((char*)box + 0x38), D_00464A08)) {
        func_001A9A18((cFEStateNet*)self, (cUIListBox_001A9A18*)box, 0);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_004A1BD8)) {
        func_001BB5D0(self, box, 1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_004A1BE0)) {
        func_001BB840(self, box, 1, 0x7C1, 1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_004A1BE8)) {
        func_001BB798(self, box, 0x7C1);
    } else if (IsHash1BA500(*(int*)((char*)box + 0x38), D_00464A18)) {
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
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1(cFEStateCreateStep1* self, int a1, int a2);
extern "C" void func_001BA9F8(cFEStateCreateStep2* self, int a1);
extern "C" void func_001BB840(cFEStateCreateStep2* self, void* box, int a2, int a3, int a4);
int func_001BBAA8(void* self);
extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement(cFEStateOnlineAgreement* self, int a1);
extern "C" int func_0039A738(void* a);
extern char D_004A1BD8[];
extern char D_004A1BE0[];
extern char D_004A1BE8[];
extern char D_004A12C0[];
extern char D_00464A08[];
extern char D_00464A18[];
extern char D_00464A28[];
extern char D_00462ED0[];
extern char D_00464860[];

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
            if (IsHash_001BA618(*(int*)(obj + 0x38), D_004A1BD8)) {
                *(int*)((char*)self->mpAccount + 0x28) = func_0039A738(obj);
                cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1BE0));
                if (box != 0) {
                    char* d = (char*)self->mpAccount;
                    func_001BB840(self, box, *(int*)(d + 0x28), *(int*)(d + 0x30), *(int*)(d + 0x2C));
                }
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), D_004A1BE8)) {
                char buf[16];
                *(int*)((char*)self->mpAccount + 0x30) = *(unsigned char*)(obj + 0x319) + 0x76C;
                sprintf(buf, D_004A12C0, *(int*)((char*)self->mpAccount + 0x30));
                cUIListBox_setEntryByAsciiString(obj, 0, buf, 0);
                cBXString_Reset((char*)self->mpAccount + 0x10);
                cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1BE0));
                if (box != 0) {
                    char* d = (char*)self->mpAccount;
                    func_001BB840(self, box, *(int*)(d + 0x28), *(int*)(d + 0x30), *(int*)(d + 0x2C));
                }
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), D_004A1BE0)) {
                *(int*)((char*)self->mpAccount + 0x2C) = func_0039A738(obj);
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), D_00464A18)) {
                *(int*)((char*)self->mpAccount + 0x34) = func_0039A738(obj);
            }
            if (IsHash_001BA618(*(int*)(obj + 0x38), D_00464A08)) {
                *(int*)((char*)self->mpAccount + 0x40) = func_0039A738(obj) == 1;
            }
        }
        break;
    case 5:
        if (IsHash_001BA618(*(int*)(obj + 0x38), D_00464A28)) {
            if (func_001BBAA8(self) != 0) {
                func_001BA9F8(self, 1);
            }
        }
        break;
    case 6:
        func_001A8A40(self, cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(0x6F4, D_00462ED0, 0, 0), (int)self->mpEngine, 1));
        break;
    case 8: {
        char* p = (char*)cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(0x6DC, D_00464860, 0x100, 0), (int)self->mpEngine);
        *(int*)(p + 0x18) = 0x16;
        func_001A8A40(self, p);
        break;
    }
    default:
        func_001A8918((cFEStateNet*)self, pWidget, msg);
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
extern char D_004648A8[];
extern char D_004648C8[];
extern char D_004648E0[];
extern char D_004648F8[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];
extern char D_00464910[];
extern char D_00464930[];

extern "C" void func_001BA8B8(cFEStateCreateStep2* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_004648A8);
    func_001DA530(menu, 0, GetHashValue32(D_004648C8));
    func_001DA530(menu, 1, GetHashValue32(D_004648E0));
    func_001DA530(menu, 2, GetHashValue32(D_004648F8));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 3, GetHashValue32(D_004614E8), 1);
    func_001DA510(menu, 4, GetHashValue32(D_00464910), 2);
    *(int*)((char*)menu + 0x64) = GetHashValue32(D_00464930);
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
extern char D_00464A38[];

extern "C" void func_001BA9F8(cFEStateCreateStep2* self, int a1)
{
    void* p = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464A38);
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0x150) = 0x820;
    func_001A9BC0((cFEStateNet*)self, p);
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
    case 0x81F:
        func_001BB158(self, pPopup, Event);
        break;
    case 0x821:
        func_001BB0E8(self, pPopup, Event);
        break;
    case 0x822:
        func_001BB098(self, pPopup, Event);
        break;
    case 0x80E:
        func_001BADE8(self, pPopup, Event);
        break;
    case 0x82C:
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
extern const char D_00462D68[];
extern char D_00464AD0[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

extern "C" int func_001BAB28(cFEStateCreateStep2* self, int id, int a2)
{
    switch (id) {
    case 0xC9:
        func_0025B9C8(LobbyMan_mLobbyMan, (char*)LobbyMan_mLobbyMan + 0x260);
        break;
    case 0xCB: {
        void* p = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)p + 0xBC) = 2;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464AD0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x81F;
        func_001A9BC0(((cFEStateNet*)self), p);
        break;
    }
    case 0xD4:
        func_00260F10(LobbyMan_mLobbyMan);
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
        break;
    case 0x102:
        func_001BAC90(self, (void*)a2);
        break;
    default:
        return func_001A8E40_3i(((cFEStateNet*)self), id, a2);
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
extern char D_00463090[];
extern char D_00462338[];
extern char D_00461618[];

extern "C" void func_001BAC90(cFEStateCreateStep2* self, void* msg)
{
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x6475706C:
        func_001A8A40(self, cFEStateCreateDuplicate_cFEStateCreateDuplicate((cFEStateCreateDuplicate*)cMemMan_alloc(0x6E0, D_00463090, 0, 0), (int)self->mpEngine, 1));
        break;
    case 0x746F6F79:
        func_001BBAC8(self, *(int*)((char*)msg + 0xC));
        break;
    case 0x706D616C:
        // PORT: string length lives in a header before the character data
        if (*(int*)(*(int*)((char*)self->mpAccount + 0x10) - 8) != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 1;
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462338);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)p + 0x150) = 0x822;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            func_001BBAC8(self, 0x11);
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
extern char D_00462D10[];

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

extern "C" void func_001BADE8(cFEStateCreateStep2* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A97B8_v(((cFEStateNet*)self), a1, a2);
        func_00261408(GetSession_001BADE8());
        func_0025B800(LobbyMan_mLobbyMan);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8_v(((cFEStateNet*)self), a1, a2);
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
extern char D_00462D80[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];
extern char D_00462C70[];
extern char D_00462C88[];

// Tracks a pending flag; on 0x16 shows either the confirm popup or the network-connect popup.
extern "C" void func_001BAED8(cFEStateCreateStep2* self, void* a1, int a2)
{
    switch (a2) {
    case 0xF: case 0x10: case 0x14:
        self->unk_6D8 = 0;
        func_001A97B8_v(self, a1, a2);
        break;
    case 0x11:
        self->unk_6D8 = 1;
        func_001A97B8_v(self, a1, a2);
        break;
    case 0x16:
        func_001A97B8_v(self, a1, a2);
        if (self->unk_6D8 != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D80);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
            func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
            *(int*)((char*)p + 0x164) = 0;
            *(int*)((char*)p + 0x150) = 0x81F;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 1;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462C70);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00462C88));
            *(int*)((char*)p + 0x150) = 0x80E;
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

extern "C" void func_001BB098(cFEStateCreateStep2* self, void* a1, int a2)
{
    if (a2 == 0x16) {
        func_001A97B8(self, a1, a2);
        func_001BBAC8(self, 0x11);
    } else {
        func_001A97B8(self, a1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB0E8);
#ifdef SKIP_ASM
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BA9F8(cFEStateCreateStep2* self, int a1);

extern "C" void func_001BB0E8(cFEStateCreateStep2* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(self, a1, 0xF);
        cBXString_cBXString4((char*)self->mpAccount + 0x10, *(const char**)((char*)a1 + 0x2B0));
        func_001BA9F8(self, 0);
    } else {
        func_001A97B8(self, a1, a2);
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
extern char D_00462D10[];
extern char D_00462C70[];
extern char D_00462C88[];

extern "C" void func_001BB158(cFEStateCreateStep2* self, void* a1, unsigned int kind)
{
    switch (kind) {
    case 15: {
        func_001A97B8(self, a1, 15);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, self, 1,
                                *(int*)self->mpAccount, *(int*)((char*)self->mpAccount + 4));
        *(int*)((char*)p + 0x18) = 0x82C;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 16:
    case 20: {
        func_001A97B8(self, a1, kind);
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462C70);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00462C88));
        *(int*)((char*)p + 0x150) = 0x80E;
        func_001A9BC0((cFEStateNet*)self, p);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, *(const char**)self->mpAccount);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, *(const char**)((char*)self->mpAccount + 4));
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    }
    default:
        func_001A97B8(self, a1, kind);
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
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A1BD8[];

extern "C" void func_001BB2D0(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1BD8));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)self->mpAccount + 0x28), &out);
        func_0039A7A8(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB340);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A1BE0[];

extern "C" void func_001BB340(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1BE0));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)self->mpAccount + 0x2C), &out);
        func_0039A7A8(box, out);
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
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A1BE8[];
extern char D_004A12C0[];

extern "C" void func_001BB3B0(cFEStateCreateStep2* self)
{
    char buf[16];
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1BE8));
    if (box != 0) {
        sprintf(buf, D_004A12C0, *(int*)((char*)self->mpAccount + 0x30));
        cUIListBox_setEntryByAsciiString(box, 0, buf, 0);
        func_0039A7A8(box, (unsigned char)(*(int*)((char*)self->mpAccount + 0x30) - 0x6C));
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB440);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464A18[];

extern "C" void func_001BB440(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464A18));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)self->mpAccount + 0x34), &out);
        func_0039A7A8(box, out);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB4B0);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464A08[];

extern "C" void func_001BB4B0(cFEStateCreateStep2* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00464A08));
    if (box != 0) {
        unsigned char out = 0;
        func_0039A768(box, *(int*)((char*)self->mpAccount + 0x40) != 0, &out);
        func_0039A7A8(box, out);
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
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464B10[];
extern char D_00464B20[];

extern "C" void func_001BB520(cFEStateCreateStep2* self, void* box, int flag)
{
    unsigned char out = 0;
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B10), 0);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B20), 1);
    func_0039A768(box, flag == 0, &out);
    func_0039A7A8(box, out);
    ((cFEVBox_001BB520*)box)->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB5D0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00464B30[];
extern char D_00464B40[];
extern char D_00464B50[];
extern char D_00464B60[];
extern char D_00464B70[];
extern char D_00464B80[];
extern char D_00464B90[];
extern char D_00464BA0[];
extern char D_00464BB0[];
extern char D_00464BC0[];
extern char D_00464BD0[];
extern char D_00464BE0[];

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
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B30), 1);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B40), 2);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B50), 3);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B60), 4);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B70), 5);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B80), 6);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464B90), 7);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464BA0), 8);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464BB0), 9);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464BC0), 10);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464BD0), 11);
    cUIListBox_addEntryByStringID(box, GetHashValue32(D_00464BE0), 12);
    func_0039A768(box, 1, &out);
    func_0039A7A8(box, out);
    func_0039A7A8(box, 1);
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
extern "C" void func_0039A8D8(void* self, int count);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_004A12C0[];

extern "C" void func_001BB798(cFEStateCreateStep2* self, void* box, int value)
{
    char buf[16];
    func_0039A670(box);
    *(int*)((char*)box + 0x14) = (*(int*)((char*)box + 0x14) & ~1) | 0x80;
    func_0039A8D8(box, 0x67);
    sprintf(buf, D_004A12C0, value);
    cUIListBox_setEntryByAsciiString(box, 0, buf, 0);
    func_0039A7A8(box, (unsigned char)(value - 0x6C));
    ((cFEVBox_001BB798*)box)->v09(1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BB840);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_001BB9B0(cFEStateCreateStep2* self, int month, int year);
extern "C" void func_0039A670(void* self);
extern "C" int func_0039A768(void* self, int key, unsigned char* out);
extern "C" void func_0039A7A8(void* self, int a1);
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* self, const char* text, int data);
extern char D_004A12C0[];

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
        sprintf(buf, D_004A12C0, i);
        cUIListBox_addEntryByAsciiString(box, buf, i);
    }
    if (n < sel) {
        sel = n;
        *(int*)((char*)self->mpAccount + 0x2C) = sel;
    }
    idx = 0;
    func_0039A768(box, sel, &idx);
    func_0039A7A8(box, idx);
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
extern "C" int func_001BBA18(cFEStateCreateStep2* self, const char* s)
{
    int hasDot = 0;
    int hasAt = 0;
    int n = strlen(s);
    int i;
    for (i = 0; i < n; i++) {
        if (s[i] == '@')
            hasAt = 1;
        else if (s[i] == '.')
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
extern char D_00464BF0[];
extern char D_00464C08[];
extern char D_00464C20[];

struct sObj1BBAC8 {
    char data[0xD0];
};

// Shows the error popup for a failed network operation; returns 1 if the code needs no popup.
// PORT: callers declare func_001BBAC8 as void; defined under an asm label with its int result.
extern "C" int func_001BBAC8_impl(void* self, int code) __asm__("func_001BBAC8");
extern "C" int func_001BBAC8_impl(void* self, int code)
{
    sObj1BBAC8 o;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&o));
    if (code < 13) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464BF0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
        cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
        return 0;
    } else if (code < 18) {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = -1;
        *(int*)((char*)p + 0x180) = 0x32;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xCC) = GetHashValue32(D_00464C08);
        *(int*)((char*)p + 0xC8) = 3;
        *(int*)((char*)p + 0xD4) = GetHashValue32(D_00464C08);
        *(int*)((char*)p + 0xD0) = 3;
        *(int*)((char*)p + 0xDC) = *(int*)(*(char**)((char*)self + 0x6D0) + 0x10);
        *(int*)((char*)p + 0xD8) = 2;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464C20);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x821;
        func_001A9BC0((cFEStateNet*)self, p);
        cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
        return 0;
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
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
extern char D_00464C38[];
extern char D_00461618[];
// PORT: the unit declares func_001BBC40 as returning void*; the body returns 0/1
extern "C" int func_001BBC40_r(void* self) __asm__("func_001BBC40");

extern "C" int func_001BBC40_r(void* self)
{
    sObj1BBC40 o;
    cPopupInfo_cPopupInfo((cPopupInfo*)(&o));
    if (*(int*)((char*)*(void**)((char*)self + 0x6D0) + 0x40) == 0) {
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464C38);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x80D;
        func_001A9BC0((cFEStateNet*)self, p);
        cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
        return 0;
    }
    cPopupInfo__cPopupInfo((cPopupInfo*)(&o), 2);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateCreateDuplicate_cFEStateCreateDuplicate);
#ifdef SKIP_ASM
extern void* cFEStateCreateDuplicate_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateCreateDuplicate_cFEStateCreateDuplicate(cFEStateCreateDuplicate* self, int a1, int a2)
{
    func_001A8500_3(self, a1, 0);
    self->mbAccount = a2;
    self->mVtbl = cFEStateCreateDuplicate_vtbl;
    self->mHashName = 0x4F;
    self->unk_6A8 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBD60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern "C" void func_001BC400(cFEStateCreateDuplicate* self);
extern char D_00464C50[];

extern "C" void func_001BBD60(cFEStateCreateDuplicate* self)
{
    self->mpAccount = (tLobbyAccount*)((char*)LobbyMan_mLobbyMan + 0x260);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00464C50), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001BC400(self);
    func_001A8768(self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBDE0__FPv);
#ifdef SKIP_ASM
void* func_001BBDE0(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BBE00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char D_00464C68[];
extern char D_00461520[];
extern char D_00464C78[];
extern char D_00464C90[];
extern char D_00464CA0[];
extern char D_00464CB8[];

static inline int IsHash1BBE00(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001BBE00(cFEStateCreateDuplicate* self, cUIText* text)
{
    char buf[0x40];
    if (IsHash1BBE00(*(int*)((char*)text + 0x38), D_00464C68)) {
        *(int*)((char*)text + 0x18) = 4;
    } else if (self->mbAccount == 0 && IsHash1BBE00(*(int*)((char*)text + 0x38), D_00461520)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00464C78));
    } else if (self->mbAccount == 0 && IsHash1BBE00(*(int*)((char*)text + 0x38), D_00464C90)) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00464CA0));
    } else {
        int i;
        for (i = 0; i < 4; i++) {
            sprintf(buf, D_00464CB8, i);
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
INCLUDE_ASM("fe/feasyncfile", func_001BBF18);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
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
extern char D_00462D10[];
extern const char D_00462D68[];
extern char D_00464C78[];
extern char D_00464CC8[];

struct sStrs_001BBF18 {
    char pad[0x14];
    char* strs[1];
};

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

// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void func_001BBF18(cFEStateCreateDuplicate* self, int a1, unsigned int msg)
{
    switch (msg) {
    case 5: {
        a1 = *(int*)((char*)a1 + 0x18);
        if (a1 == 4) {
            if (self->mbAccount != 0) {
                // PORT: string pointer passed through an int parameter (func_001A9150's a3)
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(D_00464CC8), (int)D_004A1408, 0, -1, 0x10);
                *(int*)((char*)self->mpKeyboard + 0x58) = 1;
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            } else {
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(D_00464C78), (int)D_004A1408, 0, -1, 0xC);
                *(int*)((char*)self->mpKeyboard + 0x58) = 1;
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
                cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            }
        } else if (self->mbAccount != 0) {
            sStrs_001BBF18* s = (sStrs_001BBF18*)self->mpAccount;
            cBXString_operatorE(s, &s->strs[a1]);
            func_001BC290(self);
        } else {
            func_001BC300(self, ((char**)((char*)self->mpAccount + 0x14))[a1]);
        }
        break;
    }
    case 6:
        if (self->mbAccount != 0) {
            func_00261408(GetSession_001BBF18());
            func_0025B800(LobbyMan_mLobbyMan);
            void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, self, 0, 0, 0);
            *(int*)((char*)p + 0x18) = 0x832;
            cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        } else {
            func_001A8A40(self, cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
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
        func_001A8918((cFEStateNet*)self, a1, msg);
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
extern char D_00464CE8[];
extern char D_00464D08[];
extern char D_00464D30[];
extern char D_00464D50[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001BC190(cFEStateCreateDuplicate* self, void* menu)
{
    func_001DA528(menu, 3);
    if (self->mbAccount != 0) {
        *(int*)((char*)menu + 0x54) = GetHashValue32(D_00464CE8);
        func_001DA530(menu, 0, GetHashValue32(D_00464D08));
    } else {
        *(int*)((char*)menu + 0x54) = GetHashValue32(D_00464D30);
        func_001DA530(menu, 0, GetHashValue32(D_00464D50));
    }
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(menu, 2, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC290);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_001A9C50(cFEStateNet* self);
extern "C" void func_001A9BC0(cFEStateNet* self, void* popup);
extern "C" void func_0025B9C8(void* self, void* a1);
extern char D_00464A38[];

extern "C" void func_001BC290(cFEStateCreateDuplicate* self)
{
    void* p = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464A38);
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x820;
    *(int*)((char*)p + 0x164) = 0;
    func_001A9BC0((cFEStateNet*)self, p);
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
extern char D_004615F8[];
extern char D_00461618[];
extern char D_00462DE0[];

extern "C" void func_001BC300(cFEStateCreateDuplicate* self, const char* name)
{
    if (strlen(name) < 3) {
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004615F8);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x814;
        func_001A9BC0((cFEStateNet*)self, p);
    } else {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462DE0);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x815;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, p);
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

struct sNames1BC400 {
    char pad[0x14];
    char* mNames[4];
    int mCount;
};

extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_00464CB8[];
extern char D_004A1408[];

extern "C" void func_001BC400(cFEStateCreateDuplicate* self)
{
    char buf[64];
    int i;
    for (i = 0; i < 4; i++) {
        sprintf(buf, D_00464CB8, i);
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (text != 0) {
            sNames1BC400* names = (sNames1BC400*)self->mpAccount;
            if (i < names->mCount) {
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
extern const char D_00462D68[];
extern char D_00464AD0[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

// PORT: a2 is a message pointer passed as int
extern "C" int func_001BC4F0(cFEStateCreateDuplicate* self, int id, int a2)
{
    switch (id) {
    case 0xCB: {
        void* p = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)p + 0xBC) = 2;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464AD0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)((char*)p + 0x164) = 0;
        *(int*)((char*)p + 0x150) = 0x81F;
        func_001A9BC0(((cFEStateNet*)self), p);
        break;
    }
    case 0xD4:
        func_00260F10(LobbyMan_mLobbyMan);
    case 0xD6:
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
        break;
    case 0x102:
        if (*(unsigned int*)((char*)a2 + 8) == 0x6475706C) {
            char* q = (char*)self->mpPopup;
            if (q != 0) {
                int t = *(int*)(q + 0x18);
                if (t == 0x820 || t == 0x815) {
                    func_0039F190((char*)self->mpEngine + 0x18, 1);
                }
            }
            func_001BC400(self);
        } else {
            func_001A8E40_3i(((cFEStateNet*)self), id, a2);
        }
        break;
    default:
        return func_001A8E40_3i(((cFEStateNet*)self), id, a2);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BC6A0);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BC728(cFEStateCreateDuplicate* self, void* a1, int a2);
extern "C" void func_001BC818(cFEStateCreateDuplicate* self, void* a1, int a2);
extern "C" void func_001BC990(cFEStateCreateDuplicate* self, void* a1, int a2);

extern "C" void func_001BC6A0(cFEStateCreateDuplicate* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x81F:
        func_001BC818(self, a1, a2);
        break;
    case 0x80E:
        func_001BC728(self, a1, a2);
        break;
    case 0x82C:
        func_001BC990(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
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
extern char D_00462D10[];

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

extern "C" void func_001BC728(cFEStateCreateDuplicate* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_001A97B8_v(((cFEStateNet*)self), a1, a2);
        func_00261408(GetSession_001BC728());
        func_0025B800(LobbyMan_mLobbyMan);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, ((cFEStateNet*)self), 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8_v(((cFEStateNet*)self), a1, a2);
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
extern char D_00462D10[];
extern char D_00462C70[];
extern char D_00462C88[];

extern "C" void func_001BC818(cFEStateCreateDuplicate* self, void* a1, int kind)
{
    switch ((unsigned int)kind) {
    case 15: {
        func_001A97B8(self, a1, 15);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, self, 1,
                                *(int*)self->mpAccount, *(int*)((char*)self->mpAccount + 4));
        *(int*)((char*)p + 0x18) = 0x82C;
        cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
        break;
    }
    case 16:
    case 20: {
        func_001A97B8(self, a1, kind);
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462C70);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00462C88));
        *(int*)((char*)p + 0x150) = 0x80E;
        func_001A9BC0((cFEStateNet*)self, p);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, *(const char**)self->mpAccount);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, *(const char**)((char*)self->mpAccount + 4));
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    }
    default:
        func_001A97B8(self, a1, kind);
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
extern char D_00462D80[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];
extern char D_00462C70[];
extern char D_00462C88[];

// Tracks a pending flag; on 0x16 shows either the confirm popup or the network-connect popup.
extern "C" void func_001BC990(cFEStateCreateDuplicate* self, void* a1, int a2)
{
    switch (a2) {
    case 0xF: case 0x10: case 0x14:
        self->mbAskSave = 0;
        func_001A97B8_v(self, a1, a2);
        break;
    case 0x11:
        self->mbAskSave = 1;
        func_001A97B8_v(self, a1, a2);
        break;
    case 0x16:
        func_001A97B8_v(self, a1, a2);
        if (self->mbAskSave != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462D80);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
            func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
            *(int*)((char*)p + 0x168) = 0;
            *(int*)((char*)p + 0x164) = 0;
            *(int*)((char*)p + 0x150) = 0x81F;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 1;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462C70);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00462C88));
            *(int*)((char*)p + 0x150) = 0x80E;
            func_001A9BC0((cFEStateNet*)self, p);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, *(char**)self->mpAccount);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, ((char**)self->mpAccount)[1]);
            func_0025C610(LobbyMan_mLobbyMan);
        }
        self->mbAskSave = 0;
        break;
    default:
        func_001A97B8_v(self, a1, a2);
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
extern int D_004A3E90;

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
 * points mszRoomName and mszChannelName at the empty string D_004A3E90, and zeroes miFirstPlayer, both menus, the
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
    self->fB80 = self->f76C = D_004A3E90;
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
extern char D_00464DA0[];

/**
 * Opens the "231lobby_menu" screen and, once it exists, calls the state's virtual slot 28.
 */
extern "C" void cFEStateLobbyMenu_onCreateScreen(cFEStateLobbyMenu* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEVObj_001BCC30*)self), GetHashValue32(D_00464DA0), 0);
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
extern "C" void func_0039A7A8(void* self, int a1);

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
            func_0039A7A8(list, *(unsigned char*)&self->miSelectedRoom);
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
 * Runs one frame of the lobby screen: only the cFEStateNet update (func_001A8770).
 *
 * @return func_001A8770()'s result (the pending-event handler's value)
 */
void* cFEStateLobbyMenu_onUpdate(void* self)
{
    return func_001A8770((cFEStateNet*)self);
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
extern char D_00464DB0[];
extern char D_00464DC0[];
extern char D_00464DD0[];
extern char D_00464DE8[];
extern char D_004A1BF0[];
extern char D_00464E00[];
extern char D_00464E10[];
extern char D_00464E20[];
extern char D_00464E30[];
extern char D_00464E40[];
extern char D_00464E50[];
extern char D_00464E60[];
extern char D_00464E70[];
extern char D_00464E80[];
extern char D_00464E90[];
extern char D_00464EA0[];
extern char D_00464EB0[];
extern char D_00464EC8[];
extern char D_00464EE0[];
extern char D_00464EF8[];
extern char D_00464F10[];
extern char D_00464F20[];
extern char D_00464F30[];
extern char D_00464F40[];
extern char D_00464F50[];

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
    char* o = (char*)obj;
    int id = *(int*)(o + 0x38);
    if (id == GetHashValue32(D_00464DB0)) {
        self->mpMenuRoomMenus = (cUIMenu*)obj;
    } else if (id == GetHashValue32(D_00464DC0)) {
        self->mpMenuPlayers = (cUIMenu*)obj;
        cUIMenu_setInputMap(obj, 1);
        ((cFEVObj_001BCE38*)self->mpMenuPlayers)->v07(1);
        cUIMenu_setScrollableMenu(self->mpMenuPlayers, 1);
        *(int*)((char*)self->mpMenuPlayers + 0x14) &= ~0x80;
        *(int*)((char*)self->mpMenuPlayers + 0x14) |= 1;
    } else if (id == GetHashValue32(D_00464DD0)) {
        cUIText_setUnicodeStringByID((cUIText*)obj, GetHashValue32(D_00464DE8));
    } else if (id == GetHashValue32(D_004A1BF0)) {
        self->mpText3InRoom = (cUIText*)obj;
    } else if (id == GetHashValue32(D_00464E00)) {
        self->mpTextCreateRoom = (cUIText*)obj;
        *(int*)(o + 0x18) = 4;
    } else if (id == GetHashValue32(D_00464E10)) {
        self->mpChannelPair[0] = (cUIThing*)obj;
        *(int*)(o + 0x18) = 0;
    } else if (id == GetHashValue32(D_00464E20)) {
        self->mpChannelPair[1] = (cUIThing*)obj;
        *(int*)(o + 0x18) = 1;
    } else if (id == GetHashValue32(D_00464E30)) {
        self->mpChannelPair[2] = (cUIThing*)obj;
        *(int*)(o + 0x18) = 2;
    } else if (id == GetHashValue32(D_00464E40)) {
        self->mpChannelPair[3] = (cUIThing*)obj;
        *(int*)(o + 0x18) = 3;
    } else if (id == GetHashValue32(D_00464E50)) {
        self->mpChannelArrow[0] = (cUIThing*)obj;
        obj->v09(1);
    } else if (id == GetHashValue32(D_00464E60)) {
        self->mpChannelArrow[1] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(D_00464E70)) {
        self->mpChannelArrow[2] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(D_00464E80)) {
        self->mpChannelArrow[3] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(D_00464E90)) {
        self->mpPlayerUp = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_00464EA0)) {
        self->mpPlayerDown = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_00464EB0)) {
        self->mpChannelList[0] = (cUIListBox*)obj;
        *(int*)(o + 0x14) &= ~1;
    } else if (id == GetHashValue32(D_00464EC8)) {
        self->mpChannelList[1] = (cUIListBox*)obj;
        *(int*)(o + 0x14) &= ~1;
    } else if (id == GetHashValue32(D_00464EE0)) {
        self->mpChannelList[2] = (cUIListBox*)obj;
        *(int*)(o + 0x14) &= ~1;
    } else if (id == GetHashValue32(D_00464EF8)) {
        self->mpChannelList[3] = (cUIListBox*)obj;
        *(int*)(o + 0x14) &= ~1;
    } else if (id == GetHashValue32(D_00464F10)) {
        self->mpChannelLock[0] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(D_00464F20)) {
        self->mpChannelLock[1] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(D_00464F30)) {
        self->mpChannelLock[2] = (cUIThing*)obj;
        obj->v09(0);
    } else if (id == GetHashValue32(D_00464F40)) {
        self->mpChannelLock[3] = (cUIThing*)obj;
        obj->v09(0);
    } else {
        void** p = (void**)(((char*)self) + 0x6D8);
        int i;
        for (i = 0; ; p++, i++) {
            if (i >= 10) {
                return;
            }
            sprintf(buf, D_00464F50, i);
            if (id == GetHashValue32(buf)) {
                break;
            }
        }
        *p = obj;
        *(int*)(o + 0x18) = i;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLobbyMenu_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup_D1B8(void* mem, int a1, void* owner) __asm__("cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup");
extern "C" int func_0039A738_D1B8(void* item) __asm__("func_0039A738");
extern "C" void func_001BD460_D1B8(void* self, int id, int a2) __asm__("func_001BD460");
extern "C" void func_001BD428_D1B8(void* self) __asm__("func_001BD428");
extern "C" int func_0025F180_D1B8(void* self, int id) __asm__("func_0025F180");
extern "C" void func_0025D770_D1B8(void* self, int id) __asm__("func_0025D770");
extern char D_00464F60[];

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
                int id = func_0039A738_D1B8(pWidget);
                self->curId = id;
                int r = func_0025F180_D1B8(LobbyMan_mLobbyMan, id);
                sObj_D1B8* t = self->texts[i];
                t->vt[9].fn((char*)t + t->vt[9].delta, r);
                func_0025D770_D1B8(LobbyMan_mLobbyMan, self->curId);
                return;
            }
        }
        break;
    case 5: {
        char* item = 0;
        if (pWidget == self->backBtn) {
            void* p = cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup_D1B8(cMemMan_alloc(0x6E8, D_00464F60, 0, 0), self->engine, self);
            *(int*)((char*)p + 0x18) = 0x823;
            cUIStateStack_pushPopup(*(char**)(*(char**)(self->screen + 0xD0) + 0x10) + 0x18, p);
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
        if (func_0039A738_D1B8(item) == -1) {
            break;
        }
        func_001BD460_D1B8(self, func_0039A738_D1B8(item), 0);
        break;
    }
    case 6:
        func_001A8A40(self, cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), self->engine));
        break;
    case 7: {
        self->changed = 1;
        int n = *(int*)(*(char**)(self->info + 0xA0) + 0x18);
        self->sel = n;
        if (n < 4) {
            self->selVal = (unsigned char)self->items[n][0x319];
        }
        func_001A8918((cFEStateNet*)self, (int)pWidget, msg);
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
        func_001A8918((cFEStateNet*)self, (int)pWidget, msg);
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
    char* p = (char*)self->mpKeyboard;
    if (*(int*)(p + 0x18) == 8) {
        func_001BD540(self, (int)self->mszRoomName, p + 0x74);
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
extern char D_00464FA8[];

struct sFEObj_001BD460 {
    char pad0[0x770];
    int f770;           // 0x770
    char pad774[0xB80 - 0x774];
    char fB80[4];       // 0xB80 (string object)
};

extern "C" void func_001BD460(cFEStateLobbyMenu* self, int a1, const char* str)
{
    void* p = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FA8);
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x829;
    func_001A9BC0((cFEStateNet*)self, p);
    if (str == 0 || strlen(str) == 0) {
        self->mbRoomHasPassword = 0;
        func_0025D800(LobbyMan_mLobbyMan, a1, 0);
    } else {
        self->mbRoomHasPassword = 1;
        func_0025D800(LobbyMan_mLobbyMan, a1, str);
    }
    char* s = ((char*)&self->mszRoomName);
    cBXString_cBXString4(s, func_0025F150(LobbyMan_mLobbyMan, a1));
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
extern char D_00464FA8[];


extern "C" void func_001BD540(cFEStateLobbyMenu* self, int a1, void* a2)
{
    void* p = func_001A9C50((cFEStateNet*)self);
    *(int*)((char*)p + 0xBC) = 0;
    *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FA8);
    *(int*)((char*)p + 0x164) = 0;
    *(int*)((char*)p + 0xC0) = 3;
    *(int*)((char*)p + 0x150) = 0x829;
    func_001A9BC0((cFEStateNet*)self, p);
    if (a2 == 0 || strlen((const char*)a2) == 0) {
        (self)->mbRoomHasPassword = 0;
        func_0025D860(LobbyMan_mLobbyMan, (const char*)a1, 0);
    } else {
        (self)->mbRoomHasPassword = 1;
        func_0025D860(LobbyMan_mLobbyMan, (const char*)a1, (const char*)a2);
    }
    // PORT: the unit declares a1 as int; it is the name string
    cBXString_cBXString4(((char*)&(self)->mszRoomName), (const char*)a1);
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
extern char D_00464FC0[];
extern char D_00461618[];
extern char D_00464FD8[];

// PORT: the unit declares this (void*, int, int, int); a1/a3 are string pointers passed as int.
extern "C" void func_001BD610(cFEStateLobbyMenu* self, int a1, int a2, int a3)
{
    const char* name = (const char*)a1;
    const char* extra = (const char*)a3;
    if (strlen(name) == 0) {
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FC0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x824;
        func_001A9BC0((cFEStateNet*)self, p);
    } else {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 0;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FD8);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0x150) = 0x828;
        *(int*)((char*)p + 0x164) = 0;
        func_001A9BC0((cFEStateNet*)self, p);
        LobbyMan* g = LobbyMan_mLobbyMan;
        func_0025DAC0(g, a2, name, strlen(extra) != 0 ? extra : 0);
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
extern "C" const char* func_0039A708(void* a);
extern "C" void func_001BE0A8(cFEStateLobbyMenu* self);
// PORT: func_001BE038 is defined later with its own list type; bound by asm label.
void func_001BE038_BD738(void* self) __asm__("func_001BE038");
struct sFEObj_001B1F88;
extern "C" sFEObj_001B1F88* cFEStateChatRoom_cFEStateChatRoom(sFEObj_001B1F88* mem, int a1);
extern "C" void func_001BD848(cFEStateLobbyMenu* self, int a1, int a2);
extern const char D_0045FF10[];

struct sFE1BD738 {
    char pad0[0x708];
    void* boxes[4];     // 0x708
};

// Message handler for the slot-select screen; unknown messages go to the base handler.
extern "C" void* func_001BD738(cFEStateLobbyMenu* self, int a1, int a2)
{
    switch (a1) {
    case 0xE0:
        func_001BE510_BD738(((char*)self), self->miRoomId);
        break;
    case 0xDF: {
        int n = *(int*)(*(char**)((char*)self->mpMenuRoomMenus + 0xA0) + 0x18);
        self->miSelectedChannel = n;
        if (n < 4) {
            cBXString_cBXString4(((char*)self) + 0x76C, func_0039A708(((sFE1BD738*)self)->boxes[n]));
        }
        func_001BE0A8(self);
        func_001BE038_BD738(((char*)self));
        break;
    }
    case 0xDC: case 0xE4:
        func_001A8A40(((char*)self), cFEStateChatRoom_cFEStateChatRoom((sFEObj_001B1F88*)cMemMan_alloc(0x73C, D_0045FF10, 0, 0), (int)self->mpEngine));
        break;
    case 0x102:
        func_001BD848(self, a1, a2);
        break;
    default:
        return func_001A8E40_3(((char*)self), a1, a2);
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

extern "C" void func_001BD848(cFEStateLobbyMenu* self, int a1, int a2)
{
    if (self->mAttrState & 6) {
        switch (*(int*)((char*)self->mpPopup + 0x18)) {
        case 0x828:
            func_001BDA88(self, a2);
            break;
        case 0x829:
            func_001BD8C8(self, a2);
            break;
        default:
            func_001A8E40_3(((cFEStateNet*)self), a1, a2);
            break;
        }
    } else {
        func_001A8E40_3(((cFEStateNet*)self), a1, a2);
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
extern char D_00461618[];
extern char D_004650A0[];
extern char D_004650B8[];
extern char D_004650D0[];
extern char D_00462C88[];

struct sVEntry_001BD8C8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// PORT: the unit declares the message pointer as int
extern "C" void func_001BD8C8(cFEStateLobbyMenu* self, int a2)
{
    void* msg = (void*)a2;
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x66756C6C:
    case 0x75726F6D: {
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_004650A0);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x150) = 0x82B;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    case 0x70617373:
        if (self->mbRoomHasPassword == 0) {
            char* buf = (char*)self + 0x780;
            char* name = self->mszRoomName;
            char* obj = (char*)SSXApp->FELocale;
            sVEntry_001BD8C8* vt = *(sVEntry_001BD8C8**)(obj + 4);
            sprintf1(buf, vt[4].fn(obj + vt[4].delta, GetHashValue32(D_004650B8)), name);
            void* p = func_001A9C50((cFEStateNet*)self);
            *(char**)((char*)p + 0xC4) = buf;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC0) = 1;
            char* sub = (char*)p + 0xBC;
            func_001C57C0(sub, 0, GetHashValue32(D_004650D0));
            func_001C57C0(sub, 1, GetHashValue32(D_00462C88));
            *(int*)((char*)p + 0x150) = 0x82A;
            func_001A9BC0((cFEStateNet*)self, p);
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
extern char D_004650F0[];
extern char D_00465108[];
extern char D_00465128[];
extern char D_00461618[];

// PORT: the unit declares the message pointer as int
extern "C" void func_001BDA88(cFEStateLobbyMenu* self, int a2)
{
    void* msg = (void*)a2;
    switch (*(unsigned int*)((char*)msg + 8)) {
    case 0x66756C6C:
        {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 1;
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_004650F0);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)p + 0x150) = 0x825;
            func_001A9BC0((cFEStateNet*)self, p);
        }
        break;
    case 0x6475706C:
        {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 1;
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00465108);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)p + 0x150) = 0x826;
            func_001A9BC0((cFEStateNet*)self, p);
        }
        break;
    case 0x66696C74:
        {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 1;
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00465128);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)p + 0x150) = 0x827;
            func_001A9BC0((cFEStateNet*)self, p);
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
            return 0x101;
        }
        return 0x101;
    }
    if (obj == (void*)self->mpMenuPlayers) {
        if (msg == 4) {
            self->miFirstPlayer = index;
            func_001BE510_2(self, self->miRoomId);
            return 0x101;
        }
    }
    return 0x101;
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
                        return 0x101;
                    }
                }
            }
        } else {
            return 0x101;
        }
    } while (0);
    return 0x101;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001BDCF8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001BD610(cFEStateLobbyMenu* self, int a1, int a2, int a3);

extern "C" void func_001BDCF8(cFEStateLobbyMenu* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(self, a1, a2);
        func_001BD610(self, *(int*)((char*)a1 + 0x6DC), *(int*)((char*)a1 + 0x6E0), *(int*)((char*)a1 + 0x6E4));
    } else {
        func_001A97B8(self, a1, a2);
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
extern char D_004650D0[];
extern char D_004A1408[];

extern "C" void func_001BDD60(cFEStateLobbyMenu* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8(((cFEStateNet*)self), a1, 0xF);
        // PORT: string pointer passed through an int parameter (func_001A9150's a3)
        func_001A9150(((cFEStateNet*)self), 0, GetHashValue32(D_004650D0), (int)D_004A1408, 0, 8, 0x3F);
        cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
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
    case 0x823:
        func_001BDCF8(self, pPopup, Event);
        break;
    case 0x82A:
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
extern char D_00465148[];
extern char D_00465160[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00465178[];
extern char D_00465198[];
extern char D_00461458[];
extern char D_004614E8[];

extern "C" void func_001BDE48(cFEStateLobbyMenu* self, void* menu)
{
    func_001DA528(menu, 6);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00465148);
    func_001DA530(menu, 0, GetHashValue32(D_00465160));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00465178), 8);
    func_001DA510(menu, 3, GetHashValue32(D_00465198), 0);
    func_001DA510(menu, 4, GetHashValue32(D_00461458), 2);
    func_001DA510(menu, 5, GetHashValue32(D_004614E8), 1);
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

extern "C" void func_0039A7A8(void* self, int a1);

extern "C" void func_001BE038(sFELists1BE038* self)
{
    for (unsigned int i = 0; i < 4; i++) {
        if (self->sel[i] >= self->boxes[i]->count) {
            self->sel[i] = self->boxes[i]->count - 1;
        }
        if (self->boxes[i]->count != 0) {
            func_0039A7A8(self->boxes[i], (unsigned char)self->sel[i]);
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
extern char* D_004A3E90_p __asm__("D_004A3E90");
extern const char* D_004A1A60;
extern const char* D_004A1A64;
extern const char* D_004A1A68;
extern const char* D_004A1A6C;
extern char D_004651B0[];

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
            cUIListBox_addEntryByStringID_u(lists[k], GetHashValue32(D_004651B0), 0xFFFFFFFF);
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
extern "C" int func_0039A738(void* a);
extern "C" int func_0025F180(void* self, int idx);
extern "C" void func_0025D770(void* self, int idx);
// PORT: func_001BE510 is defined as (self) in this unit, but this caller passes a second arg.
void func_001BE510_2(void* self, int a1) __asm__("func_001BE510");
extern "C" void sprintf1(void* str, int a1, char* buf);
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern char D_004651C8[];

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
                sel = func_0039A738(box);
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
        char buf[0x50];
        char* o = (char*)SSXApp->FELocale;
        sVE_001BE3A8* vt = *(sVE_001BE3A8**)(o + 4);
        char* obj = o + vt[4].delta;
        sprintf1(buf, vt[4].fn(obj, GetHashValue32(D_004651C8)), 0);
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
extern char D_004651C8[];

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
        char buf[0x50];
        sprintf1(buf, Loc1BE540(D_004651C8), (char*)((sSelf1BE540*)self)->count);
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
extern int D_004A3E90;
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup(cFEStateNetCreateRoomPopup* self, int a1, int a2)
{
    func_001A8500_3(self, a1, a2);
    int d = D_004A3E90;
    self->mVtbl = cFEStateNetCreateRoomPopup_vtbl;
    self->mszRoomName = (char*)d;
    self->mszChannelName = (char*)d;
    self->mszPassword = (char*)d;
    self->mHashName = 0x47;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE828);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004651E0[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void* func_001BE828(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004651E0), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        screen = cUIScreen_playFrame(screen, 0, 0);
    }
    return screen;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE890__FPv);
#ifdef SKIP_ASM
void* func_001BE890(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BE8B0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern const char* D_004A1A6C;
extern char D_004651F8[];
extern char D_00465208[];
extern char D_00462BA0[];
extern char D_00465218[];
extern char D_00465228[];
extern char D_00462B48[];
extern char D_00465238[];
extern char D_00463AC0[];
extern char D_00463A90[];
extern char D_00463A78[];
extern char D_00463AA8[];
extern char D_004A1408[];

extern "C" void func_001BE8B0(cFEStateNetCreateRoomPopup* self, cUIText* text)
{
    int id = *(int*)((char*)text + 0x38);
    if (id == GetHashValue32(D_004651F8)) {
        cUIListBox_addEntryByStringID(text, GetHashValue32(D_00463AC0), 2);
        cUIListBox_addEntryByStringID(text, GetHashValue32(D_00463A90), 1);
        cUIListBox_addEntryByStringID(text, GetHashValue32(D_00463A78), 0);
        cUIListBox_addEntryByStringID(text, GetHashValue32(D_00463AA8), 3);
        cBXString_cBXString4((char*)self + 0x6E0, D_004A1A6C);
        self->mpLBChannelName = (cUIListBox*)text;
    } else if (id == GetHashValue32(D_00465208)) {
        cUIText_setAsciiString(text, D_004A1408);
        self->mpTextRoomName = text;
    } else if (id == GetHashValue32(D_00462BA0)) {
        cUIText_setAsciiString(text, D_004A1408);
        self->mpTextPassword = text;
    } else if (id == GetHashValue32(D_00465218)) {
        *(int*)((char*)text + 0x18) = 0;
    } else if (id == GetHashValue32(D_00465228)) {
        *(int*)((char*)text + 0x18) = 1;
    } else if (id == GetHashValue32(D_00462B48)) {
        *(int*)((char*)text + 0x18) = 2;
    } else if (id == GetHashValue32(D_00465238)) {
        *(int*)((char*)text + 0x18) = 3;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEA30__FPv);
#ifdef SKIP_ASM
int func_001BEA30(void* self)
{
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEA38);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A9150(cFEStateNet* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern "C" int func_001BEC78(cFEStateNetCreateRoomPopup* self);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" int func_0039A738(void* a);
extern const char* D_004A1A60;
extern const char* D_004A1A64;
extern const char* D_004A1A68;
extern const char* D_004A1A6C;
extern char D_00465248[];
extern char D_00465260[];

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

// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void func_001BEA38(cFEStateNetCreateRoomPopup* self, int a1, unsigned int msg)
{
    char* obj = (char*)a1;
    switch (msg) {
    case 5:
        switch (*(int*)(obj + 0x18)) {
        case 1:
            func_001A9150((cFEStateNet*)self, (int)self->mpTextRoomName, GetHashValue32(D_00465248), (int)self->mszRoomName, 0, 1, 8);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            break;
        case 2:
            func_001A9150((cFEStateNet*)self, (int)self->mpTextPassword, GetHashValue32(D_00465260), (int)self->mszPassword, 0, 2, 8);
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x35, 0, 2);
            break;
        case 3:
            if (func_001BEC78(self)) {
                ((cFEVObj_001BEA38*)self->mpParentState)->v17(self, 0xF);
            }
            break;
        }
        break;
    case 6:
        ((cFEVObj_001BEA38*)self->mpParentState)->v17(self, 0x14);
        break;
    case 9:
        if (obj == (char*)self->mpLBChannelName) {
            switch (func_0039A738(obj)) {
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
        char* p = (char*)self->mpKeyboard;
        switch (*(int*)(p + 0x18)) {
        case 1:
            cBXString_cBXString4((char*)self + 0x6DC, p + 0x74);
            cUIText_setAsciiString(self->mpTextRoomName, (const char*)self->mszRoomName);
            break;
        case 2:
            cBXString_cBXString4((char*)self + 0x6E4, p + 0x74);
            cUIText_setAsciiString(self->mpTextPassword, (const char*)self->mszPassword);
            break;
        }
    }
    default:
        func_001A8918((cFEStateNet*)self, a1, msg);
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
extern char D_00464FC0[];
extern char D_00461618[];

extern "C" int func_001BEC78(cFEStateNetCreateRoomPopup* self)
{
    if (strlen((const char*)self->mszRoomName) == 0) {
        int three = 3;
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 1;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00464FC0);
        *(int*)((char*)p + 0xC0) = three;
        func_001C57C0(sub, 0, GetHashValue32(D_00461618));
        *(int*)((char*)p + 0x14C) = three;
        *(int*)((char*)p + 0x150) = 0x847;
        func_001A9BC0((cFEStateNet*)self, p);
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
extern char D_00465278[];
extern char D_00465290[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001BED58(cFEStateNetCreateRoomPopup* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465278);
    func_001DA530(popup, 0, GetHashValue32(D_00465290));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(popup, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 3, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateLogin_cFEStateLogin);
#ifdef SKIP_ASM
extern void* cFEStateLogin_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateLogin_cFEStateLogin(cFEStateLogin* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->mHashName = 0x33;
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
INCLUDE_ASM("fe/feasyncfile", func_001BEED0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void func_001A8768(void* self);
extern char D_004652B0[];

extern "C" void func_001BEED0(cFEStateLogin* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_004652B0), 0);
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
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BEF60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void cUIText_setUnicodeString(cUIText* text, char* str);
extern "C" int strlen(const char* s);
// PORT: func_002C2718 is a varargs formatter (buf, fmt, ...); the unit declares a 3-arg view
extern "C" void func_002C2718_1BEF60(unsigned short* buf, const char* fmt, ...) __asm__("func_002C2718");
extern char D_004A1BF8[];
extern char D_004652C0[];
extern char D_004652D0[];
extern char D_004652E0[];
extern char D_004652F0[];
extern char D_00465300[];
extern char D_00465318[];
extern char D_00465330[];
extern char D_00462F08[];
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

extern "C" void func_001BEF60(cFEStateLogin* self, cFEVObj_001BEF60* t)
{
    unsigned short buf[64];
    sItem_001BEF60* o = (sItem_001BEF60*)t;
    if (IsHash_001BEF60(o->mId, D_004652C0)) {
        o->mState = 0;
        int s = Loc_001BEF60(D_004652D0);
        func_002C2718_1BEF60(buf, D_004A1BF8, s, D_00536760);
        cUIText_setUnicodeString((cUIText*)t, (char*)buf);
        t->v09(1);
        if (strlen(D_00536760) == 0) {
            t->v25(1);
            t->v08(1);
            t->v09(0);
        }
    } else if (IsHash_001BEF60(o->mId, D_004652E0)) {
        if (strlen(D_00536760) == 0) {
            t->v09(0);
        } else {
            t->v09(1);
        }
    } else if (IsHash_001BEF60(o->mId, D_004652F0)) {
        if (strlen(D_00536760) == 0) {
            t->v09(0);
        } else {
            t->v09(1);
        }
    } else if (IsHash_001BEF60(o->mId, D_00465300)) {
        o->mState = 2;
        t->v09(1);
    } else if (IsHash_001BEF60(o->mId, D_00465318)) {
        o->mState = 1;
        t->v09(1);
    } else if (IsHash_001BEF60(o->mId, D_00465330)) {
        o->mState = 3;
        int s = Loc_001BEF60(D_00462F08);
        func_002C2718_1BEF60(buf, D_004A1BF8, s, D_00536760);
        cUIText_setUnicodeString((cUIText*)t, (char*)buf);
        t->v09(1);
        if (strlen(D_00536760) == 0) {
            t->v25(1);
            t->v08(1);
            t->v09(0);
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
extern const char D_00462D68[];
extern char D_00462C70[];
extern char D_00462C88[];
extern char D_00536760[];

extern "C" int func_001BF228(cFEStateLogin* self, int id, int a2)
{
    switch (id) {
    case 0xC8: {
        void* p = func_001A9C50(((cFEStateNet*)self));
        *(int*)((char*)p + 0xBC) = 1;
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00462C70);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_00462C88));
        *(int*)((char*)p + 0x150) = 0x80E;
        func_001A9BC0(((cFEStateNet*)self), p);
        break;
    }
    case 0xC9: {
        char* names = D_00536760;
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, names);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, names + 0x11);
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    }
    case 0xD4:
        func_001A8A40(((cFEStateNet*)self), cFEStateSelectPersona_cFEStateSelectPersona((cFEStateSelectPersona*)cMemMan_alloc(0x6D8, D_00462D68, 0, 0), (int)self->mpEngine));
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

extern "C" void func_001BF378(cFEStateLogin* self, void* a1, int a2)
{
    if (a2 == 0xF || a2 == 0x14) {
        func_00261408(GetSession_001BF378());
        func_0025B800(LobbyMan_mLobbyMan);
        func_001A97B8(self, a1, a2);
    } else {
        func_001A97B8(self, a1, a2);
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
extern char D_004652C0[];
extern char D_00465330[];
extern char D_004652E0[];
extern char D_004652F0[];
extern char D_0045DB90[];

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
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004652C0));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        {
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465330));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        {
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004652E0));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        {
            void* o = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004652F0));
            if (o) {
                ((cFEVObj_001BF438*)self)->v18(o);
            }
        }
        void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_0045DB90));
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
extern const char D_00460BF0[];

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
        if (func_001A8A40_r(((cFEStateNet*)self), cFEStateNetConfig_cFEStateNetConfig((cFEStateNetConfig*)cMemMan_alloc(0x91C, D_00460BF0, 0, 0), (int)self->mpEngine)) != 0) {
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
INCLUDE_ASM("fe/feasyncfile", func_001BF6C0);
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

extern "C" void func_001BF6C0(cFEStateLogin* self, void* a1, int a2)
{
    switch (*(unsigned int*)((char*)a1 + 0x18)) {
    case 0x80E:
        ((cFEVObj_001BF6C0*)self)->v33(a1, a2);
        break;
    case 0x82C:
        func_001BF438(self, a1, a2);
        break;
    case 0x848:
        func_001BF5A8(self, a1, a2);
        break;
    default:
        func_001A97B8(((cFEVObj_001BF6C0*)self), a1, a2);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BF758);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int USTR_length(void* s);
extern "C" int strlen(const char* s);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void cMemMan_free(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d)
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
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
extern char D_00465348[];
extern char D_00462ED0[];
extern char D_00462D10[];
extern char D_00464860[];
extern char D_00465360[];
extern char D_00465380[];
extern char D_00465398[];
extern const char D_00461CD8[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

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

extern "C" void func_001BF758(cFEStateLogin* self, void* a1, unsigned int kind)
{
    if (a1 == 0) {
        return;
    }
    switch (kind) {
    case 5:
        switch (*(int*)((char*)a1 + 0x18)) {
        case 0:
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, (const char*)SSXApp->UserName);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, (const char*)SSXApp->Password);
            func_0025B6D8(LobbyMan_mLobbyMan, 0);
            break;
        case 2:
            func_001A8A40(self, cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(0x6F4, D_00462ED0, 0, 0), (int)self->mpEngine, 1));
            break;
        case 1:
            func_001A8A40(self, cFEUseExistingAccount_cFEUseExistingAccount((cFEUseExistingAccount*)cMemMan_alloc(0x6E4, D_00465348, 0, 0), (int)self->mpEngine));
            break;
        case 3: {
            void* o = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, self, 3, 0, 0);
            *(int*)((char*)o + 0x18) = 0x82C;
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, o);
            break;
        }
        }
        break;
    case 6: {
        if (self->mpustrDisconnect != 0) {
            cMemMan_free(self->mpustrDisconnect);
            self->mpustrDisconnect = 0;
        }
        int a = Loc1BF758(D_00465360);
        char* title = D_00465380;
        int b = Loc1BF758(title);
        char* sep = D_00465398;
        int len = USTR_length((void*)a) + USTR_length((void*)b) + strlen(sep) + 1;
        self->mpustrDisconnect = (unsigned short*)operator_new_tag(len * 2, D_00461CD8, 0x100, 0);
        void* p = func_001A9C50((cFEStateNet*)self);
        char* sub = (char*)p + 0xBC;
        if (self->mpustrDisconnect != 0) {
            sprintf2(self->mpustrDisconnect, sep, a, b);
            *(void**)((char*)p + 0xC4) = (void*)self->mpustrDisconnect;
            *(int*)((char*)p + 0xC0) = 1;
        } else {
            *(int*)((char*)p + 0xC4) = GetHashValue32(title);
            *(int*)((char*)p + 0xC0) = 3;
        }
        *(int*)sub = 2;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)(sub + 0xAC) = 1;
        *(int*)(sub + 0x94) = 0x848;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    case 8: {
        void* o = cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(0x6DC, D_00464860, 0x100, 0), (int)self->mpEngine);
        *(int*)((char*)o + 0x18) = 0x12;
        func_001A8A40(self, o);
        break;
    }
    default:
        func_001A8918((cFEStateNet*)self, (int)a1, kind);
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
extern char D_004653A8[];
extern char D_004653C8[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_004614E8[];
extern char D_00464910[];

extern "C" void func_001BFAE0(cFEStateLogin* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_004653A8);
    func_001DA530(popup, 0, GetHashValue32(D_004653C8));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
    func_001DA510(popup, 3, GetHashValue32(D_00464910), 2);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateOnlineMainMenu_cFEStateOnlineMainMenu);
#ifdef SKIP_ASM
extern void* cFEStateOnlineMainMenu_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cFEStateNet* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->mHashName = 0x3B;
    self->mVtbl = cFEStateOnlineMainMenu_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFBF8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A9930(cFEStateNet* self);
extern "C" void func_00256C50();
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* p, int a1);
extern char D_004653F0[];

extern "C" void func_001BFBF8(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004653F0), 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001BFCA0__FPv);
#ifdef SKIP_ASM
void* func_001BFCA0(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFCC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00465408[];
extern char D_00465420[];
extern char D_00465430[];
extern char D_00465440[];
extern char D_00465458[];
extern char D_00465470[];

static inline int IsHash1BFCC0(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001BFCC0(cFEStateNet* self, void* obj)
{
    if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465408)) {
        *(int*)((char*)obj + 0x18) = 1;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465420)) {
        *(int*)((char*)obj + 0x18) = 2;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465430)) {
        *(int*)((char*)obj + 0x18) = 3;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465440)) {
        *(int*)((char*)obj + 0x18) = 0;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465458)) {
        *(int*)((char*)obj + 0x18) = 4;
    } else if (IsHash1BFCC0(*(int*)((char*)obj + 0x38), D_00465470)) {
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
extern char D_00465480[];
extern char D_004654A0[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_00461458[];
extern char D_004654C0[];

extern "C" void func_001BFD98(cFEStateNet* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465480);
    func_001DA530(popup, 0, GetHashValue32(D_004654A0));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_00461458), 2);
    func_001DA510(popup, 3, GetHashValue32(D_004654C0), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001BFE70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup_FE70(void* mem, int a1, void* owner) __asm__("cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup");
extern "C" void* cFEStateLobbyMenu_cFEStateLobbyMenu_FE70(void* mem, int a1) __asm__("cFEStateLobbyMenu_cFEStateLobbyMenu");
extern "C" void* cFEStateStats_cFEStateStats_FE70(void* mem, int a1) __asm__("cFEStateStats_cFEStateStats");
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld_FE70(void* mem, int a1) __asm__("cFEStateNetSSXWorld_cFEStateNetSSXWorld");
extern char D_004654E0[];
extern char D_00465500[];
extern const char D_004611A8[];
extern char D_00465510[];
extern char D_00465530[];

class cFEFactory_001BFE70 {
public:
    int f0;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* create(void* owner, int a2);
};

// Memory-card async state message handler: creates the next screen or shows an error popup.
extern "C" void func_001BFE70(cFEStateNet* self, void* a1, unsigned int a2)
{
    if (a1 == 0) {
        return;
    }
    switch (a2) {
    case 5:
        switch (*(unsigned int*)((char*)a1 + 0x18)) {
        case 1: {
            D_004A4E88 = 1;
            void* p = cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup_FE70(cMemMan_alloc(0x734, D_004654E0, 0, 0), (int)self->mpEngine, self);
            *(int*)((char*)p + 0x18) = 0x82E;
            cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
            break;
        }
        case 2:
            func_001A8A40(self, cFEStateLobbyMenu_cFEStateLobbyMenu_FE70(cMemMan_alloc(0xB84, D_00461B30, 0, 0), (int)self->mpEngine));
            break;
        case 3:
            func_001A8A40(self, cFEStateStats_cFEStateStats_FE70(cMemMan_alloc(0x6F0, D_00465500, 0, 0), (int)self->mpEngine));
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
            func_001A8A40(self, cFEStateNetSSXWorld_cFEStateNetSSXWorld_FE70(cMemMan_alloc(0x6D0, D_004611A8, 0, 0), (int)self->mpEngine));
            break;
        case 5: {
            void* p = func_001A9C50(self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 2;
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00465510);
            *(int*)((char*)p + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            func_001C57C0(sub, 1, GetHashValue32(D_00462C88));
            *(int*)((char*)p + 0x150) = 0x82D;
            func_001A9BC0(self, p);
            break;
        }
        }
        break;
    case 6: {
        void* p = func_001A9C50(self);
        char* sub = (char*)p + 0xBC;
        *(int*)((char*)p + 0xBC) = 2;
        *(int*)((char*)p + 0xC4) = GetHashValue32(D_00465530);
        *(int*)((char*)p + 0xC0) = 3;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)((char*)p + 0x150) = 0x846;
        func_001A9BC0(self, p);
        break;
    }
    case 8:
        if ((self->mAttrState & 6) == 0) {
            func_001A9B58(self);
        }
        break;
    default:
        func_001A8918(self, (int)a1, a2);
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
extern const char D_00460BA8[];

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
        if (func_001A8A40_r(self, func_00194A60(cMemMan_alloc(0x58, D_00460BA8, 0, 0), (int)self->mpEngine)) != 0) {
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

extern "C" void func_001C0218(cFEStateNet* self, void* a1, int kind)
{
    if (kind != 15) {
        func_001A97B8(self, a1, kind);
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
extern char D_00462D10[];

extern "C" void func_001C0240(cFEStateNet* self, void* a1, int a2)
{
    if (a2 == 0xF) {
        func_001A97B8_v(self, a1, 0xF);
        func_00261408(BuddyMan_mBuddyMan);
        func_00261070(BuddyMan_mBuddyMan);
        func_00261058(BuddyMan_mBuddyMan);
        func_0025B800(LobbyMan_mLobbyMan);
        void* p = cFEStateNetAccountMem_cFEStateNetAccountMem((cFEStateNetAccountMem*)cMemMan_alloc(0xB10, D_00462D10, 0, 0), (int)self->mpEngine, self, 0, 0, 0);
        *(int*)((char*)p + 0x18) = 0x832;
        cUIStateStack_pushPopup(*(char**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, p);
    } else {
        func_001A97B8_v(self, a1, a2);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C02F8);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C0138(cFEStateNet* self, void* a1, int a2);
extern "C" void func_001C0240(cFEStateNet* self, void* a1, int a2);

extern "C" void func_001C02F8(cFEStateNet* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x82D:
        func_001C0138((cFEStateNet*)self, a1, a2);
        break;
    case 0x846:
        func_001C0240((cFEStateNet*)self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
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
extern const char D_00461C60[];

extern "C" void* func_001C0358(cFEStateNet* self, int a1, int a2)
{
    switch (a1) {
    default:
        return func_001A8E40_3(self, a1, a2);
    case 0xF3:
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), (int)self->mpEngine));
        break;
    case 0xEF:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C03D0);
#ifdef SKIP_ASM
extern void* cFEStateOnlineAgreement_vtbl[];
extern "C" void* func_001A8500(void* self);

extern "C" void* func_001C03D0(cFEStateOnlineAgreement* self)
{
    func_001A8500(self);
    self->mHashName = 0x43;
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

extern "C" void* cFEStateOnlineAgreement_cFEStateOnlineAgreement(cFEStateOnlineAgreement* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->mHashName = 0x43;
    self->mVtbl = cFEStateOnlineAgreement_vtbl;
    self->mpTOSLines = 0;
    self->miNextTOSLine = (unsigned int)0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0460);
#ifdef SKIP_ASM
extern void* cFEStateOnlineAgreement_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001C0460(cFEStateOnlineAgreement* self)
{
    func_001A8500_3(self, 0, 0);
    self->mpTOSLines = 0;
    self->mVtbl = cFEStateOnlineAgreement_vtbl;
    self->miNextTOSLine = (unsigned int)0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C04A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void func_001C0540(cFEStateOnlineAgreement* self);
extern char D_00465568[];
extern char D_00465580[];

extern "C" void func_001C04A8(cFEStateOnlineAgreement* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_00465568), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        func_001C0540(self);
    }
    cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465580));
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
extern char D_00464860[];
extern char D_00465590[];
extern char D_004655A0[];
extern char D_004655B0[];
extern char D_004655C0[];
extern char D_004655D0[];
extern char D_004655E0[];
extern char D_004655F0[];
extern char D_00465600[];
extern char D_00465610[];
extern char D_00465620[];
extern char D_00465630[];
extern char D_00465640[];
extern char D_00465650[];
extern char D_00465660[];
extern char D_00465670[];
extern char D_00465680[];
extern char D_00465690[];
extern char D_004656A0[];
extern char D_004656B0[];
extern char D_004656C0[];
extern char D_004656D0[];
extern char D_004656E0[];
extern char D_004656F0[];
extern char D_00465700[];

extern "C" void func_001C0540(cFEStateOnlineAgreement* self)
{
    self->miNumTOSLines = 0xB2;
    if (self->mpTOSLines != 0) {
        cMemMan_free(self->mpTOSLines);
        self->mpTOSLines = 0;
    }
    if (self->miNumTOSLines != 0) {
        char buf[32];
        int i;
        int n;
        self->mpTOSLines = (int*)operator_new_tag(self->miNumTOSLines * 4, D_00464860, 0x100, 0);
        n = 0;
        for (i = 0x0; i < 0x74; i++) {
            sprintf(buf, D_00465590, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(D_004655A0);
        for (i = 0x74; i < 0x76; i++) {
            sprintf(buf, D_00465590, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(D_004655B0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004655C0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004655D0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004655E0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004655F0);
        for (i = 0x7A; i < 0x7D; i++) {
            sprintf(buf, D_00465590, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465600);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465610);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465620);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465630);
        for (i = 0x7E; i < 0x81; i++) {
            sprintf(buf, D_00465590, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465640);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465650);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465660);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465670);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465680);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465690);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004656A0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004656B0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004656C0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004656D0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_004656E0);
        for (i = 0x83; i < 0x9A; i++) {
            sprintf(buf, D_00465590, i);
            (self->mpTOSLines)[n++] = GetHashValue32(buf);
        }
        (self->mpTOSLines)[n++] = GetHashValue32(D_004656F0);
        (self->mpTOSLines)[n++] = GetHashValue32(D_00465700);
        for (i = 0x9E; i < 0xA6; i++) {
            sprintf(buf, D_00465590, i);
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
INCLUDE_ASM("fe/feasyncfile", func_001C0A80);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* func_001A8770(cFEStateNet* self);
extern "C" void func_003A1F18(cUIText* text, int a1);
extern char D_00465710[];

extern "C" void func_001C0A80(cFEStateOnlineAgreement* self)
{
    if (self->miNextTOSLine < self->miNumTOSLines && self->mpTOSLines != 0) {
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465710));
        if (text != 0) {
            func_003A1F18(text, (self->mpTOSLines)[(self->miNextTOSLine)++]);
        }
    }
    func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0B08);
#ifdef SKIP_ASM
extern "C" int func_001C0B08(cFEStateOnlineAgreement* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0B20);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" void func_003A1310(void* self, int a1);
extern char D_004619B8[];
extern char D_004619C8[];
extern char D_004619D8[];
extern char D_004619E8[];
extern char D_00465710[];
extern char D_00465720[];
extern char D_00465730[];

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

extern "C" int func_001C0B20(cFEStateOnlineAgreement* self)
{
    ((cFEVObj_001C0B20*)self)->v32();
    char* o = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619B8));
    cUIText* a = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619C8));
    cUIText* b = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619D8));
    cUIText* c = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619E8));
    *(cUIText**)(o + 0x7C) = a;
    *(cUIText**)(o + 0x80) = b;
    if ((unsigned int)strlen(D_00465720) > 24) {
        o[0x8C] = 0;
    } else {
        strcpy(o + 0x8C, D_00465720);
    }
    if ((unsigned int)strlen(D_00465730) > 24) {
        o[0xA5] = 0;
    } else {
        strcpy(o + 0xA5, D_00465730);
    }
    if ((unsigned int)strlen(D_00465730) > 24) {
        o[0xA5] = 0;
    } else {
        strcpy(o + 0xA5, D_00465730);
    }
    *(cUIText**)(o + 0x84) = c;
    char* x = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465710));
    *(char**)(x + 0xB8) = o;
    func_003A1310(x, 1);
    ((cFEVObj_001C0B20*)x)->v07(1);
    if (o) {
        ((cFEVObj_001C0B20*)o)->v09(1);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0CE0);
#ifdef SKIP_ASM
extern char D_004619B8[];
int GetHashValue32(char* str);

extern "C" void func_001C0CE0(cFEStateOnlineAgreement* self, void* item)
{
    int hash = *(int*)((char*)item + 0x38);
    if (hash == GetHashValue32(D_004619B8)) {
        *(int*)((char*)item + 0x14) |= 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C0D28);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void* cFEStateLogin_cFEStateLogin(cFEStateLogin* mem, int a1);
extern "C" void* cFEStateCreateStep1_cFEStateCreateStep1(cFEStateCreateStep1* self, int a1, int a2);
extern "C" void* cFEStateCreateStep2_cFEStateCreateStep2(cFEStateCreateStep2* self, int a1);
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int a1);
extern char D_00465740[];
extern char D_00462ED0[];
extern char D_00464848[];
extern const char D_004611A8[];

// On confirm (msg 5/6), opens the screen that matches this menu's id; other messages go to the base handler.
extern "C" void func_001C0D28(cFEStateOnlineAgreement* self, int a1, unsigned int msg)
{
    void* obj;
    if (a1 == 0) {
        return;
    }
    switch (msg) {
    case 5:
    case 6:
        switch (self->miMiscData) {
        case 0x12:
            obj = cFEStateLogin_cFEStateLogin((cFEStateLogin*)cMemMan_alloc(0x6D4, D_00465740, 0, 0), (int)self->mpEngine);
            break;
        case 0x14:
            obj = cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(0x6F4, D_00462ED0, 0, 0), (int)self->mpEngine, 1);
            break;
        case 0x15:
            obj = cFEStateCreateStep1_cFEStateCreateStep1((cFEStateCreateStep1*)cMemMan_alloc(0x6F4, D_00462ED0, 0, 0), (int)self->mpEngine, 0);
            break;
        case 0x16:
            obj = cFEStateCreateStep2_cFEStateCreateStep2((cFEStateCreateStep2*)((cFEStateNet*)cMemMan_alloc(0x6E0, D_00464848, 0, 0)), (int)self->mpEngine);
            break;
        case 0x13:
        default:
            obj = cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(0x6D0, D_004611A8, 0, 0), (int)self->mpEngine);
            break;
        }
        func_001A8A40(((cFEStateNet*)self), obj);
        break;
    default:
        func_001A8918(((cFEStateNet*)self), a1, msg);
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
extern char D_00465778[];
extern char D_00465798[];
extern char D_004657C0[];
extern char D_004614E8[];

extern "C" void func_001C0EA8(cFEStateOnlineAgreement* self, void* popup)
{
    func_001DA528(popup, 2);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465778);
    func_001DA530(popup, 0, GetHashValue32(D_00465798));
    func_001DA510(popup, 0, GetHashValue32(D_004657C0), 8);
    func_001DA510(popup, 1, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup);
#ifdef SKIP_ASM
extern void* cFEStateNetQuickPlayPopup_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup(cFEStateNetQuickPlayPopup* self, int a1, int a2)
{
    func_001A8500_3(self, a1, a2);
    self->mVtbl = cFEStateNetQuickPlayPopup_vtbl;
    self->mHashName = 0x48;
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
INCLUDE_ASM("fe/feasyncfile", func_001C0FC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void MEM_fill(void* dst, int c, int n);
extern char D_004657E0[];
extern char D_005308B8[];

extern "C" void func_001C0FC0(cFEStateNetQuickPlayPopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_004657E0), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        MEM_fill(D_005308B8, 0, 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1038);
#ifdef SKIP_ASM
extern "C" void* func_001A8770(cFEStateNet* self);
extern "C" void* func_001C1038(cFEStateNetQuickPlayPopup* self)
{
    if (self->mbSearching != 0) {
        int t = (self->miBlinkFrames)--;
        if (t == 0) {
            self->miBlinkFrames = 30;
        }
    }
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1080);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A1A80[];
extern char D_004657F8[];
extern char D_004A1C00[];
extern char D_00465808[];
extern char D_00465818[];
extern char D_00465828[];
extern char D_00465838[];
extern char D_00465848[];
extern char D_00465858[];
extern char D_00465868[];
extern char D_00465878[];
extern char D_00465890[];
extern char D_004658A0[];
extern char D_004658B0[];
extern char D_004658C0[];
extern char D_004658D0[];
extern char D_004658E0[];

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

extern "C" void func_001C1080(cFEStateNetQuickPlayPopup* self, cFEVObj_001C1080* obj)
{
    int id = *(int*)((char*)obj + 0x38);
    obj->v09(1);
    if (id == GetHashValue32(D_004A1A80)) {
        self->mpTextOK = (cUIText*)obj;
        obj->v25(1);
    } else if (id == GetHashValue32(D_004657F8)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBRanked = (cUIListBox*)obj;
    } else if (id == GetHashValue32(D_004A1C00)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBRace = (cUIListBox*)obj;
    } else if (id == GetHashValue32(D_00465808)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBBigAir = (cUIListBox*)obj;
    } else if (id == GetHashValue32(D_00465818)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBSuperpipe = (cUIListBox*)obj;
    } else if (id == GetHashValue32(D_00465828)) {
        func_001A9AB8((cFEStateNet*)self, (cUIListBox_001A9AB8*)obj, 1);
        obj->v24(1);
        self->mpLBSlopeStyle = (cUIListBox*)obj;
    } else if (id == GetHashValue32(D_00465838)) {
        obj->v25(1);
        self->mpStaticRanked = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_00465848)) {
        obj->v25(1);
        self->mpStaticRace = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_00465858)) {
        obj->v25(1);
        self->mpStaticBigAir = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_00465868)) {
        obj->v25(1);
        self->mpStaticSuperpipe = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_00465878)) {
        obj->v25(1);
        self->mpStaticSlopeStyle = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_00465890)) {
        self->mpRankedPair = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_004658A0)) {
        self->mpRacePair = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_004658B0)) {
        self->mpBigAirPair = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_004658C0)) {
        self->mpSuperpipePair = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_004658D0)) {
        self->mpSlopestylePair = (cUIThing*)obj;
    } else if (id == GetHashValue32(D_004658E0)) {
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
extern char D_00463CA0[];
extern char D_004658F0[];

extern "C" void func_001C13E0(cFEStateNetQuickPlayPopup* self, int show)
{
    if (self->mpScreen != 0) {
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00463CA0));
        if (text != 0) {
            ((cFEVText_001C13E0*)text)->v09(show);
        }
        text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004658F0));
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
extern char D_00465900[];
extern char D_00465918[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00462C50[];
extern char D_004614E8[];

extern "C" void func_001C1488(cFEStateNetQuickPlayPopup* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465900);
    func_001DA530(popup, 0, GetHashValue32(D_00465918));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(popup, 2, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 3, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1560);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001C13E0(cFEStateNetQuickPlayPopup* self, int show);
extern "C" void func_001C17F8(cFEStateNetQuickPlayPopup* self, int a1);
extern "C" void func_0025EE20(void* self, void* ids, int a2);
extern "C" void func_0025EED8(void* self);
extern "C" int func_0039A738(void* a);
// D_00534B34 is a splat label inside cBENetworkInterface_networkOpt
extern int D_00534B34[];
extern char D_00465938[];
extern char D_00462C88[];

struct sSelf_001C1560 {
    char pad[0x6D0];
    int ids[5];
};

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

// PORT: obj is a pointer passed as int (the unit's handler signature)
extern "C" void func_001C1560(cFEStateNetQuickPlayPopup* self, int a1, unsigned int msg)
{
    switch (msg) {
    case 7:
        if (self->mbSearching == 0) {
            func_001A8918((cFEStateNet*)self, a1, 7);
        }
        break;
    case 5:
        if ((int)self->mpTextOK == a1) {
            if (self->mbSearching != 0) {
                cUIText_setUnicodeStringByID(self->mpTextSearching, GetHashValue32(D_00465938));
                ((cFEVObj_001C1560*)self->mpTextOK)->v08(1);
                func_001C13E0(self, self->mbSearching ^ 1);
                func_0025EED8(LobbyMan_mLobbyMan);
            } else {
                self->unk_6F0 = 0;
                self->mbSearching = 1;
                sSelf_001C1560* o = (sSelf_001C1560*)self;
                int* ids = o->ids;
                func_001C13E0(self, 0);
                char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
                D_00534B34[0] = func_0039A738(self->mpLBRanked) != 0;
                sVE_001C1560* vt = *(sVE_001C1560**)(iface + 0xC);
                vt[1].fn(iface + vt[1].delta);
                cBENewRaceInterface_setNumberAI(cBE_getInterface_Fv(SSXApp->BE, 0), 0);
                o->ids[0] = func_0039A738(self->mpLBRanked);
                o->ids[1] = func_0039A738(self->mpLBRace);
                o->ids[2] = func_0039A738(self->mpLBBigAir);
                o->ids[3] = func_0039A738(self->mpLBSuperpipe);
                o->ids[4] = func_0039A738(self->mpLBSlopeStyle);
                func_0025EE20(LobbyMan_mLobbyMan, ids, self->mCurrentCtrl);
                ((cFEVObj_001C1560*)self->mpTextSearching)->v09(1);
                cUIText_setUnicodeStringByID(self->mpTextOK, GetHashValue32(D_00462C88));
                self->miBlinkFrames = 0x1E;
                func_001C17F8(self, 1);
            }
        }
        break;
    case 6:
        func_001C13E0(self, self->mbSearching ^ 1);
        if (self->mbSearching != 0) {
            cUIText_setUnicodeStringByID(self->mpTextSearching, GetHashValue32(D_00465938));
            ((cFEVObj_001C1560*)self->mpTextOK)->v08(1);
            func_0025EED8(LobbyMan_mLobbyMan);
        } else {
            ((cFEVObj_001C1560*)self->mpParentState)->v17(self, 0x14);
        }
        break;
    default:
        func_001A8918((cFEStateNet*)self, a1, msg);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C17D8__FPv);
#ifdef SKIP_ASM
int func_001C17D8(void* self)
{
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C17E0);
#ifdef SKIP_ASM
extern "C" int func_001C17E0(cFEStateNetQuickPlayPopup* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
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
extern const char D_00461C60[];
extern char D_00461618[];
extern char D_00465958[];

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
    case 0xE6:
        self->unk_6F0 = 1;
        break;
    case 0x10C: case 0x112:
        func_0039F698((char*)self->mpEngine + 0x18);
        func_001AB178((cFEStateNet*)self, a1);
        break;
    case 0xE7:
        if (self->unk_6F0 != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            char* sub = (char*)p + 0xBC;
            *(int*)((char*)p + 0xBC) = 1;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)p + 0xC4) = GetHashValue32(D_00465958);
            *(int*)((char*)p + 0xC0) = 3;
            *(int*)((char*)p + 0x150) = 0x82F;
            *(int*)((char*)p + 0x14C) = 6;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            self->mbSearching = 0;
            ((cFEObj_001C19C8*)self->mpParentState)->v17(self, 0x14);
        }
        break;
    case 0xF3:
        func_001A8A40(self, cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect((cFEStateOnlineCharacterSelect*)cMemMan_alloc(0x6E4, D_00461C60, 0x100, 0), (int)self->mpEngine));
        break;
    default:
        return func_001A8E40_3(self, a1, a2);
    case 0xEF:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1B48);
#ifdef SKIP_ASM
extern "C" int func_001C1B88(cFEStateNetQuickPlayPopup* self, void* a1, int a2);
extern "C" int func_001A97B8(void* self, void* a1, int a2);

extern "C" int func_001C1B48(cFEStateNetQuickPlayPopup* self, void* a1, int a2)
{
    int r;
    if (*(int*)((char*)a1 + 0x18) == 0x82F) {
        r = func_001C1B88(self, a1, a2);
    } else {
        r = func_001A97B8(self, a1, a2);
    }
    return r;
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
extern char D_00461618[];

extern "C" int func_001C1B88(cFEStateNetQuickPlayPopup* self, void* a1, int a2)
{
    int r;
    if (a2 == 0xF || a2 == 0x14) {
        self->mbSearching = 0;
        ((cFEVText_001C1B88*)self->mpTextSearching)->v09(0);
        func_001C13E0(self, self->mbSearching ^ 1);
        cUIText_setUnicodeStringByID(self->mpTextOK, GetHashValue32(D_00461618));
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

extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->mHashName = 0x4A;
    self->mVtbl = cFEStateNetSSXWorld_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1C90);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001A9930(cFEStateNet* self);
extern char D_00465A38[];

extern "C" void func_001C1C90(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00465A38), 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001C1D10__FPv);
#ifdef SKIP_ASM
void* func_001C1D10(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1D30);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465A48[];
extern char D_00465A60[];
extern char D_0045FE60[];
extern char D_00462C50[];
extern char D_00461458[];
extern char D_004614E8[];

extern "C" void func_001C1D30(cFEStateNet* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465A48);
    func_001DA530(popup, 0, GetHashValue32(D_00465A60));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00462C50), 0);
    func_001DA510(popup, 2, GetHashValue32(D_00461458), 2);
    func_001DA510(popup, 3, GetHashValue32(D_004614E8), 1);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1E08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00465A80[];
extern char D_00465A90[];
extern char D_00465AA8[];
extern char D_00465AC0[];

static inline int IsHash1C1E08(int id, char* name) { return id == GetHashValue32(name); }

extern "C" void func_001C1E08(cFEStateNet* self, void* obj)
{
    if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465A80)) {
        *(int*)((char*)obj + 0x18) = 0;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465A90)) {
        *(int*)((char*)obj + 0x18) = 1;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465AA8)) {
        *(int*)((char*)obj + 0x18) = 2;
    } else if (IsHash1C1E08(*(int*)((char*)obj + 0x38), D_00465AC0)) {
        *(int*)((char*)obj + 0x18) = 3;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C1EA8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cFEStateNetNews_cFEStateNetNews(cFEStateNet* self, int a1);
extern "C" void* cFEStateNetQuickMessages_cFEStateNetQuickMessages(cFEStateNetQuickMessages* self, int a1);
extern "C" void* func_001887A0(void* self, int engine, int reset);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* st, int a2, int a3);
extern const char D_00465AD0[];
extern const char D_00465AE0[];
extern char D_00464860[];
extern const char D_0045D698[];

struct sColor_1C1EA8 {
    float r, g, b, a;
};

struct sTextVE_1C1EA8 {
    short delta;
    short index;
    void (*fn)(void*, sColor_1C1EA8*);
};

extern "C" void func_001C1EA8(cFEStateNet* self, char* msg, int ev)
{
    if (msg == 0) {
        return;
    }
    switch (ev) {
    case 5:
        switch (*(int*)(msg + 0x18)) {
        case 0:
            func_001A8A40(((char*)self), cFEStateNetNews_cFEStateNetNews((cFEStateNet*)cMemMan_alloc(0x6D0, D_00465AD0, 0, 0), (int)self->mpEngine));
            break;
        case 1:
            func_001A8A40(((char*)self), cFEStateNetQuickMessages_cFEStateNetQuickMessages((cFEStateNetQuickMessages*)((cFEStateNet*)cMemMan_alloc(0x6FC, D_00465AE0, 0, 0)), (int)self->mpEngine));
            break;
        case 2: {
            char* p = (char*)cFEStateOnlineAgreement_cFEStateOnlineAgreement((cFEStateOnlineAgreement*)cMemMan_alloc(0x6DC, D_00464860, 0x100, 0), (int)self->mpEngine);
            *(int*)(p + 0x18) = 0x13;
            func_001A8A40(((char*)self), p);
            break;
        }
        case 3:
            if ((self->mAttrState & 6) == 0) {
                char* q = (char*)func_001887A0(cMemMan_alloc(0x54, D_0045D698, 0x100, 0), (int)self->mpEngine, 0);
                *(int*)(q + 0x1C) = (*(int*)(q + 0x1C) & ~0x3F00) | 0x100;
                cUIStateStack_pushSpecial((char*)self->mpEngine + 0x18, q, 0, 0);
            }
            break;
        }
        break;
    case 6:
        func_001A8A40(((char*)self), cFEStateOnlineMainMenu_cFEStateOnlineMainMenu((cFEStateNet*)cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), (int)self->mpEngine));
        break;
    case 1: {
        sColor_1C1EA8 c;
        sTextVE_1C1EA8* vt = *(sTextVE_1C1EA8**)(msg + 8);
        char* obj = msg + vt[11].delta;
        c.r = 1.0f;
        c.g = 1.0f;
        c.b = 1.0f;
        c.a = 1.0f;
        vt[11].fn(obj, &c);
        break;
    }
    case 2: {
        sColor_1C1EA8 c;
        sTextVE_1C1EA8* vt = *(sTextVE_1C1EA8**)(msg + 8);
        char* obj = msg + vt[11].delta;
        c.r = 1.0f;
        c.g = 0.0f;
        c.b = 0.0f;
        c.a = 0.0f;
        vt[11].fn(obj, &c);
        break;
    }
    case 8:
        if ((self->mAttrState & 6) == 0) {
            func_001A9B58(self);
        }
        break;
    default:
        func_001A8918(self, (int)msg, ev);
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

extern "C" void* cFEStateNetQuickMessages_cFEStateNetQuickMessages(cFEStateNetQuickMessages* self, int a1)
{
    func_001A8500_3(((cFEStateNet*)self), a1, 0);
    self->mHashName = 0x4B;
    self->mVtbl = cFEStateNetQuickMessages_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2138);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char D_00465B20[];

extern "C" void func_001C2138(cFEStateNetQuickMessages* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00465B20), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    self->mbChanged = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C21A0__FPv);
#ifdef SKIP_ASM
void* func_001C21A0(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C21C0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern char D_00465B38[];
extern char D_00465B50[];
extern char D_0045FE60[];
extern char D_00465B70[];
extern char D_004614E8[];
extern char D_00465B88[];

extern "C" void func_001C21C0(cFEStateNetQuickMessages* self, void* popup)
{
    func_001DA528(popup, 4);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465B38);
    func_001DA530(popup, 0, GetHashValue32(D_00465B50));
    func_001DA510(popup, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(popup, 1, GetHashValue32(D_00465B70), 0);
    func_001DA510(popup, 2, GetHashValue32(D_004614E8), 1);
    func_001DA510(popup, 3, GetHashValue32(D_00465B88), 7);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2298);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char D_00465BA0[];
extern char D_004A1398[];
extern char D_00465BB0[];
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

extern "C" void func_001C2298(cFEStateNetQuickMessages* self, cUIText* text)
{
    char buf[32];
    if (IsHash1C2298(*(int*)((char*)text + 0x38), D_00465BA0)) {
        cUIText_setAsciiString(text, D_004A1390);
        ((cFEVObj_001C2298*)text)->v09(1);
    } else if (IsHash1C2298(*(int*)((char*)text + 0x38), D_004A1398)) {
        *(int*)((char*)text + 0x14) |= 1;
    } else {
        int i;
        for (i = 0; i < 10; i++) {
            sprintf(buf, D_00465BB0, i);
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
INCLUDE_ASM("fe/feasyncfile", func_001C23C0);
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

extern "C" int func_001C23C0(cFEStateNetQuickMessages* self, int a1)
{
    ((cFEVObj_001C23C0*)self)->v32();
    if (a1 != 0) {
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
extern char D_004A1C08[];

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
    sBXString1C2468 s;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    sprintf(buf, D_004A1C08, idx + 1);
    cBXString_cBXString2(&s, buf);
    char* msg = func_0014F6A8(iface, idx);
    if (msg == 0) {
        char tmp[0x40];
        char* o = (char*)SSXApp->FELocale;
        sVE_001C2468* vt = *(sVE_001C2468**)(o + 4);
        char* obj = o + vt[4].delta;
        void* str = vt[4].fn(obj, cBEOptionInterface_getDefaultQuickKeyMessageHashValue(iface, idx));
        USTR_strcpyW(tmp, str);
        cBXString_Concat(&s, tmp);
    } else {
        cBXString_Concat(&s, msg);
    }
    func_001C2578(self, *(void**)((char*)self + (idx << 2) + 0x6D4), s.str);
    cBXString__cBXString(&s, 2);
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
extern "C" char* func_004162D0(char*, const char*);
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
    char buf[0x50];
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
        func_004162D0(buf, ell);
        cUIText_setAsciiString(text, buf);
    } else {
        cUIText_setAsciiString(text, str);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2740);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int i);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" char* func_0014F6A8(void* self, int i);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
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
extern char D_00465BC0[];
extern char D_00465BE0[];
extern char D_00465C00[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];
extern const char D_004611A8[];

struct sVE_001C2740 {
    short delta;
    short index;
    void* (*fn)(void* self, int hash);
};

extern "C" void func_001C2740(cFEStateNetQuickMessages* self, void* w, int msg)
{
    switch (msg) {
    case 5: {
        unsigned int idx = *(unsigned int*)((char*)w + 0x18);
        if (idx < 10) {
            void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
            char* name = func_0014F6A8(iface, idx);
            if (name == 0) {
                char tmp[0x40];
                char* o = (char*)SSXApp->FELocale;
                sVE_001C2740* vt = *(sVE_001C2740**)(o + 4);
                char* obj = o + vt[4].delta;
                void* str = vt[4].fn(obj, cBEOptionInterface_getDefaultQuickKeyMessageHashValue(iface, idx));
                USTR_strcpyW(tmp, str);
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(D_00465BC0), (int)tmp, 0, idx, 0x3F);
            } else {
                func_001A9150((cFEStateNet*)self, 0, GetHashValue32(D_00465BC0), (int)name, 0, idx, 0x3F);
            }
            cKeyboardPopup_setIncludeKey(self->mpKeyboard, 0x4B, 1, 2);
        }
        break;
    }
    case 6:
        if (self->mbChanged != 0) {
            void* p = func_001A9C50((cFEStateNet*)self);
            *(int*)((char*)p + 0xBC) = 2;
            char* sub = (char*)p + 0xBC;
            int h = GetHashValue32(D_00465BE0);
            *(int*)((char*)p + 0xC0) = 3;
            *(int*)((char*)p + 0xC4) = h;
            func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
            func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
            *(int*)((char*)p + 0x168) = 0;
            *(int*)((char*)p + 0x150) = 0x83A;
            func_001A9BC0((cFEStateNet*)self, p);
        } else {
            func_001A8A40(self, cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(0x6D0, D_004611A8, 0, 0), (int)self->mpEngine));
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
        void* p = func_001A9C50((cFEStateNet*)self);
        *(int*)((char*)p + 0xBC) = 2;
        char* sub = (char*)p + 0xBC;
        int h = GetHashValue32(D_00465C00);
        *(int*)((char*)p + 0xC0) = 3;
        *(int*)((char*)p + 0xC4) = h;
        func_001C57C0(sub, 0, GetHashValue32(D_0045DCC8));
        func_001C57C0(sub, 1, GetHashValue32(D_0045FCD8));
        *(int*)((char*)p + 0x150) = 0x83B;
        *(int*)((char*)p + 0x168) = 1;
        func_001A9BC0((cFEStateNet*)self, p);
        break;
    }
    default:
        func_001A8918((cFEStateNet*)self, (int)w, msg);
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
extern char D_00465C48[];
extern char D_00461618[];
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
            *(int*)((char*)pop + 0xC4) = GetHashValue32(D_00465C48);
            *(int*)((char*)pop + 0xC0) = 3;
            func_001C57C0(sub, 0, GetHashValue32(D_00461618));
            *(int*)((char*)pop + 0x150) = 0x83C;
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
INCLUDE_ASM("fe/feasyncfile", func_001C2B48);
#ifdef SKIP_ASM
extern "C" int func_001A97B8(void* self, void* a1, int a2);
extern "C" void func_001C2C50(cFEStateNetQuickMessages* self, void* a1, int a2);
extern "C" void func_001C2BA8(cFEStateNetQuickMessages* self, void* a1, int a2);

extern "C" void func_001C2B48(cFEStateNetQuickMessages* self, void* a1, int a2)
{
    switch (*(int*)((char*)a1 + 0x18)) {
    case 0x83A:
        func_001C2C50(self, a1, a2);
        break;
    case 0x83B:
        func_001C2BA8(self, a1, a2);
        break;
    default:
        func_001A97B8(self, a1, a2);
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
extern const char D_00460D90[];
extern const char D_004611A8[];

extern "C" void func_001C2C50(cFEStateNetQuickMessages* self, void* a1, int a2)
{
    switch (a2) {
    case 0xF:
        func_001A97B8(((cFEStateNet*)self), a1, 0xF);
        func_001A8A40(((cFEStateNet*)self), func_0018D7F0(cMemMan_alloc(0x23C, D_00460D90, 0, 0), (int)self->mpEngine, 2, 1, 1));
        break;
    case 0x10:
        func_001A97B8(((cFEStateNet*)self), a1, 0x10);
        func_001A8A40(((cFEStateNet*)self), cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(0x6D0, D_004611A8, 0, 0), (int)self->mpEngine));
        break;
    default:
        func_001A97B8(((cFEStateNet*)self), a1, a2);
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
    self->mHashName = 0x42;
    cFEStateStats* s = self;
    s->miMode = 0;
    s->miFocusMenu = 0;
    s->miFirstResult = 0;
    for (int i = 3; i >= 0; i--) {
        ((int*)&s->mpMenus[0])[i] = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C2D90);
#ifdef SKIP_ASM
extern void* cFEStateStats_vtbl[];

extern "C" void* func_001C2D90(cFEStateStats* self)
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
extern char D_00465C68[];

/**
 * Opens the "249Stats" screen and, once it exists, fills the result rows (func_001C3310).
 */
extern "C" void cFEStateStats_onCreateScreen(cFEStateStats* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((cFEStateNet*)self), GetHashValue32(D_00465C68), 0);
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
 * Runs one frame of the stats screen: only the cFEStateNet update (func_001A8770).
 *
 * @return func_001A8770()'s result (the pending-event handler's value)
 */
void* cFEStateStats_onUpdate(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateStats_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIMenu_setInputMap(void* self, int a1);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern char D_00465C78[];
extern char D_00465C88[];
extern char D_00465C98[];
extern char D_004A1C10[];
extern char D_00465CA8[];
extern char D_00465CB8[];
extern char D_00465CC8[];
extern char D_00465CE0[];
extern char D_004A1C18[];
extern char D_00465CF0[];
extern char D_00465D00[];
extern char D_00465D10[];
extern char D_004A1BB8[];
extern char D_00465D20[];
extern char D_00465D30[];
extern char D_00465D40[];
extern char D_00465D50[];
extern char D_00465D60[];
extern char D_00465D70[];
extern char D_00465D80[];

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
    char* o = (char*)obj;
    if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465C78) || IsHash_001C2F00(*(int*)(o + 0x38), D_00465C88)) {
        *(int*)(o + 0x14) &= ~1;
        obj->v11(sCol_001C2F00(1.0f));
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465C98)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_004A1C10)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465CA8)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465CB8)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465CC8)) {
        obj->v09(0);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465CE0)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_004A1C18)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465CF0)) {
        obj->v07(1);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465D00)) {
        obj->v07(1);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465D10)) {
        obj->v07(1);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_004A1BB8)) {
        obj->v09(1);
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465D20)) {
        *(int*)(o + 0x14) &= ~1;
        obj->v07(1);
        self->mpMenus[0] = (cUIMenu*)obj;
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465D30)) {
        self->mpMenus[1] = (cUIMenu*)obj;
        *(int*)(o + 0x90) |= 8;
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465D40)) {
        self->mpMenus[2] = (cUIMenu*)obj;
        *(int*)(o + 0x90) |= 8;
    } else if (IsHash_001C2F00(*(int*)(o + 0x38), D_00465D50)) {
        cUIMenu_setScrollableMenu(obj, 10);
        self->mpMenus[3] = (cUIMenu*)obj;
        cUIMenu_setInputMap(obj, 1);
        ((cFEVObj_001C2F00*)self->mpMenus[3])->v07(1);
    } else {
        char buf[32];
        for (int i = 0; i < 10; i++) {
            sprintf(buf, D_00465D60, i);
            if (IsHash_001C2F00(*(int*)(o + 0x38), buf)) {
                obj->v09(1);
                break;
            }
            sprintf(buf, D_00465D70, i);
            if (IsHash_001C2F00(*(int*)(o + 0x38), buf)) {
                obj->v09(1);
                break;
            }
            sprintf(buf, D_00465D80, i);
            if (IsHash_001C2F00(*(int*)(o + 0x38), buf)) {
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
extern char D_00465D90[];
extern char D_00465DA0[];
extern char D_00465DB0[];
extern char D_004A1390_s __asm__("D_004A1390");
extern char D_004A1C20[];

extern "C" void func_001C3310(cFEStateStats* self_)
{
    char name[32];
    char txt[64];
    int v0;
    int v1;
    if (self_->mpScreen != 0) {
        int n = func_0025F338(LobbyMan_mLobbyMan);
        char* obj = (char*)cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(D_00465D50));
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
            sprintf(name, D_00465D90, i);
            cUIText* t0 = cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(name));
            sprintf(name, D_00465DA0, i);
            cUIText* t1 = cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(name));
            sprintf(name, D_00465DB0, i);
            cUIText* t2 = cUIScreen_getObjectByHashName(self_->mpScreen, GetHashValue32(name));
            char* e = func_0025F388(LobbyMan_mLobbyMan, idx, pa, pb);
            if (e != 0 && n > 0) {
                if (t2 != 0) {
                    if (self_->mbShowTime != 0) {
                        func_001C4F58(self_, txt, v0);
                    } else {
                        sprintf(txt, D_004A12C0, v0);
                    }
                    cUIText_setAsciiString(t2, txt);
                }
                if (t0 != 0) {
                    cUIText_setAsciiString(t0, e + 8);
                }
                if (t1 != 0) {
                    sprintf(txt, D_004A1C20, *(int*)(e + 4));
                    cUIText_setAsciiString(t1, txt);
                }
            } else {
                cUIText_setAsciiString(t1, &D_004A1390_s);
                cUIText_setAsciiString(t2, &D_004A1390_s);
                cUIText_setAsciiString(t0, &D_004A1390_s);
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
extern char D_00465DC0[];
extern char D_00465DD8[];
extern char D_0045FE60[];
extern char D_0045FE40[];
extern char D_00461458[];
extern char D_00465DF8[];
extern char D_004614E8[];

extern "C" void func_001C3570(cFEStateStats* self, void* menu)
{
    func_001DA528(menu, 5);
    *(int*)((char*)menu + 0x54) = GetHashValue32(D_00465DC0);
    func_001DA530(menu, 0, GetHashValue32(D_00465DD8));
    func_001DA510(menu, 0, GetHashValue32(D_0045FE60), 3);
    func_001DA510(menu, 1, GetHashValue32(D_0045FE40), 4);
    func_001DA510(menu, 2, GetHashValue32(D_00461458), 2);
    func_001DA510(menu, 3, GetHashValue32(D_00465DF8), 8);
    func_001DA510(menu, 4, GetHashValue32(D_004614E8), 1);
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
extern "C" void func_001A8918_3668(void* self, void* w, int msg) __asm__("func_001A8918");
extern "C" void func_001A8A40_3668(void* self, void* obj) __asm__("func_001A8A40");
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu_3668(void* mem, void* a1) __asm__("cFEStateOnlineMainMenu_cFEStateOnlineMainMenu");
extern "C" void func_001C49C8_3668(void* self) __asm__("func_001C49C8");
extern "C" int func_0039A738_3668(void* w) __asm__("func_0039A738");
extern "C" void func_001C4B78_3668(void* self, int v) __asm__("func_001C4B78");
extern "C" void func_001C3310_3668(void* self) __asm__("func_001C3310");
extern "C" void func_001C3EE8_3668(void* self, int mode) __asm__("func_001C3EE8");
extern "C" void func_001A9B58_3668(void* self) __asm__("func_001A9B58");
extern "C" void func_003A0290_3668(void* w, void* pos) __asm__("func_003A0290");
extern "C" int func_0025F338_3668(void* p) __asm__("func_0025F338");
extern "C" void func_001C3D68_3668(void* self, int idx, int count) __asm__("func_001C3D68");
extern const char D_0045FF28[];
extern char D_00465C78[];
extern char D_00465C88[];
extern char D_00465C98[];
extern char D_00465CA8[];
extern char D_00465CB8[];
extern char D_00465CC8[];
extern char D_00465CF0[];
extern char D_00465D00[];
extern char D_00465D10[];
extern char D_00465D60[];
extern char D_00465E10[];
extern char D_00465E20[];
extern char D_004A1C10[];

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
 * 9, 1, 2 and any other also go to the cFEStateNet handler (func_001A8918).
 *
 * @param w  the widget (On Tour pWidget); null is ignored
 * @param msg  the widget event (On Tour Event)
 */
// Widget events of the online leaderboard screen: page buttons, row highlight and row selection.
extern "C" void cFEStateStats_onWidgetEvent(cFEStateStats* self, cObj_3668* w, int msg)
{
    cObj_3668* o;
    cObjB_3668* p;
    if (w == 0) {
        return;
    }
    switch (msg) {
    case 6:
        func_001A8A40_3668(((char*)self), cFEStateOnlineMainMenu_cFEStateOnlineMainMenu_3668(cMemMan_alloc(0x6D0, D_0045FF28, 0, 0), self->mpEngine));
        return;
    case 9:
        if (isButton_3668(w, D_00465C78)) {
            func_001C49C8_3668(((char*)self));
        } else if (isButton_3668(w, D_00465C88)) {
            func_001C4B78_3668(((char*)self), func_0039A738_3668(w));
            func_001C3310_3668(((char*)self));
        }
    case 1:
        if (self->mpScreen == 0) {
            return;
        }
        if (isButton_3668(w, D_00465CF0)) {
            func_001C3EE8_3668(((char*)self), 0);
        } else if (isButton_3668(w, D_00465D10)) {
            func_001C3EE8_3668(((char*)self), 2);
        } else if (isButton_3668(w, D_00465D00)) {
            func_001C3EE8_3668(((char*)self), 1);
        } else if (isButton_3668(w, D_00465C78)) {
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
            o = getObj_3668(((char*)self), D_00465CA8);
            o->setVisible(1);
            o = getObj_3668(((char*)self), D_00465C98);
            o->setVisible(1);
            o = getObj_3668(((char*)self), D_00465E10);
            o->setVisible(1);
        } else if (isButton_3668(w, D_00465C88)) {
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
            o = getObj_3668(((char*)self), D_00465CC8);
            o->setVisible(1);
            o = getObj_3668(((char*)self), D_00465CB8);
            o->setVisible(1);
            o = getObj_3668(((char*)self), D_00465E20);
            o->setVisible(1);
        } else {
            char buf[0x20];
            for (int i = 0; i < 10; i++) {
                sprintf(buf, D_00465D60, i);
                if (isButton_3668(w, buf)) {
                    cPos_3668 pos;
                    cPos_3668 size;
                    cObj_3668* t = getObj_3668(((char*)self), D_004A1C10);
                    func_003A0290_3668(w, &pos);
                    w->getSize(&size);
                    pos.y = pos.y - size.y + 14.0f;
                    pos.x = pos.x + -40.0f;
                    *(cPos_3668*)((char*)t + 0x44) = pos;
                    t->setVisible(1);
                    func_001C3D68_3668(((char*)self), i + self->miFirstResult, func_0025F338_3668(LobbyMan_mLobbyMan));
                    break;
                }
            }
        }
        break;
    case 2:
        if (isButton_3668(w, D_00465CF0)) {
            if (self->miMode != 0) {
                break;
            }
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (isButton_3668(w, D_00465D10)) {
            if (self->miMode != 2) {
                break;
            }
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (isButton_3668(w, D_00465D00)) {
            if (self->miMode != 1) {
                break;
            }
            w->setColor(cVec_3668(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (isButton_3668(w, D_00465C78)) {
            w->setColor(cVec_3668(1.0f, 0.0f, 0.0f, 0.0f));
            p = (cObjB_3668*)getObj_3668(((char*)self), D_00465CA8);
            p->setVisible(0);
            p = (cObjB_3668*)getObj_3668(((char*)self), D_00465C98);
            p->setVisible(0);
            p = (cObjB_3668*)getObj_3668(((char*)self), D_00465E10);
            p->setVisible(0);
        } else if (isButton_3668(w, D_00465C88)) {
            w->setColor(cVec_3668(1.0f, 0.0f, 0.0f, 0.0f));
            p = (cObjB_3668*)getObj_3668(((char*)self), D_00465CC8);
            ((cObjC_3668*)p)->setVisible(0);
            p = (cObjB_3668*)getObj_3668(((char*)self), D_00465CB8);
            ((cObjC_3668*)p)->setVisible(0);
            p = (cObjB_3668*)getObj_3668(((char*)self), D_00465E20);
            ((cObjC_3668*)p)->setVisible(0);
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
    func_001A8918_3668(((char*)self), w, msg);
    return;
other:
    func_001A8918_3668(((char*)self), w, msg);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C3D68);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern char D_00465CE0[];
extern char D_004A1C18[];

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
    cUIObj_001C3D68* track = (cUIObj_001C3D68*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465CE0));
    cUIObj_001C3D68* thumb = (cUIObj_001C3D68*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1C18));
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
extern char D_00465D10[];
extern char D_00465CF0[];
extern char D_00465D00[];
extern char D_00465E58[];
extern char D_004A1C28[];
extern char D_00465E68[];
extern char D_00465E78[];
extern char D_00465E88[];
extern char D_00465E98[];

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
    cFEVObj_001C3EE8* o;
    switch (mode) {
    case 2:
        self->mbShowTime = 0;
        func_001C4538(self, 1);
        o = Find_001C3EE8(self, D_00465D10);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465CF0);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465D00);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465E58);
        if (o != 0) {
            o->setVisible(0);
        }
        o = Find_001C3EE8(self, D_004A1C28);
        if (o != 0) {
            o->setVisible(0);
        }
        o = Find_001C3EE8(self, D_00465E68);
        if (o != 0) {
            o->setVisible(1);
        }
        {
            cUIText* t = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465E78));
            if (t != 0) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_00465E88));
            }
        }
        if (changed) {
            func_001C47A8(self);
        }
        break;
    case 0:
        self->mbShowTime = 0;
        func_001C4538(self, 0);
        o = Find_001C3EE8(self, D_00465CF0);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465D10);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465D00);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465E58);
        if (o != 0) {
            o->setVisible(1);
        }
        o = Find_001C3EE8(self, D_004A1C28);
        if (o != 0) {
            o->setVisible(0);
        }
        o = Find_001C3EE8(self, D_00465E68);
        if (o != 0) {
            o->setVisible(0);
        }
        {
            cUIText* t = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465E78));
            if (t != 0) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_00465E88));
            }
        }
        func_001C4B78(self, 0);
        break;
    case 1:
        self->mbShowTime = 1;
        func_001C4538(self, 1);
        o = Find_001C3EE8(self, D_00465D00);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465CF0);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465D10);
        if (o != 0) {
            o->setColor(sColor_001C3EE8(1.0f, 1.0f, 1.0f, 1.0f));
        }
        o = Find_001C3EE8(self, D_00465E58);
        if (o != 0) {
            o->setVisible(0);
        }
        o = Find_001C3EE8(self, D_004A1C28);
        if (o != 0) {
            o->setVisible(1);
        }
        o = Find_001C3EE8(self, D_00465E68);
        if (o != 0) {
            o->setVisible(0);
        }
        {
            cUIText* t = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465E78));
            if (t != 0) {
                cUIText_setUnicodeStringByID(t, GetHashValue32(D_00465E98));
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
extern char D_00465EA8[];
extern char D_00465D30[];
extern char D_00465C78[];
extern char D_00465EB8[];
extern char D_00465D40[];
extern char D_00465C88[];
extern char D_00465EC8[];
extern char D_00465ED8[];
extern char D_00465E10[];
extern char D_00465E20[];

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
    Show_001C4538(((cFEStateNet*)self), D_00465EA8, on);
    Show_001C4538(((cFEStateNet*)self), D_00465D30, on);
    Show_001C4538(((cFEStateNet*)self), D_00465C78, on);
    Show_001C4538(((cFEStateNet*)self), D_00465EB8, on);
    Show_001C4538(((cFEStateNet*)self), D_00465D40, on);
    Show_001C4538(((cFEStateNet*)self), D_00465C88, on);
    Show_001C4538(((cFEStateNet*)self), D_00465EC8, on);
    Show_001C4538(((cFEStateNet*)self), D_00465ED8, on);
    if (on == 0) {
        Show_001C4538(((cFEStateNet*)self), D_00465E10, 0);
        Show_001C4538(((cFEStateNet*)self), D_00465E20, 0);
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
extern char D_00465C78[];
extern char D_00465EE8[];
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
    cFEVObj_001C47A8* box = (cFEVObj_001C47A8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465C78));
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
                cUIListBox_addEntryByStringID(box, GetHashValue32(D_00465EE8), 4);
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
                cUIListBox_addEntryByStringID(box, GetHashValue32(D_00465EE8), 4);
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
extern "C" int func_0039A738(void* a);
extern "C" void func_0039A7A8(void* self, int a1);
extern char D_00465C78[];
extern char D_00465C88[];
extern char D_00465D50[];

struct sEntry1C49C8 {
    int data;
    int name;
    int key;
    int mode;
};
extern sEntry1C49C8 D_0046D9B8[];

extern "C" void func_001C49C8(cFEStateStats* self)
{
    cUIText* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465C78));
    if (box == 0) {
        return;
    }
    int key = func_0039A738(box);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465C88));
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
            func_0039A7A8(box, 0);
            cUIMenu_setSelectedByIndex(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465D50)), 0);
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
    return 0x101;
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
    if (msg == 0xE1) {
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
extern char D_004A5A58;

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
    if (snd == 0) snd = &D_004A5A58;
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
extern char D_004A5A58;

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
    if (snd == 0) snd = &D_004A5A58;
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
extern char D_00465F00[];
extern char D_00465F18[];

// Formats a frame count (60 fps) as hours:minutes:seconds.milliseconds, dropping the hours when zero.
extern "C" void func_001C4F58(cFEStateStats* self, char* buf, int frames)
{
    float t = frames * (1.0f / 60.0f);
    int ms = (int)((t - (int)t) * 1000.0f);
    int secs = (int)t;
    int mins = secs / 60;
    int hours = mins / 60;
    secs -= mins * 60;
    mins -= hours * 60;
    if (hours > 0) {
        sprintf(buf, D_00465F00, hours, mins, secs, ms);
    } else {
        sprintf(buf, D_00465F18, mins, secs, ms);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cFEStateNetNews_cFEStateNetNews);
#ifdef SKIP_ASM
extern void* cFEStateNetNews_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* cFEStateNetNews_cFEStateNetNews(cFEStateNet* self, int a1)
{
    func_001A8500_3(self, a1, 0);
    self->mHashName = 0x43;
    self->mVtbl = cFEStateNetNews_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5058);
#ifdef SKIP_ASM
extern void* cFEStateNetNews_vtbl[];
// PORT: func_001A8500 takes (self, a1, a2); the unit declares it with one arg
void* func_001A8500_3(void* self, int a1, int a2) __asm__("func_001A8500");

extern "C" void* func_001C5058(cFEStateNet* self)
{
    func_001A8500_3(self, 0, 0);
    self->mVtbl = cFEStateNetNews_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5098);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0025E0C8(void* self, int a1);
extern char D_004A1C30[];

extern "C" void func_001C5098(cFEStateNet* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A1C30), 0);
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
INCLUDE_ASM("fe/feasyncfile", func_001C5178__FPv);
#ifdef SKIP_ASM
void* func_001C5178(void* self)
{
    return func_001A8770((cFEStateNet*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5198);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" cUIText* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern char D_00465F28[];
extern char D_004619C8[];
extern char D_004619D8[];
extern char D_004619E8[];
extern char D_00465F38[];
extern char D_00465720[];
extern char D_00465730[];

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

extern "C" int func_001C5198(cFEStateNet* self, int on)
{
    if (on) {
        char* o = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465F28));
        cUIText* a = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619C8));
        cUIText* b = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619D8));
        cUIText* c = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004619E8));
        *(cUIText**)(o + 0x7C) = a;
        *(cUIText**)(o + 0x80) = b;
        if ((unsigned int)strlen(D_00465720) > 24) {
            o[0x8C] = 0;
        } else {
            strcpy(o + 0x8C, D_00465720);
        }
        if ((unsigned int)strlen(D_00465730) > 24) {
            o[0xA5] = 0;
        } else {
            strcpy(o + 0xA5, D_00465730);
        }
        *(cUIText**)(o + 0x84) = c;
        char* x = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465F38));
        *(char**)(x + 0xB8) = o;
        ((cFEVObj_001C5198*)x)->v07(1);
        if (o) {
            ((cFEVObj_001C5198*)o)->v09(1);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5310);
#ifdef SKIP_ASM
extern char D_00465F48[];
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

extern "C" void func_001C5310(cFEStateNet* self, cFEVObj_001C5310* item)
{
    int hash = *(int*)((char*)item + 0x38);
    if (hash == GetHashValue32(D_00465F48)) {
        item->v07(1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feasyncfile", func_001C5368);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_001A8918(cFEStateNet* self, int a1, int msg);
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" void func_001A9B58(cFEStateNet* self);
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(cFEStateNet* self, int a1);
extern const char D_004611A8[];

extern "C" void func_001C5368(cFEStateNet* self, int a1, unsigned int msg)
{
    if (a1 != 0) {
        switch (msg) {
        case 8:
            if ((self->mAttrState & 6) == 0) {
                func_001A9B58(self);
            }
            break;
        case 6:
            func_001A8A40(self, cFEStateNetSSXWorld_cFEStateNetSSXWorld((cFEStateNet*)cMemMan_alloc(0x6D0, D_004611A8, 0, 0), (int)self->mpEngine));
            break;
        case 5:
            break;
        default:
            func_001A8918(self, a1, msg);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5420);
#ifdef SKIP_ASM
extern "C" int func_001C5420(cFEStateNet* self, void* a1, int a2)
{
    return a2 == 4 ? 0 : 0x101;
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
extern "C" void func_003A19F8(cUIText* text, char* str);
// PORT: unit declares func_001A8E40 with one arg; the handler passes (self, a1, a2) through
extern "C" void* func_001A8E40_3(void* self, int a1, int a2) __asm__("func_001A8E40");
extern char D_00465F38[];

struct sBXString1C5430 {
    char* str;
    sBXString1C5430() {}
    sBXString1C5430(const sBXString1C5430& o);
};

// PORT: a2 is a message-data pointer passed as int
extern "C" void* func_001C5430(cFEStateNet* self, int msg, int a2)
{
    if (msg == 0xE2) {
        cUIText* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00465F38));
        if (text != 0) {
            sBXString1C5430 s;
            cBXString_cBXString1(&s, (char*)a2 + 8);
            int pos;
            while ((pos = cBXString_FindLastOf((cBXString*)&s, '\n', 0)) != -1) {
                int next = pos + 1;
                if (s.str[next] == '\n') {
                    cBXString_setAt((cBXString*)&s, pos, '\r');
                    cBXString_setAt((cBXString*)&s, next, '\r');
                } else {
                    cBXString_setAt((cBXString*)&s, pos, ' ');
                }
            }
            func_003A19F8(text, s.str);
            cBXString__cBXString(&s, 2);
            return 0;
        }
    } else {
        return func_001A8E40_3(self, msg, a2);
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
extern char D_00465F58[];
extern char D_00465F70[];
extern char D_00465F90[];
extern char D_004614E8[];
extern char D_00461458[];

extern "C" void func_001C5540(cFEStateNet* self, void* popup)
{
    func_001DA528(popup, 3);
    *(int*)((char*)popup + 0x54) = GetHashValue32(D_00465F58);
    func_001DA530(popup, 0, GetHashValue32(D_00465F70));
    func_001DA510(popup, 0, GetHashValue32(D_00465F90), 3);
    func_001DA510(popup, 1, GetHashValue32(D_004614E8), 1);
    func_001DA510(popup, 2, GetHashValue32(D_00461458), 2);
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
    char* b = (char*)self + 0x54;
    char* a = (char*)self + 0x24;
    func_00416210((char*)self + 0x4, 0, 8);
    func_00416210((char*)self + 0xC, 0, 8);
    func_00416210((char*)self + 0x14, 0, 8);
    func_00416210((char*)self + 0x1C, 0, 8);
    int i;
    for (i = 4; i >= 0; i--) {
        func_00416210(a, 0, 8);
        a += 8;
        func_00416210(b, 0, 0xC);
        b += 0xC;
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

void func_001C5780(void* self, int a1, int a2)
{
    cFEAsyncSlots* s = (cFEAsyncSlots*)self;
    s->mSlots[a1].mValue = a2;
    s->mSlots[a1].mState = 2;
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
    cFEAsyncSlots* s = (cFEAsyncSlots*)self;
    s->mSlots[i] = *(cFEAsyncSlot*)src;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_getPopupOptionNames);
#ifdef SKIP_ASM
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

extern "C" void func_001C5810(void* self, int i, int a, int b)
{
    sFEAsyncReqs_1C5810* s = (sFEAsyncReqs_1C5810*)self;
    s->mReqs[i].mValue = a;
    s->mReqs[i].mState = 2;
    s->mReqs[i].mExtra = b;
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

extern "C" void func_001C5838(void* self, int a1, int a2, int a3)
{
    cFEAsyncReqs* s = (cFEAsyncReqs*)self;
    s->mReqs[a1].mValue = a2;
    s->mReqs[a1].mState = 1;
    s->mReqs[a1].mExtra = a3;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_clearPopupConfirmation);
#ifdef SKIP_ASM
extern "C" void cPopupInfo_clearPopupConfirmation(cPopupInfo* self, int iEntryNum)
{
    char* p = (char*)self + iEntryNum * 0xc;
    *(int*)(p + 0x54) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C5878);
#ifdef SKIP_ASM
extern "C" void func_001C5878(void* self, int a1, int a2, int a3)
{
    cFEAsyncReqs* s = (cFEAsyncReqs*)self;
    s->mReqs[a1].mValue = a2;
    s->mReqs[a1].mState = 3;
    s->mReqs[a1].mExtra = a3;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", func_001C58A0);
#ifdef SKIP_ASM
extern "C" void func_001C58A0(void* self, int i, void* src)
{
    cFEAsyncReqs* s = (cFEAsyncReqs*)self;
    s->mReqs[i] = *(cFEAsyncReq*)src;
}
#endif

//100%
INCLUDE_ASM("fe/feasyncfile", cPopupInfo_getPopupConfirmation);
#ifdef SKIP_ASM
extern "C" void* cPopupInfo_getPopupConfirmation(cPopupInfo* self, int iEntryNum)
{
    return (char*)self + (iEntryNum * 0xc + 0x54);
}
#endif

