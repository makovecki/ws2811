#include "wifi.h"
#include "udpserver.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include <string>
#include <iostream>
#include <list>
extern "C" {
   #include "string.h" 
}

WiFi::~WiFi()
{

}
int WiFi::s_retry_num(0);
std::list<UdpServer> WiFi::udp_servers;
EventGroupHandle_t WiFi::wifi_event_group = xEventGroupCreate();
void WiFi::EventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            s_retry_num++;
            
        }
        std::cout <<"connect to the AP fail\n";
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        s_retry_num = 0;
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    }
}
void WiFi::WaitForConnection()
{
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
}
void WiFi::WaitToConnectTask(void *pvParameters)
{
    EventBits_t connectedToAP = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    if (connectedToAP) 
    {
        std::cout << "Connected to WiFi!\n";
        for (auto udp : udp_servers) udp.Bind();
    };
    vTaskDelete(NULL);
}

void WiFi::AddUdpServer(uint16_t port)
{
    udp_servers.push_back(UdpServer(port));
}
WiFi::WiFi(/* args */)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &EventHandler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &EventHandler, NULL));

    wifi_config_t wifi_config {};
    std::string ssid(CONFIG_ESP_WIFI_SSID);
    memcpy(wifi_config.sta.ssid, ssid.data(), ssid.size());
    std::string pwd(CONFIG_ESP_WIFI_PASSWORD);
	memcpy(wifi_config.sta.password, pwd.data(), pwd.size());

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    xTaskCreate(this->WaitToConnectTask, "waitConnect", 1024, this, 10, NULL);
}