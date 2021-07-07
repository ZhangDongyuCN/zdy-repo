#include "file_transfer.h"

bool file_transfer::socket_send_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //检查文件是否存在
    FILE* fp = fopen(file_name.c_str(), "rb");    //以二进制方式打开文件
    if (NULL == fp)
    {
        cout << file_name << " open failed" << endl;
        fclose(fp);
        return false;
    }

    //创建套接字
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //将套接字和IP、端口绑定
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));    //每个字节都用 0 填充
    server_address.sin_family = AF_INET;    //使用IPv4地址
    server_address.sin_addr.s_addr = inet_addr(ipv4_address.c_str());    //IP地址
    server_address.sin_port = htons(stoi(port));    //端口
    if (-1 == bind(server_socket, (struct sockaddr*) & server_address, sizeof(server_address))) 
    { 
        cout << "socket bind failed" << endl;
        fclose(fp);
        close(server_socket);
        return false;
    }

    //进入监听状态，等待用户发起请求
    listen(server_socket, 20);

    //接收客户端请求
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*) & client_address, &client_address_size);

    //循环发送数据，直到文件结尾
    char buffer[FILE_TRANSFER_BUFFER_SIZE];    //文件缓冲区
    int count;
    while ((count = fread(buffer, 1, FILE_TRANSFER_BUFFER_SIZE, fp)) > 0)
    {
        send(client_socket, buffer, count, 0);
    }
    shutdown(client_socket, SHUT_WR);    //文件读取完毕，断开输出流，向客户端发送 FIN 包
    recv(client_socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0);    //阻塞，等待客户端接收完毕

    //关闭文件和套接字
    fclose(fp);
    close(client_socket);
    close(server_socket);
    return true;
}    //socket_send_file()

bool file_transfer::socket_recv_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //打开（创建）文件
    FILE* fp = fopen(file_name.c_str(), "wb");    //以二进制方式打开（创建）文件
    if (NULL == fp)
    {
        cout << file_name << " create failed" << endl;
        fclose(fp);
        remove(file_name.c_str());
        return false;
    }

    //创建套接字
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));    //每个字节都用 0 填充
    server_address.sin_family = AF_INET;    //使用IPv4地址
    server_address.sin_addr.s_addr = inet_addr(ipv4_address.c_str());    //具体的IP地址
    server_address.sin_port = htons(stoi(port));    //端口
    int connect_count = 0;
    while (-1 == connect(client_socket, (struct sockaddr*) & server_address, sizeof(server_address)))
    {
        cout << "socket connecting ..." << endl;
        connect_count++;
        if (FILE_TRANSFER_SOCKET_CONNECT_COUNT + 1 == connect_count)
        {
            cout << "socket connect failed" << endl;
            fclose(fp);
            remove(file_name.c_str());
            return false;
        }
        sleep(1);
    }

    //循环接收数据，直到文件传输完毕
    char buffer[FILE_TRANSFER_BUFFER_SIZE];    //文件缓冲区
    int count;
    while ((count = recv(client_socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, count, 1, fp);
    }
    
    //关闭文件和套接字
    close(client_socket);
    fclose(fp);
    return true;
}    //socket_recv_file()

