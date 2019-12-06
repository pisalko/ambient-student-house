// Input from Motion Sensor
const int pirPin = 7;

// Variable to store value from PIR
int pirValue;

void setup() {
  Serial.begin(9600);

  // Setup PIR as Input
  pinMode(pirPin, INPUT);
  //we let the sensor detect ambient IR light
  delay(60000);
}

void loop() {
    // Get value from motion sensor
    pirValue = digitalRead(pirPin);//detectedPin
    Serial.println(pirValue);

    if (pirValue == 1)
    {
      Serial.println("LED lit up");
    }

    if (pirValue == 0)
    {
      Serial.println("NO LED");
    } 
}
