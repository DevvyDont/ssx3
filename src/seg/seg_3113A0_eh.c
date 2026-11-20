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
INCLUDE_ASM("seg/seg_3113A0_eh", func_004103A0);
#ifdef SKIP_ASM
extern int func_00418790(void) __attribute__((noreturn));
/**
 * Default terminate handler: calls func_00418790 (abort) and never returns.
 *
 * The terminate-handler pointer D_00453440 starts out pointing here.
 *
 * @return never returns
 */
int func_004103A0(void) {
    return func_00418790();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_004103B0);
#ifdef SKIP_ASM
extern void (*D_00453440[])(void);
/**
 * Calls the current terminate handler D_00453440 (never returns).
 *
 * The exception-handling code calls it for every fatal state (no handler, no active exception, failed allocation or
 * unwind); the pure-virtual filler func_00411530 calls it too.
 */
void func_004103B0(void) {
    D_00453440[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_004103D8);
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
int func_004103D8(int catchType, int throwType, int obj) {
    if (func_004165A8(catchType, throwType)) {
        obj = 0;
    }
    return obj;
}
#endif

/**
 * Does nothing.
 */
void func_00410408(void) {
}

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410410);
#ifdef SKIP_ASM
extern int *func_00410410_alloc(int) __asm__("func_004188C0");
extern void func_004103B0(void);
extern void func_00416210(int *, int, int);
/**
 * Allocates and clears a new EH context with its own top element for the handler chain.
 *
 * Allocates 0x18 bytes (func_004188C0), calls func_004103B0 (terminate) when that fails, clears them (func_00416210)
 * and points the dynamic handler chain (word 1) at the two-word top element at +0x10. Nothing in this single-threaded
 * build calls it.
 *
 * @return the context
 */
int *func_00410410(void) {
    int *fullContext = func_00410410_alloc(0x18);
    if (!fullContext)
        func_004103B0();
    func_00416210(fullContext, 0, 0x18);
    fullContext[1] = (int)(fullContext + 4);
    return fullContext;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410468);
#ifdef SKIP_ASM
extern int (*D_00453444[])(void);
/**
 * Calls the EH-context getter D_00453444 and leaves its result as this function's result.
 *
 * The getter starts as func_004104B8, which switches it to func_004104E0.
 */
