#include "wifi.h"

#include "esp_event.h"
#include "esp_log.h"

static const char *TAG = "sensor";

struct Weather {
    float temperature;
    float pressure;
    float humidity;
    float battery;
};

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    EventGroupHandle_t connect_evt_group = xEventGroupCreate();
//    QueueHandle_t queue = xQueueCreate(2, sizeof(struct Weather));

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init(&connect_evt_group);
}
