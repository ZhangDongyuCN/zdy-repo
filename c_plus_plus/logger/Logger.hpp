#ifndef LOGGER_
#define LOGGER_

/*
* Environment：
* Linux(Ubuntu), C++11，gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
* Description：
* 1> 写日志方式
* CPLUSPLUS_STYLE：写日志实现函数用的 C++ 方式
* C_STYLE：写日志实现函数用的 C 方式
* 
* 2> 函数/变量 前缀
* s_：static，静态变量
* cs_：const static，不可修改的静态变量
* m_：member，数据成员
* ms_：member static，静态数据成员
* cms_：const member static，不可修改的静态数据成员
* 
* 3>
* 支持「全局日志」写入和「模块日志」写入模式
* 支持多线程同时写入
*/

#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <exception>
#include <mutex>
#include <cstdarg>

using std::ofstream;
using std::string;
using std::mutex;
using std::to_string;
using std::ios_base;
using std::runtime_error;
using std::lock_guard;
using std::endl;

namespace sinicheveen
{
    //写日志函数的实现方式（C++ 方式和 C 方式），选择其中一种
    //#define CPLUSPLUS_STYLE
    #define C_STYLE

    //函数参数流向
    #define IN
    #define OUT
    #define INOUT

    //写 buffer 的大小
    #define LOG_WRITE_BUF_SIZE 1000

    //转字符串
    #define TO_STR(x) #x
    #define STR_HELPER(x) TO_STR(x)

    //日志前缀：日期 + 时间 + 文件名 + 行号
    #define LOG_PREFIX "[" __DATE__ ": " __TIME__ ": " __FILE__ ": " STR_HELPER(__LINE__) "]"

    //把调试宏传递给函数
    #define GLOBAL_LOG_TRACE(...)     Logger::globalTrace(LOG_PREFIX, __VA_ARGS__)
    #define GLOBAL_LOG_DEBUG(...)     Logger::globalDebug(LOG_PREFIX, __VA_ARGS__)
    #define GLOBAL_LOG_INFO(...)      Logger::globalInfo(LOG_PREFIX, __VA_ARGS__)
    #define GLOBAL_LOG_ERROR(...)     Logger::globalError(LOG_PREFIX, __VA_ARGS__)

    //方便使用静态变量
    #define ms_ofLogger               Logger::s_ofLogger()
    #define cms_ofLogger              Logger::cs_ofLogger()
    #define ms_strFileName            Logger::s_strFileName()
    #define cms_strFileName           Logger::cs_strFileName()
    #define ms_iEachFileSize          Logger::s_iEachFileSize()
    #define cms_iEachFileSize         Logger::cs_iEachFileSize()
    #define ms_iRotatingFileNum       Logger::s_iRotatingFileNum()
    #define cms_iRotatingFileNum      Logger::cs_iRotatingFileNum()
    #define ms_enLogLevel             Logger::s_enLogLevel()
    #define cms_enLogLevel            Logger::cs_enLogLevel()
    #define ms_udlCurrWriteFileSize   Logger::s_udlCurrWriteFileSize()
    #define cms_udlCurrWriteFileSize  Logger::cs_udlCurrWriteFileSize()
    #define ms_bInit                  Logger::s_bInit()
    #define cms_bInit                 Logger::cs_bInit()
    #define ms_mu                     Logger::s_mu()

    //日志等级
    enum class LOG_LEVEL_E
    {
        TRACE = 0,
        DEBUG,
        INFO
    };

    /*******************************************************************************************************
    *                                             类声明
    *******************************************************************************************************/
    class Logger
    {
    /**********************************************全局日志*************************************************/
    public:

        //全局日志初始化函数
        inline static void globalRotatingLoggerInit(IN const string& strFileName, IN const uint32_t iEachFileSize, IN const uint32_t iRotatingFileNum, IN LOG_LEVEL_E enLogLevel);

#ifdef CPLUSPLUS_STYLE
        //写入 trace 级别日志（全局日志）
        template<class T1, class T2, class... Args>
        inline static void globalTrace(IN T1 prefix, IN T2 format, IN Args... args);

