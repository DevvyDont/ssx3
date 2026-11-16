#include "common.h"

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int a2);
extern "C" void func_00190A08(void* self);
extern "C" void func_00190CD8(void* self);
extern char str_14rid_prof[];
extern char str_ListBox0000[];
extern char str_kT_14Heading1[];
extern char str_kT_14Heading2[];
extern char str_kT_14Heading3[];
extern char str_kT_14Heading4[];

struct sVEnt_001908C0 { short delta; short index; void (*fn)(void*); };

/**
 * Builds the rider bio screen ("14rid_prof"): its four heading tabs, the rider and the first page.
 *
 * Adds "kT_14Heading1" to "kT_14Heading4" to "ListBox0000", stores the character ID of player mUserNum
 * (+0x44) at +0x48 (then discards the player interface, its vtable slot 2), selects page 0 (+0x4C), shows
 * that page (func_00190A08) and shows only this rider's "<name>" and "<name>_s" objects (func_00190CD8).
 */
extern "C" void cFEStateRiderBio_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_14rid_prof), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* box = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ListBox0000));
    if (box != 0) {
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading1), 0);
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading2), 0);
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading3), 0);
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading4), 0);
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    *(int*)((char*)self + 0x48) = cBENewPlayerInterface_getPlayerCharID(iface, *(signed char*)((char*)self + 0x44));
    sVEnt_001908C0* vt = *(sVEnt_001908C0**)((char*)iface + 0xC);
    vt[2].fn((char*)iface + vt[2].delta);
    *(int*)((char*)self + 0x4C) = 0;
    func_00190A08(self);
    func_00190CD8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", func_00190A08);
#ifdef SKIP_ASM
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cFEStateRiderBio_fillDNAInfo(void* self);
extern "C" void cFEStateRiderBio_fillFavesInfo(void* self);
extern "C" void cFEStateRiderBio_fillQnAInfo(void* self);
extern "C" void cFEStateRiderBio_fillBioInfo(void* self);
extern char str_DNALabels[];
extern char str_DNAText[];
extern char str_FavesLabels[];
extern char str_FavesText[];
extern char str_QNALabels[];
extern char str_QNAText[];
extern char str_BIOAutoScrollText[];
extern char str_vertical_dash[];
extern char str_horizontal_dash[];

