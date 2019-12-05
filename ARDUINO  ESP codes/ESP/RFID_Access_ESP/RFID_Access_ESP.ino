#define SS_PIN D4  //D2
#define RST_PIN D3 //D1

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String oldContent = "";
bool once = true;
bool once2 = true;

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.flush();
  Serial.println();
}
void loop()
{
  if(once2)
  {
    Serial.println();
    once2 = false;
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }


  String content = "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  
  if (once)
  {
    oldContent = content;
    
  }
  
  if (oldContent != content || once)
  {
    Serial.flush();
    Serial.println(content);
    oldContent = content;
    once = false;
  }
  else
  {
    //If the card read is the same
  }
}
