#include "otac.h"
#include <stdio.h>
#include <esp_https_ota.h>
#include <esp_ota_ops.h>

extern const uint8_t server_cert_pem_start[] asm("_binary_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_cert_pem_end");

void ota_update(const char* url)
{
    esp_http_client_config_t config = {

        .url = url,
        .cert_pem = (char *)server_cert_pem_start,
    };
    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {

        esp_restart();

    } else {

        printf("OTA failed !!!\n");
        //esp_restart();
    }
}