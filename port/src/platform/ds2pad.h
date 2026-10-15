/*
 * Builds the DualShock 2 packet that scePadRead returns, from host input.
 *
 * The port replaces only the device read. The game's own conversion
 * (0x00326EB0, see docs/notes/subsystems/input.md) still turns the packet into
 * its 24 input channels, so dead zones and pressure scaling stay the game's.
 */
#ifndef PLATFORM_DS2PAD_H
#define PLATFORM_DS2PAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ds2_button {
    DS2_SELECT, DS2_L3, DS2_R3, DS2_START,
    DS2_UP, DS2_RIGHT, DS2_DOWN, DS2_LEFT,
    DS2_L2, DS2_R2, DS2_L1, DS2_R1,
    DS2_TRIANGLE, DS2_CIRCLE, DS2_CROSS, DS2_SQUARE,
    DS2_NUM_BUTTONS
};

struct ds2_state {
    /* 0 = released, 255 = fully pressed. Digital host buttons use 0 or 255;
     * analog triggers can use the whole range. */
    uint8_t pressure[DS2_NUM_BUTTONS];
    /* 0 = full left/up, 128 = centre, 255 = full right/down. */
    uint8_t rx, ry, lx, ly;
};

#define DS2_PACKET_SIZE 20

/* Writes a DualShock 2 analog-mode packet with pressure bytes:
 * byte 0 status, 1 mode (0x79), 2..3 buttons active low, 4..7 sticks
 * (right X, right Y, left X, left Y), 8..19 pressure. */
void ds2_build_packet(const struct ds2_state *state, uint8_t out[DS2_PACKET_SIZE]);

#ifdef __cplusplus
}
#endif

#endif
