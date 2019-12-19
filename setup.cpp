#include "setup.h"

#include <ESP8266WiFi.h>

void setupWifi() {
  const String ssid     = "";
  const String password = "";

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void makeRequest(String url, String method, String data[5]) {
  WiFiClient client;
  const String host = "";
  const int httpPort = 5000;

  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String dataString = "{\"temperature\": \"" + data[0] + "\",\"pressure\": \"" + data[1] + "\",\"humidity\": \"" + data[2] + "\",\"gas\": \"" + data[3] + "\",\"battery\": \"" + data[4] + "\"}";

  client.print(String(method + " ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json" + "\r\n" +
               "Content-Length: " + dataString.length() + "\r\n\r\n" +
               dataString + "\r\n");
}
