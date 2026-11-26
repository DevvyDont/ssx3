#include "common.h"
#include "bx/appman.h"
#include "fe/festatereadmessagepopup.h"
#include "fe/festatenetmessenger.h"
#include "online/lobbyman.h"
#include "vtable.h"
#include "render/font.h"
#include "util/menu.h"
#include "online/privatemessage.h"
#include "fe/debuglobbymenus.h"
#include "fe/festatelegal.h"
#include "fe/festate.h"
#include "fe/fepopupoptionreset.h"
#include "camera/camera.h"
#include "fe/fereal.h"
#include "fe/fepopupconfirm.h"
#include "fe/debugfe.h"
#include "fe/screenkeyboard.h"
#include "fe/festatememcard.h"
#include "util/locale.h"
#include "fe/festateriderdetail.h"
#include "fe/fecustom.h"
#include "fe/festatecharsetup.h"
#include "fe/festateoptions.h"
#include "fe/festateload.h"
#include "fe/festateprofile.h"
#include "ui/uithing.h"
#include "fe/fememcard.h"
#include "fe/feinput.h"
#include "fe/festatecredits.h"
#include "fe/festatetransition.h"
#include "fe/festatetitle.h"
#include "fe/festateoptionssound.h"
#include "fe/festateoptionshud.h"
#include "fe/festateoptionsgame.h"
#include "fe/festatemainmenu.h"
#include "fe/festatebonusmaterial.h"
#include "fe/fepopupvideocalibration.h"
#include "fe/festatecharselect.h"
#include "fe/festateubertrick.h"
#include "fe/popupinfo.h"
#include "fe/keyboardpopup.h"
#include "fe/screenpopup.h"
#include "fe/uistatebuypopup.h"
#include "fe/festatenet.h"

//100%
INCLUDE_ASM("main/all_in_one_main", ChallengeResponseMenu_ChallengeResponseMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;

void cMenu_cMenu(void*);
extern "C" void cMenu_addItem(void *, char*, int);
extern "C" void cCommandMenuItem_cCommandMenuItem2(char*, int, int *, int);
void cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* label);
void cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* height);

extern char D_0046D3D0[];
extern char str_You_ve_been_Challenged[];
extern char str_by[];
extern char D_00467DC8[];
extern int D_00467DD0[];
extern int D_00467DD8[];

/**
 * Constructs the debug front end's "You've been Challenged!" menu; despite its name it is not a static
 * initializer.
 *
 * The items are "You've been Challenged!", "by", an empty line, a spacer and the Accept and Decline items.
 * Accept and Decline later call func_0017E490 / func_0017E4F0 with the owner stored at +0x130 / +0x134.
 *
 * @param acceptOwner  the Accept functor (one word: the owning screen), copied to mAcceptScreen (+0x130)
 * @param declineOwner  the Decline functor (one word: the owning screen), copied to mDeclineScreen (+0x134)
 * @return self
 */
extern "C" char* ChallengeResponseMenu_ChallengeResponseMenu(ChallengeResponseMenu* self,
                                                             cDebugLobbyScreen* const* acceptOwner,
                                                             cDebugLobbyScreen* const* declineOwner)
{
    char* item0 = (char*)&self->mTitleItem;
    char* item1 = (char*)&self->mByItem;
    char* item2 = (char*)&self->mChallengerItem;
    char* item3 = (char*)&self->mSpaceItem;
    char* item4 = (char*)&self->mAcceptItem;
    char* item5 = (char*)&self->mDeclineItem;
    cMenu_cMenu(self);
    self->mVtbl = (void**)D_0046D3D0;
    self->mAcceptScreen = *acceptOwner;
    self->mDeclineScreen = *declineOwner;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)item0, str_You_ve_been_Challenged);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)item1, str_by);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)item2, D_00467DC8);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)item3, (void*)0xE);
    cCommandMenuItem_cCommandMenuItem2(item4, ChallengeResponseMenu::kItem_Accept, D_00467DD0, 0);
    cCommandMenuItem_cCommandMenuItem2(item5, ChallengeResponseMenu::kItem_Decline, D_00467DD8, 0);
    cMenu_addItem(self, item0, -1);
    cMenu_addItem(self, item1, -1);
    cMenu_addItem(self, item2, -1);
    cMenu_addItem(self, item3, -1);
    cMenu_addItem(self, item4, -1);
    cMenu_addItem(self, item5, -1);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AbortMenu_AbortMenu);
#ifdef SKIP_ASM
void cMenu_cMenu(void*);
extern "C" void cCommandMenuItem_cCommandMenuItem2(char*, int, int *, int);
extern "C" void cMenu_addItem(void *, char*, int);
extern int D_00467DE0[];
extern int AbortMenu_vtbl[];

/**
 * Constructs the debug online front end's "Abort" menu: a cMenu with one command item.
 *
 * Runs cMenu_cMenu, stores the menu's vtable AbortMenu_vtbl at +0x12C and the owner's first word at +0x130, builds a
 * command item (cCommandMenuItem_cCommandMenuItem2) at +0x134 with ID 0 and caption "Abort", and adds it with cMenu_addItem. Selecting it
 * runs AbortMenu_onCommand. cDebugLobbyScreen_cDebugLobbyScreen builds it at +0x1AAC of the debug front end.
 *
 * @param menu  the menu to construct
 * @param owner  the Abort functor (one word: the owning debug lobby screen), copied to mScreen (+0x130)
 * @return @p menu
 */
