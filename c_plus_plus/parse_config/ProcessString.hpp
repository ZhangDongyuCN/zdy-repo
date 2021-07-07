#ifndef PROCESS_STRING_
#define PROCESS_STRING_

/*
*Environment：
*Linux(Ubuntu), C++11，gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
*Description：
*字符串处理
*/

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <string.h>

//宏定义：参数流向
#define IN
#define OUT
#define INOUT

//使用命名
using std::string;
using std::istringstream;
using std::vector;
using std::cin;

namespace sinicheveen
{
	class ProcessString
	{
	public:
	//group 1：判断型函数

		//判断一个字符串是不是数字字符串
		//e.g. "023456789" --> true; "12A56" --> false
		inline static bool isNumString(IN const string& strIn);

		//判断一个字符串是不是以逗号分隔的数字字符串，且字符串最后一个字符不能是逗号，中间不能有空格
		//e.g. "1,2,3,4" --> true; "1,3.14,9" --> true; "1,2,3,4," --> false; "1,A,5" --> false
		inline static bool isNumStringWithComma(IN const string& strIn);

		//判断一个字符串是不是以某种分隔符分隔的数字字符串，且字符串最后一个字符不能是分隔符，中间不能有空格
		//e.g. "1,2,3,4" --> true; "1,3.14,9" --> true; "1,2,3,4," --> false; "1,A,5" --> false
		inline static bool isNumStringWithSeparator(IN const string& strIn, IN const char cSeparator = ',');

	//group 2：字符串转数字

		//字符串转数字
		template<typename T>
		inline static T stringToNumber(IN const string& strIn);

		//从数字字符串中提取出数字
		template <typename T>
		inline static bool extractNumFromString(IN const string& strIn, OUT vector<T>& vExtractedNums, IN const char cSeparator = ',');

	//group 3：格式处理函数

		//移除行首和行尾的空白字符
		inline static void rmSpaceInBegAndEnd(INOUT string& strIn);

	//gruop 4：从键盘读取

		//从键盘读取一行字符串，直到遇到 \n 为止，中间遇到其它空白字符，仍继续
		inline static void readStringEndByEnter(OUT string& strWhatRead);

		//从键盘读取一行字符串，直到遇到多个 \n 为止（由用户定义个数），中间遇到其它空白字符，仍继续
		inline static void readStringEndByMultiEnter(OUT string& strWhatRead, IN const int32_t numOfEnter = 2);

	//group 5：字符串分隔

		//分隔 C 风格字符串
		inline static void spliceString(IN char* pcStrIn, OUT vector<string>& vSubStrings, IN const char* pcSepartor);

	};    //ProcessString

	/*
	 *Name：isNumString
	 *Description：判断一个字符串是不是数字字符串
	 *                         e.g. "023456789" --> true; "12A56" --> false
	 *Input：IN const string& strIn，一个字符串
	 *Return：bool，如果是返回 ture，不是则返回 false
	 *Caution：
	 */
	inline bool ProcessString::isNumString(IN const string& strIn)
	{
		bool bFlag = true;
		for (const auto& iChar : strIn)
		{
			if (iChar >= '0' && iChar <= '9')
			{
				continue;
			}
			else
			{
				bFlag = false;
				return bFlag;
			}
		}
		return bFlag;
	}    //isNumString()

