#include "common.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int USTR_length(unsigned short* s);
void cMemMan_free(void* p);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, int n);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
// getDollarString formats a money amount; the unit declares its parameter as void*.
extern "C" const char* getDollarString_amount(int amount) __asm__("getDollarString");
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00460470[];
extern char D_00460488[];
extern char D_00460588[];
extern char D_00460598[];
extern char D_004605A8[];
extern char D_004605B8[];
extern char D_004605C8[];
extern char D_004605D8[];
extern char D_004605F0[];
extern char D_00460600[];
extern char D_00460610[];
extern char D_00460620[];
extern char D_00460630[];
extern char D_00460640[];
extern char D_0045DB90[];
extern char D_004A1408[];
extern char D_004A18B8[];
extern char D_004A18C0[];
extern char D_004A18C8[];
extern char D_004A18D0[];
extern char* D_004A18A0;
extern char* D_004A18A4;
extern char* D_004411A0[];
extern char* D_004411C0[];
extern char* D_00441200[];

struct sVEi_6F98 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEs_6F98 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

struct sWidget_6F98 {
    char pad0[0x8];
    sVEi_6F98* vt;      // 0x8
    char padC[0x18 - 0xC];
    int index;          // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;           // 0x38
};

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sReqLine_6F98 {
    char pad0[0x158];
    ulong done;                  // 0x158
    ulong sel;                   // 0x160
    int count;                   // 0x168
    int f16C;                    // 0x16C
    sWidget_6F98* w170;          // 0x170
    sWidget_6F98* slotA[8];      // 0x174
    sWidget_6F98* slotB[8];      // 0x194
    sWidget_6F98* title;         // 0x1B4
    sWidget_6F98* artist;        // 0x1B8
    sWidget_6F98* opts[6];       // 0x1BC
    sWidget_6F98* w1D4;          // 0x1D4
    sWidget_6F98* w1D8;          // 0x1D8
    sWidget_6F98* w1DC;          // 0x1DC
    sWidget_6F98* w1E0;          // 0x1E0
    sWidget_6F98* w1E4;          // 0x1E4
    sWidget_6F98* text;          // 0x1E8
    sWidget_6F98* w1EC;          // 0x1EC
    sWidget_6F98* w1F0;          // 0x1F0
    sWidget_6F98* w1F4;          // 0x1F4
    sWidget_6F98* w1F8;          // 0x1F8
    sWidget_6F98* w1FC;          // 0x1FC
    sWidget_6F98* w200;          // 0x200
};

static inline void show_6F98(sWidget_6F98* w, int on)
{
    w->vt[9].fn((char*)w + w->vt[9].delta, on);
}

static inline int countBits_6F98(ulong v)
{
    int n = 0;
    for (int i = 0; i < 64; i++) {
        n += (v >> i) & 1;
    }
    return n;
}

/**
 * Binds the request-line screen's named widgets to their fields and sets their first state.
 *
 * Keeps the song/artist texts, help text, arrows, menu, buttons, the 8 song rows ("songs%d") and check boxes
 * ("%dbox", misc data = row) and the 6 "%dfree" markers. The playlist counter gets kT_OVRCMNSongsInList with
 * the playlist size; the cost/cash widgets show only in the store (+0x16C), the cash (+0x168) as dollars.
 *
 * @param w  the widget being created; matched by its hash name
 */
