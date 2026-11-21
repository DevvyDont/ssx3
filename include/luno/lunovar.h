#ifndef LUNO_LUNOVAR_H
#define LUNO_LUNOVAR_H

// luno::cLunoVar: a Luno script value (nil / int / float / index / table / function), 12 bytes: 8 bytes of data and the
// type. Used everywhere in luno/lunovm and by the script engines (wscript). The data holds a luno::cLunoTable handle
// (one pointer) for tables and a luno::cLunoFunc (luno/lunofunc.h, 8 bytes) for functions.
//
// Name: EA's luno::cLunoVar (the July 2003 preview's linkonce sections castInt__CQ24luno8cLunoVar,
// castFloat__CQ24luno8cLunoVar, __pl__CQ24luno8cLunoVarRCQ24luno8cLunoVar and its asserts from
// ../source/luno/lunovar.h, "Type == eLunoVar_Int" ...; On Tour's DWARF: class cLunoVar, 0xC, {anonymous union at
// 0x0, eLunoVarType Type at 0x8}). Flattened as luno_cLunoVar (the project's luno_cLunoVar_* functions).
// eLunoVarType (On Tour): eLunoVar_Nil 0, eLunoVar_Int 1, eLunoVar_Float 2, eLunoVar_Index 3, eLunoVar_Table 4,
// eLunoVar_Func 5.
// Local definitions it replaces: src/luno/lunovm.cpp's cLunoValue {word0, word1, type} and the SKIP_ASM view
// class cLunoVar {union Data; eLunoVarType Type} (peak3project's names for the union members are kept).
//
// FLAT layout. Evidence tags: [O] On Tour's DWARF; [P] the preview's asserts; [U] retail use.

#include "layout.h"

union luno_cLunoVarData {
    int Int;                            // eLunoVar_Int [P]
    float Float;                        // eLunoVar_Float [P]
    unsigned int Index;                 // eLunoVar_Index [P]
    void* Table;                        // eLunoVar_Table: the luno::cLunoTable handle (its table data pointer) [U]
    int Raw[2];                         // eLunoVar_Func: a luno::cLunoFunc {Chunk, FuncNum} [U]
};

struct luno_cLunoVar {
    luno_cLunoVarData Data;             // 0x0 [O] (anonymous union)
    int Type;                           // 0x8 [O] eLunoVarType; [U] cLunoVar_writeToReplayFrame / readFromReplayFrame,
                                        //     the table code (type 4 = table)
};

CHECK_SIZE(luno_cLunoVarData, 0x8);
CHECK_OFFSET(luno_cLunoVar, Data, 0x0);
CHECK_OFFSET(luno_cLunoVar, Type, 0x8);
CHECK_SIZE(luno_cLunoVar, 0xC);

#endif /* LUNO_LUNOVAR_H */
