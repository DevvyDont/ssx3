#include "common.h"

//100%
INCLUDE_ASM("seg/ctor_1218", func_00100218);
#ifdef SKIP_ASM
extern void __deregister_frame_info(void* p);
extern char D_0043CE00[];

/**
 * Unregisters, at exit, the frame-info object crt0 registered at startup.
 *
 * Passes D_0043CE00, the key crt0 hands to __register_frame_info_table together with the object D_004A5D48, to __deregister_frame_info, which
 * unlinks the object registered under that key from the list D_0052ADE8 and frees its table. It is the last entry (word
 * 12) of the global destructor table D_0043CEE4, whose other entries run the units' static destructors. The unit was
 * built as C at -O0 (frame pointer, unfilled delay slots), so it has its own flags.
 */
void func_00100218(void)
{
    __deregister_frame_info(D_0043CE00);
}
#endif