extern "C" void cFEStateRequestLine_onWidgetCreate(sReqLine_6F98* self, sWidget_6F98* w)
{
    int hash = w->hash;
    if (hash == GetHashValue32(D_00460588)) {
        self->title = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
    } else if (hash == GetHashValue32(D_00460598)) {
        self->artist = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
    } else if (hash == GetHashValue32(D_00460488)) {
        self->text = w;
    } else if (hash == GetHashValue32(D_004A18B8)) {
        self->w1EC = w;
    } else if (hash == GetHashValue32(D_004605A8)) {
        self->w1F0 = w;
    } else if (hash == GetHashValue32(D_004605B8)) {
        show_6F98(w, 0);
    } else if (hash == GetHashValue32(D_004605C8)) {
        char* db = (char*)SSXApp->FELocale;
        sVEs_6F98* vt = *(sVEs_6F98**)(db + 4);
        unsigned short* fmt = vt[4].fn(db + vt[4].delta, GetHashValue32(D_004605D8));
        int len = USTR_length(fmt) + 5;
        unsigned short* buf = (unsigned short*)operator_new_tag(len * 2, D_00460470, 0x100, 0);
        sprintf1(buf, fmt, countBits_6F98(self->done));
        self->w1E4 = w;
        cUIText_setUnicodeString(w, buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    } else if (hash == GetHashValue32(D_004A18C0)) {
        show_6F98(w, 0);
    } else if (hash == GetHashValue32(D_004A18C8)) {
        self->w1D4 = w;
        show_6F98(w, 0);
    } else if (hash == GetHashValue32(D_004A18D0)) {
        if (self->f16C == 0) {
            show_6F98(w, 0);
        }
        self->w1DC = w;
    } else if (hash == GetHashValue32(D_004605F0)) {
        self->w1D8 = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
        if (self->f16C == 0) {
            show_6F98(w, 0);
        }
    } else if (hash == GetHashValue32(D_00460600)) {
        if (self->f16C == 0) {
            show_6F98(w, 0);
        }
    } else if (hash == GetHashValue32(D_00460610)) {
        self->w1E0 = w;
        cUIText_setAsciiString((cUIText*)self->w1E0, getDollarString_amount(self->count));
        if (self->f16C == 0) {
            show_6F98(w, 0);
        }
    } else if (hash == GetHashValue32(D_00460620)) {
        if (self->f16C == 0 || countBits_6F98(self->sel) >= 6) {
            show_6F98(w, 0);
        }
    } else if (hash == GetHashValue32(D_0045DB90)) {
        self->w170 = w;
    } else if (hash == GetHashValue32(D_00460630)) {
        self->w1F4 = w;
        show_6F98(w, 0);
    } else if (hash == GetHashValue32(D_00460640)) {
        self->w1FC = w;
        show_6F98(w, 1);
    } else if (hash == GetHashValue32(D_004A18A0)) {
        self->w1F8 = w;
        show_6F98(self->w1F4, 0);
    } else if (hash == GetHashValue32(D_004A18A4)) {
        self->w200 = w;
        show_6F98(w, 1);
    } else {
        int i;
        for (i = 0; i < 8; i++) {
            if (hash == GetHashValue32(D_004411A0[i])) {
                self->slotA[i] = w;
                show_6F98(w, 0);
                break;
            }
            if (hash == GetHashValue32(D_004411C0[i])) {
                self->slotB[i] = w;
                show_6F98(w, 0);
                break;
            }
        }
        if (i < 8) {
            w->index = i;
            return;
        }
        for (i = 0; i < 6; i++) {
            if (hash == GetHashValue32(D_00441200[i])) {
                self->opts[i] = w;
                if (self->f16C != 0 && 6 - i > countBits_6F98(self->sel)) {
                    show_6F98(w, 1);
                } else {
                    show_6F98(w, 0);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cFEStateRequestLine_updatePlayListCheckMarks(void* self);
extern "C" void cFEStateRequestLine_updatePlayListCounter(void* self);
extern "C" void cFEStateRequestLine_showBuyPopup_7500(void* self, int idx) __asm__("cFEStateRequestLine_showBuyPopup");
extern "C" void cFEStateRequestLine_updateHilightedSongInfo(void* self, int idx);
extern "C" void cFEStateRequestLine_updateHelpText(void* self, int idx);
extern "C" void cFEStateRequestLine_updateButtonsText(void* self, int idx);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00158848(void* iface, int a, int b, unsigned long m);
extern "C" void func_00158820(void* iface, int a, int b, unsigned long m);
extern "C" void* func_0028B180();
extern "C" void func_0028C2D0(void* p, unsigned long m);
extern "C" void func_0039F190(void* list, int a);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateAudioOptions_cFEStateAudioOptions(void* mem, void* engine, int a, int b);
extern "C" void cUIStateStack_push(void* list, void* item);
int func_00198AE8(void* self, int idx) __asm__("func_00198AE8__FPv");
extern const char D_00460650[];

struct sReqLine_7500 {
    char pad_0x0[0x10];
    char* engine;           // 0x10
    char pad_0x14[0x30];
    signed char player;     // 0x44
    char pad_0x45[3];
    int f48;                // 0x48
    int list[64];           // 0x4C
    int count;              // 0x14C
    int top;                // 0x150
    char pad_0x154[0x4];
    unsigned long done;     // 0x158
    unsigned long sel;      // 0x160
    int limit;              // 0x168
    int f16C;               // 0x16C
};

struct sVE_7500 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int isSet_7500(unsigned long m, int i)
{
    return (m >> i) & 1;
}

static inline int onlyBit_7500(unsigned long m, int i)
{
    unsigned long rest = m & ~((unsigned long)1 << i);
    return rest == 0;
}

static inline int countSel_7500(sReqLine_7500* self)
{
    int n = 0;
    for (int i = 0; i < 64; i++) {
        n += (self->sel >> i) & 1;
    }
    return n;
}

/**
 * Handles highlight, select and back on the song request list.
 *
 * Song masks: +0x158 = songs in the playlist, +0x160 = owned songs; +0x168 is the cash, +0x16C the in-store
 * flag.
 *
 * Event 5 on a song: an unowned song opens the buy popup when in the store and affordable or free; an owned
 * song toggles in the playlist unless it is the last one. Event 6 saves both masks (back-end interface 13),
 * hands the playlist to the sound manager and pops (in game) or opens cFEStateAudioOptions. Event 1 updates
 * the song info, help and button texts.
 *
 * @param p  the cFEStateRequestLine (raw pointer)
 * @param item  the widget; its misc data (+0x18) is the row
 * @param msg  UI event code (1, 5 or 6)
 */
// Request-line widget events: 1 hilights a song, 5 toggles it (done/selected masks), 6 saves the masks and leaves.
extern "C" void cFEStateRequestLine_onWidgetEvent(void* p, void* item, unsigned int msg)
{
    sReqLine_7500* self = (sReqLine_7500*)p;
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        int k = self->top + *(int*)((char*)item + 0x18);
        if (k >= self->count) {
            return;
        }
        int idx = self->list[k];
        if (!isSet_7500(self->sel, idx)) {
            if (self->f16C != 0) {
                if (self->limit >= func_00198AE8(self, idx) || countSel_7500(self) < 6) {
                    cFEStateRequestLine_showBuyPopup_7500(self, idx);
                }
            }
        } else {
            unsigned long done = self->done;
            int empty = done == 0;
            if (empty || !onlyBit_7500(done, idx)) {
                if (isSet_7500(done, idx)) {
                    self->done = done & ~((unsigned long)1 << idx);
                } else {
                    self->done = done | ((unsigned long)1 << idx);
                }
                cFEStateRequestLine_updatePlayListCheckMarks(self);
                cFEStateRequestLine_updatePlayListCounter(self);
            }
        }
        cFEStateRequestLine_updateButtonsText(self, idx);
        break;
    }
    case 6: {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xD);
        func_00158848(iface, self->player, self->f48, self->done);
        func_00158820(iface, self->player, self->f48, self->sel);
        func_0028C2D0(func_0028B180(), self->done);
        if (self->f16C != 0) {
            char* pl = (char*)func_0028B180();
            sVE_7500* vt = *(sVE_7500**)(pl + 0x5558);
            char* obj = pl + 0x118;
            vt[3].fn(obj + vt[3].delta, 0xA);
        }
        if ((int)SSXApp->Game != 0) {
            func_0039F190(self->engine + 0x18, 1);
        } else {
            void* nextState = cFEStateAudioOptions_cFEStateAudioOptions(cMemMan_alloc(0x80, D_00460650, 0, 0), self->engine, self->player, self->f16C);
            if (nextState != 0) {
                cUIStateStack_push(self->engine + 0x18, nextState);
            }
        }
        break;
    }
    case 1: {
        int idx = self->list[self->top + *(int*)((char*)item + 0x18)];
        cFEStateRequestLine_updateHilightedSongInfo(self, idx);
        cFEStateRequestLine_updateHelpText(self, idx);
        cFEStateRequestLine_updateButtonsText(self, idx);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_onWidgetMenu);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern "C" void* func_0028B180();

extern "C" void cFEStateRequestLine_updatePlayList_77D0(void* self) __asm__("cFEStateRequestLine_updatePlayList");

extern "C" void func_00197E70_77D0(void* self) __asm__("func_00197E70");
extern "C" void func_0028CF98(void* p, int a, int b, long c, int d);
extern "C" void func_002B4978(void* list, int idx);

/**
 * Handles menu keys on the song request list.
 *
 * Key 6 returns 0x10 (refuse) for the last playlist song, for an unowned song outside the store, and in the
 * store when there is no free credit and too little cash. Key 8 plays an owned song in game (sound manager
 * +0x508, func_0028CF98) or previews it in the front end (func_002B4978); key 4 scrolls the list to the row;
 * key 9 gives 0.
 *
 * @param p  the cFEStateRequestLine (raw pointer)
 * @param item  the menu (unused)
 * @param msg  menu key/event code (4, 6, 8 or 9)
 * @param k  row index in the menu
 * @return 0x10 to refuse, 0 for key 9, else 0x101
 */
extern "C" int cFEStateRequestLine_onWidgetMenu(void* p, void* item, unsigned int msg, int k)
{
    sReqLine_7500* self = (sReqLine_7500*)p;
    switch (msg) {
    case 6: {
        int idx = self->list[self->top + k];
        if (onlyBit_7500(self->done, idx)) {
            if (self->f16C != 0) {
                if (!isSet_7500(self->sel, idx))
                    return 0x101;
            }
            return 0x10;
        }
        if (self->f16C == 0) {
            if (isSet_7500(self->sel, idx))
                return 0x101;
        } else {
            if (self->limit < func_00198AE8(self, idx)) {
                if (countSel_7500(self) >= 6)
                    goto ret10;
            }
            break;
        }
        goto ret10;
    }
    case 9:
        return 0;
    case 8: {
        int n;
        int idx;
        n = self->top + k;
        if (n < self->count) {
            idx = self->list[n];
            if ((int)SSXApp->Game != 0) {
                if (!isSet_7500(self->sel, idx)) {
                ret10:
                    return 0x10;
                }
                *(int*)((char*)func_0028B180() + 0x508) = idx;
                func_0028CF98(func_0028B180(), 0, 0, -1, 1);
                func_00197E70_77D0(self);
            } else {
                func_002B4978((char*)func_0028B180() + 0x118, idx);
            }
        }
        break;
    }
    case 4:
        self->top = k;
        cFEStateRequestLine_updatePlayList_77D0(self);
        break;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_onPopupEvent);
#ifdef SKIP_ASM
extern "C" void func_001988D8(void* self, int idx);
extern "C" void cFEStateRequestLine_updatePlayList(void* self);
extern "C" void cFEStateRequestLine_updateHilightedSongInfo(void* self, int a1);
extern "C" void cFEStateRequestLine_updateCash(void* self);
extern "C" void cFEStateRequestLine_updateFreeSongMarkers(void* self);
extern "C" void cFEStateRequestLine_updateButtonsText(void* self, int a1);
extern "C" void cFEStateRequestLine_updateHelpText(void* self, int a1);

struct sMsgCenter_979D8 {
    char pad_0x0[0x1C];
    int flags;        // 0x1C
    char pad_0x20[0x2C];
    int list[64];     // 0x4C
    int count;        // 0x14C
    int top;          // 0x150
    char pad_0x154[0x1C];
    char* menu;       // 0x170
};

/**
 * Hides the request line while another popup is up and completes song purchases.
 *
 * Event 0x15 clears the visible bit (bit 3 of mAttrState) unless the popup's id (+0xC) is 7 (the buy popup);
 * event 0x16 sets it again and, for a confirmed buy popup (+0x6C), buys the selected song (func_001988D8,
 * probably buySong) and refreshes the list, song info, cash, free markers, buttons and help.
 *
 * @param item  the popup state
 * @param msg  popup event code (0x15 or 0x16)
 */
extern "C" void cFEStateRequestLine_onPopupEvent(sMsgCenter_979D8* self, void* item, int msg)
{
    switch (msg) {
    case 0x15:
        if (*(int*)((char*)item + 0xC) != 7) {
            self->flags &= ~8;
        }
        break;
    case 0x16:
        if (((self->flags >> 3) & 1) == 0) {
            self->flags |= 8;
        }
        if (*(int*)((char*)item + 0xC) == 7 && *(int*)((char*)item + 0x6C) != 0) {
            int song = self->list[self->top + *(int*)(*(char**)(self->menu + 0xA0) + 0x18)];
            func_001988D8(self, song);
            cFEStateRequestLine_updatePlayList(self);
            cFEStateRequestLine_updateHilightedSongInfo(self, song);
            cFEStateRequestLine_updateCash(self);
            cFEStateRequestLine_updateFreeSongMarkers(self);
            cFEStateRequestLine_updateButtonsText(self, song);
            cFEStateRequestLine_updateHelpText(self, song);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197AD8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_97AD8 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    char pad_0x150[0x10];
    ulong mask;     // 0x160
};

extern "C" void func_00197AD8(void* self)
{
    sMsgCenter_97AD8* s = (sMsgCenter_97AD8*)self;
    int i;
    int j;
    int n = 0;
    for (i = 0; i < s->count; i++) {
        if ((int)((s->mask >> i) & 1)) {
            s->list[n++] = i;
        }
    }
    for (j = 0; j < s->count; j++) {
        if (!(int)((s->mask >> j) & 1)) {
            s->list[n++] = j;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updatePlayList);
#ifdef SKIP_ASM
extern "C" void cFEStateRequestLine_updatePlayListSongTitles(void* self);
extern "C" void cFEStateRequestLine_updatePlayListCheckMarks(void* self);
extern "C" void cFEStateRequestLine_updatePlayListArrows(void* self);
extern "C" void cFEStateRequestLine_updatePlayListCounter(void* self);

/**
 * Refreshes the request-line play list: song titles, check marks, arrows and counter, in that order.
 */
extern "C" void cFEStateRequestLine_updatePlayList(void* self)
{
    cFEStateRequestLine_updatePlayListSongTitles(self);
    cFEStateRequestLine_updatePlayListCheckMarks(self);
    cFEStateRequestLine_updatePlayListArrows(self);
    cFEStateRequestLine_updatePlayListCounter(self);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updatePlayListSongTitles);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cFEStateRequestLine_getSongTitle(void* self, int i);

class cMsgWidget_97BC8 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

struct sMsgCenter_97BC8 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    int top;        // 0x150
    char pad_0x154[0x20];
    cMsgWidget_97BC8* texts[8]; // 0x174
};

/**
 * Writes the song titles into the 8 visible playlist rows.
 *
 * Row i shows the title of list[top + i] (getSongTitle) and is hidden past the end of the list.
 */
extern "C" void cFEStateRequestLine_updatePlayListSongTitles(void* self)
{
    sMsgCenter_97BC8* requestLine = (sMsgCenter_97BC8*)self;
    int i;
    for (i = 0; i < 8; i++) {
        int idx = requestLine->top + i;
        if (idx < requestLine->count) {
            int id = requestLine->list[idx];
            requestLine->texts[i]->show(1);
            cUIText_setAsciiString((cUIText*)requestLine->texts[i], (const char*)cFEStateRequestLine_getSongTitle(self, id));
        } else {
            requestLine->texts[i]->show(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updatePlayListCheckMarks);
#ifdef SKIP_ASM
class cMsgWidget_97CA0 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};
// PORT: 64-bit `long` masks (8 bytes on EE, 4 on Windows); use uint64_t off-PS2.
struct sMsgCenter_97CA0 {
    char pad_0x0[0x4C];
    int list[64];   // 0x4C
    int count;      // 0x14C
    int top;        // 0x150
    char pad_0x154[0x4];
    ulong newMask;  // 0x158
    ulong readMask; // 0x160
    char pad_0x168[0x2C];
    cMsgWidget_97CA0* icons[8]; // 0x194
    char pad_0x1B4[0x50];
    int colNew;     // 0x204
    int colRead;    // 0x208
    int colOther;   // 0x20C
};

static inline void setColors_97CA0(cMsgWidget_97CA0* w, int a, int b)
{
    *(int*)((char*)w + 0x78) = a;
    *(int*)((char*)w + 0x7C) = b;
}

static inline int isNew_97CA0(sMsgCenter_97CA0* s, int id)
{
    return (int)((s->newMask >> id) & 1);
}

static inline int isRead_97CA0(sMsgCenter_97CA0* s, int id)
{
    return (int)((s->readMask >> id) & 1);
}

static inline void setVisible_97CA0(cMsgWidget_97CA0* w, int v)
{
    w->show(v);
}

/**
 * Shows and colours the check box of each visible playlist row.
 *
 * A song in the playlist (+0x158) gets the colour at +0x204, an owned one (+0x160) the colour at +0x208, any
 * other the colour at +0x20C; rows past the end are hidden.
 */
extern "C" void cFEStateRequestLine_updatePlayListCheckMarks(void* self)
{
    sMsgCenter_97CA0* requestLine = (sMsgCenter_97CA0*)self;
    int i;
    for (i = 0; i < 8; i++) {
        setVisible_97CA0(requestLine->icons[i], 0);
        int idx = requestLine->top + i;
        if (idx < requestLine->count) {
            int id = requestLine->list[idx];
            setVisible_97CA0(requestLine->icons[i], 1);
            if (isNew_97CA0(requestLine, id)) {
                setColors_97CA0(requestLine->icons[i], -1, requestLine->colNew);
            } else if (isRead_97CA0(requestLine, id)) {
                setColors_97CA0(requestLine->icons[i], -1, requestLine->colRead);
            } else {
                setColors_97CA0(requestLine->icons[i], -1, requestLine->colOther);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updatePlayListArrows);
#ifdef SKIP_ASM
class cMsgWidget_97DB8 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

/**
 * Shows or hides the play list's scroll arrows.
 *
 * The up arrow (+0x1EC) shows when the list is scrolled (+0x150 above 0); the down arrow (+0x1F0) shows while
 * more than 8 rows lie below the scroll position (+0x150 + 8 below the count at +0x14C).
 */
extern "C" void cFEStateRequestLine_updatePlayListArrows(void* self)
{
    if (*(int*)((char*)self + 0x150) == 0) {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1EC))->show(0);
    } else {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1EC))->show(1);
    }
    if (*(int*)((char*)self + 0x150) + 8 >= *(int*)((char*)self + 0x14C)) {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1F0))->show(0);
    } else {
        (*(cMsgWidget_97DB8**)((char*)self + 0x1F0))->show(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00197E70);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int USTR_length(unsigned short* s);
extern "C" int strlen(const char* s);
void cMemMan_free(void* p);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, int n);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* func_0028B180();
extern "C" void* func_002B40F0(void* self, int i);
extern "C" void* func_002B4120(void* self, int i);
extern char D_00460470[];
extern char D_00460668[];
extern char D_00460678[];

struct sVtEntry_197E70 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

class cMsgWidget_197E70 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

struct sReqLine_197E70 {
    char pad_0x0[0x1B4];
    cMsgWidget_197E70* title;   // 0x1B4
    cMsgWidget_197E70* artist;  // 0x1B8
};

extern "C" void func_00197E70(void* vself)
{
    sReqLine_197E70* self = (sReqLine_197E70*)vself;
    if (self->title == 0 || self->artist == 0) {
        return;
    }
    int cur = *(int*)((char*)func_0028B180() + 0x508);
    if (cur < *(int*)((char*)func_0028B180() + 0x504)) {
        char* name = (char*)func_002B40F0((char*)func_0028B180() + 0x118, cur);
        char* artist = (char*)func_002B4120((char*)func_0028B180() + 0x118, cur);
        if (strlen(name) != 0) {
            self->title->show(1);
            char* db = (char*)SSXApp->FELocale;
            sVtEntry_197E70* vt = *(sVtEntry_197E70**)(db + 4);
            unsigned short* fmt = vt[4].fn(db + vt[4].delta, GetHashValue32(D_00460668));
            int len = USTR_length(fmt) + strlen(name) + 1;
            unsigned short* buf = (unsigned short*)operator_new_tag(len * 2, D_00460470, 0x100, 0);
            // PORT: the format argument is a string pointer passed through an int parameter
            sprintf1(buf, fmt, (int)name);
            cUIText_setUnicodeString(self->title, buf);
            if (buf != 0) {
                cMemMan_free(buf);
            }
        } else {
            self->title->show(0);
        }
        if (strlen(artist) != 0) {
            self->artist->show(1);
            char* db = (char*)SSXApp->FELocale;
            sVtEntry_197E70* vt = *(sVtEntry_197E70**)(db + 4);
            unsigned short* fmt = vt[4].fn(db + vt[4].delta, GetHashValue32(D_00460678));
            int len = USTR_length(fmt) + strlen(artist) + 1;
            unsigned short* buf = (unsigned short*)operator_new_tag(len * 2, D_00460470, 0x100, 0);
            // PORT: the format argument is a string pointer passed through an int parameter
            sprintf1(buf, fmt, (int)artist);
            cUIText_setUnicodeString(self->artist, buf);
            if (buf != 0) {
                cMemMan_free(buf);
            }
        } else {
            self->artist->show(0);
        }
    } else {
        self->title->show(0);
        self->artist->show(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updatePlayListCounter);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int USTR_length(unsigned short* s);
void cMemMan_free(void* p);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, int n);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004605D8[];
extern char D_00460688[];

struct sVtEntry_00198118 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_00198118 {
    char pad_0x0[0x158];
    ulong mask;     // 0x158
    char pad_0x160[0x84];
    void* text;     // 0x1E4
};

/**
 * Shows how many songs are in the playlist (kT_OVRCMNSongsInList).
 *
 * Counts the bits of the playlist mask (+0x158) and writes the formatted text to the counter widget (+0x1E4).
 *
 * @param p  the cFEStateRequestLine (raw pointer)
 */
extern "C" void cFEStateRequestLine_updatePlayListCounter(void* p)
{
    sMsgCenter_00198118* self = (sMsgCenter_00198118*)p;
    char* db = (char*)SSXApp->FELocale;
    sVtEntry_00198118* vt = *(sVtEntry_00198118**)(db + 4);
    unsigned short* fmt = vt[4].fn(db + vt[4].delta, GetHashValue32(D_004605D8));
    unsigned short* buf = (unsigned short*)operator_new_tag((USTR_length(fmt) + 5) * 2, D_00460688, 0x100, 0);
    int n = 0;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((self->mask >> j) & 1);
    }
    sprintf1(buf, fmt, n);
    cUIText_setUnicodeString(self->text, buf);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateHelpText);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
int func_00198AE8(void* self);
extern char D_004606B0[];
extern char D_004606C8[];
extern char D_004606E0[];
extern char D_004606F8[];
extern char D_00460710[];
extern char D_00460720[];
extern signed char D_00535C11[];

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sReqLine_001981F8 {
    char pad_0x0[0x158];
    ulong done;     // 0x158
    ulong sel;      // 0x160
    int count;      // 0x168
    int f16C;       // 0x16C
    char pad_0x170[0x78];
    cUIText* text;  // 0x1E8
};

static inline int countSel_001981F8(sReqLine_001981F8* self)
{
    int n = 0;
    for (int i = 0; i < 64; i++) {
        n += (self->sel >> i) & 1;
    }
    return n;
}

/**
 * Picks the help text for the highlighted song.
 *
 * Owned: kT_HELPAudio1SongMin if it is the only playlist song, else kT_HELPAUDIOAddRem. In the store:
 * kT_HELPAUDIOAvailBuy when affordable or free, else kT_16SaveMoreCarsh. Otherwise kT_FAQRADIOBA or
 * kT_CMNHELPBuySongCTM by D_00535C11[0].
 *
 * @param p  the cFEStateRequestLine (raw pointer)
 * @param idx  song index
 */
extern "C" void cFEStateRequestLine_updateHelpText(void* p, int idx)
{
    sReqLine_001981F8* self = (sReqLine_001981F8*)p;
    if (self->text == 0) {
        return;
    }
    int on = (self->sel >> idx) & 1;
    if (on) {
        ulong bit = (ulong)1 << idx;
        int only = (self->done & ~bit) == 0;
        if (only) {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606B0));
        } else {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606C8));
        }
    } else if (self->f16C != 0) {
        if (self->count >= func_00198AE8(self) || countSel_001981F8(self) < 6) {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606E0));
        } else {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_004606F8));
        }
    } else {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 0) {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_00460710));
        } else {
            cUIText_setUnicodeStringByID(self->text, GetHashValue32(D_00460720));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateButtonsText);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
