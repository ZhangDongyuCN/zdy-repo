//Linux & C++11

//根据两点式方法求直线，并求两条直线的交点

#ifndef SINICH_EVEEN_LINE_
#define SINICH_EVEEN_LINE_

//参数流向
#define IN
#define OUT
#define INOUT

namespace sinicheveen
{
	struct tPoint
	{
		double x;
		double y;
	};    //tPoint

	class Line
	{
	public:
		//两点法求直线
		inline static Line getLine(IN tPoint sourcePoint, IN tPoint endPoint);

		//计算两条直线的交点
		inline static tPoint calCrossPoint(IN Line line1, IN Line line2);

	public:
		double a;    //a*x + b*y + c = 0
		double b;
		double c;
	};    //Line

   /*
	*Name：getLine
	*Description：两点法求直线
	*Input：IN tPoint sourcePoint，起点
	*              IN tPoint endPoint，重点
	*Return：Line，求得的直线
	*Caution：
	*/
	inline Line Line::getLine(IN tPoint sourcePoint, IN tPoint endPoint)
	{
		Line Line_;
		Line_.a = sourcePoint.y - endPoint.y;
		Line_.b = endPoint.x - sourcePoint.x;
		Line_.c = sourcePoint.x * endPoint.y - endPoint.x * sourcePoint.y;
		return Line_;
	}    //getLine

   /*
	*Name：calCrossPoint
	*Description：计算两条直线的交点
	*Input：IN Line line1，直线 1
	*              IN Line line2，直线 2
	*Return：tPoint，交点
	*Caution：如果两条直线平行，则 x 和 y 为无穷，因此要用 isnan() 或者 isinf() 函数判断
	*/
	inline tPoint Line::calCrossPoint(IN Line line1, IN Line line2)
	{
		tPoint Point_;
		double D;
		D = line1.a * line2.b - line2.a * line1.b;
		Point_.x = (line1.b * line2.c - line2.b * line1.c) / D;
		Point_.y = (line1.c * line2.a - line2.c * line1.a) / D;
		return Point_;
	}    //calCrossPoint

}    //sinicheveen

#endif    //SINICH_EVEEN_LINE_

/*用法示例
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
*/
