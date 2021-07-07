#ifndef ANY_TYPE_
#define ANY_TYPE_

#include <string>
#include <map>
#include <exception>
#include <type_traits>

using std::string;
using std::map;
using std::is_same;
using std::move;
using std::runtime_error;
using std::enable_if;

namespace sinicheveen
{
    #define ms_mapTypeString AnyType::s_mapTypeString()

    /*******************************************************************************************************
    *                                             类声明
    *******************************************************************************************************/
    class AnyType
    {
    public:

    //gruop：数据类型
        enum TYPE_E
        {
            INTEGER = 0,
            NUMBER,
            STRING
        };

    //gruop：构造函数、析构函数

        inline AnyType() {};

        inline explicit AnyType(int32_t&& integer);

        inline explicit AnyType(int64_t&& integer);

        inline explicit AnyType(float&& number);

        inline explicit AnyType(double&& number);

        inline explicit AnyType(string&& str);

        inline AnyType(const AnyType& any_type_value);

        inline AnyType(AnyType&& any_type_value);

        inline ~AnyType();

    //group：运算符

        inline void operator=(int32_t&& interger);

        inline void operator=(int64_t&& interger);
        
        inline void operator=(double&& number);

        inline void operator=(string&& str);

        inline void operator=(const AnyType& any_type_value);

        inline void operator=(AnyType&& any_type_value);

    //group：类型判断

        inline bool isInteger();

        inline bool isNumber();

        inline bool isString();

    //group：取值、获取类型

        template <class T, class = typename enable_if<is_same<T, int64_t>::value || is_same<T, double>::value || is_same<T, string>::value>::type>
        inline T value();

        inline TYPE_E type();

    private:

    //group：模拟静态数据成员

        inline static map<TYPE_E, string>& s_mapTypeString();

        inline static map<TYPE_E, string> initTypeString();

    //group：数据成员

        TYPE_E enType;    //数据类型
        void* pValue = nullptr;    //存放数据的指针

    };    //AnyType

    /*******************************************************************************************************
    *                                             类实现
    *******************************************************************************************************/
    inline AnyType::AnyType(int32_t&& integer)
    {
        int64_t* pINT64 = new int64_t;
        *pINT64 = move(integer);
        pValue = pINT64;
        enType = INTEGER;
    }

    inline AnyType::AnyType(int64_t&& integer)
    {
        int64_t* pINT64 = new int64_t;
        *pINT64 = move(integer);
        pValue = pINT64;
        enType = INTEGER;
    }

    inline AnyType::AnyType(float&& number)
    {
        double* pDOUBLE = new double;
        *pDOUBLE = move(number);
        pValue = pDOUBLE;
        enType = NUMBER;
    }

    inline AnyType::AnyType(double&& number)
    {
        double* pDOUBLE = new double;
        *pDOUBLE = move(number);
        pValue = pDOUBLE;
        enType = NUMBER;
    }

    inline AnyType::AnyType(string&& str)
    {
        string* pSTRING = new string;
        *pSTRING = move(str);
        pValue = pSTRING;
        enType = STRING;
    }

    inline AnyType::AnyType(const AnyType& any_type_value)
    {
        if (INTEGER == any_type_value.enType)
        {
            int64_t* pINT64_New = new int64_t;
            int64_t* pINT64_Old = reinterpret_cast<int64_t*>(any_type_value.pValue);
            *pINT64_New = *pINT64_Old;
            pValue = pINT64_New;
            enType = any_type_value.enType;
        }
        else if (STRING == any_type_value.enType)
        {
            string* pSTRING_New = new string;
            string* pSTRING_Old = reinterpret_cast<string*>(any_type_value.pValue);
            *pSTRING_New = *pSTRING_Old;
            pValue = pSTRING_New;
            enType = any_type_value.enType;
        }
        else if (NUMBER == any_type_value.enType)
        {
            double* pDOUBLE_New = new double;
            double* pDOUBLE_Old = reinterpret_cast<double*>(any_type_value.pValue);
            *pDOUBLE_New = *pDOUBLE_Old;
            pValue = pDOUBLE_New;
            enType = any_type_value.enType;
        }
    }

    inline AnyType::AnyType(AnyType&& any_type_value)
    {
        pValue = any_type_value.pValue;
        any_type_value.pValue = nullptr;
        enType = any_type_value.enType;
    }

    inline AnyType::~AnyType()
    {
        if (INTEGER == enType)
            delete reinterpret_cast<int64_t*>(pValue);
        else if (STRING == enType)
            delete reinterpret_cast<string*>(pValue);
        else
            delete reinterpret_cast<double*>(pValue);
    }

    inline void AnyType::operator=(int32_t&& interger)
    {
        if (INTEGER == enType)
        {
            int64_t* pINT64 = reinterpret_cast<int64_t*>(pValue);
            *pINT64 = move(interger);
        }
        else
        {
            throw runtime_error("type error, current type is " + ms_mapTypeString[enType]);
        }
    }

