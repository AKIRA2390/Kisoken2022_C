#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLed.h>
#include <M5Unified.h>
#include <Servo.h>

#include "WiFi.h"

const char* WIFI_SSID = "ipad_akira";
const char* WIFI_PASS = "kisokenCWiFi";
const uint16_t port = 8893;
const char* host = "192.168.1.100";
WiFiClient client;
CRGB led[1];
bool ExecFlagMain = false, ExecFlagTest = false;

typedef struct coordinate_t {
  int x, y, r;
} coordinates;

int TrayPos[3][2][3] = {
    // TrayNum, PrePost, xyr
    {{158, 317, 135}, {158, -317, -135}},
    {{264, 145, 135}, {264, -145, -135}},
    {{399, -24, 135}, {399, 24, -135}},
};

int ServerPos[3] = {285, 276, -45};

enum class Zs {
  z_Move = 140,
  z_Catch = 115,
  Z_Carry = 160,
  z_Release = 115,
  z_ServerInit = 180,
  z_ServerExec = 155,
};

Servo GripServo;
Servo CounterServo;
const coordinate_t pos_PreA = {158, 317, 135};
const coordinate_t pos_PreAu = {158, 317, 135};
const coordinate_t pos_PostA = {158, -317, -135};
const coordinate_t pos_PostAu = {168, -307, -135};
const coordinate_t pos_PreB = {264, 145, 135};
const coordinate_t pos_PreBu = {264, 145, 135};
const coordinate_t pos_PostB = {264, -145, -135};
const coordinate_t pos_PostBu = {264, -145, -135};
const coordinate_t pos_PreC = {399, -24, 135};
const coordinate_t pos_PreCu = {399, -24, 135};
const coordinate_t pos_PostC = {399, 24, -135};
const coordinate_t pos_PostCu = {399, 24, -135};

void sendJson(StaticJsonDocument<1024>);
void MOVJ(int x, int y, int z, int r);
void MOVJ(const coordinate_t xyr, int z);
void ArmOrientation(int isItRight);
void WaitDobot(int millisec);
void MainMovement();
void TestMovement();
void delaye(int duration);

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  FastLED.addLeds<SK6812, 27, RGB>(led, 1);

  led[0] = HUE_ORANGE;
  FastLED.show();

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
  led[0] = HUE_GREEN;
  FastLED.show();
  Serial.begin(115200);
  pinMode(26, OUTPUT);
  pinMode(18, OUTPUT);
  GripServo.attach(26);
  CounterServo.attach(18);
}

void loop() {
  M5.update();
  // GripServo.write(70);
  // GripServo.write(150);

  TestMovement();

  // if (M5.BtnA.wasPressed()) {
  //   ExecFlagMain = true;
  //   ExecFlagTest = true;
  // }
  // if (ExecFlagMain) {
  //   MainMovement();
  // }
  // if (ExecFlagTest) {
  //   TestMovement();
  // }
}

void sendJson(StaticJsonDocument<1024> json) {
  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println(host);
    Serial.println(port);
    return;
  }
  char data[500];
  serializeJson(json, data);
  client.write(data);
  json.clear();
}

void MOVJ(int x, int y, int z, int r) {
  StaticJsonDocument<1024> json;
  json["command"] = "JumpTo";
  json["x"] = x;
  json["y"] = y;
  json["z"] = z;
  json["r"] = r;
  sendJson(json);
  json.clear();
}
void MOVJ(coordinate_t xyr, int z) {
  StaticJsonDocument<1024> json;
  json["command"] = "JumpTo";
  json["x"] = xyr.x;
  json["y"] = xyr.y;
  json["z"] = z;
  json["r"] = xyr.r;
  sendJson(json);
  json.clear();
}
void ArmOrientation(int isItRight){
  StaticJsonDocument<1024> json;
  json["command"] = "ArmOrientation";
  json["mode"] = isItRight;
  sendJson(json);
  json.clear();}
void WaitDobot(int millisec){
  StaticJsonDocument<1024> json;
  json["command"] = "Wait";
  json["ms"] = millisec;
  sendJson(json);
  json.clear();}

void MainMovement() {
  static uint64_t StartTime = millis();
  uint64_t ExecTime = StartTime - millis();
}
void TestMovement() {
  WaitDobot(1000);
  GripServo.write(70);
  ArmOrientation(1);
  MOVJ(pos_PreA, int(Zs::z_Move));
  ArmOrientation(1);
  MOVJ(pos_PreA, int(Zs::z_Catch));
  WaitDobot(1000);
  GripServo.write(150);
  ArmOrientation(1);
  MOVJ(pos_PreA, int(Zs::Z_Carry));

  ArmOrientation(0);
  MOVJ(pos_PostA, int(Zs::Z_Carry));
  ArmOrientation(0);
  MOVJ(pos_PostA, int(Zs::z_Catch));
  WaitDobot(1000);
  GripServo.write(70);
  ArmOrientation(0);
  MOVJ(pos_PostAu, int(Zs::Z_Carry));
}

/*
座�

// void delaye(int duration){
//   static uint64_t _startTime = millis();
//   uint64_t _execTime = _startTime-millis();

// }
*/












