#ifndef FE_UITEMPLATEMAP_H
#define FE_UITEMPLATEMAP_H

// cUITemplate_MAP: the mountain-map template (peaks, event groups and locations with their unlock / completed state,
// the selected location's info panel, the map picture, the transport popup). It is not a state: the screens that
// show the map embed one and forward their virtuals to it with its address: cFEStateEventSelect (fe/festateload.h,
// at 0x48) and cOVState_TRANSPORT (fe/ovstatetransport.h, at 0x9C). Its class statics are globals:
// cUITemplate_MAP::mMapStyle (cUITemplate_MAP_mMapStyle, D_004A2594), mMapState (D_004A259C), mSelectedEvent
// (D_004A25A0), mPeak (D_004A25A4) and On Tour's mMapFEStart (by position cUITemplate_MAP_mMapFEStart, which the FE state factories
// set next to mMapStyle).
// EA's source file is not known (no assert of its own); retail code: fe/uitemplatemap (0x00200700 ... 0x00208438) and
// cUITemplate_MAP_init in fe/ovstaterewardslist (0x002006B8, statics only).
//
// Size 0x2F4:
//   [R] cOVState_TRANSPORT is 0x390 (operator new(912, "cOVStateOverlay") in cOVStateManager_triggerOverlay) with the
//       template at 0x9C and nothing after it (On Tour: cOVState 0x90 + cUITemplate_MAP), so 0x390 - 0x9C = 0x2F4;
//       cFEStateEventSelect, the template at 0x48 and nothing after it, is cMemMan_alloc(0x33C, "cFEStateEventSelect")
//       (cFECustom_getNextState / getPrevState, cFEStateChallengePopup_onWidgetEvent): 0x33C - 0x48 = 0x2F4.
//   [O] On Tour's DWARF says 0x124: its 3 + 4 + 72 + 72 + 4 bools are 1 byte (retail: words) and its tMapLocation is
//       0x64 (retail 0x6C, two word bools). With those, On Tour's members fill retail's 0x2F4 exactly, in On Tour's
//       order, and every retail access lands on one (the highest is the word mScreenExiting at 0x2F0).
// No base class and no virtual functions (no vtable; the methods are called directly).
//
// A FLAT layout, like main/game.h: every field at its retail offset. bools are ints (ee-gcc 2.95's bool is 4 bytes).
// Evidence tags:
//   [R] retail's allocation sizes of the embedding classes;
//   [U] how retail code uses the field (cUITemplate_MAP_* asm and C bodies);
//   [O] SSX On Tour's DWARF (class cUITemplate_MAP, struct tMapLocation): names, types and order.
// (No preview evidence: layout_xlate finds no paired cUITemplate_MAP methods.)

#include "layout.h"

struct cUIIcon;
struct cUIScreen;
struct cUIState;

// One map location / event (On Tour tMapLocation, 0x64; retail 0x6C). The goal tables kPeakRaceLocations [3][4] (race),
// kPeakFreestyleLocations [3][5] (freestyle) and kPeakFreerideLocations [3][8] (freeride) are arrays of it; onWidgetEvent copies the
// picked entry into cUITemplate_MAP::mSelectedLocation, isMapEventCompleted takes one by value.
struct tMapLocation {
    int location;                   // 0x00 [O] eBELocationID; [U] the track (isMapEventCompleted: 23 never completes)
    int gameMode;                   // 0x04 [O] eBEGameMode
    int gameEvent;                  // 0x08 [O] eBEGameEvent; [U] the event kind (isMapEventCompleted: 4, 5 = peak
                                    //       events)
    int disableInSingleEvent;       // 0x0C [O] (bool)
    int useLocationAsLabel;         // 0x10 [O] (bool); [U] setupLocationInfo: use the track's name as the label
    char labelName[30];             // 0x14 [O]; [U] the location widget's hash name / label
    char eventDescriptionStr[35];   // 0x32 [O]; [U] setupLocationInfo's help-text key
    char pathName[20];              // 0x55 [O]; [U] showEventPath(pathName)
    char unk_69[0x3];               // 0x69 padding
};

CHECK_OFFSET(tMapLocation, location, 0x00);
CHECK_OFFSET(tMapLocation, gameMode, 0x04);
CHECK_OFFSET(tMapLocation, gameEvent, 0x08);
CHECK_OFFSET(tMapLocation, disableInSingleEvent, 0x0C);
CHECK_OFFSET(tMapLocation, useLocationAsLabel, 0x10);
CHECK_OFFSET(tMapLocation, labelName, 0x14);
CHECK_OFFSET(tMapLocation, eventDescriptionStr, 0x32);
CHECK_OFFSET(tMapLocation, pathName, 0x55);
CHECK_OFFSET(tMapLocation, unk_69, 0x69);
CHECK_SIZE(tMapLocation, 0x6C);

