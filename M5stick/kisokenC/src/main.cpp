#include <M5StickCPlus.h>
// #include <Arduino.h>
#include "WiFi.h"

const char* WIFI_SSID = "ipad_akira";
const char* WIFI_PASS = "kisokenCWiFi";
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  M5.begin();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.config(IPAddress(192,168,1,101),IPAddress(192, 168, 1, 1),IPAddress(255, 255, 0, 0));

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

  M5.Lcd.setCursor(7, 20, 2);
  m5.Lcd.fillScreen(BLUE);
  M5.Lcd.print(WiFi.localIP());
  Serial.begin(115200);
}

void loop() {
  // put your main
  M5.update();
  if(M5.BtnA.wasPressed()){

    client.write()
  }
}