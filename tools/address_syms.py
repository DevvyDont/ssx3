#!/usr/bin/env python3
"""Define the address-named symbols the objects use but nothing defines.

    tools/address_syms.py OUT.ld OBJECT...

Decompiled C refers to globals by their splat names (D_004FF160, D_828,
func_0014A0B0, possibly mangled as func_0014A0B0__Fi). When no asm still
references such a symbol, splat doesn't list it in undefined_syms_auto.txt,
so this writes a PROVIDE() at the address in its name for each one left
undefined. The output is passed to the linker with -T.
"""
import re
import subprocess
import sys

NAME = re.compile(r"^(?:D|func|lbl|jtbl)_([0-9A-F]{1,8})(?:__\w*)?$")


def main():
    out, objects = sys.argv[1], sys.argv[2:]
    defined, undefined = set(), set()
    for i in range(0, len(objects), 200):
        nm = subprocess.run(["mips-linux-gnu-nm", *objects[i:i + 200]], capture_output=True, text=True, check=True)
        for line in nm.stdout.splitlines():
            parts = line.split()
            if len(parts) == 2 and parts[0] == "U":
                undefined.add(parts[1])
            elif len(parts) == 3:
                defined.add(parts[2])
    lines = []
    for name in sorted(undefined - defined):
        m = NAME.match(name)
        if m:
            lines.append(f"PROVIDE({name} = 0x{m.group(1)});\n")
    with open(out, "w") as f:
        f.writelines(lines)


if __name__ == "__main__":
    main()
