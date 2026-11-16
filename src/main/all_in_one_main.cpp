#include "common.h"
#include "fe/uistatebuypopup.h"
#include "fe/festatenet.h"

//100%
INCLUDE_ASM("main/all_in_one_main", __sti__all_in_one_main_cpp);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;

void cMenu_cMenu(void*);
extern "C" void cMenu_addItem(void *, char*, int);
extern "C" void func_002CCF98(char*, int, int *, int);
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
 * @param acceptOwner  the owner for Accept: its first word is stored at +0x130
 * @param declineOwner  the owner for Decline: its first word is stored at +0x134
 * @return self
 */
extern "C" char* __sti__all_in_one_main_cpp(char* self, int* acceptOwner, int* declineOwner)
{
    char* item0 = self + 0x138;
    char* item1 = self + 0x154;
    char* item2 = self + 0x170;
    char* item3 = self + 0x18C;
    char* item4 = self + 0x1A4;
    char* item5 = self + 0x1C8;
    cMenu_cMenu(self);
    *(char**)(self + 0x12C) = D_0046D3D0;
    *(int*)(self + 0x130) = *acceptOwner;
    *(int*)(self + 0x134) = *declineOwner;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)item0, str_You_ve_been_Challenged);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)item1, str_by);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)item2, D_00467DC8);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)item3, (void*)0xE);
    func_002CCF98(item4, 0, D_00467DD0, 0);
    func_002CCF98(item5, 1, D_00467DD8, 0);
    cMenu_addItem(self, item0, -1);
    cMenu_addItem(self, item1, -1);
    cMenu_addItem(self, item2, -1);
    cMenu_addItem(self, item3, -1);
    cMenu_addItem(self, item4, -1);
    cMenu_addItem(self, item5, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB2B0);
#ifdef SKIP_ASM
void cMenu_cMenu(void*);
extern "C" void func_002CCF98(char*, int, int *, int);
extern "C" void cMenu_addItem(void *, char*, int);
extern int D_00467DE0[];
extern int D_0046D378[];

/**
 * Constructs the debug online front end's "Abort" menu: a cMenu with one command item.
 *
 * Runs cMenu_cMenu, stores the menu's vtable D_0046D378 at +0x12C and the owner's first word at +0x130, builds a
 * command item (func_002CCF98) at +0x134 with ID 0 and caption "Abort", and adds it with cMenu_addItem. Selecting it
 * runs func_001DBAB8. func_0017D298 builds it at +0x1AAC of the debug front end.
 *
 * @param arg0  the menu to construct
 * @param arg1  the owner reference; its first word (the debug front end) is stored at +0x130
 * @return @p arg0
 */
extern "C" void *func_001DB2B0(char *arg0, int *arg1) {
    cMenu_cMenu(arg0);
    char *p = arg0 + 0x134;
    *(int **)(arg0 + 0x12C) = D_0046D378;
    *(int *)(arg0 + 0x130) = *arg1;
    func_002CCF98(p, 0, D_00467DE0, 0);
    cMenu_addItem(arg0, p, -1);
    return arg0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB330);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_002CA388(void* item, int enable);

// Separate inline accessor: its own `i * 0x24` (not CSE'd with the call's address).
static inline void setItemData_B330(char* row, int value) { *(int*)(row + 0x17C) = value; }

extern "C" void func_001DB330_impl(char *self, char *src) __asm__("func_001DB330");
/**
 * Fills the debug "Delete Persona" menu with every persona except the first.
 *
 * For each persona i from 1 to LobbyMan::mNumPersonas - 1 it uses command item i - 1 (items of 0x24 bytes from +0x168):
 * the caption (+0x14 of the item) becomes mPersonas[i], the item is activated (func_002CA388 with 1) and its ID (+0x8)
 * is set to i. The remaining items up to 4 are deactivated. Persona 0 is never offered for deletion.
 *
 * The object is the Delete Persona menu (built by func_001DABD0).
 *
 * @param src  the lobby manager (LobbyMan): mNumPersonas at +0x6C, mPersonas at +0x70
 */
