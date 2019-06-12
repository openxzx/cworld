#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define COUNT_MAX	10

int counter = 0;
unsigned int buf[COUNT_MAX];

/* Create pthread and mutex variables */
pthread_t producter_t, consumer_t;
pthread_mutex_t mutex;

void *producter(void *arg)
{
	static int in = 0;
	int tmp;

	while (1) {
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
		sleep(1);
	}

	return NULL;
}

void *consumer(void *arg)
{
	static int out = 0;
	int tmp;

	while (1) {
		pthread_mutex_lock(&mutex);

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
		sleep(1);
	}

	return NULL;
}

int create_thread(void)
{
	/* Create threads */
	if (pthread_create(&producter_t, NULL, producter, (void *)NULL) != 0) {
		printf("\033[31mCreate Producter thread failed!\033[0m\n");
		return 1;
	}

	if (pthread_create(&consumer_t, NULL, consumer, (void *)NULL) != 0) {
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

	return 0;
}
