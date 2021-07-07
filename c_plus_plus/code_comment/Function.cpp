#include "Function.h"

void parseConfig(OUT json& jsConf)
{
    //从配置文件读取数据
    ifstream fin("./config.json", ifstream::in | ifstream::binary);
    if (!fin.is_open())
    {
        throw runtime_error("./config.json open failed");
    }
    fin.seekg(0, fin.end);
    uint64_t udlLen = fin.tellg();
    fin.seekg(0, fin.beg);
    char* npBuffer = new char[udlLen];
    fin.read(npBuffer, udlLen);
    if (fin)
    {
        //nothing to do
    }
    else
    {
        delete[] npBuffer;
        throw runtime_error("read file error");
    }
    fin.close();

    //正则替换掉注释部分，/*xxxx*/ 为注释内容
    string rConf(npBuffer, udlLen);
    delete[] npBuffer;
    regex find("/\\*.*?\\*/");    //非贪婪匹配
    string rAfterReplace;
    regex_replace(back_inserter(rAfterReplace), rConf.begin(), rConf.end(), find, "");
    rConf = move(rAfterReplace);
    rAfterReplace.clear();

    //把读取到的数据解析到 json 中
    jsConf = json::parse(rConf);
}    //parseConfig()

bool ignorePath(IN json& jsConf, OUT string& rIgnorePathShellCmdPart1, OUT string& rIgnorePathShellCmdPart2)
{
    rIgnorePathShellCmdPart1 = "\\(";
    if (!jsConf["ignore_path"].empty())
    {
        for (auto& iIgnorePath : jsConf["ignore_path"])
        {
            rIgnorePathShellCmdPart1 += " -path " + string(iIgnorePath) + " -o";
            rIgnorePathShellCmdPart2 += " | grep -v " + string(iIgnorePath);
        }
        rIgnorePathShellCmdPart1 = rIgnorePathShellCmdPart1.substr(0, rIgnorePathShellCmdPart1.length() - 3);
        rIgnorePathShellCmdPart1 += " \\) -prune -o";

        return true;
    }
    else
    {
        return false;
    }
}    //ignorePath()

const char* exeShellCommand(IN const string& rCommmand)
{
    FILE* pFile = NULL;
    static char szBuffer[BUFFER_LEN] = { '0' };
    pFile = popen(rCommmand.c_str(), "r");
    if (NULL == pFile)
    {
        throw runtime_error("exe shell command error");
    }
    if (NULL == fgets(szBuffer, sizeof(szBuffer), pFile))
    {
        pclose(pFile);
        return "";
    }
    else
    {
        pclose(pFile);
        szBuffer[strlen(szBuffer) - 1] = '\0';    //去掉末尾的 linux 换行符
        return szBuffer;
    }
}    //exeShellCommand()

void exeShellCommand(IN const string& rCommmand, OUT vector<string>& vExeResult)
{
    FILE* pFile = NULL;
    char szBuffer[BUFFER_LEN] = { '0' };
    pFile = popen(rCommmand.c_str(), "r");
    if (NULL == pFile)
    {
        throw runtime_error("exe shell command error");
        return;
    }
    while (NULL != fgets(szBuffer, sizeof(szBuffer), pFile))
    {
        szBuffer[strlen(szBuffer) - 1] = '\0';    //去掉末尾的 linux 换行符（\n）
        vExeResult.emplace_back(szBuffer);
    }
    pclose(pFile);
}    //exeShellCommand()

void toCRLF(INOUT string& rContent)
{
    string rAfterReplace;

    //先把 \r\n 替换为 \n
    regex find("\\r\\n");
    regex_replace(back_inserter(rAfterReplace), rContent.begin(), rContent.end(), find, "\\n");
    rContent = move(rAfterReplace);
    rAfterReplace.clear();

    //再把 \n 替换为 \r\n
    find = "\\n";
    regex_replace(back_inserter(rAfterReplace), rContent.begin(), rContent.end(), find, "\\r\\n");
    rContent = move(rAfterReplace);
}    //toCRLF()

void toLF(INOUT string& rContent)
{
    string rAfterReplace;

    //\r\n 替换为 \n
    regex find("\\r\\n");
    regex_replace(back_inserter(rAfterReplace), rContent.begin(), rContent.end(), find, "\\n");
    rContent = move(rAfterReplace);
}    //toLF()

