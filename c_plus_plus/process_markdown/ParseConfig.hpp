#ifndef PARSE_CONFIG_
#define PARSE_CONFIG_

/*
*Environment：
*Linux(Ubuntu), C++11，gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
*Description：
*解析配置文件，获取配置项的值
*/

#include <string>
#include <exception>
#include <memory>
#include <map>
#include <fstream>
#include <iostream>
#include <utility>
#include <algorithm>

#include "ProcessString.hpp"

//宏定义：函数参数流向
#define IN
#define OUT
#define INOUT

using std::string;
using std::exception;
using std::shared_ptr;
using std::make_shared;
using std::map;
using std::ifstream;
using std::ios_base;
using std::size_t;
using std::to_string;
using std::cout;
using std::endl;

using tripleStrMap = map<string, map<string, string>>;

namespace sinicheveen
{
    class ParseConfig
    {
    public:
    //group 1：解析、获取配置

        //解析配置文件到 map 中
        inline static void parseConfFileIntoMap();

        //输出 m_mConfs 中的配置项
        inline static void printConfInMap();

        //从 m_mConfs 中获取某个 section 下的某项配置
        inline static string getSpecConfValFromMap(IN const string& strSectionName, IN const string& strConfName);

        //从 m_mConfs 中获取某项配置，如果在多个 section 下有相同的配置名，则获取第一个 seciton 下的配置值
        inline static string getSpecConfValFromMap(IN const string& strConfName);

        //从配置文件中获取某个 section 下的某项配置
        inline static string getSpecConfValFromFile(IN const string& strSectionName, IN const string& strConfName);

        //从配置文件中获取某项配置，如果在多个 section 下有相同的配置名，则获取第一个 seciton 下的配置值
        inline static string getSpecConfValFromFile(IN const string& strConfName);

    //guoup 2：格式校验

        //校验 section name 格式，格式必须是 [XXXX]
        inline static void checkSectionNameFromat(INOUT const string& strConf, IN const uint32_t lineNum);

        //校验配置项格式
        inline static void checkConfItemFormat(INOUT const string& strConf, IN const uint32_t lineNum);

    //group 3：静态变量

        //静态变量 s_strConfFilePath 初始化或地址获取
        inline static string* s_strConfFilePath(IN const string& strConfFilePath = string{});

        //静态变量 s_mConfs 初始化或地址获取
        inline static tripleStrMap* s_mConfs(IN const tripleStrMap& mConfs = tripleStrMap{});

    };    //ParseConfig

    /*
     *Name：parseConfFileIntoMap
     *Description：解析配置文件到 m_mConfs 中
     *Input：
     *Return：
     *Caution：
     */
    inline void ParseConfig::parseConfFileIntoMap()
    {
        //获取静态变量 s_strConfFilePath 和 s_mConfs 的地址
        string* pstrConfFilePath = s_strConfFilePath();
        tripleStrMap* pmConfs = s_mConfs();
        pmConfs->clear();    //清空map

        //打开配置文件
        ifstream finConf;
        finConf.open(pstrConfFilePath->c_str(), std::ios_base::in);
        if (!finConf.is_open())    //判断文件是否打开成功
        {
            string strErrorInfo = *pstrConfFilePath + " open failed";
            throw strErrorInfo;
        }

        //逐行读取，把配置存入 map 中
        string strConf;
        uint32_t lineNum = 1;
        string strSectionName;
        while (getline(finConf, strConf))    //read one lineNum, discard \n, but keep \r
        {
            //清除行首位的空白
            ProcessString::rmSpaceInBegAndEnd(strConf);

            if (!strConf.empty())
            {
                if ('#' != strConf.at(0))    //# 开头的行为注释行
                {
                    size_t pos = strConf.find("[");
                    if (pos != string::npos)
                    {
                        //校验 section name 格式，格式必须是[XXXX]
                        checkSectionNameFromat(strConf, lineNum);

                        strSectionName = strConf;
                    }
                    else
                    {
                        // 校验配置项格式
                        checkConfItemFormat(strConf, lineNum);

                        //取出配置名和配置值，放入到对应 section 下
                        size_t pos = strConf.find("=");
                        string strConfName(strConf, 0, pos);
                        string strConfVal(strConf, pos + 1);
                        (*pmConfs)[strSectionName][strConfName] = strConfVal;
                    }
                }    //if
            }    //if

            lineNum++;

        }    //while

        finConf.close();

    }    //parseConfFileIntoMap()

