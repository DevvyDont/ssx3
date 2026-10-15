# Linking a byte-identical executable

`./configure.py && ninja` builds `out/SLUS_207.72` from the decompiled C of
matched functions and the original asm of everything else, and checks its
sha1. These are the things that make C and asm link back into the retail
bytes.

## Functions and units

- Every function has to be in a source file, as C or as `INCLUDE_ASM`. A
  function in neither is in no object, and everything after it moves.
- Functions are 8-aligned. A unit ending in C has no trailing nop, so the
  linker aligns each object to 8 (`subalign: 8`) to recreate it. A few
  hand-written asm routines (VU0 code in `ealib/seg_2B4578`) sit at
  addresses that aren't 8-aligned: those can't be linked from C, and
  `scripts/unwrap_matched.py` leaves them wrapped.
- C++ units are compiled with `-fno-exceptions`; otherwise every object gets
  0x18 bytes of exception info in `.data` that the game doesn't have.

## The assembler's loop padding

The SN assembler the game was built with pads short loops with nops (an
R5900 erratum workaround), so compiled C gets the padding it needs. The asm
already contains the original padding, but the assembler would pad some of
its loops again, depending on where they sit. `configure.py` writes every
branch in the split asm as a `.word` of its original encoding
(`freeze_asm_branches`), so the assembler sees no loops in the asm.

## Float constants (.lit4)

The compiler emits a float constant as `li.s`, and the assembler puts the
value in the object's `.lit4`: one entry per load, in instruction order. To
reproduce each unit's `.lit4` with any mix of C and asm:

- `scripts/split_data.py` splits `.lit4` into one chunk per unit. Every
  constant is loaded by exactly one instruction, so this is exact.
- `configure.py` rewrites each asm load of a constant as `li.s` with its
  value (`inline_lit4_loads`), which assembles to the same instruction, and
  links each C unit's `.lit4` from its object instead of the chunk.

The decompiled C refers to everything in `.data`, `.rodata` and `.sdata` by
symbol (`extern`), so those sections stay whole asm objects. Once C emits
data of its own there (string literals, jump tables from `switch`), those
sections need the same per-unit treatment.

The data chunks are exact byte ranges, so the data is linked in its own
output section without padding (`.main_data`, `SUBALIGN(1)`).

## Symbols

- splat lists every `.bss` symbol the asm references in
  `config/undefined_syms_auto.txt`. It runs without its cache, since cached
  segments aren't scanned for references.
- C can refer to a global by its splat name (`D_004FF160`, `func_0014A0B0`)
  after no asm references it any more. `tools/address_syms.py` defines each
  such symbol the linked objects leave undefined at the address in its name.
