#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <Wire.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <ESP8266TrueRandom.h>

const byte STARTUP[][8] = {
    B00100100,
    B01011010,
    B10000001,
    B10000001,
    B10000001,
    B01000010,
    B00100100,
    B00011000};

const byte UPDATE[][8] = {
    B11111111,
    B00000000,
    B00111100,
    B00111100,
    B00111100,
    B00111100,
    B00000000,
    B11111111};

const byte HAPPY[][8] = {
    {B00000000,
     B01000010,
     B01000010,
     B01000010,
     B00000000,
     B10000001,
     B01111110,
     B00000000},
    {B00000000,
     B00000000,
     B00000000,
     B01100110,
     B00000000,
     B10000001,
     B01111110,
     B00000000}};

const byte SAD[][8] = {
    {B00000000,
     B01000010,
     B01000010,
     B01000010,
     B00000000,
     B00000000,
     B01111110,
     B10000001},
    {B00000000,
     B00000000,
     B00000000,
     B01100110,
     B00000000,
     B00000000,
     B01111110,
     B10000001}};

const byte ANGRY[][8] = {
    {B01100110,
     B01000010,
     B01000010,
     B01000010,
     B00000000,
     B00000000,
     B01111110,
     B10000001},
    {B01100110,
     B00000000,
     B00000000,
     B01100110,
     B00000000,
     B00000000,
     B01111110,
     B10000001}};

const byte INDIFFERENT[][8] = {
    {B00000000,
     B01000010,
     B01000010,
     B01000010,
     B00000000,
     B00000000,
     B01111110,
     B00000000},
    {B00000000,
     B00000000,
     B00000000,
     B01100110,
     B00000000,
     B00000000,
     B01111110,
     B00000000}};

const byte SHOCKED[][8] = {
    {B00000000,
     B01000010,
     B01000010,
     B01000010,
     B00011000,
     B00100100,
     B00100100,
     B00011000},
    {B00000000,
     B00000000,
     B00000000,
     B01100110,
     B00011000,
     B00100100,
     B00100100,
     B00011000}};

const byte DEAD[][8] = {
    {B00000000,
     B10100101,
     B01000010,
     B10100101,
     B00000000,
     B00000000,
     B01111110,
     B10000001},
    {B00000000,
     B00000000,
     B00000000,
     B10100101,
     B00000000,
     B00000000,
     B01111110,
     B10000001}};

const byte AWKWARD[][8] = {
    {B00000000,
     B01000010,
     B01000010,
     B01000010,
     B00000000,
     B01000000,
     B00100000,
     B00010000},
    {B00000000,
     B00000000,
     B00000000,
     B01100110,
     B00000000,
     B01000000,
     B00100000,
     B00010000}};

const byte ROBOT[][8] = {
    {B00000000,
     B11000000,
     B11000000,
     B00000000,
     B00000000,
     B00000000,
     B00111100,
     B00000000},
    {B00000000,
     B01100000,
     B01100000,
     B00000000,
     B00000000,
     B00000000,
     B00111100,
     B00000000},
    {B00000000,
     B00110000,
     B00110000,
     B00000000,
     B00000000,
     B00000000,
     B00111100,
     B00000000},
    {B00000000,
     B00011000,
     B00011000,
     B00000000,
     B00000000,
     B00000000,
     B00111100,
     B00000000},
    {B00000000,
     B00001100,
     B00001100,
     B00000000,
     B00000000,
     B00000000,
     B00111100,
     B00000000},
    {B00000000,
     B00000110,
     B00000110,
     B00000000,
     B00000000,
     B00000000,
     B00111100,
     B00000000},
    {B00000000,
     B00000011,
     B00000011,
     B00000000,
     B00000000,
     B00000000,
     B00111100,
     B00000000}};

const byte PLEASED[][8] = {
    {B00000000,
     B01000010,
     B10100101,
     B00000000,
     B00000000,
     B00000000,
     B00100100,
     B00011000},
    {B00000000,
     B00000000,
     B01000010,
     B00000000,
     B00000000,
     B00000000,
     B00100100,
     B00011000}};

