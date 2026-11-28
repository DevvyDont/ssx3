#ifndef FE_FESTATEMOUNTAINROOM_H
#define FE_FESTATEMOUNTAINROOM_H
#pragma interface

// cFEStateMountainRoom: the lodge's mountain room screen (a cFEState); cFEReal creates it with cMemMan_alloc(200, "cFEStateMountainRoom").
// Size 0xC8 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIGroup;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUIText;
struct tPeakPassInfo;

struct cFEStateMountainRoom : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateMountainRoom
    cUIGroup* mpPassGroup;                     // 0x048 [O]
    cUIGroup* mpPassFrontGroup;                // 0x04C [O]
    cUIIcon* mpPassIcon;                       // 0x050 [O]
    cUIText* mpHelpText;                       // 0x054 [O]
    cUIIcon* mpRaceMarkers[5];                 // 0x058 [O]
    cUIIcon* mpSlopeStyleMarkers[3];           // 0x06C [O]
    cUIIcon* mpRaceBackCountryMarkers[3];      // 0x078 [O]
    cUIIcon* mpFreestyleBackCountryMarkers[3]; // 0x084 [O]
    cUIIcon* mpSuperPipeMarkers[3];            // 0x090 [O]
    cUIIcon* mpBigAirMarkers[3];               // 0x09C [O]
    cTextureElement* mpDoneElem;               // 0x0A8 [O]
    cTextureElement* mpNotDoneElem;            // 0x0AC [O]
    int mCharID;                               // 0x0B0 [O]
    int mPeakNum;                              // 0x0B4 [O]
    int mLevel;                                // 0x0B8 [O]
    int mTextureFileIndex;                     // 0x0BC [O]
    tPeakPassInfo* mpPeakPassInfo;             // 0x0C0 [O]
    int mFrontTextureID;                       // 0x0C4 [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateMountainRoom1() __asm__("cFEStateMountainRoom_cFEStateMountainRoom1");
    bool isPeakPassLoaded() __asm__("cFEStateMountainRoom_isPeakPassLoaded");
    void hidePass() __asm__("cFEStateMountainRoom_hidePass");
    int isPassVisible() __asm__("cFEStateMountainRoom_isPassVisible");
    int isPassFrontVisible() __asm__("cFEStateMountainRoom_isPassFrontVisible");
    void loadPeakPass() __asm__("cFEStateMountainRoom_loadPeakPass");
    void updateTextureLoad() __asm__("cFEStateMountainRoom_updateTextureLoad");
    void showPassFront() __asm__("cFEStateMountainRoom_showPassFront");
    virtual ~cFEStateMountainRoom();   // g++'s own symbol _$_20cFEStateMountainRoom (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateMountainRoom_onCreateScreen");
    virtual bool onInputBegin(cUIInput* widget) __asm__("cFEStateMountainRoom_onInputBegin");
    virtual void onWidgetEvent(cUIThing* w, unsigned int msg) __asm__("cFEStateMountainRoom_onWidgetEvent");
    virtual void onUpdate() __asm__("cFEStateMountainRoom_onUpdate");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int)
        __asm__("cFEStateMountainRoom_onWidgetMenu");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateMountainRoom_onWidgetCreate");
    void updateMarkers() __asm__("cFEStateMountainRoom_updateMarkers");
    virtual void onGainFocus(cUIState* pPrevState) __asm__("cFEStateMountainRoom_onGainFocus");
};

CHECK_OFFSET(cFEStateMountainRoom, mpPassGroup, 0x048);
CHECK_OFFSET(cFEStateMountainRoom, mpPassFrontGroup, 0x04C);
CHECK_OFFSET(cFEStateMountainRoom, mpPassIcon, 0x050);
CHECK_OFFSET(cFEStateMountainRoom, mpHelpText, 0x054);
CHECK_OFFSET(cFEStateMountainRoom, mpRaceMarkers, 0x058);
CHECK_OFFSET(cFEStateMountainRoom, mpSlopeStyleMarkers, 0x06C);
CHECK_OFFSET(cFEStateMountainRoom, mpRaceBackCountryMarkers, 0x078);
CHECK_OFFSET(cFEStateMountainRoom, mpFreestyleBackCountryMarkers, 0x084);
CHECK_OFFSET(cFEStateMountainRoom, mpSuperPipeMarkers, 0x090);
CHECK_OFFSET(cFEStateMountainRoom, mpBigAirMarkers, 0x09C);
CHECK_OFFSET(cFEStateMountainRoom, mpDoneElem, 0x0A8);
CHECK_OFFSET(cFEStateMountainRoom, mpNotDoneElem, 0x0AC);
CHECK_OFFSET(cFEStateMountainRoom, mCharID, 0x0B0);
CHECK_OFFSET(cFEStateMountainRoom, mPeakNum, 0x0B4);
CHECK_OFFSET(cFEStateMountainRoom, mLevel, 0x0B8);
CHECK_OFFSET(cFEStateMountainRoom, mTextureFileIndex, 0x0BC);
CHECK_OFFSET(cFEStateMountainRoom, mpPeakPassInfo, 0x0C0);
CHECK_OFFSET(cFEStateMountainRoom, mFrontTextureID, 0x0C4);
CHECK_SIZE(cFEStateMountainRoom, 0xC8);
__asm__("_vt$20cFEStateMountainRoom = cFEStateMountainRoom_vtbl");   // g++'s name for the retail vtable

#endif
