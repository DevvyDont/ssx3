#include "common.h"
#include "online/buddyman.h"
#include "online/lobbyman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "online/dsockchannel.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("dirtysock/tagsunk", cDirtysock_tag_TagFieldSetUnk);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldSetString(char* buf, int size, const char* key, const char* value);

/**
 * Stores a string in a DirtySock tag record, escaping a leading apostrophe with a backquote.
 *
 * When @p value starts with a single quote, it copies "`" plus the string into a 256-byte local buffer (no length
 * check) and stores that; otherwise it stores @p value unchanged. Callers use it for lobby message fields ("TEXT",
 * "BODY").
 *
 * @param buf    tag record to add the field to
 * @param size   size of @p buf in bytes
 * @param key    field name
 * @param value  the string, passed as an int; must not be null
 * @return cDirtysock_tag_TagFieldSetString()'s result
 */
// PORT: the unit declares value as int (callers pass a string pointer in it).
extern "C" int cDirtysock_tag_TagFieldSetUnk(char* buf, int size, const char* key, int value)
{
    char escaped[256];
    int result;
    if (*(const char*)value == '\'') {
        escaped[0] = '`';
        strcpy(escaped + 1, (const char*)value);
        result = cDirtysock_tag_TagFieldSetString(buf, size, key, escaped);
    } else {
        result = cDirtysock_tag_TagFieldSetString(buf, size, key, (const char*)value);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cDirtysock_tag_TagFieldGetUnk);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int cDirtysock_tag_TagFieldGetString(const char* tag, char* buf, int size);

/**
 * Reads a string field and copies it to dst, dropping a leading backquote.
 *
 * Fetches the value with cDirtysock_tag_TagFieldGetString() into a stack buffer of @p size + 1 bytes; if the
 * value starts with '`' that character is skipped when copying.
 *
 * @param tag   field data to read, as for cDirtysock_tag_TagFieldGetString()
 * @param dst   destination; receives up to @p size characters plus the terminator
 * @param size  maximum number of characters to read
 */
// PORT: variable-length array (GNU extension in C++).
extern "C" void cDirtysock_tag_TagFieldGetUnk(const char* tag, char* dst, int size)
{
    char buf[size + 1];
    cDirtysock_tag_TagFieldGetString(tag, buf, size + 1);
    if (buf[0] == '`')
        strcpy(dst, buf + 1);
    else
        strcpy(dst, buf);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259390__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the empty callback that func_0025A948() hands to func_003E7FF0() when it creates the LobbyApi.
 */
void func_00259390(void* self)
{
}
#endif

extern "C" void* strcasecmp(void*, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259398__FPviT0T0);
#ifdef SKIP_ASM
/**
 * Compares two lobby room-list entries by name, ignoring case.
 *
 * func_0025A778() installs it as the room list's compare callback (func_003EE5A0); the name is the string at +0x1C of
 * each entry.
 *
 * @param sortParam   not read
 * @param lhs  first entry
 * @param rhs  second entry
 * @return strcasecmp()'s result: negative, 0 or positive as lhs sorts before, with or after rhs
 */
void* func_00259398(void* self, int sortParam, void* lhs, void* rhs)
{
    return strcasecmp((char*)lhs + 0x1c, (char*)rhs + 0x1c);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002593B8__FPviT0T0);
#ifdef SKIP_ASM
/**
 * Compares two lobby user-list entries by name, ignoring case.
 *
 * func_0025A778() installs it as the user list's compare callback (func_003EE5A0); the name is the string at +0x8 of
 * each entry.
 *
 * @param sortParam   not read
 * @param lhs  first entry
 * @param rhs  second entry
 * @return strcasecmp()'s result: negative, 0 or positive as lhs sorts before, with or after rhs
 */
void* func_002593B8(void* self, int sortParam, void* lhs, void* rhs)
{
    return strcasecmp((char*)lhs + 0x8, (char*)rhs + 0x8);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002593D8);
#ifdef SKIP_ASM
/**
 * Tells whether a list entry's flag bit 21 (0x200000 in the word at +0x4) is clear.
 *
 * Has the (self, a1, entry) shape of the lobby list callbacks; no caller or reference is known in the split.
 *
 * @param filterParam     not read
 * @param entry  the list entry
 * @return 1 when bit 21 of the entry's word at +0x4 is clear, 0 when it is set
 */
extern "C" int func_002593D8(void* self, void* filterParam, void* entry)
{
    return (((*(int*)((char*)entry + 4)) >> 0x15) ^ 1) & 1;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002593F0);
#ifdef SKIP_ASM
/**
 * Compares two list entries by their first word, largest first.
 *
 * A compare callback of the lobby-list shape; no caller or reference is known in the split.
 *
 * @param sortParam   not read
 * @param lhs  first entry
 * @param rhs  second entry
 * @return *rhs - *lhs (negative when lhs's value is larger)
 */
extern "C" int func_002593F0(void* self, void* sortParam, int* lhs, int* rhs)
{
    return *rhs - *lhs;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259400);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldGetStructure(const char* tag, void* data, int len, const char* fmt);
extern char str_l[];

/**
 * Compares two list entries by the first number of their stats text, largest first.
 *
 * Decodes the tag text at +0x28 of each entry with cDirtysock_tag_TagFieldGetStructure() and the template "l*" (hex
 * longs, into two ints) and subtracts the first values. func_00259468() is the ascending version. No caller or
 * reference is known in the split.
 *
 * @param sortParam   not read
 * @param lhs  first entry
 * @param rhs  second entry
 * @return rhs's first value minus lhs's
 */
extern "C" int func_00259400(void* self, int sortParam, char* lhs, char* rhs)
{
    int lhsStats[2];
    int rhsStats[2];
    cDirtysock_tag_TagFieldGetStructure(lhs + 0x28, lhsStats, 8, str_l);
    cDirtysock_tag_TagFieldGetStructure(rhs + 0x28, rhsStats, 8, str_l);
    return rhsStats[0] - lhsStats[0];
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259468);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldGetStructure(const char* tag, void* data, int len, const char* fmt);
extern char str_l[];

/**
 * Compares two list entries by the first number of their stats text, smallest first.
 *
 * Same decoding as func_00259400() (the text at +0x28, template "l*"), with the subtraction reversed. No caller or
 * reference is known in the split.
 *
 * @param sortParam   not read
 * @param lhs  first entry
 * @param rhs  second entry
 * @return lhs's first value minus rhs's
 */
extern "C" int func_00259468(void* self, int sortParam, char* lhs, char* rhs)
{
    int lhsStats[2];
    int rhsStats[2];
    cDirtysock_tag_TagFieldGetStructure(lhs + 0x28, lhsStats, 8, str_l);
    cDirtysock_tag_TagFieldGetStructure(rhs + 0x28, rhsStats, 8, str_l);
    return lhsStats[0] - rhsStats[0];
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_LobbyMan);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cBXString2;
cBXString2* cBXString_cBXString(cBXString2* self);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_00260F10(LobbyMan* self);
// The shared empty-string pointer is read as const so one load serves several string inits.
extern void* const D_004A3E90_94D0 __asm__("cBXString_EmptyString");
extern char str_ps2ssx04_ea_com[];
extern char str_StlBlock_b[];

struct sBXStr_94D0 {
    void* str;
};

struct sListNode_94D0 {
    sListNode_94D0* next;
    sListNode_94D0* prev;
    void* data;
};

struct sList_94D0 {
    int f0;
    sListNode_94D0* head;           // 0x4
    int f8;
    int fC;
    int f10;
    int f14;
};

/**
 * Constructs the lobby manager in already-allocated memory and returns it.
 *
 * Zeroes the state words and handles (mState, mServerAddr, mAuthenticated, mLoggedIn, mDisconnected, unk_014,
 * mHaveCPAT, mFindingPlayer, mpLobbyApi, mpHostLookup, the three list handles, mNumPersonas, mpConfig, mRankSlot,
 * unk_0FC, mpSlotTag, the 29 mRankSnapshot slots), sets every string field (mUserName, mPassword, mPersonas[4],
 * mPersona through mAuth) to the shared empty cBXString, builds mServerName as "ps2ssx04.ea.com", allocates the empty
 * mEventList head node ("StlBlock") and constructs the two account forms (cBXString_cBXString), then resets the
 * new-account form with func_00260F10(). func_0025B650() calls it on a 0x300-byte "LobbyMan" block.
 *
 * @param self_  the memory to construct in
 * @return @p self_
 */
extern "C" void* LobbyMan_LobbyMan(LobbyMan* self_)
{
    // An unused 16-byte local is needed to reproduce the 0x40-byte frame.
    int frameFiller[4];
    self_->mState = 0;
    self_->mServerAddr = 0;
    self_->mAuthenticated = 0;
    self_->mLoggedIn = 0;
    self_->mDisconnected = 0;
    self_->unk_014 = 0;
    self_->mHaveCPAT = 0;
    self_->mFindingPlayer = 0;
    self_->mUserName = (char*)D_004A3E90_94D0;
    self_->mPassword = (char*)D_004A3E90_94D0;
    self_->mpLobbyApi = 0;
    self_->mpHostLookup = 0;
    self_->mpRoomList = 0;
    self_->mpUserList = 0;
    self_->mpRankList = 0;
    self_->mNumPersonas = 0;
    sBXStr_94D0* persona = (sBXStr_94D0*)(((char*)self_) + 0x70);
    for (int i = 3; i != -1; i--, persona++) {
        persona->str = D_004A3E90_94D0;
    }
    self_->mPersona = (char*)D_004A3E90_94D0;
    self_->mRoomName = (char*)D_004A3E90_94D0;
    self_->mGameName = (char*)D_004A3E90_94D0;
    self_->unk_08C = (char*)D_004A3E90_94D0;
    self_->mHostName = (char*)D_004A3E90_94D0;
    self_->mOppoName = (char*)D_004A3E90_94D0;
    self_->mOpponent = (char*)D_004A3E90_94D0;
    self_->mAuth = (char*)D_004A3E90_94D0;
    cBXString_cBXString2(((char*)self_) + 0xE8, str_ps2ssx04_ea_com);
    sList_94D0* lst = (sList_94D0*)(((char*)self_) + 0xEC);
    *(sListNode_94D0**)(((char*)self_) + 0xF0) = (sListNode_94D0*)cMemMan_alloc(0xC, str_StlBlock_b, 0x20000000, 0);
    lst->head->next = *(sListNode_94D0**)(((char*)self_) + 0xF0);
    lst->head->prev = *(sListNode_94D0**)(((char*)self_) + 0xF0);
    self_->mpConfig = 0;
    self_->mRankSlot = 0;
    self_->unk_0FC = 0;
    self_->mpSlotTag = 0;
    cBXString_cBXString((cBXString2*)(((char*)self_) + 0x260));
    cBXString_cBXString((cBXString2*)(((char*)self_) + 0x2B0));
    for (int i = 0x1C; i >= 0; i--)
        ((int*)(((char*)self_) + 0x1EC))[i] = 0;
    func_00260F10(self_);
    return ((char*)self_);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan__LobbyMan);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void func_001DB718(void* self);
extern "C" void func_0025B800(LobbyMan* self);
void operator_delete(int*);

static inline void dtorSub_9628(char* sub)
{
    if (sub + 0x14 != 0) {
        char* p = sub + 0x14 + 0x10;
        while (sub + 0x14 != p) {
            p -= 4;
            cBXString__cBXString(p, 0);
        }
    }
    cBXString__cBXString(sub + 0x10, 2);
    cBXString__cBXString(sub + 0xC, 2);
    cBXString__cBXString(sub + 0x8, 2);
    cBXString__cBXString(sub + 0x4, 2);
    cBXString__cBXString(sub, 2);
}

static inline void dtor4_9628(char* p)
{
    cBXString__cBXString(p + 0xC, 2);
    cBXString__cBXString(p + 0x8, 2);
    cBXString__cBXString(p + 0x4, 2);
    cBXString__cBXString(p, 2);
}

static inline void dtorList_9628(char* lst)
{
    func_001DB718(lst);
    operator_delete(*(int**)(lst + 4));
}

/**
 * Destroys the lobby manager: disconnects, releases its strings and event list, and frees it when asked to.
 *
 * Calls func_0025B800() (disconnect), then runs the cBXString destructors for mAccount and mNewAccount (5 strings each
 * plus the mNameOptions array), destroys mEventList (func_001DB718, then frees the head node), and destroys
 * mServerName, mAuth, mOpponent, mGameName..mOppoName, mRoomName, mPersona, mPersonas[4], mPassword and mUserName.
 * func_0025B688() calls it with flags 3.
 *
 * @param self_  the lobby manager
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void LobbyMan__LobbyMan(LobbyMan* self_, int flags)
{
    func_0025B800(self_);
    dtorSub_9628(((char*)self_) + 0x2B0);
    dtorSub_9628(((char*)self_) + 0x260);
    dtorList_9628(((char*)self_) + 0xEC);
    cBXString__cBXString(((char*)self_) + 0xE8, 2);
    cBXString__cBXString(((char*)self_) + 0x9C, 2);
    cBXString__cBXString(((char*)self_) + 0x98, 2);
    dtor4_9628(((char*)self_) + 0x88);
    cBXString__cBXString(((char*)self_) + 0x84, 2);
    cBXString__cBXString(((char*)self_) + 0x80, 2);
    if (((char*)self_) + 0x70 != 0) {
        char* personaSlot = ((char*)self_) + 0x70 + 0x10;
        while (((char*)self_) + 0x70 != personaSlot) {
            personaSlot -= 4;
            cBXString__cBXString(personaSlot, 0);
        }
    }
    cBXString__cBXString(((char*)self_) + 0x4C, 2);
    cBXString__cBXString(((char*)self_) + 0x48, 2);
    if (flags & 1)
        operator_delete((int*)self_);
}
#endif

extern "C" void* func_003E8FD0(int);

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onincomingmsg__FPv);
#ifdef SKIP_ASM
/**
 * Logs an incoming lobby message: passes mpLobbyApi and the message to func_003E8FD0().
 *
 * The message arrives in the second argument register (the C declares only self) and reaches func_003E8FD0() untouched;
 * that function prints "recv: %c%c%c%c/%c%c%c%c" with the message's two codes plus its text (or "[null data]", "[binary
 * data]", "[long data]"). The functor thunk func_00268B08, registered as LobbyApi message callback 0 by
 * func_0025A948(), calls it.
 *
 * @return no meaningful value (func_003E8FD0() returns nothing)
 */
void* LobbyMan_onincomingmsg(void* self)
{
    return func_003E8FD0(*(int*)((char*)self + 0x54));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onconnectionmsg);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern "C" int* func_002591D0();
extern "C" void func_00263BA8(void* p, int arg);

struct sMsgListNodeK2 {
    sMsgListNodeK2* next;
    sMsgListNodeK2* prev;
    void* data;
};

struct sMsgListIterK2 {
    sMsgListNodeK2* node;
    sMsgListIterK2(sMsgListNodeK2* x) : node(x) {}
    sMsgListIterK2(const sMsgListIterK2& x) : node(x.node) {}
};

static inline sMsgListIterK2 sMsgListK2_insert(sMsgListIterK2 pos, void* const& x)
{
    sMsgListNodeK2* tmp = (sMsgListNodeK2*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Handles a lobby connection message: on a 'disc' (disconnect) message it marks the lobby disconnected.
 *
 * When the message code at +0x4 is 'disc' it pushes a type-277 event (8 bytes, vtable online_Event_vtbl) onto mEventList, sets
 * mDisconnected (func_0025AC50() then disconnects), and when the game connection object func_002591D0() returns is
 * active it shuts that down with func_00263BA8(..., 2). Called through the functor thunk func_00268B58 (LobbyApi
 * callback 3).
 *
 * @param msg  the LobbyApi message (word +0x4 is its code)
 */
extern "C" void LobbyMan_onconnectionmsg(LobbyMan* self, int* msg)
{
    if (msg[1] == 0x64697363) {
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 277;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        void* mp = event;
        sMsgListK2_insert(*(sMsgListNodeK2**)(((char*)self) + 0xF0), mp);
        self->mDisconnected = 1;
        if (*func_002591D0() != 0)
            func_00263BA8(func_002591D0(), 2);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onuserlistupdate);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNodeK2b {
    sMsgListNodeK2b* next;
    sMsgListNodeK2b* prev;
    void* data;
};

struct sMsgListIterK2b {
    sMsgListNodeK2b* node;
    sMsgListIterK2b(sMsgListNodeK2b* x) : node(x) {}
    sMsgListIterK2b(const sMsgListIterK2b& x) : node(x.node) {}
};

static inline sMsgListIterK2b sMsgListK2b_insert(sMsgListIterK2b pos, void* const& x)
{
    sMsgListNodeK2b* tmp = (sMsgListNodeK2b*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Posts a "user list updated" event (type 0xE0) on mEventList.
 *
 * Called through the functor thunk func_00268AB8, which func_0025A778() registers as the user list's (mpUserList)
 * update callback.
 */
extern "C" void LobbyMan_onuserlistupdate(LobbyMan* self)
{
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = 0xE0;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListK2b_insert(*(sMsgListNodeK2b**)(((char*)self) + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onranklistupdate);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNodeK2c {
    sMsgListNodeK2c* next;
    sMsgListNodeK2c* prev;
    void* data;
};

struct sMsgListIterK2c {
    sMsgListNodeK2c* node;
    sMsgListIterK2c(sMsgListNodeK2c* x) : node(x) {}
    sMsgListIterK2c(const sMsgListIterK2c& x) : node(x.node) {}
};

static inline sMsgListIterK2c sMsgListK2c_insert(sMsgListIterK2c pos, void* const& x)
{
    sMsgListNodeK2c* tmp = (sMsgListNodeK2c*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Posts a "rank list updated" event (type 225) on mEventList.
 *
 * Called through the functor thunk func_00268AE0 (the rank-list and rank-snapshot callback set by func_0025A778() and
 * func_0025E348()); func_0025E348() also calls it directly.
 *
 * @param message  not read
 */
extern "C" void LobbyMan_onranklistupdate(LobbyMan* self, int message)
{
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = 225;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListK2c_insert(*(sMsgListNodeK2c**)(((char*)self) + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onchatmsg);
#ifdef SKIP_ASM
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
// PORT: TagFieldGetUnk is called with a 4th (default value) argument; the unit defines 3.
extern "C" void cDirtysock_tag_TagFieldGetUnk4(const char* tag, char* dst, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetUnk");
extern "C" int func_004178B0(const char* str, const char* fmt, ...);
extern "C" int func_004165A8(void*, void*);
extern "C" char* cCommSystem_getSignature(void* comm);
extern "C" void* strcasecmp(void*, void*);
extern "C" void func_00260F80(void);
extern "C" int func_00261460(void* p);
extern "C" void func_002613C8(BuddyMan* p);
extern "C" void func_0025B800(LobbyMan* self);
extern "C" int func_002623B0(BuddyMan* self, int name);
extern "C" int func_00263128(BuddyMan* self, char* name);
extern "C" void func_00263018(BuddyMan* self, void* name);
extern "C" void func_00262AF0(BuddyMan* self, char* name, int a2, int a3, int a4, int a5, int a6, int a7,
                              int s0, int s1, char* s2, int s3, int s4, int s5, int s6);
extern "C" void func_0025F628(LobbyMan* selfp, int notify, int flag);
extern "C" void func_0025F858(LobbyMan* self);
extern "C" void func_0025FA48(LobbyMan* self, const char* name);
extern "C" void func_0025FAD0(LobbyMan* self, const char* name);
extern "C" void* func_0025FB80(LobbyMan*, int, void*);
extern "C" void func_0025AB40(LobbyMan* self, const char* a, const char* b);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void cBXString__cBXString(void* self, int flags);
extern char D_004A2F38[];
extern char D_004A2F40[];
extern char D_004A2F48[];
extern char str_pct_s_pct_d_pct_d_pct_d_pct_d_pct_d_pct[];
extern char str_challenge[];
extern char str_lockChal[];
extern char str_decline[];
extern char str_abortChal[];
extern char str_ignore[];
extern char str_badversion[];
extern char str_broadcast[];
extern void* D_004807E8[];
extern void* D_004A2F50[];
extern void* D_004807F8[];

struct sMsgListNode9B10 {
    sMsgListNode9B10* next;
    sMsgListNode9B10* prev;
    void* data;
};

struct sMsgListIter9B10 {
    sMsgListNode9B10* node;
    sMsgListIter9B10(sMsgListNode9B10* x) : node(x) {}
    sMsgListIter9B10(const sMsgListIter9B10& x) : node(x.node) {}
};

static inline sMsgListIter9B10 sMsgList9B10_insert(sMsgListIter9B10 pos, void* const& x)
{
    sMsgListNode9B10* tmp = (sMsgListNode9B10*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_9B10(char* self, void* msg)
{
    void* mp = msg;
    sMsgList9B10_insert(*(sMsgListNode9B10**)(self + 0xF0), mp);
}

static inline int* newMsg_9B10()
{
    return (int*)operator new(8, str_NetEvent, 0x20000000, 0);
}

static inline void* Mgr_9B10()
{
    if (BuddyMan_mBuddyMan == 0)
        func_00260F80();
    if (func_00261460(BuddyMan_mBuddyMan) == 0)
        func_002613C8(BuddyMan_mBuddyMan);
    if (BuddyMan_mBuddyMan == 0)
        func_0025B800(LobbyMan_mLobbyMan);
    return BuddyMan_mBuddyMan;
}

// Minimal view of cBXString; the ctor is bound to the game's (unmangled) symbol.
// PORT: asm labels are not accepted on destructors, so the dtor forwards to the out-of-line one (in-charge flag 2).
struct sBXStr_9B10 {
    char* p;
    sBXStr_9B10(const char* s) __asm__("cBXString_cBXString2");
    sBXStr_9B10(const sBXStr_9B10& o) __asm__("cBXString_cBXString1");
    ~sBXStr_9B10() { cBXString__cBXString(this, 2); }
};
extern "C" sBXStr_9B10 func_00318630(const sBXStr_9B10& a, const char* b);

/**
 * Handles a lobby chat message: private challenge commands, room chat and broadcasts.
 *
 * Reads the sender ("N", into a 256-byte buffer) and the text ("T", via cDirtysock_tag_TagFieldGetUnk, which drops an
 * escaping backquote), then switches on the message code at +0x4. Every BuddyMan lookup first makes sure the buddy
 * manager exists (func_00260F80) and is connected (func_00261460 / func_002613C8).
 * - 'priv' with flag 0x40000000 set and 0x200000 clear in the word at +0x8: scans the text with func_004178B0()
 *   (sscanf-style, "%s" then 8 "%d", "%s", 4 "%d") into a command word, 8 numbers, a version signature and 4 more
 *   numbers, then by command:
 * - "challenge": a signature other than cCommSystem_getSignature() answers "badversion" (func_0025FB80); a sender
 *   func_002623B0() reports as ignored answers "ignore"; when waiting for a game (mState 22) with that sender as
 *   mOpponent it calls func_0025F628(self, 1, 1); otherwise it adds the challenger to BuddyMan (func_00262AF0).
 * - "lockChal": from mOpponent in mState 22 calls func_0025FA48(), in mState 23 func_0025FAD0(), otherwise answers
 *   "abortChal".
 * - "decline", "ignore", "badversion": mState 22 drops back to 1 and an event of type 237, 240 or 242 is posted.
 * - "abortChal": removes the sender's pending challenge from BuddyMan (func_00263128 / func_00263018); if the sender is
 *   mOpponent, mState 24 calls func_0025F858() and 22/23 drop to 1; posts type 238 unless the abort came from someone
 *   else while in 22..24.
 * - 'chat': unless func_002623B0() reports the sender as ignored, posts it as a chat event (func_0025AB40).
 * - 'cast': posts "*broadcast* " + text as a chat event from the sender.
 *
 * Called through the functor thunk func_00268B30 (LobbyApi callback 1).
 *
 * @param msg  the LobbyApi message: code at +0x4, flags at +0x8, tag record pointer at +0xC
 */
extern "C" void LobbyMan_onchatmsg(LobbyMan* self, char* msg)
{
    char name[0x100];
    char text[0x100];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A2F38), name, 0x100, D_004A2F40);
    cDirtysock_tag_TagFieldGetUnk4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A2F48), text, 0x100, D_004A2F40);
    switch (*(int*)(msg + 4)) {
    case 0x70726976: { // 'priv'
        int flags = *(int*)(msg + 8);
        if (flags & 0x200000)
            break;
        if (!(flags & 0x40000000))
            break;
        char cmd[0x100];
        char sig[0x40];
        int locationId, gameMode, gameEvent, ranked, multipliers, powerUps, numberAI, pointIcons, i, j, k, rank;
        func_004178B0(text, str_pct_s_pct_d_pct_d_pct_d_pct_d_pct_d_pct, cmd, &locationId, &gameMode, &gameEvent, &ranked, &multipliers, &powerUps, &numberAI, &pointIcons, sig, &i, &j, &k, &rank);
        if (func_004165A8(cmd, str_challenge) == 0) {
            if (func_004165A8(sig, cCommSystem_getSignature(cCommSystem_mCommSystem)) != 0) {
                func_0025FB80(self, (int)name, D_004807E8);
            } else if (func_002623B0((BuddyMan*)Mgr_9B10(), (int)name) != 0) {
                func_0025FB80(self, (int)name, D_004A2F50);
            } else if (*(int*)self == 22 && func_004165A8(self->mOpponent, name) == 0) {
                func_0025F628(self, 1, 1);
            } else {
                func_00262AF0((BuddyMan*)((char*)Mgr_9B10()), name, locationId, gameMode, gameEvent, ranked != 0, multipliers != 0, powerUps != 0, numberAI, pointIcons != 0, text, i, j, k, rank);
            }
        } else if (func_004165A8(cmd, str_lockChal) == 0) {
            if (*(int*)self == 22 && func_004165A8(self->mOpponent, name) == 0) {
                func_0025FA48(self, name);
            } else if (*(int*)self == 23) {
                func_0025FAD0(self, name);
            } else {
                func_0025FB80(self, (int)name, D_004807F8);
            }
        } else if (func_004165A8(cmd, str_decline) == 0) {
            if (*(int*)self == 22)
                *(int*)self = 1;
            {
                int* event = newMsg_9B10();
                event[0] = 237;
                *(void***)((char*)event + 0x4) = online_Event_vtbl;
                post_9B10(((char*)self), event);
            }
        } else if (func_004165A8(cmd, str_abortChal) == 0) {
            if (func_00263128((BuddyMan*)((char*)Mgr_9B10()), name) != 0)
                func_00263018((BuddyMan*)Mgr_9B10(), name);
            int gone = strcasecmp(self->mOpponent, name) == 0;
            int send = 0;
            if (*(int*)self == 0x18) {
                if (gone) {
                    func_0025F858(self);
                    send = 1;
                }
            } else if (*(int*)self == 22 || *(int*)self == 23) {
                if (gone) {
                    *(int*)self = 1;
                    send = 1;
                }
            } else {
                send = 1;
            }
            if (send)
                {
                int* event = newMsg_9B10();
                event[0] = 238;
                *(void***)((char*)event + 0x4) = online_Event_vtbl;
                post_9B10(((char*)self), event);
            }
        } else if (func_004165A8(cmd, str_ignore) == 0) {
            if (*(int*)self == 22)
                *(int*)self = 1;
            {
                int* event = newMsg_9B10();
                event[0] = 0xF0;
                *(void***)((char*)event + 0x4) = online_Event_vtbl;
                post_9B10(((char*)self), event);
            }
        } else if (func_004165A8(cmd, str_badversion) == 0) {
            if (*(int*)self == 22)
                *(int*)self = 1;
            {
                int* event = newMsg_9B10();
                event[0] = 242;
                *(void***)((char*)event + 0x4) = online_Event_vtbl;
                post_9B10(((char*)self), event);
            }
        }
        break;
    }
    case 0x63686174: // 'chat'
        if (func_002623B0((BuddyMan*)Mgr_9B10(), (int)name) == 0)
            func_0025AB40(self, name, text);
        break;
    case 0x63617374: { // 'cast'
        sBXStr_9B10 res = func_00318630(sBXStr_9B10(str_broadcast), text);
        func_0025AB40(self, name, res.p);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onplaymsg);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);
extern "C" int cDirtysock_tag_TagFieldGetTime(const char* tag, int defval);
extern "C" int cDirtysock_tag_TagFieldGetAddr(const char* tag, int defval);
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" int func_0025A730(LobbyMan* self, const char* key, int def);
extern "C" int func_004165A8(void*, void*);
extern "C" int* func_002591D0();
extern "C" void func_00263BA8(void* p, int arg);
extern "C" void func_00259098(void* p, int a, int b);
extern "C" unsigned int strlen(const char*);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_setGameMode(void* self, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* self, int ev);
extern "C" void cBENewRaceInterface_setNumberAI(void* self, int n);
extern char D_004A2F40[];
extern char str_NAME[];
extern char str_SELF[];
extern char str_HOST[];
extern char str_OPPO[];
extern char str_AUTH[];
extern char str_WHEN[];
extern char str_P1[];
extern char str_P2[];
extern char str_FROM[];
extern char str_ADDR[];
extern char str_SEED[];
extern int D_00535C08[];
extern int D_004A11B8;
extern int D_00534B34[];
extern char D_005308B8[];
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNodeA1D0 {
    sMsgListNodeA1D0* next;
    sMsgListNodeA1D0* prev;
    void* data;
};

struct sMsgListIterA1D0 {
    sMsgListNodeA1D0* node;
    sMsgListIterA1D0(sMsgListNodeA1D0* x) : node(x) {}
    sMsgListIterA1D0(const sMsgListIterA1D0& x) : node(x.node) {}
};

static inline sMsgListIterA1D0 sMsgListA1D0_insert(sMsgListIterA1D0 pos, void* const& x)
{
    sMsgListNodeA1D0* tmp = (sMsgListNodeA1D0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_A1D0(char* self, void* msg)
{
    void* mp = msg;
    sMsgListA1D0_insert(*(sMsgListNodeA1D0**)(self + 0xF0), mp);
}


struct sVEntryA1D0 { short delta; short index; int (*fn)(void*); };

/**
 * Handles the lobby's game-start message: stores the session's players and addresses and sets up the race.
 *
 * Reads NAME, SELF, HOST, OPPO, AUTH (strings), WHEN (mWhen, time), P1 and P2 (numbers, default -1), FROM and ADDR
 * (addresses, which func_0025A730() may override by the host's / opponent's name) and SEED (mSeed). Stores mGameName,
 * mHostName, mOppoName, mAuth, mHostAddr, mOppoAddr, mIsHost (SELF equals HOST) and mOpponent (the one of HOST/OPPO
 * that is not us). Unless mState is 24 or 20 (waits that expect this message) it just sets mState 25 and shuts the game
 * connection down (func_00263BA8(..., 0)). Otherwise it sets mState 25 and, when P1 is present, configures
 * cBENewRaceInterface: P2 0 or 1 selects game mode 0 or 1 (5 or 6, with event 4 or 5, when P1 is 14..16), 2 selects
 * mode 3 and 3 mode 2; sets D_00535C08[0] = P1 and D_004A11B8 = 1, and when D_00534B34[0] is set ORs 0x1C0 into the
 * word at D_005308B8 + 0x18, sets no AI riders and calls the interface's virtual slot 1. Finally it starts the game
 * connection with func_00259098(..., mMatchType, 0) and posts a type-239 event. Called through the functor thunk
 * func_00268B80 (LobbyApi callback 4).
 *
 * @param msg  the LobbyApi message (tag record pointer at +0xC)
 */
extern "C" void LobbyMan_onplaymsg(LobbyMan* self, char* msg)
{
    char name[0x100];
    char selfName[0x100];
    char hostName[0x100];
    char oppoName[0x100];
    char auth[0x100];
    char p1Text[0x100];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_NAME), name, 0x100, D_004A2F40);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_SELF), selfName, 0x100, D_004A2F40);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_HOST), hostName, 0x100, D_004A2F40);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_OPPO), oppoName, 0x100, D_004A2F40);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_AUTH), auth, 0x100, D_004A2F40);
    self->mWhen = cDirtysock_tag_TagFieldGetTime(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_WHEN), 0);
    int ev = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_P1), -1);
    int kind = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_P2), -1);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_P1), p1Text, 0x100, D_004A2F40);
    self->mHostAddr = func_0025A730(self, hostName, cDirtysock_tag_TagFieldGetAddr(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_FROM), 0));
    self->mOppoAddr = func_0025A730(self, oppoName, cDirtysock_tag_TagFieldGetAddr(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_ADDR), 0));
    cBXString_operatorE1(((char*)self) + 0x88, name);
    cBXString_operatorE1(((char*)self) + 0x90, hostName);
    cBXString_operatorE1(((char*)self) + 0x94, oppoName);
    cBXString_operatorE1(((char*)self) + 0x9C, auth);
    int same = func_004165A8(selfName, hostName) == 0;
    self->mIsHost = same;
    cBXString_operatorE1(((char*)self) + 0x98, same ? oppoName : hostName);
    self->mSeed = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_SEED), 0);
    if (*(int*)self != 0x18 && *(int*)self != 20)
    {
        *(int*)self = 25;
        func_00263BA8(func_002591D0(), 0);
        return;
    }
    *(int*)self = 25;
    if (strlen(p1Text))
    {
        char* ri = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
        int mode = 0;
        switch (kind)
        {
        case 0:
            if (ev >= 14 && ev <= 16)
            {
                mode = 5;
                kind = 4;
            }
            else
                mode = 0;
            break;
        case 1:
            if (ev >= 14 && ev <= 16)
            {
                mode = 6;
                kind = 5;
            }
            else
                mode = 1;
            break;
        case 2:
            mode = 3;
            break;
        case 3:
            mode = 2;
            break;
        }
        cBENewRaceInterface_setGameMode(ri, mode);
        cBENewRaceInterface_setGameEvent(ri, kind);
        D_00535C08[0] = ev;
        D_004A11B8 = 1;
        cBE_getInterface_Fv(((void*)(SSXApp)->BE), 7);
        if (D_00534B34[0] != 0)
        {
            char* cfg = D_005308B8;
            *(int*)(cfg + 0x18) |= 0x1C0;
            cBENewRaceInterface_setNumberAI(ri, 0);
            sVEntryA1D0* vt = *(sVEntryA1D0**)(ri + 0xC);
            vt[1].fn(ri + vt[1].delta);
        }
    }
    int type = 239;
    func_00259098(func_002591D0(), self->mMatchType, 0);
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    post_A1D0(((char*)self), event);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A650);
#ifdef SKIP_ASM
extern "C" char* func_003E8A70(void* p, int tag);
extern "C" void* func_003E6E08();
extern "C" void func_003E7120(void* h, char* fmt, char* arg);
extern char str_SSX_PS2_2004[];

/**
 * Creates the lobby config handle mpConfig from the logged-in persona's 'pers' data, once.
 *
 * Does nothing unless mLoggedIn is set, mpConfig is still 0 and the LobbyApi's 'pers' value (func_003E8A70) is a
 * non-empty string. Then it creates the handle with func_003E6E08() and loads that text into it under the section
 * "SSX-PS2-2004" (func_003E7120). func_0025A730() reads keys from it and func_0025A6F8() frees it.
 */
extern "C" void func_0025A650(LobbyMan* self)
{
    if (self->mLoggedIn == 0)
        return;
    if (self->mpConfig != 0)
        return;
    if (func_003E8A70(self->mpLobbyApi, 0x70657273) == 0)
        return;
    if (*func_003E8A70(self->mpLobbyApi, 0x70657273) == 0)
        return;
    self->mpConfig = func_003E6E08();
    if (self->mpConfig == 0)
        return;
    func_003E7120(self->mpConfig, str_SSX_PS2_2004, func_003E8A70(self->mpLobbyApi, 0x70657273));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A6F8);
#ifdef SKIP_ASM
extern "C" void NetGameUtilDestroy(void*);

/**
 * Frees the lobby config handle mpConfig (NetGameUtilDestroy) and clears it, if there is one.
 */
extern "C" void func_0025A6F8(LobbyMan* self)
{
    void* config = self->mpConfig;
    if (config != 0) {
        NetGameUtilDestroy(config);
        self->mpConfig = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A730);
#ifdef SKIP_ASM
extern char str_SSX_PS2_2004[];
extern "C" int func_003E7218(void* tags, const char* key, const char* def);

/**
 * Looks a key up in the lobby config (mpConfig, section "SSX-PS2-2004") and returns its value, or a default.
 *
 * The game-start handler LobbyMan_onplaymsg() uses it to override the host's and opponent's addresses by player name.
 *
 * @param key  the key to look up
 * @param def  value returned when there is no config or func_003E7218() returns 0
 * @return the configured value, or @p def
 */
extern "C" int func_0025A730(LobbyMan* self, const char* key, int def)
{
    void* tags = self->mpConfig;
    if (tags != 0) {
        int configValue = func_003E7218(tags, str_SSX_PS2_2004, key);
        if (configValue != 0) {
            def = configValue;
        }
    }
    return def;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A778);
#ifdef SKIP_ASM
extern "C" void* func_003E85B0(void* mgr, int idx, void* cb, void* user);
extern "C" void func_003EE5A0(void* h, int a, int b, void* cb);
extern "C" void func_00268A90(void* self, void* a1, int a2);
extern "C" void func_00268AB8(void* self, void* a1, int a2);
extern "C" void func_00268AE0(void* self, void* a1, int a2);

struct sDsSessK2 {
    char pad_0x00[0x54];
    void* mgr;                   // 0x54
    char pad_0x58[4];
    void* chan[3];               // 0x5C
    char pad_0x68[0x104 - 0x68];
    unsigned int slotA[29];      // 0x104
    unsigned int slotB[29];      // 0x178
};

/**
 * Opens the lobby's room, user and rank lists and resets the rank-request slots.
 *
 * Opens list 0 (rooms, mpRoomList) with the functor thunk func_00268A90 and the name compare func_00259398(), list 1
 * (users, mpUserList) with func_00268AB8 and func_002593B8(), and list 2 (ranks, mpRankList) with func_00268AE0, all
 * through func_003E85B0() on the LobbyApi; then sets all 29 mRankCount and mRankRequest entries to -1. func_0025A948()
 * calls it right after creating the LobbyApi.
 */
extern "C" void func_0025A778(sDsSessK2* self)
{
    int i;
    self->chan[0] = func_003E85B0(self->mgr, 0, (void*)func_00268A90, self);
    func_003EE5A0(self->chan[0], 0, 0, (void*)func_00259398);
    self->chan[1] = func_003E85B0(self->mgr, 1, (void*)func_00268AB8, self);
    func_003EE5A0(self->chan[1], 0, 0, (void*)func_002593B8);
    self->chan[2] = func_003E85B0(self->mgr, 2, (void*)func_00268AE0, self);
    for (i = 0; i < 29; i++) {
        self->slotA[i] = 0xFFFFFFFF;
        self->slotB[i] = 0xFFFFFFFF;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A850);
#ifdef SKIP_ASM
extern "C" void func_003E8688(void* ctx, int kind, void* p);

/**
 * Releases every open rank-list snapshot: each non-null mRankSnapshot[i] (29 slots) goes to func_003E8688(..., 4, ...)
 * and is cleared.
 */
extern "C" void func_0025A850(LobbyMan* self)
{
    int i;
    void** arr = (void**)((char*)self + 0x1EC);
    for (i = 0; i < 29; i++) {
        if (arr[i] != 0) {
            func_003E8688(self->mpLobbyApi, 4, arr[i]);
            arr[i] = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025A8B0);
#ifdef SKIP_ASM
struct sTagsUnkCallbackObj {
    char pad[0xC];
    void (*fn)(sTagsUnkCallbackObj* self);
};

/**
 * Closes the lobby's lists, rank snapshots and pending host lookup.
 *
 * Releases mpRoomList, mpUserList and mpRankList through func_003E8688() (kinds 0, 1, 2) and clears them, frees the
 * snapshots with func_0025A850(), and releases mpHostLookup through its own release function (+0xC) before clearing it.
 */
extern "C" void func_0025A8B0(LobbyMan* self)
{
    if (self->mpRoomList != 0) {
        func_003E8688(self->mpLobbyApi, 0, self->mpRoomList);
        self->mpRoomList = 0;
    }
    if (self->mpUserList != 0) {
        func_003E8688(self->mpLobbyApi, 1, self->mpUserList);
        self->mpUserList = 0;
    }
    if (self->mpRankList != 0) {
        func_003E8688(self->mpLobbyApi, 2, self->mpRankList);
        self->mpRankList = 0;
    }
    func_0025A850(self);
    sTagsUnkCallbackObj* obj = (sTagsUnkCallbackObj*)self->mpHostLookup;
    if (obj != 0) {
        obj->fn(obj);
        self->mpHostLookup = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A948);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* func_003E7FF0(const char* tag, int a, void* cb);
extern "C" void func_003E8248(void* mgr, int idx, void* cb, void* user);
extern "C" const char* func_00259198();
extern "C" void func_00268B08(void* self, void* a1, int a2);
extern "C" void func_00268B30(void* self, void* a1, int a2);
extern "C" void func_00268B58(void* self, void* a1, int a2);
extern "C" void func_00268B80(void* self, void* a1, int a2);
extern char str_PROD[];
extern char str_PS2[];
extern char str_VERS[];
extern char str_LANG[];
extern char str_SLUS[];
extern char str_SSX_PS2_2004[];
extern char str_Beta_1_04[];
extern char str_SLUS_20772[];
extern char str_pct_s_pct_s_pct_s_b[];
extern char str_Sep_17_2003[];

/**
 * Creates the DirtySock LobbyApi instance and registers the lobby manager's message callbacks.
 *
 * Builds the client identity record: PROD "SSX-PS2-2004", VERS "PS2/Beta 1.04-Sep 17 2003" ("%s/%s-%s"), LANG
 * func_00259198() ("en") and SLUS "SLUS_20772"; creates the instance with func_003E7FF0(record, 0, func_00259390) into
 * mpLobbyApi; registers the functor thunks func_00268B08 (callback 0, logging), func_00268B30 (1, chat), func_00268B58
 * (3, connection) and func_00268B80 (4, game start) with func_003E8248(); then opens the lists with func_0025A778().
 * The connect function func_0025B6D8() calls it when there is no instance yet.
 *
 * @param self_  the lobby manager
 */
extern "C" void func_0025A948(LobbyMan* self_)
{
    char buf[0x40];
    char tag[0x100];
    tag[0] = 0;
    cDirtysock_tag_TagFieldSetString(tag, 0x100, str_PROD, str_SSX_PS2_2004);
    sprintf(buf, str_pct_s_pct_s_pct_s_b, str_PS2, str_Beta_1_04, str_Sep_17_2003);
    cDirtysock_tag_TagFieldSetString(tag, 0x100, str_VERS, buf);
    const char* key = str_LANG;
    cDirtysock_tag_TagFieldSetString(tag, 0x100, key, func_00259198());
    cDirtysock_tag_TagFieldSetString(tag, 0x100, str_SLUS, str_SLUS_20772);
    void* mgr = func_003E7FF0(tag, 0, (void*)func_00259390);
    self_->mpLobbyApi = (LobbyApiRefT*)mgr;
    func_003E8248(mgr, 0, (void*)func_00268B08, ((char*)self_));
    func_003E8248(self_->mpLobbyApi, 1, (void*)func_00268B30, ((char*)self_));
    func_003E8248(self_->mpLobbyApi, 3, (void*)func_00268B58, ((char*)self_));
    func_003E8248(self_->mpLobbyApi, 4, (void*)func_00268B80, ((char*)self_));
    func_0025A778((sDsSessK2*)self_);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025AA98);
#ifdef SKIP_ASM
extern "C" void func_0025A8B0(LobbyMan* self);
extern "C" void func_003E80C0(void* p);
extern "C" void func_003E81C8(void* p);

/**
 * Shuts the LobbyApi down: closes the lists (func_0025A8B0), then calls func_003E80C0() and func_003E81C8() on
 * mpLobbyApi and clears it.
 */
extern "C" void func_0025AA98(LobbyMan* self)
{
    func_0025A8B0(self);
    if (self->mpLobbyApi != 0) {
        func_003E80C0(self->mpLobbyApi);
        func_003E81C8(self->mpLobbyApi);
        self->mpLobbyApi = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025AAE0);
#ifdef SKIP_ASM
extern char str_TEXT[];
extern "C" int cDirtysock_tag_TagFieldSetUnk(char* buf, int size, const char* key, int value);
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);

/**
 * Sends a 'mesg' lobby request carrying @p value as its TEXT field, when the lobby is not idle (mState != 0).
 *
 * The field is written with cDirtysock_tag_TagFieldSetUnk() (a leading apostrophe gets a backquote) into a 0x200-byte
 * record and sent with func_003E8B10() without a reply callback; mState and mTimeout are left alone.
 *
 * @param value  the message text (a string pointer passed as an int)
 */
extern "C" void func_0025AAE0(LobbyMan* self, int value)
{
    char buf[0x200];
    if (*(int*)self != 0) {
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetUnk(buf, 0x200, str_TEXT, value);
        func_003E8B10(self->mpLobbyApi, 0x6D657367, buf, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025AB40);
#ifdef SKIP_ASM
extern char str_ChatEvent[];
extern char str_StlBlock_b[];
extern void* D_00481230[];
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void* cBXString_cBXString2(void* self, const char* s);

struct sMsgListNodeAB40 {
    sMsgListNodeAB40* next;
    sMsgListNodeAB40* prev;
    void* data;
};

struct sMsgListIterAB40 {
    sMsgListNodeAB40* node;
    sMsgListIterAB40(sMsgListNodeAB40* x) : node(x) {}
    sMsgListIterAB40(const sMsgListIterAB40& x) : node(x.node) {}
};

static inline sMsgListIterAB40 sMsgListAB40_insert(sMsgListIterAB40 pos, void* const& x)
{
    sMsgListNodeAB40* tmp = (sMsgListNodeAB40*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

// cBXString temporary (one pointer; non-POD, so it gets its own 16-byte stack slot).
struct cBXStrAB40 {
    char* str;
    cBXStrAB40() {}
    cBXStrAB40(const cBXStrAB40& o) : str(o.str) {}
};

/**
 * Posts a chat event carrying a sender and a text on mEventList.
 *
 * The event is 0x10 bytes from cMemMan_alloc ("ChatEvent"): type 222, vtable D_00481230, cBXString copies of @p sender at
 * +0x8 and @p text at +0xC. The chat handler LobbyMan_onchatmsg() calls it for room chat and broadcasts.
 *
 * @param sender  the sender's name
 * @param text  the message text
 */
extern "C" void func_0025AB40(LobbyMan* self, const char* sender, const char* text)
{
    int* event = (int*)operator new(0x10, str_ChatEvent, 0x20000000, 0);
    cBXStrAB40 sa;
    cBXString_cBXString2(&sa, sender);
    cBXStrAB40 sb;
    cBXString_cBXString2(&sb, text);
    event[0] = 222;
    *(void***)((char*)event + 0x4) = D_00481230;
    cBXString_cBXString1((char*)event + 0x8, &sa);
    cBXString_cBXString1((char*)event + 0xC, &sb);
    cBXString__cBXString(&sa, 2);
    cBXString__cBXString(&sb, 2);
    void* mp = event;
    sMsgListAB40_insert(*(sMsgListNodeAB40**)(((char*)self) + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025AC50);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];
extern void* online_BuddyErrorEvent_vtbl[];
extern void* D_004811B0[];
extern int D_004A3018;
extern float D_004A5154;
extern "C" void func_003E95C8(void* p);
extern "C" int func_003E8FB8(void* p);
extern "C" void func_0025A650(LobbyMan* self);
extern "C" void func_0025B800(LobbyMan* self);
extern "C" void func_003E8260(void* p, int a, int timeout, void* cb, void* user);
extern "C" void func_00268BA8(void* self, void* a1, int a2);
extern "C" char* func_003E8A70(void* p, int tag);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void func_00260F80(void);
extern "C" int func_00261460(void* p);
extern "C" void func_002613C8(BuddyMan* p);
extern "C" void func_00261408(BuddyMan* self);
extern "C" int func_00263128(BuddyMan* self, char* name);
extern "C" void func_00263018(BuddyMan* self, void* name);
extern "C" void func_0025F628(LobbyMan* selfp, int notify, int flag);
extern "C" void func_0025EED8(LobbyMan* self);
extern "C" int* func_002591D0();
extern "C" void func_00263BA8(void* p, int arg);
extern "C" int func_00255D38(void* comm, int chan);
extern "C" void func_00255CC8(void* comm, int chan);
extern "C" void func_003EE620(void* p);
extern "C" void func_00261490(BuddyMan* p);
extern "C" void func_0017D260(void* self, void* msg);
void operator_delete(int*);

struct sMsgListNodeAC50 {
    sMsgListNodeAC50* next;
    sMsgListNodeAC50* prev;
    void* data;
};

struct sMsgListIterAC50 {
    sMsgListNodeAC50* node;
    sMsgListIterAC50(sMsgListNodeAC50* x) : node(x) {}
    sMsgListIterAC50(const sMsgListIterAC50& x) : node(x.node) {}
};

static inline sMsgListIterAC50 sMsgListAC50_insert(sMsgListIterAC50 pos, void* const& x)
{
    sMsgListNodeAC50* tmp = (sMsgListNodeAC50*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsgListAC50 {
    int pad;
    sMsgListNodeAC50* node;
    sMsgListIterAC50 begin() { return node->next; }
    sMsgListIterAC50 end() { return node; }
    sMsgListIterAC50 erase(sMsgListIterAC50 pos)
    {
        sMsgListNodeAC50* next = pos.node->next;
        sMsgListNodeAC50* prev = pos.node->prev;
        prev->next = next;
        next->prev = prev;
        operator_delete((int*)pos.node);
        return next;
    }
    sMsgListNodeAC50* first() { return node->next; }
    void* front() { return begin().node->data; }
    void pop_front() { erase(begin()); }
    bool empty() { return begin().node == end().node; }
};

static inline void post_AC50(char* self, void* msg)
{
    void* mp = msg;
    sMsgListAC50_insert(*(sMsgListNodeAC50**)(self + 0xF0), mp);
}

static inline void* Mgr_AC50()
{
    if (BuddyMan_mBuddyMan == 0)
        func_00260F80();
    if (func_00261460(BuddyMan_mBuddyMan) == 0)
        func_002613C8(BuddyMan_mBuddyMan);
    if (BuddyMan_mBuddyMan == 0)
        func_0025B800(LobbyMan_mLobbyMan);
    return BuddyMan_mBuddyMan;
}

static inline int isNeg_AC50(float t)
{
    return t < 0.0f;
}

static inline int expired_AC50(char* self)
{
    return isNeg_AC50(*(float*)(self + 0xE0));
}

struct sDsCallbackAC50 {
    char pad[0x4];
    int value;
    int (*poll)(sDsCallbackAC50* self);
    void (*close)(sDsCallbackAC50* self);
};


struct sVEntryAC50 { short delta; short index; void (*fn)(void*, int); };

static inline void closeConn_AC50(char* self)
{
    sDsCallbackAC50* obj = *(sDsCallbackAC50**)(self + 0x58);
    if (obj != 0) {
        obj->close(obj);
        *(void**)(self + 0x58) = 0;
    }
}

/**
 * Runs the lobby manager once per frame: pumps the LobbyApi, advances the connection and request states, and delivers
 * queued events to the front end.
 *
 * With a LobbyApi: calls func_003E95C8() on it, reads its status with func_003E8FB8() and creates mpConfig when ready
 * (func_0025A650). If mDisconnected is set it clears it, disconnects (func_0025B800) and posts type 277. Otherwise, by
 * mState:
 * - 2 (host lookup): polls mpHostLookup; on success stores the address in mServerAddr and D_004A3018, enters mState 3
 *   with a 30-second mTimeout and connects to port 11000 (func_003E8260, reply thunk func_00268BA8); on failure
 *   disconnects and posts an error event (vtable online_ErrorEvent_vtbl, type 258) with 'dnsf'; either way releases the lookup.
 * - 3 (connecting): status 0x400 posts an error event (vtable D_004811B0) with 'down' and the server's 'down' text and
 *   disconnects; a timeout disconnects with error 'time' and releases the lookup.
 * - 26: a BuddyMan miError calls func_00261408() and posts it in an event (vtable online_BuddyErrorEvent_vtbl, type 259); a connected
 *   BuddyMan (func_00261460) sets mState 1 and posts type 218.
 * - 23 and 24 (challenge waits): lower D_004A5154 to mTimeout whenever it falls more than 1 below it; on timeout post
 *   type 274, then 23 removes mOpponent's pending challenge from BuddyMan and returns to mState 1, 24 calls
 *   func_0025F628(self, 0, 1).
 * - 20: on timeout posts type 230 and calls func_0025EED8().
 * - any other state but 22 and 25: when busy (not 1, or mFindingPlayer set) and timed out, disconnects, posts error
 *   'time', clears mFindingPlayer and releases the lookup.
 *
 * Outside mState 25 it also shuts down an active game connection (func_00263BA8(..., 0)) or else closes comm channel 0
 * if open. When mLoggedIn it refreshes the room and user lists (func_003EE620), and it counts mTimeout and unk_0E4 down
 * by the frame time. Always: updates BuddyMan (func_00261490), then pops every mEventList event, hands it to
 * func_0017D260() and deletes it through its virtual destructor (slot 1, flags 3). cRealFE_update calls it.
 */
extern "C" void func_0025AC50(LobbyMan* self)
{
    if (self->mpLobbyApi != 0) {
        func_003E95C8(self->mpLobbyApi);
        int ev = func_003E8FB8(self->mpLobbyApi);
        func_0025A650(self);
        if (self->mDisconnected != 0) {
            self->mDisconnected = 0;
            func_0025B800(self);
            int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            event[0] = 277;
            *(void***)((char*)event + 0x4) = online_Event_vtbl;
            post_AC50(((char*)self), event);
        } else {
            int state = *(int*)self;
            if (state == 2) {
                sDsCallbackAC50* obj = (sDsCallbackAC50*)self->mpHostLookup;
                int lookupStatus = obj->poll(obj);
                if (lookupStatus > 0) {
                    LobbyMan* sess = self;
                    int serverAddr = ((sDsCallbackAC50*)sess->mpHostLookup)->value;
                    sess->mServerAddr = serverAddr;
                    D_004A3018 = serverAddr;
                    sess->mState = 3;
                    sess->mTimeout = 30.0f;
                    // PORT: function pointer passed as a callback argument.
                    func_003E8260(((void*)sess->mpLobbyApi), serverAddr, 11000, (void*)func_00268BA8, ((char*)self));
                } else if (lookupStatus < 0) {
                    func_0025B800(self);
                    int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
                    event[0] = 258;
                    *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
                    event[2] = 0x646E7366; // 'dnsf'
                    post_AC50(((char*)self), event);
                }
                if (lookupStatus != 0)
                    closeConn_AC50(((char*)self));
            } else if (state == 3) {
                if (ev == 0x400) {
                    char* downReason = func_003E8A70(self->mpLobbyApi, 0x646F776E);
                    int* event = (int*)operator new(0x10, str_NetEvent, 0x20000000, 0);
                    event[0] = 258;
                    event[2] = 0x646F776E; // 'down'
                    *(void***)((char*)event + 0x4) = D_004811B0;
                    cBXString_cBXString2((char*)event + 0xC, downReason);
                    post_AC50(((char*)self), event);
                    func_0025B800(self);
                } else if (expired_AC50(((char*)self))) {
                    func_0025B800(self);
                    int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
                    event[0] = 258;
                    *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
                    event[2] = 0x74696D65; // 'time'
                    post_AC50(((char*)self), event);
                    closeConn_AC50(((char*)self));
                }
            } else if (state == 26) {
                if (BuddyMan_mBuddyMan->miError != 0) {
                    func_00261408(BuddyMan_mBuddyMan);
                    int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
                    int val = BuddyMan_mBuddyMan->miError;
                    event[0] = 259;
                    *(void***)((char*)event + 0x4) = online_BuddyErrorEvent_vtbl;
                    event[2] = val;
                    post_AC50(((char*)self), event);
                } else if (func_00261460(BuddyMan_mBuddyMan) != 0) {
                    *(int*)self = 1;
                    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
                    event[0] = 218;
                    *(void***)((char*)event + 0x4) = online_Event_vtbl;
                    post_AC50(((char*)self), event);
                }
            } else if (state == 22) {
            } else if (state == 23) {
                float timeout = self->mTimeout;
                if (timeout < D_004A5154 - 1.0f)
                    D_004A5154 = timeout;
                if (isNeg_AC50(timeout)) {
                    if (func_00263128((BuddyMan*)((char*)Mgr_AC50()), self->mOpponent) != 0)
                        func_00263018((BuddyMan*)Mgr_AC50(), self->mOpponent);
                    int type = 274;
                    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
                    event[0] = type;
                    *(void***)((char*)event + 0x4) = online_Event_vtbl;
                    post_AC50(((char*)self), event);
                    *(int*)self = 1;
                }
            } else if (state == 0x18) {
                float timeout = self->mTimeout;
                if (timeout < D_004A5154 - 1.0f)
                    D_004A5154 = timeout;
                if (isNeg_AC50(timeout)) {
                    int type = 274;
                    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
                    event[0] = type;
                    *(void***)((char*)event + 0x4) = online_Event_vtbl;
                    post_AC50(((char*)self), event);
                    func_0025F628(self, 0, 1);
                }
            } else if (state == 20) {
                if (expired_AC50(((char*)self))) {
                    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
                    event[0] = 230;
                    *(void***)((char*)event + 0x4) = online_Event_vtbl;
                    post_AC50(((char*)self), event);
                    func_0025EED8(self);
                }
            } else if (state != 25) {
                int notIdle = state != 1;
                int active = notIdle || self->mFindingPlayer != 0;
                if (active && expired_AC50(((char*)self))) {
                    func_0025B800(self);
                    int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
                    event[0] = 258;
                    *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
                    event[2] = 0x74696D65; // 'time'
                    post_AC50(((char*)self), event);
                    self->mFindingPlayer = 0;
                    closeConn_AC50(((char*)self));
                }
            }
        }
        int notJoined = *(int*)self != 25;
        if (notJoined) {
            if (*func_002591D0() != 0)
                func_00263BA8(func_002591D0(), 0);
            else if (func_00255D38(cCommSystem_mCommSystem, 0) != 0)
                func_00255CC8(cCommSystem_mCommSystem, 0);
        }
        if (self->mLoggedIn != 0) {
            func_003EE620(self->mpRoomList);
            func_003EE620(self->mpUserList);
        }
        char* app = (char*)SSXApp;
        self->mTimeout -= *(float*)(app + 0x14);
        self->unk_0E4 -= *(float*)(app + 0x14);
    }
    func_00261490(BuddyMan_mBuddyMan);
    while (((sMsgListAC50*)(((char*)self) + 0xEC))->first() != *(sMsgListNodeAC50**)(((char*)self) + 0xF0)) {
        void* msg = ((sMsgListAC50*)(((char*)self) + 0xEC))->front();
        ((sMsgListAC50*)(((char*)self) + 0xEC))->pop_front();
        func_0017D260(((char*)self), msg);
        if (msg != 0) {
            sVEntryAC50* vt = *(sVEntryAC50**)((char*)msg + 4);
            vt[1].fn((char*)msg + vt[1].delta, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025B608);
#ifdef SKIP_ASM
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);

/**
 * Starts a LobbyApi request: sets mState and mTimeout and stores the request handle in mRequestId.
 *
 * The request goes out through func_003E8B10(mpLobbyApi, tag, cb, callback, self). All the account, persona, room,
 * challenge and report requests use it with a 20-second timeout.
 *
 * @param newState  the state to enter (see LobbyMan::mState)
 * @param tag       the request's four-character code, e.g. 'acct'
 * @param cb        the request's tag record (the fields to send), despite the name
 * @param callback  the functor thunk that receives the reply, passed as an int
 * @param timeout   seconds before func_0025AC50() gives up (stored in mTimeout)
 */
extern "C" void func_0025B608(LobbyMan* self, int newState, int tag, void* cb, int callback, float timeout)
{
    *(int*)self = newState;
    self->mTimeout = timeout;
    self->mRequestId = func_003E8B10(self->mpLobbyApi, tag, cb, callback, self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025B650);
#ifdef SKIP_ASM
extern char str_LobbyMan[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* LobbyMan_LobbyMan(LobbyMan* self);

/**
 * Creates the lobby manager singleton: allocates 0x300 bytes ("LobbyMan") and constructs them with LobbyMan_LobbyMan() into
 * LobbyMan_mLobbyMan.
 *
 * An existing instance is overwritten without a check. The main menu and the debug menu screen call it.
 */
extern "C" void func_0025B650(void)
{
    LobbyMan_mLobbyMan = (LobbyMan*)(LobbyMan_LobbyMan((LobbyMan*)cMemMan_alloc(0x300, str_LobbyMan, 0x20000000, 0)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025B688);
#ifdef SKIP_ASM
extern "C" void func_0025B800(LobbyMan* self);
extern "C" void LobbyMan__LobbyMan(LobbyMan* self, int flags);
extern "C" void func_00256C50(void);

/**
 * Destroys the lobby manager singleton and the in-race comm object.
 *
 * When LobbyMan_mLobbyMan exists it disconnects it (func_0025B800), runs the destructor LobbyMan__LobbyMan() with flags 3
 * (free) and clears the pointer; then destroys cGameComm_mGameComm with func_00256C50() if there is one.
 */
extern "C" void func_0025B688(void)
{
    if (LobbyMan_mLobbyMan != 0) {
        func_0025B800(LobbyMan_mLobbyMan);
        if (LobbyMan_mLobbyMan != 0) {
            LobbyMan__LobbyMan(LobbyMan_mLobbyMan, 3);
        }
        LobbyMan_mLobbyMan = 0;
    }
    if (cGameComm_mGameComm != 0) {
        func_00256C50();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025B6D8);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern int D_004A3018;
extern "C" void func_0025A948(LobbyMan* self);
extern "C" void func_003E8260(void* p, int a, int timeout, void* cb, void* user);
extern "C" int func_003F3A98(void* p, int a);
extern "C" void func_00268BA8(void* self, void* a1, int a2);

struct sMsgListNodeB6D8 {
    sMsgListNodeB6D8* next;
    sMsgListNodeB6D8* prev;
    void* data;
};

struct sMsgListIterB6D8 {
    sMsgListNodeB6D8* node;
    sMsgListIterB6D8(sMsgListNodeB6D8* x) : node(x) {}
    sMsgListIterB6D8(const sMsgListIterB6D8& x) : node(x.node) {}
};

static inline sMsgListIterB6D8 sMsgListB6D8_insert(sMsgListIterB6D8 pos, void* const& x)
{
    sMsgListNodeB6D8* tmp = (sMsgListNodeB6D8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Starts connecting the lobby manager to the lobby server.
 *
 * Creates the LobbyApi first if needed (func_0025A948) and stores @p conn in unk_014. With @p conn set it reuses the
 * address cached in D_004A3018: enters mState 3 with a 30-second mTimeout, stores mServerAddr and connects on port
 * 11000 (func_003E8260, reply thunk func_00268BA8). Otherwise it enters mState 2 and starts a host lookup of
 * mServerName with func_003F3A98() into mpHostLookup (func_0025AC50() polls it). Either way posts a type-200 event.
 *
 * @param conn  nonzero to connect to the cached server address, 0 to look the server name up first
 */
extern "C" void func_0025B6D8(LobbyMan* self, int conn)
{
    if (self->mpLobbyApi == 0)
        func_0025A948(self);
    self->unk_014 = conn;
    if (conn != 0) {
        int serverAddr = D_004A3018;
        *(int*)self = 3;
        self->mTimeout = 30.0f;
        self->mServerAddr = serverAddr;
        // PORT: function pointer passed as a callback argument.
        func_003E8260(self->mpLobbyApi, serverAddr, 11000, (void*)func_00268BA8, ((char*)self));
    } else {
        *(int*)self = 2;
        self->mpHostLookup = (void*)func_003F3A98(self->mServerName, 0);
    }
    int type = 200;
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListB6D8_insert(*(sMsgListNodeB6D8**)(((char*)self) + 0xF0), mp);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025B800);
#ifdef SKIP_ASM
extern "C" void func_0025A6F8(LobbyMan* self);
extern "C" void func_0025AA98(LobbyMan* self);

/**
 * Disconnects the lobby manager: frees mpConfig (func_0025A6F8), shuts the LobbyApi down if there is one
 * (func_0025AA98) and clears mAuthenticated, mLoggedIn and mState.
 */
extern "C" void func_0025B800(LobbyMan* self)
{
    func_0025A6F8(self);
    if (self->mpLobbyApi != 0) {
        func_0025AA98(self);
    }
    self->mAuthenticated = 0;
    self->mLoggedIn = 0;
    self->mState = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onlobbyconnect);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern char str_ROOMS_1_USERS_1_RANKS_0_MESGS_1_GAMES_1[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeB848 {
    sMsgListNodeB848* next;
    sMsgListNodeB848* prev;
    void* data;
};

struct sMsgListIterB848 {
    sMsgListNodeB848* node;
    sMsgListIterB848(sMsgListNodeB848* x) : node(x) {}
    sMsgListIterB848(const sMsgListIterB848& x) : node(x.node) {}
};

static inline sMsgListIterB848 sMsgListB848_insert(sMsgListIterB848 pos, void* const& x)
{
    sMsgListNodeB848* tmp = (sMsgListNodeB848*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_B848(char* self, void* msg)
{
    void* mp = msg;
    sMsgListB848_insert(*(sMsgListNodeB848**)(self + 0xF0), mp);
}

/**
 * Handles the lobby server's connect reply.
 *
 * Sets mState to 1. On success (reply code at +0x8 is 0) it sends a 'sele' request with "ROOMS=1 USERS=1 RANKS=0
 * MESGS=1 GAMES=1" (no reply callback) and posts a type-201 event; otherwise it posts an error event (vtable
 * online_ErrorEvent_vtbl, type 258) carrying the code and sets mDisconnected. Called through the functor thunk func_00268BA8, which
 * func_0025B6D8() and func_0025AC50() pass to func_003E8260().
 *
 * @param msg  the LobbyApi reply (error code at +0x8, 0 on success)
 */
extern "C" void LobbyMan_onlobbyconnect(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        func_003E8B10(self->mpLobbyApi, 0x73656C65, str_ROOMS_1_USERS_1_RANKS_0_MESGS_1_GAMES_1, 0, 0);
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 201;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_B848(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_B848(((char*)self), event);
        self->mDisconnected = 1;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025B9C8);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_pct_4dpct_02dpct_02d[];
extern char str_NAME[];
extern char str_LANG[];
extern char str_pct_cpct_c[];
extern char str_pct_c[];
extern char str_PASS[];
extern char str_SPAM[];
extern char str_MAIL[];
extern char str_PMAIL[];
extern char str_GEND[];
extern char str_BORN[];
extern char str_DEFPER[];
extern char str_ALTS[];
extern char str_MINAGE[];
extern "C" void func_00268BD0(void* self, void* a1, int a2);
extern "C" int cDirtysock_tag_TagFieldSetNumber(char* buf, int size, const char* key, int value);
extern "C" const char* func_00259198();

struct sMsgListNodeB9C8 {
    sMsgListNodeB9C8* next;
    sMsgListNodeB9C8* prev;
    void* data;
};

struct sMsgListIterB9C8 {
    sMsgListNodeB9C8* node;
    sMsgListIterB9C8(sMsgListNodeB9C8* x) : node(x) {}
    sMsgListIterB9C8(const sMsgListIterB9C8& x) : node(x.node) {}
};

static inline sMsgListIterB9C8 sMsgListB9C8_insert(sMsgListIterB9C8 pos, void* const& x)
{
    sMsgListNodeB9C8* tmp = (sMsgListNodeB9C8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Sends a create-account ('acct') request built from an account form.
 *
 * Fills a 0x200-byte record: NAME (mName), PASS (mPassword), SPAM ("%c%c", 'Y'/'N' from mSpam[0] and mSpam[1]), MAIL
 * (mMail), PMAIL (mParentMail, only when non-empty), GEND ('F' when mGender is set, else 'M'), BORN ("%4d%02d%02d" from
 * mBornYear, mBornMonth, mBornDay), DEFPER 0, ALTS 4, MINAGE 13 and LANG (func_00259198); sends it with func_0025B608()
 * (mState 4, reply thunk func_00268BD0, 20 s) and posts a type-202 event. The account screens pass the lobby manager's
 * mNewAccount.
 *
 * @param msg  the account form (a tLobbyAccount)
 */
extern "C" void func_0025B9C8(LobbyMan* self, char* msg)
{
    char buf[0x200];
    char yn[16];
    char mf[16];
    char ver[16];
    sprintf(yn, str_pct_cpct_c, *(int*)(msg + 0x38) ? 'Y' : 'N', *(int*)(msg + 0x3C) ? 'Y' : 'N');
    sprintf(mf, str_pct_c, *(int*)(msg + 0x34) ? 'F' : 'M');
    sprintf(ver, str_pct_4dpct_02dpct_02d, *(int*)(msg + 0x30), *(int*)(msg + 0x28), *(int*)(msg + 0x2C));
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_NAME, *(const char**)(msg + 0x0));
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PASS, *(const char**)(msg + 0x4));
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_SPAM, yn);
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_MAIL, *(const char**)(msg + 0xC));
    const char* email = *(const char**)(msg + 0x10);
    // PORT: string length read from the header before the characters.
    if (*(int*)((int)email - 8) != 0)
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PMAIL, email);
    int type = 202;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_GEND, mf);
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_BORN, ver);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x200, str_DEFPER, 0);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x200, str_ALTS, 4);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x200, str_MINAGE, 13);
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_LANG, func_00259198());
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 4, 0x61636374, buf, (int)func_00268BD0, 20.0f);
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListB9C8_insert(*(sMsgListNodeB9C8**)(((char*)self) + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025BC48);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern char str_OPTS[];
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" void* cBXString_operatorE1(void* self, const char* str);

struct sDsNameListK2 {
    char pad_0x00[0x274];
    char* names[4];   // 0x274 (cBXString)
    int count;        // 0x284
};

/**
 * Splits the reply's "OPTS" field (a comma-separated list of suggested names) into mNewAccount.mNameOptions.
 *
 * Resets mNewAccount.mNumNameOptions and stores up to 4 names as cBXStrings. The create-account reply handler
 * LobbyMan_onaccountcreated() calls it when the name is taken ('dupl'), and LobbyMan_onpersonacreated() calls it too.
 *
 * @param msg  the LobbyApi reply (tag record pointer at +0xC)
 */
// Splits a comma-separated tag field into up to 4 strings.
extern "C" void func_0025BC48(LobbyMan* self, char* msg)
{
    char buf[256];
    char* cursor;
    char* start;
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_OPTS), buf, 256, D_004A2F40);
    self->mNewAccount.mNumNameOptions = 0;
    cursor = buf;
    while ((*cursor != 0) && (self->mNewAccount.mNumNameOptions < 4)) {
        start = cursor;
        while ((*cursor != 0) && (*cursor != ','))
            cursor++;
        if (*cursor != 0)
            *cursor++ = 0;
        cBXString_operatorE1(&self->mNewAccount.mNameOptions[self->mNewAccount.mNumNameOptions++], start);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onaccountcreated);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_AcctTooYoungEvent_vtbl[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];
extern char str_AGE[];
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);
struct sDsNameListK2;
extern "C" void func_0025BC48(LobbyMan* self, char* msg);

struct sMsgListNodeBD30 {
    sMsgListNodeBD30* next;
    sMsgListNodeBD30* prev;
    void* data;
};

struct sMsgListIterBD30 {
    sMsgListNodeBD30* node;
    sMsgListIterBD30(sMsgListNodeBD30* x) : node(x) {}
    sMsgListIterBD30(const sMsgListIterBD30& x) : node(x.node) {}
};

static inline sMsgListIterBD30 sMsgListBD30_insert(sMsgListIterBD30 pos, void* const& x)
{
    sMsgListNodeBD30* tmp = (sMsgListNodeBD30*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_BD30(char* self, void* msg)
{
    void* mp = msg;
    sMsgListBD30_insert(*(sMsgListNodeBD30**)(self + 0xF0), mp);
}

/**
 * Handles the create-account reply.
 *
 * Sets mState to 1. Success (code 0 at +0x8) posts a type-203 event. 'tooy' (too young) posts a 0x10-byte error event
 * (vtable online_AcctTooYoungEvent_vtbl, type 258) with the code and the reply's "AGE" number. Any other code posts an error event (vtable
 * online_ErrorEvent_vtbl, type 258) with the code, after loading the suggested names (func_0025BC48) when the code is 'dupl'.
 * Called through the functor thunk func_00268BD0, the reply callback of func_0025B9C8().
 *
 * @param msg  the LobbyApi reply (error code at +0x8, tag record pointer at +0xC)
 */
extern "C" void LobbyMan_onaccountcreated(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    int err = *(int*)(msg + 0x8);
    if (err == 0) {
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 203;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_BD30(((char*)self), event);
    } else if (err == 0x746F6F79) {
        int n = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_AGE), 0);
        int* event = (int*)operator new(0x10, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        event[2] = err;
        *(void***)((char*)event + 0x4) = online_AcctTooYoungEvent_vtbl;
        event[3] = n;
        post_BD30(((char*)self), event);
    } else {
        if (err == 0x6475706C)
            func_0025BC48(self, msg);
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_BD30(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025BF18);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_NAME[];
extern char str_pct_cpct_c[];
extern char str_PASS[];
extern char str_SPAM[];
extern char str_MAIL[];
extern char str_CHNG[];
extern "C" void func_00268BF8(void* self, void* a1, int a2);

struct sMsgListNodeBF18 {
    sMsgListNodeBF18* next;
    sMsgListNodeBF18* prev;
    void* data;
};

struct sMsgListIterBF18 {
    sMsgListNodeBF18* node;
    sMsgListIterBF18(sMsgListNodeBF18* x) : node(x) {}
    sMsgListIterBF18(const sMsgListIterBF18& x) : node(x.node) {}
};

static inline sMsgListIterBF18 sMsgListBF18_insert(sMsgListIterBF18 pos, void* const& x)
{
    sMsgListNodeBF18* tmp = (sMsgListNodeBF18*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Sends an edit-account ('edit') request with the fields an account form marks as changed.
 *
 * NAME is always mUserName. With mChangePassword set it adds PASS (the lobby manager's current mPassword) and CHNG (the
 * form's mPassword, the new one); with mChangeSpam, SPAM ("%c%c" from mSpam); with mChangeMail, MAIL (mMail). Sends it
 * with func_0025B608() (mState 5, reply thunk func_00268BF8, 20 s) and posts a type-205 event.
 *
 * @param msg  the account form (a tLobbyAccount)
 */
extern "C" void func_0025BF18(LobbyMan* self, char* msg)
{
    char buf[0x200];
    char spamText[16];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_NAME, (const char*)self->mUserName);
    if (*(int*)(msg + 0x44) != 0) {
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PASS, (const char*)self->mPassword);
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_CHNG, *(const char**)(msg + 0x4));
    }
    if (*(int*)(msg + 0x48) != 0) {
        sprintf(spamText, str_pct_cpct_c, *(int*)(msg + 0x38) ? 'Y' : 'N', *(int*)(msg + 0x3C) ? 'Y' : 'N');
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_SPAM, spamText);
    }
    if (*(int*)(msg + 0x4C) != 0) {
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_MAIL, *(const char**)(msg + 0xC));
    }
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 5, 0x65646974, buf, (int)func_00268BF8, 20.0f);
    int type = 205;
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListBF18_insert(*(sMsgListNodeBF18**)(((char*)self) + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onaccountedited);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeC0C0 {
    sMsgListNodeC0C0* next;
    sMsgListNodeC0C0* prev;
    void* data;
};

struct sMsgListIterC0C0 {
    sMsgListNodeC0C0* node;
    sMsgListIterC0C0(sMsgListNodeC0C0* x) : node(x) {}
    sMsgListIterC0C0(const sMsgListIterC0C0& x) : node(x.node) {}
};

static inline sMsgListIterC0C0 sMsgListC0C0_insert(sMsgListIterC0C0 pos, void* const& x)
{
    sMsgListNodeC0C0* tmp = (sMsgListNodeC0C0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_C0C0(char* self, void* msg)
{
    void* mp = msg;
    sMsgListC0C0_insert(*(sMsgListNodeC0C0**)(self + 0xF0), mp);
}

/**
 * Handles the edit-account reply: sets mState to 1 and posts a type-206 event on success, else an error event (vtable
 * online_ErrorEvent_vtbl, type 258) with the reply code.
 *
 * Called through the functor thunk func_00268BF8, the reply callback of func_0025BF18().
 *
 * @param msg  the LobbyApi reply (error code at +0x8, 0 on success)
 */
extern "C" void LobbyMan_onaccountedited(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 206;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_C0C0(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_C0C0(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025C1E0);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_MAIL[];
extern "C" void func_00268C20(void* self, void* a1, int a2);

struct sMsgListNodeC1E0 {
    sMsgListNodeC1E0* next;
    sMsgListNodeC1E0* prev;
    void* data;
};

struct sMsgListIterC1E0 {
    sMsgListNodeC1E0* node;
    sMsgListIterC1E0(sMsgListNodeC1E0* x) : node(x) {}
    sMsgListIterC1E0(const sMsgListIterC1E0& x) : node(x.node) {}
};

static inline sMsgListIterC1E0 sMsgListC1E0_insert(sMsgListIterC1E0 pos, void* const& x)
{
    sMsgListNodeC1E0* tmp = (sMsgListNodeC1E0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Sends a lost-account ('lost') request that looks the account up by e-mail address.
 *
 * Sends MAIL = @p value with func_0025B608() (mState 6, reply thunk func_00268C20, 20 s) and posts a type-209 event.
 * The account screen sends the typed e-mail address.
 *
 * @param value  the e-mail address
 */
extern "C" void func_0025C1E0(LobbyMan* self, const char* value)
{
    char buf[0x200];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_MAIL, value);
    int type = 209;
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 6, 0x6C6F7374, buf, (int)func_00268C20, 20.0f);
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListC1E0_insert(*(sMsgListNodeC1E0**)(((char*)self) + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onlostaccountrequestsent);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeC2D8 {
    sMsgListNodeC2D8* next;
    sMsgListNodeC2D8* prev;
    void* data;
};

struct sMsgListIterC2D8 {
    sMsgListNodeC2D8* node;
    sMsgListIterC2D8(sMsgListNodeC2D8* x) : node(x) {}
    sMsgListIterC2D8(const sMsgListIterC2D8& x) : node(x.node) {}
};

static inline sMsgListIterC2D8 sMsgListC2D8_insert(sMsgListIterC2D8 pos, void* const& x)
{
    sMsgListNodeC2D8* tmp = (sMsgListNodeC2D8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_C2D8(char* self, void* msg)
{
    void* mp = msg;
    sMsgListC2D8_insert(*(sMsgListNodeC2D8**)(self + 0xF0), mp);
}

/**
 * Handles the lost-account reply: sets mState to 1 and posts a type-207 event on success, else an error event (vtable
 * online_ErrorEvent_vtbl, type 258) with the reply code.
 *
 * Called through the functor thunk func_00268C20, the reply callback of func_0025C1E0().
 *
 * @param msg  the LobbyApi reply (error code at +0x8, 0 on success)
 */
extern "C" void LobbyMan_onlostaccountrequestsent(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 207;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_C2D8(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_C2D8(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025C3F8);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_NAME[];
extern "C" void func_00268C48(void* self, void* a1, int a2);

struct sMsgListNodeC3F8 {
    sMsgListNodeC3F8* next;
    sMsgListNodeC3F8* prev;
    void* data;
};

struct sMsgListIterC3F8 {
    sMsgListNodeC3F8* node;
    sMsgListIterC3F8(sMsgListNodeC3F8* x) : node(x) {}
    sMsgListIterC3F8(const sMsgListIterC3F8& x) : node(x.node) {}
};

static inline sMsgListIterC3F8 sMsgListC3F8_insert(sMsgListIterC3F8 pos, void* const& x)
{
    sMsgListNodeC3F8* tmp = (sMsgListNodeC3F8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Sends a lost-password ('lost') request for an account name.
 *
 * Sends NAME = @p value with func_0025B608() (mState 7, reply thunk func_00268C48, 20 s) and posts a type-208 event.
 * The account screen sends mUserName.
 *
 * @param value  the account name
 */
extern "C" void func_0025C3F8(LobbyMan* self, const char* value)
{
    char buf[0x200];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_NAME, value);
    int type = 208;
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 7, 0x6C6F7374, buf, (int)func_00268C48, 20.0f);
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListC3F8_insert(*(sMsgListNodeC3F8**)(((char*)self) + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onlostpasswordrequestsent);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeC4F0 {
    sMsgListNodeC4F0* next;
    sMsgListNodeC4F0* prev;
    void* data;
};

struct sMsgListIterC4F0 {
    sMsgListNodeC4F0* node;
    sMsgListIterC4F0(sMsgListNodeC4F0* x) : node(x) {}
    sMsgListIterC4F0(const sMsgListIterC4F0& x) : node(x.node) {}
};

static inline sMsgListIterC4F0 sMsgListC4F0_insert(sMsgListIterC4F0 pos, void* const& x)
{
    sMsgListNodeC4F0* tmp = (sMsgListNodeC4F0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_C4F0(char* self, void* msg)
{
    void* mp = msg;
    sMsgListC4F0_insert(*(sMsgListNodeC4F0**)(self + 0xF0), mp);
}

/**
 * Handles the lost-password reply: sets mState to 1 and posts a type-210 event on success, else an error event (vtable
 * online_ErrorEvent_vtbl, type 258) with the reply code.
 *
 * Called through the functor thunk func_00268C48, the reply callback of func_0025C3F8().
 *
 * @param msg  the LobbyApi reply (error code at +0x8, 0 on success)
 */
extern "C" void LobbyMan_onlostpasswordrequestsent(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 210;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_C4F0(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_C4F0(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025C610);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_SSX_PS2_2004[];
extern char str_NAME[];
extern char str_PASS[];
extern char str_TOS[];
extern char str_MID[];
extern char str_PID[];
extern char str_HWFLAG[];
extern char str_HWMASK[];
extern void* D_004A33F0;
extern void* D_004A33F4;
extern "C" void func_00268C70();
extern "C" int cDirtysock_tag_TagFieldSetNumber(char* buf, int size, const char* key, int value);
extern "C" const char* func_003F3F78();
void* func_00266F38(void* self);

struct sMsgListNodeC610 {
    sMsgListNodeC610* next;
    sMsgListNodeC610* prev;
    void* data;
};

struct sMsgListIterC610 {
    sMsgListNodeC610* node;
    sMsgListIterC610(sMsgListNodeC610* x) : node(x) {}
    sMsgListIterC610(const sMsgListIterC610& x) : node(x.node) {}
};

static inline sMsgListIterC610 sMsgListC610_insert(sMsgListIterC610 pos, void* const& x)
{
    sMsgListNodeC610* tmp = (sMsgListNodeC610*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_C610(char* lst, void* msg)
{
    void* mp = msg;
    sMsgListC610_insert(*(sMsgListNodeC610**)(lst + 4), mp);
}

struct sDsSlotC610 {
    char pad[0x88];
    int flags;
};

/**
 * Sends the lobby login ('auth') request with the account name, password and hardware flags.
 *
 * Builds a 0x200-byte tag record: NAME = mUserName, PASS = mPassword, TOS = 1, MID = func_003F3F78()'s hex string (from
 * the 'macx' query), PID = "SSX-PS2-2004", then HWFLAG and HWMASK. When cCommSystem_mCommSystem->mInterfaceType is not
 * 1, HWFLAG gets its bit 0x4, 0x10000 for bit 0x10 and 0x20 for bit 0x8, and HWMASK starts at 0x10024; HWMASK always
 * adds 0x800 and 0x100. HWFLAG adds 0x800 when bit 0 of the word at +0x88 is set in any of 8 0x8C-byte records of the
 * USB keyboard object D_004A33F0 (read with no null check, unlike the other D_004A33F0 users), and 0x100 when the
 * voice-chat manager D_004A33F4's status (func_00266F38) has bit 0x10. func_0025B608() then sends it (mState 8, 20 s
 * timeout, reply through func_00268C70 to LobbyMan_onauthentication()), and an Event of type 211 (vtable online_Event_vtbl) is pushed onto
 * mEventList.
 */
extern "C" void func_0025C610(LobbyMan* self)
{
    char buf[0x200];
    int flags = 0;
    int caps = 0;
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_NAME, (const char*)self->mUserName);
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PASS, (const char*)self->mPassword);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x200, str_TOS, 1);
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_MID, func_003F3F78());
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PID, str_SSX_PS2_2004);
    int interfaceType = cCommSystem_mCommSystem->mInterfaceType;
    if (interfaceType != 1) {
        flags = interfaceType & 4;
        caps = 0x10024;
        if (interfaceType & 0x10)
            flags |= 0x10000;
        if (interfaceType & 8)
            flags |= 0x20;
    }
    caps |= 0x800;
    int i = 0;
    void* conn = D_004A33F4;

    for (; i < 8; i++) {
        if (((sDsSlotC610*)D_004A33F0)[i].flags & 1) {
            flags |= 0x800;
            break;
        }
    }
    caps |= 0x100;
    if ((int)func_00266F38(conn) & 0x10)
        flags |= 0x100;
    cDirtysock_tag_TagFieldSetNumber(buf, 0x200, str_HWFLAG, flags);
    int type = 211;
    cDirtysock_tag_TagFieldSetNumber(buf, 0x200, str_HWMASK, caps);
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 8, 0x61757468, buf, (int)func_00268C70, 20.0f);
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    post_C610(((char*)self) + 0xEC, event);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025C878);
#ifdef SKIP_ASM
extern "C" void func_003E8D70(void* p, int a);

/**
 * Cancels a pending login: when mState is 8 (authenticating), passes mRequestId to func_003E8D70() on mpLobbyApi.
 *
 * mState itself is left unchanged.
 */
extern "C" void func_0025C878(LobbyMan* self)
{
    if (self->mState == 8) {
        func_003E8D70(self->mpLobbyApi, self->mRequestId);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onauthentication);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: identical if/else arms around the b3 GetString block change GCSE's table size and so the spill order (permuter); plain form 99.98%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.

extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" unsigned int strlen(const char*);
extern "C" void func_003EFD30(const char* s, int len, void* out, int n);
extern char D_004A2F40[];
extern char str_PERSONAS[];
extern char str_MAIL[];
extern char str_SPAM[];
extern char D_004A30F0[];
extern char str_DATE[];
extern char str_INTRO[];
extern void* D_004811D0_C8A8[] __asm__("online_AcctLockedEvent_vtbl");
extern void* online_ErrorEvent_vtbl[];
static inline void post_C8A8(char* self, void* msg)
{
    void* mp = msg;
    sMsgListC610_insert(*(sMsgListNodeC610**)(self + 0xF0), mp);
}
struct sBXStrC8A8 { char* p; };




/**
 * Handles the lobby server's reply to the login ('auth') request.
 *
 * Sets mState to 1. On success (status word +0x8 of @p msg is 0) it splits the comma list in "PERSONAS" into mPersonas
 * (at most 4, counted in mNumPersonas), stores "MAIL" in mAccount.mMail and the two 'Y'/'N' letters of "SPAM" in
 * mAccount.mSpam, copies mUserName to mAccount.mName and mPassword to mAccount.mPassword and mAccount.unk_08, decodes
 * "CPAT" into mCPAT with func_003EFD30() (setting mHaveCPAT, or clearing it when the field is missing), sets
 * mAuthenticated and pushes an Event of type 212 onto mEventList. Status 'lock' pushes a type-258 event with vtable
 * online_AcctLockedEvent_vtbl holding the code and the "DATE" and "INTRO" strings; any other status pushes a type-258 error event
 * (vtable online_ErrorEvent_vtbl) carrying the status. Called only from the LobbyApi callback thunk func_00268C70.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onauthentication(LobbyMan* self, char *msg)
{
  *((int *) self) = 1;
  int status = *((int *) (msg + 8));
  if (status == 0)
  {
    char name[0x100];
    char mail[0x100];
    char spam[0x10];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*((char **) (msg + 0xC)), str_PERSONAS), name, 0x100, D_004A2F40);
    char *cursor = name;
    (self)->mNumPersonas = 0;
    while (((*cursor) != 0) && ((self)->mNumPersonas < 4))
    {
      char *start = cursor;
      while (((*cursor) != 0) && ((*cursor) != ','))
      {
        cursor++;
      }

      if ((*cursor) != 0)
      {
        *cursor = 0;
        cursor++;
      }
      cBXString_operatorE1(&((sBXStrC8A8*)&(self)->mPersonas[0])[(self)->mNumPersonas++], start);
    }

    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*((char **) (msg + 0xC)), str_MAIL), mail, 0x100, D_004A2F40);
    cBXString_operatorE1(((char*)self) + 0x2BC, mail);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*((char **) (msg + 0xC)), str_SPAM), spam, 3, D_004A2F40);
    self->mAccount.mSpam[0] = spam[0] == 'Y';
    self->mAccount.mSpam[1] = spam[1] == 'Y';
    cBXString_operatorE(((char*)self) + 0x2B0, ((char*)self) + 0x48);
    cBXString_operatorE(((char*)self) + 0x2B4, ((char*)self) + 0x4C);
    cBXString_operatorE(((char*)self) + 0x2B8, ((char*)self) + 0x4C);
    const char *cpatField = cDirtysock_tag_TagFieldFind(*((char **) (msg + 0xC)), D_004A30F0);
    if (cpatField != 0)
    {
      char cpatText[0x100];
      if (cursor)
      {
        cDirtysock_tag_TagFieldGetString4(cpatField, cpatText, 0x100, D_004A2F40);
        func_003EFD30(cpatText, strlen(cpatText), ((char*)self) + 0x1C, 33);
        self->mHaveCPAT = 1;
      }
      else
      {
        cDirtysock_tag_TagFieldGetString4(cpatField, cpatText, 0x100, D_004A2F40);
        func_003EFD30(cpatText, strlen(cpatText), ((char*)self) + 0x1C, 33);
        self->mHaveCPAT = 1;
      }
    }
    else
    {
      self->mHaveCPAT = 0;
    }
    self->mAuthenticated = 1;
    int *event = (int *) operator new(8, str_NetEvent, 0x20000000, 0);
    *((void ***) (((char *) event) + 0x4)) = online_Event_vtbl;
    event[0] = 212;
    post_C610(((char*)self) + 0xEC, event);
  }
  else
    if (status == 0x6C6F636B)
  {
    char lockDate[0x100];
    char intro[0x100];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*((char **) (msg + 0xC)), str_DATE), lockDate, 0x100, D_004A2F40);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*((char **) (msg + 0xC)), str_INTRO), intro, 0x100, D_004A2F40);
    int *event = (int *) operator new(20, str_NetEvent, 0x20000000, 0);
    event[0] = 258;
    *((void ***) (((char *) event) + 0x4)) = D_004811D0_C8A8;
    event[2] = 0x6C6F636B;
    cBXString_cBXString2(event + 3, lockDate);
    cBXString_cBXString2(event + 4, intro);
    post_C8A8(((char*)self), event);
  }
  else
  {
    int *event = (int *) operator new(0xC, str_NetEvent, 0x20000000, 0);
    event[2] = *((int *) (msg + 8));
    event[0] = 258;
    *((void ***) (((char *) event) + 0x4)) = online_ErrorEvent_vtbl;
    post_C8A8(((char*)self), event);
  }
}
#endif

extern "C" void* func_0025CD50(void*, int);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025CD28__FPvi);
#ifdef SKIP_ASM
/**
 * Selects one of the account's personas by index: passes mPersonas[@p personaIndex] to func_0025CD50().
 *
 * @param personaIndex  index into mPersonas (0..3); not range-checked
 * @return no meaningful value (func_0025CD50() returns nothing)
 */
void* func_0025CD28(void* self, int personaIndex)
{
    return func_0025CD50(self, *(int*)((char*)((char*)self + personaIndex * 4) + 0x70));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025CD50);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern char str_PERS[];
extern char str_MID[];
extern char str_PID[];
extern char str_CDEV[];
extern char str_SSX_PS2_2004[];
extern "C" const char* func_003F3F78();
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void func_0025B608(LobbyMan* self, int a1, int tag, void* cb, int a4, float f);
extern "C" void func_00268C98(void* self, void* a1, int a2);
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeCD50 {
    sMsgListNodeCD50* next;
    sMsgListNodeCD50* prev;
    void* data;
};

struct sMsgListIterCD50 {
    sMsgListNodeCD50* node;
    sMsgListIterCD50(sMsgListNodeCD50* x) : node(x) {}
    sMsgListIterCD50(const sMsgListIterCD50& x) : node(x.node) {}
};

static inline sMsgListIterCD50 sMsgListCD50_insert(sMsgListIterCD50 pos, void* const& x)
{
    sMsgListNodeCD50* tmp = (sMsgListNodeCD50*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

// PORT: the unit declares func_0025CD50 as void* (void*, int) (a caller tail-calls it); the body takes (self, name).
extern "C" void func_0025CD50_impl(char* self, const char* name) __asm__("func_0025CD50");

/**
 * Sends the persona selection ('pers') request and remembers the chosen persona.
 *
 * Assigns @p name to mPersona, then builds PERS = @p name, MID = func_003F3F78()'s hex string, PID = "SSX-PS2-2004"
 * and, when mHaveCPAT is set, CDEV = mCPAT. func_0025B608() sends it (mState 9, 20 s timeout, reply through
 * func_00268C98 to LobbyMan_onpersonaselected()) and an Event of type 217 is pushed onto mEventList. Unlike most requests it does
 * not check mState first.
 *
 * @param name  the persona name
 */
extern "C" void func_0025CD50_impl(char* self, const char* name)
{
    cBXString_operatorE1(self + 0x80, name);
    char buf[0x200];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PERS, name);
    const char* key = str_MID;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, key, func_003F3F78());
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PID, str_SSX_PS2_2004);
    if (*(int*)(self + 0x18) != 0)
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_CDEV, self + 0x1C);
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608((LobbyMan*)self, 9, 0x70657273, buf, (int)func_00268C98, 20.0f);
    int type = 217;
    int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    event[0] = type;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListCD50_insert(*(sMsgListNodeCD50**)(self + 0xF0), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onpersonaselected);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];
extern "C" void func_0025CFE0(LobbyMan* self);

struct sMsgListNodeCEB0 {
    sMsgListNodeCEB0* next;
    sMsgListNodeCEB0* prev;
    void* data;
};

struct sMsgListIterCEB0 {
    sMsgListNodeCEB0* node;
    sMsgListIterCEB0(sMsgListNodeCEB0* x) : node(x) {}
    sMsgListIterCEB0(const sMsgListIterCEB0& x) : node(x.node) {}
};

static inline sMsgListIterCEB0 sMsgListCEB0_insert(sMsgListIterCEB0 pos, void* const& x)
{
    sMsgListNodeCEB0* tmp = (sMsgListNodeCEB0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_CEB0(char* self, void* msg)
{
    void* mp = msg;
    sMsgListCEB0_insert(*(sMsgListNodeCEB0**)(self + 0xF0), mp);
}

/**
 * Handles the lobby server's reply to the persona selection ('pers') request.
 *
 * Sets mState to 1. On success it requests the persona's online stats with func_0025CFE0() and pushes an Event of type
 * 218 onto mEventList; otherwise it pushes a type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. Called only
 * from the LobbyApi callback thunk func_00268C98.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onpersonaselected(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        func_0025CFE0(self);
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 218;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_CEB0(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_CEB0(((char*)self), event);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025CFE0);
#ifdef SKIP_ASM
extern char str_PERS[];
extern "C" int cDirtysock_tag_TagFieldSetString(char* buf, int size, const char* key, const char* value);
extern "C" void func_0025B608(LobbyMan* self, int a1, int tag, void* cb, int a4, float f);
extern "C" void func_00268CC0(void* self, void* a1, int a2);

/**
 * Requests the current persona's online stats: sends 'onln' with PERS = mPersona.
 *
 * func_0025B608() sends it with mState 27 and a 20 s timeout; the reply goes through func_00268CC0 to func_0025D048().
 */
extern "C" void func_0025CFE0(LobbyMan* self)
{
    char buf[0x200];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PERS, (const char*)self->mPersona);
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 27, 0x6F6E6C6E, buf, (int)func_00268CC0, 20.0f);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025D048);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldGetStructure(const char* tag, void* data, int len, const char* fmt);
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern char str_STAT[];
extern char D_004A3130[];
extern char D_004A2F40[];
extern char str_l[];
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" void func_0025E0C8(LobbyMan* self, int value);
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeD048 {
    sMsgListNodeD048* next;
    sMsgListNodeD048* prev;
    void* data;
};

struct sMsgListIterD048 {
    sMsgListNodeD048* node;
    sMsgListIterD048(sMsgListNodeD048* x) : node(x) {}
    sMsgListIterD048(const sMsgListIterD048& x) : node(x.node) {}
};

static inline sMsgListIterD048 sMsgListD048_insert(sMsgListIterD048 pos, void* const& x)
{
    sMsgListNodeD048* tmp = (sMsgListNodeD048*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_D048(char* self, void* msg)
{
    void* mp = msg;
    sMsgListD048_insert(*(sMsgListNodeD048**)(self + 0xF0), mp);
}

/**
 * Handles the reply to the persona's 'onln' stats request and fills mStats.
 *
 * Sets mState to 1. On success it decodes the "STAT" field with the "l*" template (cDirtysock_tag_TagFieldGetStructure)
 * and stores mStats.unk_04 = item 2, mStats.unk_08 = items 2 + 3 + 4, mStats.unk_0C = item 6, mStats.mRank = the "R"
 * number (default 0) and mStats.unk_10 = -1, then requests news item 0 with func_0025E0C8() (no event is posted).
 * Otherwise it pushes a type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. Called only from the LobbyApi
 * callback thunk func_00268CC0.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void func_0025D048(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        char buf[0x100];
        int st[16];
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_STAT), buf, 0x100, D_004A2F40);
        cDirtysock_tag_TagFieldGetStructure(buf, st, 0x40, str_l);
        self->mStats.unk_04 = st[2];
        self->mStats.unk_08 = st[2] + st[3] + st[4];
        self->mStats.unk_0C = st[6];
        self->mStats.mRank = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A3130), 0);
        self->mStats.unk_10 = -1;
        func_0025E0C8(self, 0);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_D048(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025D1B8);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_PERS[];
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void func_00268CE8(void* self, void* a1, int a2);

struct sMsgListNodeD1B8 {
    sMsgListNodeD1B8* next;
    sMsgListNodeD1B8* prev;
    void* data;
};

struct sMsgListIterD1B8 {
    sMsgListNodeD1B8* node;
    sMsgListIterD1B8(sMsgListNodeD1B8* x) : node(x) {}
    sMsgListIterD1B8(const sMsgListIterD1B8& x) : node(x.node) {}
};

static inline sMsgListIterD1B8 sMsgListD1B8_insert(sMsgListIterD1B8 pos, void* const& x)
{
    sMsgListNodeD1B8* tmp = (sMsgListNodeD1B8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}


/**
 * Sends the create-persona ('cper') request and adds the name to the persona list ahead of the reply.
 *
 * Does nothing when mState is 0. Sends PERS = @p name with func_0025B608() (mState 10, 20 s timeout, reply through
 * func_00268CE8 to LobbyMan_onpersonacreated()), assigns @p name to mPersonas[mNumPersonas] (the count only grows when the reply
 * succeeds) and pushes an Event of type 213 onto mEventList. Likely bug: there is no check that mNumPersonas is below 4
 * (the preview asserted "numpersonas < max_personas"), so a fifth persona would overwrite mPersona.
 *
 * @param name  the new persona's name
 */
extern "C" void func_0025D1B8(LobbyMan* self, const char* name)
{
    if (*(int*)self != 0) {
        char buf[0x200];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PERS, name);
        int type = 213;
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 10, 0x63706572, buf, (int)func_00268CE8, 20.0f);
        LobbyMan* lobbyMan = self;
        cBXString_operatorE1(&((char**)&lobbyMan->mPersonas[0])[lobbyMan->mNumPersonas], name);
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = type;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        void* mp = event;
        sMsgListD1B8_insert(*(sMsgListNodeD1B8**)(((char*)self) + 0xF0), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onpersonacreated);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];

extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeD2D8 {
    sMsgListNodeD2D8* next;
    sMsgListNodeD2D8* prev;
    void* data;
};

struct sMsgListIterD2D8 {
    sMsgListNodeD2D8* node;
    sMsgListIterD2D8(sMsgListNodeD2D8* x) : node(x) {}
    sMsgListIterD2D8(const sMsgListIterD2D8& x) : node(x.node) {}
};

static inline sMsgListIterD2D8 sMsgListD2D8_insert(sMsgListIterD2D8 pos, void* const& x)
{
    sMsgListNodeD2D8* tmp = (sMsgListNodeD2D8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_D2D8(char* self, void* msg)
{
    void* mp = msg;
    sMsgListD2D8_insert(*(sMsgListNodeD2D8**)(self + 0xF0), mp);
}

/**
 * Handles the lobby server's reply to the create-persona ('cper') request.
 *
 * Sets mState to 1. On success it increments mNumPersonas (keeping the name func_0025D1B8() stored) and pushes an Event
 * of type 214 onto mEventList. On failure it first passes a 'dupl' (name taken) reply to func_0025BC48(), which reads
 * the server's suggested names, then pushes a type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. Called only
 * from the LobbyApi callback thunk func_00268CE8.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onpersonacreated(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        (self->mNumPersonas)++;
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 214;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_D2D8(((char*)self), event);
    } else {
        if (*(int*)(msg + 0x8) == 0x6475706C)
            func_0025BC48(self, msg);
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_D2D8(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025D428);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_PERS[];
extern "C" void func_00268D10(void* self, void* a1, int a2);

struct sMsgListNodeD428 {
    sMsgListNodeD428* next;
    sMsgListNodeD428* prev;
    void* data;
};

struct sMsgListIterD428 {
    sMsgListNodeD428* node;
    sMsgListIterD428(sMsgListNodeD428* x) : node(x) {}
    sMsgListIterD428(const sMsgListIterD428& x) : node(x.node) {}
};

static inline sMsgListIterD428 sMsgListD428_insert(sMsgListIterD428 pos, void* const& x)
{
    sMsgListNodeD428* tmp = (sMsgListNodeD428*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Sends the delete-persona ('dper') request for one of the account's personas.
 *
 * Does nothing when mState is 0. Sends PERS = mPersonas[@p idx] with func_0025B608() (mState 11, 20 s timeout, reply
 * through func_00268D10 to LobbyMan_onpersonadeleted()), stores @p idx in mDeletePersonaIdx and pushes an Event of type 215 onto
 * mEventList.
 *
 * @param idx  index into mPersonas; not range-checked
 */
extern "C" void func_0025D428(LobbyMan* self, int idx)
{
    if (*(int*)self != 0) {
        char buf[0x200];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x200, str_PERS, *(const char**)(((char*)self) + 0x70 + (idx << 2)));
        int type = 215;
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 11, 0x64706572, buf, (int)func_00268D10, 20.0f);
        self->mDeletePersonaIdx = idx;
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = type;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        void* mp = event;
        sMsgListD428_insert(*(sMsgListNodeD428**)(((char*)self) + 0xF0), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onpersonadeleted);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern "C" void* cBXString_operatorE(void* self, void* other);
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeD538 {
    sMsgListNodeD538* next;
    sMsgListNodeD538* prev;
    void* data;
};

struct sMsgListIterD538 {
    sMsgListNodeD538* node;
    sMsgListIterD538(sMsgListNodeD538* x) : node(x) {}
    sMsgListIterD538(const sMsgListIterD538& x) : node(x.node) {}
};

static inline sMsgListIterD538 sMsgListD538_insert(sMsgListIterD538 pos, void* const& x)
{
    sMsgListNodeD538* tmp = (sMsgListNodeD538*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_D538(char* lst, void* msg)
{
    void* mp = msg;
    sMsgListD538_insert(*(sMsgListNodeD538**)(lst + 4), mp);
}


/**
 * Handles the lobby server's reply to the delete-persona ('dper') request.
 *
 * Sets mState to 1. On success it decrements mNumPersonas, shifts the mPersonas entries after mDeletePersonaIdx down by
 * one (the old last slot keeps a copy of its name) and pushes an Event of type 216 onto mEventList; otherwise it pushes
 * a type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. Called only from the LobbyApi callback thunk
 * func_00268D10.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onpersonadeleted(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        LobbyMan* lobbyMan = self;
        char* lst = ((char*)self) + 0xEC;
        lobbyMan->mNumPersonas--;
        for (int i = lobbyMan->mDeletePersonaIdx; i < lobbyMan->mNumPersonas; i++)
            cBXString_operatorE(&((char**)&lobbyMan->mPersonas[0])[i], &((char**)&lobbyMan->mPersonas[0])[i + 1]);
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        event[0] = 216;
        post_D538(lst, event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_D538(((char*)self) + 0xEC, event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onroomlistupdate);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNodeK6c0 {
    sMsgListNodeK6c0* next;
    sMsgListNodeK6c0* prev;
    void* data;
};

struct sMsgListIterK6c0 {
    sMsgListNodeK6c0* node;
    sMsgListIterK6c0(sMsgListNodeK6c0* x) : node(x) {}
    sMsgListIterK6c0(const sMsgListIterK6c0& x) : node(x.node) {}
};

static inline sMsgListIterK6c0 sMsgListK6c0_insert(sMsgListIterK6c0 pos, void* const& x)
{
    sMsgListNodeK6c0* tmp = (sMsgListNodeK6c0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Posts a room-list-changed notice: pushes an Event of type 223 onto mEventList.
 *
 * It ignores the update message. Called through thunk func_00268A90, which func_0025A778 registers as the callback of
 * LobbyApi list channel 0 (mpRoomList).
 */
extern "C" void LobbyMan_onroomlistupdate(LobbyMan* self)
{
    int* event = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    sMsgListNodeK6c0* list = *(sMsgListNodeK6c0**)(((char*)self) + 0xF0);
    event[0] = 223;
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    void* mp = event;
    sMsgListK6c0_insert(list, mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025D770);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern char str_NAME[];
extern "C" void* func_0025F150(LobbyMan* self);

/**
 * Sends a 'peek' request for a room in the room list, or with an empty name.
 *
 * Does nothing when mState is 0. NAME is the name of room @p idx (func_0025F150(), which gets @p idx through the second
 * argument register its C declaration omits) or "" for -1; the request goes straight to func_003E8B10() with no
 * callback, no state change and no timeout.
 *
 * @param idx  index into the room list, or -1
 */
extern "C" void func_0025D770(LobbyMan* self, int idx)
{
    char buf[256];
    if (*(int*)self != 0) {
        const char* key = str_NAME;
        const char* name;
        buf[0] = 0;
        if (idx != -1) {
            name = (const char*)func_0025F150(self);
        } else {
            name = D_004A2F40;
        }
        cDirtysock_tag_TagFieldSetString(buf, 0x100, key, name);
        func_003E8B10(self->mpLobbyApi, 0x7065656B, buf, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025D800);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern "C" void* func_0025F150(LobbyMan* self);
extern "C" void func_0025D860(LobbyMan* self, void* name, int a2);

/**
 * Joins a room from the room list (or the room with an empty name for index -1) with an optional password.
 *
 * Takes the name of room @p idx from func_0025F150() (which gets @p idx through the second argument register) and
 * passes it to func_0025D860().
 *
 * @param idx  index into the room list, or -1
 * @param password   the room password, or 0
 */
extern "C" void func_0025D800(LobbyMan* self, int idx, int password)
{
    void* name;
    if (idx != -1) {
        name = func_0025F150(self);
    } else {
        name = D_004A2F40;
    }
    func_0025D860(self, name, password);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025D860);
#ifdef SKIP_ASM
extern char str_NAME[];
extern char str_PASS[];
extern "C" void func_00268D38(void* self, void* a1, int a2);

/**
 * Sends the room-join ('move') request.
 *
 * Does nothing when mState is 0. Builds NAME = @p name and, when given, PASS = @p password, and sends it with
 * func_0025B608() (mState 12, 20 s timeout, reply through func_00268D38 to LobbyMan_onroomselected()).
 *
 * @param name      the room name (a string passed as void*)
 * @param password  the room password as a string pointer in an int, or 0 for none
 */
// PORT: the unit declares a2 as int; it carries an optional string pointer.
extern "C" void func_0025D860(LobbyMan* self, void* name, int password)
{
    if (*(int*)self != 0) {
        char buf[0x100];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_NAME, (const char*)name);
        if (password != 0) {
            cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PASS, (const char*)password);
        }
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 0xC, 0x6D6F7665, buf, (int)func_00268D38, 20.0f);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onroomselected);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];
extern char D_004A2F40[];
extern char str_NAME[];
extern "C" void cBXString_Reset(void* self);
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");

struct sMsgListNodeD8F8 {
    sMsgListNodeD8F8* next;
    sMsgListNodeD8F8* prev;
    void* data;
};

struct sMsgListIterD8F8 {
    sMsgListNodeD8F8* node;
    sMsgListIterD8F8(sMsgListNodeD8F8* x) : node(x) {}
    sMsgListIterD8F8(const sMsgListIterD8F8& x) : node(x.node) {}
};

static inline sMsgListIterD8F8 sMsgListD8F8_insert(sMsgListIterD8F8 pos, void* const& x)
{
    sMsgListNodeD8F8* tmp = (sMsgListNodeD8F8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_D8F8(char* self, void* msg)
{
    void* mp = msg;
    sMsgListD8F8_insert(*(sMsgListNodeD8F8**)(self + 0xF0), mp);
}

/**
 * Handles the lobby server's reply to the room-join ('move') request.
 *
 * Sets mState to 1. On success it stores the reply's "NAME" in mRoomName and pushes an Event of type 220 onto
 * mEventList; otherwise it empties mRoomName (cBXString_Reset) and pushes a type-258 error event (vtable online_ErrorEvent_vtbl)
 * carrying the status. Called only from the LobbyApi callback thunk func_00268D38.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onroomselected(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    if (*(int*)(msg + 0x8) == 0) {
        char buf[0x100];
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_NAME), buf, 0x100, D_004A2F40);
        cBXString_operatorE1(((char*)self) + 0x84, buf);
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 220;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_D8F8(((char*)self), event);
    } else {
        cBXString_Reset(((char*)self) + 0x84);
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = *(int*)(msg + 0x8);
        post_D8F8(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025DA90);
#ifdef SKIP_ASM
/**
 * Returns the current room's name, or 0 when not connected or not in a room.
 *
 * @return mRoomName when mpLobbyApi is set and the name is non-empty, otherwise 0
 */
extern "C" char* func_0025DA90(LobbyMan* self)
{
    char* roomName;
    if ((int)self->mpLobbyApi == 0) {
        return 0;
    }
    roomName = self->mRoomName;
    if (roomName != 0) {
        if (*roomName != 0) {
            return roomName;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025DAC0);
#ifdef SKIP_ASM
extern char str_NAME[];
extern char str_PASS[];
extern char str_pct_s_pct_s_f[];
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_00268D60(void* self, void* a1, int a2);

/**
 * Sends the create-room ('room') request for a room named "<prefix>.<suffix>".
 *
 * Does nothing when mState is 0. Formats the name with "%s.%s", builds NAME and, when given, PASS, and sends it with
 * func_0025B608() (mState 17, 20 s timeout, reply through func_00268D60 to LobbyMan_onRoomCreated()).
 *
 * @param namePrefix  first part of the room name (a string pointer in an int)
 * @param nameSuffix  second part of the room name (a string pointer in an int)
 * @param password    the room password, or 0 for none
 */
extern "C" void func_0025DAC0(LobbyMan* self, int namePrefix, int nameSuffix, const char* password)
{
    if (*(int*)self != 0) {
        char buf[0x100];
        char name[0x100];
        sprintf(name, str_pct_s_pct_s_f, namePrefix, nameSuffix);
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_NAME, name);
        if (password != 0) {
            cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PASS, password);
        }
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 17, 0x726F6F6D, buf, (int)func_00268D60, 20.0f);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onRoomCreated);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern char str_NAME[];
extern char D_004A2F40[];
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeDB80 {
    sMsgListNodeDB80* next;
    sMsgListNodeDB80* prev;
    void* data;
};

struct sMsgListIterDB80 {
    sMsgListNodeDB80* node;
    sMsgListIterDB80(sMsgListNodeDB80* x) : node(x) {}
    sMsgListIterDB80(const sMsgListIterDB80& x) : node(x.node) {}
};

static inline sMsgListIterDB80 sMsgListDB80_insert(sMsgListIterDB80 pos, void* const& x)
{
    sMsgListNodeDB80* tmp = (sMsgListNodeDB80*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_DB80(char* self, void* msg)
{
    void* mp = msg;
    sMsgListDB80_insert(*(sMsgListNodeDB80**)(self + 0xF0), mp);
}

/**
 * Handles the lobby server's reply to the create-room ('room') request.
 *
 * Sets mState to 1. On success it stores the reply's "NAME" in mRoomName and pushes an Event of type 228 onto
 * mEventList; otherwise it pushes a type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. Called only from the
 * LobbyApi callback thunk func_00268D60.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onRoomCreated(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    int status = *(int*)(msg + 0x8);
    if (status == 0) {
        int type = 228;
        char buf[0x100];
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_NAME), buf, 0x100, D_004A2F40);
        cBXString_operatorE1(((char*)self) + 0x84, buf);
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = type;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_DB80(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = status;
        post_DB80(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025DD08);
#ifdef SKIP_ASM
extern "C" int func_0025A730(LobbyMan* self, const char* key, int def);
extern "C" void func_00268D88(void* self, void* a1, int a2);
extern "C" int func_003E8DC0(void* p, const char* key, int value, int timeout, void* cb, void* user);

/**
 * Pings a player by name through the lobby.
 *
 * Does nothing when mState is 0. Calls func_003E8DC0() (a LobbyApi 'ping' with NAME and ADDR) on mpLobbyApi with @p
 * key, the address func_0025A730() returns for it (a "SSX-PS2-2004" config override of @p def), a 5000 timeout and
 * callback thunk func_00268D88 (reply to LobbyMan_onPingReceived()). func_001AFA60 calls it for a found player.
 *
 * @param key  the player's name (also the config key looked up)
 * @param def  the player's address, used unless the config overrides it
 */
extern "C" void func_0025DD08(LobbyMan* self, const char* key, int def)
{
    if (*(int*)self != 0) {
        func_003E8DC0(self->mpLobbyApi, key, func_0025A730(self, key, def), 5000, (void*)func_00268D88, self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025DD68);
#ifdef SKIP_ASM
extern "C" int func_0025A730(LobbyMan* self, const char* key, int def);
extern "C" void func_00268D88(void* self, void* a1, int a2);
extern "C" int func_003E8DC0(void* p, const char* key, int value, int timeout, void* cb, void* user);
extern "C" void* func_003EE560(void*);

/**
 * Pings a player from the room's user list.
 *
 * Does nothing when mState is 0. Takes the user-list entry from func_003EE560() on mpUserList; its index is whatever
 * the caller left in the second argument register (the C declaration omits that parameter). When the entry exists it
 * pings its name (entry + 0x8) at the address from its word +0x30 (or the config override, func_0025A730()) with
 * func_003E8DC0(), timeout 5000 and callback thunk func_00268D88.
 */
extern "C" void func_0025DD68(LobbyMan* self)
{
    if (*(int*)self != 0) {
        char* userEntry = (char*)func_003EE560(self->mpUserList);
        if (userEntry != 0) {
            const char* key = userEntry + 8;
            func_003E8DC0(self->mpLobbyApi, key, func_0025A730(self, key, *(int*)(userEntry + 0x30)), 5000, (void*)func_00268D88, ((char*)self));
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onPingReceived);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_PingEvent_vtbl[];
extern char str_NAME[];
extern char D_004A2F40[];
extern char str_PING[];
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);
extern "C" void* cBXString_cBXString2(void* self, const char* s);

struct sMsgListNodeDDE0 {
    sMsgListNodeDDE0* next;
    sMsgListNodeDDE0* prev;
    void* data;
};

struct sMsgListIterDDE0 {
    sMsgListNodeDDE0* node;
    sMsgListIterDDE0(sMsgListNodeDDE0* x) : node(x) {}
    sMsgListIterDDE0(const sMsgListIterDDE0& x) : node(x.node) {}
};

static inline sMsgListIterDDE0 sMsgListDDE0_insert(sMsgListIterDDE0 pos, void* const& x)
{
    sMsgListNodeDDE0* tmp = (sMsgListNodeDDE0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Handles a ping reply: pushes an event with the player's name and ping time onto mEventList.
 *
 * Reads "NAME" (up to 0x1F characters) and, on success, the "PING" number (default 0); a failed ping gets 32656. The
 * event is 0x10 bytes: type 229, vtable online_PingEvent_vtbl, the name as a cBXString at +0x8 and the ping at +0xC. mState is not
 * touched. Called only from the LobbyApi callback thunk func_00268D88.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onPingReceived(LobbyMan* self, char* msg)
{
    char name[0x20];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_NAME), name, 0x20, D_004A2F40);
    int pingTime;
    if (*(int*)(msg + 0x8) != 0)
        pingTime = 32656;
    else
        pingTime = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_PING), 0);
    int* event = (int*)operator new(0x10, str_NetEvent, 0x20000000, 0);
    event[0] = 229;
    *(void***)((char*)event + 0x4) = online_PingEvent_vtbl;
    cBXString_cBXString2((char*)event + 0x8, name);
    event[3] = pingTime;
    void* mp = event;
    sMsgListDDE0_insert(*(sMsgListNodeDDE0**)(((char*)self) + 0xF0), mp);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025DF00);
#ifdef SKIP_ASM
extern char str_PERS[];
extern char str_LANG[];
extern "C" const char* func_00259198();
extern "C" void func_00268DB0(void* self, void* a1, int a2);

/**
 * Sends the report-abuse ('rept') request about a player.
 *
 * Does nothing when mState is 0. Builds PERS = @p name and LANG = func_00259198()'s "en", and sends it with
 * func_0025B608() (mState 16, 20 s timeout, reply through func_00268DB0 to LobbyMan_onAbuseReportReceived()).
 *
 * @param name  the reported persona
 */
extern "C" void func_0025DF00(LobbyMan* self, const char* name)
{
    if (*(int*)self != 0) {
        char buf[0x100];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PERS, name);
        const char* key = str_LANG;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, key, func_00259198());
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 0x10, 0x72657074, buf, (int)func_00268DB0, 20.0f);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onAbuseReportReceived);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeDF98 {
    sMsgListNodeDF98* next;
    sMsgListNodeDF98* prev;
    void* data;
};

struct sMsgListIterDF98 {
    sMsgListNodeDF98* node;
    sMsgListIterDF98(sMsgListNodeDF98* x) : node(x) {}
    sMsgListIterDF98(const sMsgListIterDF98& x) : node(x.node) {}
};

static inline sMsgListIterDF98 sMsgListDF98_insert(sMsgListIterDF98 pos, void* const& x)
{
    sMsgListNodeDF98* tmp = (sMsgListNodeDF98*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_DF98(char* self, void* msg)
{
    void* mp = msg;
    sMsgListDF98_insert(*(sMsgListNodeDF98**)(self + 0xF0), mp);
}

/**
 * Handles the lobby server's reply to the report-abuse ('rept') request.
 *
 * Sets mState to 1. On success it pushes an Event of type 232 onto mEventList; otherwise it pushes a type-258 error
 * event (vtable online_ErrorEvent_vtbl) carrying the status. Called only from the LobbyApi callback thunk func_00268DB0.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onAbuseReportReceived(LobbyMan* self, char* msg)
{
    *(int*)self = 1;
    int status = *(int*)(msg + 0x8);
    if (status == 0) {
        int type = 232;
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = type;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_DF98(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = status;
        post_DF98(((char*)self), event);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025E0C8);
#ifdef SKIP_ASM
extern char str_NAME[];
extern "C" int cDirtysock_tag_TagFieldSetNumber(char* buf, int size, const char* key, int value);
extern "C" void func_0025B608(LobbyMan* self, int a1, int tag, void* cb, int a4, float f);
extern "C" void func_00268DD8(void* self, void* a1, int a2);

/**
 * Requests a news item from the lobby server ('news' with NAME = @p value as a number).
 *
 * Does nothing when mState is 0. func_0025B608() sends it with mState 14 and a 20 s timeout; the reply goes through
 * func_00268DD8 to LobbyMan_onNewsRetrieved(). func_0025D048() asks for item 0 after login, the news screen
 * (cFEStateNetNews_onCreateScreen) for others.
 *
 * @param value  the news item number
 */
extern "C" void func_0025E0C8(LobbyMan* self, int value)
{
    if (*(int*)self != 0) {
        char buf[0x100];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_NAME, value);
        // PORT: function pointer passed through func_0025B608's int parameter.
        func_0025B608(self, 0xE, 0x6E657773, buf, (int)func_00268DD8, 20.0f);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onNewsRetrieved);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_NewsEvent_vtbl[];
extern void* online_Event_vtbl[];
extern char str_BUDDYSERVERNAME[];
extern char str_BUDDYPORT[];
extern char str_BUDDYRESOURCE[];
extern char str_BUDDYMSGTIMEOUT[];
extern char str_SSX_PS2_2004[];
extern char D_004A2F40[];
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" int func_00261460(void* p);
extern "C" void func_00261248(BuddyMan* self, void* self2, int idx, void* a, void* b, int c);


struct sNetMgrE138 {
    char pad[0xA4];
    int timeout;    // 0xA4
};

struct sMsgListNodeE138 {
    sMsgListNodeE138* next;
    sMsgListNodeE138* prev;
    void* data;
};

struct sMsgListIterE138 {
    sMsgListNodeE138* node;
    sMsgListIterE138(sMsgListNodeE138* x) : node(x) {}
    sMsgListIterE138(const sMsgListIterE138& x) : node(x.node) {}
};

static inline sMsgListIterE138 sMsgListE138_insert(sMsgListIterE138 pos, void* const& x)
{
    sMsgListNodeE138* tmp = (sMsgListNodeE138*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_E138(char* self, void* msg)
{
    void* mp = msg;
    sMsgListE138_insert(*(sMsgListNodeE138**)(self + 0xF0), mp);
}

/**
 * Handles the lobby server's reply to a 'news' request, starting the buddy-server login on item 0.
 *
 * Sets mState to 1. Status 'new1' pushes a news event (0xC bytes, type 226, vtable online_NewsEvent_vtbl) holding the reply record
 * as a string. Status 'new0' sets mLoggedIn and, when BuddyMan_mBuddyMan is not already connected (func_00261460()
 * returns 0), reads "BUDDYSERVERNAME", "BUDDYPORT" (default 13505), "BUDDYRESOURCE" (default "SSX-PS2-2004") and
 * "BUDDYMSGTIMEOUT" (default 864000, stored in BuddyMan_mBuddyMan->miMsgTimeout) and connects with func_00261248()
 * using mPersona and mPassword; it then pushes an Event of type 273. Other statuses post nothing. Called only from the
 * LobbyApi callback thunk func_00268DD8.
 *
 * @param msg  the LobbyApi reply: status at +0x8 ('new0' or 'new1'), tag record at +0xC
 */
extern "C" void LobbyMan_onNewsRetrieved(LobbyMan* self, char* msg)
{
    LobbyMan* st = self;
    st->mState = 1;
    int type = *(int*)(msg + 0x8);
    if (type == 0x6E657731) {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 226;
        *(void***)((char*)event + 0x4) = online_NewsEvent_vtbl;
        cBXString_cBXString2(event + 2, *(char**)(msg + 0xC));
        post_E138(((char*)self), event);
    } else if (type == 0x6E657730) {
        st->mLoggedIn = 1;
        if (func_00261460(BuddyMan_mBuddyMan) == 0) {
            char host[0x100];
            char name[0x100];
            cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_BUDDYSERVERNAME), host, 0x100, D_004A2F40);
            int port = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_BUDDYPORT), 13505);
            cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_BUDDYRESOURCE), name, 0x100, str_SSX_PS2_2004);
            BuddyMan_mBuddyMan->miMsgTimeout = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_BUDDYMSGTIMEOUT), 0xD2F00);
            // PORT: a pointer passed through func_00261248's int parameter.
            func_00261248(BuddyMan_mBuddyMan, host, port, self->mPersona, self->mPassword, (int)name);
        }
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = 273;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_E138(((char*)self), event);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025E348);
#ifdef SKIP_ASM
extern "C" void* func_003E8F08(void* ctx, int tag);
extern "C" unsigned int func_0025F338(LobbyMan* self);
extern "C" void func_0025E420(LobbyMan* self, int a1, int a2, int a3);
extern "C" void LobbyMan_onranklistupdate(LobbyMan* self, int unused);

struct sDsSlotSessK2 {
    int active;                  // 0x0
    char pad_0x04[0x50];
    void* ctx;                   // 0x54
    char pad_0x58[0xF8 - 0x58];
    int slot;                    // 0xF8
    int pad_0xFC;
    void* slotTag;               // 0x100
    unsigned int limit[58];      // 0x104
    void* handles[29];           // 0x1EC
};

/**
 * Selects which rank list (slot 0..28) the ranking screens show and refreshes it when needed.
 *
 * Does nothing when mState is 0 (the view's `active`). Stores @p slot in mRankSlot and func_003E8F08(mpLobbyApi,
 * 'slot') in mpSlotTag. When the slot's snapshot holds fewer rows (func_0025F338()) than mRankCount[@p slot], it
 * releases the slot's mRankSnapshot entry (func_003E8688(api, 4, ..)) and asks for rows 0..99 with func_0025E420()
 * (passing the previous slot); otherwise it just calls the snapshot callback LobbyMan_onranklistupdate() with no message. The
 * parameter's view type (sDsSlotSessK2) is LobbyMan read through its own layout.
 *
 * @param slot  rank-list slot, 0..28; not range-checked
 */
extern "C" void func_0025E348(sDsSlotSessK2* self, int slot)
{
    if (self->active != 0) {
        int old = self->slot;
        self->slot = slot;
        self->slotTag = func_003E8F08(self->ctx, 0x736C6F74);
        if (func_0025F338((LobbyMan*)self) < self->limit[slot]) {
            if (self->handles[self->slot] != 0) {
                func_003E8688(self->ctx, 4, self->handles[self->slot]);
                self->handles[self->slot] = 0;
            }
            func_0025E420((LobbyMan*)self, 0, 100, old);
        } else {
            LobbyMan_onranklistupdate((LobbyMan*)((char*)self), 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025E420);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldSetNumber(char* buf, int size, const char* key, int value);
extern "C" void func_0025B608(LobbyMan* self, int a1, int tag, void* cb, int a4, float f);
extern "C" void* func_003E85B0(void* mgr, int idx, void* cb, void* user);
extern "C" void func_003E8D70(void* p, int a);
extern "C" void func_00268AE0(void* self, void* a1, int a2);
extern "C" void func_00268E00(void* self, void* a1, int a2);
extern int D_004A3180;
extern char str_INDEX[];
extern char str_CHAN[];
extern char str_START[];
extern char str_RANGE[];
extern char str_SEQN[];

struct sDsSlotSessE420 {
    int active;                  // 0x0
    char pad_0x04[0x3C];
    int f40;                     // 0x40
    char pad_0x44[0x10];
    void* ctx;                   // 0x54
    char pad_0x58[0xF8 - 0x58];
    int slot;                    // 0xF8
    int pad_0xFC;
    void* slotTag;               // 0x100
    unsigned int limitA[29];     // 0x104
    unsigned int limitB[29];     // 0x178
    void* handles[29];           // 0x1EC
};

/**
 * Requests a range of rows of the selected rank list ('snap').
 *
 * Does nothing when mState is 0. Opens a type-4 snapshot for mRankSlot when it has none (func_003E85B0, callback thunk
 * func_00268AE0), builds INDEX = mRankSlot, CHAN = D_004A3180 + 4 (D_004A3180 is zero and only read here), START = @p
 * a1, RANGE = @p range and SEQN = mRankSlot, cancels the request stored for slot @p prevSlot (func_003E8D70) and sends 'snap'
 * with func_0025B608(), keeping the current mState (20 s timeout, reply through func_00268E00 to LobbyMan_onRanklistRetrieved()). The
 * new request id goes into mRankRequest[mRankSlot].
 *
 * @param self_  the lobby manager (LobbyMan_mLobbyMan)
 * @param start     first row to fetch
 * @param range     number of rows to fetch
 * @param prevSlot     slot whose pending request is cancelled (the previously selected one)
 */
extern "C" void func_0025E420(LobbyMan* self_, int start, int range, int prevSlot)
{
    sDsSlotSessE420* self = (sDsSlotSessE420*)self_;
    if (self->active == 0)
        return;
    if (self->handles[self->slot] == 0)
        self->handles[self->slot] = func_003E85B0(self->ctx, 4, (void*)func_00268AE0, self);
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_INDEX, self->slot);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_CHAN, D_004A3180 + 4);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_START, start);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_RANGE, range);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x100, str_SEQN, self->slot);
    func_003E8D70(self->ctx, self->limitB[prevSlot]);
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608((LobbyMan*)self, self->active, 0x736E6170, buf, (int)func_00268E00, 20.0f);
    self->limitB[self->slot] = self->f40;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onRanklistRetrieved);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];
extern char str_START[];
extern char str_SEQN[];
extern char str_RANGE[];
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);

struct sMsgListNodeE590 {
    sMsgListNodeE590* next;
    sMsgListNodeE590* prev;
    void* data;
};

struct sMsgListIterE590 {
    sMsgListNodeE590* node;
    sMsgListIterE590(sMsgListNodeE590* x) : node(x) {}
    sMsgListIterE590(const sMsgListIterE590& x) : node(x.node) {}
};

static inline sMsgListIterE590 sMsgListE590_insert(sMsgListIterE590 pos, void* const& x)
{
    sMsgListNodeE590* tmp = (sMsgListNodeE590*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Handles a rank-list ('snap') reply: records how many rows of the slot are available.
 *
 * On success it stores "START" + "RANGE" in mRankCount["SEQN"] (the index comes from the server and is not
 * range-checked); otherwise it pushes a type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. mState is not
 * touched. Called only from the LobbyApi callback thunk func_00268E00.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onRanklistRetrieved(LobbyMan* self, char* msg)
{
    int status = *(int*)(msg + 0x8);
    if (status == 0) {
        int start = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_START), 0);
        int idx = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_SEQN), 0);
        int range = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_RANGE), 0);
        *(int*)(((char*)self) + (idx << 2) + 0x104) = start + range;
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = status;
        void* mp = event;
        sMsgListE590_insert(*(sMsgListNodeE590**)(((char*)self) + 0xF0), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025E6D0);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_PERS[];
extern "C" void func_00268E28(void* self, void* a1, int a2);

struct sMsgListNodeE6D0 {
    sMsgListNodeE6D0* next;
    sMsgListNodeE6D0* prev;
    void* data;
};

struct sMsgListIterE6D0 {
    sMsgListNodeE6D0* node;
    sMsgListIterE6D0(sMsgListNodeE6D0* x) : node(x) {}
    sMsgListIterE6D0(const sMsgListIterE6D0& x) : node(x.node) {}
};

static inline sMsgListIterE6D0 sMsgListE6D0_insert(sMsgListIterE6D0 pos, void* const& x)
{
    sMsgListNodeE6D0* tmp = (sMsgListNodeE6D0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Looks up a player by persona name ('user' request), unless it is the local persona.
 *
 * Does nothing when mState is 0. When @p name equals mPersona (case-insensitive) it only pushes an Event of type 270
 * onto mEventList. Otherwise it sends PERS = @p name with func_0025B608(), keeping the current mState (20 s timeout,
 * reply through func_00268E28 to LobbyMan_onPlayerFound()), and sets mFindingPlayer.
 *
 * @param name  the persona to find
 */
extern "C" void func_0025E6D0(LobbyMan* self, const char* name)
{
    if (*(int*)self != 0) {
        if (strcasecmp(self->mPersona, (void*)name) == 0) {
            int type = 270;
            int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            event[0] = type;
            *(void***)((char*)event + 0x4) = online_Event_vtbl;
            void* mp = event;
            sMsgListE6D0_insert(*(sMsgListNodeE6D0**)(((char*)self) + 0xF0), mp);
        } else {
            char buf[0x100];
            buf[0] = 0;
            cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PERS, name);
            self->mFindingPlayer = 1;
            // PORT: function pointer passed through func_0025B608's int parameter.
            func_0025B608(self, *(int*)self, 0x75736572, buf, (int)func_00268E28, 20.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onPlayerFound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cDirtysock_tag_TagFieldGetStructure(const char* tag, void* data, int len, const char* fmt);
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* tag, int defval);
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern char str_l[];
extern char D_004A2F40[];
extern char str_ADDR[];
extern char str_PERS[];
extern char str_STAT[];
extern char str_RANK[];
extern char str_ROOM[];
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern void* online_FindPlayerEvent_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeE7F0 {
    sMsgListNodeE7F0* next;
    sMsgListNodeE7F0* prev;
    void* data;
};

struct sMsgListIterE7F0 {
    sMsgListNodeE7F0* node;
    sMsgListIterE7F0(sMsgListNodeE7F0* x) : node(x) {}
    sMsgListIterE7F0(const sMsgListIterE7F0& x) : node(x.node) {}
};

static inline sMsgListIterE7F0 sMsgListE7F0_insert(sMsgListIterE7F0 pos, void* const& x)
{
    sMsgListNodeE7F0* tmp = (sMsgListNodeE7F0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_E7F0(char* self, void* msg)
{
    void* mp = msg;
    sMsgListE7F0_insert(*(sMsgListNodeE7F0**)(self + 0xF0), mp);
}

struct sRecE7F0 {
    int id;
    int start;
    int end;
    int len;
    int extra;
};

struct sMsgE7F0 {
    int type;
    void** vt;
    char name[4];       // 0x8 cBXString
    char text[4];       // 0xC cBXString
    int on;             // 0x10
    int count;          // 0x14
    sRecE7F0 rec;       // 0x18
};

/**
 * Handles the lobby server's reply to the find-player ('user') request.
 *
 * Clears mFindingPlayer (mState is not touched). On success it reads "PERS", "STAT" (decoded with the "l*" template),
 * "RANK" (default -1), "ADDR" as a number and "ROOM", and pushes a 44-byte event: type 227, vtable online_FindPlayerEvent_vtbl, the
 * persona and room as cBXStrings at +0x8 and +0xC, 1 at +0x10 when ADDR is positive, ADDR at +0x14, and at +0x18 a
 * tLobbyStats-shaped block (RANK, item 2, items 2 + 3 + 4, item 6, -1) that func_001AFA60 copies into mOpponentStats.
 * Otherwise it pushes a type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. Called only from the LobbyApi
 * callback thunk func_00268E28.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onPlayerFound(LobbyMan* self, char* msg)
{
    self->mFindingPlayer = 0;
    int err = *(int*)(msg + 0x8);
    if (err == 0) {
        char name[0x100];
        char text[0x100];
        sRecE7F0 rec;
        char buf[0x100];
        int st[16];
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_PERS), name, 0x100, D_004A2F40);
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_STAT), buf, 0x100, D_004A2F40);
        cDirtysock_tag_TagFieldGetStructure(buf, st, 0x40, str_l);
        rec.start = st[2];
        rec.end = st[2] + st[3] + st[4];
        rec.len = st[6];
        rec.id = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_RANK), -1);
        rec.extra = -1;
        int count = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_ADDR), 0);
        int on = count > 0;
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_ROOM), text, 0x100, D_004A2F40);
        sMsgE7F0* event = (sMsgE7F0*)operator new(44, str_NetEvent, 0x20000000, 0);
        event->type = 227;
        event->vt = online_FindPlayerEvent_vtbl;
        cBXString_cBXString2(event->name, name);
        cBXString_cBXString2(event->text, text);
        event->on = on;
        event->count = count;
        event->rec = rec;
        post_E7F0(((char*)self), event);
    } else {
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = err;
        post_E7F0(((char*)self), event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025EAA0);
#ifdef SKIP_ASM
extern "C" void func_00268E50();

/**
 * Requests another player's online info: sends 'onln' with PERS = @p name.
 *
 * Calls func_003E8B10() directly, so mState, mTimeout and mRequestId stay as they are; the reply goes through
 * func_00268E50 to func_0025EB00().
 *
 * @param name  the persona to look up
 */
extern "C" void func_0025EAA0(LobbyMan* self, const char* name)
{
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PERS, name);
    // PORT: function pointer passed through func_003E8B10's int parameter.
    func_003E8B10(self->mpLobbyApi, 0x6F6E6C6E, buf, (int)func_00268E50, ((char*)self));
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025EB00);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Handles a user record: with no type code it parses the record's name fields, counts and
// stats into a 0x10D message; any type other than 'uusr' becomes a 0x102 message.
extern char D_004A31D8[];
extern char D_004A2F38[];
extern char D_004A31E0[];
extern char D_004A31E8[];
extern char D_004A31F0[];
extern char D_004A31F8[];
extern char D_004A3130[];
extern void* D_00481110[];

struct sMsgListNode_25EB00 {
    sMsgListNode_25EB00* next;
    sMsgListNode_25EB00* prev;
    void* data;
};

struct sMsgListIter_25EB00 {
    sMsgListNode_25EB00* node;
    sMsgListIter_25EB00(sMsgListNode_25EB00* x) : node(x) {}
    sMsgListIter_25EB00(const sMsgListIter_25EB00& x) : node(x.node) {}
};

struct sMsgList_25EB00 {
    sMsgListNode_25EB00* head;
    sMsgListIter_25EB00 end() { return head; }
    sMsgListIter_25EB00 insert(sMsgListIter_25EB00 pos, void* const& x)
    {
        sMsgListNode_25EB00* tmp = (sMsgListNode_25EB00*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
        void** data = &tmp->data;
        if (data != 0)
            *data = x;
        tmp->next = pos.node;
        tmp->prev = pos.node->prev;
        pos.node->prev->next = tmp;
        pos.node->prev = tmp;
        return tmp;
    }
};

struct sUserStats_25EB00 { int a, b, c, d, e; };

struct sUserMsg_25EB00 {
    int kind;
    void** vt;
    char name[0x20];
    char s2[0x20];
    char s3[0x100];
    int n2;                     // 0x148
    int n1;                     // 0x14C
    sUserStats_25EB00 stats;    // 0x150
};

/**
 * Handles the reply to another player's 'onln' info request.
 *
 * On success it reads "M", "N" and "A" (strings), "F" and "P" (numbers, default -1), "S" (stats, decoded with the "l*"
 * template) and "R" (number, default 0), and pushes a 356-byte event: type 269, vtable D_00481110, M, N and A strcpy'd
 * to +0x8 (0x20 bytes), +0x28 (0x20 bytes) and +0x48 (0x100 bytes), P at +0x148, F at +0x14C, and at +0x150 a stats
 * block (R, item 2, items 2 + 3 + 4, item 6, F). Likely bug: M and N are read into 0x100-byte buffers and strcpy'd into
 * 0x20-byte fields with no length check. Status 'uusr' posts nothing; other statuses push a type-258 error event
 * (vtable online_ErrorEvent_vtbl). mState is not touched. Called only from the LobbyApi callback thunk func_00268E50.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void func_0025EB00(LobbyMan* self, char* msg)
{
    int type = *(int*)(msg + 0x8);
    if (type == 0) {
        char name[0x100];
        char tagN[0x100];
        char tagA[0x100];
        char raw[0x20];
        int st[0x10];
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A31D8), name, 0x100, D_004A2F40);
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A2F38), tagN, 0x100, D_004A2F40);
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A31E0), tagA, 0x100, D_004A2F40);
        int tagF = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A31E8), -1);
        int tagP = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A31F0), -1);
        cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A31F8), raw, 20, D_004A2F40);
        int rank = cDirtysock_tag_TagFieldGetNumber(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), D_004A3130), 0);
        cDirtysock_tag_TagFieldGetStructure(raw, st, 0x40, str_l);
        sUserStats_25EB00 info;
        info.a = rank;
        info.b = st[2];
        info.c = st[2] + st[3] + st[4];
        info.d = st[6];
        info.e = tagF;
        sUserMsg_25EB00* netEvent = (sUserMsg_25EB00*)operator new(356, str_NetEvent, 0x20000000, 0);
        netEvent->kind = 269;
        netEvent->vt = D_00481110;
        netEvent->n2 = tagP;
        netEvent->n1 = tagF;
        strcpy(netEvent->name, name);
        strcpy(netEvent->s2, tagN);
        strcpy(netEvent->s3, tagA);
        netEvent->stats = info;
        sMsgList_25EB00* eventList = (sMsgList_25EB00*)(((char*)self) + 0xF0);
        void* mp = netEvent;
        eventList->insert(eventList->end(), mp);
    } else if (type != 0x75757372) {
        int* netEvent = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        netEvent[0] = 258;
        *(void***)((char*)netEvent + 0x4) = online_ErrorEvent_vtbl;
        netEvent[2] = type;
        sMsgList_25EB00* eventList = (sMsgList_25EB00*)(((char*)self) + 0xF0);
        void* mp = netEvent;
        eventList->insert(eventList->end(), mp);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025EE20);
#ifdef SKIP_ASM
extern char str_KIND[];
extern char str_QMFP[];
extern char str_DeathRace[];
extern char str_l[];
extern "C" int cDirtysock_tag_TagFieldSetStructure(char* buf, int size, const char* key, const void* data, int len, const char* fmt);
extern "C" void func_00268E78(void* self, void* a1, int a2);

/**
 * Starts a quick-match search ('quik') when the lobby is idle.
 *
 * Only when mState is 1: stores @p matchType in mMatchType and sends KIND = "DeathRace" plus QMFP = the 20 bytes at @p
 * data encoded with the "l*" template (cDirtysock_tag_TagFieldSetStructure) with func_0025B608() (mState 18, 20 s
 * timeout, reply through func_00268E78 to LobbyMan_onQuickMatchResponse()).
 *
 * @param data       the search options (five ints; the quick-play popup's miSearchOptions)
 * @param matchType  the game kind, kept in mMatchType
 */
extern "C" void func_0025EE20(LobbyMan* self, const void* data, int matchType)
{
    int st = *(int*)self;
    if (st != 0) {
        bool on = (st == 1);
        if (on) {
            char buf[0x100];
            self->mMatchType = matchType;
            buf[0] = 0;
            cDirtysock_tag_TagFieldSetString(buf, 0x100, str_KIND, str_DeathRace);
            cDirtysock_tag_TagFieldSetStructure(buf, 0x100, str_QMFP, data, 20, str_l);
            // PORT: function pointer passed through func_0025B608's int parameter.
            func_0025B608(self, 18, 0x7175696B, buf, (int)func_00268E78, 20.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025EED8);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern char str_KIND[];
extern char D_004A3218[];
extern "C" int* func_002591D0();
extern "C" void func_00263BA8(void* p, int arg);
extern "C" void func_00268E78(void* self, void* a1, int a2);
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNodeEED8 {
    sMsgListNodeEED8* next;
    sMsgListNodeEED8* prev;
    void* data;
};

struct sMsgListIterEED8 {
    sMsgListNodeEED8* node;
    sMsgListIterEED8(sMsgListNodeEED8* x) : node(x) {}
    sMsgListIterEED8(const sMsgListIterEED8& x) : node(x.node) {}
};

static inline sMsgListIterEED8 sMsgListEED8_insert(sMsgListIterEED8 pos, void* const& x)
{
    sMsgListNodeEED8* tmp = (sMsgListNodeEED8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_EED8(char* self, void* msg)
{
    void* mp = msg;
    sMsgListEED8_insert(*(sMsgListNodeEED8**)(self + 0xF0), mp);
}

/**
 * Cancels a quick match or ends the game connection, then returns the lobby to idle.
 *
 * Does nothing when mState is 0. In state 25 (in game) it closes the game connection held at SSXApp->RealFE + 0xB5ABC
 * (func_002591D0) with func_00263BA8(conn, 2) when that is active. In any other state but 1 it sends 'quik' with KIND =
 * "*" straight to func_003E8B10() (setting mTimeout to 20.0 and mRequestId; reply to LobbyMan_onQuickMatchResponse()). Then sets mState
 * to 1 and pushes an Event of type 231 onto mEventList.
 */
extern "C" void func_0025EED8(LobbyMan* self)
{
    int st = *(int*)self;
    if (st == 0)
        return;
    if (st == 25) {
        if (*func_002591D0() != 0)
            func_00263BA8(func_002591D0(), 2);
    } else if (st != 1) {
        char buf[0x100];
        buf[0] = 0;
        cDirtysock_tag_TagFieldSetString(buf, 0x100, str_KIND, D_004A3218);
        self->mTimeout = 20.0f;
        // PORT: function pointer passed through func_003E8B10's int parameter.
        self->mRequestId = func_003E8B10(self->mpLobbyApi, 0x7175696B, buf, (int)func_00268E78, ((char*)self));
    }
    *(int*)self = 1;
    int type = 231;
    int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = type;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    post_EED8(((char*)self), netEvent);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onQuickMatchResponse);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNodeK2e {
    sMsgListNodeK2e* next;
    sMsgListNodeK2e* prev;
    void* data;
};

struct sMsgListIterK2e {
    sMsgListNodeK2e* node;
    sMsgListIterK2e(sMsgListNodeK2e* x) : node(x) {}
    sMsgListIterK2e(const sMsgListIterK2e& x) : node(x.node) {}
};

static inline sMsgListIterK2e sMsgListK2e_insert(sMsgListIterK2e pos, void* const& x)
{
    sMsgListNodeK2e* tmp = (sMsgListNodeK2e*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

extern void* online_ErrorEvent_vtbl[];

/**
 * Handles the lobby server's reply to a quick-match ('quik') request.
 *
 * On success it moves mState from 18 to 20 (other states are kept). On failure it sets mState to 1 and pushes a
 * type-258 error event (vtable online_ErrorEvent_vtbl) carrying the status. Called only from the LobbyApi callback thunk
 * func_00268E78.
 *
 * @param msg  the LobbyApi reply: status at +0x8 (0 = success), tag record at +0xC
 */
extern "C" void LobbyMan_onQuickMatchResponse(LobbyMan* self, char* msg)
{
    if (*(int*)(msg + 0x8) == 0) {
        if (*(int*)self == 18)
            *(int*)self = 20;
    } else {
        *(int*)self = 1;
        int status = *(int*)(msg + 0x8);
        int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = status;
        void* mp = event;
        sMsgListK2e_insert(*(sMsgListNodeK2e**)(((char*)self) + 0xF0), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F120);
#ifdef SKIP_ASM
extern "C" int func_003EE558(void*);

/**
 * Returns the number of rooms in the lobby's room list.
 *
 * @return func_003EE558()'s count for mpRoomList, or 0 when mState is 0
 */
extern "C" int func_0025F120(LobbyMan* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return func_003EE558(self->mpRoomList);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F150);
#ifdef SKIP_ASM
extern "C" void* func_003EE560(void*);

/**
 * Returns the name of a room in the room list.
 *
 * The room index is whatever the caller left in the second argument register (the C declaration omits it); it is passed
 * on to func_003EE560() on mpRoomList and the name is the entry + 0x1C. Likely bug: there is no null check, so an index
 * out of range returns (char*)0x1C.
 *
 * @return the room's name, or 0 when mState is 0
 */
extern "C" void* func_0025F150(LobbyMan* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return (char*)func_003EE560(self->mpRoomList) + 0x1C;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F180);
#ifdef SKIP_ASM
extern "C" void* func_003EE560(void*);

static inline short getFlags(void* p)
{
    return *(short*)((char*)p + 0xA);
}

/**
 * Tells whether a room in the room list is locked (bit 0 of its halfword at +0xA).
 *
 * The room index is whatever the caller left in the second argument register (the C declaration omits it); it is passed
 * on to func_003EE560() on mpRoomList. The lobby menu shows the result as the room's lock. Likely bug: there is no null
 * check on the entry.
 *
 * @return 1 when the room's bit is set, otherwise 0 (also when mState is 0)
 */
extern "C" int func_0025F180(LobbyMan* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return getFlags(func_003EE560(self->mpRoomList)) & 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F1B0);
#ifdef SKIP_ASM
extern "C" int func_003EE558(void*);

/**
 * Returns the number of players in the current room's user list.
 *
 * @return func_003EE558()'s count for mpUserList, or 0 when mState is 0
 */
extern "C" int func_0025F1B0(LobbyMan* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return func_003EE558(self->mpUserList);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F1E0);
#ifdef SKIP_ASM
extern char D_004A2ED0[];
// PORT: the unit declares func_003EE560 with one parameter; this caller passes (list, index).
void* func_003EE560_2(void* list, int idx) __asm__("func_003EE560");

/**
 * Returns the name of a player in the room's user list, optionally with a flag bit.
 *
 * @param idx      index into mpUserList
 * @param outFlag  receives bit 21 of the entry's word +0x4, or 0 to skip it (written only when the entry exists)
 * @return the entry's name (entry + 0x8), " " (D_004A2ED0) when there is no such entry, or 0 when mState is 0
 */
extern "C" char* func_0025F1E0(LobbyMan* self, int idx, int* outFlag)
{
    if (*(int*)self == 0) {
        return 0;
    }
    char* userEntry = (char*)func_003EE560_2(self->mpUserList, idx);
    if (userEntry != 0) {
        if (outFlag != 0) {
            *outFlag = (*(int*)(userEntry + 0x4) >> 21) & 1;
        }
        return userEntry + 8;
    }
    return D_004A2ED0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F248);
#ifdef SKIP_ASM
/**
 * Returns bit 21 of a user-list entry's word +0x4 (the flag func_0025F1E0() reports).
 *
 * The entry index is whatever the caller left in the second argument register (the C declaration omits it); it is
 * passed on to func_003EE560() on mpUserList.
 *
 * @return the bit, or 0 when mState is 0 or there is no such entry
 */
extern "C" int func_0025F248(LobbyMan* self)
{
    void* userEntry;
    if (*(int*)self == 0) {
        return 0;
    }
    userEntry = func_003EE560(self->mpUserList);
    if (userEntry != 0) {
        return (*(int*)((char*)userEntry + 0x4) >> 21) & 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F288);
#ifdef SKIP_ASM
extern char str_l[];
extern "C" void* func_003EE560(void*);
extern "C" int atoi(const char* s);
extern "C" int cDirtysock_tag_TagFieldGetStructure(const char* tag, void* data, int len, const char* fmt);

struct sDsPeerK2 {
    char pad_0x00[0x28];
    char name[0xC];      // 0x28
    int addr;            // 0x34
    char record[1];      // 0x38
};

struct sDsPeerInfoK2 {
    int addr;            // 0x0
    int a;               // 0x4
    int total;           // 0x8
    int b;               // 0xC
    int nameLen;         // 0x10
};

/**
 * Fills a small info block about a player in the room's user list.
 *
 * Does nothing when mState is 0. For entry @p userIndex of mpUserList it stores the entry's word +0x34 in @p out +0x0 and the
 * number parsed (atoi, base 10) from its text at +0x28 in @p out +0x10, then decodes the stats record at +0x38
 * with the "l*" template and stores item 2 at +0x4, items 2 + 3 + 4 at +0x8 and item 6 at +0xC. Without an entry +0x0
 * and +0x10 are 0 and the stats come from decoding a null record.
 *
 * @param userIndex   index into mpUserList (also passed on to func_003EE560() through the second argument register)
 * @param out  receives the five words
 */
extern "C" void func_0025F288(LobbyMan* self, int userIndex, sDsPeerInfoK2* out)
{
    if (*(int*)self != 0) {
        int st[16];
        const char* rec;
        sDsPeerK2* userEntry = (sDsPeerK2*)func_003EE560(self->mpUserList);
        if (userEntry != 0 && userEntry->record != 0) {
            rec = userEntry->record;
            out->addr = userEntry->addr;
            out->nameLen = atoi(userEntry->name);
        } else {
            out->addr = 0;
            rec = 0;
            out->nameLen = 0;
        }
        cDirtysock_tag_TagFieldGetStructure(rec, st, 0x40, str_l);
        out->a = st[2];
        out->total = st[2] + st[3] + st[4];
        out->b = st[6];
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F338);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// The tag message queue (a g++ 2.95 list<void*> at +0xF0 of the session object): node and iterator.
struct sMsgListNode {
    sMsgListNode* next;
    sMsgListNode* prev;
    void* data;
};

struct sMsgListIter {
    sMsgListNode* node;
    sMsgListIter(sMsgListNode* x) : node(x) {}
    sMsgListIter(const sMsgListIter& x) : node(x.node) {}
    void*& operator*() const { return node->data; }
};

extern "C" int func_003EE550(void*);

// The tag message queue itself (list<void*>: allocator word, then the sentinel node).
struct sMsgList {
    int pad;
    sMsgListNode* node;
    sMsgListIter begin() { return node->next; }
    void*& front() { return *begin(); }
    sMsgListIter erase(sMsgListIter pos)
    {
        sMsgListNode* next = pos.node->next;
        sMsgListNode* prev = pos.node->prev;
        prev->next = next;
        next->prev = prev;
        operator_delete((int*)pos.node);
        return next;
    }
    void pop_front() { erase(begin()); }
    sMsgListNode* first() { return node->next; }
};

// An asynchronous dirtysock operation polled by the session.
struct sAsyncOp {
    int f0;
    int result;                         // 0x4
    int (*poll)(sAsyncOp* op);      // 0x8
    void (*release)(sAsyncOp* op);  // 0xC
};


static inline int handleCount_F338(void* h)
{
    return h ? func_003EE550(h) : 0;
}

/**
 * Returns how many rows the selected rank list's snapshot holds.
 *
 * @param self_  the lobby manager (LobbyMan_mLobbyMan)
 * @return func_003EE550()'s count for mRankSnapshot[mRankSlot], or 0 when there is no snapshot or mState is 0
 */
extern "C" unsigned int func_0025F338(LobbyMan* self_)
{
    LobbyMan* self = self_;
    if (self->mState == 0)
        return 0;
    return handleCount_F338(self->mRankSnapshot[self->mRankSlot]);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F388);
#ifdef SKIP_ASM
extern char str_l[];
extern "C" void* func_003EE560(void*);
extern "C" int cDirtysock_tag_TagFieldGetStructure(const char* tag, void* data, int len, const char* fmt);

struct sDsSlotSessK2b {
    int active;                  // 0x0
    char pad_0x04[0xF8 - 0x4];
    int slot;                    // 0xF8
    char pad_0xFC[0x1EC - 0xFC];
    void* handles[29];           // 0x1EC
};

/**
 * Returns a row of the selected rank list and decodes two of its numbers.
 *
 * Looks up row @p rowIndex in mRankSnapshot[mRankSlot] (func_003EE560), decodes the text at entry + 0x28 with the "l*"
 * template into two ints, stores the second in @p outB and, for slots other than 0, the first in @p outA; slot 0 gets
 * the entry's first word instead. Nothing is written when the row is missing.
 *
 * @param rowIndex    row index (passed on to func_003EE560() through the second argument register)
 * @param outA  receives the row's first value
 * @param outB  receives the row's second value
 * @return the row entry, or 0 when it is missing or mState is 0
 */
extern "C" void* func_0025F388(LobbyMan* self, int rowIndex, int* outA, int* outB)
{
    if (self->mState == 0)
        return 0;
    char* rankEntry = (char*)func_003EE560(self->mRankSnapshot[self->mRankSlot]);
    if (rankEntry != 0) {
        int st[2];
        cDirtysock_tag_TagFieldGetStructure(rankEntry + 0x28, st, 8, str_l);
        *outB = st[1];
        if (self->mRankSlot != 0)
            *outA = st[0];
        else
            *outA = *(int*)rankEntry;
    }
    return rankEntry;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F430);
#ifdef SKIP_ASM
extern "C" char* func_003E8A70(void* p, int tag);
/**
 * Tells whether the lobby connection reports a current room.
 *
 * @return true when mState is non-zero and func_003E8A70(mpLobbyApi, 'room') gives a non-empty string
 */
extern "C" bool func_0025F430(LobbyMan* self)
{
    char* room;
    if (*(int*)self == 0) {
        return false;
    }
    room = func_003E8A70(self->mpLobbyApi, 0x726F6F6D);
    return room != 0 && *room != 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F478);
#ifdef SKIP_ASM
extern "C" void func_0025F4C0(void*, char*, int);

/**
 * Challenges a player from the room's user list: passes the name of entry @p idx (func_0025F1E0) to func_0025F4C0().
 *
 * @param idx        index into mpUserList
 * @param matchType  the game kind, passed on
 */
extern "C" void func_0025F478(LobbyMan* self, int idx, int matchType)
{
    func_0025F4C0(self, func_0025F1E0(self, idx, 0), matchType);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F4C0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern int cCommSystem_getSignature_F4C0(void* comm) __asm__("cCommSystem_getSignature");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" void* func_0025FB80(LobbyMan*, int, void*);
// func_0025FB80 is used as void here (its v0 is only a pass-through of its last call).
void func_0025FB80_v(void*, int, void*) __asm__("func_0025FB80");
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_005308D0[];
extern int D_00534B34[];
extern char str_pct_s_pct_d_pct_d_pct_d_pct_d_pct_d_pct_b[];
extern char str_challenge[];

static inline char* Race_25F4C0()
{
    return ((char*)&cBENewRaceInterface_newRace);
}

struct sDsStats_25F4C0 {
    char pad[0xB8];
    int fB8, fBC, fC0, fC4;
};

extern "C" void func_0025F4C0_impl(char* self, const char* name, int a2) __asm__("func_0025F4C0");
/**
 * Challenges a player: sends a "challenge" private message with the race setup and waits for the answer.
 *
 * Does nothing when mState is 0. Sets mIsHost to 1, assigns @p name to mOpponent and stores @p matchType in mMatchType. It
 * then formats "%s %d %d %d %d %d %d %d %d %s %d %d %d %d" into a 64-byte buffer: "challenge", the location id
 * (cBENewRaceInterface_getLocationID), cBENewRaceInterface_newRace's cGameMode and cGameEvent, D_00534B34, bits 0x40
 * and 0x100 of D_005308D0, iNumberAI, bit 0x80 of D_005308D0, cCommSystem_getSignature(), then mStats.unk_04, unk_08,
 * unk_0C and mRank. func_0025FB80() sends it to mOpponent, and mState becomes 22. The first
 * cBE_getInterface__Fv(SSXApp->BE, 7) call's result is unused.
 *
 * @param name  the challenged persona
 * @param matchType    the game kind, kept in mMatchType
 */
extern "C" void func_0025F4C0_impl(char* self, const char* name, int matchType)
{
    if (*(int*)self == 0)
        return;
    *(int*)(self + 0xB4) = 1;
    cBXString_operatorE1(self + 0x98, name);
    *(int*)(self + 0xA4) = matchType;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    int locationID = cBENewRaceInterface_getLocationID(cBE_getInterface_Fv(cBE_getBE(), 0));
    int flags = D_005308D0[0];
    int event = *(signed char*)(Race_25F4C0() + 0x48);
    int track = *(signed char*)(Race_25F4C0() + 0x4A);
    int y = D_00534B34[0];
    int ai = *(int*)(Race_25F4C0() + 0x3C);
    bool multipliers = (flags & 0x40) != 0;
    bool powerUps = (flags & 0x100) != 0;
    bool pointIcons = (flags & 0x80) != 0;
    char buf[0x40];
    sprintf(buf, str_pct_s_pct_d_pct_d_pct_d_pct_d_pct_d_pct_b, str_challenge, locationID, event, track, y, multipliers, powerUps, ai, pointIcons,
            cCommSystem_getSignature_F4C0(cCommSystem_mCommSystem),
            ((sDsStats_25F4C0*)self)->fBC, ((sDsStats_25F4C0*)self)->fC0, ((sDsStats_25F4C0*)self)->fC4, ((sDsStats_25F4C0*)self)->fB8);
    func_0025FB80_v(self, *(int*)(self + 0x98), buf);
    *(int*)self = 22;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F628);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern "C" void func_003E8D70(void* p, int a);
extern "C" void func_0025F858(LobbyMan* self);
extern "C" void func_0025F740(LobbyMan* self, int type);
void* func_0025FC48(void* self);

struct sMsgListNodeF628 {
    sMsgListNodeF628* next;
    sMsgListNodeF628* prev;
    void* data;
};

struct sMsgListIterF628 {
    sMsgListNodeF628* node;
    sMsgListIterF628(sMsgListNodeF628* x) : node(x) {}
    sMsgListIterF628(const sMsgListIterF628& x) : node(x.node) {}
};

static inline sMsgListIterF628 sMsgListF628_insert(sMsgListIterF628 pos, void* const& x)
{
    sMsgListNodeF628* tmp = (sMsgListNodeF628*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Cancels the lobby challenge or game in progress and optionally tells the front end.
 *
 * Does nothing when mState is 0. In the challenge states 21, 22 and 24 it first sends "abortChal" to mOpponent
 * (func_0025FC48) when @p flag is set, cancels the pending request (func_003E8D70(mpLobbyApi, mRequestId)) and sends an
 * empty "chal" request (func_0025F858, which sets mState to 1). In state 25 (in game) it calls func_0025F740(self, 0)
 * instead. With @p notify set it then posts an online::Event of type 238 (vtable online_Event_vtbl) on mEventList. The update
 * func_0025AC50 calls it when the state-24 timeout runs out; the challenge popup calls it to cancel.
 *
 * @param selfp   the lobby manager
 * @param notify  nonzero to post the type-238 event
 * @param flag    nonzero to send "abortChal" to the opponent first (challenge states only)
 */
extern "C" void func_0025F628(LobbyMan* selfp, int notify, int flag)
{
    int st = *(int*)selfp;
    if (st != 0) {
        if (st == 21 || st == 22 || st == 0x18) {
            if (flag)
                func_0025FC48(((char*)selfp));
            func_003E8D70(selfp->mpLobbyApi, selfp->mRequestId);
            func_0025F858(selfp);
        } else if (st == 25) {
            func_0025F740(selfp, 0);
        }
        if (notify) {
            int type = 238;
            int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            netEvent[0] = type;
            *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
            void* mp = netEvent;
            sMsgListF628_insert(*(sMsgListNodeF628**)(((char*)selfp) + 0xF0), mp);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F740);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_PERS[];
extern char D_004A3218[];

struct sMsgListNodeF740 {
    sMsgListNodeF740* next;
    sMsgListNodeF740* prev;
    void* data;
};

struct sMsgListIterF740 {
    sMsgListNodeF740* node;
    sMsgListIterF740(sMsgListNodeF740* x) : node(x) {}
    sMsgListIterF740(const sMsgListIterF740& x) : node(x.node) {}
};

static inline sMsgListIterF740 sMsgListF740_insert(sMsgListIterF740 pos, void* const& x)
{
    sMsgListNodeF740* tmp = (sMsgListNodeF740*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Leaves the lobby game in progress and optionally posts an event of the given type.
 *
 * Does nothing when mState is 0. In state 25 (in game) it sets mTimeout to 20.0, sends a "chal" request (func_003E8B10,
 * no callback) whose record holds PERS="*" (D_004A3218), and sets mState to 1. When @p type is nonzero it then posts an
 * online::Event of that type (vtable online_Event_vtbl) on mEventList. The challenge-received popup calls it to back out of an
 * accepted game.
 *
 * @param selfp  the lobby manager
 * @param type   event type to post, or 0 for none
 */
extern "C" void func_0025F740(LobbyMan* selfp, int type)
{
    if (*(int*)selfp != 0) {
        if (*(int*)selfp == 25) {
            char buf[0x100];
            selfp->mTimeout = 20.0f;
            buf[0] = 0;
            cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PERS, D_004A3218);
            func_003E8B10(selfp->mpLobbyApi, 0x6368616C, buf, 0, 0);
            *(int*)selfp = 1;
        }
        if (type != 0) {
            int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            netEvent[0] = type;
            *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
            void* mp = netEvent;
            sMsgListF740_insert(*(sMsgListNodeF740**)(((char*)selfp) + 0xF0), mp);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F858);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);

/**
 * Sends an empty "chal" request and sets the lobby state back to 1 (ready).
 *
 * The record is the empty string D_004A2F40 and no reply callback is registered.
 */
extern "C" void func_0025F858(LobbyMan* self)
{
    func_003E8B10(self->mpLobbyApi, 0x6368616C, D_004A2F40, 0, 0);
    *(int*)self = 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onchallengeresponse);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_ErrorEvent_vtbl[];
extern char D_004A2F40[];
extern char str_MODE[];
extern char str_chal[];
extern char str_idle[];
extern char str_play[];
extern float D_004A5154;
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");
void* func_0025FC48(void* self);
extern "C" void func_0025FC70(LobbyMan* self, int a1, int a2);
extern "C" int func_004165A8(void*, void*);

struct sMsgListNodeF8A0 {
    sMsgListNodeF8A0* next;
    sMsgListNodeF8A0* prev;
    void* data;
};

struct sMsgListIterF8A0 {
    sMsgListNodeF8A0* node;
    sMsgListIterF8A0(sMsgListNodeF8A0* x) : node(x) {}
    sMsgListIterF8A0(const sMsgListIterF8A0& x) : node(x.node) {}
};

static inline sMsgListIterF8A0 sMsgListF8A0_insert(sMsgListIterF8A0 pos, void* const& x)
{
    sMsgListNodeF8A0* tmp = (sMsgListNodeF8A0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Handles the lobby server's reply to a "chal" (challenge) request.
 *
 * The callback thunk func_00268EA0 calls it; func_0025FA48 and func_0025FAD0 register it. It reads the reply's "MODE"
 * field (up to 15 characters, default ""). A reply with an error code (the word at @p msg + 0x8) in state 21 sets
 * mState to 1, sends "abortChal" to mOpponent (func_0025FC48) and posts an online error event of type 258 (vtable
 * online_ErrorEvent_vtbl) carrying the code. Otherwise, in state 21: MODE "chal" sends "lockChal" to mOpponent and enters state 24
 * (func_0025FC70); "play" sets mState to 24 and mTimeout and the countdown D_004A5154 to 20.0; "idle" does nothing.
 *
 * @param msg   the LobbyApi reply: error code at +0x8, tag record pointer at +0xC
 */
extern "C" void LobbyMan_onchallengeresponse(LobbyMan* self, char* msg)
{
    char buf[16];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_MODE), buf, 0x10, D_004A2F40);
    if (*(int*)(msg + 0x8) != 0) {
        if (*(int*)self == 21) {
            *(int*)self = 1;
            func_0025FC48(((char*)self));
            int* event = (int*)operator new(0xC, str_NetEvent, 0x20000000, 0);
            event[0] = 258;
            *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
            event[2] = *(int*)(msg + 0x8);
            void* mp = event;
            sMsgListF8A0_insert(*(sMsgListNodeF8A0**)(((char*)self) + 0xF0), mp);
        }
    } else if (func_004165A8(buf, str_chal) == 0) {
        if (*(int*)self == 21)
            func_0025FC70(self, (int)self->mOpponent, 0x18);
    } else if (func_004165A8(buf, str_idle) == 0) {
    } else if (func_004165A8(buf, str_play) == 0) {
        if (*(int*)self == 21) {
            *(int*)self = 0x18;
            self->mTimeout = 20.0f;
            D_004A5154 = 20.0f;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025FA48);
#ifdef SKIP_ASM
extern char str_PERS[];
extern char str_HOST[];
extern char D_004A3240[];
extern "C" void func_00268EA0(void* self, void* a1, int a2);

/**
 * Sends a "chal" challenge request for the given persona as the host side and enters state 21.
 *
 * The record holds PERS = @p name and HOST="1" (D_004A3240). It sets mState to 1, then func_0025B608 sends the request
 * with the reply callback func_00268EA0 (which runs LobbyMan_onchallengeresponse), sets mState to 21 and mTimeout to 20.0 and stores
 * mRequestId. Called from LobbyMan_onchatmsg.
 *
 * @param name  persona name sent as PERS
 */
extern "C" void func_0025FA48(LobbyMan* self, const char* name)
{
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PERS, name);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_HOST, D_004A3240);
    *(int*)self = 1;
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 21, 0x6368616C, buf, (int)func_00268EA0, 20.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_0025FAD0);
#ifdef SKIP_ASM
extern char str_PERS[];
extern char str_HOST[];
extern char D_004A3250[];
extern "C" void func_00268EA0(void* self, void* a1, int a2);

/**
 * Sends a "chal" challenge request for the given persona as the non-host side and enters state 24.
 *
 * Like func_0025FA48(), but sends HOST="0" (D_004A3250) and the new state is 24.
 *
 * @param name  persona name sent as PERS
 */
extern "C" void func_0025FAD0(LobbyMan* self, const char* name)
{
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PERS, name);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_HOST, D_004A3250);
    *(int*)self = 1;
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608(self, 0x18, 0x6368616C, buf, (int)func_00268EA0, 20.0f);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FB58);
#ifdef SKIP_ASM
extern "C" void func_0025FC70(LobbyMan* self, int a1, int a2);

/**
 * Records the match type and locks the challenge with a player (state 23).
 *
 * Does nothing when mState is 0; otherwise stores @p matchType in mMatchType and calls func_0025FC70(self, recipient,
 * 23), which sends "lockChal" to @p recipient.
 *
 * @param recipient  name of the challenged player (a string pointer passed as an int)
 * @param matchType  the game kind stored in mMatchType
 */
extern "C" void func_0025FB58(LobbyMan* self, int recipient, int matchType)
{
    if (*(int*)self != 0) {
        self->mMatchType = matchType;
        func_0025FC70(self, recipient, 23);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FB80);
#ifdef SKIP_ASM
extern char str_PRIV[];
extern char str_TEXT[];
extern char str_ATTR[];
extern "C" int cDirtysock_tag_TagFieldSetFlags(char* record, int len, const char* name, int value);
extern "C" void func_00268EC8(void* self, void* a1, int a2);

/**
 * Sends a private lobby message (a "mesg" request) to a player.
 *
 * Builds a 256-byte record with PRIV = @p recipient, TEXT = @p text and ATTR flags 0x40004000, and sends it with
 * func_003E8B10(mpLobbyApi, 'mesg', ..). The reply callback func_00268EC8 runs the empty func_0025FC20. It does not
 * touch mState or mRequestId. The challenge protocol uses it for its control words ("decline", "abortChal",
 * "lockChal").
 *
 * @param recipient  name of the receiving player (a string pointer passed as an int)
 * @param text       the message text
 * @return the request handle func_003E8B10() returns
 */
// PORT: a1 carries a string pointer in an int (the unit's declaration).
extern "C" void* func_0025FB80(LobbyMan* self, int recipient, void* text)
{
    char buf[0x100];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_PRIV, (const char*)recipient);
    cDirtysock_tag_TagFieldSetString(buf, 0x100, str_TEXT, (const char*)text);
    cDirtysock_tag_TagFieldSetFlags(buf, 0x100, str_ATTR, 0x40004000);
    // PORT: function pointer passed through func_003E8B10's int parameter.
    return (void*)func_003E8B10(self->mpLobbyApi, 0x6D657367, buf, (int)func_00268EC8, self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC20__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the reply handler for private "mesg" requests (run by the thunk func_00268EC8) has an empty body.
 */
void func_0025FC20(void* self)
{
}
#endif

extern void* D_004A3270[];
extern "C" void* func_0025FB80(LobbyMan*, int, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC28__FPvi);
#ifdef SKIP_ASM
/**
 * Sends "decline" as a private lobby message to a player (func_0025FB80).
 *
 * @param playerName  name of the player whose challenge is declined (a string pointer passed as an int)
 * @return func_0025FB80()'s result
 */
void* func_0025FC28(void* self, int playerName)
{
    return func_0025FB80((LobbyMan*)self, playerName, (void*)D_004A3270);
}
#endif

extern void* D_004807F8[];
extern "C" void* func_0025FB80(LobbyMan*, int, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC48__FPv);
#ifdef SKIP_ASM
/**
 * Sends "abortChal" as a private lobby message to the current opponent (mOpponent, +0x98) with func_0025FB80().
 *
 * @return func_0025FB80()'s result
 */
void* func_0025FC48(void* self)
{
    return func_0025FB80((LobbyMan*)self, *(int*)((char*)self + 0x98), (void*)D_004807F8);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC70);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_lockChal_b[];
extern float D_004A5154;

struct sMsgListNode_0025FC70 {
    sMsgListNode_0025FC70* next;
    sMsgListNode_0025FC70* prev;
    void* data;
};

struct sMsgListIter_0025FC70 {
    sMsgListNode_0025FC70* node;
    sMsgListIter_0025FC70(sMsgListNode_0025FC70* x) : node(x) {}
    sMsgListIter_0025FC70(const sMsgListIter_0025FC70& x) : node(x.node) {}
};

static inline sMsgListIter_0025FC70 sMsgList_0025FC70_insert(sMsgListIter_0025FC70 pos, void* const& x)
{
    sMsgListNode_0025FC70* tmp = (sMsgListNode_0025FC70*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Sends "lockChal" to a player, switches to the given challenge state and restarts the challenge timer.
 *
 * After the private message (func_0025FB80) it stores @p newState in mState, posts an online::Event of type 272 (vtable
 * online_Event_vtbl) on mEventList, sets mTimeout to 20.0 and the countdown D_004A5154 to 21.0.
 *
 * @param playerName    name of the player (a string pointer passed as an int)
 * @param newState    the new mState (23 from func_0025FB58, 24 from LobbyMan_onchallengeresponse)
 */
extern "C" void func_0025FC70(LobbyMan* self, int playerName, int newState)
{
    func_0025FB80(self, playerName, str_lockChal_b);
    int kind = 272;
    *(int*)self = newState;
    int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = kind;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    void* mp = netEvent;
    sMsgList_0025FC70_insert(*(sMsgListNode_0025FC70**)((char*)self + 0xF0), mp);
    self->mTimeout = 20.0f;
    D_004A5154 = 21.0f;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FD50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

extern char str_StlBlock_b[];

// The tag message queue (a g++ 2.95 list<void*> at +0xF0 of the session object): node and iterator.
// list<void*>::insert(pos, x)
static inline sMsgListIter sMsgList_insert(sMsgListIter pos, void* const& x)
{
    sMsgListNode* tmp = (sMsgListNode*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sName7 {
    char c[7];
};

// The network game state cBENetworkInterface_networkOpt (D_00534B34 = +0x4 and D_00534E58 = +0x328 are their own symbols).
struct sNetState {
    int active;                     // 0x000
    int f4;                         // 0x004
    int chan;                       // 0x008
    int port;                       // 0x00C
    float rate;                     // 0x010
    char pad_0x014[0x328 - 0x14];
    int charID0;                    // 0x328
    int charID;                     // 0x32C
    char pad_0x330[0x4A8 - 0x330];
    sName7 name;                    // 0x4A8
};


extern void* online_Event_vtbl[];

extern char str_NetEvent[];


void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" void* cBE_getBE();

extern "C" int cDirtysock_tag_TagFieldSetNumber(char* buf, int size, const char* key, int value);

extern "C" void func_0025B608(LobbyMan* self, int a1, int tag, void* cb, int a4, float f);

extern "C" int cDirtysock_tag_TagFieldSetString(char* buf, int size, const char* key, const char* value);


// Queue a simple tag message {type, online_Event_vtbl vtable} on the session's list.
static inline void postMsg(char* self, int type)
{
    int* m = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    m[0] = type;
    *(void***)((char*)m + 0x4) = online_Event_vtbl;
    void* mp = m;
    sMsgList_insert(*(sMsgListNode**)(self + 0xF0), mp);
}

// One rider's race statistics (cBENetworkInterface_networkOpt + 0x330 / + 0x3DC).
struct sRaceStats {
    int f00;                        // 0x00
    int f04;                        // 0x04
    int f08;                        // 0x08
    int pad_0x0C;
    int f10;                        // 0x10
    int f14;                        // 0x14
    int f18;                        // 0x18
    int pad_0x1C[2];
    int f24;                        // 0x24
    int f28;                        // 0x28
    int f2C;                        // 0x2C
    int f30;                        // 0x30
    int f34;                        // 0x34
    int f38;                        // 0x38
    int f3C;                        // 0x3C
    int f40;                        // 0x40
    float f44;                      // 0x44
    float f48;                      // 0x48
    float f4C;                      // 0x4C
    float f50;                      // 0x50
    float f54;                      // 0x54
    float f58;                      // 0x58
    int pad_0x5C;
    float f60;                      // 0x60
    float f64;                      // 0x64
    float f68;                      // 0x68
    float f6C;                      // 0x6C
    int f70;                        // 0x70
    int f74;                        // 0x74
    int f78;                        // 0x78
    int f7C;                        // 0x7C
    int f80;                        // 0x80
    int f84;                        // 0x84
    int pad_0x88[2];
    int f90;                        // 0x90
    int pad_0x94[2];
    int f9C;                        // 0x9C
    int pad_0xA0;
    int fA4;                        // 0xA4
    int fA8;                        // 0xA8
};


extern "C" int cDirtysock_tag_TagFieldSetEpoch(char* record, int len, char* name, unsigned int epoch);
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
// PORT: func_004139F8 is libgcc's __fixunssfsi (float -> unsigned); called by its address name.
extern "C" unsigned int func_004139F8(float f);
extern "C" void func_00268EF0(void* self, void* a1, int a2);
extern signed char D_00535C12[];
extern int D_005366A8[];
extern int D_00536640[];
extern char str_WHEN[];
extern char str_AUTH[];
extern char str_REPT[];
extern char str_COMMERR[];
extern char str_SCOMERR[];
extern char str_PINGRSLT[];
extern char str_STALLCNT[];
extern char str_CERRSTAT[];
extern char str_CERRFLGS[];
extern char str_DIDQUIT0[];
extern char str_DIDQUIT1[];
extern char str_FINISH0[];
extern char str_FINISH1[];
extern char str_DNF0[];
extern char str_DNF1[];
extern char str_RSLTSLCK[];
extern char str_RACETRA0[];
extern char str_RACEEVE0[];
extern char str_DESYNC[];
extern char str_NAME0[];
extern char str_QUIT0[];
extern char str_CHEAT0[];
extern char str_DISC0[];
extern char str_CHARACTER0[];
extern char str_RACETIM0[];
extern char str_SCORE0[];
extern char str_CHARACTER1[];
extern char str_RACETIM1[];
extern char str_SCORE1[];
extern char str_NAME1[];
extern char str_QUIT1[];
extern char str_CHEAT1[];
extern char str_DISC1[];
extern char str_SPINDEG0[];
extern char str_FLIPDEG0[];
extern char str_NUMGRAB0[];
extern char str_NUMRAIL0[];
extern char str_NUMTLAN0[];
extern char str_NUMUTRI0[];
extern char str_NUMRESE0[];
extern char str_NUMWIPE0[];
extern char str_NUMFRAG0[];
extern char str_NUMDEAD0[];
extern char str_NUMPLAY0[];
extern char str_NUMCCHL0[];
extern char str_NUMPLAB0[];
extern char str_NUMSLAB0[];
extern char str_AIRTIME0[];
extern char str_GRABTIM0[];
extern char str_RAILDIS0[];
extern char str_BAIRTIM0[];
extern char str_BGRABTI0[];
extern char str_BRAILDI0[];
extern char str_MAXSPEE0[];
extern char str_MAXAVSP0[];
extern char str_SUMSPEE0[];
extern char str_SUMTICK0[];
extern char str_TRICKL10[];
extern char str_TRICKL20[];
extern char str_TRICKL30[];
extern char str_TRICKL40[];
extern char str_TRICKL50[];
extern char str_TRICKL60[];
extern char str_BESTRPO0[];
extern char str_PLOSTWI0[];
extern char str_PLOSTRE0[];
extern char str_SPINDEG1[];
extern char str_FLIPDEG1[];
extern char str_NUMGRAB1[];
extern char str_NUMRAIL1[];
extern char str_NUMTLAN1[];
extern char str_NUMUTRI1[];
extern char str_NUMRESE1[];
extern char str_NUMWIPE1[];
extern char str_NUMFRAG1[];
extern char str_NUMDEAD1[];
extern char str_NUMPLAY1[];
extern char str_NUMCCHL1[];
extern char str_NUMPLAB1[];
extern char str_NUMSLAB1[];
extern char str_AIRTIME1[];
extern char str_GRABTIM1[];
extern char str_RAILDIS1[];
extern char str_BAIRTIM1[];
extern char str_BGRABTI1[];
extern char str_BRAILDI1[];
extern char str_MAXSPEE1[];
extern char str_MAXAVSP1[];
extern char str_SUMSPEE1[];
extern char str_SUMTICK1[];
extern char str_TRICKL11[];
extern char str_TRICKL21[];
extern char str_TRICKL31[];
extern char str_TRICKL41[];
extern char str_TRICKL51[];
extern char str_TRICKL61[];
extern char str_BESTRPO1[];
extern char str_PLOSTWI1[];
extern char str_PLOSTRE1[];
extern char str_POINTS0[];
extern char str_POINTS1[];

static inline cGameComm* Race() { return cGameComm_mGameComm; }

// 1 when the value is set.
static inline int IsSet(int v)
{
    int r = 0;
    if (v != 0)
        r = 1;
    return r;
}

// Time from the race start to a finish time (0 when not set).
static inline int RaceTime(int end)
{
    int t = 0;
    if (end != 0)
        t = end - Race()->unk_034;
    return t;
}

/**
 * Reports the result of a finished online race to the lobby server (a "rank" request).
 *
 * Does nothing when mState is 0. Builds a 0x800-byte tag record from cGameComm_mGameComm, the network options
 * cBENetworkInterface_networkOpt and both riders' stats. The *0 fields describe the host and the *1 fields the other
 * rider: bIsHost picks whether the host is this machine's side A (mLocalFinishedRace, mLocalDisconnected, unk_038,
 * mCheating[0], @p statsA, @p idxA) or the peer's side B (the remote flags, unk_03C, mCheating[1], @p statsB, @p idxB).
 * Fields: WHEN (mWhen), REPT (the reporter: mHostName on the host, else mOppoName), AUTH (mAuth, only with @p
 * sendName), COMMERR, SCOMERR, PINGRSLT, STALLCNT, CERRSTAT, CERRFLGS, DIDQUIT0/1, FINISH0/1, DNF0/1 (D_005366A8),
 * RSLTSLCK (game-mode manager +0x9C), RACETRA0 (the location id), RACEEVE0 (D_00535C12[0]; for locations 14..16 it
 * becomes 0 when that is 4, else 1), DESYNC, then per rider NAME (mHostName / mOppoName), QUIT (race time from unk_034
 * to the end time when the rider did not finish), CHEAT (mCheating), DISC (the same time when a significant comm error
 * is put on that side), CHARACTER, RACETIM or SCORE (D_00536640; SCORE when RACEEVE0 is nonzero) and the trick and
 * speed totals (SPINDEG .. PLOSTRE, floats converted with func_004139F8). It sends the record with func_0025B608 (state
 * unchanged, callback func_00268EF0, which runs LobbyMan_onresultsreportresponse, timeout 20.0) and posts an online::Event of type 0x100
 * on mEventList. Likely bug: CHARACTER0 is always MyCharacterID, so on the non-host side both CHARACTER0 and CHARACTER1
 * are this player's character.
 *
 * @param sendName  nonzero to include the AUTH field (the trophy-room caller passes bIsRankedGame)
 * @param idxA      this machine's rider index into D_005366A8 / D_00536640 (the caller passes !bIsHost)
 * @param idxB      the peer's rider index (the caller passes bIsHost != 0)
 * @param statsA    this machine's race statistics (networkOpt MyStats)
 * @param statsB    the peer's race statistics (networkOpt OpponentStats)
 */
// self is a const parameter: its spill reloads are unchanging memory, so sched2 lets the message stores pass them.
extern "C" void func_0025FD50(char* const self, int sendName, int idxA, int idxB, sRaceStats* statsA, sRaceStats* statsB)
{
    char buf[0x800];
    if (*(int*)self == 0)
        return;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    char* prof = (char*)SSXApp->GameModeMan;
    int quitA = 0;
    int quitB = 0;
    if (Race()->mCommErrorWasSignificant != 0) {
        if (Race()->unk_09C == 0)
            quitA = 1;
        else
            quitB = 1;
    }
    int finA = 0;
    int finB = 0;
    if (Race()->mLocalFinishedRace == 0)
        finA = 1;
    if (Race()->mRemoteFinishedRace == 0)
        finB = 1;
    int myIdx;
    char* myName;
    int myTime;
    int myQuitTime;
    int myDone;
    int myOut;
    sRaceStats* mine;
    int myScore;
    int oppIdx;
    int oppChar;
    int oppTime;
    int oppQuitTime;
    int oppOut;
    int oppDone;
    sRaceStats* opp;
    int oppScore;
    if (cBENetworkInterface_networkOpt.bIsHost != 0) {
        myIdx = idxA;
        myName = *(char**)(self + 0x90);
        myTime = finA ? RaceTime(Race()->unk_038) : 0;
        myQuitTime = quitA ? RaceTime(Race()->unk_038) : 0;
        myDone = 0;
        if (Race()->mLocalDisconnected != 0)
            myDone = 1;
        myOut = 0;
        if (Race()->mLocalFinishedRace != 0)
            myOut = 1;
        mine = statsA;
        myScore = Race()->mCheating[0];
        oppIdx = idxB;
        oppChar = cBENetworkInterface_networkOpt.OpponentCharacterID;
        oppTime = finB ? RaceTime(Race()->unk_03C) : 0;
        oppQuitTime = quitB ? RaceTime(Race()->unk_03C) : 0;
        oppDone = 0;
        if (Race()->mRemoteDisconnected != 0)
            oppDone = 1;
        opp = statsB;
        oppScore = Race()->mCheating[1];
        oppOut = 0;
        if (Race()->mRemoteFinishedRace != 0)
            oppOut = 1;
    } else {
        myIdx = idxB;
        myName = *(char**)(self + 0x94);
        myTime = finB ? RaceTime(Race()->unk_03C) : 0;
        myQuitTime = quitB ? RaceTime(Race()->unk_03C) : 0;
        myDone = 0;
        if (Race()->mRemoteDisconnected != 0)
            myDone = 1;
        myOut = 0;
        if (Race()->mRemoteFinishedRace != 0)
            myOut = 1;
        mine = statsB;
        myScore = Race()->mCheating[1];
        oppIdx = idxA;
        oppChar = cBENetworkInterface_networkOpt.MyCharacterID;
        oppTime = finA ? RaceTime(Race()->unk_038) : 0;
        oppQuitTime = quitA ? RaceTime(Race()->unk_038) : 0;
        oppDone = 0;
        if (Race()->mLocalDisconnected != 0)
            oppDone = 1;
        opp = statsA;
        oppScore = Race()->mCheating[0];
        oppOut = 0;
        if (Race()->mLocalFinishedRace != 0)
            oppOut = 1;
    }
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetEpoch(buf, 0x800, str_WHEN, *(unsigned int*)(self + 0xA0));
    cDirtysock_tag_TagFieldSetString(buf, 0x800, str_REPT, myName);
    if (sendName != 0)
        cDirtysock_tag_TagFieldSetString(buf, 0x800, str_AUTH, *(const char**)(self + 0x9C));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_COMMERR, Race()->mCommError);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SCOMERR, Race()->mCommErrorWasSignificant);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_PINGRSLT, Race()->unk_09C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_STALLCNT, Race()->mStallCount);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_CERRSTAT, Race()->mStateOnCommError);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_CERRFLGS, ((int)Race()->mFlagsOnCommError));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_DIDQUIT0, myDone);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_DIDQUIT1, oppDone);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_FINISH0, myOut);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_FINISH1, oppOut);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_DNF0, IsSet(D_005366A8[myIdx]));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_DNF1, IsSet(D_005366A8[oppIdx]));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_RSLTSLCK, IsSet(*(int*)(prof + 0x9C)));
    int mode = cBENewRaceInterface_getLocationID(iface);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_RACETRA0, mode);
    int ranked = D_00535C12[0];
    if (mode >= 0xE && mode <= 0x10) {
        if (ranked == 4)
            ranked = 0;
        else
            ranked = 1;
    }
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_RACEEVE0, ranked);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_DESYNC, Race()->mDesync);
    cDirtysock_tag_TagFieldSetString(buf, 0x800, str_NAME0, *(const char**)(self + 0x90));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_QUIT0, myTime);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_CHEAT0, myScore);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_DISC0, myQuitTime);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_CHARACTER0, cBENetworkInterface_networkOpt.MyCharacterID);
    if (ranked == 0)
        cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_RACETIM0, D_00536640[myIdx]);
    else
        cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SCORE0, D_00536640[myIdx]);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SPINDEG0, mine->f00);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_FLIPDEG0, mine->f04);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMGRAB0, mine->f08);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMRAIL0, mine->f10);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMTLAN0, mine->f14);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMUTRI0, mine->f18);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMRESE0, mine->f24);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMWIPE0, mine->f28);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMFRAG0, mine->f2C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMDEAD0, mine->f30);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMPLAY0, mine->f34);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMCCHL0, mine->f38);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMPLAB0, mine->f3C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMSLAB0, mine->f40);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_AIRTIME0, func_004139F8(mine->f44));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_GRABTIM0, func_004139F8(mine->f48));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_RAILDIS0, func_004139F8(mine->f4C));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BAIRTIM0, func_004139F8(mine->f50));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BGRABTI0, func_004139F8(mine->f54));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BRAILDI0, func_004139F8(mine->f58));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_MAXSPEE0, func_004139F8(mine->f60));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_MAXAVSP0, func_004139F8(mine->f64));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SUMSPEE0, func_004139F8(mine->f68));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SUMTICK0, func_004139F8(mine->f6C));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL10, mine->f70);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL20, mine->f74);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL30, mine->f78);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL40, mine->f7C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL50, mine->f80);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL60, mine->f84);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BESTRPO0, mine->f90);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_POINTS0, mine->f9C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_PLOSTWI0, mine->fA4);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_PLOSTRE0, mine->fA8);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_CHARACTER1, oppChar);
    if (ranked == 0)
        cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_RACETIM1, D_00536640[oppIdx]);
    else
        cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SCORE1, D_00536640[oppIdx]);
    cDirtysock_tag_TagFieldSetString(buf, 0x800, str_NAME1, *(const char**)(self + 0x94));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_QUIT1, oppTime);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_CHEAT1, oppScore);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_DISC1, oppQuitTime);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SPINDEG1, opp->f00);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_FLIPDEG1, opp->f04);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMGRAB1, opp->f08);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMRAIL1, opp->f10);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMTLAN1, opp->f14);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMUTRI1, opp->f18);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMRESE1, opp->f24);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMWIPE1, opp->f28);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMFRAG1, opp->f2C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMDEAD1, opp->f30);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMPLAY1, opp->f34);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMCCHL1, opp->f38);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMPLAB1, opp->f3C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_NUMSLAB1, opp->f40);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_AIRTIME1, func_004139F8(opp->f44));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_GRABTIM1, func_004139F8(opp->f48));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_RAILDIS1, func_004139F8(opp->f4C));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BAIRTIM1, func_004139F8(opp->f50));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BGRABTI1, func_004139F8(opp->f54));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BRAILDI1, func_004139F8(opp->f58));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_MAXSPEE1, func_004139F8(opp->f60));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_MAXAVSP1, func_004139F8(opp->f64));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SUMSPEE1, func_004139F8(opp->f68));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_SUMTICK1, func_004139F8(opp->f6C));
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL11, opp->f70);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL21, opp->f74);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL31, opp->f78);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL41, opp->f7C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL51, opp->f80);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_TRICKL61, opp->f84);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_BESTRPO1, opp->f90);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_POINTS1, opp->f9C);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_PLOSTWI1, opp->fA4);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x800, str_PLOSTRE1, opp->fA8);
    // PORT: function pointer passed through func_0025B608's int parameter.
    func_0025B608((LobbyMan*)self, *(int*)self, 0x72616E6B, buf, (int)func_00268EF0, 20.0f);
    postMsg(self, 0x100);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", LobbyMan_onresultsreportresponse);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];
extern int D_00534B34[];

struct sMsgListNode0C68 {
    sMsgListNode0C68* next;
    sMsgListNode0C68* prev;
    void* data;
};

struct sMsgListIter0C68 {
    sMsgListNode0C68* node;
    sMsgListIter0C68(sMsgListNode0C68* x) : node(x) {}
    sMsgListIter0C68(const sMsgListIter0C68& x) : node(x.node) {}
};

static inline sMsgListIter0C68 sMsgList0C68_insert(sMsgListIter0C68 pos, void* const& x)
{
    sMsgListNode0C68* tmp = (sMsgListNode0C68*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_0C68(char* lst, void* msg)
{
    void* mp = msg;
    sMsgList0C68_insert(*(sMsgListNode0C68**)(lst + 4), mp);
}

/**
 * Handles the lobby server's reply to the race results report ("rank" request).
 *
 * The callback thunk func_00268EF0 calls it (registered by func_0025FD50). A reply without an error code (the word at
 * @p msg + 0x8), or one whose error is 'rank' while the game is unranked (bIsRankedGame, D_00534B34, is 0), posts an
 * online::Event of type 257 on mEventList; any other error first posts an error event of type 258 (vtable online_ErrorEvent_vtbl)
 * carrying the code, then the type-257 event. It also fetches back-end interface 7 and ignores it.
 *
 * @param msg   the LobbyApi reply: error code at +0x8
 */
extern "C" void LobbyMan_onresultsreportresponse(LobbyMan* self, char* msg)
{
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (*(int*)(msg + 0x8) == 0 || (*(int*)(msg + 0x8) == 0x72616E6B && D_00534B34[0] == 0)) {
        int type = 257;
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        event[0] = type;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        post_0C68(((char*)self) + 0xEC, event);
    } else {
        int err = *(int*)(msg + 0x8);
        char* lst = ((char*)self) + 0xEC;
        int* event = (int*)cMemMan_alloc(0xC, str_NetEvent, 0x20000000, 0);
        event[0] = 258;
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        *(void***)((char*)event + 0x4) = online_ErrorEvent_vtbl;
        event[2] = err;
        post_0C68(lst, event);
        int type = 257;
        int* resultEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
        *(void***)((char*)resultEvent + 0x4) = online_Event_vtbl;
        resultEvent[0] = type;
        post_0C68(lst, resultEvent);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00260E50);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void func_003194A8(void* str, const char* fmt, ...);
extern void* cBXString_EmptyString;
extern char str_Duplicate[];
extern char str_Timeout[];
extern char D_00480E20[];

struct sBXStr_00260E50 {
    char* p;
};

/**
 * Builds the display text for a lobby error code given as a four-character tag.
 *
 * 'dupl' gives "Duplicate", 'time' gives "Timeout"; any other code is formatted with func_003194A8 as "Unknown Error
 * Code %08x '%c%c%c%c'\n" (D_00480E20) from the code and its four bytes. Used by the debug front end (cDebugLobbyScreen_draw).
 *
 * @param ret   the cBXString to construct (the g++ return slot)
 * @param tag   the error code
 * @return @p ret
 */
// Builds the display name of a 4-character tag.
extern "C" void* func_00260E50(void* ret, void* self, unsigned int tag)
{
    switch (tag)
    {
    case 0x6475706C:
        cBXString_cBXString2(ret, str_Duplicate);
        break;
    case 0x74696D65:
        cBXString_cBXString2(ret, str_Timeout);
        break;
    default:
    {
        sBXStr_00260E50 tagText;
        tagText.p = (char*)cBXString_EmptyString;
        func_003194A8(&tagText, D_00480E20, tag, tag >> 24, (tag >> 16) & 0xFF, (tag >> 8) & 0xFF, tag & 0xFF);
        cBXString_cBXString1(ret, &tagText);
        cBXString__cBXString(&tagText, 2);
        break;
    }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00260F10);
#ifdef SKIP_ASM
extern "C" void cBXString_Reset(void* self);

/**
 * Resets the lobby manager's new-account form (mNewAccount) to its defaults.
 *
 * Empties its five strings (mName, mPassword, unk_08, mMail, mParentMail) with cBXString_Reset and sets mSpam to {1,
 * 1}, mBornMonth and mBornDay to 1, mBornYear to 1985, mGender to 0 and unk_40 to 0. mNameOptions and the mChange*
 * flags are left alone. The constructor LobbyMan_LobbyMan and the account screens call it.
 */
extern "C" void func_00260F10(LobbyMan* self)
{
    cBXString_Reset((char*)self + 0x260);
    cBXString_Reset((char*)self + 0x264);
    cBXString_Reset((char*)self + 0x268);
    cBXString_Reset((char*)self + 0x26C);
    cBXString_Reset((char*)self + 0x270);
    self->mNewAccount.mSpam[0] = 1;
    self->mNewAccount.mSpam[1] = 1;
    self->mNewAccount.mBornMonth = 1;
    self->mNewAccount.mGender = 0;
    self->mNewAccount.mBornDay = 1;
    self->mNewAccount.mBornYear = 1985;
    self->mNewAccount.unk_40 = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00260F80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_BuddyMan[];
extern void* cBXString_EmptyString;

/**
 * Creates the buddy-list manager singleton BuddyMan_mBuddyMan.
 *
 * Allocates 0xF0 bytes (cMemMan_alloc, tag "BuddyMan") and runs the inlined constructor: miPort -1, miMsgTimeout
 * 0x15180 (one day in seconds), mResource the empty cBXString, and the message and challenger vectors, mpBuddyApi,
 * mpRoster, miError, mState, mbDisconnected, mbLoggedIn, unk_0E4 and mbRosterDone zeroed. It does not check for an
 * existing instance; callers test BuddyMan_mBuddyMan first. (The C body's comment calling it the memory-card manager is
 * wrong.)
 */
// Creates the memory-card manager singleton (BuddyMan_mBuddyMan).
extern "C" void func_00260F80(void)
{
    // An unused 32-byte local is needed to reproduce the 0x30-byte frame.
    int framePad[8];
    char* buddyMan = (char*)operator new(0xF0, str_BuddyMan, 0x20000000, 0);
    *(int*)(buddyMan + 0x40) = -1;
    *(int*)(buddyMan + 0xA4) = 0x15180;
    *(void**)(buddyMan + 0xEC) = cBXString_EmptyString;
    *(int*)(buddyMan + 0xAC) = 0;
    *(int*)(buddyMan + 0xB0) = 0;
    *(int*)(buddyMan + 0xB4) = 0;
    *(int*)(buddyMan + 0xBC) = 0;
    *(int*)(buddyMan + 0xC0) = 0;
    *(int*)(buddyMan + 0xC4) = 0;
    *(int*)(buddyMan + 0xC8) = 0;
    *(int*)(buddyMan + 0xCC) = 0;
    *(int*)(buddyMan + 0xD0) = 0;
    *(int*)(buddyMan + 0xD8) = 0;
    *(int*)(buddyMan + 0xDC) = 0;
    *(int*)(buddyMan + 0xE0) = 0;
    *(int*)(buddyMan + 0xE4) = 0;
    *(int*)(buddyMan + 0xE8) = 0;
    BuddyMan_mBuddyMan = (BuddyMan*)(buddyMan);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261008);
#ifdef SKIP_ASM
void func_00261058(void* self);
struct sVec261070;
extern "C" void func_00261070(BuddyMan* self);
extern "C" void func_00261408(BuddyMan* self);
extern "C" void BuddyMan__BuddyMan(void* self, int flags);

/**
 * Destroys the buddy-list manager singleton, if there is one, and clears BuddyMan_mBuddyMan.
 *
 * Clears the connect settings (func_00261058), deletes the received messages (func_00261070), disconnects
 * (func_00261408), then destroys and frees the object with BuddyMan__BuddyMan(.., 3).
 */
extern "C" void func_00261008(void)
{
    if (BuddyMan_mBuddyMan != 0) {
        func_00261058(BuddyMan_mBuddyMan);
        func_00261070(BuddyMan_mBuddyMan);
        func_00261408(BuddyMan_mBuddyMan);
        if (BuddyMan_mBuddyMan != 0) {
            BuddyMan__BuddyMan(BuddyMan_mBuddyMan, 3);
        }
        BuddyMan_mBuddyMan = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261058__FPv);
#ifdef SKIP_ASM
/**
 * Clears the buddy manager's saved connect settings so it cannot reconnect.
 *
 * Empties mServerName, mUserName and mPassword (first byte 0) and sets miPort to -1, which func_002613C8 treats as "no
 * saved server".
 */
void func_00261058(void* self)
{
    *(char*)self = 0;
    *(int*)((char*)self + 0x40) = -1;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x64) = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261070);
#ifdef SKIP_ASM
struct sVE261070 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj261070 {
    char pad[0xC];
    sVE261070* vt;
};


/**
 * Deletes every received private message, emptying the mMessages vector.
 *
 * Pops the pointers from the back (mMessagesFinish) and destroys each non-null cPrivateMessage through its virtual
 * destructor (vtable at +0xC, slot 1) with delete flags 3.
 */
extern "C" void func_00261070(BuddyMan* self)
{
    while (((sObj261070**)self->mMessagesStart) != ((sObj261070**)self->mMessagesFinish)) {
        sObj261070* message = ((sObj261070**)self->mMessagesFinish)[-1];
        (*(sObj261070***)&self->mMessagesFinish)--;
        if (message != 0) {
            message->vt[1].fn((char*)message + message->vt[1].delta, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002610D0);
#ifdef SKIP_ASM
extern "C" void func_00260F80(void);
extern "C" int func_00261460(void* p);
extern "C" void func_002613C8(BuddyMan* p);
extern "C" void func_0025B800(LobbyMan* self);
extern "C" int func_00262938(BuddyMan* self);
extern "C" void* func_00263630(BuddyMan* self, unsigned int i);
extern "C" int func_00262468(BuddyMan* self, int a1);
extern "C" void func_00261EF8(BuddyMan* self, int a1, int a2, void* out);

static inline void* Mgr_2610D0()
{
    if (BuddyMan_mBuddyMan == 0)
        func_00260F80();
    if (func_00261460(BuddyMan_mBuddyMan) == 0)
        func_002613C8(BuddyMan_mBuddyMan);
    if (BuddyMan_mBuddyMan == 0)
        func_0025B800(LobbyMan_mLobbyMan);
    return BuddyMan_mBuddyMan;
}

/**
 * Adds the sender of each stored private message to the buddy roster when the sender is not on it yet.
 *
 * For each message index below func_00262938() it reads the sender name (the message's first word) and, when
 * func_00262468() finds no roster entry for it, calls func_00261EF8(mgr, sender, 2, &result). Each step re-fetches the
 * manager through an inline accessor that creates BuddyMan_mBuddyMan when missing (func_00260F80), reconnects it when
 * it is not connected (func_002613C8) and disconnects LobbyMan_mLobbyMan (func_0025B800) if it is still null. The
 * roster callback BuddyMan_onrosterupdate calls it at the end of the first roster download.
 */
extern "C" void func_002610D0()
{
    for (int i = 0; i < func_00262938((BuddyMan*)Mgr_2610D0()); i++)
    {
        int* message = (int*)func_00263630((BuddyMan*)Mgr_2610D0(), i);
        if (func_00262468((BuddyMan*)Mgr_2610D0(), *message) < 0)
        {
            int out;
            func_00261EF8((BuddyMan*)Mgr_2610D0(), *message, 2, &out);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261248);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void* cBXString_Concat(void* self, const char* str);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" const char* func_00259198();
extern "C" void func_002614E8(BuddyMan* self, void* a1);
extern "C" void func_003F0818(void* p, const char* a1, int tag, const char* a3, int a4);
extern "C" void func_003F08E8(void* p, const char* name, int a2, int a3, const char* s1, const char* s2, void* cb, void* user);
extern "C" void func_00268F18(void* self, void* a1, int a2);
extern void* cBXString_EmptyString;
extern char D_004A3330[];
extern char str_cso[];


struct sBXStr_261248 {
    char* p;
    sBXStr_261248() : p((char*)cBXString_EmptyString) {}
    sBXStr_261248(const sBXStr_261248& o) : p(o.p) {}
};

/**
 * Connects the buddy manager to a buddy server and logs in.
 *
 * Creates the BuddyApi on first use (func_002614E8 with the resource name, then func_003F0818(mpBuddyApi, resource,
 * 'ssx3', "en", 0)). Copies the host into mServerName, @p port into miPort, the user into mUserName and the password into
 * mPassword; clears miError, mbDisconnected and mbLoggedIn, sets mfTimeout to 30.0 and mState to 1 (connecting). It
 * stores the resource in mResource and calls func_003F08E8(mpBuddyApi, host, port, 30, "user:password", "/cso/" +
 * resource, func_00268F18, self), whose callback runs BuddyMan_onconnect. LobbyMan's LobbyMan_onNewsRetrieved passes the server's
 * BUDDYSERVERNAME / BUDDYPORT values; func_002613C8 reconnects with the saved ones. No length checks on the strcpy's.
 *
 * @param self_  the buddy manager
 * @param name_  buddy server host name (at most 0x3F characters)
 * @param port     buddy server port
 * @param a3_    user name (the persona; at most 0x1F characters)
 * @param a4_    password (at most 0x3F characters)
 * @param a5_    resource name, a string pointer passed as an int
 */
// PORT: the unit declares (void*, void*, int, void*, void*, int); the last int is really a string pointer.
extern "C" void func_00261248(BuddyMan* self_, void* name_, int port, void* a3_, void* a4_, int a5_)
{
    const char* name = (const char*)name_;
    const char* userName = (const char*)a3_;
    const char* password = (const char*)a4_;
    const char* resource = (const char*)a5_;
    if (self_->mpBuddyApi == 0)
    {
        func_002614E8(self_, (void*)resource);
        func_003F0818(self_->mpBuddyApi, resource, 0x73737833, func_00259198(), 0);
    }
    strcpy(((char*)self_), name);
    self_->miPort = port;
    strcpy(((char*)self_) + 0x44, userName);
    strcpy(((char*)self_) + 0x64, password);
    BuddyMan* buddyMan = self_;
    buddyMan->miError = 0;
    buddyMan->mfTimeout = 30.0f;
    buddyMan->mState = 1;
    buddyMan->mbDisconnected = 0;
    buddyMan->mbLoggedIn = 0;
    sBXStr_261248 loginString;
    cBXString_operatorE1(&loginString, userName);
    cBXString_Concat(&loginString, D_004A3330);
    cBXString_Concat(&loginString, password);
    sBXStr_261248 resourceString;
    cBXString_operatorE1(&resourceString, str_cso);
    cBXString_Concat(&resourceString, resource);
    cBXString_operatorE1(((char*)self_) + 0xEC, resource);
    func_003F08E8(self_->mpBuddyApi, name, port, 0x1E, loginString.p, resourceString.p, (void*)func_00268F18, ((char*)self_));
    cBXString__cBXString(&resourceString, 2);
    cBXString__cBXString(&loginString, 2);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002613C8);
#ifdef SKIP_ASM
extern "C" void func_00261248(BuddyMan* self, void* self2, int idx, void* a, void* b, int c);
/**
 * Reconnects the buddy manager with its saved server, port, user, password and resource.
 *
 * Only when miPort is not negative and the low byte of mState is 0 (idle); then calls func_00261248() with mServerName,
 * miPort, mUserName, mPassword and mResource.
 */
extern "C" void func_002613C8(BuddyMan* self)
{
    int idx = self->miPort;
    if (idx >= 0) {
        bool ok = !*(bool*)&self->mState;
        if (ok) {
            func_00261248(self, self, idx, (char*)self + 0x44, (char*)self + 0x64, (int)self->mResource);
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261408);
#ifdef SKIP_ASM
extern "C" void func_00262768(BuddyMan* self, int a1, int a2, int a3, int a4, int a5);
extern "C" void func_00261530(BuddyMan* self);

/**
 * Disconnects the buddy manager and resets its connection flags.
 *
 * First updates the presence with func_00262768(self, 0, 1, 0, 0, 0), then releases the BuddyApi (func_00261530), then
 * clears mState, miError, mbDisconnected, mbLoggedIn and mbRosterDone.
 */
extern "C" void func_00261408(BuddyMan* self)
{
    func_00262768(self, 0, 1, 0, 0, 0);
    func_00261530(self);
    self->mState = 0;
    self->miError = 0;
    self->mbDisconnected = 0;
    self->mbLoggedIn = 0;
    self->mbRosterDone = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261460);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" long f_261460(void* p) __asm__("func_00261460");
/**
 * Tells whether the buddy manager is connected and logged in.
 *
 * @param buddyMan  the buddy manager
 * @return 1 when mpBuddyApi is set, mState is not 1 (connecting) and mbLoggedIn is set, otherwise 0
 */
extern "C" long f_261460(void* buddyMan)
{
    char* buddyBytes = (char*)buddyMan;
    return *(int*)(buddyBytes + 0xC8) != 0 && (*(int*)(buddyBytes + 0xD8) ^ 1) != 0 && *(int*)(buddyBytes + 0xE0) != 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261490);
#ifdef SKIP_ASM
extern "C" void func_003F17D8(void* p);
extern "C" void func_00261408(BuddyMan* self);

/**
 * Polls the BuddyApi and handles a lost connection; LobbyMan's update func_0025AC50 calls it every frame.
 *
 * With a BuddyApi it calls func_003F17D8(mpBuddyApi); when mbDisconnected was set (by BuddyMan_onconnect) it then
 * disconnects (func_00261408) and clears mbDisconnected.
 */
extern "C" void func_00261490(BuddyMan* self)
{
    void* buddyApi = self->mpBuddyApi;
    if (buddyApi != 0) {
        func_003F17D8(buddyApi);
        if (self->mbDisconnected != 0) {
            func_00261408(self);
            self->mbDisconnected = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002614E0);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty function with no callers.
 */
extern "C" void func_002614E0(void) {

}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002614E8);
#ifdef SKIP_ASM
extern char str_2003_10_20[];
extern "C" void func_00268F40(void* self);
extern "C" void* func_003F0710(void* a, const char* name);
extern "C" void func_003F17C8(void* h, void (*cb)(void*), void* user);

/**
 * Creates the buddy manager's BuddyApi instance and registers its incoming-message callback.
 *
 * Stores func_003F0710(@p resourceName, "2003-10-20") in mpBuddyApi and registers func_00268F40 (which runs BuddyMan_onReceiveMessage) with
 * func_003F17C8(mpBuddyApi, .., self).
 *
 * @param resourceName    the resource name passed on from func_00261248
 */
extern "C" void func_002614E8(BuddyMan* self, void* resourceName)
{
    void* buddyApi = func_003F0710(resourceName, str_2003_10_20);
    self->mpBuddyApi = (BuddyApiRefT*)buddyApi;
    func_003F17C8(buddyApi, func_00268F40, self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261530);
#ifdef SKIP_ASM
// PORT: callee defined as returning void* in this unit; called here as void.
void func_0025FC28_v(void* self, int a1) __asm__("func_0025FC28__FPvi");
extern "C" void func_003F0BE0(void* p);
extern "C" void func_003F07A0(void* p);

struct sVE261530 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj261530 {
    int id;
    char pad[0x38];
    sVE261530* vt;
};


/**
 * Releases the BuddyApi: declines and deletes every pending challenge, then destroys the API.
 *
 * Only with a BuddyApi. Pops each cChallenger from the back of mChallengers, sends "decline" to its first word (the
 * challenger's name) with func_0025FC28(LobbyMan_mLobbyMan, ..) and destroys it through its virtual destructor (vtable
 * at +0x3C, slot 1, delete flags 3). Then calls func_003F0BE0 and func_003F07A0 on mpBuddyApi and clears mpBuddyApi and
 * mpRoster.
 *
 * @param vself  the buddy manager
 */
extern "C" void func_00261530(BuddyMan* vself)
{
    BuddyMan* self = vself;
    if (((void*)self->mpBuddyApi) != 0) {
        while (((sObj261530**)self->mChallengersStart) != ((sObj261530**)self->mChallengersFinish)) {
            sObj261530* challenger = ((sObj261530**)self->mChallengersFinish)[-1];
            func_0025FC28_v(LobbyMan_mLobbyMan, challenger->id);
            (*(sObj261530***)&self->mChallengersFinish)--;
            if (challenger != 0) {
                challenger->vt[1].fn((char*)challenger + challenger->vt[1].delta, 3);
            }
        }
        func_003F0BE0(((void*)self->mpBuddyApi));
        func_003F07A0(((void*)self->mpBuddyApi));
        (*(void**)&self->mpBuddyApi) = 0;
        (*(int*)&self->mpRoster) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", BuddyMan_onconnect);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern "C" void* func_003F0C90(void* h, void* cb, void* user);
extern "C" void func_003EE5A0(void* h, int a, int b, void* cb);
extern "C" void func_00262768(BuddyMan* self, int a1, int a2, int a3, int a4, int a5);
extern "C" void func_00268F68(void);
extern "C" int func_002634C0(void* self, int a1, void* a, void* b);

struct sMsgListNode15C8 {
    sMsgListNode15C8* next;
    sMsgListNode15C8* prev;
    void* data;
};

struct sMsgListIter15C8 {
    sMsgListNode15C8* node;
    sMsgListIter15C8(sMsgListNode15C8* x) : node(x) {}
    sMsgListIter15C8(const sMsgListIter15C8& x) : node(x.node) {}
};

static inline sMsgListIter15C8 sMsgList15C8_insert(sMsgListIter15C8 pos, void* const& x)
{
    sMsgListNode15C8* tmp = (sMsgListNode15C8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_15C8(char* owner, void* msg)
{
    void* mp = msg;
    sMsgList15C8_insert(*(sMsgListNode15C8**)(owner + 0xF0), mp);
}

/**
 * Handles the BuddyApi's connection and login messages.
 *
 * The callback thunk func_00268F18 (registered by func_00261248) calls it. 'conn' with status 2, 3, 4 or 6 marks a lost
 * connection: sets mbDisconnected and miError (1 when it was 0); status 6 also clears mbRosterDone. 'auth' with no
 * error (@p msg[2] == 0) creates the roster (mpRoster = func_003F0C90(mpBuddyApi, func_00268F68, self), whose callback
 * runs BuddyMan_onrosterupdate), installs func_002634C0 on it with func_003EE5A0, publishes the presence (func_00262768(self, 1,
 * 0, 0, 0, 0)) and sets mState to 0 and mbLoggedIn to 1. A failed 'auth' posts an online::Event of type 260 on
 * LobbyMan_mLobbyMan's mEventList and stores the error in miError, setting mbDisconnected.
 *
 * @param msg   the BuddyApi message: kind at [0] ('conn' / 'auth'), status at [1], error at [2]
 */
extern "C" void BuddyMan_onconnect(BuddyMan* self, int* msg)
{
    switch (msg[0]) {
    case 0x636F6E6E:
        {
            int status = msg[1];
            if (status == 2) {
                self->mbDisconnected = 1;
                if (self->miError == 0)
                    self->miError = 1;
            } else if (status == 6) {
                self->mbRosterDone = 0;
                self->mbDisconnected = 1;
                if (self->miError == 0)
                    self->miError = 1;
            } else if (status == 4) {
                self->mbDisconnected = 1;
                if (self->miError == 0)
                    self->miError = 1;
            } else if (status == 3) {
                self->mbDisconnected = 1;
                if (self->miError == 0)
                    self->miError = 1;
            }
        }
        break;
    case 0x61757468:
        if (msg[2] == 0) {
            // PORT: function pointers passed as callbacks.
            self->mpRoster = func_003F0C90(self->mpBuddyApi, (void*)func_00268F68, ((char*)self));
            func_003EE5A0(self->mpRoster, 0, 0, (void*)func_002634C0);
            func_00262768(self, 1, 0, 0, 0, 0);
            self->mState = 0;
            self->mbLoggedIn = 1;
        } else {
            LobbyMan* owner = LobbyMan_mLobbyMan;
            int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            *(void***)((char*)event + 0x4) = online_Event_vtbl;
            event[0] = 260;
            post_15C8((char*)owner, event);
            int error = msg[2];
            self->mbDisconnected = 1;
            self->miError = error;
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", BuddyMan_onrosterupdate);
#ifdef SKIP_ASM
#include "fakematch.h"
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern "C" void func_002610D0();
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNode1770 {
    sMsgListNode1770* next;
    sMsgListNode1770* prev;
    void* data;
};

struct sMsgListIter1770 {
    sMsgListNode1770* node;
    sMsgListIter1770(sMsgListNode1770* x) : node(x) {}
    sMsgListIter1770(const sMsgListIter1770& x) : node(x.node) {}
};

static inline sMsgListIter1770 sMsgList1770_insert(sMsgListIter1770 pos, void* const& x)
{
    sMsgListNode1770* tmp = (sMsgListNode1770*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_1770(char* self, void* msg)
{
    void* mp = msg;
    sMsgList1770_insert(*(sMsgListNode1770**)(self + 0xF0), mp);
}


/**
 * Handles a BuddyApi roster update.
 *
 * The callback thunk func_00268F68 (registered on the roster by BuddyMan_onconnect) calls it. The first 'endr' message
 * (while mbRosterDone is 0) adds unknown message senders to the roster (func_002610D0), sets mbRosterDone and posts an
 * online::Event of type 263 on LobbyMan_mLobbyMan's mEventList; any other message posts type 262.
 *
 * @param msg   the roster message; kind at [0]
 */
// FAKEMATCH: register pin `register int type FM_REG("$17")` (local-alloc ties type and g at equal priority and
// the lower quantity, type, takes s0; retail gives s0 to g); plain form 99.43%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" void BuddyMan_onrosterupdate(BuddyMan* self, int* msg)
{
    if (msg[0] == 0x656E6472 && self->mbRosterDone == 0) {
        func_002610D0();
        register int type FM_REG("$17") = 263;
        (self)->mbRosterDone = 1;
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        event[0] = type;
        post_1770((char*)lobbyMan, event);
    } else {
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        int* event = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        *(void***)((char*)event + 0x4) = online_Event_vtbl;
        event[0] = 262;
        post_1770((char*)lobbyMan, event);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", BuddyMan_onBuddyAdd);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_002618C0 {
    sMsgListNode_002618C0* next;
    sMsgListNode_002618C0* prev;
    void* data;
};

struct sMsgListIter_002618C0 {
    sMsgListNode_002618C0* node;
    sMsgListIter_002618C0(sMsgListNode_002618C0* x) : node(x) {}
    sMsgListIter_002618C0(const sMsgListIter_002618C0& x) : node(x.node) {}
};

static inline sMsgListIter_002618C0 sMsgList_002618C0_insert(sMsgListIter_002618C0 pos, void* const& x)
{
    sMsgListNode_002618C0* tmp = (sMsgListNode_002618C0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Posts an online::Event of type 266 on LobbyMan_mLobbyMan's mEventList when a roster add request answers.
 *
 * The callback thunk func_00268FB8 calls it; func_002620D8 registers it with the add call func_003F0E38. The reply
 * itself is not read.
 *
 * @param buddyMan  not read
 */
extern "C" void BuddyMan_onBuddyAdd(int buddyMan)
{
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* event = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    *(void***)((char*)event + 0x4) = online_Event_vtbl;
    event[0] = 266;
    void* mp = event;
    sMsgList_002618C0_insert((*(sMsgListNode_002618C0**)((char*)lobbyMan + 0xF0)), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", BuddyMan_onBuddyDelete);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_00261970 {
    sMsgListNode_00261970* next;
    sMsgListNode_00261970* prev;
    void* data;
};

struct sMsgListIter_00261970 {
    sMsgListNode_00261970* node;
    sMsgListIter_00261970(sMsgListNode_00261970* x) : node(x) {}
    sMsgListIter_00261970(const sMsgListIter_00261970& x) : node(x.node) {}
};

static inline sMsgListIter_00261970 sMsgList_00261970_insert(sMsgListIter_00261970 pos, void* const& x)
{
    sMsgListNode_00261970* tmp = (sMsgListNode_00261970*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00261970 {
    int kind;
    void** vt;
};

/**
 * Posts an online::Event of type 267 on LobbyMan_mLobbyMan's mEventList when a roster delete request answers.
 *
 * The callback thunk func_00268F90 calls it; func_00261EF8 and two other BuddyMan functions register it with the delete
 * call func_003F11F8. The reply itself is not read.
 *
 * @param buddyMan  not read
 */
extern "C" void BuddyMan_onBuddyDelete(int buddyMan)
{
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    sMsg_00261970* event = (sMsg_00261970*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    event->vt = online_Event_vtbl;
    event->kind = 267;
    void* mp = event;
    sMsgList_00261970_insert((*(sMsgListNode_00261970**)((char*)lobbyMan + 0xF0)), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", BuddyMan_onBuddySend__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the reply handler for sent buddy messages (run by the thunk func_00268FE0) has an empty body.
 */
void BuddyMan_onBuddySend(void* self)
{
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", BuddyMan_onReceiveMessage);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: TagFieldGetUnk is called here with a 4th (default) argument left in $a3.
extern "C" void cDirtysock_tag_TagFieldGetUnk4(const char* tag, char* dst, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetUnk");
extern "C" int func_002623B0(BuddyMan* self, int name);
extern "C" void func_00261CD0(BuddyMan* self, char* name, char* text, char* date);
extern "C" int func_00262A08(BuddyMan* self, int name);
extern "C" void func_002635A8(BuddyMan* self, int i);
extern "C" int func_003EDD70(const char* data, int* year, int* month, int* day, int* hour, int* min, int* sec, int offset);
extern "C" char* strcat(char* d, const char* s);
extern char str_TYPE[];
extern char str_USER[];
extern char str_SUBJ[];
extern char str_BODY[];
extern char str_TIME[];
extern char str_Not_Present[];
extern char str_pct_02d_pct_02d_pct_02d_pct_02d_pct_02d[];
extern char D_004A2ED0[];

struct sMsgListNode_00261A28 {
    sMsgListNode_00261A28* next;
    sMsgListNode_00261A28* prev;
    void* data;
};

struct sMsgListIter_00261A28 {
    sMsgListNode_00261A28* node;
    sMsgListIter_00261A28(sMsgListNode_00261A28* x) : node(x) {}
    sMsgListIter_00261A28(const sMsgListIter_00261A28& x) : node(x.node) {}
};

static inline sMsgListIter_00261A28 sMsgList_00261A28_insert(sMsgListIter_00261A28 pos, void* x)
{
    sMsgListNode_00261A28* tmp = (sMsgListNode_00261A28*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void sTagVec_00261A28_remove(char* self, char* name)
{
    for (unsigned int i = 0; i < (unsigned int)((*(int*)(self + 0xB0) - *(int*)(self + 0xAC)) >> 2); i++) {
        void* e = func_00263630((BuddyMan*)self, i);
        if (e != 0 && func_004165A8(*(void**)e, name) == 0) {
            func_002635A8((BuddyMan*)self, i);
            return;
        }
    }
}

struct sMsg_00261A28 {
    int kind;
    void** vt;
    sMsg_00261A28(int k) : kind(k), vt(online_Event_vtbl) {}
};

/**
 * Handles an incoming buddy private message: stores it and tells the front end.
 *
 * The callback thunk func_00268F40 (registered by func_002614E8) calls it. Reads TYPE (unused), USER, SUBJ and BODY
 * (BODY with cDirtysock_tag_TagFieldGetUnk, which drops a leading '`') from the record at @p msg + 0xC and drops the
 * message when func_002623B0() reports the sender's roster entry has flag 0x200. Otherwise the text is SUBJ + BODY, the
 * date is TIME formatted by func_003EDD70 as "%02d/%02d/%02d, %02d:%02d GMT" (month, day, year, hour, minute), and
 * func_00261CD0 stores the message. When func_00262A08() then counts 31 or more stored messages from that sender, the
 * oldest one from them is erased (func_002635A8). Finally posts an online::Event of type 265 on LobbyMan_mLobbyMan's
 * mEventList. Likely bug: SUBJ may fill up to 2254 of the 2256-byte text buffer and BODY (up to 2047 characters) is
 * strcat'ed onto it with no bound.
 *
 * @param msg   the BuddyApi message; its tag record pointer is at +0xC
 */
extern "C" void BuddyMan_onReceiveMessage(BuddyMan* self, char* msg)
{
    char raw[0x800];
    char text[2256];
    char name[0x40];
    char date[0x40];
    char flag[0x10];
    int year, month, day, hour, minute, second;

    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_TYPE), flag, 2, str_Not_Present);
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_USER), name, 0x40, str_Not_Present);
    if (func_002623B0(self, (int)name))
        return;
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_SUBJ), text, 2255, D_004A2ED0);
    cDirtysock_tag_TagFieldGetUnk4(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_BODY), raw, 0x800, str_Not_Present);
    strcat(text, raw);
    func_003EDD70(cDirtysock_tag_TagFieldFind(*(char**)(msg + 0xC), str_TIME), &year, &month, &day, &hour, &minute, &second, 0);
    sprintf(date, str_pct_02d_pct_02d_pct_02d_pct_02d_pct_02d, month, day, year, hour, minute);
    func_00261CD0(self, name, text, date);
    if (func_00262A08(self, (int)name) >= 0x1F) {
        sTagVec_00261A28_remove(((char*)self), name);
    }
    int kind = 265;
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    sMsg_00261A28* event = new (str_NetEvent, 0x20000000, 0) sMsg_00261A28(kind);
    sMsgList_00261A28_insert((*(sMsgListNode_00261A28**)((char*)lobbyMan + 0xF0)), event);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261CD0);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern char str_cPrivateMessage_b[];
extern void* cPrivateMessage_vtbl[];
extern "C" int func_00262360(BuddyMan* self, int a1);
extern "C" int func_002623B0(BuddyMan* self, int a1);
extern "C" int func_00262438(BuddyMan* self, int a1);
extern "C" int func_00262400(BuddyMan* self, int a1);
extern "C" int func_00262598(BuddyMan* self);
extern "C" void func_00262EC0(BuddyMan* self, int idx);
extern "C" int func_002620D8(BuddyMan* self, char* name, int kind, int* err, char* other);
struct sPtrVec7128;
extern "C" void func_00267128(sPtrVec7128* self, void** pos, void* const& x);

struct sPtrVec1CD0 {
    void* alloc;
    void** start;
    void** finish;
    void** eos;
};


static inline void sPtrVec1CD0_construct(void** p, void* const& v)
{
    if (p != 0)
        *p = v;
}

static inline void sOwner1CD0_push_back(BuddyMan* buddyMan, void* const& x)
{
    sPtrVec1CD0* v = &(*(sPtrVec1CD0*)&buddyMan->mMessagesAlloc);
    if ((*(sPtrVec1CD0*)&buddyMan->mMessagesAlloc).finish != (*(sPtrVec1CD0*)&buddyMan->mMessagesAlloc).eos) {
        sPtrVec1CD0_construct(v->finish, x);
        ++(*(sPtrVec1CD0*)&buddyMan->mMessagesAlloc).finish;
    } else {
        func_00267128((sPtrVec7128*)v, (*(sPtrVec1CD0*)&buddyMan->mMessagesAlloc).finish, x);
    }
}

struct sMsgListNode1CD0 {
    sMsgListNode1CD0* next;
    sMsgListNode1CD0* prev;
    void* data;
};

struct sMsgListIter1CD0 {
    sMsgListNode1CD0* node;
    sMsgListIter1CD0(sMsgListNode1CD0* x) : node(x) {}
    sMsgListIter1CD0(const sMsgListIter1CD0& x) : node(x.node) {}
};

static inline sMsgListIter1CD0 sMsgList1CD0_insert(sMsgListIter1CD0 pos, void* const& x)
{
    sMsgListNode1CD0* tmp = (sMsgListNode1CD0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_1CD0(char* owner, void* msg)
{
    void* mp = msg;
    sMsgList1CD0_insert(*(sMsgListNode1CD0**)(owner + 0xF0), mp);
}

/**
 * Stores a received private message as a new cPrivateMessage, first adding an unknown sender to the roster.
 *
 * When the sender's roster entry has none of the flags 4, 0x200 and 0x100000 (func_00262360, func_002623B0,
 * func_00262438), it adds them with flag 0x100000 (func_002620D8 kind 2); if that fails because 10 such entries already
 * exist (error 2), it removes the first one (func_00262400 / func_00262EC0), retries and posts an online::Event of type
 * 264 on LobbyMan_mLobbyMan's mEventList. It then allocates a 0x10-byte cPrivateMessage (vtable cPrivateMessage_vtbl at +0xC;
 * cBXStrings name, text and date at +0x0/+0x4/+0x8) and appends it to mMessages (func_00267128 when the vector is
 * full).
 *
 * @param name  the sender's name
 * @param text    the message text
 * @param date    the formatted date
 */
// PORT: name pointers passed through int parameters of some callees.
extern "C" void func_00261CD0(BuddyMan* self, char* name, char* text, char* date)
{
    if (func_00262360(self, (int)name) == 0
        && func_002623B0(self, (int)name) == 0
        && func_00262438(self, (int)name) == 0) {
        int err;
        func_002620D8(self, name, 2, &err, 0);
        if (err == 2) {
            for (int i = 0; i < func_00262598(self); i++) {
                if (func_00262400(self, i)) {
                    func_00262EC0(self, i);
                    break;
                }
            }
            func_002620D8(self, name, 2, &err, 0);
            int type = 264;
            LobbyMan* owner = LobbyMan_mLobbyMan;
            int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            netEvent[0] = type;
            *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
            post_1CD0((char*)owner, netEvent);
        }
    }
    char* obj = (char*)cMemMan_alloc(0x10, str_cPrivateMessage_b, 0x20000000, 0);
    *(void***)(obj + 0xC) = cPrivateMessage_vtbl;
    cBXString_cBXString2(obj, name);
    cBXString_cBXString2(obj + 4, text);
    cBXString_cBXString2(obj + 8, date);
    void* message = obj;
    sOwner1CD0_push_back(self, message);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261EF8);
#ifdef SKIP_ASM
extern "C" void* func_003F14D8(void* h);
extern "C" int func_002620D8(BuddyMan* self, char* name, int kind, int* err, char* other);
extern "C" void func_00263018(BuddyMan* self, void* name);
extern "C" int func_00263128(BuddyMan* self, char* name);
extern "C" void func_003F11F8(void* h, void* e, void* cb, void* user, int timeout);
extern "C" void func_00268F90(void* self, void* a1, int a2);

/**
 * Moves a player into one of the buddy roster's lists, replacing the old roster entry.
 *
 * Looks the player up with func_003F14D8 (the name rides in the second argument register; the unit passes only
 * mpBuddyApi). Not on the roster: adds them with func_002620D8(.., @p kind, ..). On the roster: removes the entry with
 * func_003F11F8 (callback func_00268F90, timeout 600), then re-adds it by its old flags (+0xA8): kind 0 (flag 4)
 * re-adds unless the entry already had flag 4, and on failure puts a flag-0x100000 entry back as kind 2; kind 1 (flag
 * 0x200) re-adds as kind 1, or as kind 2 when the entry already had 0x200; kind 2 (flag 0x100000) re-adds unless it
 * already had that flag. So asking for kind 0 or 2 when the entry already has that flag only removes it. A pending
 * challenge from the player (func_00263128) is dropped (func_00263018) on success, except when kind 0 replaces a
 * flag-0x100000 entry, and on a kind-1 failure. Quirk: the not-on-roster path ends with a no-op "if (*err == 0) *err =
 * 0".
 *
 * @param self_    the buddy manager
 * @param rawName  the player's name (a string pointer passed as an int)
 * @param kind     target list: 0 (flag 4), 1 (flag 0x200), 2 (flag 0x100000)
 * @param out      receives the error code: 0 on success, else func_002620D8()'s (1 list full, 2 flag-0x100000 entries
 *                 full, 3 own persona)
 */
// PORT: the unit declares (void*, int, int, void*); a1 carries the name pointer.
extern "C" void func_00261EF8(BuddyMan* self_, int rawName, int kind, void* out)
{
    char* name = (char*)rawName;
    int* err = (int*)out;
    char* rosterEntry = (char*)func_003F14D8(self_->mpBuddyApi);
    if (rosterEntry == 0) {
        func_002620D8(self_, name, kind, err, 0);
        if (*err == 0)
            *err = 0;
        return;
    }
    int flags = *(int*)(rosterEntry + 0xA8);
    // PORT: function pointer passed as a callback argument.
    func_003F11F8(self_->mpBuddyApi, rosterEntry, (void*)func_00268F90, ((char*)self_), 600);
    int has = func_00263128(self_, name);
    switch (kind) {
    case 0:
        if (!(flags & 4)) {
            func_002620D8(self_, name, 0, err, rosterEntry);
            if (*err != 0) {
                if (flags & 0x100000) {
                    int discardErr;
                    func_002620D8(self_, name, 2, &discardErr, rosterEntry);
                }
                return;
            }
            if (flags & 0x100000)
                has = 0;
        }
        break;
    case 1:
        if (flags & 0x200)
            goto do2;
        func_002620D8(self_, name, 1, err, rosterEntry);
        if (*err != 0) {
            if (has)
                func_00263018(self_, name);
            return;
        }
        break;
    case 2:
        if (!(flags & 0x100000)) {
        do2:
            func_002620D8(self_, name, 2, err, rosterEntry);
            if (*err != 0)
                return;
        }
        break;
    }
    if (has)
        func_00263018(self_, name);
    *err = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002620D8);
#ifdef SKIP_ASM
extern "C" int func_00262280(BuddyMan* self);
extern "C" int func_002622F0(BuddyMan* self);
extern "C" int func_003F0E38(void* h, void* rec, void* cb, void* user, int timeout);
extern "C" void func_00268FB8(void* self, void* a1, int a2);

struct sTagsUnkReq20D8 {
    char name[0x20];   // 0x00
    char x20[0x20];    // 0x20
    char x40[0x68];    // 0x40
    int flags;         // 0xA8
    char pad[0x14];
};

/**
 * Sends a BuddyApi request that adds a player to the roster with the flags of a list kind.
 *
 * Refuses the local persona (LobbyMan_mLobbyMan->mPersona, compared case-insensitively) with error 3. Kinds 0 and 1
 * fail with error 1 when func_00262280 already counts 40 entries without flag 0x100000 (39 when the player has no entry
 * yet or has that flag); kind 2 fails with error 2 at 10 entries with it (func_002622F0). The request record holds the
 * name, mResource and flags 4 (kind 0), 0x200 (kind 1) or 0x100000 | 0x800000 (kind 2), and goes out with
 * func_003F0E38(mpBuddyApi, .., func_00268FB8, self, 600). Likely bug: the record's flags word (and its tail) is never
 * initialized, so the kind's bits are ORed into stack garbage.
 *
 * @param name   the player's name
 * @param kind   0, 1 or 2 (see above); any other value sends the request with no kind flag
 * @param err    receives 0 when the request was sent, or the error code
 * @param other  the player's current roster entry (flags at +0xA8), or 0 when not on the roster
 * @return func_003F0E38()'s result, or 0 when refused
 */
extern "C" int func_002620D8(BuddyMan* self, char* name, int kind, int* err, char* other)
{
    sTagsUnkReq20D8 rec;
    if (strcasecmp(LobbyMan_mLobbyMan->mPersona, name) == 0) {
        *err = 3;
        return 0;
    }
    int extraUnflagged = 0;
    int extraFlagged = 0;
    if (other == 0) {
        extraUnflagged = 1;
        extraFlagged = 1;
    } else if (*(int*)(other + 0xA8) & 0x100000) {
        extraUnflagged = 1;
    } else {
        extraFlagged = 1;
    }
    strcpy(rec.name, name);
    strcpy(rec.x40, self->mResource);
    rec.x20[0] = 0;
    switch (kind) {
    case 0:
        if (func_00262280(self) + extraUnflagged >= 41) {
            *err = 1;
            return 0;
        }
        rec.flags |= 4;
        break;
    case 1:
        if (func_00262280(self) + extraUnflagged >= 41) {
            *err = 1;
            return 0;
        }
        rec.flags |= 0x200;
        break;
    case 2:
        if (func_002622F0(self) + extraFlagged >= 11) {
            *err = 2;
            return 0;
        }
        rec.flags |= 0x100000;
        rec.flags |= 0x800000;
        break;
    }
    *err = 0;
    // PORT: function pointer passed as a callback argument.
    return func_003F0E38(self->mpBuddyApi, &rec, (void*)func_00268FB8, ((char*)self), 600);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262280);
#ifdef SKIP_ASM
extern "C" int func_00262400(BuddyMan* self, int a1);
extern "C" int func_00262598(BuddyMan* self);

/**
 * Counts the buddy roster entries without flag 0x100000 (func_00262400 is 0 for them).
 *
 * @return the number of such entries among func_00262598() roster entries
 */
extern "C" int func_00262280(BuddyMan* self)
{
    int n = 0;
    for (int i = 0; i < func_00262598(self); i++) {
        if (func_00262400(self, i) == 0)
            n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002622F0);
#ifdef SKIP_ASM
extern "C" int func_00262400(BuddyMan* self, int a1);
extern "C" int func_00262598(BuddyMan* self);

/**
 * Counts the buddy roster entries with flag 0x100000 (func_00262400 is nonzero for them).
 *
 * @return the number of such entries among func_00262598() roster entries
 */
extern "C" int func_002622F0(BuddyMan* self)
{
    int n = 0;
    for (int i = 0; i < func_00262598(self); i++) {
        if (func_00262400(self, i) != 0)
            n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262360);
#ifdef SKIP_ASM
extern "C" int func_00262468(BuddyMan* self, int a1);
// PORT: the unit declares func_00262390 as `int (void*)`; this caller passes a
// second argument (it reaches func_00262620's callee in $a1). Bound by asm label.
int func_00262390_2(void* self, int a1) __asm__("func_00262390__FPv");

/**
 * Tells whether a player's buddy roster entry has flag 4, by name.
 *
 * Finds the roster index with func_00262468() and passes it to func_00262390, which tests the entry's flag 4
 * (func_00262620; the unit declares func_00262390 with one argument, so the index rides in the second register).
 *
 * @param name  the player's name (a string pointer passed as an int)
 * @return nonzero when the player is on the roster with flag 4, otherwise 0
 */
extern "C" int func_00262360(BuddyMan* self, int name)
{
    return func_00262390_2(self, func_00262468(self, name));
}
#endif

extern "C" void* func_00262620(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262390__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether a buddy-roster entry has the 0x4 flag (the flag func_002620D8 sets for kind 0, a buddy-list entry).
 *
 * The roster index is not in the C declaration: it arrives in the second argument register and passes through to
 * func_00262620(), which returns the entry when bit 0x4 of its flags word (+0xA8) is set. func_00262360 is the by-name
 * wrapper.
 *
 * @return 1 when func_00262620() finds the entry with the flag, otherwise 0
 */
int func_00262390(void* self)
{
    return (func_00262620(self) != 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002623B0);
#ifdef SKIP_ASM
extern "C" int func_00262468(BuddyMan* self, int a1);
// PORT: the unit declares func_002623E0 as `int (void*)`; this caller passes a
// second argument (it reaches func_00262658's callee in $a1). Bound by asm label.
int func_002623E0_2(void* self, int a1) __asm__("func_002623E0__FPv");

/**
 * Tells whether the buddy-roster entry with a given name has the 0x200 flag (the "blocked" entry func_002620D8 adds for
 * kind 1).
 *
 * Looks the name up with func_00262468() and tests the index with func_002623E0(). The front end uses it to choose
 * between "Unblock User" and "Block User", and LobbyMan_onchatmsg answers a challenge from such a player with "ignore".
 *
 * @param name  the player name, passed as an int
 * @return 1 when the entry exists and has the flag, otherwise 0
 */
extern "C" int func_002623B0(BuddyMan* self, int name)
{
    return func_002623E0_2(self, func_00262468(self, name));
}
#endif

extern "C" void* func_00262658(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002623E0__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether a buddy-roster entry has the 0x200 flag (the "blocked" entry func_002620D8 adds for kind 1).
 *
 * The roster index is not in the C declaration: it arrives in the second argument register and passes through to
 * func_00262658(), which returns the entry when bit 0x200 of its flags word (+0xA8) is set.
 *
 * @return 1 when func_00262658() finds the entry with the flag, otherwise 0
 */
int func_002623E0(void* self)
{
    return (func_00262658(self) != 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262400);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(BuddyMan* self, int idx);

/**
 * Returns bit 20 (0x100000) of a buddy-roster entry's flags word (+0xA8).
 *
 * func_002620D8 sets that bit (with 0x800000) on kind-2 entries, at most 10 of them, which func_00262AF0 adds for a
 * challenger who is not on the roster yet; func_00262280 / func_002622F0 count the entries without / with it.
 *
 * @param rosterIndex  index into mpRoster
 * @return 1 when the entry exists and has the bit, otherwise 0
 */
extern "C" int func_00262400(BuddyMan* self, int rosterIndex)
{
    void* rosterEntry = func_002625C8(self, rosterIndex);
    if (rosterEntry != 0) {
        return (*(int*)((char*)rosterEntry + 0xA8) >> 20) & 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262438);
#ifdef SKIP_ASM
extern "C" int func_00262468(BuddyMan* self, int a1);
extern "C" int func_00262400(BuddyMan* self, int a1);

/**
 * Returns bit 20 (0x100000) of the flags of the buddy-roster entry with a given name.
 *
 * Looks the name up with func_00262468() and passes the index to func_00262400().
 *
 * @param name  the player name, passed as an int
 * @return 1 when the entry exists and has the bit, otherwise 0
 */
extern "C" int func_00262438(BuddyMan* self, int name)
{
    return func_00262400(self, func_00262468(self, name));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262468);
#ifdef SKIP_ASM
extern "C" int func_004165A8(void*, void*);
extern "C" void* func_002625C8(BuddyMan* self, int idx);

/**
 * Finds a buddy-roster entry by name and returns its index.
 *
 * Walks the func_00262598() entries of mpRoster and compares each entry's name (its first bytes) with func_004165A8
 * (strcmp, so case-sensitive).
 *
 * @param name  the player name, passed as an int
 * @return the entry's index, or -1 when no entry has that name
 */
// PORT: a1 carries a name pointer in an int (the unit's declaration).
extern "C" int func_00262468(BuddyMan* self, int name)
{
    int i;
    int n = func_00262598(self);
    for (i = 0; i < n; i++) {
        void* rosterEntry = func_002625C8(self, i);
        if (rosterEntry != 0 && func_004165A8(rosterEntry, (void*)name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002624F8);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(BuddyMan* self, int idx);

/**
 * Finds a buddy-roster entry by name, ignoring case, and returns its index.
 *
 * Like func_00262468(), but the name comes as a cBXString (a pointer to the text pointer) and is compared with
 * strcasecmp.
 *
 * @param key  the name as a cBXString, or 0
 * @return the entry's index, or -1 when @p key is 0 or no entry matches
 */
extern "C" int func_002624F8(BuddyMan* self, void** key)
{
    int i;
    int n;
    if (key == 0) {
        return -1;
    }
    n = func_00262598(self);
    for (i = 0; i < n; i++) {
        void* rosterEntry = func_002625C8(self, i);
        if (rosterEntry != 0 && strcasecmp(*key, rosterEntry) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262598);
#ifdef SKIP_ASM
extern "C" int func_003EE550(void*);

/**
 * Returns the number of entries in the buddy roster.
 *
 * Asks func_003EE550() for the size of mpRoster.
 *
 * @return the entry count, or 0 when there is no roster yet (mpRoster is 0)
 */
extern "C" int func_00262598(BuddyMan* self)
{
    void* roster = self->mpRoster;
    if (roster == 0) {
        return 0;
    }
    return func_003EE550(roster);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002625C8);
#ifdef SKIP_ASM
extern "C" int func_00262598(BuddyMan* self);
// PORT: the unit declares func_003EE560 with one parameter; this caller passes (list, index).
void* func_003EE560_2(void* list, int idx) __asm__("func_003EE560");

/**
 * Returns a buddy-roster entry by index.
 *
 * Gets it from mpRoster with func_003EE560(). Likely bug: the range check only rejects an index above the count
 * (func_00262598() < idx), so idx == count still reaches func_003EE560().
 *
 * @param idx  index into mpRoster
 * @return the entry (its name is at +0, its flags word at +0xA8), or 0 when @p idx is negative or too large
 */
extern "C" void* func_002625C8(BuddyMan* self, int idx)
{
    void* rosterEntry;
    if (idx < 0 || func_00262598(self) < idx) {
        rosterEntry = 0;
    } else {
        rosterEntry = func_003EE560_2(self->mpRoster, idx);
    }
    return rosterEntry;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262620);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(BuddyMan* self, int idx);
// PORT: the unit declares func_00262620 as `void* (void*)`; the body passes a second
// argument through to func_002625C8 in $a1. Bound by asm label.
void* func_00262620_2(void* self, int idx) __asm__("func_00262620");

/**
 * Returns a buddy-roster entry if it has the 0x4 flag (a buddy-list entry), otherwise 0.
 *
 * Fetches the entry with func_002625C8() and tests bit 0x4 of its flags word (+0xA8).
 *
 * @param idx  index into mpRoster
 * @return the entry, or 0 when it is missing or lacks the flag
 */
void* func_00262620_2(void* self, int idx)
{
    void* rosterEntry = func_002625C8((BuddyMan*)self, idx);
    if ((rosterEntry != 0) && (*(int*)((char*)rosterEntry + 0xA8) & 0x4)) {
        return rosterEntry;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262658);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(BuddyMan* self, int idx);
// PORT: the unit declares func_00262658 as `void* (void*)`; the body passes a second
// argument through to func_002625C8 in $a1. Bound by asm label.
void* func_00262658_2(void* self, int idx) __asm__("func_00262658");

/**
 * Returns a buddy-roster entry if it has the 0x200 flag (a blocked entry), otherwise 0.
 *
 * Fetches the entry with func_002625C8() and tests bit 0x200 of its flags word (+0xA8).
 *
 * @param idx  index into mpRoster
 * @return the entry, or 0 when it is missing or lacks the flag
 */
void* func_00262658_2(void* self, int idx)
{
    void* rosterEntry = func_002625C8((BuddyMan*)self, idx);
    if ((rosterEntry != 0) && (*(int*)((char*)rosterEntry + 0xA8) & 0x200)) {
        return rosterEntry;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262690);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern char str_TYPE[];
extern char str_USER[];
extern char str_SUBJ[];
extern char str_BODY[];
extern char D_004A3390[];
extern char str_SECS[];
extern "C" void func_00268FE0(void* self, void* a1, int a2);
extern "C" void func_003F16D8(void* ctx, char* buf, void* cb, void* user, int a4);

/**
 * Sends a private text message to another player through the buddy server.
 *
 * Builds a 0x1000-byte tag record: TYPE "C", SUBJ "" (D_004A2F40), USER @p name, BODY @p messageText (through
 * cDirtysock_tag_TagFieldSetUnk(), which escapes a leading quote) and SECS miMsgTimeout, then hands it to
 * func_003F16D8() on mpBuddyApi with the callback func_00268FE0, this BuddyMan as its user data and miMsgTimeout again.
 *
 * @param name  recipient's name
 * @param messageText    message text, passed as an int
 */
extern "C" void func_00262690(BuddyMan* self, const char* name, int messageText)
{
    char buf[0x1000];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 0x1000, str_TYPE, D_004A3390);
    cDirtysock_tag_TagFieldSetString(buf, 0x1000, str_SUBJ, D_004A2F40);
    cDirtysock_tag_TagFieldSetString(buf, 0x1000, str_USER, name);
    cDirtysock_tag_TagFieldSetUnk(buf, 0x1000, str_BODY, messageText);
    cDirtysock_tag_TagFieldSetNumber(buf, 0x1000, str_SECS, self->miMsgTimeout);
    func_003F16D8(self->mpBuddyApi, buf, (void*)func_00268FE0, ((char*)self), self->miMsgTimeout);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262768);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldSetNumber(char* buf, int size, const char* key, int value);
extern "C" int cDirtysock_tag_TagFieldSetString(char* buf, int size, const char* key, const char* value);
extern "C" int func_002628C0(BuddyMan* self, int sel);
extern "C" void func_003F1538(void* p);
extern "C" void func_003F1548(void* p, char* buf);
extern "C" void func_003F1578(void* p, const char* a, const char* b);
extern "C" void func_003F15C8(void* p, const char* name, int a2, int a3);
extern char D_004A31F8[];
extern char str_SSXID[];
extern char str_LOCID_b[];
extern char str_SZ1_b[];
extern char str_SZ2_b[];
extern char str_SZ3_b[];
extern char str_is_playing_SSX_3[];
extern char str_en[];

/**
 * Publishes this player's presence on the buddy server.
 *
 * Does nothing without mpBuddyApi. Builds an 80-byte tag record: S = mResource, SSXID = 3, LOCID = @p locationId, then SZ1, SZ2
 * and SZ3 from @p detail1..@p detail3 (each only when it and the ones before it are non-null), and passes it through
 * func_003F1538(), func_003F1548() (with the record) and func_003F1578() ("is playing SSX 3", "en"); finally
 * func_003F15C8() gets mResource, the state func_002628C0() maps @p presenceState to, and 2000. func_00261408 calls it with state
 * 0 and location 1 on disconnect, func_00263CF0 with state 1, location 9 and the opponent's name when an online race
 * starts.
 *
 * @param self_  the BuddyMan
 * @param presenceState     presence state, 0..3 (anything else becomes -1)
 * @param locationId     location id, sent as LOCID
 * @param detail1     first detail string (SZ1), passed as an int, or 0
 * @param detail2     second detail string (SZ2), passed as an int, or 0
 * @param detail3     third detail string (SZ3), passed as an int, or 0
 */
// PORT: the unit declares every argument as int; a3..a5 are really string pointers.
extern "C" void func_00262768(BuddyMan* self_, int presenceState, int locationId, int detail1, int detail2, int detail3)
{
    if (self_->mpBuddyApi == 0)
        return;
    char buf[80];
    buf[0] = 0;
    cDirtysock_tag_TagFieldSetString(buf, 80, D_004A31F8, (const char*)self_->mResource);
    cDirtysock_tag_TagFieldSetNumber(buf, 80, str_SSXID, 3);
    cDirtysock_tag_TagFieldSetNumber(buf, 80, str_LOCID_b, locationId);
    if (detail1 != 0)
    {
        cDirtysock_tag_TagFieldSetString(buf, 80, str_SZ1_b, (const char*)detail1);
        if (detail2 != 0)
        {
            cDirtysock_tag_TagFieldSetString(buf, 80, str_SZ2_b, (const char*)detail2);
            if (detail3 != 0)
                cDirtysock_tag_TagFieldSetString(buf, 80, str_SZ3_b, (const char*)detail3);
        }
    }
    func_003F1538(self_->mpBuddyApi);
    func_003F1548(self_->mpBuddyApi, buf);
    func_003F1578(self_->mpBuddyApi, str_is_playing_SSX_3, str_en);
    const char* name = (const char*)self_->mResource;
    func_003F15C8(self_->mpBuddyApi, name, func_002628C0(self_, presenceState), 2000);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002628C0);
#ifdef SKIP_ASM
/**
 * Maps a presence state for func_00262768(): 0..3 stay as they are, anything else gives -1.
 *
 * @param sel  the requested state
 * @return @p sel when it is 0..3, otherwise -1; also -1 when mpBuddyApi is 0
 */
extern "C" int func_002628C0(BuddyMan* self, int sel)
{
    if ((int)self->mpBuddyApi == 0) {
        return -1;
    }
    switch (sel) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262938);
#ifdef SKIP_ASM
/**
 * Returns the number of received private messages (the mMessagesStart..mMessagesFinish vector).
 *
 * @return the message count
 */
extern "C" int func_00262938(BuddyMan* self)
{
    return ((int)self->mMessagesFinish - (int)self->mMessagesStart) >> 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262950);
#ifdef SKIP_ASM
extern "C" void* func_00263630(BuddyMan* self, unsigned int i);
extern "C" int func_004165A8(void*, void*);

static inline unsigned int sDsListK2_size(void* self)
{
    return *(void***)((char*)self + 0xB0) - *(void***)((char*)self + 0xAC);
}

/**
 * Counts the received private messages sent by a buddy-roster entry.
 *
 * Takes the entry's name with func_002625C8() and compares it (func_004165A8, strcmp) with each message's first string
 * (the sender) from func_00263630().
 *
 * @param idx  index into mpRoster
 * @return the number of messages from that player, or -1 when the entry does not exist
 */
// Counts the entries whose name matches entry `idx`'s name.
extern "C" int func_00262950(BuddyMan* self, int idx)
{
    void* key = func_002625C8(self, idx);
    if (key == 0)
        return -1;
    int count = 0;
    unsigned int i;
    for (i = 0; i < sDsListK2_size(self); i++) {
        void** message = (void**)func_00263630(self, i);
        if (message != 0) {
            if (func_004165A8(*message, key) == 0)
                count++;
        }
    }
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00262A08);
#ifdef SKIP_ASM
extern "C" int func_00262468(BuddyMan* self, int a1);
extern "C" int func_00262950(BuddyMan* self, int a1);

/**
 * Counts the received private messages from the buddy-roster entry with a given name.
 *
 * Looks the name up with func_00262468() and passes the index to func_00262950(). The buddy-message reader
 * BuddyMan_onReceiveMessage calls it.
 *
 * @param name  the player name, passed as an int
 * @return the number of messages from that player, or -1 when the name is not on the roster
 */
extern "C" int func_00262A08(BuddyMan* self, int name)
{
    return func_00262950(self, func_00262468(self, name));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262A38);
#ifdef SKIP_ASM
extern "C" int func_004165A8(void*, void*);

/**
 * Tells whether any received private message is from a given player.
 *
 * Compares each message's first string (the sender) with @p name using func_004165A8 (strcmp).
 *
 * @param name  the player name
 * @return 1 when a message from that player is held, otherwise 0
 */
extern "C" int func_00262A38(BuddyMan* self, void* name)
{
    for (void*** it = (void***)self->mMessagesStart; it != (void***)self->mMessagesFinish; it++) {
        if (func_004165A8(**it, name) == 0)
            return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262AB0);
#ifdef SKIP_ASM
/**
 * Tells whether any private message has been received; meant as a by-name test, but the comparison result is ignored.
 *
 * Likely bug: it calls func_004165A8 (strcmp) on the first message's sender and *@p nameStr, discards the result and
 * returns 1 whenever the message vector is non-empty. No caller in the source.
 *
 * @param nameStr  the name as a cBXString (a pointer to the text pointer)
 * @return 1 when at least one message is held, otherwise 0
 */
extern "C" int func_00262AB0(BuddyMan* self, void** nameStr)
{
    void*** messageIter = (void***)self->mMessagesStart;
    if (messageIter != (void***)self->mMessagesFinish) {
        func_004165A8(**messageIter, *nameStr);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262AF0);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" int func_002620D8(BuddyMan* self, char* name, int kind, int* err, char* other);
int func_002623E0_2(void* self, int a1) __asm__("func_002623E0__FPv");
extern "C" int func_00262400(BuddyMan* self, int a1);
extern "C" int func_00262468(BuddyMan* self, int a1);
extern "C" int func_00262598(BuddyMan* self);
extern "C" void func_00262EC0(BuddyMan* self, int idx);
extern "C" void func_00263018(BuddyMan* self, void* name);
extern "C" int func_00263128(BuddyMan* self, char* name);
extern "C" void* strcasecmp(void*, void*);
struct sPtrVec72C8;
extern "C" void func_002672C8(sPtrVec72C8* self, void** pos, void* const& x);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern char str_cChallenger[];
extern char cChallenger_vtbl[];
extern void* online_Event_vtbl[];

struct sMsgListNode_262AF0 {
    sMsgListNode_262AF0* next;
    sMsgListNode_262AF0* prev;
    void* data;
};

struct sMsgListIter_262AF0 {
    sMsgListNode_262AF0* node;
    sMsgListIter_262AF0(sMsgListNode_262AF0* x) : node(x) {}
    sMsgListIter_262AF0(const sMsgListIter_262AF0& x) : node(x.node) {}
};

static inline sMsgListIter_262AF0 sMsgList_262AF0_insert(sMsgListIter_262AF0 pos, void* const& x)
{
    sMsgListNode_262AF0* tmp = (sMsgListNode_262AF0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_262AF0(char* owner, void* msg)
{
    void* mp = msg;
    sMsgList_262AF0_insert(*(sMsgListNode_262AF0**)(owner + 0xF0), mp);
}

struct sObjVec_262AF0 {
    void* alloc;
    void** start;
    void** finish;
    void** eos;
};

static inline void construct_262AF0(void** p, void* const& x)
{
    if (p != 0)
        *p = x;
}

static inline void push_back_262AF0(char* o, sObjVec_262AF0* v, void* const& x)
{
    if (*(void***)(o + 0xC0) != *(void***)(o + 0xC4))
    {
        construct_262AF0(v->finish, x);
        ++*(void***)(o + 0xC0);
    }
    else
    {
        func_002672C8((sPtrVec72C8*)v, *(void***)(o + 0xC0), x);
    }
}

/**
 * Records a challenge received from another player as a new cChallenger and tells the front end.
 *
 * Ignores challenges from this player's own persona (LobbyMan mPersona). Removes older challenges from the same name
 * first (func_00263128() / func_00263018(), which also sends each of them "decline"). A name that is not on the roster
 * is added as a kind-2 entry with func_002620D8(); when those are full (error 2) it removes the first entry with bit 20
 * (func_00262EC0()), retries, and posts an online::Event of type 264 to LobbyMan's mEventList. A name on the roster
 * with the blocked flag (func_002623E0()) is ignored. Otherwise it allocates a 0x40-byte object (tag "cChallenger",
 * vtable cChallenger_vtbl at +0x3C) holding @p name and @p messageText as cBXStrings and the numbers below, appends it to
 * mChallengersStart..mChallengersFinish (func_002672C8 when full) and posts an event of type 236. The only caller,
 * LobbyMan_onchatmsg, passes the fields of a "challenge" private message.
 *
 * @param name  challenger's name; stored at +0x0
 * @param locationId    first number of the message; stored at +0x8
 * @param gameMode    second number; stored at +0xC
 * @param gameEvent    third number; stored at +0x10
 * @param ranked    fourth number, as 0/1; stored at +0x14
 * @param multipliers    fifth number, as 0/1; stored at +0x18
 * @param powerUps    sixth number, as 0/1; stored at +0x1C
 * @param numberAI    seventh number; stored at +0x20
 * @param pointIcons    eighth number, as 0/1; stored at +0x24
 * @param messageText    the whole message text; stored as a cBXString at +0x4
 * @param statField2    first number after the signature; stored at +0x2C
 * @param statTotal    second number after it; stored at +0x30
 * @param statField6    third number after it; stored at +0x34
 * @param rank    fourth number after it; stored at +0x28
 */
extern "C" void func_00262AF0(BuddyMan* self, char* name, int locationId, int gameMode, int gameEvent, int ranked, int multipliers, int powerUps,
                              int numberAI, int pointIcons, char* messageText, int statField2, int statTotal, int statField6, int rank)
{
    if (strcasecmp(LobbyMan_mLobbyMan->mPersona, name) == 0)
        return;
    while (func_00263128(self, name))
        func_00263018(self, name);
    int idx = func_00262468(self, (int)name);
    if (idx < 0)
    {
        int err;
        func_002620D8(self, name, 2, &err, 0);
        if (err == 2)
        {
            int i;
            for (i = 0; i < func_00262598(self); i++)
            {
                if (func_00262400(self, i))
                {
                    func_00262EC0(self, i);
                    break;
                }
            }
            int type = 264;
            func_002620D8(self, name, 2, &err, 0);
            LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
            int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            netEvent[0] = type;
            *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
            post_262AF0((char*)lobbyMan, netEvent);
        }
    }
    else if (func_002623E0_2(((char*)self), idx))
    {
        return;
    }
    char* obj = (char*)operator new(0x40, str_cChallenger, 0x20000000, 0);
    *(char**)(obj + 0x3C) = cChallenger_vtbl;
    cBXString_cBXString2(obj, name);
    cBXString_cBXString2(obj + 4, messageText);
    *(int*)(obj + 0x8) = locationId;
    *(int*)(obj + 0xC) = gameMode;
    *(int*)(obj + 0x10) = gameEvent;
    *(int*)(obj + 0x14) = ranked;
    *(int*)(obj + 0x18) = multipliers;
    *(int*)(obj + 0x1C) = powerUps;
    *(int*)(obj + 0x20) = numberAI;
    *(int*)(obj + 0x24) = pointIcons;
    *(int*)(obj + 0x2C) = statField2;
    *(int*)(obj + 0x30) = statTotal;
    *(int*)(obj + 0x34) = statField6;
    *(int*)(obj + 0x28) = rank;
    void* op = obj;
    push_back_262AF0(((char*)self), (sObjVec_262AF0*)(((char*)self) + 0xB8), op);
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    netEvent[0] = 236;
    post_262AF0((char*)lobbyMan, netEvent);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262E20);
#ifdef SKIP_ASM
static inline unsigned int func_00262E20_count(void* self)
{
    return *(void***)((char*)self + 0xC0) - *(void***)((char*)self + 0xBC);
}

static inline void** func_00262E20_at(void* self, unsigned int i)
{
    return *(void***)((char*)*(void***)((char*)self + 0xBC) + (i << 2));
}

/**
 * Finds a pending challenge by the challenger's name, ignoring case, and returns its index.
 *
 * Compares each cChallenger's name (its first string) in mChallengersStart..mChallengersFinish with strcasecmp.
 *
 * @param name  the challenger's name
 * @return the index in the challenger vector, or -1 when there is none
 */
extern "C" int func_00262E20(BuddyMan* self, void* name)
{
    for (unsigned int i = 0; i < func_00262E20_count(self); i++) {
        void** challenger = func_00262E20_at(self, i);
        if (challenger != 0 && strcasecmp(*challenger, name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262EC0);
#ifdef SKIP_ASM
// PORT: callee defined as returning void* in this unit; called here as void.
void func_0025FC28_v(void* self, int a1) __asm__("func_0025FC28__FPvi");
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" int func_00262400(BuddyMan* self, int a1);
extern "C" void* func_002625C8(BuddyMan* self, int idx);
extern "C" int func_00262938(BuddyMan* self);
extern "C" int func_00262A38(BuddyMan* self, void* name);
extern "C" int func_00262E20(BuddyMan* self, void* name);
extern "C" int func_00263128(BuddyMan* self, char* name);
extern "C" void func_002635A8(BuddyMan* self, int i);
extern "C" void* func_00263630(BuddyMan* self, unsigned int i);
extern "C" void func_003F11F8(void* h, void* e, void* cb, void* user, int timeout);
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);
extern "C" void* strcasecmp(void*, void*);
extern "C" void func_00268F90(void* self, void* a1, int a2);

struct sBXStr_262EC0 {
    char* p;
};

struct sObjVec_262EC0 {
    void* alloc;
    void** mStart;
    void** mFinish;
};

static inline void** sObjVec_262EC0_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

/**
 * Removes a bit-20 buddy-roster entry and everything held from that player.
 *
 * Only acts on an entry that has bit 20 (func_00262400(), the kind-2 entries func_00262AF0 adds). Asks the buddy server
 * to drop it (func_003F11F8 on mpBuddyApi, callback func_00268F90, timeout 600), deletes every received message from
 * that name (func_002635A8(), newest first) and, when a challenge from it is pending, sends "decline" (func_0025FC28())
 * and erases the challenge from the vector. Likely bug: unlike func_00263018() it never deletes the erased cChallenger,
 * so the object leaks.
 *
 * @param idx  index into mpRoster
 */
extern "C" void func_00262EC0(BuddyMan* self, int idx)
{
    if (func_00262400(self, idx) == 0)
        return;
    char* name = (char*)func_002625C8(self, idx);
    if (name == 0)
        return;
    sBXStr_262EC0 nameStr;
    cBXString_cBXString2(&nameStr, name);
    func_003F11F8(self->mpBuddyApi, name, (void*)func_00268F90, ((char*)self), 600);
    if (func_00262A38(self, nameStr.p))
    {
        for (int i = func_00262938(self) - 1; i >= 0; i--)
        {
            void** message = (void**)func_00263630(self, i);
            if (message != 0 && strcasecmp(*message, nameStr.p) == 0)
                func_002635A8(self, i);
        }
    }
    if (func_00263128(self, nameStr.p))
    {
        // PORT: the name is passed through an int parameter.
        func_0025FC28_v(LobbyMan_mLobbyMan, (int)nameStr.p);
        int k = func_00262E20(self, nameStr.p);
        sObjVec_262EC0* challengers = (sObjVec_262EC0*)(((char*)self) + 0xB8);
        // PORT: pointer arithmetic through int.
        void** pos = (void**)(k * 4 + (int)(void**)self->mChallengersStart);
        if (pos + 1 != (void**)self->mChallengersFinish)
            sObjVec_262EC0_copy(pos + 1, challengers->mFinish, pos);
        --*(void***)&self->mChallengersFinish;
    }
    cBXString__cBXString(&nameStr, 2);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263018);
#ifdef SKIP_ASM
// PORT: callee defined as returning void* in this unit; called here as void.
void func_0025FC28_v(void* self, int a1) __asm__("func_0025FC28__FPvi");
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);
extern "C" int func_00262438(BuddyMan* self, int a1);
extern "C" int func_00262A38(BuddyMan* self, void* name);
extern "C" int func_00262468(BuddyMan* self, int a1);
extern "C" void* func_002625C8(BuddyMan* self, int idx);
extern "C" void func_003F11F8(void* h, void* e, void* cb, void* user, int timeout);
extern "C" void func_00268F90(void* self, void* a1, int a2);

struct sTagsUnkVEntry18 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sTagsUnkObjVec18 {
    void* alloc;
    void** mStart;
    void** mFinish;
};

static inline void** sTagsUnkObjVec18_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

/**
 * Removes the pending challenge from a player, declining it, and drops that player's temporary roster entry.
 *
 * Finds the challenge with func_00262E20(); when there is one it sends "decline" (func_0025FC28()), erases it from
 * mChallengersStart..mChallengersFinish and deletes the cChallenger through its virtual destructor (vtable at +0x3C,
 * flags 3). Then, when the name is a bit-20 roster entry (func_00262438()) with no received messages (func_00262A38()),
 * asks the buddy server to drop that entry (func_003F11F8, callback func_00268F90, timeout 600).
 *
 * @param name  the challenger's name
 */
// PORT: the name is passed through int parameters of some callees.
extern "C" void func_00263018(BuddyMan* self, void* name)
{
    int idx = func_00262E20(self, name);
    if (idx >= 0) {
        func_0025FC28_v(LobbyMan_mLobbyMan, (int)name);
        sTagsUnkObjVec18* challengers = (sTagsUnkObjVec18*)((char*)self + 0xB8);
        void** pos = (void**)self->mChallengersStart + idx;
        char* obj = (char*)*pos;
        if (pos + 1 != (void**)self->mChallengersFinish) {
            sTagsUnkObjVec18_copy(pos + 1, challengers->mFinish, pos);
        }
        --*(void***)&self->mChallengersFinish;
        if (obj != 0) {
            sTagsUnkVEntry18* vt = *(sTagsUnkVEntry18**)(obj + 0x3C);
            vt[1].fn(obj + vt[1].delta, 3);
        }
        if (func_00262438(self, (int)name)) {
            if (func_00262A38(self, name) == 0) {
                void* rosterEntry = func_002625C8(self, func_00262468(self, (int)name));
                if (rosterEntry != 0) {
                    // PORT: function pointer passed as a callback argument.
                    func_003F11F8(self->mpBuddyApi, rosterEntry, (void*)func_00268F90, self, 600);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263128);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" int func_004165A8(void*, void*);

/**
 * Tells whether a challenge from a given player is pending.
 *
 * Compares each cChallenger's name with func_004165A8 (strcmp, case-sensitive, unlike func_00262E20()); it also builds
 * and destroys an unused cBXString copy of @p name on every step.
 *
 * @param name  the challenger's name
 * @return 1 when a challenge from that player is pending, otherwise 0
 */
// Returns 1 if any entry's name equals `name`.
extern "C" int func_00263128(BuddyMan* self, char* name)
{
    if (((void***)self->mChallengersFinish - (void***)self->mChallengersStart) == 0)
        return 0;
    for (void*** it = (void***)self->mChallengersStart; it != (void***)self->mChallengersFinish; it++) {
        void** challenger = *it;
        char* nameCopy;
        cBXString_cBXString2(&nameCopy, name);
        if (func_004165A8(*challenger, name) == 0) {
            cBXString__cBXString(&nameCopy, 2);
            return 1;
        }
        cBXString__cBXString(&nameCopy, 2);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002631E0);
#ifdef SKIP_ASM
/**
 * Tells whether a challenge from a given player is pending; takes the name as a cBXString.
 *
 * Like func_00263128() (strcmp on each cChallenger's name) without the temporary copies. No caller in the source.
 *
 * @param name  the name as a cBXString (a pointer to the text pointer)
 * @return 1 when a challenge from that player is pending, otherwise 0
 */
extern "C" int func_002631E0(BuddyMan* self, void** name)
{
    if ((unsigned int)((void**)self->mChallengersFinish - (void**)self->mChallengersStart) == 0) {
        return 0;
    }
    for (void** it = (void**)self->mChallengersStart; it != (void**)self->mChallengersFinish; it++) {
        void* challenger = *it;
        if (func_004165A8(*(void**)challenger, *name) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263270);
#ifdef SKIP_ASM
/**
 * Returns the pending cChallenger from a given player.
 *
 * Compares each cChallenger's name with func_004165A8 (strcmp). cFEStateChallengeReceivedPopup_onCreateScreen uses it
 * to show the challenge's settings.
 *
 * @param name  the challenger's name
 * @return the cChallenger, or 0 when none is pending from that player
 */
extern "C" void* func_00263270(BuddyMan* self, void* name)
{
    if ((unsigned int)((void**)self->mChallengersFinish - (void**)self->mChallengersStart) == 0) {
        return 0;
    }
    for (void** it = (void**)self->mChallengersStart; it != (void**)self->mChallengersFinish; it++) {
        void* challenger = *it;
        if (func_004165A8(*(void**)challenger, name) == 0) {
            return challenger;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263308);
#ifdef SKIP_ASM
/**
 * Returns a pending challenge by index.
 *
 * @param i  index into mChallengersStart..mChallengersFinish
 * @return the cChallenger, or 0 when @p i is out of range
 */
extern "C" void* func_00263308(BuddyMan* self, unsigned int i)
{
    void** begin = (void**)self->mChallengersStart;
    void** end = (void**)self->mChallengersFinish;
    if (i >= (unsigned int)(end - begin)) {
        return 0;
    }
    return *(void**)((char*)begin + (i << 2));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263338);
#ifdef SKIP_ASM
// PORT: the target calls func_00262598 without setting $4 (self is still there from entry);
// bound here with a no-arg prototype.
extern "C" int func_00262598_noarg() __asm__("func_00262598");

/**
 * Tells whether a buddy-roster entry has bit 0 of its byte at +0xA7 or bit 20 (0x100000) of its flags word (+0xA8).
 *
 * The count check calls func_00262598() with this BuddyMan still in the first argument register (the C declares the
 * call without arguments) and, like func_002625C8(), lets idx == count through.
 *
 * @param idx  index into mpRoster
 * @return 1 when either bit is set, 0 when neither is or the entry does not exist
 */
extern "C" int func_00263338(BuddyMan* self, int idx)
{
    if (idx < 0) {
        return 0;
    }
    if (func_00262598_noarg() < idx) {
        return 0;
    }
    char* rosterEntry = (char*)func_002625C8(self, idx);
    if (rosterEntry == 0) {
        return 0;
    }
    int result = 0;
    if ((*(unsigned char*)(rosterEntry + 0xA7) & 1) || (*(unsigned int*)(rosterEntry + 0xA8) & 0x100000)) {
        result = 1;
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_002633C8);
#ifdef SKIP_ASM
extern "C" int func_00262468(BuddyMan* self, int a1);
extern "C" int func_00263338(BuddyMan* self, int a1);

/**
 * Tests func_00263338()'s two roster bits for the entry with a given name.
 *
 * @param name  the player name, passed as an int
 * @return func_00263338()'s result for the entry func_00262468() finds (0 when the name is not on the roster)
 */
extern "C" int func_002633C8(BuddyMan* self, int name)
{
    return func_00263338(self, func_00262468(self, name));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002633F8);
#ifdef SKIP_ASM
extern char D_004A2F40[];
extern char D_004A31F8[];
extern "C" const char* cDirtysock_tag_TagFieldFind(const char* record, const char* name);
// PORT: the target calls func_00262598 without setting $4 (self is still there from entry);
// bound here with a no-arg prototype.
extern "C" int func_00262598_noarg() __asm__("func_00262598");
// PORT: TagFieldGetString really takes a 4th (default value) argument; the unit declares 3.
extern "C" int cDirtysock_tag_TagFieldGetString4(const char* tag, char* buf, int size, const char* defval) __asm__("cDirtysock_tag_TagFieldGetString");

/**
 * Tells whether a buddy is running the same game resource as this player.
 *
 * Reads the "S" field (D_004A31F8; func_00262768() publishes mResource there) from the entry's presence record at +0x54
 * into a 16-byte buffer and compares it with mResource (func_004165A8, strcmp). The count check calls func_00262598()
 * with this BuddyMan still in the first argument register and lets idx == count through.
 *
 * @param idx  index into mpRoster
 * @return 1 when the entry's "S" value equals mResource, otherwise 0 (also for a missing entry)
 */
extern "C" int func_002633F8(BuddyMan* self, int idx)
{
    if (idx < 0) {
        return 0;
    }
    if (func_00262598_noarg() < idx) {
        return 0;
    }
    char* rosterEntry = (char*)func_002625C8(self, idx);
    if (rosterEntry == 0) {
        return 0;
    }
    char buf[16];
    cDirtysock_tag_TagFieldGetString4(cDirtysock_tag_TagFieldFind(rosterEntry + 0x54, D_004A31F8), buf, 0x10, D_004A2F40);
    return func_004165A8(self->mResource, buf) == 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00263490);
#ifdef SKIP_ASM
extern "C" int func_00262468(BuddyMan* self, int a1);
extern "C" int func_002633F8(BuddyMan* self, int a1);

/**
 * Tells whether the buddy with a given name runs the same game resource as this player.
 *
 * @param name  the player name, passed as an int
 * @return func_002633F8()'s result for the entry func_00262468() finds (0 when the name is not on the roster)
 */
extern "C" int func_00263490(BuddyMan* self, int name)
{
    return func_002633F8(self, func_00262468(self, name));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002634C0);
#ifdef SKIP_ASM
/**
 * Orders two buddy-roster entries by name, ignoring case; the roster's sort callback.
 *
 * BuddyMan_onconnect installs it on mpRoster with func_003EE5A0. A null entry sorts after a non-null one.
 *
 * @param sortParam    not read
 * @param lhs   first entry (its name is at +0), or 0
 * @param rhs   second entry, or 0
 * @return strcasecmp of the names; 0 when both are null, 1 when only @p lhs is, -1 when only @p rhs is
 */
extern "C" int func_002634C0(void* self, int sortParam, void* lhs, void* rhs)
{
    if (lhs == 0) {
        return rhs != 0;
    }
    if (rhs == 0) {
        return -1;
    }
    // PORT: strcasecmp (strcmp-like) is declared void* in this unit but returns int
    return (int)strcasecmp(lhs, rhs);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263500);
#ifdef SKIP_ASM
/**
 * Removes a given private message from the received-message vector and deletes it.
 *
 * Searches from the back and, on a match, erases the slot and deletes the message through its virtual destructor
 * (vtable at +0xC, flags 3). Likely bug: the walk starts at mMessagesFinish itself (one past the last element) and
 * stops before mMessagesStart, so it reads past the end and never checks the first message. No caller in the source.
 *
 * @param obj  the cPrivateMessage to remove
 */
extern "C" void func_00263500(BuddyMan* self, void* obj)
{
    for (void** it = (void**)self->mMessagesFinish; it != (void**)self->mMessagesStart; it--) {
        char* message = (char*)*it;
        if (message == obj) {
            sTagsUnkObjVec18* messages = (sTagsUnkObjVec18*)(((char*)self) + 0xA8);
            if (it + 1 != (void**)self->mMessagesFinish) {
                sTagsUnkObjVec18_copy(it + 1, messages->mFinish, it);
            }
            --*(void***)&self->mMessagesFinish;
            if (message != 0) {
                sTagsUnkVEntry18* vt = *(sTagsUnkVEntry18**)(message + 0xC);
                vt[1].fn(message + vt[1].delta, 3);
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002635A8);
#ifdef SKIP_ASM
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);

struct sTagsUnkVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sTagsUnkObjVec {
    void* alloc;
    void** mStart;
    void** mFinish;
};

static inline void** sTagsUnkObjVec_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

/**
 * Removes the received private message at an index and deletes it.
 *
 * Erases slot @p idx of mMessagesStart..mMessagesFinish (moving the later pointers down with func_0041610C) and deletes
 * the message through its virtual destructor (vtable at +0xC, flags 3). No range check.
 *
 * @param idx  index of the message
 */
extern "C" void func_002635A8(BuddyMan* self, int idx)
{
    sTagsUnkObjVec* messages = (sTagsUnkObjVec*)((char*)self + 0xA8);
    void** pos = (void**)self->mMessagesStart + idx;
    char* obj = (char*)*pos;
    if (pos + 1 != (void**)self->mMessagesFinish) {
        sTagsUnkObjVec_copy(pos + 1, messages->mFinish, pos);
    }
    --*(void***)&self->mMessagesFinish;
    if (obj != 0) {
        sTagsUnkVEntry* vt = *(sTagsUnkVEntry**)(obj + 0xC);
        vt[1].fn(obj + vt[1].delta, 3);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263630);
#ifdef SKIP_ASM
/**
 * Returns a received private message by index.
 *
 * @param i  index into mMessagesStart..mMessagesFinish
 * @return the cPrivateMessage, or 0 when @p i is out of range
 */
extern "C" void* func_00263630(BuddyMan* self, unsigned int i)
{
    void** begin = (void**)self->mMessagesStart;
    void** end = (void**)self->mMessagesFinish;
    if (i >= (unsigned int)(end - begin)) {
        return 0;
    }
    return *(void**)((char*)begin + (i << 2));
}
#endif

extern void* cConnMan_vtbl[];

//100%
INCLUDE_ASM("dirtysock/tagsunk", cConnMan_cConnMan__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the front end's connection manager: clears its state word and stores vtable cConnMan_vtbl at +0x8.
 *
 * cRealFE's constructor (cRealFE_cRealFE) builds it at +0xB5AB0; func_002636A8() starts a connection and func_00263828()
 * updates it.
 *
 * @return the object
 */
void* cConnMan_cConnMan(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x8) = (int)(void*)cConnMan_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cConnMan__cConnMan);
#ifdef SKIP_ASM
extern void* cConnMan_vtbl[];
void operator_delete(int*);

/**
 * Destroys the connection manager: restores vtable cConnMan_vtbl at +0x8, then frees the object when asked to.
 *
 * cRealFE__cRealFE calls it with flags 2 for its member at +0xB5AB0.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cConnMan__cConnMan(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = cConnMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002636A8);
#ifdef SKIP_ASM
extern float D_004A2F0C;
extern float D_004A2F10;
extern "C" void func_00255F50(void* p);

/**
 * Starts connecting to the network: calls func_00255F50 (cCommSystem's netConnConnect) and arms a timeout.
 *
 * The connection manager is cRealFE + 0xB5AB0 (func_002591B8 returns it). The caller's second argument (the network
 * configuration func_001B45E8 chose) is not in the C declaration and reaches func_00255F50 untouched in the second
 * argument register. Sets the timeout at +0x4 to 60.0 seconds on broadband (mBroadBand set) or 180.0 otherwise, and the
 * state at +0x0 to 1; func_00263828() then watches mNetConnState.
 */
extern "C" void func_002636A8(void* self)
{
    func_00255F50(cCommSystem_mCommSystem);
    float timeout;
    if (cCommSystem_mCommSystem->mBroadBand != 0) {
        timeout = D_004A2F0C;
    } else {
        timeout = D_004A2F10;
    }
    *(float*)((char*)self + 0x4) = timeout;
    *(int*)self = 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002636F0);
#ifdef SKIP_ASM
extern char str_StlBlock_b[];
extern char str_NetErrorEvent[];
extern void* online_NetErrorEvent_vtbl[];
extern "C" void func_00260F80(void);
extern "C" int func_00261460(void* p);
extern "C" void func_002613C8(BuddyMan* p);
extern "C" void func_0025B800(LobbyMan* self);
extern "C" void func_00261408(BuddyMan* p);
extern "C" void func_00255FA0(void* p);
struct sMsgListNode36F0 {
    sMsgListNode36F0* next;
    sMsgListNode36F0* prev;
    void* data;
};

struct sMsgListIter36F0 {
    sMsgListNode36F0* node;
    sMsgListIter36F0(sMsgListNode36F0* x) : node(x) {}
    sMsgListIter36F0(const sMsgListIter36F0& x) : node(x.node) {}
};

static inline sMsgListIter36F0 sMsgList36F0_insert(sMsgListIter36F0 pos, void* const& x)
{
    sMsgListNode36F0* tmp = (sMsgListNode36F0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline bool isState_36F0(int st)
{
    return cCommSystem_mCommSystem->mNetConnState == st;
}

/**
 * Logs out of the online services and drops the network connection, optionally reporting an error.
 *
 * Makes sure BuddyMan_mBuddyMan exists and is connected (func_00260F80 / func_002613C8, func_0025B800 if it still does
 * not exist), then disconnects it (func_00261408()) and the lobby (func_0025B800). Unless mNetConnState is 1 (idle) it
 * also calls func_00255FA0 (netConnDisconnect), posts an online::Event of type 278 (tag "NetErrorEvent", vtable
 * online_NetErrorEvent_vtbl, @p val at +0x8) to LobbyMan's mEventList when @p val is non-zero, and sets the connection manager's state
 * to 0. Quirk: it may create and reconnect BuddyMan only to disconnect it.
 *
 * @param out  the connection manager's state word (cRealFE + 0xB5AB0, from func_002591B8)
 * @param val  error code for the event (mNetConnError, or 0x2D3F3F3F '-???' on a timeout), or 0 for none
 */
extern "C" void func_002636F0(int* out, int val)
{
    if (BuddyMan_mBuddyMan == 0)
        func_00260F80();
    if (func_00261460(BuddyMan_mBuddyMan) == 0)
        func_002613C8(BuddyMan_mBuddyMan);
    if (BuddyMan_mBuddyMan == 0)
        func_0025B800(LobbyMan_mLobbyMan);
    func_00261408(BuddyMan_mBuddyMan);
    func_0025B800(LobbyMan_mLobbyMan);
    if (!isState_36F0(1)) {
        func_00255FA0(cCommSystem_mCommSystem);
        if (val != 0) {
            LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
            int* errorEvent = (int*)operator new(0xC, str_NetErrorEvent, 0x20000000, 0);
            errorEvent[0] = 278;
            *(void***)((char*)errorEvent + 0x4) = online_NetErrorEvent_vtbl;
            errorEvent[2] = val;
            void* mp = errorEvent;
            sMsgList36F0_insert((*(sMsgListNode36F0**)((char*)lobbyMan + 0xF0)), mp);
        }
        *out = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263828);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern "C" void func_002636F0(int* out, int val);

static inline bool isState_3828(int st)
{
    return cCommSystem_mCommSystem->mNetConnState == st;
}
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNode3828 {
    sMsgListNode3828* next;
    sMsgListNode3828* prev;
    void* data;
};

struct sMsgListIter3828 {
    sMsgListNode3828* node;
    sMsgListIter3828(sMsgListNode3828* x) : node(x) {}
    sMsgListIter3828(const sMsgListIter3828& x) : node(x.node) {}
};

static inline sMsgListIter3828 sMsgList3828_insert(sMsgListIter3828 pos, void* const& x)
{
    sMsgListNode3828* tmp = (sMsgListNode3828*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_3828(char* self, void* msg)
{
    void* mp = msg;
    sMsgList3828_insert(*(sMsgListNode3828**)(self + 0xF0), mp);
}

struct sDsTimer_263828 {
    int state;
    float timer;
};

/**
 * Updates the connection manager started by func_002636A8(), once per front-end frame (cRealFE_update).
 *
 * State 1 (connecting): when mNetConnState reaches 5 it moves to state 2 and posts an online::Event of type 261 to
 * LobbyMan's mEventList; at 6 (failed) it calls func_002636F0() with mNetConnError; otherwise it counts the timeout at
 * +0x4 down by the frame time (SSXApp + 0x14) and calls func_002636F0() with 0x2D3F3F3F once it is negative. State 2
 * (connected): a later mNetConnState of 6 calls func_002636F0() with mNetConnError. State 0 does nothing.
 */
extern "C" void func_00263828(int* self)
{
    sDsTimer_263828* connection = (sDsTimer_263828*)self;
    switch (connection->state)
    {
    case 0:
        break;
    case 1:
        if (isState_3828(5)) {
            connection->state = 2;
            LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
            int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
            *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
            netEvent[0] = 261;
            post_3828((char*)lobbyMan, netEvent);
        } else if (isState_3828(6)) {
            func_002636F0(self, cCommSystem_mCommSystem->mNetConnError);
        } else {
            int expired = connection->timer < 0.0f;
            if (expired)
                func_002636F0(self, 0x2D3F3F3F);
            else
                connection->timer -= *(float*)((char*)SSXApp + 0x14);
        }
        break;
    case 2:
        if (isState_3828(6))
            func_002636F0(self, cCommSystem_mCommSystem->mNetConnError);
        break;
    }
}
#endif

extern void* cGameConnMan_vtbl[];

//100%
INCLUDE_ASM("dirtysock/tagsunk", cGameConnMan_cGameConnMan__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the front end's game-connection manager: clears its state word and stores vtable cGameConnMan_vtbl at +0x14.
 *
 * cRealFE's constructor (cRealFE_cRealFE) builds it at +0xB5ABC; func_002639E0() starts it, func_00263CF0() updates it
 * and func_00263BA8() shuts it down.
 *
 * @return the object
 */
void* cGameConnMan_cGameConnMan(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x14) = (int)(void*)cGameConnMan_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cGameConnMan__cGameConnMan);
#ifdef SKIP_ASM
extern void* cGameConnMan_vtbl[];
void operator_delete(int*);

/**
 * Destroys the game-connection manager: restores vtable cGameConnMan_vtbl at +0x14, then frees the object when asked to.
 *
 * cRealFE__cRealFE calls it with flags 2 for its member at +0xB5ABC.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cGameConnMan__cGameConnMan(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = cGameConnMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002639E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cBENewRaceInterface_setNumberHumans(void* self, int n);
extern "C" void cBENewPlayerInterface_setPlayerCtrlID(void* self, int a1, int a2);
extern "C" int cCommSystem_openDSockChannel(void* comm, const char* addr, unsigned short port, int a3);
extern "C" void func_00266D58(void* self, void* ip);
extern "C" void func_00266E30(void* self);
extern void* D_004A33F4;
extern float D_004A2F14;
extern int cBEOptionInterface_mOption[];
extern char str_pct_d_pct_d_pct_d_pct_d_b[];

struct sVE_002639E0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sIf_002639E0 {
    char pad[0xC];
    sVE_002639E0* vt;
};

struct sNetCfg_002639E0 {
    int active;     // 0x0
    int f4;         // 0x4
    int chan;       // 0x8
    int port;       // 0xC
    float rate;     // 0x10
};


/**
 * Sets up a two-player online race and opens the peer-to-peer channel to the opponent.
 *
 * Sets the new-race interface to 2 humans, player 0's controller to @p ctrlId and player 1's to 0 (each interface's slot-1
 * virtual applies it), and writes the network options: bIsNetworkGame 1, fLatency 1/60, iPort 3658, bIsHost @p isHost
 * (the network interface's slot-1 virtual runs twice). Formats @p ip as dotted decimal and opens a DirtySock channel to
 * it with cCommSystem_openDSockChannel(), keeping the channel at +0x4. When bit 18 of cBEOptionInterface_mOption is set
 * it also passes @p ip to the voice manager D_004A33F4 (tag "cVoipMan"): func_00266D58, then func_00266E30. Finally
 * sets the timeout at +0x10 to 60.0 and the state at +0x0 to 1 for func_00263CF0(). func_00259098 calls it with the
 * opponent's lobby address once LobbyMan has a game.
 *
 * @param ip      the opponent's IPv4 address, most significant byte first
 * @param isHost  non-zero when this console hosts the race; stored at +0x8
 * @param ctrlId      controller id for player 0
 * @param cancel      stored at +0xC; when non-zero func_00263CF0() shuts the connection down (event 268) as soon as it
 *                connects
 */
extern "C" void func_002639E0(int* self, unsigned int ip, int isHost, int ctrlId, int cancel)
{
    char buf[256];
    int one = 1;
    self[3] = cancel;
    sIf_002639E0* playerInterface = (sIf_002639E0*)cBE_getInterface_Fv(cBE_getBE(), 1);
    sIf_002639E0* raceInterface = (sIf_002639E0*)cBE_getInterface_Fv(cBE_getBE(), 0);
    cBENewRaceInterface_setNumberHumans(raceInterface, 2);
    raceInterface->vt[1].fn((char*)raceInterface + raceInterface->vt[1].delta);
    cBENewPlayerInterface_setPlayerCtrlID(playerInterface, 0, ctrlId);
    cBENewPlayerInterface_setPlayerCtrlID(playerInterface, 1, 0);
    playerInterface->vt[1].fn((char*)playerInterface + playerInterface->vt[1].delta);
    sIf_002639E0* networkInterface = (sIf_002639E0*)cBE_getInterface_Fv(SSXApp->BE, 7);
    cBENetworkInterface_networkOpt.bIsNetworkGame = one;
    cBENetworkInterface_networkOpt.fLatency = 0.01666666753590107f;
    cBENetworkInterface_networkOpt.iPort = 3658;
    cBENetworkInterface_networkOpt.bIsHost = isHost;
    networkInterface->vt[1].fn((char*)networkInterface + networkInterface->vt[1].delta);
    networkInterface->vt[1].fn((char*)networkInterface + networkInterface->vt[1].delta);
    self[2] = isHost;
    sprintf(buf, str_pct_d_pct_d_pct_d_pct_d_b, ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
    self[1] = cCommSystem_openDSockChannel(cCommSystem_mCommSystem, buf, cBENetworkInterface_networkOpt.iPort, isHost);
    cBE_getInterface_Fv(cBE_getBE(), 4);
    if ((cBEOptionInterface_mOption[0] >> 18) & 1) {
        func_00266D58(D_004A33F4, (void*)ip);
        func_00266E30(D_004A33F4);
    }
    *(float*)&self[4] = D_004A2F14;
    self[0] = one;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263BA8);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" int func_003E8B10(void* p, int tag, void* cb, int a3, void* user);
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* D_004A33CC;
extern void* D_004A33F4;
extern "C" int func_00255D38(void* comm, int chan);
extern "C" void func_00255CC8(void* comm, int chan);
extern "C" void func_00264098(void);
extern "C" void func_00266DF8(void* self);
extern "C" void func_00266E88(void* self);
extern "C" void func_0025F740(LobbyMan* self, int type);
extern void* online_Event_vtbl[];
extern void* online_ErrorEvent_vtbl[];

struct sMsgListNode3BA8 {
    sMsgListNode3BA8* next;
    sMsgListNode3BA8* prev;
    void* data;
};

struct sMsgListIter3BA8 {
    sMsgListNode3BA8* node;
    sMsgListIter3BA8(sMsgListNode3BA8* x) : node(x) {}
    sMsgListIter3BA8(const sMsgListIter3BA8& x) : node(x.node) {}
};

static inline sMsgListIter3BA8 sMsgList3BA8_insert(sMsgListIter3BA8 pos, void* const& x)
{
    sMsgListNode3BA8* tmp = (sMsgListNode3BA8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void post_3BA8(char* self, void* msg)
{
    void* mp = msg;
    sMsgList3BA8_insert(*(sMsgListNode3BA8**)(self + 0xF0), mp);
}

/**
 * Shuts the game-connection manager down and tells the front end why.
 *
 * When it is active and its channel is open (func_00255D38) it closes the channel (func_00255CC8) and calls
 * func_00266E88 and func_00266DF8 on the voice manager D_004A33F4. It destroys the setup manager if there is one
 * (func_00264098()), calls func_0025F740(LobbyMan, 0), clears the state and posts an online::Event to LobbyMan's
 * mEventList: type 268 for @p reason 1, type 274 for @p reason 0, none otherwise (callers pass 2).
 *
 * @param gameConn    the game-connection manager (cRealFE + 0xB5ABC, from func_002591D0)
 * @param reason  reason: 0 connect failure or timeout, 1 cancelled after connecting, other values post nothing
 */
extern "C" void func_00263BA8(void* gameConn, int reason)
{
    int* self = (int*)gameConn;
    if (self[0] != 0 && func_00255D38(cCommSystem_mCommSystem, self[1]) != 0)
    {
        func_00255CC8(cCommSystem_mCommSystem, self[1]);
        func_00266E88(D_004A33F4);
        func_00266DF8(D_004A33F4);
    }
    if (D_004A33CC != 0)
        func_00264098();
    func_0025F740(LobbyMan_mLobbyMan, 0);
    self[0] = 0;
    if (reason == 1) {
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
        netEvent[0] = 268;
        post_3BA8((char*)lobbyMan, netEvent);
    } else if (reason == 0) {
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
        netEvent[0] = 274;
        post_3BA8((char*)lobbyMan, netEvent);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263CF0);
#ifdef SKIP_ASM
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setRiderCharID(void* iface, int rider, int ch);
extern "C" void cGameComm_construct(int a, int b);
extern "C" void cBENetworkInterface_backupCharProfile(void* self, int a);
extern "C" int func_00255D50(void* comm, int chan);
extern "C" int func_00255E40(void* comm, int chan);
extern "C" void func_00256C50(void);
extern "C" void func_0025B800(LobbyMan* self);
extern "C" void func_00260F80(void);
extern "C" void func_002613C8(BuddyMan* p);
extern "C" int func_00261460(void* p);
extern "C" void func_00262768(BuddyMan* self, int a1, int a2, int a3, int a4, int a5);
extern "C" void func_00263BA8(void* p, int arg);
extern "C" void func_00264030(int a1, int a2, int a3);
extern "C" void func_00264098(void);
extern "C" void func_002642B8(void* self);
// PORT: func_00264CB8 is defined with one parameter; this caller passes a second in $a1.
extern "C" void func_00264CB8_2(void* self, int a) __asm__("func_00264CB8");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern void* D_004A33CC;
extern int cRealFE_mStartState;
extern int D_00534B34[];
extern int D_00534E58[];
extern char str_cPreGameLoadScreen_c[];

struct sVEntry_263CF0 { short delta; short index; void (*fn)(void*); };

static inline bool tsTimedOut_263CF0(int* self)
{
    return *(float*)(self + 4) < 0.0f;
}

static inline bool tsIsState_263CF0(int st)
{
    return *(int*)D_004A33CC == st;
}

static inline void* Mgr_263CF0()
{
    if (BuddyMan_mBuddyMan == 0)
        func_00260F80();
    if (func_00261460(BuddyMan_mBuddyMan) == 0)
        func_002613C8(BuddyMan_mBuddyMan);
    if (BuddyMan_mBuddyMan == 0)
        func_0025B800(LobbyMan_mLobbyMan);
    return BuddyMan_mBuddyMan;
}

/**
 * Updates the game-connection manager once per front-end frame (cRealFE_update) and starts the race when both sides are
 * set up.
 *
 * State 1 (connecting): once the channel is connected (func_00255D50) it either shuts down with reason 1 (cancel flag
 * +0xC set) or creates the setup manager (func_00264030() with the channel, host flag and bIsRankedGame), sends our
 * rank to the peer (func_00264CB8()) and moves to state 2; a channel error (func_00255E40) or an expired timeout
 * (+0x10, counted down by the frame time) shuts down with reason 0. State 2: a channel error posts event 275 (unless
 * the setup manager is in state 11) and shuts down with reason 2; otherwise it updates the setup manager
 * (func_002642B8()). Setup state 11 shuts down with reason 2; state 13 starts the race: destroys the setup manager,
 * disconnects the lobby (func_0025B800), sets cRealFE_mStartState to 52, publishes buddy presence (state 1, location 9,
 * the opponent's name), applies MyCharacterID to rider 0, backs up the character profile, replaces any old cGameComm
 * (func_00256C50, cGameComm_construct with the channel and host flag), queues a cPreGameLoadScreen as the next module
 * and returns to state 0.
 */
extern "C" void func_00263CF0(int* self)
{
    switch (self[0])
    {
    case 0:
        break;
    case 1:
        if (func_00255D50(cCommSystem_mCommSystem, self[1]))
        {
            if (self[3] != 0)
            {
                func_00263BA8(self, 1);
                return;
            }
            cBE_getInterface_Fv(SSXApp->BE, 7);
            func_00264030(self[1], self[2], D_00534B34[0]);
            func_00264CB8_2(D_004A33CC, ((int)LobbyMan_mLobbyMan->unk_08C));
            self[0] = 2;
            return;
        }
        if (func_00255E40(cCommSystem_mCommSystem, self[1]) || tsTimedOut_263CF0(self))
        {
            func_00263BA8(self, 0);
            return;
        }
        *(float*)(self + 4) = *(float*)(self + 4) - *(float*)((char*)SSXApp + 0x14);
        return;
    case 2:
        if (func_00255E40(cCommSystem_mCommSystem, self[1]))
        {
            if (!tsIsState_263CF0(11))
            {
                LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
                int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
                *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
                netEvent[0] = 275;
                post_3BA8((char*)lobbyMan, netEvent);
            }
            func_00263BA8(self, 2);
            return;
        }
        {
            func_002642B8(D_004A33CC);
            int go = 0;
            int st = *(int*)D_004A33CC;
            if (st == 13)
                go = 1;
            else if (st == 11)
                func_00263BA8(self, 2);
            if (go == 0)
                return;
            func_00264098();
            func_0025B800(LobbyMan_mLobbyMan);
            cRealFE_mStartState = 52;
            func_00262768((BuddyMan*)Mgr_263CF0(), 1, 9, ((int)LobbyMan_mLobbyMan->mOpponent), 0, 0);
            void* bi7 = cBE_getInterface_Fv(SSXApp->BE, 7);
            char* bi1 = (char*)cBE_getInterface_Fv(SSXApp->BE, 1);
            cBENewPlayerInterface_setRiderCharID(bi1, 0, D_00534E58[0]);
            sVEntry_263CF0* vt = *(sVEntry_263CF0**)(bi1 + 0xC);
            vt[1].fn(bi1 + vt[1].delta);
            cBENetworkInterface_backupCharProfile(bi7, 0);
            if (cGameComm_mGameComm != 0)
                func_00256C50();
            cGameComm_construct(self[1], self[2]);
            cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen_c, 0x100, 0)));
            self[0] = 0;
        }
        return;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264030);
#ifdef SKIP_ASM
extern char str_cSetupMan[];
extern void* D_004A33CC;
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002640C0(void* self, int a1, int a2, int a3);

/**
 * Creates the online setup manager D_004A33CC: allocates 300 bytes (tag "cSetupMan") and constructs it with
 * func_002640C0().
 *
 * @param channel  comm channel index (from the game-connection manager)
 * @param isHost  non-zero on the host
 * @param ranked  bIsRankedGame from the network options
 */
extern "C" void func_00264030(int channel, int isHost, int ranked)
{
    D_004A33CC = func_002640C0(cMemMan_alloc(300, str_cSetupMan, 0x20000000, 0), channel, isHost, ranked);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264098);
#ifdef SKIP_ASM
extern void* D_004A33CC;
extern "C" void func_00264138(void* self, int flags);

/**
 * Destroys the online setup manager D_004A33CC, if any (func_00264138() with flags 3), and clears the pointer.
 */
extern "C" void func_00264098(void)
{
    if (D_004A33CC != 0) {
        func_00264138(D_004A33CC, 3);
        D_004A33CC = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002640C0);
#ifdef SKIP_ASM
extern "C" void* MEM_fill(void* dst, int value, int size);

/**
 * Constructs the online setup manager (the 300-byte "cSetupMan" object func_00264030 allocates).
 *
 * Sets the state (+0x0) to 1, clears the pending receive buffer (+0x4), both 32-slot packet queues (receive: counts at
 * +0x8/+0xC, slots at +0x10; send: counts at +0x90/+0x94, slots at +0x98) and the words at +0x118/+0x11C, and stores
 * the arguments.
 *
 * @param channel  comm channel index; stored at +0x120
 * @param isHost   non-zero on the host; stored at +0x124
 * @param ranked       bIsRankedGame; stored at +0x128 (func_00264E50() then sends flat attributes)
 * @return the object
 */
extern "C" void* func_002640C0(void* self, int channel, int isHost, int ranked)
{
    *(int*)((char*)self + 0x0) = 1;
    *(int*)((char*)self + 0x120) = channel;
    *(int*)((char*)self + 0x124) = isHost;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x118) = 0;
    *(int*)((char*)self + 0x11C) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x128) = ranked;
    MEM_fill((char*)self + 0x10, 0, 0x80);
    *(int*)((char*)self + 0x90) = 0;
    *(int*)((char*)self + 0x94) = 0;
    MEM_fill((char*)self + 0x98, 0, 0x80);
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264138);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void func_00264990(void* self, int* p);

struct sRing_264138
{
    int end;
    int head;
    int* q[32];
};

struct sRingOwner_264138
{
    int f0;
    int* f4;
    sRing_264138 a;     // 0x8
    sRing_264138 b;     // 0x90
};

static inline int* PeekA_264138(sRingOwner_264138* o)
{
    if (o->a.head < o->a.end)
        return o->a.q[o->a.head % 32];
    return 0;
}

static inline int* PopA_264138(sRingOwner_264138* o)
{
    int h = o->a.head++;
    return o->a.q[h % 32];
}

static inline int* PeekB_264138(sRingOwner_264138* o)
{
    if (o->b.head < o->b.end)
        return o->b.q[o->b.head % 32];
    return 0;
}

static inline int* PopB_264138(sRingOwner_264138* o)
{
    int h = o->b.head++;
    return o->b.q[h % 32];
}

/**
 * Destroys the online setup manager: frees the pending receive buffer and every packet still in its two queues.
 *
 * Each packet goes to func_00264990(); the state is then set to 1 and the object freed when bit 0 of @p flags is set.
 *
 * @param self_  the setup manager (D_004A33CC)
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_00264138(void* self_, int flags)
{
    sRingOwner_264138* self = (sRingOwner_264138*)self_;
    if (self->f4 != 0)
        func_00264990(self, self->f4);
    while (PeekA_264138(self) != 0)
        func_00264990(self, PopA_264138(self));
    while (PeekB_264138(self) != 0)
        func_00264990(self, PopB_264138(self));
    self->f0 = 1;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002642B8);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern "C" void func_00264A58(void* self);
extern "C" void func_00264B20(void* p);
extern "C" void func_00264BE8(void* self, void* msg, int size);
// PORT: func_002649B0 is defined with two parameters; this caller also passes the buffer size.
extern "C" int func_002649B0_3(void* self, void* dst, int size) __asm__("func_002649B0");
extern "C" void func_00264F88(void* self, int a1);
extern "C" void func_0025F740(LobbyMan* self, int type);
extern "C" void func_00260F80(void);
extern "C" int func_00261460(void* p);
extern "C" void func_002613C8(BuddyMan* p);
extern "C" void func_0025B800(LobbyMan* self);
extern "C" int func_00263128(BuddyMan* self, char* name);
extern "C" void func_00263018(BuddyMan* self, void* name);
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int i);
// PORT: func_00265190 takes an sName7_00264D80* (defined later in the unit).
extern "C" void func_00265190_p(void* self, int rider, void* name) __asm__("func_00265190");
extern "C" void func_00265290(void* self, int rider);
extern "C" void func_00265398(int* self);
extern "C" void func_00269008(void* self, int a, int b);
extern "C" void func_002690C8(void* self, int a, int b);
extern "C" void func_00269188(void* self, int b);
extern "C" void func_00269240(void* self, int a, int b, void* c);
extern "C" void func_00269300(void* self, int a, int b);
extern "C" void func_002693C0(void* self, int b);
extern "C" void func_00269478(void* self, int a);
extern "C" void func_00269538(void* self, int a, int b);
extern "C" void func_00269608(void* self);
extern "C" void func_002696F8(void* self);
extern "C" void func_002697D0(void* self, int a);
extern "C" void func_00269890(void* self, int a);
extern "C" void func_00269948(void* self, int a, int b);
extern "C" void func_00269A18(void* self, int a, int b);
extern "C" void func_00269AE0(void* self, int a);
extern "C" void func_00269B90(void* self, int a);
extern "C" void func_00269C40(void* self, int a);
extern "C" void func_00269CF0(void* self, int a);

struct sMsgListNode42B8 {
    sMsgListNode42B8* next;
    sMsgListNode42B8* prev;
    void* data;
};

struct sMsgListIter42B8 {
    sMsgListNode42B8* node;
    sMsgListIter42B8(sMsgListNode42B8* x) : node(x) {}
    sMsgListIter42B8(const sMsgListIter42B8& x) : node(x.node) {}
};

static inline sMsgListIter42B8 sMsgList42B8_insert(sMsgListIter42B8 pos, void* const& x)
{
    sMsgListNode42B8* tmp = (sMsgListNode42B8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

static inline void* Mgr_42B8()
{
    if (BuddyMan_mBuddyMan == 0)
        func_00260F80();
    if (func_00261460(BuddyMan_mBuddyMan) == 0)
        func_002613C8(BuddyMan_mBuddyMan);
    if (BuddyMan_mBuddyMan == 0)
        func_0025B800(LobbyMan_mLobbyMan);
    return BuddyMan_mBuddyMan;
}

// 0x14-byte session packet.
struct sMsg42B8 {
    unsigned int type;  // 0x0
    int a;              // 0x4
    int b;              // 0x8
    int c;              // 0xC
    int d;              // 0x10
};


static inline void post_42B8(char* g, void* msg)
{
    void* mp = msg;
    sMsgList42B8_insert(*(sMsgListNode42B8**)(g + 0xF0), mp);
}

struct sNetSession42B8 {
    int state;                  // 0x0
    char pad_0x4[0x124 - 0x4];
    int host;                   // 0x124
    int ready;                  // 0x128
};

/**
 * Updates the online setup manager (D_004A33CC): pumps its packet queues and handles one message from the peer.
 *
 * Pumps both queues (func_00264A58(), func_00264B20()). In state 5 or 10: when the game is ranked (+0x128) or the state
 * is 10, the host moves to state 12 and sends message 0xF; an unranked state 5 moves to 7 and posts an online::Event of
 * type 246. Then it pops one 20-byte message (func_002649B0()): 0xF moves to state 13 (the client first answers 0xF and
 * sets 12); 0x10 stores the peer's rank as LobbyMan's mOpponentStats and sends our character pick (func_00264F88()).
 * Other types go by role and phase (state below 7 or not) to per-type handlers: host 1/3/5 (func_00265190,
 * func_00265290, func_00265398) or 7/9/11/13 (func_00269478 .. func_002696F8); client 0..5 (func_00269240,
 * func_00269008 ..) or 6..13 (func_002697D0 .. func_00269CF0). 0xF8 and 250 (abort) set state 11, remove a pending
 * challenge from mOpponent (func_00263018()) and pass the type to func_0025F740. Likely bug: the 0x10 case block-copies
 * a tLobbyStats of which only mRank is set, so the other four fields get stack garbage.
 *
 * @param vself  the setup manager: state +0x0, host flag +0x124, ranked flag +0x128
 */
extern "C" void func_002642B8(void* vself)
{
    sNetSession42B8* self = (sNetSession42B8*)vself;
    sMsg42B8 msg;
    func_00264A58(self);
    func_00264B20(self);
    int st = self->state;
    if (st == 5 || st == 10) {
        if (self->ready != 0 || st == 10) {
            if (self->host != 0) {
                self->state = 12;
                msg.type = 0xF;
                msg.a = 0;
                msg.b = 0;
                func_00264BE8(self, &msg, 20);
            }
        } else {
            self->state = 7;
            LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
            int* netEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
            *(void***)((char*)netEvent + 4) = online_Event_vtbl;
            netEvent[0] = 246;
            post_42B8((char*)lobbyMan, netEvent);
        }
    }
    if (func_002649B0_3(self, &msg, 20) == 0)
        return;
    if (msg.type == 0xF) {
        if (self->host == 0) {
            self->state = 0xC;
            sMsg42B8 reply;
            reply.type = 0xF;
            reply.a = 0;
            reply.b = 0;
            func_00264BE8(self, &reply, 20);
        }
        self->state = 13;
        return;
    }
    if (msg.type == 0x10) {
        tLobbyStats info;
        ((int*)&info.mRank)[0] = msg.a;
        (*(&LobbyMan_mLobbyMan->mOpponentStats)) = info;
        func_00264F88(self, cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(SSXApp->BE, 1), 0));
        return;
    }
    if (self->host != 0) {
        if (self->state < 7) {
            switch (msg.type) {
            case 1: func_00265190_p(self, msg.a, &msg.c); break;
            case 3: func_00265290(self, msg.a); break;
            case 5: func_00265398((int*)self); break;
            case 0xF8:
            case 250: {
                self->state = 11;
                if (func_00263128((BuddyMan*)((char*)Mgr_42B8()), LobbyMan_mLobbyMan->mOpponent) != 0)
                    func_00263018((BuddyMan*)Mgr_42B8(), LobbyMan_mLobbyMan->mOpponent);
                func_0025F740(LobbyMan_mLobbyMan, msg.type);
                break;
            }
            }
        } else {
            switch (msg.type) {
            case 7: func_00269478(self, msg.a); break;
            case 9: func_00269538(self, msg.a, msg.b); break;
            case 11: func_00269608(self); break;
            case 13: func_002696F8(self); break;
            case 0xF8:
            case 250: {
                self->state = 11;
                if (func_00263128((BuddyMan*)((char*)Mgr_42B8()), LobbyMan_mLobbyMan->mOpponent) != 0)
                    func_00263018((BuddyMan*)Mgr_42B8(), LobbyMan_mLobbyMan->mOpponent);
                func_0025F740(LobbyMan_mLobbyMan, msg.type);
                break;
            }
            }
        }
    } else {
        if (self->state < 7) {
            switch (msg.type) {
            case 1: func_00269008(self, msg.a, msg.b); break;
            case 3: func_002690C8(self, msg.a, msg.b); break;
            case 5: func_00269188(self, msg.b); break;
            case 0: func_00269240(self, msg.a, msg.b, &msg.c); break;
            case 2: func_00269300(self, msg.a, msg.b); break;
            case 4: func_002693C0(self, msg.b); break;
            case 0xF8:
            case 250: {
                self->state = 11;
                if (func_00263128((BuddyMan*)((char*)Mgr_42B8()), LobbyMan_mLobbyMan->mOpponent) != 0)
                    func_00263018((BuddyMan*)Mgr_42B8(), LobbyMan_mLobbyMan->mOpponent);
                func_0025F740(LobbyMan_mLobbyMan, msg.type);
                break;
            }
            }
        } else {
            switch (msg.type) {
            case 6: func_002697D0(self, msg.a); break;
            case 7: func_00269890(self, msg.a); break;
            case 8: func_00269948(self, msg.a, msg.b); break;
            case 9: func_00269A18(self, msg.a, msg.b); break;
            case 10: func_00269AE0(self, msg.a); break;
            case 11: func_00269B90(self, msg.a); break;
            case 0xC: func_00269C40(self, msg.a); break;
            case 13: func_00269CF0(self, msg.a); break;
            case 0xF8:
            case 250: {
                self->state = 11;
                if (func_00263128((BuddyMan*)((char*)Mgr_42B8()), LobbyMan_mLobbyMan->mOpponent) != 0)
                    func_00263018((BuddyMan*)Mgr_42B8(), LobbyMan_mLobbyMan->mOpponent);
                func_0025F740(LobbyMan_mLobbyMan, msg.type);
                break;
            }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264960);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_tGameCommPacket[];

/**
 * Allocates a 0xF0-byte buffer for one setup packet (tag "tGameCommPacket").
 *
 * The C declares no parameters; callers pass the setup manager in the first argument register, which is ignored.
 *
 * @return the new buffer
 */
extern "C" void* func_00264960(void)
{
    return cMemMan_alloc(0xF0, str_tGameCommPacket, 0x20000000, 0);
}
#endif

void operator_delete(int*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264990);
#ifdef SKIP_ASM
/**
 * Frees a setup packet buffer (operator delete).
 *
 * @param packet  the buffer to free
 */
extern "C" void func_00264990(void* self, int* packet)
{
    operator_delete(packet);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002649B0);
#ifdef SKIP_ASM
extern "C" void func_00264A58(void* self);
extern "C" void* MEM_copy(void* dst, const void* src, int n);

struct sDsPacketK2 {
    unsigned short kind;   // 0x0
    unsigned short size;   // 0x2 (including this 4-byte header)
    char data[1];          // 0x4
};

struct sDsPacketQueueK2 {
    int pad_0x0[2];
    int tail;                    // 0x8
    int head;                    // 0xC
    sDsPacketK2* slots[32];      // 0x10
};

/**
 * Pops the oldest received packet of the setup manager and copies its payload out.
 *
 * Receive queue: tail +0x8, head +0xC, 32 slots at +0x10. First pumps the channel (func_00264A58()). Copies the
 * packet's size (u16 at +0x2, header included) minus 4 bytes from +0x4 into @p dst, then frees the packet. Quirk: there
 * is no destination size; func_002642B8 passes one (20) in the third argument register, which is ignored, so a larger
 * packet would overrun its buffer.
 *
 * @param dst   receives the payload
 * @return the payload size, or 0 when the queue is empty
 */
// Pops the oldest queued packet into `dst`; returns its payload size (0 if empty).
extern "C" int func_002649B0(sDsPacketQueueK2* self, void* dst)
{
    func_00264A58(self);
    int head = self->head;
    if (head != self->tail) {
        self->head = head + 1;
        sDsPacketK2* packet = self->slots[head % 32];
        int len = packet->size - 4;
        MEM_copy(dst, packet->data, len);
        func_00264990(self, (int*)packet);
        return len;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264A58);
#ifdef SKIP_ASM
extern "C" int func_00255DC0(void* mgr, int idx, void* buf, int len);
// PORT: func_00264960 is defined as f(void); this caller passes self in $a0.
void* func_00264960_self(void* self) __asm__("func_00264960");

struct sDsRecvQueue_00264A58 {
    int f0;                     // 0x0
    void* cur;                  // 0x4
    int tail;                   // 0x8
    int head;                   // 0xC
    void* slots[32];            // 0x10
    char pad_0x90[0x120 - 0x90];
    int chan;                   // 0x120
};

/**
 * Reads every pending packet from the setup manager's comm channel into its 32-slot receive queue.
 *
 * Allocates a 0xF0-byte buffer (func_00264960()) as the pending one at +0x4 and fills it with func_00255DC0 on the
 * channel (+0x120); each packet read is queued. It stops when the read returns nothing (freeing the unused buffer) or
 * when 31 packets are queued (keeping the filled buffer pending for the next call).
 *
 * @param setupMan  the setup manager (receive queue: tail +0x8, head +0xC, slots +0x10)
 */
// Fills the 32-entry receive queue until it is full or no packet is pending.
extern "C" void func_00264A58(void* setupMan)
{
    sDsRecvQueue_00264A58* self = (sDsRecvQueue_00264A58*)setupMan;
    void* buf;
    while (self->cur != 0
           || (buf = func_00264960_self(self), self->cur = buf,
               func_00255DC0(cCommSystem_mCommSystem, self->chan, buf, 0xF0) != 0))
    {
        bool full = self->tail - self->head >= 0x1F;
        if (full)
            return;
        self->slots[self->tail++ % 32] = self->cur;
        self->cur = 0;
    }
    operator_delete((int*)self->cur);
    self->cur = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264B20);
#ifdef SKIP_ASM
extern "C" int func_00255E00(void* mgr, int idx, void* buf, int len);

struct sDsPacket_00264B20 {
    unsigned short kind;   // 0x0
    unsigned short size;   // 0x2 (including this 4-byte header)
};

struct sDsSendQueue_00264B20 {
    char pad_0x00[0x90];
    int tail;                          // 0x90
    int head;                          // 0x94
    sDsPacket_00264B20* slots[32];     // 0x98
    char pad_0x118[0x120 - 0x118];
    int chan;                          // 0x120
};

/**
 * Sends the setup manager's queued outgoing packets until the queue is empty or the channel refuses one.
 *
 * Each packet goes to func_00255E00 on the channel (+0x120) with its size (u16 at +0x2); a sent packet is freed with
 * func_00264990().
 *
 * @param setupMan  the setup manager (send queue: tail +0x90, head +0x94, slots +0x98)
 */
// Sends queued packets until the queue is empty or the channel refuses one.
extern "C" void func_00264B20(void* setupMan)
{
    sDsSendQueue_00264B20* self = (sDsSendQueue_00264B20*)setupMan;
    while (self->head != self->tail)
    {
        sDsPacket_00264B20* pkt;
        int head = self->head;
        if (head < self->tail)
            pkt = self->slots[head % 32];
        else
            pkt = 0;
        if (func_00255E00(cCommSystem_mCommSystem, self->chan, pkt, pkt->size) == 0)
            return;
        self->head++;
        func_00264990(self, (int*)pkt);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264BE8);
#ifdef SKIP_ASM
extern "C" int func_00255E00(void* mgr, int idx, void* buf, int len);
extern "C" void func_00264B20(void* p);
// PORT: func_00264960 is defined as f(void); this caller passes self in $a0.
void* func_00264960_self(void* self) __asm__("func_00264960");

struct sDsOutPacket_00264BE8 {
    unsigned char kind;     // 0x0
    unsigned char flags;    // 0x1
    unsigned short size;    // 0x2 (including this 4-byte header)
    char data[0xEC];        // 0x4
};

struct sDsSendQueue_00264BE8 {
    char pad_0x00[0x90];
    int tail;                          // 0x90
    int head;                          // 0x94
    void* slots[32];                   // 0x98
    char pad_0x118[0x120 - 0x118];
    int chan;                          // 0x120
};

// PORT: the unit declares func_00264BE8 as returning void (callers ignore the result); the body
// returns 1, so it is bound by asm label.
int func_00264BE8_impl(void* p, const void* src, int len) __asm__("func_00264BE8");

/**
 * Sends a message to the peer through the setup manager's channel, queueing it when the channel is busy.
 *
 * Flushes the send queue first (func_00264B20()), then wraps @p src in a packet with a 4-byte header (kind 0, flags 0,
 * u16 size = @p len + 4) and offers it to func_00255E00; when that refuses, a copy goes into a new 0xF0-byte buffer at
 * the tail of the send queue. Quirks: there is no check that the queue has room or that @p len fits 0xEC bytes, and a
 * new message can go out ahead of older ones still queued.
 *
 * @param setupMan  the setup manager (send queue: tail +0x90, slots +0x98; channel +0x120)
 * @param src       the message
 * @param len       message size in bytes
 * @return always 1
 */
// Flushes the send queue, then sends `len` bytes now or queues a copy for later.
int func_00264BE8_impl(void* setupMan, const void* src, int len)
{
    sDsSendQueue_00264BE8* self = (sDsSendQueue_00264BE8*)setupMan;
    func_00264B20(self);
    sDsOutPacket_00264BE8 pkt;
    pkt.kind = 0;
    pkt.flags = 0;
    pkt.size = len + 4;
    MEM_copy(pkt.data, src, len);
    if (func_00255E00(cCommSystem_mCommSystem, self->chan, &pkt, pkt.size) != 0)
        return 1;
    void* buf = func_00264960_self(self);
    MEM_copy(buf, &pkt, pkt.size);
    self->slots[self->tail++ % 32] = buf;
    return 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264CB8);
#ifdef SKIP_ASM
extern "C" void func_00264BE8(void* self, void* msg, int size);

struct sTagMsg264CB8 {
    int type;          // 0x0
    int state;         // 0x4
    unsigned int id;   // 0x8
    int pad[2];
};

/**
 * Sends this player's lobby rank to the peer as setup message 0x10 (LobbyMan mStats.mRank in the first word).
 *
 * func_00263CF0 calls it right after creating the setup manager, passing LobbyMan's unk_08C in the second argument
 * register, which is ignored. func_002642B8() stores the peer's rank when the message arrives.
 */
extern "C" void func_00264CB8(void* self)
{
    sTagMsg264CB8 msg;
    msg.state = LobbyMan_mLobbyMan->mStats.mRank;
    msg.type = 16;
    msg.id = 0;
    func_00264BE8(self, &msg, sizeof(msg));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264CF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setPlayerCharID(void* self, int a1, int charID);
extern int D_00534E58[];

struct sVE264CF0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sIf264CF0 {
    char pad[0xC];
    sVE264CF0* vt;
};

/**
 * Applies this player's character choice: player 0's character in the player interface and MyCharacterID.
 *
 * Calls cBENewPlayerInterface_setPlayerCharID(0, @p charID) and the player interface's slot-1 virtual, stores @p charID
 * in the network options' MyCharacterID (D_00534E58) and calls the network interface's slot-1 virtual.
 *
 * @param charID  the chosen character
 */
extern "C" void func_00264CF0(void* self, int charID)
{
    sIf264CF0* playerInterface = (sIf264CF0*)cBE_getInterface_Fv(cBE_getBE(), 1);
    cBENewPlayerInterface_setPlayerCharID(playerInterface, 0, charID);
    playerInterface->vt[1].fn((char*)playerInterface + playerInterface->vt[1].delta);
    sIf264CF0* networkInterface = (sIf264CF0*)cBE_getInterface_Fv(SSXApp->BE, 7);
    D_00534E58[0] = charID;
    networkInterface->vt[1].fn((char*)networkInterface + networkInterface->vt[1].delta);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264D80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setPlayerCharID(void* self, int a1, int charID);

struct sVE_00264D80 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sIf_00264D80 {
    char pad[0xC];
    sVE_00264D80* vt;
};

struct sName7_00264D80 {
    char c[7];
};

struct sNetState_00264D80 {
    char pad_0x000[0x32C];
    int charID;                     // 0x32C
    char pad_0x330[0x4A8 - 0x330];
    sName7_00264D80 name;           // 0x4A8
};


/**
 * Applies the opponent's character choice: player 1's character, OpponentCharacterID and its attributes.
 *
 * Calls cBENewPlayerInterface_setPlayerCharID(1, @p charID) and the player interface's slot-1 virtual, stores @p charID
 * in cBENetworkInterface_networkOpt.OpponentCharacterID and, when @p name is given, copies its 7 bytes into
 * oppCharacterAttributes (+0x4A8), then calls the network interface's slot-1 virtual.
 *
 * @param charID  the opponent's character
 * @param name    the opponent's 7 attribute bytes from the setup message, or 0
 */
extern "C" void func_00264D80(void* self, int charID, sName7_00264D80* name)
{
    sIf_00264D80* playerIface = (sIf_00264D80*)cBE_getInterface_Fv(cBE_getBE(), 1);
    cBENewPlayerInterface_setPlayerCharID(playerIface, 1, charID);
    playerIface->vt[1].fn((char*)playerIface + playerIface->vt[1].delta);
    sIf_00264D80* networkIface = (sIf_00264D80*)cBE_getInterface_Fv(SSXApp->BE, 7);
    cBENetworkInterface_networkOpt.OpponentCharacterID = charID;
    if (name != 0)
        *(sName7_00264D80*)((char*)&cBENetworkInterface_networkOpt + 0x4A8) = *name;
    networkIface->vt[1].fn((char*)networkIface + networkIface->vt[1].delta);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264E50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00264BE8(void* self, void* msg, int size);
extern "C" int cBEStatInterface_getHumanAccel(void* iface, int a, int rider);
extern "C" int cBEStatInterface_getHumanEdging(void* iface, int a, int rider);
extern "C" int cBEStatInterface_getHumanSpeed(void* iface, int a, int rider);
extern "C" int cBEStatInterface_getHumanSpin(void* iface, int a, int rider);
extern "C" int cBEStatInterface_getHumanStability(void* iface, int a, int rider);
extern "C" int cBEStatInterface_getHumanToughness(void* iface, int a, int rider);
extern "C" int cBEStatInterface_getHumanTrick(void* iface, int a, int rider);

struct sTagMsg264E50 {
    int type;                   // 0x0
    int rider;                  // 0x4
    int state;                  // 0x8
    unsigned char stats[7];     // 0xC
};

/**
 * Sends a character-select message to the peer with the rider's 7 attribute values.
 *
 * Builds a 20-byte message {@p type, @p rider, @p state, 7 bytes}: on a ranked game (+0x128 set) every attribute is 40,
 * otherwise they come from interface 3's getters cBEStatInterface_getHumanAccel, cBEStatInterface_getHumanEdging, cBEStatInterface_getHumanSpeed, cBEStatInterface_getHumanSpin,
 * cBEStatInterface_getHumanStability, cBEStatInterface_getHumanToughness and cBEStatInterface_getHumanTrick (player 0, @p rider); then sends it with func_00264BE8().
 *
 * @param type   message type (func_00264F88, func_00265018 and func_00265140 send 0..5: pick, confirm and withdraw,
 *               host then client)
 * @param rider  the character id, or -1
 * @param state  the sender's setup state (0 for a client pick)
 */
extern "C" void func_00264E50(void* self, int type, int rider, int state)
{
    sTagMsg264E50 msg;
    msg.type = type;
    msg.state = state;
    msg.rider = rider;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 3);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (*(int*)((char*)self + 0x128) != 0) {
        int i;
        for (i = 0; i < 7; i++)
            msg.stats[i] = 40;
    } else {
        msg.stats[0] = cBEStatInterface_getHumanAccel(iface, 0, rider);
        msg.stats[1] = cBEStatInterface_getHumanEdging(iface, 0, rider);
        msg.stats[2] = cBEStatInterface_getHumanSpeed(iface, 0, rider);
        msg.stats[3] = cBEStatInterface_getHumanSpin(iface, 0, rider);
        msg.stats[4] = cBEStatInterface_getHumanStability(iface, 0, rider);
        msg.stats[5] = cBEStatInterface_getHumanToughness(iface, 0, rider);
        msg.stats[6] = cBEStatInterface_getHumanTrick(iface, 0, rider);
    }
    func_00264BE8(self, &msg, sizeof(msg));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00264F88);
#ifdef SKIP_ASM
extern "C" void func_00264CF0(void* self, int a1);
extern "C" void func_00264E50(void* self, int event, int a2, int state);

/**
 * Sends this player's highlighted character to the peer during online character select.
 *
 * Ignored in state 12. The host moves state 1 to 2, applies the pick (func_00264CF0()) and sends message 0; a client
 * only sends message 1. cFEStateOnlineCharacterSelect_onWidgetEvent calls it, and func_002642B8 when the peer's rank
 * arrives.
 *
 * @param rider  the character id
 */
extern "C" void func_00264F88(void* self, int rider)
{
    int state = *(int*)self;
    if (state == 0xC) {
        return;
    }
    if (*(int*)((char*)self + 0x124) != 0) {
        if (state == 1) {
            *(int*)self = 2;
        }
        func_00264CF0(self, rider);
        func_00264E50(self, 0, rider, *(int*)self);
    } else {
        func_00264E50(self, 1, rider, 0);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265018);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
extern "C" void func_00264CF0(void* self, int charID);
extern "C" void func_00264E50(void* self, int type, int rider, int state);

struct sMsgListNode5018 {
    sMsgListNode5018* next;
    sMsgListNode5018* prev;
    void* data;
};

struct sMsgListIter5018 {
    sMsgListNode5018* node;
    sMsgListIter5018(sMsgListNode5018* x) : node(x) {}
    sMsgListIter5018(const sMsgListIter5018& x) : node(x.node) {}
};

static inline sMsgListIter5018 sMsgList5018_insert(sMsgListIter5018 pos, void* const& x)
{
    sMsgListNode5018* tmp = (sMsgListNode5018*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Confirms this player's character pick and tells the peer.
 *
 * The host advances its state (1..2 to 4, 3 to 5), applies the pick (func_00264CF0()), sends message 2 with the new
 * state and posts an online::Event of type 244 to LobbyMan's mEventList; a client sends message 3 with its state.
 * cFEStateOnlineCharacterSelect_onWidgetEvent calls it.
 *
 * @param rider  the chosen character
 */
extern "C" void func_00265018(void* self, int rider)
{
    if (*(int*)((char*)self + 0x124) != 0) {
        int st = *(int*)self;
        if (st > 0) {
            if (st < 3)
                *(int*)self = 4;
            else if (st == 3)
                *(int*)self = 5;
        }
        func_00264CF0(self, rider);
        int type = 244;
        func_00264E50(self, 2, rider, *(int*)self);
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        netEvent[0] = type;
        *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
        void* mp = netEvent;
        sMsgList5018_insert((*(sMsgListNode5018**)((char*)lobbyMan + 0xF0)), mp);
    } else {
        func_00264E50(self, 3, rider, *(int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00265140);
#ifdef SKIP_ASM
extern "C" void func_00264E50(void* self, int event, int a2, int state);

/**
 * Withdraws this player's confirmed character pick ("back" in online character select).
 *
 * Ignored in state 12. The host returns state 4 to 2 and sends message 4; a client sends message 5 (rider -1, its
 * current state).
 */
extern "C" void func_00265140(int* self)
{
    int event;
    if (*self != 0xC) {
        if (*(int*)((char*)self + 0x124) != 0) {
            if (*self == 4) {
                *self = 2;
            }
            event = 4;
        } else {
            event = 5;
        }
        func_00264E50(self, event, -1, *self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265190);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
struct sName7_00264D80;
extern "C" void func_00264D80(void* self, int charID, sName7_00264D80* name);
extern "C" void func_00264E50(void* self, int type, int rider, int state);

struct sMsgListNode5190 {
    sMsgListNode5190* next;
    sMsgListNode5190* prev;
    void* data;
};

struct sMsgListIter5190 {
    sMsgListNode5190* node;
    sMsgListIter5190(sMsgListNode5190* x) : node(x) {}
    sMsgListIter5190(const sMsgListIter5190& x) : node(x.node) {}
};

static inline sMsgListIter5190 sMsgList5190_insert(sMsgListIter5190 pos, void* const& x)
{
    sMsgListNode5190* tmp = (sMsgListNode5190*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Handles the opponent's character choice on the host's setup manager: records it, echoes it back and posts an event.
 *
 * The setup manager (D_004A33CC, built by func_002640C0) moves its state word (+0x0) from 1 to 2, stores the opponent's
 * character and name in the back end (func_00264D80), sends a type-1 message with that character and the new state
 * (func_00264E50) and pushes an 8-byte online event of kind 243 (vtable online_Event_vtbl, tag "NetEvent") onto LobbyMan's
 * mEventList. func_002642B8 calls it for a type-1 message while the host is in a state below 7.
 *
 * @param rider  the opponent's character id from the message
 * @param name   the opponent's name block from the message, copied into the back end
 */
extern "C" void func_00265190(void* self, int rider, sName7_00264D80* name)
{
    int st = *(int*)self;
    if (st != 2) {
        if (st < 3) {
            if (st == 1)
                *(int*)self = 2;
        }
    }
    func_00264D80(self, rider, name);
    int type = 243;
    func_00264E50(self, 1, rider, *(int*)self);
    {
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        netEvent[0] = type;
        *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
        void* mp = netEvent;
        sMsgList5190_insert((*(sMsgListNode5190**)((char*)lobbyMan + 0xF0)), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265290);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];
struct sName7_00264D80;
extern "C" void func_00264D80(void* self, int charID, sName7_00264D80* name);
extern "C" void func_00264E50(void* self, int type, int rider, int state);

struct sMsgListNode5290 {
    sMsgListNode5290* next;
    sMsgListNode5290* prev;
    void* data;
};

struct sMsgListIter5290 {
    sMsgListNode5290* node;
    sMsgListIter5290(sMsgListNode5290* x) : node(x) {}
    sMsgListIter5290(const sMsgListIter5290& x) : node(x.node) {}
};

static inline sMsgListIter5290 sMsgList5290_insert(sMsgListIter5290 pos, void* const& x)
{
    sMsgListNode5290* tmp = (sMsgListNode5290*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

/**
 * Handles the opponent confirming a character on the host's setup manager.
 *
 * Moves the state word (+0x0) from 1 or 2 to 3, or from 4 to 5, stores the character in the back end (func_00264D80, no
 * name), sends a type-3 message with it and the new state (func_00264E50) and pushes an online event of kind 245 onto
 * LobbyMan's mEventList. func_002642B8 calls it for a type-3 message while the host is in a state below 7.
 *
 * @param rider  the opponent's character id from the message
 */
extern "C" void func_00265290(void* self, int rider)
{
    int st = *(int*)self;
    if (st > 0) {
        if (st < 3)
            *(int*)self = 3;
        else if (st == 4)
            *(int*)self = 5;
    }
    int type = 245;
    func_00264D80(self, rider, 0);
    func_00264E50(self, 3, rider, *(int*)self);
    {
        LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
        int* netEvent = (int*)operator new(8, str_NetEvent, 0x20000000, 0);
        netEvent[0] = type;
        *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
        void* mp = netEvent;
        sMsgList5290_insert((*(sMsgListNode5290**)((char*)lobbyMan + 0xF0)), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265398);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_NetEvent[];
extern char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_00265398 {
    sMsgListNode_00265398* next;
    sMsgListNode_00265398* prev;
    void* data;
};

struct sMsgListIter_00265398 {
    sMsgListNode_00265398* node;
    sMsgListIter_00265398(sMsgListNode_00265398* x) : node(x) {}
    sMsgListIter_00265398(const sMsgListIter_00265398& x) : node(x.node) {}
};

static inline sMsgListIter_00265398 sMsgList_00265398_insert(sMsgListIter_00265398 pos, void* const& x)
{
    sMsgListNode_00265398* tmp = (sMsgListNode_00265398*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00265398 {
    int kind;
    void** vt;
};

/**
 * Handles the opponent withdrawing a character confirmation on the host's setup manager.
 *
 * Moves the state word (+0x0) from 3 back to 2 and pushes an online event of kind 247 onto LobbyMan's mEventList; no
 * message is sent. func_002642B8 calls it for a type-5 message while the host is in a state below 7.
 */
extern "C" void func_00265398(int* self)
{
    if (*self == 3)
        *self = 2;
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    sMsg_00265398* netEvent = (sMsg_00265398*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    netEvent->vt = online_Event_vtbl;
    netEvent->kind = 247;
    void* mp = netEvent;
    sMsgList_00265398_insert((*(sMsgListNode_00265398**)((char*)lobbyMan + 0xF0)), mp);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265458);
#ifdef SKIP_ASM
extern char str_StlBlock_b[];
extern char str_cSetupMan_Update[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_00264BE8(void* self, void* msg, int size);
extern void* D_00481150[];

struct sMsgListNode5458 {
    sMsgListNode5458* next;
    sMsgListNode5458* prev;
    void* data;
};

struct sMsgListIter5458 {
    sMsgListNode5458* node;
    sMsgListIter5458(sMsgListNode5458* x) : node(x) {}
    sMsgListIter5458(const sMsgListIter5458& x) : node(x.node) {}
};

static inline sMsgListIter5458 sMsgList5458_insert(sMsgListIter5458 pos, void* const& x)
{
    sMsgListNode5458* tmp = (sMsgListNode5458*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sTagMsg5458 {
    int type;   // 0x0
    int id;     // 0x4
    int state;  // 0x8
    int pad[2];
};

struct sNetMsg5458 {
    int type;       // 0x0
    void** vt;      // 0x4
    int flag;       // 0x8
    int v0;         // 0xC
    int v1;         // 0x10
};

/**
 * Sends the row highlighted on the bragging-rights screen to the peer and posts it as a lobby event.
 *
 * Does nothing in state 12. On the host (+0x124 set, func_002640C0's isHost) the state word moves from 5 or 6 to 7 and
 * the message type is 6; a client sends type 7. The {type, id, state} message goes out through func_00264BE8, and a
 * 0x10-byte event of kind 251 (vtable D_00481150, tag "cSetupMan::Update") carrying @p id is pushed onto LobbyMan's
 * mEventList. cFEStateBraggingRights_onWidgetMenu calls it with the menu's current row.
 *
 * @param id  the highlighted row (the menu's byte at +0x95)
 */
extern "C" void func_00265458(void* self, int id)
{
    int st = *(int*)self;
    if (st != 12) {
        int type;
        if (*(int*)((char*)self + 0x124) != 0) {
            if (st >= 5) {
                if (st < 7)
                    *(int*)self = 7;
            }
            type = 6;
        } else
            type = 7;
        sTagMsg5458 msg;
        msg.type = type;
        msg.state = *(int*)self;
        msg.id = id;
        func_00264BE8(self, &msg, sizeof(msg));
        sNetMsg5458* updateEvent = (sNetMsg5458*)cMemMan_alloc(0x10, str_cSetupMan_Update, 0x20000000, 0);
        updateEvent->type = 251;
        updateEvent->vt = D_00481150;
        updateEvent->v0 = id;
        updateEvent->flag = 0;
        void* mp = updateEvent;
        sMsgList5458_insert((*(sMsgListNode5458**)((char*)LobbyMan_mLobbyMan + 0xF0)), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265568);
#ifdef SKIP_ASM
extern char str_StlBlock_b[];
extern char str_cSetupMan_Update[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_00264BE8(void* self, void* msg, int size);
extern void* D_00481130[];

struct sMsgListNode5568 {
    sMsgListNode5568* next;
    sMsgListNode5568* prev;
    void* data;
};

struct sMsgListIter5568 {
    sMsgListNode5568* node;
    sMsgListIter5568(sMsgListNode5568* x) : node(x) {}
    sMsgListIter5568(const sMsgListIter5568& x) : node(x.node) {}
};

static inline sMsgListIter5568 sMsgList5568_insert(sMsgListIter5568 pos, void* const& x)
{
    sMsgListNode5568* tmp = (sMsgListNode5568*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sTagMsg5568 {
    int type;   // 0x0
    int a;      // 0x4
    int b;      // 0x8
    int pad[2];
};

struct sNetMsg5568 {
    int type;       // 0x0
    void** vt;      // 0x4
    int flag;       // 0x8
    int v0;         // 0xC
    int v1;         // 0x10
};

/**
 * Sends a changed bragging-rights rule value to the peer and posts it as a lobby event.
 *
 * Does nothing in state 12. On the host (+0x124 set) the state word moves from 5 or 6 to 7 and the message type is 8; a
 * client sends type 9. The {type, a, b} message goes out through func_00264BE8, and a 0x14-byte event of kind 252
 * (vtable D_00481130, tag "cSetupMan::Update") carrying both values is pushed onto LobbyMan's mEventList.
 * cFEStateBraggingRights_onWidgetEvent calls it with the rule slot and the list box's new value.
 *
 * @param ruleSlot  the rule slot
 * @param newValue  the new value
 */
extern "C" void func_00265568(void* self, int ruleSlot, int newValue)
{
    int st = *(int*)self;
    if (st != 12) {
        int type;
        if (*(int*)((char*)self + 0x124) != 0) {
            if (st >= 5) {
                if (st < 7)
                    *(int*)self = 7;
            }
            type = 8;
        } else
            type = 9;
        sTagMsg5568 msg;
        msg.type = type;
        msg.a = ruleSlot;
        msg.b = newValue;
        func_00264BE8(self, &msg, sizeof(msg));
        sNetMsg5568* updateEvent = (sNetMsg5568*)cMemMan_alloc(20, str_cSetupMan_Update, 0x20000000, 0);
        updateEvent->type = 0xFC;
        updateEvent->vt = D_00481130;
        updateEvent->v0 = ruleSlot;
        updateEvent->v1 = newValue;
        updateEvent->flag = 0;
        void* mp = updateEvent;
        sMsgList5568_insert((*(sMsgListNode5568**)((char*)LobbyMan_mLobbyMan + 0xF0)), mp);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265688);
#ifdef SKIP_ASM
// PORT: really returns int (the unit declares it void); the int return keeps the target's store scheduling.
extern "C" int func_00264BE8_5688(void* self, void* msg, int size) __asm__("func_00264BE8");
struct sMsg_5688 {
    int code;
    int state;
    unsigned int arg;
    int pad[2];
};

/**
 * Tells the peer that the local bragging-rights panel was locked in.
 *
 * On the host (+0x124 set) the message code is 10 and the state word moves from 5..7 to 9, or from 8 to 10; a client
 * sends code 11 and keeps its state. The {code, state, -1} message goes out through func_00264BE8.
 * cFEStateBraggingRights_onWidgetEvent calls it when panel 0 is locked online.
 */
// Advances the connection state (5..7 -> 9, 8 -> 10) and posts a status message.
extern "C" void func_00265688(int* self)
{
    sMsg_5688 statusMsg;
    int code = 11;
    if (self[292 / 4] != 0) {
        code = 10;
        int st = self[0];
        if (st >= 5) {
            int nst;
            if (st < 8)
                nst = 9;
            else if (st == 8)
                nst = 10;
            else
                goto send;
            self[0] = nst;
            code = 10;
        }
    }
send:
    statusMsg.code = code;
    statusMsg.state = self[0];
    statusMsg.arg = 0xFFFFFFFF;
    func_00264BE8_5688(self, &statusMsg, 20);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265700);
#ifdef SKIP_ASM
struct sTagMsg265700 {
    int type;          // 0x0
    int state;         // 0x4
    unsigned int id;   // 0x8
    int pad[2];
};

extern "C" void func_00264BE8(void* self, void* msg, int size);

/**
 * Tells the peer that the local bragging-rights panel was unlocked again.
 *
 * Does nothing in state 12. On the host (+0x124 set) the state word moves from 9 back to 7 and the message type is 12;
 * a client sends type 13. The {type, state, -1} message goes out through func_00264BE8. cFEStateBraggingRights_onUpdate
 * calls it when Back unlocks a locked panel online.
 */
extern "C" void func_00265700(void* self)
{
    sTagMsg265700 msg;
    int type;
    int state = *(int*)self;
    if (state == 12)
        return;
    if (*(int*)((char*)self + 0x124) != 0) {
        if (state == 9)
            *(int*)self = 7;
        type = 12;
    } else {
        type = 13;
    }
    msg.type = type;
    msg.state = *(int*)self;
    msg.id = 0xFFFFFFFF;
    func_00264BE8(self, &msg, sizeof(msg));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265768);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sTagMsg5768 {
    int type;           // 0x0
    unsigned int state; // 0x4
    int id;             // 0x8
    int pad[2];
};
/**
 * Cancels the online race setup: tells the peer, leaves the lobby game and sets the state word to 11.
 *
 * Does nothing in state 10. Event 249 is sent to the peer as code 0xF8 and also pushes an online event of kind 268 onto
 * LobbyMan's mEventList; any other @p ev is sent unchanged. The {code, -1, 0} message goes out through func_00264BE8,
 * then func_0025F740(LobbyMan_mLobbyMan, @p ev) leaves the game and posts @p ev. func_002642B8 treats codes 0xF8 and
 * 250 from the peer as the same cancel. The bragging-rights screen (func_00194440) and func_001B1F18 call it with 249.
 *
 * @param ev  the cancel reason (249 when the player cancels)
 */
extern "C" void func_00265768(int* self, int ev)
{
    if (self[0] != 10) {
        int code = ev;
        if (ev == 249) {
            code = 0xF8;
            postMsg((char*)LobbyMan_mLobbyMan, 268);
        }
        sTagMsg5768 msg;
        msg.type = code;
        msg.state = 0xFFFFFFFF;
        msg.id = 0;
        func_00264BE8(self, &msg, sizeof(msg));
        func_0025F740(LobbyMan_mLobbyMan, ev);
        self[0] = 11;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265880);
#ifdef SKIP_ASM
extern char str_cSharedLoginMan[];
extern void* D_004A33D0;
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002658E0(void* self);

/**
 * Creates the shared-login manager singleton D_004A33D0.
 *
 * Allocates 8 bytes tagged "cSharedLoginMan" and runs the constructor func_002658E0 on them.
 * cFEStateNetAccountMem_cFEStateNetAccountMem calls it.
 */
extern "C" void func_00265880(void)
{
    D_004A33D0 = func_002658E0(cMemMan_alloc(8, str_cSharedLoginMan, 0x20000000, 0));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002658B8);
#ifdef SKIP_ASM
extern void* D_004A33D0;
extern "C" void func_00265908(void* self, int flags);

/**
 * Destroys the shared-login manager singleton D_004A33D0, if there is one, and clears the pointer.
 *
 * Calls the deleting destructor func_00265908 with flags 3. cFEStateNetAccountMem__cFEStateNetAccountMem calls it.
 */
extern "C" void func_002658B8(void)
{
    if (D_004A33D0 != 0) {
        func_00265908(D_004A33D0, 3);
        D_004A33D0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002658E0);
#ifdef SKIP_ASM
extern "C" void func_00265950(void* self);

/**
 * Constructs a shared-login manager by running func_00265950 on it.
 *
 * @return @p self
 */
extern "C" void* func_002658E0(void* self)
{
    func_00265950(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265908);
#ifdef SKIP_ASM
extern "C" void func_002659C0(void* self);
void operator_delete(int*);

/**
 * Destroys a shared-login manager: runs func_002659C0, then frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_00265908(void* self, int flags)
{
    func_002659C0(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265950);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void* D_004A33D8;
extern int D_004A33D4;
extern int D_004A33DC;
extern int D_004A33E0;
extern int D_004A33E4;
struct sDsInit265950 { void* buf; int len; };
extern sDsInit265950 D_004A5B48_s __asm__("D_004A5B48");
extern char D_00442A18[];
extern char D_00537190[];
extern "C" void* MEM_fill(void* dst, int value, int size);
extern "C" void func_003F7EE0(void* p);
extern "C" void* cMemCard_CreateInstance();

/**
 * Initializes the shared-login manager and the network-account library it serves.
 *
 * Sets the state word (+0x0) to 1 and the result word (+0x4) to 0, clears the card globals D_004A33D4 (current card
 * operation), D_004A33DC (port), D_004A33E0 (card-checked flag) and D_004A33E4 (retry count), hands the 17-entry
 * memory-card callback table D_00442A18 (func_00265FB8 .. func_00266550) to the library through func_003F7EE0
 * (D_004A5B48 = {table, 0}), clears the 0x40-byte name buffer D_00537190 and creates the memory-card object D_004A33D8
 * with cMemCard_CreateInstance (a cPS2MemCard).
 *
 * @param vself  the shared-login manager
 */
// Resets the DirtySock session state and creates the session object.
extern "C" void func_00265950(void* vself)
{
    int* self = (int*)vself;
    self[0] = 1;
    D_004A33D4 = 0;
    self[1] = 0;
    D_004A5B48_s.buf = D_00442A18;
    D_004A33E0 = 0;
    D_004A33DC = 0;
    D_004A33E4 = 0;
    D_004A5B48_s.len = 0;
    func_003F7EE0(&D_004A5B48_s);
    MEM_fill(D_00537190, 0, 0x40);
    D_004A33D8 = cMemCard_CreateInstance();
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002659C0);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern "C" void func_003F7F18();
extern "C" void cMemCard_DeleteInstance(void* h);

/**
 * Shuts the shared-login manager down: clears its state word, closes the network-account library and frees the card.
 *
 * Calls func_003F7F18 (the library shutdown) and cMemCard_DeleteInstance(D_004A33D8).
 */
extern "C" void func_002659C0(void* self)
{
    *(int*)self = 0;
    func_003F7F18();
    cMemCard_DeleteInstance(D_004A33D8);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002659E8);
#ifdef SKIP_ASM
struct cDsObj2659E8 {
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int a);
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual int v11();
};

extern void* D_004A33D8;
extern int D_004A33DC;

/**
 * Restarts the card check on the account's port once the memory card is idle.
 *
 * When the card D_004A33D8 is not busy (virtual slot 11, cPS2MemCard_IsBusy) it calls slot 5 (cPS2MemCard_CheckDevice)
 * with the port D_004A33DC that func_00266060 recorded. cFEStateNetAccountMem_onUpdate calls it in its popup-wait
 * states 7, 13, 21 and 25.
 */
extern "C" void func_002659E8(void* self)
{
    if (((cDsObj2659E8*)D_004A33D8)->v11() == 0) {
        ((cDsObj2659E8*)D_004A33D8)->v05(D_004A33DC);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265A38);
#ifdef SKIP_ASM
struct cDsObj265A38 {
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
    virtual int v20();
};

extern void* D_004A33D8;

/**
 * Returns the free space on the memory card, through the card's virtual slot 20 (cPS2MemCard_GetFreeSpace).
 *
 * func_001B70D0 needs at least 17 before it starts saving a new account (func_00265E10).
 *
 * @return the free space in KB
 */
extern "C" int func_00265A38(void* self)
{
    return ((cDsObj265A38*)D_004A33D8)->v20();
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265A68);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A3940;
extern "C" void func_00265D98(void* state);
extern "C" void func_00265E40(void* state);
extern "C" void func_00265E80(void* state);
extern "C" void func_00265EC8(void* state);
extern "C" void func_00265F00(void* state);
extern "C" int func_003F80E0(int* status);

struct cDsObj265A68 {
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int a);
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual int v11();
};

/**
 * Updates the shared-login manager's card state machine one step.
 *
 * First runs the card's Update (virtual slot 3 of D_004A33D8). Then by the state word: 2, 6, 10 and 14 wait for the
 * card to be idle (slot 11, cPS2MemCard_IsBusy) and start their account operation (func_00265D98, func_00265E40,
 * func_00265E80, func_00265EC8); 3, 7, 11 and 15 poll it with func_003F80E0, store the error in the result word (+0x4)
 * and step to the next state when it finished (status 2) or two states on when it failed; 4, 8, 12, 16 and 21 restart
 * the card check on port 0 (slot 5) whenever the card is idle; 18 waits idle and starts the format setup
 * (func_00265F00); 19 formats the card (slot 8, cPS2MemCard_Format) and goes to 20; 20 waits idle and goes to 22 when
 * the last libmc result D_004A3940 is non-zero, else 21. cFEStateNetAccountMem_onUpdate calls it every frame.
 *
 * @param state  the shared-login manager (its state word is the first field)
 */
// Advances the DirtySock login state machine one step; each "wait" state checks that the socket is idle.
extern "C" void func_00265A68(int* state)
{
    ((cDsObj265A68*)D_004A33D8)->v03();
    int status = 0;
    switch (*state) {
    case 0:
        break;
    case 2:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            func_00265D98(state);
        }
        break;
    case 6:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            func_00265E40(state);
        }
        break;
    case 10:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            func_00265E80(state);
        }
        break;
    case 14:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            func_00265EC8(state);
        }
        break;
    case 18:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            func_00265F00(state);
        }
        break;
    case 3:
    case 7:
    case 11:
    case 15: {
        int pollError = func_003F80E0(&status);
        state[1] = pollError;
        if (pollError == 0) {
            if (status == 2) {
                *state += 1;
            }
        } else {
            *state += 2;
        }
        break;
    }
    case 4:
    case 8:
    case 12:
    case 16:
    case 21:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            ((cDsObj265A68*)D_004A33D8)->v05(0);
        }
        break;
    case 19:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            ((cDsObj265A68*)D_004A33D8)->v08();
            *state = 20;
        }
        break;
    case 20:
        if (!((cDsObj265A68*)D_004A33D8)->v11()) {
            if (D_004A3940 != 0) {
                *state = 22;
            } else {
                *state = 21;
            }
        }
        break;
    case 22:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265CB0);
#ifdef SKIP_ASM
struct cDsObj265CB0 {
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
    virtual bool v53();
    virtual void v54();
    virtual bool v55();
};

extern void* D_004A33D8;

/**
 * Tells whether the memory card is present but unformatted.
 *
 * Uses the card's virtual slots 53 (func_002D3810, cPS2MemCard_IsAvailable for the current port) and 55 (func_002D38F0,
 * cPS2MemCard_IsFormatted for it).
 *
 * @return 1 when the card is available and not formatted, otherwise 0
 */
extern "C" int func_00265CB0(void* self)
{
    if (((cDsObj265CB0*)D_004A33D8)->v53() != 0) {
        return !((cDsObj265CB0*)D_004A33D8)->v55();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265D08);
#ifdef SKIP_ASM
struct cDsObj265D08 {
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
};

extern void* D_004A33D8;

/**
 * Clears the memory card's changed flag for the current port, through the card's virtual slot 47 (func_002C44E0).
 *
 * That slot also turns a last result of -1 into 0. func_001B70D0 calls it before starting most card steps.
 */
extern "C" void func_00265D08(void* self)
{
    ((cDsObj265D08*)D_004A33D8)->v47();
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265D38);
#ifdef SKIP_ASM
struct cDsObj265D38 {
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
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual int v63();
};

extern void* D_004A33D8;

/**
 * Tells whether the memory card in the current port was changed, through the card's virtual slot 63 (func_002D3930,
 * cPS2MemCard_CardChanged).
 *
 * @return non-zero when the card was changed
 */
extern "C" int func_00265D38(void* self)
{
    return ((cDsObj265D38*)D_004A33D8)->v63();
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265D68);
#ifdef SKIP_ASM
struct cDsObj265D68 {
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
    virtual bool v53();
};

extern void* D_004A33D8;

/**
 * Tells whether there is no usable memory card, by negating the card's virtual slot 53 (func_002D3810,
 * cPS2MemCard_IsAvailable for the current port).
 *
 * @return 1 when the card is not available, otherwise 0
 */
extern "C" int func_00265D68(void* self)
{
    return !((cDsObj265D68*)D_004A33D8)->v53();
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265D98);
#ifdef SKIP_ASM
extern char D_00536760[0x11];
extern int D_004A33E0;
extern "C" void* MEM_fill(void* dst, int value, int size);
extern "C" int func_003F8000(void* p);

/**
 * Starts reading the saved account from the card: the shared-login manager's step for state 2.
 *
 * Clears the result word (+0x4) and D_004A33E0, zeroes the 36-byte account buffer D_00536760 (user name at +0, password
 * at +0x11), starts func_003F8000(D_00536760) and sets the state word to 3. func_001B7A98 later compares the buffer
 * with the typed user name and password.
 */
extern "C" void func_00265D98(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    D_004A33E0 = 0;
    MEM_fill(D_00536760, 0, 36);
    func_003F8000(D_00536760);
    *(int*)self = 3;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265DF0__FPv);
#ifdef SKIP_ASM
/**
 * Requests a read of the saved account: sets the shared-login manager's state word to 2.
 *
 * func_001B70D0 uses it for its "checking card" steps; func_00265A68 then runs func_00265D98.
 */
void func_00265DF0(void* self)
{
    *(int*)self = 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E00__FPv);
#ifdef SKIP_ASM
/**
 * Requests a save of the account: sets the shared-login manager's state word to 6.
 *
 * func_00265A68 then runs func_00265E40.
 */
void func_00265E00(void* self)
{
    *(int*)self = 6;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E10__FPv);
#ifdef SKIP_ASM
/**
 * Requests creating the account save: sets the shared-login manager's state word to 10.
 *
 * func_001B70D0 uses it once the card has at least 17 KB free; func_00265A68 then runs func_00265E80.
 */
void func_00265E10(void* self)
{
    *(int*)self = 10;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E20__FPv);
#ifdef SKIP_ASM
/**
 * Requests the account operation run by func_00265EC8: sets the shared-login manager's state word to 14.
 *
 * func_001B70D0 uses it in its delete-account step ("kT_MEMDeletingPS2") and in step 26.
 */
void func_00265E20(void* self)
{
    *(int*)self = 14;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E30__FPv);
#ifdef SKIP_ASM
/**
 * Requests a card format: sets the shared-login manager's state word to 18.
 *
 * func_001B70D0 uses it in its "formatting" step; func_00265A68 then formats the card.
 */
void func_00265E30(void* self)
{
    *(int*)self = 18;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E40);
#ifdef SKIP_ASM
extern char D_00536760[0x11];
extern int D_004A33E0;
extern "C" int func_003F8048(void* p);

/**
 * Starts saving the account to the card: the shared-login manager's step for state 6.
 *
 * Clears the result word (+0x4) and D_004A33E0, starts func_003F8048(D_00536760) (which copies the user name and
 * password from the buffer into the library session) and sets the state word to 7.
 */
extern "C" void func_00265E40(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    D_004A33E0 = 0;
    func_003F8048(D_00536760);
    *(int*)self = 7;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E80);
#ifdef SKIP_ASM
extern char D_00536760[0x11];
extern char D_00442A60[];
extern int D_004A33E0;
extern "C" int func_003F7F48(void* a, void* b);

/**
 * Starts creating the account save: the shared-login manager's step for state 10.
 *
 * Clears the result word (+0x4) and D_004A33E0, starts func_003F7F48(D_00442A60, D_00536760) (which also copies the
 * user name and password from the buffer) and sets the state word to 11.
 */
extern "C" void func_00265E80(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    D_004A33E0 = 0;
    func_003F7F48(D_00442A60, D_00536760);
    *(int*)self = 11;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265EC8);
#ifdef SKIP_ASM
extern int D_004A33E0;
extern "C" int func_003F80C0();

/**
 * Starts the network-account library's operation 3 (func_003F80C0): the shared-login manager's step for state 14.
 *
 * Clears the result word (+0x4) and D_004A33E0 and sets the state word to 15.
 */
extern "C" void func_00265EC8(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    D_004A33E0 = 0;
    func_003F80C0();
    *(int*)self = 0xF;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265F00);
#ifdef SKIP_ASM
extern int D_004A33E0;

/**
 * Prepares a card format: the shared-login manager's step for state 18.
 *
 * Clears the result word (+0x4) and D_004A33E0 and sets the state word to 19, where func_00265A68 formats the card.
 */
extern "C" void func_00265F00(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    D_004A33E0 = 0;
    *(int*)self = 19;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265F18);
#ifdef SKIP_ASM
extern char D_00536760[0x11];
extern "C" void* MEM_fill(void* dst, int value, int size);
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Copies a user name into the account buffer D_00536760 (17 bytes, cleared first; no length check on the copy).
 *
 * func_001B7778 passes the account screen's mszUserName.
 *
 * @param name  the user name
 */
extern "C" void func_00265F18(const char* name)
{
    MEM_fill(D_00536760, 0, 17);
    strcpy(D_00536760, name);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265F68);
#ifdef SKIP_ASM
extern char D_00536771[0x11];
extern "C" void* MEM_fill(void* dst, int value, int size);
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Copies a password into the account buffer's password field D_00536771 (17 bytes, cleared first; no length check).
 *
 * func_001B7778 passes the account screen's mszPassword.
 *
 * @param name  the password
 */
extern "C" void func_00265F68(const char* name)
{
    MEM_fill(D_00536771, 0, 17);
    strcpy(D_00536771, name);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265FB8);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
extern char D_00537190[];
extern "C" void* MEM_fill(void* dst, int value, int size);
extern "C" int func_003FA1D0(int code, void* out);
extern "C" void func_002C6810(void* obj, void* a1);

struct sDsErr265FB8 {
    char a;
    char b;
};

/**
 * Memory-card callback 0 of the network-account library: starts a directory search on the card.
 *
 * Decodes @p code into a port/slot pair (func_003FA1D0; the result is not used), clears the 0x40-byte name buffer
 * D_00537190 and starts a sceMcGetDir with @p pattern as the pattern (func_002C6810 on the card D_004A33D8). Sets the card
 * operation D_004A33D4 to 2; func_00266358 then looks for a matching entry. Entry 0 of D_00442A18.
 *
 * @param pattern   the directory pattern
 * @param code  the library's card unit code
 */
extern "C" void func_00265FB8(void* pattern, int code)
{
    sDsErr265FB8 err;
    func_003FA1D0(code, &err);
    MEM_fill(D_00537190, 0, 0x40);
    func_002C6810(D_004A33D8, pattern);
    D_004A33D4 = 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266010);
#ifdef SKIP_ASM
extern int D_004A33D4;
extern "C" int func_003FA1D0(int code, void* out);

struct sDsErr266010 {
    char a;
    char b;
};

/**
 * Memory-card callback 1 of the network-account library: asks for the card's free space.
 *
 * Decodes @p code (func_003FA1D0) and sets the card operation D_004A33D4 to 9; func_00266358 then reports the free
 * space in bytes. Entry 1 of D_00442A18.
 *
 * @param code  the library's card unit code
 */
extern "C" void func_00266010(int code)
{
    sDsErr266010 err;
    func_003FA1D0(code, &err);
    D_004A33D4 = 9;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266038);
#ifdef SKIP_ASM
extern int D_004A33D4;
extern "C" int func_003FA1D0(int code, void* out);

struct sDsErr266038 {
    char a;
    char b;
};

/**
 * Memory-card callback 2 of the network-account library: sets up a query that func_00266358 answers with 0x800.
 *
 * Decodes @p code (func_003FA1D0) and sets the card operation D_004A33D4 to 10. Entry 2 of D_00442A18.
 *
 * @param code  the library's card unit code
 */
extern "C" void func_00266038(int code)
{
    sDsErr266038 err;
    func_003FA1D0(code, &err);
    D_004A33D4 = 10;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266060);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
extern int D_004A33DC;
extern int D_004A33E4;
extern "C" int func_003FA1D0(int code, void* out);
void func_002C67E8(void* self, int a1);

struct sDsErr266060 {
    unsigned char a;
    unsigned char b;
};

/**
 * Memory-card callback 3 of the network-account library: starts checking the card in a port.
 *
 * Decodes @p code into a port/slot pair (func_003FA1D0), stores the port in D_004A33DC, sets the retry count D_004A33E4
 * to 9, starts a card check on that port (func_002C67E8: sceMcGetInfo) and sets the card operation D_004A33D4 to 1.
 * func_00266358 repeats the check until the retries run out. Entry 3 of D_00442A18.
 *
 * @param code  the library's card unit code (port in the high bits, slot in the low two)
 */
extern "C" void func_00266060(int code)
{
    sDsErr266060 err;
    func_003FA1D0(code, &err);
    D_004A33DC = err.a;
    D_004A33E4 = 9;
    func_002C67E8(D_004A33D8, D_004A33DC);
    D_004A33D4 = 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002660A0);
#ifdef SKIP_ASM
extern int D_004A33D4;
extern "C" int func_003FA1D0(int code, void* out);

struct sDsErr2660A0 {
    char a;
    char b;
};

/**
 * Memory-card callback 4 of the network-account library: does no card work and reports success.
 *
 * Decodes @p code (func_003FA1D0) and sets the card operation D_004A33D4 to 3, which func_00266358 reports as done at
 * once. Entry 4 of D_00442A18.
 *
 * @param code  the library's card unit code
 */
extern "C" void func_002660A0(int code)
{
    sDsErr2660A0 err;
    func_003FA1D0(code, &err);
    D_004A33D4 = 0x3;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002660C8);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
extern char str_pct_s_pct_s_g[];
extern "C" int func_003FA1D0(int code, void* out);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_002C6968(void* obj, char* str, int a2);

struct sDsErr2660C8 {
    unsigned char a;
    unsigned char b;
};

/**
 * Memory-card callback 5 of the network-account library: opens a file on the card.
 *
 * Decodes @p code (func_003FA1D0), joins @p dirName and @p fileName with "%s/%s" into an 80-byte stack buffer (no length check),
 * starts a sceMcOpen of it with mode @p openMode (func_002C6968) and sets the card operation D_004A33D4 to 4. Entry 5 of
 * D_00442A18.
 *
 * @param dirName    the directory name
 * @param fileName    the file name
 * @param code  the library's card unit code
 * @param openMode    the open mode passed to sceMcOpen
 */
extern "C" void func_002660C8(const char* dirName, const char* fileName, int code, int openMode)
{
    sDsErr2660C8 err;
    char buf[80];
    func_003FA1D0(code, &err);
    sprintf(buf, str_pct_s_pct_s_g, dirName, fileName);
    func_002C6968(D_004A33D8, buf, openMode);
    D_004A33D4 = 4;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266148);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
void func_002C6A08(void* obj);

/**
 * Memory-card callback 6 of the network-account library: closes the open card file (func_002C6A08, sceMcClose).
 *
 * Sets the card operation D_004A33D4 to 8. Entry 6 of D_00442A18.
 */
extern "C" void func_00266148(void)
{
    func_002C6A08(D_004A33D8);
    D_004A33D4 = 8;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266170);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
extern "C" int func_003FA1D0(int code, void* out);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void func_002C6C50(void* obj, char* str, int a2);

struct sDsErr266170 {
    unsigned char a;
    unsigned char b;
};

/**
 * Memory-card callback 7 of the network-account library: creates a directory on the card.
 *
 * Decodes @p code (func_003FA1D0), copies @p name into an 80-byte stack buffer (no length check), starts a sceMcMkdir
 * of it (func_002C6C50, which also stores @p requestValue in the card's unk_020) and sets the card operation D_004A33D4 to 11.
 * Entry 7 of D_00442A18.
 *
 * @param name  the directory name
 * @param requestValue    value stored with the request (the card's unk_020)
 * @param code  the library's card unit code
 */
extern "C" void func_00266170(const char* name, int requestValue, int code)
{
    sDsErr266170 err;
    char buf[80];
    func_003FA1D0(code, &err);
    strcpy(buf, name);
    func_002C6C50(D_004A33D8, buf, requestValue);
    D_004A33D4 = 11;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002661D8);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
extern char str_pct_s_pct_s_g[];
extern "C" int func_003FA1D0(int code, void* out);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_002C6CE0(void* obj, char* str);

struct sDsErr2661D8 {
    unsigned char a;
    unsigned char b;
};

/**
 * Memory-card callback 8 of the network-account library: deletes a file on the card.
 *
 * Decodes @p code (func_003FA1D0), joins @p dirName and @p fileName with "%s/%s" into an 80-byte stack buffer (no length check),
 * starts a sceMcDelete of it (func_002C6CE0) and sets the card operation D_004A33D4 to 12. Entry 8 of D_00442A18.
 *
 * @param dirName    the directory name
 * @param fileName    the file name
 * @param code  the library's card unit code
 */
extern "C" void func_002661D8(const char* dirName, const char* fileName, int code)
{
    sDsErr2661D8 err;
    char buf[80];
    func_003FA1D0(code, &err);
    sprintf(buf, str_pct_s_pct_s_g, dirName, fileName);
    func_002C6CE0(D_004A33D8, buf);
    D_004A33D4 = 0xC;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266240);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
extern "C" int func_003FA1D0(int code, void* out);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void func_002C6CE0(void* obj, char* str);

struct sDsErr266240 {
    unsigned char a;
    unsigned char b;
};

/**
 * Memory-card callback 9 of the network-account library: deletes a directory on the card.
 *
 * Decodes @p code (func_003FA1D0), copies @p name into an 80-byte stack buffer (no length check), starts a sceMcDelete
 * of it (func_002C6CE0) and sets the card operation D_004A33D4 to 13. Entry 9 of D_00442A18.
 *
 * @param name  the directory name
 * @param code  the library's card unit code
 */
extern "C" void func_00266240(const char* name, int code)
{
    sDsErr266240 err;
    char buf[80];
    func_003FA1D0(code, &err);
    strcpy(buf, name);
    func_002C6CE0(D_004A33D8, buf);
    D_004A33D4 = 13;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266298);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
void func_002C6B50(void* obj, int a1, int a2);

/**
 * Memory-card callback 10 of the network-account library: reads from the open card file.
 *
 * Starts a sceMcRead into @p dest of @p byteCount bytes (func_002C6B50) and sets the card operation D_004A33D4 to 6;
 * func_00266358 reports the libmc result (bytes read). Entry 10 of D_00442A18.
 *
 * @param fileHandle  not read (the library's file handle)
 * @param dest  the destination buffer
 * @param byteCount  the number of bytes to read
 */
extern "C" void func_00266298(int fileHandle, int dest, int byteCount)
{
    func_002C6B50(D_004A33D8, dest, byteCount);
    D_004A33D4 = 6;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002662C0);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
void func_002C6BD0(void* obj, int a1, int a2);

/**
 * Memory-card callback 11 of the network-account library: writes to the open card file.
 *
 * Starts a sceMcWrite from @p source of @p byteCount bytes (func_002C6BD0) and sets the card operation D_004A33D4 to 14;
 * func_00266358 reports the libmc result (bytes written). Entry 11 of D_00442A18.
 *
 * @param fileHandle  not read (the library's file handle)
 * @param source  the source buffer
 * @param byteCount  the number of bytes to write
 */
extern "C" void func_002662C0(int fileHandle, int source, int byteCount)
{
    func_002C6BD0(D_004A33D8, source, byteCount);
    D_004A33D4 = 0xE;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002662E8);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
void func_002C6A60(void* obj, int a1, int a2);

/**
 * Memory-card callback 12 of the network-account library: seeks in the open card file.
 *
 * Starts a sceMcSeek to @p offset (func_002C6A60) and sets the card operation D_004A33D4 to 5. The origin passed on is 1
 * when @p origin is 1 and 0 otherwise. Quirk: an origin of 2 (from the end) becomes 0 (from the start). Entry 12 of
 * D_00442A18.
 *
 * @param fileHandle  not read (the library's file handle)
 * @param offset  the seek offset
 * @param origin  the seek origin (0 start, 1 current)
 */
extern "C" void func_002662E8(int fileHandle, int offset, int origin)
{
    int mcOrigin = 0;
    if (origin != 0) {
        mcOrigin = (origin == 1);
    }
    func_002C6A60(D_004A33D8, offset, mcOrigin);
    D_004A33D4 = 5;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266320);
#ifdef SKIP_ASM
extern void* D_004A33D8;
extern int D_004A33D4;
void func_002C6AE0(void* obj);

/**
 * Memory-card callback 13 of the network-account library: flushes the open card file (func_002C6AE0, sceMcFlush).
 *
 * Sets the card operation D_004A33D4 to 7. Entry 13 of D_00442A18.
 */
extern "C" void func_00266320(void)
{
    func_002C6AE0(D_004A33D8);
    D_004A33D4 = 7;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266348);
#ifdef SKIP_ASM
extern int D_004A33D4;

/**
 * Memory-card callback 14 of the network-account library: does no card work, setting the card operation D_004A33D4 to
 * 15, which func_00266358 reports as done at once.
 */
extern "C" void func_00266348(void)
{
    D_004A33D4 = 0xF;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266358);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_00266578(int err);
extern int D_004A3940;
extern "C" int cPS2MemCard_GetLastError(void* self, int id);
void func_002C67E8(void* self, int a1);
extern "C" void func_002C68C8(void* self, int i, char* out);
extern "C" int func_003FA360(char* name);

struct cDsObj266358 {
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
    virtual int v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual int v20();
};

/**
 * Memory-card callback 15 of the network-account library: polls the card operation started by the other callbacks.
 *
 * Operations 3, 15 and 16 report done (status 2) at once, operation 10 reports done with 0x800. Otherwise, while the
 * card D_004A33D8 is busy (slot 11) the status is 1; when it is idle the status is 2, and a card error (cPS2MemCard_GetLastError
 * for the current port) is returned through func_00266578. Per operation D_004A33D4: 1 re-runs the card check while the
 * retry count D_004A33E4 lasts (status stays 1), then sets D_004A33E0; 2 copies each directory entry (mNumDirTable at
 * +0x54, func_002C68C8) into D_00537190 and reports 1 for the first one func_003FA360 accepts; 5, 6 and 14 report the
 * last libmc result D_004A3940; 9 reports the free space (slot 20) times 1024.
 *
 * @param status  receives 1 while busy, 2 when finished
 * @param out     receives the operation's result (0 when there is none)
 * @return 0, or the library error func_00266578() maps a card error to
 */
// Polls the current DirtySock operation: *status is 1 while busy, 2 when finished; *out receives the result.
extern "C" int func_00266358(int* status, int* out)
{
    *out = 0;
    switch (D_004A33D4) {
    case 3:
    case 15:
    case 16:
        *status = 2;
        return 0;
    case 10:
        *status = 2;
        *out = 0x800;
        return 0;
    }
    if (((cDsObj266358*)D_004A33D8)->v11()) {
        *status = 1;
        return 0;
    }
    *status = 2;
    if (cPS2MemCard_GetLastError(D_004A33D8, *(int*)((char*)D_004A33D8 + 0xC))) {
        return func_00266578(cPS2MemCard_GetLastError(D_004A33D8, *(int*)((char*)D_004A33D8 + 0xC)));
    }
    switch (D_004A33D4) {
    case 0:
        break;
    case 1:
        if (D_004A33E4 > 0) {
            *status = 1;
            D_004A33E4--;
            func_002C67E8(D_004A33D8, D_004A33DC);
            return 0;
        }
        D_004A33E0 = 1;
        break;
    case 2:
        if (*(int*)((char*)D_004A33D8 + 0x54) <= 0) {
            *out = 0;
            break;
        }
        for (int i = 0; i < *(int*)((char*)D_004A33D8 + 0x54); i++) {
            func_002C68C8(D_004A33D8, i, D_00537190);
            if (func_003FA360(D_00537190)) {
                *out = 1;
                return 0;
            }
        }
        *out = 0;
        break;
    case 5:
        *out = D_004A3940;
        break;
    case 6:
        *out = D_004A3940;
        break;
    case 14:
        *out = D_004A3940;
        break;
    case 9:
        *out = ((cDsObj266358*)D_004A33D8)->v20() << 10;
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266550);
#ifdef SKIP_ASM
extern int D_004A33D4;
extern char D_00537190[];
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Memory-card callback 16 of the network-account library: copies the name found by the directory search.
 *
 * Copies D_00537190 (the entry func_00266358 accepted) into @p dst and sets the card operation D_004A33D4 to 16. Entry
 * 16 of D_00442A18.
 *
 * @param dst  receives the file name
 */
extern "C" void func_00266550(char* dst)
{
    strcpy(dst, D_00537190);
    D_004A33D4 = 0x10;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266578);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A33D4;

/**
 * Maps a memory-card error to a network-account library error code for the current card operation D_004A33D4.
 *
 * -2 gives 9 and -4 gives 4 for the search (2), open (4), seek (5), read (6), flush (7), close (8), write (14) and the
 * deletes (12, 13); -3, -5, -7 and -8 give 13 where open, seek, read and write list them, except that -3 gives 14 for
 * seek, write and mkdir (11); mkdir maps -2 to 9 and -4 to 8; the deletes map -5 and -6 to 5 and 6; the free-space
 * query (9) maps -2 to 17; close gives 17 for any other code. Anything unlisted, and every code during the card check
 * (1), gives 3.
 *
 * @param err  the card error (cPS2MemCard GetLastError result, negative)
 * @return the library error code
 */
// Maps a network error code to a front-end message id depending on the current connection stage.
extern "C" int func_00266578(int err)
{
    switch (D_004A33D4) {
    case 1:
        break;
    case 2:
        switch (err) {
        case -2: return 9;
        case -4: return 4;
        }
        break;
    case 9:
        switch (err) {
        case -2: return 17;
        }
        break;
    case 4:
        switch (err) {
        case -2: return 9;
        case -3: return 13;
        case -4: return 4;
        case -5: return 13;
        case -7: return 13;
        }
        break;
    case 8:
        switch (err) {
        case -2: return 9;
        case -4: return 4;
        }
        return 17;
    case 5:
        switch (err) {
        case -2: return 9;
        case -3: return 0xE;
        case -4: return 4;
        case -5: return 13;
        case -8: return 13;
        }
        break;
    case 7:
        switch (err) {
        case -2: return 9;
        case -4: return 4;
        }
        break;
    case 6:
        switch (err) {
        case -2: return 9;
        case -4: return 4;
        case -5: return 13;
        }
        break;
    case 14:
        switch (err) {
        case -2: return 9;
        case -3: return 0xE;
        case -4: return 4;
        case -5: return 13;
        case -8: return 13;
        }
        break;
    case 11:
        switch (err) {
        case -2: return 9;
        case -3: return 0xE;
        case -4: return 8;
        }
        break;
    case 12:
    case 13:
        switch (err) {
        case -2: return 9;
        case -4: return 4;
        case -5: return 5;
        case -6: return 6;
        }
        break;
    }
    return 3;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266728);
#ifdef SKIP_ASM
extern char str_TibExtMemAlloc[];
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

/**
 * Allocates memory for the DirtySock libraries, adding 0x400 bytes of slack.
 *
 * Uses operator new with the tag "TibExtMemAlloc" and flags 0x20000000, or 0x23000000 when @p kind is 0x20. The
 * network-account and lobby code in seg_2F41F8 / seg_2FDDC8 call it as their allocator (func_003F6C70, func_003F9218,
 * func_003FB3A0, func_003FC0E8, func_003FCE10).
 *
 * @param size  the number of bytes wanted
 * @param kind  the caller's memory kind; 0x20 selects the other heap flags
 * @return the new block
 */
extern "C" void* func_00266728(void* self, int size, int kind)
{
    int flags = 0x20000000;
    if (kind == 0x20) {
        flags = 0x23000000;
    }
    return operator_new_tag(size + 0x400, str_TibExtMemAlloc, flags, 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266760);
#ifdef SKIP_ASM
void* cMemMan_free(void* ptr);

/**
 * Frees a block from func_00266728 with cMemMan_free; a null pointer is ignored.
 *
 * @param ptr  the block, or 0
 */
extern "C" void func_00266760(void* self, void* ptr)
{
    if (ptr != 0) {
        cMemMan_free(ptr);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266788);
#ifdef SKIP_ASM
extern char str_cUsbKeyboard[];
extern void* D_004A33F0;
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002667E8(void* self);

/**
 * Creates the USB keyboard reader singleton D_004A33F0.
 *
 * Allocates 1124 bytes tagged "cUsbKeyboard" and runs the constructor func_002667E8 on them. initOnline calls it.
 */
extern "C" void func_00266788(void)
{
    D_004A33F0 = func_002667E8(cMemMan_alloc(1124, str_cUsbKeyboard, 0x20000000, 0));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002667C0);
#ifdef SKIP_ASM
extern "C" void func_00266850(int, int);

/**
 * Destroys the USB keyboard reader singleton D_004A33F0, if there is one, and clears the pointer.
 *
 * Calls the deleting destructor func_00266850 with flags 3.
 */
extern "C" void func_002667C0(void) {
    if ((*(int*)&D_004A33F0) != 0) {
        func_00266850((*(int*)&D_004A33F0), 3);
        (*(int*)&D_004A33F0) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002667E8);
#ifdef SKIP_ASM
extern char D_005371D0[];
extern "C" void func_002668A8(void* self);
extern "C" int func_003F61D0(int a0, int a1, void* name, void* cb, int a4);
extern "C" void* MEM_fill(void* dst, int value, int size);

/**
 * Constructs the USB keyboard reader: starts the keyboard library and clears the 8 key rings.
 *
 * Calls func_003F61D0(0, 0, D_005371D0, func_002668A8, 0) (D_005371D0 the library's read buffers, func_002668A8 its
 * keyboard-connect callback) and sets the enabled word (+0x460) only when it succeeds, then zeroes the 0x460 bytes of
 * rings (8 x 0x8C).
 *
 * @return @p self
 */
extern "C" void* func_002667E8(void* self)
{
    *(int*)((char*)self + 0x460) = 0;
    if (func_003F61D0(0, 0, D_005371D0, (void*)func_002668A8, 0) >= 0) {
        *(int*)((char*)self + 0x460) = 1;
    }
    MEM_fill(self, 0, 1120);
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266850);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003F62F8();
extern "C" void func_00266850_impl(void* self, int flags) __asm__("func_00266850");

/**
 * Destroys the USB keyboard reader: shuts the keyboard library down (func_003F62F8) when it was started, then frees the
 * object when bit 0 of @p flags is set.
 *
 * Clears the enabled word (+0x460).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_00266850_impl(void *self, int flags) {
    if (*(int *)((char*)self + 0x460) != 0) {
        func_003F62F8();
        *(int *)((char*)self + 0x460) = 0;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002668A8);
#ifdef SKIP_ASM
extern "C" void func_003F6358(void* self);
extern "C" void func_0040C7C0(void* self, int a1);
extern "C" void func_0040D430(void* self, int a1);

/**
 * Sets up a newly connected USB keyboard: the connect callback func_002667E8 registers.
 *
 * Runs the library's default callback func_003F6358 (LED setup; the second argument register passes through untouched),
 * then func_0040C7C0 and func_0040D430 with the keyboard number (the first argument) and 0, which put that keyboard's
 * two modes back to 0.
 */
extern "C" void func_002668A8(void* self)
{
    func_003F6358(self);
    func_0040C7C0(self, 0);
    func_0040D430(self, 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002668E8);
#ifdef SKIP_ASM
extern "C" void func_003F63F0();
extern "C" int func_003F6520(int idx);
extern "C" int func_003F6348();
extern "C" void* func_003F64E0(int idx);
extern "C" unsigned char* func_003F68F0(void* conn, int a1, int idx);
extern "C" int func_003F6838(void* conn, int a1, int a2, int a3);

struct sSlotFlags_668E8 {
    unsigned active : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
};

struct sSlot_668E8 {
    int count;                  // 0x0
    int start;                  // 0x4
    unsigned char buf[64][2];   // 0x8
    sSlotFlags_668E8 flags;     // 0x88
};

struct sSlots_668E8 {
    sSlot_668E8 slot[8];        // 0x0
    int enabled;                // 0x460
};

struct sConn_668E8 {
    unsigned on : 1;            // 0x0
    int f4;
    int count;                  // 0x8
};

static inline int ringIdx_668E8(int w)
{
    return w % 64;
}

/**
 * Updates the USB keyboard reader: reads every connected keyboard's new keys into its 64-entry ring.
 *
 * Does nothing unless enabled (+0x460). Polls the library (func_003F63F0), marks each of the 8 slots active when
 * func_003F6520 reports a keyboard (clearing all its flags otherwise), then for each active slot below the connect
 * count (func_003F6348) takes the ready read block (func_003F64E0): each key code is translated by func_003F68F0 and,
 * when either result byte is non-zero, the pair is appended to the ring (the oldest entry is dropped once 64 are
 * waiting). Flag bit 4 is set from func_003F6838(block, 0, 0, 0x10) and bits 1..3 from bit 0 of the block's first word.
 * cSSXApp_preUpdate calls it.
 */
extern "C" void func_002668E8(sSlots_668E8* self)
{
    if (self->enabled == 0)
        return;
    func_003F63F0();
    for (int i = 0; i < 8; i++) {
        sSlot_668E8* keyboardSlot = &self->slot[i];
        keyboardSlot->flags.active = func_003F6520(i) == 1;
        if (!keyboardSlot->flags.active)
            *(int*)&keyboardSlot->flags = 0;
    }
    int n = func_003F6348();
    for (int j = 0; j < n; j++) {
        if (self->slot[j].flags.active) {
            sConn_668E8* readBlock = (sConn_668E8*)func_003F64E0(j);
            if (readBlock != 0) {
                for (int k = 0; k < readBlock->count; k++) {
                    unsigned char* translated = func_003F68F0(readBlock, 0, k);
                    if (translated[0] != 0 || translated[1] != 0) {
                        int w = self->slot[j].count;
                        int idx = ringIdx_668E8(w);
                        int ringStart = self->slot[j].start;
                        if (w - ringStart >= 0x40)
                            self->slot[j].start = ringStart + 1;
                        self->slot[j].buf[idx][0] = translated[0];
                        self->slot[j].buf[idx][1] = translated[1];
                        self->slot[j].count++;
                    }
                }
                if (func_003F6838(readBlock, 0, 0, 0x10))
                    self->slot[j].flags.b4 = 1;
                else
                    self->slot[j].flags.b4 = 0;
                self->slot[j].flags.b2 = readBlock->on;
                self->slot[j].flags.b3 = readBlock->on;
                self->slot[j].flags.b1 = readBlock->on;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266B80);
#ifdef SKIP_ASM
struct sEnt8C {
    int a;
    int b;
    char pad[0x84];
};

struct sEntTable {
    sEnt8C ents[8];
    int active;
};

/**
 * Empties one USB keyboard slot's key ring (count and read position set to 0), when the reader is enabled.
 *
 * @param idx  the keyboard slot, 0..7
 */
extern "C" void func_00266B80(sEntTable* self, int idx)
{
    if (self->active != 0) {
        self->ents[idx].b = 0;
        self->ents[idx].a = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266BA8);
#ifdef SKIP_ASM
struct sChan_266BA8 {
    int count;                      // 0x0
    int read;                       // 0x4
    unsigned char buf[64][2];       // 0x8
    int pad;
};

struct sChanSet_266BA8 {
    sChan_266BA8 chan[8];           // 0x0
    char pad[0x460 - 8 * 0x8C];
    int active;                     // 0x460
};

/**
 * Takes the next key pair from a USB keyboard slot's ring.
 *
 * Both outputs start at -1 and a zero byte leaves its output at -1. Does nothing when the reader is disabled or @p ch
 * is not below the connect count (func_003F6348). cFEInput_update and cMenuSys_input call it.
 *
 * @param ch         the keyboard slot
 * @param firstOut   receives the pair's first byte, or -1
 * @param secondOut  receives the pair's second byte, or -1
 * @return 1 when a pair was taken, 0 when the ring is empty
 */
// Pops the next (a, b) pair from channel ch's 64-entry ring; zero entries leave the outputs at -1.
extern "C" int func_00266BA8(sChanSet_266BA8* self, int ch, int* firstOut, int* secondOut)
{
    *firstOut = -1;
    *secondOut = -1;
    if (self->active != 0 && ch < func_003F6348()) {
        if (self->chan[ch].read < self->chan[ch].count) {
            int i = self->chan[ch].read;
            i %= 64;
            if (self->chan[ch].buf[i][0] != 0)
                *firstOut = self->chan[ch].buf[i][0];
            if (self->chan[ch].buf[i][1] != 0)
                *secondOut = self->chan[ch].buf[i][1];
            self->chan[ch].read++;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266CA0);
#ifdef SKIP_ASM
extern char str_cVoipMan[];
extern void* D_004A33F4;
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* func_00266D00(void* self);

/**
 * Creates the voice-chat manager singleton D_004A33F4.
 *
 * Allocates 8 bytes tagged "cVoipMan" and runs the constructor func_00266D00 on them. cFEStateMainMenu_onWidgetEvent
 * and cDebugMenuScreen_cDebugMenuScreen call it.
 */
extern "C" void func_00266CA0(void)
{
    D_004A33F4 = func_00266D00(cMemMan_alloc(8, str_cVoipMan, 0x20000000, 0));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266CD8);
#ifdef SKIP_ASM
extern void* D_004A33F4;
extern "C" void func_00266D10(void* self, int flags);

/**
 * Destroys the voice-chat manager singleton D_004A33F4, if there is one, and clears the pointer.
 *
 * Calls the deleting destructor func_00266D10 with flags 3.
 */
extern "C" void func_00266CD8(void)
{
    if (D_004A33F4 != 0) {
        func_00266D10(D_004A33F4, 3);
        D_004A33F4 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266D00__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the voice-chat manager: clears its connected word (+0x0) and its voice-on word (+0x4).
 *
 * @return @p self
 */
void* func_00266D00(void* self)
{
    int zero = 0;
    *(int*)self = zero;
    *(int*)((char*)self + 0x4) = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266D10);
#ifdef SKIP_ASM
extern "C" void func_00266DF8(void* self);
void operator_delete(int*);

/**
 * Destroys the voice-chat manager: disconnects it (func_00266DF8), then frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_00266D10(void* self, int flags)
{
    func_00266DF8(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266D58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A33F8;
extern int cBEOptionInterface_mOption[];
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_003F6AB8(void* a0, int a1, int a2);
extern "C" void func_003F6C10(int a0, int a1);
extern "C" void func_00266E88(void* self);
extern "C" void func_00266E30(void* self);

/**
 * Connects voice chat to a peer and turns the voice on or off from the settings.
 *
 * Only when not yet connected: connects the voice module to @p peerAddr on ports 6000/6000 (func_003F6AB8), sets the
 * connected word (+0x0) and applies the voice level D_004A33F8 (func_003F6C10 with 85). A level below 10 turns the
 * voice off (func_00266E88); otherwise it is turned on (func_00266E30) when bit 18 of the option word
 * cBEOptionInterface_mOption is set. func_002639E0 and the debug front end call it.
 *
 * @param peerAddr  the peer's address
 */
extern "C" void func_00266D58(void* self, void* peerAddr)
{
    if (*(int*)self == 0) {
        func_003F6AB8(peerAddr, 6000, 6000);
        int n = D_004A33F8;
        *(int*)self = 1;
        func_003F6C10(n, 85);
        if (D_004A33F8 < 10) {
            func_00266E88(self);
        } else {
            cBE_getInterface_Fv(cBE_getBE(), 4);
            if ((cBEOptionInterface_mOption[0] >> 18) & 1) {
                func_00266E30(self);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266DF8);
#ifdef SKIP_ASM
extern "C" void func_003F6AF0(void* self);

/**
 * Disconnects voice chat: when connected (+0x0), stops the voice module (func_003F6AF0) and clears the connected word.
 */
extern "C" void func_00266DF8(void* self)
{
    if (*(void**)self != 0) {
        func_003F6AF0(self);
        *(void**)self = 0;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266E30);
#ifdef SKIP_ASM
extern int D_004A33F8;
extern "C" void func_003F6B20();

/**
 * Turns the voice on (func_003F6B20) when connected, not already on and the voice level D_004A33F8 is at least 10.
 *
 * Sets the voice-on word (+0x4).
 */
extern "C" void func_00266E30(void* self)
{
    if (*(int*)self != 0 && *(int*)((char*)self + 0x4) == 0 && D_004A33F8 >= 10) {
        func_003F6B20();
        *(int*)((char*)self + 0x4) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266E88);
#ifdef SKIP_ASM
extern "C" void func_003F6B50(void* self);

/**
 * Turns the voice off (func_003F6B50) when connected and on, and clears the voice-on word (+0x4).
 */
extern "C" void func_00266E88(void* self)
{
    if (*(int*)self != 0 && *(int*)((char*)self + 0x4) != 0) {
        func_003F6B50(self);
        *(int*)((char*)self + 0x4) = 0;
    }
}
#endif

extern "C" void* func_003F6C40(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266ED0);
#ifdef SKIP_ASM
/**
 * Toggles the voice: turns it off when the voice-on word (+0x4) is set (func_00266E88), else on (func_00266E30).
 */
extern "C" void func_00266ED0(void* self)
{
    if (*(int*)((char*)self + 4) != 0) {
        func_00266E88(self);
    } else {
        func_00266E30(self);
    }
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F08);
#ifdef SKIP_ASM
extern "C" int func_003F6B80(void*);

/**
 * Returns bit 0 of the voice module's status word (func_003F6B80) when connected (+0x0).
 *
 * @return that bit, or 0 when not connected
 */
extern "C" int func_00266F08(int* self)
{
    return *self != 0 ? func_003F6B80(self) : 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F38__FPv);
#ifdef SKIP_ASM
/**
 * Returns the voice module's status word (func_003F6C40), whether or not voice is connected.
 *
 * func_0025C610 adds 0x100 to the account's "HWFLAG" when bit 4 of it is set.
 *
 * @return the status word
 */
void* func_00266F38(void* self)
{
    return func_003F6C40(self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F58);
#ifdef SKIP_ASM
extern int D_004A33F8;
extern "C" void func_003F6C10(int a0, int a1);

/**
 * Sets the voice level: sends @p level and @p maxLevel to the voice module (func_003F6C10) when connected, and always stores @p
 * a1 in D_004A33F8.
 *
 * cOVState_PAUSE_OPTIONS_onWidgetEvent passes the voice slider value times 10 and the maximum func_00266F90 read.
 *
 * @param level  the voice level (below 10 counts as off)
 * @param maxLevel  the second value for the module (the maximum level)
 */
extern "C" void func_00266F58(void* self, int level, int maxLevel)
{
    if (*(int*)self != 0) {
        func_003F6C10(level, maxLevel);
    }
    D_004A33F8 = level;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F90);
#ifdef SKIP_ASM
extern "C" void func_003F6BA0(void* a, void* b);

/**
 * Reads two words from the voice module (func_003F6BA0) when connected; otherwise leaves them unchanged.
 *
 * The pause options screen reads them as the current voice level and its maximum.
 *
 * @param outLevel  receives the first word (the level)
 * @param outMax  receives the second word (the maximum)
 */
extern "C" void func_00266F90(void* self, void* outLevel, void* outMax)
{
    if (*(int*)self != 0) {
        func_003F6BA0(outLevel, outMax);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266FC0);
#ifdef SKIP_ASM
/**
 * Returns its first argument unchanged; the variadic arguments are not read.
 *
 * @return @p self
 */
extern "C" void* func_00266FC0(void* self, ...)
{
    return self;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267000);
#ifdef SKIP_ASM
/**
 * Compares two signed char ranges lexicographically.
 *
 * @param first1  start of the first range
 * @param last1   end of the first range
 * @param first2  start of the second range
 * @param last2   end of the second range
 * @return 1 when the first range sorts before the second (including when it is a proper prefix of it), otherwise 0
 */
extern "C" int func_00267000(signed char* first1, signed char* last1, signed char* first2, signed char* last2)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2)
            return 1;
        if (*first2 < *first1)
            return 0;
        ++first1;
        ++first2;
    }
    return first1 == last1 && first2 != last2;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267068);
#ifdef SKIP_ASM
/**
 * Compares two signed char ranges lexicographically, three ways.
 *
 * @param first1  start of the first range
 * @param last1   end of the first range
 * @param first2  start of the second range
 * @param last2   end of the second range
 * @return -1 when the first range sorts first, 1 when the second does, 0 when they are equal
 */
extern "C" int func_00267068(signed char* first1, signed char* last1, signed char* first2, signed char* last2)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2)
            return -1;
        if (*first2 < *first1)
            return 1;
        ++first1;
        ++first2;
    }
    if (first2 == last2)
        return !(first1 == last1);
    else
        return -1;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002670C8);
#ifdef SKIP_ASM
struct sListNode_2670C8 {
    sListNode_2670C8* next;
    sListNode_2670C8* prev;
};

/**
 * Empties a linked list: frees every node after the head node (pointer at +0x4) and links the head node to itself.
 *
 * The layout (allocator word at +0x0, head node at +0x4) is that of LobbyMan's mEventList; the nodes' data is not
 * freed.
 */
extern "C" void func_002670C8(char* self)
{
    sListNode_2670C8* head = *(sListNode_2670C8**)(self + 4);
    sListNode_2670C8* cur = head->next;
    while (cur != *(sListNode_2670C8**)(self + 4)) {
        sListNode_2670C8* node = cur;
        cur = cur->next;
        operator_delete((int*)node);
    }
    (*(sListNode_2670C8**)(self + 4))->next = *(sListNode_2670C8**)(self + 4);
    (*(sListNode_2670C8**)(self + 4))->prev = *(sListNode_2670C8**)(self + 4);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267128);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);
void* cMemMan_free(void* ptr);
void operator_delete(int*);
extern char str_StlBlock_b[];

struct sPtrVec7128 {
    void* alloc;
    void** start;
    void** finish;
    void** eos;
};

static inline void** sPtrVec7128_allocate(unsigned int n)
{
    if (n == 1)
        return (void**)cMemMan_alloc(4, str_StlBlock_b, 0x20000000, 0);
    return (void**)operator_new_tag(n * 4, str_StlBlock_b, 0x20000000, 0);
}

static inline void sPtrVec7128_deallocate(void** p, unsigned int n)
{
    if (p != 0) {
        if (n == 1)
            operator_delete((int*)p);
        else
            cMemMan_free(p);
    }
}

static inline void** sPtrVec7128_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

static inline void** sPtrVec7128_ucopy(void** first, void** last, void** result)
{
    return sPtrVec7128_copy(first, last, result);
}

static inline void sPtrVec7128_construct(void** p, void* const& v)
{
    if (p != 0)
        *p = v;
}

static inline void** sPtrVec7128_copy_backward(void** first, void** last, void** result)
{
    int n = last - first;
    func_0041610C(result - n, first, n * sizeof(void*));
    return result - n;
}

/**
 * Inserts a pointer into a pointer vector at a position, growing the storage when it is full.
 *
 * Vector words: allocator, start, finish, end of storage. With spare room the tail moves up one slot (func_0041610C);
 * otherwise a block of twice the size (1 when empty; tag "StlBlock") receives the front, the new pointer and the tail,
 * and the old block is freed. func_00261CD0 uses it for BuddyMan's received-message vector (mMessagesStart).
 *
 * @param pos  where the pointer goes
 * @param x    the pointer to insert
 */
extern "C" void func_00267128(sPtrVec7128* self, void** pos, void* const& x)
{
    if (self->finish != self->eos) {
        if (self->finish != 0)
            *self->finish = self->finish[-1];
        ++self->finish;
        void* x_copy = x;
        sPtrVec7128_copy_backward(pos, self->finish - 2, self->finish - 1);
        *pos = x_copy;
    } else {
        unsigned int old_size = self->finish - self->start;
        unsigned int len = old_size != 0 ? 2 * old_size : 1;
        void** new_start = sPtrVec7128_allocate(len);
        void** new_finish = new_start;
        new_finish = sPtrVec7128_ucopy(self->start, pos, new_start);
        sPtrVec7128_construct(new_finish, x);
        ++new_finish;
        new_finish = sPtrVec7128_ucopy(pos, self->finish, new_finish);
        sPtrVec7128_deallocate(self->start, self->eos - self->start);
        self->start = new_start;
        self->finish = new_finish;
        self->eos = new_start + len;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002672C8);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);
void* cMemMan_free(void* ptr);
void operator_delete(int*);
extern char str_StlBlock_b[];

struct sPtrVec72C8 {
    void* alloc;
    void** start;
    void** finish;
    void** eos;
};

static inline void** sPtrVec72C8_allocate(unsigned int n)
{
    if (n == 1)
        return (void**)cMemMan_alloc(4, str_StlBlock_b, 0x20000000, 0);
    return (void**)operator_new_tag(n * 4, str_StlBlock_b, 0x20000000, 0);
}

static inline void sPtrVec72C8_deallocate(void** p, unsigned int n)
{
    if (p != 0) {
        if (n == 1)
            operator_delete((int*)p);
        else
            cMemMan_free(p);
    }
}

static inline void** sPtrVec72C8_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

static inline void** sPtrVec72C8_ucopy(void** first, void** last, void** result)
{
    return sPtrVec72C8_copy(first, last, result);
}

static inline void sPtrVec72C8_construct(void** p, void* const& v)
{
    if (p != 0)
        *p = v;
}

static inline void** sPtrVec72C8_copy_backward(void** first, void** last, void** result)
{
    int n = last - first;
    func_0041610C(result - n, first, n * sizeof(void*));
    return result - n;
}

/**
 * Inserts a pointer into a pointer vector at a position, growing the storage when it is full.
 *
 * Same code as func_00267128. func_00262AF0 uses it for BuddyMan's challenger vector (mChallengersStart).
 *
 * @param pos  where the pointer goes
 * @param x    the pointer to insert
 */
extern "C" void func_002672C8(sPtrVec72C8* self, void** pos, void* const& x)
{
    if (self->finish != self->eos) {
        if (self->finish != 0)
            *self->finish = self->finish[-1];
        ++self->finish;
        void* x_copy = x;
        sPtrVec72C8_copy_backward(pos, self->finish - 2, self->finish - 1);
        *pos = x_copy;
    } else {
        unsigned int old_size = self->finish - self->start;
        unsigned int len = old_size != 0 ? 2 * old_size : 1;
        void** new_start = sPtrVec72C8_allocate(len);
        void** new_finish = new_start;
        new_finish = sPtrVec72C8_ucopy(self->start, pos, new_start);
        sPtrVec72C8_construct(new_finish, x);
        ++new_finish;
        new_finish = sPtrVec72C8_ucopy(pos, self->finish, new_finish);
        sPtrVec72C8_deallocate(self->start, self->eos - self->start);
        self->start = new_start;
        self->finish = new_finish;
        self->eos = new_start + len;
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267468);
#ifdef SKIP_ASM
// Static data of this unit: the 56-entry colour table (the same table is built in
// camera/trigger/cameratriggerfactory func_00177650 and animation/gameanimmap func_003150F8)
// and the handle constants (bank, 24-bit index) D_004A50D8..D_004A5150.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_267468 {
    float r, g, b, a;
    sColor_267468(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sHandle_267468 {
    char bank;
    unsigned int index : 24;
    sHandle_267468(int abank, unsigned int aindex) : bank(abank), index(aindex) {}
};

struct sPair_267468 {
    int a;
    int b;
    sPair_267468() { b = 0; a = 0; }
};

extern sColor_267468 D_004D2C38;
extern sColor_267468 D_004D2C48;
extern sColor_267468 D_004D2C58;
extern sColor_267468 D_004D2C68;
extern sColor_267468 D_004D2C78;
extern sColor_267468 D_004D2C88;
extern sColor_267468 D_004D2C98;
extern sColor_267468 D_004D2CA8;
extern sColor_267468 D_004D2CB8;
extern sColor_267468 D_004D2CC8;
extern sColor_267468 D_004D2CD8;
extern sColor_267468 D_004D2CE8;
extern sColor_267468 D_004D2CF8;
extern sColor_267468 D_004D2D08;
extern sColor_267468 D_004D2D18;
extern sColor_267468 D_004D2D28;
extern sColor_267468 D_004D2D38;
extern sColor_267468 D_004D2D48;
extern sColor_267468 D_004D2D58;
extern sColor_267468 D_004D2D68;
extern sColor_267468 D_004D2D78;
extern sColor_267468 D_004D2D88;
extern sColor_267468 D_004D2D98;
extern sColor_267468 D_004D2DA8;
extern sColor_267468 D_004D2DB8;
extern sColor_267468 D_004D2DC8;
extern sColor_267468 D_004D2DD8;
extern sColor_267468 D_004D2DE8;
extern sColor_267468 D_004D2DF8;
extern sColor_267468 D_004D2E08;
extern sColor_267468 D_004D2E18;
extern sColor_267468 D_004D2E28;
extern sColor_267468 D_004D2E38;
extern sColor_267468 D_004D2E48;
extern sColor_267468 D_004D2E58;
extern sColor_267468 D_004D2E68;
extern sColor_267468 D_004D2E78;
extern sColor_267468 D_004D2E88;
extern sColor_267468 D_004D2E98;
extern sColor_267468 D_004D2EA8;
extern sColor_267468 D_004D2EB8;
extern sColor_267468 D_004D2EC8;
extern sColor_267468 D_004D2ED8;
extern sColor_267468 D_004D2EE8;
extern sColor_267468 D_004D2EF8;
extern sColor_267468 D_004D2F08;
extern sColor_267468 D_004D2F18;
extern sColor_267468 D_004D2F28;
extern sColor_267468 D_004D2F38;
extern sColor_267468 D_004D2F48;
extern sColor_267468 D_004D2F58;
extern sColor_267468 D_004D2F68;
extern sColor_267468 D_004D2F78;
extern sColor_267468 D_004D2F88;
extern sColor_267468 D_004D2F98;
extern sColor_267468 D_004D2FA8;
extern sHandle_267468 D_004A50D8;
extern sPair_267468 D_004A50E0;
extern sHandle_267468 D_004A50E8;
extern sHandle_267468 D_004A50F0;
extern sHandle_267468 D_004A50F8;
extern sHandle_267468 D_004A5100;
extern sHandle_267468 D_004A5108;
extern sHandle_267468 D_004A5110;
extern sHandle_267468 D_004A5118;
extern sHandle_267468 D_004A5120;
extern sHandle_267468 D_004A5128;
extern sHandle_267468 D_004A5130;
extern sHandle_267468 D_004A5138;
extern sHandle_267468 D_004A5140;
extern sHandle_267468 D_004A5148;
extern sHandle_267468 D_004A5150;

/**
 * Constructs the unit's static constants: 56 colours and 16 handle words.
 *
 * Runs only for @p initialize non-zero and @p priority 0xFFFF (func_00269EA0 passes 1, 0xFFFF). The colours
 * (D_004D2C38..D_004D2FA8, four floats each) are the table also built in cameratriggerfactory and gameanimmap; the
 * handles are D_004A50D8 = (-1, 0xFFFFFF) and D_004A50E8..D_004A5150 = (n, n) for n = 0..13, each an 8-bit bank plus a
 * 24-bit index; D_004A50E0 is zeroed.
 *
 * @param initialize  non-zero to construct
 * @param priority    the init priority; only 0xFFFF does anything
 */
// Static initializer (initialize, priority).
extern "C" void func_00267468(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004D2C38) sColor_267468(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D2C48) sColor_267468(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D2C58) sColor_267468(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D2C68) sColor_267468(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D2C78) sColor_267468(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D2C88) sColor_267468(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D2C98) sColor_267468(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D2CA8) sColor_267468(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D2CB8) sColor_267468(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D2CC8) sColor_267468(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D2CD8) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2CE8) sColor_267468(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D2CF8) sColor_267468(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D2D08) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2D18) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2D28) sColor_267468(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D2D38) sColor_267468(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D2D48) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2D58) sColor_267468(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D2D68) sColor_267468(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D2D78) sColor_267468(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D2D88) sColor_267468(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D2D98) sColor_267468(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D2DA8) sColor_267468(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D2DB8) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2DC8) sColor_267468(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D2DD8) sColor_267468(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D2DE8) sColor_267468(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D2DF8) sColor_267468(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D2E08) sColor_267468(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D2E18) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2E28) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2E38) sColor_267468(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D2E48) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2E58) sColor_267468(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D2E68) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2E78) sColor_267468(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D2E88) sColor_267468(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D2E98) sColor_267468(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D2EA8) sColor_267468(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D2EB8) sColor_267468(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D2EC8) sColor_267468(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D2ED8) sColor_267468(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D2EE8) sColor_267468(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D2EF8) sColor_267468(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D2F08) sColor_267468(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D2F18) sColor_267468(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D2F28) sColor_267468(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D2F38) sColor_267468(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D2F48) sColor_267468(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D2F58) sColor_267468(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D2F68) sColor_267468(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D2F78) sColor_267468(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D2F88) sColor_267468(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D2F98) sColor_267468(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D2FA8) sColor_267468(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004A50D8) sHandle_267468(-1, 0xFFFFFF);
        new (&D_004A50E0) sPair_267468();
        new (&D_004A50E8) sHandle_267468(0, 0);
        new (&D_004A50F0) sHandle_267468(1, 1);
        new (&D_004A50F8) sHandle_267468(2, 2);
        new (&D_004A5100) sHandle_267468(3, 3);
        new (&D_004A5108) sHandle_267468(4, 4);
        new (&D_004A5110) sHandle_267468(5, 5);
        new (&D_004A5118) sHandle_267468(6, 6);
        new (&D_004A5120) sHandle_267468(7, 7);
        new (&D_004A5128) sHandle_267468(8, 8);
        new (&D_004A5130) sHandle_267468(9, 9);
        new (&D_004A5138) sHandle_267468(10, 10);
        new (&D_004A5140) sHandle_267468(11, 11);
        new (&D_004A5148) sHandle_267468(12, 12);
        new (&D_004A5150) sHandle_267468(13, 13);
    }
}
#endif

extern "C" void* cCommChannel_channelClose(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_isChannelOpen);
#ifdef SKIP_ASM
/**
 * Tells whether a comm channel is open: returns mChannelOpen (+0x0).
 *
 * @param channel  the cCommChannel
 * @return mChannelOpen
 */
extern "C" int cCommChannel_isChannelOpen(int *channel) {
    return *channel;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_setError);
#ifdef SKIP_ASM
/**
 * Flags an error on a comm channel: sets mError (+0x4) to 1.
 *
 * @param channel  the cCommChannel
 */
extern "C" void cCommChannel_setError(void *channel) {
    (*(int *)((char*)(channel) + (4))) = 1;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_getError);
#ifdef SKIP_ASM
/**
 * Tells whether a comm channel has an error: returns mError (+0x4).
 *
 * @param channel  the cCommChannel
 * @return mError
 */
extern "C" int cCommChannel_getError(void *channel) {
    return (*(int *)((char*)(channel) + (4)));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_disconnect__FPv);
#ifdef SKIP_ASM
/**
 * Disconnects the channel by clearing its first two words, if the first is set (cCommChannel_channelClose).
 *
 * @return no meaningful value (cCommChannel_channelClose() returns nothing)
 */
void* cCommChannel_disconnect(void* self)
{
    return cCommChannel_channelClose(self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base comm channel's update has an empty body.
 */
void cCommChannel_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_isConnected__FPv);
#ifdef SKIP_ASM
/**
 * Reports the base comm channel as never connected.
 *
 * @return always 0
 */
int cCommChannel_isConnected(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_getLatency);
#ifdef SKIP_ASM
/**
 * Returns a fixed latency of one 60 Hz frame.
 *
 * @return 1/60 (about 0.0167)
 */
extern "C" float cCommChannel_getLatency(void)
{
    return 0.01666666753590107f;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_getChannelType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the base comm channel's type id.
 *
 * @return always 0 (cDSockChannel returns 1)
 */
int cCommChannel_getChannelType(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_readPacket__FPv);
#ifdef SKIP_ASM
/**
 * Reads no packet: the base comm channel always reports nothing read.
 *
 * @return always 0
 */
int cCommChannel_readPacket(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cCommChannel_writePacket__FPv);
#ifdef SKIP_ASM
/**
 * Writes no packet: the base comm channel always reports nothing written.
 *
 * @return always 0
 */
int cCommChannel_writePacket(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cDSockChannel_isConnected);
#ifdef SKIP_ASM
/**
 * Tells whether the DirtySock channel is connected.
 *
 * @return 1 when mState is 2 (connected), otherwise 0
 */
extern "C" int cDSockChannel_isConnected(cDSockChannel* self)
{
    return self->mState == 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", cDSockChannel_getChannelType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the DirtySock channel's type id.
 *
 * @return always 1 (the base cCommChannel returns 0)
 */
int cDSockChannel_getChannelType(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_Event__Event);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys a plain online event: restores the event vtable online_Event_vtbl at +0x4 and frees the object when bit 0 of @p
 * flags is set.
 *
 * Slot 1 of online_Event_vtbl, the vtable of the 8-byte kind-only events (kind at +0x0, tag "NetEvent") posted on LobbyMan's
 * mEventList.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void online_Event__Event(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_Event_clone);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* online_Event_vtbl[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

/**
 * Copies a plain online event: allocates 8 bytes tagged "NetEvent" with the same kind (+0x0) and vtable online_Event_vtbl.
 *
 * Slot 2 of online_Event_vtbl.
 *
 * @return the new event
 */
extern "C" void* online_Event_clone(int* self)
{
    int* copy = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    int kind = *self;
    *(void***)((char*)copy + 0x4) = online_Event_vtbl;
    copy[0] = kind;
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_ErrorEvent__ErrorEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys an error event (vtable online_ErrorEvent_vtbl): restores the base event vtable online_Event_vtbl at +0x4 and frees the object
 * when bit 0 of @p flags is set.
 *
 * Slot 1 of online_ErrorEvent_vtbl, the vtable of the 0xC-byte kind-258 events that carry an error code at +0x8 (LobbyMan posts
 * them with codes such as 'time').
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void online_ErrorEvent__ErrorEvent(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_ErrorEvent_clone);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* online_ErrorEvent_vtbl[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

/**
 * Copies an error event: allocates 0xC bytes tagged "NetEvent" with kind 258, vtable online_ErrorEvent_vtbl and the error code
 * (+0x8) of @p self.
 *
 * Slot 2 of online_ErrorEvent_vtbl.
 *
 * @return the new event
 */
extern "C" void* online_ErrorEvent_clone(void* self)
{
    int* copy = (int*)cMemMan_alloc(0xC, str_NetEvent, 0x20000000, 0);
    int errorCode = *(int*)((char*)self + 0x8);
    copy[0] = 258;
    *(void***)((char*)copy + 0x4) = online_ErrorEvent_vtbl;
    copy[2] = errorCode;
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_BuddyErrorEvent__BuddyErrorEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys a buddy error event (vtable online_BuddyErrorEvent_vtbl): restores the base event vtable online_Event_vtbl at +0x4 and frees the
 * object when bit 0 of @p flags is set.
 *
 * Slot 1 of online_BuddyErrorEvent_vtbl, the vtable of the kind-259 events LobbyMan's update (func_0025AC50) posts with BuddyMan's
 * miError at +0x8 when the buddy login fails.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void online_BuddyErrorEvent__BuddyErrorEvent(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_BuddyErrorEvent_clone);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* online_ErrorEvent_vtbl[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

/**
 * Copies a buddy error event, but as a plain error event: allocates 0xC bytes tagged "NetEvent" with kind 258, vtable
 * online_ErrorEvent_vtbl and the error code (+0x8) of @p self.
 *
 * Slot 2 of online_BuddyErrorEvent_vtbl. Quirk: the copy gets the error event's kind (258) and vtable online_ErrorEvent_vtbl, not this event's kind
 * 259 and vtable online_BuddyErrorEvent_vtbl; the body is identical to online_ErrorEvent_clone (the July 2003 preview's copy does the same).
 *
 * @return the new event
 */
extern "C" void* online_BuddyErrorEvent_clone(void* self)
{
    int* copy = (int*)cMemMan_alloc(0xC, str_NetEvent, 0x20000000, 0);
    int errorCode = *(int*)((char*)self + 0x8);
    copy[0] = 258;
    *(void***)((char*)copy + 0x4) = online_ErrorEvent_vtbl;
    copy[2] = errorCode;
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_NetErrorEvent__NetErrorEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys a network error event (vtable online_NetErrorEvent_vtbl): restores the base event vtable online_Event_vtbl at +0x4 and frees the
 * object when bit 0 of @p flags is set.
 *
 * Slot 1 of online_NetErrorEvent_vtbl, the vtable of the kind-278 events (error value at +0x8) that tagsunk allocates with the tag
 * "NetErrorEvent".
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void online_NetErrorEvent__NetErrorEvent(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_NetErrorEvent_clone);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* online_NetErrorEvent_vtbl[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

/**
 * Copies a network error event: allocates 0xC bytes tagged "NetEvent" with kind 278, vtable online_NetErrorEvent_vtbl and the value
 * (+0x8) of @p self.
 *
 * Slot 2 of online_NetErrorEvent_vtbl.
 *
 * @return the new event
 */
extern "C" void* online_NetErrorEvent_clone(void* self)
{
    int* copy = (int*)cMemMan_alloc(0xC, str_NetEvent, 0x20000000, 0);
    int errorValue = *(int*)((char*)self + 0x8);
    copy[0] = 278;
    *(void***)((char*)copy + 0x4) = online_NetErrorEvent_vtbl;
    copy[2] = errorValue;
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268048);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

/**
 * Destroys a chat event (vtable D_00481230): destroys its two cBXStrings (+0xC, then +0x8), restores the base event
 * vtable online_Event_vtbl at +0x4 and frees the object when bit 0 of @p flags is set.
 *
 * Slot 1 of D_00481230; func_0025AB40 builds these 0x10-byte kind-222 events (tag "ChatEvent").
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_00268048(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0xC, 2);
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002680A8);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void* cBXString_cBXString1(void* self, void* other);
extern char str_ChatEvent[];
extern void* D_00481230[];

// Stand-in for cBXString (defined elsewhere): a class with a non-trivial copy constructor,
// which g++ keeps in memory (BLKmode, 16-byte stack slots).
struct sBXStrK2 {
    char* s;
    sBXStrK2() {}
    sBXStrK2(const sBXStrK2& o);
};

/**
 * Copies a chat event: allocates 0x10 bytes tagged "ChatEvent" with kind 222, vtable D_00481230 and copies of both
 * cBXStrings (+0x8, +0xC).
 *
 * Copies each string into a stack temporary first (cBXString_cBXString1) and destroys the temporaries after. Slot 2 of
 * D_00481230.
 *
 * @return the new event
 */
// Clone of a two-string message (type 0xDE).
extern "C" void* func_002680A8(char* self)
{
    char* eventCopy = (char*)cMemMan_alloc(0x10, str_ChatEvent, 0x20000000, 0);
    sBXStrK2 sender;
    sBXStrK2 text;
    cBXString_cBXString1(&sender, self + 0x8);
    cBXString_cBXString1(&text, self + 0xC);
    *(void***)(eventCopy + 0x4) = D_00481230;
    *(int*)eventCopy = 222;
    cBXString_cBXString1(eventCopy + 0x8, &sender);
    cBXString_cBXString1(eventCopy + 0xC, &text);
    cBXString__cBXString(&sender, 2);
    cBXString__cBXString(&text, 2);
    return eventCopy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_NewsEvent__NewsEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

/**
 * Destroys a news event (vtable online_NewsEvent_vtbl): destroys its cBXString (+0x8), restores the base event vtable online_Event_vtbl
 * at +0x4 and frees the object when bit 0 of @p flags is set.
 *
 * Slot 1 of online_NewsEvent_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void online_NewsEvent__NewsEvent(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_NewsEvent_clone);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* online_NewsEvent_vtbl[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* s);

/**
 * Copies a news event: allocates 0xC bytes tagged "NetEvent" with kind 226, vtable online_NewsEvent_vtbl and a cBXString built
 * from @p self's string (+0x8).
 *
 * Slot 2 of online_NewsEvent_vtbl.
 *
 * @return the new event
 */
extern "C" void* online_NewsEvent_clone(void* self)
{
    int* copy = (int*)cMemMan_alloc(0xC, str_NetEvent, 0x20000000, 0);
    const char* newsText = *(const char**)((char*)self + 0x8);
    *(void***)((char*)copy + 0x4) = online_NewsEvent_vtbl;
    copy[0] = 226;
    cBXString_cBXString2((char*)copy + 0x8, newsText);
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_AcctTooYoungEvent__AcctTooYoungEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys an account-too-young event (vtable online_AcctTooYoungEvent_vtbl): restores the base event vtable online_Event_vtbl at +0x4 and frees
 * the object when bit 0 of @p flags is set.
 *
 * Slot 1 of online_AcctTooYoungEvent_vtbl, the vtable of the kind-258 events that LobbyMan_onaccountcreated posts for the 'tooy' account error, with
 * the reply's "AGE" number at +0xC.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void online_AcctTooYoungEvent__AcctTooYoungEvent(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_AcctTooYoungEvent_clone);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* online_AcctTooYoungEvent_vtbl[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

/**
 * Copies an account-too-young event: allocates 0x10 bytes tagged "NetEvent" with kind 258, error code 'tooy' (+0x8),
 * vtable online_AcctTooYoungEvent_vtbl and @p self's age (+0xC).
 *
 * Slot 2 of online_AcctTooYoungEvent_vtbl. The error code is written as the constant, not copied.
 *
 * @return the new event
 */
extern "C" void* online_AcctTooYoungEvent_clone(void* self)
{
    int* copy = (int*)cMemMan_alloc(0x10, str_NetEvent, 0x20000000, 0);
    int age = *(int*)((char*)self + 0xC);
    copy[0] = 258;
    copy[2] = 0x746F6F79;
    *(void***)((char*)copy + 0x4) = online_AcctTooYoungEvent_vtbl;
    copy[3] = age;
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_AcctLockedEvent__AcctLockedEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

/**
 * Destroys the account-locked error event (an online::Event of type 258 with the code 'lock').
 *
 * Destroys the two cBXStrings at +0x10 (the reply's INTRO text) and +0xC (its DATE text), restores the online::Event
 * base vtable online_Event_vtbl at +0x4 and calls operator_delete when bit 0 of flags is set. LobbyMan's LobbyMan_onauthentication posts
 * this event when the login reply's status is 'lock'.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void online_AcctLockedEvent__AcctLockedEvent(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0x10, 2);
    cBXString__cBXString((char*)self + 0xC, 2);
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_AcctLockedEvent_clone);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern char str_NetEvent[];
extern char online_AcctLockedEvent_vtbl[];

struct sLockMsg268308 {
    int type;       // 0x00
    char* name;     // 0x04
    int tag;        // 0x08
    char a[4];      // 0x0C
    char b[4];      // 0x10
};

static inline void Init268308(sLockMsg268308* m, const char* a, const char* b)
{
    m->type = 0x102;
    m->tag = 0x6C6F636B;
    m->name = online_AcctLockedEvent_vtbl;
    cBXString_cBXString2(m->a, a);
    cBXString_cBXString2(m->b, b);
}

/**
 * Returns a heap copy of this account-locked error event (type 258, code 'lock').
 *
 * Allocates 20 bytes with the "NetEvent" tag, stores type 0x102 (258), code 'lock' (0x6C6F636B) at +0x8 and vtable
 * online_AcctLockedEvent_vtbl at +0x4, and copies the two strings at +0xC (DATE) and +0x10 (INTRO) with cBXString_cBXString2. The
 * matched code's view struct calls the vtable word at +0x4 `name`; it is the vtable.
 *
 * @return the new event
 */
// Builds a "lock" message carrying the two name strings of this request.
extern "C" void* online_AcctLockedEvent_clone(char* self)
{
    sLockMsg268308* lockMsg = (sLockMsg268308*)operator new(20, str_NetEvent, 0x20000000, 0);
    Init268308(lockMsg, *(const char**)(self + 0xC), *(const char**)(self + 0x10));
    return lockMsg;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268398);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

/**
 * Destroys the server-down error event (an online::Event of type 258 with the code 'down').
 *
 * Destroys the cBXString at +0xC (the server's message text), restores the online::Event base vtable online_Event_vtbl at +0x4
 * and calls operator_delete when bit 0 of flags is set. LobbyMan's update func_0025AC50 posts this event when the lobby
 * server reports 'down' while connecting.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void func_00268398(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0xC, 2);
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002683F0);
#ifdef SKIP_ASM
extern void* D_004811B0[];

/**
 * Returns a heap copy of this server-down error event (type 258, code 'down').
 *
 * Allocates 0x10 bytes with the "NetEvent" tag, stores type 258, code 'down' (0x646F776E) at +0x8 and vtable D_004811B0
 * at +0x4, and copies the message string at +0xC with cBXString_cBXString2.
 *
 * @return the new event
 */
extern "C" void* func_002683F0(void* self)
{
    int* eventCopy = (int*)cMemMan_alloc(0x10, str_NetEvent, 0x20000000, 0);
    const char* message = *(const char**)((char*)self + 0xC);
    eventCopy[2] = 0x646F776E;
    *(void***)((char*)eventCopy + 0x4) = D_004811B0;
    eventCopy[0] = 258;
    cBXString_cBXString2((char*)eventCopy + 0xC, message);
    return eventCopy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_FindPlayerEvent__FindPlayerEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

/**
 * Destroys the find-player result event (an online::Event of type 227).
 *
 * Destroys the cBXStrings at +0xC (the room name) and +0x8 (the player's persona name), restores the online::Event base
 * vtable online_Event_vtbl at +0x4 and calls operator_delete when bit 0 of flags is set. LobbyMan's find-player reply handler
 * LobbyMan_onPlayerFound posts this event.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void online_FindPlayerEvent__FindPlayerEvent(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0xC, 2);
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_FindPlayerEvent_clone);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern char str_NetEvent[];
extern void* online_FindPlayerEvent_vtbl[];

struct sBlk2684C0 {
    int v[5];
};

struct sMsg2684C0 {
    int type;           // 0x00
    void** vt;          // 0x04
    char a[4];          // 0x08
    char b[4];          // 0x0C
    int f10;            // 0x10
    int f14;            // 0x14
    sBlk2684C0 blk;     // 0x18
};

static inline void Init2684C0(sMsg2684C0* m, const char* a, const char* b, int c, int d)
{
    m->type = 0xE3;
    m->vt = online_FindPlayerEvent_vtbl;
    cBXString_cBXString2(m->a, a);
    cBXString_cBXString2(m->b, b);
    m->f10 = c;
    m->f14 = d;
}

/**
 * Returns a heap copy of this find-player result event (type 227).
 *
 * Allocates 44 bytes with the "NetEvent" tag and fills it like LobbyMan_onPlayerFound does: type 0xE3, vtable online_FindPlayerEvent_vtbl, copies
 * of the persona name (+0x8) and room name (+0xC) strings, the online flag (+0x10) and the ADDR number (+0x14), then
 * copies the 20-byte stats record at +0x18 (RANK and STAT fields, laid out like tLobbyStats).
 *
 * @param src  the event to copy
 * @return the new event
 */
// Clones a 0xE3 message (strings, ids and trailing data block).
extern "C" void* online_FindPlayerEvent_clone(sMsg2684C0* src)
{
    sMsg2684C0* copy = (sMsg2684C0*)operator new(44, str_NetEvent, 0x20000000, 0);
    Init2684C0(copy, *(const char**)src->a, *(const char**)src->b, src->f10, src->f14);
    copy->blk = src->blk;
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_PingEvent__PingEvent);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);

/**
 * Destroys the ping result event (an online::Event of type 229).
 *
 * Destroys the cBXString at +0x8 (the pinged user's name), restores the online::Event base vtable online_Event_vtbl at +0x4
 * and calls operator_delete when bit 0 of flags is set. LobbyMan's LobbyMan_onPingReceived posts this event with the reply's NAME
 * and PING (32656 when the reply carried an error).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void online_PingEvent__PingEvent(void* self, int flags)
{
    cBXString__cBXString((char*)self + 0x8, 2);
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", online_PingEvent_clone);
#ifdef SKIP_ASM
extern void* online_PingEvent_vtbl[];

/**
 * Returns a heap copy of this ping result event (type 229).
 *
 * Allocates 0x10 bytes with the "NetEvent" tag, stores type 229 and vtable online_PingEvent_vtbl, copies the name string at +0x8
 * with cBXString_cBXString2 and the ping value at +0xC.
 *
 * @return the new event
 */
extern "C" void* online_PingEvent_clone(void* self)
{
    int* copy = (int*)cMemMan_alloc(0x10, str_NetEvent, 0x20000000, 0);
    int ping = *(int*)((char*)self + 0xC);
    const char* name = *(const char**)((char*)self + 0x8);
    *(void***)((char*)copy + 0x4) = online_PingEvent_vtbl;
    copy[0] = 229;
    cBXString_cBXString2((char*)copy + 0x8, name);
    copy[3] = ping;
    return copy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268658);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys a race-setup update event (an online::Event of type 251, vtable D_00481150).
 *
 * The event holds no strings, so this only restores the online::Event base vtable online_Event_vtbl at +0x4 and calls
 * operator_delete when bit 0 of flags is set. The setup manager posts these events with two words at +0x8/+0xC
 * (func_00265458; the senders in bx/bxstringctor.cpp such as func_00269478).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268658(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268688);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* D_00481150[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

/**
 * Returns a heap copy of this race-setup update event (type 251).
 *
 * Allocates 0x10 bytes with the "NetEvent" tag, stores type 251 and vtable D_00481150 and copies the two words at +0x8
 * and +0xC.
 *
 * @return the new event
 */
extern "C" void* func_00268688(void* self)
{
    int* eventCopy = (int*)cMemMan_alloc(0x10, str_NetEvent, 0x20000000, 0);
    int rowId = *(int*)((char*)self + 0x8);
    int flagWord = *(int*)((char*)self + 0xC);
    eventCopy[0] = 251;
    *(void***)((char*)eventCopy + 0x4) = D_00481150;
    eventCopy[2] = rowId;
    eventCopy[3] = flagWord;
    return eventCopy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002686F0);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys a race-setup update event with three values (an online::Event of type 252, vtable D_00481130).
 *
 * The event holds no strings, so this only restores the online::Event base vtable online_Event_vtbl at +0x4 and calls
 * operator_delete when bit 0 of flags is set. The setup manager's func_00265568 and the bx/bxstringctor.cpp senders
 * post these events (words at +0x8, +0xC, +0x10).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_002686F0(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268720);
#ifdef SKIP_ASM
extern void* D_00481130[];

/**
 * Returns a heap copy of this three-value race-setup update event (type 252).
 *
 * Allocates 20 bytes with the "NetEvent" tag, stores type 0xFC and vtable D_00481130 and copies the words at +0x8, +0xC
 * and +0x10.
 *
 * @return the new event
 */
extern "C" void* func_00268720(void* self)
{
    int* eventCopy = (int*)cMemMan_alloc(20, str_NetEvent, 0x20000000, 0);
    int ruleSlot = *(int*)((char*)self + 0x8);
    int ruleValue = *(int*)((char*)self + 0xC);
    int flagWord = *(int*)((char*)self + 0x10);
    eventCopy[0] = 0xFC;
    *(void***)((char*)eventCopy + 0x4) = D_00481130;
    eventCopy[2] = ruleSlot;
    eventCopy[3] = ruleValue;
    eventCopy[4] = flagWord;
    return eventCopy;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268790);
#ifdef SKIP_ASM
extern void* online_Event_vtbl[];
void operator_delete(int*);

/**
 * Destroys the user-record event (an online::Event of type 269, vtable D_00481110).
 *
 * Its names are inline char arrays, not cBXStrings, so this only restores the online::Event base vtable online_Event_vtbl at
 * +0x4 and calls operator_delete when bit 0 of flags is set. LobbyMan's reply handler func_0025EB00 posts this event.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268790(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = online_Event_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002687C0);
#ifdef SKIP_ASM
extern char str_NetEvent[];
extern void* D_00481110[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" char* strcpy(char* dst, const char* src);

struct sDsBlob14K2b {
    int v[5];
};

/**
 * Returns a heap copy of this user-record event (type 269, 356 bytes).
 *
 * Allocates 356 bytes with the "NetEvent" tag, stores type 269 and vtable D_00481110, strcpy's the three name fields
 * (+0x8 and +0x28, 0x20 bytes each; +0x48, 0x100 bytes: the reply's "M", "N" and "A" tags in func_0025EB00), copies the
 * numbers at +0x148/+0x14C and the 20-byte stats record at +0x150.
 *
 * @return the new event
 */
// Clone of a message of type 0x10D (three names, two ints, a 20-byte blob).
extern "C" void* func_002687C0(char* self)
{
    char* eventCopy = (char*)cMemMan_alloc(356, str_NetEvent, 0x20000000, 0);
    const char* tagN = self + 0x28;
    int x = *(int*)(self + 0x148);
    int y = *(int*)(self + 0x14C);
    const char* tagA;
    *(void***)(eventCopy + 0x4) = D_00481110;
    *(int*)eventCopy = 269;
    tagA = self + 0x48;
    *(int*)(eventCopy + 0x148) = x;
    *(int*)(eventCopy + 0x14C) = y;
    strcpy(eventCopy + 0x8, self + 0x8);
    strcpy(eventCopy + 0x28, tagN);
    strcpy(eventCopy + 0x48, tagA);
    *(sDsBlob14K2b*)(eventCopy + 0x150) = *(sDsBlob14K2b*)(self + 0x150);
    return eventCopy;
}
#endif

