#ifndef UISCREEN_H
#define UISCREEN_H

#include "common.h"

class cUIState;

class cUIScreen {
public:
    void playFrame(int frame, int unk);

    char pad0[0x54];
    int m_flags;        // 0x40: hidden
};

class cUIEngine {
public:
    cUIScreen* addScreenByHashName(cUIState* owner, uint hash, int unk);
};

int GetHashValue32(char* str);

#endif
