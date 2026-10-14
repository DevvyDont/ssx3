#!/usr/bin/env python3
"""Rename function symbols in config/symbol_addrs.txt and in src/ INCLUDE_ASM lines.

    python3 scripts/rename_symbol.py OLD=NEW [OLD=NEW ...]

A symbol not yet in symbol_addrs.txt (a splat func_XXXXXXXX name) is added,
with the address taken from its name. Re-run ./configure.py afterwards so
splat writes the asm files under the new names.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent
SYMBOLS = ROOT / "config" / "symbol_addrs.txt"


def main():
    pairs = [arg.split("=", 1) for arg in sys.argv[1:]]
    if not pairs or any(len(p) != 2 for p in pairs):
        print(__doc__)
        sys.exit(1)

    text = SYMBOLS.read_text()
    for old, new in pairs:
        line = re.compile(rf"^{re.escape(old)} = (0x[0-9A-Fa-f]+);", re.M)
        if line.search(text):
            text = line.sub(rf"{new} = \1;", text)
        else:
            m = re.match(r"func_([0-9A-Fa-f]{8})", old)
            if not m:
                sys.exit(f"{old} isn't in symbol_addrs.txt and has no address in its name")
            text = text.rstrip("\n") + f"\n{new} = 0x{m.group(1).upper()}; //type:func\n"
    SYMBOLS.write_text(text)

    for path in (ROOT / "src").rglob("*.cpp"):
        src = path.read_text()
        out = src
        for old, new in pairs:
            out = re.sub(rf'(INCLUDE_ASM\("[^"]+", ){re.escape(old)}\)', rf"\g<1>{new})", out)
        if out != src:
            path.write_text(out)
            print(f"updated {path.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
