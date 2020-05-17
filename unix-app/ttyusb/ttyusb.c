#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

static unsigned char send_cmd[] = {
        0x00, 0x01, 0x00
};

int open_dev(const char *dev_name);
int set_port(const int fd);
int send_data(const int fd, const char *buf, const int size);
int read_data(const int fd, char *buf, const int size);

int open_dev(const char *dev_name)
{
        return open(dev_name, O_RDWR);
}

int set_port(const int fd)
{
        struct termios opt;

        /* Setting bandrate */
        if(tcgetattr(fd, &opt) != 0)
                return -1;

        cfsetispeed(&opt, 115200);
        cfsetospeed(&opt, 115200);
        tcsetattr(fd, TCSANOW, &opt);

        /* Setting databit for 8bits */
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS8;

        /* Setting parity */
        opt.c_cflag &= ~PARENB;
        opt.c_iflag &= ~INPCK;

        /* Setting stopbit */
        opt.c_cflag &= ~CSTOPB;

        tcflush(fd, TCIFLUSH);
        opt.c_cc[VTIME] = 1;
        opt.c_cc[VMIN] = 0;

        if(tcsetattr(fd, TCSANOW, &opt) != 0)
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
        int i;
        int fd;
        char *dev_name = "/dev/ttyUSB0";
        char read_buf[320];
        int read_buf_size;

        /* Open devcice file */
        if((fd = open_dev(dev_name)) == -1) {
                printf("Open %s failed, fd: %d.\n", dev_name, fd);
                return 0;
        }

        /* Setting serial parameters */
        if(set_port(fd) != 0) {
                perror("Setting Port failed.");
                return 0;
        }

        /* Sending command */
        send_data(fd, &send_cmd[0], sizeof(send_cmd[0]));

        read_buf_size = read_data(fd, read_buf, 12);
        printf("read: %s\nsize: %d\n", read_buf, read_buf_size);

        close(fd);
        return 0;
}
