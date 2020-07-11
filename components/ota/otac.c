#include "otac.h"
#include <stdio.h>
#include <esp_https_ota.h>
#include <esp_ota_ops.h>
#include <stdbool.h>



bool ota_update(const char* url)
{
    esp_http_client_config_t config = {

        .url = url,
    };
    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {

        esp_restart();

    } else {

        printf("OTA failed !!!\n");
    }

    return false;
}