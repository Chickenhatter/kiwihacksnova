#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "MIT-Guest";
const char* password = "kiwihack";

const char* api_url  = "http://ip-api.com";

float latitude  = 0.0;
float longitude = 0.0;
String city     = "";

bool fetchLocation() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, fuck");
    return false;
  }

  HTTPClient http;
  http.begin(api_url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    String payload = http.getString();
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.cwise());
      http.end();
      return false;
    }
    latitude  = doc["lat"];
    longitude = doc["lon"];
    city      = doc["city"].as<String>();

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