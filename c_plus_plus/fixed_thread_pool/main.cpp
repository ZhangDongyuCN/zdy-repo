#include <iostream>

#include "FixedThreadPool.hpp"

using namespace std;
using namespace sinicheveen;

void fun()
{
    while (true)
    {
        cout << 1 << endl;
    }
}


int main()
{
    thread t{ fun };
    
    t.detach();

    for (int i = 0; i < 100000; i++)
    {

    }
    

	return 0;
}