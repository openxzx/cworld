/*
 * The size of base data type, struct type and union type
 */

#include <stdio.h>

struct stype {
	int a;
	float b;
	char c;
	unsigned int d;
	double f;
	long unsigned int g;
};

/* Struct initrial */
struct stype stmp = {
	.a = 1,
	.b = 1.0,
	.c = '1',
	.d = 1,
	.f = 1.0,
	.g = 1,
};

union utype {
	int a;
	float b;
	char c;
	unsigned int d;
	double f;
	long unsigned int g;
};

/* Union initrial */
union utype utmp = {
	.a = 1,
	.b = 1.0,
	.c = '1',
	.d = 1,
	.f = 1.0,
	.g = 1,
};

int main(int argc, char **argv)
{
	printf("%d\n", (unsigned int)sizeof(stmp.a));
	printf("%d\n", (unsigned int)sizeof(stmp.b));
	printf("%d\n", (unsigned int)sizeof(stmp.c));
	printf("%d\n", (unsigned int)sizeof(stmp.d));
	printf("%d\n", (unsigned int)sizeof(stmp.f));
	printf("%d\n", (unsigned int)sizeof(stmp.g));
	printf("%d\n", (unsigned int)sizeof(struct stype));
	printf("\n");

	printf("%d\n", (unsigned int)sizeof(utmp.a));
	printf("%d\n", (unsigned int)sizeof(utmp.b));
	printf("%d\n", (unsigned int)sizeof(utmp.c));
	printf("%d\n", (unsigned int)sizeof(utmp.d));
	printf("%d\n", (unsigned int)sizeof(utmp.f));
	printf("%d\n", (unsigned int)sizeof(utmp.g));
	printf("%d\n", (unsigned int)sizeof(union utype));
	printf("\n");

	return 0;
}
