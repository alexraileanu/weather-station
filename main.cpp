#include "setup.h"
#include "sensor.h"

ADC_MODE(ADC_VCC);
#define SEALEVELPRESSURE_HPA (1013.25)
#define SLEEP_TIME 1800000000

void preinit() {
  ESP8266WiFiClass::preinitWiFiOff();
}

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);

  setupSensor();
}

void loop() {
  setupWifi();
  
  float batteryLevel = ESP.getVcc();
  Adafruit_BME680 bme = getBME();
  bme.performReading();

  String data[5] = {
    String(bme.temperature),
    String(bme.pressure / 100.0),
    String(bme.humidity),
    String(bme.gas_resistance / 1000.0),
    String(batteryLevel),
  };

  makeRequest("/collect", "POST", data);

  ESP.deepSleep(SLEEP_TIME);
}
