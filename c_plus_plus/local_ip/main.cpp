#include <stdio.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <map>
#include <iostream>

#include "LocalIp.h"

using namespace std;

int main(int argc, const char** argv)
{
    map<string, string> a;
    if (sinicheveen::LocalIp::getAllLocalIps(a))
    {
        for (const auto& iElem : a)
        {
            cout << iElem.first << " " << iElem.second << endl;
        }
    }
    else
    {
        cout << "failed" << endl;
    }
    
    return 0;
}