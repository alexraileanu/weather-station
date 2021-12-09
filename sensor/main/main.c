#include "nvs_flash.h"
#include "esp_log.h"

#include "weather.h"
#include "sensor.h"
#include "wifi.h"
#include "queue.h"

static const char *TAG = "weather";

void create_weather_message(char *msg, struct Weather *weather) {
    sprintf(msg, "{\"temperature\": %.2f, \"pressure\": %.2f, \"humidity\": %.2f}", weather->temperature, weather->pressure, weather->humidity);
}

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(ret);

    EventGroupHandle_t connect_evt_group = xEventGroupCreate();
    QueueHandle_t queue = xQueueCreate(2, sizeof(struct Weather));
    if (queue == 0) {
        ESP_LOGE(TAG, "Error creating queue");
        return;
    }

    xTaskCreatePinnedToCore(sensor_collect_data, "sensor_collect_data", configMINIMAL_STACK_SIZE * 8, (void *)queue, 5, NULL, APP_CPU_NUM);

    wifi_init(&connect_evt_group);

    queue_start(queue);
}