int func_00198AE8_UBT(void* self, int idx) __asm__("func_00198AE8__FPv");
extern char D_004A1390[];
extern char D_00460738[];
extern char D_00460748[];
extern char D_00460758[];

class cUIObj_UBT {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void setVisible(int on);
};

struct sReqLine_UBT {
    char pad_0x0[0x158];
    unsigned long done;     // 0x158
    unsigned long sel;      // 0x160
    int count;              // 0x168
    int f16C;               // 0x16C
    char pad_0x170[0x1F4 - 0x170];
    cUIObj_UBT* text;       // 0x1F4
    cUIObj_UBT* back;       // 0x1F8
    cUIObj_UBT* btnA;       // 0x1FC
    cUIObj_UBT* btnB;       // 0x200
};

// Release-build assert: expands to an empty do { } while (0). The empty loop still leaves loop notes in the
// RTL, which is what keeps the done-mask copy scheduled where the target has it.
#define ASSERT_UBT(cond) do { } while (0)

static inline int isSet_UBT(unsigned long m, int i)
{
    return (m >> i) & 1;
}

static inline int countSel_UBT(sReqLine_UBT* self)
{
    int n = 0;
    for (int i = 0; i < 64; i++) {
        n += (self->sel >> i) & 1;
    }
    return n;
}