	/*
	 *Name：isNumStringWithComma
	 *Description：判断一个字符串是不是以逗号分隔的数字字符串，且字符串最后一个字符不能是逗号，中间不能有空格
	 *                         e.g. "1,2,3,4" --> true; "1,3.14,9" --> true; "1,2,3,4," --> false; "1,A,5" --> false
	 *Input：IN const string& strIn，一个字符串
	 *Return：bool，如果是返回 ture，不是则返回 false
	 *Caution：
	 */
	inline bool ProcessString::isNumStringWithComma(IN const string& strIn)
	{
		bool bFlag = true;

		//先判断最后一位是不是数字
		if ((*(strIn.rbegin())) >= '0' && (*(strIn.rbegin())) <= '9')
		{

		}
		else
		{
			bFlag = false;
			return bFlag;
		}

		//再判断其它位是不是数字、小数点或逗号
		for (auto ptr = strIn.begin(); ptr != strIn.end() - 1; ptr++)
		{
			if (((*ptr) >= '0' && (*ptr) <= '9') || (*ptr) == '.' || (*ptr) == ',')
			{
				continue;
			}
			else
			{
				bFlag = false;
				return bFlag;
			}
		}

		return bFlag;
	}    //isNumStringWithComma()

	/*
	 *Name：isNumStringWithSeparator
	 *Description：判断一个字符串是不是以某种分隔符分隔的数字字符串，且字符串最后一个字符不能是分隔符，中间不能有空格
	 *                         e.g. "1,2,3,4" --> true; "1,3.14,9" --> true; "1,2,3,4," --> false; "1,A,5" --> false
	 *Input：IN const string& strIn，一个字符串
	 *              IN const char cSeparator = ','，数字之间的分隔符，默认是英文逗号
	 *Return：bool，如果是返回 ture，不是则返回 false
	 *Caution：
	 */
	inline bool ProcessString::isNumStringWithSeparator(IN const string& strIn, IN const char cSeparator)
	{
		bool bFlag = true;

		//先判断最后一位是不是数字
		if ((*(strIn.rbegin())) >= '0' && (*(strIn.rbegin())) <= '9')
		{

		}
		else
		{
			bFlag = false;
			return bFlag;
		}

		//再判断其它位是不是数字、小数点或分隔符
		for (auto ptr = strIn.begin(); ptr != strIn.end() - 1; ptr++)
		{
			if (((*ptr) >= '0' && (*ptr) <= '9') || (*ptr) == '.' || (*ptr) == cSeparator)
			{
				continue;
			}
			else
			{
				bFlag = false;
				return bFlag;
			}
		}

		return bFlag;
	}    //isNumStringWithSeparator()

   /*
	*Name：stringToNumber
	*Description：字符串转数字
	*Input：IN const string& strIn，一个字符串
	*Return：T：转换后的数字
	*Caution：
	*/
	template<typename T>
	inline T ProcessString::stringToNumber(IN const string& strIn)
	{
		istringstream iss(strIn);
		T num;
		iss >> num;
		return num;
	}    //stringToNumber()

    /*
	 *Name：extractNumFromString
	 *Description：从数字字符串中提取出数字
	 *Input：IN const string& strIn，以分隔符分隔的一个数字字符串
	 *             OUT vector<T>& vExtractedNums，提取出的数字
	 *             IN const char cSeparator = ','，数字之间的分隔符，默认是英文逗号
	 *Return：bool，成功返回 true，失败返回 false
	 *Caution：
	 */
	template <typename T>
	inline bool ProcessString::extractNumFromString(IN const string& strIn, OUT vector<T>& vExtractedNums, IN const char cSeparator)
	{
		//判断是不是数字字符串
		for (const auto& iChar : strIn)
		{
			if ((iChar >= '0' && iChar <= '9') || iChar == '.' || iChar == cSeparator)
			{
				continue;
			}
			else
			{
				return false;
			}
		}

		//提取数字
		size_t startPos = 0;
		do
		{
			size_t endPos = strIn.find(cSeparator, startPos);
			if (endPos != string::npos)
			{
				string strNum;
				strNum.assign(strIn, startPos, endPos - startPos);
				T num;
				num = stringToNumber<T>(strNum);
				vExtractedNums.push_back(num);
				startPos = endPos + 1;
				if (startPos > (strIn.size() - 1))
				{
					break;
				}
			}
			else
			{
				string strNum;
				strNum.assign(strIn, startPos, strIn.size() - startPos);
				T num;
				num = stringToNumber<T>(strNum);
				vExtractedNums.push_back(num);
				break;
			}
		} while (1);

		return true;
	}    //extractNumFromString()

