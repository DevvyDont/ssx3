#include "common.h"

INCLUDE_ASM("seg/seg_6D428", readTrigger);

//100%
INCLUDE_ASM("seg/seg_6D428", readVolume);
#ifdef SKIP_ASM
extern "C" int get_camvolume();

extern "C" int readVolume(void) {
    return (get_camvolume() != 0) ? 0 : 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_6D428", readBoundObj);
#ifdef SKIP_ASM
extern "C" int get_camboundobj();

extern "C" int readBoundObj(void) {
    return (get_camboundobj() != 0) ? 0 : 5;
}
#endif

INCLUDE_ASM("seg/seg_6D428", readAction);
