#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>

#include "queue.h"

bool queue_init(link_queue *q)
{
	q_node *new_node = (q_node *)malloc(sizeof(q_node));
	if (new_node == NULL) {
		printf("Queue init, malloc queue node failed.\n");
		return false;
	}

	new_node->next = NULL;
	q->front = q->rear = new_node;

	return true;
}

int queue_empty(link_queue *q)
{
	return q->front == q->rear;
}

bool queue_en(link_queue *q, int e)
{
	q_node *new_node = (q_node *)malloc(sizeof(q_node));
	if (!new_node) {
		printf("Queue insert, malloc queue node failed.\n");
		return false;
	}

	new_node->data = e;
	new_node->next = NULL;
	q->rear->next = new_node;
	q->rear = new_node;

	return true;
}

bool queue_de(link_queue *q, int *e)
{
	if (q->front == q->rear) {
		printf("Queue delete: queue is NULL.\n");
		return false;
	}

	*e = q->front->next->data;
	q_node *tmp = q->front->next;
	q->front->next = tmp->next;
	if (tmp == q->rear)
		q->rear = q->front;
	free(tmp);
	tmp = NULL;

	return true;
}

void queue_clear(link_queue *q)
{
	q_node *head = q->front->next;
	head->next = NULL;
	q->front = q->rear = head;

	q_node *tmp = head->next;
	while(tmp) {
		q_node *p = tmp;
		tmp = p->next;
		free(p);
		p = NULL;
	}
}

bool get_front(link_queue *q, int *e)
{
	if (q->front == q->rear) {
		printf("Get queue front, queue is NULL.\n");
		return false;
	}

	*e = q->front->next->data;

	return true;
}

int queue_len(link_queue *q)
{
	int cnt = 0;
	q_node *p = q->front->next;

	while (p) {
		cnt += 1;
		p = p->next;
	}

	return cnt;
}

void queue_traverse(link_queue *q)
{
	q_node *p = q->front->next;
	if (!p)
		printf("Queue traverse: Queue is NULL.\n");

	while (p) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}
