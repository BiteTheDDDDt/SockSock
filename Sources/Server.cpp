#include "Server.h"
const std::string filename="Server.cpp";
Server::Server()
{
    const std::string funcname="Server()";
    try
    {
        //socket信息设置
        server_socket.sin_family=PF_INET;//随便设置
        server_socket.sin_port=htons(CDEF::server_port);//设置端口
        server_socket.sin_addr.s_addr=inet_addr(CDEF::server_ip);//设置IP

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
    try
    {
        while(true)
        {
            std::cout<<"server blocking..."<<std::endl;
            int epoll_events_count = epoll_wait(epoll_fd, events, CDEF::EPOLL_SIZE, -1);//就绪事件的数目
    
            if(epoll_events_count < 0)throw errno;
    
            std::cout<<"epoll_events_count ="<<epoll_events_count<<std::endl;
    
            //处理就绪事件
            for(int i = 0; i < epoll_events_count; i++)
            {
                int sockfd = events[i].data.fd;
                //新用户连接
                if(sockfd == listener)
                {
                    sockaddr_in client_address;
                    socklen_t client_addrLength = sizeof(struct sockaddr_in);
                    int clientfd = accept(listener, (sockaddr*)&client_address, &client_addrLength);
    
                    std::cout << "client connection from: "
                        << inet_ntoa(client_address.sin_addr) << ":"
                        << ntohs(client_address.sin_port) << ", clientfd = "
                        << clientfd << std::endl;
    
                    CDEF::addFd(epoll_fd, clientfd, true);
    
                    // 服务端用list保存用户连接
                    clients.push_back(clientfd);
                    std::cout << "Add new clientfd = " << clientfd << " to epoll" << std::endl;
                    std::cout << "Now there are " << clients.size() << " clients int the chat room" << std::endl;
    
                    // 服务端发送欢迎信息  
                    std::cout << "welcome message" << std::endl;
                    char message[CDEF::BUFF_SIZE];
                    bzero(message, CDEF::BUFF_SIZE);
                    sprintf(message, "SERVER_WELCOME %d", clientfd);
                    if(send(clientfd, message, CDEF::BUFF_SIZE, 0)==-1)throw errno;
                }
                //处理用户发来的消息，并广播，使其他用户收到信息
                else
                {
                    if(sendBroadcast(sockfd)==-1)throw errno;
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
int Server::sendBroadcast(int clientfd)
{

}