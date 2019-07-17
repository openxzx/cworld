/*
 * Copyright: 2019.07
 *
 * Author: Zhenxing Xu
 *
 * Function pointer format: 返回值类型 ( * 指针变量名) ([形参列表]);
 * eg. int (*add)(int a, int b);
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

#define COUNT_MAX	10

int counter = 0;
unsigned int buf[COUNT_MAX];

pthread_t producter_t, consumer_t;

pthread_mutex_t mutex;

sem_t p_sem;
pthread_attr_t p_attr;
struct sched_param p_param;

sem_t c_sem;
pthread_attr_t c_attr;
struct sched_param c_param;

struct producter_consumer {
	int (*init)(void);
	int (*run)(void);
	int (*stop)(void);
} pc;

void *producter(void *arg)
{
	static int in = 0;
	int tmp;
	struct sched_param param;

	prctl(PR_SET_NAME, "Producter");

	while (1) {
		sem_wait(&p_sem);
		pthread_mutex_lock(&mutex);

		pthread_attr_getschedparam(&p_attr, &param);
		printf("\033[34mproducter priority: %d.\033[0m\n", param.sched_priority);

		if (counter >= COUNT_MAX) {
			pthread_mutex_unlock(&mutex);
			continue;
		}

		buf[in] = in;
		tmp = in;
		in = (in + 1) % COUNT_MAX;
		printf("\033[32mproducter: counter: %d, in: %d, tmp: %d.\033[0m\n", counter, in, buf[tmp]);
		counter++;

		pthread_mutex_unlock(&mutex);
		sem_post(&c_sem);

		sleep(1);
	}

	return NULL;
}

void *consumer(void *arg)
{
	int i;
	static int out = 0;
	int tmp;
	struct sched_param param;

	prctl(PR_SET_NAME, "Consumer");

	while (1) {
		sem_wait(&c_sem);
		pthread_mutex_lock(&mutex);

		pthread_attr_getschedparam(&c_attr, &param);
		printf("\033[34mconsumer priority: %d.\033[0m\n", param.sched_priority);

		if (counter <= 0) {
			printf("\033[31mconsumer: counter is 0.\033[0m\n");
			pthread_mutex_unlock(&mutex);
			continue;
		}

		tmp = buf[out];
		out = (out + 1) % COUNT_MAX;
		printf("\033[33mconsumer: counter: %d, out: %d, tmp: %d.\033[0m\n", counter, out, tmp);
		counter--;

		pthread_mutex_unlock(&mutex);
		sem_post(&p_sem);

		sleep(3);
	}

	return NULL;
}

int init(void)
{
	/* Init semaphores */
	sem_init(&p_sem, 0, COUNT_MAX);
	sem_init(&c_sem, 0, 0);

	/* Init mutex */
	pthread_mutex_init(&mutex, NULL);

	/* Create producter thread */
	pthread_attr_init(&p_attr);
	pthread_attr_setschedpolicy(&p_attr, SCHED_FIFO);
	p_param.sched_priority = 60;
	pthread_attr_setschedparam(&p_attr, &p_param);
	if (pthread_create(&producter_t, &p_attr, producter, (void *)NULL) != 0) {
		printf("\033[31mCreate Producter thread failed!\033[0m\n");
		return 1;
	}

	/* Create consumer thread */
	pthread_attr_init(&c_attr);
	pthread_attr_setschedpolicy(&c_attr, SCHED_FIFO);
	c_param.sched_priority = 30;
	pthread_attr_setschedparam(&c_attr, &c_param);
	if (pthread_create(&consumer_t, &c_attr, consumer, (void *)NULL) != 0) {
		printf("\033[31mCreate Consumer thread failed!\033[0m\n");
		return 2;
	}

	return 0;
}

int run(void)
{
	pthread_join(producter_t, NULL);
	pthread_join(consumer_t, NULL);

	return 0;
}

int stop(void)
{
	sem_destroy(&p_sem);
	sem_destroy(&c_sem);

	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&p_attr);
	pthread_attr_destroy(&c_attr);

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;

	/* Init producter, consumer struct */
	pc.init = init;
	pc.run = run;
	pc.stop = stop;

	/* Run threads */
	ret = pc.init();
	if (ret) {
		printf("\033[31mCreate thead failed, return: %d\033[0m\n", ret);
		return 0;
	}
	pc.run();
	pc.stop();

	return 0;
}
