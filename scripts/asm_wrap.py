#!/usr/bin/env python3
"""Move functions in a source file between plain C and INCLUDE_ASM wrappers.

    python3 scripts/asm_wrap.py wrap src/main/sectionman.cpp
    python3 scripts/asm_wrap.py unwrap src/main/sectionman.cpp
    python3 scripts/asm_wrap.py sync src/main/sectionman.cpp

`wrap` puts every plain C function definition in the file under
INCLUDE_ASM + #ifdef SKIP_ASM, so objdiff's target object is built from the
original assembly while the C is worked on. The INCLUDE_ASM names are the
symbols the C compiles to, taken from a test compile.

`sync` is for C written under an existing INCLUDE_ASM line (usually a
splat func_XXXXXXXX name): each wrapped function whose INCLUDE_ASM name
differs from the symbol its C compiles to is renamed to the compiled name,
in config/symbol_addrs.txt and in src/ (scripts/rename_symbol.py). Run
./configure.py afterwards.

`unwrap` removes the wrapper from every function whose compiled C matches the
original assembly instruction for instruction (scripts/diff_func.sh), so the
C becomes what gets linked. It needs an up-to-date dual build
(`./configure.py -o && ninja`). Afterwards run `./configure.py` and `ninja` to
confirm the checksum.
"""
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).parent.parent.resolve()
CC_DIR = ROOT / "tools" / "cc" / "eegcc-2.95.3-V1.36"

# A top-level function definition: an unindented signature line, then "{",
# up to the next "}" in column 0
FUNC = re.compile(r"^(?!#|extern|//|\s|typedef|struct|class)([^\n;]*\)[^\n;]*)\n\{\n.*?^\}\n", re.S | re.M)
WRAPPED = re.compile(r'INCLUDE_ASM\("([^"]+)", ([^)]+)\);\n#ifdef SKIP_ASM\n(.*?)#endif\n', re.S)


def compiled_symbols(src_text):
    """Function symbols, in order, that the file compiles to with SKIP_ASM."""
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "x.cpp"
        obj = Path(tmp) / "x.o"
        src.write_text(src_text)
        cmd = ["wine", str(CC_DIR / "bin/ee-gcc2953.exe"), "-xc++", "-c", "-Iinclude",
               f"-B{CC_DIR}/lib/gcc-lib/ee/2.95.3/", "-O2", "-fno-exceptions", "-DSKIP_ASM",
               str(src), "-o", str(obj)]
        subprocess.run(cmd, cwd=ROOT, check=True, env=dict(os.environ, WINEDEBUG="-all"))
        out = subprocess.run(["mips-linux-gnu-objdump", "-t", str(obj)],
                             capture_output=True, text=True, check=True).stdout
    syms = []
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 6 and parts[2] == "F" and parts[3] == ".text":
            syms.append((int(parts[0], 16), parts[5]))
    return [name for _, name in sorted(syms)]


def wrap(path):
    text = path.read_text()
    unit = path.relative_to(ROOT / "src").with_suffix("").as_posix()
    # Wrapped functions are skipped: blank them out so FUNC can't see them
    masked = WRAPPED.sub(lambda m: " " * len(m.group(0)), text)
    plain = list(FUNC.finditer(masked))
    if not plain:
        print("nothing to wrap")
        return
    # Symbols for the plain functions, in file order
    names = compiled_symbols(text)
    already = {m.group(2) for m in WRAPPED.finditer(text)}
    included = set(re.findall(r'INCLUDE_ASM\("[^"]+", ([^)]+)\);', text))
    names = [n for n in names if n not in already and n not in included]
    if len(names) != len(plain):
        sys.exit(f"found {len(plain)} function definitions but {len(names)} new symbols")
    for m, name in reversed(list(zip(plain, names))):
        body = text[m.start():m.end()]
        text = text[:m.start()] + f'INCLUDE_ASM("{unit}", {name});\n#ifdef SKIP_ASM\n{body}#endif\n' + text[m.end():]
        print(f"wrapped {name}")
    path.write_text(text)


def sync(path):
    text = path.read_text()
    masked = WRAPPED.sub(lambda m: "\x00" * len(m.group(0)), text)
    # Every slot with C, in file order: wrapped blocks and plain definitions
    slots = sorted([(m.start(), m.group(2)) for m in WRAPPED.finditer(text)] +
                   [(m.start(), None) for m in FUNC.finditer(masked)])
    names = compiled_symbols(text)
    names = [n for n in names if n not in set(re.findall(r'INCLUDE_ASM\("[^"]+", ([^)]+)\);', text))
             or n in {m.group(2) for m in WRAPPED.finditer(text)}]
    if len(names) != len(slots):
        sys.exit(f"found {len(slots)} functions with C but {len(names)} compiled symbols")
    renames = [f"{old}={new}" for (_, old), new in zip(slots, names) if old and old != new]
    if not renames:
        print("nothing to rename")
        return
    subprocess.run([sys.executable, str(ROOT / "scripts" / "rename_symbol.py")] + renames, check=True)


def unwrap(path):
    text = path.read_text()

    def keep_or_unwrap(m):
        unit, name, body = m.group(1), m.group(2), m.group(3)
        diff = subprocess.run(["sh", "scripts/diff_func.sh", unit, name], cwd=ROOT,
                              capture_output=True, text=True).stdout
        if not diff.strip():
            print(f"{name}: no object to compare, left wrapped")
            return m.group(0)
        n = sum(1 for line in diff.splitlines()
                if re.search(r"[^\t]\s+[|<>](\s|$)", line) and not re.match(r"^nop\s+<$", line))
        if n:
            print(f"{name}: {n} differing lines, left wrapped")
            return m.group(0)
        print(f"{name}: matches, unwrapped")
        return body

    path.write_text(WRAPPED.sub(keep_or_unwrap, text))


def main():
    if len(sys.argv) != 3 or sys.argv[1] not in ("wrap", "unwrap", "sync"):
        print(__doc__)
        sys.exit(1)
    path = Path(sys.argv[2]).resolve()
    {"wrap": wrap, "unwrap": unwrap, "sync": sync}[sys.argv[1]](path)


if __name__ == "__main__":
    main()