        //写入 degub 级别日志（全局日志）
        template<class T1, class T2, class... Args>
        inline static void globalDebug(IN T1 prefix, IN T2 format, IN Args... args);

        //写入 info 级别日志（全局日志）
        template<class T1, class T2, class... Args>
        inline static void globalInfo(IN T1 prefix, IN T2 format, IN Args... args);

        //写入 error 级别日志（全局日志）
        template<class T1, class T2, class... Args>
        inline static void globalError(IN T1 prefix, IN T2 format, IN Args... args);
#endif    //CPLUSPLUS_STYLE

#ifdef C_STYLE
        //写入 trace 级别日志（全局日志
        inline static void globalTrace(IN const char* prefix, IN const char* format, IN ...);

        //写入 degub 级别日志（全局日志）
        inline static void globalDebug(IN const char* prefix, IN const char* format, IN ...);

        //写入 info 级别日志（全局日志）
        inline static void globalInfo(IN const char* prefix, IN const char* format, IN ...);

        //写入 error 级别日志（全局日志）
        inline static void globalError(IN const char* prefix, IN const char* format, IN ...);
#endif    //C_STYLE

    private:

        //最新日志文件写满后，开始轮询操作（删除最旧日志，然后依次重命名，再新建一个文件用于写入日志）（全局日志）
        inline static void globalRotatingFile();

    private:    //模拟实现类内静态数据成员

        //当前日志写文件对象
        inline static ofstream& s_ofLogger();
        inline static const ofstream& cs_ofLogger();

        //日志文件名
        inline static string& s_strFileName();
        inline static const string& cs_strFileName();

        //每个分片日志的大小，单位：B
        inline static uint32_t& s_iEachFileSize();
        inline static const uint32_t& cs_iEachFileSize();

        //轮询日志个数
        inline static uint32_t& s_iRotatingFileNum();
        inline static const uint32_t& cs_iRotatingFileNum();

        //日志等级
        inline static LOG_LEVEL_E& s_enLogLevel();
        inline static const LOG_LEVEL_E& cs_enLogLevel();

        //当前日志写文件大小
        inline static uint64_t& s_udlCurrWriteFileSize();
        inline static const uint64_t& cs_udlCurrWriteFileSize();

        //是否初始化
        inline static bool& s_bInit();
        inline static const bool& cs_bInit();

        //锁
        inline static mutex& s_mu();

    /**********************************************模块日志*************************************************/
    public:

        //模块日志初始化函数
        inline void moduleRotatingLoggerInit(IN const string& strFileName, IN const uint32_t iEachFileSize, IN const uint32_t iRotatingFileNum, IN LOG_LEVEL_E enLogLevel);

#ifdef CPLUSPLUS_STYLE
        //写入 trace 级别日志（模块日志）
        template<class T1, class T2, class... Args>
        inline void moduleTrace(IN T1 prefix, IN T2 format, IN Args... args);

        //写入 debug 级别日志（模块日志）
        template<class T1, class T2, class... Args>
        inline void moduleDebug(IN T1 prefix, IN T2 format, IN Args... args);

        //写入 info 级别日志（模块日志）
        template<class T1, class T2, class... Args>
        inline void moduleInfo(IN T1 prefix, IN T2 format, IN Args... args);

        //写入 error 级别日志（模块日志）
        template<class T1, class T2, class... Args>
        inline void moduleError(IN T1 prefix, IN T2 format, IN Args... args);
#endif    //CPLUSPLUS_STYLE

#ifdef C_STYLE
        //写入 trace 级别日志（模块日志）
        inline void moduleTrace(IN const char* prefix, IN const char* format, IN ...);

        //写入 debug 级别日志（模块日志）
        inline void moduleDebug(IN const char* prefix, IN const char* format, IN ...);

