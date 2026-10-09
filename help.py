#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "He";
const char* password = "testingz";

const char* api_url  = "http://ip-api.com/json/?fields=61439";

float latitude  = 0.0;
float longitude = 0.0;
String city     = "";

bool fetchLocation() { 
  if (WiFi.status() != WL_CONNECTED) { 
    Serial.println("WiFi not connected, fuck"); 
    return false; 
  } else { 
    Serial.println("WiFi connected, fuck"); 
  } 

  HTTPClient http; 
  http.begin(api_url); 
  
  http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS); 
  
  int httpResponseCode = http.GET(); 
  
  if (httpResponseCode > 0) { 
    Serial.print("HTTP Response code: "); 
    Serial.println(httpResponseCode); 
    JsonDocument doc; 
    DeserializationError error = deserializeJson(doc, http.getStream()); 
    
    if (error) { 
      Serial.print("JSON parsing failed: "); 
      Serial.println(error.c_str()); 
      http.end(); 
      return false; 
    } 
    
    latitude = doc["lat"]; 
    longitude = doc["lon"]; 
    city = doc["city"].as<String>(); 
    
    http.end(); 
    return true; 
  } else { 
    Serial.print("Error code on HTTP request: "); 
    Serial.println(httpResponseCode); 
    http.end(); 
    return false; 
  } 
}




void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to network!");

  if (fetchLocation()) {
    Serial.println("--- Location Data Stored Successfully ---");
    Serial.print("Stored Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Stored Longitude: ");
    Serial.println(longitude, 6);
  } else {
    Serial.println("Failed to update coordinates.");
  }
}









void loop() {
    
}