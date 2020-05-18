#include "Server.h"
const std::string filename = "Server.cpp";
Server::Server()
{
    const std::string funcname = "Server()";
    try
    {
        //socket信息设置
        server_socket.sin_family = PF_INET;                         //设置模式
        server_socket.sin_port = htons(CDEF::SERVER_PORT);          //设置端口
        server_socket.sin_addr.s_addr = inet_addr(CDEF::SERVER_IP); //设置IP

        //生成socket
        std::cout << "Server initing..." << std::endl;
        listener = socket(PF_INET, SOCK_STREAM, 0);
        if (listener < 0)
            throw errno;

        //std::cout << "Server binding..." << std::endl;
        //绑定信息
        if (bind(listener, (sockaddr *)(&server_socket), sizeof(server_socket)) == -1)
            throw errno;

        //开始监听
        if (listen(listener, 5) < 0)
            throw errno;
        //std::cout << "Server listening..." << std::endl;

        //在内核中创建事件表 epfd是一个句柄
        //std::cout << "Epoll creating..." << std::endl;
        epoll_fd = epoll_create(10);
        if (epoll_fd < 0)
            throw errno;

        //往事件表里添加监听事件
        CDEF::addFd(epoll_fd, listener, true);
        CDEF::addFd(epoll_fd, 0, true); //0为标准输入流 用来监控退出
        std::cout << "Server inited." << std::endl;
    }
    catch (int err)
    {
        CDEF::deBug(filename, funcname, err);
    }
}
Server::~Server()
{
    close();
}
bool Server::close()
{
    ::close(listener);
    ::close(epoll_fd);
    return true;
}
bool Server::start()
{
    bool isRunning = true;
    const std::string funcname = "start()";
    epoll_event events[CDEF::EPOLL_SIZE]; //epoll事件队列

    std::cout << "Server started" << std::endl;
    //main loop
    CDEF::Messege msg;
    try
    {
        while (isRunning)
        {
            //std::cout << "server blocking..." << std::endl;
            int epoll_events_count = epoll_wait(epoll_fd, events, CDEF::EPOLL_SIZE, -1); //就绪事件的数目

            if (epoll_events_count < 0)
                throw errno;

            //std::cout << "epoll_events_count =" << epoll_events_count << std::endl;

            //处理就绪事件
            for (int i = 0; i < epoll_events_count; i++)
            {
                int sock_fd = events[i].data.fd;
                if (sock_fd == listener) //连接监听
                {
                    sockaddr_in client_address;
                    socklen_t client_addrLength = sizeof(struct sockaddr_in);
                    int client_fd = accept(listener, (sockaddr *)&client_address, &client_addrLength);

                    std::cout << "Connection from: "
                              << inet_ntoa(client_address.sin_addr) << ":"
                              << ntohs(client_address.sin_port) << std::endl;

                    CDEF::addFd(epoll_fd, client_fd, true);

                    // 服务端用list保存用户连接
                    clients.push_back(client_fd);

                    std::cout << "Client " << client_fd << " coming." << std::endl;
                    // 服务端发送欢迎信息
                    sprintf(buff, "Server: Client %d coming.\n", client_fd);
                    memcpy(msg.content, buff, sizeof(msg.content));
                    broadcastMessage(msg);
                }
                else if (sock_fd == 0) //标准IO
                {
                    if (read(0, buff, sizeof(buff)) > 0)
                    {
                        std::cout << "Standrad server quit." << std::endl;
                        isRunning = false;
                        break;
                    }
                }
                else //其他连接
                {
                    if (recvMessage(sock_fd, msg)) //接受消息
                    {
                        //格式化消息
                        sprintf(buff, "Client %d: %s", sock_fd, msg.content);
                        memcpy(msg.content, buff, sizeof(msg.content));
                        broadcastMessage(msg);
                    }
                    else //断开连接
                    {
                        ::close(sock_fd);
                        // 在客户端列表中删除该客户端
                        clients.remove(sock_fd);
                        std::cout << "Client " << sock_fd << " quit." << std::endl;
                    }
                }
            }
        }
    }
    catch (int err)
    {
        CDEF::deBug(filename, funcname, err);
    }
    // 关闭服务
    return close();
}
void Server::broadcastMessage(CDEF::Messege msg)
{
    for (int client : clients)
    {
        if (!sendMessage(client, msg))
        {
            std::cout << "Client " << client << " can't receive message." << std::endl;
        }
    }
}
bool Server::sendMessage(int client_fd, CDEF::Messege msg)
{
    //std::cout<<"send:"<<msg.content<<std::endl;
    return (send(client_fd, msg.content, sizeof(msg.content), 0) >= 0);
}
bool Server::recvMessage(int client_fd, CDEF::Messege &msg)
{
    //std::cout << "read from client(clientID = " << client_fd << ")" << std::endl;
    if (recv(client_fd, buff, sizeof(buff), 0) > 0)
    {
        memcpy(&msg, buff, sizeof(msg));
        return true;
    }
    else
    {
        return false;
    }
}