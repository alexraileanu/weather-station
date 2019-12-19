#include "setup.h"
#include "sensor.h"

ADC_MODE(ADC_VCC);


#define SEALEVELPRESSURE_HPA (1013.25)
void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);

  setupWifi();
  setupSensor();
}

void loop() {
  digitalWrite(0, HIGH);

  // wait half an hour before reading again
  delay(1000 * 60 * 30);

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
  digitalWrite(0, LOW);
}
