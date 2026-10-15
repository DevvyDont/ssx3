/*
 * PC host for the SSX 3 port.
 *
 * Today this only brings up the platform layers: it maps the EE address
 * space, optionally loads SLUS_207.72 into it, starts an EE thread and drives
 * it with a 60 Hz VSync and controller packets. The game itself is not linked
 * in yet; that is milestone M1 in the port plan.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "platform/ds2pad.h"
#include "platform/sdlpad.h"
#include "ps2os/eemem.h"
#include "ps2os/kernel.h"

#define FRAME_NS (SDL_NS_PER_SECOND / 60)

struct options {
    int headless;
    long frames; /* 0 = run until the window closes */
    const char *elf_path;
};

/* Written by the host with the CPU held, read by EE code. */
static uint8_t pad_packet[DS2_PACKET_SIZE];
static volatile int ee_quit;
static uint64_t ee_frames;

static void ee_main(void *arg)
{
    uint8_t last[DS2_PACKET_SIZE] = { 0 };

    (void)arg;
    while (!ee_quit) {
        ee_vsync_wait();
        if (ee_quit)
            break;
        if (memcmp(last, pad_packet, sizeof(last)) != 0) {
            memcpy(last, pad_packet, sizeof(last));
            SDL_Log("pad: buttons %02X%02X  left %3u,%3u  right %3u,%3u",
                    last[3], last[2], last[6], last[7], last[4], last[5]);
        }
        ee_frames++;
    }
}

static int parse_args(int argc, char **argv, struct options *opt)
{
    int i;

    memset(opt, 0, sizeof(*opt));
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--headless") == 0) {
            opt->headless = 1;
        } else if (strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
            opt->frames = strtol(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--elf") == 0 && i + 1 < argc) {
            opt->elf_path = argv[++i];
        } else {
            fprintf(stderr, "usage: %s [--headless] [--frames N] [--elf SLUS_207.72]\n", argv[0]);
            return -1;
        }
    }
    if (opt->headless && opt->frames <= 0) {
        fprintf(stderr, "--headless needs --frames\n");
        return -1;
    }
    return 0;
}

static int load_elf(const char *path)
{
    size_t size;
    void *image = SDL_LoadFile(path, &size);
    uint32_t entry;
    int ret;

    if (image == NULL) {
        SDL_Log("cannot read %s: %s", path, SDL_GetError());
        return -1;
    }
    ret = ee_load_elf(image, size, &entry);
    SDL_free(image);
    if (ret != 0) {
        SDL_Log("%s is not a loadable EE executable", path);
        return -1;
    }
    SDL_Log("loaded %s, entry point 0x%08X", path, entry);
    return 0;
}

int main(int argc, char **argv)
{
    struct options opt;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Uint64 next_frame;
    long frame = 0;
    int running = 1, ee_thread, ret = 1;

    if (parse_args(argc, argv, &opt) != 0)
        return 2;

    /* Map EE memory before SDL starts threads and allocates, so nothing else
     * can land on the addresses first. */
    if (ee_mem_init() != 0) {
        fprintf(stderr, "cannot map EE memory at its PS2 addresses\n");
        return 1;
    }
    if (!SDL_Init(opt.headless ? SDL_INIT_EVENTS : SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        goto out_mem;
    }
    if (ee_kernel_init() != 0)
        goto out_sdl;
    if (opt.elf_path != NULL && load_elf(opt.elf_path) != 0)
        goto out_kernel;

    if (!opt.headless &&
        !SDL_CreateWindowAndRenderer("SSX 3", 640, 448, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("cannot open a window: %s", SDL_GetError());
        goto out_kernel;
    }

    ee_thread = ee_thread_start(ee_main, NULL, "ee-main");
    if (ee_thread < 0)
        goto out_window;

    next_frame = SDL_GetTicksNS();
    while (running) {
        SDL_Event ev;
        struct ds2_state pad;

        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT)
                running = 0;
            sdlpad_handle_event(&ev);
        }

        if (!opt.headless) {
            sdlpad_read(&pad);
        } else {
            SDL_zero(pad);
            pad.rx = pad.ry = pad.lx = pad.ly = 128;
        }
        ee_cpu_enter();
        ds2_build_packet(&pad, pad_packet);
        ee_cpu_leave();

        if (renderer != NULL) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }

        if (opt.headless)
            ee_vsync_wait_for_waiter();
        ee_vsync_raise();
        if (opt.frames > 0 && ++frame >= opt.frames)
            running = 0;

        if (!opt.headless) {
            next_frame += FRAME_NS;
            Uint64 now = SDL_GetTicksNS();
            if (next_frame > now)
                SDL_DelayPrecise(next_frame - now);
            else
                next_frame = now;
        }
    }

    ee_vsync_wait_for_waiter();
    ee_cpu_enter();
    ee_quit = 1;
    ee_cpu_leave();
    ee_vsync_raise(); /* wake the EE thread so it sees ee_quit */
    ee_thread_join(ee_thread);
    SDL_Log("ran %ld host frames, EE thread saw %llu", frame, (unsigned long long)ee_frames);
    /* Headless runs in lockstep, so the EE thread must see every frame. */
    ret = opt.headless && ee_frames != (uint64_t)frame;

out_window:
    sdlpad_close_all();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
out_kernel:
    ee_kernel_shutdown();
out_sdl:
    SDL_Quit();
out_mem:
    ee_mem_shutdown();
    return ret;
}
