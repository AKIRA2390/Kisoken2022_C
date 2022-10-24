#include <M5Unified.h>
// #include <Arduino.h>
#include <ArduinoJson.h>>
#include <ESP32Servo.h>
#include "WiFi.h"

const char* WIFI_SSID = "ipad_akira";
const char* WIFI_PASS = "kisokenCWiFi";
const uint16_t port = 8893;
const char* host = "192.168.1.100";
WiFiClient client;
StaticJsonDocument<1024> json;

Servo GripServo;
Servo CounterServo;
// void MOVJ(StaticJsonDocument<1024>* json, int x, int y, int z,
//           bool isrightOrientation);

void setup() {
  // put your setup code here, to run once:
  M5.begin();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.config(IPAddress(192, 168, 1, 101), IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 0, 0));

  while (WiFi.status() != WL_CONNECTED) {
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("Wifi connection failed: ");
      Serial.println();
      Serial.print("SSID: ");
      Serial.println(WIFI_SSID);
      Serial.print("Password: ");
      Serial.println(WIFI_PASS);
      Serial.println();
    }
    delay(5000);
  }
  Serial.begin(115200);
  pinMode(26,OUTPUT);
  GripServo.attach(26);
  CounterServo.attach(36);
  GripServo.write(180);
}

void loop() {
  // put your main
  M5.update();

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    delay(5000);
    return;
  }

  json["command"] = "JumpTo";
  json["x"] = 0;
  json["y"] = 0;
  json["z"] = 70;
  json["r"] = 0;
  char data[500];
  serializeJson(json, data);
  if (M5.BtnA.wasPressed()) {
    client.write(data);
  }
}