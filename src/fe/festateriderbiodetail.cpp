#include "common.h"
#include "fe/festateruleselect.h"
#include "fe/festateriderbio.h"

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
extern "C" void cFEStateRiderBio_setupBioScreen(cFEStateRiderBio* self);
extern "C" void cFEStateRiderBio_setupCharPics(cFEStateRiderBio* self);
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
 * that page (cFEStateRiderBio_setupBioScreen) and shows only this rider's "<name>" and "<name>_s" objects (cFEStateRiderBio_setupCharPics).
 */
extern "C" void cFEStateRiderBio_onCreateScreen(cFEStateRiderBio* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_14rid_prof), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* box = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ListBox0000));
    if (box != 0) {
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading1), 0);
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading2), 0);
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading3), 0);
        cUIListBox_addEntryByStringID(box, GetHashValue32(str_kT_14Heading4), 0);
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    self->mCharID = cBENewPlayerInterface_getPlayerCharID(iface, self->mUserNum);
    sVEnt_001908C0* vt = *(sVEnt_001908C0**)((char*)iface + 0xC);
    vt[2].fn((char*)iface + vt[2].delta);
    self->mScreen = 0;
    cFEStateRiderBio_setupBioScreen(self);
    cFEStateRiderBio_setupCharPics(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_setupBioScreen);
#ifdef SKIP_ASM
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cFEStateRiderBio_fillDNAInfo(cFEStateRiderBio* self);
extern "C" void cFEStateRiderBio_fillFavesInfo(cFEStateRiderBio* self);
extern "C" void cFEStateRiderBio_fillQnAInfo(cFEStateRiderBio* self);
extern "C" void cFEStateRiderBio_fillBioInfo(cFEStateRiderBio* self);
extern char str_DNALabels[];
extern char str_DNAText[];
extern char str_FavesLabels[];
extern char str_FavesText[];
extern char str_QNALabels[];
extern char str_QNAText[];
extern char str_BIOAutoScrollText[];
extern char str_vertical_dash[];
extern char str_horizontal_dash[];

/**
 * Shows the rider-bio page selected by +0x4C (On Tour mScreen) and fills it.
 *
 * Page 0 is DNA ("DNALabels", "DNAText", cFEStateRiderBio_fillDNAInfo), 1 favourites ("FavesLabels", "FavesText",
 * cFEStateRiderBio_fillFavesInfo), 2 Q&A ("QNALabels", "QNAText", cFEStateRiderBio_fillQnAInfo) and 3 the bio
 * ("BIOAutoScrollText", cFEStateRiderBio_fillBioInfo). The page's objects are shown and the other pages' hidden
 * (cUIState_showObjSafe, cUIState_hideObjSafe); "vertical_dash" and "horizontal_dash" are shown on pages 0-1 and hidden
 * on 2-3. Any other value does nothing.
 */
extern "C" void cFEStateRiderBio_setupBioScreen(cFEStateRiderBio* self)
{
    switch (self->mScreen) {
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
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_setupCharPics);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);

struct sNames_0CD8 {
    char* name[10];
};
extern sNames_0CD8 D_0045E470;
extern char str_pct_s_s[];

/**
 * Shows the picture objects of the rider in +0x48 (On Tour mCharID) and hides the other riders'.
 *
 * For each of the ten rider names in D_0045E470 ("moby", "kaori", ...) the object of that name and its "<name>_s"
 * companion (str_pct_s_s, "%s_s") are shown when the index equals +0x48 and hidden otherwise (cUIState_showObjSafe,
 * cUIState_hideObjSafe).
 */
