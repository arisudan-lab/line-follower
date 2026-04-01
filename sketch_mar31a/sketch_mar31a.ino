#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Anukul Pradhan";
const char* password = "Apradhan";

#define ain1 26
#define ain2 27
#define pwma 14

#define bin1 33
#define bin2 32
#define pwmb 25

WebServer server(80);

int motorSpeed = 200;

void setup() {
  Serial.begin(115200);

  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);

  // NEW PWM (ESP32 Core v3+)
  ledcAttach(pwma, 1000, 8);
  ledcAttach(pwmb, 1000, 8);

  stopMotors();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());

  server.on("/F", HTTP_GET, []() { moveForward(); sendResponse(); });
  server.on("/B", HTTP_GET, []() { moveBackward(); sendResponse(); });
  server.on("/L", HTTP_GET, []() { turnLeft(); sendResponse(); });
  server.on("/R", HTTP_GET, []() { turnRight(); sendResponse(); });
  server.on("/S", HTTP_GET, []() { stopMotors(); sendResponse(); });

  server.begin();
}

void loop() {
  server.handleClient();
}

void sendResponse() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK");
}

void moveForward() {
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);

  ledcWrite(pwma, motorSpeed);
  ledcWrite(pwmb, motorSpeed);
}

void moveBackward() {
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);

  ledcWrite(pwma, motorSpeed);
  ledcWrite(pwmb, motorSpeed);
}

void turnRight() {
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);

  ledcWrite(pwma, motorSpeed);
  ledcWrite(pwmb, motorSpeed);
}

void turnLeft() {
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);

  ledcWrite(pwma, motorSpeed);
  ledcWrite(pwmb, motorSpeed);
}

void stopMotors() {
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);

  ledcWrite(pwma, 0);
  ledcWrite(pwmb, 0);
}