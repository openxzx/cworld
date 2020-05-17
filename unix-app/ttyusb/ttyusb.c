#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

/* Speed array value */
int speed_arr[] = {
        B115200,
        B38400,
        B19200,
        B9600,
        B4800,
        B2400
};

int name_arr[] = {
        115200,
        38400,
        19200,
        9600,
        4800,
        2400
};

/* Uart communication protrol */
static char send_cmd[] = {
        0x00, 0x01, 0x00
};

int open_dev(const char *dev_name)
{
        return open(dev_name, O_RDWR);
}

int set_port(const int fd, int speed, int databits, int stopbits, int parity)
{
        struct termios opt;

        /* Getting device file attr */
        if (tcgetattr(fd, &opt) != 0)
                return -1;

        /* Setting speed */
        tcflush(fd, TCIFLUSH);
        switch (speed) {
        case 115200:
                cfsetispeed(&opt, B115200);
                cfsetospeed(&opt, B115200);
                break;
        default:
                cfsetispeed(&opt, B115200);
                cfsetospeed(&opt, B115200);
                break;
        }
        if (tcsetattr(fd, TCSANOW, &opt) != 0)
                return -1;

        opt.c_cflag &= ~CSIZE;

        /* Setting databit for 8bits */
        switch (databits) {
        case 7:
                opt.c_cflag |= CS7;
                break;
        case 8:
                opt.c_cflag |= CS8;
                break;
        default:
                opt.c_cflag |= CS8;
                break;
        }

        /* Setting parity */
        switch (parity) {
        case 'n':
        case 'N':
                opt.c_cflag &= ~PARENB;     /* Clear parity enable */
                opt.c_iflag &= ~INPCK;      /* Enable parity checking */
                break;
        case 'o':
        case 'O':
                opt.c_cflag |= (PARODD | PARENB);
                opt.c_iflag |= INPCK;       /* Disnable parity checking */
                break;
        case 'e':
        case 'E':
                opt.c_cflag |= PARENB;
                opt.c_cflag &= ~PARODD;
                opt.c_iflag |= INPCK;
                break;
        case 'S':
        case 's':
                opt.c_cflag &= ~PARENB;     /* Clear parity enable */
                opt.c_iflag &= ~INPCK;      /* Enable parity checking */
                break;
        default:
                opt.c_cflag &= ~PARENB;     /* Clear parity enable */
                opt.c_iflag &= ~INPCK;      /* Enable parity checking */
                break;
        }

        /* Setting stopbit */
        switch (stopbits) {
        case 1:
                opt.c_cflag &= ~CSTOPB;
                break;
        case 2:
                opt.c_cflag |= CSTOPB;
                break;
        default:
                opt.c_cflag &= ~CSTOPB;
                break;
        }

        tcflush(fd, TCIFLUSH);
        opt.c_cc[VTIME] = 150;
        opt.c_cc[VMIN] = 0;
        if (tcsetattr(fd, TCSANOW, &opt) != 0)
                return -1;

        return 0;
}

int send_data(const int fd, const char *buf, const int size)
{
        return write(fd, buf, size);
}

int read_data(const int fd, char *buf, const int size)
{
        return read(fd, buf, size);
}

int main(int argc, char *argv[])
{
        int i, n;
        int fd;
        char *dev_name = "/dev/ttyUSB0";
        char read_buf[32];
        int read_buf_size;

        /* Open devcice file */
        if ((fd = open_dev(dev_name)) == -1) {
                printf("Open %s failed, fd: %d.\n", dev_name, fd);
                return 0;
        }

        /* Setting serial parameters */
        if (set_port(fd, 115200, 8, 1, 'N') != 0) {
                printf("Setting Port failed.");
                return 0;
        }

        /* Sending command */
        n = sizeof(send_cmd) / sizeof(unsigned char);
        if (send_data(fd, send_cmd, n) != n) {
                printf("Sending data failed\n");
                close(fd);
                return 0;
        }
        printf("Sending datas finish.\n");
        read_buf_size = read_data(fd, read_buf, 12);
        printf("Reading datas: ");
        for (i = 0; i < read_buf_size; i++)
                printf("0x%x ", read_buf[i]);
        printf("\n");

        close(fd);
        return 0;
}
