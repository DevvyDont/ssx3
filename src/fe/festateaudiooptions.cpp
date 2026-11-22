#include "vtable.h"
#include "common.h"
#include "ui/uistate.h"
#include "ui/uiengine.h"
#include "ui/uiicon.h"
#include "sound/ssxaudio.h"
#include "fe/festateaudiooptions.h"
#include "fe/festatebigradio.h"
#include "ui/uithing.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateAudioOptions_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_0box[];
extern char str_1box[];
extern char str_2box[];
extern char str_3box[];
extern char str_radio_big[];
extern char str_big_mountain[];
extern char str_request_line[];
extern char str_edit_req[];
extern char str_playlist[];

static inline void SetText_6540(void* vw, int text)
{
    cUIIcon* w = (cUIIcon*)vw;
    w->mTextureID = -1;
    w->mpTextureElem = (const cTextureElement*)text;
}

/**
 * Binds the audio options' widgets: the four music-mode boxes, coloured by the current mode, and the menu
 * entries.
 *
 * The entries are numbered 0 radio big, 1 request line, 2 big mountain, 3 playlist and 4 edit requests. The
 * "2box"
 * test appears twice; the second is unreachable.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateAudioOptions_onWidgetCreate(cFEStateAudioOptions* self, cUIThing* w)
{
    int id = w->mHashName;
    if (id == GetHashValue32(str_0box))
    {
        self->mpChecks[0] = (cUIIcon*)((void*)w);
        if (self->mRadioMode == 0)
            SetText_6540(w, (int)self->mpCheckBox);
        else
            SetText_6540(w, (int)self->mpEmptyBox);
    }
    else if (id == GetHashValue32(str_1box))
    {
        self->mpChecks[2] = (cUIIcon*)((void*)w);
        if (self->mRadioMode == 2)
            SetText_6540(w, (int)self->mpCheckBox);
        else
            SetText_6540(w, (int)self->mpEmptyBox);
    }
    else if (id == GetHashValue32(str_2box))
    {
        self->mpChecks[1] = (cUIIcon*)((void*)w);
        if (self->mRadioMode == 1)
            SetText_6540(w, (int)self->mpCheckBox);
        else
            SetText_6540(w, (int)self->mpEmptyBox);
    }
    else if (id == GetHashValue32(str_2box))
    {
        self->mpChecks[1] = (cUIIcon*)((void*)w);
        if (self->mRadioMode == 1)
            SetText_6540(w, (int)self->mpCheckBox);
        else
            SetText_6540(w, (int)self->mpEmptyBox);
    }
    else if (id == GetHashValue32(str_3box))
    {
        self->mpChecks[3] = (cUIIcon*)((void*)w);
        if (self->mRadioMode == 3)
            SetText_6540(w, (int)self->mpCheckBox);
        else
            SetText_6540(w, (int)self->mpEmptyBox);
    }
    else if (id == GetHashValue32(str_radio_big))
    {
        w->mMiscData = 0;
    }
    else if (id == GetHashValue32(str_big_mountain))
    {
        w->mMiscData = 2;
    }
    else if (id == GetHashValue32(str_request_line))
    {
        self->mpRequestLineOption = (cUIText*)((void*)w);
        w->mMiscData = 1;
    }
    else if (id == GetHashValue32(str_edit_req))
    {
        self->mpEditRequestOption = (cUIText*)((void*)w);
        w->mMiscData = 4;
    }
    else if (id == GetHashValue32(str_playlist))
    {
        self->mpPlaylistOption = (cUIText*)((void*)w);
        w->mMiscData = 3;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateAudioOptions_onWidgetEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" cUIText* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void cBERewardInterface_setRadioMode(void* iface, int player, int a2, int a3);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028BF78(void* audio, int a1);
extern "C" void* cFEStateRequestLine_cFEStateRequestLine(cFEStateRequestLine* p, int a1, signed char idx, int a3);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void cUIStateStack_pop(void* stack, int a1);
extern "C" void cUIStateStack_push(void* stack, void* state);
extern char str_cFEStateRequestLine[];
extern char str_helptext[];
extern char str_kT_HELPAUDIORadioBig[];
extern char str_kT_HELPAUDIOBMA[];
extern char str_kT_HELPAUDIOCreateInLodge[];
extern char str_kT_HELPAUDIOCreate[];
extern char str_kT_HELPAUDIOCreateInCTM[];
extern char str_kT_FEHELPCustPlayDJ[];
extern char str_kT_FEHELPCustPlayNoDJ[];
extern char str_kT_HELPAUDIOEditReq[];

struct sVE_AOWE {
    short delta;
    short index;
    void* (*fn)(void*, ...);
};

struct sOption_AOWE {
    char pad_0x0[0x78];
    int state;                      // 0x78
    int color;                      // 0x7C
};

struct sToggle_AOWE {
    char pad_0x0[0x8];
    sVE_AOWE* vt;                   // 0x8
};

struct sAudioOptions_AOWE {
    char pad_0x0[0x10];
    char* owner;                    // 0x10
    char pad_0x14[0x2C];
    void* screen;                   // 0x40
    signed char player;             // 0x44
    char pad_0x45[0x7];
    sOption_AOWE* options[4];       // 0x4C
    int selColor;                   // 0x5C
    int normColor;                  // 0x60
    sToggle_AOWE* toggles[3];       // 0x64
    int f70;                        // 0x70
    int f74;                        // 0x74
    int sel;                        // 0x78
    int f7C;                        // 0x7C
};

struct sWidget_AOWE {
    char pad_0x0[0x18];
    int idx;                        // 0x18
};

static inline int online_AOWE()
{
    return (int)SSXApp->Game;
}

static inline void setOption_AOWE(sOption_AOWE* o, int state, int color)
{
    o->state = state;
    o->color = color;
}

static inline void setText_AOWE(cUIText* t, char* str)
{
    cUIText_setUnicodeStringByID(t, GetHashValue32(str));
}

static inline void applyMode_AOWE(sAudioOptions_AOWE* self)
{
    cBERewardInterface_setRadioMode(cBE_getInterface_Fv(cBE_getBE(), 0xD), self->player, self->f70, self->sel);
    func_0028BF78(cAudioInterface_bxAudio(), self->sel);
}

static inline void* vcall_AOWE(sAudioOptions_AOWE* self, int slot, sWidget_AOWE* w)
{
    char* o = *(char**)self->owner;
    sVE_AOWE* vt = *(sVE_AOWE**)(o + 4);
    return vt[slot].fn(vthis(o, vt[slot]), self, w->idx);
}

static inline void setEnabled_AOWE(sToggle_AOWE* t, int on)
{
    ((void (*)(void*, int))t->vt[6].fn)(vthis(t, t->vt[6]), on);
}

/**
 * Handles the audio options menu: select a music mode, open the request-line editor, leave, or show help
 * text.
 *
 * Selecting a mode recolours the boxes and stores it (per player in the reward interface, and in the audio
 * system);
 * the request-line and playlist modes need a custom list (+0x7C). Entry 4 opens a cFEStateRequestLine.
 * Leaving
 * stores the mode too. Highlight help depends on whether a list exists and whether this is the in-game pause
 * menu.
 *
 * @param w  the menu entry (its index at +0x18), or 0
 * @param event  widget event number (On Tour Event)
 */
