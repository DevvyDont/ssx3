#!/usr/bin/env python3
import argparse
import bisect
import os
import shutil
import subprocess
import sys
import json
import re
from pathlib import Path
from typing import Dict, List, Set, Union

import ninja_syntax
import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry

ROOT = Path(__file__).parent.resolve()
TOOLS_DIR = ROOT / "tools"
OUTDIR = "out"

# Kept relative (not ROOT / ...) so splat resolves base_path relatively too;
# otherwise it bakes this machine's absolute path into generated files like
# include/macro.inc's ".include" directives.
YAML_FILE = Path("config") / "ssx3_us.yaml"
BASENAME = "SLUS_207.72"
LD_PATH = f"{BASENAME}.ld"
ELF_PATH = f"{OUTDIR}/{BASENAME}"
MAP_PATH = f"{OUTDIR}/{BASENAME}.map"
PRE_ELF_PATH = f"{OUTDIR}/{BASENAME}.elf"

# tools/macos/include supplies the few standard headers the sources need
# (<stdint.h>). Without it the compiler falls back to the host's /usr/include:
# missing on macOS, and glibc's headers on Linux, which it can't parse.
COMMON_INCLUDES = "-Iinclude -isystem include/sdk/ee -isystem include/gcc -isystem tools/macos/include"


# The compiler is SN V1.36 with its dormant `vec_mark` insn switched on (see tools/patch_vecmark.py):
# the retail code shows EA's compiler emitted it in every empty array-ctor loop. The patched copy is
# generated from the stock download on first configure.
STOCK_CC_DIR = f"{TOOLS_DIR}/cc/eegcc-2.95.3-V1.36"
CC_DIR = f"{TOOLS_DIR}/cc/eegcc-2.95.3-V1.36-vecmark"
if not os.path.isdir(CC_DIR) and os.path.isdir(STOCK_CC_DIR):
    subprocess.run([sys.executable, f"{TOOLS_DIR}/patch_vecmark.py", STOCK_CC_DIR, CC_DIR], check=True)
DRIVER_PATH_FLAG = f"-B{CC_DIR}/lib/gcc-lib/ee/2.95.3/"

# See tools/cc/README.md for how these were gathered
COMMON_CFLAGS = "-O2"
# Fork-only: per-unit extra compiler flags. EA's support library (ealib/) was built with -G0 (no $gp small data:
# globals and float constants are addressed with lui/%lo and lui/ori/mtc1), except one embedded module at
# 0x3D6498-0x3DBAE8 (ealib/seg_2D7498) that uses $gp and default flags. Evidence: notes/stuck.md "ealib".
UNIT_CFLAGS = {
    "ealib/seg_2B4578": "-G0",
    "ealib/seg_2DCAE8": "-G0",
}
# Fork-only: per-unit compiler. EA's embedded $gp module (ealib/seg_2D7498, 0x3D6498-0x3DBAE8) was built with an
# older SN build, ee-gcc 2.95.3-114 (its non-leaf functions save ra with sq; 107 and 114 reproduce all 48 landed
# functions plus the sq-ra prologues, 136 does not). Local copy from the decomp.me PS2 compiler bundle; if it is
# missing the unit falls back to the default compiler (and those functions just don't match).
UNIT_CC = {
    "ealib/seg_2D7498": f"{TOOLS_DIR}/cc/eegcc-2.95.3-114",
}
for _u, _d in list(UNIT_CC.items()):
    if not os.path.isfile(f"{_d}/bin/ee-gcc2953.exe"):
        print(f"warning: {_d} missing; {_u} builds with the default compiler")
        del UNIT_CC[_u]
# The game's objects have no exception frame info: without -fno-exceptions every C++ object gets
# 0x18 bytes of .data the original doesn't have
COMMON_CXXFLAGS = "-fno-exceptions"


# splat's generated INCLUDE_ASM macro emits `.include "FOLDER/NAME.s"` relative
# to the nonmatchings dir (no longer hardcodes "asm/nonmatchings/"), so the
# internal assembler needs that dir on its include search path.
ASM_INCLUDE_FLAG = "-Wa,-Iasm/nonmatchings"

