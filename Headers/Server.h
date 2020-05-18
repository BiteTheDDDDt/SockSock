#ifndef Server_H
#define Server_H

#include "CommonDef.h"

class Server
{
private:
    std::list<int> clients; //客户列表

    int listener;              //监听socket
    int epoll_fd;              //epoll_create创建后的返回值
    int key_fd;                //标准输入
    sockaddr_in server_socket; //用于存放本机ip和端口的结构
    bool recvMessage(int client_fd, CDEF::Messege &msg);
    bool sendMessage(int client_fd, CDEF::Messege msg);
    void broadcastMessage(CDEF::Messege msg);
    char buff[CDEF::BUFF_SIZE];

public:
    Server();
    ~Server();
    bool close();
    bool start();
};

#endif