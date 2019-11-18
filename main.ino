#include <ESP8266WiFi.h>

// needed to make requests to https hosts
#include <WiFiClientSecure.h>

const char* ssid     = "";
const char* password = "";
const char* host     = "";

ADC_MODE(ADC_VCC);

void makeWifiConnection () { 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  
  makeWifiConnection();
}

void loop() {
  digitalWrite(0, HIGH);
  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 443;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  float batteryLevel = ESP.getVcc();
  char url[64];

  // We now create a URI for the request
  sprintf(url, "/batterytest?level=%f", batteryLevel / 1024);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  
  Serial.println();
  Serial.println("closing connection");
  digitalWrite(0, LOW);
}