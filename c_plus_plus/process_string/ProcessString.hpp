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
#include <cstring>
#include <exception>

//宏定义：参数流向
#define IN
#define OUT
#define INOUT

//使用命名
using std::string;
using std::istringstream;
using std::vector;
using std::exception;
using std::cin;

namespace sinicheveen
{
    class ProcessString
    {
    public:
    //group 1：判断型函数

        //判断字符串是不是数字
        inline static bool isNum(IN const string& rStrIn);

        //判断一个字符串是不是以某种分隔符分隔的数字字符串
        inline static bool isNumStr(IN const string& rStrIn, IN const char cDelim);

    //group 2：字符串转数字

        //字符串转数字
        template<typename T>
        inline static T strToNum(IN const string& rStrIn);

        //从数字字符串中提取出数字
        template <typename T>
        inline static bool extractNum(IN const string& rStrIn, OUT vector<T>& vExtractedNum, IN const char cDelim);

    //group 3：格式处理函数

        //删除字符串开始或结尾的一些特定字符，默认删除两端的空白字符
        enum class DEL_SIDE_E { BEGIN, END, BOTH };    //enum for delSomeCharAtBegOrEnd()
        inline static void delSomeCharAtBegOrEnd(INOUT string& rStrIn, IN const DEL_SIDE_E& enDelSide = DEL_SIDE_E::BOTH, IN const string& rPattern = " \f\n\r\t\v");

    //gruop 4：从键盘读取

        //从键盘读取一行字符串，直到遇到 n 个 \n 为止（由用户定义个数，默认为 1），中间遇到其它空白字符，仍继续
        inline static void readStrEndByEnter(OUT string& rWhatRead, IN const int numOfEnter = 1);

    //group 5：字符串分隔

        //分隔 C++ 风格字符串
        inline static void splitStr(IN const string& rStrIn, OUT vector<string>& vToken, IN const char cDelim);

        //分隔 C++ 风格字符串
        inline static void splitStr(IN const string& rStrIn, OUT vector<string>& vToken, IN const string& rDelim);

    //group 6：查找

        //find_first_not_of 的倒序查找版本
        inline static size_t rfind_first_not_of(IN const string& rStrIn, IN const string& rPattern, IN const size_t pos = string::npos);

    };    //ProcessString

    /*
    *Name：isNum
    *Description：判断字符串是不是数字
    *Input：IN const string& rStrIn，一个字符串
    *Return：T：bool，是数字返回 true，不是返回 false
    *Caution：
    */
    inline bool ProcessString::isNum(IN const string& rStrIn)
    {
        try
        {
            string::size_type pos;
            stold(rStrIn, &pos);
            if (rStrIn.length() == pos)
                return true;
            else
                return false;
        }
        catch (const exception& e)
        {
            return false;
        }
    }    //isNum()

    /*
     *Name：isNumStr
     *Description：判断一个字符串是不是以某种分隔符分隔的数字字符串
     *Input：IN const string& rStrIn，一个字符串
     *       IN const char cDelim = ','，数字之间的分隔符，默认是英文逗号
     *Return：bool，如果是返回 ture，不是则返回 false
     *Caution：
     */
    inline bool ProcessString::isNumStr(IN const string& rStrIn, IN const char cDelim)
    {
        auto findFirstNotOf = [rStrIn, cDelim](size_t endPos = 0) -> size_t
        {
            for (size_t i = endPos; i < rStrIn.size(); i++)
            {
                if (rStrIn[i] != cDelim)
                    return i;
            }
            return string::npos;
        };

        string strNum;
        size_t startPos = findFirstNotOf(0);
        size_t endPos = rStrIn.find(cDelim, startPos);
        while (startPos != string::npos)
        {
            strNum = rStrIn.substr(startPos, endPos - startPos);
            if (isNum(strNum))
            {
                startPos = findFirstNotOf(endPos);
                endPos = rStrIn.find(cDelim, startPos);
            }
            else
            {
                return false;
            }
        }

        return true;
    }    //isNumStr()