struct cUIText;

struct cUITemplate_MAP {
    // The map's enums. [O] On Tour's eMapStyle (global there; the class static mMapStyle), eMapFEStartPoint
    // (mMapFEStart) and cUITemplate_MAP's own eMapPicTypes, eMapPopupType, eMapSelectableEvent and eMapState, names
    // and values. Retail fits them: [U] onCreateScreen adds the PDA template for kMAPSTYLE_BE_SELECTPEAKEVENT,
    // kMAPSTYLE_TRANSPORT and kMAPSTYLE_EOE_TRANSPORT and hides the PDA buttons only for kMAPSTYLE_EOE_TRANSPORT;
    // setupLayout titles kMAPSTATE_SELECTPEAK "kT_240SelectPeak", kMAPSTATE_SELECTGOAL "kT_FESelPeakGoal" and
    // kMAPSTATE_SELECTLOCATION the race / freestyle / freeride event titles by mSelectedEvent; setupPeakGoals' goal
    // groups are race, freestyle (5 goals), freeride and earnings; setupMapPic loads the three-peak picture
    // (Load3PeakPic) for kMAP_3PEAK; setupPopup gives kMAPPOPUP_TRANSPORT_NOMONEY a lone "OK" button.
    enum eMapStyle {
        kMAPSTYLE_FE_SELECTPEAKEVENT = 0, kMAPSTYLE_BE_SELECTPEAKEVENT = 1, kMAPSTYLE_PAUSE_MAP = 2,
        kMAPSTYLE_TRANSPORT = 3, kMAPSTYLE_EOE_TRANSPORT = 4
    };
    enum eMapFEStartPoint { kMAPFESTARTPOINT_PEAK = 0, kMAPFESTARTPOINT_LOCATION = 1 };
    enum eMapPicTypes { kMAP_NONE = 0, kMAP_3PEAK = 1, kMAP_PEAKA = 2, kMAP_PEAKB = 3, kMAP_PEAKC = 4 };
    enum eMapPopupType { kMAPPOPUP_TRANSPORT = 0, kMAPPOPUP_TRANSPORT_NOMONEY = 1 };
    enum eMapSelectableEvent {
        kMAPEVENT_RACE = 0, kMAPEVENT_FREESTYLE = 1, kMAPEVENT_FREERIDE = 2, kMAPEVENT_EARNINGS = 3
    };
    enum eMapState { kMAPSTATE_SELECTPEAK = 0, kMAPSTATE_SELECTGOAL = 1, kMAPSTATE_SELECTLOCATION = 2 };

    int mShowInfo;                  // 0x000 [O] (bool); [U] setShowInfo stores it; onGainTransition / onUpdate /
                                    //       onWidgetEvent / setupLayout test it
    int mbPeakUnlocked[3];          // 0x004 [O] (bool[3]); [U] setupPeakUnlock fills it; setupMenus / setupPeakInfo
    int mbEventUnlocked[4];         // 0x010 [O] (bool[4]); [U] setupEventUnlock__FP15cUITemplate_MAP fills it;
                                    //       setupMenus
    int mbLocationUnlocked[3][3][8]; // 0x020 [O] (bool; On Tour's DWARF prints [8][3][3]); [U] setupMenus fills it,
                                    //       setupLocationInfo indexes it [mPeak][mSelectedEvent][location]
    int mbLocationEventCompleted[3][3][8]; // 0x140 [O] (bool); [U] setupMenus (isMapEventCompleted) fills it,
                                    //       setupLocationInfo indexes it like mbLocationUnlocked
    tMapLocation mSelectedLocation; // 0x260 [O]; [U] onWidgetEvent copies the picked goal-table entry here and reads
                                    //       location / gameMode / gameEvent (0x260 / 0x264 / 0x268)
    int mTransportPopupActive;      // 0x2CC [O] (bool); [U] onCreateScreen 0; setupPopup / onWidgetEvent / setupLayout
    int mPopupType;                 // 0x2D0 [O]; [U] onCreateScreen, setupPopup, setupMenuFocus
    int mMapPicType;                // 0x2D4 [O]; [U] onCreateScreen 0, setupMapPic
    int mFEAsyncMapFileIndex;       // 0x2D8 [O]; [U] setupMapPic loads the map picture file into it; onUpdate waits
                                    //       on it; onDestroyScreen releases it
    int mMapTextureSet;             // 0x2DC [O] (bool); [U] setupMapPic clears it, onUpdate sets it once the texture
                                    //       is on mpMap
    float mMapPicAlpha;             // 0x2E0 [O]; [U] onUpdate fades the map picture in (lwc1/swc1)
    cUIIcon* mpMap;                 // 0x2E4 [O]; [U] onCreateScreen looks the map picture up; onUpdate /
                                    //       setupScreenExit
    cUIScreen* mpScreen;            // 0x2E8 [O]; [U] onCreateScreen's screen argument; every widget lookup
    cUIState* mpUIState;            // 0x2EC [O]; [U] onCreateScreen's state argument (the embedding screen); its
                                    //       engine / state stack are reached through it
    int mScreenExiting;             // 0x2F0 [O] (bool); [U] onCreateScreen 0, setupScreenExit sets it, onUpdate

