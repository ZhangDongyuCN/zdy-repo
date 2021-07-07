//include module header
#include <iostream>
#include <string>
#include<fstream>
#include <dirent.h>
#include <vector>
#include <regex>
#include <chrono>
#include <cstdio>

//include module header
#include "operatingMode.h"
#include "sqlite3pp.h"
#include "ParseConfig.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::exception;
using std::string;
using std::to_string;
using std::ifstream;
using std::ios_base;
using std::ofstream;
using std::vector;
using std::back_inserter;
using std::regex;
using std::regex_replace;
using std::regex_search;
using sqlite3pp::database;

using ParseConfig = sinicheveen::ParseConfig;

using namespace std::chrono;

//数据库还不存在，新建数据库
void createDatabase()
{
    cout << "createDatabase() ..." << endl << endl;

    try
    {
        //读取配置
        string strDatabasePath = ParseConfig::getSpecConfValFromMap("[PATH]", "DATABASE_FILE_PATH");

        //创建数据库、创建表、建立索引
        database db(strDatabasePath.c_str());
        db.execute("CREATE TABLE onedrive (name CHAR(100) PRIMARY KEY, id CHAR(20), authkey CHAR(20), url CHAR(200), markdownImgUrl CHAR(200) )");
        db.execute("CREATE INDEX index_name ON onedrive  (name)");
        db.execute("CREATE INDEX index_id ON onedrive  (id)");

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //createDatabase()

//向数据库中插入图片的 OneDrive 直链信息
void insertImgInfoToDatabaseOneDrive()
{
    cout << "insertImgInfoToDatabaseOneDrive() ..." << endl << endl;

    try
    {
        //读取配置
        string strDatabasePath = ParseConfig::getSpecConfValFromMap("[PATH]", "DATABASE_FILE_PATH");
        string strImgFilePath = ParseConfig::getSpecConfValFromMap("[PATH]", "URL_FILE_PATH");

        //打开数据库
        database db(strDatabasePath.c_str());

        //打开文件操作
        ifstream finMarkdownImgUrl;
        finMarkdownImgUrl.open(strImgFilePath.c_str(), ios_base::in);
        string strBuf;
        while (getline(finMarkdownImgUrl, strBuf))
        {
            //取出信息
            string strName(strBuf, strBuf.find("![") + 2, strBuf.find("]") - strBuf.find("![") - 2);
            string strID(strBuf, strBuf.find("storage.live.com/items/") + 23, strBuf.rfind(":/") - strBuf.find("storage.live.com/items/") - 23);
            string strAuthkey(strBuf, strBuf.find("?authkey=") + 9, strBuf.find(")") - strBuf.find("?authkey=") - 9);
            string strUrl(strBuf, strBuf.find("(") + 1, strBuf.find(")") - strBuf.find("(") - 1);
            string strMarkdownImgUrl(strBuf, strBuf.find("!["), strBuf.find(")") - strBuf.find("![") + 1);
            strBuf.erase();

            //把信息插入数据库
            string strValue = "( '" + strName + "','" + strID + "','" + strAuthkey + "','" + strUrl + "','" + strMarkdownImgUrl + "' )";
            string strSQL = "INSERT INTO onedrive VALUES " + strValue;
            db.execute(strSQL.c_str());
        }
        finMarkdownImgUrl.close();    //关闭文件

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //insertImgInfoToDatabase()

//替换 Markdown 文章图片链接为 OneDrive 直链
void replaceImgUrlOneDrive()
{
    cout << "replaceImgUrlOneDrive() ..." << endl << endl;

    try
    {
        //读取配置
        string strDatabasePath = ParseConfig::getSpecConfValFromMap("[PATH]", "DATABASE_FILE_PATH");
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //打开数据库
        database db(strDatabasePath.c_str());

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //替换文章中的图片链接
            auto startPos = strArticle.find("![");
            while (startPos != string::npos)
            {
                //获取文章中图片的名字
                auto endPos = strArticle.find(")", startPos);
                string strImgUrl(strArticle, startPos, endPos + 1 - startPos);
                string strImgName(strImgUrl, strImgUrl.find("![") + 2, strImgUrl.find("]") - strImgUrl.find("![") - 2);

                //根据名字从数据库查询出对应的链接
                string strSQLstatement = "SELECT markdownImgUrl FROM onedrive WHERE name = '" + strImgName + "'";
                sqlite3pp::query queryResult(db, strSQLstatement.c_str());
                auto ptr = queryResult.begin();
                const char* cMarkdownImgUrl = (*ptr).get<char const*>(0);
                if (nullptr != cMarkdownImgUrl)
                {
                    //用查询出的链接替换文章中现有图片的链接
                    strArticle.replace(startPos, endPos + 1 - startPos, cMarkdownImgUrl);
                }

                //开始下一次查找
                startPos = strArticle.find("![", startPos + 2);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //replaceMarkdownImg()

//在 Markdown 文章图片链接中添加图片描述
//即 ![在这里插入图片描述](1.img/20190605202304965.png) --> ![20190605202304965.png](1.img/20190605202304965.png)
void addImgDescription()
{
    cout << "addImgDescription() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //给图片链接添加图片描述
            auto startPos = strArticle.find("![");
            while (startPos != string::npos)
            {
                //获取文章中图片的名字
                auto endPos = strArticle.find(")", startPos);
                string strImgUrl(strArticle, startPos, endPos + 1 - startPos);
                string strImgName(strImgUrl, strImgUrl.find_last_of("/") + 1, strImgUrl.find(")") - strImgUrl.find_last_of("/") - 1);

                //插入图片描述，图片描述就是图片名字
                auto startPos1 = strImgUrl.find("![");
                auto endPos1 = strImgUrl.find("]");
                strImgUrl.replace(startPos1 + 2, endPos1 - startPos1 - 2, strImgName);

                //用新的图片链接替换文章中现有图片的链接
                strArticle.replace(startPos, endPos + 1 - startPos, strImgUrl);

                //开始下一次查找
                startPos = strArticle.find("![", startPos + 2);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //addImgDescription()

//在 Markdown 文章换行符前边添加两个空格，代码区不添加
void addTwoSpaceBeforeEachEnter()
{
    cout << "addTwoSpaceBeforeEachEnter() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            ofstream foutArticle;
            auto pos = iArticleFullPath.find_last_of(".");
            string strTempFile(iArticleFullPath, 0, pos);
            strTempFile += "_New";
            foutArticle.open(strTempFile.c_str(),  ios_base::out | ios_base::app);

            //逐行读取并替换
            string strBuf, strBufNew;
            bool bCodeFieldIn = false;    //是否进入了代码区
            bool bCodeFieldOut = true;    //是否出了代码区
            while (getline(finArticle, strBuf))    //read a line, discard \n
            {
                strBuf += "\n";

                if (string::npos != strBuf.find("```"))
                {
                    //初次遇见 ```，说明进入了代码区，把变量置为进入代码区的状态
                    if (false==bCodeFieldIn && true==bCodeFieldOut)
                    {
                        bCodeFieldIn = true;
                        bCodeFieldOut = false;

                        foutArticle << strBuf;
                        continue;
                    }
                    else    //再次遇见 ```，说明出了代码区，把变量置为出代码区的状态
                    {
                        bCodeFieldIn = false;
                        bCodeFieldOut = true;

                        foutArticle << strBuf;
                        continue;
                    }
                }

                //如果没在代码区，\r\n 之前加两个空格
                if (false == bCodeFieldIn && true == bCodeFieldOut)
                {
                    auto pos = strBuf.find("\r\n");
                    if (string::npos != pos)
                    {
                        strBuf.insert(pos, "  ");
                        foutArticle << strBuf;
                    }
                    else    //最后一行为 xxx\n
                    {
                        strBuf.insert(strBuf.length() - 1, "  \r");
                        foutArticle << strBuf;
                    }
                }
                else    //如果在代码区则直接写入文件，不追加空格
                {
                    foutArticle << strBuf;
                }

            }

            finArticle.close();
            foutArticle.close();

            remove(iArticleFullPath.c_str());
            rename(strTempFile.c_str(), iArticleFullPath.c_str());

        }    //for

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //addTwoSpaceBeforeEachEnter()

//有颜色字体改为加粗
//<font color=XXX>XXX</font> --> XXX
void clearFontColor()
{
    cout << "clearFontColor() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //清除字体颜色
            auto startPos = strArticle.find("<font");
            while (startPos != string::npos)
            {
                try
                {
                    auto endPos = strArticle.find(">", startPos);
                    strArticle.erase(strArticle.begin() + startPos, strArticle.begin() + endPos + 1);
                    startPos = strArticle.find("</font>");
                    strArticle.erase(startPos, 7);
                }
                catch (const exception & e)
                {
                    string strErrorInfo = "error in: " + iArticleFullPath;
                    cout << strErrorInfo << endl << endl;
                    return;
                }
                catch (...)
                {
                    string strErrorInfo = "error in: " + iArticleFullPath;
                    cout << strErrorInfo << endl << endl;
                    return;
                }

                //开始下一次查找
                startPos = strArticle.find("<font", startPos);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //clearFontColor()

//<b>XXX</b> --> **XXX**
void changeBoldGrammarToAsterisk()
{
    cout << "changeBoldGrammarToAsterisk() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //<b>XXX</b>  --> **XXX**
            auto startPos = strArticle.find("<b>");
            while (startPos != string::npos)
            {
                try
                {
                    strArticle.erase(startPos, 3);
                    strArticle.insert(startPos, "**");
                    startPos = strArticle.find("</b>", startPos + 2);
                    strArticle.erase(startPos, 4);
                    strArticle.insert(startPos, "**");
                }
                catch (const exception & e)
                {
                    string strErrorInfo = "error in: " + iArticleFullPath;
                    cout << strErrorInfo << endl << endl;
                    return;
                }
                catch (...)
                {
                    string strErrorInfo = "error in: " + iArticleFullPath;
                    cout << strErrorInfo << endl << endl;
                    return;
                }

                //开始下一次查找
                startPos = strArticle.find("<b>", startPos + 2);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //changeBoldGrammarToAsterisk()

//**XXX** --> <b>XXX</b>
void changeBoldGrammarToHtmlFormat()
{
    cout << "changeBoldGrammarToHtmlFormat() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //<b>XXX</b>  --> **XXX**
            auto startPos = strArticle.find("**");
            while (startPos != string::npos)
            {
                try
                {
                    strArticle.erase(startPos, 2);
                    strArticle.insert(startPos, "<b>");
                    startPos = strArticle.find("**", startPos + 3);
                    strArticle.erase(startPos, 2);
                    strArticle.insert(startPos, "</b>");
                }
                catch (const exception & e)
                {
                    string strErrorInfo = "error in: " + iArticleFullPath;
                    cout << strErrorInfo << endl << endl;
                    return;
                }
                catch (...)
                {
                    string strErrorInfo = "error in: " + iArticleFullPath;
                    cout << strErrorInfo << endl << endl;
                    return;
                }

                //开始下一次查找
                startPos = strArticle.find("**", startPos + 4);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //changeBoldGrammarToHtmlFormat()

//添加打赏码
void addRewardCode()
{
    cout << "addRewardCode() ..." << endl << endl;

    //读取配置
    string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

    //读取路径下所有的文件名字
    vector<string> vArticleFullPath;
    readFileNameInDir(strArticleDir, vArticleFullPath);

    //遍历所有文件
    for (auto& iArticleFullPath : vArticleFullPath)
    {
        //添加打赏码
        ofstream foutArticle;
        foutArticle.open(iArticleFullPath.c_str(), ios_base::out | ios_base::app);
        foutArticle << "\r\n------\r\n**打赏作者：**\r\n![Scr_20191004_120920.png](http://storage.live.com/items/E13D069FCADA12DD!186015:/Scr_20191004_120920.png?authkey=AB14uJSpRB2SSNY)\r\n";
        foutArticle.close();
    }

    cout << "done!" << endl;
}    //addRewardCode()

//删除打赏码
void deleteRewardCode()
{
    cout << "deleteRewardCode() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //string 
        string numOfHorizontalLine;
        int ifHaveHorizontalLine, howMany;
        cout << "if there are horizontal line befor \"打赏作者\"? input: y/n" << endl;
        //判断输入是否非法，输入正确则跳出继续，否则重新输入，直到输入正确
        while (1)
        {
            cin >> numOfHorizontalLine;
            if ("y" == numOfHorizontalLine)
            {
                ifHaveHorizontalLine = 1;
                cout << endl << "please input there are how many horizontal line" << endl;
                while (1)
                {
                    cin >> howMany;
                    if (howMany >= 1)
                        break;
                    else
                        cout << endl << "input error, please reinput there are how many horizontal line" << endl;
                }
            }
            else if ("n" == numOfHorizontalLine)
                ifHaveHorizontalLine = 2;
            else
                ifHaveHorizontalLine = 3;

            if (3 == ifHaveHorizontalLine)
                cout << endl << "input error, please reinput: y/n" << endl;
            else
                break;
        }
        cout << endl;

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //删除打赏码
            if (1 == ifHaveHorizontalLine)
            {
                string strTotalHorizontalLine(howMany, '-');
                auto startPos = strArticle.find_last_of("打赏作者");
                if (startPos != string::npos)
                {
                    startPos = strArticle.rfind(strTotalHorizontalLine, startPos);
                    strArticle.erase(startPos);
                }
            }
            else
            {
                auto startPos = strArticle.find_last_of("打赏作者");
                if (startPos != string::npos)
                {
                    startPos = strArticle.rfind("\r\n", startPos);
                    strArticle.erase(startPos);
                }
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //deleteRewardCode()

//添加文章推送头
void addPostHeader()
{
    cout << "addPostHeader() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //获取文章名，也即是文件名字（没有后缀）
            string strArticleName(iArticleFullPath, iArticleFullPath.find_last_of("/") + 1, iArticleFullPath.find_last_of(".") - iArticleFullPath.find_last_of("/") - 1);

            //判断文章名的开头是不是形如 2019-10-10- 这样的格式
            bool bFlag;
            for (int i = 0; i < 11; i++)
            {
                if (i == 4 || i == 7 || i == 10)
                {
                    if ('-' != strArticleName.at(i))
                    {
                        bFlag = false;
                        break;
                    }
                }
                else
                {
                    if (strArticleName.at(i) < '0' && strArticleName.at(i) > '9')
                    {
                        bFlag = false;
                        break;
                    }
                }
            }

            //添加文章推送头
            if (true == bFlag)
            {
                string strDate = strArticleName.substr(0, 10);
                string strTitle = strArticleName.substr(11, strArticleName.length() - 11);
                string strPostHeader = "---\r\nlayout:     post\r\ntitle:      " + strTitle + "\r\nsubtitle:   \r\ndate:       " + strDate + "\r\nauthor:     浅沫\r\nheader-img: \r\ncatalog: true\r\ntags:\r\n    - XX\r\n    - XX\r\n---\r\n\r\n";
                strArticle.insert(0, strPostHeader);
            }
            else
            {
                string strPostHeader = "---\r\nlayout:     post\r\ntitle:      " + strArticleName + "\r\nsubtitle:   \r\ndate:       2019-XX-XX\r\nauthor:     浅沫\r\nheader-img: \r\ncatalog: true\r\ntags:\r\n    - XX\r\n    - XX\r\n---\r\n\r\n";
                strArticle.insert(0, strPostHeader);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //addPostHeader()

//删除文章推送头
void delPostHeader()
{
    cout << "delPostHeader() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //删除文章推送头
            auto firstPos = strArticle.find("---");
            auto secondPos = strArticle.find("---", firstPos + 3);
            strArticle.erase(strArticle.begin(), strArticle.begin() + secondPos + 7);

            //把删除文章推送头的文章重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //delPostHeader()

//添加马克飞象头
void addMaXiangHeader()
{
    cout << "addMaXiangHeader() ..." << endl << endl;

    try
    {
        //输入笔记本信息和标签信息 e.g. @(3.2 技术问题)[tech:office, tech:latex]
        string strNoteBookInfo;
        cout << "please input notebook and label info, .e.g. @(3.2 技术问题)[tech:office, tech:latex]" << endl;
        cin.ignore(100000, '\n');    //清空输入缓冲区
        getline(cin, strNoteBookInfo);

        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //获取文章名，也即是文件名字（没有后缀）
            string strArticleName(iArticleFullPath, iArticleFullPath.find_last_of("/") + 1, iArticleFullPath.find_last_of(".") - iArticleFullPath.find_last_of("/") - 1);

            //判断文章名的开头是不是形如 2019-10-10- 这样的格式
            bool bFlag;
            for (int i = 0; i < 11; i++)
            {
                if (i == 4 || i == 7 || i == 10)
                {
                    if ('-' != strArticleName.at(i))
                    {
                        bFlag = false;
                        break;
                    }
                }
                else
                {
                    if (strArticleName.at(i) < '0' && strArticleName.at(i) > '9')
                    {
                        bFlag = false;
                        break;
                    }
                }
            }

            //添加文章推送头
            if (true == bFlag)
            {
                strArticleName.erase(0, 11);    //清除日期 2019-10-10-
                string strMaXiangHeader = "### " + strArticleName + ".md\r\n\r\n------\r\n" + strNoteBookInfo + "\r\n\r\n";
                strArticle.insert(0, strMaXiangHeader);
            }
            else
            {
                string strMaXiangHeader = "### " + strArticleName + ".md\r\n\r\n------\r\n" + strNoteBookInfo + "\r\n\r\n";
                strArticle.insert(0, strMaXiangHeader);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << endl << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //addMaXiangHeader()

//替换 Markdown 文章图片链接为阿里云对象存储 OSS 链接（其它云也可以，更改配置文件即可）
void replaceImgUrlOSS()
{
    cout << "replaceImgUrlOSS() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");
        string strOSSUrlPrefix = ParseConfig::getSpecConfValFromMap("[PATH]", "OSS_URL_PREFIX");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //替换文章中的图片链接
            auto startPos = strArticle.find("![");
            while (startPos != string::npos)
            {
                //获取文章中图片的名字
                auto endPos = strArticle.find(")", startPos);
                string strImgUrl(strArticle, startPos, endPos + 1 - startPos);
                string strImgName(strImgUrl, strImgUrl.find("![") + 2, strImgUrl.find("]") - strImgUrl.find("![") - 2);
                string strOSSUrl = "![" + strImgName + "](" + strOSSUrlPrefix + strImgName + ")";
                strArticle.replace(startPos, endPos + 1 - startPos, strOSSUrl);

                //开始下一次查找
                startPos = strArticle.find("![", startPos + 2);
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //replaceImgUrlOSS()

//删除换行符前边的空白字符
void delSpaceBeforeEachEnter()
{
    cout << "delSpaceBeforeEachEnter() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            ofstream foutArticle;
            auto pos = iArticleFullPath.find_last_of(".");
            string strTempFile(iArticleFullPath, 0, pos);
            strTempFile += "_New";
            foutArticle.open(strTempFile.c_str(), ios_base::out | ios_base::app);

            //逐行读取并删除末尾空格
            string strBuf;
            while (getline(finArticle, strBuf))    //read a line, discard \n
            {
                regex e("(\\s+\\r)|(\\s+$)");
                foutArticle << regex_replace(strBuf, e, "\r\n");
            }

            finArticle.close();
            foutArticle.close();

            remove(iArticleFullPath.c_str());
            rename(strTempFile.c_str(), iArticleFullPath.c_str());

        }    //for

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //delSpaceBeforeEachEnter()

//更改图片名字
void renameImg()
{
    cout << "renameImg() ..." << endl << endl;

    try
    {
        //读取配置
        string strDatabasePath = ParseConfig::getSpecConfValFromMap("[PATH]", "DATABASE_FILE_PATH");
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");
        string strImgDir = ParseConfig::getSpecConfValFromMap("[PATH]", "IMAGE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //获取当前时间戳（毫秒级别）
        int64_t millSecSinceEpoch = getMillSecSinceEpoch();

        //打开数据库
        database db(strDatabasePath.c_str());

        //遍历所有文件
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            try
            {
                //替换文章中的图片名字
                auto startPos = strArticle.find("![");
                while (startPos != string::npos)
                {
                    //获取文章中图片的名字
                    auto endPos = strArticle.find(")", startPos);
                    string strImgUrl(strArticle, startPos, endPos + 1 - startPos);
                    string strImgName(strImgUrl, strImgUrl.find("![") + 2, strImgUrl.find("]") - strImgUrl.find("![") - 2);

                    //判断是否有中文字符、横线、下划线等
                    string strImgName_New, strSQL, strImgFileOldNameFullPath, strImgFileNewNameFullPath;
                    size_t posSuffix;
                    regex e("([\u4e00-\u9fa5_-])|(Scr)");
                    if (regex_search(strImgName, e) || strImgName.find(".") < 13)
                    {
                        //「时间戳 + 后缀」组成新的名字
                        posSuffix = strImgName.find(".");
                        if (string::npos == posSuffix)
                        {
                            goto nextFind;
                        }
                        string strSuffix(strImgName, posSuffix);
                        strImgName_New = to_string(millSecSinceEpoch) + strSuffix;
                        millSecSinceEpoch++;

                        //更改数据库中图片的名字和相关信息
                        strSQL = " SELECT id, authkey FROM onedrive WHERE name = '" + strImgName + "'";
                        sqlite3pp::query queryResult(db, strSQL.c_str());
                        auto ptr = queryResult.begin();
                        string id, authkey;
                        (*ptr).getter() >> id >> authkey;
                        string url = "http://storage.live.com/items/" + id + ":/" + strImgName_New + "?authkey=" + authkey;
                        string markdownImgUrl = "![" + strImgName_New + "](" + url + ")";
                        strSQL = " UPDATE onedrive SET name = '" + strImgName_New + "', url = '" + url + "', markdownImgUrl = '" + markdownImgUrl + "' WHERE name = '" + strImgName + "'";
                        db.execute(strSQL.c_str());

                        //更改文章中图片链接信息
                        strArticle.replace(startPos, endPos + 1 - startPos, markdownImgUrl);

                        //更改图片文件的文件名
                        strImgFileOldNameFullPath = strImgDir + "/" + strImgName;
                        strImgFileNewNameFullPath = strImgDir + "/" + strImgName_New;
                        rename(strImgFileOldNameFullPath.c_str(), strImgFileNewNameFullPath.c_str());
                    }

                    nextFind:
                    //开始下一次查找
                    startPos = strArticle.find("![", startPos + 2);
                }
            }
            catch (const exception & e)
            {
                string strErrorInfo = "error in: " + iArticleFullPath;
                cout << strErrorInfo << endl << endl;
                return;
            }
            catch (...)
            {
                string strErrorInfo = "error in: " + iArticleFullPath;
                cout << strErrorInfo << endl << endl;
                return;
            }

            //重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << strArticle;
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //renameImg()

//正则搜索
void regexSearch()
{
    cout << "regexSearch() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //输入搜索正则表达式
        string strSearchRegex;
        cout << "please input the regex expression which you want to search: " << endl;
        cin >> strSearchRegex;
        cout << endl;

        //遍历所有文件搜索
        vector<string>vFileNames;
        regex e(strSearchRegex);
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //搜索
            if (regex_search(strArticle, e))
            {
                vFileNames.push_back(iArticleFullPath);
            }

        }

        //输出包含关键字的文件
        for (const auto& iFileName : vFileNames)
        {
            cout << iFileName << endl;
        }

        cout << endl << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //regexSearch()

//正则替换
void regexReplace()
{
    cout << "regexReplace() ..." << endl << endl;

    try
    {
        //读取配置
        string strArticleDir = ParseConfig::getSpecConfValFromMap("[PATH]", "ARTICLE_DIR");

        //读取路径下所有的文件名字
        vector<string> vArticleFullPath;
        readFileNameInDir(strArticleDir, vArticleFullPath);

        //输入查找替换内容
        string strSearchRegex, strReplaceCont;
        cout << "please input the regex expression which you want to search: " << endl;
        cin >> strSearchRegex;
        cout << endl << endl;
        cout << "please input the replace content: " << endl;
        cin >> strReplaceCont;
        cout << endl;

        //遍历所有文件替换
        regex e(strSearchRegex);
        for (auto& iArticleFullPath : vArticleFullPath)
        {
            //读取文件的内容
            ifstream finArticle;
            finArticle.open(iArticleFullPath.c_str(), ios_base::in);
            string strArticle;
            string strBuf;
            while (getline(finArticle, strBuf))
            {
                strBuf += '\n';
                strArticle += strBuf;
                strBuf.erase();
            }
            finArticle.close();    //关闭文件

            //替换后重新覆盖写入原文件
            ofstream foutArticle;
            foutArticle.open(iArticleFullPath.c_str(), ios_base::trunc);
            foutArticle << regex_replace(strArticle, e, strReplaceCont);
            foutArticle.close();
        }

        cout << "done!" << endl;
    }
    catch (const exception & e)
    {
        cout << e.what() << endl;
        return;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        return;
    }
}    //regexReplace()

//读取指定文件夹下所有文件的名字
void readFileNameInDir(IN string strDir, INOUT vector<string>& vFileFullPath)
{
    struct dirent* pDirent;
    DIR* pDir = opendir(strDir.c_str());
    if (pDir != NULL) 
    {
        while ((pDirent = readdir(pDir)) != NULL)
        {
            string strFileName = pDirent->d_name;
            string strFileFullPath = strDir + "/" + strFileName;
            vFileFullPath.push_back(strFileFullPath);
        }
        vFileFullPath.erase(vFileFullPath.begin(), vFileFullPath.begin() + 2);    //前两个存储的是当前路径和上一级路径，所以要删除
    }
}    //readFileNameInDir()

//获取时间戳（毫秒级别）
int64_t getMillSecSinceEpoch()
{
    system_clock::time_point currentTime_tp = system_clock::now();
    system_clock::duration timeSinceEpoch_du = currentTime_tp.time_since_epoch();
    int64_t millSecSinceEpoch = duration_cast<milliseconds>(timeSinceEpoch_du).count();

    return millSecSinceEpoch;
}    //getMillSecSinceEpoch()