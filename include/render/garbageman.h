#ifndef RENDER_GARBAGEMAN_H
#define RENDER_GARBAGEMAN_H

// cGarbageMan: the deferred-delete base class of render objects the GS may still be drawing (cGridMesh derives from it,
// render/ps2gridmesh.h). Trash() pushes the object on the current one of four "cans"; Update() (once per frame, from
// cGame_render) moves to the next can and deletes everything on it through the virtual destructor, so an object dies
// four frames after it was trashed; Purge() runs Update() four times (cGame_purge).
// Names: [O] SSX On Tour's DWARF class cGarbageMan (size 0x8: mNext; statics mCurCan, mCan[4]; methods ~cGarbageMan,
// Trash, Update, Purge; compiled in render/all_in_one_render.cpp) and [P] the July 2003 preview's destructor
// _$_11cGarbageMan (builds/x/v2: byte-equal to cGarbageMan__cGarbageMan, 0x394B38).
// Size 0x8 [O]; [R] the class declares a virtual destructor and has 4 bytes of data, so g++ 2.95 puts the vptr at +0x4:
// vtable cGarbageMan_vtbl (1 slot: cGarbageMan__cGarbageMan), restored by the base destructor (cGarbageMan__cGarbageMan,
// cGridMesh__cGridMesh). EA's source path is not known; retail Trash / Update / Purge are in the splat unit
// render/graphicsman (0x36A1C0..0x36A2B0), the out-of-line destructor in render/font.
// Statics [O]: cGarbageMan::mCan[4] = cGarbageMan_mCan and cGarbageMan::mCurCan = cGarbageMan_mCurCan ([U] cGarbageMan_Trash /
// cGarbageMan_Update index the cans by the current-can counter, (mCurCan + 1) % 4 per Update).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] retail destructor / vtable; [O] On Tour's DWARF; [U] use.

#include "layout.h"

struct cGarbageMan {
    cGarbageMan* mNext;     // 0x0 [O] cGarbageMan::mNext; [U] cGarbageMan_Trash stores the can's old head here,
                            //     cGarbageMan_Update walks the can through it
    void** mVtbl;           // 0x4 [R] cGarbageMan_vtbl, stored by cGarbageMan__cGarbageMan before the optional delete;
                            //     [U] cGarbageMan_Update deletes through slot 1 (the derived destructor)
};

CHECK_OFFSET(cGarbageMan, mNext, 0x0);
CHECK_OFFSET(cGarbageMan, mVtbl, 0x4);
CHECK_SIZE(cGarbageMan, 0x8);

#endif /* RENDER_GARBAGEMAN_H */
