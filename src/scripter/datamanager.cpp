#include "common.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_cDataManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cDataManager_AddToEmptyInstanceList(void* self, void* node);
extern char cDataManager_vtbl[];
extern const char str_tDataInstanceInfo[];

struct sDataNode5828 {
    char pad[0x18];
};

struct cDataManager5828 {
    int count;              // 0x0
    sDataNode5828* nodes;   // 0x4
    int field_0x8;          // 0x8
    void* vtable;           // 0xC
};

/**
 * Constructs a data manager with a pool of data-instance records, all placed on the empty list.
 *
 * @param flags  memory flags for the record pool ("tDataInstanceInfo")
 * @param count  number of instance records (On Tour maxDataInstances)
 * @return @p self
 */
extern "C" cDataManager5828* cDataManager_cDataManager(cDataManager5828* self, int flags, int count)
{
    self->vtable = cDataManager_vtbl;
    self->count = count;
    self->nodes = (sDataNode5828*)operator_new_tag(count * sizeof(sDataNode5828), str_tDataInstanceInfo, flags, 0);
    self->field_0x8 = 0;
    for (int i = 0; i < count; i++) {
        cDataManager_AddToEmptyInstanceList(self, &self->nodes[i]);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager__cDataManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern char cDataManager_vtbl[];

/**
 * Destroys the data manager.
 *
 * Restores cDataManager_vtbl (its vptr is at +0xC), frees the instance heap at +0x4 with cMemMan_free() when there is
 * one, and frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cDataManager__cDataManager(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = cDataManager_vtbl;
    void* instanceHeap = *(void**)((char*)self + 0x4);
    if (instanceHeap != 0) {
        cMemMan_free(instanceHeap);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_AttachData);
#ifdef SKIP_ASM
void* func_00275CF8(void* self);
extern "C" int BIG_sizeofheader(void* data);
struct sDmNode5B98;
extern "C" sDmNode5B98* cDataManager_AttachDataFromBigfile(void* self, char* big, sDmNode5B98* prev);
extern int REALabortflag[];

struct sDmVEntry5920 {
    short delta;
    short index;
    void (*fn)(void*, int, void*, int, void*, void*);
};

struct sDmNode5920 {
    int field_0x0;
    int field_0x4;          // 0x4
    void* data;             // 0x8
    int size;               // 0xC
    int refs;               // 0x10
};

/**
 * Registers a block of data with the data manager and returns its instance record.
 *
 * Takes a free instance (func_00275CF8, On Tour GetEmptyInstance) and stores the data (+0x8), its size (+0xC) and a
 * reference count of 1 (+0x10). BIG_sizeofheader() then tells whether the data is a BIG archive (REALabortflag is
 * cleared around the call so a non-BIG block does not abort). A BIG file calls virtual slot 4 of the manager's vtable
 * (+0xC) and attaches its members with cDataManager_AttachDataFromBigfile; any other block calls slot 2. Both callbacks get the name, data,
 * size, the record and the record's +0x4 word. cSSXScriptEngine_SyncLoadScriptBank and
 * cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete attach the script banks with it.
 *
 * @param pName  the data's name, passed to the callbacks
 * @param data  the data block
 * @param size  its size in bytes
 * @return the instance record (the data handle)
 */
extern "C" sDmNode5920* cDataManager_AttachData(void* self, int pName, void* data, int size)
{
    sDmNode5920* n = (sDmNode5920*)func_00275CF8(self);
    int saved = REALabortflag[0];
    n->data = data;
    n->refs = 1;
    n->size = size;
    REALabortflag[0] = 0;
    int big = BIG_sizeofheader(data);
    REALabortflag[0] = saved;
    if (big != 0) {
        sDmVEntry5920* vt = *(sDmVEntry5920**)((char*)self + 0xC);
        vt[4].fn((char*)self + vt[4].delta, pName, data, size, n, &n->field_0x4);
        cDataManager_AttachDataFromBigfile(self, (char*)data, (sDmNode5B98*)n);
    } else {
        sDmVEntry5920* vt = *(sDmVEntry5920**)((char*)self + 0xC);
        vt[2].fn((char*)self + vt[2].delta, pName, data, size, n, &n->field_0x4);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_DetachData);
#ifdef SKIP_ASM
void cDataManager_AddToEmptyInstanceList(void* self, void* node);

struct sDmNode5A20 {
    int a;              // 0x0
    int b;              // 0x4
    int key;            // 0x8
    int c;              // 0xC
    int pad_10;         // 0x10
    sDmNode5A20* next;  // 0x14
};

class cDataManager5A20 {
public:
    int count;          // 0x0
    void* nodes;        // 0x4
    void* freeList;     // 0x8
    // vptr at 0xC
    virtual void v01();
    virtual void v02();
    virtual void v03(int key, int c, int a, int b);
    virtual void v04();
    virtual void v05(int key, int c, int a, int b);
};

// PORT: the unit declares cDataManager_DetachData as returning void; it really returns the entry's key.
int cDataManager_DetachData_impl(cDataManager5A20* self, sDmNode5A20* entry) __asm__("cDataManager_DetachData");

/**
 * Detaches an attached data handle and every instance linked to it, recycling their records.
 *
 * Each linked instance goes through the detach callback (virtual slot 3); the head then gets slot 5 when it
 * had
 * linked instances, otherwise slot 3.
 *
 * @param entry  the data handle returned by the attach
 * @return the detached data pointer, the handle's first word (On Tour returns void* pData)
 */
int cDataManager_DetachData_impl(cDataManager5A20* self, sDmNode5A20* entry)
{
    sDmNode5A20* first = entry->next;
    if (first != 0) {
        sDmNode5A20* n = first;
        do {
            sDmNode5A20* next = n->next;
            self->v03(n->key, n->c, n->a, n->b);
            cDataManager_AddToEmptyInstanceList(self, n);
            n = next;
        } while (n != 0);
        self->v05(entry->key, entry->c, entry->a, entry->b);
    } else {
        self->v03(entry->key, entry->c, entry->a, entry->b);
    }
    int key = entry->key;
    cDataManager_AddToEmptyInstanceList(self, entry);
    return key;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", cDataManager_DetachAll);
#ifdef SKIP_ASM
extern "C" void cDataManager_DetachData(void* self, void* entry);

struct sDmEntry275B08 {
    char pad_0x00[0x10];
    int used;
    int unk_0x14;
};

struct sDmTable275B08 {
    int count;
    sDmEntry275B08* entries;
};

/**
 * Detaches every data entry of the data manager that is in use, through cDataManager_DetachData().
 */
extern "C" void cDataManager_DetachAll(sDmTable275B08* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->entries[i].used != 0) {
            cDataManager_DetachData(self, &self->entries[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275B78);
#ifdef SKIP_ASM
extern "C" int func_00275B78(void* self, char* dataHandle)
{
    return *(int*)(dataHandle + 8);
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", func_00275B80);
#ifdef SKIP_ASM
extern "C" void func_00275B80(void* self, char* dataHandle, int* out0, int* out1)
{
    *out0 = *(int*)dataHandle;
    *out1 = *(int*)(dataHandle + 4);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_AttachDataFromBigfile);
#ifdef SKIP_ASM
void* func_00275CF8(void* self);
extern "C" int BIG_sizeofheader(void* data);
extern "C" char* BIG_locateentryz(void* big, int a1, int index, int* offset, int* size);
extern int REALabortflag[];

struct sDmVEntry5B98 {
    short delta;
    short index;
    void (*fn)(void*, char*, char*, int, void*, void*);
};

struct sDmNode5B98 {
    int field_0x0;
    int field_0x4;          // 0x4
    char* data;             // 0x8
    int size;               // 0xC
    int refs;               // 0x10
    sDmNode5B98* next;      // 0x14
};

/**
 * Attaches every member of a BIG archive to the data manager as a chain of instance records, recursing into nested
 * archives.
 *
 * Walks the entries with BIG_locateentryz() (entry 0, then 1, 2, ... until it returns 0). A member that is itself a BIG
 * file (BIG_sizeofheader() with REALabortflag cleared) is attached recursively; any other member gets a free instance
 * (func_00275CF8) holding its address (+0x8) and size (+0xC), is linked after the previous record through +0x14 and is
 * announced through virtual slot 2 of the manager's vtable (+0xC) with its name, data, size, record and record+0x4.
 *
 * Likely bug (also in On Tour): only the first BIG_locateentryz() call passes a size pointer; the later calls pass 0,
 * so every member after the first is recorded with the first member's size.
 *
 * @param big  the BIG archive in memory
 * @param prev  the record to chain the new records after
 * @return the last record of the chain
 */
extern "C" sDmNode5B98* cDataManager_AttachDataFromBigfile(void* self, char* big, sDmNode5B98* prev)
{
    int ent[2];
    ent[0] = 0;
    ent[1] = 0;
    int i = 1;
    char* name = BIG_locateentryz(big, 0, 0, &ent[0], &ent[1]);
    while (name != 0) {
        char* sub = big + ent[0];
        int saved = REALabortflag[0];
        REALabortflag[0] = 0;
        int headerSize = BIG_sizeofheader(sub);
        REALabortflag[0] = saved;
        if (headerSize != 0) {
            prev = cDataManager_AttachDataFromBigfile(self, sub, prev);
        } else {
            sDmNode5B98* n = (sDmNode5B98*)func_00275CF8(self);
            n->data = sub;
            n->size = ent[1];
            prev->next = n;
            prev = n;
            sDmVEntry5B98* vt = *(sDmVEntry5B98**)((char*)self + 0xC);
            vt[2].fn((char*)self + vt[2].delta, name, sub, ent[1], prev, &prev->field_0x4);
        }
        name = BIG_locateentryz(big, 0, i++, &ent[0], 0);
    }
    return prev;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_AddToEmptyInstanceList__FPvT0);
#ifdef SKIP_ASM
/**
 * Clears a data instance record and pushes it onto the data manager's free list.
 *
 * Zeroes the record's first five words, links it in front of the list head at +0x8 through its next field
 * (+0x14), and makes it the new head.
 *
 * @param node  the instance record (On Tour tDataInstanceInfo*)
 */
void cDataManager_AddToEmptyInstanceList(void* self, void* node)
{
    *(int*)((char*)node + 0x8) = 0;
    *(int*)((char*)node + 0xc) = 0;
    *(int*)((char*)node + 0x10) = 0;
    *(int*)((char*)node + 0x0) = 0;
    *(int*)((char*)node + 0x4) = 0;
    *(void**)((char*)node + 0x14) = *(void**)((char*)self + 0x8);
    *(void**)((char*)self + 0x8) = node;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275CF8__FPv);
#ifdef SKIP_ASM
void* func_00275CF8(void* self)
{
    void* head = *(void**)((char*)self + 0x8);
    *(void**)((char*)self + 0x8) = *(void**)((char*)head + 0x14);
    *(void**)((char*)head + 0x14) = 0;
    return head;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275D10);
#ifdef SKIP_ASM
extern "C" void func_00283C30(void* self);
void func_00283C20(void* self);

extern "C" void* func_00275D10(void* self)
{
    void* list = (char*)self + 0xC;
    func_00283C30(list);
    func_00283C20(list);
    *(int*)((char*)self + 0xBC) = -1;
    *(int*)((char*)self + 0x4) = 1;
    *(int*)((char*)self + 0xA4) = 0;
    *(int*)((char*)self + 0xA8) = 0;
    *(int*)((char*)self + 0xC8) = 0;
    *(int*)((char*)self + 0xB0) = 0;
    *(int*)((char*)self + 0xAC) = 0;
    *(int*)((char*)self + 0xB8) = 0;
    *(int*)((char*)self + 0xC0) = 0;
    *(int*)((char*)self + 0xC4) = 0;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0xB4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275D90);
#ifdef SKIP_ASM
extern "C" void func_00276868(void* self, int a1);
void operator_delete(int* ptr);

extern "C" void func_00275D90(int* self, int flags)
{
    func_00276868(self, 1);
    *self = 0;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275DD8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
extern "C" void func_002776E0(void* self);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
int func_00283C58(void* self);
extern "C" int func_00283C80(void* list, void* cmd);
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");
struct sDmCmd5DD8 {
    int a1;         // 0x0
    int id;         // 0x4
    int a2;         // 0x8
    int field_0xC;  // 0xC
    int a4;         // 0x10
    int a5;         // 0x14
    int a6;         // 0x18
};

// Queues a plain command (no lookup) and starts it when the queue was idle.
extern "C" int func_00275DD8(char* self, int a1, int a2)
{
    if (func_00283C58(self + 0xC) != 0) {
        return 0;
    }
    sDmCmd5DD8 cmd;
    cmd.a1 = a1;
    cmd.id = -1;
    cmd.a2 = a2;
    cmd.field_0xC = -1;
    cmd.a4 = -1;
    cmd.a5 = -1;
    // 64-bit "none" value compared against the pending index below; needed for the target's branch layout
    long none = 0;
    cmd.a6 = -1;
    void* list = self + 0xC;
    func_00283C80(list, &cmd);
    if (*(int*)(self + 0xA4) == 0) {
        return 1;
    }
    if (*(int*)(self + 0xBC) >= none) {
        return 1;
    }
    *(int*)(self + 0xBC) = tNISList_SetupNextScript(self, func_00283D28_count(list) - 1, 0);
    if (*(int*)(self + 0xA4) == 3) {
        int ok = *(int*)((char*)cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, *(int*)((char*)func_00283D70(list, 0) + 0xC)) + 0x20) == 4;
        if (ok)
            func_002776E0(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275ED0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
extern "C" void func_002776E0(void* self);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int cSSXScriptEngine_SelectScriptFromCategory(void* eng, int a1, int a2, int a3, int a4);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int func_00283BB8(void* self);
int func_00283C58(void* self);
extern "C" int func_00283C80(void* list, void* cmd);
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

struct sDmCmd5ED0 {
    int a1;         // 0x0
    int id;         // 0x4
    int a2;         // 0x8
    int field_0xC;  // 0xC
    int a4;         // 0x10
    int a5;         // 0x14
    int a6;         // 0x18
};

extern "C" int func_00275ED0(char* self, int a1, int a2, int lookup, int a4, int a5, int a6)
{
    if (func_00283C58(self + 0xC) != 0) {
        return 0;
    }
    sDmCmd5ED0 cmd;
    cmd.a1 = a1;
    cmd.id = -1;
    cmd.a2 = a2;
    cmd.field_0xC = -1;
    cmd.a4 = a4;
    cmd.a5 = a5;
    cmd.a6 = a6;
    if (lookup != 0) {
        int id = cSSXScriptEngine_SelectScriptFromCategory(*(void**)self, a1, a4, a5, a6);
        if (id < 0) {
            return 0;
        }
        cmd.id = id;
    }
    void* list = self + 0xC;
    func_00283C80(list, &cmd);
    if (*(int*)(self + 0xA4) == 0) {
        return 1;
    }
    if (*(int*)(self + 0xBC) >= 0) {
        return 1;
    }
    *(int*)(self + 0xBC) = tNISList_SetupNextScript(self, func_00283D28_count(list) - 1, 0);
    if (*(int*)(self + 0xA4) == 3) {
        int ok;
        if (func_00277DD8(self, func_00283D70(list, 0)) == 0) {
            ok = *(int*)((char*)cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, *(int*)((char*)func_00283D70(list, 0) + 0xC)) + 0x20) == 4;
        } else {
            ok = func_00283BB8((char*)*(void**)self + 0x500);
        }
        if (ok) {
            func_002776E0(self);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276048);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
extern "C" void func_002776E0(void* self);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int func_00283BB8(void* self);
int func_00283C58(void* self);
extern "C" int func_00283C80(void* list, void* cmd);
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

struct sDmCmd6048 {
    int type;       // 0x0
    int a1;         // 0x4
    int a2;         // 0x8
    int id;         // 0xC
    int a3;         // 0x10
    int a4;         // 0x14
    int a5;         // 0x18
};

extern "C" int func_00276048(char* self, int a1, int a2, int a3, int a4, int a5)
{
    if (func_00283C58(self + 0xC) == 0) {
        void* list = self + 0xC;
        sDmCmd6048 cmd;
        cmd.type = 0x1C;
        cmd.id = -1;
        cmd.a2 = a2;
        cmd.a1 = a1;
        cmd.a3 = a3;
        cmd.a4 = a4;
        cmd.a5 = a5;
        func_00283C80(list, &cmd);
        if (*(int*)(self + 0xA4) != 0 && *(int*)(self + 0xBC) < 0) {
        *(int*)(self + 0xBC) = tNISList_SetupNextScript(self, func_00283D28_count(list) - 1, 0);
        if (*(int*)(self + 0xA4) == 3) {
            int ok;
            if (func_00277DD8(self, func_00283D70(list, 0)) == 0) {
                ok = *(int*)((char*)cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, *(int*)((char*)func_00283D70(list, 0) + 0xC)) + 0x20) == 4;
            } else {
                ok = func_00283BB8((char*)*(void**)self + 0x500);
            }
            if (ok) {
                func_002776E0(self);
            }
        }
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276198);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00283C80(void* list, void* cmd);
extern "C" int func_00283DC0(void* self, void* out);
extern "C" void func_00276868(void* self, int a1);
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

extern "C" int func_00276198(void* self, void* other)
{
    int cmd[8];
    int n = func_00283D28_count((char*)self + 0xC);
    if (*(int*)((char*)other + 0xA4) == 3) {
        return 0;
    }
    if (func_00283D28_count((char*)other + 0xC) != 0) {
        while (func_00283DC0((char*)other + 0xC, cmd) != 0) {
            func_00283C80((char*)self + 0xC, cmd);
        }
        func_00276868(other, 1);
        if (*(int*)((char*)self + 0xA4) != 0 && *(int*)((char*)self + 0xBC) < 0) {
            *(int*)((char*)self + 0xBC) = tNISList_SetupNextScript(self, n, 0);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276270);
#ifdef SKIP_ASM
extern "C" void func_00276868(void* self, int a1);
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
extern "C" void func_00277598(void* self, int index);
extern "C" void func_00277778(void* self);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int func_00282798(void* self, int a1);
extern "C" void* func_00283D28(void*);
extern "C" void* func_00283D70(void* list, int i);

extern "C" int func_00276270(char* self, int playOnReady)
{
    if (*(int*)(self + 0xA4) == 1) {
        func_00277778(self);
    }
    if (*(int*)(self + 0xA4) != 0) {
        return *(int*)(self + 0xA4);
    }
    *(int*)(self + 0xB4) = playOnReady;
    void* list = self + 0xC;
    if (func_00283D28(list) == 0) {
        return 0;
    }
    int idx = tNISList_SetupNextScript(self, 0, 0);
    if (idx < 0) {
        func_00276868(self, 1);
        return *(int*)(self + 0xA4);
    }
    func_00277598(self, idx);
    char* entry = (char*)func_00283D70(list, 0);
    if (func_00277DD8(self, entry) == 0) {
        if (func_00282798(*(void**)self, *(int*)(entry + 0xC)) == 1) {
            *(int*)(self + 0xA4) = 1;
        } else {
            *(int*)(self + 0xA4) = 2;
        }
    } else {
        if (*(int*)(entry + 0xC) < 0) {
            *(int*)(self + 0xA4) = 1;
        } else {
            *(int*)(self + 0xA4) = 2;
        }
    }
    *(int*)(self + 0xBC) = tNISList_SetupNextScript(self, idx + 1, 0);
    return *(int*)(self + 0xA4);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_Play);
#ifdef SKIP_ASM
extern char str_hud_e[];
int GetHashValue32(char* s);
void* cGame_getTransitionMan(void* self, int a1);
char* func_0027C070(void* obj);
extern "C" void func_00277778(void* self);
extern "C" int func_00277DD8(void* self, void* a1);
// The unit declares cSSXScriptEngine_PlayRequestScript as void; it returns func_002822A0's result.
int cSSXScriptEngine_PlayRequestScript_i(void* self, int val, int key, int b, int c) __asm__("cSSXScriptEngine_PlayRequestScript");
extern "C" int cSSXScriptEngine_MakeTransitionObject(void* self, int a1, int a2);
extern "C" void func_002826D8(void* self, int a1);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int cVidEngine_PlayVideo(void* self);
extern "C" void cVidEngine_UpdateVideo(void* self);
extern "C" void* func_00283D70(void* list, int i);
extern "C" void cTransitionMan_StartTransition(void* snd, int a1, int h, int h2, int a4, int a5, float t0, float t1, float t2);
// PORT: real parameter order of cTransitionMan_StartTransition has the floats before the two trailing ints (cf. boardwakefx).
void cTransitionMan_StartTransition_f(void* snd, int a1, int h, int h2, float t0, float t1, float t2, int a4, int a5) __asm__("cTransitionMan_StartTransition");
extern "C" void cTransitionMan_StartTransitionScreenGrab(void* snd, int a1, int h, int a3, float t);
extern "C" void func_002E4578(void* p);
extern "C" void cCutSceneTransitionMan_StartCutScene(void* gm, float t);
extern "C" void cCutSceneTransitionMan_ForceToCutSceneMode(void* gm);
extern "C" void* cUIStateStack_findState(void* table, int hash);

struct sDmVEntry_6388 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

struct sDmVEntry2_6388 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sDm_6388 {
    void* eng;              // 0x0
    int active;             // 0x4
    int val;                // 0x8
    char list[0xA4 - 0xC];  // 0xC
    int state;              // 0xA4
    int fA8;
    int fAC;                // 0xAC
    int fB0;                // 0xB0
    int fB4;
    int fB8;                // 0xB8
};

struct sDmFade_6388 {
    char pad_0x0[0x8];
    signed char cat;        // 0x8
    signed char key;        // 0x9
    short tIn;              // 0xA
    short tHold;            // 0xC
    short tOut;             // 0xE
};

struct sDmItem_6388 {
    int f0;
    int f4;
    int flags;              // 0x8
    int id;                 // 0xC
};

// The unit declares tNISList_Play as void; it returns the state at 0xA4.
int tNISList_Play_impl(char* self, int fade, int notify) __asm__("tNISList_Play");

/**
 * Starts the NIS list's head script (or video) once it is ready, with its transition in, and returns the list state.
 *
 * Polls a loading list first (state 1: func_00277778, On Tour CheckOnLoading) and does nothing unless the state (+0xA4)
 * is 2 (ready). It sets the state to 3 (playing) and calls virtual slot 24 of the "hud" UI state
 * (cUIStateStack_findState) with (0, 0) when that state is up.
 *
 * A script entry (func_00277DD8, On Tour IsVideo, returns 0) is started with cSSXScriptEngine_PlayRequestScript (the
 * last flag is bit 3 of the entry's flags, +0x8). On the cut-scene list (+0x4) the running transition is ended
 * (func_002E4578), and the script header's in/hold/out frame counts (func_0027C070 of the request's player; 1/60 s
 * each) build the transition in: type 7 is a screen grab (cTransitionMan_StartTransitionScreenGrab), any other type a full transition
 * (cTransitionMan_StartTransition with two cSSXScriptEngine_MakeTransitionObject objects). When in+hold frames remain (+0xB8) the
 * request is paused (func_002826D8, On Tour PauseScript), +0xAC and +0xB0 (On Tour mIsStartTransition,
 * mInScriptTransition) are set and the forced update is skipped; then the cut scene starts (cCutSceneTransitionMan_StartCutScene, On Tour
 * cCutSceneTransitionMan::StartCutScene, over the transition-in time or 0.5 s). With the forced update the engine's
 * virtual slot 6 (On Tour ForcePlayUpdate) runs the request once.
 *
 * A video entry forces cut-scene mode on the cut-scene list (cCutSceneTransitionMan_ForceToCutSceneMode), starts the video engine (cVidEngine_PlayVideo on
 * engine+0x500) and, with the forced update, updates it once (cVidEngine_UpdateVideo).
 *
 * @param fade  start with the transition in (On Tour startWithTransitionIn); 0 drops its in and hold frames
 * @param notify  force an immediate update of the started script or video (On Tour forceUpdate)
 * @return the list state after the call (+0xA4)
 */
int tNISList_Play_impl(char* self, int fade, int notify)
{
    sDm_6388* dm = (sDm_6388*)self;
    if (*(int*)(self + 0xA4) == 1) {
        func_00277778(self);
    }
    if (*(int*)(self + 0xA4) != 2) {
        return *(int*)(self + 0xA4);
    }
    sDmItem_6388* item = (sDmItem_6388*)func_00283D70(self + 0xC, 0);
    dm->state = 3;
    char* table = (char*)SSXApp->Game->UI + 0x18;
    char* hudState = (char*)cUIStateStack_findState(table, GetHashValue32(str_hud_e));
    if (hudState != 0) {
        sDmVEntry_6388* vtEntry = &(*(sDmVEntry_6388**)(hudState + 0x8))[24];
        vtEntry->fn(hudState + vtEntry->delta, 0, 0);
    }
    if (func_00277DD8(self, item) == 0) {
        cSSXScriptEngine_PlayRequestScript_i(*(void**)self, *(int*)(self + 0x8), item->id, 0, (item->flags >> 3) & 1);
        if (*(int*)(self + 0x4) != 0) {
            void* snd = cGame_getTransitionMan(SSXApp->Game, 0);
            if (*(int*)((char*)snd + 0x44) != 0) {
                func_002E4578(snd);
            }
            int tIn = 0;
            int tHold = 0;
            float cutSceneTime = 0.0f;
            sDmFade_6388* transitionInfo = (sDmFade_6388*)func_0027C070(cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, item->id));
            if (fade) {
                tIn = transitionInfo->tIn;
                tHold = transitionInfo->tHold;
            }
            *(int*)(self + 0xB8) = 0;
            if (tIn + tHold + transitionInfo->tOut > 0) {
                if (transitionInfo->cat != 7) {
                    int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                    if (transition != 0) {
                        int transitionOut = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                        float tt = (float)tIn * 0.01666666753590107f;
                        cTransitionMan_StartTransition_f(snd, 2, transition, transitionOut, tt, (float)tHold * 0.01666666753590107f, (float)transitionInfo->tOut * 0.01666666753590107f, 0, 0);
                        cutSceneTime = tt;
                        *(int*)(self + 0xB8) = tIn + tHold;
                    }
                } else if (tIn) {
                    int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, transitionInfo->key);
                    float tt = (float)tIn * 0.01666666753590107f;
                    cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, tt);
                    cutSceneTime = tt;
                }
                if (*(int*)(self + 0xB8) > 0) {
                    notify = 0;
                    func_002826D8(*(void**)self, item->id);
                    *(int*)(self + 0xAC) = 1;
                    *(int*)(self + 0xB0) = 1;
                }
            }
            if (cutSceneTime <= 0.0f) {
                cutSceneTime = 0.5f;
            }
            cCutSceneTransitionMan_StartCutScene(SSXApp->Game->CutSceneTransitionMan, cutSceneTime);
        }
        if (notify) {
            char* eng = *(char**)self;
            sDmVEntry2_6388* vtEntry = &(*(sDmVEntry2_6388**)(eng + 0x2A8))[6];
            vtEntry->fn(eng + vtEntry->delta, item->id);
        }
    } else {
        if (*(int*)(self + 0x4) != 0) {
            cCutSceneTransitionMan_ForceToCutSceneMode(SSXApp->Game->CutSceneTransitionMan);
        }
        cVidEngine_PlayVideo(*(char**)self + 0x500);
        if (notify) {
            cVidEngine_UpdateVideo(*(char**)self + 0x500);
        }
    }
    return *(int*)(self + 0xA4);
}
#endif

extern "C" void* func_00277C08(void*, int, int);

//100%
INCLUDE_ASM("scripter/datamanager", func_002766B0__FPvi);
#ifdef SKIP_ASM
void* func_002766B0(void* self, int a1)
{
    return func_00277C08(self, a1, 0);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_002766D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00276270(char* self, int a1);
extern "C" void func_00276868(void* self, int a1);
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
extern "C" void func_00277598(void* self, int index);
extern "C" void func_00277800(void* self);
// The unit declares tNISList_DoTransitionToNext as void; it returns whether it handled the update.
extern "C" int tNISList_DoTransitionToNext_r(void* self) __asm__("tNISList_DoTransitionToNext");
extern "C" void* func_00277C08(void*, int, int);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int func_00282540(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

struct sSeqItem_66D0 {
    char pad_0x0[0xC];
    int handle;         // 0xC
};

struct sSeq_66D0 {
    char* owner;        // 0x0
    char pad_0x4[0x8];
    char list[0x98];    // 0xC
    int state;          // 0xA4
    int fA8;            // 0xA8
    char pad_0xAC[0x8];
    int fB4;            // 0xB4
    char pad_0xB8[0x4];
    int cur;            // 0xBC
    int pos;            // 0xC0
};

extern "C" int func_002766D0(sSeq_66D0* self, int delta, int force)
{
    int pos = delta + self->pos;
    if (pos == self->pos) {
        return self->state;
    }
    if (pos < self->cur) {
        pos = self->cur;
    }
    int n = func_00283D28_count(self->list);
    if (self->state != 3) {
        self->pos = 0;
        if (pos < n) {
            func_00277598(self, pos);
            int st = self->state;
            self->cur = -1;
            self->state = 0;
            if (st != 0) {
                func_00276270((char*)self, self->fB4);
            }
            return self->state;
        }
        goto reset;
    }
    self->pos = pos;
    if (self->cur > 0 && self->cur < pos) {
        sSeqItem_66D0* it = (sSeqItem_66D0*)func_00283D70(self->list, self->cur);
        if (func_00277DD8(self, it) == 0) {
            func_00282540(self->owner, it->handle);
        } else if (it->handle != 0) {
            cVidEngine_ResetVideoEngine(self->owner + 0x500);
        }
        it->handle = -1;
        self->cur = -1;
        if (self->fA8) {
            func_00277C08(self, 0, 1);
        }
        self->cur = tNISList_SetupNextScript(self, pos, 0);
    }
    if (self->fA8 == 0) {
        if (force == 0 || tNISList_DoTransitionToNext_r(self) == 0) {
            if (self->cur < 0) {
                goto reset;
            }
            func_00277800(self);
        }
    }
    return self->state;
reset:
    func_00276868(self, 1);
    return self->state;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276868);
#ifdef SKIP_ASM
extern char str_hud_e[];
int GetHashValue32(char* s);
void* cGame_getTransitionMan(void* self, int a1);
extern "C" void func_002E4578(void* p);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void* p);
extern "C" void* cUIStateStack_findState(void* table, int hash);
extern "C" void func_00277598(void* self, int index);
extern "C" void* func_00277C08(void*, int, int);

struct sDmVEntry6868 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" void func_00276868(void* nisList, int resetTransition)
{
    char* self = (char*)nisList;
    if (*(int*)(self + 0xA8) != 0) {
        func_00277C08(self, 0, 1);
    }
    if (*(int*)(self + 0xC4) != 0) {
        *(int*)(self + 0xC4) = 1;
        func_00277C08(self, 0, 0);
    }
    if (*(int*)(self + 0x4) != 0 && *(int*)(self + 0xA4) == 3) {
        void* gm = SSXApp->Game->CutSceneTransitionMan;
        if (resetTransition != 0) {
            cCutSceneTransitionMan_ForceToRegularMode(gm);
            if (*(int*)(self + 0xB0) != 0) {
                func_002E4578(cGame_getTransitionMan(SSXApp->Game, 0));
            }
        }
        char* table = (char*)SSXApp->Game->UI + 0x18;
        char* hudState = (char*)cUIStateStack_findState(table, GetHashValue32(str_hud_e));
        if (hudState != 0) {
            sDmVEntry6868* vtEntry = &(*(sDmVEntry6868**)(hudState + 0x8))[24];
            vtEntry->fn(hudState + vtEntry->delta, 1, 0);
        }
    }
    func_00277598(self, 5);
    *(int*)(self + 0xBC) = -1;
    *(int*)(self + 0xA4) = 0;
    *(int*)(self + 0xC8) = 0;
    *(int*)(self + 0xB0) = 0;
    *(int*)(self + 0xB8) = 0;
    *(int*)(self + 0xAC) = 0;
    *(int*)(self + 0xC0) = 0;
    *(int*)(self + 0xC4) = 0;
    *(int*)(self + 0xB4) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276998);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

extern "C" int func_00276998(void* self, int i)
{
    void* list = (char*)self + 0xC;
    if (i < func_00283D28_count(list) && *(int*)func_00283D70(list, i) < 0x1C) {
        return *(int*)func_00283D70(list, i);
    }
    return 0x1C;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276A08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

extern "C" int func_00276A08(void* self, int i)
{
    void* list = (char*)self + 0xC;
    if (i < func_00283D28_count(list) && *(int*)func_00283D70(list, i) >= 29) {
        return *(int*)func_00283D70(list, i);
    }
    return 0x20;
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", func_00276A78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_002827F8(void* self, int id);
int func_00283D28_count(void* list) __asm__("func_00283D28");

extern "C" int func_00276A78(void* self, int i)
{
    void* list = (char*)self + 0xC;
    if (i >= func_00283D28_count(list)) {
        return -1;
    }
    void* item = func_00283D70(list, i);
    if (func_00277DD8(self, item) != 0) {
        return -1;
    }
    int scriptIndex = *(int*)((char*)item + 0x4);
    if (scriptIndex < 0) {
        int id = *(int*)((char*)item + 0xC);
        scriptIndex = -1;
        if (id >= 0) {
            scriptIndex = func_002827F8(*(void**)self, id);
        }
    }
    return scriptIndex;
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", func_00276B08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

extern "C" void* func_00276B08(void* self, int i)
{
    void* list = (char*)self + 0xC;
    if (i < func_00283D28_count(list)) {
        void* item = func_00283D70(list, i);
        if (func_00277DD8(self, item) == 0) {
            int id = *(int*)((char*)item + 0xC);
            if (id >= 0) {
                return cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, id);
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetState);
#ifdef SKIP_ASM
extern "C" void func_00277778(void* self);

/**
 * Returns the NIS list's state, first polling the load while it is still loading.
 *
 * In state 1 (loading) it calls func_00277778 (On Tour CheckOnLoading), which may advance the state. The state word is
 * +0xA4 (On Tour mState; tNISList_Play treats 2 as ready and 3 as playing).
 *
 * @return the list state (+0xA4)
 */
extern "C" int tNISList_GetState(void* self)
{
    if (*(int*)((char*)self + 0xA4) == 1) {
        func_00277778(self);
    }
    return *(int*)((char*)self + 0xA4);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetPlayState);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00282838(void* self, int a1);

/**
 * Classifies a NIS list's playback for the script engine (On Tour eNISListStatePlay).
 *
 * Returns 0 unless the list is playing (+0xA4 == 3) and 3 while it is paused (+0xC4, On Tour mNumPause). Unless a skip
 * is in progress (+0xC8, On Tour mDoSkip), a head entry with flag bit 3 (+0x8) returns 2 while a script transition is
 * running (+0xB0) with more than one wait frame left (+0xB8), and any other head entry returns 1 while func_00282838
 * (On Tour IsRequestPlaybackEnding) returns 0 for its request (+0xC). Otherwise it returns 4, or 5 when no next script
 * is pending (+0xBC < 0, On Tour mNextPendingScriptIndex).
 *
 * @return the play state, 0..5
 */
extern "C" int tNISList_GetPlayState(void* self)
{
    if (*(int*)((char*)self + 0xA4) != 3) return 0;
    if (*(int*)((char*)self + 0xC4) != 0) return 3;
    if (*(int*)((char*)self + 0xC8) == 0) {
        void* list = (char*)self + 0xC;
        if (*(int*)((char*)func_00283D70(list, 0) + 0x8) & 8) {
            if (*(int*)((char*)self + 0xB0) == 0) return 2;
            if (*(int*)((char*)self + 0xB8) - 1 > 0) return 2;
        } else {
            if (func_00282838(*(void**)self, *(int*)((char*)func_00283D70(list, 0) + 0xC)) == 0) return 1;
        }
    }
    return *(int*)((char*)self + 0xBC) < 0 ? 5 : 4;
}
#endif

extern "C" void* func_00283D28(void*);

//100%
INCLUDE_ASM("scripter/datamanager", func_00276CA8__FPv);
#ifdef SKIP_ASM
void* func_00276CA8(void* self)
{
    return func_00283D28((char*)self + 0xc);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", func_00276CC8);
#ifdef SKIP_ASM
void* cGame_getTransitionMan(void* self, int a1);
extern "C" void tNISList_Play(void* self, int a1, int a2);
extern "C" void func_00277778(void* self);
extern "C" void func_00277800(void* self);
extern "C" void func_00277838(void* self);
extern "C" void tNISList_DoTransitionToNext(void* self);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" void func_00282720(void* self, int a1);
extern "C" int func_00282798(void* self, int a1);
extern "C" int func_00282838(void* self, int a1);
extern "C" int cVidEngine_PlayVideo(void* self);
extern "C" int func_00283BB8(void* self);
extern "C" void* func_00283D70(void* list, int i);
extern "C" void func_002E4578(void* p);

extern "C" void func_00276CC8(char* self)
{
    if (*(int*)(self + 0xA4) == 1) {
        func_00277778(self);
        return;
    }
    if (*(int*)(self + 0xA4) == 2) {
        int playOnReady = *(int*)(self + 0xB4);
        if (playOnReady != 0) {
            tNISList_Play(self, playOnReady == 1, 0);
            *(int*)(self + 0xB4) = 0;
        }
    }
    if (*(int*)(self + 0xA4) != 3) {
        return;
    }
    if (*(int*)(self + 0xC4) != 0) {
        return;
    }
    if (*(int*)(self + 0xB0) != 0) {
        char* item = (char*)func_00283D70(self + 0xC, 0);
        char* obj = (char*)cGame_getTransitionMan(SSXApp->Game, 0);
        if (*(int*)(self + 0xB8) != 0) {
            if (--*(int*)(self + 0xB8) == 0) {
                if (*(int*)(self + 0xAC) == 0) {
                    func_00277800(self);
                    return;
                }
                if (func_00277DD8(self, item) == 0) {
                    func_00282720(*(void**)self, *(int*)(item + 0xC));
                } else {
                    cVidEngine_PlayVideo(*(char**)self + 0x500);
                }
            }
        }
        if (*(int*)(obj + 0x44) == 0 && *(int*)(self + 0xB8) == 0) {
            *(int*)(self + 0xAC) = 0;
            *(int*)(self + 0xB0) = 0;
        }
    }
    if (*(int*)(self + 0xA8) != 0) {
        void* list = self + 0xC;
        char* item = (char*)func_00283D70(list, *(int*)(self + 0xBC));
        int ok;
        if (func_00277DD8(self, item) == 0) {
            ok = func_00282798(*(void**)self, *(int*)((char*)func_00283D70(list, *(int*)(self + 0xBC)) + 0xC)) == 2;
        } else {
            ok = *(int*)(item + 0xC) >= 0;
        }
        if (ok) {
            func_00277C08(self, 0, 1);
            func_00277838(self);
        }
        return;
    }
    if (*(int*)(self + 0xC8) != 0) {
        if (--*(int*)(self + 0xC8) == 0) {
            if (*(int*)(self + 0xB0) != 0) {
                func_002E4578(cGame_getTransitionMan(SSXApp->Game, 0));
                *(int*)(self + 0xAC) = 0;
                *(int*)(self + 0xB0) = 0;
                *(int*)(self + 0xB8) = 0;
            }
            func_00277800(self);
            return;
        }
    }
    char* item = (char*)func_00283D70(self + 0xC, 0);
    int done = 0;
    if (func_00277DD8(self, item) == 0) {
        if (func_00282798(*(void**)self, *(int*)(item + 0xC)) == 2 || func_00282838(*(void**)self, *(int*)(item + 0xC)) != 0) {
            done = 1;
        }
    } else if (func_00283BB8(*(char**)self + 0x500) != 0) {
        tNISList_DoTransitionToNext(self);
        done = 1;
    }
    if (done) {
        func_00277800(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276F48);
#ifdef SKIP_ASM
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
extern "C" void func_00277800(void* self);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int func_00282540(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" void* func_00283D70(void* list, int i);

extern "C" void func_00276F48(char* self)
{
    if (*(int*)(self + 0xA4) != 3) {
        return;
    }
    if (*(int*)(self + 0xC8) != 0) {
        return;
    }
    void* list = self + 0xC;
    char* entry = (char*)func_00283D70(list, 0);
    if (!(*(int*)(entry + 0x8) & 1)) {
        return;
    }
    *(int*)(self + 0xC8) = 1;
    if (*(int*)(self + 0xBC) < 0) {
        tNISList_SetupNextScript(self, 1, 1);
        return;
    }
    int idx = tNISList_SetupNextScript(self, *(int*)(self + 0xBC), 1);
    if (idx != *(int*)(self + 0xBC)) {
        char* pendingEntry = (char*)func_00283D70(list, *(int*)(self + 0xBC));
        if (func_00277DD8(self, pendingEntry) == 0) {
            func_00282540(*(void**)self, *(int*)(pendingEntry + 0xC));
            *(int*)(pendingEntry + 0xC) = -1;
        } else if (*(int*)(pendingEntry + 0xC) >= 0) {
            cVidEngine_ResetVideoEngine(*(char**)self + 0x500);
        }
        *(int*)(self + 0xBC) = idx;
    }
    if (idx >= 0) {
        func_00277800(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsSkipAllowed);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);

/**
 * Says whether the playing NIS list may be skipped now.
 *
 * True only while playing (+0xA4 == 3) when the head entry's flag bit 0 (+0x8) is set and the list is not paused
 * waiting for its next script (+0xA8, On Tour mPausedOnNextScriptLoading). cSSXScriptEngine_Update checks it before
 * honouring the skip input, and cSSXScriptEngine_IsActiveNISSkippable reports it.
 *
 * @return 1 if the list may be skipped, else 0
 */
extern "C" int tNISList_IsSkipAllowed(void* self)
{
    if (*(int*)((char*)self + 0xA4) != 3) {
        return 0;
    }
    int allowed = 0;
    void* headEntry = func_00283D70((char*)self + 0xC, 0);
    if (*(int*)((char*)headEntry + 0x8) & 1) {
        allowed = *(int*)((char*)self + 0xA8) == 0;
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_002770C0);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);

extern "C" int func_002770C0(void* self)
{
    if (*(int*)((char*)self + 0xA4) != 3) {
        return 1;
    }
    void* entry = func_00283D70((char*)self + 0xC, 0);
    return (*(int*)((char*)entry + 0x8) >> 2) & 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277100);
#ifdef SKIP_ASM
extern "C" int func_00277100(void* self)
{
    if (*(int*)((char*)self + 0xA4) == 1) {
        return 0;
    }
    int idx = *(int*)((char*)self + 0xBC);
    void* list = (char*)self + 0xC;
    if (idx < 0) {
        return 1;
    }
    void* item = func_00283D70(list, idx);
    if (func_00277DD8(self, item) != 0) {
        return *(int*)((char*)item + 0xC) >= 0;
    }
    return func_00282798(*(void**)self, *(int*)((char*)func_00283D70(list, *(int*)((char*)self + 0xBC)) + 0xC)) == 2;
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", func_002771A8);
#ifdef SKIP_ASM
extern "C" void func_00283C58__FPv(int);

extern "C" void func_002771A8(int nisList) {
    func_00283C58__FPv(nisList + 0xC);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_002771C8);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);

extern "C" int func_002771C8(void* self, void* obj)
{
    if (*(int*)((char*)self + 0xA4) != 3) {
        return 0;
    }
    void* headEntry = func_00283D70((char*)self + 0xC, 0);
    if (func_00277DD8(self, headEntry) != 0) {
        return 0;
    }
    return obj == cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, *(int*)((char*)headEntry + 0xC));
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277248);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00277DD8(void* self, void* a1);

extern "C" int func_00277248(char* self)
{
    if (*(int*)(self + 0xA4) != 3) {
        return 0;
    }
    return func_00277DD8(self, func_00283D70(self + 0xC, 0));
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPausedPendingNextScript);
#ifdef SKIP_ASM
/**
 * Says whether the playing NIS list is waiting for its next script to finish loading.
 *
 * True while playing (+0xA4 == 3) with the paused-on-loading flag (+0xA8, On Tour mPausedOnNextScriptLoading) set.
 * cSSXScriptEngine_Update shows the load indicator (cBXLoadIndicator_Show) while it is, if the "hud" UI state is up.
 *
 * @return 1 if paused pending the next script, else 0
 */
extern "C" int tNISList_IsPausedPendingNextScript(void* self)
{
    int paused = 0;
    if (*(int*)((char*)self + 0xa4) == 3) {
        paused = *(int*)((char*)self + 0xa8) != 0;
    }
    return paused;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsInTransition__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the NIS list is between scripts: returns its transition flag at +0xB0.
 *
 * @return the transition flag (nonzero during a transition)
 */
int tNISList_IsInTransition(void* self)
{
    return *(int*)((char*)self + 0xB0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptPlay);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
char* func_0027C070(void* obj);
extern "C" void cRenderStateMan_setCustomIrradiance(int a0);
extern char str_TRANSP[];

/**
 * Reacts to a script starting to play: for a script this NIS list owns, switches the custom irradiance.
 *
 * When func_002771C8 (On Tour IsPlayingScriptOwner) accepts the player and the script header from func_0027C070
 * (GetSSXScriptHeader) starts with a non-zero byte, calls cRenderStateMan_setCustomIrradiance (On Tour
 * cRenderStateMan::setCustomIrradiance) with "TRANSP".
 *
 * @param requestHandle   the play request handle (On Tour requestHandle); unused
 * @param obj  the script player (On Tour cScriptPlayerMain*)
 */
extern "C" void tNISList_OnScriptPlay(void* self, int requestHandle, void* obj)
{
    if (func_002771C8(self, obj) != 0) {
        char* name = func_0027C070(obj);
        char* msg = str_TRANSP;
        if (*name != 0) {
            cRenderStateMan_setCustomIrradiance((int)msg); // PORT: string pointer passed as int (unit declares cRenderStateMan_setCustomIrradiance(int))
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptUpdate);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
char* func_0027C070(void* obj);
extern "C" int cScriptPlayerMain_GetNumUpdatesToEndLeft(void* obj);
extern "C" void tNISList_DoTransitionToNext(void* self);

/**
 * Per-frame script callback: starts the transition to the next NIS once the playing script nears its end.
 *
 * Only for the list's own script player (func_002771C8, On Tour IsPlayingScriptOwner) on the cut-scene list (+0x4) and
 * while the player's +0x1C word is 0: when the updates left (cScriptPlayerMain_GetNumUpdatesToEndLeft() + 1) drop to
 * the script header's next-transition in-frame count (short at +0x12 of func_0027C070's header), calls tNISList_DoTransitionToNext
 * (On Tour DoTransitionToNext). The current frame and frame delta that On Tour passes are not declared (unused).
 *
 * @param requestHandle  the request handle (unused)
 * @param obj  the cScriptPlayerMain being updated
 */
extern "C" void tNISList_OnScriptUpdate(void* self, int requestHandle, void* obj)
{
    if (func_002771C8(self, obj) != 0 && *(int*)((char*)self + 0x4) != 0 &&
        *(int*)((char*)obj + 0x1C) == 0) {
        char* name = func_0027C070(obj);
        if (cScriptPlayerMain_GetNumUpdatesToEndLeft(obj) + 1 <= *(short*)(name + 0x12)) {
            tNISList_DoTransitionToNext(self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", func_002773A0);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
extern "C" void func_002776E0(void* self);
extern "C" void func_002826D8(void* self, int a1);

extern "C" void func_002773A0(void* self, int requestHandle, void* obj)
{
    if (func_002771C8(self, obj) != 0) {
        if (*(int*)((char*)self + 0xB8) != 0) {
            func_002826D8(*(void**)self, requestHandle);
        }
        func_002776E0(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptStop);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
char* func_0027C070(void* obj);
extern "C" void cRenderStateMan_setCustomIrradiance(int a0);

/**
 * Script stop callback: drops the custom irradiance the list's script had set.
 *
 * For the list's own script player (func_002771C8, On Tour IsPlayingScriptOwner), when the script header
 * (func_0027C070) starts with a non-empty name, calls cRenderStateMan_setCustomIrradiance(0) (On Tour cRenderStateMan::setCustomIrradiance)
 * to clear it.
 *
 * @param requestHandle  the request handle (unused)
 * @param obj  the cScriptPlayerMain that stopped
 */
extern "C" void tNISList_OnScriptStop(void* self, int requestHandle, void* obj)
{
    if (func_002771C8(self, obj) != 0) {
        if (*func_0027C070(obj) != 0) {
            cRenderStateMan_setCustomIrradiance(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_SetupNextScript);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int cSSXScriptEngine_RequestScriptByCategory(void* self, int category, int a, int b, int c);
extern "C" int cSSXScriptEngine_RequestScriptByIndex(void* self, int key, int a, int b, int c);
extern "C" int cVidEngine_ReadyVideo(void** self, int file);
extern "C" void* func_00283D70(void* list, int i);
extern "C" int* func_00277DE8_p(void* self, void* ent) __asm__("func_00277DE8");
int func_00283D28_count(void* list) __asm__("func_00283D28");

struct sDmEnt7450 {
    int f0;     // 0x0
    int f4;     // 0x4
    int flags;  // 0x8
    int id;     // 0xC
    int a;      // 0x10
    int b;      // 0x14
    int c;      // 0x18
};

/**
 * Finds the next entry of the NIS list that can play, requesting its script or readying its video.
 *
 * Starting at @p index, an entry already requested is returned at once; a script entry is requested by index
 * (or by category when it has none) and returned when the request succeeds; a video entry readies its video
 * when no video player exists yet and is returned.
 *
 * @param vself  the NIS list (tNISList)
 * @param index  entry to start searching at (On Tour startSearchIndex)
 * @param skipFlagged  nonzero to pass over entries with flag 2 (On Tour ignoreSkipInheritScripts)
 * @return the index of the entry set up, or -1 when none can be
 */
extern "C" int tNISList_SetupNextScript(void* vself, int index, int skipFlagged)
{
    char* self = (char*)vself;
    int count = func_00283D28_count(self + 0xC);
    int i = index;
    for (; i < count; i++) {
        sDmEnt7450* currEntry = (sDmEnt7450*)func_00283D70(self + 0xC, i);
        if (skipFlagged && (currEntry->flags & 2))
            continue;
        if (currEntry->id >= 0)
            return i;
        if (func_00277DD8(self, currEntry) == 0) {
            void* eng = *(void**)self;
            if (currEntry->f4 >= 0)
                currEntry->id = cSSXScriptEngine_RequestScriptByIndex(eng, currEntry->f4, currEntry->a, currEntry->b, currEntry->c);
            else
                currEntry->id = cSSXScriptEngine_RequestScriptByCategory(*(void**)self, currEntry->f0, currEntry->a, currEntry->b, currEntry->c);
            if (currEntry->id >= 0)
                return i;
        } else {
            if (*(int*)(*(char**)self + 0x500) == 0) {
                int* videoEntry = func_00277DE8_p(self, currEntry);
                if (cVidEngine_ReadyVideo((void**)(*(char**)self + 0x500), *videoEntry) != 0)
                    currEntry->id = 0;
            }
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277598);
#ifdef SKIP_ASM
struct sDmEnt7DE8;
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" sDmEnt7DE8* func_00277DE8(void* self, int* key);
extern "C" int func_00282540(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" int func_00283BB8(void* self);
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00283DA0(void* self);
extern "C" int func_00283DC0(void* self, void* out);
extern "C" void* cAudioInterface_bxAudio();

struct sDmCmd7598 {
    int category;   // 0x0
    int key;        // 0x4
    int flags;      // 0x8
    int id;         // 0xC
    int a;          // 0x10
    int b;          // 0x14
    int c;          // 0x18
};

struct sDmSndVEntry7598 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00277598(void* selfp, int n)
{
    char* self = (char*)selfp;
    while (n != 0) {
        if (func_00283DA0(self + 0xC) != 0) {
            return;
        }
        sDmCmd7598 cmd = *(sDmCmd7598*)func_00283D70(self + 0xC, 0);
        if (cmd.id >= 0) {
            if (func_00277DD8(self, &cmd) == 0) {
                func_00282540(*(void**)self, cmd.id);
            } else {
                if (*(int*)(*(char**)self + 0x504) != 0) {
                    int* ent = (int*)func_00277DE8(self, (int*)&cmd);
                    int snd;
                    if (func_00283BB8(*(char**)self + 0x500) != 0) {
                        snd = ent[6];
                    } else {
                        snd = ent[5];
                    }
                    if (snd >= 0) {
                        char* soundMan = (char*)cAudioInterface_bxAudio();
                        sDmSndVEntry7598* vt = *(sDmSndVEntry7598**)(soundMan + 0x5558);
                        char* sys = soundMan + 0x118;
                        vt[4].fn(sys + vt[4].delta, snd);
                    }
                }
                cVidEngine_ResetVideoEngine(*(char**)self + 0x500);
            }
        }
        func_00283DC0(self + 0xC, 0);
        n--;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_002776E0);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int func_00282798(void* self, int a1);

extern "C" void func_002776E0(void* self)
{
    int idx = *(int*)((char*)self + 0xBC);
    if (idx >= 0) {
        void* list = (char*)self + 0xC;
        char* nextEntry = (char*)func_00283D70(list, idx);
        int ok;
        if (func_00277DD8(self, nextEntry) == 0) {
            ok = func_00282798(*(void**)self,
                               *(int*)((char*)func_00283D70(list, *(int*)((char*)self + 0xBC)) + 0xC)) == 1;
        } else {
            ok = *(int*)(nextEntry + 0xC) < 0;
        }
        if (ok) {
            func_00277C08(self, 1, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277778);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int func_00282798(void* self, int a1);

extern "C" void func_00277778(void* self)
{
    if (*(int*)((char*)self + 0xA4) == 1) {
        char* headEntry = (char*)func_00283D70((char*)self + 0xC, 0);
        if (func_00277DD8(self, headEntry) == 0) {
            int requestState = func_00282798(*(void**)self, *(int*)(headEntry + 0xC));
            if (requestState == 2) {
                *(int*)((char*)self + 0xA4) = requestState;
            }
        } else {
            *(int*)((char*)self + 0xA4) = 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277800);
#ifdef SKIP_ASM
extern "C" void func_002776E0(void* self);
extern "C" void func_00277838(void* self);

extern "C" void func_00277800(void* self)
{
    func_002776E0(self);
    if (*(int*)((char*)self + 0xA8) == 0) {
        func_00277838(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277838);
#ifdef SKIP_ASM
void* cGame_getTransitionMan(void* self, int a1);
extern "C" void func_00276868(void* self, int a1);
extern "C" int tNISList_SetupNextScript(void* self, int index, int a2);
extern "C" void func_00277598(void* self, int index);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" void cSSXScriptEngine_PlayRequestScript(void* self, int val, int key, int b, int c);
extern "C" int cVidEngine_PlayVideo(void* self);
extern "C" void* func_00283D70(void* list, int i);
extern "C" float func_002E4678(void* self);
extern "C" void func_002EA780(void* self, float t);

struct sDmCmd7838 {
    int category;   // 0x0
    int key;        // 0x4
    int flags;      // 0x8
    int id;         // 0xC
};

struct sDmEngVEntry7838 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

extern "C" void func_00277838(void* selfp)
{
    char* self = (char*)selfp;
    if (*(int*)(self + 0xBC) > 0) {
        *(int*)(self + 0xC8) = 0;
        func_00277598(self, *(int*)(self + 0xBC));
        *(int*)(self + 0xC0) = 0;
        *(int*)(self + 0xBC) = -1;
        sDmCmd7838* cmd = (sDmCmd7838*)func_00283D70(self + 0xC, 0);
        if (func_00277DD8(self, cmd) == 0) {
            cSSXScriptEngine_PlayRequestScript(*(void**)self, *(int*)(self + 0x8), cmd->id, 0, (cmd->flags >> 3) & 1);
            char* eng = *(char**)self;
            sDmEngVEntry7838* vt = *(sDmEngVEntry7838**)(eng + 0x2A8);
            vt[6].fn(eng + vt[6].delta, cmd->id);
        } else {
            cVidEngine_PlayVideo(*(char**)self + 0x500);
        }
        if (*(int*)(self + 0xB0) != 0) {
            *(int*)(self + 0xAC) = 1;
        }
        *(int*)(self + 0xBC) = tNISList_SetupNextScript(self, 1, 0);
    } else {
        int wasOn = *(int*)(self + 0xB0);
        func_00276868(self, 0);
        if (*(int*)(self + 0x4) != 0) {
            void* app = SSXApp->Game;
            void* fade = *(void**)((char*)app + 0x64);
            char* transitionMan = (char*)cGame_getTransitionMan(app, 0);
            if (wasOn != 0 && *(int*)(transitionMan + 0x44) != 0) {
                func_002EA780(fade, func_002E4678(transitionMan));
            } else {
                func_002EA780(fade, 0.5f);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_DoTransitionToNext);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cGame_getTransitionMan(void* self, int a1);
char* func_0027C070(void* obj);
extern "C" int func_00277DD8(void* self, void* a1);
struct sDmEnt7DE8;
extern "C" sDmEnt7DE8* func_00277DE8(void* self, int* key);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" void* func_00283D70(void* list, int i);
extern "C" void func_002E4578(void* p);
extern "C" int cSSXScriptEngine_MakeTransitionObject(void* self, int a1, int a2);
extern "C" void cTransitionMan_StartTransition(void* snd, int a1, int h, int h2, int a4, int a5, float t0, float t1, float t2);
extern "C" void cTransitionMan_StartTransitionOut(void* snd, int a1, int h, float t);
extern "C" void cTransitionMan_StartTransitionScreenGrab(void* snd, int a1, int h, int a3, float t);

struct sDmFade_7980 {
    char pad_0x0[0x10];
    signed char cat;        // 0x10
    signed char key;        // 0x11
    short tIn;              // 0x12
    short tHold;            // 0x14
    short tOut;             // 0x16
};

struct sDmEnt_7980 {
    int v0;
    int cat;                // 0x4
    int key;                // 0x8
    float t;                // 0xC
};

// The unit declares tNISList_DoTransitionToNext as void; it returns whether it handled the update.
int tNISList_DoTransitionToNext_impl(char* self) __asm__("tNISList_DoTransitionToNext");

/**
 * Starts the transition from the current NIS entry into the next, or reports one already running.
 *
 * With a script transition running (+0xB0, On Tour mInScriptTransition) it returns 1, unless the start flag (+0xAC,
 * mIsStartTransition) is set: then it ends the transition (func_002E4578 on the game's transition manager,
 * cGame_getTransitionMan(Game, 0)) and returns 0. Otherwise it ends any transition and builds the next one. For a script head
 * entry it reads the next-transition record of the script header (func_0027C070 of the request's player: type +0x10,
 * colour +0x11, in/hold/out frame shorts +0x12..+0x16, 1/60 s each): type 7 is a screen grab (cTransitionMan_StartTransitionScreenGrab) with no
 * wait, any other type a full transition (cTransitionMan_StartTransition with two cSSXScriptEngine_MakeTransitionObject objects; returns
 * 0 if none can be made) whose in+hold frames go to +0xB8 (mTransitionWaitFrames). A video entry uses its video record
 * (func_00277DE8, On Tour GetVideoDBRecord) for a transition out (cTransitionMan_StartTransitionOut) or a screen grab. A positive wait sets
 * +0xB0 and clears +0xAC.
 *
 * @return +0xB0: 1 while a script transition is running
 */
int tNISList_DoTransitionToNext_impl(char* self)
{
    if (*(int*)(self + 0xB0) != 0) {
        if (*(int*)(self + 0xAC) != 0) {
            func_002E4578(cGame_getTransitionMan(SSXApp->Game, 0));
            return 0;
        }
        return 1;
    }
    char* item = (char*)func_00283D70(self + 0xC, 0);
    if (func_00277DD8(self, item) == 0) {
        sDmFade_7980* transitionInfo = (sDmFade_7980*)func_0027C070(cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, *(int*)(item + 0xC)));
        void* snd = cGame_getTransitionMan(SSXApp->Game, 0);
        if (*(int*)((char*)snd + 0x44) != 0) {
            func_002E4578(snd);
        }
        if (transitionInfo->tIn + transitionInfo->tHold + transitionInfo->tOut > 0) {
            if (transitionInfo->cat != 7) {
                int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                if (transition == 0) {
                    return 0;
                }
                int transitionOut = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                cTransitionMan_StartTransition(snd, 2, transition, transitionOut, 0, 0, (float)transitionInfo->tIn * 0.01666666753590107f, (float)transitionInfo->tHold * 0.01666666753590107f, (float)transitionInfo->tOut * 0.01666666753590107f);
                *(int*)(self + 0xB8) = transitionInfo->tIn + transitionInfo->tHold;
            } else {
                int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, transitionInfo->key);
                cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, (float)transitionInfo->tIn * 0.01666666753590107f);
                *(int*)(self + 0xB8) = 0;
            }
            if (*(int*)(self + 0xB8) > 0) {
                *(int*)(self + 0xAC) = 0;
                *(int*)(self + 0xB0) = 1;
            }
        }
    } else {
        sDmEnt_7980* videoInfo = (sDmEnt_7980*)func_00277DE8(self, (int*)item);
        void* snd = cGame_getTransitionMan(SSXApp->Game, 0);
        if (*(int*)((char*)snd + 0x44) != 0) {
            func_002E4578(snd);
        }
        if (0.0f < videoInfo->t) {
            if (videoInfo->cat != 7) {
                int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)self, videoInfo->cat, videoInfo->key);
                cTransitionMan_StartTransitionOut(snd, 2, transition, videoInfo->t);
                *(int*)(self + 0xB8) = 0;
            } else {
                int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, videoInfo->key);
                cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, videoInfo->t);
                *(int*)(self + 0xB8) = 0;
            }
        }
    }
    return *(int*)(self + 0xB0);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277C08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cGame_getTransitionMan(void* self, int a1);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" void func_002826D8(void* self, int a1);
extern "C" void func_00282720(void* self, int a1);
extern "C" int func_00282798(void* self, int a1);
extern "C" void func_00283B78(void* self, int on);
extern "C" void* func_00283D70(void* list, int i);
extern "C" void func_002E4638(void* p, int on);
extern "C" void func_002EA8B8(void* p, int on);

struct sSeqItem_7C08 {
    char pad_0x0[0xC];
    int handle;         // 0xC
};

struct sSeq_7C08 {
    char* owner;        // 0x0
    int f4;             // 0x4
    char pad_0x8[0x4];
    char list[0x98];    // 0xC
    int state;          // 0xA4
    int paused;         // 0xA8
    char pad_0xAC[0x18];
    int pauseCount;     // 0xC4
};

// The unit declares func_00277C08 as returning void*; the body returns nothing. Bind by asm label.
extern "C" void func_00277C08_impl(sSeq_7C08* self, int on, int hard) __asm__("func_00277C08");

extern "C" void func_00277C08_impl(sSeq_7C08* self, int on, int hard)
{
    if (self->state != 3) {
        return;
    }
    if (hard == 0) {
        int c = self->pauseCount;
        if (on) {
            self->pauseCount = c + 1;
            if (c != 0) {
                return;
            }
        } else {
            if (c <= 0) {
                return;
            }
            int d = c - 1;
            self->pauseCount = d;
            if (d != 0) {
                return;
            }
        }
        if (self->paused) {
            return;
        }
    } else {
        if (self->paused == on) {
            return;
        }
        self->paused = on;
        if (self->pauseCount) {
            return;
        }
    }
    if (on) {
        sSeqItem_7C08* it = (sSeqItem_7C08*)func_00283D70(self->list, 0);
        if (func_00277DD8(self, it) == 0) {
            int st = func_00282798(self->owner, it->handle);
            if (st >= 3 && st <= 5) {
                func_002826D8(self->owner, it->handle);
            }
        } else {
            func_00283B78(self->owner + 0x500, 1);
        }
        if (self->f4) {
            func_002EA8B8(SSXApp->Game->CutSceneTransitionMan, 1);
            char* p = (char*)cGame_getTransitionMan(SSXApp->Game, 0);
            if (*(int*)(p + 0x44)) {
                func_002E4638(p, 1);
            }
        }
    } else {
        sSeqItem_7C08* it = (sSeqItem_7C08*)func_00283D70(self->list, 0);
        if (func_00277DD8(self, it) == 0) {
            if (func_00282798(self->owner, it->handle) == 5) {
                func_00282720(self->owner, it->handle);
            }
        } else {
            func_00283B78(self->owner + 0x500, 0);
        }
        if (self->f4) {
            func_002EA8B8(SSXApp->Game->CutSceneTransitionMan, 0);
            char* p = (char*)cGame_getTransitionMan(SSXApp->Game, 0);
            if (*(int*)(p + 0x44)) {
                func_002E4638(p, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277DD8);
#ifdef SKIP_ASM
extern "C" int func_00277DD8(void* self, void* entry)
{
    return *(int*)entry >= 29;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277DE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

struct sDmCfg7DE8 {
    unsigned int flags;
    int v[(0x288 - 4) / 4];
};

struct sDmEnt7DE8 {
    int v[7];
};

extern sDmCfg7DE8 cBEOptionInterface_mOption;
extern sDmEnt7DE8 D_004823DC[];
extern sDmEnt7DE8 D_0048247C[];

extern "C" sDmEnt7DE8* func_00277DE8(void* self, int* key)
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    sDmCfg7DE8 cfg = cBEOptionInterface_mOption;
    if ((cfg.flags & 0x300000) != 0x200000) {
        return &D_0048247C[*key];
    }
    return &D_004823DC[*key];
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tSSXScriptBankInfo_Clear__FPv);
#ifdef SKIP_ASM
/**
 * Resets a script bank info record: zeroes its words at +0x0, +0x8 and +0xC and sets +0x4 to -1.
 */
void tSSXScriptBankInfo_Clear(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine_cSSXScriptEngine);
#ifdef SKIP_ASM
extern "C" void cBXScriptEngine_cBXScriptEngine(void* self);
extern "C" void cVidEngine_cVidEngine(void* self);
extern "C" void cScriptObjCameraManager_cScriptObjCameraManager(void* self);
extern "C" void cScriptObjGS_cScriptObjGS(void* self);
extern "C" void cScriptObjEvent_cScriptObjEvent(void* self);
extern "C" void cScriptAnimBankManager_cScriptAnimBankManager(void* self, int flags, int a2, int a3);
extern "C" void cScriptSoundBankManager_cScriptSoundBankManager(void* self, int flags, int a2, int a3);
void func_00281160(void* self, int i);
extern void* D_00482100[];
extern void* D_004821A0[];
extern void* D_00482168[];
extern void* cSSXScriptEngine_vtbl[];
extern void* D_004A34B0;

struct sScriptSlot_7F20 {
    char data[0x120];
    sScriptSlot_7F20() { cScriptObjCameraManager_cScriptObjCameraManager(this); }
    void* operator new[](unsigned int, void* p) { return p; }
};

struct sScriptMgr_7F20 {
    char pad_0x0[0x2A8];
    void** vt2A8;               // 0x2A8
    void** vt2AC;               // 0x2AC
    char pad_0x2B0[0x518 - 0x2B0];
    void** vt518;               // 0x518
    char pad_0x51C[0x528 - 0x51C];
    void** vt528;               // 0x528
    int f52C;                   // 0x52C
    int f530;                   // 0x530
    int f534;                   // 0x534
    int f538;                   // 0x538
    int f53C;                   // 0x53C
    int f540;                   // 0x540
    int f544;                   // 0x544
    int f548;                   // 0x548
    int f54C;                   // 0x54C
    int f550;                   // 0x550
    int f554;                   // 0x554
    int ids[50];                // 0x558
    int f620;                   // 0x620
    int f624;                   // 0x624
    char cues[10][0x10];        // 0x628
    char pad_0x6C8[0x6D0 - 0x6C8];
    char slots[2][0x120];       // 0x6D0
    char pad_0x910[0xA48 - 0x910];
    int fA48;                   // 0xA48
};

/**
 * Constructs the script engine: its bases and member objects, then empty state.
 *
 * Builds the script, video and data-manager bases (256 data instances), the two object camera managers, the
 * game-state and event objects and the animation and sound bank managers; registers itself as the engine
 * instance (D_004A34B0); no NIS list playing (2), no bank attaching (10), no anchor tables and empty banks.
 *
 * @return @p self
 */
extern "C" sScriptMgr_7F20* cSSXScriptEngine_cSSXScriptEngine(sScriptMgr_7F20* self)
{
    cBXScriptEngine_cBXScriptEngine(self);
    cVidEngine_cVidEngine((char*)self + 0x500);
    cDataManager_cDataManager((cDataManager5828*)((char*)self + 0x51C), 0, 0x100);
    self->vt2AC = D_00482100;
    self->vt518 = D_004821A0;
    self->vt528 = D_00482168;
    self->vt2A8 = cSSXScriptEngine_vtbl;
    new ((char*)self + 0x6D0) sScriptSlot_7F20[2];
    cScriptObjGS_cScriptObjGS((char*)self + 0x910);
    cScriptObjEvent_cScriptObjEvent((char*)self + 0x92C);
    cScriptAnimBankManager_cScriptAnimBankManager((char*)self + 0xA28, 0, 10, 0xF0);
    cScriptSoundBankManager_cScriptSoundBankManager((char*)self + 0xA38, 0, 10, 17);
    self->f52C = 0;
    self->f530 = 0;
    self->f534 = 0;
    self->f538 = 0;
    self->f53C = 0;
    self->f540 = 0;
    self->f544 = 0;
    self->f548 = 0;
    D_004A34B0 = self;
    self->fA48 = 10;
    self->f550 = 2;
    self->f54C = 0;
    self->f554 = 0;
    self->f620 = 0;
    self->f624 = 0;
    int i;
    for (i = 49; i >= 0; i--) {
        self->ids[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        tSSXScriptBankInfo_Clear(self->cues[i]);
    }
    for (i = 0; i < 2; i++) {
        func_00281160(self->slots[i], i);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine__cSSXScriptEngine);
#ifdef SKIP_ASM
extern void* cScriptObjEvent_vtbl[];
extern void* D_00482298[];
extern void* cScriptObjGS_vtbl[];
extern void* D_00482318[];
extern "C" void cSSXScriptEngine_DeInitOnce(cSSXScriptEngine* self);
extern "C" void cScriptSoundBankManager__cScriptSoundBankManager(void* self, int flags);
extern "C" void cScriptAnimBankManager__cScriptAnimBankManager(void* self, int flags);
extern "C" void cScriptObject__cScriptObject(void* self, int flags);
extern "C" void cDataManager__cDataManager(void* self, int flags);
extern "C" void cVidEngine__cVidEngine(void* self, int flags);
extern "C" void cBXScriptEngine__cBXScriptEngine(void* self, int flags);

struct sVE_780B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sSlot_780B8 {
    char pad[0xC];
    sVE_780B8* vt;              // 0xC
    char rest[0x120 - 0x10];
};

/**
 * Destroys the script engine: runs DeInitOnce() if needed, then destroys the members and bases in reverse
 * order.
 *
 * @param flags  g++ destructor flags, passed to the base destructor; bit 0 set frees the object's memory
 */
// Script manager destructor: members in reverse order, then the base.
extern "C" void cSSXScriptEngine__cSSXScriptEngine(cSSXScriptEngine* self, int flags)
{
    self->mInSeqCommVtbl = D_00482100;
    self->mVidEngineVtbl = D_004821A0;
    self->mDataManagerVtbl = D_00482168;
    self->mVtbl = cSSXScriptEngine_vtbl;
    if ((int)self->mpMasterTableHeader != 0) {
        cSSXScriptEngine_DeInitOnce(self);
    }
    D_004A34B0 = 0;
    cScriptSoundBankManager__cScriptSoundBankManager(((char*)self) + 0xA38, 2);
    cScriptAnimBankManager__cScriptAnimBankManager(((char*)self) + 0xA28, 2);
    *(void***)(((char*)self) + 0x944) = D_00482298;
    *(void***)(((char*)self) + 0x92C) = cScriptObjEvent_vtbl;
    cScriptObject__cScriptObject(((char*)self) + 0x938, 0);
    *(void***)(((char*)self) + 0x928) = D_00482318;
    *(void***)(((char*)self) + 0x910) = cScriptObjGS_vtbl;
    cScriptObject__cScriptObject(((char*)self) + 0x91C, 0);
    sSlot_780B8* cameraManager = (sSlot_780B8*)(((char*)self) + 0x910);
    sSlot_780B8* begin = (sSlot_780B8*)(((char*)self) + 0x6D0);
    if (begin != 0) {
        do {
            while (begin != cameraManager) {
                cameraManager--;
                cameraManager->vt[1].fn((char*)cameraManager + cameraManager->vt[1].delta, 0);
            }
        } while (0);
    }
    cDataManager__cDataManager(((char*)self) + 0x51C, 0);
    cVidEngine__cVidEngine(((char*)self) + 0x500, 0);
    cBXScriptEngine__cBXScriptEngine(((char*)self), flags);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine_InitOnce);
#ifdef SKIP_ASM
extern "C" void* FILE_loadz(const char* name, int flags);
extern "C" void cBXScriptEngine_SetupBXEngine(void* self, int count, int a2, int size);
extern char* kpScriptMasterTableFilename;
extern char* kpScriptMasterTableDebugFilename;

struct sDmScript8210 {
    int field_0x0;
    int field_0x4;
    int offset;             // 0x8
};

struct sDmEngine8210 {
    char pad_0x0[0x52C];
    int* hdr;                   // 0x52C
    sDmScript8210* scripts;     // 0x530
    char* base;                 // 0x534
    char pad_0x538[0x1C];
    int field_0x554;            // 0x554
    char pad_0x558[0xC8];
    int field_0x620;            // 0x620
};

/**
 * Loads the script master table (and its debug name table) and sets up the base script engine for it.
 *
 * With the debug table, each script's name offset becomes a pointer into it; without it, the names are 0.
 * Then
 * SetupBXEngine() is called with the script count, 10 and 0x4000.
 */
// PORT: script offsets are rebased into pointers held in int.
extern "C" void cSSXScriptEngine_InitOnce(sDmEngine8210* self)
{
    int* hdr = (int*)FILE_loadz(kpScriptMasterTableFilename, 0);
    self->hdr = hdr;
    self->scripts = (sDmScript8210*)(hdr + 1);
    char* base = (char*)FILE_loadz(kpScriptMasterTableDebugFilename, 0);
    self->base = base;
    if (base != 0) {
        for (int i = 0; i < *self->hdr; i++) {
            self->scripts[i].offset = (int)self->base + self->scripts[i].offset;
        }
    } else {
        for (int i = 0; i < *self->hdr; i++) {
            self->scripts[i].offset = 0;
        }
    }
    self->field_0x554 = 0;
    self->field_0x620 = 0;
    cBXScriptEngine_SetupBXEngine(self, *self->hdr, 10, 0x4000);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine_DeInitOnce);
#ifdef SKIP_ASM
extern "C" void cBXScriptEngine_PurgeBXEngine(void* self);
extern "C" void* MEM_free(void* self);

/**
 * Releases what the script engine's InitOnce set up: the BX script engine and the script master tables.
 *
 * Purges the BX engine, frees the master table header and, when loaded, the debug table, then clears
 * mpMasterTableHeader, mpMasterTable, mpMasterTableDebug and mNumFramesNoSkip.
 */
extern "C" void cSSXScriptEngine_DeInitOnce(cSSXScriptEngine* self)
{
    cBXScriptEngine_PurgeBXEngine(self);
    MEM_free(self->mpMasterTableHeader);
    if (self->mpMasterTableDebug != 0) {
        MEM_free(self->mpMasterTableDebug);
    }
    self->mpMasterTableHeader = 0;
    self->mpMasterTable = 0;
    self->mpMasterTableDebug = 0;
    self->mNumFramesNoSkip = 0;
}
#endif

