#include "LocalIp.h"
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

namespace sinicheveen
{
    bool LocalIp::getLocalIpByEthName(IN const string& ethName, OUT string& localIpAddress)
    {
        int error_code = false;
        int fd;
        struct ifreq ifr;

        if (ethName.empty())
        {
            return error_code;
        }

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
        {
            strcpy(ifr.ifr_name, ethName.c_str());
            if (!(ioctl(fd, SIOCGIFADDR, &ifr)))
            {
                error_code = true;
                localIpAddress = inet_ntoa(((struct sockaddr_in*) & ifr.ifr_addr)->sin_addr);
            }
        }

        if (fd > 0)
        {
            close(fd);
        }

        return error_code;
    }    //getLocalIpByEthName()

    bool LocalIp::getAllLocalIps(OUT map<string, string>& ethAndIps)
    {
        bool error_code = false;
        int fd, intrface;
        struct ifreq ifr[32];
        struct ifconf ifc;
        char* eth_name[50], local_ip_address[50];

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
        {
            ifc.ifc_len = sizeof(ifr);
            ifc.ifc_buf = (caddr_t)ifr;
            if (!ioctl(fd, SIOCGIFCONF, (char*)&ifc))    //获取所有接口信息
            {
                intrface = ifc.ifc_len / sizeof(struct ifreq);
                while (intrface-- > 0)
                {
                    //get eth name and ip address
                    if (!(ioctl(fd, SIOCGIFADDR, (char*)&ifr[intrface])))
                    {
                        error_code = true;
                        ethAndIps[ifr[intrface].ifr_name] = inet_ntoa(((struct sockaddr_in*)(&ifr[intrface].ifr_addr))->sin_addr);
                    }
                }
            }
        }

        if (fd > 0)
        {
            close(fd);
        }

        return error_code;
    }    //getAllLocalIps()

}    //sinicheveen
