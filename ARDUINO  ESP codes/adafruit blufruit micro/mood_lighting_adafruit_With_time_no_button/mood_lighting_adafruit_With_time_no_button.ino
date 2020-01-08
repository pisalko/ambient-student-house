const int RED = 5;
const int GREEN = 6;
const int BLUE = 7;
//const int KEY1 = 2;

int r = 200;
int g = 200;
int b = 200;

String moodLighting = "";

//enum lightMode {party, meeting, study};
//enum lightMode mode = party;

//unsigned long long debounceTimer, lastCheck = 0;

//bool key1State, key1OldState, key1Click = true;

void setup() {
  Serial.begin(9600);
  //pinMode(KEY1, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  if (moodLighting == "party") {
    if (millis() - lastCheck > 500) {
      r = random(0, 255);
      g = random(0, 255);
      b = random(0, 255);
      lastCheck = millis();
    }
  }
  else if (moodLighting == "default") // tuka s vremeto shano mizerii
  {
    r = 0;
    g = 108;
    b = 214;
  }

  else if (moodLighting == "study")
  {
    r = 0;
    g = 5;
    b = 11;
  }
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}
