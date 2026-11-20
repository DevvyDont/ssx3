#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EDF50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_004963C0[];
/**
 * Compares two tag strings without regard to case; either may be quoted.
 *
 * A string that starts with " or ' ends at the matching quote, an unquoted one at the first control character (below
 * 0x20). Characters are compared through the upper-case table D_004963C0. No retail code calls it.
 *
 * @param leftText  the first string
 * @param rightText  the second string (not checked for null)
 * @return -1 when @p leftText is null or empty, else the difference of the first differing characters (0 when equal)
 */
int func_003EDF50(char *leftText, char *rightText) {
    unsigned qa, qb;
    unsigned ca, cb;
    if (leftText == 0 || *leftText == 0) return -1;
    if (*leftText == '"' || *leftText == '\'') {
        qa = *(unsigned char *)leftText;
        leftText++;
    } else {
        qa = 0;
    }
    if (*rightText == '"' || *rightText == '\'') {
        qb = *(unsigned char *)rightText;
        rightText++;
    } else {
        qb = 0;
    }
    do {
        ca = D_004963C0[(int)*leftText];
        leftText++;
        if (ca == qa || (ca < 0x20 && qa == 0)) ca = 0;
        cb = D_004963C0[(int)*rightText];
        rightText++;
        if (cb == qb || (cb < 0x20 && qb == 0)) cb = 0;
    } while (ca == cb && ca != 0);
    return ca - cb;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE028);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int* func_002591E8(int);
extern void func_00416210(void*, int, int);
extern void func_0041605C(void*, void*, int);
extern void func_00259210(void*);
/**
 * Grows a sorted list's node pool and item array.
 *
 * The list is a 68-byte block of words: [0] grow count, [1] grow percent, [2] re-sort counter, [3] change counter, [4]
 * free nodes, [5] items passing the filter, [6] items, [7]-[9] filter param, context and function, [10]-[12] sort
 * param, context and function, [13] a user word, [14] free-node list, [15] chunk list, [16] item array.
 *
 * The number of new nodes is the grow count when the grow percent is not positive, else items * 100 / grow percent
 * (averaged with the grow count when that is set), at least 2. A "Lobby" heap chunk (func_002591E8) of nodes + 2 words
 * is zeroed and linked into the chunk list ([15]); its first word is the link, its last -1, and the words between
 * become free nodes ([14], [4]). The item array is reallocated at (items + free) * 8 bytes, keeping the old pointers.
 *
 * @param list  the sorted list
 * @return the number of nodes added, or 0 when the allocation fails
 */
int func_003EE028(int* list) {
    int k;
    int bytes;
    int* chunk;
    int* entry;
    int* itemArray;
    if (list[1] <= 0) {
        k = list[0] > 0 ? list[0] : 1;
    } else if (list[0] == 0) {
        k = list[6] * 100 / list[1];
    } else {
        k = (list[0] + list[6] * 100 / list[1]) / 2;
    }
    k = k > 1 ? k : 2;
    k += 2;
    bytes = k * 4;
    chunk = func_002591E8(bytes);
    if (!chunk) {
        return 0;
    }
    func_00416210(chunk, 0, bytes);
    k = 1;
    chunk[0] = list[15];
    *(unsigned*)(bytes + (int)chunk - 4) = 0xFFFFFFFF;
    list[15] = (int)chunk;
    if (chunk[1] == 0) {
        entry = chunk + 1;
        do {
            *entry = list[14];
            list[14] = (int)entry;
            entry++;
            k++;
        } while (*entry == 0);
    }
    list[4] += k - 1;
    itemArray = func_002591E8((list[6] + list[4]) * 8);
    if (list[16]) {
        func_0041605C(itemArray, (void*)list[16], list[6] * 4);
        func_00259210((void*)list[16]);
    }
    list[16] = (int)itemArray;
    return k - 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE1B0);
#ifdef SKIP_ASM
/**
 * Compares two list items with the list's sort function (the adapter func_003EE620 sorts with).
 *
 * Calls sort function [12] with sort context [11], sort param [10] and the two items' values.
 *
 * @param list  the sorted list
 * @param lhs  the first item slot
 * @param rhs  the second item slot
 * @return the sort function's result
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE1B0(int* list, int** lhs, int** rhs) {
    return ((int (*)(int, int, int, int))list[12])(list[11], list[10], **lhs, **rhs);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE1E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int* func_002591E8(int);
extern void func_00416210(void*, int, int);
extern int func_003EE028(int*);
/**
 * Creates an empty sorted list with a first pool of nodes.
 *
 * The list is a 68-byte block of words: [0] grow count, [1] grow percent, [2] re-sort counter, [3] change counter, [4]
 * free nodes, [5] items passing the filter, [6] items, [7]-[9] filter param, context and function, [10]-[12] sort
 * param, context and function, [13] a user word, [14] free-node list, [15] chunk list, [16] item array.
 *
 * Allocates and zeroes the 68-byte block ("Lobby" heap), grows it once for @p initialSize items (func_003EE028), then
 * stores the grow count and grow percent for later growth.
 *
 * @param initialSize  the number of nodes to preallocate
 * @param growCount  nodes added per later growth
 * @param growPercent  growth as a percentage, or 0 to use the count
 * @return the new list, or 0 when out of memory
 */
int* func_003EE1E8(int initialSize, int growCount, int growPercent) {
    int* list = func_002591E8(68);
    func_00416210(list, 0, 68);
    list[0] = initialSize;
    func_003EE028(list);
    list[0] = growCount;
    list[1] = growPercent;
    return list;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE260);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_00259210(void*);
