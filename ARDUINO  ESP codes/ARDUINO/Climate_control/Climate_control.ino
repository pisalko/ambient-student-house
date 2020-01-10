const int DC_FAN = 10;
unsigned long previousCheck;
int INTERVAL = 1000;
int DC_FAN_SPEED;
int weather_factor;
int temperature_factor;
int humidity_factor;
String weatherReport;
String weather;
String temperatureString;
double temperature;
String humidityString;
double humidity;

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);
}

void loop() {
  if (millis() - previousCheck > INTERVAL) {
    if (Serial.available()) {
      weatherReport = Serial.readStringUntil('\r');
      int startIndex = 0;
      int endIndex = weatherReport.indexOf(";");
      weather = weatherReport.substring(startIndex, endIndex);
      startIndex = weatherReport.indexOf(";") + 1;
      endIndex = weatherReport.indexOf(",");
      temperatureString = weatherReport.substring(startIndex, endIndex);
      temperature = temperatureString.toDouble();
      startIndex = weatherReport.indexOf(",") + 1;
      endIndex = weatherReport.indexOf(weatherReport.length());
      humidityString = weatherReport.substring(startIndex, endIndex);
      humidity = humidityString.toDouble();
      if (temperature >= 18 && temperature <= 40)
        temperature_factor = map(temperature, 18, 40, 0, 85);
      else
        temperature_factor = 0;
      if (humidity >= 70 && humidity <= 100)
        humidity_factor = map(humidity, 70, 100, 0, 85);
      else
        humidity_factor = 0;
    }
    previousCheck = millis();
    /*Serial.println(weather);
      Serial.println(temperature);
      Serial.println(humidity);*/
  }
  if (weather == "Sun")
    weather_factor = 255 / 3;
  else
    weather_factor = 0;
  DC_FAN_SPEED = weather_factor + temperature_factor + humidity_factor;
  analogWrite(DC_FAN, DC_FAN_SPEED);
  Serial.println(DC_FAN_SPEED);  
}