const byte ALPHA[][8] = {
    {B00000000,
     B00111100,
     B01100110,
     B01100110,
     B01111110,
     B01100110,
     B01100110,
     B01100110},
    {B00000000,
     B01111100,
     B01100110,
     B01100110,
     B01111100,
     B01100110,
     B01100110,
     B01111100},
    {B00000000,
     B00111100,
     B01100110,
     B01100000,
     B01100000,
     B01100000,
     B01100110,
     B00111100},
    {B00000000,
     B01111100,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B01111100},
    {B00000000,
     B01111110,
     B01100000,
     B01100000,
     B01111100,
     B01100000,
     B01100000,
     B01111110},
    {B00000000,
     B01111110,
     B01100000,
     B01100000,
     B01111100,
     B01100000,
     B01100000,
     B01100000},
    {B00000000,
     B00111100,
     B01100110,
     B01100000,
     B01100000,
     B01101110,
     B01100110,
     B00111100},
    {B00000000,
     B01100110,
     B01100110,
     B01100110,
     B01111110,
     B01100110,
     B01100110,
     B01100110},
    {B00000000,
     B00111100,
     B00011000,
     B00011000,
     B00011000,
     B00011000,
     B00011000,
     B00111100},
    {B00000000,
     B00011110,
     B00001100,
     B00001100,
     B00001100,
     B01101100,
     B01101100,
     B00111000},
    {B00000000,
     B01100110,
     B01101100,
     B01111000,
     B01110000,
     B01111000,
     B01101100,
     B01100110},
    {B00000000,
     B01100000,
     B01100000,
     B01100000,
     B01100000,
     B01100000,
     B01100000,
     B01111110},
    {B00000000,
     B01100011,
     B01110111,
     B01111111,
     B01101011,
     B01100011,
     B01100011,
     B01100011},
    {B00000000,
     B01100011,
     B01110011,
     B01111011,
     B01101111,
     B01100111,
     B01100011,
     B01100011},
    {B00000000,
     B00111100,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B00111100},
    {B00000000,
     B01111100,
     B01100110,
     B01100110,
     B01100110,
     B01111100,
     B01100000,
     B01100000},
    {B00000000,
     B00111100,
     B01100110,
     B01100110,
     B01100110,
     B01101110,
     B00111100,
     B00000110},
    {B00000000,
     B01111100,
     B01100110,
     B01100110,
     B01111100,
     B01111000,
     B01101100,
     B01100110},
    {B00000000,
     B00111100,
     B01100110,
     B01100000,
     B00111100,
     B00000110,
     B01100110,
     B00111100},
    {B00000000,
     B01111110,
     B01011010,
     B00011000,
     B00011000,
     B00011000,
     B00011000,
     B00011000},
    {B00000000,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B00111110},
    {B00000000,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B01100110,
     B00111100,
     B00011000},
    {B00000000,
     B01100011,
     B01100011,
     B01100011,
     B01101011,
     B01111111,
     B01110111,
     B01100011},
    {B00000000,
     B01100011,
     B01100011,
     B00110110,
     B00011100,
     B00110110,
     B01100011,
     B01100011},
    {B00000000,
     B01100110,
     B01100110,
     B01100110,
     B00111100,
     B00011000,
     B00011000,
     B00011000},
    {B00000000,
     B01111110,
     B00000110,
     B00001100,
     B00011000,
     B00110000,
     B01100000,
     B01111110}};

byte textFrames[99][8];

const IPAddress apIP(192, 168, 1, 1);
const bool prod = true; //CHANGE IN PRODUCTION
const char *apSSID = "BMO Setup";
const int FW_VERSION = 67;
const char *fwUrlBase = "http://printrworks.co.uk/bmo/ota/";

boolean settingMode;
String ssidList;
DNSServer dnsServer;
ESP8266WebServer webServer(80);

WiFiServer server(80);
String header;
int mood = 0; // HAPPY = 0
              // ANGRY = 1
              // SAD = 2
              // INDIFFERENT = 3
              // SHOCKED = 4
              // DEAD = 5
              // AWKWARD = 6
              // ROBOT = 7
              // PLEASED = 8
              // SCROLLING = 98
              // WRITING = 99
char wordStream[99];
uint8_t letterIndex = 0;
uint16_t letterTime = 1000;
uint16_t scrollTime = 200;
os_timer_t blinkTimer;
os_timer_t frameTimer;
os_timer_t updateTimer;
bool shouldBlink = false;
bool shouldDraw = false;
bool shouldUpdate = false;
byte brightness = 0xE1;
bool displayOn = true;
uint16_t blinkLow = 1500;
uint16_t blinkHigh = 7000;
uint16_t frameTime = 200;
uint8_t frameScrollCount = 0;
uint16_t frameScrollTotal = 0;
uint8_t frameScrollOffset = 0;
byte curScrollFrame[8];