        //写入 info 级别日志（模块日志）
        inline void moduleInfo(IN const char* prefix, IN const char* format, IN ...);

        //写入 error 级别日志（模块日志）
        inline void moduleError(IN const char* prefix, IN const char* format, IN ...);
#endif    //C_STYLE

    private:

        //最新日志文件写满后，开始轮询操作（删除最旧日志，然后依次重命名，再新建一个文件用于写入日志）（模块日志）
        inline void moduleRotatingFile();

    private:    //类内数据成员

        ofstream m_ofLogger;              //当前日志写文件对象
        string m_strFileName;             //日志文件名
        uint32_t m_iEachFileSize;         //每个分片日志的大小，单位：B
        uint32_t m_iRotatingFileNum;      //轮询日志个数
        LOG_LEVEL_E m_enLogLevel;         //日志等级
        uint64_t m_udlCurrWriteFileSize;  //当前日志写文件大小
        bool m_bInit = false;             //是否初始化
        mutex m_mu;                       //锁

    /******************************************构造函数和析构函数********************************************/
    public:

        Logger() {};

        ~Logger()
        {
            lock_guard<mutex> lck(ms_mu);
            ms_ofLogger.close();
        }
    };

    /*******************************************************************************************************
    *                                             类实现
    *******************************************************************************************************/

