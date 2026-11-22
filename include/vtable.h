#ifndef VTABLE_H
#define VTABLE_H

// A g++ 2.95 vtable slot: the this-pointer adjustment, the slot index and the function. F is the slot's function
// type (sVEntry<void> is a raw slot). Matching code makes virtual calls through these by hand:
//     sVEntry<void(void*, int)>* vt = obj->vtbl;  vt[k].fn((char*)obj + vt[k].delta, arg);
// C units can't use the template; they keep a struct with the same three fields.
template <class F> struct sVEntry {
    short delta;
    short index;
    F* fn;
};

// The object a slot's function runs on: obj moved by the slot's this-adjustment (g++ 2.95 stores in each slot the
// offset from the object the vtable belongs to to the base class that defined the function). With it a virtual call
// reads
//     vt[k].fn(vthis(obj, vt[k]), arg);
// V is any slot type with a `delta` member (sVEntry<F>, or a C-style slot struct).
template <class V> inline void* vthis(void* obj, const V& slot)
{
    return (char*)obj + slot.delta;
}

// A whole vtable held as a value: N raw slots, 8-byte aligned. [L] gcc-2.95.3 cp/search.c expand_upcast_fixups: a
// constructor that is not in charge of its virtual bases copies the class's vtable into a local (aligned for doubles)
// and patches the virtual-base deltas there before storing its address (cGroundMotion_cGroundMotion,
// cHandplantMotion_cHandplantMotion, cWipeoutControl_cWipeoutControl, cWipeoutMotion_cWipeoutMotion,
// cPlayer_cPlayer). Copying the .rodata image (`sVTable<9> v = D_xxxx;`) reproduces it. [Staged by views2 group A.]
template <int N> struct sVTable {
    sVEntry<void> entries[N];
} __attribute__((aligned(8)));

// CHECK_SIZE cannot paste a template-id, so the checks are spelled out (as online/lobbyman.h does for _List_node).
#ifndef DECOMP_NO_LAYOUT_CHECKS
typedef char check_sVTable9_size[(sizeof(sVTable<9>) == 0x48) ? 1 : -1];
typedef char check_sVTable22_size[(sizeof(sVTable<22>) == 0xB0) ? 1 : -1];
typedef char check_sVTable9_align[(__alignof__(sVTable<9>) == 8) ? 1 : -1];
#endif

#endif
