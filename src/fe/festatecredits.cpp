#include "common.h"
#include "fe/festatecredits.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_26credits[];
extern char str_Menu0000[];
extern char str_Bitmap0000[];
extern char str_Bitmap0001[];

struct sVEntryK185A98 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Opens the credits screen ("26credits") and hides its two bitmaps, keeping the second as the Dolby icon.
 *
 * Sets bit 3 of "Menu0000"'s menu attributes (tAttrMenu mDisableDirSound). "Bitmap0000" and then "Bitmap0001"
 * are stored at 0x58 (On Tour mpDolbyIcon) and hidden, so 0x58 ends up holding "Bitmap0001".
 */
extern "C" void cFEStateCredits_onCreateScreen(cFEStateCredits* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_26credits), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    char* menu = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu0000));
    if (menu != 0)
        *(int*)(menu + 0x90) |= 8;
    char* bitmap = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Bitmap0000));
    self->mpDolbyIcon = (cUIIcon*)bitmap;
    if (bitmap != 0) {
        sVEntryK185A98* vt = *(sVEntryK185A98**)(bitmap + 8);
        vt[9].fn(bitmap + vt[9].delta, 0);
    }
    bitmap = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Bitmap0001));
    self->mpDolbyIcon = (cUIIcon*)bitmap;
    if (bitmap != 0) {
        sVEntryK185A98* vt = *(sVEntryK185A98**)(bitmap + 8);
        vt[9].fn(bitmap + vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onGainFocus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUITextScroll_addUnicodeStringByID(void* self, int id);
extern "C" void cFEState_onGainFocus(void* self, void* a1);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_TextScroll[];
extern char str_kT_CREDITSpct_03d[];
extern char str_kT_CREDITS006a[];
extern char str_kT_CREDITS006b[];
extern char str_kT_CREDITS074a[];
extern char str_kT_CREDITS087[];
extern char str_kT_CREDITS088[];
extern char str_kT_CREDITS147[];
extern char str_kT_CREDITS148[];

class cUIObjK185BA0 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a);
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
    virtual int v20(float* pos);
    virtual void v21(float* pos);
};

/**
 * Loads the credits text into the "TextScroll" widget, then gives the state focus.
 *
 * Keeps the widget at 0x5C (On Tour mpTextScroll), sets its width to 600, hides it (onRender2D() draws the
 * lines) and adds kT_CREDITS000 to kT_CREDITS167 by string ID (cUITextScroll_addUnicodeStringByID) with 006a, 006b and 074a,
 * leaving out 047-048, 081-086, 089, 146 and 149. Ends with cFEState_onGainFocus().
 *
 * @param pPrevState  the state that had focus before, passed on to cFEState_onGainFocus()
 */
extern "C" void cFEStateCredits_onGainFocus(cFEStateCredits* self, void* pPrevState)
{
    cUIObjK185BA0* textScroll = (cUIObjK185BA0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextScroll));
    self->mpTextScroll = (cUITextScroll*)textScroll;
    if (textScroll != 0)
    {
        float pos[4];
        char buf[0x20];
        int i;
        textScroll->v20(pos);
        pos[0] = 600.0f;
        ((cUIObjK185BA0*)self->mpTextScroll)->v21(pos);
        ((cUIObjK185BA0*)self->mpTextScroll)->v09(0);
        for (i = 0; i < 7; i++)
        {
            sprintf(buf, str_kT_CREDITSpct_03d, i);
            cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        }
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(str_kT_CREDITS006a));
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(str_kT_CREDITS006b));
        for (i = 7; i < 47; i++)
        {
            sprintf(buf, str_kT_CREDITSpct_03d, i);
            cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        }
        for (i = 49; i < 75; i++)
        {
            sprintf(buf, str_kT_CREDITSpct_03d, i);
            cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        }
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(str_kT_CREDITS074a));
        for (i = 75; i < 81; i++)
        {
            sprintf(buf, str_kT_CREDITSpct_03d, i);
            cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        }
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(str_kT_CREDITS087));
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(str_kT_CREDITS088));
        for (i = 90; i < 146; i++)
        {
            sprintf(buf, str_kT_CREDITSpct_03d, i);
            cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        }
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(str_kT_CREDITS147));
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(str_kT_CREDITS148));
        sprintf(buf, str_kT_CREDITSpct_03d, 150);
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        sprintf(buf, str_kT_CREDITSpct_03d, 151);
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        sprintf(buf, str_kT_CREDITSpct_03d, 152);
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        sprintf(buf, str_kT_CREDITSpct_03d, 153);
        cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        for (i = 154; i < 168; i++)
        {
            sprintf(buf, str_kT_CREDITSpct_03d, i);
            cUITextScroll_addUnicodeStringByID(self->mpTextScroll, GetHashValue32(buf));
        }
    }
    cFEState_onGainFocus(self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onRender2D);
