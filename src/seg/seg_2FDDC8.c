#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FCDC8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FC0D8(void);
extern int func_003FA240(int);
extern int func_003FB810(int);
/**
 * Hands the save description to the request layer (func_003FA240).
 *
 * @param saveDesc  the 976-byte save description
 * @return 0; 2 when the manager is not open, or the request error + 100
 */
int func_003FCDC8(int saveDesc) {
    int result;
    if (func_003FC0D8()) {
        result = func_003FB810(func_003FA240(saveDesc));
    } else {
        result = 2;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FCE10);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003FB858(int, int*, int*);
extern int func_003FB898(int, int);
extern char* func_00266728(int, int, int);
extern void func_00416210(char*, int, int);
extern char str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL[];
/**
 * Allocates a save-record buffer: @p size bytes plus a 12-byte header holding "TAG_SENTINEL", zeroed.
 *
 * @param out  receives the address just past the header
 * @param allocator  heap
 * @param size  data size
 * @param memType  memory type (func_003FB858)
 * @return 0; 4 when @p out is 0, 6 when the allocation fails
 */
int func_003FCE10(char** out, int allocator, int size, int memType) {
    int allocInfo[2];
    int n;
    allocInfo[0] = 4;
    allocInfo[1] = 1;
    func_003FB858(memType, allocInfo, &allocInfo[1]);
    n = func_003FB898(size, memType);
    if (out == 0) {
        return 4;
    }
    *out = func_00266728(allocator, n, allocInfo[0]);
    if (*out != 0) {
        func_00416210(*out, 0, n);
        __builtin_memcpy(*out, str_TAG_SENTINELTAG_SENTINELTAG_SENTINEL, 12);
        *out += 12;
        return 0;
    }
    return 6;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FCF00);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003FB858(int, int*, int*);
extern int func_003FB898(int, int);
extern void func_00266760(int, int, int, int);
/**
 * Frees a save-record buffer from func_003FCE10 (header included).
 *
 * @param userPtr  the buffer
 * @param allocator  heap
 * @param size  data size
 * @param memType  memory type
 * @return 0, or 4 when @p userPtr is 0
 */
int func_003FCF00(int userPtr, int allocator, int size, int memType) {
    int allocInfo[2];
    int result;
    allocInfo[0] = 4;
    allocInfo[1] = 1;
    func_003FB858(memType, allocInfo, &allocInfo[1]);
    result = func_003FB898(size, memType);
    if (userPtr) {
        func_00266760(allocator, userPtr - 0xC, result, allocInfo[0]);
        result = 0;
    } else {
        result = 4;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FCF98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FB898(int, int);
/**
 * Returns the stored size of a save record: @p size plus its 12-byte header (func_003FB898, type 2).
 *
 * @param size  data size
 * @param out  receives the stored size
 * @return 0, or 4 when @p out is 0
 */
int func_003FCF98(int size, int *out) {
    int result;
    if (out) {
        *out = func_003FB898(size, 2);
        result = 0;
    } else {
        result = 4;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FCFD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_reloctable[];
extern char str_DirtyDnasRelInit[];
extern char str_DirtyDnasRelUpdt[];
extern char str_DirtyDnasRelExit[];
extern int D_0045152C_i[] __asm__("D_0045152C");
extern int D_00451530_i[] __asm__("D_00451530");
extern int D_00451534_i[] __asm__("D_00451534");
extern char func_003FD810[];
extern char func_003FD908[];
extern int func_003FD558(int*, int, int);
extern void func_003FD660(int, int);
extern int func_003FD758(int*, char*);
extern void FlushCache(int);
/**
 * Loads the DNAS module (an ELF relocatable image read from disc) so its entry points can be called.
 *
 * Checks the image (func_003FD558, -1), finds its "reloctable" symbol (-2), and relocates .text, .data and .rodata with
 * the context {size of the import table func_003FD810, that symbol, func_003FD810}, so the module's references into its
 * own reloctable resolve to the game's functions. Then looks up DirtyDnasRelInit / Updt / Exit into D_0045152C /
 * D_00451530 / D_00451534 (-3 .. -5) and flushes the data and instruction caches. The DNAS manager (func_00256460)
 * calls it with the file it loaded.
 *
 * @param image  the image
 * @param imageSize  its size
 * @return 0, or -1 .. -5
 */
int func_003FCFD8(int image, int imageSize) {
    int obj[12];
    int hdr[3];
    int relocTable;
    int* entrySlot;
    if (func_003FD558(obj, image, imageSize) == 0) {
        return -1;
    }
    relocTable = func_003FD758(obj, str_reloctable);
    if (relocTable == 0) {
        return -2;
    }
    hdr[0] = func_003FD908 - func_003FD810;
    hdr[1] = relocTable;
    hdr[2] = (int)func_003FD810;
    func_003FD660((int)obj, (int)hdr);
    entrySlot = D_0045152C_i;
    *entrySlot = func_003FD758(obj, str_DirtyDnasRelInit);
    if (*entrySlot == 0) {
        return -3;
    }
    entrySlot = D_00451530_i;
    *entrySlot = func_003FD758(obj, str_DirtyDnasRelUpdt);
    if (*entrySlot == 0) {
        return -4;
    }
    entrySlot = D_00451534_i;
    *entrySlot = func_003FD758(obj, str_DirtyDnasRelExit);
    if (*entrySlot == 0) {
        return -5;
    }
    FlushCache(0);
    FlushCache(2);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD0D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void (*D_0045152C[])(int, int);
/**
 * Calls the DNAS module's DirtyDnasRelInit with @p initParams and 0x1020000.
 *
 * @param initParams  the DNAS parameters (func_002562D0 passes its block at +0x24)
 */
void func_003FD0D8(int initParams) {
    D_0045152C[0](initParams, 0x1020000);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD100);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00451530[])(void);
/**
 * Calls the DNAS module's DirtyDnasRelUpdt (the caller's argument passes through) and returns its result.
 *
 * @return the module's result (func_002565E0 goes on to Exit once it is 0)
 */
int func_003FD100(void) {
    return D_00451530[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD128);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00451534[])(void);
/**
 * Calls the DNAS module's DirtyDnasRelExit (the caller's argument, an info buffer, passes through) and returns its
 * result.
 *
 * @return the module's result
 */
int func_003FD128(void) {
    return D_00451534[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD150);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void (*D_0045152C[])(int, int);
extern int (*D_00451530_a[])(int) __asm__("D_00451530");
extern int (*D_00451534_a[])(int) __asm__("D_00451534");
/**
 * Runs the DNAS module to completion: Init with @p initParams and 0x1020000, Updt repeatedly with (@p fn(@p fnArg) == 0) until it
 * returns something other than 1, then Exit with @p exitArg. Nothing in retail calls it.
 *
 * @param initParams  the DNAS parameters
 * @param exitArg  argument for Exit
 * @param fn  callback polled before each update
 * @param fnArg  its argument
 */
void func_003FD150(int initParams, int exitArg, int (*fn)(int), int fnArg) {
    int none;
    D_0045152C[0](initParams, 0x1020000);
    none = 0;
    do {
    } while (D_00451530_a[0](fn(fnArg) == none) == 1);
    D_00451534_a[0](exitArg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD1D8);
#ifdef SKIP_ASM
/**
 * Returns whether an image starts with the ELF magic "\x7FELF".
 *
 * @param elfImage  the image
 * @return 1 or 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FD1D8(int *elfImage) {
    return *elfImage == 0x464C457F;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD1F0);
#ifdef SKIP_ASM
/**
 * Returns the address of section header @p n of a loaded ELF image (40 bytes each from e_shoff).
 *
 * @param elfModule  the module descriptor (image first)
 * @param n  section index
 * @return the section header's address
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FD1F0(int *elfModule, int n) {
    int base = *elfModule;
    return base + *(int *)(base + 0x20) + n * 40;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD210);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_004165A8(const char*, const char*);
struct sShdr { int name; int type; int flags; int addr; int offset; int size; int link; int info; int align; int entsize; };
struct sEhdr { char pad[0x20]; int shoff; char pad2[0xC]; unsigned short shnum; unsigned short shstrndx; };
/**
 * Finds a section of a loaded ELF image by name (through the section-name string table).
 *
 * @param elfModule  the module descriptor (image first)
 * @param name  section name
 * @return the section header, or 0
 */
struct sShdr* func_003FD210(int* elfModule, char* name) {
    struct sEhdr* elfHeader = (struct sEhdr*)elfModule[0];
    struct sShdr* sh = (struct sShdr*)((char*)elfHeader + elfHeader->shoff);
    int strtab = sh[elfHeader->shstrndx].offset;
    unsigned int i = 0;
    while (i < elfHeader->shnum) {
        if (func_004165A8((char*)elfHeader + (strtab + sh->name), name) == 0) {
            return sh;
        }
        i++;
        sh++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD2B8);
#ifdef SKIP_ASM
/**
 * Returns the image offset of an ELF section index for symbol addressing: the section's file offset for indexes up to
 * 0xFEFF, 0xFFF00000 for SHN_ABS (0xFFF1) and 0 for the other reserved indexes.
 *
 * @param elfModule  the module descriptor
 * @param n  section index
 * @return the offset
 */
// From peak3project's fork (github.com/peak3project/ssx3).
unsigned func_003FD2B8(int *elfModule, unsigned n) {
    unsigned sectionBase;
    if (n <= 65279) {
        sectionBase = ((unsigned *)func_003FD1F0(elfModule, n))[4];
    } else {
        sectionBase = n == 65521 ? 0xFFF00000 : 0;
    }
    return sectionBase;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD2F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// (prototype from their definition at line 207)
struct sShdr* func_003FD210_D2F8(int* p, char* name) __asm__("func_003FD210");

struct sHiEnt { unsigned int idx; unsigned int val; };
extern int func_003FD6E0_D2F8(int*, int, char*) __asm__("func_003FD6E0");
/**
 * Applies one REL section of a loaded ELF image to its target section, in place.
 *
 * For each 8-byte entry: resolves the symbol (func_003FD6E0); an address inside the context's range [@p ci[1],
 * @p ci[1] + @p ci[0]) is replaced by the word at the same offset of @p ci[2]. R_MIPS_32 adds the address, R_MIPS_26
 * adds it shifted right by 2 (unmasked), R_MIPS_HI16 entries are queued (up to 31), and R_MIPS_LO16 completes the low
 * half and fills every queued HI16 with the carried high half. Quirk: the LO16 sum uses only the first queued HI16's
 * addend. Does nothing when either section is missing.
 *
 * @param oi  the module descriptor
 * @param targetName  name of the section to patch
 * @param relName  name of its REL section
 * @param ci  relocation context {range size, range start, replacement table}
 */
void func_003FD2F8(int oi, char* targetName, char* relName, int ci) {
    int* elfModule = (int*)oi;
    int* ctx = (int*)ci;
    char symName[0x100];
    struct sHiEnt hi[31];
    struct sShdr* tsec;
    struct sShdr* rsec;
    tsec = func_003FD210_D2F8(elfModule, targetName);
    rsec = func_003FD210_D2F8(elfModule, relName);
    if (tsec != 0 && rsec != 0) {
        unsigned int cnt = 0;
        unsigned int n = (unsigned int)rsec->size >> 3;
        unsigned int* ent = (unsigned int*)(elfModule[0] + rsec->offset);
        unsigned int* data = (unsigned int*)(elfModule[0] + tsec->offset);
        unsigned int i;
        for (i = 0; i < n; i++, ent += 2) {
            unsigned int idx = ent[0] >> 2;
            unsigned int type = ent[1] & 0xFF;
            unsigned int val = data[idx];
            unsigned int sym = func_003FD6E0_D2F8(elfModule, ent[1] >> 8, symName);
            if (sym >= (unsigned int)ctx[1] && sym < (unsigned int)ctx[1] + ctx[0]) {
                sym = *(unsigned int*)(sym - ctx[1] + ctx[2]);
            }
            switch (type) {
            case 2:
                val += sym;
                data[idx] = val;
                break;
            case 4:
                val += sym >> 2;
                data[idx] = val;
                break;
            case 5:
                if (cnt < 31) {
                    hi[cnt].idx = idx;
                    hi[cnt].val = val;
                    cnt++;
                }
                break;
            case 6: {
                unsigned int j;
                unsigned int carry;
                sym += ((hi[0].val & 0xFFFF) << 16) + (short)val;
                data[idx] = (val & 0xFFFF0000) | (sym & 0xFFFF);
                carry = (sym & 0x8000) != 0;
                for (j = 0; j < cnt; j++) {
                    data[hi[j].idx] = (hi[j].val & 0xFFFF0000) | ((sym >> 16) + carry);
                }
                cnt = 0;
                break;
            }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD558);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// (prototype from their definition at line 207)
struct sShdr* func_003FD210_D558(int* p, char* name) __asm__("func_003FD210");

extern char str_text[];
extern char str_symtab[];
extern char str_strtab[];
extern int func_003FD1D8_D558(int*) __asm__("func_003FD1D8");
/**
 * Opens an ELF relocatable image as a module descriptor.
 *
 * Requires the ELF magic and e_type 1 with e_machine 8 (MIPS). Stores the image (+0x0), @p x (+0x4), the .text address
 * and size (+0x8, +0xC), the .symtab address and symbol count (+0x10, +0x14) and the .strtab address (+0x20).
 *
 * @param elfModule  the descriptor to fill
 * @param imgi  the image
 * @param x  the image size
 * @return 1 when it is a MIPS relocatable, else 0
 */
int func_003FD558(int* elfModule, int imgi, int x) {
    int* img = (int*)imgi;
    int ok = 0;
    struct sShdr* textSec;
    struct sShdr* symtabSec;
    struct sShdr* strtabSec;
    if (func_003FD1D8_D558(img)) {
        if (img[4] == 0x80001) {
            elfModule[1] = x;
            elfModule[0] = (int)img;
            ok = 1;
            textSec = func_003FD210_D558(elfModule, str_text);
            symtabSec = func_003FD210_D558(elfModule, str_symtab);
            strtabSec = func_003FD210_D558(elfModule, str_strtab);
            if (textSec) {
                elfModule[2] = (int)img + textSec->offset;
                elfModule[3] = textSec->size;
            }
            if (strtabSec) {
                elfModule[8] = (int)img + strtabSec->offset;
            }
            if (symtabSec) {
                elfModule[5] = (unsigned int)symtabSec->size >> 4;
                elfModule[4] = (int)img + symtabSec->offset;
            }
        }
    }
    return ok;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD660);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_text[];
extern char str_rel_text[];
extern char str_data[];
extern char str_rel_data[];
extern char str_rodata[];
extern char str_rel_rodata[];
extern void func_003FD2F8(int, char*, char*, int);
/**
 * Relocates a loaded ELF module's .text, .data and .rodata with their .rel.* sections (func_003FD2F8).
 *
 * @param elfModule  the module descriptor
 * @param relocCtx  the relocation context
 */
void func_003FD660(int elfModule, int relocCtx) {
    func_003FD2F8(elfModule, str_text, str_rel_text, relocCtx);
    func_003FD2F8(elfModule, str_data, str_rel_data, relocCtx);
    func_003FD2F8(elfModule, str_rodata, str_rel_rodata, relocCtx);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD6E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char* strcpy(char* d, const char* s);
extern unsigned func_003FD2B8(int* p, unsigned n);
struct sSym { int name; int val; int size; unsigned char info; unsigned char other; unsigned short shndx; };
/**
 * Returns the address of symbol @p idx of a loaded ELF module (image + section offset + value) and copies its name to
 * @p dst.
 *
 * @param elfModule  the module descriptor
 * @param idx  symbol index
 * @param dst  receives the name
 * @return the address, or 0 without a symbol or string table
 */
int func_003FD6E0(int* elfModule, int idx, char* dst) {
    int symAddr = 0;
    int symtab = elfModule[4];
    if (symtab) {
        int strtab = elfModule[8];
        if (strtab) {
            struct sSym* sym = (struct sSym*)(idx * 16 + symtab);
            strcpy(dst, (char*)(strtab + sym->name));
            symAddr = elfModule[0] + func_003FD2B8(elfModule, sym->shndx) + sym->val;
        }
    }
    return symAddr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD758);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_004165A8(const char*, const char*);
extern unsigned func_003FD2B8_D758(int* p, unsigned n) __asm__("func_003FD2B8");
/**
 * Finds a symbol of a loaded ELF module by name and returns its address (image + section offset + value).
 *
 * @param elfModule  the module descriptor
 * @param name  symbol name
 * @return the address, or 0 when not found
 */
int func_003FD758(int* elfModule, char* name) {
    int symAddr = 0;
    char* symtab = (char*)elfModule[4];
    if (symtab) {
        if (elfModule[8]) {
            struct sSym* sym = (struct sSym*)symtab;
            unsigned int i;
            for (i = 0; i < (unsigned int)elfModule[5]; i++) {
                if (func_004165A8((char*)(elfModule[8] + sym->name), name) == 0) {
                    symAddr = elfModule[0] + func_003FD2B8_D758(elfModule, sym->shndx) + sym->val;
                    break;
                }
                sym++;
            }
        }
    }
    return symAddr;
}
#endif
