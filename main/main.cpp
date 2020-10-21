#include "wifi.h"
#include "esp_ota_ops.h"
#include "stairs.h"
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

    Stairs *stairs = new Stairs(GPIO_NUM_16,GPIO_NUM_17,GPIO_NUM_5,21,1);
    stairs->ClearLights();
   /* 
    WS2811 ledStrip(GPIO_NUM_16,501, RMT_CHANNEL_0 );

    uint8_t brightness = 0;
    bool up = true;
    while (true)
    {
        for (int i = 0; i <= 500; i++) ledStrip.SetPixel(i,brightness,255,255,255);
        ledStrip.Show();

        vTaskDelay(10 / portTICK_PERIOD_MS);
        brightness += (up)?+1:-1;
        if (brightness==255) up = false;
        if (brightness==0) up = true;
    }
    */


}