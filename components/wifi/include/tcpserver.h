#pragma once
#include "iprocessor.h"
#include <inttypes.h>
class TcpServer
{
private:
    static void ReceiveTask(void *pvParameters);
    int listen_sock;
    uint16_t port;
public:
    TcpServer(uint16_t port);
    void Bind();
    ~TcpServer();
};




