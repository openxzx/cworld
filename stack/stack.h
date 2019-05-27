#ifndef _STACK_H
#define _STACK_H

typedef struct stack_node {
	int data;
	struct stack_node *next;
} snode;

bool init_stack(snode *s);
bool push_stack(snode *s, int val);
int traverse_stack(snode *s);
bool pop_stack(snode *s, int *val);
bool clear_stack(snode *s);
int top_stack(snode s);
bool empty_stack(snode s);
int lenght_stack(snode s);

#endif
