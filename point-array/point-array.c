#include <stdio.h>
#include <string.h>

void show_hello(void)
{
    printf("Hello\n");
}

void show_world(void)
{
    printf("World\n");
}

void (* show[])(void) = {
    show_hello,
    show_world,
    show_world,
    show_hello,
};

int main(void)
{
    int i;

    for (i = 0; i < sizeof(show) / sizeof(void *); i++)
        show[i]();

    return 0;
}
