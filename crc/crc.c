/*
 * Online crc calc: https://crccalc.com/
 */

#include <stdio.h>

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

/* CRC16 Xmodem */
uint16_t crc16_xmodem(const uint8_t * data, unsigned size)
{
    unsigned int i, j;
    const uint16_t crc_poly = 0x1021;
    const uint16_t init_val = 0x0000;
    uint16_t x = init_val;

    if (data == NULL)
        return 0;

    for (i = 0; i < size; ++i) {
        x ^= (uint16_t)data[i] << 8;
        for (j = 0; j < 8; ++j)
            x = (x&0x8000) ? (x<<1)^crc_poly : (x<<1);
    }

    return x;
}

int main(void)
{
    uint8_t i;
    uint8_t buf[4];

    for (i = 0; i < sizeof(buf); i++)
        buf[i] = i;

    printf("Crc16 Xmodem: 0x%x\n", crc16_xmodem(buf, sizeof(buf)));

    return 0;
}
