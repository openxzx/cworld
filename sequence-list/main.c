/*
 * Author Zhenxing Xu <xzxlnmail.163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 */

#include<stdio.h>
#include<string.h>
#include <malloc.h>

#include"list.h"

int main(int argc, char *argv[])
{
	int i;
	list_node_type list;

	if (argc != 2) {
		printf("Please input 2 parameters.\n");
		return 0;
	}

	if (strlen(argv[1]) > 10) {
		printf("Second parameters is too lenght.\n");
		return 0;
	}

	/* Create list */
	list = list_create(argv[1]);
	if (list.elem == NULL)
		return 0;
	printf("%s, %d\n", list.elem, list.lenght);

	/* Insert data into list*/
	if (!list_insert(list.elem, 1, '8', list.lenght)) {
		list.lenght++;
		if (list.lenght > 10)
			list.lenght = 10;

		for (i = 0; i < list.lenght; i++) {
			if (i == (list.lenght - 1))
				printf("%c\n", list.elem[i]);
			else
				printf("%c, ", list.elem[i]);
		}
	}

	/* Delete data from list*/
	if (!list_delete(list.elem, 3, list.lenght)) {
		list.lenght--;

		for (i = 0; i < list.lenght; i++) {
			if (i == (list.lenght - 1))
				printf("%c\n", list.elem[i]);
			else
				printf("%c, ", list.elem[i]);
		}
	}

	return 0;
}
