#!/usr/bin/env python3
"""Split .lit4 into one chunk per text unit.

    python3 scripts/split_data.py

Every object contributes its float constants (.lit4) in the same link order
as its code, and each constant is loaded by exactly one instruction. So the
.lit4 entries in config/ssx3_us.yaml can be replaced by one chunk per unit
that loads constants, starting at its first constant. A chunk is named after
its unit when that unit is C/C++, and configure.py then links the unit's
.lit4 from its object (link_unit_lit4 there), where the compiler and the asm
(inline_lit4_loads) generate the constants.

Only .lit4 is split: the decompiled C refers to everything else in .data,
.rodata and .sdata by symbol, so those stay whole asm objects.

It runs splat itself, with every function disassembled (decompiled ones
included), and leaves asm/ in that state: run ./configure.py -c afterwards.
"""
import bisect
import re
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent
YAML = ROOT / "config/ssx3_us.yaml"
ROM_TO_VRAM = 0x100000 - 0x1000
GP = 0x004A30F0
# The ELF's .lit4, and the padding after it up to .sdata (in no object)
LIT4_START, LIT4_END, SDATA_START = 0x0049B100, 0x004A0E58, 0x004A0E80
ENTRY = re.compile(r"^(\s*)- \[0x([0-9a-fA-F]+), (\w+)(?:, ([^\]]+?))?\](.*)$", re.M)


def run_splat():
    sys.path.insert(0, str(ROOT))
    import splat.scripts.split as split
    split.main([YAML], modes="all", verbose=False, use_cache=False, disassemble_all=True)


def text_units(yaml_text):
    entries = [(int(a, 16) + ROM_TO_VRAM, t, (n or "").strip()) for _, a, t, n, _ in ENTRY.findall(yaml_text)]
    return [(a, t, n or f"{a:X}") for a, t, n in entries if t in ("cpp", "c", "asm", "hasm")]


def lit4_loads(units):
    """{constant address: index of the unit loading it}"""
    starts = [u[0] for u in units]
    loads = {}
    line = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+\S+\s+(.*)")
    for root in ("asm/nonmatchings", "asm/matchings", "asm"):
        files = (ROOT / root).rglob("*.s") if root != "asm" else (ROOT / root).glob("*.s")
        for f in files:
            for m in line.finditer(f.read_text(errors="replace")):
                pc, ops = int(m.group(1), 16), m.group(2)
                targets = [int(x, 16) for x in re.findall(r"\(D_([0-9A-F]{8})\) /\* gp_rel:", ops)]
                m2 = re.search(r"(-?0x[0-9A-Fa-f]+|-?\d+)\(\$(?:28|gp)\)", ops)
                if m2:
                    targets.append(GP + int(m2.group(1), 0))
                for t in targets:
                    if LIT4_START <= t < LIT4_END:
                        unit = bisect.bisect_right(starts, pc) - 1
                        if loads.setdefault(t, unit) != unit:
                            sys.exit(f"constant {t:08X} is loaded by two units")
    return loads


def main():
    run_splat()
    yaml_text = YAML.read_text()
    units = text_units(yaml_text)
    loads = lit4_loads(units)
    addrs = sorted(loads)
    order = [loads[a] for a in addrs]
    if any(x > y for x, y in zip(order, order[1:])):
        sys.exit("the units' constants are not in link order")
    chunks = []
    for a in addrs:
        if not chunks or chunks[-1][1] != loads[a]:
            chunks.append((a, loads[a]))
    chunks[0] = (LIT4_START, chunks[0][1])
    chunks.append((LIT4_END, None))

    lines = yaml_text.split("\n")
    idx = [i for i, l in enumerate(lines)
           if (m := ENTRY.match(l)) and m.group(3) == "lit4"
           and LIT4_START <= int(m.group(2), 16) + ROM_TO_VRAM < SDATA_START]
    indent = ENTRY.match(lines[idx[0]]).group(1)
    new = []
    for a, u in chunks:
        named = u is not None and units[u][1] in ("cpp", "c")
        new.append(f"{indent}- [0x{a - ROM_TO_VRAM:x}, lit4" + (f", {units[u][2]}]" if named else "]"))
    lines[idx[0]:idx[-1] + 1] = new
    YAML.write_text("\n".join(lines))
    print(f"{len(addrs)} constants in {len(chunks)} .lit4 chunks; wrote {YAML.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
