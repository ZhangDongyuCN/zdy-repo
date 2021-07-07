#include "MyExp.hpp"
#include <iostream>

using namespace std;

int main()
{
    try
    {
        string a("MyExp2");
        throw MyExp2(a);
    }
    catch (const MyExp2 & e)
    {
        cout << e.what() << endl;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}