extern "C" void *AbortMenu_AbortMenu(AbortMenu* menu, cDebugLobbyScreen* const* owner) {
    cMenu_cMenu(menu);
    char *abortItem = (char*)&menu->mAbortItem;
    menu->mVtbl = (void**)AbortMenu_vtbl;
    menu->mScreen = *owner;
    cCommandMenuItem_cCommandMenuItem2(abortItem, 0, D_00467DE0, 0);
    cMenu_addItem(menu, abortItem, -1);
    return ((char*)menu);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", DeletePersonaMenu_setpersonas);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMenuItem_activate(void* item, int enable);

// Separate inline accessor: its own `i * 0x24` (not CSE'd with the call's address).
static inline void setPersonaCaption(cCommandMenuItem* item, const char* name)
{
    item->Caption = name;
}

extern "C" void DeletePersonaMenu_setpersonas_impl(DeletePersonaMenu *self, LobbyMan *src)
    __asm__("DeletePersonaMenu_setpersonas");
/**
 * Fills the debug "Delete Persona" menu with every persona except the first.
 *
 * For each persona i from 1 to LobbyMan::mNumPersonas - 1 it uses command item i - 1 (mPersonaItems, 0x24 bytes each
 * from +0x168): the item's Caption becomes mPersonas[i], the item is activated (cMenuItem_activate with 1) and its ID
 * is set to i. The remaining items up to 4 are deactivated. Persona 0 is never offered for deletion.
 *
 * The object is the Delete Persona menu (built by DeletePersonaMenu_DeletePersonaMenu).
 *
 * @param src  the lobby manager (LobbyMan): mNumPersonas at +0x6C, mPersonas at +0x70
 */
extern "C" void DeletePersonaMenu_setpersonas_impl(DeletePersonaMenu *self, LobbyMan *src) {
    int i = 0;
    int next;
    while (i < src->mNumPersonas - 1) {
        next = i + 1;
        setPersonaCaption(&self->mPersonaItems[i], *(src->mPersonas + next));
        cMenuItem_activate(&self->mPersonaItems[i], 1);
        (self->mPersonaItems + i)->ID = next;
        i = next;
    }
    while (i < 4) {
        cMenuItem_activate(&self->mPersonaItems[i], 0);
        i++;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/all_in_one_main", PersonaMenu_setpersonas);
#ifdef SKIP_ASM



extern "C" void cMenuItem_activate(void* item, int enable);
extern "C" void DeletePersonaMenu_setpersonas(void* sub, LobbyMan* src);

/**
 * Fills the debug "Select Persona" menu from the lobby manager's persona list.
 *
 * For each of LobbyMan::mNumPersonas personas, the command item at +0x518 + i * 0x24 gets the persona name as its
 * caption, is activated (cMenuItem_activate) and gets ID i; the remaining items up to 4 are deactivated. It then refreshes
 * the Delete Persona submenu at +0x2E8 (DeletePersonaMenu_setpersonas), enables the "Delete Persona" item (+0x5C4) only when there are
 * at least 2 personas, and the "Create Persona" item (+0x5A8) only while there are fewer than 4.
 *
 * The object is the Select Persona menu (built by PersonaMenu_PersonaMenu).
 *
 * @param src  the lobby manager (LobbyMan): mNumPersonas at +0x6C, mPersonas at +0x70
 */
void PersonaMenu::setpersonas(LobbyMan* src)
{
    int i = 0;
    for (; i < src->mNumPersonas; i++) {
        const char* name = src->mPersonas[i];
        cCommandMenuItem* it = &((&mPersonaItems[0]))[i];
        it->Caption = name;
        cMenuItem_activate(it, 1);
        it->ID = i;
    }
    for (; i < 4; i++) {
        cMenuItem_activate(&((&mPersonaItems[0]))[i], 0);
    }
    DeletePersonaMenu_setpersonas(((char*)&mDeletePersonaMenu), src);
    cMenuItem_activate(((char*)&mDeletePersonaItem), src->mNumPersonas >= 2);
    cMenuItem_activate(((char*)&mCreatePersonaItem), src->mNumPersonas < 4);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", LobbyMenu_updaterooms);
#ifdef SKIP_ASM


extern "C" int LobbyMan_getnumrooms(void* src);
extern "C" void* func_0025F150(void* src, int i);
extern "C" void cMenuItem_activate(void* item, int enable);
extern char str_Receiving_Room_List[];

/**
 * Fills the debug "Select Room" menu with the lobby's room names.
 *
 * Asks LobbyMan_getnumrooms for the room count and gives command item i (12 items of 0x24 bytes from +0x168) the name from
 * func_0025F150(src, i), activates it and sets its ID to i. With no rooms yet, the first item shows "Receiving Room
 * List" with ID -1 (which the menu's command handler LobbyMenu_onCommand ignores). The remaining items are deactivated.
 *
 * The object is the Select Room menu (built by LobbyMenu_LobbyMenu).
 *
 * @param src  the lobby manager (LobbyMan)
 */
void LobbyMenu::updaterooms(LobbyMan* src)
{
    int i = 0;
    int n = LobbyMan_getnumrooms(src);
    for (; i < n; i++) {
        cCommandMenuItem* it = &((&mRoomItems[0]))[i];
        it->Caption = (const char*)func_0025F150(src, i);
        cMenuItem_activate(it, 1);
        it->ID = i;
    }
    if (i == 0) {
        mRoomItems[0].Caption = str_Receiving_Room_List;
        i = 1;
        cMenuItem_activate(&((&mRoomItems[0]))[0], 1);
        ((&mRoomItems[0]))[0].ID = -1;
    }
    for (; i < 12; i++) {
        cMenuItem_activate(&((&mRoomItems[0]))[i], 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", RoomMenu_updateusers);
#ifdef SKIP_ASM


extern "C" int func_0025F1B0(void* src);
extern "C" void* func_0025F1E0(void* src, int i, int flags);
extern "C" void cMenuItem_activate(void* item, int enable);
extern char str_No_users_in_room[];

/**
 * Fills the debug "Room Occupants" menu with the users in the current room.
 *
 * Asks func_0025F1B0 for the user count and gives command item i (24 items of 0x24 bytes from +0x16C) the name from
 * func_0025F1E0(src, i, 0), activates it and sets its ID to i. An empty room shows "No users in room" with ID
 * kItem_NoUsers (which the command handler RoomMenu_onCommand ignores). The remaining items are deactivated.
 *
 * The object is the Room Occupants menu (built by RoomMenu_RoomMenu).
 *
 * @param src  the lobby manager (LobbyMan)
 */
void RoomMenu::updateusers(LobbyMan* src)
{
    int i = 0;
    int n = func_0025F1B0(src);
    for (; i < n; i++) {
        cCommandMenuItem* it = &((&mUserItems[0]))[i];
        it->Caption = (const char*)func_0025F1E0(src, i, 0);
        cMenuItem_activate(it, 1);
        it->ID = i;
    }
    if (i == 0) {
        mUserItems[0].Caption = str_No_users_in_room;
        i = 1;
        cMenuItem_activate(&((&mUserItems[0]))[0], 1);
        ((&mUserItems[0]))[0].ID = kItem_NoUsers;
    }
    for (; i < 24; i++) {
        cMenuItem_activate(&((&mUserItems[0]))[i], 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB718);
#ifdef SKIP_ASM
void operator_delete(int*);

/**
 * Empties a std::list: frees every node and leaves the head node linked to itself.
 *
 * Walks from the head's next node back to the head (mpHead, +0x4), freeing each node with operator_delete, then sets
 * the head's next and prev to the head. The head itself is kept. The LobbyMan destructor uses it on mEventList; the
 * async-file and lobby-tag code use it on their own lists.
 */
extern "C" void func_001DB718(tLobbyEventList *self) {
    typedef _List_node<online_Event*> Node;
    Node *node = (Node*)((Node*)self->mpHead)->_M_next;
    while (node != ((Node*)self->mpHead)) {
        Node *n = node;
        node = (Node*)node->_M_next;
        operator_delete((int *)n);
    }
    ((Node*)self->mpHead)->_M_next = self->mpHead;
    ((Node*)self->mpHead)->_M_prev = self->mpHead;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB778);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tagDB778(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);
void* cMemMan_free(void* ptr);
void operator_delete(int*);
extern char str_StlBlock[];


static inline cBXString** sPtrVecDB778_allocate(unsigned int n)
{
    if (n == 1)
        return (cBXString**)cMemMan_alloc(4, str_StlBlock, 0x20000000, 0);
    return (cBXString**)operator_new_tagDB778(n * 4, str_StlBlock, 0x20000000, 0);
}

static inline void sPtrVecDB778_deallocate(cBXString** p, unsigned int n)
{
    if (p != 0) {
        if (n == 1)
            operator_delete((int*)p);
        else
            cMemMan_free(p);
    }
}

static inline cBXString** sPtrVecDB778_copy(cBXString** first, cBXString** last, cBXString** result)
{
    func_0041610C(result, first, (last - first) * sizeof(cBXString*));
    return result + (last - first);
}

static inline cBXString** sPtrVecDB778_ucopy(cBXString** first, cBXString** last, cBXString** result)
{
    return sPtrVecDB778_copy(first, last, result);
}

static inline void sPtrVecDB778_construct(cBXString** p, cBXString* const& v)
{
    if (p != 0)
        *p = v;
}

static inline cBXString** sPtrVecDB778_copy_backward(cBXString** first, cBXString** last, cBXString** result)
{
    int n = last - first;
    func_0041610C(result - n, first, n * sizeof(cBXString*));
    return result - n;
}

/**
 * Inserts a pointer into a vector of cBXString pointers at a position, growing the storage when it is full.
 *
 * The EA STL vector's insert helper. With spare capacity it copies the last element one slot up, moves [pos, finish -
 * 1) up by one with func_0041610C (memmove) and stores the value at @p pos. When full it allocates twice the old size
 * (1 when empty) from "StlBlock" (cMemMan_alloc for one element, operator_new__FUi otherwise), copies the elements
 * before @p pos, the new value and the rest, frees the old block (operator_delete for one element, cMemMan_free
 * otherwise) and updates start, finish and end of storage. The net messenger uses it on its tFEStringPtrVector.
 *
 * The vector holds an allocator word, then start (+0x4), finish (+0x8) and end of storage (+0xC).
 *
 * @param pos  where to insert
 * @param x  the pointer to insert (passed by reference)
 */
extern "C" void func_001DB778(tFEStringPtrVector* self, cBXString** pos, cBXString* const& x)
{
    if (self->mpFinish != self->mpEndOfStorage) {
        if (self->mpFinish != 0)
            *self->mpFinish = self->mpFinish[-1];
        ++self->mpFinish;
        cBXString* x_copy = x;
        sPtrVecDB778_copy_backward(pos, self->mpFinish - 2, self->mpFinish - 1);
        *pos = x_copy;
    } else {
        unsigned int old_size = self->mpFinish - self->mpStart;
        unsigned int len = old_size != 0 ? 2 * old_size : 1;
        cBXString** new_start = sPtrVecDB778_allocate(len);
        cBXString** new_finish = new_start;
        new_finish = sPtrVecDB778_ucopy(self->mpStart, pos, new_start);
        sPtrVecDB778_construct(new_finish, x);
        ++new_finish;
        new_finish = sPtrVecDB778_ucopy(pos, self->mpFinish, new_finish);
        sPtrVecDB778_deallocate(self->mpStart, self->mpEndOfStorage - self->mpStart);
        self->mpStart = new_start;
        self->mpFinish = new_finish;
        self->mpEndOfStorage = new_start + len;
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB918);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tagDB918(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);
void* cMemMan_free(void* ptr);
void operator_delete(int*);
extern char str_StlBlock[];


static inline cPrivateMessage** sPtrVecDB918_allocate(unsigned int n)
{
    if (n == 1)
        return (cPrivateMessage**)cMemMan_alloc(4, str_StlBlock, 0x20000000, 0);
    return (cPrivateMessage**)operator_new_tagDB918(n * 4, str_StlBlock, 0x20000000, 0);
}

static inline void sPtrVecDB918_deallocate(cPrivateMessage** p, unsigned int n)
{
    if (p != 0) {
        if (n == 1)
            operator_delete((int*)p);
        else
            cMemMan_free(p);
    }
}

static inline cPrivateMessage** sPtrVecDB918_copy(cPrivateMessage** first, cPrivateMessage** last,
        cPrivateMessage** result)
{
    func_0041610C(result, first, (last - first) * sizeof(cPrivateMessage*));
    return result + (last - first);
}

static inline cPrivateMessage** sPtrVecDB918_ucopy(cPrivateMessage** first, cPrivateMessage** last,
        cPrivateMessage** result)
{
    return sPtrVecDB918_copy(first, last, result);
}

static inline void sPtrVecDB918_construct(cPrivateMessage** p, cPrivateMessage* const& v)
{
    if (p != 0)
        *p = v;
}

static inline cPrivateMessage** sPtrVecDB918_copy_backward(cPrivateMessage** first, cPrivateMessage** last,
        cPrivateMessage** result)
{
    int n = last - first;
    func_0041610C(result - n, first, n * sizeof(cPrivateMessage*));
    return result - n;
}

/**
 * Inserts a pointer into a vector of cPrivateMessage pointers at a position, growing the storage when it is full.
 *
 * The same EA STL vector insert helper as func_001DB778, instantiated for the read-message popup's
 * tFEPrivateMessageVector: with spare capacity it shifts [pos, finish) up by one (func_0041610C) and stores the value;
 * when full it allocates twice the old size (1 when empty) from "StlBlock", copies both halves around the new value,
 * frees the old block and updates start, finish and end of storage.
 *
 * The vector holds an allocator word, then start (+0x4), finish (+0x8) and end of storage (+0xC).
 *
 * @param pos  where to insert
 * @param x  the pointer to insert (passed by reference)
 */
extern "C" void func_001DB918(tFEPrivateMessageVector* self, cPrivateMessage** pos, cPrivateMessage* const& x)
{
    if (self->mpFinish != self->mpEndOfStorage) {
        if (self->mpFinish != 0)
            *self->mpFinish = self->mpFinish[-1];
        ++self->mpFinish;
        cPrivateMessage* x_copy = x;
        sPtrVecDB918_copy_backward(pos, self->mpFinish - 2, self->mpFinish - 1);
        *pos = x_copy;
    } else {
        unsigned int old_size = self->mpFinish - self->mpStart;
        unsigned int len = old_size != 0 ? 2 * old_size : 1;
        cPrivateMessage** new_start = sPtrVecDB918_allocate(len);
        cPrivateMessage** new_finish = new_start;
        new_finish = sPtrVecDB918_ucopy(self->mpStart, pos, new_start);
        sPtrVecDB918_construct(new_finish, x);
        ++new_finish;
        new_finish = sPtrVecDB918_ucopy(pos, self->mpFinish, new_finish);
        sPtrVecDB918_deallocate(self->mpStart, self->mpEndOfStorage - self->mpStart);
        self->mpStart = new_start;
        self->mpFinish = new_finish;
        self->mpEndOfStorage = new_start + len;
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AbortMenu_onCommand);
#ifdef SKIP_ASM
extern "C" void func_0017E558(cDebugLobbyScreen* screen);

/**
 * Handles the debug "Abort" menu's command: hands control back to the debug front end via func_0017E558.
 *
 * It is the onCommand slot of the Abort menu's vtable AbortMenu_vtbl. func_0017E558 pushes the Room Occupants menu, sets
 * the debug front end's state to 0xC and calls func_0025F628 on the lobby manager.
 *
 * @return 0
 */
int AbortMenu::onCommand()
{
    func_0017E558(mScreen);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", ChallengeResponseMenu_onCommand);
#ifdef SKIP_ASM
extern "C" void func_0017E490(cDebugLobbyScreen* screen);
extern "C" void func_0017E4F0(cDebugLobbyScreen* screen);

/**
 * Handles the debug challenge menu's commands: Accept or Decline.
 *
 * It is the onCommand slot of the challenge menu's vtable D_0046D3D0 (built by
 * ChallengeResponseMenu_ChallengeResponseMenu). Item ID kItem_Accept calls func_0017E490 with the owner at +0x130,
 * which answers the challenge through func_0025FB58; ID kItem_Decline calls func_0017E4F0 with the owner at +0x134,
 * which declines through func_0025FC28.
 *
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
int ChallengeResponseMenu::onCommand(cMenuItem* item)
{
    int temp_3;

    temp_3 = item->ID;
    if (temp_3 == kItem_Accept) {
        func_0017E490(mAcceptScreen);
    } else if (temp_3 == kItem_Decline) {
        func_0017E4F0(mDeclineScreen);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", ChatMenu_onCommand);
#ifdef SKIP_ASM
extern "C" void func_0017E418(cDebugLobbyScreen* screen, const char* messageText);

/**
 * Handles the debug "Chat Menu" command: sends the chosen line as a lobby chat message.
 *
 * It is the onCommand slot of the chat menu's vtable ChatMenu_vtbl (built by ChatMenu_ChatMenu). It passes the item's caption
 * (+0x14, one of the canned lines in D_00467D70) to func_0017E418 with the debug front end at +0x130, which returns to
 * the Room Occupants menu and sends the text with func_0025AAE0 ("mesg").
 *
 * @param item  the chosen command item
 * @return 0
 */
int ChatMenu::onCommand(cCommandMenuItem* item)
{
    func_0017E418(mScreen, item->Caption);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", RoomMenu_onCommand);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E368_BB48(cDebugLobbyScreen* screen, int user) __asm__("func_0017E368");
extern "C" void func_0017E3B8_BB48(cDebugLobbyScreen* screen) __asm__("func_0017E3B8");

/**
 * Handles the debug "Room Occupants" menu's commands.
 *
 * It is the onCommand slot of vtable RoomMenu_vtbl (built by RoomMenu_RoomMenu). The "Chat" item (ID kItem_Chat) calls
 * func_0017E3B8 with the owner at +0x134, which opens the chat menu; an occupant (ID 0 and up) calls func_0017E368 with
 * the owner at +0x130 and the occupant's index; the placeholder line (ID kItem_NoUsers) does nothing.
 *
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
int RoomMenu::onCommand(cMenuItem* item)
{
    int itemValue = item->ID;
    if (itemValue == kItem_Chat) {
        func_0017E3B8_BB48(mChatScreen);
    } else if (itemValue != kItem_NoUsers) {
        func_0017E368_BB48(mSelectUserScreen, itemValue);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", LobbyMenu_onCommand);
#ifdef SKIP_ASM
extern "C" void func_0017E2F8(cDebugLobbyScreen* screen, int room);

/**
 * Handles the debug "Select Room" menu's command: joins the chosen room.
 *
 * It is the onCommand slot of vtable LobbyMenu_vtbl (built by LobbyMenu_LobbyMenu). For an item ID of 0 or more it calls
 * func_0017E2F8 with the debug front end at +0x130 and the room index (state 11, then func_0025D800 on the lobby
 * manager); the "Receiving Room List" placeholder (ID -1) is ignored.
 *
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
int LobbyMenu::onCommand(cMenuItem* item)
{
    int temp_5;

    temp_5 = item->ID;
    if (temp_5 >= 0) {
        func_0017E2F8(mScreen, temp_5);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", PersonaMenu_onCommand);
#ifdef SKIP_ASM
extern "C" void func_0017E290(cDebugLobbyScreen* screen, int persona);

/**
 * Handles the debug "Select Persona" menu's command: selects the chosen persona.
 *
 * It is the onCommand slot of vtable PersonaMenu_vtbl (built by PersonaMenu_PersonaMenu). It calls func_0017E290 with the debug front
 * end stored at +0x4E0 and the item's ID (the persona index), which sets state 7 and calls func_0025CD28 on the lobby
 * manager.
 *
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
int PersonaMenu::onCommand(cMenuItem* item)
{
    func_0017E290(mScreen, item->ID);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", CreatePersonaMenu_onCommand);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E1C0_BBE8(cDebugLobbyScreen* screen, void *name) __asm__("func_0017E1C0");

/**
 * Handles the debug "Create Persona" menu's command: creates a persona with the typed name.
 *
 * It is the onCommand slot of vtable CreatePersonaMenu_vtbl (built by CreatePersonaMenu_CreatePersonaMenu). It calls func_0017E1C0 with the debug front
 * end at +0x130 and the persona name buffer at +0x180 (the text-entry item at +0x168), which sets state 8 and calls
 * func_0025D1B8 on the lobby manager. It ignores which item was chosen.
 *
 * @return 0
 */
int CreatePersonaMenu::onCommand()
{
    func_0017E1C0_BBE8(mScreen, &mPersonaNameItem.Text[0]);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", DeletePersonaMenu_onCommand);
#ifdef SKIP_ASM
extern "C" void func_0017E228(cDebugLobbyScreen* screen, int persona);

/**
 * Handles the debug "Delete Persona" menu's command: deletes the chosen persona.
 *
 * It is the onCommand slot of vtable DeletePersonaMenu_vtbl (built by DeletePersonaMenu_DeletePersonaMenu). It calls func_0017E228 with the debug front
 * end at +0x130 and the item's ID (the persona index set by DeletePersonaMenu_setpersonas), which sets state 9 and calls func_0025D428
 * on the lobby manager.
 *
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
int DeletePersonaMenu::onCommand(cMenuItem* item)
{
    func_0017E228(mScreen, item->ID);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AccountDetailsMenu_onCommand);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E030_BC38(cDebugLobbyScreen* screen, char* name, char* password)
    __asm__("func_0017E030");

/**
 * Handles the debug "Create Account" menu's command: creates an account with the typed name and password.
 *
 * It is the onCommand slot of vtable AccountDetailsMenu_vtbl (built by AccountDetailsMenu_AccountDetailsMenu). It calls func_0017E030 with the debug front
 * end at +0x130, the login-name buffer at +0x180 and the password buffer at +0x1AC; that fills the lobby manager's
 * mNewAccount form and sends it with func_0025B9C8. It ignores which item was chosen.
 *
 * The object is the Create Account menu.
 *
 * @return 0
 */
int AccountDetailsMenu::onCommand()
{
    func_0017E030_BC38(mScreen, (char*)&mLoginNameItem.Text[0], (char*)&mPasswordItem.Text[0]);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AccountDetailsMenu_onCommand1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E118_BC68(cDebugLobbyScreen* screen, char* name, char* password)
    __asm__("func_0017E118");

/**
 * Handles the debug "Login" menu's command: logs in with the typed name and password.
 *
 * It is the onCommand slot of vtable AccountDetailsMenu1_vtbl (built by AccountDetailsMenu_AccountDetailsMenu1). It calls func_0017E118 with the debug front
 * end at +0x130, the login-name buffer at +0x180 and the password buffer at +0x1AC; that copies them into the lobby
 * manager's mUserName / mPassword and calls func_0025C610. It ignores which item was chosen.
 *
 * The object is the Login menu.
 *
 * @return 0
 */
int AccountDetailsMenu::onCommand1()
{
    func_0017E118_BC68(mScreen, (char*)&mLoginNameItem.Text[0], (char*)&mPasswordItem.Text[0]);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBC98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sE { unsigned int id:8; int val:24; };
static inline void sE_init(sE* self, int n) { self->id = n; self->val = n; }
extern int D_004A4E00;
extern int D_004A4E04;
extern sE D_004A4E08;
extern sE D_004A4E10;
extern sE D_004A4E18;
extern sE D_004A4E20;
extern sE D_004A4E28;
extern sE D_004A4E30;
extern sE D_004A4E38;
extern sE D_004A4E40;
extern sE D_004A4E48;
extern sE D_004A4E50;
extern sE D_004A4E58;
extern sE D_004A4E60;
extern sE D_004A4E68;
extern sE D_004A4E70;
extern sE D_004A4E78;
extern char gFlowState;
extern int D_004A4E84;
extern int D_004A4E88_v[2] __asm__("D_004A4E88"); // {int, int} pair (feasyncfile names it the same way)
extern "C" void cFEFlowState_purge(void*);
extern float D_004C6328[4];
extern float D_004C6338[4];
extern float D_004C6348[4];
extern float D_004C6358[4];
extern float D_004C6368[4];
extern float D_004C6378[4];
extern float D_004C6388[4];
extern float D_004C6398[4];
extern float D_004C63A8[4];
extern float D_004C63B8[4];
extern float D_004C63C8[4];
extern float D_004C63D8[4];
extern float D_004C63E8[4];
extern float D_004C63F8[4];
extern float D_004C6408[4];
extern float D_004C6418[4];
extern float D_004C6428[4];
extern float D_004C6438[4];
extern float D_004C6448[4];
extern float D_004C6458[4];
extern float D_004C6468[4];
extern float D_004C6478[4];
extern float D_004C6488[4];
extern float D_004C6498[4];
extern float D_004C64A8[4];
extern float D_004C64B8[4];
extern float D_004C64C8[4];
extern float D_004C64D8[4];
extern float D_004C64E8[4];
extern float D_004C64F8[4];
extern float D_004C6508[4];
extern float D_004C6518[4];
extern float D_004C6528[4];
extern float D_004C6538[4];
extern float D_004C6548[4];
extern float D_004C6558[4];
extern float D_004C6568[4];
extern float D_004C6578[4];
extern float D_004C6588[4];
extern float D_004C6598[4];
extern float D_004C65A8[4];
extern float D_004C65B8[4];
extern float D_004C65C8[4];
extern float D_004C65D8[4];
extern float D_004C65E8[4];
extern float D_004C65F8[4];
extern float D_004C6608[4];
extern float D_004C6618[4];
extern float D_004C6628[4];
extern float D_004C6638[4];
extern float D_004C6648[4];
extern float D_004C6658[4];
extern float D_004C6668[4];
extern float D_004C6678[4];
extern float D_004C6688[4];
extern float D_004C6698[4];
extern float gSmallSize[3];
extern float gBigSize[3];
extern float gStaticColour[4];
extern float gDisableColour[4];
extern float kInvalidColour[4];
extern float kOriginalColour[4];
extern float kColourEnabled[4];
extern float D_004C6718[4];
extern float D_004C6728[4];
extern float D_004C6738[4];
extern float D_004C6748[4];
extern float D_004C6758[4];
extern float D_004C6768[4];
extern float D_004C6778[4];
extern float gPopupWhite[4];
extern float gPopupBlack[4];
extern float gPopupBlue[4];
extern float gtStaticColor[4];
extern float gtNotStatic[4];
extern float gtEnabled[4];
extern float gtSelectedKey[4];
extern float gtNonSelectedKey[4];
extern float gSolidColour[4];
extern float gFadedColour[4];
extern float D_004C6828[4];
extern float D_004C6838[4];
void sti_001DBC98(int init, int prio) __asm__("func_001DBC98");
/**
 * Static initializer and destructor for all_in_one_main's globals (the init/priority routine g++ emits per unit).
 *
 * Only acts when @p prio is 0xFFFF. On the construct pass (@p init non-zero) it fills about 80 four-float colour
 * constants (D_004C6328 .. D_004C6838, including gDisableColour = {1, 0.6, 0.6, 0.6}, gPopupWhite = {1, 1, 1, 1} and
 * gPopupBlack = {1, 0, 0, 0}), two three-float vectors (gSmallSize = {100, 128, 0}, gBigSize = {130, 166, 0}),
 * fifteen packed id/value pairs D_004A4E08 .. D_004A4E78 (0 to 13, then -1), and zeroes D_004A4E00, D_004A4E04 and the
 * front-end flow state words gFlowState .. D_004A4E8C. On the destroy pass it calls cFEFlowState_purge on gFlowState. The
 * first colour values are the same as the ones func_003AD290 writes in world/wscriptcache, so they come from a shared
 * header.
 *
 * @param init  1 to construct, 0 to destroy
 * @param prio  initialization priority; only DEFAULT_INIT_PRIORITY does anything
 */
void sti_001DBC98(int init, int prio)
{
    if (prio == DEFAULT_INIT_PRIORITY) {
        if (init) {
            D_004C6328[0] = 1.0f;
            D_004C6328[1] = 1.0f;
            D_004C6328[2] = 1.0f;
            D_004C6328[3] = 0.5f;
            D_004C6338[0] = 0.499868006f;
            D_004C6338[1] = 1.0f;
            D_004C6338[2] = 1.0f;
            D_004C6338[3] = 0.0f;
            D_004C6348[0] = 0.129517004f;
            D_004C6348[1] = 1.0f;
            D_004C6348[2] = 1.0f;
            D_004C6348[3] = 0.0f;
            D_004C6358[0] = 0.224269003f;
            D_004C6358[1] = 0.940231025f;
            D_004C6358[2] = 0.54250598f;
            D_004C6358[3] = 0.000403000013f;
            D_004C6368[0] = 0.465207994f;
            D_004C6368[1] = 0.321705014f;
            D_004C6368[2] = 0.994343996f;
            D_004C6368[3] = 0.0f;
            D_004C6378[0] = 0.177926004f;
            D_004C6378[1] = 1.0f;
            D_004C6378[2] = 1.0f;
            D_004C6378[3] = 0.0f;
            D_004C6388[0] = 0.441174001f;
            D_004C6388[1] = 0.413886011f;
            D_004C6388[2] = 0.321126014f;
            D_004C6388[3] = 0.439823985f;
            D_004C6398[0] = 0.500997007f;
            D_004C6398[1] = 0.248495996f;
            D_004C6398[2] = 1.0f;
            D_004C6398[3] = 1.0f;
            D_004C63A8[0] = 0.276872993f;
            D_004C63A8[1] = 0.639562011f;
            D_004C63A8[2] = 0.134239003f;
            D_004C63A8[3] = 0.0f;
            D_004C63B8[0] = 0.893643975f;
            D_004C63B8[1] = 0.526346982f;
            D_004C63B8[2] = 0.505145013f;
            D_004C63B8[3] = 0.868730009f;
            D_004C63C8[0] = 1.0f;
            D_004C63C8[1] = 1.0f;
            D_004C63C8[2] = 1.0f;
            D_004C63C8[3] = 1.0f;
            D_004C63D8[0] = 1.0f;
            D_004C63D8[1] = 0.0f;
            D_004C63D8[2] = 0.0f;
            D_004C63D8[3] = 0.0f;
            D_004C63E8[0] = 1.0f;
            D_004C63E8[1] = 0.550000012f;
            D_004C63E8[2] = 0.535000026f;
            D_004C63E8[3] = 0.550000012f;
            D_004C63F8[0] = 1.0f;
            D_004C63F8[1] = 1.0f;
            D_004C63F8[2] = 1.0f;
            D_004C63F8[3] = 1.0f;
            D_004C6408[0] = 1.0f;
            D_004C6408[1] = 1.0f;
            D_004C6408[2] = 1.0f;
            D_004C6408[3] = 1.0f;
            D_004C6418[0] = 1.0f;
            D_004C6418[1] = 0.0f;
            D_004C6418[2] = 0.0f;
            D_004C6418[3] = 0.0f;
            D_004C6428[0] = 1.0f;
            D_004C6428[1] = 0.0f;
            D_004C6428[2] = 0.0f;
            D_004C6428[3] = 0.0f;
            D_004C6438[0] = 1.0f;
            D_004C6438[1] = 1.0f;
            D_004C6438[2] = 1.0f;
            D_004C6438[3] = 1.0f;
            D_004C6448[0] = 1.0f;
            D_004C6448[1] = 0.5f;
            D_004C6448[2] = 1.0f;
            D_004C6448[3] = 0.5f;
            D_004C6458[0] = 1.0f;
            D_004C6458[1] = 0.5f;
            D_004C6458[2] = 1.0f;
            D_004C6458[3] = 0.5f;
            D_004C6468[0] = 1.0f;
            D_004C6468[1] = 0.5f;
            D_004C6468[2] = 1.0f;
            D_004C6468[3] = 0.5f;
            D_004C6478[0] = 1.0f;
            D_004C6478[1] = 0.782811999f;
            D_004C6478[2] = 0.0617190003f;
            D_004C6478[3] = 0.0398439988f;
            D_004C6488[0] = 1.0f;
            D_004C6488[1] = 1.0f;
            D_004C6488[2] = 0.798825979f;
            D_004C6488[3] = 0.0f;
            D_004C6498[0] = 0.988671005f;
            D_004C6498[1] = 0.923049986f;
            D_004C6498[2] = 0.343116999f;
            D_004C6498[3] = 0.0f;
            D_004C64A8[0] = 1.0f;
            D_004C64A8[1] = 1.0f;
            D_004C64A8[2] = 1.0f;
            D_004C64A8[3] = 1.0f;
            D_004C64B8[0] = 1.0f;
            D_004C64B8[1] = 1.0f;
            D_004C64B8[2] = 0.797643006f;
            D_004C64B8[3] = 0.0f;
            D_004C64C8[0] = 1.0f;
            D_004C64C8[1] = 0.92226702f;
            D_004C64C8[2] = 0.341796994f;
            D_004C64C8[3] = 0.0f;
            D_004C64D8[0] = 1.0f;
            D_004C64D8[1] = 0.784763992f;
            D_004C64D8[2] = 0.0605470017f;
            D_004C64D8[3] = 0.0406249985f;
            D_004C64E8[0] = 1.0f;
            D_004C64E8[1] = 0.800000012f;
            D_004C64E8[2] = 0.449999988f;
            D_004C64E8[3] = 1.0f;
            D_004C64F8[0] = 1.0f;
            D_004C64F8[1] = 0.328126013f;
            D_004C64F8[2] = 1.0f;
            D_004C64F8[3] = 0.353906006f;
            D_004C6508[0] = 1.0f;
            D_004C6508[1] = 1.0f;
            D_004C6508[2] = 1.0f;
            D_004C6508[3] = 1.0f;
            D_004C6518[0] = 1.0f;
            D_004C6518[1] = 1.0f;
            D_004C6518[2] = 1.0f;
            D_004C6518[3] = 1.0f;
            D_004C6528[0] = 1.0f;
            D_004C6528[1] = 0.861329019f;
            D_004C6528[2] = 0.381247997f;
            D_004C6528[3] = 0.0f;
            D_004C6538[0] = 1.0f;
            D_004C6538[1] = 1.0f;
            D_004C6538[2] = 1.0f;
            D_004C6538[3] = 1.0f;
            D_004C6548[0] = 1.0f;
            D_004C6548[1] = 0.324438006f;
            D_004C6548[2] = 1.0f;
            D_004C6548[3] = 0.0f;
            D_004C6558[0] = 1.0f;
            D_004C6558[1] = 1.0f;
            D_004C6558[2] = 1.0f;
            D_004C6558[3] = 1.0f;
            D_004C6568[0] = 1.0f;
            D_004C6568[1] = 1.0f;
            D_004C6568[2] = 0.0f;
            D_004C6568[3] = 0.0f;
            D_004C6578[0] = 0.700706005f;
            D_004C6578[1] = 0.0f;
            D_004C6578[2] = 0.0f;
            D_004C6578[3] = 0.0f;
            D_004C6588[0] = 1.0f;
            D_004C6588[1] = 0.782853007f;
            D_004C6588[2] = 0.0614060014f;
            D_004C6588[3] = 0.0414629988f;
            D_004C6598[0] = 1.0f;
            D_004C6598[1] = 0.32487601f;
            D_004C6598[2] = 1.0f;
            D_004C6598[3] = 0.352800995f;
            D_004C65A8[0] = 1.0f;
            D_004C65A8[1] = 1.0f;
            D_004C65A8[2] = 0.797735989f;
            D_004C65A8[3] = 0.0f;
            D_004C65B8[0] = 1.0f;
            D_004C65B8[1] = 0.493748993f;
            D_004C65B8[2] = 1.0f;
            D_004C65B8[3] = 0.491797f;
            D_004C65C8[0] = 1.0f;
            D_004C65C8[1] = 1.0f;
            D_004C65C8[2] = 1.0f;
            D_004C65C8[3] = 1.0f;
            D_004C65D8[0] = 1.0f;
            D_004C65D8[1] = 0.800000012f;
            D_004C65D8[2] = 1.0f;
            D_004C65D8[3] = 0.800000012f;
            D_004C65E8[0] = 1.0f;
            D_004C65E8[1] = 1.0f;
            D_004C65E8[2] = 0.0f;
            D_004C65E8[3] = 0.0f;
            D_004C65F8[0] = 1.0f;
            D_004C65F8[1] = 1.0f;
            D_004C65F8[2] = 0.0f;
            D_004C65F8[3] = 0.0f;
            D_004C6608[0] = 1.0f;
            D_004C6608[1] = 1.0f;
            D_004C6608[2] = 0.0f;
            D_004C6608[3] = 0.0f;
            D_004C6618[0] = 1.0f;
            D_004C6618[1] = 0.327342004f;
            D_004C6618[2] = 1.0f;
            D_004C6618[3] = 0.353904992f;
            D_004C6628[0] = 1.0f;
            D_004C6628[1] = 1.0f;
            D_004C6628[2] = 0.0610000007f;
            D_004C6628[3] = 0.0390000008f;
            D_004C6638[0] = 1.0f;
            D_004C6638[1] = 1.0f;
            D_004C6638[2] = 0.799997985f;
            D_004C6638[3] = 0.0f;
            D_004C6648[0] = 1.0f;
            D_004C6648[1] = 0.958984017f;
            D_004C6648[2] = 0.614378989f;
            D_004C6648[3] = 0.0f;
            D_004C6658[0] = 1.0f;
            D_004C6658[1] = 0.928515017f;
            D_004C6658[2] = 0.339984f;
            D_004C6658[3] = 0.0f;
            D_004C6668[0] = 1.0f;
            D_004C6668[1] = 0.873825014f;
            D_004C6668[2] = 0.249317005f;
            D_004C6668[3] = 0.0175780002f;
            D_004C6678[0] = 1.0f;
            D_004C6678[1] = 0.784373999f;
            D_004C6678[2] = 0.0613280013f;
            D_004C6678[3] = 0.0394529998f;
            D_004C6688[0] = 1.0f;
            D_004C6688[1] = 0.0f;
            D_004C6688[2] = 1.0f;
            D_004C6688[3] = 0.0f;
            D_004C6698[0] = 1.0f;
            D_004C6698[1] = 1.0f;
            D_004C6698[2] = 0.0f;
            D_004C6698[3] = 0.0f;
            D_004A4E04 = 0;
            D_004A4E00 = 0;
            sE_init(&D_004A4E08, 0);
            sE_init(&D_004A4E10, 1);
            sE_init(&D_004A4E18, 2);
            sE_init(&D_004A4E20, 3);
            sE_init(&D_004A4E28, 4);
            sE_init(&D_004A4E30, 5);
            sE_init(&D_004A4E38, 6);
            sE_init(&D_004A4E40, 7);
            sE_init(&D_004A4E48, 8);
            sE_init(&D_004A4E50, 9);
            sE_init(&D_004A4E58, 10);
            sE_init(&D_004A4E60, 11);
            sE_init(&D_004A4E68, 12);
            sE_init(&D_004A4E70, 13);
            sE_init(&D_004A4E78, -1);
            gSmallSize[0] = 100.0f;
            gSmallSize[1] = 128.0f;
            gSmallSize[2] = 0.0f;
            gBigSize[0] = 130.0f;
            gBigSize[1] = 166.0f;
            gBigSize[2] = 0.0f;
            gStaticColour[0] = 0.5f;
            gStaticColour[1] = D_004C6338[3];
            gStaticColour[2] = D_004C6338[3];
            gStaticColour[3] = D_004C6338[3];
            gDisableColour[0] = 1.0f;
            gDisableColour[1] = 0.600000024f;
            gDisableColour[2] = 0.600000024f;
            gDisableColour[3] = 0.600000024f;
            kInvalidColour[0] = 1.0f;
            kInvalidColour[1] = 0.90196085f;
            kInvalidColour[2] = 0.117647067f;
            kInvalidColour[3] = 0.117647067f;
            kOriginalColour[0] = 1.0f;
            kOriginalColour[1] = 0.862745166f;
            kOriginalColour[2] = 0.34117648f;
            kOriginalColour[3] = D_004C6338[3];
            kColourEnabled[0] = 1.0f;
            kColourEnabled[1] = 1.0f;
            kColourEnabled[2] = 1.0f;
            kColourEnabled[3] = 1.0f;
            D_004C6718[0] = 1.0f;
            D_004C6718[1] = 0.800000012f;
            D_004C6718[2] = 0.200000003f;
            D_004C6718[3] = 0.200000003f;
            D_004C6728[0] = 1.0f;
            D_004C6728[1] = D_004C6338[3];
            D_004C6728[2] = 1.0f;
            D_004C6728[3] = D_004C6338[3];
            D_004C6738[0] = 1.0f;
            D_004C6738[1] = D_004C6338[3];
            D_004C6738[2] = D_004C6338[3];
            D_004C6738[3] = D_004C6338[3];
            D_004C6748[0] = 1.0f;
            D_004C6748[1] = 1.0f;
            D_004C6748[2] = 1.0f;
            D_004C6748[3] = D_004C6338[3];
            D_004C6758[0] = 1.0f;
            D_004C6758[1] = 1.0f;
            D_004C6758[2] = 1.0f;
            D_004C6758[3] = 1.0f;
            D_004C6768[0] = 1.0f;
            D_004C6768[1] = 0.5f;
            D_004C6768[2] = 1.0f;
            D_004C6768[3] = 1.0f;
            gFlowState = 0;
            D_004A4E84 = 0;
            D_004A4E88_v[0] = 0;
            D_004A4E88_v[1] = 0;
            D_004C6778[0] = 1.0f;
            D_004C6778[1] = D_004C6338[3];
            D_004C6778[2] = 0.100000001f;
            D_004C6778[3] = 0.899999976f;
            gPopupWhite[0] = 1.0f;
            gPopupWhite[1] = 1.0f;
            gPopupWhite[2] = 1.0f;
            gPopupWhite[3] = 1.0f;
            gPopupBlack[0] = 1.0f;
            gPopupBlack[1] = D_004C6338[3];
            gPopupBlack[2] = D_004C6338[3];
            gPopupBlack[3] = D_004C6338[3];
            gPopupBlue[0] = 1.0f;
            gPopupBlue[1] = 0.400000006f;
            gPopupBlue[2] = 0.720000029f;
            gPopupBlue[3] = 0.790000021f;
            gtStaticColor[0] = 0.400000006f;
            gtStaticColor[1] = D_004C6338[3];
            gtStaticColor[2] = D_004C6338[3];
            gtStaticColor[3] = D_004C6338[3];
            gtNotStatic[0] = 1.0f;
            gtNotStatic[1] = 1.0f;
            gtNotStatic[2] = 1.0f;
            gtNotStatic[3] = 1.0f;
            gtEnabled[0] = 1.0f;
            gtEnabled[1] = 1.0f;
            gtEnabled[2] = 0.5f;
            gtEnabled[3] = 0.25f;
            gtSelectedKey[0] = 1.0f;
            gtSelectedKey[1] = D_004C6338[3];
            gtSelectedKey[2] = D_004C6338[3];
            gtSelectedKey[3] = D_004C6338[3];
            gtNonSelectedKey[0] = 0.300000012f;
            gtNonSelectedKey[1] = D_004C6338[3];
            gtNonSelectedKey[2] = D_004C6338[3];
            gtNonSelectedKey[3] = D_004C6338[3];
            gSolidColour[0] = 1.0f;
            gSolidColour[1] = 1.0f;
            gSolidColour[2] = 1.0f;
            gSolidColour[3] = 1.0f;
            gFadedColour[0] = 0.600000024f;
            gFadedColour[1] = 1.0f;
            gFadedColour[2] = 1.0f;
            gFadedColour[3] = 1.0f;
            D_004C6828[0] = 1.0f;
            D_004C6828[1] = 1.0f;
            D_004C6828[2] = 1.0f;
            D_004C6828[3] = 1.0f;
            D_004C6838[0] = 1.0f;
            D_004C6838[1] = 1.0f;
            D_004C6838[2] = 1.0f;
            D_004C6838[3] = 1.0f;
        } else {
            cFEFlowState_purge(&gFlowState);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cAppModule__cAppModule);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* cAppModule_vtbl[];

/**
 * Destroys an app module base: stores the root module vtable cAppModule_vtbl at +0x0, then frees the object when asked to.
 *
 * cAppModule_vtbl is the vtable every screen and module (cPreStartScreen, cInitLoadScreen, the front end, cSSXApp's
 * secondary vtable) falls back to; its slots are the destructor, load, purge, loseFocus, gainFocus, update and render.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAppModule__cAppModule(cAppModule* self, int flags) {
    self->mVtbl = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

/**
 * Does nothing: the root module's default for vtable slot 0x1C, which the load screens, the front end and cSSXApp
 * inherit.
 */
extern "C" void cAppModule_purge(void) {
}

/**
 * Does nothing: the root module's default for vtable slot 0x2C, which the load screens, the front end and cSSXApp
 * inherit.
 */
extern "C" void cAppModule_gainFocus(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cDebugScreen__cDebugScreen);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* cDebugScreen_vtbl[];

/**
 * Destroys a debug screen: restores cDebugScreen's vtable at +0x0, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cDebugScreen__cDebugScreen(cDebugScreen* self, int flags) {
    self->mVtbl = cDebugScreen_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cDebugFE_getFont);
#ifdef SKIP_ASM
/**
 * Returns the debug front end's font (Font, +0x18), the one cDebugMenuScreen_draw reads from SSXApp's DebugFE.
 *
 * @return the font
 */
cFont* cDebugFE::getFont()
{
    return Font;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cDebugFE_getBoxTID);
#ifdef SKIP_ASM
/**
 * Returns the word at +0x20 of the debug front end, 8 bytes after its font word (cDebugFE_getFont).
 *
 * @return the word at +0x20
 */
int cDebugFE::getBoxTID()
{
    return (BoxTID);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cDebugFE_startlobby);
#ifdef SKIP_ASM
/**
 * Makes the lobby screen (Screens[1], +0x10) the debug front end's active screen (ActiveScreen, +0x14).
 *
 */
void cDebugFE::startlobby()
{
    ActiveScreen = Screens[1];
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFont_textf);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* cFont_textf_va_list;
#define cFont_textf_va_start(ap)                                                \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" void cFont_vtextf_C990(int, int, char*) __asm__("cFont_vtextf");

/**
 * Prints formatted text with a font at a position: a printf-style wrapper around cFont_vtextf.
 *
 * Builds the variable-argument list (EE EABI va_start) and passes the font, the format and the list to cFont_vtextf;
 * the x and y floats stay in their FPU argument registers. The debug menu screen and the 1-player HUD's time display
 * use it.
 *
 * @param x  left edge of the text
 * @param y  top of the text
 * @param font  the font (cFont, the method's this)
 * @param fmt  printf-style format string
 */
extern "C" void cFont_textf(float x, float y, int font, int fmt, ...)
{
    cFont_textf_va_list ap;
    cFont_textf_va_start(ap);
    cFont_vtextf_C990(font, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFont_textf1);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define cFont_textf1_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" void cFont_vtextf_C9D0(void *, int, char *) __asm__("cFont_vtextf");

struct V2_C9D0 { float x, y; };

/**
 * Prints formatted text with a font at a position and a temporary scale.
 *
 * Builds the variable-argument list, sets the font's current scale (+0x38, +0x3C) to its base scale (+0x30, +0x34)
 * times @p sx and @p sy, prints with cFont_vtextf (x and y stay in their FPU argument registers), then copies the base
 * scale back into the current scale. Used by the debug menu screen.
 *
 * The object is the font (cFont).
 *
 * @param fmt  printf-style format string
 * @param posX  left edge of the text
 * @param posY  top of the text
 * @param sx  horizontal scale factor
 * @param sy  vertical scale factor
 */
extern "C" void cFont_textf1(cFont* self, int fmt, float posX, float posY, float sx, float sy, ...)
{
    char *ap;
    cFont_textf1_va_start(ap);
    self->Scale[0] = self->BaseScale[0] * sx;
    self->Scale[1] = self->BaseScale[1] * sy;
    cFont_vtextf_C9D0(self, fmt, ap);
    *(V2_C9D0*)&self->Scale[0] = *(V2_C9D0*)&self->BaseScale[0];
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFont_centertextf);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* cFont_centertextf_va_list;
#define cFont_centertextf_va_start(ap)                                                \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" void cFont_vcentertextf_CA40(int, int, char*) __asm__("cFont_vcentertextf");

/**
 * Prints horizontally centred formatted text with a font: a printf-style wrapper around cFont_vcentertextf.
 *
 * Builds the variable-argument list and passes the font, the format and the list to cFont_vcentertextf; x and y stay in
 * their FPU argument registers.
 *
 * @param x  horizontal centre of the text
 * @param y  top of the text
 * @param font  the font (cFont, the method's this)
 * @param fmt  printf-style format string
 */
extern "C" void cFont_centertextf(float x, float y, int font, int fmt, ...)
{
    cFont_centertextf_va_list ap;
    cFont_centertextf_va_start(ap);
    cFont_vcentertextf_CA40(font, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFont_righttextf);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* cFont_righttextf_va_list;
#define cFont_righttextf_va_start(ap)                                                \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" void cFont_vrighttextf_CA80(int, int, char*) __asm__("cFont_vrighttextf");

/**
 * Prints right-aligned formatted text with a font: a printf-style wrapper around cFont_vrighttextf.
 *
 * Builds the variable-argument list and passes the font, the format and the list to cFont_vrighttextf; x and y stay in
 * their FPU argument registers.
 *
 * @param x  right edge of the text
 * @param y  top of the text
 * @param font  the font (cFont, the method's this)
 * @param fmt  printf-style format string
 */
extern "C" void cFont_righttextf(float x, float y, int font, int fmt, ...)
{
    cFont_righttextf_va_list ap;
    cFont_righttextf_va_start(ap);
    cFont_vrighttextf_CA80(font, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFECamera__cFECamera);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* cSimpleCamera_vtbl[];

/**
 * Destroys the front end's camera: stores cSimpleCamera's vtable at +0x90, then frees the object when asked to.
 *
 * It is slot 1 of cFECamera_vtbl, the vtable cFE_cFE stores at +0x90 of the camera it builds at +0x10 of the front end; the
 * derived class adds nothing to destroy, so only the base vtable store is left.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFECamera__cFECamera(cSimpleCamera* self, int flags) {
    self->mVtbl = cSimpleCamera_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPrivateMessage__cPrivateMessage);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void *, int);
extern int cPrivateMessage_vtbl[];

/**
 * Destroys a private message: its three strings, then frees the object when asked to.
 *
 * Stores the vtable cPrivateMessage_vtbl at +0xC, destroys the cBXStrings at +0x8, +0x4 and +0x0 (cBXString__cBXString with flags
 * 2: destroy without freeing) and calls operator_delete when bit 0 of @p flags is set.
 *
 * @param message  the message (0x10 bytes: three cBXStrings and the vptr at +0xC)
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPrivateMessage__cPrivateMessage(cPrivateMessage* message, int flags) {
    message->mVtbl = (void**)cPrivateMessage_vtbl;
    cBXString__cBXString(&message->mDate, 2);
    cBXString__cBXString(&message->mText, 2);
    cBXString__cBXString(message, 2);
    if (flags & 1) {
        operator_delete((int*)message);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getCamera);
#ifdef SKIP_ASM
/**
 * Returns the front end's camera, the member at +0x10.
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x10
 */
extern "C" int cFE_getCamera(int realFe) {
    return realFe + 0x10;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getCamera1);
#ifdef SKIP_ASM
/**
 * Returns the front end's camera, the member at +0x10 (the second of an identical pair of out-of-line copies).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x10
 */
extern "C" int cFE_getCamera1(int realFe) {
    return realFe + 0x10;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getHuman);
#ifdef SKIP_ASM
extern "C" void cFERiderManager_getHuman(int);

/**
 * Returns one human rider of the front end's rider manager.
 *
 * Calls cFERiderManager_getHuman on the rider manager at +0xB0; the rider index arrives in the second argument register and is
 * passed through untouched (cFERiderManager_getHuman returns manager + index * 0xCE0).
 *
 * @param realFe  the front end (cRealFE / cFE)
 */
extern "C" void cFE_getHuman(int realFe) {
    cFERiderManager_getHuman(realFe + 176);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getHuman1);
#ifdef SKIP_ASM
extern "C" void cFERiderManager_getHuman1(int);

/**
 * Returns one human rider of the front end's rider manager (the second of an identical pair of out-of-line copies).
 *
 * Calls cFERiderManager_getHuman1 on the rider manager at +0xB0 with the rider index passed through in the second argument
 * register.
 *
 * @param realFe  the front end (cRealFE / cFE)
 */
extern "C" void cFE_getHuman1(int realFe) {
    cFERiderManager_getHuman1(realFe + 176);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getRiderManager);
#ifdef SKIP_ASM
/**
 * Returns the front end's rider manager, the member at +0xB0.
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0xB0
 */
extern "C" int cFE_getRiderManager(int realFe) {
    return realFe + 176;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getRiderManager1);
#ifdef SKIP_ASM
/**
 * Returns the front end's rider manager, the member at +0xB0 (the second of an identical pair of out-of-line copies).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0xB0
 */
extern "C" int cFE_getRiderManager1(int realFe) {
    return realFe + 176;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getBoltFileManager);
#ifdef SKIP_ASM
/**
 * Returns the front end's member at +0x1A70 (the object cFE_cFE builds with func_0019CDF0).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x1A70
 */
extern "C" int cFE_getBoltFileManager(int realFe) {
    return realFe + 6768;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getBoltFileManager1);
#ifdef SKIP_ASM
/**
 * Returns the front end's member at +0x1A70 (the second of an identical pair of out-of-line copies).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x1A70
 */
extern "C" int cFE_getBoltFileManager1(int realFe) {
    return realFe + 6768;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFE_getUI);
#ifdef SKIP_ASM
/**
 * Returns the front end's UI engine, the pointer at +0xC.
 *
 * @return the UI engine pointer cFE_cFE allocates ("UIEngine") and stores at +0xC
 */
cUIEngine* cFE::getUI()
{
    return mpUI;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUILocaleInterface__cUILocaleInterface);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* cUILocaleInterface_vtbl[];

/**
 * Destroys a UI locale interface: stores its vtable cUILocaleInterface_vtbl at +0x4, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUILocaleInterface__cUILocaleInterface(cUILocaleInterface* self, int flags) {
    VPTR_OF(cUILocaleInterface, self) = cUILocaleInterface_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUILocaleInterface_setDisplayMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUILocaleInterface_setTextNumCharFlag_CC08(void*, int) __asm__("cUILocaleInterface_setTextNumCharFlag");

/**
 * Sets the locale's display mode (the byte at +0x0) and turns the text-num-char flag on for kDisplayNumChar.
 *
 * Stores @p mode at +0x0, then calls cUILocaleInterface_setTextNumCharFlag, which sets bit 3 of the attribute byte at
 * +0x3 (tAttrLocale's fourth flag, On Tour's mTextNumChar) when @p mode is kDisplayNumChar and clears it otherwise. It
 * is slot 0x14 of both the base locale vtable cUILocaleInterface_vtbl and the front-end locale's cFELocale_vtbl.
 *
 * The object is the locale interface.
 *
 * @param mode  the new display mode, cUILocaleInterface::eDisplayMode
 */
extern "C" void cUILocaleInterface_setDisplayMode(signed char *self, signed char mode) {
    *self = mode;
    cUILocaleInterface_setTextNumCharFlag_CC08(self, mode == cUILocaleInterface::kDisplayNumChar);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUILocaleInterface_setLanguage);
#ifdef SKIP_ASM
/**
 * Sets the locale's language: the byte at +0x1.
 *
 * The base locale's version (vtable cUILocaleInterface_vtbl, slot 0x1C); the front-end locale overrides it with cFELocale_setLanguage, which
 * also reloads the language files.
 *
 * @param self  the locale interface
 * @param language  the language index
 */
extern "C" void cUILocaleInterface_setLanguage(cFELocale* self, signed char language) {
    self->mLanguage = language;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", tAttrLocale_tAttrLocale);
#ifdef SKIP_ASM
/**
 * Constructs a locale attribute byte: all five locale flags off.
 *
 * Clears the low five bits of the byte (On Tour's mLocaleTest, mObjPivot, mTextBox, mTextNumChar and
 * mTextRefresh flags), keeping the top three.
 *
 * @param attr  the attribute byte (this)
 * @return @p attr
 */
extern "C" unsigned char *tAttrLocale_tAttrLocale(unsigned char *attr) {
    *attr &= 0xE0;
    return attr;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUILocaleInterface_getStringByHash);
#ifdef SKIP_ASM
/**
 * Returns 0: the base locale's default for vtable slot 0x24 (cUILocaleInterface_vtbl).
 *
 * @return always 0
 */
extern "C" int cUILocaleInterface_getStringByHash(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFECustom_cFECustom1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* cFECustom_vtbl_CC60[] __asm__("cFECustom_vtbl");

/**
 * Constructs an object with cFECustom's vtable: zeroes the word at +0x0 and stores cFECustom_vtbl at +0x4.
 *
 * No other field is set. No caller in the code; it is an out-of-line copy of an inline constructor.
 *
 * @return @p feCustom
 */
void * cFECustom::cFECustom1()
{
    mpEngine = 0;
    VPTR_OF(cUICustomInterface, this) = cFECustom_vtbl_CC60;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFECustom__cFECustom);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* cUICustomInterface_vtbl[];

/**
 * Destroys a front-end custom handler: stores the base vtable cUICustomInterface_vtbl at +0x4, then frees the object when asked to.
 *
 * It is slot 1 (the destructor) of cFECustom_vtbl; the derived class adds nothing to destroy.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFECustom__cFECustom(cFECustom* self, int flags) {
    VPTR_OF(cUICustomInterface, self) = cUICustomInterface_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

/**
 * Does nothing: cFECustom's per-frame update (slot 0x14 of cFECustom_vtbl).
 */
extern "C" void cFECustom_onUpdate(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFECustom_setProfileLaunchScreen);
#ifdef SKIP_ASM
/**
 * Stores a value in the word at +0xC of an object.
 *
 * No caller or vtable references it; it sits with the cFECustom helpers.
 *
 * @param launchScreen  the value to store
 */
void cFECustom::setProfileLaunchScreen(int launchScreen)
{
    ((int)mProfileLaunchScreen) = launchScreen;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cRealFE_setStartState);
#ifdef SKIP_ASM
extern int cRealFE_mStartState;

/**
 * Sets the front end's start state, the static cRealFE_mStartState.
 *
 * @param startState  the state to start the front end in
 */
extern "C" void cRealFE_setStartState(int startState) {
    cRealFE_mStartState = startState;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cRealFE_getStartState);
#ifdef SKIP_ASM
extern int cRealFE_mStartState;

/**
 * Returns the front end's start state, the static cRealFE_mStartState.
 *
 * @return cRealFE_mStartState
 */
extern "C" int cRealFE_getStartState(void) {
    return cRealFE_mStartState;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cRealFE_playAttractMovie);
#ifdef SKIP_ASM
extern int cRealFE_mMovieList;

/**
 * Sets bit 2 (value 4) of the front end's movie flags, the static cRealFE_mMovieList.
 */
extern "C" void cRealFE_playAttractMovie(void) {
    cRealFE_mMovieList |= 4;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cRealFE_getConnMan);
#ifdef SKIP_ASM
/**
 * Returns the front end's connection manager at +0xB5AB0 (the one cRealFE_cRealFE builds with cConnMan_cConnMan__FPv).
 *
 * @param realFe  the front end (cRealFE)
 * @return @p realFe + 0xB5AB0
 */
extern "C" int cRealFE_getConnMan(int realFe) {
    return realFe + 0xB5AB0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cRealFE_getGameConnMan);
#ifdef SKIP_ASM
/**
 * Returns the front end's game connection manager at +0xB5ABC (the one cRealFE_cRealFE builds with
 * cGameConnMan_cGameConnMan__FPv).
 *
 * @param realFe  the front end (cRealFE)
 * @return @p realFe + 0xB5ABC
 */
extern "C" int cRealFE_getGameConnMan(int realFe) {
    return realFe + 0xB5ABC;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIInput_setKey);
#ifdef SKIP_ASM
/**
 * Sets the key code this UI input reports.
 *
 * Stores @p key at +0x0 (On Tour cUIInput::mKey).
 *
 * @param input  the input (this)
 * @param key  the key code
 */
extern "C" void cUIInput_setKey(int *input, int key) {
    *input = key;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cDebugMenuScreen__cDebugMenuScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cBackgroundMan__cBackgroundMan_CD00(void *, int) __asm__("cBackgroundMan__cBackgroundMan");
void operator_delete(int*);
extern void* cDebugScreen_vtbl[];


/**
 * Destroys a debug menu screen: tears down its id list member, then runs the cDebugScreen part.
 *
 * cBackgroundMan__cBackgroundMan(self + 0x8C10, 2) releases each non-negative id in that list through the graphics manager's
 * virtual slot 50 and frees the array; then cDebugScreen's vtable is stored at +0x0.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cDebugMenuScreen__cDebugMenuScreen(cDebugMenuScreen* self, int flags) {
    cBackgroundMan__cBackgroundMan_CD00(&self->BackgroundMan, 2);
    *(void ***)self = cDebugScreen_vtbl;
    if (flags & 1) {
        operator_delete((int *)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", ContinueMenu__ContinueMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

/**
 * Destroys the debug lobby's "Continue" menu (vtable ContinueMenu_vtbl, built by ContinueMenu_ContinueMenu): its one item, then the menu.
 *
 * Destroys the item at +0x130 with cMenuItem__cMenuItem (flags 2: no free), then runs cMenu__cMenu with the caller's
 * flags. The July 2003 preview calls this class ContinueMenu.
 *
 * @param object  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void ContinueMenu__ContinueMenu(void* object, int flags) {
    ContinueMenu* menu = (ContinueMenu*)object;
    cMenuItem__cMenuItem(&menu->mContinueItem, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AccountDetailsMenu__AccountDetailsMenu1);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

/**
 * Destroys the debug lobby's "Login" menu (vtable AccountDetailsMenu1_vtbl, built by AccountDetailsMenu_AccountDetailsMenu1).
 *
 * Destroys its six items in reverse order (+0x1E4 Cancel, +0x1C0 the command item, +0x194 password, +0x168 login name,
 * +0x150 spacer, +0x134 title) with cMenuItem__cMenuItem (flags 2), then runs cMenu__cMenu with the caller's flags. The
 * preview calls this class AccountDetailsMenu<cDebugLobbyScreen::OnLogin>.
 *
 * @param object  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void AccountDetailsMenu__AccountDetailsMenu1(void* object, int flags) {
    AccountDetailsMenu* menu = (AccountDetailsMenu*)object;
    cMenuItem__cMenuItem(&menu->mCancelItem, 2);
    cMenuItem__cMenuItem(&menu->mActionItem, 2);
    cMenuItem__cMenuItem(&menu->mPasswordItem, 2);
    cMenuItem__cMenuItem(&menu->mLoginNameItem, 2);
    cMenuItem__cMenuItem(&menu->mSpaceItem, 2);
    cMenuItem__cMenuItem(&menu->mTitleItem, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AccountDetailsMenu__AccountDetailsMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

/**
 * Destroys the debug lobby's "Create Account" menu (vtable AccountDetailsMenu_vtbl, built by AccountDetailsMenu_AccountDetailsMenu).
 *
 * Same layout as the Login menu: destroys the six items from +0x1E4 down to +0x134 with cMenuItem__cMenuItem (flags 2),
 * then runs cMenu__cMenu with the caller's flags. The preview calls this class
 * AccountDetailsMenu<cDebugLobbyScreen::OnCreate>.
 *
 * @param object  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void AccountDetailsMenu__AccountDetailsMenu(void* object, int flags) {
    AccountDetailsMenu* menu = (AccountDetailsMenu*)object;
    cMenuItem__cMenuItem(&menu->mCancelItem, 2);
    cMenuItem__cMenuItem(&menu->mActionItem, 2);
    cMenuItem__cMenuItem(&menu->mPasswordItem, 2);
    cMenuItem__cMenuItem(&menu->mLoginNameItem, 2);
    cMenuItem__cMenuItem(&menu->mSpaceItem, 2);
    cMenuItem__cMenuItem(&menu->mTitleItem, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AccountMenu__AccountMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

// The rows of an embedded account menu (the same teardown as AccountDetailsMenu__AccountDetailsMenu, flags 2).
static inline void destroyAccountDetailsMenu(AccountDetailsMenu* m)
{
    cMenuItem__cMenuItem(&m->mCancelItem, 2);
    cMenuItem__cMenuItem(&m->mActionItem, 2);
    cMenuItem__cMenuItem(&m->mPasswordItem, 2);
    cMenuItem__cMenuItem(&m->mLoginNameItem, 2);
    cMenuItem__cMenuItem(&m->mSpaceItem, 2);
    cMenuItem__cMenuItem(&m->mTitleItem, 2);
    cMenu__cMenu(m, 2);
}

/**
 * Destroys the debug lobby's "Login Method" menu (vtable AccountMenu_vtbl, built by AccountMenu_AccountMenu) with its two submenus.
 *
 * Tears down, innermost first, the menu's four items (+0x578, +0x55C, +0x544, +0x528), the embedded Login menu at
 * +0x32C (its items and cMenu base) and the Create Account menu at +0x130 (its items and cMenu base), all with flags 2,
 * then runs cMenu__cMenu on itself with the caller's flags. The debug front end's destructor (cDebugLobbyScreen__cDebugLobbyScreen) calls it
 * for its member at +0x70. The preview calls this class AccountMenu<OnCreate, OnLogin>.
 *
 * @param flags  g++ destructor flags passed on to the last cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void AccountMenu__AccountMenu(AccountMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->mLoginItem, 2);
    cMenuItem__cMenuItem(&self->mCreateAccountItem, 2);
    cMenuItem__cMenuItem(&self->mSpaceItem, 2);
    cMenuItem__cMenuItem(&self->mTitleItem, 2);
    destroyAccountDetailsMenu(&self->mLoginMenu);
    destroyAccountDetailsMenu(&self->mCreateAccountMenu);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", DeletePersonaMenu__DeletePersonaMenu);
#ifdef SKIP_ASM
struct VEnt { short delta; short idx; void (*fn)(void*, int); };
struct VTab { int a; int b; VEnt dtor; };
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

/**
 * Destroys the debug lobby's "Delete Persona" menu (vtable DeletePersonaMenu_vtbl, built by DeletePersonaMenu_DeletePersonaMenu).
 *
 * Destroys the four command items at +0x168 (0x24 bytes each) in reverse order through each item's virtual destructor
 * (the vtable at +0x10 of the item, with the entry's this adjustment), then the spacer (+0x150) and the title (+0x134)
 * with cMenuItem__cMenuItem, and finally cMenu__cMenu with the caller's flags. The preview calls this class
 * DeletePersonaMenu<cDebugLobbyScreen::OnDeletePersona>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void DeletePersonaMenu__DeletePersonaMenu(DeletePersonaMenu* self, int flags) {
    if (((&self->mPersonaItems[0])) != 0) {
        cCommandMenuItem *end = ((&self->mPersonaItems[0])) + 4;
        cCommandMenuItem *elem = end;
        while (((&self->mPersonaItems[0])) != elem) {
            elem--;
            ((VTab*)elem->mVtbl)->dtor.fn(vthis(elem, ((VTab*)elem->mVtbl)->dtor), 0);
        }
    }
    cMenuItem__cMenuItem(&self->mSpaceItem, 2);
    cMenuItem__cMenuItem(&self->mTitleItem, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", CreatePersonaMenu__CreatePersonaMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

/**
 * Destroys the debug lobby's "Create Persona" menu (vtable CreatePersonaMenu_vtbl, built by CreatePersonaMenu_CreatePersonaMenu).
 *
 * Destroys its four items (+0x194 Create, +0x168 persona name, +0x150 spacer, +0x134 title) with cMenuItem__cMenuItem
 * (flags 2), then runs cMenu__cMenu with the caller's flags. The preview calls this class
 * CreatePersonaMenu<cDebugLobbyScreen::OnCreatePersona>.
 *
 * @param object  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void CreatePersonaMenu__CreatePersonaMenu(void* object, int flags) {
    CreatePersonaMenu* menu = (CreatePersonaMenu*)object;
    cMenuItem__cMenuItem(&menu->mCreateItem, 2);
    cMenuItem__cMenuItem(&menu->mPersonaNameItem, 2);
    cMenuItem__cMenuItem(&menu->mSpaceItem, 2);
    cMenuItem__cMenuItem(&menu->mTitleItem, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", PersonaMenu__PersonaMenu);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PersonaMenu<cDebugLobbyScreen::OnSelectPersona, OnCreatePersona, OnDeletePersona>::~PersonaMenu (prototype name);
// the compiler-generated member cleanup of a real class layout. Member classes are stand-ins named by their dtors.
struct cMenuPart_DD0C8 {                    // dtor cMenuItem__cMenuItem
    char pad[0x18];
    ~cMenuPart_DD0C8();
};
struct cMenuBase_DD0C8 {                    // dtor cMenu__cMenu
    char pad[0x130];
    ~cMenuBase_DD0C8();
};
struct cItemBase_DD0C8 {
    char pad[0x10];
    virtual ~cItemBase_DD0C8();
};
struct cItem_DD0C8 : cItemBase_DD0C8 {
    char pad2[0x10];
};
struct cSubMenu_DD0C8 : cMenuBase_DD0C8 {
    char pad134[0x4];
    cMenuPart_DD0C8 part134;                // 0x134
    char pad14C[0x4];
    cMenuPart_DD0C8 part150;                // 0x150
    cItem_DD0C8 items[4];                   // 0x168
};
struct cPartMenu_DD0C8 : cMenuBase_DD0C8 {
    char pad130[0x4];
    cMenuPart_DD0C8 part134;                // 0x134
    char pad14C[0x4];
    cMenuPart_DD0C8 part150;                // 0x150
    cMenuPart_DD0C8 part168;                // 0x168
    char pad180[0x14];
    cMenuPart_DD0C8 part194;                // 0x194
    char pad1AC[0xC];
};
struct cPersonaMenu_DD0C8 : cMenuBase_DD0C8 {
    cPartMenu_DD0C8 menu130;                // 0x130
    cSubMenu_DD0C8 sub2E8;                  // 0x2E8
    char pad4E0[0x4];
    cMenuPart_DD0C8 part4E4;                // 0x4E4
    char pad4FC[0x4];
    cMenuPart_DD0C8 part500;                // 0x500
    cItem_DD0C8 items518[4];                // 0x518
    cMenuPart_DD0C8 part5A8;                // 0x5A8
    char pad5C0[0x4];
    cMenuPart_DD0C8 part5C4;                // 0x5C4
    ~cPersonaMenu_DD0C8();
};

/**
 * Destroys the debug lobby's "Select Persona" menu (vtable PersonaMenu_vtbl, built by PersonaMenu_PersonaMenu) with its two submenus.
 *
 * A compiler-generated destructor: it destroys the menu's own items (the Create/Delete Persona submenu items at +0x5C4
 * and +0x5A8, the four persona items at +0x518 through their virtual destructors, the spacer and the title), then the
 * embedded Delete Persona menu at +0x2E8 and the Create Persona menu at +0x130, and finally the cMenu base, freeing the
 * object when the g++ delete flag is set. The preview calls this class PersonaMenu<cDebugLobbyScreen::OnSelectPersona,
 * OnCreatePersona, OnDeletePersona>.
 */
cPersonaMenu_DD0C8::~cPersonaMenu_DD0C8()
{
}
// PORT: the compiler names these dtors _$_<len><Class>; map them to the address-derived names.
__asm__(".globl PersonaMenu__PersonaMenu\n.type PersonaMenu__PersonaMenu,@function\nPersonaMenu__PersonaMenu = _$_18cPersonaMenu_DD0C8\n.size PersonaMenu__PersonaMenu,0x18C\n");
__asm__("_$_15cMenuPart_DD0C8 = cMenuItem__cMenuItem\n_$_15cMenuBase_DD0C8 = cMenu__cMenu\n");
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", LobbyMenu__LobbyMenu);
#ifdef SKIP_ASM

/**
 * Destroys the debug lobby's "Select Room" menu (vtable LobbyMenu_vtbl, built by LobbyMenu_LobbyMenu).
 *
 * Destroys the twelve room items at +0x168 in reverse order through their virtual destructors, then the spacer (+0x150)
 * and the title (+0x134) with cMenuItem__cMenuItem, and finally cMenu__cMenu with the caller's flags. The preview calls
 * this class LobbyMenu<cDebugLobbyScreen::OnSelectRoom>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void LobbyMenu__LobbyMenu(LobbyMenu* self, int flags) {
    if (((&self->mRoomItems[0])) != 0) {
        cCommandMenuItem *end = ((&self->mRoomItems[0])) + 12;
        cCommandMenuItem *elem = end;
        while (((&self->mRoomItems[0])) != elem) {
            elem--;
            ((VTab*)elem->mVtbl)->dtor.fn(vthis(elem, ((VTab*)elem->mVtbl)->dtor), 0);
        }
    }
    cMenuItem__cMenuItem(&self->mSpaceItem, 2);
    cMenuItem__cMenuItem(&self->mTitleItem, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", RoomMenu__RoomMenu);
#ifdef SKIP_ASM

/**
 * Destroys the debug lobby's "Room Occupants" menu (vtable RoomMenu_vtbl, built by RoomMenu_RoomMenu).
 *
 * Destroys the "Chat" item (+0x4CC), the 24 occupant items at +0x16C in reverse order through their virtual
 * destructors, the spacer (+0x154) and the title (+0x138), then runs cMenu__cMenu with the caller's flags. The preview
 * calls this class RoomMenu<cDebugLobbyScreen::OnSelectUser, OnChat>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void RoomMenu__RoomMenu(RoomMenu* self, int flags) {
    cMenuItem__cMenuItem(&self->mChatItem, 2);
    if (((&self->mUserItems[0])) != 0) {
        cCommandMenuItem *elem = ((&self->mUserItems[0])) + 24;
        while (((&self->mUserItems[0])) != elem) {
            elem--;
            ((VTab*)elem->mVtbl)->dtor.fn(vthis(elem, ((VTab*)elem->mVtbl)->dtor), 0);
        }
    }
    cMenuItem__cMenuItem(&self->mSpaceItem, 2);
    cMenuItem__cMenuItem(&self->mTitleItem, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", ChatMenu__ChatMenu);
#ifdef SKIP_ASM

/**
 * Destroys the debug lobby's "Chat Menu" (vtable ChatMenu_vtbl, built by ChatMenu_ChatMenu).
 *
 * Destroys the 14 canned-message items at +0x194 in reverse order through their virtual destructors, then the chat
 * text-entry item (+0x168), the spacer (+0x150) and the title (+0x134) with cMenuItem__cMenuItem, and finally
 * cMenu__cMenu with the caller's flags. The preview calls this class ChatMenu<cDebugLobbyScreen::OnMessage>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void ChatMenu__ChatMenu(ChatMenu* self, int flags) {
    if (((&self->mMessageItems[0])) != 0) {
        cCommandMenuItem *elem = ((&self->mMessageItems[0])) + 14;
        while (((&self->mMessageItems[0])) != elem) {
            elem--;
            ((VTab*)elem->mVtbl)->dtor.fn(vthis(elem, ((VTab*)elem->mVtbl)->dtor), 0);
        }
    }
    cMenuItem__cMenuItem(&self->mEnterChatItem, 2);
    cMenuItem__cMenuItem(&self->mSpaceItem, 2);
    cMenuItem__cMenuItem(&self->mTitleItem, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", ChallengeResponseMenu__ChallengeResponseMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

/**
 * Destroys the debug lobby's challenge menu (vtable D_0046D3D0, built by ChallengeResponseMenu_ChallengeResponseMenu).
 *
 * Destroys its six items in reverse order (+0x1C8 Decline, +0x1A4 Accept, +0x18C spacer, +0x170 empty line, +0x154
 * "by", +0x138 "You've been Challenged!") with cMenuItem__cMenuItem (flags 2), then runs cMenu__cMenu with the caller's
 * flags. The preview calls this class ChallengeResponseMenu<OnAcceptChallenge, OnDeclineChallenge>.
 *
 * @param object  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void ChallengeResponseMenu__ChallengeResponseMenu(void* object, int flags) {
    ChallengeResponseMenu* menu = (ChallengeResponseMenu*)object;
    cMenuItem__cMenuItem(&menu->mDeclineItem, 2);
    cMenuItem__cMenuItem(&menu->mAcceptItem, 2);
    cMenuItem__cMenuItem(&menu->mSpaceItem, 2);
    cMenuItem__cMenuItem(&menu->mChallengerItem, 2);
    cMenuItem__cMenuItem(&menu->mByItem, 2);
    cMenuItem__cMenuItem(&menu->mTitleItem, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", AbortMenu__AbortMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* item, int flags);
extern "C" void cMenu__cMenu(void* menu, int flags);

/**
 * Destroys the debug lobby's "Abort" menu (vtable AbortMenu_vtbl, built by AbortMenu_AbortMenu): its one item, then the menu.
 *
 * Destroys the command item at +0x134 with cMenuItem__cMenuItem (flags 2), then runs cMenu__cMenu with the caller's
 * flags. The preview calls this class AbortMenu<cDebugLobbyScreen::OnAbortChallenge>.
 *
 * @param object  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void AbortMenu__AbortMenu(void* object, int flags) {
    AbortMenu* menu = (AbortMenu*)object;
    cMenuItem__cMenuItem(&menu->mAbortItem, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEInput_cFEInput);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* cFEInput_vtbl_D508[] __asm__("cFEInput_vtbl");

/**
 * Constructs a front-end input reader: no key ready, no keys, no input device.
 *
 * Clears the pad-number byte at +0x4, stores cFEInput_vtbl at +0x8 and zeroes the words at +0xC (key ready), +0x10
 * (virtual key), +0x14 (ASCII key) and +0x18 (input device pointer). The load screens build the same object inline.
 *
 * @param self  the object
 * @return @p self
 */
extern "C" void *cFEInput_cFEInput(cFEInput* self) {
    self->mPadNum = 0;
    VPTR_OF(cUIInput, self) = cFEInput_vtbl_D508;
    self->mpInput = 0;
    self->mbKeyReady = 0;
    self->miVKey = 0;
    self->miAsciiKey = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEInput__cFEInput);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* cUIInput_vtbl[];

/**
 * Destroys a front-end input object: stores the base cUIInput vtable at +0x8, then frees it when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEInput__cFEInput(cFEInput* self, int flags) {
    VPTR_OF(cUIInput, self) = cUIInput_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEInput_getKeyReady);
#ifdef SKIP_ASM
/**
 * Tells whether a typed key is waiting: the key-ready flag at +0xC (On Tour mbKeyReady).
 *
 * @param input  the input (this)
 * @return nonzero when a key is ready
 */
extern "C" int cFEInput_getKeyReady(cFEInput* input) {
    return (input->mbKeyReady);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEInput_getVKey);
#ifdef SKIP_ASM
/**
 * Returns the virtual key code of the last typed key: +0x10 (On Tour miVKey).
 *
 * @param input  the input (this)
 * @return the virtual key code
 */
extern "C" int cFEInput_getVKey(cFEInput* input) {
    return (input->miVKey);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEInput_getAsciiKey);
#ifdef SKIP_ASM
/**
 * Returns the character code of the last typed key: +0x14 (On Tour miAsciiKey).
 *
 * @return the character code
 */
int cFEInput::getAsciiKey() const
{
    return (miAsciiKey);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEInput_getInput);
#ifdef SKIP_ASM
/**
 * Returns the front-end input's input device, mpInput (+0x18), which cFEInput_cFEInput clears.
 *
 * It follows cFEInput's out-of-line getters (cFEInput_getKeyReady, getVKey, getAsciiKey).
 *
 * @return the input device
 */
cInput* cFEInput::getInput()
{
    return mpInput;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", tAttrScreen_tAttrScreen);
#ifdef SKIP_ASM
/**
 * Constructs a UI screen's attribute word: sets bit 0 and clears bit 1, leaving the other bits.
 *
 * cUIScreen_cUIScreen and the screen-keyboard constructors call it on +0x14 of the screen; On Tour's tAttrScreen has
 * mFocused in bit 0 and mBufferAlloc in bit 1.
 *
 * @param attr  the attribute word (this)
 * @return @p attr
 */
extern "C" unsigned short *tAttrScreen_tAttrScreen(unsigned short *attr) {
    *attr = (*attr | 1) & 0xFFFD;
    return attr;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard__cScreenKeyboard);
#ifdef SKIP_ASM
extern "C" void cUIScreen__cUIScreen();

/**
 * Destroys an on-screen keyboard (vtable cScreenKeyboard_vtbl): chains straight to cUIScreen__cUIScreen.
 *
 * The object and the g++ delete flags pass through untouched in the argument registers.
 */
extern "C" void cScreenKeyboard__cScreenKeyboard(void) {
    cUIScreen__cUIScreen();
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_cScreenKeyboard);
#ifdef SKIP_ASM
extern "C" void cList_cList(void *);
extern "C" int cUIThing_cUIThing(void *, int, int);
extern int cScreenKeyboard_vtbl[];
extern int cUIScreen_vtbl[];
extern int cUIGroup_vtbl[];

/**
 * Constructs an on-screen keyboard's cUIScreen part with no owning state.
 *
 * Links the list node to itself (+0x0, +0x4), zeroes the buffer and header pointers (+0xC, +0x10), stores
 * cUIScreen_vtbl, builds the attribute word (tAttrScreen_tAttrScreen, +0x14) and the thread list (cList_cList, +0x18), zeroes the
 * keyframe, object-block and object-list pointers (+0x34..+0x3C), builds the root group (cUIThing_cUIThing at +0x40,
 * cUIGroup_vtbl at +0x48, its cList at +0xB4), zeroes the state pointer at +0xD0 and finally stores the keyboard vtable
 * cScreenKeyboard_vtbl. Unlike func_00180508 it sets none of the keyboard's own fields.
 *
 * @return @p self
 */
extern "C" void *cScreenKeyboard_cScreenKeyboard(cScreenKeyboard* self) {
    self->mpNext = (cListNode*)self;
    self->mpPrev = (cListNode*)self;
    self->mpBuffer = 0;
    self->mpScreenHeader = 0;
    self->mVtbl = (void**)cUIScreen_vtbl;
    tAttrScreen_tAttrScreen((unsigned short*)(&self->mAttrScreen));
    cList_cList(self->mThreads);
    self->mpKeyframeBlock = 0;
    self->mpObjectBlock = 0;
    self->mppObjectList = 0;
    cUIThing_cUIThing(self->mRootGroup, 0, 0);
    VPTR_OF(cListNode, self->mRootGroup) = (void**)cUIGroup_vtbl;
    cList_cList(&self->mRootGroup[116]);
    self->mpState = 0;
    self->mVtbl = (void**)cScreenKeyboard_vtbl;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_getInputText);
#ifdef SKIP_ASM
/**
 * Returns the on-screen keyboard's input text buffer at +0xEC.
 *
 * @param screen  the keyboard (vtable cScreenKeyboard_vtbl)
 * @return @p screen + 0xEC
 */
extern "C" int cScreenKeyboard_getInputText(int screen) {
    return screen + 236;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_getCurrentKey);
#ifdef SKIP_ASM
/**
 * Returns the on-screen keyboard's current key, the word at +0x170 (func_00180508 starts it at 10).
 *
 * @return the word at +0x170
 */
int cScreenKeyboard::getCurrentKey()
{
    return (mCurrentKey);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_setIncludeNumbers);
#ifdef SKIP_ASM
/**
 * Sets the on-screen keyboard's word at +0xD8 (func_00180508 starts it at 0).
 *
 * @param includeNumbers  the new value
 */
void cScreenKeyboard::setIncludeNumbers(int includeNumbers)
{
    mIncludeNumbers = includeNumbers;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_setIncludeShift);
#ifdef SKIP_ASM
/**
 * Sets the on-screen keyboard's word at +0xDC (func_00180508 starts it at 0).
 *
 * @param includeShift  the new value
 */
void cScreenKeyboard::setIncludeShift(int includeShift)
{
    mIncludeShift = includeShift;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_setIncludeSpace);
#ifdef SKIP_ASM
/**
 * Sets the on-screen keyboard's word at +0xE0 (func_00180508 starts it at 0).
 *
 * @param includeSpace  the new value
 */
void cScreenKeyboard::setIncludeSpace(int includeSpace)
{
    mIncludeSpace = includeSpace;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_cursorRight);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's text cursor one place right, unless that would reach the length limit.
 *
 * Increments the cursor position at +0x16C only while the result stays below the limit at +0xD4 (func_00180508 sets
 * 45).
 *
 */
void cScreenKeyboard::cursorRight()
{
    int temp_3;

    temp_3 = mCursorPos + 1;
    if (temp_3 < (mMaxChars)) {
        mCursorPos = temp_3;
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenKeyboard_cursorLeft);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's text cursor one place left, unless it is already at 0.
 *
 * Decrements the cursor position at +0x16C when it is above 0.
 *
 */
void cScreenKeyboard::cursorLeft()
{
    int temp_2;

    temp_2 = mCursorPos;
    if (temp_2 > 0) {
        mCursorPos = (int) (temp_2 - 1);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD6A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_D6A8(void*, int) __asm__("cUIState_cUIState");
extern void* cFEState_vtbl[];

/**
 * Constructs a front-end state with no UI engine: runs cUIState's constructor (cUIState_cUIState) with a null engine, then
 * stores cFEState's vtable cFEState_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void* func_001DD6A8(cFEState* self) {
    cUIState_cUIState_D6A8(self, 0);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD6E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_D6E0(void*) __asm__("cUIState_cUIState");
extern void* cFEState_vtbl[];

/**
 * Constructs a front-end state for a UI engine: runs cUIState's one-argument constructor (cUIState_cUIState), then stores
 * cFEState's vtable cFEState_vtbl at +0x8.
 *
 * The engine argument passes through untouched in the second argument register.
 *
 * @return @p self
 */
extern "C" void* func_001DD6E0(cFEState* self) {
    cUIState_cUIState_D6E0(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD718);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState1_D718(void*) __asm__("cUIState_cUIState1");
extern void* cFEState_vtbl[];

/**
 * Constructs a front-end state for a UI engine and a parent state: runs cUIState's two-argument constructor
 * (cUIState_cUIState1), then stores cFEState's vtable cFEState_vtbl at +0x8.
 *
 * The engine and parent arguments pass through untouched in the argument registers.
 *
 * @return @p self
 */
extern "C" void* func_001DD718(cFEState* self) {
    cUIState_cUIState1_D718(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEState__cFEState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DD750(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys a front-end state: restores cFEState's vtable cFEState_vtbl at +0x8 and runs cUIState's destructor
 * (cUIState__cUIState).
 *
 * The g++ delete flags pass through in the second argument register. cFEStateLodge, cFEStateLodgeRiderDetail,
 * cFEStateBuyAttrib, cFEStateCareerStats and cFEStateCredits destructors call it as their base destructor.
 */
extern "C" void cFEState__cFEState(cFEState* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DD750(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEState_setControllerFilter);
#ifdef SKIP_ASM
extern "C" int cFECustom_getPlayerCtrlMask(int, signed char);

/**
 * Narrows a front-end state's controller filter to the controllers no user claims.
 *
 * Starts from 0xFF and, for each user 0 .. @p n - 1, clears the bits of that user's controller mask, read with
 * cFECustom_getPlayerCtrlMask from the front-end handler (the first word of the UI engine at +0x10); stores the result in mCtrlBits
 * (+0x15).
 *
 * @param n  number of users whose masks to remove
 */
void cFEState::setControllerFilter(signed char n)
{
    int i = 0;
    int mask = 0xFF;
    int realFe = *(int*)mpEngine;
    if (n > 0) {
        do {
            mask &= ~cFECustom_getPlayerCtrlMask(realFe, i);
            i++;
        } while (i < n);
    }
    mCtrlBits = mask;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEState_setControllerMask);
#ifdef SKIP_ASM
extern "C" void cFECustom_setPlayerCtrlMask(int, signed char, int);

/**
 * Gives one user one controller: sets the user's controller mask to the single bit for @p controllerNum.
 *
 * Calls cFECustom_setPlayerCtrlMask on the front-end handler (the first word of the UI engine at +0x10) with @p userNum and 1
 * << @p controllerNum.
 *
 * @param userNum  the user
 * @param controllerNum  the controller port
 */
void cFEState::setControllerMask(signed char userNum, signed char controllerNum)
{
    cFECustom_setPlayerCtrlMask(*((int*)mpEngine), userNum, (1 << controllerNum) & 0xFF);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCharSelect__cFEStateCharSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DD840(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the character select front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateCharSelect__cFEStateCharSelect(cFEStateCharSelect* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DD840(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCheatCharSelect__cFEStateCheatCharSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DD868(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the cheat character select front-end state: restores cFEState's vtable (cFEState_vtbl) and runs
 * cUIState's destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateCheatCharSelect__cFEStateCheatCharSelect(cFEStateCheatCharSelect* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DD868(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCheatCharSelect_isDone);
#ifdef SKIP_ASM
/**
 * Tells whether the cheat character select has finished: returns the word at +0x74 (On Tour's mDone).
 *
 * @return non-zero once a choice has been made
 */
int cFEStateCheatCharSelect::isDone()
{
    return (mDone);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCheatCharSelect_getCheatCharID);
#ifdef SKIP_ASM
/**
 * Returns the cheat character select's chosen character, the word at +0x70 (On Tour's mCheatCharID).
 *
 * @return the chosen character id
 */
int cFEStateCheatCharSelect::getCheatCharID()
{
    return (mCheatCharID);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCharSetup__cFEStateCharSetup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DD8A0(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the character setup front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateCharSetup__cFEStateCharSetup(cFEStateCharSetup* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DD8A0(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateRiderDetail__cFEStateRiderDetail);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DD8C8(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the rider detail front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateRiderDetail__cFEStateRiderDetail(cFEStateRiderDetail* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DD8C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateUberTrick__cFEStateUberTrick);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DD8F0(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the uber-trick front-end state (vtable cFEStateUberTrick_vtbl): restores cFEState's vtable cFEState_vtbl and runs
 * cUIState's destructor (cUIState__cUIState).
 *
 * The g++ delete flags pass through in the second argument register. cFEStateUberTrick_vtbl is the vtable cFEStateUberTrick_cFEStateUberTrick stores in
 * the state allocated as "cFEStateUberTrick".
 */
extern "C" void cFEStateUberTrick__cFEStateUberTrick(cFEStateUberTrick* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DD8F0(self);
}
#endif

/**
 * Does nothing: the uber-trick state's onMenuGainFocus (slot 0x6C of its vtable cFEStateUberTrick_vtbl).
 */
extern "C" void cFEStateUberTrick_onMenuGainFocus(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setPopupType);
#ifdef SKIP_ASM
/**
 * Sets a popup description's type, meType (+0x0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param popupType  the type: -1 entry box, 0 no buttons, 1..5 options
 */
extern "C" void cPopupInfo_setPopupType(int *popupInfo, int popupType) {
    *popupInfo = popupType;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getPopupType);
#ifdef SKIP_ASM
/**
 * Returns a popup description's type, meType (+0x0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return the popup type
 */
extern "C" int cPopupInfo_getPopupType(int *popupInfo) {
    return *popupInfo;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setMiscData);
#ifdef SKIP_ASM
/**
 * Sets the id a popup reports back to its owner, miMiscData (+0x94).
 *
 * @param miscData  the value to report back
 */
void cPopupInfo::setMiscData(int miscData)
{
    miMiscData = miscData;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getMiscData);
#ifdef SKIP_ASM
/**
 * Returns the id a popup reports back to its owner, miMiscData (+0x94).
 *
 * @return miMiscData
 */
int cPopupInfo::getMiscData()
{
    return (miMiscData);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setPassUpdate);
#ifdef SKIP_ASM
/**
 * Sets whether the popup lets the state below it keep updating, mbPassUpdate (+0x98).
 *
 * @param passUpdate  non-zero to keep updating the parent state
 */
void cPopupInfo::setPassUpdate(int passUpdate)
{
    mbPassUpdate = passUpdate;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getPassUpdate);
#ifdef SKIP_ASM
/**
 * Returns whether the popup lets the state below it keep updating, mbPassUpdate (+0x98).
 *
 * @return mbPassUpdate
 */
int cPopupInfo::getPassUpdate()
{
    return (mbPassUpdate);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setOffset);
#ifdef SKIP_ASM
/**
 * Sets a popup description's screen offset, mvOffset (+0x9C), from a vector passed by value.
 *
 * @param offset  the offset (x, y, z)
 */
extern "C" void cPopupInfo_setOffset(cPopupInfo* self, t3Vector offset) {
    self->mvOffset = offset;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getOffset);
#ifdef SKIP_ASM
/**
 * Returns a popup description's screen offset, mvOffset (+0x9C), by value.
 *
 * Copies the three words into the caller's result buffer.
 *
 * @param dst  the result buffer (the hidden return-value pointer)
 * @param src  the popup description (cPopupInfo)
 * @return @p dst
 */
extern "C" t3Vector* cPopupInfo_getOffset(t3Vector* dst, cPopupInfo* src) {
    *dst = src->mvOffset;
    return dst;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setDefaultOption);
#ifdef SKIP_ASM
/**
 * Sets the option a popup starts on, miDefaultOption (+0xAC).
 *
 * @param defaultOption  the option index (clear() sets -1)
 */
void cPopupInfo::setDefaultOption(int defaultOption)
{
    miDefaultOption = defaultOption;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getDefaultOption);
#ifdef SKIP_ASM
/**
 * Returns the option a popup starts on, miDefaultOption (+0xAC).
 *
 * @return miDefaultOption
 */
int cPopupInfo::getDefaultOption()
{
    return (miDefaultOption);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setReceivePrevCommand);
#ifdef SKIP_ASM
/**
 * Sets whether the popup reacts to the back command, mbReceivePrev (+0xA8).
 *
 * @param receivePrev  non-zero to receive the back command (clear() sets 1)
 */
void cPopupInfo::setReceivePrevCommand(int receivePrev)
{
    mbReceivePrev = receivePrev;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getReceivePrevCommand);
#ifdef SKIP_ASM
/**
 * Returns whether the popup reacts to the back command, mbReceivePrev (+0xA8).
 *
 * @return mbReceivePrev
 */
int cPopupInfo::getReceivePrevCommand()
{
    return (mbReceivePrev);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setLayerOffset);
#ifdef SKIP_ASM
/**
 * Sets a popup's draw-layer offset, miLayerOffset (+0x90).
 *
 * @param layerOffset  the layer offset
 */
void cPopupInfo::setLayerOffset(int layerOffset)
{
    miLayerOffset = layerOffset;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getLayerOffset);
#ifdef SKIP_ASM
/**
 * Returns a popup's draw-layer offset, miLayerOffset (+0x90).
 *
 * @return miLayerOffset
 */
int cPopupInfo::getLayerOffset()
{
    return (miLayerOffset);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setBackgroundType);
#ifdef SKIP_ASM
/**
 * Sets a popup's background style, meBackgroundType (+0xC0).
 *
 * @param backgroundType  the background (0 online, 1 blue, 2 black)
 */
void cPopupInfo::setBackgroundType(int backgroundType)
{
    meBackgroundType = backgroundType;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getBackgroundType);
#ifdef SKIP_ASM
/**
 * Returns a popup's background style, meBackgroundType (+0xC0).
 *
 * @return meBackgroundType
 */
int cPopupInfo::getBackgroundType()
{
    return (meBackgroundType);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setMaxCharacters);
#ifdef SKIP_ASM
/**
 * Sets the most characters a popup's entry box takes, miMaxChars (+0xC4).
 *
 * @param maxChars  the character limit
 */
void cPopupInfo::setMaxCharacters(int maxChars)
{
    miMaxChars = maxChars;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getMaxCharacters);
#ifdef SKIP_ASM
/**
 * Returns the most characters a popup's entry box takes, miMaxChars (+0xC4).
 *
 * @return miMaxChars
 */
int cPopupInfo::getMaxCharacters()
{
    return (miMaxChars);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setMinBackgroundSize);
#ifdef SKIP_ASM
/**
 * Sets a popup's minimum background size, mfMinSize (+0xB0).
 *
 * @param fparg0  the minimum size
 */
void cPopupInfo::setMinBackgroundSize(float fparg0)
{
    mfMinSize = fparg0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getMinBackgroundSize);
#ifdef SKIP_ASM
/**
 * Returns a popup's minimum background size, mfMinSize (+0xB0).
 *
 * @return mfMinSize
 */
float cPopupInfo::getMinBackgroundSize()
{
    return (mfMinSize);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setUseCenterAlignedOptions);
#ifdef SKIP_ASM
/**
 * Sets whether a popup centres its options, mbCenterOptions (+0xB8).
 *
 * @param centerOptions  non-zero to centre the options
 */
void cPopupInfo::setUseCenterAlignedOptions(int centerOptions)
{
    mbCenterOptions = centerOptions;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getUseCenterAlignedOptions);
#ifdef SKIP_ASM
/**
 * Returns whether a popup centres its options, mbCenterOptions (+0xB8).
 *
 * @return mbCenterOptions
 */
int cPopupInfo::getUseCenterAlignedOptions()
{
    return (mbCenterOptions);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setUseCenterAlignedText);
#ifdef SKIP_ASM
/**
 * Sets whether a popup centres its text, mbCenterText (+0xBC).
 *
 * @param centerText  non-zero to centre the text (clear() sets 1)
 */
void cPopupInfo::setUseCenterAlignedText(int centerText)
{
    mbCenterText = centerText;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getUseCenterAlignedText);
#ifdef SKIP_ASM
/**
 * Returns whether a popup centres its text, mbCenterText (+0xBC).
 *
 * @return mbCenterText
 */
int cPopupInfo::getUseCenterAlignedText()
{
    return (mbCenterText);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA30);
#ifdef SKIP_ASM
/**
 * Sets a popup's message to an ASCII string: mtMessage (+0x4) becomes type 2 with the string pointer at +0x8.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param message  the ASCII string
 */
extern "C" void func_001DDA30(cPopupInfo* popupInfo, int message) {
    ((int)popupInfo->mtMessage.ui) = message;
    popupInfo->mtMessage.eType = 2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA40);
#ifdef SKIP_ASM
/**
 * Sets a popup's message to a Unicode string: mtMessage (+0x4) becomes type 1 with the string pointer at +0x8.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param message  the 16-bit string
 */
extern "C" void func_001DDA40(cPopupInfo* popupInfo, int message) {
    ((int)popupInfo->mtMessage.ui) = message;
    popupInfo->mtMessage.eType = 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA50);
#ifdef SKIP_ASM
/**
 * Sets a popup's message to a localised text id: mtMessage (+0x4) becomes type 3 with the id at +0x8.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param messageId  the text hash id
 */
extern "C" void func_001DDA50(cPopupInfo* popupInfo, int messageId) {
    ((int)popupInfo->mtMessage.ui) = messageId;
    popupInfo->mtMessage.eType = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA60);
#ifdef SKIP_ASM
/**
 * Copies a whole popup element (type and value, 8 bytes) into a popup's message, mtMessage (+0x4).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDA60(cPopupInfo* dst, tPopupElement* src) {
    dst->mtMessage = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getPopupMessage);
#ifdef SKIP_ASM
/**
 * Returns a popup's message element, mtMessage (+0x4).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 4
 */
extern "C" int cPopupInfo_getPopupMessage(int popupInfo) {
    return popupInfo + 4;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA80);
#ifdef SKIP_ASM
/**
 * Sets a popup's title to an ASCII string: mtTitle (+0xC) becomes type 2 with the string pointer at +0x10.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param title  the ASCII string
 */
extern "C" void func_001DDA80(cPopupInfo* popupInfo, int title) {
    ((int)popupInfo->mtTitle.ui) = title;
    popupInfo->mtTitle.eType = 2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA90);
#ifdef SKIP_ASM
/**
 * Sets a popup's title to a Unicode string: mtTitle (+0xC) becomes type 1 with the string pointer at +0x10.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param title  the 16-bit string
 */
extern "C" void func_001DDA90(cPopupInfo* popupInfo, int title) {
    ((int)popupInfo->mtTitle.ui) = title;
    popupInfo->mtTitle.eType = 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDAA0);
#ifdef SKIP_ASM
/**
 * Sets a popup's title to a localised text id: mtTitle (+0xC) becomes type 3 with the id at +0x10.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param titleId  the text hash id
 */
extern "C" void func_001DDAA0(cPopupInfo* popupInfo, int titleId) {
    ((int)popupInfo->mtTitle.ui) = titleId;
    popupInfo->mtTitle.eType = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDAB0);
#ifdef SKIP_ASM
/**
 * Copies a whole popup element (type and value) into a popup's title, mtTitle (+0xC).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDAB0(cPopupInfo* dst, tPopupElement* src) {
    dst->mtTitle = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getPopupTitle);
#ifdef SKIP_ASM
/**
 * Returns a popup's title element, mtTitle (+0xC).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0xC
 */
extern "C" int cPopupInfo_getPopupTitle(int popupInfo) {
    return popupInfo + 0xC;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDAD0);
#ifdef SKIP_ASM
/**
 * Sets a popup's entry-box title to an ASCII string: mtEntryBoxTitle (+0x14) becomes type 2 with the pointer at +0x18.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param entryBoxTitle  the ASCII string
 */
extern "C" void func_001DDAD0(cPopupInfo* popupInfo, int entryBoxTitle) {
    ((int)popupInfo->mtEntryBoxTitle.ui) = entryBoxTitle;
    popupInfo->mtEntryBoxTitle.eType = 2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDAE0);
#ifdef SKIP_ASM
/**
 * Sets a popup's entry-box title to a Unicode string: mtEntryBoxTitle (+0x14) becomes type 1 with the pointer at +0x18.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param entryBoxTitle  the 16-bit string
 */
extern "C" void func_001DDAE0(cPopupInfo* popupInfo, int entryBoxTitle) {
    ((int)popupInfo->mtEntryBoxTitle.ui) = entryBoxTitle;
    popupInfo->mtEntryBoxTitle.eType = 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDAF0);
#ifdef SKIP_ASM
/**
 * Sets a popup's entry-box title to a localised text id: mtEntryBoxTitle (+0x14) becomes type 3 with the id at +0x18.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param entryBoxTitleId  the text hash id
 */
extern "C" void func_001DDAF0(cPopupInfo* popupInfo, int entryBoxTitleId) {
    ((int)popupInfo->mtEntryBoxTitle.ui) = entryBoxTitleId;
    popupInfo->mtEntryBoxTitle.eType = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB00);
#ifdef SKIP_ASM
/**
 * Copies a whole popup element (type and value) into a popup's entry-box title, mtEntryBoxTitle (+0x14).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDB00(cPopupInfo* dst, tPopupElement* src) {
    dst->mtEntryBoxTitle = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getPopupEntryBoxTitle);
#ifdef SKIP_ASM
/**
 * Returns a popup's entry-box title element, mtEntryBoxTitle (+0x14).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0x14
 */
extern "C" int cPopupInfo_getPopupEntryBoxTitle(int popupInfo) {
    return popupInfo + 20;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB20);
#ifdef SKIP_ASM
/**
 * Sets the text a popup's entry box starts with to an ASCII string: mtEntryBoxInitial (+0x1C) becomes type 2 with the
 * pointer at +0x20.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param entryBoxInitial  the ASCII string
 */
extern "C" void func_001DDB20(cPopupInfo* popupInfo, int entryBoxInitial) {
    ((int)popupInfo->mtEntryBoxInitial.ui) = entryBoxInitial;
    popupInfo->mtEntryBoxInitial.eType = 2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB30);
#ifdef SKIP_ASM
/**
 * Sets the text a popup's entry box starts with to a Unicode string: mtEntryBoxInitial (+0x1C) becomes type 1 with the
 * pointer at +0x20.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param entryBoxInitial  the 16-bit string
 */
extern "C" void func_001DDB30(cPopupInfo* popupInfo, int entryBoxInitial) {
    ((int)popupInfo->mtEntryBoxInitial.ui) = entryBoxInitial;
    popupInfo->mtEntryBoxInitial.eType = 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB40);
#ifdef SKIP_ASM
/**
 * Sets the text a popup's entry box starts with to a localised text id: mtEntryBoxInitial (+0x1C) becomes type 3 with
 * the id at +0x20.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param entryBoxInitialId  the text hash id
 */
extern "C" void func_001DDB40(cPopupInfo* popupInfo, int entryBoxInitialId) {
    ((int)popupInfo->mtEntryBoxInitial.ui) = entryBoxInitialId;
    popupInfo->mtEntryBoxInitial.eType = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB50);
#ifdef SKIP_ASM
/**
 * Copies a whole popup element (type and value) into the text a popup's entry box starts with, mtEntryBoxInitial
 * (+0x1C).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDB50(cPopupInfo* dst, tPopupElement* src) {
    dst->mtEntryBoxInitial = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getPopupEntryBoxInitial);
#ifdef SKIP_ASM
/**
 * Returns the element holding the text a popup's entry box starts with, mtEntryBoxInitial (+0x1C).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0x1C
 */
extern "C" int cPopupInfo_getPopupEntryBoxInitial(int popupInfo) {
    return popupInfo + 0x1C;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB70);
#ifdef SKIP_ASM
/**
 * Sets a popup's "post pending" text to an ASCII string: mtPostPending (+0x4C) becomes type 2 with the pointer at
 * +0x50.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param postPending  the ASCII string
 */
extern "C" void func_001DDB70(cPopupInfo* popupInfo, int postPending) {
    ((int)popupInfo->mtPostPending.ui) = postPending;
    popupInfo->mtPostPending.eType = 2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB80);
#ifdef SKIP_ASM
/**
 * Sets a popup's "post pending" text to a Unicode string: mtPostPending (+0x4C) becomes type 1 with the pointer at
 * +0x50.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param postPending  the 16-bit string
 */
extern "C" void func_001DDB80(cPopupInfo* popupInfo, int postPending) {
    ((int)popupInfo->mtPostPending.ui) = postPending;
    popupInfo->mtPostPending.eType = 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB90);
#ifdef SKIP_ASM
/**
 * Sets a popup's "post pending" text to a localised text id: mtPostPending (+0x4C) becomes type 3 with the id at +0x50.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param postPendingId  the text hash id
 */
extern "C" void func_001DDB90(cPopupInfo* popupInfo, int postPendingId) {
    ((int)popupInfo->mtPostPending.ui) = postPendingId;
    popupInfo->mtPostPending.eType = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDBA0);
#ifdef SKIP_ASM
/**
 * Copies a whole popup element (type and value) into a popup's "post pending" text, mtPostPending (+0x4C).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDBA0(cPopupInfo* dst, tPopupElement* src) {
    dst->mtPostPending = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getPopupPostPending);
#ifdef SKIP_ASM
/**
 * Returns a popup's "post pending" text element, mtPostPending (+0x4C).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0x4C
 */
extern "C" int cPopupInfo_getPopupPostPending(int popupInfo) {
    return popupInfo + 76;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_setDeferEventsFlag);
#ifdef SKIP_ASM
/**
 * Sets a popup description's mbDeferEventsFlag (+0xB4); the online states set it to 1.
 *
 * @param deferEvents  the new flag value
 */
void cPopupInfo::setDeferEventsFlag(int deferEvents)
{
    mbDeferEventsFlag = deferEvents;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cPopupInfo_getDeferEventsFlag);
#ifdef SKIP_ASM
/**
 * Returns a popup description's mbDeferEventsFlag (+0xB4).
 *
 * @return mbDeferEventsFlag
 */
int cPopupInfo::getDeferEventsFlag()
{
    return (mbDeferEventsFlag);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenPopup_cScreenPopup1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState1_DBD0(void *, int, int) __asm__("cUIState_cUIState1");
extern "C" void cPopupInfo_cPopupInfo_DBD0(void *) __asm__("cPopupInfo_cPopupInfo");
extern int cScreenPopup_vtbl[];
extern char* cBXString_EmptyString;

/**
 * Constructs a screen popup with no engine and no parent (the default constructor).
 *
 * Runs cUIState's two-argument constructor (cUIState_cUIState1) with null engine and parent, stores cScreenPopup_vtbl at
 * +0x8, constructs the popup description at +0xBC (cPopupInfo_cPopupInfo) and sets the input and static text strings
 * (+0x2B0, +0x2B4) to the empty cBXString.
 *
 * @return @p self
 */
cScreenPopup * cScreenPopup::cScreenPopup1()
{
    cUIState_cUIState1_DBD0((this), 0, 0);
    VPTR_OF(cListNode, this) = (void**)cScreenPopup_vtbl;
    cPopupInfo_cPopupInfo_DBD0((char*)&info);
    mszInputText.Str = cBXString_EmptyString;
    mszStaticText.Str = cBXString_EmptyString;
    return (this);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenPopup_getPopupInfo);
#ifdef SKIP_ASM
/**
 * Returns a screen popup's description, the cPopupInfo at +0xBC.
 *
 * @param screenPopup  the popup (cScreenPopup)
 * @return @p screenPopup + 0xBC
 */
extern "C" int cScreenPopup_getPopupInfo(int screenPopup) {
    return screenPopup + 188;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenPopup_setPassUpdate);
#ifdef SKIP_ASM
/**
 * Sets whether a screen popup lets the state below it keep updating, mbPassUpdate (+0x2C0).
 *
 * @param passUpdate  non-zero to keep updating the parent state
 */
void cScreenPopup::setPassUpdate(int passUpdate)
{
    mbPassUpdate = passUpdate;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenPopup_getPassUpdate);
#ifdef SKIP_ASM
/**
 * Returns whether a screen popup lets the state below it keep updating, mbPassUpdate (+0x2C0).
 *
 * @return mbPassUpdate
 */
int cScreenPopup::getPassUpdate()
{
    return (mbPassUpdate);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenPopup_getEntryBoxText);
#ifdef SKIP_ASM
/**
 * Returns the text typed into a screen popup's entry box, the string mszInputText (+0x2B0).
 *
 * @return the input text
 */
char* cScreenPopup::getEntryBoxText()
{
    return mszInputText.Str;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cScreenPopup_isShowing);
#ifdef SKIP_ASM
/**
 * Tells whether a screen popup is on screen: returns mbShowing (+0x2B8), which onCreateScreen sets and onDestroyScreen
 * clears.
 *
 * @return mbShowing
 */
int cScreenPopup::isShowing()
{
    return (mbShowing);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup_cUIStateBuyPopup1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState1_DC48(void *, int, int) __asm__("cUIState_cUIState1");
extern "C" void cBuyPopupInfo_clear_DC48(void *) __asm__("cBuyPopupInfo_clear");
extern int cUIStateBuyPopup_vtbl[];

/**
 * Constructs a buy popup with no engine and no parent (the default constructor).
 *
 * Runs cUIState's two-argument constructor (cUIState_cUIState1) with null engine and parent, stores cUIStateBuyPopup_vtbl
 * and clears the purchase description at +0x48 with cBuyPopupInfo_clear.
 *
 * @return @p self
 */
void * cUIStateBuyPopup::cUIStateBuyPopup1()
{
    cUIState_cUIState1_DC48(this, 0, 0);
    VPTR_OF(cListNode, this) = (void**)cUIStateBuyPopup_vtbl;
    cBuyPopupInfo_clear_DC48((char*)&mInfo);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup__cUIStateBuyPopup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DDC90(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the buy popup front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cUIStateBuyPopup__cUIStateBuyPopup(cUIStateBuyPopup* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DDC90(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup_getPopupInfo);
#ifdef SKIP_ASM
/**
 * Returns a buy popup's purchase description, the cBuyPopupInfo at +0x48.
 *
 * @param buyPopup  the popup (cUIStateBuyPopup)
 * @return @p buyPopup + 0x48
 */
extern "C" int cUIStateBuyPopup_getPopupInfo(int buyPopup) {
    return buyPopup + 72;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup_isAccept);
#ifdef SKIP_ASM
/**
 * Tells whether the player accepted the purchase: returns mbAccept.
 *
 * @return mbAccept
 */
int cUIStateBuyPopup::isAccept()
{
    return (mbAccept);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup_initBuySongByCredit);
#ifdef SKIP_ASM
extern "C" void cBuyPopupInfo_initBuySongByCredit(int);

/**
 * Sets the buy popup up to buy a song with credits: calls cBuyPopupInfo_initBuySongByCredit on the description at
 * +0x48.
 *
 * The song name and price pass through untouched in the remaining argument registers.
 *
 * @param buyPopup  the popup (cUIStateBuyPopup)
 */
extern "C" void cUIStateBuyPopup_initBuySongByCredit(int buyPopup) {
    cBuyPopupInfo_initBuySongByCredit(buyPopup + 72);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup_initBuySong);
#ifdef SKIP_ASM
extern "C" void cBuyPopupInfo_initBuySong(int);

/**
 * Sets the buy popup up to buy a song: calls cBuyPopupInfo_initBuySong on the description at +0x48.
 *
 * The song arguments pass through untouched in the remaining argument registers.
 *
 * @param buyPopup  the popup (cUIStateBuyPopup)
 */
extern "C" void cUIStateBuyPopup_initBuySong(int buyPopup) {
    cBuyPopupInfo_initBuySong(buyPopup + 72);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup_initBuyReward);
#ifdef SKIP_ASM
extern "C" void cBuyPopupInfo_initBuyReward(int);

/**
 * Sets the buy popup up to buy a reward item: calls cBuyPopupInfo_initBuyReward on the description at +0x48.
 *
 * The item arguments pass through untouched in the remaining argument registers (cBuyPopupInfo_initBuyReward fills the description
 * with the item name and the "kT_CMNItemColon" / "kT_OVRCMNBuyItem" texts).
 *
 * @param buyPopup  the popup (cUIStateBuyPopup)
 */
extern "C" void cUIStateBuyPopup_initBuyReward(int buyPopup) {
    cBuyPopupInfo_initBuyReward(buyPopup + 72);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isSavePressed);
#ifdef SKIP_ASM

/**
 * Reports whether Save was pressed on the current memory-card screen: returns the word at +0x1DC of
 * cFEMemCard_mInstance.
 *
 * cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsSavePressed (+0x64).
 *
 * @return the save-pressed word
 */
extern "C" int cFEMemCard_isSavePressed(void) {
    return (cFEMemCard_mInstance->mIsSavePressed);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isLoadPressed);
#ifdef SKIP_ASM

/**
 * Reports whether Load was pressed on the current memory-card screen: returns the word at +0x1F4 of
 * cFEMemCard_mInstance.
 *
 * cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsLoadPressed (+0x78).
 *
 * @return the load-pressed word
 */
extern "C" int cFEMemCard_isLoadPressed(void) {
    return (cFEMemCard_mInstance->mIsLoadPressed);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isDeletePressed);
#ifdef SKIP_ASM

/**
 * Reports whether Delete was pressed on the current memory-card screen: returns the word at +0x1E0 of
 * cFEMemCard_mInstance.
 *
 * cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsDeletePressed (+0x68).
 *
 * @return the delete-pressed word
 */
extern "C" int cFEMemCard_isDeletePressed(void) {
    return (cFEMemCard_mInstance->mIsDeletePressed);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isCancelPressed);
#ifdef SKIP_ASM

/**
 * Reports whether Cancel was pressed on the current memory-card screen, and consumes it.
 *
 * Returns the word at +0x1E4 of cFEMemCard_mInstance and clears it. cFEMemCard_initFEMemCard installs it as the memory-card
 * manager's callbackIsCancelPressed (+0x6C).
 *
 * @return the cancel-pressed word before it was cleared
 */
extern "C" int cFEMemCard_isCancelPressed(void) {
    int temp_2;

    temp_2 = cFEMemCard_mInstance->mIsCancelPressed;
    cFEMemCard_mInstance->mIsCancelPressed = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isOWPressed);
#ifdef SKIP_ASM

/**
 * Reports whether Overwrite was confirmed on the current memory-card screen: returns the word at +0x1F0 of
 * cFEMemCard_mInstance.
 *
 * cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsOverwritePressed (+0x70).
 *
 * @return the overwrite-pressed word
 */
extern "C" int cFEMemCard_isOWPressed(void) {
    return (cFEMemCard_mInstance->mIsOverwriteOK);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isDeviceSelected);
#ifdef SKIP_ASM

/**
 * Reports whether a memory-card device is selected: returns the word at +0x1A8 of cFEMemCard_mInstance.
 *
 * cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsDeviceSelected (+0x74). The same word is the "busy"
 * flag that makes cFEMemCard_onInputBegin ignore input.
 *
 * @return the device-selected word
 */
extern "C" int cFEMemCard_isDeviceSelected(void) {
    return (cFEMemCard_mInstance->mIsProcessing);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isNextMCPressed);
#ifdef SKIP_ASM

/**
 * Reports whether the next memory card was requested, and consumes it.
 *
 * Returns the word at +0x1F8 of cFEMemCard_mInstance and clears it; cFEMemCard_onInputBegin sets the word when right is
 * pressed. cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsNextDevicePressed (+0x7C).
 *
 * @return the next-device word before it was cleared
 */
extern "C" int cFEMemCard_isNextMCPressed(void) {
    int temp_2;

    temp_2 = cFEMemCard_mInstance->mDoNextMemCard;
    cFEMemCard_mInstance->mDoNextMemCard = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isPrevMCPressed);
#ifdef SKIP_ASM

/**
 * Reports whether the previous memory card was requested, and consumes it.
 *
 * Returns the word at +0x1FC of cFEMemCard_mInstance and clears it; cFEMemCard_onInputBegin sets the word when left is
 * pressed. cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsPrevDevicePressed (+0x80).
 *
 * @return the previous-device word before it was cleared
 */
extern "C" int cFEMemCard_isPrevMCPressed(void) {
    int temp_2;

    temp_2 = cFEMemCard_mInstance->mDoPrevMemCard;
    cFEMemCard_mInstance->mDoPrevMemCard = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isFormatPressed);
#ifdef SKIP_ASM

/**
 * Reports whether Format was confirmed on the current memory-card screen: returns the word at +0x1E8 of
 * cFEMemCard_mInstance.
 *
 * cFEMemCard_initFEMemCard installs it as the memory-card manager's callbackIsFormatPressed (+0x88).
 *
 * @return the format-pressed word
 */
extern "C" int cFEMemCard_isFormatPressed(void) {
    return (cFEMemCard_mInstance->mIsFormatPressed);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_callbackIsRetryPressed);
#ifdef SKIP_ASM

/**
 * Reports whether Retry was pressed on the current memory-card screen, and consumes it.
 *
 * Returns the word at +0x1EC of cFEMemCard_mInstance and clears it. cFEMemCard_initFEMemCard installs it as the memory-card
 * manager's callbackIsRetryPressed (+0x84).
 *
 * @return the retry-pressed word before it was cleared
 */
extern "C" int cFEMemCard_callbackIsRetryPressed(void) {
    int temp_2;

    temp_2 = cFEMemCard_mInstance->mIsRetryPressed;
    cFEMemCard_mInstance->mIsRetryPressed = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_isInstance);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cFEMemCard_mInstance_DDC8 __asm__("cFEMemCard_mInstance");

/**
 * Tells whether a memory-card screen exists: returns cFEMemCard_mInstance != 0.
 *
 * @return true when cFEMemCard_mInstance is set, otherwise false
 */
extern "C" bool cFEMemCard_isInstance(void) {
    return cFEMemCard_mInstance_DDC8 != 0;
}
#endif

/**
 * Does nothing: the memory-card screen base's onCreateScreen (slot +0x24 of its vtable cFEMemCard_vtbl).
 */
extern "C" void cFEMemCard_onCreateScreen(void) {
}

/**
 * Does nothing: the memory-card screens' onMenuGainFocus (slot +0x6C).
 *
 * It sits in cFEMemCard_vtbl (the cFEMemCard_cFEMemCard2 vtable) and is inherited unchanged by cFEStateMemCard,
 * cFEStateProfileLoad, cFEStateOptionsDeviceSelect, cOVState_AUTOSAVE, cOVState_PROFILE and two more vtables
 * (cOVState_SAVEOPTIONS_vtbl, cOVState_SAVEREPLAY_vtbl).
 */
extern "C" void cFEMemCard_onMenuGainFocus(void) {
}

/**
 * Does nothing: the memory-card screen base's onWidgetEvent (slot +0x9C of cFEMemCard_vtbl).
 */
extern "C" void cFEMemCard_onWidgetEvent(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0xD4 of cFEMemCard_vtbl, which cOVState_AUTOSAVE fills with
 * cOVState_AUTOSAVE_displayStatus.
 */
extern "C" void cFEMemCard_displayStatus(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x10C of cFEMemCard_vtbl, which cFEStateProfileLoad overrides with
 * cFEStateProfileLoad_updateDone and cFEStateOptionsDeviceSelect with cFEStateOptionsDeviceSelect_updateDone.
 */
extern "C" void cFEMemCard_updateDone(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x12C of cFEMemCard_vtbl.
 *
 * cFEStateProfileLoad (cFEStateProfileLoad_hideActionButtons), cFEStateOptionsDeviceSelect (cFEStateOptionsDeviceSelect_hideActionButtons), cOVState_PROFILE and
 * cFEStateMemCard override it with functions taking one flag.
 */
extern "C" void cFEMemCard_hideActionButtons(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x134 of cFEMemCard_vtbl (kept by cFEStateMemCard and
 * cOVState_AUTOSAVE).
 *
 * cFEStateProfileLoad (cFEStateProfileLoad_hideDeleteButton) and cFEStateOptionsDeviceSelect (cFEStateOptionsDeviceSelect_hideDeleteButton) override it with functions taking
 * one flag.
 */
extern "C" void cFEMemCard_hideDeleteButton(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x13C of cFEMemCard_vtbl (kept by cFEStateMemCard and
 * cOVState_AUTOSAVE).
 *
 * cFEStateProfileLoad (cFEStateProfileLoad_hideMainButton) and cFEStateOptionsDeviceSelect (cFEStateOptionsDeviceSelect_hideMainButton) override it with functions taking
 * a load flag and a save flag.
 */
extern "C" void cFEMemCard_hideMainButton(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFEMemCard_cFEMemCard1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_DE18(void *, int) __asm__("cUIState_cUIState");
extern int cFEMemCard_vtbl[];
extern int cOVState_vtbl[];
extern unsigned short str_sp[];

/**
 * Constructs a memory-card screen base with no engine (the default constructor).
 *
 * Runs cUIState's constructor (cUIState_cUIState) with a null engine, stores the cOVState vtable cOVState_vtbl and then
 * cFEMemCard_vtbl at +0x8, clears +0x48, +0x4C, +0x54 and +0x98 and copies the empty string str_sp into +0x58. Unlike
 * the constructor that takes an engine and a type, it does not run cFEMemCard_initFEMemCard, so cFEMemCard_mInstance is left
 * alone.
 *
 * @return @p self
 */
void * cFEMemCard::cFEMemCard1()
{
    cUIState_cUIState_DE18(this, 0);
    mOVID = 0;
    mDoneTransition = 0;
    VPTR_OF(cListNode, this) = (void**)cOVState_vtbl;
    mAllowPauseMenu = 0;
    mRemovePDA = 0;
    unsigned short emptyChar = str_sp[0];
    VPTR_OF(cListNode, this) = (void**)cFEMemCard_vtbl;
    *(unsigned short*)mszChatBuff = emptyChar;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupConfirm_cFEPopupConfirm1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cScreenPopup_cScreenPopup_DE78(void *, int, int) __asm__("cScreenPopup_cScreenPopup");
extern int cFEPopupConfirm_vtbl[];

/**
 * Constructs a confirmation popup with no engine and no parent (the default constructor).
 *
 * Runs cScreenPopup_cScreenPopup with null engine and parent, then stores cFEPopupConfirm_vtbl at +0x8.
 *
 * @return @p self
 */
void * cFEPopupConfirm::cFEPopupConfirm1()
{
    cScreenPopup_cScreenPopup_DE78(this, 0, 0);
    VPTR_OF(cListNode, this) = (void**)cFEPopupConfirm_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupConfirm__cFEPopupConfirm);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cScreenPopup__cScreenPopup_DEB8(void*) __asm__("cScreenPopup__cScreenPopup");
extern void* cFEPopupConfirm_vtbl_DEB8[] __asm__("cFEPopupConfirm_vtbl");

/**
 * Destroys a confirm popup: restores the cFEPopupConfirm vtable and runs the cScreenPopup destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for
 * cScreenPopup__cScreenPopup.
 */
extern "C" void cFEPopupConfirm__cFEPopupConfirm(cFEPopupConfirm* self) {
    VPTR_OF(cListNode, self) = cFEPopupConfirm_vtbl_DEB8;
    cScreenPopup__cScreenPopup_DEB8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupConfirm_setUsePreFE);
#ifdef SKIP_ASM
extern int cFEPopupConfirm_mUsePreFE;

/**
 * Sets the static cFEPopupConfirm_mUsePreFE flag.
 *
 * @param usePreFe  the new flag value
 */
extern "C" void cFEPopupConfirm_setUsePreFE(int usePreFe) {
    cFEPopupConfirm_mUsePreFE = usePreFe;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptions_cFEStateOptions);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_DEE8(void*, int) __asm__("cUIState_cUIState");
extern void* cFEStateOptions_vtbl[];

/**
 * Constructs the options menu state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine,
 * then stores cFEStateOptions_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void* cFEStateOptions_cFEStateOptions(cFEStateOptions* self) {
    cUIState_cUIState_DEE8(self, 0);
    VPTR_OF(cListNode, self) = cFEStateOptions_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptions__cFEStateOptions);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DDF20(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the options front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateOptions__cFEStateOptions(cFEStateOptions* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DDF20(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsGame_cFEStateOptionsGame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_DF48(void*, int) __asm__("cUIState_cUIState");
extern void* cFEStateOptionsGame_vtbl[];

/**
 * Constructs the game options state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine,
 * then stores cFEStateOptionsGame_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void* cFEStateOptionsGame_cFEStateOptionsGame(cFEStateOptionsGame* self) {
    cUIState_cUIState_DF48(self, 0);
    VPTR_OF(cListNode, self) = cFEStateOptionsGame_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsGame__cFEStateOptionsGame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DDF80(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the game options front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsGame__cFEStateOptionsGame(cFEStateOptionsGame* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DDF80(self);
}
#endif

/**
 * Does nothing: the game options state's onLoseFocus (slot +0x5C of cFEStateOptionsGame_vtbl).
 */
extern "C" void cFEStateOptionsGame_onLoseFocus(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsSound_cFEStateOptionsSound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_DFB0(void*, int) __asm__("cUIState_cUIState");
extern void* cFEStateOptionsSound_vtbl[];

/**
 * Constructs the sound options state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine,
 * then stores cFEStateOptionsSound_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void* cFEStateOptionsSound_cFEStateOptionsSound(cFEStateOptionsSound* self) {
    cUIState_cUIState_DFB0(self, 0);
    VPTR_OF(cListNode, self) = cFEStateOptionsSound_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsSound__cFEStateOptionsSound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DDFE8(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the sound options front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsSound__cFEStateOptionsSound(cFEStateOptionsSound* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DDFE8(self);
}
#endif

/**
 * Does nothing: the sound options state's onLoseFocus (slot +0x5C of cFEStateOptionsSound_vtbl).
 */
extern "C" void cFEStateOptionsSound_onLoseFocus(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsHUD_cFEStateOptionsHUD);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_E018(void*, int) __asm__("cUIState_cUIState");
extern void* cFEStateOptionsHUD_vtbl[];

/**
 * Constructs the HUD options state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine, then
 * stores its vtable cFEStateOptionsHUD_vtbl at +0x8.
 *
 * cFEStateOptionsHUD_vtbl is the vtable func_0018C0E8 stores in the state fereal.cpp allocates as "cFEStateOptionsHUD".
 *
 * @return @p self
 */
extern "C" void* cFEStateOptionsHUD_cFEStateOptionsHUD(cFEStateOptionsHUD* self) {
    cUIState_cUIState_E018(self, 0);
    VPTR_OF(cListNode, self) = cFEStateOptionsHUD_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsHUD__cFEStateOptionsHUD);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DE050(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the HUD options state (vtable cFEStateOptionsHUD_vtbl): restores cFEState's vtable cFEState_vtbl at +0x8 and runs cUIState's
 * destructor (cUIState__cUIState).
 *
 * The g++ delete flags pass through in the second argument register.
 */
extern "C" void cFEStateOptionsHUD__cFEStateOptionsHUD(cFEStateOptionsHUD* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DE050(self);
}
#endif

/**
 * Does nothing: the HUD options state's onLoseFocus (slot +0x5C of cFEStateOptionsHUD_vtbl).
 */
extern "C" void cFEStateOptionsHUD_onLoseFocus(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsController_cFEStateOptionsController);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E080(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateOptionsController_vtbl[];

/**
 * Constructs the controller options state with no engine.
 *
 * Runs cUIState's constructor (cUIState_cUIState) with a null engine, stores cFEStateOptionsController_vtbl at +0x8 and
 * clears the byte at +0x50 (On Tour's mVibrateOn).
 *
 * @return @p self
 */
extern "C" void *cFEStateOptionsController_cFEStateOptionsController(cFEStateOptionsController* self) {
    cUIState_cUIState_E080(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateOptionsController_vtbl;
    self->mVibrateOn = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsController__cFEStateOptionsController);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DE0C0(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the controller options front-end state: restores cFEState's vtable (cFEState_vtbl) and runs
 * cUIState's destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsController__cFEStateOptionsController(cFEStateOptionsController* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DE0C0(self);
}
#endif

/**
 * Does nothing: the controller options state's onLoseFocus (slot +0x5C of cFEStateOptionsController_vtbl).
 */
extern "C" void cFEStateOptionsController_onLoseFocus(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E0F0(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateOptionsSaveLoad_vtbl[];

/**
 * Constructs the save/load options state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null
 * engine, then stores cFEStateOptionsSaveLoad_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad(cFEStateOptionsSaveLoad* self) {
    cUIState_cUIState_E0F0(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateOptionsSaveLoad_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsSaveLoad__cFEStateOptionsSaveLoad);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_DE128(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the save/load options front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsSaveLoad__cFEStateOptionsSaveLoad(cFEStateOptionsSaveLoad* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_DE128(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsDeviceSelect__cFEStateOptionsDeviceSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEMemCard__cFEMemCard_E150(void*) __asm__("cFEMemCard__cFEMemCard");
extern void* cFEStateOptionsDeviceSelect_vtbl[];

/**
 * Destroys the options device-select state: stores its vtable at +0x8, then runs the base destructor.
 *
 * The base, cFEMemCard__cFEMemCard(), clears the global cFEMemCard_mInstance and runs the cUIState destructor (cUIState__cUIState). The
 * g++ delete flags in the second argument register pass through untouched.
 *
 * @return no meaningful value (cFEMemCard__cFEMemCard returns nothing)
 */
extern "C" void* cFEStateOptionsDeviceSelect__cFEStateOptionsDeviceSelect(cFEStateOptionsDeviceSelect* self) {
    VPTR_OF(cListNode, self) = cFEStateOptionsDeviceSelect_vtbl;
    return cFEMemCard__cFEMemCard_E150(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEMemCard_cFEMemCard_E178(void *, int, int) __asm__("cFEMemCard_cFEMemCard2");
extern int cFEStateOptionsDeviceSelect_vtbl_E178[] __asm__("cFEStateOptionsDeviceSelect_vtbl");

/**
 * Constructs the memory-card device select state with no engine: runs cFEMemCard_cFEMemCard2 with null engine and type
 * 0, then stores cFEStateOptionsDeviceSelect_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect(cFEStateOptionsDeviceSelect* self) {
    cFEMemCard_cFEMemCard_E178(self, 0, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateOptionsDeviceSelect_vtbl_E178;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateProfileSelect_cFEStateProfileSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E1B8(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateProfileSelect_vtbl[];

/**
 * Constructs the profile select state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine,
 * then stores cFEStateProfileSelect_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateProfileSelect_cFEStateProfileSelect(cFEStateProfileSelect* self) {
    cUIState_cUIState_E1B8(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateProfileSelect_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateProfileSelect__cFEStateProfileSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E1F0(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the profile select front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever cUIState__cUIState leaves)
 */
extern "C" void* cFEStateProfileSelect__cFEStateProfileSelect(cFEStateProfileSelect* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState_E1F0(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateProfileLoad__cFEStateProfileLoad);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEMemCard__cFEMemCard_E218(void*) __asm__("cFEMemCard__cFEMemCard");
extern void* cFEStateProfileLoad_vtbl[];

/**
 * Destroys the profile-load state: stores its vtable at +0x8, then runs the base destructor.
 *
 * The base, cFEMemCard__cFEMemCard(), clears the global cFEMemCard_mInstance and runs the cUIState destructor (cUIState__cUIState). The
 * g++ delete flags in the second argument register pass through untouched.
 *
 * @return no meaningful value (cFEMemCard__cFEMemCard returns nothing)
 */
extern "C" void* cFEStateProfileLoad__cFEStateProfileLoad(cFEStateProfileLoad* self) {
    VPTR_OF(cListNode, self) = cFEStateProfileLoad_vtbl;
    return cFEMemCard__cFEMemCard_E218(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE240);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEMemCard_cFEMemCard_E240(void *, int, int) __asm__("cFEMemCard_cFEMemCard2");
extern int cFEStateProfileLoad_vtbl_E240[] __asm__("cFEStateProfileLoad_vtbl");

/**
 * Constructs the profile load/save state with no engine: runs cFEMemCard_cFEMemCard2 with null engine and type 0, then
 * stores cFEStateProfileLoad_vtbl at +0x8.
 *
 * Unlike cFEStateProfileLoad_cFEStateProfileLoad it sets none of the state's own fields.
 *
 * @return @p self
 */
extern "C" void *func_001DE240(cFEStateProfileLoad* self) {
    cFEMemCard_cFEMemCard_E240(self, 0, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateProfileLoad_vtbl_E240;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateProfileLoad_setQuitPath);
#ifdef SKIP_ASM
/**
 * Sets what a profile load/save state does when it finishes, the last word of the object (+0x27C).
 *
 * cFEStateProfileLoad_updateDone (the state's slot +0x10C) reads it: 0 pushes the state the parent's factory returns, 1 starts a
 * pre-game load (cPreGameLoadScreen), 2 returns to the title screen. The constructor clears it; cFEStateLodge's callers
 * set 1 or 2 directly.
 *
 * @param action  the action: 0, 1 or 2
 */
void cFEStateProfileLoad::setQuitPath(int action)
{
    mTypeOfQuit = action;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_cKeyboardPopup1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState1_E288(void *, int, int) __asm__("cUIState_cUIState1");
extern int cKeyboardPopup_vtbl[];
extern char* cBXString_EmptyString;

/**
 * Constructs an on-screen keyboard popup with no engine and no parent (the default constructor).
 *
 * Runs cUIState's two-argument constructor (cUIState_cUIState1) with null engine and parent, stores cKeyboardPopup_vtbl,
 * sets miMaxCharacters (+0x60) to 63 and mszStatic (+0x424) to the empty cBXString.
 *
 * @return @p self
 */
cKeyboardPopup * cKeyboardPopup::cKeyboardPopup1()
{
    cUIState_cUIState1_E288((this), 0, 0);
    VPTR_OF(cListNode, this) = (void**)cKeyboardPopup_vtbl;
    miMaxCharacters = 0x3F;
    mszStatic.Str = cBXString_EmptyString;
    return (this);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_onLoseTransition);
#ifdef SKIP_ASM
/**
 * Handles the keyboard popup's lose transition by reporting it finished immediately.
 *
 * @return always 1
 */
extern "C" int cKeyboardPopup_onLoseTransition(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getInputText);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's edited text, mszEditString (+0x74).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return @p keyboard + 0x74
 */
extern "C" int cKeyboardPopup_getInputText(int keyboard) {
    return keyboard + 116;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getInputTextLength);
#ifdef SKIP_ASM
extern "C" void strlen(int);

/**
 * Measures the keyboard popup's edited text: tail-calls strlen on mszEditString (+0x74), leaving its result as the
 * return value.
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 */
extern "C" void cKeyboardPopup_getInputTextLength(int keyboard) {
    strlen(keyboard + 116);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getCurrentKey);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's selected key, miCurrentKey (+0x140; 0..83).
 *
 * @return miCurrentKey
 */
int cKeyboardPopup::getCurrentKey()
{
    return (miCurrentKey);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setCurrentKey);
#ifdef SKIP_ASM
/**
 * Sets the keyboard popup's selected key, miCurrentKey (+0x140).
 *
 * @param currentKey  the key index (0..83)
 */
void cKeyboardPopup::setCurrentKey(int currentKey)
{
    miCurrentKey = currentKey;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setKeyboardType);
#ifdef SKIP_ASM
/**
 * Sets the keyboard popup's layout, meType (+0x43C): 0 regular, 1 compact, 2 expanded.
 *
 * @param keyboardType  the layout
 */
void cKeyboardPopup::setKeyboardType(int keyboardType)
{
    meType = keyboardType;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getKeyboardType);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's layout, meType (+0x43C).
 *
 * @return meType
 */
int cKeyboardPopup::getKeyboardType()
{
    return (meType);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getPopOnEnd);
#ifdef SKIP_ASM
/**
 * Returns whether accepting the text pops the keyboard popup, mbPopOnEnd (+0x420).
 *
 * @return mbPopOnEnd
 */
int cKeyboardPopup::getPopOnEnd()
{
    return (mbPopOnEnd);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setPopOnEnd);
#ifdef SKIP_ASM
/**
 * Sets whether accepting the text pops the keyboard popup, mbPopOnEnd (+0x420).
 *
 * @param popOnEnd  non-zero to pop on accept
 */
void cKeyboardPopup::setPopOnEnd(int popOnEnd)
{
    mbPopOnEnd = popOnEnd;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setPassword);
#ifdef SKIP_ASM
/**
 * Sets whether the keyboard popup masks its text, mbPassword (+0x70).
 *
 * @param password  non-zero to show '*' characters
 */
void cKeyboardPopup::setPassword(int password)
{
    mbPassword = password;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setClearOnFirstCharacter);
#ifdef SKIP_ASM
/**
 * Sets whether the first key press clears the text, mbClearOnFirst (+0x54).
 *
 * @param clearOnFirst  non-zero to clear on the first key
 */
void cKeyboardPopup::setClearOnFirstCharacter(int clearOnFirst)
{
    mbClearOnFirst = clearOnFirst;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setDisableSpaceForFirst);
#ifdef SKIP_ASM
/**
 * Sets whether the space key stays disabled while the text is empty, mbDisableForFirst (+0x58).
 *
 * @param disableForFirst  non-zero to disable space for the first character
 */
void cKeyboardPopup::setDisableSpaceForFirst(int disableForFirst)
{
    mbDisableForFirst = disableForFirst;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setIncludeSpace);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE350(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables the space key (key 75) in both key sets: cKeyboardPopup_setIncludeKey(keyboard, 75, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeSpace  non-zero to enable the key
 */
extern "C" void cKeyboardPopup_setIncludeSpace(int keyboard, int includeSpace) {
    cKeyboardPopup_setIncludeKey_DE350(keyboard, 75, includeSpace, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setIncludeTab);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE378(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables key 28 (the first key of the fourth row) in both key sets: cKeyboardPopup_setIncludeKey(keyboard,
 * 28, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeKey  non-zero to enable the key
 */
extern "C" void cKeyboardPopup_setIncludeTab(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE378(keyboard, 0x1C, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setIncludeUp);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE3A0(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables key 69 (the last key of the fifth row) in both key sets: cKeyboardPopup_setIncludeKey(keyboard,
 * 69, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeKey  non-zero to enable the key
 */
extern "C" void cKeyboardPopup_setIncludeUp(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE3A0(keyboard, 69, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setIncludeDown);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE3C8(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables key 79 (in the bottom row) in both key sets: cKeyboardPopup_setIncludeKey(keyboard, 79, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeKey  non-zero to enable the key
 */
extern "C" void cKeyboardPopup_setIncludeDown(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE3C8(keyboard, 79, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setIncludeBackslash);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE3F0(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables key 41 (the last key of the third row) in both key sets: cKeyboardPopup_setIncludeKey(keyboard,
 * 41, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeKey  non-zero to enable the key
 */
extern "C" void cKeyboardPopup_setIncludeBackslash(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE3F0(keyboard, 41, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setIncludeQuestionMark);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE418(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables key 66 in both key sets: cKeyboardPopup_setIncludeKey(keyboard, 66, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeKey  non-zero to enable the key
 */
extern "C" void cKeyboardPopup_setIncludeQuestionMark(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE418(keyboard, 66, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getMinCharacters);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's minimum text length, miMinCharacters (+0x64).
 *
 * @return miMinCharacters
 */
int cKeyboardPopup::getMinCharacters()
{
    return (miMinCharacters);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getMaxCharacters);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's maximum text length, miMaxCharacters (+0x60).
 *
 * @return miMaxCharacters
 */
int cKeyboardPopup::getMaxCharacters()
{
    return (miMaxCharacters);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setReferEventsToParent);
#ifdef SKIP_ASM
/**
 * Sets whether the keyboard popup forwards its events to the parent state, mbReferEventsToParent (+0x438).
 *
 * @param referEvents  non-zero to forward events (cKeyboardPopup_handleEvent)
 */
void cKeyboardPopup::setReferEventsToParent(int referEvents)
{
    mbReferEventsToParent = referEvents;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_getReferEventsToParent);
#ifdef SKIP_ASM
/**
 * Returns whether the keyboard popup forwards its events to the parent state, mbReferEventsToParent (+0x438).
 *
 * @return mbReferEventsToParent
 */
int cKeyboardPopup::getReferEventsToParent()
{
    return (mbReferEventsToParent);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_setCallParentsUpdate);
#ifdef SKIP_ASM
/**
 * Sets whether the keyboard popup runs its parent's onUpdate each frame, mbCallParentsUpdate (+0x440).
 *
 * @param callParentUpdate  non-zero to update the parent too (cKeyboardPopup_onUpdate)
 */
void cKeyboardPopup::setCallParentsUpdate(int callParentUpdate)
{
    mbCallParentsUpdate = callParentUpdate;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_isAcceptText);
#ifdef SKIP_ASM
/**
 * Tells whether the player accepted the keyboard popup's text: returns mbAcceptText (+0x5C), which the accept key sets.
 *
 * @return mbAcceptText
 */
int cKeyboardPopup::isAcceptText()
{
    return (mbAcceptText);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_cursorRight);
#ifdef SKIP_ASM
/**
 * Moves the keyboard popup's text cursor one place right, while it stays below miMaxCharacters - 1.
 *
 * Increments miCursorPos (+0x134) only when it is below miMaxCharacters (+0x60) minus one.
 *
 */
void cKeyboardPopup::cursorRight()
{
    int temp_3;

    temp_3 = miCursorPos;
    if (temp_3 < (miMaxCharacters - 1)) {
        miCursorPos = (int) (temp_3 + 1);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cKeyboardPopup_cursorLeft);
#ifdef SKIP_ASM
/**
 * Moves the keyboard popup's text cursor one place left, unless it is already at 0.
 *
 * Decrements miCursorPos (+0x134) when it is above 0.
 *
 */
void cKeyboardPopup::cursorLeft()
{
    int temp_2;

    temp_2 = miCursorPos;
    if (temp_2 > 0) {
        miCursorPos = (int) (temp_2 - 1);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCredits_cFEStateCredits);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E4B0(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateCredits_vtbl[];

/**
 * Constructs the credits state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine, then
 * stores cFEStateCredits_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateCredits_cFEStateCredits(cFEStateCredits* self) {
    cUIState_cUIState_E4B0(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateCredits_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateEventSelect_cFEStateEventSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E4E8(void *, int) __asm__("cUIState_cUIState");
extern "C" void cUITemplate_MAP_init_E4E8(void *) __asm__("cUITemplate_MAP_init");
extern int cFEStateEventSelect_vtbl[];

/**
 * Constructs the event select state with no engine.
 *
 * Runs cUIState's constructor (cUIState_cUIState) with a null engine, stores cFEStateEventSelect_vtbl at +0x8 and
 * constructs the member at +0x48 with cUITemplate_MAP_init (On Tour keeps its cUITemplate_MAP mMap there).
 *
 * @return @p self
 */
extern "C" void *cFEStateEventSelect_cFEStateEventSelect(cFEStateEventSelect* self) {
    cUIState_cUIState_E4E8(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateEventSelect_vtbl;
    cUITemplate_MAP_init_E4E8((char*)&self->mMap);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateEventSelect__cFEStateEventSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E528(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the event select front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever cUIState__cUIState leaves)
 */
extern "C" void* cFEStateEventSelect__cFEStateEventSelect(cFEStateEventSelect* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState_E528(self);
}
#endif

/**
 * Does nothing: the event select state's onMenuGainFocus (slot +0x6C of cFEStateEventSelect_vtbl).
 */
extern "C" void cFEStateEventSelect_onMenuGainFocus(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE558);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E558(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateBackground_vtbl_E558[] __asm__("cFEStateBackground_vtbl");

/**
 * Constructs the front-end background state with no engine (the default constructor): runs cUIState's constructor
 * (cUIState_cUIState) with a null engine, then stores cFEStateBackground_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE558(cFEStateBackground* self) {
    cUIState_cUIState_E558(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateBackground_vtbl_E558;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateBackground_cFEStateBackground);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_E590(void*) __asm__("cUIState_cUIState");
extern void* cFEStateBackground_vtbl[];

/**
 * Constructs the front-end background state for a UI engine: runs cUIState's constructor (cUIState_cUIState) with the
 * engine, then stores cFEStateBackground_vtbl at +0x8.
 *
 * The engine argument passes through untouched in the second argument register.
 *
 * @return @p self
 */
extern "C" void* cFEStateBackground_cFEStateBackground(cFEStateBackground* self) {
    cUIState_cUIState_E590(self);
    VPTR_OF(cListNode, self) = cFEStateBackground_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateBackground__cFEStateBackground);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E5C8(void*) __asm__("cUIState__cUIState");
extern void* cFEStateBackground_vtbl[];

/**
 * Destroys the front-end background state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (cUIState__cUIState) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (cUIState__cUIState returns nothing)
 */
extern "C" void* cFEStateBackground__cFEStateBackground(cFEStateBackground* self) {
    VPTR_OF(cListNode, self) = cFEStateBackground_vtbl;
    return cUIState__cUIState_E5C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE5F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E5F0(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateTransition_vtbl_E5F0[] __asm__("cFEStateTransition_vtbl");

/**
 * Constructs the screen transition state with no engine (the default constructor): runs cUIState's constructor
 * (cUIState_cUIState) with a null engine, then stores cFEStateTransition_vtbl at +0x8.
 *
 * Unlike cFEStateTransition_cFEStateTransition it leaves the two screen pointers (+0x48, +0x4C) unset.
 *
 * @return @p self
 */
extern "C" void *func_001DE5F0(cFEStateTransition* self) {
    cUIState_cUIState_E5F0(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateTransition_vtbl_E5F0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateTransition_cFEStateTransition);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E628(void *) __asm__("cUIState_cUIState");
extern int cFEStateTransition_vtbl_E628[] __asm__("cFEStateTransition_vtbl");

/**
 * Constructs the screen transition state for a UI engine.
 *
 * Runs cUIState's constructor (cUIState_cUIState) with the engine (passed through in the second argument register), stores
 * cFEStateTransition_vtbl at +0x8 and clears the two screen pointers at +0x48 and +0x4C (On Tour's mpSnow and mpWhite).
 *
 * @return @p self
 */
extern "C" void *cFEStateTransition_cFEStateTransition(cFEStateTransition* self) {
    cUIState_cUIState_E628(self);
    VPTR_OF(cListNode, self) = (void**)cFEStateTransition_vtbl_E628;
    self->mpSnow = 0;
    self->mpWhite = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateTransition__cFEStateTransition);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E668(void*) __asm__("cUIState__cUIState");
extern void* cFEStateTransition_vtbl[];

/**
 * Destroys the front-end transition state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (cUIState__cUIState) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (cUIState__cUIState returns nothing)
 */
extern "C" void* cFEStateTransition__cFEStateTransition(cFEStateTransition* self) {
    VPTR_OF(cListNode, self) = cFEStateTransition_vtbl;
    return cUIState__cUIState_E668(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateTitle_cFEStateTitle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E690(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateTitle_vtbl[];

/**
 * Constructs the title screen state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine,
 * then stores cFEStateTitle_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateTitle_cFEStateTitle(cFEStateTitle* self) {
    cUIState_cUIState_E690(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateTitle_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateTitle__cFEStateTitle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E6C8(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the title front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever cUIState__cUIState leaves)
 */
extern "C" void* cFEStateTitle__cFEStateTitle(cFEStateTitle* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState_E6C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateMainMenu_cFEStateMainMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E6F0(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateMainMenu_vtbl[];

/**
 * Constructs the main menu state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine, then
 * stores cFEStateMainMenu_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateMainMenu_cFEStateMainMenu(cFEStateMainMenu* self) {
    cUIState_cUIState_E6F0(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateMainMenu_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateMainMenu__cFEStateMainMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E728(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the main menu front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever cUIState__cUIState leaves)
 */
extern "C" void* cFEStateMainMenu__cFEStateMainMenu(cFEStateMainMenu* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState_E728(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateBonusMaterial_cFEStateBonusMaterial);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E750(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateBonusMaterial_vtbl[];

/**
 * Constructs the bonus material state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine,
 * then stores cFEStateBonusMaterial_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateBonusMaterial_cFEStateBonusMaterial(cFEStateBonusMaterial* self) {
    cUIState_cUIState_E750(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateBonusMaterial_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateBonusMaterial__cFEStateBonusMaterial);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E788(void*) __asm__("cUIState__cUIState");
extern void* cFEState_vtbl[];

/**
 * Destroys the bonus material front-end state: restores cFEState's vtable (cFEState_vtbl) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for cUIState__cUIState
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever cUIState__cUIState leaves)
 */
extern "C" void* cFEStateBonusMaterial__cFEStateBonusMaterial(cFEStateBonusMaterial* self) {
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState_E788(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateNet_cFEStateNet1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState_cUIState1_E7B0(void* self, int a1, int a2) __asm__("cUIState_cUIState1");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern void* cFEStateNet_vtbl[];
extern char str_sp_E7B0[] __asm__("str_sp");
extern char str_StlBlock[];

struct sListNode_E7B0 {
    sListNode_E7B0* next;   // 0x0
    sListNode_E7B0* prev;   // 0x4
    int data;               // 0x8
};

struct sList_E7B0 {
    int count;              // 0x0
    sListNode_E7B0* head;   // 0x4
};

/**
 * Constructs an online front-end state with no engine and no parent (the default constructor).
 *
 * Runs cUIState's two-argument constructor (cUIState_cUIState1) with null engine and parent, clears mHasFocus,
 * mBlockInput, mpKeyboard, mpKeyboardText, mDisconnectPopupOpen, mLeavingOnline and mpPopupTextBuf, sets mPopupResult
 * to 2, stores the cFEStateNet vtable cFEStateNet_vtbl, builds mszTargetUser from the empty string str_sp
 * (cBXString_cBXString2) and allocates the pending-message list's sentinel node (cMemMan_alloc(0xC, "StlBlock")),
 * linking it to itself.
 *
 * @return @p self
 */
void* cFEStateNet::cFEStateNet1()
{
    // An unused 16-byte local is needed to reproduce the 0x40-byte frame.
    int framePad[4];
    cUIState_cUIState1_E7B0(this, 0, 0);
    sList_E7B0* msgList = (sList_E7B0*)(&mMsgList);
    mHasFocus = 0;
    mBlockInput = 0;
    mpKeyboard = 0;
    mpKeyboardText = 0;
    mDisconnectPopupOpen = 0;
    mLeavingOnline = 0;
    mpPopupTextBuf = 0;
    mPopupResult = 2;
    VPTR_OF(cListNode, this) = cFEStateNet_vtbl;
    cBXString_cBXString2(&mszTargetUser, str_sp_E7B0);
    mMsgList.mpNode = (void*)((sListNode_E7B0*)cMemMan_alloc(0xC, str_StlBlock, 0x20000000, 0));
    msgList->head->next = (sListNode_E7B0*)mMsgList.mpNode;
    msgList->head->prev = (sListNode_E7B0*)mMsgList.mpNode;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateNet_onInputBegin);
#ifdef SKIP_ASM
/**
 * Returns an online state's mBlockInput as its onInputBegin result (slot +0x44 of the online states' vtables).
 *
 * func_001A9668 and func_001A9710 set the word to 1 before pushing the login state.
 *
 * @param feStateNet  the online state (cFEStateNet)
 * @return mBlockInput
 */
extern "C" int cFEStateNet_onInputBegin(cFEStateNet* feStateNet) {
    return (feStateNet->mBlockInput);
}
#endif

/**
 * Does nothing: slot +0xCC, the first of cFEStateNet's own virtuals, in the cFEStateNet vtable cFEStateNet_vtbl and most of
 * the online states' vtables.
 */
extern "C" void func_001DE868(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE870);
#ifdef SKIP_ASM
/**
 * Returns 1: slot +0xEC of the cFEStateNet vtable cFEStateNet_vtbl and of most online states' vtables.
 *
 * cFEBlankPopup and cFEStateNetAccountMem fill the slot with func_001DE908, which returns 0.
 *
 * @return 1
 */
extern "C" int func_001DE870(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateNet_getFEPath);
#ifdef SKIP_ASM
extern int D_004A1A70;

/**
 * Returns the front end's path, the global D_004A1A70.
 *
 * @return D_004A1A70
 */
extern "C" int cFEStateNet_getFEPath(void) {
    return D_004A1A70;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateNet_fePathIsOnline);
#ifdef SKIP_ASM
extern int D_004A1A70;

/**
 * Tells whether the front end is on the online path: returns D_004A1A70 == cFEStateNet::kFEPathOnline.
 *
 * @return true when D_004A1A70 is cFEStateNet::kFEPathOnline, otherwise false
 */
extern "C" bool cFEStateNet_fePathIsOnline(void) {
    return D_004A1A70 == cFEStateNet::kFEPathOnline;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE890);
#ifdef SKIP_ASM
/**
 * Sets an online state's mLeavingOnline to 1.
 *
 * func_001BF5A8 and func_001C0138 set the same word; no read of it is known.
 *
 * @param feStateNet  the online state (cFEStateNet)
 */
extern "C" void func_001DE890(cFEStateNet* feStateNet) {
    feStateNet->mLeavingOnline = 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE8A0);
#ifdef SKIP_ASM
/**
 * Sets an online state's mUserPopupOpen; func_001ABB30 opens the status popup only when it and mpPopup are set.
 *
 * @param feStateNet  the online state (cFEStateNet)
 * @param value  the new value
 */
extern "C" void func_001DE8A0(cFEStateNet* feStateNet, int value) {
    feStateNet->mUserPopupOpen = value;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateNet_isProcessingEvents);
#ifdef SKIP_ASM
/**
 * Tells whether an online state handles net events: its screen (+0x40) exists and mHasFocus (+0x48) is set.
 *
 * Slot +0xF4 of the cFEStateNet vtable cFEStateNet_vtbl and of most online states' vtables; cFEStateNet_onGainFocus calls it,
 * and sets mHasFocus to 1.
 *
 * @return 1 when both are set, otherwise 0
 */
int cFEStateNet::isProcessingEvents()
{
    int var_3;

    var_3 = 0;
    if (mpScreen != 0) {
        var_3 = mHasFocus != 0;
    }
    return var_3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEBlankPopup_cFEBlankPopup1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEStateNet_cFEStateNet_E8C8(void *, int, int) __asm__("cFEStateNet_cFEStateNet");
extern int cFEBlankPopup_vtbl_E8C8[] __asm__("cFEBlankPopup_vtbl");

/**
 * Constructs a blank online popup with no engine and no parent (the default constructor): runs cFEStateNet_cFEStateNet
 * with null engine and parent, then stores cFEBlankPopup_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEBlankPopup_cFEBlankPopup1(cFEStateNet* self) {
    cFEStateNet_cFEStateNet_E8C8(self, 0, 0);
    VPTR_OF(cListNode, self) = (void**)cFEBlankPopup_vtbl_E8C8;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE908);
#ifdef SKIP_ASM
/**
 * Returns 0: slot +0xEC of cFEBlankPopup_vtbl and cFEStateNetAccountMem_vtbl, where the other online states have
 * func_001DE870 (returns 1).
 *
 * @return 0
 */
extern "C" int func_001DE908(void) {
    return 0;
}
#endif

/**
 * Does nothing: slot +0x104 of cFEBlankPopup_vtbl and cFEStateNetAccountMem_vtbl, where the cFEStateNet vtable has
 * func_001A8830.
 */
extern "C" void func_001DE910(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFEBlankPopup__cFEBlankPopup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEStateNet__cFEStateNet_E918(void*) __asm__("cFEStateNet__cFEStateNet");
extern void* cFEBlankPopup_vtbl[];

/**
 * Destroys a blank popup state: installs cFEBlankPopup_vtbl (+0x8) and runs cFEStateNet__cFEStateNet, which receives
 * the caller's destructor flags in its second register.
 *
 * @return cFEStateNet__cFEStateNet's result
 */
extern "C" void* cFEBlankPopup__cFEBlankPopup(cFEStateNet* self) {
    VPTR_OF(cListNode, self) = cFEBlankPopup_vtbl;
    return cFEStateNet__cFEStateNet_E918(self);
}
#endif

/**
 * Does nothing: the memory-card state's onWidgetEvent (slot +0x9C of cFEStateMemCard_vtbl).
 */
extern "C" void cFEStateMemCard_onWidgetEvent(void) {
}

/**
 * Does nothing: slot +0x12C of cFEStateMemCard_vtbl, the slot cFEMemCard_hideActionButtons fills in the memory-card base vtable
 * cFEMemCard_vtbl.
 */
extern "C" void cFEStateMemCard_hideActionButtons(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateMemCard_cFEStateMemCard);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEMemCard_cFEMemCard_E950(void *, int, int) __asm__("cFEMemCard_cFEMemCard2");
extern int cFEStateMemCard_vtbl[];

/**
 * Constructs the memory-card state with no engine: runs cFEMemCard_cFEMemCard2 with null engine and type 0, then stores
 * cFEStateMemCard_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateMemCard_cFEStateMemCard(cFEStateMemCard* self) {
    cFEMemCard_cFEMemCard_E950(self, 0, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateMemCard_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateLegal_cFEStateLegal);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUIState_cUIState_E990(void *, int) __asm__("cUIState_cUIState");
extern int cFEStateLegal_vtbl_E990[] __asm__("cFEStateLegal_vtbl");

/**
 * Constructs the legal screen state with no engine: runs cUIState's constructor (cUIState_cUIState) with a null engine,
 * then stores cFEStateLegal_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *cFEStateLegal_cFEStateLegal(cFEStateLegal* self) {
    cUIState_cUIState_E990(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateLegal_vtbl_E990;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateLegal__cFEStateLegal);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E9C8(void*) __asm__("cUIState__cUIState");
extern void* cFEStateLegal_vtbl[];

/**
 * Destroys the legal-screen state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (cUIState__cUIState) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (cUIState__cUIState returns nothing)
 */
extern "C" void* cFEStateLegal__cFEStateLegal(cFEStateLegal* self) {
    VPTR_OF(cListNode, self) = cFEStateLegal_vtbl;
    return cUIState__cUIState_E9C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateLegal_onGainTransition);
#ifdef SKIP_ASM
/**
 * Ends the legal screen's gain transition at once.
 *
 * Declared without parameters. The state stack update cUIStateStack_updateLoop() ends a state's gain transition once this
 * returns nonzero.
 *
 * @return always 1 (transition finished)
 */
extern "C" int cFEStateLegal_onGainTransition(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupOptionReset__cFEPopupOptionReset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_E9F8(void*) __asm__("cUIState__cUIState");
extern void* cFEPopupOptionReset_vtbl[];

/**
 * Destroys the options-reset popup (vtable cFEPopupOptionReset_vtbl): stores cFEPopupOptionReset_vtbl at +0x8 and runs cUIState's destructor
 * (cUIState__cUIState).
 *
 * cFEPopupOptionReset_vtbl is the vtable festateoptions.cpp stores in the popup it allocates as "cFEPopupOptionReset". The g++ delete
 * flags pass through in the second argument register.
 *
 * @return the result of cUIState__cUIState
 */
extern "C" void* cFEPopupOptionReset__cFEPopupOptionReset(cFEPopupOptionReset* self) {
    VPTR_OF(cListNode, self) = cFEPopupOptionReset_vtbl;
    return cUIState__cUIState_E9F8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupOptionReset_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int GetHashValue32_EA20(char *) __asm__("GetHashValue32__FPc");
extern "C" cUIScreen* cUIEngine_addScreenByHashName_EA20(cUIEngine* engine, void* state, int hashName, int flags)
    __asm__("cUIEngine_addScreenByHashName");
extern "C" void cUIScreen_playFrame_EA20(cUIScreen* screen, int frame, int flags) __asm__("cUIScreen_playFrame");
extern char str_111resetopts[];

/**
 * Opens the options-reset popup's screen: looks up the screen "111resetopts" and starts it from frame 0.
 *
 * Stores GetHashValue32("111resetopts") in mHashName (+0xC), adds the screen with cUIEngine_addScreenByHashName on the
 * engine (+0x10), keeps it at +0x40 and, when it exists, calls cUIScreen_playFrame(screen, 0, 0).
 */
extern "C" void cFEPopupOptionReset_onCreateScreen(cFEPopupOptionReset* self) {
    int hashName = GetHashValue32_EA20(str_111resetopts);
    self->mHashName = hashName;
    cUIScreen* screen = cUIEngine_addScreenByHashName_EA20(self->mpEngine, self, hashName, 0);
    self->mpScreen = screen;
    if (screen != 0) {
        cUIScreen_playFrame_EA20(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupOptionReset_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Filters the options-reset popup's menu: returns 0x100 for @p key 7, 8 or 9 and 0x101 for any other value.
 *
 * It fills the onWidgetMenu slot (+0xA4) of cFEPopupOptionReset_vtbl.
 *
 * @param menu  the menu (unused)
 * @param key  the menu event
 * @return 0x100 or 0x101
 */
extern "C" int cFEPopupOptionReset_onWidgetMenu(cFEPopupOptionReset* self, int menu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 7) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupOptionReset_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char*);
extern "C" void cUIStateStack_pop(cUIStateStack* stack, int count);
extern char str_yes[];
extern char str_no[];

/**
 * Handles a button press on the options-reset popup: "yes" accepts, "no" declines, and either closes the popup.
 *
 * On event cUIState::kONNEXT from a widget, compares the widget's hash (+0x38) with GetHashValue32("yes") and ("no");
 * sets the accept byte (+0x48) to 1 or 0 and pops the engine's state stack (mStateStack, +0x18) with
 * cUIStateStack_pop. Other events and other widgets are ignored.
 *
 * @param widget  the widget (may be null)
 * @param event  the widget event
 */
extern "C" void cFEPopupOptionReset_onWidgetEvent(cFEPopupOptionReset* self, cUIThing* widget, int event) {
    if (((char*)widget) != 0) {
        if (event == cUIState::kONNEXT) {
            int yesCheckHash = widget->mHashName;
            if (yesCheckHash == GetHashValue32(str_yes)) {
                self->mAccept = 1;
                cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
            } else {
                int noCheckHash = widget->mHashName;
                if (noCheckHash == GetHashValue32(str_no)) {
                    self->mAccept = 0;
                    cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupOptionReset_getData);
#ifdef SKIP_ASM
/**
 * Returns the options-reset popup's answer, the signed byte at +0x48 (1 yes, 0 no).
 *
 * @param resetPopup  the popup
 * @return the accept byte
 */
extern "C" signed char cFEPopupOptionReset_getData(cFEPopupOptionReset* resetPopup) {
    return resetPopup->mAccept;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupVideoCalibration__cFEPopupVideoCalibration);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_EB58(void*) __asm__("cUIState__cUIState");
extern void* cFEPopupVideoCalibration_vtbl[];

/**
 * Destroys the video calibration popup: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (cUIState__cUIState) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (cUIState__cUIState returns nothing)
 */
extern "C" void* cFEPopupVideoCalibration__cFEPopupVideoCalibration(cFEPopupVideoCalibration* self) {
    VPTR_OF(cListNode, self) = cFEPopupVideoCalibration_vtbl;
    return cUIState__cUIState_EB58(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupVideoCalibration_onGainTransition);
#ifdef SKIP_ASM
/**
 * Ends the video-calibration popup's gain transition at once.
 *
 * Declared without parameters. The state stack update cUIStateStack_updateLoop() ends a state's gain transition once this
 * returns nonzero.
 *
 * @return always 1 (transition finished)
 */
extern "C" int cFEPopupVideoCalibration_onGainTransition(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupVideoCalibration_onLoseTransition);
#ifdef SKIP_ASM
/**
 * Ends the video-calibration popup's lose transition at once.
 *
 * Declared without parameters. The state stack update cUIStateStack_updateLoop() ends a state's lose transition once this
 * returns nonzero.
 *
 * @return always 1 (transition finished)
 */
extern "C" int cFEPopupVideoCalibration_onLoseTransition(void) {
    return 1;
}
#endif