    // Methods bound to their retail symbols by asm labels.
    void onCreateScreen(void* screen, cUIState* owner) __asm__("cUITemplate_MAP_onCreateScreen");
    void onGainFocus() __asm__("cUITemplate_MAP_onGainFocus");
    void onDestroyScreen() __asm__("cUITemplate_MAP_onDestroyScreen");
    int isMapEventLocked(int kind, int x) __asm__("cUITemplate_MAP_isMapEventLocked");
    void setupMenus() __asm__("cUITemplate_MAP_setupMenus");
    void setupEventUnlock() __asm__("cUITemplate_MAP_setupEventUnlock__FP15cUITemplate_MAP");
    void setupPeakUnlock() __asm__("cUITemplate_MAP_setupPeakUnlock");
    void setupScreenExit() __asm__("cUITemplate_MAP_setupScreenExit");
    int onWidgetMenu(int menu, int event) __asm__("cUITemplate_MAP_onWidgetMenu");
    void onUpdate() __asm__("cUITemplate_MAP_onUpdate");
    void setupLayout() __asm__("cUITemplate_MAP_setupLayout");
    void setupMenuFocus(int focus) __asm__("cUITemplate_MAP_setupMenuFocus");
    void setupMapPic() __asm__("cUITemplate_MAP_setupMapPic");
    void setupPopup() __asm__("cUITemplate_MAP_setupPopup");
    void setupPeakInfo() __asm__("cUITemplate_MAP_setupPeakInfo");
    int getMedalStr(int peak) __asm__("cUITemplate_MAP_getMedalStr");
    void setupPeakGoals() __asm__("cUITemplate_MAP_setupPeakGoals");
    void showEventPath(char* name) __asm__("cUITemplate_MAP_showEventPath");
    void setWidgetHiliteColor(cUIText* obj, int on) __asm__("cUITemplate_MAP_setWidgetHiliteColor");
    void setWidgetUnHiliteColor(cUIText* obj, int on) __asm__("cUITemplate_MAP_setWidgetUnHiliteColor");
};

CHECK_OFFSET(cUITemplate_MAP, mShowInfo, 0x000);
CHECK_OFFSET(cUITemplate_MAP, mbPeakUnlocked, 0x004);
CHECK_OFFSET(cUITemplate_MAP, mbEventUnlocked, 0x010);
CHECK_OFFSET(cUITemplate_MAP, mbLocationUnlocked, 0x020);
CHECK_OFFSET(cUITemplate_MAP, mbLocationEventCompleted, 0x140);
CHECK_OFFSET(cUITemplate_MAP, mSelectedLocation, 0x260);
CHECK_OFFSET(cUITemplate_MAP, mTransportPopupActive, 0x2CC);
CHECK_OFFSET(cUITemplate_MAP, mPopupType, 0x2D0);
CHECK_OFFSET(cUITemplate_MAP, mMapPicType, 0x2D4);
CHECK_OFFSET(cUITemplate_MAP, mFEAsyncMapFileIndex, 0x2D8);
CHECK_OFFSET(cUITemplate_MAP, mMapTextureSet, 0x2DC);
CHECK_OFFSET(cUITemplate_MAP, mMapPicAlpha, 0x2E0);
CHECK_OFFSET(cUITemplate_MAP, mpMap, 0x2E4);
CHECK_OFFSET(cUITemplate_MAP, mpScreen, 0x2E8);
CHECK_OFFSET(cUITemplate_MAP, mpUIState, 0x2EC);
CHECK_OFFSET(cUITemplate_MAP, mScreenExiting, 0x2F0);
CHECK_SIZE(cUITemplate_MAP, 0x2F4);

#endif /* FE_UITEMPLATEMAP_H */
