//rfid code
#define SS_PIN D4  //D2
#define RST_PIN D3 //D1
//end of rfid code

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

//rfid code
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String oldContent = "";
bool once = true;
bool once2 = true;
//end of rfid code

String ssid = "Az";
String pass = "gonoreq5";

char** data;

int size = 0;
int cap = 2;

WiFiServer server(80);

HTTPClient http;

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  server.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");

    //rfid code
    SPI.begin();      // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
    //end of rfid code
  }

  Serial.println(WiFi.localIP());

  data = (char **)malloc(sizeof(char[12]) * cap);
}

void RFIDdetected() {
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
  content = content.substring(1);
  
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

void add(String cardGiven) {
  char card[12];
  cardGiven.toCharArray(card, 12);

  for (int i = 0; i < size; i++) {
    if (strcmp(data[i], card) == 0) return;
  }

  data[size] = (char *) malloc(sizeof(char[12]));
  cardGiven.toCharArray(data[size], 12);

  size++;

  if (size == cap - 1) {
    cap += 3;
    data = (char **) realloc(data, sizeof(char[12]) * cap);

    if (data == NULL)Serial.println("QJ PISHKI GEI SMOTAN NAUCHI SE DA KODISH PEERAS NAEBAN");
  }


  Serial.println("-------------");
  for (int i = 0; i < size; i++) {
    Serial.println(String(data[i]));
  }
  Serial.println("-------------");
}

String headerS;

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) { // loop while the client's connected
      //currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte
        headerS += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/plain");


            if (headerS.indexOf("GET /test") >= 0) {
              client.println("Content-Length: 3");
              client.println("Connection: close");
              client.println();
              client.println("GET");
            } else if (headerS.indexOf("POST /add") >= 0) {
              int content_length = 0, index = headerS.indexOf("Content-Length:"), i;

              for (i = index; headerS[i] != '\n'; i++);

              content_length = headerS.substring(index + 16, i).toInt();

              delay(10);

              String data = "";

              for (int ii = 0; ii < content_length; ii++) {
                data += (char)client.read();
              }

              for (int i = 0; i < data.length(); i++) {
                if ((data[i] > 90 || data[i] < 48) && data[i] != 32) {
                  data = "";
                  break;
                }
              }


              Serial.println(">>" + data);

              if (data.length() == 11)add(data);


              client.println("Content-Length: 2");
              client.println("Connection: close");
              client.println();
              if (data == "")client.println("ne");
              else client.println("da");
            }

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    headerS = "";
    // Close the connection
    client.stop();
  }
  RFIDdetected();
}







/* GET
  http.begin("http://jsonplaceholder.typicode.com/users/1");
  int httpCode = http.GET();

  String response = http.getString();

  http.end();
*/
