/**
 * Simple serwer to cntroll LED
 *
 * @ Modified by: ArchiiLewiss
 * @ Modified time: 2020-11-09 17:46:29
 */
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <AsyncJson.h>
#include <SPIFFS.h>

#include <FastLED.h>

#define WIFI_NETWORK "Netcomic-10"
#define WIFI_PASSWORD "agusiaarchii1"
#define WIFI_TIMEOUT_MS 20000

#define PIN 16                     // Digital output to LED

// Set up Variables

uint8_t NUM_STRIPS = 1;          // number of strips/steps
int PIXELS_PER_STRIP = 34;        // number of pixels in each strip
uint8_t MAX_BRIGHTNESS = 100;     // LED brightness (0 - 255)
int TOP_SENSOR_PIN = 18;          // PIR at the top of the stairs
int BOTTOM_SENSOR_PIN = 19;       // PIR at the bottom of the stairs

unsigned long timeOut=600000;     // timestamp to remember when the PIR was triggered.
unsigned long delayWalk=3000;     // delay after pasinng second sensor

int downUp = 0;                   // variable to rememer the direction of travel up or down the stairs
int alarmValueTop = LOW;          // Variable to hold the PIR status
int alarmValueBottom = LOW;       // Variable to hold the PIR status

// total number of pixels
static int NUM_LEDS = NUM_STRIPS * PIXELS_PER_STRIP;
CRGB leds[34] = {0};

AsyncWebServer server(80);

