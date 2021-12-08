#include "wifi.h"

#define SSID CONFIG_ESP_WIFI_SSID
#define PASSWORD CONFIG_ESP_WIFI_PASSWORD
#define MAX_RETRY CONFIG_ESP_MAXIMUM_RETRY

#define WIFI_CONNECT_BIT BIT(0)
#define WIFI_FAIL_BIT BIT(1)

static const char *TAG = "sensor";
static int retry_num = 0;
static EventGroupHandle_t wifi_evt_group;

static void evt_handler(void *arg, esp_event_base_t evt_base, int32_t evt_id, void *evt_data) {
    EventGroupHandle_t* connect_event_group = (EventGroupHandle_t*) arg;

    if (evt_base == WIFI_EVENT && evt_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (evt_base == WIFI_EVENT && evt_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (retry_num < MAX_RETRY) {
            esp_wifi_connect();
            retry_num++;
            ESP_LOGI(TAG, "Retrying WIFI connection");
        } else {
            xEventGroupSetBits(wifi_evt_group, WIFI_FAIL_BIT);
        }
    } else if (evt_base == IP_EVENT && evt_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *evt = (ip_event_got_ip_t *) evt_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&evt->ip_info.ip));
        retry_num = 0;
        xEventGroupSetBits(*connect_event_group, WIFI_CONNECT_BIT);
    }
}

void wifi_init(EventGroupHandle_t* connect_event_group) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(
            esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &evt_handler, connect_event_group)
    );
    ESP_ERROR_CHECK(
            esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &evt_handler, connect_event_group)
    );

    wifi_config_t wifi_config = {
            .sta = {
                    .ssid = SSID,
                    .password = PASSWORD,
                    .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                    .pmf_cfg = {
                            .capable = true,
                            .required = false
                    },
            },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "init_wifi finished");

    EventBits_t bits = xEventGroupWaitBits(*connect_event_group, WIFI_CONNECT_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits != WIFI_CONNECT_BIT) {
        // retry connection
        esp_wifi_connect();
    }
}