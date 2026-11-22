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

#endif
