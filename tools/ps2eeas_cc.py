#!/usr/bin/env python3
"""Compile a unit for obj/current and assemble it with SN's Ps2EeAs instead of the driver's GNU as.

    python3 tools/ps2eeas_cc.py --unit UNIT --as PS2EEAS.exe -- <compiler argv with -c SRC ... -o OUT.o>

Why (fork): the retail newlib unit (seg/seg_316FB8) was assembled with SN's own Ps2EeAs, not GNU as. Only Ps2EeAs
expands `dli 0xffffffff` as `li -1; dsrl32`, builds 64-bit constants with its ori/dsll16/dsll14 chain and pads
short loops that contain an inner exit branch; with GNU as those functions can't reach 100% from any C. The
normal (linked) build is not affected: it assembles every function from its asm.

What it does: compile to assembly (-S, without -DASM_OFFSETS: Ps2EeAs can't read `.include`/`.org`), repoint the
switch jump tables with tools/jtbl_cc.py (verified against Ps2EeAs's own layout: ps2eeas_symbols), then assemble the
result with Ps2EeAs in a local temp dir (it fails on large files on some mounts, and wants LF input).
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import jtbl_cc  # noqa: E402


def ps2eeas_symbols(text: str, ps2as: str, tmp: Path) -> dict[str, int]:
    """{name: .text offset} of the functions and of every label a switch jump table names, in Ps2EeAs's layout.

    gas lays out a function differently where only Ps2EeAs's expansions apply (64-bit constants, short-loop
    padding), so its label offsets can't verify a table. Ps2EeAs keeps no local labels in its symbol table, but a
    table's words are REL R_MIPS_32 relocations against .text whose in-place addends are the label offsets: mark
    each table with a global symbol (in .rdata, so the code is untouched), assemble, and read the words."""
    marked = jtbl_cc.TABLE.sub(lambda m: m.group(0).replace(
        m.group(1) + ":", f"\t.globl\t__jtchk_{m.group(1)[2:]}\n__jtchk_{m.group(1)[2:]}:\n{m.group(1)}:", 1), text)
    src, obj, raw = tmp / "jtchk.s", tmp / "jtchk.o", tmp / "jtchk.rodata"
    src.write_text(marked, newline="\n")
    rr = subprocess.run(["wine", ps2as, "-G0", str(src), "-o", str(obj)], capture_output=True, text=True,
                        env=dict(os.environ, WINEDEBUG="-all"))
    if rr.returncode or not obj.exists():
        sys.stderr.write(rr.stdout + rr.stderr)
        sys.exit(rr.returncode or 1)
    syms, marks, secsyms = {}, {}, {}
    for line in subprocess.run(["mips-linux-gnu-readelf", "-sW", str(obj)], capture_output=True, text=True,
                               check=True).stdout.splitlines():
        f = line.split()
        if len(f) >= 8 and f[0].endswith(":") and re.fullmatch(r"[0-9a-fA-F]{8}", f[1]):
            if f[3] == "FUNC":
                syms[f[7]] = int(f[1], 16)
            elif f[7].startswith("__jtchk_"):
                marks["$L" + f[7][len("__jtchk_"):]] = int(f[1], 16)
    relocs = {}
    for line in subprocess.run(["mips-linux-gnu-readelf", "-rW", str(obj)], capture_output=True, text=True,
                               check=True).stdout.split("Relocation section")[1:]:
        if not line.lstrip().startswith("'.rel.rodata'"):
            continue
        for r in re.finditer(r"^([0-9a-f]{8})\s+[0-9a-f]{8}\s+R_MIPS_32\s+([0-9a-f]{8})\s+(\S+)", line, re.M):
            relocs[int(r.group(1), 16)] = (r.group(3), int(r.group(2), 16))
    subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "--only-section=.rodata", str(obj), str(raw)], check=True)
    data = raw.read_bytes()
    for m in jtbl_cc.TABLE.finditer(text):
        base = marks.get(m.group(1))
        if base is None:
            continue
        for i, label in enumerate(re.findall(r"\$L\d+", m.group(2))):
            rel = relocs.get(base + 4 * i)
            if rel is None or rel[0] != ".text":  # a label relocates against the section; anything else: unknown
                continue
            syms.setdefault(label, int.from_bytes(data[base + 4 * i:base + 4 * i + 4], "little") + rel[1])
    return syms


def main() -> None:
    args = sys.argv[1:]
    unit = args[args.index("--unit") + 1]
    ps2as = args[args.index("--as") + 1]
    argv = [a for a in args[args.index("--") + 1:] if a != "-DASM_OFFSETS" and not a.startswith("-Wa,-Iobj/offsets")]
    out = Path(argv[argv.index("-o") + 1])
    src = next(a for a in argv if a.endswith((".cpp", ".c")))
    tmp = Path(tempfile.mkdtemp(prefix="ps2eeas_"))
    try:
        s_path = tmp / "unit.s"
        cmd = list(argv)
        cmd[cmd.index("-c")] = "-S"
        cmd[cmd.index("-o") + 1] = str(s_path)
        r = subprocess.run(cmd, capture_output=True, text=True)
        sys.stderr.write(r.stderr)
        if r.returncode:
            sys.exit(r.returncode)

        asm_base = [a for a in argv if a not in ("-xc++", "-xc")]

        def gas(text: str, extra: list, dest: Path = None) -> Path:
            dest = dest or tmp / "check.o"
            path = Path(str(dest) + ".s")
            path.write_text(text)
            c = list(asm_base)
            c[c.index(src)] = str(path)
            c[c.index("-o") + 1] = str(dest)
            c[c.index("-c"):c.index("-c") + 1] = ["-c", *extra]
            rr = subprocess.run(c, capture_output=True, text=True)
            if rr.returncode:
                sys.stderr.write(rr.stderr)
                sys.exit(rr.returncode)
            return dest

        text = s_path.read_text().replace("\r\n", "\n")
        new, _log = jtbl_cc.repoint(text, unit, gas, lambda t: ps2eeas_symbols(t, ps2as, tmp))
        final = tmp / "final.s"
        final.write_text(new, newline="\n")
        obj = tmp / "final.o"
        rr = subprocess.run(["wine", ps2as, "-G0", str(final), "-o", str(obj)], capture_output=True, text=True,
                            env=dict(os.environ, WINEDEBUG="-all"))
        if rr.returncode or not obj.exists():
            sys.stderr.write(rr.stdout + rr.stderr)
            sys.exit(rr.returncode or 1)
        shutil.copyfile(obj, out)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    main()