bool file_transfer::zmq_req_rep_send_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //检查文件是否存在
    FILE* fp = fopen(file_name.c_str(), "rb");    //以二进制方式打开文件
    if (NULL == fp)
    { 
        cerr << "zmq_req_rep_send_file(): fopen(): error: " << file_name << " open failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        return false;
    }

    //创建 zmq socket 并建立连接
    void* context = zmq_ctx_new();
    void* socket = zmq_socket(context, ZMQ_REQ);
    zmq_setsockopt(socket, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //设置接收超时时间
    zmq_setsockopt(socket, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                          //Set linger period for socket shutdown
    string url = "tcp://" + ipv4_address + ":" + port;
    if (-1 == zmq_connect(socket, url.c_str()))
    {
        cerr << "zmq_req_rep_send_file(): zmq_connect(): error: zmq connect " << url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //循环发送数据，直到文件结尾
    char buffer[FILE_TRANSFER_BUFFER_SIZE];
    int count;
    while ((count = fread(buffer, 1, FILE_TRANSFER_BUFFER_SIZE, fp)) > 0)
    {
        //发送文件内容
        zmq_send(socket, buffer, count, 0);

        //接收回复消息
        if (-1 == zmq_recv(socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_req_rep_send_file(): zmq_recv(): error: receive file content reply message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
    }

    //发送文件结束标志
    zmq_send(socket, FILE_TRANSFER_FILE_END_FLAG, FILE_TRANSFER_FILE_END_FLAG_LEN, 0);
    if (-1 == zmq_recv(socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
    {
        cerr << "zmq_req_rep_send_file(): zmq_recv(): error: receive file end flag reply message timeout, error code " << to_string(errno) + ", error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //关闭文件和 zmq 连接，然后返回 true
    cout << "zmq_req_rep_send_file(): " << file_name << " send successfully." << endl;
    fclose(fp);
    zmq_close(socket);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //关闭文件和 zmq 连接，然后返回 false
    fclose(fp);
    zmq_close(socket);
    zmq_ctx_term(context);
    return false;
}    //zmq_req_rep_send_file()

bool file_transfer::zmq_req_rep_recv_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //打开（创建）文件
    FILE* fp = fopen(file_name.c_str(), "wb");    //以二进制方式打开（创建）文件
    if (NULL == fp)
    {
        cerr << "zmq_req_rep_recv_file(): fopen(): error: " << file_name << " create failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        remove(file_name.c_str());
        return false;
    }

    //创建 zmq socket 并建立连接
    void* context = zmq_ctx_new();
    void* socket = zmq_socket(context, ZMQ_REP);
    zmq_setsockopt(socket, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //设置接收超时时间
    zmq_setsockopt(socket, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                //Set linger period for socket shutdown
    string url = "tcp://" + ipv4_address + ":" + port;
    if (-1 == zmq_bind(socket, url.c_str()))
    {
        cerr << "zmq_req_rep_recv_file(): zmq_bind(): error: zmq bind " << url << " failed, error code " + to_string(errno) << ", error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //循环接收数据，直到文件结尾
    char buffer[FILE_TRANSFER_BUFFER_SIZE];    //文件缓冲区
    int count;
    while (true)
    {
        //接收消息
        count = zmq_recv(socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0);
        if (-1 == count)
        {
            cerr << "zmq_req_rep_recv_file(): zmq_recv(): error: receive file content message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }

        //写文件
        if (0 != memcmp(buffer, FILE_TRANSFER_FILE_END_FLAG, count >= FILE_TRANSFER_FILE_END_FLAG_LEN ? FILE_TRANSFER_FILE_END_FLAG_LEN : count))
        {
            fwrite(buffer, count, 1, fp);
        }
        else
        {
            //回复文件结束确认消息
            zmq_send(socket, "OK", 2, 0);

            //跳出循环
            break;
        }

        //回复文件内容确认消息
        zmq_send(socket, "OK", 2, 0);
    }

    //关闭文件和 zmq 连接，然后返回 true
    cout << "zmq_req_rep_recv_file(): " << file_name << " receive successfully." << endl;
    fclose(fp);
    zmq_close(socket);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //关闭并删除文件，关闭 zmq 连接，返回 false
    fclose(fp);
    remove(file_name.c_str());
    zmq_close(socket);
    zmq_ctx_term(context);
    return false;
}    //zmq_req_rep_recv_file()

bool file_transfer::zmq_pub_sub_send_file(IN const string& ipv4_address, IN const string& pub_port, IN const string& sync_port, IN const int subscribers_num, IN const string& file_name)
{
    //检查文件是否存在
    FILE* fp = fopen(file_name.c_str(), "rb");    //以二进制方式打开文件
    if (NULL == fp)
    {
        cerr << "zmq_pub_sub_send_file(): fopen(): error: " << file_name << " open failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        return false;
    }

    //创建 zmq context
    void* context = zmq_ctx_new();

    //创建发送文件套接字
    void* publisher = zmq_socket(context, ZMQ_PUB);
    string pub_url = "tcp://" + ipv4_address + ":" + pub_port;

    //创建信号同步套接字
    void* sync_requester = zmq_socket(context, ZMQ_REP);
    string rep_url = "tcp://" + ipv4_address + ":" + sync_port;

    //设置选项
    zmq_setsockopt(publisher, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));         //设置接收超时时间
    zmq_setsockopt(publisher, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                     //Set linger period for socket shutdown
    zmq_setsockopt(sync_requester, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //设置接收超时时间
    zmq_setsockopt(sync_requester, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                //Set linger period for socket shutdown

    //绑定两个套接字
    if (-1 == zmq_bind(publisher, pub_url.c_str()))
    {
        cerr << "zmq_pub_sub_send_file(): zmq_bind(): error: publisher bind " << pub_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }
    if (-1 == zmq_bind(sync_requester, rep_url.c_str()))
    {
        cerr << "zmq_pub_sub_send_file(): zmq_bind(): error: sync_requester bind " << rep_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //接收订阅者的就绪信号
    cout << "zmq_pub_sub_send_file(): waiting all subscribers ready." << endl;
    char buffer[FILE_TRANSFER_BUFFER_SIZE];
    for (int subscribers = 0; subscribers < subscribers_num; subscribers++)
    {
        if (-1 == zmq_recv(sync_requester, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_pub_sub_send_file(): zmq_recv(): error: wait subscribers ready message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
        zmq_send(sync_requester, "OK", 2, 0);
    }
    cout << "zmq_pub_sub_send_file(): all subscribers are ready." << endl;

    //循环发送文件数据，直到文件结尾
    int count;
    while ((count = fread(buffer, 1, FILE_TRANSFER_BUFFER_SIZE, fp)) > 0)
    {
        //发送文件内容
        zmq_send(publisher, buffer, count, 0);

        //接收所有订阅者文件内容处理完毕消息
        for (int subscribers = 0; subscribers < subscribers_num; subscribers++)
        {
            if (-1 == zmq_recv(sync_requester, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
            {
                cerr << "zmq_pub_sub_send_file(): zmq_recv(): error: wait subscribers reply file content has been processed message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
                goto LABEL_FALSE;
            }
            zmq_send(sync_requester, "OK", 2, 0);
        }

        //等待一会，确保 subscriber 处理了 OK 消息
        usleep(FILE_TRANSFER_ZMQ_PUB_SLEEP_TIME);
    }

    //发送文件结束标志
    zmq_send(publisher, FILE_TRANSFER_FILE_END_FLAG, FILE_TRANSFER_FILE_END_FLAG_LEN, 0);
    for (int subscribers = 0; subscribers < subscribers_num; subscribers++)
    {
        if (-1 == zmq_recv(sync_requester, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_pub_sub_send_file(): zmq_recv(): error: wait subscribers reply file end flag has been processed message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
        zmq_send(sync_requester, "OK", 2, 0);
    }

    //关闭文件和 zmq 连接，然后返回 true
    cout << "zmq_pub_sub_send_file(): " << file_name << " send successfully." << endl;
    fclose(fp);
    zmq_close(publisher);
    zmq_close(sync_requester);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //关闭文件和 zmq 连接，然后返回 false
    fclose(fp);
    zmq_close(publisher);
    zmq_close(sync_requester);
    zmq_ctx_term(context);
    return false;
}

bool file_transfer::zmq_pub_sub_recv_file(IN const string& ipv4_address, IN const string& sub_port, IN const string& sync_port, IN const string& file_name)
{
    //打开（创建）文件
    FILE* fp = fopen(file_name.c_str(), "wb");    //以二进制方式打开（创建）文件
    if (NULL == fp)
    {
        cerr << "zmq_pub_sub_recv_file(): fopen(): error: " << file_name << " create failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        remove(file_name.c_str());
        return false;
    }

    //创建 zmq context
    void* context = zmq_ctx_new();

    //创建接收文件套接字
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    string sub_url = "tcp://" + ipv4_address + ":" + sub_port;

    //创建信号同步套接字
    void* sync_responser = zmq_socket(context, ZMQ_REQ);
    string req_url = "tcp://" + ipv4_address + ":" + sync_port;

    //设置选项
    zmq_setsockopt(subscriber, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));        //设置接收超时时间
    zmq_setsockopt(subscriber, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                    //Set linger period for socket shutdown
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);                                                                       //设置订阅模式
    zmq_setsockopt(sync_responser, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //设置接收超时时间
    zmq_setsockopt(sync_responser, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                //Set linger period for socket shutdown

    //连接两个套接字
    if(-1 == zmq_connect(subscriber, sub_url.c_str()))
    {
        cerr << "zmq_pub_sub_recv_file(): zmq_connect(): error: subscriber connect " << sub_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }
    if(-1 == zmq_connect(sync_responser, req_url.c_str()))
    {
        cerr << "zmq_pub_sub_recv_file(): zmq_connect(): error: sync_responser connect " << req_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //发送同步信号
    zmq_send(sync_responser, "OK", 2, 0);

    //等待同步应答信号
    char buffer[FILE_TRANSFER_BUFFER_SIZE];
    if(-1 == zmq_recv(sync_responser, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
    {
        cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: wait publisher reply ready signal confirm message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //循环接收文件内容
    int count;
    while (true)
    {
        //接收文件内容
        count = zmq_recv(subscriber, buffer, FILE_TRANSFER_BUFFER_SIZE, 0);
        if (-1 == count)
        {
            cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: receive file content message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }

        //写文件
        if (0 != memcmp(buffer, FILE_TRANSFER_FILE_END_FLAG, count >= FILE_TRANSFER_FILE_END_FLAG_LEN ? FILE_TRANSFER_FILE_END_FLAG_LEN : count))
        {
            fwrite(buffer, count, 1, fp);
        }
        else
        {
            //回复文件结束标志已处理完毕消息
            zmq_send(sync_responser, "OK", 2, 0);
            if (-1 == zmq_recv(sync_responser, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
            {
                cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: wait publisher reply file end flag has been processed confirm message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
                goto LABEL_FALSE;
            }

            //跳出循环
            break;
        }

        //回复文件内容已处理完毕消息
        zmq_send(sync_responser, "OK", 2, 0);
        if (-1 == zmq_recv(sync_responser, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: wait publisher reply file content has been processed confirm message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
    }

    //关闭文件和 zmq 连接，然后返回 true
    cout << "zmq_pub_sub_recv_file(): " << file_name << " receive successfully." << endl;
    fclose(fp);
    zmq_close(subscriber);
    zmq_close(sync_responser);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //关闭并删除文件，关闭 zmq 连接，返回 false
    fclose(fp);
    remove(file_name.c_str());
    zmq_close(subscriber);
    zmq_close(sync_responser);
    zmq_ctx_term(context);
    return false;
}

string file_transfer::generate_file_end_flag(IN const int file_end_flag_characters_num)
{
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*+-/";

    int characters_size = characters.length();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, characters_size - 1);

    string file_end_flag;
    file_end_flag.reserve(file_end_flag_characters_num);
    for (int i = 0; i < file_end_flag_characters_num; i++)
    {
        file_end_flag += characters[distribution(generator)];
    }

    return file_end_flag;
}    //generate_file_end_flag()