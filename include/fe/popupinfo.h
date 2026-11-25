#ifndef FE_POPUPINFO_H
#define FE_POPUPINFO_H

// cPopupInfo: the description of one cScreenPopup (type, message, title, entry-box texts, up to 5 option names and
// confirmations, layer offset, the id reported back to the owner, layout flags). Every cScreenPopup embeds one at
// 0xBC (`info`, fe/screenpopup.h; getPopupInfo cScreenPopup_getPopupInfo returns it); the online states fill it after
// cFEStateNet's createPopup (func_001A9C50) and show the popup with func_001A9BC0.
//
// Size 0xCC: the members end at 0xC8 and the vptr follows (retail _vt$10cPopupInfo cPopupInfo_vtbl, 1 slot: the destructor
// cPopupInfo__cPopupInfo), as fe/screenpopup.h records; cScreenPopup's next member starts at 0x188 = 0xBC + 0xCC.
// Methods (retail fe/feasyncfile 0x1C55F8-0x1C58D0; EA's source: all_in_one_fe.cpp in On Tour):
//   cPopupInfo_cPopupInfo cPopupInfo() (stores the vptr, calls clear), cPopupInfo_clear clear(), cPopupInfo__cPopupInfo ~cPopupInfo(),
//   func_001C5780/func_001C57A0/func_001C57C0 setPopupOptionNames(int, const char* / const unsigned short* /
//   unsigned int) (eType 2/1/3), func_001C57E0 setPopupOptionNames(int, const tPopupElement*),
//   cPopupInfo_getPopupOptionNames getPopupOptionNames(int), func_001C5810/func_001C5838/func_001C5878 setPopupConfirmation(int,
//   const char* / const unsigned short* / unsigned int, int iDefault) (eType 2/1/3), cPopupInfo_clearPopupConfirmation
//   clearPopupConfirmation(int), func_001C58A0 setPopupConfirmation(int, const tConfirmElement*), cPopupInfo_getPopupConfirmation
//   getPopupConfirmation(int). The other accessors (setPopupType, setPopupMessage ...) are inlined at every call site.
//
// A FLAT layout, like main/game.h. Member names and order are EA's, from SSX On Tour's DWARF (class cPopupInfo, size
// 0xC4: its bools are 1 byte; retail's are words, so the members after mbPassUpdate sit at different offsets there).
// Evidence tags:
//   [P] the July 2003 preview's inline accessors (setPopupMessage..setPopupPostPending sw 4..0x50, getLayerOffset lw
//       0x90, getMiscData lw 0x94, getPassUpdate lw 0x98, getOffset/setOffset 0x9C, getReceivePrevCommand lw 0xA8,
//       getDefaultOption lw 0xAC, getPopupType lw 0x0); tools/layout_xlate.py measures no shift up to 0xA4;
//   [R] retail clear (cPopupInfo_clear): memsets every tPopupElement / tConfirmElement, stores 0 to 0x0, 0x90, 0x94, 0x98,
//       0xB0, 0xB4, 0xB8, 0xC0, 1 to 0xA8 and 0xBC, -1 to 0xAC and t3vect_Zero to 0x9C (not 0xC4); the constructor
//       and destructor store the vptr at 0xC8;
//   [U] how retail code uses the field: cScreenPopup's initScreenPopup/FillOptions copy them (fe/screenpopup.h's
//       comments: info + 0x90/0xA8/0xAC/0xB0/0xB8/0xBC/0xC0/0xC4), the online states store them;
//   [O] On Tour's member order and types.

#include "layout.h"
#include "bx/math3d.h"

// tPopupElement (On Tour: size 0x8): a string given as unicode (eType 1), ascii (2) or a localisation hash id (3);
// 0 = none (ePopupStringType). The preview's setters store eType and the union in one go (setPopupMessage(PCUs) li 1).
struct tPopupElement {
    int eType;                      // 0x0 [O] ePopupStringType (Invalid 0, Unicode 1, Ascii 2, ID 3); [P] the setters
    union {                         // 0x4 [O] an anonymous union in On Tour (member names not kept); named after
        const unsigned short* pustr; //     On Tour's setter parameters (pustrMessage, pszOptionNames, uiOptionNames)
        const char* psz;
        unsigned int ui;
    };
};

CHECK_OFFSET(tPopupElement, eType, 0x0);
CHECK_OFFSET(tPopupElement, pustr, 0x4);
CHECK_SIZE(tPopupElement, 0x8);