/**
 * Destroys a sorted list: frees its node chunks, its item array and the list itself.
 *
 * @param list  the sorted list
 */
void func_003EE260(int* list) {
    if (list[15]) {
        do {
            int* chunk = (int*)list[15];
            list[15] = *chunk;
            func_00259210(chunk);
        } while (list[15]);
    }
    func_00259210((void*)list[16]);
    func_00259210(list);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE2B8);
#ifdef SKIP_ASM
/**
 * Removes every item from a sorted list.
 *
 * Returns all item nodes to the free list, adds them to the free count, clears the item and passing counts and sets the
 * re-sort counter to 1. No retail code calls it.
 *
 * @param list  the sorted list
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE2B8(int* list) {
    int i;
    for (i = 0; i < list[6]; i++) {
        int* item = ((int**)list[16])[i];
        item[0] = list[14];
        list[14] = (int)item;
    }
    list[2] = 1;
    list[4] += list[6];
    list[6] = 0;
    list[5] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE320);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef int (*func_003EE320_cb)(int, int, int);
/**
 * Adds a value to a sorted list as a new item.
 *
 * Takes a free node (growing the pool with func_003EE028 when none is left), stores @p value in it and appends it to
 * the item array. The re-sort counter goes up when the value passes the filter (no filter function, or filter(context,
 * param, value) > 0).
 *
 * @param list  the sorted list
 * @param value  the item value
 * @return the new item, or 0 when the pool cannot grow
 */
