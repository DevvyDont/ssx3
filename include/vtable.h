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

#endif
