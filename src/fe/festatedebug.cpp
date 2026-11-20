#include "common.h"
#include "fe/festatecharselect.h"

//100%
INCLUDE_ASM("fe/festatedebug", cScreenKeyboard_initScreenKeyboard);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* s);
extern "C" void cScreenKeyboard_setInputText(void* self, char* s);
extern "C" void cScreenKeyboard_getKeyObjName(void* self, int id, char* buf);
extern "C" void cScreenKeyboard_getCursorObjName(void* self, int id, char* buf);
extern char str_keyboard_arrLeft[];
extern char str_keyboard_arrRight[];
extern char str_keyboard_end[];
extern char str_cursor_arrLeft[];
extern char str_cursor_arrRight[];
extern char str_cursor_end[];
extern char str_cursor_A[];
extern char str_profilename[];
extern char D_004A1370[];
extern char D_004A1378[];
extern char str_END[];
extern char str_mypro1[];

class cUIObjK180B60 {
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
    virtual void setVisible(int on);
};

/**
 * Sets up an on-screen keyboard on a screen: key labels, hidden cursors and the starting text "mypro1".
 *
 * "keyboard_arrLeft", "keyboard_arrRight" and "keyboard_end" are shown as "<-", "->" and "END"; each
 * "keyboard_A" to "keyboard_Z" and "keyboard_0" to "keyboard_9" gets its character, and every "cursor_"
 * object but "cursor_A" is hidden. The input text (+0xEC) becomes "mypro1" and is shown in "profilename".
 *
 * @param screen  the screen holding the keyboard (On Tour mpScreen)
 */
extern "C" void cScreenKeyboard_initScreenKeyboard(char* self, void* screen)
{
    cUIObjK180B60* textObj;
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_keyboard_arrLeft));
    if (textObj != 0)
    {
        textObj->setVisible(1);
        cUIText_setAsciiString((cUIText*)textObj, D_004A1370);
    }
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_keyboard_arrRight));
    if (textObj != 0)
    {
        textObj->setVisible(1);
        cUIText_setAsciiString((cUIText*)textObj, D_004A1378);
    }
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_keyboard_end));
    if (textObj != 0)
    {
        textObj->setVisible(1);
        cUIText_setAsciiString((cUIText*)textObj, str_END);
    }
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cursor_arrLeft));
    if (textObj != 0)
        textObj->setVisible(0);
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cursor_arrRight));
    if (textObj != 0)
        textObj->setVisible(0);
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cursor_end));
    if (textObj != 0)
        textObj->setVisible(0);
    char name[0x20];
    char ch[2];
    ch[1] = 0;
    ch[0] = 'A';
    for (int i = 0; i < 26; i++)
    {
        cScreenKeyboard_getKeyObjName(self, i + 10, name);
        textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        if (textObj != 0)
            cUIText_setAsciiString((cUIText*)textObj, ch);
        cScreenKeyboard_getCursorObjName(self, i + 10, name);
        textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        if (textObj != 0)
            textObj->setVisible(0);
        ch[0]++;
    }
    ch[0] = '0';
    for (int j = 0; j < 10; j++)
    {
        cScreenKeyboard_getKeyObjName(self, j, name);
        textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        if (textObj != 0)
            cUIText_setAsciiString((cUIText*)textObj, ch);
        cScreenKeyboard_getCursorObjName(self, j, name);
        textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        if (textObj != 0)
            textObj->setVisible(0);
        ch[0]++;
    }
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cursor_A));
    if (textObj != 0)
        textObj->setVisible(1);
    cScreenKeyboard_setInputText(self, str_mypro1);
    textObj = (cUIObjK180B60*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_profilename));
    if (textObj != 0)
        cUIText_setAsciiString((cUIText*)textObj, self + 0xEC);
}
#endif

//100%
INCLUDE_ASM("fe/festatedebug", cScreenKeyboard_onInput);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cScreenKeyboard_getCursorObjName(void* self, int id, char* buf);
extern "C" void cScreenKeyboard_kbMoveLeft(void* self);
extern "C" void cScreenKeyboard_kbMoveRight(void* self);
extern "C" void cScreenKeyboard_kbMoveUp(void* self);
extern "C" void cScreenKeyboard_kbMoveDown(void* self);
extern "C" void cScreenKeyboard_pressKey(void* self);
extern "C" void cScreenKeyboard_toggleShift(void* self);

