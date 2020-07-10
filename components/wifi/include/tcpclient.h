#pragma once
#include "iprocessor.h"
#include "wifi.h"
#include <list>
class TcpClient: public IProcessor 
{
private:
    int sock;
public:
    TcpClient(WiFi *wifi);
    void Send();
    list<char> ProcessMessage(char *buffer,struct in_addr ip);
    ~TcpClient();
};



