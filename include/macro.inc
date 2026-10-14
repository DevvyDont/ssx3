.macro glabel label
    .global \label
    .type \label, @function
    \label:
.endm
# A jump table target inside a function. Global, so a jump table that is
# still in a data chunk object can reach it, but not typed as a function,
# so it doesn't split the function. (The SN assembler makes it an object
# symbol, so scripts/diff_func.sh disassembles with -D.)
.macro jlabel label
    .global \label
    \label:
.endm
.macro dlabel label
    .global \label
    \label:
.endm

# Closes out a data symbol opened with glabel/dlabel so it gets a proper
# ELF size (current splat/spimdisasm emits this after every data symbol).
.macro enddlabel label
    .size \label, . - \label
.endm

# Marks a function as not-yet-matched. Current splat/spimdisasm emits this
# unconditionally before every function split, regardless of match status;
# it's purely an informational marker (scripts/matching_report.py filters
# symbols ending in .NON_MATCHING when reading objdiff's symbol table), so
# this just emits that marker object and has no effect on the actual code.
.macro nonmatching label, size=1
    .global \label\().NON_MATCHING
    .type \label\().NON_MATCHING, @object
    .size \label\().NON_MATCHING, \size
    \label\().NON_MATCHING:
.endm