    /*
     *Name：printConfInMap
     *Description：输出 m_mConfs 中的配置项
     *Input：
     *Return：
     *Caution：
     */
    inline void ParseConfig::printConfInMap()
    {
        //获取静态变量 s_mConfs 的地址
        string* pstrConfFilePath = s_strConfFilePath();
        tripleStrMap* pmConfs = s_mConfs();

        for (const auto& iSection : *pmConfs)
        {
            cout << iSection.first << endl;
            for (const auto& jConfItem : iSection.second)
            {
                cout << jConfItem.first << "=" << jConfItem.second << endl;
            }
        }
    }   //printConfInMap()

    /*
     *Name：getSpecConfValFromMap
     *Description：从 m_mConfs 中获取某个 section 下的某项配置
     *Input：IN const string& strSectionName, seciton name
     *              IN const string& strConfName，配置名
     *Return：string，配置值
     *Caution：
     */
    inline string ParseConfig::getSpecConfValFromMap(IN const string& strSectionName, IN const string& strConfName)
    {
        //获取静态变量 s_mConfs 的地址
        string* pstrConfFilePath = s_strConfFilePath();
        tripleStrMap* pmConfs = s_mConfs();

        auto pos = (*pmConfs)[strSectionName].find(strConfName);
        if (pos != (*pmConfs)[strSectionName].end())
        {
            return (*pmConfs)[strSectionName][strConfName];
        }
        else
        {
            return "";
        }
    }    //getSpecConfValFromMap()

    /*
     *Name：getSpecConfValFromMap
     *Description：从 m_mConfs 中获取某项配置，如果在多个 section 下有相同的配置名，则获取第一个 seciton 下的配置值
     *Input：IN const string& strSectionName, Section
     *              IN const string& strConfName，配置名
     *Return：string，配置值
     *Caution：
     */
    inline string ParseConfig::getSpecConfValFromMap(IN const string& strConfName)
    {
        //获取静态变量 s_mConfs 的地址
        string* pstrConfFilePath = s_strConfFilePath();
        tripleStrMap* pmConfs = s_mConfs();

        for (const auto& iSection : *pmConfs)
        {
            auto pos = iSection.second.find(strConfName);
            if (pos != iSection.second.end())
            {
                return pos->second;
            }
        }

        return "";
    }    //getSpecConfValFromMap()

