#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "CSV.hpp"

using namespace std;
using namespace sinicheveen;

int main()
{
	vector<int> vec;
	CSV<int>::csvToVector("1.csv", vec);

	for (const auto& iElem:vec)
	{
		cout << iElem << ",";
	}
	cout << endl;

	return 0;
}