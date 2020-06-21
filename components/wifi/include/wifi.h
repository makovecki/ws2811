#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"

class WiFi
{
private:
    static int s_retry_num;
    static EventGroupHandle_t wifi_event_group;
    static const int CONNECTED_BIT = BIT0;
    static void EventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
public:
    WiFi();
    ~WiFi();
};