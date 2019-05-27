/*
 * Author: Zhenxing Xu <xzxlnmail@163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "list.h"

/* Create List */
list_node_type list_create(char *s)
{
	list_node_type tmp;

	tmp.elem = (char *)malloc(10);
	if (tmp.elem == NULL) {
		printf("\033[31mList create failed.\033[0m\n");
		tmp.elem = NULL;
		tmp.lenght = 0;

		return tmp;
	}
	memset(tmp.elem, 0, 10);

	tmp.elem = s;
	tmp.lenght = strlen(tmp.elem);
	tmp.lenght = strlen(tmp.elem);

	printf("List create success.\n");

	return tmp;
}

/* Insert datas item into List Head */
int list_insert(char * elem, int index, char data, int len)
{
	int i;

	if (index < 0) {
		printf("\033[31mindex error.\033[0m\n");
		return -1;
	}

	for (i = (len - 1); i >= index; i--)
		elem[i + 1] = elem[i];

	elem[index] = data;

	printf("List insert success.\n");

	return 0;
}

/* Delete datas item form list */
int list_delete(char * elem, int index, int len)
{
	int i;

	if (index < 0) {
		printf("\033[31mindex error.\033[0m\n");
		return -1;
	}

	for (i = index; i < (len - 1); i++)
		elem[i] = elem[i + 1];

	printf("List delete success.\n");

	return 0;
}