   /*
    *Name：strToNum
    *Description：字符串转数字
    *Input：IN const string& rStrIn，一个字符串
    *Return：T：转换后的数字
    *Caution：如果 rStrIn 不是数字字符串，则返回 0
    */
    template<typename T>
    inline T ProcessString::strToNum(IN const string& rStrIn)
    {
        istringstream iss(rStrIn);
        T num;
        iss >> num;
        return num;
    }    //strToNum()

    /*
     *Name：extractNum
     *Description：extractNum = extract number from string，从数字字符串中提取出数字
     *Input：IN const string& rStrIn，以分隔符分隔的一个数字字符串
     *       OUT vector<T>& vExtractedNum，提取出的数字
     *       IN const char cDelim = ','，数字之间的分隔符，默认是英文逗号
     *Return：bool，成功返回 true，失败返回 false
     *Caution：
     */
    template <typename T>
    inline bool ProcessString::extractNum(IN const string& rStrIn, OUT vector<T>& vExtractedNum, IN const char cDelim)
    {
        auto findFirstNotOf = [rStrIn, cDelim](size_t endPos = 0) -> size_t
        {
            for (size_t i = endPos; i < rStrIn.size(); i++)
            {
                if (rStrIn[i] != cDelim)
                    return i;
            }
            return string::npos;
        };

        vExtractedNum.clear();
        string strNum;
        size_t startPos = findFirstNotOf(0);
        size_t endPos = rStrIn.find(cDelim, startPos);
        while (startPos != string::npos)
        {
            strNum = rStrIn.substr(startPos, endPos - startPos);
            if (isNum(strNum))
            {
                vExtractedNum.emplace_back(strToNum<T>(strNum));
                startPos = findFirstNotOf(endPos);
                endPos = rStrIn.find(cDelim, startPos);
            }
            else
            {
                vExtractedNum.clear();
                return false;
            }
        }

        return true;
    }    //extractNum()

    /*
     *Name：delSomeCharAtBegOrEnd
     *Description：delSomeCharAtBegOrEnd = delete some characters at begin or end，删除字符串开始或结尾的一些特定字符，默认删除两端的空白字符
     *Input：INOUT string& rStrIn，一个字符串
     *       IN const DEL_SIDE_E& enDelSide，删除位置
     *       IN const string& rPattern，范例
     *Return：void
     *Caution：
     */
    inline void ProcessString::delSomeCharAtBegOrEnd(INOUT string& rStrIn, IN const DEL_SIDE_E& enDelSide, IN const string& rPattern)
    {
        if (DEL_SIDE_E::BOTH == enDelSide)
        {
            size_t startPos = rStrIn.find_first_not_of(rPattern, 0);
            if (string::npos == startPos)
            {
                rStrIn = "";
                return;
            }
            else
            {
                size_t endPos = rfind_first_not_of(rStrIn, rPattern);
                rStrIn = rStrIn.substr(startPos, endPos - startPos + 1);
            }
        }
        else if (DEL_SIDE_E::BEGIN == enDelSide)
        {
            size_t startPos = rStrIn.find_first_not_of(rPattern, 0);
            if (string::npos == startPos)
            {
                rStrIn = "";
                return;
            }
            else
            {
                rStrIn = rStrIn.substr(startPos);
            }
        }
        else    //(DEL_SIDE_E::END == enDelSide)
        {
            size_t endPos = rfind_first_not_of(rStrIn, rPattern);
            if (string::npos == endPos)
            {
                rStrIn = "";
                return;
            }
            else
            {
                rStrIn = rStrIn.substr(0, endPos + 1);
            }
        }
    }    //delSomeCharAtBegOrEnd()

