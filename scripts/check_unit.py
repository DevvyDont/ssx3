#!/usr/bin/env python3
"""Rebuild and print objdiff's per-function match for one or more units.

Needs a dual-object build configured first: `./configure.py -o`.

    python3 scripts/check_unit.py bxrandom hashvalue
"""
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent.resolve()
OBJDIFF_CLI = ROOT / "tools" / "objdiff" / "objdiff-cli"


def main():
    units = sys.argv[1:]
    if not units:
        print(__doc__)
        sys.exit(1)

    build = subprocess.run(["ninja"], cwd=ROOT, capture_output=True, text=True)
    if build.returncode != 0:
        print(build.stdout[-4000:])
        sys.exit(1)

    out = ROOT / "objdiff_report.tmp.json"
    subprocess.run(
        [str(OBJDIFF_CLI), "report", "generate", "-p", str(ROOT), "-o", str(out), "-f", "json"],
        cwd=ROOT, check=True, capture_output=True,
    )
    report = json.loads(out.read_text())
    out.unlink()

    for unit in report["units"]:
        if unit["name"] not in units:
            continue
        print(f"== {unit['name']}")
        for fn in unit.get("functions", []):
            pct = fn.get("fuzzy_match_percent", 0.0)
            name = fn.get("metadata", {}).get("demangled_name") or fn["name"]
            mark = "OK " if pct == 100.0 else "   "
            print(f"  {mark}{pct:7.2f}%  {int(fn.get('size', 0)):5d}  {name}")


if __name__ == "__main__":
    main()
