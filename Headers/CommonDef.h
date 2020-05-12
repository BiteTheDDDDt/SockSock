#ifndef CommonDef_H
#define CommonDef_H

#include <atomic>
#include <mutex>
#include <list>
#include <iostream>
#include <string>

#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <csignal>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>

namespace CDEF //公共定义常量/函数
{
    const int SERVER_PORT = 18888;        //服务端默认端口
    const char SERVER_IP[] = "127.0.0.1"; //服务端公网IP
    //114.55.95.144
    const int EPOLL_SIZE = 1024;
    const int BUFF_SIZE = 1024;

    //输出debug信息
    static void deBug(const std::string &file, const std::string &func, int err, const std::string &desc = "none")
    {
        std::cout << "---deBug---" << std::endl;
        std::cout << "file: " << file << std::endl;
        std::cout << "function: " << func << std::endl;
        std::cout << "infomation: " << strerror(err) << std::endl;
        std::cout << "describe: " << desc << std::endl;
        std::cout << "----------" << std::endl;
    }

    static void addFd(int epollfd, int fd, bool enable_et)
    {
        epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN;
        if (enable_et)
            ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
        // 设置socket为非阻塞模式
        // 套接字立刻返回，不管I/O是否完成，该函数所在的线程会继续运行
        //eg. 在recv(fd...)时，该函数立刻返回，在返回时，内核数据还没准备好会返回WSAEWOULDBLOCK错误代码
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
        std::cout << "fd:" << fd << " "
                  << "added to epoll" << std::endl;
    }
    struct Messege //定义信息结构，在服务端和客户端之间传送
    {
        char content[BUFF_SIZE];
    };
} // namespace CDEF
#endif