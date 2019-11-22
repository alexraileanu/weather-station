#include "setup.h"

#include <ESP8266WiFi.h>
// needed to make requests to https hosts
#include <WiFiClientSecure.h>

void setupWifi() {
  const String ssid     = "";
  const String password = "";

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void makeRequest(String url, String method) {
  WiFiClient client;
  const String host = "";
  const int httpPort = 443;
  
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String(method + " ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
}