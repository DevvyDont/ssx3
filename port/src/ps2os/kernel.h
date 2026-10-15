/*
 * Minimal EE kernel services for the PC port: threads, semaphores and the
 * VSync interrupt.
 *
 * The game was written for one CPU. To keep that true on PC, EE code runs
 * only while holding the "CPU": every EE thread holds it while it runs and
 * gives it up only inside a blocking kernel call (a semaphore wait or a VSync
 * wait). Host threads (the window loop, audio, file IO) never touch game
 * memory without taking it first.
 *
 * Not modelled yet: thread priorities and the EE scheduler's rule that the
 * highest-priority ready thread always runs. When several EE threads are
 * ready, whichever wins the lock runs. The names here are deliberately not
 * the sce* ones; a compatibility layer maps those onto these once the
 * library calls in the ELF are identified.
 */
#ifndef PS2OS_KERNEL_H
#define PS2OS_KERNEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EE_MAX_SEMAS         64
#define EE_MAX_VSYNC_HANDLERS 8

int ee_kernel_init(void);
void ee_kernel_shutdown(void);

void ee_cpu_enter(void);
void ee_cpu_leave(void);

/* Starts an EE thread that runs fn(arg) while holding the CPU. Returns a
 * thread id, or -1. */
int ee_thread_start(void (*fn)(void *), void *arg, const char *name);
/* Waits for an EE thread to return. Call from a host thread, not holding the
 * CPU. */
void ee_thread_join(int id);

/* Returns a semaphore id, or -1 when the table is full. */
int ee_sema_create(int initial, int max);
void ee_sema_delete(int id);
/* Blocks (giving up the CPU) until the count is positive, then takes one. */
int ee_sema_wait(int id);
/* Takes one if the count is positive. Returns 0, or -1 if it would block. */
int ee_sema_poll(int id);
/* Returns -1 if the count is already at its maximum, as the EE kernel does. */
int ee_sema_signal(int id);

/* Handlers run with the CPU held, like an interrupt handler. */
int ee_vsync_add_handler(void (*handler)(void *), void *arg);
/* Called by the host frame loop once per frame. */
void ee_vsync_raise(void);
/* Called by EE code: blocks until the next VSync. */
void ee_vsync_wait(void);
/* Called by the host, not holding the CPU: blocks until some EE thread is
 * inside ee_vsync_wait. Running in lockstep this way makes headless runs
 * deterministic, where a real VSync would be missed by a slow frame. */
void ee_vsync_wait_for_waiter(void);
uint64_t ee_vsync_count(void);

#ifdef __cplusplus
}
#endif

#endif