extern "C" void cFEStateRiderBio_setupCharPics(cFEStateRiderBio* self)
{
    sNames_0CD8 names = D_0045E470;
    char buf[32];
    for (int i = 0; i < 10; i++) {
        sprintf(buf, str_pct_s_s, names.name[i]);
        if (i == self->mCharID) {
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
extern "C" void cFEStateRiderBio_fillDNAInfo(cFEStateRiderBio* self)
{
    sDNANames names = D_0045E998;
    char buf[16];
    for (int i = 0; i < 8; i++) {
        sprintf(buf, str_DNApct_d, i + 1);
        cUIText* obj = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (obj != 0) {
            cUIText_setUnicodeStringByID(obj, GetHashValue32(names.name[self->mCharID][i]));
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
extern "C" void cFEStateRiderBio_fillFavesInfo(cFEStateRiderBio* self)
{
    sFavesNames names = D_0045F2D0;
    char buf[16];
    for (int i = 0; i < 12; i++) {
        sprintf(buf, str_FAVESpct_d, i + 1);
        cUIText* obj = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (obj != 0) {
            cUIText_setUnicodeStringByID(obj, GetHashValue32(names.name[self->mCharID][i]));
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
extern "C" void cFEStateRiderBio_fillQnAInfo(cFEStateRiderBio* self)
{
    sQnANames names = D_0045F730;
    char buf[16];
    for (int i = 0; i < 4; i++) {
        sprintf(buf, str_QNApct_d, i + 1);
        cUIText* obj = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (obj != 0) {
            cUIText_setUnicodeStringByID(obj, GetHashValue32(names.name[self->mCharID][i]));
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
extern "C" void* cBECharacterInterface_getShortNameByCharacter(void* self, int player, int index);
extern "C" void cUITextScroll_clearBuffer(void* self);
extern "C" void cUITextScroll_addUnicodeStringByID(void* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_BIOAutoScrollText[];
extern char str_kT_FULLBIO1pct_s[];

/**
 * Fills the biography page: puts "kT_FULLBIO1" + the rider's name into the "BIOAutoScrollText" scroller.
 *
 * The name is what cBECharacterInterface_getShortNameByCharacter() returns for the character ID at +0x48 (its character record, which
 * probably starts with the name; character interface, BE interface 2). The scroller's old lines are freed
 * (cUITextScroll_clearBuffer) before cUITextScroll_addUnicodeStringByID() sets the new string ID. Nothing happens when the object is missing.
 */
extern "C" void cFEStateRiderBio_fillBioInfo(cFEStateRiderBio* self)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_BIOAutoScrollText));
    if (obj) {
        char buf[64];
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 2);
        sprintf(buf, str_kT_FULLBIO1pct_s, cBECharacterInterface_getShortNameByCharacter(iface, self->mCharID, 0));
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
extern "C" void cFEStateRiderBio_setupBioScreen(cFEStateRiderBio* self);

/**
 * Handles the rider bio screen's widget events: probably going back, and switching the page shown.
 *
 * Event 6 asks the engine's custom interface (vtable slot 5, probably getPrevState) for the state to go to,
 * passing the widget's misc data, and pushes it when there is one. Event 9 takes the widget's current
 * list-box entry (+0x319) as the page (+0x4C) and shows it (cFEStateRiderBio_setupBioScreen). A null widget is ignored.
 *
 * @param item  the widget (On Tour pWidget)
 * @param msg  the event (On Tour Event)
 */
extern "C" void cFEStateRiderBio_onWidgetEvent(cFEStateRiderBio* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 6: {
        void* obj = *(void**)self->mpEngine;
        sVEntry001912D0* vt = *(sVEntry001912D0**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push((char*)self->mpEngine + 0x18, nextState);
        }
        break;
    }
    case 9:
        self->mScreen = *(unsigned char*)((char*)item + 0x319);
        cFEStateRiderBio_setupBioScreen(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRuleSelect_cFEStateRuleSelect);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" char cFECustom_getPlayerCtrlMask(void* p, int a);
extern void* cFEStateRuleSelect_vtbl[];

/**
 * Constructs the custom race rules state (allocated as "cFEStateRuleSelect", 156 bytes).
 *
 * Runs cUIState's constructor (func_0039E2A0; the engine argument reaches it untouched in the second argument
 * register), stores cFEStateRuleSelect_vtbl, clears +0x48 (On Tour mpGameEventMapping), sets mUserNum to 0 and
 * mCtrlBits to user 0's controller bits (cFECustom_getPlayerCtrlMask on the engine's front end) and mHashName to 20. It lives in the
 * rider-bio unit.
 *
 * @return @p self
 */
extern "C" void* cFEStateRuleSelect_cFEStateRuleSelect(cFEStateRuleSelect* self)
{
    func_0039E2A0(self);
    self->mVtbl = cFEStateRuleSelect_vtbl;
    self->mpGameEventMapping = 0;
    *(char*)&self->mUserNum = 0;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask(*(void**)self->mpEngine, 0);
    self->mHashName = 20;
    return ((void*)self);
}
#endif

