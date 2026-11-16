#ifndef FAKEMATCH_H
#define FAKEMATCH_H

/* Fakematch helpers. Every use sits under a `// FAKEMATCH:` comment that names the construct and the plain
 * form's score, with a row in the workspace's notes/fakematches.tsv. They only steer the PS2 EE compilers
 * (ee-gcc 2.9 / SN 2.95.3, which define __R5900__) towards the retail binary; any other compiler, such as a
 * PC port's, gets the plain code, which behaves the same.
 *
 *   register int r FM_REG("$2") = 0;   local register variable: keep r in the register retail uses
 *   FM_OPAQUE(x);                       hide x's value from the optimiser at this point (x is unchanged)
 *   FM_BARRIER();                       stop the scheduler moving memory accesses across this point
 */
#ifdef __R5900__
#define FM_REG(r) __asm__(r)
#define FM_OPAQUE(x) __asm__("" : "=r"(x) : "0"(x))
#define FM_BARRIER() __asm__ __volatile__("" : : : "memory")
#else
#define FM_REG(r)
#define FM_OPAQUE(x) ((void)0)
#define FM_BARRIER() ((void)0)
#endif

#endif
