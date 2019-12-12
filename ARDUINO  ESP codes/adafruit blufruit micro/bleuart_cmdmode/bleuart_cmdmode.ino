#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#include <SoftwareSerial.h>

    #define FACTORYRESET_ENABLE         1


bool once = true;
String data = "";

SoftwareSerial s(13, 12);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println("here");

  s.begin(9600);
  while (!s);

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

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
    // Some data was found, its in the buffer
    Serial.print(F("[Recv] "));
    Serial.println(ble.buffer);

    // TRANSFER UART HERE

    String data = ble.buffer;
    for (int i = 0; i < data.length(); i++)s.write(data[i]);

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
