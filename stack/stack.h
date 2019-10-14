#ifndef _STACK_H
#define _STACK_H

typedef struct node {
	int data;
	struct node *next;
} node;

typedef struct snode {
	struct node *base;
	struct node *top;
} snode;

bool init_stack(snode *s);
bool push_stack(snode *s, int val);
int traverse_stack(snode *s);
int top_stack(snode *s);
bool empty_stack(snode *s);
int lenght_stack(snode *s);
bool pop_stack(snode *s, int *val);
bool clear_stack(snode *s);

#endif
