#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#include "sensor.h"

#define BME_SCK 14
#define BME_MISO 12
#define BME_MOSI 13
#define BME_CS 15

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

void setupSensor() {
  bme.begin();

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);
}

Adafruit_BME680 getBME() {
  return bme;
}

// void loop() {
//   if (! bme.performReading()) {
//     Serial.println("Failed to perform reading :(");
//     return;
//   }
//   Serial.print("Temperature = ");
//   Serial.print(bme.temperature);
//   Serial.println(" *C");

//   Serial.print("Pressure = ");
//   Serial.print(bme.pressure / 100.0);
//   Serial.println(" hPa");

//   Serial.print("Humidity = ");
//   Serial.print(bme.humidity);
//   Serial.println(" %");

//   Serial.print("Gas = ");
//   Serial.print(bme.gas_resistance / 1000.0);
//   Serial.println(" KOhms");

//   Serial.print("Approx. Altitude = ");
//   Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//   Serial.println(" m");

//   Serial.println();
//   delay(2000);
// }