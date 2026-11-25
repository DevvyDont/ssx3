#ifndef FE_FERIDERMANAGER_H
#define FE_FERIDERMANAGER_H

// cFERiderManager: the front end's two human riders (the 3D characters of the character select / setup, equip gear and
// lodge screens). It is a member of the front-end module: cFE_cFE builds it in place at cFE + 0xB0 (On Tour:
// cFE::mRiderManager at 0xB0), and the FE states reach it as RealFE + 0xB0 (cFERiderManager_getHuman1(RealFE + 0xB0,
// userNum) & co.). Every method loops over or indexes mHumans.
// EA's source: d:/ssx/code/source/fe/feridermanager.cpp (preview strings); retail code: fe/feridermanager
// (0x001A03C0 ... 0x001A0608).
//
// Size 0x19C0 = 2 x cFERider (0xCE0, fe/ferider.h):
//   [R] the ctor constructs two riders 0xCE0 apart (cFERider_cFERider), the destructor destroys them back from
//       this + 0x19C0, purge / render / update loop to this + 0x19C0; and the FE file cache that follows it in cFE is
//       at RealFE + 0x1A70 = 0xB0 + 0x19C0.
//   [O] On Tour's class is 0x3300: cFERider mHumans[4] (4 x 0xCC0); retail has 2 riders of 0xCE0.
// No base class and no virtual functions (no vtable store in the constructor). 16-byte aligned (cFERider's tPoses).
//
// A FLAT layout, like main/game.h: every field at its retail offset.
// Evidence tags:
//   [R] retail's constructor / destructor and the loops of the methods;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFERiderManager): names, types and order.
// (No preview evidence: layout_xlate finds no paired cFERiderManager methods.)

#include "layout.h"
#include "fe/ferider.h"

struct cFERider;

struct cFERiderManager {
    cFERider mHumans[2];            // 0x0000 [O] (On Tour [4]); [R] ctor / dtor build and destroy 2, 0xCE0 apart;
                                    //       [U] getHuman / getHuman1 return &mHumans[i]; init sets each
                                    //       mProfileIndex to i; setHumanCharacter / setHumanVisible forward to
                                    //       cFERider_setCharID / setVisible; isHumanVisible / render read mVisible;
                                    //       purge / update / render run over both

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("cFERiderManager_init");
    void purge() __asm__("cFERiderManager_purge");
    void setHumanCharacter(int idx, int charId, int loadAll) __asm__("cFERiderManager_setHumanCharacter");
    void* getHuman(int index) __asm__("cFERiderManager_getHuman");
    cFERider* getHuman1(int index) __asm__("cFERiderManager_getHuman1");
    int isHumanVisible(int index) __asm__("cFERiderManager_isHumanVisible");
    void setHumanVisible(int i, int visible) __asm__("cFERiderManager_setHumanVisible");
    void render() __asm__("cFERiderManager_render");
    void update() __asm__("cFERiderManager_update");
};

CHECK_OFFSET(cFERiderManager, mHumans, 0x0000);
CHECK_SIZE(cFERiderManager, 0x19C0);

#endif /* FE_FERIDERMANAGER_H */
