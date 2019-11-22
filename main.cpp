#include "setup.h"

ADC_MODE(ADC_VCC);

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  
  setupWifi();
}

void loop() {
  digitalWrite(0, HIGH);
  delay(5000);

  float batteryLevel = ESP.getVcc();
  char url[64];

  sprintf(url, "/batterytest?level=%f", batteryLevel / 1024);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  makeRequest(url, "GET");
  
  Serial.println();
  Serial.println("closing connection");
  digitalWrite(0, LOW);
}