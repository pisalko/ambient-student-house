#include <Servo.h>
Servo curtains;
int curtainsInteger;
unsigned long lastCheck;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  curtains.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    unsigned long startTime = millis();
    int previousCurtainsPosition = curtainsInteger;
    String servoCommand = Serial.readStringUntil('\r');
    int startIndex = servoCommand.indexOf("&") + 1;
    int endIndex = servoCommand.indexOf(servoCommand.length());
    String curtainsPos = servoCommand.substring(startIndex, endIndex);
    curtainsInteger = curtainsPos.toInt();
    curtains.write(curtainsInteger);
  }
  //curtains.write(90);
  Serial.println(curtainsInteger);
}
