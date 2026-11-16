#include "common.h"

/* Sony's crt0.s (EE startup), hand-written assembly: kept as one asm block per routine so the PS2 build stays
   byte-exact. notes/asm_origin.tsv in the workspace lists them; a PC port replaces the whole unit. */

//100%
INCLUDE_ASM("sce/crt0", func_00100000);
#ifdef SKIP_ASM
// PORT: PS2-only. The single `nop` at the start of .text (0x100000), ahead of the entry point func_00100008; part of
// Sony's hand-written crt0.s. A port drops it.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_00100000\n.type func_00100000, @function\nfunc_00100000:\n"
    "nop\n"
    ".size func_00100000, . - func_00100000\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("sce/crt0", func_00100008);
#ifdef SKIP_ASM
// PORT: PS2-only. Sony crt0.s `_start`, the EE entry point (hand-written: MMI padduw/mthi1/mtlo1/mtsah register clears,
// adda.s, sync.p, raw syscalls, ei). It zeroes every GPR/FPR/HI/LO/SA/ACC and FCSR, clears .bss (D_004A4C00 to
// D_0053F115) with sq, calls SetupThread (syscall 0x3C: gp D_004A30F0, stack D_1FE0000, size D_20000, args D_004A5C00,
// root func_001001D0) and takes the returned stack, SetupHeap (syscall 0x3D: D_0053F115, -1), then func_0042C300 and
// func_00424020(0), enables interrupts, runs main(argc, argv) from the args block and exits with func_0042C6E8(ret).
// A port replaces it with the platform's own startup.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_00100008\n.type func_00100008, @function\nfunc_00100008:\n"
    "padduw $1, $0, $0\n"
    "padduw $2, $0, $0\n"
    "padduw $3, $0, $0\n"
    "padduw $4, $0, $0\n"
    "padduw $5, $0, $0\n"
    "padduw $6, $0, $0\n"
    "padduw $7, $0, $0\n"
    "padduw $8, $0, $0\n"
    "padduw $9, $0, $0\n"
    "padduw $10, $0, $0\n"
    "padduw $11, $0, $0\n"
    "padduw $12, $0, $0\n"
    "padduw $13, $0, $0\n"
    "padduw $14, $0, $0\n"
    "padduw $15, $0, $0\n"
    "padduw $16, $0, $0\n"
    "padduw $17, $0, $0\n"
    "padduw $18, $0, $0\n"
    "padduw $19, $0, $0\n"
    "padduw $20, $0, $0\n"
    "padduw $21, $0, $0\n"
    "padduw $22, $0, $0\n"
    "padduw $23, $0, $0\n"
    "padduw $24, $0, $0\n"
    "padduw $25, $0, $0\n"
    "padduw $28, $0, $0\n"
    "padduw $29, $0, $0\n"
    "padduw $30, $0, $0\n"
    "padduw $31, $0, $0\n"
    "mthi $0\n"
    "mthi1 $0\n"
    "mtlo $0\n"
    "mtlo1 $0\n"
    "mtsah $0, 0x0\n"
    "mtc1 $0, $f0\n"
    "mtc1 $0, $f1\n"
    "mtc1 $0, $f2\n"
    "mtc1 $0, $f3\n"
    "mtc1 $0, $f4\n"
    "mtc1 $0, $f5\n"
    "mtc1 $0, $f6\n"
    "mtc1 $0, $f7\n"
    "mtc1 $0, $f8\n"
    "mtc1 $0, $f9\n"
    "mtc1 $0, $f10\n"
    "mtc1 $0, $f11\n"
    "mtc1 $0, $f12\n"
    "mtc1 $0, $f13\n"
    "mtc1 $0, $f14\n"
    "mtc1 $0, $f15\n"
    "mtc1 $0, $f16\n"
    "mtc1 $0, $f17\n"
    "mtc1 $0, $f18\n"
    "mtc1 $0, $f19\n"
    "mtc1 $0, $f20\n"
    "mtc1 $0, $f21\n"
    "mtc1 $0, $f22\n"
    "mtc1 $0, $f23\n"
    "mtc1 $0, $f24\n"
    "mtc1 $0, $f25\n"
    "mtc1 $0, $f26\n"
    "mtc1 $0, $f27\n"
    "mtc1 $0, $f28\n"
    "mtc1 $0, $f29\n"
    "mtc1 $0, $f30\n"
    "mtc1 $0, $f31\n"
    "adda.s $f0, $f1\n"
    "sync.p\n"
    "ctc1 $0, $31\n"
    "lui $2, %hi(D_004A4C00)\n"
    "lui $3, %hi(D_0053F115)\n"
    "addiu $2, $2, %lo(D_004A4C00)\n"
    "addiu $3, $3, %lo(D_0053F115)\n"
    ".L0010012C:\n"
    "sq $0, 0x0($2)\n"
    "nop\n"
    "sltu $1, $2, $3\n"
    "nop\n"
    "nop\n"
    "bnez $1, .L0010012C\n"
    "addiu $2, $2, 0x10\n"
    "lui $4, %hi(D_004A30F0)\n"
    "lui $5, %hi(D_1FE0000)\n"
    "lui $6, %hi(D_20000)\n"
    "lui $7, %hi(D_004A5C00)\n"
    "lui $8, %hi(func_001001D0)\n"
    "addiu $4, $4, %lo(D_004A30F0)\n"
    "addiu $5, $5, %lo(D_1FE0000)\n"
    "addiu $6, $6, %lo(D_20000)\n"
    "addiu $7, $7, %lo(D_004A5C00)\n"
    "addiu $8, $8, %lo(func_001001D0)\n"
    "daddu $28, $4, $0\n"
    "addiu $3, $0, 0x3C\n"
    "syscall 0\n"
    "daddu $29, $2, $0\n"
    "lui $4, %hi(D_0053F115)\n"
    "lui $5, (0x0 >> 16)\n"
    "addiu $4, $4, %lo(D_0053F115)\n"
    "addiu $5, $5, -0x1\n"
    "addiu $3, $0, 0x3D\n"
    "syscall 0\n"
    "jal func_0042C300\n"
    "nop\n"
    "jal func_00424020\n"
    "daddu $4, $0, $0\n"
    "ei\n"
    "lui $2, %hi(D_004A5C00)\n"
    "addiu $2, $2, %lo(D_004A5C00)\n"
    "lw $4, 0x0($2)\n"
    "jal main\n"
    "addiu $5, $2, 0x4\n"
    "j func_0042C6E8\n"
    "daddu $4, $2, $0\n"
    ".size func_00100008, . - func_00100008\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("sce/crt0", func_001001C8);