    /*
     *Name：getSpecConfValFromFile
     *Description：从配置文件中获取某个 section 下的某项配置
     *              IN const string& strConfName，配置名
     *Return：string，配置值
     *Caution：
     */
    inline string ParseConfig::getSpecConfValFromFile(IN const string& strSectionName, IN const string& strConfName)
    {
        //获取静态变量 s_strConfFilePath 的地址
        string* pstrConfFilePath = s_strConfFilePath();

        //打开配置文件
        ifstream finConf;
        finConf.open(pstrConfFilePath->c_str(), std::ios_base::in);
        if (!finConf.is_open())    //判断文件是否打开成功
        {
            string strErrorInfo = *pstrConfFilePath + " open failed";
            throw strErrorInfo;
        }

        //逐行读取，寻找所需配置
        string strConf;
        uint32_t lineNum = 1;
        string strConfVal;
        while (getline(finConf, strConf))    //read one lineNum, discard \n, but keep \r
        {
            //清除行首位的空白
            ProcessString::rmSpaceInBegAndEnd(strConf);

            if (!strConf.empty())
            {
                if ('#' != strConf.at(0))    //# 开头的行为注释行
                {
                    size_t pos = strConf.find("[");
                    if (pos != string::npos)
                    {
                        //校验 section name 格式，格式必须是[XXXX]
                        checkSectionNameFromat(strConf, lineNum);

                        if (strSectionName == strConf)    //如果找到 section，则在该 section 下寻找此配置
                        {
                            while (getline(finConf, strConf))    //在该 section 下寻找此配置
                            {
                                //清除行首位的空白
                                ProcessString::rmSpaceInBegAndEnd(strConf);

                                if (!strConf.empty())
                                {
                                    if ('#' != strConf.at(0))    //# 开头的行为注释行
                                    {
                                        size_t pos = strConf.find("[");    //检验是否读到了下一个 section
                                        if (pos != string::npos)    //如果在该行找到了 [，说明已经到了下一个 section，跳出
                                        {
                                            goto LABEL_END;
                                        }
                                        else    //如果没找到说明该行不是 section name，那么就是配置项
                                        {
                                            // 校验配置项格式
                                            checkConfItemFormat(strConf, lineNum);

                                            //取出配置名，检验是否相等
                                            size_t pos = strConf.find("=");
                                            string strConfName_(strConf, 0, pos);
                                            if (strConfName == strConfName_)    //如果配置名相等，说明已经找到，取出配置值，然后跳出
                                            {
                                                string strConfVal_(strConf, pos + 1);
                                                strConfVal = strConfVal_;
                                                goto LABEL_END;
                                            }
                                        }
                                    }     //if
                                }     //if

                                lineNum++;

                            }    //while
                        }    //if
                    }    //if
                }    //if
            }    //if

            lineNum++;

        }    //while

    LABEL_END:
        //判断是否找到该配置项
        if (strConfVal.empty())
        {
            string strErrorInfo = strSectionName + " no such config item: " + strConfName;
            throw strErrorInfo;
        }

        finConf.close();

        return strConfVal;
    }    //getSpecConfValFromFile()

    /*
     *Name：getSpecConfValFromFile
     *Description：getSpecConfValFromFile = get specific config value from file
     *                     从配置文件中获取某项配置，如果在多个 section 下有相同的配置名，则获取第一个 seciton 下的配置值
     *Input：IN const string& strConfName，配置名
     *Return：string，配置值
     *Caution：
     */
    inline string ParseConfig::getSpecConfValFromFile(IN const string& strConfName)
    {
        //获取静态变量 s_strConfFilePath 的地址
        string* pstrConfFilePath = s_strConfFilePath();

        //打开配置文件
        ifstream finConf;
        finConf.open(pstrConfFilePath->c_str(), std::ios_base::in);
        if (!finConf.is_open())    //判断文件是否打开成功
        {
            string strErrorInfo = *pstrConfFilePath + " open failed";
            throw strErrorInfo;
        }

        //逐行读取，寻找配置值
        string strConf;
        string strConfVal;
        uint32_t line = 1;
        while (getline(finConf, strConf))    //read one line, discard \n, but keep \r
        {
            //清除行首位的空白
            ProcessString::rmSpaceInBegAndEnd(strConf);

            if (!strConf.empty())
            {
                if ('#' != strConf.at(0) && '[' != strConf.at(0))    //# 开头的行为注释行，[ 开头的行为 section 名
                {
                    //校验配置项格式
                    checkConfItemFormat(strConf, line);

                    size_t pos = strConf.find(strConfName);
                    if (pos != string::npos)
                    {
                        //取出配置值
                        string strConfVal_(strConf, strConf.find_first_of("=") + 1);
                        strConfVal = strConfVal_;
                        break;
                    }
                }

            }    //if

            line++;

        }    //while

        //判断是否找到该配置项
        if (strConfVal.empty())
        {
            string strErrorInfo = "no such config item: " + strConfName;
            throw strErrorInfo;
        }

        finConf.close();

        return strConfVal;
    }    //getSpecConfValFromFile()

