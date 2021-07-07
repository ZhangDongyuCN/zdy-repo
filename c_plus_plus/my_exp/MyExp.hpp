#ifndef _MY_EXP_
#define _MY_EXP_

/*
*Environment：
*Linux(Ubuntu), C++11，gcc 7.5.0，g++ 7.5.0
*Description：
*自定义异常类
*/

#include <exception>
#include <stdexcept>
#include <string>

using std::exception;
using std::runtime_error;
using std::string;
using std::move;

class MyExp1 :public exception
{
public:
    explicit MyExp1(const string& rMessage) :rMessage_(rMessage) {}
    explicit MyExp1(string&& rMessage) :rMessage_(move(rMessage)) {}
    virtual char const* what() const noexcept override { return rMessage_.c_str(); }    //必须加 noexcept，否则编译不通过
private:
    string rMessage_;
};

class MyExp2 :public exception
{
public:
    explicit MyExp2(const string& rMessage) :runtimeError_(rMessage) {}
    explicit MyExp2(string&& rMessage) :runtimeError_(move(rMessage)) {}
    virtual char const* what() const noexcept override { return runtimeError_.what(); }    //必须加 noexcept，否则编译不通过
private:
    runtime_error runtimeError_;
};

#endif    //_MY_EXP_