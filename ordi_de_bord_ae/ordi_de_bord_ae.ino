#include <TinyGPSPlus.h>
// #include <SoftwareSerial.h>

#include <libaltimetreV2.h>  //bibliothèque MPL3115A2 custom

// #include "BNO055_support.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Wire.h>

#include <SPI.h>
#include <SX127x.h>

#include <FS.h>
#include <SD.h>

#define LEDR_PIN 42 
#define LEDJ_PIN 46
#define LEDV_PIN 45

#define JACK_PIN 40
#define JACK_OUT 41

#define SDA_PIN 38
#define SCL_PIN 39

#define GPS_RX_PIN 44
#define GPS_TX_PIN 43

#define LOG_TX_PIN 47

char rocket_status[20];

HardwareSerial gpsSerial(0); //UART 0 for gps

HardwareSerial logSerial(2); //UART 2 for logger

TinyGPSPlus gps; //The TinyGPSPlus object
uint8_t gps_status = 0;
unsigned int gps_number = 0;
float latitude = 0.001;
float longitude = 0.001;
float speed = 0.01;
float hdop = 0.0;

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
imu::Vector<3> acc;
imu::Vector<3> euler;

float baseAltitude = 0.0;
float alt = 0.0;
float temperature = 0.0;

static File log_file;

SX127x LoRa;

char msgCheck[200];

void init_I2C(){
  Serial.println("init I2C");
  Wire.setPins(SDA_PIN,SCL_PIN);
  Wire.setClock(400000UL);
  Wire.begin();
  delay(10);
}

void setup()
{
  Serial.begin(115200);   //For debugging
  delay(1000);

  init_led();

  init_jack();

  led_update(LEDV_PIN, 1);

  init_I2C();

  init_adaBNO();

  init_log();

  init_lora();

  sprintf(msgCheck, "%.6f,%.6f,%.2f,%.2f,%.1f,%.1f", 0, 0, 0, 0, 0, 0);

  while(digitalRead(JACK_PIN)){
    LoRa.beginPacket();
    LoRa.write(strlen(msgCheck));
    LoRa.write(0);
    LoRa.write(msgCheck,strlen(msgCheck));
    LoRa.endPacket();
    delay(80);
  }

  led_update(LEDJ_PIN, 1);

  init_altimetre();

  led_update(LEDR_PIN, 1);
  delay(1000);

  init_GPS();
  fed_GPS();

  led_update(LEDV_PIN, 0);
  delay(200);
  led_update(LEDJ_PIN, 0);
  delay(200);
  led_update(LEDR_PIN, 0);
}

void loop()
{
  read_GPS();
  check_Jack();
  fed_GPS();
  read_adaBNO();
  read_altimetre();
  fed_GPS();

  lora_data();
}