    inline void AnyType::operator=(int64_t&& interger)
    {
        if (INTEGER == enType)
        {
            int64_t* pINT64 = reinterpret_cast<int64_t*>(pValue);
            *pINT64 = move(interger);
        }
        else
        {
            throw runtime_error("type error, current type is " + ms_mapTypeString[enType]);
        }
    }

    inline void AnyType::operator=(double&& number)
    {
        if (NUMBER == enType)
        {
            double* pDOUBLE = reinterpret_cast<double*>(pValue);
            *pDOUBLE = move(number);
        }
        else
        {
            throw runtime_error("type error, current type is " + ms_mapTypeString[enType]);
        }
    }

    inline void AnyType::operator=(string&& str)
    {
        if (STRING == enType)
        {
            string* pSTRING = reinterpret_cast<string*>(pValue);
            *pSTRING = move(str);
        }
        else
        {
            throw runtime_error("type error, current type is " + ms_mapTypeString[enType]);
        }
    }

    inline void AnyType::operator=(const AnyType& any_type_value)
    {
        if (INTEGER == any_type_value.enType)
        {
            int64_t* pINT64_New = new int64_t;
            int64_t* pINT64_Old = reinterpret_cast<int64_t*>(any_type_value.pValue);
            *pINT64_New = *pINT64_Old;
            pValue = pINT64_New;
            enType = any_type_value.enType;
        }
        else if (STRING == any_type_value.enType)
        {
            string* pSTRING_New = new string;
            string* pSTRING_Old = reinterpret_cast<string*>(any_type_value.pValue);
            *pSTRING_New = *pSTRING_Old;
            pValue = pSTRING_New;
            enType = any_type_value.enType;
        }
        else if (NUMBER == any_type_value.enType)
        {
            double* pDOUBLE_New = new double;
            double* pDOUBLE_Old = reinterpret_cast<double*>(any_type_value.pValue);
            *pDOUBLE_New = *pDOUBLE_Old;
            pValue = pDOUBLE_New;
            enType = any_type_value.enType;
        }
    }

    inline void AnyType::operator=(AnyType&& any_type_value)
    {
        pValue = any_type_value.pValue;
        any_type_value.pValue = nullptr;
        enType = any_type_value.enType;
    }

    inline bool AnyType::isInteger()
    {
        return INTEGER == enType;
    }

    inline bool AnyType::isNumber()
    {
        return NUMBER == enType;
    }

    inline bool AnyType::isString()
    {
        return STRING == enType;
    }

    template <class T, class = typename enable_if<is_same<T, int64_t>::value || is_same<T, double>::value || is_same<T, string>::value>::type>
    inline T AnyType::value()
    {
        if ((INTEGER == enType && is_same<T, int64_t>()) || (STRING == enType && is_same<T, string>()) || (NUMBER == enType && is_same<T, double>()))
        {
            T* pPypeT = reinterpret_cast<T*>(pValue);
            return *pPypeT;
        }
        else
        {
            throw runtime_error("type error, current type is " + ms_mapTypeString[enType]);
        }
    }

    inline AnyType::TYPE_E AnyType::type()
    {
        return enType;
    }

    inline map<AnyType::TYPE_E, string>& AnyType::s_mapTypeString()
    {
        static map<TYPE_E, string> s_mapTypeString{ initTypeString() };
        return s_mapTypeString;
    }

    inline map<AnyType::TYPE_E, string> AnyType::initTypeString()
    {
        map<TYPE_E, string> mapTypeString;
        mapTypeString[INTEGER] = "int64_t";
        mapTypeString[NUMBER] = "double";
        mapTypeString[STRING] = "string";
        return mapTypeString;
    }

}    //sinicheveen

#endif    //ANY_TYPE_

/*******************************************************************************************************
*                                            用法示例
*******************************************************************************************************/
#if 0
#include <iostream>
#include <vector>

#include "AnyType.hpp"

using namespace std;
using namespace sinicheveen;

int main()
{
    vector<AnyType> vAnyType;
    vAnyType.emplace_back(1);
    vAnyType.emplace_back(1.2);
    vAnyType.emplace_back("zhangdongyu");

    for (auto& iElem : vAnyType)
    {
        if (iElem.isInteger())
        {
            cout << iElem.value<int64_t>() << endl;
        }
        else if (iElem.isNumber())
        {
            cout << iElem.value<double>() << endl;
        }
        else
        {
            cout << iElem.value<string>() << endl;
        }
    }

    cout << endl;

    //四个变量具有相同的字节数。因为类的成员变量有两个，枚举类型占用 4 字节（枚举类型就是 int 型），地址占 8 个字节，再因为字节对齐，所以一个对象占用 16 字节
    //如果加上「#pragma pack(1)」，则只占用 12 个字节
    AnyType a, b(1), c(1.2), d("zhangdongyu");
    cout << sizeof(a) << endl;
    cout << sizeof(b) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(d) << endl;

    return 0;
}
#endif