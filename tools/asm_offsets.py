#!/usr/bin/env python3
"""Write the target offset of every INCLUDE_ASM'd function of one unit, for position-faithful obj/current.

SN's assembler pads short loops with nops depending on where the loop sits in the section, so a matched
function only reproduces the target bytes at its target offset. obj/current (-DSKIP_ASM) contains only the
C functions; with -DASM_OFFSETS each INCLUDE_ASM becomes `.org __off_<NAME>` (include/include_asm.h), which
puts every C function at the offset it has in obj/target. The skipped asm functions leave zero-filled gaps
with no symbols, so they still count as unmatched.

usage: tools/asm_offsets.py SRC.cpp TARGET.o OUT.inc
"""
import re
import subprocess
import sys
from pathlib import Path

ASM_ROOT = Path("asm/nonmatchings")


def main() -> None:
    src, target, out = (Path(a) for a in sys.argv[1:4])
    syms = {}
    readelf = subprocess.run(["mips-linux-gnu-readelf", "-sW", str(target)], capture_output=True, text=True, check=True)
    for line in readelf.stdout.splitlines():
        f = line.split()
        if len(f) >= 8 and f[3] == "FUNC" and f[6] == "1":
            syms[f[7]] = int(f[1], 16)
    lines = []
    for folder, name in re.findall(r'^INCLUDE_ASM\("([^"]+)",\s*(\w+)\);', src.read_text(errors="ignore"), re.M):
        asm = ASM_ROOT / folder / f"{name}.s"
        m = re.search(r"^\s*glabel\s+(\S+)", asm.read_text(errors="ignore"), re.M) if asm.exists() else None
        sym = m.group(1) if m else name
        if sym in syms:
            lines.append(f"__off_{name} = 0x{syms[sym]:X}\n")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("".join(lines))


if __name__ == "__main__":
    main()
