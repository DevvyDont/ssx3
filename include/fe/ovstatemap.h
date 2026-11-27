#ifndef FE_OVSTATEMAP_H
#define FE_OVSTATEMAP_H

// cOVState_MAP: the in-game session-point map overlay (screen "38session": the mountain picture loaded through the FE
// async manager, a player indicator, up to 8 session points with pulsing highlights and a "$500" confirm popup).
// EA's source is fe/ovstatemap.cpp (the July 2003 preview's assert strings name d:/ssx/code/source/fe/ovstatemap.cpp;
// the preview keeps __12cOVState_MAPP9cUIEngine12tOVStateEnum, _$_12cOVState_MAP and
// onMenuGainFocus__12cOVState_MAPP8cUIGroup).
// Retail code: cOVState_MAP_* in fe/ovstatemap (0x002086A8 ... 0x0020A088) and fe/all_in_one_fe (ctor
// cOVState_MAP_cOVState_MAP, destructor, onMenuGainFocus, the engine-less ctor copy func_002212C8).
//
// Size 0x1D4:
//   [R] cOVStateManager_triggerOverlay builds overlay 32 inline in operator new(468, "cOVStateOverlay");
//   [O] On Tour 0x1B0 = cOVState 0x90 + 0x120 of own members; with retail's 4-byte bools (mbSessionUnlocked[8] alone
//       grows by 0x18) On Tour's 20 members in On Tour's order fill retail's 0x9C ... 0x1D4 exactly, and every one but
//       mHighlightAlpha / mHighlightDiff is confirmed by retail use.
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8); the retail ctors run cUIState_cUIState and
// inline cOVState's.
//
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets.
// Evidence tags:
//   [R] retail's constructors and allocation size;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (cOVState_MAP_* asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_MAP): member names, types and order (On Tour's bools are 1 byte, retail's
//       are words, so its offsets differ).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"
#include "ui/uithing.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIIcon;

struct cUIText;

struct cOVState_MAP : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_MAP_vtbl (0x00473320, 25 slots), stored by cOVState_MAP_cOVState_MAP / func_002212C8 / the
    //       inlined ctor in cOVStateManager_triggerOverlay after cOVState_vtbl
    // cOVState_MAP's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "38session" to it; onWidgetEvent pops its state stack
    //   mAttrState: [U] onWidgetEvent sets mode 7 + the pending-delete bit (lw/sw the word)
    // cOVState (fe/ovstate.h, 0x9C)
    // cOVState_MAP's notes on cOVState's fields:
    //   mOVID: [O] tOVStateEnum; [R] the ctor's id (32 from cOVStateManager_triggerOverlay)
    // --- cOVState_MAP
    int mbSessionUnlocked[8];       // 0x09C [O] (bool[8]); [U] onGainTransition stores one flag per session point (a
                                    //       view int flags[8] at 0x9C)
    int mSessionPopupActive;        // 0x0BC [O] (bool); [U] onCreateScreen clears it; onWidgetEvent opens / closes the
                                    //       confirm popup through it; setupPopup tests it
    int mSessionPointsOnPic;        // 0x0C0 [O]; [U] onCreateScreen clears it; setupLocalSessionList counts the
                                    //       session points into it; showIndicators / hideIndicators loop over it
    int mFEAsyncMapFileIndex;       // 0x0C4 [O]; [U] onCreateScreen stores cFEAsyncManager_GetFreeAsyncFile;
                                    //       setupMapPic loads the map picture into it; onUpdate polls it;
                                    //       onDestroyScreen frees it
    int mMapTextureSet;             // 0x0C8 [O] (bool); [U] setupMapPic / setupScreenExit clear it; onUpdate sets it
                                    //       once the picture is attached
    float mMapPicAlpha;             // 0x0CC [O]; [U] onUpdate fades it to 1.0 by 0.1 per frame (lwc1/swc1)
    cUIIcon* mpMap;                 // 0x0D0 [O]; [U] onCreateScreen keeps the "MapPic" object here; onUpdate shows it
                                    //       / sets its texture; setupScreenExit hides it
    cUIScreen* mpMapScreen;         // 0x0D4 [O] cOVState_MAP::mpScreen (renamed: it hides cUIState::mpScreen at 0x40);
                                    //       [U] onCreateScreen keeps the "38session" screen here; every widget lookup
                                    //       of the class
    int mSelectedSessionPoint;      // 0x0D8 [O]; [U] onWidgetEvent hands it to the mission interface (nextSessionID)
                                    //       on select
    int mScreenExiting;             // 0x0DC [O] (bool); [U] onCreateScreen clears it, setupScreenExit sets it;
                                    //       onUpdate stops while it is set
    cUIIcon* mpPlayerIcon;          // 0x0E0 [O]; [U] onCreateScreen clears it; setupPlayerIndicator creates the player
                                    //       cUIIcon; show/hide/cleanupIndicators
    cUIIcon* mpSessionIcons[8];     // 0x0E4 [O]; [U] onCreateScreen clears them; setupLocalSessionList creates one
                                    //       cUIIcon per session point; show/hide/cleanupIndicators
    cUIIcon* mpSessionHighlights[8]; // 0x104 [O]; [U] ditto (the highlight icons); onUpdate scales them around
                                    //       mHighlightOriginalPos
    t3Vector mHighlightOriginalPos[8]; // 0x124 [O]; [R] the ctors' empty 8-element constructor loop; [U]
                                    //       setupLocalSessionList stores each highlight's position, onUpdate
                                    //       re-centres around it (12-byte stride)
    cUIIcon* mpChallengeIcons[8];   // 0x184 [O]; [U] onCreateScreen clears them (no other retail use)
    cUIIcon* mpChallengeHighlights[8]; // 0x1A4 [O]; [U] onCreateScreen clears them (no other retail use)
    float mHighlightAlpha;          // 0x1C4 [O]; placed by On Tour's order and the size only (no retail access found)
    float mHighlightDiff;           // 0x1C8 [O]; placed by On Tour's order and the size only (no retail access found)
    float mIndicatorSize;           // 0x1CC [O]; [U] onCreateScreen sets 1.0; onUpdate swings it between 18 and 21.5
                                    //       (swc1/lwc1)
    float mIndicatorSizeDiff;       // 0x1D0 [O]; [U] onCreateScreen sets 0.16; onUpdate flips its sign at the limits

    // Methods bound to their retail symbols by asm labels.
    void setupScreenExit() __asm__("cOVState_MAP_setupScreenExit");
    void setupMapPic() __asm__("cOVState_MAP_setupMapPic");
    void setupPopup() __asm__("cOVState_MAP_setupPopup");
    void setWidgetHiliteColor(cUIText* obj, int on) __asm__("cOVState_MAP_setWidgetHiliteColor");
    void setWidgetUnHiliteColor(cUIText* obj, int on) __asm__("cOVState_MAP_setWidgetUnHiliteColor");
    void setupPlayerIndicator() __asm__("cOVState_MAP_setupPlayerIndicator");
    void setupLocalSessionList() __asm__("cOVState_MAP_setupLocalSessionList");
    void showIndicators() __asm__("cOVState_MAP_showIndicators");
    void hideIndicators() __asm__("cOVState_MAP_hideIndicators");
    void cleanupIndicators() __asm__("cOVState_MAP_cleanupIndicators");
    // retail's destructor cOVState_MAP__cOVState_MAP stays a C function (g++ 2.95 takes no asm label on a destructor);
    // declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_MAP();
    virtual bool onGainTransition(int on) __asm__("cOVState_MAP_onGainTransition");
    virtual void onDestroyScreen() __asm__("cOVState_MAP_onDestroyScreen");
    virtual void onUpdate() __asm__("cOVState_MAP_onUpdate");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int Event) __asm__("cOVState_MAP_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int) __asm__("cOVState_MAP_onWidgetMenu");
    virtual void onInputAlways(cUIInput* pInput) __asm__("cOVState_MAP_onInputAlways");
};

