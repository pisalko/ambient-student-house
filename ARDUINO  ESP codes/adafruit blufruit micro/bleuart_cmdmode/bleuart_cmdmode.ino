#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#include <SoftwareSerial.h>

#define FACTORYRESET_ENABLE         1

const int RED = A1;
const int GREEN = A2;
const int BLUE = A3;
int r = 200;
int g = 200;
int b = 200;
unsigned long lastCheck = 0;

bool lightModeParty, lightModeDefault, lightModeStudy, lightModeOff = false;
bool once = true;
String data = "";

SoftwareSerial s(13, 12);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  //while (1);
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println("here");

  s.begin(9600);
  while (!s);

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);


  /* Disable command echo from Bluefruit */
  ble.echo(false);
  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
    delay(200);
  }
  Serial.println("Connection Established.");
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{

  if (ble.isConnected())
  {

    if (s.available()) {
      Serial.println("tuka vliza brat");
      data = s.readString();
      once = true;
      // Send characters to Bluefruit
      Serial.print("[Send] ");
      Serial.println(data);

      ble.print("AT+BLEUARTTX=");
      ble.println(data);

      // check response stastus
      if (! ble.waitForOK() ) {
        Serial.println(F("Failed to send?"));
      }
    }


    // Check for incoming characters from Bluefruit
    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0) {
      // no data
      return;
    }
    else
    {
      // Some data was found, its in the buffer
      Serial.print(F("[Recv] "));
      Serial.println(ble.buffer);
    }

    // TRANSFER UART HERE

    String data = ble.buffer;
    //-----------------------------------------
    //-------from here lights
    if (data == "lp")
    {
      //do stuff light party
      lightModeOff = false;
      lightModeStudy = false;
      lightModeDefault = false;
      lightModeParty = true;
    }
    else if (data == "ls")
    {
      //do stuff light study
      lightModeOff = false;
      lightModeStudy = true;
      lightModeDefault = false;
      lightModeParty = false;
    }
    else if (data == "ld")
    {
      lightModeOff = false;
      lightModeStudy = false;
      lightModeDefault = true;
      lightModeParty = false;
      //do stuff light default (ith time)
    }
    else if (data == "lo")
    {
      lightModeOff = true;
      lightModeStudy = false;
      lightModeDefault = false;
      lightModeParty = false;
      //do stuff light off
    }
    //------to here for lights
    //-----------------------------------------
    /*//------from here fan state
    else if (data.startsWith("fs"))
    {
      //do stuff fan
    }
    //-------to here fan state*/
    //-----------------------------------------
    //-------from here curtain state
    else if (data.startsWith("cs"))
    {
      //do stuff curtain, servo
    }
    //-------to here curtain state
    //-----------------------------------------
    //-------from here send to ESP
    else
    {
      for (int i = 0; i < data.length(); i++)
        s.write(data[i]);
    }
    //------to here send to ESP
    //-----------------------------------------

    ble.waitForOK();
  }
  else
  {
    if (once)
    {
      Serial.println("Disconnected from device");
      once = false;
    }
  }

  if (lightModeOff)
  {
    r = 0;
    g = 0;
    b = 0;
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
  }
  else if (lightModeStudy)
  {
    r = 201;
    g = 226;
    b = 255;
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
  }
  else if (lightModeDefault)
  {
    //fancy stuff with time
    r = 0;
    g = 0;
    b = 255;
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
  }
  else if (lightModeParty)
  {
    if (millis() - lastCheck > 500)
    {
      r = random(0, 255);
      g = random(0, 255);
      b = random(0, 255);
      lastCheck = millis();
      analogWrite(RED, r);
      analogWrite(GREEN, g);
      analogWrite(BLUE, b);
      /*Serial.println("vliza zavinagi brat");
        Serial.println(r);
        Serial.println(g);
        Serial.println(b);*/
    }
  }
}

/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while ( (!Serial.available()) && !timeout.expired() ) {
    delay(1);
  }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count = 0;
  do
  {
    count += Serial.readBytes(buffer + count, maxSize);
    delay(2);
  } while ( (count < maxSize) && (Serial.available()) );

  return true;
}
