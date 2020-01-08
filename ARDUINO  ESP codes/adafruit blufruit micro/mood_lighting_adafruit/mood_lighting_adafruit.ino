const int RED = 5;
const int GREEN = 6;
const int BLUE = 9;
const int KEY1 = 2;

int r = 200;
int g = 200;
int b = 200;

enum lightMode {party, meeting, study};
enum lightMode mode = party;

unsigned long long debounceTimer = 0;

bool key1State, key1OldState, key1Click = true;

void setup() {
  Serial.begin(9600);
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  if (millis() - debounceTimer < 50)          //Debounce method
  {
    return;
  }
  else
  {
    debounceTimer = millis();
  }
  //--------------------------------------------------------------
  key1State = digitalRead(KEY1);
  //-------------------------------------------------------------
  if (key1State != key1OldState)        //StateChange Machine KEY1
  {
    key1OldState = key1State;
    key1Click = true;
  }
  else
  {
    key1Click = false;
  }
  //------------------------------------------------------------------
  if (key1Click && !key1State)      //Making KEY1 Click do something
  {
    mode -= -1;
    if (mode == 3)
      mode = 0;
  }

  switch (mode) {
    case party:
      if (millis() - lastCheck > 500) {
        r = random(0, 255);
        g = random(0, 255);
        b = random(0, 255);
        lastCheck = millis();
      }
      break;
    case meeting:
      r = 0;
      g = 108;
      b = 214;
      //RGB values taken from: http://planetpixelemporium.com/tutorialpages/light.html
      //According to some conflicting articles along with our own experience, warm light is best for meeting rooms.
      break;
    case study:
      r = 0;
      g = 5;
      b = 11;
      //According to some conflicting articles along with our own experience, cold light (4000-5000K) is most suitable for studying.
      break;
  }
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);




  /*debouncer1.update();
    int buttonValue = debouncer1.read();
    if (debouncer1.fell()) {
    mode = mode + 1;
    if (mode == 3)
      mode = 0;
    }
    switch (mode) {
    case party:
      if (millis() - lastCheck > 500) {
          r = random(0, 255);
          g = random(0, 255);
          b = random(0, 255);
        lastCheck = millis();
      }
      break;
    case meeting:
      r = 0;
      g = 108;
      b = 214;
      //RGB values taken from: http://planetpixelemporium.com/tutorialpages/light.html
      //According to some conflicting articles along with our own experience, warm light is best for meeting rooms.
      break;
    case study:
      r = 0;
      g = 5;
      b = 11;
      //According to some conflicting articles along with our own experience, cold light (4000-5000K) is most suitable for studying.
      break;
    }
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);*/
}
