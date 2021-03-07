#pragma once
#include "lwip/sockets.h"
#include <list>
using namespace std;
class IProcessor
{
private:
    /* data */
public:
    IProcessor(/* args */){}
    virtual list<char> ProcessMessage(char *buffer,struct in_addr ip);
    virtual ~IProcessor() =0;
};

