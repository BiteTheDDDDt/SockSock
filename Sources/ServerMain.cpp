#include <iostream>
#include "Server.h"

int main()
{
    std::cout<<"task starting..."<< std::endl;
    try
    {
        Server serv=Server();
        serv.start();
        std::cout<<"task endding..."<< std::endl;
    }
    catch(int err)
    {
        std::cout<<"task have some trouble."<< std::endl;
    }
    return 0;
}