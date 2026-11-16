#!/usr/bin/env python3
"""Stop the SN/Cygnus EE assembler (GNU as 2.9-ee-991111b) from splitting frags at obstack chunk boundaries.

Why: the assembler's R5900 short-loop fix pads a loop shorter than 7 instructions (label through delay slot) with
nops before the back-branch. It measures the loop with first-pass frag offsets, and when the label and the branch
sit in different frags it only counts from the label to the end of the label's frag (plus 2). Two kinds of frag
break exist:
  - alignment and relaxable-macro frags (`.p2align 3` before loop labels, gp-relative `lw $2,sym` macros): these
    come from the compiled code itself, so they are the same in our build and the original. Keep them.
  - "grow" frags: gas starts a new frag whenever its frag obstack runs out of room, every 4,072-byte chunk
    (~998 instructions). Where those fall depends on everything assembled earlier in the unit. The original
    build was all C; ours mixes INCLUDE_ASM text (no alignment/macro frags) with C, and the positioned objdiff
    build has only `.org` gaps for asm functions. So a loop label that lands 1-5 instructions before one of OUR
    chunk boundaries gets 1-5 spurious nops the original didn't have, and landing any function moves the
    boundaries for every later function in the unit (the "UNPOSITIONED" family; tools/gen/hybrid_src.py's
    keep-asm list).

This patch raises libiberty's default obstack chunk size (used for the frag obstacks) from 0xFE8 to 16 MB in
_obstack_begin and _obstack_begin_1, so no unit ever reaches a chunk boundary. Measured with probe loops: chunk
boundaries every 998 instructions with the stock binary, none with this one, and identical padding for the
alignment-frag cases. Verified on the fork: the whole objdiff report is unchanged (12,767 matched, 0 functions
changed) and the asm-only build still checksums.

The Linux-native ee-gcc 2.9-ee-991111 builds (decomp.me bundle; configure.py UNIT_CC_NATIVE) have the same assembler
behaviour but take their obstacks from the host glibc, whose default chunk is also ~4 KB. Their `as` passes its own
`chunksize` variable (0, meaning "default") to every obstack_begin, so the patch sets that data word to 16 MB.
Found 2026-11-15: three of peak3's hand-written MMI routines landed together in seg_2FE810 put a chunk boundary
inside func_00404EA8's loops, and the positioned build died with `.org backwards (-12)`.

usage: tools/patch_as_chunk.py COMPILER_DIR...   (patches ee/bin/as.exe and lib/gcc-lib/ee/2.95.3/as.exe, or a
                                                    native ee/bin/as, in place; already-patched binaries are left alone)
"""
import hashlib
import sys
from pathlib import Path

STOCK_SHA1 = "ed3d3d82c8f99608cd797a6b47bb0bcaaa8afaf0"
PATCHED_SHA1 = "beaa5f7c86b1cac175d9e646bee2b5e3d6ff09ed"
# File offsets of `mov $0xfe8,%eax` (b8 e8 0f 00 00): the `size == 0` default in _obstack_begin / _obstack_begin_1.
SITES = (0x67537, 0x675C8)
NEW_SIZE = 0x01000000

def patch(path: Path) -> str:
    data = bytearray(path.read_bytes())
    sha = hashlib.sha1(data).hexdigest()
    if sha == PATCHED_SHA1:
        return "already patched"
    if sha != STOCK_SHA1:
        return f"unknown assembler (sha1 {sha}); left alone"
    for off in SITES:
        assert data[off:off + 5] == b"\xb8\xe8\x0f\x00\x00", (path, hex(off))
        data[off + 1:off + 5] = NEW_SIZE.to_bytes(4, "little")
    assert hashlib.sha1(data).hexdigest() == PATCHED_SHA1
    path.write_bytes(data)
    return "patched"

# Native i386 `as` builds: stock sha1 -> (file offset of the `chunksize` data word, patched sha1).
NATIVE = {"51e0893fd2076a4d92cd12056de85f043244377c": (0x90C64, "6b840069a224f37187334a11397d432a627a6013"),  # 991111-01
          "97bacf4d891313c7aa696215d1c3cab11ed74c78": (0x90A64, "70dd5a2b770d5f701cf850795d8348b54edd13e4")}  # 991111

def patch_native(path: Path) -> str:
    data = bytearray(path.read_bytes())
    sha = hashlib.sha1(data).hexdigest()
    if sha in {v[1] for v in NATIVE.values()}:
        return "already patched"
    if sha not in NATIVE:
        return f"unknown assembler (sha1 {sha}); left alone"
    off, want = NATIVE[sha]
    assert data[off:off + 4] == b"\0\0\0\0", (path, hex(off))
    data[off:off + 4] = NEW_SIZE.to_bytes(4, "little")
    new_sha = hashlib.sha1(data).hexdigest()
    assert new_sha == want, (path, new_sha)
    # replace, don't rewrite in place: a running `as` keeps its old inode (writing an executing binary fails)
    tmp = path.with_name(path.name + ".patching")
    tmp.write_bytes(data)
    tmp.chmod(path.stat().st_mode)
    tmp.replace(path)
    return "patched"

def main() -> None:
    for d in sys.argv[1:]:
        for rel in ("ee/bin/as.exe", "lib/gcc-lib/ee/2.95.3/as.exe"):
            p = Path(d) / rel
            if p.is_file():
                print(f"{p}: {patch(p)}")
        p = Path(d) / "ee/bin/as"
        if p.is_file():
            print(f"{p}: {patch_native(p)}")

if __name__ == "__main__":
    main()
