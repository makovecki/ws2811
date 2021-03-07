#include "storage.h"
#include <string>
#include "nvs_flash.h"
#include "nvs.h"
using namespace std;

void Storage::SaveString(string key, string value)
{
    esp_err_t err = nvs_set_str(handle, key.c_str(), value.c_str());
    err = nvs_commit(handle);
    printf((err != ESP_OK) ? "SaveString Failed!\n" : "");
}
string Storage::GetString(string key, string def)
{
    string v = GetString(key);
    if (v.empty()) return def;
    return v;
}
string Storage::GetString(string key)
{
    size_t required_size;
    esp_err_t err= nvs_get_str(handle, key.c_str(), NULL, &required_size);
 
    string s;

    if (err != ESP_OK) return s;

    char* value = (char*)malloc(required_size);
    nvs_get_str(handle, key.c_str(), value, &required_size);

    string svalue(value);
    free(value);

    return svalue;

}

Storage::Storage()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_ERROR_CHECK( err );

    nvs_open("STORAGE", NVS_READWRITE, &handle);
}

Storage::~Storage()
{
    nvs_close(handle);
}