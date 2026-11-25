#ifndef UTIL_LOCALE_H
#define UTIL_LOCALE_H

// cFELocale: the game's string table (one per app: SSXApp->FELocale, which cSSXApp_initLocale creates and points at
// the options' language). It keeps up to 20 loaded cLocaleFiles (data/locale/*.loc: the common file per language,
// the front-end set or the in-game set) and answers string lookups by hash or index for the UI
// (cUILocaleInterface, the UI engine's mpLocaleInterface).
// EA's source is util/locale.cpp (the July 2003 preview's assert strings; retail unit util/locale). Retail code: the
// ctor, dtor and deleteAllFiles in fe/festatecharequip (0x00195880 ... 0x00195948), the rest in util/locale
// (0x001959A8 ... 0x00195F70); the base cUILocaleInterface's out-of-line methods in main/all_in_one_main
// (0x001DCBD8 ... 0x001DCC58).
//
// Size 0xC0:
//   [R] cMemMan_alloc(0xC0, "FELocale") + cFELocale_cFELocale(mem) in cSSXApp_initLocale; the highest use is the
//       string buffer at 0x5A (getStringByHash), 50 chars up to 0xBE, then 2 bytes of padding.
//   [O] On Tour 0x88 = cUILocaleInterface 0x8 + cLocaleFile* mpLocaleFile[6] + char mModule + unsigned short
//       TempString[50]; retail has 20 file slots (the ctor clears 80 bytes at 0x8; every loop runs to 20): 0x88 +
//       14 * 4 = 0xC0.
// Base: cUILocaleInterface (On Tour 0x8: four chars, then the vptr at 0x4; vtable cUILocaleInterface_vtbl
// 0x0046D848, 4 slots). The ctor stores cUILocaleInterface_vtbl, runs tAttrLocale_tAttrLocale on byte 3, then stores
// cFELocale_vtbl (0x0046BF98, 5 slots: 1 dtor, 2 cUILocaleInterface_setDisplayMode, 3 setLanguage, 4 getStringByHash,
// 5 getStringByIndex). No cUIState: this is not a screen.
//
// Real classes: cFELocale derives from cUILocaleInterface and both declare their virtuals, so callers write
// `SSXApp->FELocale->getStringByHash(hash)`. The vptr lands at 0x4 by g++ 2.95's own rule (after the own data of the
// first class in the hierarchy that declares a virtual: cUILocaleInterface's four chars). Nothing here is defined as
// a C++ member (the methods are the extern "C" functions named above), so no vtable or type_info is emitted; the
// hand-written ctor / dtors store the vptr with VPTR_OF(cUILocaleInterface, self) (include/vtable.h).
// Evidence tags:
//   [R] retail's constructor cFELocale_cFELocale / destructor cFELocale__cFELocale and the allocation size;
//   [P] the July 2003 preview (layout_xlate: the vptr at 0x4 in ~cUILocaleInterface);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFELocale, cUILocaleInterface, struct tAttrLocale): names, types and order.

#include "layout.h"

struct cLocaleFile;

// EA's tAttrLocale (On Tour struct tAttrLocale, size 0x1, LSB first): cUILocaleInterface's attribute byte at 0x3.
// [R] tAttrLocale_tAttrLocale (lbu / andi 0xE0 / sb) clears the five bits; [U] setTextNumCharFlag sets mTextNumChar
// (bit 27 of the word at 0x0: g++ widens a bit-field store in a 4-aligned object to the word, as cUIState's tAttrState).
struct tAttrLocale {
    unsigned char mLocaleTest : 1;  // bit 0 [O]; [U] setLanguage / unloadFE / unloadOV keep the files while it is set
    unsigned char mObjPivot : 1;    // bit 1 [O]
    unsigned char mTextBox : 1;     // bit 2 [O]
    unsigned char mTextNumChar : 1; // bit 3 [O]; [U] cUILocaleInterface_setTextNumCharFlag (bit 27 of word 0)
    unsigned char mTextRefresh : 1; // bit 4 [O]
};

