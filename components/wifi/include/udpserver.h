#pragma once
#include "iprocessor.h"
#include <inttypes.h>
class UdpServer
{
private:
    static void ReceiveTask(void *pvParameters);
    int listen_sock;
    uint16_t port;
public:
    UdpServer(uint16_t port);
    void Bind();
    IProcessor* ResolveProcessor(uint8_t msg);
    ~UdpServer();
};