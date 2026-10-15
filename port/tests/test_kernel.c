#include <SDL3/SDL.h>

#include "check.h"
#include "ps2os/kernel.h"

/* Counted only with the CPU held: if two EE threads ever ran at once, the
 * unsynchronised increments below would lose updates. */
static volatile int shared_counter;
static int sema_ping, sema_pong;
static volatile int vsync_hits;
static volatile int frames_seen;

#define ROUNDS 2000

static void ping(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < ROUNDS; i++) {
        int v = shared_counter;
        shared_counter = v + 1;
        ee_sema_signal(sema_pong);
        ee_sema_wait(sema_ping);
    }
}

static void pong(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < ROUNDS; i++) {
        ee_sema_wait(sema_pong);
        int v = shared_counter;
        shared_counter = v + 1;
        ee_sema_signal(sema_ping);
    }
}

static void on_vsync(void *arg)
{
    (void)arg;
    vsync_hits++;
}

static void wait_frames(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 10; i++) {
        ee_vsync_wait();
        frames_seen++;
    }
}

static void test_semas(void)
{
    int s = ee_sema_create(1, 1);

    CHECK(s >= 0);
    CHECK(ee_sema_signal(s) == -1); /* already at max */
    CHECK(ee_sema_poll(s) == 0);
    CHECK(ee_sema_poll(s) == -1);
    CHECK(ee_sema_signal(s) == 0);
    ee_sema_delete(s);
    CHECK(ee_sema_poll(s) == -1);
}

static void test_threads(void)
{
    int a, b;

    sema_ping = ee_sema_create(0, 1);
    sema_pong = ee_sema_create(0, 1);
    a = ee_thread_start(ping, NULL, "ping");
    b = ee_thread_start(pong, NULL, "pong");
    CHECK(a >= 0 && b >= 0);
    ee_thread_join(a);
    ee_thread_join(b);
    CHECK(shared_counter == 2 * ROUNDS);
}

static void test_vsync(void)
{
    int t;
    uint64_t start = ee_vsync_count();

    CHECK(ee_vsync_add_handler(on_vsync, NULL) >= 0);
    t = ee_thread_start(wait_frames, NULL, "vsync");
    /* In lockstep every raise is seen, so exactly ten are needed. */
    for (int i = 0; i < 10; i++) {
        ee_vsync_wait_for_waiter();
        ee_vsync_raise();
    }
    ee_thread_join(t);
    CHECK(frames_seen == 10);
    CHECK(vsync_hits == 10);
    CHECK(ee_vsync_count() - start == 10);
}

int main(void)
{
    if (!SDL_Init(0) || ee_kernel_init() != 0) {
        fprintf(stderr, "init failed\n");
        return 1;
    }
    test_semas();
    test_threads();
    test_vsync();
    ee_kernel_shutdown();
    SDL_Quit();
    return CHECK_DONE();
}
