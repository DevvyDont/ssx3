#!/usr/bin/env python3
"""Link the C of functions objdiff reports as matching, instead of their asm.

    python3 scripts/unwrap_matched.py REPORT.json [UNIT ...]

REPORT.json is `objdiff-cli report generate -f json` output from a dual build
(./configure.py -c -o && ninja). For every function at 100% in it, the
INCLUDE_ASM line above its C is removed, along with the #ifdef SKIP_ASM /
#endif around the C, so the linked build uses the C. With UNIT arguments
(e.g. ai/computer) only those units are changed.

Functions the original has at an address that isn't 8-aligned, or followed
by one, are left wrapped: the compiler aligns every function to 8 bytes, so
their C can't sit where they do (hand-written asm such as the VU0 routines).
Run ./configure.py first, so their asm is in asm/nonmatchings.

objdiff's 100% is not proof: rebuild with ./configure.py && ninja and check
the checksum afterwards.
"""
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent
ADDR = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/")
INCLUDE = re.compile(r'^INCLUDE_ASM\("([^"]+)", ([^)]+)\);\n#ifdef SKIP_ASM\n', re.M)


def matched_functions(report_path):
    report = json.loads(Path(report_path).read_text())
    matched = {}
    for unit in report["units"]:
        for func in unit.get("functions", []):
            if func.get("fuzzy_match_percent", 0) >= 100:
                matched.setdefault(unit["name"], set()).add(func["name"])
    return matched


def aligned(unit, name):
    """Whether the function starts and ends 8-aligned in the original."""
    asm = ROOT / f"asm/nonmatchings/{unit}/{name}.s"
    addrs = [int(a, 16) for a in ADDR.findall(asm.read_text())] if asm.exists() else []
    return bool(addrs) and addrs[0] % 8 == 0 and (max(addrs) + 4) % 8 == 0


def unwrap(text, names):
    """Unwrap the INCLUDE_ASM blocks of the given functions; returns (text, count)."""
    out = []
    pos = 0
    count = 0
    for m in INCLUDE.finditer(text):
        if m.start() < pos or m.group(2) not in names:
            continue
        # Find the #endif closing this #ifdef SKIP_ASM, allowing nested #if blocks
        depth = 1
        i = m.end()
        while depth:
            line_end = text.index("\n", i)
            line = text[i:line_end].strip()
            if line.startswith("#if"):
                depth += 1
            elif line.startswith("#endif"):
                depth -= 1
                if not depth:
                    break
            i = line_end + 1
        out.append(text[pos:m.start()])
        out.append(text[m.end():i])
        pos = line_end + 1
        count += 1
    out.append(text[pos:])
    return "".join(out), count


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)
    matched = matched_functions(sys.argv[1])
    units = sys.argv[2:] or sorted(matched)
    total = 0
    for unit in units:
        src = next((p for p in (ROOT / f"src/{unit}.cpp", ROOT / f"src/{unit}.c") if p.exists()), None)
        if src is None or unit not in matched:
            continue
        text = src.read_text()
        wrapped = {m.group(2) for m in INCLUDE.finditer(text)} & matched[unit]
        names = {n for n in wrapped if aligned(unit, n)}
        for n in sorted(wrapped - names):
            print(f"{unit}: {n} not 8-aligned in the original, left wrapped")
        text, count = unwrap(text, names)
        if count:
            src.write_text(text)
            total += count
    print(f"unwrapped {total} functions")


if __name__ == "__main__":
    main()
