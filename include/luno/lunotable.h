#ifndef LUNO_LUNOTABLE_H
#define LUNO_LUNOTABLE_H

// luno::cLunoTableEntry: one key / value slot of a Luno table's hash array (luno::cLunoTable's data holds an array of
// Modulus + 1 of them, allocated as "TableData" with an array cookie; collisions chain through Next). Retail gives it
// a virtual destructor, so the vptr sits after the data at +0x1C (On Tour's PSP build has the same 0x20 size, no
// vptr: its 4 bytes there are padding).
//
// Name: EA's luno::cLunoTableEntry (the July 2003 preview's linkonce sections _vt$Q24luno15cLunoTableEntry and
// _$_Q24luno15cLunoTableEntry; d:/ssx/code/source/luno/lunotable.cpp; retail's allocation tag "cLunoTableEntry"; On
// Tour's DWARF: class cLunoTableEntry, 0x20, {cLunoVar Index; cLunoVar Value; cLunoTableEntry* Next}). Flattened as
// luno_cLunoTableEntry (the project's luno_cLunoTableEntry__cLunoTableEntry). Vtable D_00479890 (slot 1 the dtor).
// Local definitions it replaces: src/luno/lunovm.cpp's struct cLunoTableEntry {pad[0x1C]; cLunoVtEntry* vtable}.
//
// FLAT layout. Evidence tags: [O] On Tour's DWARF; [P] the preview; [U] retail use.

#include "layout.h"
#include "luno/lunovar.h"

struct luno_cLunoTableEntry {
    luno_cLunoVar Index;                // 0x00 [O]; [U] the dtor destroys it (luno_cLunoVar__cLunoVar(self, 2))
    luno_cLunoVar Value;                // 0x0C [O]; [U] the dtor destroys it first
    luno_cLunoTableEntry* Next;         // 0x18 [O]; [U] the dtor deletes the chain through it (its slot 1, flags 3)
    void** mVtbl;                       // 0x1C [U] D_00479890 (the dtor stores it; the table code's placement new too) [P]
};

CHECK_OFFSET(luno_cLunoTableEntry, Index, 0x00);
CHECK_OFFSET(luno_cLunoTableEntry, Value, 0x0C);
CHECK_OFFSET(luno_cLunoTableEntry, Next, 0x18);
CHECK_OFFSET(luno_cLunoTableEntry, mVtbl, 0x1C);
CHECK_SIZE(luno_cLunoTableEntry, 0x20);

#endif /* LUNO_LUNOTABLE_H */
