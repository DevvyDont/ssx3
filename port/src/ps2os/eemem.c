#define _GNU_SOURCE
#include "eemem.h"

#include <string.h>

#if UINTPTR_MAX != 0xFFFFFFFFu
#error "The port must be built as a 32-bit program (see port/README.md)"
#endif

#define RAM_MAP_SIZE (EE_RAM_SIZE - EE_RAM_USER_BASE)

static const uint32_t ram_bases[] = {
    EE_RAM_USER_BASE,
    EE_RAM_UNCACHED + EE_RAM_USER_BASE,
    EE_RAM_UNCACHED_ACC + EE_RAM_USER_BASE,
};
#define NUM_RAM_VIEWS (sizeof(ram_bases) / sizeof(ram_bases[0]))

#ifdef _WIN32

#include <windows.h>

static HANDLE ram_section;
static void *ram_views[NUM_RAM_VIEWS];
static void *spr_view;

int ee_mem_init(void)
{
    size_t i;

    ram_section = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                     0, EE_RAM_SIZE, NULL);
    if (ram_section == NULL)
        return -1;
    for (i = 0; i < NUM_RAM_VIEWS; i++) {
        ram_views[i] = MapViewOfFileEx(ram_section, FILE_MAP_ALL_ACCESS, 0,
                                       EE_RAM_USER_BASE, RAM_MAP_SIZE,
                                       ee_ptr(ram_bases[i]));
        if (ram_views[i] != ee_ptr(ram_bases[i]))
            goto fail;
    }
    spr_view = VirtualAlloc(ee_ptr(EE_SPR_BASE), EE_SPR_SIZE,
                            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (spr_view != ee_ptr(EE_SPR_BASE))
        goto fail;
    return 0;
fail:
    ee_mem_shutdown();
    return -1;
}

void ee_mem_shutdown(void)
{
    size_t i;

    for (i = 0; i < NUM_RAM_VIEWS; i++) {
        if (ram_views[i] != NULL)
            UnmapViewOfFile(ram_views[i]);
        ram_views[i] = NULL;
    }
    if (spr_view != NULL)
        VirtualFree(spr_view, 0, MEM_RELEASE);
    spr_view = NULL;
    if (ram_section != NULL)
        CloseHandle(ram_section);
    ram_section = NULL;
}

#else

#include <sys/mman.h>
#include <unistd.h>

static int ram_fd = -1;
static int ram_mapped[NUM_RAM_VIEWS];
static int spr_mapped;

/* MAP_FIXED_NOREPLACE fails instead of silently replacing something the
 * process already has at that address (libc, the heap, the executable). */
static int map_at(uint32_t addr, size_t size, int fd, off_t offset)
{
    int flags = MAP_FIXED_NOREPLACE | (fd < 0 ? MAP_PRIVATE | MAP_ANONYMOUS : MAP_SHARED);
    void *p = mmap(ee_ptr(addr), size, PROT_READ | PROT_WRITE, flags, fd, offset);

    if (p == MAP_FAILED)
        return -1;
    if (p != ee_ptr(addr)) {
        /* Kernels older than 4.17 ignore the flag and treat it as a hint. */
        munmap(p, size);
        return -1;
    }
    return 0;
}

int ee_mem_init(void)
{
    size_t i;

    ram_fd = memfd_create("ee-ram", 0);
    if (ram_fd < 0 || ftruncate(ram_fd, EE_RAM_SIZE) != 0)
        goto fail;
    for (i = 0; i < NUM_RAM_VIEWS; i++) {
        if (map_at(ram_bases[i], RAM_MAP_SIZE, ram_fd, EE_RAM_USER_BASE) != 0)
            goto fail;
        ram_mapped[i] = 1;
    }
    if (map_at(EE_SPR_BASE, EE_SPR_SIZE, -1, 0) != 0)
        goto fail;
    spr_mapped = 1;
    return 0;
fail:
    ee_mem_shutdown();
    return -1;
}

void ee_mem_shutdown(void)
{
    size_t i;

    for (i = 0; i < NUM_RAM_VIEWS; i++) {
        if (ram_mapped[i])
            munmap(ee_ptr(ram_bases[i]), RAM_MAP_SIZE);
        ram_mapped[i] = 0;
    }
    if (spr_mapped)
        munmap(ee_ptr(EE_SPR_BASE), EE_SPR_SIZE);
    spr_mapped = 0;
    if (ram_fd >= 0)
        close(ram_fd);
    ram_fd = -1;
}

#endif

static int in_range(uint32_t addr, uint32_t size, uint32_t base, uint32_t len)
{
    return addr >= base && size <= len && addr - base <= len - size;
}

int ee_mem_valid(uint32_t addr, uint32_t size)
{
    size_t i;

    for (i = 0; i < NUM_RAM_VIEWS; i++) {
        if (in_range(addr, size, ram_bases[i], RAM_MAP_SIZE))
            return 1;
    }
    return in_range(addr, size, EE_SPR_BASE, EE_SPR_SIZE);
}

/* ELF32 little-endian MIPS; only the fields the loader needs. */
#define ELF_PT_LOAD 1
#define ELF_EM_MIPS 8

static uint16_t rd16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }
static uint32_t rd32(const uint8_t *p)
{
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

int ee_load_elf(const void *image, size_t size, uint32_t *entry)
{
    const uint8_t *elf = (const uint8_t *)image;
    uint32_t phoff, phentsize, phnum, i;

    if (size < 52 || memcmp(elf, "\x7F" "ELF", 4) != 0)
        return -1;
    if (elf[4] != 1 || elf[5] != 1 || rd16(elf + 18) != ELF_EM_MIPS)
        return -1; /* not ELF32, not little-endian, or not MIPS */

    phoff = rd32(elf + 28);
    phentsize = rd16(elf + 42);
    phnum = rd16(elf + 44);
    if (phentsize < 32 || phoff > size || phnum > (size - phoff) / phentsize)
        return -1;

    for (i = 0; i < phnum; i++) {
        const uint8_t *ph = elf + phoff + i * phentsize;
        uint32_t offset = rd32(ph + 4), vaddr = rd32(ph + 8);
        uint32_t filesz = rd32(ph + 16), memsz = rd32(ph + 20);

        if (rd32(ph) != ELF_PT_LOAD || memsz == 0)
            continue;
        if (filesz > memsz || offset > size || filesz > size - offset)
            return -1;
        if (!ee_mem_valid(vaddr, memsz))
            return -1;
        memcpy(ee_ptr(vaddr), elf + offset, filesz);
        memset(ee_ptr(vaddr + filesz), 0, memsz - filesz);
    }
    *entry = rd32(elf + 24);
    return 0;
}
