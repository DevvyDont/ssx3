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
  addresses that aren't 8-aligned: those can't be linked from C, so they
  stay as asm. `scripts/unwrap_matched.py` skips them; delete their C rather
  than leave it wrapped behind the asm.
- The compiler is the stock SN ee-gcc 2.95.3 V1.36 download (plus the stock
  2.95.3-114 build for `ealib/seg_2D7498`), unmodified. A function that only
  matches with a patched compiler, or that is mostly inline asm, stays asm.
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
symbol (`extern`), so those sections stay whole asm objects, except for jump
tables (below). String literals in C would still need the same per-unit
treatment.

## Jump tables (.rodata)

A `switch` compiled to a jump table puts the table in `.rodata`. To link
those from C:

- C is compiled with `-S`, then `tools/jt_sections.py` moves each jump table
  into its own section (`.rodata.jt0`, `.rodata.jt1`, ... in the order the
  compiler emits them), and the same driver assembles the result.
- `configure.py` (`link_c_jump_tables`) finds the tables no asm uses any more
  (their function is now C). It cuts each one out of the asm `.rodata`, which
  splits that file into parts (`.rodata.p1`, ...), and the linker script
  places the C table between the parts at its retail address. The k-th table
  of a unit by address is the unit's `.rodata.jtk`.
- The parts are linked unaligned, so in every part after the first, each
  item is placed with `.org` (its offset from the part's start) instead of
  `.align`.

So a `switch` function is matched like any other: write the C, remove the
`INCLUDE_ASM`, and `./configure.py && ninja` has to end with `OK`. Rerun
configure whenever an `INCLUDE_ASM` is removed, as before.

The data chunks are exact byte ranges, so the data is linked in its own
output section without padding (`.main_data`, `SUBALIGN(1)`).

## Symbols

- splat lists every `.bss` symbol the asm references in
  `config/undefined_syms_auto.txt`. It runs without its cache, since cached
  segments aren't scanned for references.
- C can refer to a global by its splat name (`D_004FF160`, `func_0014A0B0`)
  after no asm references it any more. `tools/address_syms.py` defines each
  such symbol the linked objects leave undefined at the address in its name.
