#pragma once

struct Weather {
    float temperature;
    float pressure;
    float humidity;
    float battery;
};

void create_weather_message(char *msg, struct Weather *weather);