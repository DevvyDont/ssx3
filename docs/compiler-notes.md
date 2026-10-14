# Compiler notes

The build uses SN Systems' ee-gcc 2.95.3 build 1.36 (`scripts/setup.sh`). It
matches most code byte for byte. One pattern is known not to match.

## Nearby negative offsets from one pointer

When a function reads two nearby fields at negative offsets from the same
pointer, our compiler builds the address as `-32768` plus a large offset:

```c
int k(int* m) { return m[-2] != m[-1]; }
```

```
our build                       the game's code
addu  $4,$4,-32768              lw  $3,-4($4)
lw    $3,32764($4)              lw  $2,-8($4)
lw    $2,32760($4)
```

The split is already present in the first RTL dump (`-da`, `*.rtl`), and it
happens for C as well as C++. A single negative-offset access still comes
out as a plain `lw -12(reg)`, because later passes fold it back. The
retail ELF contains no `addiu reg, reg, -0x8000` followed by a
`0x7FFx(reg)` access, so the game's compiler did not do this.

It currently blocks 4 functions in `bx/bxstring` (CopyBeforeWrite,
ConcatInPlace, FreeExtra and `operator+(const cBXString&, const char*)`),
and it will turn up wherever code reads several header fields in front of
a pointer.

## Compilers tested (decomp.me builds, October 2026)

From https://github.com/decompme/compilers (`values.yaml`):

| Compiler | Split? | Notes |
|---|---|---|
| ee-gcc2.95.3-136 (ours) | yes | 39/44 bxstring functions match |
| ee-gcc2.95.3-114 | yes | |
| ee-gcc2.95.3-107 | yes | |
| ee-gcc2.95.2-274 | yes | |
| ee-gcc2.95.2-273a | yes | |
| ee-gcc2.9-991111-01, 991111a | yes | |
| ee-gcc2.96 | no | saves registers with `sd` rather than `sq`; 1/44 bxstring match |
| ee-gcc3.2-040921 | no | different ABI and mangling |

The game saves callee-saved registers with `sq`/`lq`, which points to the
SN 2.95.x line. Every SN build available on decomp.me has the split, so the
game was probably built with a later or patched SN build that is not on
decomp.me.

Flags tried with build 1.36: `-mgp32` and `-mno-split-addresses` both
remove the split. Neither can be used: `-mgp32` drops the 64-bit register
use the game relies on (`daddu`, `sd`), and `-mno-split-addresses` stops
global addresses being split into `lui`/`addiu`, which the game needs
(bxrandom drops from 10/10 to 4/10). No other MIPS flag tested affects it.

If another SN ee-gcc build turns up (e.g. from a ProDG for PS2 3.x
install), test it on the snippet above first.

## Assembler padding in INCLUDE_ASM code

`INCLUDE_ASM` code goes through the compiler, so the SN assembler (`ee/bin/as.exe`)
assembles it. That assembler has a workaround for the R5900 short-loop
erratum. It pads some backward branches with nops even under
`.set noreorder`. For example, in `main/game` it puts 4 nops before
`bnel $20, $0, .L0022DC10` at `0x0022DCF8`, although the loop body there is
about 58 instructions. The trigger is not fully understood: a minimal
label-then-branch loop reproduces it, but other short patterns tested don't.
There is no command-line option or `.set` directive that turns it off.

Modern binutils (`mips-linux-gnu-as -march=5900`) has the opposite problem.
It assembles the included code correctly, but in compiled C it adds hazard
nops after `mtc1` and similar, because it treats the R5900 as having no
coprocessor interlocks. A leading `.set mips4` stops those nops, but it
also disables EE-only instructions (`lq`, `sq` and the VU0 macros), and no
ordering of `.set mips4` and `.set arch=r5900` fixes both.

So the build keeps the SN assembler, and the 32 units it would pad stay as
`asm` segments (listed in [splits.md](splits.md)).
