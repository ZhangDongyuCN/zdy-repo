//Linux & C++11

#ifndef __CSV__
#define __CSV__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#define IN
#define OUT
#define INOUT

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::streamoff;
using std::ios_base;
using std::endl;
using std::move;
using std::getline;
using std::regex;
using std::regex_replace;

namespace sinicheveen
{
	template<class T>
	class CSV
	{
	public:
		//把一维数组存入 CSV 文件
		inline static void saveArray(IN const T* pArray, IN int rowNum, IN const string strFileName);

		//把二维数组存入 CSV 文件
		inline static void saveArray(IN const T* pArray, IN int lineNum, IN int rowNum, IN const string strFileName);

		//把一维 vector 数组，存入 CSV 文件
		inline static void saveVector(IN const vector<T>& vArray, IN const string strFileName);

		//把二维 vector 数组，存入 CSV 文件
		inline static void saveVector(IN const vector<vector<T>>& vArray, IN const string strFileName);

		//读取 CSV 文件数据，存入到一维 vector 数组中
		inline static void csvToVector(IN const string strFileName, OUT vector<T>& vData);

		//读取 CSV 文件数据，存入到二维 vector 数组中
		inline static void csvToVector(IN const string strFileName, OUT vector<vector<T>>& vData);
	};

	/*
	*Name：saveArray
	*Description：把一维数组存入 CSV 文件
	*Input：IN cosnt T* pArray，数组地址
	*              IN int rowNum，数组列数
	*              IN const string strFileName，文件名
	*Return：void
	*Caution：
	*/
	template<class T>
	inline void CSV<T>::saveArray(IN const T* pArray, IN int rowNum, IN const string strFileName)
	{
		//保存数组到文件。如果文件不存在，创建文件，并写入数据；如果文件存在，清空重新写入
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		for (int i = 0; i < rowNum; i++)
		{
			if (i < rowNum - 1)
				fout << *(pArray + i) << ",";
			else
				fout << *(pArray + i) << endl;
		}
		fout.close();
	}    //saveArray()

	/*
	*Name：saveArray
	*Description：把二维数组存入 CSV 文件
	*Input：IN const T* pArray，数组第一个元素地址，e.g.「&array[0][0]」
	*              IN int lineNum，数组行数
	*              IN int rowNum，数组列数
	*              IN const string strFileName，文件名
	*Return：void
	*Caution：
	*/
	template<class T>
	inline void CSV<T>::saveArray(IN const T* pArray, IN int lineNum, IN int rowNum, IN const string strFileName)
	{
		//保存数组到文件。如果文件不存在，创建文件，并写入数据；如果文件存在，清空重新写入
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		for (int i = 0; i < lineNum; i++)
		{
			for (int j = 0; j < rowNum; j++)
			{
				if (j < rowNum - 1)
					fout << *(pArray + i * rowNum + j) << ",";    // pArray + i * rowNum + j：找到当前数组元素的顺序索引值
				else
					fout << *(pArray + i * rowNum + j) << endl;
			}
		}
		fout.close();
	}    //saveArray()

	/*
	*Name：saveVector
	*Description：把一维 vector 数组，存入 CSV 文件
	*Input：IN const vector<vector<T>>& vArray，vector 一维数组
	*              IN const string strFileName，文件名
	*Return：void
	*Caution：
	*/
	template<class T>
	inline void CSV<T>::saveVector(IN const vector<T>& vArray, IN const string strFileName)
	{
		//保存数组到文件。如果文件不存在，创建文件，并写入数据；如果文件存在，清空重新写入
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		int rowNum = vArray.size();
		for (int i = 0; i < rowNum; i++)
		{
			if (i < rowNum - 1)
				fout << vArray[i] << ",";
			else
				fout << vArray[i] << endl;
		}
		fout.close();
	}    //saveVector()