void checkForUpdates();
void doLights();
void drawFace();
boolean restoreConfig();
boolean checkConnection();
void startWebServer();
void setupMode();
String urlDecode(String input);
String makePage(String title, String contents);
void i2cWrite(byte send);
void doTimer(void *arg);
void forceUpdate();
void lightOnTime(void *arg);
void updateOnTime(void *arg);
unsigned char reverse(unsigned char b);
void copyFrameData(uint8_t number, char frame);
void getNextFrame();

void doTimer(void *arg)
{
  shouldBlink = true;
}

void lightOnTime(void *arg)
{
  shouldDraw = true;
}

void updateOnTime(void *arg)
{
  shouldUpdate = true;
}

void setup()
{
  Serial.begin(78440);
  EEPROM.begin(512);
  delay(1000);

  wordStream[0] = 't';
  wordStream[1] = 'e';
  wordStream[2] = 's';
  wordStream[3] = 't';
  wordStream[4] = '?';

  copyFrameData(0, 't');
  copyFrameData(1, 'e');
  copyFrameData(2, 's');
  copyFrameData(3, 't');

  os_timer_setfn(&blinkTimer, (os_timer_func_t *)doTimer, (void *)0);
  os_timer_setfn(&frameTimer, (os_timer_func_t *)lightOnTime, (void *)0);
  os_timer_setfn(&updateTimer, (os_timer_func_t *)updateOnTime, (void *)0);

  Wire.begin();
  i2cWrite(0x21);
  i2cWrite(0x81);

  Wire.beginTransmission(0x70);
  Wire.write(0x00);
  for (int i = 0; i < 8; i++)
  {
    Wire.write(STARTUP[0][i]);
    Wire.write(0x00);
  }
  Serial.print("I2C: ");
  Serial.println(Wire.endTransmission());

  i2cWrite(brightness);

  if (restoreConfig())
  {
    if (checkConnection())
    {
      settingMode = false;

      Serial.println("--[BMO STARTUP]--");
      WiFi.persistent(false);
      WiFi.hostname("BMO");
      WiFi.mode(WIFI_STA);

      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());

      checkForUpdates();

      SSDP.setSchemaURL("description.xml");
      SSDP.setHTTPPort(80);
      SSDP.setName("BMO");
      SSDP.setModelNumber(String(FW_VERSION));
      SSDP.setModelName("BMO");
      SSDP.setURL("/");
      SSDP.setUUID("00001::upnp:rootdevice");
      SSDP.setDeviceType("upnp:rootdevice");
      SSDP.begin();

      server.begin();

      os_timer_arm(&blinkTimer, ESP8266TrueRandom.random(blinkLow, blinkHigh), false);
      os_timer_arm(&frameTimer, frameTime, false);
      os_timer_arm(&updateTimer, 1 * 60 * 60 * 1000, true);
      return;
    }
  }
  settingMode = true;
  setupMode();
}

void i2cWrite(byte send)
{
  Wire.beginTransmission(0x70); // transmit to devic
  Wire.write(send);             // sends one byte
  Serial.print("I2C Response: ");
  Serial.println(Wire.endTransmission()); // stop transmitting
}

