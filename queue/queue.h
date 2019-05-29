#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct q_node {
	int data;
	struct q_node *next;
} q_node;

typedef struct l_queue {
	q_node *front;
	q_node *rear;
} link_queue;

bool queue_init(link_queue *q);
int queue_empty(link_queue *q);
bool queue_insert(link_queue *q, int e);
bool queue_delete(link_queue *q, int *e);
void queue_clear(link_queue *q);
bool get_front(link_queue *q, int *e);
int queue_len(link_queue *q);
void queue_traverse(link_queue *q);

#endif
