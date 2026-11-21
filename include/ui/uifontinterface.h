#ifndef UI_UIFONTINTERFACE_H
#define UI_UIFONTINTERFACE_H

// cUIFontInterface: the UI engine's font table (cUIEngine::mpFontInterface, SSXApp->Fonts). setData takes the font
// chunk of the UI data (a tUIFontTableHeader, then per font {hash, name length, name}) and adds one cUIFontEntry per
// font; loadFonts creates each entry's cFont (cMemMan_alloc(132, "UI Font") + cFont_cFont); getFontByHashName looks a
// font up by hash; purgeFonts deletes them. Retail code: ui/uiengine (cUIFontInterface_getFontByHashName ..
// cUIFontInterface_loadFonts).
//
// Size 0x44: [R] cSSXApp_initload: new (0x44, "cUIFontInterface") with the inline constructor zeroing 0x0, 0x34, 0x38,
// 0x3C and 0x40 and each entry's three words; [O] On Tour's class cUIFontInterface is 0x44 with the same members. No
// vtable (On Tour declares none; no vptr store in the constructor).
//
// Names: SSX On Tour's DWARF ([O] class cUIFontInterface, class cUIFontEntry, struct tUIFontTableHeader). Replaces
// ui/uiengine.cpp's s3983F0 / s3983F0Entry / sChunkHdr_3984E8 and main/ssxapp.cpp's sFontIface_82D0 / sFontEntry_82D0
// views (same layout).
//
// FLAT layouts, like main/game.h. Evidence tags: [R] the allocation and constructor; [U] how retail code uses the
// field; [O] On Tour's DWARF.

#include "layout.h"

struct cFont;
struct cUIEngine;

// The font chunk's header: the number of font records after it. [O] struct tUIFontTableHeader (0x4).
struct tUIFontTableHeader {
    unsigned int mNumEntries;       // 0x0 [O]; [U] cUIFontInterface_setData loops over it
};

// One font: its hash, its file name (inside the UI data) and the loaded font. [O] class cUIFontEntry (0xC).
struct cUIFontEntry {
    unsigned int mHashID;           // 0x0 [O]; [U] getFontByHashName compares it with the hash; addFontEntry stores it
    const char* mpFileName;         // 0x4 [O]; [U] addFontEntry stores the name (On Tour's inline setFileName);
                                    //      loadFonts passes it to cFont_cFont
    cFont* mpFont;                  // 0x8 [O]; [U] loadFonts stores the new cFont; purgeFonts deletes it;
                                    //      getFontByHashName returns it
};

struct cUIFontInterface {
    tUIFontTableHeader* mpFontTableHeader; // 0x00 [O]; [R] = 0; [U] setData stores the chunk and loops to its count
    cUIFontEntry mFontEntry[4];     // 0x04 [O]; [R] each entry's words zeroed; [U] addFontEntry fills entry index
    signed char mFontCount;         // 0x34 [O] char; [R] = 0; [U] addFontEntry raises it, loadFonts / purgeFonts /
                                    //      getFontByHashName loop to it (lb: signed)
    char unk_35[0x3];               // 0x35 padding
    cUIEngine* mpEngine;            // 0x38 [O]; [R] = 0
    char* mpFontPath;               // 0x3C [O]; [R] = 0
    char* mpFontExt;                // 0x40 [O]; [R] = 0
};

CHECK_OFFSET(tUIFontTableHeader, mNumEntries, 0x0);
CHECK_SIZE(tUIFontTableHeader, 0x4);
CHECK_OFFSET(cUIFontEntry, mHashID, 0x0);
CHECK_OFFSET(cUIFontEntry, mpFileName, 0x4);
CHECK_OFFSET(cUIFontEntry, mpFont, 0x8);
CHECK_SIZE(cUIFontEntry, 0xC);
CHECK_OFFSET(cUIFontInterface, mpFontTableHeader, 0x00);
CHECK_OFFSET(cUIFontInterface, mFontEntry, 0x04);
CHECK_OFFSET(cUIFontInterface, mFontCount, 0x34);
CHECK_OFFSET(cUIFontInterface, unk_35, 0x35);
CHECK_OFFSET(cUIFontInterface, mpEngine, 0x38);
CHECK_OFFSET(cUIFontInterface, mpFontPath, 0x3C);
CHECK_OFFSET(cUIFontInterface, mpFontExt, 0x40);
CHECK_SIZE(cUIFontInterface, 0x44);

#endif /* UI_UIFONTINTERFACE_H */
