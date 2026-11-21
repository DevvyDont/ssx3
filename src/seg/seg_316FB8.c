#include "common.h"
#include "seg/newlib_reent.h"

// PORT: PS2 kernel syscall stub: the syscall number goes in $3. Defined under a second C name with an
// asm label because this file declares these symbols with their argument lists elsewhere.
#define SYSCALL_STUB(name, n) \
    void name##_syscall(void) __asm__(#name); \
    void name##_syscall(void) { register int v __asm__("$3") = (n); __asm__ volatile("syscall" : : "r"(v)); }

//100%
INCLUDE_ASM("seg/seg_316FB8", __errno);
#ifdef SKIP_ASM
extern int D_00454944;
/**
 * Returns the address of errno: the current reentrancy struct D_00454944, whose first field is errno.
 *
 * @return a pointer to errno
 */
int __errno(void) {
    return D_00454944;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00415FC8);
#ifdef SKIP_ASM
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_00415FC8\n.ent func_00415FC8\nfunc_00415FC8:\n"
    "sltiu $2, $6, 0x10\n"
    "bnez $2, .L00416014\n"
    "or $2, $4, $5\n"
    "andi $2, $2, 0xF\n"
    "bnez $2, .L00416014\n"
    "nop\n"
    ".L00415FE0:\n"
    "lq $3, 0x0($4)\n"
    "sltiu $7, $6, 0x20\n"
    "lq $2, 0x0($5)\n"
    "addiu $4, $4, 0x10\n"
    "pxor $8, $2, $3\n"
    "addiu $2, $5, 0x10\n"
    "pcpyud $10, $8, $7\n"
    "or $9, $10, $8\n"
    "movz $5, $2, $9\n"
    "bnel $9, $0, .L00416014\n"
    "addiu $4, $4, -0x10\n"
    "beqz $7, .L00415FE0\n"
    "addiu $6, $6, -0x10\n"
    ".L00416014:\n"
    "lui $2, (0xFFFFFFFF >> 16)\n"
    "addiu $6, $6, -0x1\n"
    "ori $2, $2, (0xFFFFFFFF & 0xFFFF)\n"
    "beq $6, $2, .L00416054\n"
    "nop\n"
    "lui $7, (0xFFFFFFFF >> 16)\n"
    "ori $7, $7, (0xFFFFFFFF & 0xFFFF)\n"
    ".L00416030:\n"
    "lbu $3, 0x0($4)\n"
    "lbu $2, 0x0($5)\n"
    "beq $3, $2, .L00416048\n"
    "addiu $4, $4, 0x1\n"
    "jr $31\n"
    "subu $2, $3, $2\n"
    ".L00416048:\n"
    "addiu $6, $6, -0x1\n"
    "bne $6, $7, .L00416030\n"
    "addiu $5, $5, 0x1\n"
    ".L00416054:\n"
    "jr $31\n"
    "daddu $2, $0, $0\n"
    ".end func_00415FC8\n"
    ".set at\n.set reorder\n");
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041605C);

INCLUDE_ASM("seg/seg_316FB8", func_0041610C);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00416210);
#ifdef SKIP_ASM
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_00416210\n.ent func_00416210\nfunc_00416210:\n"
    "sltiu $2, $6, 0x8\n"
    "bnez $2, .L00416290\n"
    "daddu $3, $4, $0\n"
    "andi $2, $4, 0xF\n"
    "bnez $2, .L00416290\n"
    "daddu $7, $4, $0\n"
    "andi $9, $5, 0xFF\n"
    "sltiu $10, $6, 0x20\n"
    "daddu $8, $9, $0\n"
    "dsll $3, $8, 8\n"
    "or $8, $3, $9\n"
    "pcpyh $3, $8\n"
    "bnez $10, .L0041627C\n"
    "sltiu $2, $6, 0x8\n"
    "pcpyld $8, $3, $3\n"
    ".L0041624C:\n"
    "sq $8, 0x0($7)\n"
    "addiu $6, $6, -0x20\n"
    "addiu $7, $7, 0x10\n"
    "sltiu $2, $6, 0x20\n"
    "sq $8, 0x0($7)\n"
    "beqz $2, .L0041624C\n"
    "addiu $7, $7, 0x10\n"
    "b .L0041627C\n"
    "sltiu $2, $6, 0x8\n"
    ".L00416270:\n"
    "addiu $6, $6, -0x8\n"
    "addiu $7, $7, 0x8\n"
    "sltiu $2, $6, 0x8\n"
    ".L0041627C:\n"
    "nop\n"
    "nop\n"
    "beql $2, $0, .L00416270\n"
    "sd $3, 0x0($7)\n"
    "daddu $3, $7, $0\n"
    ".L00416290:\n"
    "lui $2, (0xFFFFFFFF >> 16)\n"
    "addiu $6, $6, -0x1\n"
    "ori $2, $2, (0xFFFFFFFF & 0xFFFF)\n"
    "beq $6, $2, .L004162C8\n"
    "nop\n"
    "lui $2, (0xFFFFFFFF >> 16)\n"
    "ori $2, $2, (0xFFFFFFFF & 0xFFFF)\n"
    ".L004162AC:\n"
    "sb $5, 0x0($3)\n"
    "addiu $6, $6, -0x1\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "bne $6, $2, .L004162AC\n"
    "addiu $3, $3, 0x1\n"
    ".L004162C8:\n"
    "jr $31\n"
    "daddu $2, $4, $0\n"
    ".end func_00416210\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strcat);
#ifdef SKIP_ASM
/**
 * Appends a string to another (C library strcat).
 *
 * Newlib's routine, kept as the binary's own instructions: finds the end of the destination (16 or 8 bytes at a time
 * with the 0x0101... / 0x8080... zero-byte test when aligned, else byte by byte), calls strcpy to copy the source
 * there, and returns the destination.
 */
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl strcat\n.ent strcat\nstrcat:\n"
    "addiu $29, $29, -0x20\n"
    "sq $16, 0x0($29)\n"
    "daddu $16, $4, $0\n"
    "andi $2, $16, 0x7\n"
    "bnez $2, .L004163CC\n"
    "sq $31, 0x10($29)\n"
    "andi $2, $16, 0xF\n"
    "ori $3, $0, 0x8080\n"
    "dsll $3, $3, 16\n"
    "ori $3, $3, 0x8080\n"
    "dsll $3, $3, 16\n"
    "ori $3, $3, 0x8080\n"
    "dsll $3, $3, 9\n"
    "ori $3, $3, 0x101\n"
    "ori $4, $0, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "bnez $2, .L00416388\n"
    "ld $6, 0x0($16)\n"
    "lq $2, 0x0($16)\n"
    "pcpyld $7, $3, $3\n"
    "pcpyld $8, $4, $4\n"
    "psubb $3, $2, $7\n"
    "pnor $2, $0, $2\n"
    "pand $3, $3, $2\n"
    "pand $3, $3, $8\n"
    "pcpyud $2, $3, $3\n"
    "or $3, $2, $3\n"
    "bnez $3, .L004163CC\n"
    "daddu $4, $16, $0\n"
    "addiu $6, $4, 0x10\n"
    ".L0041635C:\n"
    "lq $2, 0x0($6)\n"
    "pnor $3, $0, $2\n"
    "psubb $2, $2, $7\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $8\n"
    "pcpyud $3, $2, $2\n"
    "or $2, $2, $3\n"
    "beql $2, $0, .L0041635C\n"
    "addiu $6, $6, 0x10\n"
    "b .L004163CC\n"
    "daddu $4, $6, $0\n"
    ".L00416388:\n"
    "daddu $7, $3, $0\n"
    "daddu $8, $4, $0\n"
    "dsubu $3, $6, $3\n"
    "nor $2, $0, $6\n"
    "and $3, $3, $2\n"
    "and $3, $3, $4\n"
    "bnez $3, .L004163CC\n"
    "daddu $4, $16, $0\n"
    "addiu $6, $16, 0x8\n"
    ".L004163AC:\n"
    "ld $2, 0x0($6)\n"
    "nor $3, $0, $2\n"
    "dsubu $2, $2, $7\n"
    "and $2, $2, $3\n"
    "and $2, $2, $8\n"
    "beql $2, $0, .L004163AC\n"
    "addiu $6, $6, 0x8\n"
    "daddu $4, $6, $0\n"
    ".L004163CC:\n"
    "lb $2, 0x0($4)\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "bnel $2, $0, .L004163CC\n"
    "addiu $4, $4, 0x1\n"
    "jal strcpy\n"
    "nop\n"
    "daddu $2, $16, $0\n"
    "lq $31, 0x10($29)\n"
    "lq $16, 0x0($29)\n"
    "jr $31\n"
    "addiu $29, $29, 0x20\n"
    ".end strcat\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strchr);
#ifdef SKIP_ASM
/**
 * Finds the first occurrence of a character in a string (newlib strchr, EE-optimised).
 *
 * Scans 16 or 8 bytes at a time when the string is aligned. Searching for 0 returns the terminator.
 *
 * @param s  NUL-terminated string to search
 * @param c  character to find; only its low byte is used
 * @return a pointer to the first match, or 0 if there is none
 */
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 2\n.globl strchr\n.ent strchr\nstrchr:\n"
    "andi $2, $4, 0x7\n"
    "bnez $2, .L00416584\n"
    "andi $5, $5, 0xFF\n"
    "dsll $3, $5, 8\n"
    "ori $6, $0, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 9\n"
    "ori $6, $6, 0x101\n"
    "daddu $10, $3, $5\n"
    "andi $3, $4, 0xF\n"
    "dsll $2, $10, 16\n"
    "ori $8, $0, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "daddu $2, $2, $10\n"
    "dsll32 $10, $2, 0\n"
    "bnez $3, .L004164FC\n"
    "daddu $7, $2, $10\n"
    ".L00416468:\n"
    "lq $9, 0x0($4)\n"
    "pcpyld $10, $6, $6\n"
    "pnor $3, $0, $9\n"
    "psubb $2, $9, $10\n"
    "pcpyld $6, $8, $8\n"
    "pand $2, $2, $3\n"
    "pcpyld $8, $7, $7\n"
    "pand $2, $2, $6\n"
    "pcpyud $3, $2, $7\n"
    "or $3, $2, $3\n"
    "bnel $3, $0, .L00416588\n"
    "lbu $2, 0x0($4)\n"
    "pxor $2, $9, $8\n"
    "psubb $3, $2, $10\n"
    "pnor $2, $0, $2\n"
    "ori $8, $0, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "pand $3, $3, $2\n"
    "pand $3, $3, $6\n"
    "ori $6, $0, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 9\n"
    "ori $6, $6, 0x101\n"
    "pcpyud $2, $3, $5\n"
    "or $3, $2, $3\n"
    "beql $3, $0, .L00416468\n"
    "addiu $4, $4, 0x10\n"
    "b .L00416588\n"
    "lbu $2, 0x0($4)\n"
    ".L004164FC:\n"
    "ld $9, 0x0($4)\n"
    "nor $3, $0, $9\n"
    "dsubu $2, $9, $6\n"
    "and $2, $2, $3\n"
    "and $2, $2, $8\n"
    "bnel $2, $0, .L00416588\n"
    "lbu $2, 0x0($4)\n"
    "xor $2, $9, $7\n"
    "dsubu $3, $2, $6\n"
    "nor $2, $0, $2\n"
    "and $3, $3, $2\n"
    "and $3, $3, $8\n"
    "bnel $3, $0, .L00416588\n"
    "lbu $2, 0x0($4)\n"
    "daddu $9, $6, $0\n"
    "addiu $4, $4, 0x8\n"
    ".L0041653C:\n"
    "ld $6, 0x0($4)\n"
    "dsubu $2, $6, $9\n"
    "nor $3, $0, $6\n"
    "and $2, $2, $3\n"
    "and $2, $2, $8\n"
    "bnez $2, .L00416584\n"
    "xor $2, $6, $7\n"
    "nor $3, $0, $2\n"
    "dsubu $2, $2, $9\n"
    "and $2, $2, $3\n"
    "and $2, $2, $8\n"
    "beql $2, $0, .L0041653C\n"
    "addiu $4, $4, 0x8\n"
    "b .L00416588\n"
    "lbu $2, 0x0($4)\n"
    ".L00416578:\n"
    "beql $2, $5, .L00416598\n"
    "lbu $3, 0x0($4)\n"
    "addiu $4, $4, 0x1\n"
    ".L00416584:\n"
    "lbu $2, 0x0($4)\n"
    ".L00416588:\n"
    "nop\n"
    "bnez $2, .L00416578\n"
    "nop\n"
    "lbu $3, 0x0($4)\n"
    ".L00416598:\n"
    "daddu $2, $0, $0\n"
    "xor $3, $3, $5\n"
    "jr $31\n"
    "movz $2, $4, $3\n"
    ".end strchr\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004165A8);
#ifdef SKIP_ASM
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_004165A8\n.ent func_004165A8\nfunc_004165A8:\n"
    "or $8, $4, $5\n"
    "andi $2, $8, 0x7\n"
    "bnel $2, $0, .L004166E0\n"
    "lb $2, 0x0($4)\n"
    "andi $9, $8, 0xF\n"
    "ori $7, $0, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 9\n"
    "ori $7, $7, 0x101\n"
    "ori $6, $0, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "bnez $9, .L00416674\n"
    "ld $2, 0x0($5)\n"
    "lq $3, 0x0($4)\n"
    "pcpyld $8, $7, $7\n"
    "lq $2, 0x0($5)\n"
    "pcpyld $10, $6, $6\n"
    "psubw $7, $2, $3\n"
    "pcpyud $6, $7, $4\n"
    "or $3, $6, $7\n"
    "bnel $3, $0, .L004166E0\n"
    "lb $2, 0x0($4)\n"
    "lq $2, 0x0($4)\n"
    "pnor $3, $0, $2\n"
    ".L00416628:\n"
    "psubb $2, $2, $8\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $10\n"
    "pcpyud $3, $2, $4\n"
    "or $6, $3, $2\n"
    "beqz $6, .L0041664C\n"
    "addiu $4, $4, 0x10\n"
    "jr $31\n"
    "daddu $2, $0, $0\n"
    ".L0041664C:\n"
    "addiu $5, $5, 0x10\n"
    "lq $2, 0x0($4)\n"
    "lq $3, 0x0($5)\n"
    "psubw $7, $2, $3\n"
    "pcpyud $6, $7, $4\n"
    "or $9, $6, $7\n"
    "beql $9, $0, .L00416628\n"
    "pnor $3, $0, $2\n"
    "b .L004166E0\n"
    "lb $2, 0x0($4)\n"
    ".L00416674:\n"
    "ld $3, 0x0($4)\n"
    "bnel $3, $2, .L004166E0\n"
    "lb $2, 0x0($4)\n"
    "ld $2, 0x0($4)\n"
    "nor $8, $0, $2\n"
    ".L00416688:\n"
    "dsubu $2, $2, $7\n"
    "and $2, $2, $8\n"
    "and $2, $2, $6\n"
    "beqz $2, .L004166A4\n"
    "addiu $4, $4, 0x8\n"
    "jr $31\n"
    "daddu $2, $0, $0\n"
    ".L004166A4:\n"
    "addiu $5, $5, 0x8\n"
    "ld $2, 0x0($4)\n"
    "ld $3, 0x0($5)\n"
    "beql $3, $2, .L00416688\n"
    "nor $8, $0, $2\n"
    "b .L004166E0\n"
    "lb $2, 0x0($4)\n"
    ".L004166C0:\n"
    "sll $2, $3, 24\n"
    "lb $3, 0x0($5)\n"
    "sra $2, $2, 24\n"
    "bnel $2, $3, .L004166E8\n"
    "lbu $3, 0x0($4)\n"
    "addiu $4, $4, 0x1\n"
    "addiu $5, $5, 0x1\n"
    "lb $2, 0x0($4)\n"
    ".L004166E0:\n"
    "bnez $2, .L004166C0\n"
    "lbu $3, 0x0($4)\n"
    ".L004166E8:\n"
    "lbu $2, 0x0($5)\n"
    "jr $31\n"
    "subu $2, $3, $2\n"
    ".end func_004165A8\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strcpy);
#ifdef SKIP_ASM
/**
 * Copies a NUL-terminated string, including the terminator (newlib strcpy, EE-optimised).
 *
 * Copies 16 or 8 bytes at a time while both pointers are aligned and no terminator has been seen.
 *
 * @param dst  destination buffer
 * @param src  string to copy
 * @return @p dst
 */
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 2\n.globl strcpy\n.ent strcpy\nstrcpy:\n"
    "daddu $7, $4, $0\n"
    "or $8, $5, $7\n"
    "andi $2, $8, 0x7\n"
    "bnez $2, .L004167EC\n"
    "daddu $3, $7, $0\n"
    "andi $2, $8, 0xF\n"
    "ori $9, $0, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 9\n"
    "ori $9, $9, 0x101\n"
    "ori $4, $0, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "bnel $2, $0, .L004167AC\n"
    "ld $10, 0x0($5)\n"
    "pcpyld $10, $9, $9\n"
    "lq $9, 0x0($5)\n"
    "pcpyld $8, $4, $4\n"
    "psubb $2, $9, $10\n"
    "pnor $3, $0, $9\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $8\n"
    "pcpyud $4, $2, $9\n"
    "or $3, $2, $4\n"
    "bnez $3, .L004167E8\n"
    "daddu $6, $7, $0\n"
    ".L00416778:\n"
    "sq $9, 0x0($6)\n"
    "addiu $5, $5, 0x10\n"
    "lq $9, 0x0($5)\n"
    "psubb $2, $9, $10\n"
    "pnor $3, $0, $9\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $8\n"
    "pcpyud $4, $2, $9\n"
    "or $3, $2, $4\n"
    "beqz $3, .L00416778\n"
    "addiu $6, $6, 0x10\n"
    "b .L004167EC\n"
    "daddu $3, $6, $0\n"
    ".L004167AC:\n"
    "dsubu $2, $10, $9\n"
    "nor $3, $0, $10\n"
    "and $2, $2, $3\n"
    "and $2, $2, $4\n"
    "bnez $2, .L004167E8\n"
    "daddu $6, $7, $0\n"
    ".L004167C4:\n"
    "sd $10, 0x0($6)\n"
    "addiu $5, $5, 0x8\n"
    "ld $10, 0x0($5)\n"
    "nor $2, $0, $10\n"
    "dsubu $3, $10, $9\n"
    "and $3, $3, $2\n"
    "and $3, $3, $4\n"
    "beqz $3, .L004167C4\n"
    "addiu $6, $6, 0x8\n"
    ".L004167E8:\n"
    "daddu $3, $6, $0\n"
    ".L004167EC:\n"
    "lbu $2, 0x0($5)\n"
    "addiu $5, $5, 0x1\n"
    "sb $2, 0x0($3)\n"
    "sll $2, $2, 24\n"
    "nop\n"
    "bnez $2, .L004167EC\n"
    "addiu $3, $3, 0x1\n"
    "jr $31\n"
    "daddu $2, $7, $0\n"
    ".end strcpy\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strlen);
#ifdef SKIP_ASM
/**
 * Returns the length of a NUL-terminated string (newlib strlen, EE-optimised).
 *
 * Scans 16 or 8 bytes at a time when the string is aligned.
 *
 * @param s  string to measure
 * @return the number of bytes before the terminator
 */
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl strlen\n.ent strlen\nstrlen:\n"
    "andi $2, $4, 0x7\n"
    "bnez $2, .L00416934\n"
    "daddu $7, $4, $0\n"
    "andi $3, $4, 0xF\n"
    "ori $2, $0, 0x8080\n"
    "dsll $2, $2, 16\n"
    "ori $2, $2, 0x8080\n"
    "dsll $2, $2, 16\n"
    "ori $2, $2, 0x8080\n"
    "dsll $2, $2, 9\n"
    "ori $2, $2, 0x101\n"
    "bnez $3, .L004168BC\n"
    "daddu $5, $4, $0\n"
    "lq $3, 0x0($5)\n"
    "pcpyld $8, $2, $2\n"
    "ori $4, $0, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "psubb $2, $3, $8\n"
    "pnor $3, $0, $3\n"
    "pcpyld $9, $4, $4\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $9\n"
    "pcpyud $3, $2, $8\n"
    "or $6, $3, $2\n"
    "bnel $6, $0, .L00416934\n"
    "daddu $4, $5, $0\n"
    "addiu $5, $5, 0x10\n"
    ".L00416890:\n"
    "lq $2, 0x0($5)\n"
    "pnor $3, $0, $2\n"
    "psubb $2, $2, $8\n"
    "pand $2, $2, $3\n"
    "pand $4, $2, $9\n"
    "pcpyud $3, $4, $6\n"
    "or $3, $3, $4\n"
    "beql $3, $0, .L00416890\n"
    "addiu $5, $5, 0x10\n"
    "b .L00416934\n"
    "daddu $4, $5, $0\n"
    ".L004168BC:\n"
    "ld $3, 0x0($5)\n"
    "ori $4, $0, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsubu $2, $3, $2\n"
    "nor $3, $0, $3\n"
    "and $2, $2, $3\n"
    "and $2, $2, $4\n"
    "bnel $2, $0, .L00416934\n"
    "daddu $4, $5, $0\n"
    "ori $6, $0, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 16\n"
    "ori $6, $6, 0x8080\n"
    "dsll $6, $6, 9\n"
    "ori $6, $6, 0x101\n"
    "addiu $5, $5, 0x8\n"
    ".L00416914:\n"
    "ld $2, 0x0($5)\n"
    "nor $3, $0, $2\n"
    "dsubu $2, $2, $6\n"
    "and $2, $2, $3\n"
    "and $2, $2, $4\n"
    "beql $2, $0, .L00416914\n"
    "addiu $5, $5, 0x8\n"
    "daddu $4, $5, $0\n"
    ".L00416934:\n"
    "lb $2, 0x0($4)\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "bnel $2, $0, .L00416934\n"
    "addiu $4, $4, 0x1\n"
    "jr $31\n"
    "subu $2, $4, $7\n"
    ".end strlen\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00416958);
#ifdef SKIP_ASM
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_00416958\n.ent func_00416958\nfunc_00416958:\n"
    "daddu $8, $4, $0\n"
    "andi $2, $8, 0x7\n"
    "bnel $2, $0, .L00416A84\n"
    "lb $2, 0x0($4)\n"
    "andi $3, $8, 0xF\n"
    "bnel $3, $0, .L00416A0C\n"
    "ld $2, 0x0($8)\n"
    "ori $7, $0, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 9\n"
    "ori $7, $7, 0x101\n"
    "lq $2, 0x0($8)\n"
    "pcpyld $9, $7, $7\n"
    "ori $4, $0, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "psubb $3, $2, $9\n"
    "pnor $2, $0, $2\n"
    "pcpyld $10, $4, $4\n"
    "pand $3, $3, $2\n"
    "pand $3, $3, $10\n"
    "pcpyud $2, $3, $4\n"
    "or $3, $3, $2\n"
    "bnez $3, .L00416A7C\n"
    "daddu $7, $8, $0\n"
    "addiu $7, $7, 0x10\n"
    "nop\n"
    ".L004169E0:\n"
    "lq $2, 0x0($7)\n"
    "pnor $3, $0, $2\n"
    "psubb $2, $2, $9\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $10\n"
    "pcpyud $3, $2, $4\n"
    "or $2, $2, $3\n"
    "beqz $2, .L004169E0\n"
    "addiu $7, $7, 0x10\n"
    "b .L00416A7C\n"
    "addiu $7, $7, -0x10\n"
    ".L00416A0C:\n"
    "ori $9, $0, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 9\n"
    "ori $9, $9, 0x101\n"
    "ori $4, $0, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsll $4, $4, 16\n"
    "ori $4, $4, 0x8080\n"
    "dsubu $3, $2, $9\n"
    "nor $2, $0, $2\n"
    "and $3, $3, $2\n"
    "and $3, $3, $4\n"
    "bnez $3, .L00416A7C\n"
    "daddu $7, $8, $0\n"
    "addiu $7, $7, 0x8\n"
    ".L00416A60:\n"
    "ld $2, 0x0($7)\n"
    "nor $3, $0, $2\n"
    "dsubu $2, $2, $9\n"
    "and $2, $2, $3\n"
    "and $2, $2, $4\n"
    "beql $2, $0, .L00416A60\n"
    "addiu $7, $7, 0x8\n"
    ".L00416A7C:\n"
    "daddu $4, $7, $0\n"
    "lb $2, 0x0($4)\n"
    ".L00416A84:\n"
    "beqz $2, .L00416AAC\n"
    "addiu $6, $6, -0x1\n"
    "addiu $4, $4, 0x1\n"
    ".L00416A90:\n"
    "lb $2, 0x0($4)\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "bnel $2, $0, .L00416A90\n"
    "addiu $4, $4, 0x1\n"
    ".L00416AAC:\n"
    "lui $2, (0xFFFFFFFF >> 16)\n"
    "ori $2, $2, (0xFFFFFFFF & 0xFFFF)\n"
    "beq $6, $2, .L00416B0C\n"
    "nop\n"
    "lbu $2, 0x0($5)\n"
    "addiu $5, $5, 0x1\n"
    "sb $2, 0x0($4)\n"
    "sll $2, $2, 24\n"
    "beqz $2, .L00416B0C\n"
    "addiu $4, $4, 0x1\n"
    "lui $3, (0xFFFFFFFF >> 16)\n"
    "ori $3, $3, (0xFFFFFFFF & 0xFFFF)\n"
    "nop\n"
    ".L00416AE0:\n"
    "beql $6, $0, .L00416AE8\n"
    "sb $0, 0x0($4)\n"
    ".L00416AE8:\n"
    "addiu $6, $6, -0x1\n"
    "beq $6, $3, .L00416B0C\n"
    "nop\n"
    "lbu $2, 0x0($5)\n"
    "addiu $5, $5, 0x1\n"
    "sb $2, 0x0($4)\n"
    "sll $2, $2, 24\n"
    "bnez $2, .L00416AE0\n"
    "addiu $4, $4, 0x1\n"
    ".L00416B0C:\n"
    "jr $31\n"
    "daddu $2, $8, $0\n"
    ".end func_00416958\n"
    ".word 0x00000000\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00416B18);
#ifdef SKIP_ASM
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_00416B18\n.ent func_00416B18\nfunc_00416B18:\n"
    "bnez $6, .L00416B28\n"
    "or $3, $4, $5\n"
    ".L00416B20:\n"
    "jr $31\n"
    "daddu $2, $0, $0\n"
    ".L00416B28:\n"
    "andi $2, $3, 0x7\n"
    "bnez $2, .L00416C94\n"
    "daddu $2, $6, $0\n"
    "andi $2, $3, 0xF\n"
    "sltiu $7, $6, 0x10\n"
    "ori $9, $0, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 9\n"
    "ori $9, $9, 0x101\n"
    "or $2, $2, $7\n"
    "bnez $2, .L00416C08\n"
    "daddu $7, $4, $0\n"
    "lq $3, 0x0($4)\n"
    "pcpyld $10, $9, $9\n"
    "lq $2, 0x0($5)\n"
    "ori $8, $0, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "dsll $8, $8, 16\n"
    "ori $8, $8, 0x8080\n"
    "psubw $3, $3, $2\n"
    "pcpyld $9, $8, $8\n"
    "pcpyud $2, $3, $4\n"
    "daddu $8, $5, $0\n"
    "or $3, $2, $3\n"
    "bnez $3, .L00416C94\n"
    "daddu $2, $6, $0\n"
    "addiu $6, $6, -0x10\n"
    ".L00416BAC:\n"
    "beqz $6, .L00416B20\n"
    "nop\n"
    "lq $2, 0x0($7)\n"
    "pnor $3, $0, $2\n"
    "psubb $2, $2, $10\n"
    "pand $2, $2, $3\n"
    "pand $3, $2, $9\n"
    "pcpyud $2, $3, $4\n"
    "or $2, $2, $3\n"
    "bnez $2, .L00416B20\n"
    "addiu $7, $7, 0x10\n"
    "sltiu $2, $6, 0x10\n"
    "lq $3, 0x0($7)\n"
    "bnez $2, .L00416C88\n"
    "addiu $8, $8, 0x10\n"
    "lq $2, 0x0($8)\n"
    "psubw $3, $3, $2\n"
    "pcpyud $2, $3, $4\n"
    "or $2, $2, $3\n"
    "beql $2, $0, .L00416BAC\n"
    "addiu $6, $6, -0x10\n"
    "b .L00416C8C\n"
    "daddu $4, $7, $0\n"
    ".L00416C08:\n"
    "sltiu $2, $6, 0x8\n"
    "bnez $2, .L00416C88\n"
    "daddu $8, $5, $0\n"
    "ld $3, 0x0($4)\n"
    "ld $2, 0x0($5)\n"
    "bne $3, $2, .L00416C94\n"
    "daddu $2, $6, $0\n"
    "addiu $6, $6, -0x8\n"
    "ori $10, $0, 0x8080\n"
    "dsll $10, $10, 16\n"
    "ori $10, $10, 0x8080\n"
    "dsll $10, $10, 16\n"
    "ori $10, $10, 0x8080\n"
    "dsll $10, $10, 16\n"
    "ori $10, $10, 0x8080\n"
    "nop\n"
    ".L00416C48:\n"
    "beqz $6, .L00416B20\n"
    "nop\n"
    "ld $2, 0x0($7)\n"
    "nor $3, $0, $2\n"
    "dsubu $2, $2, $9\n"
    "and $2, $2, $3\n"
    "and $2, $2, $10\n"
    "bnez $2, .L00416B20\n"
    "addiu $7, $7, 0x8\n"
    "sltiu $2, $6, 0x8\n"
    "bnez $2, .L00416C88\n"
    "addiu $8, $8, 0x8\n"
    "ld $3, 0x0($7)\n"
    "ld $2, 0x0($8)\n"
    "beql $3, $2, .L00416C48\n"
    "addiu $6, $6, -0x8\n"
    ".L00416C88:\n"
    "daddu $4, $7, $0\n"
    ".L00416C8C:\n"
    "daddu $5, $8, $0\n"
    "daddu $2, $6, $0\n"
    ".L00416C94:\n"
    "beqz $2, .L00416CD0\n"
    "addiu $6, $6, -0x1\n"
    "b .L00416CC4\n"
    "lb $3, 0x0($4)\n"
    "nop\n"
    ".L00416CA8:\n"
    "beqz $6, .L00416B20\n"
    "nop\n"
    "beqz $7, .L00416B20\n"
    "addiu $4, $4, 0x1\n"
    "addiu $5, $5, 0x1\n"
    "lb $3, 0x0($4)\n"
    "addiu $6, $6, -0x1\n"
    ".L00416CC4:\n"
    "lb $2, 0x0($5)\n"
    "beq $3, $2, .L00416CA8\n"
    "lbu $7, 0x0($4)\n"
    ".L00416CD0:\n"
    "lbu $2, 0x0($5)\n"
    "lbu $3, 0x0($4)\n"
    "jr $31\n"
    "subu $2, $3, $2\n"
    ".end func_00416B18\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strncpy);
#ifdef SKIP_ASM
/**
 * Copies at most n characters of a string and pads the rest with zeros (newlib strncpy, EE-optimised).
 *
 * The destination is not terminated when @p src has n or more characters.
 *
 * @param dst  destination buffer of at least @p n bytes
 * @param src  string to copy
 * @param n    number of bytes to write
 * @return @p dst
 */
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl strncpy\n.ent strncpy\nstrncpy:\n"
    "daddu $8, $4, $0\n"
    "or $7, $5, $4\n"
    "addiu $10, $0, 0x10\n"
    "andi $2, $7, 0x7\n"
    "addiu $9, $0, 0x8\n"
    "bnez $2, .L00416E58\n"
    "andi $2, $7, 0xF\n"
    "movz $9, $10, $2\n"
    "bnez $2, .L00416DBC\n"
    "sltu $2, $6, $9\n"
    "bnez $2, .L00416E58\n"
    "nop\n"
    "ori $7, $0, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 9\n"
    "ori $7, $7, 0x101\n"
    "lq $3, 0x0($5)\n"
    "pcpyld $9, $7, $7\n"
    "pnor $3, $0, $3\n"
    "ori $7, $0, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "dsll $7, $7, 16\n"
    "ori $7, $7, 0x8080\n"
    "psubb $2, $3, $9\n"
    "pcpyld $10, $7, $7\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $10\n"
    "pcpyud $3, $2, $4\n"
    "or $3, $2, $3\n"
    "bnez $3, .L00416E54\n"
    "daddu $7, $8, $0\n"
    "lq $3, 0x0($5)\n"
    "addiu $6, $6, -0x10\n"
    "addiu $5, $5, 0x10\n"
    "sltiu $2, $6, 0x10\n"
    "sq $3, 0x0($7)\n"
    "bnez $2, .L00416E54\n"
    "addiu $7, $7, 0x10\n"
    "lq $2, 0x0($5)\n"
    "pnor $3, $0, $2\n"
    "psubb $2, $2, $9\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $10\n"
    "pcpyud $3, $2, $4\n"
    "or $2, $2, $3\n"
    "beql $2, $0, .L00416E20\n"
    "lq $3, 0x0($5)\n"
    "b .L00416E58\n"
    "daddu $4, $7, $0\n"
    ".L00416DBC:\n"
    "bnez $2, .L00416E58\n"
    "nop\n"
    "ld $3, 0x0($5)\n"
    "ori $9, $0, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 16\n"
    "ori $9, $9, 0x8080\n"
    "dsll $9, $9, 9\n"
    "ori $9, $9, 0x101\n"
    "ori $10, $0, 0x8080\n"
    "dsll $10, $10, 16\n"
    "ori $10, $10, 0x8080\n"
    "dsll $10, $10, 16\n"
    "ori $10, $10, 0x8080\n"
    "dsll $10, $10, 16\n"
    "ori $10, $10, 0x8080\n"
    "dsubu $2, $3, $9\n"
    "nor $3, $0, $3\n"
    "and $2, $2, $3\n"
    "and $2, $2, $10\n"
    "bnez $2, .L00416E54\n"
    "daddu $7, $8, $0\n"
    "ld $3, 0x0($5)\n"
    "nop\n"
    ".L00416E20:\n"
    "addiu $6, $6, -0x8\n"
    "addiu $5, $5, 0x8\n"
    "sltiu $2, $6, 0x8\n"
    "sd $3, 0x0($7)\n"
    "bnez $2, .L00416E54\n"
    "addiu $7, $7, 0x8\n"
    "ld $2, 0x0($5)\n"
    "nor $3, $0, $2\n"
    "dsubu $2, $2, $9\n"
    "and $2, $2, $3\n"
    "and $2, $2, $10\n"
    "beql $2, $0, .L00416E20\n"
    "ld $3, 0x0($5)\n"
    ".L00416E54:\n"
    "daddu $4, $7, $0\n"
    ".L00416E58:\n"
    "beqz $6, .L00416EA4\n"
    "daddu $2, $6, $0\n"
    "lbu $2, 0x0($5)\n"
    "addiu $6, $6, -0x1\n"
    "addiu $5, $5, 0x1\n"
    "sb $2, 0x0($4)\n"
    "sll $2, $2, 24\n"
    "bnez $2, .L00416E58\n"
    "addiu $4, $4, 0x1\n"
    "daddu $2, $6, $0\n"
    "beqz $2, .L00416EA4\n"
    "addiu $6, $6, -0x1\n"
    ".L00416E88:\n"
    "sb $0, 0x0($4)\n"
    "daddu $2, $6, $0\n"
    "addiu $4, $4, 0x1\n"
    "nop\n"
    "nop\n"
    "bnez $2, .L00416E88\n"
    "addiu $6, $6, -0x1\n"
    ".L00416EA4:\n"
    "jr $31\n"
    "daddu $2, $8, $0\n"
    ".end strncpy\n"
    ".word 0x00000000\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _fopen_r);