extern "C" void cFEStateAudioOptions_onWidgetEvent(sAudioOptions_AOWE* self, sWidget_AOWE* w, unsigned int event)
{
    if (w == 0)
        return;
    switch (event) {
    case 5: {
        int idx = w->idx;
        if (idx != 0 && idx != 2) {
            if (idx != 1 && idx != 3)
                goto other;
            if (self->f7C == 0)
                return;
        }
        setOption_AOWE(self->options[self->sel], -1, self->normColor);
        self->sel = w->idx;
        setOption_AOWE(self->options[self->sel], -1, self->selColor);
        applyMode_AOWE(self);
        break;
    other:
        if (idx != 4)
            return;
        if (self->f74 == 0 && self->f7C == 0)
            return;
        cFEStateRequestLine* popup = (cFEStateRequestLine*)cFEStateRequestLine_cFEStateRequestLine((cFEStateRequestLine*)cMemMan_alloc(528, str_cFEStateRequestLine, 0, 0), (int)self->owner, self->player, self->f74);
        if (online_AOWE()) {
            popup->mAttrState = (popup->mAttrState & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial(self->owner + 0x18, popup, 0, 0);
        } else {
            cUIStateStack_push(self->owner + 0x18, popup);
        }
        break;
    }
    case 6:
        applyMode_AOWE(self);
        if (online_AOWE()) {
            cUIStateStack_pop(self->owner + 0x18, 1);
        } else {
            void* nextState = vcall_AOWE(self, 5, w);
            if (nextState != 0)
                cUIStateStack_push(self->owner + 0x18, nextState);
        }
        break;
    case 1: {
        cUIText* text = cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_helptext));
        if (text == 0)
            return;
        bool isOnline = false;
        bool toggle2Enabled = false;
        bool toggle1Enabled = false;
        switch (w->idx) {
        case 0:
            setText_AOWE(text, str_kT_HELPAUDIORadioBig);
            break;
        case 2:
            setText_AOWE(text, str_kT_HELPAUDIOBMA);
            break;
        case 1:
            if (self->f7C == 0) {
                toggle1Enabled = online_AOWE();
                if (online_AOWE())
                    setText_AOWE(text, str_kT_HELPAUDIOCreateInLodge);
                else if (self->f74 == 0)
                    setText_AOWE(text, str_kT_HELPAUDIOCreateInCTM);
                else
                    setText_AOWE(text, str_kT_HELPAUDIOCreate);
            } else
                setText_AOWE(text, str_kT_FEHELPCustPlayDJ);
            break;
        case 3:
            if (self->f7C == 0) {
                toggle2Enabled = online_AOWE();
                if (online_AOWE())
                    setText_AOWE(text, str_kT_HELPAUDIOCreateInLodge);
                else if (self->f74 == 0)
                    setText_AOWE(text, str_kT_HELPAUDIOCreateInCTM);
                else
                    setText_AOWE(text, str_kT_HELPAUDIOCreate);
            } else
                setText_AOWE(text, str_kT_FEHELPCustPlayNoDJ);
            break;
        case 4:
            if (self->f74 == 0 && self->f7C == 0) {
                isOnline = online_AOWE();
                if (online_AOWE())
                    setText_AOWE(text, str_kT_HELPAUDIOCreateInLodge);
                else
                    setText_AOWE(text, str_kT_HELPAUDIOCreateInCTM);
            } else
                setText_AOWE(text, str_kT_HELPAUDIOEditReq);
            break;
        }
        if (online_AOWE()) {
            setEnabled_AOWE(self->toggles[0], isOnline);
            setEnabled_AOWE(self->toggles[1], toggle1Enabled);
            setEnabled_AOWE(self->toggles[2], toggle2Enabled);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateAudioOptions_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Refuses event 6 on the request-line, big-mountain or edit entries when they are unavailable, and blocks
 * events 8 and 9.
 *
 * @param pMenu  the menu
 * @param Event  menu event number
 * @param index  the menu item index
 * @return 0x10 for a refused event 6, 0 for events 8 and 9, otherwise 0x101
 */
extern "C" int cFEStateAudioOptions_onWidgetMenu(cFEStateAudioOptions* self, int pMenu, int Event, int index)
{
    if (Event == 6 && (index == 2 || index == 3 || index == 4)) {
        if (index == 2 && self->mCreatedRequestList == 0) {
            goto ok;
        }
        if (index == 3 && self->mCreatedRequestList == 0) {
            goto ok;
        }
        if (index == 4 && self->mInStore == 0 && self->mCreatedRequestList == 0) {
        ok:
            return 0x10;
        }
    } else if (Event == 8 || Event == 9) {
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateRequestLine_cFEStateRequestLine);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
int GetHashValue32(char* str);
extern "C" void MEM_fill(void* p, int c, int n);
extern "C" int cUITextureBank_getElementByHashname(void* list, int hash);
extern "C" int cBEEconInterface_getMoney(void* iface, int idx, int ch);
// PORT: 64-bit results (long is 8 bytes on the EE).
extern "C" long cBERewardInterface_getOwnedSongList(void* iface, int idx, int ch);
extern "C" long cBERewardInterface_getPlayList(void* iface, int idx, int ch);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cFEStateRequestLine_generatePlayList(void* self);
extern void* cFEStateRequestLine_vtbl[];
extern char str_checkbox[];
extern char str_empty_box[];
extern char str_yell_dollar_sign[];

struct sAudioState_196B90
{
    char pad0[0x4C];
    char buf[0x100];        // 0x4C
    int f14C;               // 0x14C
    int f150;               // 0x150
    char pad154[4];
    long f158;              // 0x158
    long f160;              // 0x160
    int f168;               // 0x168
    int f16C;               // 0x16C
    int f170;               // 0x170
    int a174[8];            // 0x174
    int a194[8];            // 0x194
    int f1B4;               // 0x1B4
    int f1B8;               // 0x1B8
    int a1BC[6];            // 0x1BC
    int f1D4;               // 0x1D4
    int f1D8;               // 0x1D8
    int f1DC;               // 0x1DC
    int f1E0;               // 0x1E0
    int f1E4;
    int f1E8;
    int f1EC;
    int f1F0;
    int f1F4;
    int f1F8;
    int f1FC;
    int f200;
    int f204;               // 0x204
    int f208;               // 0x208
    int f20C;               // 0x20C
};

static inline int Find_196B90(char* mgr, int hash)
{
    return cUITextureBank_getElementByHashname(mgr + 0x58, hash);
}

/**
 * Constructs a request-line (playlist) state for a user.
 *
 * Runs the cUIState constructor cUIState_cUIState (the engine reaches it in the second argument register), sets the state
 * id (+0xC) to 17, stores cFEStateRequestLine_vtbl and the user (+0x44), and copies the user's control mask from the
 * custom interface (cFECustom_getPlayerCtrlMask) into +0x15 when there is one. Zeroes the 0x100-byte buffer at +0x4C and the fields
 * up to +0x200, keeps @p inStore at +0x16C, finds the "checkbox", "empty_box" and "yell_dollar_sign" texture elements
 * (+0x204, +0x208, +0x20C), reads the user's character (+0x48, cBENewPlayerInterface_getPlayerCharID), money (+0x168,
 * cBEEconInterface_getMoney) and two 64-bit values of BE interface 13 (cBERewardInterface_getOwnedSongList into +0x160, cBERewardInterface_getPlayList into
 * +0x158), copies the audio object's word +0x504 into +0x14C and runs cFEStateRequestLine_generatePlayList.
 *
 * @param requestLine    the state's memory (528 bytes)
 * @param uiEngine   the UI engine (not read here; see above)
 * @param idx  the user
 * @param inStore   nonzero when opened from the store (cFECustom_getNextState passes gInLodge)
 * @return @p requestLine
 */
extern "C" void* cFEStateRequestLine_cFEStateRequestLine(cFEStateRequestLine* requestLine, int uiEngine, signed char idx, int inStore)
{
    sAudioState_196B90* self = (sAudioState_196B90*)requestLine;
    cUIState_cUIState(self);
    *(int*)((char*)self + 0xC) = 17;
    *(void***)((char*)self + 0x8) = cFEStateRequestLine_vtbl;
    *(signed char*)((char*)self + 0x44) = idx;
    if (**(void***)((char*)self + 0x10) != 0)
        *(char*)((char*)self + 0x15) = cFECustom_getPlayerCtrlMask(**(void***)((char*)self + 0x10), idx);
    MEM_fill(self->buf, 0, 0x100);
    self->f14C = 0;
    self->f150 = 0;
    self->f170 = 0;
    for (int i = 0; i < 8; i++)
    {
        self->a174[i] = 0;
        self->a194[i] = 0;
    }
    self->f1B4 = 0;
    self->f1B8 = 0;
    for (int i = 5; i >= 0; i--)
        self->a1BC[i] = 0;
    self->f16C = inStore;
    self->f1D4 = 0;
    self->f1DC = 0;
    self->f1D8 = 0;
    self->f1E0 = 0;
    self->f1E4 = 0;
    self->f1E8 = 0;
    self->f1EC = 0;
    self->f1F0 = 0;
    self->f1F4 = 0;
    self->f1F8 = 0;
    self->f1FC = 0;
    self->f200 = 0;
    self->f158 = 0;
    {
        char* mgr = *(char**)((char*)self + 0x10);
        self->f204 = Find_196B90(mgr, GetHashValue32(str_checkbox));
    }
    {
        char* mgr = *(char**)((char*)self + 0x10);
        self->f208 = Find_196B90(mgr, GetHashValue32(str_empty_box));
    }
    {
        char* mgr = *(char**)((char*)self + 0x10);
        self->f20C = Find_196B90(mgr, GetHashValue32(str_yell_dollar_sign));
    }
    *(int*)((char*)self + 0x48) = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    self->f168 = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0x48));
    void* ifc = cBE_getInterface_Fv(cBE_getBE(), 13);
    self->f160 = cBERewardInterface_getOwnedSongList(ifc, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0x48));
    self->f158 = cBERewardInterface_getPlayList(ifc, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0x48));
    self->f14C = cAudioInterface_bxAudio()->MusicSys.mNumFESongs;
    cFEStateRequestLine_generatePlayList(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateRequestLine_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIMenu_setScrollableMenu(void* self, int a1);
extern "C" void* cUIStateStack_findState(void* self, int id);
extern char str_hud[];
extern char str_16radio[];
extern char str_143radio_pda[];


/**
 * Creates the request-line screen: "16radio" in the front end, or "143radio_pda" in game with the HUD told to
 * hide (3).
 *
 * Then makes the song menu scrollable over the song count (+0x14C).
 */
extern "C" void cFEStateRequestLine_onCreateScreen(cFEStateRequestLine* self)
{
    char* name = str_16radio;
    if ((void*)SSXApp->Game != 0) {
        name = str_143radio_pda;
        void* list = (char*)&self->mpEngine->mStateStack;
        cUIState* obj = (cUIState*)((char*)cUIStateStack_findState(list, GetHashValue32(str_hud)));
        if (((char*)obj) != 0) {
            sVEntry<int(void*, int, int)>* vt = (sVEntry<int (void*, int, int)>*)obj->mVtbl;
            vt[24].fn(vthis(obj, vt[24]), 3, 0);
        }
    }
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(name), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIMenu_setScrollableMenu(self->mpRadioMenu, *(unsigned char*)&self->mNumSongs);
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateRequestLine_onDestroyScreen);
#ifdef SKIP_ASM

int GetHashValue32(char* str);
void* cUIState_onDestroyScreen(void* self);
extern "C" void* cUIStateStack_findState(void* self, int id);
extern char str_hud[];

/**
 * Destroys the request-line screen, first telling the in-game HUD to come back (4) when a game is running.
 */
extern "C" void cFEStateRequestLine_onDestroyScreen(cFEStateRequestLine* self)
{
    if ((int)SSXApp->Game != 0) {
        cUIState* obj = (cUIState*)cUIStateStack_findState(&self->mpEngine->mStateStack, GetHashValue32(str_hud));
        if (((void*)obj) != 0) {
            sVEntry<int(void*, int, int)>* vtEntry = &((sVEntry<int (void*, int, int)>*)obj->mVtbl)[24];
            vtEntry->fn(vthis(obj, *vtEntry), 4, 0);
        }
    }
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateRequestLine_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cFEState_onGainFocus(void* self);
extern "C" void cFEStateRequestLine_updatePlayList(void* self);
extern "C" void cFEStateRequestLine_updateCurrentlyPlaying(void* self);
extern "C" void cFEStateRequestLine_updateButtonsText(void* self, int a1);
extern "C" void cFEStateRequestLine_updateHelpText(void* self, int a1);

/**
 * Gives the request-line state focus and redraws its play list from the top.
 *
 * Runs the base cFEState_onGainFocus(), resets the list's first visible song (0x150) to 0, then refreshes the
 * play list, the now-playing title and artist texts (cFEStateRequestLine_updateCurrentlyPlaying()), the button texts and the help text.
 */
extern "C" void cFEStateRequestLine_onGainFocus(cFEStateRequestLine* self)
{
    cFEState_onGainFocus(self);
    self->mFirstVisibleSong = 0;
    cFEStateRequestLine_updatePlayList(self);
    cFEStateRequestLine_updateCurrentlyPlaying(self);
    cFEStateRequestLine_updateButtonsText(self, 0);
    cFEStateRequestLine_updateHelpText(self, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festateaudiooptions", cFEStateRequestLine_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cFEStateRequestLine_updatePlayListArrows(void* self);
extern "C" void cFEStateRequestLine_updateButtonsText(void* self, int a1);
extern "C" void cFEStateRequestLine_updateHelpText(void* self, int a1);

/**
 * Refreshes the request line's list arrows, button texts and help text when its gain transition starts.
 *
 * Does the refresh only when @p bFirstTime is nonzero, and returns 0 like the base cUIState_onGainTransition().
 *
 * @param bFirstTime  the state's first-time flag as passed by the state stack; nonzero on the transition's first call
 * @return always 0
 */
extern "C" int cFEStateRequestLine_onGainTransition(cFEStateRequestLine* self, int bFirstTime)
{
    if (bFirstTime != 0) {
        cFEStateRequestLine_updatePlayListArrows(self);
        cFEStateRequestLine_updateButtonsText(self, 0);
        cFEStateRequestLine_updateHelpText(self, 0);
    }
    return 0;
}
#endif

