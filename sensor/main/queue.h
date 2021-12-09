#pragma once

#include "esp_sleep.h"
#include "esp_log.h"
#include "mqtt_client.h"

#include "weather.h"
#include "wifi.h"

void queue_start(QueueHandle_t queue);