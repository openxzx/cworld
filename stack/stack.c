#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

bool init_stack(snode *s)
{
	node *head_node = (node *)malloc(sizeof(node));
	if (head_node == NULL)
		return false;

	head_node->data = 0;
	head_node->next = NULL;

	s->base = head_node;
	s->top = head_node;

	return true;
}

bool push_stack(snode *s, int val)
{
	node *p = (node *)malloc(sizeof(node));
	if (p == NULL)
		return false;

	p->data = val;
	p->next = s->top->next;
	s->top->next = p;
	s->top = p;

	return true;
}

int traverse_stack(snode *s)
{
	if (s->base == s->top) {
		printf("Stack is NULL.\n");
		return 0;
	}

	node *tmp = s->base->next;
	while (tmp->next != NULL) {
		printf("%d ", tmp->data);
		tmp = tmp->next;
	}
	printf("%d \n", tmp->data);

	return 0;
}

int top_stack(snode *s)
{
	return s->top->data;
}

bool empty_stack(snode *s)
{
	if (s->base == s->top)
		return true;
	else
		return false;
}

int lenght_stack(snode *s)
{
	int cnt = 0;
	node *tmp = s->base->next;

	while (tmp->next != NULL) {
		tmp = tmp->next;
		cnt++;
	}
	cnt++;

	return cnt;
}

bool pop_stack(snode *s, int *val)
{
	if (s->base == s->top)
		return true;

	node *tmp1 = s->base;
	node *tmp2 = s->top;

	while (tmp1->next != s->top) {
		tmp1 = tmp1->next;
	}

	*val = s->top->data;
	tmp1->next = s->top->next;
	s->top = tmp1;
	free(tmp2);

	return true;
}

bool clear_stack(snode *s)
{
	int i;
	int val;
	int len = lenght_stack(s);

	for (i = 0; i < len; i++)
		pop_stack(s, &val);

	return true;
}

int main(int argc, char *argv[])
{
	int i;
	int val;
	snode s;

	// Init
	if (init_stack(&s) != true) {
		printf("Init stack failed.\n");
		return 0;
	} else {
		printf("Init stack success.\n");
	}

	// Push
	for (i = 1; i <= 6; i++) {
		if (push_stack(&s, i) != true) {
			printf("Push stack %d failed.\n", i);
			return 0;
		}
	}
	printf("Push stack success.\n");

	// Traverse
	traverse_stack(&s);

	// Reading top, lenght and if stack is empty
	printf("top stack: %d\n", top_stack(&s));
	printf("stack lenght: %d\n", lenght_stack(&s));
	printf("empty stack: %d\n", empty_stack(&s));

	// Pop
	for (i = 0; i < 2; i++) {
		if (pop_stack(&s, &val) == true)
			printf("Pop stack val: %d\n", val);
		else
			printf("Pop stack failed.\n");
	}
	traverse_stack(&s);

	// Clear
	clear_stack(&s);
	traverse_stack(&s);

	return 0;
}
