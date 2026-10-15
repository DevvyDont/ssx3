#include "sdlpad.h"

static SDL_Gamepad *pad;

static const struct {
    SDL_GamepadButton sdl;
    enum ds2_button ds2;
} button_map[] = {
    { SDL_GAMEPAD_BUTTON_BACK, DS2_SELECT },
    { SDL_GAMEPAD_BUTTON_START, DS2_START },
    { SDL_GAMEPAD_BUTTON_LEFT_STICK, DS2_L3 },
    { SDL_GAMEPAD_BUTTON_RIGHT_STICK, DS2_R3 },
    { SDL_GAMEPAD_BUTTON_DPAD_UP, DS2_UP },
    { SDL_GAMEPAD_BUTTON_DPAD_RIGHT, DS2_RIGHT },
    { SDL_GAMEPAD_BUTTON_DPAD_DOWN, DS2_DOWN },
    { SDL_GAMEPAD_BUTTON_DPAD_LEFT, DS2_LEFT },
    { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, DS2_L1 },
    { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, DS2_R1 },
    { SDL_GAMEPAD_BUTTON_NORTH, DS2_TRIANGLE },
    { SDL_GAMEPAD_BUTTON_EAST, DS2_CIRCLE },
    { SDL_GAMEPAD_BUTTON_SOUTH, DS2_CROSS },
    { SDL_GAMEPAD_BUTTON_WEST, DS2_SQUARE },
};

/* Keyboard fallback: arrows steer, Z/X/C/V are the face buttons. */
static const struct {
    SDL_Scancode key;
    enum ds2_button ds2;
} key_map[] = {
    { SDL_SCANCODE_RSHIFT, DS2_SELECT },
    { SDL_SCANCODE_RETURN, DS2_START },
    { SDL_SCANCODE_W, DS2_UP },
    { SDL_SCANCODE_D, DS2_RIGHT },
    { SDL_SCANCODE_S, DS2_DOWN },
    { SDL_SCANCODE_A, DS2_LEFT },
    { SDL_SCANCODE_Q, DS2_L1 },
    { SDL_SCANCODE_E, DS2_R1 },
    { SDL_SCANCODE_1, DS2_L2 },
    { SDL_SCANCODE_3, DS2_R2 },
    { SDL_SCANCODE_V, DS2_TRIANGLE },
    { SDL_SCANCODE_X, DS2_CIRCLE },
    { SDL_SCANCODE_Z, DS2_CROSS },
    { SDL_SCANCODE_C, DS2_SQUARE },
};

void sdlpad_handle_event(const SDL_Event *event)
{
    if (event->type == SDL_EVENT_GAMEPAD_ADDED && pad == NULL) {
        pad = SDL_OpenGamepad(event->gdevice.which);
    } else if (event->type == SDL_EVENT_GAMEPAD_REMOVED && pad != NULL &&
               SDL_GetGamepadID(pad) == event->gdevice.which) {
        SDL_CloseGamepad(pad);
        pad = NULL;
    }
}

void sdlpad_close_all(void)
{
    if (pad != NULL)
        SDL_CloseGamepad(pad);
    pad = NULL;
}

/* SDL axes are -32768..32767; the DualShock 2 reports 0..255 with 128 at
 * rest. */
static uint8_t axis_to_byte(Sint16 v)
{
    return (uint8_t)((v + 32768) >> 8);
}

static uint8_t trigger_to_byte(Sint16 v)
{
    return v <= 0 ? 0 : (uint8_t)(v >> 7);
}

void sdlpad_read(struct ds2_state *state)
{
    size_t i;

    SDL_zerop(state);
    state->rx = state->ry = state->lx = state->ly = 128;

    if (pad != NULL) {
        for (i = 0; i < SDL_arraysize(button_map); i++) {
            if (SDL_GetGamepadButton(pad, button_map[i].sdl))
                state->pressure[button_map[i].ds2] = 255;
        }
        state->pressure[DS2_L2] = trigger_to_byte(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
        state->pressure[DS2_R2] = trigger_to_byte(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));
        state->lx = axis_to_byte(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTX));
        state->ly = axis_to_byte(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTY));
        state->rx = axis_to_byte(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHTX));
        state->ry = axis_to_byte(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHTY));
        return;
    }

    {
        const bool *keys = SDL_GetKeyboardState(NULL);

        for (i = 0; i < SDL_arraysize(key_map); i++) {
            if (keys[key_map[i].key])
                state->pressure[key_map[i].ds2] = 255;
        }
        if (keys[SDL_SCANCODE_LEFT])
            state->lx = 0;
        if (keys[SDL_SCANCODE_RIGHT])
            state->lx = 255;
        if (keys[SDL_SCANCODE_UP])
            state->ly = 0;
        if (keys[SDL_SCANCODE_DOWN])
            state->ly = 255;
    }
}
