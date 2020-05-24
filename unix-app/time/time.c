/*
 * Author: Zhenxing Xu <xzxlnmail@163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 *
 * Struct timespec: first parameters
 * CLOCK_REALTIME: system current time, from 1970.01.01 date.
 * CLOCK_MONOTONIC: system booting time, it can't be set.
 * CLOCK_PROCESS_CPUTIME_ID: current process running time.
 * CLOCK_THREAD_CPUTIME_ID: this thread running time.
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    /* Struct timeval include tv_sec and tv_usec */
    struct timeval timeval_tmp;
    /* Struct timeval include tv_sec and tv_nsec */
    struct timespec timespec_tmp;

    gettimeofday(&timeval_tmp, NULL);
    printf("Second: %ld, Usecond: %ld\n", timeval_tmp.tv_sec, timeval_tmp.tv_usec);

    clock_gettime(CLOCK_REALTIME, &timespec_tmp);
    printf("Second: %ld, Nsecond: %ld\n", timespec_tmp.tv_sec, timespec_tmp.tv_nsec);

    return 0;
}