void tabToSpace(INOUT string& rContent)
{
    string rAfterReplace;

    //制表符替换为四个空格
    regex find("\\t");
    regex_replace(back_inserter(rAfterReplace), rContent.begin(), rContent.end(), find, "    ");
    rContent = move(rAfterReplace);
}    //tabToSpace()

string calSpace(IN json& jsConf, IN const string& rContent)
{
    int spaceNum = 0;
    size_t pos = rContent.find(jsConf["comment_start_flag"].get<string>());    //定位注释开始位置，e.g. Func Name:
    if (string::npos != pos && 0 != pos)
    {
        //定位注释开始位置前边 /****** 的位置
        while ('/' != rContent.at(pos))
        {
            pos--;
            if (string::npos == pos)
            {
                continue;
            }
        }

        //计算 /****** 到文件边界有几个空格
        if (pos > 0)
        {
            pos--;
            while (string::npos != pos && ' ' == rContent.at(pos))
            {
                spaceNum++;
                pos--;
            }
        }
    }

    return string(spaceNum, ' ');
}    //calSpace()

bool extractComment(IN const size_t pos, IN const string& rContent, IN const string& rCommentEndFlag, OUT string& rComment)
{
    //定位注释的结束位置
    size_t comEndPos = rContent.find(rCommentEndFlag, pos);
    if (string::npos == comEndPos)
    {
        return false;
    }
    comEndPos = rContent.find('\n', comEndPos);
    if (string::npos == comEndPos)
    {
        return false;
    }
    comEndPos++;

    //取出注释内容
    rComment = string(rContent.begin() + pos, rContent.begin() + comEndPos);

    //替换行首空格
    regex find("^ +");
    string rAfterReplace;
    regex_replace(back_inserter(rAfterReplace), rComment.begin(), rComment.end(), find, "");
    rComment = move(rAfterReplace);
    rAfterReplace.clear();
    find = "(\\n)( +)";
    regex_replace(back_inserter(rAfterReplace), rComment.begin(), rComment.end(), find, "$1");
    rComment = move(rAfterReplace);
    rAfterReplace.clear();

    //加 /* * */
    rComment.insert(0, "/*\n");    //行首加 /*
    find = "(\\n)";
    regex_replace(back_inserter(rAfterReplace), rComment.begin(), rComment.end(), find, "$1*");    //中间内容加 *
    rComment = move(rAfterReplace);
    rAfterReplace.clear();
    rComment += "/\n";    //行尾加 */

    return true;
}    //extractComment()

bool extractFunDeclar(IN const size_t pos, IN const string& rContent, OUT string& rFunDeclar)
{
    //获取函数名字的末尾位置
    size_t pos2 = rContent.find("*/", pos);    //定位注释结束位置
    if (string::npos == pos2)
    {
        return false;
    }
    pos2 = rContent.find('(', pos2);    //定位函数参数列表开始括号
    if (string::npos == pos2)
    {
        return false;
    }
    size_t bracketStartPos = pos2;    //记住该位置
    size_t bracketEndPos = rContent.find(')', pos2);    //定位函数参数列表结束括号
    if (string::npos == bracketEndPos)
    {
        return false;
    }
    pos2--;
    while (' ' == rContent.at(pos2))    //跳过函数名字和函数参数列表开始括号之间的空格， e.g. fun    (int a, int b)
    {
        pos2--;
    }
    
    //获取函数名字
    while (' ' != rContent.at(pos2) && ':' != rContent.at(pos2))
    {
        rFunDeclar.insert(rFunDeclar.begin(), rContent.at(pos2));
        pos2--;
    }

    //添加参数列表
    rFunDeclar = rFunDeclar + string(rContent.begin() + bracketStartPos, rContent.begin() + bracketEndPos + 1);

    return true;
}    //extractFunDeclar()

