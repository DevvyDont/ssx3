#!/bin/sh
# Show the instruction diff for one function between obj/target and obj/current.
# Run `python3 scripts/check_unit.py <unit>` (or ninja) first to rebuild.
#
#   scripts/diff_func.sh bxrandom cBxPseudoRng_Seed__FPUiUi
set -e
unit=$1
sym=$2
cd "$(dirname "$0")/.."

dump() {
	mips-linux-gnu-objdump -drz -m mips:5900 --no-show-raw-insn "$1" |
		awk -v s="<$sym>:" '$2 == s { p = 1; next } p && /^[0-9a-f]+ </ { p = 0 } p' |
		sed -E 's/^ +[0-9a-f]+:\t//; s/[0-9a-f]+ <[^>]*>/<addr>/'
}

target=$(mktemp)
current=$(mktemp)
dump "obj/target/$unit.o" > "$target"
dump "obj/current/$unit.o" > "$current"
diff -y -W 120 "$target" "$current" || true
rm -f "$target" "$current"
