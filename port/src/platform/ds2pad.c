#include "ds2pad.h"

/* A button counts as pressed in the digital bits once its pressure passes
 * this, so a lightly touched analog trigger doesn't register as a press. */
#define DIGITAL_THRESHOLD 64

/* Packet bytes 8..19 in the order the hardware sends them. */
static const enum ds2_button pressure_order[12] = {
    DS2_RIGHT, DS2_LEFT, DS2_UP, DS2_DOWN,
    DS2_TRIANGLE, DS2_CIRCLE, DS2_CROSS, DS2_SQUARE,
    DS2_L1, DS2_R1, DS2_L2, DS2_R2,
};

void ds2_build_packet(const struct ds2_state *state, uint8_t out[DS2_PACKET_SIZE])
{
    uint16_t buttons = 0xFFFF;
    int i;

    for (i = 0; i < DS2_NUM_BUTTONS; i++) {
        if (state->pressure[i] >= DIGITAL_THRESHOLD)
            buttons &= (uint16_t)~(1u << i);
    }

    out[0] = 0x00; /* status: ok */
    out[1] = 0x79; /* DualShock 2, analog with pressure, 9 halfwords */
    out[2] = (uint8_t)(buttons & 0xFF);
    out[3] = (uint8_t)(buttons >> 8);
    out[4] = state->rx;
    out[5] = state->ry;
    out[6] = state->lx;
    out[7] = state->ly;
    for (i = 0; i < 12; i++)
        out[8 + i] = state->pressure[pressure_order[i]];
}