void func_00410468(void) {
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
 * (func_004128A8, func_00412860 and others) reads the current exception's record through it.
 *
 * @return the address of the context's info word
 */
int *func_00410490(void) {
    return (int *)D_00453444[0]() + 2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_004104B8);
#ifdef SKIP_ASM
extern int func_004104E0(void);
/**
 * First EH-context getter: switches the getter D_00453444 to func_004104E0 and calls it.
 *
 * Without threads there is one static context, so the switch happens on the first call.
 */
void func_004104B8(void) {
    int (*fp)(void) = func_004104E0;
    D_00453444[0] = fp;
    fp();
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_004104E0);
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
int func_004104E0(void) {
    if (D_0052ADB0[0] == 0) {
        D_0052ADB0[0] = 1;
        memset_4104E0(D_0052ADA0, 0, 16);
        *(char **)(D_0052ADA0 + 1) = D_0052ADB8;
    }
    return (int)D_0052ADA0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410548);
#ifdef SKIP_ASM
/**
 * Returns the address of the EH context's dynamic handler chain.
 *
 * Gets the EH context from the getter D_00453444 and returns the address of word 1.
 *
 * @return the address of the chain pointer
 */
int *func_00410548(void) {
    return (int *)D_00453444[0]() + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410570);
#ifdef SKIP_ASM
extern int (*D_00453444[])(void);
extern void func_004103B0(void);
struct N570 { int *next; int cleanup; void *jb[3]; char pad[0x62C]; };
void sjthrow_410570(void) __asm__("func_00410570");
/**
 * Throws in the setjmp/longjmp exception scheme: runs pending cleanups, then jumps to the top handler.
 *
 * Gets the EH context from the getter D_00453444. When the top handler of the dynamic chain has cleanups, it pushes a
 * temporary handler (__builtin_setjmp; a throw from a cleanup ends in func_004103B0) and runs each cleanup with
 * (argument, 2), unlinking it first, then pops the temporary handler. With no active exception (context word 2) or an
 * empty chain it calls func_004103B0; otherwise it pops the top handler and longjmps into its buffer. Never returns.
 */
void sjthrow_410570(void) {
    struct N570 node;
    int *eh;
    int **pchain;
    int *q;
    int **pcl;
    struct N570 *h;
    eh = (int *)D_00453444[0]();
    pchain = (int **)&eh[1];
    q = (int *)eh[1];
    pcl = (int **)&q[1];
    if (q[1] != 0) {
        node.next = q;
        node.cleanup = 0;
        h = &node;
        if (__builtin_setjmp(node.jb) == 0) {
            *pchain = (int *)h;
                        {
                int *n;
                while (*pcl != 0) {
                    n = *pcl;
                    *pcl = (int *)n[0];
                    ((void (*)(int, int))n[1])(n[2], 2);
                }
            }
            *pchain = h->next;
        } else {
            func_004103B0();
        }
    }
    if (eh[2] == 0 || ((int *)*pchain)[0] == 0)
        func_004103B0();
    {
        int *r = *pchain;
        *pchain = (int *)r[0];
        __builtin_longjmp((void **)&r[2], 1);
    }
}
__asm__(".globl D_00410628\n\t.type D_00410628,@function\n\tD_00410628 = func_00410570 + 0xB8\n");
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_004106E8);
#ifdef SKIP_ASM
extern int (*D_00453444[])(void);
extern int func_00410570(void) __attribute__((noreturn));
struct N6E8 { int *next; int cleanup; void *jb[3]; char pad[0x62C]; };
/**
 * Runs the top handler's pending cleanups, pops that handler and throws on (func_00410570).
 *
 * Same cleanup loop as func_00410570 under a temporary handler (__builtin_setjmp), then removes the top element of the
 * dynamic handler chain and calls func_00410570. Never returns.
 *
 * @return never returns
 */
int func_004106E8(void) {
    struct N6E8 node;
    int *eh;
    int *q;
    int **pchain;
    int **pcl;
    struct N6E8 *h;
    eh = (int *)D_00453444[0]();
    pchain = (int **)&eh[1];
    q = (int *)eh[1];
    pcl = (int **)&q[1];
    if (q[1] != 0) {
        node.next = q;
        node.cleanup = 0;
        h = &node;
        __builtin_setjmp(node.jb);
        *pchain = (int *)h;
                {
            int *n;
            while (*pcl != 0) {
                n = *pcl;
                *pcl = (int *)n[0];
                ((void (*)(int, int))n[1])(n[2], 2);
            }
        }
        *pchain = h->next;
    }
    *pchain = (int *)(*pchain)[0];
    return func_00410570();
}
__asm__(".globl D_00410790\n\t.type D_00410790,@function\n\tD_00410790 = func_004106E8 + 0xA8\n");
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410800);
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
int func_00410800(int rtime) {
    int *info = *(int **)func_00410490();
    int (*fn)(int *, int, int) = (int (*)(int *, int, int))info[0];
    if (fn == 0)
        return 0;
    return fn(info, rtime, 0) != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410848);
#ifdef SKIP_ASM
/**
 * Returns the version field (the short at +6) of an exception table descriptor.
 *
 * @param table  the descriptor
 * @return the version
 */
int func_00410848(short *table) {
    return table[3];
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410850);
#ifdef SKIP_ASM
/**
 * Returns the language field (the short at +4) of an exception table descriptor.
 *
 * @param table  the descriptor
 * @return the language
 */
int func_00410850(short *table) {
    return table[2];
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410858);
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
int func_00410858(unsigned pc, struct E410858 *tab) {
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
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410930);
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
int func_00410930(unsigned pc, int obj, struct C410930 *ctx, int flag, int *out) {
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
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410A78);
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
int func_00410A78(int idx, struct S410A78 *udata, int z) {
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
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410B00);
#ifdef SKIP_ASM
extern int func_00410A78();
extern int func_0041605C(int, int, int);
/**
 * Copies a register's saved value from one frame's save slot to another's.
 *
 * Finds both slots with func_00410A78 and copies 16 bytes for registers below 31, 4 for 31..63 and 8 above
 * (func_0041605C).
 *
 * @param reg  the register number
 * @param udata  the frame to copy from
 * @param targetUdata  the frame to copy to
 * @return func_0041605C's result
 */
