#include <iostream>
#include "Time.hpp"

using namespace std;
using namespace sinicheveen;

int main()
{
	cout << Time::getTimeStampByAge(0) << endl;
	cout << Time::getTimeStampByAge(100) << endl;

	cout << Time::timeStampToStdTime(Time::getTimeStampByAge(100)) << endl;

    return 0;
}