/**
 * Sets the action label for one song on the request line screen and shows or hides the buttons.
 *
 * For a song whose bit is set in the mask at +0x160, the label is "kT_BTNRemSong" when it is in the play list
 * mask (+0x158) with other songs, or "kT_BTNAddSong" when it is not in the play list. For any other song, when
 * +0x16C is set, the label is "kT_16BuyMusicTracks" if the cash (+0x168) is at least the 5000 price or fewer
 * than 6 songs have their bit set. In a game (SSXApp->Game set), the two buttons hide for songs without their
 * bit.
 *
 * @param p    the request line state (this)
 * @param idx  the song index (bit number in the masks)
 */
// Updates the action button label of the selected request line and shows or hides the buttons.
extern "C" void cFEStateRequestLine_updateButtonsText(void* p, int idx)
{
    sReqLine_UBT* self = (sReqLine_UBT*)p;
    if (self->text != 0) {
        int shown = 0;
        cUIText_setAsciiString((cUIText*)self->text, D_004A1390);
        self->text->setVisible(0);
        self->back->setVisible(0);
        int on = (self->sel >> idx) & 1;
        if (on) {
            unsigned long rest = self->done;
            ASSERT_UBT(idx < 64);
            rest &= ~((unsigned long)1 << idx);
            int only = rest == 0;
            if (!only) {
                if (isSet_UBT(self->done, idx)) {
                    shown = 1;
                    cUIText_setUnicodeStringByID((cUIText*)self->text, GetHashValue32(D_00460738));
                } else {
                    shown = 1;
                    cUIText_setUnicodeStringByID((cUIText*)self->text, GetHashValue32(D_00460748));
                }
            } else if (!isSet_UBT(self->done, idx)) {
                shown = 1;
                cUIText_setUnicodeStringByID((cUIText*)self->text, GetHashValue32(D_00460748));
            }
        } else if (self->f16C != 0) {
            if (self->count >= func_00198AE8_UBT(self, idx) || countSel_UBT(self) < 6) {
                shown = 1;
                cUIText_setUnicodeStringByID((cUIText*)self->text, GetHashValue32(D_00460758));
            }
        }
        if (shown) {
            self->text->setVisible(1);
            self->back->setVisible(1);
        }
    }
    if ((int)SSXApp->Game != 0 && !isSet_UBT(self->sel, idx)) {
        self->btnA->setVisible(0);
        self->btnB->setVisible(0);
    } else {
        self->btnA->setVisible(1);
        self->btnB->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateCash);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* getDollarString(void* a0);

/**
 * Shows the player's cash (+0x168) in the cash text (+0x1E0) through getDollarString(), when the text exists.
 */
extern "C" void cFEStateRequestLine_updateCash(void* self)
{
    if (*(cUIText**)((char*)self + 0x1E0) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x1E0), getDollarString(*(void**)((char*)self + 0x168)));
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateFreeSongMarkers);
#ifdef SKIP_ASM
class cMsgWidget_985F0 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_985F0 {
    char pad_0x0[0x160];
    ulong mask;     // 0x160
    char pad_0x168[0x4];
    int enabled;    // 0x16C
    char pad_0x170[0x4C];
    cMsgWidget_985F0* icons[6]; // 0x1BC
};

