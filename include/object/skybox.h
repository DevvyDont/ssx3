#ifndef OBJECT_SKYBOX_H
#define OBJECT_SKYBOX_H
#pragma interface

// cSkyBox: the sky dome. A scene node (cObjNode) that draws one world-section instance (the sky model of the current
// sky section, 44..48) centred on the current view with its own draw state; cSectionMan_skySwap switches the section
// (reset + setSection) once a pending sky swap is triggered. cGame_load creates the only instance (cGame::SkyBox,
// main/game.h +0x14; cGame_getSkyBox returns it).
// EA's source path is not in the preview's assert strings; On Tour compiles it in object/all_in_one_object.cpp, and
// retail keeps its methods in splat unit object/modifierblock (0x00353AC0 ctor .. 0x00353D98 reset), hence object/.
//
// Size 0x18: [R] cGame_load does cMemMan_alloc(0x18, "SkyBox") + cSkyBox_cSkyBox (src/main/gameload.cpp); the ctor
// writes 0x4-0x14. On Tour: 0x14 (its cObjNode is 0xC, 4 bytes smaller; m_iSectionID 0xC, m_pInstanceSky 0x10).
// Base cObjNode (object/objnode.h; retail 0x10: unk_00, Next, Prev, vptr).
// The ctor inlines cObjNode() (zeroes Next/Prev) and stores cSkyBox_vtbl (0x0048F008, 13 cObjNode
// slots: 1 dtor cSkyBox__cSkyBox__FPv, 4 render cSkyBox_render; 2, 3 and 5-13 are cObjNode's defaults).
//
// Derives from cObjNode; its own members FLAT, like main/game.h. Evidence tags:
//   [R] retail ctor cSkyBox_cSkyBox__FPv, dtor cSkyBox__cSkyBox__FPv and the 0x18 allocation;
//   [P] the July 2003 preview's inline getSection__7cSkyBox (lw 0x10); tools/layout_xlate.py sees preview == retail
//       at 0xC (the dtor's vptr store) and nothing else to measure; retail inlines getSection in cSectionMan_skySwap
//       (lw 0x10);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cSkyBox 0x14: m_iSectionID, m_pInstanceSky).

#include "layout.h"
#include "object/objnode.h"

struct cObjNode;
struct tInstanceRecord;

struct cSkyBox : public cObjNode {
    // cObjNode (object/objnode.h, 0x10): the vptr at 0x0C:
    //   [R] cSkyBox_vtbl (ctor and dtor)
    // cSkyBox's notes on cObjNode's fields:
    //   Next: [R] the ctor zeroes it (inlined cObjNode())
    //   Prev: [R] the ctor zeroes it
    // --- cSkyBox (On Tour 0xC..0x14)
    int m_iSectionID;                   // 0x10 [O]; [P] getSection (lw 0x10); [R] ctor -1; [U] setSection stores the id,
                                        //      reset -1; cSectionMan_skySwap compares it with the next sky section
    tInstanceRecord* m_pInstanceSky;    // 0x14 [O]; [R] ctor 0; [U] setSection stores record 0 of the section's
                                        //      resource set, reset 0; render skips when 0, sets bit 2 / clears bit 1 of
                                        //      the record's +0x8 flags and draws it (graphics slot 95, model +0x80)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cSkyBox();   // g++'s own symbol _$_7cSkyBox (the retail label renamed)
    void setSection(int id) __asm__("cSkyBox_setSection");
    int reset() __asm__("cSkyBox_reset__FPv");
    virtual void render() __asm__("cSkyBox_render");
};

CHECK_OFFSET(cSkyBox, m_iSectionID, 0x10);
CHECK_OFFSET(cSkyBox, m_pInstanceSky, 0x14);
CHECK_SIZE(cSkyBox, 0x18);
__asm__("_vt$7cSkyBox = cSkyBox_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_SKYBOX_H */
