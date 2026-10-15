#ifndef PLATFORM_SDLPAD_H
#define PLATFORM_SDLPAD_H

#include <SDL3/SDL.h>

#include "ds2pad.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Opens and closes gamepads as SDL reports them. */
void sdlpad_handle_event(const SDL_Event *event);
void sdlpad_close_all(void);

/* Reads the first open gamepad, falling back to the keyboard when none is
 * connected. */
void sdlpad_read(struct ds2_state *state);

#ifdef __cplusplus
}
#endif

#endif
