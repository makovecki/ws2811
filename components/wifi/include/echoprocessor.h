#pragma once
#include "iprocessor.h"
#include <list>
class EchoProcessor : public IProcessor {
private:
    /* data */
public:
    EchoProcessor(/* args */);
    list<char> ProcessMessage(char *buffer,struct in_addr ip);
    ~EchoProcessor();
};