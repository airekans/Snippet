#include "BigInt.h"
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
    int num1, num2;
    cin >> num1 >> num2;

    BigInt<char> big1(num1);
    BigInt<char> big2(num2);

    BigInt<char> result = big1 + big2;
    cout << big1 << " + " << big2 << " = " << result << endl;
    
    return 0;
}
