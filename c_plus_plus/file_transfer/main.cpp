#include <iostream>
#include "file_transfer.h"
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[])
{
    if (1 == argc)
    {
        cout << "usage: file_transfer <IPv4 address> <pub-sub port> <sync port> <subscribers num> <send_file_name> <recv_file_name> <send | recv>" << endl;
        return 0;
    }

    string ipv4_address = argv[1];
    string pub_sub_port = argv[2];
    string sync_port = argv[3];
    string subscribers_num = argv[4];
    string send_file_name = argv[5];
    string recv_file_name = argv[6];
    string send_or_recv = argv[7];

    if ("send" == send_or_recv)
    {
        file_transfer::zmq_pub_sub_send_file(ipv4_address, pub_sub_port, sync_port, stoi(subscribers_num), send_file_name);
    }
    else
    {
        file_transfer::zmq_pub_sub_recv_file(ipv4_address, pub_sub_port, sync_port, recv_file_name);
    }

    return 0;
}