int func_00410B00(int reg, int udata, int targetUdata) {
    int x = func_00410A78(reg, udata, 0);
    int y = func_00410A78(reg, targetUdata, 0);
    int k;
    k = reg < 31 ? 16 : (reg <= 63 ? 4 : 8);
    return func_0041605C(y, x, k);
}
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410B80);
#ifdef SKIP_ASM
extern int *func_00414818();
extern long long *func_00410B80_idx() __asm__("func_00410A78");
/**
 * Computes the frame state of the caller of a frame.
 *
 * Gets the unwind information for @p framePc into @p callerBuf (func_00414818; 0 when there is none). The caller's CFA
 * is the value its CFA register was saved with in @p udata (func_00410A78) when saved, else @p udata's CFA, plus the
 * caller's CFA offset.
 *
 * @param framePc  an address inside the caller
 * @param udata  the current frame's state
 * @param callerBuf  the buffer for the caller's state
 * @return the caller's frame state, or 0
 */
int *func_00410B80(int framePc, int *udata, int callerBuf) {
    int *callerUdata = func_00414818(framePc, callerBuf);
    unsigned short i;
    if (callerUdata == 0)
        return 0;
    i = *(unsigned short *)((char *)callerUdata + 0x298);
    if (*((char *)udata + i + 0x29C) != 0)
        {
            long long cfaValue = *func_00410B80_idx(i, udata, 0);
            callerUdata[0] = (int)cfaValue;
        }
    else
        callerUdata[0] = udata[0];
    callerUdata[0] = callerUdata[0] + callerUdata[2];
    return callerUdata;
}
#endif

/**
 * Empty hook that func_00410C18 calls when only cleanups are left to run for a throw.
 *
 * A debugger can stop here to see the throw before the stack is unwound.
 */
void func_00410C10(void) {
}

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410C18);
#ifdef SKIP_ASM
struct S410C18 { char b[0x2F0]; } __attribute__((aligned(8)));
/* frame.h frame_state: cfa, eh_ptr, cfa_offset, args_size, reg_or_offset[], cfa_reg, retaddr_column (idx), saved[]
   (kind) */
struct F410C18 { int f0; unsigned *p; long cfa_offset; long args_size; long reg_or_offset[0x50]; unsigned short cfa_reg;
    unsigned short idx; signed char kind[0x50]; } __attribute__((aligned(8)));
extern void func_004103B0(void);
extern void func_00410C10(void);
extern struct F410C18 *f_b80_410C18(int, struct F410C18 *, struct S410C18 *) __asm__("func_00410B80");
extern long long *f_a78_410C18(int, struct F410C18 *, struct S410C18 *) __asm__("func_00410A78");
extern void f_b00_410C18(int, struct F410C18 *, struct S410C18 *) __asm__("func_00410B00");
extern int f_930_410C18(int, int, int, int, int *) __asm__("func_00410930");
extern int f_858_410C18(int, int) __asm__("func_00410858");
/* libgcc2.c: without INCOMING_REGNO (MIPS) no register lives in a register window */
static inline int in_reg_window_410C18(int reg, struct F410C18 *udata) { return 0; }
int unwind_410C18(int *info, int pc, struct S410C18 *buf, struct F410C18 **pout) __asm__("func_00410C18");
/**
 * Finds the handler for the exception being thrown and unwinds the saved registers to its frame.
 *
 * Walks the callers' frames from @p pc (func_00410B80). Each frame's table is searched: from the context's rethrow
 * entry (word 3, cleared afterwards) on a rethrow, as a new-format table (runtime id 0xFFFFFFFE, func_00410930) or an
 * old one (func_00410858). The first cleanup-only match is remembered and the walk goes on; a real handler stops it.
 * With only cleanups it uses the remembered one and calls func_00410C10; with nothing it calls func_004103B0
 * (terminate). The handler goes into the context's word 0.
 *
 * When the handler is not in the throwing frame it walks again from @p pc, copying every register each frame saved into
 * @p buf (func_00410B00), and at the end the return-address register when that was saved in another register. *@p pout
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
            func_00410C10();
    }
    if (result == 0)
        func_004103B0();
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
INCLUDE_ASM("seg/seg_3113A0_eh", func_00410F48);
#ifdef SKIP_ASM
struct S410F48 { char b[0x2F0]; } __attribute__((aligned(8)));
extern void func_004103B0(void);
extern struct S410F48 *get_410F48(void *, void *) __asm__("func_00414818");
extern void *func_00410C18(void *, int, void *, void *);
/**
 * Throws the current exception: finds its handler and jumps there with the registers restored.
 *
 * Gets the EH context from the getter D_00453444 and calls func_004103B0 when no exception is active (word 2 is 0).
 * Gets its own frame state (func_00414818; terminate when missing) with the CFA, copies it and passes its return
 * address - 1 to func_00410C18. __builtin_eh_return then restores the handler frame's registers, moves the stack by the
 * difference of the two CFAs and jumps to the handler. Never returns. The C++ throw helpers of seg_313800_exception
 * call it.
 */
