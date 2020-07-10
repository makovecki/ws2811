#include "tcpserver.h"
#include "iprocessor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <iostream>

void TcpServer::ReceiveTask(void *pvParameters)
{
    char rx_buffer[128];
    int len;
    TcpServer *tcp = static_cast<TcpServer *>(pvParameters);
    struct sockaddr_in source_addr;
    uint addr_len = sizeof(source_addr);

     while (1) {
         
        int sock = accept(tcp->listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) break;
        len = recvfrom(tcp->listen_sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &addr_len);//recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        std::cout << "Received:"<< len << " from port:"<< ntohs(source_addr.sin_port)<<"\n";
        //tcp->processor->ProcessMessage(rx_buffer,source_addr.sin_addr);
     }
    vTaskDelete(NULL);
}
void TcpServer::Bind()
{
    struct sockaddr_in source_addr;
    source_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    source_addr.sin_family = AF_INET;
    source_addr.sin_port = htons(port);
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    int err = bind(listen_sock, (struct sockaddr *)&source_addr, sizeof(source_addr));
    if (err != 0) {
        std::cout << "Unable to bind!\n";
    } else
    {
        err = listen(listen_sock, 1);
        if (err != 0) std::cout << "Unable to listen!\n";
        else
        xTaskCreate(this->ReceiveTask, "tcp_server", 2048, this, 5, NULL);
    }
}
TcpServer::TcpServer(uint16_t port)
{

    this->port = port;
}

TcpServer::~TcpServer()
{
    
}
