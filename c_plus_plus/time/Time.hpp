#ifndef  SINICH_EVEEN_TIME
#define SINICH_EVEEN_TIME

/*
*Environment：
*Linux(Ubuntu), C++11，gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
*Description：
*时间计算类
*/

#include <chrono>
#include <ctime>
#include <string>
#include <cstring>

//参数流向
#define IN
#define OUT
#define INOUT

using std::time_t;
using std::string;
using std::mktime;

using namespace std::chrono;

namespace sinicheveen
{
    class Time
    {
    public:
        //检验 struct tm* 是否合法
        inline static bool checkTM(IN struct tm* pTime_tm);

        //获取当前标准时间
        inline static string getCurrStdTime(IN const char* szTimeFormat = "%Y-%m-%d %H:%M:%S");

        //获取当前时间戳（默认毫秒级别）
        template<class T = milliseconds>
        inline static int64_t getCurrTimeStampByChrono();

        //获取当前时间戳（毫秒级别）
        inline static int64_t getCurrTimeStampByCTime();

        //根据年龄获取出生日期时间戳（毫秒级别）
        inline static int64_t getTimeStampByAge(IN const int32_t age);

        //根据出生年月日获取出生日期时间戳（毫秒级别）
        inline static bool getTimeStampByYMD(IN const string& strBornYMD, OUT int64_t& dlTimeStamp);

        //标准时间转毫秒级时间戳，标准时间格式：「YYYY-MM-DD HH:MM:SS」
        inline static int64_t stdTimeToTimeStamp(IN const string& strStdTime);

        //毫秒级时间戳转标准时间，标准时间格式：「YYYY-MM-DD HH:MM:SS」
        inline static string timeStampToStdTime(IN const int64_t timeStamp);
    };    //Time

    /*
    *Name：checkTM
    *Description：检验 struct tm* 是否合法
    *Input：IN struct tm* pTime_tm，时间
    *Return：bool，合法返回true，否则返回false
    *Caution：
    */
    inline bool Time::checkTM(IN struct tm* pTime_tm)
    {
        if (nullptr == pTime_tm)
        {
            return false;
        }

        if (pTime_tm->tm_mon < 0 || pTime_tm->tm_mon > 11
            || pTime_tm->tm_mday < 1 || pTime_tm->tm_mday > 31
            || pTime_tm->tm_hour < 0 || pTime_tm->tm_hour > 23
            || pTime_tm->tm_min < 0 || pTime_tm->tm_min > 59
            || pTime_tm->tm_sec < 0 || pTime_tm->tm_sec > 59)
        {
            return false;
        }

        if (4 == (pTime_tm->tm_mon + 1) || 6 == (pTime_tm->tm_mon + 1) || 9 == (pTime_tm->tm_mon + 1) || 11 == (pTime_tm->tm_mon + 1))    //小月有30天
        {
            if (pTime_tm->tm_mday > 30)
            {
                return false;
            }
        }

        if (2 == (pTime_tm->tm_mon + 1))    //2月
        {
            if ((0 == ((pTime_tm->tm_year + 1900) % 400)) || ((0 == ((pTime_tm->tm_year + 1900) % 4)) && (0 != ((pTime_tm->tm_year + 1900) % 100))))
            {
                if (pTime_tm->tm_mday > 29)    //闰年2月份有29天
                {
                    return false;
                }
            }
            else    //平年2月份有28天
            {
                if (pTime_tm->tm_mday > 28)
                {
                    return false;
                }
            }
        }

        return true;
    }    //checkTM()

   /*
    *Name：getCurrStdTime
    *Description：获取当前标准时间
    *Input：IN const char* szTimeFormat，时间格式
    *Return：string，系统当前时间
    *Caution：
    */
    inline string Time::getCurrStdTime(IN const char* szTimeFormat)
    {
        time_t currTime_t;
        time(&currTime_t);
        struct tm* locTime_tm = localtime(&currTime_t);    //转换成本地日历时间
        char szStdTime[50] = { "" };    //自定义格式存储位置
        strftime(szStdTime, 50, szTimeFormat, locTime_tm);
        return string(szStdTime);
    }    //getCurrStdTime()

