#include "Line.hpp"

#include <iostream>

using namespace std;
using namespace sinicheveen;

int main()
{
	tPoint a{ 0,0 };
	tPoint b{ 1,1 };
	tPoint c{ 1,0 };
	tPoint d{ 0,1 };
	Line line1 = Line::getLine(a, b);
	Line line2 = Line::getLine(c, d);
	tPoint e= Line::calCrossPoint(line1, line2);
	cout << e.x << endl << e.y << endl;

	return 0;
}