#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FCDC8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FC0D8(void);
extern int func_003FA240(int);
extern int func_003FB810(int);
int func_003FCDC8(int a) {
    int result;
    if (func_003FC0D8()) {
        result = func_003FB810(func_003FA240(a));
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
int func_003FCF00(int userPtr, int allocator, int size, int memType) {
    int allocInfo[2];
    int r;
    allocInfo[0] = 4;
    allocInfo[1] = 1;
    func_003FB858(memType, allocInfo, &allocInfo[1]);
    r = func_003FB898(size, memType);
    if (userPtr) {
        func_00266760(allocator, userPtr - 0xC, r, allocInfo[0]);
        r = 0;
    } else {
        r = 4;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FCF98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FB898(int, int);
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
int func_003FCFD8(int a, int b) {
    int obj[12];
    int hdr[3];
    int r;
    int* p;
    if (func_003FD558(obj, a, b) == 0) {
        return -1;
    }
    r = func_003FD758(obj, str_reloctable);
    if (r == 0) {
        return -2;
    }
    hdr[0] = func_003FD908 - func_003FD810;
    hdr[1] = r;
    hdr[2] = (int)func_003FD810;
    func_003FD660((int)obj, (int)hdr);
    p = D_0045152C_i;
    *p = func_003FD758(obj, str_DirtyDnasRelInit);
    if (*p == 0) {
        return -3;
    }
    p = D_00451530_i;
    *p = func_003FD758(obj, str_DirtyDnasRelUpdt);
    if (*p == 0) {
        return -4;
    }
    p = D_00451534_i;
    *p = func_003FD758(obj, str_DirtyDnasRelExit);
    if (*p == 0) {
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
void func_003FD0D8(int a) {
    D_0045152C[0](a, 0x1020000);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD100);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00451530[])(void);
int func_003FD100(void) {
    return D_00451530[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD128);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00451534[])(void);
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
void func_003FD150(int a, int b, int (*fn)(int), int arg) {
    int none;
    D_0045152C[0](a, 0x1020000);
    none = 0;
    do {
    } while (D_00451530_a[0](fn(arg) == none) == 1);
    D_00451534_a[0](b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD1D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FD1D8(int *elfImage) {
    return *elfImage == 0x464C457F;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2FDDC8", func_003FD1F0);
#ifdef SKIP_ASM
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
