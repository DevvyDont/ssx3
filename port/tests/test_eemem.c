/*
 * With no arguments: checks the EE memory map and the ELF loader on a small
 * synthetic image. With a path: loads that executable (the retail
 * SLUS_207.72) and checks its entry point and that bss was cleared.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "check.h"
#include "ps2os/eemem.h"

static void put16(uint8_t *p, uint16_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }
static void put32(uint8_t *p, uint32_t v)
{
    put16(p, (uint16_t)v);
    put16(p + 2, (uint16_t)(v >> 16));
}

/* One PT_LOAD of 4 file bytes at 0x00200000 with 12 bytes of bss. */
static size_t make_elf(uint8_t *elf, uint32_t vaddr)
{
    memset(elf, 0, 128);
    memcpy(elf, "\x7F" "ELF", 4);
    elf[4] = 1; /* ELF32 */
    elf[5] = 1; /* little-endian */
    put16(elf + 18, 8);           /* EM_MIPS */
    put32(elf + 24, vaddr + 8);   /* entry */
    put32(elf + 28, 52);          /* phoff */
    put16(elf + 42, 32);          /* phentsize */
    put16(elf + 44, 1);           /* phnum */
    put32(elf + 52 + 0, 1);       /* PT_LOAD */
    put32(elf + 52 + 4, 96);      /* offset */
    put32(elf + 52 + 8, vaddr);   /* vaddr */
    put32(elf + 52 + 16, 4);      /* filesz */
    put32(elf + 52 + 20, 16);     /* memsz */
    memcpy(elf + 96, "\x11\x22\x33\x44", 4);
    return 100;
}

static void test_map(void)
{
    volatile uint32_t *cached = ee_ptr(0x00200000);
    volatile uint32_t *uncached = ee_ptr(EE_RAM_UNCACHED + 0x00200000);
    volatile uint32_t *accel = ee_ptr(EE_RAM_UNCACHED_ACC + 0x00200000);
    volatile uint8_t *spr = ee_ptr(EE_SPR_BASE);

    *cached = 0xDEADBEEF;
    CHECK(*uncached == 0xDEADBEEF);
    CHECK(*accel == 0xDEADBEEF);
    *accel = 0x12345678;
    CHECK(*cached == 0x12345678);

    spr[0] = 0xAB;
    spr[EE_SPR_SIZE - 1] = 0xCD;
    CHECK(spr[0] == 0xAB && spr[EE_SPR_SIZE - 1] == 0xCD);

    CHECK(ee_mem_valid(EE_RAM_USER_BASE, 4));
    CHECK(ee_mem_valid(EE_RAM_SIZE - 4, 4));
    CHECK(!ee_mem_valid(EE_RAM_SIZE - 4, 8));
    CHECK(!ee_mem_valid(0x00000000, 4)); /* kernel area is not mapped */
    CHECK(!ee_mem_valid(EE_SPR_BASE + EE_SPR_SIZE, 1));
    CHECK(!ee_mem_valid(0xFFFFFFF0, 0x20)); /* wraps */
}

static void test_loader(void)
{
    uint8_t elf[128];
    uint32_t entry = 0;
    size_t size = make_elf(elf, 0x00200000);
    uint8_t *mem = ee_ptr(0x00200000);

    memset(mem, 0xFF, 16);
    CHECK(ee_load_elf(elf, size, &entry) == 0);
    CHECK(entry == 0x00200008);
    CHECK(memcmp(mem, "\x11\x22\x33\x44", 4) == 0);
    CHECK(mem[4] == 0 && mem[15] == 0);

    /* Truncated image, wrong machine, and a segment outside EE memory. */
    CHECK(ee_load_elf(elf, 98, &entry) != 0);
    elf[18] = 3;
    CHECK(ee_load_elf(elf, size, &entry) != 0);
    make_elf(elf, 0x00000000);
    CHECK(ee_load_elf(elf, size, &entry) != 0);
}

static void test_retail(const char *path)
{
    size_t size;
    void *image = SDL_LoadFile(path, &size);
    uint32_t entry = 0;

    CHECK(image != NULL);
    if (image == NULL)
        return;
    /* Dirty the bss first so the check below proves the loader cleared it. */
    memset(ee_ptr(0x004A4BF4), 0xFF, 0x0053EADC - 0x004A4BF4);
    CHECK(ee_load_elf(image, size, &entry) == 0);
    CHECK(entry == 0x00100008);
    /* The single PT_LOAD ends its file bytes at 0x004A4BF4 and its memory at
     * 0x0053EADC; everything between is bss. */
    CHECK(memcmp(ee_ptr(0x00100000), (uint8_t *)image + 0x1000, 0x3A4BF4) == 0);
    CHECK(*(uint8_t *)ee_ptr(0x004A4BF4) == 0);
    CHECK(*(uint8_t *)ee_ptr(0x0053EADB) == 0);
    SDL_free(image);
}

int main(int argc, char **argv)
{
    if (ee_mem_init() != 0) {
        fprintf(stderr, "ee_mem_init failed\n");
        return 1;
    }
    if (argc > 1) {
        test_retail(argv[1]);
    } else {
        test_map();
        test_loader();
    }
    ee_mem_shutdown();
    /* A second init must succeed, so shutdown released every mapping. */
    CHECK(ee_mem_init() == 0);
    ee_mem_shutdown();
    return CHECK_DONE();
}