    /*
    *Name：getCurrTimeStampByChrono
    *Description：获取当前时间戳（默认毫秒级别）
    *Input：
    *Return：int64_t，时间戳，默认毫秒级别
    *Caution：
    */
    template<class T>
    inline int64_t Time::getCurrTimeStampByChrono()
    {
        system_clock::time_point currentTime_tp = system_clock::now();    //当前时间
        system_clock::duration timeSinceEpoch_du = currentTime_tp.time_since_epoch();    //从 Epoch 到现的纳秒数
        int64_t dlTimeStamp = duration_cast<T>(timeSinceEpoch_du).count();    //时间转换，默认毫秒级别
        return dlTimeStamp;
    }    //getCurrTimeStampByChrono()

    /*
    *Name：getCurrTimeStampByCTime
    *Description：获取当前时间戳（毫秒级别）
    *Input：
    *Return：int64_t，时间戳
    *Caution：
    */
    inline int64_t Time::getCurrTimeStampByCTime()
    {
        return time(NULL) * 1000;
    }    //getCurrTimeStampByCTime()

    /*
    *Name：getTimeStampByAge
    *Description：根据年龄获取出生日期时间戳（毫秒级别）
    *Input：IN const int32_t age，年龄
    *Return：int64_t，出生日期时间戳（毫秒级别）
    *Caution：
    */
    inline int64_t Time::getTimeStampByAge(IN const int32_t age)
    {
        time_t currTime_t;
        time(&currTime_t);
        struct tm* locTime_tm = localtime(&currTime_t);    //转换成本地日历时间
        locTime_tm->tm_year = locTime_tm->tm_year - age;
        locTime_tm->tm_hour = 0;
        locTime_tm->tm_min = 0;
        locTime_tm->tm_sec = 0;
        return mktime(locTime_tm) * 1000;
    }    //getTimeStampByAge()

    /*
    *Name：getTimeStampByYMD
    *Description：根据出生年月日获取出生日期时间戳（毫秒级别）
    *Input：IN const string& strBornYMD，出生年月日
            OUT int64_t& dlTimeStamp，出生日期时间戳（毫秒级别）
    *Return：bool，如果出生年月日不合法返回false，否则时间戳获取成功返回true
    *Caution：
    */
    inline bool Time::getTimeStampByYMD(IN const string& strBornYMD, OUT int64_t& dlTimeStamp)
    {
        struct tm* pLocTime_tm = (struct tm*)malloc(sizeof(struct tm));
        memset(pLocTime_tm, 0, sizeof(struct tm));
        strptime(strBornYMD.c_str(), "%Y-%m-%d", pLocTime_tm);
        if (true != checkTM(pLocTime_tm))
        {
            free(pLocTime_tm);
            return false;
        }
        dlTimeStamp = mktime(pLocTime_tm) * 1000;
        free(pLocTime_tm);

        return true;
    }

    /*
    *Name：stdTimeToTimeStamp
    *Description：标准时间转毫秒级时间戳，标准时间格式：「YYYY-MM-DD HH:MM:SS」
    *Input：IN const string& strStdTime，标准时间
    *Return：int64_t，如果标准时间正确，返回对应的时间戳，否则返回-1
    *Caution：
    */
    inline int64_t Time::stdTimeToTimeStamp(IN const string& strStdTime)
    {
        struct tm* pLocTime_tm = (struct tm*)malloc(sizeof(struct tm));
        strptime(strStdTime.c_str(), "%Y-%m-%d %H:%M:%S", pLocTime_tm);
        if (true != checkTM(pLocTime_tm))
        {
            free(pLocTime_tm);
            return -1;
        }
        int64_t dlTimeStamp = mktime(pLocTime_tm) * 1000;
        free(pLocTime_tm);

        return dlTimeStamp;
    }    //stdTimeToTimeStamp()

    /*
    *Name：timeStampToStdTime
    *Description：毫秒级时间戳转标准时间，标准时间格式：「YYYY-MM-DD HH:MM:SS」
    *Input：IN const int64_t timeStamp，毫秒级时间戳
    *Return：string，标准时间，标准时间格式：「YYYY-MM-DD HH:MM:SS」
    *Caution：
    */
    inline string Time::timeStampToStdTime(IN const int64_t timeStamp)
    {
        time_t timeStamp_t = timeStamp / 1000;
        struct tm* pLocTime_tm;
        pLocTime_tm = localtime(&timeStamp_t);    //转换成本地日历时
        char szTimeStr[20] = { 0 };    //储存标准时间
        strftime(szTimeStr, 50, "%Y-%m-%d %H:%M:%S", pLocTime_tm);    //转换时间格式

        return szTimeStr;
    }    //timeStampToStdTime()

}    //sinicheveen

#endif    //SINICH_EVEEN_TIME
