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
#include "seg/libgcc_frame.h"

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __default_terminate);
#ifdef SKIP_ASM
extern int abort(void) __attribute__((noreturn));
/**
 * Default terminate handler: calls abort (abort) and never returns.
 *
 * The terminate-handler pointer __terminate_func starts out pointing here.
 *
 * @return never returns
 */
int __default_terminate(void) {
    return abort();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __terminate);
#ifdef SKIP_ASM
extern void (*__terminate_func[])(void);
/**
 * Calls the current terminate handler __terminate_func (never returns).
 *
 * The exception-handling code calls it for every fatal state (no handler, no active exception, failed allocation or
 * unwind); the pure-virtual filler __pure_virtual calls it too.
 */
void __terminate(void) {
    __terminate_func[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __throw_type_match);
#ifdef SKIP_ASM
extern int strcmp(int, int);
/**
 * Returns @p obj when the catch and throw type names are the same string, else 0.
 *
 * Compares the names with strcmp.
 *
 * @param catchType  the catch clause's type name
 * @param throwType  the thrown type's name
 * @param obj  the thrown object
 * @return @p obj on a match, else 0
 */
int __throw_type_match(int catchType, int throwType, int obj) {
    if (strcmp(catchType, throwType)) {
        obj = 0;
    }
    return obj;
}
#endif

/**
 * Does nothing: an empty function that returns at once (libgcc's exception-handling support, libgcc2.c).
 *
 * Nothing in the ELF calls it; only its own .eh_frame FDE names it.
 */
void __empty(void) {
}

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", new_eh_context);
#ifdef SKIP_ASM
/* [L] libgcc2.c new_eh_context: the context and the top element of its handler chain in one allocation. */
struct eh_full_context {
    struct eh_context c;
    void *top_elt[2];
};
extern struct eh_full_context *new_eh_context_alloc(int) __asm__("malloc");
extern void __terminate(void);
extern void memset(void *, int, int);
/**
 * Allocates and clears a new EH context with its own top element for the handler chain.
 *
 * Allocates 0x18 bytes (malloc), calls __terminate (terminate) when that fails, clears them (memset)
 * and points the dynamic handler chain (word 1) at the two-word top element at +0x10. Nothing in this single-threaded
 * build calls it.
 *
 * @return the context
 */
struct eh_context *new_eh_context(void) {
    struct eh_full_context *ehfc = new_eh_context_alloc(sizeof *ehfc);
    if (!ehfc)
        __terminate();
    memset(ehfc, 0, sizeof *ehfc);
    ehfc->c.dynamic_handler_chain = ehfc->top_elt;
    return &ehfc->c;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __get_eh_context);
#ifdef SKIP_ASM
extern struct eh_context *(*get_eh_context[])(void);
/**
 * Calls the EH-context getter get_eh_context and leaves its result as this function's result.
 *
 * The getter starts as eh_context_initialize, which switches it to eh_context_static.
 */
void __get_eh_context(void) {
    get_eh_context[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __get_eh_info);
#ifdef SKIP_ASM
/**
 * Returns the address of the current exception's language-specific info pointer.
 *
 * Gets the EH context from the getter get_eh_context and returns the address of its info word (+0x8). The C++ runtime
 * (__start_cp_handler, __cp_exception_info and others) reads the current exception's record through it.
 *
 * @return the address of the context's info word
 */
void **__get_eh_info(void) {
    struct eh_context *eh = get_eh_context[0]();
    return &eh->info;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", eh_context_initialize);
#ifdef SKIP_ASM
extern struct eh_context *eh_context_static(void);
/**
 * First EH-context getter: switches the getter get_eh_context to eh_context_static and calls it.
 *
 * Without threads there is one static context, so the switch happens on the first call.
 */
void eh_context_initialize(void) {
    struct eh_context *(*staticGetter)(void) = eh_context_static;
    get_eh_context[0] = staticGetter;
    staticGetter();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", eh_context_static);
#ifdef SKIP_ASM
extern struct eh_context D_0052ADA0[];
extern void *D_0052ADB8[];
extern int D_0052ADB0[];
extern void *memset_4104E0(void *, int, int) __asm__("memset");
/**
 * Returns the static EH context, clearing it on the first call.
 *
 * On the first call (flag D_0052ADB0) it clears the 16-byte context D_0052ADA0 and points its handler chain (word 1) at
 * the top element D_0052ADB8.
 *
 * @return the address of D_0052ADA0
 */
struct eh_context *eh_context_static(void) {
    if (D_0052ADB0[0] == 0) {
        D_0052ADB0[0] = 1;
        memset_4104E0(D_0052ADA0, 0, sizeof D_0052ADA0[0]);
        D_0052ADA0[0].dynamic_handler_chain = D_0052ADB8;
    }
    return D_0052ADA0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __get_dynamic_handler_chain);
#ifdef SKIP_ASM
/**
 * Returns the address of the EH context's dynamic handler chain.
 *
 * Gets the EH context from the getter get_eh_context and returns the address of word 1.
 *
 * @return the address of the chain pointer
 */
void ***__get_dynamic_handler_chain(void) {
    struct eh_context *eh = get_eh_context[0]();
    return &eh->dynamic_handler_chain;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __sjthrow);
#ifdef SKIP_ASM
extern struct eh_context *(*get_eh_context[])(void);
extern void __terminate(void);
struct N570 { int *next; int cleanup; void *jb[3]; char pad[0x62C]; };
void sjthrow_410570(void) __asm__("__sjthrow");
/**
 * Throws in the setjmp/longjmp exception scheme: runs pending cleanups, then jumps to the top handler.
 *
 * Gets the EH context from the getter get_eh_context. When the top handler of the dynamic chain has cleanups, it pushes
 * a temporary handler (__builtin_setjmp; a throw from a cleanup ends in __terminate) and runs each cleanup with
 * (argument, 2), unlinking it first, then pops the temporary handler. With no active exception (context word 2) or an
 * empty chain it calls __terminate; otherwise it pops the top handler and longjmps into its buffer. Never returns.
 */
void sjthrow_410570(void) {
    // ORIGINAL-PTR: libgcc2.c __sjthrow: a dynamic handler chain element is a raw word buffer (the next element at word
    // 0, the cleanup chain at word 1, the jmpbuf from word 2) that the library reads through casts
    // (void ***cleanup = (void***)&(*dhc)[1]; func = (void(*)(void*, int))cleanup[0][1]).
    struct N570 node;
    struct eh_context *eh;
    int **pchain;
    int *topHandler;
    int **pcl;
    struct N570 *tempHandler;
    eh = get_eh_context[0]();
    pchain = (int **)&eh->dynamic_handler_chain;
    topHandler = (int *)eh->dynamic_handler_chain;
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
    if (eh->info == 0 || ((int *)*pchain)[0] == 0)
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
extern struct eh_context *(*get_eh_context[])(void);
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
    // ORIGINAL-PTR: libgcc2.c __sjpopnthrow: a dynamic handler chain element is a raw word buffer (the next element at
    // word 0, the cleanup chain at word 1, the jmpbuf from word 2) that the library reads through casts
    // (void ***cleanup = (void***)&(*dhc)[1]; func = (void(*)(void*, int))cleanup[0][1]).
    struct N6E8 node;
    struct eh_context *eh;
    int *topHandler;
    int **pchain;
    int **pcl;
    struct N6E8 *tempHandler;
    eh = get_eh_context[0]();
    pchain = (int **)&eh->dynamic_handler_chain;
    topHandler = (int *)eh->dynamic_handler_chain;
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
 * Reads the info record through __get_eh_info; without a match function (match_function) it returns 0, otherwise
 * the function's result (info, @p rtime, 0) as 1 or 0.
 *
 * @param rtime  the runtime type to test
 * @return 1 on a match, else 0
 */
int __eh_rtime_match(void *rtime) {
    __eh_info *info = *__get_eh_info();
    __eh_matcher matcher = info->match_function;
    if (matcher == 0)
        return 0;
    return matcher(info, rtime, 0) != 0;
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
void *old_find_exception_handler(void *pc, old_exception_table *tab) {
    int best, i;
    if (tab) {
        best = NO_EH_REGION;
        for (i = 0; tab[i].start_region != (void *)-1; i++) {
            if (pc >= tab[i].start_region && pc < tab[i].end_region) {
                if (best == NO_EH_REGION
                    || (tab[best].end_region >= tab[i].end_region && tab[best].start_region <= tab[i].start_region))
                    best = i;
            } else if (best >= 0 && pc < tab[i].start_region)
                break;
        }
        if (best != NO_EH_REGION)
            return tab[best].exception_handler;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", find_exception_handler);
#ifdef SKIP_ASM
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
void *find_exception_handler(void *pc, exception_descriptor *obj, __eh_info *ctx, int flag, int *out) {
    // ORIGINAL-PTR: libgcc2.c find_exception_handler: on a rethrow pc is the region table entry to rethrow out of
    // (pos = ((exception_table *) pc) - tab; pc = ((exception_table *) pc)->end_region - 1).
    void *retval = 0;
    *out = 1;
    if (obj != 0) {
        int idx = 0;
        exception_table *tbl = &obj->table[0];
        if (flag) {
            idx = (exception_table *)pc - tbl;
            pc = ((exception_table *)pc)->end_region - 1;
            if (tbl[idx].start_region != (void *)-1)
                idx++;
        } else {
            pc--;
        }
        for (; tbl[idx].start_region != (void *)-1; idx++) {
            if (pc >= tbl[idx].start_region && pc < tbl[idx].end_region) {
                if (tbl[idx].match_info) {
                    if (ctx->match_function) {
                        if (ctx->match_function(ctx, tbl[idx].match_info, obj)) {
                            if (retval == 0)
                                retval = tbl[idx].exception_handler;
                            *out = 0;
                            break;
                        }
                    }
                } else if (retval == 0) {
                    retval = tbl[idx].exception_handler;
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
extern int abort(void);
/**
 * Returns the address where a frame saved a register.
 *
 * Follows registers saved in other registers (kind 2) to the final one; for a register saved at an offset (kind 1) it
 * returns the frame's CFA plus that offset, otherwise it calls abort (abort).
 *
 * @param idx  the register number
 * @param udata  the frame state
 * @param z  the callee's frame state (unused)
 * @return the save address
 */
long long *get_reg_addr(int idx, frame_state *udata, frame_state *z) {
    // word_type is a quadword here: next_stack_level and throw_helper read the save slot with lq.
    // ORIGINAL-PTR: libgcc2.c get_reg_addr: return (word_type *)(udata->cfa + udata->reg_or_offset[reg]).
    if (udata->saved[idx] == REG_SAVED_REG) {
        do {
            idx = (int)udata->reg_or_offset[idx];
        } while (udata->saved[idx] == REG_SAVED_REG);
    }
    if (udata->saved[idx] == REG_SAVED_OFFSET)
        return (long long *)(udata->cfa + udata->reg_or_offset[idx]);
    abort();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", copy_reg);
#ifdef SKIP_ASM
extern void *func_0041605C(void *, const void *, int);
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
void *copy_reg(int reg, frame_state *udata, frame_state *targetUdata) {
    long long *x = get_reg_addr(reg, udata, 0);
    long long *y = get_reg_addr(reg, targetUdata, 0);
    int k;
    k = reg < 31 ? 16 : (reg <= 63 ? 4 : 8);
    return func_0041605C(y, x, k);
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", next_stack_level);
#ifdef SKIP_ASM
extern frame_state *__frame_state_for(void *, frame_state *);
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
frame_state *next_stack_level(void *framePc, frame_state *udata, frame_state *callerBuf) {
    // ORIGINAL-PTR: libgcc2.c next_stack_level: caller_udata->cfa = get_reg (caller_udata->cfa_reg, udata, 0) stores
    // a saved register word as the CFA, and caller_udata->cfa += caller_udata->cfa_offset.
    frame_state *callerUdata = __frame_state_for(framePc, callerBuf);
    unsigned short i;
    if (callerUdata == 0)
        return 0;
    i = callerUdata->cfa_reg;
    if (udata->saved[i] != REG_UNSAVED)
        {
            long long cfaValue = *get_reg_addr(i, udata, 0);
            callerUdata->cfa = (void *)(int)cfaValue;
        }
    else
        callerUdata->cfa = udata->cfa;
    callerUdata->cfa = callerUdata->cfa + callerUdata->cfa_offset;
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
extern void __terminate(void);
extern void __unwinding_cleanup(void);
/* libgcc2.c: without INCOMING_REGNO (MIPS) no register lives in a register window */
static inline int in_reg_window_410C18(int reg, frame_state *udata) { return 0; }
void *unwind_410C18(struct eh_context *eh, void *pc, frame_state *buf, frame_state **pout) __asm__("throw_helper");
/**
 * Finds the handler for the exception being thrown and unwinds the saved registers to its frame.
 *
 * Walks the callers' frames from @p pc (next_stack_level). Each frame's table is searched: from the context's rethrow
 * entry (table_index, cleared afterwards) on a rethrow, as a new-format table (runtime id -2,
 * find_exception_handler) or an old one (old_find_exception_handler). The first cleanup-only match is remembered and
 * the walk goes on; a real handler stops it. With only cleanups it uses the remembered one and calls
 * __unwinding_cleanup; with nothing it calls __terminate (terminate). The handler goes into the context's
 * handler_label.
 *
 * When the handler is not in the throwing frame it walks again from @p pc, copying every register each frame saved into
 * @p buf (copy_reg), and at the end the return-address register when that was saved in another register. *@p pout
 * receives the state of the frame the handler's frame called (@p buf when no unwinding was needed).
 *
 * @param eh  the EH context
 * @param pc  the throw address (return address - 1)
 * @param buf  the thrower's frame state; receives the unwound registers
 * @param pout  in: a frame-state buffer to work in; out: the frame state below the handler
 * @return the handler address
 */
void *unwind_410C18(struct eh_context *eh, void *pc, frame_state *buf, frame_state **pout) {
    // ORIGINAL-PTR: libgcc2.c get_return_addr, which throw_helper steps with: the return address is a saved register
    // word, (void *)(ptr_type)get_reg (udata->retaddr_column, udata, sub_udata).
    frame_state first;
    frame_state save;
    frame_state *cur;
    frame_state *calleeFrame;
    frame_state *prev;
    int flag;
    void *pc0, *sres, *spc;
    int have2, have;
    void *result = 0;
    __eh_info *regs = eh->info;
    int fl3 = eh->table_index != 0;
    have2 = 0;
    have = 0;
    pc0 = pc;
    flag = 0;
    cur = *pout;
    prev = &first;
    for (;;) {
        calleeFrame = cur;
        cur = next_stack_level(pc, calleeFrame, prev);
        prev = calleeFrame;
        if (cur == 0)
            break;
        {
            int newEhModel;
            if (cur->eh_ptr == 0)
                newEhModel = 0;
            else
                newEhModel = ((exception_descriptor *)cur->eh_ptr)->runtime_id_field == (void *)-2;
            if (fl3) {
                fl3 = 0;
                result = find_exception_handler(eh->table_index, cur->eh_ptr, regs, 1, &flag);
                eh->table_index = 0;
            } else if (newEhModel) {
                result = find_exception_handler(pc, cur->eh_ptr, regs, 0, &flag);
            } else {
                result = old_find_exception_handler(pc, cur->eh_ptr);
            }
        }
        if (result != 0) {
            if (flag != 0) {
                if (have == 0) {
                    save = *cur;
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
            long long returnAddr = *get_reg_addr(cur->retaddr_column, cur, calleeFrame);
            pc = (void *)(int)returnAddr - 1;
        }
    }
    if (have) {
        cur = &save;
        result = sres;
        pc = spc;
        if (have2)
            __unwinding_cleanup();
    }
    if (result == 0)
        __terminate();
    eh->handler_label = result;
    {
        void *end = pc;
        if (end == pc0) {
            cur = buf;
        } else {
            int i;
            void *stop = end;
            pc = pc0;
            *cur = *buf;
            while (pc != stop) {
                calleeFrame = cur;
                cur = next_stack_level(pc, calleeFrame, prev);
                prev = calleeFrame;
                for (i = 0; i < 79; i++) {
                    if (i != cur->retaddr_column && cur->saved[i] != REG_UNSAVED) {
                        if (in_reg_window_410C18(i, cur) && cur->saved[cur->retaddr_column] == REG_SAVED_REG
                            && cur->reg_or_offset[cur->retaddr_column] == i)
                            continue;
                        copy_reg(i, cur, buf);
                    }
                }
                {
                    long long returnAddr = *get_reg_addr(cur->retaddr_column, cur, calleeFrame);
                    pc = (void *)(int)returnAddr - 1;
                }
            }
            /* update the return address register saved in a register (REG_SAVED_REG) by the last frame unwound */
            if (cur->saved[cur->retaddr_column] == REG_SAVED_REG) {
                i = cur->reg_or_offset[cur->retaddr_column];
                if (in_reg_window_410C18(i, cur))
                    copy_reg(i, cur, buf);
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
extern void __terminate(void);
/**
 * Throws the current exception: finds its handler and jumps there with the registers restored.
 *
 * Gets the EH context from the getter get_eh_context and calls __terminate when no exception is active (info is 0).
 * Gets its own frame state (__frame_state_for; terminate when missing) with the CFA, copies it and passes its return
 * address - 1 to throw_helper. __builtin_eh_return then restores the handler frame's registers, moves the stack by the
 * difference of the two CFAs and jumps to the handler. Never returns. The C++ throw helpers of seg_313800_exception
 * call it.
 */
void __throw(void) {
    frame_state ctx0;
    frame_state buf;
    frame_state *frameState;
    struct eh_context *eh;
    frame_state *pb;
    frame_state **pa;
    void *pc;
    __builtin_unwind_init();
    eh = get_eh_context[0]();
    pb = &buf;
    frameState = &ctx0;
    // Retail tests info with a word load: a pointer-typed test (if (!eh->info)) schedules the code differently.
    if (*(int *)&eh->info == 0) {
        __terminate();
        pa = &frameState;
    } else {
        pa = &frameState;
    }
lbl:
    pc = &&lbl;
    frameState = __frame_state_for(pc, frameState);
    if (frameState == 0)
        __terminate();
    frameState->cfa = __builtin_dwarf_cfa();
    buf = *frameState;
    {
        void *handlerAddr = unwind_410C18(eh, __builtin_return_address(0) - 1, pb, pa);
        __builtin_eh_return(eh, frameState->cfa - pb->cfa, handlerAddr);
    }
}
__asm__(".globl D_004110B8\n\t.type D_004110B8,@function\n\tD_004110B8 = __throw + 0x170\n");
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", __rethrow);
#ifdef SKIP_ASM
extern void __terminate(void);
/**
 * Rethrows the current exception from a given exception-table entry.
 *
 * Like __throw, but first stores @p val in the context's rethrow index (table_index), so throw_helper starts its
 * search after that region. Never returns.
 *
 * @param val  the table entry to rethrow out of
 */
void __rethrow(void *val) {
    frame_state ctx0;
    frame_state buf;
    frame_state *frameState;
    struct eh_context *eh;
    frame_state *pb;
    frame_state **pa;
    void *pc;
    __builtin_unwind_init();
    eh = get_eh_context[0]();
    pb = &buf;
    frameState = &ctx0;
    // Retail tests info with a word load, as in __throw.
    if (*(int *)&eh->info == 0)
        __terminate();
    eh->table_index = val;
    pa = &frameState;
lbl:
    pc = &&lbl;
    frameState = __frame_state_for(pc, frameState);
    if (frameState == 0)
        __terminate();
    frameState->cfa = __builtin_dwarf_cfa();
    buf = *frameState;
    {
        void *handlerAddr = unwind_410C18(eh, __builtin_return_address(0) - 1, pb, pa);
        __builtin_eh_return(eh, frameState->cfa - pb->cfa, handlerAddr);
    }
}
__asm__(".globl D_004112A0\n\t.type D_004112A0,@function\n\tD_004112A0 = __rethrow + 0x178\n");
__asm__(".globl D_004111D4\n\t.type D_004111D4,@function\n\tD_004111D4 = __rethrow + 0xAC\n");
#endif