void insertCommentForFunDeclar(IN const string& rFunDeclar, IN const string& rComment, INOUT string& rContent)
{
    size_t funPos = rContent.find(rFunDeclar);
    regex find("(\\n)");
    string rAfterReplace;
    while (string::npos != funPos)    //循环查找，针对模板函数可能用了 enable_if 实现了重载，而重载的函数名和参数可能都一样
    {
        //函数声明到文件边界有多少空格
        size_t funDeclarPos = rContent.rfind('\n', funPos);
        int spaceNum = 0;
        if (string::npos == funDeclarPos)
        {
            if (string::npos != rContent.rfind("*/", funPos))    //如果改函数已经添加过注释，就不需要再添加了，继续下一次查找
            {
                funPos = rContent.find(rFunDeclar, funPos + 1);
                continue;
            }
            spaceNum = rContent.find_first_not_of(' ', 0);
        }
        else
        {
            if ('/' == rContent.at(funDeclarPos - 1) && '*' == rContent.at(funDeclarPos - 2))    //如果改函数已经添加过注释，就不需要再添加了，继续下一次查找
            {
                funPos = rContent.find(rFunDeclar, funPos + 1);
                continue;
            }
            spaceNum = rContent.find_first_not_of(' ', funDeclarPos + 1) - funDeclarPos - 1;
        }
        string rSpace(spaceNum, ' ');

        //注释前边添加相应个数的空格
        rAfterReplace.clear();
        regex_replace(back_inserter(rAfterReplace), rComment.begin(), rComment.end(), find, "$1" + rSpace);
        rAfterReplace = rAfterReplace.substr(0, rAfterReplace.length() - spaceNum);    //去除注释后边多加的空格
        rAfterReplace.insert(0, rSpace);    //注释第一行前加空格

        //插入注释
        if (string::npos == funDeclarPos)
        {
            rContent.insert(0, rAfterReplace);
            funPos += rAfterReplace.length();
        }
        else
        {
            rContent.insert(funDeclarPos + 1, rAfterReplace);
            funPos += rAfterReplace.length();
        }

        //进行下一次查找
        funPos = rContent.find(rFunDeclar, funPos + 1);
    }
}    //insertCommentForFunDeclar()

void addCommentInH(IN json& jsConf, IN const string& rCppPath, IN const string& rCppContent)
{
    //注释开始标志和结束标志
    string rCommentStartFlag = jsConf["comment_start_flag"].get<string>();
    string rCommentEndFlag = jsConf["comment_end_flag"].get<string>();

    //提取文件名字
    size_t startPos = rCppPath.rfind('/');
    size_t endPos = rCppPath.rfind('.');
    string rCppName(rCppPath.begin() + startPos + 1, rCppPath.begin() + endPos);
    
    //获取 .h 文件路径
    string rHPath = exeShellCommand("find . -name " + rCppName + ".h");

    cout << "adding comment in " << rHPath << " ..." << endl << endl;

    //整理格式
    if (!rHPath.empty())
    {
        //读取文件内容
        ifstream fin(rHPath.c_str(), ifstream::in | ifstream::binary);
        if (!fin.is_open())
        {
            throw runtime_error(rHPath + "open failed");
        }
        fin.seekg(0, fin.end);
        uint64_t udlLen = fin.tellg();
        fin.seekg(0, fin.beg);
        char* npBuffer = new char[udlLen];    //注意： npBuffer 只是一个二进制块，不能解释为字符串，因为最后没有 \0
        fin.read(npBuffer, udlLen);
        if (fin)
        {
            //nothing to do
        }
        else
        {
            delete[] npBuffer;
            throw runtime_error("read file error, current file: " + rHPath);
        }
        fin.close();

        //文件内容转到字符串对象
        string rHFileContent(npBuffer, udlLen);
        delete[] npBuffer;

        //换行符转换
        toLF(rHFileContent);

        //制表符转空格
        tabToSpace(rHFileContent);

        //格式整理
        size_t pos = 0;
        pos = rCppContent.find(rCommentStartFlag, pos);
        string rComment, rFunDeclar;
        while (string::npos != pos)
        {
            //取出注释内容
            rComment.clear();
            if (false == extractComment(pos, rCppContent, rCommentEndFlag, rComment))
            {
                continue;
            }

            //取出函数名字
            rFunDeclar.clear();
            if (false == extractFunDeclar(pos, rCppContent, rFunDeclar))
            {
                continue;
            }

            //在 .h 中添加注释
            insertCommentForFunDeclar(rFunDeclar, rComment, rHFileContent);

            //进行下一次查找
            pos = rCppContent.find(rCommentStartFlag, pos + 1);
        }

        //格式化处理
        //*INOUT -> *       INOUT
        //*IN -> *       IN
        //*OUT -> *       OUT
        regex find;
        string rReplace, rAfterReplace;
        for (auto& iFormat : jsConf["h_format"])
        {
            find = iFormat["find"].get<string>();
            rReplace = iFormat["replace"].get<string>();
            regex_replace(back_inserter(rAfterReplace), rHFileContent.begin(), rHFileContent.end(), find, rReplace);
            rHFileContent = move(rAfterReplace);
            rAfterReplace.clear();
        }

        //对模板函数进行处理 template<class T> + 注释 + 函数声明 -> 注释 + template<class T> 函数声明
        find = "( *?template<[\\s\\S]+?>)(\\n)( */\\*[\\s\\S]*?\\*/)";
        rReplace = "$3\r\n$1";
        regex_replace(back_inserter(rAfterReplace), rHFileContent.begin(), rHFileContent.end(), find, rReplace);
        rHFileContent = move(rAfterReplace);
        rAfterReplace.clear();

        //输出文件换行符转换，默认 CRLF
        if ("LF" != jsConf["written_line_break"].get<string>())
        {
            toCRLF(rHFileContent);
        }

        //写入原文件
        fstream fout(rHPath.c_str(), ofstream::out | ofstream::trunc | ofstream::binary);
        fout << rHFileContent;
        fout.close();
    }
}    //addCommentInH()

