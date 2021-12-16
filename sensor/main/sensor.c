#include "sensor.h"

#define SDA_GPIO 23
#define SCL_GPIO 22
#define PORT 0
#define ADDR BME680_I2C_ADDR_1

static const char *TAG = "weather_sensor";
const TickType_t xQueueBlockTime = pdMS_TO_TICKS(200);

void sensor_collect_data(void *pvParameters) {
    post_log("Preparing sensor reading");
    ESP_ERROR_CHECK(i2cdev_init());
    QueueHandle_t queue = (QueueHandle_t) pvParameters;

    bme680_t sensor;
    memset(&sensor, 0, sizeof(bme680_t));

    ESP_ERROR_CHECK(bme680_init_desc(&sensor, ADDR, PORT, SDA_GPIO, SCL_GPIO));

    // init the sensor
    ESP_ERROR_CHECK(bme680_init_sensor(&sensor));

    // Changes the oversampling rates to 4x oversampling for temperature
    // and 2x oversampling for humidity. Pressure measurement is skipped.
    bme680_set_oversampling_rates(&sensor, BME680_OSR_4X, BME680_OSR_2X, BME680_OSR_2X);

    // Change the IIR filter size for temperature and pressure to 7.
    bme680_set_filter_size(&sensor, BME680_IIR_SIZE_7);

    // Change the heater profile 0 to 200 degree Celsius for 100 ms.
    bme680_set_heater_profile(&sensor, 0, 200, 100);
    bme680_use_heater_profile(&sensor, 0);

    // Set ambient temperature to 10 degree Celsius
    bme680_set_ambient_temperature(&sensor, 20);

    float totalValues[3];

    post_log("Preparing to read 5 values");
    for (int i = 0; i < 5; i++) {
        bme680_values_float_t values;
        uint32_t duration;
        bme680_get_measurement_duration(&sensor, &duration);

        if (bme680_force_measurement(&sensor) != ESP_OK) {
            return;
        }
        vTaskDelay(duration);

        if (bme680_get_results_float(&sensor, &values) == ESP_OK) {
            totalValues[0] += values.temperature;
            totalValues[1] += values.humidity;
            totalValues[2] += values.pressure;
        }
    }
    post_log("Read 5 values");

    struct Weather post_msg;
    post_msg.temperature = totalValues[0] / 5;
    post_msg.humidity = totalValues[1] / 5;
    post_msg.pressure = totalValues[2] / 5;

    post_log("Sending data to queue");
    xQueueSendToFront(queue, (void *) &post_msg, xQueueBlockTime);

    vTaskDelete(NULL);
}