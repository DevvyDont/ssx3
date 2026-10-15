#include "kernel.h"

#include <SDL3/SDL.h>

#define EE_MAX_THREADS 16

struct ee_sema {
    int used;
    int count;
    int max;
    SDL_Condition *cond;
};

struct ee_vsync_handler {
    void (*fn)(void *);
    void *arg;
};

struct ee_thread_args {
    void (*fn)(void *);
    void *arg;
};

static SDL_Mutex *cpu;   /* held by whichever EE thread is running */
static SDL_Mutex *klock; /* protects the tables below; never held while taking cpu */
static struct ee_sema semas[EE_MAX_SEMAS];
static struct ee_vsync_handler vsync_handlers[EE_MAX_VSYNC_HANDLERS];
static int num_vsync_handlers;
static uint64_t vsync_counter;
static SDL_Condition *vsync_cond;
static int vsync_waiting;
static SDL_Condition *vsync_waiter_cond;
static SDL_Thread *threads[EE_MAX_THREADS];

int ee_kernel_init(void)
{
    cpu = SDL_CreateMutex();
    klock = SDL_CreateMutex();
    vsync_cond = SDL_CreateCondition();
    vsync_waiter_cond = SDL_CreateCondition();
    if (cpu == NULL || klock == NULL || vsync_cond == NULL || vsync_waiter_cond == NULL) {
        ee_kernel_shutdown();
        return -1;
    }
    return 0;
}

void ee_kernel_shutdown(void)
{
    int i;

    for (i = 0; i < EE_MAX_SEMAS; i++)
        ee_sema_delete(i);
    SDL_DestroyCondition(vsync_waiter_cond);
    SDL_DestroyCondition(vsync_cond);
    SDL_DestroyMutex(klock);
    SDL_DestroyMutex(cpu);
    vsync_cond = NULL;
    vsync_waiter_cond = NULL;
    klock = NULL;
    cpu = NULL;
    num_vsync_handlers = 0;
    vsync_counter = 0;
}

void ee_cpu_enter(void) { SDL_LockMutex(cpu); }
void ee_cpu_leave(void) { SDL_UnlockMutex(cpu); }

static int thread_main(void *data)
{
    struct ee_thread_args args = *(struct ee_thread_args *)data;

    SDL_free(data);
    ee_cpu_enter();
    args.fn(args.arg);
    ee_cpu_leave();
    return 0;
}

int ee_thread_start(void (*fn)(void *), void *arg, const char *name)
{
    struct ee_thread_args *args;
    int id;

    SDL_LockMutex(klock);
    for (id = 0; id < EE_MAX_THREADS && threads[id] != NULL; id++)
        ;
    if (id == EE_MAX_THREADS || (args = SDL_malloc(sizeof(*args))) == NULL) {
        SDL_UnlockMutex(klock);
        return -1;
    }
    args->fn = fn;
    args->arg = arg;
    threads[id] = SDL_CreateThread(thread_main, name, args);
    if (threads[id] == NULL) {
        SDL_free(args);
        id = -1;
    }
    SDL_UnlockMutex(klock);
    return id;
}

void ee_thread_join(int id)
{
    SDL_Thread *t;

    if (id < 0 || id >= EE_MAX_THREADS)
        return;
    SDL_LockMutex(klock);
    t = threads[id];
    threads[id] = NULL;
    SDL_UnlockMutex(klock);
    SDL_WaitThread(t, NULL);
}

static struct ee_sema *sema_get(int id)
{
    if (id < 0 || id >= EE_MAX_SEMAS || !semas[id].used)
        return NULL;
    return &semas[id];
}

int ee_sema_create(int initial, int max)
{
    int id;

    SDL_LockMutex(klock);
    for (id = 0; id < EE_MAX_SEMAS && semas[id].used; id++)
        ;
    if (id < EE_MAX_SEMAS) {
        semas[id].cond = SDL_CreateCondition();
        if (semas[id].cond != NULL) {
            semas[id].used = 1;
            semas[id].count = initial;
            semas[id].max = max;
        } else {
            id = EE_MAX_SEMAS;
        }
    }
    SDL_UnlockMutex(klock);
    return id < EE_MAX_SEMAS ? id : -1;
}

void ee_sema_delete(int id)
{
    struct ee_sema *s;

    SDL_LockMutex(klock);
    if ((s = sema_get(id)) != NULL) {
        SDL_DestroyCondition(s->cond);
        SDL_zerop(s);
    }
    SDL_UnlockMutex(klock);
}

int ee_sema_poll(int id)
{
    struct ee_sema *s;
    int ret = -1;

    SDL_LockMutex(klock);
    if ((s = sema_get(id)) != NULL && s->count > 0) {
        s->count--;
        ret = 0;
    }
    SDL_UnlockMutex(klock);
    return ret;
}

int ee_sema_wait(int id)
{
    struct ee_sema *s;

    if (ee_sema_poll(id) == 0)
        return 0;

    ee_cpu_leave();
    SDL_LockMutex(klock);
    while ((s = sema_get(id)) != NULL && s->count == 0)
        SDL_WaitCondition(s->cond, klock);
    if (s != NULL)
        s->count--;
    SDL_UnlockMutex(klock);
    ee_cpu_enter();
    return s != NULL ? 0 : -1;
}

int ee_sema_signal(int id)
{
    struct ee_sema *s;
    int ret = -1;

    SDL_LockMutex(klock);
    if ((s = sema_get(id)) != NULL && s->count < s->max) {
        s->count++;
        SDL_SignalCondition(s->cond);
        ret = 0;
    }
    SDL_UnlockMutex(klock);
    return ret;
}

int ee_vsync_add_handler(void (*handler)(void *), void *arg)
{
    int ret = -1;

    SDL_LockMutex(klock);
    if (num_vsync_handlers < EE_MAX_VSYNC_HANDLERS) {
        vsync_handlers[num_vsync_handlers].fn = handler;
        vsync_handlers[num_vsync_handlers].arg = arg;
        ret = num_vsync_handlers++;
    }
    SDL_UnlockMutex(klock);
    return ret;
}

void ee_vsync_raise(void)
{
    struct ee_vsync_handler handlers[EE_MAX_VSYNC_HANDLERS];
    int i, n;

    SDL_LockMutex(klock);
    n = num_vsync_handlers;
    SDL_memcpy(handlers, vsync_handlers, sizeof(handlers));
    SDL_UnlockMutex(klock);

    ee_cpu_enter();
    for (i = 0; i < n; i++)
        handlers[i].fn(handlers[i].arg);
    ee_cpu_leave();

    SDL_LockMutex(klock);
    vsync_counter++;
    vsync_waiting = 0; /* every current waiter is released by this VSync */
    SDL_BroadcastCondition(vsync_cond);
    SDL_UnlockMutex(klock);
}

void ee_vsync_wait(void)
{
    uint64_t target;

    ee_cpu_leave();
    SDL_LockMutex(klock);
    target = vsync_counter + 1;
    vsync_waiting++;
    SDL_BroadcastCondition(vsync_waiter_cond);
    while (vsync_counter < target)
        SDL_WaitCondition(vsync_cond, klock);
    SDL_UnlockMutex(klock);
    ee_cpu_enter();
}

void ee_vsync_wait_for_waiter(void)
{
    SDL_LockMutex(klock);
    while (vsync_waiting == 0)
        SDL_WaitCondition(vsync_waiter_cond, klock);
    SDL_UnlockMutex(klock);
}

uint64_t ee_vsync_count(void)
{
    uint64_t n;

    SDL_LockMutex(klock);
    n = vsync_counter;
    SDL_UnlockMutex(klock);
    return n;
}