struct sDbgObj_180EF0 {
    char pad0[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int on);
};
struct sDbgInput_180EF0 {
    char pad0[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
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
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual int v26();
};
struct sDbgState_180EF0 {
    char pad0[0x170];
    int sel;    // 0x170
};

/**
 * Handles one input event on the on-screen debug keyboard: moves the key cursor or presses the highlighted key, and
 * moves the highlight to match.
 *
 * Before acting, it builds the highlighted key's object name for the current key index (sel, +0x170) with
 * cScreenKeyboard_getCursorObjName() and calls slot 9 of that screen object (found with cUIScreen_getObjectByHashName()) with 0; after
 * acting it does the same for the new index with 1. Only the first matching input is handled, tested through @p in's
 * virtual slots (cFEInput's order): isLeft (19) cScreenKeyboard_kbMoveLeft(), isRight (20) cScreenKeyboard_kbMoveRight(), isUp (17)
 * cScreenKeyboard_kbMoveUp(), isDown (18) cScreenKeyboard_kbMoveDown() (cursor moves), isNext (5) cScreenKeyboard_pressKey() (types the key), isRShift2
 * (26) cScreenKeyboard_toggleShift() (toggles the +0xE4 flag). The object sits in vtable cScreenKeyboard_vtbl next to
 * cScreenKeyboard__cScreenKeyboard.
 *
 * @param in  the UI input to read
 * @return always 1 (input consumed)
 */
extern "C" int cScreenKeyboard_onInput(sDbgState_180EF0* self, sDbgInput_180EF0* in)
{
    char buf[0x20];
    cScreenKeyboard_getCursorObjName(self, self->sel, buf);
    sDbgObj_180EF0* keyObject = (sDbgObj_180EF0*)cUIScreen_getObjectByHashName(self, GetHashValue32(buf));
    if (keyObject) {
        keyObject->v09(0);
    }
    if (in->v19()) {
        cScreenKeyboard_kbMoveLeft(self);
    } else if (in->v20()) {
        cScreenKeyboard_kbMoveRight(self);
    } else if (in->v17()) {
        cScreenKeyboard_kbMoveUp(self);
    } else if (in->v18()) {
        cScreenKeyboard_kbMoveDown(self);
    } else if (in->v05()) {
        cScreenKeyboard_pressKey(self);
    } else if (in->v26()) {
        cScreenKeyboard_toggleShift(self);
    }
    cScreenKeyboard_getCursorObjName(self, self->sel, buf);
    keyObject = (sDbgObj_180EF0*)cUIScreen_getObjectByHashName(self, GetHashValue32(buf));
    if (keyObject) {
        keyObject->v09(1);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatedebug", cFEStateCharSelect_cFEStateCharSelect);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* owner);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern void* cFEStateCharSelect_vtbl[];

/**
 * Constructs the character-select state for one rider slot: base cUIState, cFEStateCharSelect_vtbl, state id 9.
 *
 * Runs the cUIState constructor cUIState_cUIState(), stores cFEStateCharSelect_vtbl and mHashName 9, sets +0x54 to 1,
 * mUserNum (+0x44) to @p count and clears +0x48 and +0x4C. mCtrlBits (+0x15) starts at 0xFF with the controller mask of
 * every earlier rider slot (0 .. @p count - 1, cFECustom_getPlayerCtrlMask() on the engine's FE custom object) removed, so a
 * controller that already picked a character cannot drive this one. cFECustom_getNextState() and cFECustom_getPrevState()
 * allocate it (144 bytes, "cFEStateCharSelect").
 *
 * @param owner  the UI engine (mpEngine)
 * @param count  the rider slot choosing now (0 = first player); stored as mUserNum
 * @return @p self
 */
extern "C" void* cFEStateCharSelect_cFEStateCharSelect(cFEStateCharSelect* self, void* owner, int count)
{
    signed char n = count;
    int i = 0;
    unsigned char mask = 0xFF;
    cUIState_cUIState(self, owner);
    self->mVtbl = cFEStateCharSelect_vtbl;
    self->mHashName = 9;
    self->mbIsFirst = 1;
    self->mUserNum = n;
    self->mpSelectedCharWidget = 0;
    self->mCheatCharID = 0;
    void* mgr = *(void**)self->mpEngine;
    for (; i < n; i++) {
        mask &= ~cFECustom_getPlayerCtrlMask(mgr, i);
    }
    self->mCtrlBits = mask;
    return ((void*)self);
}
#endif