	/*
	*Name：saveVector
	*Description：把二维 vector 数组，存入 CSV 文件
	*Input：IN const vector<vector<T>>& vArray，vector 二维数组
	*              IN const string strFileName，文件名
	*Return：void
	*Caution：
	*/
	template<class T>
	inline void CSV<T>::saveVector(IN const vector<vector<T>>& vArray, IN const string strFileName)
	{
		//保存数组到文件。如果文件不存在，创建文件，并写入数据；如果文件存在，清空重新写入
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		int lineNum = vArray.size(), rowNum = vArray[0].size();
		for (int i = 0; i < lineNum; i++)
		{
			for (int j = 0; j < rowNum; j++)
			{
				if (j < rowNum - 1)
					fout << vArray[i][j] << ",";
				else
					fout << vArray[i][j] << endl;
			}
		}
		fout.close();
	}    //saveVector()

	/*
	*Name：csvToVector
	*Description：读取 CSV 文件数据，存入到一维 vector 数组中
	*Input：IN const string strFileName，文件名
	*             OUT vector<T>& vData，读取到的数据
	*Return：void
	*Caution：
	*/
	template<class T>
	inline void CSV<T>::csvToVector(IN const string strFileName, OUT vector<T>& vData)
	{
		ifstream fin;
		fin.open(strFileName.c_str(), ios_base::in);    //以只读的方式打开文件

		//跳过 CSV 文件开头可能出现的空行
		char ch;
		while (fin.get(ch))
		{
			if (ch == '\n')
				continue;
			else
				break;
		}
		streamoff pos = fin.tellg();    //保存当前位置
		fin.seekg(pos - 1, ios_base::beg);    //返回到文件中有数据开始的那一行的首位

		//读取数据
		string strBuf;
		getline(fin, strBuf);
		regex e(",");
		stringstream ss{ regex_replace(strBuf, e, " ") };
		T data;
		while (ss >> data)
		{
			vData.push_back(move(data));
		}
	}    //csvToVector()

	/*
	*Name：csvToVector
	*Description：读取 CSV 文件数据，存入到二维 vector 数组中
	*Input：IN const string strFileName，文件名
	*             OUT vector<vector<T>>& vData，读取到的数据
	*Return：void
	*Caution：
	*/
	template<class T>
	inline void CSV<T>::csvToVector(IN const string strFileName, OUT vector<vector<T>>& vData)
	{
		ifstream fin;
		fin.open(strFileName.c_str(), ios_base::in);    //以只读的方式打开文件

		//跳过 CSV 文件开头可能出现的空行
		char ch;
		while (fin.get(ch))
		{
			if (ch == '\n')
				continue;
			else
				break;
		}
		streamoff pos = fin.tellg();    //保存当前位置
		fin.seekg(pos - 1, ios_base::beg);    //返回到文件中有数据开始的那一行的首位

		//获取 CSV 文件中数据的行数
		int lineNum = 0, rowNum = 0;
		string strBuf;
		while (getline(fin, strBuf) && !strBuf.empty())
		{
			lineNum = lineNum + 1;
		}
		fin.clear();    //getline() 读取到文件尾，接下来输入流被阻断。需要重置输入流，如果不重置，接下来将无法获取文件数据

		//获取 CSV 文件中数据的列数
		fin.seekg(pos - 1, ios_base::beg);    //返回到文件中有数据开始的那一行的首位
		while (fin.get(ch))
		{
			if (ch == ',')
			{
				rowNum = rowNum + 1;
			}
			else
			{
				if ('\r' == ch || '\n' == ch)
					break;
			}
		}
		rowNum = rowNum + 1;

		//把 CSV 文件中的数据存入 vector 中
		fin.seekg(pos - 1, ios_base::beg);    //返回到文件中有数据开始的那一行的首位
		strBuf.clear();
		stringstream ss;
		regex e(",");
		int j;
		vector<vector<T>> vData_(lineNum, vector<T>(rowNum));
		for (int i = 0; i < lineNum; i++)
		{
			//读取数据
			getline(fin, strBuf);
			ss.str(regex_replace(strBuf, e, " "));
			T data;
			j = 0;
			while (ss >> data)
			{
				vData_[i][j] = move(data);
				j++;
			}
			ss.clear();
			ss.str("");
			strBuf.clear();
		}
		fin.close();

		vData = move(vData_);    //使用移动赋值函数，减小开销
	}    //csvToVector()

}    //sinicheveen

#endif    //__CSV__