COMPILE_C_RULE = f"{CC_DIR}/bin/ee-gcc2953.exe -c {COMMON_INCLUDES} {ASM_INCLUDE_FLAG} {DRIVER_PATH_FLAG} {COMMON_CFLAGS} $in"
COMPILE_CXX_RULE = f"{CC_DIR}/bin/ee-gcc2953.exe -xc++ -c {COMMON_INCLUDES} {ASM_INCLUDE_FLAG} {DRIVER_PATH_FLAG} {COMMON_CFLAGS} {COMMON_CXXFLAGS} $in"

CATEGORY_MAP = {
    "sce": "Libs",
    "data": "Data",
}

WINE = "wine"
if sys.platform == "linux" or sys.platform == "linux2":
    COMPILE_C_RULE = f"{WINE} {COMPILE_C_RULE}"
    COMPILE_CXX_RULE = f"{WINE} {COMPILE_CXX_RULE}"
elif sys.platform == "darwin":
    # macOS: run the Windows compiler with wibo (decompals/wibo, the
    # `wibo-macos` release asset; runs under Rosetta 2 on Apple Silicon)
    # instead of Wine. Uses $WIBO, else tools/wibo/wibo, else `wibo` on PATH.
    _wibo_local = TOOLS_DIR / "wibo" / "wibo"
    WIBO = os.environ.get("WIBO") or (str(_wibo_local) if _wibo_local.exists() else "wibo")
    COMPILE_C_RULE = f"{WIBO} {COMPILE_C_RULE}"
    COMPILE_CXX_RULE = f"{WIBO} {COMPILE_CXX_RULE}"

# C is compiled to asm first, so tools/jt_sections.py can give each jump table its own section (see
# link_c_jump_tables), then assembled by the same driver: the same as.exe a plain -c would run.
ASSEMBLE_RULE = COMPILE_C_RULE.split(" -c ")[0] + f" -c -x assembler {ASM_INCLUDE_FLAG} {DRIVER_PATH_FLAG}"


def compile_command(rule):
    return (f"{rule.replace(' -c ', ' -S ')} $cflags -o $out.s && python3 tools/jt_sections.py $out.s && "
            f"{ASSEMBLE_RULE} $cflags $out.s -o $out && rm $out.s")

def clean():
    files_to_clean = [
        ".splache",
        ".ninja_log",
        "build.ninja",
        "permuter_settings.toml",
        LD_PATH
    ]
    for filename in files_to_clean:
        if os.path.exists(filename):
            os.remove(filename)

    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("obj", ignore_errors=True)
    shutil.rmtree("out", ignore_errors=True)


def write_permuter_settings():
    with open("permuter_settings.toml", "w", encoding="utf-8") as f:
        f.write(f"""compiler_command = "{COMPILE_CXX_RULE} -D__GNUC__"
assembler_command = "mips-linux-gnu-as -march=r5900 -mabi=eabi -Iinclude"
compiler_type = "gcc"

[preserve_macros]

[decompme.compilers]
"tools/cc/eegcc-2.95.3-V1.36-vecmark/bin/gcc" = "eegcc-2.95.3-V1.36"
""")


GP_VALUE = 0x004A30F0


def link_unit_lit4(ld_text):
    """
    Link each C/C++ unit's .lit4 chunk (scripts/split_data.py) from the unit's object.

    Compiled C emits its own float constants (`li.s`), one per load, in instruction order, and so does
    the unit's asm after inline_lit4_loads, so the object's .lit4 is the original chunk. Returns the
    updated linker script.
    """
    yaml_text = YAML_FILE.read_text()
    subs = re.findall(r"- \[0x([0-9a-fA-F]+), (\w+)(?:, ([^\]#]+?))?\]", yaml_text)
    code_units = {n.strip(): t for _, t, n in subs if t in ("cpp", "c") and n}
    for _, t, name in subs:
        name = (name or "").strip()
        if t != "lit4" or name not in code_units:
            continue
        obj = f"obj/src/{name}.{code_units[name]}.o(.lit4);"
        chunk_line = f"        obj/asm/data/{name}.lit4.s.o(.lit4);\n"
        assert chunk_line in ld_text, chunk_line
        ld_text = ld_text.replace(chunk_line, f"        {obj}\n")
    return ld_text