#ifdef SKIP_ASM
// PORT: PS2-only. Sony crt0.s exit stub: func_0042C6E8(0). A port uses the platform's exit.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_001001C8\n.type func_001001C8, @function\nfunc_001001C8:\n"
    "j func_0042C6E8\n"
    "daddu $4, $0, $0\n"
    ".size func_001001C8, . - func_001001C8\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("sce/crt0", func_001001D0);
#ifdef SKIP_ASM
// PORT: PS2-only. Sony crt0.s `_root`, the thread SetupThread installs as root: ExitThread (syscall 0x23), then a
// frame-pointer call func_00414728(D_0043CE00, D_004A5D48) (hand-written: raw syscall, `daddu $fp, $sp` frame).
// A port drops it.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_001001D0\n.type func_001001D0, @function\nfunc_001001D0:\n"
    "addiu $3, $0, 0x23\n"
    "syscall 0\n"
    "addiu $29, $29, -0x20\n"
    "sq $30, 0x10($29)\n"
    "sd $31, 0x0($29)\n"
    "daddu $30, $29, $0\n"
    "lui $4, %hi(D_0043CE00)\n"
    "addiu $4, $4, %lo(D_0043CE00)\n"
    "lui $5, %hi(D_004A5D48)\n"
    "addiu $5, $5, %lo(D_004A5D48)\n"
    "jal func_00414728\n"
    "nop\n"
    "daddu $29, $30, $0\n"
    "lq $30, 0x10($29)\n"
    "ld $31, 0x0($29)\n"
    "addiu $29, $29, 0x20\n"
    "jr $31\n"
    "nop\n"
    ".size func_001001D0, . - func_001001D0\n"
    ".set pop\n");
#endif