void func_00410F48(void) {
    struct S410F48 ctx0;
    struct S410F48 buf;
    struct S410F48 *a;
    int *info;
    struct S410F48 *pb;
    void **pa;
    void *pc;
    __builtin_unwind_init();
    info = (int *)D_00453444[0]();
    pb = &buf;
    a = &ctx0;
    if (info[2] == 0) {
        func_004103B0();
        pa = (void **)&a;
    } else {
        pa = (void **)&a;
    }
lbl:
    pc = (void *)&&lbl;
    a = get_410F48(pc, (void *)a);
    if (a == 0)
        func_004103B0();
    *(void **)a = __builtin_dwarf_cfa();
    buf = *a;
    {
        void *h = func_00410C18(info, (int)__builtin_return_address(0) - 1, pb, pa);
        __builtin_eh_return(info, *(int *)a - *(int *)pb, h);
    }
}
__asm__(".globl D_004110B8\n\t.type D_004110B8,@function\n\tD_004110B8 = func_00410F48 + 0x170\n");
#endif

//100%
INCLUDE_ASM("seg/seg_3113A0_eh", func_00411128);
#ifdef SKIP_ASM
struct S411128 { char b[0x2F0]; } __attribute__((aligned(8)));
extern void func_004103B0(void);
extern struct S411128 *get_411128(void *, void *) __asm__("func_00414818");
extern void *func_00410C18(void *, int, void *, void *);
/**
 * Rethrows the current exception from a given exception-table entry.
 *
 * Like func_00410F48, but first stores @p val in the context's rethrow index (word 3), so func_00410C18 starts its
 * search after that region. Never returns.
 *
 * @param val  the table entry to rethrow out of
 */
void func_00411128(int val) {
    struct S411128 ctx0;
    struct S411128 buf;
    struct S411128 *a;
    int *info;
    struct S411128 *pb;
    void **pa;
    void *pc;
    __builtin_unwind_init();
    info = (int *)D_00453444[0]();
    pb = &buf;
    a = &ctx0;
    if (info[2] == 0)
        func_004103B0();
    info[3] = val;
    pa = (void **)&a;
lbl:
    pc = (void *)&&lbl;
    a = get_411128(pc, (void *)a);
    if (a == 0)
        func_004103B0();
    *(void **)a = __builtin_dwarf_cfa();
    buf = *a;
    {
        void *h = func_00410C18(info, (int)__builtin_return_address(0) - 1, pb, pa);
        __builtin_eh_return(info, *(int *)a - *(int *)pb, h);
    }
}
__asm__(".globl D_004112A0\n\t.type D_004112A0,@function\n\tD_004112A0 = func_00411128 + 0x178\n");
__asm__(".globl D_004111D4\n\t.type D_004111D4,@function\n\tD_004111D4 = func_00411128 + 0xAC\n");
#endif