ROM_TO_VRAM = 0x100000 - 0x1000
ADDR_LINE = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8})(?: [0-9A-F]{8})? \*/")
JTBL = re.compile(r"^dlabel (jtbl_([0-9A-F]{8}))\n(.*?)^enddlabel \1$", re.M | re.S)
JTBL_WORD = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/ \.word (?:\.L|lbl_|0x)([0-9A-F]{8})")


def c_jump_tables(rodata_path):
    """
    The jump tables of C functions: the tables no asm refers to any more. Returns {table address:
    (end address, unit name, k)}, where the table is the unit's k-th (tools/jt_sections.py) and the end
    is past its last entry (a table can be followed by padding, which stays in the asm).
    """
    yaml_text = YAML_FILE.read_text()
    units = [(int(a, 16) + ROM_TO_VRAM, t, (n or "").strip())
             for a, t, n in re.findall(r"- \[0x([0-9a-fA-F]+), (\w+)(?:, ([^\]#]+?))?\]", yaml_text)
             if t in ("c", "cpp", "asm", "hasm")]
    # The asm the build uses: the C units' INCLUDE_ASM functions (asm/nonmatchings can hold stale files
    # of functions since decompiled) and the asm units
    used = [f for f in Path("asm").rglob("*.s") if f != rodata_path and "nonmatchings" not in f.parts]
    for _, kind, name in units:
        if kind in ("c", "cpp"):
            for d, fn in re.findall(r'^INCLUDE_ASM\("([^"]+)", (\w+)\);', Path(f"src/{name}.{kind}").read_text(), re.M):
                used.append(Path(f"asm/nonmatchings/{d}/{fn}.s"))
    referenced = set()
    for f in used:
        referenced.update(re.findall(r"jtbl_[0-9A-F]{8}", f.read_text()))
    starts = [u[0] for u in units]
    tables = {}
    for m in JTBL.finditer(rodata_path.read_text()):
        if m.group(1) in referenced:
            continue
        # Entries are code addresses; a 0 after them is padding
        words = [w for w in JTBL_WORD.findall(m.group(3)) if int(w[1], 16) >= 0x100000]
        assert words, m.group(1)
        _, kind, name = units[bisect.bisect_right(starts, int(words[0][1], 16)) - 1]
        assert kind in ("c", "cpp"), f"{m.group(1)} belongs to no asm and no C unit"
        tables[int(m.group(2), 16)] = (int(words[-1][0], 16) + 4, f"{name}.{kind}")
    per_unit = {}
    for addr in sorted(tables):
        end, unit = tables[addr]
        tables[addr] = (end, unit, per_unit.get(unit, 0))
        per_unit[unit] = per_unit.get(unit, 0) + 1
    return tables


def link_c_jump_tables(ld_text):
    """
    Link each C function's jump table from its object, at its retail address.

    The compiler puts a switch's table in .rodata (in its own section, .rodata.jt<k>, after
    tools/jt_sections.py), but the game's .rodata is one asm object. That object is cut into parts
    (.rodata.p<n>) around the C tables, with the tables' asm taken out, and the linker script puts each
    C table between its parts. The parts are linked unaligned, so in every part after the first each
    piece of data is placed by its offset from the part's start (.org) instead of by .align.
    """
    rodata_path = next(Path("asm/data").glob("*.rodata.s"))
    start = int(rodata_path.name.split(".")[0], 16) + ROM_TO_VRAM
    tables = c_jump_tables(rodata_path)
    if not tables:
        return ld_text
    obj_line = f"        obj/{rodata_path}.o(.rodata);\n"
    assert obj_line in ld_text, obj_line
    out, pending, links = [], [], [obj_line]
    part, part_start, skip_until = 0, start, None
    for line in rodata_path.read_text().split("\n"):
        m = ADDR_LINE.match(line)
        addr = int(m.group(1), 16) if m else None
        if skip_until is not None:
            if addr is not None and addr + 4 >= skip_until:
                skip_until = None
            continue
        label = re.match(r"(?:dlabel|enddlabel|nonmatching) (jtbl_([0-9A-F]{8}))$", line)
        if label and int(label.group(2), 16) in tables:
            if line.startswith("dlabel"):
                table = int(label.group(2), 16)
                end, unit, k = tables[table]
                out.append(f".org 0x{table - part_start:X}")
                out.extend(l for l in pending if not (part and l.startswith(".align")))
                pending = []
                part += 1
                out.append(f'.section .rodata.p{part}, "a"')
                links.append(f"        obj/src/{unit}.o(.rodata.jt{k});\n")
                links.append(f"        obj/{rodata_path}.o(.rodata.p{part});\n")
                part_start, skip_until = end, end
            continue
        if part == 0:
            out.append(line)
        elif addr is not None:
            out.append(f".org 0x{addr - part_start:X}")
            out.extend(l for l in pending if not l.startswith(".align"))
            out.append(line)
            pending = []
        elif re.match(r"(dlabel|nonmatching|\.align) ", line):
            pending.append(line)
        else:
            out.append(line)
    out.extend(l for l in pending if not l.startswith(".align"))
    rodata_path.write_text("\n".join(out))
    print(f"rodata: {len(tables)} jump tables linked from C")
    return ld_text.replace(obj_line, "".join(links))


