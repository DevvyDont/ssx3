#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EDF50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_004963C0[];
int func_003EDF50(char *a, char *b) {
    unsigned qa, qb;
    unsigned ca, cb;
    if (a == 0 || *a == 0) return -1;
    if (*a == '"' || *a == '\'') {
        qa = *(unsigned char *)a;
        a++;
    } else {
        qa = 0;
    }
    if (*b == '"' || *b == '\'') {
        qb = *(unsigned char *)b;
        b++;
    } else {
        qb = 0;
    }
    do {
        ca = D_004963C0[(int)*a];
        a++;
        if (ca == qa || (ca < 0x20 && qa == 0)) ca = 0;
        cb = D_004963C0[(int)*b];
        b++;
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
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE468(int list, int* item) {
    return item ? *item : 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE480);
#ifdef SKIP_ASM
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
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE550(int* list) {
    return list[6];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE558);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003EE558(int* list) {
    return list[5];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE560);
#ifdef SKIP_ASM
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
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE768(int* list, int value) {
    list[13] = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE770);
#ifdef SKIP_ASM
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
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EE878(int* table, int compareFunc) {
    table[8] = compareFunc;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EE880);
#ifdef SKIP_ASM
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
int func_003EEAB8(unsigned char* a, unsigned char* b) {
    int c1, c2, d;
    do {
        do {
            c1 = D_00496A58[*a++];
        } while (c1 == 1);
        do {
            c2 = D_00496A58[*b++];
        } while (c2 == 1);
        d = c1 - c2;
        if (d != 0) {
            return d;
        }
    } while (c1 != 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEB30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003EEA48(int*, int);
extern int func_003EE900(int*, int);
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
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003EEFB8(int* table) {
    table[2] = 0;
    table[3] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EEFC8);
#ifdef SKIP_ASM
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
int func_003EF090(void) {
    func_003EF090_clock c;
    int a;
    int b;
    if (D_004A4A94 == -1) {
        *(long*)&c = 0;
        c.day = 2;
        c.month = 1;
        c.year = 1;
        func_0040BFD8(&c);
        a = BCD(c.day) * 86400 + BCD(c.hour) * 3600 + BCD(c.minute) * 60 + BCD(c.second);
        *(long*)&c = 0;
        c.day = 2;
        c.month = 1;
        c.year = 1;
        func_0040BFE0(&c);
        b = BCD(c.day) * 86400 + BCD(c.hour) * 3600 + BCD(c.minute) * 60 + BCD(c.second);
        D_004A4A94 = b - a;
    }
    return D_004A4A94;
}
#endif

INCLUDE_ASM("seg/seg_2EEF50", func_003EF210);

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF600);
#ifdef SKIP_ASM
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
void func_003EF808(int dst, int maxLen, int src) {
    func_003EF828((char*)dst, maxLen, (char*)src, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EF828);
#ifdef SKIP_ASM
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
char* func_003EFCD8_p(char* s, int* lo, int* hi) {
    s = func_003EFC68(s, lo);
    if (*s++ != ':') return 0;
    s = func_003EFC68(s, hi);
    if (*s == ',') s++;
    return s;
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
void func_003EFD30(unsigned int a, int b, int c, int d) {
    char buf[96];
    unsigned int lo, hi;
    unsigned int orig;
    func_003F30F0(buf);
    orig = a;
    func_003F3128(buf, orig, b);
    if (orig) {
        do {
            a = func_003EFCD8(a, &lo, &hi);
            if (!lo || !hi) {
                break;
            }
            if (lo >= D_004A4A98 && hi < D_004A4A9C && lo < hi) {
                func_003F3128(buf, lo, hi - lo);
            }
        } while (a);
    }
    func_003F3128(buf, orig, b);
    func_003F31F8(buf, c, d);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2EEF50", func_003EFE20);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sE { int key; short code; short pad; char* name; };
extern struct sE D_004512D8[];
extern int cDirtysock_tag_TagFieldDupl(char* dst, int len, const char* src);
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
