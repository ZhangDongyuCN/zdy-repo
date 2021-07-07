#ifndef SINICH_EVEEN_INTEGRAL_
#define SINICH_EVEEN_INTEGRAL_

/*
*Environment：
*Linux(Ubuntu), C++11，gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
*Description：
*计算定积分、不定积分、模特卡罗积分法
*用法示例是以前的，根据实际情况更改
*/

#include <iostream>
#include <cmath>
#include <ctime>

//参数流向
#define IN
#define OUT
#define INOUT

using std::cout;
using std::endl;

namespace sinicheveen
{
	struct tIntegralVal
	{
		double value;    //积分值
		double error;    //误差
	};    //tIntegralVal

	class Integral
	{
	public:
		inline static double definiteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double step = 0.01);
		inline static double infiniteIntegral(IN double(*pf)(double), IN double a, IN double step = 0.01, IN int N = 100, IN double precision = 0.01, IN bool type = 1);
		inline static tIntegralVal monteCarloDefiniteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double precision = 0.01);
	};    //Integral

   /*
	*Name：definiteIntegral
	*Description：计算函数在[a,b]的定积分
	*Input：IN double(*pf)(double)，函数指针
	*              IN double a，积分上限
	*              IN double b，积分下限
	*              IN double step，步长（默认0.01）
	*Return：double，积分值
	*Caution：
	*/
	inline double Integral::definiteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double step)
	{
		double S = 0;
		double value1 = (*pf)(a);
		double value2;
		double trapezoidalArea;
		for (double i = a; i < b; i = i + step)
		{
			value2 = (*pf)(i + step);
			trapezoidalArea = (value1 + value2) * step / 2;
			value1 = value2;
			S = S + trapezoidalArea;
		}
		return S;
	}

	////示例：Visual Stuido Community 2017
	//#include <iostream>
	//#include"Integral.h"
	//
	//double fexp(double x);
	//
	//using namespace std;
	//
	//int main()
	//{
	// Integral caculute;
	// double(*pf)(double x);
	// p = fexp;
	// double sum = caculute.definiteIntegral(p, 0, 1, 0.001);
	// cout << sum << endl;
	//
	// system("pause");
	// return 0;
	//}
	//
	////exp(x)函数
	//double fexp(double x)
	//{
	// return exp(x);
	//}

	/*
	*Name：infiniteIntegral
	*Description：计算函数的无穷积分。原理：设定一个初始积分下限，不断的增加下限(每次增加N)，直到和上一次的积分之差满足设定的精度
	*Input：IN double(*pf)(double)，函数指针
	*              IN double a，积分上限
	*              IN double step，步长（默认0.01）
	*              IN int N，积分下限每次增加的值（默认100）
	*              IN double precision，精度（默认0.001）
	*              IN int type，true 计算函数在[a,infinite)的无穷积分；false 计算函数在(infinite,a]的无穷积分。（默认为 true）
	*Return：double，积分值
	*Caution：
	*/
	inline double Integral::infiniteIntegral(IN double(*pf)(double), IN double a, IN double step, IN int N, IN double precision, IN bool type)
	{
		if (type == true)
		{
			//计算函数在[a,infinity)的无穷积分
			//计算第一块的面积
			double S1 = 0;
			double value1 = (*pf)(a);    //梯形的上底
			double value11;    //梯形的下底
			double trapezoidalArea1;
			for (double i = a; i < a + N; i = i + step)
			{
				value11 = (*pf)(i + step);
				trapezoidalArea1 = (value1 + value11) * step / 2;    //梯形的面积
				value1 = value11;
				S1 = S1 + trapezoidalArea1;
			}

			//增加一块，并计算面积和
			double S2 = S1;
			a = a + N;
			double value2 = (*pf)(a);
			double value22;
			double trapezoidalArea2;
			for (double i = a; i < a + N; i = i + step)
			{
				value22 = (*pf)(i + step);
				trapezoidalArea2 = (value2 + value22) * step / 2;
				value2 = value22;
				S2 = S2 + trapezoidalArea2;
			}

			while (S2 - S1 > precision)    //判断增加一块后的面积和不增加之前的面积差是否小于所要求的精度值
			{
				S1 = S2;
				a = a + N;
				double value3 = (*pf)(a);
				double value33;
				double trapezoidalArea3;
				for (double i = a; i < a + N; i = i + step)
				{
					value33 = (*pf)(i + step);
					trapezoidalArea3 = (value3 + value33) * step / 2;
					value3 = value33;
					S2 = S2 + trapezoidalArea3;
				}
			}
			return S1;
		}
		else
		{
			//计算函数在(infinity,a]的无穷积分
			//计算第一块的面积
			double S1 = 0;
			double value1 = (*pf)(a);    //梯形的上底
			double value11;    //梯形的下底
			double trapezoidalArea1;
			for (double i = a; i > a - N; i = i - step)
			{
				value11 = (*pf)(i - step);
				trapezoidalArea1 = (value1 + value11) * step / 2;    //梯形的面积
				value1 = value11;
				S1 = S1 + trapezoidalArea1;
			}
			return S1;

			//增加一块，并计算面积和
			double S2 = S1;
			a = a - N;
			double value2 = (*pf)(a);
			double value22;
			double trapezoidalArea2;
			for (double i = a; i > a - N; i = i - step)
			{
				value22 = (*pf)(i - step);
				trapezoidalArea2 = (value2 + value22) * step / 2;
				value2 = value22;
				S2 = S2 + trapezoidalArea2;
			}


			while (S2 - S1 > precision)    //判断增加一块后的面积和不增加之前的面积差是否小于所要求的精度值
			{
				S1 = S2;
				a = a - N;
				double value3 = (*pf)(a);
				double value33;
				double trapezoidalArea3;
				for (double i = a; i > a - N; i = i - step)
				{
					value33 = (*pf)(i - step);
					trapezoidalArea3 = (value3 + value33) * step / 2;
					value3 = value33;
					S2 = S2 + trapezoidalArea3;
				}
			}
			return S1;
		}
	}    //infiniteIntegral()

	////示例：Visual Stuido Community 2017
	//#include <iostream>
	//#include"Integral.h"
	//
	//double fexp(double x);
	//double ffexp(double x);
	//
	//using namespace std;
	//
	//int main()
	//{
	// Integral caculute;
	// double(*pf)(double x);
	// p = fexp;
	// double sum1 = caculute.infiniteIntegral(p, 0, 0.001, 1000, 0.01, 2);
	// p = ffexp;
	// double sum2 = caculute.infiniteIntegral(p, 0, 0.001, 1000, 0.01, 1);
	// cout << sum1 << endl;
	// cout << sum2 << endl;
	//
	// system("pause");
	// return 0;
	//}
	//
	////exp(x)函数
	//double fexp(double x)
	//{
	// return exp(x);
	//}
	//
	////exp(-x)函数
	//double ffexp(double x)
	//{
	// return exp(-x);
	//}

	/*
	*Name：monteCarloDefiniteIntegral
	*Description：蒙特卡洛积分法，计算函数的定积分；相对常规梯形积分法，计算较慢
	*Input：IN double(*pf)(double)，函数指针
	*              IN double a，积分上限
	*              IN double b，积分下限
	*              IN double precision，精度（默认0.01）
	*Return：tIntegralVal，积分结果（含积分值和误差）
	*Caution：
	*/
	inline tIntegralVal Integral::monteCarloDefiniteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double precision)
	{
		double S, S1, S2, u, x, error;    //S是积分值；S1和S2用来误差估计
		int N = int(floor(abs(b - a) / 0.1));
		while (1)
		{
			S1 = 0.0;
			S2 = 0.0;
			srand((unsigned)time(NULL));    //初始化种子时间为系统时间
			for (int i = 0; i < N; i++)
			{
				u = 1.0 * rand() / (RAND_MAX + 1);    //产生0.0~1.1之间的随机数
				x = a + (b - a) * u;
				S1 = S1 + (*pf)(x);
				S2 = S2 + (*pf)(x) * (*pf)(x);
			}
			S = S1 * (b - a) / N;    //函数积分值
			S1 = S1 / N;
			S2 = S2 / N;
			error = (b - a) * sqrt((S2 - S1 * S1) / N);    //误差估计
			if (error > precision)
			{
				N = N + N;
				continue;
			}
			else
			{
				break;
			}
		}

		//返回积分值和误差
		tIntegralVal tIntegralVal_;
		tIntegralVal_.value = S;
		tIntegralVal_.error = error;
		return tIntegralVal_;
	}    //monteCarloDefiniteIntegral()

	////示例：Visual Studio Community 2017
	////蒙特卡洛积分法和常规梯形积分法对比
	//#include <iostream>
	//#include"Integral.h"
	//
	//using namespace std;
	//
	//double fun(double);
	//
	//int main()
	//{
	// Integral caculate;
	// double(*pf)(double);
	// double a = 0; //积分上限
	// double b = 10; //积分下限
	// double c = 0.01; //精度
	// p = fun;
	// auto A = caculate.monteCarloDefiniteIntegral(p, a, b, c);
	// double B = caculate.definiteIntegral(p, a, b, c);
	// cout << A.value << endl;
	// cout << A.error << endl;
	// cout << B << endl;
	//
	// system("pause");
	// return 0;
	//}
	//
	//double fun(double x)
	//{
	// const double pi = 3.1415926;
	// double y;
	// y = sin(x);
	// //y = cos(x);
	// //y = x * x;
	// //y = 2.0*sqrt(x) / ((x + 1.0)*(x + 1.0));
	// //y = 0.2 / (pow((x - 6.0), 2.0) + 0.02);
	// //y = x * cos(10.0*pow(x, 2.0)) / (pow(x, 2.0) + 1.0);
	// //y = sqrt(x) / (x*x + 1.0);
	// //y = sin(pi*x*x) / ((x - pi)*(x - pi) + 1);
	// return y;
	//}

}    //sinicheveen

#endif    //SINICH_EVEEN_INTEGRAL_
