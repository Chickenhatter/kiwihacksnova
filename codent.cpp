#include <WiFi.h>

const char* ssid = "ESP32-Hotspot";       
const char* password = "password123";  
  
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting up Access Point...");

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void loop() {
}