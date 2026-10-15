#!/usr/bin/env python3
"""Put each jump table the compiler emits into its own section, .rodata.jt<k>.

    python3 tools/jt_sections.py file.s

gcc emits a switch's jump table into .rdata right after the function's dispatch (`.rdata`, `.align`,
the table label, its `.word $Lnn` entries, `.text`). The game's .rodata is linked from asm, with every
C unit's jump tables cut out of it, so each table has to land at its own retail address
(configure.py link_c_jump_tables). k counts the unit's tables in the order the compiler emits them,
which is the order of their retail addresses. Rewrites the file in place.
"""
import re
import sys

TABLE = re.compile(r"^\t\.rdata\n(\t\.align\t\d+\n\$L\d+:\n(?:\t\.word\t\$L\d+\n)+)", re.M)


def main():
    path = sys.argv[1]
    text = open(path).read()
    count = 0

    def sub(m):
        nonlocal count
        line = f'\t.section .rodata.jt{count},"a"\n'
        count += 1
        return line + m.group(1)

    text = TABLE.sub(sub, text)
    open(path, "w").write(text)


if __name__ == "__main__":
    main()
