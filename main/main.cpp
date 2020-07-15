#include "wifi.h"
#include "esp_ota_ops.h"

#include "ws2811.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include <iostream>

 extern "C" void app_main()
{
    
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (running->subtype == 0) printf("Running from FACTORY partition\n");
    if (running->subtype == 16) printf("Running from OTA1 partition\n");
    if (running->subtype == 17) printf("Running from OTA2 partition\n");
    
    auto wifi =new WiFi();
    wifi->AddUdpServer(1977);
    
    WS2811 ledStrip(GPIO_NUM_16,3, RMT_CHANNEL_0 );
    for (int i = 0; i < 3; i++) ledStrip.SetPixel(i,255,255,255);
    ledStrip.Show();

}