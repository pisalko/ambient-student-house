#include <Bounce2.h>
Bounce debouncer1 = Bounce();
const int RED = 5;
const int GREEN = 6;
const int BLUE = 9;
const int BUTTON = 2;
int r = 200;
int g = 200;
int b = 200;
enum lightMode {party, meeting, study};
enum lightMode mode = party;
unsigned int lastCheck;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  debouncer1.attach(BUTTON);
  debouncer1.interval(5); // interval in ms
}

void loop() {
  debouncer1.update();
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
  analogWrite(BLUE, b);
}