/**
 * Shows one marker per free song still to pick.
 *
 * Marker i is shown in the store while fewer than 6 - i songs are owned (6 songs are free).
 */
extern "C" void cFEStateRequestLine_updateFreeSongMarkers(void* self)
{
    sMsgCenter_985F0* requestLine = (sMsgCenter_985F0*)self;
    int n = 0;
    int i;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((requestLine->mask >> j) & 1);
    }
    for (i = 0; i < 6; i++) {
        if (requestLine->icons[i] != 0) {
            if (requestLine->enabled != 0 && n < 6 - i) {
                requestLine->icons[i]->show(1);
            } else {
                requestLine->icons[i]->show(0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_updateHilightedSongInfo);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int USTR_length(unsigned short* s);
extern "C" int strlen(const char* s);
void cMemMan_free(void* p);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, int n);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* func_00198AB8(void* self, int i);
// PORT: func_00198AE8__FPv is called with (self, idx) here; bind the 2-arg form to that symbol.
int func_00198AE8_2(void* self, int idx) __asm__("func_00198AE8__FPv");
extern "C" const char* getDollarString(void* a0);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_00460770[];
extern char D_00460780[];
extern char D_00460688[];
extern char D_004A1408[];

struct sVtEntry_001986D0 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

class cMsgWidget_1986D0 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sReqLine_1986D0 {
    char pad_0x0[0x160];
    ulong mask;                 // 0x160
    char pad_0x168[0x4];
    int enabled;                // 0x16C
    char pad_0x170[0x64];
    cMsgWidget_1986D0* title;   // 0x1D4
    cUIText* info;              // 0x1D8
    cMsgWidget_1986D0* icon;    // 0x1DC
};

/**
 * Shows the highlighted song's artist line and its price.
 *
 * "song by" gets kT_OVRCMNSongBy with the artist (func_00198AB8). The cost text is empty for an owned song,
 * kT_OVRCMNFree while fewer than 6 are owned, else the price (func_00198AE8, always $5000); the cost icon
 * shows only in the store.
 *
 * @param vself  the cFEStateRequestLine
 * @param idx  song index
 */
extern "C" void cFEStateRequestLine_updateHilightedSongInfo(void* vself, int idx)
{
    sReqLine_1986D0* self = (sReqLine_1986D0*)vself;
    if (self->title != 0) {
        char* db = (char*)SSXApp->FELocale;
        sVtEntry_001986D0* vt = *(sVtEntry_001986D0**)(db + 4);
        unsigned short* fmt = vt[4].fn(db + vt[4].delta, GetHashValue32(D_00460770));
        char* name = (char*)func_00198AB8(self, idx);
        int len = USTR_length(fmt) + strlen(name) + 1;
        unsigned short* buf = (unsigned short*)operator_new_tag(len * 2, D_00460688, 0x100, 0);
        // PORT: the format argument is a string pointer passed through an int parameter
        sprintf1(buf, fmt, (int)name);
        self->title->show(1);
        cUIText_setUnicodeString(self->title, buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
    if (self->info == 0) {
        return;
    }
    ulong mask = self->mask;
    if ((int)((mask >> idx) & 1)) {
        cUIText_setAsciiString(self->info, D_004A1408);
        self->icon->show(0);
        return;
    }
    int n = 0;
    for (int j = 0; j < 64; j++) {
        n += (int)((mask >> j) & 1);
    }
    if (n < 6) {
        cUIText_setUnicodeStringByID(self->info, GetHashValue32(D_00460780));
        if (self->enabled) {
            self->icon->show(1);
        }
    } else {
        cUIText_setAsciiString(self->info, getDollarString((void*)func_00198AE8_2(self, idx)));
        if (self->enabled) {
            self->icon->show(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_001988D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00158558(void* iface, int rider, int a, int idx, int full);

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sMsgCenter_988D8 {
    char pad_0x0[0x44];
    signed char rider; // 0x44
    char pad_0x45[0x3];
    int a48;        // 0x48
    char pad_0x4C[0x10C];
    ulong seen;     // 0x158
    ulong mask;     // 0x160
    int result;     // 0x168
};

extern "C" void func_001988D8(void* self, int idx)
{
    sMsgCenter_988D8* s = (sMsgCenter_988D8*)self;
    int n = 0;
    int j;
    for (j = 0; j < 64; j++) {
        n += (int)((s->mask >> j) & 1);
    }
    int full = n >= 6;
    s->result = func_00158558(cBE_getInterface_Fv(cBE_getBE(), 0xD), s->rider, s->a48, idx, full);
    ulong bit = (ulong)1 << idx;
    s->mask |= bit;
    s->seen |= bit;
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_showBuyPopup);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* self, void* engine, void* owner, signed char idx);
extern "C" void* cFEStateRequestLine_getSongTitle(void* self, int i);
// PORT: func_00198AE8__FPv is called with (self, idx) here; bind the 2-arg form to that symbol.
int func_00198AE8_2(void* self, int idx) __asm__("func_00198AE8__FPv");
extern "C" void cBuyPopupInfo_initBuySong(int* self, int a1, int a2, int a3);
extern "C" void cBuyPopupInfo_initBuySongByCredit(void* self, int song, int credit);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern char D_00460790[];

// PORT: 64-bit `long` mask (8 bytes on EE, 4 on Windows); use uint64_t off-PS2.
struct sMsgCenter_98988 {
    char pad_0x0[0x10];
    char* engine;       // 0x10
    char pad_0x14[0x30];
    signed char rider;  // 0x44
    char pad_0x45[0x11B];
    ulong mask;         // 0x160
    int price;          // 0x168
};

/**
 * Opens the buy popup (cUIStateBuyPopup) for a song.
 *
 * With 6 or more songs owned it is a cash purchase (price from func_00198AE8, cash +0x168); otherwise it uses
 * one of the remaining free credits (6 - owned).
 *
 * @param idx  song index
 */
extern "C" void cFEStateRequestLine_showBuyPopup(sMsgCenter_98988* self, int idx)
{
    char* popup = (char*)cUIStateBuyPopup_cUIStateBuyPopup(cMemMan_alloc(0x70, D_00460790, 0, 0), self->engine, self, self->rider);
    int n = 0;
    for (int i = 0; i < 64; i++) {
        n += (int)((self->mask >> i) & 1);
    }
    int song = (int)cFEStateRequestLine_getSongTitle(self, idx);
    if (n >= 6) {
        int cost = func_00198AE8_2(self, idx);
        cBuyPopupInfo_initBuySong((int*)(popup + 0x48), song, cost, self->price);
    } else {
        cBuyPopupInfo_initBuySongByCredit(popup + 0x48, song, 6 - n);
    }
    cUIStateStack_pushPopup(self->engine + 0x18, popup);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", cFEStateRequestLine_getSongTitle);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void* func_002B40F0(void* self, int i);

/**
 * Returns a song title from the audio object's music list (gSSXAudio + 0x118) through func_002B40F0().
 *
 * @param i  the song index
 * @return the song title
 */
extern "C" void* cFEStateRequestLine_getSongTitle(void* self, int i)
{
    return func_002B40F0((char*)func_0028B180() + 0x118, i);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198AB8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void* func_002B4120(void* self, int i);

extern "C" void* func_00198AB8(void* self, int i)
{
    return func_002B4120((char*)func_0028B180() + 0x118, i);
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198AE8__FPv);
#ifdef SKIP_ASM
int func_00198AE8(void* self)
{
    return 0x1388;
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", getDollarString);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strlen(const char* s);
extern char D_004C6310[];
extern char D_004A12C0[];

struct sOptions_8AF0 {
    unsigned int pad0 : 22;
    unsigned int moneyFormat : 3;   // 0 = "$ 1,000", 2 = "$ 1.000", 1/3 = "1 000 $"
    unsigned int pad1 : 7;
    int data[0x288 / 4 - 1];
};
extern sOptions_8AF0 D_00535610_8AF0 __asm__("D_00535610");

struct cBEOptionInterface_8AF0 {
    sOptions_8AF0 getOptions() { return D_00535610_8AF0; }
};

/**
 * Formats a money amount with the currency sign and thousands separator chosen in the options.
 *
 * Money format 0 gives "$ 1,234", 2 gives "$ 1.234", and 1 and 3 give "1 234 $". Negative amounts get a '-'.
 * The text is built in the static buffer D_004C6310, which the next call overwrites.
 *
 * @param amount_  the amount, passed as void*
 * @return the formatted string
 */
// Formats a money amount with the currency sign and thousands separator chosen in the options.
extern "C" const char* getDollarString(void* amount_)
{
    // PORT: callers in this unit declare the amount parameter as void*.
    int amount = (int)amount_;
    int prefix = 1;
    char sep = ' ';
    char n;
    switch (((cBEOptionInterface_8AF0*)cBE_getInterface_Fv(cBE_getBE(), 4))->getOptions().moneyFormat) {
    case 0:
        sep = ',';
        break;
    case 1:
        prefix = 0;
        break;
    case 2:
        sep = '.';
        break;
    case 3:
        prefix = 0;
        break;
    }
    n = 0;
    if (prefix) {
        D_004C6310[0] = '$';
        D_004C6310[1] = ' ';
        n = 2;
    }
    if (amount < 0) {
        D_004C6310[n++] = '-';
        amount = -amount;
    }
    char valueStr[32];
    sprintf(valueStr, D_004A12C0, amount);
    char len = strlen(valueStr);
    char i;
    for (i = 0; i != len; i++) {
        D_004C6310[n++] = valueStr[i];
        char left = len - i - 1;
        if (left != 0 && left % 3 == 0)
            D_004C6310[n++] = sep;
    }
    if (!prefix) {
        D_004C6310[n++] = ' ';
        D_004C6310[n++] = '$';
    }
    D_004C6310[n] = 0;
    return D_004C6310;
}
#endif

//100%
INCLUDE_ASM("fe/messagecenter", func_00198DA0);
#ifdef SKIP_ASM
extern "C" int func_00198DA0(void* self, void* a1)
{
    void* p = *(void**)self;
    void* p2 = *(void**)a1;
    return *(short*)((char*)p + 0xa) - *(short*)((char*)p2 + 0xa);
}
#endif