extern "C" void func_001DB330_impl(char *self, char *src) {
    int i = 0;
    int next;
    while (i < *(int*)(src + 0x6C) - 1) {
        next = i + 1;
        setItemData_B330(self + i * 36, *(int*)(src + next * 4 + 0x70));
        func_002CA388(self + (i * 36 + 360), 1);
        *(int*)(self + i * 0x24 + 0x170) = next;
        i = next;
    }
    while (i < 4) {
        func_002CA388(self + i * 36 + 0x168, 0);
        i++;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/all_in_one_main", func_001DB418);
#ifdef SKIP_ASM
struct MenuItem_B418 {
    char pad0[8];
    int index;          // 0x08
    char padC[8];
    int data;           // 0x14
    char pad18[0xC];
};

struct Menu_B418 {
    char pad0[0x2E8];
    char sub2E8[0x518 - 0x2E8];     // 0x2E8
    MenuItem_B418 items[4];         // 0x518
    char item5A8[0x1C];             // 0x5A8
    char item5C4[0x1C];             // 0x5C4
};

struct Src_B418 {
    char pad0[0x6C];
    int count;          // 0x6C
    int ids[4];         // 0x70
};

extern "C" void func_002CA388(void* item, int enable);
extern "C" void func_001DB330(void* sub, Src_B418* src);

/**
 * Fills the debug "Select Persona" menu from the lobby manager's persona list.
 *
 * For each of LobbyMan::mNumPersonas personas, the command item at +0x518 + i * 0x24 gets the persona name as its
 * caption, is activated (func_002CA388) and gets ID i; the remaining items up to 4 are deactivated. It then refreshes
 * the Delete Persona submenu at +0x2E8 (func_001DB330), enables the "Delete Persona" item (+0x5C4) only when there are
 * at least 2 personas, and the "Create Persona" item (+0x5A8) only while there are fewer than 4.
 *
 * The object is the Select Persona menu (built by func_001DACB8).
 *
 * @param src  the lobby manager (LobbyMan): mNumPersonas at +0x6C, mPersonas at +0x70
 */
extern "C" void func_001DB418(Menu_B418* self, Src_B418* src)
{
    int i = 0;
    for (; i < src->count; i++) {
        int id = src->ids[i];
        MenuItem_B418* it = &self->items[i];
        it->data = id;
        func_002CA388(it, 1);
        it->index = i;
    }
    for (; i < 4; i++) {
        func_002CA388(&self->items[i], 0);
    }
    func_001DB330(self->sub2E8, src);
    func_002CA388(self->item5C4, src->count >= 2);
    func_002CA388(self->item5A8, src->count < 4);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB528);
#ifdef SKIP_ASM
struct MenuItem_B528 {
    char pad0[8];
    int index;          // 0x08
    char padC[8];
    void* data;         // 0x14
    char pad18[0xC];
};

struct Menu_B528 {
    char pad0[0x168];
    MenuItem_B528 items[12];    // 0x168
};

extern "C" int func_0025F120(void* src);
extern "C" void* func_0025F150(void* src, int i);
extern "C" void func_002CA388(void* item, int enable);
extern char str_Receiving_Room_List[];

/**
 * Fills the debug "Select Room" menu with the lobby's room names.
 *
 * Asks func_0025F120 for the room count and gives command item i (12 items of 0x24 bytes from +0x168) the name from
 * func_0025F150(src, i), activates it and sets its ID to i. With no rooms yet, the first item shows "Receiving Room
 * List" with ID -1 (which the menu's command handler func_001DBB90 ignores). The remaining items are deactivated.
 *
 * The object is the Select Room menu (built by func_001DAE20).
 *
 * @param src  the lobby manager (LobbyMan)
 */
extern "C" void func_001DB528(Menu_B528* self, void* src)
{
    int i = 0;
    int n = func_0025F120(src);
    for (; i < n; i++) {
        MenuItem_B528* it = &self->items[i];
        it->data = func_0025F150(src, i);
        func_002CA388(it, 1);
        it->index = i;
    }
    if (i == 0) {
        self->items[0].data = str_Receiving_Room_List;
        i = 1;
        func_002CA388(&self->items[0], 1);
        self->items[0].index = -1;
    }
    for (; i < 12; i++) {
        func_002CA388(&self->items[i], 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB620);
#ifdef SKIP_ASM
struct MenuItem_B620 {
    char pad0[8];
    int index;          // 0x08
    char padC[8];
    void* data;         // 0x14
    char pad18[0xC];
};

struct Menu_B620 {
    char pad0[0x16C];
    MenuItem_B620 items[24];    // 0x16C
};

extern "C" int func_0025F1B0(void* src);
extern "C" void* func_0025F1E0(void* src, int i, int flags);
extern "C" void func_002CA388(void* item, int enable);
extern char str_No_users_in_room[];

/**
 * Fills the debug "Room Occupants" menu with the users in the current room.
 *
 * Asks func_0025F1B0 for the user count and gives command item i (24 items of 0x24 bytes from +0x16C) the name from
 * func_0025F1E0(src, i, 0), activates it and sets its ID to i. An empty room shows "No users in room" with ID -1 (which
 * the command handler func_001DBB48 ignores). The remaining items are deactivated.
 *
 * The object is the Room Occupants menu (built by func_001DAF08).
 *
 * @param src  the lobby manager (LobbyMan)
 */
extern "C" void func_001DB620(Menu_B620* self, void* src)
{
    int i = 0;
    int n = func_0025F1B0(src);
    for (; i < n; i++) {
        MenuItem_B620* it = &self->items[i];
        it->data = func_0025F1E0(src, i, 0);
        func_002CA388(it, 1);
        it->index = i;
    }
    if (i == 0) {
        self->items[0].data = str_No_users_in_room;
        i = 1;
        func_002CA388(&self->items[0], 1);
        self->items[0].index = -1;
    }
    for (; i < 24; i++) {
        func_002CA388(&self->items[i], 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DB718);
#ifdef SKIP_ASM
void operator_delete(int*);
struct N_B718 { N_B718 *next; N_B718 *prev; };
struct L_B718 { int pad; N_B718 *head; };

/**
 * Empties a std::list: frees every node and leaves the head node linked to itself.
 *
 * Walks from head->next back to the head (the node pointer at +0x4), freeing each node with operator_delete, then sets
 * the head's next and prev to the head. The head itself is kept. The LobbyMan destructor uses it on mEventList; the
 * async-file and lobby-tag code use it on their own lists.
 */
extern "C" void func_001DB718(L_B718 *self) {
    N_B718 *node = self->head->next;
    while (node != self->head) {
        N_B718 *n = node;
        node = node->next;
        operator_delete((int *)n);
    }
    self->head->next = self->head;
    self->head->prev = self->head;
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

struct sPtrVecDB778 {
    void* alloc;
    void** start;
    void** finish;
    void** eos;
};

static inline void** sPtrVecDB778_allocate(unsigned int n)
{
    if (n == 1)
        return (void**)cMemMan_alloc(4, str_StlBlock, 0x20000000, 0);
    return (void**)operator_new_tagDB778(n * 4, str_StlBlock, 0x20000000, 0);
}

static inline void sPtrVecDB778_deallocate(void** p, unsigned int n)
{
    if (p != 0) {
        if (n == 1)
            operator_delete((int*)p);
        else
            cMemMan_free(p);
    }
}

static inline void** sPtrVecDB778_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

static inline void** sPtrVecDB778_ucopy(void** first, void** last, void** result)
{
    return sPtrVecDB778_copy(first, last, result);
}

static inline void sPtrVecDB778_construct(void** p, void* const& v)
{
    if (p != 0)
        *p = v;
}

static inline void** sPtrVecDB778_copy_backward(void** first, void** last, void** result)
{
    int n = last - first;
    func_0041610C(result - n, first, n * sizeof(void*));
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
extern "C" void func_001DB778(sPtrVecDB778* self, void** pos, void* const& x)
{
    if (self->finish != self->eos) {
        if (self->finish != 0)
            *self->finish = self->finish[-1];
        ++self->finish;
        void* x_copy = x;
        sPtrVecDB778_copy_backward(pos, self->finish - 2, self->finish - 1);
        *pos = x_copy;
    } else {
        unsigned int old_size = self->finish - self->start;
        unsigned int len = old_size != 0 ? 2 * old_size : 1;
        void** new_start = sPtrVecDB778_allocate(len);
        void** new_finish = new_start;
        new_finish = sPtrVecDB778_ucopy(self->start, pos, new_start);
        sPtrVecDB778_construct(new_finish, x);
        ++new_finish;
        new_finish = sPtrVecDB778_ucopy(pos, self->finish, new_finish);
        sPtrVecDB778_deallocate(self->start, self->eos - self->start);
        self->start = new_start;
        self->finish = new_finish;
        self->eos = new_start + len;
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

struct sPtrVecDB918 {
    void* alloc;
    void** start;
    void** finish;
    void** eos;
};

static inline void** sPtrVecDB918_allocate(unsigned int n)
{
    if (n == 1)
        return (void**)cMemMan_alloc(4, str_StlBlock, 0x20000000, 0);
    return (void**)operator_new_tagDB918(n * 4, str_StlBlock, 0x20000000, 0);
}

static inline void sPtrVecDB918_deallocate(void** p, unsigned int n)
{
    if (p != 0) {
        if (n == 1)
            operator_delete((int*)p);
        else
            cMemMan_free(p);
    }
}

static inline void** sPtrVecDB918_copy(void** first, void** last, void** result)
{
    func_0041610C(result, first, (last - first) * sizeof(void*));
    return result + (last - first);
}

static inline void** sPtrVecDB918_ucopy(void** first, void** last, void** result)
{
    return sPtrVecDB918_copy(first, last, result);
}

static inline void sPtrVecDB918_construct(void** p, void* const& v)
{
    if (p != 0)
        *p = v;
}

static inline void** sPtrVecDB918_copy_backward(void** first, void** last, void** result)
{
    int n = last - first;
    func_0041610C(result - n, first, n * sizeof(void*));
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
extern "C" void func_001DB918(sPtrVecDB918* self, void** pos, void* const& x)
{
    if (self->finish != self->eos) {
        if (self->finish != 0)
            *self->finish = self->finish[-1];
        ++self->finish;
        void* x_copy = x;
        sPtrVecDB918_copy_backward(pos, self->finish - 2, self->finish - 1);
        *pos = x_copy;
    } else {
        unsigned int old_size = self->finish - self->start;
        unsigned int len = old_size != 0 ? 2 * old_size : 1;
        void** new_start = sPtrVecDB918_allocate(len);
        void** new_finish = new_start;
        new_finish = sPtrVecDB918_ucopy(self->start, pos, new_start);
        sPtrVecDB918_construct(new_finish, x);
        ++new_finish;
        new_finish = sPtrVecDB918_ucopy(pos, self->finish, new_finish);
        sPtrVecDB918_deallocate(self->start, self->eos - self->start);
        self->start = new_start;
        self->finish = new_finish;
        self->eos = new_start + len;
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBAB8);
#ifdef SKIP_ASM
extern "C" void func_0017E558(int);

/**
 * Handles the debug "Abort" menu's command: hands control back to the debug front end via func_0017E558.
 *
 * It is the onCommand slot of the Abort menu's vtable D_0046D378. func_0017E558 pushes the Room Occupants menu, sets
 * the debug front end's state to 0xC and calls func_0025F628 on the lobby manager.
 *
 * @param menu  the Abort menu; the debug front end is at +0x130
 * @return 0
 */
extern "C" int func_001DBAB8(void *menu) {
    func_0017E558((*(int *)((char*)(menu) + (0x130))));
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBAD8);
#ifdef SKIP_ASM
extern "C" void func_0017E490(int);
extern "C" void func_0017E4F0(int);

/**
 * Handles the debug challenge menu's commands: Accept or Decline.
 *
 * It is the onCommand slot of the challenge menu's vtable D_0046D3D0 (built by __sti__all_in_one_main_cpp). Item ID 0
 * (Accept) calls func_0017E490 with the owner at +0x130, which answers the challenge through func_0025FB58; ID 1
 * (Decline) calls func_0017E4F0 with the owner at +0x134, which declines through func_0025FC28.
 *
 * @param menu  the challenge menu
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
extern "C" int func_001DBAD8(void *menu, void *item) {
    int temp_3;

    temp_3 = (*(int *)((char*)(item) + (8)));
    if (temp_3 == 0) {
        func_0017E490((*(int *)((char*)(menu) + (0x130))));
    } else if (temp_3 == 1) {
        func_0017E4F0((*(int *)((char*)(menu) + (0x134))));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBB20);
#ifdef SKIP_ASM
extern "C" void func_0017E418(int, int);

/**
 * Handles the debug "Chat Menu" command: sends the chosen line as a lobby chat message.
 *
 * It is the onCommand slot of the chat menu's vtable D_0046D428 (built by func_001DB028). It passes the item's caption
 * (+0x14, one of the canned lines in D_00467D70) to func_0017E418 with the debug front end at +0x130, which returns to
 * the Room Occupants menu and sends the text with func_0025AAE0 ("mesg").
 *
 * @param menu  the chat menu
 * @param item  the chosen command item
 * @return 0
 */
extern "C" int func_001DBB20(void *menu, void *item) {
    func_0017E418((*(int *)((char*)(menu) + (0x130))), (*(int *)((char*)(item) + (0x14))));
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBB48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E368_BB48(int, int) __asm__("func_0017E368");
extern "C" void func_0017E3B8_BB48(int) __asm__("func_0017E3B8");

/**
 * Handles the debug "Room Occupants" menu's commands.
 *
 * It is the onCommand slot of vtable D_0046D480 (built by func_001DAF08). The "Chat" item (ID -2) calls func_0017E3B8
 * with the owner at +0x134, which opens the chat menu; an occupant (ID 0 and up) calls func_0017E368 with the owner at
 * +0x130 and the occupant's index; the placeholder line (ID -1) does nothing.
 *
 * @param menu  the Room Occupants menu
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
extern "C" int func_001DBB48(void *menu, void *item) {
    int itemValue = *(int *)((char *)item + 8);
    if (itemValue == -2) {
        func_0017E3B8_BB48(*(int *)((char *)menu + 0x134));
    } else if (itemValue != -1) {
        func_0017E368_BB48(*(int *)((char *)menu + 0x130), itemValue);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBB90);
#ifdef SKIP_ASM
extern "C" void func_0017E2F8(int, int);

/**
 * Handles the debug "Select Room" menu's command: joins the chosen room.
 *
 * It is the onCommand slot of vtable D_0046D4D8 (built by func_001DAE20). For an item ID of 0 or more it calls
 * func_0017E2F8 with the debug front end at +0x130 and the room index (state 11, then func_0025D800 on the lobby
 * manager); the "Receiving Room List" placeholder (ID -1) is ignored.
 *
 * @param menu  the Select Room menu
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
extern "C" int func_001DBB90(void *menu, void *item) {
    int temp_5;

    temp_5 = (*(int *)((char*)(item) + (8)));
    if (temp_5 >= 0) {
        func_0017E2F8((*(int *)((char*)(menu) + (0x130))), temp_5);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBBC0);
#ifdef SKIP_ASM
extern "C" void func_0017E290(int, int);

/**
 * Handles the debug "Select Persona" menu's command: selects the chosen persona.
 *
 * It is the onCommand slot of vtable D_0046D530 (built by func_001DACB8). It calls func_0017E290 with the debug front
 * end stored at +0x4E0 and the item's ID (the persona index), which sets state 7 and calls func_0025CD28 on the lobby
 * manager.
 *
 * @param menu  the Select Persona menu
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
extern "C" int func_001DBBC0(void *menu, void *item) {
    func_0017E290((*(int *)((char*)(menu) + (0x4E0))), (*(int *)((char*)(item) + (8))));
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBBE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E1C0_BBE8(int, void *) __asm__("func_0017E1C0");

/**
 * Handles the debug "Create Persona" menu's command: creates a persona with the typed name.
 *
 * It is the onCommand slot of vtable D_0046D588 (built by func_001DAAC8). It calls func_0017E1C0 with the debug front
 * end at +0x130 and the persona name buffer at +0x180 (the text-entry item at +0x168), which sets state 8 and calls
 * func_0025D1B8 on the lobby manager. It ignores which item was chosen.
 *
 * @param menu  the Create Persona menu
 * @return 0
 */
extern "C" int func_001DBBE8(char *menu) {
    func_0017E1C0_BBE8(*(int *)(menu + 0x130), menu + 0x180);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBC10);
#ifdef SKIP_ASM
extern "C" void func_0017E228(int, int);

/**
 * Handles the debug "Delete Persona" menu's command: deletes the chosen persona.
 *
 * It is the onCommand slot of vtable D_0046D5E0 (built by func_001DABD0). It calls func_0017E228 with the debug front
 * end at +0x130 and the item's ID (the persona index set by func_001DB330), which sets state 9 and calls func_0025D428
 * on the lobby manager.
 *
 * @param menu  the Delete Persona menu
 * @param item  the chosen item; its ID is at +0x8
 * @return 0
 */
extern "C" int func_001DBC10(void *menu, void *item) {
    func_0017E228((*(int *)((char*)(menu) + (0x130))), (*(int *)((char*)(item) + (8))));
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBC38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E030_BC38(int, char*, char*) __asm__("func_0017E030");

/**
 * Handles the debug "Create Account" menu's command: creates an account with the typed name and password.
 *
 * It is the onCommand slot of vtable D_0046D690 (built by func_001DA648). It calls func_0017E030 with the debug front
 * end at +0x130, the login-name buffer at +0x180 and the password buffer at +0x1AC; that fills the lobby manager's
 * mNewAccount form and sends it with func_0025B9C8. It ignores which item was chosen.
 *
 * The object is the Create Account menu.
 *
 * @return 0
 */
extern "C" int func_001DBC38(char *self) {
    func_0017E030_BC38(*(int*)(self + 0x130), self + 0x180, self + 0x1AC);
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DBC68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0017E118_BC68(int, char*, char*) __asm__("func_0017E118");

/**
 * Handles the debug "Login" menu's command: logs in with the typed name and password.
 *
 * It is the onCommand slot of vtable D_0046D6E8 (built by func_001DA7E8). It calls func_0017E118 with the debug front
 * end at +0x130, the login-name buffer at +0x180 and the password buffer at +0x1AC; that copies them into the lobby
 * manager's mUserName / mPassword and calls func_0025C610. It ignores which item was chosen.
 *
 * The object is the Login menu.
 *
 * @return 0
 */
extern "C" int func_001DBC68(char *self) {
    func_0017E118_BC68(*(int*)(self + 0x130), self + 0x180, self + 0x1AC);
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
extern char D_004A4E80;
extern int D_004A4E84;
extern int D_004A4E88_v[2] __asm__("D_004A4E88"); // {int, int} pair (feasyncfile names it the same way)
extern "C" void func_001A06B0(void*);
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
extern float D_004C66A8[3];
extern float D_004C66B8[3];
extern float D_004C66C8[4];
extern float gDisableColour[4];
extern float D_004C66E8[4];
extern float D_004C66F8[4];
extern float D_004C6708[4];
extern float D_004C6718[4];
extern float D_004C6728[4];
extern float D_004C6738[4];
extern float D_004C6748[4];
extern float D_004C6758[4];
extern float D_004C6768[4];
extern float D_004C6778[4];
extern float gPopupWhite[4];
extern float gPopupBlack[4];
extern float D_004C67A8[4];
extern float D_004C67B8[4];
extern float D_004C67C8[4];
extern float D_004C67D8[4];
extern float D_004C67E8[4];
extern float D_004C67F8[4];
extern float D_004C6808[4];
extern float D_004C6818[4];
extern float D_004C6828[4];
extern float D_004C6838[4];
void sti_001DBC98(int init, int prio) __asm__("func_001DBC98");
/**
 * Static initializer and destructor for all_in_one_main's globals (the init/priority routine g++ emits per unit).
 *
 * Only acts when @p prio is 0xFFFF. On the construct pass (@p init non-zero) it fills about 80 four-float colour
 * constants (D_004C6328 .. D_004C6838, including gDisableColour = {1, 0.6, 0.6, 0.6}, gPopupWhite = {1, 1, 1, 1} and
 * gPopupBlack = {1, 0, 0, 0}), two three-float vectors (D_004C66A8 = {100, 128, 0}, D_004C66B8 = {130, 166, 0}),
 * fifteen packed id/value pairs D_004A4E08 .. D_004A4E78 (0 to 13, then -1), and zeroes D_004A4E00, D_004A4E04 and the
 * front-end flow state words D_004A4E80 .. D_004A4E8C. On the destroy pass it calls func_001A06B0 on D_004A4E80. The
 * first colour values are the same as the ones func_003AD290 writes in world/wscriptcache, so they come from a shared
 * header.
 *
 * @param init  1 to construct, 0 to destroy
 * @param prio  initialization priority; only 0xFFFF does anything
 */
void sti_001DBC98(int init, int prio)
{
    if (prio == 0xFFFF) {
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
            D_004C66A8[0] = 100.0f;
            D_004C66A8[1] = 128.0f;
            D_004C66A8[2] = 0.0f;
            D_004C66B8[0] = 130.0f;
            D_004C66B8[1] = 166.0f;
            D_004C66B8[2] = 0.0f;
            D_004C66C8[0] = 0.5f;
            D_004C66C8[1] = D_004C6338[3];
            D_004C66C8[2] = D_004C6338[3];
            D_004C66C8[3] = D_004C6338[3];
            gDisableColour[0] = 1.0f;
            gDisableColour[1] = 0.600000024f;
            gDisableColour[2] = 0.600000024f;
            gDisableColour[3] = 0.600000024f;
            D_004C66E8[0] = 1.0f;
            D_004C66E8[1] = 0.90196085f;
            D_004C66E8[2] = 0.117647067f;
            D_004C66E8[3] = 0.117647067f;
            D_004C66F8[0] = 1.0f;
            D_004C66F8[1] = 0.862745166f;
            D_004C66F8[2] = 0.34117648f;
            D_004C66F8[3] = D_004C6338[3];
            D_004C6708[0] = 1.0f;
            D_004C6708[1] = 1.0f;
            D_004C6708[2] = 1.0f;
            D_004C6708[3] = 1.0f;
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
            D_004A4E80 = 0;
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
            D_004C67A8[0] = 1.0f;
            D_004C67A8[1] = 0.400000006f;
            D_004C67A8[2] = 0.720000029f;
            D_004C67A8[3] = 0.790000021f;
            D_004C67B8[0] = 0.400000006f;
            D_004C67B8[1] = D_004C6338[3];
            D_004C67B8[2] = D_004C6338[3];
            D_004C67B8[3] = D_004C6338[3];
            D_004C67C8[0] = 1.0f;
            D_004C67C8[1] = 1.0f;
            D_004C67C8[2] = 1.0f;
            D_004C67C8[3] = 1.0f;
            D_004C67D8[0] = 1.0f;
            D_004C67D8[1] = 1.0f;
            D_004C67D8[2] = 0.5f;
            D_004C67D8[3] = 0.25f;
            D_004C67E8[0] = 1.0f;
            D_004C67E8[1] = D_004C6338[3];
            D_004C67E8[2] = D_004C6338[3];
            D_004C67E8[3] = D_004C6338[3];
            D_004C67F8[0] = 0.300000012f;
            D_004C67F8[1] = D_004C6338[3];
            D_004C67F8[2] = D_004C6338[3];
            D_004C67F8[3] = D_004C6338[3];
            D_004C6808[0] = 1.0f;
            D_004C6808[1] = 1.0f;
            D_004C6808[2] = 1.0f;
            D_004C6808[3] = 1.0f;
            D_004C6818[0] = 0.600000024f;
            D_004C6818[1] = 1.0f;
            D_004C6818[2] = 1.0f;
            D_004C6818[3] = 1.0f;
            D_004C6828[0] = 1.0f;
            D_004C6828[1] = 1.0f;
            D_004C6828[2] = 1.0f;
            D_004C6828[3] = 1.0f;
            D_004C6838[0] = 1.0f;
            D_004C6838[1] = 1.0f;
            D_004C6838[2] = 1.0f;
            D_004C6838[3] = 1.0f;
        } else {
            func_001A06B0(&D_004A4E80);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DC900);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* D_0046D970[];

/**
 * Destroys an app module base: stores the root module vtable D_0046D970 at +0x0, then frees the object when asked to.
 *
 * D_0046D970 is the vtable every screen and module (cPreStartScreen, cInitLoadScreen, the front end, cSSXApp's
 * secondary vtable) falls back to; its slots are the destructor, load, purge, loseFocus, gainFocus, update and render.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_001DC900(void *self, int flags) {
    *(void***)((char*)self + 0) = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

/**
 * Does nothing: the root module's default for vtable slot 0x1C, which the load screens, the front end and cSSXApp
 * inherit.
 */
extern "C" void func_001DC930(void) {
}

/**
 * Does nothing: the root module's default for vtable slot 0x2C, which the load screens, the front end and cSSXApp
 * inherit.
 */
extern "C" void func_001DC938(void) {
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
extern "C" void cDebugScreen__cDebugScreen(void *self, int flags) {
    *(void***)((char*)self + 0) = cDebugScreen_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DC970);
#ifdef SKIP_ASM
/**
 * Returns the word at +0x18 of the debug front end, the font cDebugMenuScreen_draw reads from SSXApp's DebugFE.
 *
 * @param arg0  the debug front end
 * @return the word at +0x18
 */
extern "C" int func_001DC970(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x18)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DC978);
#ifdef SKIP_ASM
/**
 * Returns the word at +0x20 of the debug front end, 8 bytes after its font word (func_001DC970).
 *
 * @param arg0  the debug front end
 * @return the word at +0x20
 */
extern "C" int func_001DC978(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DC980);
#ifdef SKIP_ASM
/**
 * Copies the word at +0x10 of the debug front end into +0x14, the word just before its font (+0x18).
 *
 * @param arg0  the debug front end
 */
extern "C" void func_001DC980(void *arg0) {
    (*(int *)((char*)(arg0) + (0x14))) = (int) (*(int *)((char*)(arg0) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DC990);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* func_001DC990_va_list;
#define func_001DC990_va_start(ap)                                                \
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
extern "C" void func_001DC990(float x, float y, int font, int fmt, ...)
{
    func_001DC990_va_list ap;
    func_001DC990_va_start(ap);
    cFont_vtextf_C990(font, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DC9D0);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define func_001DC9D0_va_start(ap)                                       \
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
extern "C" void func_001DC9D0(void *self, int fmt, float posX, float posY, float sx, float sy, ...)
{
    char *ap;
    func_001DC9D0_va_start(ap);
    *(float *)((char *)self + 0x38) = *(float *)((char *)self + 0x30) * sx;
    *(float *)((char *)self + 0x3C) = *(float *)((char *)self + 0x34) * sy;
    cFont_vtextf_C9D0(self, fmt, ap);
    *(V2_C9D0 *)((char *)self + 0x38) = *(V2_C9D0 *)((char *)self + 0x30);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCA40);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* func_001DCA40_va_list;
#define func_001DCA40_va_start(ap)                                                \
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
extern "C" void func_001DCA40(float x, float y, int font, int fmt, ...)
{
    func_001DCA40_va_list ap;
    func_001DCA40_va_start(ap);
    cFont_vcentertextf_CA40(font, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCA80);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* func_001DCA80_va_list;
#define func_001DCA80_va_start(ap)                                                \
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
extern "C" void func_001DCA80(float x, float y, int font, int fmt, ...)
{
    func_001DCA80_va_list ap;
    func_001DCA80_va_start(ap);
    cFont_vrighttextf_CA80(font, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCAC0);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* cSimpleCamera_vtbl[];

/**
 * Destroys the front end's camera: stores cSimpleCamera's vtable at +0x90, then frees the object when asked to.
 *
 * It is slot 1 of D_0046D8D8, the vtable cFE_cFE stores at +0x90 of the camera it builds at +0x10 of the front end; the
 * derived class adds nothing to destroy, so only the base vtable store is left.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_001DCAC0(void *self, int flags) {
    *(void***)((char*)self + 0x90) = cSimpleCamera_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCAF0);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void *, int);
extern int D_0046D8C0[];

/**
 * Destroys a private message: its three strings, then frees the object when asked to.
 *
 * Stores the vtable D_0046D8C0 at +0xC, destroys the cBXStrings at +0x8, +0x4 and +0x0 (cBXString__cBXString with flags
 * 2: destroy without freeing) and calls operator_delete when bit 0 of @p arg1 is set.
 *
 * @param arg0  the message (0x10 bytes: three cBXStrings and the vptr at +0xC)
 * @param arg1  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_001DCAF0(char *arg0, int arg1) {
    *(int **)(arg0 + 0xC) = D_0046D8C0;
    cBXString__cBXString(arg0 + 8, 2);
    cBXString__cBXString(arg0 + 4, 2);
    cBXString__cBXString(arg0, 2);
    if (arg1 & 1) {
        operator_delete((int*)arg0);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCB60);
#ifdef SKIP_ASM
/**
 * Returns the front end's camera, the member at +0x10.
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x10
 */
extern "C" int func_001DCB60(int realFe) {
    return realFe + 0x10;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCB68);
#ifdef SKIP_ASM
/**
 * Returns the front end's camera, the member at +0x10 (the second of an identical pair of out-of-line copies).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x10
 */
extern "C" int func_001DCB68(int realFe) {
    return realFe + 0x10;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCB70);
#ifdef SKIP_ASM
extern "C" void func_001A0538(int);

/**
 * Returns one human rider of the front end's rider manager.
 *
 * Calls func_001A0538 on the rider manager at +0xB0; the rider index arrives in the second argument register and is
 * passed through untouched (func_001A0538 returns manager + index * 0xCE0).
 *
 * @param realFe  the front end (cRealFE / cFE)
 */
extern "C" void func_001DCB70(int realFe) {
    func_001A0538(realFe + 176);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCB90);
#ifdef SKIP_ASM
extern "C" void func_001A0548(int);

/**
 * Returns one human rider of the front end's rider manager (the second of an identical pair of out-of-line copies).
 *
 * Calls func_001A0548 on the rider manager at +0xB0 with the rider index passed through in the second argument
 * register.
 *
 * @param realFe  the front end (cRealFE / cFE)
 */
extern "C" void func_001DCB90(int realFe) {
    func_001A0548(realFe + 176);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCBB0);
#ifdef SKIP_ASM
/**
 * Returns the front end's rider manager, the member at +0xB0.
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0xB0
 */
extern "C" int func_001DCBB0(int realFe) {
    return realFe + 176;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCBB8);
#ifdef SKIP_ASM
/**
 * Returns the front end's rider manager, the member at +0xB0 (the second of an identical pair of out-of-line copies).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0xB0
 */
extern "C" int func_001DCBB8(int realFe) {
    return realFe + 176;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCBC0);
#ifdef SKIP_ASM
/**
 * Returns the front end's member at +0x1A70 (the object cFE_cFE builds with func_0019CDF0).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x1A70
 */
extern "C" int func_001DCBC0(int realFe) {
    return realFe + 6768;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCBC8);
#ifdef SKIP_ASM
/**
 * Returns the front end's member at +0x1A70 (the second of an identical pair of out-of-line copies).
 *
 * @param realFe  the front end (cRealFE / cFE)
 * @return @p realFe + 0x1A70
 */
extern "C" int func_001DCBC8(int realFe) {
    return realFe + 6768;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCBD0);
#ifdef SKIP_ASM
/**
 * Returns the front end's UI engine, the pointer at +0xC.
 *
 * @param arg0  the front end (cFE)
 * @return the UI engine pointer cFE_cFE allocates ("UIEngine") and stores at +0xC
 */
extern "C" int func_001DCBD0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xC)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCBD8);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* D_0046D848[];

/**
 * Destroys a UI locale interface: stores its vtable D_0046D848 at +0x4, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_001DCBD8(void *self, int flags) {
    *(void***)((char*)self + 4) = D_0046D848;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCC08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_001E1090_CC08(void*, int) __asm__("func_001E1090");

/**
 * Sets the locale's display mode (the byte at +0x0) and turns the text-num-char flag on for mode 2.
 *
 * Stores @p v at +0x0, then calls func_001E1090, which sets bit 3 of the attribute byte at +0x3 (tAttrLocale's fourth
 * flag, On Tour's mTextNumChar) when @p v is 2 and clears it otherwise. It is slot 0x14 of both the base locale vtable
 * D_0046D848 and the front-end locale's D_0046BF98.
 *
 * The object is the locale interface.
 *
 * @param v  the new display mode
 */
extern "C" void func_001DCC08(signed char *self, signed char v) {
    *self = v;
    func_001E1090_CC08(self, v == 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCC38);
#ifdef SKIP_ASM
/**
 * Sets the locale's language: the byte at +0x1.
 *
 * The base locale's version (vtable D_0046D848, slot 0x1C); the front-end locale overrides it with func_00195AA0, which
 * also reloads the language files.
 *
 * @param arg0  the locale interface
 * @param arg1  the language index
 */
extern "C" void func_001DCC38(void *arg0, signed char arg1) {
    (*(signed char *)((char*)(arg0) + (1))) = arg1;
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
INCLUDE_ASM("main/all_in_one_main", func_001DCC58);
#ifdef SKIP_ASM
/**
 * Returns 0: the base locale's default for vtable slot 0x24 (D_0046D848).
 *
 * @return always 0
 */
extern "C" int func_001DCC58(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCC60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_0046D810_CC60[] __asm__("cFECustom_vtbl");

/**
 * Constructs an object with cFECustom's vtable: zeroes the word at +0x0 and stores cFECustom_vtbl at +0x4.
 *
 * No other field is set. No caller in the code; it is an out-of-line copy of an inline constructor.
 *
 * @param arg0  the object
 * @return @p arg0
 */
extern "C" void *func_001DCC60(void *arg0) {
    (*(int *)((char*)(arg0) + (0))) = 0;
    (*(void ***)((char*)(arg0) + (4))) = D_0046D810_CC60;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCC78);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* D_0046DD28[];

/**
 * Destroys a front-end custom handler: stores the base vtable D_0046DD28 at +0x4, then frees the object when asked to.
 *
 * It is slot 1 (the destructor) of cFECustom_vtbl; the derived class adds nothing to destroy.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void func_001DCC78(void *self, int flags) {
    *(void***)((char*)self + 4) = D_0046DD28;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

/**
 * Does nothing: cFECustom's per-frame update (slot 0x14 of cFECustom_vtbl).
 */
extern "C" void func_001DCCA8(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCCB0);
#ifdef SKIP_ASM
/**
 * Stores a value in the word at +0xC of an object.
 *
 * No caller or vtable references it; it sits with the cFECustom helpers.
 *
 * @param arg0  the object
 * @param arg1  the value to store
 */
extern "C" void func_001DCCB0(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0xC))) = arg1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCCB8);
#ifdef SKIP_ASM
extern int cRealFE_mStartState;

/**
 * Sets the front end's start state, the static cRealFE_mStartState.
 *
 * @param startState  the state to start the front end in
 */
extern "C" void func_001DCCB8(int startState) {
    cRealFE_mStartState = startState;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCCC0);
#ifdef SKIP_ASM
extern int cRealFE_mStartState;

/**
 * Returns the front end's start state, the static cRealFE_mStartState.
 *
 * @return cRealFE_mStartState
 */
extern "C" int func_001DCCC0(void) {
    return cRealFE_mStartState;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCCC8);
#ifdef SKIP_ASM
extern int cRealFE_mMovieList;

/**
 * Sets bit 2 (value 4) of the front end's movie flags, the static cRealFE_mMovieList.
 */
extern "C" void func_001DCCC8(void) {
    cRealFE_mMovieList |= 4;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCCD8);
#ifdef SKIP_ASM
/**
 * Returns the front end's connection manager at +0xB5AB0 (the one func_001A1CE8 builds with cConnMan_cConnMan__FPv).
 *
 * @param realFe  the front end (cRealFE)
 * @return @p realFe + 0xB5AB0
 */
extern "C" int func_001DCCD8(int realFe) {
    return realFe + 0xB5AB0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCCE8);
#ifdef SKIP_ASM
/**
 * Returns the front end's game connection manager at +0xB5ABC (the one func_001A1CE8 builds with
 * cGameConnMan_cGameConnMan__FPv).
 *
 * @param realFe  the front end (cRealFE)
 * @return @p realFe + 0xB5ABC
 */
extern "C" int func_001DCCE8(int realFe) {
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
extern "C" void cDebugMenuScreen__cDebugMenuScreen(void *self, int flags) {
    cBackgroundMan__cBackgroundMan_CD00((char *)self + 0x8C10, 2);
    *(void ***)self = cDebugScreen_vtbl;
    if (flags & 1) {
        operator_delete((int *)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCD58);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys the debug lobby's "Continue" menu (vtable D_0046D740, built by func_0017D200): its one item, then the menu.
 *
 * Destroys the item at +0x130 with cMenuItem__cMenuItem (flags 2: no free), then runs cMenu__cMenu with the caller's
 * flags. The July 2003 preview calls this class ContinueMenu.
 *
 * @param menu  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DCD58(int menu, int flags) {
    cMenuItem__cMenuItem(menu + 304, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCDA0);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys the debug lobby's "Login" menu (vtable D_0046D6E8, built by func_001DA7E8).
 *
 * Destroys its six items in reverse order (+0x1E4 Cancel, +0x1C0 the command item, +0x194 password, +0x168 login name,
 * +0x150 spacer, +0x134 title) with cMenuItem__cMenuItem (flags 2), then runs cMenu__cMenu with the caller's flags. The
 * preview calls this class AccountDetailsMenu<cDebugLobbyScreen::OnLogin>.
 *
 * @param menu  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DCDA0(int menu, int flags) {
    cMenuItem__cMenuItem(menu + 484, 2);
    cMenuItem__cMenuItem(menu + 0x1C0, 2);
    cMenuItem__cMenuItem(menu + 404, 2);
    cMenuItem__cMenuItem(menu + 360, 2);
    cMenuItem__cMenuItem(menu + 336, 2);
    cMenuItem__cMenuItem(menu + 308, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCE20);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys the debug lobby's "Create Account" menu (vtable D_0046D690, built by func_001DA648).
 *
 * Same layout as the Login menu: destroys the six items from +0x1E4 down to +0x134 with cMenuItem__cMenuItem (flags 2),
 * then runs cMenu__cMenu with the caller's flags. The preview calls this class
 * AccountDetailsMenu<cDebugLobbyScreen::OnCreate>.
 *
 * @param menu  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DCE20(int menu, int flags) {
    cMenuItem__cMenuItem(menu + 484, 2);
    cMenuItem__cMenuItem(menu + 0x1C0, 2);
    cMenuItem__cMenuItem(menu + 404, 2);
    cMenuItem__cMenuItem(menu + 360, 2);
    cMenuItem__cMenuItem(menu + 336, 2);
    cMenuItem__cMenuItem(menu + 308, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCEA0);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

// PORT: pointers passed as int (unit declares the callees with int parameters)
static inline void SubMenuB_CEA0(int m)
{
    cMenuItem__cMenuItem(m + 0x24C, 2);
    cMenuItem__cMenuItem(m + 0x230, 2);
    cMenuItem__cMenuItem(m + 0x218, 2);
    cMenuItem__cMenuItem(m + 0x1FC, 2);
    cMenuItem__cMenuItem(m + 0x1E4, 2);
    cMenuItem__cMenuItem(m + 0x1C0, 2);
    cMenuItem__cMenuItem(m + 0x194, 2);
    cMenuItem__cMenuItem(m + 0x168, 2);
    cMenuItem__cMenuItem(m + 0x150, 2);
    cMenuItem__cMenuItem(m + 0x134, 2);
    cMenu__cMenu(m, 2);
}

static inline void SubMenuA_CEA0(int m)
{
    SubMenuB_CEA0(m + 0x1FC);
    cMenuItem__cMenuItem(m + 0x1E4, 2);
    cMenuItem__cMenuItem(m + 0x1C0, 2);
    cMenuItem__cMenuItem(m + 0x194, 2);
    cMenuItem__cMenuItem(m + 0x168, 2);
    cMenuItem__cMenuItem(m + 0x150, 2);
    cMenuItem__cMenuItem(m + 0x134, 2);
    cMenu__cMenu(m, 2);
}

/**
 * Destroys the debug lobby's "Login Method" menu (vtable D_0046D638, built by func_001DA988) with its two submenus.
 *
 * Tears down, innermost first, the menu's four items (+0x578, +0x55C, +0x544, +0x528), the embedded Login menu at
 * +0x32C (its items and cMenu base) and the Create Account menu at +0x130 (its items and cMenu base), all with flags 2,
 * then runs cMenu__cMenu on itself with the caller's flags. The debug front end's destructor (func_0017D3B0) calls it
 * for its member at +0x70. The preview calls this class AccountMenu<OnCreate, OnLogin>.
 *
 * @param flags  g++ destructor flags passed on to the last cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DCEA0(int self, int flags)
{
    SubMenuA_CEA0(self + 304);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DCFC8);
#ifdef SKIP_ASM
struct VEnt { short delta; short idx; void (*fn)(void*, int); };
struct VTab { int a; int b; VEnt dtor; };
struct Elem { char pad[0x10]; VTab *vt; char pad2[0x24 - 0x14]; };
struct Obj { char pad[0x168]; Elem elems[4]; };
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys the debug lobby's "Delete Persona" menu (vtable D_0046D5E0, built by func_001DABD0).
 *
 * Destroys the four command items at +0x168 (0x24 bytes each) in reverse order through each item's virtual destructor
 * (the vtable at +0x10 of the item, with the entry's this adjustment), then the spacer (+0x150) and the title (+0x134)
 * with cMenuItem__cMenuItem, and finally cMenu__cMenu with the caller's flags. The preview calls this class
 * DeletePersonaMenu<cDebugLobbyScreen::OnDeletePersona>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DCFC8(Obj *self, int flags) {
    if (self->elems != 0) {
        Elem *end = self->elems + 4;
        Elem *elem = end;
        while (self->elems != elem) {
            elem--;
            elem->vt->dtor.fn((char*)elem + elem->vt->dtor.delta, 0);
        }
    }
    cMenuItem__cMenuItem((int)self + 336, 2);
    cMenuItem__cMenuItem((int)self + 308, 2);
    cMenu__cMenu((int)self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD060);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys the debug lobby's "Create Persona" menu (vtable D_0046D588, built by func_001DAAC8).
 *
 * Destroys its four items (+0x194 Create, +0x168 persona name, +0x150 spacer, +0x134 title) with cMenuItem__cMenuItem
 * (flags 2), then runs cMenu__cMenu with the caller's flags. The preview calls this class
 * CreatePersonaMenu<cDebugLobbyScreen::OnCreatePersona>.
 *
 * @param menu  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DD060(int menu, int flags) {
    cMenuItem__cMenuItem(menu + 404, 2);
    cMenuItem__cMenuItem(menu + 360, 2);
    cMenuItem__cMenuItem(menu + 336, 2);
    cMenuItem__cMenuItem(menu + 308, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD0C8);
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
 * Destroys the debug lobby's "Select Persona" menu (vtable D_0046D530, built by func_001DACB8) with its two submenus.
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
__asm__(".globl func_001DD0C8\n.type func_001DD0C8,@function\nfunc_001DD0C8 = _$_18cPersonaMenu_DD0C8\n.size func_001DD0C8,0x18C\n");
__asm__("_$_15cMenuPart_DD0C8 = cMenuItem__cMenuItem\n_$_15cMenuBase_DD0C8 = cMenu__cMenu\n");
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD258);
#ifdef SKIP_ASM
struct VEnt_D258 { short delta; short idx; void (*fn)(void*, int); };
struct VTab_D258 { int a; int b; VEnt_D258 dtor; };
struct Elem_D258 { char pad[0x10]; VTab_D258 *vt; char pad2[0x24 - 0x14]; };
struct Obj_D258 { char pad[0x168]; Elem_D258 elems[12]; };

/**
 * Destroys the debug lobby's "Select Room" menu (vtable D_0046D4D8, built by func_001DAE20).
 *
 * Destroys the twelve room items at +0x168 in reverse order through their virtual destructors, then the spacer (+0x150)
 * and the title (+0x134) with cMenuItem__cMenuItem, and finally cMenu__cMenu with the caller's flags. The preview calls
 * this class LobbyMenu<cDebugLobbyScreen::OnSelectRoom>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DD258(Obj_D258 *self, int flags) {
    if (self->elems != 0) {
        Elem_D258 *end = self->elems + 12;
        Elem_D258 *elem = end;
        while (self->elems != elem) {
            elem--;
            elem->vt->dtor.fn((char*)elem + elem->vt->dtor.delta, 0);
        }
    }
    cMenuItem__cMenuItem((int)self + 336, 2);
    cMenuItem__cMenuItem((int)self + 308, 2);
    cMenu__cMenu((int)self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD2F0);
#ifdef SKIP_ASM
struct VEnt_D2F0 { short delta; short idx; void (*fn)(void*, int); };
struct VTab_D2F0 { int a; int b; VEnt_D2F0 dtor; };
struct Elem_D2F0 { char pad[0x10]; VTab_D2F0 *vt; char pad2[0x24 - 0x14]; };
struct Obj_D2F0 { char pad[0x16C]; Elem_D2F0 elems[24]; };

/**
 * Destroys the debug lobby's "Room Occupants" menu (vtable D_0046D480, built by func_001DAF08).
 *
 * Destroys the "Chat" item (+0x4CC), the 24 occupant items at +0x16C in reverse order through their virtual
 * destructors, the spacer (+0x154) and the title (+0x138), then runs cMenu__cMenu with the caller's flags. The preview
 * calls this class RoomMenu<cDebugLobbyScreen::OnSelectUser, OnChat>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DD2F0(Obj_D2F0 *self, int flags) {
    cMenuItem__cMenuItem((int)self + 1228, 2);
    if (self->elems != 0) {
        Elem_D2F0 *elem = self->elems + 24;
        while (self->elems != elem) {
            elem--;
            elem->vt->dtor.fn((char*)elem + elem->vt->dtor.delta, 0);
        }
    }
    cMenuItem__cMenuItem((int)self + 340, 2);
    cMenuItem__cMenuItem((int)self + 312, 2);
    cMenu__cMenu((int)self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD398);
#ifdef SKIP_ASM
struct VEnt_D398 { short delta; short idx; void (*fn)(void*, int); };
struct VTab_D398 { int a; int b; VEnt_D398 dtor; };
struct Elem_D398 { char pad[0x10]; VTab_D398 *vt; char pad2[0x24 - 0x14]; };
struct Obj_D398 { char pad[0x194]; Elem_D398 elems[14]; };

/**
 * Destroys the debug lobby's "Chat Menu" (vtable D_0046D428, built by func_001DB028).
 *
 * Destroys the 14 canned-message items at +0x194 in reverse order through their virtual destructors, then the chat
 * text-entry item (+0x168), the spacer (+0x150) and the title (+0x134) with cMenuItem__cMenuItem, and finally
 * cMenu__cMenu with the caller's flags. The preview calls this class ChatMenu<cDebugLobbyScreen::OnMessage>.
 *
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DD398(Obj_D398 *self, int flags) {
    if (self->elems != 0) {
        Elem_D398 *elem = self->elems + 14;
        while (self->elems != elem) {
            elem--;
            elem->vt->dtor.fn((char*)elem + elem->vt->dtor.delta, 0);
        }
    }
    cMenuItem__cMenuItem((int)self + 360, 2);
    cMenuItem__cMenuItem((int)self + 336, 2);
    cMenuItem__cMenuItem((int)self + 308, 2);
    cMenu__cMenu((int)self, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD440);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys the debug lobby's challenge menu (vtable D_0046D3D0, built by __sti__all_in_one_main_cpp).
 *
 * Destroys its six items in reverse order (+0x1C8 Decline, +0x1A4 Accept, +0x18C spacer, +0x170 empty line, +0x154
 * "by", +0x138 "You've been Challenged!") with cMenuItem__cMenuItem (flags 2), then runs cMenu__cMenu with the caller's
 * flags. The preview calls this class ChallengeResponseMenu<OnAcceptChallenge, OnDeclineChallenge>.
 *
 * @param menu  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DD440(int menu, int flags) {
    cMenuItem__cMenuItem(menu + 456, 2);
    cMenuItem__cMenuItem(menu + 420, 2);
    cMenuItem__cMenuItem(menu + 396, 2);
    cMenuItem__cMenuItem(menu + 368, 2);
    cMenuItem__cMenuItem(menu + 340, 2);
    cMenuItem__cMenuItem(menu + 312, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD4C0);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys the debug lobby's "Abort" menu (vtable D_0046D378, built by func_001DB2B0): its one item, then the menu.
 *
 * Destroys the command item at +0x134 with cMenuItem__cMenuItem (flags 2), then runs cMenu__cMenu with the caller's
 * flags. The preview calls this class AbortMenu<cDebugLobbyScreen::OnAbortChallenge>.
 *
 * @param menu  the menu
 * @param flags  g++ destructor flags passed on to cMenu__cMenu; bit 0 set frees the menu
 */
extern "C" void func_001DD4C0(int menu, int flags) {
    cMenuItem__cMenuItem(menu + 308, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD508);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_0046D1D0_D508[] __asm__("cFEInput_vtbl");

/**
 * Constructs a front-end input reader: no key ready, no keys, no input device.
 *
 * Clears the pad-number byte at +0x4, stores cFEInput_vtbl at +0x8 and zeroes the words at +0xC (key ready), +0x10
 * (virtual key), +0x14 (ASCII key) and +0x18 (input device pointer). The load screens build the same object inline.
 *
 * @param arg0  the object
 * @return @p arg0
 */
extern "C" void *func_001DD508(void *arg0) {
    (*(signed char *)((char*)(arg0) + (4))) = 0;
    (*(void ***)((char*)(arg0) + (8))) = D_0046D1D0_D508;
    (*(int *)((char*)(arg0) + (0x18))) = 0;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x10))) = 0;
    (*(int *)((char*)(arg0) + (0x14))) = 0;
    return arg0;
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
extern "C" void cFEInput__cFEInput(void *self, int flags) {
    *(void***)((char*)self + 8) = cUIInput_vtbl;
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
extern "C" int cFEInput_getKeyReady(void *input) {
    return (*(int *)((char*)(input) + (0xC)));
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
extern "C" int cFEInput_getVKey(void *input) {
    return (*(int *)((char*)(input) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEInput_getAsciiKey);
#ifdef SKIP_ASM
/**
 * Returns the character code of the last typed key: +0x14 (On Tour miAsciiKey).
 *
 * @param input  the input (this)
 * @return the character code
 */
extern "C" int cFEInput_getAsciiKey(void *input) {
    return (*(int *)((char*)(input) + (0x14)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD578);
#ifdef SKIP_ASM
/**
 * Returns the word at +0x18 of an object; for a cFEInput that is the input device pointer func_001DD508 clears.
 *
 * It follows cFEInput's out-of-line getters (cFEInput_getKeyReady, getVKey, getAsciiKey).
 *
 * @param arg0  the object
 * @return the word at +0x18
 */
extern "C" int func_001DD578(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x18)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD580);
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
extern "C" unsigned short *func_001DD580(unsigned short *attr) {
    *attr = (*attr | 1) & 0xFFFD;
    return attr;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD598);
#ifdef SKIP_ASM
extern "C" void cUIScreen__cUIScreen();

/**
 * Destroys an on-screen keyboard (vtable D_0046D1A0): chains straight to cUIScreen__cUIScreen.
 *
 * The object and the g++ delete flags pass through untouched in the argument registers.
 */
extern "C" void func_001DD598(void) {
    cUIScreen__cUIScreen();
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD5B8);
#ifdef SKIP_ASM
extern "C" void cList_cList(void *);
extern "C" int cUIThing_cUIThing(void *, int, int);
extern int D_0046D1A0[];
extern int cUIScreen_vtbl[];
extern int cUIGroup_vtbl[];

/**
 * Constructs an on-screen keyboard's cUIScreen part with no owning state.
 *
 * Links the list node to itself (+0x0, +0x4), zeroes the buffer and header pointers (+0xC, +0x10), stores
 * cUIScreen_vtbl, builds the attribute word (func_001DD580, +0x14) and the thread list (cList_cList, +0x18), zeroes the
 * keyframe, object-block and object-list pointers (+0x34..+0x3C), builds the root group (cUIThing_cUIThing at +0x40,
 * cUIGroup_vtbl at +0x48, its cList at +0xB4), zeroes the state pointer at +0xD0 and finally stores the keyboard vtable
 * D_0046D1A0. Unlike func_00180508 it sets none of the keyboard's own fields.
 *
 * @return @p self
 */
extern "C" void *func_001DD5B8(char *self) {
    *(char **)(self + 4) = self;
    *(char **)(self + 0) = self;
    *(int *)(self + 0xC) = 0;
    *(int *)(self + 0x10) = 0;
    *(int **)(self + 8) = cUIScreen_vtbl;
    func_001DD580((unsigned short*)(self + 0x14));
    cList_cList(self + 0x18);
    *(int *)(self + 0x34) = 0;
    *(int *)(self + 0x38) = 0;
    *(int *)(self + 0x3C) = 0;
    cUIThing_cUIThing(self + 0x40, 0, 0);
    *(int **)(self + 0x48) = cUIGroup_vtbl;
    cList_cList(self + 0xB4);
    *(int *)(self + 0xD0) = 0;
    *(int **)(self + 8) = D_0046D1A0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD648);
#ifdef SKIP_ASM
/**
 * Returns the on-screen keyboard's input text buffer at +0xEC.
 *
 * @param screen  the keyboard (vtable D_0046D1A0)
 * @return @p screen + 0xEC
 */
extern "C" int func_001DD648(int screen) {
    return screen + 236;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD650);
#ifdef SKIP_ASM
/**
 * Returns the on-screen keyboard's current key, the word at +0x170 (func_00180508 starts it at 10).
 *
 * @param screen  the keyboard
 * @return the word at +0x170
 */
extern "C" int func_001DD650(void *screen) {
    return (*(int *)((char*)(screen) + (0x170)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD658);
#ifdef SKIP_ASM
/**
 * Sets the on-screen keyboard's word at +0xD8 (func_00180508 starts it at 0).
 *
 * @param screen  the keyboard
 * @param arg1  the new value
 */
extern "C" void func_001DD658(void *screen, int arg1) {
    (*(int *)((char*)(screen) + (0xD8))) = arg1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD660);
#ifdef SKIP_ASM
/**
 * Sets the on-screen keyboard's word at +0xDC (func_00180508 starts it at 0).
 *
 * @param screen  the keyboard
 * @param arg1  the new value
 */
extern "C" void func_001DD660(void *screen, int arg1) {
    (*(int *)((char*)(screen) + (0xDC))) = arg1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD668);
#ifdef SKIP_ASM
/**
 * Sets the on-screen keyboard's word at +0xE0 (func_00180508 starts it at 0).
 *
 * @param screen  the keyboard
 * @param arg1  the new value
 */
extern "C" void func_001DD668(void *screen, int arg1) {
    (*(int *)((char*)(screen) + (0xE0))) = arg1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD670);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's text cursor one place right, unless that would reach the length limit.
 *
 * Increments the cursor position at +0x16C only while the result stays below the limit at +0xD4 (func_00180508 sets
 * 45).
 *
 * @param screen  the keyboard
 */
extern "C" void func_001DD670(void *screen) {
    int temp_3;

    temp_3 = (*(int *)((char*)(screen) + (0x16C))) + 1;
    if (temp_3 < (*(int *)((char*)(screen) + (0xD4)))) {
        (*(int *)((char*)(screen) + (0x16C))) = temp_3;
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD690);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's text cursor one place left, unless it is already at 0.
 *
 * Decrements the cursor position at +0x16C when it is above 0.
 *
 * @param screen  the keyboard
 */
extern "C" void func_001DD690(void *screen) {
    int temp_2;

    temp_2 = (*(int *)((char*)(screen) + (0x16C)));
    if (temp_2 > 0) {
        (*(int *)((char*)(screen) + (0x16C))) = (int) (temp_2 - 1);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD6A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_D6A8(void*, int) __asm__("func_0039E2A0");
extern void* D_0046D0D0[];

/**
 * Constructs a front-end state with no UI engine: runs cUIState's constructor (func_0039E2A0) with a null engine, then
 * stores cFEState's vtable D_0046D0D0 at +0x8.
 *
 * @return @p self
 */
extern "C" void* func_001DD6A8(void *self) {
    func_0039E2A0_D6A8(self, 0);
    *(void***)((char*)self + 8) = D_0046D0D0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD6E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_D6E0(void*) __asm__("func_0039E2A0");
extern void* D_0046D0D0[];

/**
 * Constructs a front-end state for a UI engine: runs cUIState's one-argument constructor (func_0039E2A0), then stores
 * cFEState's vtable D_0046D0D0 at +0x8.
 *
 * The engine argument passes through untouched in the second argument register.
 *
 * @return @p self
 */
extern "C" void* func_001DD6E0(void *self) {
    func_0039E2A0_D6E0(self);
    *(void***)((char*)self + 8) = D_0046D0D0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD718);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E318_D718(void*) __asm__("func_0039E318");
extern void* D_0046D0D0[];

/**
 * Constructs a front-end state for a UI engine and a parent state: runs cUIState's two-argument constructor
 * (func_0039E318), then stores cFEState's vtable D_0046D0D0 at +0x8.
 *
 * The engine and parent arguments pass through untouched in the argument registers.
 *
 * @return @p self
 */
extern "C" void* func_001DD718(void *self) {
    func_0039E318_D718(self);
    *(void***)((char*)self + 8) = D_0046D0D0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD750);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DD750(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys a front-end state: restores cFEState's vtable D_0046D0D0 at +0x8 and runs cUIState's destructor
 * (func_0039E390).
 *
 * The g++ delete flags pass through in the second argument register. cFEStateLodge, cFEStateLodgeRiderDetail,
 * cFEStateBuyAttrib, cFEStateCareerStats and cFEStateCredits destructors call it as their base destructor.
 */
extern "C" void func_001DD750(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DD750(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD778);
#ifdef SKIP_ASM
extern "C" int func_001A1CD0(int, signed char);

/**
 * Narrows a front-end state's controller filter to the controllers no user claims.
 *
 * Starts from 0xFF and, for each user 0 .. @p n - 1, clears the bits of that user's controller mask, read with
 * func_001A1CD0 from the front-end handler (the first word of the UI engine at +0x10); stores the result in mCtrlBits
 * (+0x15).
 *
 * @param n  number of users whose masks to remove
 */
extern "C" void func_001DD778(char *self, signed char n) {
    int i = 0;
    int mask = 0xFF;
    int realFe = **(int **)(self + 0x10);
    if (n > 0) {
        do {
            mask &= ~func_001A1CD0(realFe, i);
            i++;
        } while (i < n);
    }
    *(char *)(self + 0x15) = mask;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD800);
#ifdef SKIP_ASM
extern "C" void func_001A1CB8(int, signed char, int);

/**
 * Gives one user one controller: sets the user's controller mask to the single bit for @p controllerNum.
 *
 * Calls func_001A1CB8 on the front-end handler (the first word of the UI engine at +0x10) with @p userNum and 1
 * << @p controllerNum.
 *
 * @param feState  the front-end state
 * @param userNum  the user
 * @param controllerNum  the controller port
 */
extern "C" void func_001DD800(void *feState, signed char userNum, signed char controllerNum) {
    func_001A1CB8(*(*(int **)((char*)(feState) + (0x10))), userNum, (1 << controllerNum) & 0xFF);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCharSelect__cFEStateCharSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DD840(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the character select front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateCharSelect__cFEStateCharSelect(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DD840(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCheatCharSelect__cFEStateCheatCharSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DD868(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the cheat character select front-end state: restores cFEState's vtable (D_0046D0D0) and runs
 * cUIState's destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateCheatCharSelect__cFEStateCheatCharSelect(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DD868(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD890);
#ifdef SKIP_ASM
/**
 * Tells whether the cheat character select has finished: returns the word at +0x74 (On Tour's mDone).
 *
 * @param cheatCharSelect  the cheat character select state
 * @return non-zero once a choice has been made
 */
extern "C" int func_001DD890(void *cheatCharSelect) {
    return (*(int *)((char*)(cheatCharSelect) + (0x74)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD898);
#ifdef SKIP_ASM
/**
 * Returns the cheat character select's chosen character, the word at +0x70 (On Tour's mCheatCharID).
 *
 * @param cheatCharSelect  the cheat character select state
 * @return the chosen character id
 */
extern "C" int func_001DD898(void *cheatCharSelect) {
    return (*(int *)((char*)(cheatCharSelect) + (0x70)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateCharSetup__cFEStateCharSetup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DD8A0(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the character setup front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateCharSetup__cFEStateCharSetup(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DD8A0(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateRiderDetail__cFEStateRiderDetail);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DD8C8(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the rider detail front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateRiderDetail__cFEStateRiderDetail(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DD8C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD8F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DD8F0(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the uber-trick front-end state (vtable D_0046CCC0): restores cFEState's vtable D_0046D0D0 and runs
 * cUIState's destructor (func_0039E390).
 *
 * The g++ delete flags pass through in the second argument register. D_0046CCC0 is the vtable func_00183A98 stores in
 * the state allocated as "cFEStateUberTrick".
 */
extern "C" void func_001DD8F0(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DD8F0(self);
}
#endif

/**
 * Does nothing: the uber-trick state's onMenuGainFocus (slot 0x6C of its vtable D_0046CCC0).
 */
extern "C" void func_001DD918(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD920);
#ifdef SKIP_ASM
/**
 * Sets a popup description's type, meType (+0x0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param popupType  the type: -1 entry box, 0 no buttons, 1..5 options
 */
extern "C" void func_001DD920(int *popupInfo, int popupType) {
    *popupInfo = popupType;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD928);
#ifdef SKIP_ASM
/**
 * Returns a popup description's type, meType (+0x0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return the popup type
 */
extern "C" int func_001DD928(int *popupInfo) {
    return *popupInfo;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD930);
#ifdef SKIP_ASM
/**
 * Sets the id a popup reports back to its owner, miMiscData (+0x94).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param miscData  the value to report back
 */
extern "C" void func_001DD930(void *popupInfo, int miscData) {
    (*(int *)((char*)(popupInfo) + (0x94))) = miscData;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD938);
#ifdef SKIP_ASM
/**
 * Returns the id a popup reports back to its owner, miMiscData (+0x94).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return miMiscData
 */
extern "C" int func_001DD938(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0x94)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD940);
#ifdef SKIP_ASM
/**
 * Sets whether the popup lets the state below it keep updating, mbPassUpdate (+0x98).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param passUpdate  non-zero to keep updating the parent state
 */
extern "C" void func_001DD940(void *popupInfo, int passUpdate) {
    (*(int *)((char*)(popupInfo) + (0x98))) = passUpdate;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD948);
#ifdef SKIP_ASM
/**
 * Returns whether the popup lets the state below it keep updating, mbPassUpdate (+0x98).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return mbPassUpdate
 */
extern "C" int func_001DD948(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0x98)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD950);
#ifdef SKIP_ASM
struct V3_D950 { float x, y, z; };
/**
 * Sets a popup description's screen offset, mvOffset (+0x9C), from a vector passed by value.
 *
 * @param offset  the offset (x, y, z)
 */
extern "C" void func_001DD950(void *self, V3_D950 offset) {
    *(V3_D950 *)((char *)self + 0x9C) = offset;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD990);
#ifdef SKIP_ASM
struct V12_D990 { int a; int b; int c; };
/**
 * Returns a popup description's screen offset, mvOffset (+0x9C), by value.
 *
 * Copies the three words into the caller's result buffer.
 *
 * @param dst  the result buffer (the hidden return-value pointer)
 * @param src  the popup description (cPopupInfo)
 * @return @p dst
 */
extern "C" V12_D990* func_001DD990(V12_D990* dst, char* src) {
    *dst = *(V12_D990*)(src + 0x9C);
    return dst;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9B0);
#ifdef SKIP_ASM
/**
 * Sets the option a popup starts on, miDefaultOption (+0xAC).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param defaultOption  the option index (clear() sets -1)
 */
extern "C" void func_001DD9B0(void *popupInfo, int defaultOption) {
    (*(int *)((char*)(popupInfo) + (0xAC))) = defaultOption;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9B8);
#ifdef SKIP_ASM
/**
 * Returns the option a popup starts on, miDefaultOption (+0xAC).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return miDefaultOption
 */
extern "C" int func_001DD9B8(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0xAC)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9C0);
#ifdef SKIP_ASM
/**
 * Sets whether the popup reacts to the back command, mbReceivePrev (+0xA8).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param receivePrev  non-zero to receive the back command (clear() sets 1)
 */
extern "C" void func_001DD9C0(void *popupInfo, int receivePrev) {
    (*(int *)((char*)(popupInfo) + (0xA8))) = receivePrev;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9C8);
#ifdef SKIP_ASM
/**
 * Returns whether the popup reacts to the back command, mbReceivePrev (+0xA8).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return mbReceivePrev
 */
extern "C" int func_001DD9C8(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0xA8)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9D0);
#ifdef SKIP_ASM
/**
 * Sets a popup's draw-layer offset, miLayerOffset (+0x90).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param layerOffset  the layer offset
 */
extern "C" void func_001DD9D0(void *popupInfo, int layerOffset) {
    (*(int *)((char*)(popupInfo) + (0x90))) = layerOffset;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9D8);
#ifdef SKIP_ASM
/**
 * Returns a popup's draw-layer offset, miLayerOffset (+0x90).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return miLayerOffset
 */
extern "C" int func_001DD9D8(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0x90)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9E0);
#ifdef SKIP_ASM
/**
 * Sets a popup's background style, meBackgroundType (+0xC0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param backgroundType  the background (0 online, 1 blue, 2 black)
 */
extern "C" void func_001DD9E0(void *popupInfo, int backgroundType) {
    (*(int *)((char*)(popupInfo) + (0xC0))) = backgroundType;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9E8);
#ifdef SKIP_ASM
/**
 * Returns a popup's background style, meBackgroundType (+0xC0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return meBackgroundType
 */
extern "C" int func_001DD9E8(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0xC0)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9F0);
#ifdef SKIP_ASM
/**
 * Sets the most characters a popup's entry box takes, miMaxChars (+0xC4).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param maxChars  the character limit
 */
extern "C" void func_001DD9F0(void *popupInfo, int maxChars) {
    (*(int *)((char*)(popupInfo) + (0xC4))) = maxChars;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DD9F8);
#ifdef SKIP_ASM
/**
 * Returns the most characters a popup's entry box takes, miMaxChars (+0xC4).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return miMaxChars
 */
extern "C" int func_001DD9F8(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0xC4)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA00);
#ifdef SKIP_ASM
/**
 * Sets a popup's minimum background size, mfMinSize (+0xB0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param fparg0  the minimum size
 */
extern "C" void func_001DDA00(void *popupInfo, float fparg0) {
    (*(float *)((char*)(popupInfo) + (0xB0))) = fparg0;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA08);
#ifdef SKIP_ASM
/**
 * Returns a popup's minimum background size, mfMinSize (+0xB0).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return mfMinSize
 */
extern "C" float func_001DDA08(void *popupInfo) {
    return (*(float *)((char*)(popupInfo) + (0xB0)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA10);
#ifdef SKIP_ASM
/**
 * Sets whether a popup centres its options, mbCenterOptions (+0xB8).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param centerOptions  non-zero to centre the options
 */
extern "C" void func_001DDA10(void *popupInfo, int centerOptions) {
    (*(int *)((char*)(popupInfo) + (0xB8))) = centerOptions;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA18);
#ifdef SKIP_ASM
/**
 * Returns whether a popup centres its options, mbCenterOptions (+0xB8).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return mbCenterOptions
 */
extern "C" int func_001DDA18(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0xB8)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA20);
#ifdef SKIP_ASM
/**
 * Sets whether a popup centres its text, mbCenterText (+0xBC).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param centerText  non-zero to centre the text (clear() sets 1)
 */
extern "C" void func_001DDA20(void *popupInfo, int centerText) {
    (*(int *)((char*)(popupInfo) + (0xBC))) = centerText;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA28);
#ifdef SKIP_ASM
/**
 * Returns whether a popup centres its text, mbCenterText (+0xBC).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return mbCenterText
 */
extern "C" int func_001DDA28(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0xBC)));
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
extern "C" void func_001DDA30(void *popupInfo, int message) {
    (*(int *)((char*)(popupInfo) + (8))) = message;
    (*(int *)((char*)(popupInfo) + (4))) = 2;
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
extern "C" void func_001DDA40(void *popupInfo, int message) {
    (*(int *)((char*)(popupInfo) + (8))) = message;
    (*(int *)((char*)(popupInfo) + (4))) = 1;
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
extern "C" void func_001DDA50(void *popupInfo, int messageId) {
    (*(int *)((char*)(popupInfo) + (8))) = messageId;
    (*(int *)((char*)(popupInfo) + (4))) = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA60);
#ifdef SKIP_ASM
struct V8_DA60 { int a; int b; };
/**
 * Copies a whole popup element (type and value, 8 bytes) into a popup's message, mtMessage (+0x4).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDA60(V8_DA60* dst, V8_DA60* src) {
    *(V8_DA60*)((char*)dst + 4) = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDA78);
#ifdef SKIP_ASM
/**
 * Returns a popup's message element, mtMessage (+0x4).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 4
 */
extern "C" int func_001DDA78(int popupInfo) {
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
extern "C" void func_001DDA80(void *popupInfo, int title) {
    (*(int *)((char*)(popupInfo) + (0x10))) = title;
    (*(int *)((char*)(popupInfo) + (0xC))) = 2;
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
extern "C" void func_001DDA90(void *popupInfo, int title) {
    (*(int *)((char*)(popupInfo) + (0x10))) = title;
    (*(int *)((char*)(popupInfo) + (0xC))) = 1;
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
extern "C" void func_001DDAA0(void *popupInfo, int titleId) {
    (*(int *)((char*)(popupInfo) + (0x10))) = titleId;
    (*(int *)((char*)(popupInfo) + (0xC))) = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDAB0);
#ifdef SKIP_ASM
struct V8_DDAB0 { int a; int b; };
/**
 * Copies a whole popup element (type and value) into a popup's title, mtTitle (+0xC).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDAB0(V8_DDAB0* dst, V8_DDAB0* src) {
    *(V8_DDAB0*)((char*)dst + 0xC) = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDAC8);
#ifdef SKIP_ASM
/**
 * Returns a popup's title element, mtTitle (+0xC).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0xC
 */
extern "C" int func_001DDAC8(int popupInfo) {
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
extern "C" void func_001DDAD0(void *popupInfo, int entryBoxTitle) {
    (*(int *)((char*)(popupInfo) + (0x18))) = entryBoxTitle;
    (*(int *)((char*)(popupInfo) + (0x14))) = 2;
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
extern "C" void func_001DDAE0(void *popupInfo, int entryBoxTitle) {
    (*(int *)((char*)(popupInfo) + (0x18))) = entryBoxTitle;
    (*(int *)((char*)(popupInfo) + (0x14))) = 1;
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
extern "C" void func_001DDAF0(void *popupInfo, int entryBoxTitleId) {
    (*(int *)((char*)(popupInfo) + (0x18))) = entryBoxTitleId;
    (*(int *)((char*)(popupInfo) + (0x14))) = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB00);
#ifdef SKIP_ASM
struct V8_DDB00 { int a; int b; };
/**
 * Copies a whole popup element (type and value) into a popup's entry-box title, mtEntryBoxTitle (+0x14).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDB00(V8_DDB00* dst, V8_DDB00* src) {
    *(V8_DDB00*)((char*)dst + 0x14) = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB18);
#ifdef SKIP_ASM
/**
 * Returns a popup's entry-box title element, mtEntryBoxTitle (+0x14).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0x14
 */
extern "C" int func_001DDB18(int popupInfo) {
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
extern "C" void func_001DDB20(void *popupInfo, int entryBoxInitial) {
    (*(int *)((char*)(popupInfo) + (0x20))) = entryBoxInitial;
    (*(int *)((char*)(popupInfo) + (0x1C))) = 2;
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
extern "C" void func_001DDB30(void *popupInfo, int entryBoxInitial) {
    (*(int *)((char*)(popupInfo) + (0x20))) = entryBoxInitial;
    (*(int *)((char*)(popupInfo) + (0x1C))) = 1;
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
extern "C" void func_001DDB40(void *popupInfo, int entryBoxInitialId) {
    (*(int *)((char*)(popupInfo) + (0x20))) = entryBoxInitialId;
    (*(int *)((char*)(popupInfo) + (0x1C))) = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB50);
#ifdef SKIP_ASM
struct V8_DDB50 { int a; int b; };
/**
 * Copies a whole popup element (type and value) into the text a popup's entry box starts with, mtEntryBoxInitial
 * (+0x1C).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDB50(V8_DDB50* dst, V8_DDB50* src) {
    *(V8_DDB50*)((char*)dst + 0x1C) = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDB68);
#ifdef SKIP_ASM
/**
 * Returns the element holding the text a popup's entry box starts with, mtEntryBoxInitial (+0x1C).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0x1C
 */
extern "C" int func_001DDB68(int popupInfo) {
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
extern "C" void func_001DDB70(void *popupInfo, int postPending) {
    (*(int *)((char*)(popupInfo) + (0x50))) = postPending;
    (*(int *)((char*)(popupInfo) + (0x4C))) = 2;
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
extern "C" void func_001DDB80(void *popupInfo, int postPending) {
    (*(int *)((char*)(popupInfo) + (0x50))) = postPending;
    (*(int *)((char*)(popupInfo) + (0x4C))) = 1;
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
extern "C" void func_001DDB90(void *popupInfo, int postPendingId) {
    (*(int *)((char*)(popupInfo) + (0x50))) = postPendingId;
    (*(int *)((char*)(popupInfo) + (0x4C))) = 3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDBA0);
#ifdef SKIP_ASM
struct V8_DDBA0 { int a; int b; };
/**
 * Copies a whole popup element (type and value) into a popup's "post pending" text, mtPostPending (+0x4C).
 *
 * @param dst  the popup description (cPopupInfo)
 * @param src  the element to copy (tPopupElement)
 */
extern "C" void func_001DDBA0(V8_DDBA0* dst, V8_DDBA0* src) {
    *(V8_DDBA0*)((char*)dst + 0x4C) = *src;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDBB8);
#ifdef SKIP_ASM
/**
 * Returns a popup's "post pending" text element, mtPostPending (+0x4C).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return @p popupInfo + 0x4C
 */
extern "C" int func_001DDBB8(int popupInfo) {
    return popupInfo + 76;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDBC0);
#ifdef SKIP_ASM
/**
 * Sets a popup description's mbDeferEventsFlag (+0xB4); the online states set it to 1.
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @param deferEvents  the new flag value
 */
extern "C" void func_001DDBC0(void *popupInfo, int deferEvents) {
    (*(int *)((char*)(popupInfo) + (0xB4))) = deferEvents;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDBC8);
#ifdef SKIP_ASM
/**
 * Returns a popup description's mbDeferEventsFlag (+0xB4).
 *
 * @param popupInfo  the popup description (cPopupInfo)
 * @return mbDeferEventsFlag
 */
extern "C" int func_001DDBC8(void *popupInfo) {
    return (*(int *)((char*)(popupInfo) + (0xB4)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDBD0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E318_DBD0(void *, int, int) __asm__("func_0039E318");
extern "C" void cPopupInfo_cPopupInfo_DBD0(void *) __asm__("cPopupInfo_cPopupInfo");
extern int cScreenPopup_vtbl[];
extern int cBXString_EmptyString;

struct S_DBD0 {
    int pad0[2];
    int *vt;
    char pad1[0x2A4];
    int v2B0;
    int v2B4;
};
/**
 * Constructs a screen popup with no engine and no parent (the default constructor).
 *
 * Runs cUIState's two-argument constructor (func_0039E318) with null engine and parent, stores cScreenPopup_vtbl at
 * +0x8, constructs the popup description at +0xBC (cPopupInfo_cPopupInfo) and sets the input and static text strings
 * (+0x2B0, +0x2B4) to the empty cBXString.
 *
 * @return @p self
 */
extern "C" S_DBD0 *func_001DDBD0(S_DBD0 *self) {
    func_0039E318_DBD0(self, 0, 0);
    self->vt = cScreenPopup_vtbl;
    cPopupInfo_cPopupInfo_DBD0((char *)self + 0xBC);
    self->v2B0 = cBXString_EmptyString;
    self->v2B4 = cBXString_EmptyString;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDC20);
#ifdef SKIP_ASM
/**
 * Returns a screen popup's description, the cPopupInfo at +0xBC.
 *
 * @param screenPopup  the popup (cScreenPopup)
 * @return @p screenPopup + 0xBC
 */
extern "C" int func_001DDC20(int screenPopup) {
    return screenPopup + 188;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDC28);
#ifdef SKIP_ASM
/**
 * Sets whether a screen popup lets the state below it keep updating, mbPassUpdate (+0x2C0).
 *
 * @param screenPopup  the popup (cScreenPopup)
 * @param passUpdate  non-zero to keep updating the parent state
 */
extern "C" void func_001DDC28(void *screenPopup, int passUpdate) {
    (*(int *)((char*)(screenPopup) + (0x2C0))) = passUpdate;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDC30);
#ifdef SKIP_ASM
/**
 * Returns whether a screen popup lets the state below it keep updating, mbPassUpdate (+0x2C0).
 *
 * @param screenPopup  the popup (cScreenPopup)
 * @return mbPassUpdate
 */
extern "C" int func_001DDC30(void *screenPopup) {
    return (*(int *)((char*)(screenPopup) + (0x2C0)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDC38);
#ifdef SKIP_ASM
/**
 * Returns the text typed into a screen popup's entry box, the string mszInputText (+0x2B0).
 *
 * @param screenPopup  the popup (cScreenPopup)
 * @return the input text
 */
extern "C" int func_001DDC38(void *screenPopup) {
    return (*(int *)((char*)(screenPopup) + (0x2B0)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDC40);
#ifdef SKIP_ASM
/**
 * Tells whether a screen popup is on screen: returns mbShowing (+0x2B8), which onCreateScreen sets and onDestroyScreen
 * clears.
 *
 * @param screenPopup  the popup (cScreenPopup)
 * @return mbShowing
 */
extern "C" int func_001DDC40(void *screenPopup) {
    return (*(int *)((char*)(screenPopup) + (0x2B8)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDC48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E318_DC48(void *, int, int) __asm__("func_0039E318");
extern "C" void cBuyPopupInfo_clear_DC48(void *) __asm__("cBuyPopupInfo_clear");
extern int cUIStateBuyPopup_vtbl[];

/**
 * Constructs a buy popup with no engine and no parent (the default constructor).
 *
 * Runs cUIState's two-argument constructor (func_0039E318) with null engine and parent, stores cUIStateBuyPopup_vtbl
 * and clears the purchase description at +0x48 with cBuyPopupInfo_clear.
 *
 * @return @p self
 */
extern "C" void *func_001DDC48(cUIStateBuyPopup* self) {
    func_0039E318_DC48(self, 0, 0);
    self->mVtbl = (void**)cUIStateBuyPopup_vtbl;
    cBuyPopupInfo_clear_DC48((char *)self + 0x48);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cUIStateBuyPopup__cUIStateBuyPopup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DDC90(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the buy popup front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cUIStateBuyPopup__cUIStateBuyPopup(cUIStateBuyPopup* self) {
    self->mVtbl = D_0046D0D0;
    func_0039E390_DDC90(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDCB8);
#ifdef SKIP_ASM
/**
 * Returns a buy popup's purchase description, the cBuyPopupInfo at +0x48.
 *
 * @param buyPopup  the popup (cUIStateBuyPopup)
 * @return @p buyPopup + 0x48
 */
extern "C" int func_001DDCB8(int buyPopup) {
    return buyPopup + 72;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDCC0);
#ifdef SKIP_ASM
/**
 * Tells whether the player accepted the purchase: returns mbAccept.
 *
 * @param uiStateBuyPopup  the popup
 * @return mbAccept
 */
extern "C" int func_001DDCC0(cUIStateBuyPopup* uiStateBuyPopup) {
    return (uiStateBuyPopup->mbAccept);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDCC8);
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
extern "C" void func_001DDCC8(int buyPopup) {
    cBuyPopupInfo_initBuySongByCredit(buyPopup + 72);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDCE8);
#ifdef SKIP_ASM
extern "C" void cBuyPopupInfo_initBuySong(int);

/**
 * Sets the buy popup up to buy a song: calls cBuyPopupInfo_initBuySong on the description at +0x48.
 *
 * The song arguments pass through untouched in the remaining argument registers.
 *
 * @param buyPopup  the popup (cUIStateBuyPopup)
 */
extern "C" void func_001DDCE8(int buyPopup) {
    cBuyPopupInfo_initBuySong(buyPopup + 72);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD08);
#ifdef SKIP_ASM
extern "C" void func_001CAA18(int);

/**
 * Sets the buy popup up to buy a reward item: calls func_001CAA18 on the description at +0x48.
 *
 * The item arguments pass through untouched in the remaining argument registers (func_001CAA18 fills the description
 * with the item name and the "kT_CMNItemColon" / "kT_OVRCMNBuyItem" texts).
 *
 * @param buyPopup  the popup (cUIStateBuyPopup)
 */
extern "C" void func_001DDD08(int buyPopup) {
    func_001CAA18(buyPopup + 72);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD28);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether Save was pressed on the current memory-card screen: returns the word at +0x1DC of
 * cFEMemCard_mInstance.
 *
 * func_001D5488 installs it as the memory-card manager's callbackIsSavePressed (+0x64).
 *
 * @return the save-pressed word
 */
extern "C" int func_001DDD28(void) {
    return (*(int *)((char*)(cFEMemCard_mInstance) + (0x1DC)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD38);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether Load was pressed on the current memory-card screen: returns the word at +0x1F4 of
 * cFEMemCard_mInstance.
 *
 * func_001D5488 installs it as the memory-card manager's callbackIsLoadPressed (+0x78).
 *
 * @return the load-pressed word
 */
extern "C" int func_001DDD38(void) {
    return (*(int *)((char*)(cFEMemCard_mInstance) + (0x1F4)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD48);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether Delete was pressed on the current memory-card screen: returns the word at +0x1E0 of
 * cFEMemCard_mInstance.
 *
 * func_001D5488 installs it as the memory-card manager's callbackIsDeletePressed (+0x68).
 *
 * @return the delete-pressed word
 */
extern "C" int func_001DDD48(void) {
    return (*(int *)((char*)(cFEMemCard_mInstance) + (0x1E0)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD58);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether Cancel was pressed on the current memory-card screen, and consumes it.
 *
 * Returns the word at +0x1E4 of cFEMemCard_mInstance and clears it. func_001D5488 installs it as the memory-card
 * manager's callbackIsCancelPressed (+0x6C).
 *
 * @return the cancel-pressed word before it was cleared
 */
extern "C" int func_001DDD58(void) {
    int temp_2;

    temp_2 = (*(int *)((char*)(cFEMemCard_mInstance) + (0x1E4)));
    (*(int *)((char*)(cFEMemCard_mInstance) + (0x1E4))) = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD68);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether Overwrite was confirmed on the current memory-card screen: returns the word at +0x1F0 of
 * cFEMemCard_mInstance.
 *
 * func_001D5488 installs it as the memory-card manager's callbackIsOverwritePressed (+0x70).
 *
 * @return the overwrite-pressed word
 */
extern "C" int func_001DDD68(void) {
    return (*(int *)((char*)(cFEMemCard_mInstance) + (0x1F0)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD78);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether a memory-card device is selected: returns the word at +0x1A8 of cFEMemCard_mInstance.
 *
 * func_001D5488 installs it as the memory-card manager's callbackIsDeviceSelected (+0x74). The same word is the "busy"
 * flag that makes cFEMemCard_onInputBegin ignore input.
 *
 * @return the device-selected word
 */
extern "C" int func_001DDD78(void) {
    return (*(int *)((char*)(cFEMemCard_mInstance) + (0x1A8)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD88);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether the next memory card was requested, and consumes it.
 *
 * Returns the word at +0x1F8 of cFEMemCard_mInstance and clears it; cFEMemCard_onInputBegin sets the word when right is
 * pressed. func_001D5488 installs it as the memory-card manager's callbackIsNextDevicePressed (+0x7C).
 *
 * @return the next-device word before it was cleared
 */
extern "C" int func_001DDD88(void) {
    int temp_2;

    temp_2 = (*(int *)((char*)(cFEMemCard_mInstance) + (0x1F8)));
    (*(int *)((char*)(cFEMemCard_mInstance) + (0x1F8))) = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDD98);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether the previous memory card was requested, and consumes it.
 *
 * Returns the word at +0x1FC of cFEMemCard_mInstance and clears it; cFEMemCard_onInputBegin sets the word when left is
 * pressed. func_001D5488 installs it as the memory-card manager's callbackIsPrevDevicePressed (+0x80).
 *
 * @return the previous-device word before it was cleared
 */
extern "C" int func_001DDD98(void) {
    int temp_2;

    temp_2 = (*(int *)((char*)(cFEMemCard_mInstance) + (0x1FC)));
    (*(int *)((char*)(cFEMemCard_mInstance) + (0x1FC))) = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDDA8);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether Format was confirmed on the current memory-card screen: returns the word at +0x1E8 of
 * cFEMemCard_mInstance.
 *
 * func_001D5488 installs it as the memory-card manager's callbackIsFormatPressed (+0x88).
 *
 * @return the format-pressed word
 */
extern "C" int func_001DDDA8(void) {
    return (*(int *)((char*)(cFEMemCard_mInstance) + (0x1E8)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDDB8);
#ifdef SKIP_ASM
extern void *cFEMemCard_mInstance;

/**
 * Reports whether Retry was pressed on the current memory-card screen, and consumes it.
 *
 * Returns the word at +0x1EC of cFEMemCard_mInstance and clears it. func_001D5488 installs it as the memory-card
 * manager's callbackIsRetryPressed (+0x84).
 *
 * @return the retry-pressed word before it was cleared
 */
extern "C" int func_001DDDB8(void) {
    int temp_2;

    temp_2 = (*(int *)((char*)(cFEMemCard_mInstance) + (0x1EC)));
    (*(int *)((char*)(cFEMemCard_mInstance) + (0x1EC))) = 0;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDDC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A2028_DDC8 __asm__("cFEMemCard_mInstance");

/**
 * Tells whether a memory-card screen exists: returns cFEMemCard_mInstance != 0.
 *
 * @return 1 when cFEMemCard_mInstance is set, otherwise 0
 */
extern "C" int func_001DDDC8(void) {
    return D_004A2028_DDC8 != 0;
}
#endif

/**
 * Does nothing: the memory-card screen base's onCreateScreen (slot +0x24 of its vtable D_0046C980).
 */
extern "C" void func_001DDDD8(void) {
}

/**
 * Does nothing: the memory-card screens' onMenuGainFocus (slot +0x6C).
 *
 * It sits in D_0046C980 (the cFEMemCard_cFEMemCard vtable) and is inherited unchanged by cFEStateMemCard,
 * cFEStateProfileLoad, cFEStateOptionsDeviceSelect, cOVState_AUTOSAVE, cOVState_PROFILE and two more vtables
 * (D_004722B8, D_00472440).
 */
extern "C" void func_001DDDE0(void) {
}

/**
 * Does nothing: the memory-card screen base's onWidgetEvent (slot +0x9C of D_0046C980).
 */
extern "C" void func_001DDDE8(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0xD4 of D_0046C980, which cOVState_AUTOSAVE fills with
 * cOVState_AUTOSAVE_displayStatus.
 */
extern "C" void func_001DDDF0(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x10C of D_0046C980, which cFEStateProfileLoad overrides with
 * func_0018FAF0 and cFEStateOptionsDeviceSelect with func_0018E478.
 */
extern "C" void func_001DDDF8(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x12C of D_0046C980.
 *
 * cFEStateProfileLoad (func_0018EF70), cFEStateOptionsDeviceSelect (func_0018E178), cOVState_PROFILE and
 * cFEStateMemCard override it with functions taking one flag.
 */
extern "C" void func_001DDE00(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x134 of D_0046C980 (kept by cFEStateMemCard and
 * cOVState_AUTOSAVE).
 *
 * cFEStateProfileLoad (func_0018F0B8) and cFEStateOptionsDeviceSelect (func_0018E2C0) override it with functions taking
 * one flag.
 */
extern "C" void func_001DDE08(void) {
}

/**
 * Does nothing: the memory-card screen base's slot +0x13C of D_0046C980 (kept by cFEStateMemCard and
 * cOVState_AUTOSAVE).
 *
 * cFEStateProfileLoad (func_0018F168) and cFEStateOptionsDeviceSelect (func_0018E368) override it with functions taking
 * a load flag and a save flag.
 */
extern "C" void func_001DDE10(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDE18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_DE18(void *, int) __asm__("func_0039E2A0");
extern int D_0046C980[];
extern int D_00474E08[];
extern unsigned short D_004A1390[];

/**
 * Constructs a memory-card screen base with no engine (the default constructor).
 *
 * Runs cUIState's constructor (func_0039E2A0) with a null engine, stores the cOVState vtable D_00474E08 and then
 * D_0046C980 at +0x8, clears +0x48, +0x4C, +0x54 and +0x98 and copies the empty string D_004A1390 into +0x58. Unlike
 * the constructor that takes an engine and a type, it does not run func_001D5488, so cFEMemCard_mInstance is left
 * alone.
 *
 * @return @p self
 */
extern "C" void *func_001DDE18(void *self) {
    func_0039E2A0_DE18(self, 0);
    *(int *)((char *)self + 0x98) = 0;
    *(int *)((char *)self + 0x48) = 0;
    *(int **)((char *)self + 8) = D_00474E08;
    *(int *)((char *)self + 0x4C) = 0;
    *(int *)((char *)self + 0x54) = 0;
    unsigned short t = D_004A1390[0];
    *(int **)((char *)self + 8) = D_0046C980;
    *(unsigned short *)((char *)self + 0x58) = t;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDE78);
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
extern "C" void *func_001DDE78(void *self) {
    cScreenPopup_cScreenPopup_DE78(self, 0, 0);
    *(int **)((char *)self + 8) = cFEPopupConfirm_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupConfirm__cFEPopupConfirm);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cScreenPopup__cScreenPopup_DEB8(void*) __asm__("cScreenPopup__cScreenPopup");
extern void* D_0046C890_DEB8[] __asm__("cFEPopupConfirm_vtbl");

/**
 * Destroys a confirm popup: restores the cFEPopupConfirm vtable and runs the cScreenPopup destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for
 * cScreenPopup__cScreenPopup.
 */
extern "C" void cFEPopupConfirm__cFEPopupConfirm(void *self) {
    *(void***)((char*)self + 8) = D_0046C890_DEB8;
    cScreenPopup__cScreenPopup_DEB8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDEE0);
#ifdef SKIP_ASM
extern int cFEPopupConfirm_mUsePreFE;

/**
 * Sets the static cFEPopupConfirm_mUsePreFE flag.
 *
 * @param usePreFe  the new flag value
 */
extern "C" void func_001DDEE0(int usePreFe) {
    cFEPopupConfirm_mUsePreFE = usePreFe;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDEE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_DEE8(void*, int) __asm__("func_0039E2A0");
extern void* cFEStateOptions_vtbl[];

/**
 * Constructs the options menu state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine,
 * then stores cFEStateOptions_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void* func_001DDEE8(void *self) {
    func_0039E2A0_DEE8(self, 0);
    *(void***)((char*)self + 8) = cFEStateOptions_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptions__cFEStateOptions);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DDF20(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the options front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateOptions__cFEStateOptions(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DDF20(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDF48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_DF48(void*, int) __asm__("func_0039E2A0");
extern void* cFEStateOptionsGame_vtbl[];

/**
 * Constructs the game options state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine,
 * then stores cFEStateOptionsGame_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void* func_001DDF48(void *self) {
    func_0039E2A0_DF48(self, 0);
    *(void***)((char*)self + 8) = cFEStateOptionsGame_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsGame__cFEStateOptionsGame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DDF80(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the game options front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsGame__cFEStateOptionsGame(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DDF80(self);
}
#endif

/**
 * Does nothing: the game options state's onLoseFocus (slot +0x5C of cFEStateOptionsGame_vtbl).
 */
extern "C" void func_001DDFA8(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DDFB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_DFB0(void*, int) __asm__("func_0039E2A0");
extern void* cFEStateOptionsSound_vtbl[];

/**
 * Constructs the sound options state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine,
 * then stores cFEStateOptionsSound_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void* func_001DDFB0(void *self) {
    func_0039E2A0_DFB0(self, 0);
    *(void***)((char*)self + 8) = cFEStateOptionsSound_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsSound__cFEStateOptionsSound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DDFE8(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the sound options front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsSound__cFEStateOptionsSound(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DDFE8(self);
}
#endif

/**
 * Does nothing: the sound options state's onLoseFocus (slot +0x5C of cFEStateOptionsSound_vtbl).
 */
extern "C" void func_001DE010(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE018);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_E018(void*, int) __asm__("func_0039E2A0");
extern void* D_0046C550[];

/**
 * Constructs the HUD options state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine, then
 * stores its vtable D_0046C550 at +0x8.
 *
 * D_0046C550 is the vtable func_0018C0E8 stores in the state fereal.cpp allocates as "cFEStateOptionsHUD".
 *
 * @return @p self
 */
extern "C" void* func_001DE018(void *self) {
    func_0039E2A0_E018(self, 0);
    *(void***)((char*)self + 8) = D_0046C550;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE050);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DE050(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the HUD options state (vtable D_0046C550): restores cFEState's vtable D_0046D0D0 at +0x8 and runs cUIState's
 * destructor (func_0039E390).
 *
 * The g++ delete flags pass through in the second argument register.
 */
extern "C" void func_001DE050(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DE050(self);
}
#endif

/**
 * Does nothing: the HUD options state's onLoseFocus (slot +0x5C of D_0046C550).
 */
extern "C" void func_001DE078(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE080);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E080(void *, int) __asm__("func_0039E2A0");
extern int cFEStateOptionsController_vtbl[];

/**
 * Constructs the controller options state with no engine.
 *
 * Runs cUIState's constructor (func_0039E2A0) with a null engine, stores cFEStateOptionsController_vtbl at +0x8 and
 * clears the byte at +0x50 (On Tour's mVibrateOn).
 *
 * @return @p self
 */
extern "C" void *func_001DE080(void *self) {
    func_0039E2A0_E080(self, 0);
    *(int **)((char *)self + 8) = cFEStateOptionsController_vtbl;
    *((char *)self + 0x50) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsController__cFEStateOptionsController);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DE0C0(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the controller options front-end state: restores cFEState's vtable (D_0046D0D0) and runs
 * cUIState's destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsController__cFEStateOptionsController(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DE0C0(self);
}
#endif

/**
 * Does nothing: the controller options state's onLoseFocus (slot +0x5C of cFEStateOptionsController_vtbl).
 */
extern "C" void func_001DE0E8(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE0F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E0F0(void *, int) __asm__("func_0039E2A0");
extern int cFEStateOptionsSaveLoad_vtbl[];

/**
 * Constructs the save/load options state with no engine: runs cUIState's constructor (func_0039E2A0) with a null
 * engine, then stores cFEStateOptionsSaveLoad_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE0F0(void *self) {
    func_0039E2A0_E0F0(self, 0);
    *(int **)((char *)self + 8) = cFEStateOptionsSaveLoad_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsSaveLoad__cFEStateOptionsSaveLoad);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_DE128(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the save/load options front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 */
extern "C" void cFEStateOptionsSaveLoad__cFEStateOptionsSaveLoad(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390_DE128(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateOptionsDeviceSelect__cFEStateOptionsDeviceSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_001D5428_E150(void*) __asm__("func_001D5428");
extern void* cFEStateOptionsDeviceSelect_vtbl[];

/**
 * Destroys the options device-select state: stores its vtable at +0x8, then runs the base destructor.
 *
 * The base, func_001D5428(), clears the global cFEMemCard_mInstance and runs the cUIState destructor (func_0039E390). The
 * g++ delete flags in the second argument register pass through untouched.
 *
 * @return no meaningful value (func_001D5428 returns nothing)
 */
extern "C" void* cFEStateOptionsDeviceSelect__cFEStateOptionsDeviceSelect(void *self) {
    *(void***)((char*)self + 8) = cFEStateOptionsDeviceSelect_vtbl;
    return func_001D5428_E150(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE178);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEMemCard_cFEMemCard_E178(void *, int, int) __asm__("cFEMemCard_cFEMemCard");
extern int D_0046C228_E178[] __asm__("cFEStateOptionsDeviceSelect_vtbl");

/**
 * Constructs the memory-card device select state with no engine: runs cFEMemCard_cFEMemCard with null engine and type
 * 0, then stores cFEStateOptionsDeviceSelect_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE178(void *self) {
    cFEMemCard_cFEMemCard_E178(self, 0, 0);
    *(int **)((char *)self + 8) = D_0046C228_E178;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE1B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E1B8(void *, int) __asm__("func_0039E2A0");
extern int cFEStateProfileSelect_vtbl[];

/**
 * Constructs the profile select state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine,
 * then stores cFEStateProfileSelect_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE1B8(void *self) {
    func_0039E2A0_E1B8(self, 0);
    *(int **)((char *)self + 8) = cFEStateProfileSelect_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateProfileSelect__cFEStateProfileSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E1F0(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the profile select front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever func_0039E390 leaves)
 */
extern "C" void* cFEStateProfileSelect__cFEStateProfileSelect(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    return func_0039E390_E1F0(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateProfileLoad__cFEStateProfileLoad);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_001D5428_E218(void*) __asm__("func_001D5428");
extern void* cFEStateProfileLoad_vtbl[];

/**
 * Destroys the profile-load state: stores its vtable at +0x8, then runs the base destructor.
 *
 * The base, func_001D5428(), clears the global cFEMemCard_mInstance and runs the cUIState destructor (func_0039E390). The
 * g++ delete flags in the second argument register pass through untouched.
 *
 * @return no meaningful value (func_001D5428 returns nothing)
 */
extern "C" void* cFEStateProfileLoad__cFEStateProfileLoad(void *self) {
    *(void***)((char*)self + 8) = cFEStateProfileLoad_vtbl;
    return func_001D5428_E218(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE240);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEMemCard_cFEMemCard_E240(void *, int, int) __asm__("cFEMemCard_cFEMemCard");
extern int D_0046BFD0_E240[] __asm__("cFEStateProfileLoad_vtbl");

/**
 * Constructs the profile load/save state with no engine: runs cFEMemCard_cFEMemCard with null engine and type 0, then
 * stores cFEStateProfileLoad_vtbl at +0x8.
 *
 * Unlike cFEStateProfileLoad_cFEStateProfileLoad it sets none of the state's own fields.
 *
 * @return @p self
 */
extern "C" void *func_001DE240(void *self) {
    cFEMemCard_cFEMemCard_E240(self, 0, 0);
    *(int **)((char *)self + 8) = D_0046BFD0_E240;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE280);
#ifdef SKIP_ASM
/**
 * Sets what a profile load/save state does when it finishes, the last word of the object (+0x27C).
 *
 * func_0018FAF0 (the state's slot +0x10C) reads it: 0 pushes the state the parent's factory returns, 1 starts a
 * pre-game load (cPreGameLoadScreen), 2 returns to the title screen. The constructor clears it; cFEStateLodge's callers
 * set 1 or 2 directly.
 *
 * @param arg0  the profile load state (cFEStateProfileLoad)
 * @param arg1  the action: 0, 1 or 2
 */
extern "C" void func_001DE280(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x27C))) = arg1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE288);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E318_E288(void *, int, int) __asm__("func_0039E318");
extern int cKeyboardPopup_vtbl[];
extern int cBXString_EmptyString;

struct S_E288 {
    int pad0[2];
    int *vt;
    char pad1[0x54];
    int v60;
    char pad2[0x3C0];
    int v424;
};
/**
 * Constructs an on-screen keyboard popup with no engine and no parent (the default constructor).
 *
 * Runs cUIState's two-argument constructor (func_0039E318) with null engine and parent, stores cKeyboardPopup_vtbl,
 * sets miMaxCharacters (+0x60) to 63 and mszStatic (+0x424) to the empty cBXString.
 *
 * @return @p self
 */
extern "C" S_E288 *func_001DE288(S_E288 *self) {
    func_0039E318_E288(self, 0, 0);
    self->vt = cKeyboardPopup_vtbl;
    self->v60 = 0x3F;
    self->v424 = cBXString_EmptyString;
    return self;
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
INCLUDE_ASM("main/all_in_one_main", func_001DE2E0);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's edited text, mszEditString (+0x74).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return @p keyboard + 0x74
 */
extern "C" int func_001DE2E0(int keyboard) {
    return keyboard + 116;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE2E8);
#ifdef SKIP_ASM
extern "C" void strlen(int);

/**
 * Measures the keyboard popup's edited text: tail-calls strlen on mszEditString (+0x74), leaving its result as the
 * return value.
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 */
extern "C" void func_001DE2E8(int keyboard) {
    strlen(keyboard + 116);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE308);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's selected key, miCurrentKey (+0x140; 0..83).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return miCurrentKey
 */
extern "C" int func_001DE308(void *keyboard) {
    return (*(int *)((char*)(keyboard) + (0x140)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE310);
#ifdef SKIP_ASM
/**
 * Sets the keyboard popup's selected key, miCurrentKey (+0x140).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param currentKey  the key index (0..83)
 */
extern "C" void func_001DE310(void *keyboard, int currentKey) {
    (*(int *)((char*)(keyboard) + (0x140))) = currentKey;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE318);
#ifdef SKIP_ASM
/**
 * Sets the keyboard popup's layout, meType (+0x43C): 0 regular, 1 compact, 2 expanded.
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param keyboardType  the layout
 */
extern "C" void func_001DE318(void *keyboard, int keyboardType) {
    (*(int *)((char*)(keyboard) + (0x43C))) = keyboardType;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE320);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's layout, meType (+0x43C).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return meType
 */
extern "C" int func_001DE320(void *keyboard) {
    return (*(int *)((char*)(keyboard) + (0x43C)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE328);
#ifdef SKIP_ASM
/**
 * Returns whether accepting the text pops the keyboard popup, mbPopOnEnd (+0x420).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return mbPopOnEnd
 */
extern "C" int func_001DE328(void *keyboard) {
    return (*(int *)((char*)(keyboard) + (0x420)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE330);
#ifdef SKIP_ASM
/**
 * Sets whether accepting the text pops the keyboard popup, mbPopOnEnd (+0x420).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param popOnEnd  non-zero to pop on accept
 */
extern "C" void func_001DE330(void *keyboard, int popOnEnd) {
    (*(int *)((char*)(keyboard) + (0x420))) = popOnEnd;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE338);
#ifdef SKIP_ASM
/**
 * Sets whether the keyboard popup masks its text, mbPassword (+0x70).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param password  non-zero to show '*' characters
 */
extern "C" void func_001DE338(void *keyboard, int password) {
    (*(int *)((char*)(keyboard) + (0x70))) = password;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE340);
#ifdef SKIP_ASM
/**
 * Sets whether the first key press clears the text, mbClearOnFirst (+0x54).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param clearOnFirst  non-zero to clear on the first key
 */
extern "C" void func_001DE340(void *keyboard, int clearOnFirst) {
    (*(int *)((char*)(keyboard) + (0x54))) = clearOnFirst;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE348);
#ifdef SKIP_ASM
/**
 * Sets whether the space key stays disabled while the text is empty, mbDisableForFirst (+0x58).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param disableForFirst  non-zero to disable space for the first character
 */
extern "C" void func_001DE348(void *keyboard, int disableForFirst) {
    (*(int *)((char*)(keyboard) + (0x58))) = disableForFirst;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE350);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE350(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables the space key (key 75) in both key sets: cKeyboardPopup_setIncludeKey(keyboard, 75, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeSpace  non-zero to enable the key
 */
extern "C" void func_001DE350(int keyboard, int includeSpace) {
    cKeyboardPopup_setIncludeKey_DE350(keyboard, 75, includeSpace, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE378);
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
extern "C" void func_001DE378(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE378(keyboard, 0x1C, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE3A0);
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
extern "C" void func_001DE3A0(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE3A0(keyboard, 69, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE3C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE3C8(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables key 79 (in the bottom row) in both key sets: cKeyboardPopup_setIncludeKey(keyboard, 79, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeKey  non-zero to enable the key
 */
extern "C" void func_001DE3C8(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE3C8(keyboard, 79, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE3F0);
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
extern "C" void func_001DE3F0(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE3F0(keyboard, 41, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE418);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cKeyboardPopup_setIncludeKey_DE418(int, int, int, int) __asm__("cKeyboardPopup_setIncludeKey");

/**
 * Enables or disables key 66 in both key sets: cKeyboardPopup_setIncludeKey(keyboard, 66, flag, 2).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param includeKey  non-zero to enable the key
 */
extern "C" void func_001DE418(int keyboard, int includeKey) {
    cKeyboardPopup_setIncludeKey_DE418(keyboard, 66, includeKey, 2);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE440);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's minimum text length, miMinCharacters (+0x64).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return miMinCharacters
 */
extern "C" int func_001DE440(void *keyboard) {
    return (*(int *)((char*)(keyboard) + (0x64)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE448);
#ifdef SKIP_ASM
/**
 * Returns the keyboard popup's maximum text length, miMaxCharacters (+0x60).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return miMaxCharacters
 */
extern "C" int func_001DE448(void *keyboard) {
    return (*(int *)((char*)(keyboard) + (0x60)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE450);
#ifdef SKIP_ASM
/**
 * Sets whether the keyboard popup forwards its events to the parent state, mbReferEventsToParent (+0x438).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param referEvents  non-zero to forward events (cKeyboardPopup_handleEvent)
 */
extern "C" void func_001DE450(void *keyboard, int referEvents) {
    (*(int *)((char*)(keyboard) + (0x438))) = referEvents;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE458);
#ifdef SKIP_ASM
/**
 * Returns whether the keyboard popup forwards its events to the parent state, mbReferEventsToParent (+0x438).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return mbReferEventsToParent
 */
extern "C" int func_001DE458(void *keyboard) {
    return (*(int *)((char*)(keyboard) + (0x438)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE460);
#ifdef SKIP_ASM
/**
 * Sets whether the keyboard popup runs its parent's onUpdate each frame, mbCallParentsUpdate (+0x440).
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @param callParentUpdate  non-zero to update the parent too (cKeyboardPopup_onUpdate)
 */
extern "C" void func_001DE460(void *keyboard, int callParentUpdate) {
    (*(int *)((char*)(keyboard) + (0x440))) = callParentUpdate;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE468);
#ifdef SKIP_ASM
/**
 * Tells whether the player accepted the keyboard popup's text: returns mbAcceptText (+0x5C), which the accept key sets.
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 * @return mbAcceptText
 */
extern "C" int func_001DE468(void *keyboard) {
    return (*(int *)((char*)(keyboard) + (0x5C)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE470);
#ifdef SKIP_ASM
/**
 * Moves the keyboard popup's text cursor one place right, while it stays below miMaxCharacters - 1.
 *
 * Increments miCursorPos (+0x134) only when it is below miMaxCharacters (+0x60) minus one.
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 */
extern "C" void func_001DE470(void *keyboard) {
    int temp_3;

    temp_3 = (*(int *)((char*)(keyboard) + (0x134)));
    if (temp_3 < ((*(int *)((char*)(keyboard) + (0x60))) - 1)) {
        (*(int *)((char*)(keyboard) + (0x134))) = (int) (temp_3 + 1);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE498);
#ifdef SKIP_ASM
/**
 * Moves the keyboard popup's text cursor one place left, unless it is already at 0.
 *
 * Decrements miCursorPos (+0x134) when it is above 0.
 *
 * @param keyboard  the keyboard popup (cKeyboardPopup)
 */
extern "C" void func_001DE498(void *keyboard) {
    int temp_2;

    temp_2 = (*(int *)((char*)(keyboard) + (0x134)));
    if (temp_2 > 0) {
        (*(int *)((char*)(keyboard) + (0x134))) = (int) (temp_2 - 1);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE4B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E4B0(void *, int) __asm__("func_0039E2A0");
extern int cFEStateCredits_vtbl[];

/**
 * Constructs the credits state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine, then
 * stores cFEStateCredits_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE4B0(void *self) {
    func_0039E2A0_E4B0(self, 0);
    *(int **)((char *)self + 8) = cFEStateCredits_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE4E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E4E8(void *, int) __asm__("func_0039E2A0");
extern "C" void func_002006B8_E4E8(void *) __asm__("func_002006B8");
extern int cFEStateEventSelect_vtbl[];

/**
 * Constructs the event select state with no engine.
 *
 * Runs cUIState's constructor (func_0039E2A0) with a null engine, stores cFEStateEventSelect_vtbl at +0x8 and
 * constructs the member at +0x48 with func_002006B8 (On Tour keeps its cUITemplate_MAP mMap there).
 *
 * @return @p self
 */
extern "C" void *func_001DE4E8(void *self) {
    func_0039E2A0_E4E8(self, 0);
    *(int **)((char *)self + 8) = cFEStateEventSelect_vtbl;
    func_002006B8_E4E8((char *)self + 0x48);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateEventSelect__cFEStateEventSelect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E528(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the event select front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever func_0039E390 leaves)
 */
extern "C" void* cFEStateEventSelect__cFEStateEventSelect(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    return func_0039E390_E528(self);
}
#endif

/**
 * Does nothing: the event select state's onMenuGainFocus (slot +0x6C of cFEStateEventSelect_vtbl).
 */
extern "C" void func_001DE550(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE558);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E558(void *, int) __asm__("func_0039E2A0");
extern int D_0046BC58_E558[] __asm__("cFEStateBackground_vtbl");

/**
 * Constructs the front-end background state with no engine (the default constructor): runs cUIState's constructor
 * (func_0039E2A0) with a null engine, then stores cFEStateBackground_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE558(void *self) {
    func_0039E2A0_E558(self, 0);
    *(int **)((char *)self + 8) = D_0046BC58_E558;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE590);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_E590(void*) __asm__("func_0039E2A0");
extern void* cFEStateBackground_vtbl[];

/**
 * Constructs the front-end background state for a UI engine: runs cUIState's constructor (func_0039E2A0) with the
 * engine, then stores cFEStateBackground_vtbl at +0x8.
 *
 * The engine argument passes through untouched in the second argument register.
 *
 * @return @p self
 */
extern "C" void* func_001DE590(void *self) {
    func_0039E2A0_E590(self);
    *(void***)((char*)self + 8) = cFEStateBackground_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateBackground__cFEStateBackground);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E5C8(void*) __asm__("func_0039E390");
extern void* cFEStateBackground_vtbl[];

/**
 * Destroys the front-end background state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (func_0039E390) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (func_0039E390 returns nothing)
 */
extern "C" void* cFEStateBackground__cFEStateBackground(void *self) {
    *(void***)((char*)self + 8) = cFEStateBackground_vtbl;
    return func_0039E390_E5C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE5F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E5F0(void *, int) __asm__("func_0039E2A0");
extern int D_0046BB88_E5F0[] __asm__("cFEStateTransition_vtbl");

/**
 * Constructs the screen transition state with no engine (the default constructor): runs cUIState's constructor
 * (func_0039E2A0) with a null engine, then stores cFEStateTransition_vtbl at +0x8.
 *
 * Unlike func_001DE628 it leaves the two screen pointers (+0x48, +0x4C) unset.
 *
 * @return @p self
 */
extern "C" void *func_001DE5F0(void *self) {
    func_0039E2A0_E5F0(self, 0);
    *(int **)((char *)self + 8) = D_0046BB88_E5F0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE628);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E628(void *) __asm__("func_0039E2A0");
extern int D_0046BB88_E628[] __asm__("cFEStateTransition_vtbl");

/**
 * Constructs the screen transition state for a UI engine.
 *
 * Runs cUIState's constructor (func_0039E2A0) with the engine (passed through in the second argument register), stores
 * cFEStateTransition_vtbl at +0x8 and clears the two screen pointers at +0x48 and +0x4C (On Tour's mpSnow and mpWhite).
 *
 * @return @p self
 */
extern "C" void *func_001DE628(void *self) {
    func_0039E2A0_E628(self);
    *(int **)((char *)self + 8) = D_0046BB88_E628;
    *(int *)((char *)self + 0x48) = 0;
    *(int *)((char *)self + 0x4C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateTransition__cFEStateTransition);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E668(void*) __asm__("func_0039E390");
extern void* cFEStateTransition_vtbl[];

/**
 * Destroys the front-end transition state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (func_0039E390) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (func_0039E390 returns nothing)
 */
extern "C" void* cFEStateTransition__cFEStateTransition(void *self) {
    *(void***)((char*)self + 8) = cFEStateTransition_vtbl;
    return func_0039E390_E668(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE690);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E690(void *, int) __asm__("func_0039E2A0");
extern int cFEStateTitle_vtbl[];

/**
 * Constructs the title screen state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine,
 * then stores cFEStateTitle_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE690(void *self) {
    func_0039E2A0_E690(self, 0);
    *(int **)((char *)self + 8) = cFEStateTitle_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateTitle__cFEStateTitle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E6C8(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the title front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever func_0039E390 leaves)
 */
extern "C" void* cFEStateTitle__cFEStateTitle(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    return func_0039E390_E6C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE6F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E6F0(void *, int) __asm__("func_0039E2A0");
extern int cFEStateMainMenu_vtbl[];

/**
 * Constructs the main menu state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine, then
 * stores cFEStateMainMenu_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE6F0(void *self) {
    func_0039E2A0_E6F0(self, 0);
    *(int **)((char *)self + 8) = cFEStateMainMenu_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateMainMenu__cFEStateMainMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E728(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the main menu front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever func_0039E390 leaves)
 */
extern "C" void* cFEStateMainMenu__cFEStateMainMenu(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    return func_0039E390_E728(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE750);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E750(void *, int) __asm__("func_0039E2A0");
extern int cFEStateBonusMaterial_vtbl[];

/**
 * Constructs the bonus material state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine,
 * then stores cFEStateBonusMaterial_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE750(void *self) {
    func_0039E2A0_E750(self, 0);
    *(int **)((char *)self + 8) = cFEStateBonusMaterial_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateBonusMaterial__cFEStateBonusMaterial);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E788(void*) __asm__("func_0039E390");
extern void* D_0046D0D0[];

/**
 * Destroys the bonus material front-end state: restores cFEState's vtable (D_0046D0D0) and runs cUIState's
 * destructor.
 *
 * The g++ destructor flags are not declared here; they stay in the second argument register for func_0039E390
 * (bit 0 frees).
 *
 * @return nothing meaningful (whatever func_0039E390 leaves)
 */
extern "C" void* cFEStateBonusMaterial__cFEStateBonusMaterial(void *self) {
    *(void***)((char*)self + 8) = D_0046D0D0;
    return func_0039E390_E788(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE7B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E318_E7B0(void* self, int a1, int a2) __asm__("func_0039E318");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern void* cFEStateNet_vtbl[];
extern char D_004A1390_E7B0[] __asm__("D_004A1390");
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
 * Runs cUIState's two-argument constructor (func_0039E318) with null engine and parent, clears unk_048, unk_058,
 * mpKeyboard, mpKeyboardText, unk_6AC, unk_6B0 and unk_6B8, sets unk_6BC to 2, stores the cFEStateNet vtable
 * cFEStateNet_vtbl, builds the string at +0x6C0 from the empty string D_004A1390 (cBXString_cBXString2) and allocates the
 * pending-message list's sentinel node (cMemMan_alloc(0xC, "StlBlock")), linking it to itself.
 *
 * @return @p self
 */
// Front-end state constructor: clears the state fields, sets the vtable, the name string and an empty list.
extern "C" void* func_001DE7B0(cFEStateNet* self)
{
    // An unused 16-byte local is needed to reproduce the 0x40-byte frame.
    int unused[4];
    func_0039E318_E7B0(self, 0, 0);
    sList_E7B0* l = (sList_E7B0*)(((char*)self) + 0x6C8);
    self->unk_048 = 0;
    self->unk_058 = 0;
    self->mpKeyboard = 0;
    self->mpKeyboardText = 0;
    self->unk_6AC = 0;
    self->unk_6B0 = 0;
    self->unk_6B8 = 0;
    self->unk_6BC = 2;
    self->mVtbl = cFEStateNet_vtbl;
    cBXString_cBXString2(((char*)self) + 0x6C0, D_004A1390_E7B0);
    self->mMsgList.mpNode = (void*)((sListNode_E7B0*)cMemMan_alloc(0xC, str_StlBlock, 0x20000000, 0));
    l->head->next = (sListNode_E7B0*)self->mMsgList.mpNode;
    l->head->prev = (sListNode_E7B0*)self->mMsgList.mpNode;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE860);
#ifdef SKIP_ASM
/**
 * Returns an online state's unk_058 (+0x58) as its onInputBegin result (slot +0x44 of the online states' vtables).
 *
 * func_001A9668 and func_001A9710 set the word to 1 before pushing the login state.
 *
 * @param feStateNet  the online state (cFEStateNet)
 * @return unk_058
 */
extern "C" int func_001DE860(cFEStateNet* feStateNet) {
    return (feStateNet->unk_058);
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
INCLUDE_ASM("main/all_in_one_main", func_001DE878);
#ifdef SKIP_ASM
extern int D_004A1A70;

/**
 * Returns the front end's path, the global D_004A1A70.
 *
 * @return D_004A1A70
 */
extern "C" int func_001DE878(void) {
    return D_004A1A70;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE880);
#ifdef SKIP_ASM
extern int D_004A1A70;

/**
 * Tells whether the front end is on the online path: returns D_004A1A70 == 1.
 *
 * @return 1 when D_004A1A70 is 1, otherwise 0
 */
extern "C" int func_001DE880(void) {
    return D_004A1A70 == 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE890);
#ifdef SKIP_ASM
/**
 * Sets an online state's unk_6B0 (+0x6B0) to 1.
 *
 * func_001BF5A8 and func_001C0138 set the same word; no read of it is known.
 *
 * @param feStateNet  the online state (cFEStateNet)
 */
extern "C" void func_001DE890(cFEStateNet* feStateNet) {
    (feStateNet->unk_6B0) = 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE8A0);
#ifdef SKIP_ASM
/**
 * Sets an online state's unk_6C4 (+0x6C4); func_001ABB30 opens the status popup only when it and mpPopup are set.
 *
 * @param feStateNet  the online state (cFEStateNet)
 * @param arg1  the new value
 */
extern "C" void func_001DE8A0(cFEStateNet* feStateNet, int arg1) {
    (feStateNet->unk_6C4) = arg1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE8A8);
#ifdef SKIP_ASM
/**
 * Tells whether an online state handles net events: its screen (+0x40) exists and unk_048 (+0x48) is set.
 *
 * Slot +0xF4 of the cFEStateNet vtable cFEStateNet_vtbl and of most online states' vtables; cFEStateNet_onGainFocus calls it,
 * and sets unk_048 to 1.
 *
 * @param netState  the online state (cFEStateNet)
 * @return 1 when both are set, otherwise 0
 */
extern "C" int func_001DE8A8(void *netState) {
    int var_3;

    var_3 = 0;
    if ((*(int *)((char*)(netState) + (0x40))) != 0) {
        var_3 = (*(int *)((char*)(netState) + (0x48))) != 0;
    }
    return var_3;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE8C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEStateNet_cFEStateNet_E8C8(void *, int, int) __asm__("cFEStateNet_cFEStateNet");
extern int D_0046B6F8_E8C8[] __asm__("cFEBlankPopup_vtbl");

/**
 * Constructs a blank online popup with no engine and no parent (the default constructor): runs cFEStateNet_cFEStateNet
 * with null engine and parent, then stores cFEBlankPopup_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE8C8(void *self) {
    cFEStateNet_cFEStateNet_E8C8(self, 0, 0);
    *(int **)((char *)self + 8) = D_0046B6F8_E8C8;
    return self;
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

extern "C" void* cFEBlankPopup__cFEBlankPopup(void *self) {
    *(void***)((char*)self + 8) = cFEBlankPopup_vtbl;
    return cFEStateNet__cFEStateNet_E918(self);
}
#endif

/**
 * Does nothing: the memory-card state's onWidgetEvent (slot +0x9C of cFEStateMemCard_vtbl).
 */
extern "C" void func_001DE940(void) {
}

/**
 * Does nothing: slot +0x12C of cFEStateMemCard_vtbl, the slot func_001DDE00 fills in the memory-card base vtable
 * D_0046C980.
 */
extern "C" void func_001DE948(void) {
}

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE950);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cFEMemCard_cFEMemCard_E950(void *, int, int) __asm__("cFEMemCard_cFEMemCard");
extern int cFEStateMemCard_vtbl[];

/**
 * Constructs the memory-card state with no engine: runs cFEMemCard_cFEMemCard with null engine and type 0, then stores
 * cFEStateMemCard_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE950(void *self) {
    cFEMemCard_cFEMemCard_E950(self, 0, 0);
    *(int **)((char *)self + 8) = cFEStateMemCard_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE990);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *func_0039E2A0_E990(void *, int) __asm__("func_0039E2A0");
extern int D_0046B4A0_E990[] __asm__("cFEStateLegal_vtbl");

/**
 * Constructs the legal screen state with no engine: runs cUIState's constructor (func_0039E2A0) with a null engine,
 * then stores cFEStateLegal_vtbl at +0x8.
 *
 * @return @p self
 */
extern "C" void *func_001DE990(void *self) {
    func_0039E2A0_E990(self, 0);
    *(int **)((char *)self + 8) = D_0046B4A0_E990;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateLegal__cFEStateLegal);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E9C8(void*) __asm__("func_0039E390");
extern void* cFEStateLegal_vtbl[];

/**
 * Destroys the legal-screen state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (func_0039E390) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (func_0039E390 returns nothing)
 */
extern "C" void* cFEStateLegal__cFEStateLegal(void *self) {
    *(void***)((char*)self + 8) = cFEStateLegal_vtbl;
    return func_0039E390_E9C8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEStateLegal_onGainTransition);
#ifdef SKIP_ASM
/**
 * Ends the legal screen's gain transition at once.
 *
 * Declared without parameters. The state stack update func_0039ECB0() ends a state's gain transition once this
 * returns nonzero.
 *
 * @return always 1 (transition finished)
 */
extern "C" int cFEStateLegal_onGainTransition(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DE9F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_E9F8(void*) __asm__("func_0039E390");
extern void* D_0046B3D0[];

/**
 * Destroys the options-reset popup (vtable D_0046B3D0): stores D_0046B3D0 at +0x8 and runs cUIState's destructor
 * (func_0039E390).
 *
 * D_0046B3D0 is the vtable festateoptions.cpp stores in the popup it allocates as "cFEPopupOptionReset". The g++ delete
 * flags pass through in the second argument register.
 *
 * @return the result of func_0039E390
 */
extern "C" void* func_001DE9F8(void *self) {
    *(void***)((char*)self + 8) = D_0046B3D0;
    return func_0039E390_E9F8(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DEA20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int GetHashValue32_EA20(char *) __asm__("GetHashValue32__FPc");
extern "C" int cUIEngine_addScreenByHashName_EA20(int, void *, int, int) __asm__("cUIEngine_addScreenByHashName");
extern "C" void cUIScreen_playFrame_EA20(int, int, int) __asm__("cUIScreen_playFrame");
extern char str_111resetopts[];

/**
 * Opens the options-reset popup's screen: looks up the screen "111resetopts" and starts it from frame 0.
 *
 * Stores GetHashValue32("111resetopts") in mHashName (+0xC), adds the screen with cUIEngine_addScreenByHashName on the
 * engine (+0x10), keeps it at +0x40 and, when it exists, calls cUIScreen_playFrame(screen, 0, 0).
 */
extern "C" void func_001DEA20(void *self) {
    int hashName = GetHashValue32_EA20(str_111resetopts);
    *(int *)((char *)self + 0xC) = hashName;
    int screen = cUIEngine_addScreenByHashName_EA20(*(int *)((char *)self + 0x10), self, hashName, 0);
    *(int *)((char *)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame_EA20(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DEA80);
#ifdef SKIP_ASM
/**
 * Filters the options-reset popup's menu: returns 0x100 for @p key 7, 8 or 9 and 0x101 for any other value.
 *
 * It fills the onWidgetMenu slot (+0xA4) of D_0046B3D0.
 *
 * @param menu  the menu (unused)
 * @param key  the menu event
 * @return 0x100 or 0x101
 */
// Key filter: keys 7, 8 and 9 return 0x100, every other key 0x101.
extern "C" int func_001DEA80(void* self, int menu, unsigned int key)
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
INCLUDE_ASM("main/all_in_one_main", func_001DEAA8);
#ifdef SKIP_ASM
int GetHashValue32(char*);
extern "C" void func_0039F190(int, int);
extern char str_yes[];
extern char str_no[];

/**
 * Handles a button press on the options-reset popup: "yes" accepts, "no" declines, and either closes the popup.
 *
 * On event 5 from a widget, compares the widget's hash (+0x38) with GetHashValue32("yes") and ("no"); sets the accept
 * byte (+0x48) to 1 or 0 and pops the state stack with func_0039F190(engine + 0x18, 1). Other events and other widgets
 * are ignored.
 *
 * @param widget  the widget (may be null)
 * @param event  the widget event
 */
extern "C" void func_001DEAA8(char *self, char *widget, int event) {
    if (widget != 0) {
        if (event == 5) {
            int yesCheckHash = *(int*)(widget + 0x38);
            if (yesCheckHash == GetHashValue32(str_yes)) {
                *(char*)(self + 0x48) = 1;
                func_0039F190(*(int*)(self + 0x10) + 0x18, 1);
            } else {
                int noCheckHash = *(int*)(widget + 0x38);
                if (noCheckHash == GetHashValue32(str_no)) {
                    *(char*)(self + 0x48) = 0;
                    func_0039F190(*(int*)(self + 0x10) + 0x18, 1);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", func_001DEB50);
#ifdef SKIP_ASM
/**
 * Returns the options-reset popup's answer, the signed byte at +0x48 (1 yes, 0 no).
 *
 * @param resetPopup  the popup
 * @return the accept byte
 */
extern "C" signed char func_001DEB50(void *resetPopup) {
    return (*(signed char *)((char*)(resetPopup) + (0x48)));
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupVideoCalibration__cFEPopupVideoCalibration);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_EB58(void*) __asm__("func_0039E390");
extern void* cFEPopupVideoCalibration_vtbl[];

/**
 * Destroys the video calibration popup: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (func_0039E390) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (func_0039E390 returns nothing)
 */
extern "C" void* cFEPopupVideoCalibration__cFEPopupVideoCalibration(void *self) {
    *(void***)((char*)self + 8) = cFEPopupVideoCalibration_vtbl;
    return func_0039E390_EB58(self);
}
#endif

//100%
INCLUDE_ASM("main/all_in_one_main", cFEPopupVideoCalibration_onGainTransition);
#ifdef SKIP_ASM
/**
 * Ends the video-calibration popup's gain transition at once.
 *
 * Declared without parameters. The state stack update func_0039ECB0() ends a state's gain transition once this
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
 * Declared without parameters. The state stack update func_0039ECB0() ends a state's lose transition once this
 * returns nonzero.
 *
 * @return always 1 (transition finished)
 */
extern "C" int cFEPopupVideoCalibration_onLoseTransition(void) {
    return 1;
}
#endif
