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

#define COUNT	5

pthread_t thread_ph[COUNT];

sem_t sem_cs[COUNT];

/* function pointer struct data*/
struct chopstick {
	int (*init)(void);
	int (*run)(void);
	int (*stop)(void);
} cs_f;

/* function pointer array */
void *(*philosopher[COUNT])(void *);

void *philosopher0(void *arg)
{
	while (1) {
		sem_wait(&sem_cs[0]);
		sem_wait(&sem_cs[(0 + 1) % COUNT]);

		printf("\033[31mphilosopher0 eat foods.\033[0m\n");

		sem_post(&sem_cs[0]);
		sem_post(&sem_cs[(0 + 1) % COUNT]);

		usleep(1);
	}
}

void *philosopher1(void *arg)
{
	while (1) {
		sem_wait(&sem_cs[(1 + 1) % COUNT]);
		sem_wait(&sem_cs[1]);

		printf("\033[32mphilosopher1 eat foods.\033[0m\n");

		sem_post(&sem_cs[(1 + 1) % COUNT]);
		sem_post(&sem_cs[1]);

		usleep(1);
	}
}

void *philosopher2(void *arg)
{
	while (1) {
		sem_wait(&sem_cs[2]);
		sem_wait(&sem_cs[(2 + 1) % COUNT]);

		printf("\033[33mphilosopher2 eat foods.\033[0m\n");

		sem_post(&sem_cs[2]);
		sem_post(&sem_cs[(2 + 1) % COUNT]);

		usleep(1);
	}
}

void *philosopher3(void *arg)
{
	while (1) {
		sem_wait(&sem_cs[(3 + 1) % COUNT]);
		sem_wait(&sem_cs[3]);

		printf("\033[34mphilosopher3 eat foods.\033[0m\n");

		sem_post(&sem_cs[(3 + 1) % COUNT]);
		sem_post(&sem_cs[3]);

		usleep(1);
	}
}

void *philosopher4(void *arg)
{
	while (1) {
		sem_wait(&sem_cs[4]);
		sem_wait(&sem_cs[(4 + 1) % COUNT]);

		printf("\033[35mphilosopher4 eat foods.\033[0m\n");

		sem_post(&sem_cs[4]);
		sem_post(&sem_cs[(4 + 1) % COUNT]);

		usleep(1);
	}
}

int init(void)
{
	int i;

	/* Init semaphores */
	for (i = 0; i < COUNT; i++)
		sem_init(&sem_cs[i], 0, 1);

	philosopher[0] = philosopher0;
	philosopher[1] = philosopher1;
	philosopher[2] = philosopher2;
	philosopher[3] = philosopher3;
	philosopher[4] = philosopher4;
	/* Create threads */
	for (i = 0; i < COUNT; i++) {
		if (pthread_create(&thread_ph[i], NULL, philosopher[i], (void *)NULL) != 0) {
			printf("\033[31mCreate philosopher%d thread failed!\033[0m\n", i);
			return 1;
		}
	}

	return 0;
}

int run(void)
{
	int i;

	for (i = 0; i < COUNT; i++)
		pthread_join(thread_ph[i], NULL);

	return 0;
}

int stop(void)
{
	int i;

	for (i = 0; i < COUNT; i++)
		sem_destroy(&sem_cs[i]);

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;

	cs_f.init = init;
	cs_f.run = run;
	cs_f.stop = stop;

	ret = cs_f.init();
	if (ret) {
		printf("\033[31mCreate thead failed, return: %d\033[0m\n", ret);
		return 0;
	}
	cs_f.run();
	cs_f.stop();

	return 0;
}
