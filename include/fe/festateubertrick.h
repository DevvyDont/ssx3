#ifndef FE_FESTATEUBERTRICK_H
#define FE_FESTATEUBERTRICK_H

// cFEStateUberTrick: the store's uber trick screen (a cUIState). Methods in fe/festatestore and fe/festateriderbio.
// Size 0x60 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIMenu;
struct cUIScreen;
struct cUIState;

struct cFEStateUberTrick : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateUberTrick
    int mbModelVisible;             // 0x048 [O] bool
    cUIMenu* mpMainMenu;            // 0x04C [O]
    cUIMenu* mpTrickMenu;           // 0x050 [O]
    cTextureElement* mpCheckmark;   // 0x054 [O]
    cTextureElement* mpCheckbox;    // 0x058 [O]
    cTextureElement* mpDollarSign;  // 0x05C [O]

    // Methods bound to their retail symbols by asm labels.
    void activateMainMenu() __asm__("cFEStateUberTrick_activateMainMenu");
    void activateTrickMenu() __asm__("cFEStateUberTrick_activateTrickMenu");
    void costVisible(int on) __asm__("cFEStateUberTrick_costVisible");
    void previewVisible(int on) __asm__("cFEStateUberTrick_previewVisible");
    void trickVisible(int on) __asm__("cFEStateUberTrick_trickVisible");
    void updateTrickMenu() __asm__("cFEStateUberTrick_updateTrickMenu");
    void updateTrickMenuEntry() __asm__("cFEStateUberTrick_updateTrickMenuEntry");
    // retail's destructor cFEStateUberTrick__cFEStateUberTrick stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateUberTrick();
    virtual void onCreateScreen() __asm__("cFEStateUberTrick_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cFEStateUberTrick_onDestroyScreen");
    virtual bool onGainTransition(int on) __asm__("cFEStateUberTrick_onGainTransition");
    virtual void onUpdate() __asm__("cFEStateUberTrick_onUpdate");
    virtual void onWidgetCreate(cUIThing* widget) __asm__("cFEStateUberTrick_onWidgetCreate");
    virtual void onPopupEvent(cUIState* item, unsigned int msg) __asm__("cFEStateUberTrick_onPopupEvent");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cFEStateUberTrick_onWidgetEvent");
    virtual void onGainFocus(cUIState* prevState) __asm__("cFEStateUberTrick_onGainFocus");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int) __asm__("cFEStateUberTrick_onWidgetMenu");
};

CHECK_OFFSET(cFEStateUberTrick, mbModelVisible, 0x048);
CHECK_OFFSET(cFEStateUberTrick, mpMainMenu, 0x04C);
CHECK_OFFSET(cFEStateUberTrick, mpTrickMenu, 0x050);
CHECK_OFFSET(cFEStateUberTrick, mpCheckmark, 0x054);
CHECK_OFFSET(cFEStateUberTrick, mpCheckbox, 0x058);
CHECK_OFFSET(cFEStateUberTrick, mpDollarSign, 0x05C);
CHECK_SIZE(cFEStateUberTrick, 0x60);

#endif
