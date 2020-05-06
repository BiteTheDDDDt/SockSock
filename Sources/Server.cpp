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
                    char message[CDEF::BUFF_SIZE];
                    memset(message,0,sizeof(message));
                    sprintf(message, "welcome user%d.", clientfd);
                    std::cout << "welcome message:" <<message<< std::endl;
                    if(send(clientfd, message, strlen(message), 0)==-1)throw errno;
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
    // buf[BUF_SIZE] 接收新消息
    // message[BUF_SIZE] 保存格式化的消息
    char recv_buff[CDEF::BUFF_SIZE];
    char send_buff[CDEF::BUFF_SIZE];
    CDEF::Messege msg;
    memset(recv_buff,0,sizeof(recv_buff));
    // 接收新消息
    std::cout << "read from client(clientID = " << clientfd << ")" << std::endl;
    int len = recv(clientfd, recv_buff, sizeof(recv_buff), 0);
    //清空结构体，把接受到的字符串转换为结构体
    memset(&msg,0,sizeof(msg));
    memcpy(&msg,recv_buff,sizeof(msg));
    //判断接受到的信息是私聊还是群聊
    msg.fromID=clientfd;
    if(msg.content[0]=='\\'&&isdigit(msg.content[1]))
    {
        msg.type=1;
        msg.toID=msg.content[1]-'0';
        memcpy(msg.content,msg.content+2,sizeof(msg.content));
    }
    else msg.type=0;
    // 如果客户端关闭了连接
    if(len == 0) 
    {
        ::close(clientfd);
        
        // 在客户端列表中删除该客户端
        clients.remove(clientfd);
        std::cout << "ClientID = " << clientfd 
             << " closed.\n now there are " 
             << clients.size()
             << " client in the char room"
             << std::endl;
 
    }
    // 发送广播消息给所有客户端
    else 
    {
        //存放格式化后的信息
        char format_message[CDEF::BUFF_SIZE];
        //群聊
        if(msg.type==0)
        {
            // 格式化发送的消息内容 #define SERVER_MESSAGE "ClientID %d say >> %s"
            sprintf(format_message, "ClientID %d say >> %s", clientfd, msg.content);
            memcpy(msg.content,format_message,sizeof(msg.content));
            // 遍历客户端列表依次发送消息，需要判断不要给来源客户端发
            for(auto it = clients.begin(); it != clients.end(); ++it)
            {
                //把发送的结构体转换为字符串
                memset(send_buff,0,sizeof(send_buff));
                memcpy(send_buff,&msg,sizeof(msg));
                if( send(*it,send_buff, sizeof(send_buff), 0) < 0 )return -1;
            }
        }
    }
    return len;
}