int* func_003EE320(int* list, int value) {
    int* n;
    if (!list[14] && func_003EE028(list) <= 0) {
        return 0;
    }
    n = (int*)list[14];
    list[14] = *n;
    *n = value;
    ((int*)list[16])[list[6]] = (int)n;
    list[6]++;
    list[4]--;
    if (!list[9] || ((func_003EE320_cb)list[9])(list[8], list[7], *n) > 0) {
        list[2]++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE3E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef int (*func_003EE3E0_cb)(int, int, int);
/**
 * Changes a list item's value.
 *
 * The re-sort counter goes up when the old or the new value passes the filter (no filter function, or filter(context,
 * param, value) > 0).
 *
 * @param list  the sorted list
 * @param item  the item
 * @param value  its new value
 */
void func_003EE3E0(int* list, int* item, int value) {
    if (!list[9] || ((func_003EE3E0_cb)list[9])(list[8], list[7], *item) > 0 || ((func_003EE3E0_cb)list[9])(list[8], list[7], value) > 0) {
        list[2]++;
    }
    *item = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE468);
#ifdef SKIP_ASM
/**
 * Returns a list item's value, or 0 for a null item.
 *
 * @param list  the sorted list (unused)
 * @param item  the item
 * @return the value
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE468(int list, int* item) {
    return item ? *item : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE480);
#ifdef SKIP_ASM
/**
 * Removes an item from a sorted list.
 *
 * Finds @p item in the item array; when it was one of the passing items the passing count drops and the re-sort counter
 * goes up. The later items move down and the node returns to the free list.
 *
 * @param list  the sorted list
 * @param item  the item
 * @return the item's value, or 0 when it is not in the list
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE480(int* list, int* item) {
    int i;
    int value;
    for (i = 0; i < list[6]; i++) {
        if (((int*)list[16])[i] == (int)item) {
            break;
        }
    }
    if (i == list[6]) {
        return 0;
    }
    if (i < list[5]) {
        list[5]--;
        list[2]++;
    }
    list[6]--;
    for (; i < list[6]; i++) {
        ((int*)list[16])[i] = ((int*)list[16])[i + 1];
    }
    value = *item;
    *item = list[14];
    list[14] = (int)item;
    list[4]++;
    return value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE550);
#ifdef SKIP_ASM
/**
 * Returns the number of items in a sorted list (word [6]).
 *
 * @param list  the sorted list
 * @return the item count
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE550(int* list) {
    return list[6];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE558);
#ifdef SKIP_ASM
/**
 * Returns the number of list items that passed the filter at the last update (word [5]).
 *
 * @param list  the sorted list
 * @return the passing count
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE558(int* list) {
    return list[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE560);
#ifdef SKIP_ASM
/**
 * Returns the value of the item at an index of a sorted list.
 *
 * @param list  the sorted list
 * @param i  the index
 * @return the value, or 0 for a null list or an index out of range
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE560(int* list, int i) {
    if (!list || i < 0 || i >= list[6]) {
        return 0;
    }
    return **(int**)(list[16] + i * 4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE5A0);
#ifdef SKIP_ASM
/**
 * Sets a sorted list's sort function, context and parameter.
 *
 * Sets the re-sort counter to 1 when the list has items.
 *
 * @param list  the sorted list
 * @param sortContext  the first argument passed to the sort function
 * @param sortParam  the second argument
 * @param sortFunc  the compare function (context, param, value, value)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE5A0(int* list, int sortContext, int sortParam, int sortFunc) {
    if (list[6] > 0) {
        list[2] = 1;
    }
    list[11] = sortContext;
    list[10] = sortParam;
    list[12] = sortFunc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE5C0);
#ifdef SKIP_ASM
/**
 * Sets a sorted list's filter function, context and parameter.
 *
 * Sets the re-sort counter to 1 when the list has items. No retail code calls it.
 *
 * @param list  the sorted list
 * @param filterContext  the first argument passed to the filter
 * @param filterParam  the second argument
 * @param filterFunc  the filter (context, param, value); above 0 keeps the item
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE5C0(int* list, int filterContext, int filterParam, int filterFunc) {
    if (list[6] > 0) {
        list[2] = 1;
    }
    list[8] = filterContext;
    list[7] = filterParam;
    list[9] = filterFunc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE5E0);
#ifdef SKIP_ASM
/**
 * Optionally bumps a sorted list's re-sort counter and returns whether it is set.
 *
 * No retail code calls it.
 *
 * @param list  the sorted list
 * @param inc  non-zero to add 1 first
 * @return 1 when the counter is above 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE5E0(int* list, int inc) {
    if (inc) {
        list[2]++;
    }
    return list[2] > 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE600);
#ifdef SKIP_ASM
/**
 * Optionally bumps a sorted list's change counter and returns whether it is set.
 *
 * @param list  the sorted list
 * @param inc  non-zero to add 1 first
 * @return 1 when the counter is above 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE600(int* list, int inc) {
    if (inc) {
        list[3]++;
    }
    return list[3] > 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE620);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// (prototype from their definition at line 90)
int func_003EE1B0_E620(int* p, int** a, int** b) __asm__("func_003EE1B0");

typedef int (*func_003EE620_cb)(int, int, int);
extern void func_003EF210(int*, int, int, int, void*);
/**
 * Brings a sorted list up to date: filters and sorts its items when they changed.
 *
 * With both counters clear it returns 0. With only the change counter set it clears it and returns 1. Otherwise it
 * moves the items that pass the filter (no filter function, or filter(context, param, value) > 0) to the front of the
 * item array (the rest after them), stores their count in [5], sorts them with func_003EF210 and func_003EE1B0 when a
 * sort function is set, clears both counters and returns 2.
 *
 * @param list  the sorted list
 * @return 0 nothing changed, 1 changed without re-sorting, 2 re-filtered and sorted
 */
int func_003EE620(int* list) {
    int* keepOut;
    int* rejectOut;
    int* scan;
    if (list[2] + list[3] == 0) {
        return 0;
    }
    if (list[2] == 0) {
        list[3] = 0;
        return 1;
    }
    keepOut = (int*)list[16];
    rejectOut = keepOut + list[6];
    scan = keepOut;
    if (keepOut != rejectOut) {
        do {
            if (!list[9] || ((func_003EE620_cb)list[9])(list[8], list[7], *(int*)*scan) > 0) {
                *keepOut++ = *scan;
            } else {
                *rejectOut++ = *scan;
            }
            scan++;
        } while (scan != (int*)list[16] + list[6]);
    }
    list[5] = keepOut - (int*)list[16];
    scan = (int*)list[16] + list[6];
    if (scan != rejectOut) {
        do {
            *keepOut++ = *scan++;
        } while (scan != rejectOut);
    }
    list[6] = keepOut - (int*)list[16];
    if (list[12]) {
        func_003EF210(list, list[16], list[5], 4, func_003EE1B0_E620);
    }
    list[2] = 0;
    list[3] = 0;
    return 2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE768);
#ifdef SKIP_ASM
/**
 * Stores a user word in a sorted list (word [13]).
 *
 * @param list  the sorted list
 * @param value  the word
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE768(int* list, int value) {
    list[13] = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE770);
#ifdef SKIP_ASM
/**
 * Returns the user word of a sorted list (word [13]).
 *
 * @param list  the sorted list
 * @return the word
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE770(int* list) {
    return list[13];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE778);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int* func_002591E8(int);
extern void* func_00416210_p(void*, int, int) __asm__("func_00416210");
extern int func_003EE900(int*, int);
extern int func_003EEAB8();
/**
 * Creates a string-keyed hash table.
 *
 * The hash table is a 36-byte block of words: [0] entries, [1] drain cursor, [2] and [3] iteration bucket and node, [4]
 * bucket count, [5] bucket array, [6] chunk list, [7] free-node list, [8] string compare function; a node is 12 bytes
 * (next, value, key).
 *
 * Allocates and zeroes the table and its bucket array ("Lobby" heap) with at least 16 buckets, sets the compare
 * function to func_003EEAB8 (case- and space-insensitive) and preallocates the nodes (func_003EE900).
 *
 * @param n  the number of nodes to preallocate (at least 1)
 * @param bucketCount  the number of buckets (at least 16)
 * @return the new table
 */
int* func_003EE778(int n, int bucketCount) {
    int cnt = n > 0 ? n : 1;
    int nb = bucketCount > 15 ? bucketCount : 16;
    int* table;
    table = func_002591E8(36);
    func_00416210_p(table, 0, 36);
    table[4] = nb;
    table[5] = (int)func_002591E8(nb * 4);
    func_00416210_p((void*)table[5], 0, table[4] * 4);
    table[8] = (int)func_003EEAB8;
    if (cnt > 0) {
        func_003EE900(table, cnt);
    }
    return table;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE820);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_00259210(void*);
/**
 * Destroys a hash table: frees its node chunks, its bucket array and the table itself.
 *
 * @param table  the hash table
 */
void func_003EE820(int* table) {
    if (table[6]) {
        do {
            int* chunk = (int*)table[6];
            table[6] = *chunk;
            func_00259210(chunk);
        } while (table[6]);
    }
    func_00259210((void*)table[5]);
    func_00259210(table);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE878);
#ifdef SKIP_ASM
/**
 * Sets a hash table's key compare function (word [8]).
 *
 * No retail code calls it.
 *
 * @param table  the hash table
 * @param compareFunc  the compare function (key, key); 0 means equal
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE878(int* table, int compareFunc) {
    table[8] = compareFunc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE880);
#ifdef SKIP_ASM
/**
 * Empties a hash table, returning every node to the free list.
 *
 * No retail code calls it.
 *
 * @param table  the hash table
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE880(int* table) {
    int i;
    table[0] = 0;
    for (i = 0; i < table[4]; i++) {
        int* n;
        while ((n = ((int**)table[5])[i]) != 0) {
            ((int**)table[5])[i] = (int*)n[0];
            n[0] = table[7];
            table[7] = (int)n;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE900);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int* func_002591E8(int);
/**
 * Adds a chunk of free nodes to a hash table.
 *
 * Allocates (count + 1) 12-byte entries ("Lobby" heap): the first links the chunk into the chunk list, the others go to
 * the free-node list.
 *
 * @param table  the hash table
 * @param n  the number of nodes (at least 1)
 * @return the number of nodes added, or -1 when out of memory
 */
int func_003EE900(int* table, int n) {
    int hasCount = n > 0;
    int cnt = 1;
    int size;
    int* mem;
    int* item;
    int* end;
    if (hasCount) {
        cnt = n;
    }
    cnt++;
    size = cnt * 12;
    mem = func_002591E8(size);
    if (!mem) {
        return -1;
    }
    mem[0] = table[6];
    table[6] = (int)mem;
    item = mem + 3;
    end = (int*)((char*)mem + size);
    while (item != end) {
        item[0] = table[7];
        table[7] = (int)item;
        item += 3;
    }
    return cnt - 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE9A0);
#ifdef SKIP_ASM
/**
 * Removes one entry from a hash table and returns its value.
 *
 * Moves the drain cursor to the next non-empty bucket (wrapping) and frees that bucket's first node.
 *
 * @param table  the hash table
 * @return the removed value, or 0 when the table is empty
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE9A0(int* table) {
    int* n;
    int** slot;
    if (!table[0]) {
        return 0;
    }
    if (!((int**)table[5])[table[1]]) {
        do {
            table[1] = (table[1] + 1) % table[4];
        } while (!((int**)table[5])[table[1]]);
    }
    slot = (int**)(table[1] * 4 + table[5]);
    n = *slot;
    *slot = (int*)n[0];
    n[0] = table[7];
    table[7] = (int)n;
    table[0]--;
    return n[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEA48);
#ifdef SKIP_ASM
/**
 * Hashes a string key to a bucket of a hash table.
 *
 * Skips characters at or below a space (and bytes above 0x7F, which are negative), folds case with & 0x5F and computes
 * hash * 131 + character, then takes it modulo the bucket count.
 *
 * @param table  the hash table
 * @param key  the key string
 * @return the bucket index
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EEA48(int* table, int key) {
    char* cursor = (char*)key;
    int hash = 0;
    while (*cursor) {
        if (*cursor > 32) {
            hash = hash * 131 + (*cursor & 0x5F);
        }
        cursor++;
    }
    hash &= 0x7FFFFFFF;
    return hash % table[4];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEAB8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_00496A58[];
/**
 * Compares two strings without regard to case, ignoring spaces and control characters.
 *
 * Both strings go through D_00496A58, which upper-cases letters and maps characters 1 to 0x20 to 1; those are skipped.
 * Used as the hash tables' default key compare.
 *
 * @param leftText  the first string
 * @param rightText  the second string
 * @return the difference of the first differing characters, or 0 when equal
 */
int func_003EEAB8(unsigned char* leftText, unsigned char* rightText) {
    int leftChar, rightChar, charDiff;
    do {
        do {
            leftChar = D_00496A58[*leftText++];
        } while (leftChar == 1);
        do {
            rightChar = D_00496A58[*rightText++];
        } while (rightChar == 1);
        charDiff = leftChar - rightChar;
        if (charDiff != 0) {
            return charDiff;
        }
    } while (leftChar != 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEB30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EEA48(int*, int);
extern int func_003EE900(int*, int);
/**
 * Adds a string-keyed entry to a hash table.
 *
 * Takes a free node (growing the pool by (entries * 3 + 10) / 2 when none is left), stores the key pointer and value
 * and puts it at the head of the key's bucket (func_003EEA48). Duplicate keys are not checked.
 *
 * @param table  the hash table
 * @param key  the key string (kept, not copied)
 * @param value  the value
 * @return 1, or -1 when the pool cannot grow
 */
int func_003EEB30(int* table, int key, int value) {
    int bucket = func_003EEA48(table, key);
    int* n;
    if (!table[7] && func_003EE900(table, (table[0] * 3 + 10) / 2) <= 0) {
        return -1;
    }
    n = (int*)table[7];
    table[7] = *n;
    n[2] = key;
    n[1] = value;
    n[0] = ((int*)table[5])[bucket];
    ((int*)table[5])[bucket] = (int)n;
    table[0]++;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEBF8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EEA48(int*, int);
/**
 * Replaces the value of a string-keyed hash table entry.
 *
 * Looks the key up with the table's compare function. No retail code calls it.
 *
 * @param table  the hash table
 * @param key  the key string
 * @param val  the new value
 * @return the old value, or 0 when the key is not found
 */
int func_003EEBF8(int* table, int key, int val) {
    int* n = ((int**)table[5])[func_003EEA48(table, key)];
    while (n) {
        if (!((int (*)(int, int))table[8])(n[2], key)) {
            int old = n[1];
            n[1] = val;
            return old;
        }
        n = (int*)n[0];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEC88);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EEA48(int*, int);
/**
 * Looks up a string key in a hash table.
 *
 * @param table  the hash table
 * @param key  the key string
 * @return the value, or 0 when the key is not found
 */
int func_003EEC88(int* table, int key) {
    int* n = ((int**)table[5])[func_003EEA48(table, key)];
    while (n) {
        if (!((int (*)(int, int))table[8])(n[2], key)) {
            return n[1];
        }
        n = (int*)n[0];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EED08);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EEA48(int*, int);
/**
 * Removes a string-keyed entry from a hash table.
 *
 * Unlinks the first matching node from its bucket and returns it to the free list.
 *
 * @param table  the hash table
 * @param key  the key string
 * @return the removed value, or 0 when the key is not found
 */
int func_003EED08(int* table, int key) {
    int* n;
    int* prev = &((int*)table[5])[func_003EEA48(table, key)];
    for (;;) {
        n = (int*)*prev;
        if (!n) {
            return 0;
        }
        if (!((int (*)(int, int))table[8])(n[2], key)) {
            break;
        }
        prev = (int*)*prev;
    }
    *prev = n[0];
    n[0] = table[7];
    table[7] = (int)n;
    table[0]--;
    return n[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEDA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EE900(int*, int);
/**
 * Adds an integer-keyed entry to a hash table.
 *
 * The bucket is |key| modulo the bucket count; otherwise like func_003EEB30. Duplicate keys are not checked.
 *
 * @param table  the hash table
 * @param key  the integer key
 * @param value  the value
 * @return 1, or -1 when the pool cannot grow
 */
int func_003EEDA0(int* table, int key, int value) {
    int bucket;
    int* n;
    bucket = (key < 0 ? -key : key) % table[4];
    if (!table[7] && func_003EE900(table, (table[0] * 3 + 10) / 2) <= 0) {
        return -1;
    }
    n = (int*)table[7];
    table[7] = *n;
    n[2] = key;
    n[1] = value;
    n[0] = ((int*)table[5])[bucket];
    ((int*)table[5])[bucket] = (int)n;
    table[0]++;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEE80);
#ifdef SKIP_ASM
/**
 * Replaces the value of an integer-keyed hash table entry.
 *
 * No retail code calls it.
 *
 * @param table  the hash table
 * @param key  the integer key
 * @param val  the new value
 * @return the old value, or 0 when the key is not found
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EEE80(int* table, int key, int val) {
    int k = key >= 0 ? key : -key;
    int* n = ((int**)table[5])[k % table[4]];
    while (n) {
        if (n[2] == key) {
            int old = n[1];
            n[1] = val;
            return old;
        }
        n = (int*)n[0];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEEE8);
#ifdef SKIP_ASM
/**
 * Looks up an integer key in a hash table.
 *
 * @param table  the hash table
 * @param key  the integer key
 * @return the value, or 0 when the key is not found
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EEEE8(int* table, int key) {
    int k = key >= 0 ? key : -key;
    int* n = ((int**)table[5])[k % table[4]];
    while (n) {
        if (n[2] == key) {
            return n[1];
        }
        n = (int*)n[0];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEF40);
#ifdef SKIP_ASM
/**
 * Removes an integer-keyed entry from a hash table.
 *
 * @param table  the hash table
 * @param key  the integer key
 * @return the removed value, or 0 when the key is not found
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EEF40(int* table, int key) {
    int k = key >= 0 ? key : -key;
    int* prev = &((int*)table[5])[k % table[4]];
    int* n;
    for (;;) {
        n = (int*)*prev;
        if (!n) {
            return 0;
        }
        if (n[2] == key) {
            break;
        }
        prev = n;
    }
    *prev = n[0];
    n[0] = table[7];
    table[7] = (int)n;
    table[0]--;
    return n[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEFB8);
#ifdef SKIP_ASM
/**
 * Restarts iteration over a hash table (clears words [2] and [3]).
 *
 * No retail code calls it.
 *
 * @param table  the hash table
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EEFB8(int* table) {
    table[2] = 0;
    table[3] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEFC8);
#ifdef SKIP_ASM
/**
 * Returns the next entry of a hash table iteration.
 *
 * Walks the buckets from the iteration bucket [2] and node [3]; func_003EEFB8 restarts it. An entry whose value is 0
 * cannot be told from the end. No retail code calls it.
 *
 * @param table  the hash table
 * @param key  receives the entry key; may be null
 * @return the entry value, or 0 at the end or for an empty table
 */
int func_003EEFC8(int* table, int* key) {
    int* n;
    if (table[0] == 0) {
        return 0;
    }
    if (table[3] == 0) {
        for (; table[2] < table[4]; table[2]++) {
            if (((int*)table[5])[table[2]]) {
                break;
            }
        }
        if (table[2] >= table[4]) {
            return 0;
        }
        table[3] = ((int*)table[5])[table[2]++];
    }
    n = (int*)table[3];
    table[3] = n[0];
    if (key) {
        *key = n[2];
    }
    return n[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF088);
#ifdef SKIP_ASM
/**
 * Returns 0.
 *
 * cDirtysock_tag_TagFieldGetTime and cDirtysock_tag_TagFieldSetEpoch call it.
 *
 * @return 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EF088(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF090);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct {
    unsigned char stat;
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char pad;
    unsigned char day;
    unsigned char month;
    unsigned char year;
} func_003EF090_clock;
extern int D_004A4A94;
extern void func_0040BFD8(void*);
extern void func_0040BFE0(void*);
#define BCD(x) (((x) >> 4) * 10 + ((x) & 0xF))
/**
 * Returns, computed once, the difference in seconds between two clock conversions of the same date.
 *
 * On the first call (D_004A4A94 is -1) it fills an 8-byte clock record (BCD seconds, minutes, hours, day 2, month 1,
 * year 1), converts it with func_0040BFD8 and with func_0040BFE0, turns each result into seconds (day * 86400 + hours *
 * 3600 + minutes * 60 + seconds) and caches the second minus the first in D_004A4A94 (in effect a time-zone offset). No
 * retail code calls it.
 *
 * @return the cached difference in seconds
 */
int func_003EF090(void) {
    func_003EF090_clock c;
    int firstSecs;
    int secondSecs;
    if (D_004A4A94 == -1) {
        *(long*)&c = 0;
        c.day = 2;
        c.month = 1;
        c.year = 1;
        func_0040BFD8(&c);
        firstSecs = BCD(c.day) * 86400 + BCD(c.hour) * 3600 + BCD(c.minute) * 60 + BCD(c.second);
        *(long*)&c = 0;
        c.day = 2;
        c.month = 1;
        c.year = 1;
        func_0040BFE0(&c);
        secondSecs = BCD(c.day) * 86400 + BCD(c.hour) * 3600 + BCD(c.minute) * 60 + BCD(c.second);
        D_004A4A94 = secondSecs - firstSecs;
    }
    return D_004A4A94;
}
#endif

INCLUDE_ASM("seg/seg_2EEF50", func_003EF210);

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF600);
#ifdef SKIP_ASM
/**
 * Returns the length a UTF-8 lead byte announces, plus one for multi-byte leads.
 *
 * Counts the leading 1 bits and adds 1 (at most 7): 1 for ASCII, 2 for a continuation byte, 3 for a two-byte lead
 * (110xxxxx), 4 for a three-byte lead.
 *
 * Likely bug: the callers use it as the sequence length, so they skip one byte too many per multi-byte character.
 *
 * @param x  the byte
 * @return the leading-one count + 1
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EF600(unsigned char x) {
    int n;
    for (n = 1; n < 7 && (x & 0x80); n++) {
        x <<= 1;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF640);
#ifdef SKIP_ASM
/**
 * Decodes one UTF-8 character (up to three bytes) into UCS-2.
 *
 * One-, two- and three-byte sequences are decoded into @p out; for a longer sequence nothing is written and
 * func_003EF600's count is returned.
 *
 * @param out  receives the UCS-2 character
 * @param src  the UTF-8 text
 * @return the bytes consumed
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EF640(unsigned short* out, char* src) {
    unsigned char* in = (unsigned char*)src;
    int n;
    if (!(in[0] & 0x80)) {
        n = 1;
        *out = in[0];
    } else if ((in[0] & 0xE0) == 0xC0) {
        n = 2;
        *out = ((in[0] & ~0xE0) << 6) | (in[1] & ~0xC0);
    } else if ((in[0] & 0xF0) == 0xE0) {
        n = 3;
        *out = ((in[0] & ~0xF0) << 12) | ((in[1] & ~0xC0) << 6) | (in[2] & ~0xC0);
    } else {
        n = func_003EF600(in[0]);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF6F8);
#ifdef SKIP_ASM
/**
 * Encodes a UCS-2 character as one to three UTF-8 bytes.
 *
 * @param dst  the output
 * @param ch  the character (its low 16 bits)
 * @return the number of bytes written
 */
int func_003EF6F8(char* dst, int ch) {
    unsigned short codePoint = ch;
    int n;
    if (codePoint < 0x80) {
        dst[0] = codePoint;
        n = 1;
    } else if (codePoint < 0x800) {
        n = 2;
        dst[0] = (codePoint >> 6) | 0xC0;
        dst[1] = (codePoint & 0x3F) | 0x80;
    } else {
        n = 3;
        dst[0] = (codePoint >> 12) | 0xE0;
        dst[1] = ((codePoint >> 6) & 0x3F) | 0x80;
        dst[2] = (codePoint & 0x3F) | 0x80;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF770);
#ifdef SKIP_ASM
/**
 * Translates a UCS-2 character to an 8-bit character through a table of ranges.
 *
 * @p range is a list of 12-byte entries (low, high, byte map) ended by a high of 0. In the first range that holds @p
 * codePoint the byte is map[codePoint - low]; a mapped 0xFF becomes @p ch when that is non-zero. When no range holds
 * it, @p ch is written if non-zero.
 *
 * @param dst  the output
 * @param range  the range table
 * @param codePoint  the character
 * @param ch  the replacement byte, or 0 for none
 * @return the bytes written (0 or 1)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EF770(unsigned char* dst, unsigned int* range, unsigned short codePoint, signed char ch) {
    int found = 0;
    unsigned char* dstStart = dst;
    for (; range[1]; range += 3) {
        if (codePoint < range[0]) {
            continue;
        }
        if (range[1] < codePoint) {
            continue;
        }
        {
            unsigned char mapped;
            codePoint = codePoint - *(unsigned short*)range;
            mapped = ((unsigned char*)range[2])[codePoint];
            if (mapped != 0xFF) {
                *dst = mapped;
            } else if (ch) {
                *dst = ch;
            } else {
                *dst = mapped;
            }
            dst++;
            found = 1;
            break;
        }
    }
    if (!found && ch) {
        *dst++ = ch;
    }
    return dst - dstStart;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF808);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EF828(char*, int, char*, signed char);
/**
 * Strips the non-ASCII characters from a UTF-8 string (func_003EF828 with no replacement byte).
 *
 * No retail code calls it.
 *
 * @param dst  the output
 * @param maxLen  the output size
 * @param src  the UTF-8 string
 */
void func_003EF808(int dst, int maxLen, int src) {
    func_003EF828((char*)dst, maxLen, (char*)src, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF828);
#ifdef SKIP_ASM
/**
 * Replaces each non-ASCII character of a UTF-8 string with a byte, or drops it.
 *
 * It is written for @p dst == @p src: the leading ASCII run is not copied, and a pure-ASCII string writes nothing and
 * returns 0. From the first multi-byte character on, ASCII bytes are copied and each multi-byte sequence becomes @p ch
 * (or nothing) while fewer than @p max bytes are written; then a terminator follows (possibly at index @p max).
 *
 * Quirk: func_003EF600 returns one more than the sequence length (3 for a two-byte sequence), so every multi-byte
 * character skips one byte too many, and a multi-byte character at the end skips the terminator.
 *
 * @param dst  the output (normally the input itself)
 * @param max  the output limit
 * @param src  the UTF-8 string
 * @param ch  the replacement byte, or 0 to drop
 * @return the output length including the terminator, or 0 when the string is pure ASCII
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EF828(char* dst, int max, char* src, signed char ch) {
    int i = 0;
    int j = 0;
    while (!(src[i] & 0x80) && src[i]) {
        i++;
    }
    if (src[i]) {
        j = i;
        while (src[i] && j < max) {
            if (src[i] & 0x80) {
                i += func_003EF600(src[i]);
                if (ch) {
                    dst[j++] = ch;
                }
            } else {
                dst[j++] = src[i++];
            }
        }
        dst[j++] = 0;
    }
    return j;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF940);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EF600(unsigned char);
/**
 * Counts the characters of a UTF-8 string.
 *
 * Quirk: func_003EF600 returns one more than the sequence length (3 for a two-byte sequence), so every multi-byte
 * character skips one byte too many, and a multi-byte character at the end skips the terminator. No retail code calls
 * it.
 *
 * @param utf8  the UTF-8 string
 * @return the character count
 */
int func_003EF940(char* utf8) {
    int i = 0;
    int count = 0;
    if (utf8[0]) {
        do {
            if (utf8[i] & 0x80) {
                i += func_003EF600(utf8[i]);
            } else {
                i++;
            }
            count++;
        } while (utf8[i]);
    }
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2EEF50", func_003EF9B8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EF6F8(char*, int);
/**
 * Encodes a UCS-2 string as UTF-8.
 *
 * Encodes characters while the output is below @p size - 2 and then terminates it; a three-byte character started at
 * size - 3 puts the terminator at index @p size, one byte past the buffer. No retail code calls it.
 *
 * @param dst  the output
 * @param size  the output size
 * @param src  the UCS-2 string
 * @return the bytes written including the terminator
 */
int func_003EF9B8(char* dst, int size, unsigned short* src) {
    int i = 0;
    int lim = size - 2;
    if (*src && lim > 0) {
        do {
            i += func_003EF6F8(dst + i, *src++);
        } while (*src && i < lim);
    }
    dst[i] = 0;
    return i + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EFA48);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EF640(unsigned short*, char*);
/**
 * Decodes a UTF-8 string into UCS-2.
 *
 * Decodes with func_003EF640 while fewer than @p size - 1 characters are written; sequences longer than three bytes are
 * skipped. func_001A7390 and func_001B4CD8 call it.
 *
 * @param dst  the UCS-2 output
 * @param size  the output size in characters
 * @param src  the UTF-8 string
 * @return the characters written including the terminator
 */
int func_003EFA48(unsigned short* dst, int size, char* src) {
    int di = 0;
    int si = 0;
    int lim = size - 1;
    if (*src && lim > 0) {
        do {
            int consumed = func_003EF640(dst + di, src + si);
            si += consumed;
            if (consumed <= 3) {
                di++;
            }
        } while (src[si] && di < lim);
    }
    dst[di] = 0;
    return di + 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2EEF50", func_003EFAF8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EF6F8(char*, int);
/**
 * Converts an 8-bit string to UTF-8 through a 256-entry UCS-2 table.
 *
 * Each byte is looked up in @p tbl and encoded with func_003EF6F8 while the output is below @p size - 2. No retail code
 * calls it.
 *
 * @param dst  the output
 * @param size  the output size
 * @param src  the 8-bit string
 * @param tbl  the byte-to-UCS-2 table
 * @return the bytes written including the terminator
 */
int func_003EFAF8(char* dst, int size, char* src, unsigned short* tbl) {
    int i = 0;
    int lim = size - 2;
    if (*src && lim > 0) {
        do {
            unsigned short* tblEntry = &tbl[(unsigned char)*src++];
            i += func_003EF6F8(dst + i, *tblEntry);
        } while (*src && i < lim);
    }
    dst[i] = 0;
    return i + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EFBA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EF640(unsigned short*, char*);
extern int func_003EF770(unsigned char*, unsigned int*, unsigned short, signed char);
/**
 * Converts a UTF-8 string to 8-bit characters through range tables.
 *
 * Decodes each character (func_003EF640) and translates it with func_003EF770 while fewer than @p size - 1 bytes are
 * written. No retail code calls it.
 *
 * @param dst  the output
 * @param size  the output size
 * @param src  the UTF-8 string
 * @param ch  the replacement byte for unmapped characters, or 0
 * @param ranges  the range table
 * @return the bytes written including the terminator
 */
int func_003EFBA0(unsigned char* dst, int size, char* src, signed char ch, unsigned int* ranges) {
    int di = 0;
    int si = 0;
    int lim = size - 1;
    if (*src && lim > 0) {
        unsigned short codePoint;
        do {
            si += func_003EF640(&codePoint, src + si);
            di += func_003EF770(dst + di, ranges, codePoint, ch);
        } while (src[si] && di < lim);
    }
    dst[di] = 0;
    return di + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EFC68);
#ifdef SKIP_ASM
/**
 * Parses a hexadecimal number (digits 0-9, a-f, A-F) from text.
 *
 * Stops at the first other character; no prefix or sign is accepted.
 *
 * @param cursor  the text
 * @param out  receives the value
 * @return the position after the digits
 */
// From peak3project's fork (github.com/peak3project/ssx3).
char* func_003EFC68(char* cursor, int* out) {
    int value = 0;
    int digit;
    for (;; cursor++) {
        if ((unsigned char)(*(unsigned char*)cursor - '0') < 10 || (unsigned char)(*(unsigned char*)cursor - 'a') < 6 || (unsigned char)(*(unsigned char*)cursor - 'A') < 6) {
            digit = *cursor;
            if (digit >= ':') {
                digit = (char)(digit + 9);
            }
            value = (value << 4) | (digit & 0xF);
            continue;
        }
        break;
    }
    *out = value;
    return cursor;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EFCD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char* func_003EFCD8_p(char* s, int* lo, int* hi) __asm__("func_003EFCD8");
/**
 * Parses one "lo:hi" hexadecimal range from a list, skipping a following comma.
 *
 * @param rangeText  the text
 * @param lo  receives the start
 * @param hi  receives the end
 * @return the position after the range, or 0 when the ':' is missing
 */
char* func_003EFCD8_p(char* rangeText, int* lo, int* hi) {
    rangeText = func_003EFC68(rangeText, lo);
    if (*rangeText++ != ':') return 0;
    rangeText = func_003EFC68(rangeText, hi);
    if (*rangeText == ',') rangeText++;
    return rangeText;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2EEF50", func_003EFD30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int func_003EFCD8(unsigned int, unsigned int*, unsigned int*);
extern void func_003F30F0(void*);
extern void func_003F3128(void*, unsigned int, int);
extern void func_003F31F8(void*, int, int);
extern unsigned int D_004A4A98;
extern unsigned int D_004A4A9C;
/**
 * Computes an MD5 digest over a range list and the memory ranges it names (a memory check).
 *
 * Starts an MD5 context (func_003F30F0, the 0x67452301... constants), adds the range-list text (func_003F3128), then
 * parses its "lo:hi," entries (func_003EFCD8) until one is empty and adds every range with lo < hi inside [D_004A4A98,
 * D_004A4A9C) (0x00100000 to 0x02000000); then adds the text again and finishes into @p digestOut (func_003F31F8).
 * LobbyMan_onauthentication calls it.
 *
 * @param rangeSpec  the range-list text
 * @param specLen  its length
 * @param digestOut  the digest output
 * @param digestLen  the digest output size
 */
void func_003EFD30(unsigned int rangeSpec, int specLen, int digestOut, int digestLen) {
    char buf[96];
    unsigned int lo, hi;
    unsigned int orig;
    func_003F30F0(buf);
    orig = rangeSpec;
    func_003F3128(buf, orig, specLen);
    if (orig) {
        do {
            rangeSpec = func_003EFCD8(rangeSpec, &lo, &hi);
            if (!lo || !hi) {
                break;
            }
            if (lo >= D_004A4A98 && hi < D_004A4A9C && lo < hi) {
                func_003F3128(buf, lo, hi - lo);
            }
        } while (rangeSpec);
    }
    func_003F3128(buf, orig, specLen);
    func_003F31F8(buf, digestOut, digestLen);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EFE20);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sE { int key; short code; short pad; char* name; };
extern struct sE D_004512D8[];
extern int cDirtysock_tag_TagFieldDupl(char* dst, int len, const char* src);
/**
 * Finds the first table entry for a key whose code is in a list, and copies its name.
 *
 * Scans D_004512D8 (12-byte entries: key, code, name; ended by key 0) for entries with @p key; for the first whose code
 * is among the @p n codes of @p list, copies its name into @p x with cDirtysock_tag_TagFieldDupl. func_003F02D0 calls
 * it; @p self is not used.
 *
 * @param key  the key to match
 * @param list  the accepted codes
 * @param n  how many codes
 * @param x  the name output
 * @param y  the output size
 * @return the matched code, or 0
 */
short func_003EFE20(void* self, int key, short* list, int n, char* x, int y) {
    int i, j;
    if (D_004512D8[0].key) {
        for (i = 0; D_004512D8[i].key; i++) {
            if (D_004512D8[i].key == key) {
                for (j = 0; j < n; j++) {
                    if (list[j] == D_004512D8[i].code) {
                        cDirtysock_tag_TagFieldDupl(x, y, D_004512D8[i].name);
                        return D_004512D8[i].code;
                    }
                }
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EFED0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_004186C8(char* dst, const char* fmt, char* ap);
/**
 * Formats a debug line for a buddy-API object and passes it to its log callback.
 *
 * Formats into a 4 KB stack buffer (func_004186C8, vsprintf) and, when the callback at +0x508 is set, calls it with the
 * user word at +0x504 and the text.
 *
 * @param buddyApi  the buddy-API object
 * @param fmt  the printf format, followed by its arguments
 */
void func_003EFED0(int* buddyApi, const char* fmt, ...) {
    char buf[0x1000];
    char* ap;
    ap = (char*)__builtin_next_arg()
         - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_004186C8(buf, fmt, ap);
    if (buddyApi[322]) {
        ((void (*)(int, char*))buddyApi[322])(buddyApi[321], buf);
    }
}
#endif
