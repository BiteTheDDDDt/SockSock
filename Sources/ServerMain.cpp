#include <iostream>
#include "Server.h"

int main()
{
    std::cout << "task starting..." << std::endl;
    Server serv = Server();
    serv.start();
    std::cout << "task endding..." << std::endl;
    return 0;
}