#include "md5.h"

INCLUDE_ASM("md5", md5_process);
#ifdef SKIP_ASM
#endif

INCLUDE_ASM("md5", md5_init);
#ifdef SKIP_ASM
void md5_init(md5_state_t* state)
{
	return;
}
#endif

INCLUDE_ASM("md5", md5_append);
#ifdef SKIP_ASM
#endif

INCLUDE_ASM("md5", md5_finish);
#ifdef SKIP_ASM
#endif