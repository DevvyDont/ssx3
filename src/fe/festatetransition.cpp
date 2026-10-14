#include "ui/uiscreen.h"

// Screen and event names
extern char D_0045FFD8[];
extern char D_0045FFE8[];
extern char D_0045FFF8[];
extern char D_004A1360[];
extern char D_004A17B8[];

extern "C" void* func_0028B180();
extern "C" void func_00294F78(void* sound, int event);

class cFEStateBackground {
public:
    cUIEngine* getEngine() { return m_engine; }
    void onCreateScreen();

    char pad0[0x10];
    cUIEngine* m_engine;
};

class cFEStateTransition {
public:
    cUIEngine* getEngine() { return m_engine; }
    void onCreateScreen();
    int onScreenEvent(cUIScreen* screen, uint event);
    int func_001946A8(int which);

    char pad0[0x10];
    cUIEngine* m_engine;
    char pad14[0x8];
    int m_flags;
    char pad20[0x28];
    cUIScreen* m_screen0;
    cUIScreen* m_screen1;
};

void cFEStateBackground::onCreateScreen()
{
    cUIScreen* screen = getEngine()->addScreenByHashName((cUIState*)this, GetHashValue32(D_0045FFD8), 0);
    if (screen)
        screen->playFrame(0, 0);
}

void cFEStateTransition::onCreateScreen()
{
    m_screen0 = getEngine()->addScreenByHashName((cUIState*)this, GetHashValue32(D_0045FFE8), 0);
    m_screen1 = getEngine()->addScreenByHashName((cUIState*)this, GetHashValue32(D_0045FFF8), 0);
}

int cFEStateTransition::onScreenEvent(cUIScreen* screen, uint event)
{
    uint show = GetHashValue32(D_004A1360);
    uint hide = GetHashValue32(D_004A17B8);

    if (event == show) {
        m_flags &= ~8;
        screen->m_flags &= ~0x40;
    } else if (event == hide) {
        m_flags |= 8;
        screen->m_flags |= 0x40;
    }
    return 0;
}

// Starts transition 0 (with UI sound 15) or 1 (the silent white fade)
int cFEStateTransition::func_001946A8(int which)
{
    if (which == 0) {
        if (m_screen0) {
            m_screen0->m_flags |= 0x40;
            m_screen0->playFrame(0, 0);
            func_00294F78(func_0028B180(), 15);
        }
    } else if (which == 1) {
        if (m_screen1) {
            m_screen1->m_flags |= 0x40;
            m_screen1->playFrame(0, 0);
        }
    }
    return 0;
}

// Constructor; stores the vtable pointer, so it waits until .data is split
INCLUDE_ASM("fe/festatetransition", func_00194738);
