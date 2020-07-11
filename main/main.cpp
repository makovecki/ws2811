#include "wifi.h"
#include "esp_ota_ops.h"


 extern "C" void app_main()
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (running->subtype == 0) printf("Running from FACTORY partition\n");
    if (running->subtype == 16) printf("Running from OTA1 partition\n");
    if (running->subtype == 17) printf("Running from OTA2 partition\n");
    
    auto wifi =new WiFi();
    wifi->AddUdpServer(1977);
 
    


}