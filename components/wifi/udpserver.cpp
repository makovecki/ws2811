#include "udpserver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <iostream>
#include "ota.h"
#include "echoprocessor.h"


void UdpServer::ReceiveTask(void *pvParameters)
{
    char rx_buffer[128];
    UdpServer *udp = static_cast<UdpServer *>(pvParameters);

    struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
    socklen_t socklen = sizeof(source_addr);
    std::cout << "Listening UDP port:"<< udp->port<<"!\n";
    while (1) {
        if (udp->listen_sock<0) 
        {
            std::cout << "Unable to create socket!\n";
            break;
        }
        
        int len=recvfrom(udp->listen_sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

        if (len>=3 && len == ((rx_buffer[1]<<8 ) + rx_buffer[2] +3 ))
        {
            //cout << "Receive "<<rx_buffer[0]<<"\n";
            auto processor = udp->ResolveProcessor(rx_buffer[0]);
            auto reply =processor->ProcessMessage(rx_buffer,source_addr.sin_addr);
            delete processor;
            uint msgSize=reply.size();
            reply.push_front(msgSize & 0xFF);
            reply.push_front((msgSize>>8) & 0xFF);
            reply.push_front(rx_buffer[0]);
            char tx_buffer[reply.size()];

            std::copy(reply.begin(), reply.end(),tx_buffer);
            int err = sendto(udp->listen_sock, tx_buffer,sizeof(tx_buffer)/sizeof(*tx_buffer) , 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
            if (err < 0) {
                if (rx_buffer[0] == 1)
                {
                    // OTA update retry send message when error
                    udp->listen_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
                    struct sockaddr_in dest_addr;
                    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
                    dest_addr.sin_family = AF_INET;
                    dest_addr.sin_port = htons(99);
                    int e = bind(udp->listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                    if (e < 0) {
                        std::cout << "Unable to bind!\n";
                    }
                    sendto(udp->listen_sock, tx_buffer,sizeof(tx_buffer)/sizeof(*tx_buffer) , 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    esp_restart();
                }
                std::cout << "Error sending to remote client!\n";
            }
        } else 
        {
            std::cout << "Wrong message size ("<<len<<"!="<<((rx_buffer[1]<<8 ) + rx_buffer[2] +3 )<<")!\n";
            if (len==-1)
            {
                shutdown(udp->listen_sock, 0);
                close(udp->listen_sock);
               
                vTaskDelete( NULL );
                udp->Bind();
                
                break;
            }
            
        }
    }

}
IProcessor* UdpServer::ResolveProcessor(uint8_t msg)
{
    switch(msg) 
    {
        case 1: return new Ota();
        default: return new EchoProcessor();
    }
}
void UdpServer::Bind()
{
    listen_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    int e = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (e < 0) {
        std::cout << "Unable to bind!\n";
    }
    
    xTaskCreate(this->ReceiveTask, "udp_server", 1024 * 10, this, 10, NULL);
}

UdpServer::UdpServer(uint16_t port)
{
    this->port = port;

}

UdpServer::~UdpServer()
{

}