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
               "Content-Type: application/json" + "\r\n" +
               "Content-Length: 100" + "\r\n\r\n" +
               "\"temperature\": \"20.300\",\"pressure\": \"1011.43\",\"humidity\": \"51.97\",\"gas\": \"34.27\",\"battery\": \"3400\"");
}
