#include "ProcessString.hpp"
#include <iostream>

using namespace std;
using namespace sinicheveen;


int main()
{
    string a{ " A,4 56, 789 " };
    vector<string> vEm;
    ProcessString::splitStr(a, vEm, ',');
    for (auto& iElem : vEm)
    {
        cout << iElem << endl;
    }
    return 0;
}