class cUILocaleInterface {
public:
    signed char mDisplayMode;       // 0x000 [O] char; [R] 0; [U] cUILocaleInterface_setDisplayMode stores it;
                                    //       getStringByHash returns the blank buffer unless it is 0
    signed char mLanguage;          // 0x001 [O] char; [R] 0; [U] setLanguage compares and stores it; loadCommon /
                                    //       loadFE / loadOV switch on it; getStringByHash matches files on it
    signed char mNumLanguages;      // 0x002 [O] char; [R] the base part stores 0, cFELocale's ctor 4
    tAttrLocale mAttrLocale;        // 0x003 [O] tAttrLocale (1 byte, LSB first: mLocaleTest:1 mObjPivot:1 mTextBox:1
                                    //       mTextNumChar:1 mTextRefresh:1); [R] tAttrLocale_tAttrLocale clears the low
                                    //       5 bits; [U] setLanguage / unloadFE / unloadOV keep the files while bit 0
                                    //       (mLocaleTest) is set
    // 0x004: the vptr. [R] cUILocaleInterface_vtbl (0x0046D848), then cFELocale_vtbl (ctor); the dtor stores them in
    //        reverse; [P] preview ~cUILocaleInterface. Slots [R] cUILocaleInterface_vtbl, names [O]:
    virtual ~cUILocaleInterface();                                   // slot 1 cUILocaleInterface__cUILocaleInterface
    virtual void setDisplayMode(signed char mode);                   // slot 2 cUILocaleInterface_setDisplayMode
    virtual void setLanguage(signed char language);                  // slot 3 cUILocaleInterface_setLanguage
    virtual unsigned short* getStringByHash(unsigned int StringID);  // slot 4 cUILocaleInterface_getStringByHash

    // Methods bound to their retail symbols by asm labels.
    void setTextNumCharFlag(int enabled) __asm__("cUILocaleInterface_setTextNumCharFlag");
};
CHECK_OFFSET(cUILocaleInterface, mDisplayMode, 0x000);
CHECK_OFFSET(cUILocaleInterface, mLanguage, 0x001);
CHECK_OFFSET(cUILocaleInterface, mNumLanguages, 0x002);
CHECK_OFFSET(cUILocaleInterface, mAttrLocale, 0x003);
CHECK_SIZE(cUILocaleInterface, 0x8);

class cFELocale : public cUILocaleInterface {
public:
    cLocaleFile* mpLocaleFile[20];  // 0x008 [O] ([6] in On Tour); [R] the ctor clears 80 bytes; [U] addFile creates
                                    //       entries 0-3 (cLocaleFile_loadFile into a 0x14-byte "LocaleFile"),
                                    //       loadFE / loadOV add 4-19, deleteFile / deleteAllFiles free them,
                                    //       getStringByHash / getStringByIndex search all 20
    signed char mModule;            // 0x058 [O] char; [R] 0; [U] loadFE sets 1, loadOV 2; setLanguage reloads the
                                    //       front-end set when it is 1, else the in-game set
    char unk_059[0x1];              // 0x059 padding
    unsigned short TempString[50];  // 0x05A [O]; [U] getStringByHash's fallback: sprintf2 writes " " here and returns
                                    //       it
    char unk_0BE[0x2];              // 0x0BE tail padding
    // cFELocale_vtbl (0x0046BF98) [R]; slot 2 stays cUILocaleInterface_setDisplayMode
    virtual ~cFELocale();                                            // slot 1 cFELocale__cFELocale
    virtual void setLanguage(signed char language);                  // slot 3 cFELocale_setLanguage
    virtual unsigned short* getStringByHash(unsigned int StringID);  // slot 4 cFELocale_getStringByHash
    virtual unsigned short* getStringByIndex(unsigned int index);    // slot 5 cFELocale_getStringByIndex

    // Methods bound to their retail symbols by asm labels.
    void deleteAllFiles() __asm__("cFELocale_deleteAllFiles");
};

CHECK_SIZE(tAttrLocale, 0x1);

CHECK_OFFSET(cFELocale, mpLocaleFile, 0x008);
CHECK_OFFSET(cFELocale, mModule, 0x058);
CHECK_OFFSET(cFELocale, unk_059, 0x059);
CHECK_OFFSET(cFELocale, TempString, 0x05A);
CHECK_OFFSET(cFELocale, unk_0BE, 0x0BE);
CHECK_SIZE(cFELocale, 0xC0);

#endif /* UTIL_LOCALE_H */
