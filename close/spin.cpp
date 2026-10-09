#include <Arduino.h> 
#include <Arduino_GFX_Library.h>  

#define COLOR_BLACK   0x0000 
#define COLOR_GREEN   0x07E0  

#define PIN_DC   2 
#define PIN_CS   10 
#define PIN_SCLK 6 
#define PIN_MOSI 7 
#define PIN_BL   3  

Arduino_DataBus *bus = new Arduino_ESP32SPI(PIN_DC, PIN_CS, PIN_SCLK, PIN_MOSI, GFX_NOT_DEFINED, FSPI); 
Arduino_GFX *output_display = new Arduino_GC9A01(bus, -1, 0, true);  

Arduino_Canvas *canvas = new Arduino_Canvas(240 , 240 , output_display);  

const float centerX = 120.0f; 
const float centerY = 120.0f; 

const float pointerLength = 100.0f;
const float baseWidth     = 25.0f; 

float bearing = 0.0f; 

void setup() {   
  output_display->begin();    
  canvas->begin();     
  pinMode(PIN_BL, OUTPUT);   
  digitalWrite(PIN_BL, HIGH);    

  canvas->fillScreen(COLOR_BLACK); 
}  

void loop() {   
  canvas->fillScreen(COLOR_BLACK);    
  // bearing += 2.0f; 
  // if (bearing >= 360.0f) {
  //   bearing -= 360.0f;
  // }
  float angleRad = (bearing * DEG_TO_RAD) - M_PI_2;
  int x1 = (int)(centerX + pointerLength * cos(angleRad));
  int y1 = (int)(centerY + pointerLength * sin(angleRad));
  float baseAngleRad = angleRad + M_PI_2;
  int x2 = (int)(centerX - baseWidth * cos(baseAngleRad));
  int y2 = (int)(centerY - baseWidth * sin(baseAngleRad));
  int x3 = (int)(centerX + baseWidth * cos(baseAngleRad));
  int y3 = (int)(centerY + baseWidth * sin(baseAngleRad));
  canvas->fillTriangle(x1, y1, x2, y2, x3, y3, COLOR_GREEN); 
  canvas->flush();   
  delay(16);
}