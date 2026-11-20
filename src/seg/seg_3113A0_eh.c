/* GNU CC runtime support, reconstructed: libgcc2.c, the L_eh exception-handling runtime.
   Original: Copyright (C) 1989, 92-98, 1999 Free Software Foundation, Inc.
   It is free software under the GNU General Public License, version 2 or (at your option) any later version, with
   this special exception: "if you link this library with other files, some of which are compiled with GCC, to
   produce an executable, this library does not by itself cause the resulting executable to be covered by the
   GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License."
   This file stays under those terms; the repository's CC0 dedication does not apply to it (README, License).
   Bodies credited to peak3project come from their fork (github.com/peak3project/ssx3). */

#include "common.h"

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __default_terminate);
#ifdef SKIP_ASM
extern int func_00418790(void) __attribute__((noreturn));
/**
 * Default terminate handler: calls func_00418790 (abort) and never returns.
 *
 * The terminate-handler pointer D_00453440 starts out pointing here.
 *
 * @return never returns
 */
int __default_terminate(void) {
    return func_00418790();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __terminate);
#ifdef SKIP_ASM
extern void (*D_00453440[])(void);
/**
 * Calls the current terminate handler D_00453440 (never returns).
 *
 * The exception-handling code calls it for every fatal state (no handler, no active exception, failed allocation or
 * unwind); the pure-virtual filler __pure_virtual calls it too.
 */