	/*
	 *Name：rmSpaceInBegAndEnd
	 *Description：rmSpaceInBegAndEnd = remove space in begin and end，移除行首和行尾的空白字符
	 *Input：INOUT string& strIn，一个字符串
	 *Return：void
	 *Caution：
	 */
	inline void ProcessString::rmSpaceInBegAndEnd(INOUT string& strIn)
	{
		//移除行首空白字符
		if (!strIn.empty())
		{
			uint64_t length = strIn.length();
			uint64_t index = 0;
			for (const auto& iChar : strIn)
			{
				switch (iChar)
				{
				case ' ':
				case '\f':
				case '\n':
				case '\r':
				case '\t':
				case '\v':
					index++;
					break;
				default:
					strIn.erase(strIn.begin(), strIn.begin() + index);
					goto LABLE_RM_SPACE_IN_END;
				}

				if (index == length)    //如果 index 等于 length，说明此行全部是由空白字符组成的，那么就置为空
				{
					strIn = "";
					return;
				}
			}
		}
		else
		{
			return;
		}

		//移除行尾空白字符
	LABLE_RM_SPACE_IN_END:
		uint64_t length = strIn.length();
		uint64_t index = 0;
		for (auto ptr = strIn.rbegin(); ptr != strIn.rend(); ptr++)
		{
			switch (*ptr)
			{
			case ' ':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
			case '\v':
				index++;
				break;
			default:
				strIn.erase(length - index);
				return;
			}
		}

	}    //rmSpaceInBegAndEnd()

	/*
	 *Name：readStringEndByEnter
	 *Description：从键盘读取一行字符串，直到遇到 \n 为止，中间遇到其它空白字符，仍继续
	 *Input：OUT string& strWhatRead，读取到的字符串
	 *Return：void
	 *Caution：
	 */
	inline void ProcessString::readStringEndByEnter(OUT string& strWhatRead)
	{
		char cWhatRead;
		while ((cWhatRead = cin.get()) != '\n')
		{
			strWhatRead += cWhatRead;
		}
	}    //readStringEndByEnter()

	/*
	 *Name：readStringEndByMultiEnter
	 *Description：从键盘读取一行字符串，直到遇到多个 \n 为止（由用户定义个数），中间遇到其它空白字符，仍继续
	 *Input：OUT string& strWhatRead，读取到的字符串
	 *             IN const int32_t numOfEnter：终止 \n 的个数
	 *Return：void
	 *Caution：
	 */
	inline void ProcessString::readStringEndByMultiEnter(OUT string& strWhatRead, IN const int32_t numOfEnter)
	{
		char cWhatRead;
		int32_t count = 0;
		while (cWhatRead = cin.get())
		{
			if ('\n' != cWhatRead)
			{
				strWhatRead += cWhatRead;
			}
			else
			{
				count++;
				if (count >= numOfEnter)
				{
					return;
				}
			}
		}

	}    //readStringEndByMultiEnter()

	/*
	 *Name：spliceString
	 *Description：分隔 C 风格字符串
	 *Input：IN char* pcStrIn，一个 C 风格字符串
	 *             OUT vector<string>& vSubStrings，分隔后的字符串存入 vector 中
	 *             IN const char* pcSepartor，分隔符
	 *Return：void
	 *Caution：
	 */
	inline void ProcessString::spliceString(IN char* pcStrIn, OUT vector<string>& vSubStrings, IN const char* pcSepartor)
	{
		char* pSubString;

		///获取第一个子字符串
		pSubString = strtok(pcStrIn, pcSepartor);

		//继续获取其它的子字符串
		while (pSubString != NULL)
		{
			vSubStrings.push_back(pSubString);
			pSubString = strtok(NULL, pcSepartor);
		}
	}    //spliceString()

}    //sinicheveen

#endif    //PROCESS_STRING_
