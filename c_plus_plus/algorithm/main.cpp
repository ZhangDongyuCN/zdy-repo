#include <iostream>
#include "Algorithm.ipp"

using namespace std;

int main()
{
    vector<int> array = { 2,1,3,9,7,5,0,6,8,4 };

    mergeSort_Iteration_Ascend(array);

    for (const auto& iElem : array)
    {
        cout << iElem << " ";
    }
    cout << endl;

    return 0;
}