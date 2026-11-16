#include "common.h"

//100%
INCLUDE_ASM("seg/seg_6D428", readTrigger);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void get_uint(int, void*);
extern "C" int readVolume_C428(int, void*, int) __asm__("readVolume");
extern "C" int readAction_C428(int, void*, int) __asm__("readAction");

/**
 * Reads one camera trigger from a trigger-list buffer.
 *
 * Reads two uints into the trigger's first two words, then its volume (offset 8) with readVolume() and, while
 * each step returns 0, two actions (offsets 0xC and 0x10) with readAction(). Declared void, but the first
 * nonzero step result (4 or 5 or 6) or the last readAction() result is left as the return value, and the caller
 * cCameraTriggerList_loadFromBuffer() treats it as an error code.
 *
 * @param cursor  pointer to the buffer's read cursor
 * @param trigger  pointer to the trigger being filled
 * @param cookie  the trigger list's format version (7), passed to each step
 */
extern "C" void readTrigger(int cursor, char **trigger, int cookie) {
    get_uint(cursor, *trigger);
    get_uint(cursor, *trigger + 4);
    if (readVolume_C428(cursor, *trigger + 8, cookie) == 0) {
        if (readAction_C428(cursor, *trigger + 0xC, cookie) == 0) {
            readAction_C428(cursor, *trigger + 0x10, cookie);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_6D428", readVolume);
#ifdef SKIP_ASM
extern "C" int get_camvolume();

/**
 * Reads a camera trigger volume as one step of readTrigger().
 *
 * Declared without parameters: the caller's arguments reach get_camvolume() unchanged in their registers.
 *
 * @return 0 on success, or error code 4 when get_camvolume() fails
 */
extern "C" int readVolume(void) {
    return (get_camvolume() != 0) ? 0 : 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_6D428", readBoundObj);
#ifdef SKIP_ASM
extern "C" int get_camboundobj();

/**
 * Reads a camera bound object as one step of readAction().
 *
 * Declared without parameters: the caller's arguments reach get_camboundobj() unchanged in their registers.
 *
 * @return 0 on success, or error code 5 when get_camboundobj() fails
 */
extern "C" int readBoundObj(void) {
    return (get_camboundobj() != 0) ? 0 : 5;
}
#endif

//100%
INCLUDE_ASM("seg/seg_6D428", readAction);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int get_camaction(int, char**, int);
extern "C" void get_camspline(int, char*);
extern "C" int readBoundObj_C510(int, char*, int) __asm__("readBoundObj");

/**
 * Reads one camera trigger action, including its bound object or spline.
 *
 * Reads the action with get_camaction(). A bounded action (type 1) then reads its bound object with
 * readBoundObj(); a spline action (type 2) reads its spline with get_camspline().
 *
 * @param cursor  pointer to the buffer's read cursor
 * @param actionOut  receives the new action
 * @param cookie  the trigger list's format version, passed through
 * @return 0 on success, readBoundObj()'s error code 5, or 6 when get_camaction() returns 0 (it always counts
 *         the 4-byte type word, so 6 looks unreachable)
 */
extern "C" int readAction(int cursor, char** actionOut, int cookie) {
    if (get_camaction(cursor, actionOut, cookie) == 0) {
        return 6;
    }
    char* action = *actionOut;
    if (*(int*)action == 1) {
        return readBoundObj_C510(cursor, action + 8, cookie);
    }
    if (*(int*)action == 2) {
        get_camspline(cursor, action + 8);
        return 0;
    }
    return 0;
}
#endif
