#include<stdio.h>

/*
 * char * argv[]: point array, one point array is a parameter of the string.
 */
int main(int argc, char *argv[])
{
	int i, j, k;
	int len;
	char tmp;

	printf("argc: %d\n", argc);

	for (i = 1; i < argc; i++) {
		printf("argv[%d]: %s\n", i, argv[i]);

		len = sizeof(argv[i]) / sizeof(char) - 1;
		for (j = 0; j < len / 2; j++) {
			tmp = argv[i][j];
			argv[i][j] = argv[i][len - j - 1];
			argv[i][len - j - 1] = tmp;
		}

		for (k = 0; k < len; k++)
			printf("%c\n", argv[i][k]);
	}

	return 0;
}
