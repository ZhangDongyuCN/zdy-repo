#include <iostream>
#include "Shell.h"

using namespace std;

int main()
{
    vector<string> v;
    auto s = Shell::exeShellCmd("ls -la", v);
    if (s.first == false)
    {
        cout << s.second << endl;
    }
    else
    {
        for (auto& iElem : v)
        {
            cout << iElem << endl;
        }
    }



    return 0;
}