// One option's confirmation request (On Tour: tConfirmElement, 0xC bytes: a tPopupElement base + iDefaultOption).
// fe/screenpopup.h defines the same 0xC-byte struct (eType, unk_04, iDefaultOption) for cScreenPopup::mtConfirmation;
// when this header lands, screenpopup.h should include it and drop its own copy (no src code names its fields yet).
struct tConfirmElement {
    int eType;                      // 0x0 [O] tPopupElement::eType; [R] setPopupConfirmation stores 1/2/3,
                                    //     clearPopupConfirmation (cPopupInfo_clearPopupConfirmation) 0; [U] non-zero = ask for confirmation
    union {                         // 0x4 [O] tPopupElement's union; [R] setPopupConfirmation stores the string / id
        const unsigned short* pustr;
        const char* psz;
        unsigned int ui;
    };
    int iDefaultOption;             // 0x8 [O]; [R] setPopupConfirmation's iDefault; [U] copied to the confirmation
                                    //     popup's default option
};

CHECK_OFFSET(tConfirmElement, eType, 0x0);
CHECK_OFFSET(tConfirmElement, pustr, 0x4);
CHECK_OFFSET(tConfirmElement, iDefaultOption, 0x8);
CHECK_SIZE(tConfirmElement, 0xC);

struct cPopupInfo {
    int meType;                     // 0x000 [O] ePopupType (EntryBox -1, NoButton 0, 1..5 options); [P] getPopupType/
                                    //       setPopupType lw/sw 0x0; [R] clear stores 0; [U] cScreenPopup::meType
    tPopupElement mtMessage;        // 0x004 [O]; [P] setPopupMessage (eType 0x4, value 0x8), getPopupMessage = this + 4
    tPopupElement mtTitle;          // 0x00C [O]; [P] setPopupTitle 0xC/0x10
    tPopupElement mtEntryBoxTitle;  // 0x014 [O]; [P] setPopupEntryBoxTitle 0x14/0x18
    tPopupElement mtEntryBoxInitial; // 0x01C [O]; [P] setPopupEntryBoxInitial 0x1C/0x20
    tPopupElement mtOptionNames[5]; // 0x024 [O]; [R] setPopupOptionNames func_001C5780.. (0x24 + 8 * i), clear
    tPopupElement mtPostPending;    // 0x04C [O]; [P] setPopupPostPending 0x4C/0x50
    tConfirmElement mtConfirmation[5]; // 0x054 [O]; [R] setPopupConfirmation func_001C5810.. (0x54 + 0xC * i), clear
    int miLayerOffset;              // 0x090 [O]; [P] getLayerOffset/setLayerOffset lw/sw 0x90; [R] clear 0
    int miMiscData;                 // 0x094 [O]; [P] getMiscData/setMiscData lw/sw 0x94; [R] clear 0; [U] the online
                                    //       states store the popup's message id (0x806..0x83E), seen again in onPopupEvent
    int mbPassUpdate;               // 0x098 [O] (On Tour: bool); [P] getPassUpdate/setPassUpdate lw/sw 0x98; [R] clear 0
    t3Vector mvOffset;              // 0x09C [O]; [P] getOffset/setOffset 0x9C..0xA4; [R] clear copies t3vect_Zero
    int mbReceivePrev;              // 0x0A8 [O] (bool); [P] getReceivePrevCommand/setReceivePrevCommand lw/sw 0xA8;
                                    //       [R] clear 1
    int miDefaultOption;            // 0x0AC [O]; [P] getDefaultOption/setDefaultOption lw/sw 0xAC; [R] clear -1
    float mfMinSize;                // 0x0B0 [O]; [R] clear stores 0 (sw $0); [U] cScreenPopup::mfMinSize copies it
    int mbDeferEventsFlag;          // 0x0B4 [O] (bool); [R] clear 0; [U] func_001A9930 and other online states set 1
    int mbCenterOptions;            // 0x0B8 [O] (bool); [R] clear 0; [U] cScreenPopup::mbCenterOptions copies it
    int mbCenterText;               // 0x0BC [O] (bool); [R] clear 1; [U] createPopup (func_001A9C50) stores 0;
                                    //       cScreenPopup::mbCenterText copies it
    int meBackgroundType;           // 0x0C0 [O] ePopupBackground (Online 0, Blue 1, Black 2); [R] clear 0;
                                    //       [U] cScreenPopup::meBackgroundType copies it
    int miMaxChars;                 // 0x0C4 [O]; [U] cScreenPopup::miMaxChars copies it (not touched by clear)
    void** mVtbl;                   // 0x0C8 [R] _vt$10cPopupInfo cPopupInfo_vtbl, stored by cPopupInfo_cPopupInfo and cPopupInfo__cPopupInfo

