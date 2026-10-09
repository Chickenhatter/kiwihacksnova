#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <cmath>

const char* ssid     = "He";
const char* password = "testingz";

const char* api_url  = "http://ip-api.com/json/?fields=61439";
const char* iss_url = "http://api.open-notify.org/iss-now.json";

float bearing = 0.0;
float latitude  = 0.0;
float longitude = 0.0;

float isslatitude  = 0.0;
float isslongitude = 0.0;


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

bool fetchISS() { 
  if (WiFi.status() != WL_CONNECTED) { 
    Serial.println("WiFi not connected, fuck"); 
    return false; 
  } else { 
    Serial.println("WiFi connected, fuck"); 
  } 

  HTTPClient http; 
  http.begin(iss_url); 
  
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
    
    isslongitude = doc["iss_position"]["longitude"].as<float>();
    isslatitude = doc["iss_position"]["latitude"].as<float>();
    
    http.end(); 
    return true; 
  } else { 
    Serial.print("Error code on HTTP request: "); 
    Serial.println(httpResponseCode); 
    http.end(); 
    return false; 
  } 
}







double to_radians(double degrees) {
    return degrees * PI / 180.0;
}

double to_degrees(double radians) {
    return radians * 180.0 / PI;
}
double calculate_bearing(double lat1, double lon1, double lat2, double lon2) {
    double lat1_rad = to_radians(lat1);
    double lat2_rad = to_radians(lat2);
    double diff_lon_rad = to_radians(lon2 - lon1);
    double x = std::sin(diff_lon_rad) * std::cos(lat2_rad);
    double y = std::cos(lat1_rad) * std::sin(lat2_rad) - 
               (std::sin(lat1_rad) * std::cos(lat2_rad) * std::cos(diff_lon_rad));
    double initial_bearing_rad = std::atan2(x, y);
    double initial_bearing_deg = to_degrees(initial_bearing_rad);
    double compass_bearing = std::fmod(initial_bearing_deg + 360.0, 360.0);
    return compass_bearing;
}



const double earthrkm = 6371.0;

double calculateDistanceArcRad(double lat1, double lon1, double lat2, double lon2) {
    return (calculateHaversineDistance(lat1, lon1, lat2, lon2) / earthrkm);
}

double angtwoside(double c) {
  float d = sqrt((earthrkm+400)*(earthrkm+400)+earthrkm*earthrkm-2*earthrkm*(earthrkm+400)*cos(c));
  float e = std::asin((sin(c)*(earthrkm+400))/d);
  Serial.print("a");
  Serial.print(e);
  Serial.print("a");
  return e;
}



double calculateHaversineDistance(double lat1, double lon1, double lat2, double lon2) {
    lat1 = to_radians(lat1);
    lon1 = to_radians(lon1);
    lat2 = to_radians(lat2);
    lon2 = to_radians(lon2);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon / 2.0) * std::sin(dLon / 2.0);
               
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    return earthrkm * c;
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
  if (fetchISS()) {
    Serial.println("--- Location Data Stored Successfully ---");
    Serial.print("Stored Latitude: ");
    Serial.println(isslatitude, 6);
    Serial.print("Stored Longitude: ");
    Serial.println(isslongitude, 6);
  } else {
    Serial.println("Failed to update coordinates.");
  }

  bearing = calculate_bearing(latitude, longitude, isslatitude, isslongitude);
  Serial.print(bearing, 6);
  float angle = angtwoside(calculateDistanceArcRad(latitude, longitude, isslatitude, isslongitude));
  Serial.print("ahhhh: ");
  Serial.print(to_degrees(angle)-90);
}

void loop() {
    
}