void __terminate(void) {
    D_00453440[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __throw_type_match);
#ifdef SKIP_ASM
extern int func_004165A8(int, int);
/**
 * Returns @p obj when the catch and throw type names are the same string, else 0.
 *
 * Compares the names with func_004165A8.
 *
 * @param catchType  the catch clause's type name
 * @param throwType  the thrown type's name
 * @param obj  the thrown object
 * @return @p obj on a match, else 0
 */
int __throw_type_match(int catchType, int throwType, int obj) {
    if (func_004165A8(catchType, throwType)) {
        obj = 0;
    }
    return obj;
}
#endif

/**
 * Does nothing.
 */
void __empty(void) {
}

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", new_eh_context);
#ifdef SKIP_ASM
extern int *new_eh_context_alloc(int) __asm__("func_004188C0");
extern void __terminate(void);
extern void func_00416210(int *, int, int);
/**
 * Allocates and clears a new EH context with its own top element for the handler chain.
 *
 * Allocates 0x18 bytes (func_004188C0), calls __terminate (terminate) when that fails, clears them (func_00416210)
 * and points the dynamic handler chain (word 1) at the two-word top element at +0x10. Nothing in this single-threaded
 * build calls it.
 *
 * @return the context
 */
int *new_eh_context(void) {
    int *fullContext = new_eh_context_alloc(0x18);
    if (!fullContext)
        __terminate();
    func_00416210(fullContext, 0, 0x18);
    fullContext[1] = (int)(fullContext + 4);
    return fullContext;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __get_eh_context);
#ifdef SKIP_ASM
extern int (*D_00453444[])(void);
/**
 * Calls the EH-context getter D_00453444 and leaves its result as this function's result.
 *
 * The getter starts as eh_context_initialize, which switches it to eh_context_static.
 */
void __get_eh_context(void) {
    D_00453444[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410490);
#ifdef SKIP_ASM
/**
 * Returns the address of the current exception's language-specific info pointer.
 *
 * Gets the EH context from the getter D_00453444 and returns the address of its info word (+0x8). The C++ runtime
 * (func_004128A8, __cp_exception_info and others) reads the current exception's record through it.
 *
 * @return the address of the context's info word
 */
int *func_00410490(void) {
    return (int *)D_00453444[0]() + 2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", eh_context_initialize);
#ifdef SKIP_ASM
extern int eh_context_static(void);
/**
 * First EH-context getter: switches the getter D_00453444 to eh_context_static and calls it.
 *
 * Without threads there is one static context, so the switch happens on the first call.
 */
void eh_context_initialize(void) {
    int (*staticGetter)(void) = eh_context_static;
    D_00453444[0] = staticGetter;
    staticGetter();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", eh_context_static);
#ifdef SKIP_ASM
extern int D_0052ADA0[];
extern char D_0052ADB8[];
extern int D_0052ADB0[];
extern void *memset_4104E0(void *, int, int) __asm__("func_00416210");
/**
 * Returns the static EH context, clearing it on the first call.
 *
 * On the first call (flag D_0052ADB0) it clears the 16-byte context D_0052ADA0 and points its handler chain (word 1) at
 * the top element D_0052ADB8.
 *
 * @return the address of D_0052ADA0
 */
int eh_context_static(void) {
    if (D_0052ADB0[0] == 0) {
        D_0052ADB0[0] = 1;
        memset_4104E0(D_0052ADA0, 0, 16);
        *(char **)(D_0052ADA0 + 1) = D_0052ADB8;
    }
    return (int)D_0052ADA0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __get_dynamic_handler_chain);
#ifdef SKIP_ASM
/**
 * Returns the address of the EH context's dynamic handler chain.
 *
 * Gets the EH context from the getter D_00453444 and returns the address of word 1.
 *
 * @return the address of the chain pointer
 */
int *__get_dynamic_handler_chain(void) {
    return (int *)D_00453444[0]() + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __sjthrow);
#ifdef SKIP_ASM
extern int (*D_00453444[])(void);
extern void __terminate(void);
struct N570 { int *next; int cleanup; void *jb[3]; char pad[0x62C]; };
void sjthrow_410570(void) __asm__("__sjthrow");
/**
 * Throws in the setjmp/longjmp exception scheme: runs pending cleanups, then jumps to the top handler.
 *
 * Gets the EH context from the getter D_00453444. When the top handler of the dynamic chain has cleanups, it pushes a
 * temporary handler (__builtin_setjmp; a throw from a cleanup ends in __terminate) and runs each cleanup with
 * (argument, 2), unlinking it first, then pops the temporary handler. With no active exception (context word 2) or an
 * empty chain it calls __terminate; otherwise it pops the top handler and longjmps into its buffer. Never returns.
 */
void sjthrow_410570(void) {
    struct N570 node;
    int *eh;
    int **pchain;
    int *topHandler;
    int **pcl;
    struct N570 *tempHandler;
    eh = (int *)D_00453444[0]();
    pchain = (int **)&eh[1];
    topHandler = (int *)eh[1];
    pcl = (int **)&topHandler[1];
    if (topHandler[1] != 0) {
        node.next = topHandler;
        node.cleanup = 0;
        tempHandler = &node;
        if (__builtin_setjmp(node.jb) == 0) {
            *pchain = (int *)tempHandler;
                        {
                int *n;
                while (*pcl != 0) {
                    n = *pcl;
                    *pcl = (int *)n[0];
                    ((void (*)(int, int))n[1])(n[2], 2);
                }
            }
            *pchain = tempHandler->next;
        } else {
            __terminate();
        }
    }
    if (eh[2] == 0 || ((int *)*pchain)[0] == 0)
        __terminate();
    {
        int *target = *pchain;
        *pchain = (int *)target[0];
        __builtin_longjmp((void **)&target[2], 1);
    }
}
__asm__(".globl D_00410628\n\t.type D_00410628,@function\n\tD_00410628 = __sjthrow + 0xB8\n");
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __sjpopnthrow);
#ifdef SKIP_ASM
extern int (*D_00453444[])(void);
extern int __sjthrow(void) __attribute__((noreturn));
struct N6E8 { int *next; int cleanup; void *jb[3]; char pad[0x62C]; };
/**
 * Runs the top handler's pending cleanups, pops that handler and throws on (__sjthrow).
 *
 * Same cleanup loop as __sjthrow under a temporary handler (__builtin_setjmp), then removes the top element of the
 * dynamic handler chain and calls __sjthrow. Never returns.
 *
 * @return never returns
 */
int __sjpopnthrow(void) {
    struct N6E8 node;
    int *eh;
    int *topHandler;
    int **pchain;
    int **pcl;
    struct N6E8 *tempHandler;
    eh = (int *)D_00453444[0]();
    pchain = (int **)&eh[1];
    topHandler = (int *)eh[1];
    pcl = (int **)&topHandler[1];
    if (topHandler[1] != 0) {
        node.next = topHandler;
        node.cleanup = 0;
        tempHandler = &node;
        __builtin_setjmp(node.jb);
        *pchain = (int *)tempHandler;
                {
            int *n;
            while (*pcl != 0) {
                n = *pcl;
                *pcl = (int *)n[0];
                ((void (*)(int, int))n[1])(n[2], 2);
            }
        }
        *pchain = tempHandler->next;
    }
    *pchain = (int *)(*pchain)[0];
    return __sjthrow();
}
__asm__(".globl D_00410790\n\t.type D_00410790,@function\n\tD_00410790 = __sjpopnthrow + 0xA8\n");
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __eh_rtime_match);
#ifdef SKIP_ASM
/**
 * Asks the current exception's match function whether it matches a runtime type.
 *
 * Reads the info record through func_00410490; without a match function (its word 0) it returns 0, otherwise the
 * function's result (info, @p rtime, 0) as 1 or 0.
 *
 * @param rtime  the runtime type to test
 * @return 1 on a match, else 0
 */
int __eh_rtime_match(int rtime) {
    int *info = *(int **)func_00410490();
    int (*fn)(int *, int, int) = (int (*)(int *, int, int))info[0];
    if (fn == 0)
        return 0;
    return fn(info, rtime, 0) != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __get_eh_table_version);
#ifdef SKIP_ASM
/**
 * Returns the version field (the short at +6) of an exception table descriptor.
 *
 * @param table  the descriptor
 * @return the version
 */
int __get_eh_table_version(short *table) {
    return table[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __get_eh_table_language);
#ifdef SKIP_ASM
/**
 * Returns the language field (the short at +4) of an exception table descriptor.
 *
 * @param table  the descriptor
 * @return the language
 */
int __get_eh_table_language(short *table) {
    return table[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", old_find_exception_handler);
#ifdef SKIP_ASM
struct E410858 { unsigned start; unsigned end; int data; };
/**
 * Finds the handler of the innermost region containing a PC in an old-format exception table.
 *
 * Scans the (start, end, handler) entries up to a start of -1, keeping the region that lies within the best one so far
 * (the later one on a tie) and stopping at the first entry past the PC once one matched.
 *
 * @param pc  the address
 * @param tab  the table, or 0
 * @return the handler address, or 0
 */
int old_find_exception_handler(unsigned pc, struct E410858 *tab) {
    int best, i;
    if (tab) {
        best = -1;
        for (i = 0; tab[i].start != 0xFFFFFFFF; i++) {
            if (pc >= tab[i].start && pc < tab[i].end) {
                if (best == -1 || (tab[best].end >= tab[i].end && tab[best].start <= tab[i].start))
                    best = i;
            } else if (best >= 0 && pc < tab[i].start)
                break;
        }
        if (best != -1)
            return tab[best].data;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", find_exception_handler);
#ifdef SKIP_ASM
struct E410930 { unsigned start; unsigned end; int val; int cb; };
struct C410930 { int (*match)(struct C410930 *, int, int); };
/**
 * Finds the handler for an exception in a new-format exception table, noting cleanup-only matches.
 *
 * Scans the regions after the table's 8-byte header, from @p pc - 1, or on a rethrow from the entry after @p pc (then a
 * table entry) with its end address - 1. A region with a type needs the exception's match function (@p ctx word 0) to
 * accept it: that handler wins and clears *@p out. A region without a type is a cleanup: its handler is kept if none
 * was found yet and the scan goes on.
 *
 * @param pc  the throw address, or the table entry on a rethrow
 * @param obj  the table descriptor, or 0
 * @param ctx  the exception's info record
 * @param flag  non-zero for a rethrow
 * @param out  receives 1 when only a cleanup was found, 0 for a real handler
 * @return the handler address, or 0
 */
int find_exception_handler(unsigned pc, int obj, struct C410930 *ctx, int flag, int *out) {
    int retval = 0;
    *out = 1;
    if (obj != 0) {
        int idx = 0;
        struct E410930 *tbl = (struct E410930 *)(obj + 8);
        if (flag) {
            idx = ((int)pc - (int)tbl) >> 4;
            pc = ((unsigned *)pc)[1] - 1;
            if (tbl[idx].start != 0xFFFFFFFF)
                idx++;
        } else {
            pc = pc - 1;
        }
        for (; tbl[idx].start != 0xFFFFFFFF; idx++) {
            if (pc >= tbl[idx].start && pc < tbl[idx].end) {
                if (tbl[idx].cb) {
                    if (ctx->match) {
                        if (ctx->match(ctx, tbl[idx].cb, obj)) {
                            if (retval == 0)
                                retval = tbl[idx].val;
                            *out = 0;
                            break;
                        }
                    }
                } else if (retval == 0) {
                    retval = tbl[idx].val;
                }
            }
        }
    }
    return retval;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", get_reg_addr);
#ifdef SKIP_ASM
struct S410A78 { int base; char pad[0x14]; long e[0x50]; unsigned short n; short pad2; signed char kind[1]; };
extern int func_00418790(void);
/**
 * Returns the address where a frame saved a register.
 *
 * Follows registers saved in other registers (kind 2) to the final one; for a register saved at an offset (kind 1) it
 * returns the frame's CFA plus that offset, otherwise it calls func_00418790 (abort).
 *
 * @param idx  the register number
 * @param udata  the frame state
 * @param z  the callee's frame state (unused)
 * @return the save address
 */
int get_reg_addr(int idx, struct S410A78 *udata, int z) {
    if (udata->kind[idx] == 2) {
        do {
            idx = (int)udata->e[idx];
        } while (udata->kind[idx] == 2);
    }
    if (udata->kind[idx] == 1)
        return udata->base + (int)udata->e[idx];
    return func_00418790();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", copy_reg);
#ifdef SKIP_ASM
extern int get_reg_addr();
extern int func_0041605C(int, int, int);
/**
 * Copies a register's saved value from one frame's save slot to another's.
 *
 * Finds both slots with get_reg_addr and copies 16 bytes for registers below 31, 4 for 31..63 and 8 above
 * (func_0041605C).
 *
 * @param reg  the register number
 * @param udata  the frame to copy from
 * @param targetUdata  the frame to copy to
 * @return func_0041605C's result
 */
int copy_reg(int reg, int udata, int targetUdata) {
    int x = get_reg_addr(reg, udata, 0);
    int y = get_reg_addr(reg, targetUdata, 0);
    int k;
    k = reg < 31 ? 16 : (reg <= 63 ? 4 : 8);
    return func_0041605C(y, x, k);
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", next_stack_level);
#ifdef SKIP_ASM
extern int *__frame_state_for();
extern long long *next_stack_level_idx() __asm__("get_reg_addr");
/**
 * Computes the frame state of the caller of a frame.
 *
 * Gets the unwind information for @p framePc into @p callerBuf (__frame_state_for; 0 when there is none). The caller's CFA
 * is the value its CFA register was saved with in @p udata (get_reg_addr) when saved, else @p udata's CFA, plus the
 * caller's CFA offset.
 *
 * @param framePc  an address inside the caller
 * @param udata  the current frame's state
 * @param callerBuf  the buffer for the caller's state
 * @return the caller's frame state, or 0
 */
int *next_stack_level(int framePc, int *udata, int callerBuf) {
    int *callerUdata = __frame_state_for(framePc, callerBuf);
    unsigned short i;
    if (callerUdata == 0)
        return 0;
    i = *(unsigned short *)((char *)callerUdata + 0x298);
    if (*((char *)udata + i + 0x29C) != 0)
        {
            long long cfaValue = *next_stack_level_idx(i, udata, 0);
            callerUdata[0] = (int)cfaValue;
        }
    else
        callerUdata[0] = udata[0];
    callerUdata[0] = callerUdata[0] + callerUdata[2];
    return callerUdata;
}
#endif

/**
 * Empty hook that throw_helper calls when only cleanups are left to run for a throw.
 *
 * A debugger can stop here to see the throw before the stack is unwound.
 */
void __unwinding_cleanup(void) {
}

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", throw_helper);
#ifdef SKIP_ASM
struct S410C18 { char b[0x2F0]; } __attribute__((aligned(8)));
/* frame.h frame_state: cfa, eh_ptr, cfa_offset, args_size, reg_or_offset[], cfa_reg, retaddr_column (idx), saved[]
   (kind) */
struct F410C18 { int f0; unsigned *p; long cfa_offset; long args_size; long reg_or_offset[0x50]; unsigned short cfa_reg;
    unsigned short idx; signed char kind[0x50]; } __attribute__((aligned(8)));
extern void __terminate(void);
extern void __unwinding_cleanup(void);
extern struct F410C18 *f_b80_410C18(int, struct F410C18 *, struct S410C18 *) __asm__("next_stack_level");
extern long long *f_a78_410C18(int, struct F410C18 *, struct S410C18 *) __asm__("get_reg_addr");
extern void f_b00_410C18(int, struct F410C18 *, struct S410C18 *) __asm__("copy_reg");
extern int f_930_410C18(int, int, int, int, int *) __asm__("find_exception_handler");
extern int f_858_410C18(int, int) __asm__("old_find_exception_handler");
/* libgcc2.c: without INCOMING_REGNO (MIPS) no register lives in a register window */
static inline int in_reg_window_410C18(int reg, struct F410C18 *udata) { return 0; }
int unwind_410C18(int *info, int pc, struct S410C18 *buf, struct F410C18 **pout) __asm__("throw_helper");
/**
 * Finds the handler for the exception being thrown and unwinds the saved registers to its frame.
 *
 * Walks the callers' frames from @p pc (next_stack_level). Each frame's table is searched: from the context's rethrow
 * entry (word 3, cleared afterwards) on a rethrow, as a new-format table (runtime id 0xFFFFFFFE, find_exception_handler) or an
 * old one (old_find_exception_handler). The first cleanup-only match is remembered and the walk goes on; a real handler stops it.
 * With only cleanups it uses the remembered one and calls __unwinding_cleanup; with nothing it calls __terminate
 * (terminate). The handler goes into the context's word 0.
 *
 * When the handler is not in the throwing frame it walks again from @p pc, copying every register each frame saved into
 * @p buf (copy_reg), and at the end the return-address register when that was saved in another register. *@p pout
 * receives the state of the frame the handler's frame called (@p buf when no unwinding was needed).
 *
 * @param info  the EH context
 * @param pc  the throw address (return address - 1)
 * @param buf  the thrower's frame state; receives the unwound registers
 * @param pout  in: a frame-state buffer to work in; out: the frame state below the handler
 * @return the handler address
 */
int unwind_410C18(int *info, int pc, struct S410C18 *buf, struct F410C18 **pout) {
    struct S410C18 first;
    struct S410C18 save;
    struct F410C18 *cur;
    struct F410C18 *calleeFrame;
    struct S410C18 *prev;
    int flag;
    int pc0, sres, spc, have2, have;
    int result = 0;
    int regs = info[2];
    int fl3 = info[3] != 0;
    have2 = 0;
    have = 0;
    pc0 = pc;
    flag = 0;
    cur = *pout;
    prev = &first;
    for (;;) {
        calleeFrame = cur;
        cur = f_b80_410C18(pc, calleeFrame, prev);
        prev = (struct S410C18 *)calleeFrame;
        if (cur == 0)
            break;
        {
            int newEhModel;
            if (cur->p == 0)
                newEhModel = 0;
            else
                newEhModel = *cur->p == 0xFFFFFFFE;
            if (fl3) {
                fl3 = 0;
                result = f_930_410C18(info[3], (int)cur->p, regs, 1, &flag);
                info[3] = 0;
            } else if (newEhModel) {
                result = f_930_410C18(pc, (int)cur->p, regs, 0, &flag);
            } else {
                result = f_858_410C18(pc, (int)cur->p);
            }
        }
        if (result != 0) {
            if (flag != 0) {
                if (have == 0) {
                    save = *(struct S410C18 *)cur;
                    spc = pc;
                    sres = result;
                    have = 1;
                    have2 = 1;
                }
            } else {
                have2 = 0;
                break;
            }
        }
        {
            long long returnAddr = *f_a78_410C18(cur->idx, cur, (struct S410C18 *)calleeFrame);
            pc = (int)returnAddr - 1;
        }
    }
    if (have) {
        cur = (struct F410C18 *)&save;
        result = sres;
        pc = spc;
        if (have2)
            __unwinding_cleanup();
    }
    if (result == 0)
        __terminate();
    *info = result;
    {
        int end = pc;
        if (end == pc0) {
            cur = (struct F410C18 *)buf;
        } else {
            int i;
            int stop = end;
            pc = pc0;
            *cur = *(struct F410C18 *)buf;
            while (pc != stop) {
                calleeFrame = cur;
                cur = f_b80_410C18(pc, calleeFrame, (struct S410C18 *)prev);
                prev = (struct S410C18 *)calleeFrame;
                for (i = 0; i < 79; i++) {
                    if (i != cur->idx && cur->kind[i] != 0) {
                        if (in_reg_window_410C18(i, cur) && cur->kind[cur->idx] == 2 && cur->reg_or_offset[cur->idx] == i)
                            continue;
                        f_b00_410C18(i, cur, buf);
                    }
                }
                {
                    long long returnAddr = *f_a78_410C18(cur->idx, cur, (struct S410C18 *)calleeFrame);
                    pc = (int)returnAddr - 1;
                }
            }
            /* update the return address register saved in a register (REG_SAVED_REG) by the last frame unwound */
            if (cur->kind[cur->idx] == 2) {
                i = cur->reg_or_offset[cur->idx];
                if (in_reg_window_410C18(i, cur))
                    f_b00_410C18(i, cur, buf);
            }
        }
        *pout = cur;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __throw);
#ifdef SKIP_ASM
struct S410F48 { char b[0x2F0]; } __attribute__((aligned(8)));
extern void __terminate(void);
extern struct S410F48 *get_410F48(void *, void *) __asm__("__frame_state_for");
extern void *throw_helper(void *, int, void *, void *);
/**
 * Throws the current exception: finds its handler and jumps there with the registers restored.
 *
 * Gets the EH context from the getter D_00453444 and calls __terminate when no exception is active (word 2 is 0).
 * Gets its own frame state (__frame_state_for; terminate when missing) with the CFA, copies it and passes its return
 * address - 1 to throw_helper. __builtin_eh_return then restores the handler frame's registers, moves the stack by the
 * difference of the two CFAs and jumps to the handler. Never returns. The C++ throw helpers of seg_313800_exception
 * call it.
 */
void __throw(void) {
    struct S410F48 ctx0;
    struct S410F48 buf;
    struct S410F48 *frameState;
    int *info;
    struct S410F48 *pb;
    void **pa;
    void *pc;
    __builtin_unwind_init();
    info = (int *)D_00453444[0]();
    pb = &buf;
    frameState = &ctx0;
    if (info[2] == 0) {
        __terminate();
        pa = (void **)&frameState;
    } else {
        pa = (void **)&frameState;
    }
lbl:
    pc = (void *)&&lbl;
    frameState = get_410F48(pc, (void *)frameState);
    if (frameState == 0)
        __terminate();
    *(void **)frameState = __builtin_dwarf_cfa();
    buf = *frameState;
    {
        void *handlerAddr = throw_helper(info, (int)__builtin_return_address(0) - 1, pb, pa);
        __builtin_eh_return(info, *(int *)frameState - *(int *)pb, handlerAddr);
    }
}
__asm__(".globl D_004110B8\n\t.type D_004110B8,@function\n\tD_004110B8 = __throw + 0x170\n");
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __rethrow);
#ifdef SKIP_ASM
struct S411128 { char b[0x2F0]; } __attribute__((aligned(8)));
extern void __terminate(void);
extern struct S411128 *get_411128(void *, void *) __asm__("__frame_state_for");
extern void *throw_helper(void *, int, void *, void *);
/**
 * Rethrows the current exception from a given exception-table entry.
 *
 * Like __throw, but first stores @p val in the context's rethrow index (word 3), so throw_helper starts its
 * search after that region. Never returns.
 *
 * @param val  the table entry to rethrow out of
 */
void __rethrow(int val) {
    struct S411128 ctx0;
    struct S411128 buf;
    struct S411128 *frameState;
    int *info;
    struct S411128 *pb;
    void **pa;
    void *pc;
    __builtin_unwind_init();
    info = (int *)D_00453444[0]();
    pb = &buf;
    frameState = &ctx0;
    if (info[2] == 0)
        __terminate();
    info[3] = val;
    pa = (void **)&frameState;
lbl:
    pc = (void *)&&lbl;
    frameState = get_411128(pc, (void *)frameState);
    if (frameState == 0)
        __terminate();
    *(void **)frameState = __builtin_dwarf_cfa();
    buf = *frameState;
    {
        void *handlerAddr = throw_helper(info, (int)__builtin_return_address(0) - 1, pb, pa);
        __builtin_eh_return(info, *(int *)frameState - *(int *)pb, handlerAddr);
    }
}
__asm__(".globl D_004112A0\n\t.type D_004112A0,@function\n\tD_004112A0 = __rethrow + 0x178\n");
__asm__(".globl D_004111D4\n\t.type D_004111D4,@function\n\tD_004111D4 = __rethrow + 0xAC\n");
#endif
