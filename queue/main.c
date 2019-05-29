#include <stdio.h>
#include <stdbool.h>

#include "queue.h"

int main(int argc, char *argv[])
{
	int i, tmp;
	link_queue q;

	if (queue_init(&q) != true)
		return 0;

	for (i = 1; i <= 6; i++)
		queue_en(&q, i);
	printf("Queue length: %d\n", queue_len(&q));
	get_front(&q, &tmp);
	printf("Queue front: %d\n", tmp);

	queue_traverse(&q);
	queue_de(&q, &tmp);
	printf("Queue delete element: %d, length: %d\n", tmp, queue_len(&q));
	queue_traverse(&q);

	queue_clear(&q);
	queue_traverse(&q);
	printf("Queue length: %d\n", queue_len(&q));

	return 0;
}
