#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_129_rewardgallery[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001D0180(void* self);
extern "C" void func_001CFE60(void* self);
struct sRewardEntry_F0B0 {
    char name[8];
    int id;
};
struct sRewardGallery_F0B0 {
    char pad_0x0[0x7C];
    sRewardEntry_F0B0 entries[180];  // 0x7C
};

/**
 * Creates the reward gallery screen ("129 rewardgallery"), clears the 180 reward entries and starts loading.
 *
 * Calls func_001D0180() (checks whether the rewards are loaded, else starts the load) and, unless flag bit 3
 * of
 * +0x48 is set, func_001CFE60().
 */
extern "C" void cFEStateRewardGalleryBase_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_129_rewardgallery), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    int i;
    for (i = 0; i < 180; i++) {
        ((sRewardGallery_F0B0*)self)->entries[i].name[0] = 0;
        ((sRewardGallery_F0B0*)self)->entries[i].id = -1;
    }
    *(int*)((char*)self + 0x78) = 0;
    func_001D0180(self);
    if (((*(int*)((char*)self + 0x48) >> 3) & 1) == 0) {
        func_001CFE60(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CF168);
#ifdef SKIP_ASM
struct sVec3_F168 { float x, y, z; };
extern "C" void func_003A0290(void* thing, sVec3_F168* out);
struct sVE_F168a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_F168b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CF168(void* self)
{
    sVec3_F168 firstCellPos;
    sVec3_F168 widgetPos;
    void* widget = *(void**)((char*)self + 0x924);
    if (widget != 0) {
        func_003A0290(widget, &firstCellPos);
        widget = *(void**)((char*)self + 0x8F8);
        if (widget != 0) {
            func_003A0290(widget, &widgetPos);
            *(float*)((char*)self + 0x74) = widgetPos.y - firstCellPos.y;
            *(float*)((char*)self + 0x70) = widgetPos.x - firstCellPos.x;
        }
    }
    *(int*)((char*)self + 0x64) = 0;
    int fl = *(int*)((char*)self + 0x48);
    if (((fl >> 3) & 1) && ((fl >> 1) & 1)) {
        if (*(int*)((char*)self + 0x8FC) == 0) {
            *(int*)((char*)self + 0x8FC) = *(int*)((char*)self + 0x924);
        }
        sVE_F168a* vt = *(sVE_F168a**)((char*)self + 8);
        vt[40].fn((char*)self + vt[40].delta);
        sVE_F168b* vt2 = *(sVE_F168b**)((char*)self + 8);
        vt2[33].fn((char*)self + vt2[33].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CF238);
#ifdef SKIP_ASM
extern "C" void func_001D0238(void* self);
extern "C" void func_001CFF10(void* self);
extern "C" void* cUIState_onUpdate(void* self);

extern "C" void func_001CF238(void* self)
{
    func_001D0238(self);
    func_001CFF10(self);
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CF270);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void cUIStateStack_push(void* self, void* a1);
extern char str_TransitionOut[];

// g++ 2.95 vtable entries: this-adjust, index, function.
struct sVEi_F270 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVEii_F270 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sVEv_F270 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEr_F270 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
struct sSnd_F270 {
    sVEv_F270* vt;
};
// $gp-relative default sound player (the unit declares it later as sDefObj_1FF0).
extern sSnd_F270 D_004A5A58_F270 __asm__("gUISoundNull");

static inline int Test_F270(char* o, int slot)
{
    sVEi_F270* vt = *(sVEi_F270**)(o + 8);
    return vt[slot].fn(o + vt[slot].delta);
}

static inline int Call_F270(char* o, int slot, int a)
{
    sVEii_F270* vt = *(sVEii_F270**)(o + 8);
    return vt[slot].fn(o + vt[slot].delta, a);
}

static inline void CallV_F270(char* o, int slot, int a)
{
    sVEv_F270* vt = *(sVEv_F270**)(o + 8);
    vt[slot].fn(o + vt[slot].delta, a);
}

static inline void Play_F270(sSnd_F270* snd, int id)
{
    snd->vt[2].fn((char*)snd + snd->vt[2].delta, id);
}

struct sGallery_F270 {
    char pad0[0x924];
    char* cells[4][8]; // 0x924
};

// Reward gallery input: back out, accept the highlighted reward, or move the cursor around the grid.
extern "C" int func_001CF270(char* self, char* in)
{
    sSnd_F270* snd = &D_004A5A58_F270;
    if (*(sSnd_F270**)(*(char**)(self + 0x10) + 0x14) != 0) {
        snd = *(sSnd_F270**)(*(char**)(self + 0x10) + 0x14);
    }
    if (Test_F270(in, 6)) {
        Play_F270(snd, 3);
        int i;
        for (i = 0; i < 4; i++) {
            int j;
            for (j = 0; j < 8; j++) {
                char* w = ((sGallery_F270*)self)->cells[i][j];
                if (w != 0) {
                    sVEv_F270* vt = *(sVEv_F270**)(w + 8);
                    vt[9].fn(w + vt[9].delta, 0);
                }
            }
        }
        cUIScreen_playFrame(*(void**)(self + 0x40), cUIScreen_getFrameByLabel(*(cUIScreen**)(self + 0x40), GetHashValue32(str_TransitionOut)), 1);
        char* mgr = **(char***)(self + 0x10);
        sVEr_F270* vt = *(sVEr_F270**)(mgr + 4);
        void* r = vt[5].fn(mgr + vt[5].delta, self, *(int*)(self + 0x8F0) >= 0);
        cUIStateStack_push(*(char**)(self + 0x10) + 0x18, r);
        return 1;
    }
    char* cur = *(char**)(self + 0x8FC);
    int curIdx = 0;
    if (cur != 0) {
        curIdx = *(int*)(cur + 0x18);
    }
    int changed = 0;
    int row = curIdx / *(int*)(self + 0x5C);
    int col = curIdx % *(int*)(self + 0x5C);
    int scroll = *(int*)(self + 0x64);
    if (Test_F270(in, 5)) {
        int idx = *(int*)(self + 0x64) * *(int*)(self + 0x5C) + curIdx;
        if (*(int*)(self + 0x68) > 0) {
            if (Call_F270(self, 38, idx)) {
                if (*(int*)(self + 0x4C) != 8) {
                    Play_F270(snd, 6);
                    CallV_F270(self, 31, idx);
                } else {
                    Play_F270(snd, 4);
                }
            } else {
                if (*(int*)(self + 0x8F0) >= 0) {
                    int item = Call_F270(self, 34, idx);
                    int kind = *(int*)(self + 0x8F0);
                    if (kind == Call_F270(self, 36, item) && Call_F270(self, 35, item) <= *(int*)(self + 0x54)) {
                        Play_F270(snd, 6);
                        CallV_F270(self, 32, idx);
                    } else {
                        Play_F270(snd, 4);
                    }
                } else {
                    Play_F270(snd, 4);
                }
            }
            return 1;
        }
        Play_F270(snd, 4);
        return 1;
    } else {
        if (Test_F270(in, 19)) {
            col--;
            if (col < 0) col = *(int*)(self + 0x5C) - 1;
            changed = 1;
        } else if (Test_F270(in, 20)) {
            col++;
            if (col >= *(int*)(self + 0x5C) || (*(int*)(self + 0x64) + row) * *(int*)(self + 0x5C) + col >= *(int*)(self + 0x68)) col = 0;
            changed = 1;
        } else if (Test_F270(in, 17)) {
            row--;
            if (row < 0) row = *(int*)(self + 0x58) - 1;
            changed = 1;
        } else if (Test_F270(in, 18)) {
            row++;
            if (row >= *(int*)(self + 0x58) || *(int*)(self + 0x60) < *(int*)(self + 0x64) + row) row = 0;
            changed = 1;
        } else if (Test_F270(in, 27)) {
            scroll -= *(int*)(self + 0x58);
            if (scroll < 0) scroll = (*(int*)(self + 0x60) - 1) / *(int*)(self + 0x58) * *(int*)(self + 0x58);
            changed = 1;
        } else if (Test_F270(in, 25)) {
            changed = 1;
            scroll += *(int*)(self + 0x58);
            if (scroll >= *(int*)(self + 0x60)) scroll = 0;
        }
        if (changed) {
        int cols = *(int*)(self + 0x5C);
        int local = row * cols + col;
        int idx = local + scroll * cols;
        if (idx >= *(int*)(self + 0x68)) idx = *(int*)(self + 0x68) - 1;
        local = idx - scroll * cols;
        if (curIdx != local || scroll != *(int*)(self + 0x64)) {
            *(int*)(self + 0x64) = scroll;
            *(char**)(self + 0x8FC) = ((sGallery_F270*)self)->cells[local / cols][local % cols];
            sVEi_F270* svt = *(sVEi_F270**)(self + 8);
            svt[40].fn(self + svt[40].delta);
            Call_F270(self, 33, idx);
            if (Test_F270(in, 17) || Test_F270(in, 18)) {
                Play_F270(snd, 2);
            } else {
                Play_F270(snd, 1);
            }
            return 1;
        }
        Play_F270(snd, 4);
        return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_helptext[];
extern char str_arrow_up[];
extern char str_arrow_down[];
extern char str_orange_select_bar[];
extern char str_max_rewards[];
extern char str_num_rewards[];
extern char str_slash[];
extern char D_004A1EF0[];
extern char str_Menu0000[];
extern char str_Poster_icon[];
extern char str_poster_icon[];
extern char str_trading_card_icon[];
extern char str_art_icon[];
extern char str_video_icon[];
extern char str_toys_icon[];
extern char str_screentitle[];
extern char str_kT_OVRCMNPosters[];
extern char str_kT_OVRCMNTradeCard[];
extern char str_kT_OVRCMNArt[];
extern char str_kT_OVRCMNVideos[];
extern char str_kT_OVRCMNToys[];
extern char str_kT_TITLECheatCharacter[];
extern char str_Page_of_text[];
extern char str_reward_name_colon[];
extern char str_kT_OVRCMNPosterColon[];
extern char str_kT_OVRCMNCardColon[];
extern char str_kT_OVRCMNArtColon[];
extern char str_kT_OVRCMNVideoColon[];
extern char str_kT_OVRCMNToyColon[];
extern char str_kT_OVRCMNCheatCharColon[];
extern char str_reward_name[];
extern char str_reward_cost[];
extern char str_you_have_amount[];
extern char str_pct_d_b[];
extern char D_004A1408[];
extern char* D_00441440[][8];
extern char* D_004414C0[][8];

struct sVEi_F820 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEv_F820 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sWidget_F820 {
    char pad0[0x8];
    sVEi_F820* vt;              // 0x8
    char padC[0x18 - 0xC];
    int index;                  // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;                   // 0x38
    char pad3C[0x78 - 0x3C];
    int reward;                 // 0x78
    void* tex;                  // 0x7C
};

struct sGallery_F820 {
    char pad0[0x8];
    sVEv_F820* vt;              // 0x8
    char padC[0x10 - 0xC];
    char* ui;                   // 0x10
    char pad14[0x4C - 0x14];
    int type;                   // 0x4C
    char pad50[0x58 - 0x50];
    int rows;                   // 0x58
    int cols;                   // 0x5C
    char pad60[0x68 - 0x60];
    int count68;                // 0x68
    int count6C;                // 0x6C
    char pad70[0x8F4 - 0x70];
    sWidget_F820* w8F4;         // 0x8F4
    sWidget_F820* w8F8;         // 0x8F8
    char pad8FC[0x900 - 0x8FC];
    sWidget_F820* w900;         // 0x900
    sWidget_F820* w904;         // 0x904
    sWidget_F820* w908;         // 0x908
    sWidget_F820* w90C;         // 0x90C
    sWidget_F820* w910;         // 0x910
    sWidget_F820* w914;         // 0x914
    sWidget_F820* w918;         // 0x918
    sWidget_F820* w91C;         // 0x91C
    sWidget_F820* w920;         // 0x920
    sWidget_F820* cells[4][8];  // 0x924
    sWidget_F820* frames[4][8]; // 0x9A4
};

static inline void show_F820(sWidget_F820* w, int on)
{
    w->vt[9].fn((char*)w + w->vt[9].delta, on);
}

/**
 * Binds the reward gallery's widgets by name as the screen creates them, and fills the fixed ones.
 *
 * Stores the help text, arrows, select bar, counters ("max_rewards", "num_rewards" as %d), page text, reward
 * name,
 * cost and cash widgets; sets the title, the "reward name colon" label and the poster icon for the gallery's
 * reward
 * type (3 posters, 4 trading cards, 5 art, 6 videos, 7 toys, 8 cheat characters); and records the grid cells.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateRewardGalleryBase_onWidgetCreate(sGallery_F820* self, sWidget_F820* w)
{
    char buf[16];
    int hash = w->hash;

    if (hash == GetHashValue32(str_helptext)) {
        self->w900 = w;
        show_F820(w, 0);
    } else if (hash == GetHashValue32(str_arrow_up)) {
        self->w91C = w;
    } else if (hash == GetHashValue32(str_arrow_down)) {
        self->w920 = w;
    } else if (hash == GetHashValue32(str_orange_select_bar)) {
        self->w8F8 = w;
        show_F820(w, 0);
    } else if (hash == GetHashValue32(str_max_rewards)) {
        self->w918 = w;
        sprintf(buf, str_pct_d_b, self->count68);
        cUIText_setAsciiString((cUIText*)self->w918, buf);
    } else if (hash == GetHashValue32(str_num_rewards)) {
        self->w914 = w;
        sprintf(buf, str_pct_d_b, self->count6C);
        cUIText_setAsciiString((cUIText*)self->w914, buf);
    } else if (hash == GetHashValue32(str_slash)) {
        cUIText_setAsciiString((cUIText*)w, D_004A1EF0);
    } else if (hash == GetHashValue32(str_Menu0000)) {
        self->w8F4 = w;
    } else if (hash == GetHashValue32(str_Poster_icon)) {
        switch (self->type) {
        case 3:
            {
                char* ui = self->ui;
                int texHash = GetHashValue32(str_poster_icon);
                w->tex = cUITextureBank_getElementByHashname(ui + 0x58, texHash);
            }
            w->reward = -1;
            break;
        case 4:
            {
                char* ui = self->ui;
                int texHash = GetHashValue32(str_trading_card_icon);
                w->tex = cUITextureBank_getElementByHashname(ui + 0x58, texHash);
            }
            w->reward = -1;
            break;
        case 5:
            {
                char* ui = self->ui;
                int texHash = GetHashValue32(str_art_icon);
                w->tex = cUITextureBank_getElementByHashname(ui + 0x58, texHash);
            }
            w->reward = -1;
            break;
        case 6:
            {
                char* ui = self->ui;
                int texHash = GetHashValue32(str_video_icon);
                w->tex = cUITextureBank_getElementByHashname(ui + 0x58, texHash);
            }
            w->reward = -1;
            break;
        case 7:
            {
                char* ui = self->ui;
                int texHash = GetHashValue32(str_toys_icon);
                w->tex = cUITextureBank_getElementByHashname(ui + 0x58, texHash);
            }
            w->reward = -1;
            break;
        }
    } else if (hash == GetHashValue32(str_screentitle)) {
        switch (self->type) {
        case 3:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNPosters));
            break;
        case 4:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNTradeCard));
            break;
        case 5:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNArt));
            break;
        case 6:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNVideos));
            break;
        case 7:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNToys));
            break;
        case 8:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLECheatCharacter));
            break;
        }
    } else if (hash == GetHashValue32(str_Page_of_text)) {
        self->w904 = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
    } else if (hash == GetHashValue32(str_reward_name_colon)) {
        int type = self->type;
        if (type == 3)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNPosterColon));
        else if (type == 4)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNCardColon));
        else if (type == 5)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNArtColon));
        else if (type == 6)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNVideoColon));
        else if (type == 7)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNToyColon));
        else if (type == 8)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNCheatCharColon));
    } else if (hash == GetHashValue32(str_reward_name)) {
        self->w908 = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
    } else if (hash == GetHashValue32(str_reward_cost)) {
        self->w90C = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
    } else if (hash == GetHashValue32(str_you_have_amount)) {
        self->w910 = w;
        self->vt[43].fn((char*)self + self->vt[43].delta);
    } else {
        for (int row = 0; row < self->rows; row++) {
            for (int col = 0; col < self->cols; col++) {
                if (hash == GetHashValue32(D_00441440[row][col])) {
                    w->index = row * self->cols + col;
                    show_F820(w, 0);
                    self->cells[row][col] = w;
                } else if (hash == GetHashValue32(D_004414C0[row][col])) {
                    self->frames[row][col] = w;
                    show_F820(w, 0);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CFD18);
#ifdef SKIP_ASM
struct sVE_FD18 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CFD18(void* self, void* widget, int msg)
{
    switch (msg) {
    case 21:
        if (*(int*)((char*)widget + 0xC) != 7) {
            *(int*)((char*)self + 0x1C) &= ~8;
        }
        break;
    case 22:
        if (((*(int*)((char*)self + 0x1C) >> 3) & 1) == 0) {
            *(int*)((char*)self + 0x1C) |= 8;
        }
        if (*(int*)((char*)widget + 0xC) == 7 && *(int*)((char*)widget + 0x6C) != 0) {
            sVE_FD18* vt = *(sVE_FD18**)((char*)self + 8);
            vt[30].fn((char*)self + vt[30].delta,
                      *(int*)((char*)self + 0x64) * *(int*)((char*)self + 0x5C) + *(int*)(*(char**)((char*)self + 0x8FC) + 0x18));
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CFDD0);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

struct sRewardName_FDD0 {
    char name[8];
    int id;
};

struct sRewardTable_FDD0 {
    char pad_0x0[0x78];
    int count;
    sRewardName_FDD0 entries[1];
};

// PORT: the unit declares func_001CFDD0 as `void (void*, char*)`, but the body returns the id.
int func_001CFDD0_find(sRewardTable_FDD0* self, char* name) __asm__("func_001CFDD0");

int func_001CFDD0_find(sRewardTable_FDD0* self, char* name)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (func_004165A8(self->entries[i].name, name) == 0) {
            return self->entries[i].id;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CFE60);
#ifdef SKIP_ASM
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);
struct sVE_FE60 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001CFE60(void* self)
{
    char* mgr = (char*)SSXApp->RealFE + 0x1A70;
    sVE_FE60* vt = *(sVE_FE60**)((char*)self + 8);
    if (vt[25].fn((char*)self + vt[25].delta) != 0) {
        sVE_FE60* fileNameEntry = &(*(sVE_FE60**)((char*)self + 8))[25];
        *(int*)((char*)self + 0x8EC) = func_0019DA10(mgr, *(int*)((char*)self + 0x50), fileNameEntry->fn((char*)self + fileNameEntry->delta), 9, 1, 0);
        *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~8) | 4;
    } else {
        *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) | 8) & ~4;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CFF10);
#ifdef SKIP_ASM
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" int func_0019E238(void* self, int bank, int i);
struct sSelf001CFFF8;
struct sData001CFFF8;
extern "C" sData001CFFF8* func_0019E2B0(void* self, int bank, int i);
extern "C" void func_001CFFF8(sSelf001CFFF8* self, sData001CFFF8* data);
struct sVE_FF10 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_FF10i {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CFF10(void* self)
{
    if (((*(int*)((char*)self + 0x48) >> 3) & 1) == 0) {
        char* mgr = (char*)SSXApp->RealFE + 0x1A70;
        if (func_0019E238(mgr, *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x8EC)) != 0) {
            func_001CFFF8((sSelf001CFFF8*)self, func_0019E2B0(mgr, *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x8EC)));
            func_0019DC20(mgr, *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x8EC));
            *(int*)((char*)self + 0x8EC) = -1;
            *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~4) | 8;
            if ((*(int*)((char*)self + 0x48) >> 1) & 1) {
                *(int*)((char*)self + 0x8FC) = *(int*)((char*)self + 0x924);
                sVE_FF10* refreshEntry = &(*(sVE_FF10**)((char*)self + 8))[40];
                refreshEntry->fn((char*)self + refreshEntry->delta);
                sVE_FF10i* helpTextEntry = &(*(sVE_FF10i**)((char*)self + 8))[33];
                helpTextEntry->fn((char*)self + helpTextEntry->delta, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001CFFF8);
#ifdef SKIP_ASM
struct cGame001CFFF8 {
    char pad[0x10D8];
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
    virtual int v46(void* name, void* entry, int a, int b, int c);
};

extern "C" void SHAPE_name(void* data, int i, void* entry);
struct sEntry001CFFF8 {
    char name[5];
    int handle;
};
struct sDataEnt001CFFF8 {
    int off;
    int f4;
};
struct sData001CFFF8 {
    int f0;
    int f4;
    int count;
    int fC;
    int f10;
    sDataEnt001CFFF8 tbl[1];
};
struct sSelf001CFFF8 {
    char pad[0x78];
    int count;
    sEntry001CFFF8 entries[1];
};

extern "C" void func_001CFFF8(sSelf001CFFF8* self, sData001CFFF8* data)
{
    if (data == 0) {
        self->count = 0;
        return;
    }
    self->count = data->count;
    int i;
    for (i = 0; i < self->count; i++) {
        SHAPE_name(data, i, &self->entries[i]);
        self->entries[i].name[4] = 0;
        self->entries[i].handle = ((cGame001CFFF8*)GrMan)->v46((char*)data + data->tbl[i].off, &self->entries[i], 0, 1, -1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D00C0);
#ifdef SKIP_ASM
struct cGame001D00C0 {
    char pad[0x10D8];
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
    virtual void v50(int id);
};

extern "C" void func_0019DC20(void* self, int bank, int i);
struct sEntry001D00C0 {
    char name[5];
    int handle;
};
struct sSelf001D00C0 {
    char pad[0x78];
    int count;
    sEntry001D00C0 entries[1];
};

extern "C" void func_001D00C0(sSelf001D00C0* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->entries[i].handle >= 0) {
            ((cGame001D00C0*)GrMan)->v50(self->entries[i].handle);
        }
        self->entries[i].handle = -1;
        self->entries[i].name[0] = 0;
    }
    int h = *(int*)((char*)self + 0x8EC);
    if (h >= 0) {
        func_0019DC20((char*)SSXApp->RealFE + 0x1A70, *(int*)((char*)self + 0x50), h);
        *(int*)((char*)self + 0x8EC) = -1;
    }
    *(int*)((char*)self + 0x48) &= ~8;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0180);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00156A38(void* iface);
extern "C" void func_00156A10(void* iface);
struct sVE_0180 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_001D0180(void* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
    if (func_00156A38(iface) == 0) {
        *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~2) | 1;
        func_00156A10(iface);
    } else {
        *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~1) | 2;
        sVE_0180* vt = *(sVE_0180**)((char*)self + 8);
        vt[28].fn((char*)self + vt[28].delta);
        *(int*)((char*)self + 0x60) = (*(int*)((char*)self + 0x68) + *(int*)((char*)self + 0x5C) - 1) / *(int*)((char*)self + 0x5C);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0238);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00156A38(void* iface);
struct sVE_0238a {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVE_0238b {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_0238c {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D0238(void* self)
{
    if (((*(int*)((char*)self + 0x48) >> 1) & 1) == 0) {
        if (func_00156A38(cBE_getInterface_Fv(cBE_getBE(), 13)) != 0) {
            *(int*)((char*)self + 0x48) = (*(int*)((char*)self + 0x48) & ~1) | 2;
            sVE_0238a* vt = *(sVE_0238a**)((char*)self + 8);
            vt[28].fn((char*)self + vt[28].delta);
            *(int*)((char*)self + 0x60) = (*(int*)((char*)self + 0x68) + *(int*)((char*)self + 0x5C) - 1) / *(int*)((char*)self + 0x5C);
            if ((*(int*)((char*)self + 0x48) >> 3) & 1) {
                *(int*)((char*)self + 0x8FC) = *(int*)((char*)self + 0x924);
                sVE_0238b* vt2 = *(sVE_0238b**)((char*)self + 8);
                vt2[40].fn((char*)self + vt2[40].delta);
                sVE_0238c* vt3 = *(sVE_0238c**)((char*)self + 8);
                vt3[33].fn((char*)self + vt3[33].delta, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0320);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00157F20(void* iface, int player, int a2, int a3, int a4, int a5);
extern "C" int cBEEconInterface_getMoney(void* iface, int a1, int charID);
struct sVE_0320a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_0320b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D0320(void* self, int rewardIndex)
{
    func_00157F20(cBE_getInterface_Fv(cBE_getBE(), 13), *(signed char*)((char*)self + 0x44),
                  *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x4C), rewardIndex, 1);
    *(int*)((char*)self + 0x54) = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), *(signed char*)((char*)self + 0x44),
                                                *(int*)((char*)self + 0x50));
    sVE_0320a* vt = *(sVE_0320a**)((char*)self + 8);
    vt[42].fn((char*)self + vt[42].delta);
    sVE_0320b* vt2 = *(sVE_0320b**)((char*)self + 8);
    vt2[33].fn((char*)self + vt2[33].delta, rewardIndex);
    sVE_0320a* vt3 = *(sVE_0320a**)((char*)self + 8);
    vt3[43].fn((char*)self + vt3[43].delta);
    sVE_0320a* vt4 = *(sVE_0320a**)((char*)self + 8);
    vt4[45].fn((char*)self + vt4[45].delta);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_showReward);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001D1AA0(void* self, void* engine, void* owner, int player);
extern "C" void func_001D20D8(void* self, int kind, void* data);
extern "C" void* func_001D22F0(void* self, void* engine, void* owner, signed char idx);
extern "C" void func_001D2598(void* self, const char* a, const char* b);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern char str_cFEStatePreviewReward[];
extern char str_cFEStateVideoPlayer[];

struct sVE_03F8 {
    short delta;
    short index;
    void* (*fn)(void*);
};

/**
 * Opens the viewer for the selected reward as a popup: a preview for posters, cards, art and toys, a video
 * player for videos.
 *
 * The reward comes from vtable slot 34 (probably getReward). Types 3, 4, 5 and 7 push a
 * cFEStatePreviewReward; type
 * 6 pushes a cFEStateVideoPlayer playing the reward's file. Other types show nothing.
 */
extern "C" void cFEStateRewardGalleryBase_showReward(void* self)
{
    void* nextState = 0;
    sVE_03F8* vt = *(sVE_03F8**)((char*)self + 8);
    void* data = vt[34].fn((char*)self + vt[34].delta);
    switch (*(int*)((char*)self + 0x4C)) {
    case 3:
    case 4:
    case 5:
    case 7:
        nextState = func_001D1AA0(cMemMan_alloc(0x78, str_cFEStatePreviewReward, 0, 0), *(void**)((char*)self + 0x10), self,
                          *(signed char*)((char*)self + 0x44));
        func_001D20D8(nextState, *(int*)((char*)self + 0x4C), data);
        break;
    case 6:
        nextState = func_001D22F0(cMemMan_alloc(640, str_cFEStateVideoPlayer, 0, 0), *(void**)((char*)self + 0x10), self,
                          *(signed char*)((char*)self + 0x44));
        func_001D2598(nextState, *(const char**)((char*)data + 4), 0);
        break;
    }
    if (nextState != 0) {
        cUIStateStack_pushPopup((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0510);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* self, void* engine, void* owner, signed char idx);
extern "C" void func_001CAA18(void* self, int a1, int a2, int a3, int a4);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern char str_cUIStateBuyPopup[];
struct sVE_0510a {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVE_0510b {
    short delta;
    short index;
    int (*fn)(void*, int);
};

extern "C" void func_001D0510(void* self)
{
    sVE_0510a* vt = *(sVE_0510a**)((char*)self + 8);
    int sel = vt[34].fn((char*)self + vt[34].delta);
    char* popup = (char*)cUIStateBuyPopup_cUIStateBuyPopup(cMemMan_alloc(0x70, str_cUIStateBuyPopup, 0, 0), *(void**)((char*)self + 0x10), self,
                                       *(signed char*)((char*)self + 0x44));
    sVE_0510b* vt2 = *(sVE_0510b**)((char*)self + 8);
    int rewardName = vt2[37].fn((char*)self + vt2[37].delta, sel);
    sVE_0510b* vt3 = *(sVE_0510b**)((char*)self + 8);
    int cost = vt3[35].fn((char*)self + vt3[35].delta, sel);
    func_001CAA18(popup + 0x48, *(int*)((char*)self + 0x4C), rewardName, cost, *(int*)((char*)self + 0x54));
    cUIStateStack_pushPopup((char*)*(void**)((char*)self + 0x10) + 0x18, popup);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D05F0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00157468(void* iface, int a, int b, int c, int d);

extern "C" void func_001D05F0(void* self, int rewardIndex)
{
    func_00157468(cBE_getInterface_Fv(cBE_getBE(), 13), *(signed char*)((char*)self + 0x44),
                  *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x4C), rewardIndex);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateHelpText);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_kT_129HELPSelectCheatChar[];
extern char str_kT_129HELPBuyInPeak1[];
extern char str_kT_129HELPBuyInPeak2[];
extern char str_kT_129HELPBuyInPeak3[];
extern char str_kT_129HELPBuyItem[];
extern char str_kT_129HELPBuyInCTM[];
extern char str_kT_129HELPViewItem[];
extern char str_kT_129HELPSaveCashItem[];

struct sVE_0648v {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVE_0648i {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sVE_0648p {
    short delta;
    short index;
    char* (*fn)(void*, int);
};

static inline void setVis_0648(char* o, int on)
{
    sVE_0648v* vt = *(sVE_0648v**)(o + 8);
    vt[9].fn(o + vt[9].delta, on);
}

static inline void setHelp_0648(char* self, char* key)
{
    cUIText_setUnicodeStringByID(*(cUIText**)(self + 0x900), GetHashValue32(key));
}

/**
 * Shows the help line for a reward: view or select it if owned, else where it can be bought and whether cash
 * suffices.
 *
 * Owned rewards show kT_129HELPViewItem (cheat characters: kT_129HELPSelectCheatChar). Otherwise, outside a
 * store
 * (+0x8F0 < 0) it says buy in CTM; in another peak's store, buy in peak 1-3; in its own store, buy or save
 * cash
 * (cash at +0x54 against the cost). A cheat character with no store number shows the reward's own text.
 *
 * @param idx  reward index (On Tour rIndex)
 */
// PORT: item pointers passed through int vtable parameters.
extern "C" void cFEStateRewardGalleryBase_updateHelpText(char* self, int idx)
{
    char* txt = *(char**)(self + 0x900);
    if (txt == 0)
        return;
    setVis_0648(txt, 0);
    sVE_0648p* vtp = *(sVE_0648p**)(self + 8);
    char* item = vtp[34].fn(self + vtp[34].delta, idx);
    if (item == 0)
        return;
    sVE_0648i* vti = *(sVE_0648i**)(self + 8);
    int state = vti[36].fn(self + vti[36].delta, (int)item);
    setVis_0648(*(char**)(self + 0x900), 1);
    if (*(int*)(self + 0x4C) == 8) {
        sVE_0648i* vt = *(sVE_0648i**)(self + 8);
        if (vt[38].fn(self + vt[38].delta, idx)) {
            setHelp_0648(self, str_kT_129HELPSelectCheatChar);
        } else if (state < 0) {
            setHelp_0648(self, *(char**)(item + 8));
        } else {
            int selected = *(int*)(self + 0x8F0);
            if (selected >= 0) {
                if (state != selected) {
                    if (state == 1)
                        setHelp_0648(self, str_kT_129HELPBuyInPeak1);
                    else if (state == 2)
                        setHelp_0648(self, str_kT_129HELPBuyInPeak2);
                    else
                        setHelp_0648(self, str_kT_129HELPBuyInPeak3);
                } else {
                    sVE_0648i* vtbl = *(sVE_0648i**)(self + 8);
                    if (*(int*)(self + 0x54) < vtbl[35].fn(self + vtbl[35].delta, (int)item))
                        setHelp_0648(self, str_kT_129HELPSaveCashItem);
                    else
                        setHelp_0648(self, str_kT_129HELPBuyItem);
                }
            } else {
                setHelp_0648(self, str_kT_129HELPBuyInCTM);
            }
        }
    } else {
        sVE_0648i* vt = *(sVE_0648i**)(self + 8);
        if (vt[38].fn(self + vt[38].delta, idx)) {
            setHelp_0648(self, str_kT_129HELPViewItem);
        } else {
            int selected = *(int*)(self + 0x8F0);
            if (selected >= 0) {
                if (state != selected) {
                    if (state == 1)
                        setHelp_0648(self, str_kT_129HELPBuyInPeak1);
                    else if (state == 2)
                        setHelp_0648(self, str_kT_129HELPBuyInPeak2);
                    else
                        setHelp_0648(self, str_kT_129HELPBuyInPeak3);
                } else {
                    sVE_0648i* vtbl = *(sVE_0648i**)(self + 8);
                    if (*(int*)(self + 0x54) < vtbl[35].fn(self + vtbl[35].delta, (int)item))
                        setHelp_0648(self, str_kT_129HELPSaveCashItem);
                    else
                        setHelp_0648(self, str_kT_129HELPBuyItem);
                }
            } else {
                setHelp_0648(self, str_kT_129HELPBuyInCTM);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateRow);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);
extern char str_dollar_sign[];
extern char str_questmark[];
extern char D_004C6818[];

struct sVEv_08A0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEp_08A0 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
struct sVEi_08A0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sGalCell_08A0 {
    char pad0[0x8];
    void* vt;                   // 0x8
    char padC[0x78 - 0xC];
    int reward;                 // 0x78
    void* tex;                  // 0x7C
};

struct sGallery_08A0 {
    char pad0[0x8];
    sVEi_08A0* vt;              // 0x8
    char padC[0x10 - 0xC];
    char* ui;                   // 0x10
    char pad14[0x5C - 0x14];
    int cols;                   // 0x5C
    int f60;
    int scroll;                 // 0x64
    int count;                  // 0x68
    char pad6C[0x8F0 - 0x6C];
    int selected;               // 0x8F0
    char pad8F4[0x924 - 0x8F4];
    sGalCell_08A0* cells[4][8];  // 0x924
    sGalCell_08A0* frames[4][8]; // 0x9A4
};

static inline void show_08A0(sGalCell_08A0* c, int on)
{
    sVEv_08A0* vt = (sVEv_08A0*)c->vt;
    vt[9].fn((char*)c + vt[9].delta, on);
}

/**
 * Refreshes one row of the reward grid: owned rewards show their icon, others a dollar sign or a question
 * mark.
 *
 * The dollar sign marks rewards sold in the current store. Cells past the last reward are hidden.
 *
 * @param row  grid row on screen (On Tour rowNum)
 */
extern "C" void cFEStateRewardGalleryBase_updateRow(sGallery_08A0* self, int row)
{
    int idx = (self->scroll + row) * self->cols;
    for (int col = 0; col < self->cols; col++, idx++) {
        if (idx < self->count) {
            show_08A0(self->cells[row][col], 1);
            show_08A0(self->frames[row][col], 1);
            if (self->vt[38].fn((char*)self + self->vt[38].delta, idx) != 0) {
                int rewardIndex = self->vt[26].fn((char*)self + self->vt[26].delta, idx);
                if (rewardIndex >= 0) {
                    sGalCell_08A0* cell = self->cells[row][col];
                    cell->reward = rewardIndex;
                    cell->tex = 0;
                } else {
                    int texture = self->vt[27].fn((char*)self + self->vt[27].delta, idx);
                    if (texture != 0) {
                        sGalCell_08A0* cell = self->cells[row][col];
                        cell->tex = (void*)texture;
                        cell->reward = -1;
                    }
                }
            } else {
                int rewardItem = self->vt[34].fn((char*)self + self->vt[34].delta, idx);
                void* icon;
                if (self->selected >= 0 && self->vt[36].fn((char*)self + self->vt[36].delta, rewardItem) == self->selected) {
                    char* ui = self->ui;
                    int iconHash = GetHashValue32(str_dollar_sign);
                    icon = cUITextureBank_getElementByHashname(ui + 0x58, iconHash);
                } else {
                    char* ui = self->ui;
                    int iconHash = GetHashValue32(str_questmark);
                    icon = cUITextureBank_getElementByHashname(ui + 0x58, iconHash);
                }
                sGalCell_08A0* cell = self->cells[row][col];
                cell->tex = icon;
                cell->reward = -1;
            }
            sGalCell_08A0* cell = self->cells[row][col];
            sVEp_08A0* vt = (sVEp_08A0*)cell->vt;
            vt[11].fn((char*)cell + vt[11].delta, D_004C6818);
        } else {
            show_08A0(self->cells[row][col], 0);
            show_08A0(self->frames[row][col], 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0B00);
#ifdef SKIP_ASM
struct sVE_0B00a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_0B00b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D0B00(void* self)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x58); i++) {
        sVE_0B00b* vt = *(sVE_0B00b**)((char*)self + 8);
        vt[39].fn((char*)self + vt[39].delta, i);
    }
    sVE_0B00a* vt1 = *(sVE_0B00a**)((char*)self + 8);
    vt1[46].fn((char*)self + vt1[46].delta);
    sVE_0B00a* vt2 = *(sVE_0B00a**)((char*)self + 8);
    vt2[45].fn((char*)self + vt2[45].delta);
    sVE_0B00a* vt3 = *(sVE_0B00a**)((char*)self + 8);
    vt3[44].fn((char*)self + vt3[44].delta);
    sVE_0B00a* vt4 = *(sVE_0B00a**)((char*)self + 8);
    vt4[41].fn((char*)self + vt4[41].delta);
    sVE_0B00a* vt5 = *(sVE_0B00a**)((char*)self + 8);
    vt5[42].fn((char*)self + vt5[42].delta);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updatePageNumber);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern char str_kT_OVRCMNPageOf[];
extern char str_R1_ps2[];
extern char str_L1_ps2[];
extern char D_004A1408[];

struct sVEK1D0BC8a {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};
struct sVEK1D0BC8b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Updates the "page X of Y" text and shows the L1/R1 page buttons only when there is more than one page.
 *
 * Clears the text when the localized format kT_OVRCMNPageOf is missing.
 */
extern "C" void cFEStateRewardGalleryBase_updatePageNumber(void* self)
{
    if (*(void**)((char*)self + 0x904) == 0)
        return;
    char* obj = *(char**)(*(char**)(*(char**)(*(char**)((char*)self + 0x40) + 0xD0) + 0x10) + 0x10);
    if (obj != 0) {
        sVEK1D0BC8a* vt = *(sVEK1D0BC8a**)(obj + 4);
        char* thisp = obj + vt[4].delta;
        unsigned short* fmt = vt[4].fn(thisp, GetHashValue32(str_kT_OVRCMNPageOf));
        if (fmt != 0) {
            unsigned short buf[100];
            int per = *(int*)((char*)self + 0x58);
            int pages = (*(int*)((char*)self + 0x60) + per - 1) / per;
            sprintf1(buf, fmt, (*(int*)((char*)self + 0x64) + per - 1) / per + 1, pages);
            cUIText_setUnicodeString(*(void**)((char*)self + 0x904), buf);
            char* buttonPrompt = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_R1_ps2));
            if (buttonPrompt != 0) {
                sVEK1D0BC8b* vt2 = *(sVEK1D0BC8b**)(buttonPrompt + 8);
                vt2[9].fn(buttonPrompt + vt2[9].delta, pages > 1);
            }
            buttonPrompt = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_L1_ps2));
            if (buttonPrompt != 0) {
                sVEK1D0BC8b* vt2 = *(sVEK1D0BC8b**)(buttonPrompt + 8);
                vt2[9].fn(buttonPrompt + vt2[9].delta, pages > 1);
            }
            return;
        }
    }
    cUIText_setAsciiString(*(cUIText**)((char*)self + 0x904), D_004A1408);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0D30);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* getDollarString(void* a0);
// PORT: getDollarString really takes the int value to format; bound by asm label
extern "C" const char* getDollarString_i(int v) __asm__("getDollarString");
extern char D_004A1408[];

struct sVEK1D0D30a {
    short delta;
    short index;
    void* (*fn)(void*, int);
};
struct sVEK1D0D30b {
    short delta;
    short index;
    const char* (*fn)(void*, void*);
};
struct sVEK1D0D30c {
    short delta;
    short index;
    int (*fn)(void*, void*);
};
struct sVEK1D0D30d {
    short delta;
    short index;
    int (*fn)(void*, int);
};

extern "C" void func_001D0D30(void* self)
{
    char* grid = *(char**)((char*)self + 0x8FC);
    if (grid == 0) {
        if (*(cUIText**)((char*)self + 0x908) != 0)
            cUIText_setAsciiString(*(cUIText**)((char*)self + 0x908), D_004A1408);
        if (*(cUIText**)((char*)self + 0x90C) != 0)
            cUIText_setAsciiString(*(cUIText**)((char*)self + 0x90C), D_004A1408);
        return;
    }
    int idx = *(int*)((char*)self + 0x64) * *(int*)((char*)self + 0x5C) + *(int*)(grid + 0x18);
    sVEK1D0D30a* vt = *(sVEK1D0D30a**)((char*)self + 8);
    void* item = vt[34].fn((char*)self + vt[34].delta, idx);
    if (*(cUIText**)((char*)self + 0x908) != 0) {
        sVEK1D0D30b* vt2 = *(sVEK1D0D30b**)((char*)self + 8);
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x908), vt2[37].fn((char*)self + vt2[37].delta, item));
    }
    if (*(cUIText**)((char*)self + 0x90C) != 0) {
        sVEK1D0D30c* vt3 = *(sVEK1D0D30c**)((char*)self + 8);
        int price = vt3[35].fn((char*)self + vt3[35].delta, item);
        if (price > 0) {
            sVEK1D0D30d* vt4 = *(sVEK1D0D30d**)((char*)self + 8);
            if (vt4[38].fn((char*)self + vt4[38].delta, idx) == 0) {
                cUIText_setAsciiString(*(cUIText**)((char*)self + 0x90C), getDollarString_i(price));
                return;
            }
        }
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x90C), D_004A1408);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0E78);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* getDollarString(void* a0);

extern "C" void func_001D0E78(void* self)
{
    if (*(cUIText**)((char*)self + 0x910) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x910), getDollarString(*(void**)((char*)self + 0x54)));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0EB8);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_d_b[];

extern "C" void func_001D0EB8(void* self)
{
    char buf[32];
    if (*(cUIText**)((char*)self + 0x918) != 0) {
        sprintf(buf, str_pct_d_b, *(int*)((char*)self + 0x6C));
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x918), buf);
    }
    if (*(cUIText**)((char*)self + 0x914) != 0) {
        sprintf(buf, str_pct_d_b, *(int*)((char*)self + 0x68));
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x914), buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0F30);
#ifdef SKIP_ASM
extern char D_004C6808[];

struct sVEK1D0F30a {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEK1D0F30b {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sVEK1D0F30c {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void func_001D0F30(void* self)
{
    char* icon = *(char**)((char*)self + 0x8F8);
    if (icon != 0) {
        if (*(char**)((char*)self + 0x8FC) != 0) {
            sVec3_F168 a;
            sVec3_F168 b;
            func_003A0290(*(char**)((char*)self + 0x8FC), &a);
            func_003A0290(*(char**)((char*)self + 0x8F8), &b);
            b.y = a.y + *(float*)((char*)self + 0x74);
            b.x = a.x + *(float*)((char*)self + 0x70);
            *(sVec3_F168*)(*(char**)((char*)self + 0x8F8) + 0x44) = b;
            char* o = *(char**)((char*)self + 0x8F8);
            sVEK1D0F30a* vt = *(sVEK1D0F30a**)(o + 8);
            vt[9].fn(o + vt[9].delta, 1);
        } else {
            sVEK1D0F30a* vt = *(sVEK1D0F30a**)(icon + 8);
            vt[9].fn(icon + vt[9].delta, 0);
        }
    }
    char* grid = *(char**)((char*)self + 0x8FC);
    if (grid == 0)
        return;
    int idx = *(int*)((char*)self + 0x64) * *(int*)((char*)self + 0x5C) + *(int*)(grid + 0x18);
    sVEK1D0F30b* vt = *(sVEK1D0F30b**)((char*)self + 8);
    if (vt[38].fn((char*)self + vt[38].delta, idx) != 0) {
        sVEK1D0F30b* vt2 = *(sVEK1D0F30b**)((char*)self + 8);
        int r = vt2[26].fn((char*)self + vt2[26].delta, idx);
        if (r < 0) {
            sVEK1D0F30b* vt3 = *(sVEK1D0F30b**)((char*)self + 8);
            int t = vt3[27].fn((char*)self + vt3[27].delta, idx);
            char* g = *(char**)((char*)self + 0x8FC);
            *(int*)(g + 0x78) = -1;
            *(int*)(g + 0x7C) = t;
        } else {
            char* g = *(char**)((char*)self + 0x8FC);
            *(int*)(g + 0x78) = r;
            *(int*)(g + 0x7C) = 0;
        }
    }
    char* g = *(char**)((char*)self + 0x8FC);
    sVEK1D0F30c* vt4 = *(sVEK1D0F30c**)(g + 8);
    vt4[11].fn(g + vt4[11].delta, D_004C6808);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D10A8);
#ifdef SKIP_ASM
struct sVEntry001D10A8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D10A8(void* self)
{
    void* obj = *(void**)((char*)self + 0x91C);
    if (obj != 0) {
        sVEntry001D10A8* vt = *(sVEntry001D10A8**)((char*)obj + 8);
        vt[9].fn((char*)obj + vt[9].delta, *(int*)((char*)self + 0x64) > 0);
    }
    void* obj2 = *(void**)((char*)self + 0x920);
    if (obj2 != 0) {
        sVEntry001D10A8* vt = *(sVEntry001D10A8**)((char*)obj2 + 8);
        vt[9].fn((char*)obj2 + vt[9].delta,
                 *(int*)((char*)self + 0x64) + *(int*)((char*)self + 0x58) < *(int*)((char*)self + 0x60));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1128);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_0046A248[];

extern "C" void* func_001D1128(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 3);
    *(void***)((char*)self + 0x8) = D_0046A248;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 42; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_00467070[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1198__FPv);
#ifdef SKIP_ASM
void* func_001D1198(void* self)
{
    return (void*)D_00467070;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D11A8);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D11A8(void* self, int i)
{
    char buf[5];
    void* reward = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)reward + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D11F8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_001572B0(void* self);
extern "C" void* cBERewardInterface_getDB();

struct sRewardList_11F8 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D11F8(sRewardList_11F8* self)
{
    int n = func_001572B0(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->count = 0;
    self->base = *(int*)((char*)cBERewardInterface_getDB() + 0x14);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1290);
#ifdef SKIP_ASM
extern "C" void func_001D1290(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 42; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D12C8);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_0046A0C8[];

extern "C" void* func_001D12C8(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 4);
    *(void***)((char*)self + 0x8) = D_0046A0C8;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 115; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_00467098[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1330__FPv);
#ifdef SKIP_ASM
void* func_001D1330(void* self)
{
    return (void*)D_00467098;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1340);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1340(void* self, int i)
{
    char buf[5];
    void* reward = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)reward + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1390);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_001572D0(void* self);
extern "C" void* cBERewardInterface_getDB();

struct sRewardList_1390 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1390(sRewardList_1390* self)
{
    int n = func_001572D0(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->count = 0;
    self->base = *(int*)((char*)cBERewardInterface_getDB() + 0x18);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1428);
#ifdef SKIP_ASM
extern "C" void func_001D1428(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 115; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1460);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469F48[];

extern "C" void* func_001D1460(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 5);
    *(void***)((char*)self + 0x8) = D_00469F48;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 99; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_004670C8[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D14C8__FPv);
#ifdef SKIP_ASM
void* func_001D14C8(void* self)
{
    return (void*)D_004670C8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D14D8);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D14D8(void* self, int i)
{
    char buf[5];
    void* reward = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)reward + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1528);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_001572F0(void* self);
extern "C" void* cBERewardInterface_getDB();

struct sRewardList_1528 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1528(sRewardList_1528* self)
{
    int n = func_001572F0(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->count = 0;
    self->base = *(int*)((char*)cBERewardInterface_getDB() + 0x1C);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D15C0);
#ifdef SKIP_ASM
extern "C" void func_001D15C0(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 99; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D15F8);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469DC8[];

extern "C" void* func_001D15F8(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 6);
    *(void***)((char*)self + 0x8) = D_00469DC8;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 1; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_004670F0[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1660__FPv);
#ifdef SKIP_ASM
void* func_001D1660(void* self)
{
    return (void*)D_004670F0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1670);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1670(void* self, int i)
{
    char buf[5];
    void* reward = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)reward + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D16C0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00157330(void* self);
extern "C" void* cBERewardInterface_getDB();

struct sRewardList_16C0 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D16C0(sRewardList_16C0* self)
{
    int n = func_00157330(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->count = 0;
    self->base = *(int*)((char*)cBERewardInterface_getDB() + 0x24);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1758);
#ifdef SKIP_ASM
extern "C" void func_001D1758(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 1; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1790);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469C48[];

extern "C" void* func_001D1790(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 7);
    *(void***)((char*)self + 0x8) = D_00469C48;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 27; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

extern void* D_00467118[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D17F8__FPv);
#ifdef SKIP_ASM
void* func_001D17F8(void* self)
{
    return (void*)D_00467118;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1808);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1808(void* self, int i)
{
    char buf[5];
    void* reward = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)reward + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1858);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00157310(void* self);
extern "C" void* cBERewardInterface_getDB();

struct sRewardList_1858 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1858(sRewardList_1858* self)
{
    int n = func_00157310(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->count = 0;
    self->base = *(int*)((char*)cBERewardInterface_getDB() + 0x20);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D18F0);
#ifdef SKIP_ASM
extern "C" void func_001D18F0(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 27; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1928);
#ifdef SKIP_ASM
extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a3, int kind);
extern void* D_00469AC8[];

extern "C" void* func_001D1928(void* self, void* engine, signed char idx, int a3)
{
    func_001CEEA0(self, engine, idx, a3, 8);
    *(void***)((char*)self + 0x8) = D_00469AC8;
    *(int*)((char*)self + 0xA24) = 0;
    for (int i = 27; i >= 0; i--) {
        ((void**)((char*)self + 0xA28))[i] = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1990);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00157370(void* self);
extern "C" void* cBERewardInterface_getDB();

struct sRewardList_1990 {
    char pad_0x0[0x68];
    int count;
    char pad_0x6C[0xA24 - 0x6C];
    int base;
    int items[1];
};

extern "C" void func_001D1990(sRewardList_1990* self)
{
    int n = func_00157370(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->count = 0;
    self->base = *(int*)((char*)cBERewardInterface_getDB() + 0x28);
    for (int i = 0; i < n; i++) {
        self->items[self->count++] = self->base + i * 16;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1A28);
#ifdef SKIP_ASM
extern "C" void func_001D1A28(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 27; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1A60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);

extern "C" void func_001D1A60(void* self, int i)
{
    char* obj = *(char**)((char*)self + 0x10);
    void* reward = *(void**)((char*)self + (i << 2) + 0xA28);
    int textureHash = GetHashValue32(*(char**)((char*)reward + 4));
    cUITextureBank_getElementByHashname(obj + 0x58, textureHash);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1AA0);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int player);
extern void* cFEStatePreviewReward_vtbl[];
struct sRewardState_1AA0 {
    char pad_0x0[0x48];
    int a[3];   // 0x48
    int b[3];   // 0x54
};

extern "C" void* func_001D1AA0(void* self, void* engine, void* owner, int player)
{
    signed char idx = player;
    int i;
    func_0039E318(self, engine, owner);
    *(int*)((char*)self + 0xC) = 0x30;
    *(void***)((char*)self + 0x8) = cFEStatePreviewReward_vtbl;
    *(signed char*)((char*)self + 0x44) = idx;
    *(unsigned char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    for (i = 0; i < 3; i++) {
        ((sRewardState_1AA0*)self)->a[i] = 0;
        ((sRewardState_1AA0*)self)->b[i] = 0;
    }
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1),
                                                                         *(signed char*)((char*)self + 0x44));
    *(int*)((char*)self + 0x68) = -1;
    *(int*)((char*)self + 0x6C) = -1;
    *(int*)((char*)self + 0x70) = -1;
    *(int*)((char*)self + 0x74) = -1;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward__cFEStatePreviewReward);
#ifdef SKIP_ASM
struct sVEntry001D1B68 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern void* cFEStatePreviewReward_vtbl[];
extern void* D_0046D0D0[];
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void func_0039E390(void* self, int flags);

/**
 * Destroys a reward preview: frees its texture and releases its file slot (func_0019DC20), then the base
 * state.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStatePreviewReward__cFEStatePreviewReward(void* self, int flags)
{
    *(void***)((char*)self + 8) = cFEStatePreviewReward_vtbl;
    int id = *(int*)((char*)self + 0x70);
    if (id >= 0) {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntry001D1B68* vt = (sVEntry001D1B68*)ps2GraphicsMan->mVtbl;
        vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, id);
    }
    int slot = *(int*)((char*)self + 0x6C);
    if (slot >= 0) {
        func_0019DC20((char*)SSXApp->RealFE + 0x1A70, *(int*)((char*)self + 0x64), slot);
    }
    *(int*)((char*)self + 0x6C) = -1;
    *(int*)((char*)self + 0x70) = -1;
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_130rewardposter[];
extern char str_Menu[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

/**
 * Creates the reward preview screen ("130rewardposter") and sets flag 8 on its "Menu" object.
 */
extern "C" void cFEStatePreviewReward_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_130rewardposter), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Menu));
    if (obj != 0) {
        *(int*)((char*)obj + 0x90) |= 8;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onWidgetCreate);
#ifdef SKIP_ASM
extern char str_Poster_icon[];
extern char str_poster_icon[];
extern char str_trading_card_icon[];
extern char str_art_icon[];
extern char str_toys_icon[];
extern char str_screentitle[];
extern char str_kT_OVRCMNPosters[];
extern char str_kT_OVRCMNTradeCard[];
extern char str_kT_OVRCMNArt[];
extern char str_kT_OVRCMNVideo[];
extern char str_kT_OVRCMNToys[];
extern char str_reward_name[];
extern char str_Square_shape[];
extern char str_Horizontal_shape[];
extern char str_Vertical_shape[];
extern char str_Square_bitmap[];
extern char str_Horizontal_bitmap[];
extern char str_Vertical_bitmap[];

struct sVE_1C98 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sWidget_1C98 {
    char pad_0x0[8];
    sVE_1C98* vt;       // 0x8
    char pad_0xC[0x38 - 0xC];
    int hash;           // 0x38
    char pad_0x3C[0x78 - 0x3C];
    int f78;            // 0x78
    void* f7C;          // 0x7C
};

struct sPreviewReward_1C98 {
    char pad_0x0[0x10];
    char* ui;                   // 0x10
    char pad_0x14[0x48 - 0x14];
    sWidget_1C98* items[3];     // 0x48
    sWidget_1C98* frames[3];    // 0x54
    char** reward;              // 0x60
    char pad_0x64[0x68 - 0x64];
    int type;                   // 0x68
    char pad_0x6C[0x70 - 0x6C];
    int f70;                    // 0x70
    int sel;                    // 0x74
};

/**
 * Binds the preview screen's widgets as they are created: icon and title by reward type, the name, and the
 * frames.
 *
 * The reward picture goes into the square, horizontal or vertical bitmap chosen by the shape index (+0x70
 * texture),
 * and that frame is shown.
 *
 * @param w  the widget being created, or 0
 */
extern "C" void cFEStatePreviewReward_onWidgetCreate(sPreviewReward_1C98* self, sWidget_1C98* w)
{
    if (w == 0)
        return;

    int hash = w->hash;
    if (hash == GetHashValue32(str_Poster_icon)) {
        switch (self->type) {
        case 3:
            {
                char* ui = self->ui;
                int iconHash = GetHashValue32(str_poster_icon);
                w->f7C = cUITextureBank_getElementByHashname(ui + 0x58, iconHash);
            }
            w->f78 = -1;
            break;
        case 4:
            {
                char* ui = self->ui;
                int iconHash = GetHashValue32(str_trading_card_icon);
                w->f7C = cUITextureBank_getElementByHashname(ui + 0x58, iconHash);
            }
            w->f78 = -1;
            break;
        case 5:
            {
                char* ui = self->ui;
                int iconHash = GetHashValue32(str_art_icon);
                w->f7C = cUITextureBank_getElementByHashname(ui + 0x58, iconHash);
            }
            w->f78 = -1;
            break;
        case 7:
            {
                char* ui = self->ui;
                int iconHash = GetHashValue32(str_toys_icon);
                w->f7C = cUITextureBank_getElementByHashname(ui + 0x58, iconHash);
            }
            w->f78 = -1;
            break;
        }
    } else if (hash == GetHashValue32(str_screentitle)) {
        switch (self->type) {
        case 3:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNPosters));
            break;
        case 4:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNTradeCard));
            break;
        case 5:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNArt));
            break;
        case 6:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNVideo));
            break;
        case 7:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNToys));
            break;
        }
    } else if (hash == GetHashValue32(str_reward_name)) {
        switch (self->type) {
        case 4:
            w->vt[9].fn((char*)w + w->vt[9].delta, 0);
            break;
        case 3:
        case 5:
        case 7:
            cUIText_setAsciiString((cUIText*)w, *self->reward);
            break;
        case 6:
            break;
        }
    } else if (hash == GetHashValue32(str_Square_shape)) {
        self->frames[0] = w;
        w->vt[9].fn((char*)w + w->vt[9].delta, 0);
    } else if (hash == GetHashValue32(str_Horizontal_shape)) {
        self->frames[1] = w;
        w->vt[9].fn((char*)w + w->vt[9].delta, 0);
    } else if (hash == GetHashValue32(str_Vertical_shape)) {
        self->frames[2] = w;
        w->vt[9].fn((char*)w + w->vt[9].delta, 0);
    } else if (hash == GetHashValue32(str_Square_bitmap)) {
        self->items[0] = w;
    } else if (hash == GetHashValue32(str_Horizontal_bitmap)) {
        self->items[1] = w;
    } else if (hash == GetHashValue32(str_Vertical_bitmap)) {
        self->items[2] = w;
    }
    int sel = self->sel;
    if (sel >= 0) {
        sWidget_1C98* item = self->items[sel];
        if (item != 0 && self->frames[sel] != 0) {
            item->f78 = self->f70;
            item->f7C = 0;
            sWidget_1C98* frame = self->frames[self->sel];
            frame->vt[9].fn((char*)frame + frame->vt[9].delta, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Swallows menu events 6, 8 and 9 on the preview screen.
 *
 * @param menu  the menu (On Tour cUIMenu* pMenu), unused
 * @param event  menu event number (On Tour Event)
 * @return 0x100 for events 6, 8 and 9, otherwise 0x101
 */
extern "C" int cFEStatePreviewReward_onWidgetMenu(void* self, int menu, unsigned int event)
{
    switch (event) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onUpdate);
#ifdef SKIP_ASM
extern "C" void func_001D2198(void* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Updates the reward preview, first finishing its preview picture load while one is pending.
 *
 * While the async slot at 0x6C is not negative, func_001D2198() checks the load; once it has loaded, the shape
 * is handed to GrMan, the slot is released and set to -1 and the preview widget shows it. Then runs the base
 * UI-state update cUIState_onUpdate().
 */
extern "C" void cFEStatePreviewReward_onUpdate(void* self)
{
    if (*(int*)((char*)self + 0x6C) >= 0) {
        func_001D2198(self);
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onInputBegin);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0039F190(void* self, int a1);
extern char str_TransitionOut[];
struct sVE_1FF0a {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVE_1FF0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sDefObj_1FF0 {
    sVE_1FF0b* vt;
};
// $gp-relative default object (target has raw `addiu $3,$28,0x2968`, no symbol)
extern sDefObj_1FF0 gUISoundNull;

/**
 * Closes the preview when the input passes its vtable slot-6 test: plays UI sound 3, hides the frames,
 * transitions out.
 *
 * @param obj  the input (On Tour cUIInput* pInput)
 * @return 1 when the preview closes, otherwise 0
 */
extern "C" int cFEStatePreviewReward_onInputBegin(void* self, void* obj)
{
    sVE_1FF0a* vt = *(sVE_1FF0a**)((char*)obj + 8);
    if (vt[6].fn((char*)obj + vt[6].delta) != 0) {
    sDefObj_1FF0* sound = *(sDefObj_1FF0**)(*(char**)((char*)self + 0x10) + 0x14);
    if (sound == 0) {
        sound = &gUISoundNull;
    }
    if (sound != 0) {
        sound->vt[2].fn((char*)sound + sound->vt[2].delta, 3);
    }
    int i;
    for (i = 0; i < 3; i++) {
        char* bitmap = ((char**)((char*)self + 0x48))[i];
        if (bitmap != 0) {
            sVE_1FF0b* vt2 = *(sVE_1FF0b**)(bitmap + 8);
            vt2[9].fn(bitmap + vt2[9].delta, 0);
        }
    }
    cUIScreen_playFrame(*(void**)((char*)self + 0x40), cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(str_TransitionOut)), 1);
    func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D20D8);
#ifdef SKIP_ASM
extern "C" void func_001D2150(void* self, int a1);

extern "C" void func_001D20D8(void* self, int kind, void* data)
{
    *(int*)((char*)self + 0x68) = kind;
    *(void**)((char*)self + 0x60) = data;
    switch (kind) {
    case 3:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    case 4:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    case 5:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    case 7:
        func_001D2150(self, *(int*)((char*)data + 4));
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2150);
#ifdef SKIP_ASM
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);

extern "C" void func_001D2150(void* self, int id)
{
    *(int*)((char*)self + 0x6C) = func_0019DA10((char*)SSXApp->RealFE + 0x1A70, *(int*)((char*)self + 0x64), id, 9, 1, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2198);
#ifdef SKIP_ASM
extern "C" int func_0019E238(void* self, int bank, int i);
extern "C" void func_0019DC20(void* self, int bank, int i);

struct sSelfK1D2198 {
    char pad_0x0[0x48];
    char* a48[3];   // 0x48
    char* a54[3];   // 0x54
    int f60;
    int bank;       // 0x64
    int f68;
    int slot;       // 0x6C
    int handle;     // 0x70
    int kind;       // 0x74
};
struct sVEK1D2198 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D2198(void* self)
{
    char* mgr = (char*)SSXApp->RealFE + 0x1A70;
    if (func_0019E238(mgr, *(int*)((char*)self + 0x64), *(int*)((char*)self + 0x6C)) == 0)
        return;
    sData001CFFF8* data = func_0019E2B0(mgr, *(int*)((char*)self + 0x64), *(int*)((char*)self + 0x6C));
    char* e = (char*)data + data->tbl[0].off;
    short a = *(short*)(e + 4);
    short b = *(short*)(e + 6);
    if (b < a)
        *(int*)((char*)self + 0x74) = 1;
    else if (a < b)
        *(int*)((char*)self + 0x74) = 2;
    else
        *(int*)((char*)self + 0x74) = 0;
    sEntry001CFFF8 ent;
    SHAPE_name(data, 0, &ent);
    ent.name[4] = 0;
    *(int*)((char*)self + 0x70) = ((cGame001CFFF8*)GrMan)->v46(e, &ent, 0, 1, -1);
    func_0019DC20(mgr, *(int*)((char*)self + 0x64), *(int*)((char*)self + 0x6C));
    *(int*)((char*)self + 0x6C) = -1;
    sSelfK1D2198* s = (sSelfK1D2198*)self;
    char* o = s->a48[s->kind];
    if (o != 0 && s->a54[s->kind] != 0) {
        *(int*)(o + 0x78) = s->handle;
        *(int*)(o + 0x7C) = 0;
        char* o2 = s->a54[s->kind];
        sVEK1D2198* vt = *(sVEK1D2198**)(o2 + 8);
        vt[9].fn(o2 + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D22F0);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" void func_00253390(void* p);
extern void* D_00469928[];

// Constructor for the reward popup state: base init, empty title/body strings and text list.
extern "C" void* func_001D22F0(void* vself, void* engine, void* owner, signed char player)
{
    char* self = (char*)vself;
    func_0039E318(self, engine, owner);
    char* title = self + 0x48;
    char* body = self + 0x148;
    *(signed char*)(self + 0x44) = player;
    title[0] = 0;
    body[0] = 0;
    *(int*)(self + 0x274) = 0;
    *(int*)(self + 0x278) = 0;
    *(int*)(self + 0x27C) = 0;
    *(int*)(self + 0x248) = 0;
    *(void***)(self + 0x8) = D_00469928;
    *(int*)(self + 0xC) = 6;
    *(int*)(self + 0x270) = 1;
    func_00253390(self + 0x24C);
    *(char**)(self + 0x24C) = title;
    *(char**)(self + 0x250) = body;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2380);
#ifdef SKIP_ASM
extern "C" void func_00253418(void* p, int a1);
extern "C" void func_0039E390(void* self, int flags);
extern void* D_00469928[];
extern void* D_0046D0D0[];

extern "C" void func_001D2380(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_00469928;
    if (*(void**)((char*)self + 0x248) != 0) {
        func_00253418(*(void**)((char*)self + 0x248), 3);
    }
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D23E0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002533C8(void* mem);
extern "C" void func_001D25E8(void* self);
extern char str_MoviePlayer[];

extern "C" void func_001D23E0(void* self)
{
    *(void**)((char*)self + 0x248) = func_002533C8(cMemMan_alloc(68, str_MoviePlayer, 0, 0));
    func_001D25E8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2430);
#ifdef SKIP_ASM
extern "C" void func_00253418(void* p, int a1);

extern "C" void func_001D2430(void* self)
{
    void* moviePlayer = *(void**)((char*)self + 0x248);
    if (moviePlayer != 0) {
        func_00253418(moviePlayer, 3);
    }
    *(void**)((char*)self + 0x248) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2468);
#ifdef SKIP_ASM
extern "C" void func_00253890(void* p, int a1);
extern "C" void* cUIState_onRender2D(void* self);

extern "C" void func_001D2468(void* self)
{
    void* moviePlayer = *(void**)((char*)self + 0x248);
    if (moviePlayer != 0) {
        *(int*)((char*)self + 0x278) = 1;
        func_00253890(moviePlayer, 0);
    }
    cUIState_onRender2D(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D24A8);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_asyncUpdate(void* p);
extern "C" void func_001D2638(void* self);
extern "C" void* cUIState_onUpdate(void* self);

extern "C" void func_001D24A8(void* self)
{
    void* moviePlayer = *(void**)((char*)self + 0x248);
    if (moviePlayer != 0) {
        *(int*)((char*)self + 0x274) = 1;
        cMoviePlayer_asyncUpdate(moviePlayer);
        if ((*(int*)((char*)self + 0x27C) != 0 && *(int*)((char*)self + 0x278) != 0)
            || **(int**)((char*)self + 0x248) != 0) {
            func_001D2638(self);
        }
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2518);
#ifdef SKIP_ASM
struct sVEntry001D2518 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int vcall001D2518(void* obj, int slot)
{
    sVEntry001D2518* vt = *(sVEntry001D2518**)((char*)obj + 8);
    return vt[slot].fn((char*)obj + vt[slot].delta);
}

extern "C" int func_001D2518(void* self, void* obj)
{
    if (*(int*)((char*)self + 0x270) != 0) {
        if (vcall001D2518(obj, 5) != 0 || vcall001D2518(obj, 6) != 0) {
            *(int*)((char*)self + 0x27C) = 1;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2598);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_001D2598(void* self, const char* videoFile, const char* audioFile)
{
    strcpy((char*)self + 0x48, videoFile);
    if (audioFile != 0) {
        strcpy((char*)self + 0x148, audioFile);
    } else {
        *((char*)self + 0x148) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D25E8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002B3A70(void* p);
extern "C" int func_002534A8(void* p, void* q);
extern "C" void func_00253860(void* p);

extern "C" void func_001D25E8(void* self)
{
    func_002B3A70((char*)func_0028B180() + 0x118);
    *(int*)((char*)self + 0x254) = 1;
    func_002534A8(*(void**)((char*)self + 0x248), (char*)self + 0x24C);
    func_00253860(*(void**)((char*)self + 0x248));
    *(int*)((char*)self + 0x278) = 0;
    *(int*)((char*)self + 0x274) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2638);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002B3A98(void* self);
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001D2638(void* self)
{
    func_002B3A98((char*)func_0028B180() + 0x118);
    *(int*)((char*)self + 0x27C) = 0;
    func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
}
#endif

