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
    {{155, 315, 135}, {155, -315, -135}},
    {{275, 145, 135}, {275, -145, -135}},
    {{395, -25, 135}, {395, 25, -135}},
};

int ServerPos[3] = {285, 275, -45};

enum class Zs {
  Move = 140,
  Pour = 105,
  Catch = 110,
  Carry = 160,
  Release = 115,
  ServerInit = 185,
  ServerExec = 155,
};
enum class GripAngle{
  Release = 70,
  Catch = 170,
} ;

Servo GripServo;
Servo CounterServo;
const coordinate_t pos_Server = {285, 275, -45};
const coordinate_t pos_Serveru = {235, 225, -45};
const coordinate_t pos_PreA = {155, 315, 135};
const coordinate_t pos_PourA = {230, 285, 45};  //+75,-30
const coordinate_t pos_PostA = {155, -315, -135};
const coordinate_t pos_PostAu = {165, -305, -135};
const coordinate_t pos_PreB = {275, 145, 135};
const coordinate_t pos_PourB = {365, 155, 45};
const coordinate_t pos_PostB = {275, -145, -135};
const coordinate_t pos_PostBu = {285, -135, -135};
const coordinate_t pos_PreC = {395, -25, 135};
const coordinate_t pos_PourC = {295, 50, 225};
const coordinate_t pos_PostC = {395, 25, 225};
const coordinate_t pos_PostCu = {385, 35, 225};

void sendJson(StaticJsonDocument<1024>);
void MOVJ(int x, int y, int z, int r);
void MOVJ(const coordinate_t xyr, int z);
void ArmOrientation(int isItRight);
void GetPose(coordinate_t* zahyo);
void WaitDobot(int millisec);
void MainMovement();
void TestMovement();
void delaye(int duration);
void ServerExec();
void PourBeads();
void CarryA();
void CarryB();
void CarryC();

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  FastLED.addLeds<SK6812, 27, GRB>(led, 1);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.config(IPAddress(192, 168, 1, 101), IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 0, 0));

  while (WiFi.status() != WL_CONNECTED) {
    led[0] = CRGB::Red;
    FastLED.show();
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
  pinMode(19, INPUT_PULLUP);
  pinMode(26, OUTPUT);
  pinMode(18, OUTPUT);
  GripServo.attach(26);
  CounterServo.attach(18);
  GripServo.write(int(GripAngle::Catch));
  CounterServo.write(0);
}

void loop() {
  // M5.update();
  // GripServo.write(70); //release:70
  // GripServo.write(170);  // catch:150
  if (digitalRead(19) == LOW) {
    // ExecFlagMain = true;
    MainMovement();
    // TestMovement();
    // ExecFlagTest = true;
    // }
    // if (ExecFlagMain) {
    //   MainMovement();
    // } else if (ExecFlagTest) {
    //   TestMovement();
  } else {
    led[0] = CRGB::Orange;
    FastLED.show();
  }
}

void sendJson(StaticJsonDocument<1024> json) {
  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println(host);
    Serial.println(port);
    led[0] = CRGB::Red;
    FastLED.show();
    return;
  }
  char data[500];
  serializeJson(json, data);
  client.write(data);
  json.clear();
  while (!client.available())
    ;
}

void MOVJ(int x, int y, int z, int r) {
  Serial.println("MOVJ");
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
  Serial.println("MOVJ");
  StaticJsonDocument<1024> json;
  json["command"] = "JumpTo";
  json["x"] = xyr.x;
  json["y"] = xyr.y;
  json["z"] = z;
  json["r"] = xyr.r;
  sendJson(json);
  json.clear();
}

void ArmOrientation(int isItRight) {
  Serial.println("ArmOrientatoin");
  StaticJsonDocument<1024> json;
  json["command"] = "ArmOrientation";
  json["mode"] = int(isItRight);
  sendJson(json);
  json.clear();
}
void WaitDobot(int millisec) {
  Serial.println("WaitDobot");
  StaticJsonDocument<1024> json;
  json["command"] = "Wait";
  json["ms"] = millisec;
  sendJson(json);
  json.clear();
}
// void GetPose(coordinate_t* zahyo) {
//   StaticJsonDocument<1024> json;
//   json["command"] = "Wait";

//   while (!client.available()) {
//     delay(100);
//   }

//   if (client.available()) {
//     int data = client.read();
//     deserializeJson(json, data);
//   }
// }

void MainMovement() {
  static uint64_t StartTime = millis();
  uint64_t ExecTime = StartTime - millis();

  ServerExec();
  PourBeads();
  CarryA();
  CarryC();
  CarryB();
  ExecFlagMain = 0;
  ArmOrientation(1);
  MOVJ(pos_Serveru, int(Zs::ServerInit));
}
void TestMovement() {
  led[0] = CRGB::Green;
  FastLED.show();
  // ServerExec();
  // PourBeads();
  // CarryA();
  // CarryB();
  // CarryC();

  ArmOrientation(1);
  MOVJ(pos_Serveru, int(Zs::ServerInit));
  // delay(2000);
  // WaitDobot(1000);
}

