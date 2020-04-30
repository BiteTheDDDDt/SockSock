#ifndef  Server_H
#define  Server_H

#include "CommonDef.h"

class Server
{
private:
    
    std::list<int>clients;          //客户列表

    int sendBroadcast(int clientfd);//广播给所有client
    int listener;                   //监听socket
    int epoll_fd;                   //epoll_create创建后的返回值
    sockaddr_in server_socket;      //用于存放本机ip和端口的结构
public:
    Server();
    ~Server();
    bool close();
    bool start();
};

#endif