def patch_linker_script():
    """Link .lit4 from the C objects, and put the data in its own output section without padding."""
    ld_text = Path(LD_PATH).read_text()
    ld_text = link_unit_lit4(ld_text)
    ld_text = link_c_jump_tables(ld_text)
    # Each data chunk is an exact byte range of the original, so the data objects are only 4-aligned
    # per object, while the code needs SUBALIGN(8)
    data_start = "        main_DATA_START = .;\n"
    assert data_start in ld_text
    ld_text = ld_text.replace(data_start, "    }\n"
                              "    .main_data : AT(main_ROM_START + SIZEOF(.main)) SUBALIGN(1)\n"
                              "    {\n" + data_start)
    rom_pos = "    __romPos += SIZEOF(.main);\n"
    assert rom_pos in ld_text
    ld_text = ld_text.replace(rom_pos, "    __romPos += SIZEOF(.main) + SIZEOF(.main_data);\n")
    Path(LD_PATH).write_text(ld_text)


BRANCH = re.compile(r"^(\s*/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/\s+)"
                    r"(b|beqz|bnez|beq|bne|beql|bnel|blez|bgtz|bltz|bgez|blezl|bgtzl|bltzl|bgezl|bc1f|bc1t|bc1fl|bc1tl)"
                    r"(\s+.*)$", re.M)


def freeze_asm_branches():
    """Write every branch in the split asm as a .word of its original encoding.

    The SN assembler the game was built with (ee/bin/as.exe, May 2001) pads short loops with nops, as a
    workaround for an R5900 erratum. The asm already contains the padding the original build added, but the
    assembler pads some of its loops again (depending on where they sit in the section), so those functions
    come out too long. It only pads what it sees as a loop branch, so with the branches written as data it
    leaves the asm alone, while compiled C still gets padded. Branch targets are local to each function, so
    the encodings are position independent.
    """
    frozen = 0
    for f in Path("asm").rglob("*.s"):
        text = f.read_text()
        if "/*" not in text:
            continue

        def sub(m):
            nonlocal frozen
            frozen += 1
            word = int.from_bytes(bytes.fromhex(m.group(2)), "little")
            return f"{m.group(1)}.word 0x{word:08X} /* {m.group(3)}{m.group(4).rstrip()} */"

        new = BRANCH.sub(sub, text)
        if new != text:
            f.write_text(new)
    print(f"Froze {frozen} asm branches")


