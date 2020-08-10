#include <iostream>

/* Std namespace */
using namespace std;

int main(void)
{
        int ival;
        float fval;

        cin >> ival;
        cin >> fval;

        ival = ival + (int)fval;

        cout << ival << endl;

        return 0;
}
