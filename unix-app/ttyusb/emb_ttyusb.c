/*
 * Description: CPU and STM32 connect by UART
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define UART_READ_TIMEOUT    1   /* Unit: Hundred milliseconds */

/* Big and little endian transfer */
const int endian = 1;
#define is_bigendian() ((*(char*)&endian) == 0)

/* Transfer for little endian 16bits value */
unsigned short htoles(unsigned short v16)
{
    unsigned short tmp = v16;

    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v16);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[1];
        d[1] = s[0];
    }

    return tmp;
}

/* Transfer for little endian 32bits value */
unsigned int htolel(unsigned int v32)
{
    unsigned int tmp = v32;

    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v32);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }

    return tmp;
}

/*
 * CRC16 XModem
 */
unsigned short crc_check(const unsigned char * data, unsigned int size)
{
    unsigned int i, j;
    const unsigned short crc_poly = 0x1021;
    const unsigned short init_val = 0x0000;
    unsigned short ret = init_val;

    if (data == NULL)
        return 0;

    for (i = 0; i < size; ++i) {
        ret ^= (unsigned short)data[i] << 8;
        for (j = 0; j < 8; ++j)
            ret = (ret&0x8000) ? (ret<<1)^crc_poly : (ret<<1);
    }

    return ret;
}

int tty_open(char *dev_name, void **uart_target_ptr, uart_baudrate_t baudrate)
{
    int dev;
    int *uart_device = NULL;
    struct termios opt;

    CHECK_NULL(dev_name);
    CHECK_NULL(uart_target_ptr);

    /* Allocate memory for the device descriptor */
    /* When handling failed, we must free memory */
    uart_device = malloc(sizeof(int));
    if (uart_device == NULL) {
        printf("ERROR: Malloc failed.\n");
        return -1;
    }

    /* Open device */
    dev = open(dev_name, O_RDWR | O_NOCTTY);
    if (dev <= 0) {
        free(uart_device);
        printf("ERROR: Open failed.\n");
        return -2;
    }

    /* Checking bautrate */
    if ((baudrate < UB115200) || (baudrate > UB921600)) {
        free(uart_device);
        close(dev);
        printf("ERROR: Bautrate value failed.\n");
        return -3;
    }

    /* Getting device file attr */
    if (tcgetattr(dev, &opt) != 0) {
        free(uart_device);
        close(dev);
        printf("ERROR: Get device attr failed.\n");
        return -4;
    }

    /* Setting speed */
    switch (baudrate) {
    case UB115200:
        cfsetispeed(&opt, B115200);
        cfsetospeed(&opt, B115200);
        break;
    case UB230400:
        cfsetispeed(&opt, B230400);
        cfsetospeed(&opt, B230400);
        break;
    case UB460800:
        cfsetispeed(&opt, B460800);
        cfsetospeed(&opt, B460800);
        break;
    case UB921600:
        cfsetispeed(&opt, B921600);
        cfsetospeed(&opt, B921600);
        break;
    default:
        break;
    }

    /* Control Modes */
    opt.c_cflag |= CLOCAL;  /* Local connection, no modem control */
    opt.c_cflag |= CREAD;   /* Enable receiving characters */
    opt.c_cflag |= CS8;     /* 8 bit frames */
    opt.c_cflag &= ~PARENB; /* No parity */
    opt.c_cflag &= ~CSTOPB; /* One stop bit */

    /* Input Modes */
    opt.c_iflag |= IGNPAR;  /* Ignore bytes with parity errors */
    opt.c_iflag &= ~ICRNL;  /* Do not map CR to NL on input*/
    opt.c_iflag &= ~IGNCR;  /* Do not ignore carriage return on input */
    opt.c_iflag &= ~IXON;   /* Disable Start/Stop output control */
    opt.c_iflag &= ~IXOFF;  /* Do not send Start/Stop characters */

    /* Output Modes */
    opt.c_oflag = 0;        /* Disable everything on output as we only write binary */

    /* Local Modes */
    opt.c_lflag &= ~ICANON; /* Disable canonical input - cannot use with binary input */
    opt.c_lflag &= ~ISIG;   /* Disable check for INTR, QUIT, SUSP special characters */
    opt.c_lflag &= ~IEXTEN; /* Disable any special control character */
    opt.c_lflag &= ~ECHO;   /* Do not echo back every character typed */
    opt.c_lflag &= ~ECHOE;  /* Does not erase the last character in current line */
    opt.c_lflag &= ~ECHOK;  /* Do not echo NL after KILL character */

    opt.c_cc[VTIME] = UART_READ_TIMEOUT; /* Unit: Hundred milliseconds */
    opt.c_cc[VMIN] = 0;

   /* TCSANOW for setting attr is valid at once */
    if (tcsetattr(dev, TCSANOW, &opt) != 0) {
        free(uart_device);
        close(dev);
        printf("ERROR: Set device datasize/parity/stop attr failed.\n");
        return -5;
    }

    /* We must clear input/output queue by tcflush */
    tcflush(dev, TCIFLUSH);

    *uart_device = dev;
    *uart_target_ptr = (void *)uart_device;

    return 0;
}

int tty_close(void *uart_target)
{
    int ret;
    int uart_device;

    CHECK_NULL(uart_target);
    uart_device = *(int *)uart_target;

    ret = close(uart_device);
    free(uart_target);

    if (ret < 0) {
        printf("ERROR: Close failed.\n");
        return -1;
    } else {
        printf("Note: Uart port closed.\n");
        return 0;
    }
}

int tty_write(void *uart_target, unsigned char *buf, int size)
{
    int uart_device;

    CHECK_NULL(buf);
    CHECK_NULL(uart_target);
    uart_device = *(int *)uart_target;

    return write(uart_device, buf, size);
}

int tty_read(void *uart_target, unsigned char *buf, int size)
{
    int uart_device;

    CHECK_NULL(buf);
    CHECK_NULL(uart_target);
    uart_device = *(int *)uart_target;

    return read(uart_device, buf, size);
}
