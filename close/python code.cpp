#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_MPU6050.h>

/* Create sensor objects */
Adafruit_HMC5883_Unified mag  = Adafruit_HMC5883_Unified(12345);
Adafruit_MPU6050         mpu;

// Function to convert degrees to a compass bearing string
String getBearingString(float degrees) {
  const char* directions[] = {
    "N", "NNE", "NE", "ENE", 
    "E", "ESE", "SE", "SSE", 
    "S", "SSW", "SW", "WSW", 
    "W", "WNW", "NW", "NNW"
  };
  int index = int((degrees + 11.25) / 22.5);
  return directions[index % 16];
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("GY-521 Pitch + XC4496 Compass Test");
  
  // Initialize custom I2C lines: SDA = GPIO 5, SCL = GPIO 4
  Wire.begin(5, 4);

  /* Initialize XC4496 Magnetometer */
  if(!mag.begin()) {
    Serial.println("Error: XC4496 (HMC5883L) not detected! Check wiring.");
    while(1);
  }

  /* Initialize GY-521 (MPU6050) Accelerometer */
  if(!mpu.begin()) {
    Serial.println("Error: GY-521 (MPU6050) not detected! Check wiring.");
    while(1);
  }

  // Configure MPU6050 accelerometer settings for stable readings
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop(void) {
  sensors_event_t magEvent;
  sensors_event_t a, g, temp;
  
  // Fetch data from both sensors over I2C
  mag.getEvent(&magEvent);
  mpu.getEvent(&a, &g, &temp);

  // 1. Calculate True Pitch from GY-521 Accelerometer (Gravity Vector)
  // atan2 uses the X, Y, and Z acceleration to find the exact angle from flat
  float pitchRadians = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z));
  float pitchDegrees = pitchRadians * 180.0 / M_PI;

  // 2. Calculate Heading and Bearing from Compass
  float heading = atan2(magEvent.magnetic.y, magEvent.magnetic.x);
  
  if(heading < 0) heading += 2 * PI;
  if(heading > 2 * PI) heading -= 2 * PI;
   
  float headingDegrees = heading * 180 / M_PI; 
  String bearing = getBearingString(headingDegrees);

  // Print outputs clearly
  Serial.print("Pitch: "); 
  Serial.print(pitchDegrees, 1);
  Serial.print("° | Heading: "); 
  Serial.print(headingDegrees, 1); 
  Serial.print("° ("); 
  Serial.print(bearing);
  Serial.println(")");
  
  delay(200); // Updated to 5Hz refresh rate for smoother tracking
}