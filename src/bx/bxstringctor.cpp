#include "common.h"
#include "online/privatemessage.h"
#include "online/setupman.h"
#include "path/trackpath.h"
#include "online/buddyman.h"
#include "path/aipath.h"
#include "path/basepath.h"
#include "path/pathsys.h"
#include "online/lobbyman.h"

extern void* cBXString_EmptyString;

struct cBXString2 {
    void* field_0x0;
    void* field_0x4;
    void* field_0x8;
    void* field_0xC;
    void* field_0x10;
    void* arr[4]; // 0x14-0x23
};

//100%
INCLUDE_ASM("bx/bxstringctor", cBXString_cBXString__FP10cBXString2);
#ifdef SKIP_ASM
struct cBXStrK2 {
    void* str;
};

/**
 * Initialises nine consecutive cBXString members to the shared empty string.
 *
 * Points five single strings and a four-entry string array at cBXString_EmptyString, the shared empty-string pointer. The
 * lobby manager's constructor runs it on two such blocks (offsets 0x260 and 0x2B0).
 *
 * @return @p self
 */
cBXString2* cBXString_cBXString(cBXString2* self)
{
    self->field_0x0 = cBXString_EmptyString;
    self->field_0x4 = cBXString_EmptyString;
    self->field_0x8 = cBXString_EmptyString;
    self->field_0xC = cBXString_EmptyString;
    self->field_0x10 = cBXString_EmptyString;
    cBXStrK2* entry = (cBXStrK2*)self->arr;
    for (int i = 3; i != -1; i--, entry++) {
        entry->str = cBXString_EmptyString;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPrivateMessage__cPrivateMessage1);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);
extern char cPrivateMessage_vtbl[];

/**
 * Destroys a private message: restores the vtable cPrivateMessage_vtbl at +0xC, destroys its three cBXStrings and frees it when
 * asked.
 *
 * Same body as cPrivateMessage__cPrivateMessage, which is the destructor slot of cPrivateMessage_vtbl; this copy is referenced by
 * nothing.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPrivateMessage__cPrivateMessage1(cPrivateMessage* self, int flags)
{
    self->mVtbl = (void**)cPrivateMessage_vtbl;
    cBXString__cBXString(&self->mDate, 2);
    cBXString__cBXString(&self->mText, 2);
    cBXString__cBXString(self, 2);
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cChallenger__cChallenger);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);
extern char cChallenger_vtbl[];

/**
 * Destroys a pending challenge: restores its vtable cChallenger_vtbl at +0x3C, destroys its two cBXStrings and frees it when
 * asked.
 *
 * The strings at +0x0 and +0x4 hold the challenger's name and message text. It is slot 1 of cChallenger_vtbl, the vtable the
 * challenge handler stores in each 0x40-byte object tagged "cChallenger" (BuddyMan's mChallengers).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cChallenger__cChallenger(cChallenger* self, int flags)
{
    self->mVtbl = (void**)cChallenger_vtbl;
    cBXString__cBXString(&self->mMessage, 2);
    cBXString__cBXString(self, 2);
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", BuddyMan__BuddyMan);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cBXString__cBXString(void* self, int flags);
void operator_delete(int*);
void cMemMan_free(void*);

static inline void Dealloc89B8(void** p, int n)
{
    if (p != 0) {
        if (n == 1) {
            operator_delete((int*)p);
        } else {
            cMemMan_free(p);
        }
    }
}

struct sVec89B8 {
    int f0;
    void** mBegin;
    void** mEnd;
    void** mCap;
};


/**
 * Destroys the buddy manager: its resource string, then the storage of its two pointer vectors, and frees it when
 * asked.
 *
 * Destroys the cBXString mResource (+0xEC), then releases the buffers of mChallengers (+0xB8) and mMessages (+0xA8): a
 * one-element buffer goes to operator_delete, a larger one to cMemMan_free. The pointed-to objects are not destroyed
 * here. The teardown in tagsunk calls it on BuddyMan_mBuddyMan with flags 3.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void BuddyMan__BuddyMan(BuddyMan* self, int flags)
{
    cBXString__cBXString(((char*)&self->mResource), 2);
    sVec89B8* vb = &(*(sVec89B8*)&self->mChallengersAlloc);
    Dealloc89B8(vb->mBegin, (*(sVec89B8*)&self->mChallengersAlloc).mCap - (*(sVec89B8*)&self->mChallengersAlloc).mBegin);
    sVec89B8* va = &(*(sVec89B8*)&self->mMessagesAlloc);
    Dealloc89B8(va->mBegin, (*(sVec89B8*)&self->mMessagesAlloc).mCap - (*(sVec89B8*)&self->mMessagesAlloc).mBegin);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target
struct sShortPad16 { short v; int pad[3]; };
extern sShortPad16 D_004A2FB0;
extern "C" void LobbyMan_onroomlistupdate(int);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268A80);
#ifdef SKIP_ASM
/**
 * Returns the first word at @p wordPtr.
 *
 * Nothing references it.
 *
 * @param wordPtr  the pointer to read
 * @return the word at @p wordPtr
 */
extern "C" int func_00268A80(int *wordPtr) {
    return *wordPtr;
}
#endif
//100%
INCLUDE_ASM("bx/bxstringctor", func_00268A88);
#ifdef SKIP_ASM
/**
 * Returns the first word at @p wordPtr.
 *
 * Nothing references it. Same body as func_00268A80.
 *
 * @param wordPtr  the pointer to read
 * @return the word at @p wordPtr
 */
extern "C" int func_00268A88(int *wordPtr) {
    return *wordPtr;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268A90);
#ifdef SKIP_ASM
/**
 * Calls LobbyMan_onroomlistupdate on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A2FB0 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025A778 registers it as the callback of list 0 (rooms).
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268A90(void* self, void* message, int lobbyMan)
{
    LobbyMan_onroomlistupdate(lobbyMan + D_004A2FB0.v);
}
#endif

extern sShortPad16 D_004A2FB8;
extern "C" void LobbyMan_onuserlistupdate(int);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268AB8);
#ifdef SKIP_ASM
/**
 * Calls LobbyMan_onuserlistupdate on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A2FB8 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025A778 registers it as the callback of list 1 (users).
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268AB8(void* self, void* message, int lobbyMan)
{
    LobbyMan_onuserlistupdate(lobbyMan + D_004A2FB8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268AE0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A2FC0;
extern "C" void LobbyMan_onranklistupdate(int);

/**
 * Calls LobbyMan_onranklistupdate on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A2FC0 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025A778 (list 2, ranks) and func_0025E348 register it.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268AE0(void* self, void* message, int lobbyMan)
{
    LobbyMan_onranklistupdate(lobbyMan + D_004A2FC0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B08);
#ifdef SKIP_ASM
extern sShortPad16 D_004A2FF0;
void* LobbyMan_onincomingmsg(void* self);

/**
 * Calls LobbyMan_onincomingmsg on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A2FF0 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025A948 registers it as LobbyApi message callback 0.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268B08(void* self, void* message, char* lobbyMan)
{
    LobbyMan_onincomingmsg(lobbyMan + D_004A2FF0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B30);
#ifdef SKIP_ASM
extern sShortPad16 D_004A2FF8;
extern "C" void LobbyMan_onchatmsg(int);

/**
 * Calls LobbyMan_onchatmsg on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A2FF8 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025A948 registers it as LobbyApi message callback 1.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268B30(void* self, void* message, int lobbyMan)
{
    LobbyMan_onchatmsg(lobbyMan + D_004A2FF8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B58);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3000;
extern "C" void LobbyMan_onconnectionmsg(int);

/**
 * Calls LobbyMan_onconnectionmsg on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3000 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025A948 registers it as LobbyApi message callback 3.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268B58(void* self, void* message, int lobbyMan)
{
    LobbyMan_onconnectionmsg(lobbyMan + D_004A3000.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268B80);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3008;
extern "C" void LobbyMan_onplaymsg(int);

/**
 * Calls LobbyMan_onplaymsg on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3008 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025A948 registers it as LobbyApi message callback 4.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268B80(void* self, void* message, int lobbyMan)
{
    LobbyMan_onplaymsg(lobbyMan + D_004A3008.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268BA8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3020;
extern "C" void LobbyMan_onlobbyconnect(int);

/**
 * Calls LobbyMan_onlobbyconnect on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3020 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. The lobby connect code passes it as the reply callback of func_003E8260 (port
 * 11000).
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268BA8(void* self, void* message, int lobbyMan)
{
    LobbyMan_onlobbyconnect(lobbyMan + D_004A3020.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268BD0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3088;
extern "C" void LobbyMan_onaccountcreated(int);

/**
 * Calls LobbyMan_onaccountcreated on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3088 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. The account-create request passes it to func_0025B608 as its reply callback.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268BD0(void* self, void* message, int lobbyMan)
{
    LobbyMan_onaccountcreated(lobbyMan + D_004A3088.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268BF8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30A8;
extern "C" void LobbyMan_onaccountedited(int);

/**
 * Calls LobbyMan_onaccountedited on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A30A8 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. The account-edit request passes it to func_0025B608 as its reply callback.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268BF8(void* self, void* message, int lobbyMan)
{
    LobbyMan_onaccountedited(lobbyMan + D_004A30A8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C20);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30B0;
extern "C" void LobbyMan_onlostaccountrequestsent(int);

/**
 * Calls LobbyMan_onlostaccountrequestsent on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A30B0 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. The lost-account request passes it to func_0025B608 as its reply callback.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268C20(void* self, void* message, int lobbyMan)
{
    LobbyMan_onlostaccountrequestsent(lobbyMan + D_004A30B0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C48);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30B8;
extern "C" void LobbyMan_onlostpasswordrequestsent(int);

/**
 * Calls LobbyMan_onlostpasswordrequestsent on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A30B8 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268C48(void* self, void* message, int lobbyMan)
{
    LobbyMan_onlostpasswordrequestsent(lobbyMan + D_004A30B8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C70);
#ifdef SKIP_ASM
extern sShortPad16 D_004A30E8;
extern "C" void LobbyMan_onauthentication(int);

/**
 * Calls LobbyMan_onauthentication on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A30E8 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. The login request passes it to func_0025B608 as its reply callback.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268C70(void* self, void* message, int lobbyMan)
{
    LobbyMan_onauthentication(lobbyMan + D_004A30E8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268C98);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3118;
extern "C" void LobbyMan_onpersonaselected(int);

/**
 * Calls LobbyMan_onpersonaselected on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3118 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268C98(void* self, void* message, int lobbyMan)
{
    LobbyMan_onpersonaselected(lobbyMan + D_004A3118.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268CC0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3120;
extern "C" void func_0025D048(int);

/**
 * Calls func_0025D048 on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3120 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025D048 handles the reply to the persona's 'onln' stats request.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268CC0(void* self, void* message, int lobbyMan)
{
    func_0025D048(lobbyMan + D_004A3120.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268CE8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3138;
extern "C" void LobbyMan_onpersonacreated(int);

/**
 * Calls LobbyMan_onpersonacreated on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3138 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268CE8(void* self, void* message, int lobbyMan)
{
    LobbyMan_onpersonacreated(lobbyMan + D_004A3138.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D10);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3140;
extern "C" void LobbyMan_onpersonadeleted(int);

/**
 * Calls LobbyMan_onpersonadeleted on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3140 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268D10(void* self, void* message, int lobbyMan)
{
    LobbyMan_onpersonadeleted(lobbyMan + D_004A3140.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D38);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3148;
extern "C" void LobbyMan_onroomselected(int);

/**
 * Calls LobbyMan_onroomselected on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3148 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268D38(void* self, void* message, int lobbyMan)
{
    LobbyMan_onroomselected(lobbyMan + D_004A3148.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D60);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3158;
extern "C" void LobbyMan_onRoomCreated(int);

/**
 * Calls LobbyMan_onRoomCreated on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3158 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268D60(void* self, void* message, int lobbyMan)
{
    LobbyMan_onRoomCreated(lobbyMan + D_004A3158.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268D88);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3160;
extern "C" void LobbyMan_onPingReceived(int);

/**
 * Calls LobbyMan_onPingReceived on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3160 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268D88(void* self, void* message, int lobbyMan)
{
    LobbyMan_onPingReceived(lobbyMan + D_004A3160.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268DB0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3170;
extern "C" void LobbyMan_onAbuseReportReceived(int);

/**
 * Calls LobbyMan_onAbuseReportReceived on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3170 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268DB0(void* self, void* message, int lobbyMan)
{
    LobbyMan_onAbuseReportReceived(lobbyMan + D_004A3170.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268DD8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3178;
extern "C" void LobbyMan_onNewsRetrieved(int);

/**
 * Calls LobbyMan_onNewsRetrieved on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3178 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. The news request passes it to func_0025B608 as its reply callback.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268DD8(void* self, void* message, int lobbyMan)
{
    LobbyMan_onNewsRetrieved(lobbyMan + D_004A3178.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268E00);
#ifdef SKIP_ASM
extern sShortPad16 D_004A31B0;
extern "C" void LobbyMan_onRanklistRetrieved(int);

/**
 * Calls LobbyMan_onRanklistRetrieved on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A31B0 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268E00(void* self, void* message, int lobbyMan)
{
    LobbyMan_onRanklistRetrieved(lobbyMan + D_004A31B0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268E28);
#ifdef SKIP_ASM
extern sShortPad16 D_004A31B8;
extern "C" void LobbyMan_onPlayerFound(int);

/**
 * Calls LobbyMan_onPlayerFound on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A31B8 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268E28(void* self, void* message, int lobbyMan)
{
    LobbyMan_onPlayerFound(lobbyMan + D_004A31B8.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268E50);
#ifdef SKIP_ASM
extern sShortPad16 D_004A31D0;
extern "C" void func_0025EB00(int);

/**
 * Calls func_0025EB00 on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A31D0 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_0025EB00 handles the reply to another player's 'onln' info request.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268E50(void* self, void* message, int lobbyMan)
{
    func_0025EB00(lobbyMan + D_004A31D0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268E78);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3210;
extern "C" void LobbyMan_onQuickMatchResponse(int);

/**
 * Calls LobbyMan_onQuickMatchResponse on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3210 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268E78(void* self, void* message, int lobbyMan)
{
    LobbyMan_onQuickMatchResponse(lobbyMan + D_004A3210.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268EA0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3248;
extern "C" void LobbyMan_onchallengeresponse(int);

/**
 * Calls LobbyMan_onchallengeresponse on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3248 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268EA0(void* self, void* message, int lobbyMan)
{
    LobbyMan_onchallengeresponse(lobbyMan + D_004A3248.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268EC8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3268;
void func_0025FC20(void* self);

/**
 * Calls func_0025FC20 on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3268 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. The private 'mesg' request passes it to func_003E8B10 as its reply callback;
 * func_0025FC20 is empty.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268EC8(void* self, void* message, char* lobbyMan)
{
    func_0025FC20(lobbyMan + D_004A3268.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268EF0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3318;
extern "C" void LobbyMan_onresultsreportresponse(int);

/**
 * Calls LobbyMan_onresultsreportresponse on the lobby manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3318 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param lobbyMan  the lobby manager the callback was registered with
 */
