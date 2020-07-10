#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include <list>
#include "tcpserver.h"
#include "udpserver.h"

class WiFi
{
private:
    static int s_retry_num;
    static EventGroupHandle_t wifi_event_group;
    static const int CONNECTED_BIT = BIT0;
    static void EventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
    static std::list<TcpServer> tcp_servers;
    static std::list<UdpServer> udp_servers;
    static void WaitToConnectTask(void *pvParameters);
public:
    WiFi();
    void AddTcpServer(uint16_t port);
    void AddUdpServer(uint16_t port);
    void WaitForConnection();
    ~WiFi();
};