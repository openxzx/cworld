/*
 * Author: Zhenxing Xu <xzxlnmail.163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 *
 * Checking the thread of the precess, command: ps -T -p PID, help message: info/man ps | less.
 *
 * 线程调度策略：
 * SCHED_NORMAL即为SCHED_OTHER：采用分时调度策略。
 * SCHED_FIFO：采用实时调度策略，且先到先服务，一直运行直到有更高优先级任务到达或自己放弃。
 * SCHED_RR：采用实时调度策略，且时间片轮转，时间片用完，系统将重新分配时间片，并置于就绪队列尾。
 * SCHED_BATCH：针对批处理进程。
 * SCHED_IDLE：使用此调度器的进程的优先级最低。在实现CFS时引入。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>

#define COUNT_MAX	10

int counter = 0;
unsigned int buf[COUNT_MAX];

/* Create pthread and mutex variables */
pthread_t producter_t, consumer_t;
pthread_mutex_t mutex;

pthread_attr_t p_attr;
struct sched_param p_param;

pthread_attr_t c_attr;
struct sched_param c_param;

void *producter(void *arg)
{
	static int in = 0;
	int tmp;
	struct sched_param param;

	prctl(PR_SET_NAME, "Producter");

	while (1) {
		pthread_attr_getschedparam(&p_attr, &param);
		printf("\033[34mproducter priority: %d.\033[0m\n", param.sched_priority);

		pthread_mutex_lock(&mutex);
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

		usleep(100);
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
		pthread_attr_getschedparam(&c_attr, &param);
		printf("\033[34mconsumer priority: %d.\033[0m\n", param.sched_priority);

		pthread_mutex_lock(&mutex);
		if (counter <= 0) {
			printf("\033[31mconsumer: counter is 0.\033[0m\n");
			pthread_mutex_unlock(&mutex);
			continue;
		}

		/*
		Dead waiting status:
		low priority thread get mutex and have a long running time,
		when high priority thread interrupt running to low priority thread, but it can't get mutex.
		*/
#if 0
		for (i = 0; i < 2^10; i++) {
			tmp = i * 1;
			tmp = 0;
		}
#endif

		tmp = buf[out];
		out = (out + 1) % COUNT_MAX;
		printf("\033[33mconsumer: counter: %d, out: %d, tmp: %d.\033[0m\n", counter, out, tmp);
		counter--;

		pthread_mutex_unlock(&mutex);

		usleep(100);
	}

	return NULL;
}

int create_thread(void)
{
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

int run_thread(void)
{
	/* Add sub threads into main thread */
	pthread_join(producter_t, NULL);
	pthread_join(consumer_t, NULL);

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;

	memset(buf, 0, sizeof(buf) / sizeof(unsigned int));

	pthread_mutex_init(&mutex, NULL);

	ret = create_thread();
	if (ret) {
		printf("\033[31mCreate thead failed, return: %d\033[0m\n", ret);
		return 0;
	}


	run_thread();

	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&p_attr);
	pthread_attr_destroy(&c_attr);

	return 0;
}
