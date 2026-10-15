#include <string.h>

#include "check.h"
#include "platform/ds2pad.h"

int main(void)
{
    struct ds2_state s;
    uint8_t p[DS2_PACKET_SIZE];

    memset(&s, 0, sizeof(s));
    s.rx = s.ry = s.lx = s.ly = 128;
    ds2_build_packet(&s, p);
    CHECK(p[0] == 0x00 && p[1] == 0x79);
    CHECK(p[2] == 0xFF && p[3] == 0xFF); /* active low: nothing pressed */
    CHECK(p[4] == 128 && p[7] == 128);

    /* Start is bit 3 of byte 2; Cross is bit 6 of byte 3. */
    s.pressure[DS2_START] = 255;
    s.pressure[DS2_CROSS] = 255;
    s.lx = 0;
    s.ry = 255;
    ds2_build_packet(&s, p);
    CHECK(p[2] == (uint8_t)~0x08);
    CHECK(p[3] == (uint8_t)~0x40);
    CHECK(p[6] == 0 && p[5] == 255);
    CHECK(p[8 + 6] == 255); /* Cross is the seventh pressure byte */

    /* A light analog press shows in pressure but not in the digital bits. */
    memset(s.pressure, 0, sizeof(s.pressure));
    s.pressure[DS2_R2] = 40;
    ds2_build_packet(&s, p);
    CHECK(p[3] == 0xFF);
    CHECK(p[19] == 40); /* R2 is the last pressure byte */

    s.pressure[DS2_R2] = 200;
    ds2_build_packet(&s, p);
    CHECK(p[3] == (uint8_t)~0x02);
    return CHECK_DONE();
}
