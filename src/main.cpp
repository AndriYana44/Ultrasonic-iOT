#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// deklare pin
#define LED D2
#define triggerPin D8
#define echoPin D7

// deklare variable and constant
const char* ssid = "vivo 2019";
const char* pass = "12345678an";
const char* host = "192.168.43.197";

// declare method
short ultraSonic();

void setup() {
  Serial.begin(9600);
  // wifi connection
  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, pass);

  pinMode(LED, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // check connection
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("WiFi Connected!");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);

  int jarak = ultraSonic();
  Serial.print(jarak);
  Serial.println(" cm");
  delay(500);

  // connection to server
  WiFiClient client;
  if(!client.connect(host, 80)) {
    Serial.println("Connection Failed!");
    return;
  }

  // send data to server
  String url;
  HTTPClient http;
  url = "http://192.168.43.197:8080/server.php?jarak=" + String(jarak);
  Serial.print("Request to: ");
  Serial.println(url);

  // access link
  http.begin(client, url);
  // method GET
  http.GET();
  http.end();
  delay(3000);
}

short ultraSonic() {
  // declare variable
  short jarak, duration;
  // IO sensor
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  // calibration sensor
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;

  return jarak;
}