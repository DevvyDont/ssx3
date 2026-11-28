#ifndef FE_FESTATERIDERBIO_H
#define FE_FESTATERIDERBIO_H
#pragma interface

// cFEStateRiderBio: the rider bio screen (a cFEState); cFEReal creates it with cMemMan_alloc(80, "cFEStateRiderBio").
// Size 0x50 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateRiderBio : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateRiderBio
    int mCharID;                    // 0x048 [O]
    int mScreen;                    // 0x04C [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRiderBio1() __asm__("cFEStateRiderBio_cFEStateRiderBio1");
    void setupBioScreen() __asm__("cFEStateRiderBio_setupBioScreen");
    void setupCharPics() __asm__("cFEStateRiderBio_setupCharPics");
    void fillDNAInfo() __asm__("cFEStateRiderBio_fillDNAInfo");
    void fillFavesInfo() __asm__("cFEStateRiderBio_fillFavesInfo");
    void fillQnAInfo() __asm__("cFEStateRiderBio_fillQnAInfo");
    void fillBioInfo() __asm__("cFEStateRiderBio_fillBioInfo");
    virtual ~cFEStateRiderBio();   // g++'s own symbol _$_16cFEStateRiderBio (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateRiderBio_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cFEStateRiderBio_onWidgetEvent");
};

CHECK_OFFSET(cFEStateRiderBio, mCharID, 0x048);
CHECK_OFFSET(cFEStateRiderBio, mScreen, 0x04C);
CHECK_SIZE(cFEStateRiderBio, 0x50);
__asm__("_vt$16cFEStateRiderBio = cFEStateRiderBio_vtbl");   // g++'s name for the retail vtable

#endif
