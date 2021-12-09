#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <bme680.h>
#include "esp_log.h"
#include "string.h"
#include "weather.h"

void sensor_collect_data(void *pvParameters);