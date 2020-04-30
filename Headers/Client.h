#ifndef  Client_H
#define  Client_H

#include "CommonDef.h"

class Client
{
private:
    int sock;//当前连接服务器端创建的socket
    int pid;//当前进程ID
    int epfd;//epoll_create创建后的返回值
    int pipe_fd[2];// 创建管道，其中fd[0]用于父进程读，fd[1]用于子进程写
    bool isClientwork;// 表示客户端是否正常工作
    CDEF::Messege msg;// 聊天信息
    //结构体要转换为字符串
    char send_buf[CDEF::BUFF_SIZE];
    char recv_buf[CDEF::BUFF_SIZE];
    sockaddr_in serverAddr;//用于存放服务器ip和端口的结构
public:
    Client();
    ~Client();
    void connect();
    void close();
    void start();
};

#endif