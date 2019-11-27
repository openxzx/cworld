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

union utype {
	int a;
	float b;
	char c;
	unsigned int d;
	double f;
	long unsigned int g;
};

int main(int argc, char **argv)
{
	struct stype stmp;
	stmp.a = 1;
	stmp.b = 1.0;
	stmp.c = '1';
	stmp.d = 1;
	stmp.f = 1.0;
	stmp.g = 1;

	union utype utmp;
	utmp.a = 1;
	utmp.b = 1.0;
	utmp.c = '1';
	utmp.d = 1;
	utmp.f = 1.0;
	utmp.g = 1;

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
