#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

bool init_stack(snode *s)
{
	s = (snode *)malloc(sizeof(snode));
	if (s == NULL)
		return false;

	s->data = 0;
	s->next = NULL;

	return true;
}

bool push_stack(snode *s, int val)
{
	snode *p;

	p = (snode *)malloc(sizeof(snode));
	if (p == NULL)
		return false;

	p->next = s->next;
	s->next = p;
	p->data = val;

	return true;
}

int traverse_stack(snode *s)
{
	snode *tmp = s->next;

	if (tmp == NULL) {
		printf("Stack is NULL.\n");
		return 0;
	}

	while (tmp != NULL) {
		printf("%d ", tmp->data);
		tmp = tmp->next;
	}

	printf("\n");

	return 0;
}

bool pop_stack(snode *s, int *val)
{
	snode *tmp = s->next;

	if (tmp != NULL) {
		s->next = tmp->next;
		*val = tmp->data;
		free(tmp);
		return true;
	} else
		return false;
}

bool clear_stack(snode *s)
{
	snode *tmp = s->next;

	while (tmp != NULL) {
		s->next = tmp->next;
		free(tmp);
		tmp = s->next;
	}

	s->next = NULL;

	return true;
}

int top_stack(snode s)
{
	return s.next->data;
}

bool empty_stack(snode s)
{
	if (s.next == NULL)
		return true;
	else
		return false;
}

int lenght_stack(snode s)
{
	int cnt = 0;
	snode *tmp = s.next;

	while (tmp != NULL) {
		s.next = tmp->next;
		tmp = s.next;
		cnt++;
	}

	return cnt;
}

int main(int argc, char *argv[])
{
	int i;
	int val = 0;
	snode s;

	if (init_stack(&s) != true) {
		printf("Init stack failed.\n");
		return 0;
	}

	/* Test1 */
	for (i = 1; i <= 6; i++) {
		if (push_stack(&s, i) != true) {
			printf("Push stack %d failed.\n", i);
			return 0;
		}
	}
	traverse_stack(&s);

	printf("top stack: %d\n", top_stack(s));
	printf("empty stack: %d\n", empty_stack(s));
	printf("stack lenght: %d\n", lenght_stack(s));

	/* Test2 */
	if (pop_stack(&s, &val) == true)
		printf("Pop stack val: %d\n", val);
	else
		printf("Pop stack failed.\n");
	traverse_stack(&s);

	/* Test3 */
	clear_stack(&s);
	traverse_stack(&s);

	return 0;
}