void ServerExec() {
  ArmOrientation(1);
  MOVJ(pos_Serveru, int(Zs::ServerInit));
  ArmOrientation(1);
  MOVJ(pos_Server, int(Zs::ServerInit));
  ArmOrientation(1);
  MOVJ(pos_Server, int(Zs::ServerExec));
  WaitDobot(2000);
  ArmOrientation(1);
  MOVJ(pos_Server, int(Zs::ServerInit));
  ArmOrientation(1);
  MOVJ(pos_Serveru, int(Zs::ServerInit));
}
void PourBeads() {
  int delaytime = 750;
  ArmOrientation(1);
  MOVJ(pos_PourA, int(Zs::ServerExec));
  ArmOrientation(1);
  MOVJ(pos_PourA, int(Zs::Pour));
  WaitDobot(delaytime * 11);
  // delay(900);
  for (int i = 0; i < 10; i++) {
    CounterServo.write((i % 2) ? 180 : 0);
    delay(delaytime);
  }
  ArmOrientation(1);
  MOVJ(pos_PourA, int(Zs::ServerExec));
  ArmOrientation(1);
  MOVJ(pos_PourB, int(Zs::ServerExec));
  ArmOrientation(1);
  MOVJ(pos_PourB, int(Zs::Pour));
  WaitDobot(delaytime * 11);
  // delay(900);
  for (int i = 0; i < 10; i++) {
    CounterServo.write((i % 2) ? 180 : 0);
    delay(delaytime);
  }
  ArmOrientation(1);
  MOVJ(pos_PourB, int(Zs::ServerExec));
  ArmOrientation(1);
  MOVJ(pos_PourC, int(Zs::ServerExec));
  ArmOrientation(1);
  MOVJ(pos_PourC, int(Zs::Pour));
  WaitDobot(delaytime * 11);
  // delay(900);
  for (int i = 0; i < 10; i++) {
    CounterServo.write((i % 2) ? 180 : 0);
    delay(delaytime);
  }
  ArmOrientation(1);
  MOVJ(pos_PourC, int(Zs::ServerExec));
}
void CarryA() {
  GripServo.write(int(GripAngle::Release));
  ArmOrientation(1);
  MOVJ(pos_PreA, int(Zs::Move));
  ArmOrientation(1);
  MOVJ(pos_PreA, int(Zs::Catch));
  WaitDobot(1000);
  delay(2000);
  GripServo.write(int(GripAngle::Catch));
  ArmOrientation(1);
  MOVJ(pos_PreA, int(Zs::Carry));

  ArmOrientation(0);
  MOVJ(pos_PostA, int(Zs::Carry));
  ArmOrientation(0);
  MOVJ(pos_PostA, int(Zs::Release));
  WaitDobot(300);
  delay(3500);
  GripServo.write(int(GripAngle::Release));
  ArmOrientation(0);
  MOVJ(pos_PostAu, int(Zs::Release));
  ArmOrientation(0);
  MOVJ(pos_PostAu, int(Zs::Carry));
  ExecFlagTest = 0;
}

void CarryB() {
  GripServo.write(int(GripAngle::Release));
  ArmOrientation(1);
  MOVJ(pos_PreB, int(Zs::Move));
  ArmOrientation(1);
  MOVJ(pos_PreB, int(Zs::Catch));
  WaitDobot(1000);
  delay(2000);
  GripServo.write(int(GripAngle::Catch));
  ArmOrientation(1);
  MOVJ(pos_PreB, int(Zs::Carry));

  ArmOrientation(0);
  MOVJ(pos_PostB, int(Zs::Carry));
  ArmOrientation(0);
  MOVJ(pos_PostB, int(Zs::Release));
  WaitDobot(300);
  delay(5000);
  GripServo.write(int(GripAngle::Release));
  ArmOrientation(0);
  MOVJ(pos_PostBu, int(Zs::Release));
  ArmOrientation(0);
  MOVJ(pos_PostBu, int(Zs::Carry));
  ExecFlagTest = 0;
}
void CarryC() {
  GripServo.write(int(GripAngle::Release));
  ArmOrientation(1);
  MOVJ(pos_PreC, int(Zs::Move));
  ArmOrientation(1);
  MOVJ(pos_PreC, int(Zs::Catch));
  WaitDobot(1000);
  delay(2000);
  GripServo.write(int(GripAngle::Catch));
  ArmOrientation(1);
  MOVJ(pos_PreC, int(Zs::Carry));

  ArmOrientation(0);
  MOVJ(pos_PostC, int(Zs::Carry));
  ArmOrientation(0);
  MOVJ(pos_PostC, int(Zs::Release));
  WaitDobot(300);
  delay(3500);
  GripServo.write(int(GripAngle::Release));
  ArmOrientation(1);
  MOVJ(pos_PostCu, int(Zs::Release));
  ArmOrientation(1);
  MOVJ(pos_PostCu, int(Zs::Carry));
  ExecFlagTest = 0;
}
/*
座�

// void delaye(int duration){
//   static uint64_t _startTime = millis();
//   uint64_t _execTime = _startTime-millis();

// }
*/