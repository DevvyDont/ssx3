#ifndef COMMON_H
#define COMMON_H

// Just include the Splat header for now.
#include "include_asm.h"

typedef unsigned int uint;
typedef unsigned long ulong;

// g++ 2.95's default static-initialization priority (gcc's DEFAULT_INIT_PRIORITY): a unit's generated
// __static_initialization_and_destruction_0(initialize, priority) builds and destroys its static objects only for it.
#define DEFAULT_INIT_PRIORITY 65535

#endif