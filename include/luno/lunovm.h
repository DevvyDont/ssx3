#ifndef LUNO_LUNOVM_H
#define LUNO_LUNOVM_H

// luno::cLunoVM: a Luno script virtual machine - just its register array (On Tour class cLunoVM, 0x4:
// cLunoVar* Registers). Script callers build one on the stack, run a function on it (luno_cLunoVM_run) and let it go.
// Retail: the constructor luno_cLunoVM_cLunoVM is empty (jr ra); cLunoVM::execute frees the register array on exit
// (src/luno/lunovm.cpp's local class cLunoVM { cLunoVar* m_Regs; }). Flattened as luno_cLunoVM, like luno_cLunoVar.
// Users: wscript (cWScriptMan_runScript: view sLunoCall_9C88, a 0x10-byte stack slot), luno/lunovm.

#include "layout.h"
#include "luno/lunovar.h"

struct luno_cLunoVM {
    luno_cLunoVar* Registers;       // 0x0 [O]; [U] cLunoVM::execute frees it (lunovm.cpp m_Regs)
};

CHECK_OFFSET(luno_cLunoVM, Registers, 0x0);
CHECK_SIZE(luno_cLunoVM, 0x4);

#endif /* LUNO_LUNOVM_H */
