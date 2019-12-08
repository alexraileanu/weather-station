#include "setup.h"
#include "sensor.h"

ADC_MODE(ADC_VCC);

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  
  setupWifi();
  setupSensor();
}

void loop() {
  digitalWrite(0, HIGH);
  delay(5000);

  float batteryLevel = ESP.getVcc();
  char url[64];
  Adafruit_BME680 bme = getBME();
  bme.performReading();

  sprintf(url, "/post?test=%f", bme.temperature);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  makeRequest(url, "POST");
  
  Serial.println();
  Serial.println("closing connection");
  digitalWrite(0, LOW);
}