    /*
     *Name：checkConfItemFormat
     *Description：校验 section name 格式，格式必须是 [XXXX]
     *Input：INOUT string& strConf，section name
     *            IN const uint32_t lineNum，section name 所在行
     *Return：void
     *Caution：
     */
    inline void ParseConfig::checkSectionNameFromat(INOUT const string& strConf, IN const uint32_t lineNum)
    {
        // + 校验左右括号位置
        if ('[' == *strConf.begin() && ']' == *(strConf.end() - 1))
        {

        }
        else
        {
            string strErrorInfo = "config format error at lineNum=" + to_string(lineNum) + ": " + strConf;
            throw strErrorInfo;
        }

        // + 校验左右括号个数
        uint32_t numLeftBracket = std::count_if(strConf.begin(), strConf.end(), [](const char& cElem) {return '[' == cElem; });
        uint32_t numRightBracket = std::count_if(strConf.begin(), strConf.end(), [](const char& cElem) {return ']' == cElem; });
        if (1 != numLeftBracket || 1 != numRightBracket)
        {
            string strErrorInfo = "config format error at lineNum=" + to_string(lineNum) + ": " + strConf;
            throw strErrorInfo;
        }
    }    //checkSectionNameFromat()

    /*
     *Name：checkConfItemFormat
     *Description：校验配置项格式
     *Input：INOUT string& strConf，配置项
     *            IN const uint32_t lineNum，配置项所在行
     *Return：void
     *Caution：
     */
    inline void ParseConfig::checkConfItemFormat(INOUT const string& strConf, IN const uint32_t lineNum)
    {
        //等号位置校验
        uint32_t length = strConf.length();
        size_t pos = strConf.find_first_of('=');
        if (1 <= pos && pos <= length - 2)    //等号出现的位置最小是 1，最大是 length - 2
        {

        }
        else
        {
            string strErrorInfo = "config format error at line=" + to_string(lineNum) + ": " + strConf;
            throw strErrorInfo;
        }

        // + 等号前后内容校验
        if (' ' != strConf.at(pos - 1) && ' ' != strConf.at(pos + 1))
        {

        }
        else
        {
            string strErrorInfo = "config format error at line=" + to_string(lineNum) + ": " + strConf;
            throw strErrorInfo;
        }

    }    //checkConfItemFormat()

    /*
     *Name：s_strConfFilePath
     *Description：静态变量 s_strConfFilePath 初始化或地址获取
     *Input：IN const string& strConfFilePath，配置文件路径
     *Return：string*，静态变量 s_strConfFilePath 的地址
     *Caution：参数只在第一次调用该函数时生效，后续调用无效；第一次调用该函数为初始化静态变量，后续调用为获取静态变量的地址，以便使用值或者更改值
     */
    inline string* ParseConfig::s_strConfFilePath(IN const string& strConfFilePath)
    {
        static string s_strConfFilePath{ strConfFilePath };
        return &s_strConfFilePath;
    }    //s_strConfFilePath()

    /*
     *Name：s_mConfs
     *Description：静态变量 s_mConfs 初始化或地址获取
     *Input：IN const tripleStrMap& mConfs，用于初始化 s_mConfs 的 map
     *Return：tripleStrMap*，静态变量 s_mConfs 的地址
     *Caution：参数只在第一次调用该函数时生效，后续调用无效；第一次调用该函数为初始化静态变量，后续调用为获取静态变量的地址，以便使用值或者更改值
     */
    inline tripleStrMap* ParseConfig::s_mConfs(IN const tripleStrMap& mConfs)
    {
        static tripleStrMap s_mConfs{ mConfs };
        return &s_mConfs;
    }    //s_mConfs()

}    //sinicheveen

#endif    //PARSE_CONFIG_