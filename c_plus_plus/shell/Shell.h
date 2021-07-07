/*
*Environment：
*Linux(Ubuntu), C++11，gcc 7.5.0，g++ 7.5.0
*Description：
*执行 Linux shell 命令并获取命令返回值或命令执行结果
*/

#ifndef PARAMETER_FLOW
#define PARAMETER_FLOW
#define IN
#define OUT
#define INOUT
#endif //PARAMETER_FLOW

#ifndef BASE_TYPE_DEF
#define BASE_TYPE_DEF
#include <stdint.h>
typedef int16_t SHORT;
typedef uint16_t USHORT;
typedef int32_t INT;
typedef uint32_t UINT;
typedef int64_t DLONG;
typedef uint64_t DULONG;
typedef void VOID;
typedef bool BOOL;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef float FLOAT;
typedef double DOUBLE;
#endif //BASE_TYPE_DEF

#include <string>
#include <string.h>
#include <utility>
#include <vector>

using std::make_pair;
using std::pair;
using std::string;
using std::vector;

class Shell
{
public:
    /*
    *Function    : exeShellCmd
    *Description : 执行 Linux shell 命令并获取命令返回值
    *Modify      : 2020.09.17
    *Input       : IN const string& cmd = ""，Linux shell 命令
    *            : OUT INT* cmdReturnValue = nullptr，命令返回值
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     :
    */
    static pair<BOOL, string> exeShellCmd(IN const string &cmd = "", OUT INT *cmdReturnValue = nullptr);

    /*
    *Function    : exeShellCmd
    *Description : 执行 Linux shell 命令并获取命令执行结果
    *Modify      : 2020.09.17
    *Input       : IN const string& cmd，Linux shell 命令
    *            : OUT vector<string>& results，命令执行结果
    *Return      : pair<BOOL, string>，<函数是否执行成功，执行失败时的错误信息>
    *Caution     :
    */
    static pair<BOOL, string> exeShellCmd(IN const string &cmd, OUT vector<string> &results);
}; //Shell