#ifdef SKIP_ASM
extern "C" void cFont_text1(void* self, float x, float y, void* m);
extern "C" float cFont_textExtent1(void* f_, const unsigned short* s, void* r_, int mono, float sx, float sy);
extern "C" void* cUIState_onRender2D(void* self);
extern "C" char* cUITextScroll_getFont(void* self);
extern "C" unsigned short* func_003A2068(void* self, int idx);

struct sV3_185F40 {
    float x, y, z;
};

struct sV4_185F40 {
    float x, y, z, w;
};

struct sV2_185F40 {
    float x, y;
};

struct sRect_185F40 {
    float x0, y0, x1, y1;
};

class cUIObj185F40 {
public:
    int pad[2];
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
    virtual void v20(sV4_185F40* out);
};

struct sRS_185F40 {
    int f0;
    int f4;
    int pad : 5;
    int mode : 5;
};

static inline void DrawText_185F40(char* font, unsigned short* s, float x, float y, const sV2_185F40& sc)
{
    *(float*)(font + 0x38) = *(float*)(font + 0x30) * sc.x;
    *(float*)(font + 0x3C) = *(float*)(font + 0x34) * sc.y;
    cFont_text1(font, x, y, s);
    *(sV2_185F40*)(font + 0x38) = *(sV2_185F40*)(font + 0x30);
}

/**
 * Draws the visible credits lines centred on the 640-pixel-wide screen, then renders the screens.
 *
 * Line spacing is the text scroll's font height x scale + 3; the first line and pixel offset come from the
 * scroll position (0x60). A line starting with '~' places the Dolby icon (0x58) there instead; '^' draws in
 * white at the scroll's scale, other lines in its colour at a scale 0.1 smaller. The position wraps from one
 * end to the other in the scroll direction (0x64). Ends with the base state render cUIState_onRender2D().
 */
