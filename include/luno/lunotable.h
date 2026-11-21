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

// luno::cLunoTableData: a Luno table's shared, reference-counted hash data (On Tour class cLunoTableData, 0x14:
// RefCount 0x8, Modulus 0xC, Entries 0x10 after its base cLunoTableDataState). [R] luno_cLunoTable_constructTable
// allocates 0x14 bytes tagged "LunoTable" and stores Entries, RefCount and Modulus; the dtor is
// luno_cLunoTableData__cLunoTableData. Flattened as luno_cLunoTableData. Local definitions it replaces: src/luno/lunovm.cpp's
// struct cLunoTable {pad[8]; refCount; modulus; nodes}.
struct luno_cLunoTableData {
    char unk_00[0x8];                   // 0x00 [O] the base cLunoTableDataState (its own RefCount / Modulus words); no
                                        //     retail access seen
    int RefCount;                       // 0x08 [O]; [R] constructTable 0, then 1; [U] operator= / destroyTable count it
    int Modulus;                        // 0x0C [O]; [R] constructTable n - 1; [U] get / set hash with it, emptyTable
                                        //     allocates Modulus + 1 entries
    luno_cLunoTableEntry* Entries;      // 0x10 [O]; [R] constructTable: new cLunoTableEntry[n] ("TableData", array
                                        //     cookie); [U] the dtor and emptyTable destroy them
};

// luno::cLunoTable: a handle to shared table data (On Tour class cLunoTable, 0x4 {cLunoTableData* Data}; ctors
// cLunoTable(int numEntries) and cLunoTable(const cLunoTable&), dtor, operator=, get, set, emptyTable, ...). The retail
// methods are luno_cLunoTable_* (src/luno/lunovm.cpp); a type-4 (table) luno_cLunoVar keeps the handle in its Data
// word. Flattened as luno_cLunoTable; the unit's struct cLunoTableRef {cLunoTable* table} is the same type.
// The copy constructor is declared because EA's class has one ([O]): it makes the class non-POD, so g++ 2.95 keeps a
// local handle in memory (BLKmode) - [R] luno_cLunoVar_readFromReplayFrame builds its type-4 temporary in a stack slot
// shared with the type-5 cLunoFunc (the view cLunoRefTmp_225CA0). No destructor is declared: the C code calls
// luno_cLunoTable__cLunoTable by hand.
struct luno_cLunoTable {
    luno_cLunoTableData* Data;          // 0x0 [O]; [U] every method goes through it (lw 0x0)

    luno_cLunoTable() {}
    luno_cLunoTable(const luno_cLunoTable& table); // [O]; not called by the matched code
};

CHECK_OFFSET(luno_cLunoTableData, unk_00, 0x00);
CHECK_OFFSET(luno_cLunoTableData, RefCount, 0x08);
CHECK_OFFSET(luno_cLunoTableData, Modulus, 0x0C);
CHECK_OFFSET(luno_cLunoTableData, Entries, 0x10);
CHECK_SIZE(luno_cLunoTableData, 0x14);

CHECK_OFFSET(luno_cLunoTable, Data, 0x0);
CHECK_SIZE(luno_cLunoTable, 0x4);

#endif /* LUNO_LUNOTABLE_H */
