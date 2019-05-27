/*
 * Author Zhenxing Xu <xzxlnmail.163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 */

#include<stdio.h>
#include<string.h>

#include"list.h"

char * merge_array(char *array1, char *array2, char *new, int len1, int len2)
{
	int i, j;
	int idx = 0;

	for (i = 0; i < len1; i++)
		new[i] = array1[i];

	for (i = 0; i < len2; i++) {
		for (j = 0; j < len1; j++) {
			if (array2[i] == array1[j])
				break;

			if (j == (len1 - 1)) {
				new[len1 + idx] = array2[i];
				idx++;
			}
		}
	}

	return new;
}

int main(int argc, char *argv[])
{
	int i;
	int len1, len2, len;
	list_node_type *list;

	/* Judging input parameters count */
	if (argc != 3) {
		printf("Input parameters count error.\n");
		return 0;
	}

	/* Get string parameters lenght */
	len1 = strlen(argv[1]);
	len2 = strlen(argv[2]);
	len = len1 + len2;

	/* Define new array */
	char tmp[len];

	memset(tmp, 0, len);

	merge_array(argv[1], argv[2], tmp, len1, len2);
	for (i = 0; i < len; i++) {
		if (tmp[i] != '\0')
			printf("tmp[%d]: %c\n", i, tmp[i]);
	}

	/* Create List */
	list = list_create();
	if (list == NULL)
		return 0;

	/* Insert data item into List */
	for (i = 0; i < len; i++) {
		if (tmp[i] != '\0')
			list_insert(list, i, tmp[i]);
	}

	/* Traverse List */
	list_traverse(list);

	/* Destroy List */
	list_destroy(list);

	/* Traverse List */
	list_traverse(list);

	return 0;
}