#ifdef SKIP_ASM
extern int __sflags(int, char *, int *);
extern char *__sfp(int);
extern int _open_r(int *, int, int, int);
extern long __sread(), __swrite(), __sseek();
extern void __sclose();
extern long fseek(char *, int, int);
char *_fopen_r_(int ptr, char *file, char *mode) __asm__("_fopen_r");
/**
 * Opens a file as a stdio stream for the given reentrancy struct and returns the stream, or null.
 *
 * Parses the mode with __sflags (which sets errno 22 for a bad mode), takes a free FILE from __sfp, and
 * opens the file with _open_r using the stream's reentrancy struct (+0x54), the open flags and permissions 0666
 * (438). On an open failure it frees the FILE again (flags = 0). Otherwise it stores the descriptor (+0xE) and the
 * stream flags (+0xC), makes the stream its own cookie and installs the descriptor callbacks __sread (read),
 * __swrite (write), __sseek (seek) and __sclose (close). Append mode (flag 0x100) seeks to the end with
 * fseek(stream, 0, 2).
 *
 * @param ptr   the reentrancy struct (receives errno)
 * @param file  the path
 * @param mode  the mode string ("r", "w", "a", optionally with "+")
 * @return the new stream, or null when the mode is bad, no FILE is free or the open fails
 */
char *_fopen_r_(int ptr, char *file, char *mode) {
    char *stream;
    int fileDesc, flags, oflags;
    if ((flags = __sflags(ptr, mode, &oflags)) == 0)
        return 0;
    if ((stream = __sfp(ptr)) == 0)
        return 0;
    if ((fileDesc = _open_r(*(int **)(stream + 0x54), (int)file, oflags, 438)) < 0) {
        *(short *)(stream + 0xC) = 0;
        return 0;
    }
    *(short *)(stream + 0xE) = fileDesc;
    *(short *)(stream + 0xC) = flags;
    *(char **)(stream + 0x1C) = stream;
    *(void **)(stream + 0x20) = __sread;
    *(void **)(stream + 0x24) = __swrite;
    *(void **)(stream + 0x28) = __sseek;
    *(void **)(stream + 0x2C) = __sclose;
    if (flags & 0x100)
        fseek(stream, 0, 2);
    return stream;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", fopen);
#ifdef SKIP_ASM
extern int D_00454944;
extern void _fopen_r(int, int, int);
/**
 * Opens a file as a stdio stream for the global reentrancy struct.
 *
 * Forwards to _fopen_r with D_00454944. The definition is declared void, but $v0 still holds the stream
 * _fopen_r returned.
 *
 * @param fileName  the path
 * @param mode  the mode string
 */
void fopen(int fileName, int mode) {
    _fopen_r(D_00454944, fileName, mode);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _cleanup_r);
#ifdef SKIP_ASM
extern void fflush(void);
extern int _fwalk(int, void (*)(void));
/**
 * Flushes every open stream of a reentrancy struct.
 *
 * Calls _fwalk with fflush as the per-stream function. __sinit stores this function in the struct's
 * cleanup hook (+0x3C), so exit-time cleanup flushes all streams.
 *
 * @param reent  the reentrancy struct
 */
void _cleanup_r(int reent) {
    _fwalk(reent, fflush);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _cleanup);
#ifdef SKIP_ASM
extern int D_00454944;
extern void _cleanup_r(int);
/**
 * Flushes every open stream of the global reentrancy struct D_00454944.
 *
 * Calls _cleanup_r(D_00454944). exit calls it.
 */
void _cleanup(void) {
    _cleanup_r(D_00454944);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", fseek);
#ifdef SKIP_ASM
extern struct _reent *g17008_impure __asm__("D_00454944");
extern void g17008_init(struct _reent *) __asm__("__sinit");
extern int g17008_fflush(struct __sFILE *) __asm__("fflush");
extern int g17008_fstat(struct _reent *, int, int *) __asm__("_fstat_r");
extern long g17008_sseek(int, long, int) __asm__("__sseek");
extern void g17008_smakebuf(struct __sFILE *) __asm__("__smakebuf");
extern int g17008_srefill(struct __sFILE *) __asm__("__srefill");
int fseek_(struct __sFILE *fp, long offset, int whence) __asm__("fseek");
/**
 * Repositions a stdio stream, seeking inside the buffer when it can, and returns 0 or -1.
 *
 * Runs the stdio set-up check (stream data from D_00454944, __sinit once) and first flushes a stream that is both
 * appending and writing (flags 0x108). A stream without a seek callback sets errno 29 (ESPIPE), a bad whence errno 22
 * (EINVAL). For whence 1 (current) it flushes with fflush, finds the current offset (cached +0x50 when flag
 * 0x1000 is set, else the seek callback), corrects it for buffered read or write data and continues as an absolute
 * seek.
 *
 * A buffer is made with __smakebuf when there is none. Unbuffered, read-write or line-buffered streams (flags 0x81A)
 * take the plain path. Otherwise only a regular file (_fstat_r st_mode 0x8000, seek callback __sseek) may be
 * optimised: it gets flag 0x400 and a block size of 0x400, while anything else gets 0x800 and the plain path. Whence 2
 * adds the file size from _fstat_r. When the target lies inside the current buffer (flag 0x2000 clear) it only
 * moves the buffer pointer; else it seeks the descriptor to the buffer start, refills with __srefill and skips the
 * bytes before the target. The plain path flushes, calls the seek callback with the original offset and whence, and
 * empties the buffer. Each success clears the EOF flag 0x20.
 *
 * @param stream  the stream
 * @param offset  the byte offset
 * @param whence  0 from the start, 1 from the current position, 2 from the end
 * @return 0 on success, -1 on failure
 */
int fseek_(struct __sFILE *stream, long offset, int whence) {
    long (*seekfn)(int, long, int);
    long target, curoff;
    unsigned n;
    int st[104 / 4];
    int havepos;
    struct _reent *ptr;
    if (((struct _reent*)stream->_data) == 0)
        (*(struct _reent**)&stream->_data) = g17008_impure;
    if (((struct _reent*)stream->_data)->__sdidinit == 0)
        g17008_init(((struct _reent*)stream->_data));
    ptr = ((struct _reent*)stream->_data);
    if ((*(int *)&(*(unsigned short*)&stream->_flags) & 0x108) == 264)
        g17008_fflush(stream);
    if ((seekfn = ((long (*)(int, long, int))stream->_seek)) == 0) {
        ptr->_errno = 29;
        return -1;
    }
    switch (whence) {
    case 1:
        g17008_fflush(stream);
        if ((*(unsigned short*)&stream->_flags) & 0x1000)
            curoff = stream->_offset;
        else {
            curoff = (*seekfn)(((int)stream->_cookie), 0, 1);
            if (curoff == -1)
                return -1;
        }
        if ((*(unsigned short*)&stream->_flags) & 4) {
            curoff -= stream->_r;
            if (stream->_ub._base)
                curoff -= stream->_ur;
        } else if ((*(unsigned short*)&stream->_flags) & 8 && stream->_p != 0)
            curoff += stream->_p - stream->_bf._base;
        offset += curoff;
        whence = 0;
        havepos = 1;
        break;
    case 0:
    case 2:
        havepos = 0;
        break;
    default:
        ptr->_errno = 22;
        return -1;
    }
    if (stream->_bf._base == 0)
        g17008_smakebuf(stream);
    if ((*(unsigned short*)&stream->_flags) & 0x81A)
        goto dumb;
    if (((*(unsigned short*)&stream->_flags) & 0x400) == 0) {
        if (seekfn != g17008_sseek || stream->_file < 0 || g17008_fstat(ptr, stream->_file, st) ||
            (st[1] & 0xF000) != 0x8000) {
            (*(unsigned short*)&stream->_flags) |= 0x800;
            goto dumb;
        }
        stream->_blksize = 0x400;
        (*(unsigned short*)&stream->_flags) |= 0x400;
    }
    if (whence == 0)
        target = offset;
    else {
        if (g17008_fstat(ptr, stream->_file, st))
            goto dumb;
        target = *(long *)&st[4] + offset;
    }
    if (!havepos) {
        if ((*(unsigned short*)&stream->_flags) & 0x1000)
            curoff = stream->_offset;
        else {
            curoff = (*seekfn)(((int)stream->_cookie), 0, 1);
            if (curoff == -1)
                goto dumb;
        }
        curoff -= stream->_r;
        if (stream->_ub._base)
            curoff -= stream->_ur;
    }
    if (stream->_ub._base) {
        curoff += stream->_r;
        n = stream->_up - stream->_bf._base;
        curoff -= n;
        n += stream->_ur;
    } else {
        n = stream->_p - stream->_bf._base;
        curoff -= n;
        n += stream->_r;
    }
    if (((*(unsigned short*)&stream->_flags) & 0x2000) == 0 && target >= curoff && target < curoff + n) {
        int bufOffset = target - curoff;
        stream->_p = stream->_bf._base + bufOffset;
        stream->_r = n - bufOffset;
        (*(unsigned short*)&stream->_flags) &= ~0x20;
        return 0;
    }
    curoff = target & -stream->_blksize;
    if ((*seekfn)(((int)stream->_cookie), curoff, 0) == -1)
        goto dumb;
    stream->_r = 0;
    (*(unsigned short*)&stream->_flags) &= ~0x20;
    n = target - curoff;
    if (n) {
        if (g17008_srefill(stream) || stream->_r < n)
            goto dumb;
        stream->_p += n;
        stream->_r -= n;
    }
    return 0;
dumb:
    if (g17008_fflush(stream) || (*seekfn)(((int)stream->_cookie), offset, whence) == -1)
        return -1;
    stream->_p = stream->_bf._base;
    stream->_r = 0;
    (*(unsigned short*)&stream->_flags) &= ~0x20;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _fwalk);
#ifdef SKIP_ASM
/**
 * Calls a function on every in-use stream of a reentrancy struct and returns the OR of the results.
 *
 * Walks the glue list that starts at +0x1D8 of the struct (next, count, FILE array of 0x58-byte entries) and calls the
 * function for each FILE whose flags (+0xC) are nonzero.
 *
 * @param r_   the reentrancy struct
 * @param fn_  the function to call with each stream
 * @return the bitwise OR of the function's results (0 when no stream is open)
 */
int _fwalk(int r_, void (*fn_)(void)) {
    char *reent = (char *)r_;
    int (*fn)(char *) = (int (*)(char *))fn_;
    struct _glue *glue;
    char *stream;
    int n;
    int ret = 0;
    for (glue = (struct _glue *)(reent + 0x1D8); glue; glue = ((struct G17418*)glue->_next)) {
        stream = ((char*)glue->_iobs);
        for (n = glue->_niobs; --n >= 0; stream += 0x58)
            if (*(short *)(stream + 0xC))
                ret |= fn(stream);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __smakebuf);
#ifdef SKIP_ASM
struct F174B0 {
    char *p;
    int r;
    int w;
    unsigned short flags;
    short file;
    char *base;
    int size;
    char pad1[0x28 - 0x18];
    void *seek;
    char pad2[0x43 - 0x2C];
    char nbuf[9];
    int blksize;
    int offset;
    int data;
};
extern long __sseek();
extern int _fstat_r(int *, int, int);
/**
 * Gives a stdio stream its buffer: in this build always the stream's own one-byte buffer, making it unbuffered.
 *
 * An already unbuffered stream (flag 2) just points the buffer base and pointer at its one-byte buffer (+0x43) with
 * size 1. Otherwise it checks the descriptor with _fstat_r: a regular file (st_mode 0x8000) whose seek callback is
 * __sseek gets flag 0x400 and a block size of 0x400 (+0x4C), anything else (or a negative descriptor or a failed
 * fstat) gets flag 0x800. Then it sets flag 2 and installs the one-byte buffer.
 *
 * Quirk: no buffer is ever allocated, so every stream it sets up is unbuffered (each byte goes straight to the read or
 * write callback).
 *
 * @param stream  the stream
 */
void __smakebuf(struct F174B0 *stream) {
    int st[104 / 4];
    if (stream->flags & 2) {
        stream->p = stream->nbuf;
        stream->base = stream->nbuf;
        stream->size = 1;
        return;
    }
    if (stream->file < 0 || _fstat_r((int *)stream->data, stream->file, (int)st) < 0) {
        stream->flags |= 0x800;
    } else if ((st[1] & 0xF000) == 0x8000 && stream->seek == (void *)__sseek) {
        stream->blksize = 0x400;
        stream->flags |= 0x400;
    } else {
        stream->flags |= 0x800;
    }
    stream->flags |= 2;
    stream->size = 1;
    stream->p = stream->nbuf;
    stream->base = stream->nbuf;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _printf_r);
#ifdef SKIP_ASM
#define _printf_r_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern void func_0041B728(int *, int, int, char *);
/**
 * Prints formatted text to the standard output stream of a reentrancy struct.
 *
 * Builds the variable-argument pointer and calls func_0041B728(reent, the struct's stdout (+8), fmt, ap). The
 * definition is declared void, so the character count func_0041B728 returns is not part of the signature.
 *
 * @param reent  the reentrancy struct
 * @param fmt    the format string
 */
void _printf_r(int *reent, int fmt, ...) {
    char *ap;
    _printf_r_va_start(ap);
    func_0041B728(reent, reent[2], fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", printf);
#ifdef SKIP_ASM
#define printf_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern int D_00454944;
extern int vfprintf(int, int, char *);
/**
 * Prints formatted text to the standard output stream of the global reentrancy struct.
 *
 * Sets the stdout stream's reentrancy pointer (+0x54) to D_00454944 and calls the dispatcher vfprintf with stdout,
 * the format and the arguments. PRINT_debuggerfunc calls it.
 *
 * @param fmt  the format string
 */
void printf(int fmt, ...) {
    char *ap;
    int *reent;
    printf_va_start(ap);
    reent = (int *)D_00454944;
    ((int *)reent[2])[0x54 / 4] = (int)reent;
    vfprintf(reent[2], fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", lflush);
#ifdef SKIP_ASM
extern void fflush(void);
/**
 * Flushes the stream it is given (the per-stream callback of the refill path).
 *
 * Declared without parameters: the stream arrives in the first argument register and goes straight on to fflush.
 * __srefill passes it to _fwalk to flush the other streams before a read.
 *
 * Quirk: there is no line-buffered-writer test, so every open stream is flushed, not only the line-buffered ones.
 */
void lflush(void) {
    fflush();
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __srefill);
#ifdef SKIP_ASM
struct R7648 {
    int err;
    char pad[0x38 - 4];
    int didinit;
};
struct F7648 {
    unsigned char *p;
    int r;
    int w;
    unsigned short flags;
    short file;
    unsigned char *base;
    int size;
    int lbfsize;
    int cookie;
    int (*rd)(int, unsigned char *, int);
    int pad24;
    int pad28;
    int pad2C;
    unsigned char *ubbase;
    int ubsize;
    unsigned char *up;
    int ur;
    char pad40[0x54 - 0x40];
    struct R7648 *data;
};
extern struct R7648 *g7648_impure __asm__("D_00454944");
extern void g7648_init(struct R7648 *) __asm__("__sinit");
extern int g7648_fflush(struct F7648 *) __asm__("fflush");
extern void g7648_smakebuf(struct F7648 *) __asm__("__smakebuf");
extern int g7648_fwalk(struct R7648 *, void *) __asm__("_fwalk");
extern void g7648_lflush(void) __asm__("lflush");
int __srefill_impl(struct F7648 *fp) __asm__("__srefill");
/* newlib's CHECK_INIT (stdio/local.h), a do { } while (0) macro: make sure stdio is set up (98.89 without the macro's
   loop) */
#define CHECK_INIT_7648(fp)                     \
  do {                                          \
      if ((fp)->data == 0)                      \
          (fp)->data = g7648_impure;            \
      if ((fp)->data->didinit == 0)             \
          g7648_init((fp)->data);               \
  } while (0)
/* __srefill */
/**
 * Refills a stdio stream's read buffer and returns 0, or -1 at end of file or on an error.
 *
 * Runs the stdio set-up check, clears the read count and fails at once when the EOF flag (0x20) is set. A stream not
 * yet reading must be read-write (0x10): a pending write is flushed with fflush and the write state cleared,
 * then the read flag (4) is set. When an ungetc buffer is in use, it switches back to the saved read pointer and count
 * instead. A missing buffer is made with __smakebuf; for an unbuffered or line-buffered stream (flags 3)
 * _fwalk first flushes all streams with lflush. Then it calls the read callback for a full buffer: 0
 * bytes sets the EOF flag, a negative result the error flag (0x40).
 *
 * @param fp  the stream
 * @return 0 when data was read, -1 at end of file or on an error
 */
int __srefill_impl(struct F7648 *fp) {
    int n;
    CHECK_INIT_7648(fp);
    fp->r = 0;
    if (fp->flags & 0x20)
        return -1;
    if ((fp->flags & 4) == 0) {
        if ((fp->flags & 0x10) == 0)
            return -1;
        if (fp->flags & 8) {
            if (g7648_fflush(fp))
                return -1;
            fp->flags &= ~8;
            fp->w = 0;
            fp->lbfsize = 0;
        }
        fp->flags |= 4;
    } else if (fp->ubbase != 0) {
        if ((fp->r = fp->ur) != 0) {
            fp->p = fp->up;
            return 0;
        }
    }
    if (fp->base == 0)
        g7648_smakebuf(fp);
    if (fp->flags & 3)
        g7648_fwalk(fp->data, g7648_lflush);
    fp->p = fp->base;
    n = fp->rd(fp->cookie, fp->base, fp->size);
    fp->r = n;
    fp->flags &= ~0x2000;
    if (n <= 0) {
        if (n == 0) {
            fp->flags |= 0x20;
        } else {
            fp->r = 0;
            fp->flags |= 0x40;
        }
        return -1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _sprintf_r);
#ifdef SKIP_ASM
#define _sprintf_r_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
/**
 * Formats into a caller buffer with no length limit, for a given reentrancy struct.
 *
 * Builds a string stream on the stack (flags 0x208, write and buffer size 0x7FFFFFFF, reentrancy pointer reent), calls
 * vfprintf and terminates the text. The definition is declared void, so the count is not part of the signature.
 *
 * @param reent  the reentrancy struct
 * @param buf    the output buffer
 * @param fmt    the format string
 */
void _sprintf_r(int reent, char *buf, int fmt, ...) {
    char *ap;
    struct __sFILE strStream;
    _sprintf_r_va_start(ap);
    (*(char**)&strStream._p) = buf;
    strStream._w = 0x7FFFFFFF;
    strStream._flags = 520;
    (*(char**)&strStream._bf) = buf;
    strStream._bf._size = 0x7FFFFFFF;
    (*(int*)&strStream._data) = reent;
    vfprintf((int)&strStream, fmt, ap);
    *((char*)strStream._p) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", sprintf);
#ifdef SKIP_ASM
#define sprintf_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
struct Fsprintf {
    char *p;
    int r;
    int w;
    short flags, file;
    char *base;
    int size;
    int pad[0x54 / 4 - 6];
    int ent;
};
extern int D_00454944;
/**
 * Formats into a string buffer with no size limit (newlib's sprintf, writing through a string FILE).
 *
 * Sets up a FILE with flags 0x208 (write, string), buffer @p buf, size INT_MAX and the reentrancy data
 * D_00454944, runs the formatter vfprintf (the floating-point or the integer-only vfprintf, by format)
 * and terminates the output. The C declaration returns void; the character count stays in the return
 * register.
 *
 * @param buf  destination buffer
 * @param fmt  the format string (typed int here), followed by its arguments
 */
void sprintf(char *buf, int fmt, ...) {
    char *ap;
    struct Fsprintf strFile;
    sprintf_va_start(ap);
    strFile.p = buf;
    strFile.w = 0x7FFFFFFF;
    strFile.flags = 520;
    strFile.base = buf;
    strFile.size = 0x7FFFFFFF;
    strFile.ent = D_00454944;
    vfprintf((int)&strFile, fmt, ap);
    *strFile.p = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", eofread);
#ifdef SKIP_ASM
/**
 * Returns 0: the read callback of a string stream, which reports end of input.
 *
 * sscanf installs it as the read callback of its string stream.
 *
 * @return always 0
 */
int eofread(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", sscanf);
#ifdef SKIP_ASM
#define sscanf_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
struct F178B0 {
    char *p;
    int r;
    int w;
    short flags, file;
    char *base;
    int size;
    int lbfsize;
    int cookie;
    int (*read)(void);
    int pad1[3];
    int ubbase;
    int pad2[0x44 / 4 - 0x34 / 4];
    int lbbase;
    int pad3[0x54 / 4 - 0x48 / 4];
    int ent;
};
extern int strlen(char *);
extern int D_00454944;
extern int __svfscanf(struct F178B0 *, int, char *);
/**
 * Scans formatted input from a string and returns the number of fields assigned.
 *
 * Builds a read-only string stream (flags 4, buffer and count from the string and its strlen, read callback
 * eofread, no ungetc or line buffer, reentrancy pointer D_00454944) and calls __svfscanf. Callers include
 * FileParser_GetInt, FileParser_GetFloat and LobbyMan_onchatmsg.
 *
 * @param str  the input string
 * @param fmt  the scanf format
 * @return the number of fields assigned, or -1 when the input ends before the first conversion
 */
int sscanf(char *str, int fmt, ...) {
    char *ap;
    struct F178B0 strStream;
    strStream.flags = 4;
    strStream.p = str;
    strStream.base = str;
    strStream.size = strStream.r = strlen(str);
    strStream.read = eofread;
    strStream.ubbase = 0;
    strStream.lbbase = 0;
    strStream.ent = D_00454944;
    sscanf_va_start(ap);
    return __svfscanf(&strStream, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sread);
#ifdef SKIP_ASM
extern long _read_r(int *, int, int, int);
/**
 * Reads from a stream's descriptor (the read callback of file streams) and returns the byte count or -1.
 *
 * Calls _read_r with the stream's reentrancy struct (+0x54) and descriptor (+0xE). A successful read advances the
 * cached offset (+0x50); a failure clears the offset-valid flag 0x1000.
 *
 * @param stream  the stream (its own cookie)
 * @param buf     the destination
 * @param count   bytes wanted
 * @return the bytes read, or -1
 */
long __sread(int *stream, int buf, int count) {
    int bytesRead = _read_r((int *)stream[84 / 4], ((short *)stream)[7], buf, count);
    if (bytesRead >= 0) {
        stream[80 / 4] += bytesRead;
    } else {
        ((unsigned short *)stream)[6] &= 0xEFFF;
    }
    return bytesRead;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __swrite);
#ifdef SKIP_ASM
extern long _write_r(int *, int, int, int);
int __swrite_(int *f, int a, int b) __asm__("__swrite");
/**
 * Writes to a stream's descriptor (the write callback of file streams) and returns the byte count or -1.
 *
 * An append-mode stream (flag 0x100) first seeks the descriptor to the end with _lseek_r. Clears the offset-valid
 * flag 0x1000, then writes with _write_r.
 *
 * @param stream  the stream (its own cookie)
 * @param buf     the data
 * @param count   bytes to write
 * @return the bytes written, or -1
 */
int __swrite_(int *stream, int buf, int count) {
    if (((unsigned short *)stream)[6] & 0x100)
        _lseek_r((int *)stream[84 / 4], ((short *)stream)[7], 0, 2);
    ((unsigned short *)stream)[6] &= 0xEFFF;
    return _write_r((int *)stream[84 / 4], ((short *)stream)[7], buf, count);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sseek);
#ifdef SKIP_ASM
extern long _lseek_r(int *, int, int, int);
/**
 * Seeks a stream's descriptor (the seek callback of file streams) and returns the new offset or -1.
 *
 * Calls _lseek_r. On success it caches the offset (+0x50) and sets the offset-valid flag 0x1000; on failure it
 * clears that flag.
 *
 * @param stream  the stream (its own cookie)
 * @param offset  the offset
 * @param whence  0 start, 1 current, 2 end
 * @return the new offset, or -1
 */
long __sseek(int *stream, int offset, int whence) {
    long newOffset = _lseek_r((int *)stream[84 / 4], ((short *)stream)[7], offset, whence);
    unsigned short *flagsPtr = (unsigned short *)stream + 6;
    if (newOffset == -1) {
        *flagsPtr &= 0xEFFF;
    } else {
        stream[80 / 4] = newOffset;
        *flagsPtr |= 0x1000;
    }
    return newOffset;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sclose);
#ifdef SKIP_ASM
extern int _close_r(int, int);
/**
 * Closes a stream's descriptor (the close callback of file streams).
 *
 * Calls _close_r with the stream's reentrancy struct and descriptor. The definition is declared void; $v0 still
 * carries _close_r's result.
 *
 * @param stream  the stream (its own cookie)
 */
void __sclose(int *stream) {
    _close_r(stream[84 / 4], ((short *)stream)[7]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __svfscanf);
#ifdef SKIP_ASM
/* newlib libc/stdio/vfscanf.c (__svfscanf). The body follows newlib 1.9.0 (MB_CAPABLE format scan) with the
 * CT_FLOAT conversion of the earlier newlib releases the retail code shows (atof, no leading-zero exponent fixup):
 *
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* EE EABI va_arg (gcc 2.95 va-mips.h, __mips64 soft/single-float branch), for types of at most 8 bytes */
#define va_arg_00417AB8(__AP, __type) (*(__type *) (void *) ((__AP += 8) - 8))
/* newlib ctype.h; D_00499C81 is _ctype_ + 1 */
#define isspace_00417AB8(c)	(D_00499C81[(unsigned)(c)]&010)
#define isupper_00417AB8(c)	(D_00499C81[(unsigned)(c)]&01)
#define MB_CUR_MAX_00417AB8 (*(int *)D_00455140)
extern char D_00499C81[];
extern char D_00455140[];
extern const short D_00499D90[17]; /* static const short basefix[17] */
extern int mbtowc_r_7AB8(int, unsigned short *, char *, int, int *) __asm__("_mbtowc_r");
extern char *__sccl(char *, char *);
extern unsigned fread(char *buf, unsigned size, unsigned count, int *fp);
extern int ungetc_7AB8(int, struct F7648 *) __asm__("ungetc");
extern unsigned long g7AB8_strtol() __asm__("strtol");
extern unsigned long g7AB8_strtoul() __asm__("strtoul");
extern double g7AB8_atof(char *) __asm__("atof");

#define	BUF_00417AB8	(308+39+3)	/* MAXEXP+MAXFRACT+3: 3 = sign + decimal point + NUL */

#define	LONG		0x01	/* l: long or double */
#define	LONGDBL		0x02	/* L: long double */
#define	SHORT		0x04	/* h: short */
#define	SUPPRESS	0x08	/* suppress assignment */
#define	POINTER		0x10	/* weird %p pointer (`fake hex') */
#define	NOSKIP		0x20	/* do not skip blanks */
#define	SIGNOK		0x40	/* +/- is (still) legal */
#define	NDIGITS		0x80	/* no digits detected */
#define	DPTOK		0x100	/* (float) decimal point is still legal */
#define	EXPOK		0x200	/* (float) exponent (e+3, etc) still legal */
#define	PFXOK		0x100	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200	/* no zero digits detected */
#define	CT_CHAR		0	/* %c conversion */
#define	CT_CCL		1	/* %[...] conversion */
#define	CT_STRING	2	/* %s conversion */
#define	CT_INT		3	/* integer, i.e., strtol or strtoul */
#define	CT_FLOAT	4	/* floating, i.e., strtod */
#define u_char_00417AB8 char
#define u_long_00417AB8 unsigned long

#define BufferEmpty_00417AB8 (fp->r <= 0 && __srefill_impl(fp))

int __svfscanf_(struct F7648 *fp, char *fmt0, char *ap) __asm__("__svfscanf");
/**
 * Scans formatted input from a stream (the scanf engine) and returns the number of fields assigned.
 *
 * The newlib scanf state machine: white space in the format skips input white space (ctype table D_00499C81), other
 * characters must match, and % conversions take an optional *, width and h/l/L size. Supported: %c, %s, %[ (set built
 * by __sccl), %d %i %o %u %x %X %p (digits collected into a buffer and converted through a function pointer:
 * strtol, or strtoul for the unsigned ones), %e %f %g (atof, then dptofp for a float target), %n and %%.
 * Characters read too far are pushed back with ungetc. Multibyte format characters are decoded with _mbtowc_r when
 * the locale's MB_CUR_MAX (D_00455140) is above 1. %c without suppression reads with fread; buffer refills go
 * through __srefill.
 *
 * @param fp    the input stream
 * @param fmt0  the scanf format
 * @param ap    the argument pointer
 * @return the number of fields assigned, or -1 when the input fails before the first conversion
 */
int
__svfscanf_(struct F7648 *fp, char *fmt0, char *ap)
{
  register u_char_00417AB8 *fmt = (u_char_00417AB8 *) fmt0;
  register int c;		/* character from format, or conversion */
  register unsigned int width;	/* field width, or 0 */
  register char *strPtr;		/* points into all kinds of strings */
  register int n;		/* handy integer */
  register int flags;		/* flags as defined above */
  register char *strStart;		/* saves original value of p when necessary */
  int nassigned;		/* number of fields assigned */
  int nread;			/* number of characters consumed from fp */
  int base = 0;			/* base argument to strtol/strtoul */
  int nbytes = 1;               /* number of bytes read from fmt string */
  unsigned short wc;            /* wchar to use to read format string */

  u_long_00417AB8 (*ccfn) () = 0;	/* conversion function (strtol/strtoul) */
  char ccltab[256];		/* character class table for %[...] */
  char buf[BUF_00417AB8];	/* buffer for numeric conversions */
  char *lptr;                   /* literal pointer */
  int state = 0;                /* value to keep track of multibyte state */

  short *sp;
  int *ip;
  float *flp;
  double *ldp;
  double *dp;
  long *lp;

  /* `basefix' is used to avoid `if' tests in the integer scanner: the function's static table
     {10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} is retail's D_00499D90 */

  nassigned = 0;
  nread = 0;
  for (;;)
    {
      nbytes = mbtowc_r_7AB8 (D_00454944, &wc, fmt, MB_CUR_MAX_00417AB8, &state);
      fmt += nbytes;
      if (wc == 0)
	return nassigned;
      if (nbytes == 1 && isspace_00417AB8 (wc))
	{
	  for (;;)
	    {
	      if (BufferEmpty_00417AB8)
		return nassigned;
	      if (!isspace_00417AB8 (*fp->p))
		break;
	      nread++, fp->r--, fp->p++;
	    }
	  continue;
	}
      if (wc != '%')
	goto literal;
      width = 0;
      flags = 0;

      /*
       * switch on the format.  continue if done; break once format
       * type is derived.
       */

    again:
      c = *fmt++;

      switch (c)
	{
	case '%':
	literal:
          lptr = fmt - nbytes;
          for (n = 0; n < nbytes; ++n)
            {
	      if (BufferEmpty_00417AB8)
	        goto input_failure;
	      if (*fp->p != *lptr)
	        goto match_failure;
	      fp->r--, fp->p++;
	      nread++;
              ++lptr;
            }
	  continue;

	case '*':
	  flags |= SUPPRESS;
	  goto again;
	case 'l':
	  flags |= LONG;
	  goto again;
	case 'L':
	  flags |= LONGDBL;
	  goto again;
	case 'h':
	  flags |= SHORT;
	  goto again;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  width = width * 10 + c - '0';
	  goto again;

	case 'D':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'd':
	  c = CT_INT;
	  ccfn = (u_long_00417AB8 (*)())g7AB8_strtol;
	  base = 10;
	  break;

	case 'i':
	  c = CT_INT;
	  ccfn = (u_long_00417AB8 (*)())g7AB8_strtol;
	  base = 0;
	  break;

	case 'O':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'o':
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 8;
	  break;

	case 'u':
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 10;
	  break;

	case 'X':		/* compat   XXX */
	case 'x':
	  flags |= PFXOK;	/* enable 0x prefixing */
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 16;
	  break;

	case 'E':		/* compat   XXX */
	case 'G':		/* compat   XXX */
/* ANSI says that E,G and X behave the same way as e,g,x */
	  /* FALLTHROUGH */
	case 'e':
	case 'f':
	case 'g':
	  c = CT_FLOAT;
	  break;

	case 's':
	  c = CT_STRING;
	  break;

	case '[':
	  fmt = __sccl (ccltab, fmt);
	  flags |= NOSKIP;
	  c = CT_CCL;
	  break;

	case 'c':
	  flags |= NOSKIP;
	  c = CT_CHAR;
	  break;

	case 'p':		/* pointer format is like hex */
	  flags |= POINTER | PFXOK;
	  c = CT_INT;
	  ccfn = g7AB8_strtoul;
	  base = 16;
	  break;

	case 'n':
	  if (flags & SUPPRESS)	/* ??? */
	    continue;
	  if (flags & SHORT)
	    {
	      sp = va_arg_00417AB8 (ap, short *);
	      *sp = nread;
	    }
	  else if (flags & LONG)
	    {
	      lp = va_arg_00417AB8 (ap, long *);
	      *lp = nread;
	    }
	  else
	    {
	      ip = va_arg_00417AB8 (ap, int *);
	      *ip = nread;
	    }
	  continue;

	  /*
	   * Disgusting backwards compatibility hacks.	XXX
	   */
	case '\0':		/* compat */
	  return -1;

	default:		/* compat */
	  if (isupper_00417AB8 (c))
	    flags |= LONG;
	  c = CT_INT;
	  ccfn = (u_long_00417AB8 (*)())g7AB8_strtol;
	  base = 10;
	  break;
	}

      /*
       * We have a conversion that requires input.
       */
      if (BufferEmpty_00417AB8)
	goto input_failure;

      /*
       * Consume leading white space, except for formats that
       * suppress this.
       */
      if ((flags & NOSKIP) == 0)
	{
	  while (isspace_00417AB8 (*fp->p))
	    {
	      nread++;
	      if (--fp->r > 0)
		fp->p++;
	      else
	      if (__srefill_impl (fp))
		goto input_failure;
	    }
	  /*
	   * Note that there is at least one character in the
	   * buffer, so conversions that do not set NOSKIP ca
	   * no longer result in an input failure.
	   */
	}

      /*
       * Do the conversion.
       */
      switch (c)
	{

	case CT_CHAR:
	  /* scan arbitrary characters (sets NOSKIP) */
	  if (width == 0)
	    width = 1;
	  if (flags & SUPPRESS)
	    {
	      unsigned int sum = 0;

	      for (;;)
		{
		  if ((n = fp->r) < width)
		    {
		      sum += n;
		      width -= n;
		      fp->p += n;
		      if (__srefill_impl (fp))
			{
			  if (sum == 0)
			    goto input_failure;
			  break;
			}
		    }
		  else
		    {
		      sum += width;
		      fp->r -= width;
		      fp->p += width;
		      break;
		    }
		}
	      nread += sum;
	    }
	  else
	    {
	      unsigned int charsRead = fread ((void *) va_arg_00417AB8 (ap, char *), 1, width, (int *) fp);

	      if (charsRead == 0)
		goto input_failure;
	      nread += charsRead;
	      nassigned++;
	    }
	  break;

	case CT_CCL:
	  /* scan a (nonempty) character class (sets NOSKIP) */
	  if (width == 0)
	    width = ~0;		/* `infinity' */
	  /* take only those things in the class */
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (ccltab[*fp->p])
		{
		  n++, fp->r--, fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    {
		      if (n == 0)
			goto input_failure;
		      break;
		    }
		}
	      if (n == 0)
		goto match_failure;
	    }
	  else
	    {
	      strStart = strPtr = va_arg_00417AB8 (ap, char *);
	      while (ccltab[*fp->p])
		{
		  fp->r--;
		  *strPtr++ = *fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    {
		      if (strPtr == strStart)
			goto input_failure;
		      break;
		    }
		}
	      n = strPtr - strStart;
	      if (n == 0)
		goto match_failure;
	      *strPtr = 0;
	      nassigned++;
	    }
	  nread += n;
	  break;

	case CT_STRING:
	  /* like CCL, but zero-length string OK, & no NOSKIP */
	  if (width == 0)
	    width = ~0;
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (!isspace_00417AB8 (*fp->p))
		{
		  n++, fp->r--, fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    break;
		}
	      nread += n;
	    }
	  else
	    {
	      strStart = strPtr = va_arg_00417AB8 (ap, char *);
	      while (!isspace_00417AB8 (*fp->p))
		{
		  fp->r--;
		  *strPtr++ = *fp->p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty_00417AB8)
		    break;
		}
	      *strPtr = 0;
	      nread += strPtr - strStart;
	      nassigned++;
	    }
	  continue;

	case CT_INT:
	  /* scan an integer as if by strtol/strtoul */
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
	  flags |= SIGNOK | NDIGITS | NZDIGITS;
	  for (strPtr = buf; width; width--)
	    {
	      c = *fp->p;
	      /*
	       * Switch on the character; `goto ok' if we
	       * accept it as a part of number.
	       */
	      switch (c)
		{
		case '0':
		  if (base == 0)
		    {
		      base = 8;
		      flags |= PFXOK;
		    }
		  if (flags & NZDIGITS)
		    flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
		  else
		    flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* 1 through 7 always legal */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		  base = D_00499D90[base];
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* digits 8 and 9 ok iff decimal or hex */
		case '8':
		case '9':
		  base = D_00499D90[base];
		  if (base <= 8)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* letters ok iff hex */
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		  /* no need to fix base here */
		  if (base <= 10)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* sign ok only as first character */
		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto ok;
		    }
		  break;

		  /* x ok iff flag still set & 2nd char */
		case 'x':
		case 'X':
		  if (flags & PFXOK && strPtr == buf + 1)
		    {
		      base = 16;/* if %i */
		      flags &= ~PFXOK;
		      goto ok;
		    }
		  break;
		}

	      /*
	       * If we got here, c is not a legal character
	       * for a number.  Stop accumulating digits.
	       */
	      break;
	    ok:
	      /*
	       * c is legal: store it and look at the next.
	       */
	      *strPtr++ = c;
	      if (--fp->r > 0)
		fp->p++;
	      else
	      if (__srefill_impl (fp))
		break;		/* EOF */
	    }
	  /*
	   * If we had only a sign, it is no good; push back the sign.
	   * If the number ends in `x', it was [sign] '0' 'x', so push back
	   * the x and treat it as [sign] '0'.
	   */
	  if (flags & NDIGITS)
	    {
	      if (strPtr > buf)
		(void) ungetc_7AB8 (*(u_char_00417AB8 *)-- strPtr, fp);
	      goto match_failure;
	    }
	  c = ((u_char_00417AB8 *) strPtr)[-1];
	  if (c == 'x' || c == 'X')
	    {
	      --strPtr;
	      /*(void)*/ ungetc_7AB8 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      u_long_00417AB8 res;

	      *strPtr = 0;
	      res = (*ccfn) (buf, (char **) 0, base);
	      if (flags & POINTER)
		*(va_arg_00417AB8 (ap, void **)) = (void *) (unsigned int) res;
	      else if (flags & SHORT)
		{
		  sp = va_arg_00417AB8 (ap, short *);
		  *sp = res;
		}
	      else if (flags & LONG)
		{
		  lp = va_arg_00417AB8 (ap, long *);
		  *lp = res;
		}
	      else
		{
		  ip = va_arg_00417AB8 (ap, int *);
		  *ip = res;
		}
	      nassigned++;
	    }
	  nread += strPtr - buf;
	  break;

	case CT_FLOAT:
	  /* scan a floating point number as if by strtod */
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
	  flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
	  for (strPtr = buf; width; width--)
	    {
	      c = *fp->p;
	      /*
	       * This code mimicks the integer conversion
	       * code, but is much simpler.
	       */
	      switch (c)
		{

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		  flags &= ~(SIGNOK | NDIGITS);
		  goto fok;

		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto fok;
		    }
		  break;
		case '.':
		  if (flags & DPTOK)
		    {
		      flags &= ~(SIGNOK | DPTOK);
		      goto fok;
		    }
		  break;
		case 'e':
		case 'E':
		  /* no exponent without some digits */
		  if ((flags & (NDIGITS | EXPOK)) == EXPOK)
		    {
		      flags =
			(flags & ~(EXPOK | DPTOK)) |
			SIGNOK | NDIGITS;
		      goto fok;
		    }
		  break;
		}
	      break;
	    fok:
	      *strPtr++ = c;
	      if (--fp->r > 0)
		fp->p++;
	      else
	      if (__srefill_impl (fp))
		break;		/* EOF */
	    }
	  /*
	   * If no digits, might be missing exponent digits
	   * (just give back the exponent) or might be missing
	   * regular digits, but had sign and/or decimal point.
	   */
	  if (flags & NDIGITS)
	    {
	      if (flags & EXPOK)
		{
		  /* no digits at all */
		  while (strPtr > buf)
		    ungetc_7AB8 (*(u_char_00417AB8 *)-- strPtr, fp);
		  goto match_failure;
		}
	      /* just a bad exponent (e and maybe sign) */
	      c = *(u_char_00417AB8 *)-- strPtr;
	      if (c != 'e' && c != 'E')
		{
		  (void) ungetc_7AB8 (c, fp);	/* sign */
		  c = *(u_char_00417AB8 *)-- strPtr;
		}
	      (void) ungetc_7AB8 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      double res;

	      *strPtr = 0;
	      res = g7AB8_atof (buf);
	      if (flags & LONG)
		{
		  dp = va_arg_00417AB8 (ap, double *);
		  *dp = res;
		}
	      else if (flags & LONGDBL)
		{
		  ldp = va_arg_00417AB8 (ap, double *);
		  *ldp = res;
		}
	      else
		{
		  flp = va_arg_00417AB8 (ap, float *);
		  *flp = res;
		}
	      nassigned++;
	    }
	  nread += strPtr - buf;
	  break;
	}
    }
input_failure:
  return nassigned ? nassigned : -1;
match_failure:
  return nassigned;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sccl);
#ifdef SKIP_ASM
/**
 * Builds the character table for a scanf %[...] set and returns the format position after the closing bracket.
 *
 * A leading ^ inverts the set. All 256 entries start as excluded; then each listed character is marked, a - between two
 * characters marks the range (a - before ] or before a smaller character is a literal -), and the first ] after at
 * least one character ends the set. A format that ends early returns the position of its NUL.
 *
 * @param tab  the 256-entry table (nonzero = character in the set)
 * @param fmt  the format just after the [
 * @return the format position after the set
 */
char *__sccl(char *tab, char *fmt) {
    int curChar, n, mark;
    curChar = *fmt++;
    if (curChar == '^') {
        mark = 1;
        curChar = *fmt++;
    } else
        mark = 0;
    for (n = 0; n < 256; n++)
        tab[n] = mark;
    if (curChar == 0)
        return fmt - 1;
    mark = 1 - mark;
    for (;;) {
        tab[curChar] = mark;
    doswitch:
        n = *fmt++;
        switch (n) {
        case 0:
            return fmt - 1;
        case '-':
            n = *fmt;
            if (n == ']' || n < curChar) {
                curChar = '-';
                break;
            }
            fmt++;
            do {
                tab[++curChar] = mark;
            } while (curChar < n);
            goto doswitch;
            break;
        case ']':
            return fmt;
        default:
            curChar = n;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", vsprintf);
#ifdef SKIP_ASM
struct Fvsprintf {
    char *p;
    int r;
    int w;
    short flags, file;
    char *base;
    int size;
    int pad[0x54 / 4 - 6];
    int ent;
};
extern int D_00454944;
extern int vfprintf(int, int, char *);
/**
 * Formats a variable argument list into a caller buffer with no length limit.
 *
 * Builds a string stream on the stack (flags 0x208, sizes 0x7FFFFFFF, reentrancy pointer D_00454944), calls
 * vfprintf and terminates the text. Declared void, but $v0 still holds vfprintf's character count. Callers
 * include cBXString_vprintf, cFont_vtextf and DEBUG_trace.
 *
 * @param buf  the output buffer
 * @param fmt  the format string
 * @param ap   the argument pointer
 */
void vsprintf(char *buf, int fmt, char *ap) {
    struct Fvsprintf strStream;
    strStream.p = buf;
    strStream.w = 0x7FFFFFFF;
    strStream.flags = 520;
    strStream.base = buf;
    strStream.size = 0x7FFFFFFF;
    strStream.ent = D_00454944;
    vfprintf((int)&strStream, fmt, ap);
    *strStream.p = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", vsnprintf);
#ifdef SKIP_ASM
struct F18720 {
    char *p;
    int r;
    unsigned w;
    short flags, file;
    char *base;
    unsigned size;
    int pad[0x54 / 4 - 6];
    int ent;
};
extern int D_00454944;
/**
 * Formats a variable argument list into a buffer of limited size and returns the count, or -1 when it did not fit.
 *
 * Builds a string stream on the stack with write and buffer size n (flags 0x208, reentrancy pointer D_00454944), calls
 * vfprintf and stores a NUL at the final position.
 *
 * Likely bug: the size given to the stream is n, not n - 1 as in newlib, so a full result writes its NUL to buf[n], one
 * byte past the buffer; the -1 for a result longer than n is a Sony change.
 *
 * @param buf  the output buffer
 * @param n    the buffer size
 * @param fmt  the format string
 * @param ap   the argument pointer
 * @return the character count, or -1 when it is larger than n
 */
int vsnprintf(char *buf, unsigned n, int fmt, char *ap) {
    struct F18720 strStream;
    int charCount;
    strStream.p = buf;
    strStream.w = n;
    strStream.flags = 520;
    strStream.base = buf;
    strStream.size = n;
    strStream.ent = D_00454944;
    charCount = vfprintf((int)&strStream, fmt, ap);
    if (n < (unsigned)charCount) charCount = -1;
    *strStream.p = 0;
    return charCount;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418790);
#ifdef SKIP_ASM
void func_001001C8(int) __attribute__((noreturn));
/**
 * Ends the program with exit status 1 (never returns).
 *
 * Calls func_001001C8(1), which does not return.
 */
void func_00418790(void) {
    func_001001C8(1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", atof);
#ifdef SKIP_ASM
extern void strtod(int, int);
/**
 * Converts a decimal string to a double by calling strtod (str, 0).
 *
 * Declared void in C; the double comes back from strtod in the return registers untouched.
 *
 * @param str  the string
 */
void atof(int str) {
    strtod(str, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", atoi);
#ifdef SKIP_ASM
extern long strtol(int, int, int);
/**
 * Converts a decimal string to an int by calling strtol (str, 0, 10).
 *
 * @param str  the string
 * @return the value, truncated to int
 */
int atoi(int str) {
    return strtol(str, 0, 10);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", bsearch);
#ifdef SKIP_ASM
/**
 * Binary-searches a sorted array and returns the matching element, or null.
 *
 * Halves [lo, hi) with the comparison function called as compar(key, element): negative searches lower, positive
 * higher, zero returns the element. An empty array or a zero element size returns null. Callers include
 * getstringidbyindex.
 *
 * @param key     the key passed to the comparison
 * @param base    the array
 * @param nmemb   the element count
 * @param size    the element size
 * @param compar  the comparison function
 * @return the matching element, or null
 */
void *bsearch(void *key, char *base, unsigned nmemb, unsigned size, int (*compar)(void *, void *)) {
    unsigned lo = 0, hi = nmemb, mid;
    char *elem;
    int cmpResult;
    if (nmemb == 0 || size == 0)
        return 0;
    while (lo < hi) {
        mid = (lo + hi) >> 1;
        elem = base + mid * size;
        cmpResult = compar(key, elem);
        if (cmpResult < 0)
            hi = mid;
        else if (cmpResult > 0)
            lo = mid + 1;
        else
            return elem;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_004188C0);
#ifdef SKIP_ASM
extern int D_00454944;
extern void __malloc_lock(int);
extern void __malloc_unlock(int);
extern int _malloc_r(int, int);
/**
 * Allocates memory from the C heap under the heap lock and returns it, or null.
 *
 * Takes the lock with __malloc_lock, allocates with _malloc_r using D_00454944, and releases the lock with
 * __malloc_unlock.
 *
 * @param byteCount  bytes wanted
 * @return the block, or null
 */
int func_004188C0(int byteCount) {
    int block;
    __malloc_lock(D_00454944);
    block = _malloc_r(D_00454944, byteCount);
    __malloc_unlock(D_00454944);
    return block;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00418910);
#ifdef SKIP_ASM
extern int D_00454944;
extern void __malloc_lock(int);
extern void __malloc_unlock(int);
extern void _free_r(int, int);
/**
 * Returns a block to the C heap under the heap lock.
 *
 * Takes the lock with __malloc_lock, releases the block with _free_r using D_00454944, and releases the lock with
 * __malloc_unlock.
 *
 * @param block  the block, or null
 */
void func_00418910(int block) {
    __malloc_lock(D_00454944);
    _free_r(D_00454944, block);
    __malloc_unlock(D_00454944);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _mbtowc_r);
#ifdef SKIP_ASM
/* newlib stdlib/mbtowc_r.c (_mbtowc_r) with stdlib/mbctype.h; neither file carries its own notice (newlib's
 * default: Copyright (c) 1994-2000 Red Hat, Inc. Permission to use, copy, modify and distribute this software is
 * freely granted, provided that this notice is preserved.) */
typedef unsigned short wchar_t_8958;

#define ESC_CHAR_8958 0x1b
#define _issjis1_8958(c)    (((c) >= 0x81 && (c) <= 0x9f) || ((c) >= 0xe0 && (c) <= 0xef))
#define _issjis2_8958(c)    (((c) >= 0x40 && (c) <= 0x7e) || ((c) >= 0x80 && (c) <= 0xfc))
#define _iseucjp_8958(c)    ((c) >= 0xa1 && (c) <= 0xfe)
#define _isjis_8958(c)      ((c) >= 0x21 && (c) <= 0x7e)

typedef enum { ESCAPE, DOLLAR, BRACKET, AT, B, J,
               NUL, JIS_CHAR, OTHER, JIS_C_NUM } JIS_CHAR_TYPE_8958;
typedef enum { ASCII, A_ESC, A_ESC_DL, JIS, JIS_1, JIS_2, J_ESC, J_ESC_BR,
               J2_ESC, J2_ESC_BR, DONE, INV, JIS_S_NUM } JIS_STATE_8958;
typedef enum { COPY_A, COPY_J, COPY_J2, MAKE_A, MAKE_J, NOOP, EMPTY, ERROR } JIS_ACTION_8958;

/* the file's static state/action tables, in .data */
extern JIS_STATE_8958 JIS_state_table_8958[JIS_S_NUM][JIS_C_NUM] __asm__("D_00454948");
extern JIS_ACTION_8958 JIS_action_table_8958[JIS_S_NUM][JIS_C_NUM] __asm__("D_00454AF8");
extern unsigned int strlen_8958(const char *) __asm__("strlen");
extern int strcmp_8958(const char *, const char *) __asm__("func_004165A8");

/**
 * Converts one multibyte character to a wide character and returns its byte length (0 for NUL, -1 when invalid).
 *
 * With no locale or a one-letter one ("C") a byte is a character. "C-SJIS" and "C-EUCJP" read one or two bytes by their
 * lead-byte ranges. "C-JIS" runs the escape-sequence state machine of the tables D_00454948 (next state) and D_00454AF8
 * (action), keeping the shift state in *state. Compares the locale names with func_004165A8.
 *
 * @param reent  the reentrancy struct (its current locale, +0x34)
 * @param pwc    receives the wide character, or null
 * @param src    the multibyte bytes, or null to reset the state
 * @param n      bytes available
 * @param state  the shift state
 * @return the bytes used, 0 for a NUL character, or -1 for an invalid or incomplete sequence
 */
int
_mbtowc_r (struct _reent *reent, wchar_t_8958 *pwc, const char *src, unsigned int n, int *state)
{
  wchar_t_8958 scratchWc;
  unsigned char *srcBytes = (unsigned char *)src;

  if (pwc == 0)
    pwc = &scratchWc;

  if (src != 0 && n == 0)
    return -1;

  if (reent->_current_locale == 0 ||
      (strlen_8958 (reent->_current_locale) <= 1))
    { /* fall-through */ }
  else if (!strcmp_8958 (reent->_current_locale, "C-SJIS"))
    {
      int char1 = *srcBytes;
      if (src == 0)
        return 0;  /* not state-dependent */
      if (_issjis1_8958 (char1))
        {
          int char2 = srcBytes[1];
          if (n <= 1)
            return -1;
          if (_issjis2_8958 (char2))
            {
              *pwc = (((wchar_t_8958)*srcBytes) << 8) + (wchar_t_8958)(*(srcBytes+1));
              return 2;
            }
          else
            return -1;
        }
    }
  else if (!strcmp_8958 (reent->_current_locale, "C-EUCJP"))
    {
      int char1 = *srcBytes;
      if (src == 0)
        return 0;  /* not state-dependent */
      if (_iseucjp_8958 (char1))
        {
          int char2 = srcBytes[1];
          if (n <= 1)
            return -1;
          if (_iseucjp_8958 (char2))
            {
              *pwc = (((wchar_t_8958)*srcBytes) << 8) + (wchar_t_8958)(*(srcBytes+1));
              return 2;
            }
          else
            return -1;
        }
    }
  else if (!strcmp_8958 (reent->_current_locale, "C-JIS"))
    {
      JIS_STATE_8958 curr_state;
      JIS_ACTION_8958 action;
      JIS_CHAR_TYPE_8958 ch;
      unsigned char *ptr;
      int i, curr_ch;

      if (src == 0)
        {
          *state = 0;
          return 1;  /* state-dependent */
        }

      curr_state = (*state == 0 ? ASCII : JIS);
      ptr = srcBytes;

      for (i = 0; i < n; ++i)
        {
          curr_ch = srcBytes[i];
          switch (curr_ch)
            {
	    case ESC_CHAR_8958:
              ch = ESCAPE;
              break;
	    case '$':
              ch = DOLLAR;
              break;
            case '@':
              ch = AT;
              break;
            case '(':
	      ch = BRACKET;
              break;
            case 'B':
              ch = B;
              break;
            case 'J':
              ch = J;
              break;
            case '\0':
              ch = NUL;
              break;
            default:
              if (_isjis_8958 (curr_ch))
                ch = JIS_CHAR;
              else
                ch = OTHER;
	    }

          action = JIS_action_table_8958[curr_state][ch];
          curr_state = JIS_state_table_8958[curr_state][ch];

          switch (action)
            {
            case NOOP:
              break;
            case EMPTY:
              *state = 0;
              *pwc = (wchar_t_8958)0;
              return i;
            case COPY_A:
	      *state = 0;
              *pwc = (wchar_t_8958)*ptr;
              return (i + 1);
            case COPY_J:
              *state = 0;
              *pwc = (((wchar_t_8958)*ptr) << 8) + (wchar_t_8958)(*(ptr+1));
              return (i + 1);
            case COPY_J2:
              *state = 1;
              *pwc = (((wchar_t_8958)*ptr) << 8) + (wchar_t_8958)(*(ptr+1));
              return (ptr - srcBytes) + 2;
            case MAKE_A:
            case MAKE_J:
              ptr = (char *)(srcBytes + i + 1);
              break;
            case ERROR:
            default:
              return -1;
            }

        }

      return -1;  /* n < bytes needed */
    }

  /* otherwise this must be the "C" locale or unknown locale */
  if (src == 0)
    return 0;  /* not state-dependent */

  *pwc = (wchar_t_8958)*srcBytes;

  if (*srcBytes == '\0')
    return 0;

  return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __malloc_lock);
#ifdef SKIP_ASM
extern int GetThreadId(void);
extern void WaitSema(int);
extern int D_00454CA8;
extern int D_00454CAC;
extern int D_00455248;
/**
 * Takes the recursive C heap lock for the calling thread.
 *
 * When the current thread (GetThreadId) already owns it (D_00454CA8) the count D_00454CAC goes up; otherwise it waits
 * on the semaphore D_00455248, records the owner and counts 1.
 *
 * @param reent  the reentrancy struct (ignored)
 */
void __malloc_lock(int reent) {
    int id = GetThreadId();
    if (D_00454CA8 == id) {
        D_00454CAC++;
    } else {
        WaitSema(D_00455248);
        D_00454CA8 = id;
        D_00454CAC++;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __malloc_unlock);
#ifdef SKIP_ASM
extern void SignalSema(int);
extern int D_00454CA8;
extern int D_00454CAC;
extern int D_00455248;
/**
 * Releases the recursive C heap lock taken by __malloc_lock.
 *
 * Decrements the count D_00454CAC; when it reaches 0 it clears the owner D_00454CA8 to -1 and signals the semaphore
 * D_00455248.
 *
 * @param reent  the reentrancy struct (ignored)
 */
void __malloc_unlock(int reent) {
    if (--D_00454CAC == 0) {
        D_00454CA8 = -1;
        SignalSema(D_00455248);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", fcvt);
#ifdef SKIP_ASM
extern void fcvtbuf(int, int, int, int, int);
/**
 * Converts a double to fixed-point decimal digits in the shared conversion buffer.
 *
 * Calls fcvtbuf with a null buffer. The double arrives in the first (64-bit) argument register although the C declares
 * an int.
 *
 * @param value      the double to convert
 * @param numDigits  digits after the point
 * @param decptOut   receives the decimal point position
 * @param signOut    receives nonzero for a negative value
 */
void fcvt(int value, int numDigits, int decptOut, int signOut) {
    fcvtbuf(value, numDigits, decptOut, signOut, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", fcvtf);
#ifdef SKIP_ASM
extern int fptodp(void);
extern void fcvt(int, int, int, int);
/**
 * Converts a float to fixed-point decimal digits in the shared conversion buffer (fcvt of the widened value).
 *
 * The float arrives in the float argument register, which the C does not declare: fptodp() picks it up there, and the
 * double result goes to fcvt.
 *
 * @param numDigits  digits after the point
 * @param decptOut   receives the decimal point position
 * @param signOut    receives nonzero for a negative value
 */
void fcvtf(int numDigits, int decptOut, int signOut) {
    fcvt(fptodp(), numDigits, decptOut, signOut);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", gcvtf);
#ifdef SKIP_ASM
extern int fptodp(void);
extern int gcvt(int, int, int);
/**
 * Formats a float %g-style into a caller buffer (gcvt of the widened value).
 *
 * The float arrives in the float argument register, which the C does not declare: fptodp() picks it up there, and the
 * double goes to gcvt.
 *
 * @param numDigits  significant digits
 * @param buf        the output buffer
 */
void gcvtf(int numDigits, int buf) {
    gcvt(fptodp(), numDigits, buf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", ecvt);
#ifdef SKIP_ASM
extern void ecvtbuf(int, int, int, int, int);
/**
 * Converts a double to ndigit significant decimal digits in the shared conversion buffer.
 *
 * Calls ecvtbuf with a null buffer. The double arrives in the first (64-bit) argument register although the C declares
 * an int.
 *
 * @param value      the double to convert
 * @param numDigits  number of digits
 * @param decptOut   receives the decimal point position
 * @param signOut    receives nonzero for a negative value
 */
void ecvt(int value, int numDigits, int decptOut, int signOut) {
    ecvtbuf(value, numDigits, decptOut, signOut, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", ecvtf);
#ifdef SKIP_ASM
extern int fptodp(void);
extern void ecvt(int, int, int, int);
/**
 * Converts a float to significant decimal digits in the shared conversion buffer (ecvt of the widened value).
 *
 * The float arrives in the float argument register, which the C does not declare: fptodp() picks it up there, and the
 * double result goes to ecvt.
 *
 * @param numDigits  number of digits
 * @param decptOut   receives the decimal point position
 * @param signOut    receives nonzero for a negative value
 */
void ecvtf(int numDigits, int decptOut, int signOut) {
    ecvt(fptodp(), numDigits, decptOut, signOut);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", gcvt);
#ifdef SKIP_ASM
extern int D_00454944;
extern int dpcmp(int, int);
extern int _gcvt(int, int, int, char *, int, int);
/**
 * Formats a double %g-style into a caller buffer, with a leading minus for negative values.
 *
 * Writes '-' and drops one digit of precision when dpcmp says the value is below zero, then formats with _gcvt
 * (reentrancy struct D_00454944, type 'g', no forced point).
 *
 * @param value     the double, passed in the first (64-bit) argument register
 * @param prec  significant digits
 * @param buf_  the output buffer
 * @return the buffer, or 0 if _gcvt failed
 */
int gcvt(int value, int prec, int buf_) {
    char *buf = (char *)buf_;
    char *cursor = buf;
    if (dpcmp(value, 0) < 0) {
        *cursor++ = '-';
        prec--;
    }
    return _gcvt(D_00454944, value, prec, cursor, 'g', 0) ? (int)buf : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", qsort);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/qsort.c (qsort), verbatim apart from the K&R wrappers:
 *
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
static inline char *med3_00418EF8(char *, char *, char *, int (*)());
static inline void swapqsort(char *, char *, int, int);

#define min_00418EF8(a, b)	(a) < (b) ? a : b

#define swapcode_00418EF8(TYPE, parmi, parmj, n) { 		\
	long i = (n) / sizeof (TYPE); 			\
	register TYPE *pi = (TYPE *) (parmi); 		\
	register TYPE *pj = (TYPE *) (parmj); 		\
	do { 						\
		register TYPE	t = *pi;		\
		*pi++ = *pj;				\
		*pj++ = t;				\
        } while (--i > 0);				\
}

#define SWAPINIT_00418EF8(a, es) swaptype = ((char *)a - (char *)0) % sizeof(long) || \
	es % sizeof(long) ? 2 : es == sizeof(long)? 0 : 1;

static inline void
swapqsort(char *a, char *b, int n, int swaptype)
{
	if(swaptype <= 1) 
		swapcode_00418EF8(long, a, b, n)
	else
		swapcode_00418EF8(char, a, b, n)
}

#define swap_00418EF8(a, b)					\
	if (swaptype == 0) {				\
		long t = *(long *)(a);			\
		*(long *)(a) = *(long *)(b);		\
		*(long *)(b) = t;			\
	} else						\
		swapqsort(a, b, es, swaptype)

#define vecswap_00418EF8(a, b, n) 	if ((n) > 0) swapqsort(a, b, n, swaptype)

static inline char *
med3_00418EF8(char *a, char *b, char *c, int (*cmp)())
{
	return cmp(a, b) < 0 ?
	       (cmp(b, c) < 0 ? b : (cmp(a, c) < 0 ? c : a ))
              :(cmp(b, c) > 0 ? b : (cmp(a, c) < 0 ? a : c ));
}

/**
 * Sorts an array in place with the comparison function (Bentley-McIlroy quicksort).
 *
 * Bentley-McIlroy quicksort as in newlib: insertion sort below 7 elements, median of three (ninther above 40) as the
 * pivot, a three-way partition that gathers equal keys at the ends, recursion on the left part and a loop on the right
 * one. When a pass makes no swaps it finishes with insertion sort. Elements are swapped as longs when the base and size
 * allow it.
 *
 * @param a    the array
 * @param n    the element count
 * @param es   the element size
 * @param cmp  the comparison function
 */
void
qsort(void *a, unsigned int n, unsigned int es, int (*cmp)())
{
	char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
	int stride, r, swaptype, swap_cnt;

loop:	SWAPINIT_00418EF8(a, es);
	swap_cnt = 0;
	if (n < 7) {
		for (pm = (char *) a + es; pm < (char *) a + n * es; pm += es)
			for (pl = pm; pl > (char *) a && cmp(pl - es, pl) > 0;
			     pl -= es)
				swap_00418EF8(pl, pl - es);
		return;
	}
	pm = (char *) a + (n / 2) * es;
	if (n > 7) {
		pl = a;
		pn = (char *) a + (n - 1) * es;
		if (n > 40) {
			stride = (n / 8) * es;
			pl = med3_00418EF8(pl, pl + stride, pl + 2 * stride, cmp);
			pm = med3_00418EF8(pm - stride, pm, pm + stride, cmp);
			pn = med3_00418EF8(pn - 2 * stride, pn - stride, pn, cmp);
		}
		pm = med3_00418EF8(pl, pm, pn, cmp);
	}
	swap_00418EF8(a, pm);
	pa = pb = (char *) a + es;

	pc = pd = (char *) a + (n - 1) * es;
	for (;;) {
		while (pb <= pc && (r = cmp(pb, a)) <= 0) {
			if (r == 0) {
				swap_cnt = 1;
				swap_00418EF8(pa, pb);
				pa += es;
			}
			pb += es;
		}
		while (pb <= pc && (r = cmp(pc, a)) >= 0) {
			if (r == 0) {
				swap_cnt = 1;
				swap_00418EF8(pc, pd);
				pd -= es;
			}
			pc -= es;
		}
		if (pb > pc)
			break;
		swap_00418EF8(pb, pc);
		swap_cnt = 1;
		pb += es;
		pc -= es;
	}
	if (swap_cnt == 0) {  /* Switch to insertion sort */
		for (pm = (char *) a + es; pm < (char *) a + n * es; pm += es)
			for (pl = pm; pl > (char *) a && cmp(pl - es, pl) > 0; 
			     pl -= es)
				swap_00418EF8(pl, pl - es);
		return;
	}

	pn = (char *) a + n * es;
	r = min_00418EF8(pa - (char *)a, pb - pa);
	vecswap_00418EF8(a, pb - r, r);
	r = min_00418EF8(pd - pc, pn - pd - es);
	vecswap_00418EF8(pb, pn - r, r);
	if ((r = pb - pa) > es)
		qsort(a, r / es, es, cmp);
	if ((r = pd - pc) > es) { 
		/* Iterate rather than recurse to save stack space */
		a = pn - r;
		n = r / es;
		goto loop;
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _strtod_r);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/strtod.c (_strtod_r) with the mprec.h macros it uses, verbatim (IEEE_8087, Pack_32,
 * no Sudden_Underflow / RND_PRODQUOT / Check_FLT_ROUNDS; FLT_ROUNDS is 1):
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
struct _Bigint_004198D8
{
  struct _Bigint_004198D8 *_next;
  int _k, _maxwds, _sign, _wds;
  unsigned int _x[1];
};
typedef struct _Bigint_004198D8 _Bigint_004198D8;
struct _reent_004198D8 { int _errno; };
union double_union_004198D8
{
  double d;
  unsigned int i[2];
};
union __dmath_004198D8
{
  unsigned int i[2];
  double d;
};
#define word0_004198D8(x) (x.i[1])
#define word1_004198D8(x) (x.i[0])
#define Exp_msk1_004198D8    ((unsigned int)0x100000L)
#define Exp_mask_004198D8  ((unsigned int)0x7ff00000L)
#define P_004198D8 53
#define Bias_004198D8 1023
#define Emin_004198D8 (-1022)
#define Frac_mask1_004198D8 ((unsigned int)0xfffffL)
#define Ten_pmax_004198D8 22
#define Bndry_mask_004198D8  ((unsigned int)0xfffffL)
#define Bndry_mask1_004198D8 ((unsigned int)0xfffffL)
#define LSB_004198D8 1
#define Log2P_004198D8 1
#define Tiny0_004198D8 0
#define Tiny1_004198D8 1
#define rounded_product_004198D8(a,b) a *= b
#define rounded_quotient_004198D8(a,b) a /= b
#define Big0_004198D8 (Frac_mask1_004198D8 | Exp_msk1_004198D8*(1024+Bias_004198D8-1))
#define Big1_004198D8 ((unsigned int)0xffffffffL)
#define n_bigtens_004198D8 5
#define DBL_DIG_004198D8 15
#define DBL_MAX_10_EXP_004198D8 308
#define DBL_MAX_EXP_004198D8 1024
#define FLT_ROUNDS_004198D8 1
#define FLT_RADIX_004198D8 2
#define ERANGE_004198D8 34
#define HUGE_VAL_004198D8 (infinity_004198D8[0].d)
#define Bcopy_004198D8(x,y) g98D8_memcpy((char *)&x->_sign, (char *)&y->_sign, y->_wds*sizeof(int) + 2*sizeof(int))
extern const union __dmath_004198D8 infinity_004198D8[] __asm__("D_0049A898");
extern const double tens_004198D8[] __asm__("D_0049A8F8");
extern const double bigtens_004198D8[] __asm__("D_0049A9C0");
extern const double tinytens_004198D8[] __asm__("D_0049A9E8");
extern void *g98D8_memcpy(void *, void *, int) __asm__("func_0041605C");
extern _Bigint_004198D8 *g98D8_Balloc(struct _reent_004198D8 *, int) __asm__("_Balloc");
extern void g98D8_Bfree(struct _reent_004198D8 *, _Bigint_004198D8 *) __asm__("_Bfree");
extern _Bigint_004198D8 *g98D8_s2b(struct _reent_004198D8 *, const char *, int, int, unsigned int) __asm__("_s2b");
extern _Bigint_004198D8 *g98D8_i2b(struct _reent_004198D8 *, int) __asm__("_i2b");
extern _Bigint_004198D8 *g98D8_mult(struct _reent_004198D8 *, _Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("_multiply");
extern _Bigint_004198D8 *g98D8_pow5mult(struct _reent_004198D8 *, _Bigint_004198D8 *, int) __asm__("_pow5mult");
extern _Bigint_004198D8 *g98D8_lshift(struct _reent_004198D8 *, _Bigint_004198D8 *, int) __asm__("_lshift");
extern int g98D8_cmp(_Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("__mcmp");
extern _Bigint_004198D8 *g98D8_diff(struct _reent_004198D8 *, _Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("__mdiff");
extern double g98D8_ulp(double) __asm__("_ulp");
extern _Bigint_004198D8 *g98D8_d2b(struct _reent_004198D8 *, double, int *, int *) __asm__("_d2b");
extern double g98D8_ratio(_Bigint_004198D8 *, _Bigint_004198D8 *) __asm__("_ratio");

/* the unit declares this as void _strtod_r(int, int, int) (strtod, strtod) */
double _strtod_r_(struct _reent_004198D8 *, const char *, char **) __asm__("_strtod_r");
/**
 * Parses a decimal floating-point number from a string with correct rounding (David Gay's strtod, newlib 1.9).
 *
 * Skips leading spaces, takes an optional sign, digits with an optional point and an optional exponent, and computes
 * the nearest double: exactly with doubles when the digits and power of ten allow, else by Bigint correction loops
 * (_s2b s2b, __mdiff diff and the other Bigint helpers). Overflow returns HUGE_VAL and underflow 0, both
 * setting the reentrancy errno to 34 (ERANGE). With no number, *se gets the input pointer and the result is 0.
 *
 * @param ptr    the reentrancy struct (errno at +0)
 * @param input  the string
 * @param se     receives the first character after the number, or null
 * @return the parsed value
 */
double _strtod_r_(struct _reent_004198D8 *ptr, const char *input, char **se)
{
  int bb2, bb5, bbe, bd2, bd5, bbbits, bs2, curChar, dsign, scaleExp, esign, i, j,
    k, nd, nd0, nf, nz, nz0, sign;
  long e;
  const char *cursor, *digitStart, *expStart;
  double aadj, aadj1, adj;
  long L;
  unsigned long z;
  unsigned int y;
  union double_union_004198D8 rv, rv0;

  _Bigint_004198D8 *bb, *bb1, *bd, *bd0, *bs, *delta;
  sign = nz0 = nz = 0;
  rv.d = 0.;
  for (cursor = input;; cursor++)
    switch (*cursor)
      {
      case '-':
	sign = 1;
	/* no break */
      case '+':
	if (*++cursor)
	  goto break2;
	/* no break */
      case 0:
	cursor = input;
	goto ret;
      case '\t':
      case '\n':
      case '\v':
      case '\f':
      case '\r':
      case ' ':
	continue;
      default:
	goto break2;
      }
break2:
  if (*cursor == '0')
    {
      nz0 = 1;
      while (*++cursor == '0');
      if (!*cursor)
	goto ret;
    }
  digitStart = cursor;
  y = z = 0;
  for (nd = nf = 0; (curChar = *cursor) >= '0' && curChar <= '9'; nd++, cursor++)
    if (nd < 9)
      y = 10 * y + curChar - '0';
    else if (nd < 16)
      z = 10 * z + curChar - '0';
  nd0 = nd;
  if (curChar == '.')
    {
      curChar = *++cursor;
      if (!nd)
	{
	  for (; curChar == '0'; curChar = *++cursor)
	    nz++;
	  if (curChar > '0' && curChar <= '9')
	    {
	      digitStart = cursor;
	      nf += nz;
	      nz = 0;
	      goto have_dig;
	    }
	  goto dig_done;
	}
      for (; curChar >= '0' && curChar <= '9'; curChar = *++cursor)
	{
	have_dig:
	  nz++;
	  if (curChar -= '0')
	    {
	      nf += nz;
	      for (i = 1; i < nz; i++)
		if (nd++ < 9)
		  y *= 10;
		else if (nd <= DBL_DIG_004198D8 + 1)
		  z *= 10;
	      if (nd++ < 9)
		y = 10 * y + curChar;
	      else if (nd <= DBL_DIG_004198D8 + 1)
		z = 10 * z + curChar;
	      nz = 0;
	    }
	}
    }
dig_done:
  e = 0;
  if (curChar == 'e' || curChar == 'E')
    {
      if (!nd && !nz && !nz0)
	{
	  cursor = input;
	  goto ret;
	}
      input = cursor;
      esign = 0;
      switch (curChar = *++cursor)
	{
	case '-':
	  esign = 1;
	case '+':
	  curChar = *++cursor;
	}
      if (curChar >= '0' && curChar <= '9')
	{
	  while (curChar == '0')
	    curChar = *++cursor;
	  if (curChar > '0' && curChar <= '9')
	    {
	      e = curChar - '0';
	      expStart = cursor;
	      while ((curChar = *++cursor) >= '0' && curChar <= '9')
		e = 10 * e + curChar - '0';
	      if (cursor - expStart > 8)
		/* Avoid confusion from exponents
		 * so large that e might overflow.
		 */
		e = 9999999L;
	      if (esign)
		e = -e;
	    }
	  else
	    e = 0;
	}
      else
	cursor = input;
    }
  if (!nd)
    {
      if (!nz && !nz0)
	cursor = input;
      goto ret;
    }
  scaleExp = e -= nf;

  /* Now we have nd0 digits, starting at s0, followed by a
   * decimal point, followed by nd-nd0 digits.  The number we're
   * after is the integer represented by those digits times
   * 10**e */

  if (!nd0)
    nd0 = nd;
  k = nd < DBL_DIG_004198D8 + 1 ? nd : DBL_DIG_004198D8 + 1;
  rv.d = y;
  if (k > 9)
    rv.d = tens_004198D8[k - 9] * rv.d + z;
  bd0 = 0;
  if (nd <= DBL_DIG_004198D8
      && FLT_ROUNDS_004198D8 == 1
    )
    {
      if (!e)
	goto ret;
      if (e > 0)
	{
	  if (e <= Ten_pmax_004198D8)
	    {
	      /* rv.d = */ rounded_product_004198D8 (rv.d, tens_004198D8[e]);
	      goto ret;
	    }
	  i = DBL_DIG_004198D8 - nd;
	  if (e <= Ten_pmax_004198D8 + i)
	    {
	      /* A fancier test would sometimes let us do
				 * this for larger i values.
				 */
	      e -= i;
	      rv.d *= tens_004198D8[i];
	      /* rv.d = */ rounded_product_004198D8 (rv.d, tens_004198D8[e]);
	      goto ret;
	    }
	}
      else if (e >= -Ten_pmax_004198D8)
	{
	  /* rv.d = */ rounded_quotient_004198D8 (rv.d, tens_004198D8[-e]);
	  goto ret;
	}
    }
  scaleExp += nd - k;

  /* Get starting approximation = rv.d * 10**e1 */

  if (scaleExp > 0)
    {
      if ((i = scaleExp & 15) != 0)
	rv.d *= tens_004198D8[i];
      if (scaleExp &= ~15)
	{
	  if (scaleExp > DBL_MAX_10_EXP_004198D8)
	    {
	    ovfl:
	      ptr->_errno = ERANGE_004198D8;
	      rv.d = HUGE_VAL_004198D8;
	      if (bd0)
		goto retfree;
	      goto ret;
	    }
	  if (scaleExp >>= 4)
	    {
	      for (j = 0; scaleExp > 1; j++, scaleExp >>= 1)
		if (scaleExp & 1)
		  rv.d *= bigtens_004198D8[j];
	      /* The last multiplication could overflow. */
	      word0_004198D8 (rv) -= P_004198D8 * Exp_msk1_004198D8;
	      rv.d *= bigtens_004198D8[j];
	      if ((z = word0_004198D8 (rv) & Exp_mask_004198D8)
		  > Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - P_004198D8))
		goto ovfl;
	      if (z > Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - 1 - P_004198D8))
		{
		  /* set to largest number */
		  /* (Can't trust DBL_MAX) */
		  word0_004198D8 (rv) = Big0_004198D8;
		  word1_004198D8 (rv) = Big1_004198D8;
		}
	      else
		word0_004198D8 (rv) += P_004198D8 * Exp_msk1_004198D8;
	    }

	}
    }
  else if (scaleExp < 0)
    {
      scaleExp = -scaleExp;
      if ((i = scaleExp & 15) != 0)
	rv.d /= tens_004198D8[i];
      if (scaleExp &= ~15)
	{
	  scaleExp >>= 4;
	  if (scaleExp >= 1 << n_bigtens_004198D8)
            goto undfl;
	  for (j = 0; scaleExp > 1; j++, scaleExp >>= 1)
	    if (scaleExp & 1)
	      rv.d *= tinytens_004198D8[j];
	  /* The last multiplication could underflow. */
	  rv0.d = rv.d;
	  rv.d *= tinytens_004198D8[j];
	  if (!rv.d)
	    {
	      rv.d = 2. * rv0.d;
	      rv.d *= tinytens_004198D8[j];
	      if (!rv.d)
		{
		undfl:
		  rv.d = 0.;
		  ptr->_errno = ERANGE_004198D8;
		  if (bd0)
		    goto retfree;
		  goto ret;
		}
	      word0_004198D8 (rv) = Tiny0_004198D8;
	      word1_004198D8 (rv) = Tiny1_004198D8;
	      /* The refinement below will clean
	       * this approximation up.
	       */
	    }
	}
    }

  /* Now the hard part -- adjusting rv to the correct value.*/

  /* Put digits into bd: true value = bd * 10^e */

  bd0 = g98D8_s2b (ptr, digitStart, nd0, nd, y);

  for (;;)
    {
      bd = g98D8_Balloc (ptr, bd0->_k);
      Bcopy_004198D8 (bd, bd0);
      bb = g98D8_d2b (ptr, rv.d, &bbe, &bbbits);	/* rv.d = bb * 2^bbe */
      bs = g98D8_i2b (ptr, 1);

      if (e >= 0)
	{
	  bb2 = bb5 = 0;
	  bd2 = bd5 = e;
	}
      else
	{
	  bb2 = bb5 = -e;
	  bd2 = bd5 = 0;
	}
      if (bbe >= 0)
	bb2 += bbe;
      else
	bd2 -= bbe;
      bs2 = bb2;
      i = bbe + bbbits - 1;	/* logb(rv.d) */
      if (i < Emin_004198D8)		/* denormal */
	j = bbe + (P_004198D8 - Emin_004198D8);
      else
	j = P_004198D8 + 1 - bbbits;
      bb2 += j;
      bd2 += j;
      i = bb2 < bd2 ? bb2 : bd2;
      if (i > bs2)
	i = bs2;
      if (i > 0)
	{
	  bb2 -= i;
	  bd2 -= i;
	  bs2 -= i;
	}
      if (bb5 > 0)
	{
	  bs = g98D8_pow5mult (ptr, bs, bb5);
	  bb1 = g98D8_mult (ptr, bs, bb);
	  g98D8_Bfree (ptr, bb);
	  bb = bb1;
	}
      if (bb2 > 0)
	bb = g98D8_lshift (ptr, bb, bb2);
      if (bd5 > 0)
	bd = g98D8_pow5mult (ptr, bd, bd5);
      if (bd2 > 0)
	bd = g98D8_lshift (ptr, bd, bd2);
      if (bs2 > 0)
	bs = g98D8_lshift (ptr, bs, bs2);
      delta = g98D8_diff (ptr, bb, bd);
      dsign = delta->_sign;
      delta->_sign = 0;
      i = g98D8_cmp (delta, bs);
      if (i < 0)
	{
	  /* Error is less than half an ulp -- check for
	   * special case of mantissa a power of two.
	   */
	  if (dsign || word1_004198D8 (rv) || word0_004198D8 (rv) & Bndry_mask_004198D8)
	    break;
	  delta = g98D8_lshift (ptr, delta, Log2P_004198D8);
	  if (g98D8_cmp (delta, bs) > 0)
	    goto drop_down;
	  break;
	}
      if (i == 0)
	{
	  /* exactly half-way between */
	  if (dsign)
	    {
	      if ((word0_004198D8 (rv) & Bndry_mask1_004198D8) == Bndry_mask1_004198D8
		  && word1_004198D8 (rv) == 0xffffffff)
		{
		  /*boundary case -- increment exponent*/
		  word0_004198D8 (rv) = (word0_004198D8 (rv) & Exp_mask_004198D8)
		    + Exp_msk1_004198D8
		    ;
		  word1_004198D8 (rv) = 0;
		  break;
		}
	    }
	  else if (!(word0_004198D8 (rv) & Bndry_mask_004198D8) && !word1_004198D8 (rv))
	    {
	    drop_down:
	      /* boundary case -- decrement exponent */
	      L = (word0_004198D8 (rv) & Exp_mask_004198D8) - Exp_msk1_004198D8;
	      word0_004198D8 (rv) = L | Bndry_mask1_004198D8;
	      word1_004198D8 (rv) = 0xffffffff;
	      break;
	    }
	  if (!(word1_004198D8 (rv) & LSB_004198D8))
	    break;
	  if (dsign)
	    rv.d += g98D8_ulp (rv.d);
	  else
	    {
	      rv.d -= g98D8_ulp (rv.d);
	      if (!rv.d)
		goto undfl;
	    }
	  break;
	}
      if ((aadj = g98D8_ratio (delta, bs)) <= 2.)
	{
	  if (dsign)
	    aadj = aadj1 = 1.;
	  else if (word1_004198D8 (rv) || word0_004198D8 (rv) & Bndry_mask_004198D8)
	    {
	      if (word1_004198D8 (rv) == Tiny1_004198D8 && !word0_004198D8 (rv))
		goto undfl;
	      aadj = 1.;
	      aadj1 = -1.;
	    }
	  else
	    {
	      /* special case -- power of FLT_RADIX to be */
	      /* rounded down... */

	      if (aadj < 2. / FLT_RADIX_004198D8)
		aadj = 1. / FLT_RADIX_004198D8;
	      else
		aadj *= 0.5;
	      aadj1 = -aadj;
	    }
	}
      else
	{
	  aadj *= 0.5;
	  aadj1 = dsign ? aadj : -aadj;
	  if (FLT_ROUNDS_004198D8 == 0)
	    aadj1 += 0.5;
	}
      y = word0_004198D8 (rv) & Exp_mask_004198D8;

      /* Check for overflow */

      if (y == Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - 1))
	{
	  rv0.d = rv.d;
	  word0_004198D8 (rv) -= P_004198D8 * Exp_msk1_004198D8;
	  adj = aadj1 * g98D8_ulp (rv.d);
	  rv.d += adj;
	  if ((word0_004198D8 (rv) & Exp_mask_004198D8) >=
	      Exp_msk1_004198D8 * (DBL_MAX_EXP_004198D8 + Bias_004198D8 - P_004198D8))
	    {
	      if (word0_004198D8 (rv0) == Big0_004198D8 && word1_004198D8 (rv0) == Big1_004198D8)
		goto ovfl;
	      word0_004198D8 (rv) = Big0_004198D8;
	      word1_004198D8 (rv) = Big1_004198D8;
	      goto cont;
	    }
	  else
	    word0_004198D8 (rv) += P_004198D8 * Exp_msk1_004198D8;
	}
      else
	{
	  /* Compute adj so that the IEEE rounding rules will
	   * correctly round rv.d + adj in some half-way cases.
	   * If rv.d * ulp(rv.d) is denormalized (i.e.,
	   * y <= (P-1)*Exp_msk1), we must adjust aadj to avoid
	   * trouble from bits lost to denormalization;
	   * example: 1.2e-307 .
	   */
	  if (y <= (P_004198D8 - 1) * Exp_msk1_004198D8 && aadj >= 1.)
	    {
	      aadj1 = (double) (int) (aadj + 0.5);
	      if (!dsign)
		aadj1 = -aadj1;
	    }
	  adj = aadj1 * g98D8_ulp (rv.d);
	  rv.d += adj;
	}
      z = word0_004198D8 (rv) & Exp_mask_004198D8;
      if (y == z)
	{
	  /* Can we stop now? */
	  L = aadj;
	  aadj -= L;
	  /* The tolerances below are conservative. */
	  if (dsign || word1_004198D8 (rv) || word0_004198D8 (rv) & Bndry_mask_004198D8)
	    {
	      if (aadj < .4999999 || aadj > .5000001)
		break;
	    }
	  else if (aadj < .4999999 / FLT_RADIX_004198D8)
	    break;
	}
    cont:
      g98D8_Bfree (ptr, bb);
      g98D8_Bfree (ptr, bd);
      g98D8_Bfree (ptr, bs);
      g98D8_Bfree (ptr, delta);
    }
retfree:
  g98D8_Bfree (ptr, bb);
  g98D8_Bfree (ptr, bd);
  g98D8_Bfree (ptr, bs);
  g98D8_Bfree (ptr, bd0);
  g98D8_Bfree (ptr, delta);
ret:
  if (se)
    *se = (char *) cursor;
  return sign ? -rv.d : rv.d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strtod);
#ifdef SKIP_ASM
extern int D_00454944;
extern void _strtod_r(int, int, int);
/**
 * Parses a floating-point number from a string with the global reentrancy struct (_strtod_r).
 *
 * Passes D_00454944 (the current reentrancy struct) on. Declared void in C; the double comes back from _strtod_r in the
 * return registers untouched.
 *
 * @param str  the string
 * @param endPtr  receives the end of the number, or null
 */
void strtod(int str, int endPtr) {
    _strtod_r(D_00454944, str, endPtr);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strtodf);
#ifdef SKIP_ASM
extern void dptofp(int);
/**
 * Parses a float from a string: the double parse rounded to single precision.
 *
 * Declared without parameters: the string and end pointer stay in the argument registers for strtod, and the result
 * goes through dptofp.
 */
void strtodf(void) {
    dptofp(((int (*)(void))strtod)());
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _strtoul_r);
#ifdef SKIP_ASM
extern char D_00499C81[];
extern long __udivdi3_div(long, long) __asm__("__udivdi3");
extern long __muldi3_mul(long, long) __asm__("__muldi3");
extern long __umoddi3_mod(long, long) __asm__("__umoddi3");
#define XCT(c) (D_00499C81[(int)(c)])
unsigned long _strtoul_r_(int *ptr, char *nptr, char **endptr, int base) __asm__("_strtoul_r");
/**
 * Parses an unsigned long from a string in the given base, for a reentrancy struct.
 *
 * Skips white space (ctype table D_00499C81), takes an optional sign and, for base 0 or 16, a 0x prefix; base 0 becomes
 * 8 after a leading 0, else 10. Digits and letters below the base are accumulated with the 64-bit helpers __udivdi3
 * (divide), __umoddi3 (remainder) and __muldi3 (multiply). Overflow gives -1 (all ones) and errno 34 (ERANGE);
 * a minus sign negates the result. *endptr gets the first unused character, or the start when no digit was read.
 *
 * @param ptr     the reentrancy struct (receives errno)
 * @param nptr    the string
 * @param endptr  receives the end, or null
 * @param base    the base (0 to detect, 2..36)
 * @return the value (64-bit), or all ones on overflow
 */
unsigned long _strtoul_r_(int *ptr, char *nptr, char **endptr, int base) {
    char *cursor = nptr;
    unsigned long acc, cutoff;
    int c, neg = 0, any, cutlim;
    do {
        c = *cursor++;
    } while (XCT(c) & 8);
    if (c == '-') {
        neg = 1;
        c = *cursor++;
    } else if (c == '+')
        c = *cursor++;
    if ((base == 0 || base == 16) && c == '0' && (*cursor == 'x' || *cursor == 'X')) {
        c = cursor[1];
        cursor += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = __udivdi3_div(-1, base);
    cutlim = __umoddi3_mod(-1, base);
    for (acc = 0, any = 0;; c = *cursor++) {
        if (XCT(c) & 4)
            c -= '0';
        else if (XCT(c) & 3)
            c -= (XCT(c) & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc = __muldi3_mul(acc, base);
            acc += c;
        }
    }
    if (any < 0) {
        acc = -1;
        *ptr = 34;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = any ? cursor - 1 : nptr;
    return acc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strtoul);
#ifdef SKIP_ASM
extern int D_00454944;
extern void _strtoul_r(int, int, int, int);
/**
 * Parses an unsigned long from a string in the given base, for the global reentrancy struct.
 *
 * Forwards to _strtoul_r with D_00454944. The definition is declared void, but $v0 still holds the value.
 *
 * @param str  the string
 * @param endPtr  receives the end, or null
 * @param base  the base
 */
void strtoul(int str, int endPtr, int base) {
    _strtoul_r(D_00454944, str, endPtr, base);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strcasecmp);
#ifdef SKIP_ASM
/**
 * Compares two strings ignoring case (C library strcasecmp).
 *
 * Steps both strings while the first has characters and their lowercase forms (tolower, tolower) agree, and
 * returns the difference of the lowercase forms where they stop.
 *
 * @param str1  the first string
 * @param str2  the second string
 * @return less than, equal to or greater than 0 as @p str1 sorts before, equal to or after @p str2
 */
int strcasecmp(char *str1, char *str2) {
    while (*str1 && tolower(*str1) == tolower(*str2)) {
        str1++;
        str2++;
    }
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strcspn);
#ifdef SKIP_ASM
/**
 * Returns the length of the leading part of a string that contains none of the reject characters.
 *
 * cBXString_spanExcluding calls it.
 *
 * @param str     the string
 * @param reject  the characters that end the span
 * @return the span length
 */
int strcspn(char *str, char *reject) {
    char *start = str;
    char *rejectPos;
    while (*str) {
        for (rejectPos = reject; *rejectPos; rejectPos++)
            if (*str == *rejectPos)
                break;
        if (*rejectPos)
            break;
        str++;
    }
    return str - start;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strncasecmp);
#ifdef SKIP_ASM
extern int tolower(int);
/**
 * Compares at most n characters of two strings ignoring case (C library strncasecmp).
 *
 * Returns 0 for a zero count; otherwise steps while the lowercase forms (tolower, tolower) agree, stopping at the
 * count or the end of either string, and returns the difference of the lowercase forms there.
 *
 * @param str1  the first string
 * @param str2  the second string
 * @param n   the most characters to compare
 * @return less than, equal to or greater than 0 as the prefix of @p str1 sorts before, equal to or after @p str2's
 */
int strncasecmp(char *str1, char *str2, unsigned n) {
    if (n == 0)
        return 0;
    while (n-- != 0 && tolower(*str1) == tolower(*str2)) {
        if (n == 0 || *str1 == '\0' || *str2 == '\0')
            break;
        str1++;
        str2++;
    }
    return tolower(*(unsigned char *)str1) - tolower(*(unsigned char *)str2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strpbrk);
#ifdef SKIP_ASM
/**
 * Returns the first character in a string that is one of the accept characters, or null.
 *
 * cBXString_findOneOf calls it.
 *
 * @param str     the string
 * @param accept  the characters searched for
 * @return a pointer to the first such character, or null
 */
char *strpbrk(char *str, char *accept) {
    char *acceptPos;
    if (*str == 0)
        return 0;
    for (; *str; str++) {
        for (acceptPos = accept; *acceptPos && *str != *acceptPos; acceptPos++)
            ;
        if (*acceptPos)
            break;
    }
    return *acceptPos ? str : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strrchr);
#ifdef SKIP_ASM
/**
 * Returns the last occurrence of a character in a string, or null.
 *
 * Searching for 0 finds the terminator. Callers include cBXString_reverseFind__FP9cBXStringc and
 * cInputPreProcessor_loadFileSize.
 *
 * @param str     the string
 * @param target  the character
 * @return a pointer to the last occurrence, or null
 */
char *strrchr(char *str, char target) {
    char *last = 0;
    while (*str) {
        if (*str == target) last = str;
        str++;
    }
    return *str == target ? str : last;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strspn);
#ifdef SKIP_ASM
/**
 * Returns the length of the leading part of a string made only of the accept characters.
 *
 * cBXString_spanIncluding calls it.
 *
 * @param str     the string
 * @param accept  the characters allowed in the span
 * @return the span length
 */
int strspn(char *str, char *accept) {
    char *start = str;
    char *acceptPos;
    while (*str) {
        for (acceptPos = accept; *acceptPos; acceptPos++)
            if (*str == *acceptPos)
                break;
        if (*acceptPos == 0)
            break;
        str++;
    }
    return str - start;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strstr);
#ifdef SKIP_ASM
/* newlib string/strstr.c (no notice in the file; newlib's default: Copyright (c) 1994-2000 Red Hat, Inc.
 * Permission to use, copy, modify and distribute this software is freely granted, provided that this notice is
 * preserved.) */
/**
 * Returns the first occurrence of one string inside another, or null.
 *
 * A plain double loop; an empty search string matches at the start. Callers include cBXString_find1 and
 * FileParser_FindPartialSection.
 *
 * @param searchee  the string searched
 * @param lookfor   the string searched for
 * @return a pointer to the first match, or null
 */
char *
strstr (const char *searchee, const char *lookfor)
{
  if (*searchee == 0)
    {
      if (*lookfor)
	return (char *) 0;
      return (char *) searchee;
    }

  while (*searchee)
    {
      unsigned int i;
      i = 0;

      while (1)
	{
	  if (lookfor[i] == 0)
	    {
	      return (char *) searchee;
	    }

	  if (lookfor[i] != searchee[i])
	    {
	      break;
	    }
	  i++;
	}
      searchee++;
    }

  return (char *) 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AE00);
#ifdef SKIP_ASM
/**
 * Rounds a decimal digit string at its last digit and returns 0 when the carry runs out of digits.
 *
 * When the last of the n digits is 5 or more, it becomes 0 and the carry moves left through 9s (which become 0). It
 * returns 0 when the first digit is also a 9, leaving the caller to add the leading 1. func_0041AF88 calls it.
 *
 * @param digits  the digit string
 * @param n       the digit count
 * @return 1 normally, 0 when the carry propagated past the first digit
 */
int func_0041AE00(char *digits, int n)
{
    if (n > 0) {
        n--;
        if (digits[n] >= '5') {
            do
                digits[n] = '0';
            while (--n > 0 && digits[n] == '9');
            if (digits[n] == '9')
                return 0;
            digits[n]++;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AE70);
#ifdef SKIP_ASM
/**
 * Reverses a string in place and returns it.
 *
 * func_0041AEE8 calls it.
 *
 * @param str  the string
 * @return str
 */
char *func_0041AE70(char *str) {
    int i, j;
    char swapChar;
    j = 0;
    while (str[j])
        j++;
    j--;
    for (i = 0; i < j; i++, j--) {
        swapChar = str[i];
        str[i] = str[j];
        str[j] = swapChar;
    }
    return str;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041AEE8);
#ifdef SKIP_ASM
/* Sony's libc addition itoa (not in newlib 1.9.0): digits from the .data table, then reversed in place by
   func_0041AE70 (strrev). */
extern char D_00454CB0[];
extern char *func_0041AE70(char *s);

/**
 * Writes an int as text in the given base and returns the buffer.
 *
 * Digits come from the table D_00454CB0, low digit first, then a - for a negative value, and func_0041AE70 reverses the
 * result.
 *
 * Likely bug: only base 10 negates the value, so a negative value in another base prints its 32-bit two's complement
 * digits with a - in front.
 *
 * @param value  the value
 * @param buf    the output buffer
 * @param base   the base
 * @return buf
 */
char *func_0041AEE8(int value, char *buf, unsigned int base)
{
    int i;
    int neg = value < 0;
    unsigned int remainingValue;

    remainingValue = value;
    if (neg && base == 10)
        remainingValue = -value;
    i = 0;
    do {
        buf[i++] = D_00454CB0[remainingValue % base];
        remainingValue /= base;
    } while (remainingValue != 0);
    if (neg)
        buf[i++] = '-';
    buf[i] = 0;
    return func_0041AE70(buf);
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041AF88);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041B528);
#ifdef SKIP_ASM
extern void func_0041B550(int, int, int, int);
/**
 * Calls the console output buffer func_0041B550 with mode 2 for a stream's descriptor, which does nothing.
 *
 * Quirk: func_0041B550 only flushes for mode 1, so this call writes nothing; no code calls this function.
 *
 * @param stream  the stream (descriptor at +0xE)
 */
void func_0041B528(short *stream) {
    func_0041B550(stream[7], 0, 0, 2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041B550);
#ifdef SKIP_ASM
extern char *D_00454CDC;
extern int D_00454CD8;
extern char D_0052AE98[];
extern int func_00424368(int, int, int);
int func_0041B550_(int fd, char *buf, unsigned n, int mode) __asm__("func_0041B550");
/**
 * Buffers bytes for a descriptor in a 128-byte buffer, or flushes that buffer (mode 1), and returns a count.
 *
 * Mode 1 writes the D_00454CD8 buffered bytes of D_0052AE98 with func_00424368 and resets the count and the cursor
 * D_00454CDC. Other modes copy n bytes in, writing the buffer out each time it holds 128; a write that returns 0 stops
 * with 0. The buffer is shared by every descriptor. func_0041B728 uses it.
 *
 * @param fd    the descriptor
 * @param buf   the bytes
 * @param n     the byte count
 * @param mode  1 to flush, else to buffer
 * @return the flush's write result for mode 1, else n (0 when a write returned 0)
 */
int func_0041B550_(int fd, char *buf, unsigned n, int mode) {
    unsigned i;
    int writeResult;
    if (mode == 1) {
        writeResult = func_00424368(fd, (int)D_0052AE98, D_00454CD8);
        D_00454CD8 = 0;
        D_00454CDC = D_0052AE98;
        return writeResult;
    }
    for (i = 0; i < n; i++) {
        *D_00454CDC = buf[i];
        D_00454CDC++;
        D_00454CD8++;
        if (D_00454CD8 >= 128) {
            writeResult = func_00424368(fd, (int)D_0052AE98, D_00454CD8);
            D_00454CDC = D_0052AE98;
            D_00454CD8 = 0;
            if (writeResult == 0)
                return 0;
        }
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", vfprintf);
#ifdef SKIP_ASM
extern int func_0041B728_f(int, int, char *, char *) __asm__("func_0041B728");
extern int func_0041CA70_i(int, int, char *, char *) __asm__("func_0041CA70");
/**
 * Formats to a stream, choosing the float-capable formatter only when the format needs it.
 *
 * Scans the format: when a % conversion ends in E, G, L, e, f or g it calls func_0041B728, else func_0041CA70 (the
 * integer-only formatter), each with the stream's reentrancy struct (+0x54). Its callers are printf,
 * _sprintf_r, sprintf, vsprintf and vsnprintf.
 *
 * @param stream  the output stream
 * @param fmt     the format string
 * @param ap      the argument pointer
 * @return the formatter's result (the character count, or -1)
 */
int vfprintf(int stream, int fmt, char *ap) {
    char *cursor = (char *)fmt;
    while (*cursor) {
        if (*cursor == '%' && cursor[1]) {
            cursor++;
            while (*cursor < 'A' && cursor[1])
                cursor++;
            switch (*cursor) {
            case 'E':
            case 'G':
            case 'L':
            case 'e':
            case 'f':
            case 'g':
                return func_0041B728_f(*(int *)(stream + 0x54), stream, (char *)fmt, ap);
            }
        }
        cursor++;
    }
    return func_0041CA70_i(*(int *)(stream + 0x54), stream, (char *)fmt, ap);
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041B728);

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041C788);
#ifdef SKIP_ASM
extern void *g_c788_memcpy(void *, const void *, unsigned) __asm__("func_0041605C");
char *func_0041C788_impl(char *dest, char *start, char *end, int *outlen) __asm__("func_0041C788");
/**
 * Copies a number string with commas inserted every three digits before the decimal point and returns dest.
 *
 * Builds the result backwards in an alloca buffer of end - start + 10 bytes. The point is the first . in the text, or
 * end when there is none; a number with an exponent (e or E) is returned unchanged as start. Stores the new length in
 * *outlen. Both formatters (func_0041B728, func_0041CA70) call it for the grouping flag 0x200.
 *
 * @param dest    the output buffer
 * @param start   the first character
 * @param end     one past the last character
 * @param outlen  receives the output length
 * @return dest, start for an exponent number, or null when an argument is null
 */
char *func_0041C788_impl(char *dest, char *start, char *end, int *outlen) {
    int len = end - start + 10;
    char *buf = __builtin_alloca(len);
    char *w = buf + len - 1;
    int cnt = 0;
    int st[2];
    char *cursor;
    if (dest == 0 || start == 0 || end == 0) return 0;
    st[0] = 0;
    st[1] = (int)end;
    cursor = start;
    if (!(end < cursor)) {
        do {
            switch (*cursor) {
            case '.':
                st[1] = (int)cursor;
                if (st[0] == 0) st[0] = 1;
                break;
            case 'E':
            case 'e':
                st[0] = 2;
                break;
            }
            cursor++;
        } while (!(end < cursor) && st[0] != 2);
    }
    if (st[0] == 2) return start;
    cursor = end;
    if (start < cursor) {
        do {
            *w = *cursor;
            w--;
            if (!((char *)st[1] < cursor)) {
                if (cnt % 3 == 0 && cnt != 0) {
                    *w = ',';
                    len++;
                    w--;
                }
                cnt++;
            }
            cursor--;
        } while (start < cursor);
    }
    *w = *cursor;
    len -= 10;
    g_c788_memcpy(dest, w, len);
    *outlen = len;
    return dest;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041C938);
#ifdef SKIP_ASM
extern char *D_00454CE4;
extern int D_00454CE0;
extern char D_0052AF18[];
int func_0041C938_(int fd, char *buf, unsigned n, int mode) __asm__("func_0041C938");
/**
 * Buffers bytes for a descriptor in a second 128-byte buffer, or flushes it (mode 1), and returns a count.
 *
 * The same code as func_0041B550 with its own buffer D_0052AF18, count D_00454CE0 and cursor D_00454CE4; writes with
 * func_00424368. func_0041CA70 uses it.
 *
 * @param fd    the descriptor
 * @param buf   the bytes
 * @param n     the byte count
 * @param mode  1 to flush, else to buffer
 * @return the flush's write result for mode 1, else n (0 when a write returned 0)
 */
int func_0041C938_(int fd, char *buf, unsigned n, int mode) {
    unsigned i;
    int writeResult;
    if (mode == 1) {
        writeResult = func_00424368(fd, (int)D_0052AF18, D_00454CE0);
        D_00454CE0 = 0;
        D_00454CE4 = D_0052AF18;
        return writeResult;
    }
    for (i = 0; i < n; i++) {
        *D_00454CE4 = buf[i];
        D_00454CE4++;
        D_00454CE0++;
        if (D_00454CE0 >= 128) {
            writeResult = func_00424368(fd, (int)D_0052AF18, D_00454CE0);
            D_00454CE4 = D_0052AF18;
            D_00454CE0 = 0;
            if (writeResult == 0)
                return 0;
        }
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041CA40);
#ifdef SKIP_ASM
extern void func_0041CA70(int, int *, int, int);
/**
 * Formats to a stream with the integer-only formatter func_0041CA70.
 *
 * Passes the stream's reentrancy struct (+0x54) first. No code calls it directly.
 *
 * @param stream  the output stream
 * @param fmt     the format string
 * @param args    the argument pointer
 */
void func_0041CA40(int *stream, int fmt, int args) {
    func_0041CA70(stream[21], stream, fmt, args);
}
#endif

INCLUDE_ASM("seg/seg_316FB8", func_0041CA70);

//100%
INCLUDE_ASM("seg/seg_316FB8", malloc_extend_top);
#ifdef SKIP_ASM
struct mchunk {
    unsigned prev_size, size;
    struct mchunk *fd, *bk;
};
typedef struct mchunk *mchunkptr;
extern unsigned D_00454CE8[];
extern unsigned long D_004550F0;
extern unsigned long D_004550F8;
#define bin_at(i) ((mchunkptr)((char *)&D_00454CE8[2 * (i) + 2] - 8))
#define top (bin_at(0)->fd)
#define last_remainder (bin_at(1))
#define binblocks (bin_at(0)->size)
#define chunk_at_offset(p, s) ((mchunkptr)((char *)(p) + (s)))
#define chunksize(p) ((p)->size & ~3u)
#define inuse_bit_at_offset(p, s) (chunk_at_offset(p, s)->size & 1)
#define unlink(P, BK, FD) \
    { \
        BK = (P)->bk; \
        FD = (P)->fd; \
        FD->bk = BK; \
        BK->fd = FD; \
    }
#define set_head(p, s) ((p)->size = (s))
#define set_foot(p, s) (chunk_at_offset(p, s)->prev_size = (s))
#define idx2binblock(ix) ((unsigned long)1 << ((ix) / 4))
#define mark_binblock(ii) (binblocks |= idx2binblock(ii))
#define smallbin_index(sz) (((unsigned)(sz)) >> 3)
#define bin_index(sz) \
    ((((unsigned)(sz)) >> 9 == 0) ? (((unsigned)(sz)) >> 3) \
    : (((unsigned)(sz)) >> 9 <= 4) ? 56 + (((unsigned)(sz)) >> 6) \
    : (((unsigned)(sz)) >> 9 <= 20) ? 91 + (((unsigned)(sz)) >> 9) \
    : (((unsigned)(sz)) >> 9 <= 84) ? 110 + (((unsigned)(sz)) >> 12) \
    : (((unsigned)(sz)) >> 9 <= 340) ? 119 + (((unsigned)(sz)) >> 15) \
    : (((unsigned)(sz)) >> 9 <= 1364) ? 124 + (((unsigned)(sz)) >> 18) \
    : 126)
#define frontlink(P, S, IDX, BK, FD) \
    { \
        if (S < 512) { \
            IDX = smallbin_index(S); \
            mark_binblock(IDX); \
            BK = bin_at(IDX); \
            FD = BK->fd; \
            P->bk = BK; \
            P->fd = FD; \
            FD->bk = BK->fd = P; \
        } else { \
            IDX = bin_index(S); \
            BK = bin_at(IDX); \
            FD = BK->fd; \
            if (FD == BK) { \
                mark_binblock(IDX); \
            } else { \
                while (FD != BK && S < chunksize(FD)) \
                    FD = FD->fd; \
                BK = FD->bk; \
            } \
            P->bk = BK; \
            P->fd = FD; \
            FD->bk = BK->fd = P; \
        } \
    }
#define link_last_remainder(P) \
    { \
        lr->fd = lr->bk = P; \
        P->fd = P->bk = lr; \
    }

#define set_head_size(p, s) ((p)->size = (((p)->size & 1) | (s)))
extern char *D_00455100;
extern int D_00455118;
extern unsigned long D_00455108;
extern unsigned long D_00455110;
extern char *_sbrk_r_sbrk(int, int) __asm__("_sbrk_r");
extern void _free_r_free(int, void *) __asm__("_free_r");
void malloc_extend_top_(int ptr, unsigned nb) __asm__("malloc_extend_top");
/**
 * Grows the C heap's top chunk by at least nb bytes with sbrk.
 *
 * Doug Lea's allocator 2.6.4 as built into newlib. Asks _sbrk_r for nb + top_pad (D_004550F8) + 16, rounded to
 * 0x1000 pages once the heap has a base. Contiguous space just extends the top chunk. Otherwise it records the first
 * sbrk base (D_00455100), aligns the new space to 16 bytes with a second sbrk to the next page, makes it the top, and
 * fences the old top with two 5-byte-size headers before freeing it with _free_r. Updates sbrked_mem D_00455118
 * and the peaks D_00455108 and D_00455110. Gives up silently when sbrk fails or returns memory below the old top.
 *
 * @param ptr  the reentrancy struct
 * @param nb   the chunk size needed
 */
void malloc_extend_top_(int ptr, unsigned nb) {
    char *brk;
    unsigned front_misalign;
    unsigned correction;
    char *new_brk;
    unsigned top_size;
    mchunkptr old_top = top;
    unsigned old_top_size = chunksize(old_top);
    char *old_end = (char *)chunk_at_offset(old_top, old_top_size);
    unsigned sbrk_size = nb + D_004550F8 + 16;
    unsigned long pagesz = 0x1000;
    if (D_00455100 != (char *)-1)
        sbrk_size = (sbrk_size + (pagesz - 1)) & ~(pagesz - 1);
    brk = _sbrk_r_sbrk(ptr, sbrk_size);
    if (brk == (char *)-1 || (brk < old_end && old_top != (mchunkptr)D_00454CE8))
        return;
    D_00455118 += sbrk_size;
    if (brk == old_end) {
        top_size = sbrk_size + old_top_size;
        set_head(top, top_size | 1);
    } else {
        if (D_00455100 == (char *)-1)
            D_00455100 = brk;
        else
            D_00455118 += brk - old_end;
        front_misalign = (unsigned long)(brk + 8) & 15;
        if (front_misalign > 0) {
            correction = 16 - front_misalign;
            brk += correction;
        } else
            correction = 0;
        correction += pagesz - ((unsigned long)(brk + sbrk_size) & (pagesz - 1));
        new_brk = _sbrk_r_sbrk(ptr, correction);
        if (new_brk == (char *)-1)
            return;
        D_00455118 += correction;
        top = (mchunkptr)brk;
        top_size = new_brk - brk + correction;
        set_head(top, top_size | 1);
        if (old_top != (mchunkptr)D_00454CE8) {
            if (old_top_size < 16) {
                set_head(top, 1);
                return;
            }
            old_top_size = (old_top_size - 12) & ~15u;
            set_head_size(old_top, old_top_size);
            chunk_at_offset(old_top, old_top_size)->size = 5;
            chunk_at_offset(old_top, old_top_size + 4)->size = 5;
            if (old_top_size >= 16)
                _free_r_free(ptr, (char *)old_top + 8);
        }
    }
    if ((unsigned long)D_00455118 > D_00455108)
        D_00455108 = D_00455118;
    if ((unsigned long)D_00455118 > D_00455110)
        D_00455110 = D_00455118;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _malloc_r);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/mallocr.c (mALLOc = _malloc_r) and its macros, verbatim:
 *
 * A version of malloc/free/realloc written by Doug Lea and released to the
 * public domain.  Send questions/comments/complaints/performance data
 * to dl@cs.oswego.edu
 * VERSION 2.6.4  Thu Nov 28 07:54:55 1996  Doug Lea  (dl at gee)
 *
 * Configuration as the retail code shows: MALLOC_ALIGNMENT 16, SIZE_T_SMALLER_THAN_LONG (EE long is 64-bit),
 * no HAVE_MMAP. av_ (__malloc_av_) is D_00454CE8, one 0x408-byte object (config/symbol_addrs.txt).
 */
typedef mchunkptr mbinptr_0041DBF8;
#define SIZE_SZ_0041DBF8                (sizeof(unsigned int))
#define MALLOC_ALIGNMENT_0041DBF8       16
#define MALLOC_ALIGN_0041DBF8           MALLOC_ALIGNMENT_0041DBF8
#define MALLOC_ALIGN_MASK_0041DBF8      (MALLOC_ALIGNMENT_0041DBF8 - 1)
#define MINSIZE_0041DBF8                (sizeof(struct mchunk))
#define chunk2mem_0041DBF8(p)   ((void*)((char*)(p) + 2*SIZE_SZ_0041DBF8))
#define request2size_0041DBF8(req) \
 (((long)((req) + (SIZE_SZ_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8)) < \
  (long)(MINSIZE_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8)) ? ((MINSIZE_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8) & ~(MALLOC_ALIGN_MASK_0041DBF8)) : \
   (((req) + (SIZE_SZ_0041DBF8 + MALLOC_ALIGN_MASK_0041DBF8)) & ~(MALLOC_ALIGN_MASK_0041DBF8)))
#define long_sub_size_t_0041DBF8(x, y) ( (x < y) ? -((long)(y - x)) : (x - y) );
#define PREV_INUSE_0041DBF8 0x1
#define SIZE_BITS_0041DBF8 (0x1|0x2)
#define set_inuse_bit_at_offset_0041DBF8(p, s)\
 (((mchunkptr)(((char*)(p)) + (s)))->size |= PREV_INUSE_0041DBF8)
#define chunksize_0041DBF8(p)          ((p)->size & ~(SIZE_BITS_0041DBF8))
#define set_head_0041DBF8(p, s)        ((p)->size = (s))
#define set_foot_0041DBF8(p, s)   (((mchunkptr)((char*)(p) + (s)))->prev_size = (s))
#define av__0041DBF8 D_00454CE8
#define bin_at_0041DBF8(i)      ((mbinptr_0041DBF8)((char*)&(av__0041DBF8[2*(i) + 2]) - 2*SIZE_SZ_0041DBF8))
#define next_bin_0041DBF8(b)    ((mbinptr_0041DBF8)((char*)(b) + 2 * sizeof(mbinptr_0041DBF8)))
#define prev_bin_0041DBF8(b)    ((mbinptr_0041DBF8)((char*)(b) - 2 * sizeof(mbinptr_0041DBF8)))
#define top_0041DBF8            (bin_at_0041DBF8(0)->fd)
#define last_remainder_0041DBF8 (bin_at_0041DBF8(1))
#define first_0041DBF8(b) ((b)->fd)
#define last_0041DBF8(b)  ((b)->bk)
#define bin_index_0041DBF8(sz)                                                          \
(((((unsigned long)(sz)) >> 9) ==    0) ?       (((unsigned long)(sz)) >>  3): \
 ((((unsigned long)(sz)) >> 9) <=    4) ?  56 + (((unsigned long)(sz)) >>  6): \
 ((((unsigned long)(sz)) >> 9) <=   20) ?  91 + (((unsigned long)(sz)) >>  9): \
 ((((unsigned long)(sz)) >> 9) <=   84) ? 110 + (((unsigned long)(sz)) >> 12): \
 ((((unsigned long)(sz)) >> 9) <=  340) ? 119 + (((unsigned long)(sz)) >> 15): \
 ((((unsigned long)(sz)) >> 9) <= 1364) ? 124 + (((unsigned long)(sz)) >> 18): \
                                          126)
#define MAX_SMALLBIN_SIZE_0041DBF8   512
#define SMALLBIN_WIDTH_0041DBF8        8
#define SMALLBIN_WIDTH_BITS_0041DBF8   3
#define MAX_SMALLBIN_0041DBF8        (MAX_SMALLBIN_SIZE_0041DBF8 / SMALLBIN_WIDTH_0041DBF8) - 1
#define smallbin_index_0041DBF8(sz)  (((unsigned long)(sz)) >> SMALLBIN_WIDTH_BITS_0041DBF8)
#define is_small_request_0041DBF8(nb) (nb < MAX_SMALLBIN_SIZE_0041DBF8 - SMALLBIN_WIDTH_0041DBF8)
#define BINBLOCKWIDTH_0041DBF8     4
#define binblocks_0041DBF8      (bin_at_0041DBF8(0)->size)
#define idx2binblock_0041DBF8(ix)    ((unsigned long)1 << (ix / BINBLOCKWIDTH_0041DBF8))
#define mark_binblock_0041DBF8(ii)   (binblocks_0041DBF8 |= idx2binblock_0041DBF8(ii))
#define frontlink_0041DBF8(P, S, IDX, BK, FD)                                 \
{                                                                             \
  if (S < MAX_SMALLBIN_SIZE_0041DBF8)                                         \
  {                                                                           \
    IDX = smallbin_index_0041DBF8(S);                                         \
    mark_binblock_0041DBF8(IDX);                                              \
    BK = bin_at_0041DBF8(IDX);                                                \
    FD = BK->fd;                                                              \
    P->bk = BK;                                                               \
    P->fd = FD;                                                               \
    FD->bk = BK->fd = P;                                                      \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    IDX = bin_index_0041DBF8(S);                                              \
    BK = bin_at_0041DBF8(IDX);                                                \
    FD = BK->fd;                                                              \
    if (FD == BK) mark_binblock_0041DBF8(IDX);                                \
    else                                                                      \
    {                                                                         \
      while (FD != BK && S < chunksize_0041DBF8(FD)) FD = FD->fd;             \
      BK = FD->bk;                                                            \
    }                                                                         \
    P->bk = BK;                                                               \
    P->fd = FD;                                                               \
    FD->bk = BK->fd = P;                                                      \
  }                                                                           \
}
#define unlink_0041DBF8(P, BK, FD)                                            \
{                                                                             \
  BK = P->bk;                                                                 \
  FD = P->fd;                                                                 \
  FD->bk = BK;                                                                \
  BK->fd = FD;                                                                \
}
#define link_last_remainder_0041DBF8(P)                                       \
{                                                                             \
  last_remainder_0041DBF8->fd = last_remainder_0041DBF8->bk =  P;             \
  P->fd = P->bk = last_remainder_0041DBF8;                                    \
}
#define clear_last_remainder_0041DBF8 \
  (last_remainder_0041DBF8->fd = last_remainder_0041DBF8->bk = last_remainder_0041DBF8)
#define MALLOC_LOCK_0041DBF8 __malloc_lock(reent_ptr)
#define MALLOC_UNLOCK_0041DBF8 __malloc_unlock(reent_ptr)
extern void g41DBF8_extend_top(int, unsigned int) __asm__("malloc_extend_top");

/* the unit declares this as int _malloc_r(int, int) (malloc, func_004188C0) */
void *_malloc_r_(int, unsigned int) __asm__("_malloc_r");
/**
 * Allocates a block from the C heap and returns it, or null (no locking).
 *
 * Doug Lea's allocator 2.6.4 as built into newlib: pads the request to a 16-byte-aligned chunk size, tries the exact
 * small bin, the last remainder, then the bin blocks (best fit, splitting and relinking remainders through frontlink),
 * and finally the top chunk, growing it with malloc_extend_top. Requests whose padding overflows set errno 12. Bins start
 * at D_00454CE8.
 *
 * @param reent_ptr  the reentrancy struct
 * @param bytes      bytes wanted
 * @return the block, or null
 */
void *_malloc_r_(int reent_ptr, unsigned int bytes)
{
  mchunkptr victim;                  /* inspected/selected chunk */
  unsigned int victim_size;          /* its size */
  int       idx;                     /* index for bin traversal */
  mbinptr_0041DBF8 bin;              /* associated bin */
  mchunkptr remainder;               /* remainder from a split */
  long      remainder_size;          /* its size */
  int       remainder_index;         /* its bin index */
  unsigned long block;               /* block traverser bit */
  int       startidx;                /* first bin of a traversed block */
  mchunkptr fwd;                     /* misc temp for linking */
  mchunkptr bck;                     /* misc temp for linking */
  mbinptr_0041DBF8 q;                /* misc temp */

  unsigned int nb  = request2size_0041DBF8(bytes);  /* padded request size; */

  MALLOC_LOCK_0041DBF8;

  /* Check for exact match in a bin */

  if (is_small_request_0041DBF8(nb))  /* Faster version for small requests */
  {
    idx = smallbin_index_0041DBF8(nb); 

    /* No traversal or size check necessary for small bins.  */

    q = bin_at_0041DBF8(idx);
    victim = last_0041DBF8(q);

    if (victim != q)
    {
      victim_size = chunksize_0041DBF8(victim);
      unlink_0041DBF8(victim, bck, fwd);
      set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
      MALLOC_UNLOCK_0041DBF8;
      return chunk2mem_0041DBF8(victim);
    }

    idx += 2; /* Set for bin scan below. We've already scanned 2 bins. */

  }
  else
  {
    idx = bin_index_0041DBF8(nb);
    bin = bin_at_0041DBF8(idx);

    for (victim = last_0041DBF8(bin); victim != bin; victim = victim->bk)
    {
      victim_size = chunksize_0041DBF8(victim);
      remainder_size = long_sub_size_t_0041DBF8(victim_size, nb);
      
      if (remainder_size >= (long)MINSIZE_0041DBF8) /* too big */
      {
        --idx; /* adjust to rescan below after checking last remainder */
        break;   
      }

      else if (remainder_size >= 0) /* exact fit */
      {
        unlink_0041DBF8(victim, bck, fwd);
        set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
	MALLOC_UNLOCK_0041DBF8;
        return chunk2mem_0041DBF8(victim);
      }
    }

    ++idx; 

  }

  /* Try to use the last split-off remainder */

  if ( (victim = last_remainder_0041DBF8->fd) != last_remainder_0041DBF8)
  {
    victim_size = chunksize_0041DBF8(victim);
    remainder_size = long_sub_size_t_0041DBF8(victim_size, nb);

    if (remainder_size >= (long)MINSIZE_0041DBF8) /* re-split */
    {
      remainder = chunk_at_offset(victim, nb);
      set_head_0041DBF8(victim, nb | PREV_INUSE_0041DBF8);
      link_last_remainder_0041DBF8(remainder);
      set_head_0041DBF8(remainder, remainder_size | PREV_INUSE_0041DBF8);
      set_foot_0041DBF8(remainder, remainder_size);
      MALLOC_UNLOCK_0041DBF8;
      return chunk2mem_0041DBF8(victim);
    }

    clear_last_remainder_0041DBF8;

    if (remainder_size >= 0)  /* exhaust */
    {
      set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
      MALLOC_UNLOCK_0041DBF8;
      return chunk2mem_0041DBF8(victim);
    }

    /* Else place in bin */

    frontlink_0041DBF8(victim, victim_size, remainder_index, bck, fwd);
  }

  /* 
     If there are any possibly nonempty big-enough blocks, 
     search for best fitting chunk by scanning bins in blockwidth units.
  */

  if ( (block = idx2binblock_0041DBF8(idx)) <= binblocks_0041DBF8)  
  {

    /* Get to the first marked block */

    if ( (block & binblocks_0041DBF8) == 0) 
    {
      /* force to an even block boundary */
      idx = (idx & ~(BINBLOCKWIDTH_0041DBF8 - 1)) + BINBLOCKWIDTH_0041DBF8;
      block <<= 1;
      while ((block & binblocks_0041DBF8) == 0)
      {
        idx += BINBLOCKWIDTH_0041DBF8;
        block <<= 1;
      }
    }
      
    /* For each possibly nonempty block ... */
    for (;;)  
    {
      startidx = idx;          /* (track incomplete blocks) */
      q = bin = bin_at_0041DBF8(idx);

      /* For each bin in this block ... */
      do
      {
        /* Find and use first big enough chunk ... */

        for (victim = last_0041DBF8(bin); victim != bin; victim = victim->bk)
        {
          victim_size = chunksize_0041DBF8(victim);
          remainder_size = long_sub_size_t_0041DBF8(victim_size, nb);

          if (remainder_size >= (long)MINSIZE_0041DBF8) /* split */
          {
            remainder = chunk_at_offset(victim, nb);
            set_head_0041DBF8(victim, nb | PREV_INUSE_0041DBF8);
            unlink_0041DBF8(victim, bck, fwd);
            link_last_remainder_0041DBF8(remainder);
            set_head_0041DBF8(remainder, remainder_size | PREV_INUSE_0041DBF8);
            set_foot_0041DBF8(remainder, remainder_size);
	    MALLOC_UNLOCK_0041DBF8;
            return chunk2mem_0041DBF8(victim);
          }

          else if (remainder_size >= 0)  /* take */
          {
            set_inuse_bit_at_offset_0041DBF8(victim, victim_size);
            unlink_0041DBF8(victim, bck, fwd);
	    MALLOC_UNLOCK_0041DBF8;
            return chunk2mem_0041DBF8(victim);
          }

        }

       bin = next_bin_0041DBF8(bin);

       if (idx < MAX_SMALLBIN_0041DBF8)
         {
           bin = next_bin_0041DBF8(bin);
           ++idx;
         }
      } while ((++idx & (BINBLOCKWIDTH_0041DBF8 - 1)) != 0);

      /* Clear out the block bit. */

      do   /* Possibly backtrack to try to clear a partial block */
      {
        if ((startidx & (BINBLOCKWIDTH_0041DBF8 - 1)) == 0)
        {
          binblocks_0041DBF8 &= ~block;
          break;
        }
        --startidx;
       q = prev_bin_0041DBF8(q);
      } while (first_0041DBF8(q) == q);

      /* Get to the next possibly nonempty block */

      if ( (block <<= 1) <= binblocks_0041DBF8 && (block != 0) ) 
      {
        while ((block & binblocks_0041DBF8) == 0)
        {
          idx += BINBLOCKWIDTH_0041DBF8;
          block <<= 1;
        }
      }
      else
        break;
    }
  }


  /* Try to use top chunk */

  /* Require that there be a remainder, ensuring top always exists  */
  remainder_size = long_sub_size_t_0041DBF8(chunksize_0041DBF8(top_0041DBF8), nb);
  if (chunksize_0041DBF8(top_0041DBF8) < nb || remainder_size < (long)MINSIZE_0041DBF8)
  {
    /* Try to extend */
    g41DBF8_extend_top(reent_ptr, nb);
    remainder_size = long_sub_size_t_0041DBF8(chunksize_0041DBF8(top_0041DBF8), nb);
    if (chunksize_0041DBF8(top_0041DBF8) < nb || remainder_size < (long)MINSIZE_0041DBF8)
    {
      MALLOC_UNLOCK_0041DBF8;
      return 0; /* propagate failure */
    }
  }

  victim = top_0041DBF8;
  set_head_0041DBF8(victim, nb | PREV_INUSE_0041DBF8);
  top_0041DBF8 = chunk_at_offset(victim, nb);
  set_head_0041DBF8(top_0041DBF8, remainder_size | PREV_INUSE_0041DBF8);
  MALLOC_UNLOCK_0041DBF8;
  return chunk2mem_0041DBF8(victim);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _free_r);
#ifdef SKIP_ASM
extern void __malloc_lock_lock(int) __asm__("__malloc_lock");
extern void __malloc_unlock_unlock(int) __asm__("__malloc_unlock");
extern int _malloc_trim_r_trim(int, unsigned) __asm__("_malloc_trim_r");
void _free_r_(int ptr, void *mem) __asm__("_free_r");
/**
 * Returns a block to the C heap (no locking).
 *
 * Doug Lea's allocator 2.6.4 as built into newlib: a null block is ignored. A chunk next to the top merges into it, and
 * when the top then reaches trim_threshold (D_004550F0) the heap is trimmed by _malloc_trim_r with top_pad. Otherwise it
 * coalesces with free neighbours and links the result into its bin.
 *
 * @param ptr  the reentrancy struct
 * @param mem  the block, or null
 */
void _free_r_(int ptr, void *mem) {
    mchunkptr p;
    unsigned hd, sz;
    int idx;
    mchunkptr next;
    unsigned nextsz, prevsz;
    mchunkptr bck, fwd;
    int islr;
    mchunkptr lr;
    if (mem == 0)
        return;
    __malloc_lock_lock(ptr);
    p = (mchunkptr)((char *)mem - 8);
    hd = p->size;
    sz = hd & ~1u;
    next = chunk_at_offset(p, sz);
    nextsz = chunksize(next);
    if (next == top) {
        sz += nextsz;
        if (!(hd & 1)) {
            prevsz = p->prev_size;
            p = chunk_at_offset(p, -(int)prevsz);
            sz += prevsz;
            unlink(p, bck, fwd);
        }
        set_head(p, sz | 1);
        top = p;
        if ((unsigned long)sz >= D_004550F0)
            _malloc_trim_r_trim(ptr, D_004550F8);
        __malloc_unlock_unlock(ptr);
        return;
    }
    set_head(next, nextsz);
    islr = 0;
    if (!(hd & 1)) {
        prevsz = p->prev_size;
        p = chunk_at_offset(p, -(int)prevsz);
        sz += prevsz;
        if (p->fd == last_remainder)
            islr = 1;
        else
            unlink(p, bck, fwd);
    }
    if (!inuse_bit_at_offset(next, nextsz)) {
        sz += nextsz;
        if (!islr && next->fd == (lr = last_remainder)) {
            islr = 1;
            link_last_remainder(p);
        } else
            unlink(next, bck, fwd);
    }
    set_head(p, sz | 1);
    set_foot(p, sz);
    if (!islr)
        frontlink(p, sz, idx, bck, fwd);
    __malloc_unlock_unlock(ptr);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _malloc_trim_r);
#ifdef SKIP_ASM
/* newlib stdlib/mallocr.c malloc_trim (_malloc_trim_r): "A version of malloc/free/realloc written by Doug Lea and
 * released to the public domain." Uses the unit's mallocr macros (top, chunksize, set_head) and declarations from
 * malloc_extend_top / _free_r, and its libgcc 64-bit helpers from _strtoul_r. */
/* newlib's struct mallinfo; sbrked_mem is current_mallinfo.arena (D_00455118, declared int earlier in the unit) */
struct mallinfo_E638 {
  int arena;    /* total space allocated from system */
  int ordblks;  /* number of non-inuse chunks */
  int smblks;   /* unused -- always zero */
  int hblks;    /* number of mmapped regions */
  int hblkhd;   /* total space in mmapped regions */
  int usmblks;  /* unused -- always zero */
  int fsmblks;  /* unused -- always zero */
  int uordblks; /* total allocated space */
  int fordblks; /* total non-inuse space */
  int keepcost; /* top-most, releasable (via malloc_trim) space */
};
extern struct mallinfo_E638 current_mallinfo_E638 __asm__("D_00455118");
#define sbrked_mem_E638  (current_mallinfo_E638.arena)
int _malloc_trim_r_(int reent_ptr, unsigned int pad) __asm__("_malloc_trim_r");
/**
 * Gives unused memory at the top of the C heap back to the system and returns 1 when it released any.
 *
 * Keeps pad bytes plus the minimum chunk; whole 0x1000-byte pages beyond that are released with a negative
 * _sbrk_r call when the heap end is still where the allocator left it. Updates sbrked_mem (D_00455118). When the
 * shrink fails it re-reads the break to resynchronise the top chunk.
 *
 * @param reent_ptr  the reentrancy struct
 * @param retainBytes        bytes to keep above the top chunk's minimum
 * @return 1 when memory was released, else 0
 */
int _malloc_trim_r_(int reent_ptr, unsigned int retainBytes)
{
  long  top_size;        /* Amount of top-most memory */
  long  extra;           /* Amount to release */
  char* current_brk;     /* address returned by pre-check sbrk call */
  char* new_brk;         /* address returned by negative sbrk call */

  unsigned long pagesz = 0x1000;

  __malloc_lock_lock(reent_ptr);

  top_size = chunksize(top);
  extra = __muldi3_mul(__udivdi3_div(top_size - retainBytes - 16 + (pagesz-1), pagesz) - 1, pagesz);

  if (extra < (long)pagesz)  /* Not enough memory to release */
  {
    __malloc_unlock_unlock(reent_ptr);
    return 0;
  }

  else
  {
    /* Test to make sure no one else called sbrk */
    current_brk = (char*)(_sbrk_r_sbrk (reent_ptr, 0));
    if (current_brk != (char*)(top) + top_size)
    {
      __malloc_unlock_unlock(reent_ptr);
      return 0;     /* Apparently we don't own memory; must fail */
    }

    else
    {
      new_brk = (char*)(_sbrk_r_sbrk (reent_ptr, -extra));

      if (new_brk == (char*)(-1)) /* sbrk failed? */
      {
        /* Try to figure out what we have */
        current_brk = (char*)(_sbrk_r_sbrk (reent_ptr, 0));
        top_size = current_brk - (char*)top;
        if (top_size >= (long)16) /* if not, we are very very dead! */
        {
          sbrked_mem_E638 = current_brk - D_00455100;
          set_head(top, top_size | 1);
        }
        __malloc_unlock_unlock(reent_ptr);
        return 0;
      }

      else
      {
        /* Success. Adjust top accordingly. */
        set_head(top, (top_size - extra) | 1);
        sbrked_mem_E638 -= extra;
        __malloc_unlock_unlock(reent_ptr);
        return 1;
      }
    }
  }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _strtol_r);
#ifdef SKIP_ASM
long _strtol_r_(int *ptr, char *nptr, char **endptr, int base) __asm__("_strtol_r");
/**
 * Parses a signed integer in a given base from a string, clamping on overflow (newlib strtol.c, 64-bit long).
 *
 * Skips spaces (ctype bit 8 of D_00499C81), takes an optional sign and, for base 0 or 16, a 0x prefix; base 0 picks 8
 * for a leading 0, else 10. Digits and letters are accepted while below the base. long is 64 bits here, so the limit is
 * computed with the 64-bit helpers __umoddi3 (remainder), __udivdi3 (division) and __muldi3
 * (multiplication); on overflow the result is LONG_MIN/LONG_MAX and errno (+0) is set to 34.
 *
 * @param ptr     the reentrancy struct (errno at +0)
 * @param nptr    the string
 * @param endptr  receives the end of the number (nptr when there was none), or null
 * @param base    0 or 2..36
 * @return the parsed value
 */
long _strtol_r_(int *ptr, char *nptr, char **endptr, int base) {
    char *cursor = nptr;
    unsigned long acc, cutoff;
    int c, neg = 0, any, cutlim;
    do {
        c = *cursor++;
    } while (XCT(c) & 8);
    if (c == '-') {
        neg = 1;
        c = *cursor++;
    } else if (c == '+')
        c = *cursor++;
    if ((base == 0 || base == 16) && c == '0' && (*cursor == 'x' || *cursor == 'X')) {
        c = cursor[1];
        cursor += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = neg ? -(unsigned long)0x8000000000000000L : 0x7FFFFFFFFFFFFFFFL;
    cutlim = __umoddi3_mod(cutoff, base);
    cutoff = __udivdi3_div(cutoff, base);
    for (acc = 0, any = 0;; c = *cursor++) {
        if (XCT(c) & 4)
            c -= '0';
        else if (XCT(c) & 3)
            c -= (XCT(c) & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc = __muldi3_mul(acc, base);
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? 0x8000000000000000L : 0x7FFFFFFFFFFFFFFFL;
        *ptr = 34;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = any ? cursor - 1 : nptr;
    return acc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", strtol);
#ifdef SKIP_ASM
extern int D_00454944;
extern long _strtol_r(int, int, int, int);
/**
 * Parses an integer in a given base from a string with the global reentrancy struct (_strtol_r).
 *
 * @param str     the string
 * @param end   receives the end of the number, or null
 * @param base  0 or 2..36
 * @return the parsed value (64-bit long)
 */
long strtol(int str, int end, int base) {
    return _strtol_r(D_00454944, str, end, base);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", tolower);
#ifdef SKIP_ASM
extern char D_00499C81[];
/**
 * Converts an upper-case letter to lower case; any other character is returned unchanged.
 *
 * Uses the upper-case bit (1) of the ctype table D_00499C81. strcasecmp and strncasecmp call it.
 *
 * @param charCode  the character
 * @return the lower-case character, or the argument
 */
int tolower(int charCode) {
    return (D_00499C81[charCode] & 1) ? charCode + 0x20 : charCode;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _setlocale_r);
#ifdef SKIP_ASM
extern char D_0049A7D0b[] __asm__("D_0049A7D0");
extern char D_0049A7C0[], D_0049A7D0[], D_0049A7D8[], D_0049A7E0[], D_0049A7E8[];
extern char D_00455140[], D_00455148[], D_00455150[];
extern int func_004165A8_cmp(char *, char *) __asm__("func_004165A8");
extern char *strcpy_(char *, char *) __asm__("strcpy");
extern unsigned strlen_(char *) __asm__("strlen");
char *_setlocale_r_(int *p, int category, char *locale) __asm__("_setlocale_r");
/**
 * Sets or queries the locale for a category and returns the locale name, or null when it is not supported.
 *
 * Only "C" and "" are accepted, plus "C-JIS", "C-EUCJP" and "C-SJIS" for category 2 (LC_CTYPE). Setting LC_CTYPE (or
 * LC_ALL, 0) saves the previous ctype name in D_00455150, copies the new one to D_00455148 and sets MB_CUR_MAX
 * (D_00455140) to 8 for C-JIS, 2 for the other two-part names, else 1. A successful set records the category (+0x30)
 * and locale (+0x34) in the struct. The second "C" comparison in the LC_CTYPE test repeats the first, as in newlib.
 *
 * @param reent         the reentrancy struct
 * @param category  the locale category
 * @param locale    the new locale, or null to query
 * @return for LC_CTYPE the previous ctype locale name after a set or the current one after a query, else "C"; null for
 *         an unsupported locale
 */
char *_setlocale_r_(int *reent, int category, char *locale) {
    if (locale) {
        if (category != 2) {
            if (func_004165A8_cmp(locale, D_0049A7D0) && func_004165A8_cmp(locale, D_0049A7C0))
                return 0;
            if (category == 0) {
                strcpy_(D_00455150, D_00455148);
                strcpy_(D_00455148, locale);
                *(int *)D_00455140 = 1;
            }
        } else {
            if (func_004165A8_cmp(locale, D_0049A7D0) && func_004165A8_cmp(locale, D_0049A7C0) && func_004165A8_cmp(locale, D_0049A7D0) &&
                func_004165A8_cmp(locale, D_0049A7D8) && func_004165A8_cmp(locale, D_0049A7E0) &&
                func_004165A8_cmp(locale, D_0049A7E8))
                return 0;
            strcpy_(D_00455150, D_00455148);
            strcpy_(D_00455148, locale);
            if (func_004165A8_cmp(locale, D_0049A7D8) == 0)
                *(int *)D_00455140 = 8;
            else if (strlen_(locale) >= 2)
                *(int *)D_00455140 = 2;
            else
                *(int *)D_00455140 = 1;
        }
        reent[13] = (int)locale;
        reent[0xC] = category;
        return category == 2 ? D_00455150 : D_0049A7D0b;
    }
    return category == 2 ? D_00455148 : D_0049A7D0b;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _localeconv_r);
#ifdef SKIP_ASM
extern char D_0049A790[];
/**
 * Returns the fixed numeric-formatting locale table D_0049A790.
 *
 * The table's decimal point is "." (D_0049A7C8). The reentrancy argument is ignored.
 *
 * @return the locale convention table
 */
char *_localeconv_r(void) {
    return D_0049A790;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", setlocale);
#ifdef SKIP_ASM
extern int D_00454944;
extern void _setlocale_r(int, int, int);
/**
 * Sets or queries the locale for a category, for the global reentrancy struct.
 *
 * Forwards to _setlocale_r with D_00454944. Declared void, but $v0 still holds the name.
 *
 * @param category  the locale category
 * @param locale  the new locale, or null to query
 */
void setlocale(int category, int locale) {
    _setlocale_r(D_00454944, category, locale);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", localeconv);
#ifdef SKIP_ASM
extern int D_00454944;
/**
 * Returns the numeric-formatting locale table, for the global reentrancy struct.
 *
 * Calls _localeconv_r with D_00454944. Declared void, but $v0 still holds the table.
 */
void localeconv(void) {
    ((char *(*)(int))_localeconv_r)(D_00454944);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041EC70);
#ifdef SKIP_ASM
// PORT: PS2-only newlib C library routine; kept as the binary's own instructions (library source is never reconstructed); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_0041EC70\n.ent func_0041EC70\nfunc_0041EC70:\n"
    "sltiu $2, $6, 0x10\n"
    "bnez $2, .L0041ED18\n"
    "andi $5, $5, 0xFF\n"
    "andi $2, $4, 0xF\n"
    "bnez $2, .L0041ED18\n"
    "daddu $7, $4, $0\n"
    "dsll $3, $5, 8\n"
    "ori $2, $0, 0x8080\n"
    "dsll $2, $2, 16\n"
    "ori $2, $2, 0x8080\n"
    "dsll $2, $2, 16\n"
    "ori $2, $2, 0x8080\n"
    "dsll $2, $2, 9\n"
    "ori $2, $2, 0x101\n"
    "daddu $10, $3, $5\n"
    "ori $3, $0, 0x8080\n"
    "dsll $3, $3, 16\n"
    "ori $3, $3, 0x8080\n"
    "dsll $3, $3, 16\n"
    "ori $3, $3, 0x8080\n"
    "dsll $3, $3, 16\n"
    "ori $3, $3, 0x8080\n"
    "pcpyh $8, $10\n"
    "pcpyld $9, $8, $8\n"
    "daddu $4, $2, $0\n"
    "pcpyld $8, $3, $3\n"
    ".L0041ECD8:\n"
    "lq $2, 0x0($7)\n"
    "pxor $2, $2, $9\n"
    "pcpyld $10, $4, $4\n"
    "pnor $3, $0, $2\n"
    "psubb $2, $2, $10\n"
    "pand $2, $2, $3\n"
    "pand $2, $2, $8\n"
    "pcpyud $3, $2, $9\n"
    "or $2, $2, $3\n"
    "bnel $2, $0, .L0041ED18\n"
    "daddu $4, $7, $0\n"
    "addiu $6, $6, -0x10\n"
    "sltiu $2, $6, 0x10\n"
    "beqz $2, .L0041ECD8\n"
    "addiu $7, $7, 0x10\n"
    "daddu $4, $7, $0\n"
    ".L0041ED18:\n"
    "lui $2, (0xFFFFFFFF >> 16)\n"
    "addiu $6, $6, -0x1\n"
    "ori $2, $2, (0xFFFFFFFF & 0xFFFF)\n"
    "beq $6, $2, .L0041ED50\n"
    "nop\n"
    "lui $3, (0xFFFFFFFF >> 16)\n"
    "ori $3, $3, (0xFFFFFFFF & 0xFFFF)\n"
    ".L0041ED34:\n"
    "lbu $2, 0x0($4)\n"
    "beq $2, $5, .L0041ED58\n"
    "addiu $6, $6, -0x1\n"
    "nop\n"
    "nop\n"
    "bne $6, $3, .L0041ED34\n"
    "addiu $4, $4, 0x1\n"
    ".L0041ED50:\n"
    "jr $31\n"
    "daddu $2, $0, $0\n"
    ".L0041ED58:\n"
    "jr $31\n"
    "daddu $2, $4, $0\n"
    ".end func_0041EC70\n"
    ".set at\n.set reorder\n");
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", floor);
#ifdef SKIP_ASM
/* newlib libm floor (fdlibm s_floor.c). The EE has no double hardware: the double travels in one 64-bit GPR and
 * the arithmetic goes through the soft-float helpers (dpadd, dpcmp).
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* newlib libm's word-access helpers (fdlibm.h), little-endian layout, verbatim: the do-while(0) is part of the
   library macro */
typedef union
{
  double value;
  struct
  {
    unsigned int lsw;
    unsigned int msw;
  } parts;
} ieee_double_shape_type_ED60;
#define EXTRACT_WORDS_ED60(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type_ED60 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define INSERT_WORDS_ED60(d,ix0,ix1)				\
do {								\
  ieee_double_shape_type_ED60 iw_u;				\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

static const double huge_ED60 = 1.0e300;

/**
 * Rounds a double down to the nearest integer and returns it.
 *
 * fdlibm bit manipulation on the two 32-bit halves: values below 1 in magnitude become 0 (or -1 for negatives),
 * integral values, infinities and NaNs come back unchanged (NaN and infinity as x + x), and otherwise the fraction bits
 * are cleared after adding one unit for negatives. The 1.0e300 + x tests only raise the inexact flag. The double
 * travels in a 64-bit integer register (soft float). func_0041AF88 calls it.
 *
 * @param x  the value
 * @return the largest integral value not above x
 */
double floor(double x)
{
	int i0,i1,exponent;
	unsigned int i,j;
	EXTRACT_WORDS_ED60(i0,i1,x);
	exponent = ((i0>>20)&0x7ff)-0x3ff;
	if(exponent<20) {
	    if(exponent<0) { 	/* raise inexact if x != 0 */
		if(huge_ED60+x>0.0) {/* return 0*sign(x) if |x|<1 */
		    if(i0>=0) {i0=i1=0;}
		    else if(((i0&0x7fffffff)|i1)!=0)
			{ i0=0xbff00000;i1=0;}
		}
	    } else {
		i = (0x000fffff)>>exponent;
		if(((i0&i)|i1)==0) return x; /* x is integral */
		if(huge_ED60+x>0.0) {	/* raise inexact flag */
		    if(i0<0) i0 += (0x00100000)>>exponent;
		    i0 &= (~i); i1=0;
		}
	    }
	} else if (exponent>51) {
	    if(exponent==0x400) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	} else {
	    i = ((unsigned int)(0xffffffff))>>(exponent-20);
	    if((i1&i)==0) return x;	/* x is integral */
	    if(huge_ED60+x>0.0) { 		/* raise inexact flag */
		if(i0<0) {
		    if(exponent==20) i0+=1;
		    else {
			j = i1+(1<<(52-exponent));
			if(j<i1) i0 +=1 ; 	/* got a carry */
			i1=j;
		    }
		}
		i1 &= (~i);
	    }
	}
	INSERT_WORDS_ED60(x,i0,i1);
	return x;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __ieee754_fmod);
#ifdef SKIP_ASM
// FAKEMATCH: `while((unsigned int)n-- > 0)` for fdlibm's `while(n--)` (the same test for every n); the plain
// form is strength-reduced to `--n != -1`; plain form 97.11%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
/* newlib libm __ieee754_fmod (fdlibm e_fmod.c). The EE has no double hardware: the double travels in one 64-bit
 * GPR and the arithmetic goes through the soft-float helpers.
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* newlib libm's word-access helpers (fdlibm.h), little-endian layout, verbatim: the do-while(0) is part of the
   library macro */
typedef union
{
  double value;
  struct
  {
    unsigned int lsw;
    unsigned int msw;
  } parts;
} ieee_double_shape_type_EF50;
#define EXTRACT_WORDS_EF50(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type_EF50 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define INSERT_WORDS_EF50(d,ix0,ix1)				\
do {								\
  ieee_double_shape_type_EF50 iw_u;				\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

static const double one_EF50 = 1.0;
/* fdlibm's `static const double Zero[] = {0.0, -0.0,};` (e_fmod.c), indexed by the code: retail's copy is D_0049A808 */
extern const double Zero_EF50[] __asm__("D_0049A808");

/**
 * Returns the floating-point remainder of x / y with the sign of x.
 *
 * fdlibm's exact shift-and-subtract remainder: y = 0, a non-finite x or a NaN y give NaN ((x*y)/(x*y)); |x| < |y|
 * returns x; |x| = |y| returns a zero of x's sign from the table D_0049A808. Otherwise the integer exponents are found
 * (subnormals by counting bits), both mantissas are aligned and y is subtracted bit by bit. The doubles travel in
 * 64-bit integer registers (soft float). func_0041AF88 calls it.
 *
 * @param x  the dividend
 * @param y  the divisor
 * @return x - n*y for the integer n that truncates x/y
 */
double __ieee754_fmod(double x, double y)
{
	int n,hx,hy,hz,ix,iy,sx,i;
	unsigned int lx,ly,lz;

	EXTRACT_WORDS_EF50(hx,lx,x);
	EXTRACT_WORDS_EF50(hy,ly,y);
	sx = hx&0x80000000;		/* sign of x */
	hx ^=sx;		/* |x| */
	hy &= 0x7fffffff;	/* |y| */

    /* purge off exception values */
	if((hy|ly)==0||(hx>=0x7ff00000)||	/* y=0,or x not finite */
	  ((hy|((ly|-ly)>>31))>0x7ff00000))	/* or y is NaN */
	    return (x*y)/(x*y);
	if(hx<=hy) {
	    if((hx<hy)||(lx<ly)) return x;	/* |x|<|y| return x */
	    if(lx==ly) 
		return Zero_EF50[(unsigned int)sx>>31];	/* |x|=|y| return x*0*/
	}

    /* determine ix = ilogb(x) */
	if(hx<0x00100000) {	/* subnormal x */
	    if(hx==0) {
		for (ix = -1043, i=lx; i>0; i<<=1) ix -=1;
	    } else {
		for (ix = -1022,i=(hx<<11); i>0; i<<=1) ix -=1;
	    }
	} else ix = (hx>>20)-1023;

    /* determine iy = ilogb(y) */
	if(hy<0x00100000) {	/* subnormal y */
	    if(hy==0) {
		for (iy = -1043, i=ly; i>0; i<<=1) iy -=1;
	    } else {
		for (iy = -1022,i=(hy<<11); i>0; i<<=1) iy -=1;
	    }
	} else iy = (hy>>20)-1023;

    /* set up {hx,lx}, {hy,ly} and align y to x */
	if(ix >= -1022) 
	    hx = 0x00100000|(0x000fffff&hx);
	else {		/* subnormal x, shift x to normal */
	    n = -1022-ix;
	    if(n<=31) {
	        hx = (hx<<n)|(lx>>(32-n));
	        lx <<= n;
	    } else {
		hx = lx<<(n-32);
		lx = 0;
	    }
	}
	if(iy >= -1022) 
	    hy = 0x00100000|(0x000fffff&hy);
	else {		/* subnormal y, shift y to normal */
	    n = -1022-iy;
	    if(n<=31) {
	        hy = (hy<<n)|(ly>>(32-n));
	        ly <<= n;
	    } else {
		hy = ly<<(n-32);
		ly = 0;
	    }
	}

    /* fix point fmod */
	n = ix - iy;
	while((unsigned int)n-- > 0) {
	    hz=hx-hy;lz=lx-ly; if(lx<ly) hz -= 1;
	    if(hz<0){hx = hx+hx+(lx>>31); lx = lx+lx;}
	    else {
	    	if((hz|lz)==0) 		/* return sign(x)*0 */
		    return Zero_EF50[(unsigned int)sx>>31];
	    	hx = hz+hz+(lz>>31); lx = lz+lz;
	    }
	}
	hz=hx-hy;lz=lx-ly; if(lx<ly) hz -= 1;
	if(hz>=0) {hx=hz;lx=lz;}

    /* convert back to floating value and restore the sign */
	if((hx|lx)==0) 			/* return sign(x)*0 */
	    return Zero_EF50[(unsigned int)sx>>31];	
	while(hx<0x00100000) {		/* normalize x */
	    hx = hx+hx+(lx>>31); lx = lx+lx;
	    iy -= 1;
	}
	if(iy>= -1022) {	/* normalize output */
	    hx = ((hx-0x00100000)|((iy+1023)<<20));
	    INSERT_WORDS_EF50(x,hx|sx,lx);
	} else {		/* subnormal output */
	    n = -1022 - iy;
	    if(n<=20) {
		lx = (lx>>n)|((unsigned int)hx<<(32-n));
		hx >>= n;
	    } else if (n<=31) {
		lx = (hx<<(32-n))|(lx>>n); hx = sx;
	    } else {
		lx = hx>>(n-32); hx = sx;
	    }
	    INSERT_WORDS_EF50(x,hx|sx,lx);
	    x *= one_EF50;		/* create necessary signal */
	}
	return x;		/* exact output */
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041F3A8);
#ifdef SKIP_ASM
/* newlib 1.9 libm/math/e_log.c (__ieee754_log) and the fdlibm.h word macros, verbatim:
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */
typedef union {
    double value;
    struct {
        unsigned int lsw;
        unsigned int msw;
    } parts;
} ieee_double_shape_type_0041F3A8;
#define EXTRACT_WORDS_0041F3A8(ix0,ix1,d)			\
do {								\
  ieee_double_shape_type_0041F3A8 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define GET_HIGH_WORD_0041F3A8(i,d)				\
do {								\
  ieee_double_shape_type_0041F3A8 gh_u;				\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)
#define SET_HIGH_WORD_0041F3A8(d,v)				\
do {								\
  ieee_double_shape_type_0041F3A8 sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

static const double
ln2_hi_0041F3A8  =  6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
ln2_lo_0041F3A8  =  1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
two54_0041F3A8   =  1.80143985094819840000e+16,  /* 43500000 00000000 */
Lg1_0041F3A8 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
Lg2_0041F3A8 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
Lg3_0041F3A8 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
Lg4_0041F3A8 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
Lg5_0041F3A8 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
Lg6_0041F3A8 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
Lg7_0041F3A8 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

static const double zero_0041F3A8   =  0.0;

/**
 * Returns the natural logarithm of a double.
 *
 * fdlibm: log(0) is -infinity, a negative argument NaN, infinity and NaN return x + x. Subnormals are scaled by 2^54
 * first. x is split as 2^k * (1 + f) with sqrt(2)/2 < 1 + f < sqrt(2); small f uses a short series, otherwise s = f /
 * (2 + f) feeds the Lg1..Lg7 polynomial, and k*ln2 is added in two parts (ln2_hi, ln2_lo). __ieee754_log10 calls it.
 *
 * @param x  the value
 * @return ln(x)
 */
double func_0041F3A8(double x)
{
	double hfsq,frac,ratio,z,poly,w,evenTerms,oddTerms,dk;
	int k,hx,i,j;
	unsigned int lx;

	EXTRACT_WORDS_0041F3A8(hx,lx,x);

	k=0;
	if (hx < 0x00100000) {			/* x < 2**-1022  */
	    if (((hx&0x7fffffff)|lx)==0) 
		return -two54_0041F3A8/zero_0041F3A8;		/* log(+-0)=-inf */
	    if (hx<0) return (x-x)/zero_0041F3A8;	/* log(-#) = NaN */
	    k -= 54; x *= two54_0041F3A8; /* subnormal number, scale up x */
	    GET_HIGH_WORD_0041F3A8(hx,x);
	} 
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	hx &= 0x000fffff;
	i = (hx+0x95f64)&0x100000;
	SET_HIGH_WORD_0041F3A8(x,hx|(i^0x3ff00000));	/* normalize x or x/2 */
	k += (i>>20);
	frac = x-1.0;
	if((0x000fffff&(2+hx))<3) {	/* |f| < 2**-20 */
          if(frac==zero_0041F3A8) { if(k==0) return zero_0041F3A8;  else {dk=(double)k;
                               return dk*ln2_hi_0041F3A8+dk*ln2_lo_0041F3A8;}}
	    poly = frac*frac*(0.5-0.33333333333333333*frac);
	    if(k==0) return frac-poly; else {dk=(double)k;
	    	     return dk*ln2_hi_0041F3A8-((poly-dk*ln2_lo_0041F3A8)-frac);}
	}
 	ratio = frac/(2.0+frac); 
	dk = (double)k;
	z = ratio*ratio;
	i = hx-0x6147a;
	w = z*z;
	j = 0x6b851-hx;
	evenTerms= w*(Lg2_0041F3A8+w*(Lg4_0041F3A8+w*Lg6_0041F3A8)); 
	oddTerms= z*(Lg1_0041F3A8+w*(Lg3_0041F3A8+w*(Lg5_0041F3A8+w*Lg7_0041F3A8))); 
	i |= j;
	poly = oddTerms+evenTerms;
	if(i>0) {
	    hfsq=0.5*frac*frac;
	    if(k==0) return frac-(hfsq-ratio*(hfsq+poly)); else
		     return dk*ln2_hi_0041F3A8-((hfsq-(ratio*(hfsq+poly)+dk*ln2_lo_0041F3A8))-frac);
	} else {
	    if(k==0) return frac-ratio*(frac-poly); else
		     return dk*ln2_hi_0041F3A8-((ratio*(frac-poly)-dk*ln2_lo_0041F3A8)-frac);
	}
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __ieee754_log10);
#ifdef SKIP_ASM
/* newlib libm __ieee754_log10 (fdlibm e_log10.c). The EE has no double hardware: the double travels in one 64-bit
 * GPR and the arithmetic goes through the soft-float helpers.
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* newlib libm's word-access helpers (fdlibm.h), little-endian layout, verbatim: the do-while(0) is part of the
   library macro */
typedef union
{
  double value;
  struct
  {
    unsigned int lsw;
    unsigned int msw;
  } parts;
} ieee_double_shape_type_F9D8;
#define EXTRACT_WORDS_F9D8(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type_F9D8 ew_u;				\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)
#define GET_HIGH_WORD_F9D8(i,d)					\
do {								\
  ieee_double_shape_type_F9D8 gh_u;				\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)
#define SET_HIGH_WORD_F9D8(d,v)					\
do {								\
  ieee_double_shape_type_F9D8 sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

static const double
two54_F9D8      =  1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
ivln10_F9D8     =  4.34294481903251816668e-01, /* 0x3FDBCB7B, 0x1526E50E */
log10_2hi_F9D8  =  3.01029995663611771306e-01, /* 0x3FD34413, 0x509F6000 */
log10_2lo_F9D8  =  3.69423907715893078616e-13; /* 0x3D59FEF3, 0x11F12B36 */

static const double zero_F9D8   =  0.0;

extern double __ieee754_log_F9D8(double) __asm__("func_0041F3A8");

/**
 * Returns the base-10 logarithm of a double.
 *
 * fdlibm: log10(0) is -infinity, a negative argument NaN, infinity and NaN return x + x; subnormals are scaled by 2^54.
 * It splits off the binary exponent k and returns k*log10(2) (in two parts) plus ivln10 * func_0041F3A8(mantissa).
 * func_0041B728 calls it.
 *
 * @param x  the value
 * @return log10(x)
 */
double __ieee754_log10(double x)
{
	double y,z;
	int i,k,hx;
	unsigned int lx;

	EXTRACT_WORDS_F9D8(hx,lx,x);

        k=0;
        if (hx < 0x00100000) {                  /* x < 2**-1022  */
            if (((hx&0x7fffffff)|lx)==0)
                return -two54_F9D8/zero_F9D8;             /* log(+-0)=-inf */
            if (hx<0) return (x-x)/zero_F9D8;        /* log(-#) = NaN */
            k -= 54; x *= two54_F9D8; /* subnormal number, scale up x */
	    GET_HIGH_WORD_F9D8(hx,x);
        }
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	i  = ((unsigned int)k&0x80000000)>>31;
        hx = (hx&0x000fffff)|((0x3ff-i)<<20);
        y  = (double)(k+i);
	SET_HIGH_WORD_F9D8(x,hx);
	z  = y*log10_2lo_F9D8 + ivln10_F9D8*__ieee754_log_F9D8(x);
	return  z+y*log10_2hi_F9D8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _close_r);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424488(int);
/**
 * Closes a descriptor and copies a failure's errno into a reentrancy struct.
 *
 * Clears the global errno D_004A5BB8, calls func_00424488 and, when it returns -1 with errno set, stores errno through
 * err. func_00424488 always returns -1 without setting errno, so nothing is stored.
 *
 * @param err  the reentrancy struct (its errno, the first field)
 * @param fileDesc    the descriptor
 * @return func_00424488's result (-1)
 */
int _close_r(int err, int fileDesc) {
    int closeResult;
    D_004A5BB8 = 0;
    closeResult = func_00424488(fileDesc);
    if (closeResult == -1 && D_004A5BB8)
        *(int *)err = D_004A5BB8;
    return closeResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _fstat_r);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424558(int, int);
/**
 * Reads a descriptor's file status and copies a failure's errno into a reentrancy struct.
 *
 * Clears the global errno D_004A5BB8, calls func_00424558 and stores errno through err when it returns -1 with errno
 * set. func_00424558 reports every descriptor as a character device.
 *
 * @param err  receives errno on failure
 * @param fileDesc    the descriptor
 * @param statBuf    the stat buffer
 * @return 0, or -1 on failure
 */
int _fstat_r(int *err, int fileDesc, int statBuf) {
    int fstatResult;
    D_004A5BB8 = 0;
    fstatResult = func_00424558(fileDesc, statBuf);
    if (fstatResult == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return fstatResult;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _lseek_r);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424498(int, int, int);
/**
 * Seeks a descriptor and copies a failure's errno into a reentrancy struct.
 *
 * Clears the global errno D_004A5BB8, calls func_00424498 and stores errno through err when it returns -1 with errno
 * set. func_00424498 always returns -1 without setting errno.
 *
 * @param err  receives errno on failure
 * @param fileDesc    the descriptor
 * @param offset    the offset
 * @param whence    the whence code
 * @return the new offset, here always -1
 */
long _lseek_r(int *err, int fileDesc, int offset, int whence) {
    int newOffset;
    D_004A5BB8 = 0;
    newOffset = func_00424498(fileDesc, offset, whence);
    if (newOffset == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return newOffset;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _open_r);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424460(int, int, int);
/**
 * Opens a file descriptor and copies a failure's errno into a reentrancy struct.
 *
 * Clears the global errno D_004A5BB8, calls func_00424460 and stores errno through err when it returns -1 with errno
 * set. func_00424460 always fails (errno 5, EIO), so every open through stdio fails.
 *
 * @param err  receives errno on failure
 * @param path    the path
 * @param openFlags    the open flags
 * @param permissions    the permission bits
 * @return the descriptor, here always -1
 */
int _open_r(int *err, int path, int openFlags, int permissions) {
    int fileDesc;
    D_004A5BB8 = 0;
    fileDesc = func_00424460(path, openFlags, permissions);
    if (fileDesc == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return fileDesc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _read_r);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_004243E8(int, int, int);
/**
 * Reads from a descriptor and copies a failure's errno into a reentrancy struct.
 *
 * Clears the global errno D_004A5BB8, calls func_004243E8 (console input for descriptor 0) and stores errno through err
 * when it returns -1 with errno set.
 *
 * @param err  receives errno on failure
 * @param fileDesc    the descriptor
 * @param buffer    the destination
 * @param byteCount    bytes wanted
 * @return the bytes read, or -1
 */
long _read_r(int *err, int fileDesc, int buffer, int byteCount) {
    int bytesRead;
    D_004A5BB8 = 0;
    bytesRead = func_004243E8(fileDesc, buffer, byteCount);
    if (bytesRead == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return bytesRead;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _sbrk_r);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern unsigned func_004244A0(int);
/**
 * Moves the heap break and copies a failure's errno into a reentrancy struct.
 *
 * Clears the global errno D_004A5BB8, calls func_004244A0 and stores errno through err when it returns all ones with
 * errno set.
 *
 * @param err  receives errno on failure
 * @param increment    the byte change (negative to shrink)
 * @return the previous break, or 0xFFFFFFFF on failure
 */
unsigned _sbrk_r(int *err, int increment) {
    unsigned oldBreak;
    D_004A5BB8 = 0;
    oldBreak = func_004244A0(increment);
    if (oldBreak == 0xFFFFFFFF && D_004A5BB8)
        *err = D_004A5BB8;
    return oldBreak;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _write_r);
#ifdef SKIP_ASM
extern int D_004A5BB8;
extern int func_00424368(int, int, int);
/**
 * Writes to a descriptor and copies a failure's errno into a reentrancy struct.
 *
 * Clears the global errno D_004A5BB8, calls func_00424368 (console output for descriptors 1 and 2) and stores errno
 * through err when it returns -1 with errno set.
 *
 * @param err  receives errno on failure
 * @param fileDesc    the descriptor
 * @param buffer    the data
 * @param byteCount    the byte count
 * @return the bytes written, or -1
 */
long _write_r(int *err, int fileDesc, int buffer, int byteCount) {
    int bytesWritten;
    D_004A5BB8 = 0;
    bytesWritten = func_00424368(fileDesc, buffer, byteCount);
    if (bytesWritten == -1 && D_004A5BB8)
        *err = D_004A5BB8;
    return bytesWritten;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", fflush);
#ifdef SKIP_ASM
/* newlib stdio/fflush.c (fflush), with CHECK_INIT from stdio/local.h and FILE from sys/reent.h:
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
struct _reent_FE30 {
  int _errno;
  struct __sFILE_FE30 *_stdin, *_stdout, *_stderr;
  int  _inc;
  char _emergency[25];
  int _current_category;
  const char *_current_locale;
  int __sdidinit;
};
struct __sbuf_FE30 {
	unsigned char *_base;
	int	_size;
};
struct __sFILE_FE30 {
  unsigned char *_p;	/* current position in (some) buffer */
  int	_r;		/* read space left for getc() */
  int	_w;		/* write space left for putc() */
  short	_flags;		/* flags, below; this FILE is free if 0 */
  short	_file;		/* fileno, if Unix descriptor, else -1 */
  struct __sbuf_FE30 _bf;	/* the buffer (at least 1 byte, if !NULL) */
  int	_lbfsize;	/* 0 or -_bf._size, for inline putc */
  void *_cookie;	/* cookie passed to io functions */
  int	(*_read)(void *_cookie, char *_buf, int _n);
  int	(*_write)(void *_cookie, const char *_buf, int _n);
  long	(*_seek)(void *_cookie, long _offset, int _whence);
  int	(*_close)(void *_cookie);
  struct __sbuf_FE30 _ub;	/* ungetc buffer */
  unsigned char *_up;	/* saved _p when _p is doing ungetc data */
  int	_ur;		/* saved _r when _r is counting ungetc data */
  unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
  unsigned char _nbuf[1];	/* guarantee a getc() buffer */
  struct __sbuf_FE30 _lb;	/* buffer for fgetline() */
  int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
  int	_offset;	/* current lseek offset */
  struct _reent_FE30 *_data;
};
#define __SLBF_FE30	0x0001
#define __SNBF_FE30	0x0002
#define __SWR_FE30	0x0008
#define __SERR_FE30	0x0040
extern struct _reent_FE30 *_impure_ptr_FE30 __asm__("D_00454944");
#define _REENT_FE30 _impure_ptr_FE30
extern void __sinit_FE30(struct _reent_FE30 *) __asm__("__sinit");
extern int _fwalk_FE30(struct _reent_FE30 *, int (*)(struct __sFILE_FE30 *)) __asm__("_fwalk");
/* newlib's CHECK_INIT (stdio/local.h), verbatim: the do-while(0) is part of the library macro */
#define CHECK_INIT_FE30(fp) \
  do					\
    {					\
      if ((fp)->_data == 0)		\
	(fp)->_data = _REENT_FE30;	\
      if (!(fp)->_data->__sdidinit)	\
	__sinit_FE30 ((fp)->_data);	\
    }					\
  while (0)

int fflush_FE30(struct __sFILE_FE30 *fp) __asm__("fflush");
/**
 * Writes out a stream's buffered output (or every stream's, for null) and returns 0 or -1.
 *
 * A null stream walks all streams of D_00454944 with _fwalk and itself. Otherwise it runs the stdio set-up check
 * (__sinit once) and, for a writing stream with a buffer, resets the buffer pointer and write count first, then
 * calls the write callback until the pending bytes are out. A write that returns 0 or less sets the error flag 0x40 and
 * returns -1.
 *
 * @param fp  the stream, or null for all streams
 * @return 0, or -1 when a write failed (for null, the OR of the per-stream results)
 */
int
fflush_FE30 (register struct __sFILE_FE30 * fp)
{
  register unsigned char *cursor;
  register int n, t;

  if (fp == 0)
    return _fwalk_FE30 (_REENT_FE30, fflush_FE30);

  CHECK_INIT_FE30 (fp);

  t = fp->_flags;
  if ((t & __SWR_FE30) == 0 || (cursor = fp->_bf._base) == 0)
    return 0;
  n = fp->_p - cursor;		/* write this much */

  /*
   * Set these immediately to avoid problems with longjmp
   * and to allow exchange buffering (via setvbuf) in user
   * write function.
   */
  fp->_p = cursor;
  fp->_w = t & (__SLBF_FE30 | __SNBF_FE30) ? 0 : fp->_bf._size;

  while (n > 0)
    {
      t = (*fp->_write) (fp->_cookie, (char *) cursor, n);
      if (t <= 0)
	{
	  fp->_flags |= __SERR_FE30;
	  return -1;
	}
      cursor += t;
      n -= t;
    }
  return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_0041FF40);
#ifdef SKIP_ASM
struct F41FF40 {
    int p, r, w;
    short flags, file;
    int base, size_, lbf;
    int cookie;
    void *rd, *wr, *sk, *cl;
    int pad[0x54 / 4 - 12];
    int ent;
};
extern long __sread(), __swrite(), __sseek();
/**
 * Initialises one of the three standard FILE structs.
 *
 * Clears the buffer pointer, counts, buffer base and line-buffer size, stores the flags and descriptor, makes the
 * stream its own cookie, installs the descriptor callbacks __sread, __swrite, __sseek and
 * __sclose, and sets the reentrancy pointer (+0x54). __sinit calls it for the input stream (flags 4,
 * descriptor 0), the output stream (9, 1) and the error stream (10, 2).
 *
 * @param stream  the FILE
 * @param flags   the stream flags
 * @param file    the descriptor
 * @param ent     the reentrancy struct
 */
void func_0041FF40(struct F41FF40 *stream, short flags, short file, int ent) {
    stream->p = 0;
    stream->r = 0;
    stream->w = 0;
    stream->flags = flags;
    stream->file = file;
    stream->base = 0;
    stream->lbf = 0;
    stream->cookie = (int)stream;
    stream->rd = __sread;
    stream->wr = __swrite;
    stream->sk = __sseek;
    stream->cl = (void *)__sclose;
    stream->ent = ent;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sfmoreglue);
#ifdef SKIP_ASM
extern void *func_00416210(void *, int, int);
/**
 * Allocates a new glue block of cnt zeroed FILE structs and returns it, or null.
 *
 * Allocates cnt * 0x58 + 0xC bytes with _malloc_r, sets next to null, the count to cnt and the FILE array pointer
 * to just after the 12-byte header, and zeroes the FILEs with func_00416210.
 *
 * @param reent  the reentrancy struct
 * @param cnt    the number of FILEs
 * @return the glue block, or null when out of memory
 */
int *__sfmoreglue(int reent, int cnt) {
    int sz = cnt * 88;
    int *glue = (int *)_malloc_r(reent, sz + 0xC);
    if (!glue)
        return 0;
    glue[1] = cnt;
    glue[0] = 0;
    glue[2] = (int)(glue + 3);
    func_00416210(glue + 3, 0, sz);
    return glue;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sfp);
#ifdef SKIP_ASM
/* newlib stdio/findfp.c (__sfp), FILE and struct _glue from sys/reent.h:
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
struct __sbuf_0010 {
	unsigned char *_base;
	int	_size;
};
/* newlib 1.9's struct _reent up to __sglue (0x1d8) */
#define NDYNAMIC_0010 4
#define ENOMEM_0010 12
extern void __sinit_0010(struct _reent *) __asm__("__sinit");
extern struct _glue *__sfmoreglue_0010(struct _reent *, int) __asm__("__sfmoreglue");

/* the unit declares __sfp(int) for its caller _fopen_r */
struct __sFILE *__sfp_0010(struct _reent *d) __asm__("__sfp");
/**
 * Finds a free FILE struct, adding more when all are taken, and returns it reset, or null.
 *
 * Runs __sinit once, then scans the glue list for a FILE with flags 0, appending a block of 4 with __sfmoreglue
 * when the list is exhausted. The FILE found is reserved (flags 1) and its pointers, counts, buffers and descriptor
 * (-1) are reset; its reentrancy pointer is set. Out of memory sets errno 12 and returns null.
 *
 * @param reent  the reentrancy struct
 * @return the FILE, or null
 */
struct __sFILE *
__sfp_0010 (struct _reent *reent)
{
  struct __sFILE *stream;
  int n;
  struct _glue *glue;

  if (!reent->__sdidinit)
    __sinit_0010 (reent);
  for (glue = &(*(struct _glue*)&reent->__sglue);; glue = ((struct _glue_0010*)glue->_next))
    {
      for (stream = ((struct __sFILE*)glue->_iobs), n = glue->_niobs; --n >= 0; stream++)
	if (stream->_flags == 0)
	  goto found;
      if (((struct _glue_0010*)glue->_next) == 0 &&
	  ((*(struct _glue_0010**)&glue->_next) = __sfmoreglue_0010 (reent, NDYNAMIC_0010)) == 0)
	break;
    }
  reent->_errno = ENOMEM_0010;
  return 0;

found:
  stream->_flags = 1;		/* reserve this slot; caller sets real flags */
  stream->_p = 0;		/* no current pointer */
  stream->_w = 0;			/* nothing to read or write */
  stream->_r = 0;
  (*(struct __sbuf_0010*)&stream->_bf)._base = 0;		/* no buffer */
  (*(struct __sbuf_0010*)&stream->_bf)._size = 0;
  stream->_lbfsize = 0;		/* not line buffered */
  stream->_file = -1;		/* no file */
  /* fp->_cookie = <any>; */	/* caller sets cookie, _read/_write etc */
  (*(struct __sbuf_0010*)&stream->_ub)._base = 0;		/* no ungetc buffer */
  (*(struct __sbuf_0010*)&stream->_ub)._size = 0;
  (*(struct __sbuf_0010*)&stream->_lb)._base = 0;		/* no line buffer */
  (*(struct __sbuf_0010*)&stream->_lb)._size = 0;
  (*(struct _reent**)&stream->_data) = reent;
  return stream;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sinit);
#ifdef SKIP_ASM
/**
 * Sets up stdio for a reentrancy struct: the cleanup hook and the stdin, stdout and stderr streams.
 *
 * Stores _cleanup_r as the cleanup hook (+0x3C) and sets the initialised flag (+0x38). Initialises the three
 * built-in FILEs at +0x1E4, +0x23C and +0x294 with func_0041FF40 (stdin flags 4 on descriptor 0, stdout 9 on 1, stderr
 * 10 on 2) and makes them the first glue block (next null, count 3). It does not set the stdin, stdout and stderr
 * pointers (+4, +8, +0xC).
 *
 * @param reent  the reentrancy struct
 */
void __sinit(char *reent) {
    char *stdinStream = reent + 0x1E4;
    *(void **)(reent + 0x3C) = (void *)_cleanup_r;
    *(int *)(reent + 0x38) = 1;
    func_0041FF40((void *)stdinStream, 4, 0, (int)reent);
    func_0041FF40((void *)(reent + 0x23C), 9, 1, (int)reent);
    func_0041FF40((void *)(reent + 0x294), 10, 2, (int)reent);
    *(char **)(reent + 0x1E0) = stdinStream;
    *(int *)(reent + 0x1DC) = 3;
    *(int *)(reent + 0x1D8) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __sflags);
#ifdef SKIP_ASM
/* newlib stdio/flags.c (__sflags), as in Sony's copy: no mode[1] guard and no O_BINARY/O_TEXT part.
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "fakematch.h"
#define __SRD_0180	0x0004
#define __SWR_0180	0x0008
#define __SRW_0180	0x0010
#define __SAPP_0180	0x0100
#define O_RDONLY_0180	0
#define O_WRONLY_0180	1
#define O_RDWR_0180	2
#define O_APPEND_0180	0x0008
#define O_CREAT_0180	0x0200
#define O_TRUNC_0180	0x0400
#define EINVAL_0180	22

/* the unit declares __sflags(int, char *, int *) for its caller _fopen_r */
int __sflags_0180(struct _reent *ptr, register char *mode, int *optr) __asm__("__sflags");
/**
 * Converts a file-open mode string into stdio flags and open flags.
 *
 * "r" gives read (4) with O_RDONLY, "w" write (8) with O_WRONLY|O_CREAT|O_TRUNC, "a" write+append (0x108) with
 * O_WRONLY|O_CREAT|O_APPEND; a + in the second or third character switches to read-write (0x10, O_RDWR). Any other
 * first character sets errno 22 and returns 0.
 *
 * Quirk: mode[2] is read even for a one-character mode, one byte past its terminator.
 *
 * @param ptr   the reentrancy struct (receives errno)
 * @param mode  the mode string
 * @param optr  receives the open flags
 * @return the stdio flags, or 0 for an invalid mode
 */
// FAKEMATCH: fakematch.h pins on all four locals (ret $9, m $7, o $8, and the `m | o` result r $3; retail keeps
// them off v0/a0, as if ptr stayed live, which no spelling reproduced; every 3-pin subset scores 98.93-99.64);
// plain form 97.62% (97.86% honest best with `*optr = o | m`).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int
__sflags_0180 (struct _reent *ptr, register char *mode, int *optr)
{
  register int openFlags FM_REG("$3");
  register int ret FM_REG("$9");
  register int accessMode FM_REG("$7");
  register int extraFlags FM_REG("$8");

  switch (mode[0])
    {
    case 'r':			/* open for reading */
      ret = __SRD_0180;
      accessMode = O_RDONLY_0180;
      extraFlags = 0;
      break;

    case 'w':			/* open for writing */
      ret = __SWR_0180;
      accessMode = O_WRONLY_0180;
      extraFlags = O_CREAT_0180 | O_TRUNC_0180;
      break;

    case 'a':			/* open for appending */
      ret = __SWR_0180 | __SAPP_0180;
      accessMode = O_WRONLY_0180;
      extraFlags = O_CREAT_0180 | O_APPEND_0180;
      break;
    default:			/* illegal mode */
      ptr->_errno = EINVAL_0180;
      return (0);
    }
  if (mode[1] == '+' || mode[2] == '+')
    {
      ret = __SRW_0180;
      accessMode = O_RDWR_0180;
    }
  openFlags = accessMode | extraFlags;
  *optr = openFlags;
  return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00420228);
#ifdef SKIP_ASM
/**
 * Copies n bytes from src to dst, one byte at a time.
 *
 * fread uses it in place of memcpy.
 *
 * @param dst  the destination
 * @param src  the source
 * @param n    the byte count
 */
void func_00420228(char *dst, char *src, unsigned n) {
    while (n--)
        *dst++ = *src++;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", fread);
#ifdef SKIP_ASM
extern void func_00420228(char *, char *, unsigned);
extern int __srefill(int *);
/**
 * Reads count items of size bytes from a stream into a buffer and returns the number of whole items read.
 *
 * Copies what is buffered with func_00420228, refilling with __srefill while more is wanted; a failed refill
 * returns the whole items read so far. A negative read count is treated as 0.
 *
 * @param buf     the destination
 * @param size    the item size
 * @param count   the item count
 * @param stream  the stream
 * @return count, or fewer on end of file or error (0 when size * count is 0)
 */
unsigned fread(char *buf, unsigned size, unsigned count, int *stream) {
    unsigned resid, total, avail;
    char *dst;
    if ((resid = count * size) == 0)
        return 0;
    if (stream[1] < 0)
        stream[1] = 0;
    total = resid;
    dst = buf;
    while (resid > (avail = stream[1])) {
        func_00420228(dst, (char *)stream[0], avail);
        dst += avail;
        stream[0] = stream[0] + avail;
        resid -= avail;
        if (__srefill(stream))
            return (total - resid) / size;
    }
    func_00420228(dst, (char *)stream[0], resid);
    stream[1] -= resid;
    stream[0] += resid;
    return count;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", ungetc);
#ifdef SKIP_ASM
/* newlib stdio/ungetc.c (ungetc), as in Sony's copy: a full ungetc buffer is reported on fd 1 instead of being
 * grown with __submore. CHECK_INIT from stdio/local.h, FILE from sys/reent.h:
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
struct _reent_0378 {
  int _errno;
  struct __sFILE_0378 *_stdin, *_stdout, *_stderr;
  int  _inc;
  char _emergency[25];
  int _current_category;
  const char *_current_locale;
  int __sdidinit;
};
struct __sbuf_0378 {
	unsigned char *_base;
	int	_size;
};
struct __sFILE_0378 {
  unsigned char *_p;	/* current position in (some) buffer */
  int	_r;		/* read space left for getc() */
  int	_w;		/* write space left for putc() */
  short	_flags;		/* flags, below; this FILE is free if 0 */
  short	_file;		/* fileno, if Unix descriptor, else -1 */
  struct __sbuf_0378 _bf;	/* the buffer (at least 1 byte, if !NULL) */
  int	_lbfsize;	/* 0 or -_bf._size, for inline putc */
  void *_cookie;	/* cookie passed to io functions */
  int	(*_read)(void *_cookie, char *_buf, int _n);
  int	(*_write)(void *_cookie, const char *_buf, int _n);
  long	(*_seek)(void *_cookie, long _offset, int _whence);
  int	(*_close)(void *_cookie);
  struct __sbuf_0378 _ub;	/* ungetc buffer */
  unsigned char *_up;	/* saved _p when _p is doing ungetc data */
  int	_ur;		/* saved _r when _r is counting ungetc data */
  unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
  unsigned char _nbuf[1];	/* guarantee a getc() buffer */
  struct __sbuf_0378 _lb;	/* buffer for fgetline() */
  int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
  int	_offset;	/* current lseek offset */
  struct _reent_0378 *_data;
};
#define __SRD_0378	0x0004
#define __SWR_0378	0x0008
#define __SRW_0378	0x0010
#define __SEOF_0378	0x0020
#define EOF_0378	(-1)
#define HASUB_0378(fp) ((fp)->_ub._base != 0)
extern struct _reent_0378 *_impure_ptr_0378 __asm__("D_00454944");
#define _REENT_0378 _impure_ptr_0378
extern void __sinit_0378(struct _reent_0378 *) __asm__("__sinit");
extern int fflush_0378(struct __sFILE_0378 *) __asm__("fflush");
extern int write_0378(int, const char *, int) __asm__("func_00424368");
/* newlib's CHECK_INIT (stdio/local.h), verbatim: the do-while(0) is part of the library macro */
#define CHECK_INIT_0378(fp) \
  do					\
    {					\
      if ((fp)->_data == 0)		\
	(fp)->_data = _REENT_0378;	\
      if (!(fp)->_data->__sdidinit)	\
	__sinit_0378 ((fp)->_data);	\
    }					\
  while (0)
#define UNGETC_OVERFLOW_0378 "\nungetc error: *static buffer overflow* - char not added to buffer\n"

/**
 * Pushes a character back onto a stdio stream (newlib's ungetc), so that the next read returns it.
 *
 * EOF is refused, and so is a stream that is neither reading nor read-write; a read-write stream that was
 * writing is flushed, and the stream is switched to reading. The character goes back into the read buffer
 * when it equals the previous byte, otherwise into the 3-byte pushback buffer; Sony's copy reports a full
 * pushback buffer on fd 1 instead of growing it.
 *
 * @param ch  the character to push back (converted to unsigned char)
 * @param fp  the stream
 * @return the pushed-back character, or EOF (-1) on failure
 */
int
ungetc (int ch, register struct __sFILE_0378 *fp)
{
  if (ch == EOF_0378)
    return (EOF_0378);

  /* Ensure stdio has been initialized.
     ??? Might be able to remove this as some other stdio routine should
     have already been called to get the char we are un-getting.  */

  CHECK_INIT_0378 (fp);

  /* After ungetc, we won't be at eof anymore */
  fp->_flags &= ~__SEOF_0378;

  if ((fp->_flags & __SRD_0378) == 0)
    {
      /*
       * Not already reading: no good unless reading-and-writing.
       * Otherwise, flush any current write stuff.
       */
      if ((fp->_flags & __SRW_0378) == 0)
	return EOF_0378;
      if (fp->_flags & __SWR_0378)
	{
	  if (fflush_0378 (fp))
	    return EOF_0378;
	  fp->_flags &= ~__SWR_0378;
	  fp->_w = 0;
	  fp->_lbfsize = 0;
	}
      fp->_flags |= __SRD_0378;
    }
  ch = (unsigned char) ch;

  /*
   * If we are in the middle of ungetc'ing, just continue.
   */

  if (HASUB_0378 (fp))
    {
      if (fp->_r >= fp->_ub._size)
	{
	  write_0378 (1, UNGETC_OVERFLOW_0378, sizeof (UNGETC_OVERFLOW_0378));
	  return EOF_0378;
	}
      *--fp->_p = ch;
      fp->_r++;
      return ch;
    }

  /*
   * If we can handle this by simply backing up, do so,
   * but never replace the original character.
   * (This makes sscanf() work when scanning `const' data.)
   */

  if (fp->_bf._base != 0 && fp->_p > fp->_bf._base && fp->_p[-1] == ch)
    {
      fp->_p--;
      fp->_r++;
      return ch;
    }

  /*
   * Create an ungetc buffer.
   * Initially, we will use the `reserve' buffer.
   */

  fp->_ur = fp->_r;
  fp->_up = fp->_p;
  fp->_ub._base = fp->_ubuf;
  fp->_ub._size = sizeof (fp->_ubuf);
  fp->_ubuf[sizeof (fp->_ubuf) - 1] = ch;
  fp->_p = &fp->_ubuf[sizeof (fp->_ubuf) - 1];
  fp->_r = 1;
  return ch;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _Balloc);
#ifdef SKIP_ASM
extern int *_calloc_r(int *, int, int);
/**
 * Allocates a Bigint of 2^k words, reusing one from the reentrancy struct's free list when it can.
 *
 * Creates the 16-entry free-list table (+0x4C) with _calloc_r on first use. A list entry is unlinked and reused;
 * otherwise a new Bigint of 0x18 + (2^k - 1) * 4 bytes is allocated with _calloc_r and its k and maxwds set. sign
 * and wds are cleared.
 *
 * @param ep_  the reentrancy struct
 * @param k    the size class (2^k words)
 * @return the Bigint, or null when out of memory
 */
int *_Balloc(int ep_, int k) {
    int *ep = (int *)ep_;
    int x;
    int *rv;
    if (ep[76 / 4] == 0) {
        ep[76 / 4] = (int)_calloc_r(ep, 4, 16);
        if (ep[76 / 4] == 0)
            return 0;
    }
    rv = ((int **)ep[0x4C / 4])[k];
    if (rv) {
        ((int **)ep[0x4C / 4])[k] = (int *)*rv;
    } else {
        x = 1 << k;
        rv = _calloc_r(ep, 1, 0x18 + (x - 1) * 4);
        if (rv == 0)
            return 0;
        rv[1] = k;
        rv[2] = x;
    }
    rv[3] = rv[4] = 0;
    return rv;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _Bfree);
#ifdef SKIP_ASM
/**
 * Returns a Bigint to its size class's free list in the reentrancy struct.
 *
 * A null Bigint is ignored. The reentrancy struct is the first argument (self).
 *
 * @param bigint  the Bigint, or null
 */
void _Bfree(int *self, int *bigint) {
    if (bigint) {
        int i = bigint[1];
        bigint[0] = ((int *)self[0x4C / 4])[i];
        ((int **)self[0x4C / 4])[i] = bigint;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _multadd);
#ifdef SKIP_ASM
struct Bigint {
    struct Bigint *next;
    int k, maxwds, sign, wds;
    unsigned x[1];
};
extern int *_Balloc(int, int);
extern void _Bfree(int *, int *);
extern void *func_0041605C(void *, void *, int);
/**
 * Multiplies a Bigint by a small integer and adds another, growing it when needed, and returns the result.
 *
 * Works in 16-bit halves of each word. A carry out of the top word becomes a new word; when the Bigint is full it is
 * copied into one of the next size class (_Balloc, func_0041605C) and the old one freed with _Bfree.
 *
 * @param ptr         the reentrancy struct
 * @param bigint      the Bigint (changed in place unless it grows)
 * @param multiplier  the factor
 * @param addend      the value added
 * @return the Bigint holding bigint * multiplier + addend
 */
struct Bigint *_multadd(int ptr, struct Bigint *bigint, int multiplier, int addend) {
    int i, wds;
    unsigned *x, y, xi, z, carry;
    struct Bigint *larger;
    wds = bigint->wds;
    x = bigint->x;
    i = 0;
    carry = addend;
    do {
        xi = *x;
        y = (xi & 0xffff) * multiplier + carry;
        z = (xi >> 16) * multiplier + (y >> 16);
        carry = z >> 16;
        *x++ = (z << 16) + (y & 0xffff);
    } while (++i < wds);
    if (carry) {
        if (wds >= bigint->maxwds) {
            larger = (struct Bigint *)_Balloc(ptr, bigint->k + 1);
            func_0041605C(&larger->sign, &bigint->sign, bigint->wds * 4 + 8);
            _Bfree((int *)ptr, (int *)bigint);
            bigint = larger;
        }
        bigint->x[wds++] = carry;
        bigint->wds = wds;
    }
    return bigint;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _s2b);
#ifdef SKIP_ASM
extern struct Bigint *_multadd(int, struct Bigint *, int, int);
/**
 * Builds a Bigint from a decimal digit string and returns it.
 *
 * Starts from the value of the first nine digits (passed in, y9) and multiplies in each further digit with
 * _multadd, skipping the decimal point that follows the integer digits.
 *
 * @param ptr     the reentrancy struct
 * @param digits  the digit string
 * @param nd0     the digits before the point
 * @param nd      the total digit count
 * @param y9      the value of the first nine digits
 * @return the Bigint
 */
struct Bigint *_s2b(int ptr, char *digits, int nd0, int nd, int y9) {
    struct Bigint *bigint;
    int i, k;
    int x, y;
    x = (nd + 8) / 9;
    for (k = 0, y = 1; x > y; y <<= 1, k++)
        ;
    bigint = (struct Bigint *)_Balloc(ptr, k);
    bigint->x[0] = y9;
    bigint->wds = 1;
    i = 9;
    if (9 < nd0) {
        digits += 9;
        do
            bigint = _multadd(ptr, bigint, 10, *digits++ - '0');
        while (++i < nd0);
        digits++;
    } else
        digits += 10;
    for (; i < nd; i++)
        bigint = _multadd(ptr, bigint, 10, *digits++ - '0');
    return bigint;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _hi0bits);
#ifdef SKIP_ASM
/**
 * Returns the number of leading zero bits in a 32-bit word (32 for 0).
 *
 * A binary search by halves; callers include _d2b, _b2d and _dtoa_r.
 *
 * @param x  the word
 * @return the leading zero count
 */
int _hi0bits(unsigned x) {
    int k = 0;
    if (!(x & 0xffff0000)) {
        k = 16;
        x <<= 16;
    }
    if (!(x & 0xff000000)) {
        k += 8;
        x <<= 8;
    }
    if (!(x & 0xf0000000)) {
        k += 4;
        x <<= 4;
    }
    if (!(x & 0xc0000000)) {
        k += 2;
        x <<= 2;
    }
    if (!(x & 0x80000000)) {
        k++;
        if (!(x & 0x40000000))
            return 32;
    }
    return k;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _lo0bits);
#ifdef SKIP_ASM
/**
 * Shifts the trailing zero bits out of a word in place and returns how many there were (32 for 0).
 *
 * A zero word is left unchanged. _d2b calls it.
 *
 * @param y  the word, shifted right in place
 * @return the trailing zero count
 */
int _lo0bits(unsigned *y) {
    int k;
    unsigned x = *y;
    if (x & 7) {
        if (x & 1)
            return 0;
        if (x & 2) {
            *y = x >> 1;
            return 1;
        }
        *y = x >> 2;
        return 2;
    }
    k = 0;
    if (!(x & 0xffff)) {
        k = 16;
        x >>= 16;
    }
    if (!(x & 0xff)) {
        k += 8;
        x >>= 8;
    }
    if (!(x & 0xf)) {
        k += 4;
        x >>= 4;
    }
    if (!(x & 3)) {
        k += 2;
        x >>= 2;
    }
    if (!(x & 1)) {
        k++;
        x >>= 1;
        if (!x)
            return 32;
    }
    *y = x;
    return k;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _i2b);
#ifdef SKIP_ASM
extern int *_Balloc(int, int);
/**
 * Returns a new one-word Bigint holding a small integer.
 *
 * Allocates with _Balloc (size class 1) and sets x[0] and wds = 1.
 *
 * @param reent  the reentrancy struct
 * @param value  the value
 * @return the Bigint
 */
int *_i2b(int reent, int value) {
    int *bigint = _Balloc(reent, 1);
    bigint[5] = value;
    bigint[4] = 1;
    return bigint;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _multiply);
#ifdef SKIP_ASM
extern int *_Balloc(int, int);
/**
 * Multiplies two Bigints and returns a new Bigint with the product.
 *
 * Schoolbook multiplication in 16-bit halves; the longer operand is the outer one. The result is allocated with
 * _Balloc one size class up when needed and trimmed of leading zero words.
 *
 * @param ptr  the reentrancy struct
 * @param lhs  one factor
 * @param rhs  the other factor
 * @return the product
 */
struct Bigint *_multiply(int ptr, struct Bigint *lhs, struct Bigint *rhs) {
    struct Bigint *product;
    int i, k, wa, wb, wc;
    unsigned *x, *xa, *xae, *xb, *xbe, *xc, *xc0;
    unsigned y, carry, z, z2;
    if (lhs->wds < rhs->wds) {
        product = lhs;
        lhs = rhs;
        rhs = product;
    }
    k = lhs->k;
    wa = lhs->wds;
    wb = rhs->wds;
    wc = wa + wb;
    if (wc > lhs->maxwds)
        k++;
    product = (struct Bigint *)_Balloc(ptr, k);
    for (x = product->x, xa = x + wc; x < xa; x++)
        *x = 0;
    xa = lhs->x;
    xae = xa + wa;
    xb = rhs->x;
    xbe = xb + wb;
    xc0 = product->x;
    for (; xb < xbe; xb++, xc0++) {
        if ((y = *xb & 0xffff)) {
            x = xa;
            xc = xc0;
            carry = 0;
            do {
                z = (*x & 0xffff) * y + (*xc & 0xffff) + carry;
                carry = z >> 16;
                z2 = (*x++ >> 16) * y + (*xc >> 16) + carry;
                carry = z2 >> 16;
                ((unsigned short *)xc)[1] = z2;
                ((unsigned short *)xc)[0] = z;
                xc++;
            } while (x < xae);
            *xc = carry;
        }
        if ((y = *xb >> 16)) {
            x = xa;
            xc = xc0;
            carry = 0;
            z2 = *xc;
            do {
                z = (*x & 0xffff) * y + (*xc >> 16) + carry;
                carry = z >> 16;
                ((unsigned short *)xc)[1] = z;
                ((unsigned short *)xc)[0] = z2;
                xc++;
                z2 = (*x++ >> 16) * y + (*xc & 0xffff) + carry;
                carry = z2 >> 16;
            } while (x < xae);
            *xc = z2;
        }
    }
    for (xc0 = product->x, xc = xc0 + wc; wc > 0 && !*--xc; --wc)
        ;
    product->wds = wc;
    return product;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _pow5mult);
#ifdef SKIP_ASM
extern struct Bigint *_multadd(int, struct Bigint *, int, int);
extern struct Bigint *_multiply(int, struct Bigint *, struct Bigint *);
extern int *_i2b(int, int);
extern void _Bfree(int *, int *);
extern int D_0049A8E8[];
/**
 * Multiplies a Bigint by 5^k and returns the result.
 *
 * k mod 4 is applied with _multadd and the table D_0049A8E8 (5, 25, 125). The rest uses the powers 5^4, 5^8,
 * 5^16... cached as a linked list in the reentrancy struct (+0x48, created with _i2b(625) and extended by
 * squaring with _multiply); each partial product frees the previous one with _Bfree.
 *
 * @param ptr  the reentrancy struct
 * @param number    the Bigint (freed when replaced)
 * @param k    the power of 5
 * @return the product
 */
struct Bigint *_pow5mult(int ptr, struct Bigint *number, int k) {
    struct Bigint *product, *fivePower, *nextFivePower;
    int i;
    if ((i = k & 3))
        number = _multadd(ptr, number, D_0049A8E8[i - 1], 0);
    if (!(k >>= 2))
        return number;
    if (!(fivePower = ((struct Bigint **)ptr)[0x48 / 4])) {
        fivePower = ((struct Bigint **)ptr)[0x48 / 4] = (struct Bigint *)_i2b(ptr, 625);
        fivePower->next = 0;
    }
    for (;;) {
        if (k & 1) {
            product = _multiply(ptr, number, fivePower);
            _Bfree((int *)ptr, (int *)number);
            number = product;
        }
        if (!(k >>= 1))
            break;
        if (!(nextFivePower = fivePower->next)) {
            nextFivePower = fivePower->next = _multiply(ptr, fivePower, fivePower);
            nextFivePower->next = 0;
        }
        fivePower = nextFivePower;
    }
    return number;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _lshift);
#ifdef SKIP_ASM
extern int *_Balloc(int, int);
extern void _Bfree(int *, int *);
/**
 * Shifts a Bigint left by k bits into a new Bigint, frees the old one and returns the new one.
 *
 * Whole words become leading zero words; the bit remainder is shifted across words. The size class grows until the
 * words fit. The input is freed with _Bfree.
 *
 * @param ptr     the reentrancy struct
 * @param bigint  the Bigint (freed)
 * @param k       the shift in bits
 * @return the shifted Bigint
 */
struct Bigint *_lshift(int ptr, struct Bigint *bigint, int k) {
    int i, k1, n, newWords;
    struct Bigint *shifted;
    unsigned *x, *x1, *xe, z;
    n = k >> 5;
    k1 = bigint->k;
    newWords = n + bigint->wds + 1;
    for (i = bigint->maxwds; newWords > i; i <<= 1)
        k1++;
    shifted = (struct Bigint *)_Balloc(ptr, k1);
    x1 = shifted->x;
    for (i = 0; i < n; i++)
        *x1++ = 0;
    x = bigint->x;
    xe = x + bigint->wds;
    if (k &= 0x1f) {
        k1 = 32 - k;
        z = 0;
        do {
            *x1++ = *x << k | z;
            z = *x++ >> k1;
        } while (x < xe);
        if ((*x1 = z))
            ++newWords;
    } else
        do
            *x1++ = *x++;
        while (x < xe);
    shifted->wds = newWords - 1;
    _Bfree((int *)ptr, (int *)bigint);
    return shifted;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __mcmp);
#ifdef SKIP_ASM
struct Bigint28 {
    struct Bigint28 *next;
    int k, maxwds, sign, wds;
    unsigned x[1];
};
/**
 * Compares two non-negative Bigints and returns their order.
 *
 * A difference in word count decides first; otherwise words are compared from the top.
 *
 * @param lhs  the first Bigint
 * @param rhs  the second Bigint
 * @return a negative, zero or positive value as lhs is smaller, equal or larger (the word-count difference, or -1/1)
 */
int __mcmp(struct Bigint28 *lhs, struct Bigint28 *rhs) {
    unsigned *xa, *xa0, *xb, *xb0;
    int i, j;
    i = lhs->wds;
    j = rhs->wds;
    if (i -= j)
        return i;
    xa0 = lhs->x;
    xa = xa0 + j;
    xb0 = rhs->x;
    xb = xb0 + j;
    for (;;) {
        if (*--xa != *--xb)
            return *xa < *xb ? -1 : 1;
        if (xa <= xa0)
            break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", __mdiff);
#ifdef SKIP_ASM
extern int *_Balloc(int, int);
/**
 * Returns a new Bigint holding |lhs - rhs|, with sign set when rhs was larger.
 *
 * Equal values give a one-word zero. Otherwise the smaller is subtracted from the larger in 16-bit halves with borrow,
 * and leading zero words are trimmed. Allocates with _Balloc; compares with __mcmp.
 *
 * @param ptr  the reentrancy struct
 * @param lhs  the minuend
 * @param rhs  the subtrahend
 * @return the difference
 */
struct Bigint *__mdiff(int ptr, struct Bigint *lhs, struct Bigint *rhs) {
    struct Bigint *difference;
    int i, wa, wb;
    int borrow, y, z;
    unsigned *xa, *xae, *xb, *xbe, *xc;
    i = __mcmp((struct Bigint28 *)lhs, (struct Bigint28 *)rhs);
    if (!i) {
        difference = (struct Bigint *)_Balloc(ptr, 0);
        difference->wds = 1;
        difference->x[0] = 0;
        return difference;
    }
    if (i < 0) {
        difference = lhs;
        lhs = rhs;
        rhs = difference;
        i = 1;
    } else
        i = 0;
    difference = (struct Bigint *)_Balloc(ptr, lhs->k);
    difference->sign = i;
    wa = lhs->wds;
    xa = lhs->x;
    xae = xa + wa;
    wb = rhs->wds;
    xb = rhs->x;
    xbe = xb + wb;
    xc = difference->x;
    borrow = 0;
    do {
        y = (*xa & 0xffff) - (*xb & 0xffff) + borrow;
        borrow = y >> 16;
        z = (*xa++ >> 16) - (*xb++ >> 16) + borrow;
        borrow = z >> 16;
        ((unsigned short *)xc)[1] = z;
        ((unsigned short *)xc)[0] = y;
        xc++;
    } while (xb < xbe);
    while (xa < xae) {
        y = (*xa & 0xffff) + borrow;
        borrow = y >> 16;
        z = (*xa++ >> 16) + borrow;
        borrow = z >> 16;
        ((unsigned short *)xc)[1] = z;
        ((unsigned short *)xc)[0] = y;
        xc++;
    }
    while (!*--xc)
        wa--;
    difference->wds = wa;
    return difference;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _ulp);
#ifdef SKIP_ASM
union double_union {
    double d;
    struct {
        unsigned long lo : 32;
        unsigned long hi : 32;
    } w;
    long l;
};
#define word0(x) x.w.hi
#define word1(x) x.w.lo
/**
 * Returns the value of one unit in the last place of a double.
 *
 * Builds 2^(exponent - 52) directly from the exponent bits, with the subnormal cases handled in the high or low word.
 * _strtod_r calls it.
 *
 * @param _x  the value
 * @return the ulp of x
 */
double _ulp(double _x) {
    union double_union x, a;
    int ulpExp;
    x.d = _x;
    ulpExp = (word0(x) & 0x7ff00000) - 52 * 0x100000;
    if (ulpExp > 0) {
        word0(a) = ulpExp;
        word1(a) = 0;
    } else {
        ulpExp = -ulpExp >> 20;
        if (ulpExp < 20) {
            word0(a) = 0x80000 >> ulpExp;
            word1(a) = 0;
        } else {
            word0(a) = 0;
            ulpExp -= 20;
            word1(a) = ulpExp >= 31 ? 1 : 1 << (31 - ulpExp);
        }
    }
    return a.d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _b2d);
#ifdef SKIP_ASM
extern int _hi0bits(unsigned);
/**
 * Converts the top bits of a Bigint to a double in [1, 2) and returns it, with the binary exponent in *expOut.
 *
 * Takes the top word and up to two more, normalised with _hi0bits. _ratio calls it.
 *
 * @param bigint  the Bigint
 * @param expOut  receives the number of significant bits in the top word
 * @return the mantissa as a double in [1, 2)
 */
double _b2d(struct Bigint *bigint, int *expOut) {
    unsigned *xa, *xa0, w, y, z;
    int k;
    union double_union result;
    xa0 = bigint->x;
    xa = xa0 + bigint->wds;
    y = *--xa;
    k = _hi0bits(y);
    *expOut = 32 - k;
    if (k < 11) {
        result.w.hi = 0x3ff00000 | y >> (11 - k);
        w = xa > xa0 ? *--xa : 0;
        result.w.lo = y << (21 + k) | w >> (11 - k);
        goto ret_d;
    }
    z = xa > xa0 ? *--xa : 0;
    if (k -= 11) {
        result.w.hi = 0x3ff00000 | y << k | z >> (32 - k);
        y = xa > xa0 ? *--xa : 0;
        result.w.lo = z << k | y >> (32 - k);
    } else {
        result.w.hi = 0x3ff00000 | y;
        result.w.lo = z;
    }
ret_d:
    return result.d;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _d2b);
#ifdef SKIP_ASM
extern int *_Balloc(int, int);
extern int _lo0bits(unsigned *);
extern int _hi0bits(unsigned);
/**
 * Converts a double to a Bigint holding its integer mantissa and returns it.
 *
 * Strips the trailing zero bits (_lo0bits) into the exponent. *expOut gets the binary exponent of the lowest bit
 * and *bits the number of significant bits (counted with _hi0bits for subnormals).
 *
 * @param ptr     the reentrancy struct
 * @param _d      the value (positive)
 * @param expOut  receives the binary exponent
 * @param bits    receives the significant bit count
 * @return the mantissa Bigint
 */
struct Bigint *_d2b(int ptr, double _d, int *expOut, int *bits) {
    struct Bigint *bigint;
    int de, i, k;
    unsigned *x, y, z;
    union double_union value;
    value.d = _d;
    bigint = (struct Bigint *)_Balloc(ptr, 1);
    x = bigint->x;
    z = value.w.hi & 0xfffff;
    value.w.hi &= 0x7fffffff;
    if ((de = (int)(value.w.hi >> 20)))
        z |= 0x100000;
    if ((y = value.w.lo)) {
        if ((k = _lo0bits(&y))) {
            x[0] = y | z << (32 - k);
            z >>= k;
        } else
            x[0] = y;
        i = bigint->wds = (x[1] = z) ? 2 : 1;
    } else {
        k = _lo0bits(&z);
        x[0] = z;
        i = bigint->wds = 1;
        k += 32;
    }
    if (de) {
        *expOut = de - 1023 - 52 + k;
        *bits = 53 - k;
    } else {
        *expOut = de - 1023 - 52 + 1 + k;
        *bits = 32 * i - _hi0bits(x[i - 1]);
    }
    return bigint;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _ratio);
#ifdef SKIP_ASM
/* newlib stdlib/mprec.c (ratio), with the unit's struct Bigint, union double_union and word0 (_ulp..):
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
#define Exp_msk1_1400 0x100000
/* PORT: the soft-float double divide (gcc's dpdiv libcall), called by name: retail's symbol for it is
   dpdiv (as in seg_325220's dpdiv(x, 10.0)); on PC this is just da.d / db.d */
extern double dpdiv_1400(double, double) __asm__("dpdiv");

/**
 * Returns the ratio of two Bigints as a double.
 *
 * Converts both with _b2d, moves the exponent difference into one of the two doubles and divides with dpdiv.
 * _strtod_r calls it.
 *
 * @param numerator    the dividend
 * @param denominator  the divisor
 * @return numerator / denominator
 */
double _ratio(struct Bigint *numerator, struct Bigint *denominator)
{
  union double_union da, db;
  int k, ka, kb;

  da.d = _b2d (numerator, &ka);
  db.d = _b2d (denominator, &kb);
  k = ka - kb + 32 * (numerator->wds - denominator->wds);
  if (k > 0)
    word0 (da) += k * Exp_msk1_1400;
  else
    {
      k = -k;
      word0 (db) += k * Exp_msk1_1400;
    }
  return dpdiv_1400 (da.d, db.d);
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _mprec_log10);
#ifdef SKIP_ASM
extern double D_0049A8F8[];
extern double dpmul(double, double);
/**
 * Returns 10 to the power n as a double.
 *
 * Powers below 24 come from the table D_0049A8F8; larger ones multiply 1.0 by 10 n times with dpmul. _gcvt calls it.
 *
 * @param n  the power (not negative)
 * @return 10^n
 */
double _mprec_log10(int n) {
    double powerOfTen = 1.0;
    if (n < 24)
        return D_0049A8F8[n];
    if (n > 0) {
        do {
            powerOfTen = dpmul(powerOfTen, 10.0);
        } while (--n > 0);
    }
    return powerOfTen;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", print_f);
#ifdef SKIP_ASM
struct R21530;
extern char *g21530_dtoa(struct R21530 *, double, int, int, int *, int *, char **) __asm__("_dtoa_r");
extern char *g21530_strcpy(char *, char *) __asm__("strcpy");
void print_f_(struct R21530 *ptr, char *buf, double invalue, int ndigit, int type, int dot, int mode) __asm__("print_f");
/**
 * Formats a double in fixed-point form (%f style) into a buffer.
 *
 * Gets the digits from _dtoa_r with the given mode, copies the integer digits (zero-padded up to the point), then
 * unless dot is 0 and no digits remain, a point (with a leading 0 when there were no integer digits), leading zeros for
 * a negative point position and ndigit fraction digits padded with zeros. Infinity and NaN copy the dtoa string. The
 * sign is not printed. _dcvt calls it.
 *
 * @param ptr      the reentrancy struct
 * @param buf      the output buffer
 * @param invalue  the value
 * @param ndigit   the fraction digits
 * @param type     the conversion character (unused)
 * @param dot      nonzero to always print the point
 * @param mode     the _dtoa_r mode (3 for fixed point)
 */
void print_f_(struct R21530 *ptr, char *buf, double invalue, int ndigit, int type, int dot, int mode) {
    int decpt;
    int sign;
    char *digits, *start, *end;
    start = digits = g21530_dtoa(ptr, invalue, mode, ndigit, &decpt, &sign, &end);
    if (decpt == 9999) {
        g21530_strcpy(buf, digits);
        return;
    }
    while (*digits != '\0' && decpt > 0) {
        *buf++ = *digits++;
        decpt--;
    }
    while (decpt > 0) {
        *buf++ = '0';
        decpt--;
    }
    if (dot == 0 && *digits == '\0') {
        *buf = '\0';
        return;
    }
    if (digits == start)
        *buf++ = '0';
    *buf++ = '.';
    while (decpt < 0 && ndigit > 0) {
        *buf++ = '0';
        decpt++;
        ndigit--;
    }
    while (*digits && ndigit > 0) {
        *buf++ = *digits++;
        ndigit--;
    }
    while (ndigit > 0) {
        *buf++ = '0';
        ndigit--;
    }
    *buf = '\0';
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", print_e);
#ifdef SKIP_ASM
struct R216E8;
extern char *g216E8_dtoa(struct R216E8 *, double, int, int, int *, int *, char **) __asm__("_dtoa_r");
extern char *g216E8_strcpy(char *, char *) __asm__("strcpy");
void print_e_(struct R216E8 *ptr, char *buf, double invalue, int ndigit, char type, int dot) __asm__("print_e");
/**
 * Formats a double in exponent form (%e style) into a buffer.
 *
 * Gets ndigit + 1 significant digits from _dtoa_r (mode 2): the first digit, a point when dot is set or ndigit is
 * nonzero, the remaining digits (zero-padded for e/E; g/G become e/E and are not padded), then the exponent letter, a
 * sign and at least two exponent digits. Infinity and NaN copy the dtoa string. The sign of the value is not printed.
 * _gcvt and _dcvt call it.
 *
 * @param ptr      the reentrancy struct
 * @param buf      the output buffer
 * @param invalue  the value
 * @param ndigit   the digits after the point
 * @param type     'e', 'E', 'g' or 'G'
 * @param dot      nonzero to always print the point
 */
void print_e_(struct R216E8 *ptr, char *buf, double invalue, int ndigit, char type, int dot) {
    int decpt;
    int i = ndigit;
    int sign;
    char *digits, *end;
    digits = g216E8_dtoa(ptr, invalue, 2, i + 1, &decpt, &sign, &end);
    if (decpt == 9999) {
        g216E8_strcpy(buf, digits);
        return;
    }
    *buf++ = *digits++;
    if (dot || i != 0)
        *buf++ = '.';
    while (*digits && i > 0) {
        *buf++ = *digits++;
        i--;
    }
    if (type == 'g')
        type = 'e';
    else if (type == 'G')
        type = 'E';
    else {
        while (i > 0) {
            *buf++ = '0';
            i--;
        }
    }
    *buf++ = type;
    decpt--;
    if (decpt < 0) {
        *buf++ = '-';
        decpt = -decpt;
    } else
        *buf++ = '+';
    if (decpt >= 100) {
        int dig = decpt / 100;
        *buf++ = dig + '0';
        decpt -= dig * 100;
    }
    {
        int dig = decpt / 10;
        *buf++ = dig + '0';
        decpt -= dig * 10;
    }
    *buf++ = decpt + '0';
    *buf = '\0';
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", fcvtbuf);
#ifdef SKIP_ASM
struct R218D8 {
    int pad0[0x50 / 4];
    int cvtlen;
    char *cvtbuf;
};
extern struct R218D8 *g218D8_impure __asm__("D_00454944");
#define REENT218D8 g218D8_impure
extern char *g218D8_realloc(struct R218D8 *, char *, int) __asm__("_realloc_r");
extern char *g218D8_dtoa(struct R218D8 *, double, int, int, int *, int *, char **) __asm__("_dtoa_r");
char *fcvtbuf_(double invalue, int ndigit, int *decpt, int *sign, char *buf) __asm__("fcvtbuf");
/**
 * Converts a double to fixed-point decimal digits (fcvt style) and returns the digit string.
 *
 * With a null buf it uses the reentrancy struct's conversion buffer (D_00454944: length +0x50, buffer +0x54), grown
 * with _realloc_r to ndigit + 1 bytes when needed. Values strictly between -1 and 1 use _dtoa_r mode 2, others mode 3
 * (ndigit places past the point). The digits are then padded with '0' only up to ndigit in total.
 *
 * Likely bug (as in newlib): for values of 1 or more the padding counts all digits, not the ones after the point, so a
 * short result such as 1.5 with ndigit 3 gives "150" rather than "1500".
 *
 * @param invalue  the double
 * @param ndigit   digits after the point
 * @param decpt    receives the decimal point position
 * @param sign     receives nonzero for a negative value
 * @param buf      output buffer, or null for the shared one
 * @return the digit string, or 0 when the shared buffer could not grow
 */
char *fcvtbuf_(double invalue, int ndigit, int *decpt, int *sign, char *buf) {
    char *save, *digits, *end;
    int done = 0;
    int newlen;
    if (buf == 0) {
        if (ndigit >= REENT218D8->cvtlen) {
            newlen = ndigit + 1;
            buf = g218D8_realloc(REENT218D8, REENT218D8->cvtbuf, newlen);
            if (buf == 0)
                return 0;
            REENT218D8->cvtlen = newlen;
            REENT218D8->cvtbuf = buf;
        }
        buf = REENT218D8->cvtbuf;
    }
    save = buf;
    if (invalue < 1.0 && invalue > -1.0)
        digits = g218D8_dtoa(REENT218D8, invalue, 2, ndigit, decpt, sign, &end);
    else
        digits = g218D8_dtoa(REENT218D8, invalue, 3, ndigit, decpt, sign, &end);
    while (digits < end) {
        *buf++ = *digits++;
        done++;
    }
    if (done < ndigit) {
        done = ndigit - done;
        do {
            *buf++ = '0';
        } while (--done);
    }
    *buf = 0;
    return save;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", ecvtbuf);
#ifdef SKIP_ASM
struct Reent51 {
    int pad0[0x50 / 4];
    int cvtlen;
    char *cvtbuf;
};
extern int D_00454944;
#define REENT ((struct Reent51 *)D_00454944)
extern char *_realloc_r(struct Reent51 *, char *, int);
extern char *_dtoa_r(struct Reent51 *, double, int, int, int *, int *, char **);
char *ecvtbuf_(double value, int ndigit, int *decpt, int *sign, char *buf) __asm__("ecvtbuf");
/**
 * Converts a double to exactly ndigit significant decimal digits (ecvt style) and returns the digit string.
 *
 * With a null buf it uses the reentrancy struct's conversion buffer (D_00454944: length +0x50, buffer +0x54), growing
 * it with _realloc_r to ndigit + 1 bytes when needed (returning 0 if that fails). The digits come from _dtoa_r mode 2
 * and are padded with '0' to ndigit and terminated.
 *
 * @param value   the double
 * @param ndigit  number of digits
 * @param decpt   receives the decimal point position
 * @param sign    receives nonzero for a negative value
 * @param buf     output buffer, or null for the shared one
 * @return the digit string, or 0 when the shared buffer could not grow
 */
char *ecvtbuf_(double value, int ndigit, int *decpt, int *sign, char *buf) {
    char *save, *digitStr, *end;
    int i = 0;
    int newlen;
    if (buf == 0) {
        if (ndigit >= REENT->cvtlen) {
            newlen = ndigit + 1;
            buf = _realloc_r(REENT, REENT->cvtbuf, newlen);
            if (buf == 0)
                return 0;
            REENT->cvtlen = newlen;
            REENT->cvtbuf = buf;
        }
        buf = REENT->cvtbuf;
    }
    save = buf;
    digitStr = _dtoa_r(REENT, value, 2, ndigit, decpt, sign, &end);
    while (digitStr < end) {
        *buf++ = *digitStr++;
        i++;
    }
    if (i < ndigit) {
        i = ndigit - i;
        do {
            *buf++ = '0';
        } while (--i);
    }
    *buf = 0;
    return save;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _gcvt);
#ifdef SKIP_ASM
/* newlib stdlib/ecvtbuf.c (_gcvt). The file carries no notice of its own (newlib's default: Copyright (c)
 * 1994-2000 Red Hat, Inc. Permission to use, copy, modify and distribute this software is freely granted, provided
 * that this notice is preserved.) */
struct _reent_1B98;
extern double _mprec_log10_1B98(int) __asm__("_mprec_log10");
extern void print_e_1B98(struct _reent_1B98 *, char *, double, int, char, int) __asm__("print_e");
extern char *_dtoa_r_1B98(struct _reent_1B98 *, double, int, int, int *, int *, char **) __asm__("_dtoa_r");
extern char *strcpy_1B98(char *, const char *) __asm__("strcpy");

char *_gcvt_(struct _reent_1B98 *ptr, double invalue, int ndigit, char *buf, char type, int dot)
    __asm__("_gcvt");
/**
 * Formats the magnitude of a double %g-style into a buffer (newlib ecvtbuf.c's _gcvt).
 *
 * Zero prints "0". Values up to 0.0001 or at least 10^ndigit (_mprec_log10) are printed in exponent form by
 * print_e with ndigit - 1 fraction digits. Otherwise the digits come from _dtoa_r (mode 3 below 1.0, else mode
 * 2): integer digits, zeros up to the point, then (when dot is set or digits remain) a point, leading zeros and the
 * remaining digits, plus trailing zeros up to ndigit when dot is set. Infinity and NaN copy the dtoa string. The sign
 * is dropped: callers such as gcvt print the minus themselves.
 *
 * @param ptr      the reentrancy struct
 * @param invalue  the value
 * @param ndigit   significant digits
 * @param buf      the output buffer
 * @param type     'g' or 'G', passed on to the exponent-form printer
 * @param dot      nonzero to keep the point and trailing zeros (the # flag)
 * @return buf
 */
char *
_gcvt_ (struct _reent_1B98 *ptr, double invalue, int ndigit, char *buf, char type, int dot)
{
  char *save = buf;

  if (invalue < 0)
    {
      invalue = -invalue;
    }

  if (invalue == 0)
    {
      *buf++ = '0';
      *buf = '\0';
    }
  else
    /* Which one to print ?
       ANSI says that anything with more that 4 zeros after the . or more
       than precision digits before is printed in e with the qualification
       that trailing zeroes are removed from the fraction portion.  */

  if (0.0001 >= invalue || invalue >= _mprec_log10_1B98 (ndigit))
    {
      /* We subtract 1 from ndigit because in the 'e' format the precision is
	 the number of digits after the . but in 'g' format it is the number
	 of significant digits.

	 We defer changing type to e/E so that print_e() can know it's us
	 calling and thus should remove trailing zeroes.  */

      print_e_1B98 (ptr, buf, invalue, ndigit - 1, type, dot);
    }
  else
    {
      int decpt;
      int sign;
      char *end;
      char *digitStr;

      if (invalue < 1.0)
	{
	  /* what we want is ndigits after the point */
	  digitStr = _dtoa_r_1B98 (ptr, invalue, 3, ndigit, &decpt, &sign, &end);
	}
      else
	{
	  digitStr = _dtoa_r_1B98 (ptr, invalue, 2, ndigit, &decpt, &sign, &end);
	}

      if (decpt == 9999)
	{
	  strcpy_1B98 (buf, digitStr);
	  return save;
	}
      while (*digitStr && decpt > 0)
	{
	  *buf++ = *digitStr++;
	  decpt--;
	  ndigit--;
	}
      /* Even if not in buffer */
      while (decpt > 0 && ndigit > 0)
	{
	  *buf++ = '0';
	  decpt--;
	  ndigit--;
	}

      if (dot || *digitStr)
	{
	  if (buf == save)
	    *buf++ = '0';
	  *buf++ = '.';
	  while (decpt < 0 && ndigit > 0)
	    {
	      *buf++ = '0';
	      decpt++;
	      ndigit--;
	    }

	  /* Print rest of stuff */
	  while (*digitStr && ndigit > 0)
	    {
	      *buf++ = *digitStr++;
	      ndigit--;
	    }
	  /* And trailing zeros */
	  if (dot)
	    {
	      while (ndigit > 0)
		{
		  *buf++ = '0';
		  ndigit--;
		}
	    }
	}
      *buf++ = 0;
    }

  return save;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _dcvt);
#ifdef SKIP_ASM
extern void print_f_f(int, char *, long, int, int, int, int) __asm__("print_f");
extern void _gcvt_g(int, long, int, char *, int, int) __asm__("_gcvt");
extern void print_e_e(int, char *, long, int, int, int) __asm__("print_e");
/**
 * Formats a double into a buffer for an f, e or g conversion and returns the buffer.
 *
 * f/F goes to print_f (mode 3, the point forced when nd is nonzero), g/G to _gcvt (at least one digit), e/E to
 * print_e. Other characters leave the buffer untouched. No code calls it.
 *
 * @param ep        the reentrancy struct
 * @param buf       the output buffer
 * @param val       the value
 * @param nd        the precision
 * @param width     the field width (unused)
 * @param fmt       the conversion character
 * @param forceDot  nonzero to always print the point
 * @return buf
 */
char *_dcvt(int ep, char *buf, long val, int nd, int width, char fmt, int forceDot) {
    switch (fmt) {
    case 'f':
    case 'F':
        print_f_f(ep, buf, val, nd, fmt, nd ? 1 : forceDot, 3);
        break;
    case 'g':
    case 'G':
        _gcvt_g(ep, val, nd ? nd : 1, buf, fmt, forceDot);
        break;
    case 'e':
    case 'E':
        print_e_e(ep, buf, val, nd, fmt, forceDot);
        break;
    }
    return buf;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _realloc_r);
#ifdef SKIP_ASM
extern void __malloc_lock_lock(int) __asm__("__malloc_lock");
extern void __malloc_unlock_unlock(int) __asm__("__malloc_unlock");
extern void *_malloc_r_malloc(int, int) __asm__("_malloc_r");
extern void _free_r_free(int, void *) __asm__("_free_r");
extern void *func_0041605C_memcpy(void *, void *, unsigned) __asm__("func_0041605C");
#define MALLOC_COPY(dest, src, nbytes)     do {         unsigned mcn = (nbytes);         if (mcn < 37) {             unsigned *mcsrc = (unsigned *)(src);             unsigned *mcdst = (unsigned *)(dest);             if (mcn >= 20) {                 *mcdst++ = *mcsrc++;                 *mcdst++ = *mcsrc++;                 if (mcn >= 28) {                     *mcdst++ = *mcsrc++;                     *mcdst++ = *mcsrc++;                     if (mcn >= 36) {                         *mcdst++ = *mcsrc++;                         *mcdst++ = *mcsrc++;                     }                 }             }             *mcdst++ = *mcsrc++;             *mcdst++ = *mcsrc++;             *mcdst = *mcsrc;         } else             func_0041605C_memcpy(dest, src, mcn);     } while (0)
#define inuse(p) (chunk_at_offset(p, (p)->size & ~1u)->size & 1)
void *_realloc_r_(int ptr, void *oldmem, unsigned bytes) __asm__("_realloc_r");
/**
 * Resizes a heap block, in place when the neighbouring chunks allow it (Doug Lea's allocator as built into newlib).
 *
 * A null block is just allocated (_malloc_r). Otherwise, under the heap lock (__malloc_lock / __malloc_unlock), the
 * request is padded to the chunk size ((bytes + 19) & ~15, at least 16). A block that must grow first tries to absorb
 * the next chunk (the top chunk, keeping at least 16 bytes of it, or a free chunk), then the previous free chunk
 * together with the next one, copying the data down; failing that it allocates a new block, copies the old contents and
 * frees the old one (_free_r). A leftover of 16 bytes or more is split off and freed.
 *
 * @param ptr     the reentrancy struct
 * @param oldmem  the block to resize, or null
 * @param bytes   the new size in bytes
 * @return the resized block, or 0 when no memory is left (the old block stays valid)
 */
void *_realloc_r_(int ptr, void *oldmem, unsigned bytes) {
    unsigned nb, oldsize, newsize, nextsize, prevsize, remainder_size;
    mchunkptr oldp, newp, next, prev, remainder, bck, fwd;
    void *newmem;
    if (oldmem == 0)
        return _malloc_r_malloc(ptr, bytes);
    __malloc_lock_lock(ptr);
    newp = oldp = (mchunkptr)((char *)oldmem - 8);
    newsize = oldsize = chunksize(oldp);
    nb = (bytes + 19 < 31) ? 16 : ((bytes + 19) & ~15u);
    if (oldsize < nb) {
        next = chunk_at_offset(oldp, oldsize);
        if (next == top || !inuse(next)) {
            nextsize = chunksize(next);
            if (next == top) {
                if (nextsize + newsize >= nb + 16) {
                    newsize += nextsize;
                    top = chunk_at_offset(oldp, nb);
                    set_head(top, (newsize - nb) | 1);
                    set_head_size(oldp, nb);
                    __malloc_unlock_unlock(ptr);
                    return (char *)oldp + 8;
                }
            } else if (nextsize + newsize >= nb) {
                unlink(next, bck, fwd);
                newsize += nextsize;
                goto split;
            }
        } else {
            next = 0;
            nextsize = 0;
        }
        if (!(oldp->size & 1)) {
            prev = chunk_at_offset(oldp, -(int)oldp->prev_size);
            prevsize = chunksize(prev);
            if (next != 0) {
                if (next == top) {
                    if (nextsize + prevsize + newsize >= nb + 16) {
                        unlink(prev, bck, fwd);
                        newp = prev;
                        newsize += prevsize + nextsize;
                        newmem = (char *)newp + 8;
                        MALLOC_COPY((char *)newp + 8, oldmem, oldsize - 4);
                        top = chunk_at_offset(newp, nb);
                        set_head(top, (newsize - nb) | 1);
                        set_head_size(newp, nb);
                        __malloc_unlock_unlock(ptr);
                        return newmem;
                    }
                } else if (nextsize + prevsize + newsize >= nb) {
                    unlink(next, bck, fwd);
                    unlink(prev, bck, fwd);
                    newp = prev;
                    newsize += nextsize + prevsize;
                    newmem = (char *)newp + 8;
                    MALLOC_COPY((char *)newp + 8, oldmem, oldsize - 4);
                    goto split;
                }
            }
            if (prev != 0 && prevsize + newsize >= nb) {
                unlink(prev, bck, fwd);
                newp = prev;
                newsize += prevsize;
                newmem = (char *)newp + 8;
                MALLOC_COPY((char *)newp + 8, oldmem, oldsize - 4);
                goto split;
            }
        }
        newmem = _malloc_r_malloc(ptr, bytes);
        if (newmem == 0) {
            __malloc_unlock_unlock(ptr);
            return 0;
        }
        if ((newp = (mchunkptr)((char *)newmem - 8)) == chunk_at_offset(oldp, oldp->size & ~1u)) {
            newsize += chunksize(newp);
            newp = oldp;
            goto split;
        }
        MALLOC_COPY(newmem, oldmem, oldsize - 4);
        _free_r_free(ptr, oldmem);
        __malloc_unlock_unlock(ptr);
        return newmem;
    }
split:
    if (newsize - nb >= 16) {
        remainder = chunk_at_offset(newp, nb);
        remainder_size = newsize - nb;
        set_head_size(newp, nb);
        set_head(remainder, remainder_size | 1);
        chunk_at_offset(remainder, remainder_size)->size |= 1;
        _free_r_free(ptr, (char *)remainder + 8);
    } else {
        set_head_size(newp, newsize);
        chunk_at_offset(newp, newsize)->size |= 1;
    }
    __malloc_unlock_unlock(ptr);
    return (char *)newp + 8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _calloc_r);
#ifdef SKIP_ASM
extern void *_malloc_r_malloc(int, int) __asm__("_malloc_r");
extern void *func_00416210_memset(void *, int, unsigned) __asm__("func_00416210");
#define MALLOC_ZERO(charp, nbytes)     do {         unsigned mctmp = (nbytes);         if (mctmp < 37) {             unsigned *mzp = (unsigned *)(charp);             if (mctmp >= 20) {                 *mzp++ = 0;                 *mzp++ = 0;                 if (mctmp >= 28) {                     *mzp++ = 0;                     *mzp++ = 0;                     if (mctmp >= 36) {                         *mzp++ = 0;                         *mzp++ = 0;                     }                 }             }             *mzp++ = 0;             *mzp++ = 0;             *mzp = 0;         } else             func_00416210_memset(charp, 0, mctmp);     } while (0)
void *_calloc_r_(int ptr, int n, int elem_size) __asm__("_calloc_r");
/**
 * Allocates a zeroed array of n elements from the C heap and returns it, or null (no locking).
 *
 * Allocates n * elem_size bytes with _malloc_r and clears the whole chunk (unrolled for up to 36 bytes, else
 * func_00416210). The multiplication is not checked for overflow. _Balloc calls it.
 *
 * @param ptr        the reentrancy struct
 * @param n          the element count
 * @param elem_size  the element size
 * @return the block, or null
 */
void *_calloc_r_(int ptr, int n, int elem_size) {
    mchunkptr p;
    unsigned csz;
    unsigned sz = n * elem_size;
    void *mem;
    mem = _malloc_r_malloc(ptr, sz);
    if (mem == 0)
        return 0;
    p = (mchunkptr)((char *)mem - 8);
    csz = chunksize(p);
    MALLOC_ZERO(mem, csz - 4);
    return mem;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", quorem);
#ifdef SKIP_ASM
extern int __mcmp_cmp(struct Bigint *, struct Bigint *) __asm__("__mcmp");
/**
 * Divides one Bigint by another when the quotient is a single digit, leaves the remainder and returns the digit.
 *
 * Estimates the quotient from the top words (top / (divisor top + 1)), subtracts quotient * divisor in 16-bit halves,
 * then corrects by one more subtraction when the remainder is still at least the divisor (__mcmp). Leading zero
 * words of the remainder are trimmed. _dtoa_r calls it for each digit.
 *
 * @param dividend  the dividend, replaced by the remainder
 * @param divisor   the divisor
 * @return the quotient digit (0..9 in dtoa use)
 */
int quorem(struct Bigint *dividend, struct Bigint *divisor) {
    int n;
    int borrow, y;
    unsigned carry, quotient, ys;
    unsigned *bx, *bxe, *sx, *sxe;
    int z;
    unsigned si, zs;
    n = divisor->wds;
    if (dividend->wds < n)
        return 0;
    sx = divisor->x;
    sxe = sx + --n;
    bx = dividend->x;
    bxe = bx + n;
    quotient = *bxe / (*sxe + 1);
    if (quotient) {
        borrow = 0;
        carry = 0;
        do {
            si = *sx++;
            ys = (si & 0xffff) * quotient + carry;
            zs = (si >> 16) * quotient + (ys >> 16);
            carry = zs >> 16;
            y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
            borrow = y >> 16;
            z = (*bx >> 16) - (zs & 0xffff) + borrow;
            borrow = z >> 16;
            ((unsigned short *)bx)[1] = (unsigned short)z;
            ((unsigned short *)bx)[0] = (unsigned short)y;
            bx++;
        } while (sx <= sxe);
        if (!*bxe) {
            bx = dividend->x;
            while (--bxe > bx && !*bxe)
                --n;
            dividend->wds = n;
        }
    }
    if (__mcmp_cmp(dividend, divisor) >= 0) {
        quotient++;
        borrow = 0;
        carry = 0;
        bx = dividend->x;
        sx = divisor->x;
        do {
            si = *sx++;
            ys = (si & 0xffff) + carry;
            zs = (si >> 16) + (ys >> 16);
            carry = zs >> 16;
            y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
            borrow = y >> 16;
            z = (*bx >> 16) - (zs & 0xffff) + borrow;
            borrow = z >> 16;
            ((unsigned short *)bx)[1] = (unsigned short)z;
            ((unsigned short *)bx)[0] = (unsigned short)y;
            bx++;
        } while (sx <= sxe);
        bx = dividend->x;
        bxe = bx + n;
        if (!*bxe) {
            while (--bxe > bx && !*bxe)
                --n;
            dividend->wds = n;
        }
    }
    return quotient;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", _dtoa_r);
#ifdef SKIP_ASM
/* newlib 1.9 libc/stdlib/dtoa.c (_dtoa_r) with the mprec.h macros it uses, verbatim (IEEE_8087, Pack_32,
 * IEEE_Arith, no Sudden_Underflow / ROUND_BIASED / Check_FLT_ROUNDS / No_leftright):
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
struct _reent_00422780 {
  int pad0[0x40 / 4];
  struct Bigint *_result;
  int _result_k;
};
union double_union_00422780
{
  double d;
  unsigned int i[2];
};
#define word0_00422780(x) (x.i[1])
#define word1_00422780(x) (x.i[0])
#define Exp_shift1_00422780 20
#define Exp_msk1_00422780    ((unsigned int)0x100000L)
#define Exp_mask_00422780  ((unsigned int)0x7ff00000L)
#define P_00422780 53
#define Bias_00422780 1023
#define Exp_11_00422780 ((unsigned int)0x3ff00000L)
#define Frac_mask1_00422780 ((unsigned int)0xfffffL)
#define Ten_pmax_00422780 22
#define Bletch_00422780 0x10
#define Bndry_mask_00422780  ((unsigned int)0xfffffL)
#define Sign_bit_00422780 ((unsigned int)0x80000000L)
#define Log2P_00422780 1
#define Quick_max_00422780 14
#define Int_max_00422780 14
#define n_bigtens_00422780 5
#define Bcopy_00422780(x,y) func_0041605C((char *)&x->sign, (char *)&y->sign, y->wds*sizeof(int) + 2*sizeof(int))
extern const double tens_00422780[] __asm__("D_0049A8F8");
extern const double bigtens_00422780[] __asm__("D_0049A9C0");
extern struct Bigint *g2780_Balloc(struct _reent_00422780 *, int) __asm__("_Balloc");
extern void g2780_Bfree(struct _reent_00422780 *, struct Bigint *) __asm__("_Bfree");
extern struct Bigint *g2780_multadd(struct _reent_00422780 *, struct Bigint *, int, int) __asm__("_multadd");
extern int g2780_hi0bits(unsigned int) __asm__("_hi0bits");
extern struct Bigint *g2780_i2b(struct _reent_00422780 *, int) __asm__("_i2b");
extern struct Bigint *g2780_mult(struct _reent_00422780 *, struct Bigint *, struct Bigint *) __asm__("_multiply");
extern struct Bigint *g2780_pow5mult(struct _reent_00422780 *, struct Bigint *, int) __asm__("_pow5mult");
extern struct Bigint *g2780_lshift(struct _reent_00422780 *, struct Bigint *, int) __asm__("_lshift");
extern int g2780_cmp(struct Bigint *, struct Bigint *) __asm__("__mcmp");
extern struct Bigint *g2780_diff(struct _reent_00422780 *, struct Bigint *, struct Bigint *) __asm__("__mdiff");
extern struct Bigint *g2780_d2b(struct _reent_00422780 *, double, int *, int *) __asm__("_d2b");

/* the unit declares this with struct Reent51 * (ecvtbuf, ecvtbuf) */
char *_dtoa_r_(struct _reent_00422780 *, double, int, int, int *, int *, char **) __asm__("_dtoa_r");
/**
 * Converts a double to a decimal digit string (David Gay's dtoa, newlib 1.9) and reports the decimal point and sign.
 *
 * Returns the significant digits only, without a point, a sign or trailing zeros. Infinity and NaN return "Infinity" /
 * "NaN" with *decpt 9999, zero returns "0" with *decpt 1. mode 0 gives the shortest string that reads back to the same
 * double, 1 the same with the Steele & White stopping rule, 2 at most max(1, ndigits) significant digits (ecvt style),
 * 3 digits through ndigits places past the point (fcvt style; ndigits may be negative); 4 and above behave like 2 and
 * 3. A floating-point fast path is tried for small digit counts before the exact Bigint arithmetic (_Balloc
 * Balloc, _multadd multadd, _multiply mult, _pow5mult pow5mult, _lshift lshift, __mcmp cmp,
 * __mdiff diff, _d2b d2b).
 *
 * The string lives in a Bigint kept in the reentrancy struct (_result +0x40, _result_k +0x44) and is freed by the next
 * call, so it must be copied before calling again.
 *
 * @param ptr      the reentrancy struct
 * @param _d       the value
 * @param mode     conversion mode, 0..9 as above
 * @param ndigits  digit count for modes 2 and above
 * @param decpt    receives the decimal point position (9999 for Inf/NaN)
 * @param sign     receives 1 for a negative value (also -0 and negative NaNs), else 0
 * @param rve      receives the end of the digit string, or null
 * @return the digit string
 */
char *
_dtoa_r_(struct _reent_00422780 *ptr, double _d, int mode, int ndigits, int *decpt, int *sign, char **rve)
{
  int bbits, bPow2, bPow5, be, dig, i, ieps, ilim, ilim0, ilim1, j, j1, k, savedK,
    k_check, leftright, mPow2, mPow5, sPow2, sPow5, spec_case, try_quick;
  union double_union_00422780 d, d2, eps;
  int digitVal;
  int denorm;
  unsigned long x;
  struct Bigint *mantissa, *product, *delta, *mlo, *mhi, *bigDivisor;
  double ds;
  char *cursor, *bufStart;

  d.d = _d;

  if (ptr->_result)
    {
      ptr->_result->k = ptr->_result_k;
      ptr->_result->maxwds = 1 << ptr->_result_k;
      g2780_Bfree (ptr, ptr->_result);
      ptr->_result = 0;
    }

  if (word0_00422780 (d) & Sign_bit_00422780)
    {
      /* set sign for everything, including 0's and NaNs */
      *sign = 1;
      word0_00422780 (d) &= ~Sign_bit_00422780;	/* clear sign bit */
    }
  else
    *sign = 0;

  if ((word0_00422780 (d) & Exp_mask_00422780) == Exp_mask_00422780)
    {
      /* Infinity or NaN */
      *decpt = 9999;
      cursor =
	!word1_00422780 (d) && !(word0_00422780 (d) & 0xfffff) ? "Infinity" :
	"NaN";
      if (rve)
	*rve =
	  cursor[3] ? cursor + 8 :
	  cursor + 3;
      return cursor;
    }
  if (!d.d)
    {
      *decpt = 1;
      cursor = "0";
      if (rve)
	*rve = cursor + 1;
      return cursor;
    }

  mantissa = g2780_d2b (ptr, d.d, &be, &bbits);
  if ((i = (int) (word0_00422780 (d) >> Exp_shift1_00422780 & (Exp_mask_00422780 >> Exp_shift1_00422780))) != 0)
    {
      d2.d = d.d;
      word0_00422780 (d2) &= Frac_mask1_00422780;
      word0_00422780 (d2) |= Exp_11_00422780;

      /* log(x)	~=~ log(1.5) + (x-1.5)/1.5
		 * log10(x)	 =  log(x) / log(10)
		 *		~=~ log(1.5)/log(10) + (x-1.5)/(1.5*log(10))
		 * log10(d) = (i-Bias)*log(2)/log(10) + log10(d2)
		 *
		 * This suggests computing an approximation k to log10(d) by
		 *
		 * k = (i - Bias)*0.301029995663981
		 *	+ ( (d2-1.5)*0.289529654602168 + 0.176091259055681 );
		 *
		 * We want k to be too large rather than too small.
		 * The error in the first-order Taylor series approximation
		 * is in our favor, so we just round up the constant enough
		 * to compensate for any error in the multiplication of
		 * (i - Bias) by 0.301029995663981; since |i - Bias| <= 1077,
		 * and 1077 * 0.30103 * 2^-52 ~=~ 7.2e-14,
		 * adding 1e-13 to the constant term more than suffices.
		 * Hence we adjust the constant term to 0.1760912590558.
		 * (We could get a more accurate k by invoking log10,
		 *  but this is probably not worthwhile.)
		 */

      i -= Bias_00422780;
      denorm = 0;
    }
  else
    {
      /* d is denormalized */

      i = bbits + be + (Bias_00422780 + (P_00422780 - 1) - 1);
      x = (i > 32) ? (word0_00422780 (d) << (64 - i)) | (word1_00422780 (d) >> (i - 32))
       : (word1_00422780 (d) << (32 - i));
      d2.d = x;
      word0_00422780 (d2) -= 31 * Exp_msk1_00422780;	/* adjust exponent */
      i -= (Bias_00422780 + (P_00422780 - 1) - 1) + 1;
      denorm = 1;
    }
  ds = (d2.d - 1.5) * 0.289529654602168 + 0.1760912590558 + i * 0.301029995663981;
  k = (int) ds;
  if (ds < 0. && ds != k)
    k--;			/* want k = floor(ds) */
  k_check = 1;
  if (k >= 0 && k <= Ten_pmax_00422780)
    {
      if (d.d < tens_00422780[k])
	k--;
      k_check = 0;
    }
  j = bbits - i - 1;
  if (j >= 0)
    {
      bPow2 = 0;
      sPow2 = j;
    }
  else
    {
      bPow2 = -j;
      sPow2 = 0;
    }
  if (k >= 0)
    {
      bPow5 = 0;
      sPow5 = k;
      sPow2 += k;
    }
  else
    {
      bPow2 -= k;
      bPow5 = -k;
      sPow5 = 0;
    }
  if (mode < 0 || mode > 9)
    mode = 0;
  try_quick = 1;
  if (mode > 5)
    {
      mode -= 4;
      try_quick = 0;
    }
  leftright = 1;
  switch (mode)
    {
    case 0:
    case 1:
      ilim = ilim1 = -1;
      i = 18;
      ndigits = 0;
      break;
    case 2:
      leftright = 0;
      /* no break */
    case 4:
      if (ndigits <= 0)
	ndigits = 1;
      ilim = ilim1 = i = ndigits;
      break;
    case 3:
      leftright = 0;
      /* no break */
    case 5:
      i = ndigits + k + 1;
      ilim = i;
      ilim1 = i - 1;
      if (i <= 0)
	i = 1;
    }
  j = sizeof (unsigned int);
  for (ptr->_result_k = 0; sizeof (struct Bigint) - sizeof (unsigned int) + j <= i;
       j <<= 1)
    ptr->_result_k++;
  ptr->_result = g2780_Balloc (ptr, ptr->_result_k);
  cursor = bufStart = (char *) ptr->_result;

  if (ilim >= 0 && ilim <= Quick_max_00422780 && try_quick)
    {
      /* Try to get by with floating-point arithmetic. */

      i = 0;
      d2.d = d.d;
      savedK = k;
      ilim0 = ilim;
      ieps = 2;			/* conservative */
      if (k > 0)
	{
	  ds = tens_00422780[k & 0xf];
	  j = k >> 4;
	  if (j & Bletch_00422780)
	    {
	      /* prevent overflows */
	      j &= Bletch_00422780 - 1;
	      d.d /= bigtens_00422780[n_bigtens_00422780 - 1];
	      ieps++;
	    }
	  for (; j; j >>= 1, i++)
	    if (j & 1)
	      {
		ieps++;
		ds *= bigtens_00422780[i];
	      }
	  d.d /= ds;
	}
      else if ((j1 = -k) != 0)
	{
	  d.d *= tens_00422780[j1 & 0xf];
	  for (j = j1 >> 4; j; j >>= 1, i++)
	    if (j & 1)
	      {
		ieps++;
		d.d *= bigtens_00422780[i];
	      }
	}
      if (k_check && d.d < 1. && ilim > 0)
	{
	  if (ilim1 <= 0)
	    goto fast_failed;
	  ilim = ilim1;
	  k--;
	  d.d *= 10.;
	  ieps++;
	}
      eps.d = ieps * d.d + 7.;
      word0_00422780 (eps) -= (P_00422780 - 1) * Exp_msk1_00422780;
      if (ilim == 0)
	{
	  bigDivisor = mhi = 0;
	  d.d -= 5.;
	  if (d.d > eps.d)
	    goto one_digit;
	  if (d.d < -eps.d)
	    goto no_digits;
	  goto fast_failed;
	}
      if (leftright)
	{
	  /* Use Steele & White method of only
	   * generating digits needed.
	   */
	  eps.d = 0.5 / tens_00422780[ilim - 1] - eps.d;
	  for (i = 0;;)
	    {
	      digitVal = d.d;
	      d.d -= digitVal;
	      *cursor++ = '0' + (int) digitVal;
	      if (d.d < eps.d)
		goto ret1;
	      if (1. - d.d < eps.d)
		goto bump_up;
	      if (++i >= ilim)
		break;
	      eps.d *= 10.;
	      d.d *= 10.;
	    }
	}
      else
	{
	  /* Generate ilim digits, then fix them up. */
	  eps.d *= tens_00422780[ilim - 1];
	  for (i = 1;; i++, d.d *= 10.)
	    {
	      digitVal = d.d;
	      d.d -= digitVal;
	      *cursor++ = '0' + (int) digitVal;
	      if (i == ilim)
		{
		  if (d.d > 0.5 + eps.d)
		    goto bump_up;
		  else if (d.d < 0.5 - eps.d)
		    {
		      while (*--cursor == '0');
		      cursor++;
		      goto ret1;
		    }
		  break;
		}
	    }
	}
    fast_failed:
      cursor = bufStart;
      d.d = d2.d;
      k = savedK;
      ilim = ilim0;
    }

  /* Do we have a "small" integer? */

  if (be >= 0 && k <= Int_max_00422780)
    {
      /* Yes. */
      ds = tens_00422780[k];
      if (ndigits < 0 && ilim <= 0)
	{
	  bigDivisor = mhi = 0;
	  if (ilim < 0 || d.d <= 5 * ds)
	    goto no_digits;
	  goto one_digit;
	}
      for (i = 1;; i++)
	{
	  digitVal = d.d / ds;
	  d.d -= digitVal * ds;
	  *cursor++ = '0' + (int) digitVal;
	  if (i == ilim)
	    {
	      d.d += d.d;
             if ((d.d > ds) || ((d.d == ds) && (digitVal & 1)))
		{
		bump_up:
		  while (*--cursor == '9')
		    if (cursor == bufStart)
		      {
			k++;
			*cursor = '0';
			break;
		      }
		  ++*cursor++;
		}
	      break;
	    }
	  if (!(d.d *= 10.))
	    break;
	}
      goto ret1;
    }

  mPow2 = bPow2;
  mPow5 = bPow5;
  mhi = mlo = 0;
  if (leftright)
    {
      if (mode < 2)
	{
	  i =
	    denorm ? be + (Bias_00422780 + (P_00422780 - 1) - 1 + 1) :
	    1 + P_00422780 - bbits;
	}
      else
	{
	  j = ilim - 1;
	  if (mPow5 >= j)
	    mPow5 -= j;
	  else
	    {
	      sPow5 += j -= mPow5;
	      bPow5 += j;
	      mPow5 = 0;
	    }
	  if ((i = ilim) < 0)
	    {
	      mPow2 -= i;
	      i = 0;
	    }
	}
      bPow2 += i;
      sPow2 += i;
      mhi = g2780_i2b (ptr, 1);
    }
  if (mPow2 > 0 && sPow2 > 0)
    {
      i = mPow2 < sPow2 ? mPow2 : sPow2;
      bPow2 -= i;
      mPow2 -= i;
      sPow2 -= i;
    }
  if (bPow5 > 0)
    {
      if (leftright)
	{
	  if (mPow5 > 0)
	    {
	      mhi = g2780_pow5mult (ptr, mhi, mPow5);
	      product = g2780_mult (ptr, mhi, mantissa);
	      g2780_Bfree (ptr, mantissa);
	      mantissa = product;
	    }
         if ((j = bPow5 - mPow5) != 0)
	    mantissa = g2780_pow5mult (ptr, mantissa, j);
	}
      else
	mantissa = g2780_pow5mult (ptr, mantissa, bPow5);
    }
  bigDivisor = g2780_i2b (ptr, 1);
  if (sPow5 > 0)
    bigDivisor = g2780_pow5mult (ptr, bigDivisor, sPow5);

  /* Check for special case that d is a normalized power of 2. */

  if (mode < 2)
    {
      if (!word1_00422780 (d) && !(word0_00422780 (d) & Bndry_mask_00422780)
	  && word0_00422780 (d) & Exp_mask_00422780
	)
	{
	  /* The special case */
	  bPow2 += Log2P_00422780;
	  sPow2 += Log2P_00422780;
	  spec_case = 1;
	}
      else
	spec_case = 0;
    }

  /* Arrange for convenient computation of quotients:
   * shift left if necessary so divisor has 4 leading 0 bits.
   *
   * Perhaps we should just compute leading 28 bits of S once
   * and for all and pass them and a shift to quorem, so it
   * can do shifts and ors to compute the numerator for q.
   */

  if ((i = ((sPow5 ? 32 - g2780_hi0bits (bigDivisor->x[bigDivisor->wds - 1]) : 1) + sPow2) & 0x1f) != 0)
    i = 32 - i;
  if (i > 4)
    {
      i -= 4;
      bPow2 += i;
      mPow2 += i;
      sPow2 += i;
    }
  else if (i < 4)
    {
      i += 28;
      bPow2 += i;
      mPow2 += i;
      sPow2 += i;
    }
  if (bPow2 > 0)
    mantissa = g2780_lshift (ptr, mantissa, bPow2);
  if (sPow2 > 0)
    bigDivisor = g2780_lshift (ptr, bigDivisor, sPow2);
  if (k_check)
    {
      if (g2780_cmp (mantissa, bigDivisor) < 0)
	{
	  k--;
	  mantissa = g2780_multadd (ptr, mantissa, 10, 0);	/* we botched the k estimate */
	  if (leftright)
	    mhi = g2780_multadd (ptr, mhi, 10, 0);
	  ilim = ilim1;
	}
    }
  if (ilim <= 0 && mode > 2)
    {
      if (ilim < 0 || g2780_cmp (mantissa, bigDivisor = g2780_multadd (ptr, bigDivisor, 5, 0)) <= 0)
	{
	  /* no digits, fcvt style */
	no_digits:
	  k = -1 - ndigits;
	  goto ret;
	}
    one_digit:
      *cursor++ = '1';
      k++;
      goto ret;
    }
  if (leftright)
    {
      if (mPow2 > 0)
	mhi = g2780_lshift (ptr, mhi, mPow2);

      /* Compute mlo -- check for special case
       * that d is a normalized power of 2.
       */

      mlo = mhi;
      if (spec_case)
	{
	  mhi = g2780_Balloc (ptr, mhi->k);
	  Bcopy_00422780 (mhi, mlo);
	  mhi = g2780_lshift (ptr, mhi, Log2P_00422780);
	}

      for (i = 1;; i++)
	{
	  dig = quorem (mantissa, bigDivisor) + '0';
	  /* Do we yet have the shortest decimal string
	   * that will round to d?
	   */
	  j = g2780_cmp (mantissa, mlo);
	  delta = g2780_diff (ptr, bigDivisor, mhi);
	  j1 = delta->sign ? 1 : g2780_cmp (mantissa, delta);
	  g2780_Bfree (ptr, delta);
	  if (j1 == 0 && !mode && !(word1_00422780 (d) & 1))
	    {
	      if (dig == '9')
		goto round_9_up;
	      if (j > 0)
		dig++;
	      *cursor++ = dig;
	      goto ret;
	    }
         if ((j < 0) || ((j == 0) && !mode
	      && !(word1_00422780 (d) & 1)
           ))
	    {
	      if (j1 > 0)
		{
		  mantissa = g2780_lshift (ptr, mantissa, 1);
		  j1 = g2780_cmp (mantissa, bigDivisor);
                 if (((j1 > 0) || ((j1 == 0) && (dig & 1)))
		      && dig++ == '9')
		    goto round_9_up;
		}
	      *cursor++ = dig;
	      goto ret;
	    }
	  if (j1 > 0)
	    {
	      if (dig == '9')
		{		/* possible if i == 1 */
		round_9_up:
		  *cursor++ = '9';
		  goto roundoff;
		}
	      *cursor++ = dig + 1;
	      goto ret;
	    }
	  *cursor++ = dig;
	  if (i == ilim)
	    break;
	  mantissa = g2780_multadd (ptr, mantissa, 10, 0);
	  if (mlo == mhi)
	    mlo = mhi = g2780_multadd (ptr, mhi, 10, 0);
	  else
	    {
	      mlo = g2780_multadd (ptr, mlo, 10, 0);
	      mhi = g2780_multadd (ptr, mhi, 10, 0);
	    }
	}
    }
  else
    for (i = 1;; i++)
      {
	*cursor++ = dig = quorem (mantissa, bigDivisor) + '0';
	if (i >= ilim)
	  break;
	mantissa = g2780_multadd (ptr, mantissa, 10, 0);
      }

  /* Round off last digit */

  mantissa = g2780_lshift (ptr, mantissa, 1);
  j = g2780_cmp (mantissa, bigDivisor);
  if ((j > 0) || ((j == 0) && (dig & 1)))
    {
    roundoff:
      while (*--cursor == '9')
	if (cursor == bufStart)
	  {
	    k++;
	    *cursor++ = '1';
	    goto ret;
	  }
      ++*cursor++;
    }
  else
    {
      while (*--cursor == '0');
      cursor++;
    }
ret:
  g2780_Bfree (ptr, bigDivisor);
  if (mhi)
    {
      if (mlo && mlo != mhi)
	g2780_Bfree (ptr, mlo);
      g2780_Bfree (ptr, mhi);
    }
ret1:
  g2780_Bfree (ptr, mantissa);
  *cursor = 0;
  *decpt = k + 1;
  if (rve)
    *rve = cursor;
  return bufStart;
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", func_00423980);
#ifdef SKIP_ASM
/**
 * Issues PS2 kernel system call 0 (hand-written stub).
 *
 * Loads 0 into $3 and executes syscall. It sits just before the named stubs ResetEE (syscall 1) and SetGsCrt (syscall
 * 2); no code calls it.
 */
// PORT: PS2-only kernel syscall 0 stub (gcc writes the zero as move $3,$0, the stub has li $3,0); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
void func_00423980(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "addiu $3, $0, 0x0\n"
        "syscall 0\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("seg/seg_316FB8", ResetEE);
#ifdef SKIP_ASM
/**
 * Calls EE kernel syscall 0x1 (ResetEE in the PS2 SDK's syscall table).
 *
 * Loads the number into $v1 and executes `syscall`; the kernel does the work. The stub declares no parameters; the
 * caller's arguments reach the kernel in the argument registers untouched.
 */
SYSCALL_STUB(ResetEE, 0x1)
#endif
