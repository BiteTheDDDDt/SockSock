#include <iostream>
#include "CommonDef.h"
#include "Client.h"

int main()
{
    //std::cout << "task starting..." << std::endl;
    Client clie = Client();
    if (clie.initConnection())
    {
        clie.start();
    }
    else
    {
        std::cout << "Connect fault." << std::endl;
    }
    //std::cout << "task endding..." << std::endl;
    return 0;
}