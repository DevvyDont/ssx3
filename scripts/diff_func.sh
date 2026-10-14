#!/bin/sh
# Show the instruction diff for one function between obj/target and obj/current.
# Run `python3 scripts/check_unit.py <unit>` (or ninja) first to rebuild.
#
#   scripts/diff_func.sh bxrandom cBxPseudoRng_Seed__FPUiUi [other.o]
set -e
unit=$1
sym=$2
cd "$(dirname "$0")/.."

dump() {
	mips-linux-gnu-objdump -drz -m mips:5900 --no-show-raw-insn "$1" |
		awk -v s="<$sym>:" '$2 == s { p = 1; next } p && /^[0-9a-f]+ </ { p = 0 } p' |
		sed -E 's/^ +[0-9a-f]+:\t//; s/[0-9a-f]+ <[^>]*>/<addr>/; s/^\t+[0-9a-f]+: (R_MIPS_GPREL16)\t.*/\t\t\1/; s/^\t+[0-9a-f]+: (R_)/\t\t\1/; s/-?[0-9]+\(gp\)/<gp>(gp)/' |
		grep -v "R_MIPS_GPREL16"
}

# Optional third argument: an object to use in place of the unit's current one
override=$3

paths=$(python3 -I -c 'import json, sys
for u in json.load(open("objdiff.json"))["units"]:
    if u["name"] == sys.argv[1]:
        print(u["target_path"], u["base_path"])' "$unit")
[ -n "$paths" ] || { echo "unknown unit: $unit" >&2; exit 1; }
set -- $paths
[ -n "$override" ] && set -- "$1" "$override"

target=$(mktemp)
current=$(mktemp)
dump "$1" > "$target"
dump "$2" > "$current"
diff -y -W 120 "$target" "$current" || true
rm -f "$target" "$current"
