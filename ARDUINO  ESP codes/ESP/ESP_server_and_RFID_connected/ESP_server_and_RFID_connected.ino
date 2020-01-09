//rfid code
#define SS_PIN D4  //D2
#define RST_PIN D3 //D1
//end of rfid code

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

//rfid code
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//rx tx
SoftwareSerial s(D1, D2);

String oldContent = "";
bool once = true;
bool once2 = true;
//end of rfid code

String ssid = "Az";
String pass = "gonoreq5";

char** dataARRAY;

int size = 0;
int cap = 2;

WiFiServer server(80);

HTTPClient http;

unsigned long timestamp;

const int REQUEST_PIN = D0;
bool lock = false;

const int G_LED = D8;

String serverIP = "";

void setup() 
{  
  Serial.begin(9600);
  s.begin(9600);
  while(!s);

  WiFi.mode(WIFI_STA);
  server.begin(); 

  //rfid code
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  //end of rfid code

  Serial.println(WiFi.localIP());

  dataARRAY = (char **)malloc(sizeof(char[12]) * cap);
  pinMode(REQUEST_PIN, INPUT);

  pinMode(G_LED, OUTPUT);
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


  if(millis() - timestamp > 5000){
    oldContent = "";
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

    // CARD CHECK HERE

    bool found = false;
    char dataArr[content.length()];
    content.toCharArray(dataArr, content.length()+1);
    for(int i=0; i<size; i++){
      if(strcmp(dataArr, dataARRAY[i]) == 0){
        found = true;
        break;
      }
    }

    if(found){
      digitalWrite(G_LED, HIGH);
      delay(2000);
      digitalWrite(G_LED, LOW);
    }

    timestamp = millis();
    
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
    if (strcmp(dataARRAY[i], card) == 0) return;
  }

  dataARRAY[size] = (char *) malloc(sizeof(char[12]));
  cardGiven.toCharArray(dataARRAY[size], 12);

  size++;

  if (size == cap - 1) {
    cap += 3;
    dataARRAY = (char **) realloc(dataARRAY, sizeof(char[12]) * cap);

    if (dataARRAY == NULL)Serial.println("QJ PISHKI GEI SMOTAN NAUCHI SE DA KODISH PEERAS NAEBAN");
  }


  Serial.println("-------------");
  for (int i = 0; i < size; i++) {
    Serial.println(String(dataARRAY[i]));
  }
  Serial.println("-------------");
}

String headerS;

void loop() {

  if(Serial.available() > 0){

    String data = "";

    while(Serial.available() > 0){
      data += (char)Serial.read();
    }

    Serial.println("SEND " + data);

    for(int i=0; i<data.length(); i++){
      s.write(data[i]);
    }
  }
  
  if(s.available()){
    String data = s.readString();

    if(data[0] == '~'){
      String ssidNew = "";
      for(int i=0; i<data.length(); i++){
        if(data[i] != '~' && data[0] != ';'){
          ssidNew += data[i];
        }
      }
      ssid = ssidNew;
      Serial.println("SSID>" + ssid);
      //WiFi.begin(ssid, pass);
    }else if(data[0] == ';'){
      String passNew = "";
      for(int i=0; i<data.length(); i++){
        if(data[i] != '~' && data[i] != ';'){
          passNew += data[i];
        }
      }
      pass = passNew;
      Serial.println("PASS>" + pass);
    }else if(data[0] == '!'){
      String ipNew = "";
      for(int i=0; i<data.length(); i++){
        if(data[i] != '!'){
          ipNew += data[i];
        }
      }
      serverIP = ipNew;
    }else if(data[0] == '|'){
      WiFi.begin(ssid, pass);
      
      while(WiFi.status() == WL_DISCONNECTED){
        Serial.println(".");
        delay(500);
      }
      String ipESP = WiFi.localIP().toString();
      Serial.println(ipESP);

      for(int i=0; i<ipESP.length(); i++){
        s.write(ipESP[i]);
      }

      http.begin("http://"+serverIP+":42069");
      http.addHeader("Content-Type", "text/plain");
       
      int httpCode = http.POST("1,"+ipESP);
      String payload = http.getString();
         
      http.end();

      Serial.println(payload);
      
    }
    if (data[0] == 't')
    {
      Serial.println("VLIZA BRAT");
      if (data == "time")
      {
        Serial.println("TUAK SUSHTO MALIIIIIIIIIIIIII");
        http.begin("http://worldtimeapi.org/api/timezone/Europe/Amsterdam.txt");
      int httpCode = http.GET();

      String response = http.getString();

      int startIndex = response.indexOf("main") + 7;
      int endIndex = response.indexOf("description") - 3;

      String timeFromApi = response.substring(startIndex, endIndex);

      for(int i=0; i<timeFromApi.length(); i++)s.write(timeFromApi[i]);
      Serial.println(timeFromApi);
      data = "";
      }
    }
    Serial.println(data);
  }

  
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

  if(digitalRead(REQUEST_PIN) == HIGH){
    if(!lock){
      lock = true;
      http.begin("http://api.openweathermap.org/data/2.5/weather?q=Eindhoven%2Cnl&APPID=effcaf77df40f6a9c26a2c67d698e1e4");
      int httpCode = http.GET();

      String response = http.getString();

      int startIndex = response.indexOf("main") + 7;
      int endIndex = response.indexOf("description") - 3;

      String weather = response.substring(startIndex, endIndex);

      for(int i=0; i<weather.length(); i++)s.write(weather[i]);
      Serial.println(weather);
    }
  }else{
    lock = false;
  }
}







/* GET
  http.begin("http://jsonplaceholder.typicode.com/users/1");
  int httpCode = http.GET();

  String response = http.getString();

  http.end();
*/

/* POST
  http.begin("http://192.168.1.88:8085/hello");
 http.addHeader("Content-Type", "text/plain");
 
 int httpCode = http.POST("Message from ESP8266");
 String payload = http.getString();
   
 http.end();
 */
