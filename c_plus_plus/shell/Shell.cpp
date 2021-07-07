#include "Shell.h"

pair<BOOL, string> Shell::exeShellCmd(IN const string &cmd, OUT INT *cmdReturnValue)
{
    pid_t status;                 //pid_t 就是 int
    status = system(cmd.c_str()); //阶段1：创建子进程等准备工作，如果失败返回 -1
    if (-1 == status)
    {
        return make_pair(false, "阶段1：创建子进程等准备工作错误, 错误信息：" + string(strerror(errno)));
    }
    else
    {
        //阶段2：调用 /bin/sh 拉起脚本执行，如果脚本拉起失败或脚本未正常执行结束，则原因值被写入到 status 的低 8~15 比特位中。
        //不管脚本中返回什么值，是 0 还是非 0，都算正常执行结束。即使脚本不存在或没有执行权限，也都算正常执行结束。
        //如果脚本执行过程中被强制 kill 掉等情况则算异常结束。
        if (WIFEXITED(status))
        {
            if (nullptr != cmdReturnValue)
            {
                *cmdReturnValue = WEXITSTATUS(status); //获取脚本返回值，一般脚本或命令正确执行返回值是 0，执行错误返回其它值
            }
            return make_pair(true, "");
        }
        else
        {
            return make_pair(false, "阶段2：调用 /bin/sh 拉起脚本(命令)执行，脚本(命令)拉起失败或脚本(命令)未正常执行结束");
        }
    }
} //exeShellCmd()

pair<BOOL, string> Shell::exeShellCmd(IN const string &cmd, OUT vector<string> &results)
{
    INT bufferSize = 10240; //10KB应该是非常充足了
    CHAR *buffer = new CHAR[bufferSize];
    FILE *pFile = NULL;
    if (NULL == (pFile = popen(cmd.c_str(), "r")))
    {
        return make_pair(false, "execute shell command error");
    }
    while (NULL != fgets(buffer, bufferSize, pFile))
    {
        buffer[strlen(buffer) - 1] = '\0'; //fgets() 会自动在末尾加入换行符，linux 下换行符就是 \n(LF)，这里把自动添加的换行符去掉
        results.emplace_back(buffer);
    }
    delete[] buffer;
    pclose(pFile);
    return make_pair(true, "");
} //exeShellCmd()
