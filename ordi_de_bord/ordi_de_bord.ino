#include "TinyGPSPlus.h"
#include "SoftwareSerial.h"

#include "BNO055_support.h"
#include "Wire.h"

#include <SPI.h>
#include <LoRa.h>

#define SDA_PIN 38
#define SCL_PIN 39

uint8_t BUG = 1;

uint8_t gps_status = 0;
float latitude = -1;
float longitude = -1;
float alt = -1;
float speed = -1;

void init_I2C(){
  Wire.setPins(SDA_PIN,SCL_PIN);
  Wire.begin();
  delay(10);
}

struct bno055_t myBNO;
struct bno055_accel acc_BNO;
struct bno055_gyro  gyro_BNO;
struct bno055_linear_accel lacc_BNO;
short int off_ax;
short int off_ay;
short int off_az;

float g_x = 0;
float g_y = 0;
float g_z = 0;
float a_x = 0;
float a_y = 0;
float a_z = 0;

unsigned long lastTime_printd = 0;
void print_data(){
  if ((millis() - lastTime_printd) >= 100) //To stream at 10Hz without using additional timers
  {
    lastTime_printd = millis();
    // Serial.print("Gps status:");
    Serial.print("/*");
    Serial.print(gps_status);
    Serial.print(",");
    // Serial.print("latitute:");
    Serial.print(latitude,6);
    Serial.print(",");
    // Serial.print("longitude:");
    Serial.print(longitude,6);
    Serial.print(",");
    // Serial.print("altitude:");
    Serial.print(alt);
    Serial.print(",");
    // Serial.print("speed:");
    Serial.print(speed);
    Serial.print(",");

    // Serial.print("gx:");
    Serial.print(g_x);
    Serial.print(",");
    // Serial.print("gy:");
    Serial.print(g_y);
    Serial.print(",");
    // Serial.print("gz:");
    Serial.print(g_z);
    Serial.print(",");

    // Serial.print("ax:");
    Serial.print(a_x);
    Serial.print(",");
    // Serial.print("ay:");
    Serial.print(a_y);
    Serial.print(",");
    // Serial.print("az:");
    Serial.print(a_z);

    Serial.print("\n");
    
  }
}

void setup()
{
  Serial.begin(115200);   //must match the Serial monitor
  delay(100);

  init_GPS();

  init_I2C();
  init_BNO();

  init_lora();
}

void loop()
{
  read_GPS();
  read_BNO();
  if(!BUG){
    lora_data();
    // print_data();
  }
}