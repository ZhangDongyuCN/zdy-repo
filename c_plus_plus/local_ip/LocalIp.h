#pragma once

#define IN
#define OUT
#define INOUT

#include <map>
#include <string>

using std::map;
using std::string;

namespace sinicheveen
{
    class LocalIp
    {
    public:

        static bool getLocalIpByEthName(IN const string& ethName, OUT string& localIpAddress);

        static bool getAllLocalIps(OUT map<string, string>& ethAndIps);

    };

}    //sinicheveen
