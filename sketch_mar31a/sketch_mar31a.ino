#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Anukul Pradhan";
const char* password = "Apradhan";

WebServer server(80);

// Motor A (Right)
const int pwma = 13;
const int ain1 = 12;
const int ain2 = 14;

// Motor B (Left)
const int pwmb = 25;
const int bin1 = 26;
const int bin2 = 27;

int motorSpeed = 200; // Speed (0-255)

void setup() {
  Serial.begin(115200);

  // Initialize Motor Pins
  pinMode(pwma, OUTPUT); pinMode(ain1, OUTPUT); pinMode(ain2, OUTPUT);
  pinMode(pwmb, OUTPUT); pinMode(bin1, OUTPUT); pinMode(bin2, OUTPUT);
  stopMotors();

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // COPY THIS IP ADDRESS FOR YOUR HTML FILE

  // Setup Web Server Routes
  server.on("/F", HTTP_GET, []() { moveForward(); sendResponse(); });
  server.on("/B", HTTP_GET, []() { moveBackward(); sendResponse(); });
  server.on("/L", HTTP_GET, []() { turnLeft(); sendResponse(); });
  server.on("/R", HTTP_GET, []() { turnRight(); sendResponse(); });
  server.on("/S", HTTP_GET, []() { stopMotors(); sendResponse(); });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

// --- Helper function to send CORS headers ---
void sendResponse() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK");
}

// --- Motor Control Functions ---
void moveForward() {
  digitalWrite(ain1, HIGH); digitalWrite(ain2, LOW);
  digitalWrite(bin1, HIGH); digitalWrite(bin2, LOW);
  analogWrite(pwma, motorSpeed); analogWrite(pwmb, motorSpeed);
}

void moveBackward() {
  digitalWrite(ain1, LOW); digitalWrite(ain2, HIGH);
  digitalWrite(bin1, LOW); digitalWrite(bin2, HIGH);
  analogWrite(pwma, motorSpeed); analogWrite(pwmb, motorSpeed);
}

void turnRight() {
  digitalWrite(ain1, LOW); digitalWrite(ain2, HIGH); 
  digitalWrite(bin1, HIGH); digitalWrite(bin2, LOW); 
  analogWrite(pwma, motorSpeed); analogWrite(pwmb, motorSpeed);
}

void turnLeft() {
  digitalWrite(ain1, HIGH); digitalWrite(ain2, LOW); 
  digitalWrite(bin1, LOW); digitalWrite(bin2, HIGH); 
  analogWrite(pwma, motorSpeed); analogWrite(pwmb, motorSpeed);
}

void stopMotors() {
  digitalWrite(ain1, LOW); digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW); digitalWrite(bin2, LOW);
  analogWrite(pwma, 0); analogWrite(pwmb, 0);
}