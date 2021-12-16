#include "queue.h"

static const char *TAG = "weather_queue";
static const char *MQTT_TOPIC = "weather";
static const char *MQTT_CLIENT_ID = "weather_station";
static const int64_t WAKEUP_TIME_SEC = 120;

static void send_weather_data(esp_mqtt_client_handle_t *client, QueueHandle_t queue) {
    post_log("Attempting to send data");
    if (queue != 0) {
        struct Weather weather;

        if (xQueueReceive(queue, &(weather), pdMS_TO_TICKS(1000))) {
            post_log("Received data from the queue. Attempting to send it to mqtt");
            char json_msg[90];
            create_weather_message(json_msg, &weather);
            esp_mqtt_client_publish(*client, MQTT_TOPIC, json_msg, 0, 1, 0);

            post_log("Sent data to mqtt topic.");
        } else {
            post_log("Queue receive failed");
        }

        post_log("Will enter deep sleep.");
        ESP_ERROR_CHECK(esp_wifi_stop());
        esp_sleep_enable_timer_wakeup(WAKEUP_TIME_SEC * 1000000);
        esp_deep_sleep_start();
    } else {
        post_log("Wrong queue?");
    }
}

static esp_err_t mqtt_callback(esp_mqtt_event_handle_t evt, QueueHandle_t queue) {
    esp_mqtt_client_handle_t client = evt->client;
    switch (evt->event_id) {
        case MQTT_EVENT_CONNECTED:
            post_log("Connected to mqtt. Sending data");
            send_weather_data(&client, queue);
            break;
        default:
            ESP_LOGI(TAG, "Got MQTT ID: %d", evt->event_id);
            break;
    }

    return ESP_OK;
}

static void evt_handler(void *arg, esp_event_base_t base, int32_t evt_id, void *evt_data) {
    QueueHandle_t queue = (QueueHandle_t) arg;
    mqtt_callback(evt_data, queue);
}

void queue_start(QueueHandle_t queue) {
    post_log("Starting mqtt connection");
    esp_mqtt_client_config_t cfg = {
            .host = CONFIG_MQTT_BROKER_HOST,
            .port = CONFIG_MQTT_BROKER_PORT,
            .username = CONFIG_MQTT_BROKER_USER,
            .password = CONFIG_MQTT_BROKER_PASSWD,
            .client_id = MQTT_CLIENT_ID,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&cfg);

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, evt_handler, (void *) queue);

    esp_mqtt_client_start(client);
    post_log("Mqtt client started");
}
