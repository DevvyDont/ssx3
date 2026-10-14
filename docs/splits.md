# File splits

`config/ssx3_us.yaml` splits the code into 169 named translation units, all `cpp` except `sce/crt0`.
Six of them (`visualfx/crowdrender2d`, `hashvalue`, `bxrandom`, `bx/bxstring`,
`md5`, `dirtysock/tags`) were split by hand. The other 162 take their
boundaries from the segment ranges in `docs/notes/functions/`. Those ranges
come from an `AI-Split-ID` branch that is not in this repository.

Before use, each range was checked against the disassembly:

- no ranges overlap
- every boundary is 8-byte aligned
- every range starts on a function splat already recognises

The rebuilt ELF still matches the original (sha1 check), but that only proves
the boundaries are byte-safe. It does not prove they are the original file
boundaries. Code between named ranges stays in the offset-named `asm` blocks.

## Names

Names come from the notes. These 35 are not in the prototype file list in
`src/readme.md`, so treat them as guesses:

- `bx/bxstringctor`
- `bx/execman`
- `camera/trigger/cameratriggerfactory`
- `dirtysock/tags`
- `fe/fepopupmisc`
- `fe/festateaudiooptions`
- `fe/festatebonusmaterial`
- `fe/festatebuyattrib`
- `fe/festatecharequipdetail`
- `fe/festatelegal`
- `fe/festateloadhint`
- `fe/festateloadscreen`
- `fe/festatelodge`
- `fe/festateriderbiodetail`
- `fe/festatetransition`
- `fe/festatetrophyroom`
- `fe/ovstateenterlodge`
- `fe/ovstatetrophy`
- `fe/ovtemplatedialog`
- `fe/ovtemplatepausemenu`
- `fe/uitemplatemap`
- `main/gameload`
- `main/loadscreens_prestart`
- `main/ssxappdtor`
- `object/bucketman`
- `object/deadfadenode`
- `object/debouncenode`
- `object/effectlink`
- `object/flagset`
- `object/instanceman`
- `object/multiparticle`
- `visualfx/boardsparkfx`
- `worldpainter/worldpaintermanquery`
- `worldpainter/worldpainterqueryctor`
- `wscript/wscriptcompile`

`bx/bxstringctor` looks wrong: per the notes, its functions are path queries,
not string code.

## Units on the older assembler

These 32 units are assembled with `bin/ee-as.exe` instead of the
driver's assembler, because the driver's assembler pads some of their
INCLUDE_ASM code with nops that the game doesn't have. They are listed as
`SN_AS_2000_UNITS` in `configure.py`; see
[compiler-notes.md](compiler-notes.md) before decompiling in one of them.

- `ai/control/handplantcontrol`
- `be/beintnewrace`
- `be/belibrary`
- `be/beintecon`
- `be/beintmission`
- `be/beintreward`
- `camera/trigger/cameratriggerman`
- `fe/festateprofile`
- `fe/feasyncfile`
- `fe/festaterewards`
- `fe/fepopupmisc`
- `fe/ovstatehud`
- `fe/ovtemplatepausemenu`
- `fe/ovstatepause`
- `fe/uitemplatemap`
- `fe/ovstatemap`
- `main/ssxapp`
- `main/game`
- `main/gamestate`
- `main/gamemode`
- `replay/replaycache`
- `sound/soundsys`
- `visualfx/avalanche`
- `visualfx/boardtrailfx`
- `animation/animmodel`
- `bx/cubicspline`
- `input/inputparse`
- `intersect/aifwddiff`
- `render/graphicsman`
- `render/particle`
- `render/lightman`
- `render/font`

## Data sections

Every object contributes its data in the same link order as its code, so
each data section is split into one chunk per text unit, in that order.
Chunks for units still in the offset-named asm blocks are anonymous.

| Section | Chunks | Named | How the boundaries were found |
|---|---|---|---|
| `.lit4` | 125 | 102 | Exact. Every word is referenced by exactly one unit, in unit order |
| `.rodata` | 187 | 148 | Fitted (below). 456 of 5,366 referenced items fall in a non-referencing unit |
| `.sdata` | 139 | 109 | Fitted. About 200 of 1,737 referenced items are shared small globals |
| `.data` | 61 | 51 | Fitted, but only 539 items are referenced from code, so many boundaries are approximate |

`.sbss` and `.bss` are not split yet. They take no file space, so they don't
affect the checksum; C code can reference their symbols as `extern`.

The fit takes every data address referenced from code (`%lo` symbol references
and gp-relative offsets), with the units that reference it, and assigns each
item to a unit in link order (never going backwards). It minimises items
assigned to a unit that doesn't reference them, weighting items referenced by
several units less, since those are shared. Each chunk starts at its first
referenced item. Shared tables and vtables explain most of the leftover
mismatches: a class's vtable is emitted by one file but referenced by
constructors and destructors elsewhere. Unreferenced data between two units'
items is given to the earlier unit, which may be wrong.

Each chunk starts as a standalone asm object (`asm/data/<unit>.<section>.s`).
`configure.py` places the data in an output section without padding
(`.main_data`, `SUBALIGN(1)`), since each chunk is an exact byte range of the
original.

### Decompiling functions that use data

Compiled C emits its own float constants, jump tables and strings into its
object, in the order the functions appear. So for a unit with decompiled
code, its `.lit4`, `.rodata` and `.sdata` have to come from the cpp object,
not from the standalone chunk. `configure.py` (`migrate_unit_data`) does this
for every cpp unit on each configure:

- Each item in the chunk is appended, in a `.section` block, to the
  nonmatching .s of the first INCLUDE_ASM function that references it, so
  INCLUDE_ASM emits it in the original place. The linker script then takes
  that section from the cpp object instead of the chunk.
- `.lit4` words and jump tables that no INCLUDE_ASM function references are
  dropped: only decompiled functions can be using them, and the compiler
  emits them again. Other unreferenced items stay with the preceding
  function.
- A unit with no INCLUDE_ASM functions left takes all of its data from the
  compiled C.
- If giving the items to their functions would change the chunk's byte
  order, the chunk stays standalone. `obj/standalone_data.txt` lists these
  (38 chunks, all "data order differs from function order"). Functions in
  them can still be decompiled if they use no data from that section.

`.data` is not migrated. Vtables and other initialised data still come from
the chunks, so constructors and destructors that would make the compiler
emit a vtable can't be decompiled yet.

Decompiled float constants match as long as each function's literals are in
the original order (the compiler emits one `.lit4` word per `li.s`, in
instruction order, with no merging). `be/beintcharacter`'s
`getModelScale`, with a jump table and eight `.lit4` constants, is the first
function matched this way.

Jump table targets in the asm use `jlabel` (`include/macro.inc`), a global
label that isn't typed as a function, so it doesn't split the function in
objdiff. The SN assembler gives it the object type, so
`scripts/diff_func.sh` disassembles with `-D`.