void formatCppComment(IN json& jsConf, IN const vector<string>& vCppPath)
{
    string rCommentStartFlag = jsConf["comment_start_flag"].get<string>();

    ifstream fin;
    ofstream fout;
    for (const auto& iCppPath : vCppPath)
    {
        cout << "formatting " << iCppPath << " ..." << endl;

        //读取文件内容
        fin.open(iCppPath.c_str(), ifstream::in | ifstream::binary);
        if (!fin.is_open())
        {
            throw runtime_error(iCppPath + "open failed");
        }
        fin.seekg(0, fin.end);
        uint64_t udlLen = fin.tellg();
        fin.seekg(0, fin.beg);
        char* npBuffer = new char[udlLen];    //注意：npBuffer 只是一个二进制块，不能解释为字符串，因为最后没有 \0
        fin.read(npBuffer, udlLen);
        if (fin)
        {
            //nothing to do
        }
        else
        {
            delete[] npBuffer;
            throw runtime_error("read file error, current file: " + iCppPath);
        }
        fin.close();

        //文件内容转到字符串对象
        string rCppContent(npBuffer, udlLen);
        delete[] npBuffer;

        //换行符转换
        toLF(rCppContent);

        //制表符转空格
        tabToSpace(rCppContent);

        //计算 comment_start_flag 前边有几个空格
        string rSpace = calSpace(jsConf, rCppContent);

        //格式化处理
        regex find;
        string rReplace, rAfterReplace;
        for (auto& iFormat : jsConf["cpp_format"])
        {
            find = iFormat["find"].get<string>();
            rReplace = rSpace + iFormat["replace"].get<string>();
            regex_replace(back_inserter(rAfterReplace), rCppContent.begin(), rCppContent.end(), find, rReplace);
            rCppContent = move(rAfterReplace);
            rAfterReplace.clear();
        }

        //输出文件换行符转换，默认 CRLF
        if ("LF" != jsConf["written_line_break"].get<string>())
        {
            toCRLF(rCppContent);
        }

        //写入原文件
        fout.open(iCppPath.c_str(), ofstream::out | ofstream::trunc | ofstream::binary);
        fout << rCppContent;
        fout.close();

        //在头文件中添加注释
        addCommentInH(jsConf, iCppPath, rCppContent);
    }
}    //formatCppComment()

void commentCpp(IN json& jsConf)
{
    //查找路径下的 cpp 和 ipp 文件
    string rShellCommmand, rIgnorePathShellCmdPart1, rIgnorePathShellCmdPart2;
    if (true == ignorePath(jsConf, rIgnorePathShellCmdPart1, rIgnorePathShellCmdPart2))
    {
        rShellCommmand = "find . " + rIgnorePathShellCmdPart1 + " -name '*.[ci]pp'" + rIgnorePathShellCmdPart2;
    }
    else
    {
        rShellCommmand = "find . -name *.[ci]pp";
    }

    //执行 shell 命令
    vector<string> vCppPath;
    exeShellCommand(rShellCommmand, vCppPath);

    //格式整理
    formatCppComment(jsConf, vCppPath);
}    //commentCpp()