    /*
     *Name：readStrEndByEnter
     *Description：从键盘读取一行字符串，直到遇到 n 个 \n 为止（由用户定义个数，默认为 1），中间遇到其它空白字符，仍继续
     *Input：OUT string& rWhatRead，读取到的字符串
     *       IN const int numOfEnter：\n 的个数
     *Return：void
     *Caution：
     */
    inline void ProcessString::readStrEndByEnter(OUT string& rWhatRead, IN const int numOfEnter)
    {
        if (1 == numOfEnter)
        {
            char cWhatRead;
            while ((cWhatRead = cin.get()) != '\n')
            {
                rWhatRead += cWhatRead;
            }
        }
        else
        {
            char cWhatRead;
            int count = 0;
            while (cWhatRead = cin.get())
            {
                if ('\n' != cWhatRead)
                {
                    rWhatRead += cWhatRead;
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
        }
    }    //readStrEndByEnter()

    /*
     *Name：splitStr
     *Description：分隔 C++ 风格字符串
     *Input：IN const string& rStrIn，一个 C++ 风格字符串
     *       OUT vector<string>& vToken，分隔后的字符串存入 vector 中
     *       IN const char cDelim，分隔符，只支持单个分隔符，e.g. “,”
     *Return：void
     *Caution：
     */
    inline void ProcessString::splitStr(IN const string& rStrIn, OUT vector<string>& vToken, IN const char cDelim)
    {
        auto findFirstNotOf = [rStrIn, cDelim](size_t endPos = 0) -> size_t
        {
            for (size_t i = endPos; i < rStrIn.size(); i++)
            {
                if (rStrIn[i] != cDelim)
                    return i;
            }
            return string::npos;
        };

        vToken.clear();
        size_t startPos = findFirstNotOf(0);
        size_t endPos = rStrIn.find(cDelim, startPos);
        while (startPos != string::npos)
        {
            vToken.emplace_back(rStrIn.substr(startPos, endPos - startPos));
            startPos = findFirstNotOf(endPos);
            endPos = rStrIn.find(cDelim, startPos);
        }
    }    //splitStr()

    /*
     *Name：splitStr
     *Description：分隔 C++ 风格字符串
     *Input：IN const string& rStrIn，一个 C++ 风格字符串
     *       OUT vector<string>& vToken，分隔后的字符串存入 vector 中
     *       IN const string& rDelim，分隔符集合，支持多个分隔符，e.g. “,-."，也支持单个分隔符，e.g. “,”
     *Return：void
     *Caution：
     */
    inline void ProcessString::splitStr(IN const string& rStrIn, OUT vector<string>& vToken, IN const string& rDelim)
    {
        vToken.clear();
        size_t startPos = rStrIn.find_first_not_of(rDelim, 0);
        size_t endPos = rStrIn.find_first_of(rDelim, startPos);
        while (startPos != string::npos)
        {
            vToken.emplace_back(rStrIn.substr(startPos, endPos - startPos));
            startPos = rStrIn.find_first_not_of(rDelim, endPos);
            endPos = rStrIn.find_first_of(rDelim, startPos);
        }
    }    //splitStr()

    /*
     *Name：rfind_first_not_of
     *Description：find_first_not_of 的倒序查找版本
     *Input：IN const string& rFindStr，要查找的字符串
     *       IN const string& rPattern，查找范例
     *       IN const size_t pos，从哪个位置开始查找，默认从末尾开始倒序查找
     *Return：void
     *Caution：
     */
    inline size_t ProcessString::rfind_first_not_of(IN const string& rFindStr, IN const string& rPattern, IN const size_t pos)
    {
        if (rFindStr.empty())
        {
            return string::npos;
        }

        size_t i;
        if (string::npos == pos)
            i = rFindStr.length() - 1;
        else
            i = pos;

        for (; i != string::npos; i--)
        {
            if (string::npos == rPattern.find(rFindStr[i]))
                return i;
        }

        return string::npos;
    }    //rfind_first_not_of()

}    //sinicheveen

#endif    //PROCESS_STRING_