extern "C" void func_00268EF0(void* self, void* message, int lobbyMan)
{
    LobbyMan_onresultsreportresponse(lobbyMan + D_004A3318.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F18);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3340;
extern "C" void BuddyMan_onconnect(int);

/**
 * Calls BuddyMan_onconnect on the buddy manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3340 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_00261248 (the buddy connect) registers it.
 *
 * @param message  the callback's message, passed through to the handler
 * @param buddyMan  the buddy manager the callback was registered with
 */
extern "C" void func_00268F18(void* self, void* message, int buddyMan)
{
    BuddyMan_onconnect(buddyMan + D_004A3340.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F40);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3348;
extern "C" void BuddyMan_onReceiveMessage(int);

/**
 * Calls BuddyMan_onReceiveMessage on the buddy manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3348 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. Registered when the BuddyApi handle is created (mpBuddyApi).
 *
 * @param message  the callback's message, passed through to the handler
 * @param buddyMan  the buddy manager the callback was registered with
 */
extern "C" void func_00268F40(void* self, void* message, int buddyMan)
{
    BuddyMan_onReceiveMessage(buddyMan + D_004A3348.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F68);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3350;
extern "C" void BuddyMan_onrosterupdate(int);

/**
 * Calls BuddyMan_onrosterupdate on the buddy manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3350 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. BuddyMan_onconnect registers it on the roster it creates.
 *
 * @param message  the callback's message, passed through to the handler
 * @param buddyMan  the buddy manager the callback was registered with
 */
extern "C" void func_00268F68(void* self, void* message, int buddyMan)
{
    BuddyMan_onrosterupdate(buddyMan + D_004A3350.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268F90);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3380;
extern "C" void BuddyMan_onBuddyDelete(int);

/**
 * Calls BuddyMan_onBuddyDelete on the buddy manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3380 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_00261EF8 and two other BuddyMan functions register it with the delete call.
 *
 * @param message  the callback's message, passed through to the handler
 * @param buddyMan  the buddy manager the callback was registered with
 */
extern "C" void func_00268F90(void* self, void* message, int buddyMan)
{
    BuddyMan_onBuddyDelete(buddyMan + D_004A3380.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268FB8);
#ifdef SKIP_ASM
extern sShortPad16 D_004A3388;
extern "C" void BuddyMan_onBuddyAdd(int);

/**
 * Calls BuddyMan_onBuddyAdd on the buddy manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A3388 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used. func_002620D8 registers it with the add call func_003F0E38.
 *
 * @param message  the callback's message, passed through to the handler
 * @param buddyMan  the buddy manager the callback was registered with
 */
extern "C" void func_00268FB8(void* self, void* message, int buddyMan)
{
    BuddyMan_onBuddyAdd(buddyMan + D_004A3388.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00268FE0);
#ifdef SKIP_ASM
extern sShortPad16 D_004A33A0;
void BuddyMan_onBuddySend(void* self);

/**
 * Calls BuddyMan_onBuddySend on the buddy manager passed as the third argument (callback thunk).
 *
 * A functor thunk: the third argument is the object registered with the callback; D_004A33A0 holds the short
 * this-adjustment added to it. The second argument passes through untouched in its register as the handler's second
 * argument, and the first is not used.
 *
 * @param message  the callback's message, passed through to the handler
 * @param buddyMan  the buddy manager the callback was registered with
 */
extern "C" void func_00268FE0(void* self, void* message, char* buddyMan)
{
    BuddyMan_onBuddySend(buddyMan + D_004A33A0.v);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269008);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_00264CF0(void* self, int a1);
extern const char str_NetEvent[];
extern const char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_00269008 {
    sMsgListNode_00269008* next;
    sMsgListNode_00269008* prev;
    void* data;
};

struct sMsgListIter_00269008 {
    sMsgListNode_00269008* node;
    sMsgListIter_00269008(sMsgListNode_00269008* x) : node(x) {}
    sMsgListIter_00269008(const sMsgListIter_00269008& x) : node(x.node) {}
};

static inline sMsgListIter_00269008 sMsgList_00269008_insert(sMsgListIter_00269008 pos, void* const& x)
{
    sMsgListNode_00269008* tmp = (sMsgListNode_00269008*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
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
 * Handles the host echoing the client's character pick: takes the host's state, applies the character and posts an
 * event.
 *
 * Stores @p hostState as the setup state (+0x0), applies @p riderId as this player's character (func_00264CF0) and pushes an 8-byte
 * online event of kind 243 (vtable online_Event_vtbl, tag "NetEvent") onto LobbyMan's mEventList. func_002642B8 calls
 * it on the client's setup manager (cSetupMan_mSetupMan) for a type-1 message while the state is below 7.
 *
 * @param riderId  the character id from the message
 * @param hostState  the host's setup state from the message
 */
extern "C" void func_00269008(cSetupMan* self, int riderId, int hostState)
{
    int kind = 243;
    *(int*)self = hostState;
    func_00264CF0(self, riderId);
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* netEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = kind;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    void* mp = netEvent;
    sMsgList_00269008_insert(((sMsgListNode_00269008*)lobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_002690C8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_00264CF0(void* self, int a1);
extern const char str_NetEvent[];
extern const char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_002690C8 {
    sMsgListNode_002690C8* next;
    sMsgListNode_002690C8* prev;
    void* data;
};

struct sMsgListIter_002690C8 {
    sMsgListNode_002690C8* node;
    sMsgListIter_002690C8(sMsgListNode_002690C8* x) : node(x) {}
    sMsgListIter_002690C8(const sMsgListIter_002690C8& x) : node(x.node) {}
};

static inline sMsgListIter_002690C8 sMsgList_002690C8_insert(sMsgListIter_002690C8 pos, void* const& x)
{
    sMsgListNode_002690C8* tmp = (sMsgListNode_002690C8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
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
 * Handles the host echoing the client's character confirmation: takes the host's state, applies the character and posts
 * an event.
 *
 * Stores @p hostState as the setup state (+0x0), applies @p riderId as this player's character (func_00264CF0) and pushes an 8-byte
 * online event of kind 244 (vtable online_Event_vtbl, tag "NetEvent") onto LobbyMan's mEventList. func_002642B8 calls
 * it on the client's setup manager (cSetupMan_mSetupMan) for a type-3 message while the state is below 7.
 *
 * @param riderId  the character id from the message
 * @param hostState  the host's setup state from the message
 */
extern "C" void func_002690C8(cSetupMan* self, int riderId, int hostState)
{
    int kind = 244;
    *(int*)self = hostState;
    func_00264CF0(self, riderId);
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* netEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = kind;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    void* mp = netEvent;
    sMsgList_002690C8_insert(((sMsgListNode_002690C8*)lobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269188);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

extern const char str_NetEvent[];
extern const char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_00269188 {
    sMsgListNode_00269188* next;
    sMsgListNode_00269188* prev;
    void* data;
};

struct sMsgListIter_00269188 {
    sMsgListNode_00269188* node;
    sMsgListIter_00269188(sMsgListNode_00269188* x) : node(x) {}
    sMsgListIter_00269188(const sMsgListIter_00269188& x) : node(x.node) {}
};

static inline sMsgListIter_00269188 sMsgList_00269188_insert(sMsgListIter_00269188 pos, void* const& x)
{
    sMsgListNode_00269188* tmp = (sMsgListNode_00269188*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
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
 * Handles the host echoing the client's withdrawn confirmation: takes the host's state and posts an event.
 *
 * Stores @p hostState as the setup state (+0x0) and pushes an 8-byte online event of kind 247 (vtable online_Event_vtbl, tag
 * "NetEvent") onto LobbyMan's mEventList. func_002642B8 calls it on the client's setup manager (cSetupMan_mSetupMan) for a
 * type-5 message while the state is below 7.
 *
 * @param hostState  the host's setup state (the message's second word)
 */
extern "C" void func_00269188(cSetupMan* self, int hostState)
{
    int kind = 247;
    (self)->mState = hostState;
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* netEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = kind;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    void* mp = netEvent;
    sMsgList_00269188_insert(((sMsgListNode_00269188*)lobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269240);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_00264D80(void* self, int a1, void* a2);
extern const char str_NetEvent[];
extern const char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_00269240 {
    sMsgListNode_00269240* next;
    sMsgListNode_00269240* prev;
    void* data;
};

struct sMsgListIter_00269240 {
    sMsgListNode_00269240* node;
    sMsgListIter_00269240(sMsgListNode_00269240* x) : node(x) {}
    sMsgListIter_00269240(const sMsgListIter_00269240& x) : node(x.node) {}
};

static inline sMsgListIter_00269240 sMsgList_00269240_insert(sMsgListIter_00269240 pos, void* const& x)
{
    sMsgListNode_00269240* tmp = (sMsgListNode_00269240*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
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
 * Handles the host's character pick on the client: takes the host's state, records the opponent's character and posts
 * an event.
 *
 * Stores @p hostState as the setup state (+0x0), applies @p riderId and the attribute bytes @p riderStats as the opponent's character
 * (func_00264D80) and pushes an 8-byte online event of kind 243 (vtable online_Event_vtbl, tag "NetEvent") onto
 * LobbyMan's mEventList. func_002642B8 calls it on the client's setup manager (cSetupMan_mSetupMan) for a type-0 message while
 * the state is below 7.
 *
 * @param riderId  the host's character id
 * @param hostState  the host's setup state
 * @param riderStats  the host rider's 7 attribute bytes from the message
 */
extern "C" void func_00269240(cSetupMan* self, int riderId, int hostState, void* riderStats)
{
    int kind = 243;
    *(int*)self = hostState;
    func_00264D80(self, riderId, riderStats);
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* netEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = kind;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    void* mp = netEvent;
    sMsgList_00269240_insert(((sMsgListNode_00269240*)lobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269300);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_00264D80(void* self, int a1, void* a2);
extern const char str_NetEvent[];
extern const char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_00269300 {
    sMsgListNode_00269300* next;
    sMsgListNode_00269300* prev;
    void* data;
};

struct sMsgListIter_00269300 {
    sMsgListNode_00269300* node;
    sMsgListIter_00269300(sMsgListNode_00269300* x) : node(x) {}
    sMsgListIter_00269300(const sMsgListIter_00269300& x) : node(x.node) {}
};

static inline sMsgListIter_00269300 sMsgList_00269300_insert(sMsgListIter_00269300 pos, void* const& x)
{
    sMsgListNode_00269300* tmp = (sMsgListNode_00269300*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
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
 * Handles the host's character confirmation on the client: takes the host's state, records the opponent's character and
 * posts an event.
 *
 * Stores @p hostState as the setup state (+0x0), applies @p riderId as the opponent's character without attribute bytes
 * (func_00264D80) and pushes an 8-byte online event of kind 245 (vtable online_Event_vtbl, tag "NetEvent") onto
 * LobbyMan's mEventList. func_002642B8 calls it on the client's setup manager (cSetupMan_mSetupMan) for a type-2 message while
 * the state is below 7.
 *
 * @param riderId  the host's character id
 * @param hostState  the host's setup state
 */
extern "C" void func_00269300(cSetupMan* self, int riderId, int hostState)
{
    int kind = 245;
    *(int*)self = hostState;
    func_00264D80(self, riderId, 0);
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* netEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = kind;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    void* mp = netEvent;
    sMsgList_00269300_insert(((sMsgListNode_00269300*)lobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_002693C0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

extern const char str_NetEvent[];
extern const char str_StlBlock_b[];
extern void* online_Event_vtbl[];

struct sMsgListNode_002693C0 {
    sMsgListNode_002693C0* next;
    sMsgListNode_002693C0* prev;
    void* data;
};

struct sMsgListIter_002693C0 {
    sMsgListNode_002693C0* node;
    sMsgListIter_002693C0(sMsgListNode_002693C0* x) : node(x) {}
    sMsgListIter_002693C0(const sMsgListIter_002693C0& x) : node(x.node) {}
};

static inline sMsgListIter_002693C0 sMsgList_002693C0_insert(sMsgListIter_002693C0 pos, void* const& x)
{
    sMsgListNode_002693C0* tmp = (sMsgListNode_002693C0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
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
 * Handles the host withdrawing its confirmation on the client: takes the host's state and posts an event.
 *
 * Stores @p hostState as the setup state (+0x0) and pushes an 8-byte online event of kind 247 (vtable online_Event_vtbl, tag
 * "NetEvent") onto LobbyMan's mEventList. func_002642B8 calls it on the client's setup manager (cSetupMan_mSetupMan) for a
 * type-4 message while the state is below 7.
 *
 * @param hostState  the host's setup state (the message's second word)
 */
extern "C" void func_002693C0(cSetupMan* self, int hostState)
{
    int kind = 247;
    (self)->mState = hostState;
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    int* netEvent = (int*)cMemMan_alloc(8, str_NetEvent, 0x20000000, 0);
    netEvent[0] = kind;
    *(void***)((char*)netEvent + 0x4) = online_Event_vtbl;
    void* mp = netEvent;
    sMsgList_002693C0_insert(((sMsgListNode_002693C0*)lobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269478);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern void* D_00481150[];

struct sMsgListNode_00269478 {
    sMsgListNode_00269478* next;
    sMsgListNode_00269478* prev;
    void* data;
};

struct sMsgListIter_00269478 {
    sMsgListNode_00269478* node;
    sMsgListIter_00269478(sMsgListNode_00269478* x) : node(x) {}
    sMsgListIter_00269478(const sMsgListIter_00269478& x) : node(x.node) {}
};

static inline sMsgListIter_00269478 sMsgList_00269478_insert(sMsgListIter_00269478 pos, void* const& x)
{
    sMsgListNode_00269478* tmp = (sMsgListNode_00269478*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269478 {
    int kind;
    void** vt;
    int f8;
    int fC;
};

/**
 * Handles the client's highlighted bragging-rights row on the host: posts it as a lobby event.
 *
 * Pushes a 0x10-byte event of kind 251 (vtable D_00481150, tag "cSetupMan::Update"), the row at +0xC, with flag +0x8 =
 * 1 onto LobbyMan's mEventList. The local senders (func_00265458, func_00265568) post the same event with flag 0.
 * func_002642B8 calls it on the host's setup manager (cSetupMan_mSetupMan) for a type-7 message in the bragging-rights phase
 * (state 7 or more).
 *
 * @param rowIndex  the highlighted row from the message
 */
extern "C" void func_00269478(cSetupMan* self, int rowIndex)
{
    sMsg_00269478* netEvent = (sMsg_00269478*)cMemMan_alloc(0x10, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 251;
    netEvent->vt = D_00481150;
    netEvent->f8 = 1;
    netEvent->fC = rowIndex;
    void* mp = netEvent;
    sMsgList_00269478_insert(((sMsgListNode_00269478*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269538);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern void* D_00481130[];

struct sMsgListNode_00269538 {
    sMsgListNode_00269538* next;
    sMsgListNode_00269538* prev;
    void* data;
};

struct sMsgListIter_00269538 {
    sMsgListNode_00269538* node;
    sMsgListIter_00269538(sMsgListNode_00269538* x) : node(x) {}
    sMsgListIter_00269538(const sMsgListIter_00269538& x) : node(x.node) {}
};

static inline sMsgListIter_00269538 sMsgList_00269538_insert(sMsgListIter_00269538 pos, void* const& x)
{
    sMsgListNode_00269538* tmp = (sMsgListNode_00269538*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269538 {
    int kind;
    void** vt;
    int f8;
    int fC;
    int f10;
};

/**
 * Handles a bragging-rights rule change from the client on the host: posts it as a lobby event.
 *
 * Pushes a 0x14-byte event of kind 252 (vtable D_00481130, tag "cSetupMan::Update"), the rule slot at +0xC and the
 * value at +0x10, with flag +0x8 = 1 onto LobbyMan's mEventList. The local senders (func_00265458, func_00265568) post
 * the same event with flag 0. func_002642B8 calls it on the host's setup manager (cSetupMan_mSetupMan) for a type-9 message in
 * the bragging-rights phase (state 7 or more).
 *
 * @param ruleSlot  the rule slot
 * @param ruleValue  the new value
 */
extern "C" void func_00269538(cSetupMan* self, int ruleSlot, int ruleValue)
{
    sMsg_00269538* netEvent = (sMsg_00269538*)cMemMan_alloc(20, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 0xFC;
    netEvent->vt = D_00481130;
    netEvent->f8 = 1;
    netEvent->fC = ruleSlot;
    netEvent->f10 = ruleValue;
    void* mp = netEvent;
    sMsgList_00269538_insert(((sMsgListNode_00269538*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269608);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern "C" void func_00264E50(void* self, int event, int a2, int state);
extern const char str_StlBlock_b[];
extern char D_004813E0[];

struct sMsgListNode_00269608 {
    sMsgListNode_00269608* next;
    sMsgListNode_00269608* prev;
    void* data;
};

struct sMsgListIter_00269608 {
    sMsgListNode_00269608* node;
    sMsgListIter_00269608(sMsgListNode_00269608* x) : node(x) {}
    sMsgListIter_00269608(const sMsgListIter_00269608& x) : node(x.node) {}
};

static inline sMsgListIter_00269608 sMsgList_00269608_insert(sMsgListIter_00269608 pos, void* const& x)
{
    sMsgListNode_00269608* tmp = (sMsgListNode_00269608*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269608 {
    int kind;
    void* vt;
    int f8;
};

/**
 * Handles the client locking its bragging-rights panel on the host: advances the state, answers and posts an event.
 *
 * Moves the state word from 6 or 7 to 8, or from 9 to 10, sends a type-11 message carrying the new state
 * (func_00264E50) and pushes a 0xC-byte event of kind 253 (vtable D_004813E0, tag "cSetupMan::Update") with flag +0x8 =
 * 1 onto LobbyMan's mEventList. func_002642B8 calls it on the host's setup manager (cSetupMan_mSetupMan) for a type-11 message
 * in the bragging-rights phase (state 7 or more).
 */
extern "C" void func_00269608(cSetupMan* self)
{
    switch (*(int*)self)
    {
    case 6:
    case 7:
        *(int*)self = 8;
        break;
    case 9:
        *(int*)self = 10;
        break;
    }
    func_00264E50(self, 11, *(int*)self, 0);
    sMsg_00269608* netEvent = (sMsg_00269608*)cMemMan_alloc(0xC, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 253;
    netEvent->vt = D_004813E0;
    netEvent->f8 = 1;
    void* mp = netEvent;
    sMsgList_00269608_insert(((sMsgListNode_00269608*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_002696F8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern "C" void func_00264E50(void* self, int event, int a2, int state);
extern const char str_StlBlock_b[];
extern char D_004813C0[];

struct sMsgListNode_002696F8 {
    sMsgListNode_002696F8* next;
    sMsgListNode_002696F8* prev;
    void* data;
};

struct sMsgListIter_002696F8 {
    sMsgListNode_002696F8* node;
    sMsgListIter_002696F8(sMsgListNode_002696F8* x) : node(x) {}
    sMsgListIter_002696F8(const sMsgListIter_002696F8& x) : node(x.node) {}
};

static inline sMsgListIter_002696F8 sMsgList_002696F8_insert(sMsgListIter_002696F8 pos, void* const& x)
{
    sMsgListNode_002696F8* tmp = (sMsgListNode_002696F8*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_002696F8 {
    int kind;
    void* vt;
    int f8;
};

/**
 * Handles the client unlocking its bragging-rights panel on the host: steps the state back, answers and posts an event.
 *
 * Moves the state word from 8 back to 7, sends a type-12 message carrying the state (func_00264E50) and pushes a
 * 0xC-byte event of kind 254 (vtable D_004813C0, tag "cSetupMan::Update") with flag +0x8 = 1 onto LobbyMan's
 * mEventList. func_002642B8 calls it on the host's setup manager (cSetupMan_mSetupMan) for a type-13 message in the
 * bragging-rights phase (state 7 or more).
 */
extern "C" void func_002696F8(cSetupMan* self)
{
    if (*(int*)self == 8)
        *(int*)self = 7;
    func_00264E50(self, 0xC, *(int*)self, 0);
    sMsg_002696F8* netEvent = (sMsg_002696F8*)cMemMan_alloc(0xC, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 0xFE;
    netEvent->vt = D_004813C0;
    netEvent->f8 = 1;
    void* mp = netEvent;
    sMsgList_002696F8_insert(((sMsgListNode_002696F8*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_002697D0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern void* D_00481150[];

struct sMsgListNode_002697D0 {
    sMsgListNode_002697D0* next;
    sMsgListNode_002697D0* prev;
    void* data;
};

struct sMsgListIter_002697D0 {
    sMsgListNode_002697D0* node;
    sMsgListIter_002697D0(sMsgListNode_002697D0* x) : node(x) {}
    sMsgListIter_002697D0(const sMsgListIter_002697D0& x) : node(x.node) {}
};

static inline sMsgListIter_002697D0 sMsgList_002697D0_insert(sMsgListIter_002697D0 pos, void* const& x)
{
    sMsgListNode_002697D0* tmp = (sMsgListNode_002697D0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_002697D0 {
    int kind;
    void* vt;
    int f8;
    int fC;
};

/**
 * Handles the host's highlighted bragging-rights row on the client: posts it as a lobby event.
 *
 * Pushes a 0x10-byte event of kind 251 (vtable D_00481150, tag "cSetupMan::Update"), the row at +0xC, with flag +0x8 =
 * 1 onto LobbyMan's mEventList. The local senders (func_00265458, func_00265568) post the same event with flag 0.
 * func_002642B8 calls it on the client's setup manager (cSetupMan_mSetupMan) for a type-6 message in the bragging-rights phase
 * (state 7 or more).
 *
 * @param rowIndex  the highlighted row from the message
 */
extern "C" void func_002697D0(cSetupMan* self, int rowIndex)
{
    sMsg_002697D0* netEvent = (sMsg_002697D0*)cMemMan_alloc(0x10, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 251;
    netEvent->vt = D_00481150;
    netEvent->f8 = 1;
    netEvent->fC = rowIndex;
    void* mp = netEvent;
    sMsgList_002697D0_insert(((sMsgListNode_002697D0*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269890);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern void* D_00481150[];

struct sMsgListNode_00269890 {
    sMsgListNode_00269890* next;
    sMsgListNode_00269890* prev;
    void* data;
};

struct sMsgListIter_00269890 {
    sMsgListNode_00269890* node;
    sMsgListIter_00269890(sMsgListNode_00269890* x) : node(x) {}
    sMsgListIter_00269890(const sMsgListIter_00269890& x) : node(x.node) {}
};

static inline sMsgListIter_00269890 sMsgList_00269890_insert(sMsgListIter_00269890 pos, void* const& x)
{
    sMsgListNode_00269890* tmp = (sMsgListNode_00269890*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269890 {
    int kind;
    void* vt;
    int f8;
    int fC;
};

/**
 * Handles a type-7 highlighted-row message on the client: posts it as a lobby event with flag 0.
 *
 * Pushes a 0x10-byte event of kind 251 (vtable D_00481150, tag "cSetupMan::Update"), the row at +0xC, with flag +0x8 =
 * 0 onto LobbyMan's mEventList, like the local sender func_00265458. func_002642B8 calls it on the client's setup
 * manager (cSetupMan_mSetupMan) for a type-7 message in the bragging-rights phase (state 7 or more).
 *
 * @param rowIndex  the highlighted row from the message
 */
extern "C" void func_00269890(cSetupMan* self, int rowIndex)
{
    sMsg_00269890* netEvent = (sMsg_00269890*)cMemMan_alloc(0x10, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 251;
    netEvent->vt = D_00481150;
    netEvent->f8 = 0;
    netEvent->fC = rowIndex;
    void* mp = netEvent;
    sMsgList_00269890_insert(((sMsgListNode_00269890*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269948);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern void* D_00481130[];

struct sMsgListNode_00269948 {
    sMsgListNode_00269948* next;
    sMsgListNode_00269948* prev;
    void* data;
};

struct sMsgListIter_00269948 {
    sMsgListNode_00269948* node;
    sMsgListIter_00269948(sMsgListNode_00269948* x) : node(x) {}
    sMsgListIter_00269948(const sMsgListIter_00269948& x) : node(x.node) {}
};

static inline sMsgListIter_00269948 sMsgList_00269948_insert(sMsgListIter_00269948 pos, void* const& x)
{
    sMsgListNode_00269948* tmp = (sMsgListNode_00269948*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269948 {
    int kind;
    void* vt;
    int f8;
    int fC;
    int f10;
};

/**
 * Handles a bragging-rights rule change from the host on the client: posts it as a lobby event.
 *
 * Pushes a 0x14-byte event of kind 252 (vtable D_00481130, tag "cSetupMan::Update"), the rule slot at +0xC and the
 * value at +0x10, with flag +0x8 = 1 onto LobbyMan's mEventList. The local senders (func_00265458, func_00265568) post
 * the same event with flag 0. func_002642B8 calls it on the client's setup manager (cSetupMan_mSetupMan) for a type-8 message in
 * the bragging-rights phase (state 7 or more).
 *
 * @param ruleSlot  the rule slot
 * @param ruleValue  the new value
 */
extern "C" void func_00269948(cSetupMan* self, int ruleSlot, int ruleValue)
{
    sMsg_00269948* netEvent = (sMsg_00269948*)cMemMan_alloc(20, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 0xFC;
    netEvent->vt = D_00481130;
    netEvent->f8 = 1;
    netEvent->fC = ruleSlot;
    netEvent->f10 = ruleValue;
    void* mp = netEvent;
    sMsgList_00269948_insert(((sMsgListNode_00269948*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269A18);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern void* D_00481130[];

struct sMsgListNode_00269A18 {
    sMsgListNode_00269A18* next;
    sMsgListNode_00269A18* prev;
    void* data;
};

struct sMsgListIter_00269A18 {
    sMsgListNode_00269A18* node;
    sMsgListIter_00269A18(sMsgListNode_00269A18* x) : node(x) {}
    sMsgListIter_00269A18(const sMsgListIter_00269A18& x) : node(x.node) {}
};

static inline sMsgListIter_00269A18 sMsgList_00269A18_insert(sMsgListIter_00269A18 pos, void* const& x)
{
    sMsgListNode_00269A18* tmp = (sMsgListNode_00269A18*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269A18 {
    int kind;
    void* vt;
    int f8;
    int fC;
    int f10;
};

/**
 * Handles a type-9 rule-change message on the client: posts it as a lobby event with flag 0.
 *
 * Pushes a 0x14-byte event of kind 252 (vtable D_00481130, tag "cSetupMan::Update"), the rule slot at +0xC and the
 * value at +0x10, with flag +0x8 = 0 onto LobbyMan's mEventList, like the local sender func_00265568. func_002642B8
 * calls it on the client's setup manager (cSetupMan_mSetupMan) for a type-9 message in the bragging-rights phase (state 7 or
 * more).
 *
 * @param ruleSlot  the rule slot
 * @param ruleValue  the new value
 */
extern "C" void func_00269A18(cSetupMan* self, int ruleSlot, int ruleValue)
{
    sMsg_00269A18* netEvent = (sMsg_00269A18*)cMemMan_alloc(20, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 0xFC;
    netEvent->vt = D_00481130;
    netEvent->f8 = 0;
    netEvent->fC = ruleSlot;
    netEvent->f10 = ruleValue;
    void* mp = netEvent;
    sMsgList_00269A18_insert(((sMsgListNode_00269A18*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269AE0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern char D_004813E0[];

struct sMsgListNode_00269AE0 {
    sMsgListNode_00269AE0* next;
    sMsgListNode_00269AE0* prev;
    void* data;
};

struct sMsgListIter_00269AE0 {
    sMsgListNode_00269AE0* node;
    sMsgListIter_00269AE0(sMsgListNode_00269AE0* x) : node(x) {}
    sMsgListIter_00269AE0(const sMsgListIter_00269AE0& x) : node(x.node) {}
};

static inline sMsgListIter_00269AE0 sMsgList_00269AE0_insert(sMsgListIter_00269AE0 pos, void* const& x)
{
    sMsgListNode_00269AE0* tmp = (sMsgListNode_00269AE0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269AE0 {
    int kind;
    void* vt;
    int f8;
};

/**
 * Handles the host locking its bragging-rights panel on the client: takes the host's state and posts an event.
 *
 * Stores @p hostState as the setup state (+0x0) and pushes a 0xC-byte event of kind 253 (vtable D_004813E0, tag
 * "cSetupMan::Update") with flag +0x8 = 1 onto LobbyMan's mEventList. func_002642B8 calls it on the client's setup
 * manager (cSetupMan_mSetupMan) for a type-10 message in the bragging-rights phase (state 7 or more).
 *
 * @param hostState  the host's setup state from the message
 */
extern "C" void func_00269AE0(cSetupMan* self, int hostState)
{
    *(int*)self = hostState;
    sMsg_00269AE0* netEvent = (sMsg_00269AE0*)cMemMan_alloc(0xC, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 253;
    netEvent->vt = D_004813E0;
    netEvent->f8 = 1;
    void* mp = netEvent;
    sMsgList_00269AE0_insert(((sMsgListNode_00269AE0*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269B90);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern char D_004813E0[];

struct sMsgListNode_00269B90 {
    sMsgListNode_00269B90* next;
    sMsgListNode_00269B90* prev;
    void* data;
};

struct sMsgListIter_00269B90 {
    sMsgListNode_00269B90* node;
    sMsgListIter_00269B90(sMsgListNode_00269B90* x) : node(x) {}
    sMsgListIter_00269B90(const sMsgListIter_00269B90& x) : node(x.node) {}
};

static inline sMsgListIter_00269B90 sMsgList_00269B90_insert(sMsgListIter_00269B90 pos, void* const& x)
{
    sMsgListNode_00269B90* tmp = (sMsgListNode_00269B90*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269B90 {
    int kind;
    void* vt;
    int f8;
};

/**
 * Handles the host's answer to the client's lock (type 11, sent by func_00269608): takes the host's state and posts an
 * event.
 *
 * Stores @p hostState as the setup state (+0x0) and pushes a 0xC-byte event of kind 253 (vtable D_004813E0, tag
 * "cSetupMan::Update") with flag +0x8 = 0 onto LobbyMan's mEventList. func_002642B8 calls it on the client's setup
 * manager (cSetupMan_mSetupMan) for a type-11 message in the bragging-rights phase (state 7 or more).
 *
 * @param hostState  the host's setup state from the message
 */
extern "C" void func_00269B90(cSetupMan* self, int hostState)
{
    *(int*)self = hostState;
    sMsg_00269B90* netEvent = (sMsg_00269B90*)cMemMan_alloc(0xC, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 253;
    netEvent->vt = D_004813E0;
    netEvent->f8 = 0;
    void* mp = netEvent;
    sMsgList_00269B90_insert(((sMsgListNode_00269B90*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269C40);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern char D_004813C0[];

struct sMsgListNode_00269C40 {
    sMsgListNode_00269C40* next;
    sMsgListNode_00269C40* prev;
    void* data;
};

struct sMsgListIter_00269C40 {
    sMsgListNode_00269C40* node;
    sMsgListIter_00269C40(sMsgListNode_00269C40* x) : node(x) {}
    sMsgListIter_00269C40(const sMsgListIter_00269C40& x) : node(x.node) {}
};

static inline sMsgListIter_00269C40 sMsgList_00269C40_insert(sMsgListIter_00269C40 pos, void* const& x)
{
    sMsgListNode_00269C40* tmp = (sMsgListNode_00269C40*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269C40 {
    int kind;
    void* vt;
    int f8;
};

/**
 * Handles a type-12 unlock message from the host on the client: takes the host's state and posts an event.
 *
 * Type 12 comes from the host unlocking its own panel (func_00265700) or answering the client's unlock (func_002696F8).
 * Stores @p hostState as the setup state (+0x0) and pushes a 0xC-byte event of kind 254 (vtable D_004813C0, tag
 * "cSetupMan::Update") with flag +0x8 = 1 onto LobbyMan's mEventList. func_002642B8 calls it on the client's setup
 * manager (cSetupMan_mSetupMan) for a type-12 message in the bragging-rights phase (state 7 or more).
 *
 * @param hostState  the host's setup state from the message
 */
extern "C" void func_00269C40(cSetupMan* self, int hostState)
{
    *(int*)self = hostState;
    sMsg_00269C40* netEvent = (sMsg_00269C40*)cMemMan_alloc(0xC, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 0xFE;
    netEvent->vt = D_004813C0;
    netEvent->f8 = 1;
    void* mp = netEvent;
    sMsgList_00269C40_insert(((sMsgListNode_00269C40*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269CF0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_cSetupMan_Update[];
extern const char str_StlBlock_b[];
extern char D_004813C0[];

struct sMsgListNode_00269CF0 {
    sMsgListNode_00269CF0* next;
    sMsgListNode_00269CF0* prev;
    void* data;
};

struct sMsgListIter_00269CF0 {
    sMsgListNode_00269CF0* node;
    sMsgListIter_00269CF0(sMsgListNode_00269CF0* x) : node(x) {}
    sMsgListIter_00269CF0(const sMsgListIter_00269CF0& x) : node(x.node) {}
};

static inline sMsgListIter_00269CF0 sMsgList_00269CF0_insert(sMsgListIter_00269CF0 pos, void* const& x)
{
    sMsgListNode_00269CF0* tmp = (sMsgListNode_00269CF0*)operator new(0xC, str_StlBlock_b, 0x20000000, 0);
    void** data = &tmp->data;
    if (data != 0)
        *data = x;
    tmp->next = pos.node;
    tmp->prev = pos.node->prev;
    pos.node->prev->next = tmp;
    pos.node->prev = tmp;
    return tmp;
}

struct sMsg_00269CF0 {
    int kind;
    void* vt;
    int f8;
};

/**
 * Handles a type-13 unlock message on the client: takes the state from it and posts an event with flag 0.
 *
 * Stores @p hostState as the setup state (+0x0) and pushes a 0xC-byte event of kind 254 (vtable D_004813C0, tag
 * "cSetupMan::Update") with flag +0x8 = 0 onto LobbyMan's mEventList. func_002642B8 calls it on the client's setup
 * manager (cSetupMan_mSetupMan) for a type-13 message in the bragging-rights phase (state 7 or more).
 *
 * @param hostState  the setup state from the message
 */
extern "C" void func_00269CF0(cSetupMan* self, int hostState)
{
    *(int*)self = hostState;
    sMsg_00269CF0* netEvent = (sMsg_00269CF0*)cMemMan_alloc(0xC, str_cSetupMan_Update, 0x20000000, 0);
    netEvent->kind = 0xFE;
    netEvent->vt = D_004813C0;
    netEvent->f8 = 0;
    void* mp = netEvent;
    sMsgList_00269CF0_insert(((sMsgListNode_00269CF0*)LobbyMan_mLobbyMan->mEventList.mpHead), mp);
}
#endif

extern "C" void* online_Event__Event(void* self);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269DA0__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a bragging-rights lock event (kind 253) through the base online event destructor online_Event__Event.
 *
 * Slot 1 of D_004813E0. The destructor flags pass through untouched in the second argument register.
 *
 * @return no meaningful value (online_Event__Event returns nothing)
 */
void* func_00269DA0(void* self)
{
    return online_Event__Event(self);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269DC0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_NetEvent[];
extern char D_004813E0[];

struct sNode00269DC0 {
    int kind;
    void* vtbl;
    int value;
};

/**
 * Returns a heap copy of a bragging-rights lock event (kind 253, vtable D_004813E0).
 *
 * Allocates 0xC bytes with the "NetEvent" tag and copies the flag word at +0x8. Slot 2 of D_004813E0.
 *
 * @return the new event
 */
extern "C" sNode00269DC0* func_00269DC0(void* self)
{
    sNode00269DC0* n = (sNode00269DC0*)cMemMan_alloc(0xC, str_NetEvent, 0x20000000, 0);
    int flagWord = *(int*)((char*)self + 8);
    n->kind = 253;
    n->vtbl = D_004813E0;
    n->value = flagWord;
    return n;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269E20__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a bragging-rights unlock event (kind 254) through the base online event destructor online_Event__Event.
 *
 * Slot 1 of D_004813C0. The destructor flags pass through untouched in the second argument register.
 *
 * @return no meaningful value (online_Event__Event returns nothing)
 */
void* func_00269E20(void* self)
{
    return online_Event__Event(self);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269E40);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_NetEvent[];
extern char D_004813C0[];

struct sNode00269E40 {
    int kind;
    void* vtbl;
    int value;
};

/**
 * Returns a heap copy of a bragging-rights unlock event (kind 254, vtable D_004813C0).
 *
 * Allocates 0xC bytes with the "NetEvent" tag and copies the flag word at +0x8. Slot 2 of D_004813C0.
 *
 * @return the new event
 */
extern "C" sNode00269E40* func_00269E40(void* self)
{
    sNode00269E40* n = (sNode00269E40*)cMemMan_alloc(0xC, str_NetEvent, 0x20000000, 0);
    int flagWord = *(int*)((char*)self + 8);
    n->kind = 0xFE;
    n->vtbl = D_004813C0;
    n->value = flagWord;
    return n;
}
#endif

extern "C" void* func_00267468(int, int);

//100%
INCLUDE_ASM("bx/bxstringctor", func_00269EA0__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static constructor func_00267468 (initialize 1, priority 0xFFFF).
 *
 * The argument is not read.
 *
 * @return no meaningful value (func_00267468 returns nothing)
 */
void* func_00269EA0(void* self)
{
    return func_00267468(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cAIPath_populateDataWithDefaults__FPv);
#ifdef SKIP_ASM
/**
 * Gives an AI path its default attributes before the path file's data is read.
 *
 * Path attributes (+0x38) = 0x2E and the reset-path flag (+0x3C) = 1 (On Tour PathAttributes, IsResetPath);
 * cAIPath_populateData overwrites them from the file. The C returns the 0x2E it stores.
 *
 * @return 0x2E (the C's view; On Tour returns nothing)
 */
int cAIPath_populateDataWithDefaults(void* vself)
{
    cAIPath* self = (cAIPath*)vself;
    int pathAttributes = 46;
    self->PathAttributes = pathAttributes;
    self->IsResetPath = 1;
    return pathAttributes;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cAIPath_populateData);
#ifdef SKIP_ASM
extern "C" void pathREAD_tInt32(void* p);

/**
 * Reads one attribute of an AI path from the path file's data.
 *
 * Id 0x64 reads the path attributes (+0x38) and id 0x65 the reset-path flag (+0x3C) with pathREAD_tInt32
 * (pathREAD_tInt32); other ids are ignored. The C prototype of pathREAD_tInt32 leaves out the source pointer, which still
 * reaches it in $a1 from @p data.
 *
 * @param data  the field's data in the path file (On Tour p)
 * @param id  the field id: 0x64 or 0x65
 */
extern "C" void cAIPath_populateData(cAIPath* self, int data, int id)
{
    if (id == 100) {
        pathREAD_tInt32(&self->PathAttributes);
    } else if (id == 101) {
        pathREAD_tInt32(&self->IsResetPath);
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cAIPath_getResetPos);
#ifdef SKIP_ASM
struct sBXVec4K2 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sBXPath;
// sBXVec4 (defined later in the unit) has this layout.
sBXVec4K2 cPath_getPathPos_K2(sBXPath* self, int* inRange, float t) __asm__("cPath_getPathPos");

struct sBXRangeK2 {
    int type;   // 0x0
    int pad_0x4;
    float lo;   // 0x8
    float hi;   // 0xC
};

struct sBXRangeListK2 {
    int count;            // 0x0
    sBXRangeK2* entries;  // 0x4
};

/**
 * Finds where on an AI path a rider should be reset to, near a path position, and returns that point.
 *
 * Starting from *@p pathParam + 50 for both bounds, every range of the path's list that contains *@p pathParam adjusts them: type 16
 * raises the upper bound to its end, type 12 lowers the lower bound to its start, type 14 raises the lower bound to its
 * end. *@p pathParam becomes the lower bound unless that equals *@p pathParam, then the upper one, and cPath_getPathPos gives
 * the position.
 *
 * @param path  the AI path (this); starts with its range list
 * @param pathParam  in: the rider's path position; out: the reset position (On Tour param)
 * @return the reset point on the path
 */
// PORT: >? and <? (g++ min/max operators).
extern "C" sBXVec4K2 cAIPath_getResetPos(sBXPath* path, float* pathParam)
{
    sBXRangeListK2* list = (sBXRangeListK2*)path;
    float hi = *pathParam + 50.0f;
    float lo = hi;
    int i;
    for (i = 0; i < list->count; i++) {
        sBXRangeK2* pathEvent = &list->entries[i];
        if (*pathParam < pathEvent->lo)
            continue;
        if (pathEvent->hi < *pathParam)
            continue;
        if (pathEvent->type == 16)
            hi = pathEvent->hi >? hi;
        else if (pathEvent->type == 12)
            lo = pathEvent->lo <? lo;
        else if (pathEvent->type == 14)
            lo = pathEvent->hi >? lo;
    }
    if (lo != *pathParam)
        *pathParam = lo;
    else
        *pathParam = hi;
    return cPath_getPathPos_K2(path, 0, *pathParam);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cAIPath_isResetPermitted);
#ifdef SKIP_ASM
struct sEntry_0026A000 {
    int type;
    int pad4;
    float lo;
    float hi;
};
struct sTable_0026A000 {
    int count;
    sEntry_0026A000* entries;
};

/**
 * Returns whether a reset is allowed at position @p x along an AI path: 0 inside any event of type 13, else 1.
 *
 * Walks the path's events (count +0x0, 16-byte events at +0x4) for one of type 13 whose range [+0x8, +0xC] contains @p
 * x. Nothing in the C calls it.
 *
 * @param table  the AI path
 * @param x  the position along the path
 * @return 1 when a reset is allowed, else 0
 */
extern "C" int cAIPath_isResetPermitted(sTable_0026A000* table, float x) {
    int i;
    for (i = 0; i < table->count; i++) {
        sEntry_0026A000* entry = &table->entries[i];
        if (!(x < entry->lo) && !(entry->hi < x) && entry->type == 13)
            return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cTrackPath_populateDataWithDefaults__FPv);
#ifdef SKIP_ASM
/**
 * Gives a track path its default attribute before the path file's data is read.
 *
 * +0x38 = 0 (On Tour stores the float 0.0).
 */
void cTrackPath_populateDataWithDefaults(void* vself)
{
    cTrackPath* self = (cTrackPath*)vself;
    *(int*)&self->BaseDistanceLeft = 0;
}
#endif

extern "C" void pathREAD_float(void*);

//100%
INCLUDE_ASM("bx/bxstringctor", cTrackPath_populateData);
#ifdef SKIP_ASM
/**
 * Reads a track path's attribute from the path file's data.
 *
 * Only field id 0 is used: pathREAD_float (On Tour pathREAD_float) reads the base distance left (+0x38). As in
 * cAIPath_populateData, the source pointer reaches the reader in $a1 though the C prototype leaves it out.
 *
 * @param data  the field's data in the path file (On Tour p)
 * @param fieldId  the field id; only 0 is read
 */
extern "C" void cTrackPath_populateData(cTrackPath* self, int data, int fieldId)
{
    if (fieldId == 0) {
        pathREAD_float(&self->BaseDistanceLeft);
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cTrackPath_getEventsInRange);
#ifdef SKIP_ASM
struct sBXRange {
    int field_0x0;
    int field_0x4;
    float min; // 0x8
    float max; // 0xC
};

struct sBXRangeList {
    int count;          // 0x0
    sBXRange* items;    // 0x4
    char pad_0x08[0x30];
    float field_0x38;   // 0x38
};

extern "C" int cPath_getEventsInRange(sBXRangeList* self, sBXRange* out, int maxOut, float lo, float hi);

/**
 * Collects the track path's events between two distances from the finish.
 *
 * Calls cPath_getEventsInRange (cPath::getEventsInRange) with base - @p ldist and base - @p hdist, where base is the path's distance
 * left at its start (+0x38).
 *
 * @param out  receives the events (On Tour events)
 * @param maxOut  the most events to return (On Tour maxEvents)
 * @param ldist  the first distance from the finish (On Tour ldist)
 * @param hdist  the second distance from the finish (On Tour hdist)
 * @return the number of events written
 */
extern "C" int cTrackPath_getEventsInRange(cTrackPath* self, sBXRange* out, int maxOut, float ldist, float hdist)
{
    float base = self->BaseDistanceLeft;
    return cPath_getEventsInRange(((sBXRangeList*)self), out, maxOut, base - ldist, base - hdist);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cTrackPath_isForkInPath);
#ifdef SKIP_ASM
extern "C" float cPath_getLength(cPath* self);
extern "C" int cPath_isForkInPath(sBXRangeList* self, float lo, float hi);

/**
 * Tells whether the track path forks between two distances from the finish.
 *
 * Low = base (+0x38) - @p ldist, or 0 when @p ldist is negative; high = base - @p hdist, or cPath_getLength (the path length) when @p
 * b is negative; then cPath_isForkInPath (cPath::isForkInPath).
 *
 * @param ldist  the first distance from the finish; negative for the path start (On Tour ldist)
 * @param hdist  the second distance from the finish; negative for the path end (On Tour hdist)
 * @return nonzero when there is a fork in that stretch
 */
extern "C" int cTrackPath_isForkInPath(cTrackPath* self, float ldist, float hdist)
{
    float lo;
    float hi;
    if (ldist >= 0.0f)
        lo = self->BaseDistanceLeft - ldist;
    else
        lo = 0.0f;
    if (hdist < 0.0f)
        hi = cPath_getLength((cPath*)self);
    else
        hi = self->BaseDistanceLeft - hdist;
    return cPath_isForkInPath(((sBXRangeList*)self), lo, hi);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cTrackPath_getFinishLineParam);
#ifdef SKIP_ASM
struct sEntry_0026A138 {
    int type;
    int pad4;
    float value;
    int pad[1];
};
struct sTable_0026A138 {
    int count;
    sEntry_0026A138* entries;
};

/**
 * Returns the start position (+0x8) of a track path's first type-1 event, or -1 when it has none.
 *
 * Walks the path's events (count +0x0, 16-byte events at +0x4). The AI uses it to find the first track path that has
 * one.
 *
 * @param table  the track path
 * @return the position along the path, or -1.0
 */
extern "C" float cTrackPath_getFinishLineParam(sTable_0026A138* table)
{
    int i;
    for (i = 0; i < table->count; i++) {
        sEntry_0026A138* entry = &table->entries[i];
        if (entry->type == 1)
            return entry->value;
    }
    return -1.0f;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_initPath);
#ifdef SKIP_ASM
// PORT: pathREAD_tInt32 takes (dst, src); the unit declares it with one arg
void pathREAD_tInt32_A180(void* dst, void* src) __asm__("pathREAD_tInt32");
extern "C" void pathREAD_t3Vector(void* p, void* q);
extern "C" void cPath_populateNode(void* self, void* src, void* dst);
extern "C" void cPath_populateEvent(void* self, char* src, int* dst);

struct sVE0_26A180 { short delta; short index; void (*fn)(void*); };
struct sVE1_26A180 { short delta; short index; void (*fn)(void*, char*, int, int); };


/**
 * Initialises a path from its serialized data and returns how many bytes it read.
 *
 * Calls the path's virtual slot 3 (defaults), then for each of the data's chunks passes (data, id, size) to virtual
 * slot 4 and skips the chunk. Then reads the node count (+0x8) and event count (+0x0), the start point (+0xC) and the
 * bounds (+0x1C, +0x28) with pathREAD_t3Vector, and converts the 16-byte nodes (+0x18, cPath_populateNode) and events (+0x4,
 * cPath_populateEvent) in place in the data.
 *
 * The path-system loaders (cPathSys_loadTrackPaths, cPathSys_loadAIPaths) call it for each path.
 *
 * @param src  the serialized path data
 * @return the number of bytes read
 */
extern "C" int cPath_initPath(cPath* self, char* src)
{
    char* start = src;
    int n;
    int chunkId;
    int itemSize;
    int itemCount;
    sVE0_26A180* vt0 = (sVE0_26A180*)((void*)self->mVtbl);
    vt0[3].fn((char*)self + vt0[3].delta);
    pathREAD_tInt32_A180(&n, src);
    src += 4;
    for (int i = 0; i < n; i++)
    {
        pathREAD_tInt32_A180(&chunkId, src);
        src += 4;
        pathREAD_tInt32_A180(&itemSize, src);
        src += 4;
        sVE1_26A180* vt1 = (sVE1_26A180*)((void*)self->mVtbl);
        vt1[4].fn((char*)self + vt1[4].delta, src, chunkId, itemSize);
        src += itemSize;
    }
    pathREAD_tInt32_A180(&itemCount, src);
    src += 4;
    self->NumNodes = itemCount;
    pathREAD_tInt32_A180(&itemCount, src);
    src += 4;
    self->NumEvents = itemCount;
    pathREAD_t3Vector(((char*)&self->Start[0]), src);
    src += 0xC;
    pathREAD_t3Vector(((char*)&self->MinBound[0]), src);
    src += 0xC;
    pathREAD_t3Vector(((char*)&self->MaxBound[0]), src);
    src += 0xC;
    *(char**)&self->Nodes = src;
    for (int i = 0; i < self->NumNodes; i++)
    {
        cPath_populateNode(self, src, ((char*)self->Nodes) + i * 16);
        src += 16;
    }
    *(char**)&self->Events = src;
    for (int i = 0; i < self->NumEvents; i++)
    {
        cPath_populateEvent(self, src, (int*)(((char*)self->Events) + i * 16));
        src += 16;
    }
    return src - start;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_populateNode);
#ifdef SKIP_ASM
extern "C" void pathREAD_t3Vector(void* p, void* q);
// PORT: pathREAD_float takes (dst, src); the unit declares it with one arg
void pathREAD_float_2(void* dst, void* src) __asm__("pathREAD_float");

/**
 * Reads one 16-byte path node: its direction (pathREAD_t3Vector) and its length at +0xC (pathREAD_float).
 *
 * cPath_initPath calls it with the same address for @p src and @p dst, converting the node in place.
 *
 * @param src  the node in the serialized data
 * @param dst  the node to fill
 */
extern "C" void cPath_populateNode(void* self, void* src, void* dst)
{
    pathREAD_t3Vector(dst, src);
    pathREAD_float_2((char*)dst + 0xC, (char*)src + 0xC);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_populateEvent);
#ifdef SKIP_ASM
// PORT: pathREAD_tInt32 takes (dst, src); the unit declares it with one arg
void pathREAD_tInt32_K2(void* dst, void* src) __asm__("pathREAD_tInt32");
extern int D_004453D0[];

/**
 * Reads one 16-byte path event: its type code, argument and start and end positions.
 *
 * The type code is looked up in the 21 codes of D_004453D0 and stored as its index (0 when not found); then the
 * argument (+0x4) and the two floats (+0x8, +0xC, pathREAD_float) follow. cPath_initPath converts the events in place
 * with it.
 *
 * @param src  the event in the serialized data
 * @param dst  the event to fill
 */
extern "C" void cPath_populateEvent(void* self, char* src, int* dst)
{
    int key;
    int i;
    pathREAD_tInt32_K2(&key, src);
    src += 4;
    *dst = 0;
    for (i = 0; i < 21; i++) {
        if (D_004453D0[i] == key) {
            *dst = i;
            break;
        }
    }
    pathREAD_tInt32_K2(dst + 1, src);
    src += 4;
    pathREAD_float_2(dst + 2, src);
    pathREAD_float_2(dst + 3, src + 4);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_intersectPath);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sBXVec4_A428 {
    float x, y, z, w;
    sBXVec4_A428() {}
    sBXVec4_A428(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sBXPathSeg_A428 {
    float x, y, z;   // direction
    float len;       // 0xC
};


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sBXVec4_A428 bxVu0Sub_A428(const sBXVec4_A428& a, const sBXVec4_A428& b)
{
    sBXVec4_A428 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float bxVu0Dot_A428(const sBXVec4_A428& a, const sBXVec4_A428& b)
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
static inline sBXVec4_A428 bxVu0Scale_A428(const sBXVec4_A428& v, float s)
{
    sBXVec4_A428 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sBXVec4_A428 bxVu0Add_A428(const sBXVec4_A428& a, const sBXVec4_A428& b)
{
    sBXVec4_A428 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void bxVu0AddEq_A428(sBXVec4_A428& dst, sBXVec4_A428 b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2-only FPU square root (sqrt.s; __builtin_sqrtf emits the errno-checked call).
static inline float bxSqrt_A428(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: <? (g++ min operator).
static inline float bxClamp_A428(float v, float lo, float hi)
{
    return lo <= v ? v <? hi : lo;
}

/**
 * Finds the point of a path nearest to @p cursor and returns its position along the path.
 *
 * Walks the path's segments from its start (+0xC): for each node (+0x18, count +0x8) it projects @p cursor onto the segment,
 * clamped to its length, and keeps the closest point. With @p flat only X and Y count. The best point goes to @p outPos
 * and the horizontal (XY) distance to it, square-rooted, to @p outDist, even when the comparison was in 3D.
 *
 * The AI (ai/computer) uses it on track and AI paths, for example when the rider is in a pipe jump.
 *
 * @param cursor  the point to project
 * @param outPos  receives the nearest point on the path
 * @param outDist  receives the XY distance to it
 * @param flat  non-zero to measure in X and Y only
 * @return the position along the path of the nearest point
 */
extern "C" float cPath_intersectPath(cPath* self, const sBXVec4_A428& cursor, sBXVec4_A428* outPos,
                               float* outDist, int flat)
{
    float bestT = 0.0f;
    float bestDist = 0.0f;
    float along = 0.0f;
    float best = 1e37f;
    sBXVec4_A428 pos(self->Start[0], self->Start[1], self->Start[2], 1.0f);
    for (int i = 0; i < self->NumNodes; i++) {
        sBXVec4_A428 dir;
        dir.x = ((sBXPathSeg_A428*)self->Nodes)[i].x;
        dir.y = ((sBXPathSeg_A428*)self->Nodes)[i].y;
        dir.z = ((sBXPathSeg_A428*)self->Nodes)[i].z;
        dir.w = 0.0f;
        float len = ((sBXPathSeg_A428*)self->Nodes)[i].len;
        sBXVec4_A428 delta = bxVu0Sub_A428(cursor, pos);
        float projLen;
        if (flat)
            projLen = delta.x * dir.x + delta.y * dir.y;
        else
            projLen = bxVu0Dot_A428(delta, dir);
        float tc = bxClamp_A428(projLen, 0.0f, len);
        sBXVec4_A428 nearPoint = bxVu0Add_A428(pos, bxVu0Scale_A428(dir, tc));
        delta = bxVu0Sub_A428(cursor, nearPoint);
        float distSq;
        if (flat)
            distSq = delta.x * delta.x + delta.y * delta.y;
        else
            distSq = bxVu0Dot_A428(delta, delta);
        if (distSq < best) {
            best = distSq;
            if (flat)
                bestDist = distSq;
            else
                bestDist = delta.x * delta.x + delta.y * delta.y;
            bestT = along + tc;
            *outPos = nearPoint;
        }
        along += len;
        bxVu0AddEq_A428(pos, bxVu0Scale_A428(dir, len));
    }
    *outDist = bxSqrt_A428(bestDist);
    return bestT;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_intersectPath1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sBXVec4_A638 {
    float x, y, z, w;
    sBXVec4_A638() {}
    sBXVec4_A638(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sBXPathSeg_A638 {
    float x, y, z;   // direction
    float len;       // 0xC
};


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sBXVec4_A638 bxVu0Sub_A638(const sBXVec4_A638& a, const sBXVec4_A638& b)
{
    sBXVec4_A638 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float bxVu0Dot_A638(const sBXVec4_A638& a, const sBXVec4_A638& b)
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
static inline sBXVec4_A638 bxVu0Scale_A638(const sBXVec4_A638& v, float s)
{
    sBXVec4_A638 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sBXVec4_A638 bxVu0Add_A638(const sBXVec4_A638& a, const sBXVec4_A638& b)
{
    sBXVec4_A638 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void bxVu0AddEq_A638(sBXVec4_A638& dst, sBXVec4_A638 b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2-only FPU square root (sqrt.s; __builtin_sqrtf emits the errno-checked call).
static inline float bxSqrt_A638(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: <? (g++ min operator).
static inline float bxClamp_A638(float v, float lo, float hi)
{
    return lo <= v ? v <? hi : lo;
}

struct sBXPathCache_A638 {
    sBXVec4_A638 pos;  // 0x0
    float along;       // 0x10
    int idx;           // 0x14
};

/**
 * Finds the point of a path nearest to @p cursor, starting from a cached segment, and returns its position along the path.
 *
 * Walks the path's segments from its start (+0xC): for each node (+0x18, count +0x8) it projects @p cursor onto the segment,
 * clamped to its length, and keeps the closest point. With @p flat only X and Y count. The best point goes to @p outPos
 * and the horizontal (XY) distance to it, square-rooted, to @p outDist, even when the comparison was in 3D.
 *
 * When @p cache holds a segment (+0x14 >= 0) the walk starts there from its saved start point (+0x0) and distance
 * (+0x10), gives up 3000 units further on, and stops at the first improvement whose projection did not need clamping.
 * Each improvement is written back to @p cache. The AI calls it every update with the rider's path hint.
 *
 * @param cursor  the point to project
 * @param outPos  receives the nearest point on the path
 * @param outDist  receives the XY distance to it
 * @param cache  the search hint: start point, distance and segment index of the last result
 * @param flat  non-zero to measure in X and Y only
 * @return the position along the path of the nearest point
 */
extern "C" float cPath_intersectPath1(cPath* self, const sBXVec4_A638& cursor, sBXVec4_A638* outPos,
                               float* outDist, sBXPathCache_A638* cache, int flat)
{
    float bestT = 0.0f;
    float bestDist = 0.0f;
    float best = 1e37f;
    float along;
    float limit;
    int i;
    sBXVec4_A638 pos;
    int cached = cache->idx >= 0;
    if (cached) {
        along = cache->along;
        i = cache->idx;
        pos = cache->pos;
        limit = along + 3000.0f;
    } else {
        i = 0;
        along = 0.0f;
        limit = best;
        pos = sBXVec4_A638(self->Start[0], self->Start[1], self->Start[2], 1.0f);
    }
    for (; i < self->NumNodes; i++) {
        sBXVec4_A638 dir;
        dir.x = ((sBXPathSeg_A638*)self->Nodes)[i].x;
        dir.y = ((sBXPathSeg_A638*)self->Nodes)[i].y;
        dir.z = ((sBXPathSeg_A638*)self->Nodes)[i].z;
        dir.w = 0.0f;
        float len = ((sBXPathSeg_A638*)self->Nodes)[i].len;
        sBXVec4_A638 delta = bxVu0Sub_A638(cursor, pos);
        float projLen;
        if (flat)
            projLen = delta.x * dir.x + delta.y * dir.y;
        else
            projLen = bxVu0Dot_A638(delta, dir);
        float tc = bxClamp_A638(projLen, 0.0f, len);
        sBXVec4_A638 nearPoint = bxVu0Add_A638(pos, bxVu0Scale_A638(dir, tc));
        delta = bxVu0Sub_A638(cursor, nearPoint);
        float distSq;
        if (flat)
            distSq = delta.x * delta.x + delta.y * delta.y;
        else
            distSq = bxVu0Dot_A638(delta, delta);
        if (distSq < best) {
            best = distSq;
            if (flat)
                bestDist = distSq;
            else
                bestDist = delta.x * delta.x + delta.y * delta.y;
            bestT = along + tc;
            *outPos = nearPoint;
            cache->pos = pos;
            cache->along = along;
            cache->idx = i;
            if (cached && tc == projLen)
                break;
        }
        if (limit < along)
            break;
        along += len;
        bxVu0AddEq_A638(pos, bxVu0Scale_A638(dir, len));
    }
    *outDist = bxSqrt_A638(bestDist);
    return bestT;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_getPathHeading);
#ifdef SKIP_ASM
struct sBXSeg16 {
    float f0;
    float f4;
    float f8;
    float fC;
};

/**
 * Returns the direction of the path at a given position along it.
 *
 * Subtracts segment lengths (+0xC of each 16-byte segment at +0x18) until @p pathParam falls inside one, or the last segment is
 * reached, and returns that segment's direction (x, y, z, 0).
 *
 * @param out  receives the direction (the hidden return value)
 * @param pathParam  the position along the path (On Tour param)
 * @return @p out
 */
extern "C" sBXSeg16* cPath_getPathHeading(sBXSeg16* out, void* self, float pathParam)
{
    int i = 0;
    for (;;) {
        sBXSeg16* segment = &(*(sBXSeg16**)((char*)self + 0x18))[i];
        if (i == *(int*)((char*)self + 0x8) - 1 || pathParam <= segment->fC) {
            out->f0 = segment->f0;
            out->f4 = segment->f4;
            out->f8 = segment->f8;
            *(int*)&out->fC = 0;
            goto done;
        }
        pathParam -= segment->fC;
        i++;
    }
done:
    return out;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_findEvent);
#ifdef SKIP_ASM
struct sEntry_0026A920 {
    int key0;
    int key1;
    int pad[2];
};
struct sTable_0026A920 {
    int count;
    sEntry_0026A920* entries;
};

/**
 * Returns the path's first event whose type (+0x0) is @p firstKey and argument (+0x4) is @p secondKey.
 *
 * Walks the 16-byte events at +0x4 (count +0x0). Nothing in the C calls it.
 *
 * @param table  the path
 * @param firstKey  the event type
 * @param secondKey  the event argument
 * @return the event, or null
 */
extern "C" sEntry_0026A920* cPath_findEvent(sTable_0026A920* table, int firstKey, int secondKey)
{
    int i = 0;
    sEntry_0026A920* entry = table->entries;
    while (i < table->count) {
        i++;
        if (entry->key0 == firstKey && entry->key1 == secondKey)
            return entry;
        entry++;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("bx/bxstringctor", cPath_findEvent1);
#ifdef SKIP_ASM
struct sEntry_0026A970 {
    int key;
    int pad[3];
};
struct sTable_0026A970 {
    int count;
    sEntry_0026A970* entries;
};

/**
 * Returns the path's first event whose type (+0x0) is @p key.
 *
 * Walks the 16-byte events at +0x4 (count +0x0). Nothing in the C calls it.
 *
 * @param table  the path
 * @param key  the event type
 * @return the event, or null
 */
extern "C" sEntry_0026A970* cPath_findEvent1(sTable_0026A970* table, int key)
{
    int i = 0;
    sEntry_0026A970* entry = table->entries;
    while (i < table->count) {
        i++;
        if (entry->key == key)
            return entry;
        entry++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_getProximity);
#ifdef SKIP_ASM
struct sBXBox {
    char pad_0x00[0x1C];
    float min[3]; // 0x1C
    float max[3]; // 0x28
};

/**
 * Returns how far a point is outside a path's bounding box: the largest per-axis distance, 0 inside.
 *
 * The box is +0x1C (minimum) to +0x28 (maximum). cPathSys_getNearAIPaths and cPathSys_getNearTrackPaths rank paths with it.
 *
 * @param point  the point (x, y, z)
 * @return the largest of the X, Y and Z distances to the box
 */
extern "C" float cPath_getProximity(sBXBox* self, float* point)
{
    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;
    if (point[0] < self->min[0]) {
        dx = self->min[0] - point[0];
    } else if (self->max[0] < point[0]) {
        dx = point[0] - self->max[0];
    }
    if (point[1] < self->min[1]) {
        dy = self->min[1] - point[1];
    } else if (self->max[1] < point[1]) {
        dy = point[1] - self->max[1];
    }
    if (point[2] < self->min[2]) {
        dz = self->min[2] - point[2];
    } else if (self->max[2] < point[2]) {
        dz = point[2] - self->max[2];
    }
    if (dy < dx) {
        if (dx < dz) {
            return dz;
        }
        return dx;
    }
    if (dy < dz) {
        return dz;
    }
    return dy;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_getEventsInRange);
#ifdef SKIP_ASM
/**
 * Collects the path events whose ranges overlap a stretch of the path.
 *
 * Returns 0 when @p hi < @p lo; otherwise copies each 16-byte event whose [min, max] overlaps [@p lo, @p hi] from the
 * list (count +0x0, entries +0x4), stopping at @p maxOut.
 *
 * @param out  receives the events (On Tour events)
 * @param maxOut  the most events to return (On Tour maxEvents)
 * @param lo  the start of the stretch (On Tour l)
 * @param hi  the end of the stretch (On Tour h)
 * @return the number of events written
 */
extern "C" int cPath_getEventsInRange(sBXRangeList* self, sBXRange* out, int maxOut, float lo, float hi)
{
    int n;
    int i;
    if (hi < lo) {
        return 0;
    }
    n = 0;
    for (i = 0; i < self->count; i++) {
        if (!(hi < self->items[i].min) && !(self->items[i].max < lo)) {
            *out++ = self->items[i];
            n++;
            if (n == maxOut) {
                goto done;
            }
        }
    }
done:
    return n;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_getPathPos);
#ifdef SKIP_ASM
struct sBXVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBXPathSeg {
    float x, y, z;   // direction
    float len;       // 0xC
};

struct sBXPath {
    char pad_0x00[8];
    int count;          // 0x8
    float x, y, z;      // 0xC start
    sBXPathSeg* segs;   // 0x18
};

// PORT: PS2-only VU0 inline asm (direction times scalar).
static inline sBXVec4 bxVu0ScaleDir(const sBXPathSeg& seg, float s)
{
    sBXVec4 v;
    v.x = seg.x;
    v.y = seg.y;
    v.z = seg.z;
    v.w = 0.0f;
    sBXVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void bxVu0AddEq(sBXVec4& dst, sBXVec4 b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sBXVec4 bxVu0Add(const sBXVec4& a, sBXVec4 b)
{
    sBXVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Returns the point at a given distance along the path.
 *
 * From the start point (+0xC, w = 1) adds each segment's direction times its length until @p pathParam falls inside a segment,
 * then returns start + direction * remainder. Past the last segment it returns the end point and clears *@p inRange. A
 * negative @p pathParam extrapolates backwards from the start and still reports on-path.
 *
 * @param inRange  optional; set to 1 when the position is on the path, 0 past its end (On Tour isOnPath)
 * @param pathParam  the distance along the path (On Tour param)
 * @return the position (w = 1)
 */
extern "C" sBXVec4 cPath_getPathPos(sBXPath* self, int* inRange, float pathParam)
{
    sBXVec4 pos;
    pos.x = self->x;
    pos.y = self->y;
    pos.z = self->z;
    pos.w = 1.0f;
    if (inRange) {
        *inRange = 1;
    }
    for (int i = 0;; ) {
        sBXPathSeg* seg = &self->segs[i];
        if (i == self->count) {
            if (inRange) {
                *inRange = 0;
            }
            return pos;
        }
        float len = seg->len;
        if (pathParam < len) {
            return bxVu0Add(pos, bxVu0ScaleDir(*seg, pathParam));
        }
        pathParam -= len;
        i++;
        bxVu0AddEq(pos, bxVu0ScaleDir(*seg, len));
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_getLength);
#ifdef SKIP_ASM
struct sBXElem16 {
    float f0;
    float f4;
    float f8;
    float fC;
};

/**
 * Returns the total length of the path.
 *
 * Sum of the length (+0xC) of each of the +0x8 segments at +0x18.
 *
 * @return the path length
 */
extern "C" float cPath_getLength(cPath* self)
{
    int n = self->NumNodes;
    float sum = 0.0f;
    int i;
    for (i = 0; i < n; i++) {
        sum += ((sBXElem16*)self->Nodes)[i].fC;
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_isForkInPath);
#ifdef SKIP_ASM
/**
 * Tells whether a fork event lies within a stretch of the path.
 *
 * Scans the event list (count +0x0, entries +0x4) for an entry of type 0x12 or 0x14 whose [min, max] overlaps [@p lo,
 * @p hi].
 *
 * @param lo  the start of the stretch (On Tour l)
 * @param hi  the end of the stretch (On Tour h)
 * @return 1 when a fork is found
 */
extern "C" int cPath_isForkInPath(sBXRangeList* self, float lo, float hi)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (!(hi < self->items[i].min) && !(self->items[i].max < lo)) {
            if (self->items[i].field_0x0 == 20 || self->items[i].field_0x0 == 18) {
                return 1;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPath_isPathInBox);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sBXBox2 {
    char pad_0x00[0x1C];
    float min[3]; // 0x1C
    float max[3]; // 0x28
};

/**
 * Returns whether the XY rectangle [@p lo, @p hi] overlaps the XY extent of a path's bounding box (+0x1C to +0x28).
 *
 * Z is ignored. Nothing in the C calls it.
 *
 * @param lo  the rectangle minimum (x, y)
 * @param hi  the rectangle maximum (x, y)
 * @return 1 when they overlap, else 0
 */
extern "C" int cPath_isPathInBox(cPath* self, float* lo, float* hi)
{
    if (hi[0] < self->MinBound[0])
        return 0;
    if (hi[1] < self->MinBound[1])
        return 0;
    if (lo[0] > self->MaxBound[0])
        return 0;
    if (lo[1] > self->MaxBound[1])
        return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_loadInjectedPaths);
#ifdef SKIP_ASM
extern "C" void cPathSys_resolvePaths(cPathSys* self);

/**
 * Installs path data that is already in memory: stores it (+0x48), marks the path file loaded (+0x4C) and parses it
 * (cPathSys_resolvePaths).
 *
 * cAI_swaploadInjectedPathFile calls it on gPaths after cPathSys_purgePaths.
 *
 * @param data  the path data
 * @return 1
 */
extern "C" int cPathSys_loadInjectedPaths(cPathSys* self, int data)
{
    self->Data = (char*)data;
    self->m_bIsFileLoaded = 1;
    cPathSys_resolvePaths(self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_removeInjectedPathFile__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body).
 *
 * cAI_removeInjectedFile calls it on gPaths when a chunk with injected AI paths is unloaded.
 */
void cPathSys_removeInjectedPathFile(void* self)
{
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_loadPaths);
#ifdef SKIP_ASM
extern "C" int FILE_loadpack(int, int);

/**
 * Loads a path file: stores the pack (FILE_loadpack) at +0x48, marks it loaded (+0x4C) and parses it (cPathSys_resolvePaths).
 *
 * cAI_swaploadPathFile calls it on gPaths after cPathSys_purgePaths; cPathSys_appendPaths calls it when no file is loaded yet.
 *
 * @param pathSys  the path system
 * @param name  the path file name (passed as an int)
 */
extern "C" void cPathSys_loadPaths(void *pathSys, int name) {
    (*(int *)((char*)(pathSys) + (0x48))) = FILE_loadpack(name, 0);
    (*(int *)((char*)(pathSys) + (0x4C))) = 1;
    cPathSys_resolvePaths((cPathSys*)pathSys);
}
#endif
//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_isPathfileLoaded);
#ifdef SKIP_ASM
/**
 * Returns the path system's loaded flag (+0x4C).
 *
 * cPathSys_loadInjectedPaths and cPathSys_loadPaths set it.
 *
 * @param pathSys  the path system
 * @return non-zero when a path file is loaded
 */
extern "C" int cPathSys_isPathfileLoaded(cPathSys* pathSys) {
    return (pathSys->m_bIsFileLoaded);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_resolvePaths);
#ifdef SKIP_ASM
// PORT: pathREAD_tInt32 takes (dst, src); the unit declares it with one arg
void pathREAD_tInt32_2(void* dst, void* src) __asm__("pathREAD_tInt32");
extern "C" int cPathSys_loadAIPaths(void* self, char* p);
extern "C" int cPathSys_loadTrackPaths(void* self, char* p);
extern "C" int cPathSys_loadCheckpoints(void* self, char* p);
extern "C" void cPathSys_loadStartpoints(void* self, char* p);

/**
 * Parses the loaded path data (+0x48) into the path system's startpoints, AI paths, track paths and checkpoints.
 *
 * Clears the startpoint, AI path and track path counts (+0x0, +0x8, +0x10), skips a header word, then runs
 * cPathSys_loadAIPaths, cPathSys_loadTrackPaths, cPathSys_loadCheckpoints and cPathSys_loadStartpoints in turn, each starting where the
 * previous one stopped. cPathSys_loadInjectedPaths and cPathSys_loadPaths call it.
 */
extern "C" void cPathSys_resolvePaths(cPathSys* self)
{
    int header;
    char* cursor = self->Data;
    self->NumAIPaths = 0;
    self->NumTrackPaths = 0;
    self->NumStartpoints = 0;
    pathREAD_tInt32_2(&header, cursor);
    cursor += 4;
    cursor += cPathSys_loadAIPaths(self, cursor);
    cursor += cPathSys_loadTrackPaths(self, cursor);
    cursor += cPathSys_loadCheckpoints(self, cursor);
    cPathSys_loadStartpoints(self, cursor);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_appendPaths);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void cPathSys_loadPaths(void* self, int arg1);
extern "C" int FILE_loadpack(int, int);
// PORT: pathREAD_tInt32 takes (dst, src); the unit declares it with one arg
void pathREAD_tInt32_3(void* dst, void* src) __asm__("pathREAD_tInt32");
extern "C" int cPathSys_loadAIPaths(void* self, char* p);
extern "C" int cPathSys_loadTrackPaths(void* self, char* p);

/**
 * Loads a further path file and parses only its AI and track paths into the path system.
 *
 * Frees the current data (+0x48) when there is one; when there is none it first loads the same file with cPathSys_loadPaths,
 * whose buffer is then dropped without being freed. Then it loads the file again with FILE_loadpack, stores it at
 * +0x48, skips a header word and runs cPathSys_loadAIPaths and cPathSys_loadTrackPaths. Nothing in the C calls it.
 *
 * @param name  the path file name (passed as an int)
 */
extern "C" void cPathSys_appendPaths(cPathSys* self, int name)
{
    int header;
    void* old = self->Data;
    if (old != 0) {
        cMemMan_free(old);
    } else {
        cPathSys_loadPaths(self, name);
    }
    self->Data = 0;
    char* cursor = (char*)FILE_loadpack(name, 0);
    self->Data = cursor;
    pathREAD_tInt32_3(&header, cursor);
    cursor += 4;
    cursor += cPathSys_loadAIPaths(self, cursor);
    cPathSys_loadTrackPaths(self, cursor);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_purgePaths);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees the loaded path data of the path system.
 *
 * Deletes the arrays at +0xC and +0x14 if present and the one at +0x48 if present and owned (+0x4C), then nulls them
 * and clears +0x4C.
 */
extern "C" void cPathSys_purgePaths(cPathSys* self)
{
    if (self->AIPaths != 0)
        cMemMan_free(self->AIPaths);
    self->AIPaths = 0;
    if (self->TrackPaths != 0)
        cMemMan_free(self->TrackPaths);
    self->TrackPaths = 0;
    if (self->Data != 0 && self->m_bIsFileLoaded != 0)
        cMemMan_free(self->Data);
    self->Data = 0;
    self->m_bIsFileLoaded = 0;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_getTrackPath);
#ifdef SKIP_ASM
/**
 * Returns track path @p pathIndex: +0x14 plus pathIndex * 0x3C.
 *
 * The AI stores the result as the rider's track path (+0xAB4).
 *
 * @param pathIndex  the track path index
 * @return the track path
 */
extern "C" int cPathSys_getTrackPath(cPathSys* self, int pathIndex)
{
    return (int)self->TrackPaths + pathIndex * 0x3c;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_getAIPath);
#ifdef SKIP_ASM
/**
 * Returns AI path @p index: +0xC plus index * 0x40.
 *
 * The AI stores the result as the rider's AI path (+0xAB8).
 *
 * @param index  the AI path index
 * @return the AI path
 */
extern "C" int cPathSys_getAIPath(cPathSys* self, int index)
{
    return (int)self->AIPaths + (index << 6);
}
#endif

extern "C" void* cPathSys_getNearAIPaths(void*);

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_getNearResetPaths__FPv);
#ifdef SKIP_ASM
// PORT: the unit declares cPathSys_getNearAIPaths(void*) and this symbol's mangling says (void*),
// but both really take more arguments (callers pass $5-$7, and this passes a 5th arg in $8).
// Bind the real signatures to the existing symbol names with asm labels.
int cPathSys_getNearAIPaths_impl(void* self, void* a1, void** out, int maxOut, int flag) __asm__("cPathSys_getNearAIPaths");
int cPathSys_getNearResetPaths_impl(void* self, void* a1, void** out, int maxOut) __asm__("cPathSys_getNearResetPaths__FPv");

/**
 * Lists the reset paths nearest to a position.
 *
 * cPathSys_getNearAIPaths with the last argument 1. The C definition is cPathSys_getNearResetPaths_impl, bound to the
 * symbol cPathSys_getNearResetPaths__FPv by asm label because the mangled name says (void *).
 *
 * @param position  the position (On Tour pos)
 * @param out  receives the paths (On Tour pathList)
 * @param maxOut  the most paths to return (On Tour maxListLen)
 * @return the number of paths written
 */
int cPathSys_getNearResetPaths_impl(void* self, void* position, void** out, int maxOut)
{
    return cPathSys_getNearAIPaths_impl(self, position, out, maxOut, 1);
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_getNearAIPaths);
#ifdef SKIP_ASM
// PORT: the unit declares cPathSys_getNearAIPaths(void*) and this symbol's mangling says (void*),
// but it really takes five arguments; bound by asm label (same as cPathSys_getNearResetPaths).
int cPathSys_getNearAIPaths_impl(void* self, float* pos, void** pathList, int maxListLen, int onlyResetPaths) __asm__("cPathSys_getNearAIPaths");

// cAIPath (On Tour name): 0x40-byte path record
struct cAIPath_AFB8 {
    char pad0[0x3C];
    int mIsReset;   // 0x3C: only these qualify when onlyResetPaths
};

// cPathSys (On Tour member names)
struct cPathSys_AFB8 {
    int NumStartpoints;        // 0x0
    void* Startpoints;         // 0x4
    int NumAIPaths;            // 0x8
    cAIPath_AFB8* AIPaths;     // 0xC
};

/**
 * Lists the AI paths nearest to a position, those containing it first.
 *
 * Proximity of each path comes from cPath_getProximity; with @p onlyResetPaths a path whose +0x3C flag is clear gets 1e37.
 * Paths at proximity 0 (containing @p pos) are listed first; the list is then filled with the remaining nearest paths.
 * Likely bugs: the fill loop does not skip the 1e37 entries, so it can return non-reset paths and repeat paths already
 * listed when fewer suitable paths exist than requested; the proximity buffer is a fixed 200 floats with no check
 * against the path count.
 *
 * @param vself  the path system (this)
 * @param pos  the position
 * @param pathList  receives the paths
 * @param maxListLen  the most paths to return; clamped to the path count
 * @param onlyResetPaths  nonzero to consider only reset paths
 * @return the number of paths written
 */
// cPathSys::getNearAIPaths (On Tour name): up to maxListLen AI paths nearest to pos
// (paths containing it first), nearest first; returns how many.
int cPathSys_getNearAIPaths_impl(void* vself, float* pos, void** pathList, int maxListLen, int onlyResetPaths)
{
    cPathSys_AFB8* self = (cPathSys_AFB8*)vself;
    int i;
    int listLen = 0;
    float prox[200];
    if (self->NumAIPaths < maxListLen) {
        maxListLen = self->NumAIPaths;
    }
    for (i = 0; i < self->NumAIPaths; i++) {
        prox[i] = cPath_getProximity((sBXBox*)&self->AIPaths[i], pos);
        if (onlyResetPaths && self->AIPaths[i].mIsReset == 0) {
            prox[i] = 9.999999933815813e+36f;
        }
        if (prox[i] == 0.0f) {
            prox[i] = 9.999999933815813e+36f;
            pathList[listLen++] = &self->AIPaths[i];
            if (listLen == maxListLen) {
                break;
            }
        }
    }
    while (listLen < maxListLen) {
        float bestProx = prox[0];
        int bestI = 0;
        for (i = 1; i < self->NumAIPaths; i++) {
            if (prox[i] < bestProx) {
                bestProx = prox[i];
                bestI = i;
            }
        }
        prox[bestI] = 9.999999933815813e+36f;
        pathList[listLen++] = &self->AIPaths[bestI];
    }
    return listLen;
}
#endif

//100%
INCLUDE_ASM("bx/bxstringctor", cPathSys_getNearTrackPaths);
#ifdef SKIP_ASM
struct sBXBox;


extern "C" float cPath_getProximity(sBXBox* self, float* p);

static inline int minB178(int a, int b)
{
    return a < b ? a : b;
}

/**
 * Collects up to @p max track paths nearest to @p pos into @p out, nearest first, and returns how many.
 *
 * Paths whose box contains the point (cPath_getProximity gives 0) come first, in index order; the rest follow by increasing
 * cPath_getProximity distance. The paths are the 0x3C-byte entries at +0x14 (count +0x10); the distances live in a fixed
 * 52-entry stack array, so more than 52 paths would overflow it.
 *
 * @param pos  the point
 * @param out  receives the path pointers
 * @param max  the most paths to return
 * @return the number of paths written
 */
extern "C" int cPathSys_getNearTrackPaths(cPathSys* self, float* pos, cTrackPath** out, int max)
{
    float dist[52];
    int n = 0;
    int want = minB178(self->NumTrackPaths, max);
    int i;
    for (i = 0; i < self->NumTrackPaths; i++) {
        dist[i] = cPath_getProximity((sBXBox*)&(self->TrackPaths)[i], pos);
        if (dist[i] == 0.0f) {
            dist[i] = 1e37f;
            out[n++] = &(self->TrackPaths)[i];
            if (n == want) {
                break;
            }
        }
    }
    while (n < want) {
        float best = dist[0];
        int bi = 0;
        for (i = 1; i < self->NumTrackPaths; i++) {
            if (dist[i] < best) {
                best = dist[i];
                bi = i;
            }
        }
        dist[bi] = 1e37f;
        out[n] = &(self->TrackPaths)[bi];
        n++;
    }
    return n;
}
#endif

