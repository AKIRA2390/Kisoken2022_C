#include <M5Unified.h>
// #include <Arduino.h>
#include <ArduinoJson.h>
#include <Servo.h>

#include "WiFi.h"

const char* WIFI_SSID = "ipad_akira";
const char* WIFI_PASS = "kisokenCWiFi";
const uint16_t port = 8893;
const char* host = "192.168.1.100";
WiFiClient client;

Servo GripServo;
Servo CounterServo;
void MOVJ(int x, int y, int z, int r);

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
  pinMode(26, OUTPUT);
  pinMode(36, OUTPUT);
  GripServo.attach(26);
  CounterServo.attach(36);
}

void loop() {
  MOVJ(280, -100, 170, 0);
  // StaticJsonDocument<1024> json2;
  // json2["command"] = "JumpTo";
  // json2["x"] = 380;
  // json2["y"] = -100;
  // json2["z"] = 170;
  // json2["r"] = 0;
  // char data2[500];
  // serializeJson(json2, data2);
  // client.write(data2);
  // GripServo.write(180);
  // CounterServo.write(180);
  delay(5000);

  MOVJ(280, 100, 170, 0);
  // GripServo.write(0);
  // CounterServo.write(0);
  delay(5000);
  // put your main
  M5.update();

  // if (M5.BtnA.wasPressed()) {
  // }
}

void MOVJ(int x, int y, int z, int r) {
    if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println(host);
    Serial.println(port);
    // Serial.println("Waiting 5 seconds before retrying...");
    // delay(5000);
    return;
  }
  StaticJsonDocument<1024> json;
  json["command"] = "JumpTo";
  json["x"] = x;
  json["y"] = y;
  json["z"] = z;
  json["r"] = r;
  char data[500];
  serializeJson(json, data);
  client.write(data);
  json.clear();
}