    /**********************************************全局日志*************************************************/
    /*
    *Name：globalRotatingLoggerInit
    *Description：全局日志初始化函数
    *Input：IN const string& strFileName，日志文件名
            IN const uint32_t iEachFileSize，每个分片日志的大小，单位：B
            IN const uint32_t iRotatingFileNum，轮询日志个数
            IN LOG_LEVEL_E enLogLevel，日志等级
    *Return：string，配置值
    *Caution：
    */
    inline void Logger::globalRotatingLoggerInit(IN const string& strFileName, IN const uint32_t iEachFileSize, IN const uint32_t iRotatingFileNum, IN LOG_LEVEL_E enLogLevel)
    {
        lock_guard<mutex> lck(ms_mu);

        //参数校验
        if (iEachFileSize < 1)
        {
            throw runtime_error("file size should >= 1");
        }
        if (iRotatingFileNum < 2)
        {
            throw runtime_error("rotating file num should >= 2");
        }

        if (false == ms_bInit)
        {
            //创建日志轮询文件
            string strFileName_;
            for (uint32_t i = 1; i <= iRotatingFileNum - 1; i++)
            {
                strFileName_ = strFileName + "." + to_string(i) + ".log";
                ms_ofLogger.open(strFileName_.c_str(), ios_base::out | ios_base::app);
                ms_ofLogger.close();
            }
            strFileName_ = strFileName + ".log";
            ms_ofLogger.open(strFileName_.c_str(), ios_base::out | ios_base::app);
            ms_ofLogger.seekp(0, ios_base::end);
            ms_udlCurrWriteFileSize = ms_ofLogger.tellp();

            //变量赋值
            ms_strFileName = strFileName;
            ms_iEachFileSize = iEachFileSize;
            ms_iRotatingFileNum = iRotatingFileNum;
            ms_enLogLevel = enLogLevel;
            ms_bInit = true;
        }
    }

#ifdef CPLUSPLUS_STYLE
    /*
    *Name：globalTrace
    *Description：写入 trace 级别日志（全局日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::globalTrace(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(ms_mu);

        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if (0 == (int)ms_enLogLevel)
        {
            if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
            {
                globalRotatingFile();
            }
            string format_ = string("%s %s ") + static_cast<string>(format);
            char buffer[LOG_WRITE_BUF_SIZE];
            snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[T]", args...);
            ms_ofLogger << buffer << endl;
            ms_udlCurrWriteFileSize += strlen(buffer) + 2;
        }
    }

    /*
    *Name：globalDebug
    *Description：写入 degub 级别日志（全局日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::globalDebug(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(ms_mu);

        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if ((int)ms_enLogLevel <= 1)
        {
            if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
            {
                globalRotatingFile();
            }
            string format_ = string("%s %s ") + static_cast<string>(format);
            char buffer[LOG_WRITE_BUF_SIZE];
            snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[D]", args...);
            ms_ofLogger << buffer << endl;
            ms_udlCurrWriteFileSize += strlen(buffer) + 2;
        }
    }

    /*
    *Name：globalInfo
    *Description：写入 info 级别日志（全局日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::globalInfo(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(ms_mu);

        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if ((int)ms_enLogLevel <= 2)
        {
            if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
            {
                globalRotatingFile();
            }
            string format_ = string("%s %s ") + static_cast<string>(format);
            char buffer[LOG_WRITE_BUF_SIZE];
            snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[I]", args...);
            ms_udlCurrWriteFileSize += strlen(buffer) + 2;
            ms_ofLogger << buffer << endl;
        }
    }

    /*
    *Name：globalError
    *Description：写入 error 级别日志（全局日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::globalError(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(ms_mu);

        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
        {
            globalRotatingFile();
        }
        string format_ = string("%s %s ") + static_cast<string>(format);
        char buffer[LOG_WRITE_BUF_SIZE];
        snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[E]", args...);
        ms_udlCurrWriteFileSize += strlen(buffer) + 2;
        ms_ofLogger << buffer << endl;
    }
#endif    //CPLUSPLUS_STYLE

#ifdef C_STYLE
    /*
    *Name：globalTrace（C 语言传可变参数风格）
    *Description：写入 trace 级别日志（全局日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::globalTrace(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(ms_mu);

        va_list args;
        va_start(args, format);
        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if (0 == (int)ms_enLogLevel)
        {
            if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
            {
                globalRotatingFile();
            }
            char buffer[LOG_WRITE_BUF_SIZE];
            vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
            ms_ofLogger << prefix << " [T] " << buffer << endl;
            ms_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        }
        va_end(args);
    }

    /*
    *Name：globalDebug（C 语言传可变参数风格）
    *Description：写入 debug 级别日志（全局日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::globalDebug(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(ms_mu);

        va_list args;
        va_start(args, format);
        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if ((int)ms_enLogLevel <= 1)
        {
            if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
            {
                globalRotatingFile();
            }
            char buffer[LOG_WRITE_BUF_SIZE];
            vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
            ms_ofLogger << prefix << " [D] " << buffer << endl;
            ms_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        }
        va_end(args);
    }

    /*
    *Name：globalInfo（C 语言传可变参数风格）
    *Description：写入 info 级别日志（全局日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::globalInfo(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(ms_mu);

        va_list args;
        va_start(args, format);
        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if ((int)ms_enLogLevel <= 2)
        {
            if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
            {
                globalRotatingFile();
            }
            char buffer[LOG_WRITE_BUF_SIZE];
            vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
            ms_ofLogger << prefix << " [I] " << buffer << endl;
            ms_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        }
        va_end(args);
    }

    /*
    *Name：globalError（C 语言传可变参数风格）
    *Description：写入 error 级别日志（全局日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::globalError(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(ms_mu);

        va_list args;
        va_start(args, format);
        if (false == ms_bInit)
        {
            throw runtime_error("please call init function globalRotatingLoggerInit() first");
        }
        if (ms_udlCurrWriteFileSize >= ms_iEachFileSize)
        {
            globalRotatingFile();
        }
        char buffer[LOG_WRITE_BUF_SIZE];
        vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
        ms_ofLogger << prefix << " [E] " << buffer << endl;
        ms_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        va_end(args);
    }
#endif    //C_STYLE

    /*
    *Name：globalRotatingFile
    *Description：最新日志文件写满后，开始轮询操作（删除最旧日志，然后依次重命名，再新建一个文件用于写入日志）（全局日志）
    *Input：void
    *Return：void
    *Caution：
    */
    inline void Logger::globalRotatingFile()
    {
        string strFileName, strFileNameOld, strFileNameNew;

        //关闭当前文件
        ms_ofLogger.close();

        //删除 xx.1.log 文件（这个文件记录的内容最早）
        strFileName = ms_strFileName + ".1.log";
        if (0 != remove(strFileName.c_str()))
        {
            throw runtime_error("remove [" + strFileName + "] failed");
        }

        //循环重命名文件 xx.2.log -> xx.1.log，xx.3.log -> xx.2.log, ......, xx.n-1.log -> xx.n-2.log
        for (uint32_t i = 2; i <= ms_iRotatingFileNum - 1; i++)
        {
            strFileNameOld = ms_strFileName + "." + to_string(i) + ".log";
            strFileNameNew = ms_strFileName + "." + to_string(i - 1) + ".log";
            if (0 != rename(strFileNameOld.c_str(), strFileNameNew.c_str()))
            {
                throw runtime_error("rename [" + strFileNameOld + "] to [" + strFileNameNew + "] failed");
            }
        }

        //把 xx.log 重命名为 xx.n-1.log
        strFileNameOld = ms_strFileName + ".log";
        strFileNameNew = ms_strFileName + "." + to_string(ms_iRotatingFileNum - 1) + ".log";
        if (0 != rename(strFileNameOld.c_str(), strFileNameNew.c_str()))
        {
            throw runtime_error("rename [" + strFileNameOld + "] to [" + strFileNameNew + "] failed");
        }

        //重新建立一个 xx.log 文件并打开
        strFileName = ms_strFileName + ".log";
        ms_ofLogger.open(strFileName.c_str(), ios_base::out | ios_base::app);
        ms_ofLogger.seekp(0, ios_base::end);
        ms_udlCurrWriteFileSize = ms_ofLogger.tellp();
    }

