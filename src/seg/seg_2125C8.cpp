#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2125C8", cAIAnimEventMap_getBlendInTime);
#ifdef SKIP_ASM
struct sEntry_15C8 {
    char b[0x1C];
};
extern sEntry_15C8 D_00446990[];

/**
 * Returns the anim event map entry for an animation.
 *
 * Despite the name it returns the whole 0x1C-byte entry of D_00446990; the blend-in time is the float at
 * offset 0x18, and callers also read the entry's first four words.
 *
 * @param animIndex  animation index
 * @return pointer to the entry
 */
extern "C" void *cAIAnimEventMap_getBlendInTime(int animIndex) {
    return &D_00446990[animIndex];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2125C8", cAIAnimEventMap_initMaps);
#ifdef SKIP_ASM
struct Range_15E0 {
    short start;
    short count;
};

struct RangeTable_15E0 {
    Range_15E0 r[0x1B6];
};

struct Groups_15E0 {
    int v[0x1D8];
};

extern Groups_15E0 D_00489D60;

// PORT: the unit declares this as void(int); defined under an asm label with its real type
extern "C" void cAIAnimEventMap_initMaps_impl(RangeTable_15E0* t) __asm__("cAIAnimEventMap_initMaps");
/**
 * Builds the AI animation event map's group ranges from the static group table.
 *
 * Clears the counts of the 437 groups, then walks the 472 entries of D_00489D60 (a group number per entry)
 * and records, for each group, the index where its run starts and how many entries it has. A group is
 * expected to occupy one contiguous run; the start of a group with no entries is left as it was.
 *
 * @param rangeTable  the map: one (start, count) pair of shorts per group
 */
extern "C" void cAIAnimEventMap_initMaps_impl(RangeTable_15E0* rangeTable)
{
    Groups_15E0 tab = D_00489D60;
    int prev;
    int i, j;
    for (j = 436; j >= 0; j--) rangeTable->r[j].count = 0;
    prev = 438;
    for (i = 0; i < 472; i++) {
        int group = tab.v[i];
        if (group != prev) {
            prev = group;
            rangeTable->r[group].start = i;
        }
        rangeTable->r[prev].count++;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2125C8", func_00311710);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sWeighted_1710 {
    int value;
    int weight;
    int flags;
};
extern sWeighted_1710 D_00449960[];
unsigned int AIrand();

extern "C" int func_00311710(RangeTable_15E0* t, int idx, int mask)
{
    int start = t->r[idx].start;
    int i, end, total;
    if (t->r[idx].count == 1) return D_00449960[start].value;
    end = start + t->r[idx].count;
    total = 0;
    for (int j = start; j < end; j++) {
        if ((D_00449960[j].flags & mask) == mask) total += D_00449960[j].weight;
    }
    int r = AIrand() % total;
    for (i = start; i < end; i++) {
        if ((D_00449960[i].flags & mask) == mask) {
            r -= D_00449960[i].weight;
            if (r <= 0) return D_00449960[i].value;
        }
    }
    return D_00449960[end - 1].value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2125C8", func_00311860);
#ifdef SKIP_ASM
extern char str_error[];

extern "C" void *func_00311860(void) {
    return str_error;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2125C8", cRiderAnimBase_initAIAnimEventMap);
#ifdef SKIP_ASM
extern "C" int cMemMan_alloc(int, char *, int, int);
extern "C" void cAIAnimEventMap_initMaps(int);
extern char str_AIAnimEventMap[];
extern int cRiderAnimBase_AIAnimEventMap;

/**
 * Allocates the shared AI animation event map and fills it.
 *
 * Allocates 0x6D4 bytes ("AIAnimEventMap"), keeps the pointer in cRiderAnimBase_AIAnimEventMap and runs
 * cAIAnimEventMap_initMaps() on it.
 */
extern "C" void cRiderAnimBase_initAIAnimEventMap(void) {
    int temp_2;

    temp_2 = cMemMan_alloc(1748, str_AIAnimEventMap, 0, 0);
    cRiderAnimBase_AIAnimEventMap = temp_2;
    cAIAnimEventMap_initMaps(temp_2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2125C8", func_003118A8);
#ifdef SKIP_ASM
extern "C" void operator_delete__FPi(int);
extern int cRiderAnimBase_AIAnimEventMap;

extern "C" void func_003118A8(void) {
    operator_delete__FPi(cRiderAnimBase_AIAnimEventMap);
    cRiderAnimBase_AIAnimEventMap = 0;
}
#endif
