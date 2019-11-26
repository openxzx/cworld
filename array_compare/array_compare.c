/*
 * Comparing if all values of one array is the same.
 */

#include <stdio.h>

int main(int argc, char **argv)
{
	int key;
	int data[20];
	unsigned int i;

	/* Initial array */
	for (i = 0; i < sizeof(data) / sizeof(int); i++) {
		if ((i == 6) || (i == 8) || (i == 9) || (i == 15) || (i == 18))
			data[i] = i;
		else
			data[i] = 1;
	}

	/* Compare array's value */
	key = data[0];
	for (i = 0; i < sizeof(data) / sizeof(int); i++) {
		if (key == data[i])
			printf("%d ", data[i]);
		else
			printf("%d(%d) ", data[i], i);
	}
	printf("\n");

	return 0;
}
