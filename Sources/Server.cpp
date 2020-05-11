#include "Server.h"
const std::string filename="Server.cpp";
Server::Server()
{
    const std::string funcname="Server()";
    try
    {
        //socket信息设置
        server_socket.sin_family=PF_INET;//设置模式
        server_socket.sin_port=htons(CDEF::SERVER_PORT);//设置端口
        server_socket.sin_addr.s_addr=inet_addr(CDEF::SERVER_IP);//设置IP

        //生成socket
        std::cout<<"server initing..."<<std::endl;
        listener=socket(PF_INET, SOCK_STREAM, 0);
        if(listener<0)throw errno;

        std::cout<<"server binding..."<<std::endl;
        //绑定信息
        if(bind(listener,(sockaddr *)(&server_socket),sizeof(server_socket))==-1)throw errno;

        //开始监听
        if(listen(listener,5)<0)throw errno;
        std::cout<<"server listening..."<<std::endl;

        //在内核中创建事件表 epfd是一个句柄 
        std::cout<<"epoll creating..."<<std::endl;
        epoll_fd=epoll_create(10);
        if(epoll_fd<0)throw errno;

        //往事件表里添加监听事件
        CDEF::addFd(epoll_fd,listener,true);
        std::cout<<"server inited."<<std::endl;
    }
    catch(int err)
    {
        CDEF::deBug(filename,funcname,err);
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
    const std::string funcname="start()";
    epoll_event events[CDEF::EPOLL_SIZE];//epoll事件队列

    std::cout<<"server started"<<std::endl;
    //main loop
    CDEF::Messege msg;
    char format_message[CDEF::BUFF_SIZE];
    try
    {
        while(true)
        {
            /*
            //非阻塞读取退出
            int key_fd = key_fd = fcntl(0,F_GETFL)|O_NONBLOCK;
            memset(format_message,0,sizeof(format_message));
            int res = read(0,format_message,CDEF::BUFF_SIZE);
            */

            std::cout<<"server blocking..."<<std::endl;
            int epoll_events_count = epoll_wait(epoll_fd, events, CDEF::EPOLL_SIZE, -1);//就绪事件的数目
    
            if(epoll_events_count < 0)throw errno;
    
            std::cout<<"epoll_events_count ="<<epoll_events_count<<std::endl;
    
            //处理就绪事件
            for(int i = 0; i < epoll_events_count; i++)
            {
                int sock_fd = events[i].data.fd;
                //新用户连接
                if(sock_fd == listener)
                {
                    sockaddr_in client_address;
                    socklen_t client_addrLength = sizeof(struct sockaddr_in);
                    int client_fd = accept(listener, (sockaddr*)&client_address, &client_addrLength);
    
                    std::cout << "client connection from: "
                        << inet_ntoa(client_address.sin_addr) << ":"
                        << ntohs(client_address.sin_port) << ", clientfd = "
                        << client_fd << std::endl;
    
                    CDEF::addFd(epoll_fd, client_fd, true);
    
                    // 服务端用list保存用户连接
                    clients.push_back(client_fd);
                    std::cout << "Add new clientfd = " << client_fd << " to epoll" << std::endl;
                    std::cout << "Now there are " << clients.size() << " clients int the chat room" << std::endl;
    
                    // 服务端发送欢迎信息  
                    sprintf(format_message, "server: client %d coming.\n",  client_fd);
                    memcpy(msg.content,format_message,sizeof(msg.content));
                    sendMessage(msg);
                }
                //处理用户发来的消息，并广播，使其他用户收到信息
                else
                {
                    if(recvMessage(sock_fd,msg))
                    {
                        //格式化消息
                        sprintf(format_message, "Client %d: %s", sock_fd, msg.content);
                        memcpy(msg.content,format_message,sizeof(msg.content));
                        sendMessage(msg);
                    }
                }
            }
        }
    }
    catch(int err)
    {
        CDEF::deBug(filename,funcname,err);
    }
    // 关闭服务
    return close();
}
bool Server::sendMessage(CDEF::Messege msg)
{
    //std::cout<<"send:"<<msg.content<<std::endl;
    // 遍历客户端列表依次发送消息，需要判断不要给来源客户端发
    for(int client:clients)
    {
        //把发送的结构体转换为字符串
        if(send(client,msg.content, sizeof(msg.content), 0) < 0)return false;
    }
    return true;
}
bool Server::recvMessage(int client_fd,CDEF::Messege& msg)
{
    char recv_buff[CDEF::BUFF_SIZE];
    char send_buff[CDEF::BUFF_SIZE];
    memset(recv_buff,0,sizeof(recv_buff));
    std::cout << "read from client(clientID = " << client_fd << ")" << std::endl;
    int len = recv(client_fd, recv_buff, sizeof(recv_buff), 0);
    memset(&msg,0,sizeof(msg));
    memcpy(&msg,recv_buff,sizeof(msg));
    if(len == 0) 
    {
        ::close(client_fd);
        // 在客户端列表中删除该客户端
        clients.remove(client_fd);
        std::cout << "ClientID = " << client_fd 
             << " closed.\n now there are " 
             << clients.size()
             << " client in the char room"
             << std::endl;
        return false;
    }
    return true;
}