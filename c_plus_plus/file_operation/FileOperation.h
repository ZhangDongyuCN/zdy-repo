#pragma once

/*
*Environment：
*Linux(Ubuntu), C++11，gcc 7.5.0，g++ 7.5.0
*Description：
*文件和目录操作（e.g. 拷贝文件，拷贝目录，计算目录大小，...)
*/

#ifndef PARAMETER_FLOW
#define PARAMETER_FLOW
#define IN
#define OUT
#define INOUT
#endif    //PARAMETER_FLOW

#ifndef BASE_TYPE_DEF
#define BASE_TYPE_DEF
#include <stdint.h>
typedef int16_t          SHORT;
typedef uint16_t         USHORT;
typedef int32_t          INT;
typedef uint32_t         UINT;
typedef int64_t          DLONG;
typedef uint64_t         DULONG;
typedef void             VOID;
typedef bool             BOOL;
typedef signed char      CHAR;
typedef signed char      BYTE;
typedef unsigned char    UCHAR;
typedef float            FLOAT;
typedef double           DOUBLE;
#endif    //BASE_TYPE_DEF

#include <dirent.h>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <vector>
#include <sys/vfs.h>

using std::make_pair;
using std::pair;
using std::string;
using std::vector;

/*
Explain:
 - fileLongName：加了路径的文件名，e.g. /root/xxx/1.txt
 - fileShortName：没有加路径的文件名，e.g. 1.txt
 - dirLongName：加了路径的目录名，e.g. /root/xxx/MyDocuments
 - dirShortName：没有加路径的目录名，e.g. MyDocuments
*/

class FileOperation
{
public:
    /*
    *Function    : getDirSize
    *Description : 获取目录大小
    *Modify      : 2020.09.16
    *Input       : IN const string& dirLongName，目录名
    *            : OUT DLONG& dirSize，目录大小，单位：字节
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 目录末尾不能有斜杠，e.g. 正确传参：/root/xxx，错误传参：/root/xxx/
    */
    static pair<BOOL, string> getDirSize(IN const string& dirLongName, OUT DLONG& dirSize);

    /*
    *Function    : getDirSizeAndMaxFileSize
    *Description : 获取目录大小和其中最大文件大小
    *Modify      : 2020.09.16
    *Input       : IN const string& dirLongName，目录名
    *            : OUT DLONG& dirSize，目录大小，单位：字节
    *            : OUT DLONG& maxFileSize，目录中最大文件大小，单位：字节
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 目录末尾不能有斜杠，e.g. 正确传参：/root/xxx，错误传参：/root/xxx/
    */
    static pair<BOOL, string> getDirSizeAndMaxFileSize(IN const string& dirLongName, OUT DLONG& dirSize, OUT DLONG& maxFileSize);

    /*
    *Function    : getFileSize
    *Description : 获取文件大小
    *Modify      : 2020.09.16
    *Input       : IN const string& fileLongName，文件名
    *            : OUT DLONG& fileSize，文件大小
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     :
    */
    static pair<BOOL, string> getFileSize(IN const string& fileLongName, OUT DLONG& fileSize);

    /*
    *Function    : getDiskSpace
    *Description : 获取磁盘总容量和剩余容量
    *Modify      : 2020.09.16
    *Input       : IN const string& diskPath，磁盘路径，e.g. /root
    *            : OUT DLONG& totalSpace，磁盘总容量，单位：字节
    *            : OUT DLONG& freeSpace，磁盘剩余容量，单位：字节
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     :
    */
    static pair<BOOL, string> getDiskSpace(IN const string& diskPath, OUT DLONG& totalSpace, OUT DLONG& freeSpace);

    /*
    *Function    : getAllFileShortNamesInDir
    *Description : 获取目录下所有文件的短文件名
    *Modify      : 2020.09.16
    *Input       : IN const string& dirLongName，目录名
    *            : OUT vector<string>& allFileShortNames，目录下所有文件的短文件名
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 目录末尾不能有斜杠，e.g. 正确传参：/root/xxx，错误传参：/root/xxx/
    *            :
    */
    static pair<BOOL, string> getAllFileShortNamesInDir(IN const string& dirLongName, OUT vector<string>& allFileShortNames);

    /*
    *Function    : getAllFileLongNamesInDir
    *Description : 获取目录下所有文件的长文件名
    *Modify      : 2020.09.16
    *Input       : IN const string& dirLongName，目录名
    *            : OUT vector<string>& allFileLongNames，目录下所有文件的长文件名
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 目录末尾不能有斜杠，e.g. 正确传参：/root/xxx，错误传参：/root/xxx/
    *            :
    */
    static pair<BOOL, string> getAllFileLongNamesInDir(IN const string& dirLongName, OUT vector<string>& allFileLongNames);

    /*
    *Function    : moveFileOrDir
    *Description : 移动文件或文件夹
    *Modify      : 2020.09.16
    *Input       : IN const string& srcLongName，源文件(文件夹)名
    *            : IN const string& destLongName，目标文件(文件夹)名
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 如果 srcLongName 和 destLongName 指定相同的路径，则进行重命名操作
    *            : 如果 srcLongName 和 destLongName 指定不同的路径，则文件(文件夹)将移动到新位置
    *            : 如果 destLongName 为现有文件，则覆盖现有文件
    *            : 如果 destLongName 为现有文件夹，则执行失败
    */
    static pair<BOOL, string> moveFileOrDir(IN const string& srcLongName, IN const string& destLongName);

    /*
    *Function    : removeFile
    *Description : 删除文件
    *Modify      : 2020.09.15
    *Input       : IN const string& fileLongName，文件名
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     :
    */
    static pair<BOOL, string> removeFile(IN const string& fileLongName);

    /*
    *Function    : removeDir
    *Description : 删除目录
    *Modify      : 2020.09.16
    *Input       : IN const string& dirLongName，目录名
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 目录末尾不能有斜杠，e.g. 正确传参：/root/xxx，错误传参：/root/xxx/
    */
    static pair<BOOL, string> removeDir(IN const string& dirLongName);

    /*
    *Function    : copyFile
    *Description : 复制文件
    *Modify      : 2020.09.16
    *Input       : IN const string& srcFileLongName，源文件名
    *            : IN const string& destLongFileName，目标文件名
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     :
    */
    static pair<BOOL, string> copyFile(IN const string& srcFileLongName, IN const string& destLongFileName);

    /*
    *Function    : copyDir
    *Description : 复制目录
    *Modify      : 2020.09.15
    *Input       : IN const string& dirLongName，源目录名
    *            : IN string& destDirPath，目标目录名
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 目录末尾不能有斜杠，e.g. 正确传参：/root/xxx，错误传参：/root/xxx/
    *            :
    */
    static pair<BOOL, string> copyDir(IN const string& srcDirPath, IN const string& destDirPath);

    /*
    *Function    : truncateFile
    *Description : 截断文件
    *Modify      : 2020.09.15
    *Input       : IN const string& fileLongName，文件名
    *            : IN const DLONG len，文件长度截断为长度 len
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     : 如果文件先前大于此大小，则多余的数据会丢失
    *            : 如果文件先前较短，则将其扩展，扩展部分填充为空字节（'\0'）
    *            : 截断后 struct stat 的 st_size 变为 len
    */
    static pair<BOOL, string> truncateFile(IN const string& fileLongName, IN const DLONG len);

};    //FileOperation()