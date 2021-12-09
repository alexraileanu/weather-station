#include "queue.h"

static const char *TAG = "weather_queue";

void queue_start(QueueHandle_t queue) {
    ESP_LOGI(TAG, "Connecting to MQTT %s with PWD %s", CONFIG_MQTT_BROKER_URL, CONFIG_MQTT_BROKER_PASSWD);

    esp_mqtt_client_config_t cfg = {
            .host = CONFIG_MQTT_BROKER_HOST,
            .port = CONFIG_MQTT_BROKER_PORT,
            .username = CONFIG_MQTT_BROKER_USERNAME,
            .password = CONFIG_MQTT_BROKER_PASSWD,
            .client_id = "WEATHER_STATION",
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_start(client);
}