    // Methods bound to their retail symbols by asm labels.
    void clear() __asm__("cPopupInfo_clear");
    void* getPopupOptionNames(int iEntryNum) __asm__("cPopupInfo_getPopupOptionNames");
    void clearPopupConfirmation(int iEntryNum) __asm__("cPopupInfo_clearPopupConfirmation");
    void* getPopupConfirmation(int iEntryNum) __asm__("cPopupInfo_getPopupConfirmation");
    void setMiscData(int miscData) __asm__("cPopupInfo_setMiscData");
    int getMiscData() __asm__("cPopupInfo_getMiscData");
    void setPassUpdate(int passUpdate) __asm__("cPopupInfo_setPassUpdate");
    int getPassUpdate() __asm__("cPopupInfo_getPassUpdate");
    void setDefaultOption(int defaultOption) __asm__("cPopupInfo_setDefaultOption");
    int getDefaultOption() __asm__("cPopupInfo_getDefaultOption");
    void setReceivePrevCommand(int receivePrev) __asm__("cPopupInfo_setReceivePrevCommand");
    int getReceivePrevCommand() __asm__("cPopupInfo_getReceivePrevCommand");
    void setLayerOffset(int layerOffset) __asm__("cPopupInfo_setLayerOffset");
    int getLayerOffset() __asm__("cPopupInfo_getLayerOffset");
    void setBackgroundType(int backgroundType) __asm__("cPopupInfo_setBackgroundType");
    int getBackgroundType() __asm__("cPopupInfo_getBackgroundType");
    void setMaxCharacters(int maxChars) __asm__("cPopupInfo_setMaxCharacters");
    int getMaxCharacters() __asm__("cPopupInfo_getMaxCharacters");
    void setMinBackgroundSize(float fparg0) __asm__("cPopupInfo_setMinBackgroundSize");
    float getMinBackgroundSize() __asm__("cPopupInfo_getMinBackgroundSize");
    void setUseCenterAlignedOptions(int centerOptions) __asm__("cPopupInfo_setUseCenterAlignedOptions");
    int getUseCenterAlignedOptions() __asm__("cPopupInfo_getUseCenterAlignedOptions");
    void setUseCenterAlignedText(int centerText) __asm__("cPopupInfo_setUseCenterAlignedText");
    int getUseCenterAlignedText() __asm__("cPopupInfo_getUseCenterAlignedText");
    void setDeferEventsFlag(int deferEvents) __asm__("cPopupInfo_setDeferEventsFlag");
    int getDeferEventsFlag() __asm__("cPopupInfo_getDeferEventsFlag");
};

CHECK_OFFSET(cPopupInfo, meType, 0x000);
CHECK_OFFSET(cPopupInfo, mtMessage, 0x004);
CHECK_OFFSET(cPopupInfo, mtTitle, 0x00C);
CHECK_OFFSET(cPopupInfo, mtEntryBoxTitle, 0x014);
CHECK_OFFSET(cPopupInfo, mtEntryBoxInitial, 0x01C);
CHECK_OFFSET(cPopupInfo, mtOptionNames, 0x024);
CHECK_OFFSET(cPopupInfo, mtPostPending, 0x04C);
CHECK_OFFSET(cPopupInfo, mtConfirmation, 0x054);
CHECK_OFFSET(cPopupInfo, miLayerOffset, 0x090);
CHECK_OFFSET(cPopupInfo, miMiscData, 0x094);
CHECK_OFFSET(cPopupInfo, mbPassUpdate, 0x098);
CHECK_OFFSET(cPopupInfo, mvOffset, 0x09C);
CHECK_OFFSET(cPopupInfo, mbReceivePrev, 0x0A8);
CHECK_OFFSET(cPopupInfo, miDefaultOption, 0x0AC);
CHECK_OFFSET(cPopupInfo, mfMinSize, 0x0B0);
CHECK_OFFSET(cPopupInfo, mbDeferEventsFlag, 0x0B4);
CHECK_OFFSET(cPopupInfo, mbCenterOptions, 0x0B8);
CHECK_OFFSET(cPopupInfo, mbCenterText, 0x0BC);
CHECK_OFFSET(cPopupInfo, meBackgroundType, 0x0C0);
CHECK_OFFSET(cPopupInfo, miMaxChars, 0x0C4);
CHECK_OFFSET(cPopupInfo, mVtbl, 0x0C8);
CHECK_SIZE(cPopupInfo, 0xCC);

#endif /* FE_POPUPINFO_H */
