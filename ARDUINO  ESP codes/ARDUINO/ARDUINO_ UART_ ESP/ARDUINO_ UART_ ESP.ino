#include <SoftwareSerial.h>

SoftwareSerial s(5, 6);

const int REQUEST_PIN = 2;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  s.begin(9600);
  while(!s);

  pinMode(REQUEST_PIN, OUTPUT);
  digitalWrite(REQUEST_PIN, LOW);
}

String data = "";

void loop() {
  if(s.available() >= 0)data = s.readString();
  data.trim();
  if(data != "")Serial.println(data);
}
