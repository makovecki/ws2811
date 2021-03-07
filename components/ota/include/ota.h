
#pragma once
#include "iprocessor.h"
#include <list>
#include <string>
class Ota : public IProcessor {
private:
    /* data */
    char* substr(char* arr, int begin, int len);
public:
    Ota(/* args */);
    list<char> ProcessMessage(char *buffer,struct in_addr ip);
    bool DoOTAUpdate(char* url);
    ~Ota();
};



