#ifndef BX_MEMMAN_H
#define BX_MEMMAN_H

// The game's global operator delete: the routine at 0x00317E50 (bx/memman, retail label operator_delete__FPi).
// g++ 2.95 calls the global operator delete by the assembler name __builtin_delete (from `delete p` and from the
// deleting destructors' `if (flags & 1)` tail), so that name is bound to the retail routine here: an equate, no code
// (gas resolves the relocations to operator_delete__FPi and keeps __builtin_delete out of the symbol table).
__asm__("__builtin_delete = operator_delete__FPi");

#endif /* BX_MEMMAN_H */
