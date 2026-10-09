#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_MPU6050.h>

// Instantiate Two Independent I2C Bus Objects
TwoWire I2C_Compass = TwoWire(0); // Uses Hardware I2C 0
TwoWire I2C_Gyro    = TwoWire(1); // Uses Hardware I2C 1

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
  Serial.println("Dual-Bus I2C Initialization...");
  
  // 1. Initialize Compass I2C Bus (SDA=5, SCL=4)
  I2C_Compass.begin(5, 4, 100000); // 100kHz clock speed
  
  // 2. Initialize GY-521 I2C Bus (SDA=19, SCL=18)
  I2C_Gyro.begin(7, 10, 400000);   // 400kHz clock speed for faster gyro data

  /* Initialize XC4496 Magnetometer on its custom bus */
  if(!mag.begin(&I2C_Compass)) {
    Serial.println("Error: XC4496 (HMC5883L) not detected on SDA:5 SCL:4!");
    while(1);
  }

  /* Initialize GY-521 (MPU6050) Accelerometer on its custom bus */
  if(!mpu.begin(0x68, &I2C_Gyro)) { // 0x68 is the standard I2C address for GY-521
    Serial.println("Error: GY-521 (MPU6050) not detected on SDA:19 SCL:18!");
    while(1);
  }

  // Configure MPU6050 setting profiles
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  Serial.println("Both sensors successfully operational on distinct pins!");
}

void loop(void) {
  sensors_event_t magEvent;
  sensors_event_t a, g, temp;
  
  // Fetch data safely from separate hardware pipelines
  mag.getEvent(&magEvent);
  mpu.getEvent(&a, &g, &temp);

  // 1. Calculate Pitch from GY-521 (On Pins 19/18)
  float pitchRadians = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z));
  float pitchDegrees = pitchRadians * 180.0 / M_PI;

  // 2. Calculate Heading and Bearing from XC4496 (On Pins 5/4)
  float heading = atan2(magEvent.magnetic.y, magEvent.magnetic.x);
  
  if(heading < 0) heading += 2 * PI;
  if(heading > 2 * PI) heading -= 2 * PI;
   
  float headingDegrees = heading * 180 / M_PI; 
  String bearing = getBearingString(headingDegrees);

  // Clean layout print
  Serial.print("Pitch: "); 
  Serial.print(pitchDegrees, 1);
  Serial.print("° | Heading: "); 
  Serial.print(headingDegrees, 1); 
  Serial.print("° ("); 
  Serial.print(bearing);
  Serial.println(")");
  
  delay(200); 
}