#include <sys/stat.h>
#include <string>
#include <dirent.h>
#include <iostream>
#include <map>

#define IN
#define OUT
#define INOUT

using namespace std;

void renameFileWithModifyTime(IN string strDir, IN const string strFormat);

string timetToStr(IN const time_t timeStamp_t, IN const string strFormat);

int main(void)
{
    srand((unsigned)time(NULL));

    string strDir, strFormat;
    cout << "Please input directory path: " << endl;
    cin >> strDir;
    cout << endl << "Please input new file name format: (input -1 to use default format: IMG_%Y%m%d_%H%M%S, e.g. IMG_20102029_202550)" << endl;
    cin >> strFormat;

    if ("-1" == strFormat)
    {
        strFormat = "IMG_%Y%m%d_%H%M%S";
    }

    renameFileWithModifyTime(strDir, strFormat);

    cout << endl << "done!" << endl;

    return 0;
}

void renameFileWithModifyTime(IN string strDir, IN const string strFormat)
{
    //变量定义
    struct stat fileStat;
    int rc;
    struct dirent* pDirent;
    string strFileFullPath, strNewFileName, strFileExtension;
    map<string, int> mNewNameAndCount;    //文件新名字 + 出现的次数

    //循环读取目录下文件名并重命名
    DIR* pDir = opendir(strDir.c_str());
    if (NULL != pDir)
    {
        while (NULL != (pDirent = readdir(pDir)))
        {
            string strFileName = pDirent->d_name;
            if ("." == strFileName || ".." == strFileName)
            {
                continue;
            }
            else
            {
                strFileFullPath = strDir + "/" + strFileName;
                rc = stat(strFileFullPath.c_str(), &fileStat);
                if (rc != 0)
                {
                    string strErrInfo = "获取文件（目录）状态信息错误，当前文件（目录）：" + strFileName;
                    throw strErrInfo;
                }
                else
                {
                    if (!S_ISDIR(fileStat.st_mode))
                    {
                        strNewFileName = timetToStr(fileStat.st_mtime, strFormat);

                        //如果多个文件的创建时间一样，直接按照创建时间重命名，原来的文件会被替换。解决方案：记录新文件名出现的次数，如果超过一次，再次重命名加上后缀，
                        //后缀为新文件名出现的次数，e.g. IMG_20200229_113050_2.jpg，其中 _2 就是后缀，表示这是第二个重名的文件
                        if (mNewNameAndCount.end() == mNewNameAndCount.find(strNewFileName))
                        {
                            mNewNameAndCount[strNewFileName] = 1;
                            strNewFileName = strDir + "/" + strNewFileName + string(strFileName, strFileName.find_last_of("."));
                        }
                        else
                        {
                            strNewFileName = strDir + "/" + strNewFileName + "_" + to_string(++mNewNameAndCount[strNewFileName]) + string(strFileName, strFileName.find_last_of("."));
                        }

                        rc = rename(strFileFullPath.c_str(), strNewFileName.c_str());
                        if (rc != 0)
                        {
                            string strErrInfo = "重命名文件（目录）错误，当前文件（目录）：" + strFileName;
                            throw strErrInfo;
                        }
                    }
                    else
                    {
                        renameFileWithModifyTime(strFileFullPath, strFormat);    //递归读取子目录
                    }
                }
            }
        }    //while
    }    //if

}    //readFileNameInDir()

string timetToStr(IN const time_t timeStamp_t, IN const string strFormat)
{
    struct tm* pLocTime_tm;
    pLocTime_tm = localtime(&timeStamp_t);    //转换成本地日历时
    char szTimeStr[256] = { 0 };    //储存字符串时间
    strftime(szTimeStr, 50, strFormat.c_str(), pLocTime_tm);    //转换时间格式

    return szTimeStr;
}    //timetToStr()