def inline_lit4_loads():
    """Make the asm generate its own float constants, the way compiled C does.

    The compiler never names float literals: it emits `li.s` and the assembler puts the value in the
    object's .lit4 (R_MIPS_LITERAL), one entry per load, in instruction order. splat instead loads the
    retail pool entry by name (`lwc1 $fN, (D_x)`, gp-relative). Each such load becomes `li.s $fN, value`,
    which assembles to the same single lwc1, so each unit's .lit4 comes out of its object in the original
    order whether its functions are asm or C, and objdiff can compare the two. A value li.s can't express
    (zero low half, which gas expands to lui+mtc1, or not a normal float) is written as a .word in .lit4
    just before a load of it by its gp offset. Every .lit4 load in the game is a gp-relative lwc1, one per
    constant.
    """
    import struct

    values = {}
    for f in Path("asm/data").rglob("*.lit4.s"):
        for addr, word in re.findall(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/", f.read_text()):
            values[addr] = bytes.fromhex(word)
    load = re.compile(r"^(\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/\s+)lwc1(\s+)(\$f\d+), "
                      r"\((D_([0-9A-F]{8}))\) /\* gp_rel: \(D_[0-9A-F]{8}\) \*/", re.M)
    counts = [0, 0]
    for f in Path("asm/nonmatchings").rglob("*.s"):
        text = f.read_text()
        if "gp_rel" not in text:
            continue

        def sub(m):
            raw = values.get(m.group(5))
            if raw is None:
                return m.group(0)
            word = int.from_bytes(raw, "little")
            exponent = (word >> 23) & 0xFF
            if word & 0xFFFF and 0 < exponent < 0xFF:
                counts[0] += 1
                value = struct.unpack("<f", raw)[0]
                return f"{m.group(1)}li.s{m.group(2)}{m.group(3)}, {value!r} /* lit4: {m.group(4)} */"
            counts[1] += 1
            offset = int(m.group(5), 16) - GP_VALUE
            return (f".section .lit4\n    .word 0x{word:08X}\n.section .text\n"
                    f"{m.group(1)}lwc1{m.group(2)}{m.group(3)}, {offset}($28) /* lit4: {m.group(4)} */")

        new = load.sub(sub, text)
        if new != text:
            f.write_text(new)
    print(f"lit4: {counts[0]} asm loads made li.s, {counts[1]} written with their .word")


def build_stuff(linker_entries: List[LinkerEntry], skip_checksum=False, objects_only=False, dual_objects=False):
    """
    Build the objects and the final ELF file.
    If objects_only is True, only build objects and skip linking/checksum.
    If dual_objects is True, build objects twice: once normally, once with -DSKIP_ASM.
    """
    built_objects: Set[Path] = set()
    objdiff_units = []  # For objdiff.json

    def build(
        object_paths: Union[Path, List[Path]],
        src_paths: List[Path],
        task: str,
        variables: Dict[str, str] = None,
        implicit_outputs: List[str] = None,
        out_dir: str = None,
        extra_flags: str = "",
        collect_objdiff: bool = False,
        orig_entry=None,
        implicit: List[str] = None,
    ):
        """
        Helper function to build objects.
        """
        # Handle none parameters
        if variables is None:
            variables = {}

        if implicit_outputs is None:
            implicit_outputs = []

        # Convert object_paths to list if it is not already
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        # Only rewrite output path to .o if out_dir is set (i.e. --objects mode)
        if out_dir:
            new_object_paths = []
            for obj in object_paths:
                obj = Path(obj)
                stem = obj.stem
                if obj.suffix in [".s", ".c" , ".cpp"]:
                    stem = obj.stem
                else:
                    if obj.suffix == ".o" and obj.with_suffix("").suffix in [".cpp",".s", ".c"]:
                        stem = obj.with_suffix("").stem
                target_dir = out_dir if out_dir else obj.parent
                new_obj = Path(target_dir) / (stem + ".o")
                new_object_paths.append(new_obj)
            object_paths = new_object_paths

        # Otherwise, use the original object_paths (with .s.o, .c.o, etc.)

        # Convert paths to strings
        object_strs = [str(obj) for obj in object_paths]

        # Add object paths to built_objects
        for idx, object_path in enumerate(object_paths):
            if object_path.suffix == ".o":
                built_objects.add(object_path)
            # Add extra_flags to variables if present
            build_vars = variables.copy()
            if extra_flags:
                build_vars["cflags"] = extra_flags
            ninja.build(
                outputs=[str(object_path)],
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=build_vars,
                implicit=implicit or [],
                implicit_outputs=implicit_outputs,
            )
            # Collect for objdiff.json if requested
            if collect_objdiff and orig_entry is not None:
                src = src_paths[0] if src_paths else None
                if src:
                    src = Path(src)
                    # Always use the final "matched" name, i.e. as if it will be in src/ with no asm/ prefix
                    try:
                        # If the file is in asm/, replace asm/ with nothing (just drop asm/)
                        if src.parts[0] == "asm":
                            rel = Path(*src.parts[1:])
                        elif src.parts[0] == "src":
                            rel = Path(*src.parts[1:])
                        else:
                            rel = src
                        # Remove extension for the name
                        name = str(rel.with_suffix(""))
                    except Exception:
                        name = str(src.with_suffix(""))
                else:
                    name = object_path.stem
                if "target" in str(object_path):
                    target_path = str(object_path)
                    # Determine if a .c or .cpp file exists in src/ for this unit (recursively)
                    src_base = rel.with_suffix("")
                    src_c_files = list(Path("src").rglob(src_base.name + ".c"))
                    src_cpp_files = list(Path("src").rglob(src_base.name + ".cpp"))
                    has_src = bool(src_c_files or src_cpp_files)
                    # Determine the category based on the name
                    categories = [name.split("/")[0]]
                    unit = {
                        "name": name,
                        "target_path": target_path,
                        "metadata": {
                            "progress_categories": categories,
                        }
                    }
                    if has_src:
                        base_path = str(object_path).replace("target", "current")
                        unit["base_path"] = base_path
                    objdiff_units.append(unit)

    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), "w", encoding="utf-8"), width=9999)

    cross = "mips-linux-gnu-"

    ld_args = "-EL -T config/undefined_syms_auto.txt -T config/undefined_funcs_auto.txt -T obj/address_syms.ld -Map $mapfile -T $in -o $out"

    as_cpp = "cpp"
    as_filter = ""
    if sys.platform == "darwin":
        # Apple's /usr/bin/cpp mis-parses `-isystem DIR`; use clang directly.
        # Also normalise $t0-$t7 to numeric names (see tools/macos/o32_gpr_names.pl).
        as_cpp = "clang -E -x assembler-with-cpp"
        as_filter = "perl -p tools/macos/o32_gpr_names.pl | "

    ninja.rule(
        "as",
        description="as $in",
        command=f"{as_cpp} {COMMON_INCLUDES} $in -o  - | {as_filter}{cross}as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o $out",
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=f"{compile_command(COMPILE_C_RULE)} && {cross}strip $out -N dummy-symbol-name",
    )

    ninja.rule(
        "cc_unit",
        description="cc $in ($ccdir)",
        command=f"{compile_command(COMPILE_C_RULE).replace(CC_DIR, '$ccdir')} && {cross}strip $out -N dummy-symbol-name",
    )

    ninja.rule(
        "cpp",
        description="cpp $in",
        command=f"{compile_command(COMPILE_CXX_RULE)} && {cross}strip $out -N dummy-symbol-name",
    )

    ninja.rule(
        "address_syms",
        description="address_syms $out",
        command="python3 tools/address_syms.py $out $in",
    )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{cross}ld {ld_args}",
    )

    ninja.rule(
        "asmoff",
        description="asmoff $out",
        command="python3 tools/asm_offsets.py $src $in $out",
    )

    ninja.rule(
        "sha1sum",
        description="sha1sum $in",
        command="sha1sum -c $in && touch $out",
    )

    ninja.rule(
        "elf",
        description="elf $out",
        command=f"{cross}objcopy $in $out -O binary",
    )

    def current_c_flags(entry):
        """-DSKIP_ASM plus the target offsets of the unit's INCLUDE_ASM functions (tools/asm_offsets.py), so
        every C function in obj/current sits at its obj/target offset: SN's assembler pads short loops
        depending on position, so a matched function only reproduces the target bytes there."""
        obj = Path(entry.object_path)
        stem = obj.with_suffix("").stem if obj.suffix == ".o" else obj.stem
        src = str(entry.src_paths[0])
        inc = f"obj/offsets/{stem}/asm_offsets.inc"
        ninja.build(outputs=[inc], rule="asmoff", inputs=[f"obj/target/{stem}.o"], implicit=[src],
                    variables={"src": src})
        return f"-DSKIP_ASM -DASM_OFFSETS -Wa,-Iobj/offsets/{stem}", [inc]

    # Add recipes for everything
    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        # Before CommonSegC: CommonSegCpp is a subclass of it
        elif isinstance(seg, splat.segtypes.common.cpp.CommonSegCpp):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "cpp", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                flags, inc = current_c_flags(entry)
                build(entry.object_path, entry.src_paths, "cpp", out_dir="obj/current", extra_flags=flags, implicit=inc)
            else:
                build(entry.object_path, entry.src_paths, "cpp")
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            unit_flags = UNIT_CFLAGS.get(seg.name, "")
            task, tvars = ("cc_unit", {"ccdir": UNIT_CC[seg.name]}) if seg.name in UNIT_CC else ("cc", None)
            if dual_objects:
                build(entry.object_path, entry.src_paths, task, out_dir="obj/target", collect_objdiff=True, orig_entry=entry,
                      extra_flags=unit_flags or None, variables=tvars)
                flags, inc = current_c_flags(entry)
                build(entry.object_path, entry.src_paths, task, out_dir="obj/current",
                      extra_flags=(flags + " " + unit_flags).strip(), implicit=inc, variables=tvars)
            else:
                build(entry.object_path, entry.src_paths, task, extra_flags=unit_flags or None, variables=tvars)
        elif isinstance(seg, splat.segtypes.common.databin.CommonSegDatabin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.rodatabin.CommonSegRodatabin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.textbin.CommonSegTextbin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.bin.CommonSegBin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    if objects_only:
        # Write objdiff.json if dual_objects (i.e. --objects)
        if dual_objects:
            objdiff = {
                "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
                "custom_make": "ninja",
                "custom_args": [],
                "build_target": False,
                "build_base": True,
                "watch_patterns": [
                    "src/**/*.c",
                    "src/**/*.cp",
                    "src/**/*.cpp",
                    "src/**/*.cxx",
                    "src/**/*.h",
                    "src/**/*.hp",
                    "src/**/*.hpp",
                    "src/**/*.hxx",
                    "src/**/*.s",
                    "src/**/*.S",
                    "src/**/*.asm",
                    "src/**/*.inc",
                    "src/**/*.py",
                    "src/**/*.yml",
                    "src/**/*.txt",
                    "src/**/*.json"
                ],
                "units": objdiff_units,
                "progress_categories": [ {"id": id, "name": name} for id, name in CATEGORY_MAP.items() ],
            }
            with open("objdiff.json", "w", encoding="utf-8") as f:
                json.dump(objdiff, f, indent=2)
        return

    ninja.build(
        "obj/address_syms.ld",
        "address_syms",
        LD_PATH,
        implicit=sorted(str(obj) for obj in built_objects),
    )

    ninja.build(
        PRE_ELF_PATH,
        "ld",
        LD_PATH,
        implicit=[str(obj) for obj in built_objects] + ["obj/address_syms.ld"],
        variables={"mapfile": MAP_PATH},
    )

    ninja.build(
        ELF_PATH,
        "elf",
        PRE_ELF_PATH,
    )

    # FIXME: Do this in a more elegant way;
    # probably a python script which reads the splat yaml to get the SHA1,
    # rather than needing a seperate sha1 file.
    # My obession with DRY will never stop >:3
    if not skip_checksum:
        ninja.build(
            ELF_PATH + ".ok",
            "sha1sum",
            "config/checksum.sha1",
            implicit=[ELF_PATH],
        )
    else:
        print("Not adding sha1 rule")

def main():
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean artifacts and build",
        action="store_true",
    )
    parser.add_argument(
        "-C",
        "--only-clean",
        help="Only clean artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-s",
        "--skip-checksum",
        help="Skip the checksum step",
        action="store_true",
    )
    parser.add_argument(
        "-o",
        "--objects",
        help="Build objects to obj/target and obj/current (with -DSKIP_ASM), skip linking and checksum",
        action="store_true",
    )
    args = parser.parse_args()

    do_clean = (args.clean or args.only_clean) or False

    do_skip_checksum = args.skip_checksum
    do_objects = args.objects or False

    if do_clean:
        clean()
        if args.only_clean:
            return

    # No cache: segments loaded from the cache aren't scanned, so the symbols
    # they reference would be left out of undefined_syms_auto.txt
    split.main([YAML_FILE], modes="all", verbose=False, use_cache=False)

    linker_entries = split.linker_writer.entries
    inline_lit4_loads()
    patch_linker_script()
    freeze_asm_branches()

    if do_objects:
        build_stuff(linker_entries, skip_checksum=True, objects_only=True, dual_objects=True)
    else:
        build_stuff(linker_entries, do_skip_checksum)

    write_permuter_settings()

if __name__ == "__main__":
    main()