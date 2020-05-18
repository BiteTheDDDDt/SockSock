#include "Client.h"
const std::string filename = "Client.cpp";
Client::Client()
{
    isClientwork = false;
}
Client::~Client()
{
}
bool Client::initConnection(const char *ip, const int &port)
{
    std::cout << "Connect Server: " << ip << " : " << port << std::endl;
    server_socket.sin_family = PF_INET;            //设置模式
    server_socket.sin_port = htons(port);          //设置端口
    server_socket.sin_addr.s_addr = inet_addr(ip); //设置IP

    // 创建socket
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("sock error");
        return false;
    }

    // 连接服务端
    if (connect(sock, (sockaddr *)&server_socket, sizeof(server_socket)) < 0)
    {
        perror("connect error");
        return false;
    }

    // 创建管道，其中fd[0]用于父进程读，fd[1]用于子进程写
    if (pipe(pipe_fd) < 0)
    {
        perror("pipe error");
        return false;
    }

    // 创建epoll
    if ((epoll_fd = epoll_create(CDEF::EPOLL_SIZE)) < 0)
    {
        perror("epfd error");
        return false;
    }

    //将sock和管道读端描述符都添加到内核事件表中
    CDEF::addFd(epoll_fd, sock, true);
    CDEF::addFd(epoll_fd, pipe_fd[0], true);

    isClientwork = true;
}
void Client::close()
{
    if (pid)
    {
        //关闭父进程的管道和sock
        ::close(pipe_fd[0]);
        ::close(sock);
    }
    else
    {
        //关闭子进程的管道
        ::close(pipe_fd[1]);
    }
}
void Client::start()
{
    const std::string funcname = "start()";
    // epoll 事件队列
    static struct epoll_event events[2];
    //结构体要转换为字符串
    char send_buff[CDEF::BUFF_SIZE];
    char recv_buff[CDEF::BUFF_SIZE];
    CDEF::Messege msg;
    try
    {
        pid = fork();
        if (pid < 0)
        {
            // 如果创建子进程失败则退出
            ::close(sock);
            throw errno;
        }
        else if (pid == 0)
        {
            // 进入子进程执行流程
            ::close(pipe_fd[0]);
            std::cout << "Please input 'EXIT' to exit the chat room" << std::endl;
            // 如果客户端运行正常则不断读取输入发送给服务端
            while (isClientwork)
            {
                //清空结构体
                memset(msg.content, 0, sizeof(msg.content));
                fgets(msg.content, CDEF::BUFF_SIZE, stdin);
                // 客户输出exit,退出
                if (strncasecmp(msg.content, "EXIT", strlen("EXIT")) == 0)
                {
                    isClientwork = false;
                }
                else
                {
                    // 子进程将信息写入管道
                    //清空发送缓存
                    memset(send_buff, 0, sizeof(send_buff));
                    //结构体转换为字符串
                    memcpy(send_buff, &msg, sizeof(msg));
                    if (write(pipe_fd[1], send_buff, sizeof(send_buff)) < 0)
                        throw errno;
                }
            }
        }
        else
        {
            //pid > 0 为父进程
            //父进程负责读管道数据，因此先关闭写端
            ::close(pipe_fd[1]);

            while (isClientwork)
            {
                int epoll_events_count = epoll_wait(epoll_fd, events, 2, -1);

                //处理就绪事件
                for (int i = 0; i < epoll_events_count; i++)
                {
                    memset(recv_buff, 0, sizeof(recv_buff));
                    //服务端发来消息
                    if (events[i].data.fd == sock)
                    {
                        //接受服务端广播消息
                        int res = recv(sock, recv_buff, sizeof(recv_buff), 0);
                        //清空结构体
                        memset(&msg, 0, sizeof(msg));
                        //将发来的消息转换为结构体
                        memcpy(&msg, recv_buff, sizeof(msg));

                        // res= 0 服务端关闭
                        if (res == 0)
                        {
                            std::cout << "Server closed connection: " << sock << std::endl;
                            ::close(sock);
                            isClientwork = 0;
                        }
                        else
                        {
                            std::cout << msg.content;
                        }
                    }
                    //子进程写入事件发生，父进程处理并发送服务端
                    else
                    {
                        //父进程从管道中读取数据
                        int res = read(events[i].data.fd, recv_buff, sizeof(recv_buff));
                        // ret = 0
                        if (res == 0)
                        {
                            isClientwork = 0;
                        }
                        else
                        {
                            // 将从管道中读取的字符串信息发送给服务端
                            send(sock, recv_buff, sizeof(recv_buff), 0);
                        }
                    }
                }
            }
        }
    }
    catch (int err)
    {
        CDEF::deBug(filename, funcname, err);
    }
    close();
}