void loop()
{

  if (!settingMode)
  {
    if (shouldUpdate && !settingMode)
    {
      checkForUpdates();
      shouldUpdate = false;
    }

    if (shouldDraw)
    {
      doLights();
      if(mood == 99 || mood == 98){
        os_timer_disarm(&frameTimer);
        os_timer_arm(&frameTimer, (mood == 99 ? letterTime : scrollTime), false);
      } else {
        os_timer_disarm(&frameTimer);
        os_timer_arm(&frameTimer, frameTime, false);
      }
    }

    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    {                               // If a new client connects,
      Serial.print("New Client: "); // print a message out in the serial port
      Serial.println(client.remoteIP());
      String currentLine = ""; // make a String to hold incoming data from the client
      while (client.connected())
      { // loop while the client's connected
        if (client.available())
        {                         // if there's bytes to read from the client,
          char c = client.read(); // read a byte, then
          //Serial.write(c);                    // print it out the serial monitor
          header += c;
          if (c == '\n')
          { // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0)
            {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              // turns the GPIOs on and off
              if (header.indexOf("GET /face/happy") >= 0)
              {
                Serial.println("Happy");
                mood = 0;
              }
              else if (header.indexOf("GET /face/angry") >= 0)
              {
                Serial.println("Angry");
                mood = 1;
              }
              else if (header.indexOf("GET /face/sad") >= 0)
              {
                Serial.println("Sad");
                mood = 2;
              }
              else if (header.indexOf("GET /face/indifferent") >= 0)
              {
                Serial.println("Indifferent");
                mood = 3;
              }
              else if (header.indexOf("GET /face/shocked") >= 0)
              {
                mood = 4;
              }
              else if (header.indexOf("GET /face/dead") >= 0)
              {
                mood = 5;
              }
              else if (header.indexOf("GET /face/awkward") >= 0)
              {
                mood = 6;
              }
              else if (header.indexOf("GET /face/robot") >= 0)
              {
                mood = 7;
              }
              else if (header.indexOf("GET /face/pleased") >= 0)
              {
                mood = 8;
              }
              else if (header.indexOf("GET /face/scroll") >= 0)
              {
                mood = 98;
              }
              else if (header.indexOf("GET /face/setText?") >= 0)
              {
                uint8_t startIndex = header.indexOf("?inWriting=");
                uint8_t endIndex = header.indexOf("&anim=");
                String text = header.substring(startIndex + 11, endIndex);
                uint8_t lastIndex = 0;
                text.toLowerCase();
                if(header.indexOf("write") > 0){
                  for (int i = 0; i < text.length(); i++)
                  {
                    wordStream[i] = text[i];
                    lastIndex = i;
                  }
                  wordStream[lastIndex + 1] = '?';
                  mood = 99;
                } else {
                  for (int i = 0; i < text.length(); i++)
                  {
                    copyFrameData(i,text[i]);
                  }
                  mood = 98;
                }
              }
              else if (header.indexOf("GET /resetWifi") >= 0)
              {
                for (int i = 0; i < 96; ++i)
                {
                  EEPROM.write(i, 0);
                }
                EEPROM.commit();
                ESP.restart();
              }
              else if (header.indexOf("GET /brightness/up") >= 0)
              {
                brightness += 0x01;
                i2cWrite(brightness);
              }
              else if (header.indexOf("GET /brightness/down") >= 0)
              {
                brightness -= 0x01;
                i2cWrite(brightness);
              }
              else if (header.indexOf("GET /blink/up") >= 0)
              {
                blinkLow = blinkLow + (uint8_t)500;
                blinkHigh = blinkHigh + (uint8_t)500;
              }
              else if (header.indexOf("GET /blink/down") >= 0)
              {
                blinkLow = blinkLow - (uint8_t)500;
                blinkHigh = blinkHigh - (uint8_t)500;
              }
              else if (header.indexOf("GET /power") >= 0)
              {
                displayOn = !displayOn;
              }
              else if (header.indexOf("GET /update/check") >= 0)
              {
                checkForUpdates();
              }
              else if (header.indexOf("GET /update/force") >= 0)
              {
                forceUpdate();
              }
              else if (header.indexOf("GET /frame/up") >= 0)
              {
                frameTime = frameTime + (uint8_t)20;
              }
              else if (header.indexOf("GET /frame/down") >= 0)
              {
                frameTime = frameTime - (uint8_t)20;
              }

              if (!(header.indexOf("GET /description.xml") >= 0))
              {
                // Display the HTML web page
                client.println("<!DOCTYPE html><html>");
                client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
                client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
                client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                client.println(".button2 {background-color: #77878A;}</style></head>");

                if ((header.indexOf("GET /admin") >= 0))
                {
                  // Web Page Heading
                  client.println("<body><h1>Advanced Settings: Ruining things since forever</h1>");

                  client.println("<p>Press buttons for rewards!</p>");
                  client.println("<p><a href=\"/update/check\"><button class=\"button\">Check for updates</button></a></p>");
                  client.println("<p><a href=\"/update/force\"><button class=\"button\">Force update</button></a></p>");
                  client.println("<p><a href=\"/face/scroll\"><button class=\"button\">[Testing] Scroll</button></a></p>");
                  client.println("<p>");
                  client.println(wordStream);
                  client.println("</p>");
                  client.println("</body></html>");
                  client.println("<footer><p>Neil Trotter 2018</p></footer>");
                }
                else
                {
                  // Web Page Heading
                  client.println("<body><h1>BMO Face controller</h1>");

                  client.println("<p><a href=\"/power\"><button class=\"button\">On / Off</button></a></p>");
                  client.println("<h2>Change face</h2>");
                  client.println("<p><a href=\"/face/happy\"><button class=\"button\">Happy</button></a>");
                  client.println("<a href=\"/face/sad\"><button class=\"button\">Sad</button></a>");
                  client.println("<a href=\"/face/angry\"><button class=\"button\">Angry</button></a></p>");
                  client.println("<p><a href=\"/face/indifferent\"><button class=\"button\">Indifferent</button></a>");
                  client.println("<a href=\"/face/shocked\"><button class=\"button\">Shocked</button></a>");
                  client.println("<a href=\"/face/awkward\"><button class=\"button\">Awkward</button></a></p>");
                  client.println("<p><a href=\"/face/dead\"><button class=\"button\">Dead</button></a>");
                  client.println("<a href=\"/face/robot\"><button class=\"button\">Robot</button></a>");
                  client.println("<a href=\"/face/pleased\"><button class=\"button\">Pleased</button></a></p>");

                  client.println("<form action=\"/face/setText\" method=\"get\">");
                  client.println("<input type=\"text\" name=\"inWriting\" value=\"Text\"><br>");
                  client.println("<select name=\"anim\">");
                  client.println("<option value=\"write\">Write</option>");
                  client.println("<option value=\"scroll\">Scroll</option></select>");
                  client.println("<input type=\"submit\" class=\"button\" value=\"Write!\"> </form>");

                  client.println("<h2>Change brightness</h2>");
                  client.println("<p><a href=\"/brightness/up\"><button class=\"button\">Up</button></a>");
                  client.println("<a href=\"/brightness/down\"><button class=\"button\">Down</button></a></p>");
                  client.println("<h2>Change Blink Rate: ");
                  client.print(blinkLow);
                  client.print("ms to ");
                  client.print(blinkHigh);
                  client.print("ms</h2>");
                  client.println("<p><a href=\"/blink/up\"><button class=\"button\">Up</button></a>");
                  client.println("<a href=\"/blink/down\"><button class=\"button\">Down</button></a></p>");

                  client.println("<h2>Change Blink Time: ");
                  client.print(frameTime);
                  client.print("ms</h2>");
                  client.println("<p><a href=\"/frame/up\"><button class=\"button\">Up</button></a>");
                  client.println("<a href=\"/frame/down\"><button class=\"button\">Down</button></a></p>");

                  client.print("<p>Version: ");
                  client.print(String(FW_VERSION));
                  client.println("</p>");
                  client.println("<p><a href=\"/resetWifi\">Reset Wifi settings</a></p>");
                  if (!prod)
                  {
                    client.println("<p><a href=\"/admin\">Advanced settings</a></p>");
                  }
                  client.println("</body></html>");
                  client.println("<footer><p>Neil Trotter 2018</p></footer>");
                }

                // The HTTP response ends with another blank line
                client.println();
                // Break out of the while loop
                break;
              }
              else
              {
                client.println("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
                client.println("<root xmlns=\"urn:schemas-upnp-org:device-1-0\" configId=\"1\">");
                client.println("<specVersion>");
                client.println("<major>1</major>");
                client.println("<minor>1</minor>");
                client.println("</specVersion>");

                client.println("<device>");
                //client.println("<deviceType>urn:dvd604.pw:device:BMO:1</deviceType>");
                client.println("<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>");
                client.println("<friendlyName>BMO</friendlyName>");
                client.println("<manufacturer>Neil Trotter</manufacturer>");
                client.println("<modelName>BMO</modelName>");
                client.println("<UDN>uuid:cc93d8e6-6b8b-4f60-87ca-228c36b5b0e8</UDN>");
                client.println("</device>");
                client.println("</root>");

                // The HTTP response ends with another blank line
                client.println();
                // Break out of the while loop
                break;
              }
            }
            else
            { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          }
          else if (c != '\r')
          {                   // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
        }
      }
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
  else
  {
    dnsServer.processNextRequest();
  }
  webServer.handleClient();
}

void doLights()
{
  if (shouldDraw)
  {
    drawFace();
    shouldDraw = false;
  }
}

int rFrame = 0;
bool lastEnd = false;

void drawFace()
{
  uint8_t curFrame = 0;

  if (shouldBlink)
  {
    curFrame = 1;
    shouldBlink = false;
    os_timer_disarm(&blinkTimer);
    os_timer_arm(&blinkTimer, ESP8266TrueRandom.random(blinkLow, blinkHigh), false);
  }
  else
  {
    curFrame = 0;
  }

  if (displayOn)
  {
    Wire.beginTransmission(0x70);
    Wire.write(0x00);
    switch (mood)
    {
    case 0:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(HAPPY[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 1:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(ANGRY[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 2:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(SAD[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 3:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(INDIFFERENT[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 4:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(SHOCKED[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 5:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(DEAD[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 6:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(AWKWARD[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 7:
    {
      if (rFrame > 5)
      {
        lastEnd = true;
      }
      if (rFrame < 1)
      {
        lastEnd = false;
      }
      rFrame = (lastEnd ? rFrame - 1 : rFrame + 1);
      for (int i = 0; i < 8; i++)
      {
        Wire.write(ROBOT[rFrame][i]);
        Wire.write(0x00);
      }
      break;
    }
    case 8:
    {
      for (int i = 0; i < 8; i++)
      {
        Wire.write(PLEASED[curFrame][i]);
        Wire.write(0x00);
      }
      break;
    }

    case 99:
    {
      if (wordStream[letterIndex] == '?')
      {
        letterIndex = 0;
      }

      for (int i = 0; i < 8; i++)
      {
        Wire.write(reverse(ALPHA[wordStream[letterIndex] - 97][i]));
        Wire.write(0x00);
      }

      letterIndex = letterIndex + 1;
      break;
    }
    case 98:
    {
      getNextFrame();
      for (int i = 0; i < 8; i++)
      {
        Wire.write(curScrollFrame[i]);
        Wire.write(0x00);
      }
      break;
    }
    }
    Wire.endTransmission();
  }
  else
  {
    Wire.beginTransmission(0x70);
    Wire.write(0x00);
    for (int i = 0; i < 16; i++)
    {
      Wire.write(0x00);
    }
    Wire.endTransmission();
  }
}

void forceUpdate()
{
  String fwURL = String(fwUrlBase);
  String fwVersionURL = fwURL;
  fwVersionURL.concat("version");

  Serial.println("Checking for firmware updates.");

  HTTPClient httpClient;
  httpClient.begin(fwVersionURL);
  int httpCode = httpClient.GET();
  if (httpCode == 200)
  {
    String newFWVersion = httpClient.getString();

    Serial.print("Current firmware version: ");
    Serial.println(FW_VERSION);
    Serial.print("Available firmware version: ");
    Serial.println(newFWVersion);
    Serial.println("Preparing to update");

    Wire.beginTransmission(0x70);
    Wire.write(0x00);
    for (int i = 0; i < 8; i++)
    {
      Wire.write(UPDATE[0][i]);
      Wire.write(0x00);
    }
    Wire.endTransmission();

    String fwImageURL = fwURL;
    fwImageURL.concat(newFWVersion);
    fwImageURL.concat(".bin");
    //Serial.println(fwImageURL);
    t_httpUpdate_return ret = ESPhttpUpdate.update(fwImageURL);

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("UPDATE OK");
      break;
    }
  }
  else
  {
    Serial.print("Firmware version check failed, got HTTP response code ");
    Serial.println(httpCode);
  }
  httpClient.end();
}

void checkForUpdates()
{
  String fwURL = String(fwUrlBase);
  String fwVersionURL = fwURL;
  fwVersionURL.concat("version");

  Serial.println("Checking for firmware updates.");

  HTTPClient httpClient;
  httpClient.begin(fwVersionURL);
  int httpCode = httpClient.GET();
  if (httpCode == 200)
  {
    String newFWVersion = httpClient.getString();

    Serial.print("Current firmware version: ");
    Serial.println(FW_VERSION);
    Serial.print("Available firmware version: ");
    Serial.println(newFWVersion);

    int newVersion = newFWVersion.toInt();

    if (newVersion > FW_VERSION)
    {
      Serial.println("Preparing to update");

      Wire.beginTransmission(0x70);
      Wire.write(0x00);
      for (int i = 0; i < 8; i++)
      {
        Wire.write(UPDATE[0][i]);
        Wire.write(0x00);
      }
      Wire.endTransmission();

      String fwImageURL = fwURL;
      fwImageURL.concat(newFWVersion);
      fwImageURL.concat(".bin");
      //Serial.println(fwImageURL);
      t_httpUpdate_return ret = ESPhttpUpdate.update(fwImageURL);

      switch (ret)
      {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("UPDATE OK");
        break;
      }
    }
    else
    {
      Serial.println("Already on latest version");
    }
  }
  else
  {
    Serial.print("Firmware version check failed, got HTTP response code ");
    Serial.println(httpCode);
  }
  httpClient.end();
}

boolean checkConnection()
{
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  while (count < 90)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

void startWebServer()
{
  if (settingMode)
  {
    Serial.print("Starting Web Server at ");
    Serial.println(WiFi.softAPIP());
    webServer.on("/settings", []() {
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the network.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    webServer.on("/setap", []() {
      for (int i = 0; i < 96; ++i)
      {
        EEPROM.write(i, 0);
      }
      String ssid = urlDecode(webServer.arg("ssid"));
      Serial.print("SSID: ");
      Serial.println(ssid);
      String pass = urlDecode(webServer.arg("pass"));
      Serial.print("Password: ");
      Serial.println(pass);
      Serial.println("Writing SSID to EEPROM...");
      for (uint8_t i = 0; i < ssid.length(); ++i)
      {
        EEPROM.write(i, ssid[i]);
      }
      Serial.println("Writing Password to EEPROM...");
      for (uint8_t i = 0; i < pass.length(); ++i)
      {
        EEPROM.write(32 + i, pass[i]);
      }
      EEPROM.commit();
      Serial.println("Write EEPROM done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      ESP.restart();
    });
    webServer.onNotFound([]() {
      String s = "<h1>BMO WiFi Setup</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("BMO", s));
    });
  }
  webServer.begin();
}

void setupMode()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial.println("");
  for (int i = 0; i < n; ++i)
  {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID);
  dnsServer.start(53, "*", apIP);
  startWebServer();
  Serial.print("Starting Access Point at \"");
  Serial.print(apSSID);
  Serial.println("\"");
}

String makePage(String title, String contents)
{
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  s += " <footer><p>Neil Trotter 2018</p></footer>";
  return s;
}

String urlDecode(String input)
{
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}

boolean restoreConfig()
{
  Serial.println("Reading EEPROM...");
  String ssid = "";
  String pass = "";
  if (EEPROM.read(0) != 0)
  {
    for (int i = 0; i < 32; ++i)
    {
      ssid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(ssid);
    for (int i = 32; i < 96; ++i)
    {
      pass += char(EEPROM.read(i));
    }
    Serial.print("Password: ");
    Serial.println(pass);
    WiFi.begin(ssid.c_str(), pass.c_str());
    return true;
  }
  else
  {
    Serial.println("Config not found.");
    return false;
  }
}

unsigned char reverse(unsigned char b)
{
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

void copyFrameData(uint8_t number, char frame)
{
  for (int i = 0; i < 8; i++)
  {
    textFrames[number][i] = ALPHA[frame - 97][i];
  }
}

void getNextFrame()
{
  for (int i = 0; i < 8; i++)
  {
    curScrollFrame[i] = reverse(textFrames[frameScrollOffset][i]);
    curScrollFrame[i] = curScrollFrame[i] >> frameScrollCount;
    if(frameScrollCount > 2){
      curScrollFrame[i] = reverse(textFrames[frameScrollOffset][i]) >> frameScrollCount | reverse(textFrames[frameScrollOffset+1][i] >> (8-frameScrollCount));
    }
  }
  frameScrollCount = (frameScrollCount + 1) % 8;
  frameScrollTotal++;
  frameScrollOffset = frameScrollTotal / 8;
  if(textFrames[frameScrollOffset][4]==0x00 && textFrames[frameScrollOffset+1][4]==0x00 && textFrames[frameScrollOffset+2][4]==0x00){
    frameScrollOffset = 0;
    frameScrollCount = 0;
    frameScrollTotal = 0;
  }
}