    inline ofstream& Logger::s_ofLogger()
    {
        static ofstream s_ofLogger;
        return s_ofLogger;
    }

    inline string& Logger::s_strFileName()
    {
        static string s_strFileName;
        return s_strFileName;
    }

    inline uint32_t& Logger::s_iEachFileSize()
    {
        static uint32_t s_iEachFileSize;
        return s_iEachFileSize;
    }

    inline uint32_t& Logger::s_iRotatingFileNum()
    {
        static uint32_t s_iRotatingFileNum;
        return s_iRotatingFileNum;
    }

    inline LOG_LEVEL_E& Logger::s_enLogLevel()
    {
        static LOG_LEVEL_E s_enLogLevel;
        return s_enLogLevel;
    }

    inline uint64_t& Logger::s_udlCurrWriteFileSize()
    {
        static uint64_t s_udlCurrWriteFileSize;
        return s_udlCurrWriteFileSize;
    }

    inline bool& Logger::s_bInit()
    {
        static bool s_bInit;
        return s_bInit;
    }

    inline mutex& Logger::s_mu()
    {
        static mutex s_mu;
        return s_mu;
    }

    inline const ofstream& Logger::cs_ofLogger()
    {
        return s_ofLogger();
    }

    inline const string& Logger::cs_strFileName()
    {
        return s_strFileName();
    }

    inline const uint32_t& Logger::cs_iEachFileSize()
    {
        return s_iEachFileSize();
    }

    inline const uint32_t& Logger::cs_iRotatingFileNum()
    {
        return s_iRotatingFileNum();
    }

    inline const LOG_LEVEL_E& Logger::cs_enLogLevel()
    {
        return s_enLogLevel();
    }

    inline const uint64_t& Logger::cs_udlCurrWriteFileSize()
    {
        return s_udlCurrWriteFileSize();
    }

    inline const bool& Logger::cs_bInit()
    {
        return s_bInit();
    }

