/*
 * Copyright: 2019.07.17
 *
 * Author: Zhenxing Xu <xzxlnmail@163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <semaphore.h>

#define COUNT	2

pthread_t r_thread[COUNT];
pthread_t w_thread[COUNT];

sem_t r_sem;
sem_t w_sem;

unsigned int data;
unsigned int readcount = 0;

struct rw_func {
	int (*init)(void);
	int (*run)(void);
	int (*stop)(void);
} rw_func_p;

void *(*reader[COUNT])(void *);
void *(*writer[COUNT])(void *);

void *reader1(void *arg)
{
	while (1) {
		sem_wait(&r_sem);
		if (!readcount)
			sem_wait(&w_sem);
		readcount++;
		sem_post(&r_sem);

		printf("\033[31mreader1 read data: %d.\033[0m\n", data);

		sem_wait(&r_sem);
		readcount--;
		if (!readcount)
			sem_post(&w_sem);
		sem_post(&r_sem);

		sleep(1);
	}
}

void *reader2(void *arg)
{
	while (1) {
		sem_wait(&r_sem);
		if (!readcount)
			sem_wait(&w_sem);
		readcount++;
		sem_post(&r_sem);

		printf("\033[31mreader2 read data: %d.\033[0m\n", data);

		sem_wait(&r_sem);
		readcount--;
		if (!readcount)
			sem_post(&w_sem);
		sem_post(&r_sem);

		sleep(1);
	}
}

void *writer1(void *arg)
{
	while (1) {
		sem_wait(&w_sem);
		data = 100;
		printf("\033[33mwriter1 write data: %d.\033[0m\n", data);
		sem_post(&w_sem);

		sleep(1);
	}
}

void *writer2(void *arg)
{
	while (1) {
		sem_wait(&w_sem);
		data = 300;
		printf("\033[33mwriter2 write data: %d.\033[0m\n", data);
		sem_post(&w_sem);

		sleep(1);
	}
}

int init(void)
{
	int i;

	/* Init semaphores */
	for (i = 0; i < COUNT; i++) {
		sem_init(&r_sem, 0, 1);
		sem_init(&w_sem, 0, 1);
	}

	reader[0] = reader1;
	reader[1] = reader2;
	/* Create reader threads */
	for (i = 0; i < COUNT; i++) {
		if (pthread_create(&r_thread[i], NULL, reader[i], (void *)NULL) != 0) {
			printf("\033[31mCreate reader%d thread failed!\033[0m\n", i);
			return 1;
		}
	}

	writer[0] = writer1;
	writer[1] = writer2;
	/* Create writer threads */
	for (i = 0; i < COUNT; i++) {
		if (pthread_create(&w_thread[i], NULL, writer[i], (void *)NULL) != 0) {
			printf("\033[31mCreate writer%d thread failed!\033[0m\n", i);
			return 1;
		}
	}

	return 0;
}

int run(void)
{
	int i;

	for (i = 0; i < COUNT; i++) {
		pthread_join(r_thread[i], NULL);
		pthread_join(w_thread[i], NULL);
	}

	return 0;
}

int stop(void)
{
	int i;

	for (i = 0; i < COUNT; i++) {
		sem_destroy(&r_sem);
		sem_destroy(&w_sem);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;

	rw_func_p.init = init;
	rw_func_p.run = run;
	rw_func_p.stop = stop;

	ret = rw_func_p.init();
	if (ret) {
		printf("\033[31mCreate threads failed, return: %d\033[0m\n", ret);
		return 0;
	}
	rw_func_p.run();
	rw_func_p.stop();

	return 0;
}
