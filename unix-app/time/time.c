#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    /* Struct timeval include tv_sec and tv_usec */
    struct timeval current_time;

    gettimeofday(&current_time, NULL);

    printf("Second: %ld, Usecond: %ld\n", current_time.tv_sec, current_time.tv_usec);

    return 0;
}