extern "C" void cFEStateCredits_onRender2D(cFEStateCredits* self)
{
    char* textScroll = (char*)self->mpTextScroll;
    if (textScroll != 0)
    {
        sV3_185F40 pos = *(sV3_185F40*)(textScroll + 0x44);
        sV3_185F40 size = *(sV3_185F40*)(textScroll + 0x50);
        sV4_185F40 box;
        ((cUIObj185F40*)textScroll)->v20(&box);
        char* font = cUITextScroll_getFont(self->mpTextScroll);
        if (font != 0)
        {
            float lineH = (float)*(int*)(font + 0x14) * *(float*)(font + 0x34);
            int any = 0;
            sV4_185F40 white;
            white.x = 1.0f;
            white.y = 1.0f;
            white.z = 1.0f;
            white.w = 1.0f;
            float scaledHeight = size.y * lineH;
            float bh = box.y;
            sV4_185F40 save = *(sV4_185F40*)((char*)self->mpTextScroll + 0x1C);
            ((sRS_185F40*)GrMan->m_pCurrentDrawState)->mode = 21;
            signed char rows = (int)scaledHeight + 3;
            int scroll = self->mPixelOffset;
            int first = scroll / rows;
            int rem = scroll % rows;
            signed char count = (int)(bh / (float)rows);
            if (rem < 0)
                rem = -scroll % rows;
            for (int i = 0; i < count; i++)
            {
                unsigned short* line = func_003A2068(self->mpTextScroll, i + first);
                float frows = (float)rows;
                if (line != 0)
                {
                    float frem = (float)rem;
                    while (*line == ' ')
                        line++;
                    sV3_185F40 newPos = pos;
                    if (self->mPixelOffset > 0)
                        newPos.y -= frem;
                    else
                        newPos.y += frem;
                    if (*line == '~' && self->mpDolbyIcon != 0)
                    {
                        sV4_185F40 iconSize;
                        any = 1;
                        ((cUIObj185F40*)self->mpDolbyIcon)->v20(&iconSize);
                        newPos.x = (640.0f - iconSize.x) * 0.5f;
                        *(sV3_185F40*)((char*)self->mpDolbyIcon + 0x44) = newPos;
                        ((cUIObj185F40*)self->mpDolbyIcon)->v09(1);
                    }
                    else
                    {
                        sV2_185F40 sz;
                        sz.x = size.x;
                        sz.y = size.y;
                        if (*line == '^')
                        {
                            *(sV4_185F40*)(font + 0x40) = white;
                            line++;
                        }
                        else
                        {
                            *(sV4_185F40*)(font + 0x40) = save;
                            sz.x -= 0.10000000149011612f;
                            sz.y -= 0.10000000149011612f;
                        }
                        sRect_185F40 textRect;
                        cFont_textExtent1(font, line, &textRect, 0, sz.x, sz.y);
                        newPos.x = (640.0f - textRect.x1) * 0.5f;
                        DrawText_185F40(font, line, newPos.x, newPos.y, sz);
                    }
                }
                pos.y += frows;
            }
            if (!any)
            {
                if (self->mpDolbyIcon != 0)
                    ((cUIObj185F40*)self->mpDolbyIcon)->v09(0);
            }
            int total = count * rows;
            int lim = *(unsigned short*)((char*)self->mpTextScroll + 0xC8) * rows + total;
            if (self->mPixelOffset < -total && self->mPixelDelta < 0)
                self->mPixelOffset = lim;
            if (lim < self->mPixelOffset && self->mPixelDelta > 0)
                self->mPixelOffset = -total;
        }
    }
    cUIState_onRender2D(((char*)self));
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Scrolls the credits by one step, then runs the base UI state update.
 *
 * Adds the scroll speed (+0x64, which input moves between -8 and 8) to the scroll position (+0x60), then calls
 * cUIState_onUpdate().
 */
extern "C" void cFEStateCredits_onUpdate(cFEStateCredits* self)
{
    self->mPixelOffset += self->mPixelDelta;
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onInputBegin);
#ifdef SKIP_ASM
struct sVEntry001863E8 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Changes the credits scroll speed with up and down.
 *
 * Up (input slot 17) lowers the speed (0x64, On Tour mPixelDelta) down to -8 and down (slot 18) raises it up
 * to 8.
 *
 * @param pInput  the controller input (cUIInput)
 * @return 1 when up or down was pressed, otherwise 0
 */
extern "C" int cFEStateCredits_onInputBegin(cFEStateCredits* self, void* pInput)
{
    sVEntry001863E8* vtEntry = &(*(sVEntry001863E8**)((char*)pInput + 8))[17];
    if (vtEntry->fn((char*)pInput + vtEntry->delta)) {
        if (self->mPixelDelta >= -7) {
            self->mPixelDelta -= 1;
        }
        return 1;
    }
    vtEntry = &(*(sVEntry001863E8**)((char*)pInput + 8))[18];
    if (vtEntry->fn((char*)pInput + vtEntry->delta)) {
        if (self->mPixelDelta < 8) {
            self->mPixelDelta += 1;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the credits menu's events: 0x100 for next, option and misc (6, 8, 9), 0x101 for the rest.
 *
 * cUIMenu_onInput() sends 6 for the next button, 7 for previous, 8 for option and 9 for misc. Bit 0x100 plays
 * the selected item's feedback frame and bit 0 also plays its mSound[1], so a 0x100 answer leaves the button
 * silent.
 *
 * @param pMenu  the menu that sent the event (unused; On Tour pMenu)
 * @param Event  the menu event
 * @return 0x100 for events 6, 8 and 9, otherwise 0x101
 */
extern "C" int cFEStateCredits_onWidgetMenu(cFEStateCredits* self, int pMenu, unsigned int Event)
{
    switch (Event) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onWidgetEvent);
#ifdef SKIP_ASM
struct sVEntry001864B0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void cUIStateStack_push(void* list, void* item);

/**
 * Leaves the credits on back, pushing the custom interface's getPrevState state for data 0x24.
 *
 * @param item  the widget that sent the event; nothing happens when null
 * @param msg  the widget event (only 6, back, is handled)
 */
extern "C" void cFEStateCredits_onWidgetEvent(cFEStateCredits* self, void* item, int msg)
{
    if (item != 0 && msg == 6) {
        void* obj = *(void**)self->mpEngine;
        sVEntry001864B0* vt = *(sVEntry001864B0**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, 36);
        if (nextState != 0) {
            cUIStateStack_push((char*)self->mpEngine + 0x18, nextState);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEState_onGainFocus);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern char str_Menu[];
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" signed char cFEFlowState_getValue(void* self, int a1);
extern "C" void cUIState_onGainFocus(void* self, void* a1);
// NOTE: placeholder for the object at $gp+0x1D90 (no symbol in the target)
struct sFlowState_865A8 {
    signed char count;
    char* states;
};
extern sFlowState_865A8 D_004A4E80;

/**
 * Gives an FE state focus and restores the selection of its first screen's "Menu" from the FE flow state.
 *
 * The index saved for the state's id (mHashName, 0xC) is read from the flow state (D_004A4E80, cFEFlowState_getValue)
 * and set with cUIMenu_setSelectedByIndex(); then cUIState_onGainFocus() runs.
 *
 * @param pPrevState  the state that had focus before, passed on to cUIState_onGainFocus()
 */
extern "C" void cFEState_onGainFocus(void* self, void* pPrevState)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    if (screen != 0) {
        void* menu = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_Menu));
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, cFEFlowState_getValue(&D_004A4E80, *(signed char*)((char*)self + 0xC)));
        }
    }
    cUIState_onGainFocus(self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEState_onLoseFocus);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern char str_Menu[];
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEFlowState_setValue(void* self, int a1, int a2);
// NOTE: placeholder for the object at $gp+0x1D90 (no symbol in the target)
extern sFlowState_865A8 D_004A4E80;

/**
 * Front-end state losing focus: remembers which menu entry was selected, so it can be restored later.
 *
 * Finds the "Menu" object on the state's first screen and calls cFEFlowState_setValue (On Tour cFEFlowState::setValue)
 * on the flow state D_004A4E80 with this state's id (+0xC) and the menu's selected index (+0x95).
 */
extern "C" void cFEState_onLoseFocus(void* self)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    if (screen != 0) {
        void* obj = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_Menu));
        if (obj != 0) {
            cFEFlowState_setValue(&D_004A4E80, *(signed char*)((char*)self + 0xC), *(signed char*)((char*)obj + 0x95));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecredits", cFEStateEventSelect_cFEStateEventSelect1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern "C" void cUITemplate_MAP_init(void* self);
extern void* cFEStateEventSelect_vtbl[];

/**
 * Constructs the event-select state with an engine, as built by the state factory (cFECustom_getNextState).
 *
 * Builds the UI state (cUIState_cUIState; the caller's engine argument reaches it unchanged in the second argument
 * register, since the C declares only self), sets state id 19 and the cFEStateEventSelect_vtbl vtable, and calls
 * cUITemplate_MAP_init on +0x48 (its definition takes no argument and, for map styles 2 and up, sets cUITemplate_MAP_mPeak to
 * the current race location's peak). cFEStateEventSelect_cFEStateEventSelect is the null-engine variant of this
 * constructor, which does not set the state id.
 *
 * @return @p self
 */
extern "C" void* cFEStateEventSelect_cFEStateEventSelect1(void* self)
{
    cUIState_cUIState(self);
    *(int*)((char*)self + 0xC) = 19;
    *(void***)((char*)self + 0x8) = cFEStateEventSelect_vtbl;
    cUITemplate_MAP_init((char*)self + 0x48);
    return self;
}
#endif