CHECK_OFFSET(cOVState_MAP, mbSessionUnlocked, 0x9C);
CHECK_OFFSET(cOVState_MAP, mSessionPopupActive, 0xBC);
CHECK_OFFSET(cOVState_MAP, mSessionPointsOnPic, 0xC0);
CHECK_OFFSET(cOVState_MAP, mFEAsyncMapFileIndex, 0xC4);
CHECK_OFFSET(cOVState_MAP, mMapTextureSet, 0xC8);
CHECK_OFFSET(cOVState_MAP, mMapPicAlpha, 0xCC);
CHECK_OFFSET(cOVState_MAP, mpMap, 0xD0);
CHECK_OFFSET(cOVState_MAP, mpMapScreen, 0xD4);
CHECK_OFFSET(cOVState_MAP, mSelectedSessionPoint, 0xD8);
CHECK_OFFSET(cOVState_MAP, mScreenExiting, 0xDC);
CHECK_OFFSET(cOVState_MAP, mpPlayerIcon, 0xE0);
CHECK_OFFSET(cOVState_MAP, mpSessionIcons, 0xE4);
CHECK_OFFSET(cOVState_MAP, mpSessionHighlights, 0x104);
CHECK_OFFSET(cOVState_MAP, mHighlightOriginalPos, 0x124);
CHECK_OFFSET(cOVState_MAP, mpChallengeIcons, 0x184);
CHECK_OFFSET(cOVState_MAP, mpChallengeHighlights, 0x1A4);
CHECK_OFFSET(cOVState_MAP, mHighlightAlpha, 0x1C4);
CHECK_OFFSET(cOVState_MAP, mHighlightDiff, 0x1C8);
CHECK_OFFSET(cOVState_MAP, mIndicatorSize, 0x1CC);
CHECK_OFFSET(cOVState_MAP, mIndicatorSizeDiff, 0x1D0);
CHECK_SIZE(cOVState_MAP, 0x1D4);

#endif /* FE_OVSTATEMAP_H */
