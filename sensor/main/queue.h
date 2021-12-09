#pragma once

#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "weather.h"

void queue_start(QueueHandle_t queue);