    /**********************************************模块日志*************************************************/
    /*
    *Name：globalRotatingLoggerInit
    *Description：模块日志初始化函数
    *Input：IN const string& strFileName，日志文件名
            IN const uint32_t iEachFileSize，每个分片日志的大小，单位：B
            IN const uint32_t iRotatingFileNum，轮询日志个数
            IN LOG_LEVEL_E enLogLevel，日志等级
    *Return：void
    *Caution：
    */
    inline void Logger::moduleRotatingLoggerInit(IN const string& strFileName, IN const uint32_t iEachFileSize, IN const uint32_t iRotatingFileNum, IN LOG_LEVEL_E enLogLevel)
    {
        {
            lock_guard<mutex> lck(m_mu);

            //参数校验
            if (iEachFileSize < 1)
            {
                throw runtime_error("file size should >= 1");
            }
            if (iRotatingFileNum < 2)
            {
                throw runtime_error("rotating file num should >= 2");
            }

            if (false == m_bInit)
            {
                //创建日志轮询文件
                string strFileName_;
                for (uint32_t i = 1; i <= iRotatingFileNum - 1; i++)
                {
                    strFileName_ = strFileName + "." + to_string(i) + ".log";
                    m_ofLogger.open(strFileName_.c_str(), ios_base::out | ios_base::app);
                    m_ofLogger.close();
                }
                strFileName_ = strFileName + ".log";
                m_ofLogger.open(strFileName_.c_str(), ios_base::out | ios_base::app);
                m_ofLogger.seekp(0, ios_base::end);
                m_udlCurrWriteFileSize = m_ofLogger.tellp();

                //变量赋值
                m_strFileName = strFileName;
                m_iEachFileSize = iEachFileSize;
                m_iRotatingFileNum = iRotatingFileNum;
                m_enLogLevel = enLogLevel;
                m_bInit = true;
            }
        }
    }

#ifdef CPLUSPLUS_STYLE
    /*
    *Name：moduleTrace
    *Description：写入 trace 级别日志（模块日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::moduleTrace(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(m_mu);

        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if (0 == (int)m_enLogLevel)
        {
            if (m_udlCurrWriteFileSize >= m_iEachFileSize)
            {
                moduleRotatingFile();
            }
            string format_ = string("%s %s ") + static_cast<string>(format);
            char buffer[LOG_WRITE_BUF_SIZE];
            snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[T]", args...);
            m_ofLogger << buffer << endl;
            m_udlCurrWriteFileSize += strlen(buffer) + 2;
        }
    }

    /*
    *Name：moduleDebug
    *Description：写入 debug 级别日志（模块日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::moduleDebug(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(m_mu);

        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if ((int)m_enLogLevel <= 1)
        {
            if (m_udlCurrWriteFileSize >= m_iEachFileSize)
            {
                moduleRotatingFile();
            }
            string format_ = string("%s %s ") + static_cast<string>(format);
            char buffer[LOG_WRITE_BUF_SIZE];
            snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[D]", args...);
            m_ofLogger << buffer << endl;
            m_udlCurrWriteFileSize += strlen(buffer) + 2;
        }
    }

    /*
    *Name：moduleInfo
    *Description：写入 info 级别日志（模块日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::moduleInfo(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(m_mu);

        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if ((int)m_enLogLevel <= 2)
        {
            if (m_udlCurrWriteFileSize >= m_iEachFileSize)
            {
                moduleRotatingFile();
            }
            string format_ = string("%s %s ") + static_cast<string>(format);
            char buffer[LOG_WRITE_BUF_SIZE];
            snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[I]", args...);
            m_udlCurrWriteFileSize += strlen(buffer) + 2;
            m_ofLogger << buffer << endl;
        }
    }

    /*
    *Name：moduleError
    *Description：写入 error 级别日志（模块日志）
    *Input：IN T1 prefix，日志前缀
            IN T2 format，格式
            IN Args... args，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    template<class T1, class T2, class... Args>
    inline void Logger::moduleError(IN T1 prefix, IN T2 format, IN Args... args)
    {
        lock_guard<mutex> lck(m_mu);

        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if (m_udlCurrWriteFileSize >= m_iEachFileSize)
        {
            moduleRotatingFile();
        }
        string format_ = string("%s %s ") + static_cast<string>(format);
        char buffer[LOG_WRITE_BUF_SIZE];
        snprintf(buffer, LOG_WRITE_BUF_SIZE, format_.c_str(), prefix, "[E]", args...);
        m_udlCurrWriteFileSize += strlen(buffer) + 2;
        m_ofLogger << buffer << endl;
    }
#endif    //CPLUSPLUS_STYLE

#ifdef C_STYLE
    /*
    *Name：moduleTrace（C 语言传可变参数风格）
    *Description：写入 trace 级别日志（模块日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::moduleTrace(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(m_mu);

        va_list args;
        va_start(args, format);
        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if (0 == (int)m_enLogLevel)
        {
            if (m_udlCurrWriteFileSize >= m_iEachFileSize)
            {
                moduleRotatingFile();
            }
            char buffer[LOG_WRITE_BUF_SIZE];
            vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
            m_ofLogger << prefix << " [T] " << buffer << endl;
            m_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        }
        va_end(args);
    }

    /*
    *Name：moduleDebug（C 语言传可变参数风格）
    *Description：写入 debug 级别日志（模块日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::moduleDebug(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(m_mu);

        va_list args;
        va_start(args, format);
        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if ((int)m_enLogLevel <= 1)
        {
            if (m_udlCurrWriteFileSize >= m_iEachFileSize)
            {
                moduleRotatingFile();
            }
            char buffer[LOG_WRITE_BUF_SIZE];
            vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
            m_ofLogger << prefix << " [D] " << buffer << endl;
            m_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        }
        va_end(args);
    }

    /*
    *Name：moduleInfo（C 语言传可变参数风格）
    *Description：写入 info 级别日志（模块日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::moduleInfo(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(m_mu);

        va_list args;
        va_start(args, format);
        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if ((int)m_enLogLevel <= 2)
        {
            if (m_udlCurrWriteFileSize >= m_iEachFileSize)
            {
                moduleRotatingFile();
            }
            char buffer[LOG_WRITE_BUF_SIZE];
            vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
            m_ofLogger << prefix << " [I] " << buffer << endl;
            m_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        }
        va_end(args);
    }

    /*
    *Name：moduleError（C 语言传可变参数风格）
    *Description：写入 error 级别日志（模块日志）
    *Input：IN const char* prefix，日志前缀
            IN const char* format，格式
            IN ...，可变参数，要写入的内容
    *Return：void
    *Caution：
    */
    inline void Logger::moduleError(IN const char* prefix, IN const char* format, IN ...)
    {
        lock_guard<mutex> lck(m_mu);

        va_list args;
        va_start(args, format);
        if (false == m_bInit)
        {
            throw runtime_error("please call init function moduleRotatingLoggerInit() first");
        }
        if (m_udlCurrWriteFileSize >= m_iEachFileSize)
        {
            moduleRotatingFile();
        }
        char buffer[LOG_WRITE_BUF_SIZE];
        vsnprintf(buffer, LOG_WRITE_BUF_SIZE, format, args);
        m_ofLogger << prefix << " [E] " << buffer << endl;
        m_udlCurrWriteFileSize += strlen(prefix) + 5 + strlen(buffer) + 2;
        va_end(args);
    }
#endif    //C_STYLE

