#include "common.h"

//100%
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void get_uint(int, void*);
extern "C" int readVolume_C428(int, void*, int) __asm__("readVolume");
extern "C" int readAction_C428(int, void*, int) __asm__("readAction");

extern "C" void readTrigger(int arg0, char **arg1, int arg2) {
    get_uint(arg0, *arg1);
    get_uint(arg0, *arg1 + 4);
    if (readVolume_C428(arg0, *arg1 + 8, arg2) == 0) {
        if (readAction_C428(arg0, *arg1 + 0xC, arg2) == 0) {
            readAction_C428(arg0, *arg1 + 0x10, arg2);
        }
    }
}

//100%
extern "C" int get_camvolume();

extern "C" int readVolume(void) {
    return (get_camvolume() != 0) ? 0 : 4;
}

//100%
extern "C" int get_camboundobj();

extern "C" int readBoundObj(void) {
    return (get_camboundobj() != 0) ? 0 : 5;
}

//100%
extern "C" int get_camaction(int reader, void* out);
extern "C" void get_camspline(int reader, void* out);
// PORT: readBoundObj is defined above as (void); this caller passes the reader, slot and flag.
extern "C" int readBoundObj_3(int reader, void* out, int arg2) __asm__("readBoundObj");

extern "C" int readAction(int arg0, int** arg1, int arg2) {
    if (get_camaction(arg0, arg1) == 0) {
        return 6;
    }
    int* act = *arg1;
    if (*act == 1) {
        return readBoundObj_3(arg0, act + 2, arg2);
    }
    if (*act == 2) {
        get_camspline(arg0, act + 2);
        return 0;
    }
    return 0;
}