void connectToWiFi() {
  Serial.println("Łączę z WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS)
  {
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Błąd połączenia!!!");
  } else {
    Serial.print("Połączono: ");
    Serial.println(WiFi.localIP());
  }
}

// 0 to 255
void brighten(uint16_t ledNum, uint16_t wait) {
  uint16_t i, count;
  count = (ledNum + PIXELS_PER_STRIP);
/*   for (c = 0; c < MAX_BRIGHTNESS; c+=20) {
    int j=1; */

    int j=1;    
    int l = 0; //light
    int half = (PIXELS_PER_STRIP/2);
    //uint8_t c = MAX_BRIGHTNESS;
    
    for (i = ledNum; i < count-half; i++) {

      l++;

      if ((count/PIXELS_PER_STRIP % 2) == 0) {
        /* leds[i].setRGB(c/half*l, c, c);
        leds[count-j].setRGB(c/half*l, c, c); */
        /*    if (i < NUM_LEDS-18 && l<6) {
          leds[i+PIXELS_PER_STRIP].setRGB(c/half*l, c, c);
        } */
      } else {
        /* leds[i].setRGB(c/half*l, c, c);
        if (i < NUM_LEDS-18 && l<6) {
          leds[i+PIXELS_PER_STRIP].setRGB(c/half*l, c, c);
        }
        
        leds[count-j].setRGB(c/half*l, c, c); */


      }
           // leds[count-j-a].setRGB(c+l*a, c+l*a, c+l*a);

           // leds[i+a+PIXELS_PER_STRIP].setRGB(c+l*a, c+l*a, c+l*a);
    
      /* if ((count/PIXELS_PER_STRIP % 2) == 0) {
        for (int a = 0; a < 5; a++)
        {        
            leds[i+a].setRGB(c+l*a, c+l*a, c+l*a);
            leds[count-j-a].setRGB(c+l*a, c+l*a, c+l*a);

            leds[i+a+PIXELS_PER_STRIP].setRGB(c+l*a, c+l*a, c+l*a);
        }
      } else {
        for (int a = 0; a < 5; a++)
        {        
            leds[i+a].setRGB(c+l*a, c+l*a, c+l*a);
            leds[count-j-a].setRGB(c+l*a, c+l*a, c+l*a);  
        }

      } */
      
      


     /*  if ( (count/PIXELS_PER_STRIP > 1) && (count/PIXELS_PER_STRIP < 18) ) {
        for (int a = 0; a < 5; a++)
        {
          leds[i+a-PIXELS_PER_STRIP].setRGB(40, 0, 40);
          leds[count-j-a-PIXELS_PER_STRIP].setRGB(40, 0, 40);
        }
      } 

      if ( (count/PIXELS_PER_STRIP > 2) && (count/PIXELS_PER_STRIP < 18) ) {
        for (int a = 0; a < 5; a++)
        {
          leds[i+a-(PIXELS_PER_STRIP*2)].setRGB(40, 100, 40);
          leds[count-j-a-(PIXELS_PER_STRIP*2)].setRGB(40, 100, 40);
        }
      }  */

      //c = c+2;
      j++;
       
      
    }

    fill_gradient(leds, 0, CHSV(100,0,100), 16, CHSV(100,0,0) );
    FastLED.show();


}

void brightenDOWN(){
  uint16_t a;
  
  for (a = 0; a < NUM_STRIPS; a++) {
    int start = PIXELS_PER_STRIP * a;
    brighten(start, 10);
    //fadeToBlackBy( leds, NUM_LEDS, 20);
       
  }
}

void brightenUP(){
  uint16_t a;
  
  for (a = NUM_STRIPS; a > 0; a--) {
    int start = PIXELS_PER_STRIP * (a-1);
    brighten(start, 10);
    //fadeToBlackBy( leds, NUM_LEDS, 20);
     FastLED.show();  
  }
}


void topdown() {
  Serial.println("LED z góry do dołu");      
  brightenDOWN();
}


void bottomup() {
  Serial.println("LED z dołu do góry");  
  brightenUP();
}




// 255 to 0
void darken(uint16_t ledNum, uint16_t wait) {
  uint16_t c, i, count;
  count = (ledNum + PIXELS_PER_STRIP);

  for (c = MAX_BRIGHTNESS; c > 0; c=c-50) {
    int j = 1;
    leds->fadeToBlackBy(25);
    for (i = ledNum; i < count-(PIXELS_PER_STRIP/2); i++) {
      
      leds[i].setRGB(c, c, c);
      leds[count-j].setRGB(c, c, c);
      j++;
     
      //delay(4);
      
      //c = 98;
    }
    //c--;
    FastLED.show();
    delay(100);
    //delay(wait);
    //c = c-5;
    downUp = 0;
  }

  // trun off the strip completely
  for (i = ledNum; i < (ledNum + PIXELS_PER_STRIP); i++) {
    leds[i].setRGB(0,0,0); 
  }
  FastLED.show();
}


void darkenDOWN(){
  uint16_t b;
  
  for (b = 0; b < NUM_STRIPS; b++) {
    int start = PIXELS_PER_STRIP * b;
    darken(start, 1);
  }
}

void darkenUP(){
  uint16_t b;
  
  for (b = NUM_STRIPS; b > 0; b--) {
    int start = PIXELS_PER_STRIP * (b-1);
    darken(start, 1);
  }
}

void setup()
{
  Serial.begin(115200);
  connectToWiFi();

  if(!SPIFFS.begin()){ 
    Serial.println("An Error has occurred while mounting SPIFFS");  
  }

  MDNS.begin("schody");

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  server.addHandler(new AsyncCallbackJsonWebHandler("/led", [](AsyncWebServerRequest *request, JsonVariant &json) {
    const JsonObject &jsonObj = json.as<JsonObject>();
    if (jsonObj["on"] && downUp == 0)
    {
      timeOut=millis();
      Serial.println("Turn on LED");
      downUp = 1;
      topdown();
    }
    else
    {
      timeOut=millis();
      Serial.println("Turn off LED");
     // delay(delayWalk);
      darkenUP();
      downUp = 0;
      jsonObj["on"] = false;
    }
    request->send(200, "OK");
  }));

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS)
    {
      request->send(200);
    }
    else
    {
      Serial.println("Not found");
      request->send(404, "Not found");
    }
  });

  server.begin();

  FastLED.addLeds<WS2812B,PIN,GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();

}

void loop()
{
   // Constantly poll the PIR at the top of the stairs
  alarmValueTop = digitalRead(TOP_SENSOR_PIN);

  // Constantly poll the PIR at the bottom of the stairs
  alarmValueBottom = digitalRead(BOTTOM_SENSOR_PIN);

  if (alarmValueTop == HIGH)  {
    timeOut=millis();  // Timestamp when the PIR is triggered.  The LED cycle wil then start.

    if (downUp == 0) {
      downUp = 1;
      topdown();         // lights up the strip from top down
    } else if(downUp == 2) {
      delay(delayWalk);
      darkenUP();
      downUp = 0;
    }
  }
  
  if (alarmValueBottom == HIGH)  {

    timeOut=millis();    // Timestamp when the PIR is triggered.  The LED cycle wil then start.

    if (downUp == 0) {
      downUp = 2;
      bottomup();
    } else if(downUp == 1) {
      delay(delayWalk);
      darkenDOWN();
      downUp = 0;
    }   
  }

  delay(100);

  if (timeOut+20000 < millis() && timeOut+30000 < millis() && alarmValueTop != HIGH && alarmValueBottom != HIGH) {    //switch off LED's in the direction of travel.
     if (downUp == 1) {
        darkenDOWN(); // Off
     }
     if (downUp == 2)  {
      darkenUP();   // Off
     }
    downUp = 0;
  }
}