    /*
    *Name：moduleRotatingFile
    *Description：最新日志文件写满后，开始轮询操作（删除最旧日志，然后依次重命名，再新建一个文件用于写入日志）（模块日志）
    *Input：void
    *Return：void
    *Caution：
    */
    inline void Logger::moduleRotatingFile()
    {
        string strFileName, strFileNameOld, strFileNameNew;

        //关闭当前文件
        m_ofLogger.close();

        //删除 xx.1.log 文件（这个文件记录的内容最早）
        strFileName = m_strFileName + ".1.log";
        if (0 != remove(strFileName.c_str()))
        {
            throw runtime_error("remove [" + strFileName + "] failed");
        }

        //循环重命名文件 xx.2.log -> xx.1.log，xx.3.log -> xx.2.log, ......, xx.n-1.log -> xx.n-2.log
        for (uint32_t i = 2; i <= m_iRotatingFileNum - 1; i++)
        {
            strFileNameOld = m_strFileName + "." + to_string(i) + ".log";
            strFileNameNew = m_strFileName + "." + to_string(i - 1) + ".log";
            if (0 != rename(strFileNameOld.c_str(), strFileNameNew.c_str()))
            {
                throw runtime_error("rename [" + strFileNameOld + "] to [" + strFileNameNew + "] failed");
            }
        }

        //把 xx.log 重命名为 xx.n-1.log
        strFileNameOld = m_strFileName + ".log";
        strFileNameNew = m_strFileName + "." + to_string(m_iRotatingFileNum - 1) + ".log";
        if (0 != rename(strFileNameOld.c_str(), strFileNameNew.c_str()))
        {
            throw runtime_error("rename [" + strFileNameOld + "] to [" + strFileNameNew + "] failed");
        }

        //重新建立一个 xx.log 文件并打开
        strFileName = m_strFileName + ".log";
        m_ofLogger.open(strFileName.c_str(), ios_base::out | ios_base::app);
        m_ofLogger.seekp(0, ios_base::end);
        m_udlCurrWriteFileSize = m_ofLogger.tellp();
    }

}    //sinicheveen

