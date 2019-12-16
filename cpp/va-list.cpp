#include <iostream>
#include <cstdarg>

using namespace std;

double average(int num, ...)
{
        int i;
        va_list args;
        double sum = 0;

        va_start(args, num);

        for (i = 0; i < num; i++)
                sum += va_arg(args, double);

        va_end(args);

        return sum / num;
}

int main(int argc, char **argv)
{
        cout << average(3, 1.0, 2.0, 3.0) << '\n';
        cout << average(3, 2.0, 2.5, 3.5) << '\n';

        return 0;
}
