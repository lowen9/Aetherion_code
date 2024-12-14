#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#include <Adafruit_MPL3115A2.h>

// #include "BNO055_support.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Wire.h>

#include <SPI.h>
#include <LoRa.h>

#define SDA_PIN 38
#define SCL_PIN 39

#define GPS_RX_PIN 44
#define GPS_TX_PIN 43

uint8_t BUG_GPS = 0;
uint8_t BUG_LoRa = 0;
uint8_t BUG_BNO = 0;

SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN); //Rx, Tx serial for GPS

TinyGPSPlus gps; //The TinyGPSPlus object
uint8_t gps_status = 0;
unsigned int gps_number = 0;
float latitude = 0.001;
float longitude = 0.001;
float alt = 0.01;
float speed = 0.01;
float hdop = 0.0;

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
imu::Vector<3> acc;
imu::Vector<3> euler;

// struct bno055_t myBNO;
// struct bno055_accel acc_BNO;
// struct bno055_gyro  gyro_BNO;
// struct bno055_linear_accel lacc_BNO;

// float g_x = 0;
// float g_y = 0;
// float g_z = 0;
// float a_x = 0;
// float a_y = 0;
// float a_z = 0;

Adafruit_MPL3115A2 baro;
float baseAltitude = 0.0;
float altitude = 0.0;
float pressure = 0.0;
float temperature = 0.0;

void init_I2C(){
  Serial.println("init I2C");
  Wire.setPins(SDA_PIN,SCL_PIN);
  Wire.setClock(400000UL);
  Wire.begin();
  delay(10);
}

void setup()
{
  Serial.begin(115200);   //must match the Serial monitor
  delay(1000);

  init_I2C();

  init_adaBNO();

  init_lora();

  // init_altimetre();

  delay(1000);

  init_GPS();
  fed_GPS();
}

void loop()
{
  read_GPS();
  fed_GPS();
  read_adaBNO();
  // read_altimetre();
  fed_GPS();
  if(!BUG_GPS && !BUG_LoRa && !BUG_BNO){
    lora_data();
    // print_data();
  }
  if(BUG_GPS){
    Serial.println("BUUUUUUG GPS");
  }
  if(BUG_LoRa){
    Serial.println("BUUUUUG Lora");
  }
  if(BUG_BNO){
    Serial.println("BUUUUUG BNO");
  }
}