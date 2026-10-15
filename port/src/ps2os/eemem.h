/*
 * EE address space for the PC port.
 *
 * The decompiled C and the recompiled asm both use EE virtual addresses as
 * pointers, so the port maps EE memory at the same addresses inside a 32-bit
 * process. A pointer read from game memory is then a valid host pointer with
 * no translation.
 *
 *   0x00100000..0x01FFFFFF  main RAM (below 1 MB is the EE kernel; not mapped)
 *   0x20100000..0x21FFFFFF  uncached mirror of main RAM
 *   0x30100000..0x31FFFFFF  uncached-accelerated mirror of main RAM
 *   0x70000000..0x70003FFF  scratchpad
 *
 * The mirrors share one backing object with main RAM, so a write through any
 * of them is visible through the others.
 */
#ifndef PS2OS_EEMEM_H
#define PS2OS_EEMEM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EE_RAM_SIZE         0x02000000u
#define EE_RAM_USER_BASE    0x00100000u
#define EE_RAM_UNCACHED     0x20000000u
#define EE_RAM_UNCACHED_ACC 0x30000000u
#define EE_SPR_BASE         0x70000000u
#define EE_SPR_SIZE         0x00004000u

/* Maps all EE regions. Returns 0 on success, -1 if any region is already
 * taken in this process or the OS refuses the mapping. */
int ee_mem_init(void);
void ee_mem_shutdown(void);

/* True if [addr, addr + size) lies entirely inside one mapped region. */
int ee_mem_valid(uint32_t addr, uint32_t size);

static inline void *ee_ptr(uint32_t addr)
{
    return (void *)(uintptr_t)addr;
}

/* Loads the PT_LOAD segments of a MIPS ELF32 image into EE memory and zeroes
 * the remainder of each segment (bss). Returns 0 and stores the entry point,
 * or -1 if the image is malformed or a segment falls outside EE memory. */
int ee_load_elf(const void *image, size_t size, uint32_t *entry);

#ifdef __cplusplus
}
#endif

#endif