extern "C" void func_00190A08(void* self)
{
    switch (*(int*)((char*)self + 0x4C)) {
    case 0:
        cUIState_showObjSafe(self, str_DNALabels);
        cUIState_showObjSafe(self, str_DNAText);
        cUIState_hideObjSafe(self, str_FavesLabels);
        cUIState_hideObjSafe(self, str_FavesText);
        cUIState_hideObjSafe(self, str_QNALabels);
        cUIState_hideObjSafe(self, str_QNAText);
        cUIState_hideObjSafe(self, str_BIOAutoScrollText);
        cUIState_showObjSafe(self, str_vertical_dash);
        cUIState_showObjSafe(self, str_horizontal_dash);
        cFEStateRiderBio_fillDNAInfo(self);
        break;
    case 1:
        cUIState_hideObjSafe(self, str_DNALabels);
        cUIState_hideObjSafe(self, str_DNAText);
        cUIState_showObjSafe(self, str_FavesLabels);
        cUIState_showObjSafe(self, str_FavesText);
        cUIState_hideObjSafe(self, str_QNALabels);
        cUIState_hideObjSafe(self, str_QNAText);
        cUIState_hideObjSafe(self, str_BIOAutoScrollText);
        cUIState_showObjSafe(self, str_vertical_dash);
        cUIState_showObjSafe(self, str_horizontal_dash);
        cFEStateRiderBio_fillFavesInfo(self);
        break;
    case 2:
        cUIState_hideObjSafe(self, str_DNALabels);
        cUIState_hideObjSafe(self, str_DNAText);
        cUIState_hideObjSafe(self, str_FavesLabels);
        cUIState_hideObjSafe(self, str_FavesText);
        cUIState_showObjSafe(self, str_QNALabels);
        cUIState_showObjSafe(self, str_QNAText);
        cUIState_hideObjSafe(self, str_BIOAutoScrollText);
        cUIState_hideObjSafe(self, str_vertical_dash);
        cUIState_hideObjSafe(self, str_horizontal_dash);
        cFEStateRiderBio_fillQnAInfo(self);
        break;
    case 3:
        cUIState_hideObjSafe(self, str_DNALabels);
        cUIState_hideObjSafe(self, str_DNAText);
        cUIState_hideObjSafe(self, str_FavesLabels);
        cUIState_hideObjSafe(self, str_FavesText);
        cUIState_hideObjSafe(self, str_QNALabels);
        cUIState_hideObjSafe(self, str_QNAText);
        cUIState_showObjSafe(self, str_BIOAutoScrollText);
        cUIState_hideObjSafe(self, str_vertical_dash);
        cUIState_hideObjSafe(self, str_horizontal_dash);
        cFEStateRiderBio_fillBioInfo(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", func_00190CD8);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);

struct sNames_0CD8 {
    char* name[10];
};
extern sNames_0CD8 D_0045E470;
extern char str_pct_s_s[];

extern "C" void func_00190CD8(void* self)
{
    sNames_0CD8 names = D_0045E470;
    char buf[32];
    for (int i = 0; i < 10; i++) {
        sprintf(buf, str_pct_s_s, names.name[i]);
        if (i == *(int*)((char*)self + 0x48)) {
            cUIState_showObjSafe(self, names.name[i]);
            cUIState_showObjSafe(self, buf);
        } else {
            cUIState_hideObjSafe(self, names.name[i]);
            cUIState_hideObjSafe(self, buf);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillDNAInfo);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);

struct sDNANames {
    char* name[10][8];
};
extern sDNANames D_0045E998;
extern char str_DNApct_d[];

/**
 * Fills the bio screen's DNA page: sets the texts "DNA1" to "DNA8" to this rider's DNA lines.
 *
 * For each text found, the string ID comes from a 10 x 8 table indexed by the character ID (+0x48), e.g.
 * "kT_DNA1Moby" for Moby.
 */
extern "C" void cFEStateRiderBio_fillDNAInfo(void* self)
{
    sDNANames names = D_0045E998;
    char buf[16];
    for (int i = 0; i < 8; i++) {
        sprintf(buf, str_DNApct_d, i + 1);
        cUIText* obj = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        if (obj != 0) {
            cUIText_setUnicodeStringByID(obj, GetHashValue32(names.name[*(int*)((char*)self + 0x48)][i]));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillFavesInfo);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);

struct sFavesNames {
    char* name[10][12];
};
extern sFavesNames D_0045F2D0;
extern char str_FAVESpct_d[];

/**
 * Fills the bio screen's favourites page: sets the texts "FAVES1" to "FAVES12" to this rider's favourites.
 *
 * For each text found, the string ID comes from a 10 x 12 table indexed by the character ID (+0x48), e.g.
 * "kT_FAVES1Moby" for Moby.
 */
extern "C" void cFEStateRiderBio_fillFavesInfo(void* self)
{
    sFavesNames names = D_0045F2D0;
    char buf[16];
    for (int i = 0; i < 12; i++) {
        sprintf(buf, str_FAVESpct_d, i + 1);
        cUIText* obj = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        if (obj != 0) {
            cUIText_setUnicodeStringByID(obj, GetHashValue32(names.name[*(int*)((char*)self + 0x48)][i]));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillQnAInfo);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);

struct sQnANames {
    char* name[10][4];
};
extern sQnANames D_0045F730;
extern char str_QNApct_d[];

/**
 * Fills the bio screen's Q&A page: sets the texts "QNA1" to "QNA4" to this rider's answers.
 *
 * For each text found, the string ID comes from a 10 x 4 table indexed by the character ID (+0x48), e.g.
 * "kT_QNA1Moby" for Moby.
 */
extern "C" void cFEStateRiderBio_fillQnAInfo(void* self)
{
    sQnANames names = D_0045F730;
    char buf[16];
    for (int i = 0; i < 4; i++) {
        sprintf(buf, str_QNApct_d, i + 1);
        cUIText* obj = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        if (obj != 0) {
            cUIText_setUnicodeStringByID(obj, GetHashValue32(names.name[*(int*)((char*)self + 0x48)][i]));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillBioInfo);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* func_0014EEC8(void* self, int player, int index);
extern "C" void cUITextScroll_clearBuffer(void* self);
extern "C" void cUITextScroll_addUnicodeStringByID(void* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_BIOAutoScrollText[];
extern char str_kT_FULLBIO1pct_s[];

/**
 * Fills the biography page: puts "kT_FULLBIO1" + the rider's name into the "BIOAutoScrollText" scroller.
 *
 * The name is what func_0014EEC8() returns for the character ID at +0x48 (its character record, which
 * probably starts with the name; character interface, BE interface 2). The scroller's old lines are freed
 * (cUITextScroll_clearBuffer) before cUITextScroll_addUnicodeStringByID() sets the new string ID. Nothing happens when the object is missing.
 */
extern "C" void cFEStateRiderBio_fillBioInfo(void* self)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_BIOAutoScrollText));
    if (obj) {
        char buf[64];
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 2);
        sprintf(buf, str_kT_FULLBIO1pct_s, func_0014EEC8(iface, *(int*)((char*)self + 0x48), 0));
        cUITextScroll_clearBuffer(obj);
        cUITextScroll_addUnicodeStringByID(obj, GetHashValue32(buf));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_onWidgetEvent);
#ifdef SKIP_ASM
struct sVEntry001912D0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void func_00190A08(void* self);

/**
 * Handles the rider bio screen's widget events: probably going back, and switching the page shown.
 *
 * Event 6 asks the engine's custom interface (vtable slot 5, probably getPrevState) for the state to go to,
 * passing the widget's misc data, and pushes it when there is one. Event 9 takes the widget's current
 * list-box entry (+0x319) as the page (+0x4C) and shows it (func_00190A08). A null widget is ignored.
 *
 * @param item  the widget (On Tour pWidget)
 * @param msg  the event (On Tour Event)
 */
extern "C" void cFEStateRiderBio_onWidgetEvent(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001912D0* vt = *(sVEntry001912D0**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    }
    case 9:
        *(int*)((char*)self + 0x4C) = *(unsigned char*)((char*)item + 0x319);
        func_00190A08(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", func_00191360);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" char func_001A1CD0(void* p, int a);
extern void* cFEStateRuleSelect_vtbl[];

extern "C" void* func_00191360(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = cFEStateRuleSelect_vtbl;
    *(int*)((char*)self + 0x48) = 0;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), 0);
    *(int*)((char*)self + 0xC) = 0x14;
    return self;
}
#endif

