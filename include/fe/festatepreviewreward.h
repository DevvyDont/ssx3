#ifndef FE_FESTATEPREVIEWREWARD_H
#define FE_FESTATEPREVIEWREWARD_H

// cFEStatePreviewReward: the reward preview screen (a cUIState); the reward gallery (fe/festaterewards) creates it
// with cMemMan_alloc(0x78, "cFEStatePreviewReward").
// Size 0x78 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUIThing;

struct cFEStatePreviewReward : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStatePreviewReward
    cUIIcon* mpIcon[3];             // 0x048 [O]
    cUIThing* mpGroup[3];           // 0x054 [O]
    void* mpReward;                 // 0x060 [O]
    int mCharID;                    // 0x064 [O]
    int mRewardType;                // 0x068 [O]
    int mTextureFileIndex;          // 0x06C [O]
    int mTextureID;                 // 0x070 [O]
    int mOrientation;               // 0x074 [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStatePreviewReward1() __asm__("cFEStatePreviewReward_cFEStatePreviewReward1");
    void setReward(int kind, void* data) __asm__("cFEStatePreviewReward_setReward");
    void loadTexture(const char* fileName) __asm__("cFEStatePreviewReward_loadTexture");
    void updateTextureLoad() __asm__("cFEStatePreviewReward_updateTextureLoad");
    // retail's destructor cFEStatePreviewReward__cFEStatePreviewReward stays a C function (g++ 2.95 takes no asm label
    // on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStatePreviewReward();
    virtual void onCreateScreen() __asm__("cFEStatePreviewReward_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStatePreviewReward_onUpdate");
    virtual bool onInputBegin(cUIInput* obj) __asm__("cFEStatePreviewReward_onInputBegin");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
        __asm__("cFEStatePreviewReward_onWidgetMenu");
};

CHECK_OFFSET(cFEStatePreviewReward, mpIcon, 0x048);
CHECK_OFFSET(cFEStatePreviewReward, mpGroup, 0x054);
CHECK_OFFSET(cFEStatePreviewReward, mpReward, 0x060);
CHECK_OFFSET(cFEStatePreviewReward, mCharID, 0x064);
CHECK_OFFSET(cFEStatePreviewReward, mRewardType, 0x068);
CHECK_OFFSET(cFEStatePreviewReward, mTextureFileIndex, 0x06C);
CHECK_OFFSET(cFEStatePreviewReward, mTextureID, 0x070);
CHECK_OFFSET(cFEStatePreviewReward, mOrientation, 0x074);
CHECK_SIZE(cFEStatePreviewReward, 0x78);

#endif
