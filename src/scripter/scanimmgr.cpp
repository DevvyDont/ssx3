#include "vtable.h"
#include "common.h"
#include "animation/animdatabase.h"
#include "scripter/scanimmgr.h"

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_cScriptAnimBankManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char cScriptAnimBankManager_vtbl[];
extern const char str_tScriptAnimBankInfo[];



/**
 * Constructs a script animation bank manager with a fixed number of empty bank slots.
 *
 * Allocates count 0x14-byte slots ("tScriptAnimBankInfo") with the given memory flags; each slot gets id -1
 * (free), bank number base + i, and zeroed data.
 *
 * @param flags  memory flags for the allocation (On Tour memFlags), also stored at +0x0
 * @param count  number of bank slots (On Tour maxBanks)
 * @param base   bank number of the first slot (On Tour startBankNum)
 * @return self
 */
extern "C" cScriptAnimBankManager* cScriptAnimBankManager_cScriptAnimBankManager(
    cScriptAnimBankManager* self, int flags, int count, int base)
{
    //S
    (*(void**)&self->mVtbl) = cScriptAnimBankManager_vtbl;
    self->mMemFlags = flags;
    self->mMaxBanks = count;
    //E
    (*(tScriptAnimBankInfo**)&self->mpBankInfo) = (tScriptAnimBankInfo*)operator_new_tag(count * sizeof(tScriptAnimBankInfo), str_tScriptAnimBankInfo, flags, 0);
    int idx = base;
    for (int i = 0; i < self->mMaxBanks; i++) {
        (self->mpBankInfo)[i].userID = -1;
        (self->mpBankInfo)[i].bankNum = idx++;
        (*(int*)&(self->mpBankInfo)[i].pData) = 0;
        (self->mpBankInfo)[i].numAnims = 0;
        (*(int*)&(self->mpBankInfo)[i].pTableAnimID) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager__cScriptAnimBankManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cScriptAnimBankManager_UnlinkBanksAll(cScriptAnimBankManager* self);
extern char cScriptAnimBankManager_vtbl[];

/**
 * Destroys the script animation bank manager: unlinks every bank, frees its bank table (+0x8) and frees the object
 * when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptAnimBankManager__cScriptAnimBankManager(cScriptAnimBankManager* self, int flags)
{
    self->mVtbl = (void**)cScriptAnimBankManager_vtbl;
    cScriptAnimBankManager_UnlinkBanksAll(self);
    void* banks = self->mpBankInfo;
    if (banks != 0) {
        cMemMan_free(banks);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_LinkBank);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cAdfDataSource_cAdfDataSource1(void* mem, void* data);
extern const char str_cAdfDataSource[];
extern const char str_pTableAnimID[];

struct sScLinkEntry {
    int id;             // 0x0
    int slot;           // 0x4, low byte = bank table index
    void* data;         // 0x8
    int used;           // 0xC
    int* map;           // 0x10
};

struct sScLinkList {
    int flags;              // 0x0
    int count;              // 0x4
    sScLinkEntry* entries;  // 0x8
};

struct sScBankHdr {
    short field_0x0;
    short count;            // 0x2
};

struct sScBankItem {
    void* ptr;              // 0x0
    char pad_0x4[0x10];
};

struct sScBank {
    int field_0x0;
    sScBankHdr* hdr;        // 0x4
    sScBankItem* items;     // 0x8
};

struct sAnimList;
extern "C" void* cScriptAnimBankManager_GetBankInfo(sAnimList* self, int id);
extern "C" void* cScriptAnimBankManager_GetEmptyBankInfo_v(void* self) __asm__("cScriptAnimBankManager_GetEmptyBankInfo");

/**
 * Links attached animation bank data to the script anim bank manager, creating the bank's entry if needed.
 *
 * When no in-use entry has @p id yet, it takes the first free entry (no check for a full table), stores @p id
 * and @p data, wraps the data in a new 0x18-byte cAdfDataSource (cAdfDataSource_cAdfDataSource1) and installs that in the anim
 * database's bank table (cAnimDataBase_TheAnimDataBase) at the entry's slot. It also counts the bank's non-null items and, if there
 * are any, builds pTableAnimID, the table of their indices. An existing entry is returned unchanged.
 *
 * @param id    bank number (cSSXScriptEngine_OnAttachData passes mCurrAttachingBankNum)
 * @param data  the attached animation bank data
 * @return the entry's slot (set by the constructor to the base slot plus the entry index)
 */
extern "C" int cScriptAnimBankManager_LinkBank(cScriptAnimBankManager* self, int id, void* data)
{
    sScLinkEntry* bankInfo = (sScLinkEntry*)cScriptAnimBankManager_GetBankInfo((sAnimList*)self, id);
    if (bankInfo == 0) {
        bankInfo = (sScLinkEntry*)cScriptAnimBankManager_GetEmptyBankInfo_v(self);
        bankInfo->id = id;
        bankInfo->data = data;
        sScBank* bank = (sScBank*)cAdfDataSource_cAdfDataSource1(cMemMan_alloc(0x18, str_cAdfDataSource, self->mMemFlags, 0), bankInfo->data);
        int n = bank->hdr->count;
        bankInfo->used = 0;
        for (int i = 0; i < n; i++) {
            if (bank->items[i].ptr != 0) {
                bankInfo->used++;
            }
        }
        if (bankInfo->used != 0) {
            bankInfo->map = (int*)operator_new_tag(bankInfo->used * 4, str_pTableAnimID, self->mMemFlags, 0);
            int k = 0;
            for (int i = 0; i < n; i++) {
                if (bank->items[i].ptr != 0) {
                    bankInfo->map[k++] = i;
                }
            }
        }
        cAnimDataBase* tbl = cAnimDataBase_TheAnimDataBase;
        void** slot = &((void**)tbl)[*(unsigned char*)&bankInfo->slot];
        *slot = bank;
    }
    return bankInfo->slot;
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_UnlinkBank);
#ifdef SKIP_ASM
struct sScAnimVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sAnimList;
extern "C" void* cScriptAnimBankManager_GetBankInfo(sAnimList* self, int id);

/**
 * Unlinks the animation bank with user ID @p id, if linked.
 *
 * Looks the bank up with cScriptAnimBankManager_GetBankInfo and passes it to the manager's virtual slot 2 (vtable at +0xC; cScriptAnimBankManager_UnlinkBank1 in
 * cScriptAnimBankManager_vtbl). cSSXScriptEngine_OnDetachData calls it on mAnimationBankManager (+0xA28).
 *
 * @param id  the bank's user ID
 */
extern "C" void cScriptAnimBankManager_UnlinkBank(cScriptAnimBankManager* self, int id)
{
    void* bankInfo = cScriptAnimBankManager_GetBankInfo(((sAnimList*)self), id);
    if (bankInfo != 0) {
        sScAnimVEntry* vt = (sScAnimVEntry*)self->mVtbl;
        vt[2].fn(vthis(self, vt[2]), bankInfo);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_UnlinkBanksAll);
#ifdef SKIP_ASM



/**
 * Unlinks every bank the script anim bank manager currently has linked.
 *
 * For each entry whose data pointer is set it calls the manager's virtual slot 2 (vtable at +0xC) on the entry.
 * In cScriptAnimBankManager's vtable that is cScriptAnimBankManager_UnlinkBank1, which takes the bank out of the anim database's
 * table, destroys it, frees its index table and clears the entry.
 *
 * @param self_  the cScriptAnimBankManager, untyped
 */
extern "C" void cScriptAnimBankManager_UnlinkBanksAll(cScriptAnimBankManager* self_)
{
    cScriptAnimBankManager* self = self_;
    int i;
    for (i = 0; i < self->mMaxBanks; i++) {
        if (((int)(self->mpBankInfo)[i].pData) != 0) {
            ((sVEntry<void(void*, tScriptAnimBankInfo*)>*)self->mVtbl)[2].fn(vthis(self, ((sVEntry<void(void*, tScriptAnimBankInfo*)>*)self->mVtbl)[2]), &(self->mpBankInfo)[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_GetBankInfo);
#ifdef SKIP_ASM
struct sAnimEntry {
    int id;
    int unk4;
    void* data;
    int unkC;
    int unk10;
};

struct sAnimList {
    int unk0;
    int count;
    sAnimEntry* entries;
};

/**
 * Returns the linked bank record (0x14 bytes: user ID, bank number, data, anim count, anim ID table) with user ID
 * @p id.
 *
 * Searches the manager's records (+0x8, count at +0x4) for one with data (+0x8) and that ID (+0x0).
 *
 * @param id  the bank's user ID
 * @return the record, or 0
 */
extern "C" void* cScriptAnimBankManager_GetBankInfo(sAnimList* self, int id)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data != 0 && self->entries[i].id == id) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_UnlinkBank1);
#ifdef SKIP_ASM
extern "C" void cAnimDataBase_purgeChanCache(void** tbl);
extern "C" void cAdfDataSource__cAdfDataSource(void* self, int flags);
void cMemMan_free(void*);

/**
 * Unlinks animation bank record @p bankInfo: removes its data source from the anim database and clears the record.
 *
 * Takes the source out of cAnimDataBase_TheAnimDataBase at the bank number (low byte of +0x4), purges the database's
 * channel cache (cAnimDataBase_purgeChanCache), deletes the source (cAdfDataSource__cAdfDataSource with flags 3), frees
 * the anim ID table (+0x10) when the record has anims (+0xC), and resets the user ID (-1), anim count, data and table.
 *
 * @param bankInfo  the bank record
 */
extern "C" void cScriptAnimBankManager_UnlinkBank1(cScriptAnimBankManager* self, sAnimEntry* bankInfo)
{
    cAnimDataBase* tbl = cAnimDataBase_TheAnimDataBase;
    void** slot = &((void**)tbl)[*(unsigned char*)&bankInfo->unk4];
    void* obj = *slot;
    *slot = 0;
    cAnimDataBase_purgeChanCache((void**)tbl);
    if (obj != 0) {
        cAdfDataSource__cAdfDataSource(obj, 3);
    }
    if (bankInfo->unkC != 0 && bankInfo->unk10 != 0) {
        cMemMan_free((void*)bankInfo->unk10);
    }
    //PSTART
    bankInfo->id = -1;
    bankInfo->unkC = 0;
    bankInfo->data = 0;
    bankInfo->unk10 = 0;
    //PEND
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_GetEmptyBankInfo);
#ifdef SKIP_ASM
/**
 * Returns the first unused bank record (data pointer at +0x8 is 0) of the manager's records (+0x8, count at +0x4).
 *
 * @return the record, or 0 when all are in use
 */
extern "C" sAnimEntry* cScriptAnimBankManager_GetEmptyBankInfo(sAnimList* self)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data == 0) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