#endif    //LOGGER_

/******************************************全局日志用法示例**********************************************/
#if 0

#include "Logger.hpp"
#include <unistd.h>
#include <thread>
#include <vector>

using namespace std;
using namespace sinicheveen;

void writeLog()
{
    while (true)
    {
        GLOBAL_LOG_TRACE("level: %s, code: %d", "TRACE", 0);
        usleep(400);
    }
}

int main()
{
    Logger::globalRotatingLoggerInit("my_log", 5 * 1024 * 1024, 5, LOG_LEVEL_E::TRACE);

#if 1    //多线程用法示例
    vector<thread> vT;
    for (int i = 0; i < 4; i++)
    {
        vT.push_back(thread(writeLog));
    }

    for (auto& iT : vT)
    {
        iT.join();
    }
#endif

#if 0    //单线程用法示例
    writeLog();
#endif

    return 0;
}

#endif

/******************************************模块日志用法示例**********************************************/
#if 0

#include "Logger.hpp"
#include <unistd.h>
#include <thread>
#include <vector>

using namespace std;
using namespace sinicheveen;

//把调试宏传递给函数
#define MODULE_LOG_TRACE(...)     logHandle.moduleTrace(LOG_PREFIX, __VA_ARGS__)
#define MODULE_LOG_DEBUG(...)     logHandle.moduleDebug(LOG_PREFIX, __VA_ARGS__)
#define MODULE_LOG_INFO(...)      logHandle.moduleInfo(LOG_PREFIX, __VA_ARGS__)
#define MODULE_LOG_ERROR(...)     logHandle.moduleError(LOG_PREFIX, __VA_ARGS__)

void writeLog(Logger& logHandle)
{
    while (true)
    {
        MODULE_LOG_TRACE("level: %s, code: %d", "TRACE", 0);
        usleep(400);
    }
}

int main()
{
    Logger logHandle;

    logHandle.moduleRotatingLoggerInit("my_log_model", 5 * 1024 * 1024, 5, LOG_LEVEL_E::TRACE);

#if 1    //多线程用法示例
    vector<thread> vT;
    for (int i = 0; i < 4; i++)
    {
        vT.push_back(thread(writeLog, ref(logHandle)));
    }

    for (auto& iT : vT)
    {
        iT.join();
    }
#endif

#if 0    //单线程用法示例
    writeLog(logHandle);
